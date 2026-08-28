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
    { "FINIS",      "",    MATERIA_LEX_FIXUM,      MATERIA_MUNUS_FINIS },
    { "IDENT",      NIHIL, MATERIA_LEX_VERBATIM,   MATERIA_MUNUS_SUBSTANTIVUM },
    { "SPATIA",     " ",   MATERIA_LEX_REPETITUM,  MATERIA_MUNUS_SPATIUM },
    { "NOVA_LINEA", "\n",  MATERIA_LEX_TERMINATOR, MATERIA_MUNUS_LINEA },
    { "COMMENTUM",  NIHIL, MATERIA_LEX_VERBATIM,   MATERIA_MUNUS_COMMENTUM },
    { "PLUS",       "+",   MATERIA_LEX_FIXUM,      MATERIA_MUNUS_SUBSTANTIVUM },
    { "ALBUM",      NIHIL, MATERIA_LEX_VERBATIM,   MATERIA_MUNUS_SPATIUM }
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
        MateriaNodus*         n;
        MateriaToken*         t1;
        MateriaToken*         t2;
        MateriaToken*         tr[2];
        MateriaArborScriptura s1;
        MateriaArborScriptura s2;
        MateriaArborScriptura s3;
        MateriaNodus*         lecta;
        MateriaNodus*         lecta2;
        MateriaArborVitium    vitium;

        imprimere("\n--- Probans circuitum (BIS) ---\n");

        /* '    a+b' : trivium ducens, duo lexemata, unum fixum */
        tr[0] = _lex(piscina, (s32)G_SPATIA, "    ", ZEPHYRUM, (i32)I, (i32)I);
        t1    = _lex(piscina, (s32)G_IDENT, "a", (s32)4, (i32)I, (i32)5);
        t2    = _lex(piscina, (s32)G_PLUS,  "+", (s32)5, (i32)I, (i32)6);
        CREDO_VERUM (materia_token_trivia_ante_ponere(t1, piscina, tr, (i32)I));

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(t1), MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(t2), MATERIA_LOCUS_LISTA_MIXTA));

        materia_arbor_consilium_nudum(&c, &REG, &ratum, "probatio");
        s1 = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s1.successus);

        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, &c, &vitium);
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);

        s2 = materia_arbor_scribere_nodum(piscina, lecta, &c);
        CREDO_VERUM (s2.successus);
        CREDO_AEQUALIS_I32 (s2.textus.mensura, s1.textus.mensura);
        CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
            (size_t)s1.textus.mensura) == ZEPHYRUM);

        /* CIRCUITUS SECUNDUS */
        lecta2 = materia_arbor_legere(piscina, NIHIL, s2.textus, &c, &vitium);
        CREDO_NON_NIHIL (lecta2);
        s3 = materia_arbor_scribere_nodum(piscina, lecta2, &c);
        CREDO_VERUM (s3.successus);
        CREDO_AEQUALIS_I32 (s3.textus.mensura, s1.textus.mensura);
        CREDO_VERUM (memcmp(s1.textus.datum, s3.textus.datum,
            (size_t)s1.textus.mensura) == ZEPHYRUM);

        /* SEDES DERIVATAE: documentum eas non fert, lector eas
         * reficit. Lexema primum ad IV (post trivium), '+' ad V. */
        {
            MateriaValor* e0 = materia_valor_lista_obtinere(lecta->loci[0],
                ZEPHYRUM);
            MateriaValor* e1 = materia_valor_lista_obtinere(lecta->loci[0],
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
        MateriaArborVitium    vitium;
        MateriaNodus*         n;
        MateriaArborScriptura s;

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
            chorda cum_nul;
            MateriaArborScriptura s3;

            cum_nul.mensura = (i32)II;
            cum_nul.datum   = (i8*)piscina_allocare(piscina,
                (memoriae_index)II);
            cum_nul.datum[ZEPHYRUM] = (i8)' ';
            cum_nul.datum[I]        = (i8)'\0';

            malum = materia_token_creare(piscina, &FORMA, (s32)G_ALBUM,
                cum_nul, ZEPHYRUM, (i32)I, (i32)I, ZEPHYRUM);
            r2 = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
            CREDO_VERUM (materia_nodus_appendere(piscina, r2, ZEPHYRUM,
                materia_valor_token(malum), MATERIA_LOCUS_LISTA_MIXTA));
            s3 = materia_arbor_scribere_nodum(piscina, r2, &c);
            CREDO_FALSUM (s3.successus);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
