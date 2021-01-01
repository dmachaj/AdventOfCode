# Intcode shared implementation

## Problems using intcode

* https://adventofcode.com/2019/day/2 (original)
* https://adventofcode.com/2019/day/5 (instructions 5-8 and param modes)
* https://adventofcode.com/2019/day/7 (feed into each other)
* https://adventofcode.com/2019/day/9 (relative mode, opcode 9, increased address space, int64)
* https://adventofcode.com/2019/day/11 (uses intcode, returns data and reads data each cycle)
* https://adventofcode.com/2019/day/13 (arcade game, parse output into rendered screen)
* https://adventofcode.com/2019/day/15 (maze exploration)
* https://adventofcode.com/2019/day/17 (traverse scaffolding)
* https://adventofcode.com/2019/day/19 (tractor beam)
* https://adventofcode.com/2019/day/21 (jumping robot with its own assembly language)
* (never touched) day 23 intcode
* (never touched) day 25 intcode

## Opcodes

Opcode number|Description
-|-
1|add
2|multiply
3|input (stdin)
4|output (stdout)
5|jump if true (if first param is nonzero jump to second param)
6|jump if false (reverse of 5)
7|less than (compares first two params, stores 1 or 0 in third param)
8|equals (compares first two, stores 1 or 0 in third param)
9|adjust relative base (add/subtract, not absolute value)
99|terminate program

## Addressing modes

opcode field has hundreds, thousands, ten-thousands digits set or unset (0) to determine mode for that param

* param mode 0 = position mode (address == position)
* param mode 1 = immediate mode (address is literal value)
* param mode 2 = relative mode (relative to a base pointer instead of 0)

## Notes

Day5 has some test programs (not immediately compatible with that program otherwise)
