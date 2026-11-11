/*
 * statua.h - statua: simulacrum trium dimensionum et positura eius
 *
 * Valor unus, a forma plagulae liber (project-specs/statuae-spec.md
 * IV.2): arbor nodorum, retia triangulorum, materiae, texturae,
 * cycli animationis. Lectores formarum (statua_glb.h) eum implent;
 * pictor (statua_pictura.h) eum legit. Nihil de plagulis aut pixelis
 * scit.
 *
 * Omnia in piscina vocatoris, tabulae planae cum indicibus. Statua
 * impleta et finita (statua_finire) non mutatur.
 *
 * Solum nodi moventur (translatio, rotatio, scala per claves);
 * vertices in spatio retis sui manent. Pelles (skins) non sunt.
 */

#ifndef STATUA_H
#define STATUA_H

/* <aedilis corpus="lib/statua.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "imago_typus.h"
#include "stereometria.h"

nomen enumeratio {
    STATUA_TRANSLATIO = ZEPHYRUM,
    STATUA_ROTATIO,
    STATUA_SCALA
} StatuaProprietas;

/* proprietas una nodi unius per tempus */
nomen structura {
                 s32 nodus;
    StatuaProprietas proprietas;
                 b32 gradatim;   /* VERUM: valor clavis prioris
                                   * tenetur (STEP); aliter linearis
                                   * (rotatio: slerp) */
                 i32  clavium;    /* >= I */
                 f32* tempora;    /* clavium, crescentia, secundae */
                 f32* valores;    /* III per clavem; rotatio IV
                                   * (x y z w) */
} StatuaCanalis;

nomen structura {
           chorda  titulus;
              f32  duratio;       /* tempus clavis maximum; >= 0 */
    StatuaCanalis* canales;
              i32  canalium;
} StatuaCyclus;

/* signa nodi: quid pars in loco sit (lector ea ex extras.tags
 * plagulae legit; mundus_solum ex eis tabulam gressus struit) */
#define STATUA_SIGNUM_GRESSIBILE  I     /* "walkable" */
#define STATUA_SIGNUM_SOLIDUM     II    /* "collidable" */
#define STATUA_SIGNUM_AQUA        IV    /* "water" */

nomen structura {
                      s32 parens;      /* -1 = radix */
                      s32 rete;        /* -1 = nullum */
                      i32 signa;       /* STATUA_SIGNUM_*; 0 = nullum */
       StereometriaVector translatio;  /* quies */
    StereometriaQuaternio rotatio;
       StereometriaVector scala;
} StatuaNodus;

nomen structura {
    s32 textura;        /* index in texturas; -1 = alba */
    f32 color[IV];      /* factor coloris basis (r g b a) */
    f32 limen_alpha;    /* < 0 = opaca; aliter pixelum cuius alpha
                         * infra limen est exciditur */
    b32 emissiva;       /* lucem neglegit */
    b32 bilateralis;    /* tergum quoque pingitur */
    b32 repetit_s;      /* FALSUM = ad marginem coercetur */
    b32 repetit_t;
    b32 speculat_s;     /* repetitio speculata (repetit_ quoque
                         * VERUM) */
    b32 speculat_t;
} StatuaMateria;

/* tractus triangulorum materiae unius */
nomen structura {
    i32 index_primus;   /* in indices */
    i32 indicum;        /* multiplum III */
    s32 materia;        /* -1 = ordinaria: alba, opaca, unilateralis */
} StatuaPars;

/* rete: partes contiguae in tabula partium */
nomen structura {
    i32 pars_prima;
    i32 partium;
} StatuaRete;

nomen structura {
           StatuaNodus* nodi;
                   i32  nodorum;
                   i32* ordo;         /* nodorum: parentes ante
                                       * liberos (statua_finire) */
            StatuaRete* retia;
                   i32  retium;
            StatuaPars* partes;
                   i32  partium;
                   f32* positiones;   /* III per verticem */
                   f32* normales;     /* III per verticem */
                   f32* coordinatae;  /* II per verticem (s, t);
                                       * t = 0 ordo summus texturae */
                   i32  verticum;
                   i32* indices;      /* in vertices, absoluti */
                   i32  indicum;
         StatuaMateria* materiae;
                   i32  materiarum;
                 Imago* texturae;
                   i32  texturarum;
          StatuaCyclus* cycli;
                   i32  cyclorum;
    StereometriaVector  centrum;      /* sphaera posituram QUIETIS
                                       * tegens (statua_finire);
                                       * cycli eam excedere possunt */
                   f32 radius;
} Statua;

/*
 * statua_finire - post impletionem, semel
 *
 * Probat: parens cuiusque nodi intra fines et sine circulo; rete,
 * partes, indices, materiae, texturae, canales intra fines suos;
 * indicum multiplum III; claves crescentes. Deinde 'ordo' et sphaera
 * ambiens positurae quietis computantur (centrum capsae verticum,
 * distantia maxima). Cycli sphaeram non augent: pictor statuam ad
 * quietem metitur, et pars quae in cyclo longe abit (saxum iactum)
 * picturam relinquit. Statua sine verticibus: centrum 0, radius 0.
 *
 * Redde: FALSUM si quid extra fines aut piscina deficit (statua tunc
 * non adhibenda)
 */
b32
statua_finire (
     Statua* statua,
    Piscina* piscina);

/*
 * statua_positura - matrices mundi nodorum ad tempus datum
 *
 * @cyclus:   index cycli; -1 (aut extra fines) = quies
 * @tempus:   secundae; per durationem involvitur (negativum quoque);
 *            duratio 0 = clavis prima
 * @matrices: nodorum loci a vocatore praebiti; matrices[n] nodum n
 *            in spatium statuae fert
 *
 * Nodus sine canali valorem quietis tenet. Ante clavem primam valor
 * primus, post ultimam ultimus. Statuam finitam exspectat.
 */
vacuum
statua_positura (
       constans Statua* statua,
                   s32  cyclus,
                   f32  tempus,
    StereometriaMatrix* matrices);

#endif /* STATUA_H */
