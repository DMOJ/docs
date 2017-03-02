# Automatically-Generating Input/Output Data Large Input/Output Data

For some problems, the test data takes up a very large amount of space, and it may not be feasible to store all the files in full. As such, the judge allows for the use of generators, to generate large data on the fly.

The judge will invoke a generator when it sees a `generator` node in the `init.yml`. The generator will have acess to the input files specified in the `test_cases` node. The generated input data is printed through `stdout` and the generated output data is printed through `stderr`.
