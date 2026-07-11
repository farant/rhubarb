/* vindex_visum.c - pictura vindicis (vide .h; conspectus purus) */

#include "vindex_visum.h"

#include <stdio.h>
#include <string.h>

interior TesseraStilus
_stilus_nativus (vacuum)
{
    TesseraStilus s;

    s.color_litterae = TESSERA_COLOR_NATIVUS;
    s.color_fundi = TESSERA_COLOR_NATIVUS;
    s.ornamenta = 0U;
    redde s;
}

interior TesseraStilus
_stilus_inversus (vacuum)
{
    TesseraStilus s = _stilus_nativus();

    s.ornamenta = TESSERA_ORNAMENTUM_INVERSUM;
    redde s;
}

interior TesseraStilus
_stilus_crassus (vacuum)
{
    TesseraStilus s = _stilus_nativus();

    s.ornamenta = TESSERA_ORNAMENTUM_CRASSUM;
    redde s;
}

/* scribere praecisum ad latitudinem tabulae */
interior vacuum
_scribe (TesseraOpus* opus, integer x, integer y, chorda textus,
    integer latitudo_maxima, TesseraStilus stilus)
{
    TesseraChorda tc;
    i32 mensura = textus.mensura;

    si (latitudo_maxima <= 0 || textus.datum == NIHIL) redde;
    si ((integer)mensura > latitudo_maxima)
    {
        mensura = (i32)latitudo_maxima;
    }
    tc.datum = (insignatus character*)textus.datum;
    tc.mensura = mensura;
    tessera_scribere(opus, x, y, tc, stilus);
}

interior vacuum
_scribe_literis (TesseraOpus* opus, integer x, integer y,
    constans character* literis, integer latitudo_maxima,
    TesseraStilus stilus)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (i32)strlen(literis);
    _scribe(opus, x, y, c, latitudo_maxima, stilus);
}

interior b32
_linea_punctata (constans VindexConspectus* conspectus, i32 linea)
{
    i32 i;

    per (i = ZEPHYRUM; i < conspectus->puncta_numerus; i++)
    {
        si (conspectus->puncta_lineae[i] == linea)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_tabulam_fontis_pingere (constans VindexConspectus* conspectus,
    TesseraOpus* opus, integer x, integer y, integer latitudo,
    integer altitudo)
{
    integer v;

    per (v = 0; v < altitudo; v++)
    {
        i32 linea = conspectus->linea_prima + (i32)v;
        character numeri[16];
        b32 currens;

        si ((integer)linea > (integer)conspectus->numerus_linearum
            || linea < I)
        {
            frange;
        }
        currens = (b32)(linea == conspectus->linea_currens);
        si (_linea_punctata(conspectus, linea))
        {
            _scribe_literis(opus, x, y + v, "*", 1,
                _stilus_crassus());
        }
        si (currens)
        {
            _scribe_literis(opus, x + 1, y + v, ">", 1,
                _stilus_crassus());
        }
        sprintf(numeri, "%4d", (integer)linea);
        _scribe_literis(opus, x + 2, y + v, numeri, 4,
            _stilus_nativus());
        _scribe_literis(opus, x + 6, y + v, "|", 1,
            _stilus_nativus());
        _scribe(opus, x + 7, y + v,
            conspectus->lineae[linea - I], latitudo - 7,
            currens ? _stilus_crassus() : _stilus_nativus());
    }
}

interior vacuum
_columnam_dextram_pingere (constans VindexConspectus* conspectus,
    TesseraOpus* opus, integer x, integer y, integer latitudo,
    integer altitudo)
{
    integer altitudo_stivae = altitudo / 2;
    integer v;

    /* stiva (supra) */
    _scribe_literis(opus, x, y, " STIVA ", latitudo,
        _stilus_inversus());
    per (v = 0; v < altitudo_stivae - 1
        && v < (integer)conspectus->tabulata_numerus; v++)
    {
        constans VindexOrdoTabulati* ordo =
            &conspectus->tabulata[v];

        _scribe(opus, x, y + 1 + v, ordo->functio,
            latitudo / 2, _stilus_nativus());
        _scribe(opus, x + latitudo / 2 + 1, y + 1 + v,
            ordo->positio, latitudo - latitudo / 2 - 1,
            _stilus_nativus());
    }

    /* tabula mutabilis (infra) */
    {
        integer y_infra = y + altitudo_stivae;
        integer altitudo_infra = altitudo - altitudo_stivae - 1;
        constans character* tituli[] = {
            " VARIABILIA ", " ANULUS ", " PUNCTA ", " MEMORIA "
        };

        _scribe_literis(opus, x, y_infra,
            tituli[conspectus->tabula_dextra], latitudo,
            _stilus_inversus());
        commutatio (conspectus->tabula_dextra)
        {
        casus VINDEX_TABULA_VARIABILIA:
            per (v = 0; v < altitudo_infra
                && v < (integer)conspectus->variabilia_numerus;
                v++)
            {
                constans VindexOrdoVariabilis* ordo =
                    &conspectus->variabilia[v];
                integer tertia = latitudo / 3;

                _scribe(opus, x, y_infra + 1 + v, ordo->titulus,
                    tertia, _stilus_nativus());
                _scribe(opus, x + tertia + 1, y_infra + 1 + v,
                    ordo->typus, tertia, _stilus_nativus());
                _scribe(opus, x + 2 * tertia + 2, y_infra + 1 + v,
                    ordo->valor, latitudo - 2 * tertia - 2,
                    _stilus_crassus());
            }
            frange;
        casus VINDEX_TABULA_ANULUS:
            per (v = 0; v < altitudo_infra
                && v < (integer)conspectus->anulus_numerus; v++)
            {
                _scribe(opus, x, y_infra + 1 + v,
                    conspectus->anulus[v], latitudo,
                    _stilus_nativus());
            }
            frange;
        casus VINDEX_TABULA_PUNCTA:
            per (v = 0; v < altitudo_infra
                && v < (integer)conspectus->puncta_ordines_numerus;
                v++)
            {
                _scribe(opus, x, y_infra + 1 + v,
                    conspectus->puncta[v], latitudo,
                    _stilus_nativus());
            }
            frange;
        ordinarius:   /* MEMORIA */
            per (v = 0; v < altitudo_infra
                && v < (integer)conspectus->memoria_numerus; v++)
            {
                _scribe(opus, x, y_infra + 1 + v,
                    conspectus->memoria[v], latitudo,
                    _stilus_nativus());
            }
            frange;
        }
    }
}

vacuum
vindex_visum_pingere (constans VindexConspectus* conspectus,
    TesseraOpus* opus)
{
    integer latitudo = (integer)tessera_latitudo(opus);
    integer altitudo = (integer)tessera_altitudo(opus);
    integer lat_sinistra;
    integer alt_media;

    si (latitudo < 40 || altitudo < 10)
    {
        redde;   /* fenestra nimis parva - nihil pingere */
    }
    lat_sinistra = (latitudo * 62) / 100;
    alt_media = altitudo - 3;
    tessera_purgare(opus, _stilus_nativus());

    /* vectis tituli */
    {
        character titulus[256];

        sprintf(titulus, " VINDEX  %.*s  [%.*s] ",
            (int)((conspectus->titulus_fontis.mensura < 180U)
                ? conspectus->titulus_fontis.mensura : 180U),
            (constans character*)conspectus->titulus_fontis.datum,
            (int)((conspectus->modus.mensura < 32U)
                ? conspectus->modus.mensura : 32U),
            (constans character*)conspectus->modus.datum);
        tessera_replere(opus, 0, 0, latitudo, 1, (insignatus
            integer)' ', _stilus_inversus());
        _scribe_literis(opus, 0, 0, titulus, latitudo,
            _stilus_inversus());
    }

    /* fons + divisor + columna dextra */
    _tabulam_fontis_pingere(conspectus, opus, 0, 1,
        lat_sinistra, alt_media);
    tessera_lineam_pingere(opus, lat_sinistra, 1, alt_media, 1,
        TESSERA_LINEA_SIMPLEX, _stilus_nativus());
    _columnam_dextram_pingere(conspectus, opus, lat_sinistra + 2,
        1, latitudo - lat_sinistra - 2, alt_media);

    /* linea positionis (acies macronum) */
    tessera_replere(opus, 0, altitudo - 2, latitudo, 1,
        (insignatus integer)' ', _stilus_inversus());
    _scribe(opus, 0, altitudo - 2, conspectus->positio, latitudo,
        _stilus_inversus());

    /* status aut linea imperii */
    si (conspectus->imperium_apertum)
    {
        _scribe_literis(opus, 0, altitudo - 1, ":", 1,
            _stilus_nativus());
        _scribe(opus, 1, altitudo - 1, conspectus->imperium,
            latitudo - 1, _stilus_nativus());
        tessera_cursorem_ponere(opus,
            1 + (integer)conspectus->imperium.mensura,
            altitudo - 1);
    }
    alioquin
    {
        _scribe(opus, 0, altitudo - 1, conspectus->status,
            latitudo, _stilus_nativus());
        tessera_cursorem_ponere(opus, -1, -1);
    }
}
