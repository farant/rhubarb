# aemulator - worklog

## 2026-10-06 — A0: the oracle before the code

**The text dump is Ghostty's `plainString`, exactly** (formatter.zig
plain emit, `unwrap=false`, `trim=false`): a never-written cell
(codepoint 0) becomes a space only if real text follows it on the row;
a WRITTEN space is text and is kept even at the end; spacer cells
(wide tail/head) are skipped; a row with no text accumulates and is
emitted as newlines only if a later row has text (trailing blank rows
vanish); rows joined by `\n`. "A\tB" -> `A       B` (tab moves, writes
nothing); erase-right of "Hello World" from col 6 -> `Hello`. Matching
it lets vectors copy Ghostty's expected strings verbatim.

**Conversion rules** used for Terminal.zig's API-call tests (written in
the vector file's header): print(c) -> c, setCursorPos(r,c) -> CSI r;c H
(0 acts as 1), cursorUp(n) -> CSI n A (B C D likewise), CR/LF/BS
literal (LF is LF only - no implied CR without LNM), eraseLine
.right/.left -> CSI K / CSI 1K, eraseDisplay .below/.above -> CSI J /
CSI 1J, setAttribute -> SGR. Tests that need origin mode, scroll
regions, reflow (resize) or mode 2027 were cut or trimmed to phase A;
the trimmed one says so.

**Erased cells keep the current background** (Ghostty "eraseLine right
preserves background sgr"): the vector asserts the erased cell's style
as canonical SGR `\x1B[0;48;2;255;0;0m` - in Ghostty it is a bg-only
cell content, in ours it will be the cell's style. Canonical SGR forms
confirmed against probatio_stilus_terminalis: `\033[0m`, palette
colours always `38;5;n` / `48;5;n`.

**STML attribute values keep backslashes literally** (only entities are
decoded - probed with `stml formare`), so `\x1B[` survives and the test
decodes it; `<`, `&`, `"` are kept out of vectors anyway.

**`casus` is the latina macro for `case`** - the C side says
`exemplum`; the STML element may keep the name `casus` (a string).

Measured: 172 of Terminal.zig's 428 tests check only externally
visible state (keyword heuristic; plan AUDIENDA).

## 2026-10-06 — A1: the skeleton (API approved unchanged)

**A test-harness false alarm worth remembering:** "resize within
capacity allocates nothing" failed at first - the CREDO assertions
interleaved between the measurement and the check write their records
into the SAME piscina (`credo_aperire(piscina)`). Measure, do the work
into plain variables, read the usage again, THEN assert. (Section IX's
steady-state loop had no CREDO inside and was green all along - I
first misread the failing line as IX.)

**Ghostty print rules reproduced** (Terminal.print, no 2027, no
margins): pending wrap is set whenever the write reaches the last
column (even with DECAWM off - the mode is consulted when the NEXT
print wraps); a wide char at the last column writes a spacer-head
there, marks the row soft-wrapped and continues at x=0 of the next
row; writing over a wide head clears its tail, writing over a tail
clears its head.

**UTF-8 across `aemulator_scribere` calls:** the tokenizer returns an
IMPRIMERE run up to the end of the buffer, so a rune can be split; up
to 3 trailing bytes are carried. A sequence (ESC...) arriving while a
rune is pending turns it into U+FFFD, like a terminal that sees a
broken byte stream.

**Accidental pass:** "cursorLeft no wrap" (CSI 10 D at column 1) is
green in A1 because ignoring the CSI and clamping at 0 both leave the
text "A\nB". A2 must keep it green for the right reason.

**Purity:** aemulator.c is pure (house headers + <string.h>); the link
closure is not - piscina.c and stilus_terminalis.c (via
chorda_aedificator) pull stdio/stdlib. Exact list for the wasm door.

## 2026-10-06 — A2: the loopback surface

All 24 A1 debts passed on the first A2 run - the debt mechanism then
FAILED the suite once per paid debt ("DEBITUM SOLUTUM - promove")
until the `debitum` attributes were removed. That is the point of it.

**Style table collection - a plant that survived:** "collection does
not renumber cells" passed the first test because compaction keeps
order: the only styled cell that outlived a collection sat at an index
nothing below it freed, so its number never changed. The test now
interns a style that dies (red, index 1) before one that stays on
screen (green, index 2) and forces a collection: green must move to 1,
and only real renumbering keeps that cell green.

**A plant that did not compile proves nothing:** P6 first failed with
"unused parameter" - recorded as SUPERSTES by the runner but it was a
build failure. Replanted in a compiling form (erase with the DEFAULT
style's background) and caught.

**Resize with two screens:** each screen keeps ITS cursor row visible;
the primary's cursor while you are on the alternate screen is where it
was at entry, so shrinking below it scrolls the primary's top rows off
(into scrollback in Ghostty; dropped here until phase C). My first test
expected the primary untouched with its cursor below the new height -
the core was right, the test was wrong.

**ESC 7/8 came with 1049:** the saved-cursor machinery 1049 needs IS
DECSC/DECRC, so they were implemented in A2 (planned for B) with
Ghostty's "cursor save and restore" vector.

## 2026-10-06 — A3: the loopback

The first run failed everywhere - in the HARNESS: tessera's front
buffer is indexed with a fixed stride (`TESSERA_LATITUDO_MAXIMA`, 512),
not the current width (tessera_opus.c `_index`). Reading
`frons[y * latitudo + x]` compared the wrong cells.

With that fixed, hand frames, 60 random frames and the whole vicus
session matched; the only remaining mismatch was a REAL tessera bug:
a full repaint (resize) cleared with `ESC[2J` under the previous
frame's pen - BCE painted every cell with the old background while
tessera skipped blank cells. Fixed in tessera (pen reset before the
clear) with its own regression test; see tessera/phase-log.md. This
is what features/009 promised: tessera's byte goldens could never see
it, a semantic comparison saw it at once.

Comparison rules: blank = no bytes or one space on both sides (tessera
writes spaces, never-written emulator cells are empty); colours PLENI
(CCLVI would quantize RGB in the bytes but not in tessera's buffer);
multi-codepoint graphemes excluded (emulator v2). Every session must
leave `aemulator_ignota` at 0 - tessera emits nothing the emulator
does not understand.

C1 question closed: same behaviour as Ghostty (raw C1 byte -> U+FFFD,
UTF-8-encoded C1 -> ignored).

Plant P8 ("harness skips the style comparison") survived by
construction - removing a check can only show when the compared
things differ; P2 (SGR ignored) is the plant that proves the check
works.

## 2026-10-06 — B1: the shell surface

80 Ghostty tests converted for regions, IND/RI/NEL/SU/SD, IL/DL,
ICH/DCH/ECH, tabs and LNM (conversion rules added to the vector file's
header), plus 4 house cases (NEL, TBC/HTS, CHT/CBT). Read Ghostty's
implementations FIRST (Terminal.zig setTopAndBottomMargin, index,
reverseIndex, cursorUp/Down, scrollUp/Down, insertLines, deleteLines,
insertBlanks, deleteChars, eraseChars, horizontalTab(Back),
tabClear/Set) - the edge rules are in the code, not the test names:
- index outside the region moves down unless on the screen's last row;
  at the region's bottom it scrolls ONLY the region;
- cursorUp/Down stop at the margins only when starting inside them;
- IL/DL do nothing with the cursor outside the region, clear the moved
  rows' soft-wrap, and put the cursor in column 0;
- ICH clears a wide char that would be split: under the cursor (tail),
  at the right edge, and at the end of the shifted run;
- DCH/ECH "split cell boundaries" first (a tail at the boundary clears
  its whole wide char).
- Ghostty `printString` maps '\n' to CR+LF.

**A redundant line found by a plant:** ECH's "extend by one if the last
erased cell is a wide head" survived its plant - the boundary split at
x+n clears exactly the same cells. Removed, with the reason in the
comment. (Ghostty has both.)

**An invisible rule:** the soft-wrap resets in DCH/ECH/IL/DL cannot be
observed through the API until reflow or an unwrapped dump exists;
their plant survives by construction. Name it when reflow lands.

Resize now scrolls the shrinking screen with an explicit full-screen
`regionem_sursum` (the region-aware index would have obeyed a stale
region).

**A glossary entry is not free (B1 commit, oratio gate red):** adding
`imus` (forms ima/imae/imam/imum) to `oratio/glossarium.stml` for the
identifier `regio_ima` dropped oratio's parsing accuracy below its
pins in `probatio_oratio_oraculum` (overall attachment and subject
relations). The glossary is oratio's LEXICON, not just the lint's word
list - a partial entry shadowed a richer dictionary analysis of corpus
sentences. Verified both ways (without the entry: green; with: red).
Fix: no entry; the identifier became `regio_ultima` (a word the lint
already knows). Rule: after any glossary edit, run the oratio suite
(or owe the oratio gate) - a new word can change parses.

## 2026-10-06 — B2: answers and effects

**A hand-counted length lied on the first vector run.** DA1 was sent
with length X for `"\033[?62;22c"` (9 bytes) - the reply carried a
trailing NUL. The unit tests had no reply assertions yet; the first
Ghostty DA1 vector caught it. Every fixed reply now goes through
`literas_respondere` (strlen); no reply length is counted by hand.

**Over-long OSC: drop, never truncate.** `series_terminalis` caps a
string body at `SERIES_CHORDA_MAXIMA` (2048) and sets `truncatum`.
`seriem_osc` ignored the flag, so a 3000-byte title would have been
delivered cut short. Ghostty's osc.zig puts title capture in a fixed
2048 buffer; overflow sets state invalid and `end` returns no command
(change_window_title.zig:36). Now: truncated body -> whole OSC dropped,
counted as unknown. Named divergence: Ghostty also needs a NUL byte in
that buffer, so it accepts titles up to 2047 bytes; our 2048 includes
the `2;` prefix, so we accept up to 2046. Not worth a lexer change.

**Testing "no reply" without empty attributes.** The STML pretty
writer turns `a=""` into `"true"`, so a vector cannot say
`responsum=""`. Instead the replayer drains replies every step, and a
case that must NOT reply puts a DSR 5 in the same step and expects
exactly `ESC[0n` (OSC 1, DA1 with a parameter).

**Replayer shape:** the real adapter wraps the core in a `Verus`
struct that captures replies (512 bytes) and titles (count + last 256
bytes) through the effects; `responsa` drains. Two harness plants
(no drain; no comparison) are caught by the vectors.

**Unit-test changes:** III and X used `ESC[99n`-style probes for
"unknown"; DSR 5 and DA2 are now real answers, so the probes became
`ESC[99n` (DSR other) and `ESC[>5c` (DA2 with a parameter).

## 2026-10-06 — B4b: esctest

**esctest reads cells only through DECRQCRA.** 322 of its assertions
are AssertScreenCharsInRectEqual -> DECRQCRA one cell at a time (VT
level 4). Under `--expected-terminal=xterm`, `--xterm-checksum=334`
makes empty() a space and leaves the raw 16-bit sum un-negated, which
matches our core (no "never written" vs "erased" distinction, as
Ghostty). Reply `DCS Pid ! ~ XXXX ST`; Pid read raw (0 is legal -
`parametrum` maps 0 to the default).

**esctest's reset() needs `CSI 18 t`** (GetScreenSize) before every
test - without it all 6 CUP tests failed inside reset. And DECSTR:
without it a test's scroll region leaks into the next (the plant
proved it: CPL/CUD tests broke).

**DECSTR is xterm's, not DEC's.** DEC STD 070 turns autowrap off; xterm
keeps it on and esctest marks that an intentional deviation. Ghostty
has no DECSTR (only RIS), so esctest's decstr.py is the reference.

**Attributing 306 failures.** `--test-case-dir` writes each test's
bytes (body only - reset and query reads are not in it). A feature
pattern list (first match wins) named 288; class names covered the
XTWINOPS tests whose bytes go through another path; DA/DA2 are our
identity (esctest's xterm profile wants DA1 64;1;2;6;9;... and DA2
41). A failure attributed to a missing feature may hide a second
cause - when that feature lands, the pinned table forces a look.

**C hex escapes are greedy.** "\x1B7" is 0x1B7 - split the literal
("\x1B" "7"). The STML vector escapes are fixed two-digit, so only C
tests bite.

**P4 (no 16-bit mask) survives by construction:** the reply prints
only four hex digits. Mask kept for intent.

## 2026-10-06 — C2: history

**The scroll-off rule is "region at the top", not "full screen".**
Ghostty `Terminal: index bottom of scroll region creates scrollback`
(Terminal.zig:10651): region 1-3 of 5 rows, IND at row 3 pushes row 1
into history while row 4 stays. Same in `scrollUp` ("if our scroll
region is at the top and we have no left/right margins"). DL at the
top row scrolls the same rows but must NOT feed history, so
`regionem_sursum` takes an explicit `historia` flag (IND, SU, resize:
VERUM; DL: FALSUM) and feeds only when summa == 0 on the primary.

**ED 2 keeps nothing.** Ghostty scrolls the screen into history on ED
2 only when the last non-empty row is an OSC 133 prompt (#905). No
prompt marks here -> plain clear.

**Pages own their styles.** Rows are copied into a page and their
style indices re-interned into the page's 128-entry table, so the
screen's mark-compact collection (which renumbers) never walks
history. Table full mid-row: roll back the row's new entries
(numerus_stilorum = n0), take a fresh page, retry once; a row that
alone needs > 127 styles degrades the rest to the default style.

**Steady-state tests had to learn about history.** With history on by
default, 1000 lines legitimately allocate pages until the limit. The
"allocates nothing" tests now use a one-page limit (historia_octeti =
1) and a warm-up that pushes a line into history; the full-limit
measurement is C3's.

**Plants that survived, and why:** width change without a new page
(the wide row's text was never checked in history); reading past a
narrow row (landed on an EMPTY neighbour - rows are now full of 'a');
alt screen hiding history (the Ghostty vector's primary had none);
soft-wrap flag (nothing read it); entering alt keeps the view (no
check). P7 first did not compile (unused variable) - replanted.

**1049 carries the cursor:** a vector expecting "A" at the top of the
alt screen was wrong - the cursor arrives where it was on the primary.

## 2026-10-06 — C3: the view

**Sign convention.** Ghostty `scroll(.{ .delta_row = -1 })` moves UP;
our `visum_movere(+1)` moves up (the view counts lines above the live
screen). Converted vectors flip the sign.

**Eviction granularity shows in the clamp.** With one page, evicting
it removes ALL history, so a view parked at the top goes back to the
bottom; with two or more, it lands on the oldest kept line and stays.
Both are "clamp to the oldest remaining line" - the first just has
none left.

**The host's snap needs "accepted".** A paste refused by a full queue
did not reach the program, so it must not move the view (plant P6).
The repaint flag was named for resizes (`amplitudo_mutata`) and now
also means "the view moved" -> renamed `repingendum` (renominare
refuses dirty files; its plan listed 6 member uses, replaced by exact
word).

## 2026-10-06 — C4: ED 3 and resize

**Ghostty's rows rules (PageList.zig ~2860):** shrink = trimTrailing
BlankRows first ("matches macOS Terminal.app"), the remainder becomes
history; grow = pull history only if `cursor.y >= rows - 1` (else
blank rows, cursor stays: "we don't want to pull down scrollback").
Our old shrink cut the bottom whenever the cursor stayed visible -
text below the cursor was lost. New vector: "shrinking keeps text
below the cursor". Blank = no text; the trim never takes the cursor's
own row (vector: "...never trims the cursor's own blank row").

**Pull after the new size.** Re-interning a pulled row's styles can
trigger the screen's style collection, which walks rows x columns at
the CURRENT size. Pulling inside `schirmum_aptare` (old size) would
leave cells beyond the old width un-renumbered. So the pull count is
decided before (old cursor, old rows) and done after the size is set.

**Index coincidence hides a missing re-intern.** Page and screen
tables interned red in the same order -> same index, so copying page
indices "worked". The test now interns (and erases) a yellow first,
and compares the pulled cell with a live SGR 31 cell.

**ED 3 keeps the pages.** All pages emptied, ring unchanged; the next
line reuses the newest (re-prepared if its width differs). One-page
fill -> ED 3 -> refill allocates nothing (plant: dropping the ring
re-allocates - caught).


**Found at the C4 commit: two tests leaked temporary volumes.**
`volumen_temporarium` tries /tmp/<prefix>-1..100.volumen and returns
NIHIL when all 100 exist. `probatio_ludus_tessera_pictor` (lt_pictor_f,
lt_pictor_t - since 2026-10-03) and my own loopback
`probatio_ludus_tessera_reditus` (lt_reditus, A3) never closed their
volumes, so every run left one behind; today's runs hit 100 for the
pictor ones and the ludus_tessera gate went red on code that had not
changed. Fix: `volumen_claudere` at the end of both tests; the 690
leftover files (with -shm/-wal) deleted. Two runs after: zero left.
Lesson: a test that opens a temporary resource and passes is not done
until it leaves nothing behind - volumen.h even says the leftover is
the signal; nobody looked in /tmp.

## 2026-10-07 — D1: quick wins

**Empty expectations were never checked.** P7 (1047 does not clear on
exit) and P12 (RIS keeps the previous char) survived although their
vectors expect `textus=""`. A direct trace showed the core misbehaving
under the plant, so the replayer was skipping the check. The house
STML interns an empty attribute value as NIHIL, and
`stml_attributum_habet` is `capere != NIHIL` - an empty value is
indistinguishable from an absent attribute. Every empty expectation in
the vectors (9, since phase A) was a silent no-op. Fix: the explicit
marker `"\0"` (the whole value) = empty string, in the reader and the
validator; all nine now run and hold. No other house fixture uses
empty attributes (git grep).

**REP is capped** at twice the screen area: a saturated parameter
(0x7FFFFFFF) would loop for ages; beyond two screens only identical
lines scroll. Named divergence from Ghostty (which loops the count).

**The unknown-sequence probe moved:** section III used `ESC # 8` as an
unknown sequence; DECALN is now real, so the probe is `ESC # 3`.

**Greedy hex, twice more:** `"\x1Bc"` is 0x1BC - split literals.

## 2026-10-07 — D2: modes

**Ghostty origin-mode bug (we diverge).** Ghostty's stream maps CHA,
HPA, HPR and VPR to `setCursorPos(cursor.y + 1, ...)`; under DECOM
setCursorPos adds the region top again, so CHA would move the cursor
DOWN by the top margin. xterm's CursorRow is origin-relative, and
esctest `CHA_RespectsOriginMode` expects the row kept (its X lands at
the region's top-left). We follow xterm: only CUP/HVP and VPA are
origin-relative; VPR is absolute + n, clamped to the region bottom
under DECOM.

**Reverse wrap 1045 cycles.** Once the extended mode wraps from the
region top to its bottom, positions repeat every rows x width steps;
the count is reduced modulo that (exact - unit test compares n and
n + 1000 cycles). Ghostty counts down one by one: a saturated CUB on a
one-column screen is ~2^31 iterations.

**esctest reverse wrap.** esctest's `ReverseWraparound()` returns 45
unless `--xterm-reverse-wrap >= 383`, then 1045; with the default it
expects the OLD xterm meaning of 45 (wrap past the top). The runner now
passes 383 - modern xterm and Ghostty semantics.

**DECRQM is now our mode oracle.** Vectors assert modes through
replies (`CSI ? n $ p` -> `CSI ? n ; s $ y`), not private accessors.
Modes outside the table answer 0 - including ones Ghostty stores but
does not act on (5, 12, 1007); xterm answers 4 for permanently reset
ANSI modes (GATM, SRTM ...), 23 esctest rows carry that cause.

**False pass exposed:** `DECSCL_Level2DoesntSupportDECRQM` passed only
because DECRQM had no answer.

**Equivalent plant:** inserting at the last column (`<=` for `<` in the
IRM guard) is indistinguishable - the blank is overwritten at once.
Ghostty's guard is a shortcut, not semantics.

**Probe moved again:** section X used `CSI ? 1 $ p` as an unknown
intermediate sequence; now DECRQPSR `CSI 1 $ w`.

**Count correction:** D1's docs say 184 vectors (+11); the file at
8e8859d0 has 191 (+12). Counted with `grep -c "<casus "` this time;
the replayer agrees (220 viridia after D2).

ICH (`cellulas_inserere`) moved above the print path - IRM calls it
(the file has no forward declarations).

## 2026-10-07 — D3: charsets

**Width before mapping.** Ghostty computes the width from the unmapped
rune and maps in printCell, so under DEC graphics an emoji becomes a
WIDE cell holding a space (vector "print charset outside of ASCII").
Mapping first would make it narrow - plant Q2 checks the order.

**Single shift and REP.** The shift is consumed by the next printed
cell only (zero-width runes return before mapping). REP keeps the
UNMAPPED previous rune and maps it again under the current set
(Ghostty test 14421: `q` repeats as `─` under DEC, then `q` under ASCII).

**SS lexeme.** Our lexer delivers `ESC N x` as one SS lexeme; the core
sets the shift and prints the printable bytes after the introducer
(normally just `x`). `ESC N` + UTF-8 used to lose the rune in the lexer
(see lib/series_terminalis.worklog.md); now it arrives as FUGA with
introducer N, which the core treats the same way.

**DECSTR resets charsets** (VT510 table, xterm) - Ghostty has no DECSTR.

**Weak vector found by plant Q15:** LS2 landing in G3 survived while G2
and G3 held the same set; G3 now differs in that step.

## 2026-10-07 — D4: kitty keyboard flags

**Ring, not stack.** Ghostty's FlagStack is 8 slots with a wrapping
index: a ninth push overwrites the oldest, and popping below the base
wraps too (slots are zeroed as they are popped, so it reads 0). We copy
that exactly; the ring vector pushes 9 and pops back through it.

**Vectors that test the slot, not the index.** RIS resets the index to
0; a vector that PUSHED before RIS left its value in slot 1, so a plant
that forgot to zero the slots still read 0. Using `CSI = n u` (writes
the current slot) makes the zeroing visible. Same lesson as D3's Q15:
a vector must make the broken and the correct paths diverge.

**Equivalent plant:** removing the `pop >= 8 -> clear` shortcut gives
the same result (popping 2^31 one by one also ends empty) in ~1 s at
our optimisation level - it is a hostile-input guard, kept, untestable
by output.

## 2026-10-07 — D5: colours

**`#` forms are not scaled (X11), `rgb:` forms are.** XParseColor:
"#RGB ... fewer than 16 bits ... represent the most significant bits
(unlike rgb:, in which values are scaled)". Ghostty scales both, so
`#f00f00f00` is ef there and f0 in xterm/esctest. With MSB semantics
and 8-bit storage all three esctest Hash tests pass. Bare `abc` /
`123456` is Ghostty's own extension - kept scaled, as Ghostty does.

**Cursor colour without its own value follows the CURRENT foreground**
(Ghostty test 3008: set 10, set 12, reset 112, query 12 -> the set
foreground). Expressed with AEMULATOR_COLOR_NULLUS in the configuration
and the live slot.

**Request lists follow Ghostty's tokenizer:** empty fields skipped; a
bad spec ends the whole list; OSC 4 indices 256-260 are "special"
(no-op, list continues), beyond that the list ends; OSC 104 with no
valid index resets the whole table; 110-112 with any parameter do
nothing.

**ChangeSpecialColor_* pass honestly:** esctest asks XTGETTCAP `Co` for
the palette size, we do not answer, it assumes 16 and its "special"
colours land on palette 16/17 - real round trips. ResetSpecialColor_*
fail: the reset goes through OSC 105, which (like Ghostty) we ignore;
their captured case files hold only the first command, so the cause is
attributed by test name.

**Equivalent plant:** `rgb:` accepting two parts - the third channel
then has zero digits and fails anyway (the guard is redundant).

**Lint:** `colores_xterm` -> `colores_ordinarii` (xterm is not a house
word; renominare refuses dirty files, exact replace used).


## 2026-10-07 — D7a: quick replies

Fran chose to close only the small answers before the D7 session.
- DECXCPR (`CSI ? 6 n`) answers `CSI ? y ; x R` WITHOUT the page field:
  esctest infers our VT level from DA2 (`>1` -> level 2) and expects the
  VT220 form; consistent with the identity we claim.
- DECDSR hardware status: fixed answers (no printer ?13n, UDK unlocked
  ?20n, keyboard ?27;1n - two parameters at level 2 - no locator ?50n,
  locator type ?57;0n). VT420 queries stay unknown.
- DECID (ESC Z) = DA1.
- DECRQSS (DCS $ q Pt ST): SGR (own fixed-buffer encoder - the house
  stilus_codificare needs a ChordaAedificator, and the core promises no
  allocation in steady state), DECSTBM, DECSCUSR (now stored; default
  reported as 2 = steady block, what terminale draws). Anything else
  `DCS 0 $ r ST`.
- Title stack CSI 22/23 ; 0|2 t: the core now remembers the current
  title and keeps xterm's 10-deep stack; a pop re-announces through the
  titulus effect. Ghostty parses these and does nothing. esctest's eight
  title-stack rows stay red: they read the title back with 21t, which
  we refuse on purpose (title echo = input injection).
- Plant R10 (icon-only push not ignored) survived at first: the next
  check rejected mode 1 anyway and only the unknown counter moved - the
  vector now asserts `ignota="0"`.
esctest 284 -> 294. Parked by Fran: protection/selective erase (24),
DECCOLM (2), XTSAVE/XTRESTORE (2).


## 2026-10-07 — D7c: vttest driver and minimal DECCOLM

**Driver.** `tools/aemulator_vttest.c` runs vttest as the child of a
headless `TerminaleApplicatio` (80x24, 6x8 cells). A script of
`mitte <bytes>` lines (escapes \r \n \e \\ \xHH) is fed one line at a
time; after each, the driver pulses until output is quiet (8 x 50 ms,
10 s cap) and saves `NNN.txt` (screen text) + `NNN.png` (the frame
terminale would draw). Build: `./bin/aedilis tools/aemulator_vttest.c
&& bash build/aedilis/aemulator_vttest/struere.sh`. Run vttest with
geometry `24x80.80` or it also walks 132-column passes we never draw.
Limits: bytes go straight to the pty, NOT through the encoder - so
vttest's LNM test reports "<13> Not expected" (the encoder's LNM path
is covered by the codificator tests); and timed tests (RIS sleeps 5 s)
need more returns than a quiet wait gives.

**DECCOLM.** vttest's autowrap screen relies on DECCOLM clearing the
screen even at 80 columns (it writes, switches, expects a blank page).
Ghostty's deccolm: ignored unless ?40; otherwise clear, reset margins,
home - and resize. We do everything but the resize: the host owns the
window. Without ?40 the ?3 bit is put back to 0 so DECRQM keeps saying
"reset". Four plants (permission ignored, no clear, region kept, bit
left set) all caught. esctest 294 -> 295; Allow80To132 and
RIS_ResetDECCOLM need the width to change -> CONSULTO.

**Walk so far** (menus 1, 2, 3, 4, 6, 7, 8, 10, 11.5, 11.6, 11.7):
correct: cursor movement, autowrap, tabs, origin mode, scroll regions
(soft/jump read alike), save/restore cursor, DEC graphics, British set,
SI/SO, single shifts, VT102 insert/delete (both passes), DSR/DA1/DA2/
DA3, ECMA-48 HPA/CBT/CHA/CHT/HPR/VPA/CNL/CPL/VPR/SD/SU, the 8x8 colour
matrix (bold brightens 0-7), SGR 0, BCE on ED/EL/ECH/IL/DL.
Findings (verdicts pending with Fran):
- terminale draws NO underline, strike, overline or italic, and bold
  only as a brighter palette colour - the core stores all of them
  (rendition screen: "underline" looks like "vanilla").
- DECSCNM (?5 reverse screen) not honoured: light/dark screens
  identical. Ghostty has it.
- blink not rendered (Ghostty blinks).
- DECDWL/DECDHL double-size lines ignored (Ghostty too).
- VT52 mode (?2) absent (Ghostty too).
- SL/SR (CSI SP @ / SP A) absent (Ghostty too).
- REP after an intervening control sequence still repeats (12 '+'
  where vttest hopes for 2) - same as Ghostty; ECMA-48 leaves it
  undefined.
- Black-on-black text visible: the contrast floor, on purpose.
- 8-bit GR bytes (locking-shift GR rows) show U+FFFD: we are a UTF-8
  terminal, like Ghostty.

**DECSCNM (?5), core half (Fran approved the AemulatorModi field).**
A bit only: `aemulator_modi(a).schirmus_inversus`; cells are never
touched. Ghostty (render.zig:633) swaps just the DEFAULT fg/bg in the
renderer - explicit colours and the cursor stay - and terminale will do
the same. RIS clears it; DECSTR keeps it (VT510's DECSTR table has no
screen mode; it resets from an explicit list, which ?5 is not on). Two
old vectors used ?5 as their "unknown mode" example - moved to ?8
(DECARM), which we do not honour. Plants: default set, snapshot not
filled, DECSTR resetting it, wrong mode number - all caught. esctest
295 -> 296 (DECRQM DECSCNM).

**vttest walk - closed (driven part), 2026-10-07.** Menus 1-4, 6-11
walked headless with tools/aemulator_vttest.c and judged against
vttest's source; PNGs looked at where attributes matter. Fixed from the
walk: DECCOLM minimum (ae44dc66), DECSCNM ?5 (12c8f851), terminale
decorations (6bc29e81), drawn box/block/braille glyphs (8068d0fe,
f7774b27). Correct: cursor movement, screen features, charsets at
level 2 (vttest sends `ESC ) A` = British, not the 96-set, because DA1
says 62), VT102 insert/delete, reports (DSR/DA1/DA2/DA3), RIS/DECSTR,
ECMA-48 cursor/scroll (HPA..VPR, SD, SU), colours incl. BCE paths,
known bugs A/W/S, VT220 DECTCEM/ECH, alternate screens 47/1047/1049.
By design (Ghostty does the same or lacks it too): double-size lines
(DECDWL/DECDHL; also makes bug B unjudgeable), VT52, SL/SR, REP after
a control sequence, 8-bit GR bytes as U+FFFD, black-on-black visible
(contrast floor). Driver artefact: LNM "Not expected" (raw bytes skip
the encoder). Parked: blink, italic. Left for Fran's session
(interactive): keyboard menu 5, mouse 11.8.5, SRM, window title typing.
