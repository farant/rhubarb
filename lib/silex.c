/* silex.c - proiecta nova e fabrica excudere (vide silex.h) */

#include "silex.h"
#include "volumen.h"
#include "filum.h"
#include "via.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "processus.h"
#include "iter_directoria.h"
#include "sigillum.h"
#include "json.h"

#include <stdio.h>
#include <string.h>

/* ==================================================
 * Auxilia
 * ================================================== */

interior constans character*
_texere (Piscina* piscina, constans character* a,
    constans character* b, constans character* c);

interior constans character*
_texere (Piscina* piscina, constans character* a,
    constans character* b, constans character* c)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)128);

    chorda_aedificator_appendere_literis(aed, a);
    si (b != NIHIL)
    {
        chorda_aedificator_appendere_literis(aed, b);
    }
    si (c != NIHIL)
    {
        chorda_aedificator_appendere_literis(aed, c);
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}

interior chorda
_praefigere (Piscina* piscina, constans character* praefixum,
    chorda cauda);

interior chorda
_praefigere (Piscina* piscina, constans character* praefixum,
    chorda cauda)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)128);

    chorda_aedificator_appendere_literis(aed, praefixum);
    chorda_aedificator_appendere_chorda(aed, cauda);
    redde chorda_aedificator_finire(aed);
}

/* ascensus ex initio: signum fabricae = include/latina.h (plagula)
 * + lib/ (directorium). via_directorium("/") = "/" - mensura
 * immota terminat; custodia graduum contra vias pathologicas. */
constans character*
silex_fabricam_invenire (Piscina* piscina,
    constans character* initium)
{
    chorda via;
    i32    gradus;

    via = via_absoluta(chorda_ex_literis(initium, piscina),
        piscina);
    si (via.mensura == 0)
    {
        redde NIHIL;
    }
    per (gradus = 0; gradus < 64; gradus = gradus + 1)
    {
        constans character* radix = chorda_ut_cstr(via, piscina);
        chorda parens;

        si (filum_existit(_texere(piscina, radix,
                "/include/latina.h", NIHIL))
            && filum_directorium_existit(_texere(piscina, radix,
                "/lib", NIHIL)))
        {
            redde radix;
        }
        parens = via_directorium(via, piscina);
        si (parens.mensura == via.mensura)
        {
            frange;   /* radix "/" attacta */
        }
        via = parens;
    }
    redde NIHIL;
}

/* ==================================================
 * Fons bibliothecarum (DISCUS | CORPUS)
 * ================================================== */

SilexFons*
silex_fons_disci (Piscina* piscina, constans character* fabrica)
{
    SilexFons* fons;

    si (fabrica == NIHIL || !filum_directorium_existit(
        _texere(piscina, fabrica, "/include", NIHIL)))
    {
        redde NIHIL;
    }
    fons = (SilexFons*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilexFons));
    si (fons == NIHIL)
    {
        redde NIHIL;
    }
    fons->genus   = SILEX_FONS_DISCUS;
    fons->fabrica = fabrica;
    fons->capsula = NIHIL;
    fons->titulus = fabrica;
    redde fons;
}

SilexFons*
silex_fons_corporis (Piscina* piscina,
    constans CapsulaEmbed* embed)
{
    SilexFons* fons;
    Capsula*   capsula = capsula_aperire(embed, piscina);

    si (capsula == NIHIL)
    {
        redde NIHIL;
    }
    fons = (SilexFons*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilexFons));
    si (fons == NIHIL)
    {
        redde NIHIL;
    }
    fons->genus   = SILEX_FONS_CORPUS;
    fons->fabrica = NIHIL;
    fons->capsula = capsula;
    fons->titulus = "(corpus sine stampa)";
    {
        CapsulaFructus stampa = capsula_legere(capsula,
            "corpus.versio", piscina);

        si (stampa.status == CAPSULA_OK
            && stampa.datum.mensura > 0)
        {
            chorda linea = stampa.datum;

            /* linea prima sine terminatione */
            dum (linea.mensura > 0
                && (linea.datum[linea.mensura - 1] == '\n'
                    || linea.datum[linea.mensura - 1] == '\r'))
            {
                linea.mensura = linea.mensura - 1;
            }
            fons->titulus = _texere(piscina, "(corpus ",
                chorda_ut_cstr(linea, piscina), ")");
        }
    }
    redde fons;
}

b32
silex_fons_existit (constans SilexFons* fons,
    constans character* via_relativa, Piscina* piscina)
{
    si (fons->genus == SILEX_FONS_CORPUS)
    {
        redde capsula_habet(fons->capsula, via_relativa);
    }
    redde filum_existit(_texere(piscina, fons->fabrica, "/",
        via_relativa));
}

chorda
silex_fons_legere (constans SilexFons* fons,
    constans character* via_relativa, Piscina* piscina,
    b32* inventum)
{
    chorda vacua;

    vacua.datum = NIHIL;
    vacua.mensura = 0;
    *inventum = FALSUM;
    si (fons->genus == SILEX_FONS_CORPUS)
    {
        CapsulaFructus f = capsula_legere(fons->capsula,
            via_relativa, piscina);

        si (f.status != CAPSULA_OK)
        {
            redde vacua;
        }
        *inventum = VERUM;
        redde f.datum;
    }
    {
        constans character* via_plena = _texere(piscina,
            fons->fabrica, "/", via_relativa);

        si (!filum_existit(via_plena))
        {
            redde vacua;
        }
        *inventum = VERUM;
        redde filum_legere_totum(via_plena, piscina);
    }
}

/* ==================================================
 * Scrutatio inclusionum (v0 textualis, lineatim)
 * ================================================== */

interior vacuum
_inclusiones_scrutari (chorda contentum, Xar* opus);

interior vacuum
_inclusiones_scrutari (chorda contentum, Xar* opus)
{
    i32 i = 0;

    dum (i < contentum.mensura)
    {
        i32 p = i;

        dum (p < contentum.mensura
            && (contentum.datum[p] == ' '
                || contentum.datum[p] == '\t'))
        {
            p = p + 1;
        }
        si (p < contentum.mensura && contentum.datum[p] == '#')
        {
            p = p + 1;
            dum (p < contentum.mensura
                && (contentum.datum[p] == ' '
                    || contentum.datum[p] == '\t'))
            {
                p = p + 1;
            }
            si (p + 7 <= contentum.mensura
                && memcmp(contentum.datum + p, "include", 7) == 0)
            {
                p = p + 7;
                dum (p < contentum.mensura
                    && (contentum.datum[p] == ' '
                        || contentum.datum[p] == '\t'))
                {
                    p = p + 1;
                }
                si (p < contentum.mensura
                    && contentum.datum[p] == '"')
                {
                    i32 initium;

                    p = p + 1;
                    initium = p;
                    dum (p < contentum.mensura
                        && contentum.datum[p] != '"'
                        && contentum.datum[p] != '\n')
                    {
                        p = p + 1;
                    }
                    si (p < contentum.mensura
                        && contentum.datum[p] == '"'
                        && p - initium >= 2
                        && contentum.datum[p - 2] == '.'
                        && contentum.datum[p - 1] == 'h')
                    {
                        chorda* cellula = (chorda*)xar_addere(opus);

                        si (cellula != NIHIL)
                        {
                            *cellula = chorda_ex_buffer(
                                contentum.datum + initium,
                                p - initium);
                        }
                    }
                }
            }
        }
        dum (i < contentum.mensura && contentum.datum[i] != '\n')
        {
            i = i + 1;
        }
        i = i + 1;
    }
}

/* ==================================================
 * Clausura bibliothecarum
 * ================================================== */

interior b32
_vendor_basim (chorda caput, chorda* basis);

/* 'vendor/' in nomine (quocumque praefixo, e.g. '../vendor/x.h')
 * -> basis post eam, si sine '/' ulteriore. Normalizatio: claves
 * corporis '..' resolvere nequeunt, ergo par venditorium semper
 * 'vendor/<basis>' fit. */
interior b32
_vendor_basim (chorda caput, chorda* basis)
{
    i32 i;

    per (i = 0; i + 7 <= caput.mensura; i = i + 1)
    {
        si (memcmp(caput.datum + i, "vendor/", 7) == 0)
        {
            chorda cauda = chorda_ex_buffer(caput.datum + i + 7,
                caput.mensura - i - 7);
            i32 j;

            per (j = 0; j < cauda.mensura; j = j + 1)
            {
                si (cauda.datum[j] == '/')
                {
                    redde FALSUM;
                }
            }
            *basis = cauda;
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_plagulam_e_fonte_colligere (Piscina* piscina,
    constans SilexFons* fons, constans character* pars,
    chorda titulus, Xar* fructus, Xar* opus);

/* pars = "include/" | "lib/" | "vendor/"; VERUM = plagula in fonte
 * exsistit et lecta. opus NIHIL = folium (venditoria: contenta
 * eorum numquam scrutantur - sqlite3.c windows.h citat!) */
interior b32
_plagulam_e_fonte_colligere (Piscina* piscina,
    constans SilexFons* fons, constans character* pars,
    chorda titulus, Xar* fructus, Xar* opus)
{
    chorda              via_rel;
    constans character* via_rel_cstr;
    chorda              contentum;
    SilexRes*           res;

    via_rel = _praefigere(piscina, pars, titulus);
    via_rel_cstr = chorda_ut_cstr(via_rel, piscina);
    si (!silex_fons_existit(fons, via_rel_cstr, piscina))
    {
        redde FALSUM;
    }
    {
        b32 inventum = FALSUM;

        contentum = silex_fons_legere(fons, via_rel_cstr, piscina,
            &inventum);
        si (!inventum)
        {
            redde FALSUM;
        }
    }
    res = (SilexRes*)xar_addere(fructus);
    si (res == NIHIL)
    {
        redde FALSUM;
    }
    res->via = via_rel;
    res->contentum = contentum;
    res->origo = _texere(piscina, "vendicata:", via_rel_cstr, NIHIL);
    si (opus != NIHIL)
    {
        _inclusiones_scrutari(contentum, opus);
    }
    redde VERUM;
}

Xar*
silex_clausuram_colligere (Piscina* piscina,
    constans SilexFons* fons,
    constans character* constans* semina, i32 numerus_seminum)
{
    Xar*            opus;
    Xar*            fructus;
    TabulaDispersa* visa;
    i32             s;
    i32             index;

    si (fons == NIHIL)
    {
        redde NIHIL;   /* validatio in constructoribus fontis */
    }

    opus = xar_creare(piscina, (i32)magnitudo(chorda));
    fructus = xar_creare(piscina, (i32)magnitudo(SilexRes));
    visa = tabula_dispersa_creare_chorda(piscina, 64);
    si (opus == NIHIL || fructus == NIHIL || visa == NIHIL)
    {
        redde NIHIL;
    }

    per (s = 0; s < numerus_seminum; s = s + 1)
    {
        chorda* cellula = (chorda*)xar_addere(opus);

        si (cellula == NIHIL)
        {
            redde NIHIL;
        }
        *cellula = chorda_ex_literis(semina[s], piscina);
    }

    per (index = 0; index < xar_numerus(opus); index = index + 1)
    {
        chorda caput = *(chorda*)xar_obtinere(opus, index);
        chorda basis_vendoris;

        si (_vendor_basim(caput, &basis_vendoris))
        {
            chorda caput_normale = _praefigere(piscina, "vendor/",
                basis_vendoris);

            si (tabula_dispersa_continet(visa, caput_normale))
            {
                perge;
            }
            tabula_dispersa_inserere(visa, caput_normale,
                (vacuum*)fructus);
            /* venditoria FOLIA sunt (opus NIHIL): contenta eorum
             * inclusiones proprias citant (windows.h...) quae
             * clausuram domus non sunt */
            si (!_plagulam_e_fonte_colligere(piscina, fons,
                "vendor/", basis_vendoris, fructus, NIHIL))
            {
                fprintf(stderr, "silex: monitio - venditorium"
                    " citatum in fonte deest: %.*s\n",
                    (integer)basis_vendoris.mensura,
                    (constans character*)basis_vendoris.datum);
            }
            /* geminus .c si basis '.h' terminatur */
            si (basis_vendoris.mensura > 2
                && basis_vendoris.datum[basis_vendoris.mensura - 2]
                    == '.'
                && basis_vendoris.datum[basis_vendoris.mensura - 1]
                    == 'h')
            {
                chorda basis_c = _praefigere(piscina, "",
                    basis_vendoris);

                basis_c.datum[basis_c.mensura - 1] = 'c';
                (vacuum)_plagulam_e_fonte_colligere(piscina, fons,
                    "vendor/", basis_c, fructus, NIHIL);
            }
            perge;
        }

        si (tabula_dispersa_continet(visa, caput))
        {
            perge;
        }
        tabula_dispersa_inserere(visa, caput, (vacuum*)fructus);

        si (!_plagulam_e_fonte_colligere(piscina, fons,
            "include/", caput, fructus, opus))
        {
            /* citata sine fonte: monitio, non mors (commentaria
             * falso positiva possunt; dependentiae verae in
             * fabrica semper exsistunt) */
            fprintf(stderr,
                "silex: monitio - caput citatum in fabrica deest:"
                " %.*s\n", (integer)caput.mensura,
                (constans character*)caput.datum);
            perge;
        }

        /* geminus lib: conventione aedilis include/X.h -> lib/X.c */
        si (caput.mensura >= 2)
        {
            chorda  radix_capitis;
            chorda  titulus_c;

            radix_capitis = chorda_ex_buffer(caput.datum,
                caput.mensura - 2);
            {
                ChordaAedificator* aed = chorda_aedificator_creare(
                    piscina, (memoriae_index)64);

                chorda_aedificator_appendere_chorda(aed,
                    radix_capitis);
                chorda_aedificator_appendere_literis(aed, ".c");
                titulus_c = chorda_aedificator_finire(aed);
            }
            si (!_plagulam_e_fonte_colligere(piscina, fons,
                "lib/", titulus_c, fructus, opus))
            {
                /* geminus obiectivus: conventione aedilis
                 * lib/X_macos.m ubi lib/X.c abest */
                ChordaAedificator* aed_m =
                    chorda_aedificator_creare(piscina,
                        (memoriae_index)64);

                chorda_aedificator_appendere_chorda(aed_m,
                    radix_capitis);
                chorda_aedificator_appendere_literis(aed_m,
                    "_macos.m");
                (vacuum)_plagulam_e_fonte_colligere(piscina, fons,
                    "lib/", chorda_aedificator_finire(aed_m),
                    fructus, opus);
            }
        }
    }

    redde fructus;
}

/* ==================================================
 * Plagulae genitae (scaffold)
 * ================================================== */

interior chorda
_fontem_fingere (Piscina* piscina, constans character* titulus);

interior chorda
_fontem_fingere (Piscina* piscina, constans character* titulus)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)1024);

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        ".c - salutatio prima (a silice genita) */\n"
        "\n"
        "#include \"latina.h\"\n"
        "#include \"piscina.h\"\n"
        "#include \"chorda.h\"\n"
        "\n"
        "#include <stdio.h>\n"
        "\n"
        "s32\n"
        "principale (vacuum)\n"
        "{\n"
        "    Piscina* piscina;\n"
        "    chorda   salutatio;\n"
        "\n"
        "    piscina = piscina_generare_dynamicum(\"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "\", 4096);\n"
        "    si (piscina == NIHIL)\n"
        "    {\n"
        "        redde I;\n"
        "    }\n"
        "    salutatio = chorda_ex_literis(\"salve, munde - hic ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "\", piscina);\n"
        "    imprimere(\"%.*s\\n\", (integer)salutatio.mensura,\n"
        "        (constans character*)salutatio.datum);\n"
        "    piscina_destruere(piscina);\n"
        "    redde ZEPHYRUM;\n"
        "}\n");
    redde chorda_aedificator_finire(a);
}

interior chorda
_probationem_fingere (Piscina* piscina, constans character* titulus);

interior chorda
_probationem_fingere (Piscina* piscina, constans character* titulus)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)1024);

    chorda_aedificator_appendere_literis(a, "/* probatio_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        ".c - probationes primae (a silice genitae) */\n"
        "\n"
        "#include \"latina.h\"\n"
        "#include \"piscina.h\"\n"
        "#include \"chorda.h\"\n"
        "#include \"credo.h\"\n"
        "\n"
        "#include <stdio.h>\n"
        "\n"
        "s32\n"
        "principale (vacuum)\n"
        "{\n"
        "    b32      praeteritus;\n"
        "    Piscina* piscina;\n"
        "\n"
        "    piscina = piscina_generare_dynamicum(\"probatio_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "\", 4096);\n"
        "    si (piscina == NIHIL)\n"
        "    {\n"
        "        imprimere(\"FRACTA: piscina_generatio\\n\");\n"
        "        redde I;\n"
        "    }\n"
        "    credo_aperire(piscina);\n"
        "\n"
        "    {\n"
        "        chorda titulus;\n"
        "\n"
        "        imprimere(\"\\n--- Probans fundamenta ---\\n\");\n"
        "        titulus = chorda_ex_literis(\"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "\", piscina);\n"
        "        CREDO_AEQUALIS_I32((i32)titulus.mensura, (i32)");
    chorda_aedificator_appendere_i32(a,
        (i32)strlen(titulus));
    chorda_aedificator_appendere_literis(a,
        ");\n"
        "        CREDO_NON_NIHIL(titulus.datum);\n"
        "    }\n"
        "\n"
        "    imprimere(\"\\n\");\n"
        "    credo_imprimere_compendium();\n"
        "    praeteritus = credo_omnia_praeterierunt();\n"
        "    credo_claudere();\n"
        "    piscina_destruere(piscina);\n"
        "    si (praeteritus)\n"
        "    {\n"
        "        redde ZEPHYRUM;\n"
        "    }\n"
        "    redde I;\n"
        "}\n");
    redde chorda_aedificator_finire(a);
}

#define SILEX_VEXILLA_COMPILATIONIS \
    "-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion" \
    " -Wsign-conversion -Wcast-qual -Wstrict-prototypes" \
    " -Wmissing-prototypes -Wwrite-strings -Wno-long-long"

interior chorda
_aedificare_sh_fingere (Piscina* piscina,
    constans character* titulus);

interior chorda
_aedificare_sh_fingere (Piscina* piscina,
    constans character* titulus)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)512);

    chorda_aedificator_appendere_literis(a,
        "#!/bin/sh\n"
        "# aedificare.sh - ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        " aedificare (a silice genitum)\n"
        "set -e\n"
        "cd \"$(dirname \"$0\")\"\n"
        "mkdir -p bin\n"
        "clang " SILEX_VEXILLA_COMPILATIONIS
        " -Iinclude \"fontes/");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".c\" lib/*.c -o \"bin/");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "\"\n"
        "echo \"aedificatum: bin/");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "\"\n");
    redde chorda_aedificator_finire(a);
}

interior chorda
_probare_sh_fingere (Piscina* piscina, constans character* titulus);

interior chorda
_probare_sh_fingere (Piscina* piscina, constans character* titulus)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)512);

    chorda_aedificator_appendere_literis(a,
        "#!/bin/sh\n"
        "# probare.sh - probationes ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        " currere (a silice genitum)\n"
        "set -e\n"
        "cd \"$(dirname \"$0\")\"\n"
        "mkdir -p bin\n"
        "clang " SILEX_VEXILLA_COMPILATIONIS
        " -Iinclude \"probationes/probatio_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        ".c\" lib/*.c -o \"bin/probatio_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "\"\n"
        "exec \"bin/probatio_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "\"\n");
    redde chorda_aedificator_finire(a);
}

interior chorda
_readme_fingere (Piscina* piscina, constans character* titulus);

interior chorda
_readme_fingere (Piscina* piscina, constans character* titulus)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)512);

    chorda_aedificator_appendere_literis(a, "# ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "\n\n"
        "Proiectum a silice excusum (v0). Veritas in `");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        ".volumen` vivit;\n"
        "arbor plagularum = proiectio eius.\n"
        "\n"
        "- `./aedificare.sh` - aedificare (bin/");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        ")\n"
        "- `./probare.sh` - probationes currere\n"
        "\n"
        "Bibliothecae in `lib/` + `include/` e fabrica vendicatae -\n"
        "TUAE sunt: nulla dependentia, nullum rete, nulla tabes.\n");
    redde chorda_aedificator_finire(a);
}

/* ==================================================
 * Novum
 * ================================================== */

interior vacuum
_rem_genitam_addere (Piscina* piscina, Xar* res_omnes,
    constans character* via, chorda contentum);

interior vacuum
_rem_genitam_addere (Piscina* piscina, Xar* res_omnes,
    constans character* via, chorda contentum)
{
    SilexRes* res = (SilexRes*)xar_addere(res_omnes);

    si (res == NIHIL)
    {
        redde;
    }
    res->via = chorda_ex_literis(via, piscina);
    res->contentum = contentum;
    res->origo = "genita";
}

SilexNovumFructus
silex_novum (Piscina* piscina, constans SilexNovumOptiones* optiones)
{
    SilexNovumFructus   fructus;
    Xar*                res_omnes;
    constans character* proiectum_dir;
    constans character* volumen_via;
    Volumen*            vol;
    i32                 index;
    interior constans character* constans SEMINA[] = {
        "latina.h", "piscina.h", "chorda.h", "credo.h"
    };

    fructus.successus = FALSUM;
    fructus.vendicatae = 0;
    fructus.genitae = 0;
    fructus.volumen_via = "";
    fructus.erratum = NIHIL;

    /* titulus: sine '/' et '.' (via fit) */
    {
        constans character* t = optiones->titulus;

        si (t == NIHIL || t[0] == '\0')
        {
            fructus.erratum = "titulus vacuus";
            redde fructus;
        }
        dum (*t != '\0')
        {
            si (*t == '/' || *t == '.')
            {
                fructus.erratum =
                    "titulus characteres '/' aut '.' fert";
                redde fructus;
            }
            t = t + 1;
        }
    }

    proiectum_dir = _texere(piscina, optiones->destinatio, "/",
        optiones->titulus);
    si (filum_directorium_existit(proiectum_dir)
        || filum_existit(proiectum_dir))
    {
        fructus.erratum = _texere(piscina,
            "destinatio iam exsistit: ", proiectum_dir, NIHIL);
        redde fructus;
    }

    /* clausura e fabrica */
    res_omnes = silex_clausuram_colligere(piscina, optiones->fons,
        SEMINA, (i32)(magnitudo(SEMINA) / magnitudo(SEMINA[0])));
    si (res_omnes == NIHIL)
    {
        fructus.erratum = "clausura bibliothecarum fracta";
        redde fructus;
    }
    fructus.vendicatae = xar_numerus(res_omnes);

    /* plagulae genitae */
    _rem_genitam_addere(piscina, res_omnes,
        _texere(piscina, "fontes/", optiones->titulus, ".c"),
        _fontem_fingere(piscina, optiones->titulus));
    _rem_genitam_addere(piscina, res_omnes,
        _texere(piscina, "probationes/probatio_", optiones->titulus,
            ".c"),
        _probationem_fingere(piscina, optiones->titulus));
    _rem_genitam_addere(piscina, res_omnes, "aedificare.sh",
        _aedificare_sh_fingere(piscina, optiones->titulus));
    _rem_genitam_addere(piscina, res_omnes, "probare.sh",
        _probare_sh_fingere(piscina, optiones->titulus));
    _rem_genitam_addere(piscina, res_omnes, "README.md",
        _readme_fingere(piscina, optiones->titulus));
    fructus.genitae = xar_numerus(res_omnes) - fructus.vendicatae;

    /* volumen primum (veritas), deinde proiectio */
    si (!filum_directorium_creare_si_necesse(proiectum_dir))
    {
        fructus.erratum = _texere(piscina,
            "directorium creari non potuit: ", proiectum_dir, NIHIL);
        redde fructus;
    }
    volumen_via = _texere(piscina,
        _texere(piscina, proiectum_dir, "/", optiones->titulus),
        ".volumen", NIHIL);
    vol = volumen_creare(piscina, volumen_via);
    si (vol == NIHIL)
    {
        fructus.erratum = "volumen creari non potuit";
        redde fructus;
    }
    per (index = 0; index < xar_numerus(res_omnes);
        index = index + 1)
    {
        SilexRes* res = (SilexRes*)xar_obtinere(res_omnes, index);

        si (!volumen_plagulam_condere(vol, res->via, res->contentum,
            res->origo))
        {
            fructus.erratum = _texere(piscina,
                "plagula condi non potuit: ",
                chorda_ut_cstr(res->via, piscina), NIHIL);
            volumen_claudere(vol);
            redde fructus;
        }
    }

    /* conditio ortus: punctum primum nominatum in cauda - verba
     * VCS (historia/proicere) proiectum recens iam vident */
    {
        ChordaAedificator* aed = chorda_aedificator_creare(piscina,
            (memoriae_index)128);
        chorda datum;

        chorda_aedificator_appendere_literis(aed,
            "{\"nuntius\":\"proiectum excusum (novum)\","
            "\"conditae\":");
        chorda_aedificator_appendere_s32(aed,
            (s32)xar_numerus(res_omnes));
        chorda_aedificator_appendere_literis(aed, ",\"remotae\":0}");
        datum = chorda_aedificator_finire(aed);
        si (volumen_actum_appendere(vol, "conditio", datum) == 0)
        {
            fructus.erratum = "conditio ortus scribi non potuit";
            volumen_claudere(vol);
            redde fructus;
        }
    }

    /* proiectio: plagulae ex VOLUMINE promuntur (via promendi ab
     * ortu probata), non e memoria */
    {
        Xar* manifestum = volumen_plagulas_enumerare(vol, piscina);

        si (manifestum == NIHIL)
        {
            fructus.erratum = "manifestum legi non potuit";
            volumen_claudere(vol);
            redde fructus;
        }
        per (index = 0; index < xar_numerus(manifestum);
            index = index + 1)
        {
            VolumenPlagula* p = (VolumenPlagula*)xar_obtinere(
                manifestum, index);
            b32    inventum;
            chorda contentum = volumen_plagulam_promere(vol, p->via,
                piscina, &inventum);
            constans character* via_plena;
            chorda parens;

            si (!inventum)
            {
                fructus.erratum = "plagula in manifesto sine massa";
                volumen_claudere(vol);
                redde fructus;
            }
            via_plena = _texere(piscina, proiectum_dir, "/",
                chorda_ut_cstr(p->via, piscina));
            parens = via_directorium(
                chorda_ex_literis(via_plena, piscina), piscina);
            filum_directorium_creare_si_necesse(
                chorda_ut_cstr(parens, piscina));
            si (!filum_scribere(via_plena, contentum))
            {
                fructus.erratum = _texere(piscina,
                    "proiectio fracta: ", via_plena, NIHIL);
                volumen_claudere(vol);
                redde fructus;
            }
        }
    }
    volumen_claudere(vol);

    /* scripta exsecutabilia (defectus non fatalis - 'sh x.sh'
     * semper manet) */
    {
        constans character* argumenta_chmod[5];
        ProcessusResultus   r;

        argumenta_chmod[0] = "/bin/chmod";
        argumenta_chmod[1] = "+x";
        argumenta_chmod[2] = _texere(piscina, proiectum_dir,
            "/aedificare.sh", NIHIL);
        argumenta_chmod[3] = _texere(piscina, proiectum_dir,
            "/probare.sh", NIHIL);
        argumenta_chmod[4] = NIHIL;
        r = processus_exsequi(argumenta_chmod, 5000, piscina);
        si (!r.successus || r.codex_exitus != 0)
        {
            fprintf(stderr,
                "silex: monitio - chmod +x fractum (scripta per"
                " 'sh' curre)\n");
        }
    }

    fructus.successus = VERUM;
    fructus.volumen_via = volumen_via;
    redde fructus;
}

/* ==================================================
 * VCS: status / condere / historia
 * ================================================== */

interior b32
_praefixum_habet (chorda via, constans character* praefixum);

interior b32
_praefixum_habet (chorda via, constans character* praefixum)
{
    memoriae_index m = strlen(praefixum);

    redde via.mensura >= (i32)m
        && memcmp(via.datum, praefixum, m) == 0;
}

interior b32
_suffixum_habet (chorda via, constans character* suffixum);

interior b32
_suffixum_habet (chorda via, constans character* suffixum)
{
    memoriae_index m = strlen(suffixum);

    redde via.mensura >= (i32)m
        && memcmp(via.datum + via.mensura - (i32)m, suffixum, m)
            == 0;
}

/* quae vias VCS numquam videt (occulta iam filtro exclusa) */
interior b32
_praetermittenda (chorda via_rel);

interior b32
_praetermittenda (chorda via_rel)
{
    redde _praefixum_habet(via_rel, "bin/")
        || _praefixum_habet(via_rel, "build/")
        || _suffixum_habet(via_rel, ".volumen")
        || _suffixum_habet(via_rel, ".volumen-wal")
        || _suffixum_habet(via_rel, ".volumen-shm");
}

/* via voluminis proiecti (publica - vide silex.h). DECISUM (Fran,
 * red-team IX 2026-08-04): conventio <dir>/<nomen(dir)>.volumen via
 * laeta manet; ea absente, *.volumen SOLITARIUM in directorio vincit
 * (cp/renominatio sanata - plagula EST documentum); plura =
 * recusatio clara nominans. */
constans character*
silex_volumen_viam_invenire (Piscina* piscina,
    constans character* proiectum_dir)
{
    chorda absoluta = via_absoluta(
        chorda_ex_literis(proiectum_dir, piscina), piscina);
    chorda titulus = via_nomen(absoluta, piscina);
    constans character* absoluta_cstr = chorda_ut_cstr(absoluta,
        piscina);
    constans character* via;

    /* I. conventio */
    {
        ChordaAedificator* aed = chorda_aedificator_creare(piscina,
            (memoriae_index)128);

        chorda_aedificator_appendere_chorda(aed, absoluta);
        chorda_aedificator_appendere_literis(aed, "/");
        chorda_aedificator_appendere_chorda(aed, titulus);
        chorda_aedificator_appendere_literis(aed, ".volumen");
        via = chorda_ut_cstr(chorda_aedificator_finire(aed),
            piscina);
        si (filum_existit(via))
        {
            redde via;
        }
    }

    /* II. *.volumen solitarium */
    {
        DirectoriumIterator*  iter;
        DirectoriumIntroitus* e;
        constans character*   inventum = NIHIL;
        i32                   numerus = 0;

        iter = directorium_iterator_aperire(absoluta_cstr, piscina);
        si (iter == NIHIL)
        {
            redde NIHIL;
        }
        dum ((e = directorium_iterator_proximum(iter)) != NIHIL)
        {
            si (e->genus != INTROITUS_FILUM
                || !_suffixum_habet(e->titulus, ".volumen"))
            {
                perge;
            }
            numerus = numerus + 1;
            {
                ChordaAedificator* aed = chorda_aedificator_creare(
                    piscina, (memoriae_index)128);

                chorda_aedificator_appendere_chorda(aed, absoluta);
                chorda_aedificator_appendere_literis(aed, "/");
                chorda_aedificator_appendere_chorda(aed,
                    e->titulus);
                inventum = chorda_ut_cstr(
                    chorda_aedificator_finire(aed), piscina);
            }
        }
        directorium_iterator_claudere(iter);
        si (numerus == 1)
        {
            redde inventum;
        }
        si (numerus > 1)
        {
            fprintf(stderr, "silex: volumina %d in directorio -"
                " ambiguum; unum relinque aut conventionem"
                " <dir>/<dir>.volumen sequere\n", (integer)numerus);
        }
    }
    redde NIHIL;
}

nomen structura {
    Piscina*        piscina;
    TabulaDispersa* manifestum;   /* via -> chorda* (sigillum hex) */
    TabulaDispersa* visae;
    Xar*            res;
    i32             mundae;
    i32             radix_mensura;   /* praefixum viae plenae */
} StatusContextus;

interior s32
_status_ambulator (chorda via_plena,
    constans DirectoriumIntroitus* introitus, vacuum* contextus);

interior s32
_status_ambulator (chorda via_plena,
    constans DirectoriumIntroitus* introitus, vacuum* contextus)
{
    StatusContextus* ctx = (StatusContextus*)contextus;
    chorda           via_rel;
    chorda           contentum;
    Sigillum         sig;
    character        hex[SIGILLUM_HEX_MENSURA];
    vacuum*          conditum = NIHIL;

    si (introitus->genus != INTROITUS_FILUM)
    {
        redde 0;
    }
    si (via_plena.mensura <= ctx->radix_mensura)
    {
        redde 0;
    }
    via_rel = chorda_ex_buffer(via_plena.datum + ctx->radix_mensura,
        via_plena.mensura - ctx->radix_mensura);
    si (_praetermittenda(via_rel))
    {
        redde 0;
    }
    contentum = filum_legere_totum(
        chorda_ut_cstr(via_plena, ctx->piscina), ctx->piscina);
    sig = sigillum_computare((constans vacuum*)contentum.datum,
        (memoriae_index)contentum.mensura);
    sigillum_hex(&sig, hex);

    si (tabula_dispersa_invenire(ctx->manifestum, via_rel,
        &conditum))
    {
        chorda* sigillum_conditum = (chorda*)conditum;

        tabula_dispersa_inserere(ctx->visae, via_rel,
            (vacuum*)ctx);
        si (sigillum_conditum->mensura == 64
            && memcmp(sigillum_conditum->datum, hex, 64) == 0)
        {
            ctx->mundae = ctx->mundae + 1;
        }
        alioquin
        {
            SilexStatusRes* r = (SilexStatusRes*)xar_addere(
                ctx->res);

            si (r != NIHIL)
            {
                r->via = via_rel;
                r->status = SILEX_PLAGULA_MUTATA;
            }
        }
    }
    alioquin
    {
        SilexStatusRes* r = (SilexStatusRes*)xar_addere(ctx->res);

        si (r != NIHIL)
        {
            r->via = via_rel;
            r->status = SILEX_PLAGULA_NOVA;
        }
    }
    redde 0;
}

SilexStatusFructus
silex_status (Piscina* piscina, constans character* proiectum_dir)
{
    SilexStatusFructus  fructus;
    constans character* volumen_via;
    Volumen*            vol;
    Xar*                manifestum_ordo;
    StatusContextus     ctx;
    DirectoriumFiltrum  filtrum;
    chorda              radix_absoluta;
    i32                 index;

    fructus.successus = FALSUM;
    fructus.mundae = 0;
    fructus.res = NIHIL;
    fructus.erratum = NIHIL;

    volumen_via = silex_volumen_viam_invenire(piscina, proiectum_dir);
    si (volumen_via == NIHIL)
    {
        fructus.erratum = "volumen deest - estne proiectum silicis?";
        redde fructus;
    }
    vol = volumen_aperire(piscina, volumen_via);
    si (vol == NIHIL)
    {
        fructus.erratum = "volumen aperiri non potuit";
        redde fructus;
    }
    manifestum_ordo = volumen_plagulas_enumerare(vol, piscina);
    si (manifestum_ordo == NIHIL)
    {
        volumen_claudere(vol);
        fructus.erratum = "manifestum legi non potuit";
        redde fructus;
    }

    ctx.piscina = piscina;
    ctx.manifestum = tabula_dispersa_creare_chorda(piscina, 128);
    ctx.visae = tabula_dispersa_creare_chorda(piscina, 128);
    ctx.res = xar_creare(piscina, (i32)magnitudo(SilexStatusRes));
    ctx.mundae = 0;
    per (index = 0; index < xar_numerus(manifestum_ordo);
        index = index + 1)
    {
        VolumenPlagula* p = (VolumenPlagula*)xar_obtinere(
            manifestum_ordo, index);
        chorda* cella = (chorda*)piscina_allocare(piscina,
            (memoriae_index)magnitudo(chorda));

        si (cella == NIHIL)
        {
            volumen_claudere(vol);
            fructus.erratum = "memoria defecit";
            redde fructus;
        }
        *cella = p->sigillum_hex;
        tabula_dispersa_inserere(ctx.manifestum, p->via,
            (vacuum*)cella);
    }

    radix_absoluta = via_absoluta(
        chorda_ex_literis(proiectum_dir, piscina), piscina);
    ctx.radix_mensura = radix_absoluta.mensura + 1;   /* + '/' */

    filtrum = directorium_filtrum_omnia();
    filtrum.includere_occultos = FALSUM;
    directorium_ambulare(
        chorda_ut_cstr(radix_absoluta, piscina), &filtrum,
        _status_ambulator, &ctx, piscina);

    /* in manifesto, disco ablatae */
    per (index = 0; index < xar_numerus(manifestum_ordo);
        index = index + 1)
    {
        VolumenPlagula* p = (VolumenPlagula*)xar_obtinere(
            manifestum_ordo, index);

        si (!tabula_dispersa_continet(ctx.visae, p->via))
        {
            SilexStatusRes* r = (SilexStatusRes*)xar_addere(
                ctx.res);

            si (r != NIHIL)
            {
                r->via = p->via;
                r->status = SILEX_PLAGULA_ABSENS;
            }
        }
    }
    volumen_claudere(vol);

    fructus.successus = VERUM;
    fructus.mundae = ctx.mundae;
    fructus.res = ctx.res;
    redde fructus;
}

SilexConditioFructus
silex_condere (Piscina* piscina, constans character* proiectum_dir,
    constans character* nuntius)
{
    SilexConditioFructus fructus;
    SilexStatusFructus   status;
    constans character*  volumen_via;
    Volumen*             vol;
    chorda               radix_absoluta;
    i32                  index;

    fructus.successus = FALSUM;
    fructus.seq = 0;
    fructus.conditae = 0;
    fructus.remotae = 0;
    fructus.erratum = NIHIL;

    status = silex_status(piscina, proiectum_dir);
    si (!status.successus)
    {
        fructus.erratum = status.erratum;
        redde fructus;
    }
    si (xar_numerus(status.res) == 0)
    {
        fructus.erratum = "nihil condendum - arbor munda";
        redde fructus;
    }

    volumen_via = silex_volumen_viam_invenire(piscina, proiectum_dir);
    vol = volumen_aperire(piscina, volumen_via);
    si (vol == NIHIL)
    {
        fructus.erratum = "volumen aperiri non potuit";
        redde fructus;
    }
    radix_absoluta = via_absoluta(
        chorda_ex_literis(proiectum_dir, piscina), piscina);

    /* omnia aut nihil */
    si (!volumen_transactionem_incipere(vol))
    {
        volumen_claudere(vol);
        fructus.erratum = "transactio incipi non potuit";
        redde fructus;
    }
    per (index = 0; index < xar_numerus(status.res);
        index = index + 1)
    {
        SilexStatusRes* r = (SilexStatusRes*)xar_obtinere(
            status.res, index);
        b32 bene = VERUM;

        si (r->status == SILEX_PLAGULA_ABSENS)
        {
            bene = volumen_plagulam_removere(vol, r->via);
            si (bene)
            {
                fructus.remotae = fructus.remotae + 1;
            }
        }
        alioquin
        {
            ChordaAedificator* aed = chorda_aedificator_creare(
                piscina, (memoriae_index)256);
            chorda contentum;

            chorda_aedificator_appendere_chorda(aed,
                radix_absoluta);
            chorda_aedificator_appendere_literis(aed, "/");
            chorda_aedificator_appendere_chorda(aed, r->via);
            contentum = filum_legere_totum(chorda_ut_cstr(
                chorda_aedificator_finire(aed), piscina), piscina);
            bene = volumen_plagulam_condere(vol, r->via, contentum,
                "condita");
            si (bene)
            {
                fructus.conditae = fructus.conditae + 1;
            }
        }
        si (!bene)
        {
            volumen_transactionem_revolvere(vol);
            volumen_claudere(vol);
            fructus.erratum = "absorptio fracta - revoluta omnia";
            fructus.conditae = 0;
            fructus.remotae = 0;
            redde fructus;
        }
    }

    /* actum conditionis: punctum nominatum in cauda */
    {
        ChordaAedificator* aed = chorda_aedificator_creare(piscina,
            (memoriae_index)256);
        chorda datum;
        s64    seq;

        chorda_aedificator_appendere_literis(aed, "{\"nuntius\":\"");
        chorda_aedificator_appendere_literis_evasus_json(aed,
            nuntius == NIHIL ? "" : nuntius);
        chorda_aedificator_appendere_literis(aed,
            "\",\"conditae\":");
        chorda_aedificator_appendere_s32(aed,
            (s32)fructus.conditae);
        chorda_aedificator_appendere_literis(aed, ",\"remotae\":");
        chorda_aedificator_appendere_s32(aed, (s32)fructus.remotae);
        chorda_aedificator_appendere_literis(aed, "}");
        datum = chorda_aedificator_finire(aed);
        seq = volumen_actum_appendere(vol, "conditio", datum);
        si (seq == 0)
        {
            volumen_transactionem_revolvere(vol);
            volumen_claudere(vol);
            fructus.erratum = "conditio scribi non potuit";
            redde fructus;
        }
        fructus.seq = seq;
    }
    si (!volumen_transactionem_committere(vol))
    {
        volumen_claudere(vol);
        fructus.erratum = "transactio committi non potuit";
        redde fructus;
    }
    volumen_claudere(vol);
    fructus.successus = VERUM;
    redde fructus;
}

Xar*
silex_historia (Piscina* piscina, constans character* proiectum_dir)
{
    constans character* volumen_via;
    Volumen*            vol;
    Xar*                acta;
    Xar*                ordo;
    i32                 index;
    i32                 tactae = 0;

    volumen_via = silex_volumen_viam_invenire(piscina, proiectum_dir);
    si (volumen_via == NIHIL)
    {
        redde NIHIL;
    }
    vol = volumen_aperire(piscina, volumen_via);
    si (vol == NIHIL)
    {
        redde NIHIL;
    }
    acta = volumen_acta_legere(vol, 0, piscina);
    volumen_claudere(vol);
    si (acta == NIHIL)
    {
        redde NIHIL;
    }
    ordo = xar_creare(piscina, (i32)magnitudo(SilexConditio));
    si (ordo == NIHIL)
    {
        redde NIHIL;
    }

    per (index = 0; index < xar_numerus(acta); index = index + 1)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, index);
        b32 est_conditio = chorda_aequalis_literis(a->genus,
            "conditio");
        b32 est_ortus = chorda_aequalis_literis(a->genus,
            "volumen-creatum");

        si (chorda_aequalis_literis(a->genus, "plagula-condita")
            || chorda_aequalis_literis(a->genus, "plagula-remota"))
        {
            tactae = tactae + 1;
        }
        si (est_conditio || est_ortus)
        {
            SilexConditio* c = (SilexConditio*)xar_addere(ordo);

            si (c == NIHIL)
            {
                redde NIHIL;
            }
            c->seq = a->seq;
            c->momentum = a->momentum;
            c->tactae = tactae;
            tactae = 0;
            si (est_ortus)
            {
                c->nuntius = chorda_ex_literis("(ortus voluminis)",
                    piscina);
            }
            alioquin
            {
                JsonResultus lectum = json_legere(a->datum,
                    piscina);

                c->nuntius = chorda_ex_literis("(sine nuntio)",
                    piscina);
                si (lectum.successus
                    && json_est_objectum(lectum.radix))
                {
                    JsonValor* n = json_objectum_capere(
                        lectum.radix, "nuntius");

                    si (n != NIHIL && json_est_chorda(n))
                    {
                        c->nuntius = json_ad_chorda(n);
                    }
                }
            }
        }
    }
    redde ordo;
}

/* ==================================================
 * Proiectio: volumen arborem scribit (vide silex.h pro foedere)
 * ================================================== */

nomen structura {
    Piscina*        piscina;
    Volumen*        vol;
    TabulaDispersa* plica;   /* via -> VolumenPlagula* */
    TabulaDispersa* visae;
    Xar*            res;     /* SilexProiciendaRes */
    i32             intactae;
    i32             obices;
    i32             radix_mensura;
} ProiectioContextus;

interior s32
_proiectio_ambulator (chorda via_plena,
    constans DirectoriumIntroitus* introitus, vacuum* contextus);

interior s32
_proiectio_ambulator (chorda via_plena,
    constans DirectoriumIntroitus* introitus, vacuum* contextus)
{
    ProiectioContextus* ctx = (ProiectioContextus*)contextus;
    chorda    via_rel;
    chorda    contentum;
    Sigillum  sig;
    character hex[SIGILLUM_HEX_MENSURA];
    vacuum*   plica_v = NIHIL;

    si (introitus->genus != INTROITUS_FILUM)
    {
        redde 0;
    }
    si (via_plena.mensura <= ctx->radix_mensura)
    {
        redde 0;
    }
    via_rel = chorda_ex_buffer(via_plena.datum + ctx->radix_mensura,
        via_plena.mensura - ctx->radix_mensura);
    si (_praetermittenda(via_rel))
    {
        redde 0;
    }

    si (!tabula_dispersa_invenire(ctx->plica, via_rel, &plica_v))
    {
        /* disco, plicae ignota: numquam tangitur, sed nominatur */
        SilexProiciendaRes* r = (SilexProiciendaRes*)xar_addere(
            ctx->res);

        si (r != NIHIL)
        {
            r->via = via_rel;
            r->status = SILEX_PROICIENDA_ALIENA;
        }
        redde 0;
    }
    tabula_dispersa_inserere(ctx->visae, via_rel, (vacuum*)ctx);

    contentum = filum_legere_totum(
        chorda_ut_cstr(via_plena, ctx->piscina), ctx->piscina);
    sig = sigillum_computare((constans vacuum*)contentum.datum,
        (memoriae_index)contentum.mensura);
    sigillum_hex(&sig, hex);
    {
        VolumenPlagula* p = (VolumenPlagula*)plica_v;

        si (p->sigillum_hex.mensura == 64
            && memcmp(p->sigillum_hex.datum, hex, 64) == 0)
        {
            ctx->intactae = ctx->intactae + 1;
        }
        alioquin
        {
            /* FOEDUS: superscriptio solum si contentum disci in
             * massis alicubi residet (recuperabile per definitionem
             * - unus aspectus sigilli, pretium tabulae contentu
             * addressatae) */
            b32 conditum = FALSUM;
            SilexProiciendaRes* r;

            (vacuum)volumen_massam_promere(ctx->vol,
                chorda_ex_literis(hex, ctx->piscina), ctx->piscina,
                &conditum);
            r = (SilexProiciendaRes*)xar_addere(ctx->res);
            si (r != NIHIL)
            {
                r->via = via_rel;
                r->status = conditum ? SILEX_PROICIENDA_SCRIBENDA
                    : SILEX_PROICIENDA_OBEX;
            }
            si (!conditum)
            {
                ctx->obices = ctx->obices + 1;
            }
        }
    }
    redde 0;
}

SilexProiectioFructus
silex_proicere (Piscina* piscina, constans character* proiectum_dir,
    s64 ad_seq, b32 scribere)
{
    SilexProiectioFructus fructus;
    constans character*   volumen_via;
    Volumen*              vol;
    Xar*                  plica_ordo;
    ProiectioContextus    ctx;
    DirectoriumFiltrum    filtrum;
    chorda                radix_absoluta;
    i32                   index;

    fructus.successus = FALSUM;
    fructus.res = NIHIL;
    fructus.intactae = 0;
    fructus.obices = 0;
    fructus.scriptae = 0;
    fructus.erratum = NIHIL;

    volumen_via = silex_volumen_viam_invenire(piscina, proiectum_dir);
    si (volumen_via == NIHIL)
    {
        fructus.erratum = "volumen deest - estne proiectum silicis?";
        redde fructus;
    }
    vol = volumen_aperire(piscina, volumen_via);
    si (vol == NIHIL)
    {
        fructus.erratum = "volumen aperiri non potuit";
        redde fructus;
    }
    plica_ordo = volumen_plicam_ad(vol, ad_seq, piscina);
    si (plica_ordo == NIHIL)
    {
        volumen_claudere(vol);
        fructus.erratum = "plica legi non potuit";
        redde fructus;
    }

    ctx.piscina = piscina;
    ctx.vol = vol;
    ctx.plica = tabula_dispersa_creare_chorda(piscina, 128);
    ctx.visae = tabula_dispersa_creare_chorda(piscina, 128);
    ctx.res = xar_creare(piscina,
        (i32)magnitudo(SilexProiciendaRes));
    ctx.intactae = 0;
    ctx.obices = 0;
    si (ctx.plica == NIHIL || ctx.visae == NIHIL
        || ctx.res == NIHIL)
    {
        volumen_claudere(vol);
        fructus.erratum = "memoria defecit";
        redde fructus;
    }
    /* monstratores in xar stabiles - plica_ordo non iam crescit */
    per (index = 0; index < xar_numerus(plica_ordo);
        index = index + 1)
    {
        VolumenPlagula* p = (VolumenPlagula*)xar_obtinere(
            plica_ordo, index);

        tabula_dispersa_inserere(ctx.plica, p->via, (vacuum*)p);
    }

    radix_absoluta = via_absoluta(
        chorda_ex_literis(proiectum_dir, piscina), piscina);
    ctx.radix_mensura = radix_absoluta.mensura + 1;   /* + '/' */

    filtrum = directorium_filtrum_omnia();
    filtrum.includere_occultos = FALSUM;
    directorium_ambulare(
        chorda_ut_cstr(radix_absoluta, piscina), &filtrum,
        _proiectio_ambulator, &ctx, piscina);

    /* in plica, disco absentes -> creandae */
    per (index = 0; index < xar_numerus(plica_ordo);
        index = index + 1)
    {
        VolumenPlagula* p = (VolumenPlagula*)xar_obtinere(
            plica_ordo, index);

        si (!tabula_dispersa_continet(ctx.visae, p->via))
        {
            SilexProiciendaRes* r = (SilexProiciendaRes*)xar_addere(
                ctx.res);

            si (r != NIHIL)
            {
                r->via = p->via;
                r->status = SILEX_PROICIENDA_CREANDA;
            }
        }
    }

    fructus.res = ctx.res;
    fructus.intactae = ctx.intactae;
    fructus.obices = ctx.obices;

    si (!scribere)
    {
        volumen_claudere(vol);
        fructus.successus = VERUM;
        redde fructus;
    }

    /* applicatio: obices recusant (nihil inconditum deletur) */
    si (ctx.obices > 0)
    {
        volumen_claudere(vol);
        fructus.erratum = "contentum inconditum obstat - conde"
            " prima";
        redde fructus;
    }
    per (index = 0; index < xar_numerus(ctx.res);
        index = index + 1)
    {
        SilexProiciendaRes* r = (SilexProiciendaRes*)xar_obtinere(
            ctx.res, index);
        vacuum*             plica_v = NIHIL;
        VolumenPlagula*     p;
        b32                 inventum = FALSUM;
        chorda              contentum;
        constans character* via_plena;
        chorda              parens;

        si (r->status != SILEX_PROICIENDA_SCRIBENDA
            && r->status != SILEX_PROICIENDA_CREANDA)
        {
            perge;
        }
        (vacuum)tabula_dispersa_invenire(ctx.plica, r->via,
            &plica_v);
        p = (VolumenPlagula*)plica_v;
        si (p == NIHIL)
        {
            volumen_claudere(vol);
            fructus.erratum = "plica claudicat (via sine plagula)";
            redde fructus;
        }
        contentum = volumen_massam_promere(vol, p->sigillum_hex,
            piscina, &inventum);
        si (!inventum)
        {
            volumen_claudere(vol);
            fructus.erratum = "plagula in plica sine massa";
            redde fructus;
        }
        via_plena = _texere(piscina,
            chorda_ut_cstr(radix_absoluta, piscina), "/",
            chorda_ut_cstr(r->via, piscina));
        parens = via_directorium(
            chorda_ex_literis(via_plena, piscina), piscina);
        filum_directorium_creare_si_necesse(
            chorda_ut_cstr(parens, piscina));
        si (!filum_scribere(via_plena, contentum))
        {
            volumen_claudere(vol);
            fructus.erratum = _texere(piscina,
                "proiectio fracta: ", via_plena, NIHIL);
            redde fructus;
        }
        fructus.scriptae = fructus.scriptae + 1;
    }
    volumen_claudere(vol);
    fructus.successus = VERUM;
    redde fructus;
}

/* ==================================================
 * Renovatio (vide silex.h pro comparatione trium sigillorum)
 * ================================================== */

interior chorda
_sigillum_contenti (Piscina* piscina, chorda contentum);

interior chorda
_sigillum_contenti (Piscina* piscina, chorda contentum)
{
    Sigillum  sig;
    character hex[SIGILLUM_HEX_MENSURA];

    sig = sigillum_computare((constans vacuum*)contentum.datum,
        (memoriae_index)contentum.mensura);
    sigillum_hex(&sig, hex);
    redde chorda_ex_literis(hex, piscina);
}

/* ==================================================
 * Semina auctorata (re-radicatio renovationis + partes):
 * inclusiones plagularum auctoratarum in DISCO (etiam ante
 * conditionem - alioquin inclusio recens invisibilis, mensuratum
 * in vocabulario 2026-08-09), bases earum notatae ne capita
 * propria proiecti in fonte quaerantur.
 * ================================================== */

nomen structura {
    Piscina*        piscina;
    i32             radix_mensura;
    Xar*            nomina;             /* chorda: nomina capitum */
    TabulaDispersa* bases_auctoratae;   /* basis -> praesens */
} SeminaContextus;

interior chorda
_basis_viae (chorda via);

interior chorda
_basis_viae (chorda via)
{
    i32 i;

    per (i = via.mensura; i > 0; i = i - 1)
    {
        si (via.datum[i - 1] == '/')
        {
            redde chorda_ex_buffer(via.datum + i,
                via.mensura - i);
        }
    }
    redde via;
}

interior b32
_fons_codicis_est (chorda via);

interior b32
_fons_codicis_est (chorda via)
{
    redde _suffixum_habet(via, ".c")
        || _suffixum_habet(via, ".h")
        || _suffixum_habet(via, ".m");
}

interior s32
_semina_ambulator (chorda via_plena,
    constans DirectoriumIntroitus* introitus, vacuum* contextus);

interior s32
_semina_ambulator (chorda via_plena,
    constans DirectoriumIntroitus* introitus, vacuum* contextus)
{
    SeminaContextus* ctx = (SeminaContextus*)contextus;
    chorda           via_rel;

    si (introitus->genus != INTROITUS_FILUM)
    {
        redde 0;
    }
    si (via_plena.mensura <= ctx->radix_mensura)
    {
        redde 0;
    }
    via_rel = chorda_ex_buffer(
        via_plena.datum + ctx->radix_mensura,
        via_plena.mensura - ctx->radix_mensura);
    si (_praetermittenda(via_rel)
        || _praefixum_habet(via_rel, "lib/")
        || _praefixum_habet(via_rel, "include/")
        || _praefixum_habet(via_rel, "vendor/"))
    {
        redde 0;
    }
    {
        chorda basis = _basis_viae(via_rel);

        si (_praefixum_habet(basis, "capsula_"))
        {
            redde 0;   /* paria generata - nec semina nec bases */
        }
        tabula_dispersa_inserere(ctx->bases_auctoratae, basis,
            (vacuum*)ctx);
    }
    si (_fons_codicis_est(via_rel))
    {
        _inclusiones_scrutari(filum_legere_totum(
            chorda_ut_cstr(via_plena, ctx->piscina),
            ctx->piscina), ctx->nomina);
    }
    redde 0;
}

interior vacuum
_semina_auctorata_colligere (Piscina* piscina,
    constans character* radix_absoluta, Xar* nomina,
    TabulaDispersa* bases_auctoratae);

interior vacuum
_semina_auctorata_colligere (Piscina* piscina,
    constans character* radix_absoluta, Xar* nomina,
    TabulaDispersa* bases_auctoratae)
{
    SeminaContextus    ctx;
    DirectoriumFiltrum filtrum;

    ctx.piscina = piscina;
    ctx.radix_mensura = (i32)strlen(radix_absoluta) + 1;
    ctx.nomina = nomina;
    ctx.bases_auctoratae = bases_auctoratae;
    filtrum = directorium_filtrum_omnia();
    filtrum.includere_occultos = FALSUM;
    directorium_ambulare(radix_absoluta, &filtrum,
        _semina_ambulator, &ctx, piscina);
}

SilexRenovatioFructus
silex_renovare (Piscina* piscina, constans character* proiectum_dir,
    constans SilexFons* fons, b32 scribere)
{
    SilexRenovatioFructus fructus;
    constans character*   volumen_via;
    Volumen*              vol;
    Xar*                  acta;
    Xar*                  clausura;
    TabulaDispersa*       tunc;    /* via -> chorda* (sigillum) */
    TabulaDispersa*       novum;   /* via -> SilexRes* clausurae */
    TabulaDispersa*       visae;
    chorda                radix_absoluta;
    i32                   index;

    fructus.successus = FALSUM;
    fructus.res = NIHIL;
    fructus.intactae = 0;
    fructus.renovatae = 0;
    fructus.additae = 0;
    fructus.erratum = NIHIL;

    volumen_via = silex_volumen_viam_invenire(piscina, proiectum_dir);
    si (volumen_via == NIHIL)
    {
        fructus.erratum = "volumen deest - estne proiectum silicis?";
        redde fructus;
    }
    vol = volumen_aperire(piscina, volumen_via);
    si (vol == NIHIL)
    {
        fructus.erratum = "volumen aperiri non potuit";
        redde fructus;
    }

    /* missum-tunc: actum vendicata ultimum per viam; remota scopum
     * demit (quod consulto amovisti non resuscitatur) */
    tunc = tabula_dispersa_creare_chorda(piscina, 64);
    acta = volumen_acta_legere(vol, (s64)0, piscina);
    si (tunc == NIHIL || acta == NIHIL)
    {
        volumen_claudere(vol);
        fructus.erratum = "acta legi non potuerunt";
        redde fructus;
    }
    per (index = 0; index < xar_numerus(acta); index = index + 1)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, index);
        JsonResultus  lectum;
        JsonValor*    via_v;
        chorda        via;

        si (!chorda_aequalis_literis(a->genus, "plagula-condita")
            && !chorda_aequalis_literis(a->genus, "plagula-remota"))
        {
            perge;
        }
        lectum = json_legere(a->datum, piscina);
        si (!lectum.successus)
        {
            perge;
        }
        via_v = json_objectum_capere(lectum.radix, "via");
        si (via_v == NIHIL || !json_est_chorda(via_v))
        {
            perge;
        }
        via = json_ad_chorda(via_v);
        si (chorda_aequalis_literis(a->genus, "plagula-remota"))
        {
            tabula_dispersa_delere(tunc, via);
        }
        alioquin
        {
            JsonValor* orig_v = json_objectum_capere(lectum.radix,
                "origo");
            JsonValor* sig_v = json_objectum_capere(lectum.radix,
                "sigillum");

            si (orig_v == NIHIL || !json_est_chorda(orig_v)
                || sig_v == NIHIL || !json_est_chorda(sig_v))
            {
                perge;
            }
            si (_praefixum_habet(json_ad_chorda(orig_v),
                "vendicata:"))
            {
                chorda* cella = (chorda*)piscina_allocare(piscina,
                    (memoriae_index)magnitudo(chorda));

                si (cella == NIHIL)
                {
                    volumen_claudere(vol);
                    fructus.erratum = "memoria defecit";
                    redde fructus;
                }
                *cella = json_ad_chorda(sig_v);
                tabula_dispersa_inserere(tunc, via,
                    (vacuum*)cella);
            }
        }
    }

    /* semina = inclusiones plagularum auctoratarum (DISCI) +
     * capita vendicata (continuitas); bases auctoratae excluduntur
     * (capita propria proiecti). Ante: capita vendicata SOLA -
     * codex novus clausuram numquam trahebat (mensuratum in
     * vocabulario 2026-08-09). CAVE: Xar SEGMENTATUS est -
     * tabulatum planum e piscina aedificandum. */
    radix_absoluta = via_absoluta(
        chorda_ex_literis(proiectum_dir, piscina), piscina);
    {
        Xar*            nomina = xar_creare(piscina,
            (i32)magnitudo(chorda));
        TabulaDispersa* bases = tabula_dispersa_creare_chorda(
            piscina, 32);
        TabulaDispersa* electa = tabula_dispersa_creare_chorda(
            piscina, 64);
        Xar*            semina_ch = xar_creare(piscina,
            (i32)magnitudo(chorda));
        TabulaIterator  iter;
        chorda          clavis;
        vacuum*         valor;
        i32             n;

        si (nomina == NIHIL || bases == NIHIL || electa == NIHIL
            || semina_ch == NIHIL)
        {
            volumen_claudere(vol);
            fructus.erratum = "memoria defecit";
            redde fructus;
        }
        _semina_auctorata_colligere(piscina,
            chorda_ut_cstr(radix_absoluta, piscina), nomina,
            bases);
        iter = tabula_dispersa_iterator_initium(tunc);
        dum (tabula_dispersa_iterator_proximum(&iter, &clavis,
            &valor))
        {
            si (_praefixum_habet(clavis, "include/"))
            {
                chorda* cella = (chorda*)xar_addere(nomina);

                si (cella != NIHIL)
                {
                    *cella = chorda_ex_buffer(clavis.datum + 8,
                        clavis.mensura - 8);
                }
            }
        }
        per (n = 0; n < xar_numerus(nomina); n = n + 1)
        {
            chorda nomen_capitis = *(chorda*)xar_obtinere(nomina,
                n);

            si (tabula_dispersa_continet(bases, nomen_capitis)
                || tabula_dispersa_continet(electa, nomen_capitis))
            {
                perge;
            }
            tabula_dispersa_inserere(electa, nomen_capitis,
                (vacuum*)nomina);
            {
                chorda* cella = (chorda*)xar_addere(semina_ch);

                si (cella != NIHIL)
                {
                    *cella = nomen_capitis;
                }
            }
        }
        si (xar_numerus(semina_ch) == 0)
        {
            volumen_claudere(vol);
            fructus.res = xar_creare(piscina,
                (i32)magnitudo(SilexRenovatioRes));
            fructus.successus = VERUM;   /* nihil quaerendum */
            redde fructus;
        }
        {
            constans character** semina = (constans character**)
                piscina_allocare(piscina, (memoriae_index)(
                    (memoriae_index)xar_numerus(semina_ch)
                    * magnitudo(constans character*)));
            i32 s;

            si (semina == NIHIL)
            {
                volumen_claudere(vol);
                fructus.erratum = "memoria defecit";
                redde fructus;
            }
            per (s = 0; s < xar_numerus(semina_ch); s = s + 1)
            {
                semina[s] = chorda_ut_cstr(
                    *(chorda*)xar_obtinere(semina_ch, s),
                    piscina);
            }
            clausura = silex_clausuram_colligere(piscina, fons,
                (constans character* constans*)semina,
                xar_numerus(semina_ch));
        }
    }
    si (clausura == NIHIL)
    {
        volumen_claudere(vol);
        fructus.erratum = "fabrica invalida - clausura colligi non"
            " potuit";
        redde fructus;
    }

    novum = tabula_dispersa_creare_chorda(piscina, 128);
    visae = tabula_dispersa_creare_chorda(piscina, 64);
    fructus.res = xar_creare(piscina,
        (i32)magnitudo(SilexRenovatioRes));
    si (novum == NIHIL || visae == NIHIL || fructus.res == NIHIL)
    {
        volumen_claudere(vol);
        fructus.erratum = "memoria defecit";
        redde fructus;
    }
    per (index = 0; index < xar_numerus(clausura);
        index = index + 1)
    {
        SilexRes* e = (SilexRes*)xar_obtinere(clausura, index);

        tabula_dispersa_inserere(novum, e->via, (vacuum*)e);
    }

    /* classificatio: clausura (novum) contra tunc et discum */
    per (index = 0; index < xar_numerus(clausura);
        index = index + 1)
    {
        SilexRes* e = (SilexRes*)xar_obtinere(clausura, index);
        constans character* via_plena = _texere(piscina,
            chorda_ut_cstr(radix_absoluta, piscina), "/",
            chorda_ut_cstr(e->via, piscina));
        b32     in_disco = filum_existit(via_plena);
        vacuum* tunc_v = NIHIL;
        SilexRenovatioStatus classis;
        b32     intacta = FALSUM;

        si (tabula_dispersa_invenire(tunc, e->via, &tunc_v))
        {
            chorda sig_tunc = *(chorda*)tunc_v;
            chorda sig_novum = _sigillum_contenti(piscina,
                e->contentum);
            b32    fabrica_mota = !chorda_aequalis(sig_tunc,
                sig_novum);

            tabula_dispersa_inserere(visae, e->via, (vacuum*)e);
            si (!in_disco)
            {
                classis = fabrica_mota ? SILEX_RENOVATIO_RENOVANDA
                    : SILEX_RENOVATIO_VULNUS;   /* ablata manu */
            }
            alioquin
            {
                chorda sig_nunc = _sigillum_contenti(piscina,
                    filum_legere_totum(via_plena, piscina));

                si (!fabrica_mota)
                {
                    si (chorda_aequalis(sig_nunc, sig_tunc))
                    {
                        intacta = VERUM;
                    }
                    alioquin
                    {
                        classis = SILEX_RENOVATIO_VULNUS;
                    }
                }
                alioquin si (chorda_aequalis(sig_nunc, sig_tunc))
                {
                    classis = SILEX_RENOVATIO_RENOVANDA;
                }
                alioquin si (chorda_aequalis(sig_nunc, sig_novum))
                {
                    intacta = VERUM;   /* iam recens manu */
                }
                alioquin
                {
                    classis = SILEX_RENOVATIO_CONFLICTUS;
                }
            }
        }
        alioquin
        {
            /* clausurae nova, numquam vendicata */
            classis = in_disco ? SILEX_RENOVATIO_CONFLICTUS
                : SILEX_RENOVATIO_ADDENDA;   /* collisio si adest */
        }

        si (intacta)
        {
            fructus.intactae = fructus.intactae + 1;
        }
        alioquin
        {
            SilexRenovatioRes* r = (SilexRenovatioRes*)xar_addere(
                fructus.res);

            si (r != NIHIL)
            {
                r->via = e->via;
                r->status = classis;
            }
        }
    }

    /* vendicatae quas clausura hodierna nescit -> derelictae */
    {
        TabulaIterator iter = tabula_dispersa_iterator_initium(
            tunc);
        chorda  clavis;
        vacuum* valor;

        dum (tabula_dispersa_iterator_proximum(&iter, &clavis,
            &valor))
        {
            si (!tabula_dispersa_continet(visae, clavis))
            {
                SilexRenovatioRes* r = (SilexRenovatioRes*)
                    xar_addere(fructus.res);

                si (r != NIHIL)
                {
                    r->via = clavis;
                    r->status = SILEX_RENOVATIO_DERELICTA;
                }
            }
        }
    }

    si (!scribere)
    {
        volumen_claudere(vol);
        fructus.successus = VERUM;
        redde fructus;
    }

    /* applicatio: renovandae + addendae scribuntur et absorbentur
     * origine vendicata, transactione una; vulnera/conflictus/
     * derelictae retinentur nominatae - non obstant */
    si (!volumen_transactionem_incipere(vol))
    {
        volumen_claudere(vol);
        fructus.erratum = "transactio incipi non potuit";
        redde fructus;
    }
    per (index = 0; index < xar_numerus(fructus.res);
        index = index + 1)
    {
        SilexRenovatioRes* r = (SilexRenovatioRes*)xar_obtinere(
            fructus.res, index);
        vacuum*   novum_v = NIHIL;
        SilexRes* e;
        constans character* via_plena;
        chorda    parens;

        si (r->status != SILEX_RENOVATIO_RENOVANDA
            && r->status != SILEX_RENOVATIO_ADDENDA)
        {
            perge;
        }
        (vacuum)tabula_dispersa_invenire(novum, r->via, &novum_v);
        e = (SilexRes*)novum_v;
        si (e == NIHIL)
        {
            volumen_transactionem_revolvere(vol);
            volumen_claudere(vol);
            fructus.erratum = "clausura claudicat (via sine fonte)";
            redde fructus;
        }
        via_plena = _texere(piscina,
            chorda_ut_cstr(radix_absoluta, piscina), "/",
            chorda_ut_cstr(r->via, piscina));
        parens = via_directorium(
            chorda_ex_literis(via_plena, piscina), piscina);
        filum_directorium_creare_si_necesse(
            chorda_ut_cstr(parens, piscina));
        si (!filum_scribere(via_plena, e->contentum)
            || !volumen_plagulam_condere(vol, r->via, e->contentum,
                e->origo))
        {
            volumen_transactionem_revolvere(vol);
            volumen_claudere(vol);
            fructus.erratum = _texere(piscina,
                "renovatio fracta: ", via_plena, NIHIL);
            fructus.renovatae = 0;
            fructus.additae = 0;
            redde fructus;
        }
        si (r->status == SILEX_RENOVATIO_RENOVANDA)
        {
            fructus.renovatae = fructus.renovatae + 1;
        }
        alioquin
        {
            fructus.additae = fructus.additae + 1;
        }
    }
    si (fructus.renovatae + fructus.additae > 0)
    {
        ChordaAedificator* aed = chorda_aedificator_creare(piscina,
            (memoriae_index)128);
        chorda datum;

        chorda_aedificator_appendere_literis(aed,
            "{\"nuntius\":\"renovatio e fabrica\",\"renovatae\":");
        chorda_aedificator_appendere_s32(aed,
            (s32)fructus.renovatae);
        chorda_aedificator_appendere_literis(aed, ",\"additae\":");
        chorda_aedificator_appendere_s32(aed, (s32)fructus.additae);
        /* stampa fontis: via fabricae aut stampa corporis (nostra
         * ambae - sine characteribus JSON-hostilibus) */
        chorda_aedificator_appendere_literis(aed, ",\"fons\":\"");
        chorda_aedificator_appendere_literis(aed, fons->titulus);
        chorda_aedificator_appendere_literis(aed, "\"}");
        datum = chorda_aedificator_finire(aed);
        si (volumen_actum_appendere(vol, "conditio", datum) == 0)
        {
            volumen_transactionem_revolvere(vol);
            volumen_claudere(vol);
            fructus.erratum = "conditio scribi non potuit";
            redde fructus;
        }
    }
    si (!volumen_transactionem_committere(vol))
    {
        volumen_claudere(vol);
        fructus.erratum = "transactio committi non potuit";
        redde fructus;
    }
    volumen_claudere(vol);
    fructus.successus = VERUM;
    redde fructus;
}

Xar*
silex_partes (Piscina* piscina, constans character* proiectum_dir,
    constans SilexFons* fons, constans character* plagula)
{
    Xar*            nomina = xar_creare(piscina,
        (i32)magnitudo(chorda));
    TabulaDispersa* bases = tabula_dispersa_creare_chorda(piscina,
        32);
    TabulaDispersa* electa = tabula_dispersa_creare_chorda(
        piscina, 64);
    Xar*            semina_ch = xar_creare(piscina,
        (i32)magnitudo(chorda));
    chorda          radix_absoluta;
    Xar*            clausura;
    Xar*            fructus;
    i32             n;

    si (nomina == NIHIL || bases == NIHIL || electa == NIHIL
        || semina_ch == NIHIL || fons == NIHIL)
    {
        redde NIHIL;
    }
    radix_absoluta = via_absoluta(
        chorda_ex_literis(proiectum_dir, piscina), piscina);
    si (plagula != NIHIL)
    {
        constans character* via_plena = _texere(piscina,
            chorda_ut_cstr(radix_absoluta, piscina), "/",
            plagula);

        si (!filum_existit(via_plena))
        {
            redde NIHIL;
        }
        /* basis plagulae datae: caput proprium non quaerendum */
        tabula_dispersa_inserere(bases,
            _basis_viae(chorda_ex_literis(plagula, piscina)),
            (vacuum*)nomina);
        _inclusiones_scrutari(filum_legere_totum(via_plena,
            piscina), nomina);
    }
    alioquin
    {
        _semina_auctorata_colligere(piscina,
            chorda_ut_cstr(radix_absoluta, piscina), nomina,
            bases);
        /* capita vendicata e manifesto - aequivalentia cum
         * renovare (ABEST = quod '-scribere' vendicaret); volumen
         * absens = auctoratae solae (directorium quodvis licet) */
        {
            constans character* volumen_via =
                silex_volumen_viam_invenire(piscina,
                    chorda_ut_cstr(radix_absoluta, piscina));
            Volumen* vol = volumen_via == NIHIL ? NIHIL
                : volumen_aperire(piscina, volumen_via);

            si (vol != NIHIL)
            {
                Xar* plica = volumen_plicam_ad(vol, (s64)0,
                    piscina);
                i32  p;

                si (plica != NIHIL)
                {
                    per (p = 0; p < xar_numerus(plica);
                        p = p + 1)
                    {
                        VolumenPlagula* pl = (VolumenPlagula*)
                            xar_obtinere(plica, p);

                        si (pl == NIHIL
                            || !_praefixum_habet(pl->via,
                                "include/")
                            || !_praefixum_habet(pl->origo,
                                "vendicata:"))
                        {
                            perge;
                        }
                        {
                            chorda* cella = (chorda*)xar_addere(
                                nomina);

                            si (cella != NIHIL)
                            {
                                *cella = chorda_ex_buffer(
                                    pl->via.datum + 8,
                                    pl->via.mensura - 8);
                            }
                        }
                    }
                }
                volumen_claudere(vol);
            }
        }
    }
    per (n = 0; n < xar_numerus(nomina); n = n + 1)
    {
        chorda nomen_capitis = *(chorda*)xar_obtinere(nomina, n);

        /* basis comparatur (inclusio '../fontes/x.h' plagulam
         * auctoratam 'fontes/x.h' nominat); capsula_* generata */
        si (tabula_dispersa_continet(bases,
                _basis_viae(nomen_capitis))
            || _praefixum_habet(_basis_viae(nomen_capitis),
                "capsula_")
            || tabula_dispersa_continet(electa, nomen_capitis))
        {
            perge;
        }
        tabula_dispersa_inserere(electa, nomen_capitis,
            (vacuum*)nomina);
        {
            chorda* cella = (chorda*)xar_addere(semina_ch);

            si (cella != NIHIL)
            {
                *cella = nomen_capitis;
            }
        }
    }
    {
        constans character** semina = (constans character**)
            piscina_allocare(piscina, (memoriae_index)(
                ((memoriae_index)xar_numerus(semina_ch) + I)
                * magnitudo(constans character*)));
        i32 s;

        si (semina == NIHIL)
        {
            redde NIHIL;
        }
        per (s = 0; s < xar_numerus(semina_ch); s = s + 1)
        {
            semina[s] = chorda_ut_cstr(
                *(chorda*)xar_obtinere(semina_ch, s), piscina);
        }
        clausura = silex_clausuram_colligere(piscina, fons,
            (constans character* constans*)semina,
            xar_numerus(semina_ch));
    }
    si (clausura == NIHIL)
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(SilexPartesRes));
    si (fructus == NIHIL)
    {
        redde NIHIL;
    }
    per (n = 0; n < xar_numerus(clausura); n = n + 1)
    {
        SilexRes*       e = (SilexRes*)xar_obtinere(clausura, n);
        SilexPartesRes* r = (SilexPartesRes*)xar_addere(fructus);

        si (r == NIHIL)
        {
            perge;
        }
        r->via = e->via;
        r->origo = e->origo;
        r->adest = filum_existit(_texere(piscina,
            chorda_ut_cstr(radix_absoluta, piscina), "/",
            chorda_ut_cstr(e->via, piscina)));
    }
    redde fructus;
}
