#ifndef _CRYPTICA_BOARD_H__
#define _CRYPTICA_BOARD_H__

#include <cstring>
#include <string>

static const int BOARD_X = 9;
static const int BOARD_Y = 7;

static const int BOARD_SIZE = BOARD_X * BOARD_Y;
static const int POSITION_BITS = 6;  // 2^POSITION_BITS > BOARD_SIZE

static const int MAX_TILES = 10;

static const int MAX_HISTORY = 2;  // times 32 moves.

static inline int POS(int Y, int X) {
  return Y * BOARD_X + X;
}

static inline int OPPOSITE(int dir) {
  return 3-dir;
}

class Board;

class State {
 public:
  static const int UP = 0;
  static const int LEFT = 1;
  static const int RIGHT = 2;
  static const int DOWN = 3;
  static const int DIRECTIONS[];

  State();

  explicit State(const char *p);
  // Make a move on a board.
  // dir is the direction (0, 1, 2, 3) or (UP, LEFT, RIGHT, DOWN)
  // n is the new state.
  // Return value: true if no tile was moved.
  void Move(const Board &board, int dir, State *n) const;
  // Move based constructor.
  State(const Board &board, const State &old_state, int move);

  long long Hash() const;

  int NumTiles() const { return num_tiles; }
  int NumGoalTiles() const { return num_goal_tiles; }

  std::string GetHistory() const;
  int GetHistoryLen() const;

 protected:
  State(char offset, const char *p);
  // offset: 'A' if tiles, 'a' if goals.
  void Initialize(char offset, const char *p);
  // --------------------------------------------------
  // Visible for testing

  // Find the tile occupying a position.
  // Return the index of the tile or -1 if position is empty.
  int Find(int pos) const;

 private:
  friend class Board;
  int num_tiles;
  int num_goal_tiles;
  unsigned long long history[MAX_HISTORY];
  int t[MAX_TILES];      // Tiles.

  static inline int MIN(int X, int Y) {
    return X < Y ? X : Y;
  }

 public:
  bool operator==(const State &other) const {
    return !(memcmp(
          &this->t, &other.t,
          MIN(this->num_goal_tiles, other.num_goal_tiles) * sizeof(int)));
  }
  bool operator!=(const State &other) const {
    return !(*this == other);
  }
};

typedef int Cell;
static const Cell BLANK = ' ';

class Board {
 public:
  explicit Board(const char *i);
  std::string DebugString() const;
  std::string DebugStringWithState(const State &s) const;
  std::string DebugStringWithDistance(int tile, int dir) const;

  // The minimum number of moves required from state.
  int MinMovesFrom(const State &state) const;
  bool IsGoal(const State& state) const { return state == goal; }

 protected:
  const State &Goal() const { return goal; }
  int ***Dist() { return (int***)dist; }

 private:
  friend class State;
  Cell b[BOARD_Y * BOARD_X];  // Board.
  State goal;
  // [tile][pos][direction]
  // What is the minimum number of pushes required to push tile from
  // pos to its goal.
  int dist[MAX_TILES][BOARD_Y * BOARD_X][4];  // Minimum distances.

  void ComputeDistances(int tile, int dir);
  void FloodFrom(int tile, int pos, int dir);
  int FindMinimum(int tile, int dir);

  std::string DebugString(const State *state) const;

 public:
  bool operator==(const Board &other) const {
    return !(memcmp(&this->b, &other.b, BOARD_X*BOARD_Y*sizeof(Cell)));
  }
  bool operator!=(const Board &other) const {
    return !(*this == other);
  }

};

inline int MoveFromChar(char di) {
  switch(di) {
    case 'U': return 0;
    case 'L': return 1;
    case 'R': return 2;
    default: return 3;
  }
}

#endif // _CRYPTICA_BOARD_H__
