/* probatio_comitatus.c - Probationes figurarum comitandi
 * (include/comitatus.h)
 *
 * Pulsus: quarta CMLX, mensura 3/4 MMDCCCLXXX, 4/4 MMMDCCCXL. Ambitus
 * ordinarius 55..76 (superiores), 36..57 (bassus). Valsa "C | C | G7 |
 * C" manu computata: bassus C3 (48), quinta G2 (43); C 60 64 67, G7
 * tribus vocibus quinta omissa, motu minimo B F G (59 65 67), C rursus
 * 60 64 67.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "comitatus.h"
#include <stdio.h>
#include <string.h>

#define TERTIA_QUARTA  MMDCCCLXXX
#define QUARTA_QUARTA  MMMDCCCXL

hic_manens constans ComitatusAmbitus AMBITUS = { 55, 76, 36, 57 };

/* n + 1 initia mensurarum aequalium */
interior s64*
_initia (
    CredoContextus* c,
               i32  n,
               s64  longitudo)
{
    s64* initia = (s64*)piscina_allocare(c->piscina,
        (memoriae_index)(magnitudo(s64) * (memoriae_index)(n + I)));
    i32 k;

    per (k = ZEPHYRUM; k <= n; k++)
    {
        initia[k] = (s64)k * longitudo;
    }
    redde initia;
}

/* acorda lecta in mensuras aequales; numerus NIHIL si vitium */
interior ComitatusAcordum*
_acorda (
         CredoContextus* c,
     constans character* textus,
                    s64  longitudo,
                    i32* numerus,
        ComitatusVitium* vitium)
{
              chorda  t = chorda_ex_literis(textus, c->piscina);
                 i32  m = comitatus_mensurae(t);
    ComitatusAcordum* a = NIHIL;

    si (!comitatus_acorda_legere(t, _initia(c, m, longitudo), m,
                                 c->piscina, &a, numerus, vitium))
    {
        redde NIHIL;
    }
    redde a;
}

/* figura et acorda -> notae; FALSUM cum vitio impresso */
interior b32
_componere (
               CredoContextus*  c,
           constans character*  figura_textus,
           constans character*  acorda_textus,
                          s64   longitudo,
    constans ComitatusAmbitus*  ambitus,
                ComitatusNota** notae,
                          i32*  numerus,
              ComitatusVitium*  vitium)
{
              chorda  t = chorda_ex_literis(acorda_textus, c->piscina);
                 i32  m = comitatus_mensurae(t);
    ComitatusAcordum* a;
                 i32  n;
     ComitatusFigura  f;

    si (!comitatus_acorda_legere(t, _initia(c, m, longitudo), m,
                                 c->piscina, &a, &n, vitium))
    {
        imprimere("  acorda: %s (%u)\n", vitium->causa, vitium->offset);
        redde FALSUM;
    }
    si (!comitatus_figuram_legere(
            chorda_ex_literis(figura_textus, c->piscina), c->piscina,
            &f,
            vitium))
    {
        imprimere("  figura: %s (%u)\n", vitium->causa, vitium->offset);
        redde FALSUM;
    }
    redde comitatus_componere(&f, a, n, _initia(c, m, longitudo), m,
                              ambitus, c->piscina, notae, numerus,
                              vitium);
}

/* nota k: pulsus, longitudo, altitudo */
interior b32
_nota (
    constans ComitatusNota* notae,
                       i32  k,
                       s64  pulsus,
                       s64  longitudo,
                       i32  nota)
{
    si (   notae[k].pulsus == pulsus && notae[k].longitudo == longitudo
        && notae[k].nota   == nota)
    {
        redde VERUM;
    }
    imprimere("  nota %u: %ld+%ld %u pro %ld+%ld %u\n", k,
              (longus)notae[k].pulsus, (longus)notae[k].longitudo,
              notae[k].nota, (longus)pulsus, (longus)longitudo, nota);
    redde FALSUM;
}


    /* ==================================================
     * ACORDA: mensurae, valores, divisio residui, nullum
     * ================================================== */

interior vacuum
_sectio_mensurae (
    CredoContextus* c)
{
    CREDO_AEQUALIS_I32 (comitatus_mensurae(
        chorda_ex_literis("", c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (comitatus_mensurae(
        chorda_ex_literis("C", c->piscina)), I);
    CREDO_AEQUALIS_I32 (comitatus_mensurae(
        chorda_ex_literis("C | G", c->piscina)), II);
    CREDO_AEQUALIS_I32 (comitatus_mensurae(
        chorda_ex_literis("C | G |", c->piscina)), II);
    CREDO_AEQUALIS_I32 (comitatus_mensurae(
        chorda_ex_literis("C |  ", c->piscina)), I);
    CREDO_AEQUALIS_I32 (comitatus_mensurae(
        chorda_ex_literis("| C", c->piscina)), II);
}

interior vacuum
_sectio_acorda (
    CredoContextus* c)
{
    ComitatusAcordum* a;
     ComitatusVitium  v;
                 i32  n;

    a = _acorda(c, "C | G7 | C", TERTIA_QUARTA, &n, &v);
    CREDO_NECESSE (a != NIHIL);
    CREDO_AEQUALIS_I32 (n, III);
    CREDO_AEQUALIS_S64 (a[1].pulsus, TERTIA_QUARTA);
    CREDO_AEQUALIS_S64 (a[2].pulsus, II * TERTIA_QUARTA);
    CREDO_AEQUALIS_S64 (a[2].longitudo, TERTIA_QUARTA);
    CREDO_AEQUALIS_I32 ((i32)a[1].acordum.genus, (i32)ACORDUM_SEPTIMA);
    CREDO_AEQUALIS_I32 (a[1].acordum.radix, VII);
    CREDO_AEQUALIS_I32 (a[1].offset, IV);
    CREDO_AEQUALIS_I32 (a[2].offset, IX);
    /* linea harmoniae idem legitur */
    a = _acorda(c, "D:2 G:2", QUARTA_QUARTA, &n, &v);
    CREDO_NECESSE (a != NIHIL && n == II);
    CREDO_AEQUALIS_S64 (a[0].longitudo, MCMXX);
    CREDO_AEQUALIS_S64 (a[1].pulsus, MCMXX);
    /* sine valore: residuum aequaliter */
    a = _acorda(c, "C:2 G A", QUARTA_QUARTA, &n, &v);
    CREDO_NECESSE (a != NIHIL && n == III);
    CREDO_AEQUALIS_S64 (a[1].longitudo, CMLX);
    CREDO_AEQUALIS_S64 (a[2].pulsus, MMDCCCLXXX);
    CREDO_AEQUALIS_S64 (a[2].longitudo, CMLX);
    /* residuum divisionis ultimo */
    a = _acorda(c, "C G", VII, &n, &v);
    CREDO_NECESSE (a != NIHIL && n == II);
    CREDO_AEQUALIS_S64 (a[0].longitudo, III);
    CREDO_AEQUALIS_S64 (a[1].longitudo, IV);
    /* trioli: dimidia ternaria */
    a = _acorda(c, "C:3 G", QUARTA_QUARTA, &n, &v);
    CREDO_NECESSE (a != NIHIL && n == II);
    CREDO_AEQUALIS_S64 (a[0].longitudo, MCCLXXX);
    CREDO_AEQUALIS_S64 (a[1].longitudo, MMDLX);
    /* nullum */
    a = _acorda(c, "r | C", TERTIA_QUARTA, &n, &v);
    CREDO_NECESSE (a != NIHIL && n == II);
    CREDO_VERUM (a[0].nullum);
    CREDO_FALSUM (a[1].nullum);
}

/* vitia acordorum cum offset */
interior b32
_acorda_malum (
         CredoContextus* c,
     constans character* textus,
                    s64  longitudo,
                    i32  offset,
     constans character* causa)
{
    ComitatusVitium v;
                i32 n;

    si (_acorda(c, textus, longitudo, &n, &v) != NIHIL)
    {
        imprimere("  '%s' lectum\n", textus);
        redde FALSUM;
    }
    si (v.offset != offset || strcmp(v.causa, causa) != ZEPHYRUM)
    {
        imprimere("  '%s': %s (%u)\n", textus, v.causa, v.offset);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_sectio_acorda_vitia (
    CredoContextus* c)
{
    ComitatusAcordum* a;
     ComitatusVitium  v;
                 i32  n;

    CREDO_VERUM (_acorda_malum(c, "C | X", QUARTA_QUARTA, IV,
                               "acordum ignotum"));
    CREDO_VERUM (_acorda_malum(c, "C:2 G:2 A:2", QUARTA_QUARTA, VIII,
                               "valores mensuram excedunt"));
    CREDO_VERUM (_acorda_malum(c, "C:2", QUARTA_QUARTA, ZEPHYRUM,
                               "residuum mensurae sine acordo"));
    CREDO_VERUM (_acorda_malum(c, "C | | G", QUARTA_QUARTA, III,
                               "mensura vacua"));
    CREDO_VERUM (_acorda_malum(c, "C:7", QUARTA_QUARTA, II,
        "longitudo non divisor integrae (1..64)"));
    CREDO_VERUM (_acorda_malum(c, "C G A B C", IV, ZEPHYRUM,
                               "residuum nimis breve"));
    /* initia pauciora quam mensurae */
    CREDO_FALSUM (comitatus_acorda_legere(
        chorda_ex_literis("C | G", c->piscina),
        _initia(c, I, QUARTA_QUARTA), I, c->piscina, &a, &n, &v));
    CREDO_AEQUALIS_I32 (v.offset, III);
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(v.causa,
        c->piscina),
                                   "mensurae plures quam initia");
}


    /* ==================================================
     * FIGURA: gradus, voces, valores, greges, lucrum
     * ================================================== */

interior vacuum
_sectio_figura (
    CredoContextus* c)
{
    ComitatusFigura f;
    ComitatusVitium v;

    CREDO_NECESSE (comitatus_figuram_legere(chorda_ex_literis(
        "B:4 [1 2 3] [1 2 3] | Q:4 [1 2 3] [1 2 3]", c->piscina),
        c->piscina, &f, &v));
    CREDO_AEQUALIS_I32 (f.numerus, VI);
    CREDO_AEQUALIS_I32 (f.numerus_mensurarum, II);
    CREDO_AEQUALIS_S64 (f.longitudines[0], TERTIA_QUARTA);
    CREDO_AEQUALIS_S64 (f.longitudines[1], TERTIA_QUARTA);
    CREDO_AEQUALIS_I32 (f.primi[1], III);
    CREDO_AEQUALIS_I32 (f.primi[2], VI);
    CREDO_AEQUALIS_I32 (f.vox_maxima, III);
    CREDO_AEQUALIS_S32 (f.gradus[0].voces[0], -I);
    CREDO_AEQUALIS_S32 (f.gradus[3].voces[0], -II);
    CREDO_AEQUALIS_I32 (f.gradus[1].numerus_vocum, III);
    CREDO_AEQUALIS_S32 (f.gradus[1].voces[2], III);
    CREDO_AEQUALIS_S64 (f.gradus[1].pulsus, CMLX);
    CREDO_AEQUALIS_S64 (f.gradus[1].longitudo, CMLX);
    CREDO_AEQUALIS_I32 (f.gradus[1].offset, IV);
    CREDO_AEQUALIS_S64 (f.gradus[4].pulsus, CMLX);
    /* puncta, valor manens */
    CREDO_NECESSE (comitatus_figuram_legere(chorda_ex_literis(
        "1:8. 2:16 3 r:2", c->piscina), c->piscina, &f, &v));
    CREDO_AEQUALIS_S64 (f.gradus[0].longitudo, DCCXX);
    CREDO_AEQUALIS_S64 (f.gradus[1].longitudo, CCXL);
    CREDO_AEQUALIS_S64 (f.gradus[2].longitudo, CCXL);
    CREDO_AEQUALIS_I32 (f.gradus[3].numerus_vocum, ZEPHYRUM);
    CREDO_AEQUALIS_S64 (f.longitudines[0], MMMCXX);
    /* fractum */
    CREDO_NECESSE (comitatus_figuram_legere(chorda_ex_literis(
        "{B 1 2 3}:2.", c->piscina), c->piscina, &f, &v));
    CREDO_VERUM (f.gradus[0].fractum);
    CREDO_AEQUALIS_I32 (f.gradus[0].numerus_vocum, IV);
    CREDO_AEQUALIS_S64 (f.gradus[0].longitudo, TERTIA_QUARTA);
    /* lucrum: v gradibus sequentibus, mensura nova a 100 */
    CREDO_NECESSE (comitatus_figuram_legere(chorda_ex_literis(
        "v50 B:4 v 80 1:2 | B:2.", c->piscina), c->piscina, &f, &v));
    CREDO_F32_PROXIMUS (f.gradus[0].lucrum, 0.5f, 0.0001f);
    CREDO_F32_PROXIMUS (f.gradus[1].lucrum, 0.8f, 0.0001f);
    CREDO_F32_PROXIMUS (f.gradus[2].lucrum, 1.0f, 0.0001f);
}

interior b32
_figura_mala (
         CredoContextus* c,
     constans character* textus,
                    i32  offset,
     constans character* causa)
{
    ComitatusFigura f;
    ComitatusVitium v;

    si (comitatus_figuram_legere(chorda_ex_literis(textus, c->piscina),
                                 c->piscina, &f, &v))
    {
        imprimere("  '%s' lecta\n", textus);
        redde FALSUM;
    }
    si (v.offset != offset || strcmp(v.causa, causa) != ZEPHYRUM)
    {
        imprimere("  '%s': %s (%u)\n", textus, v.causa, v.offset);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_sectio_figura_vitia (
    CredoContextus* c)
{
    CREDO_VERUM (_figura_mala(c, "", ZEPHYRUM,
        "mensura figurae vacua"));
    CREDO_VERUM (_figura_mala(c, "B | | B", III,
                              "mensura figurae vacua"));
    CREDO_VERUM (_figura_mala(c, "X", ZEPHYRUM, "vox ignota"));
    CREDO_VERUM (_figura_mala(c, "0", ZEPHYRUM, "vox ignota"));
    CREDO_VERUM (_figura_mala(c, "b", ZEPHYRUM, "vox ignota"));
    CREDO_VERUM (_figura_mala(c, "[1 2", ZEPHYRUM, "grex non clausus"));
    CREDO_VERUM (_figura_mala(c, "[]", ZEPHYRUM, "grex vacuus"));
    CREDO_VERUM (_figura_mala(c, "[1 2 3 4 5 6 7 8 9 1]", XIX,
                              "voces plures quam IX"));
    CREDO_VERUM (_figura_mala(c, "v101", ZEPHYRUM,
                              "lucrum extra 0..100"));
    CREDO_VERUM (_figura_mala(c, "v", I, "numerus exspectatus"));
    CREDO_VERUM (_figura_mala(c, "{1 2 3}:64", ZEPHYRUM,
                              "fractum gradu longius"));
    CREDO_VERUM (_figura_mala(c, "B:65", II,
        "longitudo non divisor integrae (1..64)"));
    CREDO_VERUM (_figura_mala(c, "B:4...", V, "puncta plura quam II"));
}


    /* ==================================================
     * COMPOSITIO: valsa, ductus vocum, bassus, quinta
     * ================================================== */

interior vacuum
_sectio_saltatio (
    CredoContextus* c)
{
      ComitatusNota* e;
    ComitatusVitium  v;
                i32  n;

    CREDO_NECESSE (_componere(c,
        "B:4 [1 2 3] [1 2 3] | Q:4 [1 2 3] [1 2 3]", "C | C | G7 | C",
        TERTIA_QUARTA, &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, XXVIII);
    /* mensura 1: bassus C3, voces medio ambitus proximae */
    CREDO_VERUM (_nota(e, 0, 0, CMLX, 48));
    CREDO_VERUM (_nota(e, 1, CMLX, CMLX, 60));
    CREDO_VERUM (_nota(e, 2, CMLX, CMLX, 64));
    CREDO_VERUM (_nota(e, 3, CMLX, CMLX, 67));
    CREDO_VERUM (_nota(e, 6, MCMXX, CMLX, 67));
    /* mensura 2, acordum tentum: quinta G2 */
    CREDO_VERUM (_nota(e, 7, TERTIA_QUARTA, CMLX, 43));
    CREDO_VERUM (_nota(e, 8, TERTIA_QUARTA + CMLX, CMLX, 60));
    /* mensura 3, G7: radix (bassus G2); quinta D omissa, B F G motu
     * minimo - sensibilis manet */
    CREDO_VERUM (_nota(e, 14, II * TERTIA_QUARTA, CMLX, 43));
    CREDO_VERUM (_nota(e, 15, II * TERTIA_QUARTA + CMLX, CMLX, 59));
    CREDO_VERUM (_nota(e, 16, II * TERTIA_QUARTA + CMLX, CMLX, 65));
    CREDO_VERUM (_nota(e, 17, II * TERTIA_QUARTA + CMLX, CMLX, 67));
    /* mensura 4, C novum: radix, non quinta; C 60 64 67 rursus */
    CREDO_VERUM (_nota(e, 21, III * TERTIA_QUARTA, CMLX, 48));
    CREDO_VERUM (_nota(e, 22, III * TERTIA_QUARTA + CMLX, CMLX, 60));
    CREDO_VERUM (_nota(e, 24, III * TERTIA_QUARTA + CMLX, CMLX, 67));
    /* litterae, lucrum, offset gradus */
    CREDO_AEQUALIS_I32 ((i32)e[0].littera, (i32)'c');
    CREDO_AEQUALIS_I32 ((i32)e[2].littera, (i32)'e');
    CREDO_F32_PROXIMUS (e[0].lucrum, 1.0f, 0.0001f);
    CREDO_AEQUALIS_I32 (e[0].offset, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (e[1].offset, IV);
}

/* figurae aliae: fractum, Alberti, octava supra, aequales inferior */
interior vacuum
_sectio_figurae (
    CredoContextus* c)
{
      ComitatusNota* e;
    ComitatusVitium  v;
                i32  n;
                i32  i;

    /* harpa: voces per sexagesimam quartam, finis communis */
    CREDO_NECESSE (_componere(c, "{B 1 2 3}:2.", "C", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, IV);
    CREDO_VERUM (_nota(e, 0, 0, TERTIA_QUARTA, 48));
    CREDO_VERUM (_nota(e, 1, LX, TERTIA_QUARTA - LX, 60));
    CREDO_VERUM (_nota(e, 2, CXX, TERTIA_QUARTA - CXX, 64));
    CREDO_VERUM (_nota(e, 3, CLXXX, TERTIA_QUARTA - CLXXX, 67));
    /* Alberti */
    CREDO_NECESSE (_componere(c, "1:8 3 2 3 1 3 2 3", "C",
                              QUARTA_QUARTA, &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, VIII);
    CREDO_VERUM (_nota(e, 0, 0, CDLXXX, 60));
    CREDO_VERUM (_nota(e, 1, CDLXXX, CDLXXX, 67));
    CREDO_VERUM (_nota(e, 2, CMLX, CDLXXX, 64));
    CREDO_VERUM (_nota(e, 7, MMMCCCLX, CDLXXX, 67));
    /* vox ultra classes: octava supra */
    CREDO_NECESSE (_componere(c, "1 2 3 4", "C", QUARTA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_VERUM (_nota(e, 3, MMDCCCLXXX, CMLX, 72));
    /* aequales: inferior (E: 40 et 52 a 46 aeque distant) */
    CREDO_NECESSE (_componere(c, "B:1", "E", QUARTA_QUARTA, &AMBITUS,
                              &e, &n, &v));
    CREDO_VERUM (_nota(e, 0, 0, QUARTA_QUARTA, 40));
    /* litterae ex nomine: C#m -> E, G# */
    CREDO_NECESSE (_componere(c, "[1 2 3]:2.", "C#m", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 ((i32)e[0].littera, (i32)'c');
    CREDO_AEQUALIS_I32 ((i32)e[1].littera, (i32)'e');
    CREDO_AEQUALIS_I32 ((i32)e[2].littera, (i32)'g');
    /* quinta acordi diminuti: F (non F#) */
    CREDO_NECESSE (_componere(c, "Q:2.", "Bdim", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (e[0].nota % XII, V);
    /* voces pauciores quam classes: quinta omittitur (C duabus: E C);
     * voces aequales: nulla omittitur (G7 quattuor) */
    CREDO_NECESSE (_componere(c, "[1 2]:2.", "C", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, II);
    CREDO_AEQUALIS_I32 (e[0].nota, 64);
    CREDO_AEQUALIS_I32 (e[1].nota, 72);
    CREDO_NECESSE (_componere(c, "[1 2 3 4]:2.", "G7", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, IV);
    CREDO_AEQUALIS_I32 (((i32)I << (e[0].nota % XII))
                        | ((i32)I << (e[1].nota % XII))
                        | ((i32)I << (e[2].nota % XII))
                        | ((i32)I << (e[3].nota % XII)),
                        ((i32)I << VII) | ((i32)I << XI) | ((i32)I
                            << II)
                        | ((i32)I << V));
    /* adhuc pauciores: radix quoque (bassus eam tenet) - C7 duabus
     * E Bb, C9 tribus E Bb D */
    CREDO_NECESSE (_componere(c, "[1 2]:2.", "C7", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, II);
    CREDO_AEQUALIS_I32 (((i32)I << (e[0].nota % XII))
                        | ((i32)I << (e[1].nota % XII)),
                        ((i32)I << IV) | ((i32)I << X));
    CREDO_NECESSE (_componere(c, "[1 2 3]:2.", "C9", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, III);
    CREDO_AEQUALIS_I32 (((i32)I << (e[0].nota % XII))
                        | ((i32)I << (e[1].nota % XII))
                        | ((i32)I << (e[2].nota % XII)),
                        ((i32)I << IV) | ((i32)I << X) | ((i32)I
                            << II));
    /* catena septimarum: motus minimus radicem numquam reducit */
    CREDO_NECESSE (_componere(c, "[1 2]:2.",
                              "C7 | F7 | Bb7 | Eb7 | Ab7 | Db7",
                              TERTIA_QUARTA, &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, XII);
    per (i = ZEPHYRUM; i < XII; i++)
    {
        CREDO_VERUM (e[i].nota % XII != ((i / II) * V) % XII);
    }
    /* tonus additus sine septima: radix prius, deinde quinta - F6
     * tribus A C D (non A D F), duabus A D; Bbadd9 tribus C D F */
    CREDO_NECESSE (_componere(c, "[1 2 3]:2.", "F6", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, III);
    CREDO_AEQUALIS_I32 (((i32)I << (e[0].nota % XII))
                        | ((i32)I << (e[1].nota % XII))
                        | ((i32)I << (e[2].nota % XII)),
                        ((i32)I << IX) | ((i32)I << ZEPHYRUM)
                        | ((i32)I << II));
    CREDO_NECESSE (_componere(c, "[1 2]:2.", "F6", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, II);
    CREDO_AEQUALIS_I32 (((i32)I << (e[0].nota % XII))
                        | ((i32)I << (e[1].nota % XII)),
                        ((i32)I << IX) | ((i32)I << II));
    CREDO_NECESSE (_componere(c, "[1 2 3]:2.", "Bbadd9", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, III);
    CREDO_AEQUALIS_I32 (((i32)I << (e[0].nota % XII))
                        | ((i32)I << (e[1].nota % XII))
                        | ((i32)I << (e[2].nota % XII)),
                        ((i32)I << ZEPHYRUM) | ((i32)I << II)
                        | ((i32)I << V));
    /* catena sextarum duabus vocibus: radix et quinta omissae - quinta
     * numquam redit */
    CREDO_NECESSE (_componere(c, "[1 2]:2.",
                              "F6 | C6 | G6 | D6 | A6 | E6",
                              TERTIA_QUARTA, &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, XII);
    per (i = ZEPHYRUM; i < XII; i++)
    {
        CREDO_VERUM (e[i].nota % XII != (XII + (i / II) * VII) % XII);
    }
    /* cum septima ordo solitus: C9 quattuor C E Bb D (non E G Bb D) */
    CREDO_NECESSE (_componere(c, "[1 2 3 4]:2.", "C9", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, IV);
    CREDO_AEQUALIS_I32 (((i32)I << (e[0].nota % XII))
                        | ((i32)I << (e[1].nota % XII))
                        | ((i32)I << (e[2].nota % XII))
                        | ((i32)I << (e[3].nota % XII)),
                        ((i32)I << ZEPHYRUM) | ((i32)I << IV)
                        | ((i32)I << X) | ((i32)I << II));
    /* C/G: quinta = bassus - octava altera (43 -> 55) */
    CREDO_NECESSE (_componere(c, "B:4 Q:2", "C/G", TERTIA_QUARTA,
                              &AMBITUS, &e, &n, &v));
    CREDO_VERUM (_nota(e, 0, 0, CMLX, 43));
    CREDO_VERUM (_nota(e, 1, CMLX, MCMXX, 55));
    /* lucrum ex figura */
    CREDO_NECESSE (_componere(c, "v50 B:4 v80 1:2 | B:2.", "C | C",
                              TERTIA_QUARTA, &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, III);
    CREDO_F32_PROXIMUS (e[0].lucrum, 0.5f, 0.0001f);
    CREDO_F32_PROXIMUS (e[1].lucrum, 0.8f, 0.0001f);
    CREDO_F32_PROXIMUS (e[2].lucrum, 1.0f, 0.0001f);
}

/* tempus: nullum tacet et figuram renovat; acordum in media mensura;
 * mensura figurae ex acordo ANTE mensuram */
interior vacuum
_sectio_tempus (
    CredoContextus* c)
{
      ComitatusNota* e;
    ComitatusVitium  v;
                i32  n;

    CREDO_NECESSE (_componere(c,
        "B:4 [1 2 3] [1 2 3] | Q:4 [1 2 3] [1 2 3]", "C | r | C",
        TERTIA_QUARTA, &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, XIV);
    CREDO_VERUM (_nota(e, 7, II * TERTIA_QUARTA, CMLX, 48));
    /* acordum mutatum in media mensura */
    CREDO_NECESSE (_componere(c, "B:4 [1 2 3] B [1 2 3]", "C:2 G:2",
                              QUARTA_QUARTA, &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, VIII);
    CREDO_VERUM (_nota(e, 4, MCMXX, CMLX, 43));
    CREDO_VERUM (_nota(e, 5, MMDCCCLXXX, CMLX, 59));
    CREDO_VERUM (_nota(e, 6, MMDCCCLXXX, CMLX, 62));
    CREDO_VERUM (_nota(e, 7, MMDCCCLXXX, CMLX, 67));
    /* G tentum trans lineam: mensura secunda figurae (quinta D2) */
    CREDO_NECESSE (_componere(c, "B:1 | Q:1", "C:2 G:2 | G",
                              QUARTA_QUARTA, &AMBITUS, &e, &n, &v));
    CREDO_AEQUALIS_I32 (n, II);
    CREDO_VERUM (_nota(e, 1, QUARTA_QUARTA, QUARTA_QUARTA, 38));
}

interior vacuum
_sectio_compositio_vitia (
    CredoContextus* c)
{
    hic_manens constans ComitatusAmbitus ANGUSTUS = { 55, 60, 36, 57 };
    hic_manens constans ComitatusAmbitus ALTUS = { 100, 127, 36,
        57 };
      ComitatusNota* e;
    ComitatusVitium  v;
                i32  n;

    CREDO_FALSUM (_componere(c, "B:4 1 1", "C", QUARTA_QUARTA, &AMBITUS,
                             &e, &n, &v));
    CREDO_AEQUALIS_I32 (v.offset, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(v.causa,
        c->piscina),
        "mensura figurae mensurae carminis non congruit");
    CREDO_FALSUM (_componere(c, "B:1", "C", QUARTA_QUARTA, &ANGUSTUS,
        &e,
                             &n, &v));
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(v.causa,
        c->piscina),
        "ambitus angustior octava");
    CREDO_FALSUM (_componere(c, "9:1", "C", QUARTA_QUARTA, &ALTUS, &e,
        &n,
                             &v));
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(v.causa,
        c->piscina),
        "nota extra MIDI");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "mensurae", _sectio_mensurae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "acorda", _sectio_acorda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "acorda_vitia", _sectio_acorda_vitia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "figura", _sectio_figura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "figura_vitia", _sectio_figura_vitia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "valsa", _sectio_saltatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "figurae", _sectio_figurae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tempus", _sectio_tempus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "compositio_vitia", _sectio_compositio_vitia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("comitatus", SECTIONES);
}
