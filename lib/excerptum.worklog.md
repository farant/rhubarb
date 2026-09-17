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
