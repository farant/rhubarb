/* probatio_differentia.c - Probationes differentiae
 *
 * Trium generum: (1) invarians partitionis - custos qui ipse
 * probatur vitio PLANTATO (tractus corruptus manu constructus
 * recusari DEBET); (2) oracula: emissio unificata contra 'diff -U'
 * GNU manu verificata (numeri @@ et lineae byte pro byte, tituli
 * et titulus notae sine-fine nostri); (3) percursus fortuitus
 * deterministicus (LCG semine constanti) - invarians + symmetria
 * super centena paria brevia.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "differentia.h"
#include "credo.h"

#include <stdio.h>

/* ==================================================
 * Identitas litterarum: littera eadem -> monstrator idem
 * ================================================== */

hic_manens character _alphabetum[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* visus lectionis: vacuum** -> typus parametri nuclei (constantia
 * omnibus gradibus addita - cast quem -Wcast-qual benedicit) */
#define LECTIO(ordo) ((constans vacuum* constans*)(ordo))

interior vacuum*
_identitas_litterae (character c);

interior vacuum*
_identitas_litterae (character c)
{
    i32 i;

    per (i = 0; _alphabetum[i] != '\0'; i = i + 1)
    {
        si (_alphabetum[i] == c)
        {
            redde (vacuum*)&_alphabetum[i];
        }
    }
    redde NIHIL;
}

interior vacuum**
_ordo_ex_literis (Piscina* piscina, constans character* literae,
    i32* numerus);

interior vacuum**
_ordo_ex_literis (Piscina* piscina, constans character* literae,
    i32* numerus)
{
    i32      n = 0;
    i32      i;
    vacuum** ordo;

    dum (literae[n] != '\0')
    {
        n = n + 1;
    }
    ordo = (vacuum**)piscina_allocare(piscina,
        (memoriae_index)(n == 0 ? I : n) * magnitudo(vacuum*));
    per (i = 0; i < n; i = i + 1)
    {
        ordo[i] = _identitas_litterae(literae[i]);
    }
    *numerus = n;
    redde ordo;
}

/* ==================================================
 * Invarians partitionis (vide differentia.h)
 * ================================================== */

interior b32
_invarians_probare (constans Xar* tractus, i32 numerus_a,
    i32 numerus_b, constans vacuum* constans* a,
    constans vacuum* constans* b);

interior b32
_invarians_probare (constans Xar* tractus, i32 numerus_a,
    i32 numerus_b, constans vacuum* constans* a,
    constans vacuum* constans* b)
{
    i32 pos_a = 0;
    i32 pos_b = 0;
    s32 prior = -1;
    i32 i;

    si (tractus == NIHIL)
    {
        redde FALSUM;
    }
    per (i = 0; i < xar_numerus(tractus); i = i + 1)
    {
        constans DifferentiaTractus* t =
            (constans DifferentiaTractus*)xar_obtinere(tractus, i);

        si (t->numerus == 0)
        {
            redde FALSUM;   /* tractus vacuus vetitus */
        }
        si (t->index_a != pos_a || t->index_b != pos_b)
        {
            redde FALSUM;   /* indices ambulationi non congruunt */
        }
        si ((s32)t->genus == prior)
        {
            redde FALSUM;   /* vicini eiusdem generis - fusio */
        }
        si (prior == (s32)DIFFERENTIA_ADDITA
            && t->genus == DIFFERENTIA_DELETA)
        {
            redde FALSUM;   /* canon: DELETA ante ADDITA */
        }
        si (t->genus == DIFFERENTIA_IDEM)
        {
            si (a != NIHIL && b != NIHIL)
            {
                i32 l;

                per (l = 0; l < t->numerus; l = l + 1)
                {
                    si (a[pos_a + l] != b[pos_b + l])
                    {
                        redde FALSUM;   /* IDEM mendax */
                    }
                }
            }
            pos_a = pos_a + t->numerus;
            pos_b = pos_b + t->numerus;
        }
        alioquin si (t->genus == DIFFERENTIA_DELETA)
        {
            pos_a = pos_a + t->numerus;
        }
        alioquin
        {
            pos_b = pos_b + t->numerus;
        }
        prior = (s32)t->genus;
    }
    redde pos_a == numerus_a && pos_b == numerus_b;
}

/* differentia + invarians uno gestu */
interior Xar*
_differre_litteris (Piscina* piscina, constans character* a,
    constans character* b);

interior Xar*
_differre_litteris (Piscina* piscina, constans character* a,
    constans character* b)
{
    i32      na;
    i32      nb;
    vacuum** ordo_a = _ordo_ex_literis(piscina, a, &na);
    vacuum** ordo_b = _ordo_ex_literis(piscina, b, &nb);
    Xar*     tractus = differentia_seriei(piscina, LECTIO(ordo_a),
        na, LECTIO(ordo_b), nb);

    CREDO_NON_NIHIL(tractus);
    CREDO_VERUM(_invarians_probare(tractus, na, nb,
        LECTIO(ordo_a), LECTIO(ordo_b)));
    redde tractus;
}

/* ==================================================
 * LCG deterministicum (fors sine tempore)
 * ================================================== */

hic_manens i32 _semen = 88;

interior i32
_fors (i32 modus);

interior i32
_fors (i32 modus)
{
    _semen = _semen * (i32)1664525 + (i32)1013904223;
    redde (_semen >> 16) % modus;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_differentia",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: series - casus limitis
     * ======================================================== */

    {
        Xar* tractus;

        imprimere("\n--- Probans seriem (limites) ---\n");

        /* ambo vacui */
        tractus = _differre_litteris(piscina, "", "");
        CREDO_AEQUALIS_I32((i32)xar_numerus(tractus), (i32)0);

        /* idem totum */
        tractus = _differre_litteris(piscina, "abc", "abc");
        CREDO_AEQUALIS_I32((i32)xar_numerus(tractus), (i32)1);
        {
            DifferentiaTractus* t = (DifferentiaTractus*)
                xar_obtinere(tractus, 0);

            CREDO_VERUM(t->genus == DIFFERENTIA_IDEM);
            CREDO_AEQUALIS_I32(t->numerus, (i32)3);
        }

        /* a vacuum -> ADDITA unus */
        tractus = _differre_litteris(piscina, "", "abc");
        CREDO_AEQUALIS_I32((i32)xar_numerus(tractus), (i32)1);
        CREDO_VERUM(((DifferentiaTractus*)xar_obtinere(tractus,
            0))->genus == DIFFERENTIA_ADDITA);

        /* b vacuum -> DELETA unus */
        tractus = _differre_litteris(piscina, "abc", "");
        CREDO_AEQUALIS_I32((i32)xar_numerus(tractus), (i32)1);
        CREDO_VERUM(((DifferentiaTractus*)xar_obtinere(tractus,
            0))->genus == DIFFERENTIA_DELETA);

        /* substitutio media: canon DELETA ante ADDITA */
        tractus = _differre_litteris(piscina, "axc", "ayc");
        CREDO_AEQUALIS_I32((i32)xar_numerus(tractus), (i32)4);
        CREDO_VERUM(((DifferentiaTractus*)xar_obtinere(tractus,
            1))->genus == DIFFERENTIA_DELETA);
        CREDO_VERUM(((DifferentiaTractus*)xar_obtinere(tractus,
            2))->genus == DIFFERENTIA_ADDITA);
    }

    /* ========================================================
     * PROBARE: exemplum chartae Myersianae (distantia minima)
     * ======================================================== */

    {
        Xar*             tractus;
        DifferentiaSumma s;

        imprimere("\n--- Probans exemplum Myersianum ---\n");

        /* ABCABBA -> CBABAC: distantia editionis minima = V
         * (exemplum ex charta ipsa Myersi 1986) */
        tractus = _differre_litteris(piscina, "ABCABBA", "CBABAC");
        s = differentia_summa(tractus);
        CREDO_AEQUALIS_I32((i32)(s.additae + s.deletae), (i32)5);
    }

    /* ========================================================
     * PROBARE: custos ipse - vitium plantatum recusatur
     * ======================================================== */

    {
        Xar*     corruptus;
        i32      na;
        i32      nb;
        vacuum** ordo_a = _ordo_ex_literis(piscina, "ab", &na);
        vacuum** ordo_b = _ordo_ex_literis(piscina, "ab", &nb);

        imprimere("\n--- Probans custodem (vitium plantatum) ---\n");

        /* tractus IDEM numero MENDACI (III pro II) - partitionem
         * violat; custos qui hoc transmittit mortuus est */
        corruptus = xar_creare(piscina,
            (i32)magnitudo(DifferentiaTractus));
        {
            DifferentiaTractus* t = (DifferentiaTractus*)
                xar_addere(corruptus);

            t->genus = DIFFERENTIA_IDEM;
            t->index_a = 0;
            t->index_b = 0;
            t->numerus = 3;
        }
        CREDO_FALSUM(_invarians_probare(corruptus, na, nb,
            LECTIO(ordo_a), LECTIO(ordo_b)));

        /* et index mendax */
        {
            DifferentiaTractus* t = (DifferentiaTractus*)
                xar_obtinere(corruptus, 0);

            t->numerus = 2;
            t->index_a = 1;
        }
        CREDO_FALSUM(_invarians_probare(corruptus, na, nb,
            LECTIO(ordo_a), LECTIO(ordo_b)));
    }

    /* ========================================================
     * PROBARE: percursus fortuitus (invarians + symmetria)
     * ======================================================== */

    {
        i32 vicis;
        i32 praeteriti = 0;

        imprimere("\n--- Probans percursum fortuitum (CC casus) ---\n");

        per (vicis = 0; vicis < 200; vicis = vicis + 1)
        {
            character         litterae_a[32];
            character         litterae_b[32];
            i32               longitudo_a = _fors(25);
            i32               longitudo_b = _fors(25);
            i32               i;
            i32              na;
            i32              nb;
            vacuum**         ordo_a;
            vacuum**         ordo_b;
            Xar*             rectus;
            Xar*             conversus;
            DifferentiaSumma s_rectus;
            DifferentiaSumma s_conversus;

            per (i = 0; i < longitudo_a; i = i + 1)
            {
                litterae_a[i] = (character)('a' + (character)
                    _fors(4));
            }
            litterae_a[longitudo_a] = '\0';
            per (i = 0; i < longitudo_b; i = i + 1)
            {
                litterae_b[i] = (character)('a' + (character)
                    _fors(4));
            }
            litterae_b[longitudo_b] = '\0';

            ordo_a = _ordo_ex_literis(piscina, litterae_a, &na);
            ordo_b = _ordo_ex_literis(piscina, litterae_b, &nb);
            rectus = differentia_seriei(piscina, LECTIO(ordo_a),
                na, LECTIO(ordo_b), nb);
            conversus = differentia_seriei(piscina,
                LECTIO(ordo_b), nb, LECTIO(ordo_a), na);

            si (rectus == NIHIL || conversus == NIHIL
                || !_invarians_probare(rectus, na, nb,
                    LECTIO(ordo_a), LECTIO(ordo_b))
                || !_invarians_probare(conversus, nb, na,
                    LECTIO(ordo_b), LECTIO(ordo_a)))
            {
                perge;
            }
            /* symmetria: deletae rectae = additae conversae */
            s_rectus = differentia_summa(rectus);
            s_conversus = differentia_summa(conversus);
            si (s_rectus.deletae != s_conversus.additae
                || s_rectus.additae != s_conversus.deletae)
            {
                perge;
            }
            praeteriti = praeteriti + 1;
        }
        CREDO_AEQUALIS_I32((i32)praeteriti, (i32)200);
    }

    /* ========================================================
     * PROBARE: lineae - fissio et identitas finis
     * ======================================================== */

    {
        DifferentiaLinearum* d;

        imprimere("\n--- Probans fissionem linearum ---\n");

        /* vacuum = lineae nullae */
        d = differentia_linearum(piscina,
            chorda_ex_literis("", piscina),
            chorda_ex_literis("", piscina));
        CREDO_NON_NIHIL(d);
        CREDO_AEQUALIS_I32((i32)xar_numerus(d->lineae_a), (i32)0);
        CREDO_FALSUM(d->a_sine_fine);

        /* "x" = linea una sine fine; "x\n" = linea una cum fine */
        d = differentia_linearum(piscina,
            chorda_ex_literis("x", piscina),
            chorda_ex_literis("x\n", piscina));
        CREDO_NON_NIHIL(d);
        CREDO_AEQUALIS_I32((i32)xar_numerus(d->lineae_a), (i32)1);
        CREDO_AEQUALIS_I32((i32)xar_numerus(d->lineae_b), (i32)1);
        CREDO_VERUM(d->a_sine_fine);
        CREDO_FALSUM(d->b_sine_fine);
        /* IDENTITAS HONESTA: "x" et "x\n" lineae DIVERSAE sunt
         * (byte ultimus) - IDEM hic mendacium esset */
        CREDO_AEQUALIS_I32((i32)xar_numerus(d->tractus), (i32)2);

        /* "x\n\n" = lineae duae, secunda vacua */
        d = differentia_linearum(piscina,
            chorda_ex_literis("x\n\n", piscina),
            chorda_ex_literis("x\n\n", piscina));
        CREDO_NON_NIHIL(d);
        CREDO_AEQUALIS_I32((i32)xar_numerus(d->lineae_a), (i32)2);
        {
            chorda secunda = *(chorda*)xar_obtinere(d->lineae_a,
                1);

            CREDO_AEQUALIS_I32(secunda.mensura, (i32)0);
        }

        /* differentia simplex: a/b/c -> a/x/c */
        d = differentia_linearum(piscina,
            chorda_ex_literis("a\nb\nc\n", piscina),
            chorda_ex_literis("a\nx\nc\n", piscina));
        CREDO_NON_NIHIL(d);
        CREDO_AEQUALIS_I32((i32)xar_numerus(d->tractus), (i32)4);
        {
            DifferentiaSumma s = differentia_summa(d->tractus);

            CREDO_AEQUALIS_I32(s.additae, (i32)1);
            CREDO_AEQUALIS_I32(s.deletae, (i32)1);
        }
    }

    /* ========================================================
     * PROBARE: emissio unificata contra oraculum GNU
     * (numeri @@ et lineae 'diff -U' manu verificata 2026-08-17;
     * tituli et nota sine-fine nostra Latina)
     * ======================================================== */

    {
        DifferentiaLinearum* d;
        chorda               emissum;

        imprimere("\n--- Probans emissionem unificatam ---\n");

        /* nulla differentia = chorda vacua */
        d = differentia_linearum(piscina,
            chorda_ex_literis("par\n", piscina),
            chorda_ex_literis("par\n", piscina));
        emissum = differentia_unificata(piscina, d, "a", "b",
            (i32)3);
        CREDO_AEQUALIS_I32(emissum.mensura, (i32)0);

        /* massa una, contextus clipsus (linea VII exclusa) */
        d = differentia_linearum(piscina,
            chorda_ex_literis(
                "unus\nduo\ntres\nquattuor\nquinque\nsex\nseptem\n",
                piscina),
            chorda_ex_literis(
                "unus\nduo\nTRES\nquattuor\nquinque\nsex\nseptem\n",
                piscina));
        emissum = differentia_unificata(piscina, d, "a/f", "b/f",
            (i32)3);
        CREDO_CHORDA_AEQUALIS_LITERIS(emissum,
            "--- a/f\n"
            "+++ b/f\n"
            "@@ -1,6 +1,6 @@\n"
            " unus\n"
            " duo\n"
            "-tres\n"
            "+TRES\n"
            " quattuor\n"
            " quinque\n"
            " sex\n");

        /* massae duae (hiatus > 2*contextus, contextus I) */
        d = differentia_linearum(piscina,
            chorda_ex_literis(
                "a\nb\nc\nd\ne\nf\ng\nh\ni\nj\n", piscina),
            chorda_ex_literis(
                "a\nB\nc\nd\ne\nf\ng\nh\nI\nj\n", piscina));
        emissum = differentia_unificata(piscina, d, "a/f", "b/f",
            (i32)1);
        CREDO_CHORDA_AEQUALIS_LITERIS(emissum,
            "--- a/f\n"
            "+++ b/f\n"
            "@@ -1,3 +1,3 @@\n"
            " a\n"
            "-b\n"
            "+B\n"
            " c\n"
            "@@ -8,3 +8,3 @@\n"
            " h\n"
            "-i\n"
            "+I\n"
            " j\n");

        /* insertio in capite: latus a numero I sine ',1' (mos
         * GNU), initium 1-basatum quamquam nihil deletur */
        d = differentia_linearum(piscina,
            chorda_ex_literis("vetus\n", piscina),
            chorda_ex_literis("novum\nnovissimum\nvetus\n",
                piscina));
        emissum = differentia_unificata(piscina, d, "a/f", "b/f",
            (i32)3);
        CREDO_CHORDA_AEQUALIS_LITERIS(emissum,
            "--- a/f\n"
            "+++ b/f\n"
            "@@ -1 +1,3 @@\n"
            "+novum\n"
            "+novissimum\n"
            " vetus\n");

        /* finis sine linea nova: nota post lineam '+' ultimam */
        d = differentia_linearum(piscina,
            chorda_ex_literis("unus\nduo\n", piscina),
            chorda_ex_literis("unus\nduo mutatus", piscina));
        emissum = differentia_unificata(piscina, d, "a/f", "b/f",
            (i32)3);
        CREDO_CHORDA_AEQUALIS_LITERIS(emissum,
            "--- a/f\n"
            "+++ b/f\n"
            "@@ -1,2 +1,2 @@\n"
            " unus\n"
            "-duo\n"
            "+duo mutatus\n"
            "\\ sine linea nova in fine\n");
    }

    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
