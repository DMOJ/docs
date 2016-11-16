# Batched Grading
In this sample, we are building off of standard grading: test input and expected output are stored in a zip file. However, this time they are _batched_, so the judge will test each batch, and assign points if the submission passes _all_ the test cases in the batch.

The `init.yml` is similar to the one used in Standard Grading, however, there is a key difference: `batched` indicates that the test cases are to be considered a _batched test case_, and will be assigned the points that is given.

```
- batched:
  - {in: hungry.1a.in, out: hungry.1a.out}
  - {in: hungry.1b.in, out: hungry.1b.out}
  - {in: hungry.1c.in, out: hungry.1c.out}
  points: 5
```
