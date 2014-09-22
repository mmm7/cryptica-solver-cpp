#ifndef _CRYPTICA_SOLVE_H__
#define _CRYPTICA_SOLVE_H__

#include <map>
#include <set>
#include <string>

//#include "hash_map.hpp"

#include "board.h"

static const int MAX_MOVES = 100;

typedef std::map<long long, const State*> StateMap;

// Represents a group of state candidates that are equally good.
class StateGroup {
 public:
  ~StateGroup() {
    for (StateMap::iterator it = states.begin();
         it != states.end(); ++it) {
      delete it->second;
    }
  }

  void AddState(const State* state) { states[state->Hash()] = state; }

  // Releases ownership. Assumes non-empty.
  const State* Pop() {
    const State* s = states.begin()->second;
    states.erase(states.begin());
    return s;
  }

  void RemoveState(long long key) {
    StateMap::iterator it = states.find(key);
    if (it != states.end()) {
      delete it->second;
      states.erase(it);
    }
  }

  bool Empty() const {
    return states.empty();
  }

 private:
  // Kinda owns the pointer, but Pop releases ownership.
  StateMap states;
};

struct SolveResult {
  SolveResult() :
    success(false),
    num_moves(0),
    num_visited_states(0),
    num_visited_hit_improve(0),
    num_visited_hit_drop(0),
    max_mem_state(0) {}
  bool success;
  State end_state;
  // Stats.
  int num_moves;
  int num_visited_states;
  int num_visited_hit_improve;
  int num_visited_hit_drop;
  int max_mem_state;
};

// Returns whether the puzzle can be solved.
SolveResult solve(const Board& board, const State& start_state);

#endif // _CRYPTICA_SOLVE_H__

