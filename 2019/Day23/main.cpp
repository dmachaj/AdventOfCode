#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "HelpfulInfrastructure.h"
#include "../Intcode/Intcode.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

namespace
{
    struct Packet
    {
        int64_t destination{};
        int64_t X{};
        int64_t Y{};
    };

    void Part1(std::istream& inStream)
    {
        constexpr uint32_t c_NICCount{50UL};

        auto state = Intcode::ParseProgram(inStream);
        std::array<Intcode::ProgramState, c_NICCount> NICs;
        for (auto i = 0UL; i < c_NICCount; ++i)
        {
            auto stateCopy = state;
            Intcode::VectorIntcodeInput input({(int64_t)i});
            Intcode::VectorIntcodeOutput output;
            Intcode::ExecuteProgram(stateCopy, &input, &output);
            NICs[i] = std::move(stateCopy);
        }

        std::vector<Packet> outstandingPackets;
        int64_t answer{};
        bool foundAnswer{false};
        while(!foundAnswer)
        {
            for (auto i = 0UL; i < c_NICCount; ++i)
            {
                auto& currentNIC = NICs[i];

                std::vector<Packet> packetsForCurrentNIC;
                uint32_t inputsConsumed{};
                for (const auto& packet : outstandingPackets)
                {
                    if (packet.destination == i)
                    {
                        packetsForCurrentNIC.emplace_back(packet);
                    }
                }

                Intcode::CallbackIntcodeInput inputCallback = [packetsForCurrentNIC, &inputsConsumed]() -> int64_t
                {
                    if (inputsConsumed < (packetsForCurrentNIC.size() * 2))
                    {
                        const auto index = inputsConsumed / 2;
                        const auto offset = inputsConsumed % 2;
                        ++inputsConsumed;
                        const auto& packet = packetsForCurrentNIC[index];
                        if (offset == 0) return packet.X;
                        else return packet.Y;
                    }
                    else
                    {
                        return -1;
                    }
                };

                std::vector<int64_t> outputs;
                Intcode::CallbackIntcodeOutput outputCallback = [&outputs](int64_t value)
                {
                    outputs.emplace_back(value);
                };

                Intcode::ExecuteProgram(currentNIC, &inputCallback, &outputCallback, 20);

                // We have a partial packet.  Step one instruction at a time until it is fully read or outputted
                while ((inputsConsumed % 2 != 0) || (outputs.size() % 3 != 0))
                {
                    Intcode::ExecuteProgram(currentNIC, &inputCallback, &outputCallback, 1);
                }

                const auto packetsToRemove = inputsConsumed / 2;
                uint32_t packetsRemoved{0UL};
                for (auto iter = outstandingPackets.begin(); (packetsRemoved < packetsToRemove) && (iter != outstandingPackets.end()); ++iter)
                {
                    if (iter->destination == i)
                    {
                        ++packetsRemoved;
                        outstandingPackets.erase(iter);

                        if (iter != outstandingPackets.begin())
                        {
                            // Back up once so that the ++iter for the loop doesn't skip the entry after the one that was removed.
                            --iter;
                        }
                    }
                }

                for (auto iOutput = 0UL; iOutput < outputs.size();)
                {
                    if ((iOutput + 2) >= outputs.size()) throw;
                    Packet packet { outputs[iOutput], outputs[iOutput + 1], outputs[iOutput + 2]};
                    outstandingPackets.emplace_back(std::move(packet));
                    iOutput += 3;
                }

                auto answerIter = std::find_if(outstandingPackets.begin(), outstandingPackets.end(), [](const Packet& packet)
                {
                    return packet.destination == 255;
                });

                if (answerIter != outstandingPackets.end())
                {
                    foundAnswer = true;
                    answer = answerIter->Y;
                }
            }
        }

        std::cout << answer << std::endl;
    }

    void Part2(std::istream& inStream)
    {
        constexpr uint32_t c_NICCount{50UL};

        auto state = Intcode::ParseProgram(inStream);
        std::array<Intcode::ProgramState, c_NICCount> NICs;
        for (auto i = 0UL; i < c_NICCount; ++i)
        {
            auto stateCopy = state;
            Intcode::VectorIntcodeInput input({(int64_t)i});
            Intcode::VectorIntcodeOutput output;
            Intcode::ExecuteProgram(stateCopy, &input, &output);
            NICs[i] = std::move(stateCopy);
        }

        std::vector<Packet> outstandingPackets;
        std::set<int64_t> answerPossibilities;
        int64_t answer{};
        bool foundAnswer{false};
        Packet NATPacket;
        Packet NATPacketPrevious;
        while(!foundAnswer)
        {
            for (auto i = 0UL; i < c_NICCount; ++i)
            {
                auto& currentNIC = NICs[i];

                std::vector<Packet> packetsForCurrentNIC;
                uint32_t inputsConsumed{};
                for (const auto& packet : outstandingPackets)
                {
                    if (packet.destination == i)
                    {
                        packetsForCurrentNIC.emplace_back(packet);
                    }
                }

                Intcode::CallbackIntcodeInput inputCallback = [packetsForCurrentNIC, &inputsConsumed]() -> int64_t
                {
                    if (inputsConsumed < (packetsForCurrentNIC.size() * 2))
                    {
                        const auto index = inputsConsumed / 2;
                        const auto offset = inputsConsumed % 2;
                        ++inputsConsumed;
                        const auto& packet = packetsForCurrentNIC[index];
                        if (offset == 0) return packet.X;
                        else return packet.Y;
                    }
                    else
                    {
                        return -1;
                    }
                };

                std::vector<int64_t> outputs;
                Intcode::CallbackIntcodeOutput outputCallback = [&outputs](int64_t value)
                {
                    outputs.emplace_back(value);
                };

                Intcode::ExecuteProgram(currentNIC, &inputCallback, &outputCallback, 20);

                // We have a partial packet.  Step one instruction at a time until it is fully read or outputted
                while ((inputsConsumed % 2 != 0) || (outputs.size() % 3 != 0))
                {
                    Intcode::ExecuteProgram(currentNIC, &inputCallback, &outputCallback, 1);
                }

                const auto packetsToRemove = inputsConsumed / 2;
                uint32_t packetsRemoved{0UL};
                for (auto iter = outstandingPackets.begin(); (packetsRemoved < packetsToRemove) && (iter != outstandingPackets.end()); ++iter)
                {
                    if (iter->destination == i)
                    {
                        ++packetsRemoved;
                        outstandingPackets.erase(iter);

                        if (iter != outstandingPackets.begin())
                        {
                            // Back up once so that the ++iter for the loop doesn't skip the entry after the one that was removed.
                            --iter;
                        }
                    }
                }

                for (auto iOutput = 0UL; iOutput < outputs.size();)
                {
                    if ((iOutput + 2) >= outputs.size()) throw;
                    Packet packet { outputs[iOutput], outputs[iOutput + 1], outputs[iOutput + 2]};
                    outstandingPackets.emplace_back(std::move(packet));
                    iOutput += 3;
                }
            }

            std::vector<Packet>::iterator answerIter{};
            do
            {
                answerIter = std::find_if(outstandingPackets.begin(), outstandingPackets.end(), [](const Packet& packet)
                {
                    return packet.destination == 255;
                });

                if (answerIter != outstandingPackets.end())
                {
                    NATPacket = *answerIter;
                    outstandingPackets.erase(answerIter);
                }
            } while (answerIter != outstandingPackets.end());

            if ((outstandingPackets.size() == 0) && (NATPacket.destination == 255))
            {
                if (NATPacket.Y == NATPacketPrevious.Y)
                {
                    answer = NATPacket.Y;
                    foundAnswer = true;
                    break;
                }
                NATPacket.destination = 0;
                outstandingPackets.emplace_back(NATPacket);
                NATPacketPrevious = NATPacket;
                NATPacket = Packet();
            }
        }

        std::cout << answer << std::endl;
    }
}

int main()
{
    std::ifstream inputFileStream;
    inputFileStream.open(LR"(2019/Day23/Part1/unique.in)");
    if (!inputFileStream.is_open())
    {
        inputFileStream.open(LR"(Part1/unique.in)");
    }
    RunPart1() ? Part1(inputFileStream) : Part2(inputFileStream);
    return 0;
}