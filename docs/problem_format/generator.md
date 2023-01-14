# Generators

When there is a large amount of test data, a generator file can be used instead of input and output files.
A generator is a program that takes command line arguments for each case, and outputs the input and output data for each case.

## The `generator` node

The `generator` node can contain either:

- a single value, the name of the generator file.
- an array, in which case the first element is the source file (in either C or C++), and the remaining elements are auxiliary files, such as header files.
- a YAML associative array that can contain the following keys:
  - `source`: either a single string: the name of generator file, or an array, in which case the first file is the generator source, and the remaining files are auxiliary files (e.g. header files).
  - `language`: the language the generator is written in. If empty, the judge tries to infer the language from `source`.
  - `flags`: additional flags to pass to the compiler. It defaults to `[]`.
  - `compiler_time_limit`: the compiler time limit for the generator. It defaults to `env.compiler_time_limit`, as defined in `dmoj/judgeenv.py`. It is recommended to set this value to 60 seconds if using `testlib.h`.
  - `time_limit`: the time limit allocated to the generator. It defaults to `env.time_limit`, as defined in `dmoj/judgeenv.py`.
  - `memory_limit`: the memory limit allocated to the generator. It defaults to `env.memory_limit`, as defined in `dmoj/judgeenv.py`.

Additionally, it is possible to specify this node in each test case, so several generators can be used for a single problem.

## Generator arguments

The `generator_args` node contains a list of arguments that will be cast to a Python `str`, then passed to the compiled generator. `generator_args` can be specified as a top-level node, or more commonly, as a key in a test case node. For example, consider:

```yaml
generator: gen.cpp
test_cases:
- {generator_args: [false, 123, "a b\nc"], points: 10}
- {points: 20}
```

For the first test case, the generator will receive 4 arguments: `"_aux_file"`, `"False"`, `"123"`, `"a b\nc"`.
For the second test case, `generator_args` defaults to `[]`, so the generator will receive 1 argument: `"_aux_file"`.

The generator should output the test case's input data to `stdout`, and the output data to `stderr`.

If a test case already has an input file **and** output file defined by the `in` and `out` keys, the generator will not be run for that test case.
