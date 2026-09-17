/* probatio_materia_sedes.c - Tractus et visio sedium
 * (materia-sedes-spec par. II-III)
 *
 * Tractus lexematis: fontis per octetos valoris promovetur, derivatum
 * punctum est, syntheticum sine sede. Tractus nodi: lexemata locorum
 * sola (trivia numquam), REFERENTIA non descensa. Visio: attributa
 * sedes/octeti in elementis nodorum, lexematum et triviorum;
 * involucrum visio="sedes"; lector eam recusat; proiectio sine
 * visione intacta; lexema communicatum sedem semel fert.
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

/* Lexicon probationis: species omnes quattuor (ut probatio arboris) */
enumeratio {
    G_FINIS = 0, G_IDENT, G_SPATIA, G_NOVA_LINEA, G_COMMENTUM, G_PLUS,
    G_ALBUM,
    G_NUMERUS_GENERUM
};

hic_manens constans MateriaLexGenus GENERA[] = {
    { "FINIS",      "",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },
    { "IDENT",      NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SPATIA",     " ",   MATERIA_LEX_REPETITUM,
        MATERIA_MUNUS_SPATIUM },
    { "NOVA_LINEA", "\n",  MATERIA_LEX_TERMINATOR,
        MATERIA_MUNUS_LINEA },
    { "COMMENTUM",  NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_COMMENTUM },
    { "PLUS",       "+",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ALBUM",      NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SPATIUM }
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
_lex (
               Piscina* p,
                   s32  genus,
    constans character* v,
                   s32  offset,
                   i32  linea,
                   i32  columna)
{
    redde materia_token_creare(p, &FORMA, genus,
        chorda_ex_literis(v, p), offset, linea, columna, ZEPHYRUM);
}

/* uncus derivationis: sedes portata, est_fons FALSUM */
interior vacuum
_sedes_derivata (
                   vacuum* datum,
    constans MateriaToken* lexema,
             MateriaSedes* sedes)
{
    (vacuum)datum;
    sedes->byte_offset  = lexema->byte_offset;
    sedes->linea        = lexema->linea;
    sedes->columna      = lexema->columna;
    sedes->fons_index   = lexema->fons_index;
    sedes->est_fons     = FALSUM;
}

hic_manens constans MateriaOrigoUncus UNCUS_DERIVATUS = {
    NIHIL, _sedes_derivata, NIHIL, NIHIL
};

interior i32
_numerare (
    constans character* textus,
    constans character* forma)
{
                   i32  numerus  = ZEPHYRUM;
    constans character* p        = strstr(textus, forma);

    dum (p != NIHIL)
    {
        numerus++;
        p = strstr(p + I, forma);
    }
    redde numerus;
}

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
MateriaLexiconRatum  ratum;
 MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_materia_sedes",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &LEXICON,
        &iudicium));


    /* ==================================================
     * PROBARE: tractus lexematis
     * ================================================== */

    {
        MateriaTractus t;

        imprimere("\n--- Probans tractum lexematis ---\n");
        materia_tractus_lexematis(NIHIL, _lex(piscina, (s32)G_IDENT,
            "abc", (s32)10, (i32)II, (i32)III), &t);
        CREDO_AEQUALIS_S32 (t.initium, (s32)10);
        CREDO_AEQUALIS_S32 (t.finis, (s32)13);
        CREDO_AEQUALIS_I32 (t.linea, (i32)II);
        CREDO_AEQUALIS_I32 (t.columna, (i32)III);
        CREDO_AEQUALIS_I32 (t.linea_finis, (i32)II);
        CREDO_AEQUALIS_I32 (t.columna_finis, (i32)VI);
        CREDO_VERUM (t.est_fons);

        materia_tractus_lexematis(NIHIL, _lex(piscina, (s32)G_IDENT,
            "a\nbc", ZEPHYRUM, (i32)I, (i32)I), &t);
        CREDO_AEQUALIS_S32 (t.finis, (s32)IV);
        CREDO_AEQUALIS_I32 (t.linea_finis, (i32)II);
        CREDO_AEQUALIS_I32 (t.columna_finis, (i32)III);

        materia_tractus_lexematis(NIHIL, _lex(piscina,
            (s32)G_NOVA_LINEA, "\r\n", (s32)V, (i32)I, (i32)VI), &t);
        CREDO_AEQUALIS_S32 (t.finis, (s32)VII);
        CREDO_AEQUALIS_I32 (t.linea_finis, (i32)II);
        CREDO_AEQUALIS_I32 (t.columna_finis, (i32)I);

        /* syntheticum: sine sede */
        materia_tractus_lexematis(NIHIL, _lex(piscina, (s32)G_IDENT,
            "q", (s32)-I, (i32)I, (i32)I), &t);
        CREDO_AEQUALIS_S32 (t.initium, (s32)-I);

        /* derivatum: punctum in sede portata */
        materia_tractus_lexematis(&UNCUS_DERIVATUS, _lex(piscina,
            (s32)G_IDENT, "decoctum", (s32)20, (i32)III, (i32)IV), &t);
        CREDO_AEQUALIS_S32 (t.initium, (s32)20);
        CREDO_AEQUALIS_S32 (t.finis, (s32)20);
        CREDO_AEQUALIS_I32 (t.linea_finis, (i32)III);
        CREDO_AEQUALIS_I32 (t.columna_finis, (i32)IV);
        CREDO_FALSUM (t.est_fons);
    }


    /* ==================================================
     * PROBARE: tractus nodi
     * ================================================== */

    {
          MateriaNodus* nodus;
          MateriaNodus* filius;
          MateriaNodus* scopus;
        MateriaTractus  t;

        imprimere("\n--- Probans tractum nodi ---\n");
        filius = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, filius, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "yy",
                (s32)8, (i32)II, (i32)I)),
            MATERIA_LOCUS_LISTA_MIXTA));
        nodus = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, nodus, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "x",
                (s32)IV, (i32)I, (i32)V)),
            MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_nodus_appendere(piscina, nodus, ZEPHYRUM,
            materia_valor_nodus(filius), MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_tractus_nodi(NIHIL, nodus, &t));
        CREDO_AEQUALIS_S32 (t.initium, (s32)IV);
        CREDO_AEQUALIS_S32 (t.finis, (s32)10);
        CREDO_AEQUALIS_I32 (t.linea, (i32)I);
        CREDO_AEQUALIS_I32 (t.columna, (i32)V);
        CREDO_AEQUALIS_I32 (t.linea_finis, (i32)II);
        CREDO_AEQUALIS_I32 (t.columna_finis, (i32)III);

        /* REFERENTIA non descensa */
        scopus = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, scopus, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "zzz",
                (s32)100, (i32)IX, (i32)I)),
            MATERIA_LOCUS_LISTA_MIXTA));
        nodus = materia_nodus_creare(piscina, ZEPHYRUM, (i32)II);
        CREDO_VERUM (materia_nodus_ponere(nodus, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "a",
                ZEPHYRUM, (i32)I, (i32)I)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_ponere(nodus, (i32)I,
            materia_valor_referentia(scopus),
            MATERIA_LOCUS_REFERENTIA));
        CREDO_VERUM (materia_tractus_nodi(NIHIL, nodus, &t));
        CREDO_AEQUALIS_S32 (t.finis, (s32)I);

        /* nullum lexema cum sede */
        nodus = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, nodus, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "s",
                (s32)-I, (i32)I, (i32)I)),
            MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_FALSUM (materia_tractus_nodi(NIHIL, nodus, &t));
        CREDO_AEQUALIS_S32 (t.initium, (s32)-I);
    }


    /* ==================================================
     * PROBARE: visio sedium
     * ================================================== */

    {
        MateriaArborConsilium c;
        MateriaArborScriptura s;
        MateriaArborScriptura nuda;
           MateriaArborVitium vitium;
                 MateriaToken* x;
                 MateriaToken* tr[3];
                 MateriaNodus* n;
                          b32  admissi[1];
                    character* txt;

        imprimere("\n--- Probans visionem sedium ---\n");
        /* fons: spatia duo ad 0, commentum quinque octetorum ad 2,
         * 'x' ad 7, linea nova ad 8 */
        x = _lex(piscina, (s32)G_IDENT, "x", (s32)VII, (i32)I,
            (i32)VIII);
        tr[0] = _lex(piscina, (s32)G_SPATIA, "  ", ZEPHYRUM, (i32)I,
            (i32)I);
        tr[1] = _lex(piscina, (s32)G_COMMENTUM, "/*c*/", (s32)II,
            (i32)I, (i32)III);
        tr[2] = _lex(piscina, (s32)G_NOVA_LINEA, "\n", (s32)VIII,
            (i32)I, (i32)IX);
        CREDO_VERUM (materia_token_trivia_ante_ponere(x, piscina, tr,
            (i32)II));
        CREDO_VERUM (materia_token_trivia_post_ponere(x, piscina,
            tr + II, (i32)I));
        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(x), MATERIA_LOCUS_LISTA_MIXTA));

        materia_arbor_consilium_nudum(&c, &REG, &ratum, "probatio");
        nuda = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (nuda.successus);
        CREDO_NIHIL (strstr(chorda_ut_cstr(nuda.textus, piscina),
            "sedes="));
        CREDO_NIHIL (strstr(chorda_ut_cstr(nuda.textus, piscina),
            "visio="));

        c.sedes_scribere = VERUM;
        s = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        txt = chorda_ut_cstr(s.textus, piscina);
        CREDO_NON_NIHIL (strstr(txt, "visio=\"sedes\""));
        /* nodus radix et lexema x: idem tractus (trivia exclusa) */
        CREDO_AEQUALIS_I32 (_numerare(txt, "sedes=\"1:8-1:9\""),
            (i32)II);
        CREDO_AEQUALIS_I32 (_numerare(txt, "octeti=\"7-8\""), (i32)II);
        CREDO_NON_NIHIL (strstr(txt, "octeti=\"0-2\""));
        CREDO_NON_NIHIL (strstr(txt, "sedes=\"1:1-1:3\""));
        CREDO_NON_NIHIL (strstr(txt, "sedes=\"1:3-1:8\""));
        CREDO_NON_NIHIL (strstr(txt, "octeti=\"2-7\""));
        CREDO_NON_NIHIL (strstr(txt, "sedes=\"1:9-2:1\""));
        CREDO_NON_NIHIL (strstr(txt, "octeti=\"8-9\""));

        /* lector visionem recusat */
        CREDO_NIHIL (materia_arbor_legere(piscina, NIHIL, s.textus, &c,
            &vitium));
        CREDO_NON_NIHIL (vitium.causa);
        si (vitium.causa != NIHIL)
        {
            CREDO_VERUM (strcmp(vitium.causa,
                "visio sedes: non arbor") == ZEPHYRUM);
        }

        /* cum filtro locorum */
        admissi[0]      = VERUM;
        c.loci_admissi  = admissi;
        s               = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_NON_NIHIL (strstr(chorda_ut_cstr(s.textus, piscina),
            "visio=\"partialis sedes\""));

        /* lexema communicatum: transclusio sedem non iterat */
        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(x), MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(x), MATERIA_LOCUS_LISTA_MIXTA));
        c.loci_admissi  = NIHIL;
        s               = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (_numerare(chorda_ut_cstr(s.textus, piscina),
            "octeti=\"7-8\""), (i32)II);
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
