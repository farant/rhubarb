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
    /* ALBUM: spatium album genere VERBATIM - forma CSS, ubi unum
     * genus spatia+tabulas+lineas absorbet et numerus solus octetos
     * determinare non potest. ULTIMUM ponitur ut indices priorum
     * IMMOTI maneant. */
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

/* Registrum cum REFERENTIA (2026-09-10): radix {liberi}, filius
 * {gradus INDEX, socius REFERENTIA} */
enumeratio { GR_RADIX = 0, GR_FILIUS };
enumeratio { LR_LIBERI = 0, LR_GRADUS, LR_SOCIUS };
hic_manens constans MateriaTabLocus LOCI_REF[] = {
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "gradus", (s32)MATERIA_LOCUS_INDEX },
    { "socius", (s32)MATERIA_LOCUS_REFERENTIA }
};
hic_manens constans MateriaTabGenus GEN_REF[] = {
    { "radix",  ZEPHYRUM, (i32)I },
    { "filius", (i32)I,   (i32)II }
};
hic_manens constans MateriaRegistrumCoctum REG_REF = {
    GEN_REF, (i32)II, LOCI_REF, (i32)III
};

/* filius cum gradu; socius postea */
hic_manens MateriaNodus*
_filium_struere (
    Piscina* p,
        s32  gradus)
{
    MateriaNodus* f = materia_nodus_creare(p, (s32)GR_FILIUS, (i32)II);

    si (f == NIHIL)
    { redde NIHIL;
    }
    si (!materia_nodus_ponere(f, (i32)LR_GRADUS - I,
            materia_valor_index(gradus), MATERIA_LOCUS_INDEX))
    {
        redde NIHIL;
    }
    redde f;
}

hic_manens MateriaNodus*
_socium_filii (
    constans MateriaNodus* f)
{
    constans MateriaValor* v = &f->loci[(i32)LR_SOCIUS - I];

    redde v->genus == MATERIA_VALOR_REFERENTIA ? v->datum.nodus : NIHIL;
}

hic_manens s32
_gradum_filii (
    constans MateriaNodus* f)
{
    constans MateriaValor* v = &f->loci[(i32)LR_GRADUS - I];

    redde v->genus == MATERIA_VALOR_INDEX ? v->datum.index : (s32)-I;
}

hic_manens MateriaNodus*
_liberum_radicis (
    constans MateriaNodus* radix,
                      i32  k)
{
    MateriaValor* e = materia_valor_lista_obtinere(
        radix->loci[(i32)LR_LIBERI], k);

    redde (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
        ? e->datum.nodus : NIHIL;
}

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

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
MateriaLexiconRatum  ratum;
 MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_materia_arbor",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &LEXICON,
        &iudicium));


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
        MateriaArborScriptura  s;

        imprimere("\n--- Probans formas triviorum ---\n");

        t = _lex(piscina, (s32)G_IDENT, "x", (s32)6, (i32)I,
            (i32)7);
        tr[0] = _lex(piscina, (s32)G_SPATIA, "    ", ZEPHYRUM, (i32)I,
            (i32)I);
        tr[1] = _lex(piscina, (s32)G_COMMENTUM, "/*c*/", (s32)4, (i32)I,
            (i32)5);
        tr[2] = _lex(piscina, (s32)G_NOVA_LINEA, "\r\n", (s32)7, (i32)I,
            (i32)8);
        CREDO_VERUM (materia_token_trivia_ante_ponere(t, piscina, tr,
            (i32)II));
        CREDO_VERUM (materia_token_trivia_post_ponere(t, piscina, tr
            + II,
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
            MateriaNodus* n = materia_nodus_creare(piscina, ZEPHYRUM,
                (i32)I);

            CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
                materia_valor_token(_lex(piscina, (s32)G_IDENT, "a",
                    ZEPHYRUM, (i32)I, (i32)I)),
                MATERIA_LOCUS_LISTA_MIXTA));
            s = materia_arbor_scribere_nodum(piscina, n, &c);
            CREDO_VERUM (s.successus);
            CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
                "b=\"0\""));
        }

        /* CASUS II: lexema primum CUM trivio ducente ad offset IV,
         * lexema ipsum ad offset VIII. Ancora IV esse DEBET - id est
         * ubi emissio incipit. Si VIII, sedes omnes longitudine
         * indentationis labuntur. */
        {
            MateriaNodus* n = materia_nodus_creare(piscina, ZEPHYRUM,
                (i32)I);
            MateriaToken* t = _lex(piscina, (s32)G_IDENT, "b", (s32)8,
                (i32)II, (i32)5);
            MateriaToken* tr[1];

            tr[0] = _lex(piscina, (s32)G_SPATIA, "    ", (s32)4,
                (i32)II,
                (i32)I);
            CREDO_VERUM (materia_token_trivia_ante_ponere(t, piscina,
                tr,
                (i32)I));
            CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
                materia_valor_token(t), MATERIA_LOCUS_LISTA_MIXTA));

            s = materia_arbor_scribere_nodum(piscina, n, &c);
            CREDO_VERUM (s.successus);
            CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
                "b=\"4\""));
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
                 MateriaNodus* n;
                 MateriaToken* t;
        MateriaArborScriptura  s;

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
                 MateriaNodus* n;
          MateriaLexiconRatum  irritum;

        imprimere("\n--- Probans custodias ---\n");

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        materia_arbor_consilium_nudum(&c, &REG, &ratum, "probatio");

        /* grammatica innominata */
        c.grammatica  = NIHIL;
        s             = materia_arbor_scribere_nodum(piscina, n, &c);
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
        n->genus  = (s32)99;
        s         = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_FALSUM (s.successus);
        CREDO_AEQUALIS_PTR ((constans vacuum*)s.sedes,
            (constans vacuum*)n);
        n->genus = ZEPHYRUM;

        /* argumenta nulla */
        s = materia_arbor_scribere_nodum(piscina, NIHIL, &c);
        CREDO_FALSUM (s.successus);
        s = materia_arbor_scribere_nodum(piscina, n, NIHIL);
        CREDO_FALSUM (s.successus);
    }


    /* ========================================================
     * PROBARE: CIRCUITUS - scribe, lege, scribe iterum
     *
     * DUO circuitus, non unus. Vitium quod se COMPOUNDAT (sedes
     * per unum octetum lapsae, trivium amissum) primo circuitu
     * saepe latet et secundo apparet: 'scribe -> lege' semel
     * documentum SUUM reddit, bis documentum SUI DOCUMENTI.
     * (Lex ex stml-text-semantics: 'circuitus unus testimonium
     * NON est cum vitium compoundat'.)
     * ======================================================== */

    {
        MateriaArborConsilium c;
                 MateriaNodus* n;
                 MateriaToken* t1;
                 MateriaToken* t2;
                 MateriaToken* tr[2];
        MateriaArborScriptura  s1;
        MateriaArborScriptura  s2;
        MateriaArborScriptura  s3;
                 MateriaNodus* lecta;
                 MateriaNodus* lecta2;
           MateriaArborVitium  vitium;

        imprimere("\n--- Probans circuitum (BIS) ---\n");

        /* '    a+b' : trivium ducens, duo lexemata, unum fixum */
        tr[0] = _lex(piscina, (s32)G_SPATIA, "    ", ZEPHYRUM, (i32)I,
            (i32)I);
        t1 = _lex(piscina, (s32)G_IDENT, "a", (s32)4, (i32)I,
            (i32)5);
        t2 = _lex(piscina, (s32)G_PLUS,  "+", (s32)5, (i32)I,
            (i32)6);
        CREDO_VERUM (materia_token_trivia_ante_ponere(t1, piscina, tr,
            (i32)I));

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(t1), MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(t2), MATERIA_LOCUS_LISTA_MIXTA));

        materia_arbor_consilium_nudum(&c, &REG, &ratum, "probatio");
        s1 = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s1.successus);

        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, &c,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);

        s2 = materia_arbor_scribere_nodum(piscina, lecta, &c);
        CREDO_VERUM (s2.successus);
        CREDO_AEQUALIS_I32 (s2.textus.mensura, s1.textus.mensura);
        CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
            (size_t)s1.textus.mensura) == ZEPHYRUM);

        /* CIRCUITUS SECUNDUS */
        lecta2 = materia_arbor_legere(piscina, NIHIL, s2.textus, &c,
            &vitium);
        CREDO_NON_NIHIL (lecta2);
        s3 = materia_arbor_scribere_nodum(piscina, lecta2, &c);
        CREDO_VERUM (s3.successus);
        CREDO_AEQUALIS_I32 (s3.textus.mensura, s1.textus.mensura);
        CREDO_VERUM (memcmp(s1.textus.datum, s3.textus.datum,
            (size_t)s1.textus.mensura) == ZEPHYRUM);

        /* SEDES DERIVATAE: documentum eas non fert, lector eas
         * reficit. Lexema primum ad IV (post trivium), '+' ad V. */
        {
            MateriaValor* e0 =
                materia_valor_lista_obtinere(lecta->loci[0],
                ZEPHYRUM);
            MateriaValor* e1 =
                materia_valor_lista_obtinere(lecta->loci[0],
                (i32)I);

            CREDO_NON_NIHIL (e0);
            CREDO_NON_NIHIL (e1);
            CREDO_AEQUALIS_S32 (e0->datum.token->byte_offset, (s32)4);
            CREDO_AEQUALIS_I32 (e0->datum.token->columna, (i32)5);
            CREDO_AEQUALIS_S32 (e1->datum.token->byte_offset, (s32)5);
            /* trivium ducens ad ZEPHYRUM - ubi emissio incipit */
            CREDO_AEQUALIS_S32 (e0->datum.token->spatia_ante[0]->byte_offset,
                ZEPHYRUM);
            /* PATER fixus */
            CREDO_NIHIL (lecta->pater);
        }
    }


    /* ========================================================
     * PROBARE: custodiae lectoris
     * ======================================================== */

    {
        MateriaArborConsilium c;
           MateriaArborVitium vitium;
                 MateriaNodus* n;
        MateriaArborScriptura  s;

        imprimere("\n--- Probans custodias lectoris ---\n");
        materia_arbor_consilium_nudum(&c, &REG, &ratum, "probatio");

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "q",
                ZEPHYRUM, (i32)I, (i32)I)), MATERIA_LOCUS_LISTA_MIXTA));
        s = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);

        /* STML pravum */
        CREDO_NIHIL (materia_arbor_legere(piscina, NIHIL,
            chorda_ex_literis("<arbor", piscina), &c, &vitium));
        CREDO_NON_NIHIL (vitium.causa);

        /* involucrum alienum */
        CREDO_NIHIL (materia_arbor_legere(piscina, NIHIL,
            chorda_ex_literis("<aliud/>", piscina), &c, &vitium));
        CREDO_NON_NIHIL (vitium.causa);

        /* grammatica dispar - documentum sanum, nomen aliud */
        c.grammatica = "aliud";
        CREDO_NIHIL (materia_arbor_legere(piscina, NIHIL, s.textus, &c,
            &vitium));
        CREDO_NON_NIHIL (vitium.causa);
        c.grammatica = "probatio";

        /* SIGILLUM: registrum ALIUD, arbor eadem. Vocabulario falso
         * iudicata arbor mendacium est - ergo recusatur. */
        {
            hic_manens constans MateriaTabLocus L2[] = {
                { "alius", (s32)MATERIA_LOCUS_LISTA_MIXTA }
            };
            hic_manens constans MateriaTabGenus G2[] = {
                { "radix", ZEPHYRUM, (i32)I }
            };
            hic_manens constans MateriaRegistrumCoctum REG2 = {
                G2, (i32)I, L2, (i32)I
            };

            c.tabularium = &REG2;
            CREDO_NIHIL (materia_arbor_legere(piscina, NIHIL, s.textus,
                &c, &vitium));
            CREDO_NON_NIHIL (vitium.causa);
            c.tabularium = &REG;
        }

        /* sanum iterum - custodiae statum non corruperunt */
        CREDO_NON_NIHIL (materia_arbor_legere(piscina, NIHIL, s.textus,
            &c, &vitium));
    }


    /* ========================================================
     * PROBARE: spatium album VERBATIM per crudum (2026-08-28)
     *
     * Contractus prius NIMIS LATUS erat UTRAQUE parte: scriptor
     * valorem albi-solius refutabat, lector eum praeteribat, et
     * partes CONGRUEBANT - sed congruentia partium veritatem non
     * probat. Pulcher dispositionem possidet EXTRA crudum; intra
     * crudum octeti verbatim sunt.
     *
     * Sine hac emendatione lingua cuius spatium genus VERBATIM est
     * (CSS) plagulam quamlibet spatium ferentem proicere NON
     * POTERAT - centesimam partem CSS veri.
     * ======================================================== */

    {
        MateriaArborConsilium c;
           MateriaArborVitium vitium;
        MateriaArborScriptura s1;
        MateriaArborScriptura s2;
                 MateriaNodus* radix;
                 MateriaNodus* lecta;
                 MateriaToken* ident;
                 MateriaToken* album;

        imprimere("\n--- Probans spatium album VERBATIM (crudum) ---\n");

        album = _lex(piscina, (s32)G_ALBUM, "\n\n  ", ZEPHYRUM,
            (i32)I, (i32)I);
        ident = _lex(piscina, (s32)G_IDENT, "x", (s32)IV, (i32)III,
            (i32)III);
        CREDO_VERUM (materia_token_trivia_ante_ponere(ident, piscina,
            &album, (i32)I));

        radix = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, radix, ZEPHYRUM,
            materia_valor_token(ident), MATERIA_LOCUS_LISTA_MIXTA));

        materia_arbor_consilium_nudum(&c, &REG, &ratum, "probatio");
        s1 = materia_arbor_scribere_nodum(piscina, radix, &c);
        CREDO_VERUM (s1.successus);
        si (!s1.successus)
        {
            imprimere("  causa: %s\n", s1.causa ? s1.causa : "(nulla)");
        }

        /* Elementum CRUDUM esse DEBET - id est quod valorem album
         * inambiguum facit. Sine '!' pulcher eum absorberet. */
        CREDO_NON_NIHIL (strstr((character*)s1.textus.datum,
            "lex-album!"));

        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, &c,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);

        /* OCTETI SERVATI, non solum 'lectio successit' */
        {
            MateriaValor* e = materia_valor_lista_obtinere(
                lecta->loci[ZEPHYRUM], ZEPHYRUM);
            MateriaToken* t;

            CREDO_NON_NIHIL (e);
            t = e->datum.token;
            CREDO_NON_NIHIL (t);
            CREDO_AEQUALIS_I32 (t->numerus_ante, (i32)I);
            CREDO_AEQUALIS_I32 (t->spatia_ante[ZEPHYRUM]->valor.mensura,
                (i32)IV);
            CREDO_VERUM (memcmp(t->spatia_ante[ZEPHYRUM]->valor.datum,
                "\n\n  ", (size_t)IV) == ZEPHYRUM);
        }

        /* CIRCUITUS SECUNDUS: vitium compoundens circuitum unum
         * superat */
        s2 = materia_arbor_scribere_nodum(piscina, lecta, &c);
        CREDO_VERUM (s2.successus);
        CREDO_AEQUALIS_I32 (s2.textus.mensura, s1.textus.mensura);
        CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
            (size_t)s1.textus.mensura) == ZEPHYRUM);

        /* NUL ADHUC REFUTATUR - cruditas eum sanare non potest,
         * quia chorda longitudinem fert et textus terminatore
         * legitur. Relaxatio albi hoc NON tetigit. */
        {
                     MateriaToken* malum;
                     MateriaNodus* r2;
                           chorda  cum_nul;
            MateriaArborScriptura  s3;

            cum_nul.mensura = (i32)II;
            cum_nul.datum   = (i8*)piscina_allocare(piscina,
                (memoriae_index)II);
            cum_nul.datum[ZEPHYRUM]  = (i8)' ';
            cum_nul.datum[I]         = (i8)'\0';

            malum = materia_token_creare(piscina, &FORMA, (s32)G_ALBUM,
                cum_nul, ZEPHYRUM, (i32)I, (i32)I, ZEPHYRUM);
            r2 = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
            CREDO_VERUM (materia_nodus_appendere(piscina, r2, ZEPHYRUM,
                materia_valor_token(malum), MATERIA_LOCUS_LISTA_MIXTA));
            s3 = materia_arbor_scribere_nodum(piscina, r2, &c);
            CREDO_FALSUM (s3.successus);
        }
    }


    /* ========================================================
     * PROBARE: REFERENTIAE (2026-09-10)
     *
     * Nexus lateralis super arborem: scopus id="nodN", locus
     * '#nodN'. Antrorsum ET in circuitu (A -> B, B -> A) - quod
     * transclusio fragmentorum dare non potest ('ante usum',
     * circuitus recusatus). Lector post arborem totam solvit;
     * circuitus bis (vitium compoundens); permutatio listae
     * referentias INTACTAS relinquit - ipsa causa speciei.
     * ======================================================== */

    {
        MateriaArborConsilium c;
           MateriaArborVitium vitium;
        MateriaArborScriptura s1;
        MateriaArborScriptura s2;
        MateriaArborScriptura s3;
                 MateriaNodus* radix;
                 MateriaNodus* a;
                 MateriaNodus* b;
                 MateriaNodus* lecta;
                 MateriaNodus* lecta2;
      MateriaArborDifferentia  d;

        imprimere("\n--- Probans referentias (circuitus) ---\n");

        radix  = materia_nodus_creare(piscina, (s32)GR_RADIX, (i32)I);
        a      = _filium_struere(piscina, (s32)I);
        b      = _filium_struere(piscina, (s32)II);
        CREDO_NON_NIHIL (radix);
        CREDO_NON_NIHIL (a);
        CREDO_NON_NIHIL (b);
        CREDO_VERUM (materia_nodus_appendere(piscina, radix,
            (i32)LR_LIBERI, materia_valor_nodus(a),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_appendere(piscina, radix,
            (i32)LR_LIBERI, materia_valor_nodus(b),
            MATERIA_LOCUS_LISTA_NODUS));
        /* A -> B antrorsum, B -> A retrorsum: circuitus */
        CREDO_VERUM (materia_nodus_ponere(a, (i32)LR_SOCIUS - I,
            materia_valor_referentia(b), MATERIA_LOCUS_REFERENTIA));
        CREDO_VERUM (materia_nodus_ponere(b, (i32)LR_SOCIUS - I,
            materia_valor_referentia(a), MATERIA_LOCUS_REFERENTIA));

        materia_arbor_consilium_nudum(&c, &REG_REF, &ratum, "probatio");
        s1 = materia_arbor_scribere_nodum(piscina, radix, &c);
        CREDO_VERUM (s1.successus);
        si (!s1.successus)
        {
            imprimere("  causa: %s\n", s1.causa ? s1.causa : "(nulla)");
        }
        {
            character* txt  = (character*)s1.textus.datum;
            character* fa   = strstr(txt, "<filius id=\"nod1\" ((>");
            character* fb   = strstr(txt, "<filius id=\"nod2\" ((>");

            /* numeratio ut primum nominatur: A scopus primus (nod1),
             * B nominatur a referente A ante elementum suum (nod2).
             * Forma pulchra: '<socius(> #nod2' in linea sua. */
            CREDO_NON_NIHIL (fa);
            CREDO_NON_NIHIL (fb);
            CREDO_VERUM (fa != NIHIL && fb != NIHIL && fa < fb);
            si (fa != NIHIL && fb != NIHIL)
            {
                character* ga = strstr(fa, "<gradus(> 1");
                character* sa = strstr(fa, "<socius(> #nod2");
                character* sb = strstr(fb, "<socius(> #nod1");

                CREDO_VERUM (ga != NIHIL && ga < fb);
                CREDO_VERUM (sa != NIHIL && sa < fb);
                CREDO_NON_NIHIL (sb);
            }
            /* nulla transclusio, nullum fragmentum */
            CREDO_NIHIL (strstr(txt, "<<#"));
            CREDO_NIHIL (strstr(txt, "<#"));
        }

        /* LECTIO: identitas soluta - A' -> B', B' -> A' */
        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, &c,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);
        si (vitium.causa != NIHIL)
        {
            imprimere("  causa: %s (linea %d)\n", vitium.causa,
                (int)vitium.linea);
        }
        si (lecta != NIHIL)
        {
            MateriaNodus* a2 = _liberum_radicis(lecta, ZEPHYRUM);
            MateriaNodus* b2 = _liberum_radicis(lecta, (i32)I);

            CREDO_NON_NIHIL (a2);
            CREDO_NON_NIHIL (b2);
            CREDO_AEQUALIS_S32 (_gradum_filii(a2), (s32)I);
            CREDO_AEQUALIS_S32 (_gradum_filii(b2), (s32)II);
            CREDO_AEQUALIS_PTR (_socium_filii(a2), b2);
            CREDO_AEQUALIS_PTR (_socium_filii(b2), a2);
            /* scopus patrem a domino habet */
            CREDO_AEQUALIS_PTR (a2->pater, lecta);
            CREDO_AEQUALIS_PTR (b2->pater, lecta);

            /* COMPARATOR: semitae relativae aequales */
            CREDO_VERUM (materia_arbor_aequalis(radix, lecta,
                MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
            si (d.campus != NIHIL)
            {
                imprimere("  divergentia: %s @ %s\n", d.campus, d.via);
            }
        }

        /* CIRCUITUS BIS */
        s2 = materia_arbor_scribere_nodum(piscina, lecta, &c);
        CREDO_VERUM (s2.successus);
        CREDO_AEQUALIS_I32 (s2.textus.mensura, s1.textus.mensura);
        CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
            (size_t)s1.textus.mensura) == ZEPHYRUM);
        lecta2 = materia_arbor_legere(piscina, NIHIL, s2.textus, &c,
            &vitium);
        CREDO_NON_NIHIL (lecta2);
        s3 = materia_arbor_scribere_nodum(piscina, lecta2, &c);
        CREDO_VERUM (s3.successus);
        CREDO_AEQUALIS_I32 (s3.textus.mensura, s1.textus.mensura);
        CREDO_VERUM (memcmp(s1.textus.datum, s3.textus.datum,
            (size_t)s1.textus.mensura) == ZEPHYRUM);

        /* PERMUTATIO: liberi [B, A]. Nihil remittitur; referentiae
         * mutuae manent et ancorae nodum sequuntur - post lectionem
         * liber primus gradum II fert et socium liberum secundum. */
        {
                              i32 ordo[II];
            MateriaArborScriptura s4;
                     MateriaNodus* lecta_permutata;

            ordo[ZEPHYRUM]  = I;
            ordo[I]         = ZEPHYRUM;
            CREDO_VERUM (materia_nodus_lista_permutare(piscina, radix,
                (i32)LR_LIBERI, ordo, (i32)II));
            CREDO_AEQUALIS_PTR (_socium_filii(a), b);
            CREDO_AEQUALIS_PTR (_socium_filii(b), a);

            s4 = materia_arbor_scribere_nodum(piscina, radix, &c);
            CREDO_VERUM (s4.successus);
            {
                character* textus_permutatus =
                    (character*)s4.textus.datum;
                character* f1 = strstr(textus_permutatus,
                    "<filius id=\"nod1\" ((>");
                character* f2 = strstr(textus_permutatus,
                    "<filius id=\"nod2\" ((>");
                character* g2 = f1 ? strstr(f1,
                    "<gradus(> 2") : NIHIL;

                CREDO_NON_NIHIL (f1);
                CREDO_NON_NIHIL (f2);
                /* liber primus nunc gradum II fert sub ancora nod1 */
                CREDO_VERUM (f1 != NIHIL && f2 != NIHIL && g2 != NIHIL
                    && f1 < g2 && g2 < f2);
            }
            lecta_permutata = materia_arbor_legere(piscina, NIHIL,
                s4.textus, &c,
                &vitium);
            CREDO_NON_NIHIL (lecta_permutata);
            si (lecta_permutata != NIHIL)
            {
                MateriaNodus* primus = _liberum_radicis(lecta_permutata,
                    ZEPHYRUM);
                MateriaNodus* secundus =
                    _liberum_radicis(lecta_permutata,
                    (i32)I);

                CREDO_AEQUALIS_S32 (_gradum_filii(primus), (s32)II);
                CREDO_AEQUALIS_S32 (_gradum_filii(secundus), (s32)I);
                CREDO_AEQUALIS_PTR (_socium_filii(primus), secundus);
                CREDO_AEQUALIS_PTR (_socium_filii(secundus), primus);
                /* arbor permutata contra lectam: aequales */
                CREDO_VERUM (materia_arbor_aequalis(radix,
                    lecta_permutata,
                    MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
                /* contra lectam ANTE permutationem: semita divergit */
                CREDO_FALSUM (materia_arbor_aequalis(lecta,
                    lecta_permutata,
                    MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
            }
        }

        /* CUSTODIAE SCRIPTORIS */
        {
            MateriaArborScriptura s5;
                     MateriaNodus* solus;

            /* subarbor A sola: socius B EXTRA - recusatur cum sede */
            s5 = materia_arbor_scribere_nodum(piscina, a, &c);
            CREDO_FALSUM (s5.successus);
            CREDO_NON_NIHIL (s5.causa);
            CREDO_AEQUALIS_PTR ((constans vacuum*)s5.sedes,
                (constans vacuum*)b);

            /* referentia NIHIL recusatur */
            solus = _filium_struere(piscina, (s32)III);
            CREDO_VERUM (materia_nodus_ponere(solus, (i32)LR_SOCIUS - I,
                materia_valor_referentia(NIHIL),
                MATERIA_LOCUS_REFERENTIA));
            s5 = materia_arbor_scribere_nodum(piscina, solus, &c);
            CREDO_FALSUM (s5.successus);
        }

        /* CUSTODIAE LECTORIS: documenta manu scripta, sigillo vivo */
        {
               chorda sigillum = materia_arbor_sigillum(piscina,
                   &REG_REF);
            character doc[512];

            CREDO_VERUM (sigillum.mensura > ZEPHYRUM);

            /* ancora ignota */
            sprintf(doc,
                "<arbor grammatica=\"probatio\""
                " registrum-sigillum=\"%.*s\">"
                "<radix><liberi><filius id=\"nod1\">"
                "<gradus>1</gradus><socius>#nod9</socius>"
                "</filius></liberi></radix></arbor>",
                (int)sigillum.mensura,
                (constans character*)sigillum.datum);
            CREDO_NIHIL (materia_arbor_legere(piscina, NIHIL,
                chorda_ex_literis(doc, piscina), &c, &vitium));
            CREDO_NON_NIHIL (vitium.causa);
            CREDO_NON_NIHIL (strstr(vitium.causa, "ignotam"));

            /* ancora duplex */
            sprintf(doc,
                "<arbor grammatica=\"probatio\""
                " registrum-sigillum=\"%.*s\">"
                "<radix><liberi>"
                "<filius id=\"nod1\"><gradus>1</gradus></filius>"
                "<filius id=\"nod1\"><gradus>2</gradus></filius>"
                "</liberi></radix></arbor>",
                (int)sigillum.mensura,
                (constans character*)sigillum.datum);
            CREDO_NIHIL (materia_arbor_legere(piscina, NIHIL,
                chorda_ex_literis(doc, piscina), &c, &vitium));
            CREDO_NON_NIHIL (vitium.causa);
            CREDO_NON_NIHIL (strstr(vitium.causa, "duplex"));

            /* locus sine '#' */
            sprintf(doc,
                "<arbor grammatica=\"probatio\""
                " registrum-sigillum=\"%.*s\">"
                "<radix><liberi><filius id=\"nod1\">"
                "<gradus>1</gradus><socius>nod1</socius>"
                "</filius></liberi></radix></arbor>",
                (int)sigillum.mensura,
                (constans character*)sigillum.datum);
            CREDO_NIHIL (materia_arbor_legere(piscina, NIHIL,
                chorda_ex_literis(doc, piscina), &c, &vitium));
            CREDO_NON_NIHIL (vitium.causa);

            /* sanum: se ipsum referens (circuitus minimus) */
            sprintf(doc,
                "<arbor grammatica=\"probatio\""
                " registrum-sigillum=\"%.*s\">"
                "<radix><liberi><filius id=\"nod1\">"
                "<gradus>1</gradus><socius>#nod1</socius>"
                "</filius></liberi></radix></arbor>",
                (int)sigillum.mensura,
                (constans character*)sigillum.datum);
            {
                MateriaNodus* ipse = materia_arbor_legere(piscina,
                    NIHIL,
                    chorda_ex_literis(doc, piscina), &c, &vitium);

                CREDO_NON_NIHIL (ipse);
                si (ipse != NIHIL)
                {
                    MateriaNodus* f = _liberum_radicis(ipse, ZEPHYRUM);

                    CREDO_AEQUALIS_PTR (_socium_filii(f), f);
                }
            }
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
