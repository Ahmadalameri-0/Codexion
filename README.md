
*Built as part of the 42 curriculum by Ahmad Bani Amer*

<div align="center">

# 🧵 Codexion

**An Advanced Multi-Threaded Concurrency & Resource Management Simulation**

[![C](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Threads](https://img.shields.io/badge/POSIX-Threads-4B32C3?style=for-the-badge&logo=linux)](https://en.wikipedia.org/wiki/Pthreads)
[![Architecture](https://img.shields.io/badge/Architecture-Concurrency-FF6F00?style=for-the-badge)](https://en.wikipedia.org/wiki/Concurrency_(computer_science))
[![42](https://img.shields.io/badge/Curriculum-42_Network-000000?style=for-the-badge&logo=42)](https://42.fr/)

*A sophisticated simulation exploring the complexities of thread synchronization, deadlock prevention, and scheduling algorithms.*

</div>

## 📖 Overview

<div align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/7/7b/An_illustration_of_the_dining_philosophers_problem.png" alt="Dining Philosophers Problem" width="350"/>
  <p><em>The classic Dining Philosophers problem, which Codexion models using Coders and Dongles.</em></p>
</div>

**Codexion** is a robust multi-threaded simulation written in C that tackles the classic challenges of concurrent programming. The project models a circular co-working hub where a specific number of coders must share a limited set of USB dongles to compile their code.

Each coder operates as an independent POSIX thread, cycling through three states:
1. ⚙️ **Compiling:** Requires acquiring two adjacent dongles atomically.
2. 🐛 **Debugging:** Independent work, releasing resources.
3. ♻️ **Refactoring:** Independent work.

**The Objective:** Design a flawless arbitration system that ensures fair resource distribution without causing deadlocks (circular wait) or allowing any coder to "burn out" (starve) from resource deprivation. The system dynamically adapts using **FIFO** (First-In, First-Out) and **EDF** (Earliest Deadline First) scheduling algorithms powered by a Min-Heap data structure.

## 🚀 Instructions
**Compilation:**
To compile the project, simply run the following command at the root of the repository:
`make`

**Execution:**
Run the executable with the required mandatory arguments:
`./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>`

*Example:*
`./codexion 5 800 200 200 200 5 10 edf`

## 🛡️ Blocking cases handled
- **Deadlock (Coffman's conditions):** a coder takes both dongles atomically (`take_dongles`), only when both are free and it is first in both queues. A coder therefore never holds one dongle while waiting for another, which breaks the hold-and-wait condition.
- **Starvation prevention:** each dongle keeps a min-heap of pending requests, served in `fifo` (arrival order) or `edf` (earliest burnout deadline) order, so no coder waits indefinitely.
- **Cooldown handling:** a dongle records its release time; a coder is granted its dongles only after `dongle_cooldown` has elapsed, using `pthread_cond_timedwait`.
- **EDF specifics:** ties on equal deadlines are broken by fewer compiles, then by request order. First compiles are staggered so that adjacent coders do not start together.
- **Burnout detection:** a dedicated monitor thread checks each coder's last compile time every 1ms and stops the simulation within 10ms of an actual burnout.
- **Log serialization:** a single `write_mutex` guarantees log lines are never interleaved.
- **Guarantee scope:** as stated in the subject, burnout-free execution is guaranteed only under `edf` and with feasible parameter. `fifo` serves requests by arrival order and ignores deadlines, so tight parameters can still cause a burnout.


## ⚙️ Thread synchronization mechanisms
- **`table_lock` + `table_cond`:** one mutex protects every dongle's `taken` flag, `last_used_time` and request heap. Coders sleep on `pthread_cond_wait` (or `pthread_cond_timedwait` during cooldown) until it is their turn, and are woken with `pthread_cond_broadcast` when a dongle is released or the simulation stops.
- **`state_mutex`:** protects shared simulation state (`simulation_stop`, `fifo_counter`, coders' compile counters) read by both coder threads and the monitor thread.
- **`write_mutex`:** serializes all `printf` calls so two state changes never print on the same line.
- **Race conditions** are prevented because every read/write to shared data (dongle state, coder counters, simulation flag) always happens while holding the corresponding mutex; the monitor and coder threads never access this data unprotected.

## 📚 Resources
- **Subject:** 42 Codexion subject (provided by the school).
- **Tutorials:** Full YouTube course on multithreading and POSIX threads - [Mutex streets in C - SMA CODING](https://www.youtube.com/watch?v=Pgfujwx5Ykg&list=PL2opeqXBU7T3IS414KPCTiHXVyM_vQsry)
- **AI usage:** AI was used to review the concurrency logic (deadlock ordering, cooldown, and scheduler design) and to help debug synchronization issues (double-lock, missing mutex/cond initialization). All code was written and understood manually before being integrated.
