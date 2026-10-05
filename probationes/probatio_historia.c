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
    h = historia_creare(piscina, intern, vol, "numerus",
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
    h2 = historia_aperire(piscina, intern, vol, "numerus",
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
        h3 = historia_creare(piscina, intern, vol_discors, "numerus",
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

    imprimere("\n--- XII: argumenta mala ---\n");
    mala            = proiectio(&l);
    mala.applicare  = NIHIL;
    CREDO_NIHIL(historia_creare(piscina, intern, vol, "numerus",
                                "probatio:checkpoint", II, mala));
    mala          = proiectio(&l);
    mala.mensura  = ZEPHYRUM;
    CREDO_NIHIL(historia_creare(piscina, intern, vol, "numerus",
                                "probatio:checkpoint", II, mala));
    CREDO_NIHIL(historia_actum(NIHIL, chorda_ex_literis("1", piscina))
                == ZEPHYRUM ? NIHIL : h);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
