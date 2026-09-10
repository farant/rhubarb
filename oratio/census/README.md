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
| `lites.py <x.tsv> [file-part] [-victor V] [-victa W] [-meet]` | the feature census over contests (T32 e): from `LIS` rows (`metire.sh x.tsv -lites`), for the same-verb subject/object pair by default, each feature the rule could see (direction, other nominatives in the clause, certain accusatives, neuter, first of clause, head number / person / voice) against the gold case, with the purity of "feature → accusative, else nominative"; `-meet` tries every pair of features (in-sample, optimistic) |

The baseline for a measurement is the tsv of the committed state; keep
it beside the variant tsvs in the scratchpad, never in the tree.

## Rule discipline (unchanged)

Every variant is measured on all nine treebanks. A pin moves down only
with a named cause and Fran's decision. A refusal is recorded with its
component in `oratio/recusata.md`, because a refusal is a fact about
the configuration it was measured against.
