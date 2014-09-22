#include <cstdlib>

extern "C" {
#include "getcputime.h"
}
#include "board.h"

static const int RANDOM_DIR_SIZE = 20000000;

static const char BOARD[] =
    "#########"
    "#   c C #"
    "# #b#B# #"
    "# a A  F#"
    "# # #E# #"
    "#  D    #"
    "#########";


static int random_dir[RANDOM_DIR_SIZE];

inline void test(Board *b, State *s) {
  for (int i = 0; i < RANDOM_DIR_SIZE; ++i) {
    State *n = new State();
    s->Move(*b, random_dir[i], n);
    delete s;
    s = n;
  }
}

int main(int args, char **argv) {
  srand(17);
  for(int i = 0; i < RANDOM_DIR_SIZE; ++i) {
    random_dir[i] = rand() % 4;
  }
  Board b(BOARD);
  {
    State *s = new State(BOARD);
    test(&b, s);
  }
  double start = getCPUTime();
  {
    State *s = new State(BOARD);
    test(&b, s);
  }
  double end = getCPUTime();
  printf("%f\n", end-start);
  return 0;
}

/*
-59.07%  8.302901 3.398140 +  Compiler option -O3
-12.08%  3.324245 2.922759 -  Always return 0.
-02.23%  3.309039 3.235412 +  Use Hash() instead of equality.
-01.66%  3.224007 3.170432 +  Cell type: char->int
-08.03%  3.168030 2.913676 -  Always return 0 (instead of 2xHash()).
+14.16%  2.913676 3.326343 i  Extra cost of MinMovesFrom() over Move()
+90.36%  1.341389 2.553433 i  3 tiles -> 6 tiles
+02.00%  2.553433 2.604416 -! BLANK: ' ' -> 0
-09.25%  2.441346 2.215454 +  Move() optimization: sor.
-00.07%  2.826602 2.824619 +  history 30->60
*/
