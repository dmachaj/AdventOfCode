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
    // Ingredients list, allergens list
    std::pair<std::vector<std::string>, std::vector<std::string>> ParseLine(const std::string& input)
    {
        std::string temp;

        const auto ingredientsStr = input.substr(0, input.find("("));
        istringstream istream(ingredientsStr);
        std::vector<std::string> ingredients;
        while (std::getline(istream, temp, ' '))
        {
            ingredients.emplace_back(std::move(temp));
        }

        auto allergensStr = input.substr(input.find("contains ") + 8);
        allergensStr = allergensStr.substr(0, allergensStr.size() - 1); // drop trailing )
        istringstream astream(allergensStr);
        std::vector<std::string> allergens;
        while (std::getline(astream, temp, ','))
        {
            allergens.emplace_back(temp.substr(1)); // drop preceeding space
        }

        return std::make_pair(ingredients, allergens);
    }

    // struct LargestPossibilityListFirst
    // {
    //     bool operator()(const std::vector<std::vector<std::string>>& lhs, const std::vector<std::vector<std::string>>& rhs)
    //     {
    //         return lhs.size() > rhs.size();
    //     }
    // };

    void Part1()
    {
        std::unordered_map<std::string, std::vector<std::vector<std::string>>/*, LargestPossibilityListFirst*/> allergenToIngredientPossibilities;
        std::unordered_set<std::string> allAllergens;
        std::unordered_set<std::string> allIngredients;
        std::vector<std::string> everyIngredient;

        std::string input;
        while (std::getline(std::cin, input))
        {
            auto [ingredients, allergens] = ParseLine(input);
            
            for (const auto& allergen : allergens)
            {
                allAllergens.emplace(allergen);
                auto& possibility = allergenToIngredientPossibilities[allergen];
                possibility.push_back(ingredients);
            }

            for (const auto& ingredient : ingredients)
            {
                allIngredients.emplace(ingredient);
            }

            std::copy(ingredients.begin(), ingredients.end(), std::back_inserter(everyIngredient));
        }

        std::unordered_map<std::string, std::string> ingredientAllergenMap; // key = ingredient, value = allergen
        do // keep looping through making matches until we match all allergens.  May take multiple passes
        {
            for (const auto& list : allergenToIngredientPossibilities)
            {
                const auto& allergen = list.first;
                for (const auto& ingredient: allIngredients)
                {
                    if (ingredientAllergenMap.find(ingredient) != ingredientAllergenMap.end()) continue; // ingredient already matched

                    const auto match = std::all_of(list.second.begin(), list.second.end(), [ingredient](const std::vector<std::string> ingredientList)
                    {
                        return std::find(ingredientList.begin(), ingredientList.end(), ingredient) != ingredientList.end();
                    });

                    if (match)
                    {
                        ingredientAllergenMap[ingredient] = allergen;
                        break;
                    }
                }
            }
        } while (ingredientAllergenMap.size() != allAllergens.size());

        uint32_t result{};
        for (const auto& ingredient : everyIngredient)
        {
            if (ingredientAllergenMap.find(ingredient) == ingredientAllergenMap.end())
            {
                ++result;
            }
        }

        std::cout << result << std::endl;
    }

    void Part2()
    {
        std::unordered_map<std::string, std::vector<std::vector<std::string>>/*, LargestPossibilityListFirst*/> allergenToIngredientPossibilities;
        std::unordered_set<std::string> allAllergens;
        std::unordered_set<std::string> allIngredients;
        std::vector<std::string> everyIngredient;

        std::string input;
        while (std::getline(std::cin, input))
        {
            auto [ingredients, allergens] = ParseLine(input);
            
            for (const auto& allergen : allergens)
            {
                allAllergens.emplace(allergen);
                auto& possibility = allergenToIngredientPossibilities[allergen];
                possibility.push_back(ingredients);
            }

            for (const auto& ingredient : ingredients)
            {
                allIngredients.emplace(ingredient);
            }

            std::copy(ingredients.begin(), ingredients.end(), std::back_inserter(everyIngredient));
        }

        std::unordered_map<std::string, std::string> ingredientAllergenMap; // key = ingredient, value = allergen
        do // keep looping through making matches until we match all allergens.  May take multiple passes
        {
            for (const auto& list : allergenToIngredientPossibilities)
            {
                const auto& allergen = list.first;
                for (const auto& ingredient: allIngredients)
                {
                    if (ingredientAllergenMap.find(ingredient) != ingredientAllergenMap.end()) continue; // ingredient already matched

                    const auto match = std::all_of(list.second.begin(), list.second.end(), [ingredient](const std::vector<std::string> ingredientList)
                    {
                        return std::find(ingredientList.begin(), ingredientList.end(), ingredient) != ingredientList.end();
                    });

                    if (match)
                    {
                        ingredientAllergenMap[ingredient] = allergen;
                        break;
                    }
                }
            }
        } while (ingredientAllergenMap.size() != allAllergens.size());

        std::map<std::string, std::string> allergenIngredientMap; // key = allergen, value = ingredient.  Sorted by allergen
        for (const auto& item : ingredientAllergenMap)
        {
            allergenIngredientMap[item.second] = item.first;
        }

        std::string result;
        for (const auto& item : allergenIngredientMap)
        {
            result += item.second;
            result += ",";
        }
        result = result.substr(0, result.size() - 1); // trim trailing comma

        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}