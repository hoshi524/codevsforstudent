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
static const int target = 80;
static const int node = 10000; // prod
// static const int node = 1000; // test
static const int depth = 10;

class Pack {
 public:
  int blocks[T][T];

  Pack() {}
  Pack(const int b[T][T]) { memcpy(blocks, b, sizeof(blocks)); }
  Pack(const Pack &p) { memcpy(this, &p, sizeof(p)); }

  static Pack input() {
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
  Field(const Field &x) { memcpy(this, &x, sizeof(x)); }

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

  inline int setDelete(const bool (&check)[6][HT], bool (&del)[HT][W]) {
    int e = 0;
    for (int j = 0; j < W; ++j) {
      if (check[0][j]) {
        // 上
        for (int i = HT - 1, ki = i, sum = 0; i >= 0; --i) {
          if (blocks[i][j] == EMPTY || blocks[i][j] == OBSTACLE) {
            ki = i - 1;
            sum = 0;
          } else {
            sum += blocks[i][j];
            while (sum > S) {
              sum -= blocks[ki][j];
              --ki;
            }
            if (sum == S) {
              e += 1 - i + ki;
              for (int pi = ki; pi >= i; --pi) del[pi][j] = true;
            }
          }
        }
      }
      if (check[1][j]) {
        // 右上
        for (int ti = HT - 1, tj = j, ki = ti, kj = tj, sum = 0; tj < W;
             --ti, ++tj) {
          if (blocks[ti][tj] == EMPTY || blocks[ti][tj] == OBSTACLE) {
            ki = ti - 1;
            kj = tj + 1;
            sum = 0;
          } else {
            sum += blocks[ti][tj];
            while (sum > S) {
              sum -= blocks[ki][kj];
              --ki;
              ++kj;
            }
            if (sum == S) {
              e += 1 + tj - kj;
              for (int pi = ki, pj = kj; pj <= tj; --pi, ++pj)
                del[pi][pj] = true;
            }
          }
        }
      }
      if (check[2][j]) {
        // 左上
        for (int ti = HT - 1, tj = j, ki = ti, kj = tj, sum = 0; tj >= 0;
             --ti, --tj) {
          if (blocks[ti][tj] == EMPTY || blocks[ti][tj] == OBSTACLE) {
            ki = ti - 1;
            kj = tj - 1;
            sum = 0;
          } else {
            sum += blocks[ti][tj];
            while (sum > S) {
              sum -= blocks[ki][kj];
              --ki;
              --kj;
            }
            if (sum == S) {
              e += 1 - tj + kj;
              for (int pi = ki, pj = kj; pj >= tj; --pi, --pj)
                del[pi][pj] = true;
            }
          }
        }
      }
    }
    for (int i = 0; i < HT; ++i) {
      if (check[3][i]) {
        // 右
        for (int j = 0, kj = j, sum = 0; j < W; ++j) {
          if (blocks[i][j] == EMPTY || blocks[i][j] == OBSTACLE) {
            kj = j + 1;
            sum = 0;
          } else {
            sum += blocks[i][j];
            while (sum > S) {
              sum -= blocks[i][kj];
              ++kj;
            }
            if (sum == S) {
              e += 1 + j - kj;
              for (int pj = kj; pj <= j; ++pj) del[i][pj] = true;
            }
          }
        }
      }
      if (check[4][i]) {
        // 右上
        for (int ti = i, tj = 0, ki = ti, kj = tj, sum = 0; ti >= 0 && tj < W;
             --ti, ++tj) {
          if (blocks[ti][tj] == EMPTY || blocks[ti][tj] == OBSTACLE) {
            ki = ti - 1;
            kj = tj + 1;
            sum = 0;
          } else {
            sum += blocks[ti][tj];
            while (sum > S) {
              sum -= blocks[ki][kj];
              --ki;
              ++kj;
            }
            if (sum == S) {
              e += 1 + tj - kj;
              for (int pi = ki, pj = kj; pj <= tj; --pi, ++pj)
                del[pi][pj] = true;
            }
          }
        }
      }
      if (check[5][i]) {
        // 左上
        for (int ti = i, tj = W - 1, ki = ti, kj = tj, sum = 0;
             ti >= 0 && tj >= 0; --ti, --tj) {
          if (blocks[ti][tj] == EMPTY || blocks[ti][tj] == OBSTACLE) {
            ki = ti - 1;
            kj = tj - 1;
            sum = 0;
          } else {
            sum += blocks[ti][tj];
            while (sum > S) {
              sum -= blocks[ki][kj];
              --ki;
              --kj;
            }
            if (sum == S) {
              e += 1 - tj + kj;
              for (int pi = ki, pj = kj; pj >= tj; --pi, --pj)
                del[pi][pj] = true;
            }
          }
        }
      }
    }
    return e;
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
        if (v) {
          setCheck(check, h, w + j);
          blocks[h--][w + j] = v;
        }
      }
    }

    bool del[HT][W];
    int score = 0;
    double chain = 1;
    while (true) {
      memset(del, false, sizeof(del));
      int e = setDelete(check, del);
      if (e == 0) break;

      memset(check, false, sizeof(check));
      for (int j = 0; j < W; ++j) {
        for (int i = HT - 1, k = -1; i >= 0 && blocks[i][j]; --i) {
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
      if (blocks[T - 1][i]) return false;
    }
    {  // value
      value = 0;
      for (int i = T; i < HT; ++i) {
        for (int j = 0; j < W; ++j) {
          if (blocks[i][j]) {
            value += S - blocks[i][j];
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
    packs[i] = Pack::input();
  }
  myField = Field();
  opField = Field();
}

bool inputTurn() {
  if (!(cin >> turn >> time)) return false;
  cin >> myObstacle;
  myField.input();
  cin >> opObstacle;
  opField.input();
  return true;
}

void execute() {
  Pack np[N - turn];
  for (int t = turn; t < N; ++t) {
    np[t - turn] = packs[t];
    if (myObstacle > 0) myObstacle -= np[t - turn].fill(myObstacle);
  }

  int value = INT_MIN, pos, rot, ti = -1, obs;
  priority_queue<Field> search[depth];
  search[0].push(myField);
  bool ok = true;
  for (int k = 0; ok && k < node;) {
    ok = false;
    for (int i = 0, is = min(depth - 1, N - turn); i < is; ++i) {
      for (int j = 0; j < 5 && !search[i].empty(); ++j) {
        ok = true;
        ++k;
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

              int tv = c.value + (min(c.obs, target) << 17) - (i << 16) +
                       (max(c.obs - target, 0) << 8);
              if (value < tv) {
                value = tv;
                pos = c.pos;
                rot = c.rot;
                obs = c.obs;
                ti = i;
              }
              if (c.obs >= target) {
                is = i + 1;
              }
            }
          }
          np[i].rotate();
        }
      }
    }
  }
  printf("%d %d\n", pos, rot);
  if (ti == 0 && obs >= target) {
    cerr << "turn : obs  " << turn << " : " << obs << endl;
  }
}
};

int main() {
  printf("hoshi524\n");

  State::input();
  while (State::inputTurn()) {
    State::execute();
  }
}
