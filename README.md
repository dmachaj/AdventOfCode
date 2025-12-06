# AdventOfCode

This repo contains my own (dmachaj) personal solutions to various Advent of Code challenges.  This repo is public but it is __not__ licensed for any sort of reuse or redistribution.

Advent of Code is a time-based challenge so the code is on the sloppy side and is not a reflection of what I consider to be production-quality code.  Similarly the build and verification scripts are super quick&dirty but are fast and effective enough for this usage.

## Building (C++)

The code for this project is C++ and uses the MSVC compiler.  There are helper scripts to build and verify a given day's challenge.
1. Begin with a "Visual Studio Dev Tools" command prompt
2. `cd` to the directory that you are trying to build.  For example `cd 2023\Day01`.
3. `..\..\build.cmd` to build that program
4. `..\..\verify.cmd Part1` to check the correctness of the first problem for that day.
5. `..\..\verify.cmd Part2` to check the second problem.

When you are done you can run `clean-build-results.cmd` in the root of the repo to clean up any leftovers.

## Building (C#)

The C# code in this project uses .net8 (or newer).  To build simply run dotnet:

1. Begin with a command-prompt in the appropriate directory (e.g. `cd 2021\Day01`).
2. `dotnet build` to compile
3. `..\..\verify.cmd Part1` to check the first problem.
4. `..\..\verify.cmd Part2` to check the second problem.

After completion you can run `clean-build-results.cmd` from the root of the repo to clean up.

## Swapping out stdin for file io (for large inputs that don't fit on the clipboard when debugging)

1. Add `#include <fstream>`
2. Add `#include <istream>`
3. Add to top of function `std::ifstream file(R"(Part1\unique.in)");`
4. Replace input loop `while (std::getline(file, input))`
