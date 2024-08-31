using HelpfulInfrastructure;
using System;

public class Program : IProgram
{
    public void Part1()
    {
        int validPasswords = 0;

        string? line;
        while (!String.IsNullOrEmpty(line = Console.ReadLine()))
        {
            string[] tokens = line.Split(' ');

            var min = int.Parse(tokens[0].Split('-')[0]);
            var max = int.Parse(tokens[0].Split('-')[1]);
            var targetLetter = tokens[1][0];
            var password = tokens[2];

            int countOfLetter = (from l in password where l == targetLetter select l).Count();
            if ((countOfLetter >= min) && (countOfLetter <= max))
            {
                validPasswords++;
            }
        }
        Console.WriteLine($"{validPasswords}");
    }

    public void Part2()
    {
        int validPasswords = 0;

        string? line;
        while (!String.IsNullOrEmpty(line = Console.ReadLine()))
        {
            string[] tokens = line.Split(' ');

            var min = int.Parse(tokens[0].Split('-')[0]);
            var max = int.Parse(tokens[0].Split('-')[1]);
            var targetLetter = tokens[1][0];
            var password = tokens[2];

            bool isValid = (password[min - 1] == targetLetter) ^ (password[max - 1] == targetLetter);
            if (isValid)
            {
                validPasswords++;
            }
        }
        Console.WriteLine($"{validPasswords}");
    }

    public static void Main()
    {
        HelpfulInfrastructure.HelpfulInfrastructure.RunEntryPoint(new Program());
    }
}
