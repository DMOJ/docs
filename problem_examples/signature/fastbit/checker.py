from dmoj.result import CheckerResult
from dmoj.utils.unicode import utf8text

def check(process_output, judge_output, judge_input, point_value, submission_source, **kwargs):
    result = utf8text(process_output.rstrip()).split('\n')
    if len(result) != 1 or result[0].strip() != 'Correct.':
        return CheckerResult(False, 0)
    return CheckerResult(True, int(point_value) / (1 if len(submission_source) < 560 else 2))
