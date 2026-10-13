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

/* septem textus unius litterae in linea II tituli (y VIII; numeri
 * dierum infra tabulam tituli XVI) ordine S M T W T F S */
interior b32
litterae_dierum_sunt (
    Mandata* md)
{
    constans Mandatum* x;
            character  visae[VIII];
                  i32  i;
                  i32  n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa) && n < VII; i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus   == MANDATUM_TEXTUS && x->textus.mensura == I
            && x->fines.y == VIII)
        {
            visae[n++] = (character)x->textus.datum[ZEPHYRUM];
        }
    }
    visae[n] = '\0';
    redde strcmp(visae, "SMTWTFS") == ZEPHYRUM;
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
    /* litterae dierum: textus in memoria mandatorum, non in acervo
     * figurae (mandata textum per indicem servant) - post vocationes
     * alias acervum rescribentes */
    CREDO_VERUM(litterae_dierum_sunt(md));
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
    /* res V abiit: electio in ordine eodem (ordines 1 2 5 3 -> 1 2 3:
     * res III, hic perfecta, alibi assignata); dies mala recusatur
     * (prompta manet), Esc claudit sine mutatione */
    CREDO_VERUM(campus_est(x, "res_electa", "3"));
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
    f = fines_componentis(x, "r1", c->piscina);
    CREDO_VERUM(textus_in_continet(md, f, "[ ]"));
    CREDO_VERUM(textus_in_continet(md, f, "panem emere"));
    CREDO_VERUM(textus_in_continet(md, f, "Oct 13"));
    /* ordo electus: nota colore textus pleno (non obscuro - in fundo
     * selectionis legi non poterat, imago A3c) */
    CREDO_VERUM(textus_coloris_est(md, f, "panem emere", COLOR_TEXT));
    CREDO_VERUM(textus_in_continet(md, fines_componentis(x, "r2",
        c->piscina), "[x]"));
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
    CREDO_VERUM(textus_ullus_continet(md, "note: abc"));
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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("kalendarium", SECTIONES);
}
