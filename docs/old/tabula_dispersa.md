# Tabula Dispersa: A Hash Table That Actually Works

Look, you need a hash table. I know you need a hash table. Everyone needs a hash table eventually. You've got keys, you've got values, you want to look them up fast. O(1) lookup, right? That's what they taught you in CS 101.

Here's what you're thinking: "I'll just use an array, or maybe a linked list, or—"

Stop. Just stop. An array means you need contiguous keys or you're doing linear search. A linked list is O(n) lookup which is pathetic. A tree is O(log n) which is *better* but why are you settling for log when you can have constant time?

You need a hash table. Specifically, you need **tabula dispersa**.

**Tabula Dispersa** - Latin for "scattered table" or "distributed table," which is exactly what a hash table is. You take your keys, hash them to scattered positions in a table, and boom - fast lookup.

This library gives you:
- O(1) average-case insert, lookup, and delete
- Open addressing with linear probing (I'll explain, hold your horses)
- Automatic resizing when the table gets crowded
- Two built-in hash functions (FNV-1a and multiplication-based)
- Support for custom hash and comparison functions
- Iterator to walk through all entries
- Statistics tracking (load factor, collisions, probe distances)
- Piscina-based memory management (no manual frees)

## How Hash Tables Actually Work (The Stuff You Forgot)

Alright, quick refresher because I know you're rusty.

A hash table is an array of slots. You hash your key to get an integer, use that integer (modulo table size) to pick a slot, and store your key-value pair there. When you want to look it up later, hash the key again, jump to that slot, boom - there's your value.

That's the theory. Here's the reality:

**Collisions happen**. Two different keys might hash to the same slot. You need a strategy for handling this.

There are two main strategies:
1. **Chaining** - Each slot is a linked list. Multiple entries in the same slot? Add them to the list.
2. **Open addressing** - Each slot holds exactly one entry. Collision? Find the next available slot.

This library uses **open addressing with linear probing**. Here's how it works:

Say you want to insert key "alice". You hash it and get slot 5. You check slot 5:
- If it's empty, great! Store the entry there.
- If it's occupied by a different key, move to slot 6.
- Still occupied? Move to slot 7.
- Keep going until you find an empty slot.

When you look up "alice" later, same process: hash to slot 5, check if it's the right key. Not the right key? Check slot 6, then 7, etc. Empty slot? Key doesn't exist.

This is called **linear probing** because you probe linearly (slot, slot+1, slot+2, ...) until you find what you want.

**Why not chaining?** Because chaining requires allocating individual list nodes, which means more allocations, worse cache locality, and more complexity. Open addressing keeps everything in one contiguous array. Cache-friendly, simple, fast.

**The catch**: As the table fills up, collisions get worse. More collisions = longer probe sequences = slower lookups. Solution: when the table gets too full (load factor > 0.75), **resize it**. Allocate a bigger table (double the size), rehash all entries into the new table. Now you have room again and lookups are fast.

### Tombstones and Deletion

Deletion is tricky with open addressing. Say you have this:

```
Slot 5: alice
Slot 6: bob
Slot 7: charlie
```

All three hashed to slot 5 originally, but 6 and 7 had collisions and probed forward.

Now you delete "bob" from slot 6. If you just mark slot 6 as empty, when you look up "charlie" later:
- Hash to slot 5: found "alice", not a match, keep probing
- Check slot 6: empty! Oops, we stop here and think "charlie" doesn't exist.

But "charlie" is in slot 7!

Solution: **tombstones**. When you delete an entry, mark the slot as DELETED instead of EMPTY. During lookup, treat DELETED slots as "keep probing" but during insertion, you can reuse them.

This library tracks three slot states:
- **SLOT_VACUUM** - Empty, never used
- **SLOT_OCCUPATUM** - Occupied with a key-value pair
- **SLOT_DELETUM** - Deleted, was occupied but now available for reuse

## Latin Crash Course (You Know The Drill)

More Latin vocabulary because apparently we're medieval scholars now:

- **tabula** = table
- **dispersa** = scattered, spread out (past participle of *dispergere*)
- **friare** = to hash (literally "to crumble" - your key gets crumbled into a number)
- **friatio** = hashing (noun)
- **clavis** = key (literally "key" or "nail")
- **valor** = value
- **inserere** = to insert
- **invenire** = to find
- **delere** = to delete
- **continet** = it contains (from *continere*)
- **slotus** = slot (Latinized from English because Latin doesn't have a word for hash table slots)
- **capacitas** = capacity
- **numerus** = number
- **onus** = burden, load
- **factor oneris** = load factor
- **crescere** = to grow
- **vacare** = to empty
- **iterator** = iterator (same word in Latin)
- **proximum** = next (accusative neuter)
- **initium** = beginning
- **comparatio** = comparison
- **collisiones** = collisions (plural)
- **distantia** = distance
- **maximus** = maximum

Now when you see `tabula_dispersa_inserere()` you can translate it as "hash_table_insert" and feel scholarly.

## Creating A Hash Table

Two ways to create a hash table: the easy way (for string keys) and the custom way (for anything else).

### The Easy Way (String Keys)

Most of the time you're mapping strings to values. Config settings, symbol tables, whatever. For this, use the convenience function:

```c
Piscina* piscina = piscina_generare_dynamicum("hash_table", 8192);

TabulaDispersa* tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
si (!tabula)
{
    imprimere("Failed to create hash table\n");
    exire(I);
}
```

**`tabula_dispersa_creare_chorda(piscina, capacitas_initialis)`** - "hash_table_create_string"

Creates a hash table optimized for `chorda` keys (your string type). Uses FNV-1a hashing and `chorda_comparare()` for comparison.

**Initial capacity**: 128 in the example above (that's `CXXVIII` - get used to reading Roman numerals). Pick a power of 2 that's bigger than you expect to need. The table will grow automatically if it fills up, but starting with a good size avoids early resizes.

Rules of thumb:
- Small lookup table (< 50 entries): start with 64 or 128
- Medium table (< 500 entries): start with 256 or 512
- Large table (thousands of entries): start with 1024 or 2048

### The Custom Way (Custom Keys and Hashing)

Maybe you're not using strings. Maybe you're using integers, or structs, or something weird. You need to provide your own hash function and comparison function:

```c
/* Your hash function */
i32 mea_friatio(chorda clavis)
{
    /* Hash the key somehow and return an i32 */
    redde some_hash_value;
}

/* Your comparison function */
i32 mea_comparatio(chorda a, chorda b)
{
    /* Return 0 if equal, non-zero if different */
    redde (a differs from b) ? I : ZEPHYRUM;
}

TabulaDispersa* tabula = tabula_dispersa_creare(
    piscina,
    CCLVI,  /* capacity: 256 */
    mea_friatio,
    mea_comparatio
);
```

**`tabula_dispersa_creare(piscina, capacitas, friatio, comparatio)`**

- `piscina` - your arena allocator
- `capacitas` - initial capacity
- `friatio` - function pointer to your hash function (type `TabulaFriatio`)
- `comparatio` - function pointer to your comparison function (type `TabulaComparatio`)

Both function pointers take a `chorda` and return an `i32`. Your hash function returns a hash code. Your comparison function returns 0 for equal, non-zero for not equal (like `strcmp`).

More on hash functions later. For now, just know: if you're using strings, use `tabula_dispersa_creare_chorda()` and let the library handle it.

## Inserting and Updating

You've got a table. Now you want to put stuff in it.

```c
chorda clavis = chorda_ex_literis("username", piscina);
i32    valor  = XLII;

si (!tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)valor))
{
    imprimere("Insert failed\n");
}
```

**`tabula_dispersa_inserere(tabula, clavis, valor)`** - "hash_table_insert"

Inserts a key-value pair. Returns `VERUM` on success, `FALSUM` on failure.

**Important**: The `valor` is a `vacuum*` (void pointer). You can store anything you want:
- Pointers to structs
- Cast integers (like the example above)
- Pointers to other data in your piscina

Whatever you store, you get back when you look it up.

### What Happens On Insert

When you insert:
1. Hash the key
2. Find the slot (linear probe if there's a collision)
3. If the key already exists, **update** the value (doesn't increase count)
4. If it's a new key, store it (increases count)
5. Check load factor - if > 0.75, resize the table

The resize is automatic. You don't think about it. The table doubles in capacity, all entries are rehashed into the new table. This is why you don't keep pointers to internal table structures - they become invalid after a resize.

### Updating Existing Keys

If you insert a key that already exists, it updates the value:

```c
chorda clavis = chorda_ex_literis("count", piscina);

tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)I);
/* Table now has: "count" -> 1 */

tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)II);
/* Table now has: "count" -> 2 (updated, not duplicated) */
```

The table only stores one entry per unique key. Updates don't increase the entry count.

## Looking Up Values

You've inserted stuff. Now you want to get it back.

```c
chorda  clavis = chorda_ex_literis("username", piscina);
vacuum* valor;

si (tabula_dispersa_invenire(tabula, clavis, &valor))
{
    i32 num = (i32)(longus)valor;
    imprimere("Found: %d\n", num);
}
alioquin
{
    imprimere("Key not found\n");
}
```

**`tabula_dispersa_invenire(tabula, clavis, &valor_out)`** - "hash_table_find"

Looks up a key. Returns `VERUM` if found, `FALSUM` if not found.

If found, the value is written to `valor_out` (an output parameter). If you don't care about the value, pass `NIHIL`:

```c
si (tabula_dispersa_invenire(tabula, clavis, NIHIL))
{
    imprimere("Key exists\n");
}
```

### Just Checking If A Key Exists

If you only want to know "does this key exist?" without retrieving the value:

```c
si (tabula_dispersa_continet(tabula, clavis))
{
    imprimere("Key exists\n");
}
```

**`tabula_dispersa_continet(tabula, clavis)`** - "hash_table_contains"

Returns `VERUM` if the key exists, `FALSUM` otherwise. This is just a convenience wrapper around `invenire()` with `NIHIL` for the value output.

## Deleting Entries

Remove a key-value pair from the table:

```c
chorda clavis = chorda_ex_literis("obsolete", piscina);

si (tabula_dispersa_delere(tabula, clavis))
{
    imprimere("Deleted\n");
}
alioquin
{
    imprimere("Key didn't exist\n");
}
```

**`tabula_dispersa_delere(tabula, clavis)`** - "hash_table_delete"

Returns `VERUM` if the key was found and deleted, `FALSUM` if the key didn't exist.

**What actually happens**: The slot is marked as `SLOT_DELETUM` (tombstone). The entry count decreases. The slot can be reused for future insertions, but lookups will probe past it.

If you delete a lot of entries, the table fills up with tombstones. This slows down lookups (more probing). The library tracks a "deleted factor" (deleted entries / capacity). When it exceeds 0.25, the next insertion triggers a resize that cleans up all the tombstones.

## Clearing The Table

You want to remove all entries and reuse the table:

```c
tabula_dispersa_vacare(tabula);
```

**`tabula_dispersa_vacare(tabula)`** - "hash_table_empty"

Marks all slots as `SLOT_VACUUM`, resets counts to zero. The capacity stays the same. This is faster than destroying and recreating the table.

**Warning**: After clearing, all keys and values you stored are still in the piscina, but the table no longer references them. If you had pointers to values in the table, those pointers are now dangling (the data exists but the table doesn't know about it).

## Iterating Over Entries

Sometimes you need to walk through all entries in the table. Maybe you're dumping state, or searching for something, or whatever.

Use a **TabulaIterator**:

```c
TabulaIterator iter = tabula_dispersa_iterator_initium(tabula);
chorda  clavis;
vacuum* valor;

dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
{
    imprimere("Key: %.*s, Value: %d\n",
        clavis.mensura, clavis.datum,
        (i32)(longus)valor);
}
```

**`tabula_dispersa_iterator_initium(tabula)`** - "hash_table_iterator_beginning"

Returns a TabulaIterator initialized to the start of the table. This is a value type (not a pointer) so you don't allocate it from the piscina.

**`tabula_dispersa_iterator_proximum(&iter, &clavis_out, &valor_out)`** - "hash_table_iterator_next"

Advances the iterator to the next occupied slot. Returns `VERUM` if an entry was found, `FALSUM` if you've reached the end.

The key and value are written to the output parameters. If you don't care about one of them, pass `NIHIL`:

```c
/* Just iterate, don't care about keys or values */
dum (tabula_dispersa_iterator_proximum(&iter, NIHIL, NIHIL))
{
    entry_count++;
}
```

### Iteration Order

**There is no order**. Hash tables don't maintain insertion order. The iterator walks through the internal slot array from index 0 to capacity-1, skipping empty and deleted slots. The order depends on hash values and collision patterns.

If you need sorted output, collect the entries into an array and sort them yourself.

### Modifying During Iteration

**Don't**. If you insert or delete while iterating, the iterator state becomes invalid. The table might resize, slots might move, and your iterator is now pointing at garbage.

If you need to delete during iteration, collect keys into an array first:

```c
/* Collect keys to delete */
chorda* keys_to_delete = (chorda*)piscina_allocare(piscina, count * magnitudo(chorda));
i32 delete_count = ZEPHYRUM;

TabulaIterator iter = tabula_dispersa_iterator_initium(tabula);
chorda clavis;
vacuum* valor;

dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
{
    si (should_delete(valor))
    {
        keys_to_delete[delete_count++] = clavis;
    }
}

/* Now delete them */
per (i32 i = ZEPHYRUM; i < delete_count; i++)
{
    tabula_dispersa_delere(tabula, keys_to_delete[i]);
}
```

## Statistics and Monitoring

The table tracks some statistics that are useful for performance tuning and debugging.

### How Full Is The Table?

```c
f32 load = tabula_dispersa_factor_oneris(tabula);
imprimere("Load factor: %.2f%%\n", load * C);
```

**`tabula_dispersa_factor_oneris(tabula)`** - "hash_table_load_factor"

Returns the load factor as a float between 0.0 and 1.0. This is (number of entries) / (capacity).

Load factor tells you how full the table is:
- 0.0 = empty
- 0.5 = half full
- 0.75 = three-quarters full (resize threshold)
- 1.0 = completely full (shouldn't happen, table resizes before this)

Higher load factor = more collisions = slower lookups. The table automatically resizes at 0.75 to keep performance good.

### How Many Deleted Entries?

```c
f32 deleted = tabula_dispersa_factor_deletorum(tabula);
imprimere("Deleted factor: %.2f%%\n", deleted * C);
```

**`tabula_dispersa_factor_deletorum(tabula)`** - "hash_table_deleted_factor"

Returns (number of deleted slots) / (capacity).

If you delete a lot of entries, tombstones accumulate. High deleted factor = slower lookups (more probing). The table automatically resizes at 0.25 deleted factor to clean up tombstones.

### How Many Entries?

```c
i32 count = tabula_dispersa_numerus(tabula);
imprimere("Table has %d entries\n", count);
```

**`tabula_dispersa_numerus(tabula)`** - "hash_table_number"

Returns the number of entries currently in the table. This is the count of occupied slots (doesn't include deleted slots).

### Detailed Status

For debugging, you can print a detailed status report:

```c
tabula_dispersa_status_imprimere(tabula);
```

**`tabula_dispersa_status_imprimere(tabula)`** - "hash_table_status_print"

Prints to stdout:
```
Status Tabulae Dispersae:
  Capacitas: 128
  Numerus: 45
  Numerus deletorum: 3
  Factor oneris: 35.16%
  Factor deletorum: 2.34%
  Collisiones totales: 12
  Distantia maxima: 4
```

**Collisions** - Total number of probing steps during lookups. Higher = more crowded table.

**Max distance** - Longest probe sequence during any lookup. High values (> 10) mean you should resize.

This is useful for understanding table performance and tuning initial capacity.

## Hash Functions

The library provides two hash functions. Both work on `chorda` keys.

### FNV-1a (Default for Strings)

```c
i32 hash = tabula_friare_fnv1a(clavis);
```

**`tabula_friare_fnv1a(clavis)`** - "table_hash_fnv1a"

FNV-1a is a simple, fast, well-tested hash function. Good distribution, low collision rate for strings. This is what `tabula_dispersa_creare_chorda()` uses by default.

How it works: Start with a magic number (2166136261), XOR each byte with the hash, multiply by another magic number (16777619), repeat. The magic numbers come from FNV algorithm research and are proven to give good distribution.

### Multiplication-Based

```c
i32 hash = tabula_friare_multiplicatio(clavis);
```

**`tabula_friare_multiplicatio(clavis)`** - "table_hash_multiplication"

An alternative hash function based on the djb2 algorithm. Start with 5381, shift-add-multiply each byte. Also fast, also good distribution.

Use this if you want to experiment with different hash functions, or if you find FNV-1a gives you lots of collisions for your specific data (unlikely).

### Which One Should You Use?

FNV-1a. Just use FNV-1a. It's the default for a reason.

If you're getting terrible collision rates and you've profiled your code and confirmed FNV-1a is the problem (it almost never is), then try the multiplication one. But 99.9% of the time, FNV-1a is fine.

### Writing Your Own Hash Function

Maybe you're not hashing strings. Maybe you're hashing structs, or integers, or something else. You need to write your own hash function.

Signature:
```c
i32 mea_friatio(chorda clavis)
{
    /* Hash clavis.datum (length clavis.mensura) to an i32 */
    redde hash_value;
}
```

Guidelines:
- Use all the bytes in the key (don't just hash the first few)
- Distribute values evenly across the i32 range
- Be deterministic (same input always gives same output)
- Be fast (this gets called on every insert/lookup)

Example for integer keys (assuming key is stored as bytes in the chorda):

```c
i32 hash_integer(chorda clavis)
{
    i32 val;
    si (clavis.mensura != magnitudo(i32)) redde ZEPHYRUM;

    memcpy(&val, clavis.datum, magnitudo(i32));

    /* Mix the bits */
    val = ((val >> XVI) ^ val) * 0x45d9f3b;
    val = ((val >> XVI) ^ val) * 0x45d9f3b;
    val = (val >> XVI) ^ val;

    redde val;
}
```

The bit-mixing at the end is important. It spreads the bits around so similar input values don't hash to similar outputs.

For more complex types, look up "MurmurHash" or "CityHash" for inspiration. But seriously, if you can represent your key as bytes, FNV-1a probably works fine.

## Memory Management With Piscina

Like everything in this codebase, the hash table is allocated from your piscina.

**What gets allocated:**
- The TabulaDispersa struct itself
- The internal array of slots
- On resize, a new larger array of slots

**What doesn't get allocated by the table:**
- Keys (you provide them)
- Values (you provide them)

You're responsible for allocating your keys and values from the piscina before inserting them. The table just stores pointers.

Example:

```c
Piscina* piscina = piscina_generare_dynamicum("app", 16384);

TabulaDispersa* tabula = tabula_dispersa_creare_chorda(piscina, CCLVI);

/* Allocate key from piscina */
chorda clavis = chorda_ex_literis("config_value", piscina);

/* Allocate value from piscina */
structura Config
{
    i32 setting1;
    i32 setting2;
};

structura Config* config = (structura Config*)piscina_allocare(
    piscina, magnitudo(structura Config));
config->setting1 = XLII;
config->setting2 = XCIX;

/* Insert */
tabula_dispersa_inserere(tabula, clavis, (vacuum*)config);

/* Later, lookup */
vacuum* val;
si (tabula_dispersa_invenire(tabula, clavis, &val))
{
    structura Config* cfg = (structura Config*)val;
    imprimere("setting1: %d\n", cfg->setting1);
}

/* When done, destroy piscina - everything gets freed */
piscina_destruere(piscina);
```

Everything comes from one piscina, everything dies with one `piscina_destruere()`. Simple.

### Typical Patterns

**Pattern 1: Per-Request Symbol Table**

You're parsing a file or handling a request. You need a temporary symbol table:

```c
vacuum handle_request(Request* req)
{
    Piscina* piscina = piscina_generare_dynamicum("request", 16384);

    TabulaDispersa* symbols = tabula_dispersa_creare_chorda(piscina, CXXVIII);

    /* Parse and populate symbols */
    parse_and_populate(req, symbols, piscina);

    /* Use the symbols */
    process_with_symbols(symbols);

    /* Done - destroy everything at once */
    piscina_destruere(piscina);
}
```

**Pattern 2: Long-Lived Global Table**

You have application-wide state that lives for the whole program:

```c
Piscina*         global_piscina;
TabulaDispersa*  global_config;

vacuum initialize_app(vacuum)
{
    global_piscina = piscina_generare_dynamicum("global", 32768);
    global_config  = tabula_dispersa_creare_chorda(global_piscina, DXII);

    /* Load config */
    load_configuration(global_config, global_piscina);
}

vacuum shutdown_app(vacuum)
{
    piscina_destruere(global_piscina);
}
```

**Pattern 3: Reusable Table With Clearing**

You're processing multiple files in a loop. Same table, different data each iteration:

```c
Piscina*         piscina = piscina_generare_dynamicum("processor", 16384);
TabulaDispersa*  tabula  = tabula_dispersa_creare_chorda(piscina, CCLVI);

per (i32 i = ZEPHYRUM; i < file_count; i++)
{
    /* Process this file */
    process_file(files[i], tabula, piscina);

    /* Clear table for next file (keys/values still in piscina) */
    tabula_dispersa_vacare(tabula);

    /* Could also clear the piscina if you want to free memory */
    /* piscina_vacare(piscina); */
}

piscina_destruere(piscina);
```

## Common Mistakes (Yes, You're Gonna Make Them)

### Mistake 1: Using Keys After Piscina Destruction

```c
/* BAD */
Piscina* p = piscina_generare_dynamicum("temp", 4096);
TabulaDispersa* tabula = tabula_dispersa_creare_chorda(p, CXXVIII);

chorda clavis = chorda_ex_literis("key", p);
tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)XLII);

piscina_destruere(p);

/* Now the table and the key are both gone! */
si (tabula_dispersa_invenire(tabula, clavis, NIHIL))  /* CRASH */
{
    /* ... */
}
```

After you destroy the piscina, the table struct, the internal slots, and all keys/values are freed. Don't use any of them.

### Mistake 2: Storing Stack Pointers As Values

```c
/* BAD */
vacuum do_something(vacuum)
{
    i32 local_value = XLII;
    chorda clavis = chorda_ex_literis("key", piscina);

    /* Storing a pointer to a stack variable! */
    tabula_dispersa_inserere(tabula, clavis, (vacuum*)&local_value);

    /* When this function returns, local_value is gone */
}

/* Later... */
vacuum* val;
tabula_dispersa_invenire(tabula, clavis, &val);
i32 num = *(i32*)val;  /* CRASH - dangling pointer */
```

Only store pointers to heap-allocated data (from piscina) or static data. Stack pointers become invalid when the function returns.

### Mistake 3: Casting Pointers To Integers Wrong

```c
/* BAD */
i32 value = XLII;
tabula_dispersa_inserere(tabula, clavis, (vacuum*)value);  /* Wrong cast */
```

This looks like it works but it's undefined behavior. The correct way to store an integer as a pointer:

```c
/* GOOD */
i32 value = XLII;
tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)value);
```

Cast through `longus` (long) first, then to `vacuum*`. On the way back:

```c
vacuum* val;
tabula_dispersa_invenire(tabula, clavis, &val);
i32 num = (i32)(longus)val;
```

This works reliably on both 32-bit and 64-bit platforms.

### Mistake 4: Modifying Keys After Insertion

```c
/* BAD */
chorda clavis = chorda_ex_literis("key", piscina);
tabula_dispersa_inserere(tabula, clavis, some_value);

/* Later, modify the key data */
clavis.datum[0] = 'X';  /* Now the hash is wrong! */

/* This will fail or find the wrong entry */
tabula_dispersa_invenire(tabula, clavis, &val);
```

The table stores a pointer to your key. If you modify the key data, the hash changes but the table doesn't know. It's stored in the wrong slot now.

Solution: **Don't modify keys after insertion**. Keys should be immutable. If you need to change a key, delete the old entry and insert a new one.

### Mistake 5: Forgetting The Capacity Is Rounded Up

```c
TabulaDispersa* tabula = tabula_dispersa_creare_chorda(piscina, C);
/* You asked for 100, but you get 128 (next power of 2) */
```

The library rounds your initial capacity up to the next power of 2, with a minimum of 16. This is for performance (masking instead of modulo). Don't assume `tabula->capacitas` equals what you passed in.

### Mistake 6: Comparing chorda Pointers Instead of Contents

```c
/* BAD */
chorda clavis1 = chorda_ex_literis("key", piscina);
chorda clavis2 = chorda_ex_literis("key", piscina);

si (clavis1.datum == clavis2.datum)  /* These are different pointers! */
{
    /* ... */
}
```

Even though both chordas have the same content ("key"), they're different allocations with different pointers. Use `chorda_comparare()` or the table's comparison function:

```c
/* GOOD */
si (chorda_comparare(clavis1, clavis2) == ZEPHYRUM)
{
    /* ... */
}
```

---

## API Reference

Alright, reference section. You know how this goes.

### Creation

#### `tabula_dispersa_creare_chorda()`

```c
TabulaDispersa* tabula_dispersa_creare_chorda(
    Piscina* piscina,
        i32  capacitas_initialis
);
```

Creates a hash table for `chorda` (string) keys.

**Parameters:**
- `piscina`: Arena allocator
- `capacitas_initialis`: Initial capacity (rounded up to next power of 2, minimum 16)

**Returns:**
- Pointer to TabulaDispersa on success
- `NIHIL` on failure

**Notes:**
- Uses FNV-1a hash function
- Uses `chorda_comparare()` for key comparison
- Load factor threshold: 0.75
- Deleted factor threshold: 0.25

**Example:**
```c
TabulaDispersa* tabula = tabula_dispersa_creare_chorda(piscina, CCLVI);
si (!tabula)
{
    imprimere("Failed to create table\n");
}
```

---

#### `tabula_dispersa_creare()`

```c
TabulaDispersa* tabula_dispersa_creare(
             Piscina* piscina,
                 i32  capacitas_initialis,
       TabulaFriatio  friatio,
    TabulaComparatio  comparatio
);
```

Creates a hash table with custom hash and comparison functions.

**Parameters:**
- `piscina`: Arena allocator
- `capacitas_initialis`: Initial capacity
- `friatio`: Hash function pointer (signature: `i32 (*)(chorda)`)
- `comparatio`: Comparison function pointer (signature: `i32 (*)(chorda, chorda)`)

**Returns:**
- Pointer to TabulaDispersa on success
- `NIHIL` if piscina, friatio, or comparatio is NULL

**Example:**
```c
TabulaDispersa* tabula = tabula_dispersa_creare(
    piscina,
    CXXVIII,
    tabula_friare_fnv1a,
    (TabulaComparatio)chorda_comparare
);
```

---

### Insertion

#### `tabula_dispersa_inserere()`

```c
b32 tabula_dispersa_inserere(
    TabulaDispersa* tabula,
            chorda  clavis,
            vacuum* valor
);
```

Inserts or updates a key-value pair.

**Parameters:**
- `tabula`: Hash table
- `clavis`: Key (chorda)
- `valor`: Value (void pointer)

**Returns:**
- `VERUM` on success
- `FALSUM` if tabula is NULL or key is empty

**Behavior:**
- If key exists, updates the value (doesn't increase count)
- If key is new, inserts it (increases count)
- May trigger resize if load factor or deleted factor is too high

**Example:**
```c
chorda clavis = chorda_ex_literis("username", piscina);
tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)XLII);
```

---

### Lookup

#### `tabula_dispersa_invenire()`

```c
b32 tabula_dispersa_invenire(
    TabulaDispersa*  tabula,
            chorda   clavis,
            vacuum** valor_out
);
```

Looks up a key and retrieves its value.

**Parameters:**
- `tabula`: Hash table
- `clavis`: Key to find
- `valor_out`: Output parameter for value (can be `NIHIL`)

**Returns:**
- `VERUM` if key found
- `FALSUM` if key not found or inputs invalid

**Example:**
```c
vacuum* valor;
si (tabula_dispersa_invenire(tabula, clavis, &valor))
{
    imprimere("Found: %d\n", (i32)(longus)valor);
}
alioquin
{
    imprimere("Not found\n");
}
```

---

#### `tabula_dispersa_continet()`

```c
b32 tabula_dispersa_continet(
    TabulaDispersa* tabula,
            chorda  clavis
);
```

Checks if a key exists without retrieving the value.

**Parameters:**
- `tabula`: Hash table
- `clavis`: Key to check

**Returns:**
- `VERUM` if key exists
- `FALSUM` if key doesn't exist

**Example:**
```c
si (tabula_dispersa_continet(tabula, clavis))
{
    imprimere("Key exists\n");
}
```

---

### Deletion

#### `tabula_dispersa_delere()`

```c
b32 tabula_dispersa_delere(
    TabulaDispersa* tabula,
            chorda  clavis
);
```

Deletes a key-value pair.

**Parameters:**
- `tabula`: Hash table
- `clavis`: Key to delete

**Returns:**
- `VERUM` if key was found and deleted
- `FALSUM` if key didn't exist

**Notes:**
- Marks slot as `SLOT_DELETUM` (tombstone)
- Decreases entry count
- Increases deleted count
- May trigger resize if deleted factor is too high

**Example:**
```c
si (tabula_dispersa_delere(tabula, clavis))
{
    imprimere("Deleted\n");
}
```

---

### Lifecycle

#### `tabula_dispersa_vacare()`

```c
vacuum tabula_dispersa_vacare(
    TabulaDispersa* tabula
);
```

Clears all entries from the table.

**Parameters:**
- `tabula`: Hash table to clear

**Notes:**
- Marks all slots as `SLOT_VACUUM`
- Resets counts to zero
- Capacity unchanged
- Faster than destroying and recreating

**Example:**
```c
tabula_dispersa_vacare(tabula);
/* Table is now empty, ready for reuse */
```

---

### Iteration

#### `tabula_dispersa_iterator_initium()`

```c
TabulaIterator tabula_dispersa_iterator_initium(
    TabulaDispersa* tabula
);
```

Creates an iterator at the beginning of the table.

**Parameters:**
- `tabula`: Hash table to iterate

**Returns:**
- TabulaIterator positioned at the start

**Example:**
```c
TabulaIterator iter = tabula_dispersa_iterator_initium(tabula);
```

---

#### `tabula_dispersa_iterator_proximum()`

```c
b32 tabula_dispersa_iterator_proximum(
    TabulaIterator*  iter,
            chorda*  clavis_out,
            vacuum** valor_out
);
```

Advances iterator to next entry.

**Parameters:**
- `iter`: Iterator to advance
- `clavis_out`: Output for key (can be `NIHIL`)
- `valor_out`: Output for value (can be `NIHIL`)

**Returns:**
- `VERUM` if entry found
- `FALSUM` if reached end

**Notes:**
- Order is undefined (depends on hash values)
- Don't modify table during iteration

**Example:**
```c
TabulaIterator iter = tabula_dispersa_iterator_initium(tabula);
chorda  clavis;
vacuum* valor;

dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
{
    imprimere("Key: %.*s\n", clavis.mensura, clavis.datum);
}
```

---

### Statistics

#### `tabula_dispersa_factor_oneris()`

```c
f32 tabula_dispersa_factor_oneris(
    TabulaDispersa* tabula
);
```

Returns load factor (entries / capacity).

**Parameters:**
- `tabula`: Hash table

**Returns:**
- Load factor as float (0.0 to 1.0)
- 0.0 if table is NULL or capacity is zero

**Example:**
```c
f32 load = tabula_dispersa_factor_oneris(tabula);
imprimere("Load: %.2f%%\n", load * C);
```

---

#### `tabula_dispersa_factor_deletorum()`

```c
f32 tabula_dispersa_factor_deletorum(
    TabulaDispersa* tabula
);
```

Returns deleted factor (deleted entries / capacity).

**Parameters:**
- `tabula`: Hash table

**Returns:**
- Deleted factor as float (0.0 to 1.0)

**Example:**
```c
f32 deleted = tabula_dispersa_factor_deletorum(tabula);
imprimere("Deleted: %.2f%%\n", deleted * C);
```

---

#### `tabula_dispersa_numerus()`

```c
i32 tabula_dispersa_numerus(
    TabulaDispersa* tabula
);
```

Returns number of entries in table.

**Parameters:**
- `tabula`: Hash table

**Returns:**
- Entry count
- 0 if table is NULL

**Example:**
```c
i32 count = tabula_dispersa_numerus(tabula);
imprimere("Table has %d entries\n", count);
```

---

#### `tabula_dispersa_status_imprimere()`

```c
vacuum tabula_dispersa_status_imprimere(
    TabulaDispersa* tabula
);
```

Prints detailed status to stdout.

**Parameters:**
- `tabula`: Hash table

**Output:**
```
Status Tabulae Dispersae:
  Capacitas: 128
  Numerus: 45
  Numerus deletorum: 3
  Factor oneris: 35.16%
  Factor deletorum: 2.34%
  Collisiones totales: 12
  Distantia maxima: 4
```

**Example:**
```c
tabula_dispersa_status_imprimere(tabula);
```

---

### Hash Functions

#### `tabula_friare_fnv1a()`

```c
i32 tabula_friare_fnv1a(
    chorda clavis
);
```

FNV-1a hash function.

**Parameters:**
- `clavis`: Key to hash

**Returns:**
- Hash value (i32)

**Notes:**
- Default hash function for string tables
- Fast, good distribution
- Industry standard

**Example:**
```c
chorda clavis = chorda_ex_literis("test", piscina);
i32 hash = tabula_friare_fnv1a(clavis);
```

---

#### `tabula_friare_multiplicatio()`

```c
i32 tabula_friare_multiplicatio(
    chorda clavis
);
```

Multiplication-based hash function (djb2 variant).

**Parameters:**
- `clavis`: Key to hash

**Returns:**
- Hash value (i32)

**Notes:**
- Alternative to FNV-1a
- Also fast, also good distribution

**Example:**
```c
i32 hash = tabula_friare_multiplicatio(clavis);
```

---

## FAQ (Questions I'm Sick Of Answering)

### Q: Why open addressing instead of chaining?

Because chaining requires allocating individual nodes for each collision, which means more allocations, more pointer chasing, worse cache locality, and more complexity. Open addressing keeps everything in one array. It's faster, simpler, and more cache-friendly.

### Q: Why linear probing instead of quadratic probing or double hashing?

Linear probing is simple and has excellent cache locality. You walk sequentially through memory, which the CPU's cache prefetcher loves. Quadratic probing and double hashing jump around more, which thrashes the cache.

Yes, linear probing can cause clustering. But with a good hash function and reasonable load factors, it's not a problem in practice.

### Q: What's a good initial capacity?

A power of 2 that's about 1.5x your expected number of entries. If you expect 100 entries, start with 128 or 256. If you expect 1000 entries, start with 2048.

The table will resize automatically if needed, but starting with a good size avoids early resizes.

### Q: Why does it round up to a power of 2?

For performance. With a power-of-2 capacity, we can use bitwise AND for modulo instead of expensive division. `hash & (capacity - 1)` is way faster than `hash % capacity`.

### Q: Can I store NULL as a value?

Yes. `NIHIL` is a valid value. When you look it up, you'll get `NIHIL` back. The table doesn't care what your values are.

### Q: What happens if I insert the same key twice?

The second insert updates the value. The table only stores one entry per unique key. The entry count doesn't increase on updates.

### Q: How do I know if a lookup returned NULL because the key doesn't exist or because the value is NULL?

Check the return value of `tabula_dispersa_invenire()`. It returns `VERUM` if the key was found (even if the value is NULL), and `FALSUM` if the key wasn't found.

```c
vacuum* valor;
si (tabula_dispersa_invenire(tabula, clavis, &valor))
{
    /* Key exists, valor might be NIHIL */
    si (valor == NIHIL)
    {
        imprimere("Value is NULL\n");
    }
}
alioquin
{
    /* Key doesn't exist */
}
```

### Q: Can I use this from multiple threads?

Not without your own locking. The table isn't thread-safe. If you need thread safety:
1. Give each thread its own table (best option)
2. Use a mutex to protect access

### Q: What's the maximum number of entries?

Practically unlimited. The capacity is an `i32`, so theoretical max is about 2 billion slots. But you'll run out of memory long before that.

### Q: Why do deletes use tombstones instead of actually removing entries?

Because of linear probing. If we actually removed entries, we'd break probe sequences for other keys that hashed to the same slot. Tombstones preserve the probe sequences while marking slots as available for reuse.

When tombstones accumulate, the table resizes and rehashes everything, cleaning them up.

### Q: How do I delete all entries matching some condition?

Iterate and collect keys, then delete after iteration finishes. Don't delete during iteration or you'll invalidate the iterator.

See the "Modifying During Iteration" section above for example code.

### Q: Can I resize the table manually?

No. Resizing is automatic. The library handles it based on load factor and deleted factor thresholds.

If you want more control, create the table with a larger initial capacity so it doesn't need to resize.

### Q: What if my hash function has terrible distribution?

Your table will be slow. You'll get lots of collisions, long probe sequences, and bad performance.

Fix: Use a better hash function. FNV-1a is good for most data. If you're hashing something weird, research appropriate hash functions for that data type.

### Q: Can I have multiple hash tables in the same piscina?

Yes. Create as many as you want from the same piscina. They'll all use the same memory pool. When you destroy the piscina, all tables are freed.

### Q: Does the iterator visit deleted entries?

No. It only visits occupied slots (`SLOT_OCCUPATUM`). Deleted and empty slots are skipped.

### Q: What's the performance compared to other hash table implementations?

For small to medium tables (< 10,000 entries), this is competitive with any hash table implementation. Open addressing with linear probing is cache-friendly and fast.

For huge tables (millions of entries), more sophisticated algorithms might win. But if you have millions of entries, you probably have bigger problems than hash table performance.

### Q: Can I store non-string keys?

Yes, but you need to represent them as `chorda` (bytes + length) and provide appropriate hash and comparison functions.

For integers: copy the integer into a byte array, create a chorda pointing to it, write a hash function that mixes the bits.

For structs: similar - serialize to bytes, hash the bytes.

### Q: Why is my table using more memory than expected?

Because capacity is rounded up to a power of 2, and the table resizes at 75% full. So if you insert 100 entries:
- Initial capacity: 128 (next power of 2 after 100)
- But 100 > 128 * 0.75 (96), so it resizes to 256
- You're using 256 slots for 100 entries

This is normal. The extra space keeps lookups fast.

### Q: How do I deep-copy a hash table?

You don't. This library doesn't provide deep copy because it depends on your key and value types. If you need it:
1. Create a new table
2. Iterate the old table
3. For each entry, copy the key and value (allocate from piscina)
4. Insert the copies into the new table

---

That's it. That's hash tables. Insert stuff, look it up fast, don't overthink it. If you screw this up I'm out of ideas.
