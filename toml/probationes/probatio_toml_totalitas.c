/* probatio_toml_totalitas.c - Porta totalitatis toml (Q11a)
 *
 * CONTRACTUS materiae (Fran, 2026-09-01): (a) NUMQUAM RUIT pro quolibet
 * fonte; (b) SEMPER arborem reddit, et emissio == fons octetim.
 * Exemplar probatio_crusta_totalitas (P9). Toml addit duas leges per
 * casum omnem:
 *   (c) parsura non sana -> diagnosticum saltem unum (lex Q7b: numerus
 *       diagnosticum celabat - chorda aperta);
 *   (d) coctio cum parsura (derivatio omissa si sana) == coctio sine
 *       parsura (derivatio semper): numerus diagnosticorum idem. Hoc
 *       omissionem Q11 invisibilem esse probat.
 * Classes: octeti fortuiti (semina I..XXXII), corpus mutatum (I ex XL
 * octetis, semina IV) et truncatum (XXIII gradus) super toml-test
 * valida et domum; nidi (formae V x clausa/aperta x I..M); CRLF (valida
 * omnia conversa manent valida); NUL (octeti, STML circuitus, refusio
 * NOMINATA).
 *
 * PROFUNDITAS, MENSURATA 2026-09-28 (a = [ x N; vide planum Q11):
 *   parsura          C milia < 0,2 s (olim XXVIII s: numeratio uncorum
 *                    per lexema acervum totum ambulabat - emendatum)
 *   coctio           C milia < 0,4 s (olim SIGSEGV: recursio - nunc
 *                    acervus operum)
 *   derivatio        quadratica (materia): XL milia IV s; aperta
 *                    (non sana) solum eam vocat - X milia hic
 *   emissio          XL milia VIVIT, C milia SIGSEGV (materia, …FAD8)
 *   proiectio STML   D VIVIT, C milia SIGSEGV (materia); X milia > LX s
 *                    (tempus, non ruina - non pinnatum)
 * Pinnae ruinae (RUIT_CUM) rubent cum materia emendetur - tunc in
 * NON_RUIT promovendae.
 *
 * IN FRACTURA: fons peccans in toml/build/totalitas_fractum.toml.
 */

#include "latina.h"
#include "credo.h"
#include "toml_arbor.h"
#include "toml_coctum.h"
#include "toml_scalaris.h"
#include "toml_lexicon.h"
#include "toml_registrum.h"
#include "toml_corpus_ambulare.h"
#include "materia_arbor.h"
#include "materia_diagnostica.h"
#include "materia_lexicon.h"
#include "materia_scribere.h"
#include "piscina.h"
#include "sors.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enumeratio {
    TOTUM_IDEM = 0,
    TOTUM_NIHIL,
    TOTUM_EMISSIO_FRACTA,
    TOTUM_DISPAR,
    TOTUM_TACITUM,
    TOTUM_DISCORS
};

hic_manens constans character* CAUSAE[] = {
    "idem",
    "parsator NIHIL reddidit",
    "emissio fracta",
    "emissio a fonte dispar",
    "parsura non sana sine diagnostico",
    "coctio cum parsura et sine ea discordant (derivatio omissa)"
};

hic_manens constans character* RADIX_VIAE = ".";


/* ==================================================
 * Casus
 * ================================================== */

/* Casus unus, piscina SUA: parsare, emittere, conferre, coquere bis. */
interior i32
_totum (
    constans character* fons,
                   i32  mensura)
{
                      Piscina* piscina;
                 MateriaNodus* radix;
             MateriaScriptura  emissa;
    MateriaScripturaConsilium  consilium;
                  TomlParsura  r;
                   TomlCoctum  cum;
                   TomlCoctum  sine;
                          i32  fructus;

    piscina  = piscina_generare_dynamicum("totalitas_casus", 65536);
    radix    = toml_arbor_parsare(piscina, fons, (s32)mensura, &r);
    si (radix == NIHIL)
    {
        piscina_destruere(piscina);
        redde (i32)TOTUM_NIHIL;
    }
    materia_scriptura_consilium_nudum(&consilium, &TOML_REGISTRUM);
    emissa  = materia_scribere_nodum(piscina, radix, &consilium);
    cum     = toml_coquere(piscina, radix, &r);
    sine    = toml_coquere(piscina, radix, NIHIL);
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
    alioquin si (!r.sana && xar_numerus(cum.diagnostica) == ZEPHYRUM)
    {
        fructus = (i32)TOTUM_TACITUM;
    }
    alioquin si (   xar_numerus(cum.diagnostica)
                 != xar_numerus(sine.diagnostica))
    {
        fructus = (i32)TOTUM_DISCORS;
    }
    alioquin
    {
        fructus = (i32)TOTUM_IDEM;
    }
    piscina_destruere(piscina);
    redde fructus;
}

/* Parsura SOLA */
interior i32
_parsura_sola (
    constans character* fons,
                   i32  mensura)
{
         Piscina* piscina =
             piscina_generare_dynamicum("totalitas_parsura",
                               1048576);
    MateriaNodus* radix   = toml_arbor_parsare(piscina, fons,
                               (s32)mensura, NIHIL);
             i32 fructus = radix == NIHIL ? (i32)TOTUM_NIHIL
                               : (i32)TOTUM_IDEM;

    piscina_destruere(piscina);
    redde fructus;
}

/* Parsura + coctio (sine emissione): VERUM si radix coctus est */
interior b32
_coctio (
    constans character* fons,
                   i32  mensura)
{
         Piscina* piscina =
             piscina_generare_dynamicum("totalitas_coctio",
                               1048576);
      TomlParsura  r;
     MateriaNodus* radix   = toml_arbor_parsare(piscina, fons,
                               (s32)mensura, &r);
       TomlCoctum c;
              b32 fructus = FALSUM;

    si (radix != NIHIL)
    {
        c        = toml_coquere(piscina, radix, &r);
        fructus  = c.radix != NIHIL && c.diagnostica != NIHIL;
    }
    piscina_destruere(piscina);
    redde fructus;
}

/* Parsura + proiectio STML: VERUM si scriptura successit */
interior b32
_proiectio (
    constans character* fons,
                   i32  mensura)
{
                  Piscina* piscina = piscina_generare_dynamicum(
                                         "totalitas_stml", 1048576);
             MateriaNodus* radix   = toml_arbor_parsare(piscina, fons,
                                         (s32)mensura, NIHIL);
      MateriaLexiconRatum ratum;
       MateriaLexIudicium iudicium;
    MateriaArborConsilium consilium;
    MateriaArborScriptura s;
                      b32 fructus = FALSUM;

    si (   radix != NIHIL
        && materia_lexicon_ratum_facere(&ratum, &TOML_LEXICON,
        &iudicium))
    {
        materia_arbor_consilium_nudum(&consilium, &TOML_REGISTRUM,
            &ratum,
            "toml");
        s = materia_arbor_scribere_nodum(piscina, radix,
            &consilium);
        fructus = s.successus;
    }
    piscina_destruere(piscina);
    redde fructus;
}

/* chorda (non nul-terminata) acum continet? */
interior b32
_continet (
                 chorda  c,
     constans character* acus)
{
    i32 n = (i32)strlen(acus);
    i32 k;

    per (k = ZEPHYRUM; k + n <= c.mensura; k++)
    {
        si (memcmp(c.datum + k, acus, (size_t)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_fractum_scribere (
    constans character* fons,
                   i32  mensura)
{
    character via[DXII];
        FILE* f;

    sprintf(via, "%s/toml/build/totalitas_fractum.toml", RADIX_VIAE);
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


/* ==================================================
 * Nidi
 * ================================================== */

nomen structura {
    constans character* titulus;
    constans character* praefixum;
    constans character* apertura;
    constans character* medium;
    constans character* clausura;
} FormaNidi;

hic_manens constans FormaNidi FORMAE[] = {
    { "series",   "a = ", "[",   "1",       "]"  },
    { "compacta", "x = ", "{a=", "1",       "}"  },
    { "mixta",    "x = ", "[{a=", "1",      "}]" },
    { "punctata", "",     "a.",  "b = 1",   ""   },
    { "caput",    "[a",   ".a",  "]\nb = 1", ""  }
};

#define NUMERUS_FORMARUM ((i32)(magnitudo(FORMAE)/magnitudo(FORMAE[0])))

/* praefixum + apertura x profunditas [+ medium + clausura x prof.] */
interior character*
_nidum_struere (
               Piscina* piscina,
    constans FormaNidi* forma,
                   i32  profunditas,
                   b32  clausum,
                   i32* mensura)
{
          i32  lp             = (i32)strlen(forma->praefixum);
          i32  la             = (i32)strlen(forma->apertura);
          i32  mensura_medii  = (i32)strlen(forma->medium);
          i32  lc             = (i32)strlen(forma->clausura);
          i32  summa          = lp + la * profunditas;
          i32  i;
    character* textus;
    character* cursor;

    si (clausum)
    {
        summa = summa + mensura_medii + lc * profunditas;
    }
    textus = (character*)piscina_allocare(piscina, (i64)summa + I);
    cursor = textus;
    memcpy(cursor, forma->praefixum, (size_t)lp);
    cursor += lp;
    per (i = ZEPHYRUM; i < profunditas; i++)
    {
        memcpy(cursor, forma->apertura, (size_t)la);
        cursor += la;
    }
    si (clausum)
    {
        memcpy(cursor, forma->medium, (size_t)mensura_medii);
        cursor += mensura_medii;
        per (i = ZEPHYRUM; i < profunditas; i++)
        {
            memcpy(cursor, forma->clausura, (size_t)lc);
            cursor += lc;
        }
    }
    *cursor   = '\0';
    *mensura  = summa;
    redde textus;
}

hic_manens constans i32 PROFUNDITATES[] = { I, X, C, M };


/* ==================================================
 * Corpus vexatum
 * ================================================== */

nomen structura {
    i32 lectae;
    i32 mutati;
    i32 truncati;
    i32 crlf;
    i32 crlf_insana;
} Vexatio;

interior vacuum
_visor (
                 vacuum* datum,
         TomlCorpusFons  fons,
     constans character* via,
                 chorda  textus,
                Piscina* opus)
{
      Vexatio* vexatio = (Vexatio*)datum;
          i32  mensura = (i32)textus.mensura;
    character  titulus[DXII];
          i32  semen;
          i32  gradus;
    character* copia;

    si (   fons == TOML_CORPUS_SILVESTRIA
        || (fons == TOML_CORPUS_TOML_TEST
            && strncmp(via, "valid/", VI) != ZEPHYRUM))
    {
        redde;
    }
    vexatio->lectae++;
    copia = (character*)piscina_allocare(opus, (i64)mensura + I);
    si (mensura > ZEPHYRUM)
    {
        memcpy(copia, textus.datum, (size_t)mensura);
    }
    copia[mensura] = '\0';

    /* mutatio: I ex XL octetis, semina IV */
    per (semen = I; semen <= IV; semen++)
    {
             Sors  s;
              i32  ictus   = mensura / XL;
        character* mutatum = (character*)piscina_allocare(opus,
                                 (i64)mensura + I);
              i32 k;

        sors_seminare(&s, (i64)semen, (i64)I);
        memcpy(mutatum, copia, (size_t)mensura + I);
        per (k = ZEPHYRUM; k < ictus; k++)
        {
            i32 sedes = sors_intra(&s, mensura
                > ZEPHYRUM ? mensura : I);

            mutatum[sedes] = (character)sors_intra(&s, CCLVI);
        }
        sprintf(titulus, "mutatum %s semen=%d", via, (integer)semen);
        _casum_probare(titulus, mutatum, mensura, &vexatio->mutati);
    }
    /* truncatio: XXIII gradus */
    per (gradus = I; gradus < XXIV; gradus++)
    {
        i32 trunca = (i32)((s64)mensura * gradus / XXIV);

        sprintf(titulus, "truncatum %s @%d/XXIV", via, (integer)gradus);
        _casum_probare(titulus, copia, trunca, &vexatio->truncati);
    }
    /* CRLF: validum conversum manet validum (et octetim idem) */
    si (fons == TOML_CORPUS_TOML_TEST)
    {
        character* conversum = (character*)piscina_allocare(opus,
                                   (i64)mensura * II + I);
              i32  n = ZEPHYRUM;
              i32  j;
          Piscina* p;
     MateriaNodus* radix;
       TomlParsura r;
        TomlCoctum c;

        per (j = ZEPHYRUM; j < mensura; j++)
        {
            si (   copia[j] == '\n'
                && (j == ZEPHYRUM || copia[j - I] != '\r'))
            {
                conversum[n++] = '\r';
            }
            conversum[n++] = copia[j];
        }
        sprintf(titulus, "crlf %s", via);
        _casum_probare(titulus, conversum, n, &vexatio->crlf);
        p      = piscina_generare_dynamicum("totalitas_crlf", 65536);
        radix  = toml_arbor_parsare(p, conversum, (s32)n, &r);
        c      = toml_coquere(p, radix, &r);
        si (!c.sanum)
        {
            vexatio->crlf_insana++;
            imprimere("  CRLF INSANUM %s\n", via);
        }
        piscina_destruere(p);
    }
}


/* ==================================================
 * Principale
 * ================================================== */

s32
principale (vacuum)
{
               Piscina* piscina;
                   b32  praeteritus;
                   i32  casus_fortuiti  = ZEPHYRUM;
                   i32  casus_nidorum   = ZEPHYRUM;
               Vexatio  vexatio;
      TomlCorpusNumeri  nn;
                   i32  i;
    constans character* radix_viae = getenv("RHUBARB_RADIX");

    piscina = piscina_generare_dynamicum("probatio_toml_totalitas",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    si (radix_viae != NIHIL)
    {
        RADIX_VIAE = radix_viae;
    }


    /* ==================================================
     * Octeti fortuiti: semina I..XXXII, LXIV..MMXLVIII octeti
     * ================================================== */

    imprimere("\n--- Probans octetos fortuitos ---\n");
    per (i = I; i <= XXXII; i++)
    {
             Sors  s;
              i32  mensura = (i32)LXIV * i;
        character* fons    = (character*)piscina_allocare(piscina,
                                 (i64)mensura);
        character titulus[LXIV];
              i32 j;

        sors_seminare(&s, (i64)i, (i64)ZEPHYRUM);
        per (j = ZEPHYRUM; j < mensura; j++)
        {
            fons[j] = (character)sors_intra(&s, CCLVI);
        }
        sprintf(titulus, "fortuiti semen=%d", (integer)i);
        _casum_probare(titulus, fons, mensura, &casus_fortuiti);
    }
    imprimere("  casus %d\n", (integer)casus_fortuiti);


    /* ==================================================
     * Corpus mutatum, truncatum, CRLF (toml-test valida + domus)
     * ================================================== */

    imprimere("\n--- Probans corpus mutatum, truncatum, CRLF ---\n");
    {
        Piscina* opus = piscina_generare_dynamicum("totalitas_opus",
                            1048576);

        memset(&vexatio, ZEPHYRUM, magnitudo(vexatio));
        toml_corpus_ambulare(piscina, opus, RADIX_VIAE, _visor,
            &vexatio,
            &nn);
        piscina_destruere(opus);
    }
    imprimere("  plagulae %d, mutati %d, truncati %d, crlf %d (insana "
        "%d)\n", (integer)vexatio.lectae, (integer)vexatio.mutati,
        (integer)vexatio.truncati, (integer)vexatio.crlf,
        (integer)vexatio.crlf_insana);
    CREDO_VERUM (nn.indices_lecti);
    CREDO_MAIOR_I32 (vexatio.lectae, (i32)CCV);
    CREDO_AEQUALIS_I32 (vexatio.crlf, (i32)CCV);
    CREDO_AEQUALIS_I32 (vexatio.crlf_insana, ZEPHYRUM);


    /* ==================================================
     * Nidi: formae V x {clausa, aperta} x {I, X, C, M}
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
                          i32  mensura = ZEPHYRUM;
                    character* fons = _nidum_struere(piscina,
                        &FORMAE[f],
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
        CREDO_AEQUALIS_I32 (casus_nidorum,
            NUMERUS_FORMARUM * (i32)VIII);
    }


    /* ==================================================
     * NUL: octetim idem, STML circuitus, refusio NOMINATA
     * ================================================== */

    imprimere("\n--- Probans NUL ---\n");
    {
        nomen structura {
            constans character* fons;
                           i32  mensura;
                           s32  sedes;
        } CasusOcteti;
        hic_manens constans CasusOcteti NULLA[] = {
            { "a = \"x\0y\"\n", X,    VI },
            { "a = 'x\0y'\n",   X,    VI },
            { "# c\0d\na = 1\n", XII, III }
        };
          MateriaLexiconRatum ratum;
           MateriaLexIudicium iud;
        MateriaArborConsilium consilium;
                          i32 k;

        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &TOML_LEXICON,
            &iud));
        materia_arbor_consilium_nudum(&consilium, &TOML_REGISTRUM,
            &ratum,
            "toml");
        per (k = ZEPHYRUM; k < III; k++)
        {
                     MateriaNodus* radix;
            MateriaArborScriptura  s;
                      TomlParsura  r;
                       TomlCoctum  c;

            CREDO_NON_RUIT (_totum(NULLA[k].fons, NULLA[k].mensura));
            CREDO_AEQUALIS_I32 (_totum(NULLA[k].fons, NULLA[k].mensura),
                (i32)TOTUM_IDEM);
            radix = toml_arbor_parsare(piscina, NULLA[k].fons,
                (s32)NULLA[k].mensura, &r);
            CREDO_NON_NIHIL (radix);
            si (radix == NIHIL)
            {
                perge;
            }
            /* refusio nominata: octetus moderans ubi NUL iacet */
            c = toml_coquere(piscina, radix, &r);
            CREDO_FALSUM (c.sanum);
            CREDO_AEQUALIS_I32 (xar_numerus(c.diagnostica), I);
            si (xar_numerus(c.diagnostica) == I)
            {
                constans MateriaDiagnosticum* d =
                    (constans MateriaDiagnosticum*)xar_obtinere(
                        c.diagnostica, ZEPHYRUM);

                CREDO_VERUM (strcmp(d->codex,
                    TOML_CODEX_OCTETUS_MODERANS) == ZEPHYRUM);
                CREDO_AEQUALIS_S32 (d->tractus.initium, NULLA[k].sedes);
            }
            /* STML circuitus plenus (attributum 'nul') */
            s = materia_arbor_scribere_nodum(piscina, radix,
                &consilium);
            CREDO_VERUM (s.successus);
            si (!s.successus)
            {
                perge;
            }
            CREDO_VERUM (_continet(s.textus, "nul="));
            {
                        MateriaNodus* relecta;
                  MateriaArborVitium  vitium;
                    MateriaScriptura  emissa;
           MateriaScripturaConsilium  ce;

                relecta = materia_arbor_legere(piscina, NIHIL, s.textus,
                    &consilium, &vitium);
                CREDO_NON_NIHIL (relecta);
                si (relecta == NIHIL)
                {
                    perge;
                }
                materia_scriptura_consilium_nudum(&ce, &TOML_REGISTRUM);
                emissa = materia_scribere_nodum(piscina, relecta, &ce);
                CREDO_VERUM (emissa.successus);
                CREDO_AEQUALIS_I32 (emissa.textus.mensura,
                    NULLA[k].mensura);
                si (emissa.successus)
                {
                    CREDO_VERUM (memcmp(emissa.textus.datum,
                        NULLA[k].fons, (size_t)NULLA[k].mensura)
                        == ZEPHYRUM);
                }
            }
        }
    }


    /* ==================================================
     * Profunditas: parsura et coctio C milia (nostra, iterativa)
     * ================================================== */

    imprimere("\n--- Profunditas: parsura et coctio C milia ---\n");
    {
        i32 f;
        i32 c;

        per (f = ZEPHYRUM; f < NUMERUS_FORMARUM; f++)
        {
            per (c = ZEPHYRUM; c < II; c++)
            {
                      i32  mensura = ZEPHYRUM;
                character* fons = _nidum_struere(piscina, &FORMAE[f],
                    (i32)100000, (b32)(c == ZEPHYRUM), &mensura);

                imprimere("  %s %s\n", FORMAE[f].titulus,
                    c == ZEPHYRUM ? "clausa" : "aperta");
                CREDO_NON_PENDET (_parsura_sola(fons, mensura),
                    (i32)(V * M));
                CREDO_AEQUALIS_I32 (_parsura_sola(fons, mensura),
                    (i32)TOTUM_IDEM);
                si (c == ZEPHYRUM)
                {
                    /* clausa = sana: derivatio omissa */
                    CREDO_NON_PENDET (_coctio(fons, mensura),
                        (i32)(V * M));
                    CREDO_VERUM (_coctio(fons, mensura));
                }
            }
        }
    }

    imprimere("\n--- Profunditas: coctio aperta X milia (derivatio "
        "quadratica, materia) ---\n");
    {
              i32  mensura = ZEPHYRUM;
        character* fons = _nidum_struere(piscina, &FORMAE[ZEPHYRUM],
            (i32)X * M, FALSUM, &mensura);

        CREDO_NON_PENDET (_coctio(fons, mensura), (i32)(V * M));
        CREDO_VERUM (_coctio(fons, mensura));
    }


    /* ==================================================
     * Profunditas: limites materiae (pinnae ruinae)
     * ================================================== */

    imprimere("\n--- Profunditas: emissio XL milia vivit ---\n");
    {
              i32  mensura = ZEPHYRUM;
        character* fons = _nidum_struere(piscina, &FORMAE[ZEPHYRUM],
            (i32)40000, VERUM, &mensura);

        CREDO_NON_RUIT (_totum(fons, mensura));
        CREDO_AEQUALIS_I32 (_totum(fons, mensura), (i32)TOTUM_IDEM);
    }

    imprimere("\n--- Pinna: emissio C milia SIGSEGV "
        "(materia, FAD8) ---\n");
    {
              i32  mensura = ZEPHYRUM;
        character* fons = _nidum_struere(piscina, &FORMAE[ZEPHYRUM],
            (i32)100000, VERUM, &mensura);

        /* parsura et coctio eiusdem fontis supra vivunt; ruina
         * SUBSTRATI. Rubet cum remedium veniat - tunc NON_RUIT. */
        CREDO_RUIT_CUM (_totum(fons, mensura), SIGSEGV);
    }

    imprimere("\n--- Profunditas: proiectio STML D vivit ---\n");
    {
              i32  mensura = ZEPHYRUM;
        character* fons = _nidum_struere(piscina, &FORMAE[ZEPHYRUM],
            (i32)D, VERUM, &mensura);

        CREDO_NON_RUIT (_proiectio(fons, mensura));
        CREDO_VERUM (_proiectio(fons, mensura));
    }

    imprimere("\n--- Pinna: proiectio STML C milia "
        "SIGSEGV (materia) ---\n");
    {
              i32  mensura = ZEPHYRUM;
        character* fons = _nidum_struere(piscina, &FORMAE[ZEPHYRUM],
            (i32)100000, VERUM, &mensura);

        CREDO_RUIT_CUM (_proiectio(fons, mensura), SIGSEGV);
    }


    /* Tegumentum SUUM: classis vacua rubet */
    CREDO_MAIOR_I32 (casus_fortuiti, ZEPHYRUM);
    CREDO_MAIOR_I32 (vexatio.mutati, ZEPHYRUM);
    CREDO_MAIOR_I32 (vexatio.truncati, ZEPHYRUM);
    imprimere("\n  summa casuum: %d\n", (integer)(casus_fortuiti
        + vexatio.mutati + vexatio.truncati + vexatio.crlf
        + casus_nidorum));

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
