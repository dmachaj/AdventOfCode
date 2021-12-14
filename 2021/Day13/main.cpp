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
        vector<pair<int, int>> spots;
        int width{};
        int height{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;

            const auto first = std::atoi(input.substr(0, input.find(',')).c_str());
            const auto second = std::atoi(input.substr(input.find(',') + 1).c_str());

            spots.emplace_back(make_pair(first, second));
            width = std::max(width, first);
            height = std::max(height, second);
        }

        std::getline(std::cin, input); // first fold only

        char axis = input[input.find('=') - 1];
        const auto amount = std::atoi(input.substr(input.find('=') + 1).c_str());

        vector<pair<int, int>> spotCopy;
        for (const auto& spot : spots)
        {
            if (axis == 'y')
            {
                if (spot.second < amount)
                {
                    spotCopy.emplace_back(spot);
                }
                else
                {
                    spotCopy.emplace_back(make_pair(spot.first, (amount - (spot.second - amount))));
                }
            }
            else if (axis == 'x')
            {
                if (spot.first < amount)
                {
                    spotCopy.emplace_back(spot);
                }
                else
                {
                    spotCopy.emplace_back(make_pair(amount - (spot.first - amount), spot.second));
                }
            }
            else throw std::exception();
        }

        vector<pair<int, int>> final;
        std::sort(spotCopy.begin(), spotCopy.end());
        std::unique_copy(spotCopy.begin(), spotCopy.end(), std::back_inserter(final));

        std::cout << final.size() << std::endl;
    }

    void Part2()
    {
        vector<pair<uint32_t, uint32_t>> spots;
        uint32_t width{};
        uint32_t height{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;

            const uint32_t first = std::atoi(input.substr(0, input.find(',')).c_str());
            const uint32_t second = std::atoi(input.substr(input.find(',') + 1).c_str());

            spots.emplace_back(make_pair(first, second));
            width = std::max(width, first);
            height = std::max(height, second);
        }

        while (std::getline(std::cin, input))
        {
            char axis = input[input.find('=') - 1];
            const uint32_t amount = std::atoi(input.substr(input.find('=') + 1).c_str());

            if (axis == 'x') width -= amount;
            else if (axis == 'y') height -= amount;

            vector<pair<uint32_t, uint32_t>> spotCopy;
            for (const auto& spot : spots)
            {
                if (axis == 'y')
                {
                    if (spot.second < amount)
                    {
                        spotCopy.emplace_back(spot);
                    }
                    else
                    {
                        spotCopy.emplace_back(make_pair(spot.first, (amount - (spot.second - amount))));
                    }
                }
                else if (axis == 'x')
                {
                    if (spot.first < amount)
                    {
                        spotCopy.emplace_back(spot);
                    }
                    else
                    {
                        spotCopy.emplace_back(make_pair(amount - (spot.first - amount), spot.second));
                    }
                }
                else throw std::exception();
            }

            std::swap(spots, spotCopy);
        }

        for (uint32_t y = 0UL; y <= height; ++y)
        {
            for (uint32_t x = 0UL; x <= width; ++x)
            {
                if (std::find(spots.begin(), spots.end(), std::make_pair(x, y)) != spots.end())
                {
                    std::cerr << "#";
                }
                else
                {
                    std::cerr << ".";
                }
            }
            std::cerr << std::endl;
        }

        std::cout << "PGHRKLKL" << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}