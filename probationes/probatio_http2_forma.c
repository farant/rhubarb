/* probatio_http2_forma.c - formae HTTP/2 (RFC 9113 4, 6; http2-plan
 * T4): rotundum omnium generum, impletio, iudicia validationis, partes
 * (octetus post octetum), positiones (SETTINGS). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "http2_forma.h"
#include "credo.h"
#include <string.h>

#define FORMA_ORDINARIA  (MMXLVIII * VIII)   /* 16384, 4.2 */

hic_manens Piscina* piscina;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina = c->piscina;
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina = NIHIL;
}

/* formam scribe, deinde lege: genus, vexilla, rivus, contentum idem */
interior b32
rotundum (
             i32  genus,
             i32  vexilla,
             i32  rivus,
    constans i8* contentum,
             i32  n)
{
               i8 b[CCLVI];
              i32 m;
              i32 k;
       Http2Forma f;
    Http2Iudicium j;

    m = http2_formam_scribere(genus, vexilla, rivus, contentum, n, b,
        CCLVI);
    si (m != HTTP2_CAPUT_FORMAE + n)
    {
        redde FALSUM;
    }
    k = http2_formam_legere(b, m, FORMA_ORDINARIA, &f, &j);
    redde k == m && j.bona && f.genus == genus && f.vexilla == vexilla
        && f.rivus == rivus && f.contentum_n == n
        && (n == ZEPHYRUM
            || memcmp(f.contentum, contentum, (size_t)n) == ZEPHYRUM);
}

/* caput formae crudum: longitudo, genus, vexilla, rivus (cum bito
 * reservato) */
interior i32
caput_scribere (
    i32  longitudo,
    i32  genus,
    i32  vexilla,
    i32  rivus,
     i8* b)
{
    b[ZEPHYRUM]  = (i8)((longitudo >> XVI) & 0xff);
    b[I]         = (i8)((longitudo >> VIII) & 0xff);
    b[II]        = (i8)(longitudo & 0xff);
    b[III]       = (i8)genus;
    b[IV]        = (i8)vexilla;
    b[V]         = (i8)((rivus >> XXIV) & 0xff);
    b[VI]        = (i8)((rivus >> XVI) & 0xff);
    b[VII]       = (i8)((rivus >> VIII) & 0xff);
    b[VIII]      = (i8)(rivus & 0xff);
    redde HTTP2_CAPUT_FORMAE;
}

interior Http2Iudicium
iudicare (
    constans i8* b,
            i32  n,
           i32* consumpti)
{
       Http2Forma f;
    Http2Iudicium j;

    *consumpti = http2_formam_legere(b, n, FORMA_ORDINARIA, &f, &j);
    redde j;
}

interior vacuum
_sectio_rotundum (
    CredoContextus* c)
{
     i8 octo[VIII];
     i8 q[VIII];
     i8 b[XXXII];
    i32 i;
    i32 k;

    (vacuum)c;
    CREDO_GRADUS("omnia genera");
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        octo[i] = (i8)(i + I);
    }
    CREDO_VERUM(rotundum(HTTP2_DATA, HTTP2_FINIS_RIVI, I, octo, V));
    CREDO_VERUM(rotundum(HTTP2_CAPITA, HTTP2_FINIS_CAPITUM, III, octo,
        III));
    CREDO_VERUM(rotundum(HTTP2_PRIORITAS, ZEPHYRUM, V, octo, V));
    CREDO_VERUM(rotundum(HTTP2_ABROGATIO, ZEPHYRUM, VII, octo, IV));
    CREDO_VERUM(rotundum(HTTP2_POSITIONES, ZEPHYRUM, ZEPHYRUM, octo,
        VI));
    CREDO_VERUM(rotundum(HTTP2_POSITIONES, HTTP2_RECEPTUM, ZEPHYRUM,
        octo, ZEPHYRUM));
    CREDO_VERUM(rotundum(HTTP2_PROMISSIO, HTTP2_FINIS_CAPITUM, I, octo,
        VI));
    CREDO_VERUM(rotundum(HTTP2_PULSUS, ZEPHYRUM, ZEPHYRUM, octo, VIII));
    CREDO_VERUM(rotundum(HTTP2_DISCESSUS, ZEPHYRUM, ZEPHYRUM, octo,
        VIII));
    CREDO_VERUM(rotundum(HTTP2_FENESTRA, ZEPHYRUM, IX, octo, IV));
    CREDO_VERUM(rotundum(HTTP2_CONTINUATIO, HTTP2_FINIS_CAPITUM, XI,
        octo, II));

    CREDO_GRADUS("bitum reservatum: neglectum lectione, purgatum scriptione");
    (vacuum)caput_scribere(ZEPHYRUM, HTTP2_POSITIONES, HTTP2_RECEPTUM,
        0x80000000, b);
    {
           Http2Forma f;
        Http2Iudicium j;

        CREDO_AEQUALIS_I32(http2_formam_legere(b, HTTP2_CAPUT_FORMAE,
            FORMA_ORDINARIA, &f, &j), HTTP2_CAPUT_FORMAE);
        CREDO_VERUM(j.bona);
        CREDO_AEQUALIS_I32(f.rivus, ZEPHYRUM);
    }
    k = http2_formam_scribere(HTTP2_FENESTRA, ZEPHYRUM, 0x80000005,
        octo,
        IV, b, XXXII);
    CREDO_AEQUALIS_I32(k, XIII);
    CREDO_AEQUALIS_I32((i32)b[V], ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)b[VIII], V);

    CREDO_GRADUS("praefatio et capacitas");
    CREDO_AEQUALIS_I32((i32)strlen(HTTP2_PRAEFATIO),
        HTTP2_PRAEFATIO_MENSURA);
    CREDO_AEQUALIS_I32(http2_formam_scribere(HTTP2_PULSUS, ZEPHYRUM,
        ZEPHYRUM, octo, VIII, q, VIII), ZEPHYRUM);
}

interior vacuum
_sectio_impletum (
    CredoContextus* c)
{
               i8 b[LXIV];
              i32 k;
       Http2Forma f;
    Http2Iudicium j;

    (vacuum)c;
    CREDO_GRADUS("DATA impletum: contentum sine impletione");
    (vacuum)caput_scribere(VIII, HTTP2_DATA, HTTP2_IMPLETUM, I, b);
    b[IX]    = (i8)III;                       /* impletio III */
    b[X]     = 'a';
    b[XI]    = 'b';
    b[XII]   = 'c';
    b[XIII]  = 'd';
    b[XIV]   = b[XV] = b[XVI] = ZEPHYRUM;
    k        = http2_formam_legere(b, XVII, FORMA_ORDINARIA, &f, &j);
    CREDO_AEQUALIS_I32(k, XVII);
    CREDO_VERUM(j.bona);
    CREDO_AEQUALIS_I32(f.contentum_n, IV);
    CREDO_VERUM(memcmp(f.contentum, "abcd", IV) == ZEPHYRUM);

    CREDO_GRADUS("HEADERS impletum et cum prioritate");
    (vacuum)caput_scribere(X, HTTP2_CAPITA,
        HTTP2_IMPLETUM | HTTP2_CUM_PRIORITATE | HTTP2_FINIS_CAPITUM,
        III,
        b);
    b[IX]    = (i8)II;                          /* impletio II */
    b[X]     = b[XI] = b[XII] = b[XIII] = ZEPHYRUM;  /* dependentia */
    b[XIV]   = (i8)XVI;                        /* pondus */
    b[XV]    = (i8)0x82;
    b[XVI]   = (i8)0x84;
    b[XVII]  = b[XVIII] = ZEPHYRUM;
    k        = http2_formam_legere(b, XIX, FORMA_ORDINARIA, &f, &j);
    CREDO_AEQUALIS_I32(k, XIX);
    CREDO_VERUM(j.bona);
    CREDO_AEQUALIS_I32(f.contentum_n, II);
    CREDO_AEQUALIS_I32((i32)f.contentum[ZEPHYRUM], 0x82);

    CREDO_GRADUS("impletio >= onus: PROTOCOL_ERROR connexionis");
    (vacuum)caput_scribere(III, HTTP2_DATA, HTTP2_IMPLETUM, I, b);
    b[IX]  = (i8)III;
    b[X]   = b[XI] = ZEPHYRUM;
    j      = iudicare(b, XII, &k);
    CREDO_FALSUM(j.bona);
    CREDO_AEQUALIS_I32(j.codex, HTTP2_ERROR_PROTOCOLLI);
    CREDO_VERUM(j.connexionis);
    CREDO_AEQUALIS_I32(k, XII);
    (vacuum)caput_scribere(ZEPHYRUM, HTTP2_DATA, HTTP2_IMPLETUM, I, b);
    j = iudicare(b, IX, &k);
    CREDO_FALSUM(j.bona);
    CREDO_AEQUALIS_I32(j.codex, HTTP2_ERROR_MAGNITUDO);
}

/* iudicium formae (longitudo, genus, vexilla, rivus, contentum
 * zephyris aut 'onus') */
interior Http2Iudicium
iudicium_formae (
    i32 longitudo,
    i32 genus,
    i32 vexilla,
    i32 rivus,
    i32 onus)
{
     i8 b[LXIV];
    i32 k;
    i32 i;

    (vacuum)caput_scribere(longitudo, genus, vexilla, rivus, b);
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        b[HTTP2_CAPUT_FORMAE + i] = (i8)((onus >> (VIII * ((III - i)
            & III))) & 0xff);
    }
    redde iudicare(b, HTTP2_CAPUT_FORMAE + longitudo, &k);
}

interior b32
iudicium_est (
    Http2Iudicium j,
              i32 codex,
              b32 connexionis)
{
    redde !j.bona && j.codex == codex && j.connexionis == connexionis;
}

interior vacuum
_sectio_iudicia (
    CredoContextus* c)
{
               i8 b[XVI];
              i32 k;
    Http2Iudicium j;
       Http2Forma f;

    (vacuum)c;
    CREDO_GRADUS("longitudines fixae: FRAME_SIZE_ERROR");
    CREDO_VERUM(iudicium_est(iudicium_formae(VII, HTTP2_PULSUS,
        ZEPHYRUM,
        ZEPHYRUM, ZEPHYRUM), HTTP2_ERROR_MAGNITUDO, VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(V, HTTP2_FENESTRA,
        ZEPHYRUM,
        I, I), HTTP2_ERROR_MAGNITUDO, VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(III, HTTP2_ABROGATIO,
        ZEPHYRUM, I, ZEPHYRUM), HTTP2_ERROR_MAGNITUDO, VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(VII, HTTP2_POSITIONES,
        ZEPHYRUM, ZEPHYRUM, ZEPHYRUM), HTTP2_ERROR_MAGNITUDO, VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(VI, HTTP2_POSITIONES,
        HTTP2_RECEPTUM, ZEPHYRUM, ZEPHYRUM), HTTP2_ERROR_MAGNITUDO,
        VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(IV, HTTP2_PRIORITAS,
        ZEPHYRUM, I, ZEPHYRUM), HTTP2_ERROR_MAGNITUDO, FALSUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(VII, HTTP2_DISCESSUS,
        ZEPHYRUM, ZEPHYRUM, ZEPHYRUM), HTTP2_ERROR_MAGNITUDO, VERUM));

    CREDO_GRADUS("rivus 0 aut non 0: PROTOCOL_ERROR");
    CREDO_VERUM(iudicium_est(iudicium_formae(ZEPHYRUM, HTTP2_POSITIONES,
        ZEPHYRUM, I, ZEPHYRUM), HTTP2_ERROR_PROTOCOLLI, VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(VIII, HTTP2_PULSUS,
        ZEPHYRUM, I, ZEPHYRUM), HTTP2_ERROR_PROTOCOLLI, VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(VIII, HTTP2_DISCESSUS,
        ZEPHYRUM, III, ZEPHYRUM), HTTP2_ERROR_PROTOCOLLI, VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(I, HTTP2_DATA, ZEPHYRUM,
        ZEPHYRUM, ZEPHYRUM), HTTP2_ERROR_PROTOCOLLI, VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(I, HTTP2_CAPITA,
        HTTP2_FINIS_CAPITUM, ZEPHYRUM, ZEPHYRUM),
        HTTP2_ERROR_PROTOCOLLI,
        VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(IV, HTTP2_ABROGATIO,
        ZEPHYRUM, ZEPHYRUM, ZEPHYRUM), HTTP2_ERROR_PROTOCOLLI, VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(I, HTTP2_CONTINUATIO,
        ZEPHYRUM, ZEPHYRUM, ZEPHYRUM), HTTP2_ERROR_PROTOCOLLI, VERUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(V, HTTP2_PRIORITAS,
        ZEPHYRUM, ZEPHYRUM, ZEPHYRUM), HTTP2_ERROR_PROTOCOLLI, VERUM));

    CREDO_GRADUS("WINDOW_UPDATE incrementum 0");
    CREDO_VERUM(iudicium_est(iudicium_formae(IV, HTTP2_FENESTRA,
        ZEPHYRUM, V, ZEPHYRUM), HTTP2_ERROR_PROTOCOLLI, FALSUM));
    CREDO_VERUM(iudicium_est(iudicium_formae(IV, HTTP2_FENESTRA,
        ZEPHYRUM, ZEPHYRUM, ZEPHYRUM), HTTP2_ERROR_PROTOCOLLI, VERUM));
    j = iudicium_formae(IV, HTTP2_FENESTRA, ZEPHYRUM, V, C);
    CREDO_VERUM(j.bona);

    CREDO_GRADUS("forma supra maximam: FRAME_SIZE_ERROR connexionis");
    (vacuum)caput_scribere(FORMA_ORDINARIA + I, HTTP2_DATA, ZEPHYRUM, I,
        b);
    k = http2_formam_legere(b, HTTP2_CAPUT_FORMAE, FORMA_ORDINARIA, &f,
        &j);
    CREDO_AEQUALIS_I32(k, HTTP2_CAPUT_FORMAE);
    CREDO_VERUM(iudicium_est(j, HTTP2_ERROR_MAGNITUDO, VERUM));

    CREDO_GRADUS("genus ignotum 0x42: praeteritum");
    j = iudicium_formae(III, 0x42, 0xff, VII, ZEPHYRUM);
    CREDO_VERUM(j.bona);
}

interior vacuum
_sectio_positiones (
    CredoContextus* c)
{
               i8 b[LXIV];
               i8 p[XVIII];
              i32 k;
              i32 id;
              i32 valor;
       Http2Forma f;
    Http2Iudicium j;

    (vacuum)c;
    CREDO_GRADUS("scribere et legere");
    k  = http2_positionem_scribere(HTTP2_POSITIO_IMPULSUS, ZEPHYRUM, p);
    k += http2_positionem_scribere(HTTP2_POSITIO_FENESTRA, 0x100000,
        p + k);
    k += http2_positionem_scribere(HTTP2_POSITIO_FORMA, FORMA_ORDINARIA,
        p + k);
    CREDO_AEQUALIS_I32(k, XVIII);
    k = http2_formam_scribere(HTTP2_POSITIONES, ZEPHYRUM, ZEPHYRUM, p,
        XVIII, b, LXIV);
    CREDO_AEQUALIS_I32(http2_formam_legere(b, k, FORMA_ORDINARIA, &f,
        &j),
        k);
    CREDO_VERUM(j.bona);
    CREDO_VERUM(http2_positionem_legere(&f, I, &id, &valor));
    CREDO_AEQUALIS_I32(id, HTTP2_POSITIO_FENESTRA);
    CREDO_AEQUALIS_I32(valor, 0x100000);
    CREDO_FALSUM(http2_positionem_legere(&f, III, &id, &valor));
    CREDO_AEQUALIS_I32(http2_u32_legere(p + II), ZEPHYRUM);

    CREDO_GRADUS("valores mali (6.5.2)");
    (vacuum)http2_positionem_scribere(HTTP2_POSITIO_IMPULSUS, II, p);
    k = http2_formam_scribere(HTTP2_POSITIONES, ZEPHYRUM, ZEPHYRUM, p,
        VI,
        b, LXIV);
    j = iudicare(b, k, &k);
    CREDO_VERUM(iudicium_est(j, HTTP2_ERROR_PROTOCOLLI, VERUM));
    (vacuum)http2_positionem_scribere(HTTP2_POSITIO_FENESTRA,
        0x80000000,
        p);
    k = http2_formam_scribere(HTTP2_POSITIONES, ZEPHYRUM, ZEPHYRUM, p,
        VI,
        b, LXIV);
    j = iudicare(b, k, &k);
    CREDO_VERUM(iudicium_est(j, HTTP2_ERROR_FLUXUS, VERUM));
    (vacuum)http2_positionem_scribere(HTTP2_POSITIO_FORMA, 0x3fff, p);
    k = http2_formam_scribere(HTTP2_POSITIONES, ZEPHYRUM, ZEPHYRUM, p,
        VI,
        b, LXIV);
    j = iudicare(b, k, &k);
    CREDO_VERUM(iudicium_est(j, HTTP2_ERROR_PROTOCOLLI, VERUM));
    (vacuum)http2_positionem_scribere(0x99, 0xffffffff, p);
    k = http2_formam_scribere(HTTP2_POSITIONES, ZEPHYRUM, ZEPHYRUM, p,
        VI,
        b, LXIV);
    j = iudicare(b, k, &k);
    CREDO_VERUM(j.bona);
}

interior vacuum
_sectio_partes (
    CredoContextus* c)
{
               i8 octo[VIII];
               i8 b[XXXII];
              i32 m;
              i32 i;
              i32 k;
       Http2Forma f;
    Http2Iudicium j;

    (vacuum)c;
    CREDO_GRADUS("octetus post octetum: 0 donec plena");
    memset(octo, 0x5a, VIII);
    m = http2_formam_scribere(HTTP2_PULSUS, ZEPHYRUM, ZEPHYRUM, octo,
        VIII, b, XXXII);
    per (i = ZEPHYRUM; i < m; i++)
    {
        CREDO_AEQUALIS_I32(http2_formam_legere(b, i, FORMA_ORDINARIA,
            &f,
            &j), ZEPHYRUM);
    }
    k = http2_formam_legere(b, m, FORMA_ORDINARIA, &f, &j);
    CREDO_AEQUALIS_I32(k, m);
    CREDO_VERUM(j.bona);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "rotundum", _sectio_rotundum, _parare, _purgare, NIHIL, NIHIL },
    { "impletio", _sectio_impletum, _parare, _purgare, NIHIL, NIHIL },
    { "iudicia (6)", _sectio_iudicia, _parare, _purgare, NIHIL, NIHIL },
    { "positiones (6.5)", _sectio_positiones, _parare, _purgare, NIHIL,
        NIHIL },
    { "partes", _sectio_partes, _parare, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("http2_forma", SECTIONES);
}
