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

Initializatio explicita addita, ET custos in probatione: volumen
ordinarium DECIES creatur et clauditur, et plagula superesse debet
omni vice (purgamentum stabile non est - unus cursus casu nullum
ferre potest).

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
