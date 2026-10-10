/* tools/fabrica_ansae.c - bin/fabrica: ANSAE GRADUUM (fabrica-6 T6c-2,
 * H4) - area, cursus in area, clausura C, compilatio per thesaurum.
 * Vide tools/fabrica_sutura.h. */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "iter_directoria.h"
#include "processus.h"
#include "internamentum.h"
#include "sigillum.h"
#include "git.h"
#include "tabula_dispersa.h"
#include "chorda_aedificator.h"
#include "fabrica.h"
#include "provenientia.h"
#include "scrinium.h"
#include "numerus_romanus.h"
#include "thesaurus.h"
#include "via.h"
#include "aedilis.h"
#include "aedilis_silva.h"
#include "compilator.h"
#include "fabrica_sutura.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>


/* ==================================================
 * GRADUS (fabrica-6 T6c-2): ansae verae - area, cursus in area,
 * clausura C (lib/aedilis + lib/aedilis_silva), compilatio
 * (lib/compilator, thesaurus build/aedilis/obiecta)
 * ================================================== */

/* via binarii (argv[0]): praefixum clavis thesauri extractoris */
constans character* suturae_via_binarii = "bin/fabrica";

/* status pigre paratus (solum si actio gradus exstat): radices
 * temporariae fumi aedilis.stml vacuam habent */
hic_manens                 b32  _gradus_tentati       = FALSUM;
hic_manens            Piscina* _piscina_gradus       = NIHIL;
hic_manens AedilisConfiguratio* _configuratio_gradus = NIHIL;
hic_manens        AedilisSilva* _extractor_gradus    = NIHIL;
hic_manens          Compilator* _compilator_gradus   = NIHIL;
hic_manens              chorda  _causa_gradus;

interior b32
_gradus_parare (
    chorda* causa_out)
{
               chorda binarium;
               chorda configuratio_octeti;
             Sigillum praefixum;
    SigillumContextus contextus;

    si (_gradus_tentati)
    {
        si (_compilator_gradus == NIHIL)
        {
            *causa_out = _causa_gradus;
        }
        redde _compilator_gradus != NIHIL;
    }
    _gradus_tentati = VERUM;
    _piscina_gradus = piscina_generare_dynamicum("fabrica_gradus",
        16777216);
    si (_piscina_gradus == NIHIL)
    {
        redde FALSUM;
    }
    _causa_gradus = chorda_ex_literis("", _piscina_gradus);
    _configuratio_gradus =
        aedilis_configurationem_legere(_piscina_gradus,
        "aedilis.stml", &_causa_gradus);
    si (_configuratio_gradus == NIHIL)
    {
        *causa_out = _causa_gradus;
        redde FALSUM;
    }
    binarium = filum_legere_totum(suturae_via_binarii, _piscina_gradus);
    configuratio_octeti = filum_legere_totum("aedilis.stml",
        _piscina_gradus);
    sigillum_incipere(&contextus);
    sigillum_addere(&contextus, binarium.datum,
        (memoriae_index)binarium.mensura);
    sigillum_addere(&contextus, configuratio_octeti.datum,
        (memoriae_index)configuratio_octeti.mensura);
    praefixum = sigillum_finire(&contextus);
    _extractor_gradus = aedilis_silva_creare(_piscina_gradus,
        _configuratio_gradus, binarium.mensura > ZEPHYRUM
            ? "build/aedilis/obiecta" : NIHIL, &praefixum, NIHIL);
    si (_extractor_gradus == NIHIL)
    {
        _causa_gradus = chorda_ex_literis("extractor aedilis parari "
            "nequit", _piscina_gradus);
        *causa_out = _causa_gradus;
        redde FALSUM;
    }
    _compilator_gradus = compilator_aperire(_piscina_gradus,
        "build/aedilis/obiecta", NIHIL);
    si (_compilator_gradus == NIHIL)
    {
        _causa_gradus = chorda_ex_literis("thesaurus compilatoris "
            "aperiri nequit (build/aedilis/obiecta)", _piscina_gradus);
        *causa_out = _causa_gradus;
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_chordam_unicam_addere (
       Xar* xar,
    chorda  valor)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(xar); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(xar, i), valor))
        {
            redde;
        }
    }
    *(chorda*)xar_addere(xar) = valor;
}

b32
suturae_clausura_c (
                vacuum* datum,
    constans character* scopus,
               Piscina* piscina,
      FabricaClausuraC* clausura_out,
                chorda* causa_out)
{
    AedilisFructus* fructus;
               i32  i;
               i32  k;

    (vacuum)datum;
    si (!_gradus_parare(causa_out))
    {
        redde FALSUM;
    }
    fructus = aedilis_derivare(piscina, _configuratio_gradus, scopus,
        NIHIL, aedilis_silva_extrahere, _extractor_gradus, causa_out);
    si (fructus == NIHIL)
    {
        redde FALSUM;
    }
    clausura_out->fontes         = xar_creare(piscina,
        (i32)magnitudo(chorda));
    clausura_out->capita         = xar_creare(piscina,
        (i32)magnitudo(chorda));
    clausura_out->vexilla_nexus  = xar_creare(piscina,
        (i32)magnitudo(chorda));
    clausura_out->facultates   = fructus->facultates;
    clausura_out->instrumenta  = fructus->instrumenta;
    /* scopus PRIMUS: fructus eum inter obiecta non fert (struere.sh
     * aedilis eum post obiecta compilat) */
    _chordam_unicam_addere(clausura_out->fontes,
        chorda_ex_literis(scopus,
        piscina));
    per (i = ZEPHYRUM; i < xar_numerus(fructus->obiecta); i++)
    {
        constans AedilisObiectum* obiectum = (constans AedilisObiectum*)
            xar_obtinere(fructus->obiecta, i);

        _chordam_unicam_addere(clausura_out->fontes, obiectum->via);
        per (k = ZEPHYRUM; obiectum->vexilla_nexus != NIHIL
             && k < xar_numerus(obiectum->vexilla_nexus); k++)
        {
            _chordam_unicam_addere(clausura_out->vexilla_nexus,
                *(chorda*)xar_obtinere(obiectum->vexilla_nexus, k));
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->vendores); i++)
    {
        _chordam_unicam_addere(clausura_out->fontes,
            ((constans AedilisVendor*)xar_obtinere(fructus->vendores,
            i))->fons);
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->capita); i++)
    {
        _chordam_unicam_addere(clausura_out->capita,
            ((constans AedilisCaput*)xar_obtinere(fructus->capita,
            i))->via);
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->vexilla_annotata); i++)
    {
        _chordam_unicam_addere(clausura_out->vexilla_nexus,
            *(chorda*)xar_obtinere(fructus->vexilla_annotata, i));
    }
    redde VERUM;
}

/* argv compilationis ut struere.sh aedilis: vendor = vexilla sua sola;
 * aliter vexilla communia, -I radicum, regula compilationis fontis */
b32
suturae_compilare_gradus (
                vacuum* datum,
    constans character* fons,
    constans character* obiectum,
               Piscina* piscina,
                chorda* erratum_out)
{
     constans AedilisConfiguratio*  c;
                              Xar*  verba;
               constans character** argv;
               CompilatorResultus   r;
                           chorda   fons_c;
                           chorda   directorium;
                              b32   vendor = FALSUM;
                              i32   i;
                              i32   k;

    (vacuum)datum;
    si (!_gradus_parare(erratum_out))
    {
        redde FALSUM;
    }
    c       = _configuratio_gradus;
    fons_c  = chorda_ex_literis(fons, piscina);
    /* argv compilatoris SINE 'clang' (verba post eum, ut bin/compilator
     * ea accipit - 'clang' ipse fons alter videretur) */
    verba   = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < xar_numerus(c->regulae_vendor); i++)
    {
        constans AedilisRegulaVendor* regula = (constans
            AedilisRegulaVendor*)xar_obtinere(c->regulae_vendor, i);

        si (chorda_aequalis(regula->fons, fons_c))
        {
            vendor = VERUM;
            per (k = ZEPHYRUM; k < xar_numerus(regula->vexilla); k++)
            {
                *(chorda*)xar_addere(verba) = *(chorda*)xar_obtinere(
                    regula->vexilla, k);
            }
        }
    }
    si (!vendor)
    {
        per (i = ZEPHYRUM; i < xar_numerus(c->vexilla); i++)
        {
            *(chorda*)xar_addere(verba) = *(chorda*)xar_obtinere(
                c->vexilla, i);
        }
        per (i = ZEPHYRUM; i < xar_numerus(c->inclusa); i++)
        {
            *(chorda*)xar_addere(verba) = chorda_concatenare(
                chorda_ex_literis("-I", piscina),
                *(chorda*)xar_obtinere(c->inclusa, i), piscina);
        }
        per (i = ZEPHYRUM; i
            < xar_numerus(c->regulae_compilationis); i++)
        {
            constans AedilisRegulaCompilationis* regula = (constans
                AedilisRegulaCompilationis*)xar_obtinere(
                c->regulae_compilationis, i);

            si (chorda_aequalis(regula->fons, fons_c))
            {
                per (k = ZEPHYRUM; k
                    < xar_numerus(regula->vexilla); k++)
                {
                    *(chorda*)xar_addere(verba) =
                        *(chorda*)xar_obtinere(
                        regula->vexilla, k);
                }
            }
        }
    }
    *(chorda*)xar_addere(verba) = chorda_ex_literis("-c", piscina);
    *(chorda*)xar_addere(verba) = fons_c;
    *(chorda*)xar_addere(verba) = chorda_ex_literis("-o", piscina);
    *(chorda*)xar_addere(verba) = chorda_ex_literis(obiectum, piscina);
    argv = (constans character**)piscina_allocare(piscina,
        magnitudo(character*) * (memoriae_index)(xar_numerus(verba)
            + I));
    si (argv == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(verba); i++)
    {
        argv[i] = chorda_ut_cstr(*(chorda*)xar_obtinere(verba, i),
            piscina);
    }
    argv[xar_numerus(verba)] = NIHIL;
    directorium = via_directorium(chorda_ex_literis(obiectum, piscina),
        piscina);
    (vacuum)filum_directorium_creare_cum_parentibus(chorda_ut_cstr(
        directorium, piscina));
    r = compilator_compilare(_compilator_gradus, argv,
        (i32)xar_numerus(verba));
    si (r.codex != ZEPHYRUM)
    {
        *erratum_out = suturae_ultimae_lineae(r.erratum.mensura
            > ZEPHYRUM
            ? r.erratum : r.effusio, V);
        redde FALSUM;
    }
    redde VERUM;
}

/* area: tota deleta, deinde <area>tmp/ creata */
b32
suturae_area_parare (
                vacuum* datum,
    constans character* area)
{
    character tmp[IV * MXXIV];

    (vacuum)datum;
    si (strlen(area) + IV >= magnitudo(tmp))
    {
        redde FALSUM;
    }
    sprintf(tmp, "%stmp", area);
    redde filum_arborem_delere(area)
        && filum_directorium_creare_cum_parentibus(tmp);
}

b32
suturae_in_area_currere (
                vacuum* datum,
          constans Xar* argv_x,
    constans character* area,
          constans Xar* ambitus,
    constans character* liber_via,
    constans character* acta_via,
               Piscina* piscina,
          FabricaActum* actum_out)
{
             constans character** argv;
             constans character** vector;
              ProcessusOptiones   optiones;
              ProcessusResultus   resultus;
              ChordaAedificator*  acta;
                         chorda   radix;
                         chorda   liber;
                            FILE* f;
                             i32  n;
                             i32  i;

    (vacuum)datum;
    (vacuum)area;
    actum_out->codex       = -I;
    actum_out->duratio_ms  = ZEPHYRUM;
    actum_out->cauda       = chorda_ex_literis("", piscina);
    n                      = (i32)xar_numerus(argv_x);
    argv = (constans character**)piscina_allocare(piscina,
        magnitudo(character*) * (memoriae_index)(n + I));
    vector = (constans character**)piscina_allocare(piscina,
        magnitudo(character*) * (memoriae_index)(xar_numerus(ambitus)
        + II));
    acta = chorda_aedificator_creare(piscina, IV * MXXIV);
    si (argv == NIHIL || vector == NIHIL || acta == NIHIL || n == 0)
    {
        actum_out->cauda = chorda_ex_literis("memoria deficit aut argv "
            "vacuum", piscina);
        redde FALSUM;
    }
    (vacuum)chorda_aedificator_appendere_literis(acta, "$");
    per (i = ZEPHYRUM; i < n; i++)
    {
        argv[i] = chorda_ut_cstr(*(constans chorda*)xar_obtinere(argv_x,
            i), piscina);
        (vacuum)chorda_aedificator_appendere_literis(acta, " ");
        (vacuum)chorda_aedificator_appendere_literis(acta, argv[i]);
    }
    argv[n] = NIHIL;
    /* liber ABSOLUTUS, vetus deletus, VACUUS creatus */
    radix = suturae_radix_absoluta(piscina);
    liber = chorda_concatenare(chorda_concatenare(radix,
        chorda_ex_literis("/", piscina), piscina),
        chorda_ex_literis(liber_via, piscina), piscina);
    (vacuum)filum_directorium_creare_cum_parentibus(chorda_ut_cstr(
        via_directorium(chorda_ex_literis(liber_via, piscina), piscina),
        piscina));
    (vacuum)remove(liber_via);
    f = fopen(liber_via, "w");
    si (f != NIHIL)
    {
        fclose(f);
    }
    per (i = ZEPHYRUM; i < xar_numerus(ambitus); i++)
    {
        vector[i] = chorda_ut_cstr(*(constans chorda*)xar_obtinere(
            ambitus, i), piscina);
    }
    vector[xar_numerus(ambitus)] = chorda_ut_cstr(chorda_concatenare(
        chorda_ex_literis("FABRICA_LECTIONES=", piscina), liber,
        piscina), piscina);
    vector[xar_numerus(ambitus) + I]  = NIHIL;
    /* cwd = radix (fabrica ipsa); ambitus exactus */
    optiones.directorium  = NIHIL;
    optiones.ambitus      = vector;
    (vacuum)filum_directorium_creare_cum_parentibus(chorda_ut_cstr(
        via_directorium(chorda_ex_literis(acta_via, piscina), piscina),
        piscina));
    resultus = processus_exsequi_cum(argv, &optiones,
        MORA_SANATIONIS_MS,
        piscina);
    redde suturae_actum_complere(resultus, acta, acta_via, piscina,
        actum_out);
}
