/* preprocessor_test.c - Test file for preprocessor directive roundtrip */
#include <stdio.h>
#include "myheader.h"

#define MAX_SIZE 100
#define MIN_SIZE 0
#define EMPTY
#define ADD(a, b) ((a) + (b))

#undef EMPTY

#pragma once

int global_var;

#ifdef DEBUG
int debug_flag = 1;
#else
int debug_flag = 0;
#endif

#ifndef RELEASE
int release_mode = 0;
#endif

#if defined(FEATURE_A)
int feature_a = 1;
#elif defined(FEATURE_B)
int feature_b = 1;
#else
int no_features = 1;
#endif

int main(void)
{
    int x = MAX_SIZE;
    return 0;
}
