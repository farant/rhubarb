/* fabrica_ordo.c - ordo fabricae: dependentiae per locos, ordo
 * topologicus, vestigia scripturae (actio scribit SOLUM intra
 * vestigium suum), undae (quae simul currere possunt). Pars
 * bibliothecae fabrica (fabrica-6 H2; vide fabrica.c). */

#include "fabrica.h"
#include "fabrica_interna.h"
#include "chorda_aedificator.h"


/* ==================================================
 * Dependentiae per locos (plan 1b T5)
 * ================================================== */

/* loci unius ingressus: verbum enumerare (aut ex particulis); si
 * explicari nequit (absens - clonus recens), via declarata */
interior vacuum
_ingressus_loci (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                      Piscina* piscina,
                          Xar* loci)
{
       Xar* particulae;
    chorda  causa;
       b32  bonum;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    si (ingressus->genus->enumerare != NIHIL)
    {
        bonum = ingressus->genus->enumerare(sutura, ingressus, piscina,
            loci, &causa);
    }
    alioquin
    {
        particulae = xar_creare(piscina,
            (i32)magnitudo(FabricaParticula));
        bonum = particulae != NIHIL
            && ingressus->genus->sigillare(sutura, ingressus, NIHIL,
                   piscina, particulae, &causa);
        si (bonum)
        {
            fabricae_loci_ex_particulis(particulae, piscina, loci);
        }
    }
    si (!bonum)
    {
        fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULA,
            ingressus->via,
            chorda_ex_literis("", piscina));
    }
}

b32
fabrica_dependentias_computare (
    constans FabricaSutura* sutura,
                       Xar* actiones,
                   Piscina* piscina,
                    chorda* causa_out)
{
    TabulaDispersa* producentes;   /* via exitus -> FabricaActio* */
               i32  i;
               i32  j;
               i32  k;

    (vacuum)causa_out;
    producentes = tabula_dispersa_creare_chorda(piscina, 256);
    si (producentes == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        FabricaActio* actio;

        actio = (FabricaActio*)xar_obtinere(actiones, i);
        per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
        {
            (vacuum)tabula_dispersa_inserere(producentes,
                ((FabricaExitus*)xar_obtinere(actio->exitus, j))->via,
                actio);
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        FabricaActio* actio;
                 Xar* loci;

        actio = (FabricaActio*)xar_obtinere(actiones, i);
        loci = xar_creare(piscina, (i32)magnitudo(FabricaLocus));
        actio->dependentiae = fabricae_xar_chordarum(piscina);
        si (loci == NIHIL || actio->dependentiae == NIHIL)
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < xar_numerus(actio->ingressus); j++)
        {
            _ingressus_loci(sutura,
                (FabricaIngressus*)xar_obtinere(actio->ingressus, j),
                piscina, loci);
        }
        /* ORDO EX VESTIGIO (plan 2 T2): quod actio lectiones="verum"
         * cursu ultimo legit, quaesivit, enumeravit - arcus ad
         * producentes, ut manifesta quae vestigium supplet */
        {
            Xar* ultimae;

            si (   actio->lectiones
                && sutura->lectiones_ultimae != NIHIL
                && sutura->lectiones_ultimae(sutura->datum,
                       chorda_ut_cstr(actio->titulus, piscina), piscina,
                       &ultimae))
            {
                per (j = ZEPHYRUM; j < xar_numerus(ultimae); j++)
                {
                    constans FabricaLectio* l;

                    l = (constans FabricaLectio*)xar_obtinere(ultimae,
                        j);
                    fabricae_locum_addere(loci,
                        l->genus == LECTIO_ENUMERAVIT
                            ? FABRICA_LOCUS_PLAGULAE
                            : FABRICA_LOCUS_PLAGULA,
                        l->via, chorda_ex_literis("", piscina));
                }
            }
        }
        per (j = ZEPHYRUM; j < xar_numerus(loci); j++)
        {
            FabricaLocus* locus;
                  vacuum* inventa;

            locus = (FabricaLocus*)xar_obtinere(loci, j);
            si (locus->forma == FABRICA_LOCUS_PLAGULA)
            {
                FabricaActio* producens;

                si (!tabula_dispersa_invenire(producentes, locus->via,
                        &inventa))
                {
                    perge;
                }
                producens = (FabricaActio*)inventa;
                si (   producens != actio
                    && !fabricae_chordam_continet(actio->dependentiae,
                           producens->titulus))
                {
                    fabricae_chordam_addere(actio->dependentiae,
                        producens->titulus);
                }
                perge;
            }
            /* PLAGULAE aut ARBOR: exitus omnes inspiciuntur (rari) */
            per (k = ZEPHYRUM; k < xar_numerus(actiones); k++)
            {
                FabricaActio* altera;
                         i32  l;

                altera = (FabricaActio*)xar_obtinere(actiones, k);
                si (   altera == actio
                    || fabricae_chordam_continet(actio->dependentiae,
                           altera->titulus))
                {
                    perge;
                }
                per (l = ZEPHYRUM; l < xar_numerus(altera->exitus); l++)
                {
                    chorda via;
                    chorda directorium;
                    chorda titulus;

                    via = ((FabricaExitus*)xar_obtinere(altera->exitus,
                        l))->via;
                    fabricae_viam_dividere(via, &directorium, &titulus);
                    si (fabricae_locus_tangit(locus, via, directorium,
                        titulus,
                            piscina))
                    {
                        fabricae_chordam_addere(actio->dependentiae,
                            altera->titulus);
                        frange;
                    }
                }
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Ordo
 * ================================================== */

/* VERUM si posterior exitum prioris legit aut priorem praecondicionem
 * nominat */
b32
fabricae_pendet (
    constans FabricaActio* posterior,
    constans FabricaActio* prior)
{
    i32 i;
    i32 j;

    /* POST (fabrica-6 T7): producens ante - ordo sine sigillo */
    per (i = ZEPHYRUM;
         posterior->post != NIHIL && i < xar_numerus(posterior->post);
         i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(posterior->post, i),
                prior->titulus))
        {
            redde VERUM;
        }
    }
    /* praecondicio: ordo sine sigillo */
    per (i = ZEPHYRUM;
         posterior->praecondiciones != NIHIL
         && i < xar_numerus(posterior->praecondiciones);
         i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(
                posterior->praecondiciones, i), prior->titulus))
        {
            redde VERUM;
        }
    }
    /* dependentiae computatae (per locos, T5) vias declaratas
     * supplent */
    si (posterior->dependentiae != NIHIL)
    {
        per (i = ZEPHYRUM; i
            < xar_numerus(posterior->dependentiae); i++)
        {
            si (chorda_aequalis(*(chorda*)xar_obtinere(
                    posterior->dependentiae, i), prior->titulus))
            {
                redde VERUM;
            }
        }
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(posterior->ingressus); i++)
    {
        FabricaIngressus* ingressus;

        ingressus = (FabricaIngressus*)xar_obtinere(
            posterior->ingressus, i);
        per (j = ZEPHYRUM; j < xar_numerus(prior->exitus); j++)
        {
            FabricaExitus* exitus;

            exitus = (FabricaExitus*)xar_obtinere(prior->exitus, j);
            si (chorda_aequalis(ingressus->via, exitus->via))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

Xar*
fabrica_ordinare (
    constans Xar* actiones,
         Piscina* piscina,
          chorda* causa_out)
{
    Xar* ordo;
    i32* gradus;
    b32* posita;
    i32  numerus;
    i32  positae;
    i32  i;
    i32  j;

    numerus  = xar_numerus(actiones);
    ordo     = xar_creare(piscina, (i32)magnitudo(FabricaActio*));
    gradus = (i32*)piscina_allocare(piscina,
        magnitudo(i32) * (memoriae_index)(numerus + 1));
    posita = (b32*)piscina_allocare(piscina,
        magnitudo(b32) * (memoriae_index)(numerus + 1));
    si (ordo == NIHIL || gradus == NIHIL || posita == NIHIL)
    {
        redde NIHIL;
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        gradus[j] = ZEPHYRUM;
        posita[j] = FALSUM;
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            si (   i != j && fabricae_pendet(
                    (FabricaActio*)xar_obtinere(actiones, j),
                    (FabricaActio*)xar_obtinere(actiones, i)))
            {
                gradus[j]++;
            }
        }
    }

    /* Kahn, ordine dato inter paratas (determinismus) */
    positae = ZEPHYRUM;
    dum (positae < numerus)
    {
        s32 electa;

        electa = -I;
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (!posita[j] && gradus[j] == 0)
            {
                electa = (s32)j;
                frange;
            }
        }
        si (electa < 0)
        {
            frange;
        }
        posita[electa] = VERUM;
        positae++;
        *(FabricaActio**)xar_addere(ordo) =
            (FabricaActio*)xar_obtinere(actiones, (i32)electa);
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (   !posita[j] && fabricae_pendet(
                    (FabricaActio*)xar_obtinere(actiones, j),
                    (FabricaActio*)xar_obtinere(actiones,
                        (i32)electa)))
            {
                gradus[j]--;
            }
        }
    }

    si (positae < numerus)
    {
        ChordaAedificator* aedificator;
                      b32  primum;

        aedificator = chorda_aedificator_creare(piscina, 128);
        si (aedificator == NIHIL)
        {
            redde NIHIL;
        }
        (vacuum)chorda_aedificator_appendere_literis(aedificator,
            "cyclus inter actiones: ");
        primum = VERUM;
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (!posita[j])
            {
                si (!primum)
                {
                    (vacuum)chorda_aedificator_appendere_literis(
                        aedificator, ", ");
                }
                (vacuum)chorda_aedificator_appendere_chorda(aedificator,
                    ((FabricaActio*)xar_obtinere(actiones,
                    j))->titulus);
                primum = FALSUM;
            }
        }
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_aedificator_finire(aedificator);
        }
        redde NIHIL;
    }
    redde ordo;
}


/* ==================================================
 * Vestigia (plan 1b T4): actio scribit SOLUM intra vestigium suum
 * ================================================== */

/* vestigium actionis: locare exituum + vestigia + communia +
 * involucrum (acta, provenientia, scriptura iudicis). status_fabricae:
 * etiam status executoris ipsius (memoria sqlite, sera) - pro
 * probatione scripturae VERUM, pro undis FALSUM (omnibus communis,
 * executoris non actionis: undas omnes separaret) */
interior Xar*
_vestigium_actionis (
    constans FabricaActio* actio,
                      b32  status_fabricae,
                  Piscina* piscina)
{
       Xar* loci;
    chorda  vacua;
       i32  i;

    loci   = xar_creare(piscina, (i32)magnitudo(FabricaLocus));
    vacua  = chorda_ex_literis("", piscina);
    si (loci == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(actio->exitus); i++)
    {
        constans FabricaExitus* exitus;

        exitus = (FabricaExitus*)xar_obtinere(actio->exitus, i);
        si (exitus->genus != NIHIL && exitus->genus->locare != NIHIL)
        {
            (vacuum)exitus->genus->locare(exitus, piscina, loci);
        }
    }
    per (i = ZEPHYRUM;
         actio->vestigia != NIHIL && i < xar_numerus(actio->vestigia);
         i++)
    {
        *(FabricaLocus*)xar_addere(loci) =
            *(FabricaLocus*)xar_obtinere(actio->vestigia, i);
    }
    per (i = ZEPHYRUM;
         actio->communia != NIHIL && i < xar_numerus(actio->communia);
         i++)
    {
        *(FabricaLocus*)xar_addere(loci) =
            *(FabricaLocus*)xar_obtinere(actio->communia, i);
    }
    fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULA,
        fabricae_iungere(piscina,
        "build/fabrica/acta/", actio->titulus, ".log"), vacua);
    fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULA,
        fabricae_iungere(piscina,
        "build/fabrica/provenientia/", actio->titulus, ".c"), vacua);
    fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULA,
        fabricae_iungere(piscina,
        "build/fabrica/provenientia/", actio->titulus, ".o"), vacua);
    fabricae_locum_addere(loci, FABRICA_LOCUS_ARBOR,
        fabricae_iungere(piscina,
        "build/fabrica/scriptura/", actio->titulus, ""), vacua);
    /* libri lectionum actionis (plan 2 T2 iudex, T6 sanare simul) */
    fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULA,
        fabricae_iungere(piscina,
        LIBRI_DIRECTORIUM, actio->titulus, ".tsv"), vacua);
    fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULA,
        fabricae_iungere(piscina,
        LIBRI_DIRECTORIUM, actio->titulus, ".sanare.tsv"), vacua);
    /* status fabricae ipsius (memoria sqlite cum -wal/-shm, sera):
     * involucrum, non actio - T4: mensor_ui -shm tetigit */
    si (status_fabricae)
    {
        fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULAE,
            chorda_ex_literis(
            "build", piscina), chorda_ex_literis(
            "fabrica.db fabrica.db-wal fabrica.db-shm", piscina));
        fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULA,
            chorda_ex_literis(
            "build/fabrica/sera", piscina), vacua);
        /* THESAURUS (plan 2 T3): cache communis fabricae, scriptura
         * atomica - quaevis actio domus (aedilis --thesaurus,
         * compilator) in eo scribit; involucrum, non vestigium actionis
         * (radix ipsa, obiecta .o, NON: ea vestigia sua manent) */
        {
            i32 k;

            per (k = ZEPHYRUM; k < III; k++)
            {
                chorda pars;

                /* sine '/' finali: locus ARBOR 'via/' ipse addit */
                pars = chorda_ex_literis(fabricae_thesauri_partes[k],
                    piscina);
                pars.mensura--;
                fabricae_locum_addere(loci, FABRICA_LOCUS_ARBOR, pars,
                    vacua);
            }
        }
    }
    redde loci;
}

b32
fabricae_in_locis (
     constans Xar* loci,
           chorda  via,
          Piscina* piscina)
{
    chorda directorium;
    chorda titulus;
       i32 i;

    fabricae_viam_dividere(via, &directorium, &titulus);
    per (i = ZEPHYRUM; i < xar_numerus(loci); i++)
    {
        si (fabricae_locus_tangit((FabricaLocus*)xar_obtinere(loci, i),
            via,
                directorium, titulus, piscina))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

Xar*
fabrica_vestigia_comparare (
    constans FabricaActio* actio,
             constans Xar* ante,
             constans Xar* post,
                  Piscina* piscina)
{
    Xar* loci;
    Xar* extra;
    i32  i;
    i32  j;

    loci   = _vestigium_actionis(actio, VERUM, piscina);
    extra  = fabricae_xar_chordarum(piscina);
    si (loci == NIHIL || extra == NIHIL)
    {
        redde NIHIL;
    }
    i = ZEPHYRUM;
    j = ZEPHYRUM;
    dum (i < xar_numerus(ante) || j < xar_numerus(post))
    {
        constans FabricaVestigium* a;
        constans FabricaVestigium* b;
                              s32  ordo;
                           chorda  mutata;

        a = (i < xar_numerus(ante))
            ? (FabricaVestigium*)xar_obtinere(ante, i) : NIHIL;
        b = (j < xar_numerus(post))
            ? (FabricaVestigium*)xar_obtinere(post, j) : NIHIL;
        si (a == NIHIL)
        {
            ordo = I;
        }
        alioquin si (b == NIHIL)
        {
            ordo = -I;
        }
        alioquin
        {
            ordo = chorda_comparare(a->via, b->via);
        }
        si (ordo < 0)
        {
            mutata = a->via;   /* deleta */
            i++;
        }
        alioquin si (ordo > 0)
        {
            mutata = b->via;   /* nova */
            j++;
        }
        alioquin
        {
            i++;
            j++;
            si (   a->tempus_ns == b->tempus_ns
                && a->mensura   == b->mensura)
            {
                perge;
            }
            mutata = a->via;   /* scripta (octetis eisdem quoque) */
        }
        si (!fabricae_in_locis(loci, mutata, piscina))
        {
            fabricae_chordam_addere(extra, mutata);
        }
    }
    redde extra;
}

/* loci concurrunt si via una alteram continet (conservativum) */
interior b32
_loci_concurrunt (
    constans FabricaLocus* a,
    constans FabricaLocus* b,
                  Piscina* piscina)
{
    si (chorda_aequalis(a->via, b->via))
    {
        redde VERUM;
    }
    redde chorda_incipit(b->via, fabricae_iungere(piscina, "", a->via,
        "/"))
        || chorda_incipit(a->via, fabricae_iungere(piscina, "", b->via,
        "/"));
}

interior b32
_simul_possunt (
    constans FabricaActio* x,
    constans FabricaActio* y,
                  Piscina* piscina)
{
    Xar* lx;
    Xar* ly;
    i32  i;
    i32  j;

    si (   (x->communia != NIHIL && xar_numerus(x->communia) > 0)
        || (y->communia != NIHIL && xar_numerus(y->communia) > 0))
    {
        redde FALSUM;
    }
    si (fabricae_pendet(x, y) || fabricae_pendet(y, x))
    {
        redde FALSUM;
    }
    lx = _vestigium_actionis(x, FALSUM, piscina);
    ly = _vestigium_actionis(y, FALSUM, piscina);
    per (i = ZEPHYRUM; i < xar_numerus(lx); i++)
    {
        per (j = ZEPHYRUM; j < xar_numerus(ly); j++)
        {
            si (_loci_concurrunt((FabricaLocus*)xar_obtinere(lx, i),
                    (FabricaLocus*)xar_obtinere(ly, j), piscina))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

Xar*
fabrica_undas_formare (
    constans Xar* ordo,
         Piscina* piscina)
{
     Xar* undae;
     i32* unda;
     i32  numerus;
     i32  i;
     i32  j;

    numerus  = xar_numerus(ordo);
    undae    = xar_creare(piscina, (i32)magnitudo(Xar*));
    unda = (i32*)piscina_allocare(piscina,
        magnitudo(i32) * (memoriae_index)(numerus + 1));
    si (undae == NIHIL || unda == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        FabricaActio* actio;
                 i32  w;

        actio  = *(FabricaActio**)xar_obtinere(ordo, i);
        w      = ZEPHYRUM;
        per (j = ZEPHYRUM; j < i; j++)
        {
            si (   fabricae_pendet(actio,
                *(FabricaActio**)xar_obtinere(ordo,
                j))
                && unda[j] + I > w)
            {
                w = unda[j] + I;
            }
        }
        dum (w < xar_numerus(undae))
        {
            Xar* membra;
            b32  apta;

            membra  = *(Xar**)xar_obtinere(undae, w);
            apta    = VERUM;
            per (j = ZEPHYRUM; apta && j < xar_numerus(membra); j++)
            {
                apta = _simul_possunt(actio,
                    *(FabricaActio**)xar_obtinere(membra, j), piscina);
            }
            si (apta)
            {
                frange;
            }
            w++;
        }
        si (w == xar_numerus(undae))
        {
            *(Xar**)xar_addere(undae) = xar_creare(piscina,
                (i32)magnitudo(FabricaActio*));
        }
        *(FabricaActio**)xar_addere(*(Xar**)xar_obtinere(undae, w)) =
            actio;
        unda[i] = w;
    }
    redde undae;
}
