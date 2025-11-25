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
    goal[n-1][n-1] = 0;
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

int getManhattanDistance(const vector2D& current, const vector2D& goal) {
    int distance = 0;
    int n = current.size();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int value = current[i][j];
            if (value != 0) { 
                std::vector<int> pos = findPosition(goal, value);
                int targetX = pos[0];
                int targetY = pos[1];
    
                distance += abs(i - targetX) + abs(j - targetY);
            }
        }
    }

    return distance;
}

vector2D selectBestNode(const std::vector<vector2D>& opened)
{
    int bestIndex = 0;
    int minDistance = INT_MAX;

    const vector2D& goal = createSnailGoal(opened[0].size());
    
    for (int i = 0; i < (int)opened.size(); ++i) {
        int distance = getManhattanDistance(opened[i], goal);
        if (distance < minDistance) {
            minDistance = distance;
            bestIndex = i;
        }
    }

    return opened[bestIndex];

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
    std::vector<vector2D> opened;
    opened.push_back(numbers);

    std::vector<vector2D> closed;

    vector2D goalState = createSnailGoal(n);

    bool bSuccess = false;
    std::cout << "Initialized opened with the first configuration." << std::endl;

    while (!opened.empty() && !bSuccess)
    {
        vector2D e = selectBestNode(opened);

        // Check Next Step
        if (e == goalState)
        {
            bSuccess = true;
            std::cout << "Goal state reached!" << std::endl;
            break;
        }

        closed.push_back(e);
    }

    return 0;
}