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
 * NIHIL significat fluxum SINE lexemate FINIS, quod vitium
 * css_lexare esset, non fontis - css_lexema.h FINIS ultimum
 * promittit. Ergo ansa vocantis semper terminatur. */
interior MateriaToken*
_significans_proximum (
    CssParsura* p)
{
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
            corpus        = _saeptum_consumere(p, token);
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

    /* "consume a list of rules" (L3 5.4.1). B2: omne lexema
     * significans regulam QUALIFICATAM incipit - regulae apud,
     * '}' supremum, CDO/CDC recuperatioque B4 sunt. */
    per (;;)
    {
        MateriaNodus* regula;

        token = _significans_proximum(&p);
        si (token == NIHIL) { redde NIHIL; }
        si (_finis_est(&p, token)) { frange; }

        regula = _regulam_qualificatam_consumere(&p, token);
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
