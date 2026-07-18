
## 2026-07-17 — praecidi_laterale: the all-whitespace double bug

The VANA codex-58 adjudication reached chorda's two sites. The
backward trim scan (`per (i = mensura - I; i >= ZEPHYRUM; i--)`)
on unsigned i32 could never terminate for all-whitespace input —
i wraps at zero and the loop reads wild memory. AND independently:
the initium/finis defaults meant an all-space string would have
returned UNTRIMMED (forward scan leaves initium = 0), not empty.
Fixed with the count-down idiom (`per (i = mensura; i > ZEPHYRUM;
i--)` testing datum[i - I]) plus initium = mensura as a not-found
sentinel, so initium >= finis catches the all-space case. Also
removed the dead negative clamp in chorda_ex_bytes_legibilis
(bytes is i64 unsigned). Empty-string was always guarded; only
all-whitespace hit the trap — never observed in practice, which
means no caller has trimmed a pure-whitespace string yet.
