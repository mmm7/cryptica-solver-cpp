#include <iostream>
#include <set>

#include "board.h"
#include "solve_dfs.h"

#include "gtest/gtest.h"

using namespace std;

#include "testboards.h"

void CheckSolutionDfs(const char* bs, int bound, const string& exp_sol) {
    cout << "Checking with bound " << bound << endl;
    Board b(bs);
    State s(bs);
    std::string sol = "";
    set<long long> visited;
  //  visited.insert(s.Hash());

    int N = 0;
    bool success = solve_dfs(b, s/*, visited*/, &bound, &sol, &N, 0);
    std::cout << "\nSOLUTION:" << (success ? "Y" : "N")
              << " " << sol.size() << " " << sol << std::endl;
    cout << b.DebugStringWithState(s);
    State curr_state = s;
    for (int i = 0; i < sol.size(); ++i) {
      State new_state;
      curr_state.Move(b, MoveFromChar(sol[i]), &new_state);
      cout << b.DebugStringWithState(new_state);
      curr_state = new_state;
    }
    EXPECT_EQ(!exp_sol.empty(), success);
    EXPECT_EQ(exp_sol, sol);
}

TEST(TestSolve, SolveDfs) {
  CheckSolutionDfs(B001, 5, "R");
  CheckSolutionDfs(B002, 5, "LRR");
  CheckSolutionDfs(B003, 5, "");
  CheckSolutionDfs(B004, 11, "RDRUUDLLRRU");
  CheckSolutionDfs(B005, 17, "RDRRUUUDLLLLLLU");
  //CheckSolutionDfs(B006, 27, "LDRUURRDDDDUUUDRUULDDDDLLLR");
  //CheckSolutionDfs(B007, 32, "LDRRRLUULLLLDRRDUURRULLLURRRDDDD");
}

