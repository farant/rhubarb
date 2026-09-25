/* shim_c89.c - PORTA: emissorem materiae contra SILVAM IPSAM conferre
 *
 * Arborem C89 a silva parsatam in typos materiae vertit, per
 * materia_scribere emittit, et contra emissionem SILVAE ipsius
 * OCTETIM confert.
 *
 * CUR HOC ANTE MATERIA_ARBOR. Porta phasis I ('shim C89 CCLXXXI/CCLXXXI
 * per materiam') circuitum STML poscit, ergo materia_arbor - V,DLXXIII
 * lineas. Sed dimidium OCTETORUM illius portae iam NUNC parari potest,
 * et id ipsum est quod uncos originis probat (radix, extentum,
 * scissurae). Portatio V,DLXXIII linearum consilio improbato inniti
 * NON debet.
 *
 * ORACULUM SEPARANS: silva ipsa iudicat, non fixtura a me scripta.
 * Fixtura sumptiones meas communicat; silva non.
 *
 * SEDES TEMPORARIA. Hoc instrumentum C89 SCIT - materia nescire debet
 * (M8). Ergo hic vivit, non in probationibus materiae, et phasis V eum
 * retirat cum silva materiam consumat (T13).
 *
 * AGITATOR SOLUM ab phasi V T6b: conversio et unci C89 quos hic
 * aedificavimus nunc in silva/fontes/silva_frons.{h,c} vivunt
 * (productio); hic parsare, convertere, emittere, conferre.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_parsare.h"
#include "silva_scribere.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_lexema.h"
#include "materia_token.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_arbor.h"
#include "silva_frons.h"
#include "silva_arbor.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ---------- proba ---------- */

hic_manens i32 PROBATAE = 0;
hic_manens i32 FRACTAE  = 0;
hic_manens i32 STML_IDEM = 0;
hic_manens i32 STML_DISPAR = 0;
hic_manens b32 STML_AGERE = FALSUM;
hic_manens b32 STML_VERBOSE = FALSUM;
hic_manens i32 CIRC_IDEM = 0;
hic_manens i32 CIRC_DISPAR = 0;
hic_manens i32 ARBOR_IDEM = 0;
hic_manens i32 ARBOR_DISPAR = 0;

/* comparatio arboris in circuitu (nomen breve: nidus profundus) */
hic_manens constans MateriaArborComparatioModus COMPARATIO_ARBORIS =
    MATERIA_ARBOR_COMPARATIO_STRUCTURALIS;

hic_manens vacuum
_probare (
    constans character* titulus,
    constans character* fons)
{
                      Piscina* p;
                 SilvaParsura* parsura;
                   SilvaFrons* frons;
               SilvaScriptura  sil;
             MateriaScriptura  mat;
                 MateriaValor  mradix;
    MateriaScripturaConsilium  consilium;

    p = piscina_generare_dynamicum("shim", 1 << 20);
    parsura = silva_c89_parsare(p, "proba.c", fons, (i32)strlen(fons),
        NIHIL);
    si (parsura == NIHIL || !parsura->successus)
    {
        imprimere("  %-28s PARSURA FRACTA\n", titulus);
        FRACTAE++; piscina_destruere(p); redde;
    }

    frons = silva_frons_creare(p, parsura->expansio);
    si (frons == NIHIL)
    {
        imprimere("  %-28s FRONS CREARI NON POTUIT\n", titulus);
        FRACTAE++; piscina_destruere(p); redde;
    }

    sil = silva_scribere_valorem(p, parsura->commissio->radix,
        &SILVA_C89_REGISTRUM, parsura->expansio);

    mradix = silva_frons_valorem_convertere(frons,
        parsura->commissio->radix);
    silva_frons_scripturam_parare(frons, &consilium);

    mat = materia_scribere_valorem(p, mradix, &consilium);

    si (sil.successus != mat.successus)
    {
        imprimere("  %-28s SUCCESSUS DISPAR"
            " (silva %d, materia %d: %s)\n",
            titulus, (int)sil.successus, (int)mat.successus,
            mat.causa ? mat.causa : "-");
        FRACTAE++;
    }
    alioquin si (!sil.successus)
    {
        imprimere("  %-28s ambo recusant (%s) OK\n", titulus,
            sil.causa);
        PROBATAE++;
    }
    alioquin si (   sil.textus.mensura != mat.textus.mensura
                 || memcmp(sil.textus.datum, mat.textus.datum,
                  (size_t)sil.textus.mensura) != 0)
    {
        imprimere("  %-28s OCTETI DISPARES (silva %d, materia %d)\n",
            titulus, (int)sil.textus.mensura, (int)mat.textus.mensura);
        FRACTAE++;
    }
    alioquin
    {
        imprimere("  %-28s %4d octeti, %3d lexemata  IDEM\n", titulus,
            (int)sil.textus.mensura,
            (int)silva_frons_lexemata_numerus(frons));
        PROBATAE++;
    }
    /* --- STML: scriptor materiae contra scriptorem silvae --- */
    si (STML_AGERE)
    {
        SilvaNodus* nodus_radicis =
            silva_frons_nodus_radicis(parsura->commissio->radix);

        si (nodus_radicis != NIHIL)
        {
              SilvaArborScriptura sa;
            MateriaArborScriptura ma;
            MateriaArborConsilium ac;
                     MateriaNodus* mnodus;

            sa = silva_arbor_scribere_nodum(p, nodus_radicis,
                &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
            mnodus = silva_frons_nodum_convertere(frons, nodus_radicis);

            si (!silva_frons_arborem_parare(frons, &ac))
            {
                imprimere("  %-28s LEXICON RECUSATUM (%s)\n", titulus,
                    silva_frons_causa(frons));
                STML_DISPAR++;
            }
            alioquin
            {
                ma = materia_arbor_scribere_nodum(p, mnodus, &ac);

                si (sa.successus != ma.successus)
                {
                    imprimere("  %-28s STML successus dispar"
                        " (s%d m%d: %s)\n",
                        titulus, (int)sa.successus, (int)ma.successus,
                        ma.causa ? ma.causa : "-");
                    STML_DISPAR++;
                }
                alioquin si (!sa.successus)
                { STML_IDEM++;
                }
                alioquin si (   sa.textus.mensura != ma.textus.mensura
                             || memcmp(sa.textus.datum, ma.textus.datum,
                              (size_t)sa.textus.mensura) != 0)
                {
                    imprimere("  %-28s STML DISPAR"
                        " (silva %d, materia %d)\n",
                        titulus, (int)sa.textus.mensura,
                        (int)ma.textus.mensura);
                    si (STML_VERBOSE)
                    {
                        FILE* f1 = fopen("/tmp/shim_silva.stml", "wb");
                        FILE* f2 = fopen("/tmp/shim_materia.stml",
                            "wb");

                        si (f1)
                        { fwrite(sa.textus.datum, 1,
                                      (size_t)sa.textus.mensura,
                                      f1); fclose(f1);
                        }
                        si (f2)
                        { fwrite(ma.textus.datum, 1,
                                      (size_t)ma.textus.mensura,
                                      f2); fclose(f2);
                        }
                        STML_VERBOSE = FALSUM;
                    }
                    STML_DISPAR++;
                }
                alioquin
                {
                    /* --- CIRCUITUS: lege, scribe iterum, confer ---
                     * BIS, quia vitium quod se compoundat primum
                     * circuitum saepe superat. */
                       MateriaArborVitium vit;
                             MateriaNodus* relecta;
                    MateriaArborScriptura  mb;
                                      i32  gyrus;
                                      b32  sanus       = VERUM;
                                      b32  arbor_sana  = VERUM;
                                   chorda  prior       = ma.textus;

                    per (gyrus = ZEPHYRUM; gyrus < (i32)II; gyrus++)
                    {
                        relecta = materia_arbor_legere(p, NIHIL, prior,
                            &ac, &vit);
                        si (relecta == NIHIL)
                        {
                            imprimere("  %-28s CIRCUITUS lectio fracta"
                                " (gyrus %d): %s (linea %d)\n", titulus,
                                (int)gyrus + 1,
                                vit.causa ? vit.causa : "-",
                                (int)vit.linea);
                            sanus = FALSUM; frange;
                        }
                        mb = materia_arbor_scribere_nodum(p, relecta,
                            &ac);
                        si (!mb.successus)
                        {
                            imprimere("  %-28s CIRCUITUS"
                                " scriptio fracta (gyrus %d): %s\n",
                                titulus,
                                (int)gyrus + 1,
                                mb.causa ? mb.causa : "-");
                            sanus = FALSUM; frange;
                        }
                        si (   mb.textus.mensura != ma.textus.mensura
                            || memcmp(mb.textus.datum, ma.textus.datum,
                                      (size_t)ma.textus.mensura) != 0)
                        {
                            imprimere("  %-28s CIRCUITUS DISPAR"
                                " (gyrus %d: %d contra %d)\n", titulus,
                                (int)gyrus + 1, (int)ma.textus.mensura,
                                (int)mb.textus.mensura);
                            sanus = FALSUM; frange;
                        }
                        /* ORACULUM ARBORIS (B1): dislocatio dominii
                         * triviorum octetos servat - sola comparatio
                         * structuralis eam videt. */
                        {
                            MateriaArborDifferentia diff;

                            si (!materia_arbor_aequalis(mnodus, relecta,
                                    COMPARATIO_ARBORIS,
                                    &diff))
                            {
                                imprimere("  %-28s ARBOR DISPAR"
                                    " (gyrus %d: %s @ %s)"
                                    " [a:%s b:%s]\n",
                                    titulus,
                                    (int)gyrus + 1,
                                    diff.campus ? diff.campus : "-",
                                    diff.via,
                                    (diff.nodus_a
                                        && diff.nodus_a->pater)
                                        ? "pater" : "nihil",
                                    (diff.nodus_b
                                        && diff.nodus_b->pater)
                                        ? "pater" : "nihil");
                                arbor_sana = FALSUM;
                            }
                        }
                        prior = mb.textus;
                    }
                    si (sanus)
                    { CIRC_IDEM++;
                    } alioquin
                    { CIRC_DISPAR++;
                    }
                    si (arbor_sana)
                    {
                        ARBOR_IDEM++;
                    }
                    alioquin
                    {
                        ARBOR_DISPAR++;
                    }
                    STML_IDEM++;
                }
            }
        }
    }

    piscina_destruere(p);
}

hic_manens vacuum
_probare_plagulam (
    constans character* via)
{
         FILE* f = fopen(via, "rb");
    character* buf;
       longus  n;

    si (f == NIHIL)
    { imprimere("  %-28s APERIRI NON POTUIT\n", via); FRACTAE++; redde;
    }
    fseek(f, 0, SEEK_END); n = ftell(f); fseek(f, 0, SEEK_SET);
    buf = (character*)malloc((size_t)n + 1);
    si (fread(buf, 1, (size_t)n, f) != (size_t)n)
    { fclose(f); free(buf); FRACTAE++; redde;
    }
    buf[n] = '\0';
    fclose(f);
    _probare(via, buf);
    free(buf);
}

s32
principale (
      integer   argc,
    character** argv)
{
    {
        integer i;
        per (i = 1; i < argc; i++)
        {
            si (strcmp(argv[i], "-stml") == 0)
            { STML_AGERE = VERUM;
            }
            si (strcmp(argv[i], "-v") == 0)
            { STML_VERBOSE = VERUM;
            }
        }
    }
    imprimere("\n=== SHIM: materia_scribere contra"
        " silva_scribere ===\n\n");

    _probare("declaratio nuda",     "int x;\n");
    _probare("functio",
        "int f(int a)\n{\n    return a + 1;\n}\n");
    _probare("commenta et trivia",  "/* c */\nint  x ;  /* d */\n");
    _probare("macro obiectum",      "#define N 4\nint a[N];\n");
    _probare("macro functio",
        "#define M(a,b) ((a)+(b))\nint x = M(1,2);\n");
    _probare("macro nidificata",
        "#define A 1\n#define B (A+A)\nint y = B;\n");
    _probare("directiva et codex",
        "#define X\nint p;\n#undef X\nint q;\n");
    _probare("structura",           "struct S { int a; char* b; };\n");
    _probare("lamina intra lexema", "int ab\\\ncd;\n");
    _probare("conditionalis",
        "#if 1\nint u;\n#else\nint v;\n#endif\n");

    si (argc > 1)
    {
        integer i;

        per (i = 1; i < argc; i++)
        {
            si (   strcmp(argv[i], "-stml") == 0
                || strcmp(argv[i], "-v")    == 0)
            { perge;
            }
            _probare_plagulam(argv[i]);
        }
    }
    si (STML_AGERE)
    {
        imprimere("  STML: idem %d, dispar %d\n", (int)STML_IDEM,
            (int)STML_DISPAR);
        imprimere("  CIRCUITUS (bis): idem %d, dispar %d\n",
            (int)CIRC_IDEM, (int)CIRC_DISPAR);
        imprimere("  ARBOR (comparator): idem %d, dispar %d\n",
            (int)ARBOR_IDEM, (int)ARBOR_DISPAR);
    }

    imprimere("\n  probatae %d, fractae %d\n\n", (int)PROBATAE,
        (int)FRACTAE);
    /* EXITUS OMNIA numerat. Prius FRACTAE solas numerabat, ergo
     * STML_DISPAR et CIRC_DISPAR nuntiabantur sed exitum NON
     * movebant - porta quae in assertione sua PRINCIPALI cadere
     * non potest. Tertium huius generis hodie. */
    redde (FRACTAE == 0 && STML_DISPAR == 0 && CIRC_DISPAR == 0
        && ARBOR_DISPAR == 0)
        ? ZEPHYRUM : I;
}
