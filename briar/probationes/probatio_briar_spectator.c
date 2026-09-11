/* probatio_briar_spectator.c - Spectator (spec par. 4.7) SINE fenestra.
 *
 * Fenestram nulla probatio aperit. Sed fere nihil operis fenestra
 * eget: vestis e capsula LEGITUR (et capsula stala vitium verum est),
 * et pagina quam spectator ostendit EADEM est quae '-html' scribit.
 * Utrumque hic sine re vitrea probatur.
 *
 * Quod fenestram POSCIT (quod homo videt) gradus XII fumi est, sub
 * '-agere', per atrium_portus et bin/manus.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_contextus.h"
#include "briar_fabrica.h"
#include "briar_facies.h"
#include "briar_nexus.h"
#include "briar_silva.h"
#include "briar_symbolum.h"
#include "capsula.h"
#include "chorda.h"
#include "filum.h"
#include "internamentum.h"
#include "piscina.h"
#include "silex.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* vestis infixa: symbolum est contractus, caput genitum numquam
 * includitur (regula speculi) */
externus constans CapsulaEmbed capsula_facies_briar;

hic_manens constans character* FIXA =
    "briar/probationes/fixa/thistle/";

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior constans character*
_texere (
               Piscina* piscina,
    constans character* a,
    constans character* b)
{
    redde chorda_ut_cstr(chorda_concatenare(
        chorda_ex_literis(a, piscina),
        chorda_ex_literis(b, piscina), piscina), piscina);
}

/* plagulam reddere veste DATA; pagina reddita */
interior chorda
_reddere (
                 Piscina* piscina,
     InternamentumChorda* intern,
      constans SilexFons* fons,
      constans character* via,
    constans BriarVestis* vestis,
                  chorda* causa)
{
     BriarFabricaFructus  fructus;
    BriarFabricaOptiones  optiones;
               character* textus;
                     i32  mensura = ZEPHYRUM;
           MateriaNodus* doc;
                    Xar* nexus;
                    Xar* fragmenta = NIHIL;
                  chorda octeti;
                  chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    textus         = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        *causa = chorda_ex_literis("plagula non lecta", piscina);
        redde vacua;
    }
    doc    = briar_arbor_parsare(piscina, textus, mensura);
    nexus  = briar_nexus_texere(piscina, doc, intern);
    (vacuum)briar_contexere(piscina, nexus, &fragmenta);
    (vacuum)briar_silvam_texere(piscina, nexus, fons);
    optiones.via_thistle   = via;
    optiones.stampa        = "probatio";
    optiones.fons_titulus  = "probatio";
    octeti.datum           = (i8*)textus;
    octeti.mensura         = mensura;
    fructus = briar_fabricare(piscina, doc, nexus, fons, &optiones,
        octeti);
    redde briar_faciem_fingere(piscina, intern, nexus, fragmenta,
        &fructus, octeti, via, vestis, causa);
}

s32
principale (vacuum)
{
                  b32  praeteritus;
              Piscina* piscina;
  InternamentumChorda* intern;
   constans character* radix;
   constans SilexFons* fons;

    piscina = piscina_generare_dynamicum("probatio_briar_spectator",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern  = internamentum_creare(piscina);
    radix   = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    fons = silex_fons_disci(piscina, radix);
    CREDO_NON_NIHIL (fons);

    imprimere("\n--- Probans vestem e capsula ---\n");
    {
        BriarVestis vestis;
             chorda causa;

        CREDO_VERUM (briar_vestem_legere(piscina,
            &capsula_facies_briar, &vestis, &causa));
        CREDO_CHORDA_VACUA (causa);
        CREDO_VERUM (vestis.involucrum.mensura > ZEPHYRUM);
        CREDO_VERUM (vestis.styli.mensura      > ZEPHYRUM);
        CREDO_VERUM (vestis.scriptum.mensura   > ZEPHYRUM);
        CREDO_VERUM (vestis.exemplar.mensura   > ZEPHYRUM);

        /* CAPSULA STALA VITIUM VERUM EST: quod vehitur id esse debet
         * quod in arbore iacet. Aliter CSS emendatum in binario non
         * apparet et nemo causam invenit. */
        CREDO_CHORDA_AEQUALIS (vestis.involucrum,
            filum_legere_totum("briar/facies/facies.html", piscina));
        CREDO_CHORDA_AEQUALIS (vestis.styli,
            filum_legere_totum("briar/facies/facies.css", piscina));
        CREDO_CHORDA_AEQUALIS (vestis.scriptum,
            filum_legere_totum("briar/facies/facies.js", piscina));
        CREDO_CHORDA_AEQUALIS (vestis.exemplar,
            filum_legere_totum("briar/facies/md-html-facies.stml",
            piscina));
    }

    imprimere("\n--- Probans paginam eandem ---\n");
    {
          BriarVestis  e_capsula;
          BriarVestis  e_disco;
               chorda  causa;
               chorda  a;
               chorda  b;
   constans character* via = _texere(piscina, FIXA, "salve.thistle");

        CREDO_VERUM (briar_vestem_legere(piscina,
            &capsula_facies_briar, &e_capsula, &causa));
        e_disco.involucrum = filum_legere_totum(
            "briar/facies/facies.html", piscina);
        e_disco.styli      = filum_legere_totum(
            "briar/facies/facies.css", piscina);
        e_disco.scriptum   = filum_legere_totum(
            "briar/facies/facies.js", piscina);
        e_disco.exemplar   = filum_legere_totum(
            "briar/facies/md-html-facies.stml", piscina);

        a = _reddere(piscina, intern, fons, via, &e_capsula, &causa);
        CREDO_CHORDA_VACUA (causa);
        b = _reddere(piscina, intern, fons, via, &e_disco, &causa);
        CREDO_CHORDA_VACUA (causa);
        CREDO_VERUM (a.mensura > ZEPHYRUM);
        /* IDENTITAS: pagina quam spectator ostendet et pagina quam
         * '-html' scribit eaedem sunt - artificium unum, viae duae */
        CREDO_CHORDA_AEQUALIS (a, b);
        /* et vestis VERA adhibita est, non probatoria */
        CREDO_CHORDA_CONTINET (a,
            chorda_ex_literis("--fr-charta", piscina));
    }

    imprimere("\n--- Probans optiones plagulae (una sedes) ---\n");
    {
        BriarFabricaOptiones  o;
          constans character* via;

        via = _texere(piscina, FIXA, "salve.thistle");
        briar_optiones_plagulae(piscina, fons, via, &o);
        /* VIA ABSOLUTA: ea in capite paginae stat. Spectator et
         * '-html' hanc functionem VOCANT, ergo differre non possunt -
         * cursus verus me hoc docuit, non probatio: duae redditiones
         * intra processum unum optiones easdem ferunt et divergentiam
         * optionum videre NON possunt. */
        CREDO_NON_NIHIL (o.via_thistle);
        CREDO_AEQUALIS_S32 ((s32)(o.via_thistle[0] == '/'), (s32)1);
        CREDO_CHORDA_CONTINET (chorda_ex_literis(o.via_thistle,
            piscina), chorda_ex_literis("salve.thistle", piscina));
        CREDO_CHORDA_AEQUALIS (chorda_ex_literis(o.stampa, piscina),
            chorda_ex_literis(fons->titulus, piscina));
    }

    imprimere("\n--- Probans symbola bibliothecae ---\n");
    {
         BriarCursorSymbolorum* cursor;
         BriarSymboliResponsum  r;

        cursor = briar_cursorem_symbolorum_creare(piscina, fons);
        CREDO_NON_NIHIL (cursor);

        /* inventum: gemellum, textus, linea */
        r = briar_symbolum_quaerere(cursor,
            chorda_ex_literis("piscina_generare_dynamicum", piscina),
            chorda_ex_literis("piscina.h", piscina));
        CREDO_VERUM (r.inventum);
        CREDO_CHORDA_AEQUALIS_LITERIS (r.via, "lib/piscina.c");
        CREDO_CHORDA_CONTINET (r.definitio,
            chorda_ex_literis("piscina_generare_dynamicum", piscina));
        CREDO_MAIOR_S32 ((s32)r.linea, (s32)0);

        /* caput SINE gemello: latina.h macra sola fert - non vitium,
         * sed 'non inventum' */
        r = briar_symbolum_quaerere(cursor,
            chorda_ex_literis("NIHIL", piscina),
            chorda_ex_literis("latina.h", piscina));
        CREDO_FALSUM (r.inventum);

        /* symbolum quod nusquam est */
        r = briar_symbolum_quaerere(cursor,
            chorda_ex_literis("nemo_hic_est_omnino", piscina),
            chorda_ex_literis("piscina.h", piscina));
        CREDO_FALSUM (r.inventum);

        /* caput FALSUM: symbolum verum, gemellum aliud - responsum
         * dimidiatum numquam */
        r = briar_symbolum_quaerere(cursor,
            chorda_ex_literis("piscina_generare_dynamicum", piscina),
            chorda_ex_literis("chorda.h", piscina));
        CREDO_FALSUM (r.inventum);

        /* plagula ALIA arborem ALIAM habet: cursor VIA clavem facit.
         * Sine hac assertione cursor viam ignorans una sola
         * assertione rubesceret (lectio T3: numerus rubrorum
         * legendus est). */
        {
            i32 ante = briar_cursoris_parsurae(cursor);

            /* xar.h nondum tacta: chorda.h iam in cursore est
               (quaesitio capitis falsi eam parsavit) */
            r = briar_symbolum_quaerere(cursor,
                chorda_ex_literis("xar_creare", piscina),
                chorda_ex_literis("xar.h", piscina));
            CREDO_VERUM (r.inventum);
            CREDO_CHORDA_AEQUALIS_LITERIS (r.via, "lib/xar.c");
            CREDO_CHORDA_CONTINET (r.definitio,
                chorda_ex_literis("xar_creare", piscina));
            CREDO_AEQUALIS_S32 ((s32)briar_cursoris_parsurae(cursor),
                (s32)(ante + I));
        }

        /* CURSOR: symbolum alterum EIUSDEM plagulae nihil parsat */
        {
                              i32 ante;
            BriarSymboliResponsum s2;

            ante = briar_cursoris_parsurae(cursor);

            s2 = briar_symbolum_quaerere(cursor,
                chorda_ex_literis("piscina_destruere", piscina),
                chorda_ex_literis("piscina.h", piscina));
            CREDO_VERUM (s2.inventum);
            CREDO_AEQUALIS_S32 ((s32)briar_cursoris_parsurae(cursor),
                (s32)ante);
            /* et responsum idem bis */
            r = briar_symbolum_quaerere(cursor,
                chorda_ex_literis("piscina_destruere", piscina),
                chorda_ex_literis("piscina.h", piscina));
            CREDO_CHORDA_AEQUALIS (r.definitio, s2.definitio);
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
