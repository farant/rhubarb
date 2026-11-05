/* probatio_gabc_compositio.c - compositio cantus gabc in paginas
 * pictorum (gabc pars II, P3)
 *
 * Valores manu derivati ex regulis include/gabc_compositio.h ad formam
 * ordinariam (spatium VIII: caput VIII x VII origo (0,III), gradus IV,
 * k I; FONS_PARVUS VI x VIII; margo VIII). Neumae ut excerpta RELATIVA
 * ad originem partis primae syllabae: "pars:parametrum dx dy" et
 * "rect dx dy LxA", ';' separata, ordine emissionis (caput, caulis,
 * caput sequens, linea connectens).
 *
 * FORMA CANONICA (gabc_compositionem_scribere), linea una per rem,
 * columnae tabulis separatae:
 *   pictum  pagina genus x y detail tinctura syllaba elementum nota
 *           genus: pars | rect | textus | initialis; detail: titulus
 *           partis ':' parametrum | LxA | "textus" '/' fons |
 *           "littera" 'x' scala
 *   syllaba index pagina x y LxA     (locus; pagina -1 sine loco)
 * Salve fixa in probationes/fixa/gabc/compositio/salve.compositio;
 * GABC_COMPOSITIO_SCRIBERE=1 eam rescribit (causa nominanda). */
#include "latina.h"
#include "credo.h"
#include "gabc.h"
#include "gabc_compositio.h"
#include "neumae_ductae.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "fons.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_SALVE       "musica/fontes/salve_2435.gabc"
#define VIA_FIXI_SALVE \
    "probationes/fixa/gabc/compositio/salve.compositio"
#define VIA_MANIFESTI   "probationes/fixa/gabc/gregobase.manifestum"
#define VIA_FIXORUM     "probationes/fixa/gabc/gregobase/"
#define SINE_INITIALI   "initial-style:0;\n%%\n"


/* ==================================================
 * Instrumenta
 * ================================================== */

interior GabcCompositio*
_componere (
            CredoContextus* c,
        constans character* literae,
    GabcCompositionisForma* forma)
{
    GabcCompositionisForma  ordinaria;
                GabcCantus* cantus;

    cantus = gabc_legere(chorda_ex_literis(literae, c->piscina),
        c->piscina);
    si (forma == NIHIL)
    {
        gabc_compositionis_formam_ordinariam(&ordinaria);
        forma = &ordinaria;
    }
    redde gabc_componere(cantus, forma, c->piscina);
}

/* runae chordae (octeti non continuationis) */
interior s32
_runae (
    chorda ch)
{
    i32 i;
    s32 n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < ch.mensura; i++)
    {
        si ((ch.datum[i] & 0xC0) != 0x80)
        {
            n++;
        }
    }
    redde n;
}

/* capsa picti (pars per mensuram); FALSUM si mensura deest */
interior b32
_capsa (
    constans GabcPictum* p,
                    s32* x0,
                    s32* y0,
                    s32* x1,
                    s32* y1)
{
    NeumaeMensura m;

    commutatio (p->genus)
    {
        casus GABC_PICTUM_PARS:
            si (!neumae_ductae_metiri(p->pars, VIII, p->parametrum, &m))
            {
                redde FALSUM;
            }
            *x0 = p->x - m.origo_x;
            *y0 = p->y - m.origo_y;
            *x1 = *x0 + (s32)m.latitudo;
            *y1 = *y0 + (s32)m.altitudo;
            redde VERUM;
        casus GABC_PICTUM_TEXTUS:
            *x0 = p->x;
            *y0 = p->y;
            *x1 = p->x + _runae(p->textus) * fons_latitudo(p->fons);
            *y1 = p->y + fons_altitudo(p->fons);
            redde VERUM;
        ordinarius:
            *x0 = p->x;
            *y0 = p->y;
            *x1 = p->x + (s32)p->latitudo;
            *y1 = p->y + (s32)p->altitudo;
            redde VERUM;
    }
}

/* excerptum relativum partium et rectangulorum syllabae */
interior vacuum
_neumae (
    constans GabcCompositio* comp,
                        s32  syllaba,
                  character* exitus,
                        i32  capacitas)
{
                    s32  x0;
                    s32  y0;
                    b32  inventa;
                    i32  i;
                    i32  n;
    constans GabcPictum* p;

    exitus[ZEPHYRUM]  = '\0';
    inventa           = FALSUM;
    x0                = ZEPHYRUM;
    y0                = ZEPHYRUM;
    n                 = ZEPHYRUM;
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        p = &comp->picta[i];
        si (   p->syllaba != syllaba
            || (p->genus != GABC_PICTUM_PARS
                && p->genus != GABC_PICTUM_RECTANGULUM))
        {
            perge;
        }
        si (!inventa && p->genus == GABC_PICTUM_PARS)
        {
            inventa  = VERUM;
            x0       = p->x;
            y0       = p->y;
        }
        si (n > ZEPHYRUM && n + II < (i32)capacitas)
        {
            exitus[n++] = ';';
        }
        si (p->genus == GABC_PICTUM_PARS)
        {
            n += (i32)sprintf(exitus + n, "%s:%d %d %d",
                neumae_partis_titulus(p->pars), (integer)p->parametrum,
                (integer)(p->x - x0), (integer)(p->y - y0));
        }
        alioquin
        {
            n += (i32)sprintf(exitus + n, "rect %d %d %ux%u",
                (integer)(p->x - x0), (integer)(p->y - y0),
                (unsigned)p->latitudo, (unsigned)p->altitudo);
        }
        si (n + LXIV > (i32)capacitas)
        {
            frange;
        }
    }
}

#define NEUMAE(literae, syllaba, exspectatum) \
    _neumas_credere(c, (literae), (syllaba), (exspectatum))

interior b32
_neumas_credere (
          CredoContextus* c,
      constans character* literae,
                     s32  syllaba,
      constans character* exspectatum)
{
         character  receptum[(IV * M)];
    GabcCompositio* comp;

    comp = _componere(c, literae, NIHIL);
    si (comp == NIHIL)
    {
        imprimere("  %s: NIHIL\n", literae);
        CREDO_VERUM(FALSUM);
        redde FALSUM;
    }
    _neumae(comp, syllaba, receptum, (IV * M));
    si (strcmp(receptum, exspectatum) != ZEPHYRUM)
    {
        imprimere("  %s\n    exspectatum: %s\n    receptum:    %s\n",
            literae, exspectatum, receptum);
    }
    CREDO_LITERAE_AEQUALES(receptum, exspectatum);
    redde strcmp(receptum, exspectatum) == ZEPHYRUM;
}

/* pictum primum generis et syllabae (pars: etiam pars data; -1
 * omnis) */
interior constans GabcPictum*
_pictum (
     constans GabcCompositio* comp,
              GabcPictiGenus  genus,
                         s32  syllaba,
                         s32  pars)
{
    i32 i;

    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        si (   comp->picta[i].genus == genus
            && (syllaba < -I || comp->picta[i].syllaba == syllaba)
            && (pars < ZEPHYRUM || (s32)comp->picta[i].pars == pars))
        {
            redde &comp->picta[i];
        }
    }
    redde NIHIL;
}

/* textus syllabae (TEXTUS primum) */
interior constans GabcPictum*
_textus (
    constans GabcCompositio* comp,
                        s32  syllaba)
{
    redde _pictum(comp, GABC_PICTUM_TEXTUS, syllaba, -I);
}

/* lineae systematis: rectangula SYSTEMA sine syllaba, IV per
 * systema, ordine positionum III V VII IX */
interior i32
_systemata (
    constans GabcCompositio* comp)
{
    i32 i;
    i32 n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        si (   comp->picta[i].genus    == GABC_PICTUM_RECTANGULUM
            && comp->picta[i].tinctura == GABC_TINCTURA_SYSTEMA
            && comp->picta[i].syllaba  == -I)
        {
            n++;
        }
    }
    redde n / IV;
}

/* y lineae imae systematis k (ordine) */
interior s32
_linea_infima (
    constans GabcCompositio* comp,
                        i32  k)
{
    i32 i;
    i32 n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        si (   comp->picta[i].genus    == GABC_PICTUM_RECTANGULUM
            && comp->picta[i].tinctura == GABC_TINCTURA_SYSTEMA
            && comp->picta[i].syllaba  == -I)
        {
            si (n == k * IV)
            {
                redde comp->picta[i].y;
            }
            n++;
        }
    }
    redde -I;
}

/* pagina picti (ex paginae[]) */
interior s32
_pagina_picti (
    constans GabcCompositio* comp,
                        i32  index)
{
    i32 p;

    per (p = ZEPHYRUM; p < comp->numerus_paginarum; p++)
    {
        si (index >= comp->paginae[p] && index < comp->paginae[p + I])
        {
            redde (s32)p;
        }
    }
    redde -I;
}

/* numerus partium generis in syllaba (-II = omnes) */
interior i32
_numerus_partium (
     constans GabcCompositio* comp,
                  NeumaePars  pars,
                         s32  syllaba)
{
    i32 i;
    i32 n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        si (   comp->picta[i].genus == GABC_PICTUM_PARS
            && comp->picta[i].pars  == pars
            && (syllaba == -II || comp->picta[i].syllaba == syllaba))
        {
            n++;
        }
    }
    redde n;
}


/* ==================================================
 * I: neumae (tabula COMPOSITIO NEUMARUM)
 * ================================================== */

interior vacuum
_neumae_probare (
    CredoContextus* c)
{
    NEUMAE("(c4) a(g)", I, "punctum:0 0 0");
    NEUMAE("(c4) a(gv)", I, "virga:2 0 0");
    NEUMAE("(c4) a(hv)", I, "virga:3 0 0");
    /* pes: superius SUPRA, linea in margine dextro */
    NEUMAE("(c4) a(gh)", I,
        "punctum:0 0 0;punctum:0 0 -4;rect 7 -4 1x5");
    /* clivis: caulis sinister ad medium spatii (g in spatio: II
     * gradus infra), inferius IUXTA */
    NEUMAE("(c4) a(hg)", I,
        "punctum:0 0 0;rect 0 0 1x11;punctum:0 8 4;rect 7 0 1x5");
    NEUMAE("(c4) a(ghg)", I,
        "punctum:0 0 0;punctum:0 8 -4;rect 7 -4 1x5;punctum:0 16 0;"
        "rect 15 -4 1x5");
    /* porrectus: tertia SUPRA finem lineae obliquae */
    NEUMAE("(c4) a(hgh)", I,
        "porrectus:1 0 0;rect 0 0 1x11;punctum:0 8 0;rect 15 0 1x5");
    NEUMAE("(c4) a(hvGF)", I,
        "virga:3 0 0;inclinatum:0 8 4;inclinatum:0 14 8");
    /* scandicus: caput, deinde pes IUXTA */
    NEUMAE("(c4) a(ghi)", I,
        "punctum:0 0 0;punctum:0 8 -4;punctum:0 8 -8;rect 15 -8 1x5");
    /* salicus: caput, deinde pes quassus (oriscus, superius IUXTA) */
    NEUMAE("(c4) a(gioj)", I,
        "punctum:0 0 0;oriscus:0 8 -8;punctum:0 16 -12;"
        "rect 15 -12 1x5");
    NEUMAE("(c4) a(gwh)", I,
        "quilisma:0 0 0;punctum:0 0 -4;rect 7 -4 1x5");
    /* pressus: aequales IUXTA sine linea */
    NEUMAE("(c4) a(hhg)", I,
        "punctum:0 0 0;punctum:0 8 0;punctum:0 16 4;rect 15 0 1x5");
    /* epiphonus: deminutum SUPRA ad marginem dextrum */
    NEUMAE("(c4) a(gh~)", I,
        "punctum:0 0 0;deminutum:0 3 -4;rect 7 -4 1x5");
    NEUMAE("(c4) a(hg~)", I,
        "punctum:0 0 0;rect 0 0 1x11;deminutum:0 8 4;rect 7 0 1x5");
    /* figura ignota: segmenta longissima nota (torculus + clivis),
     * s/II inter aequales */
    NEUMAE("(c4) a(ghffd)", I,
        "punctum:0 0 0;punctum:0 8 -4;rect 7 -4 1x5;punctum:0 16 4;"
        "rect 15 -4 1x9;punctum:0 28 4;rect 28 4 1x15;punctum:0 36 12;"
        "rect 35 4 1x9");
    NEUMAE("(c4) a(jjj)", I,
        "punctum:0 0 0;punctum:0 12 0;punctum:0 24 0");
}


/* ==================================================
 * II: signa
 * ================================================== */

/* rectangulum syllabae relativum ad lineam summam systematis primi */
interior b32
_divisio (
          CredoContextus* c,
      constans character* literae,
                     s32  dy,
                     i32  altitudo)
{
         GabcCompositio* comp;
    constans GabcPictum* r;
                    s32  summa;

    comp = _componere(c, literae, NIHIL);
    si (comp == NIHIL)
    {
        redde FALSUM;
    }
    summa  = _linea_infima(comp, ZEPHYRUM) - XXIV;
    r      = _pictum(comp, GABC_PICTUM_RECTANGULUM, II, -I);
    si (r == NIHIL || r->y - summa != dy || r->altitudo != altitudo)
    {
        imprimere("  %s: dy %d altitudo %u\n", literae,
            r ? (integer)(r->y - summa) : -999,
            r ? (unsigned)r->altitudo : 0u);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_signa_probare (
    CredoContextus* c)
{
         GabcCompositio* comp;
    constans GabcPictum* p;
    constans GabcPictum* r;
                    s32  summa;
                    i32  i;
                    i32  n;

    NEUMAE("(c4) a(g.)", I, "punctum:0 0 0;punctum_morae:0 10 0");
    NEUMAE("(c4) a(h.)", I, "punctum:0 0 0;punctum_morae:0 10 -4");
    NEUMAE("(c4) a(g..)", I,
        "punctum:0 0 0;punctum_morae:0 10 0;punctum_morae:0 14 0");
    NEUMAE("(c4) a(g_)", I, "punctum:0 0 0;episema:8 0 -6");
    NEUMAE("(c4) a(g_0)", I, "punctum:0 0 0;episema:8 0 6");
    /* episemata contigua: linea una */
    NEUMAE("(c4) a(g_h_g)", I,
        "punctum:0 0 0;punctum:0 8 -4;rect 7 -4 1x5;punctum:0 16 0;"
        "rect 15 -4 1x5;episema:16 0 -10");
    NEUMAE("(c4) a(g')", I, "punctum:0 0 0;ictus:0 4 8");
    NEUMAE("(c4) a(g'1)", I, "punctum:0 0 0;ictus:0 4 -7");
    /* alteratio ante notam, s/IV intervallum */
    NEUMAE("(c4) a(gxg)", I, "bemolle:0 0 0;punctum:0 8 0");

    /* divisiones relativae ad lineam summam */
    CREDO_VERUM(_divisio(c, "(c4) a(g) (,)", -IV, IX));
    CREDO_VERUM(_divisio(c, "(c4) a(g) (;)", IV, XVII));
    CREDO_VERUM(_divisio(c, "(c4) a(g) (:)", ZEPHYRUM, XXV));
    CREDO_VERUM(_divisio(c, "(c4) a(g) (^)", -IV, V));
    comp = _componere(c, "(c4) a(g) (::)", NIHIL);
    CREDO_NON_NIHIL(comp);
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        si (   comp->picta[i].syllaba == II
            && comp->picta[i].genus   == GABC_PICTUM_RECTANGULUM)
        {
            n++;
        }
    }
    CREDO_AEQUALIS_I32(n, II);
    r = _pictum(comp, GABC_PICTUM_RECTANGULUM, II, -I);
    CREDO_AEQUALIS_I32(r[I].latitudo, I);
    CREDO_AEQUALIS_S32(r[I].x - r->x, V);
    comp   = _componere(c, "(c4) a(g) (`)", NIHIL);
    summa  = _linea_infima(comp, ZEPHYRUM) - XXIV;
    p      = _pictum(comp, GABC_PICTUM_PARS, II, (s32)NEUMAE_VIRGULA);
    CREDO_NON_NIHIL(p);
    CREDO_AEQUALIS_S32(p->y - summa, -II);

    /* alt supra systema; translatio non picta */
    comp   = _componere(c, "(c4) <alt>Ps.</alt>a(g)", NIHIL);
    summa  = _linea_infima(comp, ZEPHYRUM) - XXIV;
    n      = ZEPHYRUM;
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        p = &comp->picta[i];
        si (p->genus == GABC_PICTUM_TEXTUS && p->textus.mensura == III
            && memcmp(p->textus.datum, "Ps.", III) == ZEPHYRUM)
        {
            n++;
            CREDO_VERUM(p->y + VIII <= summa);
        }
    }
    CREDO_AEQUALIS_I32(n, I);
    comp = _componere(c, SINE_INITIALI "(c4) a[xyz](g)", NIHIL);
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        p = &comp->picta[i];
        CREDO_FALSUM(p->genus == GABC_PICTUM_TEXTUS
            && p->textus.mensura == III
            && memcmp(p->textus.datum, "xyz", III) == ZEPHYRUM);
    }
}


/* ==================================================
 * III: syllabae et textus
 * ================================================== */

/* x notae primae syllabae I - x textus eius */
interior s32
_centrum (
          CredoContextus* c,
      constans character* literae)
{
         GabcCompositio* comp;
    constans GabcPictum* n;
    constans GabcPictum* t;

    comp  = _componere(c, literae, NIHIL);
    n     = comp ? _pictum(comp, GABC_PICTUM_PARS, I, -I) : NIHIL;
    t     = comp ? _textus(comp, I) : NIHIL;
    si (n == NIHIL || t == NIHIL)
    {
        redde -999;
    }
    redde n->x - t->x;
}

interior vacuum
_syllabas_probare (
    CredoContextus* c)
{
         GabcCompositio* comp;
    constans GabcPictum* t1;
    constans GabcPictum* t2;
    constans GabcPictum* n1;
                    i32  i;
                    i32  n;

    /* vocalis prima: Sal - a ad runam I (centrum IX), caput IV */
    CREDO_AEQUALIS_S32(_centrum(c, SINE_INITIALI "(c4) Sal(g)"), V);
    /* {} data: e{ve}r - runae I..III, centrum XII */
    CREDO_AEQUALIS_S32(_centrum(c, SINE_INITIALI "(c4) e{ve}r(g)"),
        VIII);
    /* diphthongus ae una */
    CREDO_AEQUALIS_S32(_centrum(c, SINE_INITIALI "(c4) cae(g)"), VIII);
    /* u post q consonans: qui - i */
    CREDO_AEQUALIS_S32(_centrum(c, SINE_INITIALI "(c4) qui(g)"), XI);
    /* i initialis ante vocalem consonans: Ie - e */
    CREDO_AEQUALIS_S32(_centrum(c, SINE_INITIALI "(c4) Ie(g)"), V);
    /* sine vocali: medium syllabae */
    CREDO_AEQUALIS_S32(_centrum(c, SINE_INITIALI "(c4) st(g)"), II);
    /* elisio: vocalis elisa praeteritur (m-e-a -> a) */
    CREDO_AEQUALIS_S32(_centrum(c, SINE_INITIALI "(c4) m<e>e</e>a(g)"),
        XI);

    /* syllabae eiusdem verbi contiguae; verbum novum cellulam una */
    comp  = _componere(c, SINE_INITIALI "(c4) ab(g)cd(g)", NIHIL);
    t1    = _textus(comp, I);
    t2    = _textus(comp, II);
    CREDO_AEQUALIS_S32(t2->x - (t1->x + XII), I);
    comp  = _componere(c, SINE_INITIALI "(c4) ab(g) cd(g)", NIHIL);
    t1    = _textus(comp, I);
    t2    = _textus(comp, II);
    CREDO_AEQUALIS_S32(t2->x - (t1->x + XII), VI);

    /* lineola in medio intervalli satis lati */
    comp  = _componere(c, SINE_INITIALI "(c4) a(gh/hg/gh)bc(g)", NIHIL);
    n1    = _pictum(comp, GABC_PICTUM_PARS, I, -I);
    n     = ZEPHYRUM;
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        t1 = &comp->picta[i];
        si (t1->genus == GABC_PICTUM_TEXTUS && t1->textus.mensura == I
            && t1->textus.datum[ZEPHYRUM] == '-')
        {
            n++;
            CREDO_AEQUALIS_S32(t1->x - n1->x, XXV);
        }
    }
    CREDO_AEQUALIS_I32(n, I);
    comp = _componere(c, SINE_INITIALI "(c4) ab(g)cd(g)", NIHIL);
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        t1 = &comp->picta[i];
        CREDO_FALSUM(t1->genus == GABC_PICTUM_TEXTUS
            && t1->textus.mensura == I
            && t1->textus.datum[ZEPHYRUM] == '-');
    }

    /* textus sine notis; notae sine textu; divisio sine textu */
    comp = _componere(c, SINE_INITIALI "(c4) a(g) *() b(g) (;)", NIHIL);
    CREDO_NON_NIHIL(_textus(comp, II));
    CREDO_NIHIL(_pictum(comp, GABC_PICTUM_PARS, II, -I));
    CREDO_AEQUALIS_S32(comp->syllabae[II].pagina, ZEPHYRUM);
    CREDO_NIHIL(_textus(comp, IV));
    CREDO_NON_NIHIL(_pictum(comp, GABC_PICTUM_RECTANGULUM, IV, -I));
    comp = _componere(c, SINE_INITIALI "(c4) (g)", NIHIL);
    CREDO_NIHIL(_textus(comp, I));
    CREDO_NON_NIHIL(_pictum(comp, GABC_PICTUM_PARS, I, -I));

    /* initialis: littera prima ex syllaba prima transit */
    comp  = _componere(c, "(c4) Sal(g)ve(h)", NIHIL);
    t1    = _pictum(comp, GABC_PICTUM_INITIALIS, -II, -I);
    CREDO_NON_NIHIL(t1);
    CREDO_AEQUALIS_I32(t1->textus.mensura, I);
    CREDO_AEQUALIS_I32((i32)t1->textus.datum[ZEPHYRUM], (i32)'S');
    t2 = _textus(comp, I);
    CREDO_AEQUALIS_I32(t2->textus.mensura, II);
}


/* ==================================================
 * IV: lineae et paginae
 * ================================================== */

/* x custodis + latitudo (IV) in systemate primo */
interior s32
_finis_custodis (
    constans GabcCompositio* comp)
{
    constans GabcPictum* p;

    p = _pictum(comp, GABC_PICTUM_PARS, -II, (s32)NEUMAE_CUSTOS);
    redde p ? p->x + IV : -I;
}

interior vacuum
_lineas_probare (
    CredoContextus* c)
{
              character  textus[(IV * M) * II];
         GabcCompositio* comp;
    constans GabcPictum* p;
                    i32  i;
                    i32  n;
                    i32  in_primo;
                    s32  infima;

    /* z frangit et iustificat: custos ad marginem dextrum */
    comp = _componere(c, SINE_INITIALI "(c4) a(g) (z) b(g)", NIHIL);
    CREDO_AEQUALIS_I32(_systemata(comp), II);
    CREDO_AEQUALIS_S32(_finis_custodis(comp), CMLII);
    /* custos positionem notae sequentis monstrat (g: -XII) */
    p = _pictum(comp, GABC_PICTUM_PARS, -II, (s32)NEUMAE_CUSTOS);
    CREDO_AEQUALIS_S32(p->y - _linea_infima(comp, ZEPHYRUM), -XII);
    CREDO_AEQUALIS_S32(p->parametrum, I);
    /* Z frangit sine iustificatione */
    comp = _componere(c, SINE_INITIALI "(c4) a(g) (Z) b(g)", NIHIL);
    CREDO_AEQUALIS_I32(_systemata(comp), II);
    CREDO_VERUM(_finis_custodis(comp) < CMLII);
    /* z- custodem vetat */
    comp = _componere(c, SINE_INITIALI "(c4) a(g) (z-) b(g)", NIHIL);
    CREDO_AEQUALIS_I32(_numerus_partium(comp, NEUMAE_CUSTOS, -II),
        ZEPHYRUM);

    /* avide: LX syllabae, LVIII in systemate primo */
    strcpy(textus, SINE_INITIALI "(c4)");
    per (i = ZEPHYRUM; i < LX; i++)
    {
        strcat(textus, " a(g)");
    }
    comp  = _componere(c, textus, NIHIL);
    CREDO_AEQUALIS_I32(_systemata(comp), II);
    infima    = _linea_infima(comp, ZEPHYRUM);
    in_primo  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        p = &comp->picta[i];
        si (   p->genus == GABC_PICTUM_PARS && p->pars == NEUMAE_PUNCTUM
            && p->y > infima - XXXII && p->y < infima + XVI)
        {
            in_primo++;
        }
    }
    CREDO_AEQUALIS_I32(in_primo, LVIII);

    /* titulus et annotatio solum in pagina prima; paginae systemata
     * tota tenent */
    strcpy(textus,
        "name:Probatio;\nannotation:Ant.;\ninitial-style:0;\n%%\n"
        "(c4)");
    per (i = ZEPHYRUM; i < XXV; i++)
    {
        strcat(textus, " a(g) (z)");
    }
    comp = _componere(c, textus, NIHIL);
    CREDO_NON_NIHIL(comp);
    CREDO_MAIOR_I32(comp->numerus_paginarum, I);
    CREDO_AEQUALIS_I32(comp->paginae[ZEPHYRUM], ZEPHYRUM);
    CREDO_AEQUALIS_I32(comp->paginae[comp->numerus_paginarum],
        comp->numerus_pictorum);
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        s32 x0;
        s32 y0;
        s32 x1;
        s32 y1;

        p = &comp->picta[i];
        CREDO_VERUM(_capsa(p, &x0, &y0, &x1, &y1));
        CREDO_VERUM(y0 >= ZEPHYRUM && y1 <= DLXXIII);
        si (   p->genus == GABC_PICTUM_TEXTUS
            && (p->fons == FONS_TITULI
                || p->tinctura == GABC_TINCTURA_RUBRICA))
        {
            n++;
            CREDO_AEQUALIS_S32(_pagina_picti(comp, i), ZEPHYRUM);
        }
        si (i > ZEPHYRUM)
        {
            CREDO_VERUM(_pagina_picti(comp, i)
                >= _pagina_picti(comp, i - I));
        }
    }
    CREDO_AEQUALIS_I32(n, II);
}


/* ==================================================
 * V: latitudo (Review Focus 3)
 * ================================================== */

interior vacuum
_latitudinem_probare (
    CredoContextus* c)
{
                 character  textus[(IV * M)];
    GabcCompositionisForma  forma;
            GabcCompositio* comp;
       constans GabcPictum* p;
       constans GabcPictum* t1;
       constans GabcPictum* t2;
                       i32  i;
                       s32  x0;
                       s32  y0;
                       s32  x1;
                       s32  y1;

    /* melisma XL notarum in tabula angusta: frangitur ad spatia,
     * custos in fine omnis systematis, nihil extra lineam */
    gabc_compositionis_formam_ordinariam(&forma);
    forma.latitudo = CCC;
    strcpy(textus, SINE_INITIALI "(c4) a(g");
    per (i = I; i < XL; i++)
    {
        strcat(textus, i % II ? "/h" : "/g");
    }
    strcat(textus, ") b(g)");
    comp = _componere(c, textus, &forma);
    CREDO_NON_NIHIL(comp);
    CREDO_MAIOR_I32(_systemata(comp), II);
    CREDO_AEQUALIS_I32(_numerus_partium(comp, NEUMAE_CUSTOS, -II),
        _systemata(comp) - I);
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        p = &comp->picta[i];
        CREDO_VERUM(_capsa(p, &x0, &y0, &x1, &y1));
        CREDO_VERUM(x1 <= CCC - VIII);
    }
    per (i = ZEPHYRUM; i < comp->numerus_notarum; i++)
    {
        CREDO_AEQUALIS_S32(comp->notae[i].pagina >= ZEPHYRUM ? I : 0,
            I);
    }

    /* textus latior notis: textus sequens post eum */
    comp = _componere(c,
        SINE_INITIALI "(c4) supercalifragilistic(g) b(g)", NIHIL);
    t1 = _textus(comp, I);
    t2 = _textus(comp, II);
    CREDO_VERUM(t2->x >= t1->x + (s32)t1->textus.mensura * VI + VI);

    /* neuma una latior linea: excedit, compositio finitur */
    gabc_compositionis_formam_ordinariam(&forma);
    forma.latitudo = CXXVIII;
    strcpy(textus, SINE_INITIALI "(c4) a(");
    per (i = ZEPHYRUM; i < XXX; i++)
    {
        strcat(textus, i % II ? "h" : "g");
    }
    strcat(textus, ") b(g)");
    comp = _componere(c, textus, &forma);
    CREDO_NON_NIHIL(comp);
    per (i = ZEPHYRUM; i < comp->numerus_notarum; i++)
    {
        CREDO_VERUM(comp->notae[i].pagina >= ZEPHYRUM);
    }
}


/* ==================================================
 * VI: extra systema (Review Focus 4)
 * ================================================== */

/* rectangulum SYSTEMA syllabae I (linea addita) */
interior constans GabcPictum*
_addita (
    constans GabcCompositio* comp)
{
    i32 i;

    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        si (   comp->picta[i].genus    == GABC_PICTUM_RECTANGULUM
            && comp->picta[i].tinctura == GABC_TINCTURA_SYSTEMA
            && comp->picta[i].syllaba  == I)
        {
            redde &comp->picta[i];
        }
    }
    redde NIHIL;
}

interior vacuum
_extra_systema_probare (
    CredoContextus* c)
{
         GabcCompositio* comp;
    constans GabcPictum* p;
    constans GabcPictum* n;
                    s32  infima;
                    i32  i;
                    i32  claves;

    /* a infra: linea addita in positione I, latitudo capitis + s/II */
    comp  = _componere(c, "(c4) a(a)", NIHIL);
    p     = _addita(comp);
    n     = _pictum(comp, GABC_PICTUM_PARS, I, -I);
    CREDO_NON_NIHIL(p);
    CREDO_AEQUALIS_S32(p->y - _linea_infima(comp, ZEPHYRUM), VIII);
    CREDO_AEQUALIS_S32(p->x, n->x - II);
    CREDO_AEQUALIS_I32(p->latitudo, XII);
    /* m supra: linea in positione XI */
    comp  = _componere(c, "(c4) a(m)", NIHIL);
    p     = _addita(comp);
    CREDO_NON_NIHIL(p);
    CREDO_AEQUALIS_S32(p->y - _linea_infima(comp, ZEPHYRUM), -XXXII);
    /* intra systema: nulla */
    comp = _componere(c, "(c4) a(e)", NIHIL);
    CREDO_NIHIL(_addita(comp));
    comp = _componere(c, "(c4) a(c)", NIHIL);
    CREDO_NIHIL(_addita(comp));

    /* clavis mutata in linea: c3 in positione VII */
    comp  = _componere(c, "(c4) a(g) (c3) b(g)", NIHIL);
    p     = _pictum(comp, GABC_PICTUM_PARS, II, (s32)NEUMAE_CLAVIS_C);
    CREDO_NON_NIHIL(p);
    CREDO_AEQUALIS_S32(p->y - _linea_infima(comp, ZEPHYRUM), -XVI);
    /* cb3: bemolle in b intra systema (positio VI) */
    comp = _componere(c, "(cb3) a(g)", NIHIL);
    p = _pictum(comp, GABC_PICTUM_PARS, ZEPHYRUM,
        (s32)NEUMAE_BEMOLLE);
    CREDO_NON_NIHIL(p);
    CREDO_AEQUALIS_S32(p->y - _linea_infima(comp, ZEPHYRUM), -XII);

    /* custos scriptus ante clavem mutatam: positio sua (i, VIII,
     * caulis deorsum), nullus custos automaticus; systema secundum
     * clavem novam solam habet */
    comp = _componere(c, "(c4) a(gh) (i+z) (c3) b(i)", NIHIL);
    CREDO_AEQUALIS_I32(_systemata(comp), II);
    CREDO_AEQUALIS_I32(_numerus_partium(comp, NEUMAE_CUSTOS, -II), I);
    p = _pictum(comp, GABC_PICTUM_PARS, -II, (s32)NEUMAE_CUSTOS);
    CREDO_AEQUALIS_S32(p->y - _linea_infima(comp, ZEPHYRUM), -XX);
    CREDO_AEQUALIS_S32(p->parametrum, -I);
    infima = _linea_infima(comp, I);
    claves = ZEPHYRUM;
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        p = &comp->picta[i];
        si (p->genus == GABC_PICTUM_PARS && p->pars == NEUMAE_CLAVIS_C
            && p->y > infima - XL && p->y <= infima)
        {
            claves++;
            CREDO_AEQUALIS_S32(p->y - infima, -XVI);
        }
    }
    CREDO_AEQUALIS_I32(claves, I);
    /* custos automaticus ante clavem mutatam: positio notae
     * sequentis ut scripta (i) */
    comp  = _componere(c, "(c4) a(g) (z) (c3) b(i)", NIHIL);
    p     = _pictum(comp, GABC_PICTUM_PARS, -II, (s32)NEUMAE_CUSTOS);
    CREDO_NON_NIHIL(p);
    CREDO_AEQUALIS_S32(p->y - _linea_infima(comp, ZEPHYRUM), -XX);
}


/* ==================================================
 * VII: ictus muris
 * ================================================== */

interior vacuum
_ictum_probare (
    CredoContextus* c)
{
          constans s32  syllabae[] = { I, II, IV };
        GabcCompositio* comp;
    constans GabcLocus* l;
                   i32  k;

    comp = _componere(c, SINE_INITIALI "(c4) a(g) b(h) (z) c(g)",
        NIHIL);
    CREDO_NON_NIHIL(comp);
    per (k = ZEPHYRUM; k < III; k++)
    {
        l = &comp->syllabae[syllabae[k]];
        CREDO_AEQUALIS_S32(l->pagina, ZEPHYRUM);
        CREDO_AEQUALIS_S32(gabc_compositio_syllaba_ad_punctum(comp,
            ZEPHYRUM, l->x + (s32)l->latitudo / II,
            l->y + (s32)l->altitudo / II), syllabae[k]);
    }
    /* inter systemata: nihil */
    l = &comp->syllabae[I];
    CREDO_AEQUALIS_S32(gabc_compositio_syllaba_ad_punctum(comp,
        ZEPHYRUM,
        l->x, l->y + (s32)l->altitudo + II), -I);
    /* pagina alia */
    CREDO_AEQUALIS_S32(gabc_compositio_syllaba_ad_punctum(comp, I,
        l->x, l->y), -I);
    CREDO_AEQUALIS_S32(gabc_compositio_syllaba_ad_punctum(comp, M,
        l->x, l->y), -I);
}


/* ==================================================
 * VIII: Salve (systema primum manu, deinde fixum)
 * ================================================== */

interior b32
_pictum_est (
     constans GabcPictum* p,
          GabcPictiGenus  genus,
                     s32  x,
                     s32  y)
{
    si (p == NIHIL || p->genus != genus || p->x != x || p->y != y)
    {
        imprimere("  exspectatum %d (%d,%d), receptum %d (%d,%d)\n",
            (integer)genus, (integer)x, (integer)y,
            p ? (integer)p->genus : -I, p ? (integer)p->x : -I,
            p ? (integer)p->y : -I);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_salvem_probare (
    CredoContextus* c)
{
    GabcCompositionisForma  forma;
                GabcCantus* cantus;
            GabcCompositio* comp;
         ChordaAedificator* a;
                    chorda  exscriptum;
                    chorda  fixum;
                       i32  i;

    cantus = gabc_legere(filum_legere_totum(VIA_SALVE, c->piscina),
        c->piscina);
    CREDO_NON_NIHIL(cantus);

    /* systema unum (latitudo IV millia): manu derivata - titulus VIII x
     * XVI, initialis scala II (XL / XVI), intentio XX, linea ima LXIV;
     * ordo textus ex imo totius systematis: caulis clivis ri(dc)
     * usque ad s/II + s/IV sub c (+X, imum XI) + s/IV = XIII ->
     * LXXVII */
    gabc_compositionis_formam_ordinariam(&forma);
    forma.latitudo  = (IV * M);
    comp            = gabc_componere(cantus, &forma, c->piscina);
    CREDO_NON_NIHIL(comp);
    CREDO_AEQUALIS_I32(_systemata(comp), I);
    CREDO_AEQUALIS_S32(_linea_infima(comp, ZEPHYRUM), LXIV);
    CREDO_VERUM(_pictum_est(_pictum(comp, GABC_PICTUM_INITIALIS, -II,
        -I),
        GABC_PICTUM_INITIALIS, VIII, XL));
    CREDO_AEQUALIS_I32(_pictum(comp, GABC_PICTUM_INITIALIS, -II,
        -I)->altitudo, XXXII);
    CREDO_VERUM(_pictum_est(_pictum(comp, GABC_PICTUM_PARS, ZEPHYRUM,
        (s32)NEUMAE_CLAVIS_C), GABC_PICTUM_PARS, XXVIII, XL));
    CREDO_VERUM(_pictum_est(_pictum(comp, GABC_PICTUM_PARS, I, -I),
        GABC_PICTUM_PARS, XLII, LXVIII));
    CREDO_VERUM(_pictum_est(_textus(comp, I), GABC_PICTUM_TEXTUS, XLIII,
        LXXVII));
    CREDO_VERUM(_pictum_est(_pictum(comp, GABC_PICTUM_PARS, II, -I),
        GABC_PICTUM_PARS, LX, LX));
    CREDO_VERUM(_pictum_est(_textus(comp, II), GABC_PICTUM_TEXTUS, LV,
        LXXVII));
    CREDO_VERUM(_pictum_est(_pictum(comp, GABC_PICTUM_PARS, III, -I),
        GABC_PICTUM_PARS, LXXXIV, LII));
    CREDO_VERUM(_pictum_est(_textus(comp, III), GABC_PICTUM_TEXTUS,
        LXXIX,
        LXXVII));
    CREDO_VERUM(_pictum_est(_pictum(comp, GABC_PICTUM_PARS, III,
        (s32)NEUMAE_ICTUS), GABC_PICTUM_PARS, LXXXVIII, LX));

    /* forma ordinaria: fixum */
    gabc_compositionis_formam_ordinariam(&forma);
    comp = gabc_componere(cantus, &forma, c->piscina);
    CREDO_NON_NIHIL(comp);
    per (i = ZEPHYRUM; i < comp->numerus_notarum; i++)
    {
        CREDO_VERUM(comp->notae[i].pagina >= ZEPHYRUM);
    }
    a = chorda_aedificator_creare(c->piscina, (IV * M) * XVI);
    CREDO_VERUM(gabc_compositionem_scribere(comp, a));
    exscriptum = chorda_aedificator_finire(a);
    si (getenv("GABC_COMPOSITIO_SCRIBERE") != NIHIL)
    {
        CREDO_VERUM(filum_scribere(VIA_FIXI_SALVE, exscriptum));
        imprimere("  %s rescriptum (%u octeti)\n", VIA_FIXI_SALVE,
            (unsigned)exscriptum.mensura);
    }
    fixum = filum_legere_totum(VIA_FIXI_SALVE, c->piscina);
    CREDO_CHORDA_AEQUALIS(exscriptum, fixum);
}


/* ==================================================
 * IX: corpus GregoBase
 * ================================================== */

interior vacuum
_corpus_probare (
    CredoContextus* c)
{
              constans i32  spatia[] = { VI, VIII, XXIV };
    GabcCompositionisForma  forma;
                   Piscina* opus;
                    chorda  manifestum;
                    chorda  textus;
                GabcCantus* cantus;
            GabcCompositio* comp;
                 character  via[CCLVI];
                       i32  i;
                       i32  k;
                       i32  j;
                       i32  initium;
                       i32  plagulae;
                       i32  paginae;
                       i32  picta;
                       i32  mala;
                       i32  extra;
                       s32  x0;
                       s32  y0;
                       s32  x1;
                       s32  y1;
             NeumaeMensura  m;
       constans GabcPictum* p;

    manifestum = filum_legere_totum(VIA_MANIFESTI, c->piscina);
    CREDO_MAIOR_I32(manifestum.mensura, ZEPHYRUM);
    opus      = piscina_generare_dynamicum("gabc_compositio_corpus",
                    (memoriae_index)(CCLVI * MXXIV));
    plagulae  = ZEPHYRUM;
    paginae   = ZEPHYRUM;
    picta     = ZEPHYRUM;
    mala      = ZEPHYRUM;
    extra     = ZEPHYRUM;
    i         = ZEPHYRUM;
    /* linea prima (DATA ...) praeteritur */
    dum (i < manifestum.mensura && manifestum.datum[i] != '\n')
    {
        i++;
    }
    dum (i < manifestum.mensura)
    {
        i++;
        initium = i;
        dum (   i < manifestum.mensura && manifestum.datum[i] != '\t'
             && manifestum.datum[i] != '\n')
        {
            i++;
        }
        si (i - initium < V || i >= manifestum.mensura)
        {
            frange;
        }
        strcpy(via, VIA_FIXORUM);
        memcpy(via + strlen(VIA_FIXORUM), manifestum.datum + initium,
            (size_t)(i - initium));
        via[strlen(VIA_FIXORUM) + (size_t)(i - initium)] = '\0';
        dum (i < manifestum.mensura && manifestum.datum[i] != '\n')
        {
            i++;
        }
        textus = filum_legere_totum(via, opus);
        cantus = gabc_legere(textus, opus);
        plagulae++;
        per (k = ZEPHYRUM; k < III; k++)
        {
            gabc_compositionis_formam_ordinariam(&forma);
            forma.spatium  = spatia[k];
            comp           = gabc_componere(cantus, &forma, opus);
            si (comp == NIHIL)
            {
                imprimere("  %s/%u: NIHIL\n", via, (unsigned)spatia[k]);
                mala++;
                perge;
            }
            paginae  += comp->numerus_paginarum;
            picta    += comp->numerus_pictorum;
            per (j = ZEPHYRUM; j < comp->numerus_notarum; j++)
            {
                si (comp->notae[j].pagina < ZEPHYRUM)
                {
                    mala++;
                }
            }
            per (j = ZEPHYRUM; j < comp->numerus_pictorum; j++)
            {
                p = &comp->picta[j];
                si (p->genus == GABC_PICTUM_PARS)
                {
                    neumae_ductae_metiri(p->pars, spatia[k],
                        p->parametrum,
                        &m);
                    x0 = p->x - m.origo_x;
                    y0 = p->y - m.origo_y;
                    x1 = x0 + (s32)m.latitudo;
                    y1 = y0 + (s32)m.altitudo;
                }
                alioquin
                {
                    _capsa(p, &x0, &y0, &x1, &y1);
                }
                si (x0 < ZEPHYRUM || y0 < ZEPHYRUM)
                {
                    mala++;
                }
                si (   x1 > (s32)forma.latitudo
                    && !(p->genus == GABC_PICTUM_TEXTUS
                         && p->fons == FONS_TITULI))
                {
                    extra++;
                }
            }
        }
        piscina_vacare(opus);
    }
    piscina_destruere(opus);
    imprimere("  corpus: plagulae %u, paginae %u, picta %u, extra %u\n",
        (unsigned)plagulae, (unsigned)paginae, (unsigned)picta,
        (unsigned)extra);
    CREDO_AEQUALIS_I32(plagulae, LXXXII);
    /* summae fixae (VI + VIII + XXIV; P3, rescriptae P4: lineolae,
     * caules): mutatio
     * compositionis hic apparet - causa in worklog nominanda */
    CREDO_AEQUALIS_I32(paginae, CDLXXI);
    CREDO_AEQUALIS_I32(picta, CXI * M + DCCC);
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
    CREDO_AEQUALIS_I32(extra, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I: neumae", _neumae_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "II: signa", _signa_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "III: syllabae et textus", _syllabas_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "IV: lineae et paginae", _lineas_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "V: latitudo", _latitudinem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "VI: extra systema", _extra_systema_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "VII: ictus muris", _ictum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "VIII: Salve", _salvem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "IX: corpus GregoBase", _corpus_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("gabc_compositio", SECTIONES);
}
