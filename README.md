# DMOJ Documentation

Documentation for the [DMOJ judge](https://github.com/DMOJ/judge) system.

## Table of Contents
* [**Judge configuration format (judge.json)**](/judge.md)
  
* [**Problem file format**](/problem.md)
  * [`test_cases` node](/problem.md#test_cases)
    * [Normal Cases](/problem.md#normal-cases)
    * [Batched Cases](/problem.md#batched-cases)
  * [Interactive Problems - `grader` node](/problem.md#interactive-problems---grader)
  * [Custom Checkers - `checker` node](/problem.md#custom-checkers---checker)
  * [Checkers or Interactors for Computationally-Heavy Problems (subprocessing C++)](/problem.md#checkers-or-interactors-for-computationally-heavy-problems)
  * [Function Signature Grading (IOI-style)](/problem.md#function-signature-grading-ioi-style)

For examples of the different problem types, see [the sample problems provided](/problem_examples). The related problem statements may be found on <https://dmoj.ca>, under the same codes.

* [**Colored Compiler Messages**](/colored-compiler-messages.md)
  * [Setting up the Judge](/colored-compiler-messages.md#setting-up-the-judge)
  * [Enabling Colors in GCC 4.9+](/colored-compiler-messages.md#enabling-colors-in-gcc-49)
  * [Enabling Colors in GCC < 4.9](/colored-compiler-messages.md#enabling-colors-in-gcc--49)
    * [Using `colorgcc` alongside a GCC 4.9+ installation](/colored-compiler-messages.md#using-colorgcc-alongside-a-gcc-49-installation)
  * [Clang](/colored-compiler-messages.md#clang)
  * [Colors for Other Languages](/colored-compiler-messages.md#colors-for-other-languages)
