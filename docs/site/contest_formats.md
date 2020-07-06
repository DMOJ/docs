# Contest Formats

The DMOJ ships with 4 contest formats by default: Default, Codechef IOI Ranklist (henceforth shortened to simply IOI), ECOO, and AtCoder.

## Default Contest Format

The `Default` contest format is what all contests ran on pre-April 23, 2019.

The score is the sum of the maximum scores on each problem, and ties are broken by adding the time of the last submission to each problem with a non-zero maximum score.

Note that *any* submission will increase time penalty, not just score-changing submissions.

There are no additional options that can be configured for this contest format.

## IOI Contest Format

The `IOI` contest format emulates the scoring used by Codechef's IOI Ranklist.

The score is equal to the sum of the maximum scores on each problem, and by default, ties are not broken.

The `cumtime` option can be set to `true` within the JSON configuration. This will break ties by summing the submissions times of the most recent total score-changing submissions.

## ECOO Contest Format

The `ECOO` contest format is based on the scoring system used by the ECOO contest.

The score is equal to the sum of scores of the last submissions to each problem.
By default, ties are not broken, however, setting `cumtime` to `true` will sum the times of the last submission to each problem, and use that to tiebreak.

The `first_ac_bonus` field, as suggested by the name, will add the specified number of points to a problem's score if it is solved on the first attempt, excluding compile errors and internal errors.
This field defaults to `10`.

The `time_bonus` field awards a bonus for solving problems faster.
The field is in minutes, and for each such interval of time before the contest ends, any submission with a non-zero score will have a bonus added. This field defaults to `5`.  
Note that specifying `0` will disable this bonus.  
For example, say a submission with a score of `50/100` is submitted 23 minutes before the contest ends. $\left\lfloor\frac{23}{5}\right\rfloor = 4$, so 4 bonus points are awarded, giving a total score of $50 + 4 = 54$ for that problem.

## AtCoder Contest Format

The `AtCoder` contest format is based on the contest format used by AtCoder. As such, your score equals the sum of your highest-scoring submission on each problem, and ties are broken based on the time of your last score-changing submission plus your penalty.

The penalty is specified by the `penalty` field, and defaults to 5 minutes.
The penalty is equal to the total number of incorrect submissions prior to the highest scoring submission on each solved problem, multiplied by the specified value, in minutes, and is added to the cumulative time.
