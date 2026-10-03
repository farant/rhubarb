/* series_terminalis.c - Vide series_terminalis.h
 *
 * Machina vt100.net (Williams), regulae ex Ghostty parse_table.zig
 * (MIT,
 * pin 12752b2) sine C1, cum divergentiis capitis. Tabula [octetus]
 * [status] -> (status novus, actio) ad creationem struitur
 * (_tabulam_struere: regulae legibiles ut diagramma). ESC, CAN, SUB et
 * terminatores chordarum ANTE tabulam tractantur (abruptio, FUGA,
 * terminator): ibi divergentiae habitant.
 */

#include "series_terminalis.h"
#include <string.h>

nomen enumeratio {
    STATUS_SOLUM = ZEPHYRUM,
    STATUS_FUGAE,               /* post ESC */
    STATUS_FUGAE_INTERMEDIA,
    STATUS_CSI_INITIUM,
    STATUS_CSI_PARAMETRUM,
    STATUS_CSI_INTERMEDIA,
    STATUS_CSI_IGNORARE,
    STATUS_SS,                  /* post ESC N|O (divergentia) */
    STATUS_DCS_INITIUM,
    STATUS_DCS_PARAMETRUM,
    STATUS_DCS_INTERMEDIA,
    STATUS_DCS_TRANSITUS,       /* corpus DCS */
    STATUS_DCS_IGNORARE,
    STATUS_OSC,
    STATUS_APC,                 /* SOS / PM / APC */
    STATUS_NUMERUS
} SeriesStatus;

nomen enumeratio {
    ACTIO_NULLA = ZEPHYRUM,
    ACTIO_IGNORARE,
    ACTIO_IMPRIMERE,
    ACTIO_EXSEQUI,
    ACTIO_COLLIGERE,
    ACTIO_PRIVATUM,
    ACTIO_PARAMETRUM,
    ACTIO_ESC,
    ACTIO_CSI,
    ACTIO_SS,
    ACTIO_DCS_INCIPERE,
    ACTIO_PONERE
} SeriesActio;

structura SeriesLector {
    /* tabula transitionum: [octetus][status] */
     i8 tabula_status[CCLVI][STATUS_NUMERUS];
     i8 tabula_actio[CCLVI][STATUS_NUMERUS];

    i32 status;
    b32 initus;           /* modus initus (B1b): vide caput */
    b32 post_chordam;     /* ESC chordam clausit: '\' terminator */
    b32 crudum_esc;       /* proxima vocatio crudum = "ESC" incipit */

    /* series in constructione */
     s32 parametra[SERIES_PARAMETRA_MAXIMA];
     i32 numerus_parametrorum;
     i32 separatores;
     s32 accumulator;
     i32 digiti;            /* digiti in accumulatore */
      i8 intermedia[SERIES_INTERMEDIA_MAXIMA];
     i32 numerus_intermediorum;
      i8 privatum;
      i8 introductor;
      i8 finale_dcs;
     b32 praefixum;
     b32 truncatum;

      i8 chorda[SERIES_CHORDA_MAXIMA];
     i32 chorda_mensura;
      i8 crudum[SERIES_CRUDUM_MAXIMUM];
     i32 crudum_mensura;
};


/* ==================================================
 * Tabula
 * ================================================== */

interior vacuum
_regula (
    SeriesLector* lx,
             i32  ab,
             i32  ad,
             i32  status,
             i32  status_novus,
             i32  actio)
{
    i32 c;

    per (c = ab; c <= ad; c++)
    {
        lx->tabula_status[c][status]  = (i8)status_novus;
        lx->tabula_actio[c][status]   = (i8)actio;
    }
}

/* C0 sine CAN (0x18), SUB (0x1A), ESC (0x1B): illi ante tabulam */
interior vacuum
_regula_c0 (
    SeriesLector* lx,
             i32  status,
             i32  actio)
{
    _regula(lx, 0x00, 0x17, status, status, actio);
    _regula(lx, 0x19, 0x19, status, status, actio);
    _regula(lx, 0x1C, 0x1F, status, status, actio);
}

interior vacuum
_tabulam_struere (
    SeriesLector* lx)
{
    i32 s;

    /* ordinarium: status manet, nihil agitur (Ghostty .none) */
    per (s = ZEPHYRUM; s < STATUS_NUMERUS; s++)
    {
        _regula(lx, 0x00, 0xFF, s, s, ACTIO_NULLA);
    }

    /* solum: C0 et DEL exsequi (div: DEL); 0x80+ imprimere (div: C1
     * non agnita) */
    _regula_c0(lx, STATUS_SOLUM, ACTIO_EXSEQUI);
    /* CAN, SUB in solo: regimina ut cetera (in serie abrumpunt) */
    _regula(lx, 0x18, 0x18, STATUS_SOLUM, STATUS_SOLUM, ACTIO_EXSEQUI);
    _regula(lx, 0x1A, 0x1A, STATUS_SOLUM, STATUS_SOLUM, ACTIO_EXSEQUI);
    _regula(lx, 0x20, 0x7E, STATUS_SOLUM, STATUS_SOLUM,
        ACTIO_IMPRIMERE);
    _regula(lx, 0x7F, 0x7F, STATUS_SOLUM, STATUS_SOLUM, ACTIO_EXSEQUI);
    _regula(lx, 0x80, 0xFF, STATUS_SOLUM, STATUS_SOLUM,
        ACTIO_IMPRIMERE);

    /* fugae (post ESC) */
    _regula_c0(lx, STATUS_FUGAE, ACTIO_EXSEQUI);
    _regula(lx, 0x7F, 0x7F, STATUS_FUGAE, STATUS_FUGAE, ACTIO_IGNORARE);
    _regula(lx, 0x20, 0x2F, STATUS_FUGAE, STATUS_FUGAE_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x30, 0x4D, STATUS_FUGAE, STATUS_SOLUM, ACTIO_ESC);
    _regula(lx, 0x4E, 0x4F, STATUS_FUGAE, STATUS_SS, ACTIO_NULLA);
    _regula(lx, 0x50, 0x50, STATUS_FUGAE, STATUS_DCS_INITIUM,
        ACTIO_NULLA);
    _regula(lx, 0x51, 0x57, STATUS_FUGAE, STATUS_SOLUM, ACTIO_ESC);
    _regula(lx, 0x58, 0x58, STATUS_FUGAE, STATUS_APC, ACTIO_NULLA);
    _regula(lx, 0x59, 0x5A, STATUS_FUGAE, STATUS_SOLUM, ACTIO_ESC);
    _regula(lx, 0x5B, 0x5B, STATUS_FUGAE, STATUS_CSI_INITIUM,
        ACTIO_NULLA);
    _regula(lx, 0x5C, 0x5C, STATUS_FUGAE, STATUS_SOLUM, ACTIO_ESC);
    _regula(lx, 0x5D, 0x5D, STATUS_FUGAE, STATUS_OSC, ACTIO_NULLA);
    _regula(lx, 0x5E, 0x5F, STATUS_FUGAE, STATUS_APC, ACTIO_NULLA);
    _regula(lx, 0x60, 0x7E, STATUS_FUGAE, STATUS_SOLUM, ACTIO_ESC);

    /* fugae intermedia */
    _regula_c0(lx, STATUS_FUGAE_INTERMEDIA, ACTIO_EXSEQUI);
    _regula(lx, 0x20, 0x2F, STATUS_FUGAE_INTERMEDIA,
        STATUS_FUGAE_INTERMEDIA, ACTIO_COLLIGERE);
    _regula(lx, 0x7F, 0x7F, STATUS_FUGAE_INTERMEDIA,
        STATUS_FUGAE_INTERMEDIA, ACTIO_IGNORARE);
    _regula(lx, 0x30, 0x7E, STATUS_FUGAE_INTERMEDIA, STATUS_SOLUM,
        ACTIO_ESC);

    /* csi initium */
    _regula_c0(lx, STATUS_CSI_INITIUM, ACTIO_EXSEQUI);
    _regula(lx, 0x7F, 0x7F, STATUS_CSI_INITIUM, STATUS_CSI_INITIUM,
        ACTIO_IGNORARE);
    _regula(lx, 0x40, 0x7E, STATUS_CSI_INITIUM, STATUS_SOLUM,
        ACTIO_CSI);
    _regula(lx, 0x3A, 0x3A, STATUS_CSI_INITIUM, STATUS_CSI_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x20, 0x2F, STATUS_CSI_INITIUM, STATUS_CSI_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x30, 0x39, STATUS_CSI_INITIUM, STATUS_CSI_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x3B, 0x3B, STATUS_CSI_INITIUM, STATUS_CSI_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x3C, 0x3F, STATUS_CSI_INITIUM, STATUS_CSI_PARAMETRUM,
        ACTIO_PRIVATUM);

    /* csi parametrum */
    _regula_c0(lx, STATUS_CSI_PARAMETRUM, ACTIO_EXSEQUI);
    _regula(lx, 0x30, 0x3B, STATUS_CSI_PARAMETRUM,
        STATUS_CSI_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x7F, 0x7F, STATUS_CSI_PARAMETRUM,
        STATUS_CSI_PARAMETRUM,
        ACTIO_IGNORARE);
    _regula(lx, 0x40, 0x7E, STATUS_CSI_PARAMETRUM, STATUS_SOLUM,
        ACTIO_CSI);
    _regula(lx, 0x3C, 0x3F, STATUS_CSI_PARAMETRUM, STATUS_CSI_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x20, 0x2F, STATUS_CSI_PARAMETRUM,
        STATUS_CSI_INTERMEDIA,
        ACTIO_COLLIGERE);

    /* csi intermedia */
    _regula_c0(lx, STATUS_CSI_INTERMEDIA, ACTIO_EXSEQUI);
    _regula(lx, 0x20, 0x2F, STATUS_CSI_INTERMEDIA,
        STATUS_CSI_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x7F, 0x7F, STATUS_CSI_INTERMEDIA,
        STATUS_CSI_INTERMEDIA,
        ACTIO_IGNORARE);
    _regula(lx, 0x40, 0x7E, STATUS_CSI_INTERMEDIA, STATUS_SOLUM,
        ACTIO_CSI);
    _regula(lx, 0x30, 0x3F, STATUS_CSI_INTERMEDIA, STATUS_CSI_IGNORARE,
        ACTIO_NULLA);

    /* csi ignorare: series mala tota consumitur, tacite (Ghostty) */
    _regula_c0(lx, STATUS_CSI_IGNORARE, ACTIO_EXSEQUI);
    _regula(lx, 0x20, 0x3F, STATUS_CSI_IGNORARE, STATUS_CSI_IGNORARE,
        ACTIO_IGNORARE);
    _regula(lx, 0x7F, 0x7F, STATUS_CSI_IGNORARE, STATUS_CSI_IGNORARE,
        ACTIO_IGNORARE);
    _regula(lx, 0x40, 0x7E, STATUS_CSI_IGNORARE, STATUS_SOLUM,
        ACTIO_IGNORARE);

    /* ss (div): ESC N|O parametra? finale - 'ESC O 2 P' */
    _regula_c0(lx, STATUS_SS, ACTIO_EXSEQUI);
    _regula(lx, 0x30, 0x39, STATUS_SS, STATUS_SS, ACTIO_PARAMETRUM);
    _regula(lx, 0x3B, 0x3B, STATUS_SS, STATUS_SS, ACTIO_PARAMETRUM);
    _regula(lx, 0x7F, 0x7F, STATUS_SS, STATUS_SS, ACTIO_IGNORARE);
    _regula(lx, 0x20, 0x2F, STATUS_SS, STATUS_SOLUM, ACTIO_SS);
    _regula(lx, 0x3A, 0x3A, STATUS_SS, STATUS_SOLUM, ACTIO_SS);
    _regula(lx, 0x3C, 0x7E, STATUS_SS, STATUS_SOLUM, ACTIO_SS);

    /* dcs initium */
    _regula_c0(lx, STATUS_DCS_INITIUM, ACTIO_IGNORARE);
    _regula(lx, 0x7F, 0x7F, STATUS_DCS_INITIUM, STATUS_DCS_INITIUM,
        ACTIO_IGNORARE);
    _regula(lx, 0x20, 0x2F, STATUS_DCS_INITIUM, STATUS_DCS_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x3A, 0x3A, STATUS_DCS_INITIUM, STATUS_DCS_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x30, 0x39, STATUS_DCS_INITIUM, STATUS_DCS_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x3B, 0x3B, STATUS_DCS_INITIUM, STATUS_DCS_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x3C, 0x3F, STATUS_DCS_INITIUM, STATUS_DCS_PARAMETRUM,
        ACTIO_PRIVATUM);
    _regula(lx, 0x40, 0x7E, STATUS_DCS_INITIUM, STATUS_DCS_TRANSITUS,
        ACTIO_DCS_INCIPERE);

    /* dcs parametrum */
    _regula_c0(lx, STATUS_DCS_PARAMETRUM, ACTIO_IGNORARE);
    _regula(lx, 0x30, 0x39, STATUS_DCS_PARAMETRUM,
        STATUS_DCS_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x3B, 0x3B, STATUS_DCS_PARAMETRUM,
        STATUS_DCS_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x7F, 0x7F, STATUS_DCS_PARAMETRUM,
        STATUS_DCS_PARAMETRUM,
        ACTIO_IGNORARE);
    _regula(lx, 0x3A, 0x3A, STATUS_DCS_PARAMETRUM, STATUS_DCS_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x3C, 0x3F, STATUS_DCS_PARAMETRUM, STATUS_DCS_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x20, 0x2F, STATUS_DCS_PARAMETRUM,
        STATUS_DCS_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x40, 0x7E, STATUS_DCS_PARAMETRUM, STATUS_DCS_TRANSITUS,
        ACTIO_DCS_INCIPERE);

    /* dcs intermedia */
    _regula_c0(lx, STATUS_DCS_INTERMEDIA, ACTIO_IGNORARE);
    _regula(lx, 0x20, 0x2F, STATUS_DCS_INTERMEDIA,
        STATUS_DCS_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x7F, 0x7F, STATUS_DCS_INTERMEDIA,
        STATUS_DCS_INTERMEDIA,
        ACTIO_IGNORARE);
    _regula(lx, 0x30, 0x3F, STATUS_DCS_INTERMEDIA, STATUS_DCS_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x40, 0x7E, STATUS_DCS_INTERMEDIA, STATUS_DCS_TRANSITUS,
        ACTIO_DCS_INCIPERE);

    /* dcs transitus (corpus) et ignorare */
    _regula_c0(lx, STATUS_DCS_TRANSITUS, ACTIO_PONERE);
    _regula(lx, 0x20, 0x7E, STATUS_DCS_TRANSITUS, STATUS_DCS_TRANSITUS,
        ACTIO_PONERE);
    _regula(lx, 0x7F, 0x7F, STATUS_DCS_TRANSITUS, STATUS_DCS_TRANSITUS,
        ACTIO_IGNORARE);
    _regula(lx, 0x80, 0xFF, STATUS_DCS_TRANSITUS, STATUS_DCS_TRANSITUS,
        ACTIO_PONERE);
    _regula(lx, 0x00, 0xFF, STATUS_DCS_IGNORARE, STATUS_DCS_IGNORARE,
        ACTIO_IGNORARE);

    /* osc: C0 ignorantur (BEL ante tabulam terminat); 0x20+ corpus */
    _regula_c0(lx, STATUS_OSC, ACTIO_IGNORARE);
    _regula(lx, 0x20, 0xFF, STATUS_OSC, STATUS_OSC, ACTIO_PONERE);

    /* apc / pm / sos: omnia corpus (BEL quoque) */
    _regula_c0(lx, STATUS_APC, ACTIO_PONERE);
    _regula(lx, 0x20, 0xFF, STATUS_APC, STATUS_APC, ACTIO_PONERE);
}


/* ==================================================
 * Auxilia
 * ================================================== */

/* Visus sine copia: chorda.datum non-constans est, initus et
 * tabulae lectoris constantia (unio contra -Wcast-qual, mos domus) */
interior chorda
_visus (
    constans i8* octeti,
            i32  mensura)
{
    chorda c;
    unio { constans i8* l; i8* m; } u;

    u.l        = octeti;
    c.datum    = u.m;
    c.mensura  = mensura;
    redde c;
}

interior vacuum
_seriem_vacare (
    SeriesLector* lx)
{
    lx->numerus_parametrorum   = ZEPHYRUM;
    lx->separatores            = ZEPHYRUM;
    lx->accumulator            = ZEPHYRUM;
    lx->digiti                 = ZEPHYRUM;
    lx->numerus_intermediorum  = ZEPHYRUM;
    lx->privatum               = ZEPHYRUM;
    lx->introductor            = ZEPHYRUM;
    lx->finale_dcs             = ZEPHYRUM;
    lx->praefixum              = FALSUM;
    lx->truncatum              = FALSUM;
    lx->chorda_mensura         = ZEPHYRUM;
}

interior vacuum
_crudum_addere (
    SeriesLector* lx,
              i8  c)
{
    si (lx->crudum_mensura < SERIES_CRUDUM_MAXIMUM)
    {
        lx->crudum[lx->crudum_mensura] = c;
        lx->crudum_mensura++;
    }
}

interior vacuum
_chordae_addere (
    SeriesLector* lx,
             i8  c)
{
    si (lx->chorda_mensura < SERIES_CHORDA_MAXIMA)
    {
        lx->chorda[lx->chorda_mensura] = c;
        lx->chorda_mensura++;
    }
    alioquin
    {
        lx->truncatum = VERUM;
    }
}

/* Accumulatorem in parametrum vertere (si digiti adsunt) */
interior vacuum
_parametrum_figere (
    SeriesLector* lx)
{
    si (   lx->digiti > ZEPHYRUM
        && lx->numerus_parametrorum < SERIES_PARAMETRA_MAXIMA)
    {
        lx->parametra[lx->numerus_parametrorum] = lx->accumulator;
        lx->numerus_parametrorum++;
    }
}

interior vacuum
_parametrum (
    SeriesLector* lx,
             i32  c)
{
    s32 d;

    si (c == ';' || c == ':')
    {
        /* nimia: separator neglectus; series in fine abicitur */
        si (lx->numerus_parametrorum >= SERIES_PARAMETRA_MAXIMA)
        {
            redde;
        }
        lx->parametra[lx->numerus_parametrorum] = lx->accumulator;
        si (c == ':')
        {
            lx->separatores = lx->separatores
                | ((i32)I << lx->numerus_parametrorum);
        }
        lx->numerus_parametrorum++;
        lx->accumulator  = ZEPHYRUM;
        lx->digiti       = ZEPHYRUM;
        redde;
    }
    d = (s32)(c - '0');
    si (lx->accumulator > (SERIES_PARAMETRUM_MAXIMUM - d) / X)
    {
        lx->accumulator = SERIES_PARAMETRUM_MAXIMUM;
    }
    alioquin
    {
        lx->accumulator = lx->accumulator * X + d;
    }
    lx->digiti++;
}

/* Lexema ex statu seriei (parametra, intermedia, crudum ...) */
interior vacuum
_lexema_implere (
    constans SeriesLector* lx,
             SeriesLexema* l,
              SeriesGenus  genus,
                      i32  finale)
{
    memset(l, ZEPHYRUM, magnitudo(SeriesLexema));
    l->genus                 = genus;
    l->numerus_parametrorum  = lx->numerus_parametrorum;
    memcpy(l->parametra, lx->parametra,
        (memoriae_index)lx->numerus_parametrorum * magnitudo(s32));
    l->separatores            = lx->separatores;
    l->numerus_intermediorum  = lx->numerus_intermediorum;
    memcpy(l->intermedia, lx->intermedia,
        (memoriae_index)lx->numerus_intermediorum);
    l->privatum     = lx->privatum;
    l->introductor  = lx->introductor;
    l->finale       = (i8)finale;
    l->praefixum    = lx->praefixum;
    l->truncatum    = lx->truncatum;
    l->crudum       = _visus(lx->crudum, lx->crudum_mensura);
}

interior vacuum
_fugam_implere (
    constans SeriesLector* lx,
             SeriesLexema* l)
{
    _lexema_implere(lx, l, SERIES_FUGA, ZEPHYRUM);
    l->numerus_parametrorum   = ZEPHYRUM;
    l->separatores            = ZEPHYRUM;
    l->numerus_intermediorum  = ZEPHYRUM;
    l->privatum               = ZEPHYRUM;
    l->truncatum              = FALSUM;
}

/* Modus initus: series incepta (ESC P) re vera
 * 'alterum + introductor' erat - ESC finale introductor, octetus
 * currens NON consumptus. */
interior vacuum
_alterum_reddere (
    SeriesLector* lx,
    SeriesLexema* l,
              i8  finale)
{
    lx->introductor = ZEPHYRUM;
    _lexema_implere(lx, l, SERIES_ESC, (i32)finale);
    l->numerus_parametrorum  = ZEPHYRUM;
    l->separatores           = ZEPHYRUM;
    lx->status               = STATUS_SOLUM;
}

interior b32
_est_chorda (
    i32 status)
{
    redde status == STATUS_OSC || status == STATUS_APC
        || status == STATUS_DCS_TRANSITUS
        || status == STATUS_DCS_IGNORARE;
}

/* Chordam clausam reddere (OSC, APC, DCS); DCS_IGNORARE: nihil */
interior b32
_chordam_reddere (
    constans SeriesLector* lx,
             SeriesLexema* l)
{
    SeriesGenus genus;
            i32 finale = ZEPHYRUM;

    si (lx->status == STATUS_DCS_IGNORARE)
    {
        redde FALSUM;
    }
    genus = (lx->status == STATUS_OSC) ? SERIES_OSC
          : (lx->status == STATUS_APC) ? SERIES_APC : SERIES_DCS;
    si (genus == SERIES_DCS)
    {
        finale = (i32)lx->finale_dcs;
    }
    _lexema_implere(lx, l, genus, finale);
    l->textus = _visus(lx->chorda, lx->chorda_mensura);
    redde VERUM;
}


/* ==================================================
 * Publica
 * ================================================== */

SeriesLector*
series_lectorem_creare (
    Piscina* piscina)
{
    SeriesLector* lx;

    lx = (SeriesLector*)piscina_allocare_ordinatum(piscina,
        magnitudo(SeriesLector), VIII);
    si (lx == NIHIL)
    {
        redde NIHIL;
    }
    _tabulam_struere(lx);
    lx->initus = FALSUM;
    series_lectorem_purgare(lx);
    redde lx;
}

vacuum
series_lectorem_initus_ponere (
    SeriesLector* lx,
             b32  initus)
{
    lx->initus = initus;
}

vacuum
series_lectorem_purgare (
    SeriesLector* lx)
{
    lx->status          = STATUS_SOLUM;
    lx->post_chordam    = FALSUM;
    lx->crudum_esc      = FALSUM;
    lx->crudum_mensura  = ZEPHYRUM;
    _seriem_vacare(lx);
}

SeriesGenus
series_lexema_proximum (
     SeriesLector*  lx,
      constans i8** ptr,
      constans i8*  finis,
     SeriesLexema*  l)
{
    /* ESC qui chordam clausit seriem proximam incipit (crudum) */
    si (lx->crudum_esc)
    {
        lx->crudum_esc      = FALSUM;
        lx->crudum[0]       = (i8)0x1B;
        lx->crudum_mensura  = I;
    }

    dum (*ptr < finis)
    {
        i32 c = ((i32)**ptr) & 0xFF;
        i32 status_novus;
        i32 actio;

        /* ---- ESC: initium, praefixum, terminator, abruptio ---- */
        si (c == 0x1B)
        {
            si (_est_chorda(lx->status))
            {
                b32 habet;

                (*ptr)++;
                habet             = _chordam_reddere(lx, l);
                lx->status        = STATUS_FUGAE;
                lx->post_chordam  = VERUM;
                lx->crudum_esc    = VERUM;
                _seriem_vacare(lx);
                si (habet)
                {
                    redde l->genus;
                }
                perge;
            }
            si (   lx->status == STATUS_SOLUM
                || (lx->status == STATUS_FUGAE && lx->post_chordam))
            {
                (*ptr)++;
                lx->status        = STATUS_FUGAE;
                lx->post_chordam  = FALSUM;
                _seriem_vacare(lx);
                lx->crudum[0]       = (i8)0x1B;
                lx->crudum_mensura  = I;
                perge;
            }
            si (lx->status == STATUS_FUGAE)
            {
                /* ESC ESC: praefixum (alterum + series) */
                (*ptr)++;
                lx->praefixum = VERUM;
                _crudum_addere(lx, (i8)c);
                perge;
            }
            /* series dimidia abrupta: FUGA, ESC non consumptus */
            _fugam_implere(lx, l);
            lx->status = STATUS_SOLUM;
            redde SERIES_FUGA;
        }

        /* ---- CAN, SUB: abruptio ---- */
        si ((c == 0x18 || c == 0x1A) && lx->status != STATUS_SOLUM)
        {
            si (lx->status == STATUS_FUGAE && lx->post_chordam)
            {
                lx->status        = STATUS_SOLUM;
                lx->post_chordam  = FALSUM;
                perge;
            }
            _fugam_implere(lx, l);
            lx->status = STATUS_SOLUM;
            redde SERIES_FUGA;
        }

        /* ---- post chordam: '\' terminator, alioquin series nova */
        si (lx->status == STATUS_FUGAE && lx->post_chordam)
        {
            si (c == '\\')
            {
                (*ptr)++;
                lx->status        = STATUS_SOLUM;
                lx->post_chordam  = FALSUM;
                perge;
            }
            lx->post_chordam = FALSUM;
        }

        /* ---- MODUS INITUS: alterum + clavis, non series ---- */
        si (lx->initus && lx->status == STATUS_FUGAE)
        {
            si (c < 0x20 || c == 0x7F)
            {
                /* alterum + regimen: FUGA, octetus non consumptus */
                _fugam_implere(lx, l);
                lx->status = STATUS_SOLUM;
                redde SERIES_FUGA;
            }
            si (   (c >= 0x20 && c <= 0x2F)
                || c == 'N' || c == 'X' || c == '^')
            {
                (*ptr)++;
                _crudum_addere(lx, (i8)c);
                lx->status = STATUS_SOLUM;
                _lexema_implere(lx, l, SERIES_ESC, c);
                redde SERIES_ESC;
            }
        }
        si (lx->initus)
        {
            si (   lx->status == STATUS_DCS_INITIUM
                && c >= 0x40 && c <= 0x7E
                && lx->numerus_parametrorum == ZEPHYRUM
                && lx->digiti == ZEPHYRUM && lx->privatum == ZEPHYRUM
                && lx->numerus_intermediorum == ZEPHYRUM)
            {
                _alterum_reddere(lx, l, (i8)'P');
                redde SERIES_ESC;
            }
        }

        /* ---- ESC + octetus altus: ESC solus (alterum + UTF-8) ---- */
        si (lx->status == STATUS_FUGAE && c >= 0x80)
        {
            _fugam_implere(lx, l);
            lx->status = STATUS_SOLUM;
            redde SERIES_FUGA;
        }

        /* ---- OSC: BEL terminat ---- */
        si (lx->status == STATUS_OSC && c == 0x07)
        {
            (*ptr)++;
            (vacuum)_chordam_reddere(lx, l);
            lx->status = STATUS_SOLUM;
            redde SERIES_OSC;
        }

        status_novus  = (i32)lx->tabula_status[c][lx->status];
        actio         = (i32)lx->tabula_actio[c][lx->status];

        /* ---- solum: cursus imprimibilis (visus in initum) ---- */
        si (actio == ACTIO_IMPRIMERE)
        {
            constans i8* initium = *ptr;

            dum (*ptr < finis)
            {
                i32 d = ((i32)**ptr) & 0xFF;

                si (lx->tabula_actio[d][STATUS_SOLUM]
                    != ACTIO_IMPRIMERE)
                {
                    frange;
                }
                (*ptr)++;
            }
            memset(l, ZEPHYRUM, magnitudo(SeriesLexema));
            l->genus   = SERIES_IMPRIMERE;
            l->textus  = _visus(initium, (i32)(*ptr - initium));
            redde SERIES_IMPRIMERE;
        }

        (*ptr)++;
        si (actio == ACTIO_EXSEQUI)
        {
            /* regimen: lexema proprium; series (si qua) manet */
            memset(l, ZEPHYRUM, magnitudo(SeriesLexema));
            l->genus   = SERIES_EXSEQUI;
            l->finale  = (i8)c;
            redde SERIES_EXSEQUI;
        }
        _crudum_addere(lx, (i8)c);

        /* introductor: octetus qui fugam in seriem vertit */
        si (   lx->status   == STATUS_FUGAE
            && status_novus != STATUS_FUGAE
            && status_novus != STATUS_FUGAE_INTERMEDIA
            && status_novus != STATUS_SOLUM)
        {
            lx->introductor     = (i8)c;
            lx->chorda_mensura  = ZEPHYRUM;
        }

        commutatio (actio)
        {
            casus ACTIO_COLLIGERE:
                si (lx->numerus_intermediorum
                    < SERIES_INTERMEDIA_MAXIMA)
                {
                    lx->intermedia[lx->numerus_intermediorum] = (i8)c;
                    lx->numerus_intermediorum++;
                }
                alioquin
                {
                    lx->truncatum = VERUM;
                }
                frange;

            casus ACTIO_PRIVATUM:
                lx->privatum = (i8)c;
                frange;

            casus ACTIO_PARAMETRUM:
                _parametrum(lx, c);
                frange;

            casus ACTIO_PONERE:
                _chordae_addere(lx, (i8)c);
                frange;

            casus ACTIO_DCS_INCIPERE:
                si (lx->numerus_parametrorum >= SERIES_PARAMETRA_MAXIMA)
                {
                    status_novus = STATUS_DCS_IGNORARE;
                }
                alioquin
                {
                    _parametrum_figere(lx);
                    lx->finale_dcs      = (i8)c;
                    lx->chorda_mensura  = ZEPHYRUM;
                }
                frange;

            casus ACTIO_ESC:
                lx->status = status_novus;
                _lexema_implere(lx, l, SERIES_ESC, c);
                redde SERIES_ESC;

            casus ACTIO_CSI:
            casus ACTIO_SS:
                lx->status = status_novus;
                si (lx->numerus_parametrorum >= SERIES_PARAMETRA_MAXIMA)
                {
                    perge;      /* nimia: tota abicitur (Ghostty) */
                }
                _parametrum_figere(lx);
                _lexema_implere(lx, l,
                    (actio == ACTIO_CSI) ? SERIES_CSI : SERIES_SS, c);
                redde l->genus;

            ordinarius:
                frange;
        }
        lx->status = status_novus;
    }
    redde SERIES_NIHIL;
}

b32
series_lector_pendet (
    constans SeriesLector* lx)
{
    redde (b32)(lx->status != STATUS_SOLUM);
}

b32
series_lectorem_evacuare (
    SeriesLector* lx,
    SeriesLexema* l)
{
    si (lx->status == STATUS_SOLUM)
    {
        redde FALSUM;
    }
    si (lx->status == STATUS_FUGAE && lx->post_chordam)
    {
        lx->status        = STATUS_SOLUM;
        lx->post_chordam  = FALSUM;
        redde FALSUM;
    }
    _fugam_implere(lx, l);
    lx->status = STATUS_SOLUM;
    redde VERUM;
}
