/* silex.c - proiecta nova e fabrica excudere (vide silex.h) */

#include "silex.h"
#include "volumen.h"
#include "filum.h"
#include "via.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "processus.h"

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
_plagulam_e_fabrica_colligere (Piscina* piscina,
    constans character* fabrica, constans character* pars,
    chorda titulus, Xar* fructus, Xar* opus);

/* pars = "include/" aut "lib/"; VERUM = plagula exsistit et lecta */
interior b32
_plagulam_e_fabrica_colligere (Piscina* piscina,
    constans character* fabrica, constans character* pars,
    chorda titulus, Xar* fructus, Xar* opus)
{
    chorda              via_rel;
    constans character* via_rel_cstr;
    constans character* via_plena;
    chorda              contentum;
    SilexRes*           res;

    via_rel = _praefigere(piscina, pars, titulus);
    via_rel_cstr = chorda_ut_cstr(via_rel, piscina);
    via_plena = _texere(piscina, fabrica, "/", via_rel_cstr);
    si (!filum_existit(via_plena))
    {
        redde FALSUM;
    }
    contentum = filum_legere_totum(via_plena, piscina);
    res = (SilexRes*)xar_addere(fructus);
    si (res == NIHIL)
    {
        redde FALSUM;
    }
    res->via = via_rel;
    res->contentum = contentum;
    res->origo = _texere(piscina, "vendicata:", via_rel_cstr, NIHIL);
    _inclusiones_scrutari(contentum, opus);
    redde VERUM;
}

Xar*
silex_clausuram_colligere (Piscina* piscina,
    constans character* fabrica,
    constans character* constans* semina, i32 numerus_seminum)
{
    Xar*            opus;
    Xar*            fructus;
    TabulaDispersa* visa;
    i32             s;
    i32             index;

    si (!filum_directorium_existit(
        _texere(piscina, fabrica, "/include", NIHIL)))
    {
        fprintf(stderr,
            "silex: fabrica invalida (include/ deest): %s\n",
            fabrica);
        redde NIHIL;
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

        si (tabula_dispersa_continet(visa, caput))
        {
            perge;
        }
        tabula_dispersa_inserere(visa, caput, (vacuum*)fructus);

        si (!_plagulam_e_fabrica_colligere(piscina, fabrica,
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
            _plagulam_e_fabrica_colligere(piscina, fabrica, "lib/",
                titulus_c, fructus, opus);
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
    res_omnes = silex_clausuram_colligere(piscina, optiones->fabrica,
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
