This page lists all status codes encountered on the DMOJ and their description. It should be noted that it is possible for a test case to be given multiple status codes (indeed, this is usually the case for non-AC verdicts), in which case the one with the highest priority will be displayed. This page lists status codes in order of increasing priority.

## AC - Accepted
Your program passed testing! In some cases, this may be accompanied with additional feedback from the grader.

## WA - Wrong Answer
Your program did not crash while executing, but the output it produced was wrong. As for AC, this may be accompanied with additional feedback stating what you did wrong.

## IR - Invalid Return
Your program returned with a nonzero exit code (if you're not using a native language like C++, it crashed). For languages like Python or Java, this will typically be accompanied with the name of the exception your program threw, e.g., `NameError` or `java.lang.NullPointerException`, respectively.

## RTE - Runtime Exception
Your program caused a runtime exception to occur. This will only occur for native languages like C or C++. DMOJ maps many common RTEs to more useful descriptions, described below.

| Feedback | Description |
|----------|-------------|
| `segmentation fault`| Your program was killed by SIGSEGV. Generally, this means you ran out of memory, but it can also mean that you are accessing arrays out of bounds, in some cases. |
| `floating point exception` | Your program performed a bad arithmetic operation, such as division by zero. |
| `killed` | Your program was killed by the runtime for some reason (we don’t know). |
| `opening files is not allowed` | Unless a problem specifically says you can, you may not open files — doing so will get you this message. |
| `{} syscall disallowed` | Unless you are doing something of a dubious nature, you should never see this message. If you do, please [file a ticket] so we can get it sorted out. |
| `std::bad_alloc` | `new` failed to allocate enough memory. |
| `failed initializing` | Your program uses too much data defined in global scope for it to fit inside the memory constraints at startup. A typical example is code like `int arr[10000][10000]` on a problem with 64mb — the aforementioned array will take 381mb, far above the allowed limit. |

[file a ticket]: https://github.com/DMOJ/judge/issues

## OLE - Output Limit Exceeded
Your program outputted too much data to `stdout`, typically over 256mb (though some problems may have custom — generally larger — constraints).

## MLE - Memory Limit Exceeded
Your program ran out of memory. Sometimes, this might manifest itself as an RTE with `segmentation fault` or `std::bad_alloc`.

## TLE - Time Limit Exceeded
Your program took too long to execute.

## IE - Internal Error

If you see this, it means either the judge encountered an error or the problemsetter's configuration is incorrect. Administrators get notified of every internal error by email, and as such there is no need to do anything else — IEs will typically be resolved within 24 hours.
