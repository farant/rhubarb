/* probatio_mensura_quaestio.c - quaestiones nominatae (analytica T5).
 *
 * Quaestio acceptationis quaeque (spec par. I) super cursus FICTOS in
 * memoria: arbor temporis (I), regenerationes iudicis (II), tendentia
 * per commissum (III), frigida ultimo transiens cum arbore et ramo
 * (IV), scripta numquam cursa (V), filtra historiae (VI), tentamina
 * commissionis cum culpa (VII, Franus 10-13), octeti per titulum
 * (VIII), tituli similes cum quaestio vacua (XII), sectiones memoria
 * gravissimae (XV, credo memoria T3).
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

/* mensuram fictam addere (facta cursuum, colloquia T8) */
interior vacuum
_factum (
                   Xar* mensurae,
    constans character* parens,
    constans character* titulus,
                   i32  valor,
               Piscina* piscina)
{
    Mensura* m = (Mensura*)xar_addere(mensurae);

    memset(m, ZEPHYRUM, magnitudo(Mensura));
    m->id       = chorda_ex_literis("m", piscina);
    m->parens   = chorda_ex_literis(parens, piscina);
    m->titulus  = chorda_ex_literis(titulus, piscina);
    m->valor    = (duplex)valor;
    m->unitas   = chorda_ex_literis("numerus", piscina);
}

interior constans CursusCurrens*
_currens (
    constans Xar* currentes,
             i32  k)
{
    redde (constans CursusCurrens*)xar_obtinere(currentes, k);
}

/* XIII. currentes (colloquia T8d): aperti ut arbor, radices
 * recentissimae primum; custos, consilium, progressus post ambitum
 * ULTIMUM, filii finiti (portae sub commissione; membra non IUDICIUM
 * sub porta), historia V ultimorum transeuntium; filtrum arboris */
interior vacuum
_sectio_currentes (
    CredoContextus* c)
{
                       Xar* omnes = _omnes(c->piscina);
                       Xar* mensurae = xar_creare(c->piscina,
                           (i32)magnitudo(Mensura));
               CursusIndex* index;
             CursusFiltrum  f = _filtrum();
                       Xar* currentes;
    constans CursusCurrens* r;
                    Cursus* alienus;

    /* historia porta:radix: H0 vetustissimus, H7 fractus - ultimi V
     * transeuntes H2..H6 (XX..LX) */
    (vacuum)_cursus(omnes, "H0", "", "porta:radix", V, V,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "H1", "", "porta:radix", X, X,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "H2", "", "porta:radix", XX, XX,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "H3", "", "porta:radix", XXX, XXX,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "H4", "", "porta:radix", XL, XL,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "H5", "", "porta:radix", L, L,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "H6", "", "porta:radix", LX, LX,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "H7", "", "porta:radix", LXX, CMXCIX,
        CURSUS_FRACTUS, "", c->piscina);
    /* commissio aperta R: P1 finita, Ph phasis finita (non porta), P2
     * aperta cum membris m1 (sanum), m2 (fractum), j (IUDICIUM) */
    (vacuum)_cursus(omnes, "R", "", "commissio", M, ZEPHYRUM,
        CURSUS_INTERRUPTUS, "", c->piscina);
    (vacuum)_cursus(omnes, "Ph", "R", "phasis:fabrica", M + I, V,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "P1", "R", "porta:aedilis", M + II, X,
        CURSUS_TRANSIIT, "PORTA AEDILIS", c->piscina);
    (vacuum)_cursus(omnes, "P2", "R", "porta:radix", M + III, ZEPHYRUM,
        CURSUS_INTERRUPTUS, "", c->piscina);
    (vacuum)_cursus(omnes, "m1", "P2", "probationes_radicis/a", M + IV,
        I, CURSUS_TRANSIIT, "SANATUM", c->piscina);
    (vacuum)_cursus(omnes, "m2", "P2", "probationes_radicis/b", M + V,
        I, CURSUS_FRACTUS, "FRACTUM - b", c->piscina);
    (vacuum)_cursus(omnes, "j", "P2", "bearssl_amalgama", M + VI, I,
        CURSUS_TRANSIIT, "IUDICIUM", c->piscina);
    /* m3 secundum fractum (T10a): fracta spatio iuncta */
    (vacuum)_cursus(omnes, "m3", "P2", "probationes_radicis/c", M + VII,
        I, CURSUS_FRACTUS, "FRACTUM - c", c->piscina);
    /* Q radix aperta sine custode; X orbus (parens F finitus); S
     * finitus; A apertus arboris alienae */
    (vacuum)_cursus(omnes, "Q", "", "porta:x", MM, ZEPHYRUM,
        CURSUS_INTERRUPTUS, "", c->piscina);
    (vacuum)_cursus(omnes, "F", "", "commissio", MM + C, V,
        CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "X", "F", "porta:orba", MMM, ZEPHYRUM,
        CURSUS_INTERRUPTUS, "", c->piscina);
    (vacuum)_cursus(omnes, "S", "", "porta:s", MMM + C, V,
        CURSUS_TRANSIIT, "", c->piscina);
    alienus = _cursus(omnes, "A", "", "commissio", MMM + CC, ZEPHYRUM,
        CURSUS_INTERRUPTUS, "", c->piscina);
    alienus->initium.arbor = chorda_ex_literis("/alibi", c->piscina);

    _factum(mensurae, "R", MENSURA_CUSTOS, I, c->piscina);
    _factum(mensurae, "R", MENSURA_CUSTOS, MMMDCCCXCVIII, c->piscina);
    _factum(mensurae, "R", MENSURA_PORTAE, III, c->piscina);
    _factum(mensurae, "P2", MENSURA_CUSTOS, MMMDCCCXCVIII, c->piscina);
    _factum(mensurae, "P2", MENSURA_AMBITUS, V, c->piscina);
    _factum(mensurae, "P2", MENSURA_IUDICATA, V, c->piscina);
    _factum(mensurae, "P2", MENSURA_AMBITUS, CCCLI, c->piscina);
    _factum(mensurae, "P2", MENSURA_IUDICATA, C, c->piscina);
    _factum(mensurae, "P2", MENSURA_IUDICATA, CXX, c->piscina);
    _factum(mensurae, "Q", MENSURA_AMBITUS, VII, c->piscina);
    _factum(mensurae, "Q", MENSURA_IUDICATA, VII, c->piscina);
    _factum(mensurae, "Q", MENSURA_AMBITUS, IX, c->piscina);
    /* colloquium (T9): radici R; filius P2 hereditat; Q nullum */
    _factum(mensurae, "R", MENSURA_COLLOQUIUM "abc-123", I, c->piscina);

    index = cursus_indicem_struere(omnes, c->piscina);
    CREDO_NECESSE_NON_NIHIL(index);
    f.arbor   = chorda_ex_literis("/arbor", c->piscina);
    currentes = cursus_quaerere_currentes(index, mensurae, &f,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(currentes);
    /* X (MMM), Q (MM), R (M) cum P2 sub eo; A filtro exclusus */
    CREDO_NECESSE(xar_numerus(currentes) == IV);
    r = _currens(currentes, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->cursus->initium.id, "X");
    CREDO_AEQUALIS_I32(r->profunditas, ZEPHYRUM);
    r = _currens(currentes, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->cursus->initium.id, "Q");
    CREDO_AEQUALIS_S32(r->custos, ZEPHYRUM);
    CREDO_AEQUALIS_I32(r->ambitus, IX);
    CREDO_AEQUALIS_I32(r->iudicata, ZEPHYRUM);
    CREDO_AEQUALIS_I32(r->colloquium.mensura, ZEPHYRUM);
    r = _currens(currentes, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->cursus->initium.id, "R");
    CREDO_AEQUALIS_I32(r->profunditas, ZEPHYRUM);
    CREDO_AEQUALIS_S32(r->custos, MMMDCCCXCVIII);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->colloquium, "abc-123");
    CREDO_AEQUALIS_I32(r->debiti, III);
    CREDO_AEQUALIS_I32(r->perfecti, I);
    CREDO_AEQUALIS_I32(r->fracti, ZEPHYRUM);
    CREDO_AEQUALIS_I32(r->ambitus, ZEPHYRUM);
    /* historia commissionis: F (V ms) solus transiens */
    CREDO_AEQUALIS_S64(r->minima_ms, (s64)V);
    CREDO_AEQUALIS_S64(r->maxima_ms, (s64)V);
    r = _currens(currentes, III);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->cursus->initium.id, "P2");
    CREDO_AEQUALIS_I32(r->profunditas, I);
    CREDO_AEQUALIS_S32(r->custos, MMMDCCCXCVIII);
    CREDO_AEQUALIS_I32(r->perfecti, III);
    CREDO_AEQUALIS_I32(r->fracti, II);
    /* fracta (T10a): m2 et m3 nominata spatio iuncta, j (IUDICIUM) non */
    CREDO_CHORDA_AEQUALIS_LITERIS(r->fracta,
        "probationes_radicis/b probationes_radicis/c");
    CREDO_AEQUALIS_I32(r->debiti, ZEPHYRUM);
    CREDO_AEQUALIS_I32(r->ambitus, CCCLI);
    CREDO_AEQUALIS_I32(r->iudicata, CXX);
    CREDO_AEQUALIS_S64(r->minima_ms, (s64)XX);
    CREDO_AEQUALIS_S64(r->maxima_ms, (s64)LX);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->colloquium, "abc-123");

    /* sine filtro: A quoque (recentissimus, primus) */
    currentes = cursus_quaerere_currentes(index, mensurae, NIHIL,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(currentes);
    CREDO_AEQUALIS_I32(xar_numerus(currentes), V);
    CREDO_CHORDA_AEQUALIS_LITERIS(_currens(currentes,
        ZEPHYRUM)->cursus->initium.id, "A");
    /* mensurae NIHIL licent: custos et progressus ignoti */
    currentes = cursus_quaerere_currentes(index, NIHIL, &f, c->piscina);
    CREDO_NECESSE_NON_NIHIL(currentes);
    CREDO_AEQUALIS_S32(_currens(currentes, II)->custos, ZEPHYRUM);
    CREDO_VERUM(cursus_quaerere_currentes(NIHIL, mensurae, &f,
        c->piscina) == NIHIL);
}

/* XIV. linea currentis (colloquia T10a): contractus 'currentes
 * -machina' - forma exacta, circuitus ad_lineam -> ex_linea, campi
 * vacui, campus impurus recusatus, lineae deformes */
interior vacuum
_sectio_linea (
    CredoContextus* c)
{
                   Xar* omnes = _omnes(c->piscina);
                Cursus* k;
         CursusCurrens  r;
    CursusCurrensLinea  l;
                chorda  linea;

    k = _cursus(omnes, "id-1", "par-0", "porta:radix", MMM, ZEPHYRUM,
        CURSUS_INTERRUPTUS, "", c->piscina);
    memset(&r, ZEPHYRUM, magnitudo(r));
    r.cursus       = k;
    r.profunditas  = I;
    r.custos       = MMMDCCCXCVIII;
    r.perfecti     = II;
    r.fracti       = I;
    r.debiti       = V;
    r.iudicata     = CXX;
    r.ambitus      = CCCLI;
    r.minima_ms    = (s64)XX;
    r.maxima_ms    = (s64)LX;
    r.colloquium   = chorda_ex_literis("abc-123", c->piscina);
    r.fracta       = chorda_ex_literis(
        "probationes_radicis/a probationes_radicis/b", c->piscina);
    linea = cursus_currentem_ad_lineam(&r, CURSUS_VITA_VIVA,
        c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea, "CURRENS\t1\tid-1\tpar-0\t"
        "porta:radix\t/arbor\tmain\t3000\t3898\tvivus\t2\t1\t5\t120\t"
        "351\t20\t60\tabc-123\tprobationes_radicis/a "
        "probationes_radicis/b\n");
    CREDO_NECESSE(cursus_currentem_ex_linea(linea, &l));
    CREDO_AEQUALIS_I32(l.profunditas, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(l.id, "id-1");
    CREDO_CHORDA_AEQUALIS_LITERIS(l.parens, "par-0");
    CREDO_CHORDA_AEQUALIS_LITERIS(l.titulus, "porta:radix");
    CREDO_CHORDA_AEQUALIS_LITERIS(l.arbor, "/arbor");
    CREDO_CHORDA_AEQUALIS_LITERIS(l.ramus, "main");
    CREDO_AEQUALIS_S64(l.initium, (s64)MMM);
    CREDO_AEQUALIS_S32(l.custos, MMMDCCCXCVIII);
    CREDO_AEQUALIS_I32((i32)l.vita, (i32)CURSUS_VITA_VIVA);
    CREDO_AEQUALIS_I32(l.perfecti, II);
    CREDO_AEQUALIS_I32(l.fracti, I);
    CREDO_AEQUALIS_I32(l.debiti, V);
    CREDO_AEQUALIS_I32(l.iudicata, CXX);
    CREDO_AEQUALIS_I32(l.ambitus, CCCLI);
    CREDO_AEQUALIS_S64(l.minima_ms, (s64)XX);
    CREDO_AEQUALIS_S64(l.maxima_ms, (s64)LX);
    CREDO_CHORDA_AEQUALIS_LITERIS(l.colloquium, "abc-123");
    CREDO_CHORDA_AEQUALIS_LITERIS(l.fracta,
        "probationes_radicis/a probationes_radicis/b");

    /* campi vacui (homo, nihil fractum); vitae ceterae */
    r.colloquium  = chorda_ex_literis("", c->piscina);
    r.fracta      = chorda_ex_literis("", c->piscina);
    linea = cursus_currentem_ad_lineam(&r, CURSUS_VITA_MORTUA,
        c->piscina);
    CREDO_NECESSE(cursus_currentem_ex_linea(linea, &l));
    CREDO_AEQUALIS_I32((i32)l.vita, (i32)CURSUS_VITA_MORTUA);
    CREDO_AEQUALIS_I32(l.colloquium.mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(l.fracta.mensura, ZEPHYRUM);
    linea = cursus_currentem_ad_lineam(&r, CURSUS_VITA_IGNOTA,
        c->piscina);
    CREDO_NECESSE(cursus_currentem_ex_linea(linea, &l));
    CREDO_AEQUALIS_I32((i32)l.vita, (i32)CURSUS_VITA_IGNOTA);
    /* linea sine '\n' finali quoque legitur */
    CREDO_VERUM(cursus_currentem_ex_linea(chorda_ex_literis(
        "CURRENS\t0\ta\t\tcommissio\t/r\tmain\t1\t0\tignotus\t0\t0\t0"
        "\t0\t0\t0\t0\t\t", c->piscina), &l));

    /* campus impurus: linea vacua, numquam forma fracta */
    k->initium.titulus = chorda_ex_literis("a\tb", c->piscina);
    CREDO_AEQUALIS_I32(cursus_currentem_ad_lineam(&r, CURSUS_VITA_VIVA,
        c->piscina).mensura, ZEPHYRUM);

    /* deformes */
    CREDO_FALSUM(cursus_currentem_ex_linea(chorda_ex_literis(
        "CURRENS\t1\tid", c->piscina), &l));
    CREDO_FALSUM(cursus_currentem_ex_linea(chorda_ex_literis(
        "HISTORIA\t0\ta\t\tcommissio\t/r\tmain\t1\t0\tignotus\t0\t0"
        "\t0\t0\t0\t0\t0\t\t", c->piscina), &l));
    CREDO_FALSUM(cursus_currentem_ex_linea(chorda_ex_literis(
        "CURRENS\tx\ta\t\tcommissio\t/r\tmain\t1\t0\tignotus\t0\t0"
        "\t0\t0\t0\t0\t0\t\t", c->piscina), &l));
    CREDO_FALSUM(cursus_currentem_ex_linea(chorda_ex_literis(
        "CURRENS\t0\ta\t\tcommissio\t/r\tmain\t1\t0\tquidam\t0\t0"
        "\t0\t0\t0\t0\t0\t\t", c->piscina), &l));
    CREDO_FALSUM(cursus_currentem_ex_linea(chorda_ex_literis(
        "CURRENS\t0\ta\t\tcommissio\t/r\tmain\t1\t0\tignotus\t0\t0"
        "\t0\t0\t0\t0\t0\t\t\tsuperfluum", c->piscina), &l));
}

#define UNITAS_MEMORIAE ((i32)MXXIV * (i32)MXXIV)   /* MiB */

interior constans CursusGravis*
_gravis (
    constans Xar* graves,
             i32  k)
{
    redde (constans CursusGravis*)xar_obtinere(graves, k);
}

/* XV. graves (credo memoria T3): per titulum sectionis maxima trans
 * cursus (a#gravis bis: arena 32 -> 48), numerus = cursus mensi; pondus
 * = maior arenae et aucti (b#auctus per auctum primus); sectio sine
 * factis abest; facta alia et parentes ignoti neglecti; filtra arboris,
 * praefixi, numeri */
interior vacuum
_sectio_graves (
    CredoContextus* c)
{
                       Xar* omnes = _omnes(c->piscina);
                       Xar* mensurae = xar_creare(c->piscina,
                           (i32)magnitudo(Mensura));
               CursusIndex* index;
             CursusFiltrum  f = _filtrum();
                       Xar* graves;
     constans CursusGravis* g;
                    Cursus* alienus;

    (vacuum)_cursus(omnes, "S1", "", "probationes_radicis/a#gravis", X,
        I, CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "S2", "", "probationes_radicis/a#gravis", XX,
        I, CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "S3", "", "probationes_radicis/a#levis", XXX,
        I, CURSUS_TRANSIIT, "", c->piscina);
    (vacuum)_cursus(omnes, "S4", "", "probationes_radicis/b#auctus", XL,
        I, CURSUS_FRACTUS, "", c->piscina);
    (vacuum)_cursus(omnes, "S5", "", "probationes_radicis/a#sine", L, I,
        CURSUS_TRANSIIT, "", c->piscina);
    alienus = _cursus(omnes, "S6", "", "probationes_radicis/c#alienus",
        LX, I, CURSUS_TRANSIIT, "", c->piscina);
    alienus->initium.arbor = chorda_ex_literis("/alibi", c->piscina);

    _factum(mensurae, "S1", MENSURA_ARENA_APEX, XXXII * UNITAS_MEMORIAE,
        c->piscina);
    _factum(mensurae, "S1", MENSURA_APEX_AUCTUS, UNITAS_MEMORIAE,
        c->piscina);
    _factum(mensurae, "S1", MENSURA_VESTIGIUM, XL * UNITAS_MEMORIAE,
        c->piscina);
    _factum(mensurae, "S1", MENSURA_CUSTOS, CMXCIX, c->piscina);
    _factum(mensurae, "S2", MENSURA_ARENA_APEX,
        XLVIII * UNITAS_MEMORIAE,
        c->piscina);
    _factum(mensurae, "S2", MENSURA_APEX_AUCTUS, ZEPHYRUM, c->piscina);
    _factum(mensurae, "S2", MENSURA_VESTIGIUM, L * UNITAS_MEMORIAE,
        c->piscina);
    _factum(mensurae, "S3", MENSURA_ARENA_APEX, ZEPHYRUM, c->piscina);
    _factum(mensurae, "S3", MENSURA_APEX_AUCTUS, ZEPHYRUM, c->piscina);
    _factum(mensurae, "S3", MENSURA_VESTIGIUM, II * UNITAS_MEMORIAE,
        c->piscina);
    _factum(mensurae, "S4", MENSURA_ARENA_APEX, UNITAS_MEMORIAE,
        c->piscina);
    _factum(mensurae, "S4", MENSURA_APEX_AUCTUS, LXIV * UNITAS_MEMORIAE,
        c->piscina);
    _factum(mensurae, "S4", MENSURA_VESTIGIUM, LXX * UNITAS_MEMORIAE,
        c->piscina);
    _factum(mensurae, "S6", MENSURA_ARENA_APEX,
        CMXCIX * UNITAS_MEMORIAE,
        c->piscina);
    _factum(mensurae, "S6", MENSURA_VESTIGIUM, M * UNITAS_MEMORIAE,
        c->piscina);
    _factum(mensurae, "Z", MENSURA_ARENA_APEX, CMXCIX * UNITAS_MEMORIAE,
        c->piscina);

    index = cursus_indicem_struere(omnes, c->piscina);
    CREDO_NECESSE_NON_NIHIL(index);
    f.arbor  = chorda_ex_literis("/arbor", c->piscina);
    graves   = cursus_quaerere_graves(index, mensurae, &f, c->piscina);
    CREDO_NECESSE_NON_NIHIL(graves);
    CREDO_NECESSE(xar_numerus(graves) == III);
    g = _gravis(graves, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(g->titulus,
        "probationes_radicis/b#auctus");
    CREDO_AEQUALIS_S64(g->pondus, (s64)LXIV * (s64)UNITAS_MEMORIAE);
    CREDO_AEQUALIS_S64(g->arena_apex, (s64)UNITAS_MEMORIAE);
    CREDO_AEQUALIS_I32(g->numerus, I);
    g = _gravis(graves, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(g->titulus,
        "probationes_radicis/a#gravis");
    CREDO_AEQUALIS_S64(g->arena_apex,
        (s64)XLVIII * (s64)UNITAS_MEMORIAE);
    CREDO_AEQUALIS_S64(g->apex_auctus, (s64)UNITAS_MEMORIAE);
    CREDO_AEQUALIS_S64(g->vestigium, (s64)L * (s64)UNITAS_MEMORIAE);
    CREDO_AEQUALIS_S64(g->pondus, (s64)XLVIII * (s64)UNITAS_MEMORIAE);
    CREDO_AEQUALIS_I32(g->numerus, II);
    g = _gravis(graves, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(g->titulus,
        "probationes_radicis/a#levis");
    CREDO_AEQUALIS_S64(g->pondus, (s64)ZEPHYRUM);
    CREDO_AEQUALIS_S64(g->vestigium, (s64)II * (s64)UNITAS_MEMORIAE);

    /* praefixum membri et numerus */
    f.praefixum = chorda_ex_literis("probationes_radicis/a#",
        c->piscina);
    f.numerus = I;
    graves = cursus_quaerere_graves(index, mensurae, &f, c->piscina);
    CREDO_NECESSE_NON_NIHIL(graves);
    CREDO_AEQUALIS_I32(xar_numerus(graves), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(_gravis(graves, ZEPHYRUM)->titulus,
        "probationes_radicis/a#gravis");

    /* sine filtro: alienus (CMXCIX MiB) primus */
    graves = cursus_quaerere_graves(index, mensurae, NIHIL, c->piscina);
    CREDO_NECESSE_NON_NIHIL(graves);
    CREDO_AEQUALIS_I32(xar_numerus(graves), IV);
    CREDO_CHORDA_AEQUALIS_LITERIS(_gravis(graves, ZEPHYRUM)->titulus,
        "probationes_radicis/c#alienus");
    /* mensurae NIHIL: vacuum, non NIHIL; index NIHIL: NIHIL */
    graves = cursus_quaerere_graves(index, NIHIL, NIHIL, c->piscina);
    CREDO_NECESSE_NON_NIHIL(graves);
    CREDO_AEQUALIS_I32(xar_numerus(graves), ZEPHYRUM);
    CREDO_NIHIL(cursus_quaerere_graves(NIHIL, mensurae, NIHIL,
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
    { "XIII currentes", _sectio_currentes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "XIV linea currentis", _sectio_linea, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "XV graves", _sectio_graves, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mensura_quaestio", SECTIONES);
}
