/* ludus_tessera.h - glutinum tesserae: eventus -> dispensator,
 * quadrum -> pingere -> demittere -> praesentare (modulus 013 A3;
 * gemellus ludus_fenestra.h)
 *
 * Unicus locus ubi ludus in terminali horologium tangit
 * (fenestra_tempus_ms): tempus in eventu stampatur, infra nemo legit.
 * Terminalis possidet (terminalis + rivus, modi per
 * ludus_tessera_modos_componere), tessera pingit (pons
 * ludus_tessera_pontem_initiare). Modulus: cellula in pixelis nostris
 * (ordinarie VI x VIII, ut glyphus fenestrae); extensio = opus x
 * cellula, cum amplitudine renovatur. Rivus cellulam eandem accipit
 * (mus ad centrum cellulae in pixelis nostris).
 *
 * CLAUSURA: terminalis fenestram claudendam non habet - chorda
 * claudendi (ordinarie Ctrl-C, ut auscultator_terminalis) ansam finit
 * ut EVENTUS_CLAUDERE in fenestra: applicatio eam numquam videt.
 * Campi claudendi_* mutari possunt (applicatio quae Ctrl-C vult).
 */

#ifndef LUDUS_TESSERA_H
#define LUDUS_TESSERA_H

#include "latina.h"
#include "piscina.h"
#include "eventus.h"
#include "dispensator.h"
#include "figura.h"
#include "mandatum.h"
#include "modulus.h"
#include "delineare_mandata.h"
#include "tessera_opus.h"

nomen structura {
    i32 quadra;
    s64 ms_compositionis;     /* intra pulsum */
    s64 ms_pingendi;
    s64 ms_demittendi;        /* mandata -> cellulae */
    s64 ms_quadri_maximum;
    i32 octeti_emissi;        /* ex TesseraFructus (currere) */
} LudusTesseraMensurae;

/* pulsus applicationis vivae (vicus-latera S1c): vocatur iteratione
 * ansae quaque; VERUM = aliquid mutatum. Typus suus (non
 * LudusPulsator): geminus caput fenestrae non includit. */
nomen b32 (*LudusTesseraPulsator)(vacuum* ctx);

nomen structura {
             Dispensator* d;
         FiguraRegistrum* figurae;
                     i32  thema;
               ImagoFons  fons;
                  vacuum* fons_ctx;
             TesseraOpus* opus;
                 Modulus  modulus;
                 Piscina* piscina;            /* rivus (currere) */
                 Piscina* piscina_quadri;
                 Mandata* mandata;            /* quadri ultimi */
                     s32  claudendi_runa;     /* ordinarie 'c' */
                     i32  claudendi_modificantes;
                     b32  magnitudo_nuntiata;  /* B1: initialis missa */
    LudusTesseraMensurae  mensurae;
    LudusTesseraPulsator  pulsator;           /* S1c: NIHIL = nullus */
                  vacuum* pulsator_ctx;
                     i32  versio_picta;       /* S1c: quadro ultimo */
} LudusTessera;

/* opus apertum (pons ludus_tessera aut memoriae); cellula in pixelis
 * nostris (> 0). NIHIL si argumentum NIHIL. */
LudusTessera*
ludus_tessera_creare (
            Piscina* piscina,
        Dispensator* d,
    FiguraRegistrum* figurae,
                i32  thema,
          ImagoFons  fons,
             vacuum* fons_ctx,
        TesseraOpus* opus,
                s32  cellula_latitudo,
                s32  cellula_altitudo);

/* Eventusne chorda claudendi est? (clavis pressa, runa et
 * modificantes claudendi) */
b32
ludus_tessera_claudendum_est (
    constans LudusTessera* lt,
         constans Eventus* ev);

/* Eventus in dispensatorem; tempus ZEPHYRUM stampatur 'nunc' ANTE
 * traditionem. Ante eventum primum (aut quadrum primum) magnitudo
 * initialis semel nuntiatur (MUTARE_MAGNITUDINEM: cellulae x modulus)
 * - superficies status est (013 B1). MUTARE_MAGNITUDINEM: opus
 * amplitudinem a ponte relegit, extensio moduli renovatur (pictura
 * plena sequitur). RESUMPTIO: pictura plena sequitur. Uterque etiam
 * dispensatori traditur. */
vacuum
ludus_tessera_tractare (
        LudusTessera* lt,
    constans Eventus* ev,
                 s64  nunc);

/* Quadrum unum: pulsus, pingere, demittere (non praesentat). */
vacuum
ludus_tessera_quadrum (
    LudusTessera* lt,
             s64  nunc);

/* Ansa vera in terminali: modi (MUS + GLUTINUM) per terminalis
 * intrati, rivus legit, tractare, quadrum, praesentare; finis in chorda
 * claudendi, terminali amisso, aut post quadra_maxima (> 0), deinde
 * dispensator_finire (pendentia effunduntur). Redde 0;
 * I si non terminalis (isatty) aut intratio deficit - terminalis
 * intactus. Mensurae in stdout post exitum. */
s32
ludus_tessera_currere (
    LudusTessera* lt,
             i32  quadra_maxima);

/* vicus-latera S1c: pulsum ponere (NIHIL tollit); cum pulsu ansa
 * XVI ms ad summum exspectat et quadrum solum pingit cum
 * ludus_tessera_pingendum VERUM reddit. */
vacuum
ludus_tessera_pulsum_ponere (
            LudusTessera* lt,
    LudusTesseraPulsator  fn,
                  vacuum* ctx);

/* decisio iterationis ut ludus_fenestra_pingendum (versio
 * repositorii contra quadrum ultimum); sine pulsu semper VERUM */
b32
ludus_tessera_pingendum (
    LudusTessera* lt,
             b32  eventa,
             s64  nunc);

#endif /* LUDUS_TESSERA_H */
