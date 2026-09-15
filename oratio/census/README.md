# oratio/census — the gold census and the measurement harness

*Born 2026-09-09 (T31 d). Until then every census lived in a session
scratchpad and died with it; knotapel keeps every experiment in its
demo folder, and this directory is oratio's version of that rule.*

Two halves of one loop, **predict then verify**:

1. **Census** — before a law is built, ask the gold what the law could
   reach. Nearest-X-is-Y rates, adjacency, distance, per relation, per
   treebank. A census names the ceiling of a law and the file where it
   cannot work (the charters, usually) before a line of C is written.
2. **Measurement** — after the law is built, run the oracle on all
   eight files and compare against the committed baseline.

All scripts run from the repository root. The four shelf treebanks live
in `oratio/build/ud/` (git-ignored; `./oratio/oraculum.sh -petere`
fetches them). `conllu.py` is the shared reader; `THESAURI` is the
seven-file Latin list.

## Census scripts

| script | question | tranche |
|---|---|---|
| `aurum.py <conllu…>` | what is in a treebank: tokens, roots, adjacent arcs, upos, deprels, feature keys | T13 / T26 |
| `genus.py <conllu…>` | gender: which classes carry it, agreement with the head, commune | T30 a |
| `caput.py <conllu…>` | is the nearest agreeing noun the adjective's head, by distance and direction | T28 |
| `stella.py` | subjects: explicit rate, case, position, distance, number agreement, nearest agreeing nominative | T31 a |
| `obiectum.py` | objects of finite verbs: rate by voice, case, position, nearest accusative with and without adposition objects, non-finite heads | T31 b |
| `infinitivum.py` | objects and accusative subjects of infinitives and participles; an adjacent free accusative is whose | T31 c |
| `participium.py` | active participles by tense; where an adjacent accusative really hangs | T31 c |
| `auctores.py <mensura.tsv>` | the trust view: per rule, words decided and primary, ARCS and precision, adjacent precision, worst file (from a `metire.sh` tsv, T32 a `AUCTOR` columns XI–XIV) | T32 a |
| `fiducia.py <mensura.tsv> [out]` | writes `oratio/probationes/fixa/auctores.tsv`, the executor's trust table, from the PINNED files only; regenerate with a named cause | T32 a |
| `pondera.py <lites.tsv> [-scribere [out]]` | the DECODER's weight table (T38 b, decisions 57–58): from the contest rows (`metire.sh x.tsv -lites`) of the pinned SHIPPABLE corpora only (`la_circse`, `la_llct`; refuses a run lacking either, exit 2), contests and rights per (rule, dialect, form, bucket, value) — each side of a judged contest credited (unjudged: no gold arc, or same head with an unjudgeable reading), right = its arc gold and its reading not wrong; keys: bucket rows (`numerus-capitis` and `positio-numerus` under dialect, each also pooled; `directio-distantia` measured and REFUSED by the lattice, kept as an inactive data row with its cause), the rule's plain rate in four context variants, the structural key `@relatio+ante\|post+proximo\|remoto` (`#` is the comment marker); bucket rows are DATA in the script with the lattice's numbers as cause; `-scribere` writes `oratio/probationes/fixa/pondera.tsv` with a provenance line — regenerate with a named cause | T38 b |

## Measurement harness

| script | use |
|---|---|
| `metire.sh <out.tsv> [oracle flags…]` | `oraculum.sh -machina` over the eight files into one tsv (environment variables reach the oracle: `ORATIO_…=… ./oratio/census/metire.sh x.tsv`; extra flags pass through, e.g. `-lites`) |
| `compara.py A.tsv B.tsv` | per file: primary, forced, case, number, gender, attachment, and the word deltas |
| `notae.py A.tsv B.tsv` | the feature columns: verb form, voice, gender, number, person |
| `relationes.sh x.tsv` | subject and object precision and recall per file |
| `grep CONTESTA x.tsv` | contested dependents, claims on them, right claims (T32 a) |
| `grep TECTUM x.tsv` | arc coverage per file: umbrae with any claim, with a right claim, right primaries, alternatives, right alternatives (T32 b) |
| `contentiones.py <mensura.tsv> [file-part] [-casus]` | the contest table: per (winner rule, loser rule) contests, loser right, then ARCS (loser right alone with the winner's actual head wrong, winner right alone, the gain of a flip) and CASES (contests judged for case, loser's reading right alone, winner's reading right alone, the gain of a flip); same-head contests are reading contests and discriminate by case only; `-casus` sorts by case discrimination (T32 b/c/d) |
| `contentiones.py <mensura.tsv> -scribere [out]` | writes `oratio/probationes/fixa/contentiones.tsv` (discriminating contests per pair, pinned files only) — the judge's table, inactive (`ORATIO_IUDEX=1`), T32 c refused |
| `errata_delta.py A B [nota…]` | feature errata deltas between two `-errata -nota` runs |
| `lites.py <x.tsv> [file-part] [-victor V] [-victa W] [-meet]` | the feature census over contests (T32 e): from `LIS` rows (`metire.sh x.tsv -lites`), for the same-verb subject/object pair by default, each feature the rule could see (direction, other nominatives in the clause, certain accusatives, neuter, first of clause, head number / person / voice) against the gold case, with the purity of "feature → accusative, else nominative"; `-meet` tries every pair of features (in-sample, optimistic); columns XXII–XXIII carry the number of the loser's and the winner's reading (T32 f: under a conditional order, which promoted readings were wrong); columns XXXIV–XXXV (T38 a) carry the declared context, `dialectus` (classicus \| medius, per file) and `forma` (versus \| prosa, per sentence from the ids: CIRCSE `_prose` = Tacitus prose, Perseus by PHI document) |
| `oraculum.sh … -decreta` (with `-machina`) | the DECODER's decision rows (T38 c): `DECRETOR` summary per file always (`cellae contestatae mutatae coacti coacti-rectae ordinati ordinati-rectae aperti aperti-rectae`); with `-decreta` a `COLUMNAE DECRETUM` header and one `DECRETUM` row per decided cell — `sententia dependens caput regula-electa regula-cedens ante numerus-capitis numerus-lectionis distantia gradus pondus-electae pondus-cedentis habitus aurum-electio thesaurus dialectus forma mutata` — `aurum-electio` ∈ electa \| cedens \| neutra \| ignotum (the chosen candidate's arc gold and reading not wrong, else the runner-up's, else neither, else no gold arc), readable by the lattice (`reticulum.sh x.tsv -genus DECRETUM -aurum aurum-electio -sortes thesaurus …`). The decoder is ON by default (the pure chooser, Fran 2026-09-15); `ORATIO_DECRETOR=0` disables it, `ORATIO_DECRETOR_ADDITIONES=1|classicus|medius` lets it also head words whose only standing claim is invisible (a recall move, off), `ORATIO_PONDERA_SINE=<folliculus|regula|structura>` drops one rung or bucket, `ORATIO_PONDERA_LIMEN=N` moves the row minimum (XX) |
| `./oratio/reticulum.sh <mensura.tsv> -genus LIS -aurum aurum-… (-sortes thesaurus \| -sortes-alternae N) [-ubi t=v1,v2]… [-gradus …] [-praeter …] [-profunditas N] [-prima N] [-initium t1,t2] [-greges columna\|catena\|catena-libera\|initium] [-exempla N] [-machina]` | the partition lattice (T35–T37): every column judged held out, leave one corpus out (or N interleaved folds); single columns by gain with the folds that gain and fall and captive groups, lattice of the non-constant columns (duplicates, covering edges, width), greedy chain with and without the fold veto (seeded from `-initium`; every candidate at the chain's end named with its cause), the refused-but-pooled steps as CONDITIONED candidates, k-meets to depth `-profunditas` by interaction gain, per-group evidence (profile coactus \| ordinatus \| apertus, vote and margin per fold) with `-exempla N` rows per group (`linea L: t=v … sententia=id`); reads the `COLUMNAE` header rows, refuses by name (exit 2); rows `RETICULUM-OPTIONES/-COLUMNA/-SORS/-CATENA/-FINIS/-CONDICIO/-INFIMUM/-TEGIT/-DUPLEX/-GREX/-GREX-SORS` with `-machina`, the options row first |
| `reticulum_recensio.py <mensura.tsv> <same flags>` | independent recount of every instrument row kind the judgment derives — `RETICULUM-OPTIONES/-COLUMNA/-SORS/-CATENA/-FINIS/-CONDICIO/-INFIMUM/-GREX/-GREX-SORS` — from the §7 definitions (plain dictionaries, never the C tool): `diff` of the two must be empty (T35 e: 146 rows; T36 d: 184 + 190 + 166; T37 b: 224 + 245 + 203 rows equal). zsh: `${=F}` splits a flag string, and never keep the command itself in a variable |

The baseline for a measurement is the tsv of the committed state; keep
it beside the variant tsvs in the scratchpad, never in the tree.

## Rule discipline (unchanged)

Every variant is measured on all nine treebanks. A pin moves down only
with a named cause and Fran's decision. A refusal is recorded with its
component in `oratio/recusata.md`, because a refusal is a fact about
the configuration it was measured against.
