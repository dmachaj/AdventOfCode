namespace HelpfulInfrastructure
{
    public interface IProgram
    {
        public void Part1();
        public void Part2();
    }

    public static class HelpfulInfrastructure
    {
        private static bool RunPart1()
        {
            string? partToRun = Environment.GetEnvironmentVariable("PART_TO_RUN");
            return partToRun != "Part2";
        }
        public static void RunEntryPoint(IProgram program)
        {
            if (RunPart1())
            {
                program.Part1();
            }
            else
            {
                program.Part2();
            }
        }
    }
}