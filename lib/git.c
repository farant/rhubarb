/* git.c - corpus lectoris git (doctrina in git.h)
 *
 * SHA-1 hic INTERIOR est (FIPS 180-1) - sigillum SHA-256 domus
 * manet unicum publicum; SHA-1 formae git servit nec vagatur.
 * Zlib = involucrum super flatura_inflare (caput II octetorum +
 * cauda adler32); flatura ad frustum finale desinit, ergo octeti
 * sequentes in sarcinis innocui sunt (mensuratum probatione).
 * Corpora sarcinarum numquam tota leguntur - fenestra per obiectum
 * (fseek/fread), crescens si INCOMPLETUS.
 */

#include "git.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "flatura.h"
#include "filum.h"
#include "via.h"
#include "iter_directoria.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GIT_PROFUNDITAS_DELTAE 64
#define GIT_FENESTRA_PRIMA     65536

/* --------------------------------------------------
 * auxilia octetorum
 * -------------------------------------------------- */

interior i32
_be32 (constans i8* p);

interior i32
_be32 (constans i8* p)
{
    redde ((i32)p[0] << 24) | ((i32)p[1] << 16)
        | ((i32)p[2] << 8) | (i32)p[3];
}

hic_manens constans character _hex_digiti[] = "0123456789abcdef";

interior vacuum
_bin_ad_hex (constans i8* bin, i32 quantum, character* exitus);

interior vacuum
_bin_ad_hex (constans i8* bin, i32 quantum, character* exitus)
{
    i32 i;

    per (i = 0; i < quantum; i = i + 1)
    {
        exitus[i * 2] = _hex_digiti[(bin[i] >> 4) & 0x0F];
        exitus[i * 2 + 1] = _hex_digiti[bin[i] & 0x0F];
    }
    exitus[quantum * 2] = '\0';
}

interior s32
_hex_valor (character c);

interior s32
_hex_valor (character c)
{
    si (c >= '0' && c <= '9')
    {
        redde c - '0';
    }
    si (c >= 'a' && c <= 'f')
    {
        redde c - 'a' + 10;
    }
    si (c >= 'A' && c <= 'F')
    {
        redde c - 'A' + 10;
    }
    redde -1;
}

/* VERUM si 40 hex; bin_exitus (20 octeti) impletur */
interior b32
_hex_ad_bin (constans character* hex, i8* bin_exitus);

interior b32
_hex_ad_bin (constans character* hex, i8* bin_exitus)
{
    i32 i;

    per (i = 0; i < 20; i = i + 1)
    {
        s32 altus = _hex_valor(hex[i * 2]);
        s32 imus = _hex_valor(hex[i * 2 + 1]);

        si (altus < 0 || imus < 0)
        {
            redde FALSUM;
        }
        bin_exitus[i] = (i8)((altus << 4) | imus);
    }
    redde hex[40] == '\0';
}

/* --------------------------------------------------
 * SHA-1 (FIPS 180-1) - interior, formae git servus
 * -------------------------------------------------- */

interior i32
_rotl (i32 x, i32 n);

interior i32
_rotl (i32 x, i32 n)
{
    redde (x << n) | (x >> (32 - n));
}

interior vacuum
_sha1_massam (constans i8* frustum, i32* h);

interior vacuum
_sha1_massam (constans i8* frustum, i32* h)
{
    i32 w[80];
    i32 a = h[0];
    i32 b = h[1];
    i32 c = h[2];
    i32 d = h[3];
    i32 e = h[4];
    i32 i;

    per (i = 0; i < 16; i = i + 1)
    {
        w[i] = _be32(frustum + i * 4);
    }
    per (i = 16; i < 80; i = i + 1)
    {
        w[i] = _rotl(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16],
            1);
    }
    per (i = 0; i < 80; i = i + 1)
    {
        i32 f;
        i32 k;
        i32 novum;

        si (i < 20)
        {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999u;
        }
        alioquin si (i < 40)
        {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1u;
        }
        alioquin si (i < 60)
        {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDCu;
        }
        alioquin
        {
            f = b ^ c ^ d;
            k = 0xCA62C1D6u;
        }
        novum = _rotl(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = _rotl(b, 30);
        b = a;
        a = novum;
    }
    h[0] = h[0] + a;
    h[1] = h[1] + b;
    h[2] = h[2] + c;
    h[3] = h[3] + d;
    h[4] = h[4] + e;
}

interior vacuum
_sha1 (constans i8* datum, memoriae_index mensura, i8* exitus);

interior vacuum
_sha1 (constans i8* datum, memoriae_index mensura, i8* exitus)
{
    i32            h[5];
    memoriae_index plena = mensura / 64;
    memoriae_index reliqua = mensura % 64;
    memoriae_index i;
    i8             cauda[128];
    memoriae_index cauda_mensura;

    h[0] = 0x67452301u;
    h[1] = 0xEFCDAB89u;
    h[2] = 0x98BADCFEu;
    h[3] = 0x10325476u;
    h[4] = 0xC3D2E1F0u;

    per (i = 0; i < plena; i = i + 1)
    {
        _sha1_massam(datum + i * 64, h);
    }

    memset(cauda, 0, 128);
    si (reliqua > 0)
    {
        memcpy(cauda, datum + plena * 64, reliqua);
    }
    cauda[reliqua] = (i8)0x80;
    cauda_mensura = reliqua < 56 ? 64 : 128;
    {
        /* longitudo in bitis, big-endian, octeti VIII ultimi */
        memoriae_index bita = mensura * 8;
        i32 j;

        per (j = 0; j < 8; j = j + 1)
        {
            cauda[cauda_mensura - 1 - (memoriae_index)j] =
                (i8)((bita >> (j * 8)) & 0xFF);
        }
    }
    _sha1_massam(cauda, h);
    si (cauda_mensura == 128)
    {
        _sha1_massam(cauda + 64, h);
    }
    per (i = 0; i < 5; i = i + 1)
    {
        exitus[i * 4] = (i8)((h[i] >> 24) & 0xFF);
        exitus[i * 4 + 1] = (i8)((h[i] >> 16) & 0xFF);
        exitus[i * 4 + 2] = (i8)((h[i] >> 8) & 0xFF);
        exitus[i * 4 + 3] = (i8)(h[i] & 0xFF);
    }
}

/* --------------------------------------------------
 * adler32 + involucrum zlib
 * -------------------------------------------------- */

interior i32
_adler32 (constans i8* datum, i32 mensura);

interior i32
_adler32 (constans i8* datum, i32 mensura)
{
    i32 a = 1;
    i32 b = 0;
    i32 i;

    per (i = 0; i < mensura; i = i + 1)
    {
        a = (a + (i32)datum[i]) % 65521u;
        b = (b + a) % 65521u;
    }
    redde (b << 16) | a;
}

/* caput zlib (II octeti) + deflate crudum + cauda adler32.
 * verificare_adler: solum ubi finis fluxus notus est (obiecta
 * laxa - plagula tota); in sarcinis longitudo compressa ignota. */
interior FlaturaFructus
_zlib_inflare (constans i8* datum, i32 mensura, Piscina* piscina,
    b32 verificare_adler);

interior FlaturaFructus
_zlib_inflare (constans i8* datum, i32 mensura, Piscina* piscina,
    b32 verificare_adler)
{
    FlaturaFructus fructus;

    fructus.status = FLATURA_STATUS_FRACTA_DATUM;
    fructus.datum = NIHIL;
    fructus.mensura = 0;
    si (mensura < 2)
    {
        redde fructus;
    }
    si ((datum[0] & 0x0F) != 8
        || (((i32)datum[0] * 256 + (i32)datum[1]) % 31u) != 0
        || (datum[1] & 0x20) != 0)
    {
        redde fructus;
    }
    fructus = flatura_inflare(datum + 2, mensura - 2, piscina);
    si (fructus.status != FLATURA_STATUS_OK)
    {
        redde fructus;
    }
    si (verificare_adler && mensura >= 6)
    {
        i32 exspectatum = _be32(datum + mensura - 4);
        i32 computatum = _adler32(fructus.datum, fructus.mensura);

        si (exspectatum != computatum)
        {
            fructus.status = FLATURA_STATUS_FRACTA_DATUM;
            fructus.datum = NIHIL;
            fructus.mensura = 0;
        }
    }
    redde fructus;
}

/* --------------------------------------------------
 * lectio plagularum per partes (sarcinae magnae)
 * -------------------------------------------------- */

interior i8*
_partem_legere (constans character* via, s64 offset, s64 quantum,
    Piscina* piscina, s64* lectum);

interior i8*
_partem_legere (constans character* via, s64 offset, s64 quantum,
    Piscina* piscina, s64* lectum)
{
    FILE* pl = fopen(via, "rb");
    i8*   buffer;

    *lectum = 0;
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, (longus)offset, SEEK_SET) != 0)
    {
        fclose(pl);
        redde NIHIL;
    }
    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)quantum);
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    *lectum = (s64)fread(buffer, 1, (memoriae_index)quantum, pl);
    fclose(pl);
    redde buffer;
}

/* --------------------------------------------------
 * apertio repositorii
 * -------------------------------------------------- */

nomen structura {
    Piscina* piscina;
    Xar*     viae_idx;   /* de chorda (viae absolutae .idx) */
} SarcinaeContextus;

interior s32
_sarcinae_ambulator (chorda via_plena,
    constans DirectoriumIntroitus* introitus, vacuum* contextus);

interior s32
_sarcinae_ambulator (chorda via_plena,
    constans DirectoriumIntroitus* introitus, vacuum* contextus)
{
    SarcinaeContextus* ctx = (SarcinaeContextus*)contextus;

    si (introitus->genus != INTROITUS_FILUM)
    {
        redde 0;
    }
    si (chorda_terminatur(via_plena,
        chorda_ex_literis(".idx", ctx->piscina)))
    {
        chorda* cella = (chorda*)xar_addere(ctx->viae_idx);

        si (cella != NIHIL)
        {
            *cella = via_plena;
        }
    }
    redde 0;
}

interior constans character*
_viam_serere (Piscina* piscina, constans character* a,
    constans character* b);

interior constans character*
_viam_serere (Piscina* piscina, constans character* a,
    constans character* b)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)128);

    chorda_aedificator_appendere_literis(aed, a);
    chorda_aedificator_appendere_literis(aed, b);
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}

GitRepositorium*
git_aperire (Piscina* piscina, constans character* via)
{
    GitRepositorium* repositorium;
    chorda           radix;
    constans character* via_git = NIHIL;

    radix = via_absoluta(chorda_ex_literis(via, piscina), piscina);
    dum (radix.mensura > 0)
    {
        constans character* candidata = _viam_serere(piscina,
            chorda_ut_cstr(radix, piscina), "/.git/HEAD");

        si (filum_existit(candidata))
        {
            via_git = _viam_serere(piscina,
                chorda_ut_cstr(radix, piscina), "/.git");
            frange;
        }
        /* ascendere: segmentum ultimum tondere */
        {
            i32 i = radix.mensura;

            dum (i > 0 && ((character*)radix.datum)[i - 1] != '/')
            {
                i = i - 1;
            }
            si (i <= 1)
            {
                frange;
            }
            radix.mensura = i - 1;
        }
    }
    si (via_git == NIHIL)
    {
        redde NIHIL;
    }

    /* recusatio sha256 (extensiones in config) */
    {
        constans character* via_config = _viam_serere(piscina,
            via_git, "/config");

        si (filum_existit(via_config))
        {
            chorda config = filum_legere_totum(via_config,
                piscina);

            si (chorda_continet(config,
                chorda_ex_literis("sha256", piscina)))
            {
                redde NIHIL;
            }
        }
    }

    repositorium = (GitRepositorium*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(GitRepositorium));
    si (repositorium == NIHIL)
    {
        redde NIHIL;
    }
    repositorium->piscina = piscina;
    repositorium->via_git = via_git;
    repositorium->sarcinae = xar_creare(piscina,
        (i32)magnitudo(GitSarcina));
    si (repositorium->sarcinae == NIHIL)
    {
        redde NIHIL;
    }

    /* sarcinae: indices toti in memoriam (parvi), corpora manent
     * in disco */
    {
        SarcinaeContextus  ctx;
        DirectoriumFiltrum filtrum;
        constans character* via_pack = _viam_serere(piscina,
            via_git, "/objects/pack");
        i32 i;

        ctx.piscina = piscina;
        ctx.viae_idx = xar_creare(piscina, (i32)magnitudo(chorda));
        si (ctx.viae_idx == NIHIL)
        {
            redde NIHIL;
        }
        filtrum = directorium_filtrum_omnia();
        filtrum.includere_occultos = FALSUM;
        directorium_ambulare(via_pack, &filtrum,
            _sarcinae_ambulator, &ctx, piscina);

        per (i = 0; i < xar_numerus(ctx.viae_idx); i = i + 1)
        {
            chorda via_idx = *(chorda*)xar_obtinere(ctx.viae_idx,
                i);
            constans character* via_idx_cstr = chorda_ut_cstr(
                via_idx, piscina);
            chorda idx = filum_legere_totum(via_idx_cstr, piscina);
            GitSarcina* s;

            /* v2 solum: magia \377tOc versio 2 */
            si (idx.mensura < 8 + 1024
                || (i8)idx.datum[0] != (i8)0xFF
                || idx.datum[1] != (i8)'t'
                || idx.datum[2] != (i8)'O'
                || idx.datum[3] != (i8)'c'
                || _be32((i8*)idx.datum + 4) != 2)
            {
                perge;   /* idx v1 aut fracta - praetermissa */
            }
            s = (GitSarcina*)xar_addere(repositorium->sarcinae);
            si (s == NIHIL)
            {
                redde NIHIL;
            }
            /* .idx -> .pack */
            {
                ChordaAedificator* aed = chorda_aedificator_creare(
                    piscina, (memoriae_index)128);

                chorda_aedificator_appendere_chorda(aed,
                    chorda_ex_buffer(via_idx.datum,
                        via_idx.mensura - 4));
                chorda_aedificator_appendere_literis(aed, ".pack");
                s->via_sarcinae = chorda_ut_cstr(
                    chorda_aedificator_finire(aed), piscina);
            }
            s->idx_datum = (i8*)idx.datum;
            s->idx_mensura = idx.mensura;
            s->numerus = _be32((i8*)idx.datum + 8 + 255 * 4);
        }
    }
    redde repositorium;
}

/* --------------------------------------------------
 * quaestio in indice sarcinae: sha_bin -> offset
 * -------------------------------------------------- */

interior b32
_sarcinae_offset (constans GitSarcina* s, constans i8* sha_bin,
    s64* offset_exitus);

interior b32
_sarcinae_offset (constans GitSarcina* s, constans i8* sha_bin,
    s64* offset_exitus)
{
    constans i8* fanout = s->idx_datum + 8;
    constans i8* shas = fanout + 1024;
    i32 imus = sha_bin[0] == 0 ? 0
        : _be32(fanout + ((i32)sha_bin[0] - 1) * 4);
    i32 summus = _be32(fanout + (i32)sha_bin[0] * 4);
    i32 inventus = -0;
    b32 est_inventus = FALSUM;

    dum (imus < summus)
    {
        i32 medius = imus + (summus - imus) / 2;
        integer ordo = memcmp(shas + medius * 20, sha_bin, 20);

        si (ordo == 0)
        {
            inventus = medius;
            est_inventus = VERUM;
            frange;
        }
        alioquin si (ordo < 0)
        {
            imus = medius + 1;
        }
        alioquin
        {
            summus = medius;
        }
    }
    si (!est_inventus)
    {
        redde FALSUM;
    }
    {
        constans i8* offsets32 = shas + (memoriae_index)s->numerus * 20
            + (memoriae_index)s->numerus * 4;
        i32 crudus = _be32(offsets32 + inventus * 4);

        si (crudus & 0x80000000u)
        {
            /* tabula offsetuum magnorum (VIII octeti) */
            constans i8* magni = offsets32
                + (memoriae_index)s->numerus * 4;
            i32 index_magni = crudus & 0x7FFFFFFFu;
            i32 altus = _be32(magni + index_magni * 8);
            i32 imus_pars = _be32(magni + index_magni * 8 + 4);

            *offset_exitus = ((s64)altus << 32) | (s64)imus_pars;
        }
        alioquin
        {
            *offset_exitus = (s64)crudus;
        }
    }
    redde VERUM;
}

/* --------------------------------------------------
 * obiecta e sarcina (cum deltis)
 * -------------------------------------------------- */

interior chorda
_deltam_applicare (Piscina* piscina, chorda basis,
    constans i8* delta, i32 mensura, b32* successus);

interior chorda
_deltam_applicare (Piscina* piscina, chorda basis,
    constans i8* delta, i32 mensura, b32* successus)
{
    chorda fructus;
    i32    positus = 0;
    memoriae_index basis_mensura = 0;
    memoriae_index fructus_mensura = 0;
    i32    gradus;
    i8*    exitus;
    memoriae_index scriptum = 0;

    fructus.datum = NIHIL;
    fructus.mensura = 0;
    *successus = FALSUM;

    /* mensurae: catervae VII-bitorum, minima prima */
    gradus = 0;
    dum (positus < mensura)
    {
        i8 b = delta[positus];

        positus = positus + 1;
        basis_mensura = basis_mensura
            | ((memoriae_index)(b & 0x7F) << gradus);
        gradus = gradus + 7;
        si ((b & 0x80) == 0)
        {
            frange;
        }
    }
    gradus = 0;
    dum (positus < mensura)
    {
        i8 b = delta[positus];

        positus = positus + 1;
        fructus_mensura = fructus_mensura
            | ((memoriae_index)(b & 0x7F) << gradus);
        gradus = gradus + 7;
        si ((b & 0x80) == 0)
        {
            frange;
        }
    }
    si (basis_mensura != (memoriae_index)basis.mensura)
    {
        redde fructus;
    }
    exitus = (i8*)piscina_allocare(piscina,
        fructus_mensura == 0 ? (memoriae_index)1 : fructus_mensura);
    si (exitus == NIHIL)
    {
        redde fructus;
    }

    dum (positus < mensura)
    {
        i8 op = delta[positus];

        positus = positus + 1;
        si (op & 0x80)
        {
            /* copia e basi: offset/mensura octetatim per vexilla */
            memoriae_index co = 0;
            memoriae_index cm = 0;
            i32 j;

            per (j = 0; j < 4; j = j + 1)
            {
                si (op & (i8)(1 << j))
                {
                    si (positus >= mensura)
                    {
                        redde fructus;
                    }
                    co = co | ((memoriae_index)delta[positus]
                        << (j * 8));
                    positus = positus + 1;
                }
            }
            per (j = 0; j < 3; j = j + 1)
            {
                si (op & (i8)(1 << (4 + j)))
                {
                    si (positus >= mensura)
                    {
                        redde fructus;
                    }
                    cm = cm | ((memoriae_index)delta[positus]
                        << (j * 8));
                    positus = positus + 1;
                }
            }
            si (cm == 0)
            {
                cm = 0x10000;
            }
            si (co + cm > (memoriae_index)basis.mensura
                || scriptum + cm > fructus_mensura)
            {
                redde fructus;
            }
            memcpy(exitus + scriptum,
                (constans i8*)basis.datum + co, cm);
            scriptum = scriptum + cm;
        }
        alioquin si (op != 0)
        {
            memoriae_index quantum = (memoriae_index)op;

            si (positus + (i32)quantum > mensura
                || scriptum + quantum > fructus_mensura)
            {
                redde fructus;
            }
            memcpy(exitus + scriptum, delta + positus, quantum);
            positus = positus + (i32)quantum;
            scriptum = scriptum + quantum;
        }
        alioquin
        {
            redde fructus;   /* op 0 reservatum */
        }
    }
    si (scriptum != fructus_mensura)
    {
        redde fructus;
    }
    fructus.datum = exitus;
    fructus.mensura = (i32)fructus_mensura;
    *successus = VERUM;
    redde fructus;
}

interior GitObiectum
_obiectum_ad_offset (GitRepositorium* repositorium,
    constans GitSarcina* s, s64 offset, Piscina* piscina,
    i32 profunditas);

/* fenestra crescens: legere partem, inflare; INCOMPLETUS ->
 * fenestra quadruplicata */
interior FlaturaFructus
_zlib_e_sarcina (constans character* via, s64 offset,
    Piscina* piscina);

interior FlaturaFructus
_zlib_e_sarcina (constans character* via, s64 offset,
    Piscina* piscina)
{
    s64 fenestra = GIT_FENESTRA_PRIMA;
    FlaturaFructus fructus;

    fructus.status = FLATURA_STATUS_FRACTA_DATUM;
    fructus.datum = NIHIL;
    fructus.mensura = 0;
    per (;;)
    {
        s64 lectum = 0;
        i8* datum = _partem_legere(via, offset, fenestra, piscina,
            &lectum);

        si (datum == NIHIL || lectum < 2)
        {
            redde fructus;
        }
        fructus = _zlib_inflare(datum, (i32)lectum, piscina,
            FALSUM);
        si (fructus.status == FLATURA_STATUS_OK)
        {
            redde fructus;
        }
        si (fructus.status != FLATURA_STATUS_INCOMPLETUS
            || lectum < fenestra)
        {
            redde fructus;   /* fractura vera aut finis plagulae */
        }
        fenestra = fenestra * 4;
    }
}

interior GitObiectum
_obiectum_ad_offset (GitRepositorium* repositorium,
    constans GitSarcina* s, s64 offset, Piscina* piscina,
    i32 profunditas)
{
    GitObiectum obiectum;
    s64 lectum = 0;
    i8* caput;
    i32 positus = 0;
    i32 genus_crudum;
    i8  b;

    obiectum.successus = FALSUM;
    obiectum.genus = GIT_OBIECTUM_IGNOTUM;
    obiectum.datum.datum = NIHIL;
    obiectum.datum.mensura = 0;
    si (profunditas > GIT_PROFUNDITAS_DELTAE)
    {
        redde obiectum;
    }

    /* caput obiecti: genus (III bita) + mensura (varint) - caput
     * parvum, XXXII octeti abunde */
    caput = _partem_legere(s->via_sarcinae, offset, 32, piscina,
        &lectum);
    si (caput == NIHIL || lectum < 1)
    {
        redde obiectum;
    }
    b = caput[positus];
    positus = positus + 1;
    genus_crudum = (i32)((b >> 4) & 0x07);
    dum ((b & 0x80) != 0 && positus < (i32)lectum)
    {
        b = caput[positus];
        positus = positus + 1;
    }

    si (genus_crudum == 6)
    {
        /* OFS_DELTA: offset negativum (codificatio +1 gradatim) */
        s64 negativum;

        si (positus >= (i32)lectum)
        {
            redde obiectum;
        }
        b = caput[positus];
        positus = positus + 1;
        negativum = (s64)(b & 0x7F);
        dum ((b & 0x80) != 0)
        {
            si (positus >= (i32)lectum)
            {
                redde obiectum;
            }
            b = caput[positus];
            positus = positus + 1;
            negativum = ((negativum + 1) << 7) | (s64)(b & 0x7F);
        }
        {
            GitObiectum basis = _obiectum_ad_offset(repositorium,
                s, offset - negativum, piscina, profunditas + 1);
            FlaturaFructus delta;
            b32 bene = FALSUM;
            chorda corpus;

            si (!basis.successus)
            {
                redde obiectum;
            }
            delta = _zlib_e_sarcina(s->via_sarcinae,
                offset + positus, piscina);
            si (delta.status != FLATURA_STATUS_OK)
            {
                redde obiectum;
            }
            corpus = _deltam_applicare(piscina, basis.datum,
                delta.datum, delta.mensura, &bene);
            si (!bene)
            {
                redde obiectum;
            }
            obiectum.successus = VERUM;
            obiectum.genus = basis.genus;
            obiectum.datum = corpus;
            redde obiectum;
        }
    }
    si (genus_crudum == 7)
    {
        /* REF_DELTA: sha basis (XX octeti crudi) */
        character sha_basis[GIT_SHA_HEX_MENSURA];
        GitObiectum basis;
        FlaturaFructus delta;
        b32 bene = FALSUM;
        chorda corpus;

        si (positus + 20 > (i32)lectum)
        {
            redde obiectum;
        }
        _bin_ad_hex(caput + positus, 20, sha_basis);
        basis = git_obiectum_legere(repositorium, sha_basis,
            piscina);
        si (!basis.successus)
        {
            redde obiectum;
        }
        delta = _zlib_e_sarcina(s->via_sarcinae,
            offset + positus + 20, piscina);
        si (delta.status != FLATURA_STATUS_OK)
        {
            redde obiectum;
        }
        corpus = _deltam_applicare(piscina, basis.datum,
            delta.datum, delta.mensura, &bene);
        si (!bene)
        {
            redde obiectum;
        }
        obiectum.successus = VERUM;
        obiectum.genus = basis.genus;
        obiectum.datum = corpus;
        redde obiectum;
    }
    si (genus_crudum >= 1 && genus_crudum <= 4)
    {
        FlaturaFructus corpus = _zlib_e_sarcina(s->via_sarcinae,
            offset + positus, piscina);

        si (corpus.status != FLATURA_STATUS_OK)
        {
            redde obiectum;
        }
        obiectum.successus = VERUM;
        obiectum.genus = genus_crudum == 1 ? GIT_OBIECTUM_COMMISSUM
            : genus_crudum == 2 ? GIT_OBIECTUM_ARBOR
            : genus_crudum == 3 ? GIT_OBIECTUM_MASSA
            : GIT_OBIECTUM_SIGNUM;
        obiectum.datum.datum = (i8*)corpus.datum;
        obiectum.datum.mensura = corpus.mensura;
        redde obiectum;
    }
    redde obiectum;
}

/* --------------------------------------------------
 * obiecta laxa + lectio generalis
 * -------------------------------------------------- */

interior GitObiectumGenus
_genus_ex_titulo (constans i8* datum, i32 mensura);

interior GitObiectumGenus
_genus_ex_titulo (constans i8* datum, i32 mensura)
{
    si (mensura >= 6 && memcmp(datum, "commit", 6) == 0)
    {
        redde GIT_OBIECTUM_COMMISSUM;
    }
    si (mensura >= 4 && memcmp(datum, "tree", 4) == 0)
    {
        redde GIT_OBIECTUM_ARBOR;
    }
    si (mensura >= 4 && memcmp(datum, "blob", 4) == 0)
    {
        redde GIT_OBIECTUM_MASSA;
    }
    si (mensura >= 3 && memcmp(datum, "tag", 3) == 0)
    {
        redde GIT_OBIECTUM_SIGNUM;
    }
    redde GIT_OBIECTUM_IGNOTUM;
}

/* VERIFICATIO OMNIS LECTIONIS: index contenti recomputatus contra
 * sha petitum - lector se ipsum probat (lectio poculi CRC veneno
 * infecti: oraculum alterius GENERIS in ipso instrumento). */
interior b32
_obiectum_verificare (Piscina* piscina, constans GitObiectum* o,
    constans character* sha);

interior b32
_obiectum_verificare (Piscina* piscina, constans GitObiectum* o,
    constans character* sha)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)64);
    chorda    caput;
    i8*       totum;
    i8        sha_bin[20];
    character sha_hex[GIT_SHA_HEX_MENSURA];

    chorda_aedificator_appendere_literis(aed,
        o->genus == GIT_OBIECTUM_COMMISSUM ? "commit"
        : o->genus == GIT_OBIECTUM_ARBOR ? "tree"
        : o->genus == GIT_OBIECTUM_MASSA ? "blob" : "tag");
    chorda_aedificator_appendere_character(aed, ' ');
    chorda_aedificator_appendere_i32(aed, o->datum.mensura);
    caput = chorda_aedificator_finire(aed);

    totum = (i8*)piscina_allocare(piscina,
        (memoriae_index)caput.mensura + 1
            + (memoriae_index)o->datum.mensura);
    si (totum == NIHIL)
    {
        redde FALSUM;
    }
    memcpy(totum, caput.datum, (memoriae_index)caput.mensura);
    totum[caput.mensura] = 0;
    si (o->datum.mensura > 0)
    {
        memcpy(totum + caput.mensura + 1, o->datum.datum,
            (memoriae_index)o->datum.mensura);
    }
    _sha1(totum, (memoriae_index)caput.mensura + 1
        + (memoriae_index)o->datum.mensura, sha_bin);
    _bin_ad_hex(sha_bin, 20, sha_hex);
    redde memcmp(sha_hex, sha, 40) == 0;
}

GitObiectum
git_obiectum_legere (GitRepositorium* repositorium,
    constans character* sha, Piscina* piscina)
{
    GitObiectum obiectum;
    i8          sha_bin[20];

    obiectum.successus = FALSUM;
    obiectum.genus = GIT_OBIECTUM_IGNOTUM;
    obiectum.datum.datum = NIHIL;
    obiectum.datum.mensura = 0;
    si (!_hex_ad_bin(sha, sha_bin))
    {
        redde obiectum;
    }

    /* laxum primum */
    {
        ChordaAedificator* aed = chorda_aedificator_creare(
            repositorium->piscina, (memoriae_index)128);
        constans character* via_laxa;

        chorda_aedificator_appendere_literis(aed,
            repositorium->via_git);
        chorda_aedificator_appendere_literis(aed, "/objects/");
        chorda_aedificator_appendere_character(aed, sha[0]);
        chorda_aedificator_appendere_character(aed, sha[1]);
        chorda_aedificator_appendere_character(aed, '/');
        chorda_aedificator_appendere_literis(aed, sha + 2);
        via_laxa = chorda_ut_cstr(chorda_aedificator_finire(aed),
            repositorium->piscina);

        si (filum_existit(via_laxa))
        {
            chorda compressa = filum_legere_totum(via_laxa,
                piscina);
            FlaturaFructus cruda = _zlib_inflare(
                (constans i8*)compressa.datum, compressa.mensura,
                piscina, VERUM);
            i32 i;

            si (cruda.status != FLATURA_STATUS_OK)
            {
                redde obiectum;
            }
            /* caput: "genus mensura\0" */
            per (i = 0; i < cruda.mensura; i = i + 1)
            {
                si (cruda.datum[i] == 0)
                {
                    obiectum.genus = _genus_ex_titulo(cruda.datum,
                        i);
                    obiectum.datum.datum = cruda.datum + i + 1;
                    obiectum.datum.mensura = cruda.mensura - i - 1;
                    obiectum.successus =
                        obiectum.genus != GIT_OBIECTUM_IGNOTUM
                        && _obiectum_verificare(piscina, &obiectum,
                               sha);
                    redde obiectum;
                }
            }
            redde obiectum;
        }
    }

    /* deinde sarcinae */
    {
        i32 i;

        per (i = 0; i < xar_numerus(repositorium->sarcinae);
            i = i + 1)
        {
            constans GitSarcina* s = (constans GitSarcina*)
                xar_obtinere(repositorium->sarcinae, i);
            s64 offset = 0;

            si (_sarcinae_offset(s, sha_bin, &offset))
            {
                obiectum = _obiectum_ad_offset(repositorium, s,
                    offset, piscina, 0);
                si (obiectum.successus)
                {
                    obiectum.successus = _obiectum_verificare(
                        piscina, &obiectum, sha);
                }
                redde obiectum;
            }
        }
    }
    redde obiectum;
}

/* --------------------------------------------------
 * refs: CAPUT, laxae, compactae
 * -------------------------------------------------- */

interior b32
_sha_ex_chorda (chorda textus, character* exitus);

interior b32
_sha_ex_chorda (chorda textus, character* exitus)
{
    i32 i;

    si (textus.mensura < 40)
    {
        redde FALSUM;
    }
    per (i = 0; i < 40; i = i + 1)
    {
        character c = (character)textus.datum[i];

        si (_hex_valor(c) < 0)
        {
            redde FALSUM;
        }
        exitus[i] = c;
    }
    exitus[40] = '\0';
    redde VERUM;
}

interior b32
_ref_legere (GitRepositorium* repositorium,
    constans character* titulus, character* sha_exitus);

interior b32
_ref_legere (GitRepositorium* repositorium,
    constans character* titulus, character* sha_exitus)
{
    /* laxa: .git/<titulus> */
    {
        ChordaAedificator* aed = chorda_aedificator_creare(
            repositorium->piscina, (memoriae_index)128);
        constans character* via_ref;

        chorda_aedificator_appendere_literis(aed,
            repositorium->via_git);
        chorda_aedificator_appendere_character(aed, '/');
        chorda_aedificator_appendere_literis(aed, titulus);
        via_ref = chorda_ut_cstr(chorda_aedificator_finire(aed),
            repositorium->piscina);
        si (filum_existit(via_ref))
        {
            chorda contentum = filum_legere_totum(via_ref,
                repositorium->piscina);

            redde _sha_ex_chorda(contentum, sha_exitus);
        }
    }
    /* compacta: packed-refs lineatim "sha titulus" */
    {
        constans character* via_compacta = _viam_serere(
            repositorium->piscina, repositorium->via_git,
            "/packed-refs");

        si (filum_existit(via_compacta))
        {
            chorda contentum = filum_legere_totum(via_compacta,
                repositorium->piscina);
            i32 initium = 0;
            i32 i;
            i32 titulus_mensura = (i32)strlen(titulus);

            per (i = 0; i <= contentum.mensura; i = i + 1)
            {
                si (i == contentum.mensura
                    || contentum.datum[i] == '\n')
                {
                    chorda linea = chorda_ex_buffer(
                        contentum.datum + initium, i - initium);

                    initium = i + 1;
                    si (linea.mensura < 42
                        || linea.datum[0] == '#'
                        || linea.datum[0] == '^')
                    {
                        perge;
                    }
                    si (linea.mensura == 41 + titulus_mensura
                        && linea.datum[40] == ' '
                        && memcmp(linea.datum + 41, titulus,
                               (memoriae_index)titulus_mensura)
                               == 0)
                    {
                        redde _sha_ex_chorda(linea, sha_exitus);
                    }
                }
            }
        }
    }
    redde FALSUM;
}

/* --------------------------------------------------
 * sha breve: praefixum unicum trans sarcinas et laxa
 * -------------------------------------------------- */

interior b32
_praefixo_congruit (constans i8* sha_bin,
    constans character* praefixum, i32 mensura);

interior b32
_praefixo_congruit (constans i8* sha_bin,
    constans character* praefixum, i32 mensura)
{
    i32 i;

    per (i = 0; i < mensura; i = i + 1)
    {
        i32 octetus = (i32)sha_bin[i / 2];
        s32 nibulus = (i % 2) == 0
            ? (s32)((octetus >> 4) & 0x0F)
            : (s32)(octetus & 0x0F);

        si (nibulus != _hex_valor(praefixum[i]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

nomen structura {
    constans character* praefixum;
    i32                 mensura;
    i32                 inventa;
    character           candidatus[GIT_SHA_HEX_MENSURA];
} ShaBreveContextus;

interior vacuum
_candidatum_notare (ShaBreveContextus* ctx,
    constans character* sha_hex);

interior vacuum
_candidatum_notare (ShaBreveContextus* ctx,
    constans character* sha_hex)
{
    si (ctx->inventa > 0
        && memcmp(ctx->candidatus, sha_hex, 40) == 0)
    {
        redde;   /* idem obiectum laxum ET in sarcina - non ambiguum */
    }
    ctx->inventa = ctx->inventa + 1;
    memcpy(ctx->candidatus, sha_hex, 41);
}

interior s32
_laxa_breve_ambulator (chorda via_plena,
    constans DirectoriumIntroitus* introitus, vacuum* contextus);

interior s32
_laxa_breve_ambulator (chorda via_plena,
    constans DirectoriumIntroitus* introitus, vacuum* contextus)
{
    ShaBreveContextus* ctx = (ShaBreveContextus*)contextus;
    character          sha_hex[GIT_SHA_HEX_MENSURA];
    i32                i;

    si (introitus->genus != INTROITUS_FILUM
        || via_plena.mensura < 38)
    {
        redde 0;
    }
    /* titulus plagulae = 38 hex; praefixum[0..2) iam directorio
     * congruit */
    sha_hex[0] = ctx->praefixum[0];
    sha_hex[1] = ctx->praefixum[1];
    per (i = 0; i < 38; i = i + 1)
    {
        character c = (character)via_plena.datum[
            via_plena.mensura - 38 + i];

        si (_hex_valor(c) < 0)
        {
            redde 0;
        }
        sha_hex[2 + i] = c;
    }
    sha_hex[40] = '\0';
    per (i = 2; i < ctx->mensura; i = i + 1)
    {
        si (sha_hex[i] != ctx->praefixum[i])
        {
            redde 0;
        }
    }
    _candidatum_notare(ctx, sha_hex);
    redde 0;
}

interior b32
_sha_breve_resolvere (GitRepositorium* repositorium,
    constans character* praefixum, character* sha_exitus);

interior b32
_sha_breve_resolvere (GitRepositorium* repositorium,
    constans character* praefixum, character* sha_exitus)
{
    ShaBreveContextus ctx;
    i32 mensura = (i32)strlen(praefixum);
    i32 i;

    si (mensura < 4 || mensura > 39)
    {
        redde FALSUM;
    }
    per (i = 0; i < mensura; i = i + 1)
    {
        si (_hex_valor(praefixum[i]) < 0)
        {
            redde FALSUM;
        }
    }
    ctx.praefixum = praefixum;
    ctx.mensura = mensura;
    ctx.inventa = 0;

    /* sarcinae: fanout octetum primum angustat, deinde percursus */
    per (i = 0; i < xar_numerus(repositorium->sarcinae); i = i + 1)
    {
        constans GitSarcina* s = (constans GitSarcina*)
            xar_obtinere(repositorium->sarcinae, i);
        constans i8* fanout = s->idx_datum + 8;
        constans i8* shas = fanout + 1024;
        i32 primus = (i32)((_hex_valor(praefixum[0]) << 4)
            | _hex_valor(praefixum[1]));
        i32 imus = primus == 0 ? 0 : _be32(fanout + (primus - 1) * 4);
        i32 summus = _be32(fanout + primus * 4);
        i32 j;

        per (j = imus; j < summus; j = j + 1)
        {
            si (_praefixo_congruit(shas + j * 20, praefixum,
                mensura))
            {
                character sha_hex[GIT_SHA_HEX_MENSURA];

                _bin_ad_hex(shas + j * 20, 20, sha_hex);
                _candidatum_notare(&ctx, sha_hex);
                si (ctx.inventa > 1)
                {
                    redde FALSUM;   /* ambiguum */
                }
            }
        }
    }

    /* laxa: directorium praefixi bilitterati */
    {
        ChordaAedificator* aed = chorda_aedificator_creare(
            repositorium->piscina, (memoriae_index)128);
        DirectoriumFiltrum filtrum;

        chorda_aedificator_appendere_literis(aed,
            repositorium->via_git);
        chorda_aedificator_appendere_literis(aed, "/objects/");
        chorda_aedificator_appendere_character(aed, praefixum[0]);
        chorda_aedificator_appendere_character(aed, praefixum[1]);
        filtrum = directorium_filtrum_omnia();
        filtrum.includere_occultos = FALSUM;
        directorium_ambulare(chorda_ut_cstr(
            chorda_aedificator_finire(aed),
            repositorium->piscina), &filtrum,
            _laxa_breve_ambulator, &ctx, repositorium->piscina);
    }

    si (ctx.inventa != 1)
    {
        redde FALSUM;   /* nihil aut ambiguum */
    }
    memcpy(sha_exitus, ctx.candidatus, 41);
    redde VERUM;
}

interior b32
_caput_legere (GitRepositorium* repositorium,
    character* sha_exitus);

interior b32
_caput_legere (GitRepositorium* repositorium,
    character* sha_exitus)
{
    constans character* via_caput = _viam_serere(
        repositorium->piscina, repositorium->via_git, "/HEAD");
    chorda contentum;

    si (!filum_existit(via_caput))
    {
        redde FALSUM;
    }
    contentum = filum_legere_totum(via_caput,
        repositorium->piscina);
    si (contentum.mensura > 5
        && memcmp(contentum.datum, "ref: ", 5) == 0)
    {
        /* titulus ref sine linea nova finali */
        i32 finis = contentum.mensura;
        ChordaAedificator* aed;

        dum (finis > 5 && (contentum.datum[finis - 1] == '\n'
            || contentum.datum[finis - 1] == '\r'))
        {
            finis = finis - 1;
        }
        aed = chorda_aedificator_creare(repositorium->piscina,
            (memoriae_index)128);
        chorda_aedificator_appendere_chorda(aed,
            chorda_ex_buffer(contentum.datum + 5,
                (i32)(finis - 5)));
        redde _ref_legere(repositorium,
            chorda_ut_cstr(chorda_aedificator_finire(aed),
                repositorium->piscina), sha_exitus);
    }
    redde _sha_ex_chorda(contentum, sha_exitus);
}

/* signum adnotatum -> commissum (linea "object <sha>") */
interior b32
_ad_commissum_pellere (GitRepositorium* repositorium,
    character* sha, Piscina* piscina);

interior b32
_ad_commissum_pellere (GitRepositorium* repositorium,
    character* sha, Piscina* piscina)
{
    i32 gradus;

    per (gradus = 0; gradus < 8; gradus = gradus + 1)
    {
        GitObiectum obiectum = git_obiectum_legere(repositorium,
            sha, piscina);

        si (!obiectum.successus)
        {
            redde FALSUM;
        }
        si (obiectum.genus != GIT_OBIECTUM_SIGNUM)
        {
            redde VERUM;
        }
        si (obiectum.datum.mensura < 47
            || memcmp(obiectum.datum.datum, "object ", 7) != 0)
        {
            redde FALSUM;
        }
        si (!_sha_ex_chorda(chorda_ex_buffer(
                obiectum.datum.datum + 7, 40), sha))
        {
            redde FALSUM;
        }
    }
    redde FALSUM;
}

b32
git_ref_resolvere (GitRepositorium* repositorium,
    constans character* textus, character* sha_exitus)
{
    i32 mensura = (i32)strlen(textus);
    i32 tilde = -0;
    b32 habet_tilde = FALSUM;
    s32 gradus_parentum = 0;
    character basis[256];
    i32 i;

    si (mensura == 0 || mensura >= 250)
    {
        redde FALSUM;
    }
    per (i = 0; i < mensura; i = i + 1)
    {
        si (textus[i] == '~')
        {
            tilde = i;
            habet_tilde = VERUM;
            frange;
        }
    }
    si (habet_tilde)
    {
        memcpy(basis, textus, (memoriae_index)tilde);
        basis[tilde] = '\0';
        gradus_parentum = tilde + 1 < mensura
            ? (s32)atoi(textus + tilde + 1) : 1;
        si (gradus_parentum < 0)
        {
            redde FALSUM;
        }
    }
    alioquin
    {
        memcpy(basis, textus, (memoriae_index)mensura + 1);
    }

    /* basis: sha plenum | CAPUT | nomen ref */
    {
        b32 resolutum = FALSUM;
        i8  bin_probatio[20];

        si (strlen(basis) == 40 && _hex_ad_bin(basis, bin_probatio))
        {
            memcpy(sha_exitus, basis, 41);
            resolutum = VERUM;
        }
        alioquin si (strcmp(basis, "HEAD") == 0
            || strcmp(basis, "CAPUT") == 0)
        {
            resolutum = _caput_legere(repositorium, sha_exitus);
        }
        alioquin
        {
            si (strncmp(basis, "refs/", 5) == 0)
            {
                resolutum = _ref_legere(repositorium, basis,
                    sha_exitus);
            }
            si (!resolutum)
            {
                constans character* candidata = _viam_serere(
                    repositorium->piscina, "refs/heads/", basis);

                resolutum = _ref_legere(repositorium, candidata,
                    sha_exitus);
            }
            si (!resolutum)
            {
                constans character* candidata = _viam_serere(
                    repositorium->piscina, "refs/tags/", basis);

                resolutum = _ref_legere(repositorium, candidata,
                    sha_exitus);
            }
            /* postremo: sha breve (refs vincunt praefixa) */
            si (!resolutum)
            {
                resolutum = _sha_breve_resolvere(repositorium,
                    basis, sha_exitus);
            }
        }
        si (!resolutum)
        {
            redde FALSUM;
        }
    }

    /* ~N: parens primus N-ies (signa pelluntur) */
    dum (gradus_parentum > 0)
    {
        GitCommissum commissum;

        si (!_ad_commissum_pellere(repositorium, sha_exitus,
            repositorium->piscina))
        {
            redde FALSUM;
        }
        commissum = git_commissum_legere(repositorium, sha_exitus,
            repositorium->piscina);
        si (!commissum.successus
            || xar_numerus(commissum.parentes) == 0)
        {
            redde FALSUM;
        }
        {
            chorda primus = *(chorda*)xar_obtinere(
                commissum.parentes, 0);

            memcpy(sha_exitus, primus.datum, 40);
            sha_exitus[40] = '\0';
        }
        gradus_parentum = gradus_parentum - 1;
    }
    redde VERUM;
}

/* --------------------------------------------------
 * commissum + arbor + massa per viam
 * -------------------------------------------------- */

GitCommissum
git_commissum_legere (GitRepositorium* repositorium,
    constans character* sha, Piscina* piscina)
{
    GitCommissum commissum;
    GitObiectum  obiectum;
    chorda       datum;
    i32          initium = 0;
    i32          i;

    commissum.successus = FALSUM;
    commissum.arbor.datum = NIHIL;
    commissum.arbor.mensura = 0;
    commissum.parentes = xar_creare(piscina,
        (i32)magnitudo(chorda));
    commissum.auctor = commissum.arbor;
    commissum.commissor = commissum.arbor;
    commissum.tempus = 0;
    commissum.nuntius = commissum.arbor;

    obiectum = git_obiectum_legere(repositorium, sha, piscina);
    si (!obiectum.successus
        || obiectum.genus != GIT_OBIECTUM_COMMISSUM
        || commissum.parentes == NIHIL)
    {
        redde commissum;
    }
    datum = obiectum.datum;

    per (i = 0; i <= datum.mensura; i = i + 1)
    {
        si (i == datum.mensura || datum.datum[i] == '\n')
        {
            chorda linea = chorda_ex_buffer(datum.datum + initium,
                i - initium);

            initium = i + 1;
            si (linea.mensura == 0)
            {
                /* nuntius sequitur */
                si (initium <= datum.mensura)
                {
                    commissum.nuntius = chorda_ex_buffer(
                        datum.datum + initium,
                        datum.mensura - initium);
                }
                frange;
            }
            si (linea.datum[0] == ' ')
            {
                perge;   /* continuatio (gpgsig) */
            }
            si (linea.mensura >= 45
                && memcmp(linea.datum, "tree ", 5) == 0)
            {
                commissum.arbor = chorda_ex_buffer(
                    linea.datum + 5, 40);
            }
            alioquin si (linea.mensura >= 47
                && memcmp(linea.datum, "parent ", 7) == 0)
            {
                chorda* cella = (chorda*)xar_addere(
                    commissum.parentes);

                si (cella == NIHIL)
                {
                    redde commissum;
                }
                *cella = chorda_ex_buffer(linea.datum + 7, 40);
            }
            alioquin si (linea.mensura > 7
                && memcmp(linea.datum, "author ", 7) == 0)
            {
                commissum.auctor = chorda_ex_buffer(
                    linea.datum + 7, linea.mensura - 7);
            }
            alioquin si (linea.mensura > 10
                && memcmp(linea.datum, "committer ", 10) == 0)
            {
                commissum.commissor = chorda_ex_buffer(
                    linea.datum + 10, linea.mensura - 10);
                /* tempus: verbum paenultimum (ante zonam) */
                {
                    i32 finis = commissum.commissor.mensura;
                    i32 j = finis;

                    dum (j > 0
                        && commissum.commissor.datum[j - 1] != ' ')
                    {
                        j = j - 1;
                    }
                    si (j > 1)
                    {
                        i32 k = j - 1;

                        dum (k > 0 && commissum.commissor
                            .datum[k - 1] != ' ')
                        {
                            k = k - 1;
                        }
                        commissum.tempus = (s64)strtol(
                            chorda_ut_cstr(chorda_ex_buffer(
                                commissum.commissor.datum + k,
                                (i32)(j - 1 - k)), piscina),
                            NIHIL, 10);
                    }
                }
            }
        }
    }
    commissum.successus = commissum.arbor.mensura == 40;
    redde commissum;
}

Xar*
git_arborem_legere (GitRepositorium* repositorium,
    constans character* sha, Piscina* piscina)
{
    GitObiectum obiectum = git_obiectum_legere(repositorium, sha,
        piscina);
    Xar*        introitus;
    i32         positus = 0;

    si (!obiectum.successus || obiectum.genus != GIT_OBIECTUM_ARBOR)
    {
        redde NIHIL;
    }
    introitus = xar_creare(piscina,
        (i32)magnitudo(GitArborIntroitus));
    si (introitus == NIHIL)
    {
        redde NIHIL;
    }
    dum (positus < obiectum.datum.mensura)
    {
        i32 initium_modi = positus;
        i32 initium_tituli;
        GitArborIntroitus* e;

        dum (positus < obiectum.datum.mensura
            && obiectum.datum.datum[positus] != ' ')
        {
            positus = positus + 1;
        }
        si (positus >= obiectum.datum.mensura)
        {
            redde NIHIL;   /* forma fracta */
        }
        e = (GitArborIntroitus*)xar_addere(introitus);
        si (e == NIHIL)
        {
            redde NIHIL;
        }
        e->modus = chorda_ex_buffer(
            obiectum.datum.datum + initium_modi,
            positus - initium_modi);
        positus = positus + 1;
        initium_tituli = positus;
        dum (positus < obiectum.datum.mensura
            && obiectum.datum.datum[positus] != 0)
        {
            positus = positus + 1;
        }
        si (positus + 21 > obiectum.datum.mensura)
        {
            redde NIHIL;
        }
        e->titulus = chorda_ex_buffer(
            obiectum.datum.datum + initium_tituli,
            positus - initium_tituli);
        positus = positus + 1;
        {
            character* hex = (character*)piscina_allocare(piscina,
                (memoriae_index)GIT_SHA_HEX_MENSURA);

            si (hex == NIHIL)
            {
                redde NIHIL;
            }
            _bin_ad_hex((constans i8*)obiectum.datum.datum
                + positus, 20, hex);
            e->sha = chorda_ex_buffer((i8*)hex, 40);
        }
        positus = positus + 20;
        e->est_arbor = chorda_aequalis_literis(e->modus, "40000");
    }
    redde introitus;
}

b32
git_sha_per_viam (GitRepositorium* repositorium,
    constans character* sha_commissi, constans character* via,
    Piscina* piscina, character* sha_exitus,
    b32* est_arbor_exitus)
{
    character sha[GIT_SHA_HEX_MENSURA];
    GitCommissum commissum;
    chorda    arbor_sha;
    constans character* reliqua = via;

    *est_arbor_exitus = FALSUM;
    memcpy(sha, sha_commissi, strlen(sha_commissi) + 1);
    si (!_ad_commissum_pellere(repositorium, sha, piscina))
    {
        redde FALSUM;
    }
    commissum = git_commissum_legere(repositorium, sha, piscina);
    si (!commissum.successus)
    {
        redde FALSUM;
    }
    arbor_sha = commissum.arbor;

    dum (*reliqua != '\0')
    {
        constans character* finis = reliqua;
        i32 segmentum_mensura;
        Xar* introitus;
        i32  i;
        b32  inventum_hic = FALSUM;
        chorda sequens_sha;
        b32  sequens_arbor = FALSUM;

        dum (*finis != '\0' && *finis != '/')
        {
            finis = finis + 1;
        }
        segmentum_mensura = (i32)(finis - reliqua);
        si (segmentum_mensura == 0)
        {
            reliqua = *finis == '/' ? finis + 1 : finis;
            perge;
        }
        introitus = git_arborem_legere(repositorium,
            chorda_ut_cstr(arbor_sha, piscina), piscina);
        si (introitus == NIHIL)
        {
            redde FALSUM;
        }
        sequens_sha.datum = NIHIL;
        sequens_sha.mensura = 0;
        per (i = 0; i < xar_numerus(introitus); i = i + 1)
        {
            GitArborIntroitus* e = (GitArborIntroitus*)
                xar_obtinere(introitus, i);

            si (e->titulus.mensura == segmentum_mensura
                && memcmp(e->titulus.datum, reliqua,
                       (memoriae_index)segmentum_mensura) == 0)
            {
                sequens_sha = e->sha;
                sequens_arbor = e->est_arbor;
                inventum_hic = VERUM;
                frange;
            }
        }
        si (!inventum_hic)
        {
            redde FALSUM;
        }
        si (*finis == '\0')
        {
            /* segmentum ultimum: sha introitus, sine contento */
            memcpy(sha_exitus, sequens_sha.datum,
                (memoriae_index)40);
            sha_exitus[40] = '\0';
            *est_arbor_exitus = sequens_arbor;
            redde VERUM;
        }
        si (!sequens_arbor)
        {
            redde FALSUM;
        }
        arbor_sha = sequens_sha;
        reliqua = finis + 1;
    }
    redde FALSUM;
}

chorda
git_massam_per_viam (GitRepositorium* repositorium,
    constans character* sha_commissi, constans character* via,
    Piscina* piscina, b32* inventum)
{
    chorda      vacua;
    character   sha[GIT_SHA_HEX_MENSURA];
    b32         est_arbor = FALSUM;
    GitObiectum obiectum;

    vacua.datum = NIHIL;
    vacua.mensura = 0;
    *inventum = FALSUM;
    si (!git_sha_per_viam(repositorium, sha_commissi, via,
            piscina, sha, &est_arbor)
        || est_arbor)
    {
        redde vacua;
    }
    obiectum = git_obiectum_legere(repositorium, sha, piscina);
    si (!obiectum.successus
        || obiectum.genus != GIT_OBIECTUM_MASSA)
    {
        redde vacua;
    }
    *inventum = VERUM;
    redde obiectum.datum;
}

/* ==================================================
 * Differentia arborum (screen sharum: subarbores
 * aequales numquam apertae)
 * ================================================= */

interior chorda
_viam_construere (Piscina* piscina, constans character* praefixum,
    chorda titulus);

interior chorda
_viam_construere (Piscina* piscina, constans character* praefixum,
    chorda titulus)
{
    i32    praefixi_mensura = (i32)strlen(praefixum);
    i32    tota = praefixi_mensura + titulus.mensura;
    i8*    datum;
    chorda via;

    via.datum = NIHIL;
    via.mensura = 0;
    datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)(tota == 0 ? 1 : tota));
    si (datum == NIHIL)
    {
        redde via;
    }
    memcpy(datum, praefixum, (memoriae_index)praefixi_mensura);
    memcpy(datum + praefixi_mensura, titulus.datum,
        (memoriae_index)titulus.mensura);
    via.datum = datum;
    via.mensura = tota;
    redde via;
}

interior constans character*
_praefixum_filii (Piscina* piscina, constans character* praefixum,
    chorda titulus);

interior constans character*
_praefixum_filii (Piscina* piscina, constans character* praefixum,
    chorda titulus)
{
    i32        praefixi_mensura = (i32)strlen(praefixum);
    character* exitus = (character*)piscina_allocare(piscina,
        (memoriae_index)(praefixi_mensura + titulus.mensura + 2));

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(exitus, praefixum, (memoriae_index)praefixi_mensura);
    memcpy(exitus + praefixi_mensura, titulus.datum,
        (memoriae_index)titulus.mensura);
    exitus[praefixi_mensura + titulus.mensura] = '/';
    exitus[praefixi_mensura + titulus.mensura + 1] = '\0';
    redde exitus;
}

interior b32
_viam_mutatam_addere (Xar* exitus, Piscina* piscina,
    constans character* praefixum, chorda titulus,
    chorda sha_vetus, chorda sha_novus, GitViaGenus genus);

interior b32
_viam_mutatam_addere (Xar* exitus, Piscina* piscina,
    constans character* praefixum, chorda titulus,
    chorda sha_vetus, chorda sha_novus, GitViaGenus genus)
{
    GitViaMutata* m = (GitViaMutata*)xar_addere(exitus);

    si (m == NIHIL)
    {
        redde FALSUM;
    }
    m->via = _viam_construere(piscina, praefixum, titulus);
    si (m->via.datum == NIHIL)
    {
        redde FALSUM;
    }
    m->sha_vetus = sha_vetus;
    m->sha_novus = sha_novus;
    m->genus = genus;
    redde VERUM;
}

/* gradus unus recursionis: latus NIHIL = subarbor tota absens
 * (introitus omnes lateris alterius ADDITA aut REMOTA fiunt) */
interior b32
_arbores_differre_gradus (GitRepositorium* repositorium,
    constans character* praefixum, constans character* sha_vetus,
    constans character* sha_novus, Xar* exitus, Piscina* piscina);

interior b32
_arbores_differre_gradus (GitRepositorium* repositorium,
    constans character* praefixum, constans character* sha_vetus,
    constans character* sha_novus, Xar* exitus, Piscina* piscina)
{
    Xar*            vetus_introitus = NIHIL;
    Xar*            novus_introitus = NIHIL;
    TabulaDispersa* nomina_veterum;
    i32*            sumptae;
    i32             numerus_veterum;
    i32             numerus_novorum;
    chorda          vacua;
    i32             k;

    vacua.datum = NIHIL;
    vacua.mensura = 0;
    si (sha_vetus != NIHIL)
    {
        vetus_introitus = git_arborem_legere(repositorium,
            sha_vetus, piscina);
        si (vetus_introitus == NIHIL)
        {
            redde FALSUM;
        }
    }
    si (sha_novus != NIHIL)
    {
        novus_introitus = git_arborem_legere(repositorium,
            sha_novus, piscina);
        si (novus_introitus == NIHIL)
        {
            redde FALSUM;
        }
    }
    numerus_veterum = vetus_introitus != NIHIL
        ? xar_numerus(vetus_introitus) : 0;
    numerus_novorum = novus_introitus != NIHIL
        ? xar_numerus(novus_introitus) : 0;

    /* index nominum lateris veteris (nomina in arbore unica sunt);
     * valor = ORDINALIS in cella propria (Xar segmentatum -
     * arithmetica trans elementa vetita) */
    nomina_veterum = tabula_dispersa_creare_chorda(piscina, 32);
    sumptae = (i32*)piscina_allocare(piscina,
        (memoriae_index)(numerus_veterum == 0 ? 1
            : numerus_veterum) * magnitudo(i32));
    si (nomina_veterum == NIHIL || sumptae == NIHIL)
    {
        redde FALSUM;
    }
    per (k = 0; k < numerus_veterum; k = k + 1)
    {
        GitArborIntroitus* va = (GitArborIntroitus*)
            xar_obtinere(vetus_introitus, k);
        i32* cella = (i32*)piscina_allocare(piscina,
            (memoriae_index)magnitudo(i32));

        si (cella == NIHIL)
        {
            redde FALSUM;
        }
        *cella = k;
        sumptae[k] = 0;
        tabula_dispersa_inserere(nomina_veterum, va->titulus,
            (vacuum*)cella);
    }

    per (k = 0; k < numerus_novorum; k = k + 1)
    {
        GitArborIntroitus* na = (GitArborIntroitus*)
            xar_obtinere(novus_introitus, k);
        vacuum*            sedes = NIHIL;

        si (tabula_dispersa_invenire(nomina_veterum, na->titulus,
            &sedes))
        {
            GitArborIntroitus* va = (GitArborIntroitus*)
                xar_obtinere(vetus_introitus, *(i32*)sedes);

            sumptae[*(i32*)sedes] = 1;
            si (va->est_arbor && na->est_arbor)
            {
                si (!chorda_aequalis(va->sha, na->sha))
                {
                    constans character* filii = _praefixum_filii(
                        piscina, praefixum, na->titulus);

                    si (filii == NIHIL
                        || !_arbores_differre_gradus(repositorium,
                               filii,
                               chorda_ut_cstr(va->sha, piscina),
                               chorda_ut_cstr(na->sha, piscina),
                               exitus, piscina))
                    {
                        redde FALSUM;
                    }
                }
            }
            alioquin si (!va->est_arbor && !na->est_arbor)
            {
                si ((!chorda_aequalis(va->sha, na->sha)
                        || !chorda_aequalis(va->modus, na->modus))
                    && !_viam_mutatam_addere(exitus, piscina,
                           praefixum, na->titulus, va->sha,
                           na->sha, GIT_VIA_MUTATA))
                {
                    redde FALSUM;
                }
            }
            alioquin
            {
                /* conversio generis eodem nomine: latus vetus
                 * totum REMOTA, novum totum ADDITA */
                constans character* filii = _praefixum_filii(
                    piscina, praefixum, na->titulus);

                si (filii == NIHIL)
                {
                    redde FALSUM;
                }
                si (va->est_arbor
                    ? !_arbores_differre_gradus(repositorium,
                          filii, chorda_ut_cstr(va->sha, piscina),
                          NIHIL, exitus, piscina)
                    : !_viam_mutatam_addere(exitus, piscina,
                          praefixum, va->titulus, va->sha, vacua,
                          GIT_VIA_REMOTA))
                {
                    redde FALSUM;
                }
                si (na->est_arbor
                    ? !_arbores_differre_gradus(repositorium,
                          filii, NIHIL,
                          chorda_ut_cstr(na->sha, piscina),
                          exitus, piscina)
                    : !_viam_mutatam_addere(exitus, piscina,
                          praefixum, na->titulus, vacua, na->sha,
                          GIT_VIA_ADDITA))
                {
                    redde FALSUM;
                }
            }
        }
        alioquin si (na->est_arbor)
        {
            constans character* filii = _praefixum_filii(piscina,
                praefixum, na->titulus);

            si (filii == NIHIL
                || !_arbores_differre_gradus(repositorium, filii,
                       NIHIL, chorda_ut_cstr(na->sha, piscina),
                       exitus, piscina))
            {
                redde FALSUM;
            }
        }
        alioquin si (!_viam_mutatam_addere(exitus, piscina,
            praefixum, na->titulus, vacua, na->sha,
            GIT_VIA_ADDITA))
        {
            redde FALSUM;
        }
    }

    per (k = 0; k < numerus_veterum; k = k + 1)
    {
        GitArborIntroitus* va;

        si (sumptae[k])
        {
            perge;
        }
        va = (GitArborIntroitus*)xar_obtinere(vetus_introitus, k);
        si (va->est_arbor)
        {
            constans character* filii = _praefixum_filii(piscina,
                praefixum, va->titulus);

            si (filii == NIHIL
                || !_arbores_differre_gradus(repositorium, filii,
                       chorda_ut_cstr(va->sha, piscina), NIHIL,
                       exitus, piscina))
            {
                redde FALSUM;
            }
        }
        alioquin si (!_viam_mutatam_addere(exitus, piscina,
            praefixum, va->titulus, va->sha, vacua,
            GIT_VIA_REMOTA))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior s32
_viae_mutatae_comparare (constans vacuum* a, constans vacuum* b);

interior s32
_viae_mutatae_comparare (constans vacuum* a, constans vacuum* b)
{
    redde chorda_comparare(
        ((constans GitViaMutata*)a)->via,
        ((constans GitViaMutata*)b)->via);
}

Xar*
git_arbores_differre (GitRepositorium* repositorium,
    constans character* sha_arboris_vetus,
    constans character* sha_arboris_novum, Piscina* piscina)
{
    Xar* exitus = xar_creare(piscina,
        (i32)magnitudo(GitViaMutata));

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    si (strcmp(sha_arboris_vetus, sha_arboris_novum) == 0)
    {
        redde exitus;   /* arbores eaedem - differentia nulla */
    }
    si (!_arbores_differre_gradus(repositorium, "",
        sha_arboris_vetus, sha_arboris_novum, exitus, piscina))
    {
        redde NIHIL;
    }
    xar_ordinare(exitus, _viae_mutatae_comparare);
    redde exitus;
}
