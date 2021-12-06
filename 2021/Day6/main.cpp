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
    void Part1()
    {
        std::string input;
        std::getline(std::cin, input);
        istringstream instream(input);

        vector<int> numbers;
        while (getline(instream, input, ','))
        {
            numbers.emplace_back(std::atoi(input.c_str()));
        }

        for (auto days = 0UL; days < 80; ++days)
        {
            const auto startingSize = numbers.size();
            for (auto i = 0UL; i < startingSize; ++i)
            {
                --numbers[i];
                if (numbers[i] < 0)
                {
                    numbers[i] = 6;
                    numbers.emplace_back(8);
                }
            }
        }

        std::cout << numbers.size() << std::endl;
    }

    void Part2()
    {
        std::string input;
        std::getline(std::cin, input);
        istringstream instream(input);

        vector<int64_t> numbers;
        while (getline(instream, input, ','))
        {
            numbers.emplace_back(std::atoi(input.c_str()));
        }
        
        std::array<int64_t, 9> fishes{};
        for (const auto& number : numbers)
        {
            ++fishes[number];
        }

        for (auto days = 0UL; days < 256; ++days)
        {
            std::array<int64_t, 9> fishesCopy{};

            for (auto i = 1UL; i < fishes.size(); ++i)
            {
                fishesCopy[i - 1] = fishes[i];
            }
            
            fishesCopy[6] += fishes[0];
            fishesCopy[8] += fishes[0];

            std::swap(fishes, fishesCopy);
        }

        int64_t result = std::accumulate(fishes.begin(), fishes.end(), 0LL);

        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}