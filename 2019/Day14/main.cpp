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
    struct Reaction
    {
        std::pair<uint64_t, std::string> produces;
        std::vector<std::pair<uint64_t, std::string>> consumes;

        // Reaction()
        // {
        //     produces = make_pair(1ULL, "ORE");
        // }

        Reaction(const std::string& input)
        {
            const auto divider = input.find("=");
            {
                std::string temp = input.substr(divider + 3);
                const auto space = temp.find(" ");
                uint64_t cost = (uint64_t)std::atoi(temp.substr(0, space).c_str());
                std::string name = temp.substr(space + 1);
                produces = std::make_pair(cost, name);
            }

            const auto consumerStr = input.substr(0, divider - 1);
            std::istringstream inStream(consumerStr);
            std::string temp;
            while (std::getline(inStream, temp, ','))
            {
                const auto space = temp.find(" ", 1);
                uint64_t cost = (uint64_t)std::atoi(temp.substr(0, space).c_str());
                std::string name = temp.substr(space + 1);
                consumes.emplace_back(std::make_pair(cost, name));
            }
        }
    };

    void Part1()
    {
        std::vector<Reaction> reactions;

        std::string input;
        while (std::getline(std::cin, input))
        {
            reactions.emplace_back(Reaction(input));
        }

        // Name of necessary component and the amount needed
        std::map<std::string, uint64_t> pending;
        pending["FUEL"] = 1;

        // Name of component and the amount leftover after previous reactions
        std::map<std::string, uint64_t> leftovers;

        const auto OnlyOreRemaining = [](const std::map<std::string, uint64_t>& pending)
        {
            return std::all_of(pending.begin(), pending.end(), [](std::pair<std::string, uint64_t> component)
            {
                return (component.first == "ORE") || (component.second == 0);
            });
        };

        while (!OnlyOreRemaining(pending))
        {
            for (auto& pendingComponent : pending)
            {
                if (pendingComponent.second == 0) continue;
                if (pendingComponent.first == "ORE") continue;

                auto recipeIter = std::find_if(reactions.begin(), reactions.end(), [pendingComponent](const Reaction& reaction)
                {
                    return pendingComponent.first == reaction.produces.second;
                });

                // We have enough leftovers.  Subtract them and zero out our needs.
                if (pendingComponent.second <= leftovers[pendingComponent.first])
                {
                    leftovers[pendingComponent.first] -= pendingComponent.second;
                    pendingComponent.second = 0ULL;
                    continue;
                }

                const uint64_t amountNeeded = pendingComponent.second - leftovers[pendingComponent.first];
                leftovers[pendingComponent.first] = 0ULL;
                uint64_t reactionsNeeded = amountNeeded / recipeIter->produces.first;
                if ((amountNeeded % recipeIter->produces.first) != 0)
                {
                    ++reactionsNeeded;
                }

                for (const auto needed : recipeIter->consumes)
                {
                    pending[needed.second] += (needed.first * reactionsNeeded);
                }

                leftovers[pendingComponent.first] = (recipeIter->produces.first * reactionsNeeded) - amountNeeded;

                pendingComponent.second = 0ULL;
            }
        }

        std::cout << pending["ORE"] << std::endl;
    }

    void Part2()
    {
        std::vector<Reaction> reactions;

        std::string input;
        while (std::getline(std::cin, input))
        {
            reactions.emplace_back(Reaction(input));
        }

        // Name of necessary component and the amount needed
        std::map<std::string, uint64_t> pending;
        pending["FUEL"] = 1;

        // Name of component and the amount leftover after previous reactions
        std::map<std::string, uint64_t> leftovers;

        const auto OnlyOreRemaining = [](const std::map<std::string, uint64_t>& pending)
        {
            return std::all_of(pending.begin(), pending.end(), [](std::pair<std::string, uint64_t> component)
            {
                return (component.first == "ORE") || (component.second == 0);
            });
        };

        while (!OnlyOreRemaining(pending))
        {
            for (auto& pendingComponent : pending)
            {
                if (pendingComponent.second == 0) continue;
                if (pendingComponent.first == "ORE") continue;

                auto recipeIter = std::find_if(reactions.begin(), reactions.end(), [pendingComponent](const Reaction& reaction)
                {
                    return pendingComponent.first == reaction.produces.second;
                });

                // We have enough leftovers.  Subtract them and zero out our needs.
                if (pendingComponent.second <= leftovers[pendingComponent.first])
                {
                    leftovers[pendingComponent.first] -= pendingComponent.second;
                    pendingComponent.second = 0ULL;
                    continue;
                }

                const uint64_t amountNeeded = pendingComponent.second - leftovers[pendingComponent.first];
                leftovers[pendingComponent.first] = 0ULL;
                uint64_t reactionsNeeded = amountNeeded / recipeIter->produces.first;
                if ((amountNeeded % recipeIter->produces.first) != 0)
                {
                    ++reactionsNeeded;
                }

                for (const auto needed : recipeIter->consumes)
                {
                    pending[needed.second] += (needed.first * reactionsNeeded);
                }

                leftovers[pendingComponent.first] = (recipeIter->produces.first * reactionsNeeded) - amountNeeded;

                pendingComponent.second = 0ULL;
            }
        }

        uint64_t result = 1000000000000 / pending["ORE"];

        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}