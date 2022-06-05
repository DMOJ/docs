# Custom Checkers

A problem with many possible outputs (e.g. not a single possible answer, with score based on accuracy) may benefit from the `checker` field in the `init.yml` file. A checker is a Python script that is executed per-case post-execution — it grades the output of a process but does not interact with it.

The DMOJ judge already defines several checkers.
These checkers are specified as follows:

```yaml
checker:
  name: <name of checker>
  args: {}
```

Where `args` is a dictionary of arguments to pass to the checker's `check` function.
If you do not need to pass arguments, then you can write:

```yaml
checker: <name of checker>
```

## Standard Checker - `standard`

If no `checker` field is specified, then the problem will default to the `standard` checker.

This checker returns `True` if the submission's output and the judge's output are equal, modulo whitespace. Specifically, the submission's output and the judge's output are split line-by-line and tokenized, with lines with no tokens being discarded. For each individual line, the submission's tokens must match exactly with the judge's tokens.

## Easy Checker - `easy`

This checker ignores all whitespace and letter case, then checks if the number of occurrences of each character are equal.

## Floating Point Checkers - `floats`

The `floats` checker is used when outputs may suffer from floating point errors.

`args` can contain a key for `precision`, indicating an epsilon of ~10^{-\text{precision}}~.
This value defaults to `6`.

Additionally, `args` can contain a key for `error_mode`. The supported values are:

- `default`: check if the submission's output is within an absolute or relative error of epsilon.
- `relative`: only check for relative error.
- `absolute`: only check for absolute error.

Finally, all non-numeric outputs will be treated as strings, and will be compared for equality.

### Absolute Floating Point Error Checker - `floatsabs`

`floatsabs` is an alias for `floats` with `error_mode` set to `absolute`.

### Relative Floating Point Error Checker - `floatsrel`

`floatsrel` is an alias for `floats` with `error_mode` set to `relative`.

## Identical Checker - `identical`

The `identical` checker will check if the submission's output and the judge's output are identical, including whitespace.

`args` can contain a key for `pe_allowed`, which defaults to `True`.
If `pe_allowed` is true, the checker will give the feedback `Presentation Error, check your whitespace`, if the output is correct modulo whitespace.
Otherwise, the checker will return `True` if the two outputs are identical, and `CheckerResult(False, 0, feedback=None)` otherwise.

## Line-by-Line Checker - `linecount`

The `linecount` checker is a custom checker primarily used for ECOO problems.

`args` can contain a key for `feedback`, which defaults to `True`.
`feedback` indicates if the judge should give per-line feedback: a `✓` for a correct line, and an `✗` for an incorrect line.

## Sorted Checker - `sorted`

The `sorted` checker checks if the submission's output and judge's output are equal modulo their ordering.

`args` can contain a key for `split_on`, which defaults to `lines`. The supported values for `split_on` are:

- `lines`: the checker will return `True` if the two outputs are identical, modulo the ordering of their lines. Empty lines are ignored.
- `whitespace`: the checker will return `True` if the two outputs are identical, modulo the ordering of their tokens.

### Unordered Checker - `unordered`

This is an alias for `sorted` with `split_on` equal to `whitespace`.

## Custom Checkers

A `checker` Python script must implement a function that is called by the judge:

```python
def check(process_output, judge_output, **kwargs):
```

Variables in global scope will exist throughout the grading process.

`**kwargs` is a dictionary containing:

- `submission_source`: the source code of the submission.
- `judge_input`: the judge's input.
- `point_value`: the point value of the test case.
- `case_position`: the index of the test case.
- `batch`: the batch the test case belongs to, or `0` if the test case is not in a batch.
- `submission_language`: the language the submission was submitted in.
- `binary_data`: a boolean, which is `True` if the data was not normalized to Linux line endings, and `False` otherwise.
- `execution_time`: the runtime of the program, in seconds.
- `problem_id`: the problem code.
- `result`: the submission's preliminary `Result`.

Additionally, if the `check` method has the flag `run_on_error` set (i.e. `check.run_on_error = True`), it will be run on the submission's output, even if the submission received a preliminary IR/TLE/RTE/MLE verdict.
The only built-in checker that has this flag set is the `linecount` checker.

### Returns

`check` can return either a `CheckerResult` object (`from dmoj.result import CheckerResult`), or a boolean (`case_passed_bool`).
A `CheckerResult` can be instantiated through `CheckerResult(case_passed_bool, points_awarded, feedback='')`.

## Native Checkers

Sometimes, problems may require a computationally expensive checker.
In such cases, it is often beneficial to move the checker from the slow Python problem module and into a native language.
This is handled by the `bridged` checker.

The `bridged` checker takes the following arguments:

- `files`: either a filename, or a list of filenames, corresponding to the checker.
- `lang`: the language the checker is written in, using the same conventions as the judge.
- `time_limit`: the time limit allocated to the checker. It defaults to `env['generator_time_limit']`.
- `memory_limit`: the memory limit allocated to the checker. It defaults to `env['generator_memory_limit']`.
- `compiler_time_limit`: the time limit allocated to compiling the checker. It defaults to `env['generator_compiler_limit']`.
- `feedback`: if true, the checker's standard output will be shown as feedback. Defaults to true.
- `flags`: compilation flags to pass to the checker.
- `type`: specifies the arguments to pass to the checker and how to interpret the checker's return code and output.
  - The `default` type passes the arguments in the order `input_file output_file judge_file`. A return code of `0` is an AC, `1` is a WA, and anything else results in an internal error.
  - The `testlib` type passes the arguments in the order `input_file output_file judge_file`. A return code of `0` is an AC, `1` is a WA, `2` is a presentation error, `3` corresponds to an assertion failing,
  and `7`, along with an output to `stderr` of the format `points X` for an integer ~X~ awards ~X~ points. Anything else results in an internal error.
  - The `coci` type behaves similarly to the `testlib` type, but has partial format `partial X/Y`, which awards ~\frac X Y~ of the points.
  - The `peg` type exists for compatibility with the WCIPEG judge.

The files will be compiled and sandboxed, then executed with the arguments `input_file`, `output_file`, and `judge_file`, which are files containing input, submission output, and judge output, respectively.
