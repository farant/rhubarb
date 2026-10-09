/* herbarium.h - specimina pressa responsorum API: captura et redditio
 *
 * project-specs/herbarium-spec.md. Responsum inexspectatum (status >=
 * limen, aut a consumptore pressum: NOVITAS) prima vice qua GENUS eius
 * videtur premitur; visiones posteriores numerantur (index.jsonl),
 * non iterum servantur. Redditio: vectura quae specimina servata
 * reddit, ut probationes contra responsa VERA currant sine rete.
 *
 * STATUS: PROBATUM a Frano 2026-10-07 (vates-plan-1 T3); implementatio
 * in vates-plan-2. iudex et sedes ordinaria: PROBATUM a Frano 2026-10-09 (herbarium-spec-2
 * H0).
 *
 * USUS (captura):
 *   HerbariumOptiones o = herbarium_optiones_ordinariae();
 *   o.directorium = "/via/ad/captura";
 *   h = herbarium_aperire(piscina, &o);
 *   vectura = herbarium_vectura(h, http_vectura_ordinaria());
 *   res = http_vectura_exsequi(vectura, petitio, piscina);
 *
 * USUS (redditio in probatione):
 *   Xar* s = herbarium_enumerare(piscina, "probationes/fixa/vates/herbarium");
 *   vectura = herbarium_reddens(piscina, s);
 */

#ifndef HERBARIUM_H
#define HERBARIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "http.h"


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Functio clavis GENERIS: quid 'unicum' significet. Duo responsa eadem
 * clave = idem genus (visio numeratur, non servatur). */
nomen chorda (*HerbariumClavis)(
    HttpPetitio*   petitio,
    HttpResponsum* responsum,
    Piscina*       piscina,
    vacuum*        datum);

/* Functio IUDICIS (herbarium-spec-2, custos ante usum): causa cur
 * responsum INEXSPECTATUM sit; chorda vacua = exspectatum. Vocatur in
 * vectura capiente pro responsis sub status_minimus, ANTE redditionem -
 * specimen in disco antequam consumptor octetum videt. Solum codicem
 * hostili introitu tutum adhibeat (status, json_legere, norma_iudicare). */
nomen chorda (*HerbariumIudex)(
    HttpPetitio*   petitio,
    HttpResponsum* responsum,
    Piscina*       piscina,
    vacuum*        datum);

nomen structura {
     constans character* directorium;      /* NIHIL = captura exstincta */
                    i32  status_minimus;   /* 0 = CD */
                    i32  variantes_maximae; /* 0 = III */
        HerbariumClavis  clavis;           /* NIHIL = herbarium_clavis_sceleti */
                 vacuum* clavis_datum;
    constans character* constans* capita_admissa;   /* NIHIL-terminatum; ADDUNTUR ordinariis */
    constans character* constans* campi_petitionis; /* campi JSON corporis petitionis in summario (e.g. "model"); NIHIL-terminatum */
         HerbariumIudex  iudex;            /* NIHIL = status solus */
                 vacuum* iudex_datum;
} HerbariumOptiones;

nomen structura Herbarium Herbarium;   /* opacum */

/* Specimen unum (ex disco lectum). */
nomen structura {
        chorda  clavis;          /* clavis generis */
        chorda  sigillum;        /* hex SHA-256 clavis (nomen plagulae) */
           i32  variantes_index; /* I.. */
        chorda  causa;           /* "status" aut causa consumptoris */
           i32  status;
     HttpCaput* capita;          /* admissa sola */
           i32  capita_numerus;
        chorda  corpus;          /* verbatim */
        chorda  primum_visum;    /* ISO 8601 */
        chorda  methodus;
        chorda  hospes;
        chorda  via;
        chorda  summarium;       /* objectum JSON: campi_petitionis */
} HerbariumSpecimen;


/* ========================================================================
 * CAPTURA
 * ======================================================================== */

HerbariumOptiones
herbarium_optiones_ordinariae (vacuum);

/* Sedes ordinaria speciminum (herbarium-spec-2 §II.b):
 * $RHUBARB_HERBARIUM, aliter ~/.rhubarb/herbarium; deinde '/<hospes>'
 * (acervus unus per hospitem API). Chorda vacua + causa si
 * $RHUBARB_HERBARIUM directorium non exstans nominat (numquam tacite
 * alio cadit), si $HOME deest, aut si hospes vacuus est aut '/' aut
 * '..' continet. Directorium non creatur (herbarium_aperire creat). */
chorda
herbarium_sedes_ordinaria (
    constans character* hospes,
               Piscina* piscina,
                chorda* causa);

/* Aperire. directorium creatur si abest. NIHIL si creari non potest
 * (nuntius in stderr) - vocans sine captura pergit. */
Herbarium*
herbarium_aperire (
                       Piscina* piscina,
    constans HerbariumOptiones* optiones);

/* Vectura capiens: interiorem exsequitur, responsum TRANSMITTIT
 * immutatum; si praedicatum congruit, premit. Defectus capturae
 * (discus plenus) semel in stderr nuntiatur, numquam petitionem
 * frangit. */
HttpVectura
herbarium_vectura (
      Herbarium* herbarium,
    HttpVectura  involuta);

/* Premere explicite (NOVITAS): causa nominat quid inexspectatum fuit,
 * e.g. "blocus ignotus: server_tool_use". Praedicatum status NON
 * consulitur. */
vacuum
herbarium_premere (
        Herbarium* herbarium,
      HttpPetitio* petitio,
    HttpResponsum* responsum,
           chorda  causa);

/* Clavis ordinaria: status + sceleton JSON corporis (claves et genera,
 * recursive, valores omissi; corpus non-JSON -> status + content-type
 * + classis longitudinis). ORDINE NON PENDET (2026-10-09): claves
 * objecti ordinatae; tabulatum = unio formarum elementorum (distinctae,
 * ordinatae) - elementum formae novae ubicumque genus novum facit.
 * Publica ut consumptores eam componant. */
chorda
herbarium_clavis_sceleti (
      HttpPetitio* petitio,
    HttpResponsum* responsum,
          Piscina* piscina,
           vacuum* datum);


/* ========================================================================
 * REDDITIO
 * ======================================================================== */

/* Specimina directorii (HerbariumSpecimen*), ordine clavis deinde
 * variantis. Directorium absens -> Xar vacuum. */
Xar*
herbarium_enumerare (
               Piscina* piscina,
    constans character* directorium);

/* Vectura reddens: specimina ordine reddit (status, capita, corpus
 * verbatim); post finem HTTP_ERROR_CONNEXIO "herbarium exhaustum". */
HttpVectura
herbarium_reddens (
    Piscina* piscina,
        Xar* specimina);

#endif /* HERBARIUM_H */
