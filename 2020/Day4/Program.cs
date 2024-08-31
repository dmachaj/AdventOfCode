using HelpfulInfrastructure;
using System;
using System.Linq;
using static System.Runtime.InteropServices.JavaScript.JSType;

public class Program : IProgram
{
    public void Part1()
    {
        int validPassports = 0;

        List<string> passports = new();
        string currentInput = "";
        while (true)
        {
            string? line = Console.ReadLine();
            if (line == null)
            {
                passports.Add(currentInput);
                break;
            }
            else if (string.IsNullOrEmpty(line))
            {
                passports.Add(currentInput);
                currentInput = "";
            }

            currentInput += line + " ";
        }

        foreach (var passport in passports)
        {
            if (passport.Contains("byr") &&
                passport.Contains("iyr") &&
                passport.Contains("eyr") &&
                passport.Contains("hgt") &&
                passport.Contains("hcl") &&
                passport.Contains("ecl") &&
                passport.Contains("pid"))
                // (passport.Contains("cid")) // this is the puzzle
            {
                validPassports++;
            }
        }
        Console.WriteLine(validPassports);
    }

    public void Part2()
    {
        int validPassports = 0;

        List<string> passports = new();
        string currentInput = "";
        while (true)
        {
            string? line = Console.ReadLine();
            if (line == null)
            {
                passports.Add(currentInput);
                break;
            }
            else if (string.IsNullOrEmpty(line))
            {
                passports.Add(currentInput);
                currentInput = "";
            }

            currentInput += line + " ";
        }

        foreach (var passport in passports)
        {
            if (passport.Contains("byr") &&
                passport.Contains("iyr") &&
                passport.Contains("eyr") &&
                passport.Contains("hgt") &&
                passport.Contains("hcl") &&
                passport.Contains("ecl") &&
                passport.Contains("pid"))
            // (passport.Contains("cid")) // this is the puzzle
            {
                var tokens = passport.Split(' ');
                bool isValid = true;
                foreach (var token in tokens)
                {
                    if (!isValid) break;

                    if (string.IsNullOrEmpty(token)) continue;
                    else if (token.StartsWith("byr"))
                    {
                        int year = int.Parse(token.Split(':')[1]);
                        if ((year < 1920) || (year > 2002)) { isValid = false; continue; }
                    }
                    else if (token.StartsWith("iyr"))
                    {
                        int year = int.Parse(token.Split(':')[1]);
                        if ((year < 2010) || (year > 2020)) { isValid = false; continue; }
                    }
                    else if (token.StartsWith("eyr"))
                    {
                        int year = int.Parse(token.Split(':')[1]);
                        if ((year < 2020) || (year > 2030)) { isValid = false; continue; }
                    }
                    else if (token.StartsWith("hgt"))
                    {
                        var heightStr = token.Split(':')[1];
                        if (heightStr.EndsWith("cm"))
                        {
                            int height = int.Parse(heightStr.Substring(0, heightStr.Length - 2));
                            if ((height < 150) || (height > 193)) { isValid = false; continue; }
                        }
                        else if (heightStr.EndsWith("in"))
                        {
                            int height = int.Parse(heightStr.Substring(0, heightStr.Length - 2));
                            if ((height < 59) || (height > 76)) { isValid = false; continue; }
                        }
                        else
                        {
                            isValid = false;
                            continue;
                        }
                    }
                    else if (token.StartsWith("hcl"))
                    {
                        string hcl = token.Split(':')[1];
                        if (hcl.Length != 7) { isValid = false; continue; }
                        if (hcl[0] != '#') { isValid = false; continue; }
                        int validLetters = (from letter in hcl where (letter >= 'a' && letter <= 'f') || (letter >= 'A' && letter <= 'A') || Char.IsDigit(letter) select letter).Count();
                        if (validLetters != 6) { isValid = false; continue; }
                    }
                    else if (token.StartsWith("ecl"))
                    {
                        string ecl = token.Split(':')[1];
                        if ((ecl != "amb") && (ecl != "blu") && (ecl != "brn") && (ecl != "gry") && (ecl != "grn") && (ecl != "hzl") && (ecl != "oth")) { isValid = false; continue; }
                    }
                    else if (token.StartsWith("pid"))
                    {
                        string pid = token.Split(':')[1];
                        if (pid.Length != 9) { isValid = false; continue; }
                        int validDigits = (from letter in pid where Char.IsDigit(letter) select letter).Count();
                        if (validDigits != 9) { isValid = false; continue; }
                    }
                    else if (token.StartsWith("cid")) { } // do nothing
                    else { throw new Exception("wtf"); }
                }

                if (isValid)
                {
                    validPassports++;
                }
            }
        }
        Console.WriteLine(validPassports);
    }

    public static void Main()
    {
        HelpfulInfrastructure.HelpfulInfrastructure.RunEntryPoint(new Program());
    }
}
