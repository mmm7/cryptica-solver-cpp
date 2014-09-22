#ifndef _CRYPTICA_SOLVE_DFS_H__
#define _CRYPTICA_SOLVE_DFS_H__

#include <string>

class Board;
class State;

bool solve_dfs(const Board& board, const State& state,
               /*std::set<long long> visited, */
               int* upper_bound, std::string* moves, int* N, int depth);

#endif // _CRYPTICA_SOLVE_DFS_H__

