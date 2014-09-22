#include <cstdlib>
#include <stddef.h>

#include "board.h"
#include "network_sort.h"

static const int INFINITY = 1000;

////////////////////////////////////////////////////////////////////////////////
// Board
////////////////////////////////////////////////////////////////////////////////

Board::Board(const char *p)
    : goal('a', p) {
  goal.num_tiles = goal.num_goal_tiles;
  for (int i = 0; i < BOARD_Y * BOARD_X; ++i) {
    const char c = p[i];
    b[i] = c == '#' ? '#' : BLANK;
  }
  for (int i = 0; i < goal.num_tiles; ++i) {
    for (int dir = 0; dir < 4; ++dir) {
      ComputeDistances(i, dir);
    }
  }
}

std::string Board::DebugString() const {
  return DebugString(NULL);
}

std::string Board::DebugStringWithState(const State &s) const {
  return DebugString(&s);
}

std::string Board::DebugStringWithDistance(int tile, int dir) const {
  std::string s;
  int pos = 0;
  for (int y = 0; y < BOARD_Y; ++y) {
    for (int x = 0; x < BOARD_X; ++x, ++pos) {
      char c = b[pos];
      if (c == BLANK) {
        int d = dist[tile][pos][dir];
        if (d == INFINITY) {
          c = '-';
        } else {
          c = char(d) + '0';
        }
      }
      s += c;
    }
    s += '\n';
  }
  return s;
}

std::string Board::DebugString(const State *state) const {
  std::string s;
  int pos = 0;
  for (int y = 0; y < BOARD_Y; ++y) {
    for (int x = 0; x < BOARD_X; ++x, ++pos) {
      char c = b[pos];
      if (c == BLANK) c = ' ';
      int tile = goal.Find(pos);
      if (tile != -1) c = tile + 'a';
      if (state) {
        int tile = state->Find(pos);
        if (tile != -1) {
          if (tile < state->num_goal_tiles) {
            c = tile + 'A';
          } else {
            c = '@';
          }
        }
      }
      s += c;
    }
    s += '\n';
  }
  return s;
}

void Board::FloodFrom(int tile, int pos, int nodir) {
  int value = dist[tile][pos][nodir];
  //printf("pos: %d\n", pos);
  //printf("value:%d\n", value);
  for (int di = 0; di < 4; ++di) {
    if (di == OPPOSITE(nodir)) continue;
    int d = State::DIRECTIONS[di];
    //printf("pos: %d, nodir: %d, dir: %d\n", pos, nodir, d);
    int newpos = pos + d;
    if (b[newpos] == BLANK && dist[tile][newpos][nodir] == INFINITY) {
      dist[tile][newpos][nodir] = value;
      FloodFrom(tile, newpos, nodir);
    }
  }
}

int Board::FindMinimum(int tile, int dir) {
  int min = INFINITY;
  int minpos = -1;
  for (int pos = BOARD_X; pos < BOARD_SIZE - BOARD_X; ++pos) {
    const int nextpos = pos + State::DIRECTIONS[dir];
    if (b[pos] == BLANK && b[nextpos] == BLANK &&
        dist[tile][pos][dir] == INFINITY &&
        dist[tile][nextpos][dir] < min) {
      min = dist[tile][nextpos][dir];
      minpos = pos;
    }
  }
  if (minpos >= 0) dist[tile][minpos][dir] = min + 1;
  return minpos;
}

void Board::ComputeDistances(int tile, int dir) {
  // int dist[MAX_TILES][BOARD_Y * BOARD_X][4];  // Minimum distances.
  for (int i = 0; i < BOARD_SIZE; ++i) {
    dist[tile][i][dir] = INFINITY;
  }
  int pos = goal.t[tile];
  dist[tile][pos][dir] = 0;
  //printf("----------------------tile: %d, pos: %d, dir:%d\n", tile, pos, dir);
  do {
    FloodFrom(tile, pos, dir);
    pos = FindMinimum(tile, dir);
  } while (pos >= 0);
}

int Board::MinMovesFrom(const State &state) const {
  int mindir[4] = {0, 0, 0, 0};
  for (int tile = 0; tile < goal.num_tiles; ++tile) {
    for (int dir = 0; dir < 4; ++dir) {
      const int &localmin = dist[tile][state.t[tile]][dir];
      if (localmin > mindir[dir]) {
        mindir[dir] = localmin;
      }
    }
  }
  return mindir[0] + mindir[1] + mindir[2] + mindir[3];
}

////////////////////////////////////////////////////////////////////////////////
// State
////////////////////////////////////////////////////////////////////////////////

const int State::DIRECTIONS[] = {-BOARD_X, -1, +1, +BOARD_X};  // 3-dir must work!

State::State() {
  for (int i = 0; i < MAX_HISTORY; ++i) history[i] = 0;
  history[0] = 1;
}

State::State(const Board &board, const State &old_state, int move) {
  old_state.Move(board, move, this);
}

State::State(const char *p) {
  Initialize('A', p);
}

State::State(char offset, const char *p) {
  Initialize(offset, p);
}

void State::Initialize(char offset, const char *p) {
  for (int i = 0; i < MAX_HISTORY; ++i) history[i] = 0;
  history[0] = 1;
  num_tiles = 0;
  num_goal_tiles = 0;
  for (int i = 0; i < MAX_TILES; ++i) t[i] = 0;
  for (int i = 0; i < BOARD_Y * BOARD_X; ++i) {
    const char c = p[i];
    if (c >= offset && c <= offset + ('Z'-'A')) {
      const int tile = c - offset;
      if (tile >= MAX_TILES) {
        printf("Tile too high: '%c'. Increase MAX_TILES.", c);
        exit(10);
      }
      if (t[tile] != 0) {
        printf("Duplicate tile: '%c'.", c);
        exit(11);
      }
      t[tile] = i;
      ++num_tiles;
      ++num_goal_tiles;
    }
  }
  // Other tiles.
  for (int i = 0; i < BOARD_Y * BOARD_X; ++i) {
    const char c = p[i];
    if (c == '@') {
      t[num_tiles++] = i;
    }
  }
  network_sort(&t[num_goal_tiles], num_tiles-num_goal_tiles);
}

int State::Find(int pos) const {
  for (int i = 0; i < num_tiles; ++i) {
    if (t[i] == pos) return i;
  }
  return -1;
}

void State::Move(const Board &board, int dir, State *n) const {
  for (int i = MAX_HISTORY - 1; i > 0; --i) {
    unsigned long long carry = (history[i-1] & (3llu << 62)) >> 62;
    n->history[i] = (history[i] << 2) | carry;
  }
  n->history[0] = history[0] << 2 | dir;
  const int move = DIRECTIONS[dir];
  const State *o = this;  // "old"
  n->num_tiles = num_tiles;
  n->num_goal_tiles = num_goal_tiles;
  for (int i = 0; i < MAX_TILES; ++i) n->t[i] = 0;
  for (int i = 0; i < num_tiles; ++i) {
    if (n->t[i] != 0) continue;
    int newpos = o->t[i] + move;
    if (board.b[newpos] != BLANK) {
      n->t[i] = o->t[i];  // Blocked on wall.
    } else {
      int row[MAX_TILES];
      int row_len = 0;
      row[row_len++] = i;
      while(1) {
        if (board.b[newpos] != BLANK) {  // Blocked.
          for (int re = 0; re < row_len; ++re) {
            n->t[row[re]] = o->t[row[re]];
          }
          break;
        }
        int next = Find(newpos);
        if (next == -1) {
          for (int re = 0; re < row_len; ++re) {
            n->t[row[re]] = o->t[row[re]] + move;
          }
          break;
        }
        if (n->t[next] != 0) {
          // This is optimization.
          if (n->t[next] == o->t[next]) {
            for (int re = 0; re < row_len; ++re) {
              n->t[row[re]] = o->t[row[re]];
            }
            break;
          } else {
            for (int re = 0; re < row_len; ++re) {
              n->t[row[re]] = o->t[row[re]] + move;
            }
            break;
          }
        }
        row[row_len++] = next;
        newpos = o->t[next] + move;
      }
    }
  }
  if (move != -1 && move != 1) {
    network_sort(&n->t[num_goal_tiles], n->num_tiles - n->num_goal_tiles);
  }
  //return n->Hash() == o->Hash();  // TODO: Room for optimization.
}

long long State::Hash() const {
  long long h = 0;
  for (int i = 0; i < num_tiles; ++i) {
    h <<= POSITION_BITS;
    h += t[i];
  }
  return h;
}

std::string State::GetHistory() const {
  static const char DIRNAME[] = "ULRD";
  std::string ret;

  int idx = MAX_HISTORY - 1;
  while(history[idx] == 0) --idx;

  // First element.
  int shift = 62;
  unsigned long long mask = 3ull << shift;
  while (!((history[idx] & mask) >> shift)) {
    shift -= 2;
    mask >>= 2;
    //printf ("%llx - %d\n", mask, shift);
  }
  while(shift > 0) {
    shift -= 2;
    mask >>= 2;
    ret += DIRNAME[(history[idx] & mask) >> shift];
  }
  // Other elements.
  while(--idx >= 0) {
    int shift = 62;
    unsigned long long mask = 3ull << shift;
    while(shift >= 0) {
      ret += DIRNAME[(history[idx] & mask) >> shift];
      shift -= 2;
      mask >>= 2;
    }
  }
  return ret;
}

int State::GetHistoryLen() const {
    int idx = MAX_HISTORY - 1;
    while(history[idx] == 0) --idx;

    int ret = sizeof(unsigned long long) * 4 * idx;

    unsigned long long h = history[idx];

    while (h >>= 2)	ret += 1;
    return ret;
}
