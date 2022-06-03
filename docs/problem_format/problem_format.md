# Problem Format
Each problem is stored in its own directory. That directory must contain a file named `init.yml`.

# `init.yml`
The entire file is a YAML object.
It must contain one key, `test_cases`.
`test_cases` can either be a list of test cases, or two regexes to match input and output test cases.
Optionally, but almost always, will there be an `archive` key, which allows the problem data to be stored, compressed, in a `.zip` file, instead of the problem directory as flat files.

# `test_cases`
There are two methods to specify test cases.

The first method is to use a list of YAML associative arrays.
Each element in the list is a YAML associative array (usually written as a keyed branch) that represents a test case. The element contains the key `points`, mapping to an integer specifying the number of points that test case is worth.

If `points: 0` is specified, getting a non-AC verdict on this case will result in the remaining test cases being skipped.
This also applies to batched test cases where `points: 0` is specified.

However, note that these cases will not be automatically run at the beginning, that is if you specify

```yaml
test_cases:
- {in: case1.1.in, out: case1.1.out, points: 100}
- {in: case1.0.in, out: case1.0.out, points: 0}
```

`case1.1` will be graded before `case1.0`, and getting a non-AC verdict on `case1.0` will result in a verdict of `100/100 (WA)`.

The correct configuration would be:

```yaml
test_cases:
- {in: case1.0.in, out: case1.0.out, points: 0}
- {in: case1.1.in, out: case1.1.out, points: 100}
```

## Normal Cases
For normal cases, the test case will contain keys `in` and `out`, mapping to the path for the input and output files, respectively. The path is in the zip file if `archive` is defined, otherwise relative to the problem directory. A normal case will contain the key `points`.

## Batched Cases
The batch will contain the keys `points` and `batched`. `batched` will map to a list of batched cases, where each case contains `in` and `out`.

Optionally, the batch can contain a key `dependencies`. If specified, it should be a list of integers, indicating the one-indexed batch numbers that the batch depends on. This batch will only run if all of the dependent batches passed. The sample `init.yml` below is set up so the last batch only runs if the first two passed.

Note that if short circuit is enabled, it overrides this setting, i.e., a failed case implies immediate judging termination. Pretests (cases or batches with `points: 0`) also still imply immediate judging termination.

Below is a sample `init.yml`:

```yaml
archive: tle16p4.zip
test_cases:
- {points: 0, in: tle16p4.p0.in, out: tle16p4.p0.out}
- {points: 10, in: tle16p4.p1.in, out: tle16p4.p1.out}
- points: 10
  batched:
  - {in: tle16p4.0.in, out: tle16p4.0.out}
  - {in: tle16p4.1.in, out: tle16p4.1.out}
- points: 10
  batched:
  - {in: tle16p4.2.in, out: tle16p4.2.out}
  - {in: tle16p4.3.in, out: tle16p4.3.out}
- points: 10
  batched:
  - {in: tle16p4.4.in, out: tle16p4.4.out}
  - {in: tle16p4.5.in, out: tle16p4.5.out}
  dependencies: [1, 2]
```

As DMOJ's YAML dialect supports dynamic keys, large `init.yml`s can be programmatically generated.
See the sample files for examples.

## Specifying Cases with Regexes
If the test cases follow a similar format, it is possible to specify them with a regex.

The default regex for input files is `^(?=.*?\.in|in).*?(?:(?:^|\W)(?P<batch>\d+)[^\d\s]+)?(?P<case>\d+)[^\d\s]*$`, and the default regex for output files is `^(?=.*?\.out|out).*?(?:(?:^|\W)(?P<batch>\d+)[^\d\s]+)?(?P<case>\d+)[^\d\s]*$`.

Some examples of fie formats they can match are:
```
test.1.in
test-1.in
test-case-1.in

test-1-2.in
test-batch-1-case-2.in
1.2.in
problem-1-case-1-batch-2.in
```

Where the first three are standalone cases (i.e. not in a batch) and the latter
four are batched.

Note that non-batched cases treat their case number as their batch number, i.e
```
1.in
2.1.in
2.2.in
3.in
```
would be sorted in this order by the judge.

These can be overwritten by specifying `input_format` and `output_format` within `test_cases`, respectively.
The points awarded to each test case is given by `case_points`, and it defaults to 1 point per test case/batch.
If there are many cases, a global `points:` can be used to set the points for all cases.
