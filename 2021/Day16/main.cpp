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
    void TEST(uint64_t first, uint64_t second)
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

    uint64_t BinToInt(string_view input)
    {
        uint64_t result{};
        for (auto i = 0; i < input.size(); ++i)
        {
            if (input[i] == '1')
            {
                result += (1ULL << (input.size() - i - 1ULL));
            }
        }
        return result;
    }

    struct NibbleResult
    {
        uint64_t versionSum{};
        uint64_t result{};
        std::string_view nextPacket{};
    };

    NibbleResult NibblePacket(std::string_view input)
    {
        if (input.size() == 0) return {0ULL, 0ULL, ""sv};

        const string_view versionStr = input.substr(0, 3);
        const auto version = BinToInt(versionStr);

        const string_view typeStr = input.substr(3, 3);
        const auto type = BinToInt(typeStr);

        uint64_t mathTotal{};
        uint64_t versionTotal{ version };
        string_view nextPacket;
        if (type == 4)
        {
            std::string numberBits;
            uint64_t skipAmount{5};
            auto numberStr = input.substr(6);
            while (numberStr[0] == '1')
            {
                skipAmount += 5;
                numberBits += numberStr.substr(1, 4);
                numberStr = numberStr.substr(5);
            }
            numberBits += numberStr.substr(1, 4);
            mathTotal = BinToInt(numberBits);
            nextPacket = numberStr.substr(5);

            // uint64_t value{};
            // uint64_t skipAmount{5};
            // auto numberStr = input.substr(6);
            // while (numberStr[0] == '1')
            // {
            //     auto temp = BinToInt(numberStr.substr(1, 4));
            //     value = value << ((skipAmount % 5) - 1);
            //     value += temp;

            //     skipAmount += 5;
            //     numberStr = numberStr.substr(5);
            // }
            // nextPacket = numberStr.substr(5);
        }
        else
        {
            std::vector<uint64_t> subResults;
            if (input[6] == '0') // 15 bits
            {
                auto subpacketLengthStr = input.substr(7, 15);
                auto subpacketLength = BinToInt(subpacketLengthStr);

                nextPacket = input.substr(7 + 15);
                while ((uint64_t)std::distance(input.begin(), nextPacket.begin()) < (subpacketLength + 7ULL + 15ULL))
                {
                    auto result = NibblePacket(nextPacket);

                    nextPacket = result.nextPacket;
                    versionTotal += result.versionSum;
                    subResults.emplace_back(result.result);
                }
            }
            else if (input[6] == '1') // 11 bits
            {
                auto subpacketCountStr = input.substr(7, 11);
                auto subpacketCount = BinToInt(subpacketCountStr);
                nextPacket = input.substr(7 + 11);
                for (auto i = 0ULL; i < subpacketCount; ++i)
                {
                    auto result = NibblePacket(nextPacket);
                    versionTotal += result.versionSum;
                    nextPacket = result.nextPacket;
                    subResults.emplace_back(result.result);
                }
            }

            if (type == 0) // sum
            {
                for (const auto result : subResults)
                {
                    mathTotal += result;
                }
            }
            else if (type == 1) // product
            {
                mathTotal = 1;
                for (const auto result : subResults)
                {
                    mathTotal *= result;
                }
            }
            else if (type == 2) // min
            {
                mathTotal = *std::min_element(subResults.begin(), subResults.end());
            }
            else if (type == 3) // max
            {
                mathTotal = *std::max_element(subResults.begin(), subResults.end());
            }
            else if (type == 5) // greater than
            {
                if (subResults.size() != 2) throw std::exception();
                mathTotal = (subResults[0] > subResults[1]) ? 1ULL : 0ULL;
            }
            else if (type == 6) // less than
            {
                if (subResults.size() != 2) throw std::exception();
                mathTotal = (subResults[0] < subResults[1]) ? 1ULL : 0ULL;
            }
            else if (type == 7) // equal
            {
                if (subResults.size() != 2) throw std::exception();
                mathTotal = (subResults[0] == subResults[1]) ? 1ULL : 0ULL;
            }
            else throw std::exception();
        }

        return {versionTotal, mathTotal, nextPacket};
    }

    void Part1()
    {
        TEST(BinToInt("001"sv), 1ULL);
        TEST(BinToInt("010"sv), 2ULL);
        TEST(BinToInt("011"sv), 3ULL);
        TEST(BinToInt("100"sv), 4ULL);
        TEST(BinToInt("101"sv), 5ULL);
        TEST(BinToInt("110"sv), 6ULL);
        TEST(BinToInt("111"sv), 7ULL);

        std::string input;
        std::getline(std::cin, input);

        std::string binary;
        for (auto c : input)
        {
            binary += HexToStr(c);
        }

        const auto nibble = NibblePacket(binary);
        std::cout << nibble.versionSum << std::endl;
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

        const auto nibble = NibblePacket(binary);
        std::cout << nibble.result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}