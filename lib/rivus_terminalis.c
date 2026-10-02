/* rivus_terminalis.c - Vide rivus_terminalis.h
 *
 * Pipeline ex tessera_eventum.c (B1b) translata in formam PURAM
 * (octeti trahuntur, non leguntur): regulae morae, reliquiae (H8),
 * canales crudi eaedem; semantica clavium/muris nunc in
 * interpres_terminalis.
 */

#include "rivus_terminalis.h"
#include "series_terminalis.h"
#include "utf8.h"
#include <string.h>

#define RELIQUIAE_NULLAE ZEPHYRUM
#define RELIQUIAE_FUGA   I      /* ESC solus, iam Effugium redditus */
#define RELIQUIAE_SGR    II     /* CSI < dimidia */

/* Cursus imprimibilis per passum: runa quaeque CLAVIS + TEXTUS, ergo
 * passus unus <= II * LXIV eventa - cauda (CCLVI) numquam superfluit */
#define CURSUS_MAXIMUS LXIV

#define CODEX_INITII_GLUTINI CC                 /* CSI 200 ~ */
#define TERMINUS_GLUTINI     "\033[201~"
#define TERMINI_LONGITUDO    ((i32)(magnitudo(TERMINUS_GLUTINI) - I))

structura RivusTerminalis {
                   i8  buffer[RIVUS_BUFFER];
                  i32  mensura;
         SeriesLector* series;
  InterpresTerminalis  interpres;
         EventusCauda* cauda;

    /* canales crudi */
             b32 x10_pendens;       /* post CSI M: tres octeti */
             b32 alienum_pendens;   /* post CSI [: usque ad finalem */

    /* reliquiae post moram (H8) */
              i8 reliquiae[SERIES_CRUDUM_MAXIMUM];
             i32 reliquiae_mensura;
             i32 reliquiae_genus;

    /* glutinum (?2004): corpus inter CSI 200~ et CSI 201~ */
             b32  glutinum_pendens;
              i8* glutinum;          /* RIVUS_GLUTINUM_CAPACITAS */
             i32  glutinum_mensura;
             i32  congruentes;       /* octeti termini congruentes */
             b32  glutinum_truncatum;

    /* modi declarati (RIVUS_MODUS_*), ZEPHYRUM = nulli */
             i32 modi_intrati;
};

#define MODI_OMNES (RIVUS_MODUS_MUS | RIVUS_MODUS_SUPER \
    | RIVUS_MODUS_GLUTINUM | RIVUS_MODUS_FOCUS | RIVUS_MODUS_KITTY)

/* vexilla impulsa (31): discernere, genera, alternae, omnes, textus */
#define KITTY_IMPULSA (INTERPRES_KITTY_DISCERNERE \
    | INTERPRES_KITTY_GENERA | INTERPRES_KITTY_ALTERNAE \
    | INTERPRES_KITTY_OMNES | INTERPRES_KITTY_TEXTUS)


/* ==================================================
 * Auxilia
 * ================================================== */

interior vacuum
_consumere (
    RivusTerminalis* r,
                i32  numerus)
{
    si (numerus >= r->mensura)
    {
        r->mensura = ZEPHYRUM;
        redde;
    }
    memmove(r->buffer, r->buffer + numerus,
        (memoriae_index)(r->mensura - numerus));
    r->mensura -= numerus;
}

/* Octeti modorum: intrandi ordine tabulae, exeundi ordine INVERSO.
 * MUS ?1006 post ?1003 (forma SGR ultima, ut tessera_modi.h). Tabula
 * INTRA functionem: amalgama quae modos non adhibet eam cum functione
 * demittit (staticum inusitatum = -Werror). */
interior i32
_modos_scribere (
    i32  modi,
    b32  exeundo,
     i8* buffer)
{
    hic_manens constans structura {
                       i32  modus;
        constans character* intrandi;
        constans character* exeundi;
    } MODI[] = {
        { RIVUS_MODUS_MUS,      "\033[?1000h", "\033[?1000l" },
        { RIVUS_MODUS_MUS,      "\033[?1002h", "\033[?1002l" },
        { RIVUS_MODUS_SUPER,    "\033[?1003h", "\033[?1003l" },
        { RIVUS_MODUS_MUS,      "\033[?1006h", "\033[?1006l" },
        { RIVUS_MODUS_GLUTINUM, "\033[?2004h", "\033[?2004l" },
        { RIVUS_MODUS_FOCUS,    "\033[?1004h", "\033[?1004l" },
        { RIVUS_MODUS_KITTY,    "\033[>31u",   "\033[<u"     }
    };
    s32 numerus  = (s32)(magnitudo(MODI) / magnitudo(MODI[0]));
    i32 n        = ZEPHYRUM;
    s32 j;

    per (j = ZEPHYRUM; j < numerus; j++)
    {
                        s32  k = exeundo ? numerus - I - j : j;
         constans character* t = exeundo ? MODI[k].exeundi
                                         : MODI[k].intrandi;

        si (MODI[k].modus & modi)
        {
            i32 longitudo = (i32)strlen(t);

            memcpy(buffer + n, t, (memoriae_index)longitudo);
            n += longitudo;
        }
    }
    redde n;
}

/* FACULTATES interpretis in caudam (primum fluxus; modi mutati) */
interior vacuum
_facultates_impellere (
    RivusTerminalis* r)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_FACULTATES;
    e.datum.facultates  = r->interpres.facultates;
    (vacuum)eventus_caudae_impellere(r->cauda, &e);
}

interior b32
_rivi_fuga_sola (
    constans SeriesLexema* l)
{
    i32 k;

    si (l->crudum.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < l->crudum.mensura; k++)
    {
        si (l->crudum.datum[k] != (i8)0x1B)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Octetos lexematori tradere (status eius restituitur; reliquiae
 * praefixum seriei pendentis sunt - nullum lexema completur) */
interior vacuum
_octetos_tradere (
    RivusTerminalis* r,
        constans i8* octeti,
                i32  mensura)
{
     constans i8* p = octeti;
    SeriesLexema  l;

    dum (   p < octeti + mensura
         && series_lexema_proximum(r->series, &p, octeti + mensura, &l)
                != SERIES_NIHIL)
    {
    }
}

/* Reliquiae continuationi redduntur si initus novus eam continuare
 * videtur; alioquin abiciuntur. FALSUM = nondum decernitur ('[' solum
 * post ESC: octetus proximus dicet). */
interior b32
_reliquias_reddere (
    RivusTerminalis* r)
{
    b32 congruit  = FALSUM;
     i8 b         = r->buffer[ZEPHYRUM];

    si (r->reliquiae_genus == RELIQUIAE_FUGA)
    {
        si (r->mensura == I && b == '[')
        {
            redde FALSUM;
        }
        congruit = r->mensura >= II && b == '['
            && (r->buffer[I] == '<' || r->buffer[I] == 'M');
    }
    alioquin si (r->reliquiae_genus == RELIQUIAE_SGR)
    {
        congruit = (b >= '0' && b <= '9') || b == ';' || b == 'M'
            || b == 'm';
    }
    si (congruit)
    {
        _octetos_tradere(r, r->reliquiae, r->reliquiae_mensura);
    }
    r->reliquiae_genus    = RELIQUIAE_NULLAE;
    r->reliquiae_mensura  = ZEPHYRUM;
    redde VERUM;
}

interior vacuum
_glutino_addere (
    RivusTerminalis* r,
                 i8  octetus)
{
    si (r->glutinum_mensura < RIVUS_GLUTINUM_CAPACITAS)
    {
        r->glutinum[r->glutinum_mensura] = octetus;
        r->glutinum_mensura++;
    }
    alioquin
    {
        r->glutinum_truncatum = VERUM;
    }
}

/* Glutinum finitum: TEXT GLUTINATA (copiatum in caudam), truncatum
 * notatur in eventu ipso */
interior vacuum
_glutinum_finire (
    RivusTerminalis* r,
                s64  tempus)
{
    si (r->glutinum_mensura == ZEPHYRUM)
    {
        /* glutinum VACUUM est eventus (cauda textum vacuum recusat) */
        Eventus e;

        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus                   = EVENTUS_TEXTUS;
        e.tempus                  = tempus;
        e.datum.textus.origo      = EVENTUS_ORIGO_GLUTINATA;
        e.datum.textus.truncatum  = r->glutinum_truncatum;
        (vacuum)eventus_caudae_impellere(r->cauda, &e);
    }
    alioquin si (   interpres_glutinum(&r->interpres, r->glutinum,
                 r->glutinum_mensura, tempus, r->cauda) > ZEPHYRUM
                 && r->glutinum_truncatum)
    {
        r->cauda->eventus[(r->cauda->finis + EVENTUS_CAUDA_CAPACITAS
            - I)
            % EVENTUS_CAUDA_CAPACITAS].datum.textus.truncatum = VERUM;
    }
    r->glutinum_pendens    = FALSUM;
    r->glutinum_mensura    = ZEPHYRUM;
    r->congruentes         = ZEPHYRUM;
    r->glutinum_truncatum  = FALSUM;
}

/* Corpus glutini ex buffere: VERUM si terminus inventus. Discordia:
 * praefixum congruens corpus est, octetus ut ESC novum iterum temptatur
 * (terminus ESC solum in capite habet). */
interior b32
_glutinum_colligere (
    RivusTerminalis* r,
               s64  tempus)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < r->mensura; k++)
    {
        i8 b = r->buffer[k];

        si (b == (i8)TERMINUS_GLUTINI[r->congruentes])
        {
            r->congruentes++;
            si (r->congruentes == TERMINI_LONGITUDO)
            {
                _consumere(r, k + I);
                _glutinum_finire(r, tempus);
                redde VERUM;
            }
            perge;
        }
        per (j = ZEPHYRUM; j < r->congruentes; j++)
        {
            _glutino_addere(r, (i8)TERMINUS_GLUTINI[j]);
        }
        si (b == (i8)0x1B)
        {
            r->congruentes = I;
        }
        alioquin
        {
            r->congruentes = ZEPHYRUM;
            _glutino_addere(r, b);
        }
    }
    r->mensura = ZEPHYRUM;
    redde FALSUM;
}

/* Cursus imprimibilis: runae INTEGRAE interpreti traduntur; runa
 * dimidia
 * in fine bufferis manet (lexemator in solo - reditus innocuus). Redde
 * octetos consumendos. */
interior i32
_cursum_tradere (
          RivusTerminalis* r,
    constans SeriesLexema* l,
                      s64  tempus)
{
      constans i8* initium  = l->textus.datum;
      constans i8* finis    = initium + l->textus.mensura;
      constans i8* p        = initium;
     SeriesLexema  integrum;

    /* runa dimidia SOLUM si cursus ad finem bufferis pertinet */
    si (finis == r->buffer + r->mensura)
    {
         constans i8* q = finis;
                 i32  k;

        per (k = ZEPHYRUM; k < III && q > initium; k++)
        {
            q--;
            si (!utf8_est_continuatio(*q))
            {
                s32 longitudo = utf8_longitudo_byte(*q);

                si (   longitudo > ZEPHYRUM
                    && (s32)(finis - q) < longitudo)
                {
                    finis = q;   /* runa dimidia: exspectatur */
                }
                frange;
            }
        }
    }
    /* passus finitus (cauda): sectio ad initium runae */
    si (finis - initium > CURSUS_MAXIMUS)
    {
        finis = initium + CURSUS_MAXIMUS;
        dum (finis > initium && utf8_est_continuatio(*finis))
        {
            finis--;
        }
    }
    p = finis;
    si (p > initium)
    {
        integrum                 = *l;
        integrum.textus.mensura  = (i32)(p - initium);
        (vacuum)interpres_lexema(&r->interpres, &integrum, FALSUM,
            tempus,
            r->cauda);
    }
    redde (i32)(p - r->buffer);
}

/* Passus unus: lexema aut canalis crudus. VERUM si progressus. */
interior b32
_passus (
    RivusTerminalis* r,
                s64  tempus)
{
      constans i8* ptr;
     SeriesLexema  l;
      SeriesGenus  g;
              i32  consumpti;

    si (   r->mensura > ZEPHYRUM
        && r->reliquiae_genus != RELIQUIAE_NULLAE
        && !_reliquias_reddere(r))
    {
        redde FALSUM;   /* '[' post ESC: exspecta */
    }
    si (r->glutinum_pendens)
    {
        redde _glutinum_colligere(r, tempus);
    }
    si (r->x10_pendens)
    {
        si (r->mensura < III)
        {
            redde FALSUM;
        }
        r->x10_pendens = FALSUM;
        (vacuum)interpres_x10(&r->interpres,
            (i32)r->buffer[ZEPHYRUM], (i32)r->buffer[I],
            (i32)r->buffer[II], tempus, r->cauda);
        _consumere(r, III);
        redde VERUM;
    }
    si (r->alienum_pendens)
    {
        i32 k;

        per (k = ZEPHYRUM; k < r->mensura; k++)
        {
            si (r->buffer[k] >= 0x40 && r->buffer[k] <= 0x7E)
            {
                _consumere(r, k + I);
                r->alienum_pendens = FALSUM;
                redde VERUM;
            }
        }
        r->mensura = ZEPHYRUM;
        redde FALSUM;
    }
    si (r->mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    ptr        = r->buffer;
    g          = series_lexema_proximum(r->series, &ptr,
        r->buffer + r->mensura, &l);
    consumpti  = (i32)(ptr - r->buffer);

    si (g == SERIES_NIHIL)
    {
        _consumere(r, consumpti);
        redde (b32)(consumpti > ZEPHYRUM);
    }
    si (g == SERIES_IMPRIMERE)
    {
        i32 n = _cursum_tradere(r, &l, tempus);

        _consumere(r, n);
        redde (b32)(n > ZEPHYRUM);
    }
    _consumere(r, consumpti);
    si (   g == SERIES_CSI && l.privatum == ZEPHYRUM
        && l.numerus_intermediorum == ZEPHYRUM
        && l.separatores == ZEPHYRUM)
    {
        /* canales crudi: X10 (CSI M), forma aliena (CSI [), glutinum */
        si (l.numerus_parametrorum == ZEPHYRUM && l.finale == 'M')
        {
            r->x10_pendens = VERUM;
            redde VERUM;
        }
        si (l.numerus_parametrorum == ZEPHYRUM && l.finale == '[')
        {
            r->alienum_pendens = VERUM;
            redde VERUM;
        }
        si (   l.numerus_parametrorum == I && l.finale == '~'
            && l.parametra[ZEPHYRUM]  == CODEX_INITII_GLUTINI)
        {
            r->glutinum_pendens    = VERUM;
            r->glutinum_mensura    = ZEPHYRUM;
            r->congruentes         = ZEPHYRUM;
            r->glutinum_truncatum  = FALSUM;
            redde VERUM;
        }
    }
    (vacuum)interpres_lexema(&r->interpres, &l, FALSUM, tempus,
        r->cauda);
    redde VERUM;
}


/* ==================================================
 * Publica
 * ================================================== */

RivusTerminalis*
rivus_creare (
    Piscina* piscina,
        s32  cellula_latitudo,
        s32  cellula_altitudo)
{
    RivusTerminalis* r;

    r = (RivusTerminalis*)piscina_allocare_ordinatum(piscina,
        magnitudo(RivusTerminalis), VIII);
    si (r == NIHIL)
    {
        redde NIHIL;
    }
    memset(r, ZEPHYRUM, magnitudo(RivusTerminalis));
    r->series   = series_lectorem_creare(piscina);
    r->cauda    = (EventusCauda*)piscina_allocare_ordinatum(piscina,
        magnitudo(EventusCauda), VIII);
    r->glutinum = (i8*)piscina_allocare(piscina,
        (memoriae_index)RIVUS_GLUTINUM_CAPACITAS);
    si (r->series == NIHIL || r->cauda == NIHIL || r->glutinum == NIHIL)
    {
        redde NIHIL;
    }
    series_lectorem_initus_ponere(r->series, VERUM);
    interpres_initiare(&r->interpres, cellula_latitudo,
        cellula_altitudo);
    eventus_caudam_initiare(r->cauda);
    _facultates_impellere(r);     /* primum fluxus (spec Q4) */
    redde r;
}

InterpresTerminalis*
rivus_interpres (
    RivusTerminalis* r)
{
    redde &r->interpres;
}

i32
rivus_spatium (
    constans RivusTerminalis* r)
{
    redde RIVUS_BUFFER - r->mensura;
}

i32
rivus_tradere (
    RivusTerminalis* r,
        constans i8* octeti,
                i32  mensura)
{
    i32 n = (mensura < RIVUS_BUFFER - r->mensura)
        ? mensura : RIVUS_BUFFER - r->mensura;

    memcpy(r->buffer + r->mensura, octeti, (memoriae_index)n);
    r->mensura += n;
    redde n;
}

b32
rivus_eventum (
    RivusTerminalis* r,
                s64  tempus,
            Eventus* eventus)
{
    si (eventus_caudae_extrahere(r->cauda, eventus))
    {
        redde VERUM;
    }
    /* cauda vacua: onera (textus, exempla) vacantur, deinde pigre
     * decoditur - passus unus quoad eventum adest */
    eventus_cauda_lectio_incipit(r->cauda);
    dum (_passus(r, tempus))
    {
        si (eventus_caudae_extrahere(r->cauda, eventus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
rivus_eventum_coalitum (
    RivusTerminalis* r,
                s64  tempus,
            Eventus* eventus)
{
    /* decodificatio solum cauda VACUA: onera tum vacantur, et motus
     * huius lectionis in caudam coalescunt (dimidium caudae: passus
     * unus <= II * CURSUS_MAXIMUS eventa) */
    si (r->cauda->numerus == ZEPHYRUM)
    {
        eventus_cauda_lectio_incipit(r->cauda);
        dum (   r->cauda->numerus < EVENTUS_CAUDA_CAPACITAS / II
             && _passus(r, tempus))
        {
        }
    }
    redde eventus_caudae_extrahere(r->cauda, eventus);
}

i32
rivus_modos_intrare (
    RivusTerminalis* r,
                i32  modi,
                 i8* buffer,
                i32  capacitas)
{
    i32 n;

    si (modi & RIVUS_MODUS_SUPER)
    {
        modi |= RIVUS_MODUS_MUS;
    }
    modi &= MODI_OMNES;
    si (   r->modi_intrati != ZEPHYRUM || modi == ZEPHYRUM
        || capacitas < RIVUS_MODI_MAXIMUM)
    {
        redde ZEPHYRUM;
    }
    n                = _modos_scribere(modi, FALSUM, buffer);
    r->modi_intrati  = modi;
    r->interpres.kitty_vexilla =
        (modi & RIVUS_MODUS_KITTY) ? KITTY_IMPULSA : ZEPHYRUM;
    r->interpres.facultates.super = (b32)((modi & RIVUS_MODUS_SUPER)
        != ZEPHYRUM);
    _facultates_impellere(r);
    redde n;
}

i32
rivus_modos_exire (
    RivusTerminalis* r,
                 i8* buffer,
                i32  capacitas)
{
    i32 n;

    si (r->modi_intrati == ZEPHYRUM || capacitas < RIVUS_MODI_MAXIMUM)
    {
        redde ZEPHYRUM;
    }
    n                           = _modos_scribere(r->modi_intrati,
        VERUM,
        buffer);
    r->modi_intrati             = ZEPHYRUM;
    r->interpres.kitty_vexilla  = ZEPHYRUM;
    redde n;
}

s32
rivus_mora_ms (
    constans RivusTerminalis* r)
{
    si (r->glutinum_pendens)
    {
        redde RIVUS_MORA_GLUTINI_MS;
    }
    si (   series_lector_pendet(r->series) || r->x10_pendens
        || r->alienum_pendens || r->interpres.alterum_pendens
        || r->mensura > ZEPHYRUM)
    {
        redde RIVUS_MORA_FUGAE_MS;
    }
    redde ZEPHYRUM;
}

vacuum
rivus_moram (
    RivusTerminalis* r,
                s64  tempus)
{
    SeriesLexema l;

    si (r->glutinum_pendens)
    {
        /* silentium sine termino: praefixum pendens corpus fit */
        i32 j;

        per (j = ZEPHYRUM; j < r->congruentes; j++)
        {
            _glutino_addere(r, (i8)TERMINUS_GLUTINI[j]);
        }
        r->glutinum_truncatum = VERUM;
        _glutinum_finire(r, tempus);
        redde;
    }
    si (r->x10_pendens)
    {
        r->x10_pendens  = FALSUM;   /* X10 dimidium abicitur (H7) */
        r->mensura      = ZEPHYRUM;
        redde;
    }
    si (r->alienum_pendens)
    {
        r->alienum_pendens = FALSUM;
        redde;
    }
    si (series_lector_pendet(r->series))
    {
        si (!series_lectorem_evacuare(r->series, &l))
        {
            redde;   /* solum terminator chordae */
        }
        si (   l.crudum.mensura   >= III && l.crudum.datum[I] == '['
            && l.crudum.datum[II] == '<')
        {
            /* mus SGR dimidia: servatur pro continuatione (H8) */
            memcpy(r->reliquiae, l.crudum.datum,
                (memoriae_index)l.crudum.mensura);
            r->reliquiae_mensura  = l.crudum.mensura;
            r->reliquiae_genus    = RELIQUIAE_SGR;
            redde;
        }
        (vacuum)interpres_lexema(&r->interpres, &l, VERUM, tempus,
            r->cauda);
        si (_rivi_fuga_sola(&l) && l.crudum.mensura == I)
        {
            /* mus forte sequetur ('[<..', '[M..'): H8 */
            r->reliquiae[ZEPHYRUM]  = (i8)0x1B;
            r->reliquiae_mensura    = I;
            r->reliquiae_genus      = RELIQUIAE_FUGA;
        }
        redde;
    }
    si (r->reliquiae_genus != RELIQUIAE_NULLAE && r->mensura > ZEPHYRUM)
    {
        /* '[' post ESC sine continuatione: reliquiae abiciuntur, '['
         * ut runa legitur */
        r->reliquiae_genus    = RELIQUIAE_NULLAE;
        r->reliquiae_mensura  = ZEPHYRUM;
        redde;
    }
    si (r->interpres.alterum_pendens)
    {
        /* 'ESC' + runa dimidia: Effugium (FUGA post moram ficta - eadem
         * via ac ESC solus); runa dimidia mox abicitur */
        i8 fuga[I];

        fuga[ZEPHYRUM] = (i8)0x1B;
        memset(&l, ZEPHYRUM, magnitudo(SeriesLexema));
        l.genus           = SERIES_FUGA;
        l.crudum.datum    = fuga;
        l.crudum.mensura  = I;
        (vacuum)interpres_lexema(&r->interpres, &l, VERUM, tempus,
            r->cauda);
        redde;
    }
    si (r->mensura > ZEPHYRUM)
    {
        _consumere(r, I);   /* runa dimidia (aut invalida) abicitur */
    }
}

i32
rivus_pendentes (
    constans RivusTerminalis* r)
{
    redde r->mensura;
}
