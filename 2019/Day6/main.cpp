#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "HelpfulInfrastructure.h"

namespace
{
    void Part1()
    {
        // first is orbiter, second is center
        std::map<std::string, std::string> orbits;

        std::string input;
        while (std::getline(std::cin, input))
        {
            std::istringstream inputStream(input);
            std::string center;
            std::string orbiter;
            std::getline(inputStream, center, ')');
            std::getline(inputStream, orbiter);
            
            orbits[orbiter] = center;
            // std::cerr << center << " orbited by " << orbiter << std::endl;
        }

        uint32_t numOrbits{};
        for (const auto& object : orbits)
        {
            auto parent = orbits.find(object.second);
            ++numOrbits;
            while (parent != orbits.end())
            {
                ++numOrbits;
                parent = orbits.find(parent->second);
            }
        }

        std::cout << numOrbits << std::endl;
    }

    void Part2()
    {
        // first is orbiter, second is center
        std::map<std::string, std::string> orbits;

        std::string input;
        while (std::getline(std::cin, input))
        {
            std::istringstream inputStream(input);
            std::string center;
            std::string orbiter;
            std::getline(inputStream, center, ')');
            std::getline(inputStream, orbiter);
            
            orbits[orbiter] = center;
            // std::cerr << center << " orbited by " << orbiter << std::endl;
        }

        std::vector<std::string> myAncestors{};
        {
            auto parent = orbits.find("YOU");
            while (parent != orbits.end())
            {
                myAncestors.emplace_back(parent->second);
                parent = orbits.find(parent->second);
            }
        }

        std::vector<std::string> santasAncestors{};
        {
            auto parent = orbits.find("SAN");
            while (parent != orbits.end())
            {
                santasAncestors.emplace_back(parent->second);
                parent = orbits.find(parent->second);
            }
        }

        std::string firstCommonAncestor{};
        for (const auto& myAncestor : myAncestors)
        {
            const auto result = std::find(santasAncestors.begin(), santasAncestors.end(), myAncestor);
            if (result != santasAncestors.end())
            {
                firstCommonAncestor = *result;
                break;
            }
        }

        uint32_t transfers{};
        {
            auto parent = orbits.find("YOU");
            while (parent->second != firstCommonAncestor)
            {
                ++transfers;
                parent = orbits.find(parent->second);
            }
        }

        {
            auto parent = orbits.find("SAN");
            while (parent->second != firstCommonAncestor)
            {
                ++transfers;
                parent = orbits.find(parent->second);
            }
        }

        // transfers--;

        std::cout << transfers << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}