/* css_adaptare.c - Vide css_adaptare.h pro ratione divisionis. */

#include "css_adaptare.h"
#include "css_lexema.h"
#include "chorda.h"
#include <string.h>


/* ==================================================
 * Adaptatio
 * ================================================== */

/* VEXILLUM 'initium_lineae' CONSULTO NON PONITUR.
 *
 * Silva id ponit quia lexema NOVA_LINEA habet
 * (silva_lexema.c:888): 'primum significans post lineam logicam'.
 * CSS munus LINEA OMNINO CARET - lineae novae intra SPATIA
 * absorbentur - ergo signum illud lingua nostra dicere non potest.
 *
 * ET SI PONERETUR, CIRCUITUM NON SUPERARET. materia_arbor.c:908
 * attributa sedis (b, linea, columna, linea-initium) solum pro
 * lexemate DERIVATO scribit (si !sedes.est_fons). CSS derivationem
 * non habet, ergo omne lexema fons est, ergo documentum
 * 'linea-initium' NUMQUAM fert; lector eam ex cursore reficit
 * (materia_arbor.c:2305), et cursor per munus LINEA SOLUM movetur.
 * Vexillum ergo ex arbore lecta ABESSET.
 *
 * Notandum quod discrepantia illa OCTETIM INVISIBILIS esset:
 * documentum eam non fert, ergo rescriptio idem redderet. Solum
 * COMPARATOR ARBORUM eam videret - quae est ipsa causa cur
 * arbor_aequalitas (nondum portata) genus probationis ALIUD sit,
 * non idem aliter. */

Xar*
css_adaptare (
                       Piscina* piscina,
                           Xar* lexemata_css,
    constans MateriaTokenForma* forma)
{
    Xar* exitus;
    i32  numerus;
    i32  i;

    si (piscina == NIHIL || lexemata_css == NIHIL)
    { redde NIHIL;
    }

    numerus  = xar_numerus(lexemata_css);
    exitus   = xar_creare(piscina, magnitudo(MateriaToken*));
    si (exitus == NIHIL)
    { redde NIHIL;
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        constans CssLexema* lexema;
              MateriaToken* token;
             MateriaToken** locus;

        lexema = (constans CssLexema*)xar_obtinere(lexemata_css, i);
        si (lexema == NIHIL)
        { redde NIHIL;
        }

        /* fons_index ZEPHYRUM: CSS fontem UNUM habet (spec par. II).
         * materia eum opacum tenet - aequalitas et -I solum. */
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
css_ligator_incipere (
                      CssLigator* ligator,
                         Piscina* piscina,
    constans MateriaLexiconRatum* lexicon)
{
    si (ligator == NIHIL)
    { redde;
    }

    /* ZEPHYRUM PRIMUM, deinde campi non-zephyri. Constructor qui
     * campos ENUMERAT laqueus est: campus additus INCONDITUS
     * advenit, et punctator inconditus vagus est quam primum
     * quisquam eum legit. Id materiam 2026-08-27 momordit
     * (EXC_BAD_ACCESS, campus 'lacunae' consilio additus, nudo
     * non). */
    memset(ligator, ZEPHYRUM, magnitudo(CssLigator));

    ligator->piscina = piscina;
    ligator->lexicon = lexicon;
    si (piscina != NIHIL)
    {
        ligator->pendentia = xar_creare(piscina,
            magnitudo(MateriaToken*));
    }
}

b32
css_ligator_cumulare (
      CssLigator* ligator,
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

/* An trivium terminatorem lineae CONTINEAT. CSS praeprocessionem
 * non agit (css_lexema.h): \r\n, \r solum, \f verbatim manent,
 * ergo omnia tria quaerenda sunt - non \n solum. */
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
css_ligator_solvere (
      CssLigator* ligator,
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

    /* XAR SEGMENTATUS EST (xar.h:59, 'segmenta[]'), ergo elementa
     * eius contigua NON SUNT et xar_obtinere ut initium seriei
     * tradi NEQUIT. materia_token_trivia_*_ponere seriem contiguam
     * copiat; ergo in tabulam planam prius transferimus. */
    plana = (MateriaToken**)piscina_allocare_ordinatum(
        ligator->piscina,
        (memoriae_index)magnitudo(MateriaToken*) * (memoriae_index)numerus,
        (memoriae_index)magnitudo(MateriaToken*));
    si (plana == NIHIL)
    { redde FALSUM;
    }

    per (j = ZEPHYRUM; j < numerus; j++)
    {
        plana[j] = *(MateriaToken**)xar_obtinere(ligator->pendentia, j);
    }

    /* DIVISIO - vide capitis rationem.
     *   sine priore    -> ZEPHYRUM, omnia SEQUENTI (initium plagulae)
     *   sine linea nova-> numerus,  omnia PRIORI  (cauda lineae)
     *   aliter         -> j, index primi SPATII lineam novam ferentis */
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
