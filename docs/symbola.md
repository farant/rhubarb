# Symbola: Symbol Table Parsing for Mach-O Binaries

Look, I'm gonna level with you. If you're reading this, you probably opened up a binary in a hex editor, saw a bunch of garbage, and thought "there's gotta be a better way." There is. That's what symbola does. It reads the symbol table from Mach-O binaries so you can actually figure out what's in there without losing your mind.

This library sits on top of macho.h and pulls out the LC_SYMTAB load command, parses it, and gives you a nice clean API to work with symbols. You want to know what functions are in a binary? Where `_main` lives? What external libraries it's calling? Symbola's got you covered.

## What Even Is A Symbol Table?

Okay, systems programming 101 here. When you compile a C program, the compiler turns your nice readable function names like `calculate_sum` into memory addresses like `0x100003f80`. But here's the thing - the linker, the debugger, even crash reports need to remember what those addresses *mean*. That's what a symbol table is. It's basically a phone book that says "hey, `0x100003f80` is actually `_calculate_sum`" (note the underscore - C functions get prefixed with `_` on macOS, don't ask me why, it's a convention from the 1970s that nobody bothered to change).

Every symbol in the table has:
- **A name** (stored in a string table, more on that later)
- **An address** (where it lives in memory)
- **A type** (is it a function? data? debug info?)
- **A section** (which part of the binary it's in)
- **Flags** (is it external? weak? private?)

The Latin word **symbola** just means "symbols" (plural of symbolum). We're parsing multiple symbols, so plural makes sense. The library gives you a `TabulaSymbolorum` (symbol table) that contains many `Symbolum` (symbol) entries.

## Why Zero-Copy?

Before we dive in, you need to understand the philosophy here. This library doesn't copy data around like some kind of amateur hour operation. When you parse a Mach-O file, it's already sitting in memory. The symbol table is just bytes at some offset in that file. The string table is more bytes at another offset.

So instead of allocating new memory and copying everything (which would be SLOW and WASTEFUL), symbola just stores pointers into the original buffer. When you ask for a symbol name, it hands you a `chorda` (string) that points directly into the Mach-O file's string table. Zero copies. Zero allocations (well, except for the bookkeeping structures). Just pointers.

This is fast. This is how professionals do it. This is what Kernighan and Pike would do.

**Latin note**: *chorda* means "string" or "cord" - like a rope made of fibers. Your strings are made of characters, see? It's poetic. Get over it.

## Basic Usage: Reading Symbols

Let me show you the basics before you get bored and wander off.

```c
#include "symbola.h"
#include "macho.h"
#include "piscina.h"
#include <stdio.h>

integer principale(vacuum)
{
    Piscina* p;
    MachO* macho;
    TabulaSymbolorum* tabula;
    SymbolorumIterator iter;
    Symbolum* sym;

    /* Setup - you need a pool allocator */
    p = piscina_generare_dynamicum("symbola_test", 4*1024*1024);

    /* Open the binary */
    macho = macho_aperire("/bin/ls", p);
    si (!macho)
    {
        imprimere("Failed to open binary\n");
        redde I;
    }

    /* Extract the symbol table */
    tabula = tabula_symbolorum_ex_macho(macho, p);
    si (!tabula)
    {
        imprimere("No symbol table: %s\n", symbola_error_recens());
        redde I;
    }

    /* Iterate over all symbols */
    iter = symbolorum_iterator_initium(tabula);
    dum ((sym = symbolorum_iterator_proximum(&iter)) != NIHIL)
    {
        chorda appellatio = symbolum_nomen(sym, p);
        si (symbolum_est_functio(sym))
        {
            imprimere("Function: %.*s at 0x%lx\n",
                     appellatio.mensura, appellatio.datum,
                     symbolum_addressa(sym));
        }
    }

    piscina_destruere(p);
    redde ZEPHYRUM;
}
```

Let's break down what just happened because I know you're going to ask.

**Piscina**: This is your memory pool. All allocations go through it. *Piscina* means "pool" or "fish pond" in Latin. You allocate everything from the pool, and at the end you destroy the whole pool at once. No individual `free()` calls. It's beautiful, it's simple, it's how arena allocators work.

**macho_aperire**: Opens a Mach-O binary. This is from the macho library, not symbola. Symbola depends on macho because, duh, you need to parse the Mach-O file before you can extract the symbol table.

**tabula_symbolorum_ex_macho**: This is where the magic happens. *Tabula* means "table" or "tablet". *Symbolorum* is the genitive plural of symbolum, so "of symbols". "Table of symbols". This function searches through the Mach-O load commands, finds LC_SYMTAB, validates that it's not garbage, and sets up pointers to the symbol array and string table.

**symbolorum_iterator_initium**: *Initium* means "beginning". This gives you an iterator positioned at the start.

**symbolorum_iterator_proximum**: *Proximum* means "next". Call this in a loop until it returns NIHIL (NULL).

**si/dum/redde**: These are your Latin control flow keywords. *Si* = "if", *dum* = "while", *redde* = "return". You'll get used to it.

## Understanding Symbol Types

Not all symbols are created equal. Mach-O has a bunch of different symbol types, and you need to know the difference or you're going to have a bad time.

### Defined vs Undefined

A **defined symbol** is something that exists IN this binary. Like if you wrote a function called `calculate_sum`, that's defined. The code is right there in the binary.

An **undefined symbol** is something this binary NEEDS but doesn't have. Like when you call `printf` - that's in libc, not your binary. So your binary has an undefined symbol that says "hey, I need `_printf` from somewhere else". The dynamic linker figures this out at runtime.

Check this:
```c
si (symbolum_est_indefinitum(sym))
{
    imprimere("This symbol needs to be provided by a dylib\n");
}
```

*Indefinitum* is the Latin past participle of "indefinire" - "undefined".

### External vs Local

**External symbols** (N_EXT flag set) are visible outside this binary. Public functions, global variables - stuff that other code can link against.

**Local symbols** are internal. Static functions, file-scope variables. Nobody outside this compilation unit cares about them.

```c
si (symbolum_est_externum(sym))
{
    imprimere("This symbol is visible to other binaries\n");
}
```

*Externum* means "external" or "outside". Straightforward.

### Functions vs Everything Else

Here's where it gets annoying. Mach-O doesn't explicitly mark which symbols are functions. You have to infer it from context. Symbola uses a heuristic:

1. If it's a debug symbol (stab), it's not a function
2. If it's undefined, it's not a function (we don't have the code)
3. If it's not in a section, it's not a function
4. If it's in section 1 (usually `__TEXT,__text`), it's probably a function

Is this perfect? No. Could Apple have made this easier? Yes. Are we working with what we've got? Also yes.

```c
si (symbolum_est_functio(sym))
{
    /* This is PROBABLY a function */
    /* Good enough for most purposes */
}
```

## Searching For Specific Symbols

Iterating over every symbol is fine for a report or a dump, but usually you want something specific. Like "where's `_main`?" or "what's at address 0x100003f80?"

### Search By Name

```c
Symbolum* main_sym = symbolum_invenire(tabula, "_main", p);
si (main_sym != NIHIL)
{
    imprimere("Found _main at 0x%lx\n", symbolum_addressa(main_sym));
}
alioquin
{
    imprimere("No _main? What kind of binary is this?\n");
}
```

*Invenire* means "to find". *Alioquin* means "otherwise" or "else".

This does a linear search through the symbol table. Is it O(n)? Yes. Could we build a hash table? Sure, but then we'd need to allocate memory for the hash table, and that breaks the zero-copy philosophy. For most binaries with a few thousand symbols, linear search is FINE. Your CPU will do this in microseconds. Don't prematurely optimize.

### Search By Address

```c
memoriae_index addr = 0x100003f80;
Symbolum* sym = symbolum_invenire_per_addressa(tabula, addr, p);
si (sym != NIHIL)
{
    chorda appellatio = symbolum_nomen(sym, p);
    imprimere("0x%lx is %.*s\n", addr,
             appellatio.mensura, appellatio.datum);
}
```

*Per* means "through" or "by". So "find by address".

This is also linear search. Same reasoning. If you're doing this millions of times in a hot loop, sure, build an index. But you're probably not, so don't worry about it.

**Latin note**: *memoriae_index* is "memory index" - basically size_t. An index into memory.

## Filtering Symbols

Sometimes you want a subset of symbols. Like "give me all the functions" or "give me all the external undefined symbols I need to link against". Symbola has convenience functions for this.

### Get All Functions

```c
integer numerus_functionum;
Symbolum** functiones = symbola_obtinere_functiones(tabula,
                                                     &numerus_functionum,
                                                     p);

per (integer i = ZEPHYRUM; i < numerus_functionum; i++)
{
    chorda appellatio = symbolum_nomen(functiones[i], p);
    imprimere("[%d] %.*s\n", i, appellatio.mensura, appellatio.datum);
}
```

*Obtinere* means "to obtain" or "to get". *Functiones* is plural of "functio" - "function".

This does a two-pass algorithm:
1. First pass: count how many symbols match the filter
2. Allocate exactly that much memory
3. Second pass: collect the matching symbols

It's efficient. It's clean. It's what you should do when you don't know how big the result set will be.

### Get External Undefined Symbols

```c
integer numerus_externa;
Symbolum** externa = symbola_obtinere_externa_indefinita(tabula,
                                                          &numerus_externa,
                                                          p);

per (integer i = ZEPHYRUM; i < numerus_externa; i++)
{
    chorda appellatio = symbolum_nomen(externa[i], p);
    imprimere("Need from dylib: %.*s\n",
             appellatio.mensura, appellatio.datum);
}
```

*Externa indefinita* = "external undefined" symbols. These are the imports. The stuff your binary needs from shared libraries. This is useful for dependency analysis, reverse engineering, figuring out "what does this program actually DO?"

## Symbol Attributes

Each symbol has a bunch of attributes you can query. Let me just show you what's available.

```c
Symbolum* sym = /* get a symbol somehow */;

/* Basic info */
chorda appellatio = symbolum_nomen(sym, p);
memoriae_index addr = symbolum_addressa(sym);
integer genus = symbolum_genus(sym);
integer sect = symbolum_numerus_sectionis(sym);
integer desc = symbolum_descriptor(sym);

/* Boolean flags */
b32 est_functio = symbolum_est_functio(sym);
b32 est_externum = symbolum_est_externum(sym);
b32 est_indefinitum = symbolum_est_indefinitum(sym);
b32 est_debug = symbolum_est_debug(sym);
b32 est_debile = symbolum_est_debile(sym);
b32 est_privatum = symbolum_est_privatum_externum(sym);
```

*Appellatio* means "name" or "designation". *Genus* means "type" or "kind". *Debile* means "weak".

**Weak symbols** are a linker thing. If you have a weak symbol and a strong symbol with the same name, the strong one wins. It's used for stuff like providing default implementations that can be overridden. Most of the time you don't care about weak symbols, but when you do, you REALLY care.

## The LC_SYMTAB Structure

Alright, let's talk about what's actually in the binary. When you call `tabula_symbolorum_ex_macho`, it's looking for a load command with type LC_SYMTAB. This command has:

- **symoff**: Offset into the file where the symbol array starts
- **nsyms**: Number of symbols
- **stroff**: Offset into the file where the string table starts
- **strsize**: Size of the string table

The symbol array is either an array of `nlist_64` structs (for 64-bit) or `nlist` structs (for 32-bit). Each entry has:

```
n_strx:  offset into string table for this symbol's name
n_type:  type flags (debug? external? undefined?)
n_sect:  section number (1-based, or 0 if not in a section)
n_desc:  descriptor flags (weak? thumb? lazy bind?)
n_value: the address (or 0 for undefined symbols)
```

The string table is just a big blob of null-terminated strings packed together. It starts with a null byte (so offset 0 is always the empty string). Each symbol's `n_strx` field is an offset into this table.

Symbola validates all these offsets to make sure they're actually inside the file. If the binary is corrupt or malicious, it'll return NULL and set an error message. Always check for NULL returns. I know you're going to forget, everyone forgets, and then you'll segfault and waste 20 minutes debugging it.

## Working With Chorda Strings

Symbola returns strings as `chorda` structs, not C strings. This is important. A chorda has:

```c
structura chorda
{
    i32 mensura;    /* length in bytes */
    i8* datum;      /* pointer to data (NOT null-terminated) */
}
```

The data is NOT null-terminated because it's pointing into the string table, and the string table packs strings together. To print a chorda, you use the `%.*s` printf format:

```c
chorda s = symbolum_nomen(sym, p);
imprimere("%.*s\n", s.mensura, s.datum);
```

The `.*` means "the next argument is the length". Then you pass the length, then the pointer. This is standard C, not some weird Latin thing.

If you need a null-terminated C string for some legacy API that demands it, use:

```c
character* cstr = chorda_ut_cstr(s, p);
```

This will allocate a new string from the pool and copy it with a null terminator. But try to avoid this if you can, because you're making copies and that's exactly what we're trying to avoid.

## Error Handling

Symbola uses the same error pattern as the other libraries in this project. When something fails, the function returns NULL, and you can call `symbola_error_recens()` to get a string describing what went wrong.

```c
tabula = tabula_symbolorum_ex_macho(macho, p);
si (!tabula)
{
    constans character* err = symbola_error_recens();
    si (err)
    {
        imprimere("Error: %s\n", err);
    }
    alioquin
    {
        imprimere("Unknown error\n");
    }
    redde I;
}
```

*Recens* means "recent" or "fresh". So "recent error" = "last error".

The error string is in a static buffer, so don't try to free it. It gets overwritten by the next error. If you need to save it, copy it somewhere.

Common errors:
- "LC_SYMTAB non inventum" - The binary has no symbol table. Stripped binaries are like this.
- "Symbol table excedit mensuram fili" - The offsets point outside the file. Corrupted binary.
- "piscina_allocare fracta" - Out of memory. Allocate a bigger pool.

## Performance Notes

You want to know how fast this is? Fine.

Parsing the symbol table is basically free - we're just storing a few pointers. No copying, no allocations beyond the TabulaSymbolorum struct itself.

Iterating over symbols is as fast as iterating over an array, because that's what it is. Memory bandwidth limited. On a modern CPU you'll process millions of symbols per second.

Searching by name is O(n) with the string comparison being the expensive part. For a typical binary with 1000-10000 symbols, this is sub-millisecond. If you're doing this in a hot loop, cache the result.

Filtering functions does two passes over the symbol table, so it's 2*O(n). Still fast.

The zero-copy design means you're bottlenecked by the speed of reading the Mach-O file into memory, not by symbola itself. If you're reading from disk, that's your bottleneck. If the file's already in page cache, you're golden.

## Building A Symbol Table Index

Alright, say you're building a debugger or a profiler, and you're going to be looking up symbols by address A LOT. Like, millions of times. In that case, you want to build an index once and reuse it.

Here's how I'd do it:

```c
/* Build a sorted array of (address, symbol*) pairs */
structura AdressaSymbolum
{
    memoriae_index addressa;
    Symbolum* symbolum;
}

AdressaSymbolum* aedificare_indicem(TabulaSymbolorum* tabula,
                                     Piscina* p)
{
    integer numerus = tabula_symbolorum_numerus(tabula);
    AdressaSymbolum* index;
    SymbolorumIterator iter;
    Symbolum* sym;
    integer i;

    index = piscina_allocare(p, numerus * magnitudo(AdressaSymbolum));

    i = ZEPHYRUM;
    iter = symbolorum_iterator_initium(tabula);
    dum ((sym = symbolorum_iterator_proximum(&iter)) != NIHIL)
    {
        index[i].addressa = symbolum_addressa(sym);
        index[i].symbolum = sym;
        i++;
    }

    /* Sort by address (use qsort or your own) */
    /* ... sorting code ... */

    redde index;
}
```

Now you can binary search the index. That's O(log n) instead of O(n). For 10000 symbols, that's ~14 comparisons instead of 5000 on average. Worth it if you're doing this a lot.

But for most use cases? Don't bother. Linear search is fine. Measure first, optimize later.

## Integration With Other Libraries

Symbola is designed to work with the rest of the rhubarb ecosystem.

**With macho.h**: You need a MachO* to create a symbol table. Symbola reads from the same buffer that macho.h already loaded.

**With sectio.h**: Symbols reference sections by number. You can use sectio to figure out which section a symbol is in:

```c
integer sect_num = symbolum_numerus_sectionis(sym);
/* Now use sectio_* functions to find section by number */
/* (you'll need to iterate and count, sectio uses names not numbers) */
```

**With piscina.h**: Everything allocates from your pool. One pool, one `piscina_destruere()` call, everything gets cleaned up. This is the beauty of arena allocation.

**With chorda.h**: Symbol names come back as chorda structs. Use the chorda API to compare them, search them, convert them, whatever.

The whole project is designed to compose. Lego blocks that fit together. That's good API design.

---

# API Reference

Alright, here's the boring part where I document every single function. You're welcome.

## Types

### TabulaSymbolorum

```c
nomen structura TabulaSymbolorum TabulaSymbolorum;
```

Opaque type representing a symbol table. You don't get to see the internals. Create it with `tabula_symbolorum_ex_macho`, query it with the API functions, that's it.

### Symbolum

```c
nomen structura Symbolum Symbolum;
```

Opaque type representing a single symbol. Created by iterator or search functions. Query it with `symbolum_*` accessors.

### SymbolorumIterator

```c
nomen structura SymbolorumIterator {
    constans TabulaSymbolorum* tabula;
    i32 index_currens;
} SymbolorumIterator;
```

Iterator for walking all symbols. Not opaque because you need to declare it on the stack. Don't mess with the fields, just pass it to the iterator functions.

## Constants

### Symbol Type Masks

```c
#define SYMBOLUM_TYPE_UNDEF       0x0   /* Undefined */
#define SYMBOLUM_TYPE_ABS         0x2   /* Absolute */
#define SYMBOLUM_TYPE_SECT        0xe   /* In a section */
#define SYMBOLUM_TYPE_PBUD        0xc   /* Prebound */
#define SYMBOLUM_TYPE_INDR        0xa   /* Indirect */
#define SYMBOLUM_TYPE_MASK        0x0e  /* Mask to extract type */
```

Use `SYMBOLUM_TYPE_MASK` with `symbolum_genus()` to get the base type.

### Symbol Flags

```c
#define SYMBOLUM_EXT              0x01  /* External symbol */
#define SYMBOLUM_PEXT             0x10  /* Private external */
#define SYMBOLUM_STAB             0xe0  /* Debug symbol mask */
```

These are bit flags in the `n_type` field.

### Descriptor Flags

```c
#define SYMBOLUM_WEAK_REF         0x0040
#define SYMBOLUM_WEAK_DEF         0x0080
#define SYMBOLUM_NO_DEAD_STRIP    0x0020
```

These are bit flags in the `n_desc` field.

## Creation

### tabula_symbolorum_ex_macho

```c
TabulaSymbolorum*
tabula_symbolorum_ex_macho(
    constans MachO* macho,
    Piscina* piscina);
```

Create a symbol table from a Mach-O binary.

**Parameters:**
- `macho`: The Mach-O binary to extract from (must not be NULL)
- `piscina`: Pool allocator for allocations (must not be NULL)

**Returns:** Symbol table, or NULL on error (check `symbola_error_recens()`)

**Errors:**
- No LC_SYMTAB load command found
- Offsets are out of bounds (corrupt binary)
- Allocation failure

**Example:**
```c
MachO* macho = macho_aperire("/bin/ls", p);
TabulaSymbolorum* tabula = tabula_symbolorum_ex_macho(macho, p);
si (!tabula)
{
    imprimere("Error: %s\n", symbola_error_recens());
}
```

## Interrogation - Table

### tabula_symbolorum_numerus

```c
i32 tabula_symbolorum_numerus(constans TabulaSymbolorum* tabula);
```

Get the total number of symbols in the table.

**Returns:** Number of symbols, or 0 if table is NULL.

### tabula_symbolorum_symbolum

```c
Symbolum*
tabula_symbolorum_symbolum(
    constans TabulaSymbolorum* tabula,
    i32 index);
```

Get a symbol by index (0-based).

**Returns:** Symbol at that index, or NULL if index is out of bounds.

**Note:** This allocates a Symbolum from the pool each time you call it. If you're iterating, use the iterator instead.

## Iteration

### symbolorum_iterator_initium

```c
SymbolorumIterator
symbolorum_iterator_initium(constans TabulaSymbolorum* tabula);
```

Initialize an iterator at the beginning of the symbol table.

**Returns:** Iterator positioned at start.

### symbolorum_iterator_proximum

```c
Symbolum* symbolorum_iterator_proximum(SymbolorumIterator* iter);
```

Get the next symbol from the iterator.

**Returns:** Next symbol, or NULL when done.

**Example:**
```c
SymbolorumIterator iter = symbolorum_iterator_initium(tabula);
Symbolum* sym;
dum ((sym = symbolorum_iterator_proximum(&iter)) != NIHIL)
{
    /* process symbol */
}
```

### symbolorum_iterator_finis

```c
b32 symbolorum_iterator_finis(SymbolorumIterator* iter);
```

Check if iteration is complete.

**Returns:** VERUM if done, FALSUM if more symbols remain.

## Search

### symbolum_invenire

```c
Symbolum*
symbolum_invenire(
    constans TabulaSymbolorum* tabula,
    constans character* appellatio,
    Piscina* piscina);
```

Find a symbol by name. Does linear search.

**Parameters:**
- `appellatio`: Name to search for (e.g., "_main")
- `piscina`: Pool for allocation

**Returns:** Symbol if found, or NULL.

**Performance:** O(n) in number of symbols. Fast enough for typical usage.

### symbolum_invenire_per_addressa

```c
Symbolum*
symbolum_invenire_per_addressa(
    constans TabulaSymbolorum* tabula,
    memoriae_index addressa,
    Piscina* piscina);
```

Find a symbol by address. Does linear search.

**Returns:** Symbol at that exact address, or NULL. Does not find "nearest" symbol.

## Symbol Attributes

### symbolum_nomen

```c
chorda symbolum_nomen(constans Symbolum* symbolum, Piscina* piscina);
```

Get the symbol's name as a chorda (zero-copy, points into string table).

**Returns:** chorda with name, or empty chorda if NULL.

### symbolum_addressa

```c
memoriae_index symbolum_addressa(constans Symbolum* symbolum);
```

Get the symbol's address (n_value field).

**Returns:** Address, or 0 if NULL or undefined symbol.

### symbolum_genus

```c
i32 symbolum_genus(constans Symbolum* symbolum);
```

Get the symbol's type flags (n_type field).

**Returns:** Type byte, or 0 if NULL.

**Usage:** Mask with `SYMBOLUM_TYPE_MASK` to get base type, check for `SYMBOLUM_EXT`, etc.

### symbolum_numerus_sectionis

```c
i32 symbolum_numerus_sectionis(constans Symbolum* symbolum);
```

Get the section number (1-based).

**Returns:** Section number, or 0 if not in a section.

**Note:** Sections are 1-based in Mach-O. 0 means "not in a section" (like absolute symbols).

### symbolum_descriptor

```c
i32 symbolum_descriptor(constans Symbolum* symbolum);
```

Get the descriptor flags (n_desc field).

**Returns:** Descriptor flags, or 0 if NULL.

## Boolean Predicates

### symbolum_est_functio

```c
b32 symbolum_est_functio(constans Symbolum* symbolum);
```

Check if this symbol is (probably) a function.

**Heuristic:** Not debug, not undefined, in a section, section == 1.

**Returns:** VERUM if looks like a function, FALSUM otherwise.

### symbolum_est_externum

```c
b32 symbolum_est_externum(constans Symbolum* symbolum);
```

Check if this symbol is external (visible to other binaries).

**Returns:** VERUM if N_EXT flag set.

### symbolum_est_privatum_externum

```c
b32 symbolum_est_privatum_externum(constans Symbolum* symbolum);
```

Check if this symbol is private external.

**Returns:** VERUM if N_PEXT flag set.

### symbolum_est_indefinitum

```c
b32 symbolum_est_indefinitum(constans Symbolum* symbolum);
```

Check if this symbol is undefined (needs to be provided by a dylib).

**Returns:** VERUM if type is SYMBOLUM_TYPE_UNDEF.

### symbolum_est_debug

```c
b32 symbolum_est_debug(constans Symbolum* symbolum);
```

Check if this is a debug symbol (stab).

**Returns:** VERUM if SYMBOLUM_STAB mask matches.

### symbolum_est_debile

```c
b32 symbolum_est_debile(constans Symbolum* symbolum);
```

Check if this is a weak symbol.

**Returns:** VERUM if WEAK_DEF or WEAK_REF flag set.

## Filtering

### symbola_obtinere_functiones

```c
Symbolum**
symbola_obtinere_functiones(
    constans TabulaSymbolorum* tabula,
    i32* numerus,
    Piscina* piscina);
```

Get all function symbols.

**Parameters:**
- `numerus`: Output parameter, set to count of functions found
- `piscina`: Pool for allocation

**Returns:** Array of Symbolum pointers, or NULL if no functions or error.

**Performance:** Two-pass algorithm, 2*O(n) in number of symbols.

**Example:**
```c
integer count;
Symbolum** funcs = symbola_obtinere_functiones(tabula, &count, p);
per (integer i = ZEPHYRUM; i < count; i++)
{
    /* process funcs[i] */
}
```

### symbola_obtinere_externa_indefinita

```c
Symbolum**
symbola_obtinere_externa_indefinita(
    constans TabulaSymbolorum* tabula,
    i32* numerus,
    Piscina* piscina);
```

Get all external undefined symbols (imports from dylibs).

**Parameters:**
- `numerus`: Output parameter, set to count found
- `piscina`: Pool for allocation

**Returns:** Array of Symbolum pointers, or NULL if none found or error.

## Error Handling

### symbola_error_recens

```c
constans character* symbola_error_recens(vacuum);
```

Get the last error message.

**Returns:** Error string, or NULL if no error.

**Note:** String is in a static buffer. Don't free it. Copy it if you need to save it.

---

# FAQ

## Why doesn't symbola copy the symbol data?

Because copying is slow and wastes memory. The Mach-O file is already in memory. The symbol table is already in memory. The string table is already in memory. Why would we allocate more memory and copy it? That's insane.

Zero-copy means we just store pointers. It's faster, it uses less memory, and it's how serious systems programmers do it. Kernighan and Pike would approve.

The tradeoff is that the MachO buffer needs to stay alive while you're using the symbol table. Since they're both allocated from the same piscina, this happens automatically. When you destroy the pool, everything goes away at once.

## Why can't I modify symbols?

Because they're read-only views into the binary. If you want to modify a binary, you need to write a whole different tool that builds a new binary. That's not what symbola does.

Also, modifying symbols in place would require tracking which ones you changed, maybe growing the string table, updating offsets... it's a mess. Just read the data, do your thing, and if you need to change it, write a new binary.

## Why are the search functions O(n)?

Because building an index costs memory and time, and for most usage patterns, you don't need it. If you search for "_main" once in a binary with 5000 symbols, that's 2500 comparisons on average. That's microseconds on a modern CPU. Why complicate things?

If you're searching millions of times, build your own index. I showed you how earlier in this document. The API is flexible enough to let you do that.

Premature optimization is the root of all evil. - Donald Knuth. - Also me, right now.

## What's the deal with the underscore prefix on function names?

This is a macOS/Darwin thing inherited from the original Mach kernel and ultimately from classic Unix. C function names get prefixed with an underscore at the symbol level. So `main()` in your C code becomes `_main` in the symbol table.

Why? Because it avoids conflicts with assembly code that might want to use the same names. Back in the day, people wrote a lot of inline assembly, and this was a way to keep the namespaces separate.

You have to deal with it. When searching for a C function, remember the underscore.

## Why is "is this a function" a heuristic instead of exact?

Because Mach-O doesn't have a bit that says "this is a function". The format just doesn't work that way. A symbol is a (name, address, type, section, flags) tuple. Nothing says "function" vs "data".

So we infer it. If it's in section 1 (which is conventionally `__TEXT,__text`, the code section), and it's defined (not undefined), and it's not a debug symbol, it's probably a function.

Is this perfect? No. Could there be data symbols in the text section? Technically, yes. Does that happen in practice? Almost never.

Could Apple fix this by adding a flag? Yes. Will they? Probably not, because it would break backward compatibility with 40 years of tools.

## What if the binary is stripped?

Then there's no LC_SYMTAB, and `tabula_symbolorum_ex_macho` returns NULL with an error "LC_SYMTAB non inventum".

Stripped binaries don't have symbol tables. That's the whole point of stripping - remove debugging and symbol info to make the binary smaller and harder to reverse engineer.

You can't get symbols from a stripped binary with this library. You'd need to use other techniques like analyzing the code flow, looking at objective-C runtime structures, checking exports in LC_DYLD_INFO, etc. That's beyond the scope of symbola.

## Can I use this library on Windows or Linux?

No. This library parses Mach-O binaries, which are macOS/iOS/Darwin specific. If you're on Windows, you want something that parses PE/COFF. If you're on Linux, you want ELF. Different formats, different structures.

That said, you can run this library on Windows or Linux to *parse* Mach-O files from macOS. Like if you have a macOS binary and you want to analyze it on your Linux box, this will work fine. The library doesn't care what OS it's running on, it just cares about the format of the input file.

## Why Latin?

Because it filters out people who aren't serious. If you're not willing to learn a few Latin words to use a library, you're probably not going to put in the effort to understand systems programming anyway.

Also, it's fun. It's different. It makes the code distinctive. And honestly, once you get used to it, `si/alioquin/per/dum/redde` is just as readable as `if/else/for/while/return`. It's syntax. Your brain adapts.

Plus, it connects us to a 2000-year tradition of scholarship and precision. Latin was the language of science and engineering for centuries. Seems appropriate for low-level systems code.

## What's the difference between a symbol and a section?

A **section** is a chunk of the binary. Like `__TEXT,__text` contains executable code. `__DATA,__data` contains initialized global variables. `__TEXT,__cstring` contains string literals.

A **symbol** is a name that points to a location in memory. Like `_main` is a symbol that points to an address in the `__TEXT,__text` section.

Multiple symbols can be in the same section. In fact, all your functions are probably in the same `__text` section. But each function has its own symbol.

Use sectio.h to work with sections. Use symbola.h to work with symbols. Use them together to understand the structure of a binary.

## Can symbols have the same name?

Technically yes, though it's rare. The linker usually prevents this, but you can end up with duplicate symbols in weird cases (like weak symbols, or symbols from different shared libraries).

The search functions return the first match. If you need to find all symbols with a given name, iterate manually and check each one.

## What's the deal with symbolum_nomen returning chorda instead of char*?

Because we don't want to allocate and copy the string. The name is sitting right there in the string table, already in memory. So we return a chorda that points to it.

A chorda is just `{length, pointer}`. It's not null-terminated because the string table packs strings together without padding. That's fine. Use `%.*s` to print it. If you absolutely need a C string, use `chorda_ut_cstr()` to make a copy.

This is the zero-copy philosophy in action.

## How do I debug symbol parsing issues?

1. Use `symbola_error_recens()` to get error messages
2. Print the raw symbol count: `tabula_symbolorum_numerus()`
3. Iterate and print everything, see what you get:
   ```c
   SymbolorumIterator iter = symbolorum_iterator_initium(tabula);
   Symbolum* sym;
   dum ((sym = symbolorum_iterator_proximum(&iter)) != NIHIL)
   {
       chorda n = symbolum_nomen(sym, p);
       imprimere("%.*s: type=0x%x sect=%d addr=0x%lx\n",
                n.mensura, n.datum,
                symbolum_genus(sym),
                symbolum_numerus_sectionis(sym),
                symbolum_addressa(sym));
   }
   ```

4. Compare against `nm -pa /path/to/binary` to see what the system tools show
5. Check that you're looking for the right name (remember the underscore!)

## Can I use this to build a disassembler?

Sure. Symbola tells you where functions are. You'd combine this with:
- sectio.h to get the actual code bytes from `__TEXT,__text`
- A disassembly library (or write your own) to decode instructions
- Symbol addresses to label the functions

It's a building block. You still need to do the hard work of disassembling, but at least you know what you're disassembling.

## Is this library thread-safe?

The symbol table itself is read-only after creation, so multiple threads can query it simultaneously without problems.

The error buffer is a static global, so if multiple threads hit errors at the same time, they'll clobber each other's error messages. If you care about that, wrap your calls in a mutex. Or just don't hit errors.

The piscina allocator is not thread-safe by default (arena allocators rarely are). If you're allocating from multiple threads, you need either separate pools per thread or a lock around allocations.

For typical usage (single-threaded or read-only queries), you're fine.

## What about DWARF debug info?

That's a different thing. LC_SYMTAB is the basic symbol table. DWARF is a much richer debug format with source file names, line numbers, variable types, etc.

DWARF is more complex to parse and not covered by symbola. You'd need a separate library for that. But symbola gives you the foundation - once you know where the functions are, DWARF can tell you more about them.

Maybe that's the next library you write. Or maybe I write it. We'll see how annoying these super chats get.

## Why does the library use "appellatio" for name instead of "nomen"?

*Nomen* is reserved for the `typedef` keyword via the latina.h macros. Remember, `nomen structura Foo Foo` becomes `typedef struct Foo Foo`.

So we use *appellatio* which means "name" or "designation" or "that by which something is called". It's more specific anyway. A symbol's appellatio is what it's called, its identifier.

Latin is a rich language. There are multiple words for similar concepts with different nuances. That's a feature, not a bug.
