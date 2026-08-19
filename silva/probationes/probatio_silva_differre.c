/* probatio_silva_differre.c - machina parium differentiae
 * (01M0D4RN3B: unitates spatiis-solum reformatae paria frangebant)
 *
 * SPECIMINA ANTE PRAEDICATUM. Missio differre est mutationes
 * cosmeticas classificare - par frangi IN mutatione cosmetica
 * ipsa contra missionem. Classes venatae (evolutio formatoris):
 *
 *   I.   directiva bis apparens (bracchia #ifdef) spatiis mutata:
 *        tabula titulorum ordinalem PRIMUM solum servabat - copia
 *        secunda irreperibilis -> ADDITA+REMOTA falsa.
 *   II.  unitas SINE titulo (#endif) spatiis mutata: sigillum
 *        crudum mutatum, titulus nullus -> par numquam.
 *   III. tituli collidentes corporibus diversis (bracchia #ifdef
 *        functionis eiusdem): paritio per titulum trans bracchia
 *        LIGARE potest -> 'substantiva' falsa; sigillum normatum
 *        (series lexematum) parem VERUM eligit.
 *   IV.  moderamina: ADDITA/REMOTA/substantiva/documentaria verae
 *        manent - nulla sectio nimia.
 *   V.   PROPRIETAS METAMORPHICA: pro plagula F et transformatione
 *        spatiali W (formator!): differre(F, W(F)) = ADDITA 0,
 *        REMOTA 0, MUTATA omnes cosmeticae.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "silva_differre.h"
#include "silva_formator.h"
#include "credo.h"

#include <stdio.h>
#include <string.h>

/* latera duo + paria ex litteris; NIHIL = apparatus fractus */
interior Xar*
_paria (
               Piscina* piscina,
    constans character* vetus,
    constans character* novum,
    SilvaDifferreLatus* a,
    SilvaDifferreLatus* b,
                   i32* immotae)
{
    InternamentumChorda* intern;

    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde NIHIL;
    }
    si (   !silva_differre_latus_ex_textu(piscina, intern,
            chorda_ex_literis(vetus, piscina), "vetus", a)
        || !silva_differre_latus_ex_textu(piscina, intern,
               chorda_ex_literis(novum, piscina), "novum", b))
    {
        redde NIHIL;
    }
    *immotae = ZEPHYRUM;
    redde silva_differre_paria(piscina, a, b, immotae);
}

interior i32
_numerus_status (
                   Xar* paria,
    constans character* status)
{
    i32 n;
    i32 k;

    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < xar_numerus(paria); k += I)
    {
        constans SilvaDifferrePar* par =
            (constans SilvaDifferrePar*)xar_obtinere(paria, k);

        si (strcmp(par->status, status) == ZEPHYRUM)
        {
            n += I;
        }
    }
    redde n;
}

/* MUTATA omnes classificatione data? (per spatia parium) */
interior b32
_mutatae_omnes (
                        Piscina* piscina,
    constans SilvaDifferreLatus* a,
    constans SilvaDifferreLatus* b,
                            Xar* paria,
             constans character* classificatio)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(paria); k += I)
    {
        constans SilvaDifferrePar* par =
            (constans SilvaDifferrePar*)xar_obtinere(paria, k);

        si (strcmp(par->status, "MUTATA") == ZEPHYRUM)
        {
            constans character* c = silva_differre_classificare_textus(
                piscina,
                silva_differre_spatium(a, (i32)par->a_index),
                silva_differre_spatium(b, (i32)par->b_index));

            si (strcmp(c, classificatio) != ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

s32
principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_differre",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I: directiva bis, spatiis mutata ---\n");
    {
        SilvaDifferreLatus  a;
        SilvaDifferreLatus  b;
                       i32  immotae;
                       Xar* paria = _paria(piscina,
                           "#ifdef _WIN32\n"
                           "#\tinclude <sys/stat.h>\n"
                           "#else\n"
                           "#\tinclude <sys/stat.h>\n"
                           "#endif\n"
                           "int x;\n",
                           "#ifdef _WIN32\n"
                           "#   include <sys/stat.h>\n"
                           "#else\n"
                           "#   include <sys/stat.h>\n"
                           "#endif\n"
                           "int x;\n",
                           &a, &b, &immotae);

        CREDO_NON_NIHIL(paria);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "ADDITA"),
            (i32)0);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "REMOTA"),
            (i32)0);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "MUTATA"),
            (i32)2);
        CREDO_VERUM(_mutatae_omnes(piscina, &a, &b, paria,
            "cosmetica"));
    }

    imprimere("\n--- II: sine titulo (#endif) spatiis mutatum ---\n");
    {
        SilvaDifferreLatus  a;
        SilvaDifferreLatus  b;
                       i32  immotae;
                       Xar* paria = _paria(piscina,
                           "#ifdef A\n"
                           "int x;\n"
                           "#\tendif\n",
                           "#ifdef A\n"
                           "int x;\n"
                           "#   endif\n",
                           &a, &b, &immotae);

        CREDO_NON_NIHIL(paria);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "ADDITA"),
            (i32)0);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "REMOTA"),
            (i32)0);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "MUTATA"),
            (i32)1);
        CREDO_VERUM(_mutatae_omnes(piscina, &a, &b, paria,
            "cosmetica"));
    }

    imprimere("\n--- III: tituli collidentes, corpora diversa ---\n");
    {
        /* bracchia #ifdef functionis eiusdem: titulus idem, corpora
         * diversa. Spatiis solis mutatis par VERUM = intra bracchium
         * (sigillo normato) - trans bracchia ligatum 'substantiva'
         * falsum pareret. */
        SilvaDifferreLatus  a;
        SilvaDifferreLatus  b;
                       i32  immotae;
                       Xar* paria = _paria(piscina,
                           "#ifdef _WIN32\n"
                           "int f(void) {\treturn 1; }\n"
                           "#else\n"
                           "int f(void) {\treturn 2; }\n"
                           "#endif\n",
                           "#ifdef _WIN32\n"
                           "int f(void) {   return 1; }\n"
                           "#else\n"
                           "int f(void) {   return 2; }\n"
                           "#endif\n",
                           &a, &b, &immotae);

        CREDO_NON_NIHIL(paria);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "ADDITA"),
            (i32)0);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "REMOTA"),
            (i32)0);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "MUTATA"),
            (i32)2);
        CREDO_VERUM(_mutatae_omnes(piscina, &a, &b, paria,
            "cosmetica"));
    }

    imprimere("\n--- IV: moderamina (sectio nimia nulla) ---\n");
    {
        /* additum verum */
        SilvaDifferreLatus  a;
        SilvaDifferreLatus  b;
                       i32  immotae;
                       Xar* paria = _paria(piscina,
                           "int x;\n",
                           "int x;\nint y;\n",
                           &a, &b, &immotae);

        CREDO_NON_NIHIL(paria);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "ADDITA"),
            (i32)1);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "REMOTA"),
            (i32)0);
    }
    {
        /* remotum verum */
        SilvaDifferreLatus  a;
        SilvaDifferreLatus  b;
                       i32  immotae;
                       Xar* paria = _paria(piscina,
                           "int x;\nint y;\n",
                           "int x;\n",
                           &a, &b, &immotae);

        CREDO_NON_NIHIL(paria);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "REMOTA"),
            (i32)1);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "ADDITA"),
            (i32)0);
    }
    {
        /* mutatio substantiva vera */
        SilvaDifferreLatus  a;
        SilvaDifferreLatus  b;
                       i32  immotae;
                       Xar* paria = _paria(piscina,
                           "int f(void) { return 1; }\n",
                           "int f(void) { return 2; }\n",
                           &a, &b, &immotae);

        CREDO_NON_NIHIL(paria);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "MUTATA"),
            (i32)1);
        CREDO_VERUM(_mutatae_omnes(piscina, &a, &b, paria,
            "substantiva"));
    }
    {
        /* mutatio documentaria vera (commentum solum) */
        SilvaDifferreLatus  a;
        SilvaDifferreLatus  b;
                       i32  immotae;
                       Xar* paria = _paria(piscina,
                           "/* vetus */\nint f(void) { return 1; }\n",
                           "/* novum */\nint f(void) { return 1; }\n",
                           &a, &b, &immotae);

        CREDO_NON_NIHIL(paria);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "MUTATA"),
            (i32)1);
        CREDO_VERUM(_mutatae_omnes(piscina, &a, &b, paria,
            "documentaria"));
    }

    imprimere("\n--- V: proprietas metamorphica (W = formator) ---\n");
    {
        /* differre(F, W(F)) pro W spatiali solum: paria integra,
         * cosmetica omnia. Fixtura: directivae geminae + functio
         * tabulis indentata (W tabulas emendat). */
        constans character* fons =
            "#ifdef _WIN32\n"
            "#\tinclude <a.h>\n"
            "#else\n"
            "#\tinclude <a.h>\n"
            "#endif\n"
            "\n"
            "interior i32\n"
            "_adiuvare (\n"
            "    i32 valor)\n"
            "{\n"
            "\tsi (valor > I) redde valor;\n"
            "\tredde ZEPHYRUM;\n"
            "}\n";
        FormatorScriptum s = formator_scribere(piscina, NIHIL,
            fons, (i32)strlen(fons));
         SilvaDifferreLatus  a;
         SilvaDifferreLatus  b;
                        i32  immotae;
        InternamentumChorda* intern;
                        Xar* paria;

        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        intern = internamentum_creare(piscina);
        CREDO_NON_NIHIL(intern);
        CREDO_VERUM(silva_differre_latus_ex_textu(piscina, intern,
            chorda_ex_literis(fons, piscina), "vetus", &a));
        CREDO_VERUM(silva_differre_latus_ex_textu(piscina, intern,
            s.textus, "novum", &b));
        immotae  = ZEPHYRUM;
        paria    = silva_differre_paria(piscina, &a, &b, &immotae);
        CREDO_NON_NIHIL(paria);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "ADDITA"),
            (i32)0);
        CREDO_AEQUALIS_I32(_numerus_status(paria, "REMOTA"),
            (i32)0);
        CREDO_VERUM(_mutatae_omnes(piscina, &a, &b, paria,
            "cosmetica"));
    }

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
