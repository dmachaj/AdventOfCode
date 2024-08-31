using HelpfulInfrastructure;

public class Program : IProgram
{
    public void Part1()
    {
        Console.WriteLine("0");
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
