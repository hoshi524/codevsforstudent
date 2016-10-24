#include <bits/stdc++.h>
using namespace std;

static const int W = 10;
static const int H = 16;
static const int T = 3;
static const int HT = H + T;
static const int S = 10;
static const int N = 500;
static const int EMPTY = 0;
static const int OBSTACLE = S + 1;
static const int target = 60;
// static const int depth = 10;   // prod
// static const int width = 1000;  // prod
static const int depth = 10;   // test
static const int width = 100;  // test

class Pack {
 public:
  int blocks[T][T];

  Pack() {}
  Pack(const int b[T][T]) { memcpy(blocks, b, sizeof(blocks)); }
  Pack(const Pack &p) { memcpy(blocks, p.blocks, sizeof(blocks)); }

  static Pack inputFirst() {
    int blocks[T][T];
    for (int i = 0; i < T; ++i) {
      for (int j = 0; j < T; ++j) {
        int block;
        cin >> block;
        blocks[i][j] = block;
      }
    }
    string end;
    cin >> end;
    return Pack(blocks);
  }

  int fill(const int obstacle) {
    if (obstacle == 0) return 0;
    int x = 0;
    for (int i = 0; i < T; ++i) {
      for (int j = 0; j < T; ++j) {
        if (blocks[i][j] == EMPTY) {
          blocks[i][j] = OBSTACLE;
          if (++x >= obstacle) return x;
        }
      }
    }
    return x;
  }

  void rotate() {
    int tmp[T][T];
    memcpy(tmp, blocks, sizeof(blocks));
    for (int i = 0; i < T; ++i) {
      for (int j = 0; j < T; ++j) {
        blocks[j][T - 1 - i] = tmp[i][j];
      }
    }
  }
};

class Field {
 public:
  int blocks[HT][W], pos, rot, obs, value;

  Field() { memset(blocks, 0, sizeof(blocks)); }
  Field(const Field &x) : pos(x.pos), rot(x.rot), obs(x.obs), value(x.value) {
    memcpy(blocks, x.blocks, sizeof(blocks));
  }

  void input() {
    for (int i = T; i < HT; ++i) {
      for (int j = 0; j < W; ++j) {
        int block;
        cin >> block;
        blocks[i][j] = block;
      }
    }
    string end;
    cin >> end;
  }

  inline bool in(const int h, const int w) {
    return 0 <= h && h < HT && 0 <= w && w < W;
  }

  inline int setDelete(bool &end, bool (&del)[HT][W], int i, int j,
                       const int di, const int dj) {
    int x = 0;
    for (int ki = i, kj = j, sum = 0; in(i, j); i += di, j += dj) {
      if (blocks[i][j] == EMPTY || blocks[i][j] == OBSTACLE) {
        ki = i + di;
        kj = j + dj;
        sum = 0;
      } else {
        sum += blocks[i][j];
        while (sum > S) {
          sum -= blocks[ki][kj];
          ki += di;
          kj += dj;
        }
        if (sum == S) {
          end = false;
          x += 1 + max(abs(i - ki), abs(j - kj));
          del[i][j] = true;
          for (int pi = ki, pj = kj; i != pi || j != pj; pi += di, pj += dj)
            del[pi][pj] = true;
        }
      }
    }
    return x;
  }

  inline void setCheck(bool (&check)[6][HT], const int i, const int j) {
    check[0][j] = true;
    check[3][i] = true;
    if (HT - i - 1 <= j) {
      check[1][j - (HT - i - 1)] = true;
    } else {
      check[4][i + j] = true;
    }
    if (HT - i - 1 < W - j) {
      check[2][j + (HT - i - 1)] = true;
    } else {
      check[5][i + (W - j - 1)] = true;
    }
  }

  bool next(const Pack &p, const int w) {
    bool check[6][HT];
    memset(check, false, sizeof(check));
    for (int j = 0; j < T; ++j) {
      int h = HT - 1;
      for (; h >= 0; --h) {
        if (blocks[h][w + j] == EMPTY) break;
      }
      for (int i = T - 1; i >= 0; --i) {
        const int v = p.blocks[i][j];
        if (v != EMPTY) {
          setCheck(check, h, w + j);
          blocks[h--][w + j] = v;
        }
      }
    }

    bool del[HT][W];
    int score = 0;
    double chain = 1;
    while (true) {
      int e = 0;
      bool end = true;
      memset(del, false, sizeof(del));
      for (int j = 0; j < W; ++j) {
        if (check[0][j]) e += setDelete(end, del, HT - 1, j, -1, 0);
        if (check[1][j]) e += setDelete(end, del, HT - 1, j, -1, 1);
        if (check[2][j]) e += setDelete(end, del, HT - 1, j, -1, -1);
      }
      for (int i = 0; i < HT; ++i) {
        if (check[3][i]) e += setDelete(end, del, i, 0, 0, 1);
        if (check[4][i]) e += setDelete(end, del, i, 0, -1, 1);
        if (check[5][i]) e += setDelete(end, del, i, W - 1, -1, -1);
      }
      if (end) break;
      memset(check, false, sizeof(check));
      for (int j = 0; j < W; ++j) {
        for (int i = HT - 1, k = -1; i >= 0 && blocks[i][j] != EMPTY; --i) {
          if (del[i][j]) {
            blocks[i][j] = EMPTY;
            if (k == -1) k = i;
          } else if (k != -1) {
            blocks[k][j] = blocks[i][j];
            blocks[i][j] = EMPTY;
            setCheck(check, k, j);
            --k;
          }
        }
      }
      chain *= 1.3;
      e /= 2;
      score += (int)chain * e;
    }
    obs = score / 5;

    for (int i = 0; i < W; ++i) {
      if (blocks[T - 1][i] != EMPTY) return false;
    }
    {  // value
      value = 0;
      int x = 0;
      for (int i = T; i < HT; ++i) {
        for (int j = 0; j < W; ++j) {
          if (blocks[i][j] != EMPTY) {
            value += S - blocks[i][j];
            if (++x >= 120) value -= S;
          }
        }
      }
    }
    return true;
  }

  void show() {
    for (int i = 0; i < H; ++i) {
      for (int j = 0; j < W; ++j) {
        cerr << blocks[i + T][j] << " ";
      }
      cerr << endl;
    }
    cerr << endl;
  }
};

bool operator<(const Field &left, const Field &right) {
  return left.value < right.value;
}

bool operator!=(const Field &left, const Field &right) {
  for (int i = 0; i < HT; ++i) {
    for (int j = 0; j < W; ++j) {
      if (left.blocks[i][j] != right.blocks[i][j]) return true;
    }
  }
  return false;
}

namespace State {
int turn;
int time;
Pack packs[N];
Field myField;
Field opField;
int myObstacle;
int opObstacle;

void input() {
  int w, h, t, s, n;
  cin >> w >> h >> t >> s >> n;
  for (int i = 0; i < N; ++i) {
    packs[i] = Pack::inputFirst();
  }
  myField = Field();
  opField = Field();
}

void execute() {
  cin >> turn >> time;
  cin >> myObstacle;
  myField.input();
  cin >> opObstacle;
  opField.input();

  Pack np[N - turn];
  for (int t = turn; t < N; ++t) {
    np[t - turn] = packs[t];
    if (myObstacle > 0) myObstacle -= np[t - turn].fill(myObstacle);
  }

  int value = INT_MIN, pos, rot;
  priority_queue<Field> search[depth];
  search[0].push(myField);
  for (int i = 0, is = min(depth - 1, N - turn); i < is; ++i) {
    for (int j = 0; j < width && !search[i].empty(); ++j) {
      Field field = search[i].top();
      search[i].pop();
      for (int r = 0; r < 4; ++r) {
        for (int w = 0; w < W - T + 1; ++w) {
          Field c = field;
          if (c.next(np[i], w)) {
            if (i == 0) {
              c.rot = r;
              c.pos = w;
            }
            search[i + 1].push(c);

            int tv = c.value + (min(c.obs, target) << 16) - (i << 12) +
                     (max(c.obs - target, 0) << 8);
            if (value < tv) {
              value = tv;
              pos = c.pos;
              rot = c.rot;
            }
          }
        }
        np[i].rotate();
      }
    }
  }

  cout << pos << " " << rot << endl;
  cout.flush();
}
};

int main() {
  cout << "hoshi524" << endl;
  cout.flush();

  State::input();
  while (true) {
    State::execute();
  }
}
