/* briar_silva.c - Vide briar_silva.h. */

#include "briar_silva.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>

interior b32
_suffixum_est (
                chorda  c,
    constans character* suffixum)
{
    i32 m = (i32)strlen(suffixum);

    redde (b32)(c.mensura >= m
        && memcmp(c.datum + (c.mensura - m), suffixum, (size_t)m)
            == ZEPHYRUM);
}

/* nodus ERROR primus (profunditate prima) in valore: linea silvae aut
 * 0. Ambulatio per VALORES (NODUS aut LISTA), non per
 * silva_nodus_liberi: radix commissionis parsurae fractae LISTA esse
 * potest, et liberi eam ut nodum legeret. */
interior i32
_errorem_quaerere (
    SilvaValor valor,
       integer fons_index)
{
    si (valor.genus == SILVA_VALOR_NODUS)
    {
         constans SilvaNodus* nodus = valor.datum.nodus;
          insignatus integer  k;

        si (nodus == NIHIL)
        {
            redde ZEPHYRUM;
        }
        si (nodus->genus == (integer)SILVA_C89_GENUS_ERROR)
        {
            insignatus integer la = ZEPHYRUM;
            insignatus integer ca = ZEPHYRUM;
            insignatus integer lb = ZEPHYRUM;
            insignatus integer cb = ZEPHYRUM;

            silva_nodus_extensionem_lineis(nodus, fons_index, &la, &ca,
                &lb, &cb);
            redde (i32)la;
        }
        per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
        {
            i32 linea = _errorem_quaerere(nodus->loci[k], fons_index);

            si (linea > ZEPHYRUM)
            {
                redde linea;
            }
        }
        redde ZEPHYRUM;
    }
    si (valor.genus == SILVA_VALOR_LISTA)
    {
        insignatus integer n = silva_valor_lista_numerus(valor);
        insignatus integer k;

        per (k = ZEPHYRUM; k < n; k++)
        {
            SilvaValor* e = silva_valor_lista_obtinere(valor, k);
                   i32  linea = (e != NIHIL)
                       ? _errorem_quaerere(*e, fons_index) : ZEPHYRUM;

            si (linea > ZEPHYRUM)
            {
                redde linea;
            }
        }
    }
    redde ZEPHYRUM;
}

interior b32
_regionem_parsare (
               Piscina* piscina,
         BriarNexusRes* r,
    constans SilexFons* fons)
{
    ChordaAedificator* aed;
                  Xar* clausura;
        SilvaExpansio* exp;
           BriarSilva* bs;
                  i32  k;

    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(r->contentum.mensura + 256));
    si (aed == NIHIL)
    {
        redde FALSUM;
    }
    chorda_aedificator_appendere_literis(aed,
        "#include \"latina.h\"\n");
    r->praeludium = I;
    si (briar_nexus_attributum_habet(r, "methodus"))
    {
        chorda_aedificator_appendere_literis(aed,
            "#include \"internuntius.h\"\n"
            "hic_manens InternuntiusTractator"
            " briar_tractator_exemplar;\n");
        r->praeludium = III;
    }
    r->praeludium_octeti = (i32)chorda_aedificator_longitudo(aed);
    chorda_aedificator_appendere_chorda(aed, r->contentum);
    r->textus_silvae = chorda_aedificator_finire(aed);

    clausura = silex_clausuram_e_contentis(piscina, fons,
        &r->textus_silvae, I);
    si (clausura == NIHIL)
    {
        r->causa = chorda_ex_literis(
            "clausura capitum fracta (fons silicis)", piscina);
        r->linea_erroris = r->linea_initium;
        redde VERUM;
    }
    bs = (BriarSilva*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(BriarSilva));
    si (bs == NIHIL)
    {
        redde FALSUM;
    }
    bs->parsura    = NIHIL;
    bs->semantica  = NIHIL;
    bs->piscina   = silva_piscina_generare_dynamicum("briar_silva",
        (size_t)8388608);
    si (bs->piscina == NIHIL)
    {
        redde FALSUM;
    }
    r->silva  = bs;
    exp       = silva_expansio_creare(bs->piscina);
    per (k = ZEPHYRUM; k < xar_numerus(clausura); k++)
    {
        constans SilexRes* res = (constans SilexRes*)xar_obtinere(
            clausura, k);

        si (!_suffixum_est(res->via, ".h"))
        {
            perge;
        }
        silva_includendum_praebere(exp, chorda_ut_cstr(res->via,
            piscina),
            (constans character*)res->contentum.datum,
            (insignatus integer)res->contentum.mensura);
    }
    bs->parsura = silva_parsare_cum_expansione(bs->piscina, exp,
        "regio.c", (constans character*)r->textus_silvae.datum,
        (insignatus integer)r->textus_silvae.mensura,
        &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
    si (bs->parsura == NIHIL || bs->parsura->commissio == NIHIL)
    {
        bs->parsura = NIHIL;
        r->causa = chorda_ex_literis("silva: parsura fracta",
            piscina);
        r->linea_erroris = r->linea_initium;
        redde VERUM;
    }
    si (bs->parsura->numerus_errorum > ZEPHYRUM)
    {
        i32 linea = _errorem_quaerere(bs->parsura->commissio->radix,
            bs->parsura->fons_princeps);
        character b[96];

        r->linea_erroris = (linea > ZEPHYRUM)
            ? briar_nexus_linea_silvae(r, linea) : r->linea_initium;
        sprintf(b, "regio C: parsura fracta (%u errores)",
            bs->parsura->numerus_errorum);
        r->causa = chorda_ex_literis(b, piscina);
    }
    bs->semantica = silva_c89_semantica_analysare(bs->piscina,
        bs->parsura);
    redde VERUM;
}

s32
briar_silvam_texere (
                Piscina* piscina,
                    Xar* nexus,
     constans SilexFons* fons)
{
    i32 i;
    s32 numerus = ZEPHYRUM;

    si (piscina == NIHIL || nexus == NIHIL || fons == NIHIL)
    {
        redde -I;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

        si (   r->genus != BRIAR_NEXUS_REGIO
            || !briar_nexus_titulus_est(r, "c"))
        {
            perge;
        }
        si (!_regionem_parsare(piscina, r, fons))
        {
            redde -I;
        }
        numerus = numerus + I;
    }
    redde numerus;
}

vacuum
briar_silvam_solvere (
    Xar* nexus)
{
    i32 i;

    si (nexus == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

        si (r->silva != NIHIL && r->silva->piscina != NIHIL)
        {
            silva_piscina_destruere(r->silva->piscina);
        }
        r->silva = NIHIL;
    }
}
