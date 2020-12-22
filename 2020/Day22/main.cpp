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
#include <queue>
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
        std::string input;
        std::getline(std::cin, input); // player name

        std::queue<uint32_t> player1;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;
            player1.push((uint32_t)std::atoi(input.c_str()));
        }

        std::getline(std::cin, input); // player name

        std::queue<uint32_t> player2;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;
            player2.push((uint32_t)std::atoi(input.c_str()));
        }

        while ((player1.size() != 0) && (player2.size() != 0))
        {
            const auto p1 = player1.front();
            player1.pop();
            const auto p2 = player2.front();
            player2.pop();

            if (p1 > p2)
            {
                player1.push(p1);
                player1.push(p2);
            }
            else
            {
                player2.push(p2);
                player2.push(p1);
            }
        }

        const auto TotalScore = [](std::queue<uint32_t> deck)
        {
            uint64_t result{};
            for (auto i = deck.size(); i > 0; --i)
            {
                result += (deck.front() * i);
                deck.pop();
            }
            return result;
        };

        if (player1.empty())
        {
            std::cout << TotalScore(player2) << std::endl;
        }
        else
        {
            std::cout << TotalScore(player1) << std::endl;
        }
    }

    // True == cache hit win
    bool RecursiveCombat(std::queue<uint32_t>& player1, std::queue<uint32_t>& player2, 
        std::vector<std::queue<uint32_t>>& player1Cache, std::vector<std::queue<uint32_t>>& player2Cache, uint32_t depth)
    {
        // const auto Indent = [depth]()
        // {
        //     std::string result;
        //     for (auto i = 0UL; i < depth; ++i)
        //     {
        //         result += " ";
        //     }
        //     return result;
        // };

        if (std::find(player1Cache.begin(), player1Cache.end(), player1) != player1Cache.end())
        {
            // std::cerr << Indent() << "Cache hit.  Player 1 wins" << std::endl;
            return true;
        }

        if (std::find(player2Cache.begin(), player2Cache.end(), player2) != player2Cache.end())
        {
            // std::cerr << Indent() << "Cache hit.  Player 2 wins" << std::endl;
            return true;
        }

        // Push to caches after checking both
        player1Cache.push_back(player1);
        player2Cache.push_back(player2);

        const auto p1 = player1.front();
        player1.pop();
        const auto p2 = player2.front();
        player2.pop();

        if ((player1.size() >= p1) && (player2.size() >= p2))
        {
            // recursive combat!
            const auto CopyFirstN = [](uint32_t N, const std::queue<uint32_t>& deck)
            {
                std::queue<uint32_t> deckCopy = deck;
                std::vector<uint32_t> deckVector;
                while (!deckCopy.empty())
                {
                    deckVector.emplace_back(deckCopy.front());
                    deckCopy.pop();
                }
                for (auto i = 0UL; i < N; ++i)
                {
                    deckCopy.push(deckVector[i]);
                }
                return deckCopy;
            };

            std::vector<std::queue<uint32_t>> player1CacheCopy;
            std::vector<std::queue<uint32_t>> player2CacheCopy;
            std::queue<uint32_t> player1Copy = CopyFirstN(p1, player1);
            std::queue<uint32_t> player2Copy = CopyFirstN(p2, player2);
            // std::cerr << Indent() << "Recursive combat" << std::endl;
            while ((player1Copy.size() != 0) && (player2Copy.size() != 0))
            {
                if (RecursiveCombat(player1Copy, player2Copy, player1CacheCopy, player2CacheCopy, depth + 2))
                {
                    // std::cerr << Indent() << "P1 always wins cache hits" << std::endl;
                    break;
                    // Treating the player with the cache hit as the winner was my bug that slowed me down a lot.
                }
            }
            // std::cerr << Indent() << "END Recursive combat" << std::endl;

            if (!player1Copy.empty())
            {
                player1.push(p1);
                player1.push(p2);
                // std::cerr << Indent() << "P1 played: " << p1 << "\tP2 played: " << p2 << "\tPlayer 1 wins!" << std::endl;
            }
            else
            {
                player2.push(p2);
                player2.push(p1);
                // std::cerr << Indent() << "P1 played: " << p1 << "\tP2 played: " << p2 << "\tPlayer 2 wins!" << std::endl;
            }
        }
        else
        {
            if (p1 > p2)
            {
                player1.push(p1);
                player1.push(p2);
                // std::cerr << Indent() << "P1 played: " << p1 << "\tP2 played: " << p2 << "\tPlayer 1 wins!" << std::endl;
            }
            else
            {
                player2.push(p2);
                player2.push(p1);
                // std::cerr << Indent() << "P1 played: " << p1 << "\tP2 played: " << p2 << "\tPlayer 2 wins!" << std::endl;
            }
        }
        return false;
    }

    void Part2()
    {
        std::string input;
        std::getline(std::cin, input); // player name

        std::queue<uint32_t> player1;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;
            player1.push((uint32_t)std::atoi(input.c_str()));
        }

        std::getline(std::cin, input); // player name

        std::queue<uint32_t> player2;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;
            player2.push((uint32_t)std::atoi(input.c_str()));
        }

        std::vector<std::queue<uint32_t>> player1Cache;
        std::vector<std::queue<uint32_t>> player2Cache;
        while ((player1.size() != 0) && (player2.size() != 0))
        {
            RecursiveCombat(player1, player2, player1Cache, player2Cache, 0);
        }

        const auto TotalScore = [](std::queue<uint32_t> deck)
        {
            uint64_t result{};
            for (auto i = deck.size(); i > 0; --i)
            {
                result += (deck.front() * i);
                deck.pop();
            }
            return result;
        };

        if (player1.empty())
        {
            std::cout << TotalScore(player2) << std::endl;
        }
        else
        {
            std::cout << TotalScore(player1) << std::endl;
        }
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}