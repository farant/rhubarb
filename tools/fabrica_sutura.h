/* tools/fabrica_sutura.h - INTERNA bin/fabrica (fabrica-6 H4):
 * sutura vera (discus, processus, photographia), memoria
 * (build/fabrica.db) et ansae graduum, quas mandata (tools/fabrica.c)
 * nectunt. Praefixum suturae_ (genetivus). Bibliotheca vera postea:
 * desideratum ...DCF4G1. */

/* <aedilis corpus="tools/fabrica_sutura.c"/> */
/* <aedilis corpus="tools/fabrica_memoria.c"/> */
/* <aedilis corpus="tools/fabrica_ansae.c"/> */
/* <aedilis corpus="tools/fabrica_portae.c"/> */
/* <aedilis corpus="tools/fabrica_unitates.c"/> */
/* <aedilis corpus="tools/fabrica_interrogationes.c"/> */
/* <aedilis corpus="tools/fabrica_relatio.c"/> */
/* <aedilis corpus="tools/fabrica_consilium.c"/> */

#ifndef FABRICA_SUTURA_H
#define FABRICA_SUTURA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "scrinium.h"
#include "processus.h"
#include "chorda_aedificator.h"
#include "fabrica.h"
#include "provenientia.h"
#include "sigillum.h"
#include "mensura.h"
#include "diurna.h"
#include "unitates.h"
#include "portae.h"

/* sanare: terminus unus per actum (briar ~3 min, obiecta radicis
 * frigida ~150 s); termini per actionem non in 1b */
#define MORA_SANATIONIS_MS  1800000

/* tectum memoriae membri gradus (credo memoria T6, decretum
 * 01M5CV9RETJA): FABRICA_MEMORIA_MB ex ambitu FABRICAE (non membri;
 * 0 = nullum), aliter hoc - XVI GiB, circa bis suitae verae
 * gravissimae (silva corporis VII.VI G, parcum 01M5CQYDD841);
 * minuendum cum illae emendantur. Tecta per sectionem credo sunt
 * (credo.h, T4) - hoc fugam solum sistit. */
#define FABRICA_MEMORIA_ORDINARIA_MB  (XVI * MXXIV)

/* memoria verificationum (tools/fabrica_memoria.c) */
nomen structura {
                  Scrinium* scrinium;
                   Piscina* piscina;
    constans FabricaSutura* sutura;   /* a suturae_memoriam_nectere
                                       * positum: lectio plagulae
                                       * sectionum (analytica T3b) */
            CursusInitium radix_aliena;    /* contextus radicis si ab
                                             * arbore sessionis differt
                                             * (T4), semel computatus */
                      b32  radix_aliena_nota;
                      b32  radix_repositorium;
                   Diurna* diurna;          /* corpora actionum (T4) */
                      b32  diurna_temptata;
                      s64  progressus_ultimus; /* ms lineae progressus
                                                * ultimae (colloquia T8c,
                                                * strangulatio) */
} Memoria;

/* mandata (fabrica.c) */
externus b32 suturae_machina;

vacuum
suturae_campus (
    chorda c);

/* sutura vera, photographia (fabrica_sutura.c) */
externus i32 suturae_fila_maxima;

b32
suturae_actum_complere (
       ProcessusResultus  resultus,
       ChordaAedificator* acta,
      constans character* acta_via,
                 Piscina* piscina,
            FabricaActum* actum_out);

vacuum
suturae_agentem_addere (
     FabricaSutura* sutura,
      constans Xar* actiones,
               b32  siccum,
           Piscina* piscina);

/* ambitus verus (getenv): explicatio membrorum (cursores ad gradus T4) */
vacuum
suturae_ambitum_addere (
    FabricaSutura* sutura);

vacuum
suturae_currentem_addere (
    FabricaSutura* sutura);

vacuum
suturae_currere_simul (
                            vacuum*  datum,
    constans FabricaActio* constans* actiones,
                               i32  numerus,
       constans character* constans* scripturae,
       constans character* constans* libri,
                           Piscina*  piscina,
                               b32*  felices_out,
                            chorda*  causae_out,
                               i32*  durationes_out);

i32
suturae_fila_computare (
    Piscina* piscina);

vacuum
suturae_iudicii_parare (
    FabricaSutura* sutura,
     constans Xar* actiones,
          Piscina* piscina);

vacuum
suturae_legentem_parare (
    FabricaSutura* sutura);

chorda
suturae_ultimae_lineae (
    chorda textus,
       i32 numerus);

/* photographia arboris (cwd) et directoriorum '~/X' (Xar de chorda;
 * NIHIL licet): FabricaVestigium ordinata per viam (consilium S5c1) */
b32
suturae_photographare (
    constans Xar*  directoria_extra,
         Piscina*  piscina,
             Xar** vestigia_out);

/* memoria (fabrica_memoria.c) */

constans character*
suturae_eventus_titulus (
    FabricaEventus eventus);

b32
suturae_lectiones_ultimae (
                vacuum*  datum,
    constans character*  titulus,
               Piscina*  piscina,
                   Xar** lectiones_out);

b32
suturae_memoriam_aperire (
    Memoria* memoria,
    Piscina* piscina);

vacuum
suturae_memoriam_nectere (
    FabricaSutura* sutura,
          Memoria* memoria);

chorda
suturae_radix_absoluta (
    Piscina* piscina);

/* ansae graduum (fabrica_ansae.c) */
externus constans character* suturae_via_binarii;
/* digestum extractoris infixum (build/fabrica/provenientia/extractor.c,
 * circulus commissionis P1) */
externus constans ProvenientiaRelatio provenientia_extractor;

/* clavis thesauri extractoris (aedilis_thesauri_praefixum super
 * digestum infixum) - sedes una: gradus et 'bin/fabrica
 * clavis-thesauri' */
b32
suturae_clavem_thesauri (
               Piscina*  piscina,
              Sigillum*  praefixum_out,
    constans character** fons_out);

b32
suturae_area_parare (
                vacuum* datum,
    constans character* area);

b32
suturae_clausura_c (
                vacuum* datum,
    constans character* scopus,
               Piscina* piscina,
      FabricaClausuraC* clausura_out,
                chorda* causa_out);

b32
suturae_compilare_gradus (
                vacuum* datum,
    constans character* fons,
    constans character* obiectum,
          constans Xar* vexilla_addita,
               Piscina* piscina,
                chorda* obiectum_out,
                chorda* erratum_out);

/* bin/fabrica portae debitae|declaratae (tools/fabrica_portae.c) */
s32
suturae_portae (
          s32   argc,
    character** argv,
      Piscina*  piscina);

/* bin/fabrica consilium (tools/fabrica_consilium.c, consilium S3b):
 * documentum consilii iudicatum - exitus 0 licet, 1 non sine iudicio,
 * 2 usus aut illegibile */
s32
suturae_consilium (
          s32   argc,
    character** argv,
      Piscina*  piscina);

/* bin/fabrica quid (tools/fabrica_unitates.c) */
s32
suturae_quid (
          s32   argc,
    character** argv,
      Piscina*  piscina);

/* portae relatio (tools/fabrica_relatio.c, unitates-codicis U5 a):
 * portae per genera unitatum - relatio, exitus 0; 2 tactus aut unitates
 * deficiunt */
s32
suturae_portae_relatio (
    constans PortaeSutura* sutura,
             constans Xar* declaratae,
             constans Xar* operturae,
             constans Xar* viae,
                   chorda  tempora,
                      b32  machina,
                  Piscina* piscina);

/* quid -interrogationes -quaere/-symbolum (tools/
 * fabrica_interrogationes.c): textus XOR symbolum; exitus 0 inventa,
 * 1 nihil congruit, 2 quaestio sine verbis */
s32
suturae_interrogationes_quaerere (
    constans UnitatesInterrogationes* res,
                  constans character* textus,
                  constans character* symbolum,
                                 b32  machina,
                             Piscina* piscina);

/* quid -interrogationes -html VIA: pagina (docs/faq.html); exitus 0
 * scripta, 1 vitia interrogationum (nihil scriptum), 2 scribi nequit */
s32
suturae_interrogationes_paginam_scribere (
    constans UnitatesInterrogationes* res,
                  constans character* via,
                             Piscina* piscina);

b32
suturae_in_area_currere (
                vacuum* datum,
          constans Xar* argv_x,
    constans character* area,
          constans Xar* ambitus,
    constans character* liber_via,
    constans character* acta_via,
               Piscina* piscina,
          FabricaActum* actum_out);

#endif /* FABRICA_SUTURA_H */
