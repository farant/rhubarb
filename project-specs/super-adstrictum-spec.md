# Super adstrictum — path-scoped citations, multiplex sites checked

**Date:** 2026-08-08 · **Status:** spec + plan (grounded)
**Origin:** parked 01KZE65SAM, THREE independent pulls: descriptor's measured hole (L8/L15 model violations passed green), the monolith preface's own confession (402 multiplex relations judged name-only), and L12 (editio_de flip would today LOSE checking).

## 1. The gap

Single-valued relations live as attributes on kind tags (unique names) and are citation-checked. Multiplex relations project as repeatable children — `<auctor ad="#x"/>` — whose NAMES COLLIDE across parents (auctor under lingua-programmandi cites persons; an auctor-shaped child elsewhere may cite differently). `super=` matches element titles globally, so the generator emits NO citation for multiplex children: `<auctor ad="#dennis_richie"/>` (typo) is green today.

## 2. Design

`super=` entries learn ONE new form: `parens/titulus` — the element titled `titulus` whose PARENT element is titled `parens`. Bare entries keep today's meaning. No grammar change (super= is already free textus, space-separated); canon.canon's nota documents the form. lib/canon.c: pass II's title-equality becomes `_super_congruit(n, t)` — split on '/', match child title AND parent title. Pass I (key collection from ad= closures) unchanged; intra= instance scoping composes unchanged.

Emission (natura_canones): the closure machinery (`_petitum_citabile`) already computes multiplex closures — it was never wired because the canon couldn't aim. Wire the multiplex relatio branch to keep citatio_ad (same modulus_requisitus rule as attributes: per-module only when closure ⊆ module), and `_citationes_scribere` emits for NC_MEMBRUM_LIBERUM relation members: `attributum="ad"`, super entries `<bearer-kebab>/<membrum-kebab>` for every bearer elementum.

**Corpus risk, deliberate:** 402 previously unchecked sites gain checking at regen — latent dangling refs in semina/hortus/fixtures become vitia. That is the point; heal or rule each.

## 3. Plan

- **T1 mechanism**: probatio_canon fixture (canon with two parents sharing a child name, different closures; scoped citatio; RED first: today the global super smears — actually today the fixture cannot even be WRITTEN correctly, assert the misbehavior then the fix). `_super_congruit` in lib/canon.c pass II. canon.canon nota on super=. Rebuild, self-judge, commit.
- **T2 emission**: natura_canones multiplex wiring + regen all; suite + corpus healing (expect semina green — its refs are self-enrolled; fixtures may need repair); count citations before/after in commit message. Sentinel: individua.canon contains a `super=".../auctor"` scoped entry; descriptor's `<auctor ad="#glossator_a"/>` probe now RED (measured hole closed — assert the vitium fires on an undeclared target).
- **T3 editio_de flip** (L12): multiplex="verum", migrate the one corpus site if any, sentinel: facing-page probe with two `<editio-de ad=>` children green AND a dangling one red.
- **T4 records**: close 01KZE65SAM (impletum with measured counts), worklogs, memory.
