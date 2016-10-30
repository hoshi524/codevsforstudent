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
  char blocks[HT][W];
  int pos, rot, obs, value;

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

  inline int setDelete(const int (&task)[HT * W][2], const int size,
                       bool (&del)[HT][W]) {
    int e = 0;
    for (int t = 0; t < size; ++t) {
      int i = task[t][1];
      switch (task[t][0]) {
        case 0:
          // 上
          for (int j = HT - 1, kj = j, sum = 0; j >= 0 && blocks[j][i]; --j) {
            sum += blocks[j][i];
            while (sum > S) {
              sum -= blocks[kj][i];
              --kj;
            }
            if (sum == S) {
              e += 1 - j + kj;
              for (int pj = kj; pj >= j; --pj) del[pj][i] = true;
            }
          }
          break;
        case 1:
          // 右上
          for (int tj = HT - 1, ti = i, kj = tj, ki = ti, sum = 0; ti < W;
               --tj, ++ti) {
            if (blocks[tj][ti]) {
              sum += blocks[tj][ti];
              while (sum > S) {
                sum -= blocks[kj][ki];
                --kj;
                ++ki;
              }
              if (sum == S) {
                e += 1 + ti - ki;
                for (int pj = kj, pi = ki; pi <= ti; --pj, ++pi)
                  del[pj][pi] = true;
              }
            } else {
              kj = tj - 1;
              ki = ti + 1;
              sum = 0;
            }
          }
          break;
        case 2:
          // 左上
          for (int tj = HT - 1, ti = i, kj = tj, ki = ti, sum = 0; ti >= 0;
               --tj, --ti) {
            if (blocks[tj][ti]) {
              sum += blocks[tj][ti];
              while (sum > S) {
                sum -= blocks[kj][ki];
                --kj;
                --ki;
              }
              if (sum == S) {
                e += 1 - ti + ki;
                for (int pj = kj, pi = ki; pi >= ti; --pj, --pi)
                  del[pj][pi] = true;
              }
            } else {
              kj = tj - 1;
              ki = ti - 1;
              sum = 0;
            }
          }
          break;
        case 3:
          // 右
          for (int j = 0, kj = j, sum = 0; j < W; ++j) {
            if (blocks[i][j]) {
              sum += blocks[i][j];
              while (sum > S) {
                sum -= blocks[i][kj];
                ++kj;
              }
              if (sum == S) {
                e += 1 + j - kj;
                for (int pj = kj; pj <= j; ++pj) del[i][pj] = true;
              }
            } else {
              kj = j + 1;
              sum = 0;
            }
          }
          break;
        case 4:
          // 右上
          for (int ti = i, tj = 0, ki = ti, kj = tj, sum = 0; ti >= 0 && tj < W;
               --ti, ++tj) {
            if (blocks[ti][tj]) {
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
            } else {
              ki = ti - 1;
              kj = tj + 1;
              sum = 0;
            }
          }
          break;
        case 5:
          // 左上
          for (int ti = i, tj = W - 1, ki = ti, kj = tj, sum = 0;
               ti >= 0 && tj >= 0; --ti, --tj) {
            if (blocks[ti][tj]) {
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
            } else {
              ki = ti - 1;
              kj = tj - 1;
              sum = 0;
            }
          }
          break;
      }
    }
    return e;
  }

  inline void setCheck(bool (&check)[6][HT], int (&task)[HT * W][2], int &size,
                       const int i, const int j) {
    if (blocks[i][j] == OBSTACLE) return;
    if (!check[0][j]) {
      check[0][j] = true;
      task[size][0] = 0;
      task[size][1] = j;
      ++size;
    }
    if (!check[3][i]) {
      check[3][i] = true;
      task[size][0] = 3;
      task[size][1] = i;
      ++size;
    }
    if (HT - i - 1 <= j) {
      if (!check[1][j - (HT - i - 1)]) {
        check[1][j - (HT - i - 1)] = true;
        task[size][0] = 1;
        task[size][1] = j - (HT - i - 1);
        ++size;
      }
    } else {
      if (!check[4][i + j]) {
        check[4][i + j] = true;
        task[size][0] = 4;
        task[size][1] = i + j;
        ++size;
      }
    }
    if (HT - i - 1 < W - j) {
      if (!check[2][j + (HT - i - 1)]) {
        check[2][j + (HT - i - 1)] = true;
        task[size][0] = 2;
        task[size][1] = j + (HT - i - 1);
        ++size;
      }
    } else {
      if (!check[5][i + (W - j - 1)]) {
        check[5][i + (W - j - 1)] = true;
        task[size][0] = 5;
        task[size][1] = i + (W - j - 1);
        ++size;
      }
    }
  }

  int chain(bool (&check)[6][HT], int (&task)[HT * W][2], int &size) {
    bool del[HT][W];
    int score = 0;
    double chain = 1;
    while (true) {
      memset(del, false, sizeof(del));
      int e = setDelete(task, size, del);
      if (e == 0) break;

      memset(check, false, sizeof(check));
      size = 0;
      for (int j = 0; j < W; ++j) {
        for (int i = HT - 1, k = 0; i >= 0 && blocks[i][j]; --i) {
          if (del[i][j]) {
            blocks[i][j] = EMPTY;
            if (k == 0) k = i;
          } else if (k) {
            blocks[k][j] = blocks[i][j];
            setCheck(check, task, size, k, j);
            blocks[i][j] = EMPTY;
            --k;
          }
        }
      }
      chain *= 1.3;
      score += (int)chain * (e / 2);
    }
    return score / 5;
  }

  bool next(const Pack &p, const int w) {
    bool check[6][HT];
    memset(check, false, sizeof(check));
    int task[HT * W][2], size = 0;
    for (int j = 0; j < T; ++j) {
      int h = HT - 1;
      while (blocks[h][w + j]) --h;
      for (int i = T - 1; i >= 0; --i) {
        const int v = p.blocks[i][j];
        if (v) {
          setCheck(check, task, size, h, w + j);
          blocks[h--][w + j] = v;
        }
      }
    }

    obs = chain(check, task, size);

    for (int i = 0; i < W; ++i) {
      if (blocks[T - 1][i]) return false;
    }
    {  // value
      value = 0;
      for (int w = 0; w < W; ++w) {
        int h = HT - 1;
        while (blocks[h][w]) --h;
        for (int b = 1; b < S; ++b) {
          Field f = *this;
          memset(check, false, sizeof(check));
          size = 0;
          f.setCheck(check, task, size, h, w);
          f.blocks[h][w] = b;
          int obs = f.chain(check, task, size);
          if (value < obs) value = obs;
        }
      }
      value <<= 10;
      for (int i = T; i < HT; ++i) {
        for (int j = 0; j < W; ++j) {
          if (blocks[i][j] == OBSTACLE) {
            value += i;
          } else if (blocks[i][j]) {
            value += S - blocks[i][j] + HT - i;
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

inline bool operator<(const Field &left, const Field &right) {
  return left.value < right.value;
}

inline bool operator!=(const Field &left, const Field &right) {
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
  }
  return true;
}

void execute() {
  Pack np[N - turn];
  for (int t = turn; t < N; ++t) {
    np[t - turn] = packs[t];
    if (myObstacle) myObstacle -= np[t - turn].fill(myObstacle);
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
          for (int a = T - 1; a >= 0; --a) {
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

            int tv = c.value + (min(c.obs, target) << 24) - (i << 22) +
                     (max(c.obs - target, 0) << 18);
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
