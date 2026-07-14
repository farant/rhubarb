/* tabellarius.c - Cursor JSON-RPC (vide tabellarius.h) */

#include "tabellarius.h"
#include "chorda_aedificator.h"

#include <stdlib.h>
#include <string.h>

/* ==================================================
 * auxilia
 * ================================================== */

interior character
_minusculum (character c)
{
    si (c >= 'A' && c <= 'Z')
    {
        redde (character)(c + ('a' - 'A'));
    }
    redde c;
}

/* praefixum sine casu; VERUM si linea praefixo incipit */
interior b32
_praefixum_sine_casu (constans character* linea,
    constans character* praefixum)
{
    dum (*praefixum != '\0')
    {
        si (*linea == '\0'
            || _minusculum(*linea) != _minusculum(*praefixum))
        {
            redde FALSUM;
        }
        linea++;
        praefixum++;
    }
    redde VERUM;
}

interior chorda
_chorda_vacua (vacuum)
{
    chorda c;

    c.mensura = ZEPHYRUM;
    c.datum = NIHIL;
    redde c;
}

/* ==================================================
 * EPISTULA (framing)
 * ================================================== */

chorda
tabellarius_epistulam_legere (FILE* fluxus, Piscina* piscina,
    b32* fluxus_finitus)
{
    longus mensura_corporis = -1L;

    *fluxus_finitus = FALSUM;
    si (fluxus == NIHIL || piscina == NIHIL)
    {
        *fluxus_finitus = VERUM;
        redde _chorda_vacua();
    }

    /* lineae capitis usque ad lineam vacuam */
    per (;;)
    {
        character linea[CCLVI];
        s32 plenitudo = ZEPHYRUM;
        int c;

        dum ((c = fgetc(fluxus)) != EOF && c != '\n')
        {
            si (plenitudo < (s32)(magnitudo(linea) - I))
            {
                linea[plenitudo] = (character)c;
                plenitudo++;
            }
            alioquin
            {
                /* linea capitis nimis longa - fluxus corruptus */
                *fluxus_finitus = VERUM;
                redde _chorda_vacua();
            }
        }
        si (c == EOF)
        {
            *fluxus_finitus = VERUM;
            redde _chorda_vacua();
        }
        si (plenitudo > ZEPHYRUM && linea[plenitudo - I] == '\r')
        {
            plenitudo--;
        }
        linea[plenitudo] = '\0';
        si (plenitudo == ZEPHYRUM)
        {
            frange;   /* caput finitum */
        }
        si (_praefixum_sine_casu(linea, "content-length:"))
        {
            mensura_corporis = strtol(linea + XV, NIHIL, X);
        }
        /* capita cetera (Content-Type...) ignorata */
    }

    si (mensura_corporis < 0L
        || (insignatus longus)mensura_corporis
            > TABELLARIUS_CORPUS_MAXIMUM)
    {
        *fluxus_finitus = VERUM;
        redde _chorda_vacua();
    }

    {
        chorda corpus;
        character* datum = (character*)piscina_allocare(piscina,
            (memoriae_index)(mensura_corporis + 1L));

        si (datum == NIHIL)
        {
            *fluxus_finitus = VERUM;
            redde _chorda_vacua();
        }
        si (mensura_corporis > 0L
            && fread(datum, I, (memoriae_index)mensura_corporis,
                   fluxus)
                != (memoriae_index)mensura_corporis)
        {
            *fluxus_finitus = VERUM;
            redde _chorda_vacua();
        }
        datum[mensura_corporis] = '\0';
        corpus.mensura = (i32)mensura_corporis;
        corpus.datum = (i8*)datum;
        redde corpus;
    }
}

vacuum
tabellarius_epistulam_scribere (FILE* fluxus, chorda corpus)
{
    si (fluxus == NIHIL)
    {
        redde;
    }
    fprintf(fluxus, "Content-Length: %lu\r\n\r\n",
        (insignatus longus)corpus.mensura);
    si (corpus.mensura > ZEPHYRUM && corpus.datum != NIHIL)
    {
        fwrite(corpus.datum, I, (memoriae_index)corpus.mensura,
            fluxus);
    }
    fflush(fluxus);
}

chorda
tabellarius_lineam_legere (FILE* fluxus, Piscina* piscina,
    b32* fluxus_finitus)
{
    *fluxus_finitus = FALSUM;
    si (fluxus == NIHIL || piscina == NIHIL)
    {
        *fluxus_finitus = VERUM;
        redde _chorda_vacua();
    }
    per (;;)
    {
        ChordaAedificator* aed = chorda_aedificator_creare(piscina,
            CCLVI);
        int c;
        b32 aliquid = FALSUM;

        si (aed == NIHIL)
        {
            *fluxus_finitus = VERUM;
            redde _chorda_vacua();
        }
        dum ((c = fgetc(fluxus)) != EOF && c != '\n')
        {
            chorda_aedificator_appendere_character(aed,
                (character)c);
            aliquid = VERUM;
        }
        si (c == EOF)
        {
            /* EOF ante '\n': etiam cum reliquiis = finitus (nuntius
             * truncatus fide indignus) */
            *fluxus_finitus = VERUM;
            redde _chorda_vacua();
        }
        {
            chorda linea = chorda_aedificator_finire(aed);

            si (aliquid && linea.mensura > ZEPHYRUM
                && linea.datum[linea.mensura - I] == (i8)'\r')
            {
                linea.mensura--;
            }
            si (linea.mensura == ZEPHYRUM)
            {
                perge;   /* linea vacua praetermissa */
            }
            si ((insignatus longus)linea.mensura
                > TABELLARIUS_CORPUS_MAXIMUM)
            {
                *fluxus_finitus = VERUM;
                redde _chorda_vacua();
            }
            redde linea;
        }
    }
}

b32
tabellarius_lineam_scribere (FILE* fluxus, chorda corpus)
{
    i32 i;

    si (fluxus == NIHIL || corpus.datum == NIHIL
        || corpus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* lex serializationis: '\n' crudum canalem corrumperet */
    per (i = ZEPHYRUM; i < corpus.mensura; i++)
    {
        si (corpus.datum[i] == (i8)'\n')
        {
            fprintf(stderr, "tabellarius: linea '\\n' crudum"
                " continet - scriptura recusata\n");
            redde FALSUM;
        }
    }
    fwrite(corpus.datum, I, (memoriae_index)corpus.mensura, fluxus);
    fputc('\n', fluxus);
    fflush(fluxus);
    redde VERUM;
}

/* ==================================================
 * NUNTIUS (envelope)
 * ================================================== */

TabellariusNuntius
tabellarius_nuntium_legere (chorda corpus, Piscina* piscina)
{
    TabellariusNuntius n;
    JsonResultus res;

    n.genus = TABELLARIUS_PRAVUM;
    n.radix = NIHIL;
    n.id = NIHIL;
    n.methodus = _chorda_vacua();
    n.params = NIHIL;

    si (piscina == NIHIL || corpus.datum == NIHIL)
    {
        redde n;
    }
    res = json_legere(corpus, piscina);
    si (!res.successus || res.radix == NIHIL)
    {
        redde n;
    }
    n.radix = res.radix;
    si (!json_est_objectum(res.radix))
    {
        redde n;   /* fasces (tabulata) quoque prava - LSP numquam */
    }

    {
        JsonValor* methodus_v = json_objectum_capere(res.radix,
            "method");
        JsonValor* id_v = json_objectum_capere(res.radix, "id");

        n.id = id_v;
        n.params = json_objectum_capere(res.radix, "params");
        si (methodus_v != NIHIL && json_est_chorda(methodus_v))
        {
            n.methodus = json_ad_chorda(methodus_v);
            n.genus = (id_v != NIHIL) ? TABELLARIUS_PETITIO
                                      : TABELLARIUS_NUNTIATIO;
        }
        alioquin si (id_v != NIHIL
            && (json_objectum_habet(res.radix, "result")
                || json_objectum_habet(res.radix, "error")))
        {
            n.genus = TABELLARIUS_RESPONSUM;
        }
    }
    redde n;
}

/* ==================================================
 * FABRI
 * ================================================== */

chorda
tabellarius_responsum (Piscina* piscina, JsonValor* id,
    JsonValor* resultatum)
{
    JsonValor* obj;

    si (piscina == NIHIL)
    {
        redde _chorda_vacua();
    }
    obj = json_objectum_creare(piscina);
    si (obj == NIHIL)
    {
        redde _chorda_vacua();
    }
    json_objectum_ponere(obj, "jsonrpc",
        json_chorda_creare_literis(piscina, "2.0"));
    json_objectum_ponere(obj, "id",
        id != NIHIL ? id : json_nullum_creare(piscina));
    json_objectum_ponere(obj, "result",
        resultatum != NIHIL ? resultatum
                            : json_nullum_creare(piscina));
    redde json_scribere(obj, piscina);
}

chorda
tabellarius_errorem (Piscina* piscina, JsonValor* id, longus codex,
    constans character* nuntius)
{
    JsonValor* obj;
    JsonValor* error_v;

    si (piscina == NIHIL || nuntius == NIHIL)
    {
        redde _chorda_vacua();
    }
    obj = json_objectum_creare(piscina);
    error_v = json_objectum_creare(piscina);
    si (obj == NIHIL || error_v == NIHIL)
    {
        redde _chorda_vacua();
    }
    json_objectum_ponere(error_v, "code",
        json_integer_creare(piscina, (s64)codex));
    json_objectum_ponere(error_v, "message",
        json_chorda_creare_literis(piscina, nuntius));
    json_objectum_ponere(obj, "jsonrpc",
        json_chorda_creare_literis(piscina, "2.0"));
    json_objectum_ponere(obj, "id",
        id != NIHIL ? id : json_nullum_creare(piscina));
    json_objectum_ponere(obj, "error", error_v);
    redde json_scribere(obj, piscina);
}

chorda
tabellarius_nuntiationem (Piscina* piscina,
    constans character* methodus, JsonValor* params)
{
    JsonValor* obj;

    si (piscina == NIHIL || methodus == NIHIL)
    {
        redde _chorda_vacua();
    }
    obj = json_objectum_creare(piscina);
    si (obj == NIHIL)
    {
        redde _chorda_vacua();
    }
    json_objectum_ponere(obj, "jsonrpc",
        json_chorda_creare_literis(piscina, "2.0"));
    json_objectum_ponere(obj, "method",
        json_chorda_creare_literis(piscina, methodus));
    si (params != NIHIL)
    {
        json_objectum_ponere(obj, "params", params);
    }
    redde json_scribere(obj, piscina);
}
