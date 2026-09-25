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
/* PARITAS VERDICTI (T10b): circuitus SILVAE ipsius (scriptor et lector
 * silvae) contra circuitum materiae - verdicta comparatoris congruere
 * debent. Silva sana ubi materia non = regressio si silva in lectorem
 * materiae migrat. */
hic_manens i32 SILVA_ARBOR_DISPAR = 0;
hic_manens i32 VERDICTA_DISPARIA = 0;
/* SEMITA NOVA (T10b): materia + unci super caudam SILVAE (sine
 * conversione) contra scriptorem silvae VETEREM - octeti, circuitus,
 * comparator silvae contra arborem parsatam. Probatio ante switch. */
hic_manens i32 NOVA_IDEM = 0;
hic_manens i32 NOVA_DISPAR = 0;
hic_manens i32 NOVA_CIRC_DISPAR = 0;
hic_manens i32 NOVA_ARBOR_DISPAR = 0;

/* comparatio arboris in circuitu (nomen breve: nidus profundus) */
hic_manens constans MateriaArborComparatioModus COMPARATIO_ARBORIS =
    MATERIA_ARBOR_COMPARATIO_STRUCTURALIS;

/* SEMITA NOVA unius nodi (T10b) - vide NOVA_* supra */
hic_manens vacuum
_novam_probare (
     constans character* titulus,
                Piscina* p,
           SilvaParsura* parsura,
             SilvaNodus* nodus,
                    i32  index,
    SilvaArborScriptura  sa)
{
               SilvaFrons* fs = silva_frons_creare(p,
                   parsura->expansio);
    MateriaArborConsilium cs;
    MateriaArborScriptura ns;

    si (   fs == NIHIL
        || !silva_frons_arborem_silvae_parare(fs, &SILVA_C89_REGISTRUM,
               "c89", NIHIL, &cs))
    {
        NOVA_DISPAR++;
        imprimere("  %-28s #%-4d NOVA consilium fractum\n", titulus,
            (int)index);
        redde;
    }
    ns = materia_arbor_scribere_nodum(p, nodus, &cs);
    si (   ns.successus != sa.successus
        || (sa.successus
            && (   ns.textus.mensura != sa.textus.mensura
                || memcmp(ns.textus.datum, sa.textus.datum,
                       (size_t)sa.textus.mensura) != 0)))
    {
        NOVA_DISPAR++;
        imprimere("  %-28s #%-4d NOVA DISPAR (silva %d/%d, nova %d/%d:"
            " %s)\n", titulus, (int)index, (int)sa.successus,
            (int)sa.textus.mensura, (int)ns.successus,
            (int)ns.textus.mensura, ns.causa ? ns.causa : "-");
        si (STML_VERBOSE && sa.successus && ns.successus)
        {
            FILE* f1 = fopen("/tmp/shim_silva.stml", "wb");
            FILE* f2 = fopen("/tmp/shim_nova.stml", "wb");

            si (f1)
            { fwrite(sa.textus.datum, 1, (size_t)sa.textus.mensura, f1);
              fclose(f1);
            }
            si (f2)
            { fwrite(ns.textus.datum, 1, (size_t)ns.textus.mensura, f2);
              fclose(f2);
            }
            STML_VERBOSE = FALSUM;
        }
        redde;
    }
    si (!sa.successus)
    {
        NOVA_IDEM++;
        redde;
    }
    NOVA_IDEM++;
    {
                   SilvaFrons* fr = silva_frons_creare(p, NIHIL);
        MateriaArborConsilium  cr;
           MateriaArborVitium  vn;
                 MateriaNodus* rel = NIHIL;

        vn.causa = NIHIL;
        si (   fr != NIHIL
            && silva_frons_arborem_silvae_parare(fr,
            &SILVA_C89_REGISTRUM,
                   "c89", NIHIL, &cr))
        {
            rel = materia_arbor_legere(p, NIHIL, ns.textus, &cr, &vn);
        }
        si (rel == NIHIL)
        {
            NOVA_CIRC_DISPAR++;
            imprimere("  %-28s #%-4d NOVA lectio fracta (%s)\n",
                titulus,
                (int)index, vn.causa ? vn.causa : "-");
            redde;
        }
        {
            MateriaArborScriptura nb = materia_arbor_scribere_nodum(p,
                rel, &cr);
            SilvaArborDifferentia nd;

            si (   !nb.successus
                || nb.textus.mensura != ns.textus.mensura
                || memcmp(nb.textus.datum, ns.textus.datum,
                       (size_t)ns.textus.mensura) != 0)
            {
                NOVA_CIRC_DISPAR++;
                imprimere("  %-28s #%-4d NOVA CIRCUITUS DISPAR (%s)\n",
                    titulus, (int)index,
                    nb.causa ? nb.causa : "octeti");
            }
            si (!silva_arbor_aequalis(nodus, rel,
                    SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &nd))
            {
                NOVA_ARBOR_DISPAR++;
                imprimere("  %-28s #%-4d NOVA ARBOR DISPAR (%s @ %s)\n",
                    titulus, (int)index, nd.campus ? nd.campus : "-",
                    nd.via);
            }
        }
    }
}

/* STML unius nodi supremi: scriptor materiae (per frontem) contra
 * scriptorem silvae, deinde circuitus bis et comparator.
 *
 * OMNES NODI SUPREMI (silva-migratio T10b, 2026-09-25). Prius nodus
 * PRIMUS solus cuiusque plagulae probabatur
 * (silva_frons_nodus_radicis): 'STML idem CDI' CDI nodos primos
 * significabat, non plagulas totas. Paritas quam T10b poscit TOTA
 * esse debet. */
hic_manens vacuum
_stml_nodum_probare (
    constans character* titulus,
               Piscina* p,
          SilvaParsura* parsura,
            SilvaFrons* frons,
            SilvaNodus* nodus,
                   i32  index)
{
          SilvaArborScriptura sa;
        MateriaArborScriptura ma;
        MateriaArborConsilium ac;
                 MateriaNodus* mnodus;

        sa = silva_arbor_scribere_nodum(p, nodus,
            &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
        mnodus = silva_frons_nodum_convertere(frons, nodus);
        _novam_probare(titulus, p, parsura, nodus, index, sa);

        si (!silva_frons_arborem_parare(frons, &ac))
        {
            imprimere("  %-28s #%-4d LEXICON RECUSATUM (%s)\n", titulus,
                (int)index,
                silva_frons_causa(frons));
            STML_DISPAR++;
        }
        alioquin
        {
            ma = materia_arbor_scribere_nodum(p, mnodus, &ac);

            si (sa.successus != ma.successus)
            {
                imprimere("  %-28s #%-4d STML successus dispar"
                    " (s%d m%d: %s)\n",
                    titulus, (int)index, (int)sa.successus,
                    (int)ma.successus,
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
                imprimere("  %-28s #%-4d STML DISPAR"
                    " (silva %d, materia %d)\n",
                    titulus, (int)index, (int)sa.textus.mensura,
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
                        imprimere("  %-28s #%-4d CIRCUITUS lectio fracta"
                            " (gyrus %d): %s (linea %d)\n", titulus,
                            (int)index,
                            (int)gyrus + 1,
                            vit.causa ? vit.causa : "-",
                            (int)vit.linea);
                        sanus = FALSUM; frange;
                    }
                    mb = materia_arbor_scribere_nodum(p, relecta,
                        &ac);
                    si (!mb.successus)
                    {
                        imprimere("  %-28s #%-4d CIRCUITUS"
                            " scriptio fracta (gyrus %d): %s\n",
                            titulus, (int)index,
                            (int)gyrus + 1,
                            mb.causa ? mb.causa : "-");
                        sanus = FALSUM; frange;
                    }
                    si (   mb.textus.mensura != ma.textus.mensura
                        || memcmp(mb.textus.datum, ma.textus.datum,
                                  (size_t)ma.textus.mensura) != 0)
                    {
                        imprimere("  %-28s #%-4d CIRCUITUS DISPAR"
                            " (gyrus %d: %d contra %d)\n", titulus,
                            (int)index,
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
                            imprimere("  %-28s #%-4d ARBOR DISPAR"
                                " (gyrus %d: %s @ %s)"
                                " [a:%s b:%s]",
                                titulus, (int)index,
                                (int)gyrus + 1,
                                diff.campus ? diff.campus : "-",
                                diff.via,
                                (diff.nodus_a
                                    && diff.nodus_a->pater)
                                    ? "pater" : "nihil",
                                (diff.nodus_b
                                    && diff.nodus_b->pater)
                                    ? "pater" : "nihil");
                            /* LEXEMA divergens NOMINATUM: via sola
                             * bisectionem manualem petit */
                            si (diff.lexema_a != NIHIL)
                            {
                                imprimere(" lexema '%.*s' linea %d",
                                    (int)diff.lexema_a->valor.mensura,
                                    (constans character*)
                                        diff.lexema_a->valor.datum,
                                    (int)diff.lexema_a->linea);
                            }
                            imprimere("\n");
                            arbor_sana = FALSUM;
                        }
                    }
                    prior = mb.textus;
                }
                /* circuitus silvae, gyro uno */
                {
                         SilvaArborVitium sv;
                    SilvaArborDifferentia sd;
                    SilvaNodus*            sr;
                    b32                    silva_sana;

                    sr = silva_arbor_legere(p, NIHIL, sa.textus,
                        &SILVA_C89_REGISTRUM, "c89", &sv);
                    silva_sana = (sr != NIHIL)
                        && silva_arbor_aequalis(nodus, sr,
                               SILVA_ARBOR_COMPARATIO_STRUCTURALIS,
                               &sd);
                    si (!silva_sana)
                    {
                        SILVA_ARBOR_DISPAR++;
                        imprimere("  %-28s #%-4d ARBOR SILVAE DISPAR"
                            " (%s @ %s)\n", titulus, (int)index,
                            sr == NIHIL
                                ? (sv.causa ? sv.causa : "lectio")
                                : (sd.campus ? sd.campus : "-"),
                            sr == NIHIL ? "-" : sd.via);
                    }
                    si (silva_sana != arbor_sana)
                    {
                        VERDICTA_DISPARIA++;
                    }
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
    /* --- STML: scriptor materiae contra scriptorem silvae, per
     * nodum supremum QUEMQUE --- */
    si (STML_AGERE)
    {
        SilvaValor radix = parsura->commissio->radix;

        si (radix.genus == SILVA_VALOR_NODUS)
        {
            _stml_nodum_probare(titulus, p, parsura, frons,
                radix.datum.nodus, ZEPHYRUM);
        }
        alioquin si (radix.genus == SILVA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM; k
                < silva_valor_lista_numerus(radix); k++)
            {
                SilvaValor* e = silva_valor_lista_obtinere(radix, k);

                si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
                {
                    _stml_nodum_probare(titulus, p, parsura, frons,
                        e->datum.nodus, k);
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
    /* T10b: lexema post invocationem functio-similem multi-linearem
     * initium_lineae (et sedem) servat - extentum LECTUM invenitur */
    _probare("invocatio ante lineam",
        "#define M(p) \\\n    { 1, p }, \\\n    { 2, p }\n"
        "int a[][2] = {\n    M(3)\n};\n");

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
        imprimere("  ARBOR SILVAE (circuitus silvae): dispar %d;"
            " verdicta disparia %d\n",
            (int)SILVA_ARBOR_DISPAR, (int)VERDICTA_DISPARIA);
        imprimere("  NOVA (materia super caudam silvae): idem %d,"
            " dispar %d; circuitus dispar %d; arbor dispar %d\n",
            (int)NOVA_IDEM, (int)NOVA_DISPAR, (int)NOVA_CIRC_DISPAR,
            (int)NOVA_ARBOR_DISPAR);
    }

    imprimere("\n  probatae %d, fractae %d\n\n", (int)PROBATAE,
        (int)FRACTAE);
    /* EXITUS OMNIA numerat. Prius FRACTAE solas numerabat, ergo
     * STML_DISPAR et CIRC_DISPAR nuntiabantur sed exitum NON
     * movebant - porta quae in assertione sua PRINCIPALI cadere
     * non potest. Tertium huius generis hodie. */
    redde (FRACTAE == 0 && STML_DISPAR == 0 && CIRC_DISPAR == 0
        && ARBOR_DISPAR == 0 && SILVA_ARBOR_DISPAR == 0
        && NOVA_DISPAR == 0 && NOVA_CIRC_DISPAR == 0
        && NOVA_ARBOR_DISPAR == 0)
        ? ZEPHYRUM : I;
}
