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

## Blocking cases handled
- **Deadlock (Coffman's conditions):** dongles are always acquired in ascending `id` order (lowest if first), breaking the circular-wait condition that causes the classic dining-philosophers deadlock.
- **Starvation prevention:** each dongle keeps a min-heap of pending requests, served in `fifo` (arrival order) or `edf` (earliest burnout deadline) order, so no coder waits indefinitely.
- **Cooldown handling:** a dongle records its release time; the next holder waits out `dongle_cooldown` before it is granted.
- **Burnout detection:** a dedicated monitor thread checks each coder's last compile time every 1ms and stops the simulation within 10ms of an actual burnout.
- **Log serialization:** a single `write_mutex` guarantees log lines are never interleaved.

## Thread synchronization mechanisms
- **`pthread_mutex_t` (per dongle):** protects each dongle's `taken` state and its request queue from concurrency access by coder threads.
- **`pthread_cond_t` (per dongle):** coders call `pthread_cond_wait` when a dongle is unavailable or it is not their turn, and are woken with `pthread_cond_broadcast` when the dongle is released — avoiding busy-waiting.
- **`state_mutex`:** protects shared simulation state (`simulation_stop`, `fifo_counter`, coders' compile counters) read by both coder threads and the monitor thread.
- **`write_mutex`:** serializes all `printf` calls so two state changes never print on the same line.
- **Race conditions** are prevented because every read/write to shared data (dongle state, coder counters, simulation flag) always happens while holding the corresponding mutex; the monitor and coder threads never access this data unprotected.

## Resources
- **Subject:** 42 Codexion subject (provided by the school).
- **Tutorials:** Full YouTube course on multithreading and POSIX threads - [Mutex streets in C - SMA CODING](https://www.youtube.com/watch?v=Pgfujwx5Ykg&list=PL2opeqXBU7T3IS414KPCTiHXVyM_vQsry)
- **AI usage:** AI was used to review the concurrency logic (deadlock ordering, cooldown, and scheduler design) and to help debug synchronization issue (double-lock, missing mutex/cond initialization). All code was written and understood manually before being integrated.