# C++ Problem Setting Templates - `cpp_psetting_templates`

There are three C++ input-handling templates provided for aiding problem setters. They are as follows:

- [Validator Template](https://github.com/DMOJ/docs/blob/master/sample_files/problem_setting/validator.cpp)
- [Identical Checker/Interactor Template](https://github.com/DMOJ/docs/blob/master/sample_files/problem_setting/identical_checker_interactor.cpp)
- [Standard Checker/Interactor Template](https://github.com/DMOJ/docs/blob/master/sample_files/problem_setting/standard_checker_interactor.cpp)

Examples of their use are as follows:

- A validator for <https://dmoj.ca/problem/aplusb> is [here](https://github.com/DMOJ/docs/blob/master/sample_files/problem_setting/examples/validator.cpp).
- An identical-style checker for <https://dmoj.ca/problem/seq3> is [here](https://github.com/DMOJ/docs/blob/master/sample_files/problem_setting/examples/identical_checker.cpp).
- A standard-style interactor for <https://dmoj.ca/problem/seed2> is [here](https://github.com/DMOJ/docs/blob/master/sample_files/problem_setting/examples/standard_interactor.cpp).

## Validator

This is a template for validating the input data of problems. It aims to be simple and of course, correct. It contains seven functions. The first three are whitespace functions:

- `void readSpace()` expects a space at the current position in the input, and aborts the program if there is not a space.
- `void readNewLine()` expects a newline at the current position in the input.
- `void readEOF()` expects the input file to end immediately at the current position.

The remaining four are for actual content:

- `std::string readToken(char min_char = 0, char max_char = 127)` returns the next token in the input stream. A token is defined as a whitespace-separated string. If the next character in the input is a whitespace character, this function aborts the program. The optional arguments `min_char` and `max_char` can be used to enforce a range on the characters in the token. For instance, `readToken('a', 'z')` reads a lowercase string of english letters.
- `std::string readLine(char min_char = 0, char max_char = 127)` returns the next line in the input stream. Specifically, it reads until it encounters a `\n`, and discards it (the newline is not part of the returned string). `min_char` and `max_char` are the same as for `readToken`. If `readLine` encounters an EOF, it fails.
- `long long readInt(long long lo, long long hi)` calls `readToken()` and parses the token as an integer. It aborts on overflow, malformed integers, and if the resultant integer is not in the range [lo, hi], inclusive. Leading zeroes and `-0` are not accepted. Please note that reading `unsigned long long` types will not work properly with this function, as it returns a `long long`.
- `long double readFloat(long double lo, long double hi, long double eps = 1e-9)` calls `readToken()` and parses the token as a float. It aborts on overflow, malformed floats, and if the resultant float is not in the range [lo, hi], inclusive, using the provided epsilon to perform the comparison. Scientific notation and NaNs are not accepted, nor are leading zeroes. `-0` is allowed. Trailing zeroes in the decimal portion are also permitted.
- `std::vector<T> readIntArray(size_t N, long long lo, long long hi)` parses the next space-separated N integers into an array, and then reads a final newline. It must be given a template argument, which is the type of the array elements. For example, `readIntArray<int>(5, 1, 10)` reads five space-separated integers into a `std::vector<int>`, where each integer is in the range [1, 10], inclusive. Because this method uses `readInt` internally, it does not handle `unsigned long long` properly.

A small caveat: `readToken` and `readLine` will throw if the string exceeds 10 million characters.

`readFloat()` and `readLine()` will likely be of no use for many validators, and can be safely deleted. Similarly, `readIntArray` can be deleted if unneeded.

## Checkers/Interactors

The next pair of templates are for checkers/interactors. The difference is the type of whitespace handling: the identical checker/interactor expects whitespace to match exactly. The standard checker/interactor handles whitespace like the `standard` checker.

The checkers and interactors are designed for the `coci` bridged checker/interactor type. However, updating the codes used and the order of command line parameters to work with other types should not be challenging.

Both files can be used for either checkers/interactors, with the following caveat: interactors MUST close `stdout` BEFORE calling `readEOF()`, so that the user process can terminate in case it _also_ expects an EOF. Checker stdout is used for feedback displayed to the user, and as such `stdout` should not be closed in this case. Validators also do not need to worry about this - only interactors do, and they should only call `readEOF()` once they have finished communicating with the user, to clean up and assert that the user didn't send any trailing data.

### Identical Checker/Interactor

This template expects whitespace to match exactly, just as in the validator. The template is simpler, but it is less forgiving to contestants.

The same functions are provided, but have slightly different behaviour:

- `readSpace(), readNewLine(), readEOF()`: These return Presentation Error if the check fails.
- `readToken()`: This exits with a Presentation Error if the token is empty, and WA if any character is not in range.
- `readLine()`: This exits with a Presentation Error if an EOF is encountered, and WA if any character is not in range.
- `readInt(), readIntArray(), readFloat()`: These exit with WA on failure.

Four new functions are provided:

- `exitWA(), exitPE()`: These functions exit immediately with the specified code.
- `assertWA(bool), assertPE(bool)`: These functions exit if the provided condition is false. Useful as a replacement for `assert()`.

One new namespace is provided. The `CheckerCodes` namespace contains the constants `AC, WA, PE`, and `PARTIAL`. It is recommended to use them in `main` to return a verdict. For instance, to return an AC verdict, use `return CheckerCodes::AC;`

Finally, there is an empty function `errorHook()`, which is called whenever any of the functions in the API would exit with an error. This can be used to implement functionality such as partial points, or outputting `-1` to signal errors in interactors.

### Standard Checker/Interactor

This template is much more complex, but is more lenient for submissions. It matches the whitespace of the `standard` builtin checker.

The behaviour of the non-whitespace functions are the same as for the identical checker template, with the following caveats:

- `exitPE()` and `assertPE()` don't exist, since the builtin `standard` checker never uses the Presentation Error code. Checker writers are discouraged from using it.
- `readToken()` always exits with WA on failure.
- `readLine()` doesn't exist, since the way it should process whitespace is not clear for the standard checker. Checker writers reaching for this method should consider the identical checker template instead, or rethink their output format entirely.
- `CheckerCodes` doesn't contain the constant for `PE`.

The remaining functions have the same behaviour as with the identical checker.

#### Whitespace functions

These functions all exit with `WA` on failure instead of `PE`, for the reasons described above.

The code maintains a flag of the type of whitespace it expects, one of `NONE, SPACE, NEWLINE, ALL`, initially ALL. `readSpace()` sets the flag to `SPACE` and `readNewLine()` sets it to `NEWLINE`.

`readToken()` sets the flag to `NONE` and consumes all the whitespace, and exits with WA if either:

- The current flag is `SPACE` and a newline was found.
- The current flag is `NEWLINE` and no newline was found.

It never exits with WA if the flag is `ALL`. It causes an IE if the flag is `NONE`, which only happens when `readToken()` is called twice in a row without an intervening whitespace function. Note that `readInt()` and `readFloat()` call `readToken()` internally.

`readEOF()` sets the flag to `ALL`, consumes all whitespace, and then exits with WA if any character remains in the stream.

No two whitespace functions can be called back to back, except for `readNewLine()` followed by `readEOF()`. The reason for the exception is that the canonical form for output should have a trailing newline, and so this exception allows checkers writers to think in terms of the canonical form of the output. Also, it allows calling `readIntArray()` right before `readEOF()`, since `readIntArray()` internally calls `readNewLine()`.

Note that this scheme is lazy. This is intentional; it allows the same code to be used by interactors without difficulty.
