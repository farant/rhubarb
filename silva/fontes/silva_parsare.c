/* silva_parsare.c - Implementatio gubernatoris */

#include "silva_parsare.h"
#include "silva_lexema.h"
#include <stdio.h>
#include <string.h>

/* Genus ERROR + locus lexematum + numerus locorum ex registro per
 * nomen */
interior b32
_formam_erroris_invenire (
    constans SilvaRegistrumCoctum* tabularium,
    s32*                           genus_out,
    i32*                           locus_out,
    i32*                           numerus_locorum_out)
{
    i32 i;

    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans SilvaTabGenus* genus = &tabularium->genera[i];

        si (genus->titulus != NIHIL
            && strcmp(genus->titulus, "error") == ZEPHYRUM)
        {
            i32 k;

            per (k = ZEPHYRUM; k < genus->loci_numerus; k++)
            {
                constans SilvaTabLocus* locus =
                    &tabularium->loci[genus->loci_offset + k];

                si (locus->titulus != NIHIL
                    && strcmp(locus->titulus, "tokens") == ZEPHYRUM)
                {
                    *genus_out = (s32)i;
                    *locus_out = k;
                    *numerus_locorum_out = genus->loci_numerus;
                    redde VERUM;
                }
            }
        }
    }
    fprintf(stderr,
        "silva_parsare: forma erroris in registro abest\n");
    redde FALSUM;
}

/* Nodus ERROR ex lexematibus segmenti [initium, finis) */
interior SilvaValor
_nodum_erroris_facere (
    Piscina*      piscina,
    constans Xar* lexemata,
    i32           initium,
    i32           finis,
    s32           genus_erroris,
    i32           locus_lexematum,
    i32           numerus_locorum)
{
    SilvaNodus* nodus;
    i32 i;

    nodus = silva_nodus_creare(piscina, genus_erroris, numerus_locorum);
    si (nodus == NIHIL)
    {
        redde silva_valor_nihil();
    }
    per (i = initium; i < finis; i++)
    {
        SilvaToken** ref = (SilvaToken**)xar_obtinere(lexemata, i);

        si (ref != NIHIL && *ref != NIHIL)
        {
            silva_nodus_appendere(piscina, nodus, locus_lexematum,
                silva_valor_token(*ref), SILVA_LOCUS_LISTA_TOKEN);
        }
    }
    redde silva_valor_nodus(nodus);
}

SilvaParsura*
silva_lexemata_parsare (
    Piscina*                  piscina,
    constans Xar*             lexemata,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   contextus)
{
    SilvaParsura* parsura;
    SilvaGLR*     glr;
    SilvaToken*   lexema_eof = NIHIL;
    SilvaValor    radix;
    s32           genus_erroris = -I;
    i32           locus_lexematum = ZEPHYRUM;
    i32           numerus_locorum_erroris = ZEPHYRUM;
    i32           numerus;
    i32           initium;
    i32           i;
    i32           profunditas;

    si (piscina == NIHIL || lexemata == NIHIL || grammatica == NIHIL)
    {
        redde NIHIL;
    }
    si (!_formam_erroris_invenire(grammatica->tabularium,
            &genus_erroris, &locus_lexematum,
            &numerus_locorum_erroris))
    {
        redde NIHIL;
    }
    glr = silva_glr_creare(piscina, grammatica->tabula,
        grammatica->constructor, grammatica->fabrica);
    si (glr == NIHIL)
    {
        redde NIHIL;
    }

    parsura = (SilvaParsura*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaParsura));
    si (parsura == NIHIL)
    {
        redde NIHIL;
    }
    parsura->successus = FALSUM;
    parsura->commissio = NIHIL;
    parsura->lexema_finis = NIHIL;
    parsura->lexemata = NIHIL;
    parsura->expansio = NIHIL;
    parsura->directivae = NIHIL;
    parsura->fons_princeps = -I;
    parsura->numerus_errorum = ZEPHYRUM;
    parsura->numerus_segmentorum = ZEPHYRUM;
    parsura->fusiones = ZEPHYRUM;
    parsura->transmutationes = ZEPHYRUM;
    parsura->transmutationes_negatae = ZEPHYRUM;
    parsura->eventa_marginis_novi = ZEPHYRUM;
    parsura->frons_maxima = ZEPHYRUM;

    numerus = xar_numerus(lexemata);

    /* Lexema EOF (ultimum - forma silva_lexare) */
    si (numerus > ZEPHYRUM)
    {
        SilvaToken** ref = (SilvaToken**)xar_obtinere(lexemata,
            numerus - I);

        si (ref != NIHIL && *ref != NIHIL
            && (*ref)->genus == SILVA_LEX_EOF)
        {
            lexema_eof = *ref;
        }
    }
    parsura->lexema_finis = lexema_eof;

    radix = silva_valor_lista_nova(piscina);

    /* CIRCUITUS SECANS: segmentum = lexemata usque ad SEMICOLON in
     * profunditate 0 (inclusum); quodque + EOF independenter
     * parsatur. Fractura -> nodus ERROR (recuperatio per-rem). */
    initium = ZEPHYRUM;
    profunditas = ZEPHYRUM;
    i = ZEPHYRUM;
    dum (i < numerus)
    {
        SilvaToken** ref = (SilvaToken**)xar_obtinere(lexemata, i);
        SilvaToken*  lexema = (ref != NIHIL) ? *ref : NIHIL;
        b32 est_finis_segmenti = FALSUM;
        b32 est_eof = FALSUM;

        si (lexema == NIHIL)
        {
            i++;
            perge;
        }
        si (lexema->genus == SILVA_LEX_EOF)
        {
            est_eof = VERUM;
            est_finis_segmenti = (i > initium) ? VERUM : FALSUM;
        }
        alioquin si (lexema->genus == SILVA_LEX_PAREN_APERTA)
        {
            profunditas++;
        }
        alioquin si (lexema->genus == SILVA_LEX_PAREN_CLAUSA)
        {
            si (profunditas > ZEPHYRUM)
            {
                profunditas--;
            }
        }
        alioquin si (lexema->genus == SILVA_LEX_SEMICOLON
            && profunditas == ZEPHYRUM)
        {
            est_finis_segmenti = VERUM;
        }

        si (est_finis_segmenti)
        {
            i32 finis = est_eof ? i : (i + I);
            Xar* segmentum = xar_creare(piscina,
                (i32)magnitudo(SilvaToken*));
            i32 k;
            SilvaGLRFructus fructus;

            per (k = initium; k < finis; k++)
            {
                SilvaToken** fons_ref =
                    (SilvaToken**)xar_obtinere(lexemata, k);
                SilvaToken** slot =
                    (SilvaToken**)xar_addere(segmentum);

                si (slot != NIHIL && fons_ref != NIHIL)
                {
                    *slot = *fons_ref;
                }
            }
            si (lexema_eof != NIHIL)
            {
                SilvaToken** slot =
                    (SilvaToken**)xar_addere(segmentum);

                si (slot != NIHIL)
                {
                    *slot = lexema_eof;
                }
            }

            fructus = silva_glr_parsare(glr, segmentum, piscina);
            parsura->numerus_segmentorum++;
            parsura->fusiones += fructus.fusiones;
            parsura->transmutationes += fructus.transmutationes;
            parsura->transmutationes_negatae +=
                fructus.transmutationes_negatae;
            parsura->eventa_marginis_novi +=
                fructus.eventa_marginis_novi;
            si (fructus.frons_maxima > parsura->frons_maxima)
            {
                parsura->frons_maxima = fructus.frons_maxima;
            }

            si (fructus.successus
                && fructus.valor.genus == SILVA_VALOR_LISTA)
            {
                i32 n = silva_valor_lista_numerus(fructus.valor);
                i32 e;

                per (e = ZEPHYRUM; e < n; e++)
                {
                    SilvaValor* elem = silva_valor_lista_obtinere(
                        fructus.valor, e);

                    si (elem != NIHIL)
                    {
                        radix = silva_valor_lista_appendere(piscina,
                            radix, *elem);
                    }
                }
            }
            alioquin
            {
                /* Recuperatio: nodus ERROR lexemata segmenti fert */
                SilvaValor nodus_erroris = _nodum_erroris_facere(
                    piscina, lexemata, initium, finis,
                    genus_erroris, locus_lexematum,
                    numerus_locorum_erroris);

                si (nodus_erroris.genus == SILVA_VALOR_NODUS)
                {
                    radix = silva_valor_lista_appendere(piscina,
                        radix, nodus_erroris);
                    parsura->numerus_errorum++;
                }
            }
            initium = est_eof ? i : (i + I);
        }

        si (est_eof)
        {
            frange;
        }
        i++;
    }

    /* Commissio: pater + normalizatio + resolutio (collapse+diarium) */
    parsura->commissio = silva_committere(piscina, radix,
        grammatica->tabularium, oraculum, resolutor, contextus);
    parsura->successus = (parsura->commissio != NIHIL) ? VERUM : FALSUM;
    redde parsura;
}

SilvaParsura*
silva_parsare_cum_expansione (
    Piscina*                  piscina,
    SilvaExpansio*            expansio,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   contextus)
{
    s32            fons_index;
    Xar*           lexemata;
    Xar*           reliqua;
    Xar*           expansa;
    Xar*           directivae;
    SilvaParsura*  parsura;

    si (piscina == NIHIL || expansio == NIHIL || fons == NIHIL
        || grammatica == NIHIL)
    {
        redde NIHIL;
    }

    /* Fistula praeprocessoris: lexare -> directivae (lineae captae -
     * reconstructio fontis, Phase 5) -> expansio positionalis
     * (point-in-time - Phase 2.5) */
    fons_index = silva_fons_addere(expansio,
        (titulus_fontis != NIHIL) ? titulus_fontis : "<fons>", FALSUM);
    lexemata = silva_lexare(piscina, fons, mensura, fons_index);
    directivae = NIHIL;
    reliqua = silva_expansio_directivas_processare(expansio, lexemata,
        &directivae);
    expansa = silva_expansio_expandere_reliqua(expansio, reliqua,
        NIHIL);

    parsura = silva_lexemata_parsare(piscina, expansa, grammatica,
        oraculum, resolutor, contextus);
    si (parsura != NIHIL)
    {
        parsura->lexemata = expansa;
        parsura->expansio = expansio;
        parsura->directivae = directivae;
        parsura->fons_princeps = fons_index;
    }
    redde parsura;
}

SilvaParsura*
silva_parsare (
    Piscina*                  piscina,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   contextus)
{
    SilvaExpansio* expansio;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    expansio = silva_expansio_creare(piscina);
    si (expansio == NIHIL)
    {
        redde NIHIL;
    }
    redde silva_parsare_cum_expansione(piscina, expansio,
        titulus_fontis, fons, mensura, grammatica, oraculum, resolutor,
        contextus);
}
