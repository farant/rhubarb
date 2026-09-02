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

nomen enumeratio {
    PARS_COMPLETUM = 0,
    PARS_INCOMPLETUM,
    PARS_VACUUM,
    PARS_PRAETERITUM
} ParsFructus;

interior vacuum
_eventum_vacare (
    TesseraEventum* ev)
{
    ev->genus          = TESSERA_EVENTUM_NIHIL;
    ev->runa           = ZEPHYRUM;
    ev->clavis         = TESSERA_CLAVIS_NULLA;
    ev->modificatores  = ZEPHYRUM;
    ev->numerus        = ZEPHYRUM;
    ev->mus_genus      = TESSERA_MUS_PRESSUS;
    ev->mus_x          = ZEPHYRUM;
    ev->mus_y          = ZEPHYRUM;
    ev->mus_pulsus     = ZEPHYRUM;
    ev->latitudo       = ZEPHYRUM;
    ev->altitudo       = ZEPHYRUM;
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

    si (m <= I)
    {
        redde ZEPHYRUM;
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

/* CSI: buffer[0]=ESC buffer[1]='['. Parametra numerica leguntur,
 * octetus finalis 0x40-0x7E. Mus SGR: '<' post CSI. */
interior ParsFructus
_csi_parsare (
     TesseraLector* lector,
    TesseraEventum* ev,
               i32* consumendum)
{
          i32 i        = II;
          b32 est_mus  = FALSUM;
          b32 privata  = FALSUM;
          s32 parametra[IV];
          i32 numerus_parametrorum  = ZEPHYRUM;
          s32 valor_currens         = ZEPHYRUM;
          b32 valor_visus           = FALSUM;
    character finalis               = '\0';

    si (i < lector->mensura && lector->buffer[i] == '<')
    {
        est_mus = VERUM;
        i++;
    }
    /* Grammatica CSI: parametra 0x30-0x3F, intermedia 0x20-0x2F,
     * finalis 0x40-0x7E. Praefixa privata (?, >, =) sequentiam
     * TOTAM ignotam faciunt - sed usque ad finalem scanditur
     * (\033[?1049h etc. octetos phantasma non effundit). */
    dum (i < lector->mensura)
    {
        i8 b = lector->buffer[i];

        si (b >= '0' && b <= '9')
        {
            valor_currens  = valor_currens * X + (s32)(b - '0');
            valor_visus    = VERUM;
            i++;
        }
        alioquin si (b == ';')
        {
            si (numerus_parametrorum < IV)
            {
                parametra[numerus_parametrorum++] = valor_currens;
            }
            valor_currens  = ZEPHYRUM;
            valor_visus    = FALSUM;
            i++;
        }
        alioquin si (b >= 0x30 && b <= 0x3F)
        {
            privata = VERUM;  /* ?, >, =, : - privata/ignota */
            i++;
        }
        alioquin si (b >= 0x20 && b <= 0x2F)
        {
            privata = VERUM;  /* intermedia - forma ignota nobis */
            i++;
        }
        alioquin si (b >= 0x40 && b <= 0x7E)
        {
            finalis = (character)b;
            i++;
            frange;
        }
        alioquin
        {
            /* octetus regiminis INTRA seriem: series abrupta -
             * partem visam consumere, octetum relinquere */
            *consumendum = i;
            redde PARS_PRAETERITUM;
        }
    }
    si (finalis == '\0')
    {
        redde PARS_INCOMPLETUM;
    }
    si (privata)
    {
        *consumendum = i;
        redde PARS_PRAETERITUM;  /* tota tacite consumpta */
    }
    si (valor_visus && numerus_parametrorum < IV)
    {
        parametra[numerus_parametrorum++] = valor_currens;
    }
    *consumendum = i;

    si (   est_mus && (finalis == 'M' || finalis == 'm')
        && numerus_parametrorum >= III)
    {
        s32 pulsus = parametra[ZEPHYRUM];

        ev->genus = TESSERA_EVENTUM_MUS;
        ev->mus_x = parametra[I] - I;    /* 1-basata -> 0 */
        ev->mus_y = parametra[II] - I;
        si (pulsus & LXIV)
        {
            ev->mus_genus = (pulsus & I) ? TESSERA_MUS_ROTA_DEORSUM
                                         : TESSERA_MUS_ROTA_SURSUM;
            ev->mus_pulsus = ZEPHYRUM;
        }
        alioquin
        {
            ev->mus_genus = (finalis == 'M') ? TESSERA_MUS_PRESSUS
                                             : TESSERA_MUS_SOLUTUS;
            ev->mus_pulsus = (i32)(pulsus & III);
        }
        redde PARS_COMPLETUM;
    }

    {
        i32 modificatores = (numerus_parametrorum >= II)
            ? _modificatores_csi(parametra[I]) : ZEPHYRUM;

        si (finalis == '~' && numerus_parametrorum >= I)
        {
            si (_clavem_tildae(parametra[ZEPHYRUM], ev,
                    modificatores))
            {
                redde PARS_COMPLETUM;
            }
            redde PARS_PRAETERITUM;
        }
        si (_clavem_finalem(finalis, ev, modificatores))
        {
            redde PARS_COMPLETUM;
        }
    }
    redde PARS_PRAETERITUM;  /* CSI ignota tacite consumpta */
}

/* SS3: ESC O <finalis> - frecce + F1-F4 (modus applicationis) */
interior ParsFructus
_ss3_parsare (
     TesseraLector* lector,
    TesseraEventum* ev,
               i32* consumendum)
{
    character finalis;

    si (lector->mensura < III)
    {
        redde PARS_INCOMPLETUM;
    }
    finalis       = (character)lector->buffer[II];
    *consumendum  = III;
    si (_clavem_finalem(finalis, ev, ZEPHYRUM))
    {
        redde PARS_COMPLETUM;
    }
    si (finalis >= 'P' && finalis <= 'S')
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_FUNCTIO, ZEPHYRUM);
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

/* Unum eventum a fronte bufferis parsare temptare */
interior ParsFructus
_parsare (
     TesseraLector* lector,
    TesseraEventum* ev)
{
             i8 primus;
            i32 consumendum = ZEPHYRUM;
    ParsFructus fructus;

    si (lector->mensura == ZEPHYRUM)
    {
        redde PARS_VACUUM;
    }
    primus = lector->buffer[ZEPHYRUM];

    si (primus == 0x1B)
    {
        si (lector->mensura == I)
        {
            redde PARS_INCOMPLETUM;  /* FUGA sola? mora dicet */
        }
        si (lector->buffer[I] == '[')
        {
            fructus = _csi_parsare(lector, ev, &consumendum);
            si (   fructus == PARS_COMPLETUM
                || fructus == PARS_PRAETERITUM)
            {
                _consumere(lector, consumendum);
            }
            redde fructus;
        }
        si (lector->buffer[I] == 'O')
        {
            fructus = _ss3_parsare(lector, ev, &consumendum);
            si (   fructus == PARS_COMPLETUM
                || fructus == PARS_PRAETERITUM)
            {
                _consumere(lector, consumendum);
            }
            redde fructus;
        }
        /* ALTERUM + clavis: octetum post ESC parsare */
        {
            i8 alter = lector->buffer[I];

            si (alter == 0x1B)
            {
                /* ESC ESC: FUGA una, altera manet */
                _clavem_ponere(ev, TESSERA_CLAVIS_FUGA, ZEPHYRUM);
                _consumere(lector, I);
                redde PARS_COMPLETUM;
            }
            si (alter < 0x20 || alter == 0x7F)
            {
                _regimen_parsare(alter, ev,
                    TESSERA_MODIFICATOR_ALTERUM);
                _consumere(lector, II);
                redde PARS_COMPLETUM;
            }
            {
                constans i8* cursor  = lector->buffer + I;
                constans i8* finis   = lector->buffer + lector->mensura;
                        s32  longitudo_runae =
                            utf8_longitudo_byte(lector->buffer[I]);
                s32 runa;

                si (   longitudo_runae > ZEPHYRUM
                    && (i32)longitudo_runae > lector->mensura - I)
                {
                    redde PARS_INCOMPLETUM;
                }
                runa = utf8_decodere(&cursor, finis);
                si (runa < ZEPHYRUM)
                {
                    /* invalidum post ESC: FUGA + octetum relinquere */
                    _clavem_ponere(ev, TESSERA_CLAVIS_FUGA, ZEPHYRUM);
                    _consumere(lector, I);
                    redde PARS_COMPLETUM;
                }
                _runam_ponere(ev, runa, TESSERA_MODIFICATOR_ALTERUM);
                _consumere(lector,
                    (i32)(cursor - lector->buffer));
                redde PARS_COMPLETUM;
            }
        }
    }

    si (primus < 0x20 || primus == 0x7F)
    {
        _regimen_parsare(primus, ev, ZEPHYRUM);
        _consumere(lector, I);
        redde PARS_COMPLETUM;
    }

    /* runa UTF-8 */
    {
        constans i8* cursor = lector->buffer;
        constans i8* finis = lector->buffer + lector->mensura;
                s32  longitudo_runae = utf8_longitudo_byte(primus);
                s32  runa;

        si (   longitudo_runae > ZEPHYRUM
            && (i32)longitudo_runae > lector->mensura)
        {
            redde PARS_INCOMPLETUM;  /* runa dimidia in fine */
        }
        runa = utf8_decodere(&cursor, finis);
        si (runa < ZEPHYRUM)
        {
            _consumere(lector, I);   /* octetus invalidus abicitur */
            redde PARS_PRAETERITUM;
        }
        _runam_ponere(ev, runa, ZEPHYRUM);
        _consumere(lector, (i32)(cursor - lector->buffer));
        redde PARS_COMPLETUM;
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
    lector->pons     = pons;
    lector->mensura  = ZEPHYRUM;
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

    /* Octeti gestati primum */
    dum (VERUM)
    {
        fructus = _parsare(lector, eventum);
        si (fructus == PARS_COMPLETUM)
        {
            redde eventum->genus;
        }
        si (fructus == PARS_PRAETERITUM)
        {
            perge;  /* strepitus consumptus; iterum */
        }
        frange;  /* VACUUM aut INCOMPLETUM: legendum */
    }

    /* Legere (mora vocantis), deinde parsare iterum */
    {
        s32 n = lector->pons->legere(lector->pons->datum,
            lector->buffer + lector->mensura,
            (i32)TESSERA_LECTOR_BUFFER - lector->mensura, mora_ms);

        si (n > ZEPHYRUM)
        {
            lector->mensura += (i32)n;
        }
    }
    dum (VERUM)
    {
        fructus = _parsare(lector, eventum);
        si (fructus == PARS_COMPLETUM)
        {
            redde eventum->genus;
        }
        si (fructus == PARS_PRAETERITUM)
        {
            perge;
        }
        frange;
    }

    /* INCOMPLETUM: mora fugae (~25ms) pro reliquo seriei */
    si (fructus == PARS_INCOMPLETUM)
    {
        s32 n = lector->pons->legere(lector->pons->datum,
            lector->buffer + lector->mensura,
            (i32)TESSERA_LECTOR_BUFFER - lector->mensura,
            TESSERA_MORA_FUGAE_MS);

        si (n > ZEPHYRUM)
        {
            lector->mensura += (i32)n;
            dum (VERUM)
            {
                fructus = _parsare(lector, eventum);
                si (fructus == PARS_COMPLETUM)
                {
                    redde eventum->genus;
                }
                si (fructus == PARS_PRAETERITUM)
                {
                    perge;
                }
                frange;
            }
        }
        si (fructus == PARS_INCOMPLETUM)
        {
            /* mora exacta: ESC solum = FUGA; runa dimidia abicitur */
            si (lector->buffer[ZEPHYRUM] == 0x1B)
            {
                _clavem_ponere(eventum, TESSERA_CLAVIS_FUGA,
                    ZEPHYRUM);
                _consumere(lector, I);
                redde eventum->genus;
            }
            _consumere(lector, I);
        }
    }
    redde TESSERA_EVENTUM_NIHIL;
}
