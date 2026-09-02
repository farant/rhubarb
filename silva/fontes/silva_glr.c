/* silva_glr.c - Implementatio motoris GLR (Phase 4 Chunk A)
 *
 * Ordo passus per lexema (refinamentum arbor2):
 *   1. Nodi frontis processantur: TRANSPONERE -> frons_nova,
 *      REDUCERE -> nodi recentes in reducenda (FIFO), ACCIPERE
 *      -> acceptati.
 *   2. Reducenda exhauriuntur FIFO (undae latitudinis-primae) -
 *      nodi reductione creati easdem actiones subeunt.
 *   3. Acceptati colliguntur; si nulli et frons_nova vacua -> error.
 *
 * Enumeratio viarum (lapifex): reductio longitudinis N omnes vias N
 * graduum per praedecessores enumerat; quaeque via historiam parsurae
 * unam repraesentat; valores dextri sinistrorsum-dextrorsum implentur.
 *
 * NB monstratores Xar stabiles trans additiones (segmenta fixa, sine
 * reallocatio - vide xar.c); reducenda dum exhauritur crescere potest.
 */

#include "silva_glr.h"
#include <stdio.h>


/* ==================================================
 * Via per GSS
 * ================================================== */

nomen structura {
       SilvaValor  valores[SILVA_GLR_DEXTRUM_MAXIMUM];
    SilvaGSSNodus* basis;     /* nodus sub via - fons goto */
} SilvaGSSVia;

/* Introitus indicis reducendorum (spec-v2 par 12): quisque nodus
 * reductione creatus sub clavi (status, basis) registratur. Fenestra
 * transmutationis = UNA exhaustio (index per lexema recens - regula
 * ordinis quae periculum retroactivitatis claudit). */
nomen structura {
              s32  status;
    SilvaGSSNodus* basis;
    SilvaGSSNodus* nodus;
              b32  exhaustus;         /* reductiones eius iam cucurrerunt */
              b32  est_ambiguum;      /* valor iam involucrum AMBIGUUS */
       SilvaValor  interpretationes;  /* prospectus - ab motore tentus, ne
                                       * nodum umquam legere debeat */
} SilvaIndexIntroitus;

/* Contextus passus unius lexematis */
nomen structura {
           s32  terminale;
    SilvaToken* lexema;           /* NIHIL in prospectu $ */
           Xar* index;            /* SilvaIndexIntroitus (per valorem) */
           Xar* reducenda;        /* SilvaIndexIntroitus* (FIFO) */
           Xar* frons_nova;       /* SilvaGSSNodus* */
           Xar* acceptati;        /* SilvaGSSNodus* */
       Piscina* piscina_arborum;
} SilvaGLRPassus;


/* ==================================================
 * Aequalitas structuralis (recursio generica - modellum
 * nodorum uniforme nullos casus per-genus poscit)
 * ================================================== */

interior b32
_valores_aequales (
    SilvaValor a,
    SilvaValor b);

interior b32
_nodi_aequales (
    constans SilvaNodus* a,
    constans SilvaNodus* b)
{
    i32 i;

    si (a == b)
    {
        redde VERUM;
    }
    si (   a == NIHIL || b == NIHIL || a->genus != b->genus
        || a->numerus_locorum != b->numerus_locorum)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < a->numerus_locorum; i++)
    {
        si (!_valores_aequales(a->loci[i], b->loci[i]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_valores_aequales (
    SilvaValor a,
    SilvaValor b)
{
    si (a.genus != b.genus)
    {
        redde FALSUM;
    }
    commutatio (a.genus)
    {
    casus SILVA_VALOR_NIHIL:
        redde VERUM;
    casus SILVA_VALOR_NODUS:
        redde _nodi_aequales(a.datum.nodus, b.datum.nodus);
    casus SILVA_VALOR_TOKEN:
        /* IDENTITAS monstratoris: parsurae eiusdem fluxus eadem
         * lexemata communicant; aequalitas textus positiones fontis
         * diversas false funderet */
        redde (a.datum.token == b.datum.token) ? VERUM : FALSUM;
    casus SILVA_VALOR_LISTA:
    {
        i32 n = silva_valor_lista_numerus(a);
        i32 i;

        si (n != silva_valor_lista_numerus(b))
        {
            redde FALSUM;
        }
        si (a.datum.lista.xar == b.datum.lista.xar)
        {
            redde VERUM;  /* idem repositorium + eadem mensura */
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            SilvaValor* ea = silva_valor_lista_obtinere(a, i);
            SilvaValor* eb = silva_valor_lista_obtinere(b, i);

            si (   ea == NIHIL || eb == NIHIL
                || !_valores_aequales(*ea, *eb))
            {
                redde FALSUM;
            }
        }
        redde VERUM;
    }
    casus SILVA_VALOR_INDEX:
        redde (a.datum.index == b.datum.index) ? VERUM : FALSUM;
    ordinarius:
        redde FALSUM;
    }
}


/* ==================================================
 * Index reducendorum + compactio + transmutatio
 * ================================================== */

/* Quaerere clavem exactam; status_visus_out = VERUM si status idem
 * cum basi ALIA visus (classis Farshi - numeratur, non compingi
 * potest) */
interior SilvaIndexIntroitus*
_index_quaerere (
              Xar* index,
              s32  status,
    SilvaGSSNodus* basis,
              b32* status_visus_out)
{
    i32 i;
    i32 numerus = xar_numerus(index);

    *status_visus_out = FALSUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaIndexIntroitus* introitus =
            (SilvaIndexIntroitus*)xar_obtinere(index, i);

        si (introitus == NIHIL) perge;
        si (introitus->status == status)
        {
            si (introitus->basis == basis)
            {
                redde introitus;
            }
            *status_visus_out = VERUM;
        }
    }
    redde NIHIL;
}

/* Compactio ANTE exhaustionem: nihil adhuc valorem consumpsit, ergo
 * valor GSS simpliciter religatur - identitas non requiritur */
interior vacuum
_compingere (
               SilvaGLR* glr,
         SilvaGLRPassus* passus,
    SilvaIndexIntroitus* introitus,
             SilvaValor  valor_novus)
{
    SilvaValor interps;
    SilvaValor involucrum;

    si (!introitus->est_ambiguum)
    {
        interps = silva_valor_lista_nova(passus->piscina_arborum);
        interps = silva_valor_lista_appendere(passus->piscina_arborum,
            interps, introitus->nodus->valor);
    }
    alioquin
    {
        interps = introitus->interpretationes;
    }
    interps = silva_valor_lista_appendere(passus->piscina_arborum,
        interps, valor_novus);
    si (interps.genus != SILVA_VALOR_LISTA)
    {
        fprintf(stderr, "silva_glr: compactio fracta (lista)\n");
        redde;
    }
    involucrum = glr->fabrica(passus->piscina_arborum, interps,
        ZEPHYRUM);
    si (involucrum.genus != SILVA_VALOR_NODUS)
    {
        fprintf(stderr, "silva_glr: fabrica ambigui fracta\n");
        redde;
    }
    introitus->nodus->valor      = involucrum;
    introitus->est_ambiguum      = VERUM;
    introitus->interpretationes  = interps;
    glr->fusiones++;
}

/* TRANSMUTATIO (spec-v2 par 12, simulatio ⑤): brachium serum ad
 * clavem iam exhaustam - valor eius iam per COPIAM VALORIS in
 * constructiones fluxit, sed monstratorem communicant. Contenta per
 * vias probatas aedificantur (creare/ponere/fabrica), deinde UNA
 * copia structurae super allocationem stabilem - identitas manet,
 * nullum praesidium S32 praeteritur. */
interior vacuum
_transmutare (
               SilvaGLR* glr,
         SilvaGLRPassus* passus,
    SilvaIndexIntroitus* introitus,
             SilvaValor  valor_novus)
{
    SilvaNodus* sedes = introitus->nodus->valor.datum.nodus;
    SilvaValor  interps;
    SilvaValor  involucrum;

    si (sedes == NIHIL)
    {
        redde;
    }
    si (!introitus->est_ambiguum)
    {
        /* Clon contenta vetera servat: copia structurae seriem loci
         * VETEREM ad clonem transfert (sedes seriem novam ambigui
         * accipiet - series vetus clonis sola fit) */
        SilvaNodus* clon = (SilvaNodus*)piscina_allocare(
            passus->piscina_arborum,
            (memoriae_index)magnitudo(SilvaNodus));

        si (clon == NIHIL)
        {
            redde;
        }
        *clon    = *sedes;
        interps  = silva_valor_lista_nova(passus->piscina_arborum);
        interps = silva_valor_lista_appendere(passus->piscina_arborum,
            interps, silva_valor_nodus(clon));
    }
    alioquin
    {
        interps = introitus->interpretationes;
    }
    interps = silva_valor_lista_appendere(passus->piscina_arborum,
        interps, valor_novus);
    si (interps.genus != SILVA_VALOR_LISTA)
    {
        fprintf(stderr, "silva_glr: transmutatio fracta (lista)\n");
        redde;
    }
    involucrum = glr->fabrica(passus->piscina_arborum, interps,
        ZEPHYRUM);
    si (   involucrum.genus       != SILVA_VALOR_NODUS
        || involucrum.datum.nodus == NIHIL)
    {
        fprintf(stderr, "silva_glr: fabrica ambigui fracta\n");
        redde;
    }
    *sedes                       = *involucrum.datum.nodus;  /* identitas religata */
    introitus->est_ambiguum      = VERUM;
    introitus->interpretationes  = interps;
    glr->transmutationes++;
}


/* ==================================================
 * Nodi GSS
 * ================================================== */

interior SilvaGSSNodus*
_nodus_creare (
      SilvaGLR* glr,
           s32  status,
    SilvaValor  valor,
    SilvaToken* lexema)
{
    SilvaGSSNodus* nodus;

    nodus = (SilvaGSSNodus*)piscina_allocare(glr->piscina,
        (memoriae_index)magnitudo(SilvaGSSNodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->status  = status;
    nodus->valor   = valor;
    nodus->praedecessores = xar_creare(glr->piscina,
        (i32)magnitudo(SilvaGSSNodus*));
    nodus->lexema = lexema;
    glr->nodi_creati++;
    redde nodus;
}

interior vacuum
_praedecessorem_addere (
    SilvaGSSNodus* nodus,
    SilvaGSSNodus* praedecessor)
{
    SilvaGSSNodus** slot;
              i32   i;
              i32   numerus;

    si (nodus == NIHIL || praedecessor == NIHIL)
    {
        redde;
    }
    numerus = xar_numerus(nodus->praedecessores);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGSSNodus** existens =
            (SilvaGSSNodus**)xar_obtinere(nodus->praedecessores, i);

        si (existens != NIHIL && *existens == praedecessor)
        {
            redde;  /* iam adest */
        }
    }
    slot = (SilvaGSSNodus**)xar_addere(nodus->praedecessores);
    si (slot != NIHIL)
    {
        *slot = praedecessor;
    }
}


/* ==================================================
 * Enumeratio viarum
 * ================================================== */

interior vacuum
_vias_recursio (
    SilvaGSSNodus* nodus,
              i32  profunditas,
              i32  gradus,
       SilvaValor* partiales,
              Xar* viae)
{
    si (profunditas == gradus)
    {
        SilvaGSSVia* via = (SilvaGSSVia*)xar_addere(viae);

        si (via != NIHIL)
        {
            i32 i;

            per (i = ZEPHYRUM; i < gradus; i++)
            {
                via->valores[i] = partiales[i];
            }
            via->basis = nodus;
        }
        redde;
    }

    si (nodus == NIHIL)
    {
        redde;
    }

    {
        i32 idx = gradus - I - profunditas;
        i32 numerus_praed;
        i32 pi;

        si (idx >= (i32)SILVA_GLR_DEXTRUM_MAXIMUM)
        {
            redde;
        }
        partiales[idx] = nodus->valor;

        numerus_praed = xar_numerus(nodus->praedecessores);
        per (pi = ZEPHYRUM; pi < numerus_praed; pi++)
        {
            SilvaGSSNodus** praed =
                (SilvaGSSNodus**)xar_obtinere(nodus->praedecessores,
                pi);

            si (praed != NIHIL && *praed != NIHIL)
            {
                _vias_recursio(*praed, profunditas + I, gradus,
                    partiales, viae);
            }
        }
    }
}

interior Xar*
_vias_enumerare (
         SilvaGLR* glr,
    SilvaGSSNodus* culmen,
              i32  gradus)
{
           Xar* viae;
    SilvaValor  partiales[SILVA_GLR_DEXTRUM_MAXIMUM];

    /* Effimera reusa - non recens: vide notam in SilvaGLR (una viva
     * simul; contenta ante reditum plene scripta, ergo vacatio tuta) */
    viae = glr->viae_effimerae;
    xar_vacare(viae);

    si (gradus == ZEPHYRUM)
    {
        SilvaGSSVia* via = (SilvaGSSVia*)xar_addere(viae);

        si (via != NIHIL)
        {
            via->basis = culmen;
        }
        redde viae;
    }

    _vias_recursio(culmen, ZEPHYRUM, gradus, partiales, viae);
    redde viae;
}


/* ==================================================
 * Quaestio tabularum (laminae per statum)
 * ================================================== */

/* Omnes actiones pro (status, terminale): monstratorem ad primam et
 * numerum reddit (ordinatae adiacentes - in validatione impositum). */
interior i32
_actiones_invenire (
    constans SilvaTabulaCocta*  tabula,
                          s32   status,
                          s32   terminale,
       constans SilvaTabActio** prima_out)
{
    constans SilvaTabStatus* lamina;
                        i32  i;
                        i32  numerus = ZEPHYRUM;

        *prima_out = NIHIL;
    si (status < ZEPHYRUM || status >= (s32)tabula->numerus_statuum)
    {
        redde ZEPHYRUM;
    }
    lamina = &tabula->status[status];

    /* Index densus (2026-09-02): cella -> prima actio, cursus adiacens
     * numeratur; sine indice scansio laminae ut olim */
    si (tabula->actiones_index != NIHIL)
    {
        s32 cella;
        i32 finis;

        si (   terminale < -I
            || terminale >= (s32)tabula->numerus_terminalium)
        {
            redde ZEPHYRUM;
        }
        cella = tabula->actiones_index[
            status * ((s32)tabula->numerus_terminalium + I)
            + (terminale + I)];
        si (cella < ZEPHYRUM)
        {
            redde ZEPHYRUM;
        }
        *prima_out = &tabula->actiones[cella];
        finis = lamina->actiones_offset + lamina->actiones_numerus;
        per (i = (i32)cella;
             i < finis && tabula->actiones[i].terminalis == terminale;
             i++)
        {
            numerus++;
        }
        redde numerus;
    }

    per (i = ZEPHYRUM; i < lamina->actiones_numerus; i++)
    {
        constans SilvaTabActio* actio =
            &tabula->actiones[lamina->actiones_offset + i];

        si (actio->terminalis == terminale)
        {
            si (*prima_out == NIHIL)
            {
                *prima_out = actio;
            }
            numerus++;
        }
        alioquin si (*prima_out != NIHIL)
        {
            frange;  /* ordinatae: praeter cursum */
        }
    }
    redde numerus;
}

interior s32
_goto_quaerere (
    constans SilvaTabulaCocta* tabula,
                          s32  status,
                          s32  non_terminalis)
{
    constans SilvaTabStatus* lamina;
                        i32  i;

        si (status < ZEPHYRUM || status >= (s32)tabula->numerus_statuum)
        {
        redde -I;
        }
    lamina = &tabula->status[status];

    si (tabula->goto_index != NIHIL)
    {
        s32 nt  = non_terminalis - (s32)tabula->numerus_terminalium;
        s32 lat = (s32)tabula->numerus_symbolorum
                - (s32)tabula->numerus_terminalium;

        si (nt < ZEPHYRUM || nt >= lat)
        {
            redde -I;
        }
        redde tabula->goto_index[status * lat + nt];
    }

    per (i = ZEPHYRUM; i < lamina->goto_numerus; i++)
    {
        constans SilvaTabGoto* g =
            &tabula->goto_introitus[lamina->goto_offset + i];

        si (g->non_terminalis == non_terminalis)
        {
            redde g->status_novus;
        }
    }
    redde -I;
}

s32
silva_glr_terminale_ex_genere (
    constans SilvaTabulaCocta* tabula,
    SilvaLexemaGenus           genus)
{
    i32 i;

    per (i = ZEPHYRUM; i < tabula->numerus_terminalium; i++)
    {
        si (tabula->symbola[i].genus_lexematis == genus)
        {
            redde (s32)i;
        }
    }
    redde SILVA_GLR_TERMINALE_IGNOTUM;
}


/* ==================================================
 * Validatio tabulae
 * ================================================== */

b32
silva_glr_tabulam_validare (
                      Piscina* piscina,
    constans SilvaTabulaCocta* tabula)
{
    i32 st;
    i32 i;

    si (   piscina == NIHIL || tabula == NIHIL
        || tabula->numerus_statuum == ZEPHYRUM
        || tabula->symbola == NIHIL || tabula->productiones == NIHIL
        || tabula->status == NIHIL || tabula->actiones == NIHIL
        || tabula->goto_introitus == NIHIL)
    {
        fprintf(stderr, "silva_glr: tabula vacua aut manca\n");
        redde FALSUM;
    }

    /* Productiones: longitudo intra terminum, sinistrum non-terminale */
    per (i = ZEPHYRUM; i < tabula->numerus_productionum; i++)
    {
        constans SilvaTabProductio* p = &tabula->productiones[i];

        si (p->longitudo > (i32)SILVA_GLR_DEXTRUM_MAXIMUM)
        {
            fprintf(stderr, "silva_glr: productio %d longior quam %d\n",
                (int)i, (int)SILVA_GLR_DEXTRUM_MAXIMUM);
            redde FALSUM;
        }
        si (   p->sinistrum < (s32)tabula->numerus_terminalium
            || p->sinistrum >= (s32)tabula->numerus_symbolorum)
        {
            fprintf(stderr,
                "silva_glr: productio %d sinistrum non non-terminale\n",
                (int)i);
            redde FALSUM;
        }
    }

    /* Laminae statuum: fines, introitus, ordinatio, goto pro LHS */
    per (st = ZEPHYRUM; st < tabula->numerus_statuum; st++)
    {
        constans SilvaTabStatus* lamina           = &tabula->status[st];
                            s32  terminale_prius  = -II;

        si (   lamina->actiones_offset + lamina->actiones_numerus
                > tabula->numerus_actionum
            || lamina->goto_offset + lamina->goto_numerus
                > tabula->numerus_goto)
        {
            fprintf(stderr, "silva_glr: lamina status %d extra fines\n",
                (int)st);
            redde FALSUM;
        }

        per (i = ZEPHYRUM; i < lamina->actiones_numerus; i++)
        {
            constans SilvaTabActio* actio =
                &tabula->actiones[lamina->actiones_offset + i];

            si (   actio->terminalis < -I
                || actio->terminalis
                    >= (s32)tabula->numerus_terminalium)
            {
                fprintf(stderr,
                    "silva_glr: status %d terminale %d extra fines\n",
                    (int)st, (int)actio->terminalis);
                redde FALSUM;
            }
            si (actio->terminalis < terminale_prius)
            {
                fprintf(stderr,
                    "silva_glr: status %d actiones non ordinatae\n",
                    (int)st);
                redde FALSUM;
            }
            terminale_prius = actio->terminalis;

            commutatio (actio->actio)
            {
            casus SILVA_TAB_ACTIO_TRANSPONERE:
                si (   actio->valor < ZEPHYRUM
                    || actio->valor >= (s32)tabula->numerus_statuum)
                {
                    fprintf(stderr,
                        "silva_glr: status %d translatio extra fines\n",
                        (int)st);
                    redde FALSUM;
                }
                frange;
            casus SILVA_TAB_ACTIO_REDUCERE:
                si (   actio->valor < ZEPHYRUM
                    || actio->valor
                        >= (s32)tabula->numerus_productionum)
                {
                    fprintf(stderr,
                        "silva_glr: status %d reductio extra fines\n",
                        (int)st);
                    redde FALSUM;
                }
                /* Goto absens = circuitus infinitus, non defectus
                 * mundus (arbor2 worklog) - LHS goto alicubi habere
                 * debet. */
                {
                    s32 lhs =
                        tabula->productiones[actio->valor].sinistrum;
                    b32 inventum = FALSUM;
                    i32 gi;

                    per (gi = ZEPHYRUM; gi < tabula->numerus_goto; gi++)
                    {
                        si (tabula->goto_introitus[gi].non_terminalis
                            == lhs)
                        {
                            inventum = VERUM;
                            frange;
                        }
                    }
                    si (!inventum)
                    {
                        fprintf(stderr,
                            "silva_glr: symbolum %d sine goto ullo\n",
                            (int)lhs);
                        redde FALSUM;
                    }
                }
                frange;
            casus SILVA_TAB_ACTIO_ACCIPERE:
            casus SILVA_TAB_ACTIO_RECUPERARE:
            casus SILVA_TAB_ACTIO_ERRARE:
                frange;
            ordinarius:
                fprintf(stderr,
                    "silva_glr: status %d actio ignota %d\n",
                    (int)st, (int)actio->actio);
                redde FALSUM;
            }
        }

        per (i = ZEPHYRUM; i < lamina->goto_numerus; i++)
        {
            constans SilvaTabGoto* g =
                &tabula->goto_introitus[lamina->goto_offset + i];

            si (   g->non_terminalis < (s32)tabula->numerus_terminalium
                || g->non_terminalis >= (s32)tabula->numerus_symbolorum
                || g->status_novus < ZEPHYRUM
                || g->status_novus   >= (s32)tabula->numerus_statuum)
            {
                fprintf(stderr,
                    "silva_glr: status %d goto extra fines\n", (int)st);
                redde FALSUM;
            }
        }
    }

    /* Unicitas symboli accessus (spec-v2 par 12.2, simulatio ⑤ C3):
     * quisque status uno solo symbolo intratur - proprietas LR quam
     * index reducendorum praesumit (nodi translatione creati cum
     * nodis reductione creatis in statu collidere non possunt).
     * Assertio ex tabulis, non assumptio ex theoria. */
        /* Indices densi (si adsunt): cella quaeque cum scansione laminae
     * congruat - artificium ex laminis eisdem coctum, sed probatum,
     * non creditum */
    si (tabula->actiones_index != NIHIL || tabula->goto_index != NIHIL)
    {
        si (   tabula->actiones_index == NIHIL
            || tabula->goto_index     == NIHIL)
        {
            fprintf(stderr, "silva_glr: index densus dimidiatus\n");
            redde FALSUM;
        }
        per (st = ZEPHYRUM; st < tabula->numerus_statuum; st++)
        {
            constans SilvaTabStatus* lamina = &tabula->status[st];
            s32 t;
            s32 nt;
            s32 lat_act = (s32)tabula->numerus_terminalium + I;
            s32 lat_nt  = (s32)tabula->numerus_symbolorum
                        - (s32)tabula->numerus_terminalium;

            per (t = -I; t < (s32)tabula->numerus_terminalium; t++)
            {
                s32 exspectata = -I;
                s32 cella;

                per (i = ZEPHYRUM; i < lamina->actiones_numerus; i++)
                {
                    si (tabula->actiones[lamina->actiones_offset + i]
                            .terminalis == t)
                    {
                        exspectata = (s32)(lamina->actiones_offset + i);
                        frange;
                    }
                }
                cella = tabula->actiones_index[(s32)st * lat_act + (t
                    + I)];
                si (cella != exspectata)
                {
                    fprintf(stderr,
                        "silva_glr: index actionum st%d t%d: %d"
                        " pro %d\n", (int)st, (int)t, (int)cella,
                        (int)exspectata);
                    redde FALSUM;
                }
            }
            per (nt = ZEPHYRUM; nt < lat_nt; nt++)
            {
                s32 exspectata = -I;
                s32 cella;

                per (i = ZEPHYRUM; i < lamina->goto_numerus; i++)
                {
                    constans SilvaTabGoto* g =
                        &tabula->goto_introitus[lamina->goto_offset
                            + i];

                    si (g->non_terminalis
                        == nt + (s32)tabula->numerus_terminalium)
                    {
                        exspectata = g->status_novus;
                        frange;
                    }
                }
                cella = tabula->goto_index[(s32)st * lat_nt + nt];
                si (cella != exspectata)
                {
                    fprintf(stderr,
                        "silva_glr: index goto st%d nt%d: %d"
                        " pro %d\n", (int)st, (int)nt, (int)cella,
                        (int)exspectata);
                    redde FALSUM;
                }
            }
        }
    }

    {
        s32* accessus;
        i32  st2;

        accessus = (s32*)piscina_allocare(piscina,
            (memoriae_index)((memoriae_index)magnitudo(s32)
                * (memoriae_index)tabula->numerus_statuum));
        si (accessus == NIHIL)
        {
            redde FALSUM;
        }
        per (st2 = ZEPHYRUM; st2 < tabula->numerus_statuum; st2++)
        {
            accessus[st2] = -I;  /* nondum intratus */
        }
        per (st2 = ZEPHYRUM; st2 < tabula->numerus_statuum; st2++)
        {
            constans SilvaTabStatus* lamina = &tabula->status[st2];

            per (i = ZEPHYRUM; i < lamina->actiones_numerus; i++)
            {
                constans SilvaTabActio* actio =
                    &tabula->actiones[lamina->actiones_offset + i];

                si (actio->actio == (s32)SILVA_TAB_ACTIO_TRANSPONERE)
                {
                    si (   accessus[actio->valor] != -I
                        && accessus[actio->valor] != actio->terminalis)
                    {
                        fprintf(stderr, "silva_glr: status %d symbolis "
                            "pluribus intratur (%d, %d)\n",
                            (int)actio->valor,
                            (int)accessus[actio->valor],
                            (int)actio->terminalis);
                        redde FALSUM;
                    }
                    accessus[actio->valor] = actio->terminalis;
                }
            }
            per (i = ZEPHYRUM; i < lamina->goto_numerus; i++)
            {
                constans SilvaTabGoto* g =
                    &tabula->goto_introitus[lamina->goto_offset + i];

                si (   accessus[g->status_novus] != -I
                    && accessus[g->status_novus] != g->non_terminalis)
                {
                    fprintf(stderr, "silva_glr: status %d symbolis "
                        "pluribus intratur (%d, %d)\n",
                        (int)g->status_novus,
                        (int)accessus[g->status_novus],
                        (int)g->non_terminalis);
                    redde FALSUM;
                }
                accessus[g->status_novus] = g->non_terminalis;
            }
        }
    }

    redde VERUM;
}


/* ==================================================
 * Processio nodi unius (dispatch actionum)
 * ================================================== */

interior vacuum
_nodum_processare (
          SilvaGLR* glr,
     SilvaGSSNodus* nodus,
    SilvaGLRPassus* passus)
{
    constans SilvaTabActio* actio;
                       i32  numerus_actionum;
                       i32  ai;

    numerus_actionum = _actiones_invenire(glr->tabula, nodus->status,
        passus->terminale, &actio);

    per (ai = ZEPHYRUM; ai < numerus_actionum; ai++, actio++)
    {
        commutatio (actio->actio)
        {
        casus SILVA_TAB_ACTIO_TRANSPONERE:
        {
            SilvaGSSNodus*  novus;
            SilvaGSSNodus** slot;

            si (passus->lexema == NIHIL)
            {
                frange;  /* translatio sine lexemate impossibilis */
            }
            novus = _nodus_creare(glr, actio->valor,
                silva_valor_token(passus->lexema), passus->lexema);
            si (novus == NIHIL)
            {
                frange;
            }
            _praedecessorem_addere(novus, nodus);
            slot = (SilvaGSSNodus**)xar_addere(passus->frons_nova);
            si (slot != NIHIL)
            {
                *slot = novus;
            }
            frange;
        }

        casus SILVA_TAB_ACTIO_REDUCERE:
        {
            constans SilvaTabProductio* prod;
                                   Xar* viae;
                                   i32  numerus_viarum;
                                   i32  vi;

            prod = &glr->tabula->productiones[actio->valor];
            viae = _vias_enumerare(glr, nodus, prod->longitudo);
            numerus_viarum = xar_numerus(viae);

            per (vi = ZEPHYRUM; vi < numerus_viarum; vi++)
            {
                        SilvaGSSVia* via;
                         SilvaValor  valor_novus;
                                s32  goto_status;
                SilvaIndexIntroitus* introitus;
                                b32  status_visus;

                via = (SilvaGSSVia*)xar_obtinere(viae, vi);
                si (via == NIHIL || via->basis == NIHIL)
                {
                    perge;
                }

                valor_novus = glr->constructor(passus->piscina_arborum,
                    actio->valor, via->valores);

                goto_status = _goto_quaerere(glr->tabula,
                    via->basis->status, prod->sinistrum);
                si (goto_status < ZEPHYRUM)
                {
                    /* Validatio praeterita - corruptio tabulae */
                    fprintf(stderr,
                        "silva_glr: goto absens (status %d, symbolum %d)"
                        " - via moritur\n",
                        (int)via->basis->status, (int)prod->sinistrum);
                    perge;
                }
                glr->reductiones_factae++;

                /* Dispatch compactionis (spec-v2 par 12): clavis
                 * (status, basis) reconventiones classificat */
                introitus = _index_quaerere(passus->index, goto_status,
                    via->basis, &status_visus);

                si (introitus == NIHIL)
                {
                    /* Clavis nova - nodus recens registratur */
                          SilvaGSSNodus*  novus;
                    SilvaIndexIntroitus*  recens;
                    SilvaIndexIntroitus** slot;

                    si (status_visus)
                    {
                        /* margo novus ad statum visum (classis
                         * Farshi): brachium separatum, numeratum */
                        glr->eventa_marginis_novi++;
                    }
                    novus = _nodus_creare(glr, goto_status, valor_novus,
                        nodus->lexema);
                    si (novus == NIHIL)
                    {
                        perge;
                    }
                    _praedecessorem_addere(novus, via->basis);

                    recens = (SilvaIndexIntroitus*)xar_addere(
                        passus->index);
                    si (recens == NIHIL)
                    {
                        perge;
                    }
                    recens->status            = goto_status;
                    recens->basis             = via->basis;
                    recens->nodus             = novus;
                    recens->exhaustus         = FALSUM;
                    recens->est_ambiguum      = FALSUM;
                    recens->interpretationes  = silva_valor_nihil();

                    slot = (SilvaIndexIntroitus**)xar_addere(
                        passus->reducenda);
                    si (slot != NIHIL)
                    {
                        *slot = recens;
                    }
                }
                alioquin si (_valores_aequales(introitus->nodus->valor,
                             valor_novus))
                {
                    /* Derivatio duplex - abicitur (superstes solus
                     * consistens manet) */
                    glr->fusiones++;
                }
                alioquin si (!introitus->exhaustus)
                {
                    _compingere(glr, passus, introitus, valor_novus);
                }
                alioquin si (introitus->nodus->valor.genus
                             == SILVA_VALOR_NODUS)
                {
                    _transmutare(glr, passus, introitus, valor_novus);
                }
                alioquin
                {
                    /* Exhaustum, valor non-NODUS (lista/lexema):
                     * identitas religanda non existit - brachium
                     * separatum, scala gratiosa. Introitus additus
                     * (exhauriendus) sed a clavi priore obumbratus. */
                          SilvaGSSNodus*  novus;
                    SilvaIndexIntroitus*  recens;
                    SilvaIndexIntroitus** slot;

                    glr->transmutationes_negatae++;
                    novus = _nodus_creare(glr, goto_status, valor_novus,
                        nodus->lexema);
                    si (novus == NIHIL)
                    {
                        perge;
                    }
                    _praedecessorem_addere(novus, via->basis);
                    recens = (SilvaIndexIntroitus*)xar_addere(
                        passus->index);
                    si (recens == NIHIL)
                    {
                        perge;
                    }
                    recens->status            = goto_status;
                    recens->basis             = via->basis;
                    recens->nodus             = novus;
                    recens->exhaustus         = FALSUM;
                    recens->est_ambiguum      = FALSUM;
                    recens->interpretationes  = silva_valor_nihil();
                    slot = (SilvaIndexIntroitus**)xar_addere(
                        passus->reducenda);
                    si (slot != NIHIL)
                    {
                        *slot = recens;
                    }
                }
            }
            frange;
        }

        casus SILVA_TAB_ACTIO_ACCIPERE:
        {
            SilvaGSSNodus** slot =
                (SilvaGSSNodus**)xar_addere(passus->acceptati);

            si (slot != NIHIL)
            {
                *slot = nodus;
            }
            frange;
        }

        casus SILVA_TAB_ACTIO_RECUPERARE:
            /* Recuperatio pretio-ducta post-substratum (par 8.2) -
             * clare deficere donec impleta */
            fprintf(stderr,
                "silva_glr: actio RECUPERARE nondum impleta\n");
            frange;

        ordinarius:
            frange;
        }
    }
}


/* ==================================================
 * API
 * ================================================== */

/* Fusio frontis translationis: nodi eiusdem status uniuntur per
 * unionem praedecessorum. Valores comparandi NON sunt: nodi frontis
 * translatione creantur et IDEM lexema portant (unicitas symboli
 * accessus in validatione probata). Ordo servatur (compactio in
 * loco). */
interior vacuum
_frontem_compingere (
    SilvaGLR* glr,
         Xar* frons)
{
    i32 i;
    i32 j;
    i32 numerus = xar_numerus(frons);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGSSNodus** slot_a =
            (SilvaGSSNodus**)xar_obtinere(frons, i);

        si (slot_a == NIHIL || *slot_a == NIHIL) perge;
        per (j = i + I; j < numerus; j++)
        {
            SilvaGSSNodus** slot_b =
                (SilvaGSSNodus**)xar_obtinere(frons, j);

            si (slot_b == NIHIL || *slot_b == NIHIL) perge;
            si ((*slot_a)->status == (*slot_b)->status)
            {
                i32 pi;

                per (pi = ZEPHYRUM;
                     pi < xar_numerus((*slot_b)->praedecessores); pi++)
                {
                    SilvaGSSNodus** praed =
                        (SilvaGSSNodus**)xar_obtinere(
                        (*slot_b)->praedecessores, pi);

                    si (praed != NIHIL && *praed != NIHIL)
                    {
                        _praedecessorem_addere(*slot_a, *praed);
                    }
                }
                *slot_b = NIHIL;
                glr->fusiones++;
            }
        }
    }

    /* Compactio in loco - ordo servatur */
    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGSSNodus** slot =
            (SilvaGSSNodus**)xar_obtinere(frons, i);

        si (slot != NIHIL && *slot != NIHIL)
        {
            si (i != j)
            {
                SilvaGSSNodus** dest =
                    (SilvaGSSNodus**)xar_obtinere(frons, j);

                si (dest != NIHIL)
                {
                    *dest = *slot;
                }
            }
            j++;
        }
    }
    xar_truncare(frons, j);
}

/* Numeratores in zephyrum */
interior vacuum
_statisticas_purgare (
    SilvaGLR* glr)
{
    glr->frons_maxima             = ZEPHYRUM;
    glr->nodi_creati              = ZEPHYRUM;
    glr->reductiones_factae       = ZEPHYRUM;
    glr->fusiones                 = ZEPHYRUM;
    glr->transmutationes          = ZEPHYRUM;
    glr->transmutationes_negatae  = ZEPHYRUM;
    glr->eventa_marginis_novi     = ZEPHYRUM;
}

/* Numeratores in fructum (productum queribile, par 12.2) */
interior vacuum
_statisticas_copiare (
    constans SilvaGLR* glr,
      SilvaGLRFructus* fructus)
{
    fructus->frons_maxima             = glr->frons_maxima;
    fructus->fusiones                 = glr->fusiones;
    fructus->transmutationes          = glr->transmutationes;
    fructus->transmutationes_negatae  = glr->transmutationes_negatae;
    fructus->eventa_marginis_novi     = glr->eventa_marginis_novi;
}

SilvaGLR*
silva_glr_creare (
                      Piscina* piscina,
    constans SilvaTabulaCocta* tabula,
          SilvaGLRConstructor  constructor,
       SilvaGLRFabricaAmbigui  fabrica)
{
    SilvaGLR* glr;

        si (   piscina == NIHIL || constructor == NIHIL
            || fabrica == NIHIL)
        {
        redde NIHIL;
        }
    /* Tabula NON hic validatur (2026-09-02): validatio O(actiones x
     * goto) = XIV M iterationes per parsuram (I-IV% temporis; plagulis
     * parvis plus) super artificium generatum COMMISSUM quod
     * probationes (probatio_silva_glr, probatio_silva_ambiguitas)
     * per silva_glr_tabulam_validare in omni cursu suitae iudicant.
     * Res cocta semel iudicatur, non per parsuram. Tabula manca
     * (NIHIL) tamen refutatur. */
    si (   tabula == NIHIL || tabula->numerus_statuum == ZEPHYRUM
        || tabula->status == NIHIL || tabula->actiones == NIHIL
        || tabula->goto_introitus == NIHIL)
    {
        redde NIHIL;
    }

    glr = (SilvaGLR*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaGLR));
    si (glr == NIHIL)
    {
        redde NIHIL;
    }
    glr->tabula       = tabula;
    glr->constructor  = constructor;
    glr->fabrica      = fabrica;
    glr->piscina      = piscina;
    /* Segmentum primum = I: reductio fere omnis viam UNAM habet */
    glr->viae_effimerae = xar_creare_cum_magnitudine(piscina,
        (i32)magnitudo(SilvaGSSVia), I);
    si (glr->viae_effimerae == NIHIL)
    {
        redde NIHIL;
    }
    glr->limen_frontis    = SILVA_GLR_LIMEN_FRONTIS_DEFALTUM;
    glr->pergere          = NIHIL;
    glr->pergere_datum    = NIHIL;
    glr->passus_pergendi  = SILVA_GLR_PASSUS_PERGENDI_DEFALTUM;
    _statisticas_purgare(glr);
    redde glr;
}

SilvaGLRFructus
silva_glr_parsare (
    SilvaGLR*     glr,
    constans Xar* lexemata,
    Piscina*      piscina_arborum)
{
    SilvaGLRFructus  fructus;
                Xar* frons;
                Xar* acceptati;
                i32  numerus_lexematum;
                i32  positio;
                Xar* frons_prima;
                Xar* frons_altera;
                Xar* reducenda;
                Xar* index;

    fructus.successus                = FALSUM;
    fructus.valor                    = silva_valor_nihil();
    fructus.lexema_finis             = NIHIL;
    fructus.numerus_acceptorum       = ZEPHYRUM;
    fructus.frons_maxima             = ZEPHYRUM;
    fructus.fusiones                 = ZEPHYRUM;
    fructus.transmutationes          = ZEPHYRUM;
    fructus.transmutationes_negatae  = ZEPHYRUM;
    fructus.eventa_marginis_novi     = ZEPHYRUM;
    fructus.est_error                = FALSUM;
    fructus.positio                  = ZEPHYRUM;
    fructus.terminalis               = ZEPHYRUM;
    fructus.status                   = ZEPHYRUM;
    fructus.est_ultra_limen          = FALSUM;
    fructus.est_intermissus          = FALSUM;

    si (glr == NIHIL || lexemata == NIHIL || piscina_arborum == NIHIL)
    {
        fructus.est_error = VERUM;
        redde fructus;
    }

    _statisticas_purgare(glr);

    numerus_lexematum  = xar_numerus(lexemata);
    positio            = ZEPHYRUM;

    /* Frons initialis: nodus unus in statu 0 */
    frons = xar_creare(glr->piscina, (i32)magnitudo(SilvaGSSNodus*));
    {
        SilvaGSSNodus*  initialis;
        SilvaGSSNodus** slot;

        initialis = _nodus_creare(glr, ZEPHYRUM, silva_valor_nihil(),
            NIHIL);
        slot = (SilvaGSSNodus**)xar_addere(frons);
        si (slot != NIHIL)
        {
            *slot = initialis;
        }
    }
        acceptati = xar_creare(glr->piscina,
            (i32)magnitudo(SilvaGSSNodus*));
    /* Scriptoria passus REUSA (2026-09-02): olim tria Xar per passum
     * lexematis creabantur (reducenda, frons_nova, index) - CV M
     * creationes in lib/stml.c, X% foliorum profili. Contenta intra
     * passum consumuntur (index per _index_quaerere passus sui,
     * reducenda exhausta, frons_nova frons proximi fit; introitus
     * indicis campos OMNES in creatione ponunt, ergo memoria stalis
     * innocua), ergo frons duplici alveo alternat et ceteri
     * xar_vacare purgantur - segmenta manent, capacitas passus
     * maximi servatur. */
    frons_prima   = frons;
    frons_altera  = xar_creare(glr->piscina,
        (i32)magnitudo(SilvaGSSNodus*));
    reducenda     = xar_creare(glr->piscina,
        (i32)magnitudo(SilvaGSSNodus*));
    index         = xar_creare(glr->piscina,
        (i32)magnitudo(SilvaIndexIntroitus));
    dum (VERUM)
    {
        SilvaGLRPassus passus;
                   i32 fi;
                   i32 cursor;

        /* Intermissio (Phase 7): interrogatio determinata per passum
         * lexematum; FALSUM = fractura munda (gubernator recuperat) */
        si (   glr->pergere != NIHIL
            && (glr->passus_pergendi <= I
                || (positio % glr->passus_pergendi) == ZEPHYRUM)
            && !glr->pergere(glr->pergere_datum))
        {
            fructus.est_error        = VERUM;
            fructus.est_intermissus  = VERUM;
            fructus.positio          = (s32)positio;
            _statisticas_copiare(glr, &fructus);
            redde fructus;
        }

        /* Prospectus: lexema currens aut $ ultra fluxum */
        passus.terminale  = SILVA_GLR_PROSPECTUS_FINIS;
        passus.lexema     = NIHIL;
        si (positio < numerus_lexematum)
        {
            SilvaToken** ref =
                (SilvaToken**)xar_obtinere(lexemata, positio);

            passus.lexema = (ref != NIHIL) ? *ref : NIHIL;
            si (passus.lexema != NIHIL)
            {
                passus.terminale = silva_glr_terminale_ex_genere(
                    glr->tabula, passus.lexema->genus);
                si (passus.terminale == SILVA_GLR_TERMINALE_IGNOTUM)
                {
                    fructus.est_error   = VERUM;
                    fructus.positio     = (s32)positio;
                    fructus.terminalis  = SILVA_GLR_TERMINALE_IGNOTUM;
                    _statisticas_copiare(glr, &fructus);
                    redde fructus;
                }
            }
        }

                passus.frons_nova = (frons
                    == frons_altera) ? frons_prima
                                                    : frons_altera;
        xar_vacare(passus.frons_nova);
        passus.reducenda  = reducenda;
        xar_vacare(reducenda);
        passus.index      = index;
        xar_vacare(index);
        passus.acceptati        = acceptati;
        passus.piscina_arborum  = piscina_arborum;
        xar_vacare(acceptati);

        /* Passus 1: frons; passus 2: exhaustio reducendorum (FIFO -
         * crescit dum exhauritur; introitus indicis per valorem in
         * Xar sedent, monstratores stabiles trans additiones).
         * exhaustus POST processionem signatur - fenestra
         * transmutationis his signis definitur. */
        per (fi = ZEPHYRUM; fi < xar_numerus(frons); fi++)
        {
            SilvaGSSNodus** ref =
                (SilvaGSSNodus**)xar_obtinere(frons, fi);

            si (ref != NIHIL && *ref != NIHIL)
            {
                _nodum_processare(glr, *ref, &passus);
            }
        }
        cursor = ZEPHYRUM;
        dum (cursor < xar_numerus(passus.reducenda))
        {
            SilvaIndexIntroitus** ref = (SilvaIndexIntroitus**)
                xar_obtinere(passus.reducenda, cursor);

            cursor++;
            si (ref != NIHIL && *ref != NIHIL && (*ref)->nodus != NIHIL)
            {
                _nodum_processare(glr, (*ref)->nodus, &passus);
                (*ref)->exhaustus = VERUM;
            }
        }

        /* Acceptatio: scala reconciliationis (spec-v2 par 12) -
         * valores praedecessorum omnium colliguntur; aequales
         * deduplicantur; radices vere diversae in AMBIGUUS
         * involvuntur (ambiguitas radicalis - scala gratiosa) */
        si (xar_numerus(acceptati) > ZEPHYRUM)
        {
            Xar* radices = xar_creare(glr->piscina,
                (i32)magnitudo(SilvaValor));
            i32 vias_acceptae = ZEPHYRUM;
            i32 ai;

            per (ai = ZEPHYRUM; ai < xar_numerus(acceptati); ai++)
            {
                SilvaGSSNodus* acceptus =
                    *(SilvaGSSNodus**)xar_obtinere(acceptati, ai);
                i32 pi;

                si (acceptus == NIHIL) perge;
                si (   ai                    == ZEPHYRUM
                    && acceptus->valor.genus == SILVA_VALOR_TOKEN)
                {
                    /* Lexema EOF translatum - trivia Phase 5 emittet */
                    fructus.lexema_finis = acceptus->valor.datum.token;
                }
                per (pi = ZEPHYRUM;
                     pi < xar_numerus(acceptus->praedecessores); pi++)
                {
                    SilvaGSSNodus** praed =
                        (SilvaGSSNodus**)xar_obtinere(
                        acceptus->praedecessores, pi);
                    b32 novum = VERUM;
                    i32 ri;

                    si (praed == NIHIL || *praed == NIHIL) perge;
                    vias_acceptae++;
                    per (ri = ZEPHYRUM; ri < xar_numerus(radices); ri++)
                    {
                        SilvaValor* r =
                            (SilvaValor*)xar_obtinere(radices, ri);

                        si (   r != NIHIL
                            && _valores_aequales(*r, (*praed)->valor))
                        {
                            novum = FALSUM;
                            frange;
                        }
                    }
                    si (novum)
                    {
                        SilvaValor* slot =
                            (SilvaValor*)xar_addere(radices);

                        si (slot != NIHIL)
                        {
                            *slot = (*praed)->valor;
                        }
                    }
                }
            }

            fructus.successus           = VERUM;
            fructus.numerus_acceptorum  = vias_acceptae;
            si (xar_numerus(radices) == I)
            {
                fructus.valor = *(SilvaValor*)xar_obtinere(radices,
                    ZEPHYRUM);
            }
            alioquin si (xar_numerus(radices) > I)
            {
                /* Radices diversae: involucrum AMBIGUUS radicale */
                SilvaValor interps =
                    silva_valor_lista_nova(piscina_arborum);
                i32 ri;

                per (ri = ZEPHYRUM; ri < xar_numerus(radices); ri++)
                {
                    SilvaValor* r =
                        (SilvaValor*)xar_obtinere(radices, ri);

                    si (r != NIHIL)
                    {
                        interps = silva_valor_lista_appendere(
                            piscina_arborum, interps, *r);
                    }
                }
                fructus.valor = glr->fabrica(piscina_arborum, interps,
                    ZEPHYRUM);
            }
            _statisticas_copiare(glr, &fructus);
            redde fructus;
        }

        /* Fusio frontis: nodi eiusdem status uniuntur (valores
         * translationis = IDEM lexema, ergo unio praedecessorum
         * sufficit - fusio valorum in exhaustione sola accidit) */
        _frontem_compingere(glr, passus.frons_nova);

        /* Frons vacua sine acceptatione = error parsurae */
        si (xar_numerus(passus.frons_nova) == ZEPHYRUM)
        {
            fructus.est_error   = VERUM;
            fructus.positio     = (s32)positio;
            fructus.terminalis  = passus.terminale;
            si (xar_numerus(frons) > ZEPHYRUM)
            {
                SilvaGSSNodus** ref =
                    (SilvaGSSNodus**)xar_obtinere(frons, ZEPHYRUM);

                si (ref != NIHIL && *ref != NIHIL)
                {
                    fructus.status = (*ref)->status;
                }
            }
            _statisticas_copiare(glr, &fructus);
            redde fructus;
        }

        si (xar_numerus(passus.frons_nova) > glr->frons_maxima)
        {
            glr->frons_maxima = xar_numerus(passus.frons_nova);
        }

        /* Limen frontis (Phase 7): fractura munda pro fluxu infesto -
         * gubernator segmentum in nodum ERROR vertit (totalitas) */
        si (   glr->limen_frontis > ZEPHYRUM
            && xar_numerus(passus.frons_nova) > glr->limen_frontis)
        {
            fructus.est_error        = VERUM;
            fructus.est_ultra_limen  = VERUM;
            fructus.positio          = (s32)positio;
            fructus.terminalis       = passus.terminale;
            _statisticas_copiare(glr, &fructus);
            redde fructus;
        }

        frons = passus.frons_nova;
        positio++;
    }
}
