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

std::vector<bool> ParseInput(const std::string& input)
{
    std::vector<bool> moves;
    for (char c : input)
    {
        if (c == '<')
        {
            moves.emplace_back(true);
        }
        else
        {
            moves.emplace_back(false);
        }
    }
    return moves;
}

using Piece = std::vector<int32_t>;

constexpr int32_t c_boardWidth{7};

Piece SpawnBar(int32_t x, int32_t y)
{
    Piece result{
        y * c_boardWidth + x,
        y * c_boardWidth + x + 1,
        y * c_boardWidth + x + 2,
        y * c_boardWidth + x + 3,
    };
    return result;
}

Piece SpawnPlus(int32_t x, int32_t y)
{
    Piece result{
        (y + 2) * c_boardWidth + x + 1,
        (y + 1) * c_boardWidth + x,
        (y + 1) * c_boardWidth + x + 1,
        (y + 1) * c_boardWidth + x + 2,
        y * c_boardWidth + x + 1,
    };
    return result;
}

Piece SpawnL(int32_t x, int32_t y)
{
    Piece result{
        (y + 2) * c_boardWidth + x + 2,
        (y + 1) * c_boardWidth + x + 2,
        y * c_boardWidth + x + 2,
        y * c_boardWidth + x + 1,
        y * c_boardWidth + x,
    };
    return result;
}

Piece SpawnI(int32_t x, int32_t y)
{
    Piece result{
        (y + 3) * c_boardWidth + x,
        (y + 2) * c_boardWidth + x,
        (y + 1) * c_boardWidth + x,
        y * c_boardWidth + x,
    };
    return result;
}

Piece SpawnBox(int32_t x, int32_t y)
{
    Piece result{
        (y + 1) * c_boardWidth + x,
        (y + 1) * c_boardWidth + x + 1,
        y * c_boardWidth + x,
        y * c_boardWidth + x + 1,
    };
    return result;
}

void PrintBottom(const std::vector<char>& board, const Piece& curr, int32_t rowCount)
{
    auto boardCopy = board;
    for (auto box : curr)
    {
        boardCopy[box] = '@';
    }

    for (auto y = rowCount; y >= 0; --y)
    {
        std::cerr << "|";
        for (auto x = 0; x < c_boardWidth; ++x)
        {
            std::cerr << boardCopy[y * c_boardWidth + x];
        }
        std::cerr << "|"<< std::endl;
    }
    std::cerr << "+-------+" << std::endl;
}

void TryMoveLeft(const std::vector<char>& board, Piece& piece)
{
    Piece copy = piece;
    for (auto& box : copy)
    {
        const auto before = box / c_boardWidth;
        --box;
        if (box < 0) return; // underflow
        const auto after = box / c_boardWidth;

        if (before != after)
        {
            return; // hit left wall
        }
        else if (board[box] == '#')
        {
            return; // hit another piece
        }
    }

    std::swap(copy, piece);
}

void TryMoveRight(const std::vector<char>& board, Piece& piece)
{
    Piece copy = piece;
    for (auto& box : copy)
    {
        const auto before = box / c_boardWidth;
        ++box;
        const auto after = box / c_boardWidth;

        if (before != after)
        {
            return; // hit right wall
        }
        else if (board[box] == '#')
        {
            return; // hit another piece
        }
    }

    std::swap(copy, piece);
}

bool TryMoveDown(const std::vector<char>& board, Piece& piece)
{
    Piece copy = piece;
    for (auto& box : copy)
    {
        box -= c_boardWidth;
    }

    for (const auto& box : copy)
    {
        if (box < 0)
        {
            return true;
        }
        else if (board[box] == '#')
        {
            return true;
        }
    }

    std::swap(copy, piece);
    return false;
}

int32_t RunSimulation(const std::vector<bool>& moves, std::function<bool(int32_t, int32_t, bool, int32_t, int32_t, const std::vector<char>&)> terminateFn)
{
    std::vector<char> board(c_boardWidth * 7, '.'); // pre-allocate 7 rows (arbitrary)

    int32_t highestPoint{};
    int32_t moveNumber{};
    int32_t blockCount{};

    std::vector<std::function<Piece(int32_t, int32_t)>> spawners =
        { SpawnBar, SpawnPlus, SpawnL, SpawnI, SpawnBox };

    while(true)
    {
        // Ensure at least 7 rows of buffer above the highest piece allocated
        const auto newHighest = ((highestPoint + 7) * c_boardWidth);
        while (newHighest > board.size())
        {
            for (auto i = 0; i < c_boardWidth; ++i)
            {
                board.emplace_back('.');
            }
        }

        // Spawn piece
        auto curr = spawners[blockCount % spawners.size()](2, highestPoint + 3);

        // Move piece until it lands
        while (true)
        {
            auto currentMove = moves[moveNumber++ % moves.size()];
            // PrintBottom(board, curr, 10);
            if (currentMove)
            {
                TryMoveLeft(board, curr);
            }
            else
            {
                TryMoveRight(board, curr);
            }

            // PrintBottom(board, curr, 10);
            bool landed = TryMoveDown(board, curr);
            if (landed) break;
        }

        // Commit piece
        for (auto box : curr)
        {
            board[box] = '#';
            const auto y = (box / c_boardWidth) + 1;
            highestPoint = std::max(highestPoint, y);
        }
        // PrintBottom(board, curr, 10);

        ++blockCount;
        if (terminateFn(blockCount, highestPoint, (moveNumber > moves.size()), (moveNumber - 1) % moves.size(), (blockCount - 1) % spawners.size(), board))
        {
            return highestPoint;
        }
    }
    return -1;
}

void Part1()
{
    std::string input;
    std::getline(std::cin, input);
    const auto moves = ParseInput(input); // true == left
    const auto highestPoint = RunSimulation(moves, [](int32_t blockCount, int32_t highestPoint, bool, int32_t, int32_t, const std::vector<char>&) { return blockCount == 2022;});
    std::cout << highestPoint << std::endl;
}

struct BoardState
{
    std::vector<char> topRows;
    int32_t windIndex;
    int32_t pieceIndex;

    static constexpr int32_t RowsToInclude{100};
};

bool operator==(const BoardState& left, const BoardState& right)
{
    if (left.topRows.size() != right.topRows.size()) return false;
    for (auto i = 0; i < left.topRows.size(); ++i)
    {
        if (left.topRows[i] != right.topRows[i]) return false;
    }
    // return std::equal(left.topRows.begin(), left.topRows.end(), right.topRows.begin(), right.topRows.end()) &&
    return
        left.windIndex == right.windIndex &&
        left.pieceIndex == right.pieceIndex;
    // return (left.windIndex * left.pieceIndex) == (right.windIndex * right.pieceIndex);
}

struct BoardStateHash
{
    size_t operator()(const BoardState& b) const noexcept
    {
        size_t result{};
        for (auto i = 0; i < b.topRows.size(); ++i)
        {
            result ^= std::hash<char>{}(b.topRows[i]) << i;
        }
        result ^= std::hash<int32_t>{}(b.windIndex) << b.topRows.size();
        result ^= std::hash<int32_t>{}(b.pieceIndex) << (b.topRows.size() + 1);
        return result;
    }
};

// 1540804597680 too low
// 1540545886050 too low
void Part2()
{
    std::string input;
    std::getline(std::cin, input);
    const auto moves = ParseInput(input); // true == left

    std::unordered_map<BoardState, int32_t, BoardStateHash> boardStates;
    std::unordered_map<int32_t, int32_t> simulationResults;
    int32_t blockCountEndOfCycle{};
    int32_t blockCountBeginningOfCycle{};
    const auto highestPoint = RunSimulation(moves, [&](int32_t blockCount, int32_t highestPoint, bool allJetsUsed, int32_t windIndex, int32_t pieceIndex, const std::vector<char>& board)
    {
        blockCountEndOfCycle = blockCount;
        if (blockCount % 10000 == 0) { std::cerr << "Iteration count: " << blockCount << std::endl; }
        simulationResults[blockCount] = highestPoint;

        // Don't start looking for cycles until at least one round of moves is complete.  Eases bounds checking.
        if (allJetsUsed && (highestPoint > BoardState::RowsToInclude + 1))
        {
            std::vector<char> boardTop;
            // auto boardIter = board.begin() + ((highestPoint - BoardState::RowsToInclude) * c_boardWidth);
            // std::copy(boardIter, boardIter + (BoardState::RowsToInclude * c_boardWidth), std::back_inserter(boardTop));
            std::copy(board.rbegin(), board.rbegin() + (BoardState::RowsToInclude * c_boardWidth), std::back_inserter(boardTop));
            BoardState currentState{std::move(boardTop), windIndex, pieceIndex};

            if (boardStates.find(currentState) != boardStates.end())
            {
                blockCountBeginningOfCycle = boardStates[currentState];
                return true;
            }
            boardStates.emplace(std::move(currentState), blockCount);
        }
        return false;
    });

    const uint64_t c_targetBlockCount{1000000000000 - (uint64_t)blockCountBeginningOfCycle};
    const uint64_t blocksPerCycle = blockCountEndOfCycle - blockCountBeginningOfCycle;
    const uint64_t cyclesNeeded = c_targetBlockCount / blocksPerCycle;
    const uint64_t remainder = c_targetBlockCount % blocksPerCycle;
    const uint64_t gainPerCycle = simulationResults[blockCountEndOfCycle] - simulationResults[blockCountBeginningOfCycle];
    const uint64_t heightBeforeCycle = simulationResults[blockCountBeginningOfCycle - 1];
    const uint64_t heightInRemainder = simulationResults[blockCountBeginningOfCycle + (int32_t)remainder - 1] - simulationResults[blockCountBeginningOfCycle - 1];

    std::cerr << "The board cycled between these block counts: " << blockCountBeginningOfCycle << " " << blockCountEndOfCycle << std::endl;
    std::cerr << "The cycle took " << blocksPerCycle << " blocks and increased the height by " << gainPerCycle << std::endl;
    std::cerr << "Before the cycle began the height was " << heightBeforeCycle << std::endl;
    std::cerr << "After the cycle there were " << remainder << " more blocks to make up the difference and those added height " << heightInRemainder << std::endl;

    uint64_t result = heightBeforeCycle;
    result += (gainPerCycle * cyclesNeeded);
    result += heightInRemainder;

    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}