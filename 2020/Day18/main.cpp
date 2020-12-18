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
    enum class Token
    {
        Number,
        Plus,
        Times,
        Open,
        Close
    };

    std::string TokenToString(Token t)
    {
        switch (t)
        {
            case Token::Number: return "";
            case Token::Plus: return " + ";
            case Token::Times: return " * ";
            case Token::Open: return "(";
            case Token::Close: return ")";
        }
        return "";
    }

    struct Operator
    {
        Token token{};
        uint64_t value{};
    };

    uint64_t Eval(std::vector<Operator> ops, uint64_t& currentOp)
    {
        if (currentOp == ops.size()) return 0;
        if ((ops[currentOp].token != Token::Number) && (ops[currentOp].token != Token::Open)) throw 0; // should always begin with a number

        uint64_t result{};
        if (ops[currentOp].token == Token::Number)
        {
            result = ops[currentOp].value;
            currentOp++;
        }

        while (currentOp < ops.size())
        {
            auto op = ops[currentOp];
            if (op.token == Token::Plus)
            {
                currentOp++;
                op = ops[currentOp];
                if (op.token == Token::Number)
                {
                    result += ops[currentOp].value;
                    currentOp++;
                }
                else if (op.token == Token::Open)
                {
                    result += Eval(ops, ++currentOp);
                }
            }
            else if (op.token == Token::Times)
            {
                currentOp++;
                op = ops[currentOp];
                if (op.token == Token::Number)
                {
                    result *= ops[currentOp].value;
                    currentOp++;
                }
                else if (op.token == Token::Open)
                {
                    result *= Eval(ops, ++currentOp);
                }
            }
            else if (op.token == Token::Open)
            {
                result = Eval(ops, ++currentOp);
            }
            else if (op.token == Token::Close)
            {
                currentOp++;
                return result;
            }
        }
        return result;
    }

    uint64_t ProcessEquation(std::string input)
    {
        std::vector<Operator> ops{};
        {
            std::istringstream instream(input);
            std::string substr;
            while (std::getline(instream, substr, ' '))
            {
                auto c = substr[0];
                if (c == '(')
                {
                    auto i = 0UL;
                    do
                    {
                        ops.emplace_back(Operator{Token::Open, 0});
                        i++;
                    } while (substr[i] == '(');
                    
                    ops.emplace_back(Operator{Token::Number, (uint64_t)std::atoi(substr.substr(i).c_str())});
                }
                else if (substr[substr.length() - 1] == ')')
                {
                    ops.emplace_back(Operator{Token::Number, (uint64_t)std::atoi(substr.c_str())});
                    auto i = substr.length() - 1;
                    do
                    {
                        ops.emplace_back(Operator{Token::Close});
                        i--;
                    } while (substr[i] == ')');
                    
                }
                else if (c == '+') ops.emplace_back(Operator{Token::Plus});
                else if (c == '*') ops.emplace_back(Operator{Token::Times});
                else
                {
                    ops.emplace_back(Operator{Token::Number, (uint64_t)std::atoi(substr.c_str())});
                }
            }
        }

        std::cerr << "  ";
        for (auto op : ops)
        {
            if (op.token == Token::Number)
            {
                std::cerr << op.value;
            }
            else
            {
                std::cerr << TokenToString(op.token);
            }
        }

        uint64_t currentOp{0UL};
        const uint64_t result = Eval(ops, currentOp);
        return result;
    }

    void Part1()
    {
        uint64_t total{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            cerr << "  " << input << endl;
            const auto result = ProcessEquation(input);
            std::cerr << "    " << result << endl;
            total += result;
        }
        
        std::cout << total << std::endl;
    }

    uint64_t EvalNoParens(std::vector<Operator> ops)
    {
        if (ops[0].token != Token::Number) throw 0; // should always begin with a number

        for (auto iter = ops.begin(); iter != ops.end(); ++iter)
        {
            if (iter->token == Token::Plus)
            {
                --iter; // back up so we're on the number again
                const auto val = iter->value + (iter + 2)->value;
                ops.erase(iter);
                ops.erase(iter);
                ops.erase(iter);
                ops.insert(iter, Operator{Token::Number, val});
            }
        }

        uint64_t result{ops[0].value};
        for (auto i = 1UL; i < ops.size(); ++i)
        {
            if (ops[i].token == Token::Number)
            {
                result *= ops[i].value;
            }
        }
        
        return result;
    }

    uint64_t EvalPart2(std::vector<Operator> ops, uint64_t& currentOp)
    {
        if (currentOp == ops.size()) return 0;
        if ((ops[currentOp].token != Token::Number) && (ops[currentOp].token != Token::Open)) throw 0; // should always begin with a number

        std::vector<Operator> opsNoParens;

        if (ops[currentOp].token == Token::Number)
        {
            opsNoParens.emplace_back(ops[currentOp]);
            currentOp++;
        }

        while (currentOp < ops.size())
        {
            auto op = ops[currentOp];
            if (op.token == Token::Plus)
            {
                opsNoParens.emplace_back(op);

                currentOp++;
                op = ops[currentOp];
                if (op.token == Token::Number)
                {
                    opsNoParens.emplace_back(ops[currentOp]);
                    currentOp++;
                }
                else if (op.token == Token::Open)
                {
                    const uint64_t eval = EvalPart2(ops, ++currentOp);
                    opsNoParens.emplace_back(Operator{Token::Number, eval});
                }
            }
            else if (op.token == Token::Times)
            {
                opsNoParens.emplace_back(op);

                currentOp++;
                op = ops[currentOp];
                if (op.token == Token::Number)
                {
                    opsNoParens.emplace_back(ops[currentOp]);
                    currentOp++;
                }
                else if (op.token == Token::Open)
                {
                    const uint64_t eval = EvalPart2(ops, ++currentOp);
                    opsNoParens.emplace_back(Operator{Token::Number, eval});
                }
            }
            else if (op.token == Token::Open)
            {
                const uint64_t eval = EvalPart2(ops, ++currentOp);
                opsNoParens.emplace_back(Operator{Token::Number, eval});
            }
            else if (op.token == Token::Close)
            {
                currentOp++;
                return EvalNoParens(opsNoParens);
            }
        }

        return EvalNoParens(opsNoParens);
    }

    uint64_t ProcessEquationPart2(std::string input)
    {
        std::vector<Operator> ops{};
        {
            std::istringstream instream(input);
            std::string substr;
            while (std::getline(instream, substr, ' '))
            {
                auto c = substr[0];
                if (c == '(')
                {
                    auto i = 0UL;
                    do
                    {
                        ops.emplace_back(Operator{Token::Open, 0});
                        i++;
                    } while (substr[i] == '(');
                    
                    ops.emplace_back(Operator{Token::Number, (uint64_t)std::atoi(substr.substr(i).c_str())});
                }
                else if (substr[substr.length() - 1] == ')')
                {
                    ops.emplace_back(Operator{Token::Number, (uint64_t)std::atoi(substr.c_str())});
                    auto i = substr.length() - 1;
                    do
                    {
                        ops.emplace_back(Operator{Token::Close});
                        i--;
                    } while (substr[i] == ')');
                    
                }
                else if (c == '+') ops.emplace_back(Operator{Token::Plus});
                else if (c == '*') ops.emplace_back(Operator{Token::Times});
                else
                {
                    ops.emplace_back(Operator{Token::Number, (uint64_t)std::atoi(substr.c_str())});
                }
            }
        }

        std::cerr << "  ";
        for (auto op : ops)
        {
            if (op.token == Token::Number)
            {
                std::cerr << op.value;
            }
            else
            {
                std::cerr << TokenToString(op.token);
            }
        }

        uint64_t currentOp{0UL};
        const uint64_t result = EvalPart2(ops, currentOp);
        return result;
    }

    void Part2()
    {
        uint64_t total{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            cerr << "  " << input << endl;
            const auto result = ProcessEquationPart2(input);
            std::cerr << "    " << result << endl;
            total += result;
        }
        
        std::cout << total << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}