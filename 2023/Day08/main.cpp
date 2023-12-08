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

void Part1()
{
    std::string directions;
    std::getline(std::cin, directions);
    std::cin.get(); // eat newline

    struct Node
    {
        std::string Begin;
        std::string Left;
        std::string Right;
    };

    std::vector<Node> nodes;

    std::string input;
    while (std::getline(std::cin, input))
    {
        auto begin = input.substr(0, 3);
        auto left = input.substr(7, 3);
        auto right = input.substr(12, 3);

        nodes.emplace_back(Node{std::move(begin), std::move(left), std::move(right)});
    }

    uint32_t currentDirectionIndex{};
    std::string current = "AAA";
    while (current != "ZZZ")
    {
        const auto node = std::find_if(nodes.begin(), nodes.end(), [current](const auto& node)
        {
            return node.Begin == current;
        });

        if (node == nodes.end()) throw std::exception();

        const auto direction = directions[currentDirectionIndex++ % directions.size()];
        if (direction == 'L')
        {
            current = node->Left;
        }
        else if (direction == 'R')
        {
            current = node->Right;
        }
        else throw std::exception();
    }

    std::cout << currentDirectionIndex << std::endl;
}

void Part2()
{
    std::string directions;
    std::getline(std::cin, directions);
    std::cin.get(); // eat newline

    struct Node
    {
        std::string Begin;
        std::string LeftStr;
        std::string RightStr;
        Node* Left;
        Node* Right;

        bool StartingLocation{};
        bool EndingLocation{};
    };

    std::unordered_map<std::string, Node> nodes;

    std::string input;
    while (std::getline(std::cin, input))
    {
        auto begin = input.substr(0, 3);
        auto left = input.substr(7, 3);
        auto right = input.substr(12, 3);

        const bool start = begin[2] == 'A';
        const bool end = begin[2] == 'Z';

        nodes[begin] = Node{begin, std::move(left), std::move(right), nullptr, nullptr, start, end };
    }

    std::vector<const Node*> locations;
    for (auto& node : nodes)
    {
        if (node.second.StartingLocation)
        {
            locations.emplace_back(&(node.second));
        }

        node.second.Left = &(nodes.find(node.second.LeftStr)->second);
        node.second.Right = &(nodes.find(node.second.RightStr)->second);
    }

    std::vector<uint64_t> results;
    for (auto& node : locations)
    {
        uint64_t currentDirectionIndex{};
        while (!node->EndingLocation)
        {
            const auto direction = directions[currentDirectionIndex++ % directions.size()];
            
            if (direction == 'L')
            {
                node = node->Left;
            }
            else if (direction == 'R')
            {
                node = node->Right;
            }
            else throw std::exception();
        }

        results.emplace_back(currentDirectionIndex);
    }

    uint64_t lowestMultiple{results[0]};
    for (auto i = 1; i < results.size(); ++i)
    {
        lowestMultiple = LeastCommonMultiple(lowestMultiple, results[i]);
    }

    std::cout << lowestMultiple << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}