/* probatio_silex.c - Probationes Silicis (clausura + novum)
 *
 * Currendum e radice repositorii (fabrica = "." - suite semper
 * inde currit). Area probationis: build/probatio_silex_area/
 * (deleta ante percursum). Scaffoldatum hic NON compilatur -
 * id cibus caninus est (silicetum), non suite. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "via.h"
#include "xar.h"
#include "silex.h"
#include "volumen.h"
#include "processus.h"
#include "credo.h"

#include <stdio.h>

#define AREA "build/probatio_silex_area"

interior b32
_manifestum_continet (Xar* res_omnes, constans character* via);

interior b32
_manifestum_continet (Xar* res_omnes, constans character* via)
{
    i32 index;

    per (index = 0; index < xar_numerus(res_omnes);
        index = index + 1)
    {
        SilexRes* res = (SilexRes*)xar_obtinere(res_omnes, index);

        si (chorda_aequalis_literis(res->via, via))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silex", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* tabula rasa (percursus prior) */
    {
        constans character* argumenta_rm[4];

        argumenta_rm[0] = "/bin/rm";
        argumenta_rm[1] = "-rf";
        argumenta_rm[2] = AREA;
        argumenta_rm[3] = NIHIL;
        processus_exsequi(argumenta_rm, 5000, piscina);
    }

    /* ========================================================
     * PROBARE: clausura - semen sine gemino lib
     * ======================================================== */

    {
        Xar* res_omnes;
        interior constans character* constans SEMINA_LATINA[] = {
            "latina.h"
        };

        imprimere("\n--- Probans clausuram (latina sola) ---\n");

        res_omnes = silex_clausuram_colligere(piscina, ".",
            SEMINA_LATINA, 1);
        CREDO_NON_NIHIL(res_omnes);
        CREDO_AEQUALIS_I32((i32)xar_numerus(res_omnes), (i32)1);
        CREDO_VERUM(_manifestum_continet(res_omnes,
            "include/latina.h"));
    }

    /* ========================================================
     * PROBARE: clausura - geminus lib + transitiva
     * ======================================================== */

    {
        Xar*      res_omnes;
        SilexRes* primus;
        interior constans character* constans SEMINA_CHORDA[] = {
            "chorda.h"
        };

        imprimere("\n--- Probans clausuram (chorda + trans.) ---\n");

        res_omnes = silex_clausuram_colligere(piscina, ".",
            SEMINA_CHORDA, 1);
        CREDO_NON_NIHIL(res_omnes);
        CREDO_VERUM(_manifestum_continet(res_omnes,
            "include/chorda.h"));
        CREDO_VERUM(_manifestum_continet(res_omnes, "lib/chorda.c"));
        /* chorda piscinam trahit (transitive) */
        CREDO_VERUM(_manifestum_continet(res_omnes,
            "include/piscina.h"));
        CREDO_VERUM(_manifestum_continet(res_omnes,
            "lib/piscina.c"));

        primus = (SilexRes*)xar_obtinere(res_omnes, 0);
        CREDO_VERUM(primus->contentum.mensura > ZEPHYRUM);
        CREDO_VERUM(primus->origo[0] == 'v');   /* "vendicata:..." */
    }

    /* ========================================================
     * PROBARE: fabrica invalida = recusatio
     * ======================================================== */

    {
        Xar* res_omnes;
        interior constans character* constans SEMINA_LATINA[] = {
            "latina.h"
        };

        imprimere("\n--- Probans recusationem fabricae ---\n");

        res_omnes = silex_clausuram_colligere(piscina,
            "/non/exsistit/fabrica", SEMINA_LATINA, 1);
        CREDO_NIHIL(res_omnes);
    }

    /* ========================================================
     * PROBARE: fabricam invenire (ascensus e cwd)
     * ======================================================== */

    {
        constans character* radix;
        chorda              expectata;

        imprimere("\n--- Probans fabricam invenire ---\n");

        /* e subdirectorio arboris: radicem ipsam invenit
         * (probationes e radice repositorii currunt) */
        radix = silex_fabricam_invenire(piscina, "lib");
        CREDO_VERUM(radix != NIHIL);
        expectata = via_absoluta(chorda_ex_literis(".", piscina),
            piscina);
        CREDO_CHORDAE_AEQUALES(radix,
            chorda_ut_cstr(expectata, piscina));

        /* ex ipsa radice quoque */
        radix = silex_fabricam_invenire(piscina, ".");
        CREDO_VERUM(radix != NIHIL);

        /* extra arborem: NIHIL (ascensus ad "/" sine signo) */
        CREDO_VERUM(silex_fabricam_invenire(piscina, "/tmp")
            == NIHIL);
    }

    /* ========================================================
     * PROBARE: novum - proiectum integrum
     * ======================================================== */

    {
        SilexNovumOptiones optiones;
        SilexNovumFructus  fructus;

        imprimere("\n--- Probans silex_novum ---\n");

        filum_directorium_creare_si_necesse("build");
        filum_directorium_creare_si_necesse(AREA);

        optiones.fabrica = ".";
        optiones.destinatio = AREA;
        optiones.titulus = "specimen";
        fructus = silex_novum(piscina, &optiones);
        si (!fructus.successus)
        {
            imprimere("silex_novum erratum: %s\n",
                fructus.erratum == NIHIL ? "?" : fructus.erratum);
        }
        CREDO_VERUM(fructus.successus);
        CREDO_VERUM(fructus.vendicatae > (i32)4);
        CREDO_AEQUALIS_I32((i32)fructus.genitae, (i32)5);

        /* veritas + proiectio ambae in disco */
        CREDO_VERUM(filum_existit(
            AREA "/specimen/specimen.volumen"));
        CREDO_VERUM(filum_existit(
            AREA "/specimen/include/latina.h"));
        CREDO_VERUM(filum_existit(AREA "/specimen/lib/chorda.c"));
        CREDO_VERUM(filum_existit(
            AREA "/specimen/fontes/specimen.c"));
        CREDO_VERUM(filum_existit(
            AREA "/specimen/probationes/probatio_specimen.c"));
        CREDO_VERUM(filum_existit(AREA "/specimen/aedificare.sh"));
        CREDO_VERUM(filum_existit(AREA "/specimen/probare.sh"));
        CREDO_VERUM(filum_existit(AREA "/specimen/README.md"));

        /* volumen relegibile; plagulae = vendicatae + genitae */
        {
            Volumen* vol = volumen_aperire(piscina,
                AREA "/specimen/specimen.volumen");

            CREDO_NON_NIHIL(vol);
            CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol),
                (s64)(fructus.vendicatae + fructus.genitae));
            volumen_claudere(vol);
        }

        /* proiectio == volumen: plagula e disco = plagula promota */
        {
            Volumen* vol = volumen_aperire(piscina,
                AREA "/specimen/specimen.volumen");
            chorda e_disco = filum_legere_totum(
                AREA "/specimen/lib/chorda.c", piscina);
            b32    inventum;
            chorda e_volumine = volumen_plagulam_promere(vol,
                chorda_ex_literis("lib/chorda.c", piscina), piscina,
                &inventum);

            CREDO_VERUM(inventum);
            CREDO_CHORDA_AEQUALIS(e_disco, e_volumine);
            volumen_claudere(vol);
        }

        /* novum iterum in eandem destinationem = recusatio */
        fructus = silex_novum(piscina, &optiones);
        CREDO_FALSUM(fructus.successus);
    }

    /* ========================================================
     * PROBARE: VCS - status/condere/historia in specimine
     * ======================================================== */

    {
        SilexStatusFructus   status;
        SilexConditioFructus conditio;
        Xar*                 historia;
        i32                  plagulae_primae;

        imprimere("\n--- Probans status (arbor munda) ---\n");

        status = silex_status(piscina, AREA "/specimen");
        CREDO_VERUM(status.successus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(status.res), (i32)0);
        CREDO_VERUM(status.mundae > (i32)10);
        plagulae_primae = status.mundae;

        imprimere("\n--- Probans status (mutata/nova/absens) ---\n");

        /* mutare, addere, delere */
        CREDO_VERUM(filum_scribere_literis(
            AREA "/specimen/fontes/specimen.c",
            "/* mutatum manu */\n"));
        CREDO_VERUM(filum_scribere_literis(
            AREA "/specimen/notae.md", "# notae\n"));
        CREDO_VERUM(filum_delere(AREA "/specimen/lib/piscina.c"));

        status = silex_status(piscina, AREA "/specimen");
        CREDO_VERUM(status.successus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(status.res), (i32)3);
        CREDO_AEQUALIS_I32((i32)status.mundae,
            (i32)(plagulae_primae - 2));   /* mutata + deleta */
        {
            i32 index;
            i32 mutatae = 0;
            i32 novae = 0;
            i32 absentes = 0;

            per (index = 0; index < xar_numerus(status.res);
                index = index + 1)
            {
                SilexStatusRes* r = (SilexStatusRes*)xar_obtinere(
                    status.res, index);

                si (r->status == SILEX_PLAGULA_MUTATA)
                {
                    mutatae = mutatae + 1;
                }
                alioquin si (r->status == SILEX_PLAGULA_NOVA)
                {
                    novae = novae + 1;
                }
                alioquin
                {
                    absentes = absentes + 1;
                }
            }
            CREDO_AEQUALIS_I32((i32)mutatae, (i32)1);
            CREDO_AEQUALIS_I32((i32)novae, (i32)1);
            CREDO_AEQUALIS_I32((i32)absentes, (i32)1);
        }

        imprimere("\n--- Probans condere ---\n");

        conditio = silex_condere(piscina, AREA "/specimen",
            "prima conditio manualis");
        CREDO_VERUM(conditio.successus);
        CREDO_AEQUALIS_I32((i32)conditio.conditae, (i32)2);
        CREDO_AEQUALIS_I32((i32)conditio.remotae, (i32)1);
        CREDO_VERUM(conditio.seq > (s64)0);

        /* post conditionem: arbor munda iterum */
        status = silex_status(piscina, AREA "/specimen");
        CREDO_VERUM(status.successus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(status.res), (i32)0);
        CREDO_AEQUALIS_I32((i32)status.mundae,
            (i32)plagulae_primae);   /* -1 deleta +1 nova */

        /* arbor munda -> recusatio */
        conditio = silex_condere(piscina, AREA "/specimen",
            "vacua");
        CREDO_FALSUM(conditio.successus);
        CREDO_VERUM(conditio.erratum != NIHIL);

        imprimere("\n--- Probans historiam ---\n");

        historia = silex_historia(piscina, AREA "/specimen");
        CREDO_NON_NIHIL(historia);
        /* ortus + conditio novi + conditio nostra = 3 */
        CREDO_AEQUALIS_I32((i32)xar_numerus(historia), (i32)3);
        {
            SilexConditio* prima = (SilexConditio*)xar_obtinere(
                historia, 0);
            SilexConditio* ultima = (SilexConditio*)xar_obtinere(
                historia, (i32)(xar_numerus(historia) - 1));

            CREDO_CHORDA_AEQUALIS_LITERIS(prima->nuntius,
                "(ortus voluminis)");
            CREDO_CHORDA_AEQUALIS_LITERIS(ultima->nuntius,
                "prima conditio manualis");
            CREDO_AEQUALIS_I32((i32)ultima->tactae, (i32)3);
            CREDO_VERUM(ultima->momentum.mensura > ZEPHYRUM);
        }
    }

    /* ========================================================
     * PROBARE: proicere - volumen arborem scribit
     * ======================================================== */

    {
        SilexProiectioFructus p;
        SilexConditioFructus  conditio;
        SilexStatusFructus    status;
        Xar*                  historia;
        chorda                relatum;

        imprimere("\n--- Probans proicere (consilium/scriptio) ---\n");

        /* arbor munda: nihil proiciendum */
        p = silex_proicere(piscina, AREA "/specimen", (s64)0,
            FALSUM);
        CREDO_VERUM(p.successus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(p.res), (i32)0);
        CREDO_VERUM(p.intactae > (i32)10);

        /* plagula ablata -> CREANDA; -scribere restituit */
        CREDO_VERUM(filum_delere(AREA "/specimen/notae.md"));
        p = silex_proicere(piscina, AREA "/specimen", (s64)0,
            FALSUM);
        CREDO_VERUM(p.successus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(p.res), (i32)1);
        {
            SilexProiciendaRes* r = (SilexProiciendaRes*)
                xar_obtinere(p.res, 0);

            CREDO_VERUM(r->status == SILEX_PROICIENDA_CREANDA);
            CREDO_CHORDA_AEQUALIS_LITERIS(r->via, "notae.md");
        }
        p = silex_proicere(piscina, AREA "/specimen", (s64)0,
            VERUM);
        CREDO_VERUM(p.successus);
        CREDO_AEQUALIS_I32((i32)p.scriptae, (i32)1);
        relatum = filum_legere_totum(AREA "/specimen/notae.md",
            piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(relatum, "# notae\n");

        imprimere("\n--- Probans obicem (inconditum) ---\n");

        /* contentum inconditum -> OBEX; scriptio recusatur,
         * plagula INTACTA manet (foedus: nihil inconditum petit) */
        CREDO_VERUM(filum_scribere_literis(
            AREA "/specimen/notae.md", "# vandalismus\n"));
        p = silex_proicere(piscina, AREA "/specimen", (s64)0,
            FALSUM);
        CREDO_VERUM(p.successus);   /* consilium semper redditur */
        CREDO_AEQUALIS_I32((i32)p.obices, (i32)1);
        {
            SilexProiciendaRes* r = (SilexProiciendaRes*)
                xar_obtinere(p.res, 0);

            CREDO_VERUM(r->status == SILEX_PROICIENDA_OBEX);
        }
        p = silex_proicere(piscina, AREA "/specimen", (s64)0,
            VERUM);
        CREDO_FALSUM(p.successus);
        CREDO_VERUM(p.erratum != NIHIL);
        relatum = filum_legere_totum(AREA "/specimen/notae.md",
            piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(relatum, "# vandalismus\n");

        imprimere("\n--- Probans iter temporis (-ad seq) ---\n");

        /* conditum -> contentum residet -> superscribibile */
        conditio = silex_condere(piscina, AREA "/specimen",
            "experimentum");
        CREDO_VERUM(conditio.successus);

        historia = silex_historia(piscina, AREA "/specimen");
        CREDO_NON_NIHIL(historia);
        CREDO_AEQUALIS_I32((i32)xar_numerus(historia), (i32)4);
        {
            SilexConditio* pristina = (SilexConditio*)xar_obtinere(
                historia, 2);

            CREDO_CHORDA_AEQUALIS_LITERIS(pristina->nuntius,
                "prima conditio manualis");

            /* arbor ut erat ad punctum pristinum */
            p = silex_proicere(piscina, AREA "/specimen",
                pristina->seq, FALSUM);
            CREDO_VERUM(p.successus);
            CREDO_AEQUALIS_I32((i32)xar_numerus(p.res), (i32)1);
            {
                SilexProiciendaRes* r = (SilexProiciendaRes*)
                    xar_obtinere(p.res, 0);

                CREDO_VERUM(r->status
                    == SILEX_PROICIENDA_SCRIBENDA);
            }
            p = silex_proicere(piscina, AREA "/specimen",
                pristina->seq, VERUM);
            CREDO_VERUM(p.successus);
            CREDO_AEQUALIS_I32((i32)p.scriptae, (i32)1);
            relatum = filum_legere_totum(
                AREA "/specimen/notae.md", piscina);
            CREDO_CHORDA_AEQUALIS_LITERIS(relatum, "# notae\n");
        }

        /* arbor vetus, manifestum novum: status honestus MUTATA;
         * praeteritum praesens fit ut punctum NOVUM caudae */
        status = silex_status(piscina, AREA "/specimen");
        CREDO_VERUM(status.successus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(status.res), (i32)1);
        conditio = silex_condere(piscina, AREA "/specimen",
            "reversio");
        CREDO_VERUM(conditio.successus);
        status = silex_status(piscina, AREA "/specimen");
        CREDO_AEQUALIS_I32((i32)xar_numerus(status.res), (i32)0);

        imprimere("\n--- Probans alienam (numquam tangitur) ---\n");

        CREDO_VERUM(filum_scribere_literis(
            AREA "/specimen/vagus.txt", "vagus\n"));
        p = silex_proicere(piscina, AREA "/specimen", (s64)0,
            FALSUM);
        CREDO_VERUM(p.successus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(p.res), (i32)1);
        {
            SilexProiciendaRes* r = (SilexProiciendaRes*)
                xar_obtinere(p.res, 0);

            CREDO_VERUM(r->status == SILEX_PROICIENDA_ALIENA);
        }
        p = silex_proicere(piscina, AREA "/specimen", (s64)0,
            VERUM);
        CREDO_VERUM(p.successus);   /* alienae non obstant */
        CREDO_AEQUALIS_I32((i32)p.scriptae, (i32)0);
        CREDO_VERUM(filum_existit(AREA "/specimen/vagus.txt"));
        CREDO_VERUM(filum_delere(AREA "/specimen/vagus.txt"));
    }

    /* ========================================================
     * PROBARE: identitas voluminis (decisum red-team IX):
     * solitarium vincit, plura recusantur
     * ======================================================== */

    {
        SilexStatusFructus status;

        imprimere("\n--- Probans volumen solitarium ---\n");

        filum_directorium_creare_si_necesse(AREA "/tergum");
        CREDO_VERUM(filum_copiare(
            AREA "/specimen/specimen.volumen",
            AREA "/tergum/aliud.volumen"));

        /* nomen directorio non congruit - solitarium tamen vincit */
        status = silex_status(piscina, AREA "/tergum");
        CREDO_VERUM(status.successus);
        CREDO_AEQUALIS_I32((i32)status.mundae, (i32)0);
        CREDO_VERUM(xar_numerus(status.res) > 10);   /* omnes ABSENS */
        {
            SilexStatusRes* r = (SilexStatusRes*)xar_obtinere(
                status.res, 0);

            CREDO_VERUM(r->status == SILEX_PLAGULA_ABSENS);
        }

        /* volumina duo = ambiguum, recusatio */
        CREDO_VERUM(filum_copiare(
            AREA "/specimen/specimen.volumen",
            AREA "/tergum/alterum.volumen"));
        status = silex_status(piscina, AREA "/tergum");
        CREDO_FALSUM(status.successus);
    }

    /* ========================================================
     * PROBARE: tituli mali recusantur
     * ======================================================== */

    {
        SilexNovumOptiones optiones;
        SilexNovumFructus  fructus;

        imprimere("\n--- Probans recusationem tituli ---\n");

        optiones.fabrica = ".";
        optiones.destinatio = AREA;
        optiones.titulus = "malus/titulus";
        fructus = silex_novum(piscina, &optiones);
        CREDO_FALSUM(fructus.successus);

        optiones.titulus = "";
        fructus = silex_novum(piscina, &optiones);
        CREDO_FALSUM(fructus.successus);
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
