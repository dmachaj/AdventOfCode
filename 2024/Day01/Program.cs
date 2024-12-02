using HelpfulInfrastructure;

public class Program : IProgram
{
    public void Part1()
    {
        List<int> left = new();
        List<int> right = new();

        string? line;
        while (!string.IsNullOrEmpty(line = Console.ReadLine()))
        {
            // Always 3 spaces in between
            left.Add(int.Parse(line.Split(' ')[0]));
            right.Add(int.Parse(line.Split(' ')[3]));
        }

        left.Sort();
        right.Sort();

        int total = 0;
        for (var i = 0; i < left.Count; i++)
        {
            total += Math.Abs(right[i] - left[i]);
        }

        Console.WriteLine($"{total}");
    }

    public void Part2()
    {
        List<int> left = new();
        List<int> right = new();

        string? line;
        while (!string.IsNullOrEmpty(line = Console.ReadLine()))
        {
            // Always 3 spaces in between
            left.Add(int.Parse(line.Split(' ')[0]));
            right.Add(int.Parse(line.Split(' ')[3]));
        }

        int total = 0;
        foreach (int leftVal in left)
        {
            var countInRight = right.Count(x => x == leftVal);
            total += leftVal * countInRight;
        }

        Console.WriteLine($"{total}");
    }

    public static void Main()
    {
        HelpfulInfrastructure.HelpfulInfrastructure.RunEntryPoint(new Program());
    }
}
