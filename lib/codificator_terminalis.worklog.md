# codificator_terminalis worklog

## 2026-10-07 — D6a: the emulator's modes (aemulator-plan D6)

The encoder now takes everything `aemulator_modi` reports except the
keypad: DECCKM (arrows, Home, End without modifiers -> SS3, legacy path
only - Ghostty's kitty path ignores it too), X10 event mode (?9: left /
middle / right presses only, no modifiers, no wheel), the four other
report formats (X10, UTF-8, urxvt, SGR-pixels) and LNM.

**Format 0 stays SGR.** Every caller zero-fills CodificatorModi and sets
`mus` expecting SGR (terminale, three ludus_tessera tests), so
CODIFICATOR_FORMA_SGR = 0 although a real terminal's default is X10;
terminale maps the emulator's enum explicitly.

**Legacy releases are button 3** (X10, UTF-8, urxvt; mouse_encode.zig
buttonCode), modifiers and motion bit kept. X10 cannot encode a cell
past 222 - nothing is sent (Ghostty logs and returns).

**SGR-pixels dedupes on pixels, not cells** (Ghostty: "only send motion
events when the cell changed unless sgr_pixels"). The first version of
plant F10 changed only the comparison and survived because the stored
value was still a pixel - a plant has to break the whole idea.

**LNM is a pass over what this call appended** (Ghostty applies it in
Exec.queueWrite to every write: keys, paste). In place: count CRs, grow
the builder by that many bytes, shift backwards inserting LF. Callers
reuse one builder across calls (the round-trip test does), so the pass
starts at the length on entry - a test appends twice and checks no
`\r\n\n`.

**Keypad application mode is deferred:** Eventus has no numpad keys
(the Mac's numpad arrives as digits), so the field would be dead.

## 2026-10-07 — shift+; sent ';' in nvim (kitty disambiguate)

Found in Fran's first terminale session: nvim (0.9.4, `vim` alias)
queries `CSI ? u` at startup; since D4 we answer, so it pushes kitty
flag 1. Our disambiguate path sent text plainly only with NO modifier;
any shifted key became `CSI <runa>;2 u`. nvim decodes `CSI 104;2u` as
`H` (letters looked fine) but `CSI 59;2u` as shift+';' - so `:` typed
`;`. Kitty (and Ghostty, via effectiveMods = mods - consumed_mods) send
the TEXT when the only modifiers were consumed producing it. Eventus
carries no consumed mods, so: shift counts as consumed when the text
differs from the unshifted rune (`:` vs `;`, `A` vs `a`, `!` vs `1`).
Ghostty's unit test "kitty: shift+a on US keyboard" expects CSI 97:65;2u
only because it sets no consumed_mods; real macOS reports shift as
consumed. Verified end to end headlessly: real nvim in terminale via the
probe harness (pty), shift+; q a shift+1 Enter -> nvim exits; with the
rule reverted it stays open. Limitation of the heuristic: a shifted key
whose text equals its rune (shift+space) still goes out as CSI u.
