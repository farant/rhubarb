# excerptum — worklog

## 2026-09-17 — birth (materia-sedes B5, steps 1–2)

The picture half of the diagnostics arc. materia gives a **place**
(`MateriaTractus`: byte range, line, column); this gives it a **form** —
the compiler-style excerpt with a gutter, the source line, and a
`^~~~` marker underneath. It knows nothing about trees, so anything
with bytes and an offset can call it.

Three decisions worth keeping:

**The gutter preserves tabs.** Under the source line, a tab in the
prefix is re-emitted as a tab rather than expanded to spaces. That
keeps the marker in the true column whatever tab width the reader's
terminal uses — the alternative (guessing 4 or 8) is wrong on someone's
screen by construction.

**`~` counts CHARACTERS, not bytes.** Continuation bytes (`10xxxxxx`)
are skipped on both the prefix and the underline, so a three-byte `€`
gets one marker column, not three.

**Control bytes become spaces, CR before the newline is dropped.** The
excerpt is for reading; a raw control byte would corrupt the alignment
it exists to establish.

### The invisible plant

The test's UTF-8 case used `é` (`C3 A9`) and **could not detect a
broken continuation mask.** Flipping `(c & 0xC0) == 0x80` to `== 0xC0`
inverts which byte of a sequence counts as a character start — and for
a TWO-byte sequence the count is unchanged either way (one start
before, one after), so the output is byte-identical and the gate stayed
green.

A THREE-byte sequence separates them (one start vs two), so the fixture
now uses `€` (`E2 82 AC`) as well, in both the prefix position and the
underline position. The plant is red now.

Measured, which is the only reason this was found rather than assumed:

| sequence | correct mask | planted mask | |
|---|---|---|---|
| `C3 A9` (é, 2 bytes) | 1 start | 1 start | plant invisible |
| `E2 82 AC` (€, 3 bytes) | 1 start | 2 starts | plant visible |

**A green plant measures the TEST, not the code** — the third time that
rule has paid out in two days (crusta's `separator` rule with no case,
crusta's gravitas pin with no golden, and now a fixture too narrow to
discriminate). The shape here is the subtlest of the three: the fixture
looked like it covered UTF-8, and did cover UTF-8, but its particular
character was a fixed point of the fault.

### C trap met on the way

`"a\xE2\x82\xACb\n"` does not compile: **hex escapes are greedy**, so
`\xACb` is read as one escape and is out of range. Ended the escape
with adjacent-literal concatenation: `"a\xE2\x82\xAC" "b\n"`. Same
family as the trigraph that ate `"quid??)"` in crusta's B1 fixture — a
string literal is not the bytes you typed until you check.

Plants, all red, all green on revert: `'^'` → `'*'`; the tab branch
disabled; the CR-stripping disabled; the continuation mask flipped
(after the fixture was widened). A fifth — making `_continuatio`
return `FALSUM` outright — was REFUSED by `silva.planta` because it
would not compile (`-Wunused-parameter`), which is the tool working:
it ran nothing rather than reporting a red it had not earned.

## 2026-09-19 — fenestra: the excerpt had to learn about long lines

html tier-1 diagnostics landed and the printing was unusable. Not
inelegant — unusable: all 13 findings over the house's 260 pages sit
on lines of 453 to 20,030 bytes (median 1,049), so every excerpt
dumped the whole line and one of them would have thrown 20 KB at the
terminal. C and bash never exposed this because their lines are short
by convention; html and minified css have no such convention.

`_fenestra` narrows `principium`/`terminus` around the tract when the
line exceeds 160 bytes, marking elision with `...` on each side. Both
writers already walk from `principium`, so the caret follows for
free — the only arithmetic is the three columns `...` steals on the
left, added back in `_signum_scribere`.

Under the limit NOTHING changes, which is what keeps the older clients
safe: crusta's and css's output is byte-identical, verified by running
their suites and by eye on a real `fi exspectatum` two-sede excerpt.

**The first version was dead code, and only running it said so.**

I put the window in `excerptum_scribere`, gated it, watched 63/63 go
green, and then ran the actual tool: output unchanged, full line still
dumped. `materia_pictor` calls `excerptum_scribere_multa` and NOTHING
else — the single-sede entry I had just gated is not on the diagnostic
path at all. I had even written a comment in `_multa` explaining why
skipping the window there was acceptable ("gradus I html sedes relatas
nullas dat"), which was wrong on the only fact that mattered: the
painter always goes through `_multa`, one sede or twenty.

So: a capability that passed its own tests, shipped, and did nothing.
That is the third time this class has bitten in a week (EX8's
`regulae_plenae`, the facade's `CRUSTA_LINTRUM`, this). The gate was
not wrong — it tested what it said it tested. It just tested a
function the product does not call. **Ask which entry point the
PRODUCT uses before gating the one you happened to edit.**

The fix wanted the thing the old comment claimed was hard, and it was
not: `_multa` computes ONE window spanning every sede on the line
(first initium to max finis — sedes are sorted, so the minimum is
free) and hands the same window to the line and to every caret.

**Born red twice.** Neutering the three-column offset
(`elisa_ante && FALSUM`) fails 2 of 4 window cases — precisely the two
with left elision, while the near-start case stays green, which is the
evidence that the test discriminates rather than just noticing change.
Disabling the window outright fails 6. Both compile, so both are red
gates and not build breaks.

The window cases assert STRUCTURALLY, not against an expected string:
the source is `a` everywhere except one `X` at the tract, and the test
finds the `^` column and checks that the content line carries `X`
there. An expected string hand-computed by me would have repeated the
same arithmetic the code does, and a three-column error would have sat
in both halves agreeing with itself.

## 2026-09-28 — the caret measures columns (runae U6a)

The caret used to emit one space per UTF-8 *character* before the
tract and one `~` per character inside it. That was wrong in both
directions, which is why no correction factor could fix it (ledger
…VRTHANR, measured 2026-09-17): `広` holds two columns but counted one,
`e` + U+0301 holds one but counted two.

Now `_signum_scribere` walks **grapheme clusters** with
`runae_graphema_proximum` (UAX #29 + Ghostty's width rule, the
GRAPHEMATUM policy — diagnostics have no terminal handle, so no
environment policy; a ZWJ family on Terminal.app is the one place
this can disagree with what the user sees). Rules:

- before the tract: each cluster emits `latitudo` spaces, a tab stays
  a tab;
- `initium` inside a cluster (e.g. on a combining mark) puts the `^`
  under that cluster's base;
- `^` takes the first column of the tract, `~` covers the rest of its
  WIDTH (`広` alone = `^~`); an empty tract is `^` alone, as before;
- a control byte counts ONE column, because `_lineam_scribere` prints
  it as a space. runae says 0 (Cc) — without the special case in
  `_graphema` the caret drifts left. The old code got this right by
  accident (non-continuation byte = 1); a plant proved no test held it,
  so `a\x01b` is now a case.

ASCII is unchanged byte for byte (all 70 prior cases green). The 160
limit of the long-line window still counts BYTES, deliberately: a CJK
line of 160 bytes is ~106 columns, so the window arrives a little
late, but the caret stays right because both writers walk from the same
`principium`.

**Blast radius, measured.** excerptum is in the silva amalgam and in
five hand-kept probationes lists (toml, css, silva, materia, crusta)
plus `tools/diagnostica.sh` (the pre-commit hook's tool — found only when
the hook refused to link),
and ~22 silva launcher snippets derive from it. The first Editio
apply failed inside examen itself — examen *links* excerptum, so the
judge could not be built until the snippets were regenerated. Order
that worked: edit → regenerate every `# regeneratio:` snippet →
hand lists → `silva/amalgamare.sh` (builds objects, then stops at porta
0) → `amalgama_excludenda_generare.sh silva` → `amalgama_fontes_generare.sh
silva` → `amalgamare.sh` VERIFICATUM.

**Data leak the gate didn't see.** The amalgam makes claimed functions
static but not DATA: `RUNAE_GRADUS_PRIMUS/SECUNDUS` came out as global
`S` symbols. Silva's nm gate (step 5) only intersects against its own
six RADIX_FONTES objects, so it stayed green; saltuarius (which will
link lib/runae_tabulae.o next to silva.o in U6b) would have failed with
duplicate symbols. Fixed with exact renames to `SILVA_RUNAE_GRADUS_*`,
the same fix tessera needed at U5.

Plants (all compile), each caught by name: spaces per cluster instead
of per width (2), tildes per cluster (2), initium-inside-cluster
ignored (1), control width 0 (1, after adding the case).
