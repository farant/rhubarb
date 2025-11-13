# Latina.h: Yes, We're Really Doing This

Alright. I know what you're thinking. You opened this file, you saw C keywords replaced with Latin words, and you thought "this person has lost their mind."

I get that reaction about forty times a week. "Why would you write C in Latin?" "Isn't this just making things harder?" "Are you doing this to be pretentious?"

Look, I'm gonna level with you. I don't care if you think it's crazy. I'm not trying to convince you to use this. If you want to write normal C with normal English keywords, go ahead. Nobody's stopping you.

But if you're still reading, I'm gonna explain why this exists, what it actually does, and why it's not as insane as it looks at first glance.

## The Actual Reason This Exists

Here's the thing about software. Most of it doesn't last. You write code in JavaScript, five years later half your dependencies don't work anymore. You write it in Python 2, suddenly Python 3 breaks everything. You write it for Windows XP, good luck running that in 2025.

Software has been on this incredibly short time horizon since the 90s. Months, weeks, sometimes days. Gold rush mentality. First mover advantage. Move fast and break things. All that nonsense.

But what if you're trying to build something that lasts? Not five years. Not ten years. Generations. Centuries, potentially. What does that even look like?

Well, you probably don't use JavaScript. You use something lower level, something that's been stable for decades. Like C89. That's a good start.

But there's another problem. English isn't a sacred language. It's not a universal language. It's the current lingua franca because of historical accidents - British Empire, American dominance, whatever. In 500 years? Who knows. Maybe everyone speaks Mandarin. Maybe we're all speaking some creole nobody can predict.

You know what language has been a universal language for educated people for over 2000 years? Latin. The Catholic Church has been using it since before there was a printing press. Cicero would recognize it. Thomas Aquinas would recognize it. It's stable in a way English never will be.

So if you're building something for the long term - really long term, multi-generational long term - using Latin isn't crazy. It's actually kind of obvious.

Plus, it filters people. If someone sees Latin C code and immediately dismisses it as "pretentious" without thinking about why it exists, that person probably isn't going to engage with the long-term thinking anyway. You want people who can get past their initial reaction and think about the reasoning. Latin does that filtering automatically.

## What This File Actually Does

`latina.h` is a header file that defines macros. That's it. It's not complicated.

```c
#define si          if
#define alioquin    else
#define per         for
#define dum         while
#define redde       return
```

You include this header in your C file, and now you can write:

```c
si (x > 0)
{
    redde VERUM;
}
alioquin
{
    redde FALSUM;
}
```

Instead of:

```c
if (x > 0)
{
    return VERUM;
}
else
{
    return FALSUM;
}
```

The compiler doesn't care. The preprocessor expands the macros before compilation. You're still writing C89. It's just C89 with Latin keywords instead of English keywords.

That's the whole trick. No language extension. No custom compiler. Just the C preprocessor doing what it's always done.

## The Complete Latin Vocabulary

Alright, let's go through every macro in the file. I'll give you the Latin, the English, and some notes on usage.

### Basic Types

```c
character    char       /* A single character */
brevis       short      /* A short integer (brevis = short, brief) */
integer      int        /* An integer (same root as English) */
longus       long       /* A long integer (longus = long) */
fluitans     float      /* Floating point (fluitans = floating) */
duplex       double     /* Double precision (duplex = double, twofold) */
```

Pretty straightforward. Most of these are cognates - you can guess the meaning even if you don't know Latin.

```c
integer x = 42;               /* int x = 42; */
fluitans y = 3.14f;           /* float y = 3.14f; */
duplex z = 2.718281828;       /* double z = 2.718281828; */
character littera = 'A';      /* char letter = 'A'; */
```

### Type Qualifiers

```c
vacuum       void       /* The void type (vacuum = empty, void) */
signatus     signed     /* Signed (signatus = marked, signed) */
insignatus   unsigned   /* Unsigned (in- = not, so "not signed") */
constans     const      /* Constant (constans = constant, unchanging) */
volatilis    volatile   /* Volatile (volatilis = flying, fleeting) */
```

The cool thing about `volatilis` is that the Latin actually captures the meaning better than the English. "Volatile" sounds like "explosive" in English. In Latin, `volatilis` means "flying" or "fleeting" - something that can change unexpectedly. That's exactly what the C keyword means.

```c
constans integer MAX = 100;        /* const int MAX = 100; */
vacuum* ptr = NIHIL;                /* void* ptr = NULL; */
volatilis insignatus integer reg;  /* volatile unsigned int reg; */
```

### Storage Classes

```c
sponte       auto       /* Automatic storage (sponte = of one's own accord) */
registrum    register   /* Register storage (registrum = register, list) */
staticus     static     /* Static storage (staticus = stationary, fixed) */
externus     extern     /* External linkage (externus = external, foreign) */
```

Now here's where it gets interesting. Everyone uses `static` for three completely different things in C:
1. Internal linkage for functions (file-scope private)
2. Persistent local variables (survives function calls)
3. Internal linkage for global variables (file-scope global)

This is confusing. Same keyword, three meanings. So `latina.h` provides three aliases:

```c
interior     static     /* Internal/private function (interior = inner, private) */
hic_manens   static     /* Persistent local var (hic manens = remaining here) */
universalis  static     /* File-scope global (universalis = universal) */
```

Now you can write:

```c
/* Private function (internal linkage) */
interior vacuum helper_function(vacuum)
{
    /* Only visible in this file */
}

/* File-scope global variable */
universalis integer counter = 0;

/* Function with persistent local variable */
vacuum increment(vacuum)
{
    hic_manens integer count = 0;  /* Persists between calls */
    count++;
    imprimere("Called %d times\n", count);
}
```

See? Now the code tells you WHICH meaning of `static` you're using. No ambiguity.

### Control Flow

```c
si           if         /* If (si = if) */
alioquin     else       /* Else (alioquin = otherwise) */
commutatio   switch     /* Switch (commutatio = change, exchange) */
casus        case       /* Case (casus = case, occasion, event) */
ordinarius   default    /* Default (ordinarius = ordinary, usual) */
```

Usage:

```c
si (x > 0)
{
    imprimere("positive\n");
}
alioquin si (x < 0)
{
    imprimere("negative\n");
}
alioquin
{
    imprimere("zero\n");
}

commutatio (x)
{
    casus 1:
        imprimere("one\n");
        frange;
    casus 2:
        imprimere("two\n");
        frange;
    ordinarius:
        imprimere("other\n");
        frange;
}
```

### Loops

```c
per          for        /* For (per = through, by means of) */
dum          while      /* While (dum = while, as long as) */
fac          do         /* Do (fac = do, make - imperative form) */
```

The word `per` is actually perfect for `for` loops. In Latin, `per` means "through" or "by means of" - you're iterating THROUGH a range. Makes more sense than "for" when you think about it.

```c
per (integer i = 0; i < 10; i++)
{
    imprimere("%d\n", i);
}

dum (condition)
{
    processum();
}

fac
{
    processum();
}
dum (condition);
```

### Loop Control

```c
frange       break      /* Break (frange = break, shatter - imperative) */
perge        continue   /* Continue (perge = go on, proceed - imperative) */
salta        goto       /* Goto (salta = jump - imperative) */
redde        return     /* Return (redde = give back - imperative) */
```

These are all imperative verb forms - commands. "Frange!" = "Break!" "Perge!" = "Continue!" "Salta!" = "Jump!" "Redde!" = "Give back!"

```c
per (integer i = 0; i < 100; i++)
{
    si (i == 50)
    {
        frange;  /* break out of loop */
    }
    si (i % 2 == 0)
    {
        perge;  /* skip to next iteration */
    }
    processum(i);
}

si (error)
{
    salta cleanup;  /* goto cleanup; */
}

cleanup:
    liberare_res();
    redde FALSUM;  /* return FALSE; */
```

### User-Defined Types

```c
structura    struct     /* Structure (structura = structure, arrangement) */
unio         union      /* Union (unio = union, unity) */
enumeratio   enum       /* Enumeration (enumeratio = counting, enumeration) */
nomen        typedef    /* Typedef (nomen = name) */
```

**IMPORTANT NOTE:** Don't use `nomen` as a variable name. It's a macro that expands to `typedef`. I've seen people do this:

```c
character* nomen = "Marcus";  /* WRONG - expands to "typedef* = ..." */
```

Use `titulus` (title) or `appellatio` (name, designation) instead:

```c
character* titulus = "Marcus";     /* Correct */
character* appellatio = "Marcus";  /* Also correct */
```

Usage examples:

```c
structura Persona
{
    character nomen[64];
    integer   aetas;
    fluitans  altitudo;
};

nomen structura Persona Persona;

enumeratio Status
{
    STATUS_BENE,
    STATUS_ERROR,
    STATUS_EXPECTANS
};

unio Datum
{
    integer   i;
    fluitans  f;
    character c;
};
```

### Operators and Special

```c
magnitudo    sizeof     /* Size of (magnitudo = size, magnitude) */
principale   main       /* Main function (principale = principal, main) */
```

The `principale` one is fun. Your main function becomes:

```c
integer principale(vacuum)
{
    imprimere("Salve, munde!\n");  /* Hello, world! */
    redde 0;
}
```

### Constants

```c
NIHIL        NULL       /* Null pointer (nihil = nothing) */
VERUM        1          /* True (verum = true, real) */
FALSUM       0          /* False (falsum = false) */
```

Latin doesn't have a word for "zero" (they didn't have the concept), but they sure had words for "nothing," "true," and "false."

```c
integer* ptr = NIHIL;

si (ptr == NIHIL)
{
    redde FALSUM;
}
redde VERUM;
```

### Standard Library

```c
imprimere    printf     /* Print (imprimere = to print, press into) */
```

Just `printf` for now. More standard library wrappers could be added, but honestly, you can just use the regular names. This is C, not a completely new language. Mix and match as needed.

```c
imprimere("x = %d, y = %f\n", x, y);
```

## Practical Examples (How This Actually Looks In Real Code)

### Simple Function

```c
#include "latina.h"

interior integer addere_numeros(integer a, integer b)
{
    redde a + b;
}

integer principale(vacuum)
{
    integer summa = addere_numeros(5, 7);
    imprimere("Summa: %d\n", summa);
    redde 0;
}
```

Equivalent English:

```c
static int add_numbers(int a, int b)
{
    return a + b;
}

int main(void)
{
    int sum = add_numbers(5, 7);
    printf("Sum: %d\n", sum);
    return 0;
}
```

### Struct Example

```c
structura Vector3
{
    fluitans x;
    fluitans y;
    fluitans z;
};

fluitans magnitudine_vectoris(constans structura Vector3* v)
{
    redde sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

vacuum normalizare_vectorem(structura Vector3* v)
{
    fluitans mag = magnitudine_vectoris(v);
    si (mag > 0.0f)
    {
        v->x /= mag;
        v->y /= mag;
        v->z /= mag;
    }
}
```

### Loop Example

```c
vacuum processum_tabula(integer* tabula, integer mensura)
{
    per (integer i = 0; i < mensura; i++)
    {
        si (tabula[i] < 0)
        {
            perge;  /* Skip negatives */
        }
        si (tabula[i] > 1000)
        {
            frange;  /* Stop at large values */
        }
        imprimere("Valor: %d\n", tabula[i]);
    }
}
```

### Error Handling

```c
nomen enumeratio
{
    SUCCESSUS = 0,
    ERROR_MEMORIA,
    ERROR_FILUM,
    ERROR_ARGUMENTUM
} Status;

Status legere_filum(constans character* via, character* buffer, integer mensura)
{
    si (via == NIHIL || buffer == NIHIL)
    {
        redde ERROR_ARGUMENTUM;
    }

    FILE* f = fopen(via, "r");
    si (f == NIHIL)
    {
        redde ERROR_FILUM;
    }

    integer lectus = (integer)fread(buffer, 1, (size_t)mensura, f);
    fclose(f);

    si (lectus < 0)
    {
        redde ERROR_FILUM;
    }

    redde SUCCESSUS;
}
```

## The Philosophy (Why This Isn't Just For Show)

Look, I get it. At first glance this looks like affectation. "Oh, he's using Latin to seem smart." That's not what this is about.

Here's the actual reasoning:

**1. Long-term thinking.** If you're building something you want to last 50, 100, 500 years, you need to think about linguistic drift. English changes. Slang changes. What's clear today might not be clear in a century. Latin is stable because it's dead - it stopped evolving 1500 years ago. That's a feature, not a bug.

**2. Filtering mechanism.** Not everyone needs to work on your code. If someone sees Latin and immediately dismisses it without thinking about why it exists, they're probably not the person you want contributing anyway. You want people who can look past surface strangeness and engage with underlying ideas.

**3. Sacred vs. profane.** There's a Catholic concept of using sacred language for sacred purposes. If you're building something intended to last and serve truth, using a sacred language makes sense. It's a sign that you're taking the work seriously, that you see it as more than just "shipping features."

**4. Clarity through disambiguation.** English overloads words. "static" means three different things. "long" is both a type and an adjective. "break" is both a keyword and a common English word. Latin forces you to be more precise. `interior` vs `hic_manens` vs `universalis` is clearer than `static` three times.

**5. Cognitive shift.** When you switch languages, you think differently. Sapir-Whorf is basically true - different languages encode different thought patterns. Latin forces a different mindset, one that's more explicit about structure and relationship. That's useful when you're trying to think clearly about systems.

## Common Objections (And Why They're Missing The Point)

### "This is unreadable!"

To you. Right now. Because you don't know Latin.

You know what was unreadable to you once? C syntax. Pointers. Operator precedence. You learned those. You can learn this too.

And honestly, most of the words are cognates or near-cognates. `integer`, `structura`, `constans` - these are basically the same word in English. `redde` is "render" (give back). `per` is "per." You're not learning ancient Greek here.

### "Nobody will be able to maintain this!"

Good. I don't want "anybody" maintaining it. I want people who are serious enough to learn some basic Latin vocabulary.

There are maybe 50 words total in `latina.h`. You can learn 50 words in an afternoon. If someone won't spend an afternoon learning to read your codebase, do you really want them making changes?

### "This is just elitism!"

Maybe. So what?

Not everything has to be accessible to everyone. Some things require prerequisites. You can't contribute to the Linux kernel without learning C. You can't contribute to GCC without learning compiler theory. You can't contribute to this codebase without learning some Latin.

That's not elitism, that's having standards.

### "Why not just use English?"

Because English is temporary. It's the current lingua franca because of historical accidents. In 500 years, who knows.

Latin has been a universal language for educated people for two millennia. The Catholic Church has been using it continuously. Scientists used it for centuries (Newton wrote in Latin). It's about as stable as language gets.

If you're thinking in terms of centuries, not quarters, Latin makes sense.

### "The compiler doesn't care what language you use!"

Exactly. The compiler doesn't care. So why not use Latin?

If it makes no difference to the machine, and it has philosophical advantages for the human, why wouldn't you?

## Mixing Latin And English (You Don't Have To Be Pure)

Here's the thing - you don't have to use ALL the Latin macros. You can mix and match.

Some people prefer to keep type names in English but use Latin for control flow:

```c
int calculate_sum(int* numbers, int count)
{
    int sum = 0;
    per (int i = 0; i < count; i++)
    {
        si (numbers[i] < 0)
        {
            perge;
        }
        sum += numbers[i];
    }
    redde sum;
}
```

Others use Latin types but English control flow:

```c
integer calculate_sum(integer* numbers, integer count)
{
    integer sum = 0;
    for (integer i = 0; i < count; i++)
    {
        if (numbers[i] < 0)
        {
            continue;
        }
        sum += numbers[i];
    }
    return sum;
}
```

It's not all-or-nothing. Use what makes sense for your project.

The core libraries in this codebase (piscina, xar, chorda, etc.) use full Latin because they're intended to be foundational pieces that last. Your application code? Do what you want.

## Learning The Vocabulary (It's Not That Hard)

You don't need to be fluent in Latin. You just need to recognize the keywords.

Here's a cheat sheet of the most common ones:

**Types:**
- `integer` = int
- `character` = char
- `vacuum` = void

**Constants:**
- `NIHIL` = NULL
- `VERUM` = true/1
- `FALSUM` = false/0

**Control flow:**
- `si` = if
- `alioquin` = else
- `per` = for
- `dum` = while
- `redde` = return

**Common words:**
- `magnitudo` = sizeof
- `structura` = struct
- `constans` = const

That's like 15 words. You can learn 15 words in ten minutes.

After that, everything else is either similar (like `duplex` = double) or you can infer from context.

Give it a week and you'll be reading it as naturally as English. I've watched this happen. People complain it's unreadable, they work with it for a few days, and then they stop noticing. The brain adapts fast.

## Practical Notes For Actually Using This

### 1. Include it first

```c
#include "latina.h"
#include <stdio.h>
#include <stdlib.h>
```

The macros need to be defined before any code that uses them.

### 2. Don't use `nomen` as a variable name

Seriously. It's a macro for `typedef`. It will break your code in confusing ways.

Use `titulus`, `appellatio`, or just `name` if you're mixing English.

### 3. Editor syntax highlighting will be wrong

Your editor thinks `si` is a variable name, not a keyword. It won't color it blue like `if`.

You can fix this by adding the Latin keywords to your editor's syntax file. Or just live with it. The code still works fine.

### 4. Compiler errors will show English keywords

The preprocessor expands macros before compilation, so error messages will say `if` not `si`, `return` not `redde`.

This is actually helpful - you can read compiler errors even if you don't know Latin.

### 5. You can mix with regular C

You can use `if` and `si` in the same file. You can use `int` and `integer` interchangeably. They're the same thing after preprocessing.

Don't go crazy with mixing (pick a style and stick to it), but you CAN do it.

## Complete Reference (Every Macro In The File)

Here's the complete list, organized by category:

### Primitive Types
```c
character    char
brevis       short
integer      int
longus       long
fluitans     float
duplex       double
```

### Type Modifiers
```c
vacuum       void
signatus     signed
insignatus   unsigned
constans     const
volatilis    volatile
```

### Storage Classes
```c
sponte       auto
registrum    register
staticus     static
externus     extern
interior     static      /* For internal functions */
hic_manens   static      /* For persistent locals */
universalis  static      /* For file-scope globals */
```

### Control Flow - Conditionals
```c
si           if
alioquin     else
commutatio   switch
casus        case
ordinarius   default
```

### Control Flow - Loops
```c
per          for
dum          while
fac          do
```

### Control Flow - Jumps
```c
frange       break
perge        continue
salta        goto
redde        return
```

### Compound Types
```c
structura    struct
unio         union
enumeratio   enum
nomen        typedef
```

### Operators / Special
```c
magnitudo    sizeof
principale   main
```

### Constants
```c
NIHIL        NULL
VERUM        1
FALSUM       0
```

### Standard Library (Limited)
```c
imprimere    printf
```

## Etymology Notes (For The Curious)

If you're interested in where these words come from:

- **vacuum**: Latin "vacuus" = empty, void. We get "vacuum" directly from this.
- **constans**: Present participle of "constare" = to stand firm, be consistent.
- **volatilis**: From "volare" = to fly. Something that flies away, fleeting.
- **integer**: Latin "integer" = whole, complete, untouched.
- **duplex**: From "duo" (two) + "plicare" (to fold). Literally "two-fold."
- **frange**: From "frangere" = to break, shatter. We get "fracture" from this.
- **redde**: Imperative of "reddere" = to give back, return. "Render" in English.
- **per**: Latin preposition meaning "through, by means of, during."
- **dum**: Latin conjunction meaning "while, as long as, until."
- **si**: Latin conjunction meaning "if." Shows up in English in phrases like "si vis pacem, para bellum."
- **alioquin**: From "alio" (otherwise) + "quin" (but not). Literally "otherwise."
- **nihil**: From "ne" (not) + "hilum" (a small thing). Literally "not even a small thing," i.e., nothing.

## FAQ (Yes, More Questions)

### Q: Is this a joke?

No. This is a serious approach to building software for the long term.

Does it seem weird? Sure. So did the Gothic cathedrals that took 200 years to build. People thought that was crazy too. Now they're still standing 800 years later while the "practical" buildings have fallen apart.

### Q: Can I use this in production?

You can use it anywhere you can use C. It's just macros. The compiler sees normal C code.

Whether you SHOULD use it in production depends on your team and your goals. If you're shipping a SaaS app that needs to scale fast, probably not. If you're building something you want your grandchildren to maintain, maybe.

### Q: What if I want to add more standard library wrappers?

Go for it. Add them to your local copy of `latina.h`.

Some suggestions:
- `allocare` = malloc
- `liberare` = free
- `copiare` = memcpy
- `comparare` = memcmp
- `aperire` = fopen
- `claudere` = fclose
- `legere` = fread
- `scribere` = fwrite

Just remember: these are MACROS. They expand to the C standard library functions. They're not reimplementing anything.

### Q: Does this hurt performance?

Zero impact. Macros are expanded at compile time. The generated assembly is identical whether you wrote `if` or `si`.

### Q: How do I debug this?

Use a debugger like gdb or lldb. They see the compiled code, which is normal C. Breakpoints, stepping, variable inspection - all work normally.

Some debuggers can show you the original source with macros expanded, which is actually helpful.

### Q: Is there a performance cost to the macro expansion?

No. Preprocessing is basically free - takes microseconds. Your compile time is dominated by actual compilation, not macro expansion.

### Q: Can I use this with C++?

Probably not. C++ has way more keywords (`class`, `template`, `namespace`, etc.) and the interactions might be weird.

This is designed for C89. Stick with C.

### Q: What about international programmers?

Latin is more international than English. It's been the universal language of the educated for 2000 years. More people worldwide can read Latin than can read English at a high level.

Plus, cognates. If you speak Spanish, Italian, French, Portuguese, or Romanian, you already know half these words. If you speak English, you know a bunch of them too (integer, structure, constant, etc.).

### Q: Isn't this just pretentious gatekeeping?

Call it what you want. I call it having standards.

Not every project needs to be accessible to everyone. Some projects are worth putting in the effort to learn. If you won't learn 50 Latin words to work on a codebase, you probably weren't going to contribute meaningfully anyway.

---

Look, I'm not saying you have to use this. I'm not even saying you should use this. I'm saying it exists for reasons that make sense in a particular context - building software for the very long term with a particular philosophical orientation.

If that's not your context, use normal C with English keywords. No problem.

But if you're thinking in terms of generations, if you care about stability across centuries, if you want to build something that might outlive you by several lifetimes... then maybe this makes sense.

Or maybe you still think it's crazy. That's fine too. Not every idea is for everyone.

Now you know what `latina.h` does, why it exists, and how to use it. Do with that information what you will.

---

## Postscript: The Extended Type System And Other Stuff You'll Actually Need

Okay, so I told you about the basic macros in `latina.h` - the keywords, the control flow, all that. But if you actually look at code in this project, you're gonna see a bunch of other stuff that's not in that header file.

You're gonna see `i32` and `u32` and `ZEPHYRUM` and `memoriae_index` and you're gonna think "wait, where did these come from?"

So let me explain the extended ecosystem around `latina.h`. This stuff isn't technically IN the header, but it's conventions used throughout the codebase. Consider this your field guide to the actual code you'll be reading.

## Roman Numeral Constants (Because Of Course)

Alright, so you know how in normal C you write numeric literals like `0`, `1`, `2`, `42`, etc.? Well, this codebase uses Roman numerals for constants.

Why? Same reason as everything else - long-term stability. Roman numerals have been around for 2500 years and they're not going anywhere. Plus they're unambiguous - when you see `X` in code, you know it's 10. When you see `10`, is that decimal 10? Octal 10? Hex 10? Depends on context.

Here's the mapping:

```c
#define ZEPHYRUM    0    /* Zero (from Arabic/Greek - Latin didn't have zero) */
#define I           1    /* 1 */
#define II          2    /* 2 */
#define III         3    /* 3 */
#define IV          4    /* 4 */
#define V           5    /* 5 */
#define VI          6    /* 6 */
#define VII         7    /* 7 */
#define VIII        8    /* 8 */
#define IX          9    /* 9 */
#define X           10   /* 10 */
```

And it keeps going:

```c
#define XI          11
#define XII         12
#define XV          15
#define XVI         16
#define XX          20
#define XXX         30
#define XL          40
#define L           50
#define LX          60
#define LXX         70
#define LXXX        80
#define XC          90
#define C           100
#define CC          200
#define CCC         300
#define CD          400
#define D           500
#define DC          600
#define DCC         700
#define DCCC        800
#define CM          900
#define M           1000
```

Usage in actual code:

```c
per (integer i = ZEPHYRUM; i < X; i++)
{
    processum(i);
}

integer buffer[XXXII];  /* Array of 32 elements */

si (count > C)
{
    imprimere("Too many: %d\n", count);
}
```

**Note on ZEPHYRUM:** Latin didn't have a concept of zero (Romans used context and position, not a digit). The word "zephyrum" comes from Arabic "ṣifr" via Medieval Latin "zephirum." So we're using the Latin transliteration of the Arabic word for zero. It's historically appropriate for post-Roman mathematics.

You don't HAVE to use Roman numerals. You can write `0`, `1`, `2` like normal. But the convention in this codebase is to use Roman numerals for named constants and literals in code, especially for array sizes, loop bounds, and magic numbers.

Why not for everything? Because Roman numerals get unwieldy for large numbers. Nobody wants to write `MMMDCCCLXXXVIII` (3888). So for computed values, intermediate results, and large numbers, use regular decimal.

**Rule of thumb:** If it's a constant that appears in source code and it's under 100, use Roman numerals. If it's computed or large, use decimal.

## Fixed-Width Integer Types (Actually Important)

C89's integer types (`int`, `short`, `long`) have platform-dependent sizes. An `int` might be 16 bits, 32 bits, or 64 bits depending on your architecture.

That's a problem if you're trying to write code that lasts for decades across multiple platforms. So this codebase uses fixed-width types modeled after C99's `stdint.h`:

### Signed Integers

```c
typedef int8_t   s8;    /* Signed 8-bit:  -128 to 127 */
typedef int16_t  s16;   /* Signed 16-bit: -32768 to 32767 */
typedef int32_t  s32;   /* Signed 32-bit: -2^31 to 2^31-1 */
typedef int64_t  s64;   /* Signed 64-bit: -2^63 to 2^63-1 */
```

The `s` stands for "signed" (signatus in Latin). The number is the bit width.

Usage:

```c
s32 counter = ZEPHYRUM;
s64 timestamp = get_timestamp();
s8  small_value = XLII;
```

### Unsigned Integers

```c
typedef uint8_t   u8;    /* Unsigned 8-bit:  0 to 255 */
typedef uint16_t  u16;   /* Unsigned 16-bit: 0 to 65535 */
typedef uint32_t  u32;   /* Unsigned 32-bit: 0 to 2^32-1 */
typedef uint64_t  u64;   /* Unsigned 64-bit: 0 to 2^64-1 */
```

The `u` stands for "unsigned" (insignatus in Latin).

Usage:

```c
u32 hash = calculate_hash(data);
u8  byte = buffer[i];
u16 port = LXXX;  /* 80 */
```

### Platform-Sized Integers

```c
typedef int32_t  i32;   /* General-purpose 32-bit signed integer */
typedef uint32_t b32;   /* Boolean (32-bit, for struct alignment) */
```

**i32** is the default integer type in this codebase. It's always 32 bits, signed. Use this instead of `int` or `integer` when you care about exact size.

**b32** is for booleans. Why 32 bits instead of 8? Alignment and performance. On most architectures, 32-bit values are faster to access than 8-bit values. Plus they align naturally in structs.

Values are still just `VERUM` (1) or `FALSUM` (0), but they're stored in 32 bits:

```c
b32 is_valid = VERUM;
b32 has_error = FALSUM;

si (is_valid)
{
    processum();
}
```

### Floating Point

```c
typedef float  f32;    /* 32-bit floating point */
typedef double f64;    /* 64-bit floating point */
```

These are just shorter names for `float` and `double`. The `f` obviously stands for floating point.

```c
f32 position = 3.14f;
f64 precise_value = 2.718281828459045;
```

### Memory Sizes

```c
typedef size_t  memoriae_index;   /* For array indices and memory sizes */
```

**memoriae_index** (memory index) is used for array sizes, memory sizes, and anything else that represents an amount of memory or an index into memory.

Why not just use `size_t`? Because we're being explicit about what the value represents. When you see `memoriae_index`, you know it's a size or index. When you see `i32`, you know it's a general-purpose integer.

Usage:

```c
memoriae_index buffer_size = 4096;
memoriae_index bytes_read = legere(filum, buffer, buffer_size);

per (memoriae_index i = ZEPHYRUM; i < buffer_size; i++)
{
    processum(buffer[i]);
}
```

## Common Functions (The Stdlib Wrappers)

Okay, so `latina.h` only defines `imprimere` (printf), but you're gonna see other Latin function names throughout the code. These aren't macros - they're either wrappers or direct usage of C library functions with Latin names.

Here are the common ones:

### Memory Management

```c
exire(integer status)              /* exit(status) - Exit the program */
liberare(vacuum* ptr)              /* free(ptr) - Free allocated memory */
```

**exire** comes from "exire" = to go out, to exit. Imperative form of the verb.

**liberare** comes from "liberare" = to free, to liberate.

Wait, I said we're using arena allocation (piscina) so you don't need to call `free`, right? So why does `liberare` exist?

Because sometimes you DO need to allocate outside the arena. Not often, but sometimes. When you do, you use `liberare` to free it.

```c
character* temp = malloc(1024);  /* Emergency allocation outside piscina */
processum(temp);
liberare(temp);  /* Don't forget to free */
```

**Why exire instead of just exit?** Consistency. If you're using Latin everywhere else, may as well use it for exit too. Plus "exire" is more explicit - it's clearly a function that terminates the program.

Usage:

```c
si (error_critica)
{
    imprimere("FRACTA: error critica\n");
    exire(I);  /* Exit with error code 1 */
}

/* Success */
exire(ZEPHYRUM);  /* Exit with code 0 */
```

### String Operations

These are usually just the standard C string functions, but you might see Latin names in comments or wrappers:

```c
/* copiare   = strcpy, memcpy (to copy) */
/* comparare = strcmp, memcmp (to compare) */
/* mensura   = strlen (measure, length) */
```

Most code just uses the standard C names directly:

```c
character* nomen = "Marcus";
memoriae_index len = strlen(nomen);
character* copia = piscina_allocare(p, len + I);
strcpy(copia, nomen);
```

But if you see Latin names in function signatures or comments, now you know what they mean.

## Type Naming Conventions (Patterns You'll See)

When defining types, there are some conventions:

### Struct Types

```c
nomen structura Persona
{
    character nomen[LXIV];
    i32       aetas;
} Persona;
```

The struct name is typically a Latin noun in nominative case. Common examples:
- `Persona` = person
- `Datum` = data, thing given
- `Fructus` = fruit, result, outcome
- `Status` = state, status
- `Nodus` = node, knot
- `Tabula` = table, list

### Function Names

Functions typically use Latin verbs:
- `creare` = to create
- `destruere` = to destroy
- `addere` = to add
- `removere` = to remove
- `obtinere` = to obtain, get
- `ponere` = to put, place
- `legere` = to read
- `scribere` = to write
- `aperire` = to open
- `claudere` = to close

Combined with the type name:

```c
Persona* persona_creare(Piscina* piscina);
vacuum   persona_destruere(Persona* persona);
vacuum   persona_imprimere(Persona* persona);
```

The pattern is: `type_verb()` - the struct name, underscore, the action.

### Constants and Enums

Enum values and constants are typically in UPPERCASE:

```c
enumeratio Status
{
    STATUS_BENE = ZEPHYRUM,     /* Good, well (status OK) */
    STATUS_ERROR = I,            /* Error */
    STATUS_EXPECTANS = II        /* Waiting, expecting */
};
```

## Putting It All Together (Real Example)

Here's what actual code looks like with all these conventions:

```c
#include "latina.h"
#include "piscina.h"

#define BUFFER_MAXIMUS  CCLVI  /* 256 */

nomen structura Documentum
{
    character  titulus[LXIV];   /* 64 character title */
    character* contentum;
    i32        lineas;
    b32        est_valida;
} Documentum;

interior Documentum* documentum_creare(Piscina* piscina, constans character* titulus)
{
    Documentum* doc;

    si (!piscina || !titulus)
    {
        redde NIHIL;
    }

    doc = (Documentum*)piscina_allocare(piscina, magnitudo(Documentum));
    si (!doc)
    {
        redde NIHIL;
    }

    /* Copy title */
    memoriae_index len = strlen(titulus);
    si (len >= LXIV)
    {
        len = LXIV - I;
    }
    memcpy(doc->titulus, titulus, len);
    doc->titulus[len] = '\0';

    doc->contentum = NIHIL;
    doc->lineas = ZEPHYRUM;
    doc->est_valida = VERUM;

    redde doc;
}

interior b32 documentum_legere(Documentum* doc, constans character* via)
{
    FILE* f;
    memoriae_index mensura;

    si (!doc || !via)
    {
        redde FALSUM;
    }

    f = fopen(via, "r");
    si (!f)
    {
        imprimere("FRACTA: non potest aperire filum: %s\n", via);
        redde FALSUM;
    }

    /* Get file size */
    fseek(f, ZEPHYRUM, SEEK_END);
    mensura = (memoriae_index)ftell(f);
    fseek(f, ZEPHYRUM, SEEK_SET);

    /* Read content */
    doc->contentum = (character*)malloc(mensura + I);
    si (!doc->contentum)
    {
        fclose(f);
        redde FALSUM;
    }

    memoriae_index lectus = fread(doc->contentum, I, mensura, f);
    doc->contentum[lectus] = '\0';
    fclose(f);

    /* Count lines */
    doc->lineas = I;
    per (memoriae_index i = ZEPHYRUM; i < lectus; i++)
    {
        si (doc->contentum[i] == '\n')
        {
            doc->lineas++;
        }
    }

    redde VERUM;
}

interior vacuum documentum_destruere(Documentum* doc)
{
    si (doc && doc->contentum)
    {
        liberare(doc->contentum);  /* We malloc'd this, so we free it */
        doc->contentum = NIHIL;
    }
    /* doc itself is from piscina, so no explicit free needed */
}

integer principale(vacuum)
{
    Piscina* piscina;
    Documentum* doc;

    piscina = piscina_generare_dynamicum("documentum", CDXCVI);  /* 4096 */
    si (!piscina)
    {
        imprimere("FRACTA: piscina generatio\n");
        exire(I);
    }

    doc = documentum_creare(piscina, "Exemplum Documentum");
    si (!doc)
    {
        imprimere("FRACTA: documentum creatio\n");
        piscina_destruere(piscina);
        exire(I);
    }

    si (documentum_legere(doc, "probatio.txt"))
    {
        imprimere("Documentum: %s\n", doc->titulus);
        imprimere("Lineas: %d\n", doc->lineas);
    }
    alioquin
    {
        imprimere("ERROR: legere fractum\n");
    }

    documentum_destruere(doc);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
```

See how it all fits together? Roman numerals for constants, fixed-width types for precision, Latin names for functions and structs, Latin keywords for control flow.

## Quick Reference Card (Tape This To Your Monitor)

**Numbers (< 100):**
- 0 = ZEPHYRUM
- 1 = I, 2 = II, 3 = III, 4 = IV, 5 = V
- 10 = X, 20 = XX, 50 = L, 100 = C

**Types:**
- i32, s32, u32 = 32-bit int (general, signed, unsigned)
- f32, f64 = 32-bit float, 64-bit double
- b32 = boolean (32-bit)
- memoriae_index = size_t (for sizes/indices)

**Functions:**
- exire = exit
- liberare = free
- imprimere = printf

**Patterns:**
- `type_verb()` for functions: `xar_creare`, `piscina_destruere`
- UPPERCASE for constants: `STATUS_ERROR`, `BUFFER_MAXIMUS`
- Lowercase for variables: `counter`, `buffer`, `mensura`

## Final Thoughts On The Extended System

Look, I know this seems like a lot. Latin keywords, Roman numerals, fixed-width types with weird names, function naming conventions...

But here's the thing: once you've worked with it for a week, it becomes natural. Your brain adapts. You stop translating and just read it directly.

And then you get the benefits:
- **Unambiguous sizes:** `i32` is always 32 bits. No platform surprises.
- **Clear constants:** `X` is obviously ten. `10` could be octal.
- **Consistent naming:** Every create function is `foo_creare`. Every destroy is `foo_destruere`.
- **Long-term stability:** Latin isn't changing. Roman numerals aren't changing. This code will be readable in 100 years.

Is it weird at first? Sure. So was learning pointers. So was learning recursion. You got over those. You'll get over this.

And if you don't - if this is too much, if you can't deal with Latin keywords and Roman numerals - that's fine. Write your code in English with Arabic numerals. No judgment.

But now you know what you're looking at when you read code from this project. You've got the full picture. Latin keywords from `latina.h`, Roman numeral constants, fixed-width types, naming conventions, the whole ecosystem.

Use it or don't. But at least now you understand it.
