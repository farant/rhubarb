/* gabc_compositio.c - GabcCantus -> paginae pictorum (gabc pars II,
 * P3). Regulae in include/gabc_compositio.h; valores in
 * probationes/probatio_gabc_compositio.c.
 *
 * CURSUS: syllaba quaeque in FRUSTA componitur (elementa usque ad
 * spatium fragilis aut fractionem; coordinatae x ab initio frusti,
 * y a linea ima systematis = 0, sursum negativa). Deinde frusta in
 * systema currens ponuntur duobus cursoribus (notarum et textus);
 * systema plenum aut fractum FINITUR: custos, iustificatio,
 * lineolae, ordo textus, lineae systematis, pagina. Capsae notarum
 * ut picta interna (genus PICTUM_CAPSA) cum ceteris moventur et in
 * fine in notae[] transeunt. */
#include "latina.h"
#include "gabc_compositio.h"
#include "fons.h"
#include "utf8.h"
#include "xar.h"
#include <string.h>

#define PICTUM_CAPSA     ((GabcPictiGenus)(GABC_PICTUM_INITIALIS + I))
#define Y_CANTATUS       (-(M * M))
#define Y_SUPRA          (-(M * M) - I)
#define RUNAE_MAXIMAE    CCLVI
#define LATITUDO_FONTIS_INITIALIS  VIII
#define ALTITUDO_FONTIS_INITIALIS  XVI

#define FRUSTUM_FINIS    ZEPHYRUM
#define FRUSTUM_SPATIUM  I
#define FRUSTUM_FRACTIO  II

nomen structura {
          b32 nota;
    character clavis;
           i8 linea;
          b32 bemolle;
} ClavisStatus;

nomen structura {
             i32 primum;                  /* in frusta_picta */
             i32 numerus;
             s32 latitudo;
             s32 post;                    /* spatium post frustum */
             i32 genus;                   /* FRUSTUM_* */
             b32 iustificata;
             s32 custos_modus;
             s32 elementum_primum;
             s32 elementum_sequens;
             b32 finit_custode;
             b32 cantatum;                /* neuma, alteratio, divisio
                                           * (non clavis sola) */
    ClavisStatus clavis;         /* in initio frusti */
} Frustum;

nomen structura {
    s32 syllaba;
    i32 primum;                  /* in systemate */
    b32 textus;
    s32 textus_initium;
    s32 textus_finis;
    b32 verbum_incipit;
} SyllabaPosita;

nomen structura {
    s32 x;
    s32 latitudo;
    s32 summum;
    s32 imum;
    b32 ultimum_segmenti;
    s32 finis_segmenti;
} CaputPositum;

nomen structura {
    b32 adest;
    s32 latitudo;                /* pixelis */
    s32 centrum;                 /* pixelis ab initio */
} TextusMensura;

nomen structura {
                constans GabcCantus* cantus;
    constans GabcCompositionisForma* forma;
                            Piscina* piscina;
                                s32  s;
                                s32  h;
                                s32  k;
                                s32  q;
                                s32  cw;
                                s32  fh;
                                s32  cwt;
                                s32  fht;
                                s32  lineae;
    /* positio lineae summae; limes = finis lineae - custos */
             s32 summa;
             s32 margo;
             s32 finis_lineae;
             s32 limes;
    ClavisStatus clavis;
    /* initialis */
       s32 initialis_syllaba;
       s32 initialis_scala;
       s32 initialis_systemata;
       s32 intentio;
    chorda initialis_littera;
    /* systema currens */
    Xar* systema;                /* GabcPictum */
    Xar* positae;                /* SyllabaPosita */
    s32  systematis_index;
    s32  x_linea;
    s32  cursor_notae;
    s32  textus_finis;
    b32  plenum;
    b32  cantatum;               /* notae aut textus, non clavis sola */
    b32  ultimum_custos;
    s32  fractio_syllaba;
    s32  fractio_elementum;
    /* frusta syllabae currentis */
             Xar* frusta_picta;
             Xar* frusta;
             s32  libramentum;
    CaputPositum  capita[CCLVI];
    /* exitus */
    Xar* omnia;
    Xar* paginae_pictorum;       /* s32 per pictum */
    Xar* systemata_pictorum;     /* s32 per pictum */
    s32  pagina;
    s32  cursor_paginae;
    b32  pagina_vacua;
} Compositor;


/* ==================================================
 * Instrumenta parva
 * ================================================== */

interior s32
maximum (
    s32 a,
    s32 b)
{
    redde a > b ? a : b;
}

interior s32
minimum (
    s32 a,
    s32 b)
{
    redde a < b ? a : b;
}

interior s32
y_positionis (
    constans Compositor* co,
                    s32  positio)
{
    redde -(positio - III) * co->h;
}

interior b32
nota_mixta (
    s32 runa)
{
    redde runa >= 0x300 && runa <= 0x36F;
}

/* runae visibiles (notae compositae non numerantur) */
interior s32
runae_numerare (
    chorda ch)
{
    constans i8* p;
    constans i8* finis;
            s32  n;

    p      = ch.datum;
    finis  = ch.datum + ch.mensura;
    n      = ZEPHYRUM;
    dum (p < finis)
    {
        si (!nota_mixta(utf8_decodere(&p, finis)))
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
mensuram_capere (
    constans Compositor* co,
             NeumaePars  pars,
                    s32  parametrum,
          NeumaeMensura* m)
{
    si (!neumae_ductae_metiri(pars, (i32)co->s, parametrum, m))
    {
        memset(m, ZEPHYRUM, magnitudo(*m));
    }
}

/* capsa picti; FALSUM pro textu cuius ordo nondum statutus */
interior b32
capsam_picti (
    constans Compositor* co,
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
            mensuram_capere(co, p->pars, p->parametrum, &m);
            *x0 = p->x - m.origo_x;
            *y0 = p->y - m.origo_y;
            *x1 = *x0 + (s32)m.latitudo;
            *y1 = *y0 + (s32)m.altitudo;
            redde VERUM;
        casus GABC_PICTUM_TEXTUS:
            si (p->y == Y_CANTATUS || p->y == Y_SUPRA)
            {
                redde FALSUM;
            }
            *x0 = p->x;
            *y0 = p->y;
            *x1 = p->x
                + runae_numerare(p->textus) * fons_latitudo(p->fons);
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

interior GabcPictum*
emittere (
                 Xar* ad,
      GabcPictiGenus  genus,
                 s32  x,
                 s32  y,
                 s32  syllaba,
                 s32  elementum,
                 s32  nota)
{
    GabcPictum* p;

    p = (GabcPictum*)xar_addere(ad);
    si (p == NIHIL)
    {
        redde NIHIL;
    }
    memset(p, ZEPHYRUM, magnitudo(*p));
    p->genus      = genus;
    p->x          = x;
    p->y          = y;
    p->tinctura   = GABC_TINCTURA_ATRAMENTUM;
    p->syllaba    = syllaba;
    p->elementum  = elementum;
    p->nota       = nota;
    redde p;
}

interior vacuum
partem_emittere (
                 Xar* ad,
          NeumaePars  pars,
                 s32  parametrum,
                 s32  x,
                 s32  y,
                 s32  syllaba,
                 s32  elementum,
                 s32  nota)
{
    GabcPictum* p;

    p = emittere(ad, GABC_PICTUM_PARS, x, y, syllaba, elementum, nota);
    si (p)
    {
        p->pars        = pars;
        p->parametrum  = parametrum;
    }
}

interior vacuum
rectangulum_emittere (
                 Xar* ad,
                 s32  x,
                 s32  y,
                 s32  latitudo,
                 s32  altitudo,
        GabcTinctura  tinctura,
                 s32  syllaba,
                 s32  elementum,
                 s32  nota)
{
    GabcPictum* p;

    si (latitudo <= ZEPHYRUM || altitudo <= ZEPHYRUM)
    {
        redde;
    }
    p = emittere(ad, GABC_PICTUM_RECTANGULUM, x, y, syllaba, elementum,
        nota);
    si (p)
    {
        p->latitudo = (i32)latitudo;
        p->altitudo = (i32)altitudo;
        p->tinctura = tinctura;
    }
}

interior vacuum
textum_emittere (
                 Xar* ad,
              chorda  textus,
                 i32  fons,
                 s32  x,
                 s32  y,
        GabcTinctura  tinctura,
                 s32  syllaba)
{
    GabcPictum* p;

    p = emittere(ad, GABC_PICTUM_TEXTUS, x, y, syllaba, -I, -I);
    si (p)
    {
        p->textus    = textus;
        p->fons      = fons;
        p->tinctura  = tinctura;
    }
}

/* nota sequens a loco (syllaba, elementum): positio; -1 nulla */
interior s32
nota_sequens (
    constans Compositor* co,
                    s32  syllaba,
                    s32  elementum)
{
      constans GabcSyllaba* sy;
    constans GabcElementum* el;
                       s32  s;
                       s32  e;

    per (s = syllaba; s < (s32)co->cantus->numerus_syllabarum; s++)
    {
        sy = &co->cantus->syllabae[s];
        per (e = s == syllaba ? elementum : ZEPHYRUM;
             e < (s32)sy->numerus_elementorum; e++)
        {
            el = &sy->elementa[e];
            si (   el->genus == GABC_EL_NEUMA && el->numerus > ZEPHYRUM
                && el->prima < co->cantus->numerus_notarum)
            {
                redde (s32)co->cantus->notae[el->prima].positio;
            }
        }
    }
    redde -I;
}

/* sequitur clavis (elementum primum non spatium/fractio/opacum)? */
interior b32
clavis_sequitur (
    constans Compositor* co,
                    s32  syllaba,
                    s32  elementum)
{
      constans GabcSyllaba* sy;
    constans GabcElementum* el;
                       s32  s;
                       s32  e;

    per (s = syllaba; s < (s32)co->cantus->numerus_syllabarum; s++)
    {
        sy = &co->cantus->syllabae[s];
        per (e = s == syllaba ? elementum : ZEPHYRUM;
             e < (s32)sy->numerus_elementorum; e++)
        {
            el = &sy->elementa[e];
            si (   el->genus == GABC_EL_SPATIUM
                || el->genus == GABC_EL_FRACTIO
                || el->genus == GABC_EL_SIGNUM_OPACUM)
            {
                perge;
            }
            redde el->genus == GABC_EL_CLAVIS && !el->clavis_secunda;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Textus: vocales, mensura, emissio
 * ================================================== */

/* basis vocalis minuscula; 'x' = monophthongus (æ œ); 0 = non
 * vocalis */
interior character
vocalis_basis (
    s32 runa)
{
    si (runa >= 'A' && runa <= 'Z')
    {
        runa += 'a' - 'A';
    }
    commutatio (runa)
    {
        casus 'a': casus 'e': casus 'i': casus 'o': casus 'u':
        casus 'y':
            redde (character)runa;
        ordinarius:
            frange;
    }
    si ((runa >= 0xC0 && runa <= 0xC5) || (runa >= 0xE0 && runa <= 0xE5)
        || (runa >= 0x100 && runa <= 0x105))
    {
        redde 'a';
    }
    si ((runa >= 0xC8 && runa <= 0xCB) || (runa >= 0xE8 && runa <= 0xEB)
        || (runa >= 0x112 && runa <= 0x11B))
    {
        redde 'e';
    }
    si ((runa >= 0xCC && runa <= 0xCF) || (runa >= 0xEC && runa <= 0xEF)
        || (runa >= 0x12A && runa <= 0x131))
    {
        redde 'i';
    }
    si (   (runa >= 0xD2 && runa <= 0xD6) || runa == 0xD8
        || (runa >= 0xF2 && runa <= 0xF6) || runa == 0xF8
        || (runa >= 0x14C && runa <= 0x151))
    {
        redde 'o';
    }
    si ((runa >= 0xD9 && runa <= 0xDC) || (runa >= 0xF9 && runa <= 0xFC)
        || (runa >= 0x16A && runa <= 0x173))
    {
        redde 'u';
    }
    si (runa == 0xDD || runa == 0xFD || runa == 0xFF)
    {
        redde 'y';
    }
    si (runa == 0xC6 || runa == 0xE6 || runa == 0x152 || runa == 0x153
        || runa == 0x1FC || runa == 0x1FD || runa == 0x1E2
        || runa == 0x1E3)
    {
        redde 'x';
    }
    redde ZEPHYRUM;
}

interior character
littera_minuscula (
    s32 runa)
{
    si (runa >= 'A' && runa <= 'Z')
    {
        redde (character)(runa + ('a' - 'A'));
    }
    redde runa < 0x80 ? (character)runa : ZEPHYRUM;
}

/* cursus textus cantati qui initialem amittit (primus non vacuus) */
interior b32
cursus_initialis (
    constans Compositor* co,
                    s32  syllaba,
                    i32  cursus)
{
    constans GabcSyllaba* sy;
                     i32  r;

    si (syllaba != co->initialis_syllaba)
    {
        redde FALSUM;
    }
    sy = &co->cantus->syllabae[syllaba];
    per (r = ZEPHYRUM; r < sy->numerus_textuum; r++)
    {
        si (   sy->textus[r].genus == GABC_TEXTUS_CANTATUS
            && sy->textus[r].textus.mensura > ZEPHYRUM)
        {
            redde r == cursus;
        }
    }
    redde FALSUM;
}

/* chorda post runam primam (et notas compositas eius) */
interior chorda
post_runam_primam (
    chorda ch)
{
    constans i8* p;
    constans i8* q;
    constans i8* finis;
         chorda  reliqua;

    p      = ch.datum;
    finis  = ch.datum + ch.mensura;
    utf8_decodere(&p, finis);
    q = p;
    dum (q < finis && nota_mixta(utf8_decodere(&q, finis)))
    {
        p = q;
    }
    reliqua.datum    = ch.datum + (p - ch.datum);
    reliqua.mensura  = (i32)(finis - p);
    redde reliqua;
}

/* textus cantatus syllabae visibilis cum centro vocalis */
interior vacuum
textum_metiri (
    constans Compositor* co,
                    s32  syllaba,
          TextusMensura* tm)
{
    constans GabcSyllaba* sy;
     constans GabcTextus* t;
               character  bases[RUNAE_MAXIMAE];
               character  litterae[RUNAE_MAXIMAE];
                     b32  elisae[RUNAE_MAXIMAE];
                     s32  offseta[RUNAE_MAXIMAE];
             constans i8* p;
             constans i8* finis;
             constans i8* initium;
                     s32  n;
                     s32  i;
                     s32  j;
                     s32  a;
                     s32  longitudo;
                     s32  runa;
                     s32  offset_cursus;
                     i32  r;
                  chorda  ch;

    sy             = &co->cantus->syllabae[syllaba];
    n              = ZEPHYRUM;
    offset_cursus  = ZEPHYRUM;
    per (r = ZEPHYRUM; r < sy->numerus_textuum; r++)
    {
        t = &sy->textus[r];
        si (t->genus != GABC_TEXTUS_CANTATUS)
        {
            perge;
        }
        ch       = cursus_initialis(co, syllaba, r)
                 ? post_runam_primam(t->textus) : t->textus;
        initium  = t->textus.datum;
        p        = ch.datum;
        finis    = ch.datum + ch.mensura;
        dum (p < finis && n < RUNAE_MAXIMAE)
        {
            constans i8* ante = p;

            runa = utf8_decodere(&p, finis);
            si (nota_mixta(runa))
            {
                perge;
            }
            bases[n]     = vocalis_basis(runa);
            litterae[n]  = littera_minuscula(runa);
            elisae[n]    = (t->stilus & GABC_STILUS_ELISIO) != ZEPHYRUM;
            offseta[n]   = offset_cursus + (s32)(ante - initium);
            n++;
        }
        offset_cursus += (s32)t->textus.mensura;
    }
    tm->adest     = n > ZEPHYRUM;
    tm->latitudo  = n * co->cw;
    tm->centrum   = tm->latitudo / II;
    si (n == ZEPHYRUM)
    {
        redde;
    }
    si (sy->vocalis_initium >= ZEPHYRUM)
    {
        a = ZEPHYRUM;
        dum (a < n && offseta[a] < sy->vocalis_initium)
        {
            a++;
        }
        j = a;
        dum (j < n && offseta[j] < sy->vocalis_finis)
        {
            j++;
        }
        si (a >= n)
        {
            a = ZEPHYRUM;
            j = I;
        }
        longitudo    = maximum(j - a, I);
        tm->centrum  = co->cw * a + co->cw * longitudo / II;
        redde;
    }
    i = ZEPHYRUM;
    dum (i < n && (bases[i] == ZEPHYRUM || elisae[i]))
    {
        i++;
    }
    si (i >= n)
    {
        redde;
    }
    j = i;
    dum (j < n && bases[j] != ZEPHYRUM && !elisae[j])
    {
        j++;
    }
    a          = i;
    longitudo  = j - i;
    si (   longitudo >= II && bases[a] == 'u' && a >= I
        && (litterae[a - I] == 'q' || litterae[a - I] == 'g'))
    {
        a++;
        longitudo--;
    }
    alioquin si (longitudo >= II && a == ZEPHYRUM && bases[a] == 'i')
    {
        a++;
        longitudo--;
    }
    si (   longitudo >= II
        && ((bases[a] == 'a'
        && (bases[a + I] == 'e' || bases[a + I] == 'u'))
            || (bases[a] == 'o' && bases[a + I] == 'e')
            || (bases[a] == 'e' && bases[a + I] == 'u')))
    {
        longitudo = II;
    }
    alioquin
    {
        longitudo = I;
    }
    tm->centrum = co->cw * a + co->cw * longitudo / II;
}

/* rubrica: signum c aut solum * † ℟ ℣ + et spatia (et . , : ut
 * "℣." - P4) */
interior b32
rubrica_est (
    constans GabcTextus* t)
{
    constans i8* p;
    constans i8* finis;
            s32  runa;
            b32  signum;

    si (t->stilus & GABC_STILUS_COLORATUS)
    {
        redde VERUM;
    }
    p       = t->textus.datum;
    finis   = p + t->textus.mensura;
    signum  = FALSUM;
    dum (p < finis)
    {
        runa = utf8_decodere(&p, finis);
        si (   runa == '*' || runa == '+' || runa == 0x2020
            || runa == 0x211F
            || runa == 0x2123)
        {
            signum = VERUM;
        }
        alioquin si (   runa != ' ' && runa != '.' && runa != ','
                     && runa != ':')
        {
            redde FALSUM;
        }
    }
    redde signum;
}

/* cursus textus cantati emittuntur ab x (ordo postea); latitudo */
interior s32
textum_emittere_syllabae (
    Compositor* co,
           s32  syllaba,
           s32  x)
{
    constans GabcSyllaba* sy;
     constans GabcTextus* t;
                  chorda  ch;
                     s32  x0;
                     i32  r;

    sy = &co->cantus->syllabae[syllaba];
    x0 = x;
    per (r = ZEPHYRUM; r < sy->numerus_textuum; r++)
    {
        t = &sy->textus[r];
        si (t->genus != GABC_TEXTUS_CANTATUS)
        {
            perge;
        }
        ch = cursus_initialis(co, syllaba, r)
            ? post_runam_primam(t->textus) : t->textus;
        si (ch.mensura == ZEPHYRUM)
        {
            perge;
        }
        textum_emittere(co->systema, ch, co->forma->fons_textus, x,
            Y_CANTATUS, rubrica_est(t) ? GABC_TINCTURA_RUBRICA
                                       : GABC_TINCTURA_ATRAMENTUM,
            syllaba);
        x += runae_numerare(ch) * co->cw;
    }
    redde x - x0;
}

interior vacuum
supra_emittere (
    Compositor* co,
           s32  syllaba,
           s32  x)
{
    constans GabcSyllaba* sy;
                     i32  r;

    sy = &co->cantus->syllabae[syllaba];
    per (r = ZEPHYRUM; r < sy->numerus_textuum; r++)
    {
        si (   sy->textus[r].genus == GABC_TEXTUS_SUPRA
            && sy->textus[r].textus.mensura > ZEPHYRUM)
        {
            textum_emittere(co->systema, sy->textus[r].textus,
                co->forma->fons_textus, x, Y_SUPRA,
                GABC_TINCTURA_ATRAMENTUM, syllaba);
            x += runae_numerare(sy->textus[r].textus) * co->cw;
        }
    }
}


/* ==================================================
 * Capita et neumae
 * ================================================== */

/* pars capitis ex formis notae */
interior NeumaePars
caput_notae (
    constans GabcNota* nota,
                  s32* parametrum)
{
    i32 f;

    f            = nota->formae;
    *parametrum  = ZEPHYRUM;
    si (f & GABC_FORMA_INCLINATUM)
    {
        redde (f & GABC_FORMA_LIQUESCENS) ? NEUMAE_INCLINATUM_DEMINUTUM
                                         : NEUMAE_INCLINATUM;
    }
    si (f & (GABC_FORMA_LIQUESCENS | GABC_FORMA_INITIO_DEBILIS))
    {
        redde NEUMAE_DEMINUTUM;
    }
    si (f & GABC_FORMA_AUCTA_ASCENDENS)
    {
        redde NEUMAE_AUCTUM_ASCENDENS;
    }
    si (f & GABC_FORMA_AUCTA_DESCENDENS)
    {
        redde NEUMAE_AUCTUM_DESCENDENS;
    }
    si (f & (GABC_FORMA_VIRGA | GABC_FORMA_VIRGA_REVERSA))
    {
        *parametrum = (nota->positio % II) ? III : II;
        redde (f & GABC_FORMA_VIRGA) ? NEUMAE_VIRGA
                                     : NEUMAE_VIRGA_REVERSA;
    }
    si (f & GABC_FORMA_STROPHA)
    {
        redde NEUMAE_STROPHA;
    }
    si (f & (GABC_FORMA_ORISCUS | GABC_FORMA_ORISCUS_SCAPUS))
    {
        redde NEUMAE_ORISCUS;
    }
    si (f & (GABC_FORMA_QUILISMA | GABC_FORMA_QUILISMA_QUADRATUM))
    {
        redde NEUMAE_QUILISMA;
    }
    si (f & GABC_FORMA_LINEA_PUNCTUM)
    {
        redde NEUMAE_LINEA_PUNCTUM;
    }
    si (f & GABC_FORMA_CAVUM)
    {
        redde NEUMAE_CAVUM;
    }
    si (f & GABC_FORMA_LINEA)
    {
        redde NEUMAE_LINEA;
    }
    redde NEUMAE_PUNCTUM;
}

/* capsa capitis (capsa interna) et lineae additae */
interior vacuum
capsam_et_additas (
    Compositor* co,
           Xar* ad,
           s32  nota,
           s32  x,
           s32  latitudo,
           s32  summum,
           s32  imum,
           s32  syllaba,
           s32  elementum)
{
    GabcPictum* p;
           s32  positio;
           s32  l;

    p = emittere(ad, PICTUM_CAPSA, x, summum, syllaba, elementum, nota);
    si (p)
    {
        p->latitudo = (i32)latitudo;
        p->altitudo = (i32)(imum - summum);
    }
    positio = (s32)co->cantus->notae[nota].positio;
    per (l = I; l >= positio && l < III; l -= II)
    {
        rectangulum_emittere(ad, x - co->q,
            y_positionis(co, l) - co->k / II, latitudo + II * co->q,
            co->k,
            GABC_TINCTURA_SYSTEMA, syllaba, elementum, nota);
    }
    per (l = co->summa + II; l <= positio; l += II)
    {
        rectangulum_emittere(ad, x - co->q,
            y_positionis(co, l) - co->k / II, latitudo + II * co->q,
            co->k,
            GABC_TINCTURA_SYSTEMA, syllaba, elementum, nota);
    }
}

/* caput notae ad x; capsa in co->capita[index] */
interior s32
caput_ponere (
    Compositor* co,
           Xar* ad,
           s32  nota,
           s32  index,
           s32  x,
           s32  syllaba,
           s32  elementum)
{
    constans GabcNota* n;
        NeumaeMensura  m;
        NeumaeMensura  capsa;
           NeumaePars  pars;
                  s32  parametrum;
                  s32  y;
                  s32  summum;

    n     = &co->cantus->notae[nota];
    pars  = caput_notae(n, &parametrum);
    y     = y_positionis(co, (s32)n->positio);
    mensuram_capere(co, pars, parametrum, &m);
    partem_emittere(ad, pars, parametrum, x, y, syllaba, elementum,
        nota);
    si (n->formae & GABC_FORMA_ORISCUS_SCAPUS)
    {
        rectangulum_emittere(ad, x, y, co->k,
            (n->positio % II ? III : II) * co->h + I,
            GABC_TINCTURA_ATRAMENTUM, syllaba, elementum, nota);
    }
    /* capsa: caput solum (virga, auctum: punctum) */
    si (   pars == NEUMAE_VIRGA || pars == NEUMAE_VIRGA_REVERSA
        || pars == NEUMAE_AUCTUM_ASCENDENS
        || pars == NEUMAE_AUCTUM_DESCENDENS)
    {
        mensuram_capere(co, NEUMAE_PUNCTUM, ZEPHYRUM, &capsa);
    }
    alioquin
    {
        capsa = m;
    }
    summum = y - capsa.origo_y;
    capsam_et_additas(co, ad, nota, x, (s32)m.capitis_latitudo, summum,
        summum + (s32)capsa.altitudo, syllaba, elementum);
    co->capita[index].x                 = x;
    co->capita[index].latitudo          = (s32)m.capitis_latitudo;
    co->capita[index].summum            = summum;
    co->capita[index].imum              = summum + (s32)capsa.altitudo;
    co->capita[index].ultimum_segmenti  = FALSUM;
    redde (s32)m.capitis_latitudo;
}

/* linea connectens verticalis a centro ad centrum */
interior vacuum
lineam_connectere (
    Compositor* co,
           Xar* ad,
           s32  x,
           s32  y_prima,
           s32  y_secunda,
           s32  syllaba,
           s32  elementum,
           s32  nota)
{
    si (y_prima != y_secunda)
    {
        rectangulum_emittere(ad, x, minimum(y_prima, y_secunda), co->k,
            maximum(y_prima, y_secunda) - minimum(y_prima, y_secunda)
                + I,
            GABC_TINCTURA_ATRAMENTUM, syllaba, elementum, nota);
    }
}

/* caulis sinister a y deorsum ultra notam inferiorem */
interior vacuum
caulem_ponere (
    Compositor* co,
           Xar* ad,
           s32  x,
           s32  y,
           s32  positio_infima,
           s32  syllaba,
           s32  elementum,
           s32  nota)
{
    s32 finis;

    /* s/II + s/IV infra centrum notae inferioris: ultra caput, numquam
     * in linea (P4: regula virgae caulem dimidio spatio longiorem
     * dabat - visum in Dies irae, Ingrediente) */
    finis = y_positionis(co, positio_infima) + co->h + co->q;
    rectangulum_emittere(ad, x, y, co->k, finis - y + I,
        GABC_TINCTURA_ATRAMENTUM, syllaba, elementum, nota);
}

interior s32
y_notae (
    constans Compositor* co,
                    s32  nota)
{
    redde y_positionis(co, (s32)co->cantus->notae[nota].positio);
}

/* segmentum [a, a+n) notarum (indices in cantu) figurae datae ab x;
 * capita in co->capita[a - prima ...]; latitudo; libramentum */
interior s32
segmentum_componere (
     Compositor* co,
            Xar* ad,
            s32  syllaba,
            s32  elementum,
            s32  prima,
            s32  a,
            s32  n,
     GabcFigura  figura,
            s32  x0,
            s32* libramentum)
{
    constans GabcNota* notae;
                  s32  i;
                  s32  c0;
                  s32  c1;
                  s32  c2;
                  s32  x;
                  s32  w;
                  s32  d;
                  s32  x_finis;
                  s32  ii;
        NeumaeMensura  m;

    notae         = co->cantus->notae;
    ii            = a - prima;
    *libramentum  = -I;
    w             = ZEPHYRUM;

    commutatio (figura)
    {
        casus GABC_FIGURA_PES:
        casus GABC_FIGURA_PES_QUILISMATICUS:
        casus GABC_FIGURA_EPIPHONUS:
            c0 = caput_ponere(co, ad, a, ii, x0, syllaba, elementum);
            /* superius SUPRA ad marginem dextrum */
            mensuram_capere(co, caput_notae(&notae[a + I], &d), d, &m);
            caput_ponere(co, ad, a + I, ii + I,
                x0 + c0 - (s32)m.capitis_latitudo, syllaba, elementum);
            lineam_connectere(co, ad, x0 + c0 - co->k, y_notae(co, a),
                y_notae(co, a + I), syllaba, elementum, a + I);
            w           = c0;
            *libramentum  = c0 / II;
            frange;

        casus GABC_FIGURA_PES_QUADRATUS:
        casus GABC_FIGURA_PES_QUASSUS:
            c0 = caput_ponere(co, ad, a, ii, x0, syllaba, elementum);
            c1 = caput_ponere(co, ad, a + I, ii + I, x0 + c0, syllaba,
                elementum);
            lineam_connectere(co, ad, x0 + c0 - co->k, y_notae(co, a),
                y_notae(co, a + I), syllaba, elementum, a + I);
            w           = c0 + c1;
            *libramentum  = w / II;
            frange;

        casus GABC_FIGURA_CLIVIS:
        casus GABC_FIGURA_CEPHALICUS:
            c0 = caput_ponere(co, ad, a, ii, x0, syllaba, elementum);
            si (!(notae[a].formae & (GABC_FORMA_ORISCUS
                | GABC_FORMA_FUSIO
                    | GABC_FORMA_ORISCUS_SCAPUS)))
            {
                caulem_ponere(co, ad, x0, y_notae(co, a),
                    (s32)notae[a + I].positio, syllaba, elementum, a);
            }
            c1 = caput_ponere(co, ad, a + I, ii + I, x0 + c0, syllaba,
                elementum);
            lineam_connectere(co, ad, x0 + c0 - co->k, y_notae(co, a),
                y_notae(co, a + I), syllaba, elementum, a + I);
            w           = c0 + c1;
            *libramentum  = w / II;
            frange;

        casus GABC_FIGURA_TORCULUS:
        casus GABC_FIGURA_TORCULUS_QUILISMATICUS:
        casus GABC_FIGURA_PRESSUS:
        casus GABC_FIGURA_ANCUS:
            x = x0;
            per (i = ZEPHYRUM; i < n; i++)
            {
                si (   figura == GABC_FIGURA_ANCUS && i == II
                    && !(notae[a + i].formae & GABC_FORMA_INCLINATUM))
                {
                    /* ancus: deminuta sub secunda, eadem columna */
                    mensuram_capere(co, caput_notae(&notae[a + i], &d),
                        d,
                        &m);
                    caput_ponere(co, ad, a + i, ii + i,
                        x - (s32)m.capitis_latitudo, syllaba,
                        elementum);
                    lineam_connectere(co, ad, x - co->k,
                        y_notae(co, a + i - I), y_notae(co, a + i),
                        syllaba,
                        elementum, a + i);
                    perge;
                }
                c1 = caput_ponere(co, ad, a + i, ii + i, x, syllaba,
                    elementum);
                si (i == ZEPHYRUM && figura == GABC_FIGURA_ANCUS)
                {
                    caulem_ponere(co, ad, x0, y_notae(co, a),
                        (s32)notae[a + I].positio, syllaba, elementum,
                        a);
                }
                si (i > ZEPHYRUM)
                {
                    lineam_connectere(co, ad, x - co->k,
                        y_notae(co, a + i - I), y_notae(co, a + i),
                        syllaba,
                        elementum, a + i);
                }
                x += c1;
            }
            w           = x - x0;
            *libramentum  = co->capita[ii].latitudo / II;
            frange;

        casus GABC_FIGURA_PORRECTUS:
        casus GABC_FIGURA_PORRECTUS_FLEXUS:
        casus GABC_FIGURA_TORCULUS_RESUPINUS:
        casus GABC_FIGURA_TORCULUS_RESUPINUS_FLEXUS:
            x = x0;
            i = ZEPHYRUM;
            si (   figura == GABC_FIGURA_TORCULUS_RESUPINUS
                || figura == GABC_FIGURA_TORCULUS_RESUPINUS_FLEXUS)
            {
                c0 = caput_ponere(co, ad, a, ii, x, syllaba, elementum);
                lineam_connectere(co, ad, x + c0 - co->k, y_notae(co,
                    a),
                    y_notae(co, a + I), syllaba, elementum, a + I);
                x += c0;
                i = I;
            }
            d = (s32)notae[a + i].positio - (s32)notae[a + i
                + I].positio;
            si (d < I || d > V)
            {
                /* differentia magna: clivis + caput (nota tertia) */
                c0 = caput_ponere(co, ad, a + i, ii + i, x, syllaba,
                    elementum);
                si (i == ZEPHYRUM)
                {
                    caulem_ponere(co, ad, x, y_notae(co, a + i),
                        (s32)notae[a + i + I].positio, syllaba,
                        elementum,
                        a + i);
                }
                c1 = caput_ponere(co, ad, a + i + I, ii + i + I, x + c0,
                    syllaba, elementum);
                lineam_connectere(co, ad, x + c0 - co->k,
                    y_notae(co, a + i), y_notae(co, a + i + I), syllaba,
                    elementum, a + i + I);
                x += c0 + c1;
                c2 = caput_ponere(co, ad, a + i + II, ii + i + II, x,
                    syllaba, elementum);
                lineam_connectere(co, ad, x - co->k, y_notae(co, a + i
                    + I),
                    y_notae(co, a + i + II), syllaba, elementum,
                    a + i + II);
                x += c2;
            }
            alioquin
            {
                partem_emittere(ad, NEUMAE_PORRECTUS, d, x,
                    y_notae(co, a + i), syllaba, elementum, a + i);
                si (i == ZEPHYRUM)
                {
                    caulem_ponere(co, ad, x, y_notae(co, a),
                        (s32)notae[a + I].positio, syllaba, elementum,
                        a);
                }
                /* capsae duarum notarum lineae obliquae: quadrata
                 * imaginaria in initio et fine */
                mensuram_capere(co, NEUMAE_PUNCTUM, ZEPHYRUM, &m);
                capsam_et_additas(co, ad, a + i, x, co->s,
                    y_notae(co, a + i) - m.origo_y,
                    y_notae(co, a + i) - m.origo_y + (s32)m.altitudo,
                    syllaba, elementum);
                capsam_et_additas(co, ad, a + i + I, x + co->s, co->s,
                    y_notae(co, a + i + I) - m.origo_y,
                    y_notae(co, a + i + I) - m.origo_y
                        + (s32)m.altitudo,
                    syllaba, elementum);
                co->capita[ii + i].x         = x;
                co->capita[ii + i].latitudo  = co->s;
                co->capita[ii + i].summum            = y_notae(co, a
                    + i)
                                                     - m.origo_y;
                co->capita[ii + i].imum              =
                    co->capita[ii + i].summum + (s32)m.altitudo;
                co->capita[ii + i].ultimum_segmenti = FALSUM;
                co->capita[ii + i + I] = co->capita[ii
                    + i];
                co->capita[ii + i + I].x = x + co->s;
                co->capita[ii + i + I].summum        =
                    y_notae(co, a + i + I) - m.origo_y;
                co->capita[ii + i + I].imum          =
                    co->capita[ii + i + I].summum + (s32)m.altitudo;
                /* tertia SUPRA finem obliquae */
                mensuram_capere(co, caput_notae(&notae[a + i + II], &d),
                    d,
                    &m);
                caput_ponere(co, ad, a + i + II, ii + i + II,
                    x + II * co->s - (s32)m.capitis_latitudo, syllaba,
                    elementum);
                lineam_connectere(co, ad, x + II * co->s - co->k,
                    y_notae(co, a + i + I), y_notae(co, a + i + II),
                    syllaba,
                    elementum, a + i + II);
                x += II * co->s;
            }
            /* flexus: nota ultima IUXTA */
            si (i + III < n)
            {
                c1 = caput_ponere(co, ad, a + i + III, ii + i + III, x,
                    syllaba, elementum);
                lineam_connectere(co, ad, x - co->k, y_notae(co, a + i
                    + II),
                    y_notae(co, a + i + III), syllaba, elementum,
                    a + i + III);
                x += c1;
            }
            w           = x - x0;
            *libramentum  = i == ZEPHYRUM ? co->s / II
                                        : co->capita[ii].latitudo / II;
            frange;

        casus GABC_FIGURA_SCANDICUS:
        casus GABC_FIGURA_SCANDICUS_QUILISMATICUS:
        casus GABC_FIGURA_SCANDICUS_FLEXUS:
        casus GABC_FIGURA_SALICUS:
        casus GABC_FIGURA_SALICUS_FLEXUS:
            c0 = caput_ponere(co, ad, a, ii, x0, syllaba, elementum);
            x  = x0 + c0;
            c1 = caput_ponere(co, ad, a + I, ii + I, x, syllaba,
                elementum);
            si (   figura == GABC_FIGURA_SALICUS
                || figura == GABC_FIGURA_SALICUS_FLEXUS)
            {
                /* pes quassus: superius IUXTA */
                c2 = caput_ponere(co, ad, a + II, ii + II, x + c1,
                    syllaba,
                    elementum);
                lineam_connectere(co, ad, x + c1 - co->k,
                    y_notae(co, a + I), y_notae(co, a + II), syllaba,
                    elementum, a + II);
                x += c1 + c2;
            }
            alioquin
            {
                mensuram_capere(co, caput_notae(&notae[a + II], &d), d,
                    &m);
                caput_ponere(co, ad, a + II, ii + II,
                    x + c1 - (s32)m.capitis_latitudo, syllaba,
                    elementum);
                lineam_connectere(co, ad, x + c1 - co->k,
                    y_notae(co, a + I), y_notae(co, a + II), syllaba,
                    elementum, a + II);
                x += c1;
            }
            si (n > III)
            {
                c2 = caput_ponere(co, ad, a + III, ii + III, x, syllaba,
                    elementum);
                lineam_connectere(co, ad, x - co->k, y_notae(co, a
                    + II),
                    y_notae(co, a + III), syllaba, elementum, a + III);
                x += c2;
            }
            w           = x - x0;
            *libramentum  = c0 / II;
            frange;

        casus GABC_FIGURA_CLIMACUS:
        casus GABC_FIGURA_PES_SUBBIPUNCTIS:
        casus GABC_FIGURA_PES_SUBTRIPUNCTIS:
            i = ZEPHYRUM;
            si (figura == GABC_FIGURA_CLIMACUS)
            {
                c0 = caput_ponere(co, ad, a, ii, x0, syllaba,
                    elementum);
                i = I;
            }
            alioquin
            {
                c0 = caput_ponere(co, ad, a, ii, x0, syllaba,
                    elementum);
                mensuram_capere(co, caput_notae(&notae[a + I], &d), d,
                    &m);
                caput_ponere(co, ad, a + I, ii + I,
                    x0 + c0 - (s32)m.capitis_latitudo, syllaba,
                    elementum);
                lineam_connectere(co, ad, x0 + c0 - co->k, y_notae(co,
                    a),
                    y_notae(co, a + I), syllaba, elementum, a + I);
                i = II;
            }
            x  = x0 + c0;
            c1 = ZEPHYRUM;
            per (; i < n; i++)
            {
                c1 = caput_ponere(co, ad, a + i, ii + i, x, syllaba,
                    elementum);
                x += co->s * III / IV;
            }
            w           = x - co->s * III / IV + c1 - x0;
            *libramentum  = c0 / II;
            frange;

        casus GABC_FIGURA_BIVIRGA:
        casus GABC_FIGURA_TRIVIRGA:
        casus GABC_FIGURA_DISTROPHA:
        casus GABC_FIGURA_TRISTROPHA:
            x = x0;
            per (i = ZEPHYRUM; i < n; i++)
            {
                si (i > ZEPHYRUM)
                {
                    x += co->h;
                }
                x += caput_ponere(co, ad, a + i, ii + i, x, syllaba,
                    elementum);
            }
            w           = x - x0;
            *libramentum  = co->capita[ii].latitudo / II;
            frange;

        ordinarius:
            /* nota una (aut figura sine regula: singulae) */
            x = x0;
            per (i = ZEPHYRUM; i < n; i++)
            {
                si (i > ZEPHYRUM)
                {
                    x += co->q;
                }
                x += caput_ponere(co, ad, a + i, ii + i, x, syllaba,
                    elementum);
            }
            w           = x - x0;
            *libramentum  = co->capita[ii].latitudo / II;
            frange;
    }
    x_finis = x0 + w;
    per (i = ZEPHYRUM; i < n; i++)
    {
        co->capita[ii + i].finis_segmenti = x_finis;
    }
    co->capita[ii + n - I].ultimum_segmenti = VERUM;
    redde w;
}

/* signa notarum elementi: mora, episema, ictus, r1-r8 */
interior vacuum
signa_ponere (
    Compositor* co,
           Xar* ad,
           s32  syllaba,
           s32  elementum,
           s32  prima,
           s32  numerus)
{
    constans GabcNota* n;
         CaputPositum* cp;
        NeumaeMensura  m;
           NeumaePars  pars;
                  s32  i;
                  s32  j;
                  s32  x;
                  s32  y;
                  s32  x0;
                  s32  x1;
                  s32  extremum;
                  b32  infra;
                  i32  signum;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        n   = &co->cantus->notae[prima + i];
        cp  = &co->capita[i];
        si (n->mora > ZEPHYRUM)
        {
            x = (cp->ultimum_segmenti ? cp->finis_segmenti
                                      : cp->x + cp->latitudo) + co->q;
            y = y_positionis(co, (s32)n->positio);
            si (n->positio % II)
            {
                y += n->mora_locus == ZEPHYRUM ? co->h : -co->h;
            }
            partem_emittere(ad, NEUMAE_PUNCTUM_MORAE, ZEPHYRUM, x, y,
                syllaba, elementum, prima + i);
            si (n->mora > I)
            {
                partem_emittere(ad, NEUMAE_PUNCTUM_MORAE, ZEPHYRUM,
                    x + co->h, y, syllaba, elementum, prima + i);
            }
        }
        si (n->ictus)
        {
            x = cp->x + cp->latitudo / II - co->k / II;
            y = n->ictus_locus == I
                ? cp->summum - co->q - co->h + co->h / II
                : cp->imum + co->q + co->h / II;
            partem_emittere(ad, NEUMAE_ICTUS, ZEPHYRUM, x, y, syllaba,
                elementum, prima + i);
        }
        per (signum = ZEPHYRUM; signum < VI; signum++)
        {
            si (!(n->signa & (I << signum)))
            {
                perge;
            }
            commutatio (signum)
            {
                casus ZEPHYRUM: pars = NEUMAE_ACCENTUS; frange;
                casus I: pars = NEUMAE_ACCENTUS_REVERSUS; frange;
                casus II: pars = NEUMAE_CIRCULUS; frange;
                casus III: pars = NEUMAE_SEMICIRCULUS; frange;
                casus IV: pars = NEUMAE_SEMICIRCULUS_REVERSUS; frange;
                ordinarius:
                    pars = n->alteratio < ZEPHYRUM ? NEUMAE_BEMOLLE
                         : n->alteratio > ZEPHYRUM ? NEUMAE_DIESIS
                                                  : NEUMAE_NATURALE;
                    frange;
            }
            mensuram_capere(co, pars, ZEPHYRUM, &m);
            partem_emittere(ad, pars, ZEPHYRUM,
                cp->x + cp->latitudo / II - (s32)m.latitudo / II,
                cp->summum - co->q - (s32)m.altitudo + m.origo_y,
                syllaba,
                elementum, prima + i);
        }
    }

    /* episemata: cursus notarum contiguarum eodem latere */
    i = ZEPHYRUM;
    dum (i < numerus)
    {
        n = &co->cantus->notae[prima + i];
        si (!(n->episema & GABC_EPISEMA_ADEST))
        {
            i++;
            perge;
        }
        infra     = (n->episema & GABC_EPISEMA_NUMERUS(0)) != ZEPHYRUM;
        x0        = co->capita[i].x;
        x1        = co->capita[i].x + co->capita[i].latitudo;
        extremum  = infra ? co->capita[i].imum : co->capita[i].summum;
        j         = i + I;
        dum (   j < numerus
             && (co->cantus->notae[prima
                + j].episema & GABC_EPISEMA_ADEST)
             && ((co->cantus->notae[prima + j].episema
                 & GABC_EPISEMA_NUMERUS(0)) != ZEPHYRUM) == infra
             && !(co->cantus->notae[prima + j].episema
                 & GABC_EPISEMA_NUMERUS(2)))
        {
            x0        = minimum(x0, co->capita[j].x);
            x1        = maximum(x1, co->capita[j].x
                                    + co->capita[j].latitudo);
            extremum  = infra ? maximum(extremum, co->capita[j].imum)
                              : minimum(extremum, co->capita[j].summum);
            j++;
        }
        partem_emittere(ad, NEUMAE_EPISEMA,
            minimum(maximum(x1 - x0, I), CCLVI), x0,
            infra ? extremum + co->q + co->k / II
                  : extremum - co->q - co->k + co->k / II,
            syllaba, elementum, prima + i);
        i = j;
    }
}

/* neuma: segmenta longissima nota a sinistra; latitudo */
interior s32
neumam_componere (
    Compositor* co,
           Xar* ad,
           s32  syllaba,
           s32  elementum,
           s32  x0,
           s32* libramentum)
{
    constans GabcElementum* el;
             GabcElementum  pars;
                GabcFigura  figura;
                       s32  prima;
                       s32  numerus;
                       s32  i;
                       s32  longitudo;
                       s32  x;
                       s32  al;

    el       = &co->cantus->syllabae[syllaba].elementa[elementum];
    prima    = (s32)el->prima;
    numerus  = minimum((s32)el->numerus, CCLVI);
    si (prima + numerus > (s32)co->cantus->numerus_notarum)
    {
        redde ZEPHYRUM;
    }
    x             = x0;
    i             = ZEPHYRUM;
    *libramentum  = -I;
    dum (i < numerus)
    {
        figura = GABC_FIGURA_IGNOTA;
        per (longitudo = numerus - i; longitudo >= II; longitudo--)
        {
            pars          = *el;
            pars.prima    = (i32)(prima + i);
            pars.numerus  = (i32)longitudo;
            figura        = gabc_figura(co->cantus, &pars);
            si (figura != GABC_FIGURA_IGNOTA)
            {
                frange;
            }
        }
        si (longitudo < II)
        {
            longitudo  = I;
            figura     = GABC_FIGURA_IGNOTA;
        }
        si (i > ZEPHYRUM)
        {
            x += co->cantus->notae[prima + i - I].positio
                 == co->cantus->notae[prima
                     + i].positio ? co->h : co->q;
        }
        x += segmentum_componere(co, ad, syllaba, elementum, prima,
            prima + i, longitudo, figura, x, &al);
        si (*libramentum < ZEPHYRUM)
        {
            *libramentum = al;
        }
        i += longitudo;
    }
    signa_ponere(co, ad, syllaba, elementum, prima, numerus);
    redde x - x0;
}

/* clavis cum bemolle; latitudo */
interior s32
clavem_ponere (
               Compositor* co,
                      Xar* ad,
    constans ClavisStatus* cs,
                      s32  x,
                      s32  syllaba,
                      s32  elementum)
{
    NeumaeMensura m;
       NeumaePars pars;
              s32 positio;
              s32 b;
              s32 w;

    pars     = cs->clavis == 'f' ? NEUMAE_CLAVIS_F : NEUMAE_CLAVIS_C;
    positio  = II * (s32)cs->linea + I;
    mensuram_capere(co, pars, ZEPHYRUM, &m);
    partem_emittere(ad, pars, ZEPHYRUM, x, y_positionis(co, positio),
        syllaba, elementum, -I);
    w = (s32)m.latitudo;
    si (cs->bemolle)
    {
        b = pars == NEUMAE_CLAVIS_F ? positio + III : positio - I;
        si (b < III || b > co->summa)
        {
            b = pars == NEUMAE_CLAVIS_F ? positio - IV : positio + VI;
        }
        mensuram_capere(co, NEUMAE_BEMOLLE, ZEPHYRUM, &m);
        partem_emittere(ad, NEUMAE_BEMOLLE, ZEPHYRUM, x + w + co->q,
            y_positionis(co, b), syllaba, elementum, -I);
        w += co->q + (s32)m.latitudo;
    }
    redde w;
}

/* divisio: rectangula aut virgula; latitudo */
interior s32
divisionem_ponere (
                Compositor* co,
                       Xar* ad,
    constans GabcElementum* el,
                       s32  x,
                       s32  syllaba,
                       s32  elementum)
{
    s32 ys;
    s32 y_infima;
    s32 y_cursor;
    s32 alta;

    ys        = y_positionis(co, co->summa);
    y_infima  = ZEPHYRUM;
    alta      = el->alta ? -co->h : ZEPHYRUM;
    commutatio (el->divisio)
    {
        casus GABC_DIVISIO_VIRGULA:
            partem_emittere(ad, NEUMAE_VIRGULA, ZEPHYRUM, x,
                ys - co->q + alta, syllaba, elementum, -I);
            {
                NeumaeMensura m;

                mensuram_capere(co, NEUMAE_VIRGULA, ZEPHYRUM, &m);
                redde (s32)m.latitudo;
            }
        casus GABC_DIVISIO_MINIMIS:
            rectangulum_emittere(ad, x, ys - co->h + alta, co->k,
                co->h + co->k, GABC_TINCTURA_ATRAMENTUM, syllaba,
                elementum,
                -I);
            redde co->k;
        casus GABC_DIVISIO_MINIMA:
            rectangulum_emittere(ad, x, ys - co->h + alta, co->k,
                II * co->h + co->k, GABC_TINCTURA_ATRAMENTUM, syllaba,
                elementum, -I);
            redde co->k;
        casus GABC_DIVISIO_MAIOR:
            rectangulum_emittere(ad, x, ys, co->k, y_infima - ys
                + co->k,
                GABC_TINCTURA_ATRAMENTUM, syllaba, elementum, -I);
            redde co->k;
        casus GABC_DIVISIO_MAIOR_PUNCTATA:
            per (y_cursor = ys; y_cursor <= y_infima; y_cursor += co->s)
            {
                rectangulum_emittere(ad, x, y_cursor, co->k,
                    minimum(co->h, y_infima + co->k - y_cursor),
                    GABC_TINCTURA_ATRAMENTUM, syllaba, elementum, -I);
            }
            redde co->k;
        casus GABC_DIVISIO_FINALIS:
            rectangulum_emittere(ad, x, ys, co->k, y_infima - ys
                + co->k,
                GABC_TINCTURA_ATRAMENTUM, syllaba, elementum, -I);
            rectangulum_emittere(ad, x + co->k + co->h, ys, co->k,
                y_infima - ys + co->k, GABC_TINCTURA_ATRAMENTUM,
                syllaba,
                elementum, -I);
            redde II * co->k + co->h;
        casus GABC_DIVISIO_MINOR:
        casus GABC_DIVISIO_DOMINICANA:
        ordinarius:
            rectangulum_emittere(ad, x, ys + co->h, co->k,
                y_infima - co->h - (ys + co->h) + co->k,
                GABC_TINCTURA_ATRAMENTUM,
                syllaba, elementum, -I);
            redde co->k;
    }
}

/* spatium elementi SPATIUM (pixelis); fragilis? */
interior s32
spatium_metiri (
       constans Compositor* co,
    constans GabcElementum* el,
                       b32* fragilis)
{
    s32 n;
    s32 fractio;
    s32 divisor;
    s32 signum;
    i32 i;

    *fragilis = VERUM;
    commutatio (el->spatium)
    {
        casus GABC_SPATIUM_NULLUM:
            *fragilis = FALSUM;
            redde ZEPHYRUM;
        casus GABC_SPATIUM_INSEPARABILE:
            *fragilis = FALSUM;
            redde co->s * III / IV;
        casus GABC_SPATIUM_DIMIDIUM:
            redde co->s * III / VIII;
        casus GABC_SPATIUM_MAGNUM:
            redde co->s * III / II;
        casus GABC_SPATIUM_GLYPHAE:
            redde co->q;
        casus GABC_SPATIUM_FACTOR:
            /* /[n]: n * (/) ; n decimalis, signum permissum */
            n        = ZEPHYRUM;
            fractio  = ZEPHYRUM;
            divisor  = I;
            signum   = I;
            per (i = ZEPHYRUM; i < el->factor.mensura; i++)
            {
                character ch = (character)el->factor.datum[i];

                si (ch == '-')
                {
                    signum = -I;
                }
                alioquin si (ch == '.')
                {
                    divisor = X;
                }
                alioquin si (ch >= '0' && ch <= '9')
                {
                    si (divisor == I)
                    {
                        n = n * X + (ch - '0');
                    }
                    alioquin si (divisor < M * M)
                    {
                        fractio = fractio * X + (ch - '0');
                        divisor *= X;
                    }
                }
            }
            si (divisor > I)
            {
                divisor /= X;
            }
            n = signum * (n * divisor + fractio);
            redde maximum(ZEPHYRUM, (n * co->s * III / IV) / divisor);
        casus GABC_SPATIUM_NEUMATICUM:
        ordinarius:
            redde co->s * III / IV;
    }
}

/* elementum non spatium ab x in ad; latitudo */
interior s32
elementum_componere (
    Compositor* co,
           Xar* ad,
           s32  syllaba,
           s32  elementum,
           s32  x,
           s32* libramentum)
{
    constans GabcElementum* el;
              ClavisStatus  cs;
             NeumaeMensura  m;
                NeumaePars  pars;
                       s32  positio;

    el            = &co->cantus->syllabae[syllaba].elementa[elementum];
    *libramentum  = -I;
    commutatio (el->genus)
    {
        casus GABC_EL_NEUMA:
            redde neumam_componere(co, ad, syllaba, elementum, x,
                libramentum);
        casus GABC_EL_CLAVIS:
            cs.nota     = VERUM;
            cs.clavis   = el->clavis;
            cs.linea    = el->clavis_linea;
            cs.bemolle  = el->clavis_bemolle;
            si (!el->clavis_secunda)
            {
                co->clavis = cs;
            }
            redde clavem_ponere(co, ad, &cs, x, syllaba, elementum);
        casus GABC_EL_ALTERATIO:
            pars = NEUMAE_NATURALE;
            si (el->alteratio == GABC_ALTERATIO_BEMOLLE)
            {
                pars = NEUMAE_BEMOLLE;
            }
            alioquin si (el->alteratio == GABC_ALTERATIO_DIESIS)
            {
                pars = NEUMAE_DIESIS;
            }
            mensuram_capere(co, pars, ZEPHYRUM, &m);
            partem_emittere(ad, pars, ZEPHYRUM, x,
                y_positionis(co, el->positio), syllaba, elementum, -I);
            redde (s32)m.latitudo;
        casus GABC_EL_DIVISIO:
            redde divisionem_ponere(co, ad, el, x, syllaba, elementum);
        casus GABC_EL_CUSTOS:
            positio = el->positio >= ZEPHYRUM ? el->positio
                    : nota_sequens(co, syllaba, elementum + I);
            si (positio < ZEPHYRUM)
            {
                redde ZEPHYRUM;
            }
            partem_emittere(ad, NEUMAE_CUSTOS,
                positio >= VIII ? -I : I, x, y_positionis(co, positio),
                syllaba, elementum, -I);
            redde co->h;
        ordinarius:
            redde ZEPHYRUM;
    }
}

/* latitudo picti relativa ab x0: maximum finis capsarum [a, ...) */
interior s32
finis_pictorum (
    constans Compositor* co,
           constans Xar* xar,
                    i32  a,
                    s32  minimus)
{
    i32 i;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 finis;

    finis = minimus;
    per (i = a; i < xar_numerus(xar); i++)
    {
        constans GabcPictum* p = (constans GabcPictum*)xar_obtinere(xar,
            i);

        si (   p->genus != PICTUM_CAPSA
            && capsam_picti(co, p, &x0, &y0, &x1, &y1))
        {
            finis = maximum(finis, x1);
        }
    }
    redde finis;
}

/* frusta syllabae: co->frusta, co->frusta_picta, co->libramentum */
interior vacuum
frusta_aedificare (
    Compositor* co,
           s32  syllaba)
{
      constans GabcSyllaba* sy;
    constans GabcElementum* el;
                   Frustum* f;
                       s32  e;
                       s32  x;
                       s32  w;
                       s32  al;
                       s32  hiatus;
                       s32  spatium;
                       b32  fragilis;
                       i32  initium;

    xar_vacare(co->frusta);
    xar_vacare(co->frusta_picta);
    co->libramentum  = -I;
    sy               = &co->cantus->syllabae[syllaba];
    f                = (Frustum*)xar_addere(co->frusta);
    memset(f, ZEPHYRUM, magnitudo(*f));
    f->clavis            = co->clavis;
    f->elementum_primum  = ZEPHYRUM;
    x                    = ZEPHYRUM;
    hiatus               = -I;
    per (e = ZEPHYRUM; e < (s32)sy->numerus_elementorum; e++)
    {
        el = &sy->elementa[e];
        si (el->genus == GABC_EL_SIGNUM_OPACUM)
        {
            perge;
        }
        si (   el->genus == GABC_EL_SPATIUM
            || el->genus == GABC_EL_FRACTIO)
        {
            si (el->genus == GABC_EL_SPATIUM)
            {
                spatium = spatium_metiri(co, el, &fragilis);
                si (!fragilis)
                {
                    hiatus = spatium;
                    perge;
                }
            }
            /* frustum clauditur */
            f->numerus            = xar_numerus(co->frusta_picta)
                                  - f->primum;
            f->latitudo           = x;
            f->elementum_sequens  = e + I;
            si (el->genus == GABC_EL_SPATIUM)
            {
                f->genus  = FRUSTUM_SPATIUM;
                f->post   = spatium;
            }
            alioquin
            {
                f->genus         = FRUSTUM_FRACTIO;
                f->iustificata   = el->iustificata;
                f->custos_modus  = el->custos;
            }
            initium  = xar_numerus(co->frusta_picta);
            f        = (Frustum*)xar_addere(co->frusta);
            memset(f, ZEPHYRUM, magnitudo(*f));
            f->primum            = initium;
            f->clavis            = co->clavis;
            f->elementum_primum  = e + I;
            x                    = ZEPHYRUM;
            hiatus               = -I;
            perge;
        }
        si (x > ZEPHYRUM || xar_numerus(co->frusta_picta) > f->primum)
        {
            x += hiatus >= ZEPHYRUM ? hiatus : co->q;
        }
        hiatus   = -I;
        initium  = xar_numerus(co->frusta_picta);
        w       = elementum_componere(co, co->frusta_picta, syllaba, e,
            x,
                      &al);
        w       = maximum(w, finis_pictorum(co, co->frusta_picta,
            initium,
                      x) - x);
        si (xar_numerus(co->frusta_picta) == initium && w == ZEPHYRUM)
        {
            perge;
        }
        si (   co->libramentum < ZEPHYRUM && el->genus == GABC_EL_NEUMA
            && xar_numerus(co->frusta) == I && al >= ZEPHYRUM)
        {
            co->libramentum = x + al;
        }
        f->finit_custode  = el->genus == GABC_EL_CUSTOS;
        si (el->genus != GABC_EL_CLAVIS && el->genus != GABC_EL_CUSTOS)
        {
            f->cantatum = VERUM;
        }
        x                 += w;
    }
    f->numerus            = xar_numerus(co->frusta_picta) - f->primum;
    f->latitudo           = x;
    f->genus              = FRUSTUM_FINIS;
    f->elementum_sequens  = (s32)sy->numerus_elementorum;
    si (co->libramentum < ZEPHYRUM)
    {
        f = (Frustum*)xar_obtinere(co->frusta, (i32)(ZEPHYRUM));
        co->libramentum = f->latitudo / II;
    }
}


/* ==================================================
 * Systemata et paginae
 * ================================================== */

interior vacuum
systema_incipere (
               Compositor* co,
    constans ClavisStatus* clavis,
                      b32  sequens_clavis)
{
    NeumaeMensura m;
              s32 w;
              s32 centrum;
              s32 y;
              i32 a;
           chorda annotatio;

    xar_vacare(co->systema);
    xar_vacare(co->positae);
    co->plenum          = FALSUM;
    co->cantatum        = FALSUM;
    co->ultimum_custos  = FALSUM;
    co->x_linea         = co->margo
                        + (co->systematis_index
                        < co->initialis_systemata
                           ? co->intentio : ZEPHYRUM);
    co->cursor_notae  = co->x_linea;
    co->textus_finis  = co->x_linea;
    w                 = ZEPHYRUM;
    si (!sequens_clavis && clavis->nota)
    {
        w                 = clavem_ponere(co, co->systema, clavis,
                                co->x_linea, -I, -I);
        co->cursor_notae = co->x_linea + w + co->s;
        co->textus_finis = co->x_linea + w;
    }
    si (co->systematis_index == ZEPHYRUM)
    {
        si (co->initialis_syllaba >= ZEPHYRUM)
        {
            GabcPictum* p = emittere(co->systema, GABC_PICTUM_INITIALIS,
                co->margo, y_positionis(co, co->summa), -I, -I, -I);

            si (p)
            {
                p->latitudo  = (i32)(LATITUDO_FONTIS_INITIALIS
                                     * co->initialis_scala);
                p->altitudo  = (i32)(ALTITUDO_FONTIS_INITIALIS
                                     * co->initialis_scala);
                p->textus  = co->initialis_littera;
                p->fons    = (i32)co->initialis_scala;
            }
            centrum = co->margo + LATITUDO_FONTIS_INITIALIS
                    * co->initialis_scala / II;
        }
        alioquin
        {
            mensuram_capere(co, NEUMAE_CLAVIS_C, ZEPHYRUM, &m);
            centrum = co->x_linea + (s32)m.latitudo / II;
        }
        per (a = ZEPHYRUM; a < co->cantus->numerus_annotationum; a++)
        {
            annotatio  = co->cantus->annotationes[a];
            y          = y_positionis(co, co->summa) - co->s
                       - (s32)(co->cantus->numerus_annotationum
                       - a) * co->fh;
            textum_emittere(co->systema, annotatio,
                co->forma->fons_textus,
                maximum(ZEPHYRUM,
                    centrum - runae_numerare(annotatio) * co->cw / II),
                y, GABC_TINCTURA_RUBRICA, -I);
        }
    }
}

/* systema currens finitur: custos, iustificatio, lineolae, ordo
 * textus, lineae systematis, pagina */
interior vacuum
systema_finire (
    Compositor* co,
           b32  iustificare,
           s32  custos_modus)
{
    SyllabaPosita* ra;
    SyllabaPosita* rb;
       GabcPictum* p;
       GabcPictum* d;
              s32  positio;
              s32  finis;
              s32  extra;
              s32  basis;
              s32  residuum;
              s32  n;
              s32  i;
              s32  motus;
              s32  hiatus;
              s32  summum;
              s32  imum;
              s32  imum_notarum;
              s32  summum_notarum;
              s32  y_textus;
              s32  y_supra;
              s32  x0;
              s32  y0;
              s32  x1;
              s32  y1;
              s32  l;
              s32  dy;
              i32  j;
              i32  numerus;
              i32  finis_r;
              b32  textus_adest;
              b32  supra_adest;
           chorda  lineola;
               i8* datum;

    si (!co->plenum)
    {
        xar_vacare(co->systema);
        redde;
    }

    /* custos */
    si (custos_modus >= ZEPHYRUM && !co->ultimum_custos)
    {
        positio = nota_sequens(co, co->fractio_syllaba,
            co->fractio_elementum);
        si (positio >= ZEPHYRUM)
        {
            finis = ZEPHYRUM;
            per (j = ZEPHYRUM; j < xar_numerus(co->systema); j++)
            {
                p = (GabcPictum*)xar_obtinere(co->systema, j);
                si (   p->genus != GABC_PICTUM_TEXTUS
                    && p->genus != PICTUM_CAPSA
                    && p->genus != GABC_PICTUM_INITIALIS
                    && capsam_picti(co, p, &x0, &y0, &x1, &y1))
                {
                    finis = maximum(finis, x1);
                }
            }
            partem_emittere(co->systema, NEUMAE_CUSTOS,
                positio >= VIII ? -I : I, finis + co->h,
                y_positionis(co, positio), -I, -I, -I);
        }
    }

    /* iustificatio inter syllabas */
    n = (s32)xar_numerus(co->positae);
    si (iustificare && n >= II)
    {
        finis = ZEPHYRUM;
        per (j = ZEPHYRUM; j < xar_numerus(co->systema); j++)
        {
            p = (GabcPictum*)xar_obtinere(co->systema, j);
            si (   p->genus != PICTUM_CAPSA
                && p->genus != GABC_PICTUM_INITIALIS
                && capsam_picti(co, p, &x0, &y0, &x1, &y1))
            {
                finis = maximum(finis, x1);
            }
            si (p->genus == GABC_PICTUM_TEXTUS && p->y == Y_CANTATUS)
            {
                finis = maximum(finis, p->x + runae_numerare(p->textus)
                    * co->cw);
            }
        }
        extra = co->finis_lineae - finis;
        si (extra > ZEPHYRUM)
        {
            basis     = extra / (n - I);
            residuum  = extra % (n - I);
            per (i = I; i < n; i++)
            {
                ra = (SyllabaPosita*)xar_obtinere(co->positae,
                    (i32)(i));
                motus = basis * i + minimum(i, residuum);
                finis_r  = i + I < n
                         ? ((SyllabaPosita*)xar_obtinere(co->positae,
                         (i32)(i + I)))->primum
                         : xar_numerus(co->systema);
                per (j = ra->primum; j < finis_r; j++)
                {
                    p     = (GabcPictum*)xar_obtinere(co->systema, j);
                    p->x  += motus;
                }
                ra->textus_initium  += motus;
                ra->textus_finis    += motus;
            }
        }
    }

    /* lineolae inter syllabas eiusdem verbi */
    per (i = I; i < n; i++)
    {
        ra = (SyllabaPosita*)xar_obtinere(co->positae, (i32)(i - I));
        rb = (SyllabaPosita*)xar_obtinere(co->positae, (i32)(i));
        si (!ra->textus || !rb->textus || rb->verbum_incipit)
        {
            perge;
        }
        hiatus = rb->textus_initium - ra->textus_finis;
        si (hiatus >= co->cw / II)
        {
            datum = (i8*)piscina_allocare(co->piscina, I);
            si (datum)
            {
                datum[ZEPHYRUM]  = '-';
                lineola.datum    = datum;
                lineola.mensura  = I;
                textum_emittere(co->systema, lineola,
                    co->forma->fons_textus,
                    ra->textus_finis + (hiatus - co->cw) / II,
                    Y_CANTATUS,
                    GABC_TINCTURA_ATRAMENTUM, -I);
            }
        }
    }

    /* ordo verticalis */
    summum_notarum  = y_positionis(co, co->summa);
    imum_notarum    = ZEPHYRUM;
    textus_adest    = FALSUM;
    supra_adest     = FALSUM;
    summum          = y_positionis(co, co->summa) - co->s;
    per (j = ZEPHYRUM; j < xar_numerus(co->systema); j++)
    {
        p = (GabcPictum*)xar_obtinere(co->systema, j);
        si (p->genus == GABC_PICTUM_TEXTUS && p->y == Y_CANTATUS)
        {
            textus_adest = VERUM;
            perge;
        }
        si (p->genus == GABC_PICTUM_TEXTUS && p->y == Y_SUPRA)
        {
            supra_adest = VERUM;
            perge;
        }
        si (!capsam_picti(co, p, &x0, &y0, &x1, &y1))
        {
            perge;
        }
        summum = minimum(summum, y0);
        si (   p->genus == GABC_PICTUM_PARS || p->genus == PICTUM_CAPSA
            || (p->genus == GABC_PICTUM_RECTANGULUM
                && p->syllaba >= ZEPHYRUM))
        {
            summum_notarum  = minimum(summum_notarum, y0);
            imum_notarum    = maximum(imum_notarum, y1);
        }
    }
    y_textus  = maximum(co->s, imum_notarum + co->q);
    y_supra   = minimum(y_positionis(co, co->summa) - co->s,
                    summum_notarum - co->q) - co->fh;
    si (supra_adest)
    {
        summum = minimum(summum, y_supra);
    }
    imum = textus_adest ? y_textus + co->fh : maximum(imum_notarum,
        co->k);
    per (j = ZEPHYRUM; j < xar_numerus(co->systema); j++)
    {
        p = (GabcPictum*)xar_obtinere(co->systema, j);
        si (p->genus == GABC_PICTUM_TEXTUS && p->y == Y_CANTATUS)
        {
            p->y = y_textus;
        }
        alioquin si (p->genus == GABC_PICTUM_TEXTUS && p->y == Y_SUPRA)
        {
            p->y = y_supra;
        }
    }

    /* pagina */
    si (   !co->pagina_vacua
        && co->cursor_paginae + (imum - summum)
           > (s32)co->forma->altitudo - co->margo)
    {
        co->pagina++;
        co->cursor_paginae  = co->margo;
        co->pagina_vacua    = VERUM;
    }
    dy = co->cursor_paginae - summum;

    /* lineae systematis primae, deinde picta systematis */
    per (l = III; l <= co->summa; l += II)
    {
        d = emittere(co->omnia, GABC_PICTUM_RECTANGULUM, co->x_linea,
            y_positionis(co, l) - co->k / II + dy, -I, -I, -I);
        si (d)
        {
            d->latitudo = (i32)(co->finis_lineae - co->x_linea);
            d->altitudo = (i32)co->k;
            d->tinctura = GABC_TINCTURA_SYSTEMA;
        }
        *(s32*)xar_addere(co->paginae_pictorum) = co->pagina;
        *(s32*)xar_addere(co->systemata_pictorum) =
            co->systematis_index;
    }
    numerus = xar_numerus(co->systema);
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        p = (GabcPictum*)xar_obtinere(co->systema, j);
        d = (GabcPictum*)xar_addere(co->omnia);
        *d = *p;
        d->y += dy;
        *(s32*)xar_addere(co->paginae_pictorum) = co->pagina;
        *(s32*)xar_addere(co->systemata_pictorum) =
            co->systematis_index;
    }
    co->cursor_paginae  += imum - summum + co->s;
    co->pagina_vacua    = FALSUM;
    co->systematis_index++;
    xar_vacare(co->systema);
}

/* syllaba posita in systemate currente (creatur si deest) */
interior SyllabaPosita*
positam_capere (
    Compositor* co,
           s32  syllaba)
{
    SyllabaPosita* r;
              i32  n;

    n = xar_numerus(co->positae);
    si (n > ZEPHYRUM)
    {
        r = (SyllabaPosita*)xar_obtinere(co->positae, (i32)(n - I));
        si (r->syllaba == syllaba)
        {
            redde r;
        }
    }
    r = (SyllabaPosita*)xar_addere(co->positae);
    memset(r, ZEPHYRUM, magnitudo(*r));
    r->syllaba         = syllaba;
    r->primum          = xar_numerus(co->systema);
    r->verbum_incipit  = co->cantus->syllabae[syllaba].verbum_incipit;
    redde r;
}

/* frustum in systema ad x */
interior vacuum
frustum_ponere (
            Compositor* co,
      constans Frustum* f,
                   s32  syllaba,
                   s32  x)
{
           i32  j;
    GabcPictum* d;

    si (f->numerus == ZEPHYRUM)
    {
        redde;
    }
    positam_capere(co, syllaba);
    per (j = f->primum; j < f->primum + f->numerus; j++)
    {
        d     = (GabcPictum*)xar_addere(co->systema);
        *d    = *(GabcPictum*)xar_obtinere(co->frusta_picta, j);
        d->x  += x;
    }
    co->plenum          = VERUM;
    co->ultimum_custos  = f->finit_custode;
    si (f->cantatum)
    {
        co->cantatum = VERUM;
    }
}

/* textus syllabae ad x in systemate currente */
interior vacuum
textum_syllabae_ponere (
    Compositor* co,
           s32  syllaba,
           s32  x)
{
    SyllabaPosita* r;

    r                  = positam_capere(co, syllaba);
    r->textus          = VERUM;
    r->textus_initium  = x;
    r->textus_finis    = x + textum_emittere_syllabae(co, syllaba, x);
    co->textus_finis   = r->textus_finis;
    co->plenum         = VERUM;
    co->cantatum       = VERUM;
}

interior vacuum
frangere (
               Compositor* co,
                      s32  syllaba,
                      s32  elementum,
                      b32  iustificare,
                      s32  custos_modus,
    constans ClavisStatus* clavis)
{
    co->fractio_syllaba    = syllaba;
    co->fractio_elementum  = elementum;
    systema_finire(co, iustificare, custos_modus);
    systema_incipere(co, clavis, clavis_sequitur(co, syllaba,
        elementum));
}

interior vacuum
syllabam_ponere (
    Compositor* co,
           s32  syllaba)
{
    TextusMensura  tm;
          Frustum* f;
          Frustum* g;
              s32  numerus;
              s32  a;
              s32  b;
              s32  i;
              s32  W;
              s32  x_notarum;
              s32  x_textus;
              s32  o;
              s32  x;
              s32  finis;
              s32  dextra;
              s32  cursor_textus;
              b32  prima;
              b32  notae;
              b32  textus;
     ClavisStatus  post;

    textum_metiri(co, syllaba, &tm);
    frusta_aedificare(co, syllaba);
    post     = co->clavis;
    numerus  = (s32)xar_numerus(co->frusta);
    prima    = VERUM;
    a        = ZEPHYRUM;
    dum (a < numerus)
    {
        b = a;
        dum (   b < numerus - I
             && ((Frustum*)xar_obtinere(co->frusta, (i32)(b)))->genus
               != FRUSTUM_FRACTIO)
        {
            b++;
        }
        W = ZEPHYRUM;
        per (i = a; i <= b; i++)
        {
            f = (Frustum*)xar_obtinere(co->frusta, (i32)(i));
            W += f->latitudo + (i < b ? f->post : ZEPHYRUM);
        }
        notae   = FALSUM;
        per (i = a; i <= b; i++)
        {
            si (((Frustum*)xar_obtinere(co->frusta, (i32)(i)))->numerus
                > ZEPHYRUM)
            {
                notae = VERUM;
            }
        }
        textus     = prima && tm.adest;
        x_notarum  = ZEPHYRUM;
        x_textus   = ZEPHYRUM;
        si (textus && notae)
        {
            si (co->libramentum >= tm.centrum)
            {
                x_textus = co->libramentum - tm.centrum;
            }
            alioquin
            {
                x_notarum = tm.centrum - co->libramentum;
            }
        }
        f = (Frustum*)xar_obtinere(co->frusta, (i32)(a));

        /* positio et fractio ante unitatem */
        cursor_textus = co->textus_finis
                      + (co->cantus->syllabae[syllaba].verbum_incipit
                         ? co->cw : ZEPHYRUM);
        o = notae ? co->cursor_notae - x_notarum : cursor_textus
            - x_textus;
        si (textus)
        {
            o = maximum(o, cursor_textus - x_textus);
        }
        dextra = maximum(notae ? o + x_notarum + W : o, textus ? o
            + x_textus
            + tm.latitudo
                                                        : o);
        si ((notae || textus) && dextra > co->limes && co->cantatum)
        {
            frangere(co, syllaba, f->elementum_primum, VERUM, ZEPHYRUM,
                &f->clavis);
            cursor_textus = co->textus_finis;
            o = notae ? co->cursor_notae - x_notarum : cursor_textus
                - x_textus;
            si (textus)
            {
                o = maximum(o, cursor_textus - x_textus);
            }
        }

        /* frusta */
        x      = o + x_notarum;
        finis  = x;
        per (i = a; i <= b; i++)
        {
            f = (Frustum*)xar_obtinere(co->frusta, (i32)(i));
            si (i > a)
            {
                g = (Frustum*)xar_obtinere(co->frusta, (i32)(i - I));
                x += g->post;
                si (f->numerus > ZEPHYRUM && x + f->latitudo > co->limes
                    && co->plenum && g->genus == FRUSTUM_SPATIUM)
                {
                    frangere(co, syllaba, f->elementum_primum, VERUM,
                        ZEPHYRUM, &f->clavis);
                    x = co->cursor_notae;
                }
            }
            frustum_ponere(co, f, syllaba, x);
            si (i == a && textus)
            {
                /* textus cum parte prima manet (G7): ante fractionem
                 * mediam syllabae ponitur */
                textum_syllabae_ponere(co, syllaba, o + x_textus);
            }
            si (i == a && prima && notae)
            {
                supra_emittere(co, syllaba, o + x_notarum);
            }
            si (f->numerus > ZEPHYRUM)
            {
                finis = x + f->latitudo;
            }
            x += f->latitudo;
        }
        si (!textus && notae)
        {
            co->textus_finis = maximum(co->textus_finis, finis);
        }
        si (notae)
        {
            co->cursor_notae = maximum(co->cursor_notae, finis + co->s);
        }

        /* fractio scripta */
        f = (Frustum*)xar_obtinere(co->frusta, (i32)(b));
        si (f->genus == FRUSTUM_FRACTIO && co->plenum)
        {
            frangere(co, syllaba, f->elementum_sequens, f->iustificata,
                f->custos_modus,
                b + I < numerus
                    ? &((Frustum*)xar_obtinere(co->frusta, (i32)(b
                        + I)))->clavis
                    : &post);
        }
        a      = b + I;
        prima  = FALSUM;
    }
}


/* ==================================================
 * Initialis et caput paginae
 * ================================================== */

interior vacuum
initialem_parare (
    Compositor* co)
{
    constans GabcCantus* c;
            constans i8* p;
            constans i8* finis;
                    s32  stilus;
                    s32  s;
                    s32  altitudo;
                    i32  j;
                    i32  r;

    c       = co->cantus;
    stilus  = I;
    per (j = ZEPHYRUM; j < c->numerus_capitum; j++)
    {
        si (   c->capita[j].clavis.mensura == XIII
            && memcmp(c->capita[j].clavis.datum, "initial-style", XIII)
               == ZEPHYRUM
            && c->capita[j].valor.mensura > ZEPHYRUM)
        {
            stilus = c->capita[j].valor.datum[ZEPHYRUM] - '0';
        }
    }
    co->initialis_syllaba    = -I;
    co->initialis_systemata  = ZEPHYRUM;
    co->intentio             = ZEPHYRUM;
    si (stilus < I || stilus > II)
    {
        redde;
    }
    per (s = ZEPHYRUM; s < (s32)c->numerus_syllabarum; s++)
    {
        per (r = ZEPHYRUM; r < c->syllabae[s].numerus_textuum; r++)
        {
            si (c->syllabae[s].textus[r].genus == GABC_TEXTUS_CANTATUS
                && c->syllabae[s].textus[r].textus.mensura > ZEPHYRUM)
            {
                p      = c->syllabae[s].textus[r].textus.datum;
                finis  = p + c->syllabae[s].textus[r].textus.mensura;
                utf8_decodere(&p, finis);
                co->initialis_littera.datum    =
                    c->syllabae[s].textus[r].textus.datum;
                co->initialis_littera.mensura  =
                    (i32)(p - c->syllabae[s].textus[r].textus.datum);
                co->initialis_syllaba          = s;
                frange;
            }
        }
        si (co->initialis_syllaba >= ZEPHYRUM)
        {
            frange;
        }
    }
    si (co->initialis_syllaba < ZEPHYRUM)
    {
        redde;
    }
    altitudo = (co->lineae - I) * co->s + co->s + co->fh;
    si (stilus == II)
    {
        altitudo = II * altitudo + co->s;
    }
    co->initialis_scala      = maximum(I, altitudo
                                          / ALTITUDO_FONTIS_INITIALIS);
    co->initialis_systemata  = stilus;
    co->intentio             = LATITUDO_FONTIS_INITIALIS
                               * co->initialis_scala + co->h;
}


/* ==================================================
 * Functiones
 * ================================================== */

vacuum
gabc_compositionis_formam_ordinariam (
    GabcCompositionisForma* forma)
{
    forma->latitudo     = CMLX;
    forma->altitudo     = DLXXIII;
    forma->spatium      = VIII;
    forma->fons_textus  = FONS_PARVUS;
    forma->fons_tituli  = FONS_TITULI;
}

/* locus syllabarum ex pictis eiusdem systematis ac pictum primum */
interior vacuum
locos_computare (
                Compositor* co,
            GabcCompositio* comp,
              constans s32* paginae,
              constans s32* systemata)
{
                    s32* systema_syllabae;
                    s32  x0;
                    s32  y0;
                    s32  x1;
                    s32  y1;
                    i32  i;
                    s32  s;
              GabcLocus* l;
    constans GabcPictum* p;

    systema_syllabae = (s32*)piscina_allocare(co->piscina,
        (memoriae_index)(magnitudo(s32)
            * (comp->numerus_syllabarum + I)));
    per (i = ZEPHYRUM; i < comp->numerus_syllabarum; i++)
    {
        comp->syllabae[i].pagina    = -I;
        comp->syllabae[i].x         = ZEPHYRUM;
        comp->syllabae[i].y         = ZEPHYRUM;
        comp->syllabae[i].latitudo  = ZEPHYRUM;
        comp->syllabae[i].altitudo  = ZEPHYRUM;
        si (systema_syllabae)
        {
            systema_syllabae[i] = -I;
        }
    }
    si (systema_syllabae == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        p = &comp->picta[i];
        s = p->syllaba;
        si (   s < ZEPHYRUM || s >= (s32)comp->numerus_syllabarum
            || !capsam_picti(co, p, &x0, &y0, &x1, &y1))
        {
            perge;
        }
        l = &comp->syllabae[s];
        si (systema_syllabae[s] < ZEPHYRUM)
        {
            systema_syllabae[s]  = systemata[i];
            l->pagina            = paginae[i];
            l->x                 = x0;
            l->y                 = y0;
            l->latitudo          = (i32)(x1 - x0);
            l->altitudo          = (i32)(y1 - y0);
        }
        alioquin si (systema_syllabae[s] == systemata[i])
        {
            x1           = maximum(x1, l->x + (s32)l->latitudo);
            y1           = maximum(y1, l->y + (s32)l->altitudo);
            l->x         = minimum(l->x, x0);
            l->y         = minimum(l->y, y0);
            l->latitudo  = (i32)(x1 - l->x);
            l->altitudo  = (i32)(y1 - l->y);
        }
    }
}

GabcCompositio*
gabc_componere (
              constans GabcCantus* cantus,
    constans GabcCompositionisForma* forma,
                           Piscina* piscina)
{
        Compositor* co;
    GabcCompositio* comp;
      ClavisStatus  nulla;
        GabcPictum* d;
               s32* paginae;
               s32* systemata;
               i32  i;
               i32  n;
               i32  numerus;
               s32  pagina;
               s32  w;

    si (   cantus == NIHIL || forma == NIHIL || piscina == NIHIL
        || forma->spatium < VI || forma->spatium > XLVIII
        || forma->spatium % II != ZEPHYRUM
        || forma->latitudo < XVI * forma->spatium)
    {
        redde NIHIL;
    }
    co   = (Compositor*)piscina_allocare(piscina,
        magnitudo(Compositor));
    comp = (GabcCompositio*)piscina_allocare(piscina,
        magnitudo(GabcCompositio));
    si (co == NIHIL || comp == NIHIL)
    {
        redde NIHIL;
    }
    memset(co, ZEPHYRUM, magnitudo(*co));
    co->cantus   = cantus;
    co->forma    = forma;
    co->piscina  = piscina;
    co->s        = (s32)forma->spatium;
    co->h        = co->s / II;
    co->k        = maximum(I, co->s / VIII);
    co->q        = co->s / IV;
    co->cw       = fons_latitudo(forma->fons_textus);
    co->fh       = fons_altitudo(forma->fons_textus);
    co->cwt      = fons_latitudo(forma->fons_tituli);
    co->fht      = fons_altitudo(forma->fons_tituli);
    co->lineae        = cantus->lineae >= II && cantus->lineae <= V
                      ? (s32)cantus->lineae : IV;
    co->summa         = II * co->lineae + I;
    co->margo         = co->s;
    co->finis_lineae  = (s32)forma->latitudo - co->margo;
    co->limes         = co->finis_lineae - co->h - co->h;
    co->systema = xar_creare(piscina,
        magnitudo(GabcPictum));
    co->positae = xar_creare(piscina,
        magnitudo(SyllabaPosita));
    co->frusta_picta = xar_creare(piscina,
        magnitudo(GabcPictum));
    co->frusta = xar_creare(piscina, magnitudo(Frustum));
    co->omnia = xar_creare(piscina,
        magnitudo(GabcPictum));
    co->paginae_pictorum    = xar_creare(piscina, magnitudo(s32));
    co->systemata_pictorum  = xar_creare(piscina, magnitudo(s32));
    si (   co->systema == NIHIL || co->positae == NIHIL
        || co->frusta_picta == NIHIL || co->frusta == NIHIL
        || co->omnia == NIHIL || co->paginae_pictorum == NIHIL
        || co->systemata_pictorum == NIHIL)
    {
        redde NIHIL;
    }
    initialem_parare(co);

    /* titulus */
    co->cursor_paginae  = co->margo;
    co->pagina_vacua    = VERUM;
    si (cantus->titulus.mensura > ZEPHYRUM)
    {
        w = runae_numerare(cantus->titulus) * co->cwt;
        textum_emittere(co->omnia, cantus->titulus, forma->fons_tituli,
            maximum(co->margo, ((s32)forma->latitudo - w) / II),
            co->margo,
            GABC_TINCTURA_ATRAMENTUM, -I);
        *(s32*)xar_addere(co->paginae_pictorum)    = ZEPHYRUM;
        *(s32*)xar_addere(co->systemata_pictorum)  = -I;
        co->cursor_paginae                         += co->fht + co->s;
    }

    memset(&nulla, ZEPHYRUM, magnitudo(nulla));
    systema_incipere(co, &nulla, clavis_sequitur(co, ZEPHYRUM,
        ZEPHYRUM));
    per (i = ZEPHYRUM; i < cantus->numerus_syllabarum; i++)
    {
        syllabam_ponere(co, (s32)i);
    }
    co->fractio_syllaba    = (s32)cantus->numerus_syllabarum;
    co->fractio_elementum  = ZEPHYRUM;
    systema_finire(co, FALSUM, -I);

    /* exitus: picta sine capsis, paginae, loci */
    numerus                   = xar_numerus(co->omnia);
    comp->forma               = *forma;
    comp->numerus_syllabarum  = cantus->numerus_syllabarum;
    comp->numerus_notarum     = cantus->numerus_notarum;
    comp->numerus_paginarum   = (i32)(co->pagina + I);
    comp->picta     = (GabcPictum*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(GabcPictum) * (numerus + I)));
    paginae         = (s32*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(s32) * (numerus + I)));
    systemata       = (s32*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(s32) * (numerus + I)));
    comp->paginae   = (i32*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(i32) * (comp->numerus_paginarum
            + I)));
    comp->syllabae  = (GabcLocus*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(GabcLocus)
            * (comp->numerus_syllabarum + I)));
    comp->notae     = (GabcLocus*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(GabcLocus)
            * (comp->numerus_notarum + I)));
    si (comp->picta == NIHIL || paginae == NIHIL || systemata == NIHIL
        || comp->paginae == NIHIL || comp->syllabae == NIHIL
        || comp->notae   == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < comp->numerus_notarum; i++)
    {
        memset(&comp->notae[i], ZEPHYRUM, magnitudo(GabcLocus));
        comp->notae[i].pagina = -I;
    }
    n       = ZEPHYRUM;
    pagina  = -I;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        d = (GabcPictum*)xar_obtinere(co->omnia, i);
        si (d->genus == PICTUM_CAPSA)
        {
            si (   d->nota >= ZEPHYRUM
                && d->nota < (s32)comp->numerus_notarum
                && comp->notae[d->nota].pagina < ZEPHYRUM)
            {
                comp->notae[d->nota].pagina    =
                    *(s32*)xar_obtinere(co->paginae_pictorum, i);
                comp->notae[d->nota].x         = d->x;
                comp->notae[d->nota].y         = d->y;
                comp->notae[d->nota].latitudo  = d->latitudo;
                comp->notae[d->nota].altitudo  = d->altitudo;
            }
            perge;
        }
        dum (pagina < *(s32*)xar_obtinere(co->paginae_pictorum, i))
        {
            pagina++;
            comp->paginae[pagina] = n;
        }
        comp->picta[n]  = *d;
        paginae[n]      = *(s32*)xar_obtinere(co->paginae_pictorum, i);
        systemata[n] = *(s32*)xar_obtinere(co->systemata_pictorum,
            i);
        n++;
    }
    dum (pagina < (s32)comp->numerus_paginarum - I)
    {
        pagina++;
        comp->paginae[pagina] = n;
    }
    comp->numerus_pictorum                  = n;
    comp->paginae[comp->numerus_paginarum]  = n;
    locos_computare(co, comp, paginae, systemata);
    redde comp;
}

s32
gabc_compositio_syllaba_ad_punctum (
    constans GabcCompositio* compositio,
                        i32  pagina,
                        s32  x,
                        s32  y)
{
    constans GabcLocus* l;
                   s32  optima;
                   s32  distantia;
                   s32  d;
                   i32  i;

    si (compositio == NIHIL || pagina >= compositio->numerus_paginarum)
    {
        redde -I;
    }
    per (i = ZEPHYRUM; i < compositio->numerus_syllabarum; i++)
    {
        l = &compositio->syllabae[i];
        si (   l->pagina == (s32)pagina && x >= l->x
            && x < l->x + (s32)l->latitudo && y >= l->y
            && y < l->y + (s32)l->altitudo)
        {
            redde (s32)i;
        }
    }
    optima     = -I;
    distantia  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < compositio->numerus_syllabarum; i++)
    {
        l = &compositio->syllabae[i];
        si (   l->pagina != (s32)pagina || y < l->y
            || y         >= l->y + (s32)l->altitudo)
        {
            perge;
        }
        d = x < l->x ? l->x - x : x - (l->x + (s32)l->latitudo);
        si (optima < ZEPHYRUM || d < distantia)
        {
            optima     = (s32)i;
            distantia  = d;
        }
    }
    redde optima;
}

interior constans character*
tincturae_titulus (
    GabcTinctura t)
{
    redde t == GABC_TINCTURA_SYSTEMA ? "systema"
         : t == GABC_TINCTURA_RUBRICA ? "rubrica" : "atramentum";
}

b32
gabc_compositionem_scribere (
    constans GabcCompositio* compositio,
          ChordaAedificator* exitus)
{
    constans GabcPictum* p;
     constans GabcLocus* l;
                    i32  i;
                    i32  pagina;

    si (compositio == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    pagina = ZEPHYRUM;
    per (i = ZEPHYRUM; i < compositio->numerus_pictorum; i++)
    {
        p = &compositio->picta[i];
        dum (   pagina + I < compositio->numerus_paginarum
             && i >= compositio->paginae[pagina + I])
        {
            pagina++;
        }
        chorda_aedificator_appendere_literis(exitus, "pictum\t");
        chorda_aedificator_appendere_i32(exitus, pagina);
        commutatio (p->genus)
        {
            casus GABC_PICTUM_PARS:
                chorda_aedificator_appendere_literis(exitus,
                    "\tpars\t");
                frange;
            casus GABC_PICTUM_RECTANGULUM:
                chorda_aedificator_appendere_literis(exitus,
                    "\trect\t");
                frange;
            casus GABC_PICTUM_TEXTUS:
                chorda_aedificator_appendere_literis(exitus,
                    "\ttextus\t");
                frange;
            ordinarius:
                chorda_aedificator_appendere_literis(exitus,
                    "\tinitialis\t");
                frange;
        }
        chorda_aedificator_appendere_s32(exitus, p->x);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_s32(exitus, p->y);
        chorda_aedificator_appendere_character(exitus, '\t');
        commutatio (p->genus)
        {
            casus GABC_PICTUM_PARS:
                chorda_aedificator_appendere_literis(exitus,
                    neumae_partis_titulus(p->pars));
                chorda_aedificator_appendere_character(exitus, ':');
                chorda_aedificator_appendere_s32(exitus, p->parametrum);
                frange;
            casus GABC_PICTUM_RECTANGULUM:
                chorda_aedificator_appendere_i32(exitus, p->latitudo);
                chorda_aedificator_appendere_character(exitus, 'x');
                chorda_aedificator_appendere_i32(exitus, p->altitudo);
                frange;
            casus GABC_PICTUM_TEXTUS:
                chorda_aedificator_appendere_character(exitus, '"');
                chorda_aedificator_appendere_chorda(exitus, p->textus);
                chorda_aedificator_appendere_literis(exitus, "\"/");
                chorda_aedificator_appendere_i32(exitus, p->fons);
                frange;
            ordinarius:
                chorda_aedificator_appendere_character(exitus, '"');
                chorda_aedificator_appendere_chorda(exitus, p->textus);
                chorda_aedificator_appendere_literis(exitus, "\"x");
                chorda_aedificator_appendere_i32(exitus, p->fons);
                frange;
        }
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_literis(exitus,
            tincturae_titulus(p->tinctura));
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_s32(exitus, p->syllaba);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_s32(exitus, p->elementum);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_s32(exitus, p->nota);
        chorda_aedificator_appendere_character(exitus, '\n');
    }
    per (i = ZEPHYRUM; i < compositio->numerus_syllabarum; i++)
    {
        l = &compositio->syllabae[i];
        chorda_aedificator_appendere_literis(exitus, "syllaba\t");
        chorda_aedificator_appendere_i32(exitus, i);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_s32(exitus, l->pagina);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_s32(exitus, l->x);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_s32(exitus, l->y);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_i32(exitus, l->latitudo);
        chorda_aedificator_appendere_character(exitus, 'x');
        chorda_aedificator_appendere_i32(exitus, l->altitudo);
        chorda_aedificator_appendere_character(exitus, '\n');
    }
    redde VERUM;
}
