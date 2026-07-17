# sententia_fissio.c worklog

## 2026-07-17 — sentinel removed from _invenire_finem_sententiae

VANA adjudication (quaestio 01KXQ9NF8J class 2): the function
documented "-1 si nihil" but returned it as unsigned i32, so all
four callers' `si (finis < ZEPHYRUM)` clamps were dead. Both -1
branches were also unreachable from the call sites (every caller
guards initium < mensura first). Rather than s32-ify, the sentinel
was DELETED: the function now returns textus.mensura itself when no
sentence end exists — which is exactly what every caller converted
-1 into. Four dead clamps removed; sententia_initium became unused
and was dropped.

Also: _extrahere_verbum_ante_punctum's backward scan condition
`dum (initium >= ZEPHYRUM)` (always true on unsigned) became an
honest `dum (VERUM)` — the loop already breaks at 0 BEFORE
decrementing, so it never wrapped; the condition was just
misleading. And sententia_prior's `prior_index < 0` wrap check
became a live `index_currens == ZEPHYRUM` pre-check (the old code
was accidentally safe only because 4e9 failed the next bounds
check).
