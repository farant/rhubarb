/* include_test_nested.h - Test nested include */

#ifndef INCLUDE_TEST_NESTED_H
#define INCLUDE_TEST_NESTED_H

#include "include_test_header.h"

#define NESTED_MACRO 99
#define DOUBLE_HEADER(x) ADD_HEADER((x), (x))

typedef HeaderInt NestedInt;

#endif /* INCLUDE_TEST_NESTED_H */
