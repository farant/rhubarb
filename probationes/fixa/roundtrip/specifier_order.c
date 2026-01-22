/* Test specifier order preservation */

/* Single specifiers */
static int static_var;
extern int extern_var;
const int const_var = 10;
volatile int volatile_var;

/* Double specifiers (static/extern first, then qualifier) */
static const int static_const_var = 1;
extern const int extern_const_var;
static volatile int static_volatile_var;

/* Const/volatile combinations - order matters! */
const volatile int const_volatile_var;
volatile const int volatile_const_var;

/* With initializers */
static int static_init = 100;
const int const_init = 200;
static const int static_const_init = 300;
