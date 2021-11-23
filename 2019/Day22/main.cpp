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
    using deck_t = std::list<uint32_t>;

    void DealIntoNewStack(deck_t& deck)
    {
        std::reverse(deck.begin(), deck.end());
    }

    void DealWithIncrement(deck_t& deck, uint32_t dealCount)
    {
        const auto inSize = deck.size();

        std::vector<uint32_t> table(deck.size(), 0);

        uint32_t current{ 0UL };
        while (deck.size() != 0)
        {
            table[current] = deck.front();
            deck.pop_front();

            current += dealCount;
            current = current % table.size();
        }

        std::copy(table.begin(), table.end(), std::back_inserter(deck));
    }

    void Cut(deck_t& deck, int32_t cutCount)
    {
        if (cutCount > 0)
        {
            std::vector<uint32_t> stash;
            stash.reserve(cutCount);
            for (auto i = 0UL; i < (uint32_t)cutCount; ++i)
            {
                stash.emplace_back(deck.front());
                deck.pop_front();
            }

            for (auto card : stash)
            {
                deck.push_back(card);
            }
        }
        else if (cutCount < 0)
        {
            uint32_t absCutCount = std::abs(cutCount);
            
            std::vector<uint32_t> stash;
            stash.reserve(absCutCount);
            for (auto i = 0UL; i < absCutCount; ++i)
            {
                stash.emplace_back(deck.back());
                deck.pop_back();
            }

            for (auto card : stash)
            {
                deck.push_front(card);
            }
        }
    }

    void Part1()
    {
        uint32_t deckSize;
        std::cin >> deckSize;
        if (deckSize != 10 && deckSize != 10007) throw std::exception("Bad input");
        std::cin.get(); // eat newline 

        deck_t deck;
        for (auto i = 0UL; i < deckSize; ++i)
        {
            deck.emplace_back(i);
        }

        std::vector<std::string> commands;
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                commands.emplace_back(std::move(input));
            }
        }

        for (auto& command : commands)
        {
            if (command == "deal into new stack")
            {
                DealIntoNewStack(deck);
            }
            else if (command.starts_with("deal with"))
            {
                std::string dealCommandStr = command.substr("deal with increment "sv.length());
                std::istringstream dealStream(dealCommandStr);
                uint32_t dealCount;
                dealStream >> dealCount;
                DealWithIncrement(deck, dealCount);
            }
            else if (command.starts_with("cut"))
            {
                std::string cutCountStr = command.substr(4);
                std::istringstream cutStream(cutCountStr);
                int32_t cutCount;
                cutStream >> cutCount;
                Cut(deck, cutCount);
            }
        }

        if (deckSize == 10)
        {
            std::ostringstream out;
            for (auto card : deck)
            {
                out << card << " ";
            }
            std::cout << out.str().substr(0, out.str().length() - 1); // super ugly trim of trailing whitespace
        }
        else if (deckSize == 10007)
        {
            std::vector<uint32_t> finalDeck;
            std::copy(deck.begin(), deck.end(), std::back_inserter(finalDeck));
            for (auto i = 0UL; i < finalDeck.size(); ++i)
            {
                if (finalDeck[i] == 2019)
                {
                    std::cout << i << std::endl;
                    break;
                }
            }
        }
    }

    void Part2()
    {
        std::cout << 0 << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}