/* probatio_lapifex_glr.c - Probationes Motoris GLR
 *
 * Probat motorem GLR cum grammaticis ambiguis
 * (shift/reduce et reduce/reduce conflictus).
 */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "lapifex_glr.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ================================================
 * Grammatica Ambigua: E -> E + E | E * E | id
 *
 * Haec grammatica habet conflictus shift/reduce
 * (e.g. post "id + id", cum * sequente)
 * et est classica probatio pro GLR.
 * ================================================ */

hic_manens constans character* GRAMMATICA_AMBIGUA =
    "<grammatica>"
    "  <terminalia>"
    "    <terminalis titulus=\"PLUS\"/>"
    "    <terminalis titulus=\"MULT\"/>"
    "    <terminalis titulus=\"ID\"/>"
    "    <terminalis titulus=\"EOF\"/>"
    "  </terminalia>"
    "  <regulae>"
    "    <regula titulus=\"E\">"
    "      <productio>E PLUS E</productio>"
    "      <productio>E MULT E</productio>"
    "      <productio>ID</productio>"
    "    </regula>"
    "  </regulae>"
    "  <initium>E</initium>"
    "</grammatica>";

/* Callback: computare valorem arithmeticum */
hic_manens s64
reductio_arithmetica(
    s32           productio_index,
    constans s64* valori,
    s32           numerus_dextrum,
    vacuum*       contextus)
{
    (vacuum)contextus;
    (vacuum)numerus_dextrum;

    /* Productiones (post augmentatam):
     * P0: E -> E PLUS E
     * P1: E -> E MULT E
     * P2: E -> ID
     * P3: __initium__ -> E EOF
     */
    commutatio (productio_index)
    {
        casus ZEPHYRUM:  /* E -> E PLUS E */
            redde valori[ZEPHYRUM] + valori[II];
        casus I:  /* E -> E MULT E */
            redde valori[ZEPHYRUM] * valori[II];
        casus II: /* E -> ID */
            redde valori[ZEPHYRUM];
        casus III: /* __initium__ -> E EOF */
            redde valori[ZEPHYRUM];
        ordinarius:
            redde 0;
    }
}

/* ================================================
 * Grammatica Simplex (non ambigua): S -> a b
 * ================================================ */

hic_manens constans character* GRAMMATICA_SIMPLEX =
    "<grammatica>"
    "  <terminalia>"
    "    <terminalis titulus=\"A\"/>"
    "    <terminalis titulus=\"B\"/>"
    "    <terminalis titulus=\"EOF\"/>"
    "  </terminalia>"
    "  <regulae>"
    "    <regula titulus=\"S\">"
    "      <productio>A B</productio>"
    "    </regula>"
    "  </regulae>"
    "  <initium>S</initium>"
    "</grammatica>";

hic_manens s64
reductio_simplex(
    s32           productio_index,
    constans s64* valori,
    s32           numerus_dextrum,
    vacuum*       contextus)
{
    (vacuum)contextus;
    (vacuum)numerus_dextrum;

    commutatio (productio_index)
    {
        casus ZEPHYRUM: /* S -> A B */
            redde valori[ZEPHYRUM] + valori[I];
        casus I: /* __initium__ -> S EOF */
            redde valori[ZEPHYRUM];
        ordinarius:
            redde 0;
    }
}

/* ================================================
 * Auxiliaris: Construere tabulam ex STML
 * ================================================ */

nomen structura {
    LapifexGrammatica*  grammatica;
    LapifexCollectio*   collectio;
    LapifexTabula*      tabula;
} GrammaticaParata;

hic_manens GrammaticaParata
grammaticam_parare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  stml)
{
    GrammaticaParata res;

    res.grammatica = NIHIL;
    res.collectio = NIHIL;
    res.tabula = NIHIL;

    res.grammatica = lapifex_grammaticam_legere(piscina, intern, stml);
    si (!res.grammatica) redde res;

    lapifex_first_computare(res.grammatica);
    lapifex_follow_computare(res.grammatica);

    res.collectio = lapifex_collectio_construere(res.grammatica);
    si (!res.collectio) redde res;

    res.tabula = lapifex_tabulam_construere(res.collectio);

    redde res;
}

/* Auxiliaris: invenire index terminalis per titulum */
hic_manens s32
terminalem_invenire(
    LapifexGrammatica*  grammatica,
    constans character*  titulis)
{
    i32 i;
    i32 num = (i32)xar_numerus(grammatica->symbola);

    per (i = ZEPHYRUM; i < num; i++)
    {
        LapifexSymbolum* sym =
            (LapifexSymbolum*)xar_obtinere(grammatica->symbola, i);
        si (sym && sym->est_terminale)
        {
            si ((i32)sym->titulus->mensura == (i32)strlen(titulis) &&
                memcmp(sym->titulus->datum, titulis,
                    (memoriae_index)sym->titulus->mensura) == ZEPHYRUM)
            {
                redde sym->index;
            }
        }
    }
    redde -I;
}

/* ================================================
 * Probationes
 * ================================================ */

s32 principale(vacuum)
{
    b32                  praeteritus;
    Piscina*             piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_lapifex_glr", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina non creata\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: Grammatica simplex (non ambigua)
     * ======================================================== */
    {
        GrammaticaParata gp;
        LapifexGLR*      glr;
        LapifexGLRFructus fructus;
        LapifexSignum    signa[III];
        s32              idx_a;
        s32              idx_b;
        s32              idx_eof;

        imprimere("\n--- Probans grammaticam simplicem ---\n");

        gp = grammaticam_parare(piscina, intern, GRAMMATICA_SIMPLEX);
        CREDO_NON_NIHIL(gp.tabula);

        /* Nulla conflictus in grammatica simplice */
        CREDO_AEQUALIS_S32(gp.tabula->numerus_conflictuum, ZEPHYRUM);

        glr = lapifex_glr_creare(piscina, gp.tabula);
        CREDO_NON_NIHIL(glr);

        /* Invenire indices terminalium */
        idx_a = terminalem_invenire(gp.grammatica, "A");
        idx_b = terminalem_invenire(gp.grammatica, "B");
        idx_eof = terminalem_invenire(gp.grammatica, "EOF");

        CREDO_VERUM(idx_a >= ZEPHYRUM);
        CREDO_VERUM(idx_b >= ZEPHYRUM);
        CREDO_VERUM(idx_eof >= ZEPHYRUM);

        /* Signa: A(10) B(20) EOF */
        signa[ZEPHYRUM].terminalis = idx_a;
        signa[ZEPHYRUM].valor = X;
        signa[I].terminalis = idx_b;
        signa[I].valor = XX;
        signa[II].terminalis = idx_eof;
        signa[II].valor = ZEPHYRUM;

        fructus = lapifex_glr_parsare(
            glr, signa, III, reductio_simplex, NIHIL);

        CREDO_VERUM(fructus.successus);
        CREDO_FALSUM(fructus.ambiguus);
        CREDO_AEQUALIS_S32(fructus.numerus_fructuum, I);
        CREDO_NON_NIHIL(fructus.valori);
        CREDO_AEQUALIS_S64(fructus.valori[ZEPHYRUM], XXX);
    }

    /* ========================================================
     * PROBARE: GLR cum grammatica ambigua - "id + id * id"
     * Debet producere plures parsationes
     * ======================================================== */
    {
        GrammaticaParata  gp;
        LapifexGLR*       glr;
        LapifexGLRFructus fructus;
        LapifexSignum     signa[VI];
        s32               idx_id;
        s32               idx_plus;
        s32               idx_mult;
        s32               idx_eof;

        imprimere("\n--- Probans grammaticam ambiguam (id + id * id) ---\n");

        gp = grammaticam_parare(piscina, intern, GRAMMATICA_AMBIGUA);
        CREDO_NON_NIHIL(gp.tabula);

        /* Debet habere conflictus */
        CREDO_VERUM(gp.tabula->numerus_conflictuum > ZEPHYRUM);

        glr = lapifex_glr_creare(piscina, gp.tabula);
        CREDO_NON_NIHIL(glr);

        idx_id = terminalem_invenire(gp.grammatica, "ID");
        idx_plus = terminalem_invenire(gp.grammatica, "PLUS");
        idx_mult = terminalem_invenire(gp.grammatica, "MULT");
        idx_eof = terminalem_invenire(gp.grammatica, "EOF");

        /* "2 + 3 * 4" */
        signa[ZEPHYRUM].terminalis = idx_id;
        signa[ZEPHYRUM].valor = II;
        signa[I].terminalis = idx_plus;
        signa[I].valor = ZEPHYRUM;
        signa[II].terminalis = idx_id;
        signa[II].valor = III;
        signa[III].terminalis = idx_mult;
        signa[III].valor = ZEPHYRUM;
        signa[IV].terminalis = idx_id;
        signa[IV].valor = IV;
        signa[V].terminalis = idx_eof;
        signa[V].valor = ZEPHYRUM;

        fructus = lapifex_glr_parsare(
            glr, signa, VI, reductio_arithmetica, NIHIL);

        CREDO_VERUM(fructus.successus);
        CREDO_VERUM(fructus.ambiguus);
        CREDO_VERUM(fructus.numerus_fructuum >= II);

        imprimere("  Fructus: %d parsationes\n",
            (int)fructus.numerus_fructuum);

        /* Verificare quod ambae interpretationes praesentes:
         * (2 + 3) * 4 = 20
         * 2 + (3 * 4) = 14
         */
        {
            b32 invenit_xx = FALSUM;
            b32 invenit_xiv = FALSUM;
            i32 i;

            per (i = ZEPHYRUM; i < (i32)fructus.numerus_fructuum; i++)
            {
                imprimere("  Valor[%d] = %lld\n",
                    (int)i, (longus longus)fructus.valori[i]);

                si (fructus.valori[i] == XX) invenit_xx = VERUM;
                si (fructus.valori[i] == XIV) invenit_xiv = VERUM;
            }

            CREDO_VERUM(invenit_xx);
            CREDO_VERUM(invenit_xiv);
        }
    }

    /* ========================================================
     * PROBARE: GLR cum grammatica ambigua - "id" singulare
     * Debet producere unam parsationem (non ambigua)
     * ======================================================== */
    {
        GrammaticaParata  gp;
        LapifexGLR*       glr;
        LapifexGLRFructus fructus;
        LapifexSignum     signa[II];
        s32               idx_id;
        s32               idx_eof;

        imprimere("\n--- Probans singulare id ---\n");

        gp = grammaticam_parare(piscina, intern, GRAMMATICA_AMBIGUA);
        CREDO_NON_NIHIL(gp.tabula);

        glr = lapifex_glr_creare(piscina, gp.tabula);
        CREDO_NON_NIHIL(glr);

        idx_id = terminalem_invenire(gp.grammatica, "ID");
        idx_eof = terminalem_invenire(gp.grammatica, "EOF");

        /* "42" */
        signa[ZEPHYRUM].terminalis = idx_id;
        signa[ZEPHYRUM].valor = XLII;
        signa[I].terminalis = idx_eof;
        signa[I].valor = ZEPHYRUM;

        fructus = lapifex_glr_parsare(
            glr, signa, II, reductio_arithmetica, NIHIL);

        CREDO_VERUM(fructus.successus);
        CREDO_FALSUM(fructus.ambiguus);
        CREDO_AEQUALIS_S32(fructus.numerus_fructuum, I);
        CREDO_AEQUALIS_S64(fructus.valori[ZEPHYRUM], XLII);
    }

    /* ========================================================
     * PROBARE: Error - signum invalidum
     * ======================================================== */
    {
        GrammaticaParata  gp;
        LapifexGLR*       glr;
        LapifexGLRFructus fructus;
        LapifexSignum     signa[II];
        s32               idx_plus;
        s32               idx_eof;

        imprimere("\n--- Probans errorem (signum invalidum) ---\n");

        gp = grammaticam_parare(piscina, intern, GRAMMATICA_AMBIGUA);
        CREDO_NON_NIHIL(gp.tabula);

        glr = lapifex_glr_creare(piscina, gp.tabula);
        CREDO_NON_NIHIL(glr);

        idx_plus = terminalem_invenire(gp.grammatica, "PLUS");
        idx_eof = terminalem_invenire(gp.grammatica, "EOF");

        /* "+" - non potest incipere cum operatore */
        signa[ZEPHYRUM].terminalis = idx_plus;
        signa[ZEPHYRUM].valor = ZEPHYRUM;
        signa[I].terminalis = idx_eof;
        signa[I].valor = ZEPHYRUM;

        fructus = lapifex_glr_parsare(
            glr, signa, II, reductio_arithmetica, NIHIL);

        CREDO_FALSUM(fructus.successus);
        CREDO_VERUM(fructus.error.est_error);
    }

    /* ========================================================
     * PROBARE: "id + id" (ambigua grammatica, sed non ambiguus initus)
     * ======================================================== */
    {
        GrammaticaParata  gp;
        LapifexGLR*       glr;
        LapifexGLRFructus fructus;
        LapifexSignum     signa[IV];
        s32               idx_id;
        s32               idx_plus;
        s32               idx_eof;

        imprimere("\n--- Probans id + id ---\n");

        gp = grammaticam_parare(piscina, intern, GRAMMATICA_AMBIGUA);
        CREDO_NON_NIHIL(gp.tabula);

        glr = lapifex_glr_creare(piscina, gp.tabula);
        CREDO_NON_NIHIL(glr);

        idx_id = terminalem_invenire(gp.grammatica, "ID");
        idx_plus = terminalem_invenire(gp.grammatica, "PLUS");
        idx_eof = terminalem_invenire(gp.grammatica, "EOF");

        /* "5 + 3" */
        signa[ZEPHYRUM].terminalis = idx_id;
        signa[ZEPHYRUM].valor = V;
        signa[I].terminalis = idx_plus;
        signa[I].valor = ZEPHYRUM;
        signa[II].terminalis = idx_id;
        signa[II].valor = III;
        signa[III].terminalis = idx_eof;
        signa[III].valor = ZEPHYRUM;

        fructus = lapifex_glr_parsare(
            glr, signa, IV, reductio_arithmetica, NIHIL);

        CREDO_VERUM(fructus.successus);
        /* id + id habet unam solam parsationem */
        CREDO_AEQUALIS_S32(fructus.numerus_fructuum, I);
        CREDO_AEQUALIS_S64(fructus.valori[ZEPHYRUM], VIII);
    }

    /* ========================================================
     * PROBARE: Motor NIHIL tutela
     * ======================================================== */
    {
        LapifexGLRFructus fructus;

        imprimere("\n--- Probans tutelas NIHIL ---\n");

        fructus = lapifex_glr_parsare(NIHIL, NIHIL, ZEPHYRUM, NIHIL, NIHIL);
        CREDO_FALSUM(fructus.successus);
        CREDO_VERUM(fructus.error.est_error);
    }

    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
