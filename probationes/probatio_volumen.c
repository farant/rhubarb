/* probatio_volumen.c - Probationes Voluminis (documentum = plagula
 * sqlite una; acta veritas, plagulae/massae proiectio) */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "volumen.h"
#include "credo.h"

#include <stdio.h>

#define VIA_PROBATIONIS "build/probatio_volumen.db"

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_volumen", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* tabula rasa (percursus prior) */
    si (filum_existit(VIA_PROBATIONIS))
    {
        filum_delere(VIA_PROBATIONIS);
    }

    /* ========================================================
     * PROBARE: volumen_creare - ortus + recusatio duplicati
     * ======================================================== */

    {
        Volumen* vol;
        Volumen* iterum;

        imprimere("\n--- Probans volumen_creare ---\n");

        vol = volumen_creare(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);
        CREDO_VERUM(filum_existit(VIA_PROBATIONIS));

        /* actum ortus iam scriptum */
        CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), (s64)1);
        CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), (s64)0);

        /* creare super exsistentem = recusatio */
        iterum = volumen_creare(piscina, VIA_PROBATIONIS);
        CREDO_NIHIL(iterum);

        volumen_claudere(vol);
    }

    /* ========================================================
     * PROBARE: actum_appendere - seq crescens
     * ======================================================== */

    {
        Volumen* vol;
        chorda   datum;
        s64      seq;

        imprimere("\n--- Probans volumen_actum_appendere ---\n");

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        datum = chorda_ex_literis("{\"probatio\":1}", piscina);
        seq = volumen_actum_appendere(vol, "probatio-actus", datum);
        CREDO_AEQUALIS_S64(seq, (s64)2);
        seq = volumen_actum_appendere(vol, "probatio-actus", datum);
        CREDO_AEQUALIS_S64(seq, (s64)3);
        CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), (s64)3);

        volumen_claudere(vol);
    }

    /* ========================================================
     * PROBARE: plagulam condere/promere - circuitus integer
     * ======================================================== */

    {
        Volumen* vol;
        chorda   via;
        chorda   contentum;
        chorda   relatum;
        b32      inventum;

        imprimere("\n--- Probans plagulam condere/promere ---\n");

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        via = chorda_ex_literis("lib/exemplum.c", piscina);
        contentum = chorda_ex_literis(
            "/* exemplum */\ninteger quadraginta_duo = 42;\n",
            piscina);
        CREDO_VERUM(volumen_plagulam_condere(vol, via, contentum,
            "vendicata:lib/exemplum.c"));
        CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), (s64)1);

        relatum = volumen_plagulam_promere(vol, via, piscina,
            &inventum);
        CREDO_VERUM(inventum);
        CREDO_CHORDA_AEQUALIS(relatum, contentum);

        /* via ignota */
        relatum = volumen_plagulam_promere(vol,
            chorda_ex_literis("non/exsistit.c", piscina), piscina,
            &inventum);
        CREDO_FALSUM(inventum);

        volumen_claudere(vol);
    }

    /* ========================================================
     * PROBARE: massae dedup + upsert per viam
     * ======================================================== */

    {
        Volumen* vol;
        chorda   contentum;
        chorda   alterum;
        chorda   relatum;
        b32      inventum;
        s64      massae_ante;

        imprimere("\n--- Probans dedup sigillo + upsert ---\n");

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        contentum = chorda_ex_literis("contentum idem", piscina);
        CREDO_VERUM(volumen_plagulam_condere(vol,
            chorda_ex_literis("a.txt", piscina), contentum,
            "genita"));
        massae_ante = volumen_summa_massarum(vol);
        CREDO_VERUM(volumen_plagulam_condere(vol,
            chorda_ex_literis("b.txt", piscina), contentum,
            "genita"));
        /* contentum idem -> massa nova NULLA */
        CREDO_AEQUALIS_S64(volumen_summa_massarum(vol), massae_ante);
        CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), (s64)3);

        /* upsert: via eadem, contentum novum */
        alterum = chorda_ex_literis("contentum alterum", piscina);
        CREDO_VERUM(volumen_plagulam_condere(vol,
            chorda_ex_literis("a.txt", piscina), alterum,
            "genita"));
        CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), (s64)3);
        relatum = volumen_plagulam_promere(vol,
            chorda_ex_literis("a.txt", piscina), piscina,
            &inventum);
        CREDO_VERUM(inventum);
        CREDO_CHORDA_AEQUALIS(relatum, alterum);

        volumen_claudere(vol);
    }

    /* ========================================================
     * PROBARE: plagula vacua legitima
     * ======================================================== */

    {
        Volumen* vol;
        chorda   vacua;
        chorda   relatum;
        b32      inventum;

        imprimere("\n--- Probans plagulam vacuam ---\n");

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        vacua.datum = NIHIL;
        vacua.mensura = ZEPHYRUM;
        CREDO_VERUM(volumen_plagulam_condere(vol,
            chorda_ex_literis("vacua.txt", piscina), vacua,
            "genita"));
        relatum = volumen_plagulam_promere(vol,
            chorda_ex_literis("vacua.txt", piscina), piscina,
            &inventum);
        CREDO_VERUM(inventum);
        CREDO_AEQUALIS_I32(relatum.mensura, (i32)ZEPHYRUM);

        volumen_claudere(vol);
    }

    /* ========================================================
     * PROBARE: enumerare + permanentia trans aperturas
     * ======================================================== */

    {
        Volumen*        vol;
        Xar*            manifestum;
        VolumenPlagula* p;

        imprimere("\n--- Probans enumerare + permanentiam ---\n");

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        manifestum = volumen_plagulas_enumerare(vol, piscina);
        CREDO_NON_NIHIL(manifestum);
        CREDO_AEQUALIS_I32((i32)xar_numerus(manifestum), (i32)4);

        /* ordo per viam: a.txt primum */
        p = (VolumenPlagula*)xar_obtinere(manifestum, 0);
        CREDO_CHORDA_AEQUALIS_LITERIS(p->via, "a.txt");
        CREDO_VERUM(p->sigillum_hex.mensura == 64);
        CREDO_CHORDA_AEQUALIS_LITERIS(p->origo, "genita");

        volumen_claudere(vol);
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
