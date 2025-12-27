# Chapter 1: Foundations

This chapter introduces the fundamental abstractions of the rhubarb codebase: the
Latin dialect, memory allocation, and string handling. These three elements form
the substrate upon which everything else is built. Master them, and the rest
follows naturally.


## 1.1 The Latin Dialect

The file `latina.h` transforms C89 into something that reads like ecclesiastical
Latin. This is not ornament. The vocabulary serves as a filter: if you cannot
read the code, you are not the intended audience.

```c
nomen structura chorda {
    i32  mensura;
     i8* datum;
} chorda;
```

The mapping is consistent. Control flow:

| Latin       | C        |
|-------------|----------|
| si          | if       |
| alioquin    | else     |
| per         | for      |
| dum         | while    |
| redde       | return   |
| frange      | break    |

Types:

| Latin       | C               |
|-------------|-----------------|
| i8, i16, i32, i64 | unsigned integers |
| s8, s16, s32, s64 | signed integers   |
| f32, f64    | float, double   |
| b32         | boolean (int)   |
| vacuum      | void            |
| character   | char            |

Keywords:

| Latin       | C        |
|-------------|----------|
| nomen       | typedef  |
| structura   | struct   |
| constans    | const    |
| magnitudo   | sizeof   |

Constants use Roman numerals. `NIHIL` is `NULL`. `VERUM` and `FALSUM` are 1 and 0.
Numbers from I to MMM (and select others) are predefined.

**Exercise 1-1.** Read `latina.h` in full. Write a small program that prints
"Salve Munde" using only the Latin vocabulary.


## 1.2 Memory: The Piscina

The piscina (pool) is an arena allocator. All memory in rhubarb flows through it.
There is no `free()` of individual allocations. You allocate forward, and
eventually reset the entire pool.

The interface is minimal:

```c
/* Create a dynamic pool that grows as needed */
Piscina* piscina_generare_dynamicum(
    constans character* piscinae_titulum,
    memoriae_index      mensura_alvei_initia);

/* Create a fixed-size pool that cannot grow */
Piscina* piscina_generare_certae_magnitudinis(
    constans character* piscinae_titulum,
    memoriae_index      mensura_buffer);

/* Allocate from the pool (fatal on failure) */
vacuum* piscina_allocare(
    Piscina*       piscina,
    memoriae_index mensura);

/* Allocate with alignment */
vacuum* piscina_allocare_ordinatum(
    Piscina*       piscina,
    memoriae_index mensura,
    memoriae_index ordinatio);

/* Reset the pool, invalidating all allocations */
vacuum piscina_vacare(Piscina* piscina);

/* Destroy the pool entirely */
vacuum piscina_destruere(Piscina* piscina);
```

A typical usage pattern:

```c
Piscina* piscina = piscina_generare_dynamicum("principale", 4096);

/* allocate freely */
vacuum* data = piscina_allocare(piscina, 256);

/* when done with a phase of work */
piscina_vacare(piscina);

/* at program end */
piscina_destruere(piscina);
```

The internal structure is a linked list of buffers called `Alveus` (trough).
When the current buffer fills, a new one is allocated at double the size.
The `offset` tracks position within the current buffer:

```c
structura Piscina {
    Alveus*        primus;              /* first buffer */
    Alveus*        nunc;                /* current buffer */
    memoriae_index mensura_alvei_initia;
    character*     titulus;
    b32            est_dynamicum;
    memoriae_index maximus_usus;        /* peak usage */
};
```

### The Mark/Reset Pattern

For temporary allocations within a larger computation, the piscina supports
checkpointing:

```c
PiscinaNotatio nota = piscina_notare(piscina);

/* allocate temporary data */
vacuum* temp = piscina_allocare(piscina, 1024);
/* ... use temp ... */

/* restore to checkpoint, invalidating temp */
piscina_reficere(piscina, nota);
```

This pattern nests:

```c
PiscinaNotatio nota_exterior = piscina_notare(piscina);
    vacuum* a = piscina_allocare(piscina, 50);

    PiscinaNotatio nota_interior = piscina_notare(piscina);
        vacuum* b = piscina_allocare(piscina, 50);
    piscina_reficere(piscina, nota_interior);  /* b invalid */

piscina_reficere(piscina, nota_exterior);      /* a invalid */
```

### Two Allocation Modes

`piscina_allocare` is fatal on failure: the program exits. This is intentional.
Memory exhaustion in an arena-based program is unrecoverable.

For cases where graceful handling is possible, use `piscina_conari_allocare`
(try to allocate), which returns `NIHIL` on failure:

```c
vacuum* ptr = piscina_conari_allocare(piscina, huge_size);
si (!ptr) {
    /* handle gracefully */
}
```

**Exercise 1-2.** Write a program that allocates 1000 small blocks, uses
`piscina_notare`/`piscina_reficere` to free half of them, then queries
`piscina_summa_usus` to verify the memory was reclaimed.

**Exercise 1-3.** Create a fixed-size piscina of 256 bytes. Attempt to allocate
300 bytes with `piscina_conari_allocare`. Verify it returns `NIHIL`.


## 1.3 Strings: The Chorda

A chorda is a counted string. It is *not* null-terminated:

```c
nomen structura chorda {
    i32  mensura;   /* length in bytes */
     i8* datum;     /* pointer to data */
} chorda;
```

This design has consequences. You cannot pass `chorda.datum` to `printf("%s")`
or `strlen()`. The standard C string functions are off-limits.

### Creating Chordae

From a C string literal (copies the data):

```c
chorda s = chorda_ex_literis("salve munde", piscina);
```

From an existing buffer (no copy, just a view):

```c
i8 buffer[10] = "testdata";
chorda s = chorda_ex_buffer(buffer, 8);
```

A slice of another chorda (no copy):

```c
chorda full = chorda_ex_literis("abcdef", piscina);
chorda part = chorda_sectio(full, 2, 5);  /* "cde" */
```

### The View Distinction

Many chorda operations return *views*—they point into existing data without
allocation. Others *allocate* new storage. The header documents which is which:

```c
/* View (no allocation) */
chorda chorda_sectio(chorda s, i32 initium, i32 finis);
chorda chorda_praecidere(chorda s);  /* trim whitespace */

/* Allocates new storage */
chorda chorda_transcribere(chorda s, Piscina* piscina);
chorda chorda_concatenare(chorda a, chorda b, Piscina* piscina);
```

When a function takes a `Piscina*` parameter, it allocates. When it doesn't,
it returns a view.

### Comparison

```c
b32 chorda_aequalis(chorda a, chorda b);
b32 chorda_aequalis_literis(chorda s, constans character* cstr);
b32 chorda_aequalis_case_insensitivus(chorda a, chorda b);
s32 chorda_comparare(chorda a, chorda b);  /* like strcmp */
```

### Search

```c
b32 chorda_continet(chorda fenum, chorda acus);  /* haystack, needle */
b32 chorda_incipit(chorda s, chorda prefixum);
b32 chorda_terminatur(chorda s, chorda suffixum);

chorda chorda_invenire(chorda fenum, chorda acus);        /* returns view */
s32    chorda_invenire_index(chorda fenum, chorda acus);  /* returns position */
```

### Conversion to C Strings

When you must interface with C libraries:

```c
character* cstr = chorda_ut_cstr(s, piscina);
printf("%s\n", cstr);
```

This allocates a null-terminated copy.

### Splitting and Joining

```c
chorda_fissio_fructus fructus = chorda_fissio(s, ',', piscina);
per (i32 i = 0; i < fructus.numerus; i++) {
    /* fructus.elementa[i] is each piece */
}

chorda joined = chorda_iungere(elementa, numerus, separator, piscina);
```

**Exercise 1-4.** Given the string "salve::munde::orbis", split on "::" using
`chorda_fissio_chorda`, then rejoin with " - " as separator. Verify the result
is "salve - munde - orbis".


## 1.4 The Test Pattern

Every library has a corresponding test file in `probationes/`. The pattern is
consistent:

```c
#include "latina.h"
#include "piscina.h"
#include "credo.h"    /* testing framework */

s32 principale(vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("probatio", 4096);
    credo_aperire(piscina);

    /* tests go here */
    {
        chorda s = chorda_ex_literis("test", piscina);
        CREDO_AEQUALIS_I32(s.mensura, 4);
    }

    credo_imprimere_compendium();
    b32 praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);
    redde praeteritus ? 0 : 1;
}
```

The `credo` (I believe) framework provides assertions:

| Macro                  | Purpose                    |
|------------------------|----------------------------|
| CREDO_VERUM(x)         | assert x is true           |
| CREDO_FALSUM(x)        | assert x is false          |
| CREDO_NIHIL(x)         | assert x is NULL           |
| CREDO_NON_NIHIL(x)     | assert x is not NULL       |
| CREDO_AEQUALIS_I32     | assert i32 equality        |
| CREDO_AEQUALIS_S32     | assert s32 equality        |
| CREDO_CHORDA_AEQUALIS  | assert chorda equality     |

Run tests with:

```bash
./compile_tests.sh           # all tests
./compile_tests.sh chorda    # tests matching "chorda"
./compile_tests.sh piscina   # tests matching "piscina"
```

**Exercise 1-5.** Read `probationes/probatio_piscina.c` in full. Note how each
test block is isolated in its own scope with braces.


## 1.5 Reading Order

With the foundations understood, proceed through the codebase in this order:

**Phase 1: Data Structures**
1. `xar.h/c` — exponential array (dynamic collections)
2. `tabula_dispersa.h/c` — hash table
3. `internamentum.h/c` — string interning

**Phase 2: Parsing**
4. `stml.h/c` — the STML markup language
5. `json.h/c` — JSON parsing
6. `nuntium.h/c` — binary serialization

**Phase 3: Network**
7. `url.h/c` — URL handling
8. `tcp.h` / `tcp_posix.c` — sockets
9. `tls.h` / `tls_macos.m` — TLS
10. `http.h/c` — HTTP client

**Phase 4: Rendering**
11. `fenestra.h` / `fenestra_macos.m` — windows
12. `delineare.h/c` — drawing primitives
13. `layout.h/c` — layout engine

For each library, read in this order:
1. The header (`.h`) — understand the interface
2. The implementation (`.c`) — understand the mechanism
3. The test (`probatio_*.c`) — understand the usage

The tests are the best documentation.


## 1.6 Summary

Three primitives underlie everything:

- **latina.h** defines the vocabulary
- **piscina** manages all memory through arena allocation
- **chorda** represents strings as counted, non-null-terminated buffers

The piscina eliminates manual memory management. You allocate forward, checkpoint
with `piscina_notare`, and restore with `piscina_reficere`. At phase boundaries,
`piscina_vacare` resets everything.

The chorda eliminates null-termination bugs and enables zero-copy slicing. But
it requires discipline: never pass `chorda.datum` to functions expecting C strings.

The test files in `probationes/` demonstrate every function. When in doubt,
read the test.

---

**Further Reading**

- `probationes/probatio_piscina.c` — comprehensive piscina tests
- `probationes/probatio_chorda.c` — comprehensive chorda tests
- `include/credo.h` — the testing framework interface
