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
                               * vel genus ambiens si nidificatum
                               * (.species/.cultivar semper hoc) */
    NaturaGenus*  parens_etiam; /* etiam= resolutum: membrum
                               * essentiale duplex GENERUM (arbor
                               * porphyriana 2026-08-10 - doctrina
                               * a rebus portata; apparatus ambas
                               * catenas debet) */
            Xar*  liberi;     /* index reversus: Xar de NaturaGenus* */
            Xar*  res_suae;   /* Xar de NaturaRes* (individua sola
                               * post arborem porphyrianam) */
      StmlNodus*  nodus;      /* subtree - cetera hinc leguntur */
};

/* res = INDIVIDUA SOLA (arbor porphyriana 2026-08-10, decretum
 * 01KZPK4B7X - haec doctrina catenas veteres SUBSTITUIT):
 *
 *   Genus et species MUNERA relativa sunt, non naturae - ergo
 *   .genus/.species/.cultivar UNUM registrant (NaturaGenus supra)
 *   et profunditas Porphyriana catena UNA fertur: parens/liberi.
 *   Species apparatum CONFERT (proprietates suas aperit, valores
 *   determinat - differentia esse addit, non solum eligit);
 *   hereditas per parentelam fluit, nihil transilitur.
 *
 *   Rebus (individuis) catenae duae manent:
 *     genus_suum = catena APPARATUS (quid hereditet - genus aut
 *       species proxime continens, iam plene genus);
 *     continens + res_suae = catena TAXINOMIAE (quid re vera quid
 *       contineat - individuum intra individuum solum, quia
 *       nidificatio kind-in-kind nunc parentela est).
 *
 * Lectio trium stratorum, ubique eadem: proprietas APERIT -
 * valor in genere CLAUDIT (determinatio, domus proprii) - valor
 * in individuo REFERT. */
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

/* necessitudo - genus relationis PRIMAE CLASSIS (spec
 * necessitudines, 2026-08-10): identitas semel declarata quam
 * sedes usus (relatio/terminus) titulo suo, converso, aut
 * scriptura ligant. Tres strata: identitas / titulus / scriptura. */
nomen structura NaturaNecessitudo NaturaNecessitudo;
structura NaturaNecessitudo {
              chorda*  titulus;    /* nomen= (internatum) */
              chorda*  conversum;  /* conversum= aut NIHIL -
                                    * IDENTITAS UNA, scripturae duae */
              chorda*  modulus;
    NaturaNecessitudo*  parens;    /* sub= resolutum (nectere) */
           StmlNodus*  nodus;      /* a=/ad=/scriptiones hinc */
};

/* discrimen entis - genera, res dictionarii, et necessitudines
 * spatium nominum UNUM per exemplar habent (regula XV), ergo
 * index unus */
nomen enumeratio {
    NATURA_ENS_GENUS         = I,
    NATURA_ENS_RES           = II,
    NATURA_ENS_NECESSITUDO   = III
} NaturaEnsDiscrimen;

nomen structura {
    NaturaEnsDiscrimen  discrimen;
               vacuum*  corpus;   /* NaturaGenus* aut NaturaRes*
                                   * aut NaturaNecessitudo* */
} NaturaEns;

nomen structura {
              chorda*  stirps;         /* modulus (= nomen plagulae) */
              chorda*  versio;         /* attributum versio= */
           StmlNodus*  radix;          /* elementum <natura> */
} NaturaExemplar;

/* verdictum sedis ligatae contra fines familiae (regula XXII
 * in tabula servatum): APERTA = nihil probandum (sedes sine ad=
 * aut finis familiae ipse apertus aut scopus irresolutus) */
nomen enumeratio {
    NATURA_SEDES_APERTA   = ZEPHYRUM,
    NATURA_SEDES_INTRA    = I,
    NATURA_SEDES_EXCEDIT  = II
} NaturaSedesVerdictum;

/* sedes ligata - ordo tabulae quam nectere pro OMNI sede ligata
 * implet (fundamentum relationis '-sedes': quis familiae huic
 * dependet, qua directione, quo verdicto) */
nomen structura {
          NaturaExemplar*  exemplar;
             NaturaGenus*  possessor;  /* genus sedem ferens */
               StmlNodus*  nodus;      /* relatio aut terminus */
                  chorda*  titulus;    /* verbum sedis aut munus */
       NaturaNecessitudo*  ligata;
                      b32  conversa;   /* directio e nomine
                                        * efficaci (decretum
                                        * 2026-08-11) */
                  chorda*  ad_attr;    /* NIHIL si absens */
     NaturaSedesVerdictum  verdictum;
} NaturaSedesLigata;

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
                    Xar*  necessitudines_omnes;
                                         /* Xar de NaturaNecessitudo* */
         TabulaDispersa*  scriptiones;   /* scriptura quaevis (nomen/
                                          * conversum/scriptio) ->
                                          * NaturaNecessitudo*; nectere
                                          * aedificat, NIHIL ante */
                    Xar*  diagnostica;   /* Xar de NaturaDiagnosticum */
                    Xar*  sedes_ligatae; /* Xar de NaturaSedesLigata -
                                          * nectere implet */
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

/* finem familiae HYPOTHETICE superponere - inter legere et
 * nectere solum (post nexuram recusat). Familia NOMINE suo
 * (non converso); finis "a" aut "ad". Arbor in memoria sola
 * mutatur - plagulae intactae. Fundamentum '-angustatio':
 * regulae OMNES sub hypothesi currunt (XXII sedium, XXII
 * declarationum, XXIV), quod ambulatio manualis non praestat.
 * FALSUM = familia ignota aut iam nexum. */
b32
natura_finem_superponere(
    NaturaBibliotheca*   bib,
    constans character*  familia,
    constans character*  finis,
    constans character*  valor);

/* finis effectivus declarationis ("a" aut "ad"): proximus
 * non-'*' in catena sub= - NIHIL = apertus */
chorda*
natura_finem_effectivum(
    NaturaNecessitudo*   declarata,
    constans character*  finis);

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

/* necessitudo scriptura quavis (nomen / conversum / scriptio) -
 * NIHIL si absens aut ambigua. Ante nectere nomen primarium solum
 * resolvit. */
NaturaNecessitudo*
natura_necessitudo(
    NaturaBibliotheca*   bib,
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
