/* probatio_argon2id.c - Argon2id (hospitium v3 T5). Oraculum: textus
 * RFC 9106 IMMUTATUS (probationes/fixa/argon2/rfc9106.txt, sha256 infra
 * fixum), par. 5.3: parametri, tessera, sal, secretum, data associata,
 * digestum praehash (H0) et Tag EX TEXTU leguntur. H0 hic per blake2b
 * secundum formulam RFC re-computatur (H0 = textus probat blake2b et
 * ordinem campurum). Gradus quaevis magnitudinis = idem Tag; PHC itus
 * et reditus; tempus parametrorum productionis < 1 s (custos
 * pathologiae, non mensura). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "sigillum.h"
#include "blake2b.h"
#include "argon2id.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define VIA_RFC "probationes/fixa/argon2/rfc9106.txt"
#define SIGILLUM_RFC \
    "855c06f060379e34285e83a217e9069b5c72e161a1e54df9af5cd88dbb231f31"

interior s32
_post (
                chorda  t,
    constans character* quaesitum)
{
    i32 n = (i32)strlen(quaesitum);
    i32 i;

    per (i = ZEPHYRUM; i + n <= t.mensura; i++)
    {
        si (memcmp(t.datum + i, quaesitum, (size_t)n) == ZEPHYRUM)
        {
            redde (s32)(i + n);
        }
    }
    redde -I;
}

interior s32
_hex (
    i8 c)
{
    si (c >= '0' && c <= '9')
    {
        redde (s32)(c - '0');
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (s32)(c - 'a') + X;
    }
    redde -I;
}

interior b32
_octeti (
     chorda  t,
        s32  a,
        i32  n,
         i8* exitus)
{
    i32 k = ZEPHYRUM;
    i32 i;

    si (a < ZEPHYRUM)
    {
        redde FALSUM;
    }
    i = (i32)a;
    dum (k < n && i + I < t.mensura)
    {
        si (   _hex(t.datum[i]) >= ZEPHYRUM && _hex(t.datum[i + I])
            >= ZEPHYRUM)
        {
            exitus[k++] = (i8)(_hex(t.datum[i]) * XVI
                + _hex(t.datum[i + I]));
            i += II;
            perge;
        }
        si (t.datum[i] == ' ' || t.datum[i] == '\n')
        {
            i++;
            perge;
        }
        redde FALSUM;
    }
    redde k == n;
}

interior s32
_numerus (
     chorda t,
        s32 a)
{
    i32 v = ZEPHYRUM;
    i32 i;

    si (a < ZEPHYRUM)
    {
        redde -I;
    }
    per (i = (i32)a; i < t.mensura && t.datum[i] >= '0'
         && t.datum[i] <= '9'; i++)
    {
        v = v * X + (i32)(t.datum[i] - '0');
    }
    redde (s32)v;
}

/* vector RFC 9106 par. 5.3 ex textu */
nomen structura {
    Argon2idParametri par;
                   i8 tessera[XXXII];
                   i8 sal[XVI];
                   i8 secretum[VIII];
                   i8 ad[XII];
                   i8 h0[LXIV];
                   i8 tag[XXXII];
} Vector;

interior b32
_vector (
    Piscina* p,
     Vector* v)
{
    chorda t = filum_legere_totum(VIA_RFC, p);
    /* caput sectionis ipsum (index initio idem verbum habet) */
       s32 initium = _post(t, "\n5.3.  Argon2id Test Vectors\n");
    chorda s;

    si (initium < ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* solum sectio 5.3 (5.1 Argon2d, 5.2 Argon2i eadem verba) */
    s.datum    = t.datum + initium;
    s.mensura  = t.mensura - (i32)initium;
    memset(v, ZEPHYRUM, magnitudo(*v));
    v->par.m_cost_kib  = (i32)_numerus(s, _post(s, "Memory: "));
    v->par.t_cost      = (i32)_numerus(s, _post(s, "Passes: "));
    v->par.lanae       = (i32)_numerus(s, _post(s, "Parallelism: "));
    v->par.longitudo   = (i32)_numerus(s, _post(s, "Tag length: "));
    v->par.secretum    = v->secretum;
    v->par.secretum_n  = VIII;
    v->par.ad          = v->ad;
    v->par.ad_n        = XII;
    redde v->par.m_cost_kib == XXXII && v->par.t_cost == III
        && v->par.lanae == IV && v->par.longitudo == XXXII
        && _octeti(s, _post(s, "Password[32]: "), XXXII, v->tessera)
        && _octeti(s, _post(s, "Salt[16]: "), XVI, v->sal)
        && _octeti(s, _post(s, "Secret[8]: "), VIII, v->secretum)
        && _octeti(s, _post(s, "Associated data[12]: "), XII, v->ad)
        && _octeti(s, _post(s, "Pre-hashing digest: "), LXIV, v->h0)
        && _octeti(s, _post(s, "Tag: "), XXXII, v->tag);
}

interior vacuum
_quaterni (
    Blake2b* b,
        i32  x)
{
    i8 o[IV];

    o[ZEPHYRUM]  = (i8)(x & 0xFF);
    o[I]         = (i8)((x >> VIII) & 0xFF);
    o[II]        = (i8)((x >> XVI) & 0xFF);
    o[III]       = (i8)((x >> XXIV) & 0xFF);
    blake2b_addere(b, o, IV);
}

interior vacuum
_sectio_fixum (CredoContextus* c)
{
       chorda t = filum_legere_totum(VIA_RFC, c->piscina);
     Sigillum s;
    character hex[SIGILLUM_HEX_MENSURA];

    CREDO_NECESSE(t.mensura > ZEPHYRUM);
    s = sigillum_computare(t.datum, (memoriae_index)t.mensura);
    sigillum_hex(&s, hex);
    CREDO_VERUM(strcmp(hex, SIGILLUM_RFC) == ZEPHYRUM);
}

/* H0 secundum RFC 9106 par. 3.2 ex parametris textus */
interior vacuum
_sectio_digestum_primum (CredoContextus* c)
{
     Vector v;
    Blake2b b;
         i8 h0[LXIV];

    CREDO_NECESSE(_vector(c->piscina, &v));
    CREDO_VERUM(blake2b_incipere(&b, LXIV, NIHIL, ZEPHYRUM));
    _quaterni(&b, v.par.lanae);
    _quaterni(&b, v.par.longitudo);
    _quaterni(&b, v.par.m_cost_kib);
    _quaterni(&b, v.par.t_cost);
    _quaterni(&b, ARGON2ID_VERSIO);
    _quaterni(&b, II);                 /* y = 2: Argon2id */
    _quaterni(&b, XXXII);
    blake2b_addere(&b, v.tessera, XXXII);
    _quaterni(&b, XVI);
    blake2b_addere(&b, v.sal, XVI);
    _quaterni(&b, VIII);
    blake2b_addere(&b, v.secretum, VIII);
    _quaterni(&b, XII);
    blake2b_addere(&b, v.ad, XII);
    blake2b_finire(&b, h0);
    CREDO_VERUM(memcmp(h0, v.h0, LXIV) == ZEPHYRUM);
}

interior vacuum
_sectio_vector (CredoContextus* c)
{
    Vector v;
        i8 tag[XXXII];

    CREDO_NECESSE(_vector(c->piscina, &v));
    CREDO_VERUM(argon2id_computare(&v.par, v.tessera, XXXII, v.sal, XVI,
        tag, c->piscina));
    CREDO_VERUM(memcmp(tag, v.tag, XXXII) == ZEPHYRUM);
}

/* gradus {1, 7, 64, omnes}: idem Tag; gradus 1 vere gradatim */
interior vacuum
_sectio_gradatim (CredoContextus* c)
{
    Vector v;
       i32 magnitudines[IV];
       i32 k;

    CREDO_NECESSE(_vector(c->piscina, &v));
    magnitudines[ZEPHYRUM]  = I;
    magnitudines[I]         = VII;
    magnitudines[II]        = LXIV;
    magnitudines[III]       = ZEPHYRUM;
    per (k = ZEPHYRUM; k < IV; k++)
    {
        Piscina* p = piscina_generare_dynamicum("argon2id", LXIV
                              * MXXIV);
        Argon2idOpus* o = argon2id_incipere(&v.par, v.tessera, XXXII,
                              v.sal, XVI, p);
                  i8 tag[XXXII];
                 i32 gradus = ZEPHYRUM;

        CREDO_NECESSE_NON_NIHIL(o);
        dum (!argon2id_gradus(o, magnitudines[k]))
        {
            gradus++;
        }
        argon2id_digestum(o, tag);
        CREDO_VERUM(memcmp(tag, v.tag, XXXII) == ZEPHYRUM);
        si (magnitudines[k] == I)
        {
            /* m = 32 caudices, t = 3: > 32 gradus */
            CREDO_VERUM(gradus > XXXII);
        }
        si (magnitudines[k] == ZEPHYRUM)
        {
            CREDO_AEQUALIS_I32(gradus, ZEPHYRUM);
        }
        piscina_destruere(p);
    }
}

interior vacuum
_sectio_phc (CredoContextus* c)
{
    Argon2idParametri par;
    Argon2idParametri lecti;
                   i8 sal[XVI];
                   i8 digestum[XXXII];
                   i8 sal_lectum[LXIV];
                   i8 digestum_lectum[LXIV];
                  i32 sn;
                  i32 dn;
               chorda phc;
                  i32 i;

    memset(&par, ZEPHYRUM, magnitudo(par));
    par.t_cost      = II;
    par.m_cost_kib  = 19456;
    par.lanae       = I;
    par.longitudo   = XXXII;
    memcpy(sal, "somesaltsomesalt", XVI);
    per (i = ZEPHYRUM; i < XXXII; i++)
    {
        digestum[i] = (i8)(i * VII);
    }
    phc = argon2id_phc_scribere(&par, sal, XVI, digestum, c->piscina);
    CREDO_VERUM(chorda_aequalis_literis(phc,
        "$argon2id$v=19$m=19456,t=2,p=1$c29tZXNhbHRzb21lc2FsdA$"
        "AAcOFRwjKjE4P0ZNVFtiaXB3foWMk5qhqK+2vcTL0tk"));

    memset(&lecti, ZEPHYRUM, magnitudo(lecti));
    CREDO_VERUM(argon2id_phc_legere(phc, &lecti, sal_lectum, &sn,
        digestum_lectum,
        &dn));
    CREDO_AEQUALIS_I32(lecti.t_cost, II);
    CREDO_AEQUALIS_I32(lecti.m_cost_kib, 19456);
    CREDO_AEQUALIS_I32(lecti.lanae, I);
    CREDO_AEQUALIS_I32(lecti.longitudo, XXXII);
    CREDO_AEQUALIS_I32(sn, XVI);
    CREDO_AEQUALIS_I32(dn, XXXII);
    CREDO_VERUM(memcmp(sal_lectum, sal, XVI) == ZEPHYRUM);
    CREDO_VERUM(memcmp(digestum_lectum, digestum, XXXII) == ZEPHYRUM);

    /* malae recusantur */
    {
        constans character* malae[IX];
                       i32  k;

        malae[ZEPHYRUM] =
            "$argon2i$v=19$m=19456,t=2,p=1$c29tZXNhbHQ$AAcOFRwjKjE4";
        malae[I] =
            "$argon2id$v=16$m=19456,t=2,p=1$c29tZXNhbHQ$AAcOFRwjKjE4";
        malae[II] = "$argon2id$v=19$m=19456,t=2,p=1$c29tZXNhbHQ";
        malae[III] =
            "$argon2id$v=19$m=19456,t=2$c29tZXNhbHQ$AAcOFRwjKjE4";
        malae[IV] =
            "$argon2id$v=19$m=19456,t=2,p=1$c29t*XNhbHQ$AAcOFRwjKjE4";
        malae[V] =
            "$argon2id$v=19$m=19456,t=0,p=1$c29tZXNhbHQ$AAcOFRwjKjE4";
        malae[VI] =
            "$argon2id$v=19$m=x,t=2,p=1$c29tZXNhbHQ$AAcOFRwjKjE4";
        malae[VII] =
            "$argon2id$v=19$m=19456,t=2,p=1$c29tZXNhbHQ$AAcOFRwjKjE4$";
        malae[VIII] = "";
        per (k = ZEPHYRUM; k < IX; k++)
        {
            CREDO_FALSUM(argon2id_phc_legere(chorda_ex_literis(malae[k],
                c->piscina), &lecti, sal_lectum, &sn, digestum_lectum,
                &dn));
        }
    }
}

interior vacuum
_sectio_limites (CredoContextus* c)
{
    Argon2idParametri par;
                   i8 sal[XVI];

    memset(sal, ZEPHYRUM, magnitudo(sal));
    memset(&par, ZEPHYRUM, magnitudo(par));
    par.t_cost      = I;
    par.m_cost_kib  = VIII;
    par.lanae       = I;
    par.longitudo   = XXXII;
    CREDO_NECESSE_NON_NIHIL(argon2id_incipere(&par,
        (constans i8*)"x", I, sal, XVI, c->piscina));
    par.t_cost = ZEPHYRUM;
    CREDO_NIHIL(argon2id_incipere(&par, (constans i8*)"x", I, sal, XVI,
        c->piscina));
    par.t_cost  = I;
    par.lanae   = ZEPHYRUM;
    CREDO_NIHIL(argon2id_incipere(&par, (constans i8*)"x", I, sal, XVI,
        c->piscina));
    par.lanae = II;   /* m < 8 p */
    CREDO_NIHIL(argon2id_incipere(&par, (constans i8*)"x", I, sal, XVI,
        c->piscina));
    par.lanae      = I;
    par.longitudo  = III;
    CREDO_NIHIL(argon2id_incipere(&par, (constans i8*)"x", I, sal, XVI,
        c->piscina));
    par.longitudo = XXXII;
    CREDO_NIHIL(argon2id_incipere(&par, (constans i8*)"x", I, sal, VII,
        c->piscina));   /* sal < 8 */
}

/* parametri productionis: tempus < 1 s (custos, non mensura) */
interior vacuum
_sectio_tempus (CredoContextus* c)
{
    Argon2idParametri par;
                   i8 sal[XVI];
                   i8 tag[XXXII];
      structura timeval  a;
      structura timeval  b;
                  s64  ms;
              Piscina* p = piscina_generare_dynamicum("argon2id_tempus",
                            LXIV * MXXIV);

    memset(sal, VII, magnitudo(sal));
    memset(&par, ZEPHYRUM, magnitudo(par));
    par.t_cost      = II;
    par.m_cost_kib  = 19456;
    par.lanae       = I;
    par.longitudo   = XXXII;
    gettimeofday(&a, NIHIL);
    CREDO_VERUM(argon2id_computare(&par, (constans i8*)"tessera", VII,
        sal, XVI, tag, p));
    gettimeofday(&b, NIHIL);
    ms = ((s64)b.tv_sec - (s64)a.tv_sec) * M
        + ((s64)b.tv_usec - (s64)a.tv_usec) / M;
    imprimere("  argon2id m=19456 t=2 p=1: %ld ms\n", (longus)ms);
    CREDO_VERUM(ms < M);
    piscina_destruere(p);
    (vacuum)c;
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "fixum", _sectio_fixum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "praehash", _sectio_digestum_primum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vector", _sectio_vector, NIHIL, NIHIL, NIHIL, NIHIL },
    { "gradatim", _sectio_gradatim, NIHIL, NIHIL, NIHIL, NIHIL },
    { "phc", _sectio_phc, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limites", _sectio_limites, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tempus", _sectio_tempus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("argon2id", SECTIONES);
}
