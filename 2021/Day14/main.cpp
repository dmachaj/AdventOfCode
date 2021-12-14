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
        std::string start;
        std::getline(std::cin, start);
        std::cin.get(); // eat newline 

        std::vector<pair<string, char>> reactions;

        std::string input;
        while (std::getline(std::cin, input))
        {
            auto in = input.substr(0, 2);
            auto result = input[input.size() - 1];

            reactions.emplace_back(make_pair(move(in), result));
        }

        string current = start;
        for (auto i = 0UL; i < 10; ++i)
        {
            string temp;
            bool previousMatched{false};
            for (auto x = 0UL; x < current.size() - 1; ++x)
            {
                string match = current.substr(x, 2);
                const auto find = std::find_if(reactions.begin(), reactions.end(), [match](const auto curr) { return curr.first == match; });
                if (find != reactions.end())
                {
                    if (!previousMatched)
                    {
                        temp += match[0];
                    }
                    temp += find->second;
                    temp += match[1];
                    previousMatched = true;
                }
                else
                {
                    if (!previousMatched)
                    {
                        temp += match[0];
                    }
                    temp += match[1];
                    previousMatched = false;
                }
            }

            std::swap(temp, current);
        }

        uint64_t min{9999};
        uint64_t max{};

        string allChars = start;
        for (const auto& reaction : reactions)
        {
            allChars += reaction.second;
        }

        for (const auto c : allChars)
        {
            const uint64_t instances = std::accumulate(current.begin(), current.end(), 0UL, [c](auto in, const auto curr) { return c == curr ? in + 1 : in; });
            min = std::min(min, instances);
            max = std::max(max, instances);
        }

        std::cout << (max - min) << std::endl;
    }

    void Part2()
    {
        std::string start;
        std::getline(std::cin, start);
        std::cin.get(); // eat newline 

        std::vector<pair<string, char>> reactions;

        std::string input;
        while (std::getline(std::cin, input))
        {
            auto in = input.substr(0, 2);
            auto result = input[input.size() - 1];

            reactions.emplace_back(make_pair(move(in), result));
        }

        string allChars;
        {
            string allCharsTemp = start;
            for (const auto& reaction : reactions)
            {
                allCharsTemp += reaction.second;
            }
            std::sort(allCharsTemp.begin(), allCharsTemp.end());
            std::unique_copy(allCharsTemp.begin(), allCharsTemp.end(), std::back_inserter(allChars));
        }

        unordered_map<string, uint64_t> pairCounts;
        for (auto outer = 0UL; outer < start.size() - 1; ++outer)
        {
            string current = start.substr(outer, 2);
            pairCounts[current]++;
        }

        for (auto i = 0UL; i < 40; ++i)
        {
            unordered_map<string, uint64_t> pairCountsCopy;
            for (const auto& pair : pairCounts)
            {
                const auto reaction = std::find_if(reactions.begin(), reactions.end(), [pair](auto curr) { return curr.first == pair.first;});
                string first;
                first += pair.first[0];
                first += reaction->second;

                string second;
                second += reaction->second;
                second += pair.first[1];

                pairCountsCopy[first] += pair.second;
                pairCountsCopy[second] += pair.second;
            }

            std::swap(pairCountsCopy, pairCounts);
        }

        // const auto totalCount = std::accumulate(pairCounts.begin(), pairCounts.end(), 0ULL, [](auto input, auto curr) { return input + curr.second; });
        // uint64_t expectedCount { start.size() };
        // for (auto i = 0UL; i < 40; ++i)
        // {
        //     expectedCount += (expectedCount - 1);
        // }

        // if (totalCount != expectedCount) throw std::exception();

        unordered_map<char, uint64_t> letterCounts;
        for (const auto c : allChars)
        {
            const auto count = std::accumulate(pairCounts.begin(), pairCounts.end(), 0ULL, [c](auto input, auto curr)
            {
                auto result = input;
                if (curr.first[0] == c)
                {
                    result += curr.second;
                }
                
                if (curr.first[1] == c)
                {
                    result += curr.second;
                }
                return result;
            });
            letterCounts[c] += count;
        }
        letterCounts[start[0]] += 1;
        letterCounts[start[start.size() - 1]] += 1;
        for (auto& count : letterCounts)
        {
            count.second /= 2;
        }

        uint64_t min = std::min_element(letterCounts.begin(), letterCounts.end(), [](auto first, auto second) { return first.second < second.second; })->second;
        uint64_t max = std::max_element(letterCounts.begin(), letterCounts.end(), [](auto first, auto second) { return first.second < second.second; })->second;
        std::cout << (max - min) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}