# tonus.worklog.md

## 2026-10-27 - tonus: melodic mode reading of a single line (chant arc, step 2)

New pure library (include/tonus.h): one monodic part in a modal key (carmen.h MODUS, key taken
at the LAST note) -> final, range, authentic/plagal, tone number I-XII (Glarean), reciting tone
(total duration per pitch class, final excluded; tie -> the expected tenor, then the lowest
class), phrase endings (a note followed by a rest, a note whose start a fermata covers, the
last note; deduplicated). Not judged: tonal key, empty part, any overlap between notes. Why not
functio: functio reads harmony windows - over a drone it produced Isus4 / iv43 for primus.
Plagal test uses the line's last note of the final class; if the final never sounds, the final
just BELOW the lowest note is assumed (authentic is the usual case) - first draft assumed above,
which made such lines plagal; changed with a test. auditio -carmen T -tonus prints one line per
part (a per-part summary does not fit partitura's per-bar extra lines - the one deviation from
the proposed header). censura 'finalis' (ORD.): a monodic line in a modal key not ending on
the final. primus: "I (dorius authenticus): finalis D, ambitus C4-D5, tenor A, clausulae A A F
D"; its ison also reads (D3-D3, tenor nullus) - correct, a little noisy.
