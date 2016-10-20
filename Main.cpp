#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

static const int W = 10;
static const int H = 16;
static const int T = 3;
static const int S = 10;
static const int N = 500;
static const int EMPTY = 0;
static const int OBSTACLE_BLOCK = 11;

mt19937 MT(8410325);

int randInt(int from, int to) {
  uniform_int_distribution<int> rand(from, to - 1);
  return rand(MT);
}

class Field {
public:
  vector<vector<int>> blocks;

  Field() : blocks(vector<vector<int>>(H, vector<int>(W, 0))) {}

  void input() {
    blocks.clear();
    for (int i = 0; i < H; i++) {
      vector<int> row;
      for (int j = 0; j < W; j++) {
        int block;
        cin >> block;
        row.push_back(block);
      }
      blocks.push_back(row);
    }
    string endStr;
    cin >> endStr;
  }

  void show() {
    for (int i = 0; i < H; i++) {
      for (int j = 0; j < W; j++) {
        cerr << blocks[i][j] << " ";
      }
      cerr << endl;
    }
    cerr << "====" << endl;
    cerr.flush();
  }
};

class Pack {
public:
  vector<vector<int>> blocks;
  int rotateCnt;

  Pack(vector<vector<int>> blocks) : blocks(blocks), rotateCnt(0) {}

  static Pack inputFirst() {
    vector<vector<int>> blocks;
    for (int i = 0; i < T; i++) {
      vector<int> row;
      for (int j = 0; j < T; j++) {
        int block;
        cin >> block;
        row.push_back(block);
      }
      blocks.push_back(row);
    }
    string endStr;
    cin >> endStr;
    return Pack(blocks);
  }

  int fillWithObstacle(int obstacleCnt) {
    int fillCnt = 0;
    for (int i = 0; i < T; i++) {
      for (int j = 0; j < T; j++) {
        if (fillCnt < obstacleCnt && blocks[i][j] == EMPTY) {
          blocks[i][j] = OBSTACLE_BLOCK;
          fillCnt++;
        }
        if (fillCnt >= obstacleCnt) {
          return fillCnt;
        }
      }
    }
    return fillCnt;
  }

  void rotate(int rot) {
    rotateCnt += rot;
    rotateCnt %= 4;

    for (int r = 0; r < rot; r++) {
      vector<vector<int>> tmp = blocks;
      for (int i = 0; i < T; i++) {
        for (int j = 0; j < T; j++) {
          blocks[j][T - 1 - i] = tmp[i][j];
        }
      }
    }
  }

  void show() {
    for (int i = 0; i < T; i++) {
      for (int j = 0; j < T; j++) {
        cerr << blocks[i][j] << " ";
      }
      cerr << endl;
    }
    cerr.flush();
  }
};

class State {
public:
  int turn;
  int remTime;
  vector<Pack> packs;
  Field myField;
  Field enemyField;
  int myObstacle;
  int enemyObstacle;

  State() {}

  static State inputFirst() {
    State st;
    int w, h, t, s, n;
    cin >> w >> h >> t >> s >> n;
    st.packs.clear();
    for (int i = 0; i < N; i++) {
      Pack pack = Pack::inputFirst();
      st.packs.push_back(pack);
    }
    st.myField = Field();
    st.enemyField = Field();
    return st;
  }

  void executeTurn() {
    cin >> turn >> remTime;
    cin >> myObstacle;
    myField.input();
    cin >> enemyObstacle;
    enemyField.input();

    int rot = randInt(0, 4);

    myObstacle -= packs[turn].fillWithObstacle(myObstacle);
    packs[turn].rotate(rot);

    int pos = randInt(0, W - T + 1);

    cout << pos << " " << packs[turn].rotateCnt << endl;
    cout.flush();
  }
};

int main() {
  cout << "hoshi524" << endl;
  cout.flush();

  State state = State::inputFirst();
  while (true) {
    state.executeTurn();
  }
}
