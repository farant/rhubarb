/* probatio_iussum.c - iussa in textu (vicus-latera S3a)
 *
 * Tabula XX x I; linea ex literis ponitur, reliquum cellulae vacuae
 * ('\0', ut pagina scribae). I: iussum simplex et fines (columna
 * ante, '$', ultima, post). II: argumenta (spatia dempta, '()',
 * argumentum vacuum, ')' in ultima columna). III: non iussa ('a$b',
 * '$5', '$Magnus', '$' ultimum, '$foo(' apertum, '$-x'); '($dies)'
 * iussum est. IV: verba ignota prosa manent. V: proximum per lineam
 * ordine. VI: exitus copia est (tabula postea mutata). VII: argumenta
 * mala recusantur. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_characterum.h"
#include "iussum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LATITUDO XX

hic_manens Piscina*          piscina;
hic_manens TabulaCharacterum tabula;

/* linea 0 = textus, ceterae cellulae '\0' */
interior vacuum
ponere (
    constans character* textus)
{
    memset(tabula.cellulae, ZEPHYRUM, (memoriae_index)LATITUDO);
    memcpy(tabula.cellulae, textus, strlen(textus));
}

interior b32
ad (
       s32  columna,
    Iussum* i)
{
    redde iussum_ad_locum(&tabula, ZEPHYRUM, columna, NIHIL, NIHIL,
        piscina, i);
}

/* verba nota: 'dies' et 'scriba' solum */
interior b32
notum (
    chorda  verbum,
    vacuum* ctx)
{
    (vacuum)ctx;
    redde chorda_aequalis_literis(verbum, "dies")
        || chorda_aequalis_literis(verbum, "scriba");
}

s32 principale (vacuum)
{
    Iussum i;
       s32 c;

    piscina = piscina_generare_dynamicum("probatio_iussum", LXIV * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    tabula_initiare(&tabula, piscina, LATITUDO, I);

    imprimere("\n--- I: iussum simplex, fines ---\n");
    ponere("vide $terminale x");
    CREDO_FALSUM(ad(IV, &i));
    CREDO_VERUM(ad(V, &i));
    CREDO_CHORDA_AEQUALIS_LITERIS(i.verbum, "terminale");
    CREDO_AEQUALIS_S32(i.linea, ZEPHYRUM);
    CREDO_AEQUALIS_S32(i.initium, V);
    CREDO_AEQUALIS_S32(i.finis, XV);
    CREDO_AEQUALIS_I32(i.numerus_argumentorum, ZEPHYRUM);
    CREDO_VERUM(ad(XIV, &i));
    CREDO_FALSUM(ad(XV, &i));
    /* verbum ad finem lineae, cellulae vacuae post */
    ponere("$dies");
    CREDO_VERUM(ad(ZEPHYRUM, &i));
    CREDO_AEQUALIS_S32(i.finis, V);
    ponere("abcdefghijklmn $dies");
    CREDO_VERUM(ad(XIX, &i));
    CREDO_AEQUALIS_S32(i.finis, XX);

    imprimere("\n--- II: argumenta ---\n");
    ponere("$scriba( notae , b )");
    CREDO_VERUM(ad(X, &i));
    CREDO_CHORDA_AEQUALIS_LITERIS(i.verbum, "scriba");
    CREDO_AEQUALIS_S32(i.finis, XX);
    CREDO_AEQUALIS_I32(i.numerus_argumentorum, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(i.argumenta[ZEPHYRUM], "notae");
    CREDO_CHORDA_AEQUALIS_LITERIS(i.argumenta[I], "b");
    /* ')' in ultima columna tegit */
    CREDO_VERUM(ad(XIX, &i));
    ponere("$f() z");
    CREDO_VERUM(ad(III, &i));
    CREDO_AEQUALIS_S32(i.finis, IV);
    CREDO_AEQUALIS_I32(i.numerus_argumentorum, ZEPHYRUM);
    CREDO_FALSUM(ad(IV, &i));
    ponere("$f(a,)");
    CREDO_VERUM(ad(ZEPHYRUM, &i));
    CREDO_AEQUALIS_I32(i.numerus_argumentorum, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(i.argumenta[ZEPHYRUM], "a");
    CREDO_AEQUALIS_I32(i.argumenta[I].mensura, ZEPHYRUM);
    ponere("$f(   )");
    CREDO_VERUM(ad(ZEPHYRUM, &i));
    CREDO_AEQUALIS_I32(i.numerus_argumentorum, ZEPHYRUM);
    /* verbum cum numeris et '_' */
    ponere("$mensis_2(2027, 8)");
    CREDO_VERUM(ad(ZEPHYRUM, &i));
    CREDO_CHORDA_AEQUALIS_LITERIS(i.verbum, "mensis_2");
    CREDO_CHORDA_AEQUALIS_LITERIS(i.argumenta[I], "8");

    imprimere("\n--- III: non iussa ---\n");
    ponere("a$b");
    CREDO_FALSUM(ad(I, &i));
    ponere("x_$dies");
    CREDO_FALSUM(ad(II, &i));
    ponere("$5.00");
    CREDO_FALSUM(ad(ZEPHYRUM, &i));
    ponere("$Magnus");
    CREDO_FALSUM(ad(ZEPHYRUM, &i));
    ponere("$-x");
    CREDO_FALSUM(ad(ZEPHYRUM, &i));
    ponere("abcdefghijklmnopqr $");
    CREDO_FALSUM(ad(XIX, &i));
    ponere("$foo(abc");
    CREDO_FALSUM(ad(ZEPHYRUM, &i));
    CREDO_FALSUM(ad(II, &i));
    /* ')' in linea sequenti non valet: linea una */
    ponere("abcdefghijklm $foo(x");
    CREDO_FALSUM(ad(XV, &i));
    ponere("($dies) et");
    CREDO_VERUM(ad(I, &i));
    CREDO_CHORDA_AEQUALIS_LITERIS(i.verbum, "dies");
    CREDO_FALSUM(ad(ZEPHYRUM, &i));
    CREDO_FALSUM(ad(VI, &i));

    imprimere("\n--- IV: verba ignota prosa manent ---\n");
    ponere("$foo $dies");
    CREDO_FALSUM(iussum_ad_locum(&tabula, ZEPHYRUM, I, notum, NIHIL,
        piscina, &i));
    CREDO_VERUM(iussum_ad_locum(&tabula, ZEPHYRUM, VI, notum, NIHIL,
        piscina, &i));
    CREDO_CHORDA_AEQUALIS_LITERIS(i.verbum, "dies");
    CREDO_VERUM(iussum_proximum(&tabula, ZEPHYRUM, ZEPHYRUM, notum,
        NIHIL,
        piscina, &i));
    CREDO_AEQUALIS_S32(i.initium, V);
    /* ignotum cum argumentis apertis: nec iussum nec impedimentum */
    ponere("$foo( $scriba(a)");
    CREDO_VERUM(iussum_proximum(&tabula, ZEPHYRUM, ZEPHYRUM, notum,
        NIHIL,
        piscina, &i));
    CREDO_CHORDA_AEQUALIS_LITERIS(i.verbum, "scriba");
    CREDO_AEQUALIS_S32(i.initium, VI);

    imprimere("\n--- V: proximum per lineam ---\n");
    ponere("$a $b(x) zz $c");
    c = ZEPHYRUM;
    CREDO_VERUM(iussum_proximum(&tabula, ZEPHYRUM, c, NIHIL, NIHIL,
        piscina, &i));
    CREDO_CHORDA_AEQUALIS_LITERIS(i.verbum, "a");
    c = i.finis;
    CREDO_VERUM(iussum_proximum(&tabula, ZEPHYRUM, c, NIHIL, NIHIL,
        piscina, &i));
    CREDO_CHORDA_AEQUALIS_LITERIS(i.verbum, "b");
    CREDO_AEQUALIS_S32(i.finis, VIII);
    c = i.finis;
    CREDO_VERUM(iussum_proximum(&tabula, ZEPHYRUM, c, NIHIL, NIHIL,
        piscina, &i));
    CREDO_CHORDA_AEQUALIS_LITERIS(i.verbum, "c");
    CREDO_AEQUALIS_S32(i.initium, XII);
    c = i.finis;
    CREDO_FALSUM(iussum_proximum(&tabula, ZEPHYRUM, c, NIHIL, NIHIL,
        piscina, &i));
    /* ad locum post iussum primum: secundum invenit */
    CREDO_VERUM(ad(V, &i));
    CREDO_CHORDA_AEQUALIS_LITERIS(i.verbum, "b");
    CREDO_FALSUM(ad(IX, &i));

    imprimere("\n--- VI: exitus copia ---\n");
    ponere("$dies(x)");
    CREDO_VERUM(ad(ZEPHYRUM, &i));
    ponere("$zzzz(y)");
    CREDO_CHORDA_AEQUALIS_LITERIS(i.verbum, "dies");
    CREDO_CHORDA_AEQUALIS_LITERIS(i.argumenta[ZEPHYRUM], "x");

    imprimere("\n--- VII: argumenta mala ---\n");
    ponere("$dies");
    CREDO_FALSUM(iussum_ad_locum(NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL,
        NIHIL,
        piscina, &i));
    CREDO_FALSUM(iussum_ad_locum(&tabula, I, ZEPHYRUM, NIHIL, NIHIL,
        piscina, &i));
    CREDO_FALSUM(iussum_ad_locum(&tabula, -I, ZEPHYRUM, NIHIL, NIHIL,
        piscina, &i));
    CREDO_FALSUM(iussum_ad_locum(&tabula, ZEPHYRUM, -I, NIHIL, NIHIL,
        piscina, &i));
    CREDO_FALSUM(iussum_ad_locum(&tabula, ZEPHYRUM, XX, NIHIL, NIHIL,
        piscina, &i));
    CREDO_FALSUM(iussum_proximum(&tabula, ZEPHYRUM, ZEPHYRUM, NIHIL,
        NIHIL, NIHIL, &i));
    CREDO_FALSUM(iussum_proximum(&tabula, ZEPHYRUM, ZEPHYRUM, NIHIL,
        NIHIL, piscina, NIHIL));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
