# Filum: File I/O That Doesn't Make You Want To Scream

Look, I get it. You need to read files. Write files. Maybe copy them around. You've done this before in other languages and you're thinking "how hard could this be in C?"

Oh, you sweet summer child.

Here's what's about to happen if you try to roll your own file I/O in C89: You're gonna open a file, forget to check if it's NULL, crash. Then you're gonna remember to check NULL, but forget to close the file, leak file descriptors. Then you're gonna remember to close it, but do it in the wrong order and corrupt your data. Then you're gonna get the order right but forget to handle partial reads. Then—

You know what? Forget it. Use **filum**.

**Filum** - Latin for "thread" or "string," but here it means "file." And before you ask, no, I don't know why they didn't call it "file" in Latin. They just didn't. That's *filum*.

This library gives you:
- Read whole files into memory (one function call, done)
- Read files line-by-line without losing your mind
- Write files, append to files
- Copy, move, delete files
- Check if files exist, get their size
- Actual error messages instead of mysterious crashes

All using your piscina (arena allocator) so you don't have to track individual frees. Because we're civilized here.

## How File I/O Actually Works (The Stuff You Forgot)

Real quick, because I know you think you know this but you probably don't.

When you read a file, the OS gives you a **file descriptor** - basically a handle that says "yeah, you can read from this thing." You read bytes from it. When you're done, you close it. If you don't close it, your program can only open so many files before the OS says "that's enough" and your next `fopen()` returns NULL.

When you write a file, same deal. Open, write, close. The "write" part might buffer in memory before hitting disk. That's why you close - it flushes the buffer.

There are three modes we care about:
- **Read (`"r"` or `"rb"`)**: Open existing file for reading. File must exist.
- **Write (`"w"` or `"wb"`)**: Create new file or **wipe existing file** and write from scratch.
- **Append (`"a"` or `"ab"`)**: Create new file or open existing file and write at the end.

The `b` means "binary mode" which on Unix doesn't matter but on Windows it means "don't mess with my `\r\n` line endings." Always use binary mode unless you have a specific reason not to.

Text mode? Text mode is for people who enjoy surprises.

## Latin Crash Course (Pay Attention This Time)

Since you're writing C89 in Latin like you're Thomas Aquinas with a compiler, here's vocabulary:

- **filum** = file, thread
- **legere** = to read
- **scribere** = to write
- **aperire** = to open
- **claudere** = to close
- **existit** = it exists (from *existere*, to exist)
- **mensura** = size, measure
- **lector** = reader
- **linea** = line
- **proximam** = next (accusative feminine of *proximus*)
- **numerus** = number
- **versus** = line (literally "a turn," like turning at the end of a line)
- **finis** = end
- **totum** = whole, entire
- **contentum** = content
- **via** = path, way, road
- **delere** = to delete
- **movere** = to move
- **copiare** = to copy
- **appendere** = to append
- **literis** = with letters (ablative plural - means "as text")
- **status** = status, state
- **error** = error (yeah, same in Latin)
- **recens** = recent

Now when you see `filum_legere_totum()` you can translate it as "file_read_whole" and feel smart. You're welcome.

## Reading An Entire File (The Easy Way)

Most of the time you just want the whole file in memory. Config files, small data files, whatever. You don't want to mess around with buffers and loops and checking EOF. You just want the bytes.

```c
chorda contentum = filum_legere_totum("config.txt", piscina);
si (contentum.datum == NIHIL)
{
    /* Error happened, check filum_error_recens() */
    constans character* error = filum_error_recens();
    imprimere("Error: %s\n", error);
    redde FALSUM;
}

/* Use contentum.datum and contentum.mensura */
imprimere("File size: %d bytes\n", contentum.mensura);
```

Let's break this down because there's some Latin happening:

**`chorda`** - This is your string type. It's a struct with two fields:
- `datum` - pointer to the bytes (type `i8*` which is signed char)
- `mensura` - size in bytes (type `i32`)

A `chorda` is **not null-terminated**. You have the length, you don't need a null terminator. This means it can hold binary data with embedded nulls. Very handy.

**`filum_legere_totum(via, piscina)`** - "file read whole"
- `via` is the path (C string)
- `piscina` is your arena allocator

This function:
1. Opens the file in binary read mode
2. Seeks to the end to get the size
3. Seeks back to the beginning
4. Allocates a buffer from your piscina
5. Reads the entire file into that buffer
6. Returns a `chorda` with the data and size

If anything goes wrong, you get back a `chorda` with `datum == NIHIL` and `mensura == 0`. Check `filum_error_recens()` to see what broke.

**Memory management**: The buffer comes from your piscina. When you destroy the piscina, the buffer gets freed. You don't free it manually. That's the whole point.

Here's a more complete example:

```c
Piscina* p = piscina_generare_dynamicum("file_reading", 8192);
si (!p)
{
    imprimere("Failed to create pool\n");
    exire(I);
}

chorda config = filum_legere_totum("/etc/myapp.conf", p);
si (config.datum == NIHIL)
{
    imprimere("Couldn't read config: %s\n", filum_error_recens());
    piscina_destruere(p);
    exire(I);
}

/* Parse the config... */
per (i32 i = ZEPHYRUM; i < config.mensura; i++)
{
    character c = config.datum[i];
    /* ... */
}

/* Done with everything */
piscina_destruere(p);
```

See how simple that is? No manual cleanup of the file buffer. The piscina handles it.

## Checking If Files Exist (Before You Do Something Dumb)

Before you try to read a file, you might want to know if it exists. Saves you an error.

```c
si (filum_existit("config.txt"))
{
    chorda config = filum_legere_totum("config.txt", piscina);
    /* ... */
}
alioquin
{
    imprimere("Config file not found, using defaults\n");
}
```

**`filum_existit(via)`** returns `VERUM` if the file exists, `FALSUM` if it doesn't.

Simple. It just tries to open the file for reading. If that works, file exists. If it fails, file doesn't exist. Then it closes the file and returns.

## Getting File Size (Without Reading The Whole Thing)

Sometimes you need to know how big a file is before you read it. Maybe you want to allocate a specific pool size, or check if it's too big, or whatever.

```c
memoriae_index size = filum_mensura("bigfile.dat");
si (size == ZEPHYRUM)
{
    imprimere("File doesn't exist or is empty\n");
}
alioquin
{
    imprimere("File is %zu bytes\n", size);
}
```

**`filum_mensura(via)`** - "file measure" - returns the size in bytes.

Returns `ZEPHYRUM` (zero) if the file doesn't exist or if there's an error. Also returns zero if the file is actually empty. So if you need to distinguish "doesn't exist" from "exists but empty," use `filum_existit()` first.

This function opens the file, seeks to the end, gets the position (which is the size), closes the file, returns the size. Classic.

## Reading Line By Line (When Files Are Too Big)

Okay, so `filum_legere_totum()` is great but what if your file is huge? What if it's a log file with a million lines and you don't want to load all of that into memory at once?

You use a **FilumLector**.

**Lector** - Latin for "reader." A FilumLector is an opaque struct (you don't see inside it) that tracks:
- The open file handle
- Current line number
- Whether you've hit EOF
- An internal 4096-byte buffer for reading
- Pointer to your piscina

Here's how it works:

```c
FilumLector* lector = filum_lector_aperire("bigfile.log", piscina);
si (!lector)
{
    imprimere("Couldn't open file: %s\n", filum_error_recens());
    redde FALSUM;
}

chorda linea;
dum (filum_lector_lineam_proximam(lector, &linea))
{
    /* Process this line */
    imprimere("Line %d: %.*s\n",
        filum_lector_numerus_versus(lector),
        linea.mensura,
        linea.datum);
}

filum_lector_claudere(lector);
```

Let's break down what's happening:

**`filum_lector_aperire(via, piscina)`** - "file reader open"

Opens the file for reading, allocates a FilumLector from your piscina, initializes it. Returns `NIHIL` if something goes wrong.

**`filum_lector_lineam_proximam(lector, &linea)`** - "file reader line next"

Reads the next line from the file. Returns `VERUM` if it got a line, `FALSUM` if you hit EOF or an error.

The line is returned via the `linea` output parameter. It's a `chorda` - data pointer and length. The line does **not** include the newline characters. Those get stripped.

The line data is allocated from your piscina. You don't free it. When you destroy the piscina, it's gone.

**`filum_lector_numerus_versus(lector)`** - "file reader number of lines"

Returns how many lines you've read so far. Starts at 0, increments each time you successfully read a line.

**`filum_lector_finis(lector)`** - "file reader end"

Returns `VERUM` if you've hit EOF, `FALSUM` otherwise. Useful if you want to check EOF without trying to read another line.

**`filum_lector_claudere(lector)`** - "file reader close"

Closes the file handle. The FilumLector itself doesn't get freed (it came from the piscina) but the file handle is closed so you're not leaking descriptors.

### How Line Reading Actually Works

Here's what happens under the hood, because I know you're gonna ask:

The FilumLector has a 4096-byte internal buffer. When you call `lineam_proximam()`:

1. It reads up to 4096 bytes into the buffer using `fgets()`
2. It looks for a newline (`\n` or `\r` or `\r\n`)
3. If it finds one, it strips the newline characters and returns the line
4. If the line is shorter than 4096 bytes, we're done
5. If the line filled the buffer without a newline, it's a long line - we switch to using ChordaAedificator (dynamic string builder) and keep reading chunks until we hit a newline

This means:
- **Short lines** (< 4096 bytes): One allocation from piscina, very fast
- **Long lines** (> 4096 bytes): Multiple chunks assembled using ChordaAedificator, still works transparently

You don't have to think about any of this. It just handles lines of any length.

### Line Endings: We Handle All The Nonsense

Different systems use different line endings:
- Unix/Linux/Mac: `\n` (LF)
- Windows: `\r\n` (CRLF)
- Old Mac: `\r` (CR)

The line reader strips all of these. It checks the end of each line and removes any `\r` or `\n` characters. So you get clean lines regardless of where the file came from.

### Empty Lines

Empty lines are valid. If you read a line that's just a newline, you get back a `chorda` with `mensura == 0` and `datum == NIHIL`. This is fine. Don't freak out about it.

```c
dum (filum_lector_lineam_proximam(lector, &linea))
{
    si (linea.mensura == ZEPHYRUM)
    {
        imprimere("Empty line\n");
    }
    alioquin
    {
        imprimere("Line: %.*s\n", linea.mensura, linea.datum);
    }
}
```

### Files Without Final Newlines

Some text files don't have a newline at the end of the last line. The line reader handles this. When it hits EOF, it returns whatever it read as a line, even if there's no newline. So you don't lose the last line.

## Writing Files (Overwriting Everything)

You want to write a file from scratch. This **wipes out** any existing file with the same name. If that's not what you want, read the next section about appending.

Two flavors: binary-safe `chorda` or null-terminated string.

### Writing A chorda (Binary Safe)

```c
chorda data;
data.datum   = (i8*)"Hello, world!";
data.mensura = XIII;  /* 13 bytes */

si (!filum_scribere("output.txt", data))
{
    imprimere("Write failed: %s\n", filum_error_recens());
}
```

**`filum_scribere(via, contentum)`** - "file write"

Opens the file in binary write mode (`"wb"`), writes the bytes, closes the file. Returns `VERUM` on success, `FALSUM` on failure.

The `chorda` can contain binary data with embedded nulls. It's binary-safe. The `mensura` field tells it exactly how many bytes to write.

### Writing A C String (Text Mode)

If you're just writing text and you have a null-terminated C string:

```c
constans character* message = "Hello, world!\n";

si (!filum_scribere_literis("output.txt", message))
{
    imprimere("Write failed: %s\n", filum_error_recens());
}
```

**`filum_scribere_literis(via, contentum)`** - "file write with letters"

Opens the file in text write mode (`"w"`), calculates the string length with `strlen()`, writes it, closes the file.

This is convenient when you know you have text and it's null-terminated. But it can't handle binary data with embedded nulls.

## Appending To Files (Without Destroying Them)

Sometimes you want to add to an existing file without wiping it. Log files, for example.

Same two flavors: binary-safe and text.

### Appending A chorda

```c
chorda log_entry;
log_entry.datum   = (i8*)"[ERROR] Something broke\n";
log_entry.mensura = XXIV;  /* 24 bytes */

si (!filum_appendere("app.log", log_entry))
{
    imprimere("Append failed: %s\n", filum_error_recens());
}
```

**`filum_appendere(via, contentum)`** - "file append"

Opens in binary append mode (`"ab"`), writes to the end, closes.

### Appending A C String

```c
si (!filum_appendere_literis("app.log", "[INFO] Starting up\n"))
{
    imprimere("Append failed: %s\n", filum_error_recens());
}
```

**`filum_appendere_literis(via, contentum)`** - "file append with letters"

Opens in text append mode (`"a"`), writes the string, closes.

## Copying Files (The Whole File, No Questions Asked)

You need to copy a file. You could read it into memory and write it back out, but if it's huge that's wasteful. `filum_copiare()` does it efficiently with a 4096-byte buffer:

```c
si (!filum_copiare("source.txt", "destination.txt"))
{
    imprimere("Copy failed: %s\n", filum_error_recens());
}
```

**`filum_copiare(via_fons, via_destinatio)`** - "file copy"
- `fons` = source (literally "fountain")
- `destinatio` = destination

This:
1. Opens source file for binary reading
2. Opens destination file for binary writing (overwrites if exists)
3. Reads 4096 bytes at a time from source
4. Writes to destination
5. Repeats until source is exhausted
6. Closes both files

If the write fails partway through, the destination file might be corrupted. This is not a transactional copy. It's a simple "copy bytes from A to B" operation.

## Moving/Renaming Files (Same Thing, Really)

Moving and renaming are the same operation. You're changing the path.

```c
si (!filum_movere("old_name.txt", "new_name.txt"))
{
    imprimere("Move failed: %s\n", filum_error_recens());
}
```

**`filum_movere(via_vetus, via_nova)`** - "file move"
- `vetus` = old
- `nova` = new (feminine singular)

This calls the C standard library `rename()` function. On Unix, if you move across filesystems, `rename()` might fail. In that case you'd need to copy and delete manually. But within the same filesystem, it's atomic and fast.

## Deleting Files (Carefully)

```c
si (!filum_delere("temporary.txt"))
{
    imprimere("Delete failed: %s\n", filum_error_recens());
}
```

**`filum_delere(via)`** - "file delete"

Calls the C standard library `remove()` function. If the file doesn't exist, this returns failure. If you try to delete a directory, it might fail depending on the OS.

**There is no undo**. The file is gone. Be careful.

## Getting File Status (Metadata)

Sometimes you want to know things about a file beyond just "does it exist" and "how big is it."

```c
FilumStatus status;
si (!filum_status("somefile.txt", &status))
{
    imprimere("Status failed: %s\n", filum_error_recens());
}
alioquin
{
    imprimere("Size: %zu bytes\n", status.mensura);
    imprimere("Is file: %s\n", status.est_filum ? "yes" : "no");
    imprimere("Is directory: %s\n", status.est_directorium ? "yes" : "no");
    imprimere("Can read: %s\n", status.potest_legere ? "yes" : "no");
    imprimere("Can write: %s\n", status.potest_scribere ? "yes" : "no");
}
```

**FilumStatus** is a struct with:
- `mensura` - size in bytes
- `est_filum` - is it a regular file?
- `est_directorium` - is it a directory?
- `potest_legere` - do you have read permission?
- `potest_scribere` - do you have write permission?

This uses the POSIX `stat()` function under the hood, with platform-specific handling for Windows vs Unix.

## Error Handling (So You Know What Broke)

Every function that can fail sets an error message. You retrieve it with:

```c
constans character* error = filum_error_recens();
si (error)
{
    imprimere("Last error: %s\n", error);
}
alioquin
{
    imprimere("No error\n");
}
```

**`filum_error_recens()`** - "file error recent"

Returns a pointer to a static error buffer (256 bytes). If there's no error, returns `NIHIL`.

**Important**: This is **not thread-safe**. There's one global error buffer. If you call a filum function from multiple threads, they'll clobber each other's errors. Don't do that. If you need thread safety, give each thread its own pool and coordinate access to files.

The error gets cleared whenever a function succeeds. So after a successful operation, `filum_error_recens()` returns `NIHIL`.

Typical errors:
- `"fopen fracta"` - couldn't open the file (doesn't exist, no permission, etc)
- `"fread fracta"` - couldn't read (disk error, file changed, who knows)
- `"fwrite fracta"` - couldn't write (disk full, no permission, etc)
- `"via vel piscina est NIHIL"` - you passed NULL for path or pool
- `"piscina_allocare fracta"` - arena allocator couldn't allocate (out of memory)

## Memory Management With Piscina

I'm gonna say this once and only once: **All allocations come from your piscina**.

When you call `filum_legere_totum()`, it allocates the file buffer from your piscina.

When you call `filum_lector_lineam_proximam()`, it allocates the line buffer from your piscina.

When you create a FilumLector, it's allocated from your piscina.

You **never** call `free()` on any of this. When you destroy the piscina with `piscina_destruere()`, everything gets freed at once.

This has implications:

### Pattern 1: Per-Request Pool

You're processing requests. Each request reads some files, does some work, returns a response. Create a pool for the request, do your work, destroy the pool:

```c
vacuum handle_request(Request* req)
{
    Piscina* p = piscina_generare_dynamicum("request", 16384);

    chorda config = filum_legere_totum("config.txt", p);
    chorda input  = filum_legere_totum(req->input_file, p);

    /* Process... */

    piscina_destruere(p);  /* Everything freed */
}
```

### Pattern 2: Long-Lived Pool With Clearing

You're reading a bunch of files in a loop. You don't want to create and destroy a pool every iteration. Create one pool, clear it between iterations:

```c
Piscina* p = piscina_generare_dynamicum("file_processor", 32768);

per (i32 i = ZEPHYRUM; i < file_count; i++)
{
    chorda data = filum_legere_totum(file_paths[i], p);

    /* Process data... */

    piscina_vacare(p);  /* Clear pool for next iteration */
}

piscina_destruere(p);
```

**Warning**: After `piscina_vacare()`, all pointers you got from that pool are invalid. Don't use them. The memory is still there but it's about to get overwritten.

### Pattern 3: Permanent Data

You read a file at startup and need to keep it for the lifetime of the program. Use a long-lived pool:

```c
Piscina* permanent = piscina_generare_dynamicum("permanent", 8192);

chorda config = filum_legere_totum("config.txt", permanent);

/* Keep config around forever, or until program exits */

/* Eventually, when program shuts down: */
piscina_destruere(permanent);
```

## Common Mistakes (That You're Definitely Gonna Make)

### Mistake 1: Using Pointers After Destroying The Pool

```c
/* BAD */
Piscina* p = piscina_generare_dynamicum("temp", 4096);
chorda data = filum_legere_totum("file.txt", p);
piscina_destruere(p);
imprimere("%.*s\n", data.mensura, data.datum);  /* CRASH */
```

After you destroy the pool, `data.datum` points to freed memory. Don't do this.

### Mistake 2: Null-Terminating A chorda

```c
/* BAD */
chorda text = filum_legere_totum("file.txt", p);
text.datum[text.mensura] = '\0';  /* BUFFER OVERFLOW */
imprimere("%s\n", text.datum);
```

The allocated buffer is exactly `text.mensura` bytes. There's no room for a null terminator. If you need a null-terminated string, allocate one extra byte:

```c
/* GOOD */
chorda text = filum_legere_totum("file.txt", p);
character* str = (character*)piscina_allocare(p, text.mensura + I);
memcpy(str, text.datum, text.mensura);
str[text.mensura] = '\0';
imprimere("%s\n", str);
```

Or just use the `%.*s` format specifier which takes a length:

```c
imprimere("%.*s\n", text.mensura, text.datum);
```

### Mistake 3: Forgetting To Check For NIHIL

```c
/* BAD */
chorda data = filum_legere_totum("file.txt", p);
per (i32 i = ZEPHYRUM; i < data.mensura; i++)  /* CRASH if datum is NIHIL */
{
    /* ... */
}
```

Always check if the operation succeeded:

```c
/* GOOD */
chorda data = filum_legere_totum("file.txt", p);
si (data.datum == NIHIL)
{
    imprimere("Failed to read: %s\n", filum_error_recens());
    redde FALSUM;
}

per (i32 i = ZEPHYRUM; i < data.mensura; i++)
{
    /* ... */
}
```

### Mistake 4: Not Closing The FilumLector

```c
/* BAD */
FilumLector* lector = filum_lector_aperire("file.txt", p);
dum (filum_lector_lineam_proximam(lector, &linea))
{
    /* ... */
}
/* Forgot to close! File descriptor leaked! */
```

Always close it:

```c
/* GOOD */
FilumLector* lector = filum_lector_aperire("file.txt", p);
si (!lector) /* ... error ... */

dum (filum_lector_lineam_proximam(lector, &linea))
{
    /* ... */
}

filum_lector_claudere(lector);
```

### Mistake 5: Mixing Up filum_scribere vs filum_scribere_literis

```c
/* BAD */
constans character* text = "Hello";
chorda data;
data.datum = (i8*)text;
data.mensura = V;
filum_scribere_literis("file.txt", (character*)data.datum);  /* Works, but wrong */
```

If you have a `chorda`, use `filum_scribere()`. If you have a C string, use `filum_scribere_literis()`. Don't mix them up. They have different modes (binary vs text) and different behaviors.

---

## API Reference

Alright, here's the boring part where I list every function and what it does. You've probably figured this out already, but for completeness:

### Simple Reading

#### `filum_legere_totum()`

```c
chorda filum_legere_totum(
    constans character* via,
               Piscina* piscina
);
```

Reads an entire file into memory.

**Parameters:**
- `via`: File path (C string)
- `piscina`: Arena allocator to allocate from

**Returns:**
- `chorda` with file contents on success
- `chorda` with `datum == NIHIL` and `mensura == 0` on failure

**Errors:**
- Path or piscina is NULL
- File doesn't exist or can't be opened
- Can't determine file size
- Can't allocate buffer
- Read fails or returns wrong number of bytes

**Example:**
```c
chorda data = filum_legere_totum("config.txt", piscina);
si (data.datum == NIHIL)
{
    imprimere("Error: %s\n", filum_error_recens());
    redde FALSUM;
}
```

---

#### `filum_existit()`

```c
b32 filum_existit(
    constans character* via
);
```

Checks if a file exists.

**Parameters:**
- `via`: File path (C string)

**Returns:**
- `VERUM` if file exists
- `FALSUM` if file doesn't exist or path is NULL

**Example:**
```c
si (filum_existit("config.txt"))
{
    /* ... */
}
```

---

#### `filum_mensura()`

```c
memoriae_index filum_mensura(
    constans character* via
);
```

Gets file size in bytes.

**Parameters:**
- `via`: File path (C string)

**Returns:**
- File size in bytes
- `ZEPHYRUM` if file doesn't exist, can't be opened, or on error

**Note:** Also returns zero for empty files.

**Example:**
```c
memoriae_index size = filum_mensura("bigfile.dat");
si (size > ZEPHYRUM)
{
    imprimere("File is %zu bytes\n", size);
}
```

---

### Line-By-Line Reading

#### `filum_lector_aperire()`

```c
FilumLector* filum_lector_aperire(
    constans character* via,
               Piscina* piscina
);
```

Opens a file for line-by-line reading.

**Parameters:**
- `via`: File path (C string)
- `piscina`: Arena allocator

**Returns:**
- Pointer to FilumLector on success
- `NIHIL` on failure (check `filum_error_recens()`)

**Example:**
```c
FilumLector* lector = filum_lector_aperire("data.txt", piscina);
si (!lector)
{
    imprimere("Error: %s\n", filum_error_recens());
}
```

---

#### `filum_lector_lineam_proximam()`

```c
b32 filum_lector_lineam_proximam(
    FilumLector* lector,
         chorda* linea_out
);
```

Reads the next line from file.

**Parameters:**
- `lector`: FilumLector from `filum_lector_aperire()`
- `linea_out`: Output parameter for the line

**Returns:**
- `VERUM` if line was read successfully
- `FALSUM` if EOF or error

**Notes:**
- Line does not include newline characters
- Handles `\n`, `\r\n`, and `\r` line endings
- Empty lines return `chorda` with `mensura == 0`
- Lines of any length supported (uses dynamic growth for long lines)
- Line data allocated from the piscina

**Example:**
```c
chorda linea;
dum (filum_lector_lineam_proximam(lector, &linea))
{
    imprimere("Line: %.*s\n", linea.mensura, linea.datum);
}
```

---

#### `filum_lector_numerus_versus()`

```c
i32 filum_lector_numerus_versus(
    FilumLector* lector
);
```

Gets current line number.

**Parameters:**
- `lector`: FilumLector

**Returns:**
- Number of lines read so far (starts at 0)
- `ZEPHYRUM` if lector is NULL

**Example:**
```c
imprimere("Currently at line %d\n", filum_lector_numerus_versus(lector));
```

---

#### `filum_lector_finis()`

```c
b32 filum_lector_finis(
    FilumLector* lector
);
```

Checks if EOF has been reached.

**Parameters:**
- `lector`: FilumLector

**Returns:**
- `VERUM` if at EOF
- `FALSUM` if not at EOF or lector is NULL

**Example:**
```c
si (filum_lector_finis(lector))
{
    imprimere("Reached end of file\n");
}
```

---

#### `filum_lector_claudere()`

```c
vacuum filum_lector_claudere(
    FilumLector* lector
);
```

Closes the file handle.

**Parameters:**
- `lector`: FilumLector to close

**Notes:**
- Always call this when done reading
- Does not free the FilumLector struct (it came from piscina)
- Safe to call with NULL lector

**Example:**
```c
filum_lector_claudere(lector);
```

---

### Writing

#### `filum_scribere()`

```c
b32 filum_scribere(
    constans character* via,
                chorda  contentum
);
```

Writes binary data to file (overwrites existing file).

**Parameters:**
- `via`: File path (C string)
- `contentum`: Data to write (chorda with data pointer and length)

**Returns:**
- `VERUM` on success
- `FALSUM` on failure (check `filum_error_recens()`)

**Notes:**
- Opens in binary write mode (`"wb"`)
- Overwrites existing file
- Binary-safe (can write embedded nulls)

**Example:**
```c
chorda data;
data.datum = (i8*)"binary\0data";
data.mensura = XI;
si (!filum_scribere("output.bin", data))
{
    imprimere("Error: %s\n", filum_error_recens());
}
```

---

#### `filum_scribere_literis()`

```c
b32 filum_scribere_literis(
    constans character* via,
    constans character* contentum
);
```

Writes text to file (overwrites existing file).

**Parameters:**
- `via`: File path (C string)
- `contentum`: Null-terminated C string to write

**Returns:**
- `VERUM` on success
- `FALSUM` on failure

**Notes:**
- Opens in text write mode (`"w"`)
- Uses `strlen()` to determine length
- Cannot handle embedded nulls

**Example:**
```c
si (!filum_scribere_literis("output.txt", "Hello, world!\n"))
{
    imprimere("Write failed\n");
}
```

---

#### `filum_appendere()`

```c
b32 filum_appendere(
    constans character* via,
                chorda  contentum
);
```

Appends binary data to file.

**Parameters:**
- `via`: File path
- `contentum`: Data to append

**Returns:**
- `VERUM` on success
- `FALSUM` on failure

**Notes:**
- Opens in binary append mode (`"ab"`)
- Creates file if it doesn't exist
- Writes at end of file without removing existing content

**Example:**
```c
chorda entry;
entry.datum = (i8*)"[LOG] entry\n";
entry.mensura = XII;
filum_appendere("app.log", entry);
```

---

#### `filum_appendere_literis()`

```c
b32 filum_appendere_literis(
    constans character* via,
    constans character* contentum
);
```

Appends text to file.

**Parameters:**
- `via`: File path
- `contentum`: Null-terminated C string

**Returns:**
- `VERUM` on success
- `FALSUM` on failure

**Notes:**
- Opens in text append mode (`"a"`)

**Example:**
```c
filum_appendere_literis("app.log", "[INFO] Started\n");
```

---

### File Manipulation

#### `filum_delere()`

```c
b32 filum_delere(
    constans character* via
);
```

Deletes a file.

**Parameters:**
- `via`: File path

**Returns:**
- `VERUM` on success
- `FALSUM` on failure

**Warning:** No undo. File is gone.

**Example:**
```c
si (!filum_delere("temp.txt"))
{
    imprimere("Delete failed: %s\n", filum_error_recens());
}
```

---

#### `filum_movere()`

```c
b32 filum_movere(
    constans character* via_vetus,
    constans character* via_nova
);
```

Moves/renames a file.

**Parameters:**
- `via_vetus`: Old path
- `via_nova`: New path

**Returns:**
- `VERUM` on success
- `FALSUM` on failure

**Notes:**
- Uses `rename()` system call
- Atomic within same filesystem
- May fail if moving across filesystems

**Example:**
```c
filum_movere("old.txt", "new.txt");
```

---

#### `filum_copiare()`

```c
b32 filum_copiare(
    constans character* via_fons,
    constans character* via_destinatio
);
```

Copies a file.

**Parameters:**
- `via_fons`: Source path
- `via_destinatio`: Destination path

**Returns:**
- `VERUM` on success
- `FALSUM` on failure

**Notes:**
- Reads in 4096-byte chunks
- Overwrites destination if it exists
- Not transactional (partial copy if write fails)

**Example:**
```c
si (!filum_copiare("source.txt", "backup.txt"))
{
    imprimere("Copy failed\n");
}
```

---

### File Status

#### `filum_status()`

```c
b32 filum_status(
    constans character* via,
           FilumStatus* status_out
);
```

Gets file metadata.

**Parameters:**
- `via`: File path
- `status_out`: Output parameter for status

**Returns:**
- `VERUM` on success
- `FALSUM` on failure

**FilumStatus fields:**
```c
structura FilumStatus
{
    memoriae_index  mensura;            /* Size in bytes */
             b32    est_directorium;    /* Is directory? */
             b32    est_filum;          /* Is regular file? */
             b32    potest_legere;      /* Readable? */
             b32    potest_scribere;    /* Writable? */
};
```

**Example:**
```c
FilumStatus status;
si (filum_status("file.txt", &status))
{
    imprimere("Size: %zu bytes\n", status.mensura);
    imprimere("Can write: %s\n", status.potest_scribere ? "yes" : "no");
}
```

---

### Error Handling

#### `filum_error_recens()`

```c
constans character* filum_error_recens(vacuum);
```

Gets the most recent error message.

**Returns:**
- Pointer to error string if there was an error
- `NIHIL` if no error

**Notes:**
- Error buffer is static (256 bytes)
- Not thread-safe
- Cleared on successful operations

**Example:**
```c
chorda data = filum_legere_totum("file.txt", piscina);
si (data.datum == NIHIL)
{
    constans character* error = filum_error_recens();
    si (error)
    {
        imprimere("Failed: %s\n", error);
    }
}
```

---

## FAQ (Questions I'm Tired Of Answering)

### Q: Why is filum_legere_totum() better than fopen/fread myself?

Because you're gonna forget to check for errors, or you're gonna calculate the size wrong, or you're gonna forget to close the file, or you're gonna leak the buffer. `filum_legere_totum()` does all of that correctly and consistently. Use it.

### Q: What's the difference between filum_scribere() and filum_scribere_literis()?

`filum_scribere()` takes a `chorda` (pointer + length) and writes in binary mode. It can handle embedded nulls. `filum_scribere_literis()` takes a C string and writes in text mode. Use the binary version unless you specifically need text mode.

### Q: Can I use filum functions from multiple threads?

Not safely. The error handling uses a global buffer. If you need thread safety, give each thread its own piscina and don't share FilumLectors across threads. File operations themselves are thread-safe (the OS handles that), but the error tracking isn't.

### Q: What happens if I read a file that's being written by another process?

You get whatever's in the file at the moment you read it. If the writer hasn't flushed its buffers, you might not see all the data. If the writer is appending, you might read a partial write. This library doesn't do any locking. If you need that, use platform-specific file locking APIs.

### Q: Why doesn't filum_mensura() distinguish between "doesn't exist" and "exists but empty"?

Because it returns `ZEPHYRUM` (zero) in both cases. If you need to distinguish, call `filum_existit()` first. Nine times out of ten you don't care about the difference anyway.

### Q: How big can lines be in filum_lector_lineam_proximam()?

Any size. Short lines (< 4096 bytes) are handled with a single allocation. Long lines use ChordaAedificator to build the line dynamically. There's no practical limit beyond available memory.

### Q: What if my file has mixed line endings (both \n and \r\n)?

The line reader strips both. It checks the end of each line and removes any `\r` or `\n` characters. So mixed line endings work fine.

### Q: Can I seek to a specific line with FilumLector?

No. It's a forward-only reader. If you need random access to lines, read the whole file with `filum_legere_totum()` and parse it yourself.

### Q: What if filum_lector_lineam_proximam() fails partway through a file?

It returns `FALSUM` and you can check `filum_error_recens()` to see what went wrong. The FilumLector is now in an error state. Don't keep trying to read from it. Close it and handle the error.

### Q: Why do I have to pass a piscina to everything?

Because that's how memory management works in this codebase. Arena allocation. Everything goes into the piscina. When you destroy the piscina, everything gets freed. This is a feature, not a bug.

### Q: What if I want to keep some data after destroying the piscina?

Copy it to a different piscina or to malloc'd memory before destroying the original piscina. Once you destroy a piscina, everything in it is gone.

### Q: Can FilumLector handle files with no final newline?

Yes. When it hits EOF, it returns whatever it read as the final line, even without a newline. You don't lose the last line.

### Q: What's the performance difference between filum_legere_totum() and FilumLector?

`filum_legere_totum()` is faster for small files because it's one allocation and one read. FilumLector is better for large files because you process lines as you read them without loading the whole thing into memory. Pick based on your use case.

### Q: Can I read binary files line-by-line?

You can, but it probably doesn't make sense. The line reader looks for newline characters (`\n`, `\r`, `\r\n`). If your binary file has those bytes, you'll get "lines" at random places. For binary files, use `filum_legere_totum()` and parse the structure yourself.

### Q: Why doesn't filum_copiare() preserve file permissions?

Because the C standard library doesn't have a portable way to do that. On Unix you'd use `chmod()`, on Windows you'd use different APIs. This library sticks to standard C89 for portability. If you need to preserve permissions, use platform-specific code.

### Q: What if I try to filum_scribere() to a path that doesn't exist?

If the directory exists but the file doesn't, it creates the file. If the directory doesn't exist, `fopen()` fails and you get an error. This library doesn't create directories.

### Q: How do I read a file as a null-terminated C string?

Allocate one extra byte and add the null terminator yourself:

```c
chorda data = filum_legere_totum("file.txt", piscina);
character* str = (character*)piscina_allocare(piscina, data.mensura + I);
memcpy(str, data.datum, data.mensura);
str[data.mensura] = '\0';
```

Or use the `%.*s` printf format specifier which takes a length.

### Q: Can I use FilumLector to read from stdin?

No. It opens files by path. If you want to read stdin line-by-line, you'll need to use `fgets()` directly. Maybe I'll add that feature later, maybe I won't.

### Q: What if filum_appendere() fails halfway through?

The file might be partially updated. This isn't a transactional append. If you need atomic appends, you'll need OS-specific APIs or write-to-temp-file-then-rename patterns.

### Q: Why is the error handling not thread-safe?

Because thread-safe error handling requires thread-local storage, which isn't in C89. If you need it, either upgrade to C99+ or give each thread its own error context. Or just don't use filum from multiple threads.

---

That's it. That's filum. Read files, write files, don't overcomplicate it. If you somehow screw this up I don't know what to tell you.
