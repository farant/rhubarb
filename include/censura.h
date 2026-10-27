/* censura.h - Carmen iudicatum: regulae musicae ut armamentarium
 *
 * Agens non audit: censura invenit quod auris inveniret. Regulae
 * NOMINATAE, singulae accenduntur et exstinguuntur (Franus 2026-10-22:
 * armamentarium cum forma ordinaria); inventa MONITA sunt, non vitia
 * lectoris - carmen etiam cum inventis sonat.
 *
 * REGULAE (titulus; ORD. = in forma ordinaria):
 *   lineae       '|' scripta non in linea mensurae (mappa metri)  ORD.
 *   clavis       nota extra clavem; in minore VI et VII elevati
 *                licent (harmonica, melodica); nota acordi
 *                declarati ad initium suum licet (dominans
 *                applicata scripta: D7 in F, F#)                  ORD.
 *   ambitus      nota extra ambitum instrumenti (ordinarius:
 *                aether 36..96, quadrans 48..96, triangulum
 *                24..72, chorda 28..88, campana 48..96; ignotum
 *                tacet)                                           ORD.
 *   dissonantia  secunda, septima, tritonus (etiam compositi)
 *                inter notas partium diversarum sonantes ad
 *                tempus forte, NISI explicata (explicatio.h): nota
 *                par aliena explicata (transitus, vicinus, innixa,
 *                anticipatio, effugiens), suspensio / retardatio /
 *                septima resoluta, aut ambae acordi nominati (G#dim
 *                tritonus suus). Causa nominatur: nota sine
 *                explicatione, suspensio / retardatio / septima non
 *                resoluta, acordum sine nomine                    ORD.
 *   parallelae   quintae aut octavae parallelae inter duas partes:
 *                initia sequentia, ambae motae, eodem sensu       -
 *   saltus       saltus melodicus > saltus_maximus semitonia (XII
 *                ordinarie) inter notas sequentes partis - signum
 *                octavae ('<' '>') oblitum fere; bassi octavis
 *                alternantibus saltus veros habent (Korobeiniki:
 *                VII, ad XIX), ergo non ORD.                      -
 *   tertia       ad tempus forte >= III notae sonantes (partes
 *                omnes) quarum classes acordum sine tertia faciunt
 *                (acordum_sine_tertia: D3 A4 D5, A2 G4 E5); semel
 *                per sonum - nota aliqua post tempus forte prius
 *                incipiat (acordum sustentum non iteratur); NON si
 *                fenestra explicationis cui tempus pertinet acordum
 *                cum tertia nominat (arpeggium E2 E3 B3, G4: Em)  -
 *   acorda       harmonia declarata (<acorda>) et sonans discordant
 *                (fenestra declarata, explicatio.h): nota sine forma
 *                alienae extra acordum declaratum, aut radix /
 *                tertia declarata nusquam in fenestra sonans. Semel
 *                per fenestram: "D declaratum, sonat Gmaj9/D (G5
 *                B5)"                                             ORD.
 *
 * STYLUS (censura_carminis_forma, carmen.h <stylus>): forma ordinaria
 * (classicus), deinde stylus carminis per catenam basium (basis prior:
 * regulae, septimae), deinde <censura regulae> carminis; vocans
 * (auditio -regulae) postremo (censura_regulas_legere). REGULAE:
 * "+a,-b" formam datam mutat; "a,b" (sine signis) hae solae.
 * septimae_liberae: septima acordi nominati (et nona, sexta - toni
 * acordi) resolutionem non postulat (dissonantia: causa 'septima non
 * resoluta' tacet); dissonantia notarum alienarum manet.
 *
 * TEMPUS FORTE: initium mensurae; in 4/x etiam medium; in metris
 * compositis (6/8, 9/8, 12/8) omnis tertia octava. Percussio ('@')
 * regulas notarum non subit.
 *
 * ORDO: inventa ordine (mensura, pulsus, regula). Plura quam capacitas:
 * numerus TOTUS redditur, priora inventa (ordine regularum) servantur.
 *
 * USUS:
 *   CensuraForma     f;
 *   CensuraInventum  inv[LXIV];
 *   censura_ordinaria(&f);
 *   f.activae[CENSURA_DISSONANTIA] = VERUM;
 *   n = censura_iudicare(carmen, &f, inv, LXIV);
 *   per (i = 0; i < n && i < LXIV; i++)
 *       (vacuum)censura_inventum_scribere(carmen, &inv[i], a);
 */
/* <aedilis corpus="lib/censura.c"/> */
#ifndef CENSURA_H
#define CENSURA_H

#include "latina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "carmen.h"

nomen enumeratio {
    CENSURA_LINEAE,
    CENSURA_CLAVIS,
    CENSURA_AMBITUS,
    CENSURA_DISSONANTIA,
    CENSURA_PARALLELAE,
    CENSURA_SALTUS,
    CENSURA_TERTIA,
    CENSURA_ACORDA,
    CENSURA_REGULAE
} CensuraRegula;

nomen structura {
    b32 activae[CENSURA_REGULAE];
    /* ambitus instrumenti (MIDI, inclusivus); NIHIL = ordinarius
     * (censura_ambitus_ordinarius); FALSUM = ignotum, regula tacet */
    b32 (*ambitus)(vacuum* ctx, chorda instrumentum, i32* infima,
                   i32* summa);
    vacuum* ctx;
    /* saltus maximus licitus, semitonia; 0 = XII (octava) */
    i32 saltus_maximus;
    /* septima acordi resolutionem non postulat (stylus popularis) */
    b32 septimae_liberae;
} CensuraForma;

nomen structura {
          CensuraRegula regula;
                 chorda pars;
                 chorda pars_altera;   /* dissonantia, parallelae */
                    i32 mensura;       /* 1 = prima */
                    s64 pulsus;        /* intra mensuram */
                    s32 frustum;       /* index in parte; -1 = nullum */
                    i32 offset;        /* in textu frusti (nota aut
                                         * '|') */
    constans CarmenNota* nota;          /* in carmine (tertia: infima
                                         * sonantium); NIHIL: lineae */
    constans CarmenNota* nota_altera;   /* partis alterius */
    /* parallelae: notae initii prioris (par primum); saltus: nota
     * prior partis; acorda: nota aliena altera; NIHIL aliter */
    constans CarmenNota* nota_prior;
    constans CarmenNota* nota_altera_prior;
    /* dissonantia: cur non explicata (statica); acorda: "radix
     * deest" / "tertia deest" (NIHIL: notae alienae); NIHIL aliter */
    constans character* causa;
    /* acorda: harmonia SONANS fenestrae (litteralis, classes omnes) */
                Acordum sonans;
} CensuraInventum;

/* ORD. activae (lineae, clavis, ambitus, dissonantia), ambitus
 * ordinarius */
vacuum
censura_ordinaria (
    CensuraForma* forma);

/* "lineae", "clavis" ...; NIHIL extra regulas */
constans character*
censura_titulus (
    CensuraRegula regula);

b32
censura_regulam_invenire (
           chorda  titulus,
    CensuraRegula* regula);

/* regulae "+a,-b" (formam mutat) aut "a,b" (hae solae). FALSUM:
 * regula ignota (*ignota = titulus eius, forma partim mutata) */
b32
censura_regulas_legere (
          chorda  regulae,
    CensuraForma* forma,
          chorda* ignota);

/* forma carminis: ordinaria, stylus (catena basium), <censura
 * regulae>. FALSUM: regula ignota in stylo aut carmine (*ignota) */
b32
censura_carminis_forma (
    constans Carmen* carmen,
       CensuraForma* forma,
             chorda* ignota);

/* tabula ordinaria; ctx non adhibetur */
b32
censura_ambitus_ordinarius (
     vacuum* ctx,
     chorda  instrumentum,
        i32* infima,
        i32* summa);

/* inventa in 'inventa' (capacitas maxima); numerus TOTUS redditur.
 * forma NIHIL = ordinaria */
i32
censura_iudicare (
           constans Carmen* carmen,
     constans CensuraForma* forma,
           CensuraInventum* inventa,
                       i32  capacitas);

/* linea una cum '\n':
 *   mensura 3 +480 melodia: G#4 extra clavem c maior (frustum 0,
 *   offset 41)
 *   mensura 14 melodia/bassus: G5 contra A2, septima - septima non
 *   resoluta (frustum 0, offset 61)
 * FALSUM si memoria deest */
b32
censura_inventum_scribere (
             constans Carmen* carmen,
    constans CensuraInventum* inventum,
           ChordaAedificator* textus);

#endif /* CENSURA_H */
