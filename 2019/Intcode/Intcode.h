#pragma once
#include <functional>
#include <sstream>

namespace Intcode
{
    constexpr size_t c_memorySize{10000};

    enum class OpCode
    {
        Add = 1,
        Multiply = 2,
        Input = 3,
        Output = 4,
        JumpIfTrue = 5,
        JumpIfFalse = 6,
        LessThan = 7,
        Equals = 8,
        AdjustRelativeBase = 9,
        Terminate = 99
    };

    struct ProgramState
    {
        std::vector<int64_t> program{};
        uint64_t instructionCounter{};
        int64_t relativeBasePointer{};
    };

    bool ParamIsImmediate(int64_t data, uint64_t offset)
    {
        if ((uint64_t)data >= offset)
        {
            const auto result = (data / offset % 10);
            return (result == 1);
        } 
        return false;
    }

    bool ParamIsRelative(int64_t data, uint64_t offset)
    {
        if ((uint64_t)data >= offset)
        {
            const auto result = (data / offset % 10);
            return (result == 2);
        } 
        return false;
    }

    ProgramState ParseProgram()
    {
        std::vector<int64_t> program(c_memorySize, 0);

        std::string input;
        size_t offset{};
        while (std::getline(std::cin, input, ','))
        {
            program[offset] = (std::atoll(input.c_str()));
            ++offset;
        }

        return {std::move(program), 0, 0};
    }

    std::string ProgramToString(const ProgramState& state, size_t trailingZerosToKeep = 0)
    {
        auto copy = state;

        // Remove all trailing zeroes from the unallocated space
        auto lastNonZero = std::find_if(copy.program.rbegin(), copy.program.rend(), [](const auto value) { return value != 0; });
        copy.program.erase((lastNonZero.base() + trailingZerosToKeep), copy.program.end());

        std::ostringstream outputStream{};
        for (const auto entry : copy.program)
        {
            outputStream << entry << ",";
        }
        std::string output = outputStream.str();
        output.erase(output.length() - 1, 1); // remove trailing comma
        return output;
    }

    class IIntcodeInput
    {
    public:
        virtual int64_t NextInput() = 0;
    };

    // Takes a vector of integers and returns them one-by-one as input to the intcode program
    class VectorIntcodeInput : public IIntcodeInput
    {
    public:
        VectorIntcodeInput() = default;
        VectorIntcodeInput(std::vector<int64_t> inputs):
            inputs(std::move(inputs))
        {}

        int64_t NextInput() override
        {
            if (index >= inputs.size()) { throw std::out_of_range("Overflow"); }
            return inputs[index++];
        }

    private:
        std::vector<int64_t> inputs{};
        uint32_t index{};
    };

    // Calls back the calling code whenever there is a request for input.
    class CallbackIntcodeInput : public IIntcodeInput
    {
    public:
        CallbackIntcodeInput(std::function<int64_t(void)> callback):
            callback(callback)
        {}

        int64_t NextInput() override
        {
            return callback();
        }

    private:
        std::function<int64_t(void)> callback;
    };

    class IIntcodeOutput
    {
    public:
        virtual void WriteOutput(int64_t value) = 0;
    };

    // Contains a vector of integers, output one at a time by the program
    class VectorIntcodeOutput : public IIntcodeOutput
    {
    public:
        virtual void WriteOutput(int64_t value) override
        {
            outputs.emplace_back(value);
        }

    // private:
        std::vector<int64_t> outputs{};
    };

    // Calls back the calling code whenever there is output to write
    class CallbackIntcodeOutput : public IIntcodeOutput
    {
    public:
        CallbackIntcodeOutput(std::function<void(int64_t)> callback):
            callback(callback)
        {}

        virtual void WriteOutput(int64_t value) override
        {
            callback(value);
        }

    private:
        std::function<void(int64_t)> callback;
    };

    void ExecuteProgram(ProgramState& state, IIntcodeInput* input, IIntcodeOutput* output)
    {
        auto& program = state.program;
        auto& instructionCounter = state.instructionCounter;

        while ((OpCode)program[instructionCounter] != OpCode::Terminate)
        {
            const auto opcode = (OpCode)(program[instructionCounter] % 100);
            const bool firstParamImmediate = ParamIsImmediate(program[instructionCounter], 100);
            const bool firstParamRelative = ParamIsRelative(program[instructionCounter], 100);
            const bool secondParamImmediate = ParamIsImmediate(program[instructionCounter], 1000);
            const bool secondParamRelative = ParamIsRelative(program[instructionCounter], 1000);
            const bool thirdParamImmediate = ParamIsImmediate(program[instructionCounter], 10000);
            const bool thirdParamRelative = ParamIsRelative(program[instructionCounter], 10000);

            switch (opcode)
            {
            case OpCode::Add:
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                const auto destination = thirdParamRelative ? state.relativeBasePointer + program[instructionCounter + 3] : program[instructionCounter + 3];
                program[destination] = param1 + param2;
                instructionCounter += 4;
                break;
            }
            case OpCode::Multiply:
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                const auto destination = thirdParamRelative ? state.relativeBasePointer + program[instructionCounter + 3] : program[instructionCounter + 3];
                program[destination] = param1 * param2;
                instructionCounter += 4;
                break;
            }
            case OpCode::Input:
            {
                const auto destination = firstParamRelative ? state.relativeBasePointer + program[instructionCounter + 1] : program[instructionCounter + 1];
                program[destination] = input->NextInput();
                instructionCounter += 2;
                break;
            }
            case OpCode::Output:
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                output->WriteOutput(param1);
                instructionCounter += 2;
                break;
            }
            case OpCode::JumpIfTrue:
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                if (param1 != 0)
                {
                    instructionCounter = (uint64_t)param2;
                }
                else
                {
                    instructionCounter += 3;
                }
                break;
            }
            case OpCode::JumpIfFalse:
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                if (param1 == 0)
                {
                    instructionCounter = (uint64_t)param2;
                }
                else
                {
                    instructionCounter += 3;
                }
                break;
            }
            case OpCode::LessThan:
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                const auto destination = thirdParamRelative ? state.relativeBasePointer + program[instructionCounter + 3] : program[instructionCounter + 3];
                program[destination] = (param1 < param2) ? 1 : 0;
                instructionCounter += 4;
                break;
            }
            case OpCode::Equals:
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                const auto destination = thirdParamRelative ? state.relativeBasePointer + program[instructionCounter + 3] : program[instructionCounter + 3];
                program[destination] = (param1 == param2) ? 1 : 0;
                instructionCounter += 4;
                break;
            }
            case OpCode::AdjustRelativeBase:
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                state.relativeBasePointer += param1;
                instructionCounter += 2;
                break;
            }
            default:
                std::cerr << "program[0] at time of crash = " << program[0] << std::endl;
                throw std::exception("Invalid opcode");
            }
        }
    }
}