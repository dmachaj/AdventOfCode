using HelpfulInfrastructure;
using System;
using System.Diagnostics.Metrics;

public class Program : IProgram
{
    public void Part1()
    {
        List<string> map = new();
        string? line;
        while (!String.IsNullOrEmpty(line = Console.ReadLine()))
        {
            map.Add(line);
        }

        int treesHit = 0;
        for (int i = 1; i < map.Count; ++i) // first row is a freebie
        {
            string row = map[i];
            if (row[i * 3 % row.Length] == '#')
            {
                treesHit++;
            }
        }
        Console.WriteLine($"{treesHit}");
    }

    public void Part2()
    {
        List<string> map = new();
        string? line;
        while (!String.IsNullOrEmpty(line = Console.ReadLine()))
        {
            map.Add(line);
        }

        Int64 treesHit1x1= 0;
        for (var i = 1; i < map.Count; ++i) // first row is a freebie
        {
            var row = map[i];
            if (row[(i) % row.Length] == '#')
            {
                treesHit1x1++;
            }
        }

        Int64 treesHit3x1 = 0;
        for (var i = 1; i < map.Count; ++i) // first row is a freebie
        {
            var row = map[i];
            if (row[(i * 3) % row.Length] == '#')
            {
                treesHit3x1++;
            }
        }

        Int64 treesHit5x1 = 0;
        for (var i = 1; i < map.Count; ++i) // first row is a freebie
        {
            var row = map[i];
            if (row[(i * 5) % row.Length] == '#')
            {
                treesHit5x1++;
            }
        }

        Int64 treesHit7x1 = 0;
        for (var i = 1; i < map.Count; ++i) // first row is a freebie
        {
            var row = map[i];
            if (row[(i * 7) % row.Length] == '#')
            {
                treesHit7x1++;
            }
        }

        Int64 treesHit1x2 = 0;
        for (var i = 2; i < map.Count; i += 2) // first row is a freebie
        {
            var row = map[i];
            if (row[(i / 2) % row.Length] == '#')
            {
                treesHit1x2++;
            }
        }

        Console.WriteLine($"{treesHit1x1 * treesHit3x1 * treesHit5x1 * treesHit7x1 * treesHit1x2}");
    }

    public static void Main()
    {
        HelpfulInfrastructure.HelpfulInfrastructure.RunEntryPoint(new Program());
    }
}
