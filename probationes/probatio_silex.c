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
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

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

/* ==================================================
 * Corpus fictum: capsula in memoria (forma: caput XII + TOC
 * XX/introitum + chordae + data; comp==raw = introitus crudus)
 * ================================================== */

nomen structura {
    constans character* via;
    constans character* contentum;
} ParFictum;

interior vacuum
_i32_le_scribere (i8* p, i32 v);

interior vacuum
_i32_le_scribere (i8* p, i32 v)
{
    p[0] = (i8)(v & 0xFF);
    p[1] = (i8)((v >> VIII) & 0xFF);
    p[2] = (i8)((v >> XVI) & 0xFF);
    p[3] = (i8)((v >> XXIV) & 0xFF);
}

interior CapsulaEmbed
_corpus_fictum (Piscina* piscina, constans ParFictum* paria,
    i32 numerus);

interior CapsulaEmbed
_corpus_fictum (Piscina* piscina, constans ParFictum* paria,
    i32 numerus)
{
    CapsulaEmbed embed;
    i32 chordae_mensura = 0;
    i32 datorum_mensura = 0;
    i32 i;
    i32 summa;
    i8* buf;
    i32 chorda_off;
    i32 datum_off;

    per (i = 0; i < numerus; i = i + 1)
    {
        chordae_mensura = chordae_mensura
            + (i32)strlen(paria[i].via);
        datorum_mensura = datorum_mensura
            + (i32)strlen(paria[i].contentum);
    }
    summa = XII + numerus * XX + chordae_mensura + datorum_mensura;
    buf = (i8*)piscina_allocare(piscina, (memoriae_index)summa);
    si (buf == NIHIL)
    {
        embed.datum = NIHIL;
        embed.mensura = 0;
        redde embed;
    }
    _i32_le_scribere(buf, (i32)0x53504143);
    _i32_le_scribere(buf + IV, I);
    _i32_le_scribere(buf + VIII, numerus);
    chorda_off = XII + numerus * XX;
    datum_off  = chorda_off + chordae_mensura;
    per (i = 0; i < numerus; i = i + 1)
    {
        i8* introitus = buf + XII + i * XX;
        i32 via_mensura = (i32)strlen(paria[i].via);
        i32 datum_mensura = (i32)strlen(paria[i].contentum);

        _i32_le_scribere(introitus, chorda_off);
        _i32_le_scribere(introitus + IV, via_mensura);
        _i32_le_scribere(introitus + VIII, datum_off);
        _i32_le_scribere(introitus + XII, datum_mensura);
        _i32_le_scribere(introitus + XVI, datum_mensura);
        memcpy(buf + chorda_off, paria[i].via,
            (size_t)via_mensura);
        memcpy(buf + datum_off, paria[i].contentum,
            (size_t)datum_mensura);
        chorda_off = chorda_off + via_mensura;
        datum_off  = datum_off + datum_mensura;
    }
    embed.datum = buf;
    embed.mensura = summa;
    redde embed;
}

interior constans character*
_texere_probationis (Piscina* piscina, constans character* a,
    constans character* b);

interior constans character*
_texere_probationis (Piscina* piscina, constans character* a,
    constans character* b)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)128);

    chorda_aedificator_appendere_literis(aed, a);
    chorda_aedificator_appendere_literis(aed, b);
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
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

        res_omnes = silex_clausuram_colligere(piscina,
            silex_fons_disci(piscina, "."), SEMINA_LATINA, 1);
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

        res_omnes = silex_clausuram_colligere(piscina,
            silex_fons_disci(piscina, "."), SEMINA_CHORDA, 1);
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
     * PROBARE: fabrica invalida = recusatio (in constructore)
     * ======================================================== */

    {
        Xar* res_omnes;
        interior constans character* constans SEMINA_LATINA[] = {
            "latina.h"
        };

        imprimere("\n--- Probans recusationem fabricae ---\n");

        CREDO_NIHIL(silex_fons_disci(piscina,
            "/non/exsistit/fabrica"));
        res_omnes = silex_clausuram_colligere(piscina,
            silex_fons_disci(piscina, "/non/exsistit/fabrica"),
            SEMINA_LATINA, 1);
        CREDO_NIHIL(res_omnes);
    }

    /* ========================================================
     * PROBARE: fons disci - existit/legere
     * ======================================================== */

    {
        SilexFons* fons = silex_fons_disci(piscina, ".");
        b32        inventum = FALSUM;
        chorda     contentum;

        imprimere("\n--- Probans fontem disci ---\n");
        CREDO_NON_NIHIL(fons);
        CREDO_VERUM(silex_fons_existit(fons, "include/latina.h",
            piscina));
        CREDO_FALSUM(silex_fons_existit(fons, "include/nusquam.h",
            piscina));
        contentum = silex_fons_legere(fons, "include/latina.h",
            piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_VERUM(contentum.mensura > 0);
    }

    /* ========================================================
     * PROBARE: fons corporis == fons disci (porta differentialis)
     * ======================================================== */

    {
        interior constans ParFictum PARIA[] = {
            { "corpus.versio", "commit=abc123 dies=2026-08-10\n" },
            { "include/minima.h", "/* minima */\n" },
            { "lib/minima.c",
              "#include \"minima.h\"\n"
              "#include \"../vendor/parva.h\"\n" },
            { "include/altera.h",
              "#include \"minima.h\"\n" },
            { "lib/altera.c", "/* altera */\n" },
            { "include/obiectiva.h", "/* ob */\n" },
            { "lib/obiectiva_macos.m", "/* ob m */\n" },
            { "vendor/parva.h", "/* v */\n" },
            { "vendor/parva.c", "/* vc */\n" }
        };
        interior constans character* constans SEMINA2[] = {
            "altera.h", "obiectiva.h"
        };
        SilexFons*   discus;
        SilexFons*   corpus;
        Xar*         ex_disco;
        Xar*         ex_corpore;
        i32          index;
        CapsulaEmbed embed = _corpus_fictum(piscina, PARIA, 9);

        imprimere("\n--- Probans corpus == discus ---\n");
        filum_directorium_creare_si_necesse("build");
        filum_directorium_creare_si_necesse(AREA);
        filum_directorium_creare_si_necesse(AREA "/ficta2");
        filum_directorium_creare_si_necesse(AREA "/ficta2/include");
        filum_directorium_creare_si_necesse(AREA "/ficta2/lib");
        filum_directorium_creare_si_necesse(AREA "/ficta2/vendor");
        per (index = 1; index < 9; index = index + 1)
        {
            CREDO_VERUM(filum_scribere_literis(
                _texere_probationis(piscina, AREA "/ficta2/",
                    PARIA[index].via), PARIA[index].contentum));
        }

        discus = silex_fons_disci(piscina, AREA "/ficta2");
        corpus = silex_fons_corporis(piscina, &embed);
        CREDO_NON_NIHIL(discus);
        CREDO_NON_NIHIL(corpus);
        CREDO_CHORDA_CONTINET(chorda_ex_literis(corpus->titulus,
            piscina), chorda_ex_literis("abc123", piscina));

        ex_disco = silex_clausuram_colligere(piscina, discus,
            SEMINA2, 2);
        ex_corpore = silex_clausuram_colligere(piscina, corpus,
            SEMINA2, 2);
        CREDO_NON_NIHIL(ex_disco);
        CREDO_NON_NIHIL(ex_corpore);
        CREDO_AEQUALIS_I32((i32)xar_numerus(ex_disco),
            (i32)xar_numerus(ex_corpore));
        per (index = 0; index < xar_numerus(ex_disco);
            index = index + 1)
        {
            SilexRes* a = (SilexRes*)xar_obtinere(ex_disco, index);
            SilexRes* b = (SilexRes*)xar_obtinere(ex_corpore,
                index);

            CREDO_CHORDA_AEQUALIS(a->via, b->via);
            CREDO_CHORDA_AEQUALIS(a->contentum, b->contentum);
        }
        /* regulae clausurae: geminus obiectivus .m ubi lib/X.c
         * abest; par venditorium normalizatum (numquam '..') */
        CREDO_VERUM(_manifestum_continet(ex_disco,
            "lib/obiectiva_macos.m"));
        CREDO_VERUM(_manifestum_continet(ex_disco,
            "vendor/parva.h"));
        CREDO_VERUM(_manifestum_continet(ex_disco,
            "vendor/parva.c"));
        CREDO_FALSUM(_manifestum_continet(ex_disco,
            "include/../vendor/parva.h"));
        /* altera par II + minima par II + obiectiva.h +
         * obiectiva_macos.m + vendor par II = VIII */
        CREDO_AEQUALIS_I32((i32)xar_numerus(ex_disco), (i32)8);
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

        optiones.fons = silex_fons_disci(piscina, ".");
        optiones.destinatio = AREA;
        optiones.vitrea = FALSUM;
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
     * PROBARE: ascensus proiectum silicis PRAETERIT (volumen
     * radicis = proiectum, numquam fabrica sui ipsius - AMBULATIO
     * ACCEPTIONIS 2026-08-10: partes intra probandum fabricam
     * probandum ipsum invenit, xar.h 'deest', corpus numquam
     * attactum)
     * ======================================================== */

    {
        constans character* radix;
        chorda              expectata;

        imprimere("\n--- Probans ascensum praeter proiectum ---\n");

        /* AREA/specimen signum fabricae fert (include/latina.h +
         * lib/) SED volumen radicis habet -> praetermittendum;
         * ascensus radicem repositorii veri inveniat */
        radix = silex_fabricam_invenire(piscina, AREA "/specimen");
        CREDO_VERUM(radix != NIHIL);
        expectata = via_absoluta(chorda_ex_literis(".", piscina),
            piscina);
        CREDO_CHORDAE_AEQUALES(radix,
            chorda_ut_cstr(expectata, piscina));
    }

    /* ========================================================
     * PROBARE: novum -vitrea - inventarium seminis cordis
     * ======================================================== */

    {
        SilexNovumOptiones optiones;
        SilexNovumFructus  fructus;

        imprimere("\n--- Probans novum -vitrea ---\n");

        optiones.fons = silex_fons_disci(piscina, ".");
        optiones.destinatio = AREA;
        optiones.titulus = "vitreum";
        optiones.vitrea = VERUM;
        fructus = silex_novum(piscina, &optiones);
        si (!fructus.successus)
        {
            imprimere("novum -vitrea erratum: %s\n",
                fructus.erratum);
        }
        CREDO_VERUM(fructus.successus);
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/fontes/vitreum.c"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/fontes/vitreum_pipa.h"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/fontes/vitreum_pipa.c"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/assets/vitreum.toml"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/assets/index.html"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/assets/vitreum.js"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/instrumenta/capsula_generare.c"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/probationes/probatio_vitreum.c"));
        /* clausura vitreae tier: .m par + venditorium par */
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/lib/vitrea_macos.m"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/lib/fenestra_macos.m"));
        /* atrium: semen baculos inclusos accipit (imperium ->
         * '-vivum'), et trahit CLAUSURAM SUAM - tls_macos.m hic
         * est quia atrium -> imperium -> hospitium -> tls. Illa
         * plagula est quae '-framework Security' poscit; sine
         * ea assertio infra causam suam amitteret. */
        CREDO_VERUM(filum_existit(AREA "/vitreum/lib/atrium.c"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/include/atrium.h"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/lib/tls_macos.m"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/vendor/sqlite3.c"));
        /* clausura instrumenti: toml quod app non trahit */
        CREDO_VERUM(filum_existit(AREA "/vitreum/lib/toml.c"));
        /* clausura probationis: credo quem app non trahit
         * (ambulatio acceptionis hoc cepit) */
        CREDO_VERUM(filum_existit(AREA "/vitreum/lib/credo.c"));
        /* titulus cum '-' recusatur (symbolum C fieret) */
        optiones.titulus = "malum-nomen";
        fructus = silex_novum(piscina, &optiones);
        CREDO_FALSUM(fructus.successus);

        /* ordines generati: stampa, sine globo, listae
         * computatae */
        {
            chorda aed = filum_legere_totum(
                AREA "/vitreum/aedificare.sh", piscina);
            chorda prob = filum_legere_totum(
                AREA "/vitreum/probare.sh", piscina);
            chorda app = filum_legere_totum(
                AREA "/vitreum/fontes/vitreum.c", piscina);
            chorda prob_c = filum_legere_totum(
                AREA "/vitreum/probationes/probatio_vitreum.c",
                piscina);

            /* '-vivum' in semine: NON vexillum quod app legat, sed
             * atrium_vexilla_legere cui argv traditur. Sine argv in
             * principali vexillum invisibile est - ergo utrumque
             * figitur, non alterum solum. */
            CREDO_CHORDA_CONTINET(app, chorda_ex_literis(
                "principale (integer argc, character** argv)",
                piscina));
            CREDO_CHORDA_CONTINET(app, chorda_ex_literis(
                "atrium_vexilla_legere(&figura, argc, argv)",
                piscina));
            /* gyrus APP est, non atrii (vide atrium.h) */
            CREDO_CHORDA_CONTINET(app, chorda_ex_literis(
                "dum (atrium_currendum(atrium))", piscina));

            /* '-volumen': sine eo probatio integrationis in datis
             * VERIS usoris currere cogitur (mensuratum in
             * silicetum/003 et ante in laboratorio) */
            CREDO_CHORDA_CONTINET(app, chorda_ex_literis(
                "strcmp(argv[i], \"-volumen\")", piscina));

            /* ORDO: volumen POST atrium. Volumen aperire plagulam
             * CREAT, ergo cursus vexillo pravo moriens statum in
             * disco relinquebat. Figitur per semitam purgationis
             * quae SOLA in ea ordinatione exsistere potest. */
            CREDO_CHORDA_CONTINET(app, chorda_ex_literis(
                "atrium_destruere(atrium);\n        redde I;",
                piscina));

            /* probatio genita volumen TEMPORARIUM adhibeat - et
             * viam fixam NON iam nominet (assertio negativa: ea
             * sola distinguit 'mutatum' a 'utrumque praesens') */
            CREDO_CHORDA_CONTINET(prob_c, chorda_ex_literis(
                "volumen_temporarium(piscina,", piscina));
            CREDO_FALSUM(chorda_continet(prob_c,
                chorda_ex_literis("build/probatio.volumen",
                    piscina)));

            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "GENERATUM a silice (novum -vitrea", piscina));
            CREDO_FALSUM(chorda_continet(aed,
                chorda_ex_literis("lib/*.c", piscina)));
            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "lib/toml.c", piscina));
            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "lib/vitrea_macos.m", piscina));
            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "vendor/sqlite3.c", piscina));
            /* Security ADEST quia tls_macos.m in clausura est
             * (vide supra). Hoc ANTE portam fumi deerat, et nexus
             * omnis proiecti novi deficiebat sine ulla probatione
             * rubente - quia haec probatio manifestum iudicat, non
             * aedificationem. Porta vera:
             * ./tools/silex_semen_fumus.sh */
            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "-framework Cocoa -framework Security"
                " -framework WebKit", piscina));
            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "rm -f bin/vitreum", piscina));
            CREDO_CHORDA_CONTINET(prob, chorda_ex_literis(
                "GENERATUM a silice", piscina));
            CREDO_FALSUM(chorda_continet(prob,
                chorda_ex_literis("lib/*.c", piscina)));
            CREDO_FALSUM(chorda_continet(prob,
                chorda_ex_literis("_macos.m", piscina)));
            CREDO_CHORDA_CONTINET(prob, chorda_ex_literis(
                "fontes/vitreum_pipa.c", piscina));
        }
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
     * PROBARE: renovare - sigilla tria contra fabricam FICTAM
     * (fabrica mutabilis in area - fabricam veram mutare non
     * possumus)
     * ======================================================== */

    {
        SilexRenovatioFructus r;
        Volumen*              vol;
        chorda                contentum;
        b32                   bene;
        SilexFons*            fons_fictae;

        imprimere("\n--- Probans renovare (fabrica ficta) ---\n");

        /* fabrica ficta v1 */
        filum_directorium_creare_si_necesse(AREA "/ficta");
        filum_directorium_creare_si_necesse(AREA "/ficta/include");
        filum_directorium_creare_si_necesse(AREA "/ficta/lib");
        CREDO_VERUM(filum_scribere_literis(
            AREA "/ficta/include/minima.h", "/* minima v1 */\n"));
        CREDO_VERUM(filum_scribere_literis(
            AREA "/ficta/lib/minima.c",
            "#include \"minima.h\"\n/* corpus v1 */\n"));
        fons_fictae = silex_fons_disci(piscina, AREA "/ficta");
        CREDO_NON_NIHIL(fons_fictae);

        /* proiectum manu vendicatum (v1 utrimque) + plagula genita */
        filum_directorium_creare_si_necesse(AREA "/renovandum");
        filum_directorium_creare_si_necesse(
            AREA "/renovandum/include");
        filum_directorium_creare_si_necesse(AREA "/renovandum/lib");
        filum_directorium_creare_si_necesse(
            AREA "/renovandum/fontes");
        vol = volumen_creare(piscina,
            AREA "/renovandum/renovandum.volumen");
        CREDO_NON_NIHIL(vol);
        bene = volumen_plagulam_condere(vol,
            chorda_ex_literis("include/minima.h", piscina),
            chorda_ex_literis("/* minima v1 */\n", piscina),
            "vendicata:include/minima.h");
        CREDO_VERUM(bene);
        bene = volumen_plagulam_condere(vol,
            chorda_ex_literis("lib/minima.c", piscina),
            chorda_ex_literis(
                "#include \"minima.h\"\n/* corpus v1 */\n",
                piscina),
            "vendicata:lib/minima.c");
        CREDO_VERUM(bene);
        bene = volumen_plagulam_condere(vol,
            chorda_ex_literis("fontes/meum.c", piscina),
            chorda_ex_literis("/* meum est */\n", piscina),
            "genita");
        CREDO_VERUM(bene);
        volumen_claudere(vol);
        CREDO_VERUM(filum_scribere_literis(
            AREA "/renovandum/include/minima.h",
            "/* minima v1 */\n"));
        CREDO_VERUM(filum_scribere_literis(
            AREA "/renovandum/lib/minima.c",
            "#include \"minima.h\"\n/* corpus v1 */\n"));
        CREDO_VERUM(filum_scribere_literis(
            AREA "/renovandum/fontes/meum.c", "/* meum est */\n"));

        /* I. omnia recentia */
        r = silex_renovare(piscina, AREA "/renovandum",
            fons_fictae, FALSUM);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(r.res), (i32)0);
        CREDO_AEQUALIS_I32((i32)r.intactae, (i32)2);

        /* II. fabrica movetur -> RENOVANDA; -scribere applicat */
        CREDO_VERUM(filum_scribere_literis(
            AREA "/ficta/include/minima.h", "/* minima v2 */\n"));
        r = silex_renovare(piscina, AREA "/renovandum",
            fons_fictae, FALSUM);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(r.res), (i32)1);
        {
            SilexRenovatioRes* rr = (SilexRenovatioRes*)
                xar_obtinere(r.res, 0);

            CREDO_VERUM(rr->status == SILEX_RENOVATIO_RENOVANDA);
            CREDO_CHORDA_AEQUALIS_LITERIS(rr->via,
                "include/minima.h");
        }
        r = silex_renovare(piscina, AREA "/renovandum",
            fons_fictae, VERUM);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32((i32)r.renovatae, (i32)1);
        contentum = filum_legere_totum(
            AREA "/renovandum/include/minima.h", piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(contentum,
            "/* minima v2 */\n");
        /* idempotens: iterum = omnia recentia (tunc novum factum) */
        r = silex_renovare(piscina, AREA "/renovandum",
            fons_fictae, FALSUM);
        CREDO_AEQUALIS_I32((i32)xar_numerus(r.res), (i32)0);
        CREDO_AEQUALIS_I32((i32)r.intactae, (i32)2);

        /* III. manus editio, fabrica immota -> VULNUS retinetur */
        CREDO_VERUM(filum_scribere_literis(
            AREA "/renovandum/lib/minima.c", "/* meum iam */\n"));
        r = silex_renovare(piscina, AREA "/renovandum",
            fons_fictae, VERUM);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32((i32)r.renovatae, (i32)0);
        CREDO_AEQUALIS_I32((i32)xar_numerus(r.res), (i32)1);
        {
            SilexRenovatioRes* rr = (SilexRenovatioRes*)
                xar_obtinere(r.res, 0);

            CREDO_VERUM(rr->status == SILEX_RENOVATIO_VULNUS);
        }
        contentum = filum_legere_totum(
            AREA "/renovandum/lib/minima.c", piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(contentum, "/* meum iam */\n");

        /* IV. ambae motae -> CONFLICTUS retinetur */
        CREDO_VERUM(filum_scribere_literis(
            AREA "/ficta/lib/minima.c",
            "#include \"minima.h\"\n/* corpus v2 */\n"));
        r = silex_renovare(piscina, AREA "/renovandum",
            fons_fictae, VERUM);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32((i32)r.renovatae, (i32)0);
        {
            SilexRenovatioRes* rr = (SilexRenovatioRes*)
                xar_obtinere(r.res, 0);

            CREDO_VERUM(rr->status == SILEX_RENOVATIO_CONFLICTUS);
        }
        contentum = filum_legere_totum(
            AREA "/renovandum/lib/minima.c", piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(contentum, "/* meum iam */\n");

        /* manus retracta (v1 restituta) -> RENOVANDA tuta iterum */
        CREDO_VERUM(filum_scribere_literis(
            AREA "/renovandum/lib/minima.c",
            "#include \"minima.h\"\n/* corpus v1 */\n"));
        r = silex_renovare(piscina, AREA "/renovandum",
            fons_fictae, VERUM);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32((i32)r.renovatae, (i32)1);
        contentum = filum_legere_totum(
            AREA "/renovandum/lib/minima.c", piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(contentum,
            "#include \"minima.h\"\n/* corpus v2 */\n");

        /* V. dependentia nova in clausura -> ADDENDA */
        CREDO_VERUM(filum_scribere_literis(
            AREA "/ficta/include/minima.h",
            "#include \"nova.h\"\n/* minima v3 */\n"));
        CREDO_VERUM(filum_scribere_literis(
            AREA "/ficta/include/nova.h", "/* nova v1 */\n"));
        r = silex_renovare(piscina, AREA "/renovandum",
            fons_fictae, VERUM);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32((i32)r.renovatae, (i32)1);
        CREDO_AEQUALIS_I32((i32)r.additae, (i32)1);
        CREDO_VERUM(filum_existit(
            AREA "/renovandum/include/nova.h"));
        {
            b32 inventum = FALSUM;

            vol = volumen_aperire(piscina,
                AREA "/renovandum/renovandum.volumen");
            CREDO_NON_NIHIL(vol);
            (vacuum)volumen_plagulam_promere(vol,
                chorda_ex_literis("include/nova.h", piscina),
                piscina, &inventum);
            CREDO_VERUM(inventum);
            volumen_claudere(vol);
        }

        /* VI. fabrica plagulam demittit -> DERELICTA retinetur */
        CREDO_VERUM(filum_delere(AREA "/ficta/include/minima.h"));
        CREDO_VERUM(filum_delere(AREA "/ficta/lib/minima.c"));
        r = silex_renovare(piscina, AREA "/renovandum",
            fons_fictae, FALSUM);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32((i32)xar_numerus(r.res), (i32)2);
        {
            i32 index;
            i32 derelictae = 0;

            per (index = 0; index < xar_numerus(r.res);
                index = index + 1)
            {
                SilexRenovatioRes* rr = (SilexRenovatioRes*)
                    xar_obtinere(r.res, index);

                si (rr->status == SILEX_RENOVATIO_DERELICTA)
                {
                    derelictae = derelictae + 1;
                }
            }
            CREDO_AEQUALIS_I32((i32)derelictae, (i32)2);
        }
        CREDO_VERUM(filum_existit(
            AREA "/renovandum/include/minima.h"));
        CREDO_VERUM(filum_existit(AREA "/renovandum/lib/minima.c"));

        /* VII. RE-RADICATIO: plagula auctorata (disco solo, ante
         * conditionem!) inclusionem novam accipit -> ADDENDA e
         * clausura recomputata. ORACULUM: ante emendationem hoc
         * FALLIT ('omnia recentia' praeter derelictas - semina
         * erant capita vendicata sola; mensuratum in vocabulario
         * 2026-08-09). */
        CREDO_VERUM(filum_scribere_literis(
            AREA "/ficta/include/addenda.h", "/* addenda */\n"));
        CREDO_VERUM(filum_scribere_literis(
            AREA "/ficta/lib/addenda.c", "/* addenda c */\n"));
        CREDO_VERUM(filum_scribere_literis(
            AREA "/renovandum/fontes/novus.c",
            "#include \"addenda.h\"\n"));
        r = silex_renovare(piscina, AREA "/renovandum",
            fons_fictae, FALSUM);
        CREDO_VERUM(r.successus);
        {
            b32 addenda_caput = FALSUM;
            b32 addenda_lib = FALSUM;
            i32 index;

            per (index = 0; index < xar_numerus(r.res);
                index = index + 1)
            {
                SilexRenovatioRes* rr = (SilexRenovatioRes*)
                    xar_obtinere(r.res, index);

                si (rr->status == SILEX_RENOVATIO_ADDENDA)
                {
                    si (chorda_aequalis_literis(rr->via,
                        "include/addenda.h"))
                    {
                        addenda_caput = VERUM;
                    }
                    si (chorda_aequalis_literis(rr->via,
                        "lib/addenda.c"))
                    {
                        addenda_lib = VERUM;
                    }
                }
            }
            CREDO_VERUM(addenda_caput);
            CREDO_VERUM(addenda_lib);
        }

        /* VIII. partes: oraculum clausurae cum columna ADEST.
         * addenda nondum vendicata (VII consilium solum erat) ->
         * ABEST; nova.h vendicata (V) et in disco -> ADEST.
         * (minima e ficta deleta VI - in clausura non apparet.) */
        {
            Xar* partes = silex_partes(piscina, AREA "/renovandum",
                fons_fictae, NIHIL);
            b32  addenda_abest = FALSUM;
            b32  nova_adest = FALSUM;
            i32  index;

            imprimere("\n--- Probans partes ---\n");
            CREDO_NON_NIHIL(partes);
            per (index = 0; index < xar_numerus(partes);
                index = index + 1)
            {
                SilexPartesRes* pr = (SilexPartesRes*)xar_obtinere(
                    partes, index);

                si (chorda_aequalis_literis(pr->via,
                    "include/addenda.h") && !pr->adest)
                {
                    addenda_abest = VERUM;
                }
                si (chorda_aequalis_literis(pr->via,
                    "include/nova.h") && pr->adest)
                {
                    nova_adest = VERUM;
                }
            }
            CREDO_VERUM(addenda_abest);
            CREDO_VERUM(nova_adest);
        }
    }

    /* ========================================================
     * PROBARE: historia plagulae unius + vexillum renovationis
     * (fixturae iam divites: notae.md ter condita in specimine,
     * lib/piscina.c nata et remota; renovandum conditiones
     * renovationum veras fert)
     * ======================================================== */

    {
        Xar* hp;

        imprimere("\n--- Probans historiam plagulae (notae.md) ---\n");

        /* nova (prima manualis) + vandalismus (experimentum)
         * + restitutio (reversio) = 3 */
        hp = silex_historia_plagulae(piscina, AREA "/specimen",
            "notae.md");
        CREDO_NON_NIHIL(hp);
        CREDO_AEQUALIS_I32((i32)xar_numerus(hp), (i32)3);
        {
            SilexPlagulaConditio* prima = (SilexPlagulaConditio*)
                xar_obtinere(hp, 0);
            SilexPlagulaConditio* ultima = (SilexPlagulaConditio*)
                xar_obtinere(hp, 2);

            CREDO_CHORDA_AEQUALIS_LITERIS(prima->nuntius,
                "prima conditio manualis");
            CREDO_CHORDA_AEQUALIS_LITERIS(ultima->nuntius,
                "reversio");
            CREDO_FALSUM(prima->remota);
            CREDO_FALSUM(prima->renovatio);
            CREDO_AEQUALIS_I32((i32)prima->sigillum.mensura,
                (i32)64);
            CREDO_VERUM(prima->seq < ultima->seq);
            /* reversio contentum pristinum restituit - sigilla
             * prima et ultima aequalia, media alia */
            CREDO_VERUM(chorda_aequalis(prima->sigillum,
                ultima->sigillum));
            {
                SilexPlagulaConditio* media =
                    (SilexPlagulaConditio*)xar_obtinere(hp, 1);

                CREDO_FALSUM(chorda_aequalis(prima->sigillum,
                    media->sigillum));
            }
        }

        imprimere("\n--- Probans historiam plagulae (remota) ---\n");

        /* nata (conditio novi) + remota (prima manualis) = 2 */
        hp = silex_historia_plagulae(piscina, AREA "/specimen",
            "lib/piscina.c");
        CREDO_NON_NIHIL(hp);
        CREDO_AEQUALIS_I32((i32)xar_numerus(hp), (i32)2);
        {
            SilexPlagulaConditio* nata = (SilexPlagulaConditio*)
                xar_obtinere(hp, 0);
            SilexPlagulaConditio* remota = (SilexPlagulaConditio*)
                xar_obtinere(hp, 1);

            CREDO_FALSUM(nata->remota);
            CREDO_VERUM(remota->remota);
            CREDO_AEQUALIS_I32((i32)remota->sigillum.mensura,
                (i32)0);
        }

        imprimere("\n--- Probans historiam plagulae (ignota) ---\n");

        hp = silex_historia_plagulae(piscina, AREA "/specimen",
            "numquam/fuit.c");
        CREDO_NON_NIHIL(hp);
        CREDO_AEQUALIS_I32((i32)xar_numerus(hp), (i32)0);

        imprimere("\n--- Probans vexillum renovationis ---\n");

        /* specimen: nulla renovatio umquam */
        {
            Xar* h = silex_historia(piscina, AREA "/specimen");
            i32  index;

            CREDO_NON_NIHIL(h);
            per (index = 0; index < xar_numerus(h);
                index = index + 1)
            {
                SilexConditio* c = (SilexConditio*)xar_obtinere(
                    h, index);

                CREDO_FALSUM(c->renovatio);
            }
        }

        /* renovandum: conditiones renovationum verae adsunt */
        {
            Xar* h = silex_historia(piscina, AREA "/renovandum");
            i32  index;
            i32  renovationes = 0;

            CREDO_NON_NIHIL(h);
            per (index = 0; index < xar_numerus(h);
                index = index + 1)
            {
                SilexConditio* c = (SilexConditio*)xar_obtinere(
                    h, index);

                si (c->renovatio)
                {
                    renovationes = renovationes + 1;
                }
            }
            CREDO_VERUM(renovationes >= (i32)2);
            /* ortus numquam renovatio */
            CREDO_FALSUM(((SilexConditio*)xar_obtinere(h, 0))
                ->renovatio);
        }

        /* historia plagulae vexillum quoque fert: actus nudi ortus
         * (ante conditionem ullam) conditioni renovationis primae
         * attribuuntur - regula 'ultimus inter limites vincit' */
        hp = silex_historia_plagulae(piscina, AREA "/renovandum",
            "include/minima.h");
        CREDO_NON_NIHIL(hp);
        CREDO_AEQUALIS_I32((i32)xar_numerus(hp), (i32)2);
        {
            SilexPlagulaConditio* prima = (SilexPlagulaConditio*)
                xar_obtinere(hp, 0);
            SilexPlagulaConditio* ultima = (SilexPlagulaConditio*)
                xar_obtinere(hp, 1);

            CREDO_VERUM(prima->renovatio);
            CREDO_VERUM(ultima->renovatio);
            CREDO_VERUM(chorda_incipit(prima->origo,
                chorda_ex_literis("vendicata:", piscina)));
        }
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

        optiones.fons = silex_fons_disci(piscina, ".");
        optiones.destinatio = AREA;
        optiones.vitrea = FALSUM;
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
