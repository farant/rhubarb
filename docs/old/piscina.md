# Piscina: The Memory Pool You Actually Need

Look, I'm gonna level with you here. You're writing C89. You know what that means? It means you're gonna be calling `malloc()` and `free()` approximately ten thousand times, you're gonna leak memory like a busted radiator, and you're gonna spend your Saturday nights running Valgrind wondering why your perfectly good code is suddenly eating 4GB of RAM.

Or - OR - you could use an arena allocator and sleep at night.

This is **piscina**. Latin for "swimming pool." Because that's what it is - a big pool of memory you can splash around in without worrying about cleaning up every individual drop of water. You allocate from it, you use what you got, and when you're done with the whole thing, you drain the pool. One function call. Everything gone. No leaks. No fragmentation. No staying up until 3 AM tracking down that one `free()` you forgot.

I've explained this concept to approximately four thousand people this week alone, so buckle up, we're doing this one more time.

## What Even IS This Thing?

An arena allocator. A memory pool. A bump allocator. Whatever you want to call it - the concept is dead simple and people somehow make it complicated anyway.

Here's the deal: you create one big chunk of memory (we call it a *piscina*), and then you hand out pieces of it on demand. That's it. No freeing individual allocations. When you're done with EVERYTHING, you destroy the whole pool and get your life back.

```c
Piscina* p = piscina_generare_dynamicum("my_stuff", 4096);
vacuum*  a = piscina_allocare(p, 100);  /* gimme 100 bytes */
vacuum*  b = piscina_allocare(p, 200);  /* gimme 200 more */
vacuum*  c = piscina_allocare(p, 50);   /* and 50 more */

/* ... do your work ... */

piscina_destruere(p);  /* BOOM. Everything gone. */
```

See that? Three allocations, one cleanup. Beautiful. Simple. Chef's kiss.

### Wait, Why Would I Want This?

Great question, glad you asked for the five hundredth time today. Here's why:

**Performance**: Allocating from a pool is basically just bumping a pointer. It's O(1), it's cache-friendly, it's fast as hell. No free lists, no block coalescing, no nothing.

**Simplicity**: Your cleanup code is one line. ONE LINE. You know how many memory leaks you can have with one line? Zero. That's how many.

**Predictability**: You want to know how much memory your operation uses? Look at the pool size. Done. No fragmentation mysteries, no "where did my 16MB go" debugging sessions.

**Locality**: Everything you allocate is close together in memory. Your CPU's cache prefetcher will actually work for once instead of thrashing all over the heap.

The tradeoff? You can't free individual allocations. You're all-in until you destroy the pool. But here's the thing - if you're writing parsers, compilers, request handlers, game frames, or basically anything with a clear lifetime, this is EXACTLY what you want.

## Latin Crash Course (Because Apparently We're Doing This)

Alright, since you decided to write your C code in Latin like some kind of scholastic programmer from the 13th century, let's learn some vocabulary. I'll teach you as we go, but here's the essentials:

- **piscina** = pool, swimming pool (the main memory pool)
- **alveus** = trough, bucket, channel (the individual memory chunks)
- **generare** = to generate, to create
- **destruere** = to destroy
- **allocare** = to allocate (yeah, this one's easy)
- **conari** = to try, to attempt
- **vacare** = to empty, to clear out
- **mensura** = size, measurement
- **capacitas** = capacity (also easy)
- **usus** = usage, use
- **summa** = total, sum

There. Now when you see `piscina_generare_dynamicum()` you can translate it as "pool_create_dynamic" and feel smart about it. You're welcome.

## How This Actually Works (The Mental Model)

Picture this: you've got a swimming pool - your *piscina*. But it's not just one big pool, it's potentially multiple connected pools - these are your *alvei* (plural of *alveus*, which means bucket or trough, and yes, I know that's a weird mental image but just go with it).

Each *alveus* is a chunk of memory with:
- A buffer (the actual memory you can use)
- A capacity (how big the buffer is)
- An offset (how much you've used so far)
- A link to the next bucket (if there is one)

When you allocate memory, the pool just bumps the offset forward and hands you a pointer. Fast. Simple. No thinking required.

```c
/* Simplified view of what's happening internally */

Piscina:
  alveus #1: [used: 500 bytes] [free: 3596 bytes] -> alveus #2
  alveus #2: [used: 1000 bytes] [free: 7192 bytes] -> NULL
             ^
             |
          current allocation point
```

When the current bucket fills up, one of three things happens:

1. **If there's a next bucket already**: Move to it and keep allocating
2. **If you're in dynamic mode**: Create a new bucket (2x the size of the original)
3. **If you're in fixed mode**: Return NULL and make you deal with it

That's literally it. That's the whole algorithm.

## Creating Your Pool

You've got two choices here, and the names are annoyingly long but at least they're descriptive.

### Dynamic Pools (The Normal Choice)

```c
Piscina* p = piscina_generare_dynamicum("parser_pool", 4096);
```

Let's break down that function name because I know you're squinting at it:
- `piscina` - pool (duh)
- `generare` - to generate/create
- `dynamicum` - dynamic (it can grow)

This creates a pool that starts with one 4KB bucket. If you fill it up, it automatically allocates a new bucket that's twice the size of the original (8KB in this case). Then if you fill THAT up, it makes a 16KB bucket. And so on.

The first argument is a name. It's optional (pass `NIHIL` if you don't care), but it helps with debugging. When you're staring at allocation failures at midnight, you'll thank yourself for naming your pools.

The second argument is the initial bucket size. Pick something reasonable:
- Too small (like 64 bytes): You'll constantly allocate new buckets. Wasteful.
- Too large (like 1GB): You'll waste memory you're not using. Also wasteful.
- Just right (4KB - 64KB): Goldilocks zone. You're smart.

### Fixed-Size Pools (The "I Know Exactly What I'm Doing" Choice)

```c
Piscina* p = piscina_generare_certae_magnitudinis("fixed_pool", 8192);
```

More Latin for you:
- `certae` - certain, definite
- `magnitudinis` - of size

This creates a pool with exactly one bucket of exactly the size you specified. It will NEVER grow. When you're out of space, allocations return NULL (if you use the safe variant) or crash your program (if you use the unsafe variant).

Why would you want this? Because sometimes you know EXACTLY how much memory your operation needs, and you want a hard failure if you go over. Better than silently allocating gigabytes because you had a bug.

```c
/* I'm parsing a config file. I KNOW it's under 8KB. */
Piscina* p = piscina_generare_certae_magnitudinis("config", 8192);

/* ... parse config ... */

si (something_went_wrong)
{
    /* If I somehow exceeded 8KB, something is VERY wrong */
    imprimere("Config file too large, aborting\n");
    piscina_destruere(p);
    exire(I);
}
```

### Destroying Your Pool (The Easy Part)

```c
piscina_destruere(p);
```

That's it. Every allocation you made from this pool? Gone. Every bucket? Freed. Every pointer you got from `piscina_allocare()`? Invalid.

This is the beauty of arena allocation - your cleanup is a single line. You don't track individual allocations. You don't maintain free lists. You just nuke the whole thing and move on with your life.

**Important**: After you call this, that `Piscina*` pointer is dead. Don't use it. Don't even look at it. It's gone.

## Allocating Memory (Finally)

Okay, you've got your pool. Now you want memory. You've got options.

### The Normal Way (Dies On Failure)

```c
vacuum* ptr = piscina_allocare(piscina, 256);
```

This allocates 256 bytes and gives you a pointer. If it can't allocate (you're out of space and can't grow), it prints an error and calls `exire(I)`. Your program dies. Harsh but simple.

Use this when allocation failure means something is catastrophically wrong and you should just give up. Which, let's be honest, is most of the time.

```c
structura Config
{
    character  nome[64];
    i32        valor;
};

Piscina* p = piscina_generare_dynamicum("app_pool", 4096);

/* This will succeed or die trying */
structura Config* cfg = piscina_allocare(p, magnitudo(structura Config));
cfg->valor = 42;
```

### The Safe Way (Returns NULL On Failure)

```c
vacuum* ptr = piscina_conari_allocare(piscina, 256);
si (!ptr)
{
    /* Handle the failure yourself */
    imprimere("Out of memory!\n");
    redde FALSUM;
}
```

`conari` means "to try" - this function TRIES to allocate but won't crash if it fails. You get NULL back and you handle it like an adult.

Use this when you're not sure if the allocation will succeed and you want to gracefully handle failure.

```c
/* Reading user input - could be anything */
vacuum* buffer = piscina_conari_allocare(p, user_size);
si (!buffer)
{
    imprimere("Allocation failed, size too large: %zu\n", user_size);
    redde NIHIL;
}
```

### Aligned Allocations (For The Performance Nerds)

Sometimes you need your memory aligned to specific boundaries. Maybe you're doing SIMD operations and need 16-byte alignment. Maybe you're allocating structures with strict alignment requirements. Whatever.

```c
vacuum* ptr = piscina_allocare_ordinatum(p, 64, XVI);
```

That allocates 64 bytes, aligned to a 16-byte boundary. The pointer you get back will be a multiple of 16.

There's also `piscina_conari_allocare_ordinatum()` if you want the safe version.

Why would you need this? Here's a real example:

```c
/* SSE requires 16-byte alignment */
#define ALIGNMENT_SSE XVI

structura Vector4
{
    f32 x, y, z, w;
};

/* This better be aligned or SSE instructions will crash */
structura Vector4* v = piscina_allocare_ordinatum(
    p,
    magnitudo(structura Vector4),
    ALIGNMENT_SSE
);
```

The pool handles the alignment for you. It might waste a few bytes padding to get there, but that's fine - you're using an arena allocator, you've already accepted some waste in exchange for speed and simplicity.

## Managing The Lifecycle

Alright, you've allocated a bunch of stuff. Now what?

### Clearing The Pool (Reuse Without Destroying)

```c
piscina_vacare(p);
```

`vacare` means "to empty" or "to be empty." This resets all the buckets back to their starting state. The memory is still allocated, but the offsets are back to zero. It's like draining and refilling the pool.

```c
/* Process 1000 files, reusing the same pool */
per (i32 i = ZEPHYRUM; i < M; i++)
{
    /* Allocate stuff for this file */
    character* buffer = piscina_allocare(p, file_sizes[i]);
    /* ... process file ... */

    /* Clear pool for next iteration */
    piscina_vacare(p);
}
```

This is way faster than destroying and recreating the pool every iteration. The buckets stay allocated, you just reset them.

**Warning**: After calling `vacare()`, all the pointers you got from `piscina_allocare()` are invalid. The memory they point to still exists, but you're about to overwrite it with new allocations. Don't use those pointers.

### Checking Usage (Because You're Curious)

Want to know how much memory you're using? We've got functions for that.

```c
/* Total bytes allocated from the pool */
memoriae_index used = piscina_summa_usus(p);

/* Total bytes wasted (allocated but not used) */
memoriae_index waste = piscina_summa_inutilis_allocatus(p);

/* Bytes remaining in current bucket before growth */
memoriae_index remaining = piscina_reliqua_antequam_cresca_alvei(p);

/* Peak usage across the pool's lifetime */
memoriae_index peak = piscina_summa_apex_usus(p);
```

More Latin vocabulary:
- `summa` = total, sum
- `usus` = use, usage
- `inutilis` = useless, wasted
- `allocatus` = allocated
- `reliqua` = remaining, leftover
- `antequam` = before
- `cresca` = growth (from crescere, to grow)
- `alvei` = of the bucket
- `apex` = peak, summit

These are mostly useful for debugging and profiling. You can check if you're wasting a ton of memory on alignment padding, or if your pool is constantly growing and you should just start with a bigger size.

```c
Piscina* p = piscina_generare_dynamicum("test", 1024);

vacuum* a = piscina_allocare(p, 500);
vacuum* b = piscina_allocare(p, 300);

imprimere("Used: %zu bytes\n", piscina_summa_usus(p));         /* 800 */
imprimere("Wasted: %zu bytes\n", piscina_summa_inutilis_allocatus(p)); /* 224 */
imprimere("Remaining: %zu bytes\n", piscina_reliqua_antequam_cresca_alvei(p)); /* 224 */
```

The peak usage one is particularly useful - it tells you the high-water mark, even after you've cleared the pool. Great for figuring out how big your initial bucket should be.

## Common Patterns (That You're Gonna Use Anyway)

### Per-Request Pools

You're writing a web server. Each request comes in, you parse it, process it, send a response. You could track all those little allocations... or:

```c
vacuum handle_request(Request* req)
{
    Piscina* p = piscina_generare_dynamicum("request", 8192);

    /* Parse request body */
    character* body = parse_body(req, p);

    /* Allocate response buffer */
    character* response = piscina_allocare(p, 4096);

    /* Do your work */
    generate_response(body, response);
    send_response(response);

    /* One line cleanup */
    piscina_destruere(p);
}
```

Every request gets its own pool. When the request is done, nuke the pool. No leaks, no tracking, no problems.

### Parser Scratch Space

You're parsing JSON or XML or whatever. You need temporary buffers for strings, arrays, objects. You could malloc each one... or:

```c
JsonValue* parse_json(constans character* input)
{
    Piscina* scratch = piscina_generare_dynamicum("json_scratch", 16384);

    /* All your temporary parsing allocations come from here */
    JsonValue* result = parse_value(input, scratch);

    /* Copy the final result to permanent storage */
    JsonValue* permanent = copy_to_permanent(result);

    /* Trash all the temporary stuff */
    piscina_destruere(scratch);

    redde permanent;
}
```

### Frame-Based Allocation (Games)

Every frame, you allocate temporary stuff. Particle effects, UI elements, whatever. At the end of the frame, trash it all.

```c
Piscina* frame_pool = piscina_generare_dynamicum("frame", 1024 * 1024);

dum (game_running)
{
    /* Allocate frame stuff */
    Particle* particles = piscina_allocare(frame_pool, particle_count * magnitudo(Particle));
    UIElement* ui = piscina_allocare(frame_pool, ui_count * magnitudo(UIElement));

    render_frame(particles, ui);

    /* Clear for next frame */
    piscina_vacare(frame_pool);
}

piscina_destruere(frame_pool);
```

See the pattern? Create pool, use pool, trash pool. Over and over. Simple.

## Things That Will Bite You (Read This Or Suffer)

### Don't Use Pointers After Destroying The Pool

I know I already said this, but people KEEP doing it, so I'm saying it again.

```c
/* BAD - DO NOT DO THIS */
character* str = piscina_allocare(p, 100);
strcpy(str, "hello");
piscina_destruere(p);
imprimere("%s\n", str);  /* CRASHES. str is invalid. */
```

After `piscina_destruere()`, every pointer you got from that pool is DEAD. Don't use them. They point to freed memory. You will crash, or worse, you'll get corrupted data and spend hours debugging.

Same goes for `piscina_vacare()`:

```c
/* ALSO BAD */
character* str = piscina_allocare(p, 100);
strcpy(str, "hello");
piscina_vacare(p);
imprimere("%s\n", str);  /* Undefined behavior. Might work, might crash, might summon demons. */
```

### The Pool Doesn't Zero Memory (Unless You Ask)

When you allocate, you get whatever garbage was in memory before. If you need zeros:

```c
vacuum* ptr = piscina_allocare(p, 100);
memset(ptr, ZEPHYRUM, 100);  /* Zero it yourself */
```

Or just be an adult and initialize your structures properly.

### You Can't Free Individual Allocations

This is BY DESIGN. You don't get a `piscina_liberare()` function. Because that's not how arena allocators work.

If you need to free things individually, use `malloc()` and `free()` like everyone else. But then you're back to tracking everything and hunting for leaks. Your choice.

### Fixed-Size Pools Don't Grow (Obviously)

If you create a fixed-size pool and run out of space, you're done. The safe functions return NULL. The unsafe ones crash your program.

```c
Piscina* p = piscina_generare_certae_magnitudinis("small", 100);

vacuum* a = piscina_allocare(p, 50);   /* OK */
vacuum* b = piscina_allocare(p, 60);   /* CRASH - doesn't fit */
```

Plan your sizes accordingly, or use dynamic pools.

## Advanced Usage (For When You Get Fancy)

### Nested Pools

You can create pools from pools. Why? Because you can, that's why.

```c
Piscina* outer = piscina_generare_dynamicum("outer", 32768);

Piscina* inner = piscina_allocare(outer, magnitudo(Piscina));
/* ... initialize inner pool manually ... */
```

Honestly, this is pretty rare. Usually you just create separate pools and manage them independently. But if you're building some kind of hierarchical system where you want to trash a whole subtree of allocations at once, this could be useful.

### Large Allocations In Dynamic Pools

What happens if you allocate something bigger than the bucket size?

```c
Piscina* p = piscina_generare_dynamicum("test", 1024);

/* This is bigger than 1KB */
vacuum* huge = piscina_allocare(p, 8192);
```

The pool is smart about this. It allocates a new bucket that's big enough to hold your request PLUS the normal growth amount. So you get an 8192 + 1024 = 9216 byte bucket, and the next bucket will be 9216 * 2 = 18432 bytes.

This keeps growth exponential even when you throw large allocations at it.

## API Reference

Alright, here's the boring part where I list every function and what it does. You've probably figured most of this out by now, but for completeness:

### Creation & Destruction

#### `piscina_generare_dynamicum()`

```c
Piscina* piscina_generare_dynamicum(
    constans character* piscinae_titulum,
        memoriae_index  mensura_alvei_initia
);
```

Creates a dynamic pool that grows automatically.

**Parameters:**
- `piscinae_titulum`: Name for debugging (can be `NIHIL`)
- `mensura_alvei_initia`: Initial bucket size in bytes

**Returns:** Pointer to new pool, or `NIHIL` on allocation failure

**Example:**
```c
Piscina* p = piscina_generare_dynamicum("my_pool", 4096);
si (!p)
{
    imprimere("Failed to create pool\n");
    exire(I);
}
```

---

#### `piscina_generare_certae_magnitudinis()`

```c
Piscina* piscina_generare_certae_magnitudinis(
    constans character* piscinae_titulum,
        memoriae_index  mensura_buffer
);
```

Creates a fixed-size pool that never grows.

**Parameters:**
- `piscinae_titulum`: Name for debugging (can be `NIHIL`)
- `mensura_buffer`: Total pool size in bytes

**Returns:** Pointer to new pool, or `NIHIL` on allocation failure

**Example:**
```c
Piscina* p = piscina_generare_certae_magnitudinis("fixed", 8192);
```

---

#### `piscina_destruere()`

```c
vacuum piscina_destruere(Piscina* piscina);
```

Destroys the pool and frees all memory.

**Parameters:**
- `piscina`: The pool to destroy

**Side effects:** All pointers allocated from this pool become invalid

**Example:**
```c
piscina_destruere(p);
/* Don't use p or anything allocated from it anymore */
```

---

### Allocation (Fatal On Failure)

#### `piscina_allocare()`

```c
vacuum* piscina_allocare(
           Piscina* piscina,
    memoriae_index  mensura
);
```

Allocates memory from the pool. Calls `exire(I)` on failure.

**Parameters:**
- `piscina`: The pool to allocate from
- `mensura`: Number of bytes to allocate

**Returns:** Pointer to allocated memory (never NULL)

**Crashes if:** Out of memory and can't grow

**Example:**
```c
i32* numbers = piscina_allocare(p, X * magnitudo(i32));
```

---

#### `piscina_allocare_ordinatum()`

```c
vacuum* piscina_allocare_ordinatum(
           Piscina* piscina,
    memoriae_index  mensura,
    memoriae_index  ordinatio
);
```

Allocates aligned memory from the pool. Calls `exire(I)` on failure.

**Parameters:**
- `piscina`: The pool to allocate from
- `mensura`: Number of bytes to allocate
- `ordinatio`: Alignment in bytes (must be power of 2)

**Returns:** Pointer to allocated memory, aligned to `ordinatio` bytes

**Example:**
```c
/* Allocate 64 bytes aligned to 16-byte boundary */
vacuum* aligned = piscina_allocare_ordinatum(p, LXIV, XVI);
```

---

### Allocation (Returns NULL On Failure)

#### `piscina_conari_allocare()`

```c
vacuum* piscina_conari_allocare(
           Piscina* piscina,
    memoriae_index  mensura
);
```

Tries to allocate memory from the pool. Returns `NIHIL` on failure.

**Parameters:**
- `piscina`: The pool to allocate from
- `mensura`: Number of bytes to allocate

**Returns:** Pointer to allocated memory, or `NIHIL` if failed

**Example:**
```c
vacuum* ptr = piscina_conari_allocare(p, huge_size);
si (!ptr)
{
    imprimere("Allocation failed\n");
    redde FALSUM;
}
```

---

#### `piscina_conari_allocare_ordinatum()`

```c
vacuum* piscina_conari_allocare_ordinatum(
           Piscina* piscina,
    memoriae_index  mensura,
    memoriae_index  ordinatio
);
```

Tries to allocate aligned memory. Returns `NIHIL` on failure.

**Parameters:**
- `piscina`: The pool to allocate from
- `mensura`: Number of bytes to allocate
- `ordinatio`: Alignment in bytes (must be power of 2)

**Returns:** Pointer to aligned memory, or `NIHIL` if failed

---

### Lifecycle Management

#### `piscina_vacare()`

```c
vacuum piscina_vacare(Piscina* piscina);
```

Clears all allocations without freeing the buckets.

**Parameters:**
- `piscina`: The pool to clear

**Side effects:**
- Resets all bucket offsets to zero
- All previous pointers from this pool become invalid
- Buckets remain allocated for reuse

**Example:**
```c
per (i32 i = ZEPHYRUM; i < iterations; i++)
{
    /* Allocate stuff */
    vacuum* temp = piscina_allocare(p, size);
    /* Use it */
    process(temp);
    /* Clear for next iteration */
    piscina_vacare(p);
}
```

---

### Query Functions

#### `piscina_summa_usus()`

```c
memoriae_index piscina_summa_usus(constans Piscina* piscina);
```

Returns total bytes currently allocated from the pool.

**Returns:** Sum of all bucket offsets

**Example:**
```c
imprimere("Pool usage: %zu bytes\n", piscina_summa_usus(p));
```

---

#### `piscina_summa_inutilis_allocatus()`

```c
memoriae_index piscina_summa_inutilis_allocatus(constans Piscina* piscina);
```

Returns total bytes allocated but unused (wasted space).

**Returns:** Sum of (capacity - offset) across all buckets

**Example:**
```c
memoriae_index waste = piscina_summa_inutilis_allocatus(p);
imprimere("Wasted: %zu bytes\n", waste);
```

---

#### `piscina_reliqua_antequam_cresca_alvei()`

```c
memoriae_index piscina_reliqua_antequam_cresca_alvei(constans Piscina* piscina);
```

Returns bytes remaining in current bucket before growth needed.

**Returns:** Current bucket's (capacity - offset)

**Example:**
```c
memoriae_index left = piscina_reliqua_antequam_cresca_alvei(p);
si (left < needed_size)
{
    imprimere("Next allocation will trigger growth\n");
}
```

---

#### `piscina_summa_apex_usus()`

```c
memoriae_index piscina_summa_apex_usus(constans Piscina* piscina);
```

Returns peak usage across the pool's lifetime.

**Returns:** Maximum value `piscina_summa_usus()` has ever returned for this pool

**Note:** This persists even after `piscina_vacare()`

**Example:**
```c
/* After many allocations and clears */
memoriae_index peak = piscina_summa_apex_usus(p);
imprimere("Peak usage was: %zu bytes\n", peak);
```

---

## FAQ (Frequently Asked Questions That Make Me Want To Scream)

### Q: Why is it called "piscina"?

Because it's Latin for "swimming pool" or "fish pond," and this is a memory pool. Also because the author has a thing for Latin and medieval scholasticism. Deal with it.

### Q: Can I free individual allocations?

No. NO. **NO.** That's not how arena allocators work. You allocate, you use, you destroy the whole pool when you're done. If you need individual freeing, use `malloc()` and `free()` like a normal person.

### Q: What happens to my pointers after `piscina_vacare()`?

They're invalid. Dead. Gone. The memory still exists but you're about to overwrite it with new allocations. Don't use them.

Same question for `piscina_destruere()` - also invalid. Don't use them. I've told you this three times now.

### Q: Should I use dynamic or fixed-size pools?

Use dynamic unless you have a VERY good reason not to. Fixed-size pools are for when you know exactly how much memory you need and you want a hard failure if you exceed it. 99% of the time, you want dynamic.

### Q: How big should my initial bucket be?

Start with 4KB - 64KB depending on your use case. Too small and you'll constantly grow. Too large and you'll waste memory. Profile and adjust.

If your peak usage is consistently 50KB, start with 64KB and you'll probably never need to grow.

### Q: What's the performance compared to malloc/free?

Allocation is O(1) pointer bumping - way faster than malloc. Freeing is also O(1) (you destroy the whole pool). But you can't free individual allocations, so it depends on your use case.

For things with clear lifetimes (requests, frames, parsing, etc.), pools destroy malloc/free in both performance and simplicity.

### Q: Why does `piscina_allocare()` crash instead of returning NULL?

Because in most code, allocation failure means something is catastrophically wrong and you should just give up. Handling allocation failures everywhere clutters your code with error checks that almost never happen.

If you want to handle failures gracefully, use `piscina_conari_allocare()` which returns NULL.

### Q: Can I create a pool from another pool?

Technically yes, but why would you? Just create separate pools. It's not like you're running out of pools.

### Q: What's with all the Latin variable names?

That's the project style. Everything is in Latin. It's weird, it's quirky, it makes your code look like it was written by Thomas Aquinas. You'll get used to it.

Quick reference:
- `vacuum` = void
- `si` = if
- `alioquin` = else
- `redde` = return
- `per` = for
- `dum` = while
- `NIHIL` = NULL
- `VERUM` = true (1)
- `FALSUM` = false (0)

### Q: Why do dynamic pools double in size?

Because exponential growth is the right strategy. If you grow linearly (add 4KB each time), you'll allocate tons of buckets. If you grow exponentially (double each time), you allocate log(n) buckets.

Plus it's simple and proven - every good dynamic array implementation does this.

### Q: What happens if I allocate zero bytes?

You get `NIHIL` back. Don't allocate zero bytes, you weirdo.

### Q: Can I use this in multithreaded code?

Not without your own locking. The pool isn't thread-safe. If you need thread-safe allocation, either:
1. Give each thread its own pool (recommended)
2. Wrap calls in mutexes (if you must)

Option 1 is better - each thread gets its own pool, no contention, no locks, everything is fast.

### Q: What if my allocation is bigger than the bucket size?

The pool allocates a new bucket big enough to hold it, plus some extra for future growth. So if your bucket is 4KB and you allocate 10KB, you get a bucket of about 14KB.

### Q: Why would I use this over malloc?

- **Speed**: Bumping a pointer beats free list searching
- **Simplicity**: One cleanup call beats tracking hundreds of frees
- **Locality**: Everything close together in memory
- **Predictability**: No fragmentation, no surprises
- **Profiling**: Easy to see exactly how much memory you're using

Use pools for things with clear lifetimes. Use malloc for long-lived data with complex lifetimes.

### Q: Can the pool run out of address space?

On 32-bit systems, theoretically yes, you could allocate 4GB of buckets. On 64-bit, you'll run out of physical RAM first.

In practice, if you're allocating gigabytes from a pool, you're probably doing something wrong. Pools are for temporary allocations with reasonable sizes.

---

Look, that's it. That's piscina. It's not complicated. Create pool, allocate from pool, destroy pool. Don't overthink it.

If you screw this up somehow, I don't even know what to tell you. The entire API is like ten functions. Just read the examples and copy them.

Now go write some code and stop bothering me with questions I've answered a thousand times already.
