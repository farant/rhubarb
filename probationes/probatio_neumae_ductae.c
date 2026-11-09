/* probatio_neumae_ductae.c - partes neumarum quadratarum ut larvae
 * (gabc pars II, P2)
 *
 * Larvae exspectatae ut picturae: '.' vacuum, '#' plenum, '1' '2' '3'
 * umbrae (0x40 0x80 0xC0, ut probatio_glyphae_ductae.c). Partes ex
 * rectangulis (punctum, cavum, virga, clavis C ...) manu derivatae ex
 * regulis capitis; partes cum marginibus obliquis aut curvis
 * (inclinatum, porrectus, oriscus, bemolle ...) pinnatae oculis post
 * picturam primam (glyphae_ductae II b idem fecit). Leges per omnem
 * spatium: nihil extra larvam, quaevis pars pingit, caulis virgae
 * longitudinem parametri habet, capita altitudinem communem. */
#include "latina.h"
#include "credo.h"
#include "neumae_ductae.h"
#include <stdio.h>
#include <string.h>

#define CUSTOS_OCTETI  0xA5
#define MARGO_CUSTODIS XVI

interior i8
valor_signi (
    character s)
{
    commutatio (s)
    {
        casus '#': redde (i8)0xFF;
        casus '1': redde (i8)0x40;
        casus '2': redde (i8)0x80;
        casus '3': redde (i8)0xC0;
        ordinarius: redde ZEPHYRUM;
    }
}

interior character
signum_valoris (
    i8 v)
{
    commutatio (v)
    {
        casus 0x00: redde '.';
        casus 0xFF: redde '#';
        casus 0x40: redde '1';
        casus 0x80: redde '2';
        casus 0xC0: redde '3';
        ordinarius: redde '?';
    }
}

/* parametrum quod omnis pars accipit (leges, mensurae) */
interior s32
parametrum_ordinarium (
    NeumaePars pars)
{
    commutatio (pars)
    {
        casus NEUMAE_VIRGA:
        casus NEUMAE_VIRGA_REVERSA:
            redde II;
        casus NEUMAE_PORRECTUS:
            redde I;
        casus NEUMAE_CUSTOS:
            redde I;
        casus NEUMAE_EPISEMA:
            redde XII;
        ordinarius:
            redde ZEPHYRUM;
    }
}

/* larva partis == pictura (lineae numerus, latitudo strlen); larva
 * recepta imprimitur si discrepat */
interior b32
pictura_est (
                NeumaePars   pars,
                       i32   spatium,
                       s32   parametrum,
      constans character**   pictura,
                       i32   numerus_linearum,
                        i8*  larva)
{
    NeumaeMensura m;
              i32 x;
              i32 y;
              b32 bonum;

    si (!neumae_ductae_metiri(pars, spatium, parametrum, &m))
    {
        imprimere("  %s/%u: non metitur\n", neumae_partis_titulus(pars),
            (unsigned)spatium);
        redde FALSUM;
    }
    memset(larva, CUSTOS_OCTETI, (size_t)(NEUMAE_LARVA_MAXIMA
        + MARGO_CUSTODIS));
    si (!neumae_ductae_pingere(pars, spatium, parametrum, m.latitudo,
        m.altitudo, larva))
    {
        imprimere("  %s/%u: non pingitur\n",
            neumae_partis_titulus(pars),
            (unsigned)spatium);
        redde FALSUM;
    }
    bonum = m.altitudo == numerus_linearum
         && m.latitudo == (i32)strlen(pictura[ZEPHYRUM]);
    si (bonum)
    {
        per (y = ZEPHYRUM; y < m.altitudo; y++)
        {
            per (x = ZEPHYRUM; x < m.latitudo; x++)
            {
                si (larva[y * m.latitudo + x]
                    != valor_signi(pictura[y][x]))
                {
                    bonum = FALSUM;
                }
            }
        }
    }
    si (!bonum)
    {
        imprimere("  %s/%u/%d: exspectatum %u x %u | receptum "
            "%u x %u\n",
            neumae_partis_titulus(pars), (unsigned)spatium,
            (integer)parametrum, (unsigned)strlen(pictura[ZEPHYRUM]),
            (unsigned)numerus_linearum, (unsigned)m.latitudo,
            (unsigned)m.altitudo);
        per (y = ZEPHYRUM; y < m.altitudo; y++)
        {
            imprimere("    \"");
            per (x = ZEPHYRUM; x < m.latitudo; x++)
            {
                imprimere("%c", signum_valoris(larva[y * m.latitudo
                    + x]));
            }
            imprimere("\",\n");
        }
    }
    redde bonum;
}

#define PICTURA(pars, spatium, par, pic) \
    CREDO_VERUM(pictura_est((pars), (spatium), (par), (pic), \
        (i32)(magnitudo(pic) / magnitudo((pic)[ZEPHYRUM])), larva))


/* ==================================================
 * I: picturae manu derivatae (rectangula)
 * ================================================== */

interior vacuum
_quadrata_probare (
    CredoContextus* c)
{
    i8 larva[NEUMAE_LARVA_MAXIMA + MARGO_CUSTODIS];

    /* caput: spatium latum, spatium - I altum, centrum ordine III */
    constans character* punctum[] = {
        "########", "########", "########", "########",
        "########", "########", "########" };
    constans character* cavum[] = {
        "########", "#......#", "#......#", "#......#",
        "#......#", "#......#", "########" };
    /* caulis a centro (ordo III) II gradibus = VIII ordines infra */
    constans character* virga[] = {
        "########", "########", "########", "########",
        "########", "########", "########", ".......#",
        ".......#", ".......#", ".......#", ".......#" };
    constans character* virga_reversa[] = {
        "########", "########", "########", "########",
        "########", "########", "########", "#.......",
        "#.......", "#.......", "#.......", "#......." };
    constans character* deminutum[] = {
        "#####", "#####", "#####", "#####", "#####" };
    constans character* linea_punctum[] = {
        "#.########.#", "#.#......#.#", "#.#......#.#", "#.#......#.#",
        "#.#......#.#", "#.#......#.#", "#.########.#" };
    constans character* auctum_ascendens[] = {
        ".......#", ".......#", "########", "########", "########",
        "########", "########", "########", "########" };
    constans character* auctum_descendens[] = {
        "########", "########", "########", "########", "########",
        "########", "########", ".......#", ".......#" };
    constans character* linea[] = {
        "################", "################", "################",
        "################", "################" };
    /* naturale: venter in ordine VI */
    constans character* naturale[] = {
        "#....", "#....", "#....", "#....", "#####", "#####", "#...#",
        "#####", "#####", "....#", "....#", "....#", "....#" };
    constans character* diesis[] = {
        ".#..#.", ".#..#.", "######", "######", ".#..#.", "######",
        "######", ".#..#.", ".#..#." };
    /* clavis C: linea clavis ordine VII, intervallum I supra et
     * infra */
    constans character* clavis_c[] = {
        "######", "######", "######", "######", "######", "######",
        "#.....", "#.....", "#.....", "######", "######", "######",
        "######", "######", "######" };
    constans character* custos_sursum[] = {
        "...#", "...#", "...#", "...#", "...#", "...#", "####", "####",
        "####", "####", "####" };
    constans character* custos_deorsum[] = {
        "####", "####", "####", "####", "####", "...#", "...#", "...#",
        "...#", "...#", "...#" };
    constans character* punctum_morae[]  = { "##", "##" };
    constans character* ictus[]          = { "#", "#", "#", "#" };
    constans character* episema[]        = { "############" };
    /* spatium XII: lex magnitudinis */
    constans character* punctum_xii[] = {
        "############", "############", "############", "############",
        "############", "############", "############", "############",
        "############", "############", "############" };
    constans character* virga_xii[] = {
        "############", "############", "############", "############",
        "############", "############", "############", "############",
        "############", "############", "############", "...........#",
        "...........#", "...........#", "...........#", "...........#",
        "...........#", "...........#" };
    constans character* clavis_c_xii[] = {
        "#########", "#########", "#########", "#########", "#########",
        "#########", "#########", "#########", "#########", "#........",
        "#........", "#........", "#########", "#########", "#########",
        "#########", "#########", "#########", "#########", "#########",
        "#########" };

    (vacuum)c;
    PICTURA(NEUMAE_PUNCTUM, VIII, ZEPHYRUM, punctum);
    PICTURA(NEUMAE_CAVUM, VIII, ZEPHYRUM, cavum);
    PICTURA(NEUMAE_VIRGA, VIII, II, virga);
    PICTURA(NEUMAE_VIRGA_REVERSA, VIII, II, virga_reversa);
    PICTURA(NEUMAE_DEMINUTUM, VIII, ZEPHYRUM, deminutum);
    PICTURA(NEUMAE_LINEA_PUNCTUM, VIII, ZEPHYRUM, linea_punctum);
    PICTURA(NEUMAE_AUCTUM_ASCENDENS, VIII, ZEPHYRUM, auctum_ascendens);
    PICTURA(NEUMAE_AUCTUM_DESCENDENS, VIII, ZEPHYRUM,
        auctum_descendens);
    PICTURA(NEUMAE_LINEA, VIII, ZEPHYRUM, linea);
    PICTURA(NEUMAE_NATURALE, VIII, ZEPHYRUM, naturale);
    PICTURA(NEUMAE_DIESIS, VIII, ZEPHYRUM, diesis);
    PICTURA(NEUMAE_CLAVIS_C, VIII, ZEPHYRUM, clavis_c);
    PICTURA(NEUMAE_CUSTOS, VIII, I, custos_sursum);
    PICTURA(NEUMAE_CUSTOS, VIII, -I, custos_deorsum);
    PICTURA(NEUMAE_PUNCTUM_MORAE, VIII, ZEPHYRUM, punctum_morae);
    PICTURA(NEUMAE_ICTUS, VIII, ZEPHYRUM, ictus);
    PICTURA(NEUMAE_EPISEMA, VIII, XII, episema);
    PICTURA(NEUMAE_PUNCTUM, XII, ZEPHYRUM, punctum_xii);
    PICTURA(NEUMAE_VIRGA, XII, II, virga_xii);
    PICTURA(NEUMAE_CLAVIS_C, XII, ZEPHYRUM, clavis_c_xii);
}


/* ==================================================
 * II: picturae pinnatae oculis (obliqua, curva)
 * ================================================== */

interior vacuum
_curvas_probare (
    CredoContextus* c)
{
    i8 larva[NEUMAE_LARVA_MAXIMA + MARGO_CUSTODIS];

    constans character* inclinatum[] = {
        "...33...",
        "..3##3..",
        ".3####3.",
        "3######3",
        ".3####3.",
        "..3##3..",
        "...33..." };
    constans character* inclinatum_deminutum[] = {
        ".13..",
        "1##3.",
        "####3",
        "1##3.",
        ".13.." };
    constans character* stropha[] = {
        "####3...",
        "#####3..",
        "######3.",
        "#######3",
        "########",
        "########",
        "########" };
    constans character* oriscus[] = {
        ".11....1",
        "2##3..1#",
        "#####3##",
        "########",
        "##3#####",
        "#1..3##2",
        "1....11." };
    constans character* quilisma[] = {
        ".2.11.2.",
        "1#.33.#1",
        "3#2##2#3",
        "########",
        "########",
        "########",
        "########" };
    constans character* porrectus_i[] = {
        "#321............",
        "#####321........",
        "#########321....",
        "#############321",
        "################",
        "################",
        "################",
        "123#############",
        "....123#########",
        "........123#####",
        "............123#" };
    constans character* porrectus_iii[] = {
        "3...............",
        "#31.............",
        "###2............",
        "####3...........",
        "#####31.........",
        "#######2........",
        "########3.......",
        "2########31.....",
        ".13########2....",
        "...3########3...",
        "....2########31.",
        ".....13########2",
        ".......3########",
        "........2#######",
        ".........13#####",
        "...........3####",
        "............2###",
        ".............13#",
        "...............3" };
    constans character* bemolle[] = {
        "#.....",
        "#.....",
        "#.....",
        "#.....",
        "#111..",
        "#####.",
        "#2..#2",
        "#...#2",
        "#..2#1",
        "#.2#2.",
        "#3#3..",
        "#31..." };
    constans character* clavis_f[] = {
        "........######",
        "........######",
        "........######",
        "........######",
        "######..######",
        "######..######",
        "######..#.....",
        "######..#.....",
        "######..#.....",
        "######..######",
        ".....#..######",
        ".....#..######",
        ".....#..######",
        ".....#..######",
        ".....#..######",
        ".....#........" };
    constans character* virgula[] = {
        ".33",
        ".##",
        ".##",
        "2#3",
        "##1",
        "##." };
    constans character* accentus[] = {
        "..3#",
        ".3#3",
        "3#3.",
        "#3.." };
    constans character* circulus[] = {
        "2##2",
        "#33#",
        "#33#",
        "2##2" };
    constans character* semicirculus[] = {
        "3113",
        "#33#",
        "2##2" };
    constans character* porrectus_ii_xii[] = {
        "31......................",
        "##31....................",
        "####31..................",
        "######31................",
        "########31..............",
        "##########31............",
        "############31..........",
        "##############31........",
        "################31......",
        "##################31....",
        "####################31..",
        "13####################31",
        "..13####################",
        "....13##################",
        "......13################",
        "........13##############",
        "..........13############",
        "............13##########",
        "..............13########",
        "................13######",
        "..................13####",
        "....................13##",
        "......................13" };

    (vacuum)c;
    PICTURA(NEUMAE_INCLINATUM, VIII, ZEPHYRUM, inclinatum);
    PICTURA(NEUMAE_INCLINATUM_DEMINUTUM, VIII, ZEPHYRUM,
        inclinatum_deminutum);
    PICTURA(NEUMAE_STROPHA, VIII, ZEPHYRUM, stropha);
    PICTURA(NEUMAE_ORISCUS, VIII, ZEPHYRUM, oriscus);
    PICTURA(NEUMAE_QUILISMA, VIII, ZEPHYRUM, quilisma);
    PICTURA(NEUMAE_PORRECTUS, VIII, I, porrectus_i);
    PICTURA(NEUMAE_PORRECTUS, VIII, III, porrectus_iii);
    PICTURA(NEUMAE_BEMOLLE, VIII, ZEPHYRUM, bemolle);
    PICTURA(NEUMAE_CLAVIS_F, VIII, ZEPHYRUM, clavis_f);
    PICTURA(NEUMAE_VIRGULA, VIII, ZEPHYRUM, virgula);
    PICTURA(NEUMAE_ACCENTUS, VIII, ZEPHYRUM, accentus);
    PICTURA(NEUMAE_CIRCULUS, VIII, ZEPHYRUM, circulus);
    PICTURA(NEUMAE_SEMICIRCULUS, VIII, ZEPHYRUM, semicirculus);
    PICTURA(NEUMAE_PORRECTUS, XII, II, porrectus_ii_xii);
}


/* ==================================================
 * III: mensurae
 * ================================================== */

interior vacuum
_mensuras_probare (
    CredoContextus* c)
{
     constans i32 spatia[] = { VI, VIII, XII, XXIV };
    NeumaeMensura m;
              s32 pars;
              i32 k;
              i32 mala;

    (vacuum)c;
    mala = ZEPHYRUM;
    per (pars = ZEPHYRUM; pars < (s32)NEUMAE_NUMERUS_PARTIUM; pars++)
    {
        per (k = ZEPHYRUM; k < IV; k++)
        {
            si (   !neumae_ductae_metiri((NeumaePars)pars, spatia[k],
                       parametrum_ordinarium((NeumaePars)pars), &m)
                || m.latitudo == ZEPHYRUM || m.altitudo == ZEPHYRUM
                || m.origo_x < ZEPHYRUM || m.origo_y < ZEPHYRUM
                || m.origo_x >= (s32)m.latitudo
                || m.origo_y >= (s32)m.altitudo
                || m.capitis_latitudo > m.latitudo
                || m.capitis_latitudo == ZEPHYRUM)
            {
                imprimere("  mensura mala: %s/%u\n",
                    neumae_partis_titulus((NeumaePars)pars),
                    (unsigned)spatia[k]);
                mala++;
            }
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);

    /* punctum ad VIII: VIII x VII, origo (0, III) */
    CREDO_VERUM(neumae_ductae_metiri(NEUMAE_PUNCTUM, VIII, ZEPHYRUM,
        &m));
    CREDO_AEQUALIS_I32(m.latitudo, VIII);
    CREDO_AEQUALIS_I32(m.altitudo, VII);
    CREDO_AEQUALIS_S32(m.origo_x, ZEPHYRUM);
    CREDO_AEQUALIS_S32(m.origo_y, III);
    CREDO_AEQUALIS_I32(m.capitis_latitudo, VIII);

    /* porrectus: capitis latitudo II spatia */
    CREDO_VERUM(neumae_ductae_metiri(NEUMAE_PORRECTUS, VIII, III, &m));
    CREDO_AEQUALIS_I32(m.capitis_latitudo, XVI);
    CREDO_AEQUALIS_I32(m.latitudo, XVI);
    CREDO_AEQUALIS_I32(m.altitudo, VII + XII);

    /* tituli: distincti, ignota pro ignota */
    CREDO_LITERAE_AEQUALES(
        neumae_partis_titulus(NEUMAE_NUMERUS_PARTIUM), "ignota");
    CREDO_LITERAE_AEQUALES(
        neumae_partis_titulus(NEUMAE_CLAVIS_C), "clavis_c");
    mala = ZEPHYRUM;
    per (pars = ZEPHYRUM; pars < (s32)NEUMAE_NUMERUS_PARTIUM; pars++)
    {
        per (k = ZEPHYRUM; k < (i32)pars; k++)
        {
            si (strcmp(neumae_partis_titulus((NeumaePars)pars),
                    neumae_partis_titulus((NeumaePars)k)) == ZEPHYRUM)
            {
                mala++;
            }
        }
        si (strcmp(neumae_partis_titulus((NeumaePars)pars), "ignota")
            == ZEPHYRUM)
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
}


/* ==================================================
 * IV: leges per omnem spatium
 * ================================================== */

/* pinge; custodes post larvam intacti; numerus pixelorum tinctorum */
interior b32
pinge_custoditus (
    NeumaePars  pars,
           i32  spatium,
           s32  parametrum,
            i8* larva,
           i32* tincta,
    NeumaeMensura* m)
{
    i32 i;

    si (   !neumae_ductae_metiri(pars, spatium, parametrum, m)
        || m->latitudo * m->altitudo > NEUMAE_LARVA_MAXIMA)
    {
        redde FALSUM;
    }
    memset(larva, CUSTOS_OCTETI, (size_t)(NEUMAE_LARVA_MAXIMA
        + MARGO_CUSTODIS));
    si (!neumae_ductae_pingere(pars, spatium, parametrum, m->latitudo,
        m->altitudo, larva))
    {
        redde FALSUM;
    }
    per (i = m->latitudo * m->altitudo;
         i < m->latitudo * m->altitudo + MARGO_CUSTODIS; i++)
    {
        si (larva[i] != (i8)CUSTOS_OCTETI)
        {
            redde FALSUM;
        }
    }
    *tincta = ZEPHYRUM;
    per (i = ZEPHYRUM; i < m->latitudo * m->altitudo; i++)
    {
        si (larva[i] != ZEPHYRUM)
        {
            (*tincta)++;
        }
    }
    redde VERUM;
}

/* ordo larvae aliquid tinctum habet */
interior b32
ordo_tinctus (
               constans i8* larva,
    constans NeumaeMensura* m,
                       s32  y)
{
    s32 x;

    per (x = ZEPHYRUM; x < (s32)m->latitudo; x++)
    {
        si (larva[y * (s32)m->latitudo + x] != ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_leges_probare (
    CredoContextus* c)
{
                     i8 larva[NEUMAE_LARVA_MAXIMA + MARGO_CUSTODIS];
    constans NeumaePars capita[] = {
        NEUMAE_PUNCTUM, NEUMAE_INCLINATUM, NEUMAE_STROPHA,
        NEUMAE_ORISCUS, NEUMAE_QUILISMA, NEUMAE_CAVUM,
        NEUMAE_LINEA_PUNCTUM };
    NeumaeMensura m;
    NeumaeMensura caput;
              s32 pars;
              s32 n;
              i32 spatium;
              i32 tincta;
              i32 mala;
              i32 k;
              s32 y;

    (vacuum)c;
    mala = ZEPHYRUM;
    per (spatium = VI; spatium <= XLVIII; spatium += II)
    {
        per (pars = ZEPHYRUM; pars
            < (s32)NEUMAE_NUMERUS_PARTIUM; pars++)
        {
            si (   !pinge_custoditus((NeumaePars)pars, spatium,
                    parametrum_ordinarium((NeumaePars)pars), larva,
                    &tincta, &m)
                || tincta == ZEPHYRUM
                || !ordo_tinctus(larva, &m, m.origo_y))
            {
                imprimere("  lex fracta: %s/%u\n",
                    neumae_partis_titulus((NeumaePars)pars),
                    (unsigned)spatium);
                mala++;
            }
        }

        /* extrema parametrorum */
        si (   !pinge_custoditus(NEUMAE_VIRGA, spatium, VI, larva,
            &tincta,
                   &m)
            || !pinge_custoditus(NEUMAE_PORRECTUS, spatium, V, larva,
                   &tincta, &m)
            || !pinge_custoditus(NEUMAE_EPISEMA, spatium, CCLVI, larva,
                   &tincta, &m)
            || !pinge_custoditus(NEUMAE_CUSTOS, spatium, -I, larva,
                   &tincta, &m))
        {
            imprimere("  extremum fractum ad %u\n", (unsigned)spatium);
            mala++;
        }

        /* caulis virgae: n gradus = n * spatium/II ordines infra
         * centrum, columna ultima */
        per (n = II; n <= VI; n++)
        {
            si (!pinge_custoditus(NEUMAE_VIRGA, spatium, n, larva,
                &tincta,
                    &m))
            {
                mala++;
                perge;
            }
            k = ZEPHYRUM;
            per (y = m.origo_y + I; y < (s32)m.altitudo; y++)
            {
                si (larva[y * (s32)m.latitudo + (s32)m.latitudo - I]
                    != ZEPHYRUM)
                {
                    k++;
                }
            }
            si (k != (i32)n * (spatium / II))
            {
                imprimere("  caulis %d ad %u: %u ordines\n", (integer)n,
                    (unsigned)spatium, (unsigned)k);
                mala++;
            }
        }

        /* capita altitudinem et originem communem habent */
        neumae_ductae_metiri(NEUMAE_PUNCTUM, spatium, ZEPHYRUM, &caput);
        per (k = ZEPHYRUM; k < VII; k++)
        {
            neumae_ductae_metiri(capita[k], spatium, ZEPHYRUM, &m);
            si (   m.altitudo != caput.altitudo
                || m.origo_y  != caput.origo_y)
            {
                imprimere("  caput %s ad %u: altitudo %u\n",
                    neumae_partis_titulus(capita[k]), (unsigned)spatium,
                    (unsigned)m.altitudo);
                mala++;
            }
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
}


/* ==================================================
 * V: ambitus
 * ================================================== */

/* FALSUM et larva mensuraque intactae */
interior b32
recusat (
    NeumaePars pars,
           i32 spatium,
           s32 parametrum,
           i32 latitudo,
           i32 altitudo)
{
               i8 larva[LXIV];
    NeumaeMensura m;
              i32 i;

    memset(larva, CUSTOS_OCTETI, magnitudo(larva));
    memset(&m, CUSTOS_OCTETI, magnitudo(m));
    si (   neumae_ductae_metiri(pars, spatium, parametrum, &m)
        || neumae_ductae_pingere(pars, spatium, parametrum, latitudo,
               altitudo, larva))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < LXIV; i++)
    {
        si (   larva[i]                    != (i8)CUSTOS_OCTETI
            || ((i8*)&m)[i % magnitudo(m)] != (i8)CUSTOS_OCTETI)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_ambitum_probare (
    CredoContextus* c)
{
               i8 larva[NEUMAE_LARVA_MAXIMA + MARGO_CUSTODIS];
    NeumaeMensura m;
              i32 i;
              b32 intacta;

    (vacuum)c;
    CREDO_VERUM(recusat(NEUMAE_PUNCTUM, VII, ZEPHYRUM, VIII, VII));
    CREDO_VERUM(recusat(NEUMAE_PUNCTUM, IV, ZEPHYRUM, IV, III));
    CREDO_VERUM(recusat(NEUMAE_PUNCTUM, L, ZEPHYRUM, L, XLIX));
    CREDO_VERUM(recusat(NEUMAE_NUMERUS_PARTIUM, VIII, ZEPHYRUM, VIII,
        VII));
    CREDO_VERUM(recusat((NeumaePars)-I, VIII, ZEPHYRUM, VIII, VII));
    CREDO_VERUM(recusat(NEUMAE_PORRECTUS, VIII, ZEPHYRUM, XVI, VII));
    CREDO_VERUM(recusat(NEUMAE_PORRECTUS, VIII, VI, XVI, XXXI));
    CREDO_VERUM(recusat(NEUMAE_VIRGA, VIII, I, VIII, VIII));
    CREDO_VERUM(recusat(NEUMAE_VIRGA, VIII, VII, VIII, XXXII));
    CREDO_VERUM(recusat(NEUMAE_CUSTOS, VIII, ZEPHYRUM, IV, XI));
    CREDO_VERUM(recusat(NEUMAE_CUSTOS, VIII, II, IV, XI));
    CREDO_VERUM(recusat(NEUMAE_EPISEMA, VIII, ZEPHYRUM, I, I));
    CREDO_VERUM(recusat(NEUMAE_EPISEMA, VIII, CCLVII, CCLVII, I));
    CREDO_VERUM(recusat(NEUMAE_PUNCTUM, VIII, I, VIII, VII));

    /* latitudo aut altitudo non ut metiri: FALSUM, larva intacta */
    CREDO_VERUM(neumae_ductae_metiri(NEUMAE_PUNCTUM, VIII, ZEPHYRUM,
        &m));
    memset(larva, CUSTOS_OCTETI, (size_t)LXIV);
    CREDO_FALSUM(neumae_ductae_pingere(NEUMAE_PUNCTUM, VIII, ZEPHYRUM,
        m.latitudo + I, m.altitudo, larva));
    CREDO_FALSUM(neumae_ductae_pingere(NEUMAE_PUNCTUM, VIII, ZEPHYRUM,
        m.latitudo, m.altitudo - I, larva));
    intacta = VERUM;
    per (i = ZEPHYRUM; i < LXIV; i++)
    {
        si (larva[i] != (i8)CUSTOS_OCTETI)
        {
            intacta = FALSUM;
        }
    }
    CREDO_VERUM(intacta);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I: picturae manu derivatae (rectangula)", _quadrata_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "II: picturae pinnatae oculis (obliqua, curva)", _curvas_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "III: mensurae et tituli", _mensuras_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "IV: leges per omnem spatium", _leges_probare, NIHIL, NIHIL,
        NIHIL,
        NIHIL },
    { "V: ambitus", _ambitum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("neumae_ductae", SECTIONES);
}
