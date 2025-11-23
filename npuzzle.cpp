#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
    // read input from file
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (inputFile.fail())
    {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    // read num of integers
    int n;

    inputFile >> n;
    if (inputFile.fail())
    {
        std::cerr << "Error: Could not read integer from file." << std::endl;
        return 1;
    }

    // read n + 1 integers
    std::vector<int> numbers(n);

    for (int i = 0; i < n; ++i)
    {
        inputFile >> numbers[i];
        if (inputFile.fail())
        {
            std::cerr << "Error: Could not read integer from file." << std::endl;
            return 1;
        }
    }

    

    return 0;
}