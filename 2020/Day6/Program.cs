using HelpfulInfrastructure;
using System;
using System.Diagnostics.Metrics;
using static System.Net.Mime.MediaTypeNames;

public class Program : IProgram
{
    private int ProcessEntry(string input)
    {
        return (from letter in input select letter).Distinct().Count();
    }
    public void Part1()
    {
        List<string> allLines = new();
        string? input;
        while ((input = Console.ReadLine()) != null)
        {
            allLines.Add(input);
        }
        allLines.Add(string.Empty); // add a blank line to the end to ease the next loop

        int runningTotal = 0;
        string current = string.Empty;
        foreach (var line in allLines)
        {
            if (string.IsNullOrEmpty(line))
            {
                runningTotal += ProcessEntry(current);
                current = string.Empty;
                continue;
            }
            current += line;
        }

        Console.WriteLine($"{runningTotal}");
    }
    private int ProcessEntry2(List<string> inputs)
    {
        int result = 0;

        for (char letter = 'a'; letter <= 'z'; ++letter)
        {
            int containsCount = (from input in inputs where input.Contains(letter) select input).Count();
            if (containsCount == inputs.Count)
            {
                result++;
            }
        }

        return result;
    }

    public void Part2()
    {
        List<string> allLines = new();
        string? input;
        while ((input = Console.ReadLine()) != null)
        {
            allLines.Add(input);
        }
        allLines.Add(string.Empty); // add a blank line to the end to ease the next loop

        int runningTotal = 0;
        List<string> current = new();
        foreach (var line in allLines)
        {
            if (string.IsNullOrEmpty(line))
            {
                runningTotal += ProcessEntry2(current);
                current = new();
                continue;
            }
            current.Add(line);
        }

        Console.WriteLine($"{runningTotal}");
    }

    public static void Main()
    {
        HelpfulInfrastructure.HelpfulInfrastructure.RunEntryPoint(new Program());
    }
}
