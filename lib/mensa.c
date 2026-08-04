/* mensa.c - status tabulae persistens (vide mensa.h) */

#include "mensa.h"
#include "json.h"
#include "filum.h"
#include "via.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <string.h>

structura MensaContextus {
    Piscina* piscina;
    Volumen* volumen;
};

/* signum deletionis in tabula per-id (valor sentinella) */
interior integer _deletum_nota;

#define PRAEFIXUM_MENSAE "mensa:"
#define PRAEFIXUM_MENSURA 6

/* ==================================================
 * Plica: acta -> status
 * ================================================== */

interior JsonValor*
_statum_valor (Volumen* volumen, Piscina* piscina);

interior JsonValor*
_statum_valor (Volumen* volumen, Piscina* piscina)
{
    Xar*            acta;
    TabulaDispersa* per_id;
    JsonValor*      radix;
    JsonValor*      elementa;
    i32             index;

    acta = volumen_acta_legere(volumen, 0, piscina);
    per_id = tabula_dispersa_creare_chorda(piscina, 64);
    radix = json_objectum_creare(piscina);
    elementa = json_objectum_creare(piscina);
    si (acta == NIHIL || per_id == NIHIL || radix == NIHIL
        || elementa == NIHIL)
    {
        redde NIHIL;
    }

    per (index = 0; index < xar_numerus(acta); index = index + 1)
    {
        VolumenActum* actum = (VolumenActum*)xar_obtinere(acta,
            index);
        JsonResultus  lectum;
        JsonValor*    id_valor;
        chorda        id;
        b32           deletum;

        si (actum->genus.mensura <= PRAEFIXUM_MENSURA
            || memcmp(actum->genus.datum, PRAEFIXUM_MENSAE,
                PRAEFIXUM_MENSURA) != 0)
        {
            perge;   /* actum alienum (non mensae) */
        }
        lectum = json_legere(actum->datum, piscina);
        si (!lectum.successus || !json_est_objectum(lectum.radix))
        {
            perge;   /* datum corruptum - plica tolerans */
        }
        id_valor = json_objectum_capere(lectum.radix, "id");
        si (id_valor == NIHIL || !json_est_chorda(id_valor))
        {
            perge;
        }
        id = json_ad_chorda(id_valor);

        deletum = (actum->genus.mensura
                == PRAEFIXUM_MENSURA + 7
            && memcmp(actum->genus.datum + PRAEFIXUM_MENSURA,
                "deletum", 7) == 0);
        si (deletum)
        {
            tabula_dispersa_inserere(per_id, id,
                (vacuum*)&_deletum_nota);
            perge;
        }

        /* fusio plana: claves datorum (praeter id) in obiectum
         * per-id superscribuntur */
        {
            vacuum*    prius = NIHIL;
            JsonValor* fusum;

            si (!tabula_dispersa_invenire(per_id, id, &prius)
                || prius == (vacuum*)&_deletum_nota)
            {
                fusum = json_objectum_creare(piscina);
                si (fusum == NIHIL)
                {
                    redde NIHIL;
                }
                tabula_dispersa_inserere(per_id, id,
                    (vacuum*)fusum);
            }
            alioquin
            {
                fusum = (JsonValor*)prius;
            }
            {
                JsonObjectumIterator iter =
                    json_objectum_iterator(lectum.radix);
                chorda     clavis;
                JsonValor* valor;

                dum (json_objectum_iterator_proxima(&iter, &clavis,
                    &valor))
                {
                    si (clavis.mensura == 2
                        && memcmp(clavis.datum, "id", 2) == 0)
                    {
                        perge;
                    }
                    json_objectum_ponere(fusum,
                        chorda_ut_cstr(clavis, piscina), valor);
                }
            }
        }
    }

    /* tabulam in obiectum elementorum vertere (deleta omissa) */
    {
        TabulaIterator iter = tabula_dispersa_iterator_initium(
            per_id);
        chorda  clavis;
        vacuum* valor;

        dum (tabula_dispersa_iterator_proximum(&iter, &clavis,
            &valor))
        {
            si (valor == (vacuum*)&_deletum_nota)
            {
                perge;
            }
            json_objectum_ponere(elementa,
                chorda_ut_cstr(clavis, piscina),
                (JsonValor*)valor);
        }
    }
    json_objectum_ponere(radix, "elementa", elementa);
    redde radix;
}

chorda
mensa_statum_fingere (Volumen* volumen, Piscina* piscina)
{
    JsonValor* status = _statum_valor(volumen, piscina);
    chorda     vacua;

    si (status == NIHIL)
    {
        vacua.datum = NIHIL;
        vacua.mensura = ZEPHYRUM;
        redde vacua;
    }
    redde json_scribere(status, piscina);
}

/* ==================================================
 * Tractatores internuntii
 * ================================================== */

interior JsonValor*
_status_tractare (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa);

interior JsonValor*
_status_tractare (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    MensaContextus* ctx = (MensaContextus*)datum;
    JsonValor*      status;

    (vacuum)argumenta;
    status = _statum_valor(ctx->volumen, piscina);
    si (status == NIHIL)
    {
        *culpa = chorda_ex_literis("status fingi non potuit",
            piscina);
        redde NIHIL;
    }
    redde status;
}

interior JsonValor*
_actum_tractare (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa);

interior JsonValor*
_actum_tractare (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    MensaContextus* ctx = (MensaContextus*)datum;
    JsonValor*      genus_valor;
    JsonValor*      datum_valor;
    JsonValor*      fructus;
    chorda          genus;
    chorda          corpus;
    s64             seq;
    ChordaAedificator* aed;

    genus_valor = argumenta == NIHIL ? NIHIL
        : json_objectum_capere(argumenta, "genus");
    datum_valor = argumenta == NIHIL ? NIHIL
        : json_objectum_capere(argumenta, "datum");
    si (genus_valor == NIHIL || !json_est_chorda(genus_valor)
        || datum_valor == NIHIL || !json_est_objectum(datum_valor))
    {
        *culpa = chorda_ex_literis(
            "mensa_actum: {genus, datum} requiruntur", piscina);
        redde NIHIL;
    }
    genus = json_ad_chorda(genus_valor);

    aed = chorda_aedificator_creare(piscina, (memoriae_index)64);
    chorda_aedificator_appendere_literis(aed, PRAEFIXUM_MENSAE);
    chorda_aedificator_appendere_chorda(aed, genus);
    corpus = json_scribere(datum_valor, piscina);
    seq = volumen_actum_appendere(ctx->volumen,
        chorda_ut_cstr(chorda_aedificator_finire(aed), piscina),
        corpus);
    si (seq == 0)
    {
        *culpa = chorda_ex_literis("actum condi non potuit",
            piscina);
        redde NIHIL;
    }
    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "seq",
        json_integer_creare(piscina, seq));
    redde fructus;
}

/* ==================================================
 * Praebere
 * ================================================== */

MensaContextus*
mensa_praebere (Internuntius* internuntius, Piscina* piscina,
    constans character* via_voluminis)
{
    MensaContextus* ctx;

    ctx = (MensaContextus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(MensaContextus));
    si (ctx == NIHIL)
    {
        redde NIHIL;
    }
    ctx->piscina = piscina;

    /* directorium parens creari si abest */
    {
        chorda parens = via_directorium(
            chorda_ex_literis(via_voluminis, piscina), piscina);

        filum_directorium_creare_si_necesse(
            chorda_ut_cstr(parens, piscina));
    }
    si (filum_existit(via_voluminis))
    {
        ctx->volumen = volumen_aperire(piscina, via_voluminis);
    }
    alioquin
    {
        ctx->volumen = volumen_creare(piscina, via_voluminis);
    }
    si (ctx->volumen == NIHIL)
    {
        fprintf(stderr, "mensa: volumen aperiri non potuit: %s\n",
            via_voluminis);
        redde NIHIL;
    }
    (vacuum)internuntius_praebere(internuntius, "mensa_status",
        _status_tractare, ctx);
    (vacuum)internuntius_praebere(internuntius, "mensa_actum",
        _actum_tractare, ctx);
    redde ctx;
}

vacuum
mensa_claudere (MensaContextus* contextus)
{
    si (contextus == NIHIL)
    {
        redde;
    }
    volumen_claudere(contextus->volumen);
}
