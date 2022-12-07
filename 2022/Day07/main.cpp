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

namespace
{
    struct Dir
    {
        string name;
        vector<Dir> children;
        uint32_t files{};
        Dir* parent{nullptr};

        uint32_t ComputeSizeRecursive() const
        {
            uint32_t total{};
            for (const auto& child : children)
            {
                total += child.ComputeSizeRecursive();
            }
            return (total + files);
        }

        Dir* FindChildDir(const string& target)
        {
            for (auto& child : children)
            {
                if (child.name == target)
                {
                    return (Dir*)&child;
                }
            }
            return nullptr;
        }

        void Visit(std::function<void(const Dir*)> func) const
        {
            func(this);
            for (const auto& child : children)
            {
                child.Visit(func);
            }
        }

        void DumpTree(uint32_t indent) const
        {
            for (auto i = 0UL; i < indent; ++i)
            {
                std::cout << " ";
            }
            std::cout << name << std::endl;

            for (const auto& child : children)
            {
                child.DumpTree(indent + 2);
            }
        }
    };

    Dir ParseFileSystem()
    {
        Dir root{ "/", {}, 0UL, nullptr };
        Dir* current = &root;

        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input[0] == '$') // command
            {
                if (input == "$ ls"sv)
                {
                    // don't care
                }
                else if (input == "$ cd .."sv)
                {
                    current = current->parent;
                }
                else if (input == "$ cd /"sv)
                {
                    current = &root;
                }
                else if (input[2] == 'c')
                {
                    const auto prev = current;
                    const auto target = input.substr(input.find_last_of(' ') + 1);
                    current = current->FindChildDir(target);
                    if (!current) { root.DumpTree(0); throw std::exception("Unknown dir"); };
                }
                else
                {
                    throw std::exception("unknown command");
                }
            }
            else if (input[0] == 'd') // dir listing
            {
                auto dirName = input.substr(input.find(' ') + 1);
                Dir newDir{ std::move(dirName), {}, 0UL, current };
                current->children.emplace_back(std::move(newDir));
            }
            else // file listing
            {
                const auto fileSize = atoi(input.substr(0, input.find(' ')).c_str());
                current->files += fileSize;
            }
        }
        return root;
    }

    void Part1()
    {
        Dir root = ParseFileSystem();

        uint32_t total{};
        root.Visit([&](const Dir* dir)
        {
            const auto size = dir->ComputeSizeRecursive();
            if (size <= 100000)
            {
                total += size;
            }
        });

        std::cout << total << std::endl;
    }

    void Part2()
    {
        Dir root = ParseFileSystem();

        constexpr uint32_t c_totalDisk{70000000UL};
        constexpr uint32_t c_neededSpace{30000000UL};

        uint32_t totalInUse = root.ComputeSizeRecursive();
        const auto availableSpace = c_totalDisk - totalInUse;
        const auto spaceNeeded = c_neededSpace - availableSpace;

        vector<uint32_t> reclaimPossibilities;
        root.Visit([&](const Dir* dir)
        {
            const auto size = dir->ComputeSizeRecursive();
            if (size > spaceNeeded)
            {
                reclaimPossibilities.emplace_back(size);
            }
        });

        std::cout << *(std::min_element(reclaimPossibilities.begin(), reclaimPossibilities.end())) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}