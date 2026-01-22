/* Test modifier order preservation */

/* Simple modifiers (2 tokens) */
unsigned int unsigned_int_var;
signed int signed_int_var;
long int long_int_var;
short int short_int_var;

/* Implicit int (modifiers without explicit int) */
unsigned long unsigned_long_var;
signed long signed_long_var;
unsigned short unsigned_short_var;
long long long_long_var;

/* Three token modifiers */
unsigned long int uli_var;
signed long int sli_var;
unsigned long long ull_var;
signed long long sll_var;

/* Qualifiers with modifiers */
const unsigned int const_uint;
const long int const_lint;
volatile unsigned int vol_uint;
const unsigned long const_ulong;
const long long const_llong;

/* Double qualifiers with modifiers */
const volatile unsigned int cv_uint;
volatile const unsigned int vc_uint;
const volatile long long cv_llong;
