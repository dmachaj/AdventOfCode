#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <stack>
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
    uint64_t ScoreSingleLine(const std::string& input)
    {
        std::stack<char> stack;
        for (const char c : input)
        {
            if (c == '(' || c == '[' || c == '{' || c == '<')
            {
                if (c == '(') stack.push(')');
                else if (c == '[') stack.push(']');
                else if (c == '{') stack.push('}');
                else if (c == '<') stack.push('>');
                else throw std::exception();
            }
            else
            {
                const auto top = stack.top();
                if (c != top)
                {
                    if (c == ')') return 3;
                    else if (c == ']') return 57;
                    else if (c == '}') return 1197;
                    else if (c == '>') return 25137;
                    else throw std::exception();
                }
                else
                {
                    stack.pop();
                }
            }
        }
        return 0;
    }

    void Part1()
    {
        uint64_t result{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            const auto score = ScoreSingleLine(input);
            // std::cerr << score << std::endl;
            result += score;
        }
        std::cout << result << std::endl;
    }

    uint64_t CompleteMissingLine(const std::string& input)
    {
        std::stack<char> stack;
        for (const char c : input)
        {
            if (c == '(' || c == '[' || c == '{' || c == '<')
            {
                if (c == '(') stack.push(')');
                else if (c == '[') stack.push(']');
                else if (c == '{') stack.push('}');
                else if (c == '<') stack.push('>');
                else throw std::exception();
            }
            else
            {
                const auto top = stack.top();
                if (c != top)
                {
                    if (c == ')') return 3;
                    else if (c == ']') return 57;
                    else if (c == '}') return 1197;
                    else if (c == '>') return 25137;
                    else throw std::exception();
                }
                else
                {
                    stack.pop();
                }
            }
        }

        std::vector<char> completion;
        while (!stack.empty())
        {
            completion.emplace_back(stack.top());
            stack.pop();
        }

        uint64_t result{};
        for (auto i = 0; i < completion.size(); ++i)
        {
            result *= 5;
            const char c = completion[i];
            if (c == ')') result += 1;
            else if (c == ']') result += 2;
            else if (c == '}') result += 3;
            else if (c == '>') result += 4;
        }

        return result;
    }

    void Part2()
    {
        std::vector<uint64_t> results{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            const auto corrupt = ScoreSingleLine(input);
            if (corrupt == 0)
            {
                const auto score = CompleteMissingLine(input);
                results.emplace_back(score);
            }
        }

        std::sort(results.begin(), results.end());
        std::cout << results[results.size() / 2] << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}