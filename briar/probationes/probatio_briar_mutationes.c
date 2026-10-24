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


/* ==================================================
 * PROBARE: versio ex capite supremo
 * ================================================== */

interior vacuum
_versionem_probare (
    CredoContextus* c)
{
    Piscina* pn = c->piscina;
     chorda  charta;

    charta = chorda_ex_literis(CHARTA, pn);
    CREDO_AEQUALIS_I32 (briar_mutationes_versio(charta), XII);
    CREDO_CHORDA_AEQUALIS_LITERIS (briar_mutationes_caput(charta),
        "v12 \xe2\x80\x94 2026-10-01");
    CREDO_VERUM (briar_mutationes_ordo_rectus(charta));
}


/* ==================================================
 * PROBARE: capita prava
 * ================================================== */

interior vacuum
_capita_prava_probare (
    CredoContextus* c)
{
    Piscina* pn = c->piscina;

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
}


/* ==================================================
 * PROBARE: mutationes ineditae
 * ================================================== */

interior vacuum
_ineditas_probare (
    CredoContextus* c)
{
    Piscina* pn = c->piscina;
     chorda  charta;

    charta = chorda_ex_literis(CHARTA, pn);
    /* CHARTA: una sub inedita */
    CREDO_AEQUALIS_I32 (briar_mutationes_inedita(charta), I);
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
}


/* ==================================================
 * PROBARE: ordo capitum
 * ================================================== */

interior vacuum
_ordinem_probare (
    CredoContextus* c)
{
    Piscina* pn = c->piscina;

    /* ascendens, aequalis, saltus sursum: pravi */
    CREDO_FALSUM (briar_mutationes_ordo_rectus(chorda_ex_literis(
        "## v1\n## v2\n", pn)));
    CREDO_FALSUM (briar_mutationes_ordo_rectus(chorda_ex_literis(
        "## v2\n## v2\n", pn)));
    CREDO_FALSUM (briar_mutationes_ordo_rectus(chorda_ex_literis(
        "## v3\n## v1\n## v2\n", pn)));
    CREDO_VERUM (briar_mutationes_ordo_rectus(chorda_ex_literis(
        "## v1", pn)));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "versionem ex capite supremo", _versionem_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "capita prava", _capita_prava_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "mutationes ineditas", _ineditas_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ordinem", _ordinem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("briar_mutationes", SECTIONES);
}
