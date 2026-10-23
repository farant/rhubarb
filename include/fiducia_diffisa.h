/* fiducia_diffisa.h - indicium certificatorum diffisorum domus
 * (fiducia-linux-spec II.1, II.4)
 *
 * Tabula COMPILATA (fiducia_diffisa_tabula.h -> lib/fiducia_diffisa_tabula.c,
 * GENERATUM ex instantanea
 * OneCRL fixa et indice manu scripto): fiducia_iudicare in OMNI
 * platforma eam ANTE iudicium systematis consulit, ut certificatum
 * diffisum causam suam nominatam ferat. Duo genera, semantica Mozillae
 * ipsa: emittens + series (certificatum unum) et subiectum + SHA-256
 * SubjectPublicKeyInfo (omne certificatum clavis eius CA, transsignata
 * comprehensa). Recentia tabulae NUNTIATUR (fiducia_diffisae_tempus),
 * numquam portam frangit - renovatio est commissio consulta. */
#ifndef FIDUCIA_DIFFISA_H
#define FIDUCIA_DIFFISA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fiducia.h"

/* <aedilis corpus="lib/fiducia_diffisa.c"/> */

nomen enumeratio {
    FIDUCIA_DIFFISA_EMITTENS_SERIES = I,  /* emittens DER + series */
    FIDUCIA_DIFFISA_SUBIECTUM_CLAVIS      /* subiectum DER + SHA-256
                                           * SPKI (XXXII octeti) */
} FiduciaDiffisaGenus;

nomen structura {
    FiduciaDiffisaGenus  genus;
            constans i8* primum;              /* emittens aut subiectum
                                               * (Name DER) */
                   i32  primum_mensura;
           constans i8* secundum;            /* series aut SHA-256 */
                   i32  secundum_mensura;
    constans character* causa;               /* "bug 2083382 (OneCRL)" */
} FiduciaDiffisa;

/* index primae diffisae in tabula congruentis, aut -1; *positio =
 * locus certificati in catena. Certificatum non DER praeteritur
 * (iudicium systematis id nominat) */
s32
fiducia_diffisam_invenire (
     constans FiduciaCertificatum* catena,
                              i32  numerus,
          constans FiduciaDiffisa* tabula,
                              i32  tabula_numerus,
                              i32* positio);

/* VERUM si nulla diffisa; FALSUM + *causa "certificatum reiectum:
 * clavis diffisa (catena [k]): <causa>". tabula NIHIL = ordinaria -
 * probationes tabulam suam dant */
b32
fiducia_diffisas_iudicare (
     constans FiduciaCertificatum* catena,
                              i32  numerus,
          constans FiduciaDiffisa* tabula,
                              i32  tabula_numerus,
                           chorda* causa,
                          Piscina* piscina);

#endif /* FIDUCIA_DIFFISA_H */
