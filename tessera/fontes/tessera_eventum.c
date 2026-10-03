/* tessera_eventum.c - Lector initus tesserae: PROIECTIO rivi (eventus
 * B3a)
 *
 * Pipeline (lexemator, mora ESC, reliquiae H7/H8, X10, caudae alienae,
 * glutinum) in lib/rivus_terminalis habitat; hic solum (1) I/O: pons
 * legitur cum mora quam rivus poscit, (2) PROIECTIO: Eventus sine
 * iactura -> TesseraEventum deperditum (xterm vetus):
 *   Ctrl+J -> REDITUS, Ctrl+H -> RETRORSUM (coniunctiones deperditae);
 *   runa = character verus (typus) - sub imperio runa minuscula;
 *   pixela -> cellulae (rivus cellula I x I: pixelum = cellula);
 *   botton ignotus -> pulsus III; motus sine bottone, focus,
 *   facultates, solutiones, textus scriptus (clavis eum fert)
 *   omittuntur; textus glutinatus -> GLUTINUM.
 */

#include "tessera_eventum.h"
#include <string.h>

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

interior i32
_modificatores (
    i32 m)
{
    i32 fructus = ZEPHYRUM;

    si (m & MOD_SHIFT)
    {
        fructus |= TESSERA_MODIFICATOR_MAIUSCULA;
    }
    si (m & MOD_ALT)
    {
        fructus |= TESSERA_MODIFICATOR_ALTERUM;
    }
    si (m & MOD_IMPERIUM)
    {
        fructus |= TESSERA_MODIFICATOR_IMPERIUM;
    }
    redde fructus;
}

interior b32
_clavem_ponere (
    TesseraEventum* ev,
     TesseraClavis  clavis,
               i32  modificatores)
{
    ev->genus          = TESSERA_EVENTUM_CLAVIS;
    ev->clavis         = clavis;
    ev->modificatores  = modificatores;
    redde VERUM;
}

/* Clavis: nominatae ad TesseraClavis, ceterae runa (character verus) */
interior b32
_clavem_proicere (
    constans Eventus* e,
      TesseraEventum* ev)
{
    i32 modi  = _modificatores(e->datum.clavis.modificantes);
    s32 c     = (s32)e->datum.clavis.clavis;
    s32 producta;

    commutatio (e->datum.clavis.clavis)
    {
        casus CLAVIS_REDITUS:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_REDITUS, modi);
        casus CLAVIS_TABULA:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_TABULA, modi);
        casus CLAVIS_RETRORSUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_RETRORSUM, modi);
        casus CLAVIS_EFFUGIUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_FUGA, modi);
        casus CLAVIS_SURSUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_SURSUM, modi);
        casus CLAVIS_DEORSUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_DEORSUM, modi);
        casus CLAVIS_DEXTER:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_DEXTRA, modi);
        casus CLAVIS_SINISTER:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_SINISTRA, modi);
        casus CLAVIS_DOMUS:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_DOMUS, modi);
        casus CLAVIS_FINIS:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_FINIS, modi);
        casus CLAVIS_PAGINA_SURSUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_PAGINA_SURSUM,
                modi);
        casus CLAVIS_PAGINA_DEORSUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_PAGINA_DEORSUM,
                modi);
        casus CLAVIS_DELERE:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_DELETIO, modi);
        ordinarius:
            frange;
    }
    si (c >= (s32)CLAVIS_F1 && c <= (s32)CLAVIS_F12)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_FUNCTIO, modi);
        ev->numerus = (i32)(c - (s32)CLAVIS_F1) + I;
        redde VERUM;
    }
    si (e->datum.clavis.codex == EVENTUS_CODEX_INSERERE)
    {
        redde _clavem_ponere(ev, TESSERA_CLAVIS_INSERTIO, modi);
    }
    /* coniunctiones deperditae: Ctrl+J = '\n' -> reditus, Ctrl+H =
     * 0x08 -> retrorsum */
    si (modi & TESSERA_MODIFICATOR_IMPERIUM)
    {
        si (c == 'J')
        {
            redde _clavem_ponere(ev, TESSERA_CLAVIS_REDITUS,
                modi & ~(i32)TESSERA_MODIFICATOR_IMPERIUM);
        }
        si (c == 'H')
        {
            redde _clavem_ponere(ev, TESSERA_CLAVIS_RETRORSUM,
                modi & ~(i32)TESSERA_MODIFICATOR_IMPERIUM);
        }
    }
    si (e->datum.clavis.runa == ZEPHYRUM)
    {
        redde FALSUM;   /* clavis sine nomine tesserae (F13, ...) */
    }
    /* runa: character verus (producta) - sub imperio runa
     * (minuscula, 'ctrl+a' ut olim) */
    producta           = e->datum.clavis.producta;
    ev->genus          = TESSERA_EVENTUM_CLAVIS;
    ev->modificatores  = modi;
    ev->runa = (   !(modi & TESSERA_MODIFICATOR_IMPERIUM)
                && producta >= 0x20 && producta != 0x7F)
        ? producta : e->datum.clavis.runa;
    redde VERUM;
}

interior i32
_pulsus (
    mus_botton_t b)
{
    commutatio (b)
    {
        casus MUS_SINISTER: redde ZEPHYRUM;
        casus MUS_MEDIUS:   redde I;
        casus MUS_DEXTER:   redde II;
        ordinarius:         redde III;   /* ignotus (X10 solutio) */
    }
}

interior b32
_proicere (
    constans Eventus* e,
      TesseraEventum* ev)
{
    commutatio (e->genus)
    {
        casus EVENTUS_CLAVIS_DEPRESSUS:
            redde _clavem_proicere(e, ev);

        casus EVENTUS_TEXTUS:
            si (e->datum.textus.origo != EVENTUS_ORIGO_GLUTINATA)
            {
                redde FALSUM;   /* textus scriptus: clavis eum fert */
            }
            ev->genus               = TESSERA_EVENTUM_GLUTINUM;
            ev->glutinum            = e->datum.textus.contentum;
            ev->glutinum_truncatum  = e->datum.textus.truncatum;
            redde VERUM;

        casus EVENTUS_MUS_DEPRESSUS:
        casus EVENTUS_MUS_LIBERATUS:
        casus EVENTUS_MUS_MOTUS:
            si (   e->genus            == EVENTUS_MUS_MOTUS
                && e->datum.mus.botton == (mus_botton_t)ZEPHYRUM)
            {
                redde FALSUM;   /* motus sine bottone (?1003 non petitus) */
            }
            ev->genus          = TESSERA_EVENTUM_MUS;
            ev->mus_genus      = (e->genus == EVENTUS_MUS_DEPRESSUS)
                                     ? TESSERA_MUS_PRESSUS
                               : (e->genus == EVENTUS_MUS_LIBERATUS)
                                     ? TESSERA_MUS_SOLUTUS
                                     : TESSERA_MUS_TRACTUS;
            ev->mus_x          = e->datum.mus.x;
            ev->mus_y          = e->datum.mus.y;
            ev->mus_pulsus     = _pulsus(e->datum.mus.botton);
            ev->modificatores  =
                _modificatores(e->datum.mus.modificantes);
            redde VERUM;

        casus EVENTUS_MUS_ROTULA:
            ev->genus          = TESSERA_EVENTUM_MUS;
            ev->mus_genus      = (e->datum.rotula.dy > ZEPHYRUM)
                                     ? TESSERA_MUS_ROTA_SURSUM
                               : (e->datum.rotula.dy < ZEPHYRUM)
                                     ? TESSERA_MUS_ROTA_DEORSUM
                               : (e->datum.rotula.dx > ZEPHYRUM)
                                     ? TESSERA_MUS_ROTA_SINISTRORSUM
                                     : TESSERA_MUS_ROTA_DEXTRORSUM;
            ev->mus_x          = e->datum.rotula.x;
            ev->mus_y          = e->datum.rotula.y;
            ev->mus_pulsus     = ZEPHYRUM;
            ev->modificatores  = _modificatores(
                e->datum.rotula.modificantes);
            redde VERUM;

        ordinarius:
            redde FALSUM;   /* solutiones, focus, facultates */
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
        (memoriae_index)magnitudo(TesseraLector), VIII);
    si (lector == NIHIL)
    {
        redde NIHIL;
    }
    /* cellula I x I: pixela rivi = cellulae tesserae */
    lector->rivus = rivus_creare(piscina, I, I);
    si (lector->rivus == NIHIL)
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

    /* Eventa rivi proiciuntur; deficientibus legitur - mora RIVI si
     * aliquid pendet (series dimidia, runa, glutinum: H6, ESC pendens
     * moram fugae SOLAM habet), alioquin mora vocantis. Silentium post
     * moram rivi -> rivus_moram (ESC = fuga, reliquiae ...). */
    per (;;)
    {
        Eventus e;
            s32 mora;
            s32 n;
            i32 capax;

        dum (rivus_eventum(lector->rivus, ZEPHYRUM, &e))
        {
            si (_proicere(&e, eventum))
            {
                lector->mensura = rivus_pendentes(lector->rivus);
                redde eventum->genus;
            }
        }
        mora   = rivus_mora_ms(lector->rivus);
        capax  = rivus_spatium(lector->rivus);
        si (capax > (i32)TESSERA_LECTOR_BUFFER)
        {
            capax = (i32)TESSERA_LECTOR_BUFFER;
        }
        n = (capax > ZEPHYRUM)
            ? lector->pons->legere(lector->pons->datum, lector->buffer,
                  capax, (mora > ZEPHYRUM) ? mora : mora_ms)
            : ZEPHYRUM;
        si (n > ZEPHYRUM)
        {
            (vacuum)rivus_tradere(lector->rivus, lector->buffer,
                (i32)n);
            perge;
        }
        si (mora > ZEPHYRUM || capax == ZEPHYRUM)
        {
            rivus_moram(lector->rivus, ZEPHYRUM);
            perge;
        }
        lector->mensura = rivus_pendentes(lector->rivus);
        redde TESSERA_EVENTUM_NIHIL;
    }
}
