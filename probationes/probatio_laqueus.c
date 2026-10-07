/* probatio_laqueus.c - Probationes laqueorum polygonalium
 *
 * Valores noti (tabulae nodorum; Jones nodorum toroidalium per formulam
 * t^((p-1)(q-1)/2) (1 - t^(p+1) - t^(q+1) + t^(p+q)) / (1 - t^2)),
 * invariantia sub speculo, inversione, rotatione rationali, directione
 * proiectionis, motibus trianguli legitimis fortuitis; simplicitas,
 * genericitas, motus illegitimi. Polygona ex parametrizationibus
 * classicis in integros rotundatis (scratchpad laqueus/nodi_c.py) -
 * laqueus ipse exactus. Vide lib/laqueus.worklog.md.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "chorda.h"
#include "sors.h"
#include "fractio.h"
#include "situs.h"
#include "polynomium.h"
#include "laqueus.h"
#include <stdio.h>

interior constans s32 trifolium[][III] = {
    { 300, 0, 0 }, { 282, 76, -38 }, { 234, 135, -71 },
    { 168, 168, -92 }, { 100, 173, -100 }, { 42, 156, -92 },
    { 0, 129, -71 }, { -28, 104, -38 }, { -50, 87, 0 },
    { -76, 76, 38 }, { -112, 65, 71 }, { -156, 42, 92 },
    { -200, 0, 100 }, { -230, -62, 92 }, { -234, -135, 71 },
    { -207, -207, 38 }, { -150, -260, 0 }, { -76, -282, -38 },
    { 0, -271, -71 }, { 62, -230, -92 }, { 100, -173, -100 },
    { 114, -114, -92 }, { 112, -65, -71 }, { 104, -28, -38 },
    { 100, 0, 0 }, { 104, 28, 38 }, { 112, 65, 71 }, { 114, 114, 92 },
    { 100, 173, 100 }, { 62, 230, 92 }, { 0, 271, 71 },
    { -76, 282, 38 }, { -150, 260, 0 }, { -207, 207, -38 },
    { -234, 135, -71 }, { -230, 62, -92 }, { -200, 0, -100 },
    { -156, -42, -92 }, { -112, -65, -71 }, { -76, -76, -38 },
    { -50, -87, 0 }, { -28, -104, 38 }, { 0, -129, 71 },
    { 42, -156, 92 }, { 100, -173, 100 }, { 168, -168, 92 },
    { 234, -135, 71 }, { 282, -76, 38 },
};

interior constans s32 octonaria[][III] = {
    { 300, 0, 0 }, { 288, 77, 34 }, { 255, 147, 64 },
    { 203, 203, 87 }, { 138, 240, 98 }, { 68, 255, 98 },
    { 0, 250, 87 }, { -61, 226, 64 }, { -109, 188, 34 },
    { -141, 141, 0 }, { -158, 91, -34 }, { -160, 43, -64 },
    { -150, 0, -87 }, { -131, -35, -98 }, { -107, -62, -98 },
    { -80, -80, -87 }, { -53, -92, -64 }, { -26, -98, -34 },
    { 0, -100, 0 }, { 26, -98, 34 }, { 53, -92, 64 }, { 80, -80, 87 },
    { 107, -62, 98 }, { 131, -35, 98 }, { 150, 0, 87 },
    { 160, 43, 64 }, { 158, 91, 34 }, { 141, 141, 0 },
    { 109, 188, -34 }, { 61, 226, -64 }, { 0, 250, -87 },
    { -68, 255, -98 }, { -138, 240, -98 }, { -203, 203, -87 },
    { -255, 147, -64 }, { -288, 77, -34 }, { -300, 0, 0 },
    { -288, -77, 34 }, { -255, -147, 64 }, { -203, -203, 87 },
    { -138, -240, 98 }, { -68, -255, 98 }, { 0, -250, 87 },
    { 61, -226, 64 }, { 109, -188, 34 }, { 141, -141, 0 },
    { 158, -91, -34 }, { 160, -43, -64 }, { 150, 0, -87 },
    { 131, 35, -98 }, { 107, 62, -98 }, { 80, 80, -87 },
    { 53, 92, -64 }, { 26, 98, -34 }, { 0, 100, 0 }, { -26, 98, 34 },
    { -53, 92, 64 }, { -80, 80, 87 }, { -107, 62, 98 },
    { -131, 35, 98 }, { -150, 0, 87 }, { -160, -43, 64 },
    { -158, -91, 34 }, { -141, -141, 0 }, { -109, -188, -34 },
    { -61, -226, -64 }, { 0, -250, -87 }, { 68, -255, -98 },
    { 138, -240, -98 }, { 203, -203, -87 }, { 255, -147, -64 },
    { 288, -77, -34 },
};

interior constans s32 quinquefolium[][III] = {
    { 300, 0, 0 }, { 289, 46, -38 }, { 257, 84, -71 },
    { 212, 108, -92 }, { 162, 118, -100 }, { 114, 114, -92 },
    { 76, 105, -71 }, { 49, 96, -38 }, { 31, 95, 0 }, { 17, 106, 38 },
    { 0, 129, 71 }, { -25, 160, 92 }, { -62, 190, 100 },
    { -108, 212, 92 }, { -159, 219, 71 }, { -207, 207, 38 },
    { -243, 176, 0 }, { -261, 133, -38 }, { -257, 84, -71 },
    { -235, 37, -92 }, { -200, 0, -100 }, { -160, -25, -92 },
    { -123, -40, -71 }, { -96, -49, -38 }, { -81, -59, 0 },
    { -76, -76, 38 }, { -76, -105, 71 }, { -73, -144, 92 },
    { -62, -190, 100 }, { -37, -235, 92 }, { 0, -271, 71 },
    { 46, -289, 38 }, { 93, -285, 0 }, { 133, -261, -38 },
    { 159, -219, -71 }, { 168, -168, -92 }, { 162, -118, -100 },
    { 144, -73, -92 }, { 123, -40, -71 }, { 106, -17, -38 },
    { 100, 0, 0 }, { 106, 17, 38 }, { 123, 40, 71 }, { 144, 73, 92 },
    { 162, 118, 100 }, { 168, 168, 92 }, { 159, 219, 71 },
    { 133, 261, 38 }, { 93, 285, 0 }, { 46, 289, -38 },
    { 0, 271, -71 }, { -37, 235, -92 }, { -62, 190, -100 },
    { -73, 144, -92 }, { -76, 105, -71 }, { -76, 76, -38 },
    { -81, 59, 0 }, { -96, 49, 38 }, { -123, 40, 71 },
    { -160, 25, 92 }, { -200, 0, 100 }, { -235, -37, 92 },
    { -257, -84, 71 }, { -261, -133, 38 }, { -243, -176, 0 },
    { -207, -207, -38 }, { -159, -219, -71 }, { -108, -212, -92 },
    { -62, -190, -100 }, { -25, -160, -92 }, { 0, -129, -71 },
    { 17, -106, -38 }, { 31, -95, 0 }, { 49, -96, 38 },
    { 76, -105, 71 }, { 114, -114, 92 }, { 162, -118, 100 },
    { 212, -108, 92 }, { 257, -84, 71 }, { 289, -46, 38 },
};

interior constans s32 septifolium[][III] = {
    { 300, 0, 0 }, { 291, 33, -38 }, { 264, 60, -71 },
    { 225, 79, -92 }, { 180, 87, -100 }, { 137, 86, -92 },
    { 101, 81, -71 }, { 76, 76, -38 }, { 62, 78, 0 }, { 57, 91, 38 },
    { 56, 116, 71 }, { 53, 153, 92 }, { 45, 195, 100 },
    { 27, 237, 92 }, { 0, 271, 71 }, { -33, 291, 38 },
    { -67, 292, 0 }, { -97, 276, -38 }, { -117, 244, -71 },
    { -127, 202, -92 }, { -125, 156, -100 }, { -114, 114, -92 },
    { -101, 81, -71 }, { -91, 57, -38 }, { -90, 43, 0 },
    { -102, 36, 38 }, { -126, 29, 71 }, { -161, 18, 92 },
    { -200, 0, 100 }, { -237, -27, 92 }, { -264, -60, 71 },
    { -276, -97, 38 }, { -270, -130, 0 }, { -248, -156, -38 },
    { -212, -169, -71 }, { -168, -168, -92 }, { -125, -156, -100 },
    { -86, -137, -92 }, { -56, -116, -71 }, { -36, -102, -38 },
    { -22, -97, 0 }, { -12, -107, 38 }, { 0, -129, 71 },
    { 18, -161, 92 }, { 45, -195, 100 }, { 79, -225, 92 },
    { 117, -244, 71 }, { 156, -248, 38 }, { 187, -235, 0 },
    { 207, -207, -38 }, { 212, -169, -71 }, { 202, -127, -92 },
    { 180, -87, -100 }, { 153, -53, -92 }, { 126, -29, -71 },
    { 107, -12, -38 }, { 100, 0, 0 }, { 107, 12, 38 },
    { 126, 29, 71 }, { 153, 53, 92 }, { 180, 87, 100 },
    { 202, 127, 92 }, { 212, 169, 71 }, { 207, 207, 38 },
    { 187, 235, 0 }, { 156, 248, -38 }, { 117, 244, -71 },
    { 79, 225, -92 }, { 45, 195, -100 }, { 18, 161, -92 },
    { 0, 129, -71 }, { -12, 107, -38 }, { -22, 97, 0 },
    { -36, 102, 38 }, { -56, 116, 71 }, { -86, 137, 92 },
    { -125, 156, 100 }, { -168, 168, 92 }, { -212, 169, 71 },
    { -248, 156, 38 }, { -270, 130, 0 }, { -276, 97, -38 },
    { -264, 60, -71 }, { -237, 27, -92 }, { -200, 0, -100 },
    { -161, -18, -92 }, { -126, -29, -71 }, { -102, -36, -38 },
    { -90, -43, 0 }, { -91, -57, 38 }, { -101, -81, 71 },
    { -114, -114, 92 }, { -125, -156, 100 }, { -127, -202, 92 },
    { -117, -244, 71 }, { -97, -276, 38 }, { -67, -292, 0 },
    { -33, -291, -38 }, { 0, -271, -71 }, { 27, -237, -92 },
    { 45, -195, -100 }, { 53, -153, -92 }, { 56, -116, -71 },
    { 57, -91, -38 }, { 62, -78, 0 }, { 76, -76, 38 },
    { 101, -81, 71 }, { 137, -86, 92 }, { 180, -87, 100 },
    { 225, -79, 92 }, { 264, -60, 71 }, { 291, -33, 38 },
};

interior constans s32 torus_iii_iv[][III] = {
    { 300, 0, 0 }, { 294, 47, -21 }, { 277, 90, -41 },
    { 250, 128, -59 }, { 216, 157, -74 }, { 177, 177, -87 },
    { 136, 187, -95 }, { 96, 188, -99 }, { 59, 180, -99 },
    { 26, 167, -95 }, { 0, 150, -87 }, { -21, 131, -74 },
    { -37, 113, -59 }, { -49, 97, -41 }, { -60, 83, -21 },
    { -71, 71, 0 }, { -83, 60, 21 }, { -97, 49, 41 },
    { -113, 37, 59 }, { -131, 21, 74 }, { -150, 0, 87 },
    { -167, -26, 95 }, { -180, -59, 99 }, { -188, -96, 99 },
    { -187, -136, 95 }, { -177, -177, 87 }, { -157, -216, 74 },
    { -128, -250, 59 }, { -90, -277, 41 }, { -47, -294, 21 },
    { 0, -300, 0 }, { 47, -294, -21 }, { 90, -277, -41 },
    { 128, -250, -59 }, { 157, -216, -74 }, { 177, -177, -87 },
    { 187, -136, -95 }, { 188, -96, -99 }, { 180, -59, -99 },
    { 167, -26, -95 }, { 150, 0, -87 }, { 131, 21, -74 },
    { 113, 37, -59 }, { 97, 49, -41 }, { 83, 60, -21 }, { 71, 71, 0 },
    { 60, 83, 21 }, { 49, 97, 41 }, { 37, 113, 59 }, { 21, 131, 74 },
    { 0, 150, 87 }, { -26, 167, 95 }, { -59, 180, 99 },
    { -96, 188, 99 }, { -136, 187, 95 }, { -177, 177, 87 },
    { -216, 157, 74 }, { -250, 128, 59 }, { -277, 90, 41 },
    { -294, 47, 21 }, { -300, 0, 0 }, { -294, -47, -21 },
    { -277, -90, -41 }, { -250, -128, -59 }, { -216, -157, -74 },
    { -177, -177, -87 }, { -136, -187, -95 }, { -96, -188, -99 },
    { -59, -180, -99 }, { -26, -167, -95 }, { 0, -150, -87 },
    { 21, -131, -74 }, { 37, -113, -59 }, { 49, -97, -41 },
    { 60, -83, -21 }, { 71, -71, 0 }, { 83, -60, 21 },
    { 97, -49, 41 }, { 113, -37, 59 }, { 131, -21, 74 },
    { 150, 0, 87 }, { 167, 26, 95 }, { 180, 59, 99 }, { 188, 96, 99 },
    { 187, 136, 95 }, { 177, 177, 87 }, { 157, 216, 74 },
    { 128, 250, 59 }, { 90, 277, 41 }, { 47, 294, 21 }, { 0, 300, 0 },
    { -47, 294, -21 }, { -90, 277, -41 }, { -128, 250, -59 },
    { -157, 216, -74 }, { -177, 177, -87 }, { -187, 136, -95 },
    { -188, 96, -99 }, { -180, 59, -99 }, { -167, 26, -95 },
    { -150, 0, -87 }, { -131, -21, -74 }, { -113, -37, -59 },
    { -97, -49, -41 }, { -83, -60, -21 }, { -71, -71, 0 },
    { -60, -83, 21 }, { -49, -97, 41 }, { -37, -113, 59 },
    { -21, -131, 74 }, { 0, -150, 87 }, { 26, -167, 95 },
    { 59, -180, 99 }, { 96, -188, 99 }, { 136, -187, 95 },
    { 177, -177, 87 }, { 216, -157, 74 }, { 250, -128, 59 },
    { 277, -90, 41 }, { 294, -47, 21 },
};

interior Piscina* piscina;

interior Laqueus
_ex_tabula (
    constans s32 (*tabula)[III],
                 i32 numerus)
{
    Punctum* puncta = (Punctum*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(Punctum));
        i32 initia[II];
        i32 k;
    Laqueus l;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        puncta[k] = situs_punctum(tabula[k][ZEPHYRUM], tabula[k][I],
            tabula[k][II]);
    }
    initia[ZEPHYRUM]  = ZEPHYRUM;
    initia[I]         = numerus;
    (vacuum)laqueus_ex_punctis(puncta, initia, I, piscina, &l);
    redde l;
}

interior Laqueus
_ex_textu (
    constans character* textus)
{
    Laqueus l;

    si (!laqueus_ex_chorda(chorda_ex_literis(textus, piscina), piscina,
        &l))
    {
        imprimere("  lectio fracta: %s\n", textus);
    }
    redde l;
}

/* triangulum ex tabula in ordine verticum dato (sex permutationes:
 * vertex plicatus in omni positione, ambo sensus) */
interior Laqueus
_triangulum (
    constans s32 (*tabula)[III],
                 i32 ordo)
{
    s32 ordines[VI][III] = {
        { 0, 1, 2 }, { 1, 2, 0 }, { 2, 0, 1 },
        { 0, 2, 1 }, { 2, 1, 0 }, { 1, 0, 2 }
    };
    s32 puncta[III][III];
    i32 k;
    i32 m;

    per (k = ZEPHYRUM; k < III; k++)
    {
        per (m = ZEPHYRUM; m < III; m++)
        {
            puncta[k][m] = tabula[ordines[ordo][k]][m];
        }
    }
    redde _ex_tabula((constans s32 (*)[III])puncta, III);
}

/* coordinatae per (10^9 + 7) / (10^6 + 3) multiplicatae: geometria
 * eadem (parallelismus, transitus), sed producta s64 excedunt -
 * fractio magnis utitur et piscinam vere consumit */
interior Laqueus
_dilatatus (
    Laqueus l)
{
        i32  n      = laqueus_numerus(l);
    Punctum* puncta = (Punctum*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(Punctum));
    i32* initia = (i32*)piscina_allocare(piscina, (memoriae_index)(
        laqueus_componentes(l) + I) * magnitudo(i32));
    Fractio factor;
        i32 k;
    Laqueus r;

    (vacuum)fractio_ex_s64_s64((s64)1000000007, (s64)1000003, piscina,
        &factor);
    per (k = ZEPHYRUM; k < n; k++)
    {
        Punctum v = laqueus_vertex(l, k);

        puncta[k].x = fractio_multiplica(v.x, factor, piscina);
        puncta[k].y = fractio_multiplica(v.y, factor, piscina);
        puncta[k].z = fractio_multiplica(v.z, factor, piscina);
    }
    per (k = ZEPHYRUM; k <= laqueus_componentes(l); k++)
    {
        initia[k] = l.initia[k];
    }
    (vacuum)laqueus_ex_punctis(puncta, initia, laqueus_componentes(l),
        piscina, &r);
    redde r;
}

/* transformatio puncti: speculum (z -> -z), rotatio rationalis per
 * quaternionem (1, 2, 3, 4) / 30 et translatio */
interior Laqueus
_transformatus (
    Laqueus l,
        i32 modus)
{
        i32  n      = laqueus_numerus(l);
    Punctum* puncta = (Punctum*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(Punctum));
    i32* initia = (i32*)piscina_allocare(piscina, (memoriae_index)(
        laqueus_componentes(l) + I) * magnitudo(i32));
        i32 k;
    Laqueus r;
    /* R = (1/30) matrix rotationis quaternionis q = (1, 2, 3, 4):
     * [[a2+b2-c2-d2, 2(bc-ad), 2(bd+ac)], [2(bc+ad), a2-b2+c2-d2,
     * 2(cd-ab)], [2(bd-ac), 2(cd+ab), a2-b2-c2+d2]] */
    s32 rotatio[III][III] = {
        { -10, 4, 22 }, { 20, -10, 20 }, { 10, 28, 4 }
    };

    initia[ZEPHYRUM] = ZEPHYRUM;
    per (k = ZEPHYRUM; k < n; k++)
    {
        Punctum p = laqueus_vertex(l, k);

        si (modus == ZEPHYRUM)
        {
            p.z = fractio_nega(p.z, piscina);
        }
        alioquin
        {
            Fractio coordinatae[III];
            Punctum q;
                i32 i;

            coordinatae[ZEPHYRUM]  = p.x;
            coordinatae[I]         = p.y;
            coordinatae[II]        = p.z;
            per (i = ZEPHYRUM; i < III; i++)
            {
                Fractio summa     = fractio_ex_s64(ZEPHYRUM);
                Fractio triginta  = fractio_ex_s64(XXX);
                    i32 j;

                per (j = ZEPHYRUM; j < III; j++)
                {
                    summa = fractio_adde(summa, fractio_multiplica(
                        fractio_ex_s64(rotatio[i][j]), coordinatae[j],
                        piscina), piscina);
                }
                (vacuum)fractio_divide(summa, triginta, piscina,
                    &summa);
                si (i == ZEPHYRUM)
                {
                    q.x = summa;
                }
                alioquin si (i == I)
                {
                    q.y = summa;
                }
                alioquin
                {
                    q.z = fractio_adde(summa, fractio_ex_s64(V),
                        piscina);
                }
            }
            p = q;
        }
        puncta[k] = p;
    }
    initia[I] = n;
    (vacuum)laqueus_ex_punctis(puncta, initia, I, piscina, &r);
    redde r;
}

/* ordo verticum inversus (orientatio contraria), componens una */
interior Laqueus
_inversus (
    Laqueus l)
{
        i32  n      = laqueus_numerus(l);
    Punctum* puncta = (Punctum*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(Punctum));
        i32 initia[II];
        i32 k;
    Laqueus r;

    per (k = ZEPHYRUM; k < n; k++)
    {
        puncta[k] = laqueus_vertex(l, n - I - k);
    }
    initia[ZEPHYRUM]  = ZEPHYRUM;
    initia[I]         = n;
    (vacuum)laqueus_ex_punctis(puncta, initia, I, piscina, &r);
    redde r;
}

/* invariantes per diagramma genericum; FALSUM si quid deficit */
interior b32
_invariantes (
       Laqueus  l,
        chorda* jones,
        chorda* alexander,
           s32* scriptura)
{
     Diagramma d;
    Polynomium j;
    Polynomium a;

    si (   !laqueus_diagramma_genericum(l, piscina, &d)
        || !diagramma_jones(d, piscina, &j)
        || !diagramma_alexander(d, piscina, &a))
    {
        redde FALSUM;
    }
    *jones      = polynomium_ad_chordam(j, 't', piscina);
    *alexander  = polynomium_ad_chordam(a, 't', piscina);
    *scriptura  = diagramma_scriptura(d);
    redde VERUM;
}

/* Jones speculi = Jones(t^-1) */
interior chorda
_speculum_jones (
    chorda jones)
{
    Polynomium j;
    Polynomium s;

    (vacuum)polynomium_ex_chorda(jones, 't', piscina, &j);
    (vacuum)polynomium_dilata(j, -I, piscina, &s);
    redde polynomium_ad_chordam(s, 't', piscina);
}

s32 principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_laqueus", 1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * TEXTUS ET SIMPLICITAS
     * ================================================== */

    {
                   Laqueus  l;
                   Laqueus  custos;
        constans character* mala[] = {
            "", "[]", "[(0, 0, 0), (1, 0, 0)]",
                "[(0, 0), (1, 0, 0), (0, 1, 0)]",
            "[(0, 0, 0) (1, 0, 0), (0, 1, 0)]",
            "[(0, 0, 0), (1, 0, 0), (0, 1, 0); (5, 5, 5)]",
            "[(0, 0, 0), (1, 0, 0), (0, 1, x)]", "(0, 0, 0)"
        };
        i32 k;
        b32 bene = VERUM;

        imprimere("\n--- Probans textum et simplicitatem ---\n");
        custos  = _ex_textu("[(0, 0, 0), (1, 0, 0), (0, 1, 0)]");
        l       = _ex_textu("[(0, 0, 0), (1/2, 0, 0), (0, 1, -3)]");
        CREDO_CHORDA_AEQUALIS_LITERIS (laqueus_ad_chordam(l, piscina),
            "[(0, 0, 0), (1/2, 0, 0), (0, 1, -3)]");
        CREDO_AEQUALIS_I32 (laqueus_numerus(l), III);
        CREDO_AEQUALIS_I32 (laqueus_componentes(l), I);
        l = _ex_textu("[(0,0,0),(1,0,0),(0,1,0);"
            "(5,5,5),(6,5,5),(5,6,5)]");
        CREDO_AEQUALIS_I32 (laqueus_componentes(l), II);
        CREDO_CHORDA_AEQUALIS_LITERIS (laqueus_ad_chordam(l, piscina),
            "[(0, 0, 0), (1, 0, 0), (0, 1, 0); (5, 5, 5), (6, 5, 5), "
            "(5, 6, 5)]");
        per (k = ZEPHYRUM; k < (i32)(magnitudo(mala) / magnitudo(mala[
            ZEPHYRUM])); k++)
        {
            l = custos;
            si (   laqueus_ex_chorda(chorda_ex_literis(mala[k],
                piscina),
                    piscina, &l)
                || laqueus_numerus(l) != III)
            {
                imprimere("  acceptum: %s\n", mala[k]);
                bene = FALSUM;
            }
        }
        CREDO_VERUM (bene);

        CREDO_VERUM (laqueus_simplex(_ex_textu(
            "[(0, 0, 0), (10, 0, 0), (10, 10, 0), (0, 10, 0)]"),
            piscina));
        /* papilio: diagonales se secant */
        CREDO_FALSUM (laqueus_simplex(_ex_textu(
            "[(0, 0, 0), (10, 10, 0), (10, 0, 0), (0, 10, 0)]"),
            piscina));
        /* reflexio: segmenta contigua superposita */
        CREDO_FALSUM (laqueus_simplex(_ex_textu(
            "[(0, 0, 0), (10, 0, 0), (5, 0, 0), (5, 5, 0)]"), piscina));
        /* triangulum collineare: omnia paria contigua (solae
         * probationes vicinorum id capiunt) - omnes ordines verticum
         * (recensio laqueus-I, M37: ordo unus ramum unum non tegit) */
        {
            constans s32 collineare[III][III] = {
                { 0, 0, 0 }, { 10, 0, 0 }, { 5, 0, 0 }
            };
            i32 o;

            per (o = ZEPHYRUM; o < VI; o++)
            {
                CREDO_FALSUM (laqueus_simplex(_triangulum(collineare,
                    o),
                    piscina));
            }
        }
        /* componentes se tangunt */
        CREDO_FALSUM (laqueus_simplex(_ex_textu(
            "[(0, 0, 0), (10, 0, 0), (0, 10, 0); (5, 0, -5), "
            "(5, 0, 5), (5, 5, 5)]"), piscina));
    }


    /* ==================================================
     * GENERICITAS ET CATENAE
     * ================================================== */

    {
         Diagramma d;
        Polynomium u;
        Polynomium j;
           Laqueus quadratum = _ex_textu(
               "[(0, 0, 0), (10, 0, 0), (10, 10, 0), (0, 10, 0)]");
        Laqueus hopf =
            _ex_textu("[(0, 0, 0), (20, 0, 0), (20, 20, 0), "
            "(0, 20, 0); (10, 10, -10), (10, 10, 10), (30, 10, 10), "
            "(30, 10, -10)]");
        Laqueus disiuncti = _ex_textu("[(0, 0, 0), (10, 0, 0), "
            "(10, 10, 0); (50, 0, 0), (60, 0, 0), (60, 10, 0)]");

        imprimere("\n--- Probans genericitatem et catenas ---\n");
        /* proiectio secundum latus quadrati: non generica */
        CREDO_FALSUM (laqueus_diagramma(quadratum, situs_punctum(I,
            ZEPHYRUM,
            ZEPHYRUM), piscina, &d));
        CREDO_FALSUM (laqueus_diagramma(hopf, situs_punctum(ZEPHYRUM,
            ZEPHYRUM, I), piscina, &d));
        /* triangula in plano xz secundum z: proiectio in rectam -
         * paria omnia contigua, solae probationes vicinorum capiunt;
         * omnes ordines verticum (recensio laqueus-I, M32) */
        {
            constans s32 plani[II][III][III] = {
                { { 0, 0, 0 }, { 10, 0, 0 }, { 5, 0, 5 } },
                { { 5, 0, 0 }, { 10, 0, 0 }, { 0, 0, 5 } }
            };
            i32 o;
            i32 m;

            per (m = ZEPHYRUM; m < II; m++)
            {
                per (o = ZEPHYRUM; o < VI; o++)
                {
                    Laqueus t = _triangulum(plani[m], o);

                    CREDO_FALSUM (laqueus_diagramma(t, situs_punctum(
                        ZEPHYRUM, ZEPHYRUM, I), piscina, &d));
                }
            }
        }
        /* punctum triplex: tria segmenta per (0, 0) altitudinibus 0, 5,
         * 10 */
        CREDO_FALSUM (laqueus_diagramma(_ex_textu(
            "[(-10, 0, 0), (10, 0, 0), (0, -30, 0); (0, -10, 5), "
            "(0, 10, 5), (-30, 3, 5); (-10, -10, 10), (10, 10, 10), "
            "(20, -25, 10)]"), situs_punctum(ZEPHYRUM, ZEPHYRUM, I),
            piscina,
            &d));
        CREDO_VERUM (laqueus_simplex(hopf, piscina));
        CREDO_VERUM (laqueus_diagramma_genericum(hopf, piscina, &d));
        CREDO_AEQUALIS_I32 (diagramma_numerus(d), II);
        /* regula dextrae: C1 contra horologium in z = 0, C2 per discum
         * sursum ad (10, 10) - ligatio +1, conventio signi physica */
        CREDO_AEQUALIS_S32 (diagramma_numerus_ligationis(d, ZEPHYRUM,
            I),
            I);
        CREDO_VERUM (diagramma_uncinus(d, piscina, &u));
        /* uncinus Hopf sub speculo invariabilis (A -> A^-1) */
        CREDO_CHORDA_AEQUALIS_LITERIS (polynomium_ad_chordam(u, 'A',
            piscina), "-A^4 - A^-4");
        /* Jones Hopf exponentes dimidios habet; Alexander nodorum solum
         */
        CREDO_FALSUM (diagramma_jones(d, piscina, &j));
        CREDO_FALSUM (diagramma_alexander(d, piscina, &j));
        /* catena disiuncta: uncinus = d = -A^2 - A^-2, ligatio 0 */
        CREDO_VERUM (laqueus_diagramma_genericum(disiuncti, piscina,
            &d));
        CREDO_AEQUALIS_S32 (diagramma_numerus_ligationis(d, ZEPHYRUM,
            I),
            ZEPHYRUM);
        CREDO_VERUM (diagramma_uncinus(d, piscina, &u));
        CREDO_CHORDA_AEQUALIS_LITERIS (polynomium_ad_chordam(u, 'A',
            piscina), "-A^2 - A^-2");

        /* transitus Hopf secundum (1, 2, 3), manu computati (spectator
         * ad +v: punctum maioris v-componentis supra): segmentum 5
         * ((10,10,10)-(30,10,10)) supra 1 ((20,0,0)-(20,20,0)) ad 2/3,
         * 1/6 - differentia (10/3)(1, 2, 3); segmentum 1 supra 7
         * ((30,10,-10)-(10,10,-10)) ad 5/6, 2/3 - differentia
         * -(10/3)(1, 2, 3). Supra/infra permutata invariantes omnes
         * servant (rotatio pi circa axem in plano): solum hic capitur
         * (recensio laqueus-I, M30) */
        {
            i32 k;
            i32 visa = ZEPHYRUM;

            CREDO_VERUM (laqueus_diagramma(hopf, situs_punctum(I, II,
                III), piscina, &d));
            CREDO_AEQUALIS_I32 (diagramma_numerus(d), II);
            per (k = ZEPHYRUM; k < II && k < diagramma_numerus(d); k++)
            {
                Transitus t   = diagramma_transitus(d, k);
                   chorda pa  = fractio_ad_chordam(t.parametrum_supra,
                       piscina);
                   chorda pb  = fractio_ad_chordam(t.parametrum_infra,
                       piscina);

                CREDO_AEQUALIS_S32 (t.signum, I);
                si (t.supra == V)
                {
                    CREDO_AEQUALIS_I32 (t.infra, I);
                    CREDO_CHORDA_AEQUALIS_LITERIS (pa, "2/3");
                    CREDO_CHORDA_AEQUALIS_LITERIS (pb, "1/6");
                    visa++;
                }
                alioquin
                {
                    CREDO_AEQUALIS_I32 (t.supra, I);
                    CREDO_AEQUALIS_I32 (t.infra, VII);
                    CREDO_CHORDA_AEQUALIS_LITERIS (pa, "5/6");
                    CREDO_CHORDA_AEQUALIS_LITERIS (pb, "2/3");
                }
            }
            CREDO_AEQUALIS_I32 (visa, I);
        }
    }


    /* ==================================================
     * DIRECTIONES ET MEMORIA
     * ================================================== */

    {
        /* polygonum simplex XIII verticum, segmentum parallelum cuique
         * directioni seriei fixae (recensio laqueus-I, B): series
         * exhauritur, curva momentorum (1, k, k^2) invenit */
        Laqueus adversus =
            _ex_textu("[(0, 0, 0), (0, 0, 1), (1, 2, 4), "
            "(3, 5, 9), (6, 10, 16), (7, 8, 20), (12, 5, 22), "
            "(19, 16, 35), (17, 19, 46), (30, 12, 51), (31, 13, 68), "
            "(50, -10, 97), (81, 27, 56)]");
        Laqueus magnus_adversus = _dilatatus(adversus);
        Laqueus magnum_trifolium = _dilatatus(_ex_tabula(trifolium,
            XLVIII));
               Diagramma d;
               Diagramma e;
              Polynomium a;
                     b32 inventum;
                     b32 directum;
                     b32 trifolii;
                     i32 numerus_trifolii;
          memoriae_index ante;
          memoriae_index usus_genericus;
          memoriae_index usus_directus;
          memoriae_index usus_trifolii;

        imprimere("\n--- Probans directiones et memoriam ---\n");
        CREDO_VERUM (laqueus_simplex(adversus, piscina));
        CREDO_FALSUM (laqueus_diagramma(adversus,
            situs_punctum(ZEPHYRUM,
            ZEPHYRUM, I), piscina, &d));
        CREDO_VERUM (laqueus_diagramma_genericum(adversus, piscina,
            &d));
        CREDO_VERUM (diagramma_alexander(d, piscina, &a));
        CREDO_CHORDA_AEQUALIS_LITERIS (polynomium_ad_chordam(a, 't',
            piscina), "1");
        /* non simplex: nulla directio generica - refutatur, non errat
         */
        CREDO_FALSUM (laqueus_diagramma_genericum(_ex_textu(
            "[(0, 0, 0), (10, 0, 0), (0, 10, 0); (5, 0, -5), "
            "(5, 0, 5), (5, 5, 5)]"), piscina, &d));

        /* conatus irriti reficiuntur (M39): genericum (series tota +
         * curva) non plus consumit quam diagramma directionis inventae.
         * Mensurae sine CREDO intermixto (CREDO in eadem piscina
         * scribit) */
        ante            = piscina_summa_usus(piscina);
        inventum        = laqueus_diagramma_genericum(magnus_adversus,
            piscina, &d);
        usus_genericus  = piscina_summa_usus(piscina) - ante;
        ante            = piscina_summa_usus(piscina);
        directum        = laqueus_diagramma(magnus_adversus, d.directio,
            piscina, &e);
        usus_directus   = piscina_summa_usus(piscina) - ante;
        /* parametri non SECANT reficiuntur (recensio laqueus-I, A):
         * sine refectione O(n^2) */
        ante              = piscina_summa_usus(piscina);
        trifolii          = laqueus_diagramma(magnum_trifolium,
            situs_punctum(II, III, V), piscina, &e);
        usus_trifolii     = piscina_summa_usus(piscina) - ante;
        numerus_trifolii  = diagramma_numerus(e);
        imprimere("  usus: genericus %lu, directus %lu, "
            "trifolium %lu\n", (unsigned long)usus_genericus,
            (unsigned long)usus_directus,
            (unsigned long)usus_trifolii);
        CREDO_VERUM (inventum);
        CREDO_VERUM (directum);
        CREDO_VERUM (trifolii);
        CREDO_AEQUALIS_I32 (numerus_trifolii, III);
        CREDO_VERUM (usus_genericus <= usus_directus + usus_directus
            / II);
        CREDO_VERUM (usus_trifolii < (memoriae_index)CCLVI * M);

        /* XXV transitus: uncinus et Jones refutant (limes XXIV; XXIV
         * ipse non probatur - 2^24 status, secunda septem -O2) */
        {
            Punctum puncta[XXVIII];
                i32 initia[II];
                i32 k;
            Laqueus pecten;

            /* pecten: zigzag (2k, 0|10, 0) k = 0..25, deinde (52, 5, 5)
             * et recta reditus y = 5, z = 5 ad (-2, 5, 5): quodque
             * segmentum zigzag semel secat */
            per (k = ZEPHYRUM; k <= XXV; k++)
            {
                puncta[k] = situs_punctum((s64)(II * k), (k % II) ? X
                    : ZEPHYRUM, ZEPHYRUM);
            }
            puncta[XXVI]      = situs_punctum(LII, V, V);
            puncta[XXVII]     = situs_punctum(-II, V, V);
            initia[ZEPHYRUM]  = ZEPHYRUM;
            initia[I]         = XXVIII;
            CREDO_VERUM (laqueus_ex_punctis(puncta, initia, I, piscina,
                &pecten));
            CREDO_VERUM (laqueus_diagramma(pecten,
                situs_punctum(ZEPHYRUM,
                ZEPHYRUM, I), piscina, &d));
            CREDO_AEQUALIS_I32 (diagramma_numerus(d), XXV);
            CREDO_FALSUM (diagramma_uncinus(d, piscina, &a));
            CREDO_FALSUM (diagramma_jones(d, piscina, &a));
            CREDO_VERUM (diagramma_alexander(d, piscina, &a));
            CREDO_CHORDA_AEQUALIS_LITERIS (polynomium_ad_chordam(a, 't',
                piscina), "1");
        }
    }


    /* ==================================================
     * VALORES NOTI ET INVARIANTIA
     * ================================================== */

    {
        nomen structura {
            constans character* titulus;
            constans s32      (*tabula)[III];
                           i32  numerus;
                           s32  scriptura;
            constans character* jones;
            constans character* alexander;
        } Notus;
        Notus noti[] = {
            { "trifolium", trifolium, XLVIII, III, "-t^4 + t^3 + t",
              "t^2 - t + 1" },
            { "octonaria", octonaria, LXXII, ZEPHYRUM,
              "t^2 - t + 1 - t^-1 + t^-2", "t^2 - 3t + 1" },
            { "quinquefolium", quinquefolium, LXXX, VI,
              "-t^7 + t^6 - t^5 + t^4 + t^2",
                  "t^4 - t^3 + t^2 - t + 1" },
            { "septifolium", septifolium, CXII, IX,
              "-t^10 + t^9 - t^8 + t^7 - t^6 + t^5 + t^3",
              "t^6 - t^5 + t^4 - t^3 + t^2 - t + 1" },
            { "torus (3, 4)", torus_iii_iv, CXX, VIII,
                "-t^8 + t^5 + t^3",
              "t^6 - t^5 + t^3 - t + 1" }
        };
        i32 k;

        imprimere("\n--- Probans valores notos et invariantiam ---\n");
        per (k = ZEPHYRUM; k < (i32)(magnitudo(noti) / magnitudo(noti[
            ZEPHYRUM])); k++)
        {
            Laqueus l = _ex_tabula(noti[k].tabula, noti[k].numerus);
             chorda jones;
             chorda alexander;
                s32 w = ZEPHYRUM;
             chorda jones_alius;
             chorda alexander_alius;
                s32 w_alius = ZEPHYRUM;
                b32 bene;

            bene = laqueus_simplex(l, piscina)
                && _invariantes(l, &jones, &alexander, &w)
                && chorda_aequalis_literis(jones, noti[k].jones)
                && chorda_aequalis_literis(alexander, noti[k].alexander)
                && w == noti[k].scriptura;
            si (!bene)
            {
                imprimere("  %s: valores\n", noti[k].titulus);
            }
            CREDO_VERUM (bene);

            /* speculum: Jones(t^-1), Alexander idem; scriptura -w solum
             * in directione speculata ((1,2,3) -> (1,2,-3)): diagramma
             * aliud aliter (septifolium: 9 contra 8 transitus) */
            bene = _invariantes(_transformatus(l, ZEPHYRUM),
                &jones_alius,
                    &alexander_alius, &w_alius)
                && chorda_aequalis(jones_alius, _speculum_jones(jones))
                && chorda_aequalis(alexander_alius, alexander);
            {
                /* directio prima generica laquei; speculum in
                 * directione speculata generica esse DEBET (symmetria)
                 */
                s32 directiones[IV][III] = {
                    { I, II, III }, { II, III, V }, { III, V, VII },
                    { I, -II, IV }
                };
                Diagramma d;
                Diagramma d_speculi;
                      i32 m = ZEPHYRUM;

                dum (   m < IV && !laqueus_diagramma(l, situs_punctum(
                    directiones[m][ZEPHYRUM], directiones[m][I],
                    directiones[m][II]), piscina, &d))
                {
                    m++;
                }
                bene = bene && m < IV
                    && laqueus_diagramma(_transformatus(l, ZEPHYRUM),
                        situs_punctum(directiones[m][ZEPHYRUM],
                        directiones[m][I], -directiones[m][II]),
                        piscina,
                        &d_speculi)
                    && diagramma_scriptura(d_speculi)
                        == -diagramma_scriptura(d)
                    && diagramma_numerus(d_speculi)
                        == diagramma_numerus(d);
            }
            si (!bene)
            {
                imprimere("  %s: speculum\n", noti[k].titulus);
            }
            CREDO_VERUM (bene);

            /* orientatio contraria: idem */
            bene = _invariantes(_inversus(l), &jones_alius,
                    &alexander_alius, &w_alius)
                && chorda_aequalis(jones_alius, jones)
                && chorda_aequalis(alexander_alius, alexander);
            si (!bene)
            {
                imprimere("  %s: inversio\n", noti[k].titulus);
            }
            CREDO_VERUM (bene);

            /* rotatio rationalis et translatio: Jones, Alexander idem
             * (nodi minores soli: coordinatae /30 sumptuosae) */
            si (k >= II)
            {
                perge;
            }
            bene = laqueus_simplex(_transformatus(l, I), piscina)
                && _invariantes(_transformatus(l, I), &jones_alius,
                    &alexander_alius, &w_alius)
                && chorda_aequalis(jones_alius, jones)
                && chorda_aequalis(alexander_alius, alexander);
            si (!bene)
            {
                imprimere("  %s: rotatio\n", noti[k].titulus);
            }
            CREDO_VERUM (bene);
        }
    }


    /* ==================================================
     * MOTUS TRIANGULI
     * ================================================== */

    {
        Laqueus hopf = _ex_textu("[(0, 0, 0), (20, 0, 0), (20, 20, 0), "
            "(0, 20, 0); (10, 10, -10), (10, 10, 10), (30, 10, 10), "
            "(30, 10, -10)]");
        Laqueus r = hopf;
        Laqueus quinque =
            _ex_textu("[(0, 0, 0), (5, 0, 0), (10, 0, 0), "
            "(10, 10, 0), (0, 10, 0)]");
        Laqueus triangulum =
            _ex_textu("[(0, 0, 0), (10, 0, 0), (0, 10, 0)]");
        Laqueus l;
           Sors s;
         chorda jones;
         chorda alexander;
            s32 w;
            i32 facti     = ZEPHYRUM;
            i32 refutati  = ZEPHYRUM;
            i32 k;

        imprimere("\n--- Probans motus trianguli ---\n");
        /* triangulum (0,0,0)-(20,0,0)-(10,20,0) componentem alteram in
         * (10,10,0) transfigit: illegitimus; extrorsum legitimus */
        CREDO_FALSUM (laqueus_motus_addere(hopf, ZEPHYRUM,
            situs_punctum(X,
            XX, ZEPHYRUM), piscina, &r));
        CREDO_AEQUALIS_I32 (laqueus_numerus(r), VIII);
        CREDO_VERUM (laqueus_motus_addere(hopf, ZEPHYRUM,
            situs_punctum(X,
            -X, ZEPHYRUM), piscina, &r));
        CREDO_AEQUALIS_I32 (laqueus_numerus(r), IX);
        CREDO_VERUM (laqueus_simplex(r, piscina));
        /* triangulum in plano quadrati segmentum contiguum tegit: ad v0
         * (cono (10,0)..(-5,5) directio (0,1) segmenti prioris inest)
         * et ad v1 (cono (-10,0)..(5,5) directio (0,1) segmenti
         * sequentis) */
        {
            Laqueus quadratum = _ex_textu(
                "[(0, 0, 0), (10, 0, 0), (10, 10, 0), (0, 10, 0)]");

            CREDO_FALSUM (laqueus_motus_addere(quadratum, ZEPHYRUM,
                situs_punctum(-V, V, ZEPHYRUM), piscina, &l));
            CREDO_FALSUM (laqueus_motus_addere(quadratum, ZEPHYRUM,
                situs_punctum(XV, V, ZEPHYRUM), piscina, &l));
            CREDO_VERUM (laqueus_motus_addere(quadratum, ZEPHYRUM,
                situs_punctum(V, -V, ZEPHYRUM), piscina, &l));
        }
        /* triangulum in plano x = 0 (M43: _collinearia sine plano yz
         * id degenerem putaret) */
        CREDO_VERUM (laqueus_motus_addere(_ex_textu(
            "[(0, 0, 0), (0, 10, 0), (0, 10, 10), (0, 0, 10)]"),
            ZEPHYRUM,
            situs_punctum(ZEPHYRUM, V, -V), piscina, &l));
        /* removere (10,10,0) e (0,0,0)-(10,10,0)-(20,0,0): segmentum
         * sequens (20,0,0)-(30,5,0) in plano trianguli sed extra
         * angulum ad c - legitimus; (30,5) - a in angulo ad a, ergo
         * vicinum cum vertice communi falso (a pro c) refutaret (M26)
         */
        CREDO_VERUM (laqueus_motus_removere(_ex_textu(
            "[(0, 0, 0), (10, 10, 0), (20, 0, 0), (30, 5, 0), "
            "(30, 5, 10), (0, -5, 10), (0, -5, 0)]"), I, piscina, &l));
        CREDO_CHORDA_AEQUALIS_LITERIS (laqueus_ad_chordam(l, piscina),
            "[(0, 0, 0), (20, 0, 0), (30, 5, 0), (30, 5, 10), "
            "(0, -5, 10), (0, -5, 0)]");
        /* degener (punctum in recta segmenti): refutatur */
        CREDO_FALSUM (laqueus_motus_addere(hopf, ZEPHYRUM,
            situs_punctum(V,
            ZEPHYRUM, ZEPHYRUM), piscina, &l));
        /* vertex collinearis inter vicinos: removetur; triangulum
         * minimum: non */
        CREDO_VERUM (laqueus_motus_removere(quinque, I, piscina, &l));
        CREDO_CHORDA_AEQUALIS_LITERIS (laqueus_ad_chordam(l, piscina),
            "[(0, 0, 0), (10, 0, 0), (10, 10, 0), (0, 10, 0)]");
        CREDO_FALSUM (laqueus_motus_removere(triangulum, ZEPHYRUM,
            piscina,
            &l));
        /* catena Hopf: removere verticem (10,10,10) [componens 2, index
         * 5] verreret per segmentum componentis primae? triangulum
         * (10,10,-10)-(10,10,10)-(30,10,10) planum y = 10 secat
         * quadratum in segmento (0..20, 10, 0): transfigit ->
         * illegitimus */
        CREDO_FALSUM (laqueus_motus_removere(hopf, V, piscina, &l));

        /* motus fortuiti in trifolio, adversarii (offsets +-400 trans
         * polygonum ~+-300: multi triangula per alia segmenta
         * traherent) et lenes (+-60): legitimi invariantes servant -
         * Alexander semper, Jones si transitus <= maximi (motus
         * adversarii diagrammata ~60 transituum faciunt; uncinus tunc
         * refutat) */
        {
            s32 ambitus[II] = { CD, LX };
            i32 r_;

            per (r_ = ZEPHYRUM; r_ < II; r_++)
            {
                 Diagramma d;
                Polynomium a_post;
                Polynomium j_post;

                l = _ex_tabula(trifolium, XLVIII);
                CREDO_VERUM (_invariantes(l, &jones, &alexander, &w));
                facti     = ZEPHYRUM;
                refutati  = ZEPHYRUM;
                sors_seminare(&s, 2026ULL, (i64)(r_ + I));
                per (k = ZEPHYRUM; k < CC; k++)
                {
                    Laqueus nova;
                        i32 i = (i32)sors_intra(&s,
                            laqueus_numerus(l));

                    si (sors_intra(&s, II) == ZEPHYRUM)
                    {
                        Punctum a = laqueus_vertex(l, i);
                        Punctum b = laqueus_vertex(l, (i + I)
                            % laqueus_numerus(l));
                        Punctum c;
                        Fractio duo = fractio_ex_s64(II);

                        c.x = fractio_adde(fractio_adde(a.x, b.x,
                            piscina),
                            fractio_ex_s64(sors_inter(&s, -ambitus[r_],
                            ambitus[r_])), piscina);
                        c.y = fractio_adde(fractio_adde(a.y, b.y,
                            piscina),
                            fractio_ex_s64(sors_inter(&s, -ambitus[r_],
                            ambitus[r_])), piscina);
                        c.z = fractio_adde(fractio_adde(a.z, b.z,
                            piscina),
                            fractio_ex_s64(sors_inter(&s, -ambitus[r_],
                            ambitus[r_])), piscina);
                        (vacuum)fractio_divide(c.x, duo, piscina, &c.x);
                        (vacuum)fractio_divide(c.y, duo, piscina, &c.y);
                        (vacuum)fractio_divide(c.z, duo, piscina, &c.z);
                        si (laqueus_motus_addere(l, i, c, piscina,
                            &nova))
                        {
                            l = nova;
                            facti++;
                        }
                        alioquin
                        {
                            refutati++;
                        }
                    }
                    alioquin si (laqueus_numerus(l) > IV)
                    {
                        si (laqueus_motus_removere(l, i, piscina,
                            &nova))
                        {
                            l = nova;
                            facti++;
                        }
                        alioquin
                        {
                            refutati++;
                        }
                    }
                }
                imprimere("  motus (ambitus %d): facti %u, "
                    "refutati %u, vertices %u\n", ambitus[r_], facti,
                    refutati,
                    laqueus_numerus(l));
                CREDO_MAIOR_I32 (facti, XX);
                CREDO_MAIOR_I32 (refutati, ZEPHYRUM);
                CREDO_VERUM (laqueus_simplex(l, piscina));
                CREDO_VERUM (laqueus_diagramma_genericum(l, piscina,
                    &d));
                CREDO_VERUM (diagramma_alexander(d, piscina, &a_post));
                CREDO_VERUM (chorda_aequalis(polynomium_ad_chordam(
                    a_post, 't', piscina), alexander));
                si (diagramma_numerus(d) <= LAQUEUS_TRANSITUS_MAXIMI)
                {
                    CREDO_VERUM (diagramma_jones(d, piscina, &j_post));
                    CREDO_VERUM (chorda_aequalis(polynomium_ad_chordam(
                        j_post, 't', piscina), jones));
                }
                alioquin
                {
                    CREDO_FALSUM (diagramma_jones(d, piscina, &j_post));
                }
            }
        }
    }


    /* ==================================================
     * SIMPLIFICATIO, DIAGRAMMA MINIMUM, SPECULUM, DETERMINANS
     * ================================================== */

    {
        nomen structura {
            constans character* titulus;
            constans s32      (*tabula)[III];
                           i32 numerus;
                           i32 transitus;
                           s32 determinans;
                           i32 baculi;
                           b32 minimus_attingitur;
                           i32 simplificati;
        } Exemplar;
        /* tabulae: numerus transituum (crossing number), determinans,
         * numerus baculorum (stick number: 3_1 6, 4_1 7, 5_1 8, 7_1 9,
         * 8_19 = T(3,4) 8) - limes inferior quem simplificatio
         * transgredi nequit; diagramma minimum ambitu II numerum
         * transituum attingit praeter T(2,7) (VIII, non VII) */
        Exemplar exempla[] = {
            { "trifolium", trifolium, XLVIII, III, III, VI, VERUM,
                VII },
            { "octonaria", octonaria, LXXII, IV, V, VII, VERUM, VIII },
            { "quinquefolium", quinquefolium, LXXX, V, V, VIII, VERUM,
              VIII },
            { "septifolium", septifolium, CXII, VII, VII, IX, FALSUM,
                XI },
            { "torus (3, 4)", torus_iii_iv, CXX, VIII, III, VIII, VERUM,
              VIII }
        };
        i32 k;

        imprimere("\n--- Probans simplificationem, diagramma minimum, "
            "speculum, determinans ---\n");
        per (k = ZEPHYRUM; k < V; k++)
        {
            Laqueus l = _ex_tabula(exempla[k].tabula,
                exempla[k].numerus);
                   Laqueus s;
                   Laqueus s2;
                   Laqueus sp;
                 Diagramma dg;
                 Diagramma dm;
                 Diagramma ds;
                Polynomium a;
                Polynomium a2;
                Polynomium j;
                Polynomium j2;
                    Magnus det;
            memoriae_index ante;
            memoriae_index usus;
                       b32 jones;
                       b32 minimus;
                       i32 i;
                       i32 legitimi = ZEPHYRUM;

            CREDO_VERUM (laqueus_diagramma_genericum(l, piscina, &dg));
            CREDO_VERUM (diagramma_alexander(dg, piscina, &a));
            jones = diagramma_jones(dg, piscina, &j);
            /* determinans = |Delta(-1)| tabulae */
            CREDO_VERUM (diagramma_determinans(dg, piscina, &det));
            CREDO_VERUM (magnus_aequalis(det,
                magnus_ex_s64(exempla[k].determinans)));
            /* diagramma minimum: non plus quam genericum, non minus
             * quam numerus transituum, idem nodus */
            minimus = laqueus_diagramma_minimum(l, II, piscina, &dm);
            CREDO_VERUM (minimus);
            CREDO_VERUM (diagramma_numerus(dm)
                <= diagramma_numerus(dg));
            CREDO_VERUM (diagramma_numerus(dm) >= exempla[k].transitus);
            si (exempla[k].minimus_attingitur)
            {
                CREDO_AEQUALIS_I32 (diagramma_numerus(dm),
                    exempla[k].transitus);
            }
            /* aequalia -> primum ordine enumerationis: trifolio (0, 1,
             * -2) (recensio) */
            si (k == ZEPHYRUM)
            {
                CREDO_VERUM (situs_puncta_aequalia(dm.directio,
                    situs_punctum(ZEPHYRUM, I, -II)));
            }
            CREDO_VERUM (diagramma_alexander(dm, piscina, &a2)
                && polynomium_aequalis(a, a2));
            /* simplificatio: simplex, idem nodus, localiter minima,
             * idempotens */
            ante = piscina_summa_usus(piscina);
            CREDO_VERUM (laqueus_simplificare(l, piscina, &s));
            usus = piscina_summa_usus(piscina) - ante;
            CREDO_VERUM (laqueus_simplex(s, piscina));
            CREDO_VERUM (laqueus_numerus(s) < laqueus_numerus(l));
            CREDO_VERUM (laqueus_numerus(s) >= exempla[k].baculi);
            /* numeri exacti (oraculum recensoris idem): avaritia
             * "index primus" fixa */
            CREDO_AEQUALIS_I32 (laqueus_numerus(s),
                exempla[k].simplificati);
            /* memoria O(n): tabula laboris una, conatus refecti (copia
             * per motum ~ n^2 octetos posceret) */
            CREDO_VERUM (usus < (memoriae_index)CCLVI
                * (memoriae_index)laqueus_numerus(l));
            CREDO_VERUM (laqueus_diagramma_genericum(s, piscina, &ds));
            CREDO_VERUM (diagramma_alexander(ds, piscina, &a2)
                && polynomium_aequalis(a, a2));
            si (jones)
            {
                CREDO_VERUM (diagramma_jones(ds, piscina, &j2)
                    && polynomium_aequalis(j, j2));
            }
            per (i = ZEPHYRUM; i < laqueus_numerus(s); i++)
            {
                Laqueus r;

                si (laqueus_motus_removere(s, i, piscina, &r))
                {
                    legitimi++;
                }
            }
            CREDO_AEQUALIS_I32 (legitimi, ZEPHYRUM);
            CREDO_VERUM (laqueus_simplificare(s, piscina, &s2));
            CREDO_VERUM (chorda_aequalis(laqueus_ad_chordam(s, piscina),
                laqueus_ad_chordam(s2, piscina)));
            /* speculum: Alexander idem, Jones(1/t) */
            CREDO_VERUM (laqueus_speculum(l, piscina, &sp));
            CREDO_VERUM (laqueus_diagramma_genericum(sp, piscina, &ds));
            CREDO_VERUM (diagramma_alexander(ds, piscina, &a2)
                && polynomium_aequalis(a, a2));
            si (jones)
            {
                CREDO_VERUM (diagramma_jones(ds, piscina, &j2)
                    && polynomium_aequalis(j2, polynomium_inversum(j,
                    piscina)));
            }
            imprimere("  %s: vertices %u -> %u, transitus genericus "
                "%u, minimus %u, usus simplificationis %lu\n",
                exempla[k].titulus, laqueus_numerus(l),
                laqueus_numerus(s), diagramma_numerus(dg),
                diagramma_numerus(dm), (unsigned long)usus);
        }
    }
    {
        Laqueus hopf =
            _ex_textu("[(0, 0, 0), (20, 0, 0), (20, 20, 0), "
            "(0, 20, 0); (10, 10, -10), (10, 10, 10), (30, 10, 10), "
            "(30, 10, -10)]");
          Laqueus s;
          Laqueus sp;
        Diagramma d;
        Diagramma ds;
           Magnus det;

        CREDO_VERUM (laqueus_diagramma_genericum(hopf, piscina, &d));
        CREDO_FALSUM (diagramma_determinans(d, piscina, &det));
        CREDO_FALSUM (laqueus_diagramma_minimum(hopf, ZEPHYRUM, piscina,
            &ds));
        /* radius "negativus" (i32 sine signo, ~4.3e9) refutatur, non
         * pendet: sub custodia, ut limes amissus FRACTA det, non
         * suitam suspendat */
        CREDO_NON_PENDET(CREDO_FALSUM (laqueus_diagramma_minimum(hopf,
            (i32)-I, piscina, &ds)), MM);
        /* catena quae vertices amittit: Hopf cum puncto medio in omni
         * latere (XVI vertices). Puncta media semper removentur, deinde
         * anguli quoque (triangula dimidia componentem alteram non iam
         * tangunt): exitus duo triangula (III + III, minimum catenae),
         * simplex, numerus ligationis +1 servatus - limites
         * componentium corrupti hic frangerent (recensio, L1) */
        CREDO_VERUM (laqueus_simplificare(_ex_textu(
            "[(0, 0, 0), (10, 0, 0), (20, 0, 0), (20, 10, 0), "
            "(20, 20, 0), (10, 20, 0), (0, 20, 0), (0, 10, 0); "
            "(10, 10, -10), (10, 10, 0), (10, 10, 10), (20, 10, 10), "
            "(30, 10, 10), (30, 10, 0), (30, 10, -10), (20, 10, -10)]"),
            piscina, &s));
        CREDO_VERUM (laqueus_simplex(s, piscina));
        CREDO_AEQUALIS_I32 (laqueus_componentes(s), II);
        CREDO_AEQUALIS_I32 (s.initia[I], III);
        CREDO_AEQUALIS_I32 (s.initia[II], VI);
        CREDO_VERUM (laqueus_diagramma_genericum(s, piscina, &ds));
        CREDO_AEQUALIS_S32 (diagramma_numerus_ligationis(ds, ZEPHYRUM,
            I),
            I);
        CREDO_VERUM (laqueus_simplificare(hopf, piscina, &s));
        CREDO_AEQUALIS_I32 (laqueus_componentes(s), II);
        CREDO_VERUM (laqueus_diagramma_genericum(s, piscina, &ds));
        CREDO_AEQUALIS_S32 (diagramma_numerus_ligationis(ds, ZEPHYRUM,
            I),
            I);
        CREDO_VERUM (laqueus_speculum(hopf, piscina, &sp));
        CREDO_VERUM (laqueus_diagramma_genericum(sp, piscina, &ds));
        CREDO_AEQUALIS_S32 (diagramma_numerus_ligationis(ds, ZEPHYRUM,
            I),
            -I);
        /* non simplex: refutatur */
        CREDO_FALSUM (laqueus_simplificare(_ex_textu(
            "[(0, 0, 0), (10, 10, 0), (10, 0, 0), (0, 10, 0)]"),
            piscina,
            &s));
        imprimere("  catena Hopf simplificata: vertices %u\n",
            laqueus_numerus(s));
    }

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
