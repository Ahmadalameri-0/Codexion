*The very first line must be italicized and read: This activity has been created as part of the 42 curriculum by abani-am*

## Description

Codexion is a multi-threaded simulation written in C that explores the challenges of concurrent programming and resource management. The project models a circular co-working hub where a specific number of coders must share a limited set of USB dongles to compile their code.

Each coder cycles through three states: Compiling (requires acquiring two adjacent dongles), Debugging, and Refactoring. The objective is to design a robust arbitration system that ensures fair access to the shared dongles without causing deadlocks or allowing any coders to "burn out" (starve) from waiting too long. The simulation dynamically adapts to different scheduling policies, specifically FIFO (First In, First Out) and EDF (Earliest Deadline First), to dictate resource distribution.

## Instructions
**Compilation:**
To compile the project, simply run the following command at the root of the repository:
`make`

**Execution:**
Run the executable with the required mandatory arguments:
`./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>`

*Example:*
`./Codexion 5 800 200 200 200 5 10 edf`

## Resources
- **Tutorials:** [Mutex streets in C - SMA CODING](https://www.youtube.com/watch?v=Pgfujwx5Ykg&list=PL2opeqXBU7T3IS414KPCTiHXVyM_vQsry)
