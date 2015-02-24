# DMOJ Problem Format

Each problem is stored in its own directory. That directory must contain one file, `init.json`.

## `init.json`

The entire file is a JSON Object. It must contain one key, `test_cases`, which maps to a list of test cases.
Optionally, but almost always, will there be an `archive` key, which allows the problem data to be stored,
compressed, in a `.zip` file, instead of the problem directory as flat files.

