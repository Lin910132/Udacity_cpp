#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<queue>

using namespace std;

enum class State{
    kEmpty, kObstacle
};

vector<State> ParseLine(const string& line){
    vector<State> row;
    istringstream s(line);
    int x;
    char c;

    while (s >> x >> c){
        row.push_back(x == 0? State::kEmpty : State::kObstacle);
    }

    return row;
}

vector<vector<State>> ReadBoard(const string& path){
    ifstream myFile(path);
    vector<vector<State>> board;

    if (myFile){
        string line;
        while (getline(myFile, line)){
            board.push_back(ParseLine(line));
        }
    }

    return board;
}

void PrintBoard(const vector<vector<State>>& board){
    for (auto line: board){
        for (auto x: line){
            switch (x)
            {
            case State::kObstacle:
                cout << "⛰️ ";
                break;
            
            default:
                cout << "0 ";
                break;
            }
        }

        cout << endl;
    }
}

int Bfs(const vector<vector<State>>& board, const int start[], const int end[]){
    if (start[0] == end[0] && start[1] == end[1]){
        return 0;
    }

    const int dx[4]{0, 1, 0, -1};
    const int dy[4]{1, 0, -1, 0};

    int m = board.size();
    int n = board[0].size();
    bool visited[m][n] = {false};
    
    queue<vector<int>> q;
    q.push(vector<int>{start[0], start[1]});

    int steps = 0;
    while (!q.empty()){
        int len = q.size();

        for (int i = 0; i < len; i++){
            auto cur = q.front();
            q.pop();

            for (int k = 0; k < sizeof(dx); k++){
                int x = cur[0] + dx[k];
                int y = cur[1] + dy[k];

                if (x == end[0] && y == end[1]){
                    return steps + 1;
                }

                if (x < 0 || x >= m || y < 0 || y > m || board[x][y] == State::kObstacle || visited[x][y]){
                    continue;
                }

                visited[x][y] = true;
                q.push(vector<int> {x, y});
            }
        }

        steps++;
    }

    return -1;
}

int main(){
    auto board = ReadBoard("./Foundations/1.board");
    int start[2]{0, 0};
    int end[2]{4, 5};
    int shortestStep = Bfs(board, start, end);
    //PrintBoard(board);
    cout << shortestStep << endl;
}