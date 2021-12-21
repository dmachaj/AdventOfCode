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
    void Part1()
    {
        std::string input;
        std::getline(std::cin, input);
        uint32_t player1 = std::atoi(input.substr(input.find(':') + 1).c_str());

        std::getline(std::cin, input);
        uint32_t player2 = std::atoi(input.substr(input.find(':') + 1).c_str());

        uint64_t player1Score{};
        uint64_t player2Score{};
        uint64_t diceRolls{};
        uint64_t currentDieValue{1};
        uint64_t player1CurrentSpace{player1};
        uint64_t player2CurrentSpace{player2};
        while (true)
        {
            // Player 1
            if ((player1Score >= 1000) || (player2Score >= 1000)) break;

            uint64_t player1Roll = currentDieValue++;
            if (currentDieValue > 100) currentDieValue = 1;
            player1Roll += currentDieValue++;
            if (currentDieValue > 100) currentDieValue = 1;
            player1Roll += currentDieValue++;
            if (currentDieValue > 100) currentDieValue = 1;
            diceRolls += 3;

            player1CurrentSpace = (player1CurrentSpace + player1Roll);
            while (player1CurrentSpace > 10) player1CurrentSpace -= 10;

            player1Score += player1CurrentSpace;

            // Player 2
            if ((player1Score >= 1000) || (player2Score >= 1000)) break;

            uint64_t player2Roll = currentDieValue++;
            if (currentDieValue > 100) currentDieValue = 1;
            player2Roll += currentDieValue++;
            if (currentDieValue > 100) currentDieValue = 1;
            player2Roll += currentDieValue++;
            if (currentDieValue > 100) currentDieValue = 1;
            diceRolls += 3;

            player2CurrentSpace = (player2CurrentSpace + player2Roll);
            while (player2CurrentSpace > 10) player2CurrentSpace -= 10;

            player2Score += player2CurrentSpace;
        }

        std::cout << (diceRolls * std::min(player1Score, player2Score)) << std::endl;
    }

    void Part2()
    {
        std::string input;
        std::getline(std::cin, input);
        uint32_t player1 = std::atoi(input.substr(input.find(':') + 1).c_str());

        std::getline(std::cin, input);
        uint32_t player2 = std::atoi(input.substr(input.find(':') + 1).c_str());

        struct GameState
        {
            uint32_t player1Pos;
            uint32_t player2Pos;
            uint32_t player1Score;
            uint32_t player2Score;
            bool player1Next;
            bool isOver;
        };
        
        struct GameStateHash
        {
            size_t operator()(const GameState& state) const noexcept
            {
                auto h1 = std::hash<int64_t>{}(state.player1Pos);
                auto h2 = std::hash<int64_t>{}(state.player2Pos);
                auto h3 = std::hash<int64_t>{}(state.player1Score);
                auto h4 = std::hash<int64_t>{}(state.player2Score);
                auto h5 = std::hash<int64_t>{}(state.player1Next);
                auto h6 = std::hash<int64_t>{}(state.isOver);
                return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5);
            }
        };

        struct GameStateEqual
        {
            size_t operator()(const GameState& left, const GameState& right) const noexcept
            {
                return (left.player1Pos == right.player1Pos) &&
                        (left.player2Pos == right.player2Pos) &&
                        (left.player1Score == right.player1Score) &&
                        (left.player2Score == right.player2Score) &&
                        (left.player1Next == right.player1Next) &&
                        (left.isOver == right.isOver);
            }
        };

        GameState starting { player1, player2, 0UL, 0UL, true, false };
        std::unordered_map<GameState, uint64_t, GameStateHash, GameStateEqual> gameStateCounts;
        gameStateCounts[starting] = 1ULL;
        
        while (std::find_if(gameStateCounts.begin(), gameStateCounts.end(), [](const auto& state) { return !state.first.isOver; }) != gameStateCounts.end())
        {
            std::unordered_map<GameState, uint64_t, GameStateHash, GameStateEqual> gameStateTemp;
            for (const auto& state : gameStateCounts)
            {
                if (state.first.isOver)
                {
                    gameStateTemp[state.first] += state.second;
                }
                else
                {
                    auto stateCopyClean = state.first;
                    stateCopyClean.player1Next = !state.first.player1Next;
                    if (state.first.player1Next)
                    {
                        // The 3 rolls have 3^3 possibilities
                        // 1-1-1    3
                        // 1-1-2    4
                        // 1-1-3    5
                        // 1-2-1    4
                        // 1-2-2    5
                        // 1-2-3    6
                        // 1-3-1    5
                        // 1-3-2    6
                        // 1-3-3    7
                        // 2-1-1    4
                        // 2-1-2    5
                        // 2-1-3    6
                        // 2-2-1    5
                        // 2-2-2    6
                        // 2-2-3    7
                        // 2-3-1    6
                        // 2-3-2    7
                        // 2-3-3    8
                        // 3-1-1    5
                        // 3-1-2    6
                        // 3-1-3    7
                        // 3-2-1    6
                        // 3-2-2    7
                        // 3-2-3    8
                        // 3-3-1    7
                        // 3-3-2    8
                        // 3-3-3    9
                        //
                        // Since the order doesn't matter, only the sum does, that gives us these
                        // posibilities
                        // 3 = 1
                        // 4 = 3
                        // 5 = 6
                        // 6 = 7
                        // 7 = 6
                        // 8 = 3
                        // 9 = 1

                        auto stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player1Pos += 3;
                        if (stateCopyTemp.player1Pos > 10) stateCopyTemp.player1Pos -= 10;
                        stateCopyTemp.player1Score += stateCopyTemp.player1Pos;
                        if (stateCopyTemp.player1Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += state.second;
                        
                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player1Pos += 4;
                        if (stateCopyTemp.player1Pos > 10) stateCopyTemp.player1Pos -= 10;
                        stateCopyTemp.player1Score += stateCopyTemp.player1Pos;
                        if (stateCopyTemp.player1Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += (state.second * 3);
                        
                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player1Pos += 5;
                        if (stateCopyTemp.player1Pos > 10) stateCopyTemp.player1Pos -= 10;
                        stateCopyTemp.player1Score += stateCopyTemp.player1Pos;
                        if (stateCopyTemp.player1Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += (state.second * 6);
                        
                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player1Pos += 6;
                        if (stateCopyTemp.player1Pos > 10) stateCopyTemp.player1Pos -= 10;
                        stateCopyTemp.player1Score += stateCopyTemp.player1Pos;
                        if (stateCopyTemp.player1Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += (state.second * 7);
                        
                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player1Pos += 7;
                        if (stateCopyTemp.player1Pos > 10) stateCopyTemp.player1Pos -= 10;
                        stateCopyTemp.player1Score += stateCopyTemp.player1Pos;
                        if (stateCopyTemp.player1Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += (state.second * 6);
                        
                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player1Pos += 8;
                        if (stateCopyTemp.player1Pos > 10) stateCopyTemp.player1Pos -= 10;
                        stateCopyTemp.player1Score += stateCopyTemp.player1Pos;
                        if (stateCopyTemp.player1Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += (state.second * 3);

                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player1Pos += 9;
                        if (stateCopyTemp.player1Pos > 10) stateCopyTemp.player1Pos -= 10;
                        stateCopyTemp.player1Score += stateCopyTemp.player1Pos;
                        if (stateCopyTemp.player1Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += state.second;
                    }
                    else
                    {
                        auto stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player2Pos += 3;
                        if (stateCopyTemp.player2Pos > 10) stateCopyTemp.player2Pos -= 10;
                        stateCopyTemp.player2Score += stateCopyTemp.player2Pos;
                        if (stateCopyTemp.player2Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += state.second;
                        
                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player2Pos += 4;
                        if (stateCopyTemp.player2Pos > 10) stateCopyTemp.player2Pos -= 10;
                        stateCopyTemp.player2Score += stateCopyTemp.player2Pos;
                        if (stateCopyTemp.player2Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += (state.second * 3);
                        
                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player2Pos += 5;
                        if (stateCopyTemp.player2Pos > 10) stateCopyTemp.player2Pos -= 10;
                        stateCopyTemp.player2Score += stateCopyTemp.player2Pos;
                        if (stateCopyTemp.player2Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += (state.second * 6);
                        
                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player2Pos += 6;
                        if (stateCopyTemp.player2Pos > 10) stateCopyTemp.player2Pos -= 10;
                        stateCopyTemp.player2Score += stateCopyTemp.player2Pos;
                        if (stateCopyTemp.player2Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += (state.second * 7);
                        
                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player2Pos += 7;
                        if (stateCopyTemp.player2Pos > 10) stateCopyTemp.player2Pos -= 10;
                        stateCopyTemp.player2Score += stateCopyTemp.player2Pos;
                        if (stateCopyTemp.player2Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += (state.second * 6);
                        
                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player2Pos += 8;
                        if (stateCopyTemp.player2Pos > 10) stateCopyTemp.player2Pos -= 10;
                        stateCopyTemp.player2Score += stateCopyTemp.player2Pos;
                        if (stateCopyTemp.player2Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += (state.second * 3);

                        stateCopyTemp = stateCopyClean;
                        stateCopyTemp.player2Pos += 9;
                        if (stateCopyTemp.player2Pos > 10) stateCopyTemp.player2Pos -= 10;
                        stateCopyTemp.player2Score += stateCopyTemp.player2Pos;
                        if (stateCopyTemp.player2Score >= 21) stateCopyTemp.isOver = true;
                        gameStateTemp[stateCopyTemp] += state.second;
                    }
                }
            }
            gameStateCounts = gameStateTemp;
        }

        uint64_t player1Wins = std::accumulate(gameStateCounts.begin(), gameStateCounts.end(), 0ULL, 
            [](uint64_t sum, const auto& state)
            {
                if (state.first.player1Score > state.first.player2Score) return sum + state.second;
                return sum;
            });
        
        uint64_t player2Wins = std::accumulate(gameStateCounts.begin(), gameStateCounts.end(), 0ULL, 
            [](uint64_t sum, const auto& state)
            {
                if (state.first.player1Score < state.first.player2Score) return sum + state.second;
                return sum;
            });

        std::cout << std::max(player1Wins, player2Wins) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}