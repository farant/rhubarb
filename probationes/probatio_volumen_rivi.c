/* probatio_volumen_rivi.c - hospitium v3 T6: durabilitas plena
 * optionalis (synchronous FULL) in scrinio et volumine; massae per
 * rivum: filum magnum in massam (sigillum verificatum, dedup sine
 * lectione, savepoint intra transactionem vocatoris), lectio per
 * partes, filum vacuum, mensura mendax; inter lectionem scriptio
 * visibilis et checkpoint perficitur (ansa per partem). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "moneta.h"
#include "sigillum.h"
#include "scrinium.h"
#include "volumen.h"
#include <stdio.h>
#include <string.h>

#define MAGNUM (III * MXXIV * MXXIV)

interior s64
_pragma_legere (
    Scrinium* s)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(s,
        "PRAGMA synchronous");
                   s64 v = -I;

    si (e != NIHIL && scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        v = scrinium_columna_numerus(e, ZEPHYRUM);
    }
    scrinium_finire(e);
    redde v;
}

interior constans character*
_via_nova (
               Piscina* p,
    constans character* finis)
{
    character  n[MONETA_NANOID_PLAGULAE_MENSURA];
    character* via = (character*)piscina_allocare(p, CXXVIII);

    (vacuum)moneta_nanoid_plagulae(n);
    sprintf(via, "/tmp/volumen_rivi_%s%s", n, finis);
    redde via;
}

interior i8*
_octeti (
    Piscina* p,
        i32  n,
        i32  semen)
{
     i8* d = (i8*)piscina_allocare(p, (i64)n + I);
    i32  x = semen;
    i32  i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        x     = x * 1103515245U + 12345U;
        d[i]  = (i8)(x >> XVI);
    }
    redde d;
}

/* filum scribere, sigillum hex reddere */
interior constans character*
_filum (
               Piscina* p,
    constans character* via,
                    i8* d,
                   i32  n)
{
    character* hex = (character*)piscina_allocare(p,
                         SIGILLUM_HEX_MENSURA);
     Sigillum s = sigillum_computare(d, (memoriae_index)n);

    (vacuum)filum_scribere(via, chorda_ex_buffer(d, n));
    sigillum_hex(&s, hex);
    redde hex;
}

interior vacuum
_sectio_plenus (CredoContextus* c)
{
      ScriniumOptiones  optiones_scrinii;
       VolumenOptiones  optiones_voluminis;
    constans character* via  = _via_nova(c->piscina, ".db");
    constans character* vv   = _via_nova(c->piscina, ".volumen");
              Scrinium* s;
               Volumen* v;

    memset(&optiones_scrinii, ZEPHYRUM, magnitudo(optiones_scrinii));
    optiones_scrinii.synchronus_plenus = VERUM;
    s = scrinium_aperire_cum(c->piscina, via,
        &optiones_scrinii);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_VERUM(_pragma_legere(s) == II);
    scrinium_claudere(s);
    s = scrinium_aperire(c->piscina, via);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_VERUM(_pragma_legere(s) == I);
    scrinium_claudere(s);

    memset(&optiones_voluminis, ZEPHYRUM,
        magnitudo(optiones_voluminis));
    optiones_voluminis.durabilitas_plena = VERUM;
    v = volumen_aperire_aut_creare_cum(c->piscina, vv,
        &optiones_voluminis);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_VERUM(_pragma_legere(volumen_scrinium(v)) == II);
    volumen_claudere(v);
    v = volumen_aperire_aut_creare_cum(c->piscina, vv, NIHIL);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_VERUM(_pragma_legere(volumen_scrinium(v)) == I);
    volumen_claudere(v);
    (vacuum)filum_delere(via);
    (vacuum)filum_delere(vv);
}

/* massa per rivum legenda in partibus VII KB; VERUM si aequalis */
interior b32
_legere_aequale (
               Volumen* v,
    constans character* hex,
                    i8* expectata,
                   i64  n,
               Piscina* p)
{
    VolumenMassaLector* l;
                   i64  mensura  = ZEPHYRUM;
                   i64  lecta    = ZEPHYRUM;
                    i8  capsa[VII * MXXIV];
                   b32  aequale = VERUM;

    l = volumen_massam_aperire(v, chorda_ex_literis(hex, p), &mensura,
        p);
    si (l == NIHIL || mensura != n)
    {
        redde FALSUM;
    }
    per (;;)
    {
        s32 k = volumen_massam_legere(l, capsa, (i32)magnitudo(capsa));

        si (k < ZEPHYRUM)
        {
            aequale = FALSUM;
            frange;
        }
        si (k == ZEPHYRUM)
        {
            frange;
        }
        si (   lecta + (i64)k > n
            || memcmp(capsa, expectata + lecta, (size_t)k)
            != ZEPHYRUM)
        {
            aequale = FALSUM;
        }
        lecta += (i64)k;
    }
    volumen_massam_claudere(l);
    redde aequale && lecta == n;
}

interior vacuum
_sectio_rivus (CredoContextus* c)
{
               Volumen* v    = volumen_temporarium(c->piscina, "rivi");
    constans character* via  = _via_nova(c->piscina, ".bin");
                    i8* d    = _octeti(c->piscina, MAGNUM, VII);
    constans character* hex;
                   b32  inventum;
                chorda  promptum;

    CREDO_NECESSE_NON_NIHIL(v);
    hex = _filum(c->piscina, via, d, MAGNUM);
    CREDO_VERUM(volumen_massam_condere_ex_filo(v, via, MAGNUM, hex));
    CREDO_VERUM(volumen_summa_massarum(v) == I);
    CREDO_VERUM(_legere_aequale(v, hex, d, MAGNUM, c->piscina));
    promptum = volumen_massam_promere(v, chorda_ex_literis(hex,
        c->piscina), c->piscina, &inventum);
    CREDO_VERUM(inventum && promptum.mensura == MAGNUM
        && memcmp(promptum.datum, d, MAGNUM) == ZEPHYRUM);
    /* ignotum: lector NIHIL */
    {
        i64 m;

        CREDO_NIHIL(volumen_massam_aperire(v, chorda_ex_literis(
            "00000000000000000000000000000000"
            "00000000000000000000000000000000", c->piscina), &m,
            c->piscina));
    }
    (vacuum)filum_delere(via);
    volumen_claudere(v);
}

interior vacuum
_sectio_semel (CredoContextus* c)
{
               Volumen* v    = volumen_temporarium(c->piscina, "rivi");
    constans character* via  = _via_nova(c->piscina, ".bin");
                    i8* d    = _octeti(c->piscina, C * MXXIV, XI);
    constans character* hex;

    CREDO_NECESSE_NON_NIHIL(v);
    hex = _filum(c->piscina, via, d, C * MXXIV);
    CREDO_VERUM(volumen_massam_condere_ex_filo(v, via, C * MXXIV, hex));
    /* filum deletum: secunda vocatio filum NON legit */
    CREDO_VERUM(filum_delere(via));
    CREDO_VERUM(volumen_massam_condere_ex_filo(v, via, C * MXXIV, hex));
    CREDO_VERUM(volumen_summa_massarum(v) == I);
    volumen_claudere(v);
}

interior vacuum
_sectio_sigillum_falsum (CredoContextus* c)
{
               Volumen* v    = volumen_temporarium(c->piscina, "rivi");
    constans character* via  = _via_nova(c->piscina, ".bin");
                    i8* d    = _octeti(c->piscina, CC * MXXIV, XIII);
    constans character* hex;
             character  falsum[SIGILLUM_HEX_MENSURA];
                   b32  inventum;

    CREDO_NECESSE_NON_NIHIL(v);
    hex = _filum(c->piscina, via, d, CC * MXXIV);
    strcpy(falsum, hex);
    falsum[ZEPHYRUM] = falsum[ZEPHYRUM] == 'a' ? 'b' : 'a';
    CREDO_FALSUM(volumen_massam_condere_ex_filo(v, via, CC * MXXIV,
        falsum));
    CREDO_VERUM(volumen_summa_massarum(v) == ZEPHYRUM);
    (vacuum)volumen_massam_promere(v, chorda_ex_literis(falsum,
        c->piscina), c->piscina, &inventum);
    CREDO_FALSUM(inventum);
    CREDO_VERUM(strstr(volumen_error(v), "sigillum") != NIHIL);
    /* forma mala */
    CREDO_FALSUM(volumen_massam_condere_ex_filo(v, via, CC * MXXIV,
        "abc"));
    CREDO_FALSUM(volumen_massam_condere_ex_filo(v, via, CC * MXXIV,
        "ABCDEF00000000000000000000000000"
        "00000000000000000000000000000000"
        ));

    /* intra transactionem vocatoris: actum vocatoris superstat */
    CREDO_VERUM(volumen_transactionem_incipere(v));
    CREDO_VERUM(volumen_actum_appendere(v, "proba",
        chorda_ex_literis("{}", c->piscina)) > ZEPHYRUM);
    CREDO_FALSUM(volumen_massam_condere_ex_filo(v, via, CC * MXXIV,
        falsum));
    CREDO_VERUM(volumen_massam_condere_ex_filo(v, via, CC * MXXIV,
        hex));
    CREDO_VERUM(volumen_transactionem_committere(v));
    CREDO_VERUM(volumen_summa_massarum(v) == I);
    CREDO_VERUM(volumen_summa_actorum(v) == II);  /* creatum + proba */
    (vacuum)filum_delere(via);
    volumen_claudere(v);
}

interior vacuum
_sectio_vacua (CredoContextus* c)
{
               Volumen* v    = volumen_temporarium(c->piscina, "rivi");
    constans character* via  = _via_nova(c->piscina, ".bin");
                    i8  nihil[I];
    constans character* hex;

    CREDO_NECESSE_NON_NIHIL(v);
    hex = _filum(c->piscina, via, nihil, ZEPHYRUM);
    CREDO_VERUM(strcmp(hex, "e3b0c44298fc1c149afbf4c8996fb924"
        "27ae41e4649b934ca495991b7852b855") == ZEPHYRUM);
    CREDO_VERUM(volumen_massam_condere_ex_filo(v, via, ZEPHYRUM, hex));
    CREDO_VERUM(_legere_aequale(v, hex, nihil, ZEPHYRUM, c->piscina));
    (vacuum)filum_delere(via);
    volumen_claudere(v);
}

interior vacuum
_sectio_mensura_mendax (CredoContextus* c)
{
               Volumen* v    = volumen_temporarium(c->piscina, "rivi");
    constans character* via  = _via_nova(c->piscina, ".bin");
                    i8* d    = _octeti(c->piscina, M, XVII);
    constans character* hex;

    CREDO_NECESSE_NON_NIHIL(v);
    hex = _filum(c->piscina, via, d, M);
    CREDO_FALSUM(volumen_massam_condere_ex_filo(v, via, M + I, hex));
    CREDO_FALSUM(volumen_massam_condere_ex_filo(v, via, M - I, hex));
    CREDO_VERUM(volumen_summa_massarum(v) == ZEPHYRUM);
    (vacuum)filum_delere(via);
    CREDO_FALSUM(volumen_massam_condere_ex_filo(v, via, M, hex));
    volumen_claudere(v);
}

/* scriptio inter lectionem per partes: connexio altera actum videt et
 * checkpoint TRUNCATE perficitur ANTE finem lectionis (ansa non
 * tenetur inter partes - ansa tenta checkpoint frangit) */
interior vacuum
_sectio_scriptio_inter_lectionem (CredoContextus* c)
{
               Volumen* v    = volumen_temporarium(c->piscina, "rivi");
    constans character* via  = _via_nova(c->piscina, ".bin");
                    i8* d    = _octeti(c->piscina, CC * MXXIV, XIX);
    constans character* hex;
    VolumenMassaLector* l;
                   i64  mensura;
                    i8  capsa[VII * MXXIV];
               Volumen* alter;

    CREDO_NECESSE_NON_NIHIL(v);
    hex = _filum(c->piscina, via, d, CC * MXXIV);
    CREDO_VERUM(volumen_massam_condere_ex_filo(v, via, CC * MXXIV,
        hex));
    l = volumen_massam_aperire(v, chorda_ex_literis(hex, c->piscina),
        &mensura, c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(volumen_massam_legere(l, capsa, (i32)magnitudo(capsa))
        > ZEPHYRUM);
    CREDO_VERUM(volumen_actum_appendere(v, "inter",
        chorda_ex_literis("{}", c->piscina)) > ZEPHYRUM);
    /* nullum snapshot tenetur: checkpoint TRUNCATE perficitur
     * (busy = 0) etiam inter lectionem */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(volumen_scrinium(v),
            "PRAGMA wal_checkpoint(TRUNCATE)");

        CREDO_NECESSE_NON_NIHIL(e);
        CREDO_VERUM(scrinium_gradi(e) == SCRINIUM_ORDO);
        CREDO_VERUM(scrinium_columna_numerus(e, ZEPHYRUM) == ZEPHYRUM);
        scrinium_finire(e);
    }
    alter = volumen_aperire(c->piscina, volumen_via(v));
    CREDO_NECESSE_NON_NIHIL(alter);
    CREDO_VERUM(volumen_summa_actorum(alter) == II);
    volumen_claudere(alter);
    CREDO_VERUM(volumen_massam_legere(l, capsa, (i32)magnitudo(capsa))
        > ZEPHYRUM);
    volumen_massam_claudere(l);
    (vacuum)filum_delere(via);
    volumen_claudere(v);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "plenus", _sectio_plenus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rivus", _sectio_rivus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dedup", _sectio_semel, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sigillum_falsum", _sectio_sigillum_falsum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "vacua", _sectio_vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mensura_mendax", _sectio_mensura_mendax, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scriptio_inter_lectionem", _sectio_scriptio_inter_lectionem,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("volumen_rivi", SECTIONES);
}
