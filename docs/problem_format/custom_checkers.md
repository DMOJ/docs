# Custom Checkers

A problem with many possible outputs (e.g. not a single possible answer, with score based on accuracy) may benefit from the `checker` field in the `init.yml` file. A checker is a Python script that is executed per-case post-execution — it grades the output of a process but does not interact with it.

The DMOJ judge already defines several checkers.
These checkers are specified as follows:
```yaml
checker:
  name: <name of checker>
  args: {}
```

Where `args` is a dictionary of arguments to pass to the `check` function.

If you do not need to pass arguments, then you can write
```yaml
checker: <name of checker>
```

## Standard Checker - `standard`

If no `checker` field is specified, then the problem will default to the standard checker.

This checker return `True` if the submission's output and the judge's output are equal, modulo white space.

## Easy Checker  - `easy`

This checker checks if the number of occurrences of each white space delimited tokens are equal.

## Floating Point Checkers - `floats`

The `floats` checker is used when outputs may suffer from floating point epsilon.

`args` can contain a key for `precision`, indicating an epsilon of $10^{-\text{precision}}$.
This value defaults to `6`.

Additionally, `args` can contain a key for `error_mode`. An `error_mode` of `default` will check if the process output is within an absolute or relative of epsilon, an `error_mode` of `relative` will only check for relative error, and an `error_mode` of `absolute` will only check for absolute error.

Finally, all non-numeric outputs will be treated as strings, and will be compared for equality.

## Absolute Floating Point Error Checker - `floatsabs`

`floatsabs` is an alias for `floats` with `error_mode` set to `absolute`.

## Relative Floating Point Error Checker - `floatsrel`

`floatsrel` is an alias for `floats` with `error_mode` set to `relative`.

## Identical Checker - `identical`

The `identical` checker will check if the user output and the judge's output are identical, including white space.

`args` can contain a key for `pe_allowed`, which defaults to `True`.
If `pe_allowed` is true, the checker will give the feedback `Presentation Error, check your whitespace`, if the output is correct modulo white space.
Otherwise, the checker will return `True` if the two outputs are identical, and `CheckerResult(False, 0, feedback=None)` otherwise.

## Line-by-Line Checker - `linecount`

The `linecount` checker is a custom checker primarily used for ECOO problems.

The two arguments you can pass are `feedback`, which defaults to `True`, and `match`, which defaults to `lambda p, j: p.strip() == j.strip()`.

`feedback` indicates if the judge should give return per-line feedback: a check mark for a correct line, and a cross if the line is incorrect.

`match` is the function used to check if two lines of output are considered identical. `p` is the user's output, and `j` is the judge's output.

An example usage would be

```yaml
checker:
  name: linecount
  args:
    match: 'lambda p, j: sorted(p.strip()) == sorted(j.strip())'
```

The checker returns `False`, with no feedback if the user's output contains more lines than the judge's output, otherwise a `CheckerResult` is returned, where the number of points is proportional to the number of correct lines.

## Sorted Checker - `sorted`

The `sorted` checker checks if the submission's output and judge's output are equal modulo their ordering.

The argument you can pass in `args` is `split_on`, which can equal either `lines`, or `whitespace`.

If `split_on` equals `lines`, the checker will return `True` if the two outputs are identical, modulo the ordering of their lines.

### Unordered Checker - `unordered`

This is an alias for `sorted` with `split_on` equal to `whitespace`.

## Custom Checkers

A `checker` Python script must implement a function that is called by the judge:

```python
def check(process_output, judge_output, **kwargs):
```

Variables in global scope will exist throughout the grading process.

`**kwargs` is a directory containing

- `submission_source`: the source code of the submission
- `judge_input`: the judge's input
- `point_value`: the point value of the test case
- `case_position`: the index of the test case
- `batch`: the batched the test case belongs to
- `submission_language`: the language the submission was submitted in
- `binary_data`: a boolean, which is `True` if the data was not normalized to Linux line endings, and `False` otherwise
- `execution_time`: the runtime of the program, in seconds

Additionally, if the `check` method has the flag `run_on_error` set, it will be run against the submission's output, even if it receives an IR/TLE/RTE/MLE verdict.
The only built-in checker that has this flag set is the `linecount` checker.

### Returns

`check` can return either a `CheckerResult` object (`from dmoj.result import CheckerResult`), or a boolean (`case_passed_bool`).
 A `CheckerResult` can be instantiated through `CheckerResult(case_passed_bool, points_awarded, feedback='')`.

# Checkers or Interactors for Computationally-Heavy Problems
Sometimes, problems that require checkers or interactive grading may also be computationally expensive. In such cases, it is often beneficial to move answer checking from the slow Python problem module and into a native language. For maximum interoperability, the judge system requires a Python script to handle raw interaction prompts, but the script itself may use the judge's compilation and execution API to spawn native processes.

To illustrate, a problem that requires a computationally expensive validator can easily be implemented as shown below.

```python
from dmoj.executors import executors
from dmoj.judgeenv get_problem_root
import os

# Locate the validator source file. get_problem_root returns the root directory of the problem passed by name.
validator_path = os.path.join(get_problem_root('problem_id'), 'validator.cpp')
# Read all source into memory
with open(validator_path, 'r') as validator_source:
    # Execute the compiler - executors[] is a mapping of all executors by language id
    executor = executors['CPP11'].Executor('validator', validator_source.read())
# Launch the validator in a sandbox - see below
process = executor.launch(time=60, memory=262144)
```

Here, `validator.cpp` exists in the problem folder root directory, and `process` is the executed validator - a `Popen`-like object. When using `Executor.launch`, you may pass a time limit (in seconds) along with a memory limit (in Kb). Here, the validator may run for a maximum of 60 seconds and use 256mb before being killed. `launch` uses the same sandboxing system as normal submissions; filesystem, network and interprocess access is denied. If you'd like to avoid the overhead of sandboxing for a validator you are sure will execute safely, you may choose to use `Executor.launch_unsafe` - note, however, that you may not specify time or memory limits if the sandbox is inactive.
