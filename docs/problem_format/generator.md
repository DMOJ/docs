# Generators - `generator`

When there is a large amount of test data, a generator file can be used instead of input and output files.
A generator is a program that takes command line arguments for each case, and outputs the input and output data for each case.

## The `generator` Node

The `generator` node can contain either a single value: the name of the generator file; an array, in which case the first entry is the source file (in either C or C++), and the remaining files are auxiliary files, such as header files; or a YAML associative array.

Additionally, it is possible to specify this node in each test case, so several generators can be used for a single problem.

If the `generator` node is a YAML associative array, then it can contain the following keys:

- `source`: either a single string: the name of generator file, or an array, in which case the first file is the generator source, and the remaining files are auxiliary files (e.g. header files).
- `compiler_time_limit`: the compiler time limit for the generator. It defaults to `env.compiler_time_limit`, as defined in `dmoj/judgeenv.py`. It is recommended to set this value to 60 seconds if using `testlib.h`.
- `time_limit`: the time limit allocated to the generator. It defaults to `env.time_limit`, as defined in `dmoj/judgeenv.py`.
- `memory_limit`: the memory limit allocated to the generator. It defaults to `env.memory_limit`, as defined in `dmoj/judgeenv.py`.
- `language`: the language the generator is written in. It defaults to the most recent version of C++/C installed on the judge.

## The Generator File

The compiled generator will be passed `'\n'.join(map(str, generator_args))` as command line arguments for each test case.

The generator should output the test case's input data to `stdout`, and the output data to `stderr`.

If the test case already has an input file defined by the `in` key, the generator will not be run.
