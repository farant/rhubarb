/* postulata_atomica.h - postulatum compilatoris: atomica __atomic
 *
 * SUTURA praeprocessoris pura, exemplar postulata_posix.h. ISO C89
 * exemplar memoriae NON habet; anulus SPSC (circulus) inter fila duo
 * duas operationes ordinatas solas eget - publicare (store-release) et
 * legere (load-acquire) - quas clang et gcc per __atomic_store_n /
 * __atomic_load_n praebent (sub -std=c89 -pedantic -Werror probatum,
 * arm64 stlr / ldapr; sonus spec X.7). Volatile NON sufficit: Apple
 * Silicon ordinem debilem habet, index ante slotum visibilis fieri
 * potest.
 *
 * LEX: plagula quae __atomic utitur hoc caput DIRECTE includat (non
 * per caput aliud) - silva sectionem lexici 'postulata_atomica.h' ex
 * inclusionibus plagulae IPSIUS derivat, ergo superficiem atomicam
 * SOLA haec plagula videt.
 *
 * SIGNUM PURUM, sine '#error': compilator sine __atomic in loco usus
 * CLAMAT (__ATOMIC_ACQUIRE identificator ignotus; functio non
 * declarata sub -Wall -Werror), numquam tacet - C89 viam rectam
 * alteram non habet, et via 'volatile' in ARM tacite falleret. Ramus
 * '#error' hic silvam fallebat: caput SOLUM iudicatum se ipsum non
 * includit, sectio non evocatur, ramus activus videtur (2026-10-21).
 */

#ifndef POSTULATA_ATOMICA_H
#define POSTULATA_ATOMICA_H

#endif /* POSTULATA_ATOMICA_H */
