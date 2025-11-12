# Chorda: Strings That Don't Suck

Listen up. You know what the worst part of C is? Null-terminated strings. You know why they're the worst? Because every single operation on them requires scanning the whole thing to find the null terminator. You want the length? Scan. You want to concatenate? Scan both strings. You want a substring? Better make sure you null-terminate it or you're in undefined behavior land.

And don't even get me STARTED on security. How many buffer overflows have happened because someone forgot to check string lengths? How many CVEs exist because `strcpy` doesn't know when to stop? I'll tell you - THOUSANDS. Thousands of security vulnerabilities because somebody in the 70s decided "hey, let's mark the end of strings with a zero byte" and we've been paying for it ever since.

**Chorda** is different. Latin for "string" or "rope" or "chord." It's a length-prefixed string. The length is stored RIGHT THERE in the structure. Every operation is O(1) to get the length. Slicing is just pointer arithmetic. No scanning, no surprises, no buffer overflows because you forgot to check the length.

And yeah, it allocates from piscina, so memory management is dead simple. Are you seeing the pattern here? Simple tools that do one thing well and compose nicely. That's how you write software that doesn't make you want to scream.

## What Is This Thing?

A **chorda** is two things:
1. A length (how many bytes)
2. A pointer to the data

That's it. That's the whole structure:

```c
structura chorda
{
    i32  mensura;  /* Length in bytes */
    i8*  datum;    /* Pointer to data */
};
```

Not null-terminated. The data might HAVE a null byte in it (like if you're reading binary data), but it doesn't need one. The length tells you where it ends.

This means:
- Getting the length is O(1) - just read `mensura`
- Slicing is O(1) - just adjust the pointer and length
- Concatenation is O(n) but you only scan once to copy, not three times (two to get lengths, one to copy)
- You can have binary data with null bytes in it and everything still works
- No buffer overflows from forgetting to null-terminate

## Why You Want This Instead of `char*`

I've had this conversation about ten thousand times, so let's just get it over with.

**Traditional C strings (`char*`):**
```c
/* How long is this? WHO KNOWS. Gotta scan it. */
character* s = "hello world";
integer len = strlen(s);  /* O(n) scan */

/* Want a substring? Better allocate and copy. */
character* sub = malloc(6);
strncpy(sub, s, 5);
sub[5] = '\0';  /* BETTER NOT FORGET THIS */

/* Want to concatenate? Hope you allocated enough space. */
strcat(dest, src);  /* Scans both strings, easy to overflow */
```

**Chorda strings:**
```c
/* Length is right there. Instant. */
chorda s = chorda_ex_literis("hello world", pool);
i32 len = s.mensura;  /* O(1) lookup */

/* Want a substring? Just math. No allocation. */
chorda sub = chorda_sectio(s, ZEPHYRUM, V);  /* O(1) pointer arithmetic */

/* Want to concatenate? One allocation, no surprises. */
chorda result = chorda_concatenare(s1, s2, pool);  /* O(n) but predictable */
```

See the difference? With chorda, you always know how much data you have. No scanning. No guessing. No buffer overflows because you miscounted.

## Latin Vocabulary (Round 2)

Since we're still doing this Latin thing, here's what you need to know:

- **chorda** = string, rope, chord (the string type itself)
- **mensura** = size, measurement, length
- **datum** = data, the actual bytes
- **literis** = letters, from letters (used for C string conversion)
- **sectio** = section, slice
- **transcribere** = to copy, to transcribe
- **concatenare** = to link together, to concatenate
- **aequalis** = equal
- **comparare** = to compare
- **continet** = contains
- **incipit** = begins with
- **terminatur** = ends with
- **invenire** = to find
- **fissio** = splitting, division
- **fructus** = result, fruit (return value)
- **praecidere** = to cut off, to trim
- **minuscula** = lowercase
- **maiuscula** = uppercase
- **fenum** = hay, haystack (in search functions)
- **acus** = needle (in search functions)
- **prefixum** = prefix
- **suffixum** = suffix

Got it? Good. Now let's get to the actual usage.

## Creating Strings

You've got a few options here, depending on what you're starting with.

### From C Strings (Most Common)

```c
Piscina* p = piscina_generare_dynamicum("strings", 4096);

chorda s = chorda_ex_literis("hello world", p);
/* s.mensura = 11 */
/* s.datum points to "hello world" (copied into pool) */
```

`ex_literis` means "from letters" - it takes a null-terminated C string, copies it into the pool, and gives you a chorda. The copy does NOT include the null terminator (you don't need it), just the actual characters.

If you pass `NIHIL` or the pool is null, you get an empty chorda back (mensura=0, datum=NIHIL).

### From Raw Buffers (No Copy)

```c
i8 buffer[100];
/* ... fill buffer with data ... */

chorda s = chorda_ex_buffer(buffer, C);
/* s.mensura = 100 */
/* s.datum points to buffer (NO COPY) */
```

This wraps an existing buffer without copying. Useful when you already have the data and don't want to waste time copying it.

**Warning**: The buffer better stay valid as long as you're using the chorda. If the buffer goes away, your chorda points to garbage.

### Slicing (The Magic Part)

```c
chorda full = chorda_ex_literis("hello world", p);
chorda slice = chorda_sectio(full, ZEPHYRUM, V);
/* slice.mensura = 5 */
/* slice.datum points to "hello" (NO COPY) */
```

`sectio` means "section" or "slice." It takes a start index and an end index (exclusive, like Python), and gives you a view into the original string. No allocation. No copying. Just pointer math.

```c
chorda s = chorda_ex_literis("abcdef", p);

chorda first_three = chorda_sectio(s, ZEPHYRUM, III);  /* "abc" */
chorda last_three  = chorda_sectio(s, III, VI);        /* "def" */
chorda middle_two  = chorda_sectio(s, II, IV);         /* "cd" */
```

This is insanely useful for parsing. You can slice up input without allocating a million temporary strings.

**Important**: The slice shares memory with the original. If you destroy the pool that holds the original, the slice becomes invalid.

### Copying (When You Need Independence)

```c
chorda original = chorda_ex_literis("source", p1);
chorda copy = chorda_transcribere(original, p2);
/* copy is an independent copy in p2 */
```

`transcribere` means "to copy" or "to transcribe." It allocates new memory and copies the bytes. Use this when you need the string to outlive the original, or when you're moving strings between pools.

### Concatenating (Joining Strings)

```c
chorda a = chorda_ex_literis("hello", p);
chorda b = chorda_ex_literis("world", p);
chorda c = chorda_concatenare(a, b, p);
/* c.mensura = 10 */
/* c.datum = "helloworld" */
```

`concatenare` means "to link together." It allocates space for both strings, copies them, and gives you the result.

Smart edge cases:
- If one string is empty, you get a copy of the other
- If both are empty, you get an empty string
- No wasted allocations

```c
chorda greeting = chorda_ex_literis("hello ", p);
chorda name     = chorda_ex_literis("Alice", p);
chorda space    = chorda_ex_literis(" ", p);
chorda punct    = chorda_ex_literis("!", p);

/* Chain them together */
chorda temp   = chorda_concatenare(greeting, name, p);
chorda temp2  = chorda_concatenare(temp, punct, p);
/* temp2 = "hello Alice!" */
```

Yeah, it's a bit verbose. If you're doing a lot of concatenation, use `chorda_aedificator` instead (the string builder). But for simple joining, this works fine.

## Comparing Strings

You've got three ways to compare chorda strings, depending on what you need.

### Equality (Exact Match)

```c
chorda a = chorda_ex_literis("test", p);
chorda b = chorda_ex_literis("test", p);
chorda c = chorda_ex_literis("different", p);

si (chorda_aequalis(a, b))
{
    imprimere("a equals b\n");  /* This prints */
}

si (!chorda_aequalis(a, c))
{
    imprimere("a doesn't equal c\n");  /* This also prints */
}
```

`aequalis` means "equal." It checks if the lengths match, then compares the bytes. Fast and simple.

### Case-Insensitive Equality

```c
chorda lower = chorda_ex_literis("hello", p);
chorda upper = chorda_ex_literis("HELLO", p);
chorda mixed = chorda_ex_literis("HeLLo", p);

b32 same1 = chorda_aequalis_case_insensitivus(lower, upper);  /* VERUM */
b32 same2 = chorda_aequalis_case_insensitivus(lower, mixed);  /* VERUM */
```

Does what it says - compares ignoring case. Useful for configuration keys, command parsing, etc.

### Ordering (Lexicographic Comparison)

```c
chorda a = chorda_ex_literis("apple", p);
chorda b = chorda_ex_literis("banana", p);

s32 result = chorda_comparare(a, b);
/* result < 0  means a < b */
/* result == 0 means a == b */
/* result > 0  means a > b */

si (result < ZEPHYRUM)
{
    imprimere("apple comes before banana\n");
}
```

`comparare` means "to compare." Returns the same convention as `strcmp` - negative if less, zero if equal, positive if greater. Perfect for sorting.

## Searching and Querying

Alright, now we're getting into the useful stuff. You've got a string, you want to find things in it.

### Contains (Substring Search)

```c
chorda haystack = chorda_ex_literis("hello world", p);
chorda needle   = chorda_ex_literis("world", p);

si (chorda_continet(haystack, needle))
{
    imprimere("Found it!\n");
}
```

`continet` means "contains." Simple substring search. Returns `VERUM` if the needle is anywhere in the haystack, `FALSUM` otherwise.

Note the variable names - `fenum` (hay) and `acus` (needle) are the standard Latin terms for haystack and needle. You'll see these throughout the API.

### Starts With / Ends With

```c
chorda path = chorda_ex_literis("/usr/local/bin", p);
chorda prefix = chorda_ex_literis("/usr", p);
chorda suffix = chorda_ex_literis("bin", p);

si (chorda_incipit(path, prefix))
{
    imprimere("Starts with /usr\n");  /* This prints */
}

si (chorda_terminatur(path, suffix))
{
    imprimere("Ends with bin\n");  /* This prints */
}
```

`incipit` = "begins with"
`terminatur` = "ends with"

Super useful for path manipulation, file extension checking, etc. Way faster than doing a full substring search when you know you're checking the edges.

### Find (Get Position)

```c
chorda text = chorda_ex_literis("hello world", p);
chorda search = chorda_ex_literis("world", p);

chorda found = chorda_invenire(text, search);
si (found.mensura > ZEPHYRUM)
{
    imprimere("Found at position: %d\n", (i32)(found.datum - text.datum));
}
alioquin
{
    imprimere("Not found\n");
}
```

`invenire` means "to find." It returns a chorda pointing to the first occurrence of the needle in the haystack. If not found, you get an empty chorda (mensura=0, datum=NIHIL).

You can compute the position by subtracting pointers: `found.datum - haystack.datum`.

### Count Occurrences

```c
chorda text = chorda_ex_literis("abcabcabc", p);
chorda pattern = chorda_ex_literis("abc", p);

i32 count = chorda_numerare_occurrentia(text, pattern);
/* count = 3 */
```

`numerare_occurrentia` = "count occurrences." Finds all non-overlapping matches. Useful for statistics, validation, etc.

## Splitting Strings

This one's a big deal. You've got comma-separated values, or tab-separated, or whatever. You need to split it up.

```c
chorda csv = chorda_ex_literis("alice,bob,charlie", p);
chorda_fissio_fructus result = chorda_fissio(csv, ',', p);

imprimere("Found %d parts\n", result.numerus);

per (i32 i = ZEPHYRUM; i < result.numerus; i++)
{
    chorda part = result.elementa[i];
    character* cstr = chorda_ut_cstr(part, p);
    imprimere("Part %d: %s\n", i, cstr);
}
```

`fissio` means "splitting" or "division." It splits the string by a delimiter character and gives you an array of chorda slices.

The return type is `chorda_fissio_fructus`:
```c
structura chorda_fissio_fructus
{
    chorda* elementa;  /* Array of string slices */
    i32     numerus;   /* How many elements */
};
```

**Important details:**
- The array starts at capacity 16 and doubles if needed
- Empty parts are included (e.g., "a,,b" splits into ["a", "", "b"])
- Trailing delimiters create empty strings (e.g., "a,b," becomes ["a", "b", ""])
- All elementa are slices of the original - no copying
- Everything is allocated from the pool

Example with multiple delimiters:

```c
chorda line = chorda_ex_literis("name:age:city", p);
chorda_fissio_fructus fields = chorda_fissio(line, ':', p);

si (fields.numerus >= III)
{
    character* name = chorda_ut_cstr(fields.elementa[ZEPHYRUM], p);
    character* age  = chorda_ut_cstr(fields.elementa[I], p);
    character* city = chorda_ut_cstr(fields.elementa[II], p);

    imprimere("Name: %s, Age: %s, City: %s\n", name, age, city);
}
```

## Trimming Whitespace

Because nobody likes leading and trailing spaces.

```c
chorda messy = chorda_ex_literis("  hello world  ", p);
chorda clean = chorda_praecidere(messy);
/* clean = "hello world" */
```

`praecidere` means "to cut off" or "to trim." Removes whitespace (spaces, tabs, newlines) from both ends. Returns a slice of the original, so no allocation.

There's also `chorda_praecidi_laterale()` which does the same thing but allocates a copy instead of returning a slice. Use `praecidere` unless you specifically need the copy.

What counts as whitespace? Anything that `isspace()` returns true for: space, tab, newline, carriage return, form feed, vertical tab.

```c
chorda whitespace_hell = chorda_ex_literis("\t\n  test  \n\t", p);
chorda trimmed = chorda_praecidere(whitespace_hell);
/* trimmed = "test" */
```

If the string is ALL whitespace, you get an empty chorda back.

## Case Conversion

Sometimes you need to yell. Sometimes you need to whisper.

### Lowercase

```c
chorda mixed = chorda_ex_literis("HeLLo WoRLd", p);
chorda lower = chorda_minuscula(mixed, p);
/* lower = "hello world" */
```

`minuscula` = "lowercase." Allocates a new string with all characters converted to lowercase. Non-ASCII characters are passed through as-is (this uses `tolower()` from C standard library).

### Uppercase

```c
chorda mixed = chorda_ex_literis("HeLLo WoRLd", p);
chorda upper = chorda_maiuscula(mixed, p);
/* upper = "HELLO WORLD" */
```

`maiuscula` = "uppercase." Same as minuscula but the other direction.

Both of these allocate new strings - they don't modify the original.

## Converting To Other Types

Because eventually you need to interface with the rest of the world.

### To C Strings (Null-Terminated)

```c
chorda s = chorda_ex_literis("test", p);
character* cstr = chorda_ut_cstr(s, p);
/* cstr = "test\0" */

imprimere("%s\n", cstr);  /* Works with printf */
```

`ut_cstr` = "as C string." Allocates `mensura + 1` bytes, copies the data, adds a null terminator. Now you can use it with standard C functions.

**Warning**: This allocates from the pool. If you do this a million times, you're allocating a million strings. Be smart about it.

### To Integers (Parsing)

```c
chorda num_str = chorda_ex_literis("12345", p);
s32 value;

si (chorda_ut_s32(num_str, &value))
{
    imprimere("Parsed: %d\n", value);  /* value = 12345 */
}
alioquin
{
    imprimere("Not a valid number\n");
}
```

`ut_s32` and `ut_i32` convert strings to signed/unsigned 32-bit integers. They return `VERUM` on success, `FALSUM` if the string isn't a valid number.

- `chorda_ut_s32()`: Signed integers (can be negative)
- `chorda_ut_i32()`: Unsigned integers (positive only)

Both use base 10 parsing. They handle leading whitespace, signs, etc., using `strtol()` / `strtoul()` internally.

```c
chorda negative = chorda_ex_literis("-999", p);
s32 neg_value;
si (chorda_ut_s32(negative, &neg_value))
{
    imprimere("Negative: %d\n", neg_value);  /* -999 */
}

chorda invalid = chorda_ex_literis("not a number", p);
i32 dummy;
si (!chorda_ut_i32(invalid, &dummy))
{
    imprimere("Invalid, as expected\n");
}
```

## Hashing (For Hash Tables)

```c
chorda key = chorda_ex_literis("my_key", p);
i32 hash = chorda_friare(key);
```

`friare` means "to hash" (from friare, to rub/crumble). Uses FNV-1a hashing - fast, simple, good distribution. Returns a 32-bit hash.

Use this with `tabula_dispersa` (the hash table library) or whenever you need a hash function.

Same strings always produce the same hash. Different strings almost always produce different hashes (collisions are rare but possible).

## Common Patterns

Alright, let's look at how you actually USE this thing in real code.

### Parsing CSV Files

```c
Piscina* p = piscina_generare_dynamicum("csv", 8192);

chorda file_contents = filum_legere_totum("data.csv", p);
chorda_fissio_fructus lines = chorda_fissio(file_contents, '\n', p);

per (i32 i = ZEPHYRUM; i < lines.numerus; i++)
{
    chorda line = lines.elementa[i];
    chorda_fissio_fructus fields = chorda_fissio(line, ',', p);

    per (i32 j = ZEPHYRUM; j < fields.numerus; j++)
    {
        chorda field = chorda_praecidere(fields.elementa[j]);
        character* cstr = chorda_ut_cstr(field, p);
        imprimere("Field[%d][%d]: %s\n", i, j, cstr);
    }
}

piscina_destruere(p);
```

Notice:
- One pool for everything
- Split lines, then split fields
- Trim whitespace from each field
- Convert to C string for printing
- One `piscina_destruere()` at the end cleans up EVERYTHING

### Parsing Key-Value Configs

```c
chorda line = chorda_ex_literis("timeout = 30", p);

chorda_fissio_fructus parts = chorda_fissio(line, '=', p);
si (parts.numerus == II)
{
    chorda key   = chorda_praecidere(parts.elementa[ZEPHYRUM]);
    chorda value = chorda_praecidere(parts.elementa[I]);

    si (chorda_aequalis_case_insensitivus(
        key,
        chorda_ex_literis("timeout", p)))
    {
        i32 timeout_value;
        si (chorda_ut_i32(value, &timeout_value))
        {
            imprimere("Timeout set to %d\n", timeout_value);
        }
    }
}
```

Pattern:
- Split on delimiter
- Trim whitespace
- Case-insensitive key comparison
- Parse value to integer

### Building Paths

```c
chorda dir  = chorda_ex_literis("/usr/local", p);
chorda sep  = chorda_ex_literis("/", p);
chorda file = chorda_ex_literis("bin", p);

chorda temp = chorda_concatenare(dir, sep, p);
chorda path = chorda_concatenare(temp, file, p);
/* path = "/usr/local/bin" */
```

Yeah, it's verbose. For complex path stuff, use the `via` library which has proper path joining. But for simple concatenation, this works.

### String Validation

```c
b32 is_valid_identifier(chorda s)
{
    si (s.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Must start with letter or underscore */
    character first = (character)s.datum[ZEPHYRUM];
    si (!isalpha(first) && first != '_')
    {
        redde FALSUM;
    }

    /* Rest must be alphanumeric or underscore */
    per (i32 i = I; i < s.mensura; i++)
    {
        character c = (character)s.datum[i];
        si (!isalnum(c) && c != '_')
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}
```

Direct byte access is totally fine. You know the length, you can iterate safely.

### String Interning (Hash Table Keys)

```c
TabulaDispersa* intern_table = tabula_dispersa_creare_chorda(p, CXXVIII);

chorda key = chorda_ex_literis("some_key", p);

chorda* stored;
si (tabula_dispersa_invenire(intern_table, key, (vacuum**)&stored))
{
    /* Already interned, use stored version */
    redde *stored;
}
alioquin
{
    /* Not interned yet, store it */
    chorda* copy = piscina_allocare(p, magnitudo(chorda));
    *copy = chorda_transcribere(key, p);
    tabula_dispersa_inserere(intern_table, *copy, copy);
    redde *copy;
}
```

Pattern: deduplicate strings by storing them in a hash table, reuse pointers for equality checks.

## Things That Will Ruin Your Day (Pay Attention)

### Chorda Strings Are NOT Null-Terminated

I know I already said this, but people KEEP forgetting, so I'm saying it AGAIN.

```c
/* BAD - DO NOT DO THIS */
chorda s = chorda_ex_literis("test", p);
imprimere("%s\n", s.datum);  /* WRONG - might crash or print garbage */
```

If you need a C string, use `chorda_ut_cstr()`:

```c
/* CORRECT */
chorda s = chorda_ex_literis("test", p);
character* cstr = chorda_ut_cstr(s, p);
imprimere("%s\n", cstr);  /* Now it works */
```

### Slices Share Memory With The Original

```c
chorda original = chorda_ex_literis("hello", p);
chorda slice = chorda_sectio(original, ZEPHYRUM, II);

piscina_destruere(p);

/* Now BOTH original and slice are invalid */
imprimere("%d\n", slice.mensura);  /* Reading freed memory - UNDEFINED */
```

If you need the slice to outlive the original, use `chorda_transcribere()` to make a copy.

### Empty Strings Have `datum = NIHIL`

```c
chorda empty = chorda_ex_literis("", p);
/* empty.mensura = 0 */
/* empty.datum = NIHIL */

si (empty.datum)  /* This is FALSUM for empty strings */
{
    /* Won't execute */
}
```

Always check `mensura` first, not `datum`:

```c
si (s.mensura > ZEPHYRUM)
{
    /* Non-empty, safe to access s.datum */
}
```

### Conversion Functions Allocate Memory

Every time you call `chorda_ut_cstr()`, you allocate from the pool:

```c
/* BAD - allocates 1000 strings */
per (i32 i = ZEPHYRUM; i < M; i++)
{
    character* cstr = chorda_ut_cstr(some_chorda, p);
    imprimere("%s\n", cstr);
}
```

If you're doing this in a loop, either reuse the pool or clear it:

```c
/* BETTER */
per (i32 i = ZEPHYRUM; i < M; i++)
{
    character* cstr = chorda_ut_cstr(some_chorda, p);
    imprimere("%s\n", cstr);
    piscina_vacare(p);  /* Clear for next iteration */
}
```

### Don't Mix Pools

```c
/* BAD */
Piscina* p1 = piscina_generare_dynamicum("pool1", 1024);
Piscina* p2 = piscina_generare_dynamicum("pool2", 1024);

chorda s1 = chorda_ex_literis("test", p1);
chorda s2 = chorda_transcribere(s1, p2);  /* s2 is in p2 */

piscina_destruere(p1);  /* s1 is now invalid, but s2 is still valid */

character* cstr = chorda_ut_cstr(s2, p1);  /* BAD - p1 is destroyed */
```

Keep track of which pool owns which string. If you destroy a pool, every chorda allocated from it becomes invalid.

## API Reference

Time for the boring part where I list every function. You probably figured most of this out already, but here it is for completeness.

### Creation & Construction

#### `chorda_ex_literis()`

```c
chorda chorda_ex_literis(
    constans character* cstr,
               Piscina* piscina
);
```

Creates a chorda from a null-terminated C string.

**Parameters:**
- `cstr`: C string to copy (can be `NIHIL`)
- `piscina`: Pool to allocate from

**Returns:** New chorda with copied data, or empty chorda if `cstr` is `NIHIL`

**Example:**
```c
chorda s = chorda_ex_literis("hello", p);
```

---

#### `chorda_ex_buffer()`

```c
chorda chorda_ex_buffer(i8* buffer, i32 mensura);
```

Wraps an existing buffer as a chorda (no copy).

**Parameters:**
- `buffer`: Pointer to existing data
- `mensura`: Length in bytes

**Returns:** Chorda pointing to the buffer

**Warning:** Buffer must remain valid while chorda is used

**Example:**
```c
i8 data[100];
chorda s = chorda_ex_buffer(data, C);
```

---

#### `chorda_sectio()`

```c
chorda chorda_sectio(chorda s, i32 initium, i32 finis);
```

Creates a slice (substring) without copying.

**Parameters:**
- `s`: Source string
- `initium`: Start index (inclusive)
- `finis`: End index (exclusive)

**Returns:** Slice of the original string

**Example:**
```c
chorda full = chorda_ex_literis("abcdef", p);
chorda slice = chorda_sectio(full, II, V);  /* "cde" */
```

---

#### `chorda_transcribere()`

```c
chorda chorda_transcribere(chorda s, Piscina* piscina);
```

Creates an independent copy of a string.

**Parameters:**
- `s`: Source string
- `piscina`: Pool to allocate copy in

**Returns:** New chorda with copied data

**Example:**
```c
chorda copy = chorda_transcribere(original, p);
```

---

#### `chorda_concatenare()`

```c
chorda chorda_concatenare(
    chorda  a,
    chorda  b,
    Piscina* piscina
);
```

Concatenates two strings.

**Parameters:**
- `a`: First string
- `b`: Second string
- `piscina`: Pool to allocate result in

**Returns:** New chorda containing a + b

**Example:**
```c
chorda result = chorda_concatenare(s1, s2, p);
```

---

#### `chorda_praecidi_laterale()`

```c
chorda chorda_praecidi_laterale(chorda s, Piscina* piscina);
```

Trims whitespace and returns a copy.

**Parameters:**
- `s`: String to trim
- `piscina`: Pool to allocate result in

**Returns:** New trimmed chorda

**Example:**
```c
chorda trimmed = chorda_praecidi_laterale(messy, p);
```

---

### Comparison

#### `chorda_aequalis()`

```c
b32 chorda_aequalis(chorda a, chorda b);
```

Tests if two strings are equal (exact match).

**Returns:** `VERUM` if equal, `FALSUM` otherwise

**Example:**
```c
si (chorda_aequalis(s1, s2))
{
    imprimere("Equal\n");
}
```

---

#### `chorda_aequalis_case_insensitivus()`

```c
b32 chorda_aequalis_case_insensitivus(chorda a, chorda b);
```

Tests if two strings are equal (ignoring case).

**Returns:** `VERUM` if equal ignoring case, `FALSUM` otherwise

**Example:**
```c
si (chorda_aequalis_case_insensitivus(
    chorda_ex_literis("Hello", p),
    chorda_ex_literis("HELLO", p)))
{
    imprimere("Same (ignoring case)\n");
}
```

---

#### `chorda_comparare()`

```c
s32 chorda_comparare(chorda a, chorda b);
```

Lexicographic comparison.

**Returns:**
- `< 0` if a < b
- `== 0` if a == b
- `> 0` if a > b

**Example:**
```c
si (chorda_comparare(s1, s2) < ZEPHYRUM)
{
    imprimere("s1 comes before s2\n");
}
```

---

### Query & Search

#### `chorda_continet()`

```c
b32 chorda_continet(chorda fenum, chorda acus);
```

Tests if haystack contains needle.

**Parameters:**
- `fenum`: Haystack (string to search in)
- `acus`: Needle (substring to find)

**Returns:** `VERUM` if found, `FALSUM` otherwise

**Example:**
```c
si (chorda_continet(text, pattern))
{
    imprimere("Found it!\n");
}
```

---

#### `chorda_incipit()`

```c
b32 chorda_incipit(chorda s, chorda prefixum);
```

Tests if string starts with prefix.

**Returns:** `VERUM` if s starts with prefixum

**Example:**
```c
si (chorda_incipit(path, chorda_ex_literis("/usr", p)))
{
    imprimere("Unix path\n");
}
```

---

#### `chorda_terminatur()`

```c
b32 chorda_terminatur(chorda s, chorda suffixum);
```

Tests if string ends with suffix.

**Returns:** `VERUM` if s ends with suffixum

**Example:**
```c
si (chorda_terminatur(filename, chorda_ex_literis(".txt", p)))
{
    imprimere("Text file\n");
}
```

---

#### `chorda_invenire()`

```c
chorda chorda_invenire(chorda fenum, chorda acus);
```

Finds first occurrence of needle in haystack.

**Returns:** Chorda pointing to match, or empty chorda if not found

**Example:**
```c
chorda found = chorda_invenire(text, pattern);
si (found.mensura > ZEPHYRUM)
{
    imprimere("Found at offset %d\n", (i32)(found.datum - text.datum));
}
```

---

#### `chorda_numerare_occurrentia()`

```c
i32 chorda_numerare_occurrentia(chorda fenum, chorda acus);
```

Counts non-overlapping occurrences of needle in haystack.

**Returns:** Number of occurrences

**Example:**
```c
i32 count = chorda_numerare_occurrentia(text, pattern);
imprimere("Found %d times\n", count);
```

---

### Splitting

#### `chorda_fissio()`

```c
chorda_fissio_fructus chorda_fissio(
    chorda  s,
    character  delim,
    Piscina* piscina
);
```

Splits string by delimiter.

**Parameters:**
- `s`: String to split
- `delim`: Delimiter character
- `piscina`: Pool for array allocation

**Returns:** Structure with array of chorda slices

```c
structura chorda_fissio_fructus
{
    chorda* elementa;  /* Array of slices */
    i32     numerus;   /* Number of elements */
};
```

**Example:**
```c
chorda_fissio_fructus parts = chorda_fissio(csv, ',', p);
per (i32 i = ZEPHYRUM; i < parts.numerus; i++)
{
    /* Process parts.elementa[i] */
}
```

---

### Manipulation

#### `chorda_praecidere()`

```c
chorda chorda_praecidere(chorda s);
```

Trims whitespace from both ends (returns slice).

**Returns:** Slice of original string with whitespace removed

**Example:**
```c
chorda trimmed = chorda_praecidere(chorda_ex_literis("  test  ", p));
```

---

#### `chorda_minuscula()`

```c
chorda chorda_minuscula(chorda s, Piscina* piscina);
```

Converts string to lowercase.

**Parameters:**
- `s`: Source string
- `piscina`: Pool for result

**Returns:** New lowercase string

**Example:**
```c
chorda lower = chorda_minuscula(mixed_case, p);
```

---

#### `chorda_maiuscula()`

```c
chorda chorda_maiuscula(chorda s, Piscina* piscina);
```

Converts string to uppercase.

**Parameters:**
- `s`: Source string
- `piscina`: Pool for result

**Returns:** New uppercase string

**Example:**
```c
chorda upper = chorda_maiuscula(mixed_case, p);
```

---

### Conversion

#### `chorda_ut_cstr()`

```c
character* chorda_ut_cstr(chorda s, Piscina* piscina);
```

Converts to null-terminated C string.

**Parameters:**
- `s`: Source string
- `piscina`: Pool for result

**Returns:** Null-terminated C string, or `NIHIL` if s is empty

**Example:**
```c
character* cstr = chorda_ut_cstr(s, p);
imprimere("%s\n", cstr);
```

---

#### `chorda_ut_s32()`

```c
b32 chorda_ut_s32(chorda s, s32* fructus);
```

Parses string as signed 32-bit integer.

**Parameters:**
- `s`: String to parse
- `fructus`: Output parameter for result

**Returns:** `VERUM` on success, `FALSUM` if not a valid number

**Example:**
```c
s32 value;
si (chorda_ut_s32(num_str, &value))
{
    imprimere("Parsed: %d\n", value);
}
```

---

#### `chorda_ut_i32()`

```c
b32 chorda_ut_i32(chorda s, i32* fructus);
```

Parses string as unsigned 32-bit integer.

**Parameters:**
- `s`: String to parse
- `fructus`: Output parameter for result

**Returns:** `VERUM` on success, `FALSUM` if not a valid number

**Example:**
```c
i32 value;
si (chorda_ut_i32(num_str, &value))
{
    imprimere("Parsed: %u\n", value);
}
```

---

### Hashing

#### `chorda_friare()`

```c
i32 chorda_friare(chorda s);
```

Computes FNV-1a hash.

**Returns:** 32-bit hash value

**Example:**
```c
i32 hash = chorda_friare(key);
```

---

## FAQ (Because You're Going To Ask Anyway)

### Q: Why not just use null-terminated strings?

Because they're slow and dangerous. Every operation requires scanning to find the null terminator. That's O(n) just to get the LENGTH. With chorda, it's O(1) - just read the `mensura` field.

Plus, null-terminated strings can't handle binary data with embedded nulls. Chorda can.

### Q: Can I put null bytes in a chorda?

Yes. Chorda stores the length explicitly, so null bytes are just another byte. This is actually useful for binary protocols, encryption, etc.

```c
i8 data[V] = {I, ZEPHYRUM, II, ZEPHYRUM, III};
chorda binary = chorda_ex_buffer(data, V);
/* Works fine, even with null bytes */
```

### Q: Are chorda strings mutable?

The chorda structure itself (mensura and datum) is just a value you can copy around. But the DATA it points to shouldn't be modified directly - allocate a new string instead.

If you need to build strings incrementally, use `chorda_aedificator` (the string builder).

### Q: What happens if I slice past the end?

You get an empty string (mensura=0, datum=NIHIL). The function checks bounds and returns a safe empty chorda if the indices are invalid.

```c
chorda s = chorda_ex_literis("test", p);
chorda bad = chorda_sectio(s, X, XX);
/* bad.mensura = 0, bad.datum = NIHIL */
```

### Q: Can I compare chorda strings with `==`?

NO. That compares the STRUCTURE (pointers and lengths), not the actual string content.

```c
/* WRONG */
si (s1 == s2)  /* This compares pointers, not content */

/* CORRECT */
si (chorda_aequalis(s1, s2))  /* This compares content */
```

### Q: How do I iterate over characters?

Direct array access. You know the length, so it's safe:

```c
chorda s = chorda_ex_literis("hello", p);
per (i32 i = ZEPHYRUM; i < s.mensura; i++)
{
    character c = (character)s.datum[i];
    imprimere("%c\n", c);
}
```

### Q: What about Unicode/UTF-8?

Chorda treats strings as byte arrays. If you put UTF-8 in, you get UTF-8 out. Operations like slicing work on BYTES, not characters.

Be careful slicing UTF-8 - you might cut in the middle of a multi-byte character. If you need proper Unicode handling, you'll need a higher-level library.

### Q: Why allocate from a pool instead of returning stack strings?

Because C89 doesn't have good support for returning structures efficiently. Plus, allocating from a pool is fast and gives you automatic lifetime management.

If you REALLY need stack strings, you can do:

```c
character buffer[256];
chorda s = chorda_ex_buffer(buffer, ZEPHYRUM);
/* Build string in buffer */
s.mensura = strlen(buffer);
```

But most of the time, pool allocation is cleaner.

### Q: Can I reuse a chorda across multiple pool clears?

NO. After `piscina_vacare()`, any chorda allocated from that pool is invalid:

```c
chorda s = chorda_ex_literis("test", p);
piscina_vacare(p);
/* s is now invalid - don't use it */
```

If you need to preserve strings across clears, copy them to a different pool first.

### Q: How do I sort an array of chorda?

Use `chorda_comparare()` as your comparison function:

```c
s32 compare_chordas(constans vacuum* a, constans vacuum* b)
{
    constans chorda* s1 = (constans chorda*)a;
    constans chorda* s2 = (constans chorda*)b;
    redde chorda_comparare(*s1, *s2);
}

/* Then use with qsort */
qsort(array, count, magnitudo(chorda), compare_chordas);
```

### Q: What's the maximum string length?

Mensura is an `i32`, so theoretically 2GB. In practice, your pool size limits it. If you're working with strings bigger than a few MB, you might want a different approach.

### Q: Do I need to free individual chordas?

NO. The whole point of using pools is that you DON'T free individual allocations. You destroy the pool when you're done with everything.

### Q: Can I use chorda in hash tables?

Yes. Use `chorda_friare()` for the hash function and `chorda_aequalis()` for key comparison. Or use `tabula_dispersa_creare_chorda()` which sets this up automatically.

### Q: What's the performance compared to C strings?

Better for almost everything:
- Length: O(1) vs O(n)
- Slicing: O(1) vs O(n)
- Concatenation: O(n) vs O(n), but no scanning overhead
- Comparison: O(n) both, but chorda can early-exit on length mismatch

Only thing slower is creating from C strings (requires a copy), but that's usually a one-time cost.

---

Alright, that's chorda. It's not rocket science. You store the length with the data, you allocate from pools, you don't null-terminate. Simple, fast, safe.

If you can't figure it out from here, I don't know what to tell you. Read the examples again. Look at the tests. Copy the patterns.

Now stop asking me questions and go write some code.
