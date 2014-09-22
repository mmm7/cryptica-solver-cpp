//#include <cstdlib>

extern "C" {
#include "getcputime.h"
}
#include <iostream>
#include <map>

#include "board.h"
#include "solve_dfs.h"

using namespace std;

double start_t = 0.0;
double end_t = 0.0;

map<long long, int> visited;

bool solve_dfs(const Board& board, const State& state,
               /*set<long long> visited, */
               int* upper_bound, string* moves, int* N, int depth) {
  if (depth == 0) {
    visited.clear();
  }
  ++(*N);
  if (((*N) % 1000000) == 0) {
    end_t = getCPUTime();
    cout << (*N) << " " << (end_t - start_t) << " dep: " << depth << " " 
         << state.GetHistory() << "\n";
    start_t = end_t;
  }
  long long old_hash = state.Hash();
  visited[old_hash] = depth;
  if (board.IsGoal(state)) {
    *moves = state.GetHistory();
    cout << "solution found. num states: " << *N
         << " steps: " << moves->size() << " " << depth << endl;
    *upper_bound = moves->size();
    return true;
  }

  for (int di = 0; di < 4; ++di) {
    State new_state;
    state.Move(board, di, &new_state);
    //    cout << new_state.GetHistory() << " ";
    if (depth + 1 + board.MinMovesFrom(new_state) > *upper_bound) {
      // Cut this branch, no solution possible here.
      //cout << "bound " << (depth + 1) << "+"
      //     << board.MinMovesFrom(new_state) << ">" << *upper_bound << endl;
      continue;
    }
    long long hash = new_state.Hash();
    map<long long, int>::const_iterator it = visited.find(hash);
    if (it != visited.end()) {
      if (it->second <= depth) {
        // Already seen with a shorter depth.
        //cout << "found\n";
        continue;
      } else {
        // This is a shorter depth, update visited.
        visited[hash] = depth;
      }
    }
    //cout << "ok" << endl;
    // Even if this returns true, we continue to find a better solution.
    solve_dfs(board, new_state/*, visited*/, upper_bound, moves, N, (depth + 1));
  }
  return moves->size() > 0;
}
