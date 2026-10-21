/* auditus.h - exitus soni: filum reale systematis reddit mixturam
 *
 * SUTURA SUGGESTUS (spec sonus §V): supra hanc nemo filum novit. macOS
 * (lib/auditus_macos.m): unitas exitus CoreAudio (AudioUnit, instrumentum
 * ordinarium), XLVIII milia f32 stereo intertextum; systema filum SUUM
 * reale possidet et reditum nostrum in eo vocat - nos filum numquam
 * creamus. Ceteri (lib/auditus_posix.c): aperire FALSUM reddit (sine
 * sono, numquam fractum) - variantes aedilis (macos posix).
 *
 * DUO ANULI (circulus.h): iussa principale -> sonus; nuntii sonus ->
 * principale. Reditus soni: iussa in mixturam (mixtura_iubere), deinde
 * mixtura_reddere in tabulam systematis, deinde nuntii mixturae in
 * anulum. In reditu nihil allocatur, nihil seratur, nihil scribitur
 * in plagulas.
 *
 * POSSESSIO: post auditus_aperire Mixtura FILI SONI est - filum
 * principale eam NUMQUAM tangit (nec iubere nec legere) donec
 * auditus_claudere redit. Pars principalis MixturaRector (sedes,
 * ansae) cum auditus_iubere / auditus_nuntium_legere utitur.
 *
 * TEMPUS: iussum momentum in quadris mixturae fert. Principale
 * 'nunc' per auditus_momentum legit et iubet ad momentum + mora +
 * margo (spec §VI-a); iussum tardum initio agminis proximi agit.
 *
 * UNUS exitus per processum (status in lib/auditus_macos.m, sicut
 * fenestra). Sine sono in instrumento (nullum, aut systema recusat):
 * aperire FALSUM - vocans sine sono pergit.
 *
 * USUS:
 *   AuditusForma forma = { CCLVI, CCLVI };
 *   si (auditus_aperire(m, &forma, piscina)) { ...
 *       iussum.momentum = auditus_momentum() + auditus_mora() + CDLXXX;
 *       (vacuum)auditus_iubere(&iussum);
 *       dum (auditus_nuntium_legere(&n)) mixtura_rector_tractare(r, &n);
 *   auditus_claudere(); }
 */
#ifndef AUDITUS_H
#define AUDITUS_H

#include "latina.h"
#include "piscina.h"
#include "mixtura.h"

/* capacitates anulorum (potentiae II); iussa >= iussa per ictum
 * vocantis, nuntii >= II x voces mixturae (spec: nuntius amissus =
 * sedes non liberata) */
nomen structura {
    i32 iussa;
    i32 nuntii;
} AuditusForma;

/* exitum aperit et sonare incipit (silentium usque ad iussa); anuli e
 * piscina allocantur. FALSUM: suggestus sine sono, instrumentum nullum,
 * forma invalida, aut iam apertum - mixtura tunc vocantis manet */
b32
auditus_aperire (
                  Mixtura* mixtura,
    constans AuditusForma* forma,
                  Piscina* piscina);

/* sonum sistit, filum soni exspectat donec reditus ultimus redit;
 * deinde mixtura iterum vocantis est. Tutum si non apertum */
vacuum
auditus_claudere (
    vacuum);

/* principale -> sonus; FALSUM si anulus plenus aut non apertum */
b32
auditus_iubere (
    constans MixturaIussum* iussum);

/* sonus -> principale; FALSUM si nullus */
b32
auditus_nuntium_legere (
    MixturaNuntius* nuntius);

/* quadra a filo soni reddita (horologium mixturae POST agmen
 * ultimum = initium agminis proximi). Filum soni XXXII bita publicat (atomus i32 solum; XXIV
 * horae ad XLVIII milia); pars principalis circumvolutiones in s64
 * extendit - vocari debet saepius quam semel in XII horis */
s64
auditus_momentum (
    vacuum);

/* aestimatio latentiae exitus in quadris: agmen systematis + latentia
 * instrumenti relata (CoreAudio). 0 si non apertum */
i32
auditus_mora (
    vacuum);

#endif /* AUDITUS_H */
