/* silva_registrum.h - Registrum generum coctum (vocabularium nodorum)
 *
 * SCISSUM ex silva_tabulae.h (M4, project-specs/materia-spec.md par. III).
 *
 * silva_tabulae.h duo capita sub uno nomine ferebat: typos tabularum
 * LR (actiones, goto, status, symbola, productiones - opus MOTORIS
 * parsandi) et typos registri (genera, loci - VOCABULARIUM nodorum,
 * quod motorem parsandi omnino nescit).
 *
 * Distinctio non aesthetica est sed STRATIFICATIO. Mensuratum
 * 2026-08-27 super omnes XXXIII plagulas silva_tabulae.h
 * includentes:
 *
 *   arbor.h XI registri / ZERO LR      quaestio.h III / ZERO
 *   scribere.h III / ZERO              quaestiones.h II / ZERO
 *   commissio.h I / ZERO               glr.h ZERO / V
 *
 * Nucleus substrati registro SOLO utitur; motor LR tabulis SOLIS.
 * Nulla plagula utrumque latus miscet praeter parsare.h et tres
 * tabulas GENERATAS - id est, scissura per limitem strati currit,
 * non per medium consuetudinis.
 *
 * HOC CAPUT NIHIL INCLUDIT PRAETER latina.h. Nec silva_token.h
 * (nullus typus registri genus lexematis fert - solum
 * SilvaTabSymbolum latere LR id facit), nec quicquam aliud. Haec
 * pauperies contractus est, non casus: quicumque vocabularium
 * nodorum legere vult motorem parsandi trahere NON debet.
 */

#ifndef SILVA_REGISTRUM_H
#define SILVA_REGISTRUM_H

#include "latina.h"
#include "materia_registrum.h"


/* ==================================================
 * Registrum generum coctum (layouts nodorum, S21/S20)
 *
 * FACIES MATERIAE (silva-migratio T9, 2026-09-25): typi registri
 * silvae SUNT typi materiae - campi idem nominibus, ordine, typis
 * (materia ex hoc capite portata est). Scriptor materiae registrum
 * silvae sine conversione accipit. materia_registrum.h quoque
 * latina.h SOLUM includit - pauperies supra servata.
 * ================================================== */

nomen MateriaTabLocus         SilvaTabLocus;
nomen MateriaTabGenus         SilvaTabGenus;
nomen MateriaRegistrumCoctum  SilvaRegistrumCoctum;

#endif /* SILVA_REGISTRUM_H */
