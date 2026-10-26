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

/* digestum extractoris (circulus commissionis P1): actio 'aedilis' in
 * aedificatione bin/fabrica infixa (tools/fabrica_struere.sh:
 * provenientia_scribere.sh extractor bin/aedilis aedilis) - clavis
 * thesauri eadem ac bin/aedilis */
/* <aedilis obiectum="build/fabrica/provenientia/extractor.c"/> */
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
/* CLAUSURA TEMPORARIA (parcum T77EH4): derivatio aedilis (machina,
 * tabulae, fructus) ~III.V MB per membrum - CCCCXI membra radicis =
 * I.IV GB in piscina cursus, olim usque ad finem servata. Hic
 * derivatur, vacatur ante vocationem quamque; listae solae
 * transcribuntur */
hic_manens            Piscina* _piscina_clausurae   = NIHIL;

/* clavis thesauri extractoris (P1): digestum infixum, aliter
 * binarium - UNA sedes pro extractore gradus et 'clavis-thesauri' */
b32
suturae_clavem_thesauri (
               Piscina*  piscina,
              Sigillum*  praefixum_out,
    constans character** fons_out)
{
    redde aedilis_thesauri_praefixum(provenientia_extractor.ingressus,
        suturae_via_binarii, "aedilis.stml", piscina, praefixum_out,
        fons_out);
}

interior b32
_gradus_parare (
    chorda* causa_out)
{
             Sigillum praefixum;
                  b32 clavis_bona;

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
    /* clavis thesauri: digestum extractoris infixum (P1); sine eo
     * binarium ipsum - illegibile = sine thesauro (ut olim) */
    clavis_bona = suturae_clavem_thesauri(_piscina_gradus, &praefixum,
        NIHIL);
    _extractor_gradus = aedilis_silva_creare(_piscina_gradus,
        _configuratio_gradus,
        clavis_bona ? "build/aedilis/obiecta" : NIHIL,
        &praefixum, NIHIL);
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

/* valor in 'piscina' transcriptus (fructus derivationis temporarius) */
interior vacuum
_chordam_unicam_addere (
        Xar* xar,
     chorda  valor,
    Piscina* piscina)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(xar); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(xar, i), valor))
        {
            redde;
        }
    }
    *(chorda*)xar_addere(xar) = chorda_transcribere(valor, piscina);
}

/* Xar chordarum in 'piscina' transcripta (NIHIL -> NIHIL) - fructus
 * derivationis in piscina temporaria vivit */
interior Xar*
_chordas_transcribere (
    constans Xar* chordae,
         Piscina* piscina)
{
    Xar* nova;
    i32  i;

    si (chordae == NIHIL)
    {
        redde NIHIL;
    }
    nova = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; nova != NIHIL && i < xar_numerus(chordae); i++)
    {
        *(chorda*)xar_addere(nova) = chorda_transcribere(
            *(constans chorda*)xar_obtinere(chordae, i), piscina);
    }
    redde nova;
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
           Piscina* derivatio;
               i32  i;
               i32  k;

    (vacuum)datum;
    si (!_gradus_parare(causa_out))
    {
        redde FALSUM;
    }
    si (_piscina_clausurae == NIHIL)
    {
        _piscina_clausurae = piscina_generare_dynamicum(
            "fabrica_clausura", 4194304);
    }
    derivatio = (_piscina_clausurae != NIHIL) ? _piscina_clausurae
        : piscina;
    si (derivatio != piscina)
    {
        piscina_vacare(derivatio);
    }
    fructus = aedilis_derivare(derivatio, _configuratio_gradus, scopus,
        NIHIL, aedilis_silva_extrahere, _extractor_gradus, causa_out);
    si (fructus == NIHIL)
    {
        *causa_out = chorda_transcribere(*causa_out, piscina);
        redde FALSUM;
    }
    clausura_out->fontes         = xar_creare(piscina,
        (i32)magnitudo(chorda));
    clausura_out->capita         = xar_creare(piscina,
        (i32)magnitudo(chorda));
    clausura_out->vexilla_nexus  = xar_creare(piscina,
        (i32)magnitudo(chorda));
    clausura_out->facultates   = _chordas_transcribere(
        fructus->facultates, piscina);
    clausura_out->instrumenta  = _chordas_transcribere(
        fructus->instrumenta, piscina);
    /* scopus PRIMUS: fructus eum inter obiecta non fert (struere.sh
     * aedilis eum post obiecta compilat) */
    _chordam_unicam_addere(clausura_out->fontes,
        chorda_ex_literis(scopus, derivatio), piscina);
    per (i = ZEPHYRUM; i < xar_numerus(fructus->obiecta); i++)
    {
        constans AedilisObiectum* obiectum = (constans AedilisObiectum*)
            xar_obtinere(fructus->obiecta, i);

        _chordam_unicam_addere(clausura_out->fontes, obiectum->via,
            piscina);
        per (k = ZEPHYRUM; obiectum->vexilla_nexus != NIHIL
             && k < xar_numerus(obiectum->vexilla_nexus); k++)
        {
            _chordam_unicam_addere(clausura_out->vexilla_nexus,
                *(chorda*)xar_obtinere(obiectum->vexilla_nexus, k),
                piscina);
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->vendores); i++)
    {
        _chordam_unicam_addere(clausura_out->fontes,
            ((constans AedilisVendor*)xar_obtinere(fructus->vendores,
            i))->fons,
            piscina);
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->capita); i++)
    {
        _chordam_unicam_addere(clausura_out->capita,
            ((constans AedilisCaput*)xar_obtinere(fructus->capita,
            i))->via,
            piscina);
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->vexilla_annotata); i++)
    {
        _chordam_unicam_addere(clausura_out->vexilla_nexus,
            *(chorda*)xar_obtinere(fructus->vexilla_annotata, i),
            piscina);
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
          constans Xar* vexilla_addita,
               Piscina* piscina,
                chorda* obiectum_out,
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
    /* vexilla variantis (gradus, 2026-10-22) post domus: ultima
     * vincunt (-O1 super -O2) */
    per (i = ZEPHYRUM; vexilla_addita != NIHIL
         && i < xar_numerus(vexilla_addita); i++)
    {
        *(chorda*)xar_addere(verba) = *(constans chorda*)xar_obtinere(
            vexilla_addita, i);
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
    /* piscina vocationis (actum membri, temporaria): lectiones
     * thesauri cum membro vacantur, non in _piscina_gradus crescunt
     * (~I MB per compilationem, parcum …T77EH4) */
    /* NEXUS EX THESAURO: in hit nulla copia in area membri - blobus
     * thesauri directe nectitur (olim XII milia obiectorum radicis in
     * areis, photographia et IO per membrum) */
    r = compilator_compilare_sine_collocatione(_compilator_gradus, argv,
        (i32)xar_numerus(verba), piscina);
    si (r.codex != ZEPHYRUM)
    {
        *erratum_out = suturae_ultimae_lineae(r.erratum.mensura
            > ZEPHYRUM
            ? r.erratum : r.effusio, V);
        redde FALSUM;
    }
    *obiectum_out = r.via;
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

/* tectum memoriae membri (credo memoria T6): FABRICA_MEMORIA_MB aut
 * FABRICA_MEMORIA_ORDINARIA_MB, in octetis; 0 = nullum */
interior i64
_memoriam_maximam_legere (vacuum)
{
     constans character* valor = getenv("FABRICA_MEMORIA_MB");
                    s64  mb;

    mb = (valor != NIHIL && valor[ZEPHYRUM] != '\0')
        ? (s64)strtol(valor, NIHIL, X)
        : (s64)FABRICA_MEMORIA_ORDINARIA_MB;
    si (mb <= (s64)ZEPHYRUM)
    {
        redde (i64)ZEPHYRUM;
    }
    redde (i64)mb * (i64)MXXIV * (i64)MXXIV;
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
    memset(&optiones, ZEPHYRUM, magnitudo(optiones));
    optiones.directorium     = NIHIL;
    optiones.ambitus         = vector;
    optiones.memoria_maxima  = _memoriam_maximam_legere();
    /* GREX PROPRIUS (2026-10-26, vicus_admonitiones): processus quos
     * membrum relinquit (mensor asynchronus cum libro SUO) post exitum
     * occiduntur - aliter in libro suo scribunt dum membrum proximum
     * currit, et photographia proximum culpat */
    optiones.grex_proprius   = VERUM;
    (vacuum)filum_directorium_creare_cum_parentibus(chorda_ut_cstr(
        via_directorium(chorda_ex_literis(acta_via, piscina), piscina),
        piscina));
    resultus = processus_exsequi_cum(argv, &optiones,
        MORA_SANATIONIS_MS,
        piscina);
    {
        b32 bonum = suturae_actum_complere(resultus, acta, acta_via,
            piscina, actum_out);

        si (resultus.relicti)
        {
            fprintf(stderr, "fabrica: MONITUM %s: processus post exitum"
                " relicti (grex occisus)\n", argv[0]);
            actum_out->cauda = chorda_concatenare(actum_out->cauda,
                chorda_ex_literis(" [MONITUM: processus relicti post"
                " exitum occisi]", piscina), piscina);
        }
        redde bonum;
    }
}
