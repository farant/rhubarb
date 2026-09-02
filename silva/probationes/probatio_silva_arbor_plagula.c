/* probatio_silva_arbor_plagula.c - PORTA M2 arboris
 *
 * Circuitus PLAGULAE INTEGRAE super corpus commune
 * (probationes/fixa/roundtrip - quaeque plagula cicatrix defectus
 * parsatoris veri):
 *
 *   parsare -> scribere <parsura> -> legere -> silva_scribere_fontem
 *   -> octetos contra PLAGULAM IN DISCO conferre
 *
 * CUR HOC FORTIUS EST QUAM PORTA M1: valor expectatus EXTRA silvam
 * iacet. Porta M1 exitum silvae contra exitum silvae confert, ergo
 * vitium SYMMETRICUM inter scriptorem et lectorem in ea latere
 * potest. Hic valor expectatus PLAGULA IPSA est - nihil quod silva
 * scripsit. Hoc est oraculum VISIONIS ipsum ('code->STML->load->
 * emit == code'), dimidium dormiens oraculi fidelitatis quod hoc
 * parcum nominat.
 *
 * COMPARATOR = DIAGNOSIS, non verdictum secundum. Cum octeti
 * divergunt, silva_arbor_parsurae_aequales campum nominat. In M1
 * ter divergentiam divinavi et ter erravi; campum imprimere rem
 * uno cursu solvit, quotiens.
 *
 * Radix repositorii per RHUBARB_RADIX (compile_probationes.sh eam
 * praebet); defaltum ".." pro cursu manuali ex silva/.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_c89.h"
#include "silva_parsare.h"
#include "silva_expandere.h"
#include "silva_scribere.h"
#include "silva_c89_oraculum.h"
#include "silva_arbor.h"
#include "apparatus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

nomen structura {
    i32 plagulae;
    i32 octetim_exactae;
    i32 scriptura_recusata;
    i32 lectio_recusata;
    i32 emissio_recusata;
    i32 octeti_divergentes;
    i32 comparator_tacuit;   /* octeti divergunt, arbor 'aequalis' */

    /* ORACULUM SEPARANS: emissio directa (silva sola, sine STML) */
    i32 directa_exacta;

    /* PORTA APPARATUS (spec 6.5) */
    i32 latinizatae;         /* plagulae quarum clausura latina.h fert */
    i32 apparatus_fracti;    /* latinizatae, sed ZERO lexemata expansa */
    i32 clausurae_truncatae;
    i32 lexemata_expansa;    /* summa - mensura pervasionis macro */

    /* PORTA COMPRESSIONIS: scriptor mensuram suam fert - familia
     * templorum mortua exitum VALIDUM incompressum relinquit
     * (portae octetorum virides manent, sola magnitudo crescit),
     * ergo PRAESENTIA aggregata asseritur. */
    SilvaArborCensusCompressionis compressio;

    CausaNumerata recusationes[CAUSAE_MAXIMAE];
              i32 numerus_recusationum;
    CausaNumerata divergentiae[CAUSAE_MAXIMAE];
              i32 numerus_divergentiarum;
} PlagulaCensus;

/* Circuitus unus. clausura == NIHIL = corpus planum (gradus T5).
 * clausura != NIHIL = gradus latinizatus (T7).
 *
 * 'praebere' est SUTURA CALIBRATIONIS, consulto permanens: FALSUM
 * clausuram silvae NEGAT dum scientiam nostram de ea servat, ergo
 * porta apparatus incendere DEBET. Polaritas tuta est - positio
 * falsa portam CLAMARE facit, non tacere. */
interior vacuum
_plagulam_probare (
    constans character* via,
    constans character* radix,
     constans Clausura* clausura,
                   b32  praebere,
         PlagulaCensus* census)
{
                  Piscina* opus;
                       i8* fons;
                      i32  mensura;
             SilvaParsura* origo;
             SilvaParsura* lecta;
      SilvaArborScriptura  scriptura;
         SilvaArborVitium  vitium;
           SilvaScriptura  emissio;
    SilvaArborDifferentia  differentia;
                      s32  fons_latina;

    fons_latina = -I;
    opus = piscina_generare_dynamicum("porta_plagulae", 67108864);
    si (opus == NIHIL)
    {
        redde;
    }
    fons = apparatus_plagulam_legere(opus, via, &mensura);
    si (fons == NIHIL || mensura <= ZEPHYRUM)
    {
        piscina_destruere(opus);
        redde;
    }
    census->plagulae++;
    si (clausura != NIHIL && clausura->truncata)
    {
        census->clausurae_truncatae++;
    }
    si (clausura != NIHIL && clausura->latina_inest)
    {
        census->latinizatae++;
    }

    origo = apparatus_parsare(opus, via, fons, mensura, radix,
                              clausura, praebere, &fons_latina);
    si (origo == NIHIL || origo->commissio == NIHIL)
    {
        apparatus_causam_notare(census->recusationes,
            &census->numerus_recusationum, "parsura fracta");
        census->scriptura_recusata++;
        piscina_destruere(opus);
        redde;
    }

    /* PORTA APPARATUS (spec 6.5) - ANTE rem quam custodit.
     * Impletio in apparatus.c, COMMUNIS cum iudicio canonis: porta
     * quae bis incendit exemplar secundum habere non debet. */
    {
        i32 expansa;

        si (!apparatus_sanus(origo, clausura, fons_latina, &expansa))
        {
            census->apparatus_fracti++;
            apparatus_causam_notare(census->recusationes,
                &census->numerus_recusationum,
                "clausura inclusionum non praebita");
            imprimere("    APPARATUS FRACTUS: %s (latina.h in "
                "clausura, ZERO lexemata expansa)\n", via);
            piscina_destruere(opus);
            redde;
        }
        census->lexemata_expansa += expansa;
    }


    /* ==========================================================
     * ORACULUM SEPARANS: emissio DIRECTA ex parsura originali,
     * sine circuitu STML.
     *
     * Respondet quaestioni quae aliter coniectura maneret: cum
     * plagula circuitum non superat, utrum SILVA ipsa eam emittere
     * nequit, an PROIECTIO NOSTRA aliquid amittit? Emissio directa
     * silvam solam probat (parsare -> scribere_fontem); si ea
     * exacta est dum circuitus STML divergit, vitium NOSTRUM est.
     * Sine hoc numero utrumque stratum uno numero involvitur.
     * ========================================================== */

    {
        SilvaScriptura directa;

        directa = silva_scribere_fontem(opus, origo,
            &SILVA_C89_REGISTRUM, origo->fons_princeps);
        si (   directa.successus
            && directa.textus.mensura == mensura
            && memcmp(directa.textus.datum, fons,
                   (memoriae_index)mensura) == ZEPHYRUM)
        {
            census->directa_exacta++;
        }
        alioquin si (clausura != NIHIL)
        {
            /* VITIUM SILVAE IPSIUS, non proiectionis nostrae -
             * nominandum, ne in numero circuitus lateat. */
            imprimere("    DIRECTA RECUSATA (vitium SILVAE): %s"
                " [successus=%d mensura=%d/%d]\n", via,
                (integer)directa.successus,
                (integer)directa.textus.mensura, (integer)mensura);
        }
    }

    scriptura = silva_arbor_scribere_parsuram(opus, origo,
        &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps, NIHIL);
    si (!scriptura.successus)
    {
        apparatus_causam_notare(census->recusationes,
            &census->numerus_recusationum, scriptura.causa);
        census->scriptura_recusata++;
        piscina_destruere(opus);
        redde;
    }
    census->compressio.spatia_vocationes +=
        scriptura.census.spatia_vocationes;
    census->compressio.folia_formae +=
        scriptura.census.folia_formae;
    census->compressio.folia_vocationes +=
        scriptura.census.folia_vocationes;
    census->compressio.parametra_visa +=
        scriptura.census.parametra_visa;
    census->compressio.parametra_compressa +=
        scriptura.census.parametra_compressa;

    {
        /* Documentum plagulae NOMINATAE effundere, EXITU QUOLIBET -
         * inspectio, non diagnosis. ARBOR_DEFIGERE_NOMEN=<pars viae>
         * cum ARBOR_DEFIGERE=<via exitus>. */
        constans character* petitum = getenv("ARBOR_DEFIGERE_NOMEN");

        si (   petitum                  != NIHIL
            && getenv("ARBOR_DEFIGERE") != NIHIL
            && strstr(via, petitum)     != NIHIL)
        {
            FILE* ef = fopen(getenv("ARBOR_DEFIGERE"), "wb");

            si (ef != NIHIL)
            {
                fwrite(scriptura.textus.datum, I,
                    (memoriae_index)scriptura.textus.mensura, ef);
                fclose(ef);
            }
        }
    }

    lecta = silva_arbor_legere_parsuram(opus, NIHIL, scriptura.textus,
        &SILVA_C89_REGISTRUM, "c89", &vitium);
    si (lecta == NIHIL)
    {
        apparatus_causam_notare(census->recusationes,
            &census->numerus_recusationum, vitium.causa);
        census->lectio_recusata++;
        si (   getenv("ARBOR_DEFIGERE") != NIHIL
            && (   getenv("ARBOR_DEFIGERE_NOMEN") == NIHIL
                || strstr(via, getenv("ARBOR_DEFIGERE_NOMEN"))
                       != NIHIL))
        {
            FILE* effusio;

            imprimere("    LECTIO RECUSATA: %s :: %s (linea STML %d)\n",
                via, vitium.causa ? vitium.causa : "?",
                (integer)vitium.linea);
            effusio = fopen(getenv("ARBOR_DEFIGERE"), "wb");
            si (effusio != NIHIL)
            {
                fwrite(scriptura.textus.datum, I,
                    (memoriae_index)scriptura.textus.mensura, effusio);
                fclose(effusio);
            }
        }
        piscina_destruere(opus);
        redde;
    }

    emissio = silva_scribere_fontem(opus, lecta,
        &SILVA_C89_REGISTRUM, lecta->fons_princeps);
    si (!emissio.successus)
    {
        apparatus_causam_notare(census->recusationes,
            &census->numerus_recusationum, emissio.causa);
        census->emissio_recusata++;
        piscina_destruere(opus);
        redde;
    }

    si (   emissio.textus.mensura == mensura
        && memcmp(emissio.textus.datum, fons,
               (memoriae_index)mensura) == ZEPHYRUM)
    {
        census->octetim_exactae++;
        piscina_destruere(opus);
        redde;
    }

    /* DIVERGENTIA: comparator campum nominat */
    census->octeti_divergentes++;
    {
        /* OCTETUM PRIMUM DIVERGENS. Comparator caecus esse potest
         * (campos quos NON confert - e.g. extenta); octeti numquam
         * mentiuntur. Diagnosis ULTIMA, ergo semper praesto. */
        i32 i;
        i32 minor;

        minor = (emissio.textus.mensura < mensura)
              ? emissio.textus.mensura : mensura;
        per (i = ZEPHYRUM; i < minor; i++)
        {
            si (emissio.textus.datum[i] != fons[i])
            {
                frange;
            }
        }
        imprimere("      extenta: A=%d B=%d | directivae: A=%d B=%d\n",
            (integer)((origo->expansio && origo->expansio->extenta)
                ? xar_numerus(origo->expansio->extenta) : ZEPHYRUM),
            (integer)((lecta->expansio && lecta->expansio->extenta)
                ? xar_numerus(lecta->expansio->extenta) : ZEPHYRUM),
            (integer)(origo->directivae
                ? xar_numerus(origo->directivae) : ZEPHYRUM),
            (integer)(lecta->directivae
                ? xar_numerus(lecta->directivae) : ZEPHYRUM));
        imprimere("    OCTETUS I DIVERGENS %s: %d (mensurae A=%d B=%d)\n",
            via, (integer)i, (integer)mensura,
            (integer)emissio.textus.mensura);
        imprimere("      A [%.*s]\n", (integer)((minor - i > 48)
            ? 48 : minor - i), (constans character*)(fons + i));
        imprimere("      B [%.*s]\n", (integer)((minor - i > 48)
            ? 48 : minor - i),
            (constans character*)(emissio.textus.datum + i));
    }
    {
        /* Documentum plagulae NOMINATAE effundere (diagnosis):
         * ARBOR_DEFIGERE_NOMEN=tempus.c ARBOR_DEFIGERE=/via/ad/exitum */
        constans character* nomen_petitum;

        nomen_petitum = getenv("ARBOR_DEFIGERE_NOMEN");
        si (   nomen_petitum              != NIHIL
            && getenv("ARBOR_DEFIGERE")   != NIHIL
            && strstr(via, nomen_petitum) != NIHIL)
        {
            FILE* effusio = fopen(getenv("ARBOR_DEFIGERE"), "wb");

            si (effusio != NIHIL)
            {
                fwrite(scriptura.textus.datum, I,
                    (memoriae_index)scriptura.textus.mensura, effusio);
                fclose(effusio);
                imprimere("    (documentum effusum: %s)\n", via);
            }
        }
    }
    si (silva_arbor_parsurae_aequales(origo, lecta,
            SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia))
    {
        /* Octeti divergunt sed arbor 'aequalis' - comparator
         * CAECUS est ad hunc defectum. Numerandum: est mensura
         * quantum diagnosis nostra valeat. */
        census->comparator_tacuit++;
        apparatus_causam_notare(census->divergentiae,
            &census->numerus_divergentiarum,
            "(comparator tacuit)");
    }
    alioquin
    {
        apparatus_causam_notare(census->divergentiae,
            &census->numerus_divergentiarum, differentia.campus);
        imprimere("      directivae: A=%d B=%d | lexemata: A=%d B=%d\n",
            (integer)(origo->directivae
                ? xar_numerus(origo->directivae) : ZEPHYRUM),
            (integer)(lecta->directivae
                ? xar_numerus(lecta->directivae) : ZEPHYRUM),
            (integer)(origo->lexemata
                ? xar_numerus(origo->lexemata) : ZEPHYRUM),
            (integer)(lecta->lexemata
                ? xar_numerus(lecta->lexemata) : ZEPHYRUM));
        imprimere("    %s: %s (via %s, index %d)\n", via,
            differentia.campus ? differentia.campus : "?",
            differentia.via, (integer)differentia.index);
        si (   differentia.lexema_a != NIHIL
            && differentia.lexema_b != NIHIL)
        {
            imprimere("      A b=%d l=%d c=%d f=%d o=%d | "
                "B b=%d l=%d c=%d f=%d o=%d | princeps=%d\n",
                (integer)differentia.lexema_a->byte_offset,
                (integer)differentia.lexema_a->linea,
                (integer)differentia.lexema_a->columna,
                (integer)differentia.lexema_a->fons_index,
                (integer)differentia.lexema_a->origo.genus,
                (integer)differentia.lexema_b->byte_offset,
                (integer)differentia.lexema_b->linea,
                (integer)differentia.lexema_b->columna,
                (integer)differentia.lexema_b->fons_index,
                (integer)differentia.lexema_b->origo.genus,
                (integer)lecta->fons_princeps);
            imprimere("      valor A [%.*s]\n",
                (integer)differentia.lexema_a->valor.mensura,
                (constans character*)
                    differentia.lexema_a->valor.datum);
        }
    }
    piscina_destruere(opus);
}

interior vacuum
_causas_imprimere (
    constans character* titulus,
         CausaNumerata* series,
                   i32  numerus)
{
    i32 i;

    si (numerus == ZEPHYRUM)
    {
        redde;
    }
    imprimere("  %s:\n", titulus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        imprimere("    %4d  %s\n", (integer)series[i].numerus,
            series[i].causa);
    }
}

s32
principale (vacuum)
{
                Piscina* piscina;
          PlagulaCensus  census;
          PlagulaCensus  census_latinus;
     constans character* radix;
              character  via_corporis[512];
              character  via_plagulae[1024];
              character  via_relativa[1024];
                    DIR* corpus;
         structura dirent* introitus;
                Clausura  clausura;
                     b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_plagulae",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    memset(&census, 0, magnitudo(PlagulaCensus));

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = "..";
    }
    sprintf(via_corporis, "%s/probationes/fixa/roundtrip", radix);

    imprimere("\n--- PORTA M2: circuitus plagulae super corpus ---\n");
    imprimere("  corpus: %s\n", via_corporis);

    corpus = opendir(via_corporis);
    si (corpus == NIHIL)
    {
        imprimere("FRACTA: corpus non apertum: %s\n", via_corporis);
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }
    dum ((introitus = readdir(corpus)) != NIHIL)
    {
        si (!apparatus_est_c_vel_h(introitus->d_name))
        {
            perge;
        }
        sprintf(via_plagulae, "%s/%s", via_corporis,
            introitus->d_name);
        _plagulam_probare(via_plagulae, radix, NIHIL, FALSUM,
            &census);
    }
    closedir(corpus);

    imprimere("  plagulae:            %d\n", (integer)census.plagulae);
    imprimere("  DIRECTA exacta:      %d / %d  (silva sola)\n",
        (integer)census.directa_exacta, (integer)census.plagulae);
    imprimere("  OCTETIM EXACTAE:     %d / %d  (per STML)\n",
        (integer)census.octetim_exactae, (integer)census.plagulae);
    imprimere("  scriptura recusata:  %d\n",
        (integer)census.scriptura_recusata);
    imprimere("  lectio recusata:     %d\n",
        (integer)census.lectio_recusata);
    imprimere("  emissio recusata:    %d\n",
        (integer)census.emissio_recusata);
    imprimere("  octeti divergentes:  %d\n",
        (integer)census.octeti_divergentes);
    imprimere("  comparator tacuit:   %d\n",
        (integer)census.comparator_tacuit);

    /* RECENSIO REPRAESENTATIONALIS - fructus quem haec phasis debet
     * (visio §2.1). Quod circuitum non supervivit NOMINATIM
     * numeratur, non tacite omittitur. */
    _causas_imprimere("RECUSATIONES", census.recusationes,
        census.numerus_recusationum);
    _causas_imprimere("DIVERGENTIAE", census.divergentiae,
        census.numerus_divergentiarum);


    /* ==============================================================
     * GRADUS LATINIZATUS (T7) - corpus quod M2 vere petit
     *
     * Corpus supra C PLANUM est: nullum latina.h, ergo nullum
     * lexema expansum. Numerus eius probat formam circuire in C
     * quod codici NOSTRO non similis est. Hic plagulis lib
     * occurrit, ubi 'si', 'per', 'redde', 'NIHIL' OMNIA
     * expansiones macro sunt - ergo lex ancorae T6b (catena
     * originis ad invocationem strati 0 sequenda) non casus
     * marginalis est sed SEMITA CALIDA.
     * ============================================================== */

    memset(&census_latinus, 0, magnitudo(PlagulaCensus));
    sprintf(via_corporis, "%s/lib", radix);

    imprimere("\n--- GRADUS LATINIZATUS: lib/*.c cum clausuris ---\n");
    imprimere("  corpus: %s\n", via_corporis);

    corpus = opendir(via_corporis);
    si (corpus == NIHIL)
    {
        imprimere("FRACTA: corpus latinum non apertum: %s\n",
            via_corporis);
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }
    dum ((introitus = readdir(corpus)) != NIHIL)
    {
        si (!apparatus_est_c(introitus->d_name))
        {
            perge;
        }
        sprintf(via_relativa, "lib/%s", introitus->d_name);
        sprintf(via_plagulae, "%s/%s", radix, via_relativa);
        apparatus_clausuram_petere(radix, via_relativa, &clausura);
        _plagulam_probare(via_plagulae, radix, &clausura, VERUM,
            &census_latinus);
    }
    closedir(corpus);

    imprimere("  plagulae:            %d\n",
        (integer)census_latinus.plagulae);
    imprimere("  latinizatae:         %d\n",
        (integer)census_latinus.latinizatae);
    imprimere("  lexemata expansa:    %d\n",
        (integer)census_latinus.lexemata_expansa);
    imprimere("  APPARATUS FRACTI:    %d\n",
        (integer)census_latinus.apparatus_fracti);
    imprimere("  clausurae truncatae: %d\n",
        (integer)census_latinus.clausurae_truncatae);
    imprimere("  DIRECTA exacta:      %d / %d  (silva sola)\n",
        (integer)census_latinus.directa_exacta,
        (integer)census_latinus.plagulae);
    imprimere("  OCTETIM EXACTAE:     %d / %d  (per STML)\n",
        (integer)census_latinus.octetim_exactae,
        (integer)census_latinus.plagulae);
    imprimere("  scriptura recusata:  %d\n",
        (integer)census_latinus.scriptura_recusata);
    imprimere("  lectio recusata:     %d\n",
        (integer)census_latinus.lectio_recusata);
    imprimere("  emissio recusata:    %d\n",
        (integer)census_latinus.emissio_recusata);
    imprimere("  octeti divergentes:  %d\n",
        (integer)census_latinus.octeti_divergentes);
    imprimere("  comparator tacuit:   %d\n",
        (integer)census_latinus.comparator_tacuit);
    _causas_imprimere("RECUSATIONES", census_latinus.recusationes,
        census_latinus.numerus_recusationum);
    _causas_imprimere("DIVERGENTIAE", census_latinus.divergentiae,
        census_latinus.numerus_divergentiarum);


    /* ==========================================================
     * PORTA - numeri PINNATI, non '>= aliquid'
     *
     * Pinna laxa ('plures quam N transeunt') regressum tacere
     * sineret. Numeri exacti transitum ANNUNTIARI cogunt.
     * ========================================================== */

    /* Corpus vere lectum - aliter porta tota vacua esset */
    CREDO_AEQUALIS_I32 (census.plagulae, 78);

    /* T6: LIMES EXPANSIONIS SUBLATUS - recusationes V -> 0.
     * Origo nestata lexemata non-FONS fert, ergo scriptor nihil
     * amplius recusat. */
    CREDO_AEQUALIS_I32 (census.scriptura_recusata, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census.numerus_recusationum, ZEPHYRUM);

    /* LXXVIII ex LXXVIII - COPERTURA PLENA. Oraculum VISIONIS
     * ('code->STML->load->emit == code') super corpus totum viret:
     * nulla recusatio, nulla divergentia, comparator nusquam
     * caecus.
     *
     * Ultimum vitium: ancora ex lexemate EXPANSO sumpta sedem
     * DEF-SITE dabat (plagulae ALTERIUS), non sedem invocationis
     * ubi octeti vere stant. Sanatio: _parsura_lexema_emissionis
     * catenam originis ad radicem strati 0 sequitur. */
    CREDO_AEQUALIS_I32 (census.octetim_exactae, 78);
    CREDO_AEQUALIS_I32 (census.octeti_divergentes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census.numerus_divergentiarum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census.lectio_recusata, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census.emissio_recusata, ZEPHYRUM);

    /* COMPARATOR CAECUS NUSQUAM: si octeti divergerent dum
     * comparator 'aequales' diceret, diagnosis nostra ibi nihil
     * valeret. Zero est mensura, non praesumptio. */
    CREDO_AEQUALIS_I32 (census.comparator_tacuit, ZEPHYRUM);


    /* ==========================================================
     * PORTA GRADUS LATINIZATI (T7) - numeri PINNATI
     *
     * Hic numerus est quem M2 vere petit: codex NOSTER, ubi
     * 'si'/'per'/'redde'/'NIHIL' omnia expansiones macro sunt.
     * ========================================================== */

    CREDO_AEQUALIS_I32 (census_latinus.plagulae, 156);

    /* Clausura vere praebita. Si porta apparatus incendit, numerus
     * circuitus NIHIL mensurat - vide _lexemata_ex_fonte_numerare:
     * numerare expansiones OMNES LIX plagulas falso transire
     * sinebat dum latina.h numquam aperiretur. */
    CREDO_AEQUALIS_I32 (census_latinus.latinizatae, 156);
    CREDO_AEQUALIS_I32 (census_latinus.apparatus_fracti, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinus.clausurae_truncatae, ZEPHYRUM);

    /* ORACULUM SEPARANS: emissio DIRECTA (silva sola, sine STML).
     * CLIV = PLENUM. Fuit CLIII dum arbor2_glr_tabula.c (tabula GLR
     * generata, MDLI KB) emissionem recusaret; causa NON erat
     * memoria ut coniectatum est, sed deferral PASTAE - tabula
     * generata '##' plena est. Campo 'invocatio' armae PASTAE
     * addito, silva plagulas OMNES emittit.
     * Si HIC numerus cadit, vitium silvae est; si OCTETIM EXACTAE
     * solae cadunt, nostrum. */
    CREDO_AEQUALIS_I32 (census_latinus.directa_exacta, 156);

    /* PROIECTIO NIHIL AMITTIT: circuitus STML numerum silvae
     * AEQUAT - CLIV/CLIV utrobique. Quidquid silva emittere potest,
     * per STML circuit, ET NIHIL RECUSATUR. */
    CREDO_AEQUALIS_I32 (census_latinus.octetim_exactae, 156);
    CREDO_AEQUALIS_I32 (census_latinus.octeti_divergentes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinus.scriptura_recusata, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinus.emissio_recusata, ZEPHYRUM);

    /* NULLA recusatio. arbor2_glr_tabula.c (documentum MDCCCXXXVI
     * milia linearum) ultima cecidit: ordinatio supremorum offset
     * LEXEMATIS utebatur dum ancora triviis INCLUSIS scriberetur,
     * ergo directiva commento magno praeeunte post nodum ordinabatur
     * quem praecedere debebat - unde transclusio ad fragmentum in
     * arbore definitum ante definitionem legebatur. */
    CREDO_AEQUALIS_I32 (census_latinus.lectio_recusata, ZEPHYRUM);

    /* Comparator nusquam caecus */
    CREDO_AEQUALIS_I32 (census_latinus.comparator_tacuit, ZEPHYRUM);


    /* ==========================================================
     * PORTA COMPRESSIONIS - PRAESENTIA, non numeri pincti
     *
     * Polaritas consulto DIVERSA a portis circuitus supra: illae
     * teguntur corpus FIXUM (numeri exacti transitum annuntiant),
     * haec mores scriptoris super contentum MUTABILE metitur -
     * pinna quaeque editione lib/ putresceret. Modus fracturae =
     * collapsus ad zephyrum (familia templorum mortua, portae
     * octetorum virides, sola magnitudo crescens), quem praesentia
     * aggregata capit. Folia gradu PLANO nulla (sine latina.h
     * nullum lexema expansum) - praesentia eorum gradu latinizato
     * solo asseritur.
     * ========================================================== */

    imprimere("\n  compressio (planum):  spatia %d | parametra %d/%d\n",
        (integer)census.compressio.spatia_vocationes,
        (integer)census.compressio.parametra_compressa,
        (integer)census.compressio.parametra_visa);
    imprimere("  compressio (latinus): spatia %d | folia %d def / %d voc"
        " | parametra %d/%d\n",
        (integer)census_latinus.compressio.spatia_vocationes,
        (integer)census_latinus.compressio.folia_formae,
        (integer)census_latinus.compressio.folia_vocationes,
        (integer)census_latinus.compressio.parametra_compressa,
        (integer)census_latinus.compressio.parametra_visa);

    CREDO_VERUM (census.compressio.spatia_vocationes > ZEPHYRUM);
    CREDO_VERUM (census.compressio.parametra_visa > ZEPHYRUM);
    CREDO_VERUM (census.compressio.parametra_compressa > ZEPHYRUM);
    CREDO_VERUM (census.compressio.parametra_compressa
                     <= census.compressio.parametra_visa);

    CREDO_VERUM (census_latinus.compressio.spatia_vocationes
                     > ZEPHYRUM);
    CREDO_VERUM (census_latinus.compressio.folia_formae > ZEPHYRUM);
    CREDO_VERUM (census_latinus.compressio.folia_vocationes
                     > ZEPHYRUM);
    CREDO_VERUM (census_latinus.compressio.parametra_visa
                     > ZEPHYRUM);
    CREDO_VERUM (census_latinus.compressio.parametra_compressa
                     > ZEPHYRUM);
    CREDO_VERUM (census_latinus.compressio.parametra_compressa
                     <= census_latinus.compressio.parametra_visa);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
