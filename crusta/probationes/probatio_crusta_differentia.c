/* probatio_crusta_differentia.c - Porta sanitatis contra 'bash -n'
 * (P11b)
 *
 * (a) Quisque casus oraculi cum sectione 'status' in auro sanitas.txt:
 *     casus crudus (non involutus) parsatur; verdictum nostrum
 *     relatio.sana (mala nulla, clausurae omnes) contra 'bash -n -c'
 *     status == 0. CONCORDES pinnati ASCENDENTES; discordes SEMPER
 *     nominantur cum verdictis ambobus - divergentiae notae (quaestio
 *     01M2NT4CDD): bash monet sed exitus 0 (heredoc ad EOF, '[[ a b
 *     ]]', arithmetica tempore parsurae non iudicata). Discordes VII:
 *     IV formae D9 (vide (c) infra) et III desiderati 01M2PN1VYH.
 * (b) Corpus domus: omnis plagula build/crusta_corpus.lst (cursor eam
 *     scribit) praeter fixa pathologiae.sh et adversarius.sh
 *     parsatur, mala summantur - pinna DESCENDENS tantum, finis
 *     ZEPHYRUM.
 * (c) Gravitas et linea super eosdem casus (planum materia-sedes B3,
 *     passus VI), ex diagnosticis DERIVATIS - via ALIA quam (a), quae
 *     numeros parsatoris legit: 'erratum nullum' contra bash exitum 0
 *     (monitum exitum non movet - delta D9), et linea errati primi
 *     contra numerum post 'line ' in textu erroris bash. Pinnae ambae
 *     ASCENDENTES; discordes SEMPER nominantur.
 *
 *     Mensa nativitatis (2026-09-17): gravitas CXXII/CXXV contra
 *     CONCORDES CXVIII eiusdem cursus - HIATUS QUATTUOR est ratio
 *     huius pinnae. Formae D9 ('[[ a b ]]', '(( 1 2 ))', '(( 3 ! ))',
 *     '$(( 1 + ))') bash exitu ZEPHYRUM accipit; relatio.sana eas
 *     negat, sed diagnostica derivata eas MONITA vocant, non errata.
 *     Ergo (c) concordat ubi (a) discordat: gravitas quod numeri
 *     parsatoris non possunt mensurat. Casus appensi in pathologiae.sh
 *     hoc passu ipso, quia corpus eos non ferebat (planta viridis
 *     mansit donec adessent - lectio eadem quam passus III dedit).
 *     Discordes tres reliqui iidem sunt quos (a) videt, OMNES
 *     desiderati 01M2PN1VYH (bash '\'+nova-linea ANTE lexin tollit:
 *     line-cont5.0, line-cont6.0; heredocum in substitutione
 *     multi-lineari: heredoc5.0) - pinna ad CXXV ascendet cum illud
 *     impletum erit.
 *
 *     Linea VII/IX: casus omnes quibus bash LEXEMA nominat ('near
 *     unexpected token', VI) concordant; discordant duo ex tribus
 *     quibus bash FINEM PLAGULAE nominat ('unexpected end of file'),
 *     ubi bash lineam EOF dat et crusta finem constructionis
 *     imperfectae (crlf: bash V, crusta II; si-apertum: bash II,
 *     crusta I). Ea divergentia CONSILIUM est, non vitium: sedes
 *     constructionis lectori plus valet quam sedes EOF.
 *
 * Mensura sui: casus, aura et plagulae domus numerantur et pinnantur.
 */

#include "latina.h"
#include "credo.h"
#include "crusta_arbor.h"
#include "crusta_diagnostica.h"
#include "crusta_exempla.h"
#include "crusta_oraculum.h"
#include "crusta_registrum.h"
#include "materia_diagnostica.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CASUS_NUMERUS      125
#define AURA_NUMERUS       125
#define PINNA_CONCORDIUM   118
#define PINNA_GRAVITATIS   122
#define PINNA_LINEARUM     7
#define MALA_DOMUS_PINNA   0
#define DOMUS_MINIMUM      200

interior i32
_status (
    chorda textus)
{
    i32 k;
    i32 v = ZEPHYRUM;

    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] < '0' || textus.datum[k] > '9')
        {
            redde (i32)CCLV;
        }
        v = v * (i32)X + (i32)(textus.datum[k] - '0');
    }
    redde textus.mensura > ZEPHYRUM ? v : (i32)CCLV;
}

/* numerus post 'line ' primum in textu erroris bash; ZEPHYRUM =
 * nullus */
interior i32
_linea_bash (
    chorda textus)
{
    i32 k;

    per (k = ZEPHYRUM; k + V < textus.mensura; k++)
    {
        si (memcmp(textus.datum + k, "line ", (size_t)V) == ZEPHYRUM)
        {
            i32 v = ZEPHYRUM;
            i32 j = k + V;

            dum (   j < textus.mensura && textus.datum[j] >= '0'
                 && textus.datum[j] <= '9')
            {
                v = v * (i32)X + (i32)(textus.datum[j] - '0');
                j++;
            }
            redde v;
        }
    }
    redde ZEPHYRUM;
}

s32
principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
    constans character* radix = getenv("RHUBARB_RADIX");
    constans character* causa;
                   Xar* collectio;
                   Xar* aurum;
             character* fons;
             character* lista;
                   i32  mensura    = ZEPHYRUM;
                   i32  concordes  = ZEPHYRUM;
                   i32  discordes  = ZEPHYRUM;
                   i32  sine_auro  = ZEPHYRUM;
                   i32  plagulae   = ZEPHYRUM;
                   i32  mala       = ZEPHYRUM;
                   i32  k;
                   i32  gravitas_concordes  = ZEPHYRUM;
                   i32  lineae_comparatae   = ZEPHYRUM;
                   i32  lineae_concordes    = ZEPHYRUM;

    piscina = piscina_generare_dynamicum("probatio_crusta_differentia",
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


    /* ==================================================
     * PROBARE: sanitas contra 'bash -n'
     * ================================================== */

    imprimere("\n--- Probans sanitatem contra aurum bash -n ---\n");

    collectio = crusta_oraculum_casus(piscina, radix, &causa);
    fons = crusta_oraculum_legere(piscina, radix,
        CRUSTA_ORACULUM_SANITAS, &mensura);
    si (collectio == NIHIL || fons == NIHIL)
    {
        CREDO_CULPA (collectio == NIHIL ? "casus oraculi non lecti"
            : "aurum sanitas.txt absens - ./crusta/oraculum.sh "
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
                              chorda status;
                              chorda erratum;
                       CrustaParsura relatio;
                       MateriaNodus* arbor;
                                 b32  accepta;
                                 b32  sana;

        status = crusta_oraculum_aurum(aurum, c->clavis, "status");
        si (status.datum == NIHIL)
        {
            sine_auro++;
            perge;
        }
        erratum = crusta_oraculum_aurum(aurum, c->clavis, "ERRATUM");
        arbor   = crusta_arbor_parsare(piscina,
            (constans character*)c->datum.datum, c->datum.mensura,
            &CRUSTA_BASH, &relatio);
        accepta  = _status(status) == ZEPHYRUM ? VERUM : FALSUM;
        sana     = (arbor != NIHIL && relatio.sana) ? VERUM : FALSUM;
        si (accepta == sana)
        {
            concordes++;
        }
        alioquin
        {
            discordes++;
            imprimere("    discors %.*s: bash status %.*s ('%.*s'), "
                "crusta sana %d (mala %d, clausurae absentes %d)\n",
                (integer)c->clavis.mensura,
                (constans character*)c->clavis.datum,
                (integer)status.mensura,
                (constans character*)status.datum,
                (integer)erratum.mensura,
                erratum.datum != NIHIL
                    ? (constans character*)erratum.datum : "",
                (integer)sana, (integer)relatio.mala,
                (integer)relatio.clausurae_absentes);
        }
        si (arbor != NIHIL)
        {
                                   Xar* diagnostica;
            constans MateriaDiagnosticum* primum = NIHIL;
                                   i32  j;

            diagnostica = crusta_diagnostica(piscina, arbor, &relatio);
            per (j = ZEPHYRUM; diagnostica != NIHIL
                 && j < xar_numerus(diagnostica); j++)
            {
                constans MateriaDiagnosticum* d =
                    (constans MateriaDiagnosticum*)xar_obtinere(
                        diagnostica, j);

                si (d->gravitas == (s32)MATERIA_GRAVITAS_ERRATUM)
                {
                    primum = d;
                    frange;
                }
            }
            si ((primum == NIHIL) == (accepta ? VERUM : FALSUM))
            {
                gravitas_concordes++;
            }
            alioquin
            {
                imprimere("    gravitas discors %.*s: bash status "
                    "%.*s, erratum primum %s\n",
                    (integer)c->clavis.mensura,
                    (constans character*)c->clavis.datum,
                    (integer)status.mensura,
                    (constans character*)status.datum,
                    primum != NIHIL ? primum->codex : "-");
            }
            si (   !accepta && erratum.datum != NIHIL
                && _linea_bash(erratum) > ZEPHYRUM && primum != NIHIL)
            {
                lineae_comparatae++;
                si (primum->tractus.linea == _linea_bash(erratum))
                {
                    lineae_concordes++;
                }
                alioquin
                {
                    imprimere("    linea discors %.*s: bash %d, "
                        "crusta %d (%s)\n", (integer)c->clavis.mensura,
                        (constans character*)c->clavis.datum,
                        (integer)_linea_bash(erratum),
                        (integer)primum->tractus.linea, primum->codex);
                }
            }
        }
    }
    imprimere("    casus %d, aura %d: concordes %d, discordes %d, sine "
        "auro %d (pinna concordium %d)\n",
        (integer)xar_numerus(collectio), (integer)xar_numerus(aurum),
        (integer)concordes, (integer)discordes, (integer)sine_auro,
        (integer)PINNA_CONCORDIUM);
    si (concordes > (i32)PINNA_CONCORDIUM)
    {
        imprimere("    NOTA: concordes %d > pinna %d - pinnam "
            "attolle\n",
            (integer)concordes, (integer)PINNA_CONCORDIUM);
    }
    imprimere("    gravitas concordes %d (pinna %d), lineae %d/%d "
        "(pinna %d)\n", (integer)gravitas_concordes,
        (integer)PINNA_GRAVITATIS, (integer)lineae_concordes,
        (integer)lineae_comparatae, (integer)PINNA_LINEARUM);
    si (gravitas_concordes > (i32)PINNA_GRAVITATIS)
    {
        imprimere("    NOTA: gravitas concordes %d > pinna %d - "
            "pinnam attolle\n",
            (integer)gravitas_concordes, (integer)PINNA_GRAVITATIS);
    }
    si (lineae_concordes > (i32)PINNA_LINEARUM)
    {
        imprimere("    NOTA: lineae concordes %d > pinna %d - pinnam "
            "attolle\n",
            (integer)lineae_concordes, (integer)PINNA_LINEARUM);
    }
    CREDO_AEQUALIS_I32 (xar_numerus(collectio), (i32)CASUS_NUMERUS);
    CREDO_AEQUALIS_I32 (xar_numerus(aurum), (i32)AURA_NUMERUS);
    CREDO_AEQUALIS_I32 (concordes + discordes, (i32)AURA_NUMERUS);
    CREDO_VERUM (concordes >= (i32)PINNA_CONCORDIUM);
    CREDO_VERUM (gravitas_concordes >= (i32)PINNA_GRAVITATIS);
    CREDO_VERUM (lineae_concordes >= (i32)PINNA_LINEARUM);


    /* ==================================================
     * PROBARE: mala corporis domus
     * ================================================== */

    imprimere("\n--- Probans mala corporis domus ---\n");

    lista = crusta_oraculum_legere(piscina, radix,
        "build/crusta_corpus.lst", &mensura);
    si (lista == NIHIL)
    {
        CREDO_CULPA ("build/crusta_corpus.lst absens - cursor eam "
            "scribit");
    }
    alioquin
    {
        i32 ab = ZEPHYRUM;

        dum (ab < mensura)
        {
                  i32  ad = ab;
            character  via[1024];
              Piscina* propria;
            character* textus;
                  i32  longitudo = ZEPHYRUM;
        CrustaParsura  relatio;

            dum (ad < mensura && lista[ad] != '\n')
            {
                ad++;
            }
            si (ad == ab || ad - ab >= (i32)magnitudo(via))
            {
                ab = ad + I;
                perge;
            }
            memcpy(via, lista + ab, (size_t)(ad - ab));
            via[ad - ab]  = '\0';
            ab            = ad + I;
            /* fixa pathologica mala sua habent (porta corporis
             * eadem regula eas excludit; exempla et reservatio eas
             * pinnant) */
            si (   strcmp(via, "probationes/fixa/crusta/pathologiae.sh")
                    == ZEPHYRUM
                || strcmp(via, "probationes/fixa/crusta/adversarius.sh")
                    == ZEPHYRUM)
            {
                perge;
            }
            propria = piscina_generare_dynamicum("differentia_domus",
                1048576);
            si (propria == NIHIL)
            {
                CREDO_CULPA ("piscina domus");
                frange;
            }
            textus = crusta_oraculum_legere(propria, radix, via,
                &longitudo);
            si (textus == NIHIL)
            {
                imprimere("    plagula absens: %s\n", via);
                CREDO_CULPA ("plagula corporis absens");
                piscina_destruere(propria);
                perge;
            }
            plagulae++;
            si (   crusta_arbor_parsare(propria, textus, longitudo,
                    &CRUSTA_BASH, &relatio) != NIHIL
                && relatio.mala > ZEPHYRUM)
            {
                mala += relatio.mala;
                imprimere("    %s: mala %d\n", via,
                    (integer)relatio.mala);
            }
            piscina_destruere(propria);
        }
    }
    imprimere("    plagulae domus %d: mala %d (pinna %d, descendens)\n",
        (integer)plagulae, (integer)mala, (integer)MALA_DOMUS_PINNA);
    CREDO_VERUM (plagulae >= (i32)DOMUS_MINIMUM);
    CREDO_VERUM (mala <= (i32)MALA_DOMUS_PINNA);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
