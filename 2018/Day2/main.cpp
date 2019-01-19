#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>

int main()
{
    std::string input;
    uint32_t countDoubles{};
    uint32_t countTriples{};
    while (std::getline(std::cin, input))
    {
        bool containsDouble{};
        bool containsTriple{};
        for (auto letter = 'a'; letter <= 'z'; ++letter)
        {
            const auto count = std::count_if(input.begin(), input.end(), [letter](auto inputLetter) -> bool { return inputLetter == letter; });
            if (count == 2)
            {
                containsDouble = true;
            }
            else if (count == 3)
            {
                containsTriple = true;
            }
        }

        if (containsDouble)
        {
            countDoubles++;
        }
        
        if (containsTriple)
        {
            countTriples++;
        }
    }

    std::cout << (countDoubles * countTriples) << std::endl;
    return 0;
}