# UUID: Universally Unique Identifier Library Documentation

Another person who needs unique IDs. You know how many times I've seen people use `rand()` to generate "unique" identifiers? Too many. That's why we have UUIDs.

This library implements **UUIDv7** - the newest, time-ordered UUID standard from RFC 4122. Not the old random UUIDv4. Not the MAC address UUIDv1. Version 7, with millisecond timestamps and proper sorting.

**UUID** = Universally Unique Identifier. 128 bits of uniqueness, formatted as 32 hex digits with dashes: `xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx`.

## What This Library Does

The library does four things:

1. **Generate UUIDv7** - Creates time-ordered unique identifiers
2. **Parse UUIDs** - Converts UUID strings to raw bytes
3. **Format UUIDs** - Converts raw bytes to UUID strings
4. **Validate UUIDs** - Checks if a string is a valid UUID

That's it. No database integration, no fancy features. Just UUIDs.

## Why UUIDv7?

Because it's better than the alternatives:

**UUIDv1** (MAC address + timestamp):
- Leaks your MAC address (privacy issue)
- Requires system state tracking (not stateless)
- Complex implementation

**UUIDv4** (random):
- No time ordering - can't sort by creation time
- Purely random - harder to debug

**UUIDv7** (timestamp + random):
- Time-ordered - sorts by creation time
- No MAC address leakage
- Stateless - no coordination needed
- Simpler than v1, more useful than v4

UUIDv7 puts the timestamp in the most significant bits, so lexicographic sorting equals chronological sorting. Database indexes love this.

## UUIDv7 Format

128 bits structured as:

```
48 bits: Unix timestamp (milliseconds)
 4 bits: Version (0111 = 7)
12 bits: Random
 2 bits: Variant (10)
62 bits: Random
```

String format: `xxxxxxxx-xxxx-7xxx-yxxx-xxxxxxxxxxxx`

The `7` in the third group is the version. The high bits of `y` are `10` (the variant). Everything else is either timestamp or random.

## Generating UUIDs

```c
#include "uuid.h"
#include "piscina.h"

Piscina* piscina = piscina_generare_dynamicum("uuid", M);
chorda uuid = uuidv7_creare(piscina);

imprimere("UUID: %.*s\n", uuid.mensura, uuid.datum);
```

**mensura** = size/measure. The UUID string is always 36 characters (32 hex + 4 dashes).

This generates a new UUIDv7 with:
- Current timestamp (milliseconds since Unix epoch)
- Random bits for uniqueness

The timestamp gives you time ordering. The random bits ensure uniqueness even if you generate multiple UUIDs in the same millisecond.

### Uniqueness Guarantees

UUIDv7 has 74 bits of randomness (12 + 62). Even if you generate a million UUIDs per second, the collision probability is astronomically low.

Math: With 2^74 possible values, generating 10^6 UUIDs/second for 100 years gives you less than 1 in 10^12 chance of collision. You'll die before you see a collision.

## Using UUIDs

### As Database Keys

```c
chorda user_id = uuidv7_creare(piscina);
chorda post_id = uuidv7_creare(piscina);

/* Store in database */
si (!database_insert_user(user_id))
{
    /* Handle error */
}
```

UUIDv7 makes a great primary key:
- Globally unique (no coordination needed)
- Time-ordered (database indexes stay compact)
- 128 bits (no collisions)

### As Filenames

```c
chorda uuid = uuidv7_creare(piscina);
character filename[L];

sprintf(filename, "%.*s.png", uuid.mensura, uuid.datum);
/* filename is "550e8400-e29b-71d4-a716-446655440000.png" */
```

Unique filenames without checking if the file exists. Just generate and go.

### As Request IDs

```c
chorda request_id = uuidv7_creare(piscina);

imprimere("Processing request %.*s\n", request_id.mensura, request_id.datum);
/* Process request */
imprimere("Request %.*s completed\n", request_id.mensura, request_id.datum);
```

Perfect for tracking requests through a system. Time-ordered logs are easier to debug.

## Parsing and Formatting

### Parsing UUID Strings

```c
chorda uuid_string = chorda_ex_literis("550e8400-e29b-71d4-a716-446655440000", piscina);
i8 bytes[XVI];

b32 success = uuid_ex_chorda(uuid_string, bytes);
si (success)
{
    /* bytes now contains 16-byte UUID */
}
alioquin
{
    imprimere("Invalid UUID format\n");
}
```

**ex_chorda** = from string. Parses the UUID string into 16 bytes.

This validates the format (36 characters, dashes in right places, valid hex) and extracts the raw bytes.

### Formatting Bytes to String

```c
i8 bytes[XVI] = { /* some UUID bytes */ };
chorda uuid_string = uuid_ad_chordam(bytes, piscina);

si (uuid_string.datum)
{
    imprimere("UUID: %.*s\n", uuid_string.mensura, uuid_string.datum);
}
```

**ad_chordam** = to string. Formats 16 bytes as a UUID string with dashes.

Use this when you have raw UUID bytes (from a database, network packet, etc.) and need the string representation.

## Validation

### Checking UUID Format

```c
chorda uuid_string = chorda_ex_literis("550e8400-e29b-71d4-a716-446655440000", piscina);

si (uuid_est_validus(uuid_string))
{
    imprimere("Valid UUID format\n");
}
alioquin
{
    imprimere("Invalid UUID format\n");
}
```

**est_validus** = is valid. Checks:
- Length is 36 characters
- Dashes at positions 8, 13, 18, 23
- All other characters are hex (0-9, a-f, A-F)

This doesn't check the version or variant bits - just the basic format.

### Checking UUIDv7 Specifically

```c
si (uuid_est_v7(uuid_string))
{
    imprimere("This is a UUIDv7\n");
}
alioquin
{
    imprimere("This is some other UUID version\n");
}
```

**est_v7** = is version 7. Checks if the version nibble (position 14 in the string) is '7'.

Use this when you need to verify you're actually getting UUIDv7 and not some other UUID variant.

## Implementation Details

### Random Number Generation

On macOS, the library uses `arc4random_buf()` for cryptographically secure random numbers. On other Unix systems, it reads from `/dev/urandom`.

If neither is available (shouldn't happen), it falls back to `rand()`. But that's not cryptographically secure and you shouldn't rely on it for actual uniqueness.

### Timestamp Resolution

The timestamp is milliseconds since Unix epoch (January 1, 1970 00:00:00 UTC). This gives you:
- Range: ~8900 years from 1970 (plenty)
- Resolution: 1 millisecond
- Sorting: Chronological order preserved

If you generate multiple UUIDs in the same millisecond, they'll have the same timestamp but different random bits. They'll sort together but in random order relative to each other.

### Memory Allocation

UUID strings are allocated from the pool. 36 characters + null terminator = 37 bytes per UUID.

The raw bytes (16 bytes) are not allocated - you provide the buffer.

## Common Patterns

### Generating Batch IDs

```c
#define BATCH_SIZE C  /* 100 UUIDs */

chorda uuids[BATCH_SIZE];
i32 i;

per (i = ZEPHYRUM; i < BATCH_SIZE; i++)
{
    uuids[i] = uuidv7_creare(piscina);
}
```

All UUIDs in the batch will be time-ordered. Even if generated in the same millisecond, the random bits ensure uniqueness.

### Converting Between Formats

```c
/* String -> Bytes -> String */
chorda original = chorda_ex_literis("550e8400-e29b-71d4-a716-446655440000", piscina);
i8 bytes[XVI];

uuid_ex_chorda(original, bytes);
chorda converted = uuid_ad_chordam(bytes, piscina);

/* converted should equal original */
```

Round-trip conversion. Useful for storing UUIDs as bytes (more compact) but displaying as strings.

### Extracting Timestamp

```c
chorda uuid = uuidv7_creare(piscina);
i8 bytes[XVI];

uuid_ex_chorda(uuid, bytes);

/* Extract timestamp (first 6 bytes, big-endian) */
i64 timestamp_ms = ((i64)bytes[0] << 40) |
                   ((i64)bytes[1] << 32) |
                   ((i64)bytes[2] << 24) |
                   ((i64)bytes[3] << 16) |
                   ((i64)bytes[4] << 8) |
                   ((i64)bytes[5]);

imprimere("Timestamp: %lld ms\n", timestamp_ms);
```

The first 48 bits (6 bytes) are the timestamp in milliseconds. You can extract it if you need the creation time.

This is big-endian, so most significant byte first.

## A Complete Example

```c
#include "uuid.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>

integer principale(vacuum)
{
    Piscina* piscina;
    chorda uuid1;
    chorda uuid2;
    i8 bytes[XVI];
    i32 i;

    piscina = piscina_generare_dynamicum("uuid_demo", M);
    si (!piscina)
    {
        imprimere("Failed to create pool\n");
        redde I;
    }

    /* Generate some UUIDs */
    imprimere("=== Generating UUIDs ===\n");
    per (i = ZEPHYRUM; i < V; i++)
    {
        chorda uuid = uuidv7_creare(piscina);
        imprimere("UUID %d: %.*s\n", i + I, uuid.mensura, uuid.datum);
    }

    /* Parse and format */
    imprimere("\n=== Parsing and Formatting ===\n");
    uuid1 = uuidv7_creare(piscina);
    imprimere("Original: %.*s\n", uuid1.mensura, uuid1.datum);

    si (uuid_ex_chorda(uuid1, bytes))
    {
        imprimere("Parsed to bytes: ");
        per (i = ZEPHYRUM; i < XVI; i++)
        {
            imprimere("%02x ", (insignatus i8)bytes[i]);
        }
        imprimere("\n");

        uuid2 = uuid_ad_chordam(bytes, piscina);
        imprimere("Formatted back: %.*s\n", uuid2.mensura, uuid2.datum);
    }

    /* Validation */
    imprimere("\n=== Validation ===\n");
    uuid1 = uuidv7_creare(piscina);
    imprimere("Valid UUID? %s\n", uuid_est_validus(uuid1) ? "Yes" : "No");
    imprimere("Is UUIDv7? %s\n", uuid_est_v7(uuid1) ? "Yes" : "No");

    chorda invalid = chorda_ex_literis("not-a-uuid", piscina);
    imprimere("\nInvalid string? %s\n", uuid_est_validus(invalid) ? "Valid!" : "Invalid");

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
```

This demonstrates generation, parsing, formatting, and validation.

---

# API Reference

## Generation

### uuidv7_creare

```c
chorda uuidv7_creare(Piscina* piscina);
```

Generates a new UUIDv7.

**Parameters:**
- `piscina` - Memory pool for string allocation

**Returns:** UUID string (36 characters), or empty string on error

**Notes:** Uses current time in milliseconds + cryptographically secure random bits.

## Conversion

### uuid_ex_chorda

```c
b32 uuid_ex_chorda(chorda uuid_chorda, i8 bytes[XVI]);
```

Parses UUID string to bytes.

**Parameters:**
- `uuid_chorda` - UUID string (must be 36 characters)
- `bytes` - Output buffer for 16 bytes

**Returns:** `VERUM` on success, `FALSUM` if invalid format

**Notes:** Validates format and extracts raw bytes. Supports uppercase and lowercase hex.

### uuid_ad_chordam

```c
chorda uuid_ad_chordam(i8 bytes[XVI], Piscina* piscina);
```

Formats bytes to UUID string.

**Parameters:**
- `bytes` - 16-byte UUID
- `piscina` - Memory pool for string allocation

**Returns:** UUID string (36 characters), or empty string on error

**Notes:** Formats with lowercase hex and dashes at standard positions.

## Validation

### uuid_est_validus

```c
b32 uuid_est_validus(chorda uuid_chorda);
```

Checks if string is a valid UUID.

**Parameters:**
- `uuid_chorda` - String to validate

**Returns:** `VERUM` if valid format, `FALSUM` otherwise

**Notes:** Checks length, dash positions, and hex characters. Doesn't check version/variant bits.

### uuid_est_v7

```c
b32 uuid_est_v7(chorda uuid_chorda);
```

Checks if UUID is version 7.

**Parameters:**
- `uuid_chorda` - UUID string

**Returns:** `VERUM` if UUIDv7, `FALSUM` otherwise

**Notes:** Checks basic validity first, then verifies version nibble is '7'.

---

# Frequently Asked Questions

## Why UUIDv7 Instead of UUIDv4?

UUIDv4 is purely random. No time ordering. If you generate a million UUIDs and insert them into a database, the index becomes fragmented because the keys are random.

UUIDv7 has timestamp ordering. Keys generated at the same time are close together numerically. Database indexes stay compact. Range queries by creation time work naturally.

Plus, UUIDv7 still has 74 bits of randomness, which is more than enough for uniqueness.

## Are UUIDs Cryptographically Secure?

The random bits are generated with `arc4random_buf()` on macOS or `/dev/urandom` on Unix, which are cryptographically secure.

But the timestamp is not secret - it's visible in the UUID. So don't use UUIDs as security tokens or passwords. Use them as identifiers.

## Can I Generate UUIDs Offline?

Yes. UUIDv7 is stateless - no coordination with servers, no MAC addresses, no shared state. Just timestamp + random.

You can generate UUIDs on a disconnected device and they'll still be unique when you sync.

## What About Clock Skew?

If your system clock jumps backward, newly generated UUIDs might sort before older ones.

This is rare (requires admin manually setting the clock back) and usually harmless. Database uniqueness constraints will still work - the UUIDs are different even if timestamps are out of order.

For distributed systems, use NTP to keep clocks synchronized.

## How Do I Store UUIDs in a Database?

**Option 1: String (36 characters)**
```sql
CREATE TABLE users (id CHAR(36) PRIMARY KEY, ...);
```
Simple but wastes space (36 bytes instead of 16).

**Option 2: Binary (16 bytes)**
```sql
CREATE TABLE users (id BINARY(16) PRIMARY KEY, ...);
```
More compact but requires converting between string and binary.

Most databases have a native UUID type. Use that if available.

## Can I Use This Library for UUIDv4?

No. This library only generates UUIDv7. If you need UUIDv4 (purely random), you'll need a different implementation.

But seriously, why do you need UUIDv4? UUIDv7 is better in almost every way.

## What's the Collision Probability?

With 74 bits of randomness:
- Generating 1 billion UUIDs: ~0.000003% chance of collision
- Generating 1 trillion UUIDs: ~0.003% chance of collision

You're more likely to win the lottery, get struck by lightning, and have a meteor land on your house - all on the same day.

In practice: don't worry about collisions.

## Why Not Just Use Integers?

Auto-increment integers require coordination. If you have multiple servers generating IDs, they need to talk to each other or use separate ranges.

UUIDs are decentralized. Every server generates unique IDs without coordination. No race conditions, no distributed locks, no complexity.

Also, integers leak information (how many records you have, when records were created relative to each other). UUIDs don't.

## Can I Extract the Timestamp?

Yes. The first 48 bits (6 bytes) are the Unix timestamp in milliseconds, big-endian.

```c
i8 bytes[XVI];
uuid_ex_chorda(uuid, bytes);

i64 ms = ((i64)bytes[0] << 40) |
         ((i64)bytes[1] << 32) |
         ((i64)bytes[2] << 24) |
         ((i64)bytes[3] << 16) |
         ((i64)bytes[4] << 8) |
         ((i64)bytes[5]);
```

Useful for range queries or filtering by creation time.

## Are UUIDs URL-Safe?

No. The dashes aren't technically illegal in URLs but they're not ideal. And the mixed case can be confusing.

If you need URL-safe IDs, use base64url encoding:
```c
/* Remove dashes, convert to base64url */
```

Or use a different encoding scheme designed for URLs.

## How Do I Compare UUIDs?

String comparison works:
```c
b32 equal = chorda_aequales(uuid1, uuid2);
```

**aequales** = equals. This compares character-by-character.

Byte comparison also works:
```c
i8 bytes1[XVI], bytes2[XVI];
uuid_ex_chorda(uuid1, bytes1);
uuid_ex_chorda(uuid2, bytes2);
b32 equal = memcmp(bytes1, bytes2, XVI) == ZEPHYRUM;
```

For sorting, string comparison works because the timestamp is in the most significant digits.

## Why 36 Characters?

32 hex digits (16 bytes × 2 characters/byte) + 4 dashes = 36 characters.

The dashes are at positions 8, 13, 18, 23 to split the UUID into 5 groups: 8-4-4-4-12 hex digits.

This is the standard format from RFC 4122. Everyone does it this way.

---

Alright, that's UUIDs. Generate them, parse them, validate them. Use them for database keys, filenames, request tracking, whatever. They're unique, time-ordered, and you don't have to think about collisions. Done.
