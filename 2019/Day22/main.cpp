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

    uint64_t FollowNewStackBackwards(uint64_t deckSize, uint64_t currentLocation)
    {
        return deckSize - currentLocation - 1;
    }

    uint64_t FollowCutBackwards(uint64_t deckSize, uint64_t currentLocation, int64_t cutSize)
    {
        auto newLocation = (int64_t)currentLocation + cutSize;
        if (newLocation > (int64_t)deckSize)
        {
            newLocation %= deckSize;
        }
        else if (newLocation < 0)
        {
            newLocation += deckSize;
        }
        return (uint64_t)newLocation;
    }

    uint64_t FollowDealIncrementBackwards(uint64_t deckSize, uint64_t endingLocation, uint64_t dealCount)
    {
        // Dumb solution.  Seems to work ok but far too slow to go through trillions of elements.
        //
        // uint64_t incrementCounter = 0ULL;
        // int64_t currentLocation = (int64_t)endingLocation;
        //
        // while (currentLocation != 0)
        // {
        //     ++incrementCounter;
        //     currentLocation -= dealCount;
        //     if (currentLocation < 0)
        //     {
        //         currentLocation += deckSize;
        //     }
        // }
        //
        // return incrementCounter;

        // Easy answer 0 doesn't move
        if (endingLocation == 0) { return 0ULL; }

        // Figure out how many iterations through the deck it took to get to this index.  Adding the deckSize
        // until it divides evenly by the dealCount will tell us that.  Dividing that by the dealCount will
        // tell us the original location of this index before the shuffle.
        //
        // This took me an hour of staring at a postit to figure out :|
        while (endingLocation % dealCount != 0)
        {
            endingLocation += deckSize;

            if (endingLocation < deckSize) { throw std::exception("Int64 overflow"); }
        }
        return endingLocation / dealCount;
    }

    struct ShuffleCommand
    {
        enum class Type
        {
            DealNewStack,
            DealWithIncrement,
            Cut
        };
        Type type;
        int64_t amount;
    };

    uint64_t FollowShuffleBackwards(uint64_t deckSize, const std::vector<ShuffleCommand>& commands, uint64_t targetCardLocation)
    {
        uint64_t currentLocation = targetCardLocation;

        for (auto commandIter = commands.rbegin(); commandIter != commands.rend(); ++commandIter)
        {
            auto command = *commandIter;
            if (command.type == ShuffleCommand::Type::DealNewStack)
            {
                currentLocation = FollowNewStackBackwards(deckSize, currentLocation);
            }
            else if (command.type == ShuffleCommand::Type::DealWithIncrement)
            {
                currentLocation = FollowDealIncrementBackwards(deckSize, currentLocation, command.amount);
            }
            else if (command.type == ShuffleCommand::Type::Cut)
            {
                currentLocation = FollowCutBackwards(deckSize, currentLocation, command.amount);
            }
            else
            {
                throw std::exception("Invalid shuffle");
            }

            if (currentLocation >= deckSize)
            {
                throw std::exception("Sort went out of bounds");
            }
        }

        return currentLocation;
    }

    void TEST(uint64_t expected, uint64_t actual)
    {
        if (expected != actual) throw std::exception();
    }

    void Part2()
    {
        constexpr uint64_t c_targetCardLocation = 2020;

        uint64_t deckSize;
        std::cin >> deckSize;
        if (deckSize != 119315717514047) throw std::exception("Bad input");
        std::cin.get(); // eat newline 

        uint64_t shuffleProcessCount;
        std::cin >> shuffleProcessCount;
        if (shuffleProcessCount != 101741582076661) throw std::exception("Bad input");
        std::cin.get(); // eat newline 

        TEST(99, FollowNewStackBackwards(100, 0));
        TEST(0, FollowNewStackBackwards(100, 99));
        TEST(49, FollowNewStackBackwards(100, 50));
        TEST(50, FollowNewStackBackwards(100, 49));

        TEST(3, FollowCutBackwards(100, 2, 1));
        TEST(1, FollowCutBackwards(100, 2, -1));
        TEST(4, FollowCutBackwards(100, 2, 2));
        TEST(0, FollowCutBackwards(100, 2, -2));
        TEST(99, FollowCutBackwards(100, 2, -3));
        TEST(22, FollowCutBackwards(100, 2, 20));
        TEST(82, FollowCutBackwards(100, 2, -20));

        TEST(0, FollowDealIncrementBackwards(10, 0, 3));
        TEST(7, FollowDealIncrementBackwards(10, 1, 3));
        TEST(4, FollowDealIncrementBackwards(10, 2, 3));
        TEST(1, FollowDealIncrementBackwards(10, 3, 3));
        TEST(8, FollowDealIncrementBackwards(10, 4, 3));
        TEST(5, FollowDealIncrementBackwards(10, 5, 3));
        TEST(2, FollowDealIncrementBackwards(10, 6, 3));
        TEST(9, FollowDealIncrementBackwards(10, 7, 3));
        TEST(6, FollowDealIncrementBackwards(10, 8, 3));
        TEST(3, FollowDealIncrementBackwards(10, 9, 3));

        TEST(0, FollowDealIncrementBackwards(10, 0, 7));
        TEST(3, FollowDealIncrementBackwards(10, 1, 7));
        TEST(6, FollowDealIncrementBackwards(10, 2, 7));
        TEST(9, FollowDealIncrementBackwards(10, 3, 7));
        TEST(2, FollowDealIncrementBackwards(10, 4, 7));
        TEST(5, FollowDealIncrementBackwards(10, 5, 7));
        TEST(8, FollowDealIncrementBackwards(10, 6, 7));
        TEST(1, FollowDealIncrementBackwards(10, 7, 7));
        TEST(4, FollowDealIncrementBackwards(10, 8, 7));
        TEST(7, FollowDealIncrementBackwards(10, 9, 7));

        std::vector<std::string> commandStrings;
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                commandStrings.emplace_back(std::move(input));
            }
        }

        std::vector<ShuffleCommand> commands;
        for (auto& commandStr : commandStrings)
        {
            if (commandStr == "deal into new stack")
            {
                commands.emplace_back(ShuffleCommand{ShuffleCommand::Type::DealNewStack, 0});
            }
            else if (commandStr.starts_with("deal with"))
            {
                std::string dealCommandStr = commandStr.substr("deal with increment "sv.length());
                std::istringstream dealStream(dealCommandStr);
                uint32_t dealCount;
                dealStream >> dealCount;
                commands.emplace_back(ShuffleCommand{ShuffleCommand::Type::DealWithIncrement, dealCount});
            }
            else if (commandStr.starts_with("cut"))
            {
                std::string cutCountStr = commandStr.substr(4);
                std::istringstream cutStream(cutCountStr);
                int64_t cutCount;
                cutStream >> cutCount;
                commands.emplace_back(ShuffleCommand{ShuffleCommand::Type::Cut, cutCount});
            }
        }

        // Algorithm is as follows:
        // Keep an eye on the 2020th spot in the deck.  Follow shuffle backwards until the beginning.  That is our answer for one shuffle.
        // Run the shuffle repeatedly to figure out the cycle count (e.g. 10 shuffles and you're back where you're started).
        // The shuffle count MOD cycle count is how many shuffles actually matter.

        uint64_t iterationsUntilCycle = 0;
        auto currentCardLocation = c_targetCardLocation;
        do
        {
            ++iterationsUntilCycle;
            currentCardLocation = FollowShuffleBackwards(deckSize, commands, currentCardLocation);
        } while (currentCardLocation != c_targetCardLocation);

        std::cerr << "Iterations until cycle: " << iterationsUntilCycle << std::endl;

        uint64_t trueIterationCount = shuffleProcessCount / iterationsUntilCycle;
        currentCardLocation = c_targetCardLocation;
        for (auto i = 0ULL; i < trueIterationCount; ++i)
        {
            currentCardLocation = FollowShuffleBackwards(deckSize, commands, currentCardLocation);
        }

        std::cout << currentCardLocation << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}