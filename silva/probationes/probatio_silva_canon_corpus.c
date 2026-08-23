/* probatio_silva_canon_corpus - PORTA CORPORIS canonis (T7)
 *
 * Omnia priora canonem contra SE IPSUM probaverunt: canon.canon
 * canonem generatum iudicat, probatio numeros eius pinnat. Nihil
 * eorum dicit an canon codicem VERUM describat.
 *
 * Hic documenta quae silva revera gignit iudicantur:
 *
 *   plagula -> parsare -> <parsura> STML -> canon_iudicare
 *
 * DUO GRADUS, et secundus est qui rem probat:
 *   I.  corpus planum (probationes/fixa/roundtrip) - nulla macro,
 *       ergo involucrum originis (expansio/pasta/api/extentum)
 *       INTACTUM manet
 *   II. gradus latinizatus (lib (plagulae .c) cum clausuris) - macro ubique,
 *       ergo elementa illa PRIMUM hic exercentur. Ea manu scripta
 *       sunt (politia T5, non derivata), et haec porta est quae ea
 *       falsificat.
 *
 * PORTA APPARATUS communis est (apparatus.c): sine clausura latina.h
 * numquam videtur et numerus purus NIHIL mensurat.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_c89.h"
#include "silva_parsare.h"
#include "silva_arbor.h"
#include "apparatus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

nomen structura {
    i32 plagulae;
    i32 purae;             /* documenta sine ullo vitio */
    i32 vitia_summa;
    i32 latinizatae;
    i32 apparatus_fracti;
    i32 parsura_recusata;
    i32 scriptura_recusata;
    i32 lectio_recusata;

    CausaNumerata causae[CAUSAE_MAXIMAE];
              i32 numerus_causarum;

    /* INVOLUCRUM EXERCITUM: elementa MANU SCRIPTA (politia T5, non
     * derivata) quae corpus planum numquam tangit. Sine his numeris
     * 'CLIV/CLIV pura' significare posset 'ea numquam apparuerunt' -
     * porta quae rem quam custodire creditur NON videt. */
    i32 vidit_expansio;
    i32 vidit_pasta;
    i32 vidit_stringificatio;
    i32 vidit_extentum;
    i32 vidit_invocatio_vacua;
    i32 vidit_scissura;
    i32 vidit_conditionalis;
    i32 vidit_error;
} CanonCensus;

/* Quotiens litterae in chorda occurrant (non nullo terminata) */
interior i32
_quot_occurrit (
                 chorda c,
    constans character* litterae)
{
    i32 numerus;
    i32 i;
    i32 m;

    numerus = ZEPHYRUM;
    m = (i32)strlen(litterae);
    si (c.datum == NIHIL || m == ZEPHYRUM || c.mensura < m)
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i + m <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, litterae, (memoriae_index)m)
            == ZEPHYRUM)
        {
            numerus++;
        }
    }
    redde numerus;
}

interior vacuum
_involucrum_numerare (
    chorda doc,
    CanonCensus* census)
{
    census->vidit_expansio        += _quot_occurrit(doc, "<expansio");
    census->vidit_pasta           += _quot_occurrit(doc, "<pasta");
    census->vidit_stringificatio  += _quot_occurrit(doc,
                                         "<stringificatio");
    census->vidit_extentum        += _quot_occurrit(doc, "<extentum");
    census->vidit_invocatio_vacua += _quot_occurrit(doc,
                                         "<invocatio-vacua");
    census->vidit_scissura        += _quot_occurrit(doc, "<scissura");
    census->vidit_conditionalis   += _quot_occurrit(doc,
                                         "<conditionalis");
    census->vidit_error           += _quot_occurrit(doc, "<error");
}

/* Documentum <parsura> ex plagula gignere. Chorda vacua = fractura
 * (causa in censum notata). */
interior chorda
_documentum_gignere (
               Piscina* opus,
    constans character* via,
    constans character* radix,
     constans Clausura* clausura,
          CanonCensus* census)
{
                 chorda vacua;
                    i8* fons;
                    i32 mensura;
           SilvaParsura* origo;
    SilvaArborScriptura  scriptura;
                    s32  fons_latina;
                    i32  expansa;

    vacua.datum   = NIHIL;
    vacua.mensura = ZEPHYRUM;

    fons = apparatus_plagulam_legere(opus, via, &mensura);
    si (fons == NIHIL || mensura <= ZEPHYRUM)
    {
        redde vacua;
    }
    census->plagulae++;
    si (clausura != NIHIL && clausura->latina_inest)
    {
        census->latinizatae++;
    }

    origo = apparatus_parsare(opus, via, fons, mensura, radix,
                              clausura, VERUM, &fons_latina);
    si (origo == NIHIL || origo->commissio == NIHIL)
    {
        census->parsura_recusata++;
        apparatus_causam_notare(census->causae,
            &census->numerus_causarum, "parsura fracta");
        redde vacua;
    }

    /* PORTA APPARATUS ANTE rem quam custodit: sine clausura
     * documentum nascitur quod codicem latinizatum NON refert, et
     * iudicium purum de eo nihil probaret. */
    si (!apparatus_sanus(origo, clausura, fons_latina, &expansa))
    {
        census->apparatus_fracti++;
        apparatus_causam_notare(census->causae,
            &census->numerus_causarum,
            "clausura inclusionum non praebita");
        imprimere("    APPARATUS FRACTUS: %s\n", via);
        redde vacua;
    }

    scriptura = silva_arbor_scribere_parsuram(opus, origo,
        &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps, NIHIL);
    si (!scriptura.successus)
    {
        census->scriptura_recusata++;
        apparatus_causam_notare(census->causae,
            &census->numerus_causarum,
            scriptura.causa ? scriptura.causa : "scriptura fracta");
        redde vacua;
    }
    redde scriptura.textus;
}

/* Documentum contra canonem iudicare; vitia in censum.
 * Reddit numerum vitiorum, aut -1 in fractura (ergo SIGNATUM). */
interior s32
_documentum_iudicare (
               Piscina* opus,
                 Canon* canon,
                 chorda documentum,
    constans character* via,
    InternamentumChorda* intern,
           CanonCensus* census)
{
    StmlResultus res;
            Xar* vitia;
             s32 numerus;
             i32 i;

    res = stml_legere(documentum, opus, intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        census->lectio_recusata++;
        apparatus_causam_notare(census->causae,
            &census->numerus_causarum, "STML relegi non potuit");
        redde -I;
    }

    vitia = canon_iudicare(canon, res.elementum_radix, opus);
    si (vitia == NIHIL)
    {
        census->lectio_recusata++;
        apparatus_causam_notare(census->causae,
            &census->numerus_causarum, "iudicium NIHIL reddidit");
        redde -I;
    }

    numerus = (s32)xar_numerus(vitia);
    si (numerus == ZEPHYRUM)
    {
        census->purae++;
        redde ZEPHYRUM;
    }

    census->vitia_summa += (i32)numerus;
    /* CENSUS PER CAUSAM, non per summam: summa immota manere potest
     * dum causae subter omnino mutantur (spec par. 5) */
    per (i = ZEPHYRUM; i < (i32)numerus; i++)
    {
        CanonVitium* v = (CanonVitium*)xar_obtinere(vitia, i);

        si (v == NIHIL)
        {
            perge;
        }
        apparatus_causam_notare(census->causae,
            &census->numerus_causarum, canon_nuntius(v->genus));
        si (i < III)
        {
            imprimere("    VITIUM %s: <%.*s>", canon_nuntius(v->genus),
                (integer)(v->elementum ? v->elementum->mensura
                                       : (i32)ZEPHYRUM),
                v->elementum ? (character*)v->elementum->datum : "");
            si (v->detail)
            {
                imprimere(" %.*s", (integer)v->detail->mensura,
                    (character*)v->detail->datum);
            }
            imprimere("  [%s]\n", via);
        }
    }
    redde numerus;
}

interior vacuum
_censum_imprimere (
    constans character* titulus,
        CanonCensus* census)
{
    i32 i;

    imprimere("\n  --- %s ---\n", titulus);
    imprimere("  plagulae:           %d\n", (integer)census->plagulae);
    imprimere("  latinizatae:        %d\n", (integer)census->latinizatae);
    imprimere("  APPARATUS FRACTI:   %d\n",
        (integer)census->apparatus_fracti);
    imprimere("  parsura recusata:   %d\n",
        (integer)census->parsura_recusata);
    imprimere("  scriptura recusata: %d\n",
        (integer)census->scriptura_recusata);
    imprimere("  lectio recusata:    %d\n",
        (integer)census->lectio_recusata);
    imprimere("  PURAE:              %d / %d\n",
        (integer)census->purae, (integer)census->plagulae);
    imprimere("  vitia summa:        %d\n",
        (integer)census->vitia_summa);
    per (i = ZEPHYRUM; i < census->numerus_causarum; i++)
    {
        imprimere("    %5d x %s\n", (integer)census->causae[i].numerus,
            census->causae[i].causa);
    }
    imprimere("  involucrum exercitum: expansio %d, pasta %d,"
        " stringif %d, extentum %d\n",
        (integer)census->vidit_expansio, (integer)census->vidit_pasta,
        (integer)census->vidit_stringificatio,
        (integer)census->vidit_extentum);
    imprimere("                        inv-vacua %d, scissura %d,"
        " conditionalis %d, error %d\n",
        (integer)census->vidit_invocatio_vacua,
        (integer)census->vidit_scissura,
        (integer)census->vidit_conditionalis,
        (integer)census->vidit_error);
}

s32
principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     constans character* radix;
              character  via_canonis[VIA_MAXIMA];
              character  via_corporis[VIA_MAXIMA];
              character  via_plagulae[VIA_MAXIMA];
              character  via_relativa[VIA_MAXIMA];
                  Canon* canon;
                  chorda fons_canonis;
                  chorda causa;
                    DIR* corpus;
          structura dirent* introitus;
             CanonCensus  census_planus;
             CanonCensus  census_latinus;

    piscina = piscina_generare_dynamicum("canon_corpus",
                                         (memoriae_index)256 * M * M);
    si (piscina == NIHIL)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL) radix = "..";

    /* ============================================================
     * CANONEM ONERARE
     * ============================================================ */
    sprintf(via_canonis, "%s/silva/c89.canon", radix);
    {
        i32 m;
        i8* datum = apparatus_plagulam_legere(piscina, via_canonis, &m);

        CREDO_NON_NIHIL (datum);
        si (datum == NIHIL)
        {
            imprimere("  (canon abest: ./silva/generare.sh)\n");
            credo_imprimere_compendium();
            redde I;
        }
        fons_canonis.datum   = datum;
        fons_canonis.mensura = m;
    }
    causa.datum   = NIHIL;
    causa.mensura = ZEPHYRUM;
    canon = canon_legere(fons_canonis, piscina, intern, &causa);
    CREDO_NON_NIHIL (canon);
    si (canon == NIHIL)
    {
        si (causa.datum)
        {
            imprimere("  CAUSA: %.*s\n", (integer)causa.mensura,
                (character*)causa.datum);
        }
        credo_imprimere_compendium();
        redde I;
    }

    /* ============================================================
     * FALSIFICATIO PRIMUM (spec par. 5, proprietas I)
     *
     * Cursus viridis ex porta INFALSIFICATA nihil mensurat. Ergo
     * ante omnia: documentum VALIDUM corrumpitur (nodus in locum
     * illicitum motus) et vitium POSTULATUR.
     *
     * Casus electus est ille qui consilium decrevit:
     * 'declarator-abstractus' in definitio-functionis/declarator
     * sedere NON potest, quamquam species utriusque NODUS est.
     * ============================================================ */
    {
        Piscina* opus;
        chorda   doc;
        s32      vitia_sana;
        s32      vitia_corrupta;
        character* copia;
        character* sedes;

        imprimere("\n--- FALSIFICATIO: porta mordere DEBET ---\n");
        opus = piscina_generare_dynamicum("falsificatio",
                                          (memoriae_index)64 * M * M);
        sprintf(via_plagulae,
                "%s/probationes/fixa/roundtrip/two_structs.c", radix);
        memset(&census_planus, 0, magnitudo(CanonCensus));
        doc = _documentum_gignere(opus, via_plagulae, radix, NIHIL,
                                  &census_planus);
        CREDO_VERUM (doc.mensura > ZEPHYRUM);

        si (doc.mensura > ZEPHYRUM)
        {
            vitia_sana = _documentum_iudicare(opus, canon, doc,
                "sanum", intern, &census_planus);
            imprimere("  documentum sanum:     vitia %d\n",
                (integer)vitia_sana);
            CREDO_AEQUALIS_I32 ((i32)vitia_sana, ZEPHYRUM);

            /* CORRUPTIO: nomen loci in aliud generis ALIENI mutatur */
            copia = (character*)piscina_allocare(opus,
                (memoriae_index)doc.mensura + I);
            memcpy(copia, doc.datum, (memoriae_index)doc.mensura);
            copia[doc.mensura] = '\0';
            sedes = strstr(copia, "<declaratores>");
            CREDO_NON_NIHIL (sedes);
            si (sedes != NIHIL)
            {
                chorda corrupta;

                /* 'declaratores' -> 'declaratorel' : elementum quod
                 * canon omnino non novit */
                sedes[12] = 'e';
                sedes[13] = 'l';
                corrupta.datum   = (i8*)copia;
                corrupta.mensura = doc.mensura;
                vitia_corrupta = _documentum_iudicare(opus, canon,
                    corrupta, "corruptum", intern, &census_planus);
                imprimere("  documentum corruptum: vitia %d\n",
                    (integer)vitia_corrupta);
                CREDO_VERUM (vitia_corrupta > ZEPHYRUM);
            }
        }
        piscina_destruere(opus);
    }

    /* ============================================================
     * GRADUS I - corpus planum (nulla macro)
     * ============================================================ */
    memset(&census_planus, 0, magnitudo(CanonCensus));
    sprintf(via_corporis, "%s/probationes/fixa/roundtrip", radix);
    imprimere("\n--- GRADUS PLANUS: %s ---\n", via_corporis);

    corpus = opendir(via_corporis);
    CREDO_NON_NIHIL (corpus);
    si (corpus != NIHIL)
    {
        dum ((introitus = readdir(corpus)) != NIHIL)
        {
            Piscina* opus;
            chorda   doc;

            si (!apparatus_est_c(introitus->d_name))
            {
                perge;
            }
            sprintf(via_plagulae, "%s/%s", via_corporis,
                    introitus->d_name);
            opus = piscina_generare_dynamicum("gradus_planus",
                                              (memoriae_index)64 * M * M);
            si (opus == NIHIL) perge;
            doc = _documentum_gignere(opus, via_plagulae, radix, NIHIL,
                                      &census_planus);
            si (doc.mensura > ZEPHYRUM)
            {
                _involucrum_numerare(doc, &census_planus);
                _documentum_iudicare(opus, canon, doc,
                    introitus->d_name, intern, &census_planus);
            }
            piscina_destruere(opus);
        }
        closedir(corpus);
    }
    _censum_imprimere("GRADUS PLANUS", &census_planus);

    /* ============================================================
     * GRADUS II - latinizatus (lib (plagulae .c) cum clausuris)
     * ============================================================ */
    memset(&census_latinus, 0, magnitudo(CanonCensus));
    sprintf(via_corporis, "%s/lib", radix);
    imprimere("\n--- GRADUS LATINIZATUS: %s ---\n", via_corporis);

    corpus = opendir(via_corporis);
    CREDO_NON_NIHIL (corpus);
    si (corpus != NIHIL)
    {
        dum ((introitus = readdir(corpus)) != NIHIL)
        {
            Piscina* opus;
            chorda   doc;
            Clausura clausura;

            si (!apparatus_est_c(introitus->d_name))
            {
                perge;
            }
            sprintf(via_relativa, "lib/%s", introitus->d_name);
            sprintf(via_plagulae, "%s/%s", radix, via_relativa);
            apparatus_clausuram_petere(radix, via_relativa, &clausura);

            opus = piscina_generare_dynamicum("gradus_latinus",
                                              (memoriae_index)256 * M * M);
            si (opus == NIHIL) perge;
            doc = _documentum_gignere(opus, via_plagulae, radix,
                                      &clausura, &census_latinus);
            si (doc.mensura > ZEPHYRUM)
            {
                _involucrum_numerare(doc, &census_latinus);
                _documentum_iudicare(opus, canon, doc,
                    introitus->d_name, intern, &census_latinus);
            }
            piscina_destruere(opus);
        }
        closedir(corpus);
    }
    _censum_imprimere("GRADUS LATINIZATUS", &census_latinus);

    /* ============================================================
     * NUMERI FIXI
     * ============================================================ */
    CREDO_AEQUALIS_I32 (census_planus.plagulae, (i32)73);
    CREDO_AEQUALIS_I32 (census_planus.apparatus_fracti, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_planus.parsura_recusata, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_planus.scriptura_recusata, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_planus.lectio_recusata, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_planus.vitia_summa, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_planus.purae, (i32)73);

    CREDO_AEQUALIS_I32 (census_latinus.plagulae, (i32)154);
    CREDO_AEQUALIS_I32 (census_latinus.latinizatae, (i32)154);
    CREDO_AEQUALIS_I32 (census_latinus.apparatus_fracti, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinus.parsura_recusata, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinus.scriptura_recusata, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinus.lectio_recusata, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinus.vitia_summa, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinus.purae, (i32)154);

    /* ============================================================
     * QUID PORTA REVERA VIDERIT
     *
     * 'CLIV/CLIV pura' significare posset 'elementa illa numquam
     * apparuerunt'. Ergo praesentia ASSERITUR, non speratur: haec
     * sunt elementa MANU SCRIPTA (politia T5) quae sola corpus
     * latinizatum exercet.
     *
     * NUMERI non pinnantur (lib/ crescit; pinna talis de re innoxia
     * clamaret) - sola PRAESENTIA, quae proprietas est quam porta
     * postulat.
     * ============================================================ */
    CREDO_VERUM (census_latinus.vidit_expansio       > ZEPHYRUM);
    CREDO_VERUM (census_latinus.vidit_pasta          > ZEPHYRUM);
    CREDO_VERUM (census_latinus.vidit_stringificatio > ZEPHYRUM);
    CREDO_VERUM (census_latinus.vidit_extentum       > ZEPHYRUM);
    CREDO_VERUM (census_latinus.vidit_conditionalis  > ZEPHYRUM);
    /* ERROR gradu PLANO solo apparet (XLI), non latinizato - ergo
     * politia 'error in radice sola' ibi exercetur */
    CREDO_VERUM (census_planus.vidit_error           > ZEPHYRUM);

    /* ============================================================
     * LACUNA NOMINATA, non tacita (spec par. 5: nulla tecta tacita)
     *
     * Haec duo elementa involucri manu scripti a NEUTRO gradu
     * tanguntur, ergo hac porta NON probantur:
     *
     *   invocatio-vacua : macro quae ZERO lexemata parit
     *   scissura        : lamina '\'+nova-linea INTRA lexema
     *
     * Ea ZERO esse ASSERITUR ut lacuna VISIBILIS maneat. Si quis
     * fixum tale addat, haec probatio franguntur - quod est
     * annuntiatio recta, non molestia.
     *
     * SEDES: fixa propria in probationes/fixa/ postulantur; corpus
     * verum ea non fert.
     * ============================================================ */
    CREDO_AEQUALIS_I32 (census_latinus.vidit_invocatio_vacua, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinus.vidit_scissura,        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_planus.vidit_invocatio_vacua,  ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_planus.vidit_scissura,         ZEPHYRUM);

    credo_imprimere_compendium();
    si (credo_omnia_praeterierunt())
    {
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }
    piscina_destruere(piscina);
    redde I;
}
