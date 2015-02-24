# DMOJ Problem Format

Each problem is stored in its own directory. That directory must contain one file, `init.json`.

## `init.json`

The entire file is a JSON Object. It must contain one key, `test_cases`, which maps to a list of test cases.
Optionally, but almost always, will there be an `archive` key, which allows the problem data to be stored,
compressed, in a `.zip` file, instead of the problem directory as flat files.

### `test_cases`

Each element in the `test_cases` list is a JSON object. It contains the key `points`, mapped to an integer specifying the number of points that test case is worth.

#### Normal Cases

For normal cases, the test case JSON object will contain keys `in` and `out`, mapping to the path for the input and output files, respectively. The path is in the zip file if `archive` is defined, otherwise relative to the problem directory.

#### Batched Cases

For batched cases, instead of `in` and `out`, the JSON object will contain `data` key, mapped to a list of objects, each containing `in` and `out`.

### Interactive Problems - `grader`

An `init.json` object can contain a top-level `grader` node, which contains a path to a Python file to be executed as a grader for the problem. The grader has access to the archive specified in `archive`. A `grader` Python script must implement a function that is called by the judge:

```python
def grade(case, process, case_input=None, case_output=None, point_value=0, **kwargs)
```

#### Parameters

* `case` is an integer, the current test case with a zero-based index.
* `process` is a `Popen`-like object with which the grader may interact. `process.stdin` is a file-like object representing the submission's `stdin` handle, and `process.stdout` wraps the process' `stdout`. Input is given to the process through `process.stdin` and output is typically read from `process.stdout`.
* `case_input` is a buffer containing the contents of the `in` file specified for the current case in `init.json`. May be empty, if no case input file was specified.
* `case_output` is a buffer containing the contents of the `out` file specified for the current case in `init.json`. May be empty, if no case output file was specified.
* `point_value` is an integer, the max points that can be awarded for the current testcase.

#### Returns

A `Result` object (`from judge import Result`).
A result object has a `result_flag` field that stores a mask defining the current testcase result code. `proc_output` contains the string that will be displayed in the partial output pane. 

To illustrate, in a problem where the process must a line of echo input, an interactive approach would look like the following.

```python
from judge import Result

def grade(case, process, case_input=None, case_output=None, point_value=0, **kwargs):
  print 'Interacting for case %d' % case
  inp = 'Hello, World!'
  process.stdout.write(inp + '\n')
  process.stdout.flush()
  
  output = process.stdin.readline().strip()
  
  res = Result()
  
  if output == inp:
    res.result_flag = Result.AC
    res.proc_output = 'Correct answer! This will be displayed in the partial output pane.'
  else:
    res.result_flag = Result.WA
    res.proc_output = 'Wrong answer! :('
  
  return res
```

A simple solution to this problem would simply `print raw_input()`.

The associated `init.json` for this problem would look like the following.

```json
{
    "grader": "interactor.py",
    "test_cases": [
        {
            "points": 100
        }
    ]
}
```

Since we use no input or output files (our testcase is hardcoded), we do not need to specify the `archive` or related `in` and `out` fields.


### Custom Checkers - `grader`

A problem with many possible outputs (e.g. not a single possible answer, with score based on accuracy) may benefit from the `checker` field in the `init.json` object. A checker is a Python script that is executed per-case, like an interactive grader, but which runs post-execution - it grades the output of a process but does not interact with it.

A `checker` Python script must implement a function that is called by the judge:

```python
def check(proc_output, judge_output, judge_input, point_value, submission_source, **kwargs)
```

#### Parameters

* `proc_output` contains the submission's output, as a string.
* `judge_output` contains the contents of the specified `out` file.
* `judge_input` contains the contents of the specified `in` file.
* `point_value` is the point value for the current case, as specified for the current testcase.
* `submission_source` contains the submission's source as a string. Useful for code golf problems.

#### Returns

A `CheckerResult` object (`from judge import CheckerResult`). A `CheckerResult` can be instantiated through `CheckerResult(case_passed_bool, points_awarded, partial_output='')`.

### Checkers or Interactors for Computationally-Heavy Problems

Sometimes, problems that require checkers or interactive grading may also be computationally expensive. In such cases, it is often beneficial to move answer checking from the slow Python problem module and into a native language. For maximum interoperability, the judge system requires a Python script to handle raw interaction prompts, but the script itself may use the judge's compilation and execution API to spawn native processes.

To illustrate, a problem that requires a computationally expensive validator can easily be implemented as shown below.

```python
from judge import executors, get_problem_root
import os

# Locate the validator source file. get_problem_root returns the root directory of the problem passed by name.
validator_path = os.path.join(get_problem_root('problem_id'), 'validator.cpp')
# Read all source into memory
validator_source = open(validator_path, 'r')
# Execute the compiler - executors[] is a mapping of all executors by language id
executor = executors['CPP11'].Executor('validator', validator_source.read())
validator_source.close()
# Launch the validator in a sandbox - see below
process = executor.launch(time=60, memory=262144)
```

Here, `validator.cpp` exists in the problem folder root directory, and `process` is the executed validator - a `Popen`-like object. When using `Executor.launch`, you may pass a time limit (in seconds) along with a memory limit (in Kb). Here, the validator may run for a maximum of 60 seconds and use 256mb before being killed. `launch` uses the same sandboxing system as normal submissions; filesystem, network and interprocess access is denied. If you'd like to avoid the overhead of sandboxing for a validator you are sure will execute safely, you may choose to use `Executor.launch_unsafe` - note, however, that you may not specify time or memory limits if the sandbox is inactive.
