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

- `--help` et `--version` synonyma sunt `-h` et `-versio` (ante
  plagulam solum; post eam argumenta programmatis manent); nota
  vexilli ignoti `-h` nominat.
- `-versio` lineam `aedificatum:` addit (tempus, sigillum fontium briar,
  commissum; SORDIDUM si fontes mutationes non commissas ferunt) - duo
  binaria eiusdem versionis nunc discernuntur. Mutationes ineditae in
  charta: linea prima `briar vN+inedita(n) — dies`.

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
