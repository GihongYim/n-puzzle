#include <iostream>
#include <fstream>
#include <vector>

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

vector2D selectBestNode(const std::vector<vector2D>& opened)
{
    // n puzzle specific logic to select the best node
    // get


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
            std::cout << "Read number: " << numbers[i][j] << std::endl;
        }
    }

    inputFile.close();

    std::vector<vector2D> opened;
    opened.push_back(numbers);

    std::vector<vector2D> closed;

    vector2D goalState; // Define the goal state as needed
    {
        int count = 1;
        for (int i = 0; i < n; ++i) {
            std::vector<int> row;
            for (int j = 0; j < n; ++j) {
                if (i == n - 1 && j == n - 1) {
                    row.push_back(0); // Assuming 0 represents the empty tile
                } else {
                    row.push_back(count++);
                }
            }
            goalState.push_back(row);
        }
    }

    bool bSuccess = false;
    std::cout << "Initialized opened with the first configuration." << std::endl;

    while (!opened.empty() && !bSuccess)
    {
        vector2D e = selectBestNode(opened);

        opened.pop_back();
        closed.push_back(e);
    }

    return 0;
}