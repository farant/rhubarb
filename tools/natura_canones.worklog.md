# natura_canones worklog

## 2026-08-06/07 — Task 4: the monolith, and why most relations do NOT become citations

Two decisions here were measured rather than reasoned-from-the-spec, and both
went against the shape the task brief assumed. Recording them because the
evidence is cheap to lose and expensive to re-derive.

### 1. A citation may only target a LEAF kind

The brief's worked example was `<rosa-canina auctor-nominis="...">` citing
`persona/nomen`. That example, implemented literally, ships a canon that
**rejects correct documents.**

`lib/canon.c` collects citation keys from nodes whose title matches
`ad_elementum` **exactly** (`chorda_aequalis(n->titulus, ci->ad_elementum)`).
There is no element inheritance. But `persona` has 14 `individuum` entries in
natura, and in the monolith every one of them is its own element. So the
entirely reasonable document

```xml
<rosa-canina nomen="rosa-mea" auctor-nominis="linnaeus-noster"/>
<carl-linnaeus nomen="linnaeus-noster"/>
```

has its key under `<carl-linnaeus>`, not `<persona>`, and the citation fires
`CITATIO_IRRITA`. Measured, not predicted — I built the lax canon and ran it:

```
/tmp/subgenus.laxum:2: <rosa-canina> citatio non resoluta
    (clavis absens in scopo): linnaeus-noster
```

The same document under the shipped monolith exits 0.

So a relation becomes a citation only when its target has **no descendants**
(no subgenera, no dictionary entries). Direct children suffice for the test —
a child genus is already a distinct element regardless of its own depth.

Corpus census (640 relation declarations, apparatus-folded, inheritance
included):

| | count |
|---|---|
| `ad="*"` or absent — no key possible | 157 |
| target has descendants — would false-positive | 366 |
| target is a leaf — citation is exact | 108 |
| target unresolvable | 7 |
| target is a dictionary entry (always a leaf) | 2 |

That yields **217 citations** over the 561 elements (higher than 108 because
dictionary entries inherit their genus's relations and get their own).

This is the house rule *porta quae falso clamat neglegitur* applied literally:
57% coverage that is always right beats 100% coverage that rejects real
documents.

**Multiplex relations get no citation at all.** Their citing node is the
relation-child element, and `super=` matches on node title only — it does not
respect `intra=`. Relation-child names are shared across kinds with *different*
targets (`continet` alone has 7), so one citation would falsely fire on the
other 6. Genera are scoped by `intra=`; citations are not.

### 2. The reference side had to widen with the identity

Task 3 typed the identity `nomen=` as `compositum` (nomen + hyphen). Every
reference-shaped attribute was still `nomen`. That combination means an
identity spelled `via-lata-x` is **legal to declare and impossible to
reference**. Proven by narrowing the shipped canon back to `nomen` and
re-running the passing document:

```
valor generi attributi non congruit: inscriptio-eius
```

So in the monolith all relation-origin attributes are `compositum`, not only
the cited ones — in the monolith any relation value may name an in-document
identity, and the canon *limits, it does not demand* (spec §3.4). Forbidding a
hyphen where nothing is resolved would be strictness with no purchase.

`certitudo=` and `fons=` stay `nomen` deliberately: they key into natura's
`<fontes clavis=>`, not into the document. Widening them would be cargo-cult.

Per-module canons keep `nomen` throughout — there the value is a natura key in
snake (§4.4) and the loader resolves it across files.

### 3. Emitter changes, and the contract

`_canonem_emittere` was contracted unchanged. Four changes were unavoidable;
all are model-driven and **inert** for the per-module path — `planta.canon`
regenerates byte-identical to the committed file, and all 33 module canons
judge clean with zero citations and no `compositum` outside the identity.

- `praefatio` parameter. The brief also required a plain-language caveat in the
  generated header, and those two requirements cannot both hold: the signum
  needs `GENERATUM` **and** `NOLI MANU MUTARE` inside `head -3`, so threading
  prose through `fons=` pushes the signum out of the window and silently
  disarms `generata-custos.sh`. A separate comment after the signum never
  moves it.
- Citation emission driven by `NcMembrum.citatio_ad`. The *decision* stays in
  `natura_canones.c`; the emitter only writes what the model already says.
- Multiplex relation `ad=` genus read from `m->genus_valoris` instead of a
  hardcoded `"nomen"` — otherwise the monolith contradicts itself, with simple
  relations accepting a hyphen and multiplex ones not.
- `_generi_congruit` now allows `-` under `compositum`. It is documented as a
  mirror of `lib/canon.c valor_congruit`; `compositum` was previously
  unreachable there and silently judged as `nomen`. Now that we emit it, the
  mirror has to cover it.

### 4. Traps hit

- **My own prose broke a count.** The preface originally contained the literal
  text `<citatio>`; `grep -c '<citatio'` then reported 218 against the tool's
  217. Element names in generated comments are now written without angle
  brackets, and the comment says why.
- The brief's fixture used `nomen="rosa ad murum"` — spaces are illegal under
  `compositum`. Spec §5.3 records this exact example being wrong twice before;
  the brief regressed it a third time. A hand-written example is not evidence.

### 5. Known gaps handed forward

- `tools/natura_canones.sh` **does not exist**, but every generated canon's
  header says `Regenera: ./tools/natura_canones.sh`. Pre-existing from Task 3;
  the freshness gate of spec §5.5 needs it.
- No `unicitas` over `nomen=` is emitted, though spec §5.3 calls for one.
  Citations still resolve without it (duplicate keys just overwrite), so this
  is a correctness gap in identity, not in citation.
- There is no `probatio_natura_canones.c`. All verification so far is by hand.
