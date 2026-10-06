/* probatio_historia.c - cauda actorum cum proiectione ludicra
 *
 * Proiectio: VIII numeratores (i8); actum "k" numeratorem k auget.
 * Contextus applicationes numerat - revocatio ex checkpoint probatur
 * numero applicationum, non solum fructu. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "volumen.h"
#include "historia.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
     i8 numeratores[VIII];
    i32 applicationes;
    i32 vacationes;
} Ludicra;

interior vacuum
ludicra_vacare (
    vacuum* ctx)
{
    Ludicra* l;

    l = (Ludicra*)ctx;
    memset(l->numeratores, ZEPHYRUM, magnitudo(l->numeratores));
    l->vacationes++;
}

interior vacuum
ludicra_applicare (
    vacuum* ctx,
    chorda  actum)
{
    Ludicra* l;
        s32  k;

    l = (Ludicra*)ctx;
    l->applicationes++;
    si (chorda_ut_s32(actum, &k) && k >= ZEPHYRUM && k < VIII)
    {
        l->numeratores[k]++;
    }
}

interior HistoriaProiectio
proiectio (
    Ludicra* l)
{
    HistoriaProiectio p;

    p.memoria    = l->numeratores;
    p.mensura    = magnitudo(l->numeratores);
    p.vacare     = ludicra_vacare;
    p.applicare  = ludicra_applicare;
    p.ctx        = l;
    redde p;
}

interior s64
actum (
     Historia* h,
      Piscina* p,
          s32  k)
{
    redde historia_actum(h, chorda_ex_s32(k, p));
}

/* numeratores ut chorda "11100100" pro comparatione legibili */
interior b32
numeratores_sunt (
               Ludicra* l,
    constans character* expectati)
{
    i32 i;

    per (i = ZEPHYRUM; i < VIII; i++)
    {
        si (l->numeratores[i] != (i8)(expectati[i] - '0'))
        {
            imprimere("  numeratores: ");
            per (i = ZEPHYRUM; i < VIII; i++)
            {
                imprimere("%d", (integer)l->numeratores[i]);
            }
            imprimere(" (expectati %s)\n", expectati);
            redde FALSUM;
        }
    }
    redde VERUM;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
               Historia* h;
               Historia* h2;
                Ludicra  l;
                Ludicra  l2;
                    s64  q[VIII];
                 chorda  s_ante;
                    Xar* plagulae;
         VolumenPlagula* pl;
                    i32  i;
      HistoriaProiectio  mala;

    piscina = piscina_generare_dynamicum("probatio_historia", XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    memset(&l, ZEPHYRUM, magnitudo(l));
    memset(&l2, ZEPHYRUM, magnitudo(l2));

    imprimere("\n--- I: creare - vacata, cursor 0 ---\n");
    vol = volumen_temporarium(piscina, "probatio_historia");
    CREDO_NON_NIHIL(vol);
    l.numeratores[III] = VII;   /* sordes ante vacationem */
    h = historia_creare(piscina, intern, vol, "", "numerus",
                        "probatio:checkpoint", II, proiectio(&l));
    CREDO_NON_NIHIL(h);
    CREDO_VERUM(numeratores_sunt(&l, "00000000"));
    CREDO_AEQUALIS_S64(historia_cursor(h), ZEPHYRUM);
    CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), ZEPHYRUM);
    CREDO_AEQUALIS_I32(historia_sigillum_hex(h, piscina).mensura, LXIV);

    imprimere("\n--- II: V acta, checkpoints post II et IV viva ---\n");
    per (i = ZEPHYRUM; i < V; i++)
    {
        q[i] = actum(h, piscina, (s32)i);
        CREDO_VERUM(q[i] > (i ? q[i - I] : ZEPHYRUM));
    }
    CREDO_VERUM(numeratores_sunt(&l, "11111000"));
    CREDO_AEQUALIS_S64(historia_cursor(h), q[IV]);
    CREDO_AEQUALIS_S64(historia_finis(h), q[IV]);
    CREDO_AEQUALIS_I32(historia_numerus_vivorum(h), V);
    plagulae = volumen_plagulas_enumerare(vol, piscina);
    CREDO_AEQUALIS_I32(xar_numerus(plagulae), II);
    per (i = ZEPHYRUM; i < xar_numerus(plagulae); i++)
    {
        pl = (VolumenPlagula*)xar_obtinere(plagulae, i);
        CREDO_VERUM(chorda_aequalis_literis(pl->origo,
            "probatio:checkpoint"));
    }

    imprimere("\n--- III: revocare ex checkpoint, non ex nihilo ---\n");
    l.applicationes = ZEPHYRUM;
    CREDO_VERUM(historia_revocare(h));    /* ad q3: checkpoint ipse */
    CREDO_AEQUALIS_S64(historia_cursor(h), q[III]);
    CREDO_VERUM(numeratores_sunt(&l, "11110000"));
    CREDO_AEQUALIS_I32(l.applicationes, ZEPHYRUM);
    CREDO_VERUM(historia_revocare(h));    /* ad q2: checkpoint q1 + I */
    CREDO_VERUM(numeratores_sunt(&l, "11100000"));
    CREDO_AEQUALIS_I32(l.applicationes, I);

    imprimere("\n--- IV: ramus - actum post revocationem ---\n");
    CREDO_VERUM(historia_revocare(h));    /* ad q1 */
    CREDO_AEQUALIS_I32(historia_numerus_vivorum(h), II);
    q[V] = actum(h, piscina, V);
    CREDO_VERUM(q[V] > q[IV]);
    CREDO_VERUM(numeratores_sunt(&l, "11000100"));
    CREDO_FALSUM(historia_reficere(h));
    CREDO_AEQUALIS_I32(historia_numerus_vivorum(h), III);
    /* vivi IV: checkpoint post ramum */
    q[VI] = actum(h, piscina, VI);
    CREDO_VERUM(numeratores_sunt(&l, "11000110"));
    CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), III);

    imprimere("\n--- V: checkpoint MORTUUS numquam basis ---\n");
    s_ante           = historia_sigillum_hex(h, piscina);
    l.applicationes  = ZEPHYRUM;
    CREDO_VERUM(historia_revocare(h));    /* ad q5; q3 mortuus <= ad */
    CREDO_AEQUALIS_S64(historia_cursor(h), q[V]);
    CREDO_VERUM(numeratores_sunt(&l, "11000100"));
    CREDO_AEQUALIS_I32(l.applicationes, I);  /* checkpoint q1 + q5 */

    imprimere("\n--- VI: reficere - sigillum idem ---\n");
    CREDO_VERUM(historia_reficere(h));
    CREDO_AEQUALIS_S64(historia_cursor(h), q[VI]);
    CREDO_VERUM(chorda_aequalis(s_ante, historia_sigillum_hex(h,
        piscina)));
    CREDO_FALSUM(historia_reficere(h));

    imprimere("\n--- VII: verificare - memoria extra machinam ---\n");
    CREDO_VERUM(historia_verificare(h));
    /* sigillum notatum verum manet: reproiectio ei congruit et
     * memoriam tacite reficit (non deprehenditur - lex nominata) */
    l.numeratores[ZEPHYRUM] = IX;
    CREDO_VERUM(historia_verificare(h));
    CREDO_VERUM(numeratores_sunt(&l, "11000110"));

    imprimere("\n--- VIII: acta aliena ignorantur ---\n");
    CREDO_VERUM(volumen_actum_appendere(vol, "alienum",
        chorda_ex_literis("7", piscina)) > ZEPHYRUM);
    q[VII] = actum(h, piscina, I);
    CREDO_VERUM(numeratores_sunt(&l, "12000110"));
    CREDO_VERUM(historia_verificare(h));

    imprimere("\n--- IX: aperire - status idem ---\n");
    h2 = historia_aperire(piscina, intern, vol, "", "numerus",
                          "probatio:checkpoint", II, proiectio(&l2));
    CREDO_NON_NIHIL(h2);
    CREDO_AEQUALIS_S64(historia_cursor(h2), q[VII]);
    CREDO_AEQUALIS_S64(historia_finis(h2), q[VII]);
    CREDO_AEQUALIS_I32(historia_numerus_vivorum(h2),
                       historia_numerus_vivorum(h));
    CREDO_VERUM(numeratores_sunt(&l2, "12000110"));
    CREDO_VERUM(chorda_aequalis(historia_sigillum_hex(h, piscina),
                                historia_sigillum_hex(h2, piscina)));

    imprimere("\n--- X: revocare ad initium ---\n");
    per (i = ZEPHYRUM; i < V; i++)
    {
        CREDO_VERUM(historia_revocare(h));
    }
    CREDO_AEQUALIS_S64(historia_cursor(h), ZEPHYRUM);
    CREDO_VERUM(numeratores_sunt(&l, "00000000"));
    CREDO_FALSUM(historia_revocare(h));

    imprimere("\n--- XI: proiectio discors SIGNATA videtur ---\n");
    {
          Volumen* vol_discors;
          Ludicra  l3;
         Historia* h3;

        memset(&l3, ZEPHYRUM, magnitudo(l3));
        vol_discors = volumen_temporarium(piscina,
            "probatio_historia_discors");
        h3 = historia_creare(piscina, intern, vol_discors, "",
            "numerus",
                             "probatio:checkpoint", II, proiectio(&l3));
        CREDO_NON_NIHIL(h3);
        actum(h3, piscina, ZEPHYRUM);
        CREDO_VERUM(historia_verificare(h3));
        l3.numeratores[V] = IX;       /* ante actum: signabitur */
        actum(h3, piscina, I);
        CREDO_FALSUM(historia_verificare(h3));
        CREDO_VERUM(numeratores_sunt(&l3, "11000000"));   /* refecta */
        volumen_claudere(vol_discors);
    }

    imprimere("\n--- XII: acta coniuncta - grex unus gradus ---\n");
    {
          Volumen* vol_grex;
          Ludicra  ludicra_grex;
          Ludicra  ludicra_aperta;
         Historia* historia_grex;
         Historia* historia_aperta;
              s64  a1;
              s64  a4;
              s64  a5;
              s64  a6;
              s64  a7;

        memset(&ludicra_grex, ZEPHYRUM, magnitudo(ludicra_grex));
        memset(&ludicra_aperta, ZEPHYRUM, magnitudo(ludicra_aperta));
        vol_grex = volumen_temporarium(piscina,
            "probatio_historia_grex");
        historia_grex = historia_creare(piscina, intern, vol_grex, "",
            "numerus",
                             "probatio:checkpoint", II,
                             proiectio(&ludicra_grex));
        CREDO_NON_NIHIL(historia_grex);
        a1 = actum(historia_grex, piscina, ZEPHYRUM);
        (vacuum)actum(historia_grex, piscina, I);
        CREDO_VERUM(historia_actum_coniunctum(historia_grex,
            chorda_ex_s32(II,
            piscina)) > ZEPHYRUM);
        a4 = historia_actum_coniunctum(historia_grex, chorda_ex_s32(III,
            piscina));
        CREDO_VERUM(numeratores_sunt(&ludicra_grex, "11110000"));
        CREDO_AEQUALIS_I32(historia_numerus_vivorum(historia_grex), IV);
        /* revocare: grex totus (I, II, III) gradu uno */
        CREDO_VERUM(historia_revocare(historia_grex));
        CREDO_AEQUALIS_S64(historia_cursor(historia_grex), a1);
        CREDO_VERUM(numeratores_sunt(&ludicra_grex, "10000000"));
        CREDO_AEQUALIS_I32(historia_numerus_vivorum(historia_grex), I);
        /* reficere: grex totus */
        CREDO_VERUM(historia_reficere(historia_grex));
        CREDO_AEQUALIS_S64(historia_cursor(historia_grex), a4);
        CREDO_VERUM(numeratores_sunt(&ludicra_grex, "11110000"));
        CREDO_AEQUALIS_I32(historia_numerus_vivorum(historia_grex), IV);
        CREDO_FALSUM(historia_reficere(historia_grex));
        CREDO_VERUM(historia_verificare(historia_grex));
        /* actum simplex post gregem: unus, deinde grex */
        a5 = actum(historia_grex, piscina, IV);
        CREDO_VERUM(a5 > a4);
        CREDO_VERUM(historia_revocare(historia_grex));
        CREDO_AEQUALIS_S64(historia_cursor(historia_grex), a4);
        CREDO_VERUM(historia_revocare(historia_grex));
        CREDO_AEQUALIS_S64(historia_cursor(historia_grex), a1);
        /* ramus post gregem revocatum: notae cum actis truncantur */
        a6 = actum(historia_grex, piscina, V);
        CREDO_VERUM(numeratores_sunt(&ludicra_grex, "10000100"));
        CREDO_AEQUALIS_I32(historia_numerus_vivorum(historia_grex), II);
        CREDO_VERUM(historia_revocare(historia_grex));
        CREDO_AEQUALIS_S64(historia_cursor(historia_grex), a1);
        CREDO_VERUM(historia_reficere(historia_grex));
        CREDO_AEQUALIS_S64(historia_cursor(historia_grex), a6);
        /* aperire: greges ex volumine soli */
        a7 = historia_actum_coniunctum(historia_grex, chorda_ex_s32(VI,
            piscina));
        historia_aperta = historia_aperire(piscina, intern, vol_grex,
            "",
            "numerus",
                               "probatio:checkpoint", II,
                               proiectio(&ludicra_aperta));
        CREDO_NON_NIHIL(historia_aperta);
        CREDO_AEQUALIS_S64(historia_cursor(historia_aperta), a7);
        CREDO_VERUM(numeratores_sunt(&ludicra_aperta, "10000110"));
        CREDO_VERUM(historia_revocare(historia_aperta));
        CREDO_AEQUALIS_S64(historia_cursor(historia_aperta), a1);
        CREDO_VERUM(numeratores_sunt(&ludicra_aperta, "10000000"));
        CREDO_VERUM(historia_verificare(historia_aperta));
        volumen_claudere(vol_grex);
    }
    {
        /* ramus post gregem: actum SECUNDUM post ramum notam veterem
         * (gregis mortui) heredare non debet */
          Volumen* vol_ramus;
          Ludicra  ludicra_rami;
         Historia* historia_rami;
              s64  c1;
              s64  d1;

        memset(&ludicra_rami, ZEPHYRUM, magnitudo(ludicra_rami));
        vol_ramus = volumen_temporarium(piscina,
            "probatio_historia_ramus");
        historia_rami = historia_creare(piscina, intern, vol_ramus, "",
            "numerus",
                             "probatio:checkpoint", C,
                             proiectio(&ludicra_rami));
        c1 = actum(historia_rami, piscina, ZEPHYRUM);
        (vacuum)actum(historia_rami, piscina, I);
        (vacuum)historia_actum_coniunctum(historia_rami,
            chorda_ex_s32(II,
            piscina));
        CREDO_VERUM(historia_revocare(historia_rami));
        CREDO_AEQUALIS_S64(historia_cursor(historia_rami), c1);
        d1 = actum(historia_rami, piscina, III);
        (vacuum)actum(historia_rami, piscina, IV);
        CREDO_VERUM(historia_revocare(historia_rami));
        CREDO_AEQUALIS_S64(historia_cursor(historia_rami), d1);
        CREDO_VERUM(numeratores_sunt(&ludicra_rami, "10010000"));
        volumen_claudere(vol_ramus);
    }

    imprimere("\n--- XIII: spatia nominum - duae historiae, volumen"
              " unum ---\n");
    {
          Volumen* vol_duplex;
          Ludicra  ludicra_a;
          Ludicra  ludicra_b;
          Ludicra  ludicra_a_aperta;
          Ludicra  ludicra_b_aperta;
         Historia* historia_a;
         Historia* historia_b;
         Historia* historia_a_aperta;
         Historia* historia_b_aperta;
              s64  a1;
              Xar* plagulae;
              i32  i;
              b32  praefixa;

        memset(&ludicra_a, ZEPHYRUM, magnitudo(ludicra_a));
        memset(&ludicra_b, ZEPHYRUM, magnitudo(ludicra_b));
        memset(&ludicra_a_aperta, ZEPHYRUM,
            magnitudo(ludicra_a_aperta));
        memset(&ludicra_b_aperta, ZEPHYRUM,
            magnitudo(ludicra_b_aperta));
        vol_duplex = volumen_temporarium(piscina,
            "probatio_historia_duplex");
        historia_a = historia_creare(piscina, intern, vol_duplex, "a",
            "numerus",
                             "probatio:checkpoint", II,
                             proiectio(&ludicra_a));
        historia_b = historia_creare(piscina, intern, vol_duplex, "b",
            "numerus",
                             "probatio:checkpoint", II,
                             proiectio(&ludicra_b));
        /* acta alternata */
        a1 = actum(historia_a, piscina, ZEPHYRUM);
        (vacuum)actum(historia_b, piscina, V);
        (vacuum)actum(historia_a, piscina, I);
        (vacuum)actum(historia_b, piscina, VI);
        (vacuum)actum(historia_a, piscina, II);
        (vacuum)actum(historia_b, piscina, VII);
        CREDO_VERUM(numeratores_sunt(&ludicra_a, "11100000"));
        CREDO_VERUM(numeratores_sunt(&ludicra_b, "00000111"));
        /* revocare in a; b intactum */
        CREDO_VERUM(historia_revocare(historia_a));
        CREDO_VERUM(historia_revocare(historia_a));
        CREDO_AEQUALIS_S64(historia_cursor(historia_a), a1);
        CREDO_VERUM(numeratores_sunt(&ludicra_a, "10000000"));
        CREDO_VERUM(numeratores_sunt(&ludicra_b, "00000111"));
        /* ramus in a: acta b post ramum a non moriuntur */
        (vacuum)actum(historia_a, piscina, III);
        CREDO_VERUM(numeratores_sunt(&ludicra_a, "10010000"));
        CREDO_VERUM(historia_revocare(historia_b));
        CREDO_VERUM(numeratores_sunt(&ludicra_b, "00000110"));
        CREDO_VERUM(historia_reficere(historia_b));
        CREDO_VERUM(historia_verificare(historia_a));
        CREDO_VERUM(historia_verificare(historia_b));
        /* aperire utramque: status suus */
        historia_a_aperta = historia_aperire(piscina, intern,
            vol_duplex, "a",
                               "numerus", "probatio:checkpoint", II,
                               proiectio(&ludicra_a_aperta));
        historia_b_aperta = historia_aperire(piscina, intern,
            vol_duplex, "b",
                               "numerus", "probatio:checkpoint", II,
                               proiectio(&ludicra_b_aperta));
        CREDO_VERUM(numeratores_sunt(&ludicra_a_aperta, "10010000"));
        CREDO_VERUM(numeratores_sunt(&ludicra_b_aperta, "00000111"));
        CREDO_AEQUALIS_I32(historia_numerus_vivorum(historia_a_aperta),
            II);
        CREDO_AEQUALIS_I32(historia_numerus_vivorum(historia_b_aperta),
            III);
        /* claves checkpointorum praefixa */
        plagulae = volumen_plagulas_enumerare(vol_duplex, piscina);
        praefixa = xar_numerus(plagulae) > ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(plagulae); i++)
        {
            chorda via;

            via = ((VolumenPlagula*)xar_obtinere(plagulae, i))->via;
            si (!chorda_incipit(via, chorda_ex_literis("a/checkpoint/",
                    piscina))
                && !chorda_incipit(via,
                chorda_ex_literis("b/checkpoint/",
                    piscina)))
            {
                praefixa = FALSUM;
            }
        }
        CREDO_VERUM(praefixa);
        volumen_claudere(vol_duplex);
    }

    imprimere("\n--- XIV: locus revocandi servatur (R5) ---\n");
    {
          Volumen* vol_locus;
          Ludicra  ludicra_loci;
         Historia* historia_loci;
              s64  q_unum;

        memset(&ludicra_loci, ZEPHYRUM, magnitudo(ludicra_loci));
        vol_locus = volumen_temporarium(piscina,
            "probatio_historia_locus");
        historia_loci = historia_creare(piscina, intern, vol_locus, "",
            "numerus",
                             "probatio:checkpoint", II,
                             proiectio(&ludicra_loci));
        (vacuum)actum(historia_loci, piscina, ZEPHYRUM);
        q_unum = actum(historia_loci, piscina, I);
        (vacuum)actum(historia_loci, piscina, II);
        CREDO_VERUM(historia_revocare(historia_loci));
        /* reapertio ad locum revocatum, non ad finem */
        memset(&ludicra_loci, ZEPHYRUM, magnitudo(ludicra_loci));
        historia_loci = historia_aperire(piscina, intern, vol_locus, "",
            "numerus",
                              "probatio:checkpoint", II,
                              proiectio(&ludicra_loci));
        CREDO_AEQUALIS_S64(historia_cursor(historia_loci), q_unum);
        CREDO_AEQUALIS_I32(historia_numerus_vivorum(historia_loci), II);
        CREDO_VERUM(numeratores_sunt(&ludicra_loci, "11000000"));
        /* reficere post reapertionem */
        CREDO_VERUM(historia_reficere(historia_loci));
        CREDO_VERUM(numeratores_sunt(&ludicra_loci, "11100000"));
        CREDO_VERUM(historia_verificare(historia_loci));
        /* reapertio post reficere: ad finem */
        memset(&ludicra_loci, ZEPHYRUM, magnitudo(ludicra_loci));
        historia_loci = historia_aperire(piscina, intern, vol_locus, "",
            "numerus",
                              "probatio:checkpoint", II,
                              proiectio(&ludicra_loci));
        CREDO_VERUM(numeratores_sunt(&ludicra_loci, "11100000"));
        /* revocare ad initium, reaperire: vacuum */
        per (i = ZEPHYRUM; i < III; i++)
        {
            CREDO_VERUM(historia_revocare(historia_loci));
        }
        memset(&ludicra_loci, ZEPHYRUM, magnitudo(ludicra_loci));
        historia_loci = historia_aperire(piscina, intern, vol_locus, "",
            "numerus",
                              "probatio:checkpoint", II,
                              proiectio(&ludicra_loci));
        CREDO_AEQUALIS_S64(historia_cursor(historia_loci), ZEPHYRUM);
        CREDO_VERUM(numeratores_sunt(&ludicra_loci, "00000000"));
        /* actum novum post revocationem: nota obsoleta, finis */
        (vacuum)actum(historia_loci, piscina, V);
        memset(&ludicra_loci, ZEPHYRUM, magnitudo(ludicra_loci));
        historia_loci = historia_aperire(piscina, intern, vol_locus, "",
            "numerus",
                              "probatio:checkpoint", II,
                              proiectio(&ludicra_loci));
        CREDO_VERUM(numeratores_sunt(&ludicra_loci, "00000100"));
        CREDO_AEQUALIS_I32(historia_numerus_vivorum(historia_loci), I);
        /* grex coniunctus revocatus et reapertus */
        CREDO_VERUM(historia_actum_coniunctum(historia_loci,
            chorda_ex_s32(VI,
            piscina)) > ZEPHYRUM);
        CREDO_VERUM(historia_revocare(historia_loci));
        memset(&ludicra_loci, ZEPHYRUM, magnitudo(ludicra_loci));
        historia_loci = historia_aperire(piscina, intern, vol_locus, "",
            "numerus",
                              "probatio:checkpoint", II,
                              proiectio(&ludicra_loci));
        CREDO_AEQUALIS_S64(historia_cursor(historia_loci), ZEPHYRUM);
        CREDO_VERUM(numeratores_sunt(&ludicra_loci, "00000000"));
        CREDO_VERUM(historia_reficere(historia_loci));
        CREDO_VERUM(numeratores_sunt(&ludicra_loci, "00000110"));
        CREDO_VERUM(historia_verificare(historia_loci));
        volumen_claudere(vol_locus);
    }

    imprimere("\n--- XV: argumenta mala ---\n");
    mala            = proiectio(&l);
    mala.applicare  = NIHIL;
    CREDO_NIHIL(historia_creare(piscina, intern, vol, "", "numerus",
                                "probatio:checkpoint", II, mala));
    mala          = proiectio(&l);
    mala.mensura  = ZEPHYRUM;
    CREDO_NIHIL(historia_creare(piscina, intern, vol, "", "numerus",
                                "probatio:checkpoint", II, mala));
    CREDO_NIHIL(historia_actum(NIHIL, chorda_ex_literis("1", piscina))
                == ZEPHYRUM ? NIHIL : h);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
