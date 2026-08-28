/* css_arbor.c - Vide css_arbor.h.
 *
 * SPINA SOLA (consilium T9). Regulae T10 veniunt, declarationes
 * T11, regulae-apud T12.
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
     MateriaLexiconRatum lexicon;
                    Xar* plana;    /* lexemata, unum pro uno */
                    i32  index;    /* positio in plana */
              CssLigator ligator;
} CssParsura;


/* Lexema significans proximum, REGIMINE STRUCTURALI: trivia
 * cumulantur et ligantur, significans redditur.
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

        token = *(MateriaToken**)xar_obtinere(p->plana, p->index);
        p->index = p->index + I;

        si (materia_lexicon_trivium_est(&p->lexicon, token->genus))
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
    constans CssParsura* p,
    constans MateriaToken* token)
{
    redde (b32)(materia_lexicon_munus(&p->lexicon, token->genus)
                == MATERIA_MUNUS_FINIS);
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

    /* "consume a list of rules" (L3 5.4.1) */
    per (;;)
    {
        token = _significans_proximum(&p);
        si (token == NIHIL) { redde NIHIL; }
        si (_finis_est(&p, token)) { frange; }

        /* NONDUM IMPLETUM (T10). Lexema hic CADIT, CONSULTO: porta
         * tegumenti octetorum ergo RUBRA manet pro fonte quovis
         * regulam ferente, quod est id quod portam PORTAM facit.
         * Recuperatio tacita in 'regula-mala' portam viridem
         * faceret dum nihil revera parsatum esset. */
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
