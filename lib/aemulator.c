/* aemulator.c - nucleus emulatoris terminalis (aemulator-plan A1, A2)
 *
 * A1: creatio, cellulae et lineae, impressio per runae (angusta, lata,
 * caudae, caput, involutio pendens), CR LF BS HT BEL, mutatio
 * magnitudinis, effusio plana.
 * A2: CUP et motus relativi, ED/EL (cellulae deletae fundum calami
 * servant), SGR per stilus_applicare cum stilis internatis (tabula
 * fixa; plena -> collectio et compactio), DECTCEM, 1049 (schirmum
 * alterum, cursor servatus), 2026, DECSC/DECRC.
 * B1: regio volutionis (DECSTBM) et quae in ea volvunt (IND RI NEL
 * SU SD, LF et involutio), IL DL ICH DCH ECH, tabulationes (HT HTS
 * TBC CHT CBT), LNM.
 * B2: responsa (DA1/DA2/DA3, DSR 5, CPR, XTVERSION) per effectum
 * 'responsum'; OSC 0/2 per effectum 'titulus'. Series ceterae
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

/* HISTORIA (phasis C, decisiones XIX-XX): paginae magnitudinis fixae
 * (cellulae PAGINA_OCTETI), lineae unius latitudinis, tabula stilorum
 * PROPRIA - collectio stilorum schirmi historiam numquam tangit; stili
 * cum pagina vivunt et cum ea evincuntur (exemplar Ghostty page). */
#define PAGINA_OCTETI      (LXIV * MXXIV)
#define PAGINA_LINEAE_MAX  (PAGINA_OCTETI / (i32)magnitudo(Cellula))
#define PAGINA_STILI       CXXVIII

nomen structura {
                 i32  latitudo;    /* cellulae per lineam */
                 i32  capacitas;   /* lineae */
                 i32  numerus;     /* lineae scriptae */
             Cellula* cellulae;    /* PAGINA_OCTETI */
                  i8* involutae;   /* PAGINA_LINEAE_MAX */
    StilusTerminalis* stili;       /* PAGINA_STILI; [0] = nativus */
                 i32  numerus_stilorum;
} Pagina;

/* MODI (D2, Ghostty modes.zig): bitum unum per modum honoratum.
 * SM/RM et DECSET/DECRST bitum ponunt, deinde effectus sequuntur;
 * DECRQM bitum nuntiat. Modus extra tabulam = ignotus (DECRQM 0). */
nomen enumeratio {
    MODUS_INSERTIO = ZEPHYRUM,  /* ANSI IV (IRM) */
    MODUS_LINEA_NOVA,           /* ANSI XX (LNM) */
    MODUS_SAGITTAE,             /* I (DECCKM) */
    MODUS_ORIGO,                /* VI (DECOM) */
    MODUS_INVOLUTIO,            /* VII (DECAWM) */
    MODUS_MUS_X10,              /* IX */
    MODUS_VISIBILIS,            /* XXV (DECTCEM) */
    MODUS_RETRO,                /* XLV: involutio retro */
    MODUS_ALTERUM_VETUS,        /* XLVII */
    MODUS_TABULA,               /* LXVI (DECNKM; ESC = / ESC >) */
    MODUS_MUS_PRESSIO,          /* M */
    MODUS_MUS_TRACTUS,          /* MII */
    MODUS_MUS_OMNIS,            /* MIII */
    MODUS_FOCUS,                /* MIV */
    MODUS_FORMA_UTF8,           /* MV */
    MODUS_FORMA_SGR,            /* MVI */
    MODUS_FORMA_URXVT,          /* MXV */
    MODUS_FORMA_SGR_PIXELA,     /* MXVI */
    MODUS_RETRO_PLENUS,         /* MXLV: retro etiam trans summam */
    MODUS_ALTERUM,              /* MXLVII */
    MODUS_CURSOR_SERVATUS,      /* MXLVIII */
    MODUS_ALTERUM_SERVATUM,     /* MXLIX */
    MODUS_GLUTINUM,             /* MMIV */
    MODUS_SYNCHRONIA,           /* MMXXVI */
    MODI_NUMERUS
} ModusIndex;

nomen structura {
    i32 numerus;
    b32 privatus;
    b32 initio;       /* valor post creationem et RIS */
} ModusDescriptio;

interior constans ModusDescriptio modi_tabula[MODI_NUMERUS] = {
    { IV,      FALSUM, FALSUM },
    { XX,      FALSUM, FALSUM },
    { I,       VERUM,  FALSUM },
    { VI,      VERUM,  FALSUM },
    { VII,     VERUM,  VERUM  },
    { IX,      VERUM,  FALSUM },
    { XXV,     VERUM,  VERUM  },
    { XLV,     VERUM,  FALSUM },
    { XLVII,   VERUM,  FALSUM },
    { LXVI,    VERUM,  FALSUM },
    { M,       VERUM,  FALSUM },
    { MII,     VERUM,  FALSUM },
    { MIII,    VERUM,  FALSUM },
    { MIV,     VERUM,  FALSUM },
    { MV,      VERUM,  FALSUM },
    { MVI,     VERUM,  FALSUM },
    { MXV,     VERUM,  FALSUM },
    { MXVI,    VERUM,  FALSUM },
    { MXLV,    VERUM,  FALSUM },
    { MXLVII,  VERUM,  FALSUM },
    { MXLVIII, VERUM,  FALSUM },
    { MXLIX,   VERUM,  FALSUM },
    { MMIV,    VERUM,  FALSUM },
    { MMXXVI,  VERUM,  FALSUM }
};

/* COPIAE CHARACTERUM (D3, Ghostty charsets.zig): G0-G3 copias tenent,
 * GL imprimit; GR servatur sed nondum adhibetur (octeti alti UTF-8
 * sunt, Ghostty idem). SS2/SS3 copiam G2/G3 pro charactere UNO
 * invocant. Status unus pro aemulatore: Ghostty eum in mutatione
 * schirmi copiat, ergo idem est. */
nomen enumeratio {
    COPIA_UTF8 = ZEPHYRUM,      /* ordinaria: runa ipsa */
    COPIA_ASCII,                /* ESC ( B */
    COPIA_BRITANNICA,           /* ESC ( A: '#' -> libra */
    COPIA_GRAPHICA              /* ESC ( 0: DEC Special Graphics */
} Copia;

#define COPIA_SEMEL_NULLA  IV  /* nulla invocatio singularis */

nomen structura {
    i8 copiae[IV];  /* G0..G3 (Copia) */
    i8 sinistra;    /* GL: index G */
    i8 dextra;      /* GR: index G (servatur) */
    i8 semel;       /* SS2/SS3: index G pro charactere uno */
} Copiae;

/* DECSC (Ghostty saveCursor: positio, stilus, involutio pendens,
 * DECOM, copiae characterum) */
nomen structura {
       i32 x;
       i32 y;
       b32 pendens;
       i32 calamus;
       b32 origo;
    Copiae copiae;
       b32 adest;
} Servatus;

nomen structura {
               Linea** lineae;   /* indirectio: volutio indices rotat */
     AemulatorCursor   cursor;   /* visibilis: vide Aemulator */
                 i32   calamus;  /* stilus currens (index) */
            Servatus   servatus;
} Schirmum;

structura Aemulator {
              Piscina*  piscina;
    AemulatorEffectus   effectus;
                  i32   latitudo;
                  i32   altitudo;
                  i32   capacitas_latitudinis;
                  i32   capacitas_altitudinis;
             Schirmum   primarium;
             Schirmum   alterum;
             Schirmum*  activum;
         SeriesLector*  lector;
                   i8   residuum[IV];      /* runa UTF-8 scissa */
                  i32   residuum_mensura;
                  i32   ignota;
                   i8   modi[MODI_NUMERUS]; /* bita cruda (D2) */
         AemulatorMus   mus;               /* ?9 / 1000 / 1002 / 1003 */
    AemulatorMusForma   mus_forma;         /* ?1005/1006/1015/1016 */
                  i32   regio_summa;       /* DECSTBM (0-based) */
                  i32   regio_ultima;
                   i8*  tabulae;           /* sistae per columnam */
               chorda   identitas;         /* XTVERSION */
               Pagina** paginae;           /* anulus historiae */
                  i32   paginae_maximae;   /* 0 = nulla historia */
                  i32   paginae_numerus;
                  i32   paginae_initium;
                  i32   historia_lineae;
                  i32   visus;             /* lineae supra vivum (C3) */
                  s32   runa_prior;       /* REP; 0 = nulla (RIS) */
               Copiae   copiae;            /* D3 */
                  b32   lectio_schirmi;    /* DECRQCRA permissa */
     StilusTerminalis*  stili;             /* [0] = nativus */
                  i32   numerus_stilorum;
                  i32*  transitus;         /* collectio: vetus->novus */
};

interior b32
modus_est (
    constans Aemulator* a,
                   i32  modus)
{
    redde a->modi[modus] != ZEPHYRUM;
}

/* index tabulae modorum; -I = ignotus */
interior s32
modum_invenire (
    i32 numerus,
    b32 privatus)
{
    s32 k;

    per (k = ZEPHYRUM; k < MODI_NUMERUS; k++)
    {
        si (   modi_tabula[k].numerus   == numerus
            && !modi_tabula[k].privatus == !privatus)
        {
            redde k;
        }
    }
    redde -I;
}

/* modi ordinarii (creatio, RIS) */
interior vacuum
modos_initiare (
    Aemulator* a)
{
    i32 k;

    per (k = ZEPHYRUM; k < MODI_NUMERUS; k++)
    {
        a->modi[k] = (i8)(modi_tabula[k].initio ? I : ZEPHYRUM);
    }
    a->mus        = AEMULATOR_MUS_NULLUS;
    a->mus_forma  = AEMULATOR_MUS_FORMA_X10;
}

/* copiae ordinariae (creatio, RIS, DECSTR, DECRC sine servatione):
 * G0-G3 UTF-8, GL = G0, GR = G2 (Ghostty CharsetState) */
interior vacuum
copias_initiare (
    Copiae* c)
{
    memset(c, ZEPHYRUM, magnitudo(Copiae));
    c->dextra  = (i8)II;
    c->semel   = (i8)COPIA_SEMEL_NULLA;
}

/* DEC Special Graphics 0x60..0x7E (Ghostty charsets.zig dec_special;
 * vt100.net) */
interior constans s32 graphica[XXXI] = {
    0x25C6, 0x2592, 0x2409, 0x240C, 0x240D, 0x240A, 0x00B0, 0x00B1,
    0x2424, 0x240B, 0x2518, 0x2510, 0x250C, 0x2514, 0x253C, 0x23BA,
    0x23BB, 0x2500, 0x23BC, 0x23BD, 0x251C, 0x2524, 0x2534, 0x252C,
    0x2502, 0x2264, 0x2265, 0x03C0, 0x2260, 0x00A3, 0x00B7
};

/* Ghostty printCell: copia GL (aut semel invocata, quae consumitur)
 * runam vertit; UTF-8 et ASCII runam ipsam reddunt, ceterae extra
 * octetum spatium */
interior s32
runam_vertere (
    Aemulator* a,
          s32  runa)
{
    i32 g;
    i32 copia;

    g = (i32)a->copiae.sinistra;
    si (a->copiae.semel != (i8)COPIA_SEMEL_NULLA)
    {
        g                = (i32)a->copiae.semel;
        a->copiae.semel  = (i8)COPIA_SEMEL_NULLA;
    }
    copia = (i32)a->copiae.copiae[g];
    si (copia == COPIA_UTF8 || copia == COPIA_ASCII)
    {
        redde runa;
    }
    si (runa > 0xFF)
    {
        redde ' ';
    }
    si (copia == COPIA_BRITANNICA)
    {
        redde runa == '#' ? 0xA3 : runa;
    }
    si (runa >= 0x60 && runa <= 0x7E)
    {
        redde graphica[runa - 0x60];
    }
    redde runa;
}


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


/* ==================================================
 * Historia (phasis C)
 * ================================================== */

interior Pagina*
paginam_ad (
    constans Aemulator* a,
                   i32  i)
{
    redde a->paginae[(a->paginae_initium + i) % a->paginae_maximae];
}

interior vacuum
paginam_parare (
    Pagina* pg,
        i32  latitudo)
{
    pg->latitudo          = latitudo;
    pg->capacitas         = PAGINA_OCTETI
                          / (latitudo * (i32)magnitudo(Cellula));
    pg->numerus           = ZEPHYRUM;
    pg->numerus_stilorum  = I;
    stilus_nativus(&pg->stili[ZEPHYRUM]);
}

/* pagina nova pro latitudine currente: ex piscina dum infra limitem,
 * aliter vetustissima recyclatur (decisio XX). NIHIL solum si nulla
 * pagina exstat et piscina deficit (linea tunc perit). */
interior Pagina*
paginam_novam (
    Aemulator* a)
{
    Pagina* pg;

    pg = NIHIL;
    si (a->paginae_numerus < a->paginae_maximae)
    {
        pg = (Pagina*)piscina_conari_allocare(a->piscina,
            magnitudo(Pagina));
        si (pg)
        {
            pg->cellulae = (Cellula*)piscina_conari_allocare(a->piscina,
                PAGINA_OCTETI);
            pg->involutae = (i8*)piscina_conari_allocare(a->piscina,
                (memoriae_index)PAGINA_LINEAE_MAX);
            pg->stili = (StilusTerminalis*)piscina_conari_allocare(
                a->piscina, PAGINA_STILI * magnitudo(StilusTerminalis));
            si (!pg->cellulae || !pg->involutae || !pg->stili)
            {
                pg = NIHIL;
            }
        }
        si (pg)
        {
            a->paginae[(a->paginae_initium + a->paginae_numerus)
                       % a->paginae_maximae] = pg;
            a->paginae_numerus++;
        }
    }
    si (!pg)
    {
        si (a->paginae_numerus == ZEPHYRUM)
        {
            redde NIHIL;
        }
        /* evictio: vetustissima ad finem anuli */
        pg                  = paginam_ad(a, ZEPHYRUM);
        a->historia_lineae  -= pg->numerus;
        a->paginae_initium   = (a->paginae_initium + I)
                             % a->paginae_maximae;
        a->paginae[(a->paginae_initium + a->paginae_numerus - I)
                   % a->paginae_maximae] = pg;
        si (a->visus > a->historia_lineae)
        {
            a->visus = a->historia_lineae;
        }
    }
    paginam_parare(pg, a->latitudo);
    redde pg;
}

/* stilus in tabula paginae; -1 si plena */
interior s32
paginae_stilum (
                       Pagina* pg,
    constans StilusTerminalis* st)
{
    i32 i;

    per (i = ZEPHYRUM; i < pg->numerus_stilorum; i++)
    {
        si (stilus_aequalis(&pg->stili[i], st))
        {
            redde (s32)i;
        }
    }
    si (pg->numerus_stilorum >= PAGINA_STILI)
    {
        redde -I;
    }
    pg->stili[pg->numerus_stilorum] = *st;
    redde (s32)pg->numerus_stilorum++;
}

/* linea schirmi primarii abiens in historiam (copia; stili in tabulam
 * paginae internantur). Tabula plena: semel in pagina nova; linea
 * sola plures stilos poscens quam pagina capit -> stili superflui
 * nativi (degradatio). */
interior vacuum
historiam_addere (
    Aemulator* a,
        Linea* l)
{
     Pagina* pg;
    Cellula* linea;
        i32  n0;
        i32  x;
        s32  k;
        b32  iteratum;

    si (a->paginae_maximae == ZEPHYRUM)
    {
        redde;
    }
    pg = a->paginae_numerus > ZEPHYRUM
        ? paginam_ad(a, a->paginae_numerus - I) : NIHIL;
    /* pagina ultima vacua (post ED 3 aut retractionem) reutitur */
    si (pg && pg->numerus == ZEPHYRUM && pg->latitudo != a->latitudo)
    {
        paginam_parare(pg, a->latitudo);
    }
    si (   !pg || pg->numerus >= pg->capacitas
        || pg->latitudo != a->latitudo)
    {
        pg = paginam_novam(a);
        si (!pg)
        {
            redde;
        }
    }
    iteratum  = FALSUM;
    n0        = pg->numerus_stilorum;
    linea     = pg->cellulae + pg->numerus * pg->latitudo;
    per (x = ZEPHYRUM; x < pg->latitudo; x++)
    {
        k = paginae_stilum(pg, &a->stili[l->cellulae[x].stilus]);
        si (k < ZEPHYRUM && !iteratum && pg->numerus > ZEPHYRUM)
        {
            pg->numerus_stilorum  = n0;
            pg                    = paginam_novam(a);
            si (!pg)
            {
                redde;
            }
            iteratum  = VERUM;
            linea     = pg->cellulae;
            x         = (i32)-I;
            perge;
        }
        linea[x]         = l->cellulae[x];
        linea[x].stilus  = k < ZEPHYRUM ? ZEPHYRUM : (i32)k;
    }
    pg->involutae[pg->numerus] = (i8)(l->involuta ? I : ZEPHYRUM);
    pg->numerus++;
    a->historia_lineae++;
    si (a->visus > ZEPHYRUM)
    {
        a->visus++;    /* decisio XXI: eaedem lineae in visu manent */
    }
}

/* ED 3 (Ghostty eraseHistory): historia vacua; paginae manent ad
 * usum iterum (nihil allocatur, nihil liberatur); visus ad imum */
interior vacuum
historiam_delere (
    Aemulator* a)
{
    i32 i;

    per (i = ZEPHYRUM; i < a->paginae_numerus; i++)
    {
        paginam_ad(a, i)->numerus = ZEPHYRUM;
    }
    a->historia_lineae  = ZEPHYRUM;
    a->visus            = ZEPHYRUM;
}

/* linea historiae novissima in lineam l schirmi primarii (retractio
 * in amplitudine crescente): cellulae usque ad latitudinem minorem,
 * stili in tabulam schirmi re-internantur, cetera vacua */
interior vacuum
lineam_ex_historia (
    Aemulator* a,
        Linea* l)
{
      Pagina* pg;
     Cellula* fons;
         i32  i;
         i32  n;
         i32  x;

    pg = NIHIL;
    per (i = a->paginae_numerus; i > ZEPHYRUM; i--)
    {
        pg = paginam_ad(a, i - I);
        si (pg->numerus > ZEPHYRUM)
        {
            frange;
        }
    }
    si (!pg || pg->numerus == ZEPHYRUM)
    {
        redde;
    }
    pg->numerus--;
    a->historia_lineae--;
    fons  = pg->cellulae + pg->numerus * pg->latitudo;
    n     = pg->latitudo < a->latitudo ? pg->latitudo : a->latitudo;
    lineam_vacare(l, ZEPHYRUM, a->capacitas_latitudinis, ZEPHYRUM);
    per (x = ZEPHYRUM; x < n; x++)
    {
        l->cellulae[x]         = fons[x];
        l->cellulae[x].stilus  = ZEPHYRUM;
    }
    l->involuta = pg->involutae[pg->numerus] != ZEPHYRUM;
    /* stili post copiam: collectio (si fit) cellulas positas videt */
    per (x = ZEPHYRUM; x < n; x++)
    {
        l->cellulae[x].stilus = stilum_internare(a,
            &pg->stili[fons[x].stilus]);
    }
}

/* historia schirmi activi (alterum nullam habet) */
interior i32
historia_activa (
    constans Aemulator* a)
{
    redde a->activum == &a->primarium ? a->historia_lineae : ZEPHYRUM;
}

/* linea absoluta r in [0, historia_activa + altitudo): cellulae,
 * latitudo, tabula stilorum, involutio */
interior Cellula*
lineam_absolutam (
             constans Aemulator*  a,
                            i32   r,
                            i32*  latitudo,
      constans StilusTerminalis** stili,
                            b32*  involuta)
{
    constans Pagina* pg;
                i32  h;
                i32  i;

    h = historia_activa(a);
    si (r >= h)
    {
        *latitudo  = a->latitudo;
        *stili     = a->stili;
        *involuta  = a->activum->lineae[r - h]->involuta;
        redde a->activum->lineae[r - h]->cellulae;
    }
    per (i = ZEPHYRUM; i < a->paginae_numerus; i++)
    {
        pg = paginam_ad(a, i);
        si (r < pg->numerus)
        {
            *latitudo  = pg->latitudo;
            *stili     = pg->stili;
            *involuta  = pg->involutae[r] != ZEPHYRUM;
            redde pg->cellulae + r * pg->latitudo;
        }
        r -= pg->numerus;
    }
    *latitudo = ZEPHYRUM;
    redde NIHIL;
}

/* lineas [summa, ultima] n sursum volvere: summae abeunt - in
 * historiam si 'historia' et regio in summa linea schirmi primarii
 * (Ghostty index/scrollUp; DL numquam) - imae novae vacuae (BCE) */
interior vacuum
regionem_sursum (
    Aemulator* a,
          i32  summa,
          i32  ultima,
          i32  n,
          b32  historia)
{
    Schirmum* s;
       Linea* l;
         i32  y;
         i32  k;
         i32  stilus;

    s       = a->activum;
    stilus  = stilus_vacuus(a);
    si (n > ultima - summa + I)
    {
        n = ultima - summa + I;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        l = s->lineae[summa];
        si (historia && summa == ZEPHYRUM && s == &a->primarium)
        {
            historiam_addere(a, l);
        }
        per (y = summa; y < ultima; y++)
        {
            s->lineae[y] = s->lineae[y + I];
        }
        s->lineae[ultima] = l;
        lineam_vacare(l, ZEPHYRUM, a->capacitas_latitudinis, stilus);
        l->involuta = FALSUM;
    }
}

/* lineas [summa, ultima] n deorsum volvere: imae abeunt, summae novae
 * vacuae (BCE) */
interior vacuum
regionem_deorsum (
    Aemulator* a,
          i32  summa,
          i32  ultima,
          i32  n)
{
    Schirmum* s;
       Linea* l;
         i32  y;
         i32  k;
         i32  stilus;

    s       = a->activum;
    stilus  = stilus_vacuus(a);
    si (n > ultima - summa + I)
    {
        n = ultima - summa + I;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        l = s->lineae[ultima];
        per (y = ultima; y > summa; y--)
        {
            s->lineae[y] = s->lineae[y - I];
        }
        s->lineae[summa] = l;
        lineam_vacare(l, ZEPHYRUM, a->capacitas_latitudinis, stilus);
        l->involuta = FALSUM;
    }
}

/* IND (Ghostty index): extra regionem deorsum nisi in ultima linea
 * schirmi; in ultima regionis regio volvitur; aliter deorsum */
interior vacuum
indicem_movere (
    Aemulator* a)
{
    Schirmum* s;

    s                  = a->activum;
    s->cursor.pendens  = FALSUM;
    si (s->cursor.y < a->regio_summa || s->cursor.y > a->regio_ultima)
    {
        si (s->cursor.y + I < a->altitudo)
        {
            s->cursor.y++;
        }
        redde;
    }
    si (s->cursor.y == a->regio_ultima)
    {
        regionem_sursum(a, a->regio_summa, a->regio_ultima, I, VERUM);
        redde;
    }
    s->cursor.y++;
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

/* ICH (Ghostty insertBlanks): lata quae scinderetur tota vacatur (sub
 * cursore cauda, in margine dextra, in fine translationis) */
interior vacuum
cellulas_inserere (
    Aemulator* a,
          i32  n)
{
    Schirmum* s;
       Linea* l;
         i32  x;
         i32  reliquae;
         i32  movendae;
         i32  k;
         i32  stilus;

    s                  = a->activum;
    l                  = s->lineae[s->cursor.y];
    x                  = s->cursor.x;
    stilus             = stilus_vacuus(a);
    s->cursor.pendens  = FALSUM;
    si (x > ZEPHYRUM && l->cellulae[x].latitudo == AEMULATOR_CAUDA)
    {
        lineam_vacare(l, x - I, x + I, stilus);
    }
    reliquae = a->latitudo - x;
    si (l->cellulae[a->latitudo - I].latitudo == AEMULATOR_LATA)
    {
        lineam_vacare(l, a->latitudo - I, a->latitudo, stilus);
    }
    si (n > reliquae)
    {
        n = reliquae;
    }
    movendae = reliquae - n;
    si (movendae > ZEPHYRUM)
    {
        si (l->cellulae[x + movendae - I].latitudo == AEMULATOR_LATA)
        {
            lineam_vacare(l, x + movendae - I, x + movendae + I,
                stilus);
        }
        per (k = movendae; k > ZEPHYRUM; k--)
        {
            l->cellulae[x + k - I + n] = l->cellulae[x + k - I];
        }
    }
    lineam_vacare(l, x, x + n, stilus);
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
    a->runa_prior = runa;
    si (latitudo == II && a->latitudo < II)
    {
        latitudo = I;
    }
    si (s->cursor.pendens && modus_est(a, MODUS_INVOLUTIO))
    {
        lineam_involvere(a);
    }
    /* IRM (Ghostty print): spatia ante scripturam, nisi in fine */
    si (   modus_est(a, MODUS_INSERTIO)
        && s->cursor.x + latitudo < a->latitudo)
    {
        cellulas_inserere(a, latitudo);
    }
    /* lata in columna ultima: caput relinquitur, in proximam fluit */
    si (latitudo == II && s->cursor.x + I >= a->latitudo)
    {
        si (!modus_est(a, MODUS_INVOLUTIO))
        {
            redde;
        }
        cellulam_vacare(&s->lineae[s->cursor.y]->cellulae[s->cursor.x],
                        ZEPHYRUM);
        s->lineae[s->cursor.y]->cellulae[s->cursor.x].latitudo =
            (i8)AEMULATOR_CAPUT;
        lineam_involvere(a);
    }
    /* copia post latitudinem (Ghostty: latitudo ex runa non versa) */
    runa     = runam_vertere(a, runa);
    mensura  = utf8_codere(runa, octeti);
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

/* HT (Ghostty horizontalTab): dextrorsum usque ad sistam aut
 * marginem dextram */
interior vacuum
tabulam_procedere (
    Aemulator* a)
{
    Schirmum* s;

    s = a->activum;
    dum (s->cursor.x + I < a->latitudo)
    {
        s->cursor.x++;
        si (a->tabulae[s->cursor.x])
        {
            redde;
        }
    }
}

/* CBT (Ghostty horizontalTabBack): sinistrorsum usque ad sistam aut
 * columnam primam */
interior vacuum
tabulam_recedere (
    Aemulator* a)
{
    Schirmum* s;

    s = a->activum;
    dum (s->cursor.x > ZEPHYRUM)
    {
        s->cursor.x--;
        si (a->tabulae[s->cursor.x])
        {
            redde;
        }
    }
}

/* sistae ordinariae (Ghostty: omnis VIII) - creatio, mutatio
 * magnitudinis */
interior vacuum
tabulas_ordinare (
    Aemulator* a)
{
    i32 x;

    per (x = ZEPHYRUM; x < a->capacitas_latitudinis; x++)
    {
        a->tabulae[x] = (i8)((x % TABULATIO)
            == ZEPHYRUM ? I : ZEPHYRUM);
    }
}

/* CUB / BS (Ghostty cursorLeft, sine marginibus lr): DECAWM et XLV
 * (involutio retro: solum in lineam supra quae involuta est, non trans
 * summam regionis) aut MXLV (plenus: quaevis linea supra, a summa ad
 * imum regionis; praevalet). Involutio pendens passum unum consumit
 * (xterm). Sine his: sinistrorsum usque ad columnam primam. */
interior vacuum
cursorem_sinistrorsum (
    Aemulator* a,
          i32  n)
{
    Schirmum* s;
         b32  plenus;
         i32  passus;

    s = a->activum;
    si (   !modus_est(a, MODUS_INVOLUTIO)
        || (   !modus_est(a, MODUS_RETRO)
            && !modus_est(a, MODUS_RETRO_PLENUS)))
    {
        s->cursor.x        -= n < s->cursor.x ? n : s->cursor.x;
        s->cursor.pendens  = FALSUM;
        redde;
    }
    plenus = modus_est(a, MODUS_RETRO_PLENUS);
    si (s->cursor.pendens)
    {
        s->cursor.pendens = FALSUM;
        n--;
        si (n == ZEPHYRUM)
        {
            redde;
        }
    }
    /* XLV in columna prima, in summa aut supra: ad summam regionis */
    si (   s->cursor.x == ZEPHYRUM && !plenus
        && s->cursor.y <= a->regio_summa)
    {
        s->cursor.y = a->regio_summa;
        redde;
    }
    dum (VERUM)
    {
        passus       = n < s->cursor.x ? n : s->cursor.x;
        n            -= passus;
        s->cursor.x  -= passus;
        si (n == ZEPHYRUM)
        {
            frange;
        }
        si (s->cursor.y == a->regio_summa)
        {
            si (!plenus)
            {
                frange;
            }
            s->cursor.x = a->latitudo - I;
            s->cursor.y = a->regio_ultima;
            n--;
            /* ab hinc positiones cyclum lineae regionis x latitudo
             * passuum servant: residuum idem (parametrum saturatum ne
             * pendeat - Ghostty totum numerat) */
            n %= (a->regio_ultima - a->regio_summa + I) * a->latitudo;
            perge;
        }
        /* xterm hic ruit; Ghostty in (0, 0) sistit */
        si (s->cursor.y == ZEPHYRUM)
        {
            frange;
        }
        si (!plenus && !s->lineae[s->cursor.y - I]->involuta)
        {
            frange;
        }
        s->cursor.x  = a->latitudo - I;
        s->cursor.y--;
        n--;
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
            cursorem_sinistrorsum(a, I);
            frange;
        casus 0x09:
            tabulam_procedere(a);
            frange;
        casus 0x0A:
        casus 0x0B:
        casus 0x0C:
            indicem_movere(a);
            si (modus_est(a, MODUS_LINEA_NOVA))
            {
                s->cursor.x = ZEPHYRUM;
            }
            frange;
        casus 0x0D:
            s->cursor.x        = ZEPHYRUM;
            s->cursor.pendens  = FALSUM;
            frange;
        casus 0x0E:
            /* SO (LS1): G1 in GL */
            a->copiae.sinistra = (i8)I;
            frange;
        casus 0x0F:
            /* SI (LS0): G0 in GL */
            a->copiae.sinistra = ZEPHYRUM;
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

/* CUP / VPA / initium sub DECOM (Ghostty setCursorPos): linea
 * relativa ad summam regionis, ad ultimam praecisa; columna semper
 * absoluta (margines lr absunt) */
interior vacuum
cursorem_locare (
    Aemulator* a,
          i32  x,
          i32  y)
{
    si (modus_est(a, MODUS_ORIGO))
    {
        y = y > a->regio_ultima - a->regio_summa
            ? a->regio_ultima : a->regio_summa + y;
    }
    cursorem_ponere(a, x, y);
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
            /* historia solum; schirmum alterum nullam habet */
            si (s == &a->primarium)
            {
                historiam_delere(a);
            }
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
    s->servatus.origo    = modus_est(a, MODUS_ORIGO);
    s->servatus.copiae   = a->copiae;
    s->servatus.adest    = VERUM;
}

/* Ghostty restoreCursor: sine servatione = initium, stilus nativus,
 * DECOM remotum, copiae ordinariae; positio ad schirmum currens
 * praeciditur */
interior vacuum
cursorem_restituere (
    Aemulator* a)
{
    Schirmum* s;

    s = a->activum;
    si (!s->servatus.adest)
    {
        s->calamus            = ZEPHYRUM;
        a->modi[MODUS_ORIGO]  = ZEPHYRUM;
        copias_initiare(&a->copiae);
        cursorem_ponere(a, ZEPHYRUM, ZEPHYRUM);
        redde;
    }
    s->calamus            = s->servatus.calamus;
    a->modi[MODUS_ORIGO]  = (i8)(s->servatus.origo ? I : ZEPHYRUM);
    a->copiae             = s->servatus.copiae;
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
        a->visus    = ZEPHYRUM;
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

/* 47 / 1047 (Ghostty switchScreenMode ex xterm srm_ALTBUF /
 * srm_OPT_ALTBUF): schirmum mutatur et cursor (cum calamo) in schirmum
 * destinatum copiatur, si mutatum est; nulla servatio, nulla deletio
 * in ingressu. 1047 (delere): exitus ex altero id prius delet. */
interior vacuum
alterum_simpliciter (
    Aemulator* a,
          b32  ingredi,
          b32  delere)
{
    Schirmum* prior;
    Schirmum* destinatum;

    prior       = a->activum;
    destinatum  = ingredi ? &a->alterum : &a->primarium;
    si (!ingredi && delere && prior == &a->alterum)
    {
        schirmum_delere(a, II);
    }
    si (prior == destinatum)
    {
        redde;
    }
    a->activum           = destinatum;
    destinatum->cursor   = prior->cursor;
    destinatum->calamus  = prior->calamus;
    a->visus             = ZEPHYRUM;
}

/* SM / RM et DECSET / DECRST (Ghostty setMode): bitum crudum, deinde
 * effectus; modus ignotus numeratur. Eventus muris et forma: positum
 * ultimum vincit, remotum quodlibet ad nullum / X10 redit. */
interior vacuum
modum_ponere (
    Aemulator* a,
          i32  numerus,
          b32  privatus,
          b32  status)
{
    s32 k;

    k = modum_invenire(numerus, privatus);
    si (k < ZEPHYRUM)
    {
        a->ignota++;
        redde;
    }
    a->modi[k] = (i8)(status ? I : ZEPHYRUM);
    commutatio (k)
    {
        casus MODUS_ORIGO:
            cursorem_locare(a, ZEPHYRUM, ZEPHYRUM);
            frange;
        casus MODUS_ALTERUM_SERVATUM:
            alterum_ponere(a, status);
            frange;
        casus MODUS_ALTERUM_VETUS:
            alterum_simpliciter(a, status, FALSUM);
            frange;
        casus MODUS_ALTERUM:
            alterum_simpliciter(a, status, VERUM);
            frange;
        casus MODUS_CURSOR_SERVATUS:
            si (status)
            {
                cursorem_servare(a);
            }
            alioquin
            {
                cursorem_restituere(a);
            }
            frange;
        casus MODUS_MUS_X10:
            a->mus = status ? AEMULATOR_MUS_X10 : AEMULATOR_MUS_NULLUS;
            frange;
        casus MODUS_MUS_PRESSIO:
            a->mus =
                status ? AEMULATOR_MUS_PRESSIO : AEMULATOR_MUS_NULLUS;
            frange;
        casus MODUS_MUS_TRACTUS:
            a->mus =
                status ? AEMULATOR_MUS_TRACTUS : AEMULATOR_MUS_NULLUS;
            frange;
        casus MODUS_MUS_OMNIS:
            a->mus =
                status ? AEMULATOR_MUS_OMNIS : AEMULATOR_MUS_NULLUS;
            frange;
        casus MODUS_FORMA_UTF8:
            a->mus_forma = status ? AEMULATOR_MUS_FORMA_UTF8
                                  : AEMULATOR_MUS_FORMA_X10;
            frange;
        casus MODUS_FORMA_SGR:
            a->mus_forma = status ? AEMULATOR_MUS_FORMA_SGR
                                  : AEMULATOR_MUS_FORMA_X10;
            frange;
        casus MODUS_FORMA_URXVT:
            a->mus_forma = status ? AEMULATOR_MUS_FORMA_URXVT
                                  : AEMULATOR_MUS_FORMA_X10;
            frange;
        casus MODUS_FORMA_SGR_PIXELA:
            a->mus_forma = status ? AEMULATOR_MUS_FORMA_SGR_PIXELA
                                  : AEMULATOR_MUS_FORMA_X10;
            frange;
        ordinarius:
            frange;
    }
}

/* responsum ad programma (effectus); sine effectu nihil */
interior vacuum
respondere (
       Aemulator* a,
     constans i8* octeti,
             i32  n)
{
    si (a->effectus.responsum)
    {
        a->effectus.responsum(a->effectus.datum, octeti, n);
    }
}

/* responsum fixum: mensura per strlen, numquam manu numerata (B2:
 * DA1 cum X pro IX octetum NUL emittebat) */
interior vacuum
literas_respondere (
                 Aemulator* a,
        constans character* literae)
{
    respondere(a, (constans i8*)literae, (i32)strlen(literae));
}

/* numerus decimalis in b[*n] (sine stdio - puritas) */
interior vacuum
numerum_appendere (
      i8* b,
     i32* n,
     i32  valor)
{
     i8 digiti[XII];
    i32 k;

    k = ZEPHYRUM;
    fac
    {
        digiti[k++]  = (i8)('0' + valor % X);
        valor        /= X;
    } dum (valor > ZEPHYRUM);
    dum (k > ZEPHYRUM)
    {
        b[(*n)++] = digiti[--k];
    }
}

/* CPR (Ghostty device_status cursor_position): ESC [ y ; x R,
 * 1-based */
interior vacuum
positum_nuntiare (
    Aemulator* a)
{
     i8 b[XXXII];
    i32 n;
    i32 y;

    n       = ZEPHYRUM;
    b[n++]  = (i8)0x1B;
    b[n++]  = '[';
    y       = a->activum->cursor.y;
    si (modus_est(a, MODUS_ORIGO) && y >= a->regio_summa)
    {
        y -= a->regio_summa;
    }
    numerum_appendere(b, &n, y + I);
    b[n++]  = ';';
    numerum_appendere(b, &n, a->activum->cursor.x + I);
    b[n++]  = 'R';
    respondere(a, b, n);
}

/* DECRPM (Ghostty Report.encode): ESC [ (?) n ; s $ y - s: I positus,
 * II remotus, 0 ignotus (modi non honorati quoque) */
interior vacuum
modum_nuntiare (
    Aemulator* a,
          i32  numerus,
          b32  privatus)
{
     i8 b[XXXII];
    i32 n;
    s32 k;

    k       = modum_invenire(numerus, privatus);
    n       = ZEPHYRUM;
    b[n++]  = (i8)0x1B;
    b[n++]  = '[';
    si (privatus)
    {
        b[n++] = '?';
    }
    numerum_appendere(b, &n, numerus);
    b[n++] = ';';
    b[n++] = (i8)(k < ZEPHYRUM               ? '0'
                : modus_est(a, (i32)k)       ? '1' : '2');
    b[n++] = '$';
    b[n++] = 'y';
    respondere(a, b, n);
}

/* XTWINOPS 18: ESC [ 8 ; altitudo ; latitudo t */
interior vacuum
magnitudinem_nuntiare (
    Aemulator* a)
{
     i8 b[XXXII];
    i32 n;

    n       = ZEPHYRUM;
    b[n++]  = (i8)0x1B;
    b[n++]  = '[';
    b[n++]  = '8';
    b[n++]  = ';';
    numerum_appendere(b, &n, a->altitudo);
    b[n++]  = ';';
    numerum_appendere(b, &n, a->latitudo);
    b[n++]  = 't';
    respondere(a, b, n);
}

/* XTVERSION: DCS > | titulus versio ST */
interior vacuum
versionem_nuntiare (
    Aemulator* a)
{
    literas_respondere(a, "\033P>|");
    respondere(a, a->identitas.datum, a->identitas.mensura);
    literas_respondere(a, "\033\\");
}

/* OSC (Ghostty osc): 0 et 2 titulus (etiam vacuus), 1 icon
 * (consumitur, nihil), cetera ignota. Corpus truncatum (ultra
 * SERIES_CHORDA_MAXIMA) totum abicitur et numeratur, ut Ghostty
 * (state invalid -> nullum mandatum): titulus praecisus numquam
 * nuntiatur. */
interior vacuum
seriem_osc (
                 Aemulator* a,
     constans SeriesLexema* lx)
{
    chorda titulus;
       i32 numerus;
       i32 i;

    si (lx->truncatum)
    {
        a->ignota++;
        redde;
    }
    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < lx->textus.mensura
             && lx->textus.datum[i] >= '0'
             && lx->textus.datum[i] <= '9';
         i++)
    {
        numerus = numerus * X + (i32)(lx->textus.datum[i] - '0');
    }
    si (   i                   == ZEPHYRUM || i >= lx->textus.mensura
        || lx->textus.datum[i] != ';')
    {
        a->ignota++;
        redde;
    }
    commutatio (numerus)
    {
        casus ZEPHYRUM:
        casus II:
            titulus.datum    = lx->textus.datum + i + I;
            titulus.mensura  = lx->textus.mensura - i - I;
            si (a->effectus.titulus)
            {
                a->effectus.titulus(a->effectus.datum, titulus);
            }
            frange;
        casus I:
            frange;
        ordinarius:
            a->ignota++;
            frange;
    }
}

/* Ghostty splitCellBoundary: si (x) cauda est, lata eius tota
 * vacatur (BCE) */
interior vacuum
limitem_findere (
    Aemulator* a,
        Linea* l,
          i32  x)
{
    si (   x > ZEPHYRUM && x < a->latitudo
        && l->cellulae[x].latitudo == AEMULATOR_CAUDA)
    {
        lineam_vacare(l, x - I, x + I, stilus_vacuus(a));
    }
}

/* CUU / CUD (Ghostty cursorUp/Down): intra regionem margines sistunt,
 * extra eam schirmi; pendens tollitur */
interior vacuum
cursorem_sursum (
    Aemulator* a,
          i32  n)
{
    Schirmum* s;
         i32  maximum;

    s        = a->activum;
    maximum  = s->cursor.y >= a->regio_summa
        ? s->cursor.y - a->regio_summa : s->cursor.y;
    s->cursor.y        -= n < maximum ? n : maximum;
    s->cursor.pendens  = FALSUM;
}

interior vacuum
cursorem_deorsum (
    Aemulator* a,
          i32  n)
{
    Schirmum* s;
         i32  maximum;

    s        = a->activum;
    maximum  = s->cursor.y <= a->regio_ultima
        ? a->regio_ultima - s->cursor.y : a->altitudo - I - s->cursor.y;
    s->cursor.y        += n < maximum ? n : maximum;
    s->cursor.pendens  = FALSUM;
}

/* IL / DL (Ghostty insertLines/deleteLines): extra regionem nihil;
 * lineae motae involutionem amittunt; cursor ad columnam primam */
interior vacuum
lineas_inserere (
    Aemulator* a,
          i32  n,
          b32  delere)
{
    Schirmum* s;
         i32  y;

    s = a->activum;
    si (s->cursor.y < a->regio_summa || s->cursor.y > a->regio_ultima)
    {
        redde;
    }
    si (delere)
    {
        regionem_sursum(a, s->cursor.y, a->regio_ultima, n, FALSUM);
    }
    alioquin
    {
        regionem_deorsum(a, s->cursor.y, a->regio_ultima, n);
    }
    per (y = s->cursor.y; y <= a->regio_ultima; y++)
    {
        s->lineae[y]->involuta = FALSUM;
    }
    s->cursor.x        = ZEPHYRUM;
    s->cursor.pendens  = FALSUM;
}

/* DCH (Ghostty deleteChars): limites latarum finduntur, sinistrorsum
 * movetur, finis vacuus (BCE); involutio et pendens tolluntur */
interior vacuum
cellulas_delere (
    Aemulator* a,
          i32  n)
{
    Schirmum* s;
       Linea* l;
         i32  x;
         i32  reliquae;
         i32  movendae;
         i32  k;

    s         = a->activum;
    l         = s->lineae[s->cursor.y];
    x         = s->cursor.x;
    reliquae  = a->latitudo - x;
    si (n > reliquae)
    {
        n = reliquae;
    }
    limitem_findere(a, l, x);
    limitem_findere(a, l, x + n);
    movendae = reliquae - n;
    per (k = ZEPHYRUM; k < movendae; k++)
    {
        l->cellulae[x + k] = l->cellulae[x + k + n];
    }
    lineam_vacare(l, x + movendae, a->latitudo, stilus_vacuus(a));
    l->involuta        = FALSUM;
    s->cursor.pendens  = FALSUM;
}

/* ECH (Ghostty eraseChars): limites finduntur (lata in fine tota per
 * fissionem ad x + n - Ghostty praeterea n auget, quod hic idem facit
 * et nullo initu discernitur; planta B1 P7); involutio et pendens
 * tolluntur */
interior vacuum
cellulas_eradere (
    Aemulator* a,
          i32  n)
{
    Schirmum* s;
       Linea* l;
         i32  x;
         i32  reliquae;

    s         = a->activum;
    l         = s->lineae[s->cursor.y];
    x         = s->cursor.x;
    reliquae  = a->latitudo - x;
    si (n > reliquae)
    {
        n = reliquae;
    }
    limitem_findere(a, l, x);
    limitem_findere(a, l, x + n);
    lineam_vacare(l, x, x + n, stilus_vacuus(a));
    l->involuta        = FALSUM;
    s->cursor.pendens  = FALSUM;
}

/* DECALN (Ghostty decaln): calamus ad colores solos redactus (manet),
 * regio tota, schirmum totum 'E' eo calamo, cursor ad initium */
interior vacuum
schirmum_probationis_implere (
    Aemulator* a)
{
            Schirmum* s;
    StilusTerminalis  st;
    StilusTerminalis  colores;
                 i32  stilus;
                 i32  x;
                 i32  y;

    s   = a->activum;
    st  = a->stili[s->calamus];
    stilus_nativus(&colores);
    colores.color_litterae  = st.color_litterae;
    colores.color_fundi     = st.color_fundi;
    stilus                  = stilum_internare(a, &colores);
    s->calamus              = stilus;
    a->regio_summa          = ZEPHYRUM;
    a->regio_ultima         = a->altitudo - I;
    per (y = ZEPHYRUM; y < a->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < a->latitudo; x++)
        {
            cellulam_vacare(&s->lineae[y]->cellulae[x], stilus);
            s->lineae[y]->cellulae[x].octeti[ZEPHYRUM]  = 'E';
            s->lineae[y]->cellulae[x].mensura           = I;
        }
        s->lineae[y]->involuta = FALSUM;
    }
    cursorem_ponere(a, ZEPHYRUM, ZEPHYRUM);
}

/* RIS (Ghostty fullReset): schirmum primarium, alterum deletum,
 * historia vacua, cursores et servati, calami nativi, modi ordinarii,
 * regio, sistae, character prior, copiae characterum. Effectus et
 * identitas manent. */
interior vacuum
plene_restituere (
    Aemulator* a)
{
    Schirmum* schirma[II];
         i32  k;
         i32  y;

    schirma[ZEPHYRUM]  = &a->primarium;
    schirma[I]         = &a->alterum;
    per (k = ZEPHYRUM; k < II; k++)
    {
        per (y = ZEPHYRUM; y < a->altitudo; y++)
        {
            lineam_vacare(schirma[k]->lineae[y], ZEPHYRUM,
                a->capacitas_latitudinis, ZEPHYRUM);
            schirma[k]->lineae[y]->involuta = FALSUM;
        }
        memset(&schirma[k]->cursor, ZEPHYRUM,
            magnitudo(AemulatorCursor));
        schirma[k]->calamus         = ZEPHYRUM;
        schirma[k]->servatus.adest  = FALSUM;
    }
    a->activum       = &a->primarium;
    historiam_delere(a);
    modos_initiare(a);
    copias_initiare(&a->copiae);
    a->regio_summa       = ZEPHYRUM;
    a->regio_ultima      = a->altitudo - I;
    a->runa_prior        = ZEPHYRUM;
    a->residuum_mensura  = ZEPHYRUM;
    tabulas_ordinare(a);
}

/* DECSTR (xterm, VT510, esctest2 decstr.py): cursor visibilis, regio
 * tota, calamus nativus, DECSC ad initium (servatio oblita -
 * restitutio initium et calamum nativum dat); IRM, DECOM, DECCKM,
 * tabula numerica, involutio retro (XLV, MXLV) remota; copiae
 * ordinariae (VT510: G0-G3 ASCII, GL = G0). Cursor manet;
 * DECAWM VERUM manet (xterm consulto, contra DEC); mus manet. */
interior vacuum
mollem_restituere (
    Aemulator* a)
{
    a->modi[MODUS_VISIBILIS]     = (i8)I;
    a->modi[MODUS_INVOLUTIO]     = (i8)I;
    a->modi[MODUS_INSERTIO]      = ZEPHYRUM;
    a->modi[MODUS_ORIGO]         = ZEPHYRUM;
    a->modi[MODUS_SAGITTAE]      = ZEPHYRUM;
    a->modi[MODUS_TABULA]        = ZEPHYRUM;
    a->modi[MODUS_RETRO]         = ZEPHYRUM;
    a->modi[MODUS_RETRO_PLENUS]  = ZEPHYRUM;
    copias_initiare(&a->copiae);
    a->regio_summa              = ZEPHYRUM;
    a->regio_ultima             = a->altitudo - I;
    a->activum->calamus         = ZEPHYRUM;
    a->activum->servatus.adest  = FALSUM;
}

/* punctum codicis cellulae pro DECRQCRA: vacua = spatium (xterm >=
 * 334), cauda = 0, ceterae ex UTF-8 */
interior i32
cellulae_punctum (
    constans Cellula* c)
{
    i32 b0;
    i32 p;
    i32 k;
    i32 n;

    si (c->latitudo == AEMULATOR_CAUDA)
    {
        redde ZEPHYRUM;
    }
    si (c->mensura == ZEPHYRUM)
    {
        redde (i32)' ';
    }
    b0 = (i32)c->octeti[ZEPHYRUM] & 0xFF;
    si (b0 < 0x80)
    {
        redde b0;
    }
    si (b0 >= 0xF0)
    {
        p = b0 & 0x07;
        n = III;
    }
    alioquin si (b0 >= 0xE0)
    {
        p = b0 & 0x0F;
        n = II;
    }
    alioquin
    {
        p = b0 & 0x1F;
        n = I;
    }
    per (k = I; k <= n && k < (i32)c->mensura; k++)
    {
        p = (p << VI) | ((i32)c->octeti[k] & 0x3F);
    }
    redde p;
}

/* DECRQCRA: CSI Pid ; Pp ; Pt ; Pl ; Pb ; Pr * y -> DCS Pid ! ~ XXXX
 * ST. Summa XVI bitorum punctorum codicis; limites 1-based,
 * ordinarii schirmus totus, ad schirmum praecisi; inversum = 0. */
interior vacuum
summam_nuntiare (
                 Aemulator* a,
     constans SeriesLexema* lx)
{
     constans character* hex = "0123456789ABCDEF";
                     i8  b[XXXII];
                    i32  n;
                    i32  summa;
                    i32  summa_y;
                    i32  summa_x;
                    i32  ultima_y;
                    i32  ultima_x;
                    i32  x;
                    i32  y;

    summa_y   = parametrum(lx, II, I) - I;
    summa_x   = parametrum(lx, III, I) - I;
    ultima_y  = parametrum(lx, IV, a->altitudo) - I;
    ultima_x  = parametrum(lx, V, a->latitudo) - I;
    si (ultima_y >= a->altitudo)
    {
        ultima_y = a->altitudo - I;
    }
    si (ultima_x >= a->latitudo)
    {
        ultima_x = a->latitudo - I;
    }
    summa = ZEPHYRUM;
    per (y = summa_y; y <= ultima_y && summa_y < a->altitudo; y++)
    {
        per (x = summa_x; x <= ultima_x && summa_x < a->latitudo; x++)
        {
            summa += cellulae_punctum(
                &a->activum->lineae[y]->cellulae[x]);
        }
    }
    summa   &= 0xFFFF;
    n       = ZEPHYRUM;
    b[n++]  = (i8)0x1B;
    b[n++]  = 'P';
    numerum_appendere(b, &n, lx->numerus_parametrorum > ZEPHYRUM
                                 ? (i32)lx->parametra[ZEPHYRUM]
                                 : ZEPHYRUM);
    b[n++] = '!';
    b[n++] = '~';
    b[n++] = (i8)hex[(summa >> XII) & 0xF];
    b[n++] = (i8)hex[(summa >> VIII) & 0xF];
    b[n++] = (i8)hex[(summa >> IV) & 0xF];
    b[n++] = (i8)hex[summa & 0xF];
    b[n++] = (i8)0x1B;
    b[n++] = '\\';
    respondere(a, b, n);
}

/* DECSTBM: summa >= ultima ignoratur; cursor ad initium (sub DECOM
 * regionis) */
interior vacuum
regionem_ponere (
                 Aemulator* a,
     constans SeriesLexema* lx)
{
    i32 summa;
    i32 ultima;

    summa   = parametrum(lx, ZEPHYRUM, I);
    ultima  = parametrum(lx, I, a->altitudo);
    si (ultima > a->altitudo)
    {
        ultima = a->altitudo;
    }
    si (summa >= ultima)
    {
        redde;
    }
    a->regio_summa   = summa - I;
    a->regio_ultima  = ultima - I;
    cursorem_locare(a, ZEPHYRUM, ZEPHYRUM);
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
    /* DECSTR (CSI ! p) et DECRQCRA (CSI ... * y, sub vexillo) */
    si (   lx->numerus_intermediorum == I && lx->privatum == ZEPHYRUM
        && lx->separatores           == ZEPHYRUM)
    {
        si (lx->intermedia[ZEPHYRUM] == '!' && lx->finale == 'p')
        {
            mollem_restituere(a);
            redde;
        }
        si (   lx->intermedia[ZEPHYRUM] == '*' && lx->finale == 'y'
            && a->lectio_schirmi)
        {
            summam_nuntiare(a, lx);
            redde;
        }
    }
    /* DECRQM (Ghostty request_mode): CSI (?) n $ p, parametrum unum */
    si (   lx->numerus_intermediorum == I
        && lx->intermedia[ZEPHYRUM] == '$'
        && lx->finale == 'p' && lx->separatores == ZEPHYRUM
        && (lx->privatum == ZEPHYRUM || lx->privatum == '?'))
    {
        si (lx->numerus_parametrorum != I)
        {
            a->ignota++;
            redde;
        }
        modum_nuntiare(a, (i32)lx->parametra[ZEPHYRUM],
                       lx->privatum == '?');
        redde;
    }
    /* intermedia aut ':' extra SGR: non nostra (DECSCUSR...) */
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
            modum_ponere(a, (i32)lx->parametra[i], VERUM,
                         lx->finale == 'h');
        }
        redde;
    }
    /* DA2, XTVERSION, DA3 (Ghostty device_attributes, xtversion) */
    si (   lx->privatum == '>' && lx->finale == 'c'
        && parametrum(lx, ZEPHYRUM, ZEPHYRUM) == ZEPHYRUM)
    {
        literas_respondere(a, "\033[>1;0;0c");
        redde;
    }
    si (lx->privatum == '>' && lx->finale == 'q')
    {
        versionem_nuntiare(a);
        redde;
    }
    si (lx->privatum == '=' && lx->finale == 'c')
    {
        literas_respondere(a, "\033P!|00000000\033\\");
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
            cursorem_locare(a, parametrum(lx, I, I) - I, n - I);
            frange;
        casus 'A':
            cursorem_sursum(a, n);
            frange;
        casus 'B':
            cursorem_deorsum(a, n);
            frange;
        casus 'C':
            cursorem_ponere(a, s->cursor.x + n, s->cursor.y);
            frange;
        casus 'D':
            cursorem_sinistrorsum(a, n);
            frange;
        casus 'E':
            cursorem_deorsum(a, n);
            s->cursor.x = ZEPHYRUM;
            frange;
        casus 'F':
            cursorem_sursum(a, n);
            s->cursor.x = ZEPHYRUM;
            frange;
        casus 'G':
        casus '`':
            cursorem_ponere(a, n - I, s->cursor.y);
            frange;
        casus 'd':
            cursorem_locare(a, s->cursor.x, n - I);
            frange;
        casus 'J':
            schirmum_delere(a, parametrum(lx, ZEPHYRUM, ZEPHYRUM));
            frange;
        casus 'K':
            lineam_delere(a, parametrum(lx, ZEPHYRUM, ZEPHYRUM));
            frange;
        casus 'r':
            regionem_ponere(a, lx);
            frange;
        casus 'c':
            /* DA1: VT220 cum coloribus ANSI (Ghostty Primary) */
            si (parametrum(lx, ZEPHYRUM, ZEPHYRUM) != ZEPHYRUM)
            {
                a->ignota++;
                frange;
            }
            literas_respondere(a, "\033[?62;22c");
            frange;
        casus 'n':
            commutatio (parametrum(lx, ZEPHYRUM, ZEPHYRUM))
            {
                casus V:
                    literas_respondere(a, "\033[0n");
                    frange;
                casus VI:
                    positum_nuntiare(a);
                    frange;
                ordinarius:
                    a->ignota++;
                    frange;
            }
            frange;
        casus 'S':
            regionem_sursum(a, a->regio_summa, a->regio_ultima, n,
                            VERUM);
            frange;
        casus 'a':
        casus 'e':
            /* HPR / VPR (Ghostty cursor_col/row_relative): absens = 1,
             * 0 explicitum = nihil; plura = ignotum */
            si (lx->numerus_parametrorum > I)
            {
                a->ignota++;
                frange;
            }
            n = lx->numerus_parametrorum == ZEPHYRUM
                ? I : (i32)lx->parametra[ZEPHYRUM];
            si (lx->finale == 'a')
            {
                cursorem_ponere(a, s->cursor.x + n, s->cursor.y);
            }
            alioquin
            {
                /* sub DECOM ad ultimam regionis (xterm CursorSet) */
                n += s->cursor.y;
                si (   modus_est(a, MODUS_ORIGO)
                    && n > a->regio_ultima)
                {
                    n = a->regio_ultima;
                }
                cursorem_ponere(a, s->cursor.x, n);
            }
            frange;
        casus 'b':
            /* REP (Ghostty printRepeat): character prior max(n, 1)
             * vicibus; terminus: bis area schirmi (ultra volvitur;
             * parametrum saturatum ne pendeat) */
            si (lx->numerus_parametrorum > I)
            {
                a->ignota++;
                frange;
            }
            si (a->runa_prior == ZEPHYRUM)
            {
                frange;
            }
            si (n > II * a->latitudo * a->altitudo)
            {
                n = II * a->latitudo * a->altitudo;
            }
            per (i = ZEPHYRUM; i < n; i++)
            {
                runam_imprimere(a, a->runa_prior);
            }
            frange;
        casus 's':
            /* SCOSC (marginibus lr absentibus, Ghostty) */
            cursorem_servare(a);
            frange;
        casus 'u':
            /* SCORC */
            cursorem_restituere(a);
            frange;
        casus 't':
            /* XTWINOPS 18 solum (Ghostty csi_18_t): magnitudo textus
             * in cellulis; parametra plura aut alia = ignota */
            si (   lx->numerus_parametrorum           != I
                || parametrum(lx, ZEPHYRUM, ZEPHYRUM) != XVIII)
            {
                a->ignota++;
                frange;
            }
            magnitudinem_nuntiare(a);
            frange;
        casus 'T':
            /* plura parametra: xterm 'mouse highlight', non SD */
            si (lx->numerus_parametrorum > I)
            {
                a->ignota++;
                frange;
            }
            regionem_deorsum(a, a->regio_summa, a->regio_ultima, n);
            frange;
        casus 'L':
            lineas_inserere(a, n, FALSUM);
            frange;
        casus 'M':
            lineas_inserere(a, n, VERUM);
            frange;
        casus '@':
            cellulas_inserere(a, n);
            frange;
        casus 'P':
            cellulas_delere(a, n);
            frange;
        casus 'X':
            cellulas_eradere(a, n);
            frange;
        casus 'I':
            per (i = ZEPHYRUM; i < n; i++)
            {
                tabulam_procedere(a);
            }
            frange;
        casus 'Z':
            per (i = ZEPHYRUM; i < n; i++)
            {
                tabulam_recedere(a);
            }
            frange;
        casus 'g':
            commutatio (parametrum(lx, ZEPHYRUM, ZEPHYRUM))
            {
                casus ZEPHYRUM:
                    a->tabulae[s->cursor.x] = ZEPHYRUM;
                    frange;
                casus III:
                    memset(a->tabulae, ZEPHYRUM,
                        (memoriae_index)a->capacitas_latitudinis);
                    frange;
                ordinarius:
                    a->ignota++;
                    frange;
            }
            frange;
        casus 'h':
        casus 'l':
            per (i = ZEPHYRUM; i < lx->numerus_parametrorum; i++)
            {
                modum_ponere(a, (i32)lx->parametra[i], FALSUM,
                             lx->finale == 'h');
            }
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

/* SCS: copia in G (0-III); finale ignotum numeratur, nihil mutat */
interior vacuum
copiam_designare (
    Aemulator* a,
          i32  g,
           i8  finale)
{
    commutatio (finale)
    {
        casus 'B':
            a->copiae.copiae[g] = (i8)COPIA_ASCII;
            frange;
        casus 'A':
            a->copiae.copiae[g] = (i8)COPIA_BRITANNICA;
            frange;
        casus '0':
            a->copiae.copiae[g] = (i8)COPIA_GRAPHICA;
            frange;
        ordinarius:
            a->ignota++;
            frange;
    }
}

/* SS2 / SS3 (Ghostty invokeCharset single): G2 / G3 pro charactere
 * proximo. Lexemator noster octetum sequentem in lexema SS includit
 * (divergentia eius, pro clavibus SS3): octeti imprimibiles post
 * introductorem hic imprimuntur, primus sub copia invocata */
interior vacuum
seriem_ss (
                 Aemulator* a,
     constans SeriesLexema* lx)
{
    i32 k;
     i8 c;

    a->copiae.semel = (i8)(lx->introductor == 'N' ? II : III);
    per (k = II; k < lx->crudum.mensura; k++)
    {
        c = lx->crudum.datum[k];
        si (c >= 0x20 && c <= 0x7E)
        {
            runam_imprimere(a, (s32)c);
        }
    }
}

interior vacuum
seriem_esc (
                 Aemulator* a,
     constans SeriesLexema* lx)
{
    Schirmum* s;

    s = a->activum;
    si (   lx->numerus_intermediorum == I
        && lx->intermedia[ZEPHYRUM]  == '#' && lx->finale == '8')
    {
        schirmum_probationis_implere(a);
        redde;
    }
    /* SCS (Ghostty configureCharset): ESC ( ) * + -> G0 G1 G2 G3;
     * finale B (ASCII), A (Britannica), 0 (DEC Special Graphics) */
    si (   lx->numerus_intermediorum == I
        && lx->intermedia[ZEPHYRUM]  >= '('
        && lx->intermedia[ZEPHYRUM]  <= '+')
    {
        copiam_designare(a, (i32)(lx->intermedia[ZEPHYRUM] - '('),
                         lx->finale);
        redde;
    }
    si (lx->numerus_intermediorum != ZEPHYRUM)
    {
        a->ignota++;
        redde;
    }
    commutatio (lx->finale)
    {
        casus 'c':
            plene_restituere(a);
            redde;
        casus 'n':
            /* LS2 / LS3: G2 / G3 in GL */
            a->copiae.sinistra = (i8)II;
            redde;
        casus 'o':
            a->copiae.sinistra = (i8)III;
            redde;
        casus '~':
            /* LS1R / LS2R / LS3R: in GR (servatur) */
            a->copiae.dextra = (i8)I;
            redde;
        casus '}':
            a->copiae.dextra = (i8)II;
            redde;
        casus '|':
            a->copiae.dextra = (i8)III;
            redde;
        casus 'D':
            indicem_movere(a);
            redde;
        casus 'E':
            s->cursor.x = ZEPHYRUM;
            indicem_movere(a);
            redde;
        casus 'M':
            /* RI (Ghostty reverseIndex): in summa regionis regio
             * deorsum volvitur, aliter cursor sursum */
            si (s->cursor.y == a->regio_summa)
            {
                regionem_deorsum(a, a->regio_summa, a->regio_ultima, I);
            }
            alioquin
            {
                cursorem_sursum(a, I);
            }
            redde;
        casus 'H':
            a->tabulae[s->cursor.x] = (i8)I;
            redde;
        casus '=':
            /* DECKPAM / DECKPNM (Ghostty keypad_keys = LXVI) */
            a->modi[MODUS_TABULA] = (i8)I;
            redde;
        casus '>':
            a->modi[MODUS_TABULA] = ZEPHYRUM;
            redde;
        ordinarius:
            frange;
    }
    si (lx->finale == '7')
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

/* 'titulus versio' in piscinam copiatum (XTVERSION) */
interior chorda
identitatem_struere (
               Piscina* piscina,
    constans character* titulus,
    constans character* versio)
{
     chorda c;
        i32 lt;
        i32 lv;

    lt         = (i32)strlen(titulus);
    lv         = (i32)strlen(versio);
    c.mensura  = lt + I + lv;
    c.datum    = (i8*)piscina_conari_allocare(piscina,
        (memoriae_index)c.mensura);
    si (!c.datum)
    {
        redde c;
    }
    memcpy(c.datum, titulus, (memoriae_index)lt);
    c.datum[lt] = ' ';
    memcpy(c.datum + lt + I, versio, (memoriae_index)lv);
    redde c;
}

vacuum
aemulator_configuratio_initiare (
    AemulatorConfiguratio* cfg)
{
    si (!cfg)
    {
        redde;
    }
    memset(cfg, ZEPHYRUM, magnitudo(AemulatorConfiguratio));
    cfg->latitudo         = LATITUDO_ORDINARIA;
    cfg->altitudo         = ALTITUDO_ORDINARIA;
    cfg->titulus          = "aemulator";
    cfg->versio           = AEMULATOR_VERSIO;
    cfg->historia_octeti  = X * MXXIV * MXXIV;
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
    a->piscina         = piscina;
    a->effectus        = cfg->effectus;
    a->lectio_schirmi  = cfg->lectio_schirmi;
    a->identitas = identitatem_struere(piscina,
        cfg->titulus ? cfg->titulus : "aemulator",
        cfg->versio ? cfg->versio : AEMULATOR_VERSIO);
    si (!a->identitas.datum)
    {
        redde NIHIL;
    }
    a->latitudo               = cfg->latitudo;
    a->altitudo               = cfg->altitudo;
    a->capacitas_latitudinis  = cfg->latitudo;
    a->capacitas_altitudinis  = cfg->altitudo;
    modos_initiare(a);
    copias_initiare(&a->copiae);
    a->activum                = &a->primarium;
    a->primarium.lineae = lineas_struere(piscina, cfg->latitudo,
                                         cfg->altitudo);
    a->alterum.lineae = lineas_struere(piscina, cfg->latitudo,
                                       cfg->altitudo);
    a->stili = (StilusTerminalis*)piscina_conari_allocare(piscina,
        STILI_MAXIMI * magnitudo(StilusTerminalis));
    a->transitus = (i32*)piscina_conari_allocare(piscina,
        STILI_MAXIMI * magnitudo(i32));
    a->tabulae = (i8*)piscina_conari_allocare(piscina,
        (memoriae_index)cfg->latitudo);
    a->lector = series_lectorem_creare(piscina);
    si (   !a->primarium.lineae || !a->alterum.lineae || !a->stili
        || !a->transitus || !a->tabulae || !a->lector)
    {
        redde NIHIL;
    }
    /* historia: limes ad paginas integras sursum, una saltem */
    si (cfg->historia_octeti > ZEPHYRUM)
    {
        a->paginae_maximae = (cfg->historia_octeti + PAGINA_OCTETI - I)
                           / PAGINA_OCTETI;
        a->paginae = (Pagina**)piscina_conari_allocare(piscina,
            (memoriae_index)a->paginae_maximae * magnitudo(Pagina*));
        si (!a->paginae)
        {
            redde NIHIL;
        }
    }
    a->regio_summa   = ZEPHYRUM;
    a->regio_ultima  = cfg->altitudo - I;
    tabulas_ordinare(a);
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
            casus SERIES_OSC:
                seriem_osc(a, &lexema);
                frange;
            casus SERIES_SS:
                seriem_ss(a, &lexema);
                frange;
            casus SERIES_FUGA:
                /* ESC N|O ante octetum altum (runa UTF-8): invocatio
                 * singularis tamen valet */
                si (   lexema.introductor == 'N'
                    || lexema.introductor == 'O')
                {
                    seriem_ss(a, &lexema);
                }
                alioquin
                {
                    a->ignota++;
                }
                frange;
            ordinarius:
                a->ignota++;
                frange;
        }
    }
}

/* linea sine textu (stilus fundi non textus est - Ghostty) */
interior b32
lineam_vacuam_esse (
     constans Aemulator* a,
         constans Linea* l)
{
    i32 x;

    per (x = ZEPHYRUM; x < a->latitudo; x++)
    {
        si (l->cellulae[x].mensura > ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* crescens cursore in ima linea (Ghostty 'pull'): k lineae historiae
 * supra schirmum redeunt - schirmum deorsum volvitur (lineae vacuae
 * novae imae abeunt), summae ex historia implentur, cursor descendit.
 * Post magnitudinem novam positam: collectio stilorum dimensiones
 * veras videt. */
interior vacuum
historiam_retrahere (
    Aemulator* a,
          i32  k)
{
    Schirmum* activum;
         i32  i;

    si (k == ZEPHYRUM)
    {
        redde;
    }
    activum     = a->activum;
    a->activum  = &a->primarium;
    regionem_deorsum(a, ZEPHYRUM, a->altitudo - I, k);
    a->activum  = activum;
    per (i = k; i > ZEPHYRUM; i--)
    {
        lineam_ex_historia(a, a->primarium.lineae[i - I]);
    }
    a->primarium.cursor.y += k;
    si (a->visus > a->historia_lineae)
    {
        a->visus = a->historia_lineae;
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

    /* minuens (Ghostty resizeWithoutReflow, mos Terminal.app): lineae
     * vacuae finales infra cursorem primum praeciduntur; quod restat
     * summas in historiam mittit (textus infra cursorem servatur),
     * cursor cum linea sua ascendit (refluxus dilatus - decisio VII) */
    si (altitudo < a->altitudo)
    {
        translatio = a->altitudo - altitudo;
        per (y = a->altitudo - I;
             y > s->cursor.y && translatio > ZEPHYRUM
             && lineam_vacuam_esse(a, s->lineae[y]);
             y--)
        {
            translatio--;
        }
        si (translatio > ZEPHYRUM)
        {
            activum     = a->activum;
            a->activum  = s;
            regionem_sursum(a, ZEPHYRUM, a->altitudo - I, translatio,
                            VERUM);
            a->activum  = activum;
            s->cursor.y = s->cursor.y >= translatio
                        ? s->cursor.y - translatio : ZEPHYRUM;
        }
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
            i8*  tabulae;
           i32   cap_lat;
           i32   cap_alt;
           i32   y;
           i32   trahendae;

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
        tabulae  = (i8*)piscina_conari_allocare(a->piscina,
            (memoriae_index)cap_lat);
        si (!primae || !alterae || !tabulae)
        {
            redde FALSUM;
        }
        a->tabulae = tabulae;
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
    /* retractio: solum crescens, cursore primarii in ima linea */
    trahendae = ZEPHYRUM;
    si (   altitudo > a->altitudo
        && a->primarium.cursor.y + I >= a->altitudo)
    {
        trahendae = altitudo - a->altitudo;
        si (trahendae > a->historia_lineae)
        {
            trahendae = a->historia_lineae;
        }
    }
    schirmum_aptare(a, &a->primarium, latitudo, altitudo);
    schirmum_aptare(a, &a->alterum, latitudo, altitudo);
    a->latitudo = latitudo;
    a->altitudo = altitudo;
    historiam_retrahere(a, trahendae);
    /* Ghostty resize: regio et sistae ad ordinem redeunt */
    a->regio_summa   = ZEPHYRUM;
    a->regio_ultima  = altitudo - I;
    tabulas_ordinare(a);
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
    c.visibilis  = modus_est(a, MODUS_VISIBILIS);
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
    s32 k;

    si (!a)
    {
        redde FALSUM;
    }
    k = modum_invenire(numerus, privatus);
    redde k >= ZEPHYRUM && modus_est(a, (i32)k);
}

AemulatorModi
aemulator_modi (
    constans Aemulator* a)
{
    AemulatorModi m;

    memset(&m, ZEPHYRUM, magnitudo(AemulatorModi));
    si (!a)
    {
        redde m;
    }
    m.sagittae_applicationis  = modus_est(a, MODUS_SAGITTAE);
    m.tabula_applicationis    = modus_est(a, MODUS_TABULA);
    m.mus                     = a->mus;
    m.mus_forma               = a->mus_forma;
    m.glutinum                = modus_est(a, MODUS_GLUTINUM);
    m.focus                   = modus_est(a, MODUS_FOCUS);
    m.lnm                     = modus_est(a, MODUS_LINEA_NOVA);
    redde m;
}

i32
aemulator_ignota (
    constans Aemulator* a)
{
    redde a ? a->ignota : ZEPHYRUM;
}

/* Ghostty formatter (plain, unwrap=false, trim=false) super lineas
 * absolutas [ab, ad): vacuae ante textum spatia, scripta spatia manent,
 * caudae et capita omittuntur, lineae sine textu solum ante lineam cum
 * textu */
interior chorda
lineas_effundere (
    constans Aemulator* a,
               Piscina* piscina,
                   i32  ab,
                   i32  ad)
{
                       chorda  ex;
                           i8* b;
               memoriae_index  capacitas;
                          i32  n;
                          i32  r;
                          i32  x;
                          i32  k;
                          i32  lat;
                          b32  involuta;
                          i32  lineae_vacuae;
                          i32  cellulae_vacuae;
                          b32  scriptum;
             constans Cellula* l;
             constans Cellula* c;
    constans StilusTerminalis* stili;

    ex.datum    = NIHIL;
    ex.mensura  = ZEPHYRUM;
    capacitas   = I;
    per (r = ab; r < ad; r++)
    {
        (vacuum)lineam_absolutam(a, r, &lat, &stili, &involuta);
        capacitas += (memoriae_index)lat * IV + I;
    }
    b = (i8*)piscina_conari_allocare(piscina, capacitas);
    si (!b)
    {
        redde ex;
    }
    n              = ZEPHYRUM;
    lineae_vacuae  = ZEPHYRUM;
    scriptum       = FALSUM;
    per (r = ab; r < ad; r++)
    {
        l = lineam_absolutam(a, r, &lat, &stili, &involuta);
        /* linea sine textu: numeratur */
        k = ZEPHYRUM;
        per (x = ZEPHYRUM; l && x < lat; x++)
        {
            si (l[x].mensura > ZEPHYRUM)
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
        per (x = ZEPHYRUM; x < lat; x++)
        {
            c = &l[x];
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

chorda
aemulator_textum_effundere (
    constans Aemulator* a,
               Piscina* piscina)
{
    chorda ex;
       i32 h;

    ex.datum    = NIHIL;
    ex.mensura  = ZEPHYRUM;
    si (!a || !piscina)
    {
        redde ex;
    }
    h = historia_activa(a);
    redde lineas_effundere(a, piscina, h, h + a->altitudo);
}


/* ==================================================
 * Historia et visus (phasis C)
 * ================================================== */

i32
aemulator_historia (
    constans Aemulator* a)
{
    redde a ? historia_activa(a) : ZEPHYRUM;
}

chorda
aemulator_historiam_effundere (
    constans Aemulator* a,
               Piscina* piscina)
{
    chorda ex;

    ex.datum    = NIHIL;
    ex.mensura  = ZEPHYRUM;
    si (!a || !piscina)
    {
        redde ex;
    }
    redde lineas_effundere(a, piscina, ZEPHYRUM,
        historia_activa(a) + a->altitudo);
}

i32
aemulator_visus (
    constans Aemulator* a)
{
    redde a ? a->visus : ZEPHYRUM;
}

vacuum
aemulator_visum_movere (
    Aemulator* a,
          s32  delta)
{
    s32 v;
    s32 h;

    si (!a)
    {
        redde;
    }
    h = (s32)historia_activa(a);
    v = (s32)a->visus + delta;
    si (v < ZEPHYRUM)
    {
        v = ZEPHYRUM;
    }
    si (v > h)
    {
        v = h;
    }
    a->visus = (i32)v;
}

b32
aemulator_visus_cellula (
    constans Aemulator* a,
                   i32  x,
                   i32  y,
      AemulatorCellula* cellula)
{
                      Cellula* l;
    constans StilusTerminalis* stili;
                          i32  lat;
                          b32  involuta;

    si (!a || !cellula || x >= a->latitudo || y >= a->altitudo)
    {
        redde FALSUM;
    }
    l = lineam_absolutam(a, historia_activa(a) - a->visus + y, &lat,
        &stili, &involuta);
    si (!l || x >= lat)
    {
        /* linea historiae angustior (latitudo prior): cellula vacua */
        cellula->graphema.datum    = NIHIL;
        cellula->graphema.mensura  = ZEPHYRUM;
        cellula->latitudo          = AEMULATOR_ANGUSTA;
        stilus_nativus(&cellula->stilus);
        redde VERUM;
    }
    cellula->graphema.datum    = l[x].octeti;
    cellula->graphema.mensura  = (i32)l[x].mensura;
    cellula->latitudo          = (AemulatorLatitudo)l[x].latitudo;
    cellula->stilus            = stili[l[x].stilus];
    redde VERUM;
}

b32
aemulator_visus_involuta (
    constans Aemulator* a,
                   i32  y)
{
    constans StilusTerminalis* stili;
                          i32  lat;
                          b32  involuta;

    si (!a || y >= a->altitudo)
    {
        redde FALSUM;
    }
    (vacuum)lineam_absolutam(a, historia_activa(a) - a->visus + y, &lat,
        &stili, &involuta);
    redde involuta;
}

chorda
aemulator_visum_effundere (
    constans Aemulator* a,
               Piscina* piscina)
{
    chorda ex;
       i32 ab;

    ex.datum    = NIHIL;
    ex.mensura  = ZEPHYRUM;
    si (!a || !piscina)
    {
        redde ex;
    }
    ab = historia_activa(a) - a->visus;
    redde lineas_effundere(a, piscina, ab, ab + a->altitudo);
}
