using HelpfulInfrastructure;
using System;

public class Program : IProgram
{
    private int ParseTicket(string input)
    {
        int lower = 0;
        int upper = 128;
        int index = 0;

        while (upper - lower > 1)
        {
            char letter = input[index];
            if (letter == 'F')
            {
                upper = ((upper - lower)) / 2 + lower;
            }
            else if (letter == 'B')
            {
                lower = ((upper - lower)) / 2 + lower;
            }
            else
            {
                throw new Exception("Unexpected character");
            }
            ++index;
        }

        int colMin = 0;
        int colMax = 8;
        while (colMax - colMin > 1)
        {
            char letter = input[index];
            if (letter == 'L')
            {
                colMax = ((colMax - colMin) / 2) + colMin;
            }
            else if (letter == 'R')
            {
                colMin = ((colMax - colMin) / 2) + colMin;
            }
            else
            {
                throw new Exception("Unexpected character");
            }
            ++index;
        }

        int result = (lower * 8) + colMin;
        return result;
    }

    public void Part1()
    {
        int highestBoardingPass = 0;

        string? input;
        while (!string.IsNullOrEmpty(input = Console.ReadLine()))
        {
            var passId = ParseTicket(input);
            if (passId > highestBoardingPass) highestBoardingPass = passId;
        }
        Console.WriteLine($"{highestBoardingPass}");
    }

    public void Part2()
    {
        List<int> allSeats = new();

        string? input;
        while (!string.IsNullOrEmpty(input = Console.ReadLine()))
        {
            var passId = ParseTicket(input);
            allSeats.Add(passId);
        }

        allSeats.Sort();
        for (int i = 1; i < allSeats.Count; ++i)
        {
            if ((allSeats[i] - allSeats[i - 1]) != 1)
            {
                Console.WriteLine($"{allSeats[i - 1] + 1}");
                return;
            }
        }

        Console.WriteLine("-1");
    }

    public static void Main()
    {
        HelpfulInfrastructure.HelpfulInfrastructure.RunEntryPoint(new Program());
    }
}
