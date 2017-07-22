# Interactive Grading

Some problems will need to interact with the judge while running. As such, the judge allows for the use of interactive graders, with predefined functions to assist in this interaction.

The interactive grader, like any custom grader, will be invoked if there is a `custom_judge` node in the `init.yml`. As with all custom judges, you must define a `Grader` class, and it should extend the class `InteractiveGrader` from `dmoj.judge.graders.interactive`. Within the `Grader` class, the method `interact` should be defined, with the arguments `self, case, interactor`. `case` is a `InvocationCase` object, from `dmoj.judge`, and `interactor` is an `Interactor` object from `dmoj.judge.graders.interactive`. To interact with the submission, the following methods are provided:

- `read( )`: Reads all of the output
- `readln( strip_newline = True )`: Reads a single line of the output
  - `strip_newline`: If true, strips any trailing newline
- `readtoken( delim = None )`: Returns the first token of the output split by `delim`
- `readint( lo = float('-inf'), hi = float('inf'), delim = None )`: Reads the next integer token, as split by `delim`. Can preform an optional range check with `lo` and `hi`.
- `readfloat( lo = float('-inf'), hi = float('inf'), delim = None )`: Reads the next floating-point token, as split by `delim`. Can preform an optional range check with `lo` and `hi`.
- `write( val )`: Casts `val` to a string and writes it to the submission's input stream, which is then flushed.
- `writeln( val)`: Similar to `write( val )`, but appends a newline afterwards
- `close( )`: Closes the submission's input, output, and error streams.

The `interact` method should return a `bool`: either `True` of `False`

If the checker needs to terminate before the time limit due to a `Wrong Answer` verdict, it can raise a `WrongAnswer` exception, from `dmoj.judge.graders.interactive`. This will also terminate the grader.
