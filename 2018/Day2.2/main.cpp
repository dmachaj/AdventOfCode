#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::vector<std::string> allInputs{};
    std::string input;
    while (std::getline(std::cin, input))
    {
        std::cerr << "Input " << input << std::endl;
        allInputs.emplace_back(std::move(input));
    }

    bool found{};
    for (auto i = 0UL; (i < allInputs.size()) && !found; ++i)
    {
        for (auto j = (i + 1); (j < allInputs.size()) && !found; ++j)
        {
            uint32_t differences{};
            for (auto k = 0UL; k < allInputs[i].size(); ++k)
            {
                if (allInputs[i][k] != allInputs[j][k])
                {
                    differences++;
                }
            }

            if (differences == 1)
            {
                for (auto k = 0UL; k < allInputs[i].size(); ++k)
                {
                    if (allInputs[i][k] == allInputs[j][k])
                    {
                        std::cout << allInputs[i][k];
                    }
                }
                found = true;
            }
        }
    }
    
    std::cout << std::endl;
    return 0;
}