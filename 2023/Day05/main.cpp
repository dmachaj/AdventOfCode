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

// seed -> soil -> fert -> water -> light -> temp -> humid -> location

struct Mapping
{
    uint64_t DestBegin{};
    uint64_t DestEnd{};
    uint64_t SourceBegin{};
    uint64_t SourceEnd{};
};

Mapping StringToMapping(std::string const& value)
{
    const auto firstSpace = value.find(' ');
    const auto secondSpace = value.find(' ', firstSpace + 1);

    const auto first = std::strtoull(value.substr(0, firstSpace).c_str(), nullptr, 10);
    const auto second = std::strtoull(value.substr(firstSpace + 1, secondSpace - firstSpace).c_str(), nullptr, 10);
    const auto third = std::strtoull(value.substr(secondSpace + 1).c_str(), nullptr, 10);

    return {
        first, first + third - 1,
        second, second + third - 1,
    };
}

std::vector<uint64_t> StringToNums(std::string const& value)
{
    std::vector<uint64_t> result;

    std::istringstream inStream(value);
    std::string input;
    while (std::getline(inStream, input, ' '))
    {
        result.emplace_back(std::strtoull(input.c_str(), nullptr, 10));
    }
    return result;
}

void Part1()
{
    std::string seedStr;
    std::getline(std::cin, seedStr);
    seedStr = seedStr.substr(seedStr.find(':') + 2);
    auto seedResults = StringToNums(seedStr);

    std::cin.get(); // eat newline

    for (auto i = 0; i < 7; ++i)
    {
        std::string input;
        std::getline(std::cin, input); // eat header

        std::vector<Mapping> mappings;
        while (std::getline(std::cin, input) && !input.empty())
        {
            mappings.emplace_back(StringToMapping(input));
        }

        for (auto& result : seedResults)
        {
            for (const auto& mapping : mappings)
            {
                if ((result >= mapping.SourceBegin) && (result <= mapping.SourceEnd))
                {
                    const auto diff = result - mapping.SourceBegin;
                    result = mapping.DestBegin + diff;
                    break;
                }
            }
        }

        std::cin.get(); // eat newline
    }

    const auto result = *std::min_element(seedResults.begin(), seedResults.end());
    std::cout << result << std::endl;
}

void Part2()
{
    std::string seedStr;
    std::getline(std::cin, seedStr);
    seedStr = seedStr.substr(seedStr.find(':') + 2);
    auto seedResultsOrig = StringToNums(seedStr);

    struct Range
    {
        uint64_t Begin{};
        uint64_t End{};
    };

    std::vector<Range> ranges;
    for (auto i = 0; i < seedResultsOrig.size(); i += 2)
    {
        const auto start = seedResultsOrig[i];
        const auto count = seedResultsOrig[i + 1];
        ranges.emplace_back(Range{start, start + count - 1});
    }

    std::cin.get(); // eat newline

    for (auto i = 0; i < 7; ++i)
    {
        std::string input;
        std::getline(std::cin, input); // eat header

        std::vector<Mapping> mappings;
        while (std::getline(std::cin, input) && !input.empty())
        {
            mappings.emplace_back(StringToMapping(input));
        }

        std::vector<Range> updatedRanges;
        bool mappingApplied{false};
        for (const auto& mapping : mappings)
        {
            for (auto& range : ranges)
            {
                // Mapping is in the middle of the range
                if (((mapping.SourceBegin > range.Begin) && (mapping.SourceBegin < range.End)) &&
                   ((mapping.SourceEnd > range.Begin) && (mapping.SourceEnd < range.End)))
                {
                    const auto newEnd = mapping.SourceBegin - 1;
                    const Range before{ range.Begin, newEnd };
                    updatedRanges.emplace_back(std::move(before));

                    const Range middle{ mapping.DestBegin, mapping.DestEnd };
                    updatedRanges.emplace_back(std::move(middle));

                    const auto newBegin = mapping.SourceEnd + 1;
                    const Range after{ newBegin, range.End };
                    updatedRanges.emplace_back(std::move(after));

                    mappingApplied = true;
                }
                // Mapping completely covers the range
                else if ((mapping.SourceBegin <= range.Begin) && (mapping.SourceEnd >= range.End))
                {
                    const auto numAffected = range.End - range.Begin;
                    const auto diffIntoMapping = range.Begin - mapping.SourceBegin;
                    const Range updated{ mapping.DestBegin + diffIntoMapping, mapping.DestBegin + diffIntoMapping + numAffected };
                    updatedRanges.emplace_back(std::move(updated));
                    mappingApplied = true;
                }
                // Mapping extends past the end of the range
                else if ((mapping.SourceBegin >= range.Begin) && (mapping.SourceBegin <= range.End) &&
                         (mapping.SourceEnd > range.End))
                {
                    const auto newEnd = mapping.SourceBegin - 1;
                    const Range before{ range.Begin, newEnd };
                    updatedRanges.emplace_back(std::move(before));

                    const auto numAffected = range.Begin - mapping.SourceBegin;
                    const Range after{ mapping.DestBegin, mapping.DestBegin + numAffected - 1 };
                    updatedRanges.emplace_back(std::move(after));

                    mappingApplied = true;
                }
                // Mapping precedes the range and stops in the middle
                else if ((mapping.SourceEnd >= range.Begin) && (mapping.SourceEnd <= range.End) &&
                         (mapping.SourceBegin < range.Begin))
                {
                    const auto numTransformed = mapping.SourceEnd - range.Begin + 1;
                    const auto diffIntoMapping = range.Begin - mapping.SourceBegin;
                    const Range before{ mapping.DestBegin + diffIntoMapping, mapping.DestBegin + diffIntoMapping + numTransformed - 1};
                    updatedRanges.emplace_back(std::move(before));

                    const Range unmodified{ range.Begin + numTransformed, range.End };
                    updatedRanges.emplace_back(std::move(unmodified));

                    mappingApplied = true;
                }
                else
                {
                    updatedRanges.emplace_back(range);
                }
            }
            std::swap(updatedRanges, ranges);

            // if (!mappingApplied)
            // {
            //     updatedRanges.emplace_back(range);
            // }
        }
        // std::swap(updatedRanges, ranges);

        std::cin.get(); // eat newline
    }

    const auto result = *std::min_element(ranges.begin(), ranges.end(),
        [](const auto& left, const auto& right) { return left.Begin < right.Begin; });
    std::cout << result.Begin << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}