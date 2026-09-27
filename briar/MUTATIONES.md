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
