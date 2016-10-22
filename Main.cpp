#include <bits/stdc++.h>
using namespace std;

static const int W = 10;
static const int H = 16;
static const int T = 3;
static const int S = 10;
static const int N = 500;
static const int EMPTY = 0;
static const int OBSTACLE = S + 1;
static const int target = 10;
static const int dx[4] = {1, 1, 0, 1};
static const int dy[4] = {0, 1, 1, -1};

class Pack {
 public:
  vector<vector<int>> blocks;

  Pack(vector<vector<int>> blocks) : blocks(blocks) {}
  Pack(const Pack &p) : blocks(p.blocks) {}

  static Pack inputFirst() {
    vector<vector<int>> blocks(T, vector<int>(T, 0));
    for (int i = 0; i < T; i++) {
      for (int j = 0; j < T; j++) {
        int block;
        cin >> block;
        blocks[i][j] = block;
      }
    }
    string endStr;
    cin >> endStr;
    return Pack(blocks);
  }

  int fill(const int obstacle) {
    if (obstacle == 0) return 0;
    int x = 0;
    for (int i = 0; i < T; i++) {
      for (int j = 0; j < T; j++) {
        if (blocks[i][j] == EMPTY) {
          blocks[i][j] = OBSTACLE;
          if (++x >= obstacle) return x;
        }
      }
    }
    return x;
  }

  void rotate() {
    vector<vector<int>> tmp = blocks;
    for (int i = 0; i < T; i++) {
      for (int j = 0; j < T; j++) {
        blocks[j][T - 1 - i] = tmp[i][j];
      }
    }
  }
};

class Field {
 public:
  vector<vector<int>> blocks;
  int pos, rot, chain, maxchain, value, prev;

  Field()
      : blocks(vector<vector<int>>(H + T, vector<int>(W, 0))), maxchain(0) {}
  Field(const Field &x)
      : blocks(x.blocks),
        pos(x.pos),
        rot(x.rot),
        chain(x.chain),
        maxchain(x.maxchain),
        value(x.value),
        prev(x.prev) {}

  void input() {
    for (int i = 0; i < H; i++) {
      for (int j = 0; j < W; j++) {
        int block;
        cin >> block;
        blocks[i + T][j] = block;
      }
    }
    string endStr;
    cin >> endStr;
  }

  void fall(const int w, const int v) {
    for (int h = H + T - 1; h >= 0; --h) {
      if (blocks[h][w] == EMPTY) {
        blocks[h][w] = v;
        return;
      }
    }
  }

  inline bool in(const int h, const int w) {
    return 0 <= h && h < H + T && 0 <= w && w < W;
  }

  bool next(const Pack &p, const int w) {
    for (int i = T - 1; i >= 0; --i) {
      for (int j = 0; j < T; ++j) {
        int v = p.blocks[i][j];
        if (v != EMPTY) fall(w + j, v);
      }
    }
    int del[H + T][W];
    chain = 0;
    while (true) {
      int d = 1;
      memset(del, 0, sizeof(del));
      for (int i = 0; i < H + T; ++i) {
        for (int j = 0; j < W; ++j) {
          if (blocks[i][j] != EMPTY) {
            for (int k = 0; k < 4; ++k) {
              int sum = 0;
              for (int a = i, b = j;
                   in(a, b) && blocks[a][b] != EMPTY && sum < S;
                   a += dy[k], b += dx[k]) {
                sum += blocks[a][b];
              }
              if (sum != S) continue;
              sum = 0;
              d = 0;
              for (int a = i, b = j;
                   in(a, b) && blocks[a][b] != EMPTY && sum < S;
                   a += dy[k], b += dx[k]) {
                sum += blocks[a][b];
                del[a][b] = 1;
              }
            }
          }
        }
      }
      if (d) break;
      ++chain;
      for (int j = 0; j < W; ++j) {
        for (int i = H + T - 1, k = -1; i >= 0; --i) {
          if (blocks[i][j] == EMPTY) break;
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
    if (maxchain < chain) maxchain = chain;
    calcValue();
    return true;
  }

  vector<Field> child(Pack &p) {
    vector<Field> child;
    for (int r = 0; r < 4; ++r) {
      for (int w = 0; w < W - T + 1; ++w) {
        Field c = *this;
        if (c.next(p, w)) {
          c.rot = r;
          c.pos = w;
          child.push_back(c);
        }
      }
      p.rotate();
    }
    return child;
  }

  void calcValue() {
    value = maxchain < target ? 0 : maxchain * 0xff;
    int x = 0;
    for (int i = T; i < H + T; ++i) {
      for (int j = 0; j < W; ++j) {
        if (blocks[i][j] != EMPTY) {
          value += S - blocks[i][j] - 1;
          if (++x >= 120) value -= S;
        }
      }
    }
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

namespace State {
int turn;
int time;
vector<Pack> packs;
Field myField;
Field opField;
int myObstacle;
int opObstacle;

void input() {
  int w, h, t, s, n;
  cin >> w >> h >> t >> s >> n;
  packs.clear();
  for (int i = 0; i < N; ++i) {
    Pack pack = Pack::inputFirst();
    packs.push_back(pack);
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

  vector<Pack> np;
  for (int t = turn; t < N; ++t) {
    Pack p = packs[t];
    myObstacle -= p.fill(myObstacle);
    np.push_back(p);
  }

  const int depth = 5;
  vector<vector<Field>> search(depth, vector<Field>());
  search[0].push_back(myField);
  for (int i = 0, is = min(depth - 1, N - turn); i < is; ++i) {
    for (int j = 0, js = min(200, (int)search[i].size()); j < js; ++j) {
      vector<Field> x = search[i][j].child(np[i]);
      for (int k = 0; k < x.size(); ++k) x[k].prev = j;
      search[i + 1].insert(search[i + 1].end(), x.begin(), x.end());
    }
    sort(search[i + 1].begin(), search[i + 1].end());
  }

  for (int i = depth - 1, index = -1; i > 0; --i) {
    if (index == -1) {
      if (search[i].size() > 0) index = 0;
    } else {
      index = search[i + 1][index].prev;
    }
    if (i == 1) {
      if (index == -1) {
        cout << "9 9\n";
      } else {
        cout << search[i][index].pos << " " << search[i][index].rot << endl;
      }
      break;
    }
  }
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
