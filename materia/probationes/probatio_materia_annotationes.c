/* probatio_materia_annotationes.c - Collector annotationum STML
 * (project-specs/excusatio-declarata-spec.md par. III)
 *
 * Arbor MANU aedificata, non per clientem: collector GENERALIS est,
 * ergo probatio eius crustam non tangat. Registrum manu scriptum
 * (radix, par, verbum) cum lexico cuius genus COMMENTUM munus
 * MATERIA_MUNUS_COMMENTUM fert.
 *
 * FIXTURA CONSULTO DISPAR EST: par prius octetos 37-44 tenet,
 * par alterum 45-51. Lex huius arcus: 'fixtura cuius campi duo
 * AEQUALES sunt ea discernere non potest' - si ambo pares eandem
 * sedem haberent, scopus nimis latus (ad parentem promotus) viridis
 * maneret.
 */

#include "latina.h"
#include "credo.h"
#include "materia_annotationes.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_lexicon.h"
#include "chorda.h"
#include "piscina.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

enumeratio {
    G_FINIS = 0, G_IDENT, G_SPATIA, G_NOVA_LINEA, G_COMMENTUM,
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
        MATERIA_MUNUS_COMMENTUM }
};
hic_manens constans MateriaLexiconCoctum LEXICON = {
    GENERA, (i32)G_NUMERUS_GENERUM, "lex-", (s32)G_SPATIA
};

/* REGISTRUM NULLUM: collector lexicon solum poscit (quod genus
 * commentarium sit), non vocabularium nodorum - genera nodorum hic
 * indices nudi manent. Id contractum ANGUSTIOREM facit quam
 * materia_diagnostica, quod tabulam registri legit. */
enumeratio { GR_RADIX = 0, GR_PAR, GR_VERBUM };
enumeratio { LP_APERTURA = 0, LP_LIBERI, LP_CLAUSURA };

/* '# <tolera codex="lint:x" (>causa vera' = XXXVII octeti */
#define NOTA_BONA  "# <tolera codex=\"lint:x\" (>causa vera"

hic_manens MateriaToken*
_lex_linea (
               Piscina* piscina,
                   s32  genus,
    constans character* valor,
                   s32  offset,
                   i32  linea)
{
    redde materia_token_creare(piscina, &FORMA, genus,
        chorda_ex_literis(valor, piscina), offset, linea,
        (i32)(offset + I), ZEPHYRUM);
}

hic_manens MateriaToken*
_lex (
               Piscina* piscina,
                   s32  genus,
    constans character* valor,
                   s32  offset)
{
    redde _lex_linea(piscina, genus, valor, offset, (i32)I);
}

/* par{apertura '(' , liberi[verbum], clausura ')'} ab offset dato */
hic_manens MateriaNodus*
_par (
               Piscina* piscina,
    constans character* interius,
                   s32  offset,
    constans character* commentarium)
{
    MateriaNodus* nodus = materia_nodus_creare(piscina, (s32)GR_PAR,
                              (i32)III);
    MateriaNodus* intus;
    MateriaToken* apertura;
             s32  post;

    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    apertura = _lex(piscina, (s32)G_IDENT, "(", offset);
    si (commentarium != NIHIL)
    {
        MateriaToken* nota = _lex(piscina, (s32)G_COMMENTUM,
                                 commentarium, ZEPHYRUM);

        si (!materia_token_trivia_ante_ponere(apertura, piscina,
                &nota, (i32)I))
        {
            redde NIHIL;
        }
    }
    si (!materia_nodus_ponere(nodus, (i32)LP_APERTURA,
            materia_valor_token(apertura), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    intus = materia_nodus_creare(piscina, (s32)GR_VERBUM, (i32)I);
    si (intus == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(intus, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, interius,
                offset + I)), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    si (!materia_nodus_appendere(piscina, nodus, (i32)LP_LIBERI,
            materia_valor_nodus(intus), MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    post = offset + I + (s32)strlen(interius);
    si (!materia_nodus_ponere(nodus, (i32)LP_CLAUSURA,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, ")",
                post)), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde nodus;
}

/* radix{liberi[par(XXXVII, cum nota), par(XLV, sine nota)]} */
hic_manens MateriaNodus*
_arbor (
               Piscina* piscina,
    constans character* commentarium)
{
    MateriaNodus* radix = materia_nodus_creare(piscina,
                              (s32)GR_RADIX, (i32)I);
    MateriaNodus* prius;
    MateriaNodus* alterum;

    si (radix == NIHIL)
    {
        redde NIHIL;
    }
    prius    = _par(piscina, "alpha", (s32)37, commentarium);
    alterum  = _par(piscina, "beta",  (s32)45, NIHIL);
    si (prius == NIHIL || alterum == NIHIL)
    {
        redde NIHIL;
    }
    si (   !materia_nodus_appendere(piscina, radix, ZEPHYRUM,
               materia_valor_nodus(prius), MATERIA_LOCUS_LISTA_NODUS)
        || !materia_nodus_appendere(piscina, radix, ZEPHYRUM,
               materia_valor_nodus(alterum),
               MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    redde radix;
}

/* Arbor cauda: par(XXXVII) commentarium in trivio POST clausurae
 * suae fert, in linea data; par(XLV) sequitur. Linea I = cauda
 * lineae clausurae (adnexio RETRO); linea II = linea propria
 * (adnexio ANTRORSUM). */
hic_manens Xar*
_arbor_cauda (
                          Piscina* piscina,
     constans MateriaLexiconRatum* ratum,
               constans character* commentarium,
                              i32  linea)
{
    MateriaNodus* radix = materia_nodus_creare(piscina,
                              (s32)GR_RADIX, (i32)I);
    MateriaNodus* prius;
    MateriaNodus* alterum;
    MateriaToken* clausura;
    MateriaToken* nota;

    si (radix == NIHIL)
    {
        redde NIHIL;
    }
    prius = materia_nodus_creare(piscina, (s32)GR_PAR, (i32)III);
    si (prius == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(prius, (i32)LP_APERTURA,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "(",
                (s32)37)), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    clausura = _lex(piscina, (s32)G_IDENT, ")", (s32)43);
    nota     = _lex_linea(piscina, (s32)G_COMMENTUM, commentarium,
                   (s32)45, linea);
    si (!materia_token_trivia_post_ponere(clausura, piscina, &nota,
            (i32)I))
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(prius, (i32)LP_CLAUSURA,
            materia_valor_token(clausura), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    alterum = _par(piscina, "beta", (s32)90, NIHIL);
    si (alterum == NIHIL)
    {
        redde NIHIL;
    }
    si (   !materia_nodus_appendere(piscina, radix, ZEPHYRUM,
               materia_valor_nodus(prius), MATERIA_LOCUS_LISTA_NODUS)
        || !materia_nodus_appendere(piscina, radix, ZEPHYRUM,
               materia_valor_nodus(alterum),
               MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    redde materia_annotationes_colligere(piscina, radix, ratum, "#",
        NIHIL, NIHIL);
}

integer
principale (
    vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_annotationes",
        262144);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &LEXICON,
        &iudicium));

    {
        MateriaNodus* radix = _arbor(piscina, NOTA_BONA);
                 Xar* a;
    MateriaAnnotatio* prima;
              chorda* codex;

        imprimere("\n--- I. Annotatio una, ambitus = nodus ---\n");
        CREDO_NON_NIHIL (radix);
        a = materia_annotationes_colligere(piscina, radix, &ratum,
                "#", NIHIL, NIHIL);
        CREDO_NON_NIHIL (a);
        CREDO_AEQUALIS_I32 (xar_numerus(a), (i32)I);
        prima = (MateriaAnnotatio*)xar_obtinere(a, ZEPHYRUM);
        CREDO_VERUM (prima->parsata);
        CREDO_NON_NIHIL (prima->arbor);
        CREDO_NON_NIHIL (prima->arbor->titulus);
        CREDO_VERUM (chorda_aequalis_literis(*prima->arbor->titulus,
            "tolera"));
        codex = stml_attributum_capere(prima->arbor, "codex");
        CREDO_NON_NIHIL (codex);
        CREDO_VERUM (chorda_aequalis_literis(*codex, "lint:x"));

        imprimere("\n--- II. Sedes commentarii EXTRA nodum ---\n");
        CREDO_AEQUALIS_S32 (prima->commentarium.initium, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (prima->commentarium.finis, (s32)37);

        imprimere("\n--- III. Scopus par PRIUS tegit ---\n");
        /* par prius XXXVII-XLIV; par alterum XLV-LI. Si scopus ad
         * parentem promoveatur, finis LI fieret et par alterum
         * TACERET - hoc est quod planta portae huius probat. */
        CREDO_AEQUALIS_S32 (prima->scopus.initium, (s32)37);
        CREDO_AEQUALIS_S32 (prima->scopus.finis, (s32)44);
        CREDO_VERUM (prima->scopus.finis < (s32)45);
    }

    {
        Xar* a;

        imprimere("\n--- IV. Prosa omittitur ---\n");
        a = materia_annotationes_colligere(piscina,
                _arbor(piscina, "# commentarium merum, nulla tag"),
                &ratum, "#", NIHIL, NIHIL);
        CREDO_NON_NIHIL (a);
        CREDO_AEQUALIS_I32 (xar_numerus(a), ZEPHYRUM);
    }

    {
                 Xar* a;
    MateriaAnnotatio* prima;

        imprimere("\n--- V. Malformatum RETINETUR ---\n");
        a = materia_annotationes_colligere(piscina,
                _arbor(piscina, "# <tolera codex=\"x\""),
                &ratum, "#", NIHIL, NIHIL);
        CREDO_NON_NIHIL (a);
        CREDO_AEQUALIS_I32 (xar_numerus(a), (i32)I);
        prima = (MateriaAnnotatio*)xar_obtinere(a, ZEPHYRUM);
        CREDO_FALSUM (prima->parsata);
        /* sedes SERVATUR: consumptor id nominare debet */
        CREDO_AEQUALIS_S32 (prima->commentarium.initium, ZEPHYRUM);
    }

    {
                 Xar* a;
    MateriaAnnotatio* prima;

        imprimere("\n--- VI. Praefixum NIHIL: nulla exuvia ---\n");
        a = materia_annotationes_colligere(piscina,
                _arbor(piscina, "<tolera codex=\"y\" (>causa"),
                &ratum, NIHIL, NIHIL, NIHIL);
        CREDO_NON_NIHIL (a);
        CREDO_AEQUALIS_I32 (xar_numerus(a), (i32)I);
        prima = (MateriaAnnotatio*)xar_obtinere(a, ZEPHYRUM);
        CREDO_VERUM (prima->parsata);
    }

    {
                 Xar* a;
    MateriaAnnotatio* prima;

        imprimere("\n--- VII. Cauda: linea PROPRIA -> antrorsum ---\n");
        /* Commentarium post ')' paris prioris sed in linea PROPRIA
         * id annotat quod SEQUITUR - par alterum (XLV-LI). Ante
         * emendationem operis V possessorem TRIVII accipiebat (par
         * prius, aut separatorem) et excusatio aut mortua erat aut
         * nimis lata. */
        a = _arbor_cauda(piscina, &ratum, NOTA_BONA, (i32)II);
        CREDO_NON_NIHIL (a);
        CREDO_AEQUALIS_I32 (xar_numerus(a), (i32)I);
        prima = (MateriaAnnotatio*)xar_obtinere(a, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (prima->scopus.initium, (s32)90);
        CREDO_AEQUALIS_S32 (prima->scopus.finis, (s32)96);
    }

    {
                 Xar* a;
    MateriaAnnotatio* prima;

        imprimere("\n--- VIII. Cauda: linea EADEM -> retro ---\n");
        /* Lex silvae 'linea eadem aut praecedens': commentarium in
         * cauda lineae id annotat quod in EA linea iacet. Par cum
         * VII - eadem arbor, linea sola mutata, sedes DIVERSA. */
        a = _arbor_cauda(piscina, &ratum, NOTA_BONA, (i32)I);
        CREDO_NON_NIHIL (a);
        CREDO_AEQUALIS_I32 (xar_numerus(a), (i32)I);
        prima = (MateriaAnnotatio*)xar_obtinere(a, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (prima->scopus.initium, (s32)37);
        CREDO_AEQUALIS_S32 (prima->scopus.finis, (s32)44);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
