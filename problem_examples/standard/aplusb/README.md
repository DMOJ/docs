# Standard Grading (IO-based)

In this sample, we are using the most common grading system: test input and expected output are stored in a zip file.

The `init.yml` declares the zip file with `archive` in the YAML file. Three test cases are listed in `test_cases`.

Each test case declares its own input and output files, which can be any file in the zip file. The test cases are worth 5, 20, and 75 points respectively, making the total score out of 100.
