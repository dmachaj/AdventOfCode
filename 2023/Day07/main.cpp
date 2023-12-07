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

namespace Part1Impl
{
    struct Hand
    {
        std::string Cards;
        uint32_t Bid{};
        std::array<uint32_t, 13> CardVals;
    };

    uint32_t AccumulateIf(std::string const& value, char desired)
    {
        return std::accumulate(value.begin(), value.end(), 0UL,
            [desired](uint32_t val, char c) -> uint32_t
            {
                return (c == desired) ? (val + 1) : val;
            });
    }

    Hand HandFromInput(std::string const& value)
    {
        const auto cards = value.substr(0, 5);
        const auto bid = value.substr(6);

        std::array<uint32_t, 13> CardVals;
        CardVals[0] = AccumulateIf(cards, '2');
        CardVals[1] = AccumulateIf(cards, '3');
        CardVals[2] = AccumulateIf(cards, '4');
        CardVals[3] = AccumulateIf(cards, '5');
        CardVals[4] = AccumulateIf(cards, '6');
        CardVals[5] = AccumulateIf(cards, '7');
        CardVals[6] = AccumulateIf(cards, '8');
        CardVals[7] = AccumulateIf(cards, '9');
        CardVals[8] = AccumulateIf(cards, 'T');
        CardVals[9] = AccumulateIf(cards, 'J');
        CardVals[10] = AccumulateIf(cards, 'Q');
        CardVals[11] = AccumulateIf(cards, 'K');
        CardVals[12] = AccumulateIf(cards, 'A');

        const auto sanity = std::accumulate(CardVals.begin(), CardVals.end(), 0UL);
        if (sanity != 5) throw std::exception("Wrong number of cards in hand");

        return Hand{ std::move(cards), std::strtoul(bid.c_str(), nullptr, 10), std::move(CardVals) };
    }

    bool IsFive(const Hand& val)
    {
        return std::count_if(val.CardVals.begin(), val.CardVals.end(),
            [](auto cardCount) { return cardCount == 5; })
            == 1;
    }

    bool IsFour(const Hand& val)
    {
        return std::count_if(val.CardVals.begin(), val.CardVals.end(),
            [](auto cardCount) { return cardCount == 4; })
            == 1;
    }

    bool IsThree(const Hand& val)
    {
        return std::count_if(val.CardVals.begin(), val.CardVals.end(),
            [](auto cardCount) { return cardCount == 3; })
            == 1;
    }

    bool IsTwoPair(const Hand& val)
    {
        return std::count_if(val.CardVals.begin(), val.CardVals.end(),
            [](auto cardCount) { return cardCount == 2; })
            == 2;
    }

    bool IsPair(const Hand& val)
    {
        return std::count_if(val.CardVals.begin(), val.CardVals.end(),
            [](auto cardCount) { return cardCount == 2; })
            == 1;
    }

    bool IsFullHouse(const Hand& val)
    {
        bool foundThree = IsThree(val);
        bool foundPair = IsPair(val);
        return foundThree && foundPair;
    }

    bool ComparCardLessThan(char left, char right)
    {
        const auto normalizeCard = [](char c) -> char
        {
            if (c == 'A') return (char)100;
            if (c == 'K') return (char)99;
            if (c == 'Q') return (char)98;
            if (c == 'J') return (char)97;
            if (c == 'T') return (char)96;
            return c;
        };

        return normalizeCard(left) < normalizeCard(right);
    }

    bool TieBreakLessThan(const Hand& left, const Hand& right)
    {
        const auto& leftStr = left.Cards;
        const auto& rightStr = right.Cards;

        for (auto i = 0; i < 5; ++i)
        {
            if (leftStr[i] != rightStr[i])
            {
                return ComparCardLessThan(leftStr[i], rightStr[i]);
            }
        }
        throw std::exception("Hands are equal");
    }

    bool operator<(const Hand& left, const Hand& right)
    {
        if (IsFive(left) != IsFive(right))
        {
            return IsFive(right);
        }
        else if (IsFour(left) != IsFour(right))
        {
            return IsFour(right);
        }
        else if (IsFullHouse(left) != IsFullHouse(right))
        {
            return IsFullHouse(right);
        }
        else if (IsThree(left) != IsThree(right))
        {
            return IsThree(right);
        }
        else if (IsTwoPair(left) != IsTwoPair(right))
        {
            return IsTwoPair(right);
        }
        else if (IsPair(left) != IsPair(right))
        {
            return IsPair(right);
        }

        return TieBreakLessThan(left, right);
    }
}

void Part1()
{
    std::vector<Part1Impl::Hand> hands;

    std::string input;
    while (std::getline(std::cin, input))
    {
        hands.emplace_back(Part1Impl::HandFromInput(input));
    }

    std::sort(hands.begin(), hands.end());

    uint64_t result{};
    for (auto i = 0; i < hands.size(); ++i)
    {
        const auto& hand = hands[i];
        result += (hand.Bid * (i + 1));
    }

    std::cout << result << std::endl;
}

namespace Part2Impl
{
    struct Hand
    {
        std::string Cards;
        uint64_t Bid{};
        std::array<uint32_t, 12> CardVals;
        uint32_t Jokers{};
    };

    uint32_t AccumulateIf(std::string const& value, char desired)
    {
        return std::accumulate(value.begin(), value.end(), 0UL,
            [desired](uint32_t val, char c) -> uint32_t
            {
                return (c == desired) ? (val + 1) : val;
            });
    }

    Hand HandFromInput(std::string const& value)
    {
        const auto cards = value.substr(0, 5);
        const auto bid = value.substr(6);

        std::array<uint32_t, 12> CardVals;
        CardVals[0] = AccumulateIf(cards, '2');
        CardVals[1] = AccumulateIf(cards, '3');
        CardVals[2] = AccumulateIf(cards, '4');
        CardVals[3] = AccumulateIf(cards, '5');
        CardVals[4] = AccumulateIf(cards, '6');
        CardVals[5] = AccumulateIf(cards, '7');
        CardVals[6] = AccumulateIf(cards, '8');
        CardVals[7] = AccumulateIf(cards, '9');
        CardVals[8] = AccumulateIf(cards, 'T');
        CardVals[9] = AccumulateIf(cards, 'Q');
        CardVals[10] = AccumulateIf(cards, 'K');
        CardVals[11] = AccumulateIf(cards, 'A');

        uint32_t Jokers = AccumulateIf(cards, 'J');

        const auto sanity = std::accumulate(CardVals.begin(), CardVals.end(), 0UL) + Jokers;
        if (sanity != 5) throw std::exception("Wrong number of cards in hand");

        return Hand{ std::move(cards), std::strtoull(bid.c_str(), nullptr, 10), std::move(CardVals), Jokers };
    }

    bool IsFive(const Hand& val)
    {
        if (val.Jokers == 5) return true;
        
        const auto fives = std::count_if(val.CardVals.begin(), val.CardVals.end(), [](auto cardCount) { return cardCount == 5; });
        const auto fours = std::count_if(val.CardVals.begin(), val.CardVals.end(), [](auto cardCount) { return cardCount == 4; });
        const auto threes = std::count_if(val.CardVals.begin(), val.CardVals.end(), [](auto cardCount) { return cardCount == 3; });
        const auto twos = std::count_if(val.CardVals.begin(), val.CardVals.end(), [](auto cardCount) { return cardCount == 2; });

        if (fives) return true;
        if (fours && val.Jokers == 1) return true;
        if (threes && val.Jokers == 2) return true;
        if (twos == 1 && val.Jokers == 3) return true;
        if (val.Jokers == 4) return true;
        return false;
    }

    bool IsFour(const Hand& val)
    {
        const auto fours = std::count_if(val.CardVals.begin(), val.CardVals.end(), [](auto cardCount) { return cardCount == 4; });
        const auto threes = std::count_if(val.CardVals.begin(), val.CardVals.end(), [](auto cardCount) { return cardCount == 3; });
        const auto twos = std::count_if(val.CardVals.begin(), val.CardVals.end(), [](auto cardCount) { return cardCount == 2; });

        if (fours) return true;
        if (threes && val.Jokers == 1) return true;
        if (twos == 1 && val.Jokers == 2) return true;
        if (val.Jokers == 3) return true;
        return false;
    }

    bool IsFullHouse(const Hand& val)
    {
        const auto threes = std::count_if(val.CardVals.begin(), val.CardVals.end(), [](auto cardCount) { return cardCount == 3; });
        const auto twos = std::count_if(val.CardVals.begin(), val.CardVals.end(), [](auto cardCount) { return cardCount == 2; });

        if (threes && twos) return true; // natural full house
        if (twos == 2 && val.Jokers == 1) return true;

        return false;
    }

    bool IsThree(const Hand& val)
    {
        const auto threes = std::count_if(val.CardVals.begin(), val.CardVals.end(), [](auto cardCount) { return cardCount == 3; });
        const auto twos = std::count_if(val.CardVals.begin(), val.CardVals.end(), [](auto cardCount) { return cardCount == 2; });
        if (threes) return true;
        if (twos == 1 && val.Jokers == 1) return true;
        if (val.Jokers == 2) return true;
        return false;
    }

    bool IsTwoPair(const Hand& val)
    {
        // A joker won't affect two pair because it would always prefer
        // to convert the first pair into three of a kind instead of completing
        // a second pair.
        return std::count_if(val.CardVals.begin(), val.CardVals.end(),
            [](auto cardCount) { return cardCount == 2; })
            == 2;
    }

    bool IsPair(const Hand& val)
    {
        const auto pair = std::count_if(val.CardVals.begin(), val.CardVals.end(),
            [](auto cardCount) { return cardCount == 2; })
            == 1;

        return (pair || (val.Jokers == 1));
    }

    bool ComparCardLessThan(char left, char right)
    {
        const auto normalizeCard = [](char c) -> char
        {
            if (c == 'A') return (char)100;
            if (c == 'K') return (char)99;
            if (c == 'Q') return (char)98;
            if (c == 'J') return (char)1; // Jokers now weakest
            if (c == 'T') return (char)96;
            return c;
        };

        return normalizeCard(left) < normalizeCard(right);
    }

    bool TieBreakLessThan(const Hand& left, const Hand& right)
    {
        const auto& leftStr = left.Cards;
        const auto& rightStr = right.Cards;

        for (auto i = 0; i < 5; ++i)
        {
            if (leftStr[i] != rightStr[i])
            {
                return ComparCardLessThan(leftStr[i], rightStr[i]);
            }
        }
        throw std::exception("Hands are equal");
    }

    bool operator<(const Hand& left, const Hand& right)
    {
        if (IsFive(left) != IsFive(right))
        {
            return IsFive(right);
        }
        else if (IsFive(left))
        {
            return TieBreakLessThan(left, right);
        }
        else if (IsFour(left) != IsFour(right))
        {
            return IsFour(right);
        }
        else if (IsFour(left))
        {
            return TieBreakLessThan(left, right);
        }
        else if (IsFullHouse(left) != IsFullHouse(right))
        {
            return IsFullHouse(right);
        }
        else if (IsFullHouse(left))
        {
            return TieBreakLessThan(left, right);
        }
        else if (IsThree(left) != IsThree(right))
        {
            return IsThree(right);
        }
        else if (IsThree(left))
        {
            return TieBreakLessThan(left, right);
        }
        else if (IsTwoPair(left) != IsTwoPair(right))
        {
            return IsTwoPair(right);
        }
        else if (IsTwoPair(left))
        {
            return TieBreakLessThan(left, right);
        }
        else if (IsPair(left) != IsPair(right))
        {
            return IsPair(right);
        }

        return TieBreakLessThan(left, right);
    }
}

void Part2()
{
    std::vector<Part2Impl::Hand> hands;

    std::string input;
    while (std::getline(std::cin, input))
    {
        hands.emplace_back(Part2Impl::HandFromInput(input));
    }

    std::sort(hands.begin(), hands.end());

    uint64_t result{};
    for (auto i = 0ULL; i < hands.size(); ++i)
    {
        const auto& hand = hands[i];
        result += (hand.Bid * (i + 1));
    }

    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}