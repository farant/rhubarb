/* probatio_crusta_oraculum.c - Porta oraculi 'declare -f' (P11b)
 *
 * Quisque casus oraculi (crusta_oraculum_casus: pathologiae.sh +
 * FreeBSD) cum sectione EXSPECTATUM in auro expectata.txt: involucrum
 * 'f() {' casus '}' parsatur, coquitur (crusta_coctum_scribere) et cum
 * effusione 'declare -f f' bash 5.2.15 comparatur. Aurum solum
 * legitur - nihil generatur, porta deterministica; aurum
 * ./crusta/oraculum.sh -scribere renovat (causa nominata), -probare
 * bash iterum currit.
 *
 * Numeri: pares / dispares / sine auro (bash casum recusavit).
 * PINNA_PARIUM ASCENDIT tantum: pares numquam infra; si supra, porta
 * nuntiat et pinna attollenda est. Mensura sui: casus et aura pinnata
 * (lector exemplorum fractus aut aurum truncatum viride mentiretur),
 * aurum orbum (clavis sine casu) nullum.
 *
 * Dispar quisque NOMINATUR semper (permutatio quae tot sanat quot
 * frangit summam immotam relinquit - nomina eam nudant; impressio
 * ipsa pinnata, 'nominati == dispares').
 * ORACULUM_OMNIA=1: lineam primam differentem cuiusque dispari
 * imprimit. ORACULUM_EXEMPLUM=<clavis>: casum unum totum (fons,
 * exspectatum, habitum).
 */

#include "latina.h"
#include "credo.h"
#include "crusta_arbor.h"
#include "crusta_coctum.h"
#include "crusta_exempla.h"
#include "crusta_oraculum.h"
#include "crusta_registrum.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CASUS_NUMERUS  125
#define AURA_NUMERUS   115
#define PINNA_PARIUM   106

interior vacuum
_differentiam_imprimere (
                  chorda clavis,
                  chorda exspectatum,
                  chorda habitum)
{
    i32 k        = ZEPHYRUM;
    i32 initium  = ZEPHYRUM;
    i32 linea    = I;
    i32 fe;
    i32 fh;

    dum (   k < exspectatum.mensura && k < habitum.mensura
         && exspectatum.datum[k] == habitum.datum[k])
    {
        si (exspectatum.datum[k] == '\n')
        {
            initium = k + I;
            linea++;
        }
        k++;
    }
    fe = initium;
    dum (fe < exspectatum.mensura && exspectatum.datum[fe] != '\n')
    {
        fe++;
    }
    fh = initium;
    dum (fh < habitum.mensura && habitum.datum[fh] != '\n')
    {
        fh++;
    }
    imprimere("    %.*s linea %d\n      bash:   '%.*s'\n      crusta: "
        "'%.*s'\n", (integer)clavis.mensura,
        (constans character*)clavis.datum, (integer)linea,
        (integer)(fe - initium),
        (constans character*)exspectatum.datum + initium,
        (integer)(fh > initium ? fh - initium : ZEPHYRUM),
        (constans character*)habitum.datum + initium);
}

interior b32
_clavis_est (
                 chorda  clavis,
     constans character* litterae)
{
    redde litterae != NIHIL
        && clavis.mensura == (i32)strlen(litterae)
        && memcmp(clavis.datum, litterae, (size_t)clavis.mensura)
            == ZEPHYRUM;
}

s32
principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
    constans character* radix     = getenv("RHUBARB_RADIX");
    constans character* omnia     = getenv("ORACULUM_OMNIA");
    constans character* exemplum  = getenv("ORACULUM_EXEMPLUM");
    constans character* causa;
                   Xar* collectio;
                   Xar* aurum;
             character* fons;
                   i32  mensura    = ZEPHYRUM;
                   i32  pares      = ZEPHYRUM;
                   i32  dispares   = ZEPHYRUM;
                   i32  nominati   = ZEPHYRUM;
                   i32  sine_auro  = ZEPHYRUM;
                   i32  orbi       = ZEPHYRUM;
                   i32  k;
                   i32  j;

    piscina = piscina_generare_dynamicum("probatio_crusta_oraculum",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    si (radix == NIHIL)
    {
        radix = ".";
    }

    imprimere("\n--- Probans casus contra aurum declare -f ---\n");

    collectio = crusta_oraculum_casus(piscina, radix, &causa);
    fons = crusta_oraculum_legere(piscina, radix,
        CRUSTA_ORACULUM_EXPECTATA, &mensura);
    si (collectio == NIHIL || fons == NIHIL)
    {
        CREDO_CULPA (collectio == NIHIL ? "casus oraculi non lecti"
            : "aurum expectata.txt absens - ./crusta/oraculum.sh "
              "-scribere");
        credo_imprimere_compendium();
        redde I;
    }
    aurum = crusta_exempla_legere(piscina, fons, mensura);
    CREDO_NON_NIHIL (aurum);
    si (aurum == NIHIL)
    {
        redde I;
    }

    per (k = ZEPHYRUM; k < xar_numerus(collectio); k++)
    {
        constans CrustaCasusOraculi* c =
            (constans CrustaCasusOraculi*)xar_obtinere(collectio, k);
                              chorda exspectatum;
                              chorda involutum;
                              chorda coctum;
                       MateriaNodus* arbor;
                       CrustaParsura  relatio;
                                 b32  par;

        exspectatum = crusta_oraculum_aurum(aurum, c->clavis,
            "EXSPECTATUM");
        si (exspectatum.datum == NIHIL)
        {
            sine_auro++;
            perge;
        }
        involutum = crusta_oraculum_involvere(piscina, c->datum);
        arbor     = crusta_arbor_parsare(piscina,
            (constans character*)involutum.datum, involutum.mensura,
            &CRUSTA_BASH, &relatio);
        coctum.datum    = NIHIL;
        coctum.mensura  = ZEPHYRUM;
        si (arbor != NIHIL)
        {
            coctum = crusta_coctum_scribere(piscina, arbor);
        }
        par = (   coctum.datum != NIHIL
               && coctum.mensura == exspectatum.mensura
               && memcmp(coctum.datum, exspectatum.datum,
                   (size_t)coctum.mensura)
                       == ZEPHYRUM) ? VERUM : FALSUM;
        si (par)
        {
            pares++;
        }
        alioquin
        {
            dispares++;
            /* NOMEN SEMPER, non sub ORACULUM_OMNIA solo.
             *
             * Pinna parium SOLUM est: permutatio (tot sanata quot
             * fracta) numerum IMMOTUM relinquit, et sine nomine
             * impresso nihil omnino mutari videtur. Nomina impressa
             * permutationem nudant quamvis summa taceat - quod
             * aurum seriei html praestat, hic pretio nullo, quia
             * dispares IX solae sunt et causam unam notam ferunt
             * (01M2PN1VYH: backslash ante lineam novam). */
            si (omnia != NIHIL && coctum.datum != NIHIL)
            {
                _differentiam_imprimere(c->clavis, exspectatum, coctum);
            }
            alioquin
            {
                imprimere("    dispar: %.*s\n",
                    (integer)c->clavis.mensura,
                    (constans character*)c->clavis.datum);
            }
            nominati++;
        }
        si (_clavis_est(c->clavis, exemplum))
        {
            imprimere("=== %s (sana %d, mala %d)\n--- fons:\n%.*s--- "
                "exspectatum:\n%.*s--- habitum:\n%.*s===\n", exemplum,
                (integer)relatio.sana, (integer)relatio.mala,
                (integer)involutum.mensura,
                (constans character*)involutum.datum,
                (integer)exspectatum.mensura,
                (constans character*)exspectatum.datum,
                (integer)coctum.mensura,
                coctum.datum != NIHIL
                    ? (constans character*)coctum.datum : "");
        }
    }

    /* aurum orbum: clavis auri sine casu */
    per (j = ZEPHYRUM; j < xar_numerus(aurum); j++)
    {
        constans CrustaExemplum* e =
            (constans CrustaExemplum*)xar_obtinere(aurum, j);
                            b32 inventa = FALSUM;

        per (k = ZEPHYRUM; k < xar_numerus(collectio) && !inventa; k++)
        {
            constans CrustaCasusOraculi* c =
                (constans CrustaCasusOraculi*)xar_obtinere(collectio,
                k);

            si (   c->clavis.mensura == e->titulus.mensura
                && memcmp(c->clavis.datum, e->titulus.datum,
                    (size_t)c->clavis.mensura) == ZEPHYRUM)
            {
                inventa = VERUM;
            }
        }
        si (!inventa)
        {
            orbi++;
            imprimere("    aurum orbum: %.*s\n",
                (integer)e->titulus.mensura,
                (constans character*)e->titulus.datum);
        }
    }

    imprimere("    casus %d, aura %d: pares %d, dispares %d, sine auro "
        "%d (pinna parium %d)\n", (integer)xar_numerus(collectio),
        (integer)xar_numerus(aurum), (integer)pares, (integer)dispares,
        (integer)sine_auro, (integer)PINNA_PARIUM);
    si (pares > (i32)PINNA_PARIUM)
    {
        imprimere("    NOTA: pares %d > pinna %d - pinnam attolle\n",
            (integer)pares, (integer)PINNA_PARIUM);
    }
    CREDO_AEQUALIS_I32 (xar_numerus(collectio), (i32)CASUS_NUMERUS);
    CREDO_AEQUALIS_I32 (xar_numerus(aurum), (i32)AURA_NUMERUS);
    CREDO_AEQUALIS_I32 (pares + dispares, (i32)AURA_NUMERUS);
    /* IMPRESSIO IPSA PINNATA: dispar quisque nominatus esse debet.
     * Sine hac assertione impressio ornamentum est quod quis tacite
     * sub condicione claudere potest, et porta caeca redit. */
    CREDO_AEQUALIS_I32 (nominati, dispares);
    CREDO_AEQUALIS_I32 (orbi, ZEPHYRUM);
    CREDO_VERUM (pares >= (i32)PINNA_PARIUM);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
