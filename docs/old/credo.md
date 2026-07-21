# Credo: Stop Pretending Your Code Works

Look, here's the thing - and I cannot BELIEVE I have to explain this again - you cannot just write code and assume it works. I know, I know, you ran it once and it didn't crash. Congratulations. You want a medal? Your function returns a value, sure, but is it the RIGHT value? Did you test edge cases? Did you test ANYTHING?

No. Of course you didn't. That's why we're having this conversation.

This is **credo**. Latin for "I believe." As in, "I believe this code actually works because I TESTED it, not because I stared at it real hard and felt good vibes." It's a testing framework. You write assertions, they either pass or fail, and at the end you get a summary that tells you if your code is garbage or not.

I've been answering questions about testing frameworks for I-don't-even-know-how-long at this point, and somehow people STILL don't get it. So buckle up, we're doing this. Again. For the millionth time.

## What Even IS This?

It's a unit testing framework. For C89. Written in Latin, because apparently that's what we're doing now.

You make assertions about your code:
- "I believe this value equals 42"
- "I believe this pointer is not NULL"
- "I believe these two strings are the same"

If you're right, you get a dot (`.`). If you're wrong, you get an F and a big embarrassing printout of what you expected vs. what you actually got. At the end, credo tells you how many tests passed and how many failed.

```c
Piscina* p = piscina_generare_dynamicum("tests", 4096);
credo_aperire(p);

CREDO_AEQUALIS_S32(2 + 2, 4);        /* Passes: . */
CREDO_VERUM(5 > 3);                  /* Passes: . */
CREDO_FALSUM(NIHIL);                 /* Passes: . */

credo_imprimere_compendium();
credo_claudere();
piscina_destruere(p);
```

Output:
```
...

=== CREDO COMPENDIUM ===
Totalis:    3
Praeteriti: 3
Fracti:     0

Conditio: OMNIA PRAETERIERUNT
=========================
```

That's it. That's the entire library. I could've explained this in a tweet but you people need PAGES of documentation because apparently reading headers is too hard.

### Why Would I Want This?

Oh, I don't know, maybe because EVERY PIECE OF SOFTWARE YOU'VE EVER USED HAS BUGS? Maybe because "it compiled" is not the same as "it works"? Maybe because you're going to change something in three months and you want to know if you broke everything?

Here's what testing gives you:

**Confidence**: You actually know your code works instead of hoping real hard.

**Regression Detection**: Change one thing, run tests, find out immediately if you broke something. Better than finding out in production when users are screaming at you.

**Documentation**: Tests show how your functions are SUPPOSED to work. Way better than comments that lie because someone changed the code and forgot to update them.

**Debugging Speed**: When something breaks, tests tell you EXACTLY what's wrong. Not "somewhere in this 5000-line file." EXACTLY which assertion failed, in which file, on which line.

The alternative? Manually testing everything every time you change anything. Have fun with that. I'm sure your QA process of "click around and see if it crashes" is working great.

## Latin Crash Course (Because Of Course)

Alright, since we're writing testing macros in Latin like it's the 13th century, here's what you need to know:

- **credo** = I believe, I trust (the name of the library)
- **aperire** = to open (start the test session)
- **claudere** = to close (end the test session)
- **verum** = true
- **falsum** = false
- **aequalis** = equal
- **maior** = greater, bigger
- **minor** = less, smaller
- **praeteritus** = passed (literally "gone by")
- **fractus** = broken, failed (literally "broken")
- **totalis** = total
- **numerus** = number
- **compendium** = summary
- **omnia** = all, everything
- **notatio** = notation, record, mark

So when you see `credo_numerus_praeteriti()`, that's "credo_number_passed" - the count of tests that passed. When you see `CREDO_AEQUALIS_S32(a, b)`, that's "I believe a equals b, both signed 32-bit integers."

You're welcome for the free Latin lesson. Next time maybe crack open a dictionary yourself.

## How This Actually Works (The Mental Model)

Here's what happens under the hood, because I KNOW you're gonna ask:

When you call `credo_aperire(piscina)`, credo sets up global state to track your tests. It keeps a linked list of test results, all allocated from the piscina you gave it.

```c
/* Internal structure - you don't touch this */
structura CredoNotatio
{
    chorda  genus;          /* Type of test: "credo_verum", etc */
    chorda  expressio;      /* The expression tested: "x > 5" */
    chorda  valor_primus;   /* What we got */
    chorda  valor_secundus; /* What we expected */
    chorda  filum;          /* File name */
    s32     versus;         /* Line number */
    b32     praeteritus;    /* Did it pass? */
    memoriae_index ordo;    /* Test number */
    structura CredoNotatio* sequens; /* Next test */
}
```

Every time you use an assertion macro like `CREDO_VERUM(x)`, it:
1. Evaluates your expression
2. Creates a CredoNotatio record
3. Stores what happened (pass or fail)
4. Prints `.` or `F` immediately so you can watch progress
5. Adds the record to the linked list

At the end, `credo_imprimere_compendium()` walks the list and prints stats.

Simple. Straightforward. No magic. Just a linked list and some macros.

## Setting Up Your Tests

You've got exactly two functions to call before and after your tests. That's it. Two.

### Opening a Test Session

```c
Piscina* p = piscina_generare_dynamicum("tests", 4096);
credo_aperire(p);
```

Let's break this down for the five-hundredth time:

`credo_aperire(piscina)` - "credo open" - initializes the testing framework. You give it a piscina (arena allocator), and from that point on, every test result gets allocated from that pool. When you're done testing and you destroy the piscina, all the test records disappear. Clean. Simple. No leaks.

### Closing a Test Session

```c
credo_claudere();
```

`claudere` = "to close." This resets credo's internal state. Note that it does NOT destroy the piscina - you still need to do that yourself. Credo just forgets it was tracking anything.

```c
credo_claudere();
piscina_destruere(p);
```

That's the normal teardown. Close credo, then destroy the pool.

### The Complete Pattern

Here's what every test file looks like. EVERY. SINGLE. ONE.

```c
#include "credo.h"
#include "piscina.h"
/* ... your other includes ... */

s32 principale (vacuum)
{
    Piscina* piscina;
    b32      praeteritus;

    /* Setup */
    piscina = piscina_generare_dynamicum("test_pool", 4096);
    si (!piscina)
    {
        imprimere("Failed to create pool\n");
        redde I;
    }
    credo_aperire(piscina);

    /* YOUR TESTS GO HERE */
    CREDO_VERUM(1 + 1 == 2);
    CREDO_FALSUM(5 < 3);
    /* ... more tests ... */

    /* Teardown */
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    /* Return 0 if all passed, 1 if any failed */
    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
```

Copy this. Paste it. Change the tests in the middle. You're done. Was that so hard?

## Writing Assertions (The Important Part)

Alright, here's where we actually test stuff. Credo gives you a bunch of macros. Each one is an assertion - a claim about reality that's either true or false.

### Basic True/False Tests

```c
CREDO_VERUM(expression)   /* "I believe this is true" */
CREDO_FALSUM(expression)  /* "I believe this is false" */
```

Examples:
```c
CREDO_VERUM(x > 0);
CREDO_VERUM(pointer != NIHIL);
CREDO_FALSUM(error_occurred);
```

If the expression evaluates to true (non-zero), `CREDO_VERUM` passes. If it's false (zero), it fails and prints what went wrong.

`CREDO_FALSUM` is the opposite - it passes if the expression is false, fails if it's true.

### Pointer Checks

```c
CREDO_NON_NIHIL(pointer)  /* "I believe this is not NULL" */
CREDO_NIHIL(pointer)      /* "I believe this IS NULL" */
```

These are just convenient wrappers. `CREDO_NON_NIHIL(p)` is the same as `CREDO_VERUM(p != NIHIL)`, but shorter and clearer.

```c
vacuum* result = some_function();
CREDO_NON_NIHIL(result);  /* Better fail if we got NULL */
```

### Integer Equality (Signed)

```c
CREDO_AEQUALIS_S32(a, b)  /* "I believe a equals b" (signed) */
```

For signed 32-bit integers. If `a != b`, you get a failure message showing both values.

```c
s32 expected = 42;
s32 actual = compute_answer();
CREDO_AEQUALIS_S32(actual, expected);
```

Failure output:
```
F
 FRACTA (credo_aequalis): actual == expected at test.c:25
  Speratus: 42
  Receptus: 17
```

See that? It tells you EXACTLY what you expected (`Speratus` = expected) vs what you got (`Receptus` = received). No more printf debugging. The test tells you.

### Integer Equality (Unsigned)

```c
CREDO_AEQUALIS_I32(a, b)  /* "I believe a equals b" (unsigned) */
```

Same thing, but for unsigned integers. Use this for sizes, indices, anything that can't be negative.

```c
i32 size = buffer_size();
CREDO_AEQUALIS_I32(size, 1024);
```

### Integer Inequality

```c
CREDO_INAEQUALITAS_S32(a, b)  /* "I believe a != b" (signed) */
CREDO_INAEQUALITAS_I32(a, b)  /* "I believe a != b" (unsigned) */
```

Sometimes you need to verify things are DIFFERENT. Hash functions, unique IDs, whatever.

```c
i32 hash1 = chorda_friare(str1);
i32 hash2 = chorda_friare(str2);
CREDO_INAEQUALITAS_I32(hash1, hash2);  /* Different strings = different hashes */
```

### Comparisons (Greater/Less)

```c
CREDO_MAIOR_S32(a, b)                /* a > b (signed) */
CREDO_MAIOR_I32(a, b)                /* a > b (unsigned) */
CREDO_MINOR_S32(a, b)                /* a < b (signed) */
CREDO_MINOR_I32(a, b)                /* a < b (unsigned) */
CREDO_MAIOR_AUT_AEQUALIS_S32(a, b)   /* a >= b (signed) */
CREDO_MAIOR_AUT_AEQUALIS_I32(a, b)   /* a >= b (unsigned) */
CREDO_MINOR_AUT_AEQUALIS_S32(a, b)   /* a <= b (signed) */
CREDO_MINOR_AUT_AEQUALIS_I32(a, b)   /* a <= b (unsigned) */
```

More Latin vocabulary:
- `maior` = greater
- `minor` = less
- `aut` = or

So `CREDO_MAIOR_AUT_AEQUALIS_S32` = "I believe greater or equal, signed 32-bit."

```c
s32 size = get_buffer_size();
CREDO_MAIOR_S32(size, ZEPHYRUM);           /* Must be positive */
CREDO_MINOR_AUT_AEQUALIS_S32(size, 4096);  /* Can't exceed 4KB */
```

### String Comparisons

```c
CREDO_CHORDAE_AEQUALES(s1, s2)  /* "I believe these chordae are equal" */
```

Wait, hold on. `CREDO_CHORDAE_AEQUALES`? That's the old name. The one you should actually use is:

```c
CREDO_CHORDA_AEQUALIS(s1, s2)  /* Correct name */
```

For comparing `chorda` strings (not C strings). This uses `chorda_aequalis()` internally.

```c
chorda expected = chorda_ex_literis("hello", piscina);
chorda actual = some_string_function(piscina);
CREDO_CHORDA_AEQUALIS(actual, expected);
```

### String Convenience Macros

These are built on top of the basic macros for common string operations:

```c
CREDO_CHORDA_CONTINET(fenum, acus)       /* "haystack contains needle" */
CREDO_CHORDA_INCIPIT(s, prefixum)        /* "string starts with prefix" */
CREDO_CHORDA_TERMINATUR(s, suffixum)     /* "string ends with suffix" */
```

More vocabulary:
- `continet` = contains
- `fenum` = hay, haystack
- `acus` = needle
- `incipit` = begins with
- `terminatur` = ends with
- `prefixum` = prefix
- `suffixum` = suffix

```c
chorda result = parse_url("https://example.com/path");
CREDO_CHORDA_INCIPIT(result, chorda_ex_literis("https://", piscina));
```

### Floating Point (The Annoying One)

```c
CREDO_F64_PROXIMUS(a, b, epsilon)  /* "a ≈ b within epsilon" */
```

`proximus` = close, near. You can't just do `a == b` with floats because of precision issues. Instead, you check if they're "close enough."

```c
f64 expected = 3.14159;
f64 actual = compute_pi();
CREDO_F64_PROXIMUS(actual, expected, 0.00001);  /* Within 0.00001 */
```

Epsilon is your tolerance. How close is "close enough"? You decide.

## Looking at Test Output

When you run tests, credo prints results as it goes. Here's what you see:

### Passing Tests

```
...................
```

Each dot is a passing test. This is good. This is what you want. Lots of dots, no Fs.

### Failing Tests

```
....F
 FRACTA (credo_aequalis): x == 5 at test.c:42
  Speratus: 5
  Receptus: 7
..F
 FRACTA (credo_verum): y > 0 at test.c:48
  Speratus: y > 0
  Receptus: y > 0
.....
```

See the `F`? That's failure. Right after the F, you get:
- What kind of test failed (`credo_aequalis`, `credo_verum`, etc.)
- The expression that failed (`x == 5`, `y > 0`)
- Where it failed (`test.c:42`)
- What you expected (`Speratus`)
- What you got (`Receptus`)

This is INSTANT feedback. You don't wait until the end to find out something broke. You see the F, you see the details, you know exactly what's wrong.

### The Summary

At the end, `credo_imprimere_compendium()` prints a summary:

```
=== CREDO COMPENDIUM ===
Totalis:    47
Praeteriti: 45
Fracti:     2

Conditio: FRACTA
=========================
```

Let's translate:
- `Totalis` = total tests run
- `Praeteriti` = passed (literally "gone by")
- `Fracti` = failed (literally "broken")
- `Conditio` = condition/status

If everything passed:
```
Conditio: OMNIA PRAETERIERUNT
```

`OMNIA PRAETERIERUNT` = "all passed" (literally "all went by"). That's what you want to see.

## Organizing Your Tests

Look, I don't care HOW you organize your tests. But here's what most people do and it works fine.

### One Test File Per Library

```
probationes/
  probatio_chorda.c      # Tests for chorda.c
  probatio_piscina.c     # Tests for piscina.c
  probatio_filum.c       # Tests for filum.c
```

`probatio` = test, proof. So `probatio_chorda.c` = "chorda tests."

Each file tests one library. Keep it simple.

### Group Related Tests in Blocks

```c
/* ================================================
 * Probare chorda_ex_literis
 * ================================================ */
{
    chorda s1 = chorda_ex_literis("hello", piscina);
    CREDO_AEQUALIS_I32(s1.mensura, V);

    chorda s2 = chorda_ex_literis("", piscina);
    CREDO_AEQUALIS_I32(s2.mensura, ZEPHYRUM);
}

/* ================================================
 * Probare chorda_concatenare
 * ================================================ */
{
    chorda a = chorda_ex_literis("hello", piscina);
    chorda b = chorda_ex_literis("world", piscina);
    chorda c = chorda_concatenare(a, b, piscina);
    CREDO_CHORDA_AEQUALIS(c, chorda_ex_literis("helloworld", piscina));
}
```

See the blocks with curly braces? That's not just for show. It creates a new scope, so variables don't leak between test groups. You can reuse variable names (`s1`, `s2`, etc.) in each block without conflicts.

`Probare` = "to test" or "to prove." So `Probare chorda_ex_literis` = "Testing chorda_ex_literis."

### Print Section Headers (Optional)

```c
imprimere("\n--- Probans chorda_ex_literis ---\n");
{
    /* tests */
}

imprimere("\n--- Probans chorda_concatenare ---\n");
{
    /* tests */
}
```

This just makes the output easier to read. You see which section is running, then you see dots or Fs. Totally optional. Do it if you want. Don't if you don't care.

## Common Patterns

Alright, here's how you actually USE this thing in real life.

### Testing a Simple Function

```c
s32 add (s32 a, s32 b)
{
    redde a + b;
}

/* Test it */
{
    CREDO_AEQUALIS_S32(add(2, 3), V);
    CREDO_AEQUALIS_S32(add(ZEPHYRUM, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32(add(-5, 5), ZEPHYRUM);
}
```

Three tests:
1. Normal case (2 + 3 = 5)
2. Edge case (0 + 0 = 0)
3. Negative numbers (-5 + 5 = 0)

This is the basic pattern. Test normal behavior, test edge cases, test weird inputs.

### Testing Functions That Return Pointers

```c
vacuum* allocate_buffer (memoriae_index size, Piscina* p)
{
    redde piscina_allocare(p, size);
}

/* Test it */
{
    vacuum* buf = allocate_buffer(100, piscina);
    CREDO_NON_NIHIL(buf);  /* Should return valid pointer */

    /* Use the buffer... */
}
```

First thing you check: did we get a valid pointer? If this fails, everything else is pointless.

### Testing String Functions

```c
{
    chorda input = chorda_ex_literis("  trim me  ", piscina);
    chorda result = chorda_praecidi_laterale(input, piscina);
    chorda expected = chorda_ex_literis("trim me", piscina);

    CREDO_CHORDA_AEQUALIS(result, expected);
}
```

Pattern:
1. Create input
2. Call function
3. Create expected output
4. Compare them

Clear. Simple. Easy to read.

### Testing Edge Cases (The Actually Important Part)

Most bugs are in edge cases. Test them.

```c
/* Empty string */
{
    chorda empty = chorda_ex_literis("", piscina);
    chorda result = chorda_minuscula(empty, piscina);
    CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
}

/* NULL input */
{
    chorda result = chorda_ex_literis(NIHIL, piscina);
    CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
    CREDO_NIHIL(result.datum);
}

/* Very long string */
{
    /* Create a 1000-character string and test it */
}
```

Empty inputs. NULL pointers. Huge values. Zero values. Negative values. Test the WEIRD stuff. That's where your bugs are.

### Testing Multiple Related Functions

```c
/* String creation and comparison */
{
    chorda s1 = chorda_ex_literis("hello", piscina);
    chorda s2 = chorda_ex_literis("hello", piscina);
    chorda s3 = chorda_transcribere(s1, piscina);

    CREDO_CHORDA_AEQUALIS(s1, s2);  /* Same content */
    CREDO_CHORDA_AEQUALIS(s1, s3);  /* Copy equals original */
}
```

You're not just testing `chorda_ex_literis`. You're testing that it works with `chorda_aequalis` and `chorda_transcribere`. This is integration testing - making sure functions work TOGETHER.

## Checking Results

Once your tests run, you need to actually CHECK if they passed. Don't just run the tests and walk away.

### Getting Test Counts

```c
memoriae_index total  = credo_numerus_totalis();
memoriae_index passed = credo_numerus_praeteriti();
memoriae_index failed = credo_numerus_fracti();
```

- `credo_numerus_totalis()` - how many tests total
- `credo_numerus_praeteriti()` - how many passed
- `credo_numerus_fracti()` - how many failed

You probably don't need these. The compendium prints them. But they're there if you want programmatic access.

### Did Everything Pass?

```c
b32 all_passed = credo_omnia_praeterierunt();
si (all_passed)
{
    imprimere("SUCCESS!\n");
    redde ZEPHYRUM;  /* Exit code 0 */
}
alioquin
{
    imprimere("FAILURES!\n");
    redde I;  /* Exit code 1 */
}
```

`credo_omnia_praeterierunt()` returns `VERUM` if every single test passed, `FALSUM` if any failed.

Use this to set your program's exit code. Exit 0 for success, exit 1 for failure. That way you can use your tests in build scripts:

```bash
./probatio_chorda
si [ $? -ne 0 ]; then
    echo "Tests failed!"
    exit 1
fi
```

### Printing the Summary

```c
credo_imprimere_compendium();
```

This is usually the last thing before cleanup. It prints that nice summary box with total/passed/failed counts.

```
=== CREDO COMPENDIUM ===
Totalis:    150
Praeteriti: 150
Fracti:     0

Conditio: OMNIA PRAETERIERUNT
=========================
```

You want to see that "OMNIA PRAETERIERUNT" at the bottom. That's your green checkmark.

## Things That Will Annoy You (But Are Actually Fine)

### You Can't Nest credo_aperire Calls

```c
/* BAD - DON'T DO THIS */
credo_aperire(p1);
credo_aperire(p2);  /* This will mess everything up */
```

Credo uses global state. One session at a time. If you need multiple test suites, run them in separate programs.

### Tests Print Immediately

When a test fails, you see the F and the error message RIGHT AWAY. You can't buffer them up and print at the end.

This is BY DESIGN. Immediate feedback is better. If your program crashes halfway through tests, you know EXACTLY which test was running when it died.

### No Test Names

Tests don't have names. They just have the expression and location.

```c
CREDO_VERUM(x > 0);  /* No name, just the expression */
```

The expression IS the name. "x > 0" tells you what you're testing. If you need more context, add a comment or a printf before the test:

```c
imprimere("Testing boundary condition...\n");
CREDO_MAIOR_S32(x, ZEPHYRUM);
```

### You Need a Piscina

Credo requires a piscina for allocations. Every test record goes in the pool. If you don't have a pool, credo crashes with a fatal error.

```c
/* This crashes */
credo_aperire(NIHIL);
```

Just create a pool. It's one line. Stop complaining.

### Failed Tests Don't Stop Execution

When a test fails, credo prints the error and keeps going. It doesn't abort or exit.

```c
CREDO_AEQUALIS_S32(2 + 2, 5);  /* Fails, prints F */
CREDO_AEQUALIS_S32(3 + 3, 6);  /* Still runs */
```

You get to see ALL failures, not just the first one. This is better. Run all tests, see all problems, fix them all at once.

## API Reference

Alright, here's the boring part where I list every function and macro alphabetically. You've probably figured most of this out by now, but for completeness:

### Setup / Teardown

#### `credo_aperire()`

```c
vacuum credo_aperire (Piscina* piscina);
```

Opens a testing session. Initializes global state and sets the memory pool for test records.

**Parameters:**
- `piscina`: Memory pool for allocating test records

**Must be called before any assertions.**

**Example:**
```c
Piscina* p = piscina_generare_dynamicum("tests", 4096);
credo_aperire(p);
```

---

#### `credo_claudere()`

```c
vacuum credo_claudere (vacuum);
```

Closes the testing session. Resets global state but does NOT destroy the piscina.

**Example:**
```c
credo_claudere();
piscina_destruere(p);  /* You still destroy the pool */
```

---

### Query Functions

#### `credo_numerus_totalis()`

```c
memoriae_index credo_numerus_totalis (vacuum);
```

Returns total number of tests run.

**Returns:** Count of all assertions executed

---

#### `credo_numerus_praeteriti()`

```c
memoriae_index credo_numerus_praeteriti (vacuum);
```

Returns number of tests that passed.

**Returns:** Count of passing assertions

---

#### `credo_numerus_fracti()`

```c
memoriae_index credo_numerus_fracti (vacuum);
```

Returns number of tests that failed.

**Returns:** Count of failing assertions

---

#### `credo_omnia_praeterierunt()`

```c
b32 credo_omnia_praeterierunt (vacuum);
```

Checks if all tests passed.

**Returns:** `VERUM` if zero failures, `FALSUM` otherwise

**Example:**
```c
si (credo_omnia_praeterierunt())
{
    redde ZEPHYRUM;  /* Success exit code */
}
alioquin
{
    redde I;  /* Failure exit code */
}
```

---

#### `credo_imprimere_compendium()`

```c
vacuum credo_imprimere_compendium (vacuum);
```

Prints test summary with totals and pass/fail status.

**Example output:**
```
=== CREDO COMPENDIUM ===
Totalis:    25
Praeteriti: 23
Fracti:     2

Conditio: FRACTA
=========================
```

---

### Assertion Macros - Basic

#### `CREDO_VERUM(x)`

Asserts expression is true (non-zero).

**Example:**
```c
CREDO_VERUM(size > 0);
CREDO_VERUM(ptr != NIHIL);
```

---

#### `CREDO_FALSUM(x)`

Asserts expression is false (zero).

**Example:**
```c
CREDO_FALSUM(error);
CREDO_FALSUM(buffer_full);
```

---

#### `CREDO_NON_NIHIL(p)`

Asserts pointer is not NULL.

**Example:**
```c
vacuum* buf = allocate_buffer();
CREDO_NON_NIHIL(buf);
```

---

#### `CREDO_NIHIL(p)`

Asserts pointer IS NULL.

**Example:**
```c
vacuum* result = find_item(empty_list);
CREDO_NIHIL(result);
```

---

### Assertion Macros - Integer Equality

#### `CREDO_AEQUALIS_S32(a, b)`

Asserts two signed 32-bit integers are equal.

**Example:**
```c
s32 result = calculate_sum();
CREDO_AEQUALIS_S32(result, 42);
```

---

#### `CREDO_AEQUALIS_I32(a, b)`

Asserts two unsigned 32-bit integers are equal.

**Example:**
```c
i32 size = buffer_size();
CREDO_AEQUALIS_I32(size, 1024);
```

---

#### `CREDO_INAEQUALITAS_S32(a, b)`

Asserts two signed integers are NOT equal.

**Example:**
```c
CREDO_INAEQUALITAS_S32(id1, id2);
```

---

#### `CREDO_INAEQUALITAS_I32(a, b)`

Asserts two unsigned integers are NOT equal.

**Example:**
```c
i32 hash1 = hash_string(s1);
i32 hash2 = hash_string(s2);
CREDO_INAEQUALITAS_I32(hash1, hash2);
```

---

### Assertion Macros - Comparisons (Signed)

#### `CREDO_MAIOR_S32(a, b)`

Asserts `a > b` for signed integers.

**Example:**
```c
CREDO_MAIOR_S32(result, ZEPHYRUM);
```

---

#### `CREDO_MINOR_S32(a, b)`

Asserts `a < b` for signed integers.

**Example:**
```c
CREDO_MINOR_S32(value, MAX_SIZE);
```

---

#### `CREDO_MAIOR_AUT_AEQUALIS_S32(a, b)`

Asserts `a >= b` for signed integers.

**Example:**
```c
CREDO_MAIOR_AUT_AEQUALIS_S32(count, I);
```

---

#### `CREDO_MINOR_AUT_AEQUALIS_S32(a, b)`

Asserts `a <= b` for signed integers.

**Example:**
```c
CREDO_MINOR_AUT_AEQUALIS_S32(index, max_index);
```

---

### Assertion Macros - Comparisons (Unsigned)

#### `CREDO_MAIOR_I32(a, b)`

Asserts `a > b` for unsigned integers.

---

#### `CREDO_MINOR_I32(a, b)`

Asserts `a < b` for unsigned integers.

---

#### `CREDO_MAIOR_AUT_AEQUALIS_I32(a, b)`

Asserts `a >= b` for unsigned integers.

---

#### `CREDO_MINOR_AUT_AEQUALIS_I32(a, b)`

Asserts `a <= b` for unsigned integers.

---

### Assertion Macros - Strings

#### `CREDO_CHORDAE_AEQUALES(s1, s2)`

**DEPRECATED.** Use `CREDO_CHORDA_AEQUALIS` instead.

Asserts two `chorda` strings are equal (content comparison).

---

#### `CREDO_CHORDA_AEQUALIS(s1, s2)`

Asserts two `chorda` strings are equal.

**Example:**
```c
chorda result = process_string(input, piscina);
chorda expected = chorda_ex_literis("output", piscina);
CREDO_CHORDA_AEQUALIS(result, expected);
```

---

#### `CREDO_CHORDA_CONTINET(fenum, acus)`

Asserts haystack contains needle.

**Example:**
```c
chorda text = chorda_ex_literis("hello world", piscina);
chorda needle = chorda_ex_literis("world", piscina);
CREDO_CHORDA_CONTINET(text, needle);
```

---

#### `CREDO_CHORDA_INCIPIT(s, prefixum)`

Asserts string starts with prefix.

**Example:**
```c
chorda url = chorda_ex_literis("https://example.com", piscina);
chorda prefix = chorda_ex_literis("https://", piscina);
CREDO_CHORDA_INCIPIT(url, prefix);
```

---

#### `CREDO_CHORDA_TERMINATUR(s, suffixum)`

Asserts string ends with suffix.

**Example:**
```c
chorda filename = chorda_ex_literis("test.txt", piscina);
chorda suffix = chorda_ex_literis(".txt", piscina);
CREDO_CHORDA_TERMINATUR(filename, suffix);
```

---

### Assertion Macros - Floating Point

#### `CREDO_F64_PROXIMUS(a, b, epsilon)`

Asserts two doubles are approximately equal within epsilon tolerance.

**Parameters:**
- `a`: First value
- `b`: Second value
- `epsilon`: Maximum acceptable difference

**Example:**
```c
f64 result = calculate_pi();
CREDO_F64_PROXIMUS(result, 3.14159, 0.00001);
```

---

## FAQ (Questions I'm Tired Of Answering)

### Q: Why is it called "credo"?

Because it's Latin for "I believe." You're making assertions - statements of belief about your code. `CREDO_VERUM(x)` = "I believe x is true."

Also the author has this whole Latin thing going on. Get used to it.

### Q: Why do I need a piscina?

Because credo allocates memory for test records. It uses an arena allocator (piscina) for clean, leak-free allocation. When tests are done, you destroy the pool and everything disappears. Simple.

### Q: Can I run multiple test suites in one program?

No. Credo uses global state. One session per program. If you need multiple test suites, make multiple test programs.

Actually, you CAN reset and reuse: `credo_claudere()`, `credo_aperire()` again with a new pool. But why would you? Just run separate programs.

### Q: Why do failed tests keep running instead of stopping?

Because seeing ALL failures is more useful than seeing just the first one. Run all tests, see all problems, fix everything at once. More efficient than fix-one-run-again-fix-one-run-again.

### Q: Can I give tests names?

No. The expression IS the name. `CREDO_VERUM(x > 0)` - the test is "x > 0". That's your name. If you need more context, add a comment:

```c
/* Testing boundary condition for buffer overflow */
CREDO_MINOR_S32(index, buffer_size);
```

### Q: How do I test private/static functions?

Include the .c file in your test file:

```c
#include "library.c"  /* Yes, include the .c file */

/* Now you can test static functions */
```

Or don't make them static. Or make them static but test them through the public API. I don't care. Figure it out.

### Q: What's the difference between CREDO_VERUM and CREDO_AEQUALIS?

`CREDO_VERUM(expr)` just checks if the expression is true/false. It prints the expression.

`CREDO_AEQUALIS_S32(a, b)` specifically compares two integers and prints BOTH VALUES when it fails. More informative.

Use `CREDO_AEQUALIS` when you're comparing specific values. Use `CREDO_VERUM` for general boolean conditions.

### Q: Why S32 and I32 versions?

`S32` = signed 32-bit (can be negative)
`I32` = unsigned 32-bit (always non-negative)

The printing is different (`%d` vs `%u`). Use the right one for your data type or you'll get weird output.

### Q: Can I test multi-threaded code?

Sure, but credo itself isn't thread-safe. If your tests spawn threads, make sure the threads don't call credo macros concurrently. Or just test thread logic with single-threaded tests. Whatever works.

### Q: How do I test code that exits/crashes?

You don't. Not with this framework anyway. Credo is for unit testing normal functions that return values. If your code calls `exit()`, your test program exits. That's not a test failure, that's a program termination.

For testing crash conditions, use separate processes or a different testing strategy.

### Q: The output is ugly. Can I make it prettier?

No. The output is what it is. You get dots for passes, Fs for failures, and a summary. This is industry standard (see: every xUnit framework ever).

If you REALLY need pretty output, write a wrapper script that parses the text and reformats it. But that's your problem, not credo's.

### Q: Should I test every function?

Yes. Test everything that can break. Which is everything.

Well, okay, you don't need to test trivial getters/setters. But anything with logic? Test it. Anything that does calculations? Test it. Anything that manipulates data? TEST IT.

### Q: How many assertions per test?

However many make sense. Some "tests" (those blocks with curly braces) have one assertion. Some have ten. Group related assertions together.

```c
/* This is ONE test with multiple assertions */
{
    chorda s = chorda_ex_literis("  trim  ", piscina);
    chorda result = chorda_praecidi_laterale(s, piscina);

    CREDO_AEQUALIS_I32(result.mensura, IV);  /* Length check */
    CREDO_CHORDA_INCIPIT(result, chorda_ex_literis("trim", piscina));  /* Content check */
}
```

### Q: What if I want setup/teardown per test?

Put it in the test:

```c
{
    /* Setup */
    Piscina* local_pool = piscina_generare_dynamicum("test", 1024);

    /* Test */
    vacuum* buf = allocate_something(local_pool);
    CREDO_NON_NIHIL(buf);

    /* Teardown */
    piscina_destruere(local_pool);
}
```

Each block is independent. Set up what you need, test it, tear it down.

### Q: Can I disable tests?

Comment them out:

```c
/* This test is broken, will fix later
{
    CREDO_VERUM(broken_function());
}
*/
```

Or use `#if 0`:

```c
#if 0
{
    CREDO_VERUM(broken_function());
}
#endif
```

Credo doesn't have a "skip" feature. Just don't run the test.

---

Look, that's it. That's credo. It's not complicated. You make assertions, they pass or fail, you get a summary. Use it. Test your code. Stop shipping bugs.

And PLEASE stop asking me why your tests aren't running when you forgot to call `credo_aperire()`. It's in the documentation. You just read it. There's no excuse.

Now go write some tests before someone finds a bug in production and you have to explain why you didn't test anything.
