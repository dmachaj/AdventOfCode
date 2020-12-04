#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint

namespace
{
    void Part1()
    {
        std::string bigInput;
        
        char oneChar = oneChar = (char)std::cin.get();
        while (oneChar != EOF)
        {
            bigInput += oneChar;
            oneChar = (char)std::cin.get();
        }

        std::vector<std::string> regions{};
        auto offset = 0;
        const auto doubleBreak = "\n\n"sv;
        while (bigInput.find(doubleBreak, offset) != std::string::npos)
        {
            auto end = bigInput.find(doubleBreak, offset);
            std::string region = bigInput.substr(offset, end - offset);
            // std::cerr << region << std::endl << std::endl;
            regions.emplace_back(std::move(region));
            offset = end + 2;
        }

        std::string region = bigInput.substr(offset);
        // std::cerr << region << std::endl << std::endl;
        regions.emplace_back(std::move(region));

        uint32_t validPassports{};
        for (const auto& passport : regions)
        {
            if ((passport.find("byr") != std::string::npos) &&
                (passport.find("iyr") != std::string::npos) &&
                (passport.find("eyr") != std::string::npos) &&
                (passport.find("hgt") != std::string::npos) &&
                (passport.find("hcl") != std::string::npos) &&
                (passport.find("ecl") != std::string::npos) &&
                (passport.find("pid") != std::string::npos))
                // (passport.find("cid") != std::string::npos)) // this is the puzzle
            {
                validPassports++;
            }
        }

        std::cout << validPassports << std::endl;
    }

    std::string ExtractSubstring(const std::string& passport, const std::string_view& token)
    {
        const auto byr = passport.find(token) + 4;
        const auto byrEnd = passport.find(" ", byr);
        const auto byrEnd2 = passport.find("\n", byr);
        const auto compareFinds = [](const auto lhs, const auto rhs)
        {
            if (lhs == std::string::npos) return rhs;
            else if (rhs == std::string::npos) return lhs;
            else return std::min(lhs, rhs);
        };
        const auto wholeString = passport.substr(byr, compareFinds(byrEnd, byrEnd2) - byr);
        std::cerr << token << wholeString << std::endl;
        return wholeString;
    }

    void Part2()
    {
        std::string bigInput;
        
        char oneChar = oneChar = (char)std::cin.get();
        while (oneChar != EOF)
        {
            bigInput += oneChar;
            oneChar = (char)std::cin.get();
        }

        std::vector<std::string> regions{};
        auto offset = 0;
        const auto doubleBreak = "\n\n"sv;
        while (bigInput.find(doubleBreak, offset) != std::string::npos)
        {
            auto end = bigInput.find(doubleBreak, offset);
            std::string region = bigInput.substr(offset, end - offset);
            // std::cerr << region << std::endl << std::endl;
            regions.emplace_back(std::move(region));
            offset = end + 2;
        }

        std::string region = bigInput.substr(offset);
        // std::cerr << region << std::endl << std::endl;
        regions.emplace_back(std::move(region));

        uint32_t validPassports{};
        for (const auto& passport : regions)
        {
            if ((passport.find("byr") != std::string::npos) &&
                (passport.find("iyr") != std::string::npos) &&
                (passport.find("eyr") != std::string::npos) &&
                (passport.find("hgt") != std::string::npos) &&
                (passport.find("hcl") != std::string::npos) &&
                (passport.find("ecl") != std::string::npos) &&
                (passport.find("pid") != std::string::npos))
                // (passport.find("cid") != std::string::npos)) // this is the puzzle
            {
                std::cerr << std::endl;
                {
                    const auto byr = ExtractSubstring(passport, "byr:"sv);
                    const auto year = std::atoi(byr.c_str());
                    if ((year < 1920) || (year > 2002)) continue;
                }

                {
                    const auto iyr = ExtractSubstring(passport, "iyr:"sv);
                    const auto year = std::atoi(iyr.c_str());
                    if ((year < 2010) || (year > 2020)) continue;
                }

                {
                    const auto eyr = ExtractSubstring(passport, "eyr:"sv);
                    const auto year = std::atoi(eyr.c_str());
                    if ((year < 2020) || (year > 2030)) continue;
                }
                
                {
                    const auto hgt = ExtractSubstring(passport, "hgt:"sv);
                    const auto cm = hgt.find("cm");
                    if (cm != std::string::npos)
                    {
                        const auto height = hgt.substr(0, cm);
                        const auto heightNum = std::atoi(height.c_str());
                        if ((heightNum < 150) || (heightNum > 193)) continue;
                    }
                    
                    const auto in = hgt.find("in");
                    if (in != std::string::npos)
                    {
                        const auto height = hgt.substr(0, in);
                        const auto heightNum = std::atoi(height.c_str());
                        if ((heightNum < 59) || (heightNum > 76)) continue;
                    }

                    if ((in == std::string::npos) && (cm == std::string::npos)) continue;
                }

                {
                    const auto hcl = ExtractSubstring(passport, "hcl:"sv);
                    if (hcl.length() != 7) continue;
                    if (hcl[0] != '#') continue;
                    bool valid{true};
                    for (auto i = 1; i < hcl.length(); ++i)
                    {
                        const auto letter = hcl[i];
                        const bool isDigit = (letter >= '0' && letter <= '9');
                        const bool isLetter = (letter >= 'a' && letter <= 'f') || (letter >= 'A' && letter <= 'F');
                        if (!(isDigit || isLetter))
                        {
                            valid = false;
                            break;
                        }
                    }
                    if (!valid) continue;
                }

                {
                    const auto ecl = ExtractSubstring(passport, "ecl:"sv);
                    if ((ecl.find("amb") == std::string::npos) && 
                        (ecl.find("blu") == std::string::npos) && 
                        (ecl.find("brn") == std::string::npos) && 
                        (ecl.find("gry") == std::string::npos) && 
                        (ecl.find("grn") == std::string::npos) && 
                        (ecl.find("hzl") == std::string::npos) && 
                        (ecl.find("oth") == std::string::npos))
                    {
                        continue;
                    }
                }

                {
                    const auto pid = ExtractSubstring(passport, "pid:"sv);
                    if (pid.length() != 9) continue;
                    bool valid{true};
                    for (const auto digit : pid)
                    {
                        if (digit < '0' || digit > '9') { valid = false; }
                    }
                    if (!valid) continue;
                }

                std::cerr << "Valid!" << std::endl;
                validPassports++;
            }
        }

        std::cout << validPassports << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}