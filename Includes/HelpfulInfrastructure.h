#pragma once
#include <string>

inline bool RunPart1()
{
    std::string partToRun;
    {
        char* partToRunRaw{};
        size_t partToRunSize{};
        getenv_s(&partToRunSize, nullptr, 0, "PART_TO_RUN");
        if (partToRunSize != 0)
        {
            partToRunRaw = new char[partToRunSize];
            getenv_s(&partToRunSize, partToRunRaw, partToRunSize, "PART_TO_RUN");
            partToRun = partToRunRaw;
        }
    }
    return (partToRun != "Part2"); // When in doubt, run part 1
}