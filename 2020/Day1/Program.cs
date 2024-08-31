using HelpfulInfrastructure;

public class Program : IProgram
{
    public void Part1()
    {
        List<int> numbers = new List<int>();
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
        Console.WriteLine("1");
    }

    public static void Main()
    {
        HelpfulInfrastructure.HelpfulInfrastructure.RunEntryPoint(new Program());
    }
}
