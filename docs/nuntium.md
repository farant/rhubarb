# Nuntium

Binary serialization. Protobuf-style wire format. No schema files, no code generation, no nonsense.

Look, sometimes you need to pack data into bytes and unpack it later. JSON is bloated. XML is worse. You want something compact, fast, and doesn't require you to install seventeen npm packages. Nuntium does that.


## What It Does

- Writes structured data to binary format
- Reads it back
- Compatible with protobuf wire format (mostly)
- Zero-copy reading where possible
- Handles nested messages
- Forward compatible (unknown fields can be skipped)

## What It Doesn't Do

- Schema files
- Code generation
- Reflection
- RPC
- Your laundry


## Dependencies

```c
#include "nuntium.h"   /* quod necessarium */
#include "piscina.h"   /* memoria */
#include "chorda.h"    /* strings */
```

You need an arena (`Piscina`). Everything allocates from the arena. When you're done, destroy the arena, everything goes away. Simple.


## Quick Start

Here's a message with an ID and a name. Writing:

```c
Piscina*         piscina;
NuntiumScriptor* scriptor;
chorda           datum;
chorda           nomen;

piscina = piscina_generare_dynamicum("exemplum", MMMMXCVI);

scriptor = nuntium_scriptor_creare(piscina, CXXVIII);

/* Tag I = id, Tag II = nomen */
nuntium_scribere_varint(scriptor, I, XLII);

nomen = chorda_ex_literis("Franciscus", piscina);
nuntium_scribere_chorda(scriptor, II, nomen);

datum = nuntium_scriptor_datum(scriptor);
/* datum nunc continet bytes serializatos */
```

Reading:

```c
NuntiumLector* lector;
i32            tag;
i32            wire_type;
i64            id;
chorda         nomen_out;

lector = nuntium_lector_creare(piscina, datum);

dum (nuntium_legere_field(lector, &tag, &wire_type))
{
    commutatio (tag)
    {
        casus I:
            id = nuntium_legere_varint(lector);
            frange;
        casus II:
            nomen_out = nuntium_legere_chorda(lector);
            frange;
        ordinarius:
            nuntium_saltare_field(lector, wire_type);
            frange;
    }
}
```

That's it. You write fields with tags, read them back by tag. Unknown tags get skipped. Forward compatible by default.


## The Wire Format

"Wire format" means how the data is arranged in bytes. Nuntium uses the same format as Protocol Buffers, so if you know protobuf, you know this.

Every field is:

```
[tag << 3 | wire_type][payload...]
```

The tag identifies which field this is. You pick the numbers. The wire type tells the reader how to parse the payload.

### Wire Types

| Wire Type | Name               | Used For                              |
|-----------|--------------------|---------------------------------------|
| 0         | Varint             | i32, i64, b32, enums                  |
| 1         | 64-bit             | f64, fixed i64                        |
| 2         | Length-delimited   | strings, bytes, nested messages       |
| 5         | 32-bit             | f32, fixed i32                        |

Wire types 3 and 4 existed in old protobuf (groups). We don't support them. Nobody uses them. Move on.


## Varints

A varint is a variable-length integer. Small numbers use fewer bytes. The number 1 takes 1 byte. The number 1000 takes 2 bytes. The number 10000000 takes 4 bytes.

How it works: each byte uses 7 bits for data. The high bit (0x80) means "there's more bytes coming." When you hit a byte without the high bit set, you're done.

```
Valor 1:    0x01                 (1 byte)
Valor 127:  0x7F                 (1 byte)
Valor 128:  0x80 0x01            (2 bytes)
Valor 300:  0xAC 0x02            (2 bytes)
```

Why does this matter? Because if most of your numbers are small, you save a lot of space compared to always using 4 bytes.

### Signed Varints (Zigzag)

Regular varints are unsigned. If you naively encode -1 as a signed integer, it looks like a huge number (all bits set), which takes 10 bytes. Disaster.

Zigzag encoding fixes this by interleaving positive and negative values:

```
Valor  0 -> Encoded 0
Valor -1 -> Encoded 1
Valor  1 -> Encoded 2
Valor -2 -> Encoded 3
Valor  2 -> Encoded 4
```

Now small negative numbers are still small. Use `nuntium_scribere_svarint` / `nuntium_legere_svarint` for signed integers that might be negative.

```c
/* Scribere numeros signatos */
nuntium_scribere_svarint(scriptor, TAG_TEMPERATURA, -XL);

/* Legere */
s64 temperatura = nuntium_legere_svarint(lector);
```


## Writing Data

### Creating a Writer

```c
NuntiumScriptor* scriptor;

scriptor = nuntium_scriptor_creare(piscina, CXXVIII);
```

The second argument is initial capacity. It grows automatically if needed. Pick something reasonable. CXXVIII (128) is fine for small messages.

### Writing Fields

Every write function takes a tag number. You decide what tags mean.

```c
/* Varint - per integers, booleans, enums */
nuntium_scribere_varint(scriptor, I, XLII);

/* Signed varint - per signed integers that might be negative */
nuntium_scribere_svarint(scriptor, II, -XVII);

/* Fixed 32-bit - per f32 or when you need exact 4 bytes */
nuntium_scribere_32bit(scriptor, III, 0x12345678);

/* Fixed 64-bit - per f64 or when you need exact 8 bytes */
nuntium_scribere_64bit(scriptor, IV, 0x123456789ABCDEF0ULL);

/* Float */
nuntium_scribere_f32(scriptor, V, 3.14159f);

/* Double */
nuntium_scribere_f64(scriptor, VI, 2.718281828);

/* String */
chorda nomen = chorda_ex_literis("Salve", piscina);
nuntium_scribere_chorda(scriptor, VII, nomen);

/* Raw bytes */
i8 bytes[] = {0x00, 0x01, 0x02};
nuntium_scribere_bytes(scriptor, VIII, bytes, III);
```

### Getting the Output

```c
chorda datum = nuntium_scriptor_datum(scriptor);
/* datum.datum = pointer ad bytes */
/* datum.mensura = numerus bytium */
```

The returned chorda points into the scriptor's buffer. It's valid until you purge or destroy the scriptor.

### Reusing a Writer

```c
nuntium_scriptor_purgare(scriptor);
/* Nunc vacuum, parata ad novum nuntium */
```

This resets the position to zero. Doesn't deallocate anything. Good for writing many messages without reallocating.


## Reading Data

### Creating a Reader

```c
NuntiumLector* lector;
chorda         datum;  /* bytes to read */

lector = nuntium_lector_creare(piscina, datum);
```

### The Read Loop

The standard pattern is a loop that reads field headers and dispatches by tag:

```c
i32 tag;
i32 wire_type;

dum (nuntium_legere_field(lector, &tag, &wire_type))
{
    commutatio (tag)
    {
        casus TAG_ID:
            id = nuntium_legere_varint(lector);
            frange;
        casus TAG_NOMEN:
            nomen = nuntium_legere_chorda(lector);
            frange;
        casus TAG_AETAS:
            aetas = nuntium_legere_svarint(lector);
            frange;
        ordinarius:
            nuntium_saltare_field(lector, wire_type);
            frange;
    }
}
```

`nuntium_legere_field` returns FALSUM when there's no more data.

### Reading Values

After reading a field header, call the appropriate read function:

```c
/* Varint */
i64 valor = nuntium_legere_varint(lector);

/* Signed varint */
s64 valor = nuntium_legere_svarint(lector);

/* Fixed 32-bit */
i32 valor = nuntium_legere_32bit(lector);

/* Fixed 64-bit */
i64 valor = nuntium_legere_64bit(lector);

/* Float */
f32 valor = nuntium_legere_f32(lector);

/* Double */
f64 valor = nuntium_legere_f64(lector);

/* String or bytes */
chorda valor = nuntium_legere_chorda(lector);
chorda bytes = nuntium_legere_bytes(lector);
```

### Zero-Copy Strings

When you read a string or bytes, the returned chorda points directly into the original buffer. No copy. Fast. But it means the original buffer must stay alive while you're using the string.

```c
chorda nomen = nuntium_legere_chorda(lector);
/* nomen.datum punta ad datum originale */
/* non copiatum! */
```

### Skipping Unknown Fields

This is the magic for forward compatibility. If you get a field you don't recognize, skip it:

```c
ordinarius:
    nuntium_saltare_field(lector, wire_type);
    frange;
```

The wire type tells the reader how many bytes to skip. Varints get read and discarded. Fixed 32/64-bit skip 4/8 bytes. Length-delimited reads the length and skips that many bytes.

This means you can add new fields to your messages and old readers won't choke.


## Nested Messages

A nested message is just bytes inside bytes. You serialize the inner message first, then write it as a field in the outer message.

### Writing Nested

```c
NuntiumScriptor* scriptor_interior;
NuntiumScriptor* scriptor_exterior;
chorda           datum_interior;

/* Primo, scribere nuntium interiorem */
scriptor_interior = nuntium_scriptor_creare(piscina, CXXVIII);
nuntium_scribere_varint(scriptor_interior, I, XCIX);
nuntium_scribere_chorda(scriptor_interior, II, titulus);

datum_interior = nuntium_scriptor_datum(scriptor_interior);

/* Deinde, includere in nuntio exteriore */
scriptor_exterior = nuntium_scriptor_creare(piscina, CXXVIII);
nuntium_scribere_varint(scriptor_exterior, I, I);
nuntium_scribere_nuntium(scriptor_exterior, II, datum_interior);
```

### Reading Nested

```c
chorda datum_interior;

/* Legere field */
nuntium_legere_field(lector_exterior, &tag, &wire_type);
datum_interior = nuntium_legere_nuntium(lector_exterior);

/* Creare novum lector pro nuntio interiore */
NuntiumLector* lector_interior;
lector_interior = nuntium_lector_creare(piscina, datum_interior);

/* Legere campos interiores */
dum (nuntium_legere_field(lector_interior, &tag, &wire_type))
{
    /* ... */
}
```


## Error Handling

Both reader and writer track an error flag. Once an error occurs, subsequent operations fail silently.

```c
/* Verificare si error */
si (nuntium_lector_error(lector))
{
    /* malum accidit */
}

si (nuntium_scriptor_error(scriptor))
{
    /* allocatio fracta vel aliud */
}
```

Common errors:
- Reading past end of buffer
- Allocation failure (writer)
- Invalid wire type when skipping

The philosophy is: check once at the end, not after every operation. The error flag latches. Once set, stays set.


## Tags and Field Numbers

Tags are just integers you choose. Some guidelines:

1. **Keep them small** - tags are varint encoded, so 1-15 take 1 byte, 16-2047 take 2 bytes
2. **Don't reuse** - once a tag means something, it means that forever
3. **Don't change wire types** - if tag 1 was a varint, it stays a varint
4. **Reserve some for future** - leave gaps in your numbering

```c
#define TAG_ID      I
#define TAG_NOMEN   II
#define TAG_AETAS   III
/* IV-X reservati pro futuro */
#define TAG_METADATA XI
```


## Practical Example: User Message

Here's a complete example serializing a user record:

```c
#define TAG_USER_ID       I
#define TAG_USER_NOMEN    II
#define TAG_USER_EMAIL    III
#define TAG_USER_ACTIVUS  IV
#define TAG_USER_CREATUS  V

vacuum
scribere_usorem(
    NuntiumScriptor* scriptor,
    i64              id,
    chorda           nomen,
    chorda           email,
    b32              activus,
    i64              timestamp_creatus)
{
    nuntium_scribere_varint(scriptor, TAG_USER_ID, id);
    nuntium_scribere_chorda(scriptor, TAG_USER_NOMEN, nomen);
    nuntium_scribere_chorda(scriptor, TAG_USER_EMAIL, email);
    nuntium_scribere_varint(scriptor, TAG_USER_ACTIVUS, activus ? I : ZEPHYRUM);
    nuntium_scribere_varint(scriptor, TAG_USER_CREATUS, timestamp_creatus);
}

structura Usor
{
    i64    id;
    chorda nomen;
    chorda email;
    b32    activus;
    i64    creatus;
};

b32
legere_usorem(
    NuntiumLector* lector,
    structura Usor* usor)
{
    i32 tag;
    i32 wire_type;

    /* Valores default */
    usor->id = ZEPHYRUM;
    usor->nomen.datum = NIHIL;
    usor->nomen.mensura = ZEPHYRUM;
    usor->email.datum = NIHIL;
    usor->email.mensura = ZEPHYRUM;
    usor->activus = FALSUM;
    usor->creatus = ZEPHYRUM;

    dum (nuntium_legere_field(lector, &tag, &wire_type))
    {
        commutatio (tag)
        {
            casus TAG_USER_ID:
                usor->id = nuntium_legere_varint(lector);
                frange;
            casus TAG_USER_NOMEN:
                usor->nomen = nuntium_legere_chorda(lector);
                frange;
            casus TAG_USER_EMAIL:
                usor->email = nuntium_legere_chorda(lector);
                frange;
            casus TAG_USER_ACTIVUS:
                usor->activus = (b32)nuntium_legere_varint(lector);
                frange;
            casus TAG_USER_CREATUS:
                usor->creatus = nuntium_legere_varint(lector);
                frange;
            ordinarius:
                nuntium_saltare_field(lector, wire_type);
                frange;
        }
    }

    redde !nuntium_lector_error(lector);
}
```


## API Reference

### Types

#### NuntiumWireGenus

Wire type enumeration.

```c
enumeratio {
    NUNTIUM_WIRE_VARINT           = 0,
    NUNTIUM_WIRE_64BIT            = 1,
    NUNTIUM_WIRE_LENGTH_DELIMITED = 2,
    NUNTIUM_WIRE_32BIT            = 5
} NuntiumWireGenus;
```

#### NuntiumScriptor

Opaque writer type. Created with `nuntium_scriptor_creare`.

#### NuntiumLector

Opaque reader type. Created with `nuntium_lector_creare`.


### Writer Functions

#### nuntium_scriptor_creare

```c
NuntiumScriptor*
nuntium_scriptor_creare(
    Piscina* piscina,
        i32  capacitas_initialis);
```

Creates a new writer.

**Parameters:**
- `piscina` - Arena for allocations
- `capacitas_initialis` - Initial buffer size (grows automatically). Use ZEPHYRUM for default (256)

**Returns:** Writer, or NIHIL if allocation fails.

---

#### nuntium_scriptor_purgare

```c
vacuum
nuntium_scriptor_purgare(
    NuntiumScriptor* scriptor);
```

Resets writer for reuse. Clears position and error flag. Does not deallocate.

---

#### nuntium_scriptor_datum

```c
chorda
nuntium_scriptor_datum(
    NuntiumScriptor* scriptor);
```

Gets the serialized bytes.

**Returns:** chorda with pointer and length. Points into scriptor's buffer.

---

#### nuntium_scriptor_error

```c
b32
nuntium_scriptor_error(
    NuntiumScriptor* scriptor);
```

Checks if an error occurred.

**Returns:** VERUM if error, FALSUM otherwise.

---

#### nuntium_scribere_varint

```c
b32
nuntium_scribere_varint(
    NuntiumScriptor* scriptor,
                 i32 tag,
                 i64 valor);
```

Writes an unsigned varint field.

**Parameters:**
- `scriptor` - Writer
- `tag` - Field number
- `valor` - Value (unsigned, 0 to 2^64-1)

**Returns:** VERUM on success.

---

#### nuntium_scribere_svarint

```c
b32
nuntium_scribere_svarint(
    NuntiumScriptor* scriptor,
                 i32 tag,
                 s64 valor);
```

Writes a signed varint field (zigzag encoded).

**Parameters:**
- `scriptor` - Writer
- `tag` - Field number
- `valor` - Signed value

**Returns:** VERUM on success.

---

#### nuntium_scribere_32bit

```c
b32
nuntium_scribere_32bit(
    NuntiumScriptor* scriptor,
                 i32 tag,
                 i32 valor);
```

Writes a fixed 32-bit field. Little-endian.

---

#### nuntium_scribere_64bit

```c
b32
nuntium_scribere_64bit(
    NuntiumScriptor* scriptor,
                 i32 tag,
                 i64 valor);
```

Writes a fixed 64-bit field. Little-endian.

---

#### nuntium_scribere_f32

```c
b32
nuntium_scribere_f32(
    NuntiumScriptor* scriptor,
                 i32 tag,
                 f32 valor);
```

Writes a 32-bit float.

---

#### nuntium_scribere_f64

```c
b32
nuntium_scribere_f64(
    NuntiumScriptor* scriptor,
                 i32 tag,
                 f64 valor);
```

Writes a 64-bit double.

---

#### nuntium_scribere_chorda

```c
b32
nuntium_scribere_chorda(
    NuntiumScriptor* scriptor,
                 i32 tag,
              chorda valor);
```

Writes a string (length-delimited).

---

#### nuntium_scribere_bytes

```c
b32
nuntium_scribere_bytes(
    NuntiumScriptor* scriptor,
                 i32 tag,
    constans    i8* datum,
                 i32 mensura);
```

Writes raw bytes (length-delimited).

---

#### nuntium_scribere_nuntium

```c
b32
nuntium_scribere_nuntium(
    NuntiumScriptor* scriptor,
                 i32 tag,
              chorda nuntium_datum);
```

Writes a nested message (length-delimited). The `nuntium_datum` should be output from another scriptor.


### Reader Functions

#### nuntium_lector_creare

```c
NuntiumLector*
nuntium_lector_creare(
    Piscina* piscina,
      chorda datum);
```

Creates a new reader.

**Parameters:**
- `piscina` - Arena for allocations
- `datum` - Bytes to read

**Returns:** Reader, or NIHIL if allocation fails.

---

#### nuntium_lector_habet_plus

```c
b32
nuntium_lector_habet_plus(
    NuntiumLector* lector);
```

Checks if more data remains.

**Returns:** VERUM if more bytes available.

---

#### nuntium_lector_error

```c
b32
nuntium_lector_error(
    NuntiumLector* lector);
```

Checks if an error occurred.

---

#### nuntium_legere_field

```c
b32
nuntium_legere_field(
    NuntiumLector* lector,
              i32* tag_out,
              i32* wire_type_out);
```

Reads the next field header.

**Parameters:**
- `lector` - Reader
- `tag_out` - Output: field number
- `wire_type_out` - Output: wire type

**Returns:** VERUM if field read, FALSUM if no more data or error.

---

#### nuntium_saltare_field

```c
b32
nuntium_saltare_field(
    NuntiumLector* lector,
               i32 wire_type);
```

Skips the current field's payload.

**Parameters:**
- `lector` - Reader
- `wire_type` - Wire type from legere_field

**Returns:** VERUM on success.

---

#### nuntium_legere_varint

```c
i64
nuntium_legere_varint(
    NuntiumLector* lector);
```

Reads a varint value. Call after legere_field when wire_type is NUNTIUM_WIRE_VARINT.

---

#### nuntium_legere_svarint

```c
s64
nuntium_legere_svarint(
    NuntiumLector* lector);
```

Reads a signed varint (zigzag decoded).

---

#### nuntium_legere_32bit

```c
i32
nuntium_legere_32bit(
    NuntiumLector* lector);
```

Reads a fixed 32-bit value.

---

#### nuntium_legere_64bit

```c
i64
nuntium_legere_64bit(
    NuntiumLector* lector);
```

Reads a fixed 64-bit value.

---

#### nuntium_legere_f32

```c
f32
nuntium_legere_f32(
    NuntiumLector* lector);
```

Reads a 32-bit float.

---

#### nuntium_legere_f64

```c
f64
nuntium_legere_f64(
    NuntiumLector* lector);
```

Reads a 64-bit double.

---

#### nuntium_legere_chorda

```c
chorda
nuntium_legere_chorda(
    NuntiumLector* lector);
```

Reads a string. Zero-copy - returns pointer into original buffer.

---

#### nuntium_legere_bytes

```c
chorda
nuntium_legere_bytes(
    NuntiumLector* lector);
```

Reads raw bytes. Zero-copy.

---

#### nuntium_legere_nuntium

```c
chorda
nuntium_legere_nuntium(
    NuntiumLector* lector);
```

Reads a nested message. Returns bytes to pass to a new lector_creare.


### Utility Functions

#### nuntium_varint_mensura

```c
i32
nuntium_varint_mensura(
    i64 valor);
```

Computes how many bytes a varint would take.

---

#### nuntium_zigzag_encode

```c
i64
nuntium_zigzag_encode(
    s64 valor);
```

Zigzag encodes a signed value.

---

#### nuntium_zigzag_decode

```c
s64
nuntium_zigzag_decode(
    i64 valor);
```

Zigzag decodes to signed value.


## FAQ

### Why not just use JSON?

JSON is text. Text is bloated. The integer 1000000 takes 7 bytes in JSON. In a varint, it takes 3. Strings have quotes. Floats have decimal points. Everything has colons and commas.

Plus JSON needs parsing. Nuntium is just pointer arithmetic and bit twiddling.

### Why not actual protobuf?

Actual protobuf wants schema files (.proto), a compiler (protoc), generated code, a runtime library, and probably some build system integration. That's fine for Google-scale stuff. For a Latin C89 hobby project, it's overkill.

Nuntium gives you the wire format without the ceremony.

### Is this actually compatible with protobuf?

Wire format, yes. If you serialize a message with nuntium and someone else reads it with protobuf (or vice versa), it works. The bytes are the same.

But there's no schema, so you're on your own for knowing what fields mean.

### What about repeated fields?

Same tag, multiple times. Protobuf allows this. Nuntium doesn't stop you:

```c
nuntium_scribere_varint(scriptor, TAG_ITEMS, I);
nuntium_scribere_varint(scriptor, TAG_ITEMS, II);
nuntium_scribere_varint(scriptor, TAG_ITEMS, III);
```

When reading, you'll see TAG_ITEMS three times. Collect them in an array.

### What about maps?

A map is just repeated nested messages with key and value fields. Serialize each entry as a sub-message.

### What's "nuntium" mean?

Latin for "message." From *nuntius*, messenger.

- *scriptor* = writer
- *lector* = reader
- *scribere* = to write
- *legere* = to read
- *datum* = data (given thing)
- *saltare* = to jump/skip
- *purgare* = to clean/purge

### Why little-endian for fixed integers?

That's what protobuf does. x86 is little-endian. ARM is... complicated but usually little-endian these days. Network byte order (big-endian) is the traditional choice, but Google went with little-endian. We follow suit.

### Can I use this for network protocols?

Yes. That's kind of the point. Serialize on one end, send the bytes, deserialize on the other end.

### What if the buffer is corrupted?

You'll get garbage or an error. The error flag will be set if we try to read past the end of the buffer. But there's no checksum or validation. If someone flips a bit in transit, you might get wrong values without knowing.

Add your own checksums or use a reliable transport if you need integrity.

### How big can messages be?

The length fields are varints, so theoretically huge. Practically, you're limited by memory. The reader uses i32 for positions, so about 2GB max per message. Should be enough.

### Thread safety?

None. A scriptor or lector is a single-threaded object. Don't share between threads. Create one per thread if you need parallelism.


---

*Four docs down. What else?*
