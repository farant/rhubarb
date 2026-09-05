/* briar_imperium.c - Vide briar_imperium.h. */

#include "briar_imperium.h"
#include "chorda_aedificator.h"
#include "sigillum.h"
#include "silex.h"
#include <stdlib.h>
#include <string.h>

interior b32
_est (
    constans character* a,
    constans character* b)
{
    redde (b32)(a != NIHIL && strcmp(a, b) == ZEPHYRUM);
}

/* vexillum actionis: actio aut -I */
interior s32
_actio_vexilli (
    constans character* a)
{
    si (_est(a, "-probatio"))
    {
        redde (s32)BRIAR_ACTIO_PROBATIO;
    }
    si (_est(a, "-struere"))
    {
        redde (s32)BRIAR_ACTIO_STRUERE;
    }
    si (_est(a, "-arbor"))
    {
        redde (s32)BRIAR_ACTIO_ARBOR;
    }
        si (_est(a, "-partes"))
        {
        redde (s32)BRIAR_ACTIO_PARTES;
        }
    si (_est(a, "-amalgama"))
    {
        redde (s32)BRIAR_ACTIO_AMALGAMA;
    }
    si (_est(a, "-versio"))
    {
        redde (s32)BRIAR_ACTIO_VERSIO;
    }
    redde -I;
}

interior vacuum
_recusare (
         BriarImperium* imp,
               Piscina* piscina,
    constans character* causa)
{
    imp->causa = chorda_ex_literis(causa, piscina);
}

b32
briar_imperium_legere (
                         Piscina* piscina,
                             i32  argc,
    constans character* constans* argv,
                   BriarImperium* imp)
{
    i32 i;
    b32 actio_data = FALSUM;

    memset(imp, 0, magnitudo(*imp));
    imp->actio = BRIAR_ACTIO_CURRERE;
    /* I. ante plagulam */
    per (i = I; i < argc && imp->via == NIHIL; i++)
    {
        constans character* a      = argv[i];
                       s32  actio  = _actio_vexilli(a);

        si (_est(a, "-h") || _est(a, "-auxilium"))
        {
            imp->actio = BRIAR_ACTIO_AUXILIUM;
            redde VERUM;
        }
        alioquin si (actio >= ZEPHYRUM)
        {
            si (actio_data)
            {
                _recusare(imp, piscina,
                    "vexilla duo actionis: unum elige");
                redde FALSUM;
            }
            imp->actio = (BriarActio)actio;
            actio_data = VERUM;
        }
        alioquin si (_est(a, "-iterum"))
        {
            imp->iterum = VERUM;
        }
        alioquin si (_est(a, "-f"))
        {
            si (   i + I >= argc || argv[i + I][0] == '-'
                || strstr(argv[i + I], ".thistle") != NIHIL)
            {
                _recusare(imp, piscina, "-f sine radice");
                redde FALSUM;
            }
            imp->fabrica  = argv[i + I];
            i             = i + I;
        }
        alioquin si (a[0] == '-' && a[1] != '\0')
        {
            ChordaAedificator* aed = chorda_aedificator_creare(piscina,
                (memoriae_index)128);

            chorda_aedificator_appendere_literis(aed,
                "vexillum ignotum: ");
            chorda_aedificator_appendere_literis(aed, a);
            chorda_aedificator_appendere_literis(aed,
                " (nota: -probatio -struere [-iterum] -arbor"
                " -partes -amalgama -versio -f <radix>)");
            imp->causa = chorda_aedificator_finire(aed);
            redde FALSUM;
        }
        alioquin
        {
            imp->via = a;
        }
    }
    si (imp->via == NIHIL)
    {
        si (imp->actio == BRIAR_ACTIO_VERSIO)
        {
            redde VERUM;   /* -versio sine plagula; -f iam lectum */
        }
        _recusare(imp, piscina, "plagula .thistle deest (usus: briar"
            " [-vexillum] <x.thistle> [argumenta])");
        redde FALSUM;
    }
    si (imp->iterum && imp->actio != BRIAR_ACTIO_STRUERE)
    {
        _recusare(imp, piscina, "-iterum solum cum -struere");
        redde FALSUM;
    }
    /* II. argumentum primum post plagulam: reservatum aut '--' */
    si (i < argc)
    {
        s32 actio = _actio_vexilli(argv[i]);

        si (_est(argv[i], "--"))
        {
            i = i + I;
        }
        alioquin si (actio >= ZEPHYRUM)
        {
            si (actio_data && (BriarActio)actio != imp->actio)
            {
                _recusare(imp, piscina,
                    "vexilla duo actionis: unum elige");
                redde FALSUM;
            }
            imp->actio  = (BriarActio)actio;
            i           = i + I;
            si (   imp->actio == BRIAR_ACTIO_STRUERE && i < argc
                && _est(argv[i], "-iterum"))
            {
                imp->iterum  = VERUM;
                i            = i + I;
            }
        }
    }
    /* III. reliqua (NIHIL-terminata) */
    imp->numerus_reliquorum = argc - i;
    {
        constans character** r = (constans character**)piscina_allocare(
            piscina, (memoriae_index)((imp->numerus_reliquorum + I)
                * (i32)magnitudo(constans character*)));
        i32 k;

        per (k = ZEPHYRUM; k < imp->numerus_reliquorum; k++)
        {
            r[k] = argv[i + k];
        }
        r[imp->numerus_reliquorum]  = NIHIL;
        imp->reliqua                = (constans character* constans*)r;
    }
    redde VERUM;
}

constans character*
briar_domus_proiecti (
               Piscina* piscina,
    constans character* titulus,
    constans character* sigillum_xvi)
{
    constans character* domus = getenv("HOME");
     ChordaAedificator* aed;

    si (domus == NIHIL || domus[0] == '\0')
    {
        redde NIHIL;
    }
    aed = chorda_aedificator_creare(piscina, (memoriae_index)256);
    chorda_aedificator_appendere_literis(aed, domus);
    chorda_aedificator_appendere_literis(aed, "/.rhubarb/briar/");
    chorda_aedificator_appendere_literis(aed, titulus);
    chorda_aedificator_appendere_literis(aed, "-");
    chorda_aedificator_appendere_literis(aed, sigillum_xvi);
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}

constans character*
briar_stampa_clausurae (
    Piscina* piscina,
        Xar* clausura)
{
    SigillumContextus  ctx;
             Sigillum  s;
            character  hex[SIGILLUM_HEX_MENSURA];
            character* r;
                  i32  i;

    sigillum_incipere(&ctx);
    per (i = ZEPHYRUM; i < xar_numerus(clausura); i++)
    {
        constans SilexRes* e =
            (constans SilexRes*)xar_obtinere(clausura,
            i);

        sigillum_addere(&ctx, e->via.datum,
            (memoriae_index)e->via.mensura);
        sigillum_addere(&ctx, "\n", (memoriae_index)I);
        sigillum_addere(&ctx, e->contentum.datum,
            (memoriae_index)e->contentum.mensura);
        sigillum_addere(&ctx, "\n", (memoriae_index)I);
    }
    s = sigillum_finire(&ctx);
    sigillum_hex(&s, hex);
    r = (character*)piscina_allocare(piscina, (memoriae_index)26);
    memcpy(r, "clausura:", (size_t)9);
    memcpy(r + 9, hex, (size_t)16);
    r[25] = '\0';
    redde r;
}

vacuum
briar_vexilla_sigillum (
    constans character* vexilla,
             character* hex_xvii)
{
     Sigillum s = sigillum_computare(vexilla,
         (memoriae_index)strlen(vexilla));
    character hex[SIGILLUM_HEX_MENSURA];

    sigillum_hex(&s, hex);
    memcpy(hex_xvii, hex, (size_t)16);
    hex_xvii[16] = '\0';
}
