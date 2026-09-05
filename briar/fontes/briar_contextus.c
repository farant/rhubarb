/* briar_contextus.c - Vide briar_contextus.h. */

#include "briar_contextus.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

nomen structura {
           Piscina* piscina;
    TabulaDispersa* per_id;      /* id -> BriarFragmentum* */
               Xar* fragmenta;   /* BriarFragmentum */
     BriarNexusRes* radix;       /* quae contexitur (recusationes) */
               Xar* acervus;     /* chorda: ids in cursu (circulus) */
} BriarContextio;


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior b32
_albus (
    character c)
{
    redde (b32)(c == ' ' || c == '\t');
}

interior vacuum
_recusare (
         BriarNexusRes* r,
               Piscina* piscina,
    constans character* causa,
                   i32  linea)
{
    r->causa          = chorda_ex_literis(causa, piscina);
    r->linea_erroris  = linea;
}

interior b32
_regio_c_est (
    constans BriarNexusRes* r)
{
    redde (b32)(r->genus == BRIAR_NEXUS_REGIO
        && briar_nexus_titulus_est(r, "c"));
}

s32
briar_contextus_lineam_legere (
    chorda  linea,
    chorda* id,
    chorda* praefixum)
{
    i32 p = ZEPHYRUM;
    i32 initium;

    dum (p < linea.mensura && _albus((character)linea.datum[p]))
    {
        p = p + I;
    }
    si (   p + III > linea.mensura
        || memcmp(linea.datum + p, "<<#", (size_t)III) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    *praefixum  = chorda_sectio(linea, ZEPHYRUM, p);
    p           = p + III;
    initium     = p;
    dum (   p < linea.mensura && (character)linea.datum[p] != '>'
         && !_albus((character)linea.datum[p]))
    {
        p = p + I;
    }
    si (   p == initium || p + II > linea.mensura
        || (character)linea.datum[p] != '>'
        || (character)linea.datum[p + I] != '>')
    {
        redde -I;
    }
    *id  = chorda_sectio(linea, initium, p);
    p    = p + II;
    dum (   p < linea.mensura
         && (   _albus((character)linea.datum[p])
             || (character)linea.datum[p] == '\r'))
    {
        p = p + I;
    }
    redde p == linea.mensura ? I : -I;
}


/* ==================================================
 * Classificatio
 * ================================================== */

interior b32
_id_validum (
    chorda id)
{
    i32 i;

    si (id.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < id.mensura; i++)
    {
        character c = (character)id.datum[i];

        si (_albus(c) || c == '>' || c == '\n' || c == '\r')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_classificare (
    BriarContextio* ctx,
               Xar* nexus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);
               chorda  id;
                  i32  linea_tagi;
               vacuum* prior = NIHIL;
            character  b[160];

        si (!_regio_c_est(r))
        {
            perge;
        }
        r->contextus       = r->contentum;
        r->lineae          = NIHIL;
        r->est_fragmentum  = FALSUM;
        si (!briar_nexus_attributum_habet(r, "id"))
        {
            perge;
        }
        r->est_fragmentum  = VERUM;
        id                 = briar_nexus_attributum(r, "id");
        linea_tagi         = r->linea_initium - I;
        si (!_id_validum(id))
        {
            sprintf(b, "fragmentum sine id valido (linea %d)",
                (integer)linea_tagi);
            _recusare(r, ctx->piscina, b, linea_tagi);
            perge;
        }
        si (   briar_nexus_attributum_habet(r, "methodus")
            || briar_nexus_attributum_habet(r, "munus"))
        {
            sprintf(b,
                "fragmentum '#%.*s' (linea %d): methodus/munus in"
                " fragmento - fragmentum radix non est",
                (integer)id.mensura, (constans character*)id.datum,
                (integer)linea_tagi);
            _recusare(r, ctx->piscina, b, linea_tagi);
            perge;
        }
        si (tabula_dispersa_invenire(ctx->per_id, id, &prior))
        {
            constans BriarFragmentum* primum =
                (constans BriarFragmentum*)prior;

            sprintf(b, "fragmentum '#%.*s' iteratum (prima linea %d)",
                (integer)id.mensura, (constans character*)id.datum,
                (integer)(primum->regio->linea_initium - I));
            _recusare(r, ctx->piscina, b, linea_tagi);
            perge;
        }
        {
            BriarFragmentum* f = (BriarFragmentum*)xar_addere(
                ctx->fragmenta);

            si (f == NIHIL)
            {
                perge;
            }
            f->id     = id;
            f->regio  = r;
            f->usus   = xar_creare(ctx->piscina, (i32)magnitudo(i32));
            tabula_dispersa_inserere(ctx->per_id, id, (vacuum*)f);
        }
    }
}


/* ==================================================
 * Textura
 * ================================================== */

interior vacuum
_usum_notare (
    BriarFragmentum* f,
                i32  linea)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(f->usus); k++)
    {
        si (*(i32*)xar_obtinere(f->usus, k) == linea)
        {
            redde;   /* fragmentum in fragmento a radicibus pluribus */
        }
    }
    {
        i32* cella = (i32*)xar_addere(f->usus);

        si (cella != NIHIL)
        {
            *cella = linea;
        }
    }
}

/* '#a -> #b -> #a' ab occurrentia prima id in acervo */
interior constans character*
_circulum_scribere (
    BriarContextio* ctx,
            chorda  id)
{
    ChordaAedificator* a = chorda_aedificator_creare(ctx->piscina,
        (memoriae_index)128);
    i32 k;
    b32 intra = FALSUM;

    per (k = ZEPHYRUM; k < xar_numerus(ctx->acervus); k++)
    {
        chorda e = *(chorda*)xar_obtinere(ctx->acervus, k);

        si (!intra && chorda_aequalis(e, id))
        {
            intra = VERUM;
        }
        si (intra)
        {
            chorda_aedificator_appendere_literis(a, "#");
            chorda_aedificator_appendere_chorda(a, e);
            chorda_aedificator_appendere_literis(a, " -> ");
        }
    }
    chorda_aedificator_appendere_literis(a, "#");
    chorda_aedificator_appendere_chorda(a, id);
    redde chorda_ut_cstr(chorda_aedificator_finire(a), ctx->piscina);
}

interior b32
_in_acervo (
    BriarContextio* ctx,
            chorda  id)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(ctx->acervus); k++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(ctx->acervus, k),
            id))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_lineam_habet_signum (
    chorda linea)
{
    i32 i;

    per (i = ZEPHYRUM; i < linea.mensura; i++)
    {
        character c = (character)linea.datum[i];

        si (!_albus(c) && c != '\r')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* fons: regio cuius contentum lineatim contexitur (radix aut
 * fragmentum); praefixum = album accumulatum; FALSUM = recusatio (in
 * ctx->radix posita) */
interior b32
_regionem_contexere (
       BriarContextio* ctx,
        BriarNexusRes* fons,
               chorda  praefixum,
    ChordaAedificator* aed,
                  Xar* lineae)
{
    chorda c = fons->contentum;
       i32 i = ZEPHYRUM;
       i32 k = ZEPHYRUM;

    dum (i < c.mensura)
    {
           i32 f = i;
        chorda linea;
        chorda id;
        chorda praefixum_lineae;
           s32 genus;
           i32 linea_thistle = fons->linea_initium + k;
     character b[192];

        dum (f < c.mensura && (character)c.datum[f] != '\n')
        {
            f = f + I;
        }
        linea  = chorda_sectio(c, i, f);
        genus  = briar_contextus_lineam_legere(linea, &id,
            &praefixum_lineae);
        si (genus < ZEPHYRUM)
        {
            sprintf(b,
                "transclusio malformata (linea %d): '<<#x>>' sola"
                " in linea sua exspectata", (integer)linea_thistle);
            _recusare(ctx->radix, ctx->piscina, b, linea_thistle);
            redde FALSUM;
        }
        si (genus == I)
        {
                     vacuum* v = NIHIL;
            BriarFragmentum* frag;
                     chorda  praefixum_novum;
                     chorda* cella;

            si (!tabula_dispersa_invenire(ctx->per_id, id, &v))
            {
                sprintf(b, "transclusio '#%.*s' (linea %d): fragmentum"
                    " non definitum", (integer)id.mensura,
                    (constans character*)id.datum,
                    (integer)linea_thistle);
                _recusare(ctx->radix, ctx->piscina, b, linea_thistle);
                redde FALSUM;
            }
            frag = (BriarFragmentum*)v;
            si (_in_acervo(ctx, id))
            {
                sprintf(b,
                    "transclusio '#%.*s' (linea %d): circulus %s",
                    (integer)id.mensura, (constans character*)id.datum,
                    (integer)linea_thistle, _circulum_scribere(ctx,
                    id));
                _recusare(ctx->radix, ctx->piscina, b, linea_thistle);
                redde FALSUM;
            }
            _usum_notare(frag, linea_thistle);
            cella = (chorda*)xar_addere(ctx->acervus);
            si (cella == NIHIL)
            {
                redde FALSUM;
            }
            *cella           = id;
            praefixum_novum  = chorda_concatenare(praefixum,
                praefixum_lineae, ctx->piscina);
            si (!_regionem_contexere(ctx, frag->regio, praefixum_novum,
                aed, lineae))
            {
                redde FALSUM;
            }
            xar_removere_ultimum(ctx->acervus);
        }
        alioquin
        {
            i32* cella;

            si (_lineam_habet_signum(linea))
            {
                chorda_aedificator_appendere_chorda(aed, praefixum);
            }
            chorda_aedificator_appendere_chorda(aed, linea);
            chorda_aedificator_appendere_literis(aed, "\n");
            cella = (i32*)xar_addere(lineae);
            si (cella == NIHIL)
            {
                redde FALSUM;
            }
            *cella = linea_thistle;
        }
        i = f + I;
        k = k + I;
    }
    redde VERUM;
}

s32
briar_contexere (
    Piscina*  piscina,
        Xar*  nexus,
        Xar** fragmenta)
{
    BriarContextio ctx;
               i32 i;
               s32 n = ZEPHYRUM;

    si (piscina == NIHIL || nexus == NIHIL)
    {
        redde -I;
    }
    ctx.piscina  = piscina;
    ctx.per_id   = tabula_dispersa_creare_chorda(piscina, 64);
    ctx.fragmenta = xar_creare(piscina,
        (i32)magnitudo(BriarFragmentum));
    ctx.acervus  = xar_creare(piscina, (i32)magnitudo(chorda));
    ctx.radix    = NIHIL;
    si (   ctx.per_id  == NIHIL || ctx.fragmenta == NIHIL
        || ctx.acervus == NIHIL)
    {
        redde -I;
    }
    _classificare(&ctx, nexus);
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
            BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);
        ChordaAedificator* aed;
                      Xar* lineae;

        si (!_regio_c_est(r) || r->est_fragmentum)
        {
            perge;
        }
        ctx.radix = r;
        xar_vacare(ctx.acervus);
        aed = chorda_aedificator_creare(piscina,
            (memoriae_index)(r->contentum.mensura + 256));
        lineae = xar_creare(piscina, (i32)magnitudo(i32));
        si (aed == NIHIL || lineae == NIHIL)
        {
            redde -I;
        }
        si (_regionem_contexere(&ctx, r, _vacua(), aed, lineae))
        {
            r->contextus  = chorda_aedificator_finire(aed);
            r->lineae     = lineae;
            n             = n + I;
        }
    }
    si (fragmenta != NIHIL)
    {
        *fragmenta = ctx.fragmenta;
    }
    redde n;
}
