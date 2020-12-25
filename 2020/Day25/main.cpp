#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

namespace
{
    uint64_t ComputeLoopSize(uint64_t publicKey)
    {
        constexpr uint64_t subjectNumber{7ULL};
        uint64_t value{1};
        uint64_t loopCount{1};
        while (true)
        {
            value *= subjectNumber;
            value %= 20201227;

            if (value == publicKey)
            {
                return loopCount;
            }
            ++loopCount;
        }
    }

    uint64_t EncryptNumber(uint64_t loopCount, uint64_t subjectNumber)
    {
        uint64_t value{1};
        for (auto i = 0ULL; i < loopCount; ++i)
        {
            value *= subjectNumber;
            value %= 20201227;
        }
        return value;
    }

    void Part1()
    {
        std::string cardKeyStr;
        std::getline(std::cin, cardKeyStr);
        uint64_t cardKey = (uint64_t)std::atoi(cardKeyStr.c_str());
        const auto cardKeyLoopCount = ComputeLoopSize(cardKey);

        std::string doorKeyStr;
        std::getline(std::cin, doorKeyStr);
        uint64_t doorKey = (uint64_t)std::atoi(doorKeyStr.c_str());
        const auto doorKeyLoopCount = ComputeLoopSize(doorKey);

        const auto result = EncryptNumber(doorKeyLoopCount, cardKey);
        std::cout << result << std::endl;
    }

    void Part2()
    {
        std::cout << 0 << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}