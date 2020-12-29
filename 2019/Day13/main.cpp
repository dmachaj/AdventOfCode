#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <fstream>
#include "HelpfulInfrastructure.h"
#include "../Intcode/Intcode.h"

// #define WATCH_IT_PLAY

namespace
{
    struct Tile
    {
        uint32_t x{};
        uint32_t y{};
        uint8_t type{};
        char printCode{};
    };

    struct BoardInfo
    {
        std::vector<Tile> allTiles{};
        uint32_t width{};
        uint32_t height{};
        uint32_t score{};

        BoardInfo(const std::vector<int64_t>& input)
        {
            if (input.empty()) { throw; }

            allTiles = std::move(ProcessInput(input));

            const auto maxX = std::max_element(allTiles.begin(), allTiles.end(), 
                [](const Tile& first, const Tile& second)
                {
                    return first.x < second.x;
                });
            const auto maxY = std::max_element(allTiles.begin(), allTiles.end(), 
                [](const Tile& first, const Tile& second)
                {
                    return first.y < second.y;
                });
            
            // std::cerr << "NumTiles=" << (allTiles.size() / 3) << " MaxX=" << maxX->x << " MaxY=" << maxY->y << " NumBlocks=" << numBlocks << std::endl;
            width = maxX->x + 1;
            height = maxY->y + 1;
        }

        void PrintToStderr() const
        {
#ifdef WATCH_IT_PLAY
            // Sets the "cursor" in the terminal to 1,1 (top left corner) so that it is possible to watch the game being played.
            std::cerr << "\033[1;1H";
#endif //WATCH_IT_PLAY
            for (auto y = 0UL; y < height; ++y)
            {
                for (auto x = 0UL; x < width; ++x)
                {
                    const auto tile = std::find_if(allTiles.begin(), allTiles.end(), [x,y](const Tile& tile) { return tile.x == x && tile.y == y; });
                    if (tile != allTiles.end())
                    {
                        std::cerr << tile->printCode;
                    }
                }
                std::cerr << std::endl;
            }
        }

        void ProcessIncrementalUpdate(const std::vector<int64_t>& input)
        {
            std::vector<Tile> updates = ProcessInput(input);
            for (const auto& tile : updates)
            {
                const auto x = tile.x;
                const auto y = tile.y;
                auto tileIter = std::find_if(allTiles.begin(), allTiles.end(), [x,y](const Tile& tile) { return tile.x == x && tile.y == y; });
                tileIter->type = tile.type;
                tileIter->printCode = tile.printCode;
            }
        }

    private:
        std::vector<Tile> ProcessInput(const std::vector<int64_t>& input)
        {
            std::vector<Tile> results;
            for (auto i = 0UL; i < input.size(); i += 3)
            {
                if ((input[i] == -1) && (input[i+1] == 0))
                {
                    score = (uint32_t)input[i+2];
                    continue;
                }

                Tile thisTile{ (uint32_t)input[i], (uint32_t)input[i+1], (uint8_t)input[i+2] };
                switch (thisTile.type)
                {
                    case 0: // empty
                        thisTile.printCode =  ' ';
                        break;
                    case 1: // wall
                        thisTile.printCode =  '#';
                        break;
                    case 2: // block
                        thisTile.printCode =  'B';
                        break;
                    case 3: // paddle
                        thisTile.printCode =  '=';
                        break;
                    case 4: // ball
                        thisTile.printCode =  'o';
                        break;
                    default:
                        thisTile.printCode =  '?';
                        // throw std::exception("Unknown tile type");
                        break;
                }
                results.emplace_back(std::move(thisTile));
            }

            return results;
        }
    };

    void Part1()
    {
        auto state = Intcode::ParseProgram();
        Intcode::VectorIntcodeInput input;
        Intcode::VectorIntcodeOutput output;
        ExecuteProgram(state, &input, &output);

        const BoardInfo board(output.outputs);

        uint32_t numBlocks = std::accumulate(board.allTiles.begin(), board.allTiles.end(), 0UL, [](uint32_t sum, const Tile& tile)
        {
            return (tile.type == 2) ? (sum + 1) : sum;
        });
        
        board.PrintToStderr();
        std::cout << numBlocks << std::endl;
    }

    void Part2(std::istream& inputStream)
    {
        auto state = Intcode::ParseProgram(inputStream);
        state.program[0] = 2; // insert quarters

        std::vector<int64_t> commands{};
        int64_t finalScore{};
        std::unique_ptr<BoardInfo> board;
        while (true)
        {
            Intcode::VectorIntcodeInput input(commands);
            Intcode::VectorIntcodeOutput output;
            ExecuteProgram(state, &input, &output);
            if (!board)
            {
                board = std::make_unique<BoardInfo>(output.outputs);
            }
            else
            {
                board->ProcessIncrementalUpdate(output.outputs);
            }
#ifdef WATCH_IT_PLAY
            board->PrintToStderr();
#endif // WATCH_IT_PLAY

            Tile ball = *std::find_if(board->allTiles.begin(), board->allTiles.end(), [](const Tile& tile) { return tile.type == 4; });
            Tile paddle = *std::find_if(board->allTiles.begin(), board->allTiles.end(), [](const Tile& tile) { return tile.type == 3; });

            if (paddle.x < ball.x)
            {
                commands = { 1 };
            }
            else if (paddle.x > ball.x)
            {
                commands = { -1 };
            }
            else
            {
                commands = { 0 };
            }

#ifdef WATCH_IT_PLAY
            std::cerr << "Score: " << board->score << std::endl;
#endif // WATCH_IT_PLAY

            if (state.IsHalted())
            {
                finalScore = board->score;
                break;
            }
        }

        std::cout << finalScore << std::endl;
    }
}

int main()
{
    if (RunPart1())
    {
        Part1();
    }
    else
    {
        // The input is too big to copy/paste into a terminal window so if debugging it is necessary to use a file stream
        // instead of stdin so that the end of the input isn't truncated
        //
        // std::ifstream inputFileStream;
        // inputFileStream.open(LR"(c:\code\AdventOfCode\2019\Day13\Part2\unique.in)");
        // Part2(inputFileStream);
        Part2(std::cin);
    }
    return 0;
}