using HelpfulInfrastructure;

public class Program : IProgram
{
    public void Part1()
    {
        List<int> numbers = new();
        string? line;
        while (!string.IsNullOrEmpty(line = Console.ReadLine()))
        {
            numbers.Add(int.Parse(line));
        }

        foreach (int n in numbers)
        {
            foreach (int m in numbers)
            {
                if (n ==m ) continue;

                if ((n + m) == 2020)
                {
                    Console.WriteLine($"{n * m}");
                    return;
                }
            }
        }
    }

    public void Part2()
    {
        List<int> numbers = new();
        string? line;
        while (!string.IsNullOrEmpty(line = Console.ReadLine()))
        {
            numbers.Add(int.Parse(line));
        }

        foreach (int a in numbers)
        {
            foreach (int b in numbers)
            {
                foreach (int c in numbers)
                {
                    if (a == b) continue;
                    if (a == c) continue;
                    if (b == c) continue;

                    if ((a + b + c) == 2020)
                    {
                        Console.WriteLine($"{a * b * c}");
                        return;
                    }
                }
            }
        }
    }

    public static void Main()
    {
        HelpfulInfrastructure.HelpfulInfrastructure.RunEntryPoint(new Program());
    }
}
