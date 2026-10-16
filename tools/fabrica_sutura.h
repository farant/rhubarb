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

/* sanare: terminus unus per actum (briar ~3 min, obiecta radicis
 * frigida ~150 s); termini per actionem non in 1b */
#define MORA_SANATIONIS_MS  1800000

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
               Piscina* piscina,
                chorda* erratum_out);

/* bin/fabrica portae debitae|declaratae (tools/fabrica_portae.c) */
s32
suturae_portae (
          s32   argc,
    character** argv,
      Piscina*  piscina);

/* bin/fabrica quid (tools/fabrica_unitates.c) */
s32
suturae_quid (
          s32   argc,
    character** argv,
      Piscina*  piscina);

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
