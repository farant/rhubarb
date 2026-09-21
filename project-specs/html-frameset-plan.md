# html frameset tail — the nine cases of `plain-text-unsafe.dat`

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans
> (inline, per house convention — no subagents), ONE TASK PER TURN with
> Fran's approval.

*The last cluster of html5lib failures that shares a name. Born
2026-09-21, from a measurement pass that found the existing note's
breakdown wrong. Two of the three groups are ordinary bugs; the third
is a collision between an HTML5 rule and the O5 law, and needs a
decree rather than a patch.*

**Spec:** `project-specs/html-arbor-spec.md` §11 governs. The O7
annotation family (`synthesis`, `sedes`, `exemplar`, `praecedens`) is
the precedent for Task 3.

**Status:** NOT STARTED. Tasks 1–2 are mechanical; Task 3 is an open
question for Fran (§V).

---

## I. Live state, measured 2026-09-21

```
ORACULUM: 1510/1700 praeterita (pinna 1510); fragmenta 155/184
fixa/oraculum/recusata.tsv: 190 lines
plain-text-unsafe.dat: 9 failures
```

## II. The existing note is wrong, and how

`html/CLAUDE.md` ("The NUL tail") describes the remainder as **4
frameset-ok + ~2 foreign**. Measured, it is **three** groups of
**4 + 3 + 2**, and the last two are not frameset cases at all.

That note also got here honestly — its arithmetic (33 − 19 = 14)
matched exactly, which is what made it look settled. It is the same
trap it warns about one paragraph later: *a coincidence of totals is
not evidence.* Read the SET.

| # | data (␀ = NUL byte) | expected | we emit | group |
|---|---|---|---|---|
| 7 | `<html><select>␀` | `<select>` empty | extra `""` | A |
| 9 | `<body>␀` | `<body>` empty | extra `""` | A |
| 16 | `<svg>␀<frameset>` | `"�"` + `<svg frameset>` | neither | B |
| 17 | `<svg>␀ <frameset>` | `"� "` + `<svg frameset>` | neither | B |
| 18 | `<svg>␀a<frameset>` | `"�a"` + `<svg frameset>` | neither | B |
| 19 | `<svg>␀</svg><frameset>` | html/head/**frameset** | html/head/body/… | C |
| 20 | `<svg>␀ </svg><frameset>` | html/head/**frameset** | html/head/body/… | C |
| 22 | `<svg><path></path></svg><frameset>` | html/head/**frameset** | html/head/body/… | C |
| 23 | `<svg><p><frameset>` | html/head/**frameset** | html/head/body/… | C |

## III. Group A + half of B — one root: NUL-only text (5 cases)

The `nul` attribute (2026-09-19) preserves the byte correctly. What
was never decided is what the COOKED VIEW does with a text node that
is empty once NUL is removed. Measured projection of `<svg>␀<frameset>`:

```
<textus(> <tok(> <lex-textus nul="0"/>
```

Value empty, byte recorded. HTML5 has two different answers, and we
give neither:

| context | HTML5 | we do |
|---|---|---|
| "in body" / "in select" | **ignore** — no text node at all | emit `""` |
| foreign content | **replace** with U+FFFD | emit `""` |

One mode-dependent rule in `html_coctum` closes Group A (2) and the
text half of Group B (3).

**Note the existing precedent and its limit.** `_textus_albus_cum_nullo`
already routes a whitespace-and-NUL run to `elementum-malum` — but
only in the two WRAPPER modes (`ANTE_RADICEM`/`ANTE_CAPUT`). `<body>`
and `<select>` are not wrapper modes, so it does not reach them; and
in foreign content the answer is not "drop" but "replace", which that
mechanism cannot express.

## IV. Rest of Group B — `<frameset>` misclassified in foreign content (3)

The existing note says the node "vanishes entirely" and "the loss
happens elsewhere". **It does not vanish.** Measured:

```
<elementum-malum(>
    <lex-tag-apertura!><frameset</lex-tag-apertura>
```

It is classified `elementum-malum`, and the cooked view drops malum by
design. The bytes are intact — the byte law holds — the CLASSIFICATION
is wrong. Reproduced without any NUL (`<svg>a<frameset>` does it too),
so the frameset handler runs ahead of the foreign-content check.

`frameset` is correctly absent from `RUMPENTIA`, so inside SVG/MathML
it should be an ordinary foreign element (`<svg frameset>`).

*A guard on `proprium == HTML_ALIENUM_NULLUM` was tried earlier and
"changed nothing"; with the malum classification now located, that
attempt was probably guarding the wrong branch. Re-measure before
concluding.*

## V. Group C — O5 vs. HTML5 (4 cases). **A decree, not a patch.**

HTML5: when `<frameset>` arrives with frameset-ok still true, remove
the body from its parent, **discard everything in it**, insert the
frameset. The whole `<svg>…</svg>` subtree is thrown away.

`_corpus_fictum_removere` refuses:

```c
si (v->datum.nodus->genus != HTML_GENUS_ELEMENTUM_MALUM)
{
    redde VERUM;        /* a child with bytes — do not remove */
}
```

**This is the O5 law working, not failing.** "A node with bytes can
never be removed" is what makes the byte law hold: every byte lands in
the tree and the emitter reproduces the source exactly. The `<svg>` has
bytes; HTML5 wants no DOM node for them. Both are correct.

Same shape as O7b, which resolved "bytes here, DOM parent elsewhere"
with the `sedes` annotation. This one is **"bytes here, no DOM node at
all"** — a fourth member of the O7 family, e.g. an `abiectum` flag on
`elementum`, with the cooked view skipping the subtree.

**The cheap route is closed, and this week closed it.** The obvious
shortcut — re-genus the discarded subtree as `elementum-malum`, since
the cooked view already drops malum — now fails, because
`elementum-malum` carries a `diagnosticum` as of tier-1 diagnostics
(2026-09-19). A valid `<svg><path/></svg>` would start emitting a
false error. Anyone reaching for that shortcut should read this
paragraph first.

## VI. Tasks

### Task 1 — NUL-only text, by mode (5 cases: A + text half of B)

**Files:** `html/fontes/html_coctum.c` (and `html_arbor.c` if the mode
is not reachable from the cooked view), `html/probationes/probatio_html_oraculum.c`

Ignore in HTML content; replace with U+FFFD in foreign content. Gate:
the four cases move from REGRESSA to passing in `recusata.tsv`; pin
1510 → 1512 (A only) or 1515 if the foreign text half lands with it —
**measure, do not predict**. Born red by giving both contexts the same
answer.

### Task 2 — `<frameset>` is a foreign element inside SVG/MathML (3 cases)

**Files:** `html/fontes/html_arbor.c`

Reach the foreign-content check before the frameset handler classifies
malum. Gate: `<svg>a<frameset>` produces `<svg frameset>`, not a malum;
cases 16–18 pass. Born red by restoring the malum path.

### Task 3 — the `abiectum` annotation (4 cases) — **NEEDS FRAN'S DECREE**

**Files:** `html/grammatica/html.registrum.stml` (append + seal move),
`html/fontes/html_arbor.c`, `html_coctum.c`, canon, registrum probatio

Not to be started without the decision in §VII.

## VII. AUDIENDA — the question for Fran

Tasks 1 and 2 are ordinary bug fixes: no registry change, no seal
move, seven cases. They should happen whatever is decided about Task 3.

Task 3 is a **fourth declared annotation in the O7 family for four
html5lib cases**. The precedent is good (O7a/b/c each paid for
themselves) but the population is small, and a declared concept is
forever — it must be carried by the canon, the round trip, every
client's understanding of the registry, and the seal.

The honest framing: the question is not "can we fix four cases" but
**"is *the DOM discarded this subtree* a thing the tree should be able
to say?"** If yes, it is cheap and the shape is known. If no, the four
cases stay pinned with a named cause, which is an honourable outcome —
the reservation and the capsula rule were both declined on measurement
this month.

---

## VIII. Method notes (why this file exists)

- **Read the SET, never the total.** The note this file corrects was
  written from arithmetic that matched by coincidence.
- **"Vanishes" was wrong**; the node was one `grep` away in the STML
  projection. Before theorising about a lost node, print the tree.
- A `*.dat` case's data is BYTES — NUL included. Extracting cases
  through a shell silently eats them; read the file in Python.
