/* probatio_briar_fabrica.c - Fabrica sine clang: inventarium, regula
 * principalis, partitio unitatum, plagulae genitae contra AURUM
 * (fixa/fabrica/<t>/...) octetim, clausura, clavis, scriptor.
 * Regeneratio auri: BRIAR_FABRICA_SCRIBERE=1 cum causa nominata.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_fabrica.h"
#include "briar_nexus.h"
#include "briar_silva.h"
#include "filum.h"
#include "internamentum.h"
#include "piscina.h"
#include "silex.h"
#include "via.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

hic_manens constans character* AURUM =
    "briar/probationes/fixa/fabrica/";
/* NON briar/build/fabrica/ - id binarium instrumenti est */
hic_manens constans character* AREA  = "briar/build/fabrica_area/";

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior constans character*
_texere (
               Piscina* piscina,
    constans character* a,
    constans character* b,
    constans character* c)
{
    chorda ab = chorda_concatenare(chorda_ex_literis(a, piscina),
        chorda_ex_literis(b, piscina), piscina);

    si (c != NIHIL)
    {
        ab = chorda_concatenare(ab, chorda_ex_literis(c, piscina),
            piscina);
    }
    redde chorda_ut_cstr(ab, piscina);
}

interior BriarFabricaFructus
_fabricare (
               Piscina*  piscina,
   InternamentumChorda*  intern,
    constans SilexFons*  fons,
    constans character*  via,
                   Xar** nexus_out)
{
     BriarFabricaFructus  fructus;
    BriarFabricaOptiones  optiones;
               character* textus;
                     i32  mensura = ZEPHYRUM;
           MateriaNodus* doc;
                    Xar* nexus;
                  chorda octeti;

    memset(&fructus, 0, magnitudo(fructus));
    *nexus_out  = NIHIL;
    textus      = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fructus.causa = chorda_ex_literis("plagula non lecta", piscina);
        redde fructus;
    }
    doc    = briar_arbor_parsare(piscina, textus, mensura);
    nexus  = briar_nexus_texere(piscina, doc, intern);
    (vacuum)briar_silvam_texere(piscina, nexus, fons);
    optiones.via_thistle   = via;
    optiones.stampa        = "probatio";
    optiones.fons_titulus  = "probatio";
    octeti.datum           = (i8*)textus;
    octeti.mensura         = mensura;
    fructus = briar_fabricare(piscina, doc, nexus, fons, &optiones,
        octeti);
    *nexus_out = nexus;
    redde fructus;
}

interior constans BriarPlagula*
_genita (
    constans BriarFabricaFructus* f,
              constans character* via)
{
    i32 i;

    si (f->genitae == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(f->genitae); i++)
    {
        constans BriarPlagula* p = (constans BriarPlagula*)xar_obtinere(
            f->genitae, i);

        si (chorda_aequalis_literis(p->via, via))
        {
            redde p;
        }
    }
    redde NIHIL;
}

interior b32
_clausura_habet (
    constans BriarFabricaFructus* f,
              constans character* via)
{
    i32 i;

    si (f->clausura == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(f->clausura); i++)
    {
        constans SilexRes* r = (constans SilexRes*)xar_obtinere(
            f->clausura, i);

        si (chorda_aequalis_literis(r->via, via))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_continet (
               Piscina* piscina,
                chorda  fenum,
    constans character* acus)
{
    redde (b32)(strstr(chorda_ut_cstr(fenum, piscina), acus) != NIHIL);
}

/* genita contra aurum; BRIAR_FABRICA_SCRIBERE=1 aurum scribit */
interior vacuum
_aurum_conferre (
                        Piscina* piscina,
    constans BriarFabricaFructus* f,
             constans character* via)
{
    constans BriarPlagula* p = _genita(f, via);
       constans character* via_auri = _texere(piscina, AURUM,
           _texere(piscina, f->titulus, "/", via), NIHIL);

    CREDO_NON_NIHIL (p);
    si (p == NIHIL)
    {
        redde;
    }
    si (getenv("BRIAR_FABRICA_SCRIBERE") != NIHIL)
    {
        chorda parens = via_directorium(chorda_ex_literis(via_auri,
            piscina), piscina);

        CREDO_VERUM (briar_directoria_creare(piscina,
            chorda_ut_cstr(parens, piscina)));
        si (filum_scribere(via_auri, p->contentum))
        {
            imprimere("  AURUM SCRIPTUM: %s\n", via_auri);
        }
        alioquin
        {
            CREDO_CULPA ("aurum non scriptum");
        }
        redde;
    }
    {
        chorda aurum = filum_legere_totum(via_auri, piscina);

        CREDO_VERUM (aurum.mensura > ZEPHYRUM);
        si (!chorda_aequalis(aurum, p->contentum))
        {
            imprimere("  DIFFERT: %s\n--- genitum ---\n%.*s\n"
                "--- aurum ---\n"
                "%.*s\n", via_auri, (integer)p->contentum.mensura,
                (constans character*)p->contentum.datum,
                (integer)aurum.mensura,
                (constans character*)aurum.datum);
        }
        CREDO_CHORDA_AEQUALIS (p->contentum, aurum);
    }
}

s32
principale (vacuum)
{
                  b32  praeteritus;
              Piscina* piscina;
  InternamentumChorda* intern;
   constans character* radix;
   constans SilexFons* fons;

    piscina = piscina_generare_dynamicum("probatio_briar_fabrica",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern  = internamentum_creare(piscina);
    radix   = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    fons = silex_fons_disci(piscina, radix);
    CREDO_NON_NIHIL (fons);

    imprimere("\n--- Probans titulus, vexilla, clavis ---\n");
    {
        character a[17];
        character b[17];
           chorda octeti =
               chorda_ex_literis("#!/usr/bin/env briar\n",
               piscina);

        CREDO_VERUM (strcmp(briar_fabrica_titulus(piscina,
            "a/b/salve.thistle"), "salve") == ZEPHYRUM);
        CREDO_VERUM (strcmp(briar_fabrica_titulus(piscina,
            "salve-mundi.thistle"), "salve_mundi") == ZEPHYRUM);
        CREDO_VERUM (strcmp(briar_fabrica_titulus(piscina, "x"), "x")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(briar_fabrica_vexilla(BRIAR_FORMA_PLANA),
            SILEX_VEXILLA_COMPILATIONIS) == ZEPHYRUM);
        CREDO_VERUM (strstr(briar_fabrica_vexilla(BRIAR_FORMA_VITREA),
            "-DSQLITE_ENABLE_FTS5") != NIHIL);
        briar_fabrica_clavem_computare("stampa", "-Wall", octeti, a);
        briar_fabrica_clavem_computare("stampa", "-Wall", octeti, b);
        CREDO_AEQUALIS_I32 ((i32)strlen(a), (i32)16);
        CREDO_VERUM (strcmp(a, b) == ZEPHYRUM);
        CREDO_VERUM (strspn(a, "0123456789abcdef") == (size_t)16);
        briar_fabrica_clavem_computare("stampa", "-Wall -O2", octeti,
            b);
        CREDO_VERUM (strcmp(a, b) != ZEPHYRUM);
        briar_fabrica_clavem_computare("stampa2", "-Wall", octeti, b);
        CREDO_VERUM (strcmp(a, b) != ZEPHYRUM);
    }

    imprimere("\n--- Probans salve (planum): aurum, scriptor ---\n");
    {
                        Xar* nexus;
        BriarFabricaFructus  f = _fabricare(piscina, intern, fons,
            "briar/probationes/fixa/thistle/salve.thistle", &nexus);

        si (!f.successus)
        {
            imprimere("  RECUSATIO: %.*s\n", (integer)f.causa.mensura,
                (constans character*)f.causa.datum);
        }
        CREDO_VERUM (f.successus);
        CREDO_AEQUALIS_S32 ((s32)f.forma, (s32)BRIAR_FORMA_PLANA);
        CREDO_VERUM (strcmp(f.titulus, "salve") == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (f.regiones_c, I);
        CREDO_VERUM (f.probatio_adest);
        CREDO_AEQUALIS_I32 ((i32)strlen(f.sigillum), (i32)16);
        _aurum_conferre(piscina, &f, "fontes/salve.c");
        _aurum_conferre(piscina, &f, "fontes/salve_regiones.c");
        _aurum_conferre(piscina, &f, "include/salve_regiones.h");
        _aurum_conferre(piscina, &f, "probationes/probatio_salve.c");
        _aurum_conferre(piscina, &f, "aedificare.sh");
        _aurum_conferre(piscina, &f, "probare.sh");
        CREDO_AEQUALIS_I32 (xar_numerus(f.genitae), (i32)6);
        /* clausura: chorda (regio) + credo (probatio) + latina */
        CREDO_VERUM (_clausura_habet(&f, "include/latina.h"));
        CREDO_VERUM (_clausura_habet(&f, "include/chorda.h"));
        CREDO_VERUM (_clausura_habet(&f, "lib/chorda.c"));
        CREDO_VERUM (_clausura_habet(&f, "include/credo.h"));
        CREDO_VERUM (_clausura_habet(&f, "lib/credo.c"));
        CREDO_FALSUM (_clausura_habet(&f, "include/atrium.h"));
        /* '#line' ad lineam unitatis: 'principale' incipit linea IX
         * (linea VII est '#include "chorda.h"' - directiva, in
         * caput) */
        {
            constans BriarPlagula* p = _genita(&f, "fontes/salve.c");
            constans BriarPlagula* h = _genita(&f,
                "include/salve_regiones.h");

            CREDO_VERUM (_continet(piscina, p->contentum,
                "#line 9 \"briar/probationes/fixa/thistle/"
                "salve.thistle\""));
            CREDO_VERUM (_continet(piscina, h->contentum,
                "#line 7 \"briar/probationes/fixa/thistle/"
                "salve.thistle\"\n"
                "#include \"chorda.h\""));
        }
        /* scriptor */
        {
            constans character* dir = _texere(piscina, AREA, "salve",
                NIHIL);
                         chorda causa;
                  struct stat st;

            si (!briar_fabricam_scribere(piscina, &f, dir, &causa))
            {
                imprimere("  SCRIPTOR: %.*s\n", (integer)causa.mensura,
                    (constans character*)causa.datum);
                CREDO_CULPA ("scriptor fractus");
            }
            CREDO_VERUM (filum_existit(_texere(piscina, dir,
                "/include/salve_regiones.h", NIHIL)));
            CREDO_VERUM (filum_existit(_texere(piscina, dir,
                "/lib/chorda.c", NIHIL)));
            CREDO_VERUM (filum_existit(_texere(piscina, dir,
                "/include/credo.h", NIHIL)));
            CREDO_VERUM (stat(_texere(piscina, dir, "/aedificare.sh",
                NIHIL),
                &st) == ZEPHYRUM);
            CREDO_VERUM ((st.st_mode & S_IXUSR) != ZEPHYRUM);
            CREDO_CHORDA_AEQUALIS (filum_legere_totum(_texere(piscina,
                dir,
                "/fontes/salve.c", NIHIL), piscina),
                _genita(&f, "fontes/salve.c")->contentum);
        }
        briar_silvam_solvere(nexus);
    }

    imprimere("\n--- Probans punctum: partitio unitatum ---\n");
    {
                        Xar* nexus;
        BriarFabricaFructus  f = _fabricare(piscina, intern, fons,
            "briar/probationes/fixa/thistle/punctum.thistle", &nexus);
        constans BriarPlagula* caput;
        constans BriarPlagula* corpus;
        constans BriarPlagula* princeps;

        si (!f.successus)
        {
            imprimere("  RECUSATIO: %.*s\n", (integer)f.causa.mensura,
                (constans character*)f.causa.datum);
        }
        CREDO_VERUM (f.successus);
        _aurum_conferre(piscina, &f, "fontes/punctum.c");
        _aurum_conferre(piscina, &f, "fontes/punctum_regiones.c");
        _aurum_conferre(piscina, &f, "include/punctum_regiones.h");
        _aurum_conferre(piscina, &f, "probationes/probatio_punctum.c");
        caput     = _genita(&f, "include/punctum_regiones.h");
        corpus    = _genita(&f, "fontes/punctum_regiones.c");
        princeps  = _genita(&f, "fontes/punctum.c");
        CREDO_NON_NIHIL (caput);
        CREDO_NON_NIHIL (corpus);
        CREDO_NON_NIHIL (princeps);
        si (caput == NIHIL || corpus == NIHIL || princeps == NIHIL)
        {
            CREDO_CULPA ("plagulae genitae absunt");
        }
        alioquin
        {
            /* caput: directivae + typus + prototypus; NON obiectum, NON
             * corpus functionis */
            CREDO_VERUM (_continet(piscina, caput->contentum,
                "#define PUNCTUM_MAXIMUM 3"));
            CREDO_VERUM (_continet(piscina, caput->contentum,
                "#include \"chorda.h\""));
            CREDO_VERUM (_continet(piscina, caput->contentum,
                "} Punctum;"));
            CREDO_VERUM (_continet(piscina, caput->contentum,
                "punctum_summa (constans Punctum* p);"));
            CREDO_FALSUM (_continet(piscina, caput->contentum,
                "numerus_vocationum = ZEPHYRUM"));
            CREDO_FALSUM (_continet(piscina, caput->contentum,
                "redde p->x + p->y"));
            CREDO_VERUM (_continet(piscina, caput->contentum,
                "#ifndef PUNCTUM_REGIONES_H"));
            /* corpus: obiectum + definitio; NON principale */
            CREDO_VERUM (_continet(piscina, corpus->contentum,
                "hic_manens i32 numerus_vocationum = ZEPHYRUM;"));
            CREDO_VERUM (_continet(piscina, corpus->contentum,
                "redde p->x + p->y;"));
            CREDO_FALSUM (_continet(piscina, corpus->contentum,
                "principale"));
            /* princeps: principale solum, extensione secta ('}'
             * ultimum) */
            CREDO_VERUM (_continet(piscina, princeps->contentum,
                "principale (vacuum)"));
            CREDO_FALSUM (_continet(piscina, princeps->contentum,
                "punctum_summa (constans Punctum* p)\n{"));
            {
                chorda c   = princeps->contentum;
                   i32 f2  = c.mensura;

                dum (   f2 > ZEPHYRUM
                     && ((character)c.datum[f2 - I] == '\n'
                    || (character)c.datum[f2 - I] == ' '))
                {
                    f2 = f2 - I;
                }
                CREDO_VERUM (f2 > ZEPHYRUM
                    && (character)c.datum[f2 - I] == '}');
            }
        }
        briar_silvam_solvere(nexus);
    }

    imprimere("\n--- Probans salve_vitreum: principale genitum ---\n");
    {
                        Xar* nexus;
        BriarFabricaFructus  f = _fabricare(piscina, intern, fons,
            "briar/probationes/fixa/thistle/salve_vitreum.thistle",
            &nexus);
        constans BriarPlagula* aed;
        constans BriarPlagula* prob;
        constans BriarPlagula* princeps;

        si (!f.successus)
        {
            imprimere("  RECUSATIO: %.*s\n", (integer)f.causa.mensura,
                (constans character*)f.causa.datum);
        }
        CREDO_VERUM (f.successus);
        CREDO_AEQUALIS_S32 ((s32)f.forma, (s32)BRIAR_FORMA_VITREA);
        _aurum_conferre(piscina, &f, "fontes/salve_vitreum.c");
        _aurum_conferre(piscina, &f, "fontes/salve_vitreum_regiones.c");
        _aurum_conferre(piscina, &f,
            "include/salve_vitreum_regiones.h");
        _aurum_conferre(piscina, &f,
            "probationes/probatio_salve_vitreum.c");
        _aurum_conferre(piscina, &f, "assets/index.html");
        _aurum_conferre(piscina, &f, "assets/salve_vitreum.toml");
        princeps  = _genita(&f, "fontes/salve_vitreum.c");
        aed       = _genita(&f, "aedificare.sh");
        prob      = _genita(&f, "probare.sh");
        CREDO_NON_NIHIL (princeps);
        CREDO_NON_NIHIL (aed);
        CREDO_NON_NIHIL (prob);
        si (princeps == NIHIL || aed == NIHIL || prob == NIHIL)
        {
            CREDO_CULPA ("plagulae vitreae absunt");
        }
        alioquin
        {
            CREDO_VERUM (_continet(piscina, princeps->contentum,
                "figura.titulus  = \"salve\";"));
            CREDO_VERUM (_continet(piscina, princeps->contentum,
                "figura.latitudo = 640;"));
            CREDO_VERUM (_continet(piscina, princeps->contentum,
                "internuntius_praebere(atrium_internuntius(atrium),\n"
                "        \"salve\", salve, NIHIL);"));
            CREDO_VERUM (_continet(piscina, princeps->contentum,
                "atrium_vexilla_legere(&figura, argc, argv);"));
            /* ordines vitrei: listae computatae (corpus-dependentes -
             * non aurum) */
            CREDO_VERUM (_continet(piscina, aed->contentum,
                "fontes/salve_vitreum.c fontes/salve_vitreum_regiones.c"
                " \\\n    assets/capsula_salve_vitreum.c"));
            CREDO_VERUM (_continet(piscina, aed->contentum,
                "-framework Cocoa"));
            CREDO_VERUM (_continet(piscina, aed->contentum,
                "(novum -vitrea, probatio)"));
            CREDO_VERUM (_continet(piscina, prob->contentum,
                "probationes/probatio_salve_vitreum.c"
                " fontes/salve_vitreum_regiones.c"));
        }
        CREDO_NON_NIHIL (_genita(&f, "instrumenta/capsula_generare.c"));
        CREDO_VERUM (_clausura_habet(&f, "include/atrium.h"));
        CREDO_VERUM (_clausura_habet(&f, "include/json.h"));
        CREDO_VERUM (_clausura_habet(&f, "lib/credo.c"));
        /* sine statu nullum volumen, ergo nullus sqlite: clausura app
         * vitreae LXXI plagulae, vendor/ vacuum (mensuratum
         * 2026-09-05) */
        CREDO_FALSUM (_clausura_habet(&f, "vendor/sqlite3.c"));
        CREDO_VERUM (_clausura_habet(&f, "lib/internuntius.c"));
        CREDO_AEQUALIS_I32 (xar_numerus(f.genitae), (i32)9);
        briar_silvam_solvere(nexus);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
