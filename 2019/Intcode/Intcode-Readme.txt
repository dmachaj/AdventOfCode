https://adventofcode.com/2019/day/2 (original)
https://adventofcode.com/2019/day/5 (instructions 5-8 and param modes)
https://adventofcode.com/2019/day/7 (feed into each other)
https://adventofcode.com/2019/day/9 (relative mode, opcode 9, increased address space, int64)
https://adventofcode.com/2019/day/11 (uses intcode, returns data and reads data each cycle)
https://adventofcode.com/2019/day/13 (arcade game, parse output into rendered screen)
https://adventofcode.com/2019/day/17 (traverse scaffolding)
https://adventofcode.com/2019/day/19
(never touched) day 15 intcode
(never touched) day 21 intcode
(never touched) day 23 intcode
(never touched) day 25 intcode

opcode 1 = add
opcode 2 = multiply
opcode 3 = input (stdin)
opcode 4 = output (stdout)
opcode 5 = jump if true (if first param is nonzero jump to second param)
opcode 6 = jump if false (reverse of 5)
opcode 7 = less than (compares first two params, stores 1 or 0 in third param)
opcode 8 = equals (compares first two, stores 1 or 0 in third param)
opcode 9 = adjust relative base (add/subtract, not absolute value)
opcode 99 = terminate program

opcode field has hundreds, thousands, ten-thousands digits set or unset (0) to determine mode for that param
param mode 0 = position mode (address == position)
param mode 1 = immediate mode (address is literal value)
param mode 2 = relative mode (relative to a base pointer instead of 0)

day5 has some test programs (not immediately compatible with that program otherwise)

input hard-coded number(s)
input from stdin?
hook up programs to get input from previous instance or program and output to next instance