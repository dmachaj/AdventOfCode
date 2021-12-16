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
    void TEST(int first, int second)
    {
        if (first != second) throw std::exception();
    }

    string HexToStr(char c)
    {
        switch (c)
        {
            case '0': return "0000";
            case '1': return "0001";
            case '2': return "0010";
            case '3': return "0011";
            case '4': return "0100";
            case '5': return "0101";
            case '6': return "0110";
            case '7': return "0111";
            case '8': return "1000";
            case '9': return "1001";
            case 'A': return "1010";
            case 'B': return "1011";
            case 'C': return "1100";
            case 'D': return "1101";
            case 'E': return "1110";
            case 'F': return "1111";
        }
        throw std::exception();
    }

    uint32_t BinToInt(string_view input)
    {
        uint32_t result{};
        for (auto i = 0; i < input.size(); ++i)
        {
            if (input[i] == '1')
            {
                result += (1 << (input.size() - i - 1));
            }
        }
        return result;
    }

    std::pair<uint32_t, string_view> NibblePacket(std::string_view input)
    {
        if (input.size() == 0) return make_pair(0UL, ""sv);

        const string_view versionStr = input.substr(0, 3);
        const auto version = BinToInt(versionStr);

        const string_view typeStr = input.substr(3, 3);
        const auto type = BinToInt(typeStr);

        uint32_t versionTotal{ version };
        string_view nextPacket;
        if (type == 4)
        {
            uint32_t skipAmount{5};
            auto numberStr = input.substr(6);
            while (numberStr[0] == '1')
            {
                skipAmount += 5;
                numberStr = numberStr.substr(5);
            }
            nextPacket = numberStr.substr(5);
        }
        else
        {
            if (input[6] == '0') // 15 bits
            {
                auto subpacketLengthStr = input.substr(7, 15);
                auto subpacketLength = BinToInt(subpacketLengthStr);

                nextPacket = input.substr(7 + 15);
                while (std::distance(input.begin(), nextPacket.begin()) < (subpacketLength + 7 + 15))
                {
                    auto pair = NibblePacket(nextPacket);

                    nextPacket = pair.second;
                    versionTotal += pair.first;
                }
            }
            else if (input[6] == '1') // 11 bits
            {
                auto subpacketCountStr = input.substr(7, 11);
                auto subpacketCount = BinToInt(subpacketCountStr);
                nextPacket = input.substr(7 + 11);
                for (auto i = 0UL; i < subpacketCount; ++i)
                {
                    auto pair = NibblePacket(nextPacket);
                    versionTotal += pair.first;
                    nextPacket = pair.second;
                }
            }
        }

        return make_pair(versionTotal, nextPacket);
    }

    void Part1()
    {
        TEST(BinToInt("001"sv), 1);
        TEST(BinToInt("010"sv), 2);
        TEST(BinToInt("011"sv), 3);
        TEST(BinToInt("100"sv), 4);
        TEST(BinToInt("101"sv), 5);
        TEST(BinToInt("110"sv), 6);
        TEST(BinToInt("111"sv), 7);

        std::string input;
        std::getline(std::cin, input);

        std::string binary;
        for (auto c : input)
        {
            binary += HexToStr(c);
        }

        auto nibble = NibblePacket(binary);
        std::cout << nibble.first << std::endl;
    }

    std::pair<uint32_t, string_view> NibblePacketPart2(std::string_view input)
    {
        if (input.size() == 0) return make_pair(0UL, ""sv);

        const string_view versionStr = input.substr(0, 3);
        const auto version = BinToInt(versionStr);

        const string_view typeStr = input.substr(3, 3);
        const auto type = BinToInt(typeStr);

        uint32_t versionTotal{ version };
        string_view nextPacket;
        if (type == 4)
        {
            uint32_t value{};
            uint32_t skipAmount{5};
            auto numberStr = input.substr(6);
            while (numberStr[0] == '1')
            {
                auto temp = BinToInt(numberStr.substr(1, 4));
                value = value << ((skipAmount % 5) - 1);
                value += temp;

                skipAmount += 5;
                numberStr = numberStr.substr(5);
            }
            nextPacket = numberStr.substr(5);
        }
        else
        {
            if (input[6] == '0') // 15 bits
            {
                auto subpacketLengthStr = input.substr(7, 15);
                auto subpacketLength = BinToInt(subpacketLengthStr);

                nextPacket = input.substr(7 + 15);
                while (std::distance(input.begin(), nextPacket.begin()) < (subpacketLength + 7 + 15))
                {
                    auto pair = NibblePacketPart2(nextPacket);

                    nextPacket = pair.second;
                    versionTotal += pair.first;
                }
            }
            else if (input[6] == '1') // 11 bits
            {
                auto subpacketCountStr = input.substr(7, 11);
                auto subpacketCount = BinToInt(subpacketCountStr);
                nextPacket = input.substr(7 + 11);
                for (auto i = 0UL; i < subpacketCount; ++i)
                {
                    auto pair = NibblePacketPart2(nextPacket);
                    versionTotal += pair.first;
                    nextPacket = pair.second;
                }
            }
        }

        return make_pair(versionTotal, nextPacket);
    }

    void Part2()
    {
        std::string input;
        std::getline(std::cin, input);

        std::string binary;
        for (auto c : input)
        {
            binary += HexToStr(c);
        }

        auto nibble = NibblePacketPart2(binary);
        std::cout << nibble.first << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}