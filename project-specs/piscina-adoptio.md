# PISCINA ADOPTIO — quid arena in solario et knotapel mutaret

*Born 2026-09-04 (Fran's question: "they use malloc instead of
piscina, but do they still use an arena APPROACH with their own
implementation? and how would adopting an arena affect
performance?"). Cuts across the three same-day surveys —
nog-translatio.md, solarium-comparatio.md, knotapel-promotio.md —
rather than belonging to any one. Those documents each noted the
malloc/Piscina gap as a port cost; this one measures what is
actually behind it.*

*Discipline note: the numbers below are MEASURED, not estimated —
method in §IV, reproducible. The recommendations are reasoned FROM
those measurements and are marked as such. Where a claim is
speculation it says so.*

Status vocabulary: **LUCRUM** (arena is a clear win) ·
**NEUTRUM** (arena buys nothing; converting is a loss) ·
**DAMNUM** (arena could actively hurt; decide deliberately).

## I. Quaestio

Both trees call `malloc`. The question is whether they nonetheless
think in arenas by other means — and the answer is yes in both, by
two DIFFERENT mechanisms, neither of which is a bump allocator. That
distinction determines where Piscina helps and where it would be a
regression.

## II. Solarium — arena-adjacent by three mechanisms

**1. Fixed-capacity arrays embedded in structs.** 40 `*_CAP` /
`MAX_*` defines across the headers — `SKEL_MAX_JOINTS`,
`RHI_MAX_ATTRS`, `WTCACHE_CAP`, `TRACERY_MAX_FOILS`, `PAGE_SLUG_CAP`,
`REVERB_COMB_MAX`. Declare the ceiling, index into it, never
allocate.

**2. Grow-only cached scratch buffers** — the dominant hot-path
idiom. `vis_fill()`, `main.c:13138`:

```c
sol_u32 need = st->scene.next_handle;
if (need > st->vis_cap) {
    unsigned char *nv = (unsigned char *)realloc(st->vis, (size_t)need);
    if (nv == NULL) return NULL;
    st->vis = nv;  st->vis_cap = need;
}
memset(st->vis, 0, (size_t)need);
```

Reallocate only on growth, then memset and reuse. Same shape for
`st->bvh_ids` / `st->bvh_boxes` (`main.c:983`) and `st->active_map`
(`main.c:952`). Amortizes to zero allocations per frame.

**3. Matched manual free.** Free counts EXCEED alloc counts in most
modules — `syncstore` 4 allocs / 17 frees, `scene` 7 / 17, `skel`
0 / 5. Disciplined ownership, the opposite of arena leak-on-exit.

Measured (headless link sets lifted from build.sh):

```
sync_snap_test      74 allocations    14.5 KB   peak  1.4 MB
campus_test          0 allocations       0 B    peak  1.2 MB
texgen_test         15 allocations    15.7 MB   peak  8.2 MB
gothic_test        770 allocations   225.7 MB   peak 34.5 MB   (587 never freed)
```

The frame loop allocates nothing. **Geometry generation is the
exception** — `gothic_test` churns 225 MB against a 34.5 MB peak and
frees 183 of 770. Note this is the module ranked top TRANSFERENDUM
in solarium-comparatio.md §V.

## III. Knotapel — beyond arenas: no allocation at all

**113 of 133 demo files allocate nothing whatsoever.** They use
file-scope `static` arrays sized by `MAX_*` defines — `MAX_ACT 65536`,
`MAX_SUM_ANGLES 200000`, `catalog[MAX_CATALOG]`, `uf_p[MAX_UF]`,
`g_dir[MAX_DIR][3]`. One BSS reservation at program start, index into
it, reclaimed at process exit. Arena semantics with no arena.

The 20 files that do allocate: **78 allocs, 78 frees** — exactly
matched across the whole demo corpus.

**`raqiya` is the outlier** — and it is precisely what
knotapel-promotio.md proposes promoting. Every allocation is a bulk
working array sized by `n` up front, all function-scoped:
`parent`, `rank_arr`, `group_id`, `group_sizes`, `pair_keys`,
`hash_head` / `hash_next`, and `adj` as `calloc(n*n, 1)`.

```
probatio_scrutinium          213 allocations     5.7 KB              (2 leaked)
probatio_raqiya              349 allocations    19.3 KB              (2 leaked)
probatio_cyc8_scrutinium     708 allocations    64.5 KB
demo_108_parity_vocabulary 8,922 allocations     2.3 MB  real 3.21s
demo_107_algebraic_graph  74,685 allocations   266.8 MB  peak 13.4 MB  real 0.25s  (20 leaked)
```

**demo_107 is the headline: 266.8 MB allocated against 13.4 MB peak
— 20x churn, ~300k allocations/second.**

## IV. Methodus mensurae

Reproducible; no profiler required. An `-include` shim redefines the
four allocation entry points to counting wrappers, linked with a
counter TU and an `atexit` reporter:

```c
/* shim.h -- pass with -include */
extern unsigned long g_n_malloc, g_n_free, g_bytes;
static void *shim_malloc(size_t n){ g_n_malloc++; g_bytes+=n; return malloc(n); }
static void *shim_calloc(size_t a,size_t b){ g_n_malloc++; g_bytes+=a*b; return calloc(a,b); }
static void *shim_realloc(void*p,size_t n){ g_n_malloc++; g_bytes+=n; return realloc(p,n); }
static void  shim_free(void*p){ if(p) g_n_free++; free(p); }
#define malloc shim_malloc
#define calloc shim_calloc
#define realloc shim_realloc
#define free   shim_free
```

Peak RSS from `/usr/bin/time -l` (`maximum resident set size`).

**What this measures:** allocation CALLS and BYTES REQUESTED, plus
process peak RSS. **What it does NOT measure:** time spent inside the
allocator. See §IX.

## V. LUCRUM — where Piscina clearly wins

| Target | Evidence | Effect |
|---|---|---|
| `raqiya` / `scrutinium` | 20x churn, ~300k allocs/sec (demo_107) | Canonical arena workload. All allocations are function-scoped bulk arrays: `piscina_notare` at entry, `piscina_reficere` at exit. Deletes 53 `free()` calls in scrutinium.h + 30 in raqiya.h. Allocator time should approach zero. |
| Cache locality (same) | `scrutinium.h:131-133` allocates `group_id`, `group_sizes`, `pair_keys` back-to-back and then walks them together | malloc scatters these across size classes; a bump allocator lays them contiguous. Union-find is pointer-chasing over `parent[]` — the access pattern most sensitive to cache. **Speculative in magnitude**, but the direction is not in doubt. |
| Leak elimination | 2 + 2 + 20 + 587 leaks found while measuring (§II, §III) | Structurally unreachable under arena discipline. |

**Where it will NOT help even here:** demo_108 runs 3.21s on 8,922
allocations — compute-bound. Most knotapel demos resemble demo_108,
not demo_107.

## VI. NEUTRUM — converting would be a loss

- **The 113 no-allocation demos.** Static BSS arrays are already
  faster than any allocator, bump pointer included. Conversion trades
  zero-cost storage for a bump plus an arena parameter threaded
  through every signature. Pure loss; leave them.
- **Solarium's fixed-capacity structs and cached grow-only buffers.**
  `vis_fill` already amortizes to zero allocations per frame. Nothing
  beats zero.

## VII. DAMNUM — where it could actively hurt

**1. Grow-only cached buffers are hostile to arena semantics.**
`st->vis` deliberately persists ACROSS frames. Under an arena there
are two bad options: never reset that arena (malloc with worse
ergonomics), or reset per frame and re-allocate every frame — which
is strictly WORSE than the current amortized-zero. The correct answer
is a long-lived arena distinct from a per-frame scratch arena. That
is a design decision, not a mechanical port, and it is the main
reason solarium's engine is not an easy Piscina conversion.

**2. BSS is free; arena memory is committed.** `MAX_SUM_ANGLES 200000`
and the several `MAX_ACT 65536` arrays are demand-zero pages — they
cost nothing until touched, and an unused one never pages in. Moving
them into an arena commits the full reservation up front.

**3. Arenas cannot free individually, and scrutinium has n-squared
allocations.** `calloc(n*n, 1)` at `scrutinium.h:440` and `:599`
builds an adjacency matrix. Under malloc it returns the moment the
analysis ends; under an arena it stays resident until the next reset.
For large `n` this raises peak RSS materially.

**4. Peak RSS rises in general.** That is the trade: throughput and
lifetime-correctness bought with retained footprint. demo_107's 20x
churn ratio means the arena high-water lands somewhere between 13.4 MB
and 266.8 MB depending ENTIRELY on where reset points are placed —
which makes reset placement the design question, not an afterthought.

## VIII. Consilium

1. **Adopt Piscina in `raqiya` where it already allocates.** Free win
   on the one component with real churn; eliminates the leaks. This
   pairs with knotapel-promotio.md §IV — the promotion and the arena
   conversion are the same edit, so do them together rather than
   translating first and converting later.
2. **Leave the static-array demos alone.** They are already past what
   an arena offers.
3. **For solarium, convert only a module being ported anyway**, and
   decide the persistent-vs-scratch arena split deliberately rather
   than letting it fall out of the port. `gothic` is the one module
   where the arena case stands on its own (225 MB churn, 587 unfreed).

## IX. AUDIENDA

- **The shim counts calls and bytes, not allocator TIME.** Where
  demo_107's 0.25s actually goes is unprofiled — it may already be
  dominated by the n-squared memsets rather than by malloc. Take that
  measurement before committing to a conversion on throughput grounds.
- **Locality gains are asserted from access patterns, not measured.**
  §V's cache argument is reasoning, not data.
- **Solarium's frame loop was measured indirectly** — via headless
  tests, since the app needs a window. The claim "the frame loop
  allocates nothing" rests on reading `update()`/`render()` and on
  `vis_fill`'s grow-only guard, not on instrumenting a running frame.
- **`gothic_test`'s 587 unfreed allocations are a TEST-exit leak**,
  harmless in place. Whether `gothic.c` itself leaks in the app was
  not established.
