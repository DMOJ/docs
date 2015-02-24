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
