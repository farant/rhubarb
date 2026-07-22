# silva_annotationes worklog

## 2026-07-21 — Chunk A: the collector (born with the amalgam seat)

Park 01KY3D7EJP chunk A; spec = project-specs/annotationes-stml-spec.md.
Module shape: trivia walk (TOLERA pattern, radix chain included) →
tag-at-comment-start anchoring → decoration pre-pass → stml_legere →
byte-based attachment (SUPRA/INTERIOR/PLAGULA). probatio: 9 sections,
63 pins.

**Seat decision (Fran):** stml vendored INTO the silva amalgam.
The pre-amalgam-arc cost argument was stale — fontes lists are
aedilis-derived and excludenda compiler-harvested, so the real hand
cost was ~12 rename entries + the silva.h reading subset + 2 policy
lines. internamentum rides along (stml interns names); selectio is
in the closure but ships EXCLUDED wholesale (dead subsystem — via
retro documented in fontes_politica.sh for the day annotation
selector queries want it).

**Bug of the day — the extensionem accumulator contract.**
`silva_nodus_extensionem` does NOT initialize its out-params; the
header's "*minimum initia < 0, *maximum initia 0" is an instruction
TO THE CALLER (it's an accumulator — that's why the recursion passes
the pointers through). Both my call sites passed uninitialized
locals: the probatio's stack slots happened to hold 0 → a
plausible-looking [0,94] "whole file" extent; the module's held a
spilled 64-bit unit POINTER → min=1/max=0x50082E80 (the pointer's
two halves — the decode that cracked the case). Every SUPRA pin in
the first run passed BY ACCIDENT (garbage min=1 > offset). The
hospes extensionem block had the contract right all along
(minimum=-1, maximum=0 before the call) — the precedent existed,
I just hadn't read it. Fix: initialize per contract in _affigere.
NOTE: this is exactly the codex-71 class (uninitialized read),
invisible to FLUXUS because the writes hide across the call
boundary — the interprocedural-summary gap the annotation type
layer (park 01KXTX7FA3) exists to close. The bug advertises the
product.

**Tree shape:** commissio->radix is a SILVA_VALOR_LISTA of
top-level sentence values (not a node) — "fortasse religata" is
real. silva_annotationes_unitates() accepts both LISTA and NODUS
radix shapes and is public (probatio needs pointer-identical units;
chunk B's index wants titles).

**Attachment semantics as shipped:** extents count REAL tokens only
(trivia excluded), maximum = one-past-end of last token. End-of-line
annotation after a unit's `;` therefore binds SUPRA to the NEXT
unit (the recorded wrinkle). INTERIOR = byte containment. Error
line mapping: fons = annotatio.linea + linea_erroris - 1 (pre-pass
preserves line count; columns shift under decoration strip).

**Amalgam integration battle log** (harvest fixes live in
tools/amalgama_excludenda_generare.sh + tools/aedilis.worklog.md):
- S41 guard caught `_est_littera` (selectio vs silva_lexema) on
  first contact → renamed `_est_littera_selectoris` at source.
- InternamentumChorda restructured to the SEPARATED typedef-opaque
  idiom (typedef + body as distinct units) so CADENDA can drop the
  typedef while the body stays internal (SilvaOraculum pattern).
- The harvest cascade seed was a STALE HAND SERVANDA:
  SERVANDA_FRIATIONIS kept only fnv1a while the new libs made the
  tabula literis path live → signal B ("callers of servanda-dropped
  fns are dead-in-context") amplified one stale line into a
  hollowed-out live core, swallowing stml_legere itself. Cure:
  fnv1a_literis added to servanda + PROTECTA guard (non_statica
  names may never be excluded — refusal names the seed).
- internamentum_globale's file-scope statics moved INSIDE the
  function so its exclusion is self-contained (the
  unused-variable-under-excluded-caller class; selectio's cache
  statics were the same class, solved by the wholesale drop).
- Final: harvest fixpoint 6 gyri / 147 names; amalgamare
  VERIFICATUM (standalone + hospes 33/33 + nm 0 + censura);
  silva.c ~2.15 MB.

**Bootstrap deadlock note:** a failed amalgamare leaves a BROKEN
committed silva.c on disk, and bin/aedilis (which the fontes
generator rebuilds) links silva.c → the generator dies against the
broken amalgam. Loop-break: `git checkout -- silva/amalgama/silva.c`
(HEAD is always a VERIFICATUM amalgam), then run the chain forward.
Remember this before diagnosing "aedilis suddenly won't build."

**Launcher lists:** new fontes module nobody includes yet broke the
principal-closure assumption in silva/*_fontes_generata.sh (and four
launchers carried FROZEN glob expansions). tools/
silva_fontes_generare.sh (no args) now owns all 12: live glob
expansion + principals persisted in each snippet's "# principalia:"
line.
