#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <limits.h>
#include <unordered_map>

using StateHash = std::string;
using vector2D = std::vector<std::vector<int>>;

struct State {
    vector2D board;
    int g;
    int h;
};

struct CompareNode {
    bool operator()(const State& a, const State& b) const {
        return (a.g + a.h) > (b.g + b.h);
    }
};

bool readPuzzle(std::ifstream& file, vector2D& board, int& n)
{
    std::string line;

    // 1️⃣ n 읽기 (주석 스킵)
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        std::stringstream ss(line);
        ss >> n;
        break;
    }

    if (n <= 0)
        return false;

    board.resize(n, std::vector<int>(n));

    // 2️⃣ 퍼즐 읽기
    int row = 0;
    while (row < n && std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        std::stringstream ss(line);
        for (int col = 0; col < n; col++) {
            if (!(ss >> board[row][col]))
                return false;
        }
        row++;
    }

    return row == n;
}

StateHash makeHash(const vector2D& state) {
    StateHash hash;

    for (int i = 0; i < (int)state.size(); i++) {
        for (int j = 0; j < (int)state[i].size(); j++) {
            hash += char(state[i][j] + '0');
            hash += ',';
        }
    }
    return hash;
}

vector2D createSnailGoal(int n) {
    vector2D goal(n, std::vector<int>(n, 0));
    int count = 1;
    int top = 0, bottom = n - 1;
    int left = 0, right = n - 1;

    while (top <= bottom && left <= right) {
        // 윗줄
        for (int j = left; j <= right; ++j) goal[top][j] = count++;
        top++;

        // 오른쪽 열
        for (int i = top; i <= bottom; ++i) goal[i][right] = count++;
        right--;

        // 아랫줄
        if (top <= bottom) {
            for (int j = right; j >= left; --j) goal[bottom][j] = count++;
            bottom--;
        }

        // 왼쪽 열
        if (left <= right) {
            for (int i = bottom; i >= top; --i) goal[i][left] = count++;
            left++;
        }
    }

    // 마지막 칸은 빈 칸
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (goal[i][j] == n * n) {
                goal[i][j] = 0;
            }
        }
    }
    return goal;
}

std::vector<int> findPosition(const vector2D& board, int value) {
    std::vector<int> pos(2);

    for (int i = 0; i < (int)board.size(); ++i) {
        for (int j = 0; j < (int)board[i].size(); ++j) {
            if (board[i][j] == value) {
                pos[0] = i;
                pos[1] = j;
                return pos;
            }
        }
    }

    pos[0] = -1;
    pos[1] = -1;
    return pos;
}

int Manhattan(const vector2D& current, const vector2D& goal) {
    int n = current.size();
    int distance = 0;
    
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            int v = current[x][y];
            if (v == 0) continue;

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (goal[i][j] == v) {
                        distance += abs(x - i) + abs(y - j);
                    }
                }
            }
        }
    }

    return distance;
}

std::vector<vector2D> expand(const vector2D& current)
{
    std::vector<vector2D> nextSteps;
    int n = current.size();
    int zeroX, zeroY;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (current[i][j] == 0) {
                zeroX = i;
                zeroY = j;
            }
        }
    }

    const std::vector<std::pair<int, int>> directions = {
        { -1, 0 }, // 위
        { 1, 0 },  // 아래
        { 0, -1 }, // 왼쪽
        { 0, 1 }   // 오른쪽
    };

    for (const auto& dir : directions) {
        int newX = zeroX + dir.first;
        int newY = zeroY + dir.second;

        if (newX >= 0 && newX < n && newY >= 0 && newY < n) {
            vector2D newStep = current;
            std::swap(newStep[zeroX][zeroY], newStep[newX][newY]);
            nextSteps.push_back(newStep);
        }
    }

    return nextSteps;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    // Open the input file
    const char* input_file = argv[1];

    std::ifstream inputFile(input_file);

    vector2D start;
    int n;

    if (!readPuzzle(inputFile, start, n))
    {
        std::cerr << "Invalid puzzle format\n";
        return 1;
    }

    // Read an integer from the file
    std::cout << "Read integer: " << n << std::endl;
    
    inputFile.close();

    vector2D goal = createSnailGoal(n);

    std::priority_queue<State, std::vector<State>, CompareNode> opened;
    std::unordered_map<StateHash, int> closed;

    // A* algorithm initialization

    opened.push({ start, 0, Manhattan(start, goal) });

    // {
    //     // for debug
    //     for (int i = 0; i < n; ++i) {
    //         for (int j = 0; j < n; ++j) {
    //             std::cout << goal[i][j] << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    // }

    // std::cout << "Initialized opened with the first configuration." << std::endl;

    while (!opened.empty()) {
        State cur = opened.top();
        opened.pop();

        StateHash h = makeHash(cur.board);

        if (closed.count(h) && closed[h] <= cur.g) continue;
        closed[h] = cur.g;

        if (cur.board == goal) {
            std::cout << "Goal reached with cost: " << cur.g << std::endl;
            return 0;
        }

        std::vector<vector2D> next = expand(cur.board);
        for (int i = 0; i < (int)next.size(); i++) {
            State nextState;
            nextState.board = next[i];
            nextState.g = cur.g + 1;
            nextState.h = Manhattan(next[i], goal);

            opened.push(nextState);
        }
    }

    std::cout << "No Solution" << std::endl;
    return 0;
}