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

namespace
{
    struct Tile
    {
        Tile() = default;
        Tile(const std::vector<std::string>& input)
        {
            id = (uint32_t)std::atoi(input[0].substr(input[0].find(" ") + 1).c_str());

            top = HorizontalToInt(input[1]);
            topFlip = FlipBits(top);

            bottom = HorizontalToInt(input[input.size() - 1]);
            bottomFlip = FlipBits(bottom);

            uint32_t leftRaw{};
            uint32_t rightRaw{};
            const uint32_t tileHeight = input.size() - 1; // title
            for (auto i = 0UL; i < tileHeight; ++i)
            {
                const auto& curr = input[i + 1];
                if (curr[0] == '#')
                {
                    leftRaw += (1 << (tileHeight - i - 1));
                }

                if (curr[curr.size() - 1] == '#')
                {
                    rightRaw += (1 << (tileHeight - i - 1));
                }
            }
            left = leftRaw;
            leftFlip = FlipBits(leftRaw);
            right = rightRaw;
            rightFlip = FlipBits(rightRaw);

            std::copy(input.begin() + 1, input.end(), std::back_inserter(inputStrings));
            ValidateSelf();
        }

        void Rotate()
        {
            uint32_t temp1 = top;
            uint32_t temp2 = topFlip;

            top = left;
            topFlip = leftFlip;
            std::swap(top, topFlip);

            left = bottom;
            leftFlip = bottomFlip;

            bottom = right;
            bottomFlip = rightFlip;
            std::swap(bottom, bottomFlip);

            right = temp1;
            rightFlip = temp2;

            std::vector<std::string> inputStringsRotated;
            for (auto col = 0UL; col < inputStrings[0].size(); ++col)
            {
                std::string flipped;
                for (auto row = inputStrings[0].size(); row > 0; --row)
                {
                    flipped.push_back(inputStrings[row-1][col]);
                }
                inputStringsRotated.push_back(std::move(flipped));
            }
            std::swap(inputStrings, inputStringsRotated);

            ValidateSelf();
        }

        void FlipVertically()
        {
            std::swap(top, bottom);
            std::swap(topFlip, bottomFlip);

            std::swap(left, leftFlip);
            std::swap(right, rightFlip);

            std::vector<std::string> inputStringsFlipped;
            for (auto i = 0UL; i < inputStrings.size(); ++i)
            {
                inputStringsFlipped.push_back(inputStrings[inputStrings.size() - i - 1]);
            }
            std::swap(inputStrings, inputStringsFlipped);

            ValidateSelf();
        }

        bool TryAllCombinationsToMatch(uint32_t prevRight, uint32_t prevBottom)
        {
            for (auto rot = 0UL; rot < 4; ++rot)
            {
                if (((prevRight == 0xFFFFFFFF) || (left == prevRight)) && ((prevBottom == 0xFFFFFFFF) || (top == prevBottom))) return true;
                Rotate();
            }

            FlipVertically();
            for (auto rot = 0UL; rot < 4; ++rot)
            {
                if (((prevRight == 0xFFFFFFFF) || (left == prevRight)) && ((prevBottom == 0xFFFFFFFF) || (top == prevBottom))) return true;
                Rotate();
            }

            Rotate();
            FlipVertically();
            for (auto rot = 0UL; rot < 4; ++rot)
            {
                if (((prevRight == 0xFFFFFFFF) || (left == prevRight)) && ((prevBottom == 0xFFFFFFFF) || (top == prevBottom))) return true;
                Rotate();
            }
            return false;
        }

        void ValidateSelf() const
        {
            // Check bit flips for correctness
            if ((bool)(top & 0x200) != (bool)(topFlip & 0x1)) throw;
            if ((bool)(left & 0x200) != (bool)(leftFlip & 0x1)) throw;
            if ((bool)(right & 0x200) != (bool)(rightFlip & 0x1)) throw;
            if ((bool)(bottom & 0x200) != (bool)(bottomFlip & 0x1)) throw;

            if ((bool)(top & 0x1) != (bool)(rightFlip & 0x1)) throw;
            if ((bool)(topFlip & 0x1) != (bool)(leftFlip & 0x1)) throw;

            if ((bool)(bottom & 0x1) != (bool)(right & 0x1)) throw;
            if ((bool)(bottomFlip & 0x1) != (bool)(left & 0x1)) throw;
        }

        uint32_t id{};
        uint32_t top{};
        uint32_t topFlip{};
        uint32_t left{};
        uint32_t leftFlip{};
        uint32_t right{};
        uint32_t rightFlip{};
        uint32_t bottom{};
        uint32_t bottomFlip{};
        std::vector<std::string> inputStrings;

    private:
        uint32_t HorizontalToInt(const std::string& input)
        {
            uint32_t result{};
            for (auto i = 0UL; i < input.length(); ++i)
            {
                if (input[i] == '#')
                {
                    result += (1 << (input.length() - i - 1));
                }
            }
            return result;
        }

        uint32_t FlipBits(uint32_t input)
        {
            uint32_t result{};
            for (auto i = 0UL; i < 10; ++i)
            {
                if ((input & 0x1) != 0)
                {
                    result += 1 << (9 - i);
                }
                input = input >> 1;
            }
            return result;
        }
    };

    void Part1()
    {
        std::vector<Tile> allTiles;

        while (!cin.eof())
        {
            std::vector<std::string> tile;
            std::string input;
            while (std::getline(std::cin, input))
            {
                if (input.empty()) break;
                tile.emplace_back(input);
            }
            allTiles.emplace_back(Tile(tile));
        }

        std::vector<uint32_t> sides;
        std::for_each(allTiles.begin(), allTiles.end(), [&sides](auto& tile)
        {
            sides.emplace_back(tile.top);
            sides.emplace_back(tile.topFlip);
            sides.emplace_back(tile.left);
            sides.emplace_back(tile.leftFlip);
            sides.emplace_back(tile.right);
            sides.emplace_back(tile.rightFlip);
            sides.emplace_back(tile.bottom);
            sides.emplace_back(tile.bottomFlip);
        });
        std::sort(sides.begin(), sides.end());

        std::unordered_map<uint32_t, uint32_t> sidesMap; // index = number, value = numinstances
        std::for_each(sides.begin(), sides.end(), [&sidesMap](auto&& side)
        {
            // std::cerr << side << std::endl;
            sidesMap[side]++;
        });

        uint64_t result{1};
        uint32_t cornerCount{0};
        for (auto tile : allTiles)
        {
            const auto uniqueTop = sidesMap[tile.top] == 1;
            const auto uniqueLeft = sidesMap[tile.left] == 1;
            const auto uniqueRight = sidesMap[tile.right] == 1;
            const auto uniqueBottom = sidesMap[tile.bottom] == 1;

            if ((uniqueTop && uniqueLeft) || (uniqueTop && uniqueRight) || (uniqueBottom && uniqueLeft) || (uniqueBottom && uniqueRight))
            {
                result *= tile.id;
                cornerCount++;
            }
        }

        if (cornerCount != 4) throw 0;

        std::cout << result << std::endl;
    }

    void RotateFinalImage(std::vector<char>& finalImage, uint32_t finalBoardWidth)
    {
        std::vector<char> finalImageRotated(finalImage.size());
        auto iter = finalImageRotated.begin();
        for (auto col = 0UL; col < finalBoardWidth; ++col)
        {
            for (auto row = finalBoardWidth; row > 0; --row)
            {
                const auto index = ((row - 1) * finalBoardWidth) + col;
                *iter++ = finalImage[index];
            }
        }
        std::swap(finalImage, finalImageRotated);
    }

    void FlipFinalImageVertically(std::vector<char>& finalImage, uint32_t finalBoardWidth)
    {
        std::vector<char> finalImageFlipped(finalImage.size());
        auto iter = finalImage.begin();
        for (auto row = finalBoardWidth; row > 0; --row)
        {
            for (auto col = 0UL; col < finalBoardWidth; ++col)
            {
                const auto index = ((row - 1) * finalBoardWidth) + col;
                finalImageFlipped[index] = *iter;
                ++iter;
            }
        }
        std::swap(finalImage, finalImageFlipped);
    }

    uint32_t FindSeaMonsters(std::vector<char>& finalImage, uint32_t finalBoardWidth)
    {
        constexpr uint32_t monsterWidth{20};
        // First sea monster row
        std::unordered_set<uint32_t> monsterPattern = {18};
        // Second monster row
        monsterPattern.emplace(0 + finalBoardWidth);
        monsterPattern.emplace(5 + finalBoardWidth);
        monsterPattern.emplace(6 + finalBoardWidth);
        monsterPattern.emplace(11 + finalBoardWidth);
        monsterPattern.emplace(12 + finalBoardWidth);
        monsterPattern.emplace(17 + finalBoardWidth);
        monsterPattern.emplace(18 + finalBoardWidth);
        monsterPattern.emplace(19 + finalBoardWidth);
        // Third monster row
        monsterPattern.emplace(1 + (2 * finalBoardWidth));
        monsterPattern.emplace(4 + (2 * finalBoardWidth));
        monsterPattern.emplace(7 + (2 * finalBoardWidth));
        monsterPattern.emplace(10 + (2 * finalBoardWidth));
        monsterPattern.emplace(13 + (2 * finalBoardWidth));
        monsterPattern.emplace(16 + (2 * finalBoardWidth));

        const auto PatternMatch = [monsterPattern](std::vector<char>& finalImage)
        {
            uint32_t seaMonsterCount{};
            for (auto i = 0UL; i < finalImage.size(); ++i)
            {
                if (std::all_of(monsterPattern.begin(), monsterPattern.end(), [finalImage, i](uint32_t offset)
                {
                    return finalImage[i + offset] == '#';
                }))
                {
                    seaMonsterCount++;
                    std::for_each(monsterPattern.begin(), monsterPattern.end(), [&finalImage, i](uint32_t offset)
                    {
                        finalImage[i + offset] = 'O';
                    });
                }
            }
            return seaMonsterCount;
        };

        auto match = PatternMatch(finalImage);
        if (match != 0) return match;

        RotateFinalImage(finalImage, finalBoardWidth);
        match = PatternMatch(finalImage);
        if (match != 0) return match;

        RotateFinalImage(finalImage, finalBoardWidth);
        match = PatternMatch(finalImage);
        if (match != 0) return match;

        RotateFinalImage(finalImage, finalBoardWidth);
        match = PatternMatch(finalImage);
        if (match != 0) return match;

        // Flip on first axis
        RotateFinalImage(finalImage, finalBoardWidth);
        FlipFinalImageVertically(finalImage, finalBoardWidth);
        match = PatternMatch(finalImage);
        if (match != 0) return match;
        
        RotateFinalImage(finalImage, finalBoardWidth);
        match = PatternMatch(finalImage);
        if (match != 0) return match;

        RotateFinalImage(finalImage, finalBoardWidth);
        match = PatternMatch(finalImage);
        if (match != 0) return match;

        RotateFinalImage(finalImage, finalBoardWidth);
        match = PatternMatch(finalImage);
        if (match != 0) return match;

        // Flip on second axis
        // RotateFinalImage(finalImage, finalBoardWidth);
        FlipFinalImageVertically(finalImage, finalBoardWidth);
        match = PatternMatch(finalImage);
        if (match != 0) return match;
        
        RotateFinalImage(finalImage, finalBoardWidth);
        match = PatternMatch(finalImage);
        if (match != 0) return match;

        RotateFinalImage(finalImage, finalBoardWidth);
        match = PatternMatch(finalImage);
        if (match != 0) return match;

        RotateFinalImage(finalImage, finalBoardWidth);
        match = PatternMatch(finalImage);
        if (match != 0) return match;

        std::cerr << "Could not find sea monsters in image" << std::endl;
        throw;
    }

    void Part2()
    {
        std::vector<Tile> allTiles;
        while (!cin.eof())
        {
            std::vector<std::string> tile;
            std::string input;
            while (std::getline(std::cin, input))
            {
                if (input.empty()) break;
                tile.emplace_back(input);
            }
            allTiles.emplace_back(Tile(tile));
        }

        std::vector<uint32_t> sides;
        std::for_each(allTiles.begin(), allTiles.end(), [&sides](auto& tile)
        {
            sides.emplace_back(tile.top);
            sides.emplace_back(tile.topFlip);
            sides.emplace_back(tile.left);
            sides.emplace_back(tile.leftFlip);
            sides.emplace_back(tile.right);
            sides.emplace_back(tile.rightFlip);
            sides.emplace_back(tile.bottom);
            sides.emplace_back(tile.bottomFlip);
        });
        std::sort(sides.begin(), sides.end());

        std::map<uint32_t, uint32_t> sidesMap; // index = number, value = numinstances
        std::for_each(sides.begin(), sides.end(), [&sidesMap](auto&& side)
        {
            // std::cerr << side << std::endl;
            sidesMap[side]++;
        });

        const auto maxSideCount = std::max_element(sidesMap.begin(), sidesMap.end())->second;
        if (maxSideCount > 2) throw 0;

        const auto boardWidth = (uint32_t)sqrt(allTiles.size());
        Tile corner{};
        for (auto tile : allTiles)
        {
            const auto uniqueTop = sidesMap[tile.top] == 1;
            const auto uniqueLeft = sidesMap[tile.left] == 1;
            const auto uniqueRight = sidesMap[tile.right] == 1;
            const auto uniqueBottom = sidesMap[tile.bottom] == 1;

            if ((uniqueTop && uniqueLeft) || (uniqueTop && uniqueRight) || (uniqueBottom && uniqueLeft) || (uniqueBottom && uniqueRight))
            {
                corner = tile;
                break;
            }
        }
        allTiles.erase(std::find_if(allTiles.begin(), allTiles.end(), [corner](const Tile& tile) { return tile.id == corner.id; }));

        while ((sidesMap[corner.right] != 2) || (sidesMap[corner.bottom] != 2))
        {
            corner.Rotate();
        }

        std::vector<std::vector<Tile>> board;
        for (auto i = 0UL; i < boardWidth; ++i)
        {
            std::vector<Tile> temp(boardWidth);
            board.push_back(temp);
        }

        board[0][0] = corner;
        for (auto y = 0UL; y < boardWidth; ++y)
        {
            for (auto x = 0UL; x < boardWidth; ++x)
            {
                if (x == 0 && y == 0) continue;

                const auto prevBottom = (y > 0) ? board[y-1][x].bottom : 0xFFFFFFFF;
                const auto prevRight = (x > 0) ? board[y][x-1].right : 0xFFFFFFFF;
                auto match = std::find_if(allTiles.begin(), allTiles.end(), [prevRight, prevBottom](Tile& tile)
                {
                    return tile.TryAllCombinationsToMatch(prevRight, prevBottom);
                });

                if (match == allTiles.end())
                {
                    throw 0; // no match found
                }

                board[y][x] = *match;
                allTiles.erase(match);
            }
        }

        std::vector<char> finalImage;
        for (auto y = 0UL; y < boardWidth; ++y)
        {
            for (auto subRow = 1UL; subRow < 9; ++subRow) // skip top and bottom rows
            {
                for (auto x = 0UL; x < boardWidth; ++x)
                {
                    const Tile& curr = board[y][x];
                    const std::string& row = curr.inputStrings[subRow];
                    for (auto subCol = 1UL; subCol < 9; ++subCol) // skip left and right cols
                    {
                        finalImage.push_back(row[subCol]);
                    }
                }
            }
        }

        const uint32_t finalBoardWidth { 8 * boardWidth };
        const auto PrintFinalBoard = [finalBoardWidth](const std::vector<char>& finalImage)
        {
            for (auto i = 0UL; i < finalImage.size(); ++i)
            {
                std::cerr << finalImage[i];
                if ((i+1) % finalBoardWidth == 0)
                {
                    std::cerr << std::endl;
                }
            }
            std::cerr << std::endl;
        };
        PrintFinalBoard(finalImage);

        const auto seaMonsterCount = FindSeaMonsters(finalImage, finalBoardWidth);
        std::cerr << "Found " << seaMonsterCount << " sea monsters" << std::endl;
        PrintFinalBoard(finalImage);

        uint32_t result = std::accumulate(finalImage.begin(), finalImage.end(), 0UL, [](uint32_t sum, char letter)
        {
            return sum + (letter == '#' ? 1 : 0);
        });
        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}