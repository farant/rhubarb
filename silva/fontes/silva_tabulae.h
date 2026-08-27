/* silva_tabulae.h - Contractus tabularum coctarum (runtime)
 *
 * Typi quos plagulae GENERATAE (silva_tabulae_*.c, per silva_coquere)
 * implent et quos motor GLR (Phase 4) legit. Series planae statica -
 * nulla constructio in tempore currendi (lapifex tabulas ~7s in
 * tempore currendi construebat; silva eas in fontem coquit).
 *
 * §8.2: introitus actionum campum `pretium` ferunt et genus
 * SILVA_TAB_ACTIO_RECUPERARE existit - forma parata pro recuperatione
 * GSS pretio-ducta (tree-sitter modo). Introitus RECUPERARE emittuntur
 * cum opus recuperationis veniat; pretium = 0 pro actionibus normalibus.
 *
 * NB: prospectus -1 = $ (finis); terminale EOF etiam ut symbolum
 * explicitum existit (productio augmentata id transponit). Ambo
 * verbatim cocta; compositio EOF<->$ est res adaptatoris Phase 4.
 */

#ifndef SILVA_TABULAE_H
#define SILVA_TABULAE_H

#include "latina.h"
#include "silva_token.h"
#include "silva_registrum.h"   /* SilvaTabGenus/Locus/RegistrumCoctum (M4) */

/* ==================================================
 * Genus actionis
 * ================================================== */

nomen enumeratio {
    SILVA_TAB_ACTIO_ERRARE = 0,
    SILVA_TAB_ACTIO_TRANSPONERE,   /* SHIFT: valor = status novus */
    SILVA_TAB_ACTIO_REDUCERE,      /* REDUCE: valor = index productionis */
    SILVA_TAB_ACTIO_ACCIPERE,      /* ACCEPT */
    SILVA_TAB_ACTIO_RECUPERARE     /* recuperatio pretio-ducta (§8.2) */
} SilvaTabActioGenus;


/* ==================================================
 * Introitus tabularum
 * ================================================== */

nomen structura {
    s32 terminalis;    /* index symboli; -1 = $ */
    s32 actio;         /* SilvaTabActioGenus */
    s32 valor;         /* TRANSPONERE: status; REDUCERE: productio */
    i32 pretium;       /* pretium recuperationis; 0 = actio normalis */
} SilvaTabActio;

nomen structura {
    s32 non_terminalis;
    s32 status_novus;
} SilvaTabGoto;

/* Lamina per statum in series planas actionum/goto */
nomen structura {
    i32 actiones_offset;
    i32 actiones_numerus;
    i32 goto_offset;
    i32 goto_numerus;
} SilvaTabStatus;

nomen structura {
    constans character* titulus;         /* nomen symboli */
    SilvaLexemaGenus    genus_lexematis; /* pro terminalibus; EOF pro non-term */
    b32                 est_terminale;
} SilvaTabSymbolum;

nomen structura {
    s32                 sinistrum;   /* symbolum LHS */
    i32                 longitudo;   /* numerus symbolorum RHS (pop) */
    constans character* id;          /* id stabile; NIHIL = pass-through/augmentata */
} SilvaTabProductio;

/* Cella praelata (<praelatio>, M2c): conflictus in tabulis
 * DECLARATE resolutus - actio retenta sola manet, reiecta
 * memoratur. Categoria census: cellae praelatae numerantur et
 * figuntur sicut cellae conflictuum (numquam tacite crescunt). */
nomen structura {
    s32 status;             /* status cellae */
    s32 terminalis;         /* index symboli terminalis */
    s32 actio_retenta;      /* SilvaTabActioGenus retentum */
    s32 productio_remota;   /* index productionis remotae */
} SilvaTabPraelata;


/* ==================================================
 * Registrum generum coctum -> silva_registrum.h
 *
 * SCISSUM (M4): SilvaTabLocus, SilvaTabGenus,
 * SilvaRegistrumCoctum in caput proprium migraverunt, quod
 * NIHIL includit praeter latina.h. Vocabularium nodorum
 * motorem parsandi nescit; consumens qui registro SOLO eget
 * silva_registrum.h includat, non hoc caput.
 *
 * Hic re-includitur ut vocantes exsistentes intacti maneant.
 * ================================================== */


/* ==================================================
 * Tabula cocta integra
 * ================================================== */

nomen structura {
    constans SilvaTabSymbolum*   symbola;
    i32                          numerus_symbolorum;
    i32                          numerus_terminalium;
    constans SilvaTabProductio*  productiones;
    i32                          numerus_productionum;
    constans SilvaTabStatus*     status;
    i32                          numerus_statuum;
    constans SilvaTabActio*      actiones;
    i32                          numerus_actionum;
    constans SilvaTabGoto*       goto_introitus;
    i32                          numerus_goto;
    s32                          initium_index;         /* symbolum augmentatum */
    s32                          productio_augmentata;  /* index productionis S' */
    s32                          numerus_conflictuum;   /* servati (GLR) */
} SilvaTabulaCocta;

#endif /* SILVA_TABULAE_H */
