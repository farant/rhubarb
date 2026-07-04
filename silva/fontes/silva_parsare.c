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

/* Lexema in declaratione K&R plausibile (inter ")" et "{"
 * definitionis veteris: "int f(a) int a; { }"). Coniunctum
 * LAXUM - suspicio falsa segmentum crassius facit (elementa
 * plura, arbores rectae), numquam arborem falsam. */
hic_manens b32
_kr_plausibile (SilvaLexemaGenus genus)
{
    commutatio (genus)
    {
    casus SILVA_LEX_IDENTIFICATOR:
    casus SILVA_LEX_INTEGER:
    casus SILVA_LEX_STAR:
    casus SILVA_LEX_COMMA:
    casus SILVA_LEX_SEMICOLON:
    casus SILVA_LEX_QUADRA_APERTA:
    casus SILVA_LEX_QUADRA_CLAUSA:
    casus SILVA_LEX_PAREN_APERTA:
    casus SILVA_LEX_PAREN_CLAUSA:
    casus SILVA_LEX_VOID:
    casus SILVA_LEX_CHAR:
    casus SILVA_LEX_SHORT:
    casus SILVA_LEX_INT:
    casus SILVA_LEX_LONG:
    casus SILVA_LEX_FLOAT_KW:
    casus SILVA_LEX_DOUBLE:
    casus SILVA_LEX_SIGNED:
    casus SILVA_LEX_UNSIGNED:
    casus SILVA_LEX_CONST:
    casus SILVA_LEX_VOLATILE:
    casus SILVA_LEX_STRUCT:
    casus SILVA_LEX_UNION:
    casus SILVA_LEX_ENUM:
    casus SILVA_LEX_REGISTER:
    casus SILVA_LEX_AUTO:
    casus SILVA_LEX_STATIC:
    casus SILVA_LEX_EXTERN:
        redde VERUM;
    ordinarius:
        redde FALSUM;
    }
}

/* Lexema proximum non-NIHIL post indicem i (NIHIL si nullum) */
hic_manens SilvaToken*
_lexema_sequens (constans Xar* lexemata, i32 i, i32 numerus)
{
    i32 k;

    per (k = i + I; k < numerus; k++)
    {
        SilvaToken** ref = (SilvaToken**)xar_obtinere(lexemata, k);

        si (ref != NIHIL && *ref != NIHIL)
        {
            redde *ref;
        }
    }
    redde NIHIL;
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

/* Textura conditionalium (infra definita - ante commissionem) */
interior SilvaValor
_texere (
    Piscina*                       piscina,
    SilvaValor                     radix,
    SilvaExpansio*                 expansio,
    constans SilvaRegistrumCoctum* tabularium,
    SilvaParsura*                  parsura);

/* Nucleus circuitus secantis; contextus NIHIL licet (fines motoris
 * defaltae tunc solae valent); expansio (si praesens) texturam
 * conditionalium ante commissionem permittit */
interior SilvaParsura*
_lexemata_parsare_interna (
    Piscina*                  piscina,
    constans Xar*             lexemata,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris,
    SilvaExpansio*            expansio,
    constans SilvaContextus*  contextus)
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
    si (contextus != NIHIL)
    {
        glr->limen_frontis = contextus->fines.frons;
        glr->pergere = contextus->pergere;
        glr->pergere_datum = contextus->pergere_datum;
        glr->passus_pergendi = contextus->passus_pergendi;
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
    parsura->est_intermissa = FALSUM;
    parsura->expansio_decisa = FALSUM;
    parsura->fines_tactae = FALSUM;
    parsura->segmenta_ultra_limen = ZEPHYRUM;
    parsura->regiones_textae = ZEPHYRUM;
    parsura->regiones_omissae = ZEPHYRUM;
    parsura->strata = NIHIL;

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
     * parsatur. Fractura -> nodus ERROR (recuperatio per-rem).
     *
     * CONTINUATIO SENTENTIARUM (M2c Chunk A): semicolon in
     * profunditate 0 limes NON est si lexema proximum sententiam
     * CURRENTEM continuat, non novam incipit:
     *   - ELSE: bracchium alioquin ("if (a) x; else y;")
     *   - WHILE cum fac pendente: cauda fac-dum
     *     ("do x; while (y);"; numerator nidificationem tractat -
     *     "do do x; while (a); while (b);" recte)
     * Suppressio falsa (else vagum) segmentum longius ERROR facit
     * - ambitus recuperationis crassior, numquam falsa arbor. */
    initium = ZEPHYRUM;
    profunditas = ZEPHYRUM;
    i = ZEPHYRUM;
    {
        i32 fac_pendentia = ZEPHYRUM;
        b32 post_clausa = FALSUM;       /* ")" in prof. 0 visum */
        i32 intervallum_post = ZEPHYRUM; /* lexemata post ")" */
        b32 modus_corporis = FALSUM;    /* "{" post ")" - segmentum
                                         * unco clauditur, non ";" */

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
        alioquin si (lexema->genus == SILVA_LEX_BRACE_APERTA)
        {
            /* Uncus quoque numeratur (M2b): "struct S { int x; };"
             * UNUM segmentum est. Uncus post ")" in prof. 0 =
             * corpus definitionis/sententiae (M2c Chunk B) -
             * segmentum ad uncum clausum finietur, non ad ";"
             * (definitiones functionum terminatorem NON habent) */
            si (profunditas == ZEPHYRUM && post_clausa)
            {
                modus_corporis = VERUM;
            }
            profunditas++;
        }
        alioquin si (lexema->genus == SILVA_LEX_PAREN_CLAUSA)
        {
            si (profunditas > ZEPHYRUM)
            {
                profunditas--;
            }
            si (profunditas == ZEPHYRUM)
            {
                post_clausa = VERUM;
                intervallum_post = ZEPHYRUM;
            }
        }
        alioquin si (lexema->genus == SILVA_LEX_BRACE_CLAUSA)
        {
            si (profunditas > ZEPHYRUM)
            {
                profunditas--;
            }
            si (profunditas == ZEPHYRUM)
            {
                SilvaToken* proximum =
                    _lexema_sequens(lexemata, i, numerus);

                post_clausa = FALSUM;
                si (modus_corporis)
                {
                    /* corpus clausum = limes, nisi sententia
                     * continuatur (alioquin / cauda fac-dum) */
                    si (proximum != NIHIL
                        && proximum->genus == SILVA_LEX_ELSE)
                    {
                        /* continuatio */
                    }
                    alioquin si (proximum != NIHIL
                        && proximum->genus == SILVA_LEX_WHILE
                        && fac_pendentia > ZEPHYRUM)
                    {
                        fac_pendentia--;
                    }
                    alioquin
                    {
                        est_finis_segmenti = VERUM;
                    }
                }
                alioquin si (proximum != NIHIL
                    && proximum->genus == SILVA_LEX_WHILE
                    && fac_pendentia > ZEPHYRUM)
                {
                    /* "do { } while" - cauda post corpus uncatum
                     * sine modo corporis; pendentia consumitur ne
                     * in sententiam sequentem effluat */
                    fac_pendentia--;
                }
            }
        }
        alioquin si (lexema->genus == SILVA_LEX_DO
            && profunditas == ZEPHYRUM)
        {
            fac_pendentia++;
        }
        alioquin si (lexema->genus == SILVA_LEX_SEMICOLON
            && profunditas == ZEPHYRUM)
        {
            SilvaToken* proximum =
                _lexema_sequens(lexemata, i, numerus);

            si (proximum != NIHIL
                && proximum->genus == SILVA_LEX_ELSE)
            {
                /* continuatio - non limes */
            }
            alioquin si (proximum != NIHIL
                && proximum->genus == SILVA_LEX_WHILE
                && fac_pendentia > ZEPHYRUM)
            {
                /* cauda fac-dum - non limes */
                fac_pendentia--;
            }
            alioquin si (post_clausa && intervallum_post > ZEPHYRUM)
            {
                /* FORTASSE K&R ("int f(a) int a; { }"): ";" post
                 * ")" cum lexematibus interiacentibus. Percurrere
                 * per material declarationis: "{" ante lexema
                 * implausibile = K&R confirmatum, segmentum
                 * pergit; aliter limes ad candidatum HUNC.
                 * Suspicio falsa segmentum crassius facit
                 * (elementa plura, arbores rectae) - numquam
                 * arborem falsam. */
                b32 confirmatum = FALSUM;
                i32 j;

                per (j = i + I; j < numerus; j++)
                {
                    SilvaToken** ref_j =
                        (SilvaToken**)xar_obtinere(lexemata, j);
                    SilvaToken* tok = (ref_j != NIHIL)
                        ? *ref_j : NIHIL;

                    si (tok == NIHIL) perge;
                    si (tok->genus == SILVA_LEX_BRACE_APERTA)
                    {
                        confirmatum = VERUM;
                        frange;
                    }
                    si (!_kr_plausibile(tok->genus)) frange;
                }
                si (!confirmatum)
                {
                    est_finis_segmenti = VERUM;
                }
                /* confirmatum: non limes - corpus "{" mox modum
                 * corporis ponet (post_clausa adhuc VERUM) */
            }
            alioquin
            {
                est_finis_segmenti = VERUM;
            }
        }
        alioquin si (profunditas == ZEPHYRUM && post_clausa)
        {
            /* Lexema aliud in prof. 0 post ")": material
             * declarationis K&R intervallum auget; lexema
             * implausibile (operator, litera, ASSIGNATIO...)
             * clausam exstinguit - ")" ille declarator functionis
             * esse non potest ("int x = (a) + b;") */
            si (_kr_plausibile(lexema->genus))
            {
                intervallum_post++;
            }
            alioquin
            {
                post_clausa = FALSUM;
            }
        }

        si (est_finis_segmenti)
        {
            i32 finis = est_eof ? i : (i + I);
            Xar* segmentum;
            i32 k;
            SilvaGLRFructus fructus;

            /* Intermissio (Phase 7): ante segmentum interrogamus;
             * cauda TOTA [initium..EOF) nodus ERROR unus fit - arbor
             * completa manet, reconstructio byte-exacta tenet */
            si (!parsura->est_intermissa
                && contextus != NIHIL && contextus->pergere != NIHIL
                && !contextus->pergere(contextus->pergere_datum))
            {
                parsura->est_intermissa = VERUM;
            }
            si (parsura->est_intermissa)
            {
                i32 finis_caudae = (lexema_eof != NIHIL)
                    ? (numerus - I) : numerus;

                si (finis_caudae > initium)
                {
                    SilvaValor nodus_caudae = _nodum_erroris_facere(
                        piscina, lexemata, initium, finis_caudae,
                        genus_erroris, locus_lexematum,
                        numerus_locorum_erroris);

                    si (nodus_caudae.genus == SILVA_VALOR_NODUS)
                    {
                        radix = silva_valor_lista_appendere(piscina,
                            radix, nodus_caudae);
                        parsura->numerus_errorum++;
                    }
                }
                frange;
            }

            segmentum = xar_creare(piscina,
                (i32)magnitudo(SilvaToken*));

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
            si (fructus.est_ultra_limen)
            {
                parsura->segmenta_ultra_limen++;
                parsura->fines_tactae = VERUM;
            }
            si (fructus.est_intermissus)
            {
                /* Segmentum hoc nodus ERROR fit (via ordinaria infra);
                 * cauda in limite proximo (unus nodus) */
                parsura->est_intermissa = VERUM;
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

            /* status secantis per segmentum - purgare */
            fac_pendentia = ZEPHYRUM;
            post_clausa = FALSUM;
            intervallum_post = ZEPHYRUM;
            modus_corporis = FALSUM;
        }

        si (est_eof)
        {
            frange;
        }
        i++;
    }
    }  /* status secantis */

    /* Textura conditionalium (Phase 7 Chunk B) - ANTE commissionem
     * (pater fixup nodos novos ambulatione generica tegit) */
    si (expansio != NIHIL)
    {
        radix = _texere(piscina, radix, expansio,
            grammatica->tabularium, parsura);
    }

    /* Uncus praecommissionis (M2b): registratio typorum et alia
     * grammaticae-propria ANTE resolutionem - radix nondum
     * commissa (lectio sola) */
    si (grammatica->praecommissio != NIHIL)
    {
        grammatica->praecommissio(radix, datum_resolutoris);
    }

    /* Commissio: pater + normalizatio + resolutio (collapse+diarium) */
    parsura->commissio = silva_committere(piscina, radix,
        grammatica->tabularium, oraculum, resolutor, datum_resolutoris);
    parsura->successus = (parsura->commissio != NIHIL) ? VERUM : FALSUM;
    redde parsura;
}

SilvaParsura*
silva_lexemata_parsare (
    Piscina*                  piscina,
    constans Xar*             lexemata,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris)
{
    redde _lexemata_parsare_interna(piscina, lexemata, grammatica,
        oraculum, resolutor, datum_resolutoris, NIHIL, NIHIL);
}

/* ==================================================
 * Textura conditionalium (Phase 7 Chunk B, simulatio ⑦)
 *
 * Passus gubernatoris ANTE commissionem: regiones conditionales in
 * arborem texuntur ubi limites regionis cum limitibus sententiarum
 * congruunt (detectio per extenta strati 0); aliter regio degradat
 * (laminis reinserendis possessa manet - mos hodiernus IPSE est via
 * regressus). Textura est opt-in per regionem; numeratores
 * regiones_textae/omissae rationem produnt.
 *
 * Formae nodorum (genera-extra, generator eas IMPONIT):
 *   conditionalis {rami:lista-nodus, finis:lista-token}
 *   ramus-sumptus {directiva, contentum:lista-nodus, conditio_id}
 *   ramus-omissus {directiva, cruda:lista-token, conditio_id}
 * Ordo locorum == ordo octetorum - emissor genericus nihil novi
 * requirit.
 * ================================================== */

/* Extentum strati 0 valoris: fons + [initium, finis) super lexemata
 * radicum. fons -1 = nondum visum; -2 = opacum (catena pasta/chorda
 * aut fontes mixti) - degradatio conservativa. */
nomen structura {
    s32 fons;
    s32 initium;
    s32 finis;
} TexExtentum;

interior vacuum
_extentum_valoris (SilvaValor valor, TexExtentum* extentum)
{
    commutatio (valor.genus)
    {
        casus SILVA_VALOR_TOKEN:
        {
            SilvaToken* t = valor.datum.token;

            dum (t != NIHIL
                && (t->origo.genus == SILVA_ORIGO_EXPANSIO
                    || t->origo.genus == SILVA_ORIGO_CHORDA))
            {
                t = (t->origo.genus == SILVA_ORIGO_EXPANSIO)
                    ? t->origo.datum.expansio.invocatio
                    : t->origo.datum.stringificatio.primus;
            }
            si (t == NIHIL || t->origo.genus != SILVA_ORIGO_FONS)
            {
                extentum->fons = -II;  /* opacum */
                redde;
            }
            si (extentum->fons == -II)
            {
                redde;
            }
            si (extentum->fons == -I)
            {
                extentum->fons = t->fons_index;
                extentum->initium = t->byte_offset;
                extentum->finis = t->byte_offset + (s32)t->longitudo;
            }
            alioquin si (extentum->fons != t->fons_index)
            {
                extentum->fons = -II;  /* fontes mixti */
            }
            alioquin
            {
                si (t->byte_offset < extentum->initium)
                {
                    extentum->initium = t->byte_offset;
                }
                si (t->byte_offset + (s32)t->longitudo > extentum->finis)
                {
                    extentum->finis = t->byte_offset + (s32)t->longitudo;
                }
            }
            frange;
        }
        casus SILVA_VALOR_NODUS:
        {
            constans SilvaNodus* nodus = valor.datum.nodus;
            i32 k;

            per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
            {
                _extentum_valoris(nodus->loci[k], extentum);
                si (extentum->fons == -II)
                {
                    redde;
                }
            }
            frange;
        }
        casus SILVA_VALOR_LISTA:
        {
            i32 n = silva_valor_lista_numerus(valor);
            i32 k;

            per (k = ZEPHYRUM; k < n; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(valor, k);

                si (elem != NIHIL)
                {
                    _extentum_valoris(*elem, extentum);
                    si (extentum->fons == -II)
                    {
                        redde;
                    }
                }
            }
            frange;
        }
        ordinarius:
            frange;  /* INDEX/NIHIL: sine positione */
    }
}

/* Formae generum texturae ex registro PER NOMEN (mos commissionis) */
nomen structura {
    s32 genus;
    i32 numerus_locorum;
    s32 locus_a;
    s32 locus_b;
    s32 locus_c;
} TexForma;

interior b32
_formam_texturae_invenire (
    constans SilvaRegistrumCoctum* tabularium,
    constans character*            titulus_generis,
    constans character*            titulus_a,
    constans character*            titulus_b,
    constans character*            titulus_c,
    TexForma*                      forma)
{
    i32 i;

    forma->genus = -I;
    forma->locus_a = -I;
    forma->locus_b = -I;
    forma->locus_c = -I;
    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans SilvaTabGenus* genus = &tabularium->genera[i];
        i32 k;

        si (genus->titulus == NIHIL
            || strcmp(genus->titulus, titulus_generis) != ZEPHYRUM)
        {
            perge;
        }
        forma->genus = (s32)i;
        forma->numerus_locorum = genus->loci_numerus;
        per (k = ZEPHYRUM; k < genus->loci_numerus; k++)
        {
            constans SilvaTabLocus* locus =
                &tabularium->loci[genus->loci_offset + k];

            si (locus->titulus == NIHIL) perge;
            si (strcmp(locus->titulus, titulus_a) == ZEPHYRUM)
            {
                forma->locus_a = (s32)k;
            }
            alioquin si (titulus_b != NIHIL
                && strcmp(locus->titulus, titulus_b) == ZEPHYRUM)
            {
                forma->locus_b = (s32)k;
            }
            alioquin si (titulus_c != NIHIL
                && strcmp(locus->titulus, titulus_c) == ZEPHYRUM)
            {
                forma->locus_c = (s32)k;
            }
        }
        redde (forma->locus_a >= ZEPHYRUM
            && (titulus_b == NIHIL || forma->locus_b >= ZEPHYRUM)
            && (titulus_c == NIHIL || forma->locus_c >= ZEPHYRUM))
            ? VERUM : FALSUM;
    }
    redde FALSUM;
}

/* Status passus texturae */
nomen structura {
    Piscina*                       piscina;
    constans SilvaRegistrumCoctum* tabularium;
    SilvaParsura*                  parsura;
    SilvaValor                     radix;    /* lista currens */
    TexForma                       conditionalis; /* a=rami b=finis */
    TexForma                       sumptus;  /* a=directiva b=contentum c=conditio_id */
    TexForma                       omissus;  /* a=directiva b=cruda c=conditio_id */
} TexturaStatus;

/* Lexemata laminae in locum listae-token appendere */
interior vacuum
_laminam_in_locum (Piscina* piscina, SilvaNodus* nodus, i32 locus,
    Xar* lamina)
{
    i32 k;

    si (lamina == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(lamina); k++)
    {
        SilvaToken** ref = (SilvaToken**)xar_obtinere(
            lamina, k);

        si (ref != NIHIL && *ref != NIHIL)
        {
            silva_nodus_appendere(piscina, nodus, locus,
                silva_valor_token(*ref), SILVA_LOCUS_LISTA_TOKEN);
        }
    }
}

/* Regionem unam texere temptare (filiae IAM visitatae - profundum
 * primum). Detectio: quodque elementum radicis regioni intersecans
 * PLENE intra corpus rami sumpti iacere debet; elementum opacum
 * intersectionis incertae = degradatio conservativa. */
interior vacuum
_regionem_texere (TexturaStatus* st, SilvaRegio* regio)
{
    SilvaRamus* ramus_sumptus = NIHIL;
    s32 regio_initium;
    s32 regio_finis;
    i32 numerus_elementorum;
    s32 primum_congregatum = -I;
    s32 primum_post = -I;
    b32 degradata = FALSUM;
    b32* congreganda;
    i32 e;

    si (regio->rami == NIHIL || xar_numerus(regio->rami) == ZEPHYRUM)
    {
        st->parsura->regiones_omissae++;
        redde;
    }

    /* Fines regionis in offsetibus */
    {
        SilvaRamus* primus = *(SilvaRamus**)xar_obtinere(regio->rami,
            ZEPHYRUM);
        SilvaToken* t;

        si (primus->directiva == NIHIL
            || xar_numerus(primus->directiva) == ZEPHYRUM)
        {
            st->parsura->regiones_omissae++;
            redde;
        }
        t = *(SilvaToken**)xar_obtinere(primus->directiva, ZEPHYRUM);
        regio_initium = t->byte_offset;
    }
    si (regio->directiva_finis != NIHIL
        && xar_numerus(regio->directiva_finis) > ZEPHYRUM)
    {
        SilvaToken* ultimum = *(SilvaToken**)xar_obtinere(
            regio->directiva_finis,
            (i32)(xar_numerus(regio->directiva_finis) - I));

        regio_finis = ultimum->byte_offset + (s32)ultimum->longitudo;
    }
    alioquin
    {
        regio_finis = 0x7FFFFFFF;  /* imperfecta: apertum ad EOF */
    }

    /* Ramus sumptus (si quis) */
    {
        i32 r;

        per (r = ZEPHYRUM; r < xar_numerus(regio->rami); r++)
        {
            SilvaRamus* ramus = *(SilvaRamus**)xar_obtinere(
                regio->rami, r);

            si (ramus != NIHIL && ramus->est_sumptum)
            {
                ramus_sumptus = ramus;
                frange;
            }
        }
    }

    /* Elementa classificare */
    numerus_elementorum = silva_valor_lista_numerus(st->radix);
    congreganda = (b32*)piscina_allocare(st->piscina,
        (memoriae_index)(magnitudo(b32)
            * (numerus_elementorum > ZEPHYRUM
                ? (memoriae_index)numerus_elementorum : I)));
    si (congreganda == NIHIL)
    {
        st->parsura->regiones_omissae++;
        redde;
    }
    per (e = ZEPHYRUM; e < numerus_elementorum; e++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(st->radix, e);
        TexExtentum extentum;

        congreganda[e] = FALSUM;
        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        extentum.fons = -I;
        extentum.initium = ZEPHYRUM;
        extentum.finis = ZEPHYRUM;
        _extentum_valoris(*elem, &extentum);
        si (extentum.fons == -II)
        {
            /* opacum: positio incerta - degradatio conservativa */
            degradata = VERUM;
            frange;
        }
        si (extentum.fons == -I || extentum.fons != regio->fons_index)
        {
            perge;  /* plagula alia aut sine positione */
        }
        si (extentum.finis <= regio_initium)
        {
            perge;  /* ante regionem */
        }
        si (extentum.initium >= regio_finis)
        {
            si (primum_post == -I)
            {
                primum_post = (s32)e;
            }
            perge;  /* post regionem */
        }
        /* intersecat: plene intra corpus rami sumpti aut degradatio */
        si (ramus_sumptus != NIHIL
            && ramus_sumptus->corpus_initium >= ZEPHYRUM
            && extentum.initium >= ramus_sumptus->corpus_initium
            && extentum.finis <= ramus_sumptus->corpus_finis)
        {
            congreganda[e] = VERUM;
            si (primum_congregatum == -I)
            {
                primum_congregatum = (s32)e;
            }
        }
        alioquin
        {
            degradata = VERUM;  /* limes transgressus */
            frange;
        }
    }
    si (degradata)
    {
        st->parsura->regiones_omissae++;
        redde;
    }

    /* Nodum conditionalis aedificare */
    {
        SilvaNodus* nodus_conditionalis;
        i32 r;

        nodus_conditionalis = silva_nodus_creare(st->piscina,
            st->conditionalis.genus,
            st->conditionalis.numerus_locorum);
        si (nodus_conditionalis == NIHIL)
        {
            st->parsura->regiones_omissae++;
            redde;
        }
        per (r = ZEPHYRUM; r < xar_numerus(regio->rami); r++)
        {
            SilvaRamus* ramus = *(SilvaRamus**)xar_obtinere(
                regio->rami, r);
            constans TexForma* forma;
            SilvaNodus* nodus_rami;

            si (ramus == NIHIL) perge;
            forma = ramus->est_sumptum ? &st->sumptus : &st->omissus;
            nodus_rami = silva_nodus_creare(st->piscina, forma->genus,
                forma->numerus_locorum);
            si (nodus_rami == NIHIL)
            {
                st->parsura->regiones_omissae++;
                redde;
            }
            _laminam_in_locum(st->piscina, nodus_rami, (i32)forma->locus_a,
                ramus->directiva);
            si (ramus->est_sumptum)
            {
                per (e = ZEPHYRUM; e < numerus_elementorum; e++)
                {
                    si (congreganda[e])
                    {
                        SilvaValor* elem = silva_valor_lista_obtinere(
                            st->radix, e);

                        si (elem != NIHIL)
                        {
                            silva_nodus_appendere(st->piscina,
                                nodus_rami, (i32)forma->locus_b, *elem,
                                SILVA_LOCUS_LISTA_NODUS);
                        }
                    }
                }
            }
            alioquin
            {
                _laminam_in_locum(st->piscina, nodus_rami,
                    (i32)forma->locus_b, ramus->lexemata_cruda);
            }
            silva_nodus_ponere(nodus_rami, (i32)forma->locus_c,
                silva_valor_index((s32)ramus->conditio_id),
                SILVA_LOCUS_INDEX);
            silva_nodus_appendere(st->piscina, nodus_conditionalis,
                (i32)st->conditionalis.locus_a,
                silva_valor_nodus(nodus_rami),
                SILVA_LOCUS_LISTA_NODUS);
        }
        _laminam_in_locum(st->piscina, nodus_conditionalis,
            (i32)st->conditionalis.locus_b, regio->directiva_finis);

        /* Listam radicis retexere: congregata sublata, conditionalis
         * in positione ordinali primi congregati (aut ante primum
         * elementum eiusdem fontis post regionem; aut in fine) */
        {
            SilvaValor lista_nova = silva_valor_lista_nova(st->piscina);
            i32 positio_insertionis;
            b32 insertum = FALSUM;

            si (primum_congregatum >= ZEPHYRUM)
            {
                positio_insertionis = (i32)primum_congregatum;
            }
            alioquin si (primum_post >= ZEPHYRUM)
            {
                positio_insertionis = (i32)primum_post;
            }
            alioquin
            {
                positio_insertionis = numerus_elementorum;
            }
            per (e = ZEPHYRUM; e < numerus_elementorum; e++)
            {
                SilvaValor* elem;

                si (e == positio_insertionis)
                {
                    lista_nova = silva_valor_lista_appendere(
                        st->piscina, lista_nova,
                        silva_valor_nodus(nodus_conditionalis));
                    insertum = VERUM;
                }
                si (congreganda[e])
                {
                    perge;  /* in contentum migravit */
                }
                elem = silva_valor_lista_obtinere(st->radix, e);
                si (elem != NIHIL)
                {
                    lista_nova = silva_valor_lista_appendere(
                        st->piscina, lista_nova, *elem);
                }
            }
            si (!insertum)
            {
                lista_nova = silva_valor_lista_appendere(st->piscina,
                    lista_nova,
                    silva_valor_nodus(nodus_conditionalis));
            }
            st->radix = lista_nova;
        }
    }

    regio->est_texta = VERUM;
    st->parsura->regiones_textae++;
}

/* Arbor regionum profundum primum (filiae ante patrem - conditionalis
 * interior elementum extenti gerens fit quod textura exterior
 * congregat) */
interior vacuum
_regiones_texere (TexturaStatus* st, Xar* regiones)
{
    i32 i;

    si (regiones == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(regiones); i++)
    {
        SilvaRegio* regio = *(SilvaRegio**)xar_obtinere(regiones, i);

        si (regio == NIHIL) perge;
        _regiones_texere(st, regio->filiae);
        _regionem_texere(st, regio);
    }
}

/* Ingressus passus: radix nova (aut eadem) redditur */
interior SilvaValor
_texere (
    Piscina*                       piscina,
    SilvaValor                     radix,
    SilvaExpansio*                 expansio,
    constans SilvaRegistrumCoctum* tabularium,
    SilvaParsura*                  parsura)
{
    TexturaStatus st;

    si (expansio == NIHIL || expansio->regiones == NIHIL
        || xar_numerus(expansio->regiones) == ZEPHYRUM)
    {
        redde radix;
    }
    si (!_formam_texturae_invenire(tabularium, "conditionalis",
            "rami", "finis", NIHIL, &st.conditionalis)
        || !_formam_texturae_invenire(tabularium, "ramus-sumptus",
            "directiva", "contentum", "conditio_id", &st.sumptus)
        || !_formam_texturae_invenire(tabularium, "ramus-omissus",
            "directiva", "cruda", "conditio_id", &st.omissus))
    {
        /* generator formas imponit - hic defensivum solum */
        fprintf(stderr,
            "silva_parsare: formae texturae in registro absunt\n");
        redde radix;
    }
    st.piscina = piscina;
    st.tabularium = tabularium;
    st.parsura = parsura;
    st.radix = radix;
    _regiones_texere(&st, expansio->regiones);
    redde st.radix;
}

/* Fistula tota: praeprocessor + circuitus secans; contextus NIHIL
 * licet (via cum_expansione) */
interior SilvaParsura*
_fistula_interna (
    Piscina*                  piscina,
    SilvaExpansio*            expansio,
    constans SilvaContextus*  contextus,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris)
{
    s32            fons_index;
    Xar*           lexemata;
    Xar*           reliqua;
    Xar*           expansa;
    Xar*           strata;
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
    strata = NIHIL;
    expansa = silva_expansio_expandere_reliqua(expansio, reliqua,
        &strata);

    parsura = _lexemata_parsare_interna(piscina, expansa, grammatica,
        oraculum, resolutor, datum_resolutoris, expansio, contextus);
    si (parsura != NIHIL)
    {
        parsura->lexemata = expansa;
        parsura->strata = strata;
        parsura->expansio = expansio;
        parsura->directivae = directivae;
        parsura->fons_princeps = fons_index;

        /* Vexilla expansionis in fructum (fines/intermissio in
         * praeprocessore tactae) */
        si (expansio->expansio_decisa)
        {
            parsura->expansio_decisa = VERUM;
            parsura->fines_tactae = VERUM;
        }
        si (expansio->fines_tactae)
        {
            parsura->fines_tactae = VERUM;
        }
        si (expansio->est_intermissa)
        {
            parsura->est_intermissa = VERUM;
        }
    }
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
    vacuum*                   datum_resolutoris)
{
    redde _fistula_interna(piscina, expansio, NIHIL, titulus_fontis,
        fons, mensura, grammatica, oraculum, resolutor,
        datum_resolutoris);
}

/* Receptum contextus expansioni recenti applicare: fines + pergere
 * deorsum copiantur (strata inferiora contextum nesciunt), praebenda
 * praebentur, lexica processantur (definitiones eorum a positione 0
 * fontis principalis valent - positus eventorum novorum in zephyrum
 * fingitur). */
interior b32
_contextum_applicare (
    Piscina*                 piscina,
    SilvaExpansio*           expansio,
    constans SilvaContextus* contextus)
{
    i32 k;

    expansio->limen_lexematum = contextus->fines.lexemata;
    expansio->limen_generationum = contextus->fines.generationes;
    expansio->limen_includendi =
        contextus->fines.profunditas_includendi;
    expansio->limen_regionum = contextus->fines.profunditas_regionum;
    expansio->pergere = contextus->pergere;
    expansio->pergere_datum = contextus->pergere_datum;

    per (k = ZEPHYRUM; k < xar_numerus(contextus->praebenda); k++)
    {
        constans SilvaContextusPlagula* plagula =
            (constans SilvaContextusPlagula*)xar_obtinere(
                contextus->praebenda, k);

        si (plagula == NIHIL
            || silva_includendum_praebere(expansio, plagula->via,
                   plagula->textus, plagula->mensura) < ZEPHYRUM)
        {
            redde FALSUM;
        }
    }

    per (k = ZEPHYRUM; k < xar_numerus(contextus->lexica); k++)
    {
        constans SilvaContextusPlagula* plagula =
            (constans SilvaContextusPlagula*)xar_obtinere(
                contextus->lexica, k);
        s32  fons_index;
        Xar* lexemata;
        i32  acta_ante;
        i32  e;

        si (plagula == NIHIL)
        {
            redde FALSUM;
        }
        acta_ante = xar_numerus(expansio->acta);
        fons_index = silva_fons_addere(expansio, plagula->via, FALSUM);
        si (fons_index < ZEPHYRUM)
        {
            redde FALSUM;
        }
        lexemata = silva_lexare(piscina, plagula->textus,
            plagula->mensura, fons_index);
        si (lexemata == NIHIL)
        {
            redde FALSUM;
        }
        /* Reliqua lexici abiciuntur (lexicon = directivae; contentum
         * praeter eas nusquam fluit - documentatum) */
        silva_expansio_directivas_processare(expansio, lexemata,
            NIHIL);

        /* Positus in zephyrum: eventa lexici a positione 0 fluxus
         * PRINCIPALIS valent (positus eorum ad fluxum lexici
         * pertinebat) */
        per (e = acta_ante; e < xar_numerus(expansio->acta); e++)
        {
            SilvaEventum* eventum =
                (SilvaEventum*)xar_obtinere(expansio->acta, e);

            si (eventum != NIHIL)
            {
                eventum->positus = ZEPHYRUM;
            }
        }
    }
    redde VERUM;
}

SilvaParsura*
silva_parsare_cum_contextu (
    Piscina*                  piscina,
    constans SilvaContextus*  contextus,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris)
{
    SilvaExpansio* expansio;

    si (piscina == NIHIL || contextus == NIHIL)
    {
        redde NIHIL;
    }
    expansio = silva_expansio_creare(piscina);
    si (expansio == NIHIL)
    {
        redde NIHIL;
    }
    si (!_contextum_applicare(piscina, expansio, contextus))
    {
        redde NIHIL;
    }
    redde _fistula_interna(piscina, expansio, contextus,
        titulus_fontis, fons, mensura, grammatica, oraculum, resolutor,
        datum_resolutoris);
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
    vacuum*                   datum_resolutoris)
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
        datum_resolutoris);
}
