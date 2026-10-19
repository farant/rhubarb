/* chorda_credo_schedae.c - sonda recensoris: schedae credo quae HODIE
 * frangi debent (verificatio quod drafts compilantur et rubent) */
#include "postulata_posix.h"
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

interior vacuum
_probare_fissio_delim_longior (
    Piscina* piscina)
{
    chorda_fissio_fructus  fr;

    fr = chorda_fissio_chorda(chorda_ex_literis("a", piscina),
                              chorda_ex_literis("abc", piscina),
                              piscina);
    si (   fr.numerus != I
        || !chorda_aequalis_literis(fr.elementa[ZEPHYRUM], "a"))
    {
        abort();
    }
}

interior vacuum
_probare_duplicare_superfluitas (
    Piscina* piscina)
{
    chorda  r;

    /* II * 0x80000001 = 0x100000002 -> i32 involvitur ad II */
    r = chorda_duplicare(chorda_ex_literis("ab", piscina),
                         0x80000001U, piscina);
    si (r.mensura != ZEPHYRUM || r.datum != NIHIL)
    {
        abort();
    }
}

interior vacuum
_probare_substituere_superfluitas (
    Piscina* piscina)
{
    chorda  s;
    chorda  antiquum;
    chorda  novum;
    chorda  r;
        i8* alveus_s;
        i8* alveus_n;

    alveus_s = (i8*)piscina_allocare(piscina, 0x10000U);
    alveus_n = (i8*)piscina_allocare(piscina, 0x10001U);
    si (!alveus_s || !alveus_n)
    {
        abort();
    }
    memset(alveus_s, 'a', 0x10000U);
    memset(alveus_n, 'b', 0x10001U);

    s        = chorda_ex_buffer(alveus_s, 0x10000U);
    antiquum = chorda_ex_buffer(alveus_s, I);
    novum    = chorda_ex_buffer(alveus_n, 0x10001U);

    /* 0x10000 + 0x10000 * 0x10000 = 2^32 + 0x10000 -> 0x10000 */
    r = chorda_substituere(s, antiquum, novum, piscina);
    si (r.mensura != ZEPHYRUM || r.datum != NIHIL)
    {
        abort();
    }
}

interior vacuum
_probare_concatenare_superfluitas (
    Piscina* piscina)
{
    vacuum* regio;
    chorda  a;
    chorda  b;
    chorda  r;

    /* regio lecta tantum, pigra: nulla pagina tangitur nisi legitur */
    regio = mmap(NIHIL, (memoriae_index)0x80000010UL, PROT_READ,
                 MAP_PRIVATE | MAP_ANON, -I, ZEPHYRUM);
    si (regio == MAP_FAILED)
    {
        abort();
    }
    a = chorda_ex_buffer((i8*)regio, 0x80000000U);
    b = chorda_ex_buffer((i8*)regio, 0x80000010U);

    /* summa 2^32 + XVI -> i32 involvitur ad XVI */
    r = chorda_concatenare(a, b, piscina);
    si (r.mensura != ZEPHYRUM || r.datum != NIHIL)
    {
        abort();
    }
}

s32 principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("sonda_schedae", 65536);
    credo_aperire(piscina);

    {
        imprimere("\n--- Probans chorda_fissio_chorda (delim longior) ---\n");
        CREDO_NON_RUIT(_probare_fissio_delim_longior(piscina));
    }

    {
        imprimere("\n--- Probans chorda_duplicare (superfluitas) ---\n");
        CREDO_NON_RUIT(_probare_duplicare_superfluitas(piscina));
    }

    {
        imprimere("\n--- Probans chorda_substituere (superfluitas) ---\n");
        CREDO_NON_RUIT(_probare_substituere_superfluitas(piscina));
    }

    {
        imprimere("\n--- Probans chorda_concatenare (superfluitas) ---\n");
        CREDO_NON_RUIT(_probare_concatenare_superfluitas(piscina));
    }

    {
        i8 octeti[V];
        chorda s;
        chorda_fissio_fructus fr;

        imprimere("\n--- Probans chorda_fissio (delimitator >= 0x80) ---\n");

        octeti[ZEPHYRUM] = 'a';
        octeti[I]        = (i8)0xFF;
        octeti[II]       = 'b';
        octeti[III]      = (i8)0xFF;
        octeti[IV]       = 'c';
        s  = chorda_ex_buffer(octeti, V);
        fr = chorda_fissio(s, (character)0xFF, piscina);
        CREDO_AEQUALIS_I32(fr.numerus, III);
        si (fr.numerus == III)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(fr.elementa[ZEPHYRUM], "a");
            CREDO_CHORDA_AEQUALIS_LITERIS(fr.elementa[I], "b");
            CREDO_CHORDA_AEQUALIS_LITERIS(fr.elementa[II], "c");
        }

        octeti[I]   = (i8)0xC3;
        octeti[III] = (i8)0xC3;
        fr = chorda_fissio(s, (character)0xC3, piscina);
        CREDO_AEQUALIS_I32(fr.numerus, III);
    }

    {
        i32 v;

        imprimere("\n--- Probans chorda_ut_i32 (signum post spatium) ---\n");

        v = DCCLXXVII;
        CREDO_FALSUM(chorda_ut_i32(
            chorda_ex_literis(" -18446744073709551615", piscina), &v));
        CREDO_FALSUM(chorda_ut_i32(
            chorda_ex_literis("\t-18446744069414584321", piscina), &v));
        CREDO_FALSUM(chorda_ut_i32(chorda_ex_literis(" -0", piscina), &v));
        CREDO_AEQUALIS_I32(v, DCCLXXVII);
        /* frater sine spatio iam recusatur */
        CREDO_FALSUM(chorda_ut_i32(chorda_ex_literis("-0", piscina), &v));
    }

    {
        imprimere("\n--- Probans chorda_ex_bytes_legibilis (unitates binariae) ---\n");

        CREDO_CHORDA_AEQUALIS_LITERIS(
            chorda_ex_bytes_legibilis((i64)1073741824UL, piscina), "1.0 GB");
        CREDO_CHORDA_AEQUALIS_LITERIS(
            chorda_ex_bytes_legibilis((i64)2147483648UL, piscina), "2.0 GB");
        CREDO_CHORDA_AEQUALIS_LITERIS(
            chorda_ex_bytes_legibilis((i64)1024, piscina), "1.0 KB");
        CREDO_CHORDA_AEQUALIS_LITERIS(
            chorda_ex_bytes_legibilis((i64)1048576UL, piscina), "1.0 MB");
    }

    {
        chorda fenum;
        chorda acus;

        imprimere("\n--- Probans quaestio cum acu vacua {0, NIHIL} ---\n");

        fenum = chorda_ex_literis("abc", piscina);
        acus  = chorda_ex_literis("", piscina);

        CREDO_VERUM(chorda_vacua(acus));
        CREDO_VERUM(chorda_continet(fenum, acus));
        CREDO_VERUM(chorda_incipit(fenum, acus));
        CREDO_VERUM(chorda_terminatur(fenum, acus));
        CREDO_AEQUALIS_S32(chorda_invenire_index(fenum, acus), ZEPHYRUM);
        CREDO_AEQUALIS_I32(chorda_invenire(fenum, acus).mensura, III);
        CREDO_AEQUALIS_PTR(chorda_invenire(fenum, acus).datum, fenum.datum);
    }

    {
        chorda fenum;
        chorda acus;
        chorda ultimum;
           s32 positio;

        imprimere("\n--- Probans chorda_invenire_ultimum contra _index (acus vacua) ---\n");

        fenum   = chorda_ex_literis("abc", piscina);
        acus    = chorda_praecidere(chorda_ex_literis("   ", piscina));
        ultimum = chorda_invenire_ultimum(fenum, acus);
        positio = chorda_invenire_ultimum_index(fenum, acus);

        CREDO_MAIOR_AUT_AEQUALIS_S32(positio, ZEPHYRUM);
        CREDO_AEQUALIS_PTR(ultimum.datum, fenum.datum + positio);
    }

    {
        chorda s;

        imprimere("\n--- Probans conversio casus cum UTF-8 ---\n");

        s = chorda_ex_literis("caf\xc3\xa9 au lait", piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_serpens(s, piscina),
                                      "caf\xc3\xa9_au_lait");
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_kebab(s, piscina),
                                      "caf\xc3\xa9-au-lait");
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_pascalis(s, piscina),
                                      "Caf\xc3\xa9" "AuLait");
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
