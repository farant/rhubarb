/* probatio_silva_arbor_circuitus.c - PORTA M1 arboris
 *
 * Circuitus arboris super corpus commune (probationes/fixa/roundtrip
 * - LXXVIII plagulae, quaeque cicatrix defectus parsatoris veri),
 * per DUO oracula DISIUNCTA:
 *
 *   A. AEQUALITAS ARBORIS  scribere -> legere -> comparare (fidelitas)
 *   B. CIRCUITUS OCTETORUM legere -> silva_scribere_nodum -> octetos
 *                          contra emissionem subarboris originalis
 *
 * CUR ORACULUM B - ET QUID MENSURA CORREXERIT.
 *
 * Spec (§8) et planum (T6) dicunt oraculum B necessarium esse quia
 * comparator ad dominium GEMINUM triviorum CAECUS sit ('ambo domini
 * localiter recti videntur'). ID MENSURATUM EST ET NON TENET hac in
 * configuratione: vitio plantato (trivium unum duobus dominis
 * datum) AMBO oracula CEPERUNT - vide sectionem 'oracula disiuncta'
 * infra, quae 'A CEPIT, B CEPIT' imprimit.
 *
 * RATIO: comparator series triviorum PER LEXEMA confert, ergo
 * dominus novus seriem ostendit ubi origo nullam habebat, et A id
 * videt ut 'trivia/ante'. Casus quem spec imaginabatur - ubi ambo
 * domini localiter recti sunt - postulat originem trivia
 * aequi-contenta in AMBOBUS sedibus habere; tunc autem octeti quoque
 * congruunt et B pariter tacet. In circuitu ergo A ad dominium
 * triviorum non infirmior est quam B.
 *
 * B TAMEN RETINETUR, alia de causa quae vera est: oraculum
 * INDEPENDENS est. (i) semitam silva_scribere_nodum super arbores
 * LECTAS exercet, quam A numquam tangit; (ii) defectum in A IPSO
 * caperet - comparator qui 'aequales' falso diceret hic clamaret.
 * Vitium plantatum 'longitudo' infra ostendit oracula vere
 * disiuncta esse: A capit, B tacet.
 *
 * Vide etiam recensionem repraesentationalem infra: quod circuitum
 * non supervivit NOMINATIM numeratur et in worklog refertur. Id est
 * fructus quem haec phasis debet (visio §2.1).
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
#include "silva_quaestio.h"
#include "silva_scribere.h"
#include "silva_c89_oraculum.h"
#include "silva_arbor.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define CAUSAE_MAXIMAE      32
#define SUBARBORES_PER_GENUS 4

/* Genera quae per corpus eliguntur. Selector IDEM tag est ac tag
 * STML - ergo porta ipsa vocabularium commune DEGUSTAT (spec §8). */
hic_manens constans character* GENERA_PROBANDA[] = {
    "declaratio",
    "definitio-functionis",
    "sententia-expressionis"
};

nomen structura {
     constans character* causa;
                    i32  numerus;
} CausaNumerata;

nomen structura {
    i32 plagulae;
    i32 subarbores;
    i32 oraculum_a;        /* aequalitas arboris */
    i32 oraculum_b;        /* circuitus octetorum */
    i32 scriptura_recusata;
    i32 lectio_recusata;

    /* RECENSIO REPRAESENTATIONALIS: quid circuitum non supervivit */
    CausaNumerata divergentiae[CAUSAE_MAXIMAE];
              i32 numerus_divergentiarum;
    CausaNumerata recusationes[CAUSAE_MAXIMAE];
              i32 numerus_recusationum;
} ArborCensus;


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

/* Lexemata subarboris colligere (ordine ambulationis) */
interior vacuum
_lexemata_colligere (
     SilvaNodus*  nodus,
     SilvaToken** series,
            i32   capacitas,
            i32*  numerus)
{
    i32 i;
    i32 j;

    si (nodus == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        SilvaValor valor = nodus->loci[i];

        si (valor.genus == SILVA_VALOR_TOKEN && *numerus < capacitas)
        {
            series[*numerus] = valor.datum.token;
            (*numerus)++;
        }
        alioquin si (valor.genus == SILVA_VALOR_NODUS)
        {
            _lexemata_colligere(valor.datum.nodus, series, capacitas,
                numerus);
        }
        alioquin si (valor.genus == SILVA_VALOR_LISTA)
        {
            i32 quot = silva_valor_lista_numerus(valor);

            per (j = ZEPHYRUM; j < quot; j++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(valor, j);

                si (elem == NIHIL)
                {
                    perge;
                }
                si (   elem->genus == SILVA_VALOR_TOKEN
                    && *numerus < capacitas)
                {
                    series[*numerus] = elem->datum.token;
                    (*numerus)++;
                }
                alioquin si (elem->genus == SILVA_VALOR_NODUS)
                {
                    _lexemata_colligere(elem->datum.nodus, series,
                        capacitas, numerus);
                }
            }
        }
    }
}

/* Subarborem unam per DUO oracula probare */
interior vacuum
_subarborem_probare (
                Piscina* opus,
    constans SilvaNodus* origo,
  constans SilvaParsura* parsura,
            ArborCensus* census)
{
      SilvaArborScriptura  scriptura;
         SilvaArborVitium  vitium;
    SilvaArborDifferentia  differentia;
               SilvaNodus* lecta;
           SilvaScriptura  octeti_originis;
           SilvaScriptura  octeti_lectae;

    census->subarbores++;

    scriptura = silva_arbor_scribere_nodum(opus, origo,
        &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
    si (!scriptura.successus)
    {
        census->scriptura_recusata++;
        _causam_notare(census->recusationes,
            &census->numerus_recusationum, scriptura.causa);
        redde;
    }

    lecta = silva_arbor_legere(opus, NIHIL, scriptura.textus,
        &SILVA_C89_REGISTRUM, "c89", &vitium);
    si (lecta == NIHIL)
    {
        census->lectio_recusata++;
        _causam_notare(census->recusationes,
            &census->numerus_recusationum, vitium.causa);
        redde;
    }

    /* ORACULUM A */
    si (silva_arbor_aequalis(origo, lecta,
            SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia))
    {
        census->oraculum_a++;
    }
    alioquin
    {
        _causam_notare(census->divergentiae,
            &census->numerus_divergentiarum, differentia.campus);
    }

    /* ORACULUM B - octeti. Classis defectuum DISIUNCTA (vide caput) */
    octeti_originis = silva_scribere_nodum(opus, origo,
        &SILVA_C89_REGISTRUM, parsura->expansio);
    octeti_lectae   = silva_scribere_nodum(opus, lecta,
        &SILVA_C89_REGISTRUM, parsura->expansio);

    si (   octeti_originis.successus
        && octeti_lectae.successus
        && chorda_aequalis(octeti_originis.textus,
               octeti_lectae.textus))
    {
        census->oraculum_b++;
    }
    alioquin si (!octeti_originis.successus || !octeti_lectae.successus)
    {
        _causam_notare(census->divergentiae,
            &census->numerus_divergentiarum,
            "octeti: scriptura recusata");
    }
    alioquin
    {
        _causam_notare(census->divergentiae,
            &census->numerus_divergentiarum, "octeti: divergentes");
    }
}

/* Plagulam unam probare. PISCINA PROPRIA per plagulam, in OMNI
 * exitu destruenda: CCXXX+ plagulae in piscinam unam longaevam
 * parsatae eam exhauserunt et processus MEDIO CURSU sine ullo
 * signo mortuus est (formator.c:265-270 - mors mensurata, non
 * timor). */
interior vacuum
_plagulam_probare (
    constans character* via,
           ArborCensus* census)
{
               Piscina* opus;
                    i8* fons;
                   i32  mensura;
          SilvaParsura* parsura;
                   i32  g;

    opus = piscina_generare_dynamicum("arbor-opus", 67108864);
    si (opus == NIHIL)
    {
        redde;
    }

    fons = _plagulam_legere(opus, via, &mensura);
    si (fons == NIHIL || mensura == ZEPHYRUM)
    {
        piscina_destruere(opus);
        redde;
    }

    parsura = silva_c89_parsare(opus, via, (constans character*)fons,
        mensura, NIHIL);
    si (parsura == NIHIL || parsura->commissio == NIHIL)
    {
        piscina_destruere(opus);
        redde;
    }
    census->plagulae++;

    per (g = ZEPHYRUM;
         g < (i32)(magnitudo(GENERA_PROBANDA)
                   / magnitudo(GENERA_PROBANDA[0]));
         g++)
    {
        constans character* causa = NIHIL;
             SilvaQuaestio* quaestio;
                       Xar* resultata;
                       i32  numerus;
                       i32  i;

        quaestio = silva_quaestio_compilare(opus,
            &SILVA_C89_REGISTRUM, GENERA_PROBANDA[g], &causa);
        si (quaestio == NIHIL)
        {
            perge;
        }
        resultata = silva_quaestio_exsequi(quaestio,
            parsura->commissio->radix, opus);
        si (resultata == NIHIL)
        {
            perge;
        }
        numerus = xar_numerus(resultata);
        si (numerus > SUBARBORES_PER_GENUS)
        {
            numerus = SUBARBORES_PER_GENUS;
        }
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaQuaestioResultatum* fructus =
                (SilvaQuaestioResultatum*)xar_obtinere(resultata, i);

            si (fructus != NIHIL && fructus->nodus != NIHIL)
            {
                _subarborem_probare(opus, fructus->nodus, parsura,
                    census);
            }
        }
    }

    piscina_destruere(opus);
}

s32
principale (vacuum)
{
        Piscina* piscina;
    ArborCensus  census;
    DIR*                corpus;
    structura dirent*   introitus;
    constans character* radix;
    character           via_corporis[1024];
    character           via_plagulae[1024];
    b32                 praeteritus;
    i32                 i;

    piscina = piscina_generare_dynamicum("probatio_circuitus", 4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    memset(&census, 0, magnitudo(ArborCensus));

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = "..";
    }
    sprintf(via_corporis, "%s/probationes/fixa/roundtrip", radix);

    imprimere("\n--- PORTA M1: circuitus arboris super corpus ---\n");
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

    imprimere("  plagulae: %d, subarbores: %d\n",
        census.plagulae, census.subarbores);
    imprimere("  oraculum A (arbor):   %d / %d\n",
        census.oraculum_a, census.subarbores);
    imprimere("  oraculum B (octeti):  %d / %d\n",
        census.oraculum_b, census.subarbores);
    imprimere("  scriptura recusata: %d, lectio recusata: %d\n",
        census.scriptura_recusata, census.lectio_recusata);


    /* ==========================================================
     * RECENSIO REPRAESENTATIONALIS
     *
     * Fructus quem haec phasis debet: quid circuitum NON supervivit,
     * NOMINATIM. Numeratio tacita hic defectus esset - 'porta
     * viridis' de corpore cui nihil dixit idem videtur ac porta
     * quae omnia probavit.
     * ========================================================== */

    imprimere("\n  --- RECENSIO REPRAESENTATIONALIS ---\n");
    si (census.numerus_recusationum == ZEPHYRUM)
    {
        imprimere("  recusationes: nullae\n");
    }
    per (i = ZEPHYRUM; i < census.numerus_recusationum; i++)
    {
        imprimere("  RECUSATIO %6d x  %s\n",
            census.recusationes[i].numerus,
            census.recusationes[i].causa);
    }
    si (census.numerus_divergentiarum == ZEPHYRUM)
    {
        imprimere("  divergentiae: nullae\n");
    }
    per (i = ZEPHYRUM; i < census.numerus_divergentiarum; i++)
    {
        imprimere("  DIVERGENTIA %4d x  %s\n",
            census.divergentiae[i].numerus,
            census.divergentiae[i].causa);
    }

    /* Corpus vere ambulatum esse - porta quae nihil vidit viridis
     * esse NON debet */
    CREDO_VERUM (census.plagulae >= 70);
    CREDO_VERUM (census.subarbores >= 100);
    /* Omnia praeter recusationes documentatas circuitum superant */
    CREDO_AEQUALIS_I32 (census.oraculum_a,
        census.subarbores - census.scriptura_recusata
            - census.lectio_recusata);
    CREDO_AEQUALIS_I32 (census.oraculum_b, census.oraculum_a);


    /* ==========================================================
     * ORACULA DUO VERE DISIUNCTA SUNT?
     *
     * Porta viridis oracula duo IDEM agere posse celaret. Vitium
     * plantetur quod UNUM capiat et alterum NON - aliter alterum
     * otiosum est et id scire debemus.
     * ========================================================== */

    {
                      Piscina* opus;
                           i8* fons;
                          i32  mensura;
                 SilvaParsura* parsura;
          constans SilvaNodus* origo;
                   SilvaNodus* lecta;
          SilvaArborScriptura  scriptura;
             SilvaArborVitium  vitium;
        SilvaArborDifferentia  differentia;
                SilvaQuaestio* quaestio;
                          Xar* resultata;
           constans character* causa = NIHIL;
               SilvaScriptura  octeti_a;
               SilvaScriptura  octeti_b;

        imprimere("\n  --- ORACULA DISIUNCTA (vitium plantatum) ---\n");

        opus = piscina_generare_dynamicum("arbor-vitium", 16777216);
        CREDO_NON_NIHIL (opus);

        sprintf(via_plagulae, "%s/probationes/fixa/roundtrip", radix);
        corpus = opendir(via_plagulae);
        CREDO_NON_NIHIL (corpus);
        via_corporis[0] = '\0';
        dum ((introitus = readdir(corpus)) != NIHIL)
        {
            si (_est_c_vel_h(introitus->d_name))
            {
                sprintf(via_corporis, "%s/%s", via_plagulae,
                    introitus->d_name);
                frange;
            }
        }
        closedir(corpus);
        CREDO_VERUM (via_corporis[0] != '\0');

        fons    = _plagulam_legere(opus, via_corporis, &mensura);
        parsura = silva_c89_parsare(opus, via_corporis,
            (constans character*)fons, mensura, NIHIL);
        CREDO_NON_NIHIL (parsura);

        quaestio = silva_quaestio_compilare(opus,
            &SILVA_C89_REGISTRUM, "declaratio", &causa);
        CREDO_NON_NIHIL (quaestio);
        resultata = silva_quaestio_exsequi(quaestio,
            parsura->commissio->radix, opus);
        CREDO_NON_NIHIL (resultata);
        CREDO_VERUM (xar_numerus(resultata) > ZEPHYRUM);

        origo = ((SilvaQuaestioResultatum*)
            xar_obtinere(resultata, ZEPHYRUM))->nodus;
        scriptura = silva_arbor_scribere_nodum(opus, origo,
            &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
        CREDO_VERUM (scriptura.successus);
        lecta = silva_arbor_legere(opus, NIHIL, scriptura.textus,
            &SILVA_C89_REGISTRUM, "c89", &vitium);
        CREDO_NON_NIHIL (lecta);

        /* basis: ambo oracula viridia */
        CREDO_VERUM (silva_arbor_aequalis(origo, lecta,
            SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));

        /* VITIUM: 'longitudo' campus DERIVATUS est quem comparator
         * confert sed emissio NON adhibet (emissio valorem legit).
         * Ergo A cadere DEBET, B transire - quod probat oracula
         * classes disiunctas vere tenere, non idem bis agere. */
        {
            SilvaToken* lexema = NIHIL;
                   i32  i;

            per (i = ZEPHYRUM; i < lecta->numerus_locorum; i++)
            {
                si (lecta->loci[i].genus == SILVA_VALOR_TOKEN)
                {
                    lexema = lecta->loci[i].datum.token;
                    frange;
                }
            }
            /* 'declaratio' locum TOKEN fert (tok_terminator), ergo
             * ansa supra eum semper invenit - assertio id custodit */
            CREDO_NON_NIHIL (lexema);

            lexema->longitudo += X;

            /* A CADIT */
            CREDO_FALSUM (silva_arbor_aequalis(origo, lecta,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            CREDO_VERUM (differentia.campus != NIHIL
                && strcmp(differentia.campus, "lexema/longitudo")
                    == ZEPHYRUM);

            /* B TRANSIT - octeti ex valore veniunt, non ex longitudine */
            octeti_a = silva_scribere_nodum(opus, origo,
                &SILVA_C89_REGISTRUM, parsura->expansio);
            octeti_b = silva_scribere_nodum(opus, lecta,
                &SILVA_C89_REGISTRUM, parsura->expansio);
            CREDO_VERUM (octeti_a.successus && octeti_b.successus);
            CREDO_VERUM (chorda_aequalis(octeti_a.textus,
                octeti_b.textus));

            lexema->longitudo -= X;
        }

        /* DOMINIUM GEMINUM TRIVIORUM - claim plani probandum, non
         * praesumendum. Planum dicit oraculum B id capere ubi A
         * caecum sit. MENSUREMUS. */
        {
            SilvaToken* donans     = NIHIL;
            SilvaToken* recipiens  = NIHIL;
                   i32  i;
                   b32  a_cepit;
                   b32  b_cepit;

            {
                SilvaToken* series[64];
                       i32  quot = ZEPHYRUM;

                _lexemata_colligere(lecta, series, (i32)64, &quot);
                per (i = ZEPHYRUM; i < quot; i++)
                {
                    si (   donans                 != NIHIL
                        || series[i]->spatia_post == NIHIL)
                    {
                        si (   recipiens              == NIHIL
                            && series[i]->spatia_ante == NIHIL)
                        {
                            recipiens = series[i];
                        }
                        perge;
                    }
                    donans = series[i];
                }
            }
            /* Constructio INVENIRI debet - ramus non-cursus
             * probationem vacuam faceret (lectio hodierna) */
            CREDO_NON_NIHIL (donans);
            CREDO_NON_NIHIL (recipiens);

            si (donans != NIHIL && recipiens != NIHIL)
            {
                recipiens->spatia_ante = donans->spatia_post;

                a_cepit = !silva_arbor_aequalis(origo, lecta,
                    SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia);
                octeti_a = silva_scribere_nodum(opus, origo,
                    &SILVA_C89_REGISTRUM, parsura->expansio);
                octeti_b = silva_scribere_nodum(opus, lecta,
                    &SILVA_C89_REGISTRUM, parsura->expansio);
                b_cepit = !(octeti_a.successus && octeti_b.successus
                    && chorda_aequalis(octeti_a.textus,
                           octeti_b.textus));

                imprimere("  dominium geminum: A %s, B %s\n",
                    a_cepit ? "CEPIT" : "caecum",
                    b_cepit ? "CEPIT" : "caecum");

                /* MENSURATUM 2026-08-20: AMBO capiunt. Praesumptio
                 * spec/plani ('A caecum ad dominium geminum') hac
                 * in configuratione NON tenet - ratio in capite.
                 * Assertio id ut MENSURAM figit, ne quis claim
                 * refutatum ex spec relegat. */
                CREDO_VERUM (b_cepit);
                CREDO_VERUM (a_cepit);
                recipiens->spatia_ante = NIHIL;
            }
        }

        piscina_destruere(opus);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
