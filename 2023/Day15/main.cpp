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
    std::string input;
    std::getline(std::cin, input);

    struct Box
    {
        struct Lens
        {
            std::string Label;
            uint32_t Value{};
        };
        std::vector<Lens> Lenses;
    };
    std::vector<Box> boxes(256);

    std::istringstream inStream(std::move(input));
    while(std::getline(inStream, input, ','))
    {
        const auto eq = input.find('=');
        bool replace{false};
        const auto dash = input.find('-');
        bool remove{false};

        std::string label;
        uint32_t hash{};
        uint32_t lensValue{};
        if (eq != input.npos)
        {
            label = input.substr(0, eq);
            replace = true;
            lensValue = std::atoi(input.substr(eq + 1).c_str());
        }
        else if (dash != input.npos)
        {
            label = input.substr(0, dash);
            remove = true;
        }
        else throw std::exception();

        hash = Hash(label);

        if (remove)
        {
            Box& thisBox = boxes[hash];
            const auto toRemove = std::find_if(thisBox.Lenses.begin(), thisBox.Lenses.end(),
                [&label](Box::Lens const& lens)
                {
                    return lens.Label == label;
                });
            
            if (toRemove != thisBox.Lenses.end())
            {
                thisBox.Lenses.erase(toRemove);
            }
        }
        else
        {
            Box& thisBox = boxes[hash];
            const auto toReplace = std::find_if(thisBox.Lenses.begin(), thisBox.Lenses.end(),
                [&label](Box::Lens const& lens)
                {
                    return lens.Label == label;
                });
            
            if (toReplace != thisBox.Lenses.end())
            {
                toReplace->Value = lensValue;
            }
            else
            {
                thisBox.Lenses.emplace_back(Box::Lens{label, lensValue});
            }
        }
    }

    uint64_t result{};
    for (auto i = 0; i < boxes.size(); ++i)
    {
        for (auto j = 0; j < boxes[i].Lenses.size(); ++j)
        {
            result += (i + 1) * (j + 1) * boxes[i].Lenses[j].Value;
        }
    }

    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}