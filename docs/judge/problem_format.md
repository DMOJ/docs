Each problem is stored in its own directory. That directory must contain a file named `init.yml`.

# `init.yml`
The entire file is a YAML object. It must contain one key, `test_cases`, which maps to a list of test cases. Optionally, but almost always, will there be an `archive` key, which allows the problem data to be stored, compressed, in a `.zip` file, instead of the problem directory as flat files.

# `test_cases`
Each element in the `test_cases` list is a YAML associative array (usually written as a keyed branch) that represents a test case. The element contains the key `points`, mapping to an integer specifying the number of points that test case is worth.

If `points: 0` is specified, the test case will be considered a pretest. If a submission receives a non-AC verdict on a pretest, the rest of the test cases/batches will not be judged.

## Normal Cases
For normal cases, the test case will contain keys `in` and `out`, mapping to the path for the input and output files, respectively. The path is in the zip file if `archive` is defined, otherwise relative to the problem directory. A normal case will contain the key `points`.

## Batched Cases
The batch will contain the keys `points` and `batched`. `batched` will map to a list of batched cases, where each case contains `in` and `out`.

Below is a sample `init.yml`:

```yaml
archive: tle16p4.zip
test_cases:
- {points: 0, in: tle16p4.p0.in, out: tle16p4.p0.out}
- {points: 10, in: tle16p4.p1.in, out: tle16p4.p1.out}
- points: 10
  batched:
  - {in: tle16p4.0.in, out: tle16p4.0.out}
  - {in: tle16p4.1.in, out: tle16p4.1.out}
- points: 10
  batched:
  - {in: tle16p4.2.in, out: tle16p4.2.out}
  - {in: tle16p4.3.in, out: tle16p4.3.out}
```

# Interactive Problems - `grader`
An `init.yml` object can contain a top-level `grader` node, which contains a path to a Python file to be executed as a grader for the problem. The grader has access to the archive specified in `archive`. A `grader` Python script must implement a function that is called by the judge:

```python
def grade(self, case):
```

## Parameters
`case` is a `TestCase` object.
- `case.position` is an integer, the current test case with a zero-based index.
- `process` is a `Popen`-like object with which the grader may interact. `process.stdin` is a file-like object representing the submission's `stdin` handle, and `process.stdout` wraps the process' `stdout`. Input is given to the process through `process.stdin` and output is typically read from `process.stdout`. (No it's not)
- `case.input_data()` is a buffer containing the contents of the `in` file specified for the current case in `init.yml`. May be empty, if no case input file was specified.
- `case.output_data()` is a buffer containing the contents of the `out` file specified for the current case in `init.yml`. May be empty, if no case output file was specified.
- `case.points` is an integer, the max points that can be awarded for the current testcase.

## Returns
A `Result` object (`from dmoj.result import Result`). A result object has a `result_flag` field that stores a mask defining the current testcase result code. `proc_output` contains the string that will be displayed in the partial output pane.

To illustrate, in a problem where the process must a line of echo input, an interactive approach would look like the following.

```python
from dmoj.result import Result

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

The associated `init.yml` for this problem would look like the following.

```yaml
grader: interactor.py
unbuffered: true
test_cases:
- points: 100
```

Since we use no input or output files (our testcase is hardcoded), we do not need to specify the `archive` or related `in` and `out` fields.

In this example, it's important to note the `unbuffered` node. If set to `true`, the judge will use a pseudoterminal device for a submission's input and output pipes. Since ptys are not buffered by design, setting `unbuffered` to `true` removes the need for user submissions to `flush()` their output stream to guarantee that the `grader` receives their response. **The `unbuffered` node is not exclusive to interactive grading: it may be specified regardless of judging mode.**

# Custom Checkers - `checker`
A problem with many possible outputs (e.g. not a single possible answer, with score based on accuracy) may benefit from the `checker` field in the `init.yml` object. A checker is a Python script that is executed per-case, like an interactive grader, but which runs post-execution &mdash; it grades the output of a process but does not interact with it.

A `checker` Python script must implement a function that is called by the judge:

```python
def check(proc_output, judge_output, **kwargs):
```

Variables in global scope will exist throughout the grading process.

## Parameters
- `proc_output` contains the submission's output, as a string.
- `judge_output` contains the contents of the specified `out` file.
- `judge_input` contains the contents of the specified `in` file.
- `point_value` is the point value for the current case, as specified for the current testcase.
- `submission_source` contains the submission's source as a string. Useful for code golf problems.

`**kwargs` contains `judge_input`, `point_value`, and `submission_source`.

## Returns
A `CheckerResult` object (`from dmoj.result import CheckerResult`). A `CheckerResult` can be instantiated through `CheckerResult(case_passed_bool, points_awarded, feedback='')`.

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

# Function Signature Grading (IOI-style)
Signature grading is used for problems where users should implement an online algorithm or interact with the grader directly without the need for traditional input and output routines. This is commonly seen in competitions such as the IOI, where all input is passed through function arguments and output is replaced with return values or directly modifying specifically allocated memory for the computed answer. Currently, C and C++ are supported for this mode. `signature_grader` should be a top-level node that contains `entry` and `header`. `entry` is a C or C++ file that contains the `main` function. It should read input from `stdin`, call the user's implemented functions specified in `header`, and write output to `stdout`. You may specify a custom `checker` to interpret the `entry`'s output. If no custom checker is specified, it will be compared to the output file using the default checker.

The user's submission will be automatically modified to include the file `header`, and the symbol `main` is redefined as `main_GUID` where `GUID` is a randomly generated GUID. This is so users testing their program do not have to manually remove their `main` function before submissions; it does not protect against the preprocessor directive `#undef main`.

The global variables in the `entry` should be declared static to prevent name collisions. Optimally, `header` should have an include guard, in case it contains something other than function prototypes.

Note that both the `entry` and the user's code will be compiled with C++11 regardless of whether C, C++, C++0x, or C++11 is selected as the language. Issues that may arise from this are naming variables like `class` in a C program, which otherwise would be allowed.

An example of the `init.yml`:

```yaml
signature_grader: {entry: handler.c, header: header.h}
test_cases:
- {in: siggrade.1.in, out: siggrade.1.out, point: 50}
- {in: siggrade.2.in, out: siggrade.2.out, point: 50}
```

An example of the `entry` file:

```c
#include "header.h"
#include <stdio.h>
#include <stdbool.h>

static int n;

int main()
{
    scanf("%d", &n);
    bool valid = is_valid(n); // Defined in header
    printf(valid ? "correct" : "wrong");
    return 0;
}
```

An example of the `header` file:

```c
#ifndef _GRADER_HEADER_INCLUDED
#define _GRADER_HEADER_INCLUDED
#include <stdbool.h>
bool is_valid(int);
#endif
```
