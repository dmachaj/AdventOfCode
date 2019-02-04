#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct GuardInfo
{
    std::string date;
    uint32_t guardNumber;
    std::array<bool, 60> sleepMinutes;
};

std::vector<std::unique_ptr<GuardInfo>> ReadProblemInput()
{
    std::vector<std::string> allInput;
    {
        std::string input;
        while(std::getline(std::cin, input))
        {
            allInput.emplace_back(std::move(input));
        }

        std::sort(allInput.begin(), allInput.end());
    }

    std::vector<std::unique_ptr<GuardInfo>> allGuards{};
    std::unique_ptr<GuardInfo> currentGuard;
    for (auto inputCount = 0UL; inputCount < allInput.size(); ++inputCount)
    {
        auto& input = allInput[inputCount];
        const auto pound = input.find("#");
        if (pound != std::string::npos)
        {
            if (currentGuard)
            {
                allGuards.emplace_back(std::move(currentGuard));
            }
            currentGuard = std::make_unique<GuardInfo>();
            const auto number = input.substr((pound + 1), input.find(' ', pound) - (pound + 1));
            currentGuard->guardNumber = std::atoi(number.c_str());
            std::cerr << "Found guard " << currentGuard->guardNumber << std::endl;
        }
        else
        {
            std::string wakeupInput = allInput[++inputCount];

            // Shift may begin before midnight but sleep/awake never will so use asleep date as the real date.
            currentGuard->date = input.substr(1, 10);
            const auto asleep = std::atoi(input.substr(15,2).c_str());
            const auto awake = std::atoi(wakeupInput.substr(15,2).c_str());
            std::cerr << "Guard " << currentGuard->guardNumber << " slept " << asleep << " until " << awake << " on " << currentGuard->date << std::endl;
            for (auto i = asleep; i < awake; ++i)
            {
                currentGuard->sleepMinutes[i] = true;
            }
        }
    }
    allGuards.emplace_back(std::move(currentGuard));

    return allGuards;
}

int main()
{
    const auto allGuards = ReadProblemInput();

    // Total up how much time each guard spends sleeping.
    std::unordered_map<uint32_t, uint32_t> guardSleepTotals{};
    for (const auto& guard : allGuards)
    {
        if (guardSleepTotals.find(guard->guardNumber) == guardSleepTotals.end())
        {
            guardSleepTotals[guard->guardNumber] = 0;
        }

        for (const auto isAsleep : guard->sleepMinutes)
        {
            if (isAsleep)
            {
                guardSleepTotals[guard->guardNumber]++;
            }
        }
    }

    // Use STL algorithms to figure out which guard sleeps the most and stash a copy.
    const auto maxSleep = *std::max_element(guardSleepTotals.begin(), guardSleepTotals.end(), [](const auto& left, const auto& right){ return left.second < right.second; });
    const auto longestSleepingGuardId = maxSleep.first;
    std::cerr << "Max sleep is guard " << maxSleep.first << " with " << maxSleep.second << " minutes" << std::endl;
    // const auto whichGuard = std::find_if(allGuards.begin(), allGuards.end(), [maxId {maxSleep.first}](const auto& guard) { return guard->guardNumber == maxId; });

    // Figure out which minute this guard sleeps the most.
    std::array<uint32_t, 60> timesAsleep{};
    for (const auto& guard : allGuards)
    {
        if (guard->guardNumber == longestSleepingGuardId)
        {
            for (auto i = 0UL; i < 60; ++i)
            {
                if (guard->sleepMinutes[i])
                {
                    timesAsleep[i]++;
                }
            }
        }
    }

    const auto maxAlseepMinuteCount = *std::max_element(timesAsleep.begin(), timesAsleep.end());
    uint32_t maxAsleepMinute{};
    for (auto i = 0UL; i < 60; ++i)
    {
        if (timesAsleep[i] == maxAlseepMinuteCount)
        {
            maxAsleepMinute = i;
            break;
        }
    }

    std::cerr << "Guard " << longestSleepingGuardId << " is asleep the most at minute " << maxAsleepMinute << std::endl;
    std::cout << (longestSleepingGuardId * maxAsleepMinute) << std::endl;
    return 0;
}