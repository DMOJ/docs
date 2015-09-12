/**
 * Handler program for the signature grader.
 * 
 * This file reads from siggrade.1.in for the first testcase and siggrade.2.in for the second.
 * It calls is_valid (defined in header.h), a function which is implemented by a user's submission.
 * 
 * The output of this handler is compared for correctness against the contents of 
 * siggrade.1.out (or siggrade.2.out for the second case).
 * 
 * This is not a secure grader, as a malicious user's implementation of is_valid 
 * may simply printf() and exit(). Therefore, it is recommended that you secure (through obscurity) 
 * your streams by cloning them and closing the original streams.
 **/ 
#include "header.h"
#include <stdio.h>
#include <stdbool.h>

int main()
{
    int n;
    scanf("%d", &n);
    bool valid = is_valid(n); // Defined in header
    printf(valid ? "correct" : "wrong");
    return 0;
}
