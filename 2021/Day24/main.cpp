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

enum class OpType
{
    unknown,
    inp,
    add,
    mul,
    div,
    mod,
    eql
};

struct Op
{
    OpType type;
    char target;
    char source;
    int32_t sourceConstant;
    bool sourceIsConstant;
};

std::vector<Op> ProgramFromInput(std::istream& instream)
{
    std::vector<Op> program;

    std::string input;
    while (std::getline(instream, input))
    {
        Op thisOp{};
        thisOp.target = input[4];

        // Share identical code via lambda
        const auto initSourceLambda = [input, &thisOp]()
        {
            if (input[6] >= 'w' && input[6] <= 'z')
            {
                thisOp.source = input[6];
            }
            else
            {
                const auto sourceStr = input.substr(6);
                thisOp.sourceConstant = std::atoi(sourceStr.c_str());
                thisOp.sourceIsConstant = true;
            }
        };

        switch (input[0])
        {
            case 'i':
            {
                thisOp.type = OpType::inp;
                break;
            }
            case 'a':
            {
                thisOp.type = OpType::add;
                initSourceLambda();
                break;
            }
            case 'd':
            {
                thisOp.type = OpType::div;
                initSourceLambda();
                break;
            }
            case 'e':
            {
                thisOp.type = OpType::eql;
                initSourceLambda();
                break;
            }
            case 'm':
            {
                if (input[1] == 'o')
                {
                    thisOp.type = OpType::mod;
                }
                else if (input[1] == 'u')
                {
                    thisOp.type = OpType::mul;
                }
                initSourceLambda();
                break;
            }
        }

        program.emplace_back(std::move(thisOp));
    }

    return program;
}

struct CPU
{
    int64_t w{};
    int64_t x{};
    int64_t y{};
    int64_t z{};
    std::vector<Op> program;
    std::vector<uint32_t> inputs;
    std::vector<int64_t> zHistory;

    CPU(const std::vector<Op>& program, const std::vector<uint32_t>& inputs):
        program(program),
        inputs(inputs)
    {
    }

    void Execute()
    {
        // if (programCounter >= program.size()) throw std::exception();

        for (const auto& op : program) // no branches supported so just run straight through
        {
            // if (z > 1000) return;

            auto targetPtr = GetRegisterPointer(op.target);
            switch (op.type)
            {
                case OpType::inp:
                    if (inputs.size() == 0) return;
                    // if (inputs.size() == 0) throw std::exception;

                    *targetPtr = *inputs.begin();
                    inputs.erase(inputs.begin());

                    zHistory.emplace_back(z);
                    // std::cerr << "Current z register: " << z << std::endl;

                    break;

                case OpType::add:
                    if (op.sourceIsConstant)
                    {
                        *targetPtr = *targetPtr + op.sourceConstant;
                    }
                    else
                    {
                        auto sourcePtr = GetRegisterPointer(op.source);
                        *targetPtr = *targetPtr + *sourcePtr;
                    }
                    break;

                case OpType::mul:
                    if (op.sourceIsConstant)
                    {
                        *targetPtr = *targetPtr * op.sourceConstant;
                    }
                    else
                    {
                        auto sourcePtr = GetRegisterPointer(op.source);
                        *targetPtr = *targetPtr * *sourcePtr;
                    }
                    break;
                    
                case OpType::div:
                    if (op.sourceIsConstant)
                    {
                        *targetPtr = *targetPtr / op.sourceConstant;
                    }
                    else
                    {
                        auto sourcePtr = GetRegisterPointer(op.source);
                        *targetPtr = *targetPtr / *sourcePtr;
                    }
                    break;

                case OpType::mod:
                    if (op.sourceIsConstant)
                    {
                        *targetPtr = *targetPtr % op.sourceConstant;
                    }
                    else
                    {
                        auto sourcePtr = GetRegisterPointer(op.source);
                        *targetPtr = *targetPtr % *sourcePtr;
                    }
                    break;

                case OpType::eql:
                    if (op.sourceIsConstant)
                    {
                        *targetPtr = (*targetPtr == op.sourceConstant) ? 1 : 0;
                    }
                    else
                    {
                        auto sourcePtr = GetRegisterPointer(op.source);
                        *targetPtr = (*targetPtr == *sourcePtr) ? 1 : 0;
                    }
                    break;

                case OpType::unknown:
                default:
                    throw std::exception();
            }
        }
    }

private:
    int64_t* GetRegisterPointer(char c)
    {
        switch (c)
        {
            case 'w': return &w;
            case 'x': return &x;
            case 'y': return &y;
            case 'z': return &z;
            default: throw std::exception();
        }
    }
};

namespace
{
    std::vector<uint32_t> BreakNumber(uint64_t input)
    {
        std::vector<uint32_t> result;

        while (input != 0)
        {
            result.emplace_back((uint32_t)(input % 10ULL));
            input /= 10ULL;
        }

        std::reverse(result.begin(), result.end());
        return result;
    }

    bool NumberContainsZero(uint64_t input)
    {
        while (input != 0ULL)
        {
            if (input % 10ULL == 0ULL) return true;
            input /= 10ULL;
        }
        return false;
    }

    void Part1()
    {
        const auto program = ProgramFromInput(std::cin);

        // uint64_t current{ 99971999999999 };
        // uint64_t current{ 99599969999ULL};
        uint64_t current{ 9999ULL};
        while (true)
        {
            if (NumberContainsZero(current))
            {
                --current;
                continue;
            }

            // if (current < 9950UL) throw std::exception();

            // if (current < 10000000000000UL) throw std::exception(); // number is now smaller than 14 digits

            // std::cerr << "Input: " << current << std::endl;
            const auto input = BreakNumber(current);
            // if (input.size() != 14) throw std::exception();
            CPU trial{program, input};
            trial.Execute();
            // std::cerr << "z: " << trial.z << std::endl;

            // if (trial.z < (trial.zHistory[trial.zHistory.size() - 1] - 1000)) break;
            if (trial.z == trial.zHistory[trial.zHistory.size() - 2]) std::cerr << "z: " << trial.z << " current: " << current << std::endl;

            // if (trial.z <= 1000) break; // solved it!
            --current;
            // current -= 101;
        }
        // CPU bigNum{program, BreakNumber(99999999999999ULL)};
        // bigNum.Execute();
        // std::cerr << "z: " << bigNum.z << std::endl;

        // CPU sample{program, BreakNumber(13579246899999ULL)};
        // sample.Execute();
        // std::cerr << "z: " << sample.z << std::endl;

        std::cout << current << std::endl;
        // std::cout << 0 << std::endl;
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