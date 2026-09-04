# vocabularium/la — Whitaker's WORDS, venditum verbatim (T7, 2026-09-04)

Fons: `https://github.com/mk270/whitakers-words` (portus curatus
programmatis WORDS Gulielmi Whitaker, 1936–2010), ramus `master`,
commissio `1f2f0fb0867a896d7b9284a03d615ed635d6f992` (2026-08-26),
plagulae per `raw.githubusercontent.com` 2026-09-04. Octeti VERBATIM
(CRLF servatum). Licentia: `LICENTIA.txt` (sectio "Licensing" ex
README.md verbatim) — auctoris verbis, "Permission is hereby freely
given for any and all use of program and data."

| plagula | octeti | recorda | forma |
|---|---|---|---|
| `DICTLINE.GEN` | 6,115,855 | 39,335 | FIXA: stirpes IV × (XVIII + spatium) = LXXVI columnae; pars orationis a columna LXXVII (XXIV columnae); codices V (aetas area geo frequentia fons) a columna CI, spatiis separati; spatium; sensus a columna CXI ad finem lineae |
| `INFLECTS.LAT` | 129,314 | 1,797 | spatiis separata, una per lineam, commentaria `--`; pars [declinatio varians] accidentia... clavis-stirpis mensura [finis] aetas frequentia |
| `ADDONS.LAT` | 34,697 | 343 | terna: `PREFIX|SUFFIX|TACKON forma` / recordum / sensus (linea sensus quaelibet, etiam 'TACKON ...' aut 'PACKON ...' incipiens) |
| `UNIQUES.LAT` | 9,642 | 79 | terna: forma / recordum / sensus |

Columnae DICTLINE ex declarationibus Ada transcriptae
(`src/latin_utils/latin_utils-dictionary_package.ads`,
`-dictionary_entry_io.adb`: `Stem_Type` XVIII + spatium × IV,
`Part_Entry_IO.Default_Width` + I → columna CI translationis, spatium,
`Meaning_Type` ad LXXX) et in OMNI recordo a coctore ASSERTAE: recordum
quod non cadit inventum est quod coctionem sistit (numerus octetorum
pinnatus fontem custodit, assertio lectionem).

Tabula cocta: `la.bin` (nuntium, recorda: caput, stirpes, flexiones,
additamenta, unica) per `./oratio/vocabularium.sh -coquere -scribere`;
sigillum (SHA-256, XVI hexadecimales) et numeri a porta
`probatio_oratio_vocabularium` pinnantur; coctio(fontes) == tabula
commissa octetim. Re-venditio = mutatio nominata portae.

| tabula | octeti | sigillum | coctio / recensio |
|---|---|---|---|
| `../la.bin` (T7, 2026-09-04) | 3,816,483 | `f598155c06f52682` | 24 ms / 3 ms |
