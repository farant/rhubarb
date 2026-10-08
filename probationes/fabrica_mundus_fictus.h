/* fabrica_mundus_fictus.h - MUNDUS FICTUS probationum fabricae
 * (fabrica-6 H3): discus in memoria, sutura ficta, actiones fictae,
 * cursus et vestigia ficta, et vexilla mundi (variabiles quas
 * probationes ponunt). Solum probationes/probatio_fabrica_*.c id
 * includunt; praefixum mundi_ (genetivus). Sectio quaeque vexilla
 * quae mutat ad ordinarium reddit - probationes ordine liberae. */

#ifndef FABRICA_MUNDUS_FICTUS_H
#define FABRICA_MUNDUS_FICTUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "sigillum.h"
#include "fabrica.h"


/* ==================================================
 * Discus in memoria (sutura ficta)
 * ================================================== */

nomen structura {
    chorda via;
    chorda contentum;
       b32 deletum;
       s64 tempus;   /* horologium scripturae (1b T4: mtime fictum) */
} FasciculusFictus;

nomen structura {
    chorda  via;
       Xar* nomina;   /* chorda */
} DirectoriumFictum;

nomen structura {
               Piscina* piscina;
                   Xar* fasciculi;     /* FasciculusFictus */
                   Xar* directoria;    /* DirectoriumFictum */
    constans character* scriptura_rel; /* ubi generator scribit */
    constans character* generatio;     /* NIHIL = generator mutus */
                   b32  fractus;
                   i32  cursus;
    constans character* relatio;       /* NIHIL = sine provenientia */
                   i32  lecturae;      /* vocationes legere (memoria) */
                   Xar* verificationes; /* VerificatioFicta (T6) */
                   i32  inscriptiones; /* vocationes inscribere */
                   Xar* scripta;       /* ScriptumFictum (1b T3) */
                   i32  acta;          /* vocationes agere */
                   s64  horologium;    /* scripturae numeratae (T4) */
                   Xar* cursus_ficti;  /* CursusFictus (1b T7) */
    constans character* scriptura_altera; /* exitus secundus (T2) */
    constans character* generatio_altera;
    constans character* lectiones_ficti; /* liber quem currere scribit
                                          * (plan 2 T2); NIHIL = nullus */
                   Xar* vestigia_lectionum; /* VestigiumLectionumFictum */
                   i32  vestigia_scripta;   /* vocationes scribere */
                   Xar* undae_actae;  /* chorda: tituli undae simul
                                       * actae, spatiis (plan 2 T6) */
                   i32  acta_simul;   /* actiones per agere_simul */
                   Xar* undae_currendi; /* chorda: praevisiones (T6b) */
                   i32  fila_ficta;   /* post fracturam: membra ultra
                                       * hunc indicem non incipiunt */
                   Xar* particulae_servatae; /* ParticulaeFictae (plan-5
                                              * T1) */
} DiscusFictus;

/* particulae transitus servatae (plan-5 T1): ultimae per (titulus,
 * exitus) */
nomen structura {
    chorda  titulus;
    chorda  exitus;
       Xar* particulae;   /* FabricaParticula */
} ParticulaeFictae;

/* vestigium lectionum fictum (plan 2 T2): tabula 'lectiones' */
nomen structura {
      chorda  titulus;
      chorda  exitus;
    Sigillum  ingressus;
    Sigillum  artificium;
         Xar* lectiones;    /* FabricaLectio */
} VestigiumLectionumFictum;

/* tabula cursus ficta (1b T7) */
nomen structura {
            chorda titulus;
               i32 duratio_ms;
    FabricaEventus eventus;
            chorda stalum;   /* cur acta (fabrica-plan-5 T1) */
} CursusFictus;

/* generator scriptus (plan 1b T3): verbum = mandatum[0] actionis.
 * Contentum = praefixum + contentum(fons) - fons DISCI HODIERNI
 * legitur,
 * ergo dependentia vera modulatur. Regeneratio (currere) semper
 * scribit in scripturam; agere in loco, nisi mutus; codex agere
 * solum afficit; relatio (non NIHIL) post agere ponitur. */
nomen structura {
    constans character* verbum;
    constans character* via;
    constans character* fons;
    constans character* praefixum;
                   s32  codex;
                   b32  mutus;
    constans character* relatio;
    constans character* alia;   /* plagula praeterea scripta (T4:
                                 * scriptura extra vestigium) */
    constans character* scriptum_s; /* plagula scripta ET in libro 'S'
                                     * notata (plan 2 T6) */
    constans character* effusio;    /* acta cursoris (plan 5 T3:
                                     * signum in eis quaeritur); NIHIL
                                     * = nulla */
} ScriptumFictum;

/* memoria verificationum ficta (T6): tabula sqlite in memoria */
nomen structura {
      chorda titulus;
    Sigillum ingressus;
    Sigillum artificium;
} VerificatioFicta;

FasciculusFictus*
mundi_fasciculum_invenire (
                DiscusFictus* discus,
          constans character* via);

vacuum
mundi_ponere (
                DiscusFictus* discus,
          constans character* via,
          constans character* contentum);

vacuum
mundi_auferre (
          DiscusFictus* discus,
    constans character* via);

vacuum
mundi_directorium_ponere (
                DiscusFictus* discus,
          constans character* via,
          constans character* nomina[],
                         i32  numerus);

b32
mundi_legere (
                vacuum* datum,
    constans character* via,
               Piscina* piscina,
                chorda* contentum_out);

b32
mundi_enumerare (
                vacuum*  datum,
    constans character*  via,
               Piscina*  piscina,
                   Xar** nomina_out);

/* scriptum actionis per mandatum[0]; NIHIL si nullum */
ScriptumFictum*
mundi_scriptum_invenire (
     DiscusFictus* discus,
     constans Xar* mandatum);

/* contentum generatum: praefixum + contentum fontis hodiernum */
constans character*
mundi_generare (
      DiscusFictus* discus,
    ScriptumFictum* scriptum);

b32
mundi_currere (
                vacuum* datum,
          constans Xar* mandatum,
    constans character* scriptura_dir,
    constans character* liber_via,
               Piscina* piscina,
                chorda* causa_out,
                   i32* duratio_ms_out);

b32
mundi_agere (
                   vacuum* datum,
    constans FabricaActio* actio,
       constans character* acta_via,
                  Piscina* piscina,
             FabricaActum* actum_out);

/* verdictum ponere fictum (plan 5 T3): contentum NIHIL = deletum */
b32
mundi_verdictum_ponere (
                vacuum* datum,
    constans character* via,
       constans chorda* contentum);

vacuum
mundi_cursum_inscribere (
                     vacuum* datum,
    constans FabricaSanatio* sanatio);

/* duratio cursus ULTIMI SANATI aut PRAEPARATI tituli (ut SQL verum) */
b32
mundi_cursum_legere (
                vacuum* datum,
    constans character* titulus,
                   i32* duratio_ms_out);

s32
mundi_vestigia_comparare_via (
    constans vacuum* a,
    constans vacuum* b);

/* currere SIMUL fictum (T6b, praevisio): mundi_currere fictum per
 * membrum, unda nominata (tituli spatiis) in undae_actae */
vacuum
mundi_currere_simul (
                            vacuum*  datum,
    constans FabricaActio* constans* actiones,
                               i32  numerus,
       constans character* constans* scripturae,
       constans character* constans* libri,
                           Piscina*  piscina,
                               b32*  felices_out,
                            chorda*  causae_out,
                               i32*  durationes_out);

/* agere SIMUL fictum (plan 2 T6): membra ordine dato 'incipiunt';
 * post fracturam membra ab indice fila_ficta non incipiunt. Unda
 * acta notatur (tituli spatiis). scriptum_s: plagula scripta et 'S' in
 * libro membri. */
vacuum
mundi_agere_simul (
                            vacuum*  datum,
    constans FabricaActio* constans* actiones,
                               i32  numerus,
       constans character* constans* acta_viae,
       constans character* constans* libri,
                           Piscina*  piscina,
                      FabricaActum*  acta_out,
                               b32*  incepta_out);

b32
mundi_vestigium_capere (
     vacuum*  datum,
    Piscina*  piscina,
        Xar** vestigia_out);

b32
mundi_rogare (
                vacuum* datum,
    constans character* via,
               Piscina* piscina,
                chorda* relatio_out);

b32
mundi_lectiones_legere (
                vacuum*  datum,
    constans character*  titulus,
    constans character*  exitus,
     constans Sigillum*  ingressus,
     constans Sigillum*  artificium,
               Piscina*  piscina,
                   Xar** lectiones_out);

vacuum
mundi_lectiones_scribere (
                vacuum* datum,
    constans character* titulus,
    constans character* exitus,
     constans Sigillum* ingressus,
     constans Sigillum* artificium,
          constans Xar* lectiones);

vacuum
mundi_discum_parare (
          DiscusFictus* discus,
         FabricaSutura* sutura,
               Piscina* piscina);


/* ==================================================
 * Actiones fictae
 * ================================================== */

FabricaActio*
mundi_actio (
                 Piscina* piscina,
      constans character* titulus,
    FabricaGenusActionis  genus);

constans FabricaGenus*
mundi_genus (
    constans character* titulus,
               Piscina* piscina);

constans FabricaStrategia*
mundi_strategia (
    constans character* titulus,
               Piscina* piscina);

vacuum
mundi_ingressum_addere (
           FabricaActio* actio,
     constans character* genus,
     constans character* via,
                Piscina* piscina);

/* genus exitus = genus ordinarium strategiae (ut lector facit) */
FabricaExitus*
mundi_exitum_addere (
           FabricaActio* actio,
     constans character* via,
     constans character* strategia,
                Piscina* piscina);

b32
mundi_sigillum (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                  Sigillum* sigillum);

b32
mundi_continet (
                chorda  fenum,
    constans character* acus,
               Piscina* piscina);


/* ==================================================
 * Sanare: auxilia (plan 1b T3)
 * ================================================== */

vacuum
mundi_scriptum_addere (
          DiscusFictus* discus,
    constans character* verbum,
    constans character* via,
    constans character* fons,
    constans character* praefixum,
                   s32  codex,
                   b32  mutus);

/* actio cum generatore scripto, ingressu uno, exitu uno (scriptura =
 * via: regeneratio in scriptura/VIA cadit) */
FabricaActio*
mundi_actio_scripta (
               Piscina* piscina,
    constans character* titulus,
    constans character* verbum,
    constans character* ingressus,
    constans character* exitus,
    constans character* strategia);

Xar*
mundi_ordinare_fictas (
          Piscina* piscina,
     FabricaActio* actiones[],
              i32  numerus);

FabricaSanatio*
mundi_sanatio_invenire (
                   Xar* sanationes,
    constans character* titulus);

/* memoriae per cursum, ut sutura vera (bin/fabrica) eas habet */
/* sutura iudicii ficta (fabrica spec 3 T5b): ambitus portae (paria
 * titulus, valor; NIHIL finit), via ALIA (FIFO), identitas clang,
 * effectus cursoris */
externus constans character* mundi_ambitus_ficti[VIII];

externus constans character* mundi_via_alia_ficta;

externus constans character* mundi_identitas_ficta;

externus constans character* mundi_effectus_effusio;

externus constans character* mundi_via_absens_ficta;

b32
mundi_ambitus_fictum (
                vacuum* datum,
    constans character* titulus,
               Piscina* piscina,
                chorda* valor_out);

i32
mundi_species_ficta (
                vacuum* datum,
    constans character* via);

b32
mundi_identitatem_fictam_dare (
     vacuum* datum,
    Piscina* piscina,
   Sigillum* identitas_out,
     chorda* causa_out);

/* effectus scripti ficti (effectus-plan T7): lineae '-clavis' */
b32
mundi_effectus_ficti (
                vacuum* datum,
    constans character* via,
               Piscina* piscina,
                chorda* effusio_out,
                   i32* codex_out);

vacuum
mundi_memorias_parare (
    FabricaSutura* sutura,
          Piscina* piscina);

b32
mundi_contentum_est (
          DiscusFictus* discus,
    constans character* via,
    constans character* contentum);

#endif /* FABRICA_MUNDUS_FICTUS_H */
