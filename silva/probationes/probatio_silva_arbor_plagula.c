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
#include "silva_scribere.h"
#include "silva_c89_oraculum.h"
#include "silva_arbor.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define CAUSAE_MAXIMAE 32

nomen structura {
    constans character* causa;
                   i32  numerus;
} CausaNumerata;

nomen structura {
    i32 plagulae;
    i32 octetim_exactae;
    i32 scriptura_recusata;
    i32 lectio_recusata;
    i32 emissio_recusata;
    i32 octeti_divergentes;
    i32 comparator_tacuit;   /* octeti divergunt, arbor 'aequalis' */

    CausaNumerata recusationes[CAUSAE_MAXIMAE];
              i32 numerus_recusationum;
    CausaNumerata divergentiae[CAUSAE_MAXIMAE];
              i32 numerus_divergentiarum;
} PlagulaCensus;

interior vacuum
_causam_notare (
      CausaNumerata* series,
                i32* numerus,
 constans character* causa)
{
    i32 i;

    si (causa == NIHIL)
    {
        causa = "(sine causa)";
    }
    per (i = ZEPHYRUM; i < *numerus; i++)
    {
        si (strcmp(series[i].causa, causa) == ZEPHYRUM)
        {
            series[i].numerus++;
            redde;
        }
    }
    si (*numerus >= CAUSAE_MAXIMAE)
    {
        redde;
    }
    series[*numerus].causa    = causa;
    series[*numerus].numerus  = I;
    (*numerus)++;
}

interior b32
_est_c_vel_h (
    constans character* titulus)
{
    memoriae_index m;

    m = strlen(titulus);
    si (m < III)
    {
        redde FALSUM;
    }
    si (   titulus[m - II] == '.'
        && (titulus[m - I] == 'c' || titulus[m - I] == 'h'))
    {
        redde VERUM;
    }
    redde FALSUM;
}

interior i8*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_out)
{
              FILE* pl;
                i8* buffer;
    signatus longus  mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);
    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (   mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
               != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    *mensura_out = (i32)mensura;
    redde buffer;
}

interior vacuum
_plagulam_probare (
    constans character* via,
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

    opus = piscina_generare_dynamicum("porta_plagulae", 67108864);
    si (opus == NIHIL)
    {
        redde;
    }
    fons = _plagulam_legere(opus, via, &mensura);
    si (fons == NIHIL || mensura <= ZEPHYRUM)
    {
        piscina_destruere(opus);
        redde;
    }
    census->plagulae++;

    origo = silva_parsare(opus, via, (constans character*)fons,
        mensura, &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
    si (origo == NIHIL || origo->commissio == NIHIL)
    {
        _causam_notare(census->recusationes,
            &census->numerus_recusationum, "parsura fracta");
        census->scriptura_recusata++;
        piscina_destruere(opus);
        redde;
    }

    scriptura = silva_arbor_scribere_parsuram(opus, origo,
        &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps, NIHIL);
    si (!scriptura.successus)
    {
        _causam_notare(census->recusationes,
            &census->numerus_recusationum, scriptura.causa);
        census->scriptura_recusata++;
        piscina_destruere(opus);
        redde;
    }

    lecta = silva_arbor_legere_parsuram(opus, NIHIL, scriptura.textus,
        &SILVA_C89_REGISTRUM, "c89", &vitium);
    si (lecta == NIHIL)
    {
        _causam_notare(census->recusationes,
            &census->numerus_recusationum, vitium.causa);
        census->lectio_recusata++;
        piscina_destruere(opus);
        redde;
    }

    emissio = silva_scribere_fontem(opus, lecta,
        &SILVA_C89_REGISTRUM, lecta->fons_princeps);
    si (!emissio.successus)
    {
        _causam_notare(census->recusationes,
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
    si (silva_arbor_parsurae_aequales(origo, lecta,
            SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia))
    {
        /* Octeti divergunt sed arbor 'aequalis' - comparator
         * CAECUS est ad hunc defectum. Numerandum: est mensura
         * quantum diagnosis nostra valeat. */
        census->comparator_tacuit++;
        _causam_notare(census->divergentiae,
            &census->numerus_divergentiarum,
            "(comparator tacuit)");
    }
    alioquin
    {
        _causam_notare(census->divergentiae,
            &census->numerus_divergentiarum, differentia.campus);
        imprimere("    %s: %s (via %s, index %d)\n", via,
            differentia.campus ? differentia.campus : "?",
            differentia.via, (integer)differentia.index);
        si (   differentia.lexema_a != NIHIL
            && differentia.lexema_b != NIHIL)
        {
            imprimere("      A b=%d l=%d c=%d | B b=%d l=%d c=%d\n",
                (integer)differentia.lexema_a->byte_offset,
                (integer)differentia.lexema_a->linea,
                (integer)differentia.lexema_a->columna,
                (integer)differentia.lexema_b->byte_offset,
                (integer)differentia.lexema_b->linea,
                (integer)differentia.lexema_b->columna);
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
     constans character* radix;
              character  via_corporis[512];
              character  via_plagulae[1024];
                    DIR* corpus;
         structura dirent* introitus;
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
        si (!_est_c_vel_h(introitus->d_name))
        {
            perge;
        }
        sprintf(via_plagulae, "%s/%s", via_corporis,
            introitus->d_name);
        _plagulam_probare(via_plagulae, &census);
    }
    closedir(corpus);

    imprimere("  plagulae:            %d\n", (integer)census.plagulae);
    imprimere("  OCTETIM EXACTAE:     %d / %d\n",
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

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
