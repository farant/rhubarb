/* unitates.h - UNITATES CODICIS: quid plagula repositorii SIT
 * (spec project-specs/unitates-codicis-spec.md; desideratum …MJ2)
 *
 * Responsum UNUM ad 'quid est haec plagula': unitas intima cui
 * pertinet (cum catena parentum), genus (via naturae), cura, vita,
 * munus, provenientia, lingua - et pro QUOQUE valore regula quae eum
 * decrevit (causa). Consumptores (lint, formator, unci, fabrica)
 * hinc rogant, numquam indicem suum viarum (novem indices exclusionum
 * spec par. 0 - lex 'porta quae NOMINAT').
 *
 * STRATA TRIA (spec par. VI tres-quartae):
 *   natura         genera (ontologia reutilis) - hic solum citata
 *   unitates.stml  POLITICA domus (canon unitates.canon): curae,
 *                  vitae, genera citabilia cum cura praestituta,
 *                  regulae munerum ORDINATAE, linguae, iudices,
 *                  conventio bibliothecarum, familiae
 *   DIR/unitas.stml  declarationes unitatum directorii (canon
 *                  unitas.canon)
 *
 * UNITATES DUORUM FONTIUM:
 *   directorii   DIR/unitas.stml: unitas directorium DIR vendicat
 *   bibliothecae stemma S (capita/S.h aut fontes/S.c exstat) vendicat
 *                capita/S.h, fontes/S.c, fontes/S.m,
 *                fontes/S.worklog.md,
 *                probationes/PRAEFIXUM S.c - stemma S_PARS (partes:
 *                _macos _posix ...) ad S pertinet; bibliotheca
 *                RADIX_X familiae RADIX intra RADIX nidificat, ceterae
 *                intra unitatem directorii intimam (radix: rhubarb)
 * Plagula ad unitatem INTIMAM pertinet; bibliotheca vincit
 * directorium continens. DIR/unitas.stml ipsa ad unitatem CONTINENTEM
 * pertinet (tabula domus de filio: vendor/unitas.stml domestica est;
 * radicis declaratio radicis manet). Identitas = genus + titulus
 * (applicatio vicus et bibliotheca vicus duae sunt).
 *
 * HEREDITAS: cura = declarata in unitate, aliter declarata in
 * maiore proximo, aliter praestituta generis; vita = declarata,
 * aliter maioris proximi, aliter 'viva'. Archivum ergo omnia sua
 * congelat, vendor omnia aliena facit.
 *
 * DERIVATA (numquam declarata): munus per regulas politicae (prima
 * congruens vincit), lingua per extensionem, provenientia per
 * SIGNUM GENERATI: 'GENERATUM' in linea prima, aut secunda si prima
 * declaratio est quam forma poscit (<?xml, <!doctype) - positio fixa,
 * non fenestra (caput quod GENERATUM in prosa nominat manu est).
 *
 * RECUSATIO, numquam praestitutum tacitum: genus non citabile, cura
 * vita munus lingua ignota (in declaratione AUT in indicibus
 * iudicum), titulus bis in eodem genere, familia sine bibliotheca,
 * unitas.stml illegibilis - causae OMNES simul nominantur, cum
 * valoribus notis.
 *
 * MACHINA PURA: ingressus per suturam (exemplar portae.h) -
 * probationes plagulas in memoria dant; CLI (bin/fabrica quid) discum
 * et 'git ls-files -co --exclude-standard'.
 */
#ifndef UNITATES_H
#define UNITATES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

nomen structura {
          vacuum* datum;
    /* viae OMNES arboris (tractae + novae non ignoratae), radici
     * relativae: Xar de chorda; NIHIL = enumeratio fracta. Hinc
     * unitas.stml inveniuntur et stemmata bibliothecarum */
            Xar* (*plagulas_enumerare) (vacuum* datum,
                Piscina* piscina);
    /* textus plagulae; FALSUM si legi nequit */
            b32  (*plagulam_legere) (vacuum* datum, chorda via,
                                     Piscina* piscina, chorda* textus);
} UnitatesSutura;

nomen enumeratio {
    UNITAS_DIRECTORII = ZEPHYRUM,   /* DIR/unitas.stml */
    UNITAS_BIBLIOTHECAE             /* stemma conventionis */
} UnitatisFons;

nomen structura Unitas Unitas;
structura Unitas {
         chorda titulus;
         chorda genus;        /* via naturae (citabilis in politica) */
         chorda descriptio;   /* bibliothecae: vacua */
   UnitatisFons fons;
         chorda via;          /* directorium, aut stemma bibliothecae */
         chorda declaratio;   /* unde: 'apps/vicus/unitas.stml',
                                * 'stemma include/xar.h' */
           chorda  cura;         /* DECLARATA; vacua = hereditata */
           chorda  vita;         /* DECLARATA; vacua = hereditata */
           chorda  successor;    /* vita vetusta; vacua aliter */
              Xar* usus;         /* chorda: homo agens machina; NIHIL */
  constans Unitas* parens;     /* NIHIL = radix */
};

/* valor cum regula quae eum decrevit. valor vacuus = nulla regula
 * congruit (munus) aut ignotum (provenientia illegibilis) - causa
 * id quoque nominat */
nomen structura {
    chorda valor;
    chorda causa;     /* 'apps/vicus/unitas.stml', 'genus
                       * /fabricatio/adscitio', 'regula munus 3
                       * (segmentum probationes, .c .m)',
                       * 'linea I: GENERATUM' */
} UnitatesProprietas;

nomen structura {
               chorda  via;
      constans Unitas* unitas;      /* intima; numquam NIHIL */
               chorda  unitatis_causa; /* 'stemma xar', 'stemma
                                        * energia_macos pars
                                        * bibliothecae energia',
                                        * 'probatio stemmatis xar',
                                        * 'directorium apps/vicus
                                        * (apps/vicus/unitas.stml)' */
   UnitatesProprietas cura;        /* hereditata resoluta */
   UnitatesProprietas vita;
   UnitatesProprietas munus;
   UnitatesProprietas provenientia; /* manu / generatum */
   UnitatesProprietas lingua;
} UnitatisResponsum;

/* recusatio una: locus (via vacua = politica tota; linea ZEPHYRUM =
 * sine linea) et causa */
nomen structura {
    chorda via;
       i32 linea;
    chorda causa;
} UnitatesRecusatio;

nomen structura Unitates Unitates;

/* Politicam (unitates.stml in radice) et declarationes omnes legere.
 * NIHIL = RECUSATIO: *causa_out causas OMNES nominat (linea una per
 * causam, 'via:linea: ...') et *recusationes_out (NIHIL licet)
 * easdem structas (Xar de UnitatesRecusatio). */
Unitates*
unitates_legere (
     constans UnitatesSutura*  sutura,
                     Piscina*  piscina,
                      chorda*  causa_out,
                         Xar** recusationes_out);

/* unitates omnes (Xar de Unitas*, ordine: directorii ordine viarum,
 * deinde bibliothecae ordine stemmatum) - pro auditibus et 'quid'
 * sine via */
constans Xar*
unitates_omnes (
    constans Unitates* unitates);

/* Quid est via (radici relativa)? Via non enumerata quoque
 * respondetur (regulae viarum), provenientia tum per suturam
 * legitur. Semper VERUM nisi via vacua aut absoluta. */
b32
unitates_quid (
             constans Unitates* unitates,
                        chorda  via,
                       Piscina* piscina,
             UnitatisResponsum* responsum);

nomen enumeratio {
    UNITATES_IUDICAT = ZEPHYRUM,
    UNITATES_NON_IUDICAT,
    UNITATES_IUDEX_IGNOTUS        /* titulus non in politica */
} UnitatesIudicium;

/* Iudicatne iudex 'iudex' viam? Plagula iudicatur sola si cura,
 * vita, munus, provenientia, lingua OMNES in indicibus iudicis sunt.
 * Iudex qui provenientias omnes admittit plagulam NON legit (via
 * illegibilis tum iudicatur - provenientia nihil decernit).
 * *causa_out (NIHIL licet): NON_IUDICAT -> proprietas prima quae
 * excludit cum causa sua ('cura aliena (vendor/unitas.stml) non in
 * curae iudicis lint_latinus'); IUDEX_IGNOTUS -> iudices noti. */
UnitatesIudicium
unitates_iudicat (
    constans Unitates* unitates,
               chorda  iudex,
               chorda  via,
              Piscina* piscina,
               chorda* causa_out);

/* Lista viarum (lineis divisa, ut corpus_tractatae.txt) -> eae solae
 * quas iudex iudicat, eadem forma (linea una per viam, '\n' finalis).
 * Consumptores qui viarum indicem filtrant (oratio vocabula: index
 * symbolorum et pinna portae) responsum UNUM communicant. *ignotus
 * (NIHIL licet) = VERUM si iudex ignotus (lista vacua tum redditur). */
chorda
unitates_viae_iudicatae (
    constans Unitates* unitates,
               chorda  iudex,
               chorda  lista,
              Piscina* piscina,
                  b32* ignotus);


/* ==================================================
 * CENSUS (U5a): quid unitates habent, quid eis deest, quae nusquam
 * cadunt - relatio, non porta (spec par. VI septem-octavae m)
 * ================================================== */

nomen structura {
    chorda clavis;      /* munus aut lingua */
       i32 numerus;
} UnitatesNumerus;

nomen structura {
    constans Unitas* unitas;
                Xar* numeri;   /* UnitatesNumerus per munus, ordine
                                * primi visi */
                Xar* desunt;   /* chorda: descriptio exspectationis
                                * non impletae (unitates archivatae
                                * numquam) */
} UnitatisCensus;

nomen structura {
    chorda  via;                /* directorium */
       Xar* numeri;            /* UnitatesNumerus per linguam */
} UnitatesDirectorium;

nomen structura {
    Xar* unitates;     /* UnitatisCensus, ordine unitates_omnes */
    Xar* orphani;      /* chorda: probationes radicis (PROBATIONES/
                        * PRAEFIXUM*.c) quae nullam unitatem per
                        * stemma aut titulum inveniunt */
    Xar* indeclarata;  /* UnitatesDirectorium: directoria cum fontibus,
                        * capitibus, scriptis aut probationibus quae
                        * radici cadunt (nulla declaratio) - capita,
                        * fontes, probationes bibliothecarum exclusa */
} UnitatesCensus;

/* Census plagularum enumeratarum (eaedem ac unitates_legere legit):
 * quaeque plagula ad unitatem intimam cum munere numeratur, deinde
 * exspectationes politicae (<exspectatio>) per genus iudicantur. Nihil
 * legitur (provenientia non poscitur). */
UnitatesCensus*
unitates_census (
    constans Unitates* unitates,
              Piscina* piscina);


/* ==================================================
 * INTERROGATIONES (U5b): documentatio unitatis ut quaestiones typatae
 * quas respondet (spec par. VII; canon interrogationes.canon).
 * Plagulae = viae enumeratae quibus munus 'interrogationes' est
 * (politica: extensio .interrogationes.stml); possessor = unitas
 * plagulae (lib/xar.interrogationes.stml -> stemma xar)
 * ================================================== */

nomen structura {
    constans Unitas* unitas;     /* possessor plagulae */
             chorda  via;
                i32  linea;      /* interrogatio (linea elementi) */
             chorda  genus;      /* quomodo possumne cur quid ... */
             chorda  quaestio;   /* lectio FLUXUS (stml_textus_fluxus):
                                  * formatione stabilis */
             chorda  responsum;  /* idem */
                Xar* symbola;    /* chorda; vacuus licet */
                Xar* viae;       /* chorda; vacuus licet */
             chorda  imperium;   /* vacuum licet */
} UnitatisInterrogatio;

nomen structura {
    Xar* interrogationes;  /* UnitatisInterrogatio, ordine viarum et
                            * documenti */
    Xar* recusationes;     /* UnitatesRecusatio: plagula illegibilis,
                            * radix aliena, 'unitas=' non possessor,
                            * elementum aut attributum ignotum,
                            * quaestio/responsum absens aut bis aut
                            * vacuum, responsum sine ancora. Vacuus =
                            * omnia sana. Interrogatio cum quaestione
                            * et responso tamen redditur */
} UnitatesInterrogationes;

/* Interrogationes omnes arboris per suturam legere. Numquam NIHIL:
 * vitia per plagulam nominantur, ceterae plagulae leguntur (lectio
 * pigra - unitates_legere eas non legit). Ancorae hic NON iudicantur
 * (symbolum exstatne?) - porta interrogationum (U5b gradus 3). */
UnitatesInterrogationes*
unitates_interrogationes (
    constans Unitates* unitates,
              Piscina* piscina);

#endif /* UNITATES_H */
