#include <iostream>
#include <set>

#include "solve.h"

#include "gtest/gtest.h"

using namespace std;

#include "testboards.h"

void CheckSolution(const char* bs, const string& exp_sol) {
  Board b(bs);
  State s(bs);
  const SolveResult res = solve(b, s);
  const std::string sol = res.end_state.GetHistory();
  int num_moves = res.end_state.GetHistoryLen();
  bool success = res.success;
  EXPECT_EQ(!exp_sol.empty(), success);
  EXPECT_EQ(exp_sol.size(), num_moves);
  EXPECT_EQ(exp_sol, sol);
}

TEST(TestSolve, Solve) {
  CheckSolution(B001, "R");
  CheckSolution(B002, "LRR");
  CheckSolution(B003, "");
  CheckSolution(B004, "RDRUUDLLRRU");
  CheckSolution(B005, "LLULDDDURRRRRRD");
  // old: CheckSolution(B006, "LDRUURRDDDDUUUDRUULDDDDLLLR");
  CheckSolution(B006, "RDLUUDLDLDULUURRRRRLDDDDLLR");
  //CheckSolution(B007, "LDRRRLUULLLLDRRDUURRULLLURRRDDDD");
  //  CheckSolution(I001, "");  // Stress test.
}
