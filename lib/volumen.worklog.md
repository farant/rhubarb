# volumen worklog

## 2026-08-04 — birth (G2 seed, silex as first consumer)

Born as the smallest true slice of the G2 design (see
project-specs/compages-rationes-tutorial.md gap table + ledger res
01KZ4Y9A8T): one sqlite file = one document, with `acta` (append-only
event log, the truth), `massae` (content-addressed blobs by sigillum
hex, INSERT OR IGNORE gives dedup for free), and `plagulae` (the
manifest projection, upsert by via). Deliberately NOT built here yet:
genera, custodia, personae, replication — those arrive on-pull per the
tutorial's G3/G7/G8.

Decisions worth remembering:
- **Timestamps stamped in SQL** (`strftime('%Y-%m-%dT%H:%M:%fZ','now')`
  as column DEFAULT) — the gesta.c precedent. No C time API, no POSIX,
  no fasti dependency. ISO text like the annales.
- **plagulam_condere is one transaction** wrapping massa + actum +
  manifest upsert (the gesta K1 law: the event write wraps the whole
  road). Failure path = single `revolve:` label + rollback.
- **Empty files are legitimate**: promere distinguishes "not found"
  (b32* inventum) from "found, empty" (chorda vacua). scrinium got a
  blob pair (ligare_massam / columna_massa) for this arc;
  sqlite3_bind_zeroblob(0) covers the empty-blob case (a NIHIL datum
  pointer would otherwise bind SQL NULL — measured trap, comment in
  scrinium.c).
- Migration array had to be renamed MIGRATIONES_VOLUMINIS — leading
  `_` + capital is implementation-reserved (C89 7.1.3), house lint
  caught it at edit time.

## 2026-08-04 — the day the volumen grew up (API additions, one day)

Born yesterday with condere/promere/enumerare; grew today, each on a
real pull, never speculatively:
- `volumen_acta_legere(vol, post_seq)` — for the mensa fold; shaped
  as "acta after seq N" because that's ALSO the G8 replication read.
- bare massa API (`massam_condere` → sigillum hex out, `massam_promere`
  by sigillum) — for image cards. Doctrine: a massa is CONTENT, not an
  event; truth about its use lives in acta.
- caller-owned transactions (`transactionem_incipere/committere/
  revolvere` + in_transactione flag; plagulam_condere/removere skip
  their own BEGIN under a caller's) — for atomic multi-file commits;
  sqlite forbids nesting.
- `plagulam_removere` — actum plagula-remota + manifest DELETE; the
  massa stays (history references it).
- `volumen_aperire_aut_creare` — born from the red-team walk: the
  tutorial promised create-or-open and no function delivered it.
All pinned in probatio_volumen (including binary blobs with embedded
zeros, dedup, and the aut_creare both-paths case).

## 2026-08-04 — plicam_ad: the manifest learns to time-travel

`volumen_plicam_ad(vol, seq, piscina)` — reconstruct the manifest at
any point by replaying plagula-condita/plagula-remota acta (seq <= N;
<= 0 = everything). Lives here rather than in silex because it is the
time-travel READ primitive and G8 replication replays the same
events. The datum shapes ({via, sigillum, origo}) had everything
needed from birth — no schema change, no migration.

The free integrity pin: plicam_ad(0) must equal plagulas_enumerare()
— the replay proving the stored projection's upsert logic. Pinned in
probatio along with: mid-seq fold shows pre-upsert content (a.txt and
b.txt shared a sigillum back when they shared content — dedup made
that assertion free), and a removed plagula reappears in earlier
folds (history integra).

Tool-reliability find while gating this: legati warm diagnostica
holds a header-dependency cache that NEVER revalidates — after adding
the plicam_ad prototype to volumen.h, the resident kept judging
probatio_volumen.c against the OLD header (false REICE, implicit-call
diagnostics), and neither re-query, header-first batching, nor
renovare ("iam recens") heals it. Filed as quaestio 01KZ6GH88Z; cold
./silva/examen.sh is the authority for files including
recently-changed headers until the resident restarts.

## 2026-08-13 — volumen temporarium (status qui probationem non superstat)

`volumen_temporarium(piscina, praefixum)` plagulam unicam sub /tmp
creat et in CLAUDENDO delet. `volumen_via` viam nunc reddit (omni
volumini, non temporario soli).

### Quid hoc coegit

Probatio fumi laboratorii viam FIXAM applicationis adhibebat, ergo in
volumen VERUM usoris scribebat. Post diem unum:

```
experimentum|21   salutatio|42
```

in ~/.rhubarb/laboratorium.volumen - acta quae nemo scripsit nec vult.

Sordes tamen minus gravis erat quam ALTERA sequela: probatio talis
REPETIBILIS non est. Numerus visitationum omni cursu crescebat, ergo
assertum quodlibet de eo hodie transiret et cras caderet. Status
inter cursus manens fontem fragilitatis facit quem nulla mora sanat -
et haec bibliotheca contra fragilitatem tota aedificatur.

### Laqueus quem paene induxi

Struct Volumen per `piscina_allocare` sumitur et CAMPO-POST-CAMPUM
impletur - nullum memset. Campos duos addidi (`temporarium`, `via`);
utrumque ergo PURGAMENTUM ferret in volumine ORDINARIO, et
`temporarium` purgamentum non-nullum `filum_delere(via_purgamenti)`
in claudendo vocaret. Volumen verum usoris perire posset.

Initializatio explicita addita, ET custos in probatione - sed custos
PRIMUS INUTILIS ERAT, et hoc pretiosius est quam remedium ipsum.

**Custos qui fallere non poterat (iterum).** Prima forma volumen
ordinarium DECIES creabat et claudebat, sperans purgamentum aliquando
non-nullum fore. Culpa plantata (initializatione ablata) probationem
VIRIDEM reliquit: paginae arenae recentes a systemate ZERO-IMPLETAE
veniunt, ergo campus 'purgamentum' fideliter FALSUM legebatur.
Probatio fortunam probabat, et fortuna cum vitio stabat.

**Repetitio memoriam non-initializatam non probat; VENENUM probat.**
Nunc: nota sumitur, spatium DXII octetorum 0xFF impletur, piscina ad
notam reficitur - et structura sequens in illos ipsos octetos cadit.
Purgamentum CERTUM est, non speratum. Culpa iterum plantata nunc
RUBRAM facit (filum_existit cadit - plagula re vera deleta est).

Lex generalis: custos contra campum non initializatum arenam
VENENARE debet, aliter nihil probat nisi systema paginas purgare.

Eadem forma qua VitreaConfiguratio hodie mane: applicationes figuram
campo-post-campum implent sine memset, ergo campus additus ibi quoque
purgamentum ferret - unde sutura per FUNCTIONEM posita est, non per
campum. **Structura quae memset non habet campum novum non patitur
sine initializatione explicita.**

### Nomina numerata, non fortuita

`/tmp/<praefixum>-1.volumen`, `-2`, ... primum liberum. Volumen ergo
pendens (probatio quae claudere omisit) nomen suum RETINET, et qui in
/tmp inspicit videt quot cursus reliquerint. Tempus aut fors nomina
nova semper daret, et reliquiae silentio congererentur.

## 2026-10-17 - durable option + streamed massae (hospitium v3 T6)

- `VolumenOptiones.durabilitas_plena` -> `synchronous=FULL` (via
  `ScriniumOptiones.synchronus_plenus`). Default unchanged (NORMAL under
  WAL: no corruption, but the last commits can be lost on power loss).
  PRAGMA synchronous is per CONNECTION, never stored in the file -
  every open must pass the option.
- `volumen_massam_condere_ex_filo`: dedup check first (an existing hash
  never reads the file), then SAVEPOINT + `INSERT ... zeroblob(n)` +
  64 KB chunks through `ScriniumMassa` + SHA-256 of the bytes written,
  compared to the hash the caller computed in flight. Any mismatch
  (hash, size, unreadable file) -> ROLLBACK TO + RELEASE, so it is safe
  inside a caller's `volumen_transactionem_*`.
- Reader opens the blob handle PER CHUNK. Not for commit visibility
  (I first believed an open read statement delays autocommit commits;
  a plant proved that false), but because a held handle pins a WAL
  snapshot: TRUNCATE checkpoints fail and the WAL grows for the whole
  download. Re-opening per 16-64 KB chunk costs microseconds.
- SQLite blobs cap at ~1 GB (SQLITE_MAX_LENGTH); the API refuses > 2 GB
  up front, sqlite refuses beyond its own limit.

## 2026-10-21 - volumen_in_transactione (canalis T4)

Public read of the existing `in_transactione` flag, so a library that
would otherwise open its own transaction can join the caller's instead
(sqlite forbids nesting). First user: admissio_revocare, called inside
the exemplar suspension transaction. Section "in_transactione" (begin,
commit, rollback, NIHIL).
