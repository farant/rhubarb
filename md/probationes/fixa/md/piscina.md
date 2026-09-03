# Piscina

An arena allocator: one big block of memory you hand out pieces of, and instead of
freeing those pieces one at a time, you free the whole block at once when you're done
with it. This document teaches you how to use it, then serves as a reference once you
already know it.

## An old technique, not a new one

Every so often a technique falls out of fashion not because it stopped working, but
because the generation that needed it retired. Arena allocation is one of those. The
Apache HTTP Server has used it since the 1990s, under the name *pools* — every request
gets one, and when the request finishes, the whole pool is freed in a single call,
regardless of how many small allocations happened inside it. PostgreSQL does the same
thing under a different name, *memory contexts*, scoped to a query or a transaction.
id Software's Quake engine has something in the same family, the Hunk allocator — a
block that grows from both ends toward the middle, holding a level's models, sounds,
and textures until the level unloads, at which point the whole thing clears at once
rather than being walked and freed piece by piece. Even GCC's own compiler internals
use a version of this, called an *obstack* — object stack — for its own working
memory.

None of these projects copied each other. Anyone who spends long enough fighting
`malloc()`'s bookkeeping problem — track every allocation, remember to free every one,
exactly once, in code paths you may not control — arrives at roughly the same idea
independently: stop tracking allocations individually, and start tracking *lifetimes*
instead. Group everything that dies at the same moment, and free it all in one motion.
The formal name for this is region-based memory management, and the idea has an
academic paper trail too — Ruggieri and Murtagh described compiler-inserted regions
tied to function scope in 1988; Tofte and Talpin generalized it into a full type
theory for Standard ML in 1994. We didn't invent any of this. We're doing what a
fairly large and varied set of people, working on unrelated problems, kept
re-discovering was the right shape for memory whose lifetime you can see coming.

```c
Piscina* p = piscina_generare_dynamicum("meum_opus", 4096);

vacuum* a = piscina_allocare(p, 100);
vacuum* b = piscina_allocare(p, 200);
vacuum* c = piscina_allocare(p, 50);

/* ... fac opus tuum ... */

piscina_destruere(p);
```

Three allocations, one cleanup call. That line at the bottom is the entire reason this
library exists.

## Latin, since you'll be reading a lot of it

You're writing C89 in Latin, so before anything else, here's the vocabulary this
library is built from. You'll see these words constantly, so it's worth having them
land now rather than working them out function name by function name later.

| Latin | Meaning | Shows up as |
|---|---|---|
| *piscina* | pool, swimming pool | the pool itself |
| *alveus* | trough, channel, bucket | the individual memory chunks inside a pool |
| *generare* | to generate, create | `piscina_generare_dynamicum` |
| *destruere* | to destroy | `piscina_destruere` |
| *allocare* | to allocate | `piscina_allocare` |
| *conari* | to try, attempt | `piscina_conari_allocare` |
| *vacare* | to empty, be empty | `piscina_vacare` |
| *notare* | to mark, note | `piscina_notare` |
| *reficere* | to restore, repair | `piscina_reficere` |
| *mensura* | measurement, size | `mensura` parameters throughout |
| *capacitas* | capacity | field inside `Alveus` |
| *usus* | use, usage | `piscina_summa_usus` |
| *summa* | total, sum | `piscina_summa_usus`, `piscina_summa_apex_usus` |
| *ordinatio* | ordering, arrangement | alignment — `piscina_allocare_ordinatum` |
| *apex* | peak, summit | `piscina_summa_apex_usus` — peak usage |

*Piscina* is Latin for "swimming pool," and the mental picture is a deliberate one:
a pool is one body of water you draw from, not a collection of individually-tracked
cups.

## How it actually works

A `Piscina` doesn't hold memory directly. It holds a chain of `Alveus` structs —
buckets — and each bucket owns one real block of memory:

```c
nomen structura Alveus
{
              vacuum* buffer;
      memoriae_index  capacitas;
      memoriae_index  offset;
    structura Alveus* sequens;
} Alveus;
```

*Buffer* is the real memory. *Capacitas* is how big it is. *Offset* is how much of it
is currently spoken for. *Sequens* — "next" — points to the following bucket, if the
pool has grown past one.

Allocating memory is nothing more than moving `offset` forward and handing back a
pointer to where it used to be:

```
alveus:  [ used: 500 bytes | free: 3596 bytes ] -> alveus #2 -> NIHIL
                            ^
                            offset (next allocation starts here)
```

That's the entire fast path. No searching for a free block big enough, no merging
adjacent free blocks back together when something is freed — because nothing gets
freed individually in the first place. When the current bucket runs out of room,
`piscina` does one of three things, in order: move to a bucket that already exists
further down the chain, create a new bucket if the pool is allowed to grow, or give up
if it isn't. That's covered properly in the next section.

## Creating a pool

Two constructors, and the difference between them is whether the pool is allowed to
grow past its starting size.

### Dynamic pools

```c
Piscina* p = piscina_generare_dynamicum("parser_pool", 4096);
```

The first argument is a name — purely for debugging, safe to pass `NIHIL` if you don't
care, but worth having when you're staring at a failed allocation and trying to figure
out which pool ran out of room. The second is the starting bucket size in bytes.

A dynamic pool begins with one bucket of that size, and that starting size doubles
into a *growth baseline* that every later bucket is sized from. When the current
bucket fills up, `piscina` creates a new bucket at exactly that baseline — not twice
the size of the bucket that just filled. In the ordinary case, that means every bucket
after the first is the same size as the one before it, not progressively larger the
way a growable array typically gets: filling many small buckets doesn't make the next
one bigger.

The baseline only moves when a single allocation is bigger than it. In that case, the
new bucket is sized to fit the oversized request plus the pool's original starting
size, and *that* becomes the new baseline every later ordinary bucket doubles from —
so a pool that's only ever seen small allocations stays flat, while one that's been
handed a couple of unusually large single requests will have visibly larger buckets
from that point on.

### Fixed-size pools

```c
Piscina* p = piscina_generare_certae_magnitudinis("config_pool", 8192);
```

*Certae magnitudinis* — "of a certain size." This creates exactly one bucket, sized
exactly as given, and it will never grow. When it fills up, allocation fails — either
by returning `NIHIL` or by killing the program, depending on which allocation function
you called (the next section covers that choice).

Reach for this when you know the ceiling ahead of time and want a hard failure if
you're wrong, rather than a silent memory increase that hides a bug. Parsing a config
file that should never exceed a few kilobytes is a reasonable case: if it somehow does,
something upstream is already broken, and you'd rather find out immediately than
allocate gigabytes trying to be accommodating.

### Destroying a pool

```c
piscina_destruere(p);
```

Every bucket in the chain gets freed, the pool's own bookkeeping struct gets freed, and
every pointer you ever got back from `piscina_allocare()` on this pool is now
dangling. Nothing about the pointers themselves changes — they still hold the same
addresses — but the memory behind them is gone. Using one after this call is undefined
behavior, the same as using any pointer after `free()`.

## Allocating memory

Every allocation in `piscina` is a choice along two independent axes: what happens on
failure, and whether you need specific alignment. That gives four functions, and the
FAQ has more to say about why it's four functions instead of one with flags — for now,
here's what each does.

**Dies on failure, no alignment requirement:**

```c
vacuum* ptr = piscina_allocare(p, 256);
```

If this can't get you 256 bytes — the pool is fixed-size and full, or a dynamic pool's
own bucket allocation failed — it prints an error naming the pool and calls `exire(I)`.
Your program ends. This sounds harsh until you notice how often "allocation failed" and
"something is already badly wrong" are the same event; in that case, a graceful `si
(!ptr)` check at every call site is just ceremony around a failure nobody was going to
recover from anyway.

**Returns `NIHIL` on failure:**

```c
vacuum* ptr = piscina_conari_allocare(p, user_supplied_size);
si (!ptr)
{
    imprimere("Allocation failed for %zu bytes\n", user_supplied_size);
    redde FALSUM;
}
```

*Conari* — "to try." Use this whenever the size in question came from outside your
control — user input, a file on disk, a network message — and a too-large request is
a normal event you intend to handle rather than a sign your program has gone wrong.

**Aligned versions of both:**

```c
structura Vector4* v = piscina_allocare_ordinatum(
    p,
    magnitudo(structura Vector4),
    XVI);
```

*Ordinatio* — alignment. SIMD types, and any structure your platform's ABI expects to
land on a specific byte boundary, need this. The pool pads the offset forward as
needed to hit the requested alignment before handing back a pointer, at the cost of a
few wasted bytes of padding — an arena allocator has already accepted some waste in
exchange for speed, and a little alignment padding is more of the same trade.
`piscina_conari_allocare_ordinatum()` combines this with the NIHIL-on-failure variant.

## Managing a pool's lifecycle

Once you're allocating from a pool, there are two different ways to reclaim space
without destroying the whole thing — and they solve different problems.

### Clearing everything: `piscina_vacare()`

```c
per (i32 i = ZEPHYRUM; i < numerus_plicarum; i++)
{
    character* buffer = piscina_allocare(p, dimensiones_plicae[i]);
    /* ... processa plicam ... */
    piscina_vacare(p);
}
```

*Vacare* — to empty. Every bucket's `offset` resets to zero, but the buckets
themselves stay allocated, ready to be reused on the next pass. This is meaningfully
cheaper than destroying and recreating the pool every iteration, because the
underlying `malloc()`-backed memory never gets freed and re-requested — you're only
resetting an integer per bucket.

Exactly like `piscina_destruere()`, every pointer handed out before the call is now
invalid. The memory is still there — it hasn't been returned to the operating system 
— but the next allocation is about to write over it.

### Rewinding partway: mark and reset

`piscina_vacare()` is all-or-nothing. Sometimes you want to allocate some temporary
scratch space, use it, and give just that back — without touching allocations that
came before it and need to survive. That's `piscina_notare()` and
`piscina_reficere()`:

```c
PiscinaNotatio nota = piscina_notare(p);

vacuum* temporaria = piscina_allocare(p, 4096);
/* ... utere temporaria pro opere transeunti ... */

piscina_reficere(p, nota);
/* Omnia allocata post 'nota' sunt nunc invalida.
 * Omnia allocata ANTE 'nota' remanent valida. */
```

*Notare* — to note, to mark — captures exactly where you are in the pool right now:
which bucket, and how far into it. *Reficere* — to restore — rewinds the pool back to
that exact point, invalidating everything allocated in between while leaving anything
allocated before the mark untouched. This is the one place `piscina` behaves like a
true stack rather than a pure bump allocator: marks nest correctly as long as you
reset them in the reverse order you set them, the same discipline any stack requires.

A parser is the natural example: mark before attempting to parse an expression,
allocate freely while trying, and reset back to the mark if the attempt fails and you
need to backtrack — the permanent parse tree built before the mark survives, the
failed attempt's scratch work doesn't.

## Checking on a pool

Four query functions, none of which change anything:

```c
memoriae_index usus     = piscina_summa_usus(p);
memoriae_index vastum   = piscina_summa_inutilis_allocatus(p);
memoriae_index reliqua  = piscina_reliqua_antequam_cresca_alvei(p);
memoriae_index apex     = piscina_summa_apex_usus(p);
```

*Usus* is straightforward — total bytes currently allocated, summed across every
bucket. *Inutilis allocatus* — "uselessly allocated" — is the gap between what's been
handed out and what each bucket actually holds, i.e. wasted capacity, mostly useful
for spotting a growth size that's too large for the workload. *Reliqua antequam
cresca alvei* — "remaining before the bucket grows" — tells you how close the
*current* bucket is to forcing a new one. *Apex usus* — peak usage — is the
high-water mark across the pool's entire life, and unlike the other three, it survives
a `piscina_vacare()` call, which makes it the right tool for answering "how big should
I have started this pool" after the fact.

There's a fifth function in the same family, `piscina_potesne_allocare()` — "can you
allocate" — which answers a yes-or-no question ahead of time instead of reporting
history:

```c
b32 cabit = piscina_potesne_allocare(p, mensura_necessaria);
```

For a dynamic pool this is always `VERUM`, since a dynamic pool can always grow. For a
fixed-size pool it checks whether the request fits in what's left — useful when you'd
rather branch on the answer than let `piscina_conari_allocare()` fail and handle it
after the fact.

## Patterns worth knowing

**Per-request pools.** A pool created at the start of handling one unit of work — a
web request, a single frame, one file — and destroyed at the end of it. This is
structurally identical to what Apache's `apr_pool_t` does per HTTP request; the
pattern predates this library by decades because the underlying shape of the problem
(bounded, predictable lifetime) hasn't changed.

**Parser scratch space.** Everything a recursive-descent parser allocates while
building an AST goes into one pool. If parsing succeeds, copy only the final result
somewhere permanent and destroy the scratch pool. If it fails partway through,
`piscina_reficere()` back to a mark taken before the attempt and try a different
production, without having to manually track and free every partial node the failed
attempt created.

**Frame-based allocation.** A pool sized for one frame of a running program — particle
effects, transient UI state, whatever doesn't need to outlive the frame it was
computed for — cleared with `piscina_vacare()` at the top of every new frame rather
than destroyed and rebuilt.

## Things that will bite you

**Using a pointer after `piscina_destruere()`, `piscina_vacare()`, or
`piscina_reficere()` past its mark.** All three invalidate previously-returned
pointers without changing the pointer's value — the address still looks valid, so the
bug tends to show up later, on unrelated data, rather than at the point of the mistake.

**Assuming allocated memory starts zeroed.** It doesn't. `piscina_allocare()` and its
variants hand back whatever bytes happened to be sitting at that offset — leftover
data from a previous `piscina_vacare()` cycle, or whatever the operating system left
behind on a fresh bucket. If your code depends on zero-initialized memory, ask for it
explicitly:

```c
vacuum* ptr = piscina_allocare(p, mensura);
memset(ptr, ZEPHYRUM, mensura);
```

**Expecting a fixed-size pool to grow.** It won't, on purpose. If you need growth,
create the pool with `piscina_generare_dynamicum()` instead — there's no function that
converts one kind of pool into the other after the fact.

## API Reference

### `piscina_generare_dynamicum()`

```c
Piscina*
piscina_generare_dynamicum (
        constans character* piscinae_titulum,
            memoriae_index  mensura_alvei_initia);
```

Creates a pool with one bucket of `mensura_alvei_initia` bytes. Later buckets are
sized at double that starting size unless a single allocation forces the baseline up.
`piscinae_titulum` may be `NIHIL`. Returns `NIHIL` if the initial allocation fails.

### `piscina_generare_certae_magnitudinis()`

```c
Piscina*
piscina_generare_certae_magnitudinis (
        constans character* piscinae_titulum,
        memoriae_index  mensura_buffer);
```

Creates a pool with exactly one bucket of `mensura_buffer` bytes, which never grows.
Returns `NIHIL` if the allocation fails.

### `piscina_destruere()`

```c
vacuum
piscina_destruere (
        Piscina* piscina);
```

Frees every bucket and the pool itself. Every pointer previously returned from this
pool becomes invalid. Safe to call with `NIHIL`.

### `piscina_allocare()`

```c
vacuum*
piscina_allocare (
                         Piscina* piscina,
        memoriae_index  mensura);
```

Allocates `mensura` bytes, unaligned. Calls `exire(I)` on failure — never returns
`NIHIL`. Allocating zero bytes returns `NIHIL` without failing.

### `piscina_allocare_ordinatum()`

```c
vacuum*
piscina_allocare_ordinatum (
                         Piscina* piscina,
        memoriae_index  mensura,
        memoriae_index  ordinatio);
```

Same as `piscina_allocare()`, aligned to `ordinatio` bytes (must be a power of two).

### `piscina_conari_allocare()`

```c
vacuum*
piscina_conari_allocare (
                         Piscina* piscina,
        memoriae_index  mensura);
```

Same as `piscina_allocare()`, but returns `NIHIL` instead of terminating the program on
failure.

### `piscina_conari_allocare_ordinatum()`

```c
vacuum*
piscina_conari_allocare_ordinatum (
                         Piscina* piscina,
        memoriae_index  mensura,
        memoriae_index  ordinatio);
```

Combines the two variants above: aligned, and `NIHIL`-on-failure rather than fatal.

### `piscina_vacare()`

```c
vacuum
piscina_vacare (
        Piscina* piscina);
```

Resets every bucket's offset to zero without freeing any of them. All previously
returned pointers become invalid. The buckets themselves remain allocated for reuse.

### `piscina_notare()`

```c
PiscinaNotatio
piscina_notare (
        Piscina* piscina);
```

Captures the pool's current position (which bucket, and the offset within it) as a
`PiscinaNotatio` value, to be passed to `piscina_reficere()` later.

### `piscina_reficere()`

```c
vacuum
piscina_reficere (
                      Piscina* piscina,
        PiscinaNotatio notatio);
```

Rewinds the pool to a previously captured `PiscinaNotatio`. Everything allocated after
the mark becomes invalid; everything allocated before it is untouched. Marks must be
reset in reverse order of when they were taken, the same discipline any stack requires.

### `piscina_potesne_allocare()`

```c
b32
piscina_potesne_allocare (
        constans Piscina* piscina,
          memoriae_index  mensura);
```

Reports whether an allocation of `mensura` bytes would currently succeed, without
performing it. Always `VERUM` for dynamic pools. For fixed-size pools, checks whether
`mensura` fits in the current bucket's remaining space.

### `piscina_summa_usus()`

```c
memoriae_index
piscina_summa_usus (
        constans Piscina* piscina);
```

Total bytes currently allocated, summed across every bucket in the pool.

### `piscina_summa_inutilis_allocatus()`

```c
memoriae_index
piscina_summa_inutilis_allocatus (
        constans Piscina* piscina);
```

Total allocated-but-unused capacity across every bucket — the gap between what each
bucket holds and what's actually been handed out from it.

### `piscina_reliqua_antequam_cresca_alvei()`

```c
memoriae_index
piscina_reliqua_antequam_cresca_alvei (
        constans Piscina* piscina);
```

Bytes remaining in the *current* bucket before the next allocation would force the
pool to move to a new one.

### `piscina_summa_apex_usus()`

```c
memoriae_index
piscina_summa_apex_usus (
        constans Piscina* piscina);
```

The highest value `piscina_summa_usus()` has ever returned for this pool. Unlike the
other query functions, this persists across `piscina_vacare()` calls.

## FAQ

**Is "arena allocator" the right term, or is this something else?** Region-based
memory management is the formal name in the literature; "arena," "pool," and "zone"
are the informal names different projects picked for the same idea. This library uses
*piscina* — pool — because that's the Latin the rest of the codebase is written in,
not because the underlying technique is unique to it.

**Why does allocation come in four functions instead of one with a couple of optional
parameters?** Because those four cover four genuinely different situations, and
threading "die on failure or hand me NIHIL" and "any alignment or a specific one"
through a single call site means every caller pays attention to options most of them
never touch. A parser reading a config file under 8KB wants the fatal, unaligned
version and nothing else — writing `si (!ptr)` at every call site when the answer is
always "then something is badly wrong, just stop" is ceremony, not safety. Code
allocating SIMD vectors genuinely cannot use the unaligned version. Four small
functions that each do one obvious thing cost less at every call site than one
flexible function threading two independent booleans through all of them would.

**Why can't you ask a pool how many buckets it currently has?** You can already ask
for total usage and total waste, both of which are computed by walking the same
internal bucket list a bucket-count function would walk. It's missing because nobody's
needed it yet, not because of a considered decision to leave it out — if a debugging
session ever needs to know the bucket count specifically rather than the aggregate
totals, that's a legitimate reason to add a fifth query function, and it would cost
about five lines.

**Can I free individual allocations?** No, and this isn't an oversight — it's the
entire point. If you need to free things individually, that's a signal you want
`malloc()`/`free()` for that particular piece of data, not an arena. Pools are for
allocations that share a lifetime; if some of your allocations need to outlive others
unpredictably, they don't actually share a lifetime, and forcing them into one pool
would be solving the wrong problem.

**Dynamic or fixed-size — which should I default to?** Dynamic, unless you have a
specific reason to want a hard failure at a known ceiling. Fixed-size pools exist for
the cases — config parsing, a known-bounded protocol message — where growing silently
past an expected size would hide a bug rather than handle a legitimate need.

**How big should the initial bucket be?** Start in the 4KB–64KB range and use
`piscina_summa_apex_usus()` after a representative run to see where you actually
landed. Too small and every bucket after the first gets sized from a baseline that's
still too conservative, so you end up with more buckets than the work really needed;
too large and you're holding memory you never touch. Either mistake costs you
something, but neither one is expensive to fix once you can see the real number.

**What's the actual performance difference from `malloc()`/`free()`?** Allocation is
pointer-bumping — O(1), no free-list search, no coalescing of adjacent freed blocks
because nothing is freed individually to begin with. Cleanup is also O(1): destroying
or clearing a pool doesn't walk and free each allocation, it discards or resets whole
buckets. The tradeoff is real, not free: you give up the ability to reclaim part of a
pool's memory before the whole pool's lifetime ends. For allocations that already
share a lifetime — which is most of what a parser, a request handler, or a game frame
actually allocates — that tradeoff costs nothing you were using anyway.

**Is this thread-safe?** No locking is built in. Give each thread its own pool rather
than sharing one across threads with a mutex around every call — that gets you actual
parallelism instead of serializing every allocation through a lock, and it avoids
needing to reason about which thread's mark/reset calls might interleave with another
thread's allocations.
