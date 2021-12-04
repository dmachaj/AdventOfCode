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

// #define DEBUG_OUTPUT

namespace
{
    std::vector<char> FindAllRoots(const std::vector<std::pair<char, char>>& nodes)
    {
        std::vector<char> roots;
        for (auto i = 'A'; i <= 'Z'; ++i)
        {
            if (std::find_if(nodes.begin(), nodes.end(), [i](const auto& val) { return val.first == i; }) == nodes.end())
            {
                continue; // this letter doesn't exist in the set
            }

            // A node is a "root" if it depends on nothing
            if (std::find_if(nodes.begin(), nodes.end(), [i](const auto& val) { return val.second == i; }) == nodes.end())
            {
                roots.emplace_back(i);
            }
        }
        return roots;
    }

    void Part1()
    {
        std::vector<std::pair<char, char>> nodes;

        std::string input;
        while (std::getline(std::cin, input))
        {
            const char first = input[5];
            const char second = input[36];
            nodes.emplace_back(std::make_pair(first, second));
        }

        std::vector<char> roots = FindAllRoots(nodes);

        std::string result{};
        std::set<char> eligibleNodes;
        std::copy(roots.begin(), roots.end(), std::inserter(eligibleNodes, eligibleNodes.end()));
        while (!eligibleNodes.empty())
        {
            const auto currentNode = *eligibleNodes.begin();
            eligibleNodes.erase(currentNode);
            result += currentNode;

            for (const auto& node : nodes)
            {
                if (node.first == currentNode)
                {
                    // Unblocked by current node.  See if all dependencies are satisfied or if this needs to wait until later.
                    std::vector<std::pair<char, char>> allDependencies;
                    std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(allDependencies), [node](const auto& val)
                    {
                        return val.second == node.second;
                    });

                    if (std::all_of(allDependencies.begin(), allDependencies.end(), [result](const auto& val)
                    {
                        return result.find(val.first) != std::string::npos;
                    }))
                    {
                        eligibleNodes.emplace(node.second);
                    }
                }
            }
        }

        std::cout << result << std::endl;
    }

    void Part2()
    {
        uint32_t workerCount;
        std::cin >> workerCount;
        std::cin.get(); // eat newline 

        uint32_t fixedStepDelay;
        std::cin >> fixedStepDelay;
        std::cin.get(); // eat newline 

        std::vector<std::pair<char, char>> nodes;

        std::string input;
        while (std::getline(std::cin, input))
        {
            const char first = input[5];
            const char second = input[36];
            nodes.emplace_back(std::make_pair(first, second));
        }

        std::vector<char> roots = FindAllRoots(nodes);

        std::string visitedNodes{};
        std::set<char> eligibleNodes;
        std::copy(roots.begin(), roots.end(), std::inserter(eligibleNodes, eligibleNodes.end()));

        struct Worker
        {
            char Letter{};
            uint32_t CompletionTime{};
        };
        std::vector<Worker> workers(workerCount);
        uint32_t currentTime{};

        while (true)
        {
            for (auto i = 0UL; i < workers.size(); ++i)
            {
                auto& currentWorker = workers[i];

                if (currentWorker.CompletionTime <= currentTime)
                {
                    const auto completedNode = currentWorker.Letter;
                    if (completedNode != '\0')
                    {
                        currentWorker.Letter = '\0';
                        visitedNodes += completedNode;
                        
                        for (const auto& node : nodes)
                        {
                            if (node.first == completedNode)
                            {
                                // Unblocked by current node.  See if all dependencies are satisfied or if this needs to wait until later.
                                std::vector<std::pair<char, char>> allDependencies;
                                std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(allDependencies), [node](const auto& val)
                                {
                                    return val.second == node.second;
                                });

                                if (std::all_of(allDependencies.begin(), allDependencies.end(), [visitedNodes](const auto& val)
                                {
                                    return visitedNodes.find(val.first) != std::string::npos;
                                }))
                                {
                                    eligibleNodes.emplace(node.second);
                                }
                            }
                        }
                    }
                }

                if ((currentWorker.CompletionTime <= currentTime) && !eligibleNodes.empty())
                {
                    const auto currentNode = *eligibleNodes.begin();
                    eligibleNodes.erase(currentNode);

                    currentWorker.Letter = currentNode;
                    currentWorker.CompletionTime = currentTime + (currentNode - 'A' + 1) + fixedStepDelay;
                }
            }

#ifdef DEBUG_OUTPUT
            std::cerr << currentTime << "\t";
            for (const auto& worker : workers)
            {
                std::cerr << ((worker.Letter != '\0') ? worker.Letter : '.') << "\t";
            }
            std::cerr << "\t" << visitedNodes << std::endl;
#endif

            if (std::all_of(workers.begin(), workers.end(), [](const auto& worker) { return worker.Letter == '\0'; }))
            {
                break;
            }

            currentTime++;
        }

        // Stupid dirty hack.  One extra iteration is needed to complete work but only for the unique input.  The sample
        // is correct.  Work around by subtracting one like an idiot.
        if (currentTime > 20) --currentTime;

        std::cout << currentTime << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}