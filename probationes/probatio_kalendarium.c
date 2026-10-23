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
#include "conspectus.h"
#include "conspectus_dies.h"
#include "calendarium_liturgicum.h"
#include "fasti_fontes.h"
#include "fons.h"
#include "agenda.h"
#include "scriba_liber.h"
#include "scriba_documentum.h"
#include "tabula_characterum.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "exemplaria.h"
#include "semita.h"
#include "organum_montatio.h"
#include "pensa.h"
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

/* id numeratoris (moneta_numerator): n -> "n" */
interior chorda
id_ex (
         s32  n,
     Piscina* p)
{
    redde chorda_ex_s32(n, p);
}

nomen structura {
      InternamentumChorda* intern;
       InsulaRepositorium* repo;
      KalendariumMontatio  m;
              Dispensator* d;
                  Volumen* volumen;
          StatusSystemata  systemata;
         AgendaDocumentum* documentum;
              FastiFontes* fontes;     /* P6: dies signati */
                      s32  apertum;    /* P6b: referentia reddita */
                      s32  latitudo;   /* A6e: superficies montat. */
                      s32  altitudo;
         constans Organum* organum;     /* semitae R4: semita_legere */
                      i32  numerator;   /* moneta_numerator: "1", "2" */
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
            argumentum, x->documentum, x->fontes, x->volumen,
            (i32)x->latitudo,
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
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = c->fines.x + c->fines.latitudo / II;
    e.datum.mus.y       = c->fines.y + c->fines.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(x->d, &e);
    redde VERUM;
}

/* ictus bottone dato in puncto (spatium radicis) */
interior vacuum
ictus_bottone_ad (
          Mundus* x,
             s32  px,
             s32  py,
    mus_botton_t  botton)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = px;
    e.datum.mus.y       = py;
    e.datum.mus.botton  = botton;
    dispensator_tractare(x->d, &e);
}

/* ictus sinister in puncto (spatium radicis) */
interior vacuum
ictus_ad (
    Mundus* x,
       s32  px,
       s32  py)
{
    ictus_bottone_ad(x, px, py, MUS_SINISTER);
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
                 chorda  a;
                 chorda  b;
                   Dies  d;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_kalendarium");
    CREDO_NECESSE_NON_NIHIL(x->volumen);
    x->documentum = agenda_aperire(c->piscina, x->intern, x->volumen,
        "agenda", ZEPHYRUM);
    agenda_monetam_ponere(x->documentum, moneta_numerator,
        &x->numerator);
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
 * XIV (hic perfecta), 4 deleta; SINE montatione (K5: libri ante
 * montationem scribendi - editor formae librum "pensa" in montatione
 * cum pagina loci creat) */
interior b32
agendam_struere (
                Mundus* x,
        CredoContextus* c)
{
    chorda id;
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
    agenda_monetam_ponere(x->documentum, moneta_numerator,
        &x->numerator);
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
    (vacuum)agenda_perficere(x->documentum, id_ex(II, c->piscina), d);
    (vacuum)agenda_perficere(x->documentum, id_ex(III, c->piscina), d);
    (vacuum)agenda_rem_delere(x->documentum, id_ex(IV, c->piscina));
    redde VERUM;
}

/* agenda diei XIV (agendam_struere); dein montatio */
interior b32
agendam_parare (
                Mundus* x,
        CredoContextus* c)
{
    redde agendam_struere(x, c) && mundum_struere(x, c->piscina, NIHIL);
}

/* agenda cum argumento "agenda": fixa agendae diei (1 domus XIV, 2 et
 * 3 perfectae, 4 deleta) + 5 sine die "opus domus" + 6 XX sine
 * rubrica */
interior b32
agendam_visus_parare (
                Mundus* x,
        CredoContextus* c)
{
    chorda id;
      Dies d;

    si (!agendam_parare(x, c))
    {
        redde FALSUM;
    }
    (vacuum)agenda_rubricas_ponere(x->documentum, id_ex(I, c->piscina),
        chorda_ex_literis(
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
    r = agenda_res(x->documentum, id_ex(I, c->piscina));
    CREDO_VERUM(r && r->perfecta && r->dies_perfectionis.dies == XIV);
    clavis(x, 'x');
    CREDO_FALSUM(agenda_res(x->documentum, id_ex(I,
        c->piscina))->perfecta);
    /* >: res I ad diem XV; visus manet, electio ad proximam */
    clavis(x, '>');
    CREDO_VERUM(agenda_res(x->documentum, id_ex(I,
        c->piscina))->dies.dies == XV);
    CREDO_VERUM(ordines_sunt(x, "r2 r3"));
    CREDO_VERUM(campus_est(x, "res_electa", "2"));
    /* l/h: dies alius */
    clavis(x, 'l');
    CREDO_VERUM(ordines_sunt(x, "r1"));
    clavis(x, 'h');
    CREDO_VERUM(ordines_sunt(x, "r2 r3"));
    /* X delere, u revocare, Ctrl-R reficere */
    clavis(x, 'X');
    CREDO_NIHIL(agenda_res(x->documentum, id_ex(II, c->piscina)));
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
    r = agenda_res(x->documentum, id_ex(V, c->piscina));
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus, "panis");
    CREDO_VERUM(r->dies_posita && r->dies.dies == XIV);
    CREDO_VERUM(campus_est(x, "res_electa", "5"));
    /* a: nota cum quota duplici, die hodierna */
    clavis(x, 'a');
    typare(x, "dic \"salve\" <hic>");
    reditus(x);
    r = agenda_res(x->documentum, id_ex(V, c->piscina));
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
    CREDO_CHORDA_AEQUALIS_LITERIS(agenda_res(x->documentum, id_ex(V,
        c->piscina))->titulus,
        "panem");
    /* D: praeimpleta die rei; vacuata, dies nova scripta */
    clavis(x, 'D');
    CREDO_VERUM(campus_est(x, "prompta", "dies"));
    CREDO_VERUM(campus_est(x, "prompta_textus", "2026-10-14"));
    retro(x, X);
    typare(x, "2026-10-20");
    reditus(x);
    CREDO_VERUM(agenda_res(x->documentum, id_ex(V,
        c->piscina))->dies.dies == XX);
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
    CREDO_VERUM(agenda_res(x->documentum, id_ex(III,
        c->piscina))->dies.dies == XX);
    /* n et Esc: nihil creatur */
    clavis(x, 'n');
    typare(x, "x");
    effugium(x);
    CREDO_NIHIL(agenda_res(x->documentum, id_ex(VI, c->piscina)));
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
    (vacuum)agenda_notam_addere(x->documentum, id_ex(I, c->piscina),
        fasti_dies(MMXXVI, X,
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
    CREDO_VERUM(agenda_res(x->documentum, id_ex(I,
        c->piscina))->perfecta);
    CREDO_VERUM(ordines_sunt(x, "r5-0 r5-1 r6-2"));
    CREDO_VERUM(campus_est(x, "res_electa", "5"));
    clavis(x, 'n');
    typare(x, "lac");
    reditus(x);
    r = agenda_res(x->documentum, id_ex(VII, c->piscina));
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
                 chorda  id,
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
    CREDO_VERUM(rubricae_rei_sunt(x, id_ex(VI, c->piscina), "opus"));
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
    CREDO_VERUM(rubricae_rei_sunt(x, id_ex(V, c->piscina),
        "domus opus"));
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
    CREDO_VERUM(rubricae_rei_sunt(x, id_ex(VII, c->piscina), "domus"));
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
    (vacuum)agenda_notam_addere(x->documentum, id_ex(I, c->piscina),
        fasti_dies(MMXXVI, X,
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
    r = agenda_res(x->documentum, id_ex(I, c->piscina));
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(r->perfecta);
    clavis_specialis(x, (clavis_t)'x', 'x', 'x', (i32)MOD_IMPERIUM);
    r = agenda_res(x->documentum, id_ex(I, c->piscina));
    CREDO_FALSUM(r->perfecta);
    clavis(x, 'x');
    r = agenda_res(x->documentum, id_ex(I, c->piscina));
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
    r = agenda_res(x->documentum, id_ex(I, c->piscina));
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_VERUM(r->perfecta);
    /* ictus in titulo rei II: electio sola */
    f = fines_componentis(x, "r2", c->piscina);
    ictus_ad(x, f.x + C, f.y + IV);
    CREDO_VERUM(campus_est(x, "res_electa", "2"));
    r = agenda_res(x->documentum, id_ex(II, c->piscina));
    CREDO_VERUM(r && r->perfecta);
    r = agenda_res(x->documentum, id_ex(I, c->piscina));
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
    (vacuum)agenda_perficere(x->documentum, id_ex(VI, c->piscina),
        fasti_dies(MMXXVI, X,
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
    r = agenda_res(x->documentum, id_ex(II, c->piscina));
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
                 chorda  id;
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

/* P6c: argumentum "YYYY-MM-DD" - montatio et latus exstans in visum
 * diei illius diei; malum (dies nulla, forma mala) FALSUM, status
 * manet */
interior vacuum
_probare_argumentum_diei (
    CredoContextus* c)
{
    Mundus* x;
      Dies  d;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c->piscina, "2026-12-25"));
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.annus == MMXXVI && d.mensis == XII && d.dies == XXV);
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "2027-03-02"));
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.annus == MMXXVII && d.mensis == III && d.dies == II);
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    CREDO_FALSUM(kalendarium_argumentum_ponere(&x->m, "2026-02-30"));
    CREDO_FALSUM(kalendarium_argumentum_ponere(&x->m, "2026-10-2"));
    CREDO_FALSUM(kalendarium_argumentum_ponere(&x->m, "2026-1x-02"));
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.annus == MMXXVII && d.mensis == III && d.dies == II);
    /* semitae R4: "YYYY-MM" = calendarium/mensis/<dies> - visus
     * mensis (mos prior visum non mutabat) */
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "2026-10"));
    CREDO_VERUM(campus_est(x, "visus", "mensis"));
}

/* P6c: latus in visu mensis - argumentum "YYYY-MM-DD" in visum diei
 * vertit */
interior vacuum
_probare_argumentum_diei_ex_mense (
    CredoContextus* c)
{
    Mundus* x;
      Dies  d;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    CREDO_NECESSE(campus_est(x, "visus", NIHIL)
        || campus_est(x, "visus", "mensis"));
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "2026-12-25"));
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.annus == MMXXVI && d.mensis == XII && d.dies == XXV);
}

/* montatio cum die mala recusatur */
interior vacuum
_probare_argumentum_diei_malum (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_FALSUM(mundum_struere(x, c->piscina, "2026-13-01"));
}

/* semitae R4: organum calendarii pro semita_legere; ctx = Mundus* */
interior constans Organum*
organa_calendarii (
     vacuum* ctx,
     chorda  genus)
{
    redde chorda_aequalis_literis(genus, "calendarium")
        ? ((Mundus*)ctx)->organum : NIHIL;
}

/* semita currens lateris ut URI aequat exspectatam? */
interior b32
semita_est (
                 Mundus* x,
                Piscina* p,
     constans character* exspectata)
{
    chorda uri;

    si (!x->organum)
    {
        x->organum = organum_legere_genus(p, x->intern, "calendarium",
            "probatio");
    }
    uri = semita_ad_uri(organa_calendarii, x, semita_legere(
        organa_calendarii, x, &x->m.ramus, "calendarium", p, x->intern),
        p);
    si (!chorda_aequalis_literis(uri, exspectata))
    {
        imprimere("semita: '%.*s', exspectata '%s'\n", (int)uri.mensura,
            (constans character*)uri.datum, exspectata);
        redde FALSUM;
    }
    redde VERUM;
}

/* semitae R4: argumentum = URI semitae aut cognomen ("YYYY-MM",
 * "YYYY-MM-DD", "agenda"); semita canonica legitur; ligatione mutata
 * res_electa et pagina resettuntur; mala nihil mutant */
interior vacuum
_probare_semitas (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    /* initium: mensis, hodie - R4d: ligatio semper nominata */
    CREDO_VERUM(semita_est(x, c->piscina, "calendarium/mensis"));
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m,
        "calendarium/dies/2026-10-19?res_electa=17"));
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    CREDO_VERUM(semita_est(x, c->piscina,
        "calendarium/dies/2026-10-19?res_electa=17"));
    /* status in eadem ligatione: res_electa servatur */
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m,
        "calendarium/dies/2026-10-19?pagina=2"));
    CREDO_VERUM(semita_est(x, c->piscina,
        "calendarium/dies/2026-10-19?res_electa=17&pagina=2"));
    /* cognomen diei: ligatio mutata - res_electa, pagina absentes */
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "2026-12-25"));
    CREDO_VERUM(semita_est(x, c->piscina,
        "calendarium/dies/2026-12-25"));
    /* cognomen mensis: visus mensis, dies prima */
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "2027-03"));
    CREDO_VERUM(semita_est(x, c->piscina,
        "calendarium/mensis/2027-03-01"));
    /* cognomen agendae: R4d ligatio tota - dies ad hodie */
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "agenda"));
    CREDO_VERUM(semita_est(x, c->piscina, "calendarium/agenda"));
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m,
        "calendarium/agenda?rubrica_filtrum=domus"));
    CREDO_VERUM(semita_est(x, c->piscina,
        "calendarium/agenda?rubrica_filtrum=domus"));
    /* semitae R4e: valores symbolici ad limen resoluti (hodie fixa
     * 2026-10-14) - cognomen typatum et URI */
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "hodie"));
    CREDO_VERUM(semita_est(x, c->piscina,
        "calendarium/dies/2026-10-14?rubrica_filtrum=domus"));
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "cras"));
    CREDO_VERUM(semita_est(x, c->piscina,
        "calendarium/dies/2026-10-15?rubrica_filtrum=domus"));
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m,
        "calendarium/mensis/heri"));
    CREDO_VERUM(semita_est(x, c->piscina,
        "calendarium/mensis/2026-10-13?rubrica_filtrum=domus"));
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m,
        "calendarium/agenda?rubrica_filtrum=domus"));
    /* mala: nihil mutatur */
    CREDO_FALSUM(kalendarium_argumentum_ponere(&x->m,
        "calendarium/dies/2026-02-30"));
    CREDO_FALSUM(kalendarium_argumentum_ponere(&x->m,
        "calendarium/hora"));
    CREDO_FALSUM(kalendarium_argumentum_ponere(&x->m,
        "calendarium/dies/2026-10-19?prompta=nova"));
    CREDO_FALSUM(kalendarium_argumentum_ponere(&x->m, "sancti/index"));
    CREDO_VERUM(semita_est(x, c->piscina,
        "calendarium/agenda?rubrica_filtrum=domus"));
}

/* semitam applicare ut vicus (historia): scriptura directa, sine
 * argumento */
interior b32
semitam_applicare (
                 Mundus* x,
                Piscina* p,
     constans character* uri)
{
     StmlNodus* semita;
        chorda  vitium;

    si (!x->organum)
    {
        x->organum = organum_legere_genus(p, x->intern, "calendarium",
            "probatio");
    }
    semita = semita_ex_uri(organa_calendarii, x, chorda_ex_literis(uri,
        p), p, x->intern, &vitium);
    redde semita && semita_applicare(organa_calendarii, x, &x->m.ramus,
        semita, p, &vitium);
}

/* semitae R4: semita a vico applicata (historia) diem mutat sine
 * argumento - liber notarum sequitur solum post
 * kalendarium_semitam_secuta */
interior vacuum
_probare_semitam_secutam (
    CredoContextus* c)
{
               Mundus* x;
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_kalendarium");
    CREDO_NECESSE_NON_NIHIL(x->volumen);
    x->documentum = agenda_aperire(c->piscina, x->intern, x->volumen,
        "agenda", ZEPHYRUM);
    agenda_monetam_ponere(x->documentum, moneta_numerator,
        &x->numerator);
    l = scriba_liber_aperire_in_spatio(c->piscina, x->intern,
        x->volumen, "dies/2026-10-14", XL, VI);
    CREDO_NECESSE_NON_NIHIL(l);
    d = scriba_liber_pagina(l, chorda_ex_literis("1", c->piscina));
    CREDO_NECESSE_NON_NIHIL(d);
    tabula_initiare(&folium, c->piscina, d->latitudo, d->altitudo);
    memcpy(folium.cellulae, scriba_documentum_tabula(d)->cellulae,
        (memoriae_index)(d->latitudo * d->altitudo));
    memcpy(&tabula_cellula(&folium, ZEPHYRUM, ZEPHYRUM), "quinque",
        VII);
    CREDO_NECESSE(scriba_documentum_committere(d, &folium) > ZEPHYRUM);
    CREDO_NECESSE(mundum_struere(x, c->piscina, "2026-10-14"));
    CREDO_VERUM(textus_ullus_continet(reddere(x, c->piscina),
        "quinque"));
    /* dies XV per semitam: liber XIV manet donec hospes sequitur */
    CREDO_NECESSE(semitam_applicare(x, c->piscina,
        "calendarium/dies/2026-10-15"));
    CREDO_VERUM(textus_ullus_continet(reddere(x, c->piscina),
        "quinque"));
    CREDO_VERUM(kalendarium_semitam_secuta(&x->m));
    CREDO_FALSUM(textus_ullus_continet(reddere(x, c->piscina),
        "quinque"));
    /* XIV redit */
    CREDO_NECESSE(semitam_applicare(x, c->piscina,
        "calendarium/dies/2026-10-14"));
    CREDO_VERUM(kalendarium_semitam_secuta(&x->m));
    CREDO_VERUM(textus_ullus_continet(reddere(x, c->piscina),
        "quinque"));
    /* latus nullum: FALSUM */
    CREDO_FALSUM(kalendarium_semitam_secuta(NIHIL));
}

/* P6: fons fictus - Anna Rossi natalis 14 X (MCMLXXXVI), "Festa
 * urbis" genere ignoto 14 X, Luca Bianchi natalis 20 X (sine anno),
 * "Feriae" genere ignoto 22 X (solae); P6d: Marco Verdi natalis 31 X
 * (MM; sabbatum - columna dextra) */
nomen structura {
     constans character* genus;
     constans character* titulus;
                    s32  dies;
                    s32  origo;
} SignumFictum;

hic_manens constans SignumFictum signa_ficta[] = {
    { "natalis", "Anna Rossi", XIV, MCMLXXXVI },
    { "aliud", "Festa urbis", XIV, ZEPHYRUM },
    { "natalis", "Luca Bianchi", XX, ZEPHYRUM },
    { "aliud", "Feriae", XXII, ZEPHYRUM },
    { "natalis", "Marco Verdi", XXXI, MM }
};

#define SIGNA_FICTA_NUMERUS \
    ((i32)(magnitudo(signa_ficta) / magnitudo(signa_ficta[ZEPHYRUM])))

interior b32
fictum_legere (
      vacuum* ctx,
        Dies  prima,
        Dies  ultima,
         Xar* exitus,
     Piscina* piscina)
{
    FastiIntroitus* t;
              Dies  d;
               i32  i;

    (vacuum)ctx;
    per (i = ZEPHYRUM; i < SIGNA_FICTA_NUMERUS; i++)
    {
        d = fasti_dies(MMXXVI, X, signa_ficta[i].dies);
        si (   fasti_comparare(d, prima) < ZEPHYRUM
            || fasti_comparare(d, ultima) > ZEPHYRUM)
        {
            perge;
        }
        t = (FastiIntroitus*)xar_addere(exitus);
        memset(t, ZEPHYRUM, magnitudo(FastiIntroitus));
        t->dies            = d;
        t->genus           = chorda_ex_literis(signa_ficta[i].genus,
            piscina);
        t->titulus         = chorda_ex_literis(signa_ficta[i].titulus,
            piscina);
        t->annus_originis  = signa_ficta[i].origo;
        t->referentia      = chorda_ex_s32((s32)i + I, piscina);
    }
    redde VERUM;
}

/* P6b: referentia ultima fonti reddita in *ctx (s32; 0 = nulla) */
interior b32
fictum_aperire (
                     vacuum* ctx,
    constans FastiIntroitus* introitus)
{
    (vacuum)chorda_ut_s32(introitus->referentia, (s32*)ctx);
    redde VERUM;
}

interior FastiFontes*
fontes_ficti (
     Mundus* x,
    Piscina* p)
{
    FastiFontes* f;

    x->apertum  = ZEPHYRUM;
    f           = fasti_fontes_creare(p);
    redde f && fasti_fontes_addere(f, "fictus", fictum_legere,
        fictum_aperire, &x->apertum) ? f : NIHIL;
}

/* P6: placenta natalis intra cellulam? - linea glacieris (rectangulum
 * plenum XIII x I, linea V pictogrammatis) */
interior b32
natalis_in (
    Mandata* md,
      Fines  f)
{
    constans Mandatum* y;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus          == MANDATUM_RECTANGULUM && y->impletum
            && y->fines.latitudo == XIII && y->fines.altitudo == I
            && intra(y->fines, f))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* P6: visus mensis - signum natalis in cellulis XIV et XX; non in XV
 * nec in XXII (genus ignotum solum) */
interior vacuum
_probare_natales_mensis (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;

    x          = (Mundus*)c->datum;
    thema_initiare();
    x->fontes  = fontes_ficti(x, c->piscina);
    CREDO_NECESSE_NON_NIHIL(x->fontes);
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    md = reddere(x, c->piscina);
    CREDO_VERUM(natalis_in(md, cellula(x, "2026-10-14", c->piscina)));
    CREDO_VERUM(natalis_in(md, cellula(x, "2026-10-20", c->piscina)));
    CREDO_FALSUM(natalis_in(md, cellula(x, "2026-10-15", c->piscina)));
    CREDO_FALSUM(natalis_in(md, cellula(x, "2026-10-22", c->piscina)));
}

/* P6d: motus muris in puncto (spatium radicis), sine bottone */
interior vacuum
motus_ad (
    Mundus* x,
       s32  px,
       s32  py)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_MOTUS;
    e.datum.mus.x  = px;
    e.datum.mus.y  = py;
    dispensator_tractare(x->d, &e);
}

/* P6d: medium placentae cellulae id (angulus dexter superus) */
interior Punctum
placenta_media (
                 Mundus* x,
     constans character* id,
                Piscina* p)
{
      Fines f;
    Punctum q;

    f    = cellula(x, id, p);
    q.x  = f.x + f.latitudo - II - XIII / II;
    q.y  = f.y + II + XII / II;
    redde q;
}

/* P6d: fines textus partem continentis (latitudo 0 si abest) et
 * capsa impleta eum continens paulo ante pictus (titulus volans) */
interior Fines
textus_fines (
                Mandata* md,
     constans character* pars,
                    b32* in_capsa)
{
     constans Mandatum* y;
     constans Mandatum* z;
                 Fines  f;
                   i32  i;
                   i32  k;

    memset(&f, ZEPHYRUM, magnitudo(f));
    *in_capsa = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus != MANDATUM_TEXTUS
            || !chorda_continet(y->textus, chorda_ex_literis(pars,
                   md->piscina)))
        {
            perge;
        }
        f = y->fines;
        per (k = i - I; k >= ZEPHYRUM && k >= i - VIII; k--)
        {
            z = (constans Mandatum*)xar_obtinere(md->elementa, k);
            si (   z->genus == MANDATUM_RECTANGULUM && z->impletum
                && f.x > z->fines.x && f.y >= z->fines.y
                && f.x < z->fines.x + z->fines.latitudo
                && f.y < z->fines.y + z->fines.altitudo)
            {
                *in_capsa = VERUM;
            }
        }
        frange;
    }
    redde f;
}

/* P6d: titulus volans natalis - motus super placentam campum
 * natalis_super ponit et nomina pinguntur in capsa ad dextram
 * cursoris; extra placentam, ictus, mensis novus delent; cellula sine
 * natali nihil; in columna dextra capsa sinistrorsum vertitur intra
 * calendarium */
interior vacuum
_probare_natalis_super (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
    Punctum  q;
      Fines  f;
      Fines  radix;
     chorda* a;
        b32  capsa;

    x          = (Mundus*)c->datum;
    thema_initiare();
    x->fontes  = fontes_ficti(x, c->piscina);
    CREDO_NECESSE_NON_NIHIL(x->fontes);
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    md = reddere(x, c->piscina);
    CREDO_FALSUM(textus_ullus_continet(md, "Anna Rossi turns 40"));

    /* super placentam XIV */
    q = placenta_media(x, "2026-10-14", c->piscina);
    motus_ad(x, q.x, q.y);
    a = insula_ramus_attributum(&x->m.ramus, "natalis_super");
    CREDO_NECESSE(a != NIHIL);
    CREDO_VERUM(chorda_incipit(*a, chorda_ex_literis("2026-10-14 ",
        c->piscina)));
    md  = reddere(x, c->piscina);
    f   = textus_fines(md, "Anna Rossi turns 40", &capsa);
    CREDO_VERUM(f.x > q.x && f.y > q.y);
    CREDO_VERUM(capsa);

    /* medium cellulae (non placenta): deletum */
    f = cellula(x, "2026-10-14", c->piscina);
    motus_ad(x, f.x + f.latitudo / II, f.y + f.altitudo / II);
    a = insula_ramus_attributum(&x->m.ramus, "natalis_super");
    CREDO_VERUM(a == NIHIL || a->mensura == ZEPHYRUM);
    md = reddere(x, c->piscina);
    CREDO_FALSUM(textus_ullus_continet(md, "Anna Rossi turns 40"));

    /* linea placentae sed ad sinistram (numerus diei XIV): nihil */
    motus_ad(x, f.x + IV, f.y + VIII);
    a = insula_ramus_attributum(&x->m.ramus, "natalis_super");
    CREDO_VERUM(a == NIHIL || a->mensura == ZEPHYRUM);

    /* angulus cellulae sine natali (XV): nihil */
    q = placenta_media(x, "2026-10-15", c->piscina);
    motus_ad(x, q.x, q.y);
    a = insula_ramus_attributum(&x->m.ramus, "natalis_super");
    CREDO_VERUM(a == NIHIL || a->mensura == ZEPHYRUM);

    /* XX: Luca (sine anno); ictus delet */
    q = placenta_media(x, "2026-10-20", c->piscina);
    motus_ad(x, q.x, q.y);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_ullus_continet(md, "Luca Bianchi's birthday"));
    CREDO_NECESSE(ictus_in(x, "2026-10-21", c->piscina));
    a = insula_ramus_attributum(&x->m.ramus, "natalis_super");
    CREDO_VERUM(a == NIHIL || a->mensura == ZEPHYRUM);

    /* XXXI (sabbatum, columna dextra): capsa sinistrorsum, intra
     * radicem */
    radix  = fines_componentis(x, "kalendarium", c->piscina);
    q      = placenta_media(x, "2026-10-31", c->piscina);
    motus_ad(x, q.x, q.y);
    md  = reddere(x, c->piscina);
    f   = textus_fines(md, "Marco Verdi turns 26", &capsa);
    CREDO_NECESSE(f.x > ZEPHYRUM);
    CREDO_VERUM(capsa);
    CREDO_VERUM(f.x < q.x);
    CREDO_VERUM(f.x + (s32)XX * VI <= radix.x + radix.latitudo);

    /* mensis novus (L): deletum */
    clavis(x, 'L');
    a = insula_ramus_attributum(&x->m.ramus, "natalis_super");
    CREDO_VERUM(a == NIHIL || a->mensura == ZEPHYRUM);
}

/* P6: visus diei - ordines signati ("n<k>") supra res: "Anna Rossi
 * turns 40", genus ignotum titulo solo; ictus in eis electionem non
 * mutat; dies XX: "Luca Bianchi's birthday" */
interior vacuum
_probare_natales_diei (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
      Fines  n0;
      Fines  n1;
      Fines  r1;
     chorda* a;
     chorda  ante;
        i32  i;

    x          = (Mundus*)c->datum;
    thema_initiare();
    x->fontes  = fontes_ficti(x, c->piscina);
    CREDO_NECESSE_NON_NIHIL(x->fontes);
    CREDO_NECESSE(agendam_parare(x, c));
    reditus(x);
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    n0 = fines_componentis(x, "n0", c->piscina);
    n1 = fines_componentis(x, "n1", c->piscina);
    r1 = fines_componentis(x, "r1", c->piscina);
    CREDO_NECESSE(   n0.altitudo > ZEPHYRUM && n1.altitudo > ZEPHYRUM
                  && r1.altitudo > ZEPHYRUM);
    CREDO_VERUM(n0.y < n1.y && n1.y < r1.y);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_ullus_continet(md, "Anna Rossi turns 40"));
    CREDO_VERUM(textus_ullus_continet(md, "Festa urbis"));
    /* placenta in ordine natalis (n1), non in genere ignoto (n0) */
    CREDO_VERUM(natalis_in(md, n1));
    CREDO_FALSUM(natalis_in(md, n0));
    a = insula_ramus_attributum(&x->m.ramus, "res_electa");
    ante = a ? chorda_transcribere(*a, c->piscina)
             : chorda_ex_literis("", c->piscina);
    CREDO_NECESSE(ictus_in(x, "n1", c->piscina));
    a = insula_ramus_attributum(&x->m.ramus, "res_electa");
    CREDO_VERUM(a ? chorda_aequalis(*a, ante) : ante.mensura
        == ZEPHYRUM);
    per (i = ZEPHYRUM; i < VI; i++)
    {
        clavis(x, 'l');
    }
    CREDO_VERUM(kalendarium_dies_electus(&x->m).dies == XX);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_ullus_continet(md, "Luca Bianchi's birthday"));
    CREDO_FALSUM(textus_ullus_continet(md, "Anna Rossi"));
}

/* P6b: ictus duplex in ordine signato introitum fonti suo reddit
 * (etiam in placenta, loco capsae); ictus simplex nihil */
interior vacuum
_probare_signatum_aperire (
    CredoContextus* c)
{
    Mundus* x;
     Fines  f;

    x          = (Mundus*)c->datum;
    thema_initiare();
    x->fontes  = fontes_ficti(x, c->piscina);
    CREDO_NECESSE_NON_NIHIL(x->fontes);
    CREDO_NECESSE(agendam_parare(x, c));
    reditus(x);
    CREDO_NECESSE(campus_est(x, "visus", "dies"));
    /* n1 = Anna Rossi (referentia I); simplex nihil */
    CREDO_NECESSE(ictus_in(x, "n1", c->piscina));
    CREDO_AEQUALIS_S32(x->apertum, ZEPHYRUM);
    CREDO_NECESSE(ictus_in(x, "n1", c->piscina));
    CREDO_AEQUALIS_S32(x->apertum, I);
    /* n0 = Festa urbis (referentia II), in placenta (x < capsa) */
    f = fines_componentis(x, "n0", c->piscina);
    CREDO_NECESSE(f.altitudo > ZEPHYRUM);
    ictus_ad(x, f.x + VIII, f.y + IV);
    ictus_ad(x, f.x + VIII, f.y + IV);
    CREDO_AEQUALIS_S32(x->apertum, II);
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
    agenda_monetam_ponere(x->documentum, moneta_numerator,
        &x->numerator);
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
    agenda_monetam_ponere(x->documentum, moneta_numerator,
        &x->numerator);
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
    agenda_monetam_ponere(x->documentum, moneta_numerator,
        &x->numerator);
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

/* rubricae in linea (n, c): "#verbum" e titulo in rubricas
 * (minusculis; c addit, non tollit); invalidum in titulo manet; ictus
 * duplex in ordine titulum edit */
interior vacuum
_probare_rubricas_in_linea (
    CredoContextus* c)
{
                Mundus* x;
    constans AgendaRes* r;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    reditus(x);
    clavis(x, 'n');
    typare(x, "panem #domus #Opus");
    reditus(x);
    r = agenda_res(x->documentum, id_ex(V, c->piscina));
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus, "panem");
    CREDO_VERUM(rubricae_rei_sunt(x, id_ex(V, c->piscina),
        "domus opus"));
    /* c: titulus novus, rubrica addita (domus opus manent) */
    CREDO_VERUM(campus_est(x, "res_electa", "5"));
    clavis(x, 'c');
    retro(x, V);
    typare(x, "panis #urbs");
    reditus(x);
    r = agenda_res(x->documentum, id_ex(V, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus, "panis");
    CREDO_VERUM(rubricae_rei_sunt(x, id_ex(V, c->piscina),
        "domus opus urbs"));
    /* invalidum ("#a/b") in titulo manet */
    clavis(x, 'n');
    typare(x, "res #a/b");
    reditus(x);
    r = agenda_res(x->documentum, id_ex(VI, c->piscina));
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus, "res #a/b");
    CREDO_VERUM(rubricae_rei_sunt(x, id_ex(VI, c->piscina), ""));
    /* ictus duplex in ordine r1: electio et prompta tituli */
    CREDO_NECESSE(ictus_in(x, "r1", c->piscina));
    CREDO_NECESSE(ictus_in(x, "r1", c->piscina));
    CREDO_VERUM(campus_est(x, "res_electa", "1"));
    CREDO_VERUM(campus_est(x, "prompta", "titulus"));
}

/* ictus dexter in medio componentis id (radicis liberi) */
interior b32
ictus_dexter_in (
                 Mundus* x,
     constans character* id,
                Piscina* p)
{
    Fines f;

    f = fines_componentis(x, id, p);
    si (f.altitudo <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    ictus_bottone_ad(x, f.x + f.latitudo / II, f.y + f.altitudo / II,
        MUS_DEXTER);
    redde VERUM;
}

/* menu apertum: nodus "elenchus" (filius ULTIMUS radicis) aut NIHIL */
interior Componens*
elenchus_apertum (
    Mundus* x)
{
    Componens* radix;
    Componens* u;

    radix = dispensator_arbor(x->d);
    si (!radix || componens_numerus_liberorum(radix) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    u = componens_liberum(radix, componens_numerus_liberorum(radix)
        - I);
    redde chorda_aequalis_literis(u->id, "elenchus") ? u : NIHIL;
}

/* res menu "A|B|~C" (~ = obscura: sine claudit); "" = clausum */
interior b32
res_elenchi_sunt (
                 Mundus* x,
     constans character* exspectatae)
{
     Componens* e;
     Componens* r;
     character  visae[CCLVI];
           i32  i;

    visae[ZEPHYRUM]  = '\0';
    e                = elenchus_apertum(x);
    per (i = ZEPHYRUM; e && i < componens_numerus_liberorum(e); i++)
    {
        r = componens_liberum(e, i);
        si (i > ZEPHYRUM)
        {
            strcat(visae, "|");
        }
        si (!r->claudit)
        {
            strcat(visae, "~");
        }
        strncat(visae, (constans character*)r->titulus.datum,
            (size_t)r->titulus.mensura);
    }
    si (strcmp(visae, exspectatae) != ZEPHYRUM)
    {
        imprimere("  res elenchi: '%s'\n", visae);
        redde FALSUM;
    }
    redde VERUM;
}

/* punctum medium rei k menu (spatium radicis) */
interior b32
punctum_rei_elenchi (
      Mundus* x,
         i32  k,
     Punctum* p)
{
    Componens* e;
    Componens* r;

    e = elenchus_apertum(x);
    si (!e || k >= componens_numerus_liberorum(e))
    {
        redde FALSUM;
    }
    r     = componens_liberum(e, k);
    p->x  = e->fines.x + r->fines.x + r->fines.latitudo / II;
    p->y  = e->fines.y + r->fines.y + r->fines.altitudo / II;
    redde VERUM;
}

/* ictus bottone dato iuxta menu apertum (dextrorsum si spatium est,
 * aliter sinistrorsum) - extra menu */
interior b32
ictus_extra_elenchum (
          Mundus* x,
    mus_botton_t  botton)
{
     Componens* e;
     Componens* radix;
           s32  px;

    e      = elenchus_apertum(x);
    radix  = dispensator_arbor(x->d);
    si (!e || !radix)
    {
        redde FALSUM;
    }
    px = e->fines.x + e->fines.latitudo + IV;
    si (px >= radix->fines.latitudo)
    {
        px = e->fines.x - IV;
    }
    si (px < ZEPHYRUM)
    {
        redde FALSUM;
    }
    ictus_bottone_ad(x, px, e->fines.y + IV, botton);
    redde VERUM;
}

interior b32
ictus_in_rem_elenchi (
    Mundus* x,
       i32  k)
{
    Punctum p;

    si (!punctum_rei_elenchi(x, k, &p))
    {
        redde FALSUM;
    }
    ictus_ad(x, p.x, p.y);
    redde VERUM;
}

interior b32
motus_in_rem_elenchi (
    Mundus* x,
       i32  k)
{
    Punctum p;
    Eventus e;

    si (!punctum_rei_elenchi(x, k, &p))
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_MOTUS;
    e.datum.mus.x  = p.x;
    e.datum.mus.y  = p.y;
    dispensator_tractare(x->d, &e);
    redde VERUM;
}

/* greges et filtrum (M2a): ictus in titulo gregis rubricam eius
 * filtrat, in titulo filtri positi tollit; "(none)" nihil agit. Botton
 * dexter nec filtrat nec perficit (menu contextuale: M2c). */
interior vacuum
_probare_greges_filtrum (
    CredoContextus* c)
{
                Mundus* x;
    constans AgendaRes* r;
                 Fines  f;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_visus_parare(x, c));
    dispensator_recomponere(x->d);
    CREDO_VERUM(ordines_sunt(x, "r1-0 r5-0 r5-1 r6-2"));
    /* "#opus" (g1) */
    CREDO_NECESSE(ictus_in(x, "g1", c->piscina));
    CREDO_VERUM(campus_est(x, "rubrica_filtrum", "opus"));
    CREDO_VERUM(ordines_sunt(x, "r5-0"));
    /* titulus idem (sub filtro g0) filtrum tollit */
    CREDO_NECESSE(ictus_in(x, "g0", c->piscina));
    CREDO_VERUM(ordines_sunt(x, "r1-0 r5-0 r5-1 r6-2"));
    /* "(none)" (g2): nihil */
    CREDO_NECESSE(ictus_in(x, "g2", c->piscina));
    CREDO_VERUM(ordines_sunt(x, "r1-0 r5-0 r5-1 r6-2"));
    /* botton dexter in titulo: filtrum nullum */
    f = fines_componentis(x, "g1", c->piscina);
    CREDO_NECESSE(f.altitudo > ZEPHYRUM);
    ictus_bottone_ad(x, f.x + f.latitudo / II, f.y + f.altitudo / II,
        MUS_DEXTER);
    CREDO_VERUM(ordines_sunt(x, "r1-0 r5-0 r5-1 r6-2"));
    /* M2c: dexter menu aperuit - claudere, ne ictum sequentem
     * deglutiat */
    effugium(x);
    CREDO_VERUM(res_elenchi_sunt(x, ""));
    /* botton dexter in capsa: non perficit; sinister ibidem perficit
     * (custodia: punctum vere in capsa est) */
    f = fines_componentis(x, "r1-0", c->piscina);
    CREDO_NECESSE(f.altitudo > ZEPHYRUM);
    ictus_bottone_ad(x, f.x + X, f.y + IV, MUS_DEXTER);
    r = agenda_res(x->documentum, id_ex(I, c->piscina));
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(r->perfecta);
    /* M2c: menu rei apertum (agenda: sine res_ante/post) - claudere */
    CREDO_VERUM(res_elenchi_sunt(x, "Complete / reopen item|Edit title"
        "|Edit rubrics|Set date|Add note|Delete item"));
    effugium(x);
    ictus_ad(x, f.x + X, f.y + IV);
    r = agenda_res(x->documentum, id_ex(I, c->piscina));
    CREDO_VERUM(r && r->perfecta);
}

/* componens id supra omnes ordines (PARTES_ITEM) paginae ? */
interior b32
supra_ordines (
                 Mundus* x,
     constans character* id,
                Piscina* p)
{
     Componens* radix;
     Componens* l;
         Fines  f;
           i32  i;

    f      = fines_componentis(x, id, p);
    radix  = dispensator_arbor(x->d);
    si (f.altitudo <= ZEPHYRUM || !radix)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(radix); i++)
    {
        l = componens_liberum(radix, i);
        si (l->partes == PARTES_ITEM && l->fines.y <= f.y)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* ordines paginae intra superficiem: omnis res II cellulas alta et
 * supra marginem inferum (cellula una) - pagina non redundat */
interior b32
ordines_intra (
    Mundus* x)
{
     Componens* radix;
     Componens* l;
           i32  i;

    radix = dispensator_arbor(x->d);
    per (i = ZEPHYRUM; radix && i < componens_numerus_liberorum(radix);
         i++)
    {
        l = componens_liberum(radix, i);
        si (   l->partes == PARTES_ITEM
            && (   l->fines.altitudo != XVI
                || l->fines.y + l->fines.altitudo
                    > radix->fines.altitudo
                   - VIII))
        {
            imprimere("  redundat: %.*s (%d+%d)\n",
                (integer)l->id.mensura,
                (constans character*)l->id.datum,
                (integer)l->fines.y, (integer)l->fines.altitudo);
            redde FALSUM;
        }
    }
    redde radix != NIHIL;
}

/* res n novae: rubrica data (NIHIL = nulla), perfecta die data
 * (dies.annus 0 = aperta) */
interior vacuum
res_addere (
                 Mundus* x,
                    i32  n,
     constans character* rubrica,
                   Dies  perfectio,
                Piscina* p)
{
    chorda id;
       i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        id = agenda_rem_creare(x->documentum, chorda_ex_literis("res",
            p));
        si (rubrica)
        {
            (vacuum)agenda_rubricas_ponere(x->documentum, id,
                chorda_ex_literis(rubrica, p));
        }
        si (perfectio.annus > ZEPHYRUM)
        {
            (vacuum)agenda_perficere(x->documentum, id, perfectio);
        }
    }
}

/* tituli gregum iterati (Franus 10-14): pagina in medio gregis
 * incipiens titulum eius iterat ("<id>-c") - agenda (CCCLX x CCCXX:
 * XV ordines per paginam). Domus: g0 + XVI res (1, 5, 7-20) - pagina I
 * g0 + XIV, pagina II "g0-c" + 19, 20, g1, g2, p, p0 + VI perfectae;
 * perfectae XXVI (2, 3, 21-44) die XIV: pagina III "p0-c" + XIV,
 * pagina IV "p0-c" + VI */
interior vacuum
_probare_paginas_gregum (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
       Dies  nulla;

    x            = (Mundus*)c->datum;
    x->latitudo  = CCCLX;
    x->altitudo  = CCCXX;
    thema_initiare();
    CREDO_NECESSE(agendam_visus_parare(x, c));
    memset(&nulla, ZEPHYRUM, magnitudo(nulla));
    res_addere(x, XIV, "domus", nulla, c->piscina);
    res_addere(x, XXIV, NIHIL, fasti_dies(MMXXVI, X, XIV), c->piscina);
    dispensator_recomponere(x->d);
    CREDO_VERUM(adest(x, "g0", c->piscina));
    CREDO_VERUM(adest(x, "r18-0", c->piscina));
    CREDO_VERUM(ordines_intra(x));
    CREDO_FALSUM(adest(x, "r19-0", c->piscina));
    CREDO_FALSUM(adest(x, "g0-c", c->piscina));
    /* pagina II: titulus domus iteratus supra 19 */
    clavis(x, ']');
    CREDO_VERUM(adest(x, "g0-c", c->piscina));
    CREDO_FALSUM(adest(x, "g0", c->piscina));
    CREDO_VERUM(ordines_intra(x));
    CREDO_VERUM(supra_ordines(x, "g0-c", c->piscina));
    CREDO_VERUM(adest(x, "r20-0", c->piscina));
    CREDO_VERUM(adest(x, "g2", c->piscina));
    CREDO_VERUM(adest(x, "p0", c->piscina));
    CREDO_FALSUM(adest(x, "p0-c", c->piscina));
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "g0-c",
        c->piscina), "#domus"));
    /* pagina III: dies perfectionis iteratus */
    clavis(x, ']');
    CREDO_VERUM(adest(x, "p0-c", c->piscina));
    CREDO_FALSUM(adest(x, "p", c->piscina));
    CREDO_VERUM(ordines_intra(x));
    CREDO_VERUM(supra_ordines(x, "p0-c", c->piscina));
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "p0-c",
        c->piscina), "Oct 14"));
    /* pagina IV (ultima): iterum */
    clavis(x, ']');
    CREDO_VERUM(adest(x, "p0-c", c->piscina));
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x,
        "index_paginae", c->piscina), "4/4"));
    /* titulus iteratus filtrat (M2a) */
    clavis(x, '[');
    clavis(x, '[');
    CREDO_NECESSE(ictus_in(x, "g0-c", c->piscina));
    CREDO_VERUM(campus_est(x, "rubrica_filtrum", "domus"));
}

/* titulus numquam solus in fine paginae, etiam si titulus alius
 * sequitur (Franus 10-14): g0 + VIII res, g1, g2 = XIII ordines; "p"
 * (XIV) et "p0" (XV) caperent, prima perfecta non - "p" cum "p0" in
 * paginam II transit */
interior vacuum
_probare_titulum_solum (
    CredoContextus* c)
{
    Mundus* x;
      Dies  nulla;

    x            = (Mundus*)c->datum;
    x->latitudo  = CCCLX;
    x->altitudo  = CCCXX;
    CREDO_NECESSE(agendam_visus_parare(x, c));
    memset(&nulla, ZEPHYRUM, magnitudo(nulla));
    res_addere(x, VI, "domus", nulla, c->piscina);
    dispensator_recomponere(x->d);
    CREDO_VERUM(adest(x, "r6-2", c->piscina));
    CREDO_FALSUM(adest(x, "p", c->piscina));
    CREDO_FALSUM(adest(x, "p0", c->piscina));
    clavis(x, ']');
    CREDO_VERUM(adest(x, "p", c->piscina));
    CREDO_VERUM(adest(x, "p0", c->piscina));
    CREDO_VERUM(ordines_intra(x));
}

/* visus diei (CCCLX x CCCXX: VI ordines per paginam): r1, c, IV
 * perfectae; pagina II incipit in perfectis - "c-c" iteratus */
interior vacuum
_probare_paginas_gregum_diei (
    CredoContextus* c)
{
    Mundus* x;
      Dies  nulla;

    x            = (Mundus*)c->datum;
    x->latitudo  = CCCLX;
    x->altitudo  = CCCXX;
    CREDO_NECESSE(agendam_parare(x, c));
    memset(&nulla, ZEPHYRUM, magnitudo(nulla));
    res_addere(x, VIII, NIHIL, fasti_dies(MMXXVI, X, XIV), c->piscina);
    reditus(x);
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    CREDO_VERUM(adest(x, "c", c->piscina));
    CREDO_FALSUM(adest(x, "c-c", c->piscina));
    clavis(x, ']');
    CREDO_VERUM(adest(x, "c-c", c->piscina));
    CREDO_FALSUM(adest(x, "c", c->piscina));
    CREDO_VERUM(supra_ordines(x, "c-c", c->piscina));
    CREDO_VERUM(textus_in_continet(reddere(x, c->piscina),
        fines_componentis(x, "c-c", c->piscina), "Completed"));
    (vacuum)nulla;
}

/* menu diei (M2c): dexter in cellula eligit et aperit; claves movent,
 * Reditus agit et claudit; clavis alia deglutitur; ictus in re agit */
interior vacuum
_probare_elenchum_diei (
    CredoContextus* c)
{
    Mundus* x;
      Dies  d;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    CREDO_NECESSE(ictus_dexter_in(x, "2026-10-20", c->piscina));
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.mensis == X && d.dies == XX);
    CREDO_VERUM(res_elenchi_sunt(x, "Open day|Go to today|Agenda"));
    /* Deorsum bis (electa 0, 1), Reditus: Go to today, clausum */
    clavis_specialis(x, CLAVIS_DEORSUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(campus_est(x, "elenchus_electa", "0"));
    clavis_specialis(x, CLAVIS_DEORSUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(campus_est(x, "elenchus_electa", "1"));
    reditus(x);
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.mensis == X && d.dies == XIV);
    CREDO_VERUM(res_elenchi_sunt(x, ""));
    /* apertum: 'l' (dies proximus) deglutitur; electa ad nullam
     * redit */
    CREDO_NECESSE(ictus_dexter_in(x, "2026-10-20", c->piscina));
    CREDO_FALSUM(campus_est(x, "elenchus_electa", "1"));
    clavis(x, 'l');
    d = kalendarium_dies_electus(&x->m);
    CREDO_VERUM(d.dies == XX);
    /* ictus in "Agenda" (II): visus agendae, clausum */
    CREDO_NECESSE(ictus_in_rem_elenchi(x, II));
    CREDO_VERUM(campus_est(x, "visus", "agenda"));
    CREDO_VERUM(res_elenchi_sunt(x, ""));
}

/* menu rei et vacuum (M2c): dexter in ordine eligit; motus muris
 * illuminat; ictus agit; obscura deglutitur (menu manet); Esc claudit;
 * titulus nihil */
interior vacuum
_probare_elenchum_rei (
    CredoContextus* c)
{
                Mundus* x;
    constans AgendaRes* r;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    reditus(x);
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    CREDO_NECESSE(ictus_dexter_in(x, "r3", c->piscina));
    CREDO_VERUM(campus_est(x, "res_electa", "3"));
    /* dexter extra menu apertum: claudit et deglutitur */
    CREDO_NECESSE(ictus_extra_elenchum(x, MUS_DEXTER));
    CREDO_VERUM(campus_est(x, "res_electa", "3"));
    CREDO_VERUM(res_elenchi_sunt(x, ""));
    CREDO_NECESSE(ictus_dexter_in(x, "r1", c->piscina));
    CREDO_VERUM(campus_est(x, "res_electa", "1"));
    CREDO_VERUM(res_elenchi_sunt(x, "Complete / reopen item|Edit title"
        "|Edit rubrics|Set date|Add note|Move item to previous day"
        "|Move item to next day|Delete item"));
    CREDO_NECESSE(motus_in_rem_elenchi(x, I));
    CREDO_VERUM(campus_est(x, "elenchus_electa", "1"));
    CREDO_NECESSE(ictus_in_rem_elenchi(x, ZEPHYRUM));
    r = agenda_res(x->documentum, id_ex(I, c->piscina));
    CREDO_VERUM(r && r->perfecta);
    CREDO_VERUM(res_elenchi_sunt(x, ""));
    /* vacuum diei: sine filtris */
    CREDO_NECESSE(ictus_dexter_in(x, "index_vacuum", c->piscina));
    CREDO_VERUM(res_elenchi_sunt(x, "New item|Previous page|Next page"
        "|Undo|Redo"));
    effugium(x);
    CREDO_VERUM(res_elenchi_sunt(x, ""));
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    /* agenda: Clear filter obscura; ictus in ea nihil, menu manet */
    CREDO_NECESSE(kalendarium_argumentum_ponere(&x->m, "agenda"));
    dispensator_recomponere(x->d);
    CREDO_NECESSE(ictus_dexter_in(x, "index_vacuum", c->piscina));
    CREDO_VERUM(res_elenchi_sunt(x, "New item|Filter by rubric"
        "|~Clear filter|Previous page|Next page|Undo|Redo"));
    CREDO_NECESSE(ictus_in_rem_elenchi(x, II));
    CREDO_VERUM(res_elenchi_sunt(x, "New item|Filter by rubric"
        "|~Clear filter|Previous page|Next page|Undo|Redo"));
    effugium(x);
    /* titulus: nihil */
    CREDO_NECESSE(ictus_dexter_in(x, "titulus", c->piscina));
    CREDO_VERUM(res_elenchi_sunt(x, ""));
}

/* textus intra fines partem continens? */
interior b32
textus_continet_in (
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

/* duplex in medio componentis id */
interior b32
duplex_in (
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
    e.genus             = EVENTUS_MUS_DUPLEX;
    e.datum.mus.x       = c->fines.x + c->fines.latitudo / II;
    e.datum.mus.y       = c->fines.y + c->fines.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(x->d, &e);
    redde VERUM;
}

/* liber diei dati: lineae datae in pagina prima (scriptura vera) */
interior b32
librum_scribere (
                 Mundus* x,
     constans character* praefixum,
     constans character* textus,
                Piscina* p)
{
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    l = scriba_liber_aperire_in_spatio(p, x->intern, x->volumen,
        praefixum, XL, XL);
    d = l ? scriba_liber_pagina(l, chorda_ex_literis("1", p)) : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, p, d->latitudo,
        d->altitudo, textus);
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

/* proiectiones D4c: registrum commune ab hospite datum (vicus: unum
 * per volumen) - dies picti in EO computantur; kalendarium proprium
 * non creat. Actum agendae diem stalum facit, pictura proxima
 * recentem */
interior vacuum
_probare_conspectum_communem (
    CredoContextus* c)
{
                  Mundus* x;
     ConspectusRegistrum* r;
              DiesFontes  fontes;
                     Xar* vitia;
                  chorda  id;
                    Dies  d;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(agendam_parare(x, c));
    memset(&fontes, ZEPHYRUM, magnitudo(fontes));
    fontes.agenda    = x->documentum;
    fontes.liturgia  = calendarium_creare(c->piscina);
    fontes.volumen   = x->volumen;
    fontes.intern    = x->intern;
    r = conspectus_registrum_creare(c->piscina, x->intern, x->volumen,
        ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(conspectus_dies_registrare(r, &fontes, &vitia));
    CREDO_NECESSE(conspectus_parare(r, &vitia));
    CREDO_AEQUALIS_I32((i32)conspectus_status(r, "dies",
        chorda_ex_literis("2026-10-14", c->piscina)),
        (i32)CONSPECTUS_ABSENS);

    kalendarium_conspectum_ponere(&x->m, r);
    dispensator_recomponere(x->d);
    (vacuum)reddere(x, c->piscina);
    /* mensis X pictus (cellulae a XXVII IX): dies in registro
     * communi */
    CREDO_AEQUALIS_I32((i32)conspectus_status(r, "dies",
        chorda_ex_literis("2026-10-14", c->piscina)),
        (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32((i32)conspectus_status(r, "dies",
        chorda_ex_literis("2026-10-01", c->piscina)),
        (i32)CONSPECTUS_RECENS);

    id = agenda_rem_creare(x->documentum, chorda_ex_literis("nova",
        c->piscina));
    d  = fasti_dies(MMXXVI, X, XIV);
    (vacuum)agenda_diem_ponere(x->documentum, id, &d);
    CREDO_AEQUALIS_I32((i32)conspectus_status(r, "dies",
        chorda_ex_literis("2026-10-14", c->piscina)),
        (i32)CONSPECTUS_STALUM);
    dispensator_recomponere(x->d);
    (vacuum)reddere(x, c->piscina);
    CREDO_AEQUALIS_I32((i32)conspectus_status(r, "dies",
        chorda_ex_literis("2026-10-14", c->piscina)),
        (i32)CONSPECTUS_RECENS);
    conspectus_registrum_destruere(r);
}

/* visus hebdomadis: hebdomas diei electi (dominica prima, XI-XVII X);
 * per diem: natales, res apertae (perfectae non), lineae libri diei
 * (vacuae omissae); "+n" si plures quam capiuntur; liber absens non
 * creatur; claves j k J K t Enter Esc; ictus eligit, duplex aperit */
interior vacuum
_probare_hebdomadem (
    CredoContextus* c)
{
    Mundus* x;
   Mandata* md;
    chorda  id;
      Dies  d;
    chorda  t;
       b32  inventum;
       i32  i;
 character  multae[CD];

    x = (Mundus*)c->datum;
    thema_initiare();
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_kalendarium");
    CREDO_NECESSE_NON_NIHIL(x->volumen);
    x->documentum = agenda_aperire(c->piscina, x->intern, x->volumen,
        "agenda", ZEPHYRUM);
    agenda_monetam_ponere(x->documentum, moneta_numerator,
        &x->numerator);
    CREDO_NECESSE_NON_NIHIL(x->documentum);
    x->fontes = fontes_ficti(x, c->piscina);
    id = agenda_rem_creare(x->documentum, chorda_ex_literis("call Anna",
        c->piscina));
    d  = fasti_dies(MMXXVI, X, XIII);
    (vacuum)agenda_diem_ponere(x->documentum, id, &d);
    id = agenda_rem_creare(x->documentum,
        chorda_ex_literis("done thing",
        c->piscina));
    (vacuum)agenda_diem_ponere(x->documentum, id, &d);
    (vacuum)agenda_perficere(x->documentum, id, d);
    id = agenda_rem_creare(x->documentum, chorda_ex_literis("far item",
        c->piscina));
    d  = fasti_dies(MMXXVI, X, XX);
    (vacuum)agenda_diem_ponere(x->documentum, id, &d);
    CREDO_NECESSE(librum_scribere(x, "dies/2026-10-15",
        "line one\n\n   \nline two", c->piscina));
    multae[ZEPHYRUM] = '\0';
    per (i = I; i <= XXX; i++)
    {
        sprintf(multae + strlen(multae), "%sentry %u", i
            > I ? "\n" : "",
            (insignatus integer)i);
    }
    CREDO_NECESSE(librum_scribere(x, "dies/2026-10-16", multae,
        c->piscina));
    /* XVII: indentatio communis NOTARUM removetur, relativa manet; res
     * eiusdem diei (sine indentatione) computationem non tangit */
    id = agenda_rem_creare(x->documentum, chorda_ex_literis("sat item",
        c->piscina));
    d  = fasti_dies(MMXXVI, X, XVII);
    (vacuum)agenda_diem_ponere(x->documentum, id, &d);
    CREDO_NECESSE(librum_scribere(x, "dies/2026-10-17",
        "\n\n     abcd\n       efg\n     hig", c->piscina));
    /* latus altum: in die celebratio + lineae III capiuntur */
    x->altitudo = CDLXXX;
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    /* globulus Week */
    CREDO_NECESSE(ictus_in(x, "visus-hebdomas", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "hebdomas"));
    CREDO_VERUM(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("2026-10-11", c->piscina)) != NIHIL);
    CREDO_VERUM(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("2026-10-17", c->piscina)) != NIHIL);
    CREDO_FALSUM(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("2026-10-10", c->piscina)) != NIHIL);
    CREDO_FALSUM(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("2026-10-18", c->piscina)) != NIHIL);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_ullus_continet(md, "Week of October 11, 2026"));
    CREDO_VERUM(textus_in(md, cellula(x, "2026-10-14", c->piscina),
        "Wed 14"));
    /* celebratio: ad libitum (Callistus, XIV), dominica (XI) */
    CREDO_VERUM(textus_continet_in(md, cellula(x, "2026-10-14",
        c->piscina), "Callistus"));
    CREDO_VERUM(textus_continet_in(md, cellula(x, "2026-10-11",
        c->piscina), "Dominica"));
    CREDO_VERUM(textus_in(md, cellula(x, "2026-10-13", c->piscina),
        "[ ] call Anna"));
    CREDO_FALSUM(textus_ullus_continet(md, "done thing"));
    CREDO_FALSUM(textus_ullus_continet(md, "far item"));
    CREDO_VERUM(textus_in(md, cellula(x, "2026-10-14", c->piscina),
        "Anna Rossi turns 40"));
    CREDO_FALSUM(textus_ullus_continet(md, "Festa urbis"));
    /* placenta volans (P6d) solum in mense: motus in angulo dextro
     * supero diei natalis nihil ponit */
    {
        Fines f;

        f = cellula(x, "2026-10-14", c->piscina);
        motus_ad(x, f.x + f.latitudo - VI, f.y + IV);
        CREDO_VERUM(insula_ramus_attributum(&x->m.ramus,
            "natalis_super")
            == NIHIL);
    }
    CREDO_VERUM(textus_in(md, cellula(x, "2026-10-15", c->piscina),
        "line one"));
    CREDO_VERUM(textus_in(md, cellula(x, "2026-10-15", c->piscina),
        "line two"));
    {
        Fines f;

        f = cellula(x, "2026-10-17", c->piscina);
        CREDO_VERUM(textus_in(md, f, "[ ] sat item"));
        CREDO_VERUM(textus_in(md, f, "abcd"));
        CREDO_VERUM(textus_in(md, f, "  efg"));
        CREDO_VERUM(textus_in(md, f, "hig"));
        CREDO_FALSUM(textus_continet_in(md, f, "     abcd"));
    }
    /* liber absens (XII) lectione non creatur */
    t = volumen_plagulam_promere(x->volumen, chorda_ex_literis(
        "dies/2026-10-12/paginae", c->piscina), c->piscina, &inventum);
    (vacuum)t;
    CREDO_FALSUM(inventum);
    /* XVI: plures quam capiuntur - prima, non ultima, "+n" */
    CREDO_VERUM(textus_in(md, cellula(x, "2026-10-16", c->piscina),
        "entry 1"));
    CREDO_FALSUM(textus_ullus_continet(md, "entry 30"));
    CREDO_VERUM(textus_in(md, cellula(x, "2026-10-16", c->piscina),
        "+"));
    /* claves */
    clavis(x, 'j');
    CREDO_VERUM(electa_est(x, MMXXVI, X, XV));
    clavis(x, 'k');
    clavis(x, 'k');
    CREDO_VERUM(electa_est(x, MMXXVI, X, XIII));
    clavis(x, 'J');
    CREDO_VERUM(electa_est(x, MMXXVI, X, XX));
    dispensator_recomponere(x->d);
    CREDO_VERUM(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("2026-10-24", c->piscina)) != NIHIL);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, cellula(x, "2026-10-20", c->piscina),
        "[ ] far item"));
    clavis(x, 'K');
    CREDO_VERUM(electa_est(x, MMXXVI, X, XIII));
    clavis(x, 't');
    CREDO_VERUM(electa_est(x, MMXXVI, X, XIV));
    /* sagittae barrae: hebdomas */
    dispensator_recomponere(x->d);
    CREDO_NECESSE(ictus_in(x, "sequens", c->piscina));
    CREDO_VERUM(electa_est(x, MMXXVI, X, XXI));
    dispensator_recomponere(x->d);
    CREDO_NECESSE(ictus_in(x, "praecedens", c->piscina));
    CREDO_VERUM(electa_est(x, MMXXVI, X, XIV));
    /* ictus eligit, duplex diem aperit */
    dispensator_recomponere(x->d);
    CREDO_NECESSE(ictus_in(x, "2026-10-16", c->piscina));
    CREDO_VERUM(electa_est(x, MMXXVI, X, XVI));
    CREDO_VERUM(campus_est(x, "visus", "hebdomas"));
    dispensator_recomponere(x->d);
    CREDO_NECESSE(duplex_in(x, "2026-10-16", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    /* Enter ex hebdomade: dies; Esc ex hebdomade: mensis */
    dispensator_recomponere(x->d);
    CREDO_NECESSE(ictus_in(x, "visus-hebdomas", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "hebdomas"));
    reditus(x);
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    dispensator_recomponere(x->d);
    CREDO_NECESSE(ictus_in(x, "visus-hebdomas", c->piscina));
    clavis_specialis(x, CLAVIS_EFFUGIUM, XXVII, XXVII, ZEPHYRUM);
    CREDO_VERUM(campus_est(x, "visus", "mensis"));
}

/* hodie in mense: fundus plenus COLOR_SUPERFICIES per cellulam fere
 * totam (Franus 10-19: "light fill"); dies alius nullus; electio
 * alibi fundum non movet */
interior b32
fundus_hodie (
    Mandata* md,
      Fines  f)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus          == MANDATUM_RECTANGULUM && x->impletum
            && color_thematis_est(x->color, COLOR_SUPERFICIES)
            && intra(x->fines, f)
            && x->fines.latitudo >= f.latitudo - II
            && x->fines.altitudo >= f.altitudo - II)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_probare_fundum_hodie (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    md = reddere(x, c->piscina);
    CREDO_VERUM(fundus_hodie(md, cellula(x, "2026-10-14", c->piscina)));
    CREDO_FALSUM(fundus_hodie(md, cellula(x, "2026-10-15",
        c->piscina)));
    clavis(x, 'l');
    clavis(x, 'l');
    dispensator_recomponere(x->d);
    md = reddere(x, c->piscina);
    CREDO_VERUM(fundus_hodie(md, cellula(x, "2026-10-14", c->piscina)));
    CREDO_FALSUM(fundus_hodie(md, cellula(x, "2026-10-16",
        c->piscina)));
}

/* K3: forma exempli in libro "pensa" voluminis mundi (pagina XL x X).
 * Dies XIV (feria IV, hodie): vitamins, morning offering, mass; XV:
 * vitamins, morning offering; XVII (sabbatum): + mow the lawn */
hic_manens constans character PENSA_EXEMPLI[] =
    "vitamins\nmorning offering\n\nwednesdays\n  mass\n\nweekends\n"
    "  mow the lawn";

interior b32
pensa_scribere (
                 Mundus* x,
                Piscina* p,
     constans character* textus)
{
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    /* pagina XL x XL: formae K4 plures quam X lineas habent (pagina X
     * alta formam praecidebat) */
    l = scriba_liber_aperire_in_spatio(p, x->intern, x->volumen,
        "pensa", XL, XL);
    d = l ? scriba_liber_pagina(l, chorda_ex_literis("1", p)) : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, p, d->latitudo,
        d->altitudo, textus);
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

/* agenda diei XIV (agendam_parare) + pensa; superficies magna (index
 * totus in pagina una); visus diei */
interior b32
pensa_parare (
             Mundus* x,
     CredoContextus* c)
{
    x->latitudo = CCCLX;
    x->altitudo = CDLXXX;
    si (   !agendam_struere(x, c)
        || !pensa_scribere(x, c->piscina, PENSA_EXEMPLI)
        || !mundum_struere(x, c->piscina, NIHIL))
    {
        redde FALSUM;
    }
    reditus(x);
    dispensator_recomponere(x->d);
    redde campus_est(x, "visus", "dies");
}

/* ordines indicis (r<n>, k, k-c, k<n>, c, c-c) ordine arboris */
interior b32
indicem_esse (
                 Mundus* x,
     constans character* exspectati)
{
    Componens* radix;
    Componens* l;
    character  visi[CCLVI];
          i32  i;
          i32  k;
          b32  noster;

    radix           = dispensator_arbor(x->d);
    k               = ZEPHYRUM;
    visi[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; radix && i < componens_numerus_liberorum(radix);
        i++)
    {
        l       = componens_liberum(radix, i);
        noster  = l->id.mensura >= I && (   l->id.datum[ZEPHYRUM] == 'k'
               || chorda_aequalis_literis(l->id, "c")
               || chorda_aequalis_literis(l->id, "c-c")
               || (   l->id.datum[ZEPHYRUM] == 'r'
                   && l->id.mensura >= II
                   && l->id.datum[I] >= '0' && l->id.datum[I] <= '9'));
        si (!noster || k + l->id.mensura + II >= (i32)magnitudo(visi))
        {
            perge;
        }
        si (k > ZEPHYRUM)
        {
            visi[k++] = ' ';
        }
        memcpy(visi + k, l->id.datum, (size_t)l->id.mensura);
        k        += l->id.mensura;
        visi[k]  = '\0';
    }
    redde strcmp(visi, exspectati) == ZEPHYRUM;
}

/* campum scribere scriptore calendarii (domini alios tacite recusant),
 * ut campum_scribere in kalendarium.c */
nomen structura {
    constans character* titulus;
    constans character* valor;
} ParProbationis;

interior vacuum
campum_ponere_probatio (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    ParProbationis* par;

    par = (ParProbationis*)ctx;
    insula_attributum_ponere(radix, p, in, par->titulus, par->valor);
}

interior b32
campum_scribere_probatio (
                 Mundus* x,
     constans character* titulus,
     constans character* valor)
{
    ParProbationis par;
            chorda prior;
               b32 ok;

    par.titulus  = titulus;
    par.valor    = valor;
    prior        = x->repo->scriptor;
    insula_scriptorem_ponere(x->repo, chorda_ex_literis(
        "calendarium.agere", x->m.piscina));
    ok = mutare_ramum(&x->m.ramus, campum_ponere_probatio, &par);
    insula_scriptorem_ponere(x->repo, prior);
    redde ok;
}

/* K3: ordines pensorum inter res apertas et perfectas; electio per
 * j/k; ictus eligit; dies alius; electio invalida */
interior vacuum
_probare_pensa_ordines (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(pensa_parare(x, c));

    CREDO_GRADUS("XIV: r1, Checklist, pensa III, Completed");
    CREDO_VERUM(indicem_esse(x, "r1 k k0 k1 k2 c r2 r3"));

    CREDO_GRADUS("j/k: r1 -> k0 .. k2 -> r2, et retro");
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "k0"));
    clavis(x, 'j');
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "k2"));
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "2"));
    clavis(x, 'k');
    clavis(x, 'k');
    CREDO_VERUM(campus_est(x, "res_electa", "k1"));

    CREDO_GRADUS("ictus in ordine pensi eligit");
    CREDO_VERUM(ictus_in(x, "k2", c->piscina));
    CREDO_VERUM(campus_est(x, "res_electa", "k2"));

    CREDO_GRADUS("XVII (sabbatum): pensa III, nullae res");
    clavis(x, 'l');
    clavis(x, 'l');
    clavis(x, 'l');
    CREDO_VERUM(indicem_esse(x, "k k0 k1 k2"));

    CREDO_GRADUS("electio k5 invalida: prima, j procedit");
    CREDO_VERUM(campum_scribere_probatio(x, "res_electa", "k5"));
    dispensator_recomponere(x->d);
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "k1"));
}

/* K3: liber "pensa" absens - index ut ante (r1 c r2 r3) */
interior vacuum
_probare_pensa_absentia (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    reditus(x);
    dispensator_recomponere(x->d);
    CREDO_VERUM(indicem_esse(x, "r1 c r2 r3"));
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "2"));
}

/* K3: superficies ordinaria (capacitas IX): pensa per paginas; titulus
 * "k-c" in pagina nova; j paginam sequitur; ] pensum eligit */
interior vacuum
_probare_pensa_paginas (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    CREDO_NECESSE(pensa_scribere(x, c->piscina, PENSA_EXEMPLI));
    reditus(x);
    dispensator_recomponere(x->d);

    CREDO_GRADUS("pagina I: r1 k k0 k1");
    CREDO_VERUM(indicem_esse(x, "r1 k k0 k1"));

    CREDO_GRADUS("j ter: k2 in pagina II cum titulo iterato");
    clavis(x, 'j');
    clavis(x, 'j');
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "k2"));
    CREDO_VERUM(campus_est(x, "pagina", "1"));
    CREDO_VERUM(indicem_esse(x, "k-c k2 c r2"));

    CREDO_GRADUS("[ et ]: pagina II incipit a pensa k2");
    clavis(x, '[');
    CREDO_VERUM(campus_est(x, "res_electa", "1"));
    clavis(x, ']');
    CREDO_VERUM(campus_est(x, "res_electa", "k2"));
}

/* K3: x et ictus in capsa signant (hodie, praeterito); futurus nihil;
 * res agendae adhuc perficiuntur */
interior vacuum
_probare_pensa_signa (
    CredoContextus* c)
{
              Mundus* x;
     PensaDocumentum* b;
               Fines  f;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(pensa_parare(x, c));
    b = pensa_aperire(c->piscina, x->intern, x->volumen);
    CREDO_NECESSE_NON_NIHIL(b);

    CREDO_GRADUS("hodie: x in k0 signat, iterum tollit");
    clavis(x, 'j');
    clavis(x, 'x');
    CREDO_VERUM(pensa_factum(b, fasti_dies(MMXXVI, X, XIV),
        chorda_ex_literis("vitamins", c->piscina)));
    CREDO_VERUM(campus_est(x, "res_electa", "k0"));
    clavis(x, 'x');
    CREDO_FALSUM(pensa_factum(b, fasti_dies(MMXXVI, X, XIV),
        chorda_ex_literis("vitamins", c->piscina)));

    CREDO_GRADUS("ictus in capsa k1: eligit et signat");
    f = fines_componentis(x, "k1", c->piscina);
    ictus_ad(x, f.x + X, f.y + IV);
    CREDO_VERUM(campus_est(x, "res_electa", "k1"));
    CREDO_VERUM(pensa_factum(b, fasti_dies(MMXXVI, X, XIV),
        chorda_ex_literis("morning offering", c->piscina)));

    CREDO_GRADUS("res agendae: x adhuc perficit");
    clavis(x, 'k');
    clavis(x, 'k');
    clavis(x, 'x');
    CREDO_VERUM(agenda_res(x->documentum, id_ex(I, c->piscina))
        ->perfecta);

    /* x in k0, capsa in k1: pensa diversa, ne actus duo se mutuo
     * tollant */
    CREDO_GRADUS("XV (futurus): x et capsa nihil agunt");
    clavis(x, 'l');
    dispensator_recomponere(x->d);
    clavis(x, 'x');
    f = fines_componentis(x, "k1", c->piscina);
    ictus_ad(x, f.x + X, f.y + IV);
    CREDO_FALSUM(pensa_factum(b, fasti_dies(MMXXVI, X, XV),
        chorda_ex_literis("vitamins", c->piscina)));
    CREDO_FALSUM(pensa_factum(b, fasti_dies(MMXXVI, X, XV),
        chorda_ex_literis("morning offering", c->piscina)));

    CREDO_GRADUS("XIII (praeteritus): x signat");
    clavis(x, 'h');
    clavis(x, 'h');
    dispensator_recomponere(x->d);
    clavis(x, 'k');
    CREDO_VERUM(campus_est(x, "res_electa", "k0"));
    clavis(x, 'x');
    CREDO_VERUM(pensa_factum(b, fasti_dies(MMXXVI, X, XIII),
        chorda_ex_literis("vitamins", c->piscina)));
}

/* K3: fascia "Checklist"; capsa (signata post x), linea per titulum;
 * electus fundo selectionis; futurus obscurus */
interior vacuum
_probare_pensa_figuras (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
      Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(pensa_parare(x, c));
    clavis(x, 'j');
    clavis(x, 'x');
    clavis(x, 'j');
    md = reddere(x, c->piscina);

    CREDO_GRADUS("fascia Checklist");
    f = fines_componentis(x, "k", c->piscina);
    CREDO_VERUM(fascia_in(md, f));
    CREDO_VERUM(textus_in_continet(md, f, "Checklist"));

    CREDO_GRADUS("k0 signatum: capsa signata, linea, obscurum");
    f = fines_componentis(x, "k0", c->piscina);
    CREDO_VERUM(capsa_in(md, f, VERUM));
    CREDO_VERUM(linea_transversa_in(md, f));
    CREDO_VERUM(textus_coloris_est(md, f, "vitamins", COLOR_TEXT_DIM));

    CREDO_GRADUS("k1 electum: capsa vacua, accentus, textus plenus");
    f = fines_componentis(x, "k1", c->piscina);
    CREDO_VERUM(capsa_in(md, f, FALSUM));
    CREDO_VERUM(accentus_in(md, f));
    CREDO_VERUM(textus_coloris_est(md, f, "morning offering",
        COLOR_TEXT));

    CREDO_GRADUS("k2 non electum hodie: textus plenus, sine linea");
    f = fines_componentis(x, "k2", c->piscina);
    CREDO_VERUM(textus_coloris_est(md, f, "mass", COLOR_TEXT));
    CREDO_FALSUM(linea_transversa_in(md, f));

    CREDO_GRADUS("XV futurus: k0 (non electum) obscurum, capsa vacua");
    clavis(x, 'l');
    md  = reddere(x, c->piscina);
    f   = fines_componentis(x, "k0", c->piscina);
    CREDO_VERUM(textus_coloris_est(md, f, "vitamins", COLOR_TEXT_DIM));
    CREDO_VERUM(capsa_in(md, f, FALSUM));
}

/* K3: superficies ordinaria - "k-c" fascia Checklist in pagina II */
interior vacuum
_probare_pensa_titulum_iteratum (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
      Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(agendam_parare(x, c));
    CREDO_NECESSE(pensa_scribere(x, c->piscina, PENSA_EXEMPLI));
    reditus(x);
    clavis(x, ']');
    md  = reddere(x, c->piscina);
    f   = fines_componentis(x, "k-c", c->piscina);
    CREDO_VERUM(fascia_in(md, f));
    CREDO_VERUM(textus_in_continet(md, f, "Checklist"));
}

/* A2b: textus omnis mandatorum in piscina mandatorum (fines [basis,
 * basis + mensura) piscinae certae magnitudinis). Registrum
 * conspectuum introitus suos eicit (tectum) - textus in eis relictus
 * post eiectionem memoria aliena legebatur (mensis: festa hebdomadis
 * primae, Franus 10-23) */
interior b32
textus_in_piscina (
               Mandata* md,
    constans character* basis,
        memoriae_index  mensura)
{
    constans Mandatum* y;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus == MANDATUM_TEXTUS
            && y->textus.mensura > ZEPHYRUM
            && (   (constans character*)y->textus.datum < basis
                || (constans character*)y->textus.datum
                   + y->textus.mensura > basis + mensura))
        {
            imprimere("  textus extra piscinam: '%.*s'\n",
                (integer)y->textus.mensura,
                (constans character*)y->textus.datum);
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_probare_textum_in_piscina (
    CredoContextus* c)
{
                  Mundus* x;
     ConspectusRegistrum* r;
              DiesFontes  fontes;
                     Xar* vitia;
                 Piscina* pm;
      constans character* basis;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(agendam_parare(x, c));
    CREDO_NECESSE(librum_scribere(x, "dies/2026-10-15", "ora et labora",
        c->piscina));
    memset(&fontes, ZEPHYRUM, magnitudo(fontes));
    fontes.fontes    = fontes_ficti(x, c->piscina);   /* Anna XIV */
    fontes.agenda    = x->documentum;
    fontes.liturgia  = calendarium_creare(c->piscina);
    fontes.volumen   = x->volumen;
    fontes.intern    = x->intern;
    r = conspectus_registrum_creare(c->piscina, x->intern, x->volumen,
        ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(conspectus_dies_registrare(r, &fontes, &vitia));
    CREDO_NECESSE(conspectus_parare(r, &vitia));
    kalendarium_conspectum_ponere(&x->m, r);
    pm = piscina_generare_certae_magnitudinis("probatio_mandata",
        IV * M * M);
    CREDO_NECESSE_NON_NIHIL(pm);
    basis = (constans character*)piscina_allocare(pm, I);
    CREDO_NECESSE_NON_NIHIL(basis);

    CREDO_GRADUS("mensis: festa (conspectus) in piscina mandatorum");
    dispensator_recomponere(x->d);
    CREDO_VERUM(textus_in_piscina(reddere(x, pm), basis, IV * M * M));

    CREDO_GRADUS("hebdomas: celebratio, natales, lineae libri");
    CREDO_NECESSE(ictus_in(x, "visus-hebdomas", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "hebdomas"));
    piscina_vacare(pm);
    basis = (constans character*)piscina_allocare(pm, I);
    CREDO_VERUM(textus_in_piscina(reddere(x, pm), basis, IV * M * M));
    piscina_destruere(pm);
    conspectus_registrum_destruere(r);
}

/* K4: globulus Reminders visum monita aperit; titulus, locus, semita;
 * Esc ad mensem; argumentum "monita" (cognomen) */
interior vacuum
_probare_monita_visum (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->latitudo = CDLXXX;
    x->altitudo = CDLXXX;
    CREDO_NECESSE(agendam_parare(x, c));

    CREDO_GRADUS("globulus Reminders");
    CREDO_VERUM(ictus_in(x, "visus-monita", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "monita"));
    /* titulus fonte titulorum (globulus barrae "Reminders" fonte
     * ordinario - non sufficit) */
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_fontis_est(md, "Reminders", FONS_TITULI));

    CREDO_GRADUS("locus et titulus loci");
    CREDO_CHORDA_AEQUALIS_LITERIS(kalendarium_locus(&x->m, c->piscina),
        "calendarium/monita");
    CREDO_CHORDA_AEQUALIS_LITERIS(kalendarium_locus_titulus(&x->m,
        c->piscina), "Reminders");

    CREDO_GRADUS("Esc ad mensem");
    effugium(x);
    CREDO_VERUM(campus_est(x, "visus", "mensis"));

    CREDO_GRADUS("argumentum monita");
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "monita"));
    CREDO_VERUM(campus_est(x, "visus", "monita"));
}

/* K4: forma exempli monitorum (hodie XIV, feria IV): greges 0
 * cotidianus (implicitus), 1 wednesdays, 2 weekends, 3 vitium, 4 semel
 * (praeteritum) */
hic_manens constans character MONITA_EXEMPLI[] =
    "vitamins\nmorning offering\n\nwednesdays\n  mass\n\nweekends\n"
    "  mow the lawn\n\nevery other monday\n  call mom\n\n2026-10-01\n"
    "  pay rent";

/* ordines monitorum ordine arboris: "m", "nullum", et h/v/u numero
 * sequente (globuli "hodie", "visus-*" excluduntur); integri aut
 * incipientes (prefixum) */
interior b32
monita_comparare (
                  Mundus* x,
      constans character* exspectati,
                     b32  prefixum)
{
    Componens* radix;
    Componens* l;
    character  visi[DXII];
          i32  i;
          i32  k;
          b32  noster;

    radix           = dispensator_arbor(x->d);
    k               = ZEPHYRUM;
    visi[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; radix && i < componens_numerus_liberorum(radix);
        i++)
    {
        l       = componens_liberum(radix, i);
        noster  = chorda_aequalis_literis(l->id, "m")
               || chorda_aequalis_literis(l->id, "nullum")
               || (   l->id.mensura >= II
                   && (   l->id.datum[ZEPHYRUM] == 'h'
                       || l->id.datum[ZEPHYRUM] == 'v'
                       || l->id.datum[ZEPHYRUM] == 'u')
                   && l->id.datum[I] >= '0' && l->id.datum[I] <= '9');
        si (!noster || k + l->id.mensura + II >= (i32)magnitudo(visi))
        {
            perge;
        }
        si (k > ZEPHYRUM)
        {
            visi[k++] = ' ';
        }
        memcpy(visi + k, l->id.datum, (size_t)l->id.mensura);
        k        += l->id.mensura;
        visi[k]  = '\0';
    }
    si (prefixum ? strncmp(visi, exspectati, strlen(exspectati))
                 != ZEPHYRUM : strcmp(visi, exspectati) != ZEPHYRUM)
    {
        imprimere("  ordines visi: '%s'\n", visi);
        redde FALSUM;
    }
    redde VERUM;
}

/* agenda + liber "pensa" cum textu; visus monita */
interior b32
monita_parare (
                 Mundus* x,
         CredoContextus* c,
     constans character* textus)
{
    /* liber ante montationem (editor formae librum creat) */
    si (   !agendam_struere(x, c)
        || (textus && !pensa_scribere(x, c->piscina, textus))
        || !mundum_struere(x, c->piscina, NIHIL))
    {
        redde FALSUM;
    }
    si (!kalendarium_argumentum_ponere(&x->m, "monita"))
    {
        redde FALSUM;
    }
    dispensator_recomponere(x->d);
    redde campus_est(x, "visus", "monita");
}

/* K4: ordines formae: fascia, greges (titulus, vitium, res) */
interior vacuum
_probare_monita_ordines (
    CredoContextus* c)
{
    Mundus* x;

    x            = (Mundus*)c->datum;
    x->latitudo  = CDLXXX;
    x->altitudo  = CDLXXX;
    CREDO_NECESSE(monita_parare(x, c, MONITA_EXEMPLI));
    CREDO_VERUM(monita_comparare(x,
        "m h0 u0-0 u0-1 h1 u1-0 h2 u2-0 h3 v3 u3-0 h4 u4-0", FALSUM));
}

/* K4: liber absens et volumen absens: "m nullum" */
interior vacuum
_probare_monita_absentiam (
    CredoContextus* c)
{
    Mundus* x;

    x            = (Mundus*)c->datum;
    x->latitudo  = CDLXXX;
    x->altitudo  = CDLXXX;

    CREDO_GRADUS("liber pensa absens");
    CREDO_NECESSE(monita_parare(x, c, NIHIL));
    CREDO_VERUM(monita_comparare(x, "m nullum", FALSUM));

    CREDO_GRADUS("sine volumine");
    volumen_claudere(x->volumen);
    x->volumen     = NIHIL;
    x->documentum  = NIHIL;
    CREDO_NECESSE(mundum_struere(x, c->piscina, "monita"));
    CREDO_VERUM(campus_est(x, "visus", "monita"));
    CREDO_VERUM(monita_comparare(x, "m nullum", FALSUM));
}

/* K4: superficies ordinaria: grex cotidianus XV rerum per paginas;
 * titulus "h0-c" in pagina II; ] et [ */
interior vacuum
_probare_monita_paginas (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(monita_parare(x, c, "i0\ni1\ni2\ni3\ni4\ni5\ni6\ni7\n"
        "i8\ni9\ni10\ni11\ni12\ni13\ni14"));

    CREDO_GRADUS("pagina I");
    CREDO_VERUM(monita_comparare(x, "m h0 u0-0 u0-1", VERUM));

    CREDO_GRADUS("] pagina II: titulus iteratus");
    clavis(x, ']');
    CREDO_VERUM(campus_est(x, "pagina", "1"));
    CREDO_VERUM(monita_comparare(x, "h0-c u0-", VERUM));

    CREDO_GRADUS("[ ad paginam I");
    clavis(x, '[');
    CREDO_VERUM(monita_comparare(x, "m h0 u0-0", VERUM));
}

/* textus coloris signi: numerus mandatorum et omnia intra f? */
interior i32
textus_coloris_numerare (
          Mandata* md,
            Fines  f,
              i32  signum,
              b32* omnia_intra)
{
    constans Mandatum* y;
                  i32  i;
                  i32  n;

    n             = ZEPHYRUM;
    *omnia_intra  = VERUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus       != MANDATUM_TEXTUS
            || y->color.genus != COLOR_MANDATI_THEMA
            || y->color.valor != signum)
        {
            perge;
        }
        n++;
        si (!punctum_intra(y->fines, f))
        {
            *omnia_intra = FALSUM;
        }
    }
    redde n;
}

/* K4: fascia; tituli (scripti, "Every day" obscurum) cum notis today /
 * next / no more dates; vitium sine nota; res */
interior vacuum
_probare_monita_figuras (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
      Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->latitudo = CDLXXX;
    x->altitudo = CDLXXX;
    CREDO_NECESSE(monita_parare(x, c, MONITA_EXEMPLI));
    md = reddere(x, c->piscina);

    CREDO_GRADUS("fascia Daily checklist");
    f = fines_componentis(x, "m", c->piscina);
    CREDO_VERUM(fascia_in(md, f));
    CREDO_VERUM(textus_in_continet(md, f, "Daily checklist"));

    CREDO_GRADUS("Every day obscurum, today");
    f = fines_componentis(x, "h0", c->piscina);
    CREDO_VERUM(textus_coloris_est(md, f, "Every day", COLOR_TEXT_DIM));
    CREDO_VERUM(textus_in_continet(md, f, "today"));

    CREDO_GRADUS("wednesdays scriptum, today");
    f = fines_componentis(x, "h1", c->piscina);
    CREDO_VERUM(textus_coloris_est(md, f, "wednesdays", COLOR_TEXT));
    CREDO_VERUM(textus_in_continet(md, f, "today"));

    CREDO_GRADUS("weekends: next Sat 17 Oct");
    f = fines_componentis(x, "h2", c->piscina);
    CREDO_VERUM(textus_in_continet(md, f, "weekends"));
    CREDO_VERUM(textus_in_continet(md, f, "next: Sat 17 Oct"));

    CREDO_GRADUS("vitium: sine nota, nuntius colore erroris");
    f = fines_componentis(x, "h3", c->piscina);
    CREDO_VERUM(textus_in_continet(md, f, "every other monday"));
    CREDO_FALSUM(textus_in_continet(md, f, "next"));
    CREDO_FALSUM(textus_in_continet(md, f, "today"));
    f = fines_componentis(x, "v3", c->piscina);
    CREDO_VERUM(textus_coloris_est(md, f, "from which day",
        COLOR_ERROR));

    CREDO_GRADUS("semel praeteritum: no more dates");
    f = fines_componentis(x, "h4", c->piscina);
    CREDO_VERUM(textus_in_continet(md, f, "no more dates"));

    CREDO_GRADUS("res");
    f = fines_componentis(x, "u2-0", c->piscina);
    CREDO_VERUM(textus_coloris_est(md, f, "mow the lawn", COLOR_TEXT));
}

/* K4: superficies ordinaria - vitium involutum (II lineae aut plures),
 * omnes intra ordinem "v0" (altitudo ex eadem involutione) */
interior vacuum
_probare_monita_vitium (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
      Fines  f;
        b32  intra;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(monita_parare(x, c,
        "every other monday\n  call mom"));
    CREDO_VERUM(monita_comparare(x, "m h0 v0 u0-0", FALSUM));
    md  = reddere(x, c->piscina);
    f   = fines_componentis(x, "v0", c->piscina);
    CREDO_VERUM(textus_coloris_numerare(md, f, COLOR_ERROR, &intra)
        >= II);
    CREDO_VERUM(intra);
}

/* K4: liber absens - "No checklist yet." */
interior vacuum
_probare_monita_vacua (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(monita_parare(x, c, NIHIL));
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "nullum",
        c->piscina), "No checklist yet. Press e to write one."));
}

/* mandata coetus componentis id: (a, b) - mandata intra coetum (sine
 * coetu ipso); FALSUM si nullus */
interior b32
coetus_componentis (
                Mandata* md,
     constans character* id,
                    i32* a,
                    i32* b)
{
    constans Mandatum* y;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   y->genus == MANDATUM_COETUS
            && chorda_aequalis_literis(y->provenientia, id))
        {
            *a = i + I;
            *b = i + y->magnitudo_arboris;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* globulus: textus non vacuus et campana (ordo latissimus IX x I,
 * impletus) in coetu eius */
interior vacuum
globulum_inspicere (
                Mandata* md,
     constans character* id,
                    b32* textus,
                    b32* campana)
{
    constans Mandatum* y;
                  i32  a;
                  i32  b;
                  i32  i;

    *textus   = FALSUM;
    *campana  = FALSUM;
    si (!coetus_componentis(md, id, &a, &b))
    {
        redde;
    }
    per (i = a; i < b; i++)
    {
        y = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (y->genus == MANDATUM_TEXTUS && y->textus.mensura > ZEPHYRUM)
        {
            *textus = VERUM;
        }
        si (   y->genus          == MANDATUM_RECTANGULUM && y->impletum
            && y->fines.latitudo == IX && y->fines.altitudo == I)
        {
            *campana = VERUM;
        }
    }
}

/* K4: globulus Reminders: verbum si barra capit (480), aliter campana
 * III cellularum (280, visus mensis = barra latissima), intra
 * superficiem */
interior vacuum
_probare_monita_globulum (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
      Fines  f;
        b32  textus;
        b32  campana;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->latitudo = CDLXXX;
    x->altitudo = CDLXXX;
    CREDO_NECESSE(agendam_parare(x, c));

    CREDO_GRADUS("CDLXXX: verbum");
    md  = reddere(x, c->piscina);
    f   = fines_componentis(x, "visus-monita", c->piscina);
    CREDO_AEQUALIS_S32(f.latitudo, (s32)(strlen("Reminders") + II)
        * VI);
    globulum_inspicere(md, "visus-monita", &textus, &campana);
    CREDO_VERUM(textus);
    CREDO_FALSUM(campana);

    CREDO_GRADUS("CCLXXX: campana, intra superficiem");
    x->latitudo = ZEPHYRUM;
    x->altitudo = ZEPHYRUM;
    CREDO_NECESSE(mundum_struere(x, c->piscina, NIHIL));
    md  = reddere(x, c->piscina);
    f   = fines_componentis(x, "visus-monita", c->piscina);
    CREDO_AEQUALIS_S32(f.latitudo, III * VI);
    CREDO_VERUM(f.x + f.latitudo <= CCLXXX);
    globulum_inspicere(md, "visus-monita", &textus, &campana);
    CREDO_FALSUM(textus);
    CREDO_VERUM(campana);
}

/* K5: e dividit visum monitorum - lectio supra, scriba libri "pensa"
 * infra (locus "forma"); ictus in forma scribam focat; Esc: scriba ->
 * index -> lectio -> mensis; globulus Edit; visus mutatus lectionem
 * restituit */
interior vacuum
_probare_monita_editionem (
    CredoContextus* c)
{
     Mundus* x;
      Motus* motus;
      Fines  f;
      Fines  r;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->latitudo = CDLXXX;
    x->altitudo = CDLXXX;
    CREDO_NECESSE(monita_parare(x, c, MONITA_EXEMPLI));
    motus = dispensator_motus(x->d);

    CREDO_GRADUS("lectio: forma absens, globulus Edit adest");
    CREDO_FALSUM(campus_est(x, "monita_modus", "editio"));
    CREDO_NIHIL(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("forma", c->piscina)));
    CREDO_NON_NIHIL(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("edere", c->piscina)));

    CREDO_GRADUS("e: editio, forma infra ordines, Edit absens");
    clavis(x, 'e');
    CREDO_VERUM(campus_est(x, "monita_modus", "editio"));
    f = fines_componentis(x, "forma", c->piscina);
    r = fines_componentis(x, "m", c->piscina);
    CREDO_NECESSE(f.altitudo > ZEPHYRUM);
    CREDO_VERUM(f.y > r.y);
    CREDO_VERUM(monita_comparare(x, "m h0", VERUM));
    CREDO_NIHIL(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("edere", c->piscina)));

    CREDO_GRADUS("ictus in forma: scriba libri pensa focata");
    ictus_ad(x, f.x + XL, f.y + XXX);
    CREDO_VERUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_pensa", c->piscina)));

    CREDO_GRADUS("Esc: ad indicem (editio manet), lectio, mensis");
    effugium(x);
    CREDO_FALSUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_pensa", c->piscina)));
    CREDO_VERUM(campus_est(x, "monita_modus", "editio"));
    effugium(x);
    CREDO_FALSUM(campus_est(x, "monita_modus", "editio"));
    CREDO_VERUM(campus_est(x, "visus", "monita"));
    CREDO_NIHIL(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("forma", c->piscina)));
    effugium(x);
    CREDO_VERUM(campus_est(x, "visus", "mensis"));

    CREDO_GRADUS("globulus Edit; visus mutatus lectionem restituit");
    CREDO_VERUM(ictus_in(x, "visus-monita", c->piscina));
    CREDO_VERUM(ictus_in(x, "edere", c->piscina));
    CREDO_VERUM(campus_est(x, "monita_modus", "editio"));
    CREDO_VERUM(ictus_in(x, "visus-mensis", c->piscina));
    CREDO_VERUM(ictus_in(x, "visus-monita", c->piscina));
    CREDO_FALSUM(campus_est(x, "monita_modus", "editio"));
}

/* K5: sine volumine - e editionem ponit, forma adest, scriba nulla */
interior vacuum
_probare_monita_editionem_sine_volumine (
    CredoContextus* c)
{
     Mundus* x;
      Motus* motus;
      Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->latitudo = CDLXXX;
    x->altitudo = CDLXXX;
    CREDO_NECESSE(mundum_struere(x, c->piscina, "monita"));
    motus = dispensator_motus(x->d);
    clavis(x, 'e');
    CREDO_VERUM(campus_est(x, "monita_modus", "editio"));
    f = fines_componentis(x, "forma", c->piscina);
    CREDO_NECESSE(f.altitudo > ZEPHYRUM);
    ictus_ad(x, f.x + XL, f.y + XXX);
    CREDO_FALSUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_pensa", c->piscina)));
    (vacuum)reddere(x, c->piscina);
}

/* K5: scribere in editore formae - gestus editori visibili (forma in
 * editione, aliter notae); grex novus cum vitio statim in lectione;
 * Esc, Tab; notae diei postea adhuc scribuntur */
interior vacuum
_probare_monita_scribere (
    CredoContextus* c)
{
     Mundus* x;
      Motus* motus;
      Fines  f;
     chorda  t;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->latitudo = CDLXXX;
    x->altitudo = CDLXXX;
    CREDO_NECESSE(monita_parare(x, c, "vitamins"));
    motus = dispensator_motus(x->d);
    /* hospes gestum ponit (ut vicus in foco lateris) */
    kalendarium_gestum_ponere(motus, &x->m);

    CREDO_GRADUS("e, ictus in forma, grex novus scriptus");
    clavis(x, 'e');
    f = fines_componentis(x, "forma", c->piscina);
    CREDO_NECESSE(f.altitudo > ZEPHYRUM);
    ictus_ad(x, f.x + XL, f.y + XXX);
    CREDO_NECESSE(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_pensa", c->piscina)));
    /* pagina scribae = craticula fixa (G = linea ultima craticulae):
     * gg, j j -> linea III (II vacua = sectio nova), i, Esc, j, i */
    textum_mittere(x, "g");
    textum_mittere(x, "g");
    textum_mittere(x, "j");
    textum_mittere(x, "j");
    textum_mittere(x, "i");
    textum_mittere(x, "every fortnight");
    effugium(x);
    textum_mittere(x, "j");
    textum_mittere(x, "0");
    textum_mittere(x, "i");
    textum_mittere(x, "  water plants");
    effugium(x);
    (vacuum)motus_gestum_effundere(motus, x->repo);
    t = pensa_textum_legere(x->volumen, x->intern, c->piscina);
    CREDO_VERUM(chorda_continet(t, chorda_ex_literis("every fortnight",
        c->piscina)));

    CREDO_GRADUS("lectio statim: titulus et vitium gregis novi");
    dispensator_recomponere(x->d);
    CREDO_VERUM(monita_comparare(x, "m h0 u0-0 h1 v1 u1-0", FALSUM));

    CREDO_GRADUS("Esc ex editore; Tab intro et foras");
    effugium(x);
    CREDO_FALSUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_pensa", c->piscina)));
    CREDO_VERUM(campus_est(x, "monita_modus", "editio"));
    clavis_specialis(x, CLAVIS_TABULA, '\t', '\t', ZEPHYRUM);
    CREDO_VERUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_pensa", c->piscina)));
    clavis_specialis(x, CLAVIS_TABULA, '\t', '\t', ZEPHYRUM);
    CREDO_FALSUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_pensa", c->piscina)));

    CREDO_GRADUS("notae diei postea: gestus redit, x in libro diei");
    effugium(x);
    CREDO_VERUM(ictus_in(x, "visus-dies", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "dies"));
    f = fines_componentis(x, "notae", c->piscina);
    CREDO_NECESSE(f.altitudo > ZEPHYRUM);
    ictus_ad(x, f.x + XL, f.y + XXX);
    textum_mittere(x, "i");
    textum_mittere(x, "x");
    effugium(x);
    (vacuum)motus_gestum_effundere(motus, x->repo);
    CREDO_VERUM(littera_in_libro(x, "dies/2026-10-14", 'x',
        c->piscina));
}

/* K5: scriptura pendens (modus inserendi, sine Esc) dum globulus
 * visum mutat: gestus ad notas transit, scripta formae prius
 * effunduntur - non pereunt */
interior vacuum
_probare_monita_gestum_pendentem (
    CredoContextus* c)
{
     Mundus* x;
      Motus* motus;
      Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->latitudo = CDLXXX;
    x->altitudo = CDLXXX;
    CREDO_NECESSE(monita_parare(x, c, "vitamins"));
    motus = dispensator_motus(x->d);
    kalendarium_gestum_ponere(motus, &x->m);
    clavis(x, 'e');
    f = fines_componentis(x, "forma", c->piscina);
    CREDO_NECESSE(f.altitudo > ZEPHYRUM);
    ictus_ad(x, f.x + XL, f.y + XXX);
    textum_mittere(x, "g");
    textum_mittere(x, "g");
    textum_mittere(x, "j");
    textum_mittere(x, "j");
    textum_mittere(x, "i");
    textum_mittere(x, "rosary");
    /* nullum Esc: globulus Month */
    CREDO_VERUM(ictus_in(x, "visus-mensis", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "mensis"));
    (vacuum)motus_gestum_effundere(motus, x->repo);
    CREDO_VERUM(chorda_continet(pensa_textum_legere(x->volumen,
        x->intern,
        c->piscina), chorda_ex_literis("rosary", c->piscina)));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "natalis_super", _probare_natalis_super, mundum_parare,
      mundum_purgare, NIHIL, NIHIL },
    { "natales_mensis", _probare_natales_mensis, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "natales_diei", _probare_natales_diei, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "signatum_aperire", _probare_signatum_aperire, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "argumentum_diei", _probare_argumentum_diei, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "semitae", _probare_semitas, mundum_parare, mundum_purgare,
      NIHIL, NIHIL },
    { "semita_secuta", _probare_semitam_secutam, mundum_parare,
      mundum_purgare, NIHIL, NIHIL },
    { "argumentum_diei_malum", _probare_argumentum_diei_malum,
        mundum_parare, mundum_purgare, NIHIL, NIHIL },
    { "argumentum_diei_ex_mense", _probare_argumentum_diei_ex_mense,
        mundum_parare, mundum_purgare, NIHIL, NIHIL },
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
    { "rubricae_in_linea", _probare_rubricas_in_linea, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "greges_filtrum", _probare_greges_filtrum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "elenchus_diei", _probare_elenchum_diei, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "elenchus_rei", _probare_elenchum_rei, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "paginae_greges", _probare_paginas_gregum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "paginae_greges_diei", _probare_paginas_gregum_diei,
        mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "titulus_solus", _probare_titulum_solum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "conspectus_communis", _probare_conspectum_communem,
        mundum_parare, mundum_purgare, NIHIL, NIHIL },
    { "hebdomas", _probare_hebdomadem, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "fundus_hodie", _probare_fundum_hodie, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "pensa_ordines", _probare_pensa_ordines, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "pensa_absentia", _probare_pensa_absentia, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "pensa_paginae", _probare_pensa_paginas, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "pensa_signa", _probare_pensa_signa, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "pensa_figurae", _probare_pensa_figuras, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "pensa_titulus_iteratus", _probare_pensa_titulum_iteratum,
        mundum_parare, mundum_purgare, NIHIL, NIHIL },
    { "textus_in_piscina", _probare_textum_in_piscina, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "monita_visus", _probare_monita_visum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "monita_ordines", _probare_monita_ordines, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "monita_absentia", _probare_monita_absentiam, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "monita_paginae", _probare_monita_paginas, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "monita_figurae", _probare_monita_figuras, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "monita_vitium", _probare_monita_vitium, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "monita_vacua", _probare_monita_vacua, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "monita_globulus", _probare_monita_globulum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "monita_editio", _probare_monita_editionem, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "monita_scribere", _probare_monita_scribere, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "monita_gestus_pendens", _probare_monita_gestum_pendentem,
        mundum_parare, mundum_purgare, NIHIL, NIHIL },
    { "monita_editio_sine_volumine",
        _probare_monita_editionem_sine_volumine, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("kalendarium", SECTIONES);
}
