#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
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

uint32_t Hash(std::string const& input)
{
    uint32_t result{};
    for (char c : input)
    {
        if (c == '\n') continue;
        if (c == '\r') continue;
        result += (uint32_t)c;
        result *= 17;
        result %= 256;
    }
    return result;
}

void Part1()
{
    std::string input;
    std::getline(std::cin, input);

    uint32_t countHashes{};
    uint64_t result{};
    std::istringstream inStream(std::move(input));
    while(std::getline(inStream, input, ','))
    {
        result += Hash(input);
        ++countHashes;
        // std::cerr << input << " hashes to " << Hash(input) << std::endl;
    }

    std::cerr << countHashes << " hashes computed" << std::endl;
    std::cout << result << std::endl;
}

void Part2()
{
    std::cout << 0 << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}