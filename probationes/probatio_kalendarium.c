/* probatio_kalendarium.c - calendarium, pagina mensis (agenda-plan
 * A2a): montatio (hodie ex porta, focus), craticula XLII cellularum
 * (dominica prima, id = dies ISO), claves (h/l/j/k/H/L/t, mensis
 * praecisus), ictus in cellula, argumentum "YYYY-MM", persistentia
 * (visus redit, dies electa non). Hodie fixa: feria IV, 2026-10-14. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "fasti.h"
#include "conservatio.h"
#include "kalendarium.h"
#include "agenda.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* porta hodie: dies fixa */
interior b32
hodie_fixa (
    vacuum* ctx,
      Dies* hodie)
{
    (vacuum)ctx;
    *hodie = fasti_dies(MMXXVI, X, XIV);
    redde VERUM;
}

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
     KalendariumMontatio  m;
             Dispensator* d;
                 Volumen* volumen;
         StatusSystemata  systemata;
        AgendaDocumentum* documentum;
} Mundus;

/* montare (sine systematis nisi volumen) et dispensatorem ligare; m in
 * ramo <calendarium id="c1"> sub <hospes/> */
interior b32
mundum_struere (
                 Mundus* x,
                Piscina* p,
     constans character* argumentum)
{
    Motus* motus;

    x->repo = insula_repositorium_creare(p, x->intern, "<hospes/>");
    si (!x->repo)
    {
        redde FALSUM;
    }
    si (x->volumen)
    {
        memset(&x->systemata, ZEPHYRUM, magnitudo(x->systemata));
        x->systemata.conservatio = conservatio_creare(p, x->intern,
            x->volumen, x->repo);
    }
    si (!kalendarium_montare(&x->m, p, x->intern, x->repo, "c1",
            argumentum, x->documentum, hodie_fixa, NIHIL,
            x->volumen ? &x->systemata : NIHIL))
    {
        redde FALSUM;
    }
    x->d = dispensator_creare(p, x->intern, x->repo, x->m.actiones,
        kalendarium_componere, &x->m, CCC);
    si (!x->d)
    {
        redde FALSUM;
    }
    motus         = dispensator_motus(x->d);
    motus->ramus  = x->m.ramus;
    dispensator_recomponere(x->d);
    redde VERUM;
}

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    x->intern  = internamentum_creare(c->piscina);
    c->datum   = x;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    si (x && x->volumen)
    {
        volumen_claudere(x->volumen);
    }
}

interior b32
electa_est (
    Mundus* x,
       s32  annus,
       s32  mensis,
       s32  dies)
{
    Dies d;

    d = kalendarium_dies_electus(&x->m);
    redde d.annus == annus && d.mensis == mensis && d.dies == dies;
}

interior vacuum
clavis (
       Mundus* x,
    character  typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.producta  = (s32)(insignatus character)typus;
    e.datum.clavis.clavis    = (clavis_t)typus;
    e.datum.clavis.runa      = (typus >= 'A' && typus <= 'Z')
                             ? (s32)(typus - 'A' + 'a') : (s32)typus;
    si (typus >= 'A' && typus <= 'Z')
    {
        e.datum.clavis.modificantes = (i32)MOD_SHIFT;
    }
    dispensator_tractare(x->d, &e);
}

/* ictus in medio cellulae id (radix in 0,0; cellulae liberi eius) */
interior b32
ictus_in (
                 Mundus* x,
     constans character* id,
                Piscina* p)
{
     Componens* c;
       Eventus  e;

    c = componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis(id, p));
    si (!c)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x  = c->fines.x + c->fines.latitudo / II;
    e.datum.mus.y  = c->fines.y + c->fines.altitudo / II;
    dispensator_tractare(x->d, &e);
    redde VERUM;
}

/* montatio: hodie electa, focus in radice, visus mensis */
interior vacuum
_probare_montationem (
    CredoContextus* c)
{
    Mundus* x;
    chorda* a;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    CREDO_VERUM(electa_est(x, MMXXVI, X, XIV));
    a = insula_ramus_attributum(&x->m.ramus, "visus");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "mensis"));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(x->d),
        "kalendarium");
}

/* craticula: XLII cellulae, prima dominica 2026-09-27 */
interior vacuum
_probare_craticulam (
    CredoContextus* c)
{
        Mundus* x;
     Componens* radix;
     Componens* prima;
     Componens* ultima;
           i32  i;
           i32  cellulae;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    radix = dispensator_arbor(x->d);
    CREDO_NECESSE_NON_NIHIL(radix);
    CREDO_CHORDA_AEQUALIS_LITERIS(radix->id, "kalendarium");
    CREDO_VERUM(radix->focusabilis);
    cellulae  = ZEPHYRUM;
    prima     = NIHIL;
    ultima    = NIHIL;
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(radix); i++)
    {
        Componens* l;

        l = componens_liberum(radix, i);
        si (l->id.mensura == X && l->id.datum[IV] == '-')
        {
            si (!prima)
            {
                prima = l;
            }
            ultima = l;
            cellulae++;
        }
    }
    CREDO_AEQUALIS_I32(cellulae, XLII);
    CREDO_NECESSE(prima && ultima);
    CREDO_CHORDA_AEQUALIS_LITERIS(prima->id, "2026-09-27");
    CREDO_CHORDA_AEQUALIS_LITERIS(ultima->id, "2026-11-07");
    CREDO_CHORDA_AEQUALIS_LITERIS(ultima->actio, "calendarium.agere");
    CREDO_NON_NIHIL(componens_invenire_per_id(radix, chorda_ex_literis(
        "titulus", c->piscina)));
    /* hebdomas: cellula VIII infra primam */
    CREDO_VERUM(componens_invenire_per_id(radix, chorda_ex_literis(
        "2026-10-04", c->piscina))->fines.x == prima->fines.x);
}

/* claves: dies, hebdomas, mensis (praecisus), hodie */
interior vacuum
_probare_claves (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    clavis(x, 'l');
    CREDO_VERUM(electa_est(x, MMXXVI, X, XV));
    clavis(x, 'h');
    clavis(x, 'h');
    CREDO_VERUM(electa_est(x, MMXXVI, X, XIII));
    clavis(x, 'j');
    CREDO_VERUM(electa_est(x, MMXXVI, X, XX));
    clavis(x, 'k');
    CREDO_VERUM(electa_est(x, MMXXVI, X, XIII));
    clavis(x, 'L');
    CREDO_VERUM(electa_est(x, MMXXVI, XI, XIII));
    clavis(x, 'H');
    clavis(x, 'H');
    CREDO_VERUM(electa_est(x, MMXXVI, IX, XIII));
    clavis(x, 't');
    CREDO_VERUM(electa_est(x, MMXXVI, X, XIV));
    /* praecisio: XXXI Dec -> XXXI Ian -> XXVIII Feb */
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "2027-01"));
    dispensator_recomponere(x->d);
    clavis(x, 'h');
    CREDO_VERUM(electa_est(x, MMXXVI, XII, XXXI));
    clavis(x, 'L');
    CREDO_VERUM(electa_est(x, MMXXVII, I, XXXI));
    clavis(x, 'L');
    CREDO_VERUM(electa_est(x, MMXXVII, II, XXVIII));
}

/* ictus in cellula diem eligit (etiam mense vicino) */
interior vacuum
_probare_ictum (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    CREDO_VERUM(ictus_in(x, "2026-10-03", c->piscina));
    CREDO_VERUM(electa_est(x, MMXXVI, X, III));
    CREDO_VERUM(ictus_in(x, "2026-09-28", c->piscina));
    CREDO_VERUM(electa_est(x, MMXXVI, IX, XXVIII));
}

/* argumentum: dies I mensis, aut hodie in mense suo; malum recusatur */
interior vacuum
_probare_argumentum (
    CredoContextus* c)
{
    Mundus* x;
    Mundus  y;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c->piscina, "2026-12"));
    CREDO_VERUM(electa_est(x, MMXXVI, XII, I));
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "2026-10"));
    CREDO_VERUM(electa_est(x, MMXXVI, X, XIV));
    CREDO_FALSUM(kalendarium_argumentum_ponere(&x->m, "x"));
    CREDO_FALSUM(kalendarium_argumentum_ponere(&x->m, "2026-13"));
    CREDO_VERUM(electa_est(x, MMXXVI, X, XIV));
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "2027-03"));
    CREDO_VERUM(electa_est(x, MMXXVII, III, I));
    memset(&y, ZEPHYRUM, magnitudo(y));
    y.intern = x->intern;
    CREDO_FALSUM(mundum_struere(&y, c->piscina, "2026-13"));
}

/* ctx = "attributum=valor" */
interior vacuum
ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
     constans character* par;
     constans character* signum;
              character  titulus[XXXII];

    par     = (constans character*)ctx;
    signum  = strchr(par, '=');
    memcpy(titulus, par, (size_t)(signum - par));
    titulus[signum - par] = '\0';
    insula_attributum_ponere(radix, p, in, titulus, signum + I);
}

/* persistentia: visus redit, dies electa (transiens) non */
interior vacuum
_probare_conservationem (
    CredoContextus* c)
{
       Mundus* x;
       Mundus  y;
       chorda* a;
    character  par[XVI];

    x = (Mundus*)c->datum;
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_kalendarium");
    CREDO_NECESSE_NON_NIHIL(x->volumen);
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    clavis(x, 'l');
    CREDO_VERUM(electa_est(x, MMXXVI, X, XV));
    strcpy(par, "visus=dies");
    insula_scriptorem_ponere(x->repo, chorda_ex_literis(
        "calendarium.agere", c->piscina));
    CREDO_NECESSE(mutare_ramum(&x->m.ramus, ponere, par));
    memset(&y, ZEPHYRUM, magnitudo(y));
    y.intern   = x->intern;
    y.volumen  = x->volumen;
    CREDO_NECESSE(mundum_struere(&y, c->piscina, NIHIL));
    a = insula_ramus_attributum(&y.m.ramus, "visus");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "dies"));
    CREDO_VERUM(electa_est(&y, MMXXVI, X, XIV));
}

/* fines cellulae dies ISO */
interior Fines
cellula (
                 Mundus* x,
     constans character* id,
                Piscina* p)
{
     Componens* c;
         Fines  f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    c = componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis(id, p));
    si (c)
    {
        f = c->fines;
    }
    redde f;
}

interior b32
intra (
    Fines a,
    Fines f)
{
    redde a.x >= f.x && a.y >= f.y
        && a.x + a.latitudo <= f.x + f.latitudo
        && a.y + a.altitudo <= f.y + f.altitudo;
}

interior b32
punctum_intra (
    Fines a,
    Fines f)
{
    redde a.x >= f.x && a.y >= f.y && a.x < f.x + f.latitudo
        && a.y < f.y + f.altitudo;
}

interior b32
color_thematis_est (
    ColorMandati c,
             i32 signum)
{
    redde c.genus == COLOR_MANDATI_THEMA && c.valor == signum;
}

/* linea coloris liturgici cellulae: rectangulum plenum II pixelorum ad
 * sinistrum, altitudine cellulae; NIHIL si nulla */
interior constans Mandatum*
linea_coloris (
    Mandata* md,
      Fines  f)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus          == MANDATUM_RECTANGULUM && x->impletum
            && x->fines.x        == f.x && x->fines.y == f.y
            && x->fines.latitudo == II
            && x->fines.altitudo == f.altitudo)
        {
            redde x;
        }
    }
    redde NIHIL;
}

interior b32
colores_aequales (
    constans Mandatum* a,
    constans Mandatum* b)
{
    redde a && b && a->color.genus == b->color.genus
        && a->color.valor == b->color.valor;
}

/* signa in cellula: quadrata III x III, plena aut vacua */
interior i32
signa_numerare (
    Mandata* md,
      Fines  f,
        b32  plena)
{
    constans Mandatum* x;
                  i32  i;
                  i32  n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_RECTANGULUM
            && x->fines.latitudo == III
            && x->fines.altitudo == III && intra(x->fines, f)
            && (x->impletum ? VERUM : FALSUM) == plena)
        {
            n++;
        }
    }
    redde n;
}

/* textus in cellula (puncto initii) qui praefixum fert */
interior b32
textus_in (
                 Mandata* md,
                   Fines  f,
      constans character* praefixum)
{
    constans Mandatum* x;
                  i32  i;
                  i32  n;

    n = (i32)strlen(praefixum);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS && punctum_intra(x->fines, f)
            && x->textus.mensura > ZEPHYRUM
            && memcmp(x->textus.datum, praefixum,
                   (size_t)(x->textus.mensura
                       < n ? x->textus.mensura : n))
               == ZEPHYRUM
            && x->textus.mensura >= (n < III ? n : III))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
textus_ullus_continet (
                Mandata* md,
     constans character* pars)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_continet(x->textus, chorda_ex_literis(pars,
                   md->piscina)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* figurae: titulus, hodie, electa, colores liturgici, festum, signa */
interior vacuum
_probare_figuras (
    CredoContextus* c)
{
                 Mundus* x;
                Mandata* md;
      constans Mandatum* r;
                    i32  i;
                  Fines  f;
                    s32  a;
                    s32  b;
                   Dies  d;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_kalendarium");
    CREDO_NECESSE_NON_NIHIL(x->volumen);
    x->documentum = agenda_aperire(c->piscina, x->intern, x->volumen,
        "agenda", ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(x->documentum);
    a = agenda_rem_creare(x->documentum, chorda_ex_literis("a",
        c->piscina));
    b = agenda_rem_creare(x->documentum, chorda_ex_literis("b",
        c->piscina));
    d = fasti_dies(MMXXVI, X, XIV);
    (vacuum)agenda_diem_ponere(x->documentum, a, &d);
    d = fasti_dies(MMXXVI, X, XIII);
    (vacuum)agenda_diem_ponere(x->documentum, b, &d);
    (vacuum)agenda_perficere(x->documentum, b, d);
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    md = mandata_creare(c->piscina, x->intern);
    pingere(dispensator_arbor(x->d), x->m.figurae, ZEPHYRUM, md);

    CREDO_VERUM(textus_ullus_continet(md, "October 2026"));
    /* hodie: numerus inversus (rectangulum accentus) in cellula XIV */
    f = cellula(x, "2026-10-14", c->piscina);
    r = NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa) && !r; i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus == MANDATUM_RECTANGULUM && y->impletum
            && intra(y->fines, f)
            && color_thematis_est(y->color, COLOR_ACCENT_PRIMARY))
        {
            r = y;
        }
    }
    CREDO_NON_NIHIL(r);
    /* electa (= hodie): margo activus finibus cellulae */
    r = NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa) && !r; i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus          == MANDATUM_RECTANGULUM && !y->impletum
            && y->fines.x        == f.x && y->fines.y == f.y
            && y->fines.latitudo == f.latitudo
            && color_thematis_est(y->color, COLOR_BORDER_ACTIVE))
        {
            r = y;
        }
    }
    CREDO_NON_NIHIL(r);
    /* colores: VII et XV (memoriae, albus) aequales; XX et XXI (feriae
     * per annum, viridis) aequales; albus != viridis, albus != XIX
     * (memoria martyrum, ruber) */
    CREDO_VERUM(colores_aequales(linea_coloris(md, cellula(x,
        "2026-10-15", c->piscina)), linea_coloris(md, cellula(x,
        "2026-10-07", c->piscina))));
    CREDO_VERUM(colores_aequales(linea_coloris(md, cellula(x,
        "2026-10-20", c->piscina)), linea_coloris(md, cellula(x,
        "2026-10-21", c->piscina))));
    CREDO_FALSUM(colores_aequales(linea_coloris(md, cellula(x,
        "2026-10-15", c->piscina)), linea_coloris(md, cellula(x,
        "2026-10-20", c->piscina))));
    CREDO_FALSUM(colores_aequales(linea_coloris(md, cellula(x,
        "2026-10-15", c->piscina)), linea_coloris(md, cellula(x,
        "2026-10-19", c->piscina))));
    /* festum in cellula XV (praecisum) */
    CREDO_VERUM(textus_in(md, cellula(x, "2026-10-15", c->piscina),
        "St. Teresa of Avila"));
    /* signa: XIV res aperta (plenum I), XIII perfecta (vacuum I) */
    CREDO_AEQUALIS_I32(signa_numerare(md, cellula(x, "2026-10-14",
        c->piscina), VERUM), I);
    CREDO_AEQUALIS_I32(signa_numerare(md, cellula(x, "2026-10-13",
        c->piscina), FALSUM), I);
    CREDO_AEQUALIS_I32(signa_numerare(md, cellula(x, "2026-10-13",
        c->piscina), VERUM), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "montatio",     _probare_montationem,   mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "craticula",    _probare_craticulam,    mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "claves",       _probare_claves,        mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "ictus",        _probare_ictum,         mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "argumentum",   _probare_argumentum,    mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "persistentia", _probare_conservationem, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "figurae",      _probare_figuras,       mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("kalendarium", SECTIONES);
}
