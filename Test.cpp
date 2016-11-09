#include <bits/stdc++.h>
using namespace std;

static const int W = 10;
static const int H = 16;
static const int HW = H * W;
static const int T = 3;
static const int HT = H + T;
static const int S = 10;
static const int N = 500;
static const int EMPTY = 0;
static const int OBSTACLE = S + 1;
static const int target = 80;
static const int tasksize = 70;
static const int minute = 60 * 1000;
// static const int node = 1100;  // prod
static const int node = 200;  // test

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

  inline int setDelete(const int (&task)[tasksize][2], const int size,
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

  inline bool isValid(const int v, const int i, const int j) {
    return 0 <= i && i < HT && 0 <= j && j < W && blocks[i][j] &&
           v + blocks[i][j] <= S;
  }

  inline void setCheck(bool (&check)[6][HT], int (&task)[tasksize][2],
                       int &size, const int i, const int j) {
    int v = blocks[i][j];
    if (v == OBSTACLE) return;
    if (check[0][j] && isValid(v, i + 1, j)) {
      check[0][j] = false;
      task[size][0] = 0;
      task[size][1] = j;
      ++size;
    }
    if (check[3][i] && (isValid(v, i, j - 1) || isValid(v, i, j + 1))) {
      check[3][i] = false;
      task[size][0] = 3;
      task[size][1] = i;
      ++size;
    }
    if (isValid(v, i + 1, j - 1) || isValid(v, i - 1, j + 1)) {
      if (HT - i - 1 <= j) {
        if (check[1][j - (HT - i - 1)]) {
          check[1][j - (HT - i - 1)] = false;
          task[size][0] = 1;
          task[size][1] = j - (HT - i - 1);
          ++size;
        }
      } else {
        if (check[4][i + j]) {
          check[4][i + j] = false;
          task[size][0] = 4;
          task[size][1] = i + j;
          ++size;
        }
      }
    }
    if (isValid(v, i - 1, j - 1) || isValid(v, i + 1, j + 1)) {
      if (HT - i - 1 < W - j) {
        if (check[2][j + (HT - i - 1)]) {
          check[2][j + (HT - i - 1)] = false;
          task[size][0] = 2;
          task[size][1] = j + (HT - i - 1);
          ++size;
        }
      } else {
        if (check[5][i + (W - j - 1)]) {
          check[5][i + (W - j - 1)] = false;
          task[size][0] = 5;
          task[size][1] = i + (W - j - 1);
          ++size;
        }
      }
    }
  }

  int chain(bool (&check)[6][HT], int (&task)[tasksize][2], int &size) {
    bool del[HT][W];
    int score = 0;
    double chain = 1;
    memset(del, false, sizeof(del));
    while (true) {
      int e = setDelete(task, size, del);
      if (e == 0) break;

      memset(check, true, sizeof(check));
      size = 0;
      for (int j = 0; j < W; ++j) {
        for (int i = HT - 1, k = 0; i >= 0 && blocks[i][j]; --i) {
          if (del[i][j]) {
            del[i][j] = false;
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
      score += (int)chain * (e >> 1);
    }
    return score / 5;
  }

  int chainBit(const int i, const int j) {
    int x = 0;
    for (int d = -1; d <= 1; ++d) {
      int li = i, lj = j, ri = i, rj = j, sum = 0;
      while (0 <= lj - 1 && 0 <= li + d && li + d < HT &&
             blocks[li + d][lj - 1] && sum + blocks[li + d][lj - 1] < S) {
        li += d;
        lj -= 1;
        sum += blocks[li][lj];
        x |= 1 << (S - sum);
      }
      while (rj + 1 < W && 0 <= ri - d && ri - d < HT &&
             blocks[ri - d][rj + 1] && blocks[ri - d][rj + 1] < OBSTACLE) {
        ri -= d;
        rj += 1;
        sum += blocks[ri][rj];
        if (sum < S) {
          x |= 1 << (S - sum);
        } else {
          while (lj < j) {
            sum -= blocks[li][lj];
            li -= d;
            lj += 1;
            if (sum < S) break;
          }
          if (sum < S) {
            x |= 1 << (S - sum);
          } else {
            break;
          }
        }
      }
    }
    return x;
  }

  bool next(const Pack &p, const int w) {
    bool check[6][HT];
    memset(check, true, sizeof(check));
    int task[tasksize][2], size = 0;
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
    if (obs) return true;
    {  // value
      value = 0;
      int highRank[W];
      for (int w = 0; w < W; ++w) {
        int h = HT - 1;
        while (blocks[h][w]) --h;
        highRank[w] = (h << 4) + w;
        value += HT - 1 - h;
      }
      sort(highRank, highRank + W, greater<int>());

      int maxobs = 0;
      for (int i = 0; i < (W * 2 / 3); ++i) {
        int w = highRank[i] & 0xf;
        int h = highRank[i] >> 4;
        int bit = chainBit(h, w);
        for (int b = 1; b < S; ++b) {
          if (bit & (1 << b)) {
            Field f = *this;
            memset(check, true, sizeof(check));
            size = 0;
            f.setCheck(check, task, size, h, w);
            f.blocks[h][w] = b;
            int obs = f.chain(check, task, size);
            if (maxobs < obs) maxobs = obs;
          }
        }
      }
      value += maxobs << 4;
    }
    return true;
  }

  int calcDepth() {
    int block = 0, allblock = 0;
    for (int i = 0; i < HT; ++i) {
      for (int j = 0; j < W; ++j) {
        if (blocks[i][j] && blocks[i][j] < OBSTACLE) ++block;
        if (blocks[i][j]) ++allblock;
      }
    }
    constexpr int maxdepth = 16;
    constexpr int mindepth = 5;
    int depth = maxdepth - block / 6;
    if (depth > (HW - allblock) / 3) depth = (HW - allblock) / 3;
    if (depth < mindepth) depth = mindepth;
    return depth;
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

bool checkOpp(const int obs) {
  Pack np[N - turn];
  for (int t = turn; t < N; ++t) {
    np[t - turn] = packs[t];
    if (opObstacle) opObstacle -= np[t - turn].fill(opObstacle);
  }
  for (int t = turn + 1, o = obs; t < N; ++t) {
    if (o) o -= np[t - turn].fill(o);
  }
  int depth = min(opField.calcDepth(), N - turn);
  priority_queue<Field> search[depth + 1];
  search[0].push(opField);
  for (int n = 0; n < (node << 1); ++n) {
    for (int i = 0; i < depth; ++i) {
      for (int j = 0; j < 10 && !search[i].empty(); ++j) {
        ++n;
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
              if (obs <= c.obs) return false;
              search[i + 1].push(c);
            }
          }
          np[i].rotate();
        }
      }
    }
  }
  return true;
}

void execute() {
  Pack np[N - turn];
  for (int t = turn, o = myObstacle; t < N; ++t) {
    np[t - turn] = packs[t];
    if (o) o -= np[t - turn].fill(o);
  }
  {
    int pos, rot, obs = 0;
    for (int r = 0; r < 4; ++r) {
      int left = 0, right = W - T + 1;
      {
        int(&p)[T][T] = np[0].blocks;
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
        Field c = myField;
        if (c.next(np[0], w)) {
          if (obs < c.obs) {
            obs = c.obs;
            pos = w;
            rot = r;
          }
        }
      }
      np[0].rotate();
    }
    if (obs > target && obs > myObstacle && checkOpp(obs - myObstacle)) {
      printf("%d %d\n", pos, rot);
      cerr << "stare turn : obs  " << turn << " : " << obs << endl;
      return;
    }
  }
  int value = INT_MIN, pos, rot, ti = -1, obs;
  int depth = min(myField.calcDepth(), N - turn), n;
  priority_queue<Field> search[depth + 1];
  search[0].push(myField);
  for (n = 0; n < (node << (time > minute ? 2 : 0)); ++n) {
    for (int i = 0; i < depth; ++i) {
      for (int j = 0; j < 10 && !search[i].empty(); ++j) {
        ++n;
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
              if (c.obs) {
                int tv = (c.obs << 1) - i;
                if (value < tv) {
                  value = tv;
                  pos = c.pos;
                  rot = c.rot;
                  obs = c.obs;
                  ti = i;
                }
              } else {
                search[i + 1].push(c);
              }
            }
          }
          np[i].rotate();
        }
      }
    }
  }
  printf("%d %d\n", pos, rot);
  // cerr << n << endl;
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
