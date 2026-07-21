# ChordaAedificator: The String Builder You Wish You Had Earlier

Okay, so you read the chorda documentation and you're thinking "that's great for strings I already have, but what if I'm BUILDING a string piece by piece?" Good question. That's exactly what this is for.

**ChordaAedificator** - Latin for "string builder" (aedificator = builder, from aedificare, to build). It's a growable buffer that lets you append strings, numbers, whatever, and automatically handles the memory management. No manual realloc, no buffer overflow checking, no counting bytes. Just append and go.

You know what the alternatives are? Either you:
1. Manually track a buffer size and realloc when you need more space (tedious, error-prone)
2. Allocate a huge fixed buffer and hope it's big enough (wasteful, still can overflow)
3. Use `snprintf` chains and pray you got the math right (good luck with that)

ChordaAedificator does what every string builder in every modern language does - it grows automatically when it needs to, and you don't have to think about it. Simple.

## What Is This Thing?

It's a wrapper around a growing buffer. Internally it's:
- A buffer (the actual bytes)
- A capacity (how big the buffer is)
- An offset (how much you've used)
- A piscina (for allocations)
- An indentation level (for pretty-printing)

You append stuff to it, it grows when needed, and when you're done you get a chorda back. That's literally it.

```c
Piscina* p = piscina_generare_dynamicum("builder", 4096);
ChordaAedificator* builder = chorda_aedificator_creare(p, CCLVI);

chorda_aedificator_appendere_literis(builder, "Hello ");
chorda_aedificator_appendere_literis(builder, "world");

chorda result = chorda_aedificator_finire(builder);
/* result = "Hello world" */
```

Way cleaner than manually managing buffers and offsets.

## Latin Vocabulary (Round 3)

More Latin words because we're committed to this bit:

- **aedificator** = builder (from aedificare, to build)
- **appendere** = to append, to hang on to
- **creare** = to create
- **destruere** = to destroy
- **finire** = to finish, to complete
- **reset** = to reset (okay this one's English in disguise)
- **literis** = letters, from letters (C string)
- **lineam_novam** = new line (linea = line, nova = new)
- **indentationem** = indentation
- **push/pop** = push/pop (also English, deal with it)
- **gradus** = level, degree, step
- **longitudo** = length
- **spectare** = to look at, to view
- **evasus** = escaped
- **character** = character
- **s32/i32/f64** = signed 32-bit, unsigned 32-bit, 64-bit float

Got it? Good.

## Why You Need This

Let's say you're building JSON. Or generating HTML. Or constructing SQL queries (don't do this, but hypothetically). You could do:

```c
/* THE BAD WAY */
character buffer[1024];  /* Hope this is big enough! */
i32 offset = ZEPHYRUM;

offset += snprintf(buffer + offset, 1024 - offset, "{");
offset += snprintf(buffer + offset, 1024 - offset, "\"name\":\"");
offset += snprintf(buffer + offset, 1024 - offset, "%s", user_input);
offset += snprintf(buffer + offset, 1024 - offset, "\",");
/* ... and so on, tracking offset the whole time ... */
/* ... and hoping you don't overflow ... */
```

Or you could do:

```c
/* THE GOOD WAY */
ChordaAedificator* b = chorda_aedificator_creare(p, CCLVI);

chorda_aedificator_appendere_character(b, '{');
chorda_aedificator_appendere_literis(b, "\"name\":\"");
chorda_aedificator_appendere_literis_evasus_json(b, user_input);
chorda_aedificator_appendere_literis(b, "\",");

chorda json = chorda_aedificator_finire(b);
```

No offset tracking. No size checking. No overflow worries. It just works.

## Creating A Builder

```c
Piscina* p = piscina_generare_dynamicum("strings", 4096);
ChordaAedificator* builder = chorda_aedificator_creare(p, CCLVI);
```

The second parameter is the initial capacity in bytes. Pick something reasonable based on what you're building:
- Small strings (< 256 bytes): Use 256 or 512
- Medium strings (< 4KB): Use 1024 or 2048
- Large strings: Use 4096 or 8192

If you exceed the capacity, it automatically doubles. So starting small is fine, it just means you'll trigger more reallocations.

**Important**: The builder allocates from the pool you give it. When you destroy the pool, the builder and everything it allocated goes away.

## Destroying A Builder

```c
chorda_aedificator_destruere(builder);
```

Wait, actually... you barely need to call this. The pool owns all the memory, so when you destroy the pool, everything goes away including the builder. This function basically does nothing except exist for API symmetry.

In practice:
```c
Piscina* p = piscina_generare_dynamicum("temp", 4096);
ChordaAedificator* b = chorda_aedificator_creare(p, CCLVI);

/* ... build string ... */

chorda result = chorda_aedificator_finire(b);
/* Don't even need chorda_aedificator_destruere() */
piscina_destruere(p);  /* This cleans up everything */
```

So yeah, you can call it if you want. It won't hurt. But it also won't really do anything.

## Appending Stuff

Here's where it gets useful. You've got a builder, now you want to put stuff in it.

### Characters

```c
chorda_aedificator_appendere_character(builder, 'A');
chorda_aedificator_appendere_character(builder, 'B');
chorda_aedificator_appendere_character(builder, 'C');
/* Result: "ABC" */
```

One character at a time. Good for when you know exactly what you're doing.

### C Strings

```c
chorda_aedificator_appendere_literis(builder, "Hello ");
chorda_aedificator_appendere_literis(builder, "world");
/* Result: "Hello world" */
```

`literis` = "letters" or "from letters." Takes a null-terminated C string and appends it.

### Chorda Strings

```c
chorda s1 = chorda_ex_literis("prefix", p);
chorda s2 = chorda_ex_literis("suffix", p);

chorda_aedificator_appendere_chorda(builder, s1);
chorda_aedificator_appendere_chorda(builder, s2);
/* Result: "prefixsuffix" */
```

Appends an existing chorda. Useful when you're mixing built strings with existing strings.

### Integers (Signed)

```c
chorda_aedificator_appendere_s32(builder, XLII);
/* Result: "42" */

chorda_aedificator_appendere_s32(builder, -CXXIII);
/* Result: "-123" */
```

Converts a signed 32-bit integer to decimal and appends it. Handles negatives automatically.

### Integers (Unsigned)

```c
chorda_aedificator_appendere_i32(builder, CCLVI);
/* Result: "256" */
```

Same but for unsigned integers. No negative sign obviously.

### Floating Point

```c
chorda_aedificator_appendere_f64(builder, 3.14159, II);
/* Result: "3.14" (2 decimal places) */

chorda_aedificator_appendere_f64(builder, 2.71828, IV);
/* Result: "2.7183" (4 decimal places) */
```

The second parameter is how many decimal places you want. Range is 0-30. It uses standard rounding.

### Newlines

```c
chorda_aedificator_appendere_literis(builder, "line 1");
chorda_aedificator_appendere_lineam_novam(builder);
chorda_aedificator_appendere_literis(builder, "line 2");
/* Result: "line 1\nline 2" */
```

Just appends `\n`. Could you do this with `appendere_character(builder, '\n')`? Yes. But this is clearer.

## JSON Escaping (The Useful Part)

If you're building JSON (or any format that needs escaping), you need to escape special characters. ChordaAedificator has this built in.

```c
chorda_aedificator_appendere_character(builder, '"');
chorda_aedificator_appendere_literis_evasus_json(builder, "hello\"world");
chorda_aedificator_appendere_character(builder, '"');
/* Result: "\"hello\\\"world\"" */
```

`evasus_json` = "escaped JSON." It handles:
- `"` → `\"`
- `\` → `\\`
- `\n` → `\\n`
- `\r` → `\\r`
- `\t` → `\\t`
- Control characters → `\uXXXX`

There's also `chorda_aedificator_appendere_evasus_json()` which takes a chorda instead of a C string.

```c
chorda unsafe = chorda_ex_literis("test\nvalue", p);

chorda_aedificator_appendere_character(builder, '"');
chorda_aedificator_appendere_evasus_json(builder, unsafe);
chorda_aedificator_appendere_character(builder, '"');
/* Result: "\"test\\nvalue\"" */
```

This is HUGE for building JSON without manually tracking what needs escaping.

## Indentation (For Pretty-Printing)

If you're generating structured text (JSON, XML, code, whatever), you want indentation. ChordaAedificator tracks this for you.

```c
ChordaAedificator* b = chorda_aedificator_creare(p, CCLVI);

chorda_aedificator_appendere_literis(b, "{");
chorda_aedificator_appendere_lineam_novam(b);

/* Increase indentation */
chorda_aedificator_push_indentationem(b);

/* Append indentation (2 spaces per level) */
chorda_aedificator_appendere_indentationem(b,
    chorda_aedificator_indentatio_gradus(b));
chorda_aedificator_appendere_literis(b, "\"key\": \"value\"");
chorda_aedificator_appendere_lineam_novam(b);

/* Decrease indentation */
chorda_aedificator_pop_indentationem(b);

chorda_aedificator_appendere_literis(b, "}");

chorda result = chorda_aedificator_finire(b);
/* Result:
{
  "key": "value"
}
*/
```

How it works:
- `push_indentationem()` - Increase indentation level by 1
- `pop_indentationem()` - Decrease indentation level by 1
- `indentatio_gradus()` - Get current indentation level
- `appendere_indentationem(level)` - Append `level * 2` spaces

By default, each indentation level is 2 spaces. This is defined by `CHORDA_AEDIFICATOR_INDENTATIO_SPATIA` if you want to change it (but why would you? 2 spaces is correct).

The pattern is:
1. Push indentation when entering a nested structure
2. Append indentation at the start of each line
3. Pop indentation when leaving the structure

```c
chorda_aedificator_appendere_literis(b, "{");
chorda_aedificator_push_indentationem(b);

chorda_aedificator_appendere_lineam_novam(b);
chorda_aedificator_appendere_indentationem(b, chorda_aedificator_indentatio_gradus(b));
chorda_aedificator_appendere_literis(b, "nested");

chorda_aedificator_pop_indentationem(b);
chorda_aedificator_appendere_lineam_novam(b);
chorda_aedificator_appendere_literis(b, "}");
```

Clean, simple, no manual space counting.

## Checking What You've Built

Sometimes you want to see what you've got so far without finishing the builder.

### Get Length

```c
memoriae_index len = chorda_aedificator_longitudo(builder);
imprimere("Current length: %zu bytes\n", len);
```

Returns how many bytes you've appended so far.

### Peek At Content

```c
chorda view = chorda_aedificator_spectare(builder);
/* view is a chorda pointing to the current content */
/* Builder is still valid, you can keep appending */
```

`spectare` = "to look at" or "to view." Gives you a chorda view of the current content WITHOUT finishing the builder. You can keep appending after this.

**Warning**: The view is only valid until the next operation. If the builder grows (reallocates), the view becomes invalid. So use it immediately or copy it.

## Finishing The Build

When you're done appending, get your final string:

```c
chorda result = chorda_aedificator_finire(builder);
/* result is a chorda with the complete string */
```

`finire` = "to finish." Returns a chorda containing everything you appended.

**Important**: After calling `finire()`, the builder is... well, it still exists, but the chorda points to its internal buffer. Don't use the builder anymore after calling `finire()`. It's done.

Actually, you CAN keep using it, but the chorda you got back will get overwritten. So don't do that. Just call `finire()` when you're done.

## Resetting For Reuse

If you want to build multiple strings with the same builder:

```c
ChordaAedificator* b = chorda_aedificator_creare(p, CCLVI);

/* Build first string */
chorda_aedificator_appendere_literis(b, "first");
chorda s1 = chorda_aedificator_finire(b);

/* Reset and build second string */
chorda_aedificator_reset(b);
chorda_aedificator_appendere_literis(b, "second");
chorda s2 = chorda_aedificator_finire(b);
```

`reset()` clears the buffer back to empty. The capacity stays the same, so no reallocation needed. Resets indentation level to 0 too.

This is useful in loops where you're building similar strings repeatedly.

## Common Patterns

Let's look at real usage because that's what matters.

### Building JSON Objects

```c
ChordaAedificator* b = chorda_aedificator_creare(p, D);

chorda_aedificator_appendere_character(b, '{');
chorda_aedificator_appendere_lineam_novam(b);
chorda_aedificator_push_indentationem(b);

/* First field */
chorda_aedificator_appendere_indentationem(b,
    chorda_aedificator_indentatio_gradus(b));
chorda_aedificator_appendere_literis(b, "\"name\": \"");
chorda_aedificator_appendere_literis_evasus_json(b, user_name);
chorda_aedificator_appendere_literis(b, "\",");
chorda_aedificator_appendere_lineam_novam(b);

/* Second field */
chorda_aedificator_appendere_indentationem(b,
    chorda_aedificator_indentatio_gradus(b));
chorda_aedificator_appendere_literis(b, "\"age\": ");
chorda_aedificator_appendere_i32(b, user_age);
chorda_aedificator_appendere_lineam_novam(b);

chorda_aedificator_pop_indentationem(b);
chorda_aedificator_appendere_character(b, '}');

chorda json = chorda_aedificator_finire(b);
```

Result:
```json
{
  "name": "Alice",
  "age": 25
}
```

Clean, readable, automatically handles escaping and indentation.

### Building CSV

```c
ChordaAedificator* b = chorda_aedificator_creare(p, CCLVI);

per (i32 i = ZEPHYRUM; i < row_count; i++)
{
    chorda_aedificator_appendere_literis(b, rows[i].name);
    chorda_aedificator_appendere_character(b, ',');
    chorda_aedificator_appendere_i32(b, rows[i].id);
    chorda_aedificator_appendere_character(b, ',');
    chorda_aedificator_appendere_literis(b, rows[i].status);

    si (i < row_count - I)
    {
        chorda_aedificator_appendere_lineam_novam(b);
    }
}

chorda csv = chorda_aedificator_finire(b);
```

### Building SQL Queries (Don't Actually Do This)

```c
/* DISCLAIMER: Use parameterized queries instead. But if you must... */

ChordaAedificator* b = chorda_aedificator_creare(p, CCLVI);

chorda_aedificator_appendere_literis(b, "SELECT * FROM users WHERE name = '");
/* Should escape SQL here, but we don't have a function for that */
chorda_aedificator_appendere_literis(b, username);
chorda_aedificator_appendere_literis(b, "' AND age > ");
chorda_aedificator_appendere_i32(b, min_age);

chorda query = chorda_aedificator_finire(b);
```

Seriously though, use parameterized queries. This is just an example.

### Building HTML

```c
ChordaAedificator* b = chorda_aedificator_creare(p, M);

chorda_aedificator_appendere_literis(b, "<!DOCTYPE html>");
chorda_aedificator_appendere_lineam_novam(b);
chorda_aedificator_appendere_literis(b, "<html>");
chorda_aedificator_appendere_lineam_novam(b);
chorda_aedificator_push_indentationem(b);

chorda_aedificator_appendere_indentationem(b, chorda_aedificator_indentatio_gradus(b));
chorda_aedificator_appendere_literis(b, "<body>");
chorda_aedificator_appendere_lineam_novam(b);
chorda_aedificator_push_indentationem(b);

chorda_aedificator_appendere_indentationem(b, chorda_aedificator_indentatio_gradus(b));
chorda_aedificator_appendere_literis(b, "<h1>");
chorda_aedificator_appendere_literis(b, page_title);
chorda_aedificator_appendere_literis(b, "</h1>");
chorda_aedificator_appendere_lineam_novam(b);

chorda_aedificator_pop_indentationem(b);
chorda_aedificator_appendere_indentationem(b, chorda_aedificator_indentatio_gradus(b));
chorda_aedificator_appendere_literis(b, "</body>");
chorda_aedificator_appendere_lineam_novam(b);

chorda_aedificator_pop_indentationem(b);
chorda_aedificator_appendere_literis(b, "</html>");

chorda html = chorda_aedificator_finire(b);
```

You get the idea. Any structured text format benefits from this.

### Building Log Messages

```c
ChordaAedificator* b = chorda_aedificator_creare(p, CXXVIII);

chorda_aedificator_appendere_character(b, '[');
chorda_aedificator_appendere_literis(b, level);  /* "INFO", "ERROR", etc */
chorda_aedificator_appendere_literis(b, "] ");
chorda_aedificator_appendere_literis(b, timestamp);
chorda_aedificator_appendere_literis(b, ": ");
chorda_aedificator_appendere_literis(b, message);

chorda log_line = chorda_aedificator_finire(b);
/* Result: "[INFO] 2025-01-15 10:30:45: User logged in" */
```

## How Growth Works

You probably don't care about this, but in case you're curious:

1. You create a builder with initial capacity (say, 256 bytes)
2. You append stuff until you hit 256 bytes
3. The builder allocates a new buffer of 512 bytes (double)
4. It copies the old content to the new buffer
5. Now you have 512 bytes to work with
6. Hit 512? It doubles to 1024
7. Hit 1024? Doubles to 2048
8. And so on

This is exponential growth - same strategy as every dynamic array. It means you do O(log n) allocations for n bytes, and the amortized cost per byte is O(1).

In practice, pick a reasonable initial capacity and you'll probably never trigger growth. But if you do, it's handled automatically.

## Things That Will Bite You

### Don't Use The Builder After finire()

Well, you CAN, but:

```c
ChordaAedificator* b = chorda_aedificator_creare(p, CCLVI);
chorda_aedificator_appendere_literis(b, "first");
chorda s1 = chorda_aedificator_finire(b);

/* If you keep appending... */
chorda_aedificator_appendere_literis(b, " second");
/* s1 is now corrupted because it points to the same buffer */
```

The chorda you got from `finire()` points to the builder's internal buffer. If you modify the builder, you're modifying what the chorda points to.

Solution: Either call `finire()` last, or use `reset()` to start fresh (which invalidates previous chordas).

### Spectare Views Are Temporary

```c
chorda view = chorda_aedificator_spectare(b);
chorda_aedificator_appendere_literis(b, "more stuff");
/* If this triggers growth, view is now invalid */
```

The view returned by `spectare()` is only valid until the buffer grows. Use it immediately or copy it to a safe location.

### Reset Clears Everything

```c
chorda_aedificator_appendere_literis(b, "important data");
chorda_aedificator_reset(b);
/* All data is gone, buffer is empty */
```

This is by design, but don't call `reset()` unless you mean it.

### Returns Are b32 (Check Them)

Most append functions return `b32` (boolean) indicating success. In practice they always succeed (the builder grows as needed), but if you're paranoid:

```c
si (!chorda_aedificator_appendere_literis(b, "test"))
{
    imprimere("Append failed (probably out of memory)\n");
}
```

In reality, if allocation fails you're in deep trouble anyway. But the API supports it.

## API Reference

Alright, here's the complete function list.

### Creation & Destruction

#### `chorda_aedificator_creare()`

```c
ChordaAedificator* chorda_aedificator_creare(
    Piscina* piscina,
    memoriae_index capacitas_initialis
);
```

Creates a new string builder.

**Parameters:**
- `piscina`: Pool for allocations
- `capacitas_initialis`: Initial capacity in bytes

**Returns:** New builder, or `NIHIL` on failure

**Example:**
```c
ChordaAedificator* b = chorda_aedificator_creare(p, CCLVI);
```

---

#### `chorda_aedificator_destruere()`

```c
vacuum chorda_aedificator_destruere(ChordaAedificator* aedificator);
```

Destroys the builder (doesn't really do anything since pool owns memory).

**Parameters:**
- `aedificator`: Builder to destroy

**Example:**
```c
chorda_aedificator_destruere(b);
```

---

### Appending - Basic Types

#### `chorda_aedificator_appendere_character()`

```c
b32 chorda_aedificator_appendere_character(
    ChordaAedificator* aedificator,
            character  c
);
```

Appends a single character.

**Returns:** `VERUM` on success

**Example:**
```c
chorda_aedificator_appendere_character(b, 'x');
```

---

#### `chorda_aedificator_appendere_literis()`

```c
b32 chorda_aedificator_appendere_literis(
     ChordaAedificator* aedificator,
    constans character* cstr
);
```

Appends a C string.

**Parameters:**
- `aedificator`: Builder
- `cstr`: Null-terminated C string

**Returns:** `VERUM` on success, `FALSUM` if `cstr` is `NIHIL`

**Example:**
```c
chorda_aedificator_appendere_literis(b, "hello");
```

---

#### `chorda_aedificator_appendere_chorda()`

```c
b32 chorda_aedificator_appendere_chorda(
    ChordaAedificator* aedificator,
               chorda  s
);
```

Appends a chorda string.

**Returns:** `VERUM` on success

**Example:**
```c
chorda s = chorda_ex_literis("test", p);
chorda_aedificator_appendere_chorda(b, s);
```

---

### Appending - Numbers

#### `chorda_aedificator_appendere_s32()`

```c
b32 chorda_aedificator_appendere_s32(
    ChordaAedificator* aedificator,
                  s32  n
);
```

Appends a signed 32-bit integer (decimal format).

**Example:**
```c
chorda_aedificator_appendere_s32(b, -XLII);  /* "-42" */
```

---

#### `chorda_aedificator_appendere_i32()`

```c
b32 chorda_aedificator_appendere_i32(
    ChordaAedificator* aedificator,
                  i32  n
);
```

Appends an unsigned 32-bit integer (decimal format).

**Example:**
```c
chorda_aedificator_appendere_i32(b, CCLVI);  /* "256" */
```

---

#### `chorda_aedificator_appendere_f64()`

```c
b32 chorda_aedificator_appendere_f64(
    ChordaAedificator* aedificator,
                  f64  n,
                  i32  decimales
);
```

Appends a 64-bit float with specified decimal places.

**Parameters:**
- `n`: Number to append
- `decimales`: Number of decimal places (0-30)

**Example:**
```c
chorda_aedificator_appendere_f64(b, 3.14159, II);  /* "3.14" */
```

---

### Appending - Special

#### `chorda_aedificator_appendere_lineam_novam()`

```c
b32 chorda_aedificator_appendere_lineam_novam(
    ChordaAedificator* aedificator
);
```

Appends a newline character (`\n`).

**Example:**
```c
chorda_aedificator_appendere_lineam_novam(b);
```

---

#### `chorda_aedificator_appendere_evasus_json()`

```c
b32 chorda_aedificator_appendere_evasus_json(
    ChordaAedificator* aedificator,
               chorda  s
);
```

Appends a chorda with JSON escaping applied.

**Escapes:** `"` `\` `\n` `\r` `\t` and control characters

**Example:**
```c
chorda unsafe = chorda_ex_literis("test\"value", p);
chorda_aedificator_appendere_evasus_json(b, unsafe);
/* Result: test\"value */
```

---

#### `chorda_aedificator_appendere_literis_evasus_json()`

```c
b32 chorda_aedificator_appendere_literis_evasus_json(
     ChordaAedificator* aedificator,
    constans character* cstr
);
```

Appends a C string with JSON escaping applied.

**Example:**
```c
chorda_aedificator_appendere_literis_evasus_json(b, "test\\value");
```

---

### Indentation

#### `chorda_aedificator_appendere_indentationem()`

```c
b32 chorda_aedificator_appendere_indentationem(
    ChordaAedificator* aedificator,
                  i32  gradus
);
```

Appends spaces for indentation.

**Parameters:**
- `gradus`: Indentation level (each level = 2 spaces)

**Example:**
```c
chorda_aedificator_appendere_indentationem(b, II);  /* 4 spaces */
```

---

#### `chorda_aedificator_push_indentationem()`

```c
vacuum chorda_aedificator_push_indentationem(
    ChordaAedificator* aedificator
);
```

Increases indentation level by 1.

**Example:**
```c
chorda_aedificator_push_indentationem(b);
```

---

#### `chorda_aedificator_pop_indentationem()`

```c
vacuum chorda_aedificator_pop_indentationem(
    ChordaAedificator* aedificator
);
```

Decreases indentation level by 1 (minimum 0).

**Example:**
```c
chorda_aedificator_pop_indentationem(b);
```

---

#### `chorda_aedificator_indentatio_gradus()`

```c
i32 chorda_aedificator_indentatio_gradus(
    ChordaAedificator* aedificator
);
```

Returns current indentation level.

**Example:**
```c
i32 level = chorda_aedificator_indentatio_gradus(b);
```

---

### Query

#### `chorda_aedificator_longitudo()`

```c
memoriae_index chorda_aedificator_longitudo(
    ChordaAedificator* aedificator
);
```

Returns current length of accumulated string.

**Example:**
```c
memoriae_index len = chorda_aedificator_longitudo(b);
```

---

#### `chorda_aedificator_spectare()`

```c
chorda chorda_aedificator_spectare(
    ChordaAedificator* aedificator
);
```

Returns a view of current content (without finishing builder).

**Warning:** View is only valid until next operation

**Example:**
```c
chorda view = chorda_aedificator_spectare(b);
```

---

### Lifecycle

#### `chorda_aedificator_reset()`

```c
vacuum chorda_aedificator_reset(
    ChordaAedificator* aedificator
);
```

Clears content and resets indentation level. Capacity remains unchanged.

**Example:**
```c
chorda_aedificator_reset(b);
```

---

#### `chorda_aedificator_finire()`

```c
chorda chorda_aedificator_finire(
    ChordaAedificator* aedificator
);
```

Finishes building and returns the final chorda.

**Returns:** Chorda containing all appended content

**Example:**
```c
chorda result = chorda_aedificator_finire(b);
```

---

## FAQ

### Q: When should I use ChordaAedificator vs regular chorda operations?

Use ChordaAedificator when you're building a string incrementally with many small appends. Use regular chorda when you know all the pieces upfront and can concatenate them directly.

### Q: What's the initial capacity I should use?

Start with 256-512 bytes for most use cases. It'll grow automatically if needed. If you know you're building large strings (> 4KB), start with 2048 or 4096.

### Q: Can I reuse a builder for multiple strings?

Yes, use `chorda_aedificator_reset()` between strings. This clears the content but keeps the allocated capacity.

### Q: What happens if I keep appending after finire()?

The chorda you got from `finire()` points to the builder's internal buffer. If you keep appending, you'll corrupt it. Don't do this.

### Q: Why does destruere() not really do anything?

Because the pool owns all the memory. When you destroy the pool, everything goes away. The destruere() function is just there for API symmetry and doesn't actually free anything.

### Q: Can I use this for binary data?

Yes, but most append functions assume text. Use `appendere_character()` for raw bytes. Or just use a regular chorda and manual memory management.

### Q: What if allocation fails during growth?

The append function returns `FALSUM`. In practice, if you're out of memory, you have bigger problems. But the API supports error handling.

### Q: How expensive is growth?

Each growth doubles capacity, so you do O(log n) allocations for n bytes. Amortized cost is O(1) per byte. It's fast enough.

### Q: Can I change the indentation width?

It's defined by `CHORDA_AEDIFICATOR_INDENTATIO_SPATIA` which defaults to 2. You can change it in the header, but why? 2 spaces is correct.

### Q: Why JSON escaping but not HTML/SQL/URL escaping?

Because JSON is common and the escaping rules are well-defined. HTML/SQL/URL escaping is more complex and context-dependent. Add your own if you need it.

### Q: Is this thread-safe?

No. Don't share a builder across threads. Give each thread its own builder if needed.

---

That's ChordaAedificator. It's a string builder. You append stuff, it grows when needed, you get a string back. Not complicated.

If you're still confused, look at the examples again. Copy the patterns. It's straightforward once you see it in action.

Now go build some strings and stop bothering me with questions.
