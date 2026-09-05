/* probatio_briar_imperium.c - Regulae imperii: vexilla ante plagulam,
 * argumentum primum reservatum post eam (forma shebang), '--', -f,
 * recusationes, domus proiecti, stampa clausurae, sigillum vexillorum.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "briar_imperium.h"
#include "piscina.h"
#include "silex.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior b32
_legere (
               Piscina* piscina,
         BriarImperium* imp,
                   i32  n,
    constans character* a1,
    constans character* a2,
    constans character* a3)
{
    constans character* argv[5];

    argv[0] = "briar";
    argv[1] = a1;
    argv[2] = a2;
    argv[3] = a3;
    argv[4] = NIHIL;
    redde briar_imperium_legere(piscina, n + I, argv, imp);
}

s32
principale (vacuum)
{
              b32  praeteritus;
          Piscina* piscina;
    BriarImperium  imp;

    piscina = piscina_generare_dynamicum("probatio_briar_imperium",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans formae imperii ---\n");
    /* briar x.thistle a b : currere, reliqua II */
    CREDO_VERUM (_legere(piscina, &imp, III, "x.thistle", "a", "b"));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_CURRERE);
    CREDO_VERUM (strcmp(imp.via, "x.thistle") == ZEPHYRUM);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, II);
    CREDO_VERUM (strcmp(imp.reliqua[0], "a") == ZEPHYRUM);
    CREDO_VERUM (strcmp(imp.reliqua[1], "b") == ZEPHYRUM);
    CREDO_VERUM (imp.reliqua[2] == NIHIL);
    CREDO_VERUM (imp.fabrica == NIHIL);
    CREDO_FALSUM (imp.iterum);

    /* briar -probatio x.thistle */
    CREDO_VERUM (_legere(piscina, &imp, II, "-probatio", "x.thistle",
        NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_PROBATIO);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, ZEPHYRUM);

    /* forma shebang: briar ./x.thistle -probatio */
    CREDO_VERUM (_legere(piscina, &imp, II, "./x.thistle", "-probatio",
        NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_PROBATIO);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, ZEPHYRUM);

    /* briar -amalgama x.thistle; shebang ./x.thistle -amalgama */
    CREDO_VERUM (_legere(piscina, &imp, II, "-amalgama", "x.thistle",
        NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_AMALGAMA);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, ZEPHYRUM);
    CREDO_VERUM (_legere(piscina, &imp, II, "./x.thistle", "-amalgama",
        NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_AMALGAMA);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, ZEPHYRUM);

    /* briar x.thistle -- -probatio : '--' abscisum, vexillum
     * programmatis */
    CREDO_VERUM (_legere(piscina, &imp, III, "x.thistle", "--",
        "-probatio"));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_CURRERE);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, I);
    CREDO_VERUM (strcmp(imp.reliqua[0], "-probatio") == ZEPHYRUM);

    /* briar x.thistle -foo : alienum post plagulam = programmatis */
    CREDO_VERUM (_legere(piscina, &imp, II, "x.thistle", "-foo",
        NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_CURRERE);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, I);

    /* briar x.thistle -struere -iterum : reservatum cum -iterum */
    CREDO_VERUM (_legere(piscina, &imp, III, "x.thistle", "-struere",
        "-iterum"));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_STRUERE);
    CREDO_VERUM (imp.iterum);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, ZEPHYRUM);

    /* -struere -iterum ante plagulam */
    CREDO_VERUM (_legere(piscina, &imp, III, "-struere", "-iterum",
        "x.thistle"));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_STRUERE);
    CREDO_VERUM (imp.iterum);

    /* -f radix */
    CREDO_VERUM (_legere(piscina, &imp, III, "-f", "/tmp/rhubarb",
        "x.thistle"));
    CREDO_VERUM (strcmp(imp.fabrica, "/tmp/rhubarb") == ZEPHYRUM);
    CREDO_VERUM (strcmp(imp.via, "x.thistle") == ZEPHYRUM);

    /* -versio / -h sine plagula */
    CREDO_VERUM (_legere(piscina, &imp, I, "-versio", NIHIL, NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_VERSIO);
    /* -versio -f radix : -f post -versio legitur (ordo liber) */
    CREDO_VERUM (_legere(piscina, &imp, III, "-versio", "-f",
        "/tmp/r"));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_VERSIO);
    CREDO_VERUM (imp.fabrica != NIHIL && strcmp(imp.fabrica, "/tmp/r")
        == ZEPHYRUM);
    CREDO_VERUM (_legere(piscina, &imp, I, "-h", NIHIL, NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_AUXILIUM);

    imprimere("\n--- Probans recusationes ---\n");
    CREDO_FALSUM (_legere(piscina, &imp, II, "-ignotum", "x.thistle",
        NIHIL));
    CREDO_VERUM (imp.causa.mensura > ZEPHYRUM);
    CREDO_FALSUM (_legere(piscina, &imp, ZEPHYRUM, NIHIL, NIHIL,
        NIHIL));
    CREDO_FALSUM (_legere(piscina, &imp, I, "-probatio", NIHIL, NIHIL));
    CREDO_FALSUM (_legere(piscina, &imp, II, "-iterum", "x.thistle",
        NIHIL));
    CREDO_FALSUM (_legere(piscina, &imp, II, "-f", "x.thistle", NIHIL));
    CREDO_FALSUM (_legere(piscina, &imp, III, "-probatio", "-struere",
        "x.thistle"));

    imprimere("\n--- Probans domus, stampa, sigillum ---\n");
    {
        constans character* d;
                 character  hex[17];

        setenv("HOME", "/tmp/domus_ficta", 1);
        d = briar_domus_proiecti(piscina, "salve", "0123456789abcdef");
        CREDO_NON_NIHIL (d);
        CREDO_VERUM (strcmp(d,
            "/tmp/domus_ficta/.rhubarb/briar/salve-0123456789abcdef")
            == ZEPHYRUM);
        unsetenv("HOME");
        CREDO_VERUM (briar_domus_proiecti(piscina, "salve", "0")
            == NIHIL);

        briar_vexilla_sigillum("-Wall", hex);
        CREDO_AEQUALIS_I32 ((i32)strlen(hex), (i32)16);
        {
                           Xar* clausura = xar_creare(piscina,
                               (i32)magnitudo(SilexRes));
                      SilexRes* r = (SilexRes*)xar_addere(clausura);
            constans character* s1;
            constans character* s2;

            r->via        = chorda_ex_literis("include/a.h", piscina);
            r->contentum  = chorda_ex_literis("int a;", piscina);
            r->origo      = "probatio";
            s1            = briar_stampa_clausurae(piscina, clausura);
            CREDO_VERUM (strncmp(s1, "clausura:", (size_t)9)
                == ZEPHYRUM);
            CREDO_AEQUALIS_I32 ((i32)strlen(s1), (i32)25);
            r->contentum  = chorda_ex_literis("int b;", piscina);
            s2            = briar_stampa_clausurae(piscina, clausura);
            CREDO_VERUM (strcmp(s1, s2) != ZEPHYRUM);
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
