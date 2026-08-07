/* probatio_planta_lectio.c - CATENA TOTA
 *
 * Genus SEMEL descriptum (natura/planta.genera) tria parit:
 *
 *   natura/planta.genera
 *       |  natura_canones
 *   natura/cocta/planta.canon        (schema - bibliotheca tagorum)
 *       |  canon_coquere
 *   include/planta_lectio.h          (structurae C typatae)
 *       |  haec probatio
 *   natura/exempla/hortus.planta     (documentum verum lectum)
 *
 * Nullum schema manu scriptum in tota via. Ergo quaeritur non
 * 'legitne lector documentum' sed 'pervenitne quod natura DIXIT
 * usque ad structuram C'.
 *
 * QUID HAEC PROBATIO PROBET QUOD STRUCTURA MANU SCRIPTA NON:
 *
 *   - nomina kebab per tres transformationes recte eunt:
 *     'rosa-canina' -> typus PlantaRosaCanina, campus rosa_canina,
 *     membrum PLANTA_ROSA_CANINA_HABITUS_FRUTEX
 *   - 'nomen' macrum latinum est (typedef), ergo campus in
 *     'nomen_v' sanatus - coquere reservata novit
 *   - ordinarius= APPLICATUR, et DIVERSUS per genus: rosa frutex,
 *     malus arbor. Hoc <valor> naturae est per DUAS generationes
 *     vectus, et complanatio sub= in opere - structura manu
 *     scripta id ipsa iterum fingere deberet
 *   - _praesens verum dicit: 'ordinarius applicatus' NON idem est
 *     ac 'in documento inventus'
 *   - genera SEIUNCTA manent: granny-smith sub malo est, sed in
 *     saccum mali non cadit (elementum per genus, non per stirpem)
 */
#include "latina.h"
#include "planta_lectio.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "canon.h"
#include "filum.h"
#include <stdio.h>

s32
principale(
    vacuum)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    PlantaIndividua*     ind;
    chorda               fons;
    chorda               causa;
    b32                  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_planta_lectio",
                                         1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    intern        = internamentum_creare(piscina);
    causa.datum   = NIHIL;
    causa.mensura = ZEPHYRUM;


    /* ========================================================
     * PROBARE: documentum verum per lectorem generatum
     * ======================================================== */

    imprimere("\n--- Probans planta_individua_legere ---\n");

    fons = filum_legere_totum("natura/exempla/hortus.planta",
                              piscina);
    CREDO_MAIOR_I32 (fons.mensura, (i32)ZEPHYRUM);

    ind = planta_individua_legere(fons, piscina, intern, &causa);
    CREDO_NON_NIHIL (ind);
    CREDO_CHORDA_VACUA (causa);

    si (!ind)
    {
        imprimere("FRACTA: radix legi nequit\n");
        credo_imprimere_compendium();
        credo_claudere();
        redde I;
    }


    /* ========================================================
     * PROBARE: fixum ipsum canone sanum est
     *
     * Lector generatus plagulam CANONE SANAM praesumit - ignota
     * tacite praeterit. Ergo fixum e canone lapsum probationem
     * VIRIDEM tenere posset dum documentum illicitum est. Neque
     * uncus id capit: canon-custos '.canon|.stml' solas spectat,
     * '.planta' nullam portam habet. Suita ergo porta sit.
     * ======================================================== */

    {
        Canon* canon;
        chorda canon_fons;
        chorda canon_causa;
        Xar*   vitia;

        imprimere("\n--- Probans fixum contra canonem ---\n");

        canon_fons = filum_legere_totum("natura/cocta/planta.canon",
                                        piscina);
        CREDO_MAIOR_I32 (canon_fons.mensura, (i32)ZEPHYRUM);

        canon_causa.datum   = NIHIL;
        canon_causa.mensura = ZEPHYRUM;
        canon = canon_legere(canon_fons, piscina, intern,
                             &canon_causa);
        CREDO_NON_NIHIL (canon);

        si (canon)
        {
            vitia = canon_iudicare(canon, ind->nodus, piscina);
            CREDO_AEQUALIS_I32 (xar_numerus(vitia),
                                (i32)ZEPHYRUM);
        }
    }


    /* ========================================================
     * PROBARE: dispositio per genus (kebab -> campus)
     * ======================================================== */

    {
        imprimere("\n--- Probans dispositionem per genus ---\n");

        CREDO_AEQUALIS_I32 (xar_numerus(ind->rosa_canina), (i32)I);
        CREDO_AEQUALIS_I32 (xar_numerus(ind->granny_smith),
                            (i32)I);

        /* granny-smith sub malo et malo-domestica sedet in
         * natura, sed elementum PROPRIUM est: sacci maiorum
         * vacui manent. Stirps in canone complanata est, non
         * in documento. */
        CREDO_AEQUALIS_I32 (xar_numerus(ind->planta),
                            (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(ind->rosaceae),
                            (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(ind->rosa),
                            (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(ind->malus),
                            (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(ind->malus_domestica),
                            (i32)ZEPHYRUM);
    }

    /* Porta: sequentia omnis ind->rosa_canina[0] et
     * ind->granny_smith[0] petit. Sacco vacuo xar_obtinere NIHIL
     * reddit, et probatio ruina (139) caderet loco rubri legibilis
     * - quod mensuratum est dum vitia ficta plantantur. */
    si (xar_numerus(ind->rosa_canina) != (i32)I ||
        xar_numerus(ind->granny_smith) != (i32)I)
    {
        imprimere("FRACTA: sacci generum vacui - cetera omissa\n");
        credo_imprimere_compendium();
        credo_claudere();
        redde I;
    }


    /* ========================================================
     * PROBARE: rosa-canina - identitas, electiones, ordinarius
     * ======================================================== */

    {
        PlantaRosaCanina* r;

        imprimere("\n--- Probans <rosa-canina> ---\n");

        r = *(PlantaRosaCanina**)xar_obtinere(ind->rosa_canina,
                                              ZEPHYRUM);
        CREDO_NON_NIHIL (r);

        /* effugium ad DOM: titulus ut in documento stat (kebab) */
        CREDO_NON_NIHIL (r->nodus);
        CREDO_CHORDA_AEQUALIS_LITERIS (*r->nodus->titulus,
                                       "rosa-canina");

        /* identitas: 'nomen' reservatum -> campus 'nomen_v';
         * signum '#' VERBATIM manet (spec canon-referentia par. 7:
         * lector nihil stringit, consumentis res est) */
        CREDO_NON_NIHIL (r->nomen_v);
        CREDO_CHORDA_AEQUALIS_LITERIS (*r->nomen_v,
                                       "#rosa-ad-murum");

        /* electio in documento posita */
        CREDO_AEQUALIS_I32 ((i32)r->status_vita,
            (i32)PLANTA_ROSA_CANINA_STATUS_VITA_FLORENS);

        /* electio ABSENS a documento: ordinarius naturae adest.
         * Sine catena hic ABSENS staret. */
        CREDO_AEQUALIS_I32 ((i32)r->habitus,
            (i32)PLANTA_ROSA_CANINA_HABITUS_FRUTEX);
        CREDO_AEQUALIS_I32 ((i32)r->durata,
            (i32)PLANTA_ROSA_CANINA_DURATA_PERENNIS);

        /* numerus absens: ordinarius=3 applicatus, sed _praesens
         * FALSUM - lector non mentitur de fonte valoris */
        CREDO_AEQUALIS_S64 (r->altitudo_matura, (s64)III);
        CREDO_FALSUM (r->altitudo_matura_praesens);

        /* electio sine ordinario et sine valore: ABSENS */
        CREDO_AEQUALIS_I32 ((i32)r->anima,
            (i32)PLANTA_ROSA_CANINA_ANIMA_ABSENS);

        /* relatio: signum '#' verbatim, corpus ut auctor scripsit
         * (orthographia stylus, signum mechanismus) */
        CREDO_NON_NIHIL (r->auctor_nominis);
        CREDO_CHORDA_AEQUALIS_LITERIS (*r->auctor_nominis,
                                       "#carl_linnaeus");

        /* attributa tacita monstratorem NIHIL retinent */
        CREDO_NIHIL (r->pollinatur_a);
        CREDO_NIHIL (r->sita_in);

        /* pars adstricta (intra=): typus qualificatus */
        CREDO_NON_NIHIL (r->radix);
        si (r->radix)
        {
            CREDO_NON_NIHIL (r->radix->nota);
            CREDO_CHORDA_AEQUALIS_LITERIS (*r->radix->nota,
                                           "muro proxima");
        }

        /* partes tacitae: lector nihil fingit */
        CREDO_NIHIL (r->caulis);
        CREDO_NIHIL (r->flos);
        CREDO_AEQUALIS_I32 (xar_numerus(r->generat),
                            (i32)ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: historia/eventum - elementum bis adstrictum
     * ======================================================== */

    {
        PlantaRosaCanina*      r;
        PlantaHistoriaEventum* ev;

        imprimere("\n--- Probans <historia>/<eventum> ---\n");

        r = *(PlantaRosaCanina**)xar_obtinere(ind->rosa_canina,
                                              ZEPHYRUM);
        CREDO_NON_NIHIL (r->historia);
        si (r->historia)
        {
            CREDO_AEQUALIS_I32 (xar_numerus(r->historia->eventum),
                                (i32)I);

            ev = *(PlantaHistoriaEventum**)xar_obtinere(
                     r->historia->eventum, ZEPHYRUM);
            CREDO_NON_NIHIL (ev);

            CREDO_NON_NIHIL (ev->quando);
            CREDO_CHORDA_AEQUALIS_LITERIS (*ev->quando, "2024-04");

            /* actiones naturae electionem faciunt */
            CREDO_AEQUALIS_I32 ((i32)ev->actio,
                (i32)PLANTA_HISTORIA_EVENTUM_ACTIO_NOMINARI);
        }
    }


    /* ========================================================
     * PROBARE: granny-smith - ordinarius DIVERSUS per genus
     * ======================================================== */

    {
        PlantaGrannySmith* g;

        imprimere("\n--- Probans <granny-smith> ---\n");

        g = *(PlantaGrannySmith**)xar_obtinere(ind->granny_smith,
                                               ZEPHYRUM);
        CREDO_NON_NIHIL (g);

        CREDO_NON_NIHIL (g->nomen_v);
        CREDO_CHORDA_AEQUALIS_LITERIS (*g->nomen_v, "#malus-prima");

        /* IDEM attributum, IDEM silentium in documento, ALIUS
         * ordinarius quam rosae-caninae: arbor, non frutex.
         * Hoc solum est quod complanationem sub= probat. */
        CREDO_AEQUALIS_I32 ((i32)g->habitus,
            (i32)PLANTA_GRANNY_SMITH_HABITUS_ARBOR);
        CREDO_AEQUALIS_I32 ((i32)g->folia_permanentia,
            (i32)PLANTA_GRANNY_SMITH_FOLIA_PERMANENTIA_DECIDUA);

        /* numerus in documento positus ordinarium (5) vincit,
         * et _praesens id fatetur */
        CREDO_AEQUALIS_S64 (g->altitudo_matura, (s64)IV);
        CREDO_VERUM (g->altitudo_matura_praesens);

        CREDO_NIHIL (g->radix);
        CREDO_NIHIL (g->historia);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
