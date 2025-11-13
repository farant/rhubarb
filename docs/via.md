# Via: Path Manipulation Library

Look, you're gonna need to work with file paths. That's just how it is. Every program that touches the filesystem - which is basically every program worth writing - needs to join paths, normalize them, split them apart, figure out what's a directory and what's a filename. You could do this yourself with string manipulation and probably get it wrong in fifteen different ways, or you could use this library that already handles all the edge cases.

## What Via Does

Via (Latin for "path" or "way" - get used to it, everything's in Latin around here) is a platform-independent path manipulation library. Right now it's Unix-flavored, but the API is designed so you could swap in Windows path handling later without changing your code. Always uses forward slashes internally because that's the sane choice.

The library does six basic things:
1. **Joins** path parts together with proper separator handling
2. **Normalizes** paths by resolving `.` and `..` and removing duplicate slashes
3. **Extracts** components like directory name, filename, extension
4. **Converts** between relative and absolute paths
5. **Queries** whether paths are absolute or relative
6. **Gets** the current working directory

Everything allocates from a `Piscina` (memory pool - we'll get to that). Nothing uses malloc directly. No memory leaks, no fragmentation, you clean up the whole pool at once when you're done.

## Basic Concepts You Need to Know

### What's a Path Anyway

A path is a string that tells you how to locate a file or directory. On Unix (what we're targeting first), paths look like `/usr/local/bin/foo` or `../config/app.ini`. The parts between slashes are called components or segments.

**Absolute paths** start with `/` and specify location from the root of the filesystem. `/home/user/documents` is absolute.

**Relative paths** don't start with `/` and are interpreted relative to some working directory. `documents/report.txt` is relative - where is it? Relative to wherever you are right now.

Two special path components:
- `.` means "current directory" - it's basically a no-op
- `..` means "parent directory" - go up one level

### About That Latin

Yeah, we're writing C in Latin. Deal with it.

Quick vocabulary you'll see constantly:
- `via` = path, way
- `chorda` = string (literally "cord" or "rope")
- `piscina` = pool (memory pool)
- `mensura` = measure, size, length
- `datum` = data (given thing)
- `fructus` = fruit, result, product
- `iungere` = to join, connect
- `normalizare` = to normalize
- `directorium` = directory
- `nomen` = name
- `radix` = root
- `extensio` = extension
- `absoluta` = absolute
- `relativa` = relative
- `currens` = current, running

The verbs are properly conjugated. `via_iungere` means "to join a path". `via_normalizare` means "to normalize a path". You'll pick it up as you go. It's not that complicated - no more complicated than memorizing arbitrary abbreviations in someone's inconsistent API where half the functions say "dir" and half say "directory" and nobody knows why.

### The String Type: `chorda`

Via works with `chorda` strings, not null-terminated C strings. A `chorda` is simple:

```c
typedef struct chorda
{
    i32  mensura;  /* length in bytes */
    i8*  datum;    /* pointer to data (NOT null-terminated) */
} chorda;
```

Why not null-terminated? Because we know the length. Because we can represent empty strings and strings with embedded nulls without ambiguity. Because substring operations become pointer arithmetic instead of copying. You convert to C strings when you need to call POSIX functions - `chorda_ut_cstr()` does that and null-terminates for you.

All the `chorda` operations allocate from your `Piscina`. You don't free individual strings. You destroy the whole pool when you're done.

### Memory: The Piscina

Via doesn't malloc. Every function that allocates takes a `Piscina*` parameter. You create one pool for your operation, do all your path manipulation, use the results, then destroy the whole pool at once. Simple. Fast. No leaks.

```c
Piscina* p = piscina_generare_dynamicum("path_work", 4096);

/* ... do path operations ... */

piscina_destruere(p);  /* everything allocated goes away */
```

If you need results to outlive the pool, copy them out first. Usually you don't - you're building a path to immediately open a file or something.

## Working With Paths: The Core Operations

### Joining Paths: `via_iungere`

The most common operation. You have path components, you need to stick them together with slashes. Sounds simple. It's not simple.

```c
chorda via_iungere(
    const chorda* partes,
    i32           numerus,
    Piscina*      piscina);
```

Takes array of parts, joins them with `/`, handles all the annoying cases:

```c
Piscina* p = piscina_generare_dynamicum("example", 1024);

chorda partes[3];
partes[0] = chorda_ex_literis("usr", p);
partes[1] = chorda_ex_literis("local", p);
partes[2] = chorda_ex_literis("bin", p);

chorda resultado = via_iungere(partes, 3, p);
/* resultado = "usr/local/bin" */
```

What makes this non-trivial:

**Trailing slashes:** If you join `"foo/"` with `"bar"`, you get `"foo/bar"`, not `"foo//bar"`. The function checks if the last character of what it's building is already a separator before adding another one.

**Leading slashes:** If you join `"foo"` with `"/bar"`, you get `"foo/bar"`, not `"foo//bar"`. It checks if the next part starts with a separator.

**Empty parts:** If you pass an empty string in the array, it's skipped. Joining `["foo", "", "bar"]` gives you `"foo/bar"`.

**Absolute paths:** If the first component is `"/usr"`, the result is absolute: `"/usr/local/bin"`.

The function is smart enough that you mostly don't have to think about whether your parts have trailing slashes or not. Just join them, it figures it out.

### Normalizing Paths: `via_normalizare`

Takes a path that might have redundant components and cleans it up:

```c
chorda via_normalizare(
    chorda   via,
    Piscina* piscina);
```

What it does:

**Resolves `.` (current directory):** `"foo/./bar"` becomes `"foo/bar"`. The dot does nothing, it's removed.

**Resolves `..` (parent directory):** `"foo/../bar"` becomes `"bar"`. The `foo` and the `..` cancel out.

**Removes duplicate slashes:** `"foo//bar"` becomes `"foo/bar"`.

Example code:

```c
chorda messy = chorda_ex_literis("foo/./bar/../baz//qux", p);
chorda clean = via_normalizare(messy, p);
/* clean = "foo/baz/qux" */
```

The algorithm splits the path on `/`, then processes components one by one using a stack:

- Regular component → push onto stack
- `.` → skip it
- `..` → pop from stack (go up one level)

After processing all components, join whatever's left on the stack.

**Edge case: absolute paths.** If the path starts with `/`, normalization won't let `..` go above root. `"/../foo"` normalizes to `"/foo"`, not `"../foo"`. You can't go above root.

**Edge case: relative paths.** If the path is relative and you have `..` when the stack is empty, the `..` stays. `"../foo"` stays `"../foo"`. It's telling you "go up from wherever you are, then into foo" - that's legitimate for a relative path.

**Edge case: empty result.** If everything cancels out and you end up with nothing, absolute paths return `"/"` and relative paths return `"."`.

This is important: **normalization doesn't touch the filesystem**. It's pure string manipulation. `"foo/../bar"` becomes `"bar"` whether or not `foo` actually exists as a directory. It's symbolic manipulation based on the rules of path syntax.

### Converting to Absolute: `via_absoluta`

Takes a relative path and makes it absolute by resolving it against the current working directory:

```c
chorda via_absoluta(
    chorda   via,
    Piscina* piscina);
```

If the path is already absolute, just normalizes it and returns. If it's relative, gets the CWD, joins them, then normalizes:

```c
/* Say CWD is "/home/user/projects" */
chorda rel = chorda_ex_literis("src/main.c", p);
chorda abs = via_absoluta(rel, p);
/* abs = "/home/user/projects/src/main.c" */
```

This one DOES touch the filesystem - it calls `getcwd()` to find out where you are. If getting the CWD fails (rare but possible - your working directory was deleted, you don't have permissions, who knows), it returns the original path unchanged.

Common use: you get a filename from command line arguments. User typed `"config.ini"`. Where is that? You need an absolute path to be unambiguous. Convert it.

### Getting Directory Name: `via_directorium`

Extracts the directory portion of a path - everything before the last `/`:

```c
chorda via_directorium(
    chorda   via,
    Piscina* piscina);
```

This is like the Unix `dirname` command:

```c
chorda path = chorda_ex_literis("/home/user/document.txt", p);
chorda dir = via_directorium(path, p);
/* dir = "/home/user" */
```

Edge cases:

- `"/foo"` → `"/"` (directory of something in root is root)
- `"foo/bar"` → `"foo"` (relative path)
- `"foo.txt"` → `"."` (no directory means current directory)

If there's no separator in the path, returns `"."` because a path like `"foo.txt"` implicitly means `"./foo.txt"`.

### Getting File Name: `via_nomen`

Extracts the file name - everything after the last `/`:

```c
chorda via_nomen(
    chorda   via,
    Piscina* piscina);
```

Like Unix `basename`:

```c
chorda path = chorda_ex_literis("/home/user/document.txt", p);
chorda name = via_nomen(path, p);
/* name = "document.txt" */
```

Edge cases:

- `"foo.txt"` → `"foo.txt"` (no directory, whole thing is filename)
- `"/foo/bar/"` → `""` (trailing slash means empty filename)
- `"/"` → `""` (root has no filename)

If there's no separator, returns the whole path because that's the filename.

### Getting Name Without Extension: `via_nomen_radix`

Gets the filename without the extension. "Radix" means "root" in Latin - the root of the filename:

```c
chorda via_nomen_radix(
    chorda   via,
    Piscina* piscina);
```

Removes the last dot and everything after:

```c
chorda path = chorda_ex_literis("document.txt", p);
chorda stem = via_nomen_radix(path, p);
/* stem = "document" */
```

Works on full paths too - extracts the name first, then removes extension:

```c
chorda path = chorda_ex_literis("/home/user/report.pdf", p);
chorda stem = via_nomen_radix(path, p);
/* stem = "report" */
```

Special cases:

- **Dotfiles:** `".bashrc"` returns `".bashrc"` unchanged. A file starting with dot is a hidden file, not a file with empty name and extension `.bashrc`.

- **Multiple extensions:** `"archive.tar.gz"` returns `"archive.tar"`. Only the LAST extension is removed. If you want to handle multi-part extensions specially, split on dots yourself.

- **No extension:** `"README"` returns `"README"` unchanged.

### Getting Extension: `via_extensio`

Extracts just the file extension, including the dot:

```c
chorda via_extensio(
    chorda   via,
    Piscina* piscina);
```

Everything from the last dot to the end:

```c
chorda path = chorda_ex_literis("document.txt", p);
chorda ext = via_extensio(path, p);
/* ext = ".txt" */
```

Same special cases as `via_nomen_radix`:

- **Dotfiles:** `".bashrc"` returns `""` (empty string). No extension.
- **Multiple extensions:** `"archive.tar.gz"` returns `".gz"`. Just the last one.
- **No extension:** `"README"` returns `""`.

Why include the dot? Because when you're reconstructing paths, it's clearer: `stem + ext` gives you back the filename. If extension didn't include the dot, you'd have to remember to add it yourself and you'd screw it up.

### Checking Path Type

Two simple predicates:

```c
b32 via_est_absoluta(chorda via);
b32 via_est_relativa(chorda via);
```

On Unix, absolute paths start with `/`. That's it. Check the first character.

```c
b32 is_abs = via_est_absoluta(chorda_ex_literis("/usr/bin", p));
/* is_abs = TRUE */

b32 is_rel = via_est_relativa(chorda_ex_literis("../foo", p));
/* is_rel = TRUE */
```

These don't allocate anything, they just look at the first character. Empty path is relative (returns FALSE for `via_est_absoluta`).

### Getting Current Directory: `via_directorium_currens`

Wrapper around `getcwd()`:

```c
chorda via_directorium_currens(Piscina* piscina);
```

Returns current working directory as an absolute path:

```c
chorda cwd = via_directorium_currens(p);
/* something like "/home/user/projects/rhubarb" */
```

If it can't get the CWD (really shouldn't happen in normal programs, but could if your CWD was deleted out from under you or permissions changed), returns empty string (`.datum = NULL`, `.mensura = 0`).

## How It Actually Works: Implementation Details

You don't need to know this to use the library, but if you're the kind of person reading documentation this far, you probably want to know how it works.

### Path Joining: The Algorithm

`via_iungere` uses a `ChordaAedificator` (string builder) to construct the result. That's a type that lets you append to a string incrementally without reallocating on every append - it grows the buffer by doubling when needed.

For each part in the input array:

1. Skip if part is empty
2. If not the first part and builder isn't empty:
   - Check if builder's last character is `/`
   - Check if part's first character is `/`
   - If neither is `/`, append a `/` separator
3. Append the part

The separator logic prevents both `foo//bar` (double slash) and `foobar` (missing slash). Only adds `/` when both sides need it.

### Path Normalization: The Stack Algorithm

`via_normalizare` uses a stack-based approach. Well, it's not a real stack, it's just an array with an index that acts as stack pointer, but same difference.

1. Split path on `/` to get components
2. Allocate array for stack (max size = number of components)
3. Track whether path was absolute (started with `/`)
4. For each component:
   - Empty or `"."` → skip
   - `".."` → pop stack if possible (and allowed)
   - Anything else → push onto stack
5. Join whatever's left on stack
6. If was absolute, prepend `/`

The tricky part is handling `..` correctly:

- For **absolute paths**, if stack is empty, `..` is discarded. You can't go above root.
- For **relative paths**, if stack is empty, `..` is kept. It's meaningful - "go up from here then..."

Edge case: if stack ends up empty, return `"/"` for absolute, `"."` for relative.

### Component Extraction: Searching Backwards

Functions like `via_directorium`, `via_nomen`, `via_extensio` need to find the last `/` or last `.` in the string. They do simple backwards linear search from the end.

Helper function `_invenire_ultimum_separatorem` (find last separator):

```c
for (i = s.mensura - 1; i >= 0; i--)
{
    if (s.datum[i] == '/')
        return i;
}
return -1;  /* not found */
```

Once you know where the last separator is, extracting directory or name is just substring operations.

For extension handling, `_invenire_ultimum_punctum` finds the last `.` the same way. Then special-case logic for dotfiles (don't treat leading `.` as extension marker).

### String Slicing: Zero-Copy Views

When `via_nomen` returns the filename portion, it doesn't allocate a new string and copy the bytes. It uses `chorda_sectio` which creates a view:

```c
chorda chorda_sectio(chorda s, i32 initium, i32 finis)
{
    chorda result;
    result.datum = s.datum + initium;
    result.mensura = finis - initium;
    return result;
}
```

Just pointer arithmetic. The returned `chorda` points into the original string's memory. This is fine because everything lives in the piscina anyway - the original string won't be freed until you destroy the whole pool.

This is why the code can do lots of substring operations without worrying about performance or memory usage. It's all views into existing buffers.

## Putting It Together: Real Examples

### Example 1: Building a Config File Path

You have a config directory and a filename, need to build full path:

```c
Piscina* p = piscina_generare_dynamicum("config_path", 1024);

chorda config_dir = chorda_ex_literis("/etc/myapp", p);
chorda filename = chorda_ex_literis("settings.ini", p);

chorda parts[2];
parts[0] = config_dir;
parts[1] = filename;

chorda full_path = via_iungere(parts, 2, p);
/* full_path = "/etc/myapp/settings.ini" */

/* Use the path... */
char* cstr = chorda_ut_cstr(full_path, p);
FILE* f = fopen(cstr, "r");

piscina_destruere(p);
```

### Example 2: Resolving Relative Paths from User Input

User gives you a path on command line, might be relative, you need absolute:

```c
/* argv[1] = "../../data/input.txt" */
Piscina* p = piscina_generare_dynamicum("resolve", 1024);

chorda user_path = chorda_ex_literis(argv[1], p);
chorda absolute = via_absoluta(user_path, p);

if (absolute.datum == NULL)
{
    fprintf(stderr, "Couldn't resolve path\n");
    exit(1);
}

/* Now use absolute path - unambiguous */
char* cstr = chorda_ut_cstr(absolute, p);
FILE* f = fopen(cstr, "r");

piscina_destruere(p);
```

### Example 3: Parsing a Path

Break path into components:

```c
chorda path = chorda_ex_literis("/var/log/messages.log", p);

chorda dir = via_directorium(path, p);
/* dir = "/var/log" */

chorda name = via_nomen(path, p);
/* name = "messages.log" */

chorda stem = via_nomen_radix(path, p);
/* stem = "messages" */

chorda ext = via_extensio(path, p);
/* ext = ".log" */
```

### Example 4: Building Path from Current Directory

You want to create a file in a subdirectory of wherever you are:

```c
Piscina* p = piscina_generare_dynamicum("subdir_path", 2048);

chorda cwd = via_directorium_currens(p);
if (cwd.datum == NULL)
{
    /* Couldn't get CWD, handle error */
}

chorda parts[3];
parts[0] = cwd;
parts[1] = chorda_ex_literis("output", p);
parts[2] = chorda_ex_literis("results.txt", p);

chorda full = via_iungere(parts, 3, p);
/* If CWD was "/home/user/project",
   full = "/home/user/project/output/results.txt" */

piscina_destruere(p);
```

### Example 5: Normalizing User-Provided Paths

User might give you messy paths with `..` and such, clean them:

```c
chorda messy = chorda_ex_literis("./src/../lib/./util.c", p);
chorda clean = via_normalizare(messy, p);
/* clean = "lib/util.c" */
```

This is good for security too - if you're accepting paths from untrusted input, normalize before using. Prevents tricks like `../../../etc/passwd` from escaping your intended directory tree. Though you should also validate the normalized path doesn't start with `..` or `/` if you want to enforce relative-within-sandbox.

## Common Patterns and Idioms

### The Temporary Pool Pattern

Most via usage looks like this:

```c
void some_function(const char* input_path)
{
    Piscina* temp = piscina_generare_dynamicum("path_work", 2048);

    chorda path = chorda_ex_literis(input_path, temp);
    /* ... do path manipulation ... */
    chorda result = via_absoluta(path, temp);

    /* Convert back to C string if needed */
    char* result_cstr = chorda_ut_cstr(result, temp);

    /* Use result_cstr for something... */

    piscina_destruere(temp);  /* All temporary strings gone */
}
```

Create pool, do work, destroy pool. If you need results to outlive the function, either:
- Copy the C string: `strdup(result_cstr)` before destroying pool
- Keep the pool alive longer (return it, or pass in from caller)

### Chaining Operations

Since everything returns `chorda` and takes `chorda`, you can chain:

```c
chorda path = chorda_ex_literis("foo/../bar/./baz.txt", p);
chorda normalized = via_normalizare(path, p);
chorda absolute = via_absoluta(normalized, p);
chorda dirname = via_directorium(absolute, p);
```

Or compress it:

```c
chorda dirname = via_directorium(
    via_absoluta(
        via_normalizare(
            chorda_ex_literis("foo/../bar/./baz.txt", p), p), p), p);
```

Though that's getting a bit hard to read. Sometimes temporary variables are okay.

### Checking Empty Results

Functions return empty `chorda` on error (`.datum = NULL`, `.mensura = 0`). Check before using:

```c
chorda cwd = via_directorium_currens(p);
if (cwd.datum == NULL)
{
    fprintf(stderr, "Couldn't get current directory\n");
    exit(1);
}
```

Or check the length:

```c
if (cwd.mensura == 0)
{
    /* Empty result */
}
```

Both work. Checking pointer is probably clearer for "did this fail" vs "did this return empty string legitimately".

## Error Handling

Via functions don't use errno. They don't print error messages. They return empty `chorda` on failure (`.datum = NULL`, `.mensura = 0`).

Functions that can fail:
- `via_iungere` - if allocation fails
- `via_normalizare` - if allocation fails
- `via_absoluta` - if can't get CWD or allocation fails
- `via_directorium_currens` - if `getcwd()` fails

Since everything allocates from piscina, and piscina allocation is fail-fast (exits on OOM by default), in practice you won't see allocation failures unless you're using the "try" variants of piscina functions.

CWD failure is the realistic error case. Can happen if:
- Your working directory was deleted
- Permissions changed
- Path too long (longer than 4096 bytes)

Handle it like any I/O error - log it, return error code to caller, use default path, whatever makes sense for your program.

## Platform Notes

Current implementation is Unix (POSIX). Uses:
- `/` as path separator
- `getcwd()` for current directory
- Forward slash semantics throughout

Windows port would need:
- Accept both `/` and `\` as separators in input
- Normalize to `/` internally (as documented in philosophy)
- Handle drive letters (`C:`)
- Handle UNC paths (`\\server\share`)
- Replace `getcwd()` with `GetCurrentDirectory()`

But the API wouldn't change. That's the point. Your code calls `via_iungere`, doesn't care whether it's joining `/foo/bar` or `C:\foo\bar` underneath.

For now, if you need Windows support, run under WSL or Cygwin. Or port the library - shouldn't be more than a couple hundred lines.

## Performance Characteristics

All the algorithms are O(n) in path length:
- Joining scans each input part once
- Normalizing splits and processes each component once
- Searching scans backwards once
- String building amortizes to O(n) with doubling buffer

Nothing is quadratic. Nothing does nested loops over path components. Reasonable performance for paths under 4096 bytes (which is everything in practice - systems impose lower limits than that).

Memory usage: proportional to path length. Normalization allocates a stack of components, worst case as big as the input. Joining allocates a growing buffer, worst case 2x input size. Everything's in your piscina anyway.

Not optimized for huge paths (like if you're processing paths with 100,000 components for some reason - what are you doing?). Optimized for normal paths, 10-50 components, total length under 1KB. Which is 99.9% of use cases.

## Testing

See `probationes/probatio_via.c` for comprehensive test suite. Covers:
- Joining with various combinations of leading/trailing slashes
- Normalization with `.`, `..`, empty components
- Edge cases: root, empty paths, dotfiles
- All extraction functions with different path shapes
- Absolute/relative detection
- CWD retrieval

Run tests: `./probatio_via`

If you change the implementation, run the tests. If you add features, add tests. Standard practice.

---

# API Reference

## Types

### `chorda`
```c
typedef struct chorda
{
    i32  mensura;  /* length in bytes */
    i8*  datum;    /* pointer to data */
} chorda;
```

String type used throughout. Not null-terminated. Length is explicitly stored.

---

## Path Joining

### `via_iungere`
```c
chorda via_iungere(
    const chorda* partes,
    i32           numerus,
    Piscina*      piscina);
```

**Parameters:**
- `partes` - Array of string components to join
- `numerus` - Number of components in array
- `piscina` - Memory pool for allocation

**Returns:**
- Joined path as `chorda`
- Empty string (`.datum = NULL`) on allocation failure

**Description:**
Joins array of path components with `/` separator. Handles trailing/leading slashes intelligently - won't create `//`. Skips empty components. If first component starts with `/`, result is absolute path.

**Example:**
```c
chorda parts[3];
parts[0] = chorda_ex_literis("usr", p);
parts[1] = chorda_ex_literis("local", p);
parts[2] = chorda_ex_literis("bin", p);
chorda result = via_iungere(parts, 3, p);
/* result = "usr/local/bin" */
```

---

## Path Normalization

### `via_normalizare`
```c
chorda via_normalizare(
    chorda   via,
    Piscina* piscina);
```

**Parameters:**
- `via` - Path to normalize
- `piscina` - Memory pool for allocation

**Returns:**
- Normalized path as `chorda`
- Empty string on allocation failure

**Description:**
Normalizes path by:
- Removing `.` (current directory) components
- Resolving `..` (parent directory) components
- Removing duplicate `/` separators

For absolute paths, `..` cannot go above root. For relative paths, leading `..` are preserved. Empty result becomes `"/"` (absolute) or `"."` (relative).

**Example:**
```c
chorda path = chorda_ex_literis("foo/./bar/../baz", p);
chorda clean = via_normalizare(path, p);
/* clean = "foo/baz" */
```

---

### `via_absoluta`
```c
chorda via_absoluta(
    chorda   via,
    Piscina* piscina);
```

**Parameters:**
- `via` - Path to convert (may be relative or absolute)
- `piscina` - Memory pool for allocation

**Returns:**
- Absolute path as `chorda`
- Empty string if cannot get CWD or allocation fails

**Description:**
Converts relative path to absolute by resolving against current working directory. If path is already absolute, just normalizes and returns. Calls `getcwd()` to get CWD, joins with path, normalizes result.

**Example:**
```c
/* CWD = "/home/user" */
chorda rel = chorda_ex_literis("projects/rhubarb", p);
chorda abs = via_absoluta(rel, p);
/* abs = "/home/user/projects/rhubarb" */
```

---

## Path Component Extraction

### `via_directorium`
```c
chorda via_directorium(
    chorda   via,
    Piscina* piscina);
```

**Parameters:**
- `via` - Full path
- `piscina` - Memory pool (currently unused, for API consistency)

**Returns:**
- Directory portion of path (everything before last `/`)
- `"."` if no directory component
- `"/"` if directory is root

**Description:**
Extracts directory name (like Unix `dirname`). Finds last `/` and returns everything before it. If no `/`, returns `"."` (current directory). If `/` is at position 0, returns `"/"` (root).

**Example:**
```c
chorda path = chorda_ex_literis("/var/log/messages.log", p);
chorda dir = via_directorium(path, p);
/* dir = "/var/log" */
```

---

### `via_nomen`
```c
chorda via_nomen(
    chorda   via,
    Piscina* piscina);
```

**Parameters:**
- `via` - Full path
- `piscina` - Memory pool (currently unused)

**Returns:**
- Filename portion of path (everything after last `/`)
- Empty string if path ends with `/`

**Description:**
Extracts filename (like Unix `basename`). Finds last `/` and returns everything after it. If no `/`, returns entire path. If path ends with `/`, returns empty string.

**Example:**
```c
chorda path = chorda_ex_literis("/var/log/messages.log", p);
chorda name = via_nomen(path, p);
/* name = "messages.log" */
```

---

### `via_nomen_radix`
```c
chorda via_nomen_radix(
    chorda   via,
    Piscina* piscina);
```

**Parameters:**
- `via` - Full path or filename
- `piscina` - Memory pool for allocation

**Returns:**
- Filename without extension (stem)
- Full filename if no extension

**Description:**
Returns filename with last extension removed. First extracts filename via `via_nomen`, then finds last `.` and returns everything before it. Special case: dotfiles (starting with `.`) return unchanged - the `.` is not treated as extension marker.

**Example:**
```c
chorda path = chorda_ex_literis("document.txt", p);
chorda stem = via_nomen_radix(path, p);
/* stem = "document" */

chorda dotfile = chorda_ex_literis(".bashrc", p);
chorda stem2 = via_nomen_radix(dotfile, p);
/* stem2 = ".bashrc" */
```

---

### `via_extensio`
```c
chorda via_extensio(
    chorda   via,
    Piscina* piscina);
```

**Parameters:**
- `via` - Full path or filename
- `piscina` - Memory pool for allocation

**Returns:**
- File extension including `.`
- Empty string if no extension

**Description:**
Returns file extension including the dot. First extracts filename, then finds last `.` and returns from there to end. Dotfiles (starting with `.`) return empty string - no extension. If no `.`, returns empty string.

**Example:**
```c
chorda path = chorda_ex_literis("archive.tar.gz", p);
chorda ext = via_extensio(path, p);
/* ext = ".gz" */
```

---

## Path Queries

### `via_est_absoluta`
```c
b32 via_est_absoluta(chorda via);
```

**Parameters:**
- `via` - Path to check

**Returns:**
- `VERUM` (1) if absolute path
- `FALSUM` (0) if relative or empty

**Description:**
Checks if path is absolute (starts with `/` on Unix). Empty path is considered relative.

**Example:**
```c
b32 is_abs = via_est_absoluta(chorda_ex_literis("/usr/bin", p));
/* is_abs = VERUM */
```

---

### `via_est_relativa`
```c
b32 via_est_relativa(chorda via);
```

**Parameters:**
- `via` - Path to check

**Returns:**
- `VERUM` (1) if relative path
- `FALSUM` (0) if absolute

**Description:**
Checks if path is relative (does NOT start with `/`). Opposite of `via_est_absoluta`.

**Example:**
```c
b32 is_rel = via_est_relativa(chorda_ex_literis("../foo", p));
/* is_rel = VERUM */
```

---

## Utilities

### `via_directorium_currens`
```c
chorda via_directorium_currens(Piscina* piscina);
```

**Parameters:**
- `piscina` - Memory pool for allocation

**Returns:**
- Current working directory as absolute path
- Empty string if `getcwd()` fails

**Description:**
Gets current working directory by calling `getcwd()`. Returns as allocated `chorda`. Maximum path length 4096 bytes. Returns empty string on failure (CWD deleted, permissions changed, path too long).

**Example:**
```c
chorda cwd = via_directorium_currens(p);
if (cwd.datum != NULL)
{
    /* Use cwd... */
}
```

---

# FAQ

## Why Latin?

Because naming things is hard and English is inconsistent. Is it "dir" or "directory"? "sep" or "separator"? "cur" or "current" or "cwd"? Nobody can agree. Latin is:
1. Explicit and formal
2. Culture-neutral (nobody's native language)
3. Forces you to think about what things are called
4. Filters out people who'd complain about it

Also it's funny watching people's faces when they see it for the first time. That's worth something.

## Why Not Just Use String Functions?

You *could* write path manipulation with `strstr`, `strchr`, manual loops. You'll get it wrong. Edge cases:
- Trailing slashes: is `/foo/` the same as `/foo`?
- Leading slashes: what does `foo//bar` mean?
- Dots: what does `./././foo` mean?
- Double dots: what does `/../foo` mean for absolute vs relative?
- Empty components: what does `foo//bar` (empty component between slashes) mean?
- Root: what's the directory of `/foo`?
- Dotfiles: is `.bashrc` a file with empty name and extension `.bashrc`?

Via handles all of these consistently. The rules are documented. The tests verify them. You don't have to think about it.

## Why the Piscina Pattern?

Three reasons:

1. **Performance:** No malloc/free overhead for temporary strings. Allocate from bump pointer, destroy entire pool at once.

2. **Safety:** Can't leak individual strings. Either you leak the whole pool (obvious) or you don't leak anything.

3. **Simplicity:** No ownership questions. Everything in pool has same lifetime. Clean up once.

The pattern is: create pool for an operation, do operation, destroy pool. Pools are cheap - make a new one whenever you need temporary strings.

## Does Normalization Touch the Filesystem?

No. `via_normalizare` is pure string manipulation. `foo/../bar` becomes `bar` whether or not `foo` exists as a directory.

`via_absoluta` DOES touch filesystem - needs to call `getcwd()` to know where you are.

`via_directorium_currens` obviously touches filesystem - that's its whole job.

Everything else is string ops.

## What About Symlinks?

Via doesn't follow symlinks. It's syntactic manipulation, not filesystem operations. If you have:
```
/foo -> /bar/baz
/qux
```

And you normalize `/foo/../qux`, you get `/qux`. It doesn't realize `/foo` is a symlink to `/bar/baz` and give you `/bar/qux`.

If you need symlink resolution, use `realpath()` from POSIX. Not part of Via.

## How Do I Convert Between chorda and C Strings?

To C string: `chorda_ut_cstr(s, piscina)` - allocates null-terminated copy from pool.

From C string: `chorda_ex_literis(cstr, piscina)` - allocates and copies into pool.

No in-place conversion. `chorda` might not be null-terminated.

## Can I Modify a chorda in Place?

Technically yes - `datum` is just a pointer, you can write to it. But don't. Operations return new chordas. Treat them as immutable. If you need a modified version, call a function to create it.

Exception: if you allocated the buffer yourself and made a `chorda` wrapping it, you can write to your buffer. But Via functions all return fresh allocations from piscina.

## Why No Unicode Handling?

Because Unicode in paths is a mess and platform-specific. On Unix, path components are byte sequences - can be UTF-8, can be Latin-1, can be arbitrary bytes. The kernel doesn't care.

Via treats paths as byte sequences. If you put UTF-8 in, you get UTF-8 out. If you put garbage in, you get garbage out. Doesn't validate, doesn't normalize Unicode, doesn't convert encodings.

If you need Unicode path handling, do it before calling Via. Convert to whatever byte encoding your filesystem uses, then manipulate paths.

## What If I Want Windows Paths?

Current implementation is Unix only. Windows port would need:
- Accept `\` as separator in input
- Handle drive letters
- Handle UNC paths
- Platform detection, different code paths

Not hard, just hasn't been done yet. The API is designed to support it - implementation detail whether separator is `/` or `\`.

For now, WSL or Cygwin if you need Windows.

## What's the Maximum Path Length?

No hardcoded limit in Via - it'll handle whatever fits in memory. System limits apply:
- Unix: usually 4096 bytes (`PATH_MAX`)
- `getcwd()` buffer is 4096 bytes
- Most filesystems have practical limits around 4096

If you're manipulating paths longer than 4KB, you're doing something weird. Via will work but filesystem operations might fail.

## Thread Safety?

Via functions don't use global state. If each thread has its own `Piscina`, no problem. Multiple threads can call Via functions simultaneously on different pools.

Don't share a `Piscina` across threads without locking. Pool allocation is not thread-safe (bump pointer, no atomic ops).

## Why Return Empty String Instead of NULL?

Consistency with `chorda` design. A `chorda` can be:
- Valid: `.datum != NULL`, `.mensura > 0`
- Empty: `.datum != NULL`, `.mensura == 0`
- Failed: `.datum == NULL`, `.mensura == 0`

Returning "empty" means you got a valid result that happens to be empty (like basename of `/foo/bar/` is empty string).

Returning "failed" means operation couldn't complete (allocation failed, couldn't get CWD, etc).

Check `.datum == NULL` for failures. Check `.mensura == 0` for empty results.

## Can I Use Via Without the Rest of Rhubarb?

Yes, but you need:
- `chorda` library (string operations)
- `piscina` library (memory pool)
- `chorda_aedificator` (string builder, used by via_iungere)
- `latina.h` (type definitions)

Via depends on those. They're all small, self-contained. Grab them together.

You don't need `filum`, `tabula_dispersa`, or anything else. Just the string and memory stuff.
