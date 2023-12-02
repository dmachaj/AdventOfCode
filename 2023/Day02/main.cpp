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

struct Bag
{
    uint32_t red{};
    uint32_t blue{};
    uint32_t green{};
};

Bag BagFromHand(std::string const& str)
{
    Bag result{};

    std::istringstream inStream(str);
    std::string temp;
    while (std::getline(inStream, temp, ','))
    {
        const auto space = temp.find(' ', 1);
        const auto numStr = temp.substr(0, space);
        const auto num = std::atoi(numStr.c_str());

        if (temp.find("red") != std::string::npos)
        {
            result.red = num;
        }
        else if (temp.find("blue") != std::string::npos)
        {
            result.blue = num;
        }
        else if (temp.find("green") != std::string::npos)
        {
            result.green = num;
        }
    }

    return result;
}

bool IsBagValid(Bag const& bag)
{
    constexpr uint32_t c_red = 12;
    constexpr uint32_t c_green = 13;
    constexpr uint32_t c_blue = 14;

    return (bag.red <= c_red) && (bag.blue <= c_blue) && (bag.green <= c_green);
}

void Part1()
{
    uint32_t result{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        auto colon = input.find(':');
        auto gameNumStr = input.substr(5, colon - 5);
        const uint32_t gameNum = std::atoi(gameNumStr.c_str());

        input = input.substr(colon + 2);

        std::vector<Bag> bags;
        while (input.find(';') != std::string::npos)
        {
            const auto semi = input.find(';');
            bags.emplace_back(BagFromHand(input.substr(0, semi)));
            input = input.substr(semi + 2);
        }
        bags.emplace_back(BagFromHand(input));

        const bool gameIsValid = std::all_of(bags.begin(), bags.end(), [](const Bag& bag) { return IsBagValid(bag); });
        if (gameIsValid)
        {
            result += gameNum;
        }
    }
    std::cout << result << std::endl;
}

void Part2()
{
    uint32_t result{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        auto colon = input.find(':');
        auto gameNumStr = input.substr(5, colon - 5);
        const uint32_t gameNum = std::atoi(gameNumStr.c_str());

        input = input.substr(colon + 2);

        std::vector<Bag> bags;
        while (input.find(';') != std::string::npos)
        {
            const auto semi = input.find(';');
            bags.emplace_back(BagFromHand(input.substr(0, semi)));
            input = input.substr(semi + 2);
        }
        bags.emplace_back(BagFromHand(input));

        const auto minRed = std::max_element(bags.begin(), bags.end(),
        [](Bag const& left, Bag const& right)
        {
            return left.red < right.red;
        })->red;

        const auto minBlue = std::max_element(bags.begin(), bags.end(),
        [](Bag const& left, Bag const& right)
        {
            return left.blue < right.blue;
        })->blue;

        const auto minGreen = std::max_element(bags.begin(), bags.end(),
        [](Bag const& left, Bag const& right)
        {
            return left.green < right.green;
        })->green;

        result += (minRed * minBlue * minGreen);
    }
    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}