/* css_arbor.c - Vide css_arbor.h.
 *
 * B2: regulae qualificatae + saepta + valores componentes.
 * Declarationes B3 veniunt, regulae-apud et recuperatio B4.
 *
 * ==================================================
 * REGIMEN CONTENTI (D7 AUCTUM) - spatia ET commentaria
 * ==================================================
 *
 * Intra praeludia (et B3: valores declarationum) OMNE lexema
 * contentum est: spatia et commentaria folia 'lexema-servatum'
 * fiunt, ligator PRAETERITUR, trivia intra regiones contenti NON
 * exsistunt. Tres rationes, quaeque alternativae fatalis:
 *
 *  I. ORDO OCTETORUM. Trivia iuxta dominum emittuntur; commentarium
 *     inter duo folia servata ('1px / * c * / 2px') ad lexema non
 *     vicinum ligari non potest sine reordinatione octetorum.
 * II. Constrictum T9 numquam ardet: materia_arbor.c:945 valorem
 *     lexematis crudum notat SOLUM cum textus filius UNICUS est -
 *     lexema spatii trivia ferens proici non posset. Sine triviis
 *     in regionibus contenti nullum folium contentum trivia fert.
 *III. Modellum obiecti CSS ipsius: L3 fluxum lexematum intra
 *     valores servat; commentaria ibi lexemata non sunt quia spec
 *     ea ABICIT - nos ea ut contentum servamus, sine iactura.
 *
 * MECHANISMUS: vexillum 'contentum' in CssParsura. Eo posito,
 * trivia non cumulantur - per solvere eunt (pendentia vacua =
 * prior solum ponitur, css_adaptare.c:162) et significantia
 * redduntur. Prior ergo trans limitem regiminum numquam
 * obsolescit, et pendentia in regionem contenti intrare non
 * possunt quia limen semper post significans solutum iacet.
 */

#include "css_arbor.h"
#include "css_adaptare.h"
#include "css_lexicon.h"
#include "css_lexema.h"
#include "css_registrum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include "xar.h"
#include <string.h>


/* ==================================================
 * Status parsurae
 * ================================================== */

nomen structura {
                 Piscina* piscina;
     MateriaLexiconRatum  lexicon;
                     Xar* plana;      /* lexemata, unum pro uno */
                     i32  index;      /* positio in plana */
              CssLigator  ligator;
                     b32  contentum;  /* regimen praesens (D7) */
            MateriaToken* finis;      /* FINIS intra regulam visum */
} CssParsura;


/* Lexema significans proximum. REGIMINE STRUCTURALI trivia
 * cumulantur et ligantur; REGIMINE CONTENTI omne lexema
 * significans est (vide caput).
 *
 * FINIS HAERENS (01M1FACW): semel visus, OMNI petenti iterum
 * redditur. Consumens interior (saeptum, functio) FINEM videt et
 * frangit; exterior deinde lexema aliud petit - sine haesione
 * fluxus exhaustus NIHIL daret et 'a{b:(}' parsuram totam
 * perderet. Ligator FINEM semel solvit (trivia caudae ad eum
 * ligantur), deinde numquam iterum.
 *
 * NIHIL significat fluxum SINE lexemate FINIS, quod vitium
 * css_lexare esset, non fontis - css_lexema.h FINIS ultimum
 * promittit. Ergo ansa vocantis semper terminatur. */
interior MateriaToken*
_significans_proximum (
    CssParsura* p)
{
    si (p->finis != NIHIL)
    {
        redde p->finis;
    }
    dum (p->index < xar_numerus(p->plana))
    {
        MateriaToken* token;

        token     = *(MateriaToken**)xar_obtinere(p->plana, p->index);
        p->index  = p->index + I;

        si (   !p->contentum
            && materia_lexicon_trivium_est(&p->lexicon, token->genus))
        {
            si (!css_ligator_cumulare(&p->ligator, token))
            {
                redde NIHIL;
            }
            perge;
        }
        si (!css_ligator_solvere(&p->ligator, token))
        {
            redde NIHIL;
        }
        si (materia_lexicon_munus(&p->lexicon, token->genus)
            == MATERIA_MUNUS_FINIS)
        {
            p->finis = token;
        }
        redde token;
    }
    redde NIHIL;
}

interior b32
_finis_est (
    constans   CssParsura* p,
    constans MateriaToken* token)
{
    redde (b32)(materia_lexicon_munus(&p->lexicon, token->genus)
                == MATERIA_MUNUS_FINIS);
}


/* ==================================================
 * Valores componentes (L3 5.4.7)
 * ================================================== */

interior MateriaNodus*
_valorem_componentem_consumere (
      CssParsura* p,
    MateriaToken* token);

interior MateriaNodus*
_servatum_creare (
      CssParsura* p,
    MateriaToken* token)
{
    MateriaNodus* servatum;

    servatum = materia_nodus_creare(p->piscina,
        (s32)CSS_GENUS_LEXEMA_SERVATUM, (i32)I);
    si (servatum == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(servatum, (i32)CSS_SERVATUM_TOK,
            materia_valor_token(token), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde servatum;
}

/* Genus clausurae congruentis; -I si non apertum. Clausura NON
 * congruens intra saeptum lexema servatum manet (L3: praeter
 * congruentem nihil saeptum claudit). */
interior s32
_clausum_congruens (
    s32 genus)
{
    si (genus == (s32)CSS_LEX_PAREN_APERTA)
    {
        redde (s32)CSS_LEX_PAREN_CLAUSA;
    }
    si (genus == (s32)CSS_LEX_QUADRA_APERTA)
    {
        redde (s32)CSS_LEX_QUADRA_CLAUSA;
    }
    si (genus == (s32)CSS_LEX_BRACE_APERTA)
    {
        redde (s32)CSS_LEX_BRACE_CLAUSA;
    }
    redde -I;
}

/* "Consume a simple block" (L3 5.4.8): contentum usque ad
 * clausuram CONGRUENTEM. FINIS saeptum claudit - TOTALITAS -
 * et tok_clausum ABSENS manet (VALOR_NIHIL), quem scriptores
 * omittunt; octeti ergo exacte quod fons fert reddunt. */
interior MateriaNodus*
_saeptum_consumere (
      CssParsura* p,
    MateriaToken* apertum)
{
    MateriaNodus* saeptum;
     MateriaValor contentum;
              s32 clausum;

    clausum = _clausum_congruens(apertum->genus);
    saeptum = materia_nodus_creare(p->piscina,
        (s32)CSS_GENUS_SAEPTUM, (i32)III);
    si (saeptum == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(saeptum, (i32)CSS_SAEPTUM_TOK_APERTUM,
            materia_valor_token(apertum), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    contentum = materia_valor_lista_nova(p->piscina);

    per (;;)
    {
        MateriaToken* token;
        MateriaNodus* valor;

        token = _significans_proximum(p);
        si (token == NIHIL)
        {
            redde NIHIL;
        }
        si (_finis_est(p, token))
        {
            p->finis = token;
            frange;
        }
        si (token->genus == clausum)
        {
            si (!materia_nodus_ponere(saeptum,
                    (i32)CSS_SAEPTUM_TOK_CLAUSUM,
                    materia_valor_token(token), MATERIA_LOCUS_TOKEN))
            {
                redde NIHIL;
            }
            frange;
        }
        valor = _valorem_componentem_consumere(p, token);
        si (valor == NIHIL)
        {
            redde NIHIL;
        }
        contentum = materia_valor_lista_appendere(p->piscina,
            contentum, materia_valor_nodus(valor));
    }

    si (!materia_nodus_ponere(saeptum, (i32)CSS_SAEPTUM_CONTENTUM,
            contentum, MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    redde saeptum;
}

/* "Consume a function" (L3 5.4.9): lexator 'ident(' in lexema
 * FUNCTIO unum iam fudit; argumenta usque ad ')'. FINIS functionem
 * claudit, tok_clausum absens (ut saeptum supra). */
interior MateriaNodus*
_functionem_consumere (
      CssParsura* p,
    MateriaToken* titulus)
{
    MateriaNodus* functio;
     MateriaValor argumenta;

    functio = materia_nodus_creare(p->piscina,
        (s32)CSS_GENUS_FUNCTIO, (i32)III);
    si (functio == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(functio, (i32)CSS_FUNCTIO_TOK_NOMEN,
            materia_valor_token(titulus), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    argumenta = materia_valor_lista_nova(p->piscina);

    per (;;)
    {
        MateriaToken* token;
        MateriaNodus* valor;

        token = _significans_proximum(p);
        si (token == NIHIL)
        {
            redde NIHIL;
        }
        si (_finis_est(p, token))
        {
            p->finis = token;
            frange;
        }
        si (token->genus == (s32)CSS_LEX_PAREN_CLAUSA)
        {
            si (!materia_nodus_ponere(functio,
                    (i32)CSS_FUNCTIO_TOK_CLAUSUM,
                    materia_valor_token(token), MATERIA_LOCUS_TOKEN))
            {
                redde NIHIL;
            }
            frange;
        }
        valor = _valorem_componentem_consumere(p, token);
        si (valor == NIHIL)
        {
            redde NIHIL;
        }
        argumenta = materia_valor_lista_appendere(p->piscina,
            argumenta, materia_valor_nodus(valor));
    }

    si (!materia_nodus_ponere(functio, (i32)CSS_FUNCTIO_ARGUMENTA,
            argumenta, MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    redde functio;
}

interior MateriaNodus*
_valorem_componentem_consumere (
      CssParsura* p,
    MateriaToken* token)
{
    si (token->genus == (s32)CSS_LEX_FUNCTIO)
    {
        redde _functionem_consumere(p, token);
    }
    si (_clausum_congruens(token->genus) >= ZEPHYRUM)
    {
        redde _saeptum_consumere(p, token);
    }
    redde _servatum_creare(p, token);
}


/* ==================================================
 * Declarationes (L3 5.4.4-5.4.6, B3)
 * ================================================== */

/* Lexema modo lectum reddere: index retro. Lexema iterum per
 * solvere ibit (prior idem ponitur - innocuum, pendentia post
 * significans semper vacua). */
interior vacuum
_retrocedere (
    CssParsura* p)
{
    p->index = p->index - I;
}

interior b32
_est_delim_clamor (
    constans MateriaToken* token)
{
    redde (b32)(   token->genus == (s32)CSS_LEX_DELIM
                && token->valor.mensura == I
                && token->valor.datum[ZEPHYRUM] == '!');
}

/* 'important', ASCII sine casu (L3). */
interior b32
_est_important (
    constans MateriaToken* token)
{
    constans character* exemplar = "important";
                   i32  i;

    si (   token->genus         != (s32)CSS_LEX_IDENTIFICATOR
        || token->valor.mensura != IX)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < IX; i++)
    {
        character c = (character)token->valor.datum[i];

        si (c >= 'A' && c <= 'Z')
        {
            c = (character)(c + XXXII);
        }
        si (c != exemplar[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Folium servatum (et alterum si adest) valori appendere; in
 * fractura regimen contenti claudit. */
interior b32
_folia_appendere (
      CssParsura* p,
    MateriaValor* valor,
    MateriaToken* primum,
    MateriaToken* alterum)
{
    MateriaNodus* folium;

    folium = _servatum_creare(p, primum);
    si (folium == NIHIL)
    {
        p->contentum = FALSUM;
        redde FALSUM;
    }
    *valor = materia_valor_lista_appendere(p->piscina, *valor,
        materia_valor_nodus(folium));
    si (alterum != NIHIL)
    {
        folium = _servatum_creare(p, alterum);
        si (folium == NIHIL)
        {
            p->contentum = FALSUM;
            redde FALSUM;
        }
        *valor = materia_valor_lista_appendere(p->piscina, *valor,
            materia_valor_nodus(folium));
    }
    redde VERUM;
}

/* Mala sine consumptione ulteriore (FINIS aut '}' iam visum).
 * Genus datur: DECLARATIO_MALA intra corpora, REGULA_MALA suprema -
 * forma eadem (tokens:lista-token), sedes alia. */
interior MateriaNodus*
_malam_consumere_finita (
      CssParsura* p,
             s32  genus,
    MateriaToken* titulus)
{
    MateriaNodus* mala;
     MateriaValor lexemata;

    mala = materia_nodus_creare(p->piscina, genus, (i32)I);
    si (mala == NIHIL)
    {
        redde NIHIL;
    }
    lexemata = materia_valor_lista_nova(p->piscina);
    lexemata = materia_valor_lista_appendere(p->piscina, lexemata,
        materia_valor_token(titulus));
    si (!materia_nodus_ponere(mala, (i32)CSS_MALUM_TOKENS, lexemata,
            MATERIA_LOCUS_LISTA_TOKEN))
    {
        redde NIHIL;
    }
    redde mala;
}

/* Recuperatio spec-definita: lexemata CRUDA usque ad ';' (inclusum)
 * aut '}' congruens (retrocessum) aut FINIS. Profunditas saeptorum
 * numeratur ne '}' nidificatum recuperationem claudat; FUNCTIO
 * quoque aperit (lexator 'ident(' unum lexema fudit). */
interior MateriaNodus*
_malam_consumere (
      CssParsura* p,
    MateriaToken* primum,
    MateriaToken* secundum)
{
    MateriaNodus* mala;
     MateriaValor lexemata;
              i32 profunditas;

    mala = materia_nodus_creare(p->piscina,
        (s32)CSS_GENUS_DECLARATIO_MALA, (i32)I);
    si (mala == NIHIL)
    {
        redde NIHIL;
    }
    lexemata = materia_valor_lista_nova(p->piscina);
    lexemata = materia_valor_lista_appendere(p->piscina, lexemata,
        materia_valor_token(primum));
    si (secundum != NIHIL)
    {
        lexemata = materia_valor_lista_appendere(p->piscina,
            lexemata, materia_valor_token(secundum));
    }
    profunditas = ZEPHYRUM;

    per (;;)
    {
        MateriaToken* token;

        token = _significans_proximum(p);
        si (token == NIHIL)
        {
            redde NIHIL;
        }
        si (_finis_est(p, token))
        {
            p->finis = token;
            frange;
        }
        si (   _clausum_congruens(token->genus) >= ZEPHYRUM
            || token->genus                     == (s32)CSS_LEX_FUNCTIO)
        {
            profunditas = profunditas + I;
        }
        alioquin si (   token->genus == (s32)CSS_LEX_PAREN_CLAUSA
                     || token->genus == (s32)CSS_LEX_QUADRA_CLAUSA)
        {
            si (profunditas > ZEPHYRUM)
            {
                profunditas = profunditas - I;
            }
        }
        alioquin si (token->genus == (s32)CSS_LEX_BRACE_CLAUSA)
        {
            si (profunditas > ZEPHYRUM)
            {
                profunditas = profunditas - I;
            }
            alioquin
            {
                _retrocedere(p);
                frange;
            }
        }
        lexemata = materia_valor_lista_appendere(p->piscina,
            lexemata, materia_valor_token(token));
        si (   token->genus == (s32)CSS_LEX_SEMICOLON
            && profunditas  == ZEPHYRUM)
        {
            frange;
        }
    }

    si (!materia_nodus_ponere(mala, (i32)CSS_MALUM_TOKENS, lexemata,
            MATERIA_LOCUS_LISTA_TOKEN))
    {
        redde NIHIL;
    }
    redde mala;
}

/* "Consume a declaration" (L3 5.4.5): titulus, ':', valor REGIMINE
 * CONTENTI usque ad ';' aut '}' (retrocessum) aut FINIS.
 *
 * PRAEVALENTIA ('!' + 'important') syntaxis est, non valor - ergo
 * lexemata eius trivia STRUCTURALIA ferunt: spatium medium
 * ('! important') ante lexematis 'important' fit, spatium sequens
 * post eius. Folia contenta trivia ferre non possunt (constrictum
 * proiectionis); lexemata praevalentiae possunt quia loci TOKEN
 * sunt, non servata. Confirmatio ante constructionem: nisi post
 * 'important' statim ';'/'}'/'FINIS' (spatio interposito licito)
 * sequatur, OMNIA folia contenta ordinaria fiunt - ordo octetorum
 * utroque tramite servatur quia nihil transponitur. */
interior MateriaNodus*
_declarationem_consumere (
      CssParsura* p,
    MateriaToken* titulus)
{
    MateriaNodus* decl;
    MateriaToken* token;
     MateriaValor valor;

    token = _significans_proximum(p);
    si (token == NIHIL)
    {
        redde NIHIL;
    }
    si (_finis_est(p, token))
    {
        /* titulus solus ante FINIS: mala uni-lexematis */
        p->finis = token;
        redde _malam_consumere_finita(p,
                (s32)CSS_GENUS_DECLARATIO_MALA, titulus);
    }
    si (token->genus != (s32)CSS_LEX_COLON)
    {
        si (token->genus == (s32)CSS_LEX_BRACE_CLAUSA)
        {
            _retrocedere(p);
            redde _malam_consumere_finita(p,
                (s32)CSS_GENUS_DECLARATIO_MALA, titulus);
        }
        redde _malam_consumere(p, titulus, token);
    }

    decl = materia_nodus_creare(p->piscina,
        (s32)CSS_GENUS_DECLARATIO, (i32)V);
    si (decl == NIHIL)
    {
        redde NIHIL;
    }
    si (   !materia_nodus_ponere(decl, (i32)CSS_DECL_TOK_NOMEN,
               materia_valor_token(titulus), MATERIA_LOCUS_TOKEN)
        || !materia_nodus_ponere(decl, (i32)CSS_DECL_TOK_COLON,
               materia_valor_token(token), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }

    valor         = materia_valor_lista_nova(p->piscina);
    p->contentum  = VERUM;
    token         = NIHIL;

    per (;;)
    {
        MateriaNodus* elementum;

        si (token == NIHIL)
        {
            token = _significans_proximum(p);
        }
        si (token == NIHIL)
        {
            p->contentum = FALSUM;
            redde NIHIL;
        }
        si (_finis_est(p, token))
        {
            p->finis = token;
            frange;
        }
        si (token->genus == (s32)CSS_LEX_SEMICOLON)
        {
            si (!materia_nodus_ponere(decl,
                    (i32)CSS_DECL_TOK_TERMINATOR,
                    materia_valor_token(token), MATERIA_LOCUS_TOKEN))
            {
                p->contentum = FALSUM;
                redde NIHIL;
            }
            frange;
        }
        si (token->genus == (s32)CSS_LEX_BRACE_CLAUSA)
        {
            _retrocedere(p);
            frange;
        }
        si (_est_delim_clamor(token))
        {
            MateriaToken* clamor = token;
            MateriaToken* medium = NIHIL;
            MateriaToken* verbum;
            MateriaToken* caudale = NIHIL;
            MateriaToken* proximum;

            verbum = _significans_proximum(p);
            si (verbum == NIHIL)
            {
                p->contentum = FALSUM;
                redde NIHIL;
            }
            si (verbum->genus == (s32)CSS_LEX_SPATIA)
            {
                medium = verbum;
                verbum = _significans_proximum(p);
                si (verbum == NIHIL)
                {
                    p->contentum = FALSUM;
                    redde NIHIL;
                }
            }
            si (_est_important(verbum))
            {
                proximum = _significans_proximum(p);
                si (proximum == NIHIL)
                {
                    p->contentum = FALSUM;
                    redde NIHIL;
                }
                si (proximum->genus == (s32)CSS_LEX_SPATIA)
                {
                    caudale   = proximum;
                    proximum  = _significans_proximum(p);
                    si (proximum == NIHIL)
                    {
                        p->contentum = FALSUM;
                        redde NIHIL;
                    }
                }
                si (   _finis_est(p, proximum)
                    || proximum->genus == (s32)CSS_LEX_SEMICOLON
                    || proximum->genus == (s32)CSS_LEX_BRACE_CLAUSA)
                {
                    /* CONFIRMATUM */
                    MateriaNodus* praevalentia;

                    praevalentia = materia_nodus_creare(p->piscina,
                        (s32)CSS_GENUS_PRAEVALENTIA, (i32)II);
                    si (praevalentia == NIHIL)
                    {
                        p->contentum = FALSUM;
                        redde NIHIL;
                    }
                    si (   medium != NIHIL
                        && !materia_token_trivia_ante_ponere(verbum,
                               p->piscina, &medium, (i32)I))
                    {
                        p->contentum = FALSUM;
                        redde NIHIL;
                    }
                    si (   caudale != NIHIL
                        && !materia_token_trivia_post_ponere(verbum,
                               p->piscina, &caudale, (i32)I))
                    {
                        p->contentum = FALSUM;
                        redde NIHIL;
                    }
                    si (   !materia_nodus_ponere(praevalentia,
                               (i32)CSS_PRAEVALENTIA_TOK_CLAMOR,
                               materia_valor_token(clamor),
                               MATERIA_LOCUS_TOKEN)
                        || !materia_nodus_ponere(praevalentia,
                               (i32)CSS_PRAEVALENTIA_TOK_VERBUM,
                               materia_valor_token(verbum),
                               MATERIA_LOCUS_TOKEN)
                        || !materia_nodus_ponere(decl,
                               (i32)CSS_DECL_PRAEVALENTIA,
                               materia_valor_nodus(praevalentia),
                               MATERIA_LOCUS_NODUS))
                    {
                        p->contentum = FALSUM;
                        redde NIHIL;
                    }
                    token = proximum;
                    perge;   /* ; aut } aut FINIS - ansa tractabit */
                }
                /* NON CONFIRMATUM: omnia folia contenta fiunt,
                 * ordine fontis - nihil transponitur. */
                si (!_folia_appendere(p, &valor, clamor, medium))
                {
                    redde NIHIL;
                }
                si (!_folia_appendere(p, &valor, verbum, caudale))
                {
                    redde NIHIL;
                }
                token = proximum;
                perge;
            }
            /* post '!' non 'important': folia contenta */
            si (!_folia_appendere(p, &valor, clamor, medium))
            {
                redde NIHIL;
            }
            token = verbum;
            perge;
        }

        elementum = _valorem_componentem_consumere(p, token);
        si (elementum == NIHIL)
        {
            p->contentum = FALSUM;
            redde NIHIL;
        }
        valor = materia_valor_lista_appendere(p->piscina, valor,
            materia_valor_nodus(elementum));
        token = NIHIL;
    }

    p->contentum = FALSUM;
    si (!materia_nodus_ponere(decl, (i32)CSS_DECL_VALOR, valor,
            MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    redde decl;
}

interior MateriaNodus*
_regulam_apud_consumere (
      CssParsura* p,
    MateriaToken* titulus);

/* "Consume a style block's contents" (L3 5.4.4): declarationes,
 * regulae apud nidificatae (@page { @top-left {...} }), ';'
 * errantia (L3 ea ABICIT - nos folia servata facimus, sine
 * iactura), recuperatio. */
interior MateriaNodus*
_corpus_regulae_consumere (
      CssParsura* p,
    MateriaToken* apertum)
{
    MateriaNodus* saeptum;
     MateriaValor contentum;

    saeptum = materia_nodus_creare(p->piscina,
        (s32)CSS_GENUS_SAEPTUM, (i32)III);
    si (saeptum == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(saeptum, (i32)CSS_SAEPTUM_TOK_APERTUM,
            materia_valor_token(apertum), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    contentum = materia_valor_lista_nova(p->piscina);

    per (;;)
    {
        MateriaToken* token;
        MateriaNodus* elementum;

        token = _significans_proximum(p);
        si (token == NIHIL)
        {
            redde NIHIL;
        }
        si (_finis_est(p, token))
        {
            p->finis = token;
            frange;
        }
        si (token->genus == (s32)CSS_LEX_BRACE_CLAUSA)
        {
            si (!materia_nodus_ponere(saeptum,
                    (i32)CSS_SAEPTUM_TOK_CLAUSUM,
                    materia_valor_token(token), MATERIA_LOCUS_TOKEN))
            {
                redde NIHIL;
            }
            frange;
        }
        si (token->genus == (s32)CSS_LEX_SEMICOLON)
        {
            elementum = _servatum_creare(p, token);
        }
        alioquin si (token->genus == (s32)CSS_LEX_APUD_VERBUM)
        {
            elementum = _regulam_apud_consumere(p, token);
        }
        alioquin si (token->genus == (s32)CSS_LEX_IDENTIFICATOR)
        {
            elementum = _declarationem_consumere(p, token);
        }
        alioquin
        {
            elementum = _malam_consumere(p, token, NIHIL);
        }
        si (elementum == NIHIL)
        {
            redde NIHIL;
        }
        contentum = materia_valor_lista_appendere(p->piscina,
            contentum, materia_valor_nodus(elementum));
        si (p->finis != NIHIL)
        {
            frange;
        }
    }

    si (!materia_nodus_ponere(saeptum, (i32)CSS_SAEPTUM_CONTENTUM,
            contentum, MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    redde saeptum;
}


/* ==================================================
 * Regulae apud (L3 5.4.2, B4)
 * ================================================== */

interior MateriaNodus*
_regulam_qualificatam_consumere (
      CssParsura* p,
    MateriaToken* primum);

interior MateriaNodus*
_regulam_apud_consumere (
      CssParsura* p,
    MateriaToken* titulus);

/* Corpus regulae apud tribus classibus (spec par. IV): regulas
 * fert (@media), declarationes fert (@font-face), aut sententia
 * sola est (@import). Ignota -> saeptum genericum valorum
 * componentium, quod totale et honestum est. CRESCERE = ordinem
 * tabulae docere, NUMQUAM re-parsare. */
nomen enumeratio {
    CSS_APUD_IGNOTA = 0,
    CSS_APUD_REGULIS,
    CSS_APUD_DECLARATIONIBUS,
    CSS_APUD_SENTENTIA
} CssApudClassis;

nomen structura {
    constans character* titulus;
        CssApudClassis  classis;
} ApudNota;

hic_manens constans ApudNota APUD_TABULA[] = {
    { "@media",         CSS_APUD_REGULIS },
    { "@supports",      CSS_APUD_REGULIS },
    { "@document",      CSS_APUD_REGULIS },
    { "@layer",         CSS_APUD_REGULIS },
    { "@font-face",     CSS_APUD_DECLARATIONIBUS },
    { "@page",          CSS_APUD_DECLARATIONIBUS },
    { "@property",      CSS_APUD_DECLARATIONIBUS },
    { "@counter-style", CSS_APUD_DECLARATIONIBUS },
    { "@top-left",      CSS_APUD_DECLARATIONIBUS },
    { "@import",        CSS_APUD_SENTENTIA },
    { "@charset",       CSS_APUD_SENTENTIA },
    { "@namespace",     CSS_APUD_SENTENTIA }
};

/* ASCII sine casu, chorda contra litteras (verba apud L3 casum
 * non curant). */
interior b32
_chorda_aequalis_sine_casu (
       constans chorda* valor,
    constans character* literis)
{
    i32 i;

    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        character a = (character)valor->datum[i];
        character b = literis[i];

        si (b == '\0')
        {
            redde FALSUM;
        }
        si (a >= 'A' && a <= 'Z')
        {
            a = (character)(a + XXXII);
        }
        si (a != b)
        {
            redde FALSUM;
        }
    }
    redde (b32)(literis[valor->mensura] == '\0');
}

interior CssApudClassis
_apud_classis (
    constans MateriaToken* token)
{
    i32 numerus = (i32)(magnitudo(APUD_TABULA)
                      / magnitudo(APUD_TABULA[0]));
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (_chorda_aequalis_sine_casu(&token->valor,
                APUD_TABULA[i].titulus))
        {
            redde APUD_TABULA[i].classis;
        }
    }
    redde CSS_APUD_IGNOTA;
}

/* Corpus regularum (@media): regulae qualificatae et apud intra
 * saeptum; ';' errantia et CDO/CDC folia servata fiunt. */
interior MateriaNodus*
_corpus_regularum_consumere (
      CssParsura* p,
    MateriaToken* apertum)
{
    MateriaNodus* saeptum;
     MateriaValor contentum;

    saeptum = materia_nodus_creare(p->piscina,
        (s32)CSS_GENUS_SAEPTUM, (i32)III);
    si (saeptum == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(saeptum, (i32)CSS_SAEPTUM_TOK_APERTUM,
            materia_valor_token(apertum), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    contentum = materia_valor_lista_nova(p->piscina);

    per (;;)
    {
        MateriaToken* token;
        MateriaNodus* elementum;

        token = _significans_proximum(p);
        si (token == NIHIL)
        {
            redde NIHIL;
        }
        si (_finis_est(p, token))
        {
            p->finis = token;
            frange;
        }
        si (token->genus == (s32)CSS_LEX_BRACE_CLAUSA)
        {
            si (!materia_nodus_ponere(saeptum,
                    (i32)CSS_SAEPTUM_TOK_CLAUSUM,
                    materia_valor_token(token), MATERIA_LOCUS_TOKEN))
            {
                redde NIHIL;
            }
            frange;
        }
        si (   token->genus == (s32)CSS_LEX_SEMICOLON
            || token->genus == (s32)CSS_LEX_CDO
            || token->genus == (s32)CSS_LEX_CDC)
        {
            elementum = _servatum_creare(p, token);
        }
        alioquin si (token->genus == (s32)CSS_LEX_APUD_VERBUM)
        {
            elementum = _regulam_apud_consumere(p, token);
        }
        alioquin
        {
            elementum = _regulam_qualificatam_consumere(p, token);
        }
        si (elementum == NIHIL)
        {
            redde NIHIL;
        }
        contentum = materia_valor_lista_appendere(p->piscina,
            contentum, materia_valor_nodus(elementum));
        si (p->finis != NIHIL)
        {
            frange;
        }
    }

    si (!materia_nodus_ponere(saeptum, (i32)CSS_SAEPTUM_CONTENTUM,
            contentum, MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    redde saeptum;
}

/* "Consume an at-rule" (L3 5.4.2): praeludium CONTENTUM usque ad
 * ';' (terminator - sententia) aut '{' (corpus per classem) aut
 * '}' patris (retrocessum) aut FINIS. */
interior MateriaNodus*
_regulam_apud_consumere (
      CssParsura* p,
    MateriaToken* titulus)
{
    MateriaNodus*  regula;
    MateriaToken*  token;
     MateriaValor  praeludium;
    CssApudClassis classis;

    regula = materia_nodus_creare(p->piscina,
        (s32)CSS_GENUS_REGULA_APUD, (i32)IV);
    si (regula == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(regula, (i32)CSS_APUD_TOK_NOMEN,
            materia_valor_token(titulus), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    classis       = _apud_classis(titulus);
    praeludium    = materia_valor_lista_nova(p->piscina);
    p->contentum  = VERUM;

    per (;;)
    {
        MateriaNodus* valor;

        token = _significans_proximum(p);
        si (token == NIHIL)
        {
            p->contentum = FALSUM;
            redde NIHIL;
        }
        si (_finis_est(p, token))
        {
            p->finis = token;
            frange;
        }
        si (token->genus == (s32)CSS_LEX_SEMICOLON)
        {
            si (!materia_nodus_ponere(regula,
                    (i32)CSS_APUD_TOK_TERMINATOR,
                    materia_valor_token(token), MATERIA_LOCUS_TOKEN))
            {
                p->contentum = FALSUM;
                redde NIHIL;
            }
            frange;
        }
        si (token->genus == (s32)CSS_LEX_BRACE_CLAUSA)
        {
            _retrocedere(p);
            frange;
        }
        si (token->genus == (s32)CSS_LEX_BRACE_APERTA)
        {
            MateriaNodus* corpus;

            p->contentum = FALSUM;
            si (classis == CSS_APUD_REGULIS)
            {
                corpus = _corpus_regularum_consumere(p, token);
            }
            alioquin si (classis == CSS_APUD_DECLARATIONIBUS)
            {
                corpus = _corpus_regulae_consumere(p, token);
            }
            alioquin
            {
                corpus = _saeptum_consumere(p, token);
            }
            si (corpus == NIHIL)
            {
                redde NIHIL;
            }
            si (!materia_nodus_ponere(regula, (i32)CSS_APUD_CORPUS,
                    materia_valor_nodus(corpus), MATERIA_LOCUS_NODUS))
            {
                redde NIHIL;
            }
            frange;
        }
        valor = _valorem_componentem_consumere(p, token);
        si (valor == NIHIL)
        {
            p->contentum = FALSUM;
            redde NIHIL;
        }
        praeludium = materia_valor_lista_appendere(p->piscina,
            praeludium, materia_valor_nodus(valor));
        si (p->finis != NIHIL)
        {
            frange;
        }
    }

    p->contentum = FALSUM;
    si (!materia_nodus_ponere(regula, (i32)CSS_APUD_PRAELUDIUM,
            praeludium, MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    redde regula;
}


/* ==================================================
 * Regula qualificata (L3 5.4.3)
 * ================================================== */

/* Praeludium REGIMINE CONTENTI (D7 auctum, vide caput) usque ad
 * '{'; corpus saeptum REGIMINE STRUCTURALI (B3 valores
 * declarationum contentos faciet).
 *
 * FINIS in praeludio: regula cum corpore ABSENTE redditur - octeti
 * servantur. B4 recuperationem spec-definitam (regula-mala)
 * adferet; hoc interim TOTALITAS est, non forma finalis. */
interior MateriaNodus*
_regulam_qualificatam_consumere (
      CssParsura* p,
    MateriaToken* primum)
{
    MateriaNodus* regula;
    MateriaToken* token;
     MateriaValor praeludium;

    regula = materia_nodus_creare(p->piscina,
        (s32)CSS_GENUS_REGULA_QUALIFICATA, (i32)II);
    si (regula == NIHIL)
    {
        redde NIHIL;
    }
    praeludium    = materia_valor_lista_nova(p->piscina);
    token         = primum;
    p->contentum  = VERUM;

    per (;;)
    {
        MateriaNodus* valor;

        si (token->genus == (s32)CSS_LEX_BRACE_APERTA)
        {
            MateriaNodus* corpus;

            p->contentum  = FALSUM;
            corpus        = _corpus_regulae_consumere(p, token);
            si (corpus == NIHIL)
            {
                redde NIHIL;
            }
            si (!materia_nodus_ponere(regula, (i32)CSS_REGULA_CORPUS,
                    materia_valor_nodus(corpus), MATERIA_LOCUS_NODUS))
            {
                redde NIHIL;
            }
            frange;
        }

        valor = _valorem_componentem_consumere(p, token);
        si (valor == NIHIL)
        {
            p->contentum = FALSUM;
            redde NIHIL;
        }
        praeludium = materia_valor_lista_appendere(p->piscina,
            praeludium, materia_valor_nodus(valor));
        si (p->finis != NIHIL)
        {
            frange;
        }

        token = _significans_proximum(p);
        si (token == NIHIL)
        {
            p->contentum = FALSUM;
            redde NIHIL;
        }
        si (_finis_est(p, token))
        {
            p->finis = token;
            frange;
        }
    }

    p->contentum = FALSUM;
    si (!materia_nodus_ponere(regula, (i32)CSS_REGULA_PRAELUDIUM,
            praeludium, MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    redde regula;
}


/* ==================================================
 * Ingressus
 * ================================================== */

MateriaNodus*
css_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
              CssParsura p;
      MateriaLexIudicium iudicium;
                    Xar* lexemata;
           MateriaNodus* plagula;
           MateriaToken* token;

    si (piscina == NIHIL) { redde NIHIL; }

    /* ZEPHYRUM PRIMUM (vide css_ligator_incipere pro ratione). */
    memset(&p, ZEPHYRUM, magnitudo(CssParsura));
    p.piscina = piscina;

    /* PORTA ONERIS, politica degradationis B. Recusatio hic vitium
     * TABULAE NOSTRAE nominat, non fontis: fons quilibet licet
     * (parsator totalis est), tabula non. */
    si (!materia_lexicon_ratum_facere(&p.lexicon, &CSS_LEXICON,
            &iudicium))
    {
        redde NIHIL;
    }

    lexemata = css_lexare(piscina, fons, mensura);
    si (lexemata == NIHIL) { redde NIHIL; }
    p.plana = css_adaptare(piscina, lexemata, NIHIL);
    si (p.plana == NIHIL) { redde NIHIL; }
    css_ligator_incipere(&p.ligator, piscina, &p.lexicon);

    plagula = materia_nodus_creare(piscina, (s32)CSS_GENUS_PLAGULA,
        (i32)II);
    si (plagula == NIHIL) { redde NIHIL; }

    /* LISTA VACUA PONITUR CONSULTO. Locus non positus VALOR_NIHIL
     * manet, quem uterque scriptor TACITE OMITTIT
     * (materia_scribere.c:379, materia_arbor.c:1185). Consumens
     * ergo duas formas tractare deberet - 'lista vacua' et 'locus
     * numquam impletus' - quae idem significant. Una sufficit. */
    si (!materia_nodus_ponere(plagula, (i32)CSS_PLAGULA_REGULAE,
            materia_valor_lista_nova(piscina),
            MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }

    /* "consume a list of rules" (L3 5.4.1). CDO/CDC legalia et a
     * L3 ignorata - folia servata fiunt (sine iactura). '}'
     * supremum error est (schema recentius; CR vetus id praeludio
     * dabat): regula-mala UNI-lexematis, ut error localis maneat
     * nec regulam sequentem contaminet. */
    per (;;)
    {
        MateriaNodus* regula;

        token = _significans_proximum(&p);
        si (token == NIHIL) { redde NIHIL; }
        si (_finis_est(&p, token)) { frange; }

        si (token->genus == (s32)CSS_LEX_APUD_VERBUM)
        {
            regula = _regulam_apud_consumere(&p, token);
        }
        alioquin si (   token->genus == (s32)CSS_LEX_CDO
                     || token->genus == (s32)CSS_LEX_CDC)
        {
            regula = _servatum_creare(&p, token);
        }
        alioquin si (token->genus == (s32)CSS_LEX_BRACE_CLAUSA)
        {
            regula = _malam_consumere_finita(&p,
                (s32)CSS_GENUS_REGULA_MALA, token);
        }
        alioquin
        {
            regula = _regulam_qualificatam_consumere(&p, token);
        }
        si (regula == NIHIL) { redde NIHIL; }
        si (!materia_nodus_appendere(piscina, plagula,
                (i32)CSS_PLAGULA_REGULAE, materia_valor_nodus(regula),
                MATERIA_LOCUS_LISTA_NODUS))
        {
            redde NIHIL;
        }
        si (p.finis != NIHIL)
        {
            token = p.finis;
            frange;
        }
    }

    /* Patres figere: politica reconstructionis materiae - lector
     * patres omnes ponit, ergo parsator quoque (aliter comparator
     * B1 falso divergeret: nodus/pater-nullitas). */
    materia_arbor_patres_figere(piscina, plagula);

    /* CAUDA: lexema FINIS spatia caudalia plagulae ut 'ante' fert
     * (spec par. VII). Dominus unus servatus, nulla sectio nova
     * documenti, nulla mutatio scriptoris. */
    si (!materia_nodus_ponere(plagula, (i32)CSS_PLAGULA_CAUDA,
            materia_valor_token(token), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }

    redde plagula;
}
