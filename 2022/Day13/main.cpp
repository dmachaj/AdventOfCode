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
#include <assert.h>
#include <variant>
#include <optional>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

namespace
{
    struct List
    {
        std::vector<std::variant<int, List>> list{};
        bool special{false};

        List(const std::string& input, size_t& offset)
        {
            if (input == "[6]" || input == "[2]") special = true;

            if (input[offset] != '[') throw std::exception();
            ++offset;

            while (input[offset] != ']')
            {
                if (input[offset] == '[')
                {
                    list.emplace_back(List{input, offset});
                    offset++; // eat the closing bracket for the list we just parsed
                    if (input[offset] == ',') { ++offset; }
                }
                else
                {
                    auto comma = input.find(',', offset);
                    auto close = input.find(']', offset);
                    auto end = std::min(comma, close);

                    auto valueStr = input.substr(offset, end - offset);
                    auto value = std::atoi(valueStr.c_str());
                    list.emplace_back(value);

                    offset = end;
                    if (end == comma) { ++offset; } // eat the comma after the value
                }
            }
        }

        List(int value)
        {
            list.emplace_back(value);
        }

        List() = default;
    };

    struct Packet
    {
        List left;
        List right;
        bool isValid{false};

        Packet(const std::string& leftStr, const string& rightStr)
        {
            size_t offset{};
            left = List(leftStr, offset);
            if (offset != leftStr.length() - 1) { throw std::exception(); }

            offset = 0;
            right = List(rightStr, offset);
            if (offset != rightStr.length() - 1) { throw std::exception(); }

            isValid = CompareListsRecursive(left, right).value();
        }

        static std::optional<bool> CompareListsRecursive(List leftList, List rightList) // deep copy you n00b
        {
            for (auto i = 0; i < leftList.list.size(); ++i)
            {
                if (rightList.list.size() <= i)
                {
                    return false;
                }

                if (std::holds_alternative<int>(leftList.list[i]) && std::holds_alternative<int>(rightList.list[i]))
                {
                    if (std::get<0>(leftList.list[i]) < std::get<0>(rightList.list[i])) return true;
                    if (std::get<0>(leftList.list[i]) > std::get<0>(rightList.list[i])) return false;
                    continue;
                }
                else if (std::holds_alternative<List>(leftList.list[i]) && std::holds_alternative<List>(rightList.list[i]))
                {
                    // both lists - fallthrough
                }
                else if (std::holds_alternative<int>(leftList.list[i]))
                {
                    // left is int, right is list
                    std::variant<int, List> newLeft = List{std::get<0>(leftList.list[i])};

                    std::swap(leftList.list[i], newLeft);
                }
                else
                {
                    // right is int, left is list
                    std::variant<int, List> newRight = List{std::get<0>(rightList.list[i])};
                    std::swap(rightList.list[i], newRight);
                }

                auto result = CompareListsRecursive(std::get<1>(leftList.list[i]), std::get<1>(rightList.list[i]));
                if (result.has_value()) { return result.value(); }
            }

            if (leftList.list.size() < rightList.list.size())
            {
                return true;
            }
            return nullopt; // inconclusive
        }
    };

    bool operator<(const List& left, const List& right)
    {
        return Packet::CompareListsRecursive(left, right).value();
    }

    void Part1()
    {
        std::vector<Packet> packets;

        std::string input;
        while (std::getline(std::cin, input))
        {
            std::string input2;
            std::getline(std::cin, input2);
            std::cin.get(); // eat newline

            packets.emplace_back(Packet{input, input2});
        }

        uint32_t total{};
        for (auto i = 0; i < packets.size(); ++i)
        {
            if (packets[i].isValid)
            {
                total += (i + 1);
            }
        }

        std::cout << total << std::endl;
    }

    void Part2()
    {
        std::vector<List> packets;
        {
            size_t offset{};
            packets.emplace_back(List{"[2]", offset});
            offset = 0;
            packets.emplace_back(List{"[6]", offset});
        }

        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) continue;

            size_t offset{};
            packets.emplace_back(List{input, offset});
        }

        std::sort(packets.begin(), packets.end());

        uint32_t special1{999};
        uint32_t special2{999};
        for (auto i = 0; i < packets.size(); ++i)
        {
            if (packets[i].special && special1 == 999)
            {
                special1 = i + 1;
            }
            else if (packets[i].special)
            {
                special2 = i + 1;
            }
        }

        std::cout << (special1 * special2) << std::endl;
    }

    void TEST()
    {
        {
            size_t offset{};
            List testee("[99]", offset);
            assert(offset == 3);
            assert(testee.list.size() == 1);
            assert(std::get<0>(testee.list[0]) == 99);
        }

        {
            size_t offset{};
            List testee("[99,100]", offset);
            assert(offset == 7);
            assert(testee.list.size() == 2);
            assert(std::get<0>(testee.list[0]) == 99);
            assert(std::get<0>(testee.list[1]) == 100);
        }

        {
            size_t offset{};
            List testee("[[1],4]", offset);
            assert(offset == 6);
            assert(testee.list.size() == 2);
            auto& subList = std::get<1>(testee.list[0]);
            assert(subList.list.size() == 1);
            assert(std::get<0>(subList.list[0]) == 1);
            assert(std::get<0>(testee.list[1]) == 4);
        }

        {
            Packet testee("[9]", "[[8,7,6]]");
            assert(!testee.isValid);
        }

        {
            Packet testee("[[]]", "[[[]]]");
            assert(testee.isValid);
        }

        {
            Packet testee("[[1,2,3],3]", "[[1,2,3],4]");
            assert(testee.isValid);
        }
        
        {
            Packet testee("[[1,2,3],5]", "[[1,2,3],4]");
            assert(!testee.isValid);
        }
    }
}

int main()
{
    TEST();

    RunPart1() ? Part1() : Part2();
    return 0;
}