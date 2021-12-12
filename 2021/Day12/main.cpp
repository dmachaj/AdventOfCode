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
    using graph_t = std::unordered_map<std::string, std::vector<std::string>>;

    uint32_t CountUniquePathsRecursive(const graph_t& graph, const std::unordered_map<std::string, bool>& isLarge, const std::string& current, std::vector<std::string> visitedNodes)
    {
        if (current == "end") return 1; // yay we reached the end

        uint32_t total{};
        for (const auto& node : graph.find(current)->second)
        {
            auto visitedNodesCopy = visitedNodes;
            if (std::find(visitedNodesCopy.begin(), visitedNodesCopy.end(), node) == visitedNodesCopy.end())
            {
                if (isLarge.find(node)->second != true)
                {
                    visitedNodesCopy.emplace_back(node);
                }

                total += CountUniquePathsRecursive(graph, isLarge, node, visitedNodesCopy);
            }
        }

        return total;
    }

    void Part1()
    {
        graph_t graph;
        std::unordered_map<std::string, bool> isLarge;

        std::string input;
        while (std::getline(std::cin, input))
        {
            std::string first = input.substr(0, input.find('-'));
            std::string second = input.substr(input.find('-') + 1);

            if (graph.find(first) == graph.end())
            {
                graph.emplace(first, std::vector<std::string>());
                isLarge.emplace(first, std::isupper(first[0]) != 0);
            }

            if (graph.find(second) == graph.end())
            {
                graph.emplace(second, std::vector<std::string>());
                isLarge.emplace(second, std::isupper(second[0]) != 0);
            }

            graph[first].emplace_back(second);
            graph[second].emplace_back(first);
        }

        std::vector<std::string> visitedNodes { "start" };
        uint32_t uniquePaths = CountUniquePathsRecursive(graph, isLarge, "start", visitedNodes);

        std::cout << uniquePaths << std::endl;
    }

    std::vector<std::string> CountUniquePathsRecursivePart2(
        const graph_t& graph,
        const std::unordered_map<std::string, bool>& isLarge,
        const std::string& current,
        const std::string doubleVisitedSmall,
        std::string stringBuilder,
        std::vector<std::string> visitedNodes)
    {
        if (current == "end") // yay we reached the end
        {
            stringBuilder += "end";
            return {stringBuilder};
        }

        stringBuilder += current;

        std::vector<std::string> total;
        for (const auto& node : graph.find(current)->second)
        {
            auto visitedNodesCopy = visitedNodes;
            if (std::find(visitedNodesCopy.begin(), visitedNodesCopy.end(), node) == visitedNodesCopy.end())
            {
                const bool isSmall = isLarge.find(node)->second != true;
                if (isSmall && doubleVisitedSmall.empty())
                {
                    auto temp = CountUniquePathsRecursivePart2(graph, isLarge, node, node, stringBuilder, visitedNodesCopy);
                    std::copy(temp.begin(), temp.end(), std::back_inserter(total));

                    visitedNodesCopy.emplace_back(node);
                    temp = CountUniquePathsRecursivePart2(graph, isLarge, node, "", stringBuilder, visitedNodesCopy);
                    std::copy(temp.begin(), temp.end(), std::back_inserter(total));
                }
                else if (isSmall)
                {
                    visitedNodesCopy.emplace_back(node);
                    auto temp = CountUniquePathsRecursivePart2(graph, isLarge, node, doubleVisitedSmall, stringBuilder, visitedNodesCopy);
                    std::copy(temp.begin(), temp.end(), std::back_inserter(total));
                }
                else
                {
                    auto temp = CountUniquePathsRecursivePart2(graph, isLarge, node, doubleVisitedSmall, stringBuilder, visitedNodesCopy);
                    std::copy(temp.begin(), temp.end(), std::back_inserter(total));
                }
            }
        }

        return total;
    }

    void Part2()
    {
        graph_t graph;
        std::unordered_map<std::string, bool> isLarge;

        std::string input;
        while (std::getline(std::cin, input))
        {
            std::string first = input.substr(0, input.find('-'));
            std::string second = input.substr(input.find('-') + 1);

            if (graph.find(first) == graph.end())
            {
                graph.emplace(first, std::vector<std::string>());
                isLarge.emplace(first, std::isupper(first[0]) != 0);
            }

            if (graph.find(second) == graph.end())
            {
                graph.emplace(second, std::vector<std::string>());
                isLarge.emplace(second, std::isupper(second[0]) != 0);
            }

            graph[first].emplace_back(second);
            graph[second].emplace_back(first);
        }

        std::vector<std::string> visitedNodes { "start" };
        auto allPaths = CountUniquePathsRecursivePart2(graph, isLarge, "start", "", "", visitedNodes);

        std::sort(allPaths.begin(), allPaths.end());
        std::vector<std::string> uniquePaths;
        std::unique_copy(allPaths.begin(), allPaths.end(), std::back_inserter(uniquePaths));

        // for (const auto& path : uniquePaths)
        // {
        //     std::cerr << path << std::endl;
        // }

        std::cout << uniquePaths.size() << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}