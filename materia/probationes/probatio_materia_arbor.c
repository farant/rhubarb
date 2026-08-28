/* probatio_materia_arbor.c - Scriptor arboris -> STML
 *
 * Tria hic probantur quae consilium PORTANT:
 *   1. FORMAE TRIVIORUM ex SPECIE lexici veniunt, non ex generibus
 *      notis. REPETITUM -> n=, TERMINATOR -> crlf, VERBATIM -> textus.
 *   2. LEX ANCORAE: ancora ex TRIVIO PRIMO sumitur cum lexema primum
 *      trivia ducentia fert. Silva CLXXVIII divergentias mensuravit
 *      cum lexema solum adhibetur - et probatio parva id NON cepit,
 *      quia lexema primum saepe ad offset 0 sine trivio stat. Ergo
 *      hic AMBO casus probantur.
 *   3. FRAGMENTUM/TRANSCLUSIO: lexema idem bis positum semel
 *      definitur, deinde transcluditur - duplicatio mentiretur.
 */

#include "latina.h"
#include "credo.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

/* Lexicon probationis: species omnes quattuor */
enumeratio {
    G_FINIS = 0, G_IDENT, G_SPATIA, G_NOVA_LINEA, G_COMMENTUM, G_PLUS,
    G_NUMERUS_GENERUM
};

hic_manens constans MateriaLexGenus GENERA[] = {
    { "FINIS",      "",    MATERIA_LEX_FIXUM,      MATERIA_MUNUS_FINIS },
    { "IDENT",      NIHIL, MATERIA_LEX_VERBATIM,   MATERIA_MUNUS_SUBSTANTIVUM },
    { "SPATIA",     " ",   MATERIA_LEX_REPETITUM,  MATERIA_MUNUS_SPATIUM },
    { "NOVA_LINEA", "\n",  MATERIA_LEX_TERMINATOR, MATERIA_MUNUS_LINEA },
    { "COMMENTUM",  NIHIL, MATERIA_LEX_VERBATIM,   MATERIA_MUNUS_COMMENTUM },
    { "PLUS",       "+",   MATERIA_LEX_FIXUM,      MATERIA_MUNUS_SUBSTANTIVUM }
};
hic_manens constans MateriaLexiconCoctum LEXICON = {
    GENERA, (i32)G_NUMERUS_GENERUM, "lex-", (s32)G_SPATIA
};

hic_manens constans MateriaTabLocus LOCI[] = {
    { "liberi", (s32)MATERIA_LOCUS_LISTA_MIXTA }
};
hic_manens constans MateriaTabGenus GEN[] = {
    { "radix", ZEPHYRUM, (i32)I }
};
hic_manens constans MateriaRegistrumCoctum REG = {
    GEN, (i32)I, LOCI, (i32)I
};

hic_manens MateriaToken*
_lex (Piscina* p, s32 genus, constans character* v, s32 offset,
      i32 linea, i32 columna)
{
    redde materia_token_creare(p, &FORMA, genus,
        chorda_ex_literis(v, p), offset, linea, columna, ZEPHYRUM);
}

s32 principale (vacuum)
{
                b32  praeteritus;
           Piscina*  piscina;
MateriaLexiconRatum  ratum;
 MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_materia_arbor", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &LEXICON, &iudicium));


    /* ========================================================
     * PROBARE: mangulatio tagorum, utroque modo
     * ======================================================== */
    {
        character tag[MATERIA_ARBOR_TAG_CAPACITAS];

        imprimere("\n--- Probans tagos ---\n");

        CREDO_VERUM (materia_arbor_lexema_tag(&ratum, (s32)G_NOVA_LINEA,
            tag, (i32)magnitudo(tag)) > ZEPHYRUM);
        CREDO_VERUM (strcmp(tag, "lex-nova-linea") == ZEPHYRUM);

        /* Inversum per mangulationem ANTRORSAM - directiones
         * divergere non possunt */
        CREDO_AEQUALIS_S32 (materia_arbor_lexema_ex_tag(&ratum,
            "lex-nova-linea", (i32)14), (s32)G_NOVA_LINEA);
        CREDO_AEQUALIS_S32 (materia_arbor_lexema_ex_tag(&ratum,
            "lex-ignotum", (i32)11), (s32)G_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 (materia_arbor_lexema_tag(&ratum, (s32)-I,
            tag, (i32)magnitudo(tag)), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: formae triviorum EX SPECIE
     * ======================================================== */
    {
        MateriaArborConsilium c;
        MateriaNodus* n;
        MateriaToken* t;
        MateriaToken* tr[3];
        MateriaArborScriptura s;

        imprimere("\n--- Probans formas triviorum ---\n");

        t     = _lex(piscina, (s32)G_IDENT, "x", (s32)6, (i32)I, (i32)7);
        tr[0] = _lex(piscina, (s32)G_SPATIA, "    ", ZEPHYRUM, (i32)I, (i32)I);
        tr[1] = _lex(piscina, (s32)G_COMMENTUM, "/*c*/", (s32)4, (i32)I, (i32)5);
        tr[2] = _lex(piscina, (s32)G_NOVA_LINEA, "\r\n", (s32)7, (i32)I, (i32)8);
        CREDO_VERUM (materia_token_trivia_ante_ponere(t, piscina, tr, (i32)II));
        CREDO_VERUM (materia_token_trivia_post_ponere(t, piscina, tr + II,
            (i32)I));

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(t), MATERIA_LOCUS_LISTA_MIXTA));

        materia_arbor_consilium_nudum(&c, &REG, &ratum, "probatio");
        s = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_VERUM (s.textus.mensura > ZEPHYRUM);

        {
            character* txt = (character*)s.textus.datum;

            /* REPETITUM -> n="4", NON textus spatiorum (qui a
             * scriptore pulchro tacite periret) */
            CREDO_NON_NIHIL (strstr(txt, "<lex-spatia n=\"4\""));
            /* TERMINATOR -> vexillum crlf */
            CREDO_NON_NIHIL (strstr(txt, "lex-nova-linea"));
            CREDO_NON_NIHIL (strstr(txt, "crlf"));
            /* VERBATIM -> textus */
            CREDO_NON_NIHIL (strstr(txt, "/*c*/"));
            /* involucra */
            CREDO_NON_NIHIL (strstr(txt, "<ante>"));
            CREDO_NON_NIHIL (strstr(txt, "<post>"));
            /* sigillum registri et grammatica */
            CREDO_NON_NIHIL (strstr(txt, "grammatica=\"probatio\""));
            CREDO_NON_NIHIL (strstr(txt, "registrum-sigillum="));
        }
    }


    /* ========================================================
     * PROBARE: LEX ANCORAE - ambo casus
     * ======================================================== */
    {
        MateriaArborConsilium c;
        MateriaArborScriptura s;

        imprimere("\n--- Probans legem ancorae ---\n");
        materia_arbor_consilium_nudum(&c, &REG, &ratum, "probatio");

        /* CASUS I: lexema primum SINE triviis ducentibus, offset 0.
         * Hic vitium EVANESCIT - probatio quae hoc solum probat de
         * lege ancorae NIHIL dicit. */
        {
            MateriaNodus* n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);

            CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
                materia_valor_token(_lex(piscina, (s32)G_IDENT, "a",
                    ZEPHYRUM, (i32)I, (i32)I)),
                MATERIA_LOCUS_LISTA_MIXTA));
            s = materia_arbor_scribere_nodum(piscina, n, &c);
            CREDO_VERUM (s.successus);
            CREDO_NON_NIHIL (strstr((character*)s.textus.datum, "b=\"0\""));
        }

        /* CASUS II: lexema primum CUM trivio ducente ad offset IV,
         * lexema ipsum ad offset VIII. Ancora IV esse DEBET - id est
         * ubi emissio incipit. Si VIII, sedes omnes longitudine
         * indentationis labuntur. */
        {
            MateriaNodus* n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
            MateriaToken* t = _lex(piscina, (s32)G_IDENT, "b", (s32)8,
                (i32)II, (i32)5);
            MateriaToken* tr[1];

            tr[0] = _lex(piscina, (s32)G_SPATIA, "    ", (s32)4, (i32)II,
                (i32)I);
            CREDO_VERUM (materia_token_trivia_ante_ponere(t, piscina, tr,
                (i32)I));
            CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
                materia_valor_token(t), MATERIA_LOCUS_LISTA_MIXTA));

            s = materia_arbor_scribere_nodum(piscina, n, &c);
            CREDO_VERUM (s.successus);
            CREDO_NON_NIHIL (strstr((character*)s.textus.datum, "b=\"4\""));
            /* et NON ad lexema ipsum */
            CREDO_NIHIL (strstr((character*)s.textus.datum, "b=\"8\""));
            CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
                "columna=\"1\""));
        }
    }


    /* ========================================================
     * PROBARE: fragmentum et transclusio (identitas, non copia)
     * ======================================================== */
    {
        MateriaArborConsilium c;
        MateriaNodus*         n;
        MateriaToken*         t;
        MateriaArborScriptura s;

        imprimere("\n--- Probans fragmentum/transclusionem ---\n");

        t = _lex(piscina, (s32)G_IDENT, "z", ZEPHYRUM, (i32)I, (i32)I);
        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        /* IDEM lexema bis - bracchia ambigua id revera faciunt */
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(t), MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(t), MATERIA_LOCUS_LISTA_MIXTA));

        materia_arbor_consilium_nudum(&c, &REG, &ratum, "probatio");
        s = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        {
            character* txt = (character*)s.textus.datum;

            CREDO_NON_NIHIL (strstr(txt, "lex1"));       /* definitio */
            CREDO_NON_NIHIL (strstr(txt, "#lex1"));      /* transclusio */
        }
    }


    /* ========================================================
     * PROBARE: custodiae
     * ======================================================== */
    {
        MateriaArborConsilium c;
        MateriaArborScriptura s;
        MateriaNodus*         n;
        MateriaLexiconRatum   irritum;

        imprimere("\n--- Probans custodias ---\n");

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        materia_arbor_consilium_nudum(&c, &REG, &ratum, "probatio");

        /* grammatica innominata */
        c.grammatica = NIHIL;
        s = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_FALSUM (s.successus);
        c.grammatica = "probatio";

        /* lexicon NON RATUM - porta oneris non transita */
        irritum.lexicon = &LEXICON;
        irritum.munera_praesentia = ZEPHYRUM;
        irritum.ratum = FALSUM;
        c.lexicon = &irritum;
        s = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_FALSUM (s.successus);
        CREDO_NON_NIHIL (s.causa);
        c.lexicon = &ratum;

        /* genus ignotum: fractura cum SEDE */
        n->genus = (s32)99;
        s = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_FALSUM (s.successus);
        CREDO_AEQUALIS_PTR ((constans vacuum*)s.sedes, (constans vacuum*)n);
        n->genus = ZEPHYRUM;

        /* argumenta nulla */
        s = materia_arbor_scribere_nodum(piscina, NIHIL, &c);
        CREDO_FALSUM (s.successus);
        s = materia_arbor_scribere_nodum(piscina, n, NIHIL);
        CREDO_FALSUM (s.successus);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
