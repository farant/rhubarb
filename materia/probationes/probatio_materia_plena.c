/* probatio_materia_plena.c - Sequentia plena: gradus I + II, una
 * collectio annotationum, una excusatio
 * (project-specs/exemplaria-c-spec.md par. III)
 *
 * Arbor manu aedificata cum lexico cuius genus COMMENTUM munus suum
 * fert, et registro cuius genus unum diagnosticum DECLARAT. Regula
 * STML gradum II dat. Ita utrumque gradum in arbore UNA habemus -
 * quod est ipsum quod 'una parsura ambobus servit' significat.
 *
 * FIXTURA DISPAR: inventum gradus I in nodo priore, gradus II in
 * posteriore, et excusatio alterum SOLUM tegit. Si ambo excusarentur,
 * porta 'unum cadit, alterum manet' discernere non posset.
 *
 * OCTETI COHAERENT, ET HOC TER IAM MOMORDIT: malum 100-102,
 * commentarium 103-147, verbum 150-155. Forma prior commentarium ad
 * octetum XL ponebat dum nodi a C incipiebant, ergo ordo octetorum
 * RETRO ibat et 'materia:ordo-octetorum' ordinem tertium peperit -
 * inventum verum, fixtura falsa.
 */

#include "latina.h"
#include "credo.h"
#include "materia_diagnostica.h"
#include "materia_exemplaria.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_lexicon.h"
#include "materia_registrum.h"
#include "internamentum.h"
#include "chorda.h"
#include "piscina.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

enumeratio {
    G_FINIS = 0, G_IDENT, G_SPATIA, G_LINEA, G_COMMENTUM,
    G_NUMERUS_GENERUM
};

hic_manens constans MateriaLexGenus GENERA[] = {
    { "FINIS",     "",    MATERIA_LEX_FIXUM,      MATERIA_MUNUS_FINIS },
    { "IDENT",     NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SPATIA",    " ",   MATERIA_LEX_REPETITUM,
        MATERIA_MUNUS_SPATIUM },
    { "LINEA",     "\n",  MATERIA_LEX_TERMINATOR, MATERIA_MUNUS_LINEA },
    { "COMMENTUM", NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_COMMENTUM }
};
hic_manens constans MateriaLexiconCoctum LEXICON = {
    GENERA, (i32)G_NUMERUS_GENERUM, "lex-", (s32)G_SPATIA
};

enumeratio { GR_RADIX = 0, GR_MALUM, GR_VERBUM };

hic_manens constans MateriaTabLocus LOCI[] = {
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok",    (s32)MATERIA_LOCUS_TOKEN },
    { "tok",    (s32)MATERIA_LOCUS_TOKEN }
};
hic_manens constans MateriaTabGenus GEN[] = {
    { "radix",  ZEPHYRUM, (i32)I },
    { "malum",  (i32)I,   (i32)I },
    { "verbum", (i32)II,  (i32)I }
};
hic_manens constans MateriaRegistrumCoctum REG = {
    GEN, (i32)III, LOCI, (i32)III
};

/* gradus I: genus 'malum' diagnosticum DECLARAT */
hic_manens constans MateriaTabDiagnosticum TABULA[] = {
    { (s32)GR_MALUM, (s32)-I, (s32)MATERIA_DIAGNOSTICUM_GENUS,
      (s32)MATERIA_GRAVITAS_ERRATUM, "malum", "lexema malum" }
};
hic_manens constans MateriaDiagnosticaCocta DECLARATA = {
    TABULA, (i32)I, NIHIL, ZEPHYRUM
};

/* gradus II: regula quae verbum quodque notat */
hic_manens constans character* REGULA_TEXTUS =
    "<EXEMPLAR output=\"$v\"><verbum $t/></EXEMPLAR>"
    "<relatum lint=\"omne-verbum\">"
      "<PER congruentia=\"$v\"><situs>&@t;</situs></PER>"
    "</relatum>";

hic_manens MateriaToken*
_lex (
               Piscina* piscina,
                   s32  genus,
    constans character* valor,
                   s32  offset)
{
    redde materia_token_creare(piscina, &FORMA, genus,
        chorda_ex_literis(valor, piscina), offset, (i32)I,
        (i32)(offset + I), ZEPHYRUM);
}

/* nodus unius lexematis, cum commentario optionali ANTE eum */
hic_manens MateriaNodus*
_nodus (
               Piscina* piscina,
                   s32  genus,
    constans character* valor,
                   s32  offset,
    constans character* commentarium,
                   s32  commentarii_offset)
{
    MateriaNodus* n = materia_nodus_creare(piscina, genus, (i32)I);
    MateriaToken* t;

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    t = _lex(piscina, (s32)G_IDENT, valor, offset);
    si (commentarium != NIHIL)
    {
        MateriaToken* nota = _lex(piscina, (s32)G_COMMENTUM,
                                 commentarium, commentarii_offset);

        si (!materia_token_trivia_ante_ponere(t, piscina, &nota,
                (i32)I))
        {
            redde NIHIL;
        }
    }
    si (!materia_nodus_ponere(n, ZEPHYRUM, materia_valor_token(t),
            MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde n;
}

integer
principale (
    vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;
    InternamentumChorda* intern;
           StmlResultus  r;
MateriaDiagnosticaRatio  ratio;
                    Xar* regulae;

    piscina = piscina_generare_dynamicum("probatio_plena", 1048576);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &LEXICON,
        &iudicium));
    intern = internamentum_creare(piscina);
    CREDO_NON_NIHIL (intern);
    r = stml_legere_ex_literis(REGULA_TEXTUS, piscina, intern);
    CREDO_VERUM (r.successus);
    regulae = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    CREDO_NON_NIHIL (regulae);
    {
        StmlNodus** cella = (StmlNodus**)xar_addere(regulae);

        CREDO_NON_NIHIL (cella);
        si (cella != NIHIL)
        {
            *cella = r.radix;
        }
    }

    memset(&ratio, ZEPHYRUM, magnitudo(ratio));
    ratio.grammatica  = "proba";
    ratio.tabularium  = &REG;
    ratio.lexicon     = &ratum;
    ratio.declarata   = &DECLARATA;
    ratio.praefixum   = "#";
    ratio.regulae     = regulae;
    ratio.intern      = intern;

    {
        MateriaNodus* radix = materia_nodus_creare(piscina,
                                  (s32)GR_RADIX, (i32)I);
                 Xar* d;

        imprimere("\n--- I. Gradus AMBO in cursu uno ---\n");
        CREDO_NON_NIHIL (radix);
        CREDO_VERUM (materia_nodus_appendere(piscina, radix, ZEPHYRUM,
            materia_valor_nodus(_nodus(piscina, (s32)GR_MALUM, "@@",
                (s32)100, NIHIL, ZEPHYRUM)),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_appendere(piscina, radix, ZEPHYRUM,
            materia_valor_nodus(_nodus(piscina, (s32)GR_VERBUM, "alpha",
                (s32)110, NIHIL, ZEPHYRUM)),
            MATERIA_LOCUS_LISTA_NODUS));
        d = materia_diagnostica_plena(piscina, radix, &ratio, NIHIL);
        CREDO_NON_NIHIL (d);
        /* gradus I 'malum' + gradus II 'omne-verbum' = DUO */
        CREDO_AEQUALIS_I32 (xar_numerus(d), (i32)II);
    }

    {
        MateriaNodus* radix = materia_nodus_creare(piscina,
                                  (s32)GR_RADIX, (i32)I);
                 Xar* d;
                 i32  k;
                 b32  habet_malum   = FALSUM;
                 b32  habet_verbum  = FALSUM;

        imprimere("\n--- II. Excusatio gradum II SOLUM tegit ---\n");
        CREDO_VERUM (materia_nodus_appendere(piscina, radix, ZEPHYRUM,
            materia_valor_nodus(_nodus(piscina, (s32)GR_MALUM, "@@",
                (s32)100, NIHIL, ZEPHYRUM)),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_appendere(piscina, radix, ZEPHYRUM,
            materia_valor_nodus(_nodus(piscina, (s32)GR_VERBUM, "alpha",
                (s32)150,
                "# <tolera codex=\"lint:omne-verbum\" (>consulto",
                (s32)103)),
            MATERIA_LOCUS_LISTA_NODUS));
        d = materia_diagnostica_plena(piscina, radix, &ratio, NIHIL);
        CREDO_NON_NIHIL (d);
        per (k = ZEPHYRUM; k < xar_numerus(d); k++)
        {
            constans MateriaDiagnosticum* x =
                (constans MateriaDiagnosticum*)xar_obtinere(d, k);

            si (strcmp(x->codex, "malum") == ZEPHYRUM)
            {
                habet_malum = VERUM;
            }
            si (strcmp(x->codex, "lint:omne-verbum") == ZEPHYRUM)
            {
                habet_verbum = VERUM;
            }
        }
        /* gradus I MANET, gradus II CADIT - asymmetria est porta */
        CREDO_VERUM (habet_malum);
        CREDO_FALSUM (habet_verbum);
        CREDO_AEQUALIS_I32 (xar_numerus(d), (i32)I);
    }

    {
        MateriaNodus* radix = materia_nodus_creare(piscina,
                                  (s32)GR_RADIX, (i32)I);
                    Xar* d;
MateriaDiagnosticaRatio  sine;

        imprimere("\n--- III. Sine regulis: gradus I solus ---\n");
        CREDO_VERUM (materia_nodus_appendere(piscina, radix, ZEPHYRUM,
            materia_valor_nodus(_nodus(piscina, (s32)GR_VERBUM, "alpha",
                (s32)110, NIHIL, ZEPHYRUM)),
            MATERIA_LOCUS_LISTA_NODUS));
        sine = ratio;
        sine.regulae = NIHIL;
        d = materia_diagnostica_plena(piscina, radix, &sine, NIHIL);
        CREDO_NON_NIHIL (d);
        CREDO_AEQUALIS_I32 (xar_numerus(d), ZEPHYRUM);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
