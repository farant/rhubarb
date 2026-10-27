/* concentus.h - Hospes soni communis: mixtura UNA, rector UNUS, exitus
 * UNUS pro omnibus clientibus (tintinnabulum, aetherophonum, ...)
 *
 * UNUS exitus per processum (auditus.h): clientes ergo non mixturas
 * suas habent sed sedes in mixtura communi capiunt.
 *
 *   EXITUS PIGER - concentus_aperire primum aperit; post
 *     CONCENTUS_QUIES ms sine voce viva clauditur (unitas exitus aperta
 *     reditum semper vocat - pila - etiam tacens).
 *   QUIESCERE - mutus factus: clauditur cum primum nulla vox vivit,
 *     post evanescentiam (sine crepitu), non post XXX s; vox alterius
 *     clientis viva exitum apertum tenet.
 *   TENERE - cliens qui notas FUTURAS habet (cantor sonans) exitum
 *     tenet: concentus_tenere(c, VERUM) ... (c, FALSUM), numerus
 *     tenentium; dum tenetur quies XXX s non numeratur (pausa longa in
 *     carmine, pars sola sero intrans). QUIESCERE (mutus) tenentes non
 *     curat.
 *   NUNTII - concentus SOLUS legit (agere) et rectori tradit; cliens
 *     vocem suam per concentus_vox_vivit interrogat (sine reditibus).
 *   PRIORITAS per sedem (mixtura_rector_capere): vox altior
 *     (aetherophonum) a cantilena non surripitur.
 *
 * Exitus per tabulam functionum, ut concentus auditum non noverit:
 * hospes auditus.h adaptat (concentus_auditus.h); probationes mixturam
 * offline cum horologio manuali dant - eadem logica, sine sono.
 *
 * TEMPUS duplex: quadra exitus (momentum, iussa) et ms hospitis (nunc:
 * quies) - exitu clauso quadra non procedunt.
 *
 * USUS:
 *   c = concentus_creare(piscina, &forma, &exitus);
 *   si (concentus_aperire(c)) {
 *       a = mixtura_rector_capere(concentus_rector(c), prioritas);
 *       i.momentum = concentus_initium(c); i.ansa = a; ...
 *       (vacuum)concentus_iubere(c, &i);
 *   }
 *   (vacuum)concentus_agere(c, nunc);           (* omni pulsu *)
 *   si (!concentus_vox_vivit(c, a)) ...         (* finita *)
 */
/* <aedilis corpus="lib/concentus.c"/> */
#ifndef CONCENTUS_H
#define CONCENTUS_H

#include "latina.h"
#include "piscina.h"
#include "mixtura.h"

/* quies ante exitum clausum, ms */
#define CONCENTUS_QUIES (XXX * M)

nomen structura {
      b32 (*aperire)        (vacuum* ctx, Mixtura* mixtura);
    vacuum (*claudere)      (vacuum* ctx);
      s64 (*momentum)       (vacuum* ctx);    /* quadra reddita */
      i32 (*mora)           (vacuum* ctx);    /* latentia, quadra */
      b32 (*iubere)         (vacuum* ctx,
          constans MixturaIussum* iussum);
      b32 (*nuntium_legere) (vacuum* ctx, MixturaNuntius* nuntius);
    vacuum* ctx;
} ConcentusExitus;

nomen structura Concentus Concentus;

/* mixtura et rector e piscina; exitus vivat dum concentus. Exitus NON
 * aperitur. NIHIL: forma invalida, forma.nuntii < II x voces (nuntius
 * amissus = sedes numquam libera), aut memoria deest */
Concentus*
concentus_creare (
                     Piscina* piscina,
       constans MixturaForma* forma,
    constans ConcentusExitus* exitus);

/* exitum aperit si clausus; FALSUM: exitus recusat (sine sono - hospes
 * pergit). Quiescere prius petitum abicitur */
b32
concentus_aperire (
    Concentus* c);

/* sedes hic capiuntur (mixtura_rector_capere, cantus_iussa_parare) */
MixturaRector*
concentus_rector (
    Concentus* c);

/* quadrum primum quo iussum nunc missum certe agit: momentum + mora +
 * margo; 0 si clausus */
s64
concentus_initium (
    Concentus* c);

/* horologium exitus: quadra reddita (momentum); 0 si clausus */
s64
concentus_momentum (
    Concentus* c);

/* iussum ad exitum. FALSUM: clausus aut anulus plenus - iussum
 * INCIPIENS (tabula, sonitus, tibia) recusatum sedem suam liberat, ne
 * occupata maneat sine voce */
b32
concentus_iubere (
                   Concentus* c,
      constans MixturaIussum* iussum);

/* omni pulsu hospitis: nuntii -> rector; quies >= CONCENTUS_QUIES aut
 * quiescere petitum -> exitus clauditur. VERUM dum vox aliqua vivit
 * (hospes pulsum celerem servet) */
b32
concentus_agere (
    Concentus* c,
          s64  nunc);   /* ms hospitis */

/* sedes ansae occupata generatione eadem (nondum VOX_FINITA lecta) */
b32
concentus_vox_vivit (
    constans Concentus* c,
           MixturaAnsa  ansa);

/* clauditur cum primum nulla vox vivit (agere proximo, etiam statim si
 * iam nulla) */
vacuum
concentus_quiescere (
    Concentus* c);

/* tenens unus addit (VERUM) aut demit (FALSUM; numquam infra 0) -
 * dum tenentes > 0 quies non numeratur */
vacuum
concentus_tenere (
    Concentus* c,
          b32  tenere);

b32
concentus_apertum (
    constans Concentus* c);

/* exitum statim claudit (si apertus); c deinde non adhibendum */
vacuum
concentus_destruere (
    Concentus* c);

#endif /* CONCENTUS_H */
