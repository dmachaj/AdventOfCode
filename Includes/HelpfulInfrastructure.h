#pragma once
#include <string>

inline bool RunPart1()
{
    std::string partToRun;
    {
        char* partToRunRaw{};
        size_t partToRunSize{};
        getenv_s(&partToRunSize, nullptr, 0, "PART_TO_RUN");
        if (partToRunSize != 0)
        {
            partToRunRaw = new char[partToRunSize];
            getenv_s(&partToRunSize, partToRunRaw, partToRunSize, "PART_TO_RUN");
            partToRun = partToRunRaw;
        }
    }
    return (partToRun != "Part2"); // When in doubt, run part 1
}

inline uint64_t LowestCommonDenominator(uint64_t first, uint64_t second)
{
    while (true)
    {
        if (first == 0) { return second; }
        second %= first;
        if (second == 0) { return first; }
        first %= second;
    }
}

inline uint64_t LeastCommonMultiple(uint64_t first, uint64_t second)
{
    return (first * second) / LowestCommonDenominator(first, second);
}

inline void PrintFloatWithPrecision(double result, int precision)
{
    std::cout.setf(std::ios::fixed, std:: ios::floatfield);
    std::cout.precision(precision);
    std::cout << result << std::endl;
}