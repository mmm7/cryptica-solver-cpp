#include <cstdlib>

extern "C" {
#include "getcputime.h"
}
#include <assert.h>
#include <iostream>
#include <set>
#include <vector>

#include "solve.h"

#define STATS true
#define DEBUG false

class Voidify {
 public:
  Voidify() { }
  // This has to be an operator with a precedence lower than << but
  // higher than ?:
  void operator&(std::ostream&) {}
};
#define LOG(x) !DEBUG ? (void)0 : Voidify() & std::cout

using namespace std;

typedef std::map<long long, int> VisitedMap;
//typedef boost::asio::detail::hash_map<long long, int> VisitedMap;

// Returns whether the puzzle can be solved. Sets the number of moves and the
// direction of the moves in the output args.
SolveResult solve(const Board& board, const State& start_state) {
  int ss = 0;  // state_size. How many states we store in memory.
  SolveResult res;
  if (board.IsGoal(start_state)) {
    res.end_state = start_state;
    res.success = true;
    return res;
  }

  // state hash -> min_moves
  VisitedMap visited_states;

  // Indexed by min_moves. min_moves is: Lower bound on the number of moves
  // needed to solve the puzzle through any states in group. The lower bound is
  // the sum of the number of moves needed from start state to current state +
  // the lower bound on the number of moves needed from the current state to
  // reach the end state. All states in the group have the same lower
  // bound of min_moves.
  vector<StateGroup> state_groups;
  int min_moves = board.MinMovesFrom(start_state);
  LOG(1) << "min:" << min_moves << " "
         << "num states: " << visited_states.size() << endl;
  state_groups.resize(MAX_MOVES);
  state_groups[min_moves].AddState(new State(start_state));
  if (STATS) { ++ss; if (ss > res.max_mem_state) res.max_mem_state = ss; }
  while(true) {
    while (state_groups[min_moves].Empty()) {
      ++min_moves;
      LOG(2) << "min:" << min_moves << " " << "num states: "
             << visited_states.size() << endl;
      if (min_moves >= MAX_MOVES) {
        LOG(2) << "num states: " << visited_states.size() << endl;
        return res;
      }
    }
    // Handle next state in group.
    const State* curr_state = state_groups[min_moves].Pop();
    if (STATS) { --ss; if (ss > res.max_mem_state) res.max_mem_state = ss; }
    // We know that this state is not the goal, no need to check here.
    //const long long curr_hash = curr_state->Hash();
    const int history_len = curr_state->GetHistoryLen();
    for (int di = 0; di < 4; ++di) {
      // Put it on the heap.
      State* new_state = new State();
      curr_state->Move(board, di, new_state);
      if (STATS) ++res.num_moves;
      const long long new_hash = new_state->Hash();
      /* Optimization. Makes it slower.
      if (new_hash == curr_hash) {
        delete new_state;
        continue;
      }
      */
      // Check if new state is the goal.
      if (board.IsGoal(*new_state)) {
        res.success = true;
        res.end_state = *new_state;
        delete new_state;
        delete curr_state;
        LOG(1) << "num states: " << visited_states.size() << endl;
        return res;
      }
      // Compute min_moves
      int new_min_moves =
        history_len + 1 + board.MinMovesFrom(*new_state);
      //assert(new_min_moves >= min_moves);
      // Check if the new state has already been seen.
      VisitedMap::iterator it = visited_states.find(new_hash);
      if (it != visited_states.end()) {
        // state already visited
        int prev_min_moves = it->second;
        if (prev_min_moves > new_min_moves) {
          // found state is in a bigger group, remove from that group.
          state_groups[prev_min_moves].RemoveState(new_hash);
          if (STATS) ++res.num_visited_hit_improve;
          if (STATS) { --ss; if (ss > res.max_mem_state) res.max_mem_state = ss; }
        } else {
          // found state is in a smaller or equal group, drop the new state.
          delete new_state;
          if (STATS) ++res.num_visited_hit_drop;
          continue;
        }
      }
      // We need to keep this new state and insert it in its group.
      state_groups[new_min_moves].AddState(new_state);
      if (STATS) { ++ss; if (ss > res.max_mem_state) res.max_mem_state = ss; }
      visited_states.insert(make_pair(new_hash, new_min_moves));
      if (STATS) ++res.num_visited_states;
    }
    delete curr_state;
  }
}
