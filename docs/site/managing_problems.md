# Managing Problems Through the Site Interface

The DMOJ comes with an online interface for creating and editing problem statements as well as data.
This guide is intended as an introduction to using these features for creating your own problems.

## Adding a Problem

To start, head to [/admin/](https://dmoj.ca/admin/) and use your credentials to log in.
Once there, click the _Add_ button on the _Problems_ menu.

![](https://i.imgur.com/RFPQaUi.png)

This will open up the main problem editor. To start, you should provide a problem code (must be unique site-wide),
and a title for your problem. **Make sure to mark yourself as an author**, as otherwise you will be locked out of your problem.

![](https://i.imgur.com/bPlNZUR.png)

Here you may edit your problem statement. The DMOJ features a rich Markdown-based syntax, with custom extensions for LaTeX-based display
math, and MathJax-based inline math. See [this template](https://raw.githubusercontent.com/DMOJ/docs/master/sample_files/problem_markdown_example.md.txt) for a full feature example (you may copy/paste
its content into your editor).

There are many options controlling your problem described in the editor, that you may use to customize the execution of your problem.

Once you are done preparing your statement, click the _Save_ button, then scroll up to the top of the page and
click the _View on site_ button.

![](https://i.imgur.com/ZgO5xcY.png)

## Editing Test Data

Internally, the DMOJ uses a YAML-based format for describing problem data, which you may read about [here](/problem_format/problem_format.md#problem-format).
The site provides an interface for managing problem data, removing the need to drop down to YAML configuration for most problems.

On the problem page, click the _Edit test data_ link to open up the test data editor.

![](https://i.imgur.com/eDWEEJk.png)

In the editor, you must first upload a zip archive containing the input/output data used for your problem. The typical convention
is to use text files ending with a `.in` extension for input files, and `.out` for output files, with the
test case number embedded in the filename.

For example, for a problem with a code of `testp1`, the first test case would be named `testp1.1.in`,
with an output file `testp1.1.out`.

Using this format is **not** necessary &mdash; the judge will accept any filenames &mdash; but using it will allow the test data
editor to autocomplete paths, saving some manual input.

![](https://i.imgur.com/w5ytsgi.png)

There are many other options, but for most problems, only one more is necessary: the per-case point value. If partial points
are enabled in the problem statement editor, then a user's score on the problem is equal to the
sum of the point values of the cases they got right divided by the total sum of case point values, multiplied by the number of
points the problem is worth.

For example, if your problem is worth 100 points and has 3 cases weighted 1/2/7 points respectively, a user who gets the first
two cases correct and then fails the last one will have a score of (1 + 2) / (1 + 2 + 7) &times; 100 = 30 points, out of 100.

## Submitting to a Problem

After you have created your test data, you should head back to the problem and click the _Submit solution_ button. If at any point in
time you need to update your data, you may do so from the test data editor, and it will update automatically.
