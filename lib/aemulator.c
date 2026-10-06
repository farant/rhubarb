/* aemulator.c - nucleus emulatoris terminalis (aemulator-plan A1)
 *
 * A1: creatio, cellulae et lineae, impressio per runae (angusta, lata,
 * caudae, caput, involutio pendens), CR LF BS HT BEL, mutatio
 * magnitudinis, effusio plana. Series ceterae consumuntur et
 * numerantur (A2 eas tractat).
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

nomen structura {
               Linea** lineae;   /* indirectio: volutio indices rotat */
     AemulatorCursor   cursor;
} Schirmum;

structura Aemulator {
              Piscina* piscina;
    AemulatorEffectus  effectus;
                  i32  latitudo;
                  i32  altitudo;
                  i32  capacitas_latitudinis;
                  i32  capacitas_altitudinis;
             Schirmum  primarium;
         SeriesLector* lector;
                   i8  residuum[IV];      /* runa UTF-8 scissa */
                  i32  residuum_mensura;
                  i32  ignota;
                  b32  involutio;         /* modus DEC VII */
};


/* ==================================================
 * Memoria
 * ================================================== */

interior vacuum
cellulam_vacare (
    Cellula* c)
{
    memset(c, ZEPHYRUM, magnitudo(Cellula));
}

interior vacuum
lineam_vacare (
      Linea* l,
        i32  ab,
        i32  ad)
{
    i32 x;

    per (x = ab; x < ad; x++)
    {
        cellulam_vacare(&l->cellulae[x]);
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

    s                  = &a->primarium;
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
    lineam_vacare(summa, ZEPHYRUM, a->capacitas_latitudinis);
    summa->involuta = FALSUM;
}

/* involutio: linea currens in proximam continuat */
interior vacuum
lineam_involvere (
    Aemulator* a)
{
    Schirmum* s;

    s                                 = &a->primarium;
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

    l = a->primarium.lineae[y];
    c = &l->cellulae[x];
    si (c->latitudo == AEMULATOR_CAUDA && x > ZEPHYRUM)
    {
        cellulam_vacare(&l->cellulae[x - I]);
    }
    si (c->latitudo == AEMULATOR_LATA && x + I < a->latitudo)
    {
        cellulam_vacare(&l->cellulae[x + I]);
    }
    cellulam_vacare(c);
    memcpy(c->octeti, octeti, (memoriae_index)mensura);
    c->mensura   = (i8)mensura;
    c->latitudo  = (i8)(latitudo == II ? AEMULATOR_LATA
                                       : AEMULATOR_ANGUSTA);
    si (latitudo == II)
    {
        c = &l->cellulae[x + I];
        si (c->latitudo == AEMULATOR_LATA && x + II < a->latitudo)
        {
            cellulam_vacare(&l->cellulae[x + II]);
        }
        cellulam_vacare(c);
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

    s         = &a->primarium;
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
        cellulam_vacare(&s->lineae[s->cursor.y]->cellulae[s->cursor.x]);
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

    s = &a->primarium;
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
    a->primarium.lineae = lineas_struere(piscina, cfg->latitudo,
                                         cfg->altitudo);
    a->primarium.cursor.visibilis  = VERUM;
    a->lector                      = series_lectorem_creare(piscina);
    si (!a->primarium.lineae || !a->lector)
    {
        redde NIHIL;
    }
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
            ordinarius:
                a->ignota++;
                frange;
        }
    }
}

b32
aemulator_amplitudo (
    Aemulator* a,
          i32  latitudo,
          i32  altitudo)
{
       Linea** novae;
    Schirmum*  s;
         i32   cap_lat;
         i32   cap_alt;
         i32   y;
         i32   translatio;

    si (   !a || latitudo < I || latitudo > LATUS_MAXIMUM
        || altitudo < I || altitudo > LATUS_MAXIMUM)
    {
        redde FALSUM;
    }
    s = &a->primarium;
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
        novae = lineas_struere(a->piscina, cap_lat, cap_alt);
        si (!novae)
        {
            redde FALSUM;
        }
        per (y = ZEPHYRUM; y < a->altitudo; y++)
        {
            memcpy(novae[y]->cellulae, s->lineae[y]->cellulae,
                (memoriae_index)a->latitudo * magnitudo(Cellula));
            novae[y]->involuta = s->lineae[y]->involuta;
        }
        s->lineae                 = novae;
        a->capacitas_latitudinis  = cap_lat;
        a->capacitas_altitudinis  = cap_alt;
    }
    /* cursor in schirmo manet: lineae summae abeunt (refluxus dilatus
     * - decisio VII) */
    si (s->cursor.y >= altitudo)
    {
        translatio = s->cursor.y - altitudo + I;
        per (y = ZEPHYRUM; y < translatio; y++)
        {
            indicem_movere(a);
            s->cursor.y--;
        }
    }
    /* columnae novae et lineae novae vacuae */
    si (latitudo > a->latitudo)
    {
        per (y = ZEPHYRUM; y < a->altitudo; y++)
        {
            lineam_vacare(s->lineae[y], a->latitudo, latitudo);
        }
    }
    per (y = a->altitudo; y < altitudo; y++)
    {
        lineam_vacare(s->lineae[y], ZEPHYRUM, a->capacitas_latitudinis);
        s->lineae[y]->involuta = FALSUM;
    }
    a->latitudo = latitudo;
    a->altitudo = altitudo;
    si (s->cursor.x >= latitudo)
    {
        s->cursor.x = latitudo - I;
    }
    s->cursor.pendens = FALSUM;
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
    redde a->primarium.cursor;
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
    c                          = &a->primarium.lineae[y]->cellulae[x];
    cellula->graphema.datum    = c->octeti;
    cellula->graphema.mensura  = (i32)c->mensura;
    cellula->latitudo          = (AemulatorLatitudo)c->latitudo;
    stilus_nativus(&cellula->stilus);
    redde VERUM;
}

b32
aemulator_alterum (
    constans Aemulator* a)
{
    (vacuum)a;
    redde FALSUM;
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
    si (numerus == VII)
    {
        redde a->involutio;
    }
    si (numerus == XXV)
    {
        redde a->primarium.cursor.visibilis;
    }
    redde FALSUM;
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
        /* linea sine textu: numeratur */
        k = ZEPHYRUM;
        per (x = ZEPHYRUM; x < a->latitudo; x++)
        {
            si (a->primarium.lineae[y]->cellulae[x].mensura > ZEPHYRUM)
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
            c = &a->primarium.lineae[y]->cellulae[x];
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
