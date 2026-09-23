> **ARCHIVUM (2026-09-23).** Nulla probatio hoc aurum legebat: munus eius
> (extractor mensuratus ANTE differentiam latam) a
> `crusta/instrumenta/differentia_exemplariorum.sh` (CCXXXIV plagulae,
> corpus congelatum) impletum est. Et instantanea super TRIBUS plagulis
> VIVIS erat (doctrina 01M365VXAF). Servatum, non deletum.

# aurum.tsv — PHOTOGRAPHIA extractoris Pythonis, non specificatio

Generatum 2026-09-18 ex `silva.exemplaria` + `congruentiae_minus`
super tribus plagulis domus (`compile_tests.sh`, `crusta/compile_probationes.sh`,
`gesta/compile_probationes.sh`) cum regula `crusta/lintrum/nt-aequalitas.stml`.

**HOC EST QUOD PYTHON HODIE FACIT, non quod extractor facere DEBET.**
Distinctio momenti est: si extractor C ab hoc differt, quaestio prior
est UTER RECTUS SIT — specificatio (`sedes-multiplices-spec.md` par.
IX, vocabularium `<relatum>`) iudicat, non haec plagula.

Porta vera arcus est differentia super CCXXXII plagulas (planum,
opus IV); haec tres sunt ut extractor ANTE eam mensurari possit.

Regenerare (cum causa nominata in commissione):

    python3 -c "import sys; sys.path.insert(0,'pythonica'); import silva; \
      x=silva.exemplaria(['compile_tests.sh','crusta/compile_probationes.sh','gesta/compile_probationes.sh'], \
        'crusta/lintrum/nt-aequalitas.stml'); \
      c=silva.congruentiae_minus(x.congruentiae,'nt-omnes','nt-negata', \
        lint='nt-aequalitas'); \
      print(silva.diagnostica_tsv(silva.diagnostica_ex_congruentiis(c)), end='')" \
      > materia/probationes/fixa/exemplaria/aurum.tsv

## Cur HAE tres plagulae

Electio prima (`tools/vexilla.sh`, `gesta/fori.sh`, `crusta/coctum.sh`)
ordines ZERO reddidit: transitus arcus prioris domum purgavit, ergo
plagulae illae candidatos nullos ferunt. **Aurum vacuum nihil
mensurat** - ergo plagulae electae sunt quae candidatos REVERA ferunt
(IV ordines), et numerus ille in probatione FIGITUR ne aurum tacite in
vacuum relabatur.
