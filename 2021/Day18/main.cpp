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
#include <variant>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

struct Number
{
    std::variant<uint32_t, std::unique_ptr<Number>> left;
    std::variant<uint32_t, std::unique_ptr<Number>> right;
    uint32_t depth;
    
    uint64_t ComputeMagnitude() const
    {
        uint64_t leftResult{};
        if (std::holds_alternative<uint32_t>(left))
        {
            leftResult = std::get<uint32_t>(left);
        }
        else
        {
            leftResult = std::get<std::unique_ptr<Number>>(left)->ComputeMagnitude();
        }
        leftResult *= 3;

        uint64_t rightResult{};
        if (std::holds_alternative<uint32_t>(right))
        {
            rightResult = std::get<uint32_t>(right);
        }
        else
        {
            rightResult = std::get<std::unique_ptr<Number>>(right)->ComputeMagnitude();
        }
        rightResult *= 2;

        return (leftResult + rightResult);
    }

    bool Explode(const std::vector<uint32_t*>& orderedNumbers) // returns true if explode took place
    {
        if (std::holds_alternative<std::unique_ptr<Number>>(left))
        {
            auto& leftPtr = std::get<std::unique_ptr<Number>>(left);
            if (leftPtr->depth >= 4)
            {
                auto leftNumIter = std::find(orderedNumbers.begin(), orderedNumbers.end(), reinterpret_cast<uint32_t*>(&(leftPtr->left)));
                if (leftNumIter == orderedNumbers.end()) throw std::exception();
                const auto leftNum = std::get<uint32_t>(leftPtr->left);
                if (std::distance(orderedNumbers.begin(), (leftNumIter - 1)) >= 0)
                {
                    *(*(leftNumIter - 1)) += leftNum;
                }
                
                auto rightNumIter = std::find(orderedNumbers.begin(), orderedNumbers.end(), reinterpret_cast<uint32_t*>(&(leftPtr->right)));
                if (rightNumIter == orderedNumbers.end()) throw std::exception();
                const auto rightNum = std::get<uint32_t>(leftPtr->right);
                if ((rightNumIter + 1) != orderedNumbers.end())
                {
                    *(*(rightNumIter + 1)) += rightNum;
                }

                left = 0UL;
                return true;
            }
            else
            {
                auto exploded = leftPtr->Explode(orderedNumbers);
                if (exploded) return true;
            }
        }
        
        if (std::holds_alternative<std::unique_ptr<Number>>(right))
        {   
            auto& rightPtr = std::get<std::unique_ptr<Number>>(right);
            if (rightPtr->depth >= 4)
            {
                auto leftNumIter = std::find(orderedNumbers.begin(), orderedNumbers.end(), reinterpret_cast<uint32_t*>(&(rightPtr->left)));
                if (leftNumIter == orderedNumbers.end()) throw std::exception();
                const auto leftNum = std::get<uint32_t>(rightPtr->left);
                if (std::distance(orderedNumbers.begin(), (leftNumIter - 1)) >= 0)
                {
                    *(*(leftNumIter - 1)) += leftNum;
                }
                
                auto rightNumIter = std::find(orderedNumbers.begin(), orderedNumbers.end(), reinterpret_cast<uint32_t*>(&(rightPtr->right)));
                if (rightNumIter == orderedNumbers.end()) throw std::exception();
                const auto rightNum = std::get<uint32_t>(rightPtr->right);
                if ((rightNumIter + 1) != orderedNumbers.end())
                {
                    *(*(rightNumIter + 1)) += rightNum;
                }

                right = 0UL;
                return true;
            }
            else
            {
                auto exploded = rightPtr->Explode(orderedNumbers);
                if (exploded) return true;
            }
        }

        return false;
    }

    bool Split()
    {
        if (std::holds_alternative<std::unique_ptr<Number>>(left))
        {
            auto& leftPtr = std::get<std::unique_ptr<Number>>(left);
            auto split = leftPtr->Split();
            if (split) return true;
        }
        else
        {
            auto leftNum = std::get<uint32_t>(left);
            if (leftNum >= 10)
            {
                auto newPair = std::make_unique<Number>();
                newPair->left = leftNum / 2;
                newPair->right = (leftNum + 1) / 2;
                newPair->depth = (depth + 1);
                left = std::move(newPair);
                return true;
            }
        }

        if (std::holds_alternative<std::unique_ptr<Number>>(right))
        {
            auto& rightPtr = std::get<std::unique_ptr<Number>>(right);
            auto split = rightPtr->Split();
            if (split) return true;
        }
        else
        {
            auto rightNum = std::get<uint32_t>(right);
            if (rightNum >= 10)
            {
                auto newPair = std::make_unique<Number>();
                newPair->left = rightNum / 2;
                newPair->right = (rightNum + 1) / 2;
                newPair->depth = (depth + 1);
                right = std::move(newPair);
                return true;
            }
        }

        return false;
    }

    void GetOrderedNumberPointers(std::vector<uint32_t*>& result)
    {
        if (std::holds_alternative<uint32_t>(left))
        {
            result.emplace_back(&(std::get<uint32_t>(left)));
        }
        else
        {
            std::get<std::unique_ptr<Number>>(left)->GetOrderedNumberPointers(result);
        }
        
        if (std::holds_alternative<uint32_t>(right))
        {
            result.emplace_back(&(std::get<uint32_t>(right)));
        }
        else
        {
            std::get<std::unique_ptr<Number>>(right)->GetOrderedNumberPointers(result);
        }
    }

    void Reduce()
    {
        while (true)
        {
            std::vector<uint32_t*> orderedNumbers;
            GetOrderedNumberPointers(orderedNumbers);

            if (Explode(orderedNumbers))
            {
                continue;
            }

            if (Split()) // Root level element has a number bigger than 10
            {
                continue;
            }

            // No explodes or splits, break the loop
            break;
        }
    }

    std::string ToString() const
    {
        std::ostringstream outstream;
        outstream << "[";

        if (std::holds_alternative<uint32_t>(left))
        {
            outstream << (char)('0' + std::get<uint32_t>(left));
        }
        else
        {
            outstream << std::get<std::unique_ptr<Number>>(left)->ToString();
        }
        
        outstream << ",";

        if (std::holds_alternative<uint32_t>(right))
        {
            outstream << (char)('0' + std::get<uint32_t>(right));
        }
        else
        {
            outstream << std::get<std::unique_ptr<Number>>(right)->ToString();
        }

        outstream << "]";
        return outstream.str();
    }
};

namespace
{
    std::pair<std::unique_ptr<Number>, std::string_view> ParseNumber(std::string_view input, uint32_t depth)
    {
        if (input[0] != '[') throw std::exception();

        auto result = std::make_unique<Number>();
        result->depth = depth;
        if (input[1] == '[') // left is recursive
        {
            auto parseResult = ParseNumber(input.substr(1), depth + 1);
            input = parseResult.second;
            result->left = std::move(parseResult.first);
            if (input[0] != ']') throw std::exception();
            input = input.substr(1);
        }
        else
        {
            if (input[2] != ',') throw std::exception(); // only works with single-digit numbers
            result->left = (uint32_t)(input[1] - '0');
            input = input.substr(2); // skip [ and number
        }

        if (input[0] != ',') throw std::exception();  // sanity check

        if (input[1] == '[') // right is recursive
        {
            auto parseResult = ParseNumber(input.substr(1), depth + 1);
            input = parseResult.second;
            result->right = std::move(parseResult.first);
            if (input[0] != ']') throw std::exception();
            input = input.substr(1);
        }
        else
        {
            if (input[2] >= '0' && input[2] <= '9') throw std::exception(); // only works with single-digit numbers
            result->right = (uint32_t)(input[1] - '0');
            input = input.substr(2); // skip [ and number
        }

        return std::make_pair(std::move(result), input);
    }

    void TEST(uint64_t expected, uint64_t actual)
    {
        if (expected != actual) throw std::exception();
    }

    void TEST_COMPUTATION(std::string_view before, std::string_view after)
    {
        auto testParse = ParseNumber(before, 0);
        testParse.first->Reduce();
        const auto testParseFinal = testParse.first->ToString();

        if (testParseFinal != after) throw std::exception();
    }

    uint64_t ComputeMagnitude(const std::string& input)
    {
        const auto result = ParseNumber(input, 0);

        // Extra testing.  We should be able to reconstruct the original string from the parsed result.
        const auto toString = result.first->ToString();
        if (toString != input) throw std::exception();

        return result.first->ComputeMagnitude();
    }

    void Part1()
    {
        std::string current;
        std::getline(std::cin, current);

        std::string input;
        while (std::getline(std::cin, input))
        {
            std::cerr << "  " << current << std::endl << "+ " << input << std::endl;

            std::ostringstream outstream;
            outstream << "[" << current << "," << input << "]";
            const auto parseResult = ParseNumber(outstream.str(), 0);

            if (parseResult.second != "]"sv) throw std::exception(); // should just have the closing ] left over
            parseResult.first->Reduce();
            current = parseResult.first->ToString();
            std::cerr << "= " << current << "\r\n" << std::endl;
        }

        const auto finalNumber = ParseNumber(current, 0);
        std::cout << finalNumber.first->ComputeMagnitude() << std::endl;
    }

    void Part2()
    {
        std::vector<std::string> allInputs;
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                allInputs.emplace_back(std::move(input));
            }
        }

        std::vector<uint64_t> allSums;
        for (auto i = 0UL; i < allInputs.size(); ++i)
        {
            for (auto j = 0UL; j < allInputs.size(); ++j)
            {
                if (i != j)
                {
                    std::ostringstream outstream;
                    outstream << "[" << allInputs[i] << "," << allInputs[j] << "]";
                    const auto parseResult = ParseNumber(outstream.str(), 0);
                    if (parseResult.second != "]"sv) throw std::exception(); // should just have the closing ] left over
                    parseResult.first->Reduce();
                    allSums.emplace_back(parseResult.first->ComputeMagnitude());
                }
            }
        }

        std::cout << *(std::max_element(allSums.begin(), allSums.end())) << std::endl;
    }
}

int main()
{
    // Parse/magnitude tests
    TEST(143ULL, ComputeMagnitude("[[1,2],[[3,4],5]]"));
    TEST(1384ULL, ComputeMagnitude("[[[[0,7],4],[[7,8],[6,0]]],[8,1]]"));
    TEST(445ULL, ComputeMagnitude("[[[[1,1],[2,2]],[3,3]],[4,4]]"));
    TEST(791ULL, ComputeMagnitude("[[[[3,0],[5,3]],[4,4]],[5,5]]"));
    TEST(1137ULL, ComputeMagnitude("[[[[5,0],[7,4]],[5,5]],[6,6]]"));
    TEST(3488ULL, ComputeMagnitude("[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]"));

    // Explode tests
    TEST_COMPUTATION("[[[[[9,8],1],2],3],4]", "[[[[0,9],2],3],4]");
    TEST_COMPUTATION("[7,[6,[5,[4,[3,2]]]]]", "[7,[6,[5,[7,0]]]]");
    TEST_COMPUTATION("[[6,[5,[4,[3,2]]]],1]", "[[6,[5,[7,0]]],3]");
    TEST_COMPUTATION("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]", "[[3,[2,[8,0]]],[9,[5,[7,0]]]]");

    // Explode+Split tests
    TEST_COMPUTATION("[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]", "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");

    RunPart1() ? Part1() : Part2();
    return 0;
}