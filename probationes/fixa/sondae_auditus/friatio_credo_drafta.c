#include "postulata_posix.h"
#include "latina.h"
#include "friatio.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

/* Digestum cum hexadecimali comparare (XX vel XXXII octeti) */
interior b32
_digestum_aequat (
    constans i8*        digestum,
    constans character* hexa,
                   i32  numerus)
{
    constans character* signa = "0123456789abcdef";
    i32 idx;

    per (idx = ZEPHYRUM; idx < numerus; idx++)
    {
        si (hexa[idx * II]      != signa[(digestum[idx] >> IV) & 0xF] ||
            hexa[idx * II + I]  != signa[digestum[idx] & 0xF])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Chorda vacua {0, NIHIL}: digestum rectum SINE UB (rubet sub
 * './tools/tegmen_probationum.sh -sanitare friatio': UBSan
 * 'applying zero offset to null pointer' friatio.c:258 et :481) */
interior vacuum
_sectio_nihil_vacua (
    CredoContextus* c)
{
    chorda          vacua;
    i8              d1[SHA1_DIGEST_MENSURA];
    i8              d2[SHA256_DIGEST_MENSURA];
    SHA1Contextus   c1;
    SHA256Contextus c2;

    (vacuum)c;
    vacua.datum   = NIHIL;
    vacua.mensura = ZEPHYRUM;

    sha1_friare_chorda(vacua, d1);
    CREDO_VERUM(_digestum_aequat(d1,
        "da39a3ee5e6b4b0d3255bfef95601890afd80709", SHA1_DIGEST_MENSURA));

    sha256_friare_chorda(vacua, d2);
    CREDO_VERUM(_digestum_aequat(d2,
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
        SHA256_DIGEST_MENSURA));

    sha1_initiare(&c1);
    sha1_addere(&c1, (constans i8*)"ab", II);
    sha1_addere(&c1, NIHIL, ZEPHYRUM);
    sha1_addere(&c1, (constans i8*)"c", I);
    sha1_finire(&c1, d1);
    CREDO_VERUM(_digestum_aequat(d1,
        "a9993e364706816aba3e25717850c26c9cd0d89d", SHA1_DIGEST_MENSURA));

    sha256_initiare(&c2);
    sha256_addere(&c2, NIHIL, ZEPHYRUM);
    sha256_addere(&c2, (constans i8*)"abc", III);
    sha256_finire(&c2, d2);
    CREDO_VERUM(_digestum_aequat(d2,
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
        SHA256_DIGEST_MENSURA));
}

/* j = 1 in buffer, deinde mensura 0xFFFFFFFF: (j + mensura) involvitur
 * ad 0 et memcpy(&buffer[1], data, 0xFFFFFFFF) contextum transscribit.
 * Oraculum: hashlib super 'a' + 0xFFFFFFFF octetos nullos. Filius abortat
 * si digestum falsum; hodie SIGSEGV/ASan. */
interior vacuum
_involutio_sha1 (vacuum)
{
    SHA1Contextus* ctx;
    i8             d[SHA1_DIGEST_MENSURA];
    vacuum*        p;

    p = mmap(NIHIL, (memoriae_index)0xFFFFFFFFUL, PROT_READ,
             MAP_ANON | MAP_PRIVATE, -I, ZEPHYRUM);
    ctx = (SHA1Contextus*)malloc(magnitudo(SHA1Contextus));
    si (p == MAP_FAILED || !ctx) { abort(); }
    sha1_initiare(ctx);
    sha1_addere(ctx, (constans i8*)"a", I);
    sha1_addere(ctx, (constans i8*)p, (i32)0xFFFFFFFFUL);
    sha1_finire(ctx, d);
    si (!_digestum_aequat(d, "8c44315959f83c84a2b5501541eee2b3a666e416",
                          SHA1_DIGEST_MENSURA)) { abort(); }
}

interior vacuum
_involutio_sha256 (vacuum)
{
    SHA256Contextus* ctx;
    i8               d[SHA256_DIGEST_MENSURA];
    vacuum*          p;

    p = mmap(NIHIL, (memoriae_index)0xFFFFFFFFUL, PROT_READ,
             MAP_ANON | MAP_PRIVATE, -I, ZEPHYRUM);
    ctx = (SHA256Contextus*)malloc(magnitudo(SHA256Contextus));
    si (p == MAP_FAILED || !ctx) { abort(); }
    sha256_initiare(ctx);
    sha256_addere(ctx, (constans i8*)"a", I);
    sha256_addere(ctx, (constans i8*)p, (i32)0xFFFFFFFFUL);
    sha256_finire(ctx, d);
    si (!_digestum_aequat(d,
        "fbe36e2ec49cb557271b2641f959253c25862df8c9745d2173fed6d53fdec229",
        SHA256_DIGEST_MENSURA)) { abort(); }
}

interior vacuum
_sectio_involutio (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_NON_PENDET(_involutio_sha1(), LX * M);
    CREDO_NON_PENDET(_involutio_sha256(), LX * M);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "sha nihil vacua", _sectio_nihil_vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sha involutio mensurae", _sectio_involutio, NIHIL, NIHIL, "lentus",
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("friatio_drafta", SECTIONES);
}
