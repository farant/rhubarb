/* interpres_terminalis.c - Vide interpres_terminalis.h
 *
 * Tabulae legacy ex tessera_eventum.c (B1b) translatae in vocabularium
 * Eventus: claves nominatae (CSI A-D/H/F/Z, '~'-codices, SS3), mus
 * SGR/X10, regimina. Modificatores xterm (parametrum m): m-1 = bits
 * maiuscula I, alterum II, imperium IV, meta VIII (-> MOD_SUPER).
 */

#include "interpres_terminalis.h"
#include "utf8.h"
#include <string.h>

#define MODIFICATOR_MAXIMUS CCLVI   /* ultra: invalidum (ingens) */


/* ==================================================
 * Auxilia
 * ================================================== */

interior i32
_modificantes_csi (
    s32 m)
{
    i32 fructus = ZEPHYRUM;
    s32 bits;

    si (m <= I || m > MODIFICATOR_MAXIMUS)
    {
        redde ZEPHYRUM;
    }
    bits = m - I;
    si (bits & I)
    { fructus |= MOD_SHIFT;
    }
    si (bits & II)
    { fructus |= MOD_ALT;
    }
    si (bits & IV)
    { fructus |= MOD_IMPERIUM;
    }
    si (bits & VIII)
    { fructus |= MOD_SUPER;
    }
    redde fructus;
}

interior i32
_clavem (
    EventusCauda* cauda,
             s64  tempus,
        clavis_t  clavis,
             s32  runa,
             i32  modificantes,
    EventusCodex  codex)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus               = tempus;
    e.datum.clavis.clavis  = clavis;
    e.datum.clavis.typus         = ((s32)clavis > ZEPHYRUM
                                    && (s32)clavis < CXXVIII)
                                   ? (character)clavis : '\0';
    e.datum.clavis.modificantes  = modificantes;
    e.datum.clavis.runa          = runa;
    e.datum.clavis.codex         = codex;
    e.datum.clavis.actio         = EVENTUS_ACTIO_PRESSA;
    redde eventus_caudae_impellere(cauda, &e) ? I : ZEPHYRUM;
}

/* Runa imprimibilis -> clavis logica (litterae MAIUSCULAE ut fenestra;
 * runa litterarum minuscula - Shift nescitur, textus casum fert) */
interior i32
_runae_clavem (
    EventusCauda* cauda,
             s64  tempus,
             s32  r,
             i32  modificantes)
{
    clavis_t clavis  = CLAVIS_IGNOTA;
         s32 runa    = r;

    si (r >= 'a' && r <= 'z')
    {
        clavis  = (clavis_t)(r - 'a' + 'A');
    }
    alioquin si (r >= 'A' && r <= 'Z')
    {
        clavis  = (clavis_t)r;
        runa    = r - 'A' + 'a';
    }
    alioquin si (r >= 0x20 && r < 0x7F)
    {
        clavis = (clavis_t)r;
    }
    redde _clavem(cauda, tempus, clavis, runa, modificantes,
        EVENTUS_CODEX_IGNOTUS);
}

/* Octetus regiminis (C0, DEL) -> clavis. HONESTA: '\n' = Ctrl+J,
 * 0x08 = Ctrl+H (proiectio tesserae eas coniungit). */
interior i32
_regimen (
    EventusCauda* cauda,
             s64  tempus,
             i32  b,
             i32  modificantes)
{
    si (b == 0x0D)
    {
        redde _clavem(cauda, tempus, CLAVIS_REDITUS, ZEPHYRUM,
            modificantes, EVENTUS_CODEX_IGNOTUS);
    }
    si (b == 0x09)
    {
        redde _clavem(cauda, tempus, CLAVIS_TABULA, ZEPHYRUM,
            modificantes, EVENTUS_CODEX_IGNOTUS);
    }
    si (b == 0x7F)
    {
        redde _clavem(cauda, tempus, CLAVIS_RETRORSUM, ZEPHYRUM,
            modificantes, EVENTUS_CODEX_IGNOTUS);
    }
    si (b == ZEPHYRUM)
    {
        redde _clavem(cauda, tempus, CLAVIS_SPATIUM, (s32)' ',
            modificantes | MOD_IMPERIUM, EVENTUS_CODEX_IGNOTUS);
    }
    si (b >= I && b <= XXVI)
    {
        redde _clavem(cauda, tempus, (clavis_t)('A' + b - I),
            (s32)('a' + b - I), modificantes | MOD_IMPERIUM,
            EVENTUS_CODEX_IGNOTUS);
    }
    si (b >= 0x1C && b <= 0x1F)
    {
        redde _clavem(cauda, tempus, (clavis_t)(b | 0x40),
            (s32)(b | 0x40), modificantes | MOD_IMPERIUM,
            EVENTUS_CODEX_IGNOTUS);
    }
    redde ZEPHYRUM;
}

/* Clavis nominata ex finali (CSI aut SS3) */
interior i32
_finalem (
    EventusCauda* cauda,
             s64  tempus,
             i32  finale,
             i32  modificantes)
{
    commutatio (finale)
    {
        casus 'A': redde _clavem(cauda, tempus, CLAVIS_SURSUM, ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_SURSUM);
        casus 'B': redde _clavem(cauda, tempus, CLAVIS_DEORSUM,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_DEORSUM);
        casus 'C': redde _clavem(cauda, tempus, CLAVIS_DEXTER, ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_DEXTRA);
        casus 'D': redde _clavem(cauda, tempus, CLAVIS_SINISTER,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_SINISTRA);
        casus 'H': redde _clavem(cauda, tempus, CLAVIS_DOMUS, ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_DOMUS);
        casus 'F': redde _clavem(cauda, tempus, CLAVIS_FINIS, ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_FINIS);
        casus 'Z': redde _clavem(cauda, tempus, CLAVIS_TABULA, ZEPHYRUM,
                       modificantes | MOD_SHIFT, EVENTUS_CODEX_TABULA);
        ordinarius:
            frange;
    }
    redde ZEPHYRUM;
}

/* F n (1..12) */
interior i32
_functionem (
    EventusCauda* cauda,
             s64  tempus,
             s32  n,
             i32  modificantes)
{
    redde _clavem(cauda, tempus, (clavis_t)((s32)CLAVIS_F1 + n - I),
        ZEPHYRUM, modificantes,
        (EventusCodex)((s32)EVENTUS_CODEX_FUNCTIONES + n - I));
}

/* '~'-codices (xterm/vt220) */
interior i32
_clavem_tildae (
    EventusCauda* cauda,
             s64  tempus,
             s32  codex,
             i32  modificantes)
{
    commutatio (codex)
    {
        casus I:
        casus VII:
            redde _clavem(cauda, tempus, CLAVIS_DOMUS, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_DOMUS);
        casus IV:
        casus VIII:
            redde _clavem(cauda, tempus, CLAVIS_FINIS, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_FINIS);
        casus II:
            redde _clavem(cauda, tempus, CLAVIS_IGNOTA, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_INSERERE);
        casus III:
            redde _clavem(cauda, tempus, CLAVIS_DELERE, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_DELERE);
        casus V:
            redde _clavem(cauda, tempus, CLAVIS_PAGINA_SURSUM, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_PAGINA_SURSUM);
        casus VI:
            redde _clavem(cauda, tempus, CLAVIS_PAGINA_DEORSUM,
                ZEPHYRUM,
                modificantes, EVENTUS_CODEX_PAGINA_DEORSUM);
        ordinarius:
            frange;
    }
    si (codex >= XI && codex <= XV)
    {
        redde _functionem(cauda, tempus, codex - X, modificantes);
    }
    si (codex >= XVII && codex <= XXI)
    {
        redde _functionem(cauda, tempus, codex - XI, modificantes);
    }
    si (codex == XXIII || codex == XXIV)
    {
        redde _functionem(cauda, tempus, codex - XII, modificantes);
    }
    redde ZEPHYRUM;   /* ignota (200/201 glutinum: fons) */
}

/* Mus (SGR aut X10): b = codex bottonis (bits 0-1 botton, 4 maiuscula,
 * 8 alterum, 16 imperium, 32 motus, 64 rota); x, y cellulae 1-basatae
 * -> CENTRUM cellulae in pixelis nostris. Rota: gradus = cellula
 * altitudo; 64 sursum = dy +, 65 = dy -, 66 = dx +, 67 = dx -. */
interior i32
_murem (
    InterpresTerminalis* in,
           EventusCauda* cauda,
                    s64  tempus,
                    s32  b,
                    s32  x,
                    s32  y,
                    b32  solutio)
{
              Eventus e;
                  i32 modi   = ZEPHYRUM;
                  s32 basis  = b & III;
         mus_botton_t botton;

    si (b & IV)
    { modi |= MOD_SHIFT;
    }
    si (b & VIII)
    { modi |= MOD_ALT;
    }
    si (b & XVI)
    { modi |= MOD_IMPERIUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.tempus = tempus;
    si (b & LXIV)
    {
        s32 g = in->cellula_altitudo;

        si (solutio)
        {
            redde ZEPHYRUM;   /* rota solutionem non habet */
        }
        e.genus               = EVENTUS_MUS_ROTULA;
        e.datum.rotula.genus  = EVENTUS_ROTULA_GRADATA;
        e.datum.rotula.dy      = (basis == ZEPHYRUM) ? g
                               : (basis == I) ? -g : ZEPHYRUM;
        e.datum.rotula.dx      = (basis == II) ? g
                               : (basis == III) ? -g : ZEPHYRUM;
        e.datum.rotula.delta_x = (f32)e.datum.rotula.dx;
        e.datum.rotula.delta_y = (f32)e.datum.rotula.dy;
        redde eventus_caudae_impellere(cauda, &e) ? I : ZEPHYRUM;
    }
    botton = (basis == ZEPHYRUM) ? MUS_SINISTER
           : (basis == I) ? MUS_MEDIUS
           : (basis == II) ? MUS_DEXTER : (mus_botton_t)ZEPHYRUM;
    e.datum.mus.x                = (x - I) * in->cellula_latitudo
                                   + in->cellula_latitudo / II;
    e.datum.mus.y                = (y - I) * in->cellula_altitudo
                                   + in->cellula_altitudo / II;
    e.datum.mus.botton           = botton;
    e.datum.mus.modificantes     = modi;
    e.datum.mus.indicator_genus  = EVENTUS_INDICATOR_MUS;
    e.datum.mus.pressio          = EVENTUS_PRESSIO_IGNOTA;
    si (b & XXXII)
    {
        e.genus = EVENTUS_MUS_MOTUS;
        redde eventus_caudae_motum_impellere(cauda, &e) ? I : ZEPHYRUM;
    }
    e.genus = (solutio || basis == III) ? EVENTUS_MUS_LIBERATUS
                                        : EVENTUS_MUS_DEPRESSUS;
    redde eventus_caudae_impellere(cauda, &e) ? I : ZEPHYRUM;
}

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

/* Alterum pendens consumitur ab eventu proximo (clavis aut textus) */
interior i32
_alterum (
    InterpresTerminalis* in)
{
    i32 m = in->alterum_pendens ? MOD_ALT : ZEPHYRUM;

    in->alterum_pendens = FALSUM;
    redde m;
}


/* ==================================================
 * Publica
 * ================================================== */

vacuum
interpres_initiare (
    InterpresTerminalis* interpres,
                    s32  cellula_latitudo,
                    s32  cellula_altitudo)
{
    interpres->cellula_latitudo  = cellula_latitudo;
    interpres->cellula_altitudo  = cellula_altitudo;
    interpres->alterum_pendens   = FALSUM;
}

i32
interpres_lexema (
         InterpresTerminalis* in,
       constans SeriesLexema* l,
                         b32  post_moram,
                         s64  tempus,
                EventusCauda* cauda)
{
    i32 n = ZEPHYRUM;
    i32 modi;

    commutatio (l->genus)
    {
        casus SERIES_IMPRIMERE:
        {
            constans i8* p      = l->textus.datum;
            constans i8* finis  = p + l->textus.mensura;

            dum (p < finis)
            {
                constans i8* initium  = p;
                        s32  r        = utf8_decodere(&p, finis);

                si (r < ZEPHYRUM)
                {
                    si (p == initium)
                    {
                        p++;    /* octetus invalidus abicitur */
                    }
                    perge;
                }
                modi  = _alterum(in);
                n     += _runae_clavem(cauda, tempus, r, modi);
                si (modi == ZEPHYRUM)
                {
                    n += eventus_caudae_textum_impellere(cauda, tempus,
                        initium, (i32)(p - initium),
                        EVENTUS_ORIGO_SCRIPTA) ? I : ZEPHYRUM;
                }
            }
            redde n;
        }

        casus SERIES_EXSEQUI:
            redde _regimen(cauda, tempus, ((i32)l->finale) & 0xFF,
                _alterum(in));

        casus SERIES_FUGA:
            si (!post_moram)
            {
                /* abrupta: ESC solus = alterum clavis proximae */
                si (_sola_fuga(l))
                {
                    in->alterum_pendens = VERUM;
                }
                redde ZEPHYRUM;
            }
            in->alterum_pendens = FALSUM;
            si (_sola_fuga(l))
            {
                i32 k;

                /* ESC (ESC) post moram: Effugium pro quoque */
                per (k = ZEPHYRUM; k < l->crudum.mensura; k++)
                {
                    n += _clavem(cauda, tempus, CLAVIS_EFFUGIUM,
                        ZEPHYRUM,
                        ZEPHYRUM, EVENTUS_CODEX_IGNOTUS);
                }
                redde n;
            }
            si (l->crudum.mensura == II)
            {
                /* 'ESC [' / 'ESC O' / 'ESC P' solum = alterum + x */
                redde _runae_clavem(cauda, tempus,
                    ((s32)l->crudum.datum[I]) & 0xFF, MOD_ALT);
            }
            redde ZEPHYRUM;   /* series dimidia abicitur */

        casus SERIES_ESC:
            in->alterum_pendens = FALSUM;
            si (   l->numerus_intermediorum > ZEPHYRUM
                || l->finale < 0x20 || l->finale > 0x7E)
            {
                redde ZEPHYRUM;
            }
            redde _runae_clavem(cauda, tempus, (s32)l->finale, MOD_ALT);

        casus SERIES_CSI:
        {
            s32 p0 = (l->numerus_parametrorum >= I) ? l->parametra[0]
                                                    : ZEPHYRUM;

            in->alterum_pendens = FALSUM;
            /* kitty (':') B2b; intermedia, privata praeter '<' */
            si (   l->numerus_intermediorum > ZEPHYRUM
                || l->separatores != ZEPHYRUM
                || (l->privatum != ZEPHYRUM && l->privatum != '<'))
            {
                redde ZEPHYRUM;
            }
            si (l->privatum == '<')
            {
                si (   (l->finale == 'M' || l->finale == 'm')
                    && l->numerus_parametrorum >= III)
                {
                    redde _murem(in, cauda, tempus, p0, l->parametra[I],
                        l->parametra[II], (b32)(l->finale == 'm'));
                }
                redde ZEPHYRUM;
            }
            si (l->numerus_parametrorum == ZEPHYRUM && l->finale == 'I')
            {
                Eventus e;

                memset(&e, ZEPHYRUM, magnitudo(Eventus));
                e.genus   = EVENTUS_FOCUS;
                e.tempus  = tempus;
                redde eventus_caudae_impellere(cauda,
                    &e) ? I : ZEPHYRUM;
            }
            si (l->numerus_parametrorum == ZEPHYRUM && l->finale == 'O')
            {
                Eventus e;

                memset(&e, ZEPHYRUM, magnitudo(Eventus));
                e.genus   = EVENTUS_DEFOCUS;
                e.tempus  = tempus;
                redde eventus_caudae_impellere(cauda,
                    &e) ? I : ZEPHYRUM;
            }
            modi = (l->numerus_parametrorum >= II)
                ? _modificantes_csi(l->parametra[I]) : ZEPHYRUM;
            si (l->praefixum)
            {
                modi |= MOD_ALT;   /* ESC ESC [ A = alterum + sursum */
            }
            si (l->finale == '~')
            {
                redde (l->numerus_parametrorum >= I)
                    ? _clavem_tildae(cauda, tempus, p0,
                    modi) : ZEPHYRUM;
            }
            redde _finalem(cauda, tempus, (i32)l->finale, modi);
        }

        casus SERIES_SS:
            in->alterum_pendens = FALSUM;
            si (l->introductor != 'O')
            {
                redde ZEPHYRUM;
            }
            modi = (l->numerus_parametrorum >= I)
                ? _modificantes_csi(l->parametra[0]) : ZEPHYRUM;
            si (l->praefixum)
            {
                modi |= MOD_ALT;
            }
            si (l->finale >= 'P' && l->finale <= 'S')
            {
                redde _functionem(cauda, tempus,
                    (s32)(l->finale - 'P') + I, modi);
            }
            redde _finalem(cauda, tempus, (i32)l->finale, modi);

        ordinarius:
            /* OSC, DCS, APC (responsa), NIHIL */
            redde ZEPHYRUM;
    }
}

i32
interpres_x10 (
    InterpresTerminalis* interpres,
                    i32  cb,
                    i32  cx,
                    i32  cy,
                    s64  tempus,
           EventusCauda* cauda)
{
    s32 b = (s32)cb - XXXII;

    si (b < ZEPHYRUM || cx < XXXIII || cy < XXXIII)
    {
        redde ZEPHYRUM;   /* onus malum */
    }
    redde _murem(interpres, cauda, tempus, b, (s32)cx - XXXII,
        (s32)cy - XXXII, FALSUM);
}

i32
interpres_glutinum (
    InterpresTerminalis* interpres,
            constans i8* octeti,
                    i32  mensura,
                    s64  tempus,
           EventusCauda* cauda)
{
    (vacuum)interpres;
    redde eventus_caudae_textum_impellere(cauda, tempus, octeti,
        mensura,
        EVENTUS_ORIGO_GLUTINATA) ? I : ZEPHYRUM;
}
