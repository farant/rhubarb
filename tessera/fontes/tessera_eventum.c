/* tessera_eventum.c - Implementatio lectoris (Phase B)
 *
 * Parsator purus a fronte bufferis consumens; fructus parsationis:
 *   COMPLETUM    - eventum paratum, octeti consumpti
 *   INCOMPLETUM  - series dimidia in fine bufferis (plura expecta)
 *   VACUUM       - buffer vacuus
 *   PRAETERITUM  - octeti consumpti sine eventu (CSI ignota)
 */

#include "tessera_eventum.h"
#include "utf8.h"
#include <string.h>

/* Limes parametri CSI: accumulatio ultra hunc cessat (s32 numquam
 * exundat); valor maior = ingens = invalidus */
#define PARAMETRUM_MAXIMUM (X * M)

/* Glutinum: CSI 200 ~ incipit, CSI 201 ~ finit */
#define CODEX_INITII_GLUTINI CC
#define TERMINUS_GLUTINI "\033[201~"
#define TERMINI_LONGITUDO ((i32)(magnitudo(TERMINUS_GLUTINI) - I))

nomen enumeratio {
    PARS_COMPLETUM = 0,
    PARS_INCOMPLETUM,
    PARS_VACUUM,
    PARS_PRAETERITUM,
    PARS_GLUTINUM      /* CSI 200 ~ consumptum: collector sequitur */
} ParsFructus;

interior vacuum
_eventum_vacare (
    TesseraEventum* ev)
{
    ev->genus               = TESSERA_EVENTUM_NIHIL;
    ev->runa                = ZEPHYRUM;
    ev->clavis              = TESSERA_CLAVIS_NULLA;
    ev->modificatores       = ZEPHYRUM;
    ev->numerus             = ZEPHYRUM;
    ev->mus_genus           = TESSERA_MUS_PRESSUS;
    ev->mus_x               = ZEPHYRUM;
    ev->mus_y               = ZEPHYRUM;
    ev->mus_pulsus          = ZEPHYRUM;
    ev->latitudo            = ZEPHYRUM;
    ev->altitudo            = ZEPHYRUM;
    ev->glutinum.mensura    = ZEPHYRUM;
    ev->glutinum.datum      = NIHIL;
    ev->glutinum_truncatum  = FALSUM;
}

interior vacuum
_consumere (
    TesseraLector* lector,
              i32  numerus)
{
    si (numerus >= lector->mensura)
    {
        lector->mensura = ZEPHYRUM;
        redde;
    }
    memmove(lector->buffer, lector->buffer + numerus,
        (memoriae_index)(lector->mensura - numerus));
    lector->mensura -= numerus;
}

interior vacuum
_clavem_ponere (
    TesseraEventum* ev,
     TesseraClavis  clavis,
               i32  modificatores)
{
    ev->genus          = TESSERA_EVENTUM_CLAVIS;
    ev->clavis         = clavis;
    ev->modificatores  = modificatores;
}

interior vacuum
_runam_ponere (
    TesseraEventum* ev,
               s32  runa,
               i32  modificatores)
{
    ev->genus          = TESSERA_EVENTUM_CLAVIS;
    ev->runa           = runa;
    ev->modificatores  = modificatores;
}

/* Modificatores CSI (parametrum m): m-1 = bits maiuscula/alterum/
 * imperium */
interior i32
_modificatores_csi (
    s32 m)
{
    i32 fructus = ZEPHYRUM;
    s32 bits;

    si (m <= I || m > PARAMETRUM_MAXIMUM)
    {
        redde ZEPHYRUM;   /* nullus aut ingens (invalidus) */
    }
    bits = m - I;
    si (bits & I)
    {
        fructus |= TESSERA_MODIFICATOR_MAIUSCULA;
    }
    si (bits & II)
    {
        fructus |= TESSERA_MODIFICATOR_ALTERUM;
    }
    si (bits & IV)
    {
        fructus |= TESSERA_MODIFICATOR_IMPERIUM;
    }
    redde fructus;
}

/* ~-codices CSI */
interior b32
_clavem_tildae (
               s32  codex,
    TesseraEventum* ev,
               i32  modificatores)
{
    commutatio (codex)
    {
        casus II:    _clavem_ponere(ev, TESSERA_CLAVIS_INSERTIO,
                         modificatores); redde VERUM;
        casus III:   _clavem_ponere(ev, TESSERA_CLAVIS_DELETIO,
                         modificatores); redde VERUM;
        casus V:     _clavem_ponere(ev, TESSERA_CLAVIS_PAGINA_SURSUM,
                         modificatores); redde VERUM;
        casus VI:    _clavem_ponere(ev, TESSERA_CLAVIS_PAGINA_DEORSUM,
                         modificatores); redde VERUM;
        casus I:     _clavem_ponere(ev, TESSERA_CLAVIS_DOMUS,
                         modificatores); redde VERUM;
        casus IV:    _clavem_ponere(ev, TESSERA_CLAVIS_FINIS,
                         modificatores); redde VERUM;
        ordinarius:  frange;
    }
    si (codex >= XI && codex <= XV)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_FUNCTIO, modificatores);
        ev->numerus = (i32)(codex - X);          /* 11-15 = F1-F5 */
        redde VERUM;
    }
    si (codex >= XVII && codex <= XXI)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_FUNCTIO, modificatores);
        ev->numerus = (i32)(codex - XI);         /* 17-21 = F6-F10 */
        redde VERUM;
    }
    si (codex == XXIII || codex == XXIV)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_FUNCTIO, modificatores);
        ev->numerus = (i32)(codex - XII);        /* 23/24 = F11/F12 */
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
_clavem_finalem (
         character  finalis,
    TesseraEventum* ev,
               i32  modificatores)
{
    commutatio (finalis)
    {
        casus 'A': _clavem_ponere(ev, TESSERA_CLAVIS_SURSUM,
                       modificatores); redde VERUM;
        casus 'B': _clavem_ponere(ev, TESSERA_CLAVIS_DEORSUM,
                       modificatores); redde VERUM;
        casus 'C': _clavem_ponere(ev, TESSERA_CLAVIS_DEXTRA,
                       modificatores); redde VERUM;
        casus 'D': _clavem_ponere(ev, TESSERA_CLAVIS_SINISTRA,
                       modificatores); redde VERUM;
        casus 'H': _clavem_ponere(ev, TESSERA_CLAVIS_DOMUS,
                       modificatores); redde VERUM;
        casus 'F': _clavem_ponere(ev, TESSERA_CLAVIS_FINIS,
                       modificatores); redde VERUM;
        casus 'Z': _clavem_ponere(ev, TESSERA_CLAVIS_TABULA,
                       modificatores | TESSERA_MODIFICATOR_MAIUSCULA);
                   redde VERUM;
        ordinarius: redde FALSUM;
    }
}

/* Mus (SGR aut X10) classificare ex codice bottonis crudo: bits 0-1
 * botton, 4 maiuscula, 8 alterum, 16 imperium, 32 motus, 64 rota.
 * solutio = SGR 'm', aut X10 botton III (solutio sine bottone noto).
 * Motus cum bottone 0-2 = TRACTUS (tessera ?1002 petit: terminal motum
 * SOLUM botton tento refert; finalis M/m in motu neglegitur, sine
 * statu - bits bottonis creduntur). COMPLETUM = eventum positum;
 * PRAETERITUM = tacite consumptum (motus sine bottone 35 = ?1003, non
 * petitus; motus + rota 96/97; rota soluta). Campi eventus SOLUM in
 * COMPLETUM scribuntur (nihil sordidum relinquitur eventui proximo). */
interior ParsFructus
_murem_classificare (
    TesseraEventum* ev,
               s32  pulsus,
               s32  x,
               s32  y,
               b32  solutio)
{
    TesseraMusGenus genus          = TESSERA_MUS_PRESSUS;
                i32 botton         = (i32)(pulsus & III);
                i32 modificatores  = ZEPHYRUM;

    si (pulsus & XXXII)
    {
        si ((pulsus & LXIV) || botton == III)
        {
            redde PARS_PRAETERITUM;   /* motus + rota, aut sine bottone */
        }
        genus = TESSERA_MUS_TRACTUS;
    }
    alioquin si (pulsus & LXIV)
    {
        si (solutio)
        {
            redde PARS_PRAETERITUM;   /* rota solutionem non habet */
        }
        commutatio (botton)
        {
            casus ZEPHYRUM: genus =
                                TESSERA_MUS_ROTA_SURSUM;       frange;
            casus I:        genus =
                                TESSERA_MUS_ROTA_DEORSUM;      frange;
            casus II:       genus =
                                TESSERA_MUS_ROTA_SINISTRORSUM; frange;
            ordinarius:     genus =
                                TESSERA_MUS_ROTA_DEXTRORSUM;   frange;
        }
        botton = ZEPHYRUM;
    }
    alioquin
    {
        genus = solutio ? TESSERA_MUS_SOLUTUS : TESSERA_MUS_PRESSUS;
    }
    si (pulsus & IV)
    {
        modificatores |= TESSERA_MODIFICATOR_MAIUSCULA;
    }
    si (pulsus & VIII)
    {
        modificatores |= TESSERA_MODIFICATOR_ALTERUM;
    }
    si (pulsus & XVI)
    {
        modificatores |= TESSERA_MODIFICATOR_IMPERIUM;
    }
    ev->genus          = TESSERA_EVENTUM_MUS;
    ev->mus_genus      = genus;
    ev->mus_x          = x;
    ev->mus_y          = y;
    ev->mus_pulsus     = botton;
    ev->modificatores  = modificatores;
    redde PARS_COMPLETUM;
}

/* Reliquiae post moram (H8) */
#define RELIQUIAE_NULLAE ZEPHYRUM
#define RELIQUIAE_FUGA   I      /* ESC solus, iam FUGA redditus */
#define RELIQUIAE_SGR    II     /* CSI < dimidia */

/* CSI completa (lexema): mus SGR, glutinum, claves. Forma ignota nobis
 * (intermedia, ':' separatores, privatum praeter '<') tacite consumitur
 * (strepitus regiminis clavem phantasma fieri non debet). Mus X10
 * (CSI M sine parametris) et forma aliena (CSI [) octetos CRUDOS
 * sequentes poscunt: status ponitur, _parsare eos legit. */
interior ParsFructus
_csi_tractare (
            TesseraLector* lector,
    constans SeriesLexema* l,
           TesseraEventum* ev)
{
          i32 n        = l->numerus_parametrorum;
    character finalis  = (character)l->finale;
          i32 modificatores;

    si (   l->numerus_intermediorum > ZEPHYRUM
        || l->separatores != ZEPHYRUM
        || (l->privatum != ZEPHYRUM && l->privatum != '<'))
    {
        redde PARS_PRAETERITUM;
    }
    si (l->privatum == ZEPHYRUM && n == ZEPHYRUM)
    {
        /* Mus X10 (ESC [ M cb cx cy): onus TRES octeti CRUDI (+32,
         * +33, +33) - terminalia quae 1006 (SGR) ignorant eum mittunt;
         * sine hoc onus claves phantasma fieret (H3; 0x7F!) */
        si (finalis == 'M')
        {
            lector->x10_pendens = VERUM;
            redde PARS_PRAETERITUM;
        }
        /* Linux console 'CSI [ A', putty 'CSI [ 5 ~': cauda usque ad
         * finalem tacite consumitur (ALIENA) */
        si (finalis == '[')
        {
            lector->alienum_pendens = VERUM;
            redde PARS_PRAETERITUM;
        }
    }
    si (   l->privatum == '<' && (finalis == 'M' || finalis == 'm')
        && n           >= III)
    {
        /* coordinatae 1-basatae -> 0 */
        redde _murem_classificare(ev, l->parametra[ZEPHYRUM],
            l->parametra[I] - I, l->parametra[II] - I, finalis == 'm');
    }
    modificatores = (n >= II) ? _modificatores_csi(l->parametra[I])
                              : ZEPHYRUM;
    si (l->praefixum)
    {
        /* ESC ESC [ A = alterum + sursum (Franus 2026-09-28) */
        modificatores |= TESSERA_MODIFICATOR_ALTERUM;
    }
    si (   finalis                == '~' && n == I
        && l->parametra[ZEPHYRUM] == CODEX_INITII_GLUTINI)
    {
        redde PARS_GLUTINUM;
    }
    si (finalis == '~' && n >= I)
    {
        si (_clavem_tildae(l->parametra[ZEPHYRUM], ev, modificatores))
        {
            redde PARS_COMPLETUM;
        }
        redde PARS_PRAETERITUM;
    }
    si (_clavem_finalem(finalis, ev, modificatores))
    {
        redde PARS_COMPLETUM;
    }
    redde PARS_PRAETERITUM;
}

/* SS3 (ESC O [parametrum] finalis): frecce + F1-F4 (modus
 * applicationis); 'ESC O 2 P' = maiuscula + F1 (xterm vetus).
 * Minusculae (rxvt) et ceterae tacite. */
interior ParsFructus
_ss_tractare (
    constans SeriesLexema* l,
           TesseraEventum* ev)
{
    character finalis = (character)l->finale;
          i32 modificatores;

    si (l->introductor != 'O' || l->numerus_intermediorum > ZEPHYRUM)
    {
        redde PARS_PRAETERITUM;
    }
    modificatores = (l->numerus_parametrorum >= I)
        ? _modificatores_csi(l->parametra[ZEPHYRUM]) : ZEPHYRUM;
    si (l->praefixum)
    {
        modificatores |= TESSERA_MODIFICATOR_ALTERUM;
    }
    si (_clavem_finalem(finalis, ev, modificatores))
    {
        redde PARS_COMPLETUM;
    }
    si (finalis >= 'P' && finalis <= 'S')
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_FUNCTIO, modificatores);
        ev->numerus = (i32)(finalis - 'P') + I;  /* P-S = F1-F4 */
        redde PARS_COMPLETUM;
    }
    redde PARS_PRAETERITUM;
}

/* Octetus regiminis solus (non ESC) */
interior vacuum
_regimen_parsare (
                i8  b,
    TesseraEventum* ev,
               i32  modificatores)
{
    si (b == 0x0D || b == 0x0A)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_REDITUS, modificatores);
    }
    alioquin si (b == 0x09)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_TABULA, modificatores);
    }
    alioquin si (b == 0x08 || b == 0x7F)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_RETRORSUM, modificatores);
    }
    alioquin si (b >= I && b <= XXVI)
    {
        _runam_ponere(ev, (s32)('a' + b - I),
            modificatores | TESSERA_MODIFICATOR_IMPERIUM);
    }
    alioquin si (b == ZEPHYRUM)
    {
        _runam_ponere(ev, (s32)' ',
            modificatores | TESSERA_MODIFICATOR_IMPERIUM);
    }
    alioquin
    {
        /* 0x1C-0x1F: imperium + symbolum */
        _runam_ponere(ev, (s32)(b | 0x40),
            modificatores | TESSERA_MODIFICATOR_IMPERIUM);
    }
}

/* FUGA lexematoris ex ESC solis constat (ESC, ESC ESC)? */
interior b32
_sola_fuga (
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

/* Octetos lexematori tradere (status eius restituitur; nullum lexema
 * completur - reliquiae praefixum seriei pendentis sunt) */
interior vacuum
_octetos_tradere (
    TesseraLector* lector,
      constans i8* octeti,
              i32  mensura)
{
     constans i8* p = octeti;
    SeriesLexema  l;

    dum (   p < octeti + mensura
         && series_lexema_proximum(lector->series, &p, octeti + mensura,
                &l) != SERIES_NIHIL)
    {
    }
}

/* Reliquiae post moram (H8) continuationi redduntur si initus novus
 * eam continuare videtur; alioquin abiciuntur. FALSUM = nondum
 * decernitur ('[' solum post ESC: octetus proximus intra moram
 * dicet). */
interior b32
_reliquias_reddere (
    TesseraLector* lector)
{
    b32 congruit  = FALSUM;
     i8 b         = lector->buffer[ZEPHYRUM];

    si (lector->reliquiae_genus == RELIQUIAE_FUGA)
    {
        si (lector->mensura == I && b == '[')
        {
            redde FALSUM;
        }
        congruit = lector->mensura >= II && b == '['
            && (lector->buffer[I] == '<' || lector->buffer[I] == 'M');
    }
    alioquin si (lector->reliquiae_genus == RELIQUIAE_SGR)
    {
        congruit = (b >= '0' && b <= '9') || b == ';' || b == 'M'
            || b == 'm';
    }
    si (congruit)
    {
        _octetos_tradere(lector, lector->reliquiae,
            lector->reliquiae_mensura);
    }
    lector->reliquiae_genus    = RELIQUIAE_NULLAE;
    lector->reliquiae_mensura  = ZEPHYRUM;
    redde VERUM;
}

/* Mus X10: tres octeti crudi post CSI M */
interior ParsFructus
_x10_parsare (
     TesseraLector* lector,
    TesseraEventum* ev)
{
            s32 cb;
    ParsFructus fructus;

    si (lector->mensura < III)
    {
        redde PARS_INCOMPLETUM;
    }
    lector->x10_pendens  = FALSUM;
    cb                   = (s32)lector->buffer[ZEPHYRUM] - XXXII;
    si (   cb < ZEPHYRUM || lector->buffer[I] < XXXIII
        || lector->buffer[II] < XXXIII)
    {
        _consumere(lector, III);
        redde PARS_PRAETERITUM;   /* onus malum: consumptum */
    }
    fructus = _murem_classificare(ev, cb,
        (s32)lector->buffer[I] - XXXIII,
        (s32)lector->buffer[II] - XXXIII,
        (cb & III) == III && !(cb & LXIV) && !(cb & XXXII));
    _consumere(lector, III);
    redde fructus;
}

/* Forma aliena: cauda usque ad octetum finalem (0x40-0x7E) */
interior ParsFructus
_alienum_consumere (
    TesseraLector* lector)
{
    i32 k;

    per (k = ZEPHYRUM; k < lector->mensura; k++)
    {
        si (lector->buffer[k] >= 0x40 && lector->buffer[k] <= 0x7E)
        {
            _consumere(lector, k + I);
            lector->alienum_pendens = FALSUM;
            redde PARS_PRAETERITUM;
        }
    }
    _consumere(lector, lector->mensura);
    redde PARS_INCOMPLETUM;
}

/* Runa prima cursus imprimibilis; ceterae in buffere manent (lexemator
 * in solo est - reditus intra cursum innocuus). */
interior ParsFructus
_runam_parsare (
            TesseraLector* lector,
    constans SeriesLexema* l,
           TesseraEventum* ev)
{
    constans i8* initium          = l->textus.datum;
    constans i8* finis            = initium + l->textus.mensura;
    constans i8* cursor           = initium;
            s32  longitudo_runae  = utf8_longitudo_byte(*initium);
            s32  runa;
            i32  ante = (i32)(initium - lector->buffer);

    si (   longitudo_runae > ZEPHYRUM
        && (i32)longitudo_runae > l->textus.mensura
        && finis == lector->buffer + lector->mensura)
    {
        _consumere(lector, ante);
        redde PARS_INCOMPLETUM;  /* runa dimidia in fine */
    }
    runa = utf8_decodere(&cursor, finis);
    si (runa < ZEPHYRUM)
    {
        _consumere(lector, ante);
        si (lector->alterum_pendens)
        {
            /* invalidum post ESC: FUGA, octetus mox abicitur */
            lector->alterum_pendens = FALSUM;
            _clavem_ponere(ev, TESSERA_CLAVIS_FUGA, ZEPHYRUM);
            redde PARS_COMPLETUM;
        }
        _consumere(lector, I);   /* octetus invalidus abicitur */
        redde PARS_PRAETERITUM;
    }
    _runam_ponere(ev, runa,
        lector->alterum_pendens ? TESSERA_MODIFICATOR_ALTERUM
                                : ZEPHYRUM);
    lector->alterum_pendens = FALSUM;
    _consumere(lector, (i32)(cursor - lector->buffer));
    redde PARS_COMPLETUM;
}

/* Unum eventum a fronte bufferis parsare temptare: lexema unum per
 * series_terminalis, deinde sensus eius. */
interior ParsFructus
_parsare (
     TesseraLector* lector,
    TesseraEventum* ev)
{
      constans i8* ptr;
     SeriesLexema  l;
      SeriesGenus  g;
              i32  consumpti;

    si (   lector->mensura > ZEPHYRUM
        && lector->reliquiae_genus != RELIQUIAE_NULLAE
        && !_reliquias_reddere(lector))
    {
        redde PARS_INCOMPLETUM;   /* '[' post ESC: exspecta */
    }
    si (lector->x10_pendens)
    {
        redde _x10_parsare(lector, ev);
    }
    si (lector->alienum_pendens)
    {
        redde _alienum_consumere(lector);
    }
    si (lector->mensura == ZEPHYRUM)
    {
        redde (series_lector_pendet(lector->series)
               || lector->alterum_pendens)
            ? PARS_INCOMPLETUM : PARS_VACUUM;
    }
    ptr        = lector->buffer;
    g          = series_lexema_proximum(lector->series, &ptr,
        lector->buffer + lector->mensura, &l);
    consumpti  = (i32)(ptr - lector->buffer);

    commutatio (g)
    {
        casus SERIES_IMPRIMERE:
            redde _runam_parsare(lector, &l, ev);

        casus SERIES_EXSEQUI:
            _consumere(lector, consumpti);
            _regimen_parsare(l.finale, ev, lector->alterum_pendens
                ? TESSERA_MODIFICATOR_ALTERUM : ZEPHYRUM);
            lector->alterum_pendens = FALSUM;
            redde PARS_COMPLETUM;

        casus SERIES_FUGA:
            /* abrupta: ESC solus = alterum clavis proximae
             * (alt+reditus,
             * alt+e acutum); series dimidia tacite abicitur */
            _consumere(lector, consumpti);
            si (_sola_fuga(&l))
            {
                lector->alterum_pendens = VERUM;
            }
            redde PARS_PRAETERITUM;

        casus SERIES_ESC:
            /* ALTERUM + clavis (modus initus: ESC P a, ESC N) */
            _consumere(lector, consumpti);
            si (   l.numerus_intermediorum > ZEPHYRUM
                || l.finale < 0x20 || l.finale > 0x7E)
            {
                redde PARS_PRAETERITUM;
            }
            _runam_ponere(ev, (s32)l.finale,
                TESSERA_MODIFICATOR_ALTERUM);
            redde PARS_COMPLETUM;

        casus SERIES_CSI:
            _consumere(lector, consumpti);
            redde _csi_tractare(lector, &l, ev);

        casus SERIES_SS:
            _consumere(lector, consumpti);
            redde _ss_tractare(&l, ev);

        casus SERIES_NIHIL:
            _consumere(lector, consumpti);
            redde series_lector_pendet(lector->series)
                ? PARS_INCOMPLETUM : PARS_PRAETERITUM;

        ordinarius:
            /* OSC, DCS, APC: responsa terminalis tacite (H2) */
            _consumere(lector, consumpti);
            redde PARS_PRAETERITUM;
    }
}

/* Mora exacta cum serie aut runa pendente: quid reddendum. */
interior TesseraEventumGenus
_moram_tractare (
     TesseraLector* lector,
    TesseraEventum* ev)
{
    SeriesLexema l;

    si (lector->x10_pendens)
    {
        /* X10 dimidium: onus abicitur (H7) */
        lector->x10_pendens  = FALSUM;
        lector->mensura      = ZEPHYRUM;
        redde TESSERA_EVENTUM_NIHIL;
    }
    si (lector->alienum_pendens)
    {
        lector->alienum_pendens = FALSUM;
        redde TESSERA_EVENTUM_NIHIL;
    }
    si (series_lector_pendet(lector->series))
    {
        si (!series_lectorem_evacuare(lector->series, &l))
        {
            /* solum terminator chordae */
            redde TESSERA_EVENTUM_NIHIL;
        }
        si (_sola_fuga(&l))
        {
            _clavem_ponere(ev, TESSERA_CLAVIS_FUGA, ZEPHYRUM);
            /* ESC ESC: ambae moram transierunt - altera STATIM
             * proxima exspectatione (non iterum morata) */
            lector->fuga_reddenda = (b32)(l.crudum.mensura >= II);
            si (!lector->fuga_reddenda)
            {
                /* mus forte sequetur ('[<..', '[M..'): H8 */
                lector->reliquiae[ZEPHYRUM]  = (i8)0x1B;
                lector->reliquiae_mensura    = I;
                lector->reliquiae_genus      = RELIQUIAE_FUGA;
            }
            redde ev->genus;
        }
        si (l.crudum.mensura == II)
        {
            /* 'ESC [' / 'ESC O' / 'ESC P' solum = alterum + octetus */
            _runam_ponere(ev, (s32)l.crudum.datum[I],
                TESSERA_MODIFICATOR_ALTERUM);
            redde ev->genus;
        }
        si (   l.crudum.mensura   >= III && l.crudum.datum[I] == '['
            && l.crudum.datum[II] == '<')
        {
            /* mus SGR dimidia: servatur pro continuatione (H8) */
            i32 m = (l.crudum.mensura < TESSERA_RELIQUIAE_CAPACITAS)
                ? l.crudum.mensura : TESSERA_RELIQUIAE_CAPACITAS;

            memcpy(lector->reliquiae, l.crudum.datum,
                (memoriae_index)m);
            lector->reliquiae_mensura  = m;
            lector->reliquiae_genus    = RELIQUIAE_SGR;
        }
        /* series dimidia abicitur (H7) */
        redde TESSERA_EVENTUM_NIHIL;
    }
    si (lector->reliquiae_genus != RELIQUIAE_NULLAE)
    {
        /* '[' post ESC sine continuatione: reliquiae abiciuntur, '['
         * ut runa legitur */
        lector->reliquiae_genus    = RELIQUIAE_NULLAE;
        lector->reliquiae_mensura  = ZEPHYRUM;
        redde TESSERA_EVENTUM_NIHIL;
    }
    si (lector->alterum_pendens)
    {
        /* 'ESC' + runa dimidia: FUGA; runa dimidia mox abicitur */
        lector->alterum_pendens = FALSUM;
        _clavem_ponere(ev, TESSERA_CLAVIS_FUGA, ZEPHYRUM);
        redde ev->genus;
    }
    si (lector->mensura > ZEPHYRUM)
    {
        _consumere(lector, I);   /* runa dimidia abicitur */
    }
    redde TESSERA_EVENTUM_NIHIL;
}

/* Octetum corpori glutini addere; ultra capacitatem abicitur et
 * truncatum notatur (hauritur tamen usque ad terminum) */
interior vacuum
_glutino_addere (
     TesseraLector* lector,
    TesseraEventum* ev,
                i8  octetus)
{
    si (ev->glutinum.mensura < (i32)TESSERA_GLUTINUM_CAPACITAS)
    {
        lector->glutinum[ev->glutinum.mensura] = octetus;
        ev->glutinum.mensura++;
    }
    alioquin
    {
        ev->glutinum_truncatum = VERUM;
    }
}

/* Glutinum colligere post CSI 200 ~ usque ad CSI 201 ~: SEMPER eventum
 * unum GLUTINUM ponit. Buffer lectoris octetum per octetum hauritur;
 * congruentes = octeti termini iam congruentes (trans lectiones
 * servatur). Discordia: praefixum congruens corpus est, octetus ut ESC
 * novum iterum temptatur (terminus ESC solum in capite habet). Post
 * terminum octeti in buffere manent (parsatio ordinaria). Lectiones
 * intra glutinum moram TESSERA_MORA_GLUTINI_MS habent, non vocantis;
 * lectio vacua = silentium (D4) aut terminal abiit: truncatum, et
 * praefixum pendens corpus fit. */
interior vacuum
_glutinum_colligere (
     TesseraLector* lector,
    TesseraEventum* ev)
{
    i32 congruentes = ZEPHYRUM;
    i32 k;
    i32 j;
    s32 n;

    ev->genus               = TESSERA_EVENTUM_GLUTINUM;
    ev->glutinum.datum      = lector->glutinum;
    ev->glutinum.mensura    = ZEPHYRUM;
    ev->glutinum_truncatum  = FALSUM;

    dum (VERUM)
    {
        per (k = ZEPHYRUM; k < lector->mensura; k++)
        {
            i8 b = lector->buffer[k];

            si (b == (i8)TERMINUS_GLUTINI[congruentes])
            {
                congruentes++;
                si (congruentes == TERMINI_LONGITUDO)
                {
                    _consumere(lector, k + I);
                    redde;
                }
                perge;
            }
            per (j = ZEPHYRUM; j < congruentes; j++)
            {
                _glutino_addere(lector, ev, (i8)TERMINUS_GLUTINI[j]);
            }
            si (b == 0x1B)
            {
                congruentes = I;
            }
            alioquin
            {
                congruentes = ZEPHYRUM;
                _glutino_addere(lector, ev, b);
            }
        }
        lector->mensura = ZEPHYRUM;

        n = lector->pons->legere(lector->pons->datum, lector->buffer,
            (i32)TESSERA_LECTOR_BUFFER, TESSERA_MORA_GLUTINI_MS);
        si (n <= ZEPHYRUM)
        {
            per (j = ZEPHYRUM; j < congruentes; j++)
            {
                _glutino_addere(lector, ev, (i8)TERMINUS_GLUTINI[j]);
            }
            ev->glutinum_truncatum = VERUM;
            redde;
        }
        lector->mensura = (i32)n;
    }
}

/* Parsare usque ad fructum non-PRAETERITUM (strepitus consumptus
 * iteratur): COMPLETUM, VACUUM aut INCOMPLETUM. Initium glutini
 * collectorem statim currit, qui eventum semper ponit: COMPLETUM. */
interior ParsFructus
_parsare_plene (
     TesseraLector* lector,
    TesseraEventum* ev)
{
    ParsFructus fructus;

    dum (VERUM)
    {
        fructus = _parsare(lector, ev);
        si (fructus == PARS_GLUTINUM)
        {
            _glutinum_colligere(lector, ev);
            redde PARS_COMPLETUM;
        }
        si (fructus != PARS_PRAETERITUM)
        {
            redde fructus;
        }
    }
}

TesseraLector*
tessera_lector_creare (
        Piscina* piscina,
    TesseraPons* pons)
{
    TesseraLector* lector;

    si (piscina == NIHIL || pons == NIHIL)
    {
        redde NIHIL;
    }
    lector = (TesseraLector*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(TesseraLector), IV);
    si (lector == NIHIL)
    {
        redde NIHIL;
    }
    lector->glutinum = (i8*)piscina_allocare(piscina,
        (memoriae_index)TESSERA_GLUTINUM_CAPACITAS);
    si (lector->glutinum == NIHIL)
    {
        redde NIHIL;
    }
    lector->series = series_lectorem_creare(piscina);
    si (lector->series == NIHIL)
    {
        redde NIHIL;
    }
    series_lectorem_initus_ponere(lector->series, VERUM);
    lector->pons               = pons;
    lector->mensura            = ZEPHYRUM;
    lector->alterum_pendens    = FALSUM;
    lector->x10_pendens        = FALSUM;
    lector->alienum_pendens    = FALSUM;
    lector->reliquiae_mensura  = ZEPHYRUM;
    lector->reliquiae_genus    = RELIQUIAE_NULLAE;
    lector->fuga_reddenda      = FALSUM;
    si (!pons->amplitudo(pons->datum, &lector->latitudo_nota,
            &lector->altitudo_nota))
    {
        lector->latitudo_nota = ZEPHYRUM;
        lector->altitudo_nota = ZEPHYRUM;
    }
    redde lector;
}

TesseraEventumGenus
tessera_eventum_expectare (
     TesseraLector* lector,
    TesseraEventum* eventum,
               s32  mora_ms)
{
    ParsFructus fructus;

    si (lector == NIHIL || eventum == NIHIL)
    {
        redde TESSERA_EVENTUM_NIHIL;
    }
    _eventum_vacare(eventum);

    /* Resumptio? (roga-et-purga; NIHIL licet) */
    si (   lector->pons->resumptum != NIHIL
        && lector->pons->resumptum(lector->pons->datum))
    {
        eventum->genus = TESSERA_EVENTUM_RESUMPTUM;
        redde eventum->genus;
    }

    /* Amplitudo mutata? (interrogatio - SIGWINCH select solum
     * interrumpit) */
    {
        i32 lat;
        i32 alt;

        si (   lector->pons->amplitudo(lector->pons->datum, &lat, &alt)
            && (lat != lector->latitudo_nota
                || alt != lector->altitudo_nota))
        {
            lector->latitudo_nota  = lat;
            lector->altitudo_nota  = alt;
            eventum->genus         = TESSERA_EVENTUM_AMPLITUDO;
            eventum->latitudo      = lat;
            eventum->altitudo      = alt;
            redde eventum->genus;
        }
    }

    /* ESC ESC post moram: FUGA altera statim */
    si (lector->fuga_reddenda)
    {
        lector->fuga_reddenda = FALSUM;
        _clavem_ponere(eventum, TESSERA_CLAVIS_FUGA, ZEPHYRUM);
        lector->reliquiae[ZEPHYRUM]  = (i8)0x1B;
        lector->reliquiae_mensura    = I;
        lector->reliquiae_genus      = RELIQUIAE_FUGA;
        redde eventum->genus;
    }

    /* Octeti gestati primum */
    fructus = _parsare_plene(lector, eventum);
    si (fructus == PARS_COMPLETUM)
    {
        redde eventum->genus;
    }

    /* Legere (mora vocantis) SOLUM si nihil pendet: octeti pendentes
     * (INCOMPLETUM, e.g. ESC ultimus lectionis prioris) moram fugae
     * SOLAM habent - aliter clavis intra mora_ms adveniens cum ESC
     * pendenti in alt+clavem confunderetur (H6) */
    si (fructus == PARS_VACUUM)
    {
        s32 n = lector->pons->legere(lector->pons->datum,
            lector->buffer + lector->mensura,
            (i32)TESSERA_LECTOR_BUFFER - lector->mensura, mora_ms);

        si (n > ZEPHYRUM)
        {
            lector->mensura += (i32)n;
        }
        fructus = _parsare_plene(lector, eventum);
        si (fructus == PARS_COMPLETUM)
        {
            redde eventum->genus;
        }
    }

    /* INCOMPLETUM: legere DUM octeti intra moram fugae (~25ms)
     * adveniunt - series in lectiones quotlibet scissa (ssh, nexus
     * lenti) integra redit; lectio VACUA sola moram exactam
     * significat. Finitum: quaeque lectio octetos addit, buffer
     * finitus est (plenus = mora exacta tractatur). */
    dum (fructus == PARS_INCOMPLETUM)
    {
        s32 n;

        si (lector->mensura >= (i32)TESSERA_LECTOR_BUFFER)
        {
            frange;  /* plenus: nihil plus capi potest */
        }
        n = lector->pons->legere(lector->pons->datum,
            lector->buffer + lector->mensura,
            (i32)TESSERA_LECTOR_BUFFER - lector->mensura,
            TESSERA_MORA_FUGAE_MS);
        si (n <= ZEPHYRUM)
        {
            frange;  /* mora exacta (aut error) */
        }
        lector->mensura  += (i32)n;
        fructus          = _parsare_plene(lector, eventum);
        si (fructus == PARS_COMPLETUM)
        {
            redde eventum->genus;
        }
    }
    si (fructus == PARS_INCOMPLETUM)
    {
        redde _moram_tractare(lector, eventum);
    }
    redde TESSERA_EVENTUM_NIHIL;
}
