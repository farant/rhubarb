/* probatio_briar_mutationes.c - versio ex charta mutationum
 * (caput supremum '## vN'), ordo capitum descendens */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "briar_mutationes.h"
#include <stdio.h>

interior constans character* CHARTA =
    "# Mutationes briar\n"
    "\n"
    "Introductio; '## v9' in textu non caput est.\n"
    "\n"
    "## inedita\n"
    "\n"
    "- nondum edita\n"
    "\n"
    "## v12 \xe2\x80\x94 2026-10-01  \n"
    "\n"
    "- duodecima\n"
    "\n"
    "## v3 \xe2\x80\x94 2026-09-24\n"
    "\n"
    "### v99 subcaput non numeratur\n"
    "\n"
    "## v1\n";

s32
principale (vacuum)
{
     Piscina* pn;
         b32  praeteritus;
      chorda  c;

    pn = piscina_generare_dynamicum("probatio_briar_mutationes",
        65536);
    si (!pn)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(pn);
    c = chorda_ex_literis(CHARTA, pn);

    imprimere("\n--- Probans versionem ex capite supremo ---\n");
    CREDO_AEQUALIS_I32 (briar_mutationes_versio(c), XII);
    CREDO_CHORDA_AEQUALIS_LITERIS (briar_mutationes_caput(c),
        "v12 \xe2\x80\x94 2026-10-01");
    CREDO_VERUM (briar_mutationes_ordo_rectus(c));

    imprimere("\n--- Probans capita prava ---\n");
    /* 'vN' sine spatio post numerum non caput versionis */
    CREDO_AEQUALIS_I32 (briar_mutationes_versio(chorda_ex_literis(
        "## v2x\n## v1\n", pn)), I);
    /* nullum caput */
    CREDO_AEQUALIS_I32 (briar_mutationes_versio(chorda_ex_literis(
        "# titulus\n## inedita\n", pn)), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (briar_mutationes_caput(chorda_ex_literis(
        "", pn)).mensura, ZEPHYRUM);
    CREDO_FALSUM (briar_mutationes_ordo_rectus(chorda_ex_literis(
        "## inedita\n", pn)));
    /* v0 non versio */
    CREDO_AEQUALIS_I32 (briar_mutationes_versio(chorda_ex_literis(
        "## v0\n", pn)), ZEPHYRUM);

    imprimere("\n--- Probans mutationes ineditas ---\n");
    /* CHARTA: una sub inedita */
    CREDO_AEQUALIS_I32 (briar_mutationes_inedita(c), I);
    CREDO_AEQUALIS_I32 (briar_mutationes_inedita(chorda_ex_literis(
        "## inedita\n\n- a\n  continuata\n- b\n\n## v1\n- c\n", pn)),
        II);
    CREDO_AEQUALIS_I32 (briar_mutationes_inedita(chorda_ex_literis(
        "## inedita\n\n## v1\n- c\n", pn)), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (briar_mutationes_inedita(chorda_ex_literis(
        "## v1\n- c\n", pn)), ZEPHYRUM);
    /* ad finem sine capite sequente */
    CREDO_AEQUALIS_I32 (briar_mutationes_inedita(chorda_ex_literis(
        "## inedita\n- a", pn)), I);

    imprimere("\n--- Probans ordinem ---\n");
    /* ascendens, aequalis, saltus sursum: pravi */
    CREDO_FALSUM (briar_mutationes_ordo_rectus(chorda_ex_literis(
        "## v1\n## v2\n", pn)));
    CREDO_FALSUM (briar_mutationes_ordo_rectus(chorda_ex_literis(
        "## v2\n## v2\n", pn)));
    CREDO_FALSUM (briar_mutationes_ordo_rectus(chorda_ex_literis(
        "## v3\n## v1\n## v2\n", pn)));
    CREDO_VERUM (briar_mutationes_ordo_rectus(chorda_ex_literis(
        "## v1", pn)));

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(pn);
    redde praeteritus ? ZEPHYRUM : I;
}
