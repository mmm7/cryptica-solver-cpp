#include <iostream>
#include <string>

#include "board.h"
#include "solve.h"

int main(int argc, char **argv) {
  char bc[] =
    "#########"
    "#       #"
    "#       #"
    "#       #"
    "#       #"
    "#       #"
    "#########";
  for (int i = 0; i < 5; ++i) {
    std::string st;
    std::cin >> st;
    for (int x = 0; x < 7; ++x) {
      bc[(i+1)*9+(x+1)] = st[x];
    }
  }
  Board b(bc);
  State s(bc);
  const SolveResult res = solve(b, s);
  std::string sol = res.end_state.GetHistory();
  if (argc > 1) {
    std::cout
      << sol.length() << ", "
      << sol << ", "
      << res.num_moves << ", "
      << res.max_mem_state << ", "
      << res.num_visited_states << ", "
      << res.num_visited_hit_improve << ", "
      << res.num_visited_hit_drop << ", "
      << std::endl;
    return 0;
  }
  std::cout << b.DebugStringWithState(s);
  std::cout << "SOLUTION:" << (res.success ? "Y" : "N") << " "
    <<"(" << sol.length() << ") " << sol << std::endl;
  std::cout << "num_moves: "
    << res.num_moves << std::endl;
  std::cout << "num_visited_states: "
    << res.num_visited_states << std::endl;
  std::cout << "num_visited_hit_improve: "
    << res.num_visited_hit_improve << std::endl;
  std::cout << "num_visited_hit_drop: "
    << res.num_visited_hit_drop << std::endl;
  std::cout << "max_mem_state: "
    << res.max_mem_state << std::endl;

  if (!res.success) {
    return 0;
  }
  std::cout << b.DebugStringWithState(s);
  State curr_state = s;
  for (int i = 0; i < sol.size(); ++i) {
    State new_state;
    curr_state.Move(b, MoveFromChar(sol[i]), &new_state);
    std::cout << b.DebugStringWithState(new_state);
    curr_state = new_state;
  }
}
