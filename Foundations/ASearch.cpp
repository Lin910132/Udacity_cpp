#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<queue>
#include<stack>
#include<bits/stdc++.h>

using namespace std;

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

string CellString(const State& state){
    switch (state)
    {
    case State::kObstacle:
        return "⛰️  ";
    case State::kPath:
        return "🚗 ";
    case State::kStart:
        return "🚦  ";
    case State::kFinish:
        return "🏁 ";
    default:
        return "0  ";
    }
}

vector<State> ParseLine(const string& line){
    istringstream row(line);
    vector<State> parsedLine;

    int x;
    char c;
    while (row >> x >> c){
        parsedLine.push_back(x == 0? State::kEmpty : State::kObstacle);
    }

    return parsedLine;
}

vector<vector<State>> ReadBoard(const string& path){
    ifstream myfile(path);
    vector<vector<State>> board;

    if (myfile){
        string line;
        while (getline(myfile, line)){
            board.push_back(ParseLine(line));
        }
    }

    return board;
}

void PrintBoard(const vector<vector<State>>& board){
    for (auto line: board){
        for (auto x: line){
            cout << CellString(x);
        }

        cout << endl;
    }
}

bool Compare(const vector<int> a, const vector<int> b) {
  int f1 = a[2] + a[3]; // f1 = g1 + h1
  int f2 = b[2] + b[3]; // f2 = g2 + h2
  return f1 > f2; 
}

/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

int Heuristic(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

void AddToOpen(int x, int y, int g, int h, vector<vector<int>>& openList, vector<vector<State>> &grid){
    openList.push_back(vector<int> {x, y, g, h});
    grid[x][y] = State::kClosed;
}

vector<vector<State>> Search(vector<vector<State>> board, int init[], int goal[]){
    constexpr int dx[4]{0, 1, 0, -1};
    constexpr int dy[4]{1, 0, -1, 0};

    vector<vector<int>> openList;
    int x = init[0];
    int y = init[1];
    int g = 0;
    int h = Heuristic(init[0], init[1], goal[0], goal[1]);
    AddToOpen(x, y, g, h, openList, board);

    while (!openList.empty()){
        CellSort(&openList);
        auto cur = openList.back();
        openList.pop_back();

        x = cur[0];
        y = cur[1];
        board[x][y] = State::kPath;

        if (x == goal[0] && y == goal[1]){
            return board;
        }

        for (int i = 0; i < 4; i++){
            int nextX = x + dx[i];
            int nextY = y + dy[i];

            if (nextX < 0 || nextX >= board.size() || nextY < 0 || nextY >= board.front().size()){
                continue;
            }

            if (board[nextX][nextY] != State::kEmpty){
                continue;
            }

            AddToOpen(nextX, nextY, cur[2] + 1, Heuristic(nextX, nextY, goal[0], goal[1]), openList, board);
        }
    }

    cout << "No path found!" << endl;
    return std::vector<vector<State>> {};
}

int main(){
    int init[2]{0, 0};
    int goal[2]{4, 5};

    auto board = ReadBoard("./Foundations/1.board");
    auto solution = Search(board, init, goal);
    solution[0][0] = State::kStart;
    solution[4][5] = State::kFinish;
    
    PrintBoard(solution);
}