# SECTIO - Mach-O Section Parser

Look, you're on macOS. Every executable, every dylib, every piece of compiled code on this system is a Mach-O file. And Mach-O files are organized into **sections**. That's what this library does - it lets you pull sections out of those binaries so you can actually DO something with them.

"Sectio" means "section" or "a cutting" in Latin. Yeah, we're cutting sections out of binaries. Very clever. Moving on.

## What Problem Does This Solve?

Here's the deal: you've got a Mach-O binary sitting on disk. Maybe it's `/bin/ls`, maybe it's your own program. Inside that binary, the code lives in `__TEXT.__text`, the string constants live in `__TEXT.__cstring`, initialized data lives in `__DATA.__data`, and so on. Apple decided to organize everything into segments (like `__TEXT`) that contain sections (like `__text`).

Great. Now how do you GET that data?

You could read Apple's Mach-O documentation (which is scattered across three different header files and half of it is obsolete), write a bunch of pointer arithmetic to walk the load commands, figure out where the section headers are, validate all the offsets... OR you could just use this library.

This is what sectio.h does:
- Iterates all sections in a Mach-O file
- Finds specific sections by name
- Gives you zero-copy access to section data
- Tells you about section attributes (size, alignment, flags)

Zero-copy. That means the data you get back **points directly into the Mach-O buffer**. No allocation, no copying. Fast. Efficient. The way it should be.

## Prerequisites

Before you even THINK about using sectio.h, you need:

1. **macho.h** - This library sits on top of the Mach-O parser. You need a `MachO*` before you can extract sections from it.
2. **piscina.h** - Arena allocator. Everything in this codebase uses arena allocation. Get used to it.
3. **chorda.h** - Latin for "string". Our string type is `{i8* datum, i32 mensura}`. Length-prefixed, not null-terminated. Better than C strings in every way.

If you don't understand those libraries yet, go read their docs first. I'm not explaining arena allocation for the eighth time today.

## Basic Concepts

### Segments vs Sections

This confuses everyone, so let me spell it out:

- **Segment** (Latin: segmentum) - Top-level organizational unit. Examples: `__TEXT`, `__DATA`, `__LINKEDIT`
- **Section** (Latin: sectio) - Lives INSIDE a segment. Examples: `__TEXT.__text`, `__DATA.__data`

When you want to find a section, you specify BOTH:
```c
Sectio* text = sectio_invenire(macho, "__TEXT", "__text", piscina);
```

First parameter after macho: segment name ("__TEXT")
Second parameter: section name ("__text")

Don't mix them up. The library won't guess for you.

### Zero-Copy Architecture

I already mentioned this, but it's important enough to repeat:

When you call `sectio_datum()`, you get back a `chorda` (string) that points DIRECTLY into the Mach-O file buffer. No allocation happens. No copying happens. You're looking at the raw bytes from the file.

```c
Sectio* text = sectio_invenire(macho, "__TEXT", "__text", p);
chorda code = sectio_datum(text);

/* code.datum points into the macho buffer */
/* code.mensura is the section size */
/* The data is READ-ONLY - don't modify it */
```

If you need to keep that data around longer than the piscina lifetime, call `chorda_transcribere()` to make a copy. But usually? Just use it and move on.

### The Section Structure

The `Sectio` type is opaque. You don't get to see inside it. You call functions to query it:

```c
chorda name = sectio_nomen(sectio, piscina);               /* "nomen" = name */
chorda seg = sectio_nomen_segmenti(sectio, piscina);       /* segment name */
memoriae_index size = sectio_mensura(sectio);              /* "mensura" = size */
memoriae_index addr = sectio_addressa(sectio);             /* VM address */
i32 align = sectio_ordinatio(sectio);                      /* "ordinatio" = alignment */
i32 flags = sectio_vexilla(sectio);                        /* "vexilla" = flags */
```

Why opaque? Because the implementation might change. The API won't. That's called **encapsulation**. It's a good thing.

## Common Operations

### Finding a Specific Section

Most common use case: you know what section you want. Just ask for it.

```c
Piscina* p = piscina_generare_dynamicum("parse", 1024 * 1024);
MachO* macho = macho_aperire("/bin/ls", p);

/* Find the __TEXT.__text section (executable code) */
Sectio* text = sectio_invenire(macho, "__TEXT", "__text", p);

si (text != NIHIL)  /* "si" = if, "NIHIL" = NULL */
{
    chorda code = sectio_datum(text);
    memoriae_index size = sectio_mensura(text);

    imprimere("Code section: %zu bytes\n", size);
}
alioquin  /* "alioquin" = else */
{
    imprimere("Section not found\n");
}

piscina_destruere(p);
```

**Latin vocab dump:**
- `si` = if
- `alioquin` = else
- `NIHIL` = NULL
- `VERUM` = true (literally "truth")
- `FALSUM` = false
- `imprimere` = printf (literally "to print")
- `redde` = return

If the section doesn't exist, you get `NIHIL` back. Not an error - just means that particular binary doesn't have that section. Check for it.

### Iterating All Sections

Sometimes you don't know what you're looking for. Or you want to see everything. Use the iterator.

```c
Piscina* p = piscina_generare_dynamicum("parse", 1024 * 1024);
MachO* macho = macho_aperire("/bin/ls", p);

SectioIterator iter = sectio_iterator_initium(macho);  /* "initium" = beginning */
Sectio* sectio;

dum ((sectio = sectio_iterator_proximum(&iter)) != NIHIL)  /* "dum" = while */
{
    chorda seg_name = sectio_nomen_segmenti(sectio, p);
    chorda sec_name = sectio_nomen(sectio, p);
    memoriae_index size = sectio_mensura(sectio);

    imprimere("%.*s.%.*s - %zu bytes\n",
              seg_name.mensura, seg_name.datum,
              sec_name.mensura, sec_name.datum,
              size);
}

piscina_destruere(p);
```

**More Latin:**
- `dum` = while
- `proximum` = next
- `initium` = beginning
- `mensura` = measure/size
- `datum` = data

The iterator walks through ALL segments, and within each segment, ALL sections. It handles the complexity of jumping between load commands internally. You just call `proximum` until you get `NIHIL`.

### Checking Section Properties

Sections have flags that tell you what kind of data they contain. Use the helper functions instead of bit-masking manually.

```c
Sectio* text = sectio_invenire(macho, "__TEXT", "__text", p);

si (sectio_est_instructiones(text))  /* "est" = is */
{
    imprimere("Section contains executable instructions\n");
}

si (sectio_est_debug(text))
{
    imprimere("Section contains debug info\n");
}

i32 type = sectio_extrahere_genus(sectio_vexilla(text));  /* "genus" = type */

si (type == SECTIO_GENUS_REGULAR)  /* regular data */
{
    imprimere("Regular section\n");
}
alioquin si (type == SECTIO_GENUS_ZEROFILL)  /* BSS-style */
{
    imprimere("Zero-fill section (no data in file)\n");
}
```

The flags tell you important things:
- Is this code or data?
- Is it debug info?
- Should the loader zero-fill it?
- Are there relocations?

Most of the time you don't care. But when you DO care, the functions are there.

## Real-World Example: Dumping String Constants

Let's do something practical. Every program has string constants. On macOS, they live in `__TEXT.__cstring`. Let's dump them.

```c
#include "latina.h"
#include "macho.h"
#include "sectio.h"
#include "piscina.h"
#include <stdio.h>

s32 principale(vacuum)  /* "principale" = main */
{
    Piscina* p;
    MachO* macho;
    Sectio* cstring_sect;
    chorda data;
    i32 i;
    i32 start;

    /* Create arena */
    p = piscina_generare_dynamicum("strings", 4 * 1024 * 1024);
    si (!p)
    {
        imprimere("Failed to create piscina\n");
        redde I;  /* "redde" = return */
    }

    /* Open binary */
    macho = macho_aperire("/bin/ls", p);
    si (!macho)
    {
        imprimere("Failed to open Mach-O file\n");
        piscina_destruere(p);
        redde I;
    }

    /* Find __TEXT.__cstring section */
    cstring_sect = sectio_invenire(macho, "__TEXT", "__cstring", p);
    si (!cstring_sect)
    {
        imprimere("No __cstring section found\n");
        piscina_destruere(p);
        redde I;
    }

    /* Get section data (zero-copy) */
    data = sectio_datum(cstring_sect);

    imprimere("String constants from __TEXT.__cstring:\n");
    imprimere("=====================================\n\n");

    /* Walk through null-terminated strings */
    start = ZEPHYRUM;  /* "ZEPHYRUM" = 0 */
    per (i = ZEPHYRUM; i < data.mensura; i++)  /* "per" = for */
    {
        si (data.datum[i] == '\0')
        {
            si (i > start)  /* Non-empty string */
            {
                imprimere("  \"%s\"\n", &data.datum[start]);
            }
            start = i + I;  /* "I" = 1 */
        }
    }

    /* Cleanup */
    piscina_destruere(p);
    redde ZEPHYRUM;  /* Success */
}
```

**Latin numerals:**
- `ZEPHYRUM` = 0 (from Greek "zephyr" = nothing)
- `I` = 1
- `II` = 2
- `III` = 3
- ... you get the idea

This program:
1. Opens `/bin/ls`
2. Finds the `__cstring` section
3. Gets a zero-copy view of the data
4. Walks through it printing each null-terminated string

No malloc. No copying. Just direct access to the file data. Fast.

## Understanding Alignment

Every section has an alignment. It's stored as a power of 2.

```c
i32 align_power = sectio_ordinatio(text);
/* If align_power is 4, actual alignment is 2^4 = 16 bytes */
```

Why does this matter? Because when the loader maps the section into memory, it needs to know where valid boundaries are. Code sections are usually 16-byte aligned (2^4). Data sections might be 8-byte aligned (2^3).

You usually don't need to think about this unless you're writing a linker or doing very low-level memory mapping. But now you know what that number means.

## Understanding Addresses

Sections have two addresses:

1. **File offset** - Where the data lives in the file on disk (internal, not exposed)
2. **VM address** - Where the section WILL BE when loaded into memory

```c
memoriae_index vm_addr = sectio_addressa(text);
/* This is where the section lives in virtual memory */
/* Usually something like 0x100000000 + offset */
```

The VM address is useful when you're correlating addresses from crash logs or debuggers back to sections. "The crash happened at 0x100003a80 - which section is that?"

For most use cases, you don't care. The file offset (used internally by `sectio_datum()`) is what matters for reading data.

## Common Sections You'll Encounter

Here's a cheat sheet of sections you'll see in basically every Mach-O binary:

| Segment | Section | Contents |
|---------|---------|----------|
| `__TEXT` | `__text` | Executable code |
| `__TEXT` | `__stubs` | Stub functions for dynamic linking |
| `__TEXT` | `__cstring` | C string literals |
| `__TEXT` | `__const` | Constant data |
| `__TEXT` | `__unwind_info` | Stack unwinding information |
| `__DATA` | `__data` | Initialized global variables |
| `__DATA` | `__bss` | Uninitialized global variables (zero-fill) |
| `__DATA` | `__common` | Common symbols |
| `__DATA` | `__const` | Const data (yes, both TEXT and DATA can have it) |
| `__LINKEDIT` | (none) | Linker metadata - no sections, just raw data |

The `__TEXT` segment is read-only and executable. The `__DATA` segment is read-write but not executable. This is basic security - W^X (write XOR execute). You can't have memory that's both writable and executable at the same time.

## Error Handling

This library follows a simple convention:

**Functions return NIHIL (NULL) on failure.**

```c
Sectio* text = sectio_invenire(macho, "__TEXT", "__fake", p);
si (!text)
{
    /* Section doesn't exist - this is normal, not an error */
}
```

If you want to know WHY something failed:

```c
constans character* error = sectio_error_recens();  /* "recens" = recent */
si (error)
{
    imprimere("Error: %s\n", error);
}
```

But honestly? Most of the time you just check for NIHIL and move on. Either the section exists or it doesn't.

## Performance Characteristics

Let's talk about what's fast and what's slow.

**Fast (O(1)):**
- `sectio_datum()` - Returns a pointer, that's it
- `sectio_mensura()` - Returns a stored value
- `sectio_addressa()` - Returns a stored value
- All the query functions - they just read from the Sectio struct

**Medium (O(n) where n = number of load commands):**
- `sectio_invenire()` - Has to walk load commands until it finds the right segment, then walk sections
- In practice, n is small (usually 10-30 load commands), so this is still fast

**Medium (O(m) where m = total sections):**
- `sectio_iterator_proximum()` - Walks sections one by one
- For `/bin/ls`, m is maybe 15-20 sections. Not a big deal.

The point is: nothing in this library is slow. The biggest cost is usually reading the file from disk (handled by macho.h via filum.h), and that's already done before you get here.

## Memory Management

Everything is allocated from the Piscina you provide. When you call `piscina_destruere()`, everything goes away - the Mach-O buffer, the Sectio structs, the chorda strings, all of it.

```c
Piscina* p = piscina_generare_dynamicum("parse", 1024 * 1024);

MachO* macho = macho_aperire("/bin/ls", p);
Sectio* text = sectio_invenire(macho, "__TEXT", "__text", p);
chorda name = sectio_nomen(text, p);

/* ... use the data ... */

piscina_destruere(p);
/* Now macho, text, name are all invalid - don't touch them */
```

This is why we use arenas. No tracking individual allocations, no leak potential, no use-after-free bugs (as long as you don't use pointers after destroying the piscina, which would be insane).

## Thread Safety

None. This library is not thread-safe.

If you want to use it from multiple threads, give each thread its own Piscina and its own MachO. Don't share. The error tracking uses a static buffer, so concurrent calls will clobber each other's errors anyway.

Look, you're parsing binaries. This is not a hot path. Just do it on one thread and move on with your life.

## Limitations

Let me be upfront about what this library DOESN'T do:

1. **No 32-bit support** - The code only handles `LC_SEGMENT_64`. If you have a 32-bit binary, you're out of luck. Who cares? It's 2025.

2. **No writing** - This is a read-only library. You can't modify sections and write them back. If you want to patch binaries, write your own code.

3. **No relocation handling** - Sections can have relocations. This library doesn't parse them. That's a separate concern.

4. **No symbol resolution** - Sections don't know about symbols. That's what the symbol table is for (not covered by this library).

5. **Assumes valid input** - If you feed it a corrupt Mach-O file, behavior is undefined. Probably a crash. Validate your inputs.

These aren't bugs. They're design decisions. This library does ONE thing - extract sections from Mach-O files. It does it well. Everything else is someone else's problem.

---

# API Reference

## Types

### `Sectio`

Opaque type representing a section. Don't touch the internals. Use the accessor functions.

### `SectioIterator`

Iterator for walking all sections in a Mach-O file. Create with `sectio_iterator_initium()`, advance with `sectio_iterator_proximum()`.

```c
nomen structura SectioIterator {
    constans MachO* macho;
    i32 mandatum_index;        /* Current load command */
    i32 sectio_index;          /* Current section in segment */
    i32 sectio_numerus;        /* Number of sections in current segment */
    memoriae_index offset_currens;  /* Offset to current section header */
} SectioIterator;
```

You can read these fields if you're debugging, but don't modify them. The iterator manages its own state.

## Functions - Iteration

### `sectio_iterator_initium`

```c
SectioIterator sectio_iterator_initium(constans MachO* macho);
```

Initialize an iterator to walk all sections in a Mach-O file.

**Parameters:**
- `macho` - The Mach-O image to iterate (from macho.h)

**Returns:**
- Iterator positioned at the beginning

**Latin:** `initium` = beginning, start

**Example:**
```c
SectioIterator iter = sectio_iterator_initium(macho);
Sectio* sectio;

dum ((sectio = sectio_iterator_proximum(&iter)) != NIHIL)
{
    /* Process sectio */
}
```

### `sectio_iterator_proximum`

```c
Sectio* sectio_iterator_proximum(SectioIterator* iter);
```

Get the next section from the iterator.

**Parameters:**
- `iter` - Pointer to the iterator

**Returns:**
- Pointer to the next Sectio, or NIHIL when done

**Latin:** `proximum` = next

**Notes:**
- The Sectio is allocated from the macho's piscina
- Returns NIHIL at end of iteration
- Safe to call multiple times after reaching the end (keeps returning NIHIL)

### `sectio_iterator_finis`

```c
b32 sectio_iterator_finis(SectioIterator* iter);
```

Check if iteration is complete.

**Parameters:**
- `iter` - Pointer to the iterator

**Returns:**
- VERUM if done, FALSUM if more sections remain

**Latin:** `finis` = end

**Notes:**
- Usually you just check for NIHIL from `proximum` instead of calling this
- Provided for completeness

## Functions - Finding Sections

### `sectio_invenire`

```c
Sectio* sectio_invenire(
    constans MachO* macho,
    constans character* nomen_segmenti,
    constans character* nomen_sectionis,
    Piscina* piscina);
```

Find a section by segment and section name.

**Parameters:**
- `macho` - The Mach-O image to search
- `nomen_segmenti` - Segment name (e.g., "__TEXT")
- `nomen_sectionis` - Section name (e.g., "__text")
- `piscina` - Arena for allocation

**Returns:**
- Pointer to Sectio if found, NIHIL if not found

**Latin:** `invenire` = to find, `nomen` = name

**Example:**
```c
Sectio* text = sectio_invenire(macho, "__TEXT", "__text", p);
si (!text)
{
    imprimere("Section not found\n");
}
```

**Notes:**
- Both names are case-sensitive
- Names are matched exactly - no wildcards
- Returns NIHIL if segment doesn't exist
- Returns NIHIL if segment exists but section doesn't
- This is NOT an error - some binaries don't have all sections

## Functions - Section Attributes

### `sectio_nomen`

```c
chorda sectio_nomen(constans Sectio* sectio, Piscina* piscina);
```

Get the section name.

**Parameters:**
- `sectio` - The section
- `piscina` - Arena for string allocation

**Returns:**
- chorda containing the section name (e.g., "__text")

**Latin:** `nomen` = name

**Notes:**
- Mach-O stores names as char[16], possibly not null-terminated
- This function properly extracts and null-terminates the name
- Returns empty chorda (mensura=0) if sectio is NIHIL

### `sectio_nomen_segmenti`

```c
chorda sectio_nomen_segmenti(constans Sectio* sectio, Piscina* piscina);
```

Get the segment name for this section.

**Parameters:**
- `sectio` - The section
- `piscina` - Arena for string allocation

**Returns:**
- chorda containing the segment name (e.g., "__TEXT")

**Latin:** `segmentum` = segment

### `sectio_datum`

```c
chorda sectio_datum(constans Sectio* sectio);
```

Get the raw data for this section (ZERO-COPY).

**Parameters:**
- `sectio` - The section

**Returns:**
- chorda pointing into the Mach-O buffer

**Latin:** `datum` = data

**IMPORTANT:**
- This is zero-copy - the chorda points directly into the macho buffer
- Do NOT modify the data - it's read-only
- The data is valid only as long as the piscina lives
- To keep the data longer, use `chorda_transcribere(data, other_piscina)`

**Example:**
```c
chorda data = sectio_datum(text);
/* data.datum points into the macho file buffer */
/* data.mensura is the section size in bytes */
```

### `sectio_mensura`

```c
memoriae_index sectio_mensura(constans Sectio* sectio);
```

Get the size of the section in bytes.

**Parameters:**
- `sectio` - The section

**Returns:**
- Size in bytes, or 0 if sectio is NIHIL

**Latin:** `mensura` = measure, size

### `sectio_addressa`

```c
memoriae_index sectio_addressa(constans Sectio* sectio);
```

Get the virtual memory address where this section will be loaded.

**Parameters:**
- `sectio` - The section

**Returns:**
- VM address (e.g., 0x100003a80), or 0 if sectio is NIHIL

**Latin:** `addressa` = address

**Notes:**
- This is the runtime address, not the file offset
- Useful for correlating addresses from debuggers/crash logs

### `sectio_ordinatio`

```c
i32 sectio_ordinatio(constans Sectio* sectio);
```

Get the alignment requirement for this section (as a power of 2).

**Parameters:**
- `sectio` - The section

**Returns:**
- Alignment power (e.g., 4 means 2^4 = 16-byte alignment)

**Latin:** `ordinatio` = alignment, arrangement

**Example:**
```c
i32 align_power = sectio_ordinatio(text);
i32 align_bytes = I << align_power;  /* 2^align_power */
imprimere("Section aligned to %d bytes\n", align_bytes);
```

### `sectio_vexilla`

```c
i32 sectio_vexilla(constans Sectio* sectio);
```

Get the raw section flags.

**Parameters:**
- `sectio` - The section

**Returns:**
- Flags bitfield, or 0 if sectio is NIHIL

**Latin:** `vexilla` = flags, banners

**Notes:**
- Use the helper functions instead of bit-masking manually
- See `sectio_est_instructiones()`, `sectio_extrahere_genus()`

## Functions - Section Type Helpers

### `sectio_extrahere_genus`

```c
i32 sectio_extrahere_genus(i32 vexilla);
```

Extract the section type from the flags bitfield.

**Parameters:**
- `vexilla` - Section flags (from `sectio_vexilla()`)

**Returns:**
- Section type constant (SECTIO_GENUS_*)

**Latin:** `extrahere` = to extract, `genus` = type, kind

**Example:**
```c
i32 flags = sectio_vexilla(text);
i32 type = sectio_extrahere_genus(flags);

si (type == SECTIO_GENUS_REGULAR)
{
    imprimere("Regular section\n");
}
```

**Section type constants:**
- `SECTIO_GENUS_REGULAR` (0x0) - Normal data
- `SECTIO_GENUS_ZEROFILL` (0x1) - BSS-style, no data in file
- `SECTIO_GENUS_CSTRING_LITERALS` (0x2) - C string literals
- `SECTIO_GENUS_4BYTE_LITERALS` (0x3) - 4-byte literals
- `SECTIO_GENUS_8BYTE_LITERALS` (0x4) - 8-byte literals
- `SECTIO_GENUS_LITERAL_POINTERS` (0x5) - Pointers to literals
- `SECTIO_GENUS_SYMBOL_STUBS` (0x8) - Dynamic linking stubs
- `SECTIO_GENUS_MOD_INIT_FUNC_POINTERS` (0x9) - Module initializer pointers
- `SECTIO_GENUS_MOD_TERM_FUNC_POINTERS` (0xa) - Module terminator pointers

### `sectio_est_instructiones`

```c
b32 sectio_est_instructiones(constans Sectio* sectio);
```

Check if section contains executable instructions.

**Parameters:**
- `sectio` - The section

**Returns:**
- VERUM if section has instruction flags, FALSUM otherwise

**Latin:** `est` = is

**Notes:**
- Checks for `SECTIO_ATTR_PURE_INSTRUCTIONS` or `SECTIO_ATTR_SOME_INSTRUCTIONS`
- Typically true for `__TEXT.__text` and `__TEXT.__stubs`

### `sectio_est_debug`

```c
b32 sectio_est_debug(constans Sectio* sectio);
```

Check if section contains debug information.

**Parameters:**
- `sectio` - The section

**Returns:**
- VERUM if section is debug-only, FALSUM otherwise

**Latin:** `est` = is

**Notes:**
- Debug sections are usually stripped in release builds
- Examples: `__DWARF.__debug_info`, `__DWARF.__debug_line`

## Functions - Error Handling

### `sectio_error_recens`

```c
constans character* sectio_error_recens(vacuum);
```

Get the most recent error message.

**Parameters:**
- None (vacuum = void)

**Returns:**
- Error message string, or NIHIL if no error

**Latin:** `recens` = recent, `vacuum` = void, empty

**Notes:**
- Error messages are stored in a static buffer
- Not thread-safe
- Calling any sectio function may overwrite the error
- Usually you just check for NIHIL returns instead of calling this

**Example:**
```c
Sectio* text = sectio_invenire(macho, "__TEXT", "__fake", p);
si (!text)
{
    constans character* error = sectio_error_recens();
    si (error)
    {
        imprimere("Error: %s\n", error);
    }
    alioquin
    {
        imprimere("Section not found (not an error)\n");
    }
}
```

---

# FAQ

## Why don't section names need to be freed?

Because they're allocated from the Piscina you passed in. When you call `piscina_destruere()`, everything goes away. No manual memory management needed.

If you're used to malloc/free, this feels weird at first. Get used to it. Arena allocation is superior for this kind of code.

## What's the difference between sectio_datum() and sectio_mensura()?

`sectio_datum()` gives you a chorda (which contains both the pointer AND the size). `sectio_mensura()` just gives you the size as a raw number.

```c
chorda data = sectio_datum(text);
/* data.datum is the pointer */
/* data.mensura is the size */

memoriae_index size = sectio_mensura(text);
/* Just the size */
```

Use `sectio_datum()` when you want the actual data. Use `sectio_mensura()` when you just need to know how big it is.

## Why does the iterator track mandatum_index and sectio_index?

Because Mach-O structure is weird. Sections don't live in a nice flat array. They're grouped into segments, and each segment is a separate load command.

So the iterator has to:
1. Walk load commands (mandatum_index)
2. Skip non-segment commands
3. For each segment, walk its sections (sectio_index)
4. Jump to the next segment when done with current one

That's why there are two indices. The implementation handles this complexity so you don't have to.

## Can I modify section data?

No. The data you get from `sectio_datum()` points into the file buffer, which should be treated as read-only.

If you want to modify it, copy it first:
```c
chorda data = sectio_datum(text);
Piscina* temp = piscina_generare_dynamicum("temp", data.mensura);
chorda copy = chorda_transcribere(data, temp);
/* Now modify copy.datum */
```

But really, if you're modifying binaries, you should be writing the data to a NEW file, not mutating the original buffer.

## Why zero-copy? Wouldn't copying be safer?

Safer from what? Use-after-free? Don't use pointers after destroying the piscina. Problem solved.

Zero-copy is faster, uses less memory, and has less code. The only "safety" you lose is protection against your own stupidity. Don't be stupid.

## What's memoriae_index?

It's a typedef for size_t. "memoriae" is the genitive case of "memoria" (memory), so "memoriae_index" literally means "index of memory" or "memory index".

Why not just use size_t? Because we're writing in Latin. Pay attention.

## Do I need to call sectio_iterator_finis()?

No. Just check if `sectio_iterator_proximum()` returns NIHIL. That's the idiomatic way.

```c
dum ((sectio = sectio_iterator_proximum(&iter)) != NIHIL)
{
    /* Do stuff */
}
/* Now you're done - no cleanup needed */
```

The `finis` function exists for completeness, but you'll probably never use it.

## Why doesn't sectio_invenire() take a combined name like "__TEXT.__text"?

Because the Mach-O format stores them separately, and keeping them separate in the API makes the contract clearer. You're specifying a segment AND a section. Two parameters.

If we took a combined string, we'd have to parse it (what's the delimiter? a dot? what if a section name contains a dot?), and that's just unnecessary complexity.

Be explicit. It's better.

## Can I iterate only sections in a specific segment?

Not directly. But you can iterate everything and filter:

```c
SectioIterator iter = sectio_iterator_initium(macho);
Sectio* sectio;

dum ((sectio = sectio_iterator_proximum(&iter)) != NIHIL)
{
    chorda seg = sectio_nomen_segmenti(sectio, p);

    si (chorda_aequalis(seg, chorda_ex_literis("__TEXT", p)))
    {
        /* This is a __TEXT section */
    }
}
```

Is this inefficient? Barely. The iterator is already walking load commands anyway. The string comparison is negligible.

## What happens if I pass NIHIL to these functions?

They return safe defaults:
- Query functions return 0 or NIHIL
- `sectio_nomen()` returns an empty chorda
- `sectio_datum()` returns an empty chorda
- The program doesn't crash

This is defensive programming. If you pass garbage input, you get garbage output, but at least it's SAFE garbage.

## Why is this library 64-bit only?

Because it's 2025 and nobody cares about 32-bit macOS anymore. Apple dropped 32-bit support in macOS 10.15 (2019). If you're running a 32-bit Mac, you have bigger problems than this library not supporting you.

Adding 32-bit support would mean doubling the code (handling both `LC_SEGMENT` and `LC_SEGMENT_64`), and for what? Nobody would use it.

## Can I use this library on Linux or Windows?

Technically yes, if you have Mach-O files on those platforms (which happens - you might be analyzing iOS app bundles on a Linux server).

But the library doesn't do any endian swapping, so it only works with little-endian Mach-O files. Apple hasn't shipped big-endian hardware since the PowerPC days, so this is fine.

## What's the performance cost of calling sectio_nomen() repeatedly?

Each call allocates a new string from the piscina. So if you're calling it in a tight loop, you're allocating memory each time.

If that bothers you, call it once and cache the result:
```c
chorda name = sectio_nomen(text, p);
/* Now use 'name' repeatedly without reallocating */
```

But honestly, piscina allocation is fast (bump pointer), so this usually doesn't matter.

## Why are the error messages in English instead of Latin?

Because I'm not a complete psychopath. The API is in Latin for consistency with the rest of the codebase. Error messages are in English because they're for humans, and humans don't speak Latin.

Well, you're learning Latin now. But error messages are still in English. Deal with it.

---

That's everything. If you have more questions, read the code. It's 533 lines. You can handle it.

Now go parse some binaries.
