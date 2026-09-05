/* briar_nexus.c - Vide briar_nexus.h. */

#include "briar_nexus.h"
#include "briar_arbor.h"
#include "briar_registrum.h"
#include "chorda_aedificator.h"
#include "materia_token.h"
#include "md_arbor.h"
#include <string.h>

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior b32
_titulus_est (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && memcmp(c.datum, literae, (size_t)c.mensura) == ZEPHYRUM);
}

/* '<nomen! attrs>' (cum terminatore) -> '<nomen attrs/>' */
interior chorda
_tagum_rescribere (
                  Piscina* piscina,
    constans MateriaToken* apertum,
                   chorda  titulus)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)256);
               chorda v = apertum->valor;
                  i32 i;
                  i32 f;

    si (aed == NIHIL)
    {
        redde _vacua();
    }
    /* finis: terminator et spatia finalia abscisa */
    f = v.mensura;
    dum (   f > ZEPHYRUM && ((character)v.datum[f - I] == '\n'
        || (character)v.datum[f - I] == '\r'
        || (character)v.datum[f - I] == ' '
        || (character)v.datum[f - I] == '\t'))
    {
        f = f - I;
    }
    /* '<' + nomen */
    i = I + titulus.mensura;
    chorda_aedificator_appendere_literis(aed, "<");
    chorda_aedificator_appendere_chorda(aed, titulus);
    /* '!' et '\' praeteriti */
    dum (   i < f
         && ((character)v.datum[i] == '!'
        || (character)v.datum[i] == '\\'))
    {
        i = i + I;
    }
    /* reliquum sine '>' finali (aut '/>') */
    si (   f >= II && (character)v.datum[f - II] == '/'
        && (character)v.datum[f - I] == '>')
    {
        f = f - II;
    }
    alioquin si (f >= I && (character)v.datum[f - I] == '>')
    {
        f = f - I;
    }
    si (i < f)
    {
        chorda_aedificator_appendere_chorda(aed, chorda_sectio(v, i,
            f));
    }
    chorda_aedificator_appendere_literis(aed, "/>");
    redde chorda_aedificator_finire(aed);
}

interior vacuum
_res_incipere (
            BriarNexusRes* r,
    constans MateriaNodus* pars,
          BriarNexusGenus  genus)
{
    r->pars           = pars;
    r->genus          = genus;
    r->arbor_md       = NIHIL;
    r->elementum      = NIHIL;
    r->titulus        = _vacua();
    r->contentum      = _vacua();
    r->linea_initium  = ZEPHYRUM;
    r->linea_erroris  = ZEPHYRUM;
    r->causa          = _vacua();
}

interior vacuum
_stml_legere_in (
                Piscina* piscina,
    InternamentumChorda* intern,
          BriarNexusRes* r,
                 chorda  textus,
                    i32  linea_basis)
{
    StmlResultus res = stml_legere(textus, piscina, intern);

    si (res.successus && res.elementum_radix != NIHIL)
    {
        r->elementum = res.elementum_radix;
        redde;
    }
    r->elementum  = NIHIL;
    r->causa      = res.error;
    r->linea_erroris  = linea_basis + ((res.linea_erroris > ZEPHYRUM)
        ? res.linea_erroris - I : ZEPHYRUM);
    si (r->causa.mensura == ZEPHYRUM)
    {
        r->causa = chorda_ex_literis("stml: parsura fracta sine nuntio",
            piscina);
    }
}

Xar*
briar_nexus_texere (
                  Piscina* piscina,
    constans MateriaNodus* documentum,
      InternamentumChorda* intern)
{
    Xar* nexus;
    i32  n;
    i32  i;

    si (piscina == NIHIL || documentum == NIHIL)
    {
        redde NIHIL;
    }
    nexus = xar_creare(piscina, (i32)magnitudo(BriarNexusRes));
    si (nexus == NIHIL)
    {
        redde NIHIL;
    }
    n = briar_partes_numerus(documentum);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaNodus* pars = briar_pars(documentum, i);
                BriarNexusRes* r = (BriarNexusRes*)xar_addere(nexus);

        si (r == NIHIL)
        {
            redde NIHIL;
        }
        si (pars->genus == (s32)BRIAR_GENUS_PROSA)
        {
            MateriaToken* t = briar_lexema(pars, (i32)BRIAR_PROSA_TOK);

            _res_incipere(r, pars, BRIAR_NEXUS_MD);
            r->linea_initium  = t->linea;
            r->arbor_md       = md_arbor_parsare(piscina,
                (constans character*)t->valor.datum, t->valor.mensura);
        }
        alioquin si (pars->genus == (s32)BRIAR_GENUS_ELEMENTUM)
        {
            MateriaToken* t = briar_lexema(pars,
                (i32)BRIAR_ELEMENTUM_TOK);

            _res_incipere(r, pars, BRIAR_NEXUS_STML);
            r->titulus        = briar_titulus(pars);
            r->linea_initium  = t->linea;
            _stml_legere_in(piscina, intern, r, t->valor, t->linea);
        }
        alioquin
        {
            MateriaToken* apertum = briar_lexema(pars,
                (i32)BRIAR_REGIO_APERTUM);
            MateriaToken* contentum = briar_lexema(pars,
                (i32)BRIAR_REGIO_CONTENTUM);
                  chorda titulus = briar_titulus(pars);

            _res_incipere(r, pars, BRIAR_NEXUS_REGIO);
            r->titulus = titulus;
            r->contentum = (contentum
                != NIHIL) ? contentum->valor : _vacua();
            r->linea_initium = apertum->linea + I;
            _stml_legere_in(piscina, intern, r,
                _tagum_rescribere(piscina, apertum, titulus),
                apertum->linea);
            /* regio 'md' = prosa explicita: arbor markdown super contentum */
            si (_titulus_est(titulus, "md"))
            {
                r->genus     = BRIAR_NEXUS_MD;
                r->arbor_md  = md_arbor_parsare(piscina,
                    (constans character*)r->contentum.datum,
                    r->contentum.mensura);
            }
        }
    }
    redde nexus;
}

chorda
briar_nexus_attributum (
    constans BriarNexusRes* res,
        constans character* titulus)
{
    chorda* v;

    si (res == NIHIL || res->elementum == NIHIL)
    {
        redde _vacua();
    }
    v = stml_attributum_capere(res->elementum, titulus);
    redde (v != NIHIL) ? *v : _vacua();
}

b32
briar_nexus_attributum_habet (
    constans BriarNexusRes* res,
        constans character* titulus)
{
    si (res == NIHIL || res->elementum == NIHIL)
    {
        redde FALSUM;
    }
    redde stml_attributum_habet(res->elementum, titulus);
}

b32
briar_nexus_titulus_est (
    constans BriarNexusRes* res,
        constans character* literae)
{
    redde (b32)(res != NIHIL && res->titulus.mensura > ZEPHYRUM
        && _titulus_est(res->titulus, literae));
}
