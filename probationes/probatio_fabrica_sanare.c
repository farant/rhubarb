/* probatio_fabrica_sanare.c - probationes fabricae (fabrica-6 H3),
 * pars sanare: seriatim, parallele, cursus
 *
 * Mundus fictus (discus in memoria, sutura ficta):
 * probationes/fabrica_mundus_fictus.h. Spec:
 * project-specs/fabrica-spec-v2.md et sequentes. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "sigillum.h"
#include "internamentum.h"
#include "fabrica.h"
#include "credo.h"
#include "fabrica_mundus_fictus.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_chordam_addere_test (
                   Xar* xar,
    constans character* valor,
               Piscina* piscina)
{
    *(chorda*)xar_addere(xar) = chorda_ex_literis(valor, piscina);
}

/* eventus sanationis tituli; M si nulla (probatio non ruit) */
interior i32
_eventus_sanationis (
                   Xar* sanationes,
    constans character* titulus)
{
    FabricaSanatio* sanatio;

    sanatio = mundi_sanatio_invenire(sanationes, titulus);
    redde (sanatio != NIHIL) ? (i32)sanatio->eventus : (i32)M;
}

/* causa sanationis tituli; vacua si nulla */
interior chorda
_causa_sanationis (
                   Xar* sanationes,
    constans character* titulus,
               Piscina* piscina)
{
    FabricaSanatio* sanatio;

    sanatio = mundi_sanatio_invenire(sanationes, titulus);
    redde (sanatio != NIHIL) ? sanatio->causa
        : chorda_ex_literis("", piscina);
}


/* ==================================================
 * PROBARE: sanare (plan 1b T3, Review Focus 2-4)
 * ================================================== */

interior vacuum
_probare_sanare (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
          DiscusFictus  discus;
         FabricaSutura  sutura;
         FabricaSutura  nuda;
          FabricaActio* actiones[III];
          FabricaActio* b;
                   Xar* ordo;
                   Xar* sanationes;
                   Xar* electa;
        FabricaSanatio* sanatio;
                chorda  causa;
              Sigillum  sigillum;
             character  hex[SIGILLUM_HEX_MENSURA];
             character* relatio_vetus;
             character* relatio_nova;
                   i32  i;
                   i32  praeparata;

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;

        /* I. sanatum: X stalum, agere in loco scribit, iudicium post
         * RECENS - et VERE iterum iudicatum (regeneratio iterum
         * currit: memoria regenerationum vacata) */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        ordo = mundi_ordinare_fictas(piscina, actiones, I);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_AEQUALIS_I32(xar_numerus(sanationes), I);
        sanatio = mundi_sanatio_invenire(sanationes, "A");
        CREDO_NON_NIHIL(sanatio);
        CREDO_AEQUALIS_I32((i32)sanatio->eventus, (i32)FABRICA_SANATUM);
        CREDO_VERUM(mundi_contentum_est(&discus, "X", "novum\n"));
        CREDO_AEQUALIS_I32(discus.acta, I);
        CREDO_AEQUALIS_I32(discus.cursus, II);

        /* II. memoria per cursum PURGATA (Review Focus 2): A ingressum
         * suum X legit (praelatio) - sigillum X vetus memoratur; B
         * (binarium, relatio) X legit. Sine purgatione B sigillo VETERE
         * iudicaretur, relationi veteri congrueret, numquam
         * ageretur. */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_b", "bin/b", NIHIL,
            "binarium\n",
            0, FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        mundi_ingressum_addere(actiones[0], "fasciculus", "X", piscina);
        b = mundi_actio_scripta(piscina, "B", "gen_b", "X", "bin/b",
            "relatio");
        actiones[1]  = b;
        nuda         = sutura;   /* sine memoriis: sigilla vera */
        CREDO_VERUM(fabrica_actionem_sigillare(&nuda, b, piscina,
            &sigillum, &causa));
        sigillum_hex(&sigillum, hex);
        relatio_vetus = (character*)piscina_allocare(piscina, 256);
        sprintf(relatio_vetus, "provenientia 1\ningressus %s\n", hex);
        mundi_ponere(&discus, "X", "novum\n");
        CREDO_VERUM(fabrica_actionem_sigillare(&nuda, b, piscina,
            &sigillum, &causa));
        sigillum_hex(&sigillum, hex);
        relatio_nova = (character*)piscina_allocare(piscina, 256);
        sprintf(relatio_nova, "provenientia 1\ningressus %s\n", hex);
        mundi_ponere(&discus, "X", "vetus\n");
        discus.relatio = relatio_vetus;
        ((ScriptumFictum*)xar_obtinere(discus.scripta, I))->relatio =
            relatio_nova;
        mundi_memorias_parare(&sutura, piscina);
        ordo = mundi_ordinare_fictas(piscina, actiones, II);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        sanatio = mundi_sanatio_invenire(sanationes, "B");
        CREDO_NON_NIHIL(sanatio);
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_SANATUM);
        CREDO_AEQUALIS_I32(discus.acta, II);

        /* III. post-condicio (Review Focus 3): exitus 0, nihil
         * scriptum -> FRACTUM, numquam SANATUM */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            0,
            VERUM);
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        ordo = mundi_ordinare_fictas(piscina, actiones, I);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "A");
        CREDO_NON_NIHIL(sanatio);
        CREDO_AEQUALIS_I32((i32)sanatio->eventus, (i32)FABRICA_FRACTUM);
        CREDO_VERUM(mundi_continet(sanatio->causa,
            "exitus 0 sed non RECENS",
            piscina));

        /* IV. dependentia fracta (Review Focus 4): A frangitur, C (ex
         * X) OMISSUM nominans A, D independens SANATUM */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "vetus\n");
        mundi_ponere(&discus, "Y", "C:alienum\n");
        mundi_ponere(&discus, "Z", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            I,
            VERUM);
        mundi_scriptum_addere(&discus, "gen_c", "Y", "X", "C:", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_d", "Z", NIHIL, "novum\n",
            0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "C", "gen_c", "X",
            "Y",
            "regeneratio");
        actiones[2] = mundi_actio_scripta(piscina, "D", "gen_d", "a",
            "Z",
            "regeneratio");
        ordo = mundi_ordinare_fictas(piscina, actiones, III);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "A");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_FRACTUM
            && mundi_continet(sanatio->causa, "error ficti acti",
            piscina));
        sanatio = mundi_sanatio_invenire(sanationes, "C");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_OMISSUM
            && mundi_continet(sanatio->causa, "A", piscina));
        sanatio = mundi_sanatio_invenire(sanationes, "D");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_SANATUM);
        CREDO_AEQUALIS_I32(discus.acta, II);
        CREDO_VERUM(mundi_contentum_est(&discus, "Y", "C:alienum\n"));

        /* V. praecondicio ignota SEMEL ante dependentes duas; ordo
         * declarationis (P1, P2, O) - ordinare O primam ponit */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "p1", "vetus\n");
        mundi_ponere(&discus, "p2", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_o", "build/o", NIHIL,
            "obiecta\n",
            0, FALSUM);
        mundi_scriptum_addere(&discus, "gen_p1", "p1", NIHIL, "novum\n",
            0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_p2", "p2", NIHIL, "novum\n",
            0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "P1", "gen_p1", "a",
            "p1",
            "regeneratio");
        _chordam_addere_test(actiones[0]->praecondiciones, "O",
            piscina);
        actiones[1] = mundi_actio_scripta(piscina, "P2", "gen_p2", "a",
            "p2",
            "regeneratio");
        _chordam_addere_test(actiones[1]->praecondiciones, "O",
            piscina);
        actiones[2] = mundi_actio_scripta(piscina, "O", "gen_o", "a",
            "build/o", "ignota");
        ordo = mundi_ordinare_fictas(piscina, actiones, III);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        praeparata = ZEPHYRUM;
        per (i = ZEPHYRUM; sanationes != NIHIL
             && i < xar_numerus(sanationes); i++)
        {
            si (((FabricaSanatio*)xar_obtinere(sanationes,
                    i))->eventus == FABRICA_PRAEPARATUM)
            {
                praeparata++;
            }
        }
        CREDO_AEQUALIS_I32(praeparata, I);
        CREDO_VERUM(mundi_contentum_est(&discus, "build/o",
            "obiecta\n"));
        CREDO_AEQUALIS_I32(discus.acta, III);
        sanatio = mundi_sanatio_invenire(sanationes, "P2");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_SANATUM);

        /* VI. electa: solum Z et quae supra eam - A (X) intacta */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "vetus\n");
        mundi_ponere(&discus, "Z", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_d", "Z", NIHIL, "novum\n",
            0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "D", "gen_d", "a",
            "Z",
            "regeneratio");
        ordo = mundi_ordinare_fictas(piscina, actiones, II);
        electa = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(electa) = chorda_ex_literis("Z", piscina);
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_AEQUALIS_I32(xar_numerus(sanationes), I);
        CREDO_NIHIL(mundi_sanatio_invenire(sanationes, "A"));
        CREDO_VERUM(mundi_contentum_est(&discus, "X", "vetus\n"));
        CREDO_AEQUALIS_I32(discus.acta, I);

        /* VII. siccum: A AGENDUM, C (ex X, nunc recens) FORTASSE;
         * nihil agitur */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "vetus\n");
        mundi_ponere(&discus, "Y", "C:vetus\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_c", "Y", "X", "C:", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "C", "gen_c", "X",
            "Y",
            "regeneratio");
        ordo = mundi_ordinare_fictas(piscina, actiones, II);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, VERUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "A");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_AGENDUM);
        sanatio = mundi_sanatio_invenire(sanationes, "C");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_FORTASSE);
        CREDO_AEQUALIS_I32(discus.acta, ZEPHYRUM);
        CREDO_VERUM(mundi_contentum_est(&discus, "X", "vetus\n"));

        /* VIII. nihil stalum: Xar vacua (non NIHIL), nihil actum */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "novum\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        ordo = mundi_ordinare_fictas(piscina, actiones, I);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_AEQUALIS_I32(xar_numerus(sanationes), ZEPHYRUM);
        CREDO_AEQUALIS_I32(discus.acta, ZEPHYRUM);

        /* IX. electa a nulla actione producta -> NIHIL nominatum */
        electa = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(electa) = chorda_ex_literis("nusquam",
            piscina);
        CREDO_NIHIL(fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa));
        CREDO_VERUM(mundi_continet(causa, "nusquam", piscina));
    }
}


/* ==================================================
 * PROBARE: sanare PARALLELE (plan 2 T6)
 * ================================================== */

interior vacuum
_probare_sanare_parallele (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
          DiscusFictus  discus;
         FabricaSutura  sutura;
          FabricaActio* actiones[V];
                   Xar* ordo;
                   Xar* sanationes;
        FabricaSanatio* sanatio;
                chorda  causa;
                   s32  index_p1;
                   s32  index_d;
                   i32  i;

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;

        /* I. tres tutae (lectiones) independentes + una non tuta +
         * dependens P1: tres in unda una (ordine tituli), U SOLA per
         * agere, D post P1 */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul       = mundi_agere_simul;
        sutura.vestigium_capere  = mundi_vestigium_capere;
        mundi_ponere(&discus, "a3", "a3\n");
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "u", "u\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_ponere(&discus, "X3", "vetus\n");
        mundi_ponere(&discus, "XU", "vetus\n");
        mundi_ponere(&discus, "XD", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_3", "X3", NIHIL, "n3\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_u", "XU", NIHIL, "nu\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_d", "XD", NIHIL, "nd\n", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "P3", "gen_3", "a3",
            "X3",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1",
            "regeneratio");
        actiones[2] = mundi_actio_scripta(piscina, "P2", "gen_2", "a2",
            "X2",
            "regeneratio");
        actiones[3] = mundi_actio_scripta(piscina, "U", "gen_u", "u",
            "XU",
            "regeneratio");
        actiones[4] = mundi_actio_scripta(piscina, "D", "gen_d", "X1",
            "XD",
            "regeneratio");
        actiones[0]->lectiones = VERUM;
        actiones[1]->lectiones = VERUM;
        actiones[2]->lectiones = VERUM;
        actiones[4]->lectiones = VERUM;
        ordo = mundi_ordinare_fictas(piscina, actiones, V);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_AEQUALIS_I32(xar_numerus(discus.undae_actae), I);
        si (xar_numerus(discus.undae_actae) == I)
        {
            CREDO_VERUM(chorda_aequalis_literis(*(chorda*)xar_obtinere(
                discus.undae_actae, ZEPHYRUM), "P1 P2 P3"));
        }
        CREDO_AEQUALIS_I32(discus.acta_simul, III);
        /* U et D seriatim (non tuta; tuta sola in unda sua) */
        CREDO_AEQUALIS_I32(discus.acta - discus.acta_simul, II);
        index_p1  = -I;
        index_d   = -I;
        per (i = ZEPHYRUM; sanationes != NIHIL
             && i < xar_numerus(sanationes); i++)
        {
            sanatio = (FabricaSanatio*)xar_obtinere(sanationes, i);
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_SANATUM);
            /* via undarum quoque causam stali fert (plan-5 T1) */
            CREDO_VERUM(sanatio->stalum.mensura > ZEPHYRUM);
            si (chorda_aequalis_literis(sanatio->actio->titulus, "P1"))
            {
                index_p1 = (s32)i;
            }
            si (chorda_aequalis_literis(sanatio->actio->titulus, "D"))
            {
                index_d = (s32)i;
            }
        }
        CREDO_VERUM(index_p1 >= ZEPHYRUM && index_d > index_p1);
        CREDO_VERUM(mundi_contentum_est(&discus, "X2", "n2\n"));

        /* II. fractura in unda: P2 fractum, P1 P3 SANATA, D (ex X2)
         * OMISSUM */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul = mundi_agere_simul;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "a3", "a3\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_ponere(&discus, "X3", "vetus\n");
        mundi_ponere(&discus, "XD", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", I,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_3", "X3", NIHIL, "n3\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_d", "XD", NIHIL, "nd\n", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "P2", "gen_2", "a2",
            "X2",
            "regeneratio");
        actiones[2] = mundi_actio_scripta(piscina, "P3", "gen_3", "a3",
            "X3",
            "regeneratio");
        actiones[3] = mundi_actio_scripta(piscina, "D", "gen_d", "X2",
            "XD",
            "regeneratio");
        per (i = ZEPHYRUM; i < IV; i++)
        {
            actiones[i]->lectiones = VERUM;
        }
        ordo = mundi_ordinare_fictas(piscina, actiones, IV);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        si (sanationes != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)mundi_sanatio_invenire(sanationes,
                "P1")->eventus, (i32)FABRICA_SANATUM);
            CREDO_AEQUALIS_I32((i32)mundi_sanatio_invenire(sanationes,
                "P2")->eventus, (i32)FABRICA_FRACTUM);
            CREDO_AEQUALIS_I32((i32)mundi_sanatio_invenire(sanationes,
                "P3")->eventus, (i32)FABRICA_SANATUM);
            CREDO_AEQUALIS_I32((i32)mundi_sanatio_invenire(sanationes,
                "D")->eventus, (i32)FABRICA_OMISSUM);
        }

        /* III. scriptura 'S' extra vestigium membri: id solum FRACTUM */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul = mundi_agere_simul;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", 0,
            FALSUM);
        ((ScriptumFictum*)xar_obtinere(discus.scripta, ZEPHYRUM))
            ->scriptum_s = "alienum";
        actiones[0] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "P2", "gen_2", "a2",
            "X2",
            "regeneratio");
        actiones[0]->lectiones = VERUM;
        actiones[1]->lectiones = VERUM;
        ordo = mundi_ordinare_fictas(piscina, actiones, II);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        si (sanationes != NIHIL)
        {
            sanatio = mundi_sanatio_invenire(sanationes, "P1");
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
            CREDO_VERUM(mundi_continet(sanatio->causa, "alienum",
                piscina));
            CREDO_AEQUALIS_I32((i32)mundi_sanatio_invenire(sanationes,
                "P2")->eventus, (i32)FABRICA_SANATUM);
        }

        /* IIIb. scriptura in THESAURO (cache communis fabricae, plan 2
         * T3): involucrum, non vitium - inventum in arbore vera
         * (fragmenta per aedilis --thesaurus) */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul = mundi_agere_simul;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", 0,
            FALSUM);
        ((ScriptumFictum*)xar_obtinere(discus.scripta, ZEPHYRUM))
            ->scriptum_s = "build/aedilis/obiecta/generationes/g.lst";
        actiones[0] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "P2", "gen_2", "a2",
            "X2",
            "regeneratio");
        actiones[0]->lectiones = VERUM;
        actiones[1]->lectiones = VERUM;
        ordo = mundi_ordinare_fictas(piscina, actiones, II);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        si (sanationes != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)mundi_sanatio_invenire(sanationes,
                "P1")->eventus, (i32)FABRICA_SANATUM);
            CREDO_AEQUALIS_I32((i32)mundi_sanatio_invenire(sanationes,
                "P2")->eventus, (i32)FABRICA_SANATUM);
        }

        /* V. sanare cum praevisione (T6b): iudicia ante et post undam
         * simul - regenerationes tutae in undis currendi, nulla
         * seriatim (cursus = II undae x III) */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul    = mundi_agere_simul;
        sutura.currere_simul  = mundi_currere_simul;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "a3", "a3\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_ponere(&discus, "X3", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_3", "X3", NIHIL, "n3\n", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "P2", "gen_2", "a2",
            "X2",
            "regeneratio");
        actiones[2] = mundi_actio_scripta(piscina, "P3", "gen_3", "a3",
            "X3",
            "regeneratio");
        per (i = ZEPHYRUM; i < III; i++)
        {
            actiones[i]->lectiones = VERUM;
        }
        ordo = mundi_ordinare_fictas(piscina, actiones, III);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_AEQUALIS_I32(xar_numerus(discus.undae_currendi), II);
        CREDO_AEQUALIS_I32(discus.cursus, VI);
        per (i = ZEPHYRUM; sanationes != NIHIL
             && i < xar_numerus(sanationes); i++)
        {
            CREDO_AEQUALIS_I32((i32)((FabricaSanatio*)xar_obtinere(
                sanationes, i))->eventus, (i32)FABRICA_SANATUM);
        }

        /* IV. scriptura NON notata extra omnia vestigia undae
         * (photographia undae): membra omnia FRACTUM, unda nominata */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul       = mundi_agere_simul;
        sutura.vestigium_capere  = mundi_vestigium_capere;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", 0,
            FALSUM);
        ((ScriptumFictum*)xar_obtinere(discus.scripta, I))->alia =
            "tacitum";
        actiones[0] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "P2", "gen_2", "a2",
            "X2",
            "regeneratio");
        actiones[0]->lectiones = VERUM;
        actiones[1]->lectiones = VERUM;
        ordo = mundi_ordinare_fictas(piscina, actiones, II);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        si (sanationes != NIHIL)
        {
            sanatio = mundi_sanatio_invenire(sanationes, "P1");
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
            CREDO_VERUM(mundi_continet(sanatio->causa, "tacitum",
                piscina));
            CREDO_VERUM(mundi_continet(sanatio->causa, "unda",
                piscina));
            CREDO_AEQUALIS_I32((i32)mundi_sanatio_invenire(sanationes,
                "P2")->eventus, (i32)FABRICA_FRACTUM);
        }

        /* VI. post fracturam membra ultra fila non incipiunt: P1
         * fractum, P2 P3 OMISSUM 'non incepta' (fabrica-6, ante
         * _undam_agere per gradus) */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul  = mundi_agere_simul;
        discus.fila_ficta   = I;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "a3", "a3\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_ponere(&discus, "X3", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", I,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_3", "X3", NIHIL, "n3\n", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1", "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "P2", "gen_2", "a2",
            "X2", "regeneratio");
        actiones[2] = mundi_actio_scripta(piscina, "P3", "gen_3", "a3",
            "X3", "regeneratio");
        per (i = ZEPHYRUM; i < III; i++)
        {
            actiones[i]->lectiones = VERUM;
        }
        ordo = mundi_ordinare_fictas(piscina, actiones, III);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_AEQUALIS_I32(discus.acta_simul, I);
        si (sanationes != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)mundi_sanatio_invenire(sanationes,
                "P1")->eventus, (i32)FABRICA_FRACTUM);
            sanatio = mundi_sanatio_invenire(sanationes, "P2");
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_OMISSUM);
            CREDO_VERUM(mundi_continet(sanatio->causa, "non incepta",
                piscina));
            CREDO_AEQUALIS_I32((i32)mundi_sanatio_invenire(sanationes,
                "P3")->eventus, (i32)FABRICA_OMISSUM);
        }

        /* VII. membrum non actum (scriptum nullum: non incipit, cauda
         * cum causa): FRACTUM 'non actum', frater SANATUM */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul = mundi_agere_simul;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1", "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "P2", "gen_nullum",
            "a2", "X2", "regeneratio");
        actiones[0]->lectiones = VERUM;
        actiones[1]->lectiones = VERUM;
        ordo = mundi_ordinare_fictas(piscina, actiones, II);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        si (sanationes != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)mundi_sanatio_invenire(sanationes,
                "P1")->eventus, (i32)FABRICA_SANATUM);
            sanatio = mundi_sanatio_invenire(sanationes, "P2");
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
            CREDO_VERUM(mundi_continet(sanatio->causa, "non actum",
                piscina));
            CREDO_VERUM(mundi_continet(sanatio->causa,
                "scriptum nullum",
                piscina));
        }

        /* VIII. scripturae II non notatae extra vestigia undae: causa
         * primam nominat et '+I' reliquarum */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul       = mundi_agere_simul;
        sutura.vestigium_capere  = mundi_vestigium_capere;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", 0,
            FALSUM);
        ((ScriptumFictum*)xar_obtinere(discus.scripta,
            ZEPHYRUM))->alia =
            "tacitum_a";
        ((ScriptumFictum*)xar_obtinere(discus.scripta, I))->alia =
            "tacitum_b";
        actiones[0] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1", "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "P2", "gen_2", "a2",
            "X2", "regeneratio");
        actiones[0]->lectiones = VERUM;
        actiones[1]->lectiones = VERUM;
        ordo = mundi_ordinare_fictas(piscina, actiones, II);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        si (sanationes != NIHIL)
        {
            sanatio = mundi_sanatio_invenire(sanationes, "P2");
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
            CREDO_VERUM(mundi_continet(sanatio->causa, "tacitum_a +I",
                piscina));
            CREDO_VERUM(mundi_continet(sanatio->causa,
                "scriptor ignotus", piscina));
        }

        /* IX. fractura in unda SISTIT undas sequentes (fabrica-6, ante
         * _sanare_per_undas per gradus): D (ex X2 fracto) OMISSUM
         * 'dependentia fracta', W (ex X1 sano) OMISSUM 'post
         * fracturam', R recens tacet (nulla sanatio) */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul = mundi_agere_simul;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_ponere(&discus, "XD", "vetus\n");
        mundi_ponere(&discus, "XW", "vetus\n");
        mundi_ponere(&discus, "XR", "nr\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", I,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_d", "XD", NIHIL, "nd\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_w", "XW", NIHIL, "nw\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_r", "XR", NIHIL, "nr\n", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1", "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "P2", "gen_2", "a2",
            "X2", "regeneratio");
        actiones[2] = mundi_actio_scripta(piscina, "D", "gen_d", "X2",
            "XD", "regeneratio");
        actiones[3] = mundi_actio_scripta(piscina, "W", "gen_w", "X1",
            "XW", "regeneratio");
        actiones[4] = mundi_actio_scripta(piscina, "R", "gen_r", "X1",
            "XR", "regeneratio");
        per (i = ZEPHYRUM; i < V; i++)
        {
            actiones[i]->lectiones = VERUM;
        }
        ordo = mundi_ordinare_fictas(piscina, actiones, V);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        si (sanationes != NIHIL)
        {
            CREDO_AEQUALIS_I32(_eventus_sanationis(sanationes, "P2"),
                (i32)FABRICA_FRACTUM);
            CREDO_AEQUALIS_I32(_eventus_sanationis(sanationes, "D"),
                (i32)FABRICA_OMISSUM);
            CREDO_VERUM(mundi_continet(_causa_sanationis(sanationes,
                "D", piscina), "dependentia fracta", piscina));
            CREDO_AEQUALIS_I32(_eventus_sanationis(sanationes, "W"),
                (i32)FABRICA_OMISSUM);
            CREDO_VERUM(mundi_continet(_causa_sanationis(sanationes,
                "W", piscina), "post fracturam", piscina));
            CREDO_NIHIL(mundi_sanatio_invenire(sanationes, "R"));
        }
        CREDO_FALSUM(mundi_contentum_est(&discus, "XW", "nw\n"));

        /* X. non tuta fracta in unda: non tuta altera et tutae undae
         * eiusdem OMISSAE 'post fracturam' (non incipiuntur) */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul = mundi_agere_simul;
        mundi_ponere(&discus, "u1", "u1\n");
        mundi_ponere(&discus, "u2", "u2\n");
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "XU1", "vetus\n");
        mundi_ponere(&discus, "XU2", "vetus\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_u1", "XU1", NIHIL, "nu1\n",
            I,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_u2", "XU2", NIHIL, "nu2\n",
            0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "U1", "gen_u1", "u1",
            "XU1", "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "U2", "gen_u2", "u2",
            "XU2", "regeneratio");
        actiones[2] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1", "regeneratio");
        actiones[3] = mundi_actio_scripta(piscina, "P2", "gen_2", "a2",
            "X2", "regeneratio");
        actiones[2]->lectiones = VERUM;
        actiones[3]->lectiones = VERUM;
        ordo = mundi_ordinare_fictas(piscina, actiones, IV);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_AEQUALIS_I32(discus.acta, I);
        si (sanationes != NIHIL)
        {
            CREDO_AEQUALIS_I32(_eventus_sanationis(sanationes, "U1"),
                (i32)FABRICA_FRACTUM);
            CREDO_AEQUALIS_I32(_eventus_sanationis(sanationes, "U2"),
                (i32)FABRICA_OMISSUM);
            CREDO_VERUM(mundi_continet(_causa_sanationis(sanationes,
                "U2", piscina), "post fracturam", piscina));
            CREDO_AEQUALIS_I32(_eventus_sanationis(sanationes, "P1"),
                (i32)FABRICA_OMISSUM);
            CREDO_VERUM(mundi_continet(_causa_sanationis(sanationes,
                "P1", piscina), "post fracturam", piscina));
            CREDO_AEQUALIS_I32(_eventus_sanationis(sanationes, "P2"),
                (i32)FABRICA_OMISSUM);
        }

        /* XI. tuta SOLA fracta (via vetus): unda sequens non incipitur
         * - W (ex XH sano) OMISSUM 'post fracturam' */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul = mundi_agere_simul;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "h", "h\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "XH", "vetus\n");
        mundi_ponere(&discus, "XW", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", I,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_h", "XH", NIHIL, "nh\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_w", "XW", NIHIL, "nw\n", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "S1", "gen_1", "a1",
            "X1", "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "H", "gen_h", "h",
            "XH", "regeneratio");
        actiones[2] = mundi_actio_scripta(piscina, "W", "gen_w", "XH",
            "XW", "regeneratio");
        actiones[0]->lectiones = VERUM;
        actiones[2]->lectiones = VERUM;
        ordo = mundi_ordinare_fictas(piscina, actiones, III);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        si (sanationes != NIHIL)
        {
            CREDO_AEQUALIS_I32(_eventus_sanationis(sanationes, "S1"),
                (i32)FABRICA_FRACTUM);
            CREDO_AEQUALIS_I32(_eventus_sanationis(sanationes, "H"),
                (i32)FABRICA_SANATUM);
            CREDO_AEQUALIS_I32(_eventus_sanationis(sanationes, "W"),
                (i32)FABRICA_OMISSUM);
            CREDO_VERUM(mundi_continet(_causa_sanationis(sanationes,
                "W", piscina), "post fracturam", piscina));
        }

        /* XII. recens et ignota (exitus non iudicatus) numquam aguntur
         * per undas: F recens, G ignota - nulla sanatio, acta II */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.agere_simul = mundi_agere_simul;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "f", "f\n");
        mundi_ponere(&discus, "g", "g\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_ponere(&discus, "XF", "nf\n");
        mundi_ponere(&discus, "XG", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_f", "XF", NIHIL, "nf\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_g", "XG", NIHIL, "ng\n", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "P1", "gen_1", "a1",
            "X1", "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "P2", "gen_2", "a2",
            "X2", "regeneratio");
        actiones[2] = mundi_actio_scripta(piscina, "F", "gen_f", "f",
            "XF", "regeneratio");
        actiones[3] = mundi_actio_scripta(piscina, "G", "gen_g", "g",
            "XG", "ignota");
        per (i = ZEPHYRUM; i < IV; i++)
        {
            actiones[i]->lectiones = VERUM;
        }
        ordo = mundi_ordinare_fictas(piscina, actiones, IV);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_AEQUALIS_I32(discus.acta, II);
        si (sanationes != NIHIL)
        {
            CREDO_AEQUALIS_I32(xar_numerus(sanationes), II);
            CREDO_NIHIL(mundi_sanatio_invenire(sanationes, "F"));
            CREDO_NIHIL(mundi_sanatio_invenire(sanationes, "G"));
        }
        CREDO_VERUM(mundi_contentum_est(&discus, "XG", "vetus\n"));
    }
}


/* ==================================================
 * PROBARE: cursus et aestimationes (plan 1b T7)
 * ================================================== */

interior vacuum
_probare_cursum (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
          DiscusFictus  discus;
         FabricaSutura  sutura;
          FabricaActio* actiones[III];
                   Xar* ordo;
                   Xar* sanationes;
        FabricaSanatio* sanatio;
                chorda  causa;
                   i32  scripti;

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        /* A frangitur (actum: scribitur), C omittitur (non scribitur),
         * D sanatur (scribitur) */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "vetus\n");
        mundi_ponere(&discus, "Y", "C:alienum\n");
        mundi_ponere(&discus, "Z", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            I,
            VERUM);
        mundi_scriptum_addere(&discus, "gen_c", "Y", "X", "C:", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_d", "Z", NIHIL, "novum\n",
            0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "C", "gen_c", "X",
            "Y",
            "regeneratio");
        actiones[2] = mundi_actio_scripta(piscina, "D", "gen_d", "a",
            "Z",
            "regeneratio");
        ordo = mundi_ordinare_fictas(piscina, actiones, III);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        {
            i32 k;
            i32 acta;

            /* acta (non IUDICIUM): A fractum, D sanatum; C omissum non */
            acta = ZEPHYRUM;
            per (k = ZEPHYRUM; k
                < xar_numerus(discus.cursus_ficti); k++)
            {
                CursusFictus* c;

                c = (CursusFictus*)xar_obtinere(discus.cursus_ficti, k);
                si (c->eventus != FABRICA_IUDICIUM)
                {
                    acta++;
                    CREDO_VERUM(chorda_aequalis_literis(c->titulus, "A")
                        || chorda_aequalis_literis(c->titulus, "D"));
                    /* cur acta (plan-5 T1): causa iudicis ante actum
                     * in cursu - sanatum ET fractum */
                    CREDO_VERUM(c->stalum.mensura > ZEPHYRUM);
                }
            }
            CREDO_AEQUALIS_I32(acta, II);
        }

        /* siccum: nullum ACTUM scribitur (iudicia licent); D iterum
         * stalum aestimatur ex cursu sanato (I ms), A (fractum solum)
         * et C (numquam acta) -> tempus ignotum */
        mundi_ponere(&discus, "Z", "vetus\n");
        scripti = ZEPHYRUM;
        {
            i32 k;

            per (k = ZEPHYRUM; k
                < xar_numerus(discus.cursus_ficti); k++)
            {
                si (((CursusFictus*)xar_obtinere(discus.cursus_ficti,
                        k))->eventus != FABRICA_IUDICIUM)
                {
                    scripti++;
                }
            }
        }
        mundi_memorias_parare(&sutura, piscina);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, VERUM,
            piscina, &causa);
        {
            i32 k;
            i32 acta;

            acta = ZEPHYRUM;
            per (k = ZEPHYRUM; k
                < xar_numerus(discus.cursus_ficti); k++)
            {
                si (((CursusFictus*)xar_obtinere(discus.cursus_ficti,
                        k))->eventus != FABRICA_IUDICIUM)
                {
                    acta++;
                }
            }
            CREDO_AEQUALIS_I32(acta, scripti);
        }
        sanatio = mundi_sanatio_invenire(sanationes, "D");
        CREDO_VERUM(sanatio != NIHIL && sanatio->tempus_notum
            && sanatio->duratio_ms == I);
        sanatio = mundi_sanatio_invenire(sanationes, "A");
        CREDO_VERUM(sanatio != NIHIL && !sanatio->tempus_notum);
        sanatio = mundi_sanatio_invenire(sanationes, "C");
        CREDO_VERUM(sanatio != NIHIL && !sanatio->tempus_notum);

        /* IUDICIUM (parcum …AR15): regeneratio iudicis VERA in cursu
         * scribitur (VII ms ficti); memorata (eadem actio, idem
         * cursus) non iterum */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "novum\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        (vacuum)fabrica_iudicare(&sutura, actiones[0],
            (FabricaExitus*)xar_obtinere(actiones[0]->exitus, ZEPHYRUM),
            VERUM, piscina);
        (vacuum)fabrica_iudicare(&sutura, actiones[0],
            (FabricaExitus*)xar_obtinere(actiones[0]->exitus, ZEPHYRUM),
            VERUM, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(discus.cursus_ficti), I);
        CREDO_VERUM(((CursusFictus*)xar_obtinere(discus.cursus_ficti,
            ZEPHYRUM))->eventus == FABRICA_IUDICIUM
            && ((CursusFictus*)xar_obtinere(discus.cursus_ficti,
            ZEPHYRUM))->duratio_ms == VII);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "sanare",
      _probare_sanare,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "sanare parallele",
      _probare_sanare_parallele,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "cursum",
      _probare_cursum,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("fabrica_sanare", SECTIONES);
}
