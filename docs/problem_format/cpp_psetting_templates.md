# C++ Problem Setting Templates - `cpp_psetting_templates`

There are three C++ input-handling templates provided for aiding problem setters. They are as follows:

- [Validator Template](https://github.com/DMOJ/docs/blob/master/sample_files/problem_setting/validator.cpp)
- [Identical Checker/Interactor Template](https://github.com/DMOJ/docs/blob/master/sample_files/problem_setting/identical_checker_interactor.cpp)
- [Standard Checker/Interactor Template](https://github.com/DMOJ/docs/blob/master/sample_files/problem_setting/standard_checker_interactor.cpp)

## Validator

This is a template for validating the input data of problems. It aims to be simple and of course, correct. It contains seven functions. The first three are whitespace functions:

- `void readSpace()` expects a space at the current position in the input, and aborts the program if there is not a space.
- `void readNewLine()` expects a newline at the current position in the input.
- `void readEOF()` expects the input file to end immediately at the current position.

The remaining four are for actual content:

- `std::string readToken(char min_char = 0, char max_char = 127)` returns the next token in the input stream. A token is defined as a whitespace-separated string. If the next character in the input is a whitespace character, this method aborts the program. The optional arguments `min_char` and `max_char` can be used to enforce a range on the characters in the token. For instance, `readToken('a', 'z')` reads a lowercase string of english letters.
- `long long readInt(long long lo, long long hi)` parses the next token as an integer. It aborts on overflow, malformed integers, and if the resultant integer is not in the range [lo, hi], inclusive.
- `long double readFloat(long double lo, long double hi, long double eps = 1e-9)` parses the next token as a float. It aborts on overflow, malformed floats, and if the resultant float is not in the range [lo, hi], inclusive, using the provided epsilon to perform the comparison. Scientific notation and NaNs are not accepted, nor are leading zeroes. `-0` is allowed. Trailing zeroes are also permitted.
- `std::vector<T> readIntArray(size_t N, long long lo, long long hi)` parses the next space-separated N integers into an array, and then reads a final newline. It must be given a template argument, which is the type of the array elements. For example, `readIntArray<int>(5, 1, 10)` reads five space-separated integers into a `std::vector<int>`, where each integer is in the range [1, 10], inclusive.

`readFloat()` will likely be of no use for many validators, and can be safely deleted. Similarly, `readIntArray` can be deleted if unneeded.

## Checkers/Interactors

The next pair of templates are for checkers/interactors. The difference is the type of whitespace handling: the identical checker/interactor expects whitespace to match exactly. The standard checker/interactor handles whitespace like the `standard` checker.

The checkers and interactors are designed for the `coci` bridged checker/interactor type. However, updating the codes used and the order of command line parameters to work with other types should not be challenging.

Both files can be used for either checkers/interactors, with the following caveat: interactors MUST close `stdout` BEFORE calling `readEOF()`, so that the user process can terminate in case it _also_ expects an EOF.

The general format of the checkers/interactors are the same as the validator, with a few changes:

- `readSpace(), readNewLine(), readEOF()`: Under the identical checker, these return Presentation Error if the check fails. Under the standard checker, these return WA.
- `readToken()`: Under the identical checker, this returns Presentation Error if the token is empty, and WA if any character is not in range.
- `readInt(), readIntArray(), readFloat()`: Returns WA if the token is malformed or out of range.

Additionally, two new functions are provided.

- `exitWA()` unconditionally exits with a WA verdict.
- `assertWA(bool)` takes a condition and exits with WA if the condition is false.

Under the identical checker, corresponding functions `exitPE()` and `assertPE` are provided. Standard checkers should not use the Presentation Error code, as the builtin `standard` checker does not use this code.

Finally, there is an empty function `errorHook()`. This function is called whenever the provided function would exit with an error. It should be used to do custom handling, such as providing partial points for outputting part of an answer, or outputting `-1` in interactors to signal errors to the user submission.

## Standard Checker/Interactor Design

This section is purely for those interested in the design and inner workings of the standard checker/interactor routines.

The general overview is that `readSpace()` should read non-line whitespace characters, `readNewLine` should read whitespace and expect a line whitespace character, and `readEOF` should read all whitespace and check for EOF. Additionally, any leading whitespace in the input should be trimmed.

There are two major challenges with making a standard checker/interactor design ergonomic:
- Under interactors, it is not acceptable to consume all whitespace in the `readNewLine` method, as the user submission will likely output a single line and then wait for the interactor to send another query. If the interactor naively tried to consume all whitespace, it would block, and the user submission would TLE.
- After reading the end of the input, it's most ergonomic to have the checker read a newline, and then call `readEOF()`, as this is the canonical input format. However, the standard checker allows users to forgo the last newline, and if the `readNewLine()` method expected a newline, we would erroneously return WA.

To solve both of these problems, we employ a lazy whitespace checking scheme. `readSpace()` and `readNewLine()` simply set a flag for `readToken()`. `readToken()` then consumes the whitespace and validates it, before reading the token. Additionally, `readEOF()`, if called after `readNewLine()`, ignores the flag and consumes all whitespace, and then checks for EOF.
