# Contest Formats

The DMOJ ships with 6 contest formats by default: Default, IOI, Codechef IOI Ranklist (henceforth shortened to simply Legacy IOI), ECOO, AtCoder, and ICPC.

## Default

The `Default` contest format is what all contests ran on pre-April 23, 2019.

The score is the sum of the highest-scoring submission on each problem, and ties are broken by adding the time of the last submission to each problem with a non-zero maximum score.

Note that *any* submission will increase time penalty, not just score-changing submissions.

There are no additional options that can be configured for this contest format.

## IOI

The `IOI` contest format emulates the scoring used by IOI.

The score is equal to the sum of the final score on each problem, where the final score for a problem is the maximum score for each subtask across all submissions, and by default, ties are not broken.
For example, consider a contestant that makes two submissions on a task with two subtasks. The first gets 30 points on the first subtask and 10 points on the second subtask. The second gets 0 points on the first subtask and 40 points on the second subtask. The final score for this problem will be 70.

The `cumtime` option can be set to `true` within the JSON configuration. This will break ties by summing the submission times of the first submissions that pass each subtask.

## Legacy IOI

The `Legacy IOI` contest format emulates the scoring used by Codechef's IOI Ranklist.

The score is equal to the sum of the highest-scoring submission on each problem, and by default, ties are not broken.

The `cumtime` option can be set to `true` within the JSON configuration. This will break ties by summing the submission times of the most recent total score-changing submissions.

## ECOO

The `ECOO` contest format is based on the scoring system used by the ECOO contest.

The score is equal to the sum of scores of the last submission to each problem.
By default, ties are not broken, however, setting `cumtime` to `true` will sum the times of the last submission to each problem, and use that for tiebreaking.

The `first_ac_bonus` field, as suggested by the name, will add the specified number of points to a problem's score if it is solved on the first attempt, excluding compile errors and internal errors.
This field defaults to 10.

The `time_bonus` field awards a bonus for solving problems faster.
The field is in minutes, and for each such interval of time before the contest ends, any submission with a non-zero score will have a bonus point added. This field defaults to 5.
Note that specifying 0 will disable this bonus.

For example, say a submission with a score of 50/100 is submitted 23 minutes before the contest ends. <math><mo>&lfloor;</mo><mfrac><mn>23</mn><mn>5</mn></mfrac><mo>&rfloor;</mo><mo>=</mo><mn>4</mn></math>, so 4 bonus points are awarded, giving a total score of <math><mn>50</mn><mo>+</mo><mn>4</mn><mo>=</mo><mn>54</mn></math> for that problem.

## AtCoder

The `AtCoder` contest format is based on the contest format used by AtCoder.

The score is equal to the sum of the highest-scoring submission on each problem, and ties are broken based on the time of the last score-changing submission plus the penalty.

The penalty is specified by the `penalty` field, and defaults to 5 minutes.
The penalty is equal to the total number of incorrect submissions prior to the highest-scoring submission on each solved problem, multiplied by the specified value, in minutes, and is added to the cumulative time.

## ICPC

The `ICPC` contest format is based on the contest format used by the ICPC.

The score is equal to the number of problems solved, and ties are broken firstly based on the sum of the elapsed time that a correct submission was submitted to each problem plus the penalty, and secondly based on the time of the last score-changing submission.

The penalty is specified by the `penalty` field, and defaults to 20 minutes.
The penalty is equal to the number of incorrect submissions prior to the first correct submission, multiplied by the specified value, in minutes, and is added to the cumulative time.
Note that the time penalty is applied to all problems with a non-zero score (this format will not automatically disable partial points).
