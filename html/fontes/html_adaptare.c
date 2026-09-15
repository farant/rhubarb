/* html_adaptare.c - Vide html_adaptare.h pro ratione divisionis. */

#include "html_adaptare.h"
#include "html_lexema.h"
#include "chorda.h"
#include <string.h>


/* ==================================================
 * Adaptatio
 * ================================================== */

/* VEXILLUM 'initium_lineae' CONSULTO NON PONITUR: HTML munus LINEA
 * caret (SPATIA lineas novas intra tag absorbet, extra tags lineae
 * novae TEXTUS sunt), ergo signum illud lingua dicere non potest; et
 * si poneretur circuitum non superaret - materia_arbor.c attributa
 * sedis lexemati DERIVATO soli scribit, quod HTML numquam habet.
 * Ratio plena in css_adaptare.c. */

Xar*
html_adaptare (
                       Piscina* piscina,
                           Xar* lexemata_html,
    constans MateriaTokenForma* forma)
{
    Xar* exitus;
    i32  numerus;
    i32  i;

    si (piscina == NIHIL || lexemata_html == NIHIL)
    { redde NIHIL;
    }

    numerus  = xar_numerus(lexemata_html);
    exitus   = xar_creare(piscina, magnitudo(MateriaToken*));
    si (exitus == NIHIL)
    { redde NIHIL;
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        constans HtmlLexema* lexema;
               MateriaToken* token;
              MateriaToken** locus;

        lexema = (constans HtmlLexema*)xar_obtinere(lexemata_html, i);
        si (lexema == NIHIL)
        { redde NIHIL;
        }

        /* fons_index ZEPHYRUM: HTML fontem UNUM habet. */
        token = materia_token_creare(piscina, forma,
            (s32)lexema->genus, lexema->valor, (s32)lexema->offset,
            lexema->linea, lexema->columna, ZEPHYRUM);
        si (token == NIHIL)
        { redde NIHIL;
        }

        locus = (MateriaToken**)xar_addere(exitus);
        si (locus == NIHIL)
        { redde NIHIL;
        }
        *locus = token;
    }

    redde exitus;
}


/* ==================================================
 * Ligator
 * ================================================== */

vacuum
html_ligator_incipere (
                      HtmlLigator* ligator,
                          Piscina* piscina,
     constans MateriaLexiconRatum* lexicon)
{
    si (ligator == NIHIL)
    { redde;
    }

    /* ZEPHYRUM PRIMUM, deinde campi non-zephyri (lectio materiae
     * 2026-08-27: campus additus inconditus punctator vagus est). */
    memset(ligator, ZEPHYRUM, magnitudo(HtmlLigator));

    ligator->piscina = piscina;
    ligator->lexicon = lexicon;
    si (piscina != NIHIL)
    {
        ligator->pendentia = xar_creare(piscina,
            magnitudo(MateriaToken*));
    }
}

b32
html_ligator_cumulare (
     HtmlLigator* ligator,
    MateriaToken* trivium)
{
    MateriaToken** locus;

    si (   ligator == NIHIL || ligator->pendentia == NIHIL
        || trivium == NIHIL)
    {
        redde FALSUM;
    }

    locus = (MateriaToken**)xar_addere(ligator->pendentia);
    si (locus == NIHIL)
    { redde FALSUM;
    }
    *locus = trivium;
    redde VERUM;
}

/* An trivium terminatorem lineae CONTINEAT. Lexator praeprocessionem
 * non agit: \r\n, \r solum, \f verbatim manent - omnia tria
 * quaerenda sunt. */
interior b32
_terminatorem_lineae_fert (
    constans MateriaToken* trivium)
{
    i32 i;

    si (trivium->valor.datum == NIHIL)
    { redde FALSUM;
    }

    per (i = ZEPHYRUM; i < trivium->valor.mensura; i++)
    {
        i8 octetus = trivium->valor.datum[i];
        si (octetus == '\n' || octetus == '\r' || octetus == '\f')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
html_ligator_solvere (
     HtmlLigator* ligator,
    MateriaToken* sequens)
{
    MateriaToken** plana;
    i32 numerus;
    i32 divisio;
    i32 j;

    si (   ligator          == NIHIL || ligator->pendentia == NIHIL
        || ligator->lexicon == NIHIL || sequens == NIHIL)
    {
        redde FALSUM;
    }

    numerus = xar_numerus(ligator->pendentia);
    si (numerus == ZEPHYRUM)
    {
        ligator->prior = sequens;
        redde VERUM;
    }

    /* XAR SEGMENTATUS EST: elementa contigua NON sunt, ergo in
     * tabulam planam prius transferimus (materia_token_trivia_*
     * seriem contiguam copiant). */
    plana = (MateriaToken**)piscina_allocare_ordinatum(
        ligator->piscina,
        (memoriae_index)magnitudo(MateriaToken*)
            * (memoriae_index)numerus,
        (memoriae_index)magnitudo(MateriaToken*));
    si (plana == NIHIL)
    { redde FALSUM;
    }

    per (j = ZEPHYRUM; j < numerus; j++)
    {
        plana[j] = *(MateriaToken**)xar_obtinere(ligator->pendentia, j);
    }

    /* DIVISIO - vide caput.
     *   sine priore     -> ZEPHYRUM, omnia SEQUENTI
     *   sine linea nova -> numerus,  omnia PRIORI
     *   aliter          -> j, primum SPATIUM lineam novam ferens */
    divisio = ZEPHYRUM;
    si (ligator->prior != NIHIL)
    {
        divisio = numerus;
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (materia_lexicon_munus(ligator->lexicon, plana[j]->genus)
                    == MATERIA_MUNUS_SPATIUM
                && _terminatorem_lineae_fert(plana[j]))
            {
                divisio = j;
                frange;
            }
        }
    }

    si (divisio > ZEPHYRUM)
    {
        si (!materia_token_trivia_post_ponere(ligator->prior,
                ligator->piscina, plana, divisio))
        {
            redde FALSUM;
        }
    }
    si (divisio < numerus)
    {
        si (!materia_token_trivia_ante_ponere(sequens,
                ligator->piscina, plana + divisio, numerus - divisio))
        {
            redde FALSUM;
        }
    }

    xar_vacare(ligator->pendentia);
    ligator->prior = sequens;
    redde VERUM;
}
