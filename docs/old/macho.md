# Macho: Parsing Mach-O Binaries Without Losing Your Mind

Alright, so you need to parse Mach-O binaries. Maybe you're writing a debugger. Maybe you're doing binary analysis. Maybe you're building dev tools. Whatever the reason, you're on macOS or iOS and you need to crack open executables, libraries, frameworks - the stuff that actually runs.

Here's what you're NOT gonna do: read the official Mach-O documentation, cry for three hours, give up, and use `otool` from a subprocess like a coward.

Here's what you ARE gonna do: use **macho** and get on with your life.

**Mach-O** - Mach Object file format. The executable format used on macOS, iOS, watchOS, tvOS, and basically everything Apple makes. It's what happens when you compile a C program on a Mac. The `.o` object files? Mach-O. The final executable? Mach-O. Framework libraries? Mach-O. System libraries? Mach-O.

This library gives you:
- Parse Mach-O files from disk or memory
- Read the main header (CPU type, file type, etc.)
- Iterate through load commands
- Handle 64-bit and 32-bit binaries
- Handle fat/universal binaries (multiple architectures in one file)
- Extract raw load command data for specialized parsing
- Error reporting when things go wrong

All using piscina for memory management because we're civilized.

## What Even Is Mach-O? (Quick Tour)

A Mach-O file has this structure:

```
┌─────────────────────┐
│      Header         │  CPU type, file type, how many load commands
├─────────────────────┤
│   Load Command 1    │  "Load this library"
├─────────────────────┤
│   Load Command 2    │  "Map this segment to memory"
├─────────────────────┤
│   Load Command 3    │  "Here's the symbol table"
├─────────────────────┤
│        ...          │  More commands
├─────────────────────┤
│   Segment Data      │  Actual code and data
│   (code, strings,   │
│    etc.)            │
└─────────────────────┘
```

**Header**: Tells you what CPU this binary runs on (x86_64, ARM64, etc.), what kind of file it is (executable, library, object file), and how many load commands follow.

**Load Commands**: Instructions to the dynamic linker. "Load this library." "Map this segment." "Here's where the entry point is." Each command has a type and a size. You iterate through them to understand what the binary needs.

**Segment Data**: The actual machine code, string literals, global variables, etc. The load commands tell you where to find it and what it means.

### Fat Binaries (Universal Binaries)

Sometimes you have one file that contains multiple architectures. Like an executable that runs on both Intel Macs and Apple Silicon Macs. This is called a **fat binary** or **universal binary**.

Structure:

```
┌─────────────────────┐
│   Fat Header        │  "This file contains 2 architectures"
├─────────────────────┤
│  Architecture 1     │  x86_64 Mach-O starts at offset 16384
├─────────────────────┤
│  Architecture 2     │  ARM64 Mach-O starts at offset 2097152
├─────────────────────┤
│  x86_64 Mach-O      │  Complete Mach-O binary
│  (offset 16384)     │
├─────────────────────┤
│  ARM64 Mach-O       │  Another complete Mach-O binary
│  (offset 2097152)   │
└─────────────────────┘
```

Each architecture is a complete, standalone Mach-O binary. The fat header just says "here's where each one starts."

This library handles both single-architecture Mach-Os and fat binaries. You don't have to think about which one you're dealing with most of the time.

## Latin Crash Course (You Know The Drill)

Latin vocabulary for Mach-O parsing:

- **macho** = Mach object (abbreviated, not technically Latin but neither is "Mach")
- **filum** = file, thread
- **imago** = image (one architecture in a binary)
- **mandatum** = command, order (load command)
- **onustum** = loaded, burdened (mandatum onustum = load command)
- **crassus** = fat, thick (fat binary)
- **magica** = magic (magic number at start of file)
- **genus** = type, kind
- **processoris** = of processor (genitive)
- **subgenus** = subtype
- **numerus** = number
- **mensura** = size, measure
- **datum** = data (neuter singular, plural is *data*)
- **iterator** = iterator (same in Latin)
- **initium** = beginning
- **proximum** = next
- **finis** = end
- **aperire** = to open
- **existit** = it exists
- **architectura** = architecture

So `macho_iterator_mandatorum_initium()` = "macho_iterator_of_commands_beginning". The genitive plural (*mandatorum*) indicates possession/relationship.

## Opening A Mach-O File

Two ways: from a file path, or from memory.

### From File

```c
Piscina* piscina = piscina_generare_dynamicum("macho", 1024 * 1024);

MachO* macho = macho_aperire("./my_program", piscina);
si (!macho)
{
    constans character* error = macho_error_recens();
    imprimere("Error: %s\n", error ? error : "unknown");
    exire(I);
}
```

**`macho_aperire(via, piscina)`** - "macho_open"

Opens and parses a Mach-O file from disk. Returns `NIHIL` on failure (check `macho_error_recens()`).

**Important**: This function reads the entire file into memory (using `filum_legere_totum()`). For huge binaries (like Xcode itself), this might be expensive. But for typical executables and libraries, it's fine.

### From Memory

If you already have the binary data in memory:

```c
chorda binary_data = load_binary_from_somewhere(piscina);

MachO* macho = macho_ex_memoria(binary_data.datum, binary_data.mensura, piscina);
si (!macho)
{
    imprimere("Parse failed\n");
    exire(I);
}
```

**`macho_ex_memoria(datum, mensura, piscina)`** - "macho_from_memory"

Parses Mach-O data from memory. Doesn't copy the data - just stores a pointer to it. Make sure the data stays valid as long as you're using the MachO object.

## Reading The Header

Once you have a MachO object, query the header:

```c
/* CPU type */
i32 cpu_type = macho_genus_processoris(macho);
si (cpu_type == MACHO_CPU_TYPE_X86_64)
{
    imprimere("Intel x86_64\n");
}
alioquin si (cpu_type == MACHO_CPU_TYPE_ARM64)
{
    imprimere("Apple Silicon ARM64\n");
}

/* File type */
i32 file_type = macho_genus_filum(macho);
si (file_type == MACHO_GENUS_EXECUTABILE)
{
    imprimere("Executable\n");
}
alioquin si (file_type == MACHO_GENUS_DYLIB)
{
    imprimere("Dynamic library\n");
}

/* Is it 64-bit? */
b32 is_64 = macho_est_64bit(macho);
imprimere("64-bit: %s\n", is_64 ? "yes" : "no");

/* How many load commands? */
i32 num_commands = macho_numerus_mandatorum(macho);
imprimere("Load commands: %d\n", num_commands);
```

### CPU Types

The library defines constants for common CPU types:

- `MACHO_CPU_TYPE_X86` - Intel 32-bit (rare now)
- `MACHO_CPU_TYPE_X86_64` - Intel 64-bit
- `MACHO_CPU_TYPE_ARM` - ARM 32-bit (iOS before 2017)
- `MACHO_CPU_TYPE_ARM64` - ARM 64-bit (Apple Silicon, modern iOS)
- `MACHO_CPU_TYPE_POWERPC` - PowerPC (ancient, pre-2006)
- `MACHO_CPU_TYPE_POWERPC64` - PowerPC 64-bit

### File Types

- `MACHO_GENUS_OBJECT` - Object file (`.o`)
- `MACHO_GENUS_EXECUTABILE` - Executable
- `MACHO_GENUS_FVMLIB` - Fixed VM library (obsolete)
- `MACHO_GENUS_CORE` - Core dump
- `MACHO_GENUS_PRELOAD` - Preloaded executable
- `MACHO_GENUS_DYLIB` - Dynamic library (`.dylib`)
- `MACHO_GENUS_DYLINKER` - Dynamic linker
- `MACHO_GENUS_BUNDLE` - Bundle (`.bundle`)
- `MACHO_GENUS_DYLIB_STUB` - Stub library
- `MACHO_GENUS_DSYM` - Debug symbols (`.dSYM`)
- `MACHO_GENUS_KEXT_BUNDLE` - Kernel extension

Most of the time you care about `EXECUTABILE` (programs), `DYLIB` (shared libraries), or `BUNDLE` (plugins).

## Iterating Through Load Commands

Load commands are the meat of the Mach-O format. They tell the dynamic linker what to do. To access them, you iterate:

```c
MachoIteratorMandatum iter = macho_iterator_mandatorum_initium(macho);
MandatumOnustum* cmd;

dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
{
    i32 cmd_type = mandatum_genus(cmd);
    memoriae_index cmd_size = mandatum_magnitudo(cmd);

    imprimere("Command type: 0x%x, size: %zu\n", cmd_type, cmd_size);

    /* Process the command... */
}
```

**`macho_iterator_mandatorum_initium(macho)`** - "macho_iterator_of_commands_beginning"

Creates an iterator positioned at the first load command.

**`macho_iterator_mandatorum_proximum(&iter)`** - "macho_iterator_of_commands_next"

Advances the iterator and returns the next load command. Returns `NIHIL` when done.

Each load command has:
- **Type** (`mandatum_genus()`) - what kind of command (segment, library dependency, symbol table, etc.)
- **Size** (`mandatum_magnitudo()`) - how many bytes the command takes
- **Data** (`mandatum_datum()`) - pointer to the raw command data

### Load Command Types

There are dozens of load command types. Here are the important ones:

**Segments**:
- `MACHO_LC_SEGMENT` - 32-bit segment
- `MACHO_LC_SEGMENT_64` - 64-bit segment (maps a region of the file into memory)

**Libraries**:
- `MACHO_LC_LOAD_DYLIB` - Load a dynamic library
- `MACHO_LC_LOAD_WEAK_DYLIB` - Load library weakly (okay if it doesn't exist)
- `MACHO_LC_REEXPORT_DYLIB` - Re-export symbols from another library

**Symbols**:
- `MACHO_LC_SYMTAB` - Symbol table location
- `MACHO_LC_DYSYMTAB` - Dynamic symbol table
- `MACHO_LC_DYLD_INFO` - Dyld info (binding, exports, etc.)
- `MACHO_LC_DYLD_INFO_ONLY` - Same but dyld-only

**Entry Point**:
- `MACHO_LC_MAIN` - Entry point offset (modern)
- `MACHO_LC_UNIXTHREAD` - Initial thread state (old style)

**Other**:
- `MACHO_LC_UUID` - Unique identifier
- `MACHO_LC_VERSION_MIN_MACOSX` - Minimum macOS version
- `MACHO_LC_VERSION_MIN_IPHONEOS` - Minimum iOS version
- `MACHO_LC_SOURCE_VERSION` - Source version
- `MACHO_LC_BUILD_VERSION` - Build version and SDK

### Parsing Specific Load Commands

The library gives you raw pointers to command data. You cast them to the appropriate structs. Example - parsing a SEGMENT_64 command:

```c
MachoIteratorMandatum iter = macho_iterator_mandatorum_initium(macho);
MandatumOnustum* cmd;

dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
{
    si (mandatum_genus(cmd) == MACHO_LC_SEGMENT_64)
    {
        /* Cast to segment_64 structure */
        structura segment_command_64
        {
            i32 cmd;
            i32 cmdsize;
            character segname[16];
            u64 vmaddr;
            u64 vmsize;
            u64 fileoff;
            u64 filesize;
            i32 maxprot;
            i32 initprot;
            i32 nsects;
            i32 flags;
        };

        constans structura segment_command_64* seg =
            (constans structura segment_command_64*)mandatum_datum(cmd);

        imprimere("Segment: %s\n", seg->segname);
        imprimere("  VM address: 0x%llx\n", seg->vmaddr);
        imprimere("  VM size: %llu\n", seg->vmsize);
    }
}
```

You define the struct yourself (or include `<mach-o/loader.h>` if you want to use Apple's definitions). The library doesn't define every possible struct because there are so many command types and most programs only care about a few.

## Fat Binaries (Universal Binaries)

If you have a fat binary (multiple architectures in one file), you use MachoFilum instead of MachO:

```c
MachoFilum* filum = macho_filum_aperire("./universal_binary", piscina);
si (!filum)
{
    imprimere("Failed to open\n");
    exire(I);
}

i32 num_images = macho_filum_numerus_imago(filum);
imprimere("This binary contains %d architectures\n", num_images);

per (i32 i = ZEPHYRUM; i < num_images; i++)
{
    MachO* imago = macho_filum_imago(filum, i);
    i32 cpu_type = macho_genus_processoris(imago);

    imprimere("Architecture %d: CPU type 0x%x\n", i, cpu_type);
}
```

**`macho_filum_aperire(via, piscina)`** - "macho_file_open"

Opens a Mach-O file, which might be single-architecture or fat. Returns a MachoFilum container.

**`macho_filum_numerus_imago(filum)`** - "macho_file_number_of_images"

Returns how many architectures are in the file. Single-architecture files return 1.

**`macho_filum_imago(filum, index)`** - "macho_file_image"

Gets the MachO object for a specific architecture. Index 0 is the first architecture.

**Convenience Functions**: If you know your file is single-architecture, use `macho_aperire()` instead of `macho_filum_aperire()`. It does the same thing but exits with an error if the file is fat.

### Current Limitation: Fat Binaries With Multiple Architectures

**WARNING**: The current implementation only supports fat binaries with **one architecture**. If you try to load a fat binary with multiple architectures (like x86_64 + ARM64), it will print an error and exit.

Why? Because I haven't needed multi-arch support yet. When I do, I'll implement it. For now, single-arch binaries work fine.

If you need multi-arch, you have options:
1. Implement it yourself (the fat header parsing is already there, you just need to handle multiple images)
2. Use `lipo -thin` to extract one architecture before loading
3. Wait for me to implement it

## Memory Management

Everything comes from the piscina:
- The MachO or MachoFilum struct
- The iterator
- The MandatumOnustum structs returned by the iterator

The actual binary data (what you read from the file or passed to `ex_memoria`) is also allocated from the piscina (when you use `aperire()`), or you manage it yourself (when you use `ex_memoria()`).

When you destroy the piscina, everything is freed.

**Important**: The library stores pointers into the binary data. Don't modify or free the binary data while you're using the MachO object.

Typical pattern:

```c
Piscina* piscina = piscina_generare_dynamicum("binary_analysis", 1024 * 1024);

MachO* macho = macho_aperire("./some_binary", piscina);

/* Analyze the binary... */

MachoIteratorMandatum iter = macho_iterator_mandatorum_initium(macho);
MandatumOnustum* cmd;
dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
{
    /* Process commands... */
}

/* Done */
piscina_destruere(piscina);
```

All allocations (the file data, the MachO struct, the iterator, the command structs) are freed when you destroy the piscina.

## Error Handling

Functions that can fail return `NIHIL`. Check for errors with `macho_error_recens()`:

```c
MachO* macho = macho_aperire("./binary", piscina);
si (!macho)
{
    constans character* error = macho_error_recens();
    si (error)
    {
        imprimere("Error: %s\n", error);
    }
    alioquin
    {
        imprimere("Unknown error\n");
    }
    exire(I);
}
```

**`macho_error_recens()`** - "macho_error_recent"

Returns a pointer to an error message if there was an error, or `NIHIL` if no error.

**Not thread-safe**: There's one global error buffer. If you parse Mach-O files from multiple threads, they'll clobber each other's errors. Don't do that.

Common errors:
- `"Magica Mach-O invalida"` - Not a valid Mach-O file (wrong magic number)
- `"Filum nimis parvum"` - File is too small to be a valid Mach-O
- `"fopen fracta"` - Couldn't open the file (doesn't exist, no permission, etc.)
- `"piscina_allocare fracta"` - Out of memory in the piscina

## Common Use Cases

### Use Case 1: Check CPU Architecture

You want to know if a binary is Intel or ARM:

```c
MachO* macho = macho_aperire("./binary", piscina);
si (!macho)
{
    imprimere("Not a valid Mach-O\n");
    exire(I);
}

i32 cpu = macho_genus_processoris(macho);
si (cpu == MACHO_CPU_TYPE_ARM64)
{
    imprimere("Apple Silicon binary\n");
}
alioquin si (cpu == MACHO_CPU_TYPE_X86_64)
{
    imprimere("Intel binary\n");
}
alioquin
{
    imprimere("Other architecture: 0x%x\n", cpu);
}
```

### Use Case 2: List All Imported Libraries

Find all LC_LOAD_DYLIB commands:

```c
MachoIteratorMandatum iter = macho_iterator_mandatorum_initium(macho);
MandatumOnustum* cmd;

imprimere("Imported libraries:\n");

dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
{
    si (mandatum_genus(cmd) == MACHO_LC_LOAD_DYLIB)
    {
        /* dylib_command structure */
        structura dylib_command
        {
            i32 cmd;
            i32 cmdsize;
            structura
            {
                i32 offset;  /* Offset to library path string */
                i32 timestamp;
                i32 current_version;
                i32 compatibility_version;
            } dylib;
        };

        constans structura dylib_command* dylib_cmd =
            (constans structura dylib_command*)mandatum_datum(cmd);

        /* Library path is at offset from command start */
        constans character* lib_path =
            (constans character*)mandatum_datum(cmd) + dylib_cmd->dylib.offset;

        imprimere("  %s\n", lib_path);
    }
}
```

### Use Case 3: Find The Entry Point

Look for LC_MAIN:

```c
MachoIteratorMandatum iter = macho_iterator_mandatorum_initium(macho);
MandatumOnustum* cmd;

dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
{
    si (mandatum_genus(cmd) == MACHO_LC_MAIN)
    {
        structura entry_point_command
        {
            i32 cmd;
            i32 cmdsize;
            u64 entryoff;  /* File offset of entry point */
            u64 stacksize;
        };

        constans structura entry_point_command* entry =
            (constans structura entry_point_command*)mandatum_datum(cmd);

        imprimere("Entry point at file offset: 0x%llx\n", entry->entryoff);
        frange;
    }
}
```

### Use Case 4: Extract Segment Data

Find __TEXT segment and read code:

```c
MachoIteratorMandatum iter = macho_iterator_mandatorum_initium(macho);
MandatumOnustum* cmd;

dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
{
    si (mandatum_genus(cmd) == MACHO_LC_SEGMENT_64)
    {
        structura segment_command_64
        {
            i32 cmd;
            i32 cmdsize;
            character segname[16];
            u64 vmaddr;
            u64 vmsize;
            u64 fileoff;
            u64 filesize;
            /* ... */
        };

        constans structura segment_command_64* seg =
            (constans structura segment_command_64*)mandatum_datum(cmd);

        si (strncmp(seg->segname, "__TEXT", 6) == 0)
        {
            imprimere("Found __TEXT segment\n");
            imprimere("  File offset: %llu\n", seg->fileoff);
            imprimere("  File size: %llu\n", seg->filesize);

            /* Access segment data */
            constans i8* segment_data = macho->datum + seg->fileoff;
            /* ... process segment_data ... */

            frange;
        }
    }
}
```

**Warning**: Make sure you bounds-check. The file might be malformed. Always verify that `fileoff + filesize` doesn't exceed the total file size.

## Common Mistakes

### Mistake 1: Not Checking For Errors

```c
/* BAD */
MachO* macho = macho_aperire("./binary", piscina);
i32 cpu = macho_genus_processoris(macho);  /* CRASH if macho is NIHIL */
```

Always check if the return value is `NIHIL`:

```c
/* GOOD */
MachO* macho = macho_aperire("./binary", piscina);
si (!macho)
{
    imprimere("Error: %s\n", macho_error_recens());
    exire(I);
}
```

### Mistake 2: Using Data After Destroying The Piscina

```c
/* BAD */
Piscina* p = piscina_generare_dynamicum("macho", 1024 * 1024);
MachO* macho = macho_aperire("./binary", p);

MandatumOnustum* cmd = get_some_command(macho);
piscina_destruere(p);

i32 cmd_type = mandatum_genus(cmd);  /* CRASH - cmd is freed */
```

Don't destroy the piscina until you're done with everything.

### Mistake 3: Assuming Single Architecture

```c
/* BAD */
MachO* macho = macho_aperire("./universal_binary", piscina);
/* This might exit with an error if the file is a fat binary */
```

If the file might be fat, use `macho_filum_aperire()` instead:

```c
/* GOOD */
MachoFilum* filum = macho_filum_aperire("./binary", piscina);
MachO* first_arch = macho_filum_imago(filum, ZEPHYRUM);
```

### Mistake 4: Not Bounds-Checking Segment Offsets

```c
/* BAD */
constans i8* segment_data = macho->datum + seg->fileoff;
/* What if fileoff is past the end of the file? */
```

Always verify offsets:

```c
/* GOOD */
si (seg->fileoff + seg->filesize > file_size)
{
    imprimere("Invalid segment offset\n");
    exire(I);
}
constans i8* segment_data = macho->datum + seg->fileoff;
```

### Mistake 5: Modifying The Binary Data

```c
/* BAD */
MachO* macho = macho_ex_memoria(data, size, piscina);
data[0] = 0xFF;  /* Corrupting the magic number! */
```

The MachO object stores pointers into your data. Don't modify it.

### Mistake 6: Forgetting That Load Commands Are Variable-Size

```c
/* BAD - Treating commands as a fixed-size array */
constans i8* cmd_ptr = macho->datum + macho->offset_mandatorum;
per (i32 i = ZEPHYRUM; i < num_commands; i++)
{
    constans _MandatumOnustumHeader* cmd = (_MandatumOnustumHeader*)cmd_ptr;
    cmd_ptr += magnitudo(_MandatumOnustumHeader);  /* WRONG */
}
```

Each command has its own size (`cmdsize`). Use the iterator, or if you're doing it manually, advance by `cmd->cmdsize`:

```c
/* GOOD */
constans i8* cmd_ptr = macho->datum + macho->offset_mandatorum;
per (i32 i = ZEPHYRUM; i < num_commands; i++)
{
    constans _MandatumOnustumHeader* cmd = (_MandatumOnustumHeader*)cmd_ptr;
    /* Process cmd... */
    cmd_ptr += cmd->mensura;  /* Advance by command size */
}
```

Or just use the iterator and don't think about it.

---

## API Reference

### Opening Files

#### `macho_aperire()`

```c
MachO* macho_aperire(
    constans character* via,
               Piscina* piscina
);
```

Opens and parses a single-architecture Mach-O file.

**Parameters:**
- `via`: File path
- `piscina`: Arena allocator

**Returns:**
- Pointer to MachO on success
- `NIHIL` on error (check `macho_error_recens()`)

**Notes:**
- Reads entire file into memory
- Exits with error if file is a fat binary with multiple architectures

**Example:**
```c
MachO* macho = macho_aperire("./program", piscina);
si (!macho)
{
    imprimere("Error: %s\n", macho_error_recens());
}
```

---

#### `macho_ex_memoria()`

```c
MachO* macho_ex_memoria(
       constans i8* datum,
    memoriae_index  mensura,
           Piscina* piscina
);
```

Parses a single-architecture Mach-O from memory.

**Parameters:**
- `datum`: Binary data
- `mensura`: Data size in bytes
- `piscina`: Arena allocator

**Returns:**
- Pointer to MachO on success
- `NIHIL` on error

**Notes:**
- Doesn't copy data - stores pointer
- Data must remain valid while using MachO object

**Example:**
```c
chorda data = load_binary(piscina);
MachO* macho = macho_ex_memoria(data.datum, data.mensura, piscina);
```

---

#### `macho_filum_aperire()`

```c
MachoFilum* macho_filum_aperire(
    constans character* via,
               Piscina* piscina
);
```

Opens and parses a Mach-O file (handles fat binaries).

**Parameters:**
- `via`: File path
- `piscina`: Arena allocator

**Returns:**
- Pointer to MachoFilum on success
- `NIHIL` on error

**Notes:**
- Use this if file might be fat
- Returns container with one or more architectures

**Example:**
```c
MachoFilum* filum = macho_filum_aperire("./universal", piscina);
i32 num = macho_filum_numerus_imago(filum);
```

---

#### `macho_filum_ex_memoria()`

```c
MachoFilum* macho_filum_ex_memoria(
       constans i8* datum,
    memoriae_index  mensura,
           Piscina* piscina
);
```

Parses a Mach-O file from memory (handles fat binaries).

**Parameters:**
- `datum`: Binary data
- `mensura`: Data size
- `piscina`: Arena allocator

**Returns:**
- Pointer to MachoFilum on success
- `NIHIL` on error

---

### Querying Header (MachO)

#### `macho_genus_processoris()`

```c
i32 macho_genus_processoris(
    constans MachO* macho
);
```

Gets CPU type.

**Returns:**
- CPU type constant (MACHO_CPU_TYPE_*)
- 0 if macho is NIHIL

**Example:**
```c
i32 cpu = macho_genus_processoris(macho);
si (cpu == MACHO_CPU_TYPE_ARM64)
{
    imprimere("Apple Silicon\n");
}
```

---

#### `macho_subgenus_processoris()`

```c
i32 macho_subgenus_processoris(
    constans MachO* macho
);
```

Gets CPU subtype.

**Returns:**
- CPU subtype
- 0 if macho is NIHIL

---

#### `macho_genus_filum()`

```c
i32 macho_genus_filum(
    constans MachO* macho
);
```

Gets file type.

**Returns:**
- File type constant (MACHO_GENUS_*)
- 0 if macho is NIHIL

**Example:**
```c
si (macho_genus_filum(macho) == MACHO_GENUS_DYLIB)
{
    imprimere("This is a dynamic library\n");
}
```

---

#### `macho_numerus_mandatorum()`

```c
i32 macho_numerus_mandatorum(
    constans MachO* macho
);
```

Gets number of load commands.

**Returns:**
- Number of load commands
- 0 if macho is NIHIL

---

#### `macho_est_64bit()`

```c
b32 macho_est_64bit(
    constans MachO* macho
);
```

Checks if binary is 64-bit.

**Returns:**
- `VERUM` if 64-bit
- `FALSUM` if 32-bit or macho is NIHIL

---

### Querying Container (MachoFilum)

#### `macho_filum_numerus_imago()`

```c
i32 macho_filum_numerus_imago(
    constans MachoFilum* filum
);
```

Gets number of architectures in file.

**Returns:**
- Number of architectures (1 for single-arch)
- 0 if filum is NIHIL

---

#### `macho_filum_imago()`

```c
MachO* macho_filum_imago(
    constans MachoFilum* filum,
                    i32  index
);
```

Gets a specific architecture.

**Parameters:**
- `filum`: File container
- `index`: Architecture index (0-based)

**Returns:**
- Pointer to MachO for that architecture
- `NIHIL` if index out of range

---

#### `macho_filum_genus_processoris()`

```c
i32 macho_filum_genus_processoris(
    constans MachoFilum* filum,
                    i32  index
);
```

Gets CPU type for a specific architecture.

**Parameters:**
- `filum`: File container
- `index`: Architecture index

**Returns:**
- CPU type
- 0 if index invalid

---

#### `macho_filum_subgenus_processoris()`

```c
i32 macho_filum_subgenus_processoris(
    constans MachoFilum* filum,
                    i32  index
);
```

Gets CPU subtype for a specific architecture.

---

### Iterating Load Commands

#### `macho_iterator_mandatorum_initium()`

```c
MachoIteratorMandatum macho_iterator_mandatorum_initium(
    constans MachO* macho
);
```

Creates iterator at first load command.

**Returns:**
- Iterator positioned at start

**Example:**
```c
MachoIteratorMandatum iter = macho_iterator_mandatorum_initium(macho);
```

---

#### `macho_iterator_mandatorum_proximum()`

```c
MandatumOnustum* macho_iterator_mandatorum_proximum(
    MachoIteratorMandatum* iter
);
```

Advances iterator to next load command.

**Parameters:**
- `iter`: Iterator to advance

**Returns:**
- Pointer to load command
- `NIHIL` when done

**Example:**
```c
MandatumOnustum* cmd;
dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
{
    i32 type = mandatum_genus(cmd);
    /* Process command... */
}
```

---

#### `macho_iterator_mandatorum_finis()`

```c
b32 macho_iterator_mandatorum_finis(
    MachoIteratorMandatum* iter
);
```

Checks if iterator is at end.

**Returns:**
- `VERUM` if at end
- `FALSUM` if more commands remain

---

### Querying Load Commands

#### `mandatum_genus()`

```c
i32 mandatum_genus(
    constans MandatumOnustum* mandatum
);
```

Gets load command type.

**Returns:**
- Command type constant (MACHO_LC_*)
- 0 if mandatum is NIHIL

---

#### `mandatum_magnitudo()`

```c
memoriae_index mandatum_magnitudo(
    constans MandatumOnustum* mandatum
);
```

Gets load command size.

**Returns:**
- Size in bytes
- 0 if mandatum is NIHIL

---

#### `mandatum_datum()`

```c
constans vacuum* mandatum_datum(
    constans MandatumOnustum* mandatum
);
```

Gets pointer to raw command data.

**Returns:**
- Pointer to command data (cast to appropriate struct)
- `NIHIL` if mandatum is NIHIL

**Example:**
```c
constans structura segment_command_64* seg =
    (constans structura segment_command_64*)mandatum_datum(cmd);
```

---

### Error Handling

#### `macho_error_recens()`

```c
constans character* macho_error_recens(vacuum);
```

Gets most recent error message.

**Returns:**
- Pointer to error string if there was an error
- `NIHIL` if no error

**Example:**
```c
si (!macho)
{
    imprimere("Error: %s\n", macho_error_recens());
}
```

---

## FAQ

### Q: Why would I parse Mach-O files?

Binary analysis, reverse engineering, debugging tools, dev tools, malware analysis, understanding how your program is structured, extracting symbols, finding dependencies, etc.

### Q: Can I modify a Mach-O and write it back?

Not with this library. This is a read-only parser. If you want to modify binaries, you need more than just parsing - you need relocation, codesigning, etc. Use `install_name_tool`, `codesign`, or similar tools.

### Q: Why doesn't this support big-endian?

Because Apple Silicon and Intel are both little-endian. Big-endian Mach-O files would be from ancient PowerPC Macs (pre-2006). If you encounter one, the library exits with an error. If you actually need to parse them, implement byte swapping.

### Q: What about 32-bit binaries?

Fully supported. The library handles both 64-bit (MACHO_MAGICA_64) and 32-bit (MACHO_MAGICA_32) binaries. It sets the `est_64bit` flag so you know which one you're dealing with.

### Q: Can I parse iOS binaries?

Yes. iOS apps use Mach-O format. Same format, different platform constants. The library parses them fine.

### Q: What about kernel extensions (.kext)?

They're Mach-O bundles. The library can parse them, but you might need special handling for the bundle structure (Info.plist, etc.). The Mach-O part works fine.

### Q: Why do I need to define my own structs for load commands?

Because there are dozens of load command types and most programs only care about a few. Including every possible struct would be huge. Define what you need, or include Apple's `<mach-o/loader.h>`.

### Q: Is this compatible with Apple's Mach-O format?

Yes. It parses the format exactly as Apple defines it. The constants (CPU types, command types, etc.) match Apple's definitions.

### Q: What's the performance like?

Fast. The entire file is in memory, so iteration is just pointer arithmetic. No I/O once the file is loaded. For a typical executable, parsing takes microseconds.

### Q: Can I use this from multiple threads?

The error handling is not thread-safe (global buffer). Parsing itself is fine if each thread has its own piscina and doesn't share MachO objects. But why would you parse binaries from multiple threads?

### Q: What if the file is corrupted?

The library does basic validation (magic number, header size, command sizes). It'll catch obviously broken files. For subtle corruption, you might get garbage data. If you're parsing untrusted binaries, add more validation.

### Q: How do I find where a function's code is?

You need to:
1. Find the symbol table (LC_SYMTAB command)
2. Parse the symbol table to find the function's address
3. Find which segment contains that address
4. Calculate the file offset from the segment's file offset and the symbol's address

This library gives you the tools (load command iteration, raw data access) but doesn't implement high-level symbol lookup. That's a whole other library.

---

That's Mach-O parsing. Open files, read headers, iterate commands, extract data. If you need more than that, you're writing a linker or something and you should already know what you're doing.