/* briar_silva.h - Genus interius quartum nexus: regiones C thistle
 * per silvam cum expansione. Unitas PROPRIA (non in briar_nexus.c):
 * caput amalgamae silvae enumerationes stml suas fert, ergo stml.h
 * et silva.h in eadem unitate translationis non coeunt - briar_nexus
 * stml tenet, briar_silva silvam; caput nexus tags solum praenuntiat.
 *
 * Praeludium textui regionis praepositum: '#include "latina.h"'
 * (+ '#include "internuntius.h"' + 'hic_manens InternuntiusTractator
 * briar_tractator_exemplar;' si methodus= - exemplar contra quod
 * fabrica signaturam methodi probat, silva_c89_typi_compatibiles).
 * Capita clausurae e FONTE silicis per textum praebita (numquam
 * discus). Expansio obligatoria: parsura nuda C domesticum male legit
 * ('principale' macro -> 'main' post expansionem solum).
 */

#ifndef BRIAR_SILVA_H
#define BRIAR_SILVA_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "silex.h"
#include "silva.h"
#include "briar_nexus.h"

nomen structura BriarSilva {
             SilvaPiscina* piscina;     /* arena silvae (solvere!) */
    constans SilvaParsura* parsura;     /* NIHIL si parsura fracta */
           SilvaSemantica* semantica;   /* symbola + typi */
} BriarSilva;

/* Regiones 'c' (quocumque munere) parsare. Reddit numerum regionum
 * parsatarum; -I = memoria aut argumenta (s32: sentinela signata).
 * Regio cum erroribus parsurae: linea_erroris (linea .thistle erroris
 * primi) + causa in BriarNexusRes; arbor tamen manet. */
s32
briar_silvam_texere (
                Piscina* piscina,
                    Xar* nexus,
     constans SilexFons* fons);

/* arenas silvae destruere (res->silva deinde NIHIL) */
vacuum
briar_silvam_solvere (
    Xar* nexus);

#endif /* BRIAR_SILVA_H */
