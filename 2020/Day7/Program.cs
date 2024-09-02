using HelpfulInfrastructure;
using System.Drawing;

public class Program : IProgram
{
    private struct Option
    {
        public Option(string outer, string inner)
        {
            this.outer = outer;
            this.inner = inner;
        }
        public string outer { get; }
        public string inner { get; }
    }

    public void Part1()
    {
        List<Option> options = new();

        string? input;
        while ((input = Console.ReadLine()) != null)
        {
            if (input.Contains("no other bags"))
            {
                // this is useless
                continue;
            }

            string outer = input.Split("bags")[0];
            string inner = input.Split("contain")[1];

            if (outer.Contains("shiny gold"))
            {
                continue;
            }

            options.Add(new Option(outer, inner));
        }

        HashSet<string> colorCandidates = new();
        foreach (var option in options)
        {
            colorCandidates.Add(option.outer);
        }

        HashSet<string> colorsToGold = new() { "shiny gold" };
        while (colorCandidates.Count > 0)
        {
            int before = colorsToGold.Count;
            foreach (var colorCandidate in colorCandidates)
            {
                Option colorSource = options.Find(option => option.outer == colorCandidate);
                var count = from color in colorsToGold where colorSource.inner.Contains(color) select color;
                if (count.Count() > 0)
                {
                    colorsToGold.Add(colorCandidate);
                    break;
                }
            }

            if (colorsToGold.Count == before)
            {
                break; // no more progress to be made
            }

            foreach (var color in colorsToGold)
            {
                colorCandidates.Remove(color);
            }
        }

        Console.WriteLine($"{colorsToGold.Count - 1}");
    }

    private int BagsNeededRecursive(in List<string> allLines, string color)
    {
        var current = (from line in allLines where line.Contains(color + " bags contain") select line).First();
        if (current.Contains("no other bags"))
        {
            return 1; // end recursion
        }

        if (current is null) throw new Exception("Color not found");

        int total = 1; // count the bag itself on top of all that it contains
        var inner = current.Split("contain ")[1];
        while (inner.Contains("bag"))
        {
            var quantity = inner[0] - '0';
            string innerColor = inner.Substring(2, inner.IndexOf("bag") - 3);
            var recurseResult = BagsNeededRecursive(allLines, innerColor);
            total += quantity * recurseResult;

            if (!inner.Contains(","))
            {
                break;
            }
            inner = inner.Substring(inner.IndexOf(",") + 2);
        }

        return total;
    }

    public void Part2()
    {
        List<string> allLines = new();
        string? input;
        while ((input = Console.ReadLine()) != null)
        {
            allLines.Add(input);
        }

        var answer = BagsNeededRecursive(allLines, "shiny gold") - 1; // subtract the gold bag itself
        Console.WriteLine($"{answer}");
    }

    public static void Main()
    {
        HelpfulInfrastructure.HelpfulInfrastructure.RunEntryPoint(new Program());
    }
}
