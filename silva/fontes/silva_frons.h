/* silva_frons.h - FRONS C89 super materiam: unci quibus scriptor et
 * lector arboris materiae lexemata SILVAE ipsa tractant (origo,
 * standard, scissurae in cauda silvae), sine conversione.
 *
 * Phasis V: T6b ex shim promotus (conversio), T10b unci super caudam
 * silvae, T10c conversio et shim recesserunt, T13a in amalgamate;
 * T14 sedes efficax = radix strati 0 (diagnostica C89).
 */

#ifndef SILVA_FRONS_H
#define SILVA_FRONS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_nodus.h"
#include "silva_expandere.h"
#include "silva_registrum.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_arbor.h"
#include "materia_diagnosticum.h"   /* forma sola, non machina (T15) */
#include "silva_parsare.h"

nomen structura SilvaFrons SilvaFrons;

/* Contextus novus pro plagula una. expansio NIHIL licet (subarbor
 * lecta, fons sine praeprocessore): extenta tunc ex extentis LECTIS
 * solis quaeruntur. */
SilvaFrons*
silva_frons_creare (
                   Piscina* piscina,
    constans SilvaExpansio* expansio);


/* CONSILIUM ARBORIS SUPER LEXEMATA SILVAE (T10b): sine conversione -
 * unci caudam silvae ipsam legunt (origo, standard, scissurae) et
 * lector lexemata forma C89 creat (vera SilvaToken). Extenta lecta
 * in frons vivunt (lectio subarboris sine expansione). FALSUM si
 * lexicon C89 recusatur. */
b32
silva_frons_arborem_silvae_parare (
                       SilvaFrons* frons,
    constans SilvaRegistrumCoctum* tabularium,
               constans character* grammatica,
              InternamentumChorda* intern,
            MateriaArborConsilium* consilium);

/* Extenta LECTA in hanc seriem registrari (Xar de
 * SilvaExtentumInvocationis). Lector parsurae (T10c) expansionem
 * NOVAM reficit et extenta in expansio->extenta ponere debet, ut
 * silva_scribere_fontem ea super parsuram lectam inveniat. */
vacuum
silva_frons_extenta_lecta_ponere (
    SilvaFrons* frons,
           Xar* extenta);

/* Longitudines post lectionem: lexema quodque (trivia, catenae
 * originis, extenta lecta) longitudinem = valor fert, sicut lector
 * silvae vetus. Lector parsurae (T10c) eam super laminas vocat. */
vacuum
silva_frons_longitudines_figere (
    SilvaFrons* frons,
    SilvaValor  valor);


/* Uncus originis C89 frontis (post silva_frons_arborem_silvae_parare):
 * sedes efficax (radix strati 0), radix emissionis, extentum. Pro
 * consumptoribus materiae praeter arborem - materia_diagnostica_
 * derivare (T14). Vita = frontis. */
constans MateriaOrigoUncus*
silva_frons_uncus (
    constans SilvaFrons* frons);

/* Mortes parsurae (T15) in diagnostica materiae, ad 'emissa'
 * materia_diagnostica_derivare: codex "error" (ut declaratio generis
 * - emissum declaratum eiusdem nodi SUPERAT), sedes per uncum.
 *   SYNTAXIS: primaria ad lexema mortis ('hic exspectatur'), relata
 *             ad lexema primum nodi ('hic coepit');
 *   LIMEN, INTERMISSIO: MONITUM ad nodum - apparatus, non fons.
 * Mors SYNTAXIS sine lexemate omittitur (declaratio supererit).
 * Xar de MateriaDiagnosticum (vacuum si nullae); NIHIL = memoria. */
Xar*
silva_mortes_diagnostica (
                       Piscina* piscina,
         constans SilvaParsura* parsura,
    constans MateriaOrigoUncus* uncus);

/* Linea fontis principalis -> linea NUNTIATA (briar: linea textus
 * silvae -> linea .thistle). datum = vocantis. */
nomen i32 (*SilvaLineaMappatio)(vacuum* datum, i32 linea);

/* Mortes syntaxis in TEXTUM scribere - UNA SEDES FORMAE (silva-
 * migratio T19b-1; desideratum ...W87Q): examen, legati, briar hanc
 * vocant, nemo formam copiat.
 *   humanus: 'via:linea:columna: [violatio] causa' + excerptum
 *            (relata 'hic coepit' + primaria 'hic exspectatur');
 *   machina: 'via<TAB>linea<TAB>columna<TAB>violatio<TAB>-1<TAB>0
 *            <TAB>causa' (forma examinis -machina).
 * Mors sine loco, in capite inclusa, aut apparatus -> summarium
 * 'nodi erroris (syntaxis) N' post ordines locatos. 'fons'/'mensura'
 * = textus fontis principalis (pro excerpto). 'mappatio' NIHIL =
 * identitas; lineae ordinum ET excerpti mappantur. *linea_prima
 * (NIHIL licet) = linea MAPPATA mortis locatae primae (0 si nulla -
 * briar eam pro linea erroris regionis ponit). Plagula sine errore
 * -> chorda vacua. Textus in piscina. */
chorda
silva_mortes_scribere (
                          Piscina* piscina,
               constans character* via,
            constans SilvaParsura* parsura,
    constans SilvaRegistrumCoctum* tabularium,
               constans character* fons,
                              i32  mensura,
                              b32  machina,
               SilvaLineaMappatio  mappatio,
                           vacuum* datum,
                              i32* linea_prima);

/* Causa fracturae ultimae (NIHIL si nulla) - additum ad API
 * probatum: shim nomen vitii lexici nuntiabat. */
constans character*
silva_frons_causa (
    constans SilvaFrons* frons);


#endif /* SILVA_FRONS_H */
