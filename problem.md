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
A result object has a `result_flag` field that stores a mask defining the current testcase result code. `proc_output` contains a list of lines that will be displayed in the partial output pane. 

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
    res.proc_output = ['Correct answer! This will be displayed in the partial output pane.']
  else:
    res.result_flag = Result.WA
    res.proc_output = ['Wrong answer! :(']
  
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
