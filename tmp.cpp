#include <bits/stdc++.h>
using namespace std;

static const int W = 10;
static const int H = 16;
static const int T = 3;
static const int HT = H + T;
static const int HTW = HT * W;
static const int S = 10;
static const int N = 500;
static const int EMPTY = 0;
static const int OBSTACLE = S + 1;
static const int target = 80;
static const int tasksize = 70;
// static const int width = 120;  // prod
static const int width = 10;  // test
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
  char blocks[HTW];
  int pos, rot, obs, value;

  Field() { memset(blocks, 0, sizeof(blocks)); }
  Field(const Field &) = default;
  Field &operator=(const Field &) = default;
  Field(Field &&) noexcept = default;
  Field &operator=(Field &&) noexcept = default;

  void input() {
    int block;
    for (int i = W * 3; i < HTW; ++i) {
      cin >> block;
      blocks[i] = block;
    }
    string end;
    cin >> end;
  }

  int setDelete(int (&task)[tasksize][2], int size, bool (&del)[HTW]) {
    int e = 0;
    for (int t = 0; t < size; ++t) {
      switch (task[t][0]) {
        case 0:
          // 上
          for (int p = task[t][1], k = p, sum = 0; p >= 0 && blocks[p];
               p -= W) {
            sum += blocks[p];
            while (sum > S) {
              sum -= blocks[k];
              k -= W;
            }
            if (sum == S) {
              e += 1 + (k - p) / W;
              for (int n = p; n <= k; n += W) del[n] = true;
            }
          }
          break;
        case 1:
          // 右上
          for (int d = W - 1, p = task[t][1], k = p, sum = 0, x = W - p % W; x;
               p -= d, --x) {
            if (blocks[p]) {
              sum += blocks[p];
              while (sum > S) {
                sum -= blocks[k];
                k -= d;
              }
              if (sum == S) {
                e += 1 + (k - p) / d;
                for (int n = p; n <= k; n += d) del[n] = true;
              }
            } else {
              k = p - d;
              sum = 0;
            }
          }
          break;
        case 2:
          // 左上
          for (int d = W + 1, p = task[t][1], k = p, sum = 0, x = p % W + 1; x;
               p -= d, --x) {
            if (blocks[p]) {
              sum += blocks[p];
              while (sum > S) {
                sum -= blocks[k];
                k -= d;
              }
              if (sum == S) {
                e += 1 + (k - p) / d;
                for (int n = p; n <= k; n += d) del[n] = true;
              }
            } else {
              k = p - d;
              sum = 0;
            }
          }
          break;
        case 3:
          // 右
          for (int p = task[t][1], k = p, sum = 0, x = W; x; ++p, --x) {
            if (blocks[p]) {
              sum += blocks[p];
              while (sum > S) {
                sum -= blocks[k];
                ++k;
              }
              if (sum == S) {
                e += 1 + p - k;
                for (int n = k; n <= p; ++n) del[n] = true;
              }
            } else {
              k = p + 1;
              sum = 0;
            }
          }
          break;
        case 4:
          // 右上
          for (int d = W - 1, p = task[t][1], k = p, sum = 0,
                   x = min(p / W + 1, W);
               x; p -= d, --x) {
            if (blocks[p]) {
              sum += blocks[p];
              while (sum > S) {
                sum -= blocks[k];
                k -= d;
              }
              if (sum == S) {
                e += 1 + (k - p) / d;
                for (int n = p; n <= k; n += d) del[n] = true;
              }
            } else {
              k = p - d;
              sum = 0;
            }
          }
          break;
        case 5:
          // 左上
          for (int d = W + 1, p = task[t][1], k = p, sum = 0,
                   x = min(p / W + 1, W);
               x; p -= d, --x) {
            if (blocks[p]) {
              sum += blocks[p];
              while (sum > S) {
                sum -= blocks[k];
                k -= d;
              }
              if (sum == S) {
                e += 1 + (k - p) / d;
                for (int n = p; n <= k; n += d) del[n] = true;
              }
            } else {
              k = p - d;
              sum = 0;
            }
          }
          break;
      }
    }
    return e;
  }

  inline void setCheck(bool (&check)[6][HTW], int (&task)[tasksize][2],
                       int &size, int c, int p) {
    assert(0 <= p && p < HTW);
    if (check[c][p]) {
      check[c][p] = false;
      task[size][0] = c;
      task[size][1] = p;
      ++size;
    }
  }

  inline void setCheck(bool (&check)[6][HTW], int (&task)[tasksize][2],
                       int &size, const int p) {
    assert(p < HTW);
    if (blocks[p] == OBSTACLE) return;
    int h = p / W, w = p % W;
    setCheck(check, task, size, 0, HTW - W + w);
    setCheck(check, task, size, 3, p - w);
    if (HT - h - 1 <= w) {
      setCheck(check, task, size, 1, HTW - W + w - (HT - h - 1));
    } else {
      setCheck(check, task, size, 4, (h + w) * W);
    }
    if (HT - h - 1 < W - w) {
      setCheck(check, task, size, 2, HTW - W + w + (HT - h - 1));
    } else {
      setCheck(check, task, size, 5, (h + (W - w - 1)) * W);
    }
  }

  int chain(bool (&check)[6][HTW], int (&task)[tasksize][2], int size) {
    bool del[HTW];
    int score = 0;
    double chain = 1;
    int x = 0;
    while (true) {
      assert(x++ < 100);
      memset(del, false, sizeof(del));
      int e = setDelete(task, size, del);
      if (e == 0) break;

      memset(check, true, sizeof(check));
      size = 0;
      for (int j = 0; j < W; ++j) {
        for (int i = HTW - 1 - j, k = 0; i >= 0 && blocks[i]; i -= W) {
          if (del[i]) {
            blocks[i] = EMPTY;
            if (k == 0) k = i;
          } else if (k) {
            blocks[k] = blocks[i];
            setCheck(check, task, size, k);
            blocks[i] = EMPTY;
            k -= W;
          }
        }
      }
      chain *= 1.3;
      score += (int)chain * (e / 2);
    }
    return score / 5;
  }

  bool next(const Pack &pack, const int w) {
    bool check[6][HTW];
    memset(check, true, sizeof(check));
    int task[tasksize][2], size = 0;
    for (int j = 0; j < T && w + j < W; ++j) {
      int p = HTW - W + j + w;
      while (blocks[p]) p -= W;
      for (int i = T - 1; i >= 0; --i) {
        const int v = pack.blocks[i][j];
        if (v) {
          blocks[p] = v;
          setCheck(check, task, size, p);
          p -= W;
        }
      }
    }
    obs = chain(check, task, size);

    for (int i = W * 2; i < W * 3; ++i) {
      if (blocks[i]) return false;
    }
    {  // value
      value = 0;
      for (int w = 0; w < W; ++w) {
        int p = HTW - 1 - w;
        while (blocks[p]) p -= W;
        for (int b = 1; b < S; ++b) {
          Field f = *this;
          memset(check, true, sizeof(check));
          size = 0;
          setCheck(check, task, size, p);
          f.blocks[p] = b;
          int obs = f.chain(check, task, size);
          if (value < obs) value = obs;
        }
      }
      value <<= 10;
      for (int i = 0; i < HTW; ++i) {
        if (blocks[i]) {
          value += S - blocks[i];
        }
      }
    }
    return true;
  }

  void show() {
    for (int i = T; i < HT; ++i) {
      for (int j = 0; j < W; ++j) {
        cerr << (int)blocks[i * W + j] << " ";
      }
      cerr << endl;
    }
    cerr << endl;
  }
};

inline bool operator<(const Field &left, const Field &right) {
  return left.value < right.value;
}

inline bool operator!=(const Field &left, const Field &right) {
  for (int i = 0; i < HTW; ++i) {
    if (left.blocks[i] != right.blocks[i]) return true;
  }
  return false;
}

namespace State {
int turn;
int time;
Pack packs[N];
Field myField;
Field opField;
Field prev;
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
  if (turn > 0 && prev != myField) {
    // cerr << "diff" << endl;
    // myField.show();
    // prev.show();
    // exit(1);
  }
  return true;
}

void execute() {
  Pack np[N - turn];
  for (int t = turn, o = myObstacle; t < N; ++t) {
    np[t - turn] = packs[t];
    if (o) o -= np[t - turn].fill(o);
  }
  int value = INT_MIN, pos, rot, ti = -1, obs;
  priority_queue<Field> search[depth];
  search[0].push(myField);
  for (int i = 0, is = min(depth - 1, N - turn); i < is; ++i) {
    for (int j = 0; j < width && !search[i].empty(); ++j) {
      Field field = search[i].top();
      search[i].pop();
      for (int r = 0; r < 4; ++r) {
        int left = 0, right = W - T + 1;
        {
          int(&p)[T][T] = np[i].blocks;
          for (int a = 0; a < T; ++a) {
            if (p[0][a] | p[1][a] | p[2][a]) break;
            --left;
          }
          for (int a = T - 1; a > 0; --a) {
            if (p[0][a] | p[1][a] | p[2][a]) break;
            ++right;
          }
        }
        for (int w = left; w < right; ++w) {
          Field c = field;
          if (c.next(np[i], w)) {
            if (i == 0) {
              c.rot = r;
              c.pos = w;
            }
            search[i + 1].push(c);

            int tv = c.value + (c.obs << 22) - (i << 20);
            if (value < tv) {
              value = tv;
              pos = c.pos;
              rot = c.rot;
              obs = c.obs;
              ti = i;
            }
          }
        }
        np[i].rotate();
      }
    }
  }
  printf("%d %d\n", pos, rot);
  if (ti == 0 && obs > 0) {
    cerr << "turn : obs  " << turn << " : " << obs << endl;
  }
  {
    prev = myField;
    for (int i = 0; i < rot; ++i) {
      np[0].rotate();
    }
    prev.next(np[0], pos);
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
