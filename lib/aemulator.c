/* aemulator.c - nucleus emulatoris terminalis (aemulator-plan A1, A2)
 *
 * A1: creatio, cellulae et lineae, impressio per runae (angusta, lata,
 * caudae, caput, involutio pendens), CR LF BS HT BEL, mutatio
 * magnitudinis, effusio plana.
 * A2: CUP et motus relativi, ED/EL (cellulae deletae fundum calami
 * servant), SGR per stilus_applicare cum stilis internatis (tabula
 * fixa; plena -> collectio et compactio), DECTCEM, 1049 (schirmum
 * alterum, cursor servatus), 2026, DECSC/DECRC. Series ceterae
 * consumuntur et numerantur.
 *
 * PURUS: solae series_terminalis, runae, utf8, stilus_terminalis,
 * piscina; <string.h> solum. Effusio sine chorda_aedificator. */

#include "aemulator.h"
#include "series_terminalis.h"
#include "runae.h"
#include "utf8.h"
#include <string.h>

#define LATITUDO_ORDINARIA  LXXX
#define ALTITUDO_ORDINARIA  XXIV
/* limes hostilis: latus maximum schirmi (cellulae) */
#define LATUS_MAXIMUM       (IV * MXXIV)
#define TABULATIO           VIII
#define RUNA_SUBSTITUTA     0xFFFD
/* stili simul vivi; plena tabula colligitur (status constans) */
#define STILI_MAXIMI        DXII

nomen structura {
     i8 octeti[IV];    /* UTF-8 runae (graphemata longiora: v2) */
     i8 mensura;       /* 0 = vacua */
     i8 latitudo;      /* AemulatorLatitudo */
     i8 reservatum[II];
    i32 stilus;        /* index tabulae stilorum; 0 = nativus */
} Cellula;

nomen structura {
     Cellula* cellulae;  /* capacitas latitudinis */
         b32  involuta;  /* in lineam proximam continuat (refluxus) */
} Linea;

/* DECSC (Ghostty saveCursor: positio, stilus, involutio pendens) */
nomen structura {
    i32 x;
    i32 y;
    b32 pendens;
    i32 calamus;
    b32 adest;
} Servatus;

nomen structura {
               Linea** lineae;   /* indirectio: volutio indices rotat */
     AemulatorCursor   cursor;   /* visibilis: vide Aemulator */
                 i32   calamus;  /* stilus currens (index) */
            Servatus   servatus;
} Schirmum;

structura Aemulator {
              Piscina* piscina;
    AemulatorEffectus  effectus;
                  i32  latitudo;
                  i32  altitudo;
                  i32  capacitas_latitudinis;
                  i32  capacitas_altitudinis;
             Schirmum  primarium;
             Schirmum  alterum;
             Schirmum* activum;
         SeriesLector* lector;
                   i8  residuum[IV];      /* runa UTF-8 scissa */
                  i32  residuum_mensura;
                  i32  ignota;
                  b32  involutio;         /* DEC VII */
                  b32  visibilis;         /* DEC XXV */
                  b32  synchronia;        /* DEC MMXXVI */
     StilusTerminalis* stili;             /* [0] = nativus */
                  i32  numerus_stilorum;
                  i32* transitus;         /* collectio: vetus->novus */
};


/* ==================================================
 * Memoria
 * ================================================== */

interior vacuum
cellulam_vacare (
    Cellula* c,
        i32  stilus)
{
    memset(c, ZEPHYRUM, magnitudo(Cellula));
    c->stilus = stilus;
}

interior vacuum
lineam_vacare (
      Linea* l,
        i32  ab,
        i32  ad,
        i32  stilus)
{
    i32 x;

    per (x = ab; x < ad; x++)
    {
        cellulam_vacare(&l->cellulae[x], stilus);
    }
}

/* lineae et cellulae capacitatis datae, omnes vacuae; NIHIL si
 * piscina deficit */
interior Linea**
lineas_struere (
    Piscina* piscina,
        i32  capacitas_latitudinis,
        i32  capacitas_altitudinis)
{
       Linea** lineae;
       Linea*  sedes;
     Cellula*  cellulae;
         i32   y;

    lineae = (Linea**)piscina_conari_allocare(piscina,
        (memoriae_index)capacitas_altitudinis * magnitudo(Linea*));
    sedes = (Linea*)piscina_conari_allocare(piscina,
        (memoriae_index)capacitas_altitudinis * magnitudo(Linea));
    cellulae = (Cellula*)piscina_conari_allocare(piscina,
        (memoriae_index)capacitas_altitudinis
        * (memoriae_index)capacitas_latitudinis * magnitudo(Cellula));
    si (!lineae || !sedes || !cellulae)
    {
        redde NIHIL;
    }
    memset(cellulae, ZEPHYRUM, (memoriae_index)capacitas_altitudinis
        * (memoriae_index)capacitas_latitudinis * magnitudo(Cellula));
    per (y = ZEPHYRUM; y < capacitas_altitudinis; y++)
    {
        sedes[y].cellulae  = cellulae + y * capacitas_latitudinis;
        sedes[y].involuta  = FALSUM;
        lineae[y]          = &sedes[y];
    }
    redde lineae;
}


/* ==================================================
 * Stili internati
 * ================================================== */

/* collectio: stili quos cellulae, calami et servati tenent manent,
 * compacti; indices renumerantur. Nihil allocat. */
interior vacuum
stilos_colligere (
    Aemulator* a)
{
    Schirmum* schirma[II];
    Schirmum* s;
       Linea* l;
         i32  i;
         i32  k;
         i32  x;
         i32  y;
         i32  novus;

    schirma[ZEPHYRUM]  = &a->primarium;
    schirma[I]         = &a->alterum;
    per (i = ZEPHYRUM; i < a->numerus_stilorum; i++)
    {
        a->transitus[i] = (i32)-I;
    }
    a->transitus[ZEPHYRUM] = ZEPHYRUM;
    per (k = ZEPHYRUM; k < II; k++)
    {
        s                                  = schirma[k];
        a->transitus[s->calamus]           = ZEPHYRUM;
        a->transitus[s->servatus.calamus]  = ZEPHYRUM;
        per (y = ZEPHYRUM; y < a->altitudo; y++)
        {
            l = s->lineae[y];
            per (x = ZEPHYRUM; x < a->latitudo; x++)
            {
                a->transitus[l->cellulae[x].stilus] = ZEPHYRUM;
            }
        }
    }
    /* compactio: usitati ad initium, ordine servato */
    novus = I;
    per (i = I; i < a->numerus_stilorum; i++)
    {
        si (a->transitus[i] == (i32)-I)
        {
            perge;
        }
        a->stili[novus] = a->stili[i];
        a->transitus[i] = novus;
        novus++;
    }
    a->numerus_stilorum = novus;
    per (k = ZEPHYRUM; k < II; k++)
    {
        s                    = schirma[k];
        s->calamus           = a->transitus[s->calamus];
        s->servatus.calamus  = a->transitus[s->servatus.calamus];
        per (y = ZEPHYRUM; y < a->altitudo; y++)
        {
            l = s->lineae[y];
            per (x = ZEPHYRUM; x < a->latitudo; x++)
            {
                l->cellulae[x].stilus =
                    a->transitus[l->cellulae[x].stilus];
            }
        }
    }
}

/* index stili (0 = nativus); tabula plena post collectionem ->
 * nativus (degradatio, numquam ruina) */
interior i32
stilum_internare (
                     Aemulator* a,
     constans StilusTerminalis* st)
{
    i32 i;

    per (i = ZEPHYRUM; i < a->numerus_stilorum; i++)
    {
        si (stilus_aequalis(&a->stili[i], st))
        {
            redde i;
        }
    }
    si (a->numerus_stilorum >= STILI_MAXIMI)
    {
        stilos_colligere(a);
        si (a->numerus_stilorum >= STILI_MAXIMI)
        {
            redde ZEPHYRUM;
        }
    }
    a->stili[a->numerus_stilorum] = *st;
    redde a->numerus_stilorum++;
}

/* stilus cellulae deletae: solum fundus calami (Ghostty blankCell) */
interior i32
stilus_vacuus (
    Aemulator* a)
{
    StilusTerminalis st;

    stilus_nativus(&st);
    st.color_fundi = a->stili[a->activum->calamus].color_fundi;
    redde stilum_internare(a, &st);
}


/* ==================================================
 * Cursor et lineae
 * ================================================== */

/* linea deorsum; in ima linea schirmum sursum volvitur (linea summa
 * abit - scrollback: phasis C) */
interior vacuum
indicem_movere (
    Aemulator* a)
{
     Schirmum* s;
        Linea* summa;
          i32  y;

    s                  = a->activum;
    s->cursor.pendens  = FALSUM;
    si (s->cursor.y + I < a->altitudo)
    {
        s->cursor.y++;
        redde;
    }
    summa = s->lineae[ZEPHYRUM];
    per (y = ZEPHYRUM; y + I < a->altitudo; y++)
    {
        s->lineae[y] = s->lineae[y + I];
    }
    s->lineae[a->altitudo - I] = summa;
    lineam_vacare(summa, ZEPHYRUM, a->capacitas_latitudinis,
                  stilus_vacuus(a));
    summa->involuta = FALSUM;
}

/* involutio: linea currens in proximam continuat */
interior vacuum
lineam_involvere (
    Aemulator* a)
{
    Schirmum* s;

    s                                 = a->activum;
    s->lineae[s->cursor.y]->involuta  = VERUM;
    s->cursor.x                       = ZEPHYRUM;
    indicem_movere(a);
}

/* cellulam (x, y) scribere; dimidia graphematis lati quae
 * superscribitur vacatur (Ghostty: print over wide char / spacer) */
interior vacuum
cellulam_scribere (
      Aemulator* a,
            i32  x,
            i32  y,
    constans i8* octeti,
            i32  mensura,
            i32  latitudo)
{
       Linea* l;
     Cellula* c;
         i32  stilus;

    l       = a->activum->lineae[y];
    c       = &l->cellulae[x];
    stilus  = a->activum->calamus;
    si (c->latitudo == AEMULATOR_CAUDA && x > ZEPHYRUM)
    {
        cellulam_vacare(&l->cellulae[x - I], ZEPHYRUM);
    }
    si (c->latitudo == AEMULATOR_LATA && x + I < a->latitudo)
    {
        cellulam_vacare(&l->cellulae[x + I], ZEPHYRUM);
    }
    cellulam_vacare(c, stilus);
    memcpy(c->octeti, octeti, (memoriae_index)mensura);
    c->mensura   = (i8)mensura;
    c->latitudo  = (i8)(latitudo == II ? AEMULATOR_LATA
                                       : AEMULATOR_ANGUSTA);
    si (latitudo == II)
    {
        c = &l->cellulae[x + I];
        si (c->latitudo == AEMULATOR_LATA && x + II < a->latitudo)
        {
            cellulam_vacare(&l->cellulae[x + II], ZEPHYRUM);
        }
        cellulam_vacare(c, stilus);
        c->latitudo = (i8)AEMULATOR_CAUDA;
    }
}

/* runa una (Ghostty Terminal.print, sine 2027 et sine marginibus) */
interior vacuum
runam_imprimere (
    Aemulator* a,
          s32  runa)
{
    Schirmum* s;
         i32  latitudo;
          i8  octeti[IV];
         s32  mensura;

    s         = a->activum;
    latitudo  = runae_latitudo(runa);
    /* latitudo nulla (notae iungentes): graphemata v2 - abiciuntur */
    si (latitudo == ZEPHYRUM)
    {
        redde;
    }
    si (latitudo == II && a->latitudo < II)
    {
        latitudo = I;
    }
    si (s->cursor.pendens && a->involutio)
    {
        lineam_involvere(a);
    }
    /* lata in columna ultima: caput relinquitur, in proximam fluit */
    si (latitudo == II && s->cursor.x + I >= a->latitudo)
    {
        si (!a->involutio)
        {
            redde;
        }
        cellulam_vacare(&s->lineae[s->cursor.y]->cellulae[s->cursor.x],
                        ZEPHYRUM);
        s->lineae[s->cursor.y]->cellulae[s->cursor.x].latitudo =
            (i8)AEMULATOR_CAPUT;
        lineam_involvere(a);
    }
    mensura = utf8_codere(runa, octeti);
    si (mensura <= ZEPHYRUM)
    {
        mensura = utf8_codere(RUNA_SUBSTITUTA, octeti);
    }
    cellulam_scribere(a, s->cursor.x, s->cursor.y, octeti, (i32)mensura,
                      latitudo);
    si (s->cursor.x + latitudo >= a->latitudo)
    {
        s->cursor.x        = a->latitudo - I;
        s->cursor.pendens  = VERUM;
    }
    alioquin
    {
        s->cursor.x += latitudo;
    }
}

/* cursus IMPRIMERE: UTF-8 in runas; runa in fine scissa servatur */
interior vacuum
cursum_imprimere (
    Aemulator* a,
       chorda  textus)
{
    constans i8* p;
    constans i8* finis;
    constans i8* initium;
             i8  iuncta[IV];
            s32  longitudo;
            s32  runa;
            i32  deest;
            i32  i;

    p      = textus.datum;
    finis  = textus.datum + textus.mensura;
    /* runa scissa vocationis prioris perficitur */
    si (a->residuum_mensura > ZEPHYRUM)
    {
        longitudo  = utf8_longitudo_byte(a->residuum[ZEPHYRUM]);
        deest      = (i32)longitudo - a->residuum_mensura;
        si ((i32)(finis - p) < deest)
        {
            per (i = ZEPHYRUM; p < finis; i++, p++)
            {
                a->residuum[a->residuum_mensura++] = *p;
            }
            redde;
        }
        memcpy(iuncta, a->residuum, a->residuum_mensura);
        memcpy(iuncta + a->residuum_mensura, p, deest);
        p += deest;
        initium = iuncta;
        runa = utf8_decodere(&initium, iuncta + longitudo);
        a->residuum_mensura = ZEPHYRUM;
        runam_imprimere(a, runa < ZEPHYRUM ? RUNA_SUBSTITUTA : runa);
    }
    dum (p < finis)
    {
        longitudo = utf8_longitudo_byte(*p);
        si (longitudo == ZEPHYRUM)
        {
            runam_imprimere(a, RUNA_SUBSTITUTA);
            p++;
            perge;
        }
        si ((i32)(finis - p) < (i32)longitudo)
        {
            per (i = ZEPHYRUM; p < finis; i++, p++)
            {
                a->residuum[i] = *p;
            }
            a->residuum_mensura = i;
            redde;
        }
        initium  = p;
        runa     = utf8_decodere(&p, finis);
        si (runa < ZEPHYRUM)
        {
            p     = initium + I;
            runa  = RUNA_SUBSTITUTA;
        }
        runam_imprimere(a, runa);
    }
}

/* C0 (Ghostty stream: execute) */
interior vacuum
regimen_exsequi (
    Aemulator* a,
           i8  octetus)
{
    Schirmum* s;

    s = a->activum;
    commutatio (octetus)
    {
        casus 0x07:
            si (a->effectus.campana)
            {
                a->effectus.campana(a->effectus.datum);
            }
            frange;
        casus 0x08:
            s->cursor.pendens = FALSUM;
            si (s->cursor.x > ZEPHYRUM)
            {
                s->cursor.x--;
            }
            frange;
        casus 0x09:
            s->cursor.x = (s->cursor.x / TABULATIO + I) * TABULATIO;
            si (s->cursor.x >= a->latitudo)
            {
                s->cursor.x = a->latitudo - I;
            }
            frange;
        casus 0x0A:
        casus 0x0B:
        casus 0x0C:
            indicem_movere(a);
            frange;
        casus 0x0D:
            s->cursor.x        = ZEPHYRUM;
            s->cursor.pendens  = FALSUM;
            frange;
        ordinarius:
            frange;
    }
}


/* ==================================================
 * Series (A2)
 * ================================================== */

/* parametrum i; absens aut 0 -> praestitutum (motus: 0 = 1) */
interior i32
parametrum (
    constans SeriesLexema* lx,
                      i32  i,
                      i32  praestitutum)
{
    si (i >= lx->numerus_parametrorum || lx->parametra[i] <= ZEPHYRUM)
    {
        redde praestitutum;
    }
    redde (i32)lx->parametra[i];
}

/* cursor absolutus (0-based), intra schirmum; pendens tollitur */
interior vacuum
cursorem_ponere (
    Aemulator* a,
          i32  x,
          i32  y)
{
    Schirmum* s;

    s                  = a->activum;
    s->cursor.x        = x < a->latitudo ? x : a->latitudo - I;
    s->cursor.y        = y < a->altitudo ? y : a->altitudo - I;
    s->cursor.pendens  = FALSUM;
}

/* EL - Ghostty eraseLine: 'right' a cauda incipit (lata tota), 'left'
 * caudam latae sub cursore includit; right et complete involutionem
 * lineae tollunt; pendens semper tollitur */
interior vacuum
lineam_delere (
    Aemulator* a,
          i32  modus)
{
    Schirmum* s;
       Linea* l;
         i32  ab;
         i32  ad;

    s = a->activum;
    l = s->lineae[s->cursor.y];
    commutatio (modus)
    {
        casus ZEPHYRUM:
            ab = s->cursor.x;
            si (   ab > ZEPHYRUM
                && l->cellulae[ab].latitudo == AEMULATOR_CAUDA)
            {
                ab--;
            }
            ad           = a->latitudo;
            l->involuta  = FALSUM;
            frange;
        casus I:
            ab = ZEPHYRUM;
            ad = s->cursor.x + I;
            si (   l->cellulae[s->cursor.x].latitudo == AEMULATOR_LATA
                && ad < a->latitudo)
            {
                ad++;
            }
            frange;
        casus II:
            ab           = ZEPHYRUM;
            ad           = a->latitudo;
            l->involuta  = FALSUM;
            frange;
        ordinarius:
            a->ignota++;
            redde;
    }
    s->cursor.pendens = FALSUM;
    lineam_vacare(l, ab, ad, stilus_vacuus(a));
}

/* lineas [ab, ad) totas delere */
interior vacuum
lineas_delere (
    Aemulator* a,
          i32  ab,
          i32  ad)
{
    i32 y;
    i32 stilus;

    stilus = stilus_vacuus(a);
    per (y = ab; y < ad; y++)
    {
        lineam_vacare(a->activum->lineae[y], ZEPHYRUM, a->latitudo,
                      stilus);
        a->activum->lineae[y]->involuta = FALSUM;
    }
}

/* ED - Ghostty eraseDisplay: below = EL right + lineae infra; above =
 * EL left + lineae supra; complete = omnes; 3 (scrollback) nihil
 * hic (phasis C) */
interior vacuum
schirmum_delere (
    Aemulator* a,
          i32  modus)
{
    Schirmum* s;

    s = a->activum;
    commutatio (modus)
    {
        casus ZEPHYRUM:
            lineam_delere(a, ZEPHYRUM);
            lineas_delere(a, s->cursor.y + I, a->altitudo);
            frange;
        casus I:
            lineam_delere(a, I);
            lineas_delere(a, ZEPHYRUM, s->cursor.y);
            frange;
        casus II:
            lineas_delere(a, ZEPHYRUM, a->altitudo);
            s->cursor.pendens = FALSUM;
            frange;
        casus III:
            frange;
        ordinarius:
            a->ignota++;
            frange;
    }
}

interior vacuum
cursorem_servare (
    Aemulator* a)
{
    Schirmum* s;

    s                    = a->activum;
    s->servatus.x        = s->cursor.x;
    s->servatus.y        = s->cursor.y;
    s->servatus.pendens  = s->cursor.pendens;
    s->servatus.calamus  = s->calamus;
    s->servatus.adest    = VERUM;
}

/* Ghostty restoreCursor: sine servatione = initium et stilus nativus;
 * positio ad schirmum currens praeciditur */
interior vacuum
cursorem_restituere (
    Aemulator* a)
{
    Schirmum* s;

    s = a->activum;
    si (!s->servatus.adest)
    {
        s->calamus = ZEPHYRUM;
        cursorem_ponere(a, ZEPHYRUM, ZEPHYRUM);
        redde;
    }
    s->calamus = s->servatus.calamus;
    cursorem_ponere(a, s->servatus.x, s->servatus.y);
    s->cursor.pendens = s->servatus.pendens;
}

/* 1049 (Ghostty switchScreenMode, ex xterm charproc.c): ingressus
 * servat cursorem, alterum delet, cursorem (et calamum) transfert;
 * exitus primarium cum cursore servato restituit */
interior vacuum
alterum_ponere (
    Aemulator* a,
          b32  ingredi)
{
    Schirmum* prior;

    si (ingredi)
    {
        cursorem_servare(a);
        prior       = a->activum;
        a->activum  = &a->alterum;
        si (prior != &a->alterum)
        {
            a->alterum.cursor   = prior->cursor;
            a->alterum.calamus  = prior->calamus;
        }
        schirmum_delere(a, II);
        redde;
    }
    a->activum = &a->primarium;
    cursorem_restituere(a);
}

/* DECSET / DECRST */
interior vacuum
modum_privatum_ponere (
    Aemulator* a,
          i32  numerus,
          b32  status)
{
    commutatio (numerus)
    {
        casus VII:
            a->involutio = status;
            frange;
        casus XXV:
            a->visibilis = status;
            frange;
        casus MXLIX:
            alterum_ponere(a, status);
            frange;
        casus MMXXVI:
            a->synchronia = status;
            frange;
        ordinarius:
            a->ignota++;
            frange;
    }
}

interior vacuum
seriem_csi (
                 Aemulator* a,
     constans SeriesLexema* lx)
{
            Schirmum* s;
    StilusTerminalis  st;
                 i32  n;
                 i32  i;

    s = a->activum;
    /* intermedia aut ':' extra SGR: non nostra (DECRQM, DECSCUSR...) */
    si (   lx->numerus_intermediorum > ZEPHYRUM
        || (lx->separatores != ZEPHYRUM && lx->finale != 'm'))
    {
        a->ignota++;
        redde;
    }
    si (lx->privatum == '?')
    {
        si (lx->finale != 'h' && lx->finale != 'l')
        {
            a->ignota++;
            redde;
        }
        per (i = ZEPHYRUM; i < lx->numerus_parametrorum; i++)
        {
            modum_privatum_ponere(a, (i32)lx->parametra[i],
                                  lx->finale == 'h');
        }
        redde;
    }
    si (lx->privatum != ZEPHYRUM)
    {
        a->ignota++;
        redde;
    }
    n = parametrum(lx, ZEPHYRUM, I);
    commutatio (lx->finale)
    {
        casus 'H':
        casus 'f':
            cursorem_ponere(a, parametrum(lx, I, I) - I, n - I);
            frange;
        casus 'A':
            cursorem_ponere(a, s->cursor.x,
                s->cursor.y > n ? s->cursor.y - n : ZEPHYRUM);
            frange;
        casus 'B':
            cursorem_ponere(a, s->cursor.x, s->cursor.y + n);
            frange;
        casus 'C':
            cursorem_ponere(a, s->cursor.x + n, s->cursor.y);
            frange;
        casus 'D':
            cursorem_ponere(a,
                s->cursor.x > n ? s->cursor.x - n : ZEPHYRUM,
                s->cursor.y);
            frange;
        casus 'E':
            cursorem_ponere(a, ZEPHYRUM, s->cursor.y + n);
            frange;
        casus 'F':
            cursorem_ponere(a, ZEPHYRUM,
                s->cursor.y > n ? s->cursor.y - n : ZEPHYRUM);
            frange;
        casus 'G':
        casus '`':
            cursorem_ponere(a, n - I, s->cursor.y);
            frange;
        casus 'd':
            cursorem_ponere(a, s->cursor.x, n - I);
            frange;
        casus 'J':
            schirmum_delere(a, parametrum(lx, ZEPHYRUM, ZEPHYRUM));
            frange;
        casus 'K':
            lineam_delere(a, parametrum(lx, ZEPHYRUM, ZEPHYRUM));
            frange;
        casus 'm':
            st = a->stili[s->calamus];
            a->ignota  += stilus_applicare(lx, &st);
            s->calamus  = stilum_internare(a, &st);
            frange;
        ordinarius:
            a->ignota++;
            frange;
    }
}

interior vacuum
seriem_esc (
                 Aemulator* a,
     constans SeriesLexema* lx)
{
    si (lx->numerus_intermediorum == ZEPHYRUM && lx->finale == '7')
    {
        cursorem_servare(a);
    }
    alioquin si (   lx->numerus_intermediorum == ZEPHYRUM
                 && lx->finale                == '8')
    {
        cursorem_restituere(a);
    }
    alioquin
    {
        a->ignota++;
    }
}


/* ==================================================
 * Vita
 * ================================================== */

vacuum
aemulator_configuratio_initiare (
    AemulatorConfiguratio* cfg)
{
    si (!cfg)
    {
        redde;
    }
    memset(cfg, ZEPHYRUM, magnitudo(AemulatorConfiguratio));
    cfg->latitudo = LATITUDO_ORDINARIA;
    cfg->altitudo = ALTITUDO_ORDINARIA;
}

Aemulator*
aemulator_creare (
                   Piscina* piscina,
    constans AemulatorConfiguratio* cfg)
{
    Aemulator* a;

    si (   !piscina || !cfg
        || cfg->latitudo < I || cfg->latitudo > LATUS_MAXIMUM
        || cfg->altitudo < I || cfg->altitudo > LATUS_MAXIMUM)
    {
        redde NIHIL;
    }
    a = (Aemulator*)piscina_conari_allocare(piscina,
        magnitudo(Aemulator));
    si (!a)
    {
        redde NIHIL;
    }
    memset(a, ZEPHYRUM, magnitudo(Aemulator));
    a->piscina                = piscina;
    a->effectus               = cfg->effectus;
    a->latitudo               = cfg->latitudo;
    a->altitudo               = cfg->altitudo;
    a->capacitas_latitudinis  = cfg->latitudo;
    a->capacitas_altitudinis  = cfg->altitudo;
    a->involutio              = VERUM;
    a->visibilis              = VERUM;
    a->activum                = &a->primarium;
    a->primarium.lineae = lineas_struere(piscina, cfg->latitudo,
                                         cfg->altitudo);
    a->alterum.lineae = lineas_struere(piscina, cfg->latitudo,
                                       cfg->altitudo);
    a->stili = (StilusTerminalis*)piscina_conari_allocare(piscina,
        STILI_MAXIMI * magnitudo(StilusTerminalis));
    a->transitus = (i32*)piscina_conari_allocare(piscina,
        STILI_MAXIMI * magnitudo(i32));
    a->lector = series_lectorem_creare(piscina);
    si (   !a->primarium.lineae || !a->alterum.lineae || !a->stili
        || !a->transitus || !a->lector)
    {
        redde NIHIL;
    }
    stilus_nativus(&a->stili[ZEPHYRUM]);
    a->numerus_stilorum = I;
    redde a;
}

vacuum
aemulator_scribere (
       Aemulator* a,
     constans i8* octeti,
             i32  n)
{
     constans i8* p;
    SeriesLexema  lexema;
     SeriesGenus  genus;

    si (!a || !octeti)
    {
        redde;
    }
    p = octeti;
    dum (p < octeti + n)
    {
        genus = series_lexema_proximum(a->lector, &p, octeti + n,
                                       &lexema);
        si (genus == SERIES_NIHIL)
        {
            frange;
        }
        /* runa scissa quam series alia interrumpit: substituta */
        si (genus != SERIES_IMPRIMERE && a->residuum_mensura > ZEPHYRUM)
        {
            a->residuum_mensura = ZEPHYRUM;
            runam_imprimere(a, RUNA_SUBSTITUTA);
        }
        commutatio (genus)
        {
            casus SERIES_IMPRIMERE:
                cursum_imprimere(a, lexema.textus);
                frange;
            casus SERIES_EXSEQUI:
                regimen_exsequi(a, lexema.finale);
                frange;
            casus SERIES_CSI:
                seriem_csi(a, &lexema);
                frange;
            casus SERIES_ESC:
                seriem_esc(a, &lexema);
                frange;
            ordinarius:
                a->ignota++;
                frange;
        }
    }
}

/* schirmum unum ad magnitudinem novam (intra capacitatem) */
interior vacuum
schirmum_aptare (
    Aemulator* a,
     Schirmum* s,
          i32  latitudo,
          i32  altitudo)
{
    Schirmum* activum;
         i32  y;
         i32  translatio;

    /* cursor in schirmo manet: lineae summae abeunt (refluxus dilatus
     * - decisio VII); volutio per indicem_movere schirmi huius */
    si (s->cursor.y >= altitudo)
    {
        activum     = a->activum;
        a->activum  = s;
        translatio  = s->cursor.y - altitudo + I;
        per (y = ZEPHYRUM; y < translatio; y++)
        {
            s->cursor.y = a->altitudo - I;
            indicem_movere(a);
        }
        s->cursor.y  = altitudo - I;
        a->activum   = activum;
    }
    si (latitudo > a->latitudo)
    {
        per (y = ZEPHYRUM; y < a->altitudo; y++)
        {
            lineam_vacare(s->lineae[y], a->latitudo, latitudo,
                          ZEPHYRUM);
        }
    }
    per (y = a->altitudo; y < altitudo; y++)
    {
        lineam_vacare(s->lineae[y], ZEPHYRUM, a->capacitas_latitudinis,
                      ZEPHYRUM);
        s->lineae[y]->involuta = FALSUM;
    }
    si (s->cursor.x >= latitudo)
    {
        s->cursor.x = latitudo - I;
    }
    s->cursor.pendens = FALSUM;
}

b32
aemulator_amplitudo (
    Aemulator* a,
          i32  latitudo,
          i32  altitudo)
{
       Linea** primae;
       Linea** alterae;
         i32   cap_lat;
         i32   cap_alt;
         i32   y;

    si (   !a || latitudo < I || latitudo > LATUS_MAXIMUM
        || altitudo < I || altitudo > LATUS_MAXIMUM)
    {
        redde FALSUM;
    }
    /* ultra capacitatem: crescit geometrice, numquam minuitur */
    si (   latitudo > a->capacitas_latitudinis
        || altitudo > a->capacitas_altitudinis)
    {
        cap_lat = a->capacitas_latitudinis;
        cap_alt = a->capacitas_altitudinis;
        si (latitudo > cap_lat)
        {
            cap_lat = cap_lat + cap_lat / II;
            si (cap_lat < latitudo)
            {
                cap_lat = latitudo;
            }
        }
        si (altitudo > cap_alt)
        {
            cap_alt = cap_alt + cap_alt / II;
            si (cap_alt < altitudo)
            {
                cap_alt = altitudo;
            }
        }
        si (cap_lat > LATUS_MAXIMUM)
        {
            cap_lat = LATUS_MAXIMUM;
        }
        si (cap_alt > LATUS_MAXIMUM)
        {
            cap_alt = LATUS_MAXIMUM;
        }
        primae   = lineas_struere(a->piscina, cap_lat, cap_alt);
        alterae  = lineas_struere(a->piscina, cap_lat, cap_alt);
        si (!primae || !alterae)
        {
            redde FALSUM;
        }
        per (y = ZEPHYRUM; y < a->altitudo; y++)
        {
            memcpy(primae[y]->cellulae,
                a->primarium.lineae[y]->cellulae,
                (memoriae_index)a->latitudo * magnitudo(Cellula));
            primae[y]->involuta = a->primarium.lineae[y]->involuta;
            memcpy(alterae[y]->cellulae, a->alterum.lineae[y]->cellulae,
                (memoriae_index)a->latitudo * magnitudo(Cellula));
            alterae[y]->involuta = a->alterum.lineae[y]->involuta;
        }
        a->primarium.lineae       = primae;
        a->alterum.lineae         = alterae;
        a->capacitas_latitudinis  = cap_lat;
        a->capacitas_altitudinis  = cap_alt;
    }
    schirmum_aptare(a, &a->primarium, latitudo, altitudo);
    schirmum_aptare(a, &a->alterum, latitudo, altitudo);
    a->latitudo = latitudo;
    a->altitudo = altitudo;
    redde VERUM;
}


/* ==================================================
 * Lectio
 * ================================================== */

i32
aemulator_latitudo (
    constans Aemulator* a)
{
    redde a ? a->latitudo : ZEPHYRUM;
}

i32
aemulator_altitudo (
    constans Aemulator* a)
{
    redde a ? a->altitudo : ZEPHYRUM;
}

AemulatorCursor
aemulator_cursor (
    constans Aemulator* a)
{
    AemulatorCursor c;

    si (!a)
    {
        memset(&c, ZEPHYRUM, magnitudo(AemulatorCursor));
        redde c;
    }
    c            = a->activum->cursor;
    c.visibilis  = a->visibilis;
    redde c;
}

b32
aemulator_cellula (
    constans Aemulator* a,
                   i32  x,
                   i32  y,
      AemulatorCellula* cellula)
{
    Cellula* c;

    si (!a || !cellula || x >= a->latitudo || y >= a->altitudo)
    {
        redde FALSUM;
    }
    c                          = &a->activum->lineae[y]->cellulae[x];
    cellula->graphema.datum    = c->octeti;
    cellula->graphema.mensura  = (i32)c->mensura;
    cellula->latitudo          = (AemulatorLatitudo)c->latitudo;
    cellula->stilus            = a->stili[c->stilus];
    redde VERUM;
}

b32
aemulator_alterum (
    constans Aemulator* a)
{
    redde a ? a->activum == &a->alterum : FALSUM;
}

b32
aemulator_modus (
    constans Aemulator* a,
                   i32  numerus,
                   b32  privatus)
{
    si (!a || !privatus)
    {
        redde FALSUM;
    }
    commutatio (numerus)
    {
        casus VII:
            redde a->involutio;
        casus XXV:
            redde a->visibilis;
        casus MXLIX:
            redde a->activum == &a->alterum;
        casus MMXXVI:
            redde a->synchronia;
        ordinarius:
            redde FALSUM;
    }
}

i32
aemulator_ignota (
    constans Aemulator* a)
{
    redde a ? a->ignota : ZEPHYRUM;
}

/* Ghostty formatter (plain, unwrap=false, trim=false): vacuae ante
 * textum spatia, scripta spatia manent, caudae et capita omittuntur,
 * lineae sine textu solum ante lineam cum textu */
chorda
aemulator_textum_effundere (
    constans Aemulator* a,
               Piscina* piscina)
{
      chorda  ex;
          i8* b;
         i32  n;
         i32  x;
         i32  y;
         i32  k;
         i32  lineae_vacuae;
         i32  cellulae_vacuae;
         b32  scriptum;
     Cellula* c;
       Linea* l;

    ex.datum    = NIHIL;
    ex.mensura  = ZEPHYRUM;
    si (!a || !piscina)
    {
        redde ex;
    }
    b = (i8*)piscina_conari_allocare(piscina,
        (memoriae_index)a->altitudo
        * ((memoriae_index)a->latitudo * IV + I) + I);
    si (!b)
    {
        redde ex;
    }
    n              = ZEPHYRUM;
    lineae_vacuae  = ZEPHYRUM;
    scriptum       = FALSUM;
    per (y = ZEPHYRUM; y < a->altitudo; y++)
    {
        l = a->activum->lineae[y];
        /* linea sine textu: numeratur */
        k = ZEPHYRUM;
        per (x = ZEPHYRUM; x < a->latitudo; x++)
        {
            si (l->cellulae[x].mensura > ZEPHYRUM)
            {
                k = I;
                frange;
            }
        }
        si (k == ZEPHYRUM)
        {
            lineae_vacuae++;
            perge;
        }
        per (k = ZEPHYRUM; k < lineae_vacuae
            + (scriptum ? I : ZEPHYRUM);
             k++)
        {
            b[n++] = '\n';
        }
        lineae_vacuae    = ZEPHYRUM;
        scriptum         = VERUM;
        cellulae_vacuae  = ZEPHYRUM;
        per (x = ZEPHYRUM; x < a->latitudo; x++)
        {
            c = &l->cellulae[x];
            si (   c->latitudo == AEMULATOR_CAUDA
                || c->latitudo == AEMULATOR_CAPUT)
            {
                perge;
            }
            si (c->mensura == ZEPHYRUM)
            {
                cellulae_vacuae++;
                perge;
            }
            per (k = ZEPHYRUM; k < cellulae_vacuae; k++)
            {
                b[n++] = ' ';
            }
            cellulae_vacuae = ZEPHYRUM;
            memcpy(b + n, c->octeti, (memoriae_index)c->mensura);
            n += (i32)c->mensura;
        }
    }
    ex.datum    = b;
    ex.mensura  = n;
    redde ex;
}
