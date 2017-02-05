# Table of Contents
- [**Linux installation**](judge/installation.md)
- [**Windows installation**](judge/windows_installation.md)

- [**Supported Languages**](judge/supported_languages.md)
- [**Problem file format**](judge/problem_format.md)
  - [`test_cases` node](judge/problem_format/#test_cases)
    - [Normal Cases](judge/problem_format/#normal-cases)
    - [Batched Cases](judge/problem_format/#batched-cases)

  - [Interactive Problems - `grader` node](judge/problem_format/#interactive-problems-grader)
  - [Custom Checkers - `checker` node](judge/problem_format/#custom-checkers-checker)
  - [Checkers or Interactors for Computationally-Heavy Problems (subprocessing C++)](judge/problem_format/#checkers-or-interactors-for-computationally-heavy-problems)
  - [Function Signature Grading (IOI-style)](judge/problem_format/#function-signature-grading-ioi-style)

For examples of the different problem types, see [the sample problems provided](/problem_examples). The related problem statements may be found on [https://dmoj.ca](https://dmoj.ca), under the same codes.
- [**Colored Compiler Messages**](judge/colored_compiler_messages)
  - [Setting up the Judge](judge/colored_compiler_messages/#setting-up-the-judge)
  - [Enabling Colors in GCC 4.9+](judge/colored_compiler_messages/#enabling-colors-in-gcc-49)
  - [Enabling Colors in GCC < 4.9](judge/colored_compiler_messages/#enabling-colors-in-gcc-49_1)
    - [Using `colorgcc` alongside a GCC 4.9+ installation](judge/colored_compiler_messages/#using-colorgcc-alongside-a-gcc-49-installation)

  - [Clang](judge/colored_compiler_messages/#clang)
  - [Colors for Other Languages](judge/colored_compiler_messages/#colors-for-other-languages)

- [**SSL Proxying for User Content**](/ssl_content_proxy)
  - [Installing camo to /code](site/ssl_content_proxy/#installing-camo-to-code)
  - [Configuring DMOJ to use Camo](site/ssl_content_proxy/#configuring-dmoj-to-use-camo)
