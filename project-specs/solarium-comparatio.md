# SOLARIUM COMPARATIO — quid inter solarium et rhubarb transire possit

*Born 2026-09-04 (Fran's question: "how compatible is solarium with
rhubarb? porting modules either direction?"). One of three surveys
taken the same day, with nog-translatio.md (the other external
predecessor) and knotapel-promotio.md (promotion from inside this
repo); piscina-adoptio.md measures the malloc/arena gap that III.2
below names as the deepest mismatch. solarium lives at ../solarium: a
3D engine with a server attached, last touched 2026-07-13 (MCP HTTP
transport). Like nog it is referenced nowhere in this repo.*

*Discipline note: a map of what COULD move, not a work queue. Rows
graduate on PULL (c89-lacunae.md house rule). The nog survey's
conclusion — treat the old tree as a spec repository — does NOT
apply here; see §II. This one is different, and the difference is
the whole finding.*

Status vocabulary: **TRANSFERENDUM** (solarium→rhubarb; unique
material rhubarb lacks) · **MUTUANDUM** (rhubarb→solarium; solarium
duplicates something rhubarb does far better) · **PROPRIUM** (stays
where it is — platform-bound or app-specific).

## I. Quid sit solarium

71 modules, flat layout, `.c`/`.h`/`_test.c` triples in one
directory. 41,122 lines of non-test C, 10,547 lines of test across
49 files, 73 headers. Normal build is `-std=c11`; vendors SQLite and
CivetWeb. GLFW + OpenGL, with a Metal twin behind an RHI seam.

`main.c` alone is **17,698 lines with zero tests** — 43% of the
non-test corpus. Everything worth moving lives in the modules around
it, not in it.

## II. Compatibilitas — mensurata

**This is not nog.** nog claimed C89 and 34 of its 56 sources would
not compile. solarium maintains C89 compatibility as an *enforced
invariant* and it holds.

`build.sh` carries a `c89check` mode. Its own comment:

> *Verify our code stays C89 / Dependable-C compatible. We don't
> BUILD in c89 mode (the normal build is c11) — we just check that
> the code COULD.*

It runs `-std=c89 -pedantic-errors -Werror -Wall -Wextra
-Wno-overlength-strings -fsyntax-only` over all 56 sources. Executed
2026-09-04:

```
c89check: PASS — all sources are C89-pedantic clean
twin-lint: PASS — all shader twins declare matching uniform names
```

Applying rhubarb's ADDITIONAL flags (`-Wconversion -Wsign-conversion
-Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings`)
across the 46 dependency-free modules — 17,134 lines — yields
**6 warnings, 0 errors**, complete:

```
glb.c:638        int -> sol_u32                   [-Wsign-conversion]
font.c:173       int -> size_t                    [-Wsign-conversion]
skel.c:21,30     int -> size_t                    [-Wsign-conversion]
nid.c:42         int -> unsigned int              [-Wsign-conversion]
sync_snap.c:162  const Scene* -> Scene*           [-Wcast-qual]
```

nog's candidates produced 134 `-Wsign-conversion` warnings alone.
Solarium's entire delta from house-strict is six lines.

Tests are healthy: built with `-std=c89 -pedantic-errors -Werror
-fsanitize=address,undefined`. Three run at random (`mapmath`,
`diskpath`, `inventory`) — all PASS.

**Consequence.** For nog the finding was "read it as a spec, the C
is a first draft." Here the C has already met a stricter compiler
than most C ever meets. Translation is a dialect problem, not a
rewrite.

## III. Tria obstacula

Code quality is not among them.

1. **Dialect.** `latina.h` keywords, Roman-numeral literals, Latin
   identifiers and comments, `<aedilis corpus=...>` directives, paired
   `.worklog.md`. Every line changes. Same tax as nog; unavoidable.

2. **Memory.** solarium is malloc/free plus fixed static buffers —
   **no arena anywhere**. `main.c` alone has 53 allocation sites.
   rhubarb threads `Piscina*` through every signature. Deepest
   mismatch, and it cuts BOTH directions: porting in means adding an
   arena parameter to every entry point; borrowing out means solarium
   adopting `Piscina` at whatever seam the borrowed library touches.

3. **Strings.** solarium is null-terminated `char*` (`stml_attr`
   returns `const char*`). rhubarb is `chorda` — length-carrying,
   explicitly NOT null-terminated. CLAUDE.md's standing warning about
   libc string functions on `chorda.datum` applies to every line of
   any port.

Narrower but real: solarium vendors SQLite `THREADSAFE=1` and links
`-lpthread`; `scrinium` vendors it `THREADSAFE=0` under the
single-writer discipline named as house law (scrinium.h). Any
convergence on storage must resolve that on purpose, not by accident
of whichever file gets compiled first.

## IV. Fines disiuncti

The two trees barely compete. rhubarb has **zero** OpenGL, GLFW,
shaders, vertices, or audio — measured by grep over all 159 headers
and 160 sources.

| Territory | src | tests | rhubarb equivalent |
|---|---|---|---|
| 3D engine / render | 6,361 | 1,738 | **none** |
| procedural geometry | 4,843 | 1,978 | **none** |
| editor / UI | 3,013 | 2,038 | partial (`widget`, `elementa`, `schirmata`) |
| server (`srv_*`) | 3,643 | 885 | partial (`http`, `hospitium`, `scrinium`) |
| scene | 1,338 | 1,240 | **none** |
| sync (client) | 1,282 | 408 | **none** |
| audio | 636 | 628 | **none** |
| auth / crypto | 345 | 241 | `sigillum`, `base64` only |
| app driver (`main.c`) | 17,698 | **0** | — |

So this is not the nog question ("which of these do we already
have?"). It is two mostly-disjoint corpora with a thin seam of
duplicated infrastructure.

## V. TRANSFERENDUM — solarium → rhubarb

Ordered by self-containedness, not priority. All PULL-gated.

| Module(s) | src | tests | Note |
|---|---|---|---|
| `sol_math` | 594 | — | vec3/vec4/mat4, perspective, look-at. rhubarb has no 3D math whatsoever. Zero deps, zero warnings. The obvious first port and the foundation everything else needs. |
| audio: `synth` `wav` `mixer` `reverb` `app_synth` | 636 | 628 | Near 1:1 test ratio, small, pure — only `platform_audio` touches AudioToolbox and would sit behind the same seam as `machina_posix`. |
| geometry: `gothic` `flora` `rock` `sweep` `furniture` `particles` | 4,843 | 1,978 | `gothic` alone is 3,608 lines. Given the causa-finalis in rhubarb.census, a procedural Gothic architecture generator is arguably more at home here than in solarium. |
| sync: `sync` `syncstore` `sync_snap` | 1,282 | 408 | Snapshot + diff over an event log. Conceptually adjacent to `volumen` and `gesta` — worth READING before extending either, whether or not it ports. |
| `sha256` `pkce` `srv_oauth` `srv_auth` | 345+ | 241 | `sigillum` already covers SHA-256, but rhubarb has no auth story at all. `pkce` is 21 lines against the RFC 7636 Appendix B vector. |

**PROPRIUM — leave.** `rhi_gl`, `mesh_gpu`, `metal_smoke` are the
GL/Metal backends; they would arrive in rhubarb with nothing to
attach to. `main.c` is the app, untested, and not a library.

## VI. MUTUANDUM — rhubarb → solarium

Where the trees do overlap, rhubarb's version is uniformly larger and
more capable:

```
SOLARIUM          LOC | RHUBARB                         LOC
stml              399 | stml+stml_html+stml_macros    17305
json              464 | json                           1968
text              239 | sectio+sententiae+paginatio    1997
platform_fs       129 | filum+iter_directoria+via      2087
caret             175 | cursor+selectio                1641
palette           351 | color+coloratio+thema          1572
font              228 | tabula_characterum             1102
srv_web           237 | http+hospitium                 3532
fuzzy              58 | quaerere                        644
b64                82 | base64                          255
sha256            193 | sigillum                        245
```

**STML is the standout.** solarium's parser is 399 lines with a
five-function API — `stml_parse`, `stml_free`, `stml_attr`,
`stml_child`, `stml_last_error`. rhubarb has 17,305 lines plus a
923-line header, plus `canon` validating five dialects, plus
`stml_macros`. solarium already stores scenes in STML (`NOTES.stml`,
`scene_io.c`) — it is running a toy parser against a format this
house has industrialized.

**The export mechanism already exists.**
`silex_clausuram_colligere` (silex.h) computes the transitive closure
of a library set by BFS over cited includes, using the aedilis
convention `include/X.h -> lib/X.c`; `tools/amalgama_*` emits that
closure as a vendorable amalgamation. Built for `silva`, `tessera`,
`officina` — solarium would be a fourth consumer, and the first
OUTSIDE this repo. That is a useful test of whether the closure
machinery actually generalizes.

**The cost is §III.2 and §III.3.** Every borrowed API wants a
`Piscina*` and speaks `chorda`. For a leaf like `quaerere` or
`sigillum` that is a thin adapter. For `stml`, whose output tree
threads through `scene_io` and `sync_snap`, it means solarium
adopting an arena at that seam — which is the real decision hiding
behind an apparently mechanical swap.

## VII. AUDIENDA

- **Full test suite never run** — three of 49 sampled. Aggregate pass
  rate unknown.
- **Implementations not read line by line** — headers, APIs,
  dependency graphs and compiler output only. Same limit as the nog
  survey; the roaring finding there (nog-translatio.md §VI) is the
  standing reminder that test volume is not evidence of coverage.
- **solarium is dormant** (last commit 2026-07-13) while rhubarb is
  active. Direction of travel favours rhubarb; a MUTUANDUM row is
  only worth paying for if solarium wakes up.
- **`c89check` covers syntax, not semantics** (`-fsyntax-only`).
  It proves the dialect is portable, not that the code is correct
  under a C89 compiler's actual codegen.
