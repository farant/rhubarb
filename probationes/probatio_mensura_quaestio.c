/* probatio_mensura_quaestio.c - quaestiones nominatae (analytica T5).
 *
 * Quaestio acceptationis quaeque (spec par. I) super cursus FICTOS in
 * memoria: arbor temporis (I), regenerationes iudicis (II), tendentia
 * per commissum (III), frigida ultimo transiens cum arbore et ramo
 * (IV), scripta numquam cursa (V), filtra historiae (VI), tentamina
 * commissionis cum culpa (VII, Franus 10-13), octeti per titulum
 * (VIII), tituli similes cum quaestio vacua (XII).
 * Quaestio VI (corpus fracti ultimi) per CLI in mensor fumus probatur.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "credo.h"
#include "mensura.h"
#include "diurna.h"
#include "mensura_quaestio.h"
#include <stdio.h>
#include <string.h>

/* cursum fictum addere */
interior Cursus*
_cursus (
                   Xar* omnes,
    constans character* id,
    constans character* parens,
    constans character* titulus,
                   s64  initium,
                   s64  duratio,
          CursusStatus  status,
    constans character* nota,
               Piscina* piscina)
{
    Cursus* c = (Cursus*)xar_addere(omnes);

    memset(c, ZEPHYRUM, magnitudo(Cursus));
    c->initium.id         = chorda_ex_literis(id, piscina);
    c->initium.parens     = chorda_ex_literis(parens, piscina);
    c->initium.titulus    = chorda_ex_literis(titulus, piscina);
    c->initium.initium    = initium;
    c->initium.commissum  = chorda_ex_literis("c0", piscina);
    c->initium.arbor      = chorda_ex_literis("/arbor", piscina);
    c->initium.ramus      = chorda_ex_literis("main", piscina);
    c->initium.argv       = chorda_ex_literis("", piscina);
    c->finis.id           = c->initium.id;
    c->finis.duratio_ms   = duratio;
    c->finis.exitus       = (status == CURSUS_TRANSIIT) ? ZEPHYRUM : I;
    c->finis.nota         = chorda_ex_literis(nota, piscina);
    c->status             = status;
    redde c;
}

interior Xar*
_omnes (
    Piscina* piscina)
{
    redde xar_creare(piscina, (i32)magnitudo(Cursus));
}

interior CursusFiltrum
_filtrum (vacuum)
{
    CursusFiltrum f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    redde f;
}

/* I. quo tempus ivit: R (C) -> A (LX) -> A1 (L), B (XXX); praeordo,
 * filii ordine durationis, pars permille parentis */
interior vacuum
_sectio_filii (
    CredoContextus* c)
{
            Xar* omnes = _omnes(c->piscina);
    CursusIndex* index;
            Xar* nodi;
    CursusNodus* n;

    (vacuum)_cursus(omnes, "R", "", "commissio", I, C, CURSUS_TRANSIIT,
        "",
        c->piscina);
    (vacuum)_cursus(omnes, "B", "R", "porta:b", II, XXX,
        CURSUS_TRANSIIT,
        "", c->piscina);
    (vacuum)_cursus(omnes, "A", "R", "porta:a", III, LX,
        CURSUS_TRANSIIT,
        "", c->piscina);
    (vacuum)_cursus(omnes, "A1", "A", "actio", IV, L, CURSUS_TRANSIIT,
        "",
        c->piscina);
    index = cursus_indicem_struere(omnes, c->piscina);
    CREDO_NECESSE_NON_NIHIL(index);
    nodi = cursus_quaerere_filii(index, chorda_ex_literis("R",
        c->piscina),
        ZEPHYRUM, c->piscina);
    CREDO_NECESSE_NON_NIHIL(nodi);
    CREDO_NECESSE(xar_numerus(nodi) == IV);
    n = (CursusNodus*)xar_obtinere(nodi, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(n->cursus->initium.id, chorda_ex_literis("R",
        c->piscina));
    CREDO_AEQUALIS_I32(n->pars, M);
    n = (CursusNodus*)xar_obtinere(nodi, I);
    CREDO_CHORDA_AEQUALIS(n->cursus->initium.id, chorda_ex_literis("A",
        c->piscina));
    CREDO_AEQUALIS_I32(n->profunditas, I);
    CREDO_AEQUALIS_I32(n->pars, DC);
    n = (CursusNodus*)xar_obtinere(nodi, II);
    CREDO_CHORDA_AEQUALIS(n->cursus->initium.id, chorda_ex_literis("A1",
        c->piscina));
    CREDO_AEQUALIS_I32(n->profunditas, II);
    CREDO_AEQUALIS_I32(n->pars, DCCCXXXIII);
    n = (CursusNodus*)xar_obtinere(nodi, III);
    CREDO_CHORDA_AEQUALIS(n->cursus->initium.id, chorda_ex_literis("B",
        c->piscina));
    nodi = cursus_quaerere_filii(index, chorda_ex_literis("R",
        c->piscina),
        I, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(nodi), III);
}

/* II. regenerationes iudicis: nota 'IUDICIUM' per titulum - numerus,
 * ms summa, parentes distincti; actio SANATUM non numeratur */
interior vacuum
_sectio_iudicia (
    CredoContextus* c)
{
            Xar* omnes = _omnes(c->piscina);
    CursusIndex* index;
  CursusFiltrum  f = _filtrum();
            Xar* ordines;
    CursusSumma* s;

    (vacuum)_cursus(omnes, "p1", "", "porta:radix", I, M,
        CURSUS_TRANSIIT,
        "", c->piscina);
    (vacuum)_cursus(omnes, "p2", "", "porta:silva", II, M,
        CURSUS_TRANSIIT,
        "", c->piscina);
    (vacuum)_cursus(omnes, "g1", "p1", "lectores_cocti", III, C,
        CURSUS_TRANSIIT, "IUDICIUM - x", c->piscina);
    (vacuum)_cursus(omnes, "g2", "p2", "lectores_cocti", IV, C,
        CURSUS_TRANSIIT, "IUDICIUM - x", c->piscina);
    (vacuum)_cursus(omnes, "g3", "p2", "lectores_cocti", V, C,
        CURSUS_TRANSIIT, "IUDICIUM - x", c->piscina);
    (vacuum)_cursus(omnes, "g4", "p1", "md_corpus", VI, L,
        CURSUS_TRANSIIT,
        "IUDICIUM - y", c->piscina);
    (vacuum)_cursus(omnes, "a1", "p1", "probatio_x", VII, D,
        CURSUS_TRANSIIT, "SANATUM", c->piscina);
    index    = cursus_indicem_struere(omnes, c->piscina);
    f.nota   = chorda_ex_literis("IUDICIUM", c->piscina);
    ordines  = cursus_quaerere_summa(index, &f, c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_NECESSE(xar_numerus(ordines) == II);
    s = (CursusSumma*)xar_obtinere(ordines, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(s->titulus,
        chorda_ex_literis("lectores_cocti",
        c->piscina));
    CREDO_AEQUALIS_I32(s->numerus, III);
    CREDO_AEQUALIS_S64(s->ms_summa, (s64)CCC);
    CREDO_AEQUALIS_I32(s->parentes, II);
    s = (CursusSumma*)xar_obtinere(ordines, I);
    CREDO_CHORDA_AEQUALIS(s->titulus, chorda_ex_literis("md_corpus",
        c->piscina));
}

/* III. tendentia: mediana (inferior) per commissum, ordine
 * apparitionis; fractus non numeratur */
interior vacuum
_sectio_tendentia (
    CredoContextus* c)
{
                Xar* omnes = _omnes(c->piscina);
        CursusIndex* index;
      CursusFiltrum  f = _filtrum();
                Xar* ordines;
    CursusTendentia* t;
             Cursus* k;

    k = _cursus(omnes, "x1", "", "X", X, C, CURSUS_TRANSIIT, "",
        c->piscina);
    k->initium.commissum = chorda_ex_literis("c1", c->piscina);
    k = _cursus(omnes, "x2", "", "X", XI, CXX, CURSUS_TRANSIIT, "",
        c->piscina);
    k->initium.commissum = chorda_ex_literis("c1", c->piscina);
    k = _cursus(omnes, "x3", "", "X", XII, CX, CURSUS_TRANSIIT, "",
        c->piscina);
    k->initium.commissum = chorda_ex_literis("c1", c->piscina);
    k = _cursus(omnes, "x4", "", "X", XX, CC, CURSUS_TRANSIIT, "",
        c->piscina);
    k->initium.commissum = chorda_ex_literis("c2", c->piscina);
    k = _cursus(omnes, "x5", "", "X", XXI, CCX, CURSUS_TRANSIIT, "",
        c->piscina);
    k->initium.commissum = chorda_ex_literis("c2", c->piscina);
    k = _cursus(omnes, "x6", "", "X", XXII, CMXCIX, CURSUS_FRACTUS, "",
        c->piscina);
    k->initium.commissum = chorda_ex_literis("c2", c->piscina);
    (vacuum)_cursus(omnes, "y1", "", "Y", XXIII, V, CURSUS_TRANSIIT, "",
        c->piscina);
    /* SEMANTICA (T5, ex datis veris): cursus INTRA commissionem quae
     * commissum PEPERIT sub commisso illo numeratur (non sub parente
     * cuius arbor sordida erat); cursus sordidus extra commissionem
     * sub 'parens+' (mutationes nondum commissae super parentem) */
    (vacuum)_cursus(omnes, "cm", "", "commissio", XXX, M,
        CURSUS_TRANSIIT,
        "commissum novum1", c->piscina);
    k = _cursus(omnes, "x7", "cm", "X", XXXI, CCC, CURSUS_TRANSIIT, "",
        c->piscina);
    k->initium.commissum  = chorda_ex_literis("c2", c->piscina);
    k->initium.sordidum   = VERUM;
    k = _cursus(omnes, "x8", "", "X", XL, CD, CURSUS_TRANSIIT, "",
        c->piscina);
    k->initium.commissum = chorda_ex_literis("c2", c->piscina);
    k->initium.sordidum = VERUM;
    index = cursus_indicem_struere(omnes, c->piscina);
    f.titulus = chorda_ex_literis("X", c->piscina);
    ordines = cursus_quaerere_tendentia(index, &f, c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_NECESSE(xar_numerus(ordines) == IV);
    t = (CursusTendentia*)xar_obtinere(ordines, II);
    CREDO_CHORDA_AEQUALIS(t->commissum, chorda_ex_literis("novum1",
        c->piscina));
    CREDO_VERUM(t->per_commissionem);
    CREDO_AEQUALIS_S64(t->mediana_ms, (s64)CCC);
    t = (CursusTendentia*)xar_obtinere(ordines, III);
    CREDO_CHORDA_AEQUALIS(t->commissum, chorda_ex_literis("c2+",
        c->piscina));
    CREDO_FALSUM(t->per_commissionem);
    CREDO_AEQUALIS_S64(t->mediana_ms, (s64)CD);
    t = (CursusTendentia*)xar_obtinere(ordines, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(t->commissum, chorda_ex_literis("c1",
        c->piscina));
    CREDO_AEQUALIS_I32(t->numerus, III);
    CREDO_AEQUALIS_S64(t->mediana_ms, (s64)CX);
    CREDO_AEQUALIS_S64(t->minima_ms, (s64)C);
    CREDO_AEQUALIS_S64(t->maxima_ms, (s64)CXX);
    t = (CursusTendentia*)xar_obtinere(ordines, I);
    CREDO_CHORDA_AEQUALIS(t->commissum, chorda_ex_literis("c2",
        c->piscina));
    CREDO_AEQUALIS_I32(t->numerus, II);
    CREDO_AEQUALIS_S64(t->mediana_ms, (s64)CC);
}

/* IV. frigida ultimo TRANSIENS: arbor et ramus eius cursus, non
 * fracti posterioris */
interior vacuum
_sectio_ultimum (
    CredoContextus* c)
{
            Xar* omnes = _omnes(c->piscina);
    CursusIndex* index;
  CursusFiltrum  f = _filtrum();
            Xar* ordines;
         Cursus* k;
constans Cursus* u;

    (vacuum)_cursus(omnes, "f1", "", "frigida", X, C, CURSUS_TRANSIIT,
        "",
        c->piscina);
    (vacuum)_cursus(omnes, "f2", "", "frigida", XX, C, CURSUS_FRACTUS,
        "",
        c->piscina);
    k = _cursus(omnes, "f3", "", "frigida", XXX, C, CURSUS_TRANSIIT, "",
        c->piscina);
    k->initium.arbor = chorda_ex_literis("/tmp/frigida.3", c->piscina);
    k->initium.ramus = chorda_ex_literis("frigida-abc", c->piscina);
    (vacuum)_cursus(omnes, "f4", "", "frigida", XL, C, CURSUS_FRACTUS,
        "",
        c->piscina);
    index      = cursus_indicem_struere(omnes, c->piscina);
    f.titulus  = chorda_ex_literis("frigida", c->piscina);
    f.status   = CURSUS_MASCULA_TRANSIIT;
    f.numerus  = I;
    ordines    = cursus_quaerere_historia(index, &f, c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_NECESSE(xar_numerus(ordines) == I);
    u = *(constans Cursus**)xar_obtinere(ordines, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(u->initium.id, chorda_ex_literis("f3",
        c->piscina));
    CREDO_CHORDA_AEQUALIS(u->initium.arbor, chorda_ex_literis(
        "/tmp/frigida.3", c->piscina));
    CREDO_CHORDA_AEQUALIS(u->initium.ramus, chorda_ex_literis(
        "frigida-abc", c->piscina));
}

/* V. numquam: titulus aequalis aut argv primum via (aut .../via) */
interior vacuum
_sectio_numquam (
    CredoContextus* c)
{
            Xar* omnes = _omnes(c->piscina);
            Xar* universum = xar_creare(c->piscina,
                (i32)magnitudo(chorda));
    CursusIndex* index;
            Xar* ordines;
         Cursus* k;

    *(chorda*)xar_addere(universum) = chorda_ex_literis("tools/a.sh",
        c->piscina);
    *(chorda*)xar_addere(universum) = chorda_ex_literis("tools/b.sh",
        c->piscina);
    *(chorda*)xar_addere(universum) = chorda_ex_literis("fumus/c",
        c->piscina);
    /* limes viae: argv '.../tools/ab.sh' scriptum 'b.sh' NON spectat */
    *(chorda*)xar_addere(universum) = chorda_ex_literis("b.sh",
        c->piscina);
    k = _cursus(omnes, "k1", "", "scriptum/alia", I, I, CURSUS_TRANSIIT,
        "",
        c->piscina);
    k->initium.argv = chorda_ex_literis("/arbor/tools/a.sh -x",
        c->piscina);
    (vacuum)_cursus(omnes, "k2", "", "fumus/c", II, I, CURSUS_FRACTUS,
        "",
        c->piscina);
    k = _cursus(omnes, "k3", "", "q", III, I, CURSUS_TRANSIIT, "",
        c->piscina);
    k->initium.argv = chorda_ex_literis("/arbor/tools/ab.sh",
        c->piscina);
    index    = cursus_indicem_struere(omnes, c->piscina);
    ordines  = cursus_quaerere_numquam(index, universum, c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_NECESSE(xar_numerus(ordines) == II);
    CREDO_CHORDA_AEQUALIS(*(chorda*)xar_obtinere(ordines, ZEPHYRUM),
        chorda_ex_literis("tools/b.sh", c->piscina));
    CREDO_CHORDA_AEQUALIS(*(chorda*)xar_obtinere(ordines, I),
        chorda_ex_literis("b.sh", c->piscina));
}

/* VI. filtra historiae: arbor, ramus, post, numerus, mascula fracti
 * (interruptus inclusus) */
interior vacuum
_sectio_filtra (
    CredoContextus* c)
{
            Xar* omnes = _omnes(c->piscina);
    CursusIndex* index;
  CursusFiltrum  f = _filtrum();
         Cursus* k;

    (vacuum)_cursus(omnes, "h1", "", "t", X, I, CURSUS_TRANSIIT, "",
        c->piscina);
    k = _cursus(omnes, "h2", "", "t", XX, I, CURSUS_FRACTUS, "",
        c->piscina);
    k->initium.arbor = chorda_ex_literis("/alia", c->piscina);
    k = _cursus(omnes, "h3", "", "t", XXX, I, CURSUS_INTERRUPTUS, "",
        c->piscina);
    k->initium.ramus = chorda_ex_literis("ramus-b", c->piscina);
    (vacuum)_cursus(omnes, "h4", "", "u", XL, I, CURSUS_TRANSIIT, "",
        c->piscina);
    index = cursus_indicem_struere(omnes, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(cursus_quaerere_historia(index, &f,
        c->piscina)), IV);
    CREDO_CHORDA_AEQUALIS((*(constans Cursus**)xar_obtinere(
        cursus_quaerere_historia(index, &f, c->piscina), ZEPHYRUM))
        ->initium.id, chorda_ex_literis("h4", c->piscina));
    f.status = CURSUS_MASCULA_FRACTUS | CURSUS_MASCULA_INTERRUPTUS;
    CREDO_AEQUALIS_I32(xar_numerus(cursus_quaerere_historia(index, &f,
        c->piscina)), II);
    f.status  = ZEPHYRUM;
    f.arbor   = chorda_ex_literis("/alia", c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(cursus_quaerere_historia(index, &f,
        c->piscina)), I);
    f.arbor = chorda_ex_literis("", c->piscina);
    f.ramus = chorda_ex_literis("ramus-b", c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(cursus_quaerere_historia(index, &f,
        c->piscina)), I);
    f.ramus    = chorda_ex_literis("", c->piscina);
    f.post_ms  = (s64)XXV;
    CREDO_AEQUALIS_I32(xar_numerus(cursus_quaerere_historia(index, &f,
        c->piscina)), II);
    f.post_ms = (s64)ZEPHYRUM;
    f.numerus = III;
    CREDO_AEQUALIS_I32(xar_numerus(cursus_quaerere_historia(index, &f,
        c->piscina)), III);
}

/* VII. commissiones: transiens (commissum ex nota), fracta cum culpa
 * PROFUNDISSIMA (sectio sub membro sub porta), fracta ad git commit;
 * recentissima primum */
interior vacuum
_sectio_commissiones (
    CredoContextus* c)
{
                Xar* omnes = _omnes(c->piscina);
        CursusIndex* index;
      CursusFiltrum  f = _filtrum();
                Xar* ordines;
    CursusCommissio* t;

    (vacuum)_cursus(omnes, "c1", "", "commissio", X, C, CURSUS_TRANSIIT,
        "commissum abc123", c->piscina);
    (vacuum)_cursus(omnes, "c2", "", "commissio", XX, C, CURSUS_FRACTUS,
        "SilvaError: porta radix non sana", c->piscina);
    (vacuum)_cursus(omnes, "c2l", "c2", "phasis:lint", XXI, I,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "c2p", "c2", "porta:radix", XXII, L,
        CURSUS_FRACTUS, "probationes_radicis: 300/301", c->piscina);
    (vacuum)_cursus(omnes, "c2m", "c2p",
        "probationes_radicis/probatio_x",
        XXIII, X, CURSUS_FRACTUS, "FRACTUM", c->piscina);
    (vacuum)_cursus(omnes, "c2s", "c2m",
        "probationes_radicis/probatio_x#beta", XXIV, V, CURSUS_FRACTUS,
        "FRACTA 1/2", c->piscina);
    (vacuum)_cursus(omnes, "c3", "", "commissio", XXX, C,
        CURSUS_FRACTUS,
        "SilvaError: git commit rc=1", c->piscina);
    (vacuum)_cursus(omnes, "c3g", "c3", "phasis:git commit", XXXI, I,
        CURSUS_FRACTUS, "", c->piscina);
    index    = cursus_indicem_struere(omnes, c->piscina);
    ordines  = cursus_quaerere_commissiones(index, &f, c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_NECESSE(xar_numerus(ordines) == III);
    t = (CursusCommissio*)xar_obtinere(ordines, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(t->commissio->initium.id,
        chorda_ex_literis("c3",
        c->piscina));
    CREDO_NECESSE_NON_NIHIL(t->culpa);
    CREDO_CHORDA_AEQUALIS(t->culpa->initium.titulus, chorda_ex_literis(
        "phasis:git commit", c->piscina));
    t = (CursusCommissio*)xar_obtinere(ordines, I);
    CREDO_NECESSE_NON_NIHIL(t->culpa);
    CREDO_CHORDA_AEQUALIS(t->culpa->initium.titulus, chorda_ex_literis(
        "probationes_radicis/probatio_x#beta", c->piscina));
    t = (CursusCommissio*)xar_obtinere(ordines, II);
    CREDO_VERUM(t->culpa == NIHIL);
    CREDO_CHORDA_AEQUALIS(t->commissum, chorda_ex_literis("abc123",
        c->piscina));
}

/* VIII. discus: octeti per titulum, deleta non numerata */
interior vacuum
_sectio_discus (
    CredoContextus* c)
{
             Xar* introitus = xar_creare(c->piscina,
                 (i32)magnitudo(DiurnaIntroitus));
 DiurnaIntroitus* i;
             Xar* ordines;
     CursusSumma* s;

    i = (DiurnaIntroitus*)xar_addere(introitus);
    memset(i, ZEPHYRUM, magnitudo(*i));
    i->titulus  = chorda_ex_literis("t1", c->piscina);
    i->mensura  = (s64)C;
    i           = (DiurnaIntroitus*)xar_addere(introitus);
    memset(i, ZEPHYRUM, magnitudo(*i));
    i->titulus  = chorda_ex_literis("t1", c->piscina);
    i->mensura  = (s64)CC;
    i           = (DiurnaIntroitus*)xar_addere(introitus);
    memset(i, ZEPHYRUM, magnitudo(*i));
    i->titulus  = chorda_ex_literis("t1", c->piscina);
    i->mensura  = (s64)L;
    i->forma    = DIURNA_DELETUM;
    i           = (DiurnaIntroitus*)xar_addere(introitus);
    memset(i, ZEPHYRUM, magnitudo(*i));
    i->titulus  = chorda_ex_literis("t2", c->piscina);
    i->mensura  = (s64)M;
    ordines     = cursus_quaerere_discus(introitus, c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_NECESSE(xar_numerus(ordines) == II);
    s = (CursusSumma*)xar_obtinere(ordines, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(s->titulus, chorda_ex_literis("t2",
        c->piscina));
    CREDO_AEQUALIS_S64(s->octeti, (s64)M);
    s = (CursusSumma*)xar_obtinere(ordines, I);
    CREDO_AEQUALIS_S64(s->octeti, (s64)CCC);
    CREDO_AEQUALIS_I32(s->numerus, II);
}

/* IX. statistica per praefixum: sectiones membri (#) solae, mediana
 * transeuntium, maxima, fracti */
interior vacuum
_sectio_statistica (
    CredoContextus* c)
{
                 Xar* omnes = _omnes(c->piscina);
         CursusIndex* index;
       CursusFiltrum  f = _filtrum();
                 Xar* ordines;
    CursusStatistica* s;

    (vacuum)_cursus(omnes, "m1", "", "probatio_g", I, CC,
        CURSUS_TRANSIIT,
        "", c->piscina);
    (vacuum)_cursus(omnes, "s1", "m1", "probatio_g#alpha", II, XX,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "s2", "m1", "probatio_g#beta", III, V,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "s3", "", "probatio_g#alpha", IV, XL,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "s4", "", "probatio_g#alpha", V, XXX,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "s5", "", "probatio_g#alpha", VI, D,
        CURSUS_FRACTUS, "", c->piscina);
    (vacuum)_cursus(omnes, "s6", "", "probatio_h#alpha", VII, M,
        CURSUS_TRANSIIT, "", c->piscina);
    index        = cursus_indicem_struere(omnes, c->piscina);
    f.praefixum  = chorda_ex_literis("probatio_g#", c->piscina);
    ordines      = cursus_quaerere_statistica(index, &f, c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_NECESSE(xar_numerus(ordines) == II);
    s = (CursusStatistica*)xar_obtinere(ordines, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(s->titulus,
        chorda_ex_literis("probatio_g#alpha",
        c->piscina));
    CREDO_AEQUALIS_I32(s->numerus, IV);
    CREDO_AEQUALIS_I32(s->fracti, I);
    CREDO_AEQUALIS_S64(s->mediana_ms, (s64)XXX);
    CREDO_AEQUALIS_S64(s->maxima_ms, (s64)XL);
}

/* X. lentescentes: A constanter tardior (ultimi III omnes supra) ->
 * nominatur; B ictus unus -> non; C historia brevis -> non */
interior vacuum
_sectio_lentescentes (
    CredoContextus* c)
{
                 Xar* omnes = _omnes(c->piscina);
         CursusIndex* index;
       CursusFiltrum  f = _filtrum();
                 Xar* ordines;
    CursusLentescens* l;
           character  id[XVI];
                 s64  a[VI] = { C, CX, C, CL, CLX, CLV };
                 s64  b[VI] = { C, C, C, D, C, C };
                 i32  k;

    per (k = ZEPHYRUM; k < (i32)VI; k++)
    {
        sprintf(id, "a%d", (integer)k);
        (vacuum)_cursus(omnes, id, "", "A", (s64)(k + I), a[k],
            CURSUS_TRANSIIT, "", c->piscina);
        sprintf(id, "b%d", (integer)k);
        (vacuum)_cursus(omnes, id, "", "B", (s64)(k + I), b[k],
            CURSUS_TRANSIIT, "", c->piscina);
    }
    (vacuum)_cursus(omnes, "c1", "", "C", I, C, CURSUS_TRANSIIT, "",
        c->piscina);
    (vacuum)_cursus(omnes, "c2", "", "C", II, D, CURSUS_TRANSIIT, "",
        c->piscina);
    index = cursus_indicem_struere(omnes, c->piscina);
    ordines = cursus_quaerere_lentescentes(index, &f, III, MCCL,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_NECESSE(xar_numerus(ordines) == I);
    l = (CursusLentescens*)xar_obtinere(ordines, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(l->titulus, chorda_ex_literis("A",
        c->piscina));
    CREDO_AEQUALIS_I32(l->priores, III);
    CREDO_AEQUALIS_S64(l->mediana_prior_ms, (s64)C);
    CREDO_AEQUALIS_S64(l->mediana_recens_ms, (s64)CLV);
    CREDO_AEQUALIS_I32(l->supra, III);
    CREDO_AEQUALIS_I32(l->ratio_permille, MDL);
}

/* XI. lentescentes, regulae singulae: fenestra V - E ictibus (ratio
 * transit, constantia non: III/V supra) -> non; F constanter tardior ->
 * nominatur; G historia brevis (IV cursus, fenestra III: priores I) ->
 * non */
interior vacuum
_sectio_lentescentes_regulae (
    CredoContextus* c)
{
                 Xar* omnes = _omnes(c->piscina);
         CursusIndex* index;
       CursusFiltrum  f = _filtrum();
                 Xar* ordines;
           character  id[XVI];
                 s64  e[X]   = { C, C, C, C, C, CD, C, CD, C, CD };
                 s64  g[IV]  = { C, D, D, D };
                 i32  k;

    per (k = ZEPHYRUM; k < (i32)X; k++)
    {
        sprintf(id, "e%d", (integer)k);
        (vacuum)_cursus(omnes, id, "", "E", (s64)(k + I), e[k],
            CURSUS_TRANSIIT, "", c->piscina);
        sprintf(id, "f%d", (integer)k);
        (vacuum)_cursus(omnes, id, "", "F", (s64)(k + I), k < (i32)V ? C
            : CCC, CURSUS_TRANSIIT, "", c->piscina);
    }
    per (k = ZEPHYRUM; k < (i32)IV; k++)
    {
        sprintf(id, "g%d", (integer)k);
        (vacuum)_cursus(omnes, id, "", "G", (s64)(k + I), g[k],
            CURSUS_TRANSIIT, "", c->piscina);
    }
    index = cursus_indicem_struere(omnes, c->piscina);
    ordines = cursus_quaerere_lentescentes(index, &f, V, MCCL,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_NECESSE(xar_numerus(ordines) == I);
    CREDO_CHORDA_AEQUALIS(((CursusLentescens*)xar_obtinere(ordines,
        ZEPHYRUM))->titulus, chorda_ex_literis("F", c->piscina));
    f.titulus = chorda_ex_literis("G", c->piscina);
    ordines = cursus_quaerere_lentescentes(index, &f, III, MCCL,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_AEQUALIS_I32(xar_numerus(ordines), ZEPHYRUM);
}

/* XII. nihil congruit (2026-10-15): titulus nudus -> similes (intra
 * titulum, segmentum ultimum), ordine numeri, maximi; titulus notus
 * filtris ceteris exclusus -> noti; nihil simile -> vacuum; sine
 * filtro tituli -> NIHIL */
interior vacuum
_sectio_similes (
    CredoContextus* c)
{
            Xar* omnes = _omnes(c->piscina);
    CursusIndex* index;
  CursusFiltrum  f = _filtrum();
            Xar* ordines;
    CursusSumma* s;
            i32  noti = (i32)XCIX;
            i32  k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        (vacuum)_cursus(omnes, "r", "",
            "probationes_radicis/probatio_t",
            (s64)(k + I), X, CURSUS_TRANSIIT, "", c->piscina);
    }
    (vacuum)_cursus(omnes, "m", "", "probationes_md/probatio_t", IV, X,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "g", "", "pythonica#probatio_t", V, X,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "x", "", "porta:aliena", VI, X,
        CURSUS_TRANSIIT, "", c->piscina);
    index = cursus_indicem_struere(omnes, c->piscina);

    /* titulus nudus: III similes (segmentum ultimum post '/' et '#'),
     * numero descendente, maximi II */
    f.titulus  = chorda_ex_literis("probatio_t", c->piscina);
    ordines    = cursus_quaerere_similes(index, &f, II, &noti,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_AEQUALIS_I32(noti, ZEPHYRUM);
    CREDO_NECESSE(xar_numerus(ordines) == II);
    s = (CursusSumma*)xar_obtinere(ordines, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(s->titulus, chorda_ex_literis(
        "probationes_radicis/probatio_t", c->piscina));
    CREDO_AEQUALIS_I32(s->numerus, III);
    /* par numeri (I et I): ordo tituli - 'probationes_md' prius */
    s = (CursusSumma*)xar_obtinere(ordines, I);
    CREDO_CHORDA_AEQUALIS(s->titulus, chorda_ex_literis(
        "probationes_md/probatio_t", c->piscina));

    /* segmentum ultimum solum (quaesitum NON intra titulum): composita
     * alia, probatio eadem */
    f          = _filtrum();
    f.titulus  = chorda_ex_literis("probationes_alia/probatio_t",
        c->piscina);
    ordines    = cursus_quaerere_similes(index, &f, ZEPHYRUM, &noti,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_AEQUALIS_I32(xar_numerus(ordines), III);

    /* quaesitum intra titulum (praefixum partiale) */
    f            = _filtrum();
    f.praefixum  = chorda_ex_literis("aliena", c->piscina);
    ordines    = cursus_quaerere_similes(index, &f, ZEPHYRUM, &noti,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_AEQUALIS_I32(xar_numerus(ordines), I);

    /* titulus notus, status exclusit: noti = III, nulli similes */
    f          = _filtrum();
    f.titulus  = chorda_ex_literis("probationes_radicis/probatio_t",
        c->piscina);
    f.status   = CURSUS_MASCULA_FRACTUS;
    ordines    = cursus_quaerere_similes(index, &f, III, &noti,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_AEQUALIS_I32(noti, III);
    CREDO_AEQUALIS_I32(xar_numerus(ordines), ZEPHYRUM);

    /* nihil simile */
    f          = _filtrum();
    f.titulus  = chorda_ex_literis("nihil_tale", c->piscina);
    ordines    = cursus_quaerere_similes(index, &f, III, &noti,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(ordines);
    CREDO_AEQUALIS_I32(noti, ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(ordines), ZEPHYRUM);

    /* sine filtro tituli: NIHIL (nihil nominandum) */
    f = _filtrum();
    CREDO_NIHIL(cursus_quaerere_similes(index, &f, III, &noti,
        c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I quo tempus ivit", _sectio_filii, NIHIL, NIHIL, NIHIL, NIHIL },
    { "II regenerationes iudicis", _sectio_iudicia, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "III tendentia per commissum", _sectio_tendentia, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "IV ultimum transiens", _sectio_ultimum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "V numquam cursa", _sectio_numquam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "VI filtra historiae", _sectio_filtra, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "VII commissiones et culpa", _sectio_commissiones, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "VIII discus", _sectio_discus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "IX statistica sectionum", _sectio_statistica, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "X lentescentes", _sectio_lentescentes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "XI lentescentes: constantia et historia",
        _sectio_lentescentes_regulae,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "XII nihil congruit: tituli similes", _sectio_similes, NIHIL,
        NIHIL,
      NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mensura_quaestio", SECTIONES);
}
