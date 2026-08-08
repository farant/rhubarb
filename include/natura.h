#ifndef NATURA_H
#define NATURA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "tabula_dispersa.h"
#include "stml.h"

/* ==================================================
 * natura - onerator bibliothecae semanticae
 *
 * Plagulae STML (natura/NOMEN.stml) VERITAS manent; graphus oneratus
 * PROIECTIO est. Onerator legit et indicat, numquam rescribit.
 *
 * Lectio duabus phasibus (compilare, deinde religare):
 *   I.  natura_legere   - exemplar unum parsare et entia indicare
 *   II. natura_nectere  - bibliothecam totam religare: parentes,
 *       relationes, proprietates trans-exemplares resolvere, et
 *       regulas contractus (METAMODULUS par.8) cogere
 *
 * Regulae IX-XIII contractus hic demum cogi possunt: resolutio
 * hereditatis generum, quae onerator solus praestat.
 *
 * Profunditas HYBRIDA: structurae typatae pro sceleto solo
 * (bibliotheca / exemplar / genus / res); cetera per StmlNodus
 * suum leguntur. Formatum crescere potest sine structuris C
 * commutandis.
 * ================================================== */

/* ==================================================
 * Diagnostica
 * ================================================== */

nomen enumeratio {
    NATURA_GRADUS_VULNUS  = I,   /* contractum violat - exitus 1 */
    NATURA_GRADUS_MONITUM = II   /* advisorium */
} NaturaDiagnosticumGradus;

nomen structura {
    NaturaDiagnosticumGradus  gradus;
                         i32  regula;    /* numerus regulae par.8 */
                      chorda* modulus;   /* ubi (exemplar) */
                      chorda* ens;       /* quod ens (potest NIHIL) */
          constans character* nuntius;   /* quid violatum */
} NaturaDiagnosticum;

/* ==================================================
 * Sceletum
 * ================================================== */

nomen structura NaturaGenus NaturaGenus;
structura NaturaGenus {
         chorda*  titulus;    /* attributum nomen= (internatum) */
         chorda*  modulus;    /* exemplar domesticum */
    NaturaGenus*  parens;     /* sub= resolutum (trans-exemplar),
                               * vel genus ambiens si nidificatum */
            Xar*  liberi;     /* index reversus: Xar de NaturaGenus* */
            Xar*  res_suae;   /* Xar de NaturaRes* (species/individua) */
      StmlNodus*  nodus;      /* subtree - cetera hinc leguntur */
};

/* res dictionarii: species / individuum / cultivar
 *
 * DUAE CATENAE, CONSULTO DISTINCTAE (2026-08-08):
 *   genus_suum + genus->res_suae = catena APPARATUS. Genus
 *     ambiens, res mediis PRAETERMISSIS: species apparatum non
 *     confert (valorem solum figit), ergo hereditas eam recte
 *     transilit. Planum manet: genus->res_suae res OMNES sub eo
 *     fert, etiam nidificatas.
 *   continens + res_suae = catena TAXINOMIAE. Quod re vera quid
 *     continet: <species isbn> speciem isbn_10 continet,
 *     <species malus_domestica> cultivar granny_smith. Haec
 *     profunditas Porphyriana in plagulis SEMPER fuit; hic
 *     demum in graphum venit (ante hoc dici non poterat).
 * Neutra alteram substituit - apparatus quaerit quid HEREDITET,
 * taxinomia quid SIT. */
nomen structura NaturaRes NaturaRes;
structura NaturaRes {
         chorda*  titulus;
         chorda*  modulus;
    NaturaGenus*  genus_suum;   /* genus continens (APPARATUS) */
    NaturaGenus*  genus_etiam;  /* etiam= resolutum: membrum
                                 * essentiale duplex (par.3/4).
                                 * Apparatus AMBARUM catenarum
                                 * huic rei debetur */
      NaturaRes*  continens;    /* res ambiens si nidificata,
                                 * NIHIL si sub genere recta
                                 * (TAXINOMIA) */
            Xar*  res_suae;     /* Xar de NaturaRes*: res HUIC
                                 * rei proxime nidificatae */
      StmlNodus*  nodus;
};

/* discrimen entis - genera et res dictionarii spatium nominum
 * UNUM per exemplar habent (regula XV), ergo index unus */
nomen enumeratio {
    NATURA_ENS_GENUS = I,
    NATURA_ENS_RES   = II
} NaturaEnsDiscrimen;

nomen structura {
    NaturaEnsDiscrimen  discrimen;
               vacuum*  corpus;   /* NaturaGenus* aut NaturaRes* */
} NaturaEns;

nomen structura {
              chorda*  stirps;         /* modulus (= nomen plagulae) */
              chorda*  versio;         /* attributum versio= */
           StmlNodus*  radix;          /* elementum <natura> */
} NaturaExemplar;

nomen structura {
                Piscina*  piscina;
    InternamentumChorda*  intern;
                    Xar*  exemplaria;    /* Xar de NaturaExemplar* */
                    Xar*  genera_omnia;  /* Xar de NaturaGenus* */
                    Xar*  res_omnes;     /* Xar de NaturaRes* */
         TabulaDispersa*  entia;         /* "modulus/nomen" -> NaturaEns* */
         TabulaDispersa*  nomina;        /* nomen nudum -> NaturaEns*
                                          * (homonyma -> sentinella,
                                          * quaestio NIHIL reddit) */
                    Xar*  diagnostica;   /* Xar de NaturaDiagnosticum */
                     b32  nexum;         /* nectere iam factum? */
} NaturaBibliotheca;

/* ==================================================
 * Creatio
 * ================================================== */

NaturaBibliotheca*
natura_bibliotheca_creare(
    Piscina* piscina);

/* ==================================================
 * Phasis I - legere (exemplar unum)
 *
 * fons = contentum plagulae; stirps = nomen plagulae sine
 * extensione ("mensura") - regula XIV eam contra attributum
 * modulus iudicat. I/O extra bibliothecam manet: vocator plagulas
 * legit (linea platformae).
 *
 * Redde FALSUM solum si parsatio ipsa fracta (regula I);
 * violationes ceterae diagnostica fiunt, lectio pergit.
 * ================================================== */

b32
natura_legere(
    NaturaBibliotheca*   bib,
    chorda               fons,
    constans character*  stirps);

/* ==================================================
 * Phasis II - nectere (bibliotheca tota)
 *
 * Parentes resolvit (sub= trans-exemplar, nidificatio),
 * indicem reversum aedificat, regulas cogit:
 *   [E] II III IV V VI VII VIII XIV XV XVI (structurales)
 *   [S->E] IX X XI XII XIII (hereditatis - ratio oneratoris)
 * Redde numerum VULNERUM (ZEPHYRUM = sanum).
 * ================================================== */

i32
natura_nectere(
    NaturaBibliotheca* bib);

/* ==================================================
 * Quaestiones (superficies Steenbergiana)
 * ================================================== */

/* nomine nudo - NIHIL si absens AUT homonymum (tunc
 * natura_genus_in adhibendum: addressa /modulus/nomen) */
NaturaGenus*
natura_genus(
    NaturaBibliotheca*   bib,
    constans character*  titulus);

NaturaGenus*
natura_genus_in(
    NaturaBibliotheca*   bib,
    constans character*  modulus,
    constans character*  titulus);

/* ens quodlibet (genus aut res dictionarii) per addressam */
NaturaEns*
natura_ens_in(
    NaturaBibliotheca*   bib,
    constans character*  modulus,
    constans character*  titulus);

/* catena parentum, a proximo ad radicem
 * Redde: Xar de NaturaGenus* */
Xar*
natura_maiores(
    NaturaGenus* genus,
    Piscina*     piscina);

/* ==================================================
 * Apparatus - consumptor primus
 *
 * Proprietates / partes / machinae statuum / actiones /
 * relationes cum hereditate: primum quae generis ipsius, deinde
 * maiorum ordine ascendente. Quodque membrum auctorem suum fert
 * (quis maiorum id contulit). Iterationes NON deduplicantur -
 * angustatio hereditata (versio_de) utramque stare vult.
 * ================================================== */

nomen structura {
      StmlNodus*  nodus;    /* proprietas/pars/machina_statuum/
                             * actio/relatio */
    NaturaGenus*  auctor;   /* genus quod contulit */
} NaturaApparatusMembrum;

/* Redde: Xar de NaturaApparatusMembrum */
Xar*
natura_apparatus(
    NaturaBibliotheca*  bib,
    NaturaGenus*        genus,
    Piscina*            piscina);

#endif /* NATURA_H */
