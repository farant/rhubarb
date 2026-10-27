/* probatio_moneta.c - ULID + fons fortuitorum + nanoid
 *
 * moneta nullam probationem habuit usque ad 2026-08-01, cum
 * moneta_octeti_fortuiti addita est (fons secretorum). Functio
 * quae claves parit sine probatione manere non debet.
 *
 * Nanoid (2026-09-10): signa per intervalla SCRIPTA iudicantur, non
 * per macra alphabetorum - macrum mutatum probationem secum non
 * traheret. Uniformitas per differentiam mediarum: modulus pro
 * larva signa prima praeferret, et probatio id videre debet.
 */

#include "latina.h"
#include "moneta.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>
#include <string.h>

/* signum alphabeti nanoid ordinarii (0-9 A-Z a-z _ -)? */
interior b32
_signum_nanoid (
    character c)
{
    si (   (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')
        || (c >= 'a' && c <= 'z') || c == '_' || c == '-')
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* signum alphabeti plagulae (0-9 A-Z)? */
interior b32
_signum_plagulae (
    character c)
{
    si ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z'))
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* forma identitatis fixae: duae differunt; CC vocationes quaeque
 * longitudinis rectae, signis validis; omnia signa alphabeti
 * apparent (200 vocationes = ~66 apparitiones singulorum signorum
 * in utroque alphabeto - signum absens = alphabetum truncatum aut
 * larva nimis parva) */
interior vacuum
_probare_formam (
    b32 (*generare)(character*),
    b32 (*validum)(character),
    i32   longitudo,
    i32   distincta_exspectata)
{
    character a[XXXII];
    character b[XXXII];
           i8 visa[CCLVI];
          i32 i;
          i32 k;
          i32 defectus   = ZEPHYRUM;
          i32 extranea   = ZEPHYRUM;
          i32 distincta  = ZEPHYRUM;

    CREDO_VERUM(generare(a));
    CREDO_VERUM(generare(b));
    CREDO_AEQUALIS_I32((i32)strlen(a), longitudo);
    CREDO_VERUM(strcmp(a, b) != ZEPHYRUM);

    memset(visa, ZEPHYRUM, magnitudo(visa));
    per (i = ZEPHYRUM; i < CC; i++)
    {
        si (!generare(a) || (i32)strlen(a) != longitudo)
        {
            defectus++;
            perge;
        }
        per (k = ZEPHYRUM; k < longitudo; k++)
        {
            si (!validum(a[k]))
            {
                extranea++;
            }
            alioquin si (!visa[(i8)a[k]])
            {
                visa[(i8)a[k]] = I;
                distincta++;
            }
        }
    }
    CREDO_AEQUALIS_I32(defectus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(extranea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(distincta, distincta_exspectata);
}

/* signa effusionis in [prima, ultima] numerat (numeri[signum -
 * prima]); redde numerum signorum extra intervallum */
interior i32
_numerare (
    constans character* effusio,
                   i32  longitudo,
             character  prima,
             character  ultima,
                   i32* numeri)
{
    i32 k;
    i32 extra = ZEPHYRUM;

    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        si (effusio[k] < prima || effusio[k] > ultima)
        {
            extra++;
        }
        alioquin
        {
            numeri[effusio[k] - prima]++;
        }
    }
    redde extra;
}

/* ---- ULID: forma et unicitas ---- */
interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    character a[MONETA_ULID_MENSURA];
    character b[MONETA_ULID_MENSURA];

    (vacuum)c;
    moneta_ulid(a);
    moneta_ulid(b);

    CREDO_AEQUALIS_I32((i32)strlen(a), XXVI);
    CREDO_AEQUALIS_I32((i32)strlen(b), XXVI);
    /* monotonia intra ms eundem: a <= b semper */
    CREDO_VERUM(strcmp(a, b) <= ZEPHYRUM);
    CREDO_VERUM(strcmp(a, b) != ZEPHYRUM);
}

/* ---- ULID fortuitum: sine monotonia, cauda differt ---- */
interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    character a[MONETA_ULID_MENSURA];
    character b[MONETA_ULID_MENSURA];

    (vacuum)c;
    moneta_ulid_fortuita(a);
    moneta_ulid_fortuita(b);

    CREDO_AEQUALIS_I32((i32)strlen(a), XXVI);
    CREDO_VERUM(strcmp(a, b) != ZEPHYRUM);
    /* praefixum temporis (X characteres) idem intra ms eundem,
     * cauda fortuita differt - id ipsum est cur haec varians
     * exsistit (praefixa brevia discernenda) */
    CREDO_VERUM(strncmp(a + X, b + X, XVI) != ZEPHYRUM);
}

/* ---- octeti fortuiti: fons secretorum ---- */
interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
     i8 a[32];
     i8 b[32];
     i8 sentinella[8];
    i32 i;
    i32 differentiae = ZEPHYRUM;

    (vacuum)c;

    memset(a, ZEPHYRUM, magnitudo(a));
    memset(b, ZEPHYRUM, magnitudo(b));
    CREDO_VERUM(moneta_octeti_fortuiti(a, XXXII));
    CREDO_VERUM(moneta_octeti_fortuiti(b, XXXII));

    /* duae vocationes differant (probabilitas collisionis
     * 2^-256 - si haec cadit, fons fractus est, non fortuna) */
    per (i = ZEPHYRUM; i < XXXII; i++)
    {
        si (a[i] != b[i])
        {
            differentiae++;
        }
    }
    CREDO_VERUM(differentiae > ZEPHYRUM);

    /* non omnes zephyri (fons mutus id daret) */
    differentiae = ZEPHYRUM;
    per (i = ZEPHYRUM; i < XXXII; i++)
    {
        si (a[i] != ZEPHYRUM)
        {
            differentiae++;
        }
    }
    CREDO_VERUM(differentiae > ZEPHYRUM);

    /* argumenta mala: FALSUM, effusio INTACTA */
    memset(sentinella, 0x5A, magnitudo(sentinella));
    CREDO_FALSUM(moneta_octeti_fortuiti(NIHIL, VIII));
    CREDO_FALSUM(moneta_octeti_fortuiti(sentinella, ZEPHYRUM));
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        CREDO_AEQUALIS_I32((i32)(i8)sentinella[i], 0x5A);
    }
}

/* ---- nanoid et nanoid plagulae: forma ---- */
interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    _probare_formam(moneta_nanoid, _signum_nanoid, XXI, LXIV);
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    _probare_formam(moneta_nanoid_plagulae, _signum_plagulae, XII,
                    XXXVI);
}

/* ---- nanoid configurabile: uniformitas (larva, non modulus) ----
 *
 * 144 x 1000 signa ex 0-9 A-Z (36). P = summa signorum primorum
 * quattuor, binomialis (144000, 1/9): media 16000, sigma ~119.
 * D = 9P - 144000: sub uniformitate 0 +- ~1073; sub modulo P =
 * 144000 x 32/256 = 18000, ergo D = 18000. Limen |D| minus 8000
 * (~7.5 sigma): casu numquam fallit, modulum semper capit.
 * Fascia 3400..4600 singulorum (sigma ~62, ~9.6 sigma) larvam
 * nimis parvam capit (signa ultima numquam apparent). */
interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        character effusio[M + I];
              i32 numeri[XXXVI];
              i32 i;
              i32 k;
              i32 defectus       = ZEPHYRUM;
              i32 extranea       = ZEPHYRUM;
              i32 extra_fasciam  = ZEPHYRUM;
              i32 prima;
              s32 differentia;
        character c;

        memset(numeri, ZEPHYRUM, magnitudo(numeri));
        per (i = ZEPHYRUM; i < CXLIV; i++)
        {
            si (!moneta_nanoid_alphabeto(effusio, M,
                    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
            {
                defectus++;
                perge;
            }
            per (k = ZEPHYRUM; k < M; k++)
            {
                c = effusio[k];
                si (c >= '0' && c <= '9')
                {
                    numeri[c - '0']++;
                }
                alioquin si (c >= 'A' && c <= 'Z')
                {
                    numeri[c - 'A' + X]++;
                }
                alioquin
                {
                    extranea++;
                }
            }
        }
        per (k = ZEPHYRUM; k < XXXVI; k++)
        {
            si (numeri[k] < 3400 || numeri[k] > 4600)
            {
                extra_fasciam++;
            }
        }
        prima = numeri[ZEPHYRUM] + numeri[I] + numeri[II] + numeri[III];
        differentia = (s32)(IX * prima) - (s32)144000;
        CREDO_AEQUALIS_I32(defectus, ZEPHYRUM);
        CREDO_AEQUALIS_I32(extranea, ZEPHYRUM);
        CREDO_AEQUALIS_I32(extra_fasciam, ZEPHYRUM);
        CREDO_VERUM(differentia < 8000 && differentia > -8000);
    }
}

/* ---- nanoid configurabile: alphabeta extrema ---- */
interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    character effusio[M + I];
    character alphabetum[CCLVI];
          i32 numeri[III];
          i32 i;

    (vacuum)c;

    /* "01": larva 1, nihil abicitur */
    memset(numeri, ZEPHYRUM, magnitudo(numeri));
    CREDO_VERUM(moneta_nanoid_alphabeto(effusio, M, "01"));
    CREDO_AEQUALIS_I32((i32)strlen(effusio), M);
    CREDO_AEQUALIS_I32(_numerare(effusio, M, '0', '1', numeri),
                       ZEPHYRUM);
    CREDO_VERUM(numeri[ZEPHYRUM] > ZEPHYRUM);
    CREDO_VERUM(numeri[I] > ZEPHYRUM);

    /* "012": larva 3, index 3 abicitur (alioquin terminator in
     * medio identitatis - strlen breviorem videret) */
    memset(numeri, ZEPHYRUM, magnitudo(numeri));
    CREDO_VERUM(moneta_nanoid_alphabeto(effusio, M, "012"));
    CREDO_AEQUALIS_I32((i32)strlen(effusio), M);
    CREDO_AEQUALIS_I32(_numerare(effusio, M, '0', '2', numeri),
                       ZEPHYRUM);
    CREDO_VERUM(numeri[ZEPHYRUM] > ZEPHYRUM);
    CREDO_VERUM(numeri[I] > ZEPHYRUM);
    CREDO_VERUM(numeri[II] > ZEPHYRUM);

    /* octeti 1..255 distincti: larva 255, index 255 abicitur;
     * octeti supra 127 per (i8) in visa legunt */
    per (i = ZEPHYRUM; i < CCLV; i++)
    {
        alphabetum[i] = (character)(i + I);
    }
    alphabetum[CCLV] = '\0';
    CREDO_VERUM(moneta_nanoid_alphabeto(effusio, M, alphabetum));
    CREDO_AEQUALIS_I32((i32)strlen(effusio), M);
}

/* ---- nanoid configurabile: terminus et recusationes ---- */
interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    character effusio[XVI];
          i32 i;
          i32 intacta = ZEPHYRUM;

    (vacuum)c;

    /* terminus: longitudo V scribit V signa + terminatorem,
     * nihil ultra */
    memset(effusio, 'Q', magnitudo(effusio));
    CREDO_VERUM(moneta_nanoid_alphabeto(effusio, V, "ABC"));
    CREDO_AEQUALIS_I32((i32)strlen(effusio), V);
    per (i = VI; i < XVI; i++)
    {
        si (effusio[i] == 'Q')
        {
            intacta++;
        }
    }
    CREDO_AEQUALIS_I32(intacta, X);

    /* recusationes: FALSUM, effusio "" (numquam dimidia). Gemellus
     * positivus supra ("ABC" in eodem buffer) mechanismum vivum
     * probat; "ABCA" ab eo solo signo duplicato differt. */
    CREDO_FALSUM(moneta_nanoid_alphabeto(NIHIL, V, "ABC"));

    memset(effusio, 'Q', magnitudo(effusio));
    CREDO_FALSUM(moneta_nanoid_alphabeto(effusio, V, NIHIL));
    CREDO_AEQUALIS_I32((i32)(i8)effusio[ZEPHYRUM], ZEPHYRUM);

    memset(effusio, 'Q', magnitudo(effusio));
    CREDO_FALSUM(moneta_nanoid_alphabeto(effusio, ZEPHYRUM, "ABC"));
    CREDO_AEQUALIS_I32((i32)(i8)effusio[ZEPHYRUM], ZEPHYRUM);

    memset(effusio, 'Q', magnitudo(effusio));
    CREDO_FALSUM(moneta_nanoid_alphabeto(effusio, V, "A"));
    CREDO_AEQUALIS_I32((i32)(i8)effusio[ZEPHYRUM], ZEPHYRUM);

    memset(effusio, 'Q', magnitudo(effusio));
    CREDO_FALSUM(moneta_nanoid_alphabeto(effusio, V, ""));
    CREDO_AEQUALIS_I32((i32)(i8)effusio[ZEPHYRUM], ZEPHYRUM);

    memset(effusio, 'Q', magnitudo(effusio));
    CREDO_FALSUM(moneta_nanoid_alphabeto(effusio, V, "ABCA"));
    CREDO_AEQUALIS_I32((i32)(i8)effusio[ZEPHYRUM], ZEPHYRUM);
}

/* moneta_numerator (D3): "1", "2", ... ex ctx; ctx positum sequitur
 * (fons praevisibilis probationum agendae et personarum) */
interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    character effusio[MONETA_ULID_MENSURA];
          i32 ultimum;

    (vacuum)c;
    ultimum = ZEPHYRUM;
    moneta_numerator(&ultimum, effusio);
    CREDO_VERUM(strcmp(effusio, "1") == ZEPHYRUM);
    moneta_numerator(&ultimum, effusio);
    CREDO_VERUM(strcmp(effusio, "2") == ZEPHYRUM);
    CREDO_AEQUALIS_I32(ultimum, II);
    ultimum = XCIX;
    moneta_numerator(&ultimum, effusio);
    CREDO_VERUM(strcmp(effusio, "100") == ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "moneta_ulid", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "moneta_ulid_fortuita", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "moneta_octeti_fortuiti", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "moneta_nanoid", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "moneta_nanoid_plagulae", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "uniformitatem nanoid", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "alphabeta extrema nanoid", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "recusationes nanoid", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "moneta_numerator", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("moneta", SECTIONES);
}
