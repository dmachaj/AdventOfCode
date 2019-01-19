#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <set>
#include <string>
#include <vector>

int main()
{
    std::string inputStr;
    std::vector<int64_t> inputSequence{};
    while (std::getline(std::cin, inputStr))
    {
        inputSequence.push_back(std::atoi(inputStr.c_str()));
    }

#if 0
    std::cerr << "Input sequence is ";
    for (auto input : inputSequence)
    {
        std::cerr << input << ", ";
    }
    std::cerr << std::endl;
#endif

    int64_t runningTotal{};
    uint64_t inputSequenceIndex{};
    std::set<int64_t> seenNumbers{0};
    while (true)
    {
        runningTotal += inputSequence[inputSequenceIndex];
        // std::cerr << "Read " << inputSequence[inputSequenceIndex] << " running total is " << runningTotal << std::endl;

        if (seenNumbers.find(runningTotal) != seenNumbers.end())
        {
            std::cout << runningTotal << std::endl;
            break;
        }
        seenNumbers.emplace(runningTotal);
        inputSequenceIndex = ++inputSequenceIndex % inputSequence.size();
    }
    return 0;
}