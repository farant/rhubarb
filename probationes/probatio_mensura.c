/*
 * probatio_mensura.c - Probationes mensurarum nidificatarum
 *
 * Nucleus formae (ad_lineam / ex_linea) PURUS est: nulla plagula,
 * nullum volumen. Diarium et conditio /tmp et volumen temporarium
 * adhibent - volumen VERUM numquam tangitur.
 */

#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "mensura.h"
#include "volumen.h"
#include <stdio.h>
#include <string.h>

#define DIARIUM "/tmp/probatio_mensura_diarium.tsv"

interior Mensura
_mensura_ficta (Piscina* piscina, constans character* titulus, duplex valor)
{
    Mensura m;

    m.sessio   = chorda_ex_literis("019a3f2b1c00", piscina);
    m.id       = chorda_ex_literis("019a3f2b1c00-1f4-3", piscina);
    m.parens   = chorda_ex_literis("019a3f2b1c00-1f4-1", piscina);
    m.titulus  = chorda_ex_literis(titulus, piscina);
    m.valor    = valor;
    m.unitas   = chorda_ex_literis("secunda", piscina);
    m.momentum = (s64)1755100000000;

    redde m;
}

interior i32
_lineas_numerare (constans character* via)
{
    FILE* f;
    i32   numerus;
    integer c;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde (i32)0;
    }

    numerus = ZEPHYRUM;
    dum ((c = fgetc(f)) != EOF)
    {
        si (c == '\n')
        {
            numerus++;
        }
    }
    fclose(f);

    redde numerus;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_mensura", M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    remove(DIARIUM);

    /* ========================================================
     * I. Circuitus formae: nihil perit
     * ======================================================== */

    imprimere("\n--- I. Circuitus lineae ---\n");
    {
        Mensura fons, reducta;
        chorda  linea;

        fons  = _mensura_ficta(piscina, "compilatio.lib/chorda.c", 1.234567);
        linea = mensura_ad_lineam(&fons, piscina);

        CREDO_MAIOR_I32(linea.mensura, ZEPHYRUM);
        CREDO_VERUM(mensura_ex_linea(linea, &reducta, piscina));

        CREDO_VERUM(chorda_aequalis(reducta.sessio, fons.sessio));
        CREDO_VERUM(chorda_aequalis(reducta.id, fons.id));
        CREDO_VERUM(chorda_aequalis(reducta.parens, fons.parens));
        CREDO_VERUM(chorda_aequalis(reducta.titulus, fons.titulus));
        CREDO_VERUM(chorda_aequalis(reducta.unitas, fons.unitas));
        CREDO_F64_PROXIMUS(reducta.valor, fons.valor, 0.000001);
        CREDO_AEQUALIS_S64(reducta.momentum, fons.momentum);
    }

    /* ========================================================
     * II. Parens vacuus (radix) circuitum superest
     * ======================================================== */

    imprimere("\n--- II. Radix (parens vacuus) ---\n");
    {
        Mensura fons, reducta;
        chorda  linea;

        fons        = _mensura_ficta(piscina, "aedificatio", 42.0);
        fons.parens = chorda_ex_literis("", piscina);

        linea = mensura_ad_lineam(&fons, piscina);
        CREDO_VERUM(mensura_ex_linea(linea, &reducta, piscina));
        CREDO_AEQUALIS_I32(reducta.parens.mensura, ZEPHYRUM);
        CREDO_VERUM(chorda_aequalis(reducta.titulus, fons.titulus));
    }

    /* ========================================================
     * III. Campus separatorem continens RECUSATUR
     *
     * Forma quae se ipsam frangere sinit datum tacite corrumpit:
     * lector campos numeraret et alium sensum inveniret.
     * ======================================================== */

    imprimere("\n--- III. Separator in campo ---\n");
    {
        Mensura fons;
        chorda  linea;

        fons         = _mensura_ficta(piscina, "bonus", 1.0);
        fons.titulus = chorda_ex_literis("malus\ttitulus", piscina);
        linea        = mensura_ad_lineam(&fons, piscina);
        CREDO_AEQUALIS_I32(linea.mensura, ZEPHYRUM);

        fons         = _mensura_ficta(piscina, "bonus", 1.0);
        fons.unitas  = chorda_ex_literis("secunda\nnova", piscina);
        linea        = mensura_ad_lineam(&fons, piscina);
        CREDO_AEQUALIS_I32(linea.mensura, ZEPHYRUM);
    }

    /* ========================================================
     * IV. Lineae malformatae RECUSANTUR
     * ======================================================== */

    imprimere("\n--- IV. Lineae malformatae ---\n");
    {
        Mensura reducta;

        CREDO_FALSUM(mensura_ex_linea(
            chorda_ex_literis("a\tb\tc", piscina), &reducta, piscina));

        CREDO_FALSUM(mensura_ex_linea(
            chorda_ex_literis("a\tb\tc\td\tNONNUMERUS\tf\t123", piscina),
            &reducta, piscina));

        CREDO_FALSUM(mensura_ex_linea(
            chorda_ex_literis("a\tb\tc\td\t1.5\tf\tg\th", piscina),
            &reducta, piscina));
    }

    /* ========================================================
     * V. Claves unicae
     *
     * Idem periculum quod machina.h habet: officium constantem
     * reddens omnes probationes obvias praeteriret.
     * ======================================================== */

    imprimere("\n--- V. Claves ---\n");
    {
        chorda id_a, id_b, sessio;

        id_a   = mensura_id_novum(piscina);
        id_b   = mensura_id_novum(piscina);
        sessio = mensura_sessionem_novam(piscina);

        CREDO_MAIOR_I32(id_a.mensura, ZEPHYRUM);
        CREDO_MAIOR_I32(sessio.mensura, ZEPHYRUM);
        CREDO_FALSUM(chorda_aequalis(id_a, id_b));
        CREDO_VERUM(mensura_momentum_nunc() > (s64)1700000000000);
    }

    /* ========================================================
     * VI. Diarium APPENDIT (non superscribit)
     * ======================================================== */

    imprimere("\n--- VI. Diarium ---\n");
    {
        Mensura prima, altera;

        prima  = _mensura_ficta(piscina, "unum", 1.0);
        altera = _mensura_ficta(piscina, "duo", 2.0);

        CREDO_VERUM(mensura_annotare(DIARIUM, &prima, piscina));
        CREDO_AEQUALIS_I32(_lineas_numerare(DIARIUM), (i32)I);

        CREDO_VERUM(mensura_annotare(DIARIUM, &altera, piscina));
        CREDO_AEQUALIS_I32(_lineas_numerare(DIARIUM), (i32)II);

        /* Via impossibilis: FALSUM, non silentium */
        CREDO_FALSUM(mensura_annotare("/nusquam/omnino/d.tsv",
                                      &prima, piscina));
    }

    /* ========================================================
     * VII. Conditio in volumen + truncatio
     * ======================================================== */

    imprimere("\n--- VII. Conditio ---\n");
    {
        Volumen*       vol;
        MensuraFructus fructus;

        vol = volumen_temporarium(piscina, "probatio_mensura");
        CREDO_NON_NIHIL(vol);

        si (vol != NIHIL)
        {
            fructus = mensura_condere(DIARIUM, vol, piscina);

            CREDO_VERUM(fructus.successus);
            CREDO_AEQUALIS_I32(fructus.conditae, (i32)II);
            CREDO_VERUM(volumen_summa_actorum(vol) > (s64)0);

            /* Diarium TRUNCATUM est: aliter conditio proxima
             * easdem mensuras iterum conderet */
            CREDO_AEQUALIS_I32(_lineas_numerare(DIARIUM), ZEPHYRUM);

            volumen_claudere(vol);
        }
    }

    /* ========================================================
     * VIII. Lineae malformatae NUMERANTUR, non silentur
     * ======================================================== */

    imprimere("\n--- VIII. Malformatae numerantur ---\n");
    {
        Volumen*       vol;
        MensuraFructus fructus;
        Mensura        bona;
        FILE*          f;

        remove(DIARIUM);

        bona = _mensura_ficta(piscina, "bona", 3.0);
        CREDO_VERUM(mensura_annotare(DIARIUM, &bona, piscina));

        f = fopen(DIARIUM, "ab");
        si (f != NIHIL)
        {
            fputs("hoc\tnon\test\tmensura\n", f);
            fclose(f);
        }

        vol = volumen_temporarium(piscina, "probatio_mensura_mala");
        si (vol != NIHIL)
        {
            fructus = mensura_condere(DIARIUM, vol, piscina);

            /* Bona condita, mala numerata et NOMINATA */
            CREDO_VERUM(fructus.successus);
            CREDO_AEQUALIS_I32(fructus.conditae, (i32)I);
            CREDO_CHORDA_NON_VACUA(fructus.causa);

            volumen_claudere(vol);
        }

        remove(DIARIUM);
    }

    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

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
