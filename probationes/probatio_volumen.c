/* probatio_volumen.c - Probationes Voluminis (documentum = plagula
 * sqlite una; acta veritas, plagulae/massae proiectio) */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "volumen.h"
#include "credo.h"
#include "filum.h"
#include <string.h>

#include <stdio.h>

#define VIA_PROBATIONIS "build/probatio_volumen.db"

interior b32
_continet (constans character* fenum, constans character* acus)
{
    redde (strstr(fenum, acus) != NIHIL) ? VERUM : FALSUM;
}

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
     * PROBARE: aperire_aut_creare (ambulatio red-team 2026-08-04)
     * ======================================================== */

    {
        Volumen* vol;

        imprimere("\n--- Probans aperire_aut_creare ---\n");

        /* exsistens: aperit (actum ortus iam ibi, non iteratum) */
        vol = volumen_aperire_aut_creare(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);
        CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), (s64)1);
        volumen_claudere(vol);

        /* absens: creat */
        si (filum_existit("build/probatio_volumen_aut.db"))
        {
            filum_delere("build/probatio_volumen_aut.db");
        }
        vol = volumen_aperire_aut_creare(piscina,
            "build/probatio_volumen_aut.db");
        CREDO_NON_NIHIL(vol);
        CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), (s64)1);
        volumen_claudere(vol);
        filum_delere("build/probatio_volumen_aut.db");
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
     * PROBARE: massae nudae - circuitus binarius (octeti nulli!)
     * ======================================================== */

    {
        Volumen*  vol;
        i8        octeti[6];
        chorda    contentum;
        chorda    relatum;
        character hex[SIGILLUM_HEX_MENSURA];
        character hex_iterum[SIGILLUM_HEX_MENSURA];
        b32       inventum;
        s64       massae_ante;

        imprimere("\n--- Probans massas nudas (binarias) ---\n");

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        octeti[0] = (i8)0x89;
        octeti[1] = 'P';
        octeti[2] = 0;          /* octetus nullus intra! */
        octeti[3] = 'N';
        octeti[4] = 0;
        octeti[5] = (i8)0xFF;
        contentum = chorda_ex_buffer(octeti, 6);

        CREDO_VERUM(volumen_massam_condere(vol, contentum, hex));
        CREDO_VERUM(hex[0] != '\0');

        relatum = volumen_massam_promere(vol,
            chorda_ex_literis(hex, piscina), piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_AEQUALIS_I32(relatum.mensura, (i32)6);
        CREDO_VERUM(relatum.datum[2] == 0 && relatum.datum[4] == 0);
        CREDO_VERUM((insignatus character)relatum.datum[5] == 0xFF);

        /* dedup: contentum idem -> sigillum idem, massa nova nulla */
        massae_ante = volumen_summa_massarum(vol);
        CREDO_VERUM(volumen_massam_condere(vol, contentum,
            hex_iterum));
        CREDO_AEQUALIS_S64(volumen_summa_massarum(vol), massae_ante);
        CREDO_CHORDAE_AEQUALES(hex, hex_iterum);

        /* sigillum ignotum */
        volumen_massam_promere(vol,
            chorda_ex_literis("deadbeef", piscina), piscina,
            &inventum);
        CREDO_FALSUM(inventum);

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
     * PROBARE: plicam_ad - iter temporis replicando acta
     * ======================================================== */

    {
        Volumen*        vol;
        Xar*            praesens;
        Xar*            manifestum;
        Xar*            vetus;
        VolumenPlagula* p;
        VolumenPlagula* q;
        i32             index;
        s64             ante_remotionem;

        imprimere("\n--- Probans plicam_ad (iter temporis) ---\n");

        vol = volumen_aperire(piscina, VIA_PROBATIONIS);
        CREDO_NON_NIHIL(vol);

        /* PINNA: plica praesens (ad 0) manifesto aequalis -
         * replicatio upsert-logicam manifesti probat */
        praesens = volumen_plicam_ad(vol, (s64)0, piscina);
        manifestum = volumen_plagulas_enumerare(vol, piscina);
        CREDO_NON_NIHIL(praesens);
        CREDO_NON_NIHIL(manifestum);
        CREDO_AEQUALIS_I32((i32)xar_numerus(praesens),
            (i32)xar_numerus(manifestum));
        per (index = 0; index < xar_numerus(praesens);
            index = index + 1)
        {
            p = (VolumenPlagula*)xar_obtinere(praesens, index);
            q = (VolumenPlagula*)xar_obtinere(manifestum, index);
            CREDO_CHORDA_AEQUALIS(p->via, q->via);
            CREDO_CHORDA_AEQUALIS(p->sigillum_hex, q->sigillum_hex);
        }

        /* ad seq 6 (ante upsert a.txt): a.txt contentum pristinum -
         * sigillum idem ac b.txt (ambae "contentum idem" erant);
         * vacua.txt (seq 8) nondum nata */
        vetus = volumen_plicam_ad(vol, (s64)6, piscina);
        CREDO_NON_NIHIL(vetus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(vetus), (i32)3);
        {
            VolumenPlagula* a_vetus = NIHIL;
            VolumenPlagula* b_vetus = NIHIL;
            VolumenPlagula* a_nunc = NIHIL;

            per (index = 0; index < xar_numerus(vetus);
                index = index + 1)
            {
                p = (VolumenPlagula*)xar_obtinere(vetus, index);
                si (chorda_aequalis_literis(p->via, "a.txt"))
                {
                    a_vetus = p;
                }
                si (chorda_aequalis_literis(p->via, "b.txt"))
                {
                    b_vetus = p;
                }
            }
            per (index = 0; index < xar_numerus(manifestum);
                index = index + 1)
            {
                q = (VolumenPlagula*)xar_obtinere(manifestum, index);
                si (chorda_aequalis_literis(q->via, "a.txt"))
                {
                    a_nunc = q;
                }
            }
            CREDO_VERUM(a_vetus != NIHIL && b_vetus != NIHIL
                && a_nunc != NIHIL);
            CREDO_CHORDA_AEQUALIS(a_vetus->sigillum_hex,
                b_vetus->sigillum_hex);
            CREDO_FALSUM(chorda_aequalis(a_vetus->sigillum_hex,
                a_nunc->sigillum_hex));
        }

        /* remotio replicata: plica praesens sine b.txt, plica ANTE
         * remotionem eam adhuc fert (historia integra) */
        ante_remotionem = volumen_summa_actorum(vol);
        CREDO_VERUM(volumen_plagulam_removere(vol,
            chorda_ex_literis("b.txt", piscina)));
        praesens = volumen_plicam_ad(vol, (s64)0, piscina);
        CREDO_NON_NIHIL(praesens);
        CREDO_AEQUALIS_I32((i32)xar_numerus(praesens), (i32)3);
        vetus = volumen_plicam_ad(vol, ante_remotionem, piscina);
        CREDO_NON_NIHIL(vetus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(vetus), (i32)4);

        volumen_claudere(vol);
    }

    /* ========================================================
     * PROBARE: volumen temporarium (scratch)
     * ======================================================== */

    {
        Volumen*            tmp;
        constans character* via_tmp;
        character           via_servata[CCLVI];
        s64                 seq;

        imprimere("\n--- Probans volumen_temporarium ---\n");

        tmp = volumen_temporarium(piscina, "probatio_volumen");
        CREDO_NON_NIHIL (tmp);
        si (tmp != NIHIL)
        {
            /* Plagula VERA est dum vivit - scribi et legi potest. */
            via_tmp = volumen_via(tmp);
            CREDO_VERUM (via_tmp != NIHIL);
            si (via_tmp != NIHIL)
            {
                strcpy(via_servata, via_tmp);
                CREDO_VERUM (filum_existit(via_servata));
                CREDO_VERUM (_continet(via_servata, "probatio_volumen"));

                seq = volumen_actum_appendere(tmp, "probatum",
                    chorda_ex_literis("{}", piscina));
                CREDO_VERUM (seq > 0);

                /* Claudere DELET - id est totum consilium. */
                volumen_claudere(tmp);
                CREDO_FALSUM (filum_existit(via_servata));
            }
        }
    }

    /* ========================================================
     * PROBARE: volumen ORDINARIUM claudendo NON deletur
     *
     * Custos contra vitium quod campum addendo paene induxi:
     * structura campo-post-campum impletur SINE memset, ergo
     * 'temporarium' purgamentum ferret et volumen VERUM in
     * claudendo periret.
     * ======================================================== */

    {
        Volumen*  ordinarium;
        character via_ord[CCLVI];
        s32       i;

        imprimere("\n--- Probans volumen ordinarium SUPERSTES ---\n");

        /* Decies, quia purgamentum stabile non est: unus cursus
         * casu nullum ferre potest. */
        per (i = 0; i < X; i = i + I)
        {
            sprintf(via_ord, "/tmp/probatio_volumen_ordinarium.volumen");
            (vacuum)filum_delere(via_ord);

            ordinarium = volumen_creare(piscina, via_ord);
            si (ordinarium == NIHIL)
            {
                CREDO_NON_NIHIL (ordinarium);
                frange;
            }
            volumen_claudere(ordinarium);
            CREDO_VERUM (filum_existit(via_ord));
            (vacuum)filum_delere(via_ord);
        }
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
