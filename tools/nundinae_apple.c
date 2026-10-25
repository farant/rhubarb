/* nundinae_apple.c - instrumentum: cliens App Store Connect ex linea
 * mandatorum (testa-ios C2, plan R5). Verba (T1): -status,
 * -applicationes. Configuratio: -configuratio <via>, aliter
 * $HOME/.rhubarb/ios/nundinae_apple.stml. Clavis secreta numquam
 * imprimitur. Exitus: 0 | 1 fractum (Apple aut rete) | 2 recusatio
 * (configuratio, clavis, usus) | 3 pactum Apple non acceptum (omnis
 * petitio refutatur donec accipitur). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "nundinae_apple.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior s32
_usus (
    vacuum)
{
    fprintf(stderr, "usus: nundinae_apple [-configuratio <via>] "
        "-status | -applicationes\n");
    redde II;
}

/* causa nuntiata; pactum non acceptum -> monitum clarum et III */
interior s32
_fractum (
     chorda  causa,
    Piscina* p)
{
    fprintf(stderr, "nundinae_apple: %.*s\n", (int)causa.mensura,
        (constans character*)causa.datum);
    si (chorda_continet(causa, chorda_ex_literis(
            NUNDINAE_APPLE_CODEX_PACTUM, p)))
    {
        fprintf(stderr, "nundinae_apple: PACTUM APPLE NON ACCEPTUM - "
            "dominus rationis pacta accipiat: "
            "developer.apple.com/account "
            "(Program License Agreement) et App Store Connect > "
            "Business; deinde iterum\n");
        redde III;
    }
    redde I;
}

/* -status: petitio levissima; "status: bonus" aut causa */
interior s32
_status (
    NundinaeApple* n,
          Piscina* p)
{
    NundinaeAppleResponsum r;

    si (nundinae_apple_statum_probare(n, &r, p))
    {
        imprimere("status: bonus\n");
        redde ZEPHYRUM;
    }
    redde _fractum(r.causa, p);
}

interior s32
_applicationes (
    NundinaeApple* n,
          Piscina* p)
{
     Xar* applicationes;
  chorda  causa;
     i32  i;

    applicationes = xar_creare(p,
        (i32)magnitudo(NundinaeAppleApplicatio));
    si (!nundinae_apple_applicationes(n, applicationes, &causa, p))
    {
        redde _fractum(causa, p);
    }
    per (i = ZEPHYRUM; i < xar_numerus(applicationes); i++)
    {
        NundinaeAppleApplicatio* a;

        a = (NundinaeAppleApplicatio*)xar_obtinere(applicationes, i);
        imprimere("%.*s\t%.*s\t%.*s\n", (int)a->id.mensura,
            (constans character*)a->id.datum,
            (int)a->signum_fasciculi.mensura,
            (constans character*)a->signum_fasciculi.datum,
            (int)a->titulus.mensura,
            (constans character*)a->titulus.datum);
    }
    redde ZEPHYRUM;
}

s32
principale (
      integer   argc,
    character** argv)
{
                     Piscina* p;
   NundinaeAppleConfiguratio  cfg;
               NundinaeApple  n;
                      chorda  causa;
                   character  via_cfg[DXII];
          constans character* verbum;
                     integer  i;
                         s32  exitus;

    p = piscina_generare_dynamicum("nundinae_apple", M * M);
    si (!p)
    {
        redde I;
    }
    via_cfg[ZEPHYRUM]  = '\0';
    verbum             = NIHIL;
    per (i = I; i < argc; i++)
    {
        si (   strcmp(argv[i], "-configuratio") == ZEPHYRUM
            && i + I < argc)
        {
            i++;
            si (strlen(argv[i]) >= magnitudo(via_cfg))
            {
                redde _usus();
            }
            strcpy(via_cfg, argv[i]);
        }
        alioquin si (argv[i][ZEPHYRUM] == '-' && !verbum)
        {
            verbum = argv[i];
        }
        alioquin
        {
            redde _usus();
        }
    }
    si (!verbum)
    {
        redde _usus();
    }
    si (via_cfg[ZEPHYRUM] == '\0')
    {
        constans character* domus = getenv("HOME");

        si (!domus || strlen(domus) > CD)
        {
            fprintf(stderr, "nundinae_apple: HOME nullum\n");
            redde II;
        }
        sprintf(via_cfg, "%s/.rhubarb/ios/nundinae_apple.stml", domus);
    }
    si (!nundinae_apple_configurationem_legere(via_cfg, &cfg, &causa, p)
        || !nundinae_apple_aperire(&n, &cfg, &causa, p))
    {
        fprintf(stderr, "%.*s\n", (int)causa.mensura,
            (constans character*)causa.datum);
        redde II;
    }
    si (strcmp(verbum, "-status") == ZEPHYRUM)
    {
        exitus = _status(&n, p);
    }
    alioquin si (strcmp(verbum, "-applicationes") == ZEPHYRUM)
    {
        exitus = _applicationes(&n, p);
    }
    alioquin
    {
        exitus = _usus();
    }
    nundinae_apple_claudere(&n);
    piscina_destruere(p);
    redde exitus;
}
