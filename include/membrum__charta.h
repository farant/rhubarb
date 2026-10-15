/* membrum__charta.h - CHARTA NOTARUM widget (nomenclator-plan U5; ex
 * calendario extractum): liber scribae nidificatus ("post-it") sub
 * ramo HOSPITIS, in loco eius cum umbra plana.
 *
 * Liber per CLAVEM: praefixum spatii in volumine ("dies/2026-10-15",
 * "persona/7"). Hospes clavem dat; charta librum vertit cum clavis
 * mutatur (gestus pendens prius effunditur). Area ad cellulas exacta:
 * (pagina + II) x (pagina + III) cellulae (margo cellulae utrimque,
 * linea status), intra locum minus marginem chartae et umbram (II);
 * pagina maior quam locus: area loco aptata (volvitur). Paginae novae
 * magnitudine areae loci dati (sine volutione).
 *
 * Charta in contextu privato hospitis habitat (registra scribae in
 * eam monstrant: structuram ne copia). */
#ifndef MEMBRUM__CHARTA_H
#define MEMBRUM__CHARTA_H

/* <aedilis corpus="lib/membrum__charta.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "volumen.h"
#include "scriba_applicatio.h"

#define CHARTA_MARGO          VIII   /* margo in loco (pixela) */
#define CHARTA_LITTERA        VI     /* cellula scribae (pixela) */
#define CHARTA_LINEA          VIII
#define CHARTA_CLAVIS_MAXIMA  LXIV

nomen structura {
         ScribaMontatio  scriba;
                Volumen* volumen;
                Piscina* piscina;
    InternamentumChorda* intern;
                    b32  montata;
              character  clavis[CHARTA_CLAVIS_MAXIMA];   /* currens */
} Charta;

/* montare: scriba nidificata <scriba id> sub ramo hospitis (id unicum
 * in volumine, canon nomen: "c1_notae"), liber clavis; registra
 * scribae in hospitis miscentur (spatio rami scribae). latitudo x
 * altitudo = locus exspectatus (pixela) - mensura paginarum novarum.
 * volumen NIHIL: VERUM, charta non montata (cetera nihil agunt). */
b32
charta_montare (
                  Charta* c,
                 Piscina* piscina,
     InternamentumChorda* intern,
                 Volumen* volumen,
    constans InsulaRamus* hospes,
          ActioRegistrum* actiones,
         FiguraRegistrum* figurae,
      constans character* id,
      constans character* clavis,
                     s32  latitudo,
                     s32  altitudo,
         StatusSystemata* systemata);

/* librum vertere si clavis mutata (motus non NIHIL: gestus pendens
 * prius effunditur in repo); latitudo x altitudo ut charta_montare.
 * VERUM si liber versus. */
b32
charta_clavem_ponere (
                 Charta* c,
                  Motus* motus,
     constans character* clavis,
                    s32  latitudo,
                    s32  altitudo);

/* area chartae in loco (pagina currens; non montata: locus vacuus) */
Fines
charta_area (
    constans Charta* c,
              Fines  locus);

/* scriba composita in area loci, spatio rami scribae signata, filius
 * radicis addita (nihil si non montata aut locus vacuus) */
vacuum
charta_componere (
                 Charta* c,
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
              Componens* radix,
                  Fines  locus);

/* umbra plana sub area (figura hospitis, ante scribam pictam) */
vacuum
charta_umbram_pingere (
    constans Charta* c,
            Mandata* md,
              Fines  locus);

/* focus in charta? (spatium intimum motus sub ramo scribae) - hospes
 * claves suas tunc non tractat (scribae et dispensatori relinquit) */
b32
charta_focata (
    constans Charta* c,
     constans Motus* motus);

/* pulsus vici: visus stalus reficitur (scriba_reficere); VERUM si
 * mutatum */
b32
charta_reficere (
    Charta* c);

/* gestus pendens scribae Motui addicitur (post reficere) */
vacuum
charta_gestum_ponere (
    Charta* c,
     Motus* motus);

#endif /* MEMBRUM__CHARTA_H */
