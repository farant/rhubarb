/* lapifex_glr.c - Implementatio Motoris GLR
 *
 * Generalized LR parser usans Acervum Grapho-Structuratum (GSS).
 * Tractat conflictus shift/reduce et reduce/reduce per furcationem
 * viarum parallelarum et fusionem quando eundem statum attingunt.
 *
 * Algorithmus (Tomita-stilus cum worklist):
 *   Pro unoquoque signo:
 *     1. REDUCE: Worklist de (nodus, productio) paribus.
 *        Pro unoquoque pari, enumerare OMNES vias per GSS,
 *        reducere, et addere novos nodos ad frontem.
 *        Si novus nodus habet actiones REDUCE, addere
 *        ad worklist. Quisque par processatur semel.
 *     2. ACCEPT: Colligere omnes fructus.
 *     3. SHIFT: Transponere signum, creare frontem novam.
 *
 * Ambiguitas:
 *   Quando reductio producit nodum cum eodem statu sed
 *   valore differenti quam nodus iam in fronte, novus nodus
 *   separatus creatur (furcatio). In SHIFT, omnes nodi cum
 *   eodem statu target fusionantur per additionem praedecessorum.
 *   In ACCEPT, omnes praedecessores enumerantur pro collectione
 *   omnium valorum fructuum.
 */

#include "lapifex_glr.h"

/* ================================================
 * Via in GSS: repraesentat unam viam per GSS
 * ================================================ */

nomen structura {
    s64              valori[LAPIFEX_MAXIMUS_DEXTRUM];
    LapifexGSSNodus* basis;
} GSSVia;

/* ================================================
 * Par in worklist: (index nodi in fronte, index productionis)
 * ================================================ */

nomen structura {
    s32  nodus_idx;
    s32  productio_idx;
} ReductioParum;

/* ================================================
 * Functiones Auxiliares Internae
 * ================================================ */

hic_manens LapifexGSSNodus*
gss_nodus_creare(
    LapifexGLR*  glr,
    s32          status,
    s64          valor,
    s32          profunditas)
{
    LapifexGSSNodus*  nodus;
    LapifexGSSNodus** ref;

    nodus = (LapifexGSSNodus*)piscina_allocare(
        glr->piscina, (memoriae_index)magnitudo(LapifexGSSNodus));

    nodus->status = status;
    nodus->valor = valor;
    nodus->praedecessores = xar_creare(glr->piscina,
        (i32)magnitudo(LapifexGSSNodus*));
    nodus->profunditas = profunditas;
    nodus->vivus = VERUM;

    ref = (LapifexGSSNodus**)xar_addere(glr->nodi_omnes);
    si (ref) *ref = nodus;

    redde nodus;
}

hic_manens vacuum
gss_praedecessorem_addere(
    LapifexGSSNodus*  nodus,
    LapifexGSSNodus*  praedecessor)
{
    LapifexGSSNodus** ref;
    i32 i;
    i32 num;

    si (!nodus || !praedecessor) redde;

    num = (i32)xar_numerus(nodus->praedecessores);
    per (i = ZEPHYRUM; i < num; i++)
    {
        LapifexGSSNodus** existens =
            (LapifexGSSNodus**)xar_obtinere(nodus->praedecessores, i);
        si (existens && *existens == praedecessor) redde;
    }

    ref = (LapifexGSSNodus**)xar_addere(nodus->praedecessores);
    si (ref) *ref = praedecessor;
}

/* Enumerare omnes vias longitudinis num_gradus a culmine.
 * Cum nodus habet plures praedecessores, unaquaeque via
 * repraesentat unam historiam parsationis.
 */
hic_manens vacuum
vias_enumerare_recursio(
    LapifexGSSNodus*  nodus,
    s32               profunditas_currens,
    s32               num_gradus,
    s64*              valori_partiales,
    Xar*              fructus)
{
    si (profunditas_currens == num_gradus)
    {
        GSSVia* via = (GSSVia*)xar_addere(fructus);
        si (via)
        {
            s32 i;
            per (i = ZEPHYRUM; i < num_gradus; i++)
            {
                via->valori[i] = valori_partiales[i];
            }
            via->basis = nodus;
        }
        redde;
    }

    si (!nodus) redde;

    {
        s32 idx = num_gradus - I - profunditas_currens;
        i32 num_pred;
        i32 pi;

        si (idx < ZEPHYRUM || idx >= LAPIFEX_MAXIMUS_DEXTRUM) redde;
        valori_partiales[idx] = nodus->valor;

        num_pred = (i32)xar_numerus(nodus->praedecessores);
        si (num_pred == ZEPHYRUM) redde;

        per (pi = ZEPHYRUM; pi < num_pred; pi++)
        {
            LapifexGSSNodus** pred_ref =
                (LapifexGSSNodus**)xar_obtinere(
                    nodus->praedecessores, pi);
            si (pred_ref && *pred_ref)
            {
                vias_enumerare_recursio(
                    *pred_ref,
                    profunditas_currens + I,
                    num_gradus,
                    valori_partiales,
                    fructus);
            }
        }
    }
}

hic_manens Xar*
vias_enumerare(
    LapifexGLR*       glr,
    LapifexGSSNodus*  culmen,
    s32               num_gradus)
{
    Xar* fructus;
    s64  valori_temp[LAPIFEX_MAXIMUS_DEXTRUM];

    fructus = xar_creare(glr->piscina, (i32)magnitudo(GSSVia));

    si (num_gradus == ZEPHYRUM)
    {
        GSSVia* via = (GSSVia*)xar_addere(fructus);
        si (via) via->basis = culmen;
        redde fructus;
    }

    vias_enumerare_recursio(
        culmen, ZEPHYRUM, num_gradus,
        valori_temp, fructus);

    redde fructus;
}

hic_manens s32
in_fronte_invenire(
    Xar*  frons,
    s32   status)
{
    i32 i;
    i32 num = (i32)xar_numerus(frons);

    per (i = ZEPHYRUM; i < num; i++)
    {
        LapifexGSSNodus** ref =
            (LapifexGSSNodus**)xar_obtinere(frons, i);
        si (ref && *ref && (*ref)->vivus && (*ref)->status == status)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* Addere actiones REDUCE pro nodo ad worklist.
 * Verificare quod par (nodus_idx, productio) non iam in worklist. */
hic_manens vacuum
reductiones_addere_ad_worklist(
    Xar*             worklist,
    LapifexTabula*   tabula,
    s32              nodus_idx,
    s32              nodus_status,
    s32              terminalis)
{
    Xar* actiones;
    i32  num_act;
    i32  ai;

    actiones = lapifex_actiones_quaerere(
        tabula, nodus_status, terminalis);
    num_act = actiones ? (i32)xar_numerus(actiones) : ZEPHYRUM;

    per (ai = ZEPHYRUM; ai < num_act; ai++)
    {
        LapifexActioIntroitus* actio =
            (LapifexActioIntroitus*)xar_obtinere(actiones, ai);

        si (!actio || actio->actio != LAPIFEX_ACTIO_REDUCERE)
            perge;

        {
            b32 iam_in = FALSUM;
            i32 wi;
            i32 num_worklist = (i32)xar_numerus(worklist);

            per (wi = ZEPHYRUM; wi < num_worklist; wi++)
            {
                ReductioParum* rp =
                    (ReductioParum*)xar_obtinere(worklist, wi);
                si (rp && rp->nodus_idx == nodus_idx &&
                    rp->productio_idx == actio->valor)
                {
                    iam_in = VERUM;
                    frange;
                }
            }

            si (!iam_in)
            {
                ReductioParum* novum =
                    (ReductioParum*)xar_addere(worklist);
                si (novum)
                {
                    novum->nodus_idx = nodus_idx;
                    novum->productio_idx = actio->valor;
                }
            }
        }
    }
}

/* ================================================
 * Creare Motor GLR
 * ================================================ */

LapifexGLR*
lapifex_glr_creare(
    Piscina*        piscina,
    LapifexTabula*  tabula)
{
    LapifexGLR* glr;

    si (!piscina || !tabula) redde NIHIL;

    glr = (LapifexGLR*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(LapifexGLR));

    glr->tabula = tabula;
    glr->piscina = piscina;
    glr->viae = xar_creare(piscina, (i32)magnitudo(LapifexGLRVia));
    glr->nodi_omnes = xar_creare(piscina,
        (i32)magnitudo(LapifexGSSNodus*));
    glr->numerus_furcationum = ZEPHYRUM;
    glr->numerus_fusionum = ZEPHYRUM;

    redde glr;
}

/* ================================================
 * Reficere Motor GLR
 * ================================================ */

vacuum
lapifex_glr_reficere(
    LapifexGLR*  glr)
{
    si (!glr) redde;

    glr->viae = xar_creare(glr->piscina, (i32)magnitudo(LapifexGLRVia));
    glr->nodi_omnes = xar_creare(glr->piscina,
        (i32)magnitudo(LapifexGSSNodus*));
    glr->numerus_furcationum = ZEPHYRUM;
    glr->numerus_fusionum = ZEPHYRUM;
}

/* ================================================
 * Parsare - Motor Principalis GLR
 * ================================================ */

LapifexGLRFructus
lapifex_glr_parsare(
    LapifexGLR*              glr,
    constans LapifexSignum*  signa,
    s32                      numerus,
    LapifexReductioCallback  reductio,
    vacuum*                  contextus)
{
    LapifexGLRFructus  fructus;
    Xar*               frons;
    s32                positio;
    Xar*               fructus_valori;

    fructus.valori = NIHIL;
    fructus.numerus_fructuum = ZEPHYRUM;
    fructus.successus = FALSUM;
    fructus.ambiguus = FALSUM;
    fructus.error.status = ZEPHYRUM;
    fructus.error.terminalis = ZEPHYRUM;
    fructus.error.positio = ZEPHYRUM;
    fructus.error.est_error = FALSUM;

    si (!glr || !glr->tabula)
    {
        fructus.error.est_error = VERUM;
        redde fructus;
    }

    lapifex_glr_reficere(glr);

    /* Frons initialis: unus nodus in statu 0 */
    frons = xar_creare(glr->piscina, (i32)magnitudo(LapifexGSSNodus*));
    {
        LapifexGSSNodus*  nodus_init;
        LapifexGSSNodus** ref;

        nodus_init = gss_nodus_creare(glr, ZEPHYRUM, 0, ZEPHYRUM);
        ref = (LapifexGSSNodus**)xar_addere(frons);
        si (ref) *ref = nodus_init;
    }

    fructus_valori = xar_creare(glr->piscina, (i32)magnitudo(s64));
    positio = ZEPHYRUM;

    dum ((i32)xar_numerus(frons) > ZEPHYRUM)
    {
        s32  terminalis;
        Xar* frons_nova;
        Xar* worklist;
        i32  cursor;
        i32  fi;

        si (positio < numerus)
        {
            terminalis = signa[positio].terminalis;
        }
        alioquin
        {
            terminalis = -I;
        }

        /* ========================================
         * PASSUS 1: REDUCE (worklist)
         * ======================================== */
        worklist = xar_creare(glr->piscina,
            (i32)magnitudo(ReductioParum));

        per (fi = ZEPHYRUM; fi < (i32)xar_numerus(frons); fi++)
        {
            LapifexGSSNodus** ref_fi =
                (LapifexGSSNodus**)xar_obtinere(frons, fi);
            si (!ref_fi || !*ref_fi || !(*ref_fi)->vivus) perge;

            reductiones_addere_ad_worklist(
                worklist, glr->tabula,
                (s32)fi, (*ref_fi)->status, terminalis);
        }

        cursor = ZEPHYRUM;
        dum (cursor < (i32)xar_numerus(worklist))
        {
            ReductioParum*    rp;
            LapifexGSSNodus** ref_nodus;
            LapifexGSSNodus*  nodus;
            LapifexProductio* prod;
            i32               num_dex;
            Xar*              viae_gss;
            i32               vi;

            rp = (ReductioParum*)xar_obtinere(worklist, cursor);
            cursor++;

            si (!rp) perge;

            ref_nodus = (LapifexGSSNodus**)xar_obtinere(
                frons, (i32)rp->nodus_idx);
            si (!ref_nodus || !*ref_nodus) perge;
            nodus = *ref_nodus;

            prod = (LapifexProductio*)xar_obtinere(
                glr->tabula->grammatica->productiones,
                (i32)rp->productio_idx);
            si (!prod) perge;

            num_dex = (i32)xar_numerus(prod->dextrum);

            viae_gss = vias_enumerare(glr, nodus, (s32)num_dex);

            per (vi = ZEPHYRUM; vi < (i32)xar_numerus(viae_gss); vi++)
            {
                GSSVia*  via_gss;
                s64      valor_novus;
                s32      goto_st;
                s32      existens_idx;

                via_gss = (GSSVia*)xar_obtinere(viae_gss, vi);
                si (!via_gss || !via_gss->basis) perge;

                si (reductio)
                {
                    valor_novus = reductio(
                        prod->index, via_gss->valori,
                        (s32)num_dex, contextus);
                }
                alioquin
                {
                    valor_novus = 0;
                }

                goto_st = lapifex_goto_quaerere(
                    glr->tabula, via_gss->basis->status,
                    prod->sinistrum);

                si (goto_st < ZEPHYRUM) perge;

                existens_idx = in_fronte_invenire(frons, goto_st);

                si (existens_idx >= ZEPHYRUM)
                {
                    LapifexGSSNodus** ref_exist =
                        (LapifexGSSNodus**)xar_obtinere(
                            frons, (i32)existens_idx);
                    si (ref_exist && *ref_exist)
                    {
                        si ((*ref_exist)->valor == valor_novus)
                        {
                            /* Idem valor: fusio (addere praedecessorem) */
                            gss_praedecessorem_addere(
                                *ref_exist, via_gss->basis);
                            glr->numerus_fusionum++;

                            reductiones_addere_ad_worklist(
                                worklist, glr->tabula,
                                existens_idx, (*ref_exist)->status,
                                terminalis);
                        }
                        alioquin
                        {
                            /* Valor differens: furcatio (ambiguitas) */
                            LapifexGSSNodus*  nn;
                            LapifexGSSNodus** ref_add2;
                            s32               novus_idx2;

                            nn = gss_nodus_creare(
                                glr, goto_st, valor_novus,
                                via_gss->basis->profunditas + I);
                            gss_praedecessorem_addere(
                                nn, via_gss->basis);

                            ref_add2 = (LapifexGSSNodus**)xar_addere(frons);
                            si (ref_add2) *ref_add2 = nn;

                            novus_idx2 = (s32)((i32)xar_numerus(frons) - I);

                            reductiones_addere_ad_worklist(
                                worklist, glr->tabula,
                                novus_idx2, nn->status,
                                terminalis);

                            glr->numerus_furcationum++;
                        }
                    }
                }
                alioquin
                {
                    LapifexGSSNodus*  nodus_novus;
                    LapifexGSSNodus** ref_add;
                    s32               novus_idx;

                    nodus_novus = gss_nodus_creare(
                        glr, goto_st, valor_novus,
                        via_gss->basis->profunditas + I);
                    gss_praedecessorem_addere(
                        nodus_novus, via_gss->basis);

                    ref_add = (LapifexGSSNodus**)xar_addere(frons);
                    si (ref_add) *ref_add = nodus_novus;

                    novus_idx = (s32)((i32)xar_numerus(frons) - I);

                    reductiones_addere_ad_worklist(
                        worklist, glr->tabula,
                        novus_idx, nodus_novus->status,
                        terminalis);

                    glr->numerus_furcationum++;
                }
            }
        }

        /* Deactivare nodos qui solum habent reductiones */
        per (fi = ZEPHYRUM; fi < (i32)xar_numerus(frons); fi++)
        {
            LapifexGSSNodus** ref_fi =
                (LapifexGSSNodus**)xar_obtinere(frons, fi);
            LapifexGSSNodus*  nodus;
            Xar*              actiones;
            i32               num_act;
            i32               ai;
            b32               habet_shift_vel_accept;

            si (!ref_fi || !*ref_fi || !(*ref_fi)->vivus) perge;
            nodus = *ref_fi;

            actiones = lapifex_actiones_quaerere(
                glr->tabula, nodus->status, terminalis);
            num_act = actiones ? (i32)xar_numerus(actiones) : ZEPHYRUM;

            habet_shift_vel_accept = FALSUM;
            per (ai = ZEPHYRUM; ai < num_act; ai++)
            {
                LapifexActioIntroitus* act =
                    (LapifexActioIntroitus*)xar_obtinere(actiones, ai);
                si (act && (act->actio == LAPIFEX_ACTIO_TRANSPONERE ||
                            act->actio == LAPIFEX_ACTIO_ACCIPERE))
                {
                    habet_shift_vel_accept = VERUM;
                    frange;
                }
            }

            si (!habet_shift_vel_accept)
            {
                nodus->vivus = FALSUM;
            }
        }

        /* ========================================
         * PASSUS 2: ACCEPT
         * ======================================== */
        per (fi = ZEPHYRUM; fi < (i32)xar_numerus(frons); fi++)
        {
            LapifexGSSNodus** ref_fi;
            LapifexGSSNodus*  nodus;
            Xar*              actiones;
            i32               num_act;
            i32               ai;

            ref_fi = (LapifexGSSNodus**)xar_obtinere(frons, fi);
            si (!ref_fi || !*ref_fi || !(*ref_fi)->vivus) perge;
            nodus = *ref_fi;

            actiones = lapifex_actiones_quaerere(
                glr->tabula, nodus->status, terminalis);
            num_act = actiones ? (i32)xar_numerus(actiones) : ZEPHYRUM;

            per (ai = ZEPHYRUM; ai < num_act; ai++)
            {
                LapifexActioIntroitus* actio =
                    (LapifexActioIntroitus*)xar_obtinere(actiones, ai);

                si (actio && actio->actio == LAPIFEX_ACTIO_ACCIPERE)
                {
                    i32 num_pred;
                    i32 pi;

                    num_pred = (i32)xar_numerus(
                        nodus->praedecessores);

                    per (pi = ZEPHYRUM; pi < num_pred; pi++)
                    {
                        LapifexGSSNodus** p_ref =
                            (LapifexGSSNodus**)xar_obtinere(
                                nodus->praedecessores, pi);
                        si (p_ref && *p_ref)
                        {
                            s64* fv_ptr = (s64*)xar_addere(
                                fructus_valori);
                            si (fv_ptr)
                            {
                                *fv_ptr = (*p_ref)->valor;
                            }
                        }
                    }
                    nodus->vivus = FALSUM;
                }
            }
        }

        si ((i32)xar_numerus(fructus_valori) > ZEPHYRUM)
        {
            frange;
        }

        /* ========================================
         * PASSUS 3: SHIFT
         * ======================================== */
        frons_nova = xar_creare(glr->piscina,
            (i32)magnitudo(LapifexGSSNodus*));

        per (fi = ZEPHYRUM; fi < (i32)xar_numerus(frons); fi++)
        {
            LapifexGSSNodus** ref_fi;
            LapifexGSSNodus*  nodus;
            Xar*              actiones;
            i32               num_act;
            i32               ai;

            ref_fi = (LapifexGSSNodus**)xar_obtinere(frons, fi);
            si (!ref_fi || !*ref_fi || !(*ref_fi)->vivus) perge;
            nodus = *ref_fi;

            actiones = lapifex_actiones_quaerere(
                glr->tabula, nodus->status, terminalis);
            num_act = actiones ? (i32)xar_numerus(actiones) : ZEPHYRUM;

            per (ai = ZEPHYRUM; ai < num_act; ai++)
            {
                LapifexActioIntroitus* actio =
                    (LapifexActioIntroitus*)xar_obtinere(actiones, ai);

                si (actio && actio->actio == LAPIFEX_ACTIO_TRANSPONERE)
                {
                    s64  val;
                    s32  existens_idx;

                    val = (positio < numerus) ?
                        signa[positio].valor : 0;

                    existens_idx = in_fronte_invenire(
                        frons_nova, actio->valor);

                    si (existens_idx >= ZEPHYRUM)
                    {
                        LapifexGSSNodus** ref_exist =
                            (LapifexGSSNodus**)xar_obtinere(
                                frons_nova, (i32)existens_idx);
                        si (ref_exist && *ref_exist)
                        {
                            gss_praedecessorem_addere(
                                *ref_exist, nodus);
                            glr->numerus_fusionum++;
                        }
                    }
                    alioquin
                    {
                        LapifexGSSNodus*  nodus_novus;
                        LapifexGSSNodus** ref_add;

                        nodus_novus = gss_nodus_creare(
                            glr, actio->valor, val,
                            nodus->profunditas + I);
                        gss_praedecessorem_addere(
                            nodus_novus, nodus);

                        ref_add = (LapifexGSSNodus**)xar_addere(frons_nova);
                        si (ref_add) *ref_add = nodus_novus;
                    }
                }
            }
        }

        si ((i32)xar_numerus(frons_nova) == ZEPHYRUM)
        {
            fructus.error.est_error = VERUM;
            fructus.error.terminalis = terminalis;
            fructus.error.positio = positio;
            redde fructus;
        }

        frons = frons_nova;
        positio++;
    }

    /* Construere fructum */
    {
        i32 num_fructus = (i32)xar_numerus(fructus_valori);

        si (num_fructus > ZEPHYRUM)
        {
            i32 i;

            fructus.successus = VERUM;
            fructus.numerus_fructuum = (s32)num_fructus;
            fructus.ambiguus = (num_fructus > I) ? VERUM : FALSUM;

            fructus.valori = (s64*)piscina_allocare(
                glr->piscina,
                (memoriae_index)((i32)magnitudo(s64) * num_fructus));

            per (i = ZEPHYRUM; i < num_fructus; i++)
            {
                s64* val = (s64*)xar_obtinere(fructus_valori, i);
                si (val) fructus.valori[i] = *val;
            }
        }
    }

    redde fructus;
}
