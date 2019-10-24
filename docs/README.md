# Table of Contents
- [**Linux installation**](judge/linux_installation.md)

- [**Supported Languages**](judge/supported_languages.md)
- [**Problem file format**](judge/problem_format.md)
  - [`test_cases` node](judge/problem_format.md/#test_cases)
    - [Normal Cases](judge/problem_format.md/#normal-cases)
    - [Batched Cases](judge/problem_format.md/#batched-cases)

  - [Interactive Problems - `grader` node](judge/problem_format.md/#interactive-problems-grader)
  - [Custom Checkers - `checker` node](judge/problem_format.md/#custom-checkers-checker)
  - [Checkers or Interactors for Computationally-Heavy Problems (subprocessing C++)](judge/problem_format.md/#checkers-or-interactors-for-computationally-heavy-problems)
  - [Function Signature Grading (IOI-style)](judge/problem_format.md/#function-signature-grading-ioi-style)

For examples of the different problem types, see [the sample problems provided](https://github.com/DMOJ/docs/tree/master/problem_examples/). The related problem statements may be found on [https://dmoj.ca](https://dmoj.ca), under the same codes.
- [**Colored Compiler Messages**](judge/colored_compiler_messages.md)
  - [Setting up the Judge](judge/colored_compiler_messages.md/#setting-up-the-judge)
  - [Enabling Colors in GCC 4.9+](judge/colored_compiler_messages.md/#enabling-colors-in-gcc-49)
  - [Enabling Colors in GCC < 4.9](judge/colored_compiler_messages.md/#enabling-colors-in-gcc-49_1)
    - [Using `colorgcc` alongside a GCC 4.9+ installation](judge/colored_compiler_messages/#using-colorgcc-alongside-a-gcc-49-installation)

  - [Clang](judge/colored_compiler_messages.md/#clang)
  - [Colors for Other Languages](judge/colored_compiler_messages.md/#colors-for-other-languages)

- [**SSL Proxying for User Content**](site/ssl_content_proxy.md)
  - [Installing camo to /code](site/ssl_content_proxy.md/#installing-camo-to-code)
  - [Configuring DMOJ to use Camo](site/ssl_content_proxy.md/#configuring-dmoj-to-use-camo)
