#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h>

typedef std::vector<std::vector<int>> vector2D;

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

int getManhattanDistance(const std::pair<vector2D, int>& current, const vector2D& goal) {
    int n = current.first.size();
    int distance = 0;
    for (int i = 0; i < (int)current.first.size(); i++) {
        for (int x = 0; x < n; x++) {
            for (int y = 0; y < n; y++) {
                int value = current.first[x][y];
                distance += (value != 0) ? abs(x - findPosition(goal, value)[0]) + abs(y - findPosition(goal, value)[1]) : 0;
            }
        }
        
    }

    return distance;
}

std::pair<vector2D, int> selectBestNode(std::vector<std::pair<vector2D, int>>& opened)
{
    int bestIndex = 0;
    int minDistance = INT_MAX;

    const vector2D& goal = createSnailGoal(opened[0].first.size());
    
    for (int i = 0; i < (int)opened.size(); ++i) {
        int distance = getManhattanDistance(opened[i], goal);
        if (distance < minDistance) {
            minDistance = distance;
            bestIndex = i;
        }
    }

    std::pair<vector2D, int> bestNode = opened[bestIndex];
    opened.erase(opened.begin() + bestIndex);

    return bestNode;
}

std::vector<vector2D> getNextSteps(const vector2D& current)
{
    std::vector<vector2D> nextSteps;
    int n = current.size();
    int zeroX, zeroY;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
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

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open file " << input_file << std::endl;
        return 1;
    }

    // Read an integer from the file
    int n;
    inputFile >> n;

    std::cout << "Read integer: " << n << std::endl;

    vector2D numbers(n);
    for (int i = 0; i < n; i++) {
        numbers[i].resize(n);
        for (int j = 0; j < n; j++) {
            inputFile >> numbers[i][j];
        }
    }

    inputFile.close();

    // A* alhorithm initialization
    std::vector<std::pair<vector2D, int>> opened;
    opened.push_back(std::make_pair(numbers, 0));

    std::vector<std::pair<vector2D, int>> closed;

    vector2D goalState = createSnailGoal(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << goalState[i][j] << " ";
        }
        std::cout << std::endl;
    }

    bool bSuccess = false;
    std::cout << "Initialized opened with the first configuration." << std::endl;

    while (!opened.empty() && !bSuccess)
    {
        // Select Best Node
        std::pair<vector2D, int> e = selectBestNode(opened);

        // Check Next Step
        if (e.first == goalState)
        {
            bSuccess = true;
            std::cout << "Goal state reached!" << std::endl;
            std::cout << "Cost: " << e.second << std::endl;
            break;
        }

        closed.push_back(e);

        std::vector<vector2D> nextSteps;
    
        // 다음 방향 생성
        nextSteps = getNextSteps(e.first);

        std::cout << "Expanding node with cost " << e.second << ", generated " << nextSteps.size() << " next steps." << std::endl;

        // 다음 방향들 중에서 opened와 closed에 없는 것들만 추가
        for (const auto& step : nextSteps) {
            bool inClosed = false;
            for (const auto& closedState : closed) {
                if (step == closedState.first) {
                    inClosed = true;
                    break;
                }
            }

            if (inClosed) {
                continue;
            }

            bool inOpened = false;
            for (const auto& openedState : opened) {
                if (step == openedState.first) {
                    inOpened = true;
                    break;
                }
            }

            if (!inOpened && !inClosed) {
                opened.push_back(std::make_pair(step, e.second + 1));
            }
        }
        std::cout << "Opened size: " << opened.size() << ", Closed size: " << closed.size() << std::endl;
    }

    return 0;
}