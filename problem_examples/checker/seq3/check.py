from dmoj.result import CheckerResult


def check(process_output, judge_output, judge_input, point_value, **kwargs):
    # process_output contains the user's output
    # Firstly, we split the user's output into lines
    process_lines = filter(None, process_output.split('\n'))

    # We check that they only output 1 line of output
    if len(process_lines) != 1:
        # They did not follow output specifications
        # Thus they get a WA verdict, 0 points, and a message telling them their output is malformed
        return CheckerResult(False, 0, "Expected 1 line of output, got %d" % len(process_lines))

    # Next we check that they printed only integers
    try:
        process_tokens = map(int, process_lines[0].split())
    except (TypeError, ValueError):
        # We again tell them they did not follow output specifications
        return CheckerResult(False, 0, "Sequence contains non-numeric values!")

    # We check that the sequence is of the correct length
    # Firstly, we split the input into lines
    input_lines = filter(None, judge_input.split('\n'))
    # Then we parse N and K from the first line of input
    N, K = map(int, input_lines[0].split())

    if len(process_tokens) != N:
        # We inform them that they did not output N numbers
        return CheckerResult(False, 0, "Sequence's length is incorrect!")
    # We check all numbers in the sequence are non-negative
    if any(process_token < 0 for process_token in process_tokens):
        # We again tell them they did not follow output specifications
        return CheckerResult(False, 0, "Sequence contains negative numbers!")

    # We check that the sequence sums to K
    conditions_met = 0

    if sum(process_tokens) == K:
        conditions_met += 1
    else:
        return CheckerResult(False, 0, "Sequence's sum is incorrect!")
    # The minimal possible product is 0, so we check if there exists a 0 in the sequence
    if not all(process_tokens):
        conditions_met += 1

    # Finally, return the verdict
    return CheckerResult(True, point_value * conditions_met / 2.0)
