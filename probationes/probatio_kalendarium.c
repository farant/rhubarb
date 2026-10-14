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
#include "fons.h"
#include "agenda.h"
#include "scriba_liber.h"
#include "scriba_documentum.h"
#include "tabula_characterum.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "exemplaria.h"
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
                      s32  latitudo;   /* A6e: superficies montat. */
                      s32  altitudo;
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
            argumentum, x->documentum, x->volumen, (i32)x->latitudo,
            (i32)x->altitudo, hodie_fixa, NIHIL,
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

/* ictus in puncto (spatium radicis) */
interior vacuum
ictus_ad (
    Mundus* x,
       s32  px,
       s32  py)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x  = px;
    e.datum.mus.y  = py;
    dispensator_tractare(x->d, &e);
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

/* quadratum coloris liturgici cellulae: rectangulum plenum V x V
 * (RGBA) intra cellulam, iuxta numerum; NIHIL si nullum */
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
            && x->color.genus    == COLOR_MANDATI_RGBA
            && x->fines.latitudo == V && x->fines.altitudo == V
            && intra(x->fines, f))
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

/* textus (quilibet) in cellula numerati */
interior i32
textus_numerare (
    Mandata* md,
      Fines  f)
{
    constans Mandatum* x;
                  i32  i;
                  i32  n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (x->genus == MANDATUM_TEXTUS && punctum_intra(x->fines, f))
        {
            n++;
        }
    }
    redde n;
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

/* textus partem continens fonte dato pictus? (V3c) */
interior b32
textus_fontis_est (
                Mandata* md,
     constans character* pars,
                    i32  fons)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS && x->fons == fons
            && chorda_continet(x->textus, chorda_ex_literis(pars,
                   md->piscina)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* litterae dierum (V2c): in fascia supra hebdomadem primam, quaeque
 * super columnam suam centrata (cellulae verae, non latitudo / VII) */
interior b32
litterae_dierum_sunt (
     Mundus* x,
    Mandata* md,
    Piscina* p)
{
    constans character* ids[VII] = { "2026-09-27", "2026-09-28",
        "2026-09-29", "2026-09-30", "2026-10-01", "2026-10-02",
        "2026-10-03" };
    constans character* litterae = "SMTWTFS";
     constans Mandatum* y;
                 Fines  f;
                   i32  i;
                   i32  j;
                   b32  inventa;

    per (j = ZEPHYRUM; j < VII; j++)
    {
        f        = cellula(x, ids[j], p);
        inventa  = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
        {
            y = (constans Mandatum*)xar_obtinere(md->elementa, i);
            si (   y->genus == MANDATUM_TEXTUS && y->textus.mensura == I
                && y->textus.datum[ZEPHYRUM] == (i8)litterae[j]
                && y->fines.x == f.x + (f.latitudo - VI) / II
                && y->fines.y == f.y - XII)
            {
                inventa = VERUM;
            }
        }
        si (!inventa)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
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
    /* V3c: titulus fonte titulorum (8x16), tempus parvo */
    CREDO_VERUM(textus_fontis_est(md, "October 2026", FONS_TITULI));
    CREDO_VERUM(textus_fontis_est(md, "Tempus", FONS_PARVUS));
    /* litterae dierum: textus in memoria mandatorum, non in acervo
     * figurae (mandata textum per indicem servant) - post vocationes
     * alias acervum rescribentes */
    CREDO_VERUM(litterae_dierum_sunt(x, md, c->piscina));
    /* V2c: fascia dierum supra hebdomadem primam */
    f = cellula(x, "2026-09-27", c->piscina);
    r = NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa) && !r; i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus                       == MANDATUM_RECTANGULUM
            && y->exemplar                    == EXEMPLAR_CINEREUM_LEVE
            && y->fines.x                     == f.x
            && y->fines.y + y->fines.altitudo == f.y)
        {
            r = y;
        }
    }
    CREDO_NON_NIHIL(r);
    /* dies alieni (IX 27): exemplar punctorum per cellulam */
    r = NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa) && !r; i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus    == MANDATUM_RECTANGULUM
            && y->exemplar == EXEMPLAR_PUNCTA_DISPERSA
            && y->fines.x  == f.x && y->fines.y == f.y)
        {
            r = y;
        }
    }
    CREDO_NON_NIHIL(r);
    /* craticula: margo cellulae (XX) I px ultra (linea communis) */
    f = cellula(x, "2026-10-20", c->piscina);
    r = NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa) && !r; i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus          == MANDATUM_RECTANGULUM && !y->impletum
            && y->fines.x        == f.x && y->fines.y == f.y
            && y->fines.latitudo == f.latitudo + I
            && color_thematis_est(y->color, COLOR_TEXT_DIM))
        {
            r = y;
        }
    }
    CREDO_NON_NIHIL(r);
    /* hodie: insigne (rectangulum COLOR_TEXT, V2c) in cellula XIV */
    f = cellula(x, "2026-10-14", c->piscina);
    r = NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa) && !r; i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus == MANDATUM_RECTANGULUM && y->impletum
            && intra(y->fines, f)
            && color_thematis_est(y->color, COLOR_TEXT))
        {
            r = y;
        }
    }
    CREDO_NON_NIHIL(r);
    /* electa (= hodie): margo duplex obscurus (V2c) */
    r = NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa) && !r; i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus          == MANDATUM_RECTANGULUM && !y->impletum
            && y->fines.x        == f.x && y->fines.y == f.y
            && y->fines.latitudo == f.latitudo + I
            && color_thematis_est(y->color, COLOR_TEXT))
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
    /* dominica per annum: numerus solus (titulus "Dominica ..." non
     * ostenditur - color et columna dicunt) */
    CREDO_AEQUALIS_I32(textus_numerare(md, cellula(x, "2026-10-18",
        c->piscina)), I);
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

/* clavis specialis (Enter, Esc, Ctrl-R) */
interior vacuum
clavis_specialis (
       Mundus* x,
     clavis_t  codex,
          s32  producta,
          s32  runa,
          i32  modificantes)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis        = codex;
    e.datum.clavis.producta      = producta;
    e.datum.clavis.runa          = runa;
    e.datum.clavis.modificantes  = modificantes;
    dispensator_tractare(x->d, &e);
}

/* agenda diei XIV: 1 aperta, 2 perfecta XIV, 3 assignata XX perfecta
 * XIV (hic perfecta), 4 deleta; dein montatio */
interior b32
agendam_parare (
                Mundus* x,
        CredoContextus* c)
{
    s32 id;
   Dies d;
    i32 i;

    x->volumen = volumen_temporarium(c->piscina,
        "probatio_kalendarium");
    si (!x->volumen)
    {
        redde FALSUM;
    }
    x->documentum = agenda_aperire(c->piscina, x->intern, x->volumen,
        "agenda", ZEPHYRUM);
    si (!x->documentum)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < IV; i++)
    {
        id = agenda_rem_creare(x->documentum, chorda_ex_literis("res",
            c->piscina));
        d  = fasti_dies(MMXXVI, X, i == II ? XX : XIV);
        (vacuum)agenda_diem_ponere(x->documentum, id, &d);
    }
    d = fasti_dies(MMXXVI, X, XIV);
    (vacuum)agenda_perficere(x->documentum, II, d);
    (vacuum)agenda_perficere(x->documentum, III, d);
    (vacuum)agenda_rem_delere(x->documentum, IV);
    redde mundum_struere(x, c->piscina, NIHIL);
}

/* agenda cum argumento "agenda": fixa agendae diei (1 domus XIV, 2 et
 * 3 perfectae, 4 deleta) + 5 sine die "opus domus" + 6 XX sine
 * rubrica */
interior b32
agendam_visus_parare (
                Mundus* x,
        CredoContextus* c)
{
    s32 id;
   Dies d;

    si (!agendam_parare(x, c))
    {
        redde FALSUM;
    }
    (vacuum)agenda_rubricas_ponere(x->documentum, I, chorda_ex_literis(
        "domus", c->piscina));
    id = agenda_rem_creare(x->documentum, chorda_ex_literis("res",
        c->piscina));
    (vacuum)agenda_rubricas_ponere(x->documentum, id, chorda_ex_literis(
        "opus domus", c->piscina));
    id = agenda_rem_creare(x->documentum, chorda_ex_literis("res",
        c->piscina));
    d  = fasti_dies(MMXXVI, X, XX);
    (vacuum)agenda_diem_ponere(x->documentum, id, &d);
    redde kalendarium_argumentum_ponere(&x->m, "agenda");
}

/* ordines visus diei (id "r<n>") ordine arboris = "r1 r2 ..." ? */
interior b32
ordines_sunt (
                 Mundus* x,
     constans character* exspectati)
{
    Componens* radix;
    character  visi[CXXVIII];
          i32  i;
          i32  k;
    Componens* l;

    radix           = dispensator_arbor(x->d);
    k               = ZEPHYRUM;
    visi[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; radix && i < componens_numerus_liberorum(radix);
        i++)
    {
        l = componens_liberum(radix, i);
        si (   l->id.mensura  >= II && l->id.datum[ZEPHYRUM] == 'r'
            && l->id.datum[I] >= '0' && l->id.datum[I] <= '9')
        {
            si (k > ZEPHYRUM)
            {
                visi[k++] = ' ';
            }
            memcpy(visi + k, l->id.datum, (size_t)l->id.mensura);
            k        += l->id.mensura;
            visi[k]  = '\0';
        }
    }
    redde strcmp(visi, exspectati) == ZEPHYRUM;
}

interior b32
campus_est (
                 Mundus* x,
     constans character* titulus,
     constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(&x->m.ramus, titulus);
    redde a && chorda_aequalis_literis(*a, valor);
}

/* Enter diem aperit (visus dies), Esc ad mensem; ordines diei */
interior vacuum
_probare_visum_diei (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    clavis_specialis(x, CLAVIS_REDITUS, XIII, XIII, ZEPHYRUM);
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    CREDO_VERUM(ordines_sunt(x, "r1 r2 r3"));
    CREDO_NON_NIHIL(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("titulus", c->piscina)));
    clavis_specialis(x, CLAVIS_EFFUGIUM, XXVII, XXVII, ZEPHYRUM);
    CREDO_VERUM(campus_est(x, "visus", "mensis"));
    CREDO_VERUM(ordines_sunt(x, ""));
}

/* electio, perfectio, translatio, dies alius, deletio, revocatio,
 * ictus */
interior vacuum
_probare_acta_diei (
    CredoContextus* c)
{
                Mundus* x;
    constans AgendaRes* r;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    clavis_specialis(x, CLAVIS_REDITUS, XIII, XIII, ZEPHYRUM);
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "2"));
    clavis(x, 'j');
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "3"));
    clavis(x, 'k');
    clavis(x, 'k');
    clavis(x, 'k');
    CREDO_VERUM(campus_est(x, "res_electa", "1"));
    /* x: perficere hodie, iterum aperire */
    clavis(x, 'x');
    r = agenda_res(x->documentum, I);
    CREDO_VERUM(r && r->perfecta && r->dies_perfectionis.dies == XIV);
    clavis(x, 'x');
    CREDO_FALSUM(agenda_res(x->documentum, I)->perfecta);
    /* >: res I ad diem XV; visus manet, electio ad proximam */
    clavis(x, '>');
    CREDO_VERUM(agenda_res(x->documentum, I)->dies.dies == XV);
    CREDO_VERUM(ordines_sunt(x, "r2 r3"));
    CREDO_VERUM(campus_est(x, "res_electa", "2"));
    /* l/h: dies alius */
    clavis(x, 'l');
    CREDO_VERUM(ordines_sunt(x, "r1"));
    clavis(x, 'h');
    CREDO_VERUM(ordines_sunt(x, "r2 r3"));
    /* X delere, u revocare, Ctrl-R reficere */
    clavis(x, 'X');
    CREDO_NIHIL(agenda_res(x->documentum, II));
    CREDO_VERUM(ordines_sunt(x, "r3"));
    clavis(x, 'u');
    CREDO_VERUM(ordines_sunt(x, "r2 r3"));
    clavis_specialis(x, (clavis_t)'r', XVIII, 'r', (i32)MOD_IMPERIUM);
    CREDO_VERUM(ordines_sunt(x, "r3"));
    /* ictus in ordine eligit */
    clavis(x, 'u');
    CREDO_VERUM(ictus_in(x, "r3", c->piscina));
    CREDO_VERUM(campus_est(x, "res_electa", "3"));
}

/* textus typare (clavibus singulis) */
interior vacuum
typare (
                Mundus* x,
    constans character* textus)
{
    constans character* q;

    per (q = textus; *q; q++)
    {
        clavis(x, *q);
    }
}

/* retrorsum n vicibus */
interior vacuum
retro (
    Mundus* x,
       i32  n)
{
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        clavis_specialis(x, CLAVIS_RETRORSUM, CXXVII, CXXVII, ZEPHYRUM);
    }
}

interior vacuum
reditus (
    Mundus* x)
{
    clavis_specialis(x, CLAVIS_REDITUS, XIII, XIII, ZEPHYRUM);
}

interior vacuum
effugium (
    Mundus* x)
{
    clavis_specialis(x, CLAVIS_EFFUGIUM, XXVII, XXVII, ZEPHYRUM);
}

/* prompta (A3b): n res nova, a nota, c titulus, D dies; Enter
 * committit, Esc claudit; quota duplex et '<' in textu servantur */
interior vacuum
_probare_promptam (
    CredoContextus* c)
{
                Mundus* x;
    constans AgendaRes* r;
            AgendaNota* nota;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    reditus(x);
    /* n: res nova in die XIV, electa */
    CREDO_NIHIL(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("prompta", c->piscina)));
    clavis(x, 'n');
    CREDO_VERUM(campus_est(x, "prompta", "nova"));
    CREDO_NON_NIHIL(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("prompta", c->piscina)));
    typare(x, "panis");
    reditus(x);
    CREDO_VERUM(campus_est(x, "prompta", "nulla"));
    r = agenda_res(x->documentum, V);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus, "panis");
    CREDO_VERUM(r->dies_posita && r->dies.dies == XIV);
    CREDO_VERUM(campus_est(x, "res_electa", "5"));
    /* a: nota cum quota duplici, die hodierna */
    clavis(x, 'a');
    typare(x, "dic \"salve\" <hic>");
    reditus(x);
    r = agenda_res(x->documentum, V);
    CREDO_NECESSE(r && xar_numerus(r->notae) == I);
    nota = (AgendaNota*)xar_obtinere(r->notae, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(nota->textus, "dic \"salve\" <hic>");
    CREDO_VERUM(nota->dies.dies == XIV);
    /* c: titulus praeimpletus, retrorsum, additio */
    clavis(x, 'c');
    CREDO_VERUM(campus_est(x, "prompta", "titulus"));
    retro(x, II);
    typare(x, "em");
    reditus(x);
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res(x->documentum, V)->titulus,
        "panem");
    /* D: praeimpleta die rei; vacuata, dies nova scripta */
    clavis(x, 'D');
    CREDO_VERUM(campus_est(x, "prompta", "dies"));
    CREDO_VERUM(campus_est(x, "prompta_textus", "2026-10-14"));
    retro(x, X);
    typare(x, "2026-10-20");
    reditus(x);
    CREDO_VERUM(agenda_res(x->documentum, V)->dies.dies == XX);
    CREDO_VERUM(campus_est(x, "prompta", "nulla"));
    /* res V abiit: electio in ordine eodem (A6b: apertae primum,
     * ordines 1 5 2 3 -> 1 2 3: res II, perfecta); dies mala
     * recusatur (prompta manet), Esc claudit sine mutatione */
    CREDO_VERUM(campus_est(x, "res_electa", "2"));
    clavis(x, 'D');
    retro(x, X);
    typare(x, "2026-13-01");
    reditus(x);
    CREDO_VERUM(campus_est(x, "prompta", "dies"));
    effugium(x);
    CREDO_VERUM(campus_est(x, "prompta", "nulla"));
    CREDO_VERUM(agenda_res(x->documentum, III)->dies.dies == XX);
    /* n et Esc: nihil creatur */
    clavis(x, 'n');
    typare(x, "x");
    effugium(x);
    CREDO_NIHIL(agenda_res(x->documentum, VI));
    CREDO_VERUM(campus_est(x, "visus", "dies"));
}

/* textus in finibus (puncto initii) qui partem continet */
interior b32
textus_in_continet (
                 Mandata* md,
                   Fines  f,
      constans character* pars)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS && punctum_intra(x->fines, f)
            && chorda_continet(x->textus, chorda_ex_literis(pars,
                   md->piscina)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* textus partem continens in finibus colore thematis dato? */
interior b32
textus_coloris_est (
                 Mandata* md,
                   Fines  f,
      constans character* pars,
                     i32  signum)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS && punctum_intra(x->fines, f)
            && chorda_continet(x->textus, chorda_ex_literis(pars,
                   md->piscina)))
        {
            redde color_thematis_est(x->color, signum);
        }
    }
    redde FALSUM;
}

interior Fines
fines_componentis (
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

interior Mandata*
reddere (
     Mundus* x,
    Piscina* p)
{
    Mandata* md;

    md = mandata_creare(p, x->intern);
    pingere(dispensator_arbor(x->d), x->m.figurae, ZEPHYRUM, md);
    redde md;
}

/* punctum (x, y) intra fines? */
interior b32
locus_intra (
      s32 x,
      s32 y,
    Fines f)
{
    redde x >= f.x && y >= f.y && x < f.x + f.latitudo
        && y < f.y + f.altitudo;
}

/* capsa picta (IX x IX, margo) in finibus; facta: lineae signi (II) */
interior b32
capsa_in (
    Mandata* md,
      Fines  f,
        b32  facta)
{
    constans Mandatum* y;
                  i32  i;
                  b32  capsa;
                  i32  signa;

    capsa = FALSUM;
    signa = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus          == MANDATUM_RECTANGULUM && !y->impletum
            && y->fines.latitudo == IX && y->fines.altitudo == IX
            && locus_intra(y->fines.x, y->fines.y, f))
        {
            capsa = VERUM;
        }
        si (   y->genus == MANDATUM_LINEA && y->crassitudo == II
            && locus_intra(y->puncta[ZEPHYRUM].x, y->puncta[ZEPHYRUM].y,
            f))
        {
            signa++;
        }
    }
    redde capsa && (facta ? signa >= II : signa == ZEPHYRUM);
}

/* linea horizontalis tenuis (per titulum perfectum) in finibus? */
interior b32
linea_transversa_in (
    Mandata* md,
      Fines  f)
{
    constans Mandatum* y;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus == MANDATUM_LINEA && y->crassitudo == I
            && y->puncta[ZEPHYRUM].y == y->puncta[I].y
            && y->puncta[ZEPHYRUM].y > f.y + II
            && y->puncta[ZEPHYRUM].y < f.y + f.altitudo - II
            && locus_intra(y->puncta[ZEPHYRUM].x, y->puncta[ZEPHYRUM].y,
            f))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* fascia accentus (III px, COLOR_ACCENT_PRIMARY) ad oram sinistram? */
interior b32
accentus_in (
    Mandata* md,
      Fines  f)
{
    constans Mandatum* y;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus          == MANDATUM_RECTANGULUM && y->impletum
            && y->fines.x        == f.x && y->fines.y == f.y
            && y->fines.latitudo == III
            && color_thematis_est(y->color, COLOR_ACCENT_PRIMARY))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* fascia gregis: exemplar leve per fines? */
interior b32
fascia_in (
    Mandata* md,
      Fines  f)
{
    constans Mandatum* y;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus          == MANDATUM_RECTANGULUM
            && y->exemplar       == EXEMPLAR_CINEREUM_LEVE
            && y->fines.x        == f.x && y->fines.y == f.y
            && y->fines.latitudo == f.latitudo)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* rectangulum plenum COLOR_SUPERFICIES (campus V2a) ? */
interior b32
campus_superficiei_adest (
    Mandata* md)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus == MANDATUM_RECTANGULUM && y->impletum
            && color_thematis_est(y->color, COLOR_SUPERFICIES)
            && y->fines.altitudo < XX)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* figurae visus diei: titulus, ordines (capsa, electio), notae,
 * prompta */
interior vacuum
_probare_figuras_diei (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
      Fines  f;
        i32  i;
        b32  electus;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(agendam_parare(x, c));
    (vacuum)agenda_notam_addere(x->documentum, I, fasti_dies(MMXXVI, X,
        XIII), chorda_ex_literis("panem emere", c->piscina));
    reditus(x);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_ullus_continet(md,
        "Wednesday, October 14, 2026"));
    CREDO_VERUM(textus_fontis_est(md, "Wednesday, October 14, 2026",
        FONS_TITULI));
    f = fines_componentis(x, "r1", c->piscina);
    /* V2b: capsa picta (non textus "[ ]"), fascia accentus electi */
    CREDO_VERUM(capsa_in(md, f, FALSUM));
    CREDO_FALSUM(textus_in_continet(md, f, "[ ]"));
    CREDO_VERUM(accentus_in(md, f));
    CREDO_VERUM(textus_in_continet(md, f, "panem emere"));
    CREDO_VERUM(textus_in_continet(md, f, "Oct 13"));
    /* ordo electus: nota colore textus pleno (non obscuro - in fundo
     * selectionis legi non poterat, imago A3c) */
    CREDO_VERUM(textus_coloris_est(md, f, "panem emere", COLOR_TEXT));
    /* r2 perfecta: capsa cum signo, linea per titulum */
    CREDO_VERUM(capsa_in(md, fines_componentis(x, "r2", c->piscina),
        VERUM));
    CREDO_VERUM(linea_transversa_in(md, fines_componentis(x, "r2",
        c->piscina)));
    /* r3: alibi assignata - dies sua */
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "r3",
        c->piscina), "Oct 20"));
    /* ordo electus (r1): fundus selectionis */
    electus = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus   == MANDATUM_RECTANGULUM && y->impletum
            && y->fines.y == f.y && y->fines.altitudo == f.altitudo
            && color_thematis_est(y->color, COLOR_SELECTION))
        {
            electus = VERUM;
        }
    }
    CREDO_VERUM(electus);
    /* prompta: genus, textus, cursor */
    clavis(x, 'a');
    typare(x, "abc");
    md = reddere(x, c->piscina);
    /* figura promptae in spatio componentis sui pingit */
    CREDO_NON_NIHIL(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("prompta", c->piscina)));
    /* V2a: titulus generis ante campum, textus in campo */
    CREDO_VERUM(textus_ullus_continet(md, "note:"));
    CREDO_VERUM(textus_ullus_continet(md, "abc"));
    CREDO_VERUM(campus_superficiei_adest(md));
    /* titulus promptae rubricarum (antea "date:") */
    effugium(x);
    clavis(x, 'r');
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_ullus_continet(md, "rubrics:"));
}

/* visus agendae (A4a): res apertae per rubricas (ordine canonico,
 * intra greges dies, sine die ultimae; sine rubrica ultimae); res in
 * gregibus duobus bis; j/k iterata praetereunt; x perficit (abit); n
 * rem sine die creat */
interior vacuum
_probare_visum_agendae (
    CredoContextus* c)
{
                Mundus* x;
    constans AgendaRes* r;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_visus_parare(x, c));
    dispensator_recomponere(x->d);
    CREDO_VERUM(campus_est(x, "visus", "agenda"));
    CREDO_VERUM(ordines_sunt(x, "r1-0 r5-0 r5-1 r6-2"));
    CREDO_NON_NIHIL(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("g1", c->piscina)));
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "5"));
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "6"));
    clavis(x, 'k');
    CREDO_VERUM(campus_est(x, "res_electa", "5"));
    clavis(x, 'k');
    CREDO_VERUM(campus_est(x, "res_electa", "1"));
    clavis(x, 'x');
    CREDO_VERUM(agenda_res(x->documentum, I)->perfecta);
    CREDO_VERUM(ordines_sunt(x, "r5-0 r5-1 r6-2"));
    CREDO_VERUM(campus_est(x, "res_electa", "5"));
    clavis(x, 'n');
    typare(x, "lac");
    reditus(x);
    r = agenda_res(x->documentum, VII);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(r->dies_posita);
    CREDO_VERUM(ordines_sunt(x, "r5-0 r5-1 r6-2 r7-2"));
    /* Esc sine filtro: visus manet */
    effugium(x);
    CREDO_VERUM(campus_est(x, "visus", "agenda"));
}

/* rubricae rei = "a b" ? */
interior b32
rubricae_rei_sunt (
                 Mundus* x,
                    s32  id,
     constans character* exspectatae)
{
     constans AgendaRes* r;
              character  visae[CXXVIII];
                    i32  i;
                    i32  k;
                 chorda* c;

    r = agenda_res(x->documentum, id);
    si (!r)
    {
        redde FALSUM;
    }
    k                = ZEPHYRUM;
    visae[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; i < xar_numerus(r->rubricae); i++)
    {
        c = (chorda*)xar_obtinere(r->rubricae, i);
        si (i > ZEPHYRUM)
        {
            visae[k++] = ' ';
        }
        memcpy(visae + k, c->datum, (size_t)c->mensura);
        k         += c->mensura;
        visae[k]  = '\0';
    }
    redde strcmp(visae, exspectatae) == ZEPHYRUM;
}

/* A4b: r rubricae (praeimpletae; mala recusatur), f filtrum (grex
 * unus, sine rubrica occultae; n rubricam filtri dat; Esc tollit;
 * persistens) */
interior vacuum
_probare_rubricas_filtrum (
    CredoContextus* c)
{
     Mundus* x;
     Mundus  y;
     chorda* a;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_visus_parare(x, c));
    dispensator_recomponere(x->d);
    /* electio in re VI (sine rubrica); r opus */
    clavis(x, 'j');
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "6"));
    clavis(x, 'r');
    CREDO_VERUM(campus_est(x, "prompta", "rubricae"));
    typare(x, "opus");
    reditus(x);
    CREDO_VERUM(rubricae_rei_sunt(x, VI, "opus"));
    CREDO_VERUM(ordines_sunt(x, "r1-0 r5-0 r6-1 r5-1"));
    /* praeimpletio et recusatio (ordines r1 r5 r6 r5: k ex VI = V) */
    clavis(x, 'k');
    CREDO_VERUM(campus_est(x, "res_electa", "5"));
    clavis(x, 'r');
    CREDO_VERUM(campus_est(x, "prompta_textus", "domus opus"));
    typare(x, " Bad");
    reditus(x);
    CREDO_VERUM(campus_est(x, "prompta", "rubricae"));
    effugium(x);
    CREDO_VERUM(rubricae_rei_sunt(x, V, "domus opus"));
    /* f domus */
    clavis(x, 'f');
    CREDO_VERUM(campus_est(x, "prompta", "filtrum"));
    typare(x, "domus");
    reditus(x);
    CREDO_VERUM(campus_est(x, "rubrica_filtrum", "domus"));
    CREDO_VERUM(ordines_sunt(x, "r1-0 r5-0"));
    /* n sub filtro: rubrica domus */
    clavis(x, 'n');
    typare(x, "lac");
    reditus(x);
    CREDO_VERUM(rubricae_rei_sunt(x, VII, "domus"));
    CREDO_VERUM(ordines_sunt(x, "r1-0 r5-0 r7-0"));
    /* persistens: montatio altera filtrum restituit */
    memset(&y, ZEPHYRUM, magnitudo(y));
    y.intern      = x->intern;
    y.volumen     = x->volumen;
    y.documentum  = x->documentum;
    CREDO_NECESSE(mundum_struere(&y, c->piscina, NIHIL));
    a = insula_ramus_attributum(&y.m.ramus, "rubrica_filtrum");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "domus"));
    /* Esc filtrum tollit */
    effugium(x);
    CREDO_VERUM(insula_ramus_attributum(&x->m.ramus, "rubrica_filtrum")
        == NIHIL);
    CREDO_VERUM(ordines_sunt(x, "r1-0 r5-0 r7-0 r6-1 r5-1"));
}

/* figurae visus agendae (A4c): titulus, tituli gregum, ordines cum
 * diebus, electa cum notis, filtrum in titulo */
interior vacuum
_probare_figuras_agendae (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
      Fines  f;
        i32  i;
        b32  electus;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(agendam_visus_parare(x, c));
    (vacuum)agenda_notam_addere(x->documentum, I, fasti_dies(MMXXVI, X,
        XIII), chorda_ex_literis("panem emere", c->piscina));
    dispensator_recomponere(x->d);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_ullus_continet(md, "Agenda: 3 open"));
    CREDO_VERUM(textus_fontis_est(md, "Agenda: 3 open", FONS_TITULI));
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "g0",
        c->piscina), "#domus"));
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "g1",
        c->piscina), "#opus"));
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "g2",
        c->piscina), "(none)"));
    /* V2b: fascia gregis (exemplar leve) */
    CREDO_VERUM(fascia_in(md, fines_componentis(x, "g0", c->piscina)));
    f = fines_componentis(x, "r1-0", c->piscina);
    CREDO_VERUM(capsa_in(md, f, FALSUM));
    CREDO_VERUM(textus_in_continet(md, f, "res"));
    CREDO_VERUM(textus_in_continet(md, f, "Oct 14"));
    CREDO_VERUM(textus_in_continet(md, f, "panem emere"));
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "r6-2",
        c->piscina), "Oct 20"));
    /* r5: sine die */
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "r5-0",
        c->piscina), "-"));
    electus = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus   == MANDATUM_RECTANGULUM && y->impletum
            && y->fines.y == f.y && y->fines.altitudo == f.altitudo
            && color_thematis_est(y->color, COLOR_SELECTION))
        {
            electus = VERUM;
        }
    }
    CREDO_VERUM(electus);
    /* filtrum in titulo */
    clavis(x, 'f');
    typare(x, "domus");
    reditus(x);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_ullus_continet(md, "#domus"));
    CREDO_VERUM(textus_ullus_continet(md, "Agenda: 2 open"));
}

/* rectangulum exemplari foci (ut scriba et pictor) ? */
interior b32
exemplar_foci_adest (
    Mandata* md)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus    == MANDATUM_RECTANGULUM
            && y->exemplar == EXEMPLAR_PUNCTA_DUPLICIA_DISPERSA)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* rectangulum plenum COLOR_SUPERFICIES per fines radicis totos ? */
interior b32
superficies_tota_adest (
     Mundus* x,
    Mandata* md)
{
    Componens* radix;
          i32  i;

    radix = dispensator_arbor(x->d);
    per (i = ZEPHYRUM; radix && i < xar_numerus(md->elementa); i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus          == MANDATUM_RECTANGULUM && y->impletum
            && y->fines.latitudo == radix->fines.latitudo
            && y->fines.altitudo == radix->fines.altitudo
            && color_thematis_est(y->color, COLOR_SUPERFICIES))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* mensa (ut scriba et pictor): superficies circum calendarium,
 * exemplar foci dum latus focatum; omnes visus intra marginem */
interior vacuum
_probare_mensam (
    CredoContextus* c)
{
     Mundus* x;
      Motus* motus;
    Mandata* md;
      Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(mundum_struere(x, c->piscina, "2026-10"));
    motus           = dispensator_motus(x->d);
    motus->spatium  = x->m.ramus.via;
    dispensator_recomponere(x->d);
    md = reddere(x, c->piscina);
    CREDO_VERUM(exemplar_foci_adest(md));
    CREDO_VERUM(superficies_tota_adest(x, md));
    /* titulus et cellulae intra marginem */
    f = fines_componentis(x, "titulus", c->piscina);
    CREDO_VERUM(f.x > ZEPHYRUM && f.y > ZEPHYRUM);
    f = fines_componentis(x, "2026-09-27", c->piscina);
    CREDO_VERUM(f.x > ZEPHYRUM);
    /* latus aliud focatum: superficies sine exemplari */
    motus->spatium = chorda_ex_literis("scriba:alia", c->piscina);
    dispensator_recomponere(x->d);
    md = reddere(x, c->piscina);
    CREDO_FALSUM(exemplar_foci_adest(md));
    /* visus agendae et diei intra marginem */
    CREDO_NECESSE(kalendarium_argumentum_ponere(&x->m, "agenda"));
    dispensator_recomponere(x->d);
    f = fines_componentis(x, "titulus", c->piscina);
    CREDO_VERUM(f.x > ZEPHYRUM && f.y > ZEPHYRUM);
}

/* tabula mandatorum: littera cum Cmd aut Ctrl mandatum NON est (Cmd
 * chorda etiam ut clavis advenit - "Cut" menu) - littera nuda est */
interior vacuum
_probare_mandata (
    CredoContextus* c)
{
                Mundus* x;
    constans AgendaRes* r;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    reditus(x);
    clavis_specialis(x, (clavis_t)'x', 'x', 'x', (i32)MOD_SUPER);
    r = agenda_res(x->documentum, I);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(r->perfecta);
    clavis_specialis(x, (clavis_t)'x', 'x', 'x', (i32)MOD_IMPERIUM);
    r = agenda_res(x->documentum, I);
    CREDO_FALSUM(r->perfecta);
    clavis(x, 'x');
    r = agenda_res(x->documentum, I);
    CREDO_VERUM(r->perfecta);
}

/* mus (M1a): ictus duplex in cellula diem aperit; ictus in capsa "[ ]"
 * rem perficit, alibi in ordine solum eligit */
interior vacuum
_probare_murem (
    CredoContextus* c)
{
                 Mundus* x;
     constans AgendaRes* r;
                  Fines  f;
                   Dies  d;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    CREDO_NECESSE(ictus_in(x, "2026-10-20", c->piscina));
    CREDO_FALSUM(campus_est(x, "visus", "dies"));
    CREDO_NECESSE(ictus_in(x, "2026-10-20", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.mensis == X && d.dies == XX);
    /* ad diem XIV: Esc, ictus simplex, Enter */
    effugium(x);
    CREDO_NECESSE(ictus_in(x, "2026-10-14", c->piscina));
    reditus(x);
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    /* ictus in capsa rei I */
    f = fines_componentis(x, "r1", c->piscina);
    CREDO_NECESSE(f.altitudo > ZEPHYRUM);
    ictus_ad(x, f.x + X, f.y + IV);
    r = agenda_res(x->documentum, I);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_VERUM(r->perfecta);
    /* ictus in titulo rei II: electio sola */
    f = fines_componentis(x, "r2", c->piscina);
    ictus_ad(x, f.x + C, f.y + IV);
    CREDO_VERUM(campus_est(x, "res_electa", "2"));
    r = agenda_res(x->documentum, II);
    CREDO_VERUM(r && r->perfecta);
    r = agenda_res(x->documentum, I);
    CREDO_VERUM(r && r->perfecta);
}

/* sagittae (polygona plena, V2a) */
interior i32
sagittas_numerare (
    Mandata* md)
{
    i32 i;
    i32 n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        si (((constans Mandatum*)xar_obtinere(md->elementa, i))->genus
            == MANDATUM_POLYGONUM)
        {
            n++;
        }
    }
    redde n;
}

/* globuli tituli (M1b): < > Today et Month Day Agenda; ictus mandatum
 * nominatum currit; globuli visus currentis fundo selectionis */
interior b32
globulum_adest (
                Mundus* x,
    constans character* id,
               Piscina* p)
{
    Componens* c;

    c = componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis(id, p));
    redde c && c->partes == PARTES_BOTTONE
        && c->fines.latitudo > ZEPHYRUM;
}

interior vacuum
_probare_globulos (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
       Dies  d;
      Fines  f;
        i32  i;
        b32  electus;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(agendam_parare(x, c));
    CREDO_VERUM(globulum_adest(x, "praecedens", c->piscina));
    CREDO_VERUM(globulum_adest(x, "sequens", c->piscina));
    CREDO_VERUM(globulum_adest(x, "hodie", c->piscina));
    CREDO_VERUM(globulum_adest(x, "visus-mensis", c->piscina));
    CREDO_VERUM(globulum_adest(x, "visus-dies", c->piscina));
    CREDO_VERUM(globulum_adest(x, "visus-agenda", c->piscina));
    /* mensis: > mensis proximus, < < duo priores, Today hodie */
    CREDO_NECESSE(ictus_in(x, "sequens", c->piscina));
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.mensis == XI);
    CREDO_NECESSE(ictus_in(x, "praecedens", c->piscina));
    CREDO_NECESSE(ictus_in(x, "praecedens", c->piscina));
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.mensis == IX);
    CREDO_NECESSE(ictus_in(x, "hodie", c->piscina));
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.mensis == X && d.dies == XIV);
    /* V2a: sagittae triangula (polygona plena) */
    md = reddere(x, c->piscina);
    CREDO_VERUM(sagittas_numerare(md) >= II);
    /* globuli "Month" activi: fundus selectionis et textus */
    md  = reddere(x, c->piscina);
    f   = fines_componentis(x, "visus-mensis", c->piscina);
    CREDO_VERUM(textus_ullus_continet(md, "Month"));
    electus = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus          == MANDATUM_RECTANGULUM && y->impletum
            && y->fines.latitudo == f.latitudo
            && color_thematis_est(y->color, COLOR_SELECTION))
        {
            electus = VERUM;
        }
    }
    CREDO_VERUM(electus);
    /* Day: visus diei; > dies proximus */
    CREDO_NECESSE(ictus_in(x, "visus-dies", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    CREDO_NECESSE(ictus_in(x, "sequens", c->piscina));
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.dies == XV);
    /* Agenda: sagittae et Today absunt; Month redit */
    CREDO_NECESSE(ictus_in(x, "visus-agenda", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "agenda"));
    CREDO_FALSUM(globulum_adest(x, "praecedens", c->piscina));
    CREDO_FALSUM(globulum_adest(x, "hodie", c->piscina));
    CREDO_NECESSE(ictus_in(x, "visus-mensis", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "mensis"));
}

/* agenda perfectae (A6a): post greges apertos "p" (Completed), per
 * diem perfectionis "p<k>" novissimo primum, ordines "q<id>" (id
 * descendens intra diem); titulus solas apertas numerat; x in q
 * reaperit; filtrum sectionem quoque filtrat */
interior vacuum
_probare_perfectas_agendae (
    CredoContextus* c)
{
                Mundus* x;
               Mandata* md;
    constans AgendaRes* r;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(agendam_visus_parare(x, c));
    (vacuum)agenda_perficere(x->documentum, VI, fasti_dies(MMXXVI, X,
        XIII));
    dispensator_recomponere(x->d);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_ullus_continet(md, "Agenda: 2 open"));
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "p",
        c->piscina), "Completed"));
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "p0",
        c->piscina), "Oct 14"));
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "p1",
        c->piscina), "Oct 13"));
    CREDO_VERUM(   fines_componentis(x, "p0", c->piscina).y
                 < fines_componentis(x, "q3", c->piscina).y);
    CREDO_VERUM(   fines_componentis(x, "q3", c->piscina).y
                 < fines_componentis(x, "q2", c->piscina).y);
    CREDO_VERUM(   fines_componentis(x, "q2", c->piscina).y
                 < fines_componentis(x, "p1", c->piscina).y);
    CREDO_VERUM(   fines_componentis(x, "p1", c->piscina).y
                 < fines_componentis(x, "q6", c->piscina).y);
    CREDO_VERUM(capsa_in(md, fines_componentis(x, "q3", c->piscina),
        VERUM));
    /* ictus in q2 (titulus) eligit; x reaperit - ad greges apertos */
    {
        Fines f;

        f = fines_componentis(x, "q2", c->piscina);
        ictus_ad(x, f.x + C, f.y + IV);
    }
    CREDO_VERUM(campus_est(x, "res_electa", "2"));
    clavis(x, 'x');
    r = agenda_res(x->documentum, II);
    CREDO_VERUM(r && !r->perfecta);
    CREDO_VERUM(fines_componentis(x, "q2", c->piscina).altitudo
        == ZEPHYRUM);
    CREDO_VERUM(fines_componentis(x, "r2-2", c->piscina).altitudo
        > ZEPHYRUM);
    /* filtrum domus: nulla perfecta domus - sectio abest */
    clavis(x, 'f');
    typare(x, "domus");
    reditus(x);
    CREDO_VERUM(fines_componentis(x, "p", c->piscina).altitudo
        == ZEPHYRUM);
}

/* sagittae obscurae (polygona COLOR_TEXT_DIM): A6b, pagina nulla */
interior i32
sagittae_obscurae (
    Mandata* md)
{
    constans Mandatum* y;
                  i32  i;
                  i32  n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus == MANDATUM_POLYGONUM
            && color_thematis_est(y->color, COLOR_TEXT_DIM))
        {
            n++;
        }
    }
    redde n;
}

/* componens adest (altitudo > 0)? */
interior b32
adest (
                 Mundus* x,
     constans character* id,
                Piscina* p)
{
    redde fines_componentis(x, id, p).altitudo > ZEPHYRUM;
}

/* paginae (A6b): visus diei 256 px alti - index IX cellulas (dimidium
 * corporis minus caput), IV ordines per paginam. Apertae (1, 5-10)
 * primum, deinde "c" et perfectae (2, 3, et 11 assignata XIV perfecta
 * XVI: "done Oct 16"); titulus gregis numquam ultimus paginae; j trans
 * paginas; [ ] et globuli; + New; dies mutatus -> pagina prima */
interior vacuum
_probare_paginas (
    CredoContextus* c)
{
                 Mundus* x;
                Mandata* md;
                    s32  id;
                   Dies  d;
                    i32  i;
                  Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(agendam_parare(x, c));
    d = fasti_dies(MMXXVI, X, XIV);
    per (i = ZEPHYRUM; i < VI; i++)
    {
        id = agenda_rem_creare(x->documentum, chorda_ex_literis("plus",
            c->piscina));
        (vacuum)agenda_diem_ponere(x->documentum, id, &d);
    }
    id = agenda_rem_creare(x->documentum, chorda_ex_literis("serius",
        c->piscina));
    (vacuum)agenda_diem_ponere(x->documentum, id, &d);
    (vacuum)agenda_perficere(x->documentum, id, fasti_dies(MMXXVI, X,
        XVI));
    reditus(x);
    /* pagina I: r1 r5 r6 r7 */
    CREDO_VERUM(adest(x, "r1", c->piscina));
    CREDO_VERUM(adest(x, "r7", c->piscina));
    CREDO_FALSUM(adest(x, "r8", c->piscina));
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x,
        "index_paginae",
        c->piscina), "1/3"));
    /* pagina prima: sola sagitta prior obscura */
    CREDO_AEQUALIS_I32(sagittae_obscurae(md), I);
    CREDO_VERUM(adest(x, "nova", c->piscina));
    /* pagina II: r8 r9 r10; "c" non ultimus - ad paginam III */
    clavis(x, ']');
    CREDO_VERUM(adest(x, "r8", c->piscina));
    CREDO_VERUM(adest(x, "r10", c->piscina));
    /* pagina media: nulla obscura */
    md = reddere(x, c->piscina);
    CREDO_AEQUALIS_I32(sagittae_obscurae(md), ZEPHYRUM);
    CREDO_FALSUM(adest(x, "r1", c->piscina));
    CREDO_FALSUM(adest(x, "c", c->piscina));
    /* pagina III: c, perfectae, "done Oct 16" */
    CREDO_NECESSE(ictus_in(x, "pagina-proxima", c->piscina));
    CREDO_VERUM(adest(x, "c", c->piscina));
    CREDO_VERUM(adest(x, "r2", c->piscina));
    CREDO_VERUM(adest(x, "r3", c->piscina));
    CREDO_VERUM(   fines_componentis(x, "c", c->piscina).y
                 < fines_componentis(x, "r2", c->piscina).y);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "r11",
        c->piscina), "done Oct 16"));
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x,
        "index_paginae",
        c->piscina), "3/3"));
    /* [ et globulus prior */
    clavis(x, '[');
    CREDO_VERUM(adest(x, "r8", c->piscina));
    CREDO_NECESSE(ictus_in(x, "pagina-prior", c->piscina));
    CREDO_VERUM(adest(x, "r1", c->piscina));
    /* j ex ordine ultimo paginae I: res 8 electa, pagina II */
    f = fines_componentis(x, "r7", c->piscina);
    ictus_ad(x, f.x + C, f.y + IV);
    CREDO_VERUM(campus_est(x, "res_electa", "7"));
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "8"));
    CREDO_VERUM(adest(x, "r8", c->piscina));
    CREDO_FALSUM(adest(x, "r7", c->piscina));
    /* dies mutatus et redditus: pagina prima */
    clavis(x, 'l');
    clavis(x, 'h');
    CREDO_VERUM(adest(x, "r1", c->piscina));
    /* + New: prompta nova */
    CREDO_NECESSE(ictus_in(x, "nova", c->piscina));
    CREDO_VERUM(campus_est(x, "prompta", "nova"));
}

/* notae diei (A6d-1): liber "dies/<ISO>" in dimidio infero visus
 * diei (scriba nidificata, spatium suum); liber diem sequitur */
interior vacuum
_probare_notas (
    CredoContextus* c)
{
               Mundus* x;
              Mandata* md;
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;
            Componens* pagina;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_kalendarium");
    CREDO_NECESSE_NON_NIHIL(x->volumen);
    x->documentum = agenda_aperire(c->piscina, x->intern, x->volumen,
        "agenda", ZEPHYRUM);
    /* liber XIV praeparatus: "quinque" in pagina prima */
    l = scriba_liber_aperire_in_spatio(c->piscina, x->intern,
        x->volumen,
        "dies/2026-10-14", XL, VI);
    CREDO_NECESSE_NON_NIHIL(l);
    d = scriba_liber_pagina(l, chorda_ex_literis("1", c->piscina));
    CREDO_NECESSE_NON_NIHIL(d);
    tabula_initiare(&folium, c->piscina, d->latitudo, d->altitudo);
    memcpy(folium.cellulae, scriba_documentum_tabula(d)->cellulae,
        (memoriae_index)(d->latitudo * d->altitudo));
    memcpy(&tabula_cellula(&folium, ZEPHYRUM, ZEPHYRUM), "quinque",
        VII);
    CREDO_NECESSE(scriba_documentum_committere(d, &folium) > ZEPHYRUM);
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    /* mensis: notae absunt */
    CREDO_FALSUM(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("pagina", c->piscina)) != NIHIL);
    reditus(x);
    pagina = componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("pagina", c->piscina));
    CREDO_NECESSE_NON_NIHIL(pagina);
    /* dimidium inferum: pagina sub capite indicis */
    CREDO_VERUM(pagina->fines.y >= ZEPHYRUM);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_ullus_continet(md, "quinque"));
    /* dies XV: liber suus (vacuus); XIV redit */
    clavis(x, 'l');
    md = reddere(x, c->piscina);
    CREDO_FALSUM(textus_ullus_continet(md, "quinque"));
    clavis(x, 'h');
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_ullus_continet(md, "quinque"));
}

/* textus commissus (imprimibiles, ut fenestra) */
interior vacuum
textum_mittere (
                 Mundus* x,
     constans character* t)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                   = EVENTUS_TEXTUS;
    e.datum.textus.genus      = EVENTUS_TEXTUS_COMMISSUM;
    e.datum.textus.contentum  = chorda_ex_literis(t, x->m.piscina);
    dispensator_tractare(x->d, &e);
}

/* x in pagina I libri diei (lectio nova ex volumine)? */
interior b32
littera_in_libro (
                 Mundus* x,
     constans character* praefixum,
              character  littera,
                Piscina* p)
{
                   ScribaLiber* l;
              ScribaDocumentum* d;
    constans TabulaCharacterum* t;
                           i32  i;

    l = scriba_liber_aperire_in_spatio(p, x->intern, x->volumen,
        praefixum, LXXII, XXX);
    d = l ? scriba_liber_pagina(l, chorda_ex_literis("1", p)) : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    t = scriba_documentum_tabula(d);
    per (i = ZEPHYRUM; i < t->latitudo * t->altitudo; i++)
    {
        si (t->cellulae[i] == (i8)littera)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* focus in notis (A6d-2): ictus in charta catenam scribit; i x Esc in
 * librum XIV; claves indicis interea nihil faciunt; Esc secundus ad
 * indicem (visus manet); Tab ad notas et retro */
interior vacuum
_probare_focum_notarum (
    CredoContextus* c)
{
     Mundus* x;
      Motus* motus;
      Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_kalendarium");
    CREDO_NECESSE_NON_NIHIL(x->volumen);
    x->documentum = agenda_aperire(c->piscina, x->intern, x->volumen,
        "agenda", ZEPHYRUM);
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    motus = dispensator_motus(x->d);
    /* hospes gestum ponit (ut vicus in foco lateris) */
    kalendarium_gestum_ponere(motus, &x->m);
    reditus(x);
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    f = fines_componentis(x, "notae", c->piscina);
    CREDO_NECESSE(f.altitudo > ZEPHYRUM);
    ictus_ad(x, f.x + XL, f.y + XXX);
    CREDO_VERUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_notae", c->piscina)));
    /* imprimibiles per TEXTUM (scriba: "per TEXTUM venit") */
    textum_mittere(x, "i");
    textum_mittere(x, "x");
    effugium(x);
    (vacuum)motus_gestum_effundere(motus, x->repo);
    CREDO_VERUM(littera_in_libro(x, "dies/2026-10-14", 'x',
        c->piscina));
    /* claves indicis non agunt: res electa et dies manent */
    clavis(x, 'l');
    CREDO_VERUM(kalendarium_dies_electus(&x->m).dies == XIV);
    /* Esc secundus: ad indicem, visus diei manet */
    effugium(x);
    CREDO_FALSUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_notae", c->piscina)));
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    /* Tab ad notas, Tab (modo normali) retro */
    clavis_specialis(x, CLAVIS_TABULA, '\t', '\t', ZEPHYRUM);
    CREDO_VERUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_notae", c->piscina)));
    clavis_specialis(x, CLAVIS_TABULA, '\t', '\t', ZEPHYRUM);
    CREDO_FALSUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_notae", c->piscina)));
    /* in indice claves iterum agunt */
    clavis(x, 'l');
    CREDO_VERUM(kalendarium_dies_electus(&x->m).dies == XV);
}

/* A6e: charta notarum cellulis exacta - pagina diei ex superficie
 * montationis (CCCLX x CCCXX: LIII x IX), area scribae = (pagina + II)
 * x (pagina + III) cellulae intra locum "notae"; indicium paginae
 * kalendarium_reficere sequitur (Ctrl-Shift-Dextra) */
interior vacuum
_probare_chartam_notarum (
    CredoContextus* c)
{
              Mundus* x;
               Motus* motus;
           Componens* scriba;
           Componens* locus;
         InsulaRamus  rs;
               Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_kalendarium");
    CREDO_NECESSE_NON_NIHIL(x->volumen);
    x->documentum = agenda_aperire(c->piscina, x->intern, x->volumen,
        "agenda", ZEPHYRUM);
    x->latitudo = CCCLX;
    x->altitudo = CCCXX;
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    motus = dispensator_motus(x->d);
    kalendarium_gestum_ponere(motus, &x->m);
    reditus(x);
    locus   = componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("notae", c->piscina));
    scriba  = componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("radix", c->piscina));
    CREDO_NECESSE(locus && scriba);
    CREDO_AEQUALIS_S32(scriba->fines.latitudo, (LIII + II) * VI);
    CREDO_AEQUALIS_S32(scriba->fines.altitudo, (IX + III) * VIII);
    CREDO_VERUM(   scriba->fines.x + scriba->fines.latitudo
                <= locus->fines.x + locus->fines.latitudo);
    CREDO_VERUM(   scriba->fines.y + scriba->fines.altitudo
                <= locus->fines.y + locus->fines.altitudo);
    /* Ctrl-Shift-Dextra in notis: pagina II; indicium post reficere */
    f = fines_componentis(x, "notae", c->piscina);
    ictus_ad(x, f.x + XL, f.y + XX);
    clavis_specialis(x, CLAVIS_DEXTER, ZEPHYRUM, ZEPHYRUM,
        (i32)(MOD_IMPERIUM | MOD_SHIFT));
    CREDO_VERUM(kalendarium_reficere(&x->m));
    rs = insula_ramus_ex_via(x->repo, motus->spatium_intimum);
    CREDO_VERUM(chorda_aequalis_literis(*insula_ramus_attributum(&rs,
        "pagina_positio"), "2"));
    CREDO_VERUM(chorda_aequalis_literis(*insula_ramus_attributum(&rs,
        "paginae_numerus"), "2"));
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
    { "visus_diei",   _probare_visum_diei,    mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "acta_diei",    _probare_acta_diei,     mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "prompta",      _probare_promptam,      mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "figurae_diei", _probare_figuras_diei,  mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "visus_agendae", _probare_visum_agendae, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "rubricae_filtrum", _probare_rubricas_filtrum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "figurae_agendae", _probare_figuras_agendae, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "mensa", _probare_mensam, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "mandata", _probare_mandata, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "mus", _probare_murem, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "globuli", _probare_globulos, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "perfectae_agendae", _probare_perfectas_agendae, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "paginae", _probare_paginas, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "notae", _probare_notas, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "notae_focus", _probare_focum_notarum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "charta_notarum", _probare_chartam_notarum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("kalendarium", SECTIONES);
}
