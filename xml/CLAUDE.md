# xml — orientation

*You are reading this because you touched a file in `xml/`.*

XML 1.0 (Fifth Edition) + Namespaces 1.0 as a materia client (the
eighth). Spec: `project-specs/xml-arbor-spec.md` (decisions X1–X12).
Plan: `project-specs/xml-arbor-plan.md` (P1–P17, one task per turn
with Fran). Ledger: parcum 01M65252W1DKBJE10VX0NBQDJH. It will replace
`lib/xml.c` / `include/xml.h` (P15).

## Status

- **P1 (oracle material) done.** No C yet.

## Oracles (P1)

- **W3C suite** — NOT in the repo (James Clark's licence forbids it):
  `./xml/xmlts_arcessere.sh` fetches it into `xml/build/xmlts/` and
  checks it against `probationes/fixa/xmlts.manifestum`. The cases we
  judge, with exclusions by cause: `probationes/fixa/xmlts/
  selectio.tsv` (regenerate with `python3 -I
  xml/instrumenta/xmlts_catalogus.py xml/build/xmlts/xmlconf`; add
  `-mensura` for the expat/libxml2 agreement). Everything measured —
  counts, licence, canonical forms, oracle credibility — is in
  `probationes/fixa/xmlts/PROVENIENTIA.md`.
- **expat** — `instrumenta/expat_canonicum.py <via>`: the suite's
  canonical form, or `INVALIDUM: l:c cause`. Never reads an external
  entity. Known divergence: editions 1–4 name rules.
- **libxml2** — `xmllint --noout --nonet <via>`. Known divergence:
  namespace errors are not failures.
- **Wild corpus** — `probationes/fixa/silvestria.manifestum` (7,931
  real files on this machine, sha256 + path only); rebuilt by
  `instrumenta/silvestria_indicem.py`.

Harnesses read untrusted files: run them with `python3 -I`, paths as
arguments.
