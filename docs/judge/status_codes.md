This page lists all status codes encountered on the DMOJ and their description. It should be noted that it is possible for a test case to be given multiple status codes (indeed, this is usually the case for non-AC verdicts), in which case the one with the highest priority will be displayed. This page lists status codes in order of increasing priority.

## AC - Accepted
Your program passed testing! In some cases, this may be accompanied with additional feedback from the grader.

## WA - Wrong Answer
Your program did not crash while executing, but the output it produced was wrong. As for AC, this may be accompanied with additional feedback stating what you did wrong.

## IR - Invalid Return
Your program returned with a nonzero exit code (if you're not using a native language like C++, it crashed). For languages like Python or Java, this will typically be accompanied with the name of the exception your program threw, e.g., `NameError` or `java.lang.NullPointerException`, respectively.

## RTE - Runtime Exception
Your program caused a runtime exception to occur. This will only occur for native languages like C or C++. DMOJ maps many common RTEs to more useful descriptions, described below.
<table class="table">
<thead>
<tr>
  <th style="min-width:17em">Feedback</th>
  <th>Description</th>
</tr>
</thead>
<tbody>
<tr>
  <td><code>segmentation fault</code></td>
  <td style="text-align:left">Your program was killed by SIGSEGV. Generally, this means you ran out of memory, but among other things it can also mean that you are accessing arrays out of bounds.</td>
</tr>
<tr>
  <td><code>floating point exception</code></td>
  <td style="text-align:left">Your program performed a bad arithmetic operation, such as division by zero.</td>
</tr>
<tr>
  <td><code>killed</code></td>
  <td style="text-align:left">Your program was killed by the runtime for some reason (which we don't know).</td>
</tr>
<tr>
  <td><code>opening files is not allowed</code></td>
  <td style="text-align:left">Unless a problem specifically says you can, you may not open files — doing so will get you this message.</td>
</tr>
<tr>
  <td><code>{} syscall disallowed</code></td>
  <td style="text-align:left">Unless you are doing something of a dubious nature, you should never see this message. If you do, please <a href="https://github.com/DMOJ/judge/issues">file a ticket</a> so we can get it sorted out.</td>
</tr>
<tr>
  <td><code><a href="http://www.cplusplus.com/reference/new/bad_alloc/">std::bad_alloc</a></code></td>
  <td style="text-align:left"><code>new</code> failed to allocate enough memory. All C++ exceptions are mapped to feedback like this, though this is the most common exception you're likely to encounter.</td>
</tr>
<tr>
  <td><code>failed initializing</code></td>
  <td style="text-align:left">Your program uses too much data defined in global scope for it to fit inside the memory constraints at startup. A typical example is code like `int arr[10000][10000];` on a problem with 64mb — the aforementioned array will take 381mb, far above the allowed limit.</td>
</tr>
</tbody>
</table>

## OLE - Output Limit Exceeded
Your program outputted too much data to `stdout`, typically over 256mb (though some problems may have custom — generally larger — constraints).

## MLE - Memory Limit Exceeded
Your program ran out of memory. Sometimes, this might manifest itself as an RTE with `segmentation fault` or `std::bad_alloc`.

## TLE - Time Limit Exceeded
Your program took too long to execute.

## IE - Internal Error

If you see this, it means either the judge encountered an error or the problemsetter's configuration is incorrect. Administrators get notified of every internal error by email, and as such there is no need to do anything else — IEs will typically be resolved within 24 hours.
