/* probatio_crusta_totalitas.c - Porta totalitatis crustae (P9)
 *
 * CONTRACTUS DUPLEX omnibus linguis materiae (Fran, 2026-09-01):
 *   (a) NUMQUAM RUIT pro quolibet fonte - furca, WIFSIGNALED
 *   (b) SEMPER arborem reddit, et emissio == fons octetim
 * Exemplar probatio_html_totalitas: octeti fortuiti (LCG seminatus),
 * mutatio corporis (I ex XL octetis), truncatio (XXIII gradus),
 * nidificatio clausa et aperta (formae XI, profunditates I..M); CRLF
 * (adversarius totus, casus pathologiarum '\n' -> '\r\n'); NUL.
 * Corpus: fixa II + FreeBSD LXXXIX + plagulae domus XII.
 *
 * PROFUNDITAS (spec par. V; 01M1FAD8), MENSURATA 2026-09-16, -O2:
 *   parsura sola      C milia, formae VI           VIVIT (< 0,3 s)
 *   emissio           XL milia nodorum VIVIT, XLV milia SIGSEGV
 *                     (materia_scribere_nodum recursivus, ut html)
 *   proiectio STML    D profunda VIVIT; C milia SIGSEGV statim; X
 *                     milia > XX s (magnitudo quadratica: II milia =
 *                     LVI MB, V s; IV milia = CCXXV MB, XLIII s) - non
 *                     ruina sed tempus, non pinnatum
 *   aestimator        '1+(' C milia VIVIT, CLX milia SIGSEGV
 *                     (crusta_arithmetica_aestimare recursivus, P4;
 *                     inclusa sola vocatio caudalis est, ergo forma
 *                     '1+(' non caudalis mensuratur)
 * Pinnae ruinae (RUIT_CUM SIGSEGV) rubent cum remedium veniat - tunc
 * in NON_RUIT promovendae.
 *
 * INVENTUM (ante portam): parsura '$(' x C milia LXXX s et SIGSEGV -
 * valor staticus verbi primi (_aedificator_est) subarborem totam
 * recursive metiebatur. Emendatum in crusta_arbor.c
 * (_longitudo_statica), casus in porta arboris.
 *
 * NUL: fons cum octeto NUL (in verbo, in corpore heredoc, in
 * commentario) parsatur et octetim emittitur; scriptor STML eum
 * NOMINATIM recusat ('valor lexematis NUL fert', 'valor trivii NUL
 * fert') - refusio, non ruina.
 *
 * INVENTA (porta ipsa, 2026-09-16: MMCMXXXIV casus, VIII fracti, omnes
 * emendati cum casu in porta arboris aut arithmeticae): iuxtapositio
 * arithmetica ('(( a b ))' operandum perdebat), iuxtapositio iudicii
 * sub '!' ('[[ ! a b' ordinem rumpebat), titulus expansionis per genus
 * octeti non per positionem ('${@E}', '${x[1][2]}' locum bis
 * scribebant - parsator NIHIL).
 *
 * HEREDOC IN LACUNIS (P9b): corpus cuius linea nova intra [[ ]],
 * (( )), $(( )) aut ante corpus functionis cadit in lista post_* ubi
 * octeti iacent ponitur (bash 5.2 ea legit); lacunae ubi bash errat
 * gradum absentem claudunt. Limes P6 (heredoca_transposita) sublatus:
 * emissio == fons SINE exceptione, et relatio quae corpus transpositum
 * numerat ipsa fractura est. Porta haec limitem invenit (I casus ex
 * MMCMXXXIV) et eius sublationem custodit; inventum sub eo: praepositum
 * post operandum ('[[ !()-a') ordinem rumpebat, emendatum.
 *
 * IN FRACTURA: fons peccans in crusta/build/totalitas_fractum.sh.
 */

#include "latina.h"
#include "credo.h"
#include "crusta_arbor.h"
#include "crusta_diagnostica.h"
#include "crusta_arithmetica.h"
#include "crusta_exempla.h"
#include "crusta_lector.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "piscina.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* DOMUS[] = {
    "silva/compile_probationes.sh",
    "tools/vexilla.sh",
    "html/compile_probationes.sh",
    "crusta/compile_probationes.sh",
    "tools/sera.sh",
    "tools/unci-git/lint_latinus.sh",
    "tools/natura_struere.sh",
    "oratio/vocabula.sh",
    "silva/renominare.sh",
    "silva/formator.sh",
    "compile_tests.sh",
    "tools/mensor_suitae.sh"
};

#define NUMERUS_DOMUS ((i32)(magnitudo(DOMUS)/magnitudo(DOMUS[0])))

enumeratio {
    TOTUM_IDEM = 0,
    TOTUM_NIHIL,
    TOTUM_EMISSIO_FRACTA,
    TOTUM_DISPAR,
    TOTUM_TRANSPOSITUM
};

hic_manens constans character* CAUSAE[] = {
    "idem",
    "parsator NIHIL reddidit",
    "emissio fracta",
    "emissio a fonte dispar",
    "heredoc transpositum (relatio.heredoca_transposita)"
};

hic_manens constans character* RADIX_VIAE = ".";

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
     character plena[4096];
         FILE*  f;
        longus  longitudo;
    character*  memoria;
        size_t  lecti;

    sprintf(plena, "%s/%s", RADIX_VIAE, via);
    f = fopen(plena, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    fseek(f, 0L, SEEK_SET);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (memoria == NIHIL)
    {
        fclose(f);
        redde NIHIL;
    }
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    memoria[lecti]  = '\0';
    *mensura        = (i32)lecti;
    redde memoria;
}

/* Casus unus, piscina SUA: parsare, emittere, conferre. */
interior i32
_totum (
    constans character* fons,
                   i32  mensura)
{
                      Piscina* piscina;
                 MateriaNodus* radix;
             MateriaScriptura  emissa;
    MateriaScripturaConsilium  consilium;
                CrustaParsura  relatio;
                          i32  fructus;

    piscina  = piscina_generare_dynamicum("totalitas_casus", 65536);
    radix    = crusta_arbor_parsare(piscina, fons, mensura,
        &CRUSTA_BASH,
        &relatio);
    si (radix == NIHIL)
    {
        piscina_destruere(piscina);
        redde (i32)TOTUM_NIHIL;
    }
    /* concordia diagnosticorum (B3 plani materia-sedes): casus
     * generatus quisque numeros parsatoris et diagnostica derivata
     * congruentia habeat - sine exemptione nominata */
    {
        CrustaConcordia concordia;

        (vacuum)crusta_diagnostica_concordia(piscina, radix, &relatio,
            &concordia);
        CREDO_VERUM (concordia.concordat);
        si (!concordia.concordat)
        {
            imprimere("    DISCORS diagnosticorum (%s)\n",
                concordia.causa != NIHIL ? concordia.causa : "-");
        }
    }
    materia_scriptura_consilium_nudum(&consilium, &CRUSTA_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    si (!emissa.successus)
    {
        fructus = (i32)TOTUM_EMISSIO_FRACTA;
    }
    alioquin si (   emissa.textus.mensura != mensura
                 || (mensura > ZEPHYRUM
                     && memcmp(emissa.textus.datum, fons,
                            (size_t)mensura) != ZEPHYRUM))
    {
        fructus = (i32)TOTUM_DISPAR;
    }
    alioquin si (relatio.heredoca_transposita > ZEPHYRUM)
    {
        /* P9b: nulla lacuna sine lista - numeratio ipsa fractura */
        fructus = (i32)TOTUM_TRANSPOSITUM;
    }
    alioquin
    {
        fructus = (i32)TOTUM_IDEM;
    }
    piscina_destruere(piscina);
    redde fructus;
}

/* Parsura SOLA (profunditas: aedificator iterativus) */
interior i32
_parsura_sola (
    constans character* fons,
                   i32  mensura)
{
         Piscina* piscina;
    MateriaNodus* radix;
             i32  fructus;

    piscina  = piscina_generare_dynamicum("totalitas_parsura", 1048576);
    radix    = crusta_arbor_parsare(piscina, fons, mensura,
        &CRUSTA_BASH,
        NIHIL);
    fructus  = (radix == NIHIL) ? (i32)TOTUM_NIHIL : (i32)TOTUM_IDEM;
    piscina_destruere(piscina);
    redde fructus;
}

/* Parsura + proiectio STML; VERUM si scriptura successit */
interior b32
_proiectio (
    constans character* fons,
                   i32  mensura)
{
                  Piscina* piscina;
             MateriaNodus* radix;
      MateriaLexiconRatum  ratum;
       MateriaLexIudicium  iudicium;
    MateriaArborConsilium  consilium;
    MateriaArborScriptura  s;
                      b32  fructus = FALSUM;

    piscina = piscina_generare_dynamicum("totalitas_stml", 1048576);
    radix   = crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH,
        NIHIL);
    si (   radix != NIHIL
        && materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON,
               &iudicium))
    {
        materia_arbor_consilium_nudum(&consilium, &CRUSTA_REGISTRUM,
            &ratum, "crusta");
        s = materia_arbor_scribere_nodum(piscina, radix, &consilium);
        fructus = s.successus;
    }
    piscina_destruere(piscina);
    redde fructus;
}

/* '((' + '1+(' x profunditas + '1' + ')' x profunditas + '))':
 * parsatur, aestimatur; VERUM si valor == profunditas + I */
interior b32
_aestimatio (
    i32 profunditas)
{
    Piscina* piscina =
        piscina_generare_dynamicum("totalitas_aestimatio",
        1048576);
               i32  mensura = profunditas * IV + V;
         character* fons;
               i32  i;
      MateriaNodus* radix;
      MateriaNodus* arithmetica;
               s64  valor    = ZEPHYRUM;
               b32  fructus  = FALSUM;

    fons = (character*)piscina_allocare(piscina,
        (memoriae_index)mensura + I);
    memcpy(fons, "((", II);
    per (i = ZEPHYRUM; i < profunditas; i++)
    {
        memcpy(fons + II + i * III, "1+(", III);
    }
    fons[II + profunditas * III] = '1';
    per (i = ZEPHYRUM; i < profunditas; i++)
    {
        fons[III + profunditas * III + i] = ')';
    }
    memcpy(fons + III + profunditas * IV, "))", II);
    radix = crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH,
        NIHIL);
    si (   radix != NIHIL
        && radix->loci[CRUSTA_PROGRAMMA_LIBERI].genus
               == MATERIA_VALOR_LISTA)
    {
        arithmetica = materia_valor_lista_obtinere(
            radix->loci[CRUSTA_PROGRAMMA_LIBERI],
            ZEPHYRUM)->datum.nodus;
        si (   arithmetica->genus == (s32)CRUSTA_GENUS_ARITHMETICA
            && arithmetica->loci[CRUSTA_ARITHMETICA_EXPRESSIO].genus
                   == MATERIA_VALOR_NODUS)
        {
            constans MateriaValor* locus =
                &arithmetica->loci[CRUSTA_ARITHMETICA_EXPRESSIO];

            fructus = crusta_arithmetica_aestimare(locus->datum.nodus,
                &valor)
                && valor == (s64)profunditas + (s64)I;
        }
    }
    piscina_destruere(piscina);
    redde fructus;
}

interior vacuum
_fractum_scribere (
    constans character* fons,
                   i32  mensura)
{
    character via[DXII];
    FILE* f;

    sprintf(via, "%s/crusta/build/totalitas_fractum.sh", RADIX_VIAE);
    f = fopen(via, "wb");
    si (f == NIHIL)
    {
        redde;
    }
    fwrite(fons, I, (size_t)mensura, f);
    fclose(f);
    imprimere("    fons peccans scriptus: %s\n", via);
}

/* Furca prima (ruina/gyrus), deinde iudicium in parente. */
interior vacuum
_casum_probare (
    constans character* titulus,
    constans character* fons,
                   i32  mensura,
                   i32* numerator)
{
    i32 fructus;

    CREDO_NON_RUIT (_totum(fons, mensura));

    fructus = _totum(fons, mensura);
    si (fructus != (i32)TOTUM_IDEM)
    {
        imprimere("  FRACTUM %s (%d octeti): %s\n", titulus,
            (integer)mensura, CAUSAE[fructus]);
        _fractum_scribere(fons, mensura);
    }
    CREDO_AEQUALIS_I32 (fructus, (i32)TOTUM_IDEM);
    *numerator = *numerator + I;
}

/* LCG classicus; i32 INSIGNATUS est, circumvolutio definita. */
interior character
_fortuitus (
    i32* status)
{
    *status = *status * (i32)1103515245 + (i32)12345;
    redde (character)((*status >> 16) & (i32)0xFF);
}

nomen structura {
    constans character* titulus;
    constans character* apertura;
    constans character* medium;
    constans character* clausura;
} FormaNidi;

hic_manens constans FormaNidi FORMAE[] = {
    { "substitutio", "$(",               "a",    ")"         },
    { "apices",      "\"$(",             "a",    ")\""       },
    { "arithmetica", "((",               "1",    "))"        },
    { "grex",        "{ ",               "a;",   " }"        },
    { "conditio",    "if a; then ",      "b",    "; fi"      },
    { "iteratio",    "for i in a; do ",  ":",    "; done"    },
    { "electio",     "case x in a) ",    ":",    " ;; esac"  },
    { "processus",   "cat <(",           "a",    ")"         },
    { "expansio",    "${x:-",            "a",    "}"         },
    { "gravis",      "`",                "a",    "`"         },
    { "heredoc",     "cat <<A\n",        "x\n",  "A\n"       }
};

#define NUMERUS_FORMARUM ((i32)(magnitudo(FORMAE)/magnitudo(FORMAE[0])))

/* apertura x profunditas [+ medium + clausura x profunditas] */
interior character*
_nidum_struere (
               Piscina* piscina,
    constans FormaNidi* forma,
                   i32  profunditas,
                   b32  clausum,
                   i32* mensura)
{
          i32  la             = (i32)strlen(forma->apertura);
          i32  mensura_medii  = (i32)strlen(forma->medium);
          i32  lc             = (i32)strlen(forma->clausura);
          i32  summa;
          i32  i;
    character* textus;
    character* cursor;

    summa = la * profunditas;
    si (clausum)
    {
        summa = summa + mensura_medii + lc * profunditas;
    }
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)summa + I);
    cursor = textus;
    per (i = ZEPHYRUM; i < profunditas; i++)
    {
        memcpy(cursor, forma->apertura, (size_t)la);
        cursor = cursor + la;
    }
    si (clausum)
    {
        memcpy(cursor, forma->medium, (size_t)mensura_medii);
        cursor = cursor + mensura_medii;
        per (i = ZEPHYRUM; i < profunditas; i++)
        {
            memcpy(cursor, forma->clausura, (size_t)lc);
            cursor = cursor + lc;
        }
    }
    *mensura = summa;
    redde textus;
}

hic_manens constans i32 PROFUNDITATES[] = { I, X, C, M };

/* forma per titulum */
interior constans FormaNidi*
_forma (
    constans character* titulus)
{
    i32 f;

    per (f = ZEPHYRUM; f < NUMERUS_FORMARUM; f++)
    {
        si (strcmp(FORMAE[f].titulus, titulus) == ZEPHYRUM)
        {
            redde &FORMAE[f];
        }
    }
    redde NIHIL;
}

/* plagulam per mutationem et truncationem ducere */
interior vacuum
_plagulam_vexare (
               Piscina* piscina,
    constans character* via,
                   i32* mutati,
                   i32* truncati,
                   i32* lectae)
{
    character* textus;
    character  titulus[DXII];
          i32  mensura = ZEPHYRUM;
          i32  semen;
          i32  gradus;

    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        imprimere("  ABEST: %s\n", via);
        CREDO_CULPA ("plagula corporis abest");
        redde;
    }
    *lectae = *lectae + I;
    per (semen = I; semen <= IV; semen++)
    {
              i32  status  = semen * (i32)7919;
              i32  ictus   = mensura / XL;
        character* mutatum = (character*)piscina_allocare(piscina,
            (memoriae_index)mensura + I);
        i32 k;

        memcpy(mutatum, textus, (size_t)mensura);
        per (k = ZEPHYRUM; k < ictus; k++)
        {
            i32 sedes;

            _fortuitus(&status);
            sedes = (status >> 8) % (i32)(mensura
                > ZEPHYRUM ? mensura : I);
            mutatum[sedes] = _fortuitus(&status);
        }
        sprintf(titulus, "mutatum %s semen=%d", via, (integer)semen);
        _casum_probare(titulus, mutatum, mensura, mutati);
    }
    per (gradus = I; gradus < XXIV; gradus++)
    {
        i32 mensura_trunca = (i32)((s64)mensura * gradus / XXIV);

        sprintf(titulus, "truncatum %s @%d/XXIV", via, (integer)gradus);
        _casum_probare(titulus, textus, mensura_trunca, truncati);
    }
}

s32
principale (vacuum)
{
               Piscina* piscina;
    constans character* radix_viae;
                   b32  praeteritus;
                   i32  plagulae_lectae  = ZEPHYRUM;
                   i32  casus_fortuiti   = ZEPHYRUM;
                   i32  casus_mutati     = ZEPHYRUM;
                   i32  casus_truncati   = ZEPHYRUM;
                   i32  casus_nidorum    = ZEPHYRUM;
                   i32  casus_crlf       = ZEPHYRUM;
                   i32  i;

    piscina = piscina_generare_dynamicum("probatio_crusta_totalitas",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae != NIHIL)
    {
        RADIX_VIAE = radix_viae;
    }


    /* ==================================================
     * OCTETI FORTUITI: semina I..XXXII, LXIV..MMXLVIII octeti
     * ================================================== */

    imprimere("\n--- Probans octetos fortuitos ---\n");
    per (i = I; i <= XXXII; i++)
    {
              i32  status   = i;
              i32  mensura  = (i32)LXIV * i;
        character* fons = (character*)piscina_allocare(piscina,
            (memoriae_index)mensura);
        character titulus[LXIV];
              i32 j;

        per (j = ZEPHYRUM; j < mensura; j++)
        {
            fons[j] = _fortuitus(&status);
        }
        sprintf(titulus, "fortuiti semen=%d", (integer)i);
        _casum_probare(titulus, fons, mensura, &casus_fortuiti);
    }
    imprimere("  casus %d\n", (integer)casus_fortuiti);


    /* ==================================================
     * CORPUS MUTATUM + TRUNCATUM: fixa, FreeBSD, domus XII
     * ================================================== */

    imprimere("\n--- Probans corpus mutatum et truncatum ---\n");
    _plagulam_vexare(piscina, "probationes/fixa/crusta/pathologiae.sh",
        &casus_mutati, &casus_truncati, &plagulae_lectae);
    _plagulam_vexare(piscina, "probationes/fixa/crusta/adversarius.sh",
        &casus_mutati, &casus_truncati, &plagulae_lectae);
    per (i = ZEPHYRUM; CRUSTA_FREEBSD_PLAGULAE[i] != NIHIL; i++)
    {
        _plagulam_vexare(piscina, CRUSTA_FREEBSD_PLAGULAE[i],
            &casus_mutati, &casus_truncati, &plagulae_lectae);
    }
    per (i = ZEPHYRUM; i < NUMERUS_DOMUS; i++)
    {
        _plagulam_vexare(piscina, DOMUS[i], &casus_mutati,
            &casus_truncati, &plagulae_lectae);
    }
    imprimere("  plagulae %d, mutati %d, truncati %d\n",
        (integer)plagulae_lectae, (integer)casus_mutati,
        (integer)casus_truncati);
    CREDO_AEQUALIS_I32 (plagulae_lectae, (i32)II + (i32)LXXXIX
        + NUMERUS_DOMUS);


    /* ==================================================
     * NIDIFICATIO: formae XI x {clausa, aperta} x {I, X, C, M}
     * ================================================== */

    imprimere("\n--- Probans nidificationem ---\n");
    {
        i32 f;
        i32 c;
        i32 d;

        per (f = ZEPHYRUM; f < NUMERUS_FORMARUM; f++)
        {
            per (c = ZEPHYRUM; c < II; c++)
            {
                per (d = ZEPHYRUM; d < IV; d++)
                {
                    character  titulus[LXIV];
                    character* fons;
                          i32  mensura = ZEPHYRUM;

                    fons = _nidum_struere(piscina, &FORMAE[f],
                        PROFUNDITATES[d], (b32)(c == ZEPHYRUM),
                        &mensura);
                    sprintf(titulus, "nidus %s %s x%d",
                        FORMAE[f].titulus,
                        c == ZEPHYRUM ? "clausus" : "apertus",
                        (integer)PROFUNDITATES[d]);
                    _casum_probare(titulus, fons, mensura,
                        &casus_nidorum);
                }
            }
        }
        imprimere("  casus %d\n", (integer)casus_nidorum);
    }


    /* ==================================================
     * CRLF: adversarius totus, casus pathologiarum conversi
     * ================================================== */

    imprimere("\n--- Probans CRLF ---\n");
    {
                       i32  mensura = ZEPHYRUM;
        constans character* fons;
                       Xar* exempla;

        fons = _plagulam_legere(piscina,
            "probationes/fixa/crusta/adversarius.sh", &mensura);
        CREDO_NON_NIHIL (fons);
        si (fons != NIHIL)
        {
            _casum_probare("adversarius totus", fons, mensura,
                &casus_crlf);
        }
        fons = _plagulam_legere(piscina,
            "probationes/fixa/crusta/pathologiae.sh", &mensura);
        exempla = fons ? crusta_exempla_legere(piscina, fons, mensura)
                       : NIHIL;
        CREDO_NON_NIHIL (exempla);
        per (i = ZEPHYRUM; exempla != NIHIL && i < xar_numerus(exempla);
             i++)
        {
            constans CrustaExemplum* e =
                (constans CrustaExemplum*)xar_obtinere(exempla, i);
            character* conversum = (character*)piscina_allocare(piscina,
                (memoriae_index)e->datum.mensura * II + I);
                  i32 n = ZEPHYRUM;
                  i32 j;
            character titulus[CXXVIII];

            per (j = ZEPHYRUM; j < e->datum.mensura; j++)
            {
                si (   e->datum.datum[j] == '\n'
                    && (j == ZEPHYRUM || e->datum.datum[j - I] != '\r'))
                {
                    conversum[n++] = '\r';
                }
                conversum[n++] = (character)e->datum.datum[j];
            }
            sprintf(titulus, "crlf pathologiae casus %d",
                (integer)e->numerus);
            _casum_probare(titulus, conversum, n, &casus_crlf);
        }
        imprimere("  casus %d\n", (integer)casus_crlf);
        CREDO_AEQUALIS_I32 (casus_crlf, (i32)XXXVII);
    }


    /* ==================================================
     * NUL: parsatur, emittitur; STML nominatim recusat
     * ================================================== */

    imprimere("\n--- Probans NUL ---\n");
    {
        nomen structura {
            constans character* fons;
                           i32  mensura;
            constans character* causa;
        } CasusOcteti;
        hic_manens constans CasusOcteti NULLA[] = {
            { "echo a\0b\n", 9, "valor lexematis NUL fert" },
            { "cat <<A\nx\0y\nA\n", 14, "valor lexematis NUL fert" },
            { "a # c\0d\n", 8, "valor trivii NUL fert" }
        };
          MateriaLexiconRatum ratum;
           MateriaLexIudicium iud;
        MateriaArborConsilium consilium;
                          i32 k;

        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum,
            &CRUSTA_LEXICON,
            &iud));
        materia_arbor_consilium_nudum(&consilium, &CRUSTA_REGISTRUM,
            &ratum, "crusta");
        per (k = ZEPHYRUM; k < III; k++)
        {
                     MateriaNodus* radix;
            MateriaArborScriptura  s;

            CREDO_NON_RUIT (_totum(NULLA[k].fons, NULLA[k].mensura));
            CREDO_AEQUALIS_I32 (_totum(NULLA[k].fons, NULLA[k].mensura),
                (i32)TOTUM_IDEM);
            radix = crusta_arbor_parsare(piscina, NULLA[k].fons,
                NULLA[k].mensura, &CRUSTA_BASH, NIHIL);
            CREDO_NON_NIHIL (radix);
            si (radix == NIHIL)
            {
                perge;
            }
            s = materia_arbor_scribere_nodum(piscina, radix,
                &consilium);
            CREDO_FALSUM (s.successus);
            CREDO_NON_NIHIL (s.causa);
            si (s.causa != NIHIL)
            {
                imprimere("  refusio nominata: %s\n", s.causa);
                CREDO_VERUM (strcmp(s.causa, NULLA[k].causa)
                    == ZEPHYRUM);
            }
        }
    }


    /* ==================================================
     * PROFUNDITAS: parsura sola C milia (aedificator iterativus)
     * ================================================== */

    imprimere("\n--- Profunditas: parsura sola C milia ---\n");
    {
        nomen structura {
            constans character* forma;
                           b32  clausum;
        } CasusProfundus;
        hic_manens constans CasusProfundus PROFUNDI[] = {
            { "substitutio", FALSUM },
            { "substitutio", VERUM  },
            { "arithmetica", FALSUM },
            { "apices",      FALSUM },
            { "grex",        FALSUM },
            { "conditio",    FALSUM }
        };
        i32 k;

        per (k = ZEPHYRUM; k < VI; k++)
        {
                  i32  mensura = ZEPHYRUM;
            character* fons = _nidum_struere(piscina,
                _forma(PROFUNDI[k].forma), (i32)100000,
                PROFUNDI[k].clausum, &mensura);

            imprimere("  %s %s\n", PROFUNDI[k].forma,
                PROFUNDI[k].clausum ? "clausa" : "aperta");
            CREDO_NON_RUIT (_parsura_sola(fons, mensura));
            CREDO_AEQUALIS_I32 (_parsura_sola(fons, mensura),
                (i32)TOTUM_IDEM);
        }
    }


    /* ==================================================
     * PROFUNDITAS: emissio (materia_scribere_nodum, 01M1FAD8)
     * ================================================== */

    imprimere("\n--- Profunditas: emissio XL milia vivit ---\n");
    {
              i32  mensura = ZEPHYRUM;
        character* fons = _nidum_struere(piscina, _forma("grex"),
            (i32)40000, VERUM, &mensura);

        CREDO_NON_RUIT (_totum(fons, mensura));
        CREDO_AEQUALIS_I32 (_totum(fons, mensura), (i32)TOTUM_IDEM);
    }

    imprimere("\n--- Pinna: emissio '$(' C milia SIGSEGV ---\n");
    {
              i32  mensura = ZEPHYRUM;
        character* fons = _nidum_struere(piscina, _forma("substitutio"),
            (i32)100000, VERUM, &mensura);

        /* parsura eiusdem fontis supra vivit; ruina SUBSTRATI. Rubet
         * cum remedium veniat (01M1FAD8) - tunc in NON_RUIT. */
        CREDO_RUIT_CUM (_totum(fons, mensura), SIGSEGV);
    }


    /* ==================================================
     * PROFUNDITAS: proiectio STML
     * ================================================== */

    imprimere("\n--- Profunditas: proiectio STML D vivit ---\n");
    {
              i32  mensura = ZEPHYRUM;
        character* fons = _nidum_struere(piscina, _forma("grex"),
            (i32)D,
            VERUM, &mensura);

        CREDO_NON_RUIT (_proiectio(fons, mensura));
        CREDO_VERUM (_proiectio(fons, mensura));
    }

    imprimere("\n--- Pinna: proiectio STML C milia SIGSEGV ---\n");
    {
              i32  mensura = ZEPHYRUM;
        character* fons = _nidum_struere(piscina, _forma("grex"),
            (i32)100000, VERUM, &mensura);

        CREDO_RUIT_CUM (_proiectio(fons, mensura), SIGSEGV);
    }


    /* ==================================================
     * PROFUNDITAS: aestimator constans (recursivus, P4)
     * ================================================== */

    imprimere("\n--- Profunditas: aestimator C milia vivit ---\n");
    CREDO_NON_RUIT (_aestimatio((i32)100000));
    CREDO_VERUM (_aestimatio((i32)100000));

    imprimere("\n--- Pinna: aestimator CC milia SIGSEGV ---\n");
    CREDO_RUIT_CUM (_aestimatio((i32)200000), SIGSEGV);


    /* Tegumentum SUUM: classis vacua rubet */
    CREDO_MAIOR_I32 (casus_fortuiti, ZEPHYRUM);
    CREDO_MAIOR_I32 (casus_mutati, ZEPHYRUM);
    CREDO_MAIOR_I32 (casus_truncati, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (casus_nidorum, NUMERUS_FORMARUM * (i32)VIII);
    imprimere("\n  summa casuum: %d\n", (integer)(casus_fortuiti
        + casus_mutati + casus_truncati + casus_nidorum + casus_crlf));
    CREDO_MAIOR_I32 (casus_fortuiti + casus_mutati + casus_truncati
        + casus_nidorum + casus_crlf, (i32)MM);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
