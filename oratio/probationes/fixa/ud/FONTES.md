# fixa/ud — treebanks UD Latinae, venditae verbatim (T13, 2026-09-04)

Oracula gradus III (oratio-spec par. V): documenta CoNLL-U manu
annotata contra quae classes universales (UPOS) et lemmata orationis
conferuntur. Venditae SOLAE quae licentia CC BY-SA 4.0 (LICENTIA-*.txt,
README-*.md verbatim) — nihil ex treebank derivatum umquam editur;
treebanks NC (ITTB Aquinas, PROIEL, Perseus, UDante) per
`./oratio/oraculum.sh -petere` in `oratio/build/ud/` petuntur et
relatae solum.

| plagula | fons | commissio | octeti | sigillum (SHA-256 XVI) | sententiae | lexemata |
|---|---|---|---|---|---|---|
| `la_circse-ud-test.conllu` | UniversalDependencies/UD_Latin-CIRCSE (Seneca, Hercules Furens; versus) | 44b969c3 (2026-05-06) | 1,444,828 | 51be34d895fcc372 | 893 | 11,503 (rangae 316) |
| `la_llct-ud-dev.conllu` | UniversalDependencies/UD_Latin-LLCT (Late Latin Charter Treebank; chartae VIII-IX saec.) | 173cd113 (2026-05-06) | 2,357,828 | 040457f2e47265a5 | 850 | 24,189 (rangae 3) |
| `la_llct-ud-test.conllu` | idem | 173cd113 | 2,345,127 | 81d9f57de66d8330 | 884 | 24,079 (rangae 2) |

LLCT: scissurae dev et test SOLAE venditae (decisio Frani 2026-09-04:
train ~CC milia lexematum nimis gravis pro fixtura); UD ipsa dev/test
ad mensuram adhibet. Octeti ex `raw.githubusercontent.com` ramo master
2026-09-04.

Notae formae (mensurae ante lectorem): comments `# sent_id`, `# text`
(omnes sententiae), `# speaker` (CIRCSE), `# reference` (LLCT); rangae
`a-b` = lexema superficiei cum encliticis scissis (pronumque = pronum +
que), UPOS `_`; nodi vacui `a.b` nulli; `SpaceAfter=No` in LLCT solum
(interpunctio CIRCSE nulla); orthographia CIRCSE `u` pro `v` (uentum,
nouus), LLCT mediaevalis (Karolus, domno) cum interpunctione `+ , .`.
Classes: NOUN VERB ADJ CCONJ DET ADV PRON ADP PROPN SCONJ PART AUX NUM
INTJ PUNCT X — DET et AUX quas WORDS non distinguit (pronomina
demonstrativa/possessiva, sum) mappationem T12 probant.
