# fasciculum — spec v1 (macOS `.app` bundles: build, read, compare)

*Born 2026-09-11, the second of three libraries that came out of a
question about packaging a binary as a double-clickable `.app`
(**plist** → **fasciculum** → **icones**, decretum 01M29AKBXD). plist
shipped the same day (`project-specs/plist-spec.md`, plan 1 sealed at
`21de2be1`), which is what makes this library small: it composes plist
values and owns nothing about XML. Fran's decisions are marked DECISUS;
everything else is PROPOSITUM and was agreed in conversation unless
marked OPEN. Every "exists" claim cites the header or file:line it rests
on. English prose, Latin identifiers.*

*Discipline note: a bundle is a directory with a fixed shape. The
temptation in a library like this is to model every bundle Apple has
ever shipped; the PULL rule says otherwise — `.app` is what the local
tools need, and §10 holds the rest until a consumer asks.*

Status vocabulary: **DECISUS** (Fran decided) · **PROPOSITUM**
(proposed, agreed in conversation) · **OPEN** (undecided) ·
**PARCATUM** (deliberately deferred, §10) · **AUDIENDUM** (not
verified; §11).

## 1. Purpose and scope

A macOS application bundle is `Foo.app/Contents/` holding an
`Info.plist`, an executable under `MacOS/`, and optional resources.
This library describes such a bundle as a value, renders that value into
a list of files, writes the list to disk, reads an existing bundle back
into a value, and compares two values.

In scope: the `.app` layout, the generated `Info.plist`, copying the
executable and an icon, setting the executable bit, reading a bundle
back, value comparison, and a named refusal for everything else.

Out of scope: code signing and notarization (DECISUS D3 — a locally
built binary is already ad-hoc signed by clang, and a bundle directory
needs no signature for local use); Launch Services registration;
`.framework`, `.bundle`, `.appex`; icon *generation* (that is **icones**,
the third library). See §10.

Consumers: local tools — `forum`, `pictor`, `silex`, a `briar` script —
and later a `briar -app` target. No consumer is built by this spec; the
layer stands alone first (DECISUS D4).

## 2. Decisions

| # | Decision | Source |
|---|---|---|
| D1 | `.app` only for now; other bundle kinds graduate on PULL | DECISUS |
| D2 | Value + pure `reddere` + thin `scribere` + `legere` | DECISUS |
| D3 | No signing, no notarization, anywhere in this arc | DECISUS |
| D4 | Layer first, consumers later | DECISUS |
| D5 | A plan entry holds either BYTES or a SOURCE PATH | DECISUS |
| D6 | The writer does files and modes only — no Launch Services | DECISUS |
| D7 | `legere` refuses, naming the first problem; no lenient mode | DECISUS |
| D8 | `plista_extra` WINS over generated keys | PROPOSITUM |

## 3. The value and the plan

```c
nomen structura {
              chorda  identitas;   /* CFBundleIdentifier, necessarium */
              chorda  titulus;     /* CFBundleName */
              chorda  versio;      /* CFBundleShortVersionString */
              chorda  versio_aedificationis;  /* CFBundleVersion */
 constans character*  exsecutabile;/* via ad binarium, necessarium */
 constans character*  icon;        /* via ad .icns, aut NIHIL */
                 b32  sine_scandali;   /* LSUIElement */
         PlistValor*  plista_extra;    /* dictio, aut NIHIL */
} Fasciculum;

nomen enumeratio {
    FASCICULUM_GENITUM,
    FASCICULUM_COPIATUM
} FasciculumOrigo;

nomen structura {
     FasciculumOrigo  origo;
              chorda  semita;   /* relativa: "Contents/Info.plist" */
              chorda  octeti;   /* GENITUM */
 constans character*  fons;     /* COPIATUM */
                 b32  exsecutabile_fiat;
} FasciculumPars;
```

- **`CFBundleExecutable` is DERIVED** from `via_nomen(exsecutabile)`
  (`include/via.h:120`), never given separately. Two ways to say one
  thing is how a bundle ends up naming a binary it does not contain.
- **`plista_extra` is a plist dict** (`include/plist.h`), merged over the
  generated keys. Document types, Services declarations and anything
  else Apple invents arrive through it, so this library never grows a
  field per key. This is why plist had to exist first.
- **A plan entry is bytes OR a path** (D5). `reddere` reads no file
  CONTENTS, so it stays pure and the whole layout is unit-testable with
  no filesystem; a 20 MB binary never passes through the arena.

Surface:

```c
b32  fasciculum_reddere (constans Fasciculum*, Xar** partes,
                         FasciculumStatus*, chorda* sedes_vitii,
                         Piscina*);
b32  fasciculum_scribere (constans Xar* partes,
                          constans character* via_radicis,
                          FasciculumStatus*, chorda* sedes_vitii,
                          Piscina*);
b32  fasciculum_legere (constans character* via_radicis,
                        Fasciculum*, FasciculumStatus*,
                        chorda* sedes_vitii, Piscina*,
                        InternamentumChorda*);
b32  fasciculum_aequalis (constans Fasciculum*, constans Fasciculum*);
```

`partes` is an `Xar` of `FasciculumPars` (`include/xar.h`). Every call
that can fail takes a status and a `sedes_vitii` — the path or key the
refusal is about — because a bundle failure that does not name its file
is not actionable.

## 4. The generated `Info.plist`

`reddere` builds a `PlistValor` dict and serializes it with
`plist_scribere`; this library writes no XML.

| key | value | note |
|---|---|---|
| `CFBundleIdentifier` | `identitas` | refused if empty |
| `CFBundleExecutable` | `via_nomen(exsecutabile)` | derived |
| `CFBundleName` | `titulus`, else executable's name | |
| `CFBundlePackageType` | `APPL` | fixed for `.app` |
| `CFBundleInfoDictionaryVersion` | `6.0` | fixed |
| `CFBundleShortVersionString` | `versio`, else `1.0` | |
| `CFBundleVersion` | `versio_aedificationis`, else `1` | |
| `CFBundleIconFile` | icon's basename | only with an icon |
| `LSUIElement` | `<true/>` | only when set |
| `NSHighResolutionCapable` | `<true/>` | ALWAYS |

Three rules:

- **The merge is one-way and `plista_extra` WINS** (D8). A caller who
  sets `CFBundlePackageType` gets it. Generated keys winning would mean
  silently overriding a caller who knows more than the library, with no
  way to discover it from outside. The cost — a mistyped key is the
  caller's problem — sits in the honest place.
- **`NSHighResolutionCapable` is always emitted.** Without it a window
  can render scaled and blurry on a retina display: a defect that looks
  like a design choice.
- **Identifier and executable are refused, never defaulted.** A bundle
  with an invented identifier collides with whatever else guessed the
  same string.

`legere` is the inverse and deliberately narrower: read
`Contents/Info.plist` via `plist_legere_plagulam`, lift the ten keys
back into the value, keep every other key in `plista_extra`, and confirm
`Contents/MacOS/<CFBundleExecutable>` exists with `filum_status`
(`include/filum.h:215`). So `legere → reddere → scribere` round-trips a
bundle we wrote, and `fasciculum_aequalis` (over `plist_aequalis`)
answers "is this bundle already what I want?" without rewriting files —
which is the reason `plist_aequalis` is public API.

## 5. Writing

`scribere` is the only part that touches the world: per entry,
`filum_directorium_creare_si_necesse` for the parent
(`include/filum.h:174`), then `filum_scribere` for GENITUM
(`:120`) or `filum_copiare` for COPIATUM (`:161`), then the mode when
`exsecutabile_fiat` (§7).

- **`reddere` validates that sources EXIST** (`filum_status` on the
  executable and icon) though it reads no contents. "You pointed at a
  binary that is not there" is caught before anything is created.
- **A failed `scribere` leaves what it wrote, and says which path
  failed. No rollback** (PROPOSITUM, agreed). Deleting files on a failed
  build is how you lose the thing you were replacing, and a bundle is a
  directory a user may have open. This deliberately differs from
  `Editio.applicare`'s all-or-nothing, because a filesystem has no cheap
  snapshot and a half-written `.app` is inert rather than dangerous.

## 6. Refusals

```c
nomen enumeratio {
    FASCICULUM_SUCCESSUS = ZEPHYRUM,
    FASCICULUM_ERROR_DESUNT,          /* identitas aut exsecutabile */
    FASCICULUM_ERROR_EXSECUTABILE,    /* via non exsistit aut non filum */
    FASCICULUM_ERROR_ICON,            /* icon non exsistit */
    FASCICULUM_ERROR_PLISTA,          /* plist recusavit */
    FASCICULUM_ERROR_NON_FASCICULUM,  /* legere: Contents/ absens */
    FASCICULUM_ERROR_DIRECTORIUM,     /* scribere: directorium */
    FASCICULUM_ERROR_SCRIPTIO,        /* scribere: filum aut copia */
    FASCICULUM_ERROR_MODUS,           /* scribere: modus +x */
    FASCICULUM_ERROR_MEMORIA
} FasciculumStatus;
```

Every refusal sets `sedes_vitii`. `FASCICULUM_ERROR_PLISTA` carries
plist's own status and `semita` rather than flattening them — a bad key
three levels inside `plista_extra` must still name itself.

## 7. Home-library fixes (the D5 rule from plist's arc)

Both rows are in `filum`, both are forced by the executable bit, and
both are useful beyond bundles.

### 7.1 `filum_modum_ponere(via, modus)`

`filum_copiare` is a plain `fopen`/`fread`/`fwrite` loop
(`lib/filum.c:814-865`), so it does **not** preserve mode, and the only
chmod in the house is inside `filum_directorium_creare_cum_modo`
(`include/filum.h:190`), which handles directories. A copied binary
therefore lands non-executable, and a bundle whose binary is not `+x`
fails at double-click with no diagnosis — the worst kind of failure this
arc could ship.

### 7.2 `potest_exsequi` on `FilumStatus`

`FilumStatus` (`include/filum.h:205`) carries `mensura`,
`est_directorium`, `est_filum`, `potest_legere`, `potest_scribere` — and
no execute flag, so **H4 cannot assert the bit it just set**. Append the
field (`filum_status` fills every member explicitly, so appending is
safe — contrast the house rule about structs that are allocated without
`memset`).

## 8. Probationes and gates

`probationes/probatio_fasciculum.c`, wired with
`./tools/compile_tests_fontes_generare.sh` — run **after**
`lib/fasciculum.c` exists (the plist plan had this backwards and it cost
a confusing red; see `plist-plan-1.md` as-built).

| # | Gate |
|---|---|
| H1 | `reddere` is PURE: four entries, correct relative paths, `Contents/MacOS/<exe>` flagged `exsecutabile_fiat`, and nothing created — asserted by rendering against a root path that does not exist |
| H2 | The generated plist: ten keys, derived `CFBundleExecutable`, `plista_extra` overriding a generated key, `NSHighResolutionCapable` present |
| H3 | Each refusal in §6 by its own status AND its `sedes_vitii` |
| H4 | `scribere` into a scratch dir: directories created, plist written, executable copied and EXECUTABLE (§7.2), icon placed; the plist validated with `plutil -lint` |
| H5 | Round trip: `scribere` → `legere` → `fasciculum_aequalis`; and `legere` of a bundle whose executable was removed REFUSES |
| H6 | The half-written case ASSERTED: make `Contents/MacOS` unwritable, confirm the status names that path and the already-written `Info.plist` survives |

Every gate is born red by a planted fault. A plant must keep the code
compiling and every variable used (`silva.planta` refuses otherwise),
calibrations run SEQUENTIALLY, and reverting a plant DELETES the built
binary — all four learned the hard way in plist's arc.

**The oracle, and its honest limit.** plist had `plutil`, which is why
its G6 was worth building. For a bundle the automated options are
weaker: `plutil -lint` still judges the plist we generate (reused inside
H4, no new machinery), but "does macOS accept this bundle" cannot be
answered hermetically — `lsregister`/`mdls` mutate a user-level
database, and `open` is a GUI act. So: **`tools/fasciculum_fumus.sh`
builds a real bundle around a real binary and reports whether it
launches, run BY HAND, never by the suite** (the pattern of
`tools/briar_fumus.sh -agere`). The suite stays hermetic; the one
unavoidable platform question is asked where a human can watch it.

## 9. Phase plan (test-first; each phase ends green)

| Phase | Work |
|---|---|
| P1 | `filum`: `filum_modum_ponere` + `potest_exsequi` (§7) + tests in `probatio_filum.c` |
| P2 | Value, plan, `reddere` + H1, H2 (no filesystem at all) |
| P3 | `scribere` + H4, H6 |
| P4 | `legere`, `fasciculum_aequalis` + H3, H5 |
| P5 | `tools/fasciculum_fumus.sh` (by hand), worklog, spec as-built, whole suite |

## 10. Named deferrals (PARCATUM)

| Row | Pull |
|---|---|
| `.framework`, `.bundle`, `.appex` | A consumer needing one. Frameworks need version symlinks, hence a `filum` symlink addition. |
| Launch Services refresh | A workflow where Finder's staleness actually bites. |
| Signing / notarization | Distribution, which is a different project (D3). |
| `PkgInfo` | Modern macOS ignores it; add if a tool complains. |
| Document types as typed fields | They work today through `plista_extra`; type them only if the strings prove error-prone. |
| Icon GENERATION | **icones**, the third library. fasciculum only places a file. |

## 11. Risks, notes, AUDIENDA

- **AUDIENDUM:** whether a bundle assembled by this library actually
  launches on double-click. That is P5's by-hand fumus, and nothing in
  the automated suite can answer it.
- **AUDIENDUM:** whether `filum_copiare`'s 4 KB buffer loop is fast
  enough for a 20 MB binary in practice. Unmeasured; if it bites,
  the fix belongs in `filum`, not here.
- **AUDIENDUM:** whether an `.icns` this library copies is ever
  validated. It is not — fasciculum places bytes. A malformed icon
  yields a bundle with a blank icon and no error, and `icones` is where
  validation belongs.
- **Risk, low:** `plista_extra` winning means a caller can write a
  `CFBundleExecutable` that disagrees with the copied binary. H2 pins
  the merge direction; nothing stops a determined caller, by design.
- **Risk, named:** no rollback (§5). A failed write leaves a partial
  directory. Accepted deliberately.

## Cross-references

- `project-specs/plist-spec.md` + `plist-plan-1.md` — the layer below,
  and five as-built corrections worth reading before writing this plan.
- `include/filum.h`, `include/via.h`, `include/plist.h`, `include/xar.h`.
- `probationes/probatio_qr_gyrus.sh`, `tools/briar_fumus.sh` — the
  foreign-oracle and by-hand smoke patterns §8 borrows.
- Next in the arc: **icones** (`.icns` and `.iconset` from an image,
  with an area/box resampler landing in `imago_opus`).
