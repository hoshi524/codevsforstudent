// #define NDEBUG
#include <bits/stdc++.h>
using namespace std;

static const int W = 10;
static const int H = 16;
static const int HW = H * W;
static const int T = 3;
static const int WT = W + 2;
static const int HT = H + T + 2;
static const int S = 10;
static const int N = 500;
static const int EMPTY = 0;
static const int WALL = -1;
static const int OBSTACLE = S + 1;
static const int target = 80;
static const int tasksize = 70;
static const int minute = 60 * 1000;
#ifdef NDEBUG
static const int node = 1100;  // prod
#else
static const int node = 100;  // test
#endif

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

  pair<int, int> getLeftRight() {
    int left = 0, right = W - T + 1;
    for (int a = 0; a < T; ++a) {
      if (blocks[0][a] | blocks[1][a] | blocks[2][a]) break;
      --left;
    }
    for (int a = T - 1; a >= 0; --a) {
      if (blocks[0][a] | blocks[1][a] | blocks[2][a]) break;
      ++right;
    }
    return make_pair(left, right);
  }
};

class Field {
 public:
  char blocks[HT][WT];
  int pos, rot, obs, value;

  Field() {}
  Field(const Field &x) { memcpy(this, &x, sizeof(x)); }

  void input() {
    memset(blocks, EMPTY, sizeof(blocks));
    for (int i = 0; i < WT; ++i) {
      blocks[0][i] = WALL;
      blocks[HT - 1][i] = WALL;
    }
    for (int i = 0; i < HT; ++i) {
      blocks[i][0] = WALL;
      blocks[i][WT - 1] = WALL;
    }
    for (int i = 0; i < H; ++i) {
      for (int j = 1; j <= W; ++j) {
        int block;
        cin >> block;
        blocks[i + T + 1][j] = block;
      }
    }
    string end;
    cin >> end;
  }

  inline int setDelete(const int (&task)[tasksize][2], const int size,
                       bool (&del)[HT][WT]) {
    assert(size < tasksize);
    int e = 0;
    for (int t = 0; t < size; ++t) {
      int i = task[t][1];
      switch (task[t][0]) {
        case 0:
          // 上
          for (int j = HT - 2, kj = j, sum = 0; blocks[j][i] > 0; --j) {
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
          for (int tj = HT - 2, ti = i, kj = tj, ki = ti, sum = 0;
               blocks[tj][ti] >= 0; --tj, ++ti) {
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
          for (int tj = HT - 2, ti = i, kj = tj, ki = ti, sum = 0;
               blocks[tj][ti] >= 0; --tj, --ti) {
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
          for (int j = 1, kj = j, sum = 0; blocks[i][j] >= 0; ++j) {
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
          for (int ti = i, tj = 1, ki = ti, kj = tj, sum = 0;
               blocks[ti][tj] >= 0; --ti, ++tj) {
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
          for (int ti = i, tj = W, ki = ti, kj = tj, sum = 0;
               blocks[ti][tj] >= 0; --ti, --tj) {
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
    return blocks[i][j] > 0 && v + blocks[i][j] <= S;
  }

  inline void setCheck(bool (&check)[6][HT], int (&task)[tasksize][2],
                       int &size, const int i, const int j, const int v) {
    blocks[i][j] = v;
    if (v == OBSTACLE) return;
    assert(0 < v && v < S);

    int h = HT - i - 2, w = j - 1;
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
      if (h <= w) {
        if (check[1][w - h + 1]) {
          check[1][w - h + 1] = false;
          task[size][0] = 1;
          task[size][1] = w - h + 1;
          ++size;
        }
      } else {
        if (check[4][i + w]) {
          check[4][i + w] = false;
          task[size][0] = 4;
          task[size][1] = i + w;
          ++size;
        }
      }
    }
    if (isValid(v, i - 1, j - 1) || isValid(v, i + 1, j + 1)) {
      if (h < W - w) {
        if (check[2][j + h]) {
          check[2][j + h] = false;
          task[size][0] = 2;
          task[size][1] = j + h;
          ++size;
        }
      } else {
        if (check[5][i + (W - j)]) {
          check[5][i + (W - j)] = false;
          task[size][0] = 5;
          task[size][1] = i + (W - j);
          ++size;
        }
      }
    }
  }

  int chain(bool (&check)[6][HT], int (&task)[tasksize][2], int &size) {
    bool del[HT][WT];
    int score = 0;
    double chain = 1;
    memset(del, false, sizeof(del));
    while (true) {
      int e = setDelete(task, size, del);
      if (e == 0) break;

      memset(check, true, sizeof(check));
      size = 0;
      for (int j = 1; j <= W; ++j) {
        for (int i = HT - 2, k = 0; blocks[i][j] > 0; --i) {
          if (del[i][j]) {
            del[i][j] = false;
            blocks[i][j] = EMPTY;
            if (k == 0) k = i;
          } else if (k) {
            setCheck(check, task, size, k, j, blocks[i][j]);
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
    assert(blocks[i][j] == 0);
    int x = 0, z[W];
    for (int d = -1; d <= 1; ++d) {
      int a = i + d, b = j - 1, sum = 0, size = 0;
      while (blocks[a][b] > 0 && sum + blocks[a][b] < S) {
        sum += blocks[a][b];
        x |= 1 << (S - sum);
        z[size++] = sum;
        a += d;
        b -= 1;
      }
      a = i - d, b = j + 1, sum = 0;
      while (blocks[a][b] > 0 && sum + blocks[a][b] < S) {
        sum += blocks[a][b];
        x |= 1 << (S - sum);
        for (int k = 0; k < size && sum + z[k] < S; ++k) {
          x |= 1 << (S - sum - z[k]);
        }
        a -= d;
        b += 1;
      }
    }
    return x;
  }

  bool next(const Pack &p, const int w) {
    bool check[6][HT];
    memset(check, true, sizeof(check));
    int task[tasksize][2], size = 0;
    for (int j = 0; j < T; ++j) {
      int h = HT - 2;
      while (blocks[h][w + j + 1]) --h;
      for (int i = T - 1; i >= 0; --i) {
        const int v = p.blocks[i][j];
        if (v) {
          setCheck(check, task, size, h, w + j + 1, v);
          --h;
        }
      }
    }

    obs = chain(check, task, size);

    for (int i = 1; i <= W; ++i) {
      if (blocks[T][i]) return false;
    }
    return true;
  }

  void calcValue() {
    value = 0;
    int highRank[W];
    bool check[6][HT];
    int task[tasksize][2], size = 0;
    for (int w = 1; w <= W; ++w) {
      int h = HT - 2;
      while (blocks[h][w]) --h;
      highRank[w - 1] = (h << 4) + w;
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
          f.setCheck(check, task, size, h, w, b);
          int obs = f.chain(check, task, size);
          if (maxobs < obs) maxobs = obs;
        }
      }
    }
    value += maxobs << 4;
  }

  int calcDepth() {
    int block = 0, allblock = 0;
    for (int i = 1; i + 1 < HT; ++i) {
      for (int j = 1; j <= W; ++j) {
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
    for (int i = 0; i < HT; ++i) {
      for (int j = 0; j < WT; ++j) {
        cerr << (int)blocks[i][j] << " ";
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
    for (int j = 0; j < WT; ++j) {
      if (left.blocks[i][j] != right.blocks[i][j]) return true;
    }
  }
  return false;
}

inline bool operator==(const Field &left, const Field &right) {
  return !(left != right);
}

namespace State {
int turn;
int time;
int prevturn = INT_MIN;
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
  assert(turn != prevturn + 1 || prev == myField);
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
    for (int i = 0; i <= depth; ++i) {
      for (int j = 0; j < 10 && !search[i].empty(); ++j) {
        ++n;
        Field field = search[i].top();
        search[i].pop();
        for (int r = 0; r < 4; ++r) {
          auto lr = np[i].getLeftRight();
          for (int w = lr.first, ws = lr.second; w < ws; ++w) {
            Field c = field;
            if (c.next(np[i], w)) {
              if (obs <= c.obs) return false;
              if (i < depth) {
                c.calcValue();
                search[i + 1].push(c);
              }
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
      auto lr = np[0].getLeftRight();
      for (int w = lr.first, ws = lr.second; w < ws; ++w) {
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
    for (int i = 0; i <= depth; ++i) {
      for (int j = 0; j < 10 && !search[i].empty(); ++j) {
        ++n;
        Field field = search[i].top();
        search[i].pop();
        for (int r = 0; r < 4; ++r) {
          auto lr = np[i].getLeftRight();
          for (int w = lr.first, ws = lr.second; w < ws; ++w) {
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
              } else if (i < depth) {
                c.calcValue();
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
    prevturn = turn;
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
