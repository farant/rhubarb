# Mutationes briar

Quid in quaque versione briar mutatum sit, pro scriptoribus plagularum
`.thistle`. Imprimitur per `briar -mutationes`.

Leges chartae:

- Caput supremum `## vN — dies` versionem binarii DAT: `briar -versio`
  eam ex hac charta (in binario infixa) legit - numerus alibi non
  scribitur. Capita numeris stricte descendentibus stant, novissimum
  primum.
- `## inedita` mutationes nondum editas colligit, linea una per
  mutationem, eo tempore quo fiunt. Editio = caput `## inedita` in
  versionem proximam vertere et novum `## inedita` supra ponere.
- Tegit: mores briar ipsius; bibliothecas corporis additas aut remotas;
  mutationes corporis quae plagulas exsistentes frangunt. Mutationes
  corporis minores non hic: linea `corpus:` in `briar -versio`
  commissum exactum nominat.

## inedita

- `imago_opus`: `imago_creare` (colore uno plena), `imago_excidere`
  (regio SINE scala, ad fontem praecisa), `imago_transcribere` (fons in
  dest ad dx, dy - pixela substituuntur, dx/dy negativi licent),
  `imago_rectangulum` (ambitus crassitudine introrsum) - omnia ad
  margines praecisa (lapide feature-requests/021). `imago_extrahere_et_
  scalare` cum regione EXTRA fontem iam imaginem vacuam reddit (olim I x
  I et lectio extra limites).

- ictus cache celer: extra arborem rhubarb (corpus infixum) clavis ex
  octetis (plagula + bibliothecae) computatur ANTE silvam et fabricam -
  programma iam aedificatum statim currit (textus lapidis IXM
  linearum: 0,75 s -> 0,00 s per cursum). Clavis eadem ac antea (nulla
  aedificatio nova). `BRIAR_VESTIGIUM=1` ictum celerem in stderr nominat.

## v4 — 2026-09-29

FRANGIT: `lib/toml.c` vetus remotum - plagulae `toml_capere_*` aut
`toml_error` utentes ad `toml` novum migrent (tabula infra). Nova:
plagulae inter se codicem communicant per `<bibliotheca via="x.thistle"/>`
(lapide feature-requests/015); TOML 1.0 integer (feature-requests/013);
vexilla atrii programmatibus vitreis praemittuntur (feature-requests/023);
adiutores probationis sine `staticus` (documentation-ideas/016);
descriptiones bibliothecarum (documentation-ideas/017,
feature-requests/016); directivae condicionales in regione C.

- FRANGIT: `toml` novum - cliens materiae TOML 1.0 integer
  (`toml/fontes/toml.h`), `lib/toml.c` vetus remotum. Vetera -> nova:
  `toml_legere(t, p)` -> `toml_legere(t, via, p)`; `toml_error(doc)` ->
  `toml_diagnostica_scribere(p, doc, VERUM)` (omnia vitia cum
  `via:linea:columna` et caret); `toml_capere_chorda(doc, k)` ->
  `toml_chorda(doc, k, &c)` (FALSUM si abest - ordinarium tuum
  explicitum scribe); `toml_capere_numerum` -> `toml_integer` (s64);
  `toml_capere_boolean` -> `toml_boolean`; `toml_capere_tabulatum` ->
  `toml_chordae(doc, k, p, &xar)`. Novum: `[tabulae]` servantur (clavis
  punctata `toml_chorda(doc, "llama-server.versio", &c)`),
  enumeratio (`toml_tabulae_*`, `toml_seriei_*`), clavis iterata
  refutatur nominata cum sede priore, fluitantes, tempora, tabulae
  inlineae, series tabularum (lapide feature-requests/013). Sine
  `#include`: `toml.h` ex usu derivatur ut quodvis caput domus.
- corpus: radices clientium materiae (`materia/fontes`, `toml/fontes`)
  in corpore infixo - `-bibliothecae` eas enumerat, `-bibliotheca toml
  -functiones` signaturas dat, `-amalgama` eas complectitur; omne
  proiectum genitum eas vendit (instrumentum capsulae toml trahit).
- `-bibliothecae`: XXV descriptiones novae cum verbis Anglicis
  (`tabula_dispersa` = hash table, `internamentum` = string interning,
  `piscina` = arena, `xar` = vector segmentatus, ...); sine descriptione
  CXIII -> LXXXVIII (lapide documentation-ideas/017). `-dialectus`:
  laqueus `atoi(c.datum)` -> `chorda_ut_s32` / `chorda_ut_s64`, quae
  iam exsistunt et nunc documentantur (feature-requests/016).
- regio `munus="probatio"`: prototypi adiutorum generantur ut in regione
  principali - adiutor sine `staticus` non iam frangit
  `-Wmissing-prototypes`, et ordo definitionum liber est (lapide
  documentation-ideas/016). `-dialectus` regulam `staticus` nominat.
  Cache: fontes briar ipsius clavem intrant - briar mutatus proiectum
  novum aedificat etiam sine commisso novo.
- vexilla atrii ante plagulam (`-vivum`, `-retro`, `-portus <n>`,
  `-radix <via>`) programmati vitreo praemittuntur: `manus incipere
  briar x.thistle` nunc currit (lapide feature-requests/023); olim
  `vexillum ignotum: -vivum`.
- directivae condicionales in regione C: coniunctio `#ifndef X` /
  `#define X` / typus / `#endif` iam aedificatur (olim directivae eius
  bis in caput genitum emittebantur - `#define` primum copiam typi
  celabat: 'unknown type name'). Coniunctio directivarum sola ordine
  fontis manet; `#ifdef` intra corpus functionis in corpore manet
  (parcum VF42V).
- `<bibliotheca via="textus.thistle"/>` (elementum, columna 0): regiones
  C PLANAE plagulae alterius (functiones, typi) in omni regione C
  praesto - `principale`, regiones ceterae, `-probatio` - sine
  `#include`, derivatione ut caput domus (lapide feature-requests/015).
  Transitiva; `via` contra directorium PLAGULAE (non cwd); `interior` /
  `staticus` privata manent; `principale`, probatio, `methodus`,
  fragmenta membri numquam praebentur. Refutationes cum sede: circulus
  (cum remedio), nomen publicum bis, titulus iteratus, plagula absens.
  Cache: octeti membrorum omnium clavem intrant (membro SOLO mutato
  aedificatur). `-amalgama` membra complectitur (statica cuiusque
  renominata; plagula una clang sola compilat); `-partes` lineam
  `bibliotheca:` per membrum dat (membra attacta, nomina publica).

## v3 — 2026-09-28

FRANGIT: `MMMM` et `MMMMXCVI` remota - plagulae eis utentes `IV * M`
et `IV * MXXIV` scribant (lapide `loca.thistle` ter). Cetera: http
sine truncatione tacita, nuntii mortis latina.h plenius, et quae
lapide invenit (bugs/012, 013, 015, 016, 018; documentation-ideas/014).

- `http` (cliens, HTTPS): corpora supra ~IV KB integra redeunt (olim
  truncata cum `successus`: tls_recipere lectionem partialem ut
  finem reddebat) - lapide bugs/015, bugs/016. Corpus brevius quam
  `Content-Length` promisit, aut chunked sine fragmento terminali,
  nunc ERROR est (`HTTP_ERROR_IO`, "Corpus truncatum: Content-Length
  N, recepti M"), numquam successus cum corpore partiali.
- `-h` linea prima = linea prima `-versio` (cum `+inedita(n)`;
  olim `vN` solum) - bugs/013.
- `-dialectus`: series numerorum non integra regulam dicit (`omnes
  0-N; supra selecti tantum`) pro `(344)` qui integram simulabat -
  documentation-ideas/014 (latina.h ipsa nunc integra, infra).
- Nuntius mortis: admonitio latina.h etiam cum verbum latinae ante
  lexema mortis stat (`s32 nomen = 1` moritur in `=`: "'nomen'
  macrum latina.h est") - bugs/018.
- `via_nomen` / `via_directorium` (corpus): separatores terminales ut
  POSIX - `"/foo/bar/"` -> `"bar"` + `"/foo"` (olim `""` +
  `"/foo/bar"`); `"/"` -> nomen `""` consulto - bugs/012.
- **FRANGIT** latina.h (corpus): numeri Romani OMNES `ZEPHYRUM`-
  `MMMCMXCIX` definiti (olim 344 selecti; `CCCXIX` nunc exstat), sectio
  generata et ordinata. `MMMM` et `MMMMXCVI` REMOTA (numerus classicus
  ad 3999 finit): scribe `IV * M` et `IV * MXXIV` - in corpore macro
  `(IV * MXXIV)` parenthesibus. `-dialectus`: `(4000)`, sine regula.

## v2 — 2026-09-26

Regiones C per silvam novam (silva in materiam migrata, phasis V): quae
lapide invenit (bugs/001, 009, 010) sanata.

- `--help` et `--version` synonyma sunt `-h` et `-versio` (ante
  plagulam solum; post eam argumenta programmatis manent); nota
  vexilli ignoti `-h` nominat.
- `-versio` lineam `aedificatum:` addit (tempus, sigillum fontium briar,
  commissum; SORDIDUM si fontes mutationes non commissas ferunt) - duo
  binaria eiusdem versionis nunc discernuntur. Mutationes ineditae in
  charta: linea prima `briar vN+inedita(n) — dies`.
- Regiones C: `va_arg(va, T)`, `offsetof(T, m)`, `va_start`, `va_end`,
  `assert` parsantur (antea `regio C: parsura fracta` - bugs/009);
  typi systematis (`FILE`, `size_t`...) noti. Transitus silvae
  lexicon systematis habet, ut examen.
- `__attribute__((...))` accipitur in omni sede (post prototypum, ante
  declarationem, post `}` structurae) et ad clang transit, qui eam
  tractat (`sentinel`, `format`, `unused`...) - bugs/010.
- Errores syntaxis in regione C: linea et columna VERAE in plagula
  `.thistle` (antea linea prima regionis semper), ordo
  `plagula:linea:columna` sub summario cum excerpto ('hic coepit' ubi
  unitas incipit, 'hic exspectatur' ubi parsura periit); `(1 error)`
  singulare; `-visio` excerptum in `<pre>` servat - bugs/001.
- Nuntii errorum causam dicunt: lexema inventum nominatur (`lexema
  'redde' quod grammatica hic non accipit`); verba `latina.h` ut nomina
  usa admonentur (`'nomen' macrum latina.h est ('typedef'): nomen aliud
  elige`, etiam `casus`, `C`...); `';'`, `'}'`, `')'` fortasse deest
  suggeritur ubi status parsurae id exspectabat. Errores plures in
  unitate una: primus solus (limes notus).

## v1 — 2026-09-24

Versio prima numerata: status omnium quae ad hunc diem exstant (antea
"v0" sine charta). Historia accuratior in git et in
`briar/fontes/briar.worklog.md`.

- Plagulae litteratae C89 (`.thistle`): regiones C, regio probationis,
  fragmenta contexta, `#line` verum ad lineam plagulae; forma
  `#!/usr/bin/env briar` cum argumento reservato uno post plagulam.
- Currere: aedificare si abest, deinde fieri; `-probatio`; `-struere`
  [`-iterum`]. Clavis aedificationis = corpus + vexilla + octeti.
- Corpus domus infixum (`include/`, `lib/`, fontes venditi): `-f
  <radix>` > ascensus ex directorio currenti > corpus infixum.
- `-arbor` (proiectio STML), `-partes` (clausura), `-amalgama`
  (plagula una quam clang SOLA compilat - effugium), `-html` (pagina
  litterata), `-visio` (spectator), `-app` (fasciculus `.app`, `-icon
  <via>`).
- Documentatio corporis: `-bibliothecae` (bibliothecae cum
  descriptione), `-bibliotheca <nomen>` [`-fons` | `-functiones`],
  `-dialectus` (charta dialecti: typi, vexilla, verba latina.h,
  laquei C89).
- `-versio` (versio, stampa corporis, sigilla vexillorum) et
  `-mutationes` (haec charta).
