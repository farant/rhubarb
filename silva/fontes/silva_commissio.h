/* silva_commissio.h - Passus commissionis + oraculum (Phase 4 Chunk C)
 *
 * OMNIS semantica hic consolidatur (spec-v2 par 12, simulatio ⑤):
 * exhaustiones motoris GLR semanticae-vacuae manent; post acceptum
 * arbor UNA ambulatione committitur:
 *   (a) pater figitur (S27) - spina canonica per nodos AMBIGUOS,
 *   (b) prospectus listarum normalizantur (A½ - mensura exacta),
 *   (c) ambigua quae oraculum iam novit COLLABUNTUR (victor super
 *       allocationem copiatur - identitas manet) et eventum in
 *       diarium scribitur (COLLAPSE+JOURNAL, interview ⑤ Q5).
 *
 * POST commissionem arbor immutabilis est PRAETER
 * silva_recanonicare: scientia sera indicem canonicum in loco
 * vertit (spina retexta localiter) - involucrum manet, monstratores
 * observatorum valent, NULLA reparsura (par 8.2).
 *
 * Ambulatio grammaticae-ignara est: formam ambigui (genus +
 * interpretationes/canonica) ex registro cocto PER NOMEN invenit;
 * semantica sola in resolutore vocatoris vivit (manu scripto per
 * grammaticam - M1).
 */

#ifndef SILVA_COMMISSIO_H
#define SILVA_COMMISSIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"

/* ==================================================
 * Oraculum typorum (binarium in M1: novit/ignotum;
 * oraculum trivalens - "non-typus notus" - grammaticae
 * C89 nominatum)
 * ================================================== */

nomen structura {
    Piscina*        piscina;
    TabulaDispersa* typi;      /* clavis chorda (copiata) -> VERUM */
} SilvaOraculum;

SilvaOraculum* silva_oraculum_creare (Piscina* piscina);

/* Titulus in piscinam oraculi COPIATUR (valores lexematum prospectus
 * in fontem sunt - lectio vitae fons->via) */
b32 silva_oraculum_typum_addere (SilvaOraculum* oraculum,
    chorda titulus);
b32 silva_oraculum_typum_addere_literis (SilvaOraculum* oraculum,
    constans character* titulus);

b32 silva_oraculum_typum_novit (constans SilvaOraculum* oraculum,
    chorda titulus);


/* ==================================================
 * Resolutor (semantica grammaticae - vocatoris)
 * ================================================== */

nomen structura {
    s32         victor;        /* index interpretationis; -1 = ignotum */
    SilvaToken* discriminans;  /* lexema discriminante (NIHIL licet) */
} SilvaResolutioResponsum;

nomen vacuum (*SilvaResolutor) (
    constans SilvaNodus*    ambiguum,
    constans SilvaOraculum* oraculum,
    vacuum*                 contextus,
    SilvaResolutioResponsum* responsum);


/* ==================================================
 * Diarium resolutionum (productum queribile - Q5)
 * ================================================== */

nomen enumeratio {
    SILVA_RESOLUTIO_COLLAPSUS = 0,   /* in commissione - involucrum abiit */
    SILVA_RESOLUTIO_RECANONICATA     /* post commissionem - canonica versa */
} SilvaResolutioGenus;

nomen structura {
    SilvaResolutioGenus genus;
    SilvaNodus*         sedes;         /* allocatio (post collapsum: victor) */
    s32                 victor;        /* index interpretationis victricis */
    s32                 genus_victoris;
    SilvaToken*         discriminans;  /* origo eius = positio queribilis */
} SilvaResolutioEventum;


/* ==================================================
 * Commissio
 * ================================================== */

nomen structura {
    SilvaValor radix;          /* fortasse religata (normalizatio radicis) */
    Xar*       ambigui;        /* SilvaNodus* - superstites (recanonicandi) */
    Xar*       resolutiones;   /* SilvaResolutioEventum */

    /* Forma ambigui ex registro (pro recanonicatione) */
    s32 genus_ambigui;
    i32 locus_interpretationum;
    i32 locus_canonicae;
} SilvaCommissio;

/* Ambulatio commissionis. Oraculum/resolutor NIHIL licent (tunc
 * nulla resolutio - pater + normalizatio solae). NIHIL reddit si
 * tabularium formam ambigui non fert (generator eam imponit).
 * NB "registrum" ut titulus vetitum - macro latina (register)! */
SilvaCommissio*
silva_committere (
    Piscina*                       piscina,
    SilvaValor                     radix,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaOraculum*        oraculum,
    SilvaResolutor                 resolutor,
    vacuum*                        contextus);

/* Scientia sera: ambigua superstitia iterum resolvere. Canonica in
 * loco vertitur + spina localiter retexta; involucrum MANET (arbor
 * post commissionem immutabilis praeter hoc). Numerum versorum
 * reddit. */
i32
silva_recanonicare (
    SilvaCommissio*         commissio,
    constans SilvaOraculum* oraculum,
    SilvaResolutor          resolutor,
    vacuum*                 contextus);

#endif /* SILVA_COMMISSIO_H */
