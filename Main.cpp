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
static const int target = 10;
static const int dx[4] = {1, 1, 0, 1};
static const int dy[4] = {0, 1, 1, -1};
// static const int depth = 10;   // prod
// static const int width = 200;  // prod
static const int depth = 5;    // test
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
  int blocks[HT][W], pos, rot, chain, value, prev;

  Field() { memset(blocks, 0, sizeof(blocks)); }
  Field(const Field &x)
      : pos(x.pos), rot(x.rot), chain(x.chain), value(x.value), prev(x.prev) {
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

  bool next(const Pack &p, const int w) {
    for (int j = 0; j < T; ++j) {
      int h = HT - 1;
      for (; h >= 0; --h) {
        if (blocks[h][w + j] == EMPTY) break;
      }
      for (int i = T - 1; i >= 0; --i) {
        const int v = p.blocks[i][j];
        if (v != EMPTY) blocks[h--][w + j] = v;
      }
    }

    bool del[H + T][W];
    chain = 0;
    while (true) {
      bool d = true;
      memset(del, false, sizeof(del));
      for (int j = 0; j < W; ++j) {
        for (int i = HT - 1; i >= 0 && blocks[i][j] != EMPTY; --i) {
          if (blocks[i][j] == OBSTACLE) continue;
          for (int k = 0; k < 4; ++k) {
            int sum = 0;
            for (int a = i, b = j; in(a, b) && blocks[a][b] != EMPTY && sum < S;
                 a += dy[k], b += dx[k]) {
              sum += blocks[a][b];
            }
            if (sum != S) continue;
            sum = 0;
            d = false;
            for (int a = i, b = j; in(a, b) && blocks[a][b] != EMPTY && sum < S;
                 a += dy[k], b += dx[k]) {
              sum += blocks[a][b];
              del[a][b] = true;
            }
          }
        }
      }
      if (d) break;
      ++chain;
      for (int j = 0; j < W; ++j) {
        for (int i = HT - 1, k = -1; i >= 0 && blocks[i][j] != EMPTY; --i) {
          if (del[i][j]) {
            blocks[i][j] = EMPTY;
            if (k == -1) k = i;
          } else if (k != -1) {
            blocks[k][j] = blocks[i][j];
            blocks[i][j] = EMPTY;
            --k;
          }
        }
      }
    }

    for (int i = 0; i < W; ++i) {
      if (blocks[T - 1][i] != EMPTY) return false;
    }
    {  // value
      value = 0;
      int x = 0;
      for (int i = T; i < HT; ++i) {
        for (int j = 0; j < W; ++j) {
          if (blocks[i][j] != EMPTY) {
            value += S - blocks[i][j] - 1;
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
  return left.value > right.value;
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

  const int mini = -1000000;
  int value = mini;
  int d = -1;
  Field best;
  vector<Field> search[depth];
  search[0].push_back(myField);
  for (int i = 0, is = min(depth - 1, N - turn); i < is; ++i) {
    for (int j = 0, js = min(width, (int)search[i].size()); j < js; ++j) {
      for (int r = 0; r < 4; ++r) {
        for (int w = 0; w < W - T + 1; ++w) {
          Field c = search[i][j];
          if (c.next(np[i], w)) {
            c.rot = r;
            c.pos = w;
            c.prev = j;
            search[i + 1].push_back(c);

            int tv = c.value;
            if (c.chain >= target) tv += (c.chain * 10 - i) * 0xff;
            if (value < tv) {
              value = tv;
              d = i;
              best = c;
            }
          }
        }
        np[i].rotate();
      }
    }
    sort(search[i + 1].begin(), search[i + 1].end());
  }

  while (d > 0) {
    best = search[d--][best.prev];
  }
  cout << best.pos << " " << best.rot << endl;
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
