/* semita.h - semitae: viae ut segmenta arboris status
 * (project-specs/semitae-spec.md R1). Semita = fragmentum STML formae
 * insulae: radix = genus organi, liberi = montationes nomine
 * (<nomen .../>), attributa = campi manifesti 'semita' declarati.
 * Forma canonica STML; URI derivatum. Semita statum SOLUM ponit.
 *
 * RAMI LIBERORUM (conventio): montatio (nomen, genus) in ramo
 * <genus id="nomen"/> sub ramo parentis (insula_ramus_liberi).
 * SCRIPTURA: campus quisque scriptore suo (dominus manifesti) -
 * scriptura una per scriptorem distinctum.
 *
 * LIGATIO HEREDITARIA: liber cuius organum ligamen a parente fert
 * ligationem suam a parente accipit - campi eius 'semita ligatio' in
 * semita recusantur et legendo omittuntur.
 *
 * URI: genus/ligatio.../?clavis.punctata=valor&... - via = ligatio
 * radicis ordine manifesti (ultimae vacuae omissae, media vacua =
 * absens; R4d: via nulla - "genus", "genus?..." - ligationem non
 * nominat, "genus/" nominat omnibus ad absens); quaestio = status
 * radicis, deinde liberorum (et ligatio liberorum propria) ordine
 * montationum, claves nominibus montationum punctatae. Effugium:
 * octeti extra A-Z a-z 0-9 - _ . ~ ut %XX (maiusculae). Valor vacuus =
 * ad absens. */

#ifndef SEMITA_H
#define SEMITA_H

/* <aedilis corpus="lib/semita.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"
#include "insula.h"
#include "organum.h"

/* organum generis dati (manifesta); NIHIL si ignotum */
nomen constans Organum* (*SemitaOrganum)(
     vacuum* ctx,
      chorda genus);

/* valor symbolicus -> concretus (resolutor vocantis: horologium);
 * FALSUM si resolvi non potest */
nomen b32 (*SemitaValor)(
                  vacuum* ctx,
    constans OrganumCampus* campus,
                  chorda  valor,
                 Piscina* piscina,
                  chorda* concretus);

/* semitam applicare: arbor TOTA primum iudicatur (genera, montationes,
 * campi semitae, valores typis, liberi montati); vitium = nihil
 * scriptum, *vitium nominat elementum et attributum. R4d: nodus cum
 * attributo ligationis ullo (etiam vacuo) ligationem TOTAM nominat -
 * campi ligationis non nominati ad absens; nodus sine ullo
 * ligationem servat. Deinde: ligatio, resetum campi ambitus
 * ligationis si ligatio mutata est, status; liberi post parentem.
 * R4e: valor symbolicus non resolutus recusatur (vitium nominat
 * campum). */
b32
semita_applicare (
           SemitaOrganum  organa,
                  vacuum* ctx,
    constans InsulaRamus* ramus,
      constans StmlNodus* semita,
                 Piscina* piscina,
                  chorda* vitium);

/* semita canonica status currentis: campi semitae soli, valores
 * absentes (= absens) omissi, ordine manifesti; R4d: ligatio radicis
 * semper nominata - omnibus ad absens campus primus ligationis
 * scribitur (absens aut vacuus); liberi montati soli (ramus
 * praesens), ligatio hereditaria omissa, liberi vacui omissi */
StmlNodus*
semita_legere (
            SemitaOrganum  organa,
                   vacuum* ctx,
     constans InsulaRamus* ramus,
       constans character* genus,
                  Piscina* piscina,
      InternamentumChorda* intern);

/* semita PLENA status currentis: ut semita_legere, sed campi semitae
 * OMNES (valores absentes expliciti: absens aut "") - restitutio
 * exacta (historia lateris); non canonica */
StmlNodus*
semita_legere_plenam (
            SemitaOrganum  organa,
                   vacuum* ctx,
     constans InsulaRamus* ramus,
       constans character* genus,
                  Piscina* piscina,
      InternamentumChorda* intern);

/* semita PERSISTENS (semitae R4d, restitutio post initium): ut
 * semita_legere, sed campi vitae transientis omissi. Ligatio nominata
 * manet si campus ligationis persistens ullus est (calendarium/dies:
 * dies_electus transiens omissus - applicata ad hodie redit); omnibus
 * transientibus ligatio non nominatur (nomenclator: index) */
StmlNodus*
semita_legere_persistentem (
            SemitaOrganum  organa,
                   vacuum* ctx,
     constans InsulaRamus* ramus,
       constans character* genus,
                  Piscina* piscina,
      InternamentumChorda* intern);

/* URI canonicum: genus/ligatio.../?clavis.punctata=valor&... ; vacua si
 * semita mala */
chorda
semita_ad_uri (
          SemitaOrganum  organa,
                 vacuum* ctx,
     constans StmlNodus* semita,
                Piscina* piscina);

/* URI -> semita (arbor eadem ac forma canonica); NIHIL et *vitium si
 * malum */
StmlNodus*
semita_ex_uri (
            SemitaOrganum  organa,
                   vacuum* ctx,
                   chorda  uri,
                  Piscina* piscina,
      InternamentumChorda* intern,
                   chorda* vitium);

/* URI singula per nodum ligationis propriae (radix et liberi non
 * hereditarii; status liberorum hereditariorum in URI parentis) - Xar
 * de chorda; NIHIL si semita mala */
Xar*
semita_ad_uri_lista (
          SemitaOrganum  organa,
                 vacuum* ctx,
     constans StmlNodus* semita,
                Piscina* piscina);

/* argumentum -> semita (semitae R4e): URI generis ("genus",
 * "genus/...", "genus?...") aut cognomen manifesti (fixum, typatum,
 * neutrum) ordine manifesti; NIHIL si nullum congruit (vocans
 * cognomina computata sua tentat) aut URI malum (*vitium) */
StmlNodus*
semita_ex_argumento (
            SemitaOrganum  organa,
                   vacuum* ctx,
       constans character* genus,
       constans character* argumentum,
                  Piscina* piscina,
      InternamentumChorda* intern,
                   chorda* vitium);

/* valores symbolicos in concretos mutare - AD LIMEN (historia, claves
 * proiectionum semper concreta); FALSUM si resolutor deest aut
 * deficit (*vitium nominat campum) */
b32
semita_resolvere (
            SemitaOrganum  organa,
                   vacuum* ctx,
                StmlNodus* semita,
              SemitaValor  resolutor,
                   vacuum* resolutor_ctx,
                  Piscina* piscina,
                   chorda* vitium);

#endif /* SEMITA_H */
