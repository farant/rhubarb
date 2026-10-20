/* argon2id.c - Argon2id secundum RFC 9106 (versio 0x13, y = 2)
 *
 * Memoria: m' = 4p * floor(m / 4p) caudices, lanae p, columnae q =
 * m'/p, segmentum SL = q/4 (scissurae IV). Ordo computandi (transitus,
 * scissura, lana, index): lanae seriatim - permissum quia intra
 * scissuram lana nulla caudices scissurae currentis lanae ALIENAE
 * legit (regulae areae relationis), ergo ordo idem ac paralleli.
 *
 * Inscriptio: Argon2i (caudex inscriptionum ex G(0, G(0, Z))) in
 * transitu 0 scissuris 0-1; postea ex caudice priore (Argon2d).
 * Opus statum (transitus, scissura, lana, index, caudex inscriptionum)
 * servat, ergo gradus quilibet ubivis resumit. */
#include "argon2id.h"
#include "blake2b.h"
#include "base64.h"

#include <stdio.h>
#include <string.h>

#define VERBA_CAUDICIS    CXXVIII     /* 1024 octeti */
#define OCTETI_CAUDICIS   MXXIV
#define SCISSURAE         IV
#define TYPUS_ID          II

structura Argon2idOpus {
    Argon2idParametri  par;
                  i64* memoria;     /* m' x CXXVIII verba */
                  i32  caudices;    /* m' */
                  i32  q;           /* columnae per lanam */
                  i32  sl;          /* longitudo segmenti */

    /* status resumendi */
    i32 transitus;
    i32 scissura;
    i32 lana;
    i32 index;           /* intra segmentum */
    b32 finitum;

    /* inscriptio data-independens (Argon2i) */
    i64 ingressus[VERBA_CAUDICIS];
    i64 inscriptiones[VERBA_CAUDICIS];
};


/* ==================================================
 * AUXILIA
 * ================================================== */

interior vacuum
_quaterni_scribere (
     i8* o,
    i32  x)
{
    o[0] = (i8)(x & 0xFF);
    o[1] = (i8)((x >> VIII) & 0xFF);
    o[2] = (i8)((x >> XVI) & 0xFF);
    o[3] = (i8)((x >> XXIV) & 0xFF);
}

interior vacuum
_quaterni_addere (
    Blake2b* b,
        i32  x)
{
    i8 o[IV];

    _quaterni_scribere(o, x);
    blake2b_addere(b, o, IV);
}

/* H'^T (RFC 9106 par. 3.3): hash longitudinis variabilis */
interior vacuum
_digestum_varium (
             i8* exitus,
            i32  t,
    constans i8* a,
            i32  an,
    constans i8* b,
            i32  bn)
{
    Blake2b s;
         i8 v[LXIV];

    si (t <= LXIV)
    {
        (vacuum)blake2b_incipere(&s, t, NIHIL, 0);
        _quaterni_addere(&s, t);
        blake2b_addere(&s, a, an);
        blake2b_addere(&s, b, bn);
        blake2b_finire(&s, exitus);
        redde;
    }
    {
        i32 r = (t + XXXI) / XXXII - II;
        i32 i;

        (vacuum)blake2b_incipere(&s, LXIV, NIHIL, 0);
        _quaterni_addere(&s, t);
        blake2b_addere(&s, a, an);
        blake2b_addere(&s, b, bn);
        blake2b_finire(&s, v);
        memcpy(exitus, v, XXXII);
        per (i = I; i < r; i++)
        {
            blake2b(v, LXIV, v, LXIV, NIHIL, 0);
            memcpy(exitus + i * XXXII, v, XXXII);
        }
        blake2b(exitus + r * XXXII, t - r * XXXII, v, LXIV, NIHIL, 0);
    }
}

interior i64
_octoni_legere (
    constans i8* p)
{
    redde (i64)p[0] | ((i64)p[1] << VIII) | ((i64)p[2] << XVI)
        | ((i64)p[3] << XXIV) | ((i64)p[4] << XXXII)
        | ((i64)p[5] << XL) | ((i64)p[6] << XLVIII)
        | ((i64)p[7] << LVI);
}

interior vacuum
_caudex_ex_octetis (
            i64* c,
    constans i8* o)
{
    i32 i;

    per (i = 0; i < VERBA_CAUDICIS; i++)
    {
        c[i] = _octoni_legere(o + VIII * i);
    }
}

interior vacuum
_caudex_ad_octetos (
              i8* o,
    constans i64* c)
{
    i32 i;
    i32 k;

    per (i = 0; i < VERBA_CAUDICIS; i++)
    {
        per (k = 0; k < VIII; k++)
        {
            o[VIII * i + k] = (i8)((c[i] >> (VIII * k)) & 0xFF);
        }
    }
}


/* ==================================================
 * COMPRESSIO G (BlaMka)
 * ================================================== */

#define CIRCUMVOLVERE(x, n) (((x) >> (n)) | ((x) << (LXIV - (n))))

/* BlaMka: x + y + 2 * infima(x) * infima(y) */
interior i64
_blamka (
    i64 x,
    i64 y)
{
    i64 infima_x = x & 0xFFFFFFFFULL;
    i64 infima_y = y & 0xFFFFFFFFULL;

    redde x + y + II * infima_x * infima_y;
}

#define GB(a, b, c, d)                     \
    fac {                                  \
        a = _blamka(a, b);                 \
        d = CIRCUMVOLVERE(d ^ a, XXXII);   \
        c = _blamka(c, d);                 \
        b = CIRCUMVOLVERE(b ^ c, XXIV);    \
        a = _blamka(a, b);                 \
        d = CIRCUMVOLVERE(d ^ a, XVI);     \
        c = _blamka(c, d);                 \
        b = CIRCUMVOLVERE(b ^ c, LXIII);   \
    } dum (0)

/* P super XVI verba (indices in v) */
interior vacuum
_permutare (
    i64* v,
    i32  i0, i32 i1, i32 i2, i32 i3, i32 i4, i32 i5, i32 i6, i32 i7,
    i32  i8_, i32 i9, i32 i10, i32 i11, i32 i12, i32 i13, i32 i14,
    i32  i15)
{
    GB(v[i0], v[i4], v[i8_], v[i12]);
    GB(v[i1], v[i5], v[i9], v[i13]);
    GB(v[i2], v[i6], v[i10], v[i14]);
    GB(v[i3], v[i7], v[i11], v[i15]);
    GB(v[i0], v[i5], v[i10], v[i15]);
    GB(v[i1], v[i6], v[i11], v[i12]);
    GB(v[i2], v[i7], v[i8_], v[i13]);
    GB(v[i3], v[i4], v[i9], v[i14]);
}

/* novus = G(prior, relatus) [XOR novus vetus si cum_xor] */
interior vacuum
_implere (
    constans i64* prior,
    constans i64* relatus,
             i64* novus,
             b32  cum_xor)
{
    i64 r[VERBA_CAUDICIS];
    i64 t[VERBA_CAUDICIS];
    i32 i;

    per (i = 0; i < VERBA_CAUDICIS; i++)
    {
        r[i] = prior[i] ^ relatus[i];
        t[i] = r[i];
        si (cum_xor)
        {
            t[i] ^= novus[i];
        }
    }
    /* ordines: registra XVI octetorum = paria verborum */
    per (i = 0; i < VIII; i++)
    {
        i32 b = XVI * i;

        _permutare(r, b, b + I, b + II, b + III, b + IV, b + V, b + VI,
            b + VII, b + VIII, b + IX, b + X, b + XI, b + XII, b + XIII,
            b + XIV, b + XV);
    }
    /* columnae */
    per (i = 0; i < VIII; i++)
    {
        i32 b = II * i;

        _permutare(r, b, b + I, b + XVI, b + XVII, b + XXXII,
            b + XXXIII, b + XLVIII, b + XLIX, b + LXIV, b + LXV,
            b + LXXX, b + LXXXI, b + XCVI, b + XCVII, b + CXII,
            b + CXIII);
    }
    per (i = 0; i < VERBA_CAUDICIS; i++)
    {
        novus[i] = t[i] ^ r[i];
    }
}

/* caudex inscriptionum proximus: ingressus.v[6]++, G(0, G(0, Z)) */
interior vacuum
_inscriptiones_novare (
    Argon2idOpus* o)
{
    i64 nihil[VERBA_CAUDICIS];

    memset(nihil, 0, magnitudo(nihil));
    o->ingressus[VI]++;
    _implere(nihil, o->ingressus, o->inscriptiones, FALSUM);
    _implere(nihil, o->inscriptiones, o->inscriptiones, FALSUM);
}

interior i64*
_caudex (
    Argon2idOpus* o,
             i32  lana,
             i32  columna)
{
    redde o->memoria + ((memoriae_index)lana * o->q + columna)
        * VERBA_CAUDICIS;
}


/* ==================================================
 * CAUDEX UNUS
 * ================================================== */

/* index caudicis relati intra lanam (RFC 9106 par. 3.4.1.2) */
interior i32
_index_alpha (
    Argon2idOpus* o,
             i32  index,
             i64  j1,
             b32  eadem_lana)
{
    i64 area;
    i64 x;
    i64 initium  = 0;
    i32 sl       = o->sl;

    si (o->transitus == 0)
    {
        si (o->scissura == 0)
        {
            area = (i64)index - I;
        }
        alioquin si (eadem_lana)
        {
            area = (i64)o->scissura * sl + index - I;
        }
        alioquin
        {
            area = (i64)o->scissura * sl - (index == 0 ? I : 0);
        }
    }
    alioquin
    {
        si (eadem_lana)
        {
            area = (i64)o->q - sl + index - I;
        }
        alioquin
        {
            area = (i64)o->q - sl - (index == 0 ? I : 0);
        }
        initium = (o->scissura == SCISSURAE - I)
            ? 0 : (i64)(o->scissura + I) * sl;
    }
    x = (j1 * j1) >> XXXII;
    x = area - I - ((area * x) >> XXXII);
    redde (i32)((initium + x) % (i64)o->q);
}

interior b32
_independens (
    Argon2idOpus* o)
{
    redde o->transitus == 0 && o->scissura < II;
}

/* caudicem (transitus, scissura, lana, index) implere; statum
 * promovere */
interior vacuum
_caudicem_implere (
    Argon2idOpus* o)
{
    i32 columna  = o->scissura * o->sl + o->index;
    i32 prior    = columna == 0 ? o->q - I : columna - I;
    i32 initium  = (o->transitus == 0 && o->scissura == 0) ? II : 0;
    i64 pseudo;
    i32 lana_rel;
    i32 index_rel;

    si (_independens(o))
    {
        si (o->index == initium || o->index % VERBA_CAUDICIS == 0)
        {
            si (o->index == initium)
            {
                memset(o->ingressus, 0, magnitudo(o->ingressus));
                o->ingressus[0]    = (i64)o->transitus;
                o->ingressus[I]    = (i64)o->lana;
                o->ingressus[II]   = (i64)o->scissura;
                o->ingressus[III]  = (i64)o->caudices;
                o->ingressus[IV]   = (i64)o->par.t_cost;
                o->ingressus[V]    = TYPUS_ID;
            }
            _inscriptiones_novare(o);
        }
        pseudo = o->inscriptiones[o->index % VERBA_CAUDICIS];
    }
    alioquin
    {
        pseudo = _caudex(o, o->lana, prior)[0];
    }

    lana_rel = (i32)((pseudo >> XXXII) % (i64)o->par.lanae);
    si (o->transitus == 0 && o->scissura == 0)
    {
        lana_rel = o->lana;
    }
    index_rel = _index_alpha(o, o->index, pseudo & 0xFFFFFFFFULL,
        lana_rel == o->lana);
    _implere(_caudex(o, o->lana, prior), _caudex(o, lana_rel,
        index_rel),
        _caudex(o, o->lana, columna), o->transitus > 0);

    /* promovere: index -> lana -> scissura -> transitus */
    o->index++;
    si (o->index < o->sl)
    {
        redde;
    }
    o->lana++;
    o->index = (o->transitus == 0 && o->scissura == 0) ? II : 0;
    si (o->lana < o->par.lanae)
    {
        redde;
    }
    o->lana = 0;
    o->scissura++;
    o->index = 0;
    si (o->scissura < SCISSURAE)
    {
        redde;
    }
    o->scissura = 0;
    o->transitus++;
    si (o->transitus >= o->par.t_cost)
    {
        o->finitum = VERUM;
    }
}


/* ==================================================
 * FUNCTIONES PUBLICAE
 * ================================================== */

interior b32
_parametri_validi (
    constans Argon2idParametri* p,
                           i32  sn)
{
    redde p && p->t_cost >= I && p->lanae >= I
        && p->lanae <= 0xFFFFFF
        && p->m_cost_kib >= VIII * p->lanae
        && p->longitudo >= IV
        && p->longitudo <= ARGON2ID_DIGESTUM_MAXIMUM
        && sn >= VIII && sn <= ARGON2ID_SAL_MAXIMUS
        && (p->secretum_n == 0 || p->secretum)
        && (p->ad_n == 0 || p->ad);
}

Argon2idOpus*
argon2id_incipere (
    constans Argon2idParametri* parametri,
                   constans i8* tessera,
                           i32  tn,
                   constans i8* sal,
                           i32  sn,
                       Piscina* piscina)
{
    Argon2idOpus* o;
         Blake2b  b;
              /* H0 || LE32(columna) || LE32(lana) */
              i8 h0[LXXII];
              i8 caudex[OCTETI_CAUDICIS];
             i32 m;
             i32 l;

    si (   !piscina || !_parametri_validi(parametri, sn) || !sal
        || (tn > 0 && !tessera))
    {
        redde NIHIL;
    }
    o = (Argon2idOpus*)piscina_allocare(piscina,
        (i64)magnitudo(Argon2idOpus));
    si (!o)
    {
        redde NIHIL;
    }
    memset(o, 0, magnitudo(*o));
    o->par       = *parametri;
    m            = parametri->m_cost_kib
                   / (SCISSURAE * parametri->lanae)
                   * (SCISSURAE * parametri->lanae);
    o->caudices  = m;
    o->q         = m / parametri->lanae;
    o->sl        = o->q / SCISSURAE;
    o->memoria   = (i64*)piscina_allocare(piscina,
        (i64)m * OCTETI_CAUDICIS);
    si (!o->memoria)
    {
        redde NIHIL;
    }

    /* H0 (par. 3.2) */
    (vacuum)blake2b_incipere(&b, LXIV, NIHIL, 0);
    _quaterni_addere(&b, parametri->lanae);
    _quaterni_addere(&b, parametri->longitudo);
    _quaterni_addere(&b, parametri->m_cost_kib);
    _quaterni_addere(&b, parametri->t_cost);
    _quaterni_addere(&b, ARGON2ID_VERSIO);
    _quaterni_addere(&b, TYPUS_ID);
    _quaterni_addere(&b, tn);
    blake2b_addere(&b, tessera, tn);
    _quaterni_addere(&b, sn);
    blake2b_addere(&b, sal, sn);
    _quaterni_addere(&b, parametri->secretum_n);
    blake2b_addere(&b, parametri->secretum, parametri->secretum_n);
    _quaterni_addere(&b, parametri->ad_n);
    blake2b_addere(&b, parametri->ad, parametri->ad_n);
    blake2b_finire(&b, h0);

    /* B[l][0], B[l][1] = H'^1024(H0 || LE32(0|1) || LE32(l)) */
    per (l = 0; l < parametri->lanae; l++)
    {
        _quaterni_scribere(h0 + LXVIII, l);
        _quaterni_scribere(h0 + LXIV, 0);
        _digestum_varium(caudex, OCTETI_CAUDICIS, h0, LXXII, NIHIL, 0);
        _caudex_ex_octetis(_caudex(o, l, 0), caudex);
        _quaterni_scribere(h0 + LXIV, I);
        _digestum_varium(caudex, OCTETI_CAUDICIS, h0, LXXII, NIHIL, 0);
        _caudex_ex_octetis(_caudex(o, l, I), caudex);
    }
    memset(h0, 0, magnitudo(h0));
    memset(caudex, 0, magnitudo(caudex));

    o->index = II;   /* transitus 0, scissura 0: columnae 0-1 factae */
    redde o;
}

b32
argon2id_gradus (
     Argon2idOpus* opus,
              i32  caudices_maximi)
{
    i32 facti = 0;

    si (!opus)
    {
        redde VERUM;
    }
    dum (   !opus->finitum
         && (caudices_maximi == 0 || facti < caudices_maximi))
    {
        _caudicem_implere(opus);
        facti++;
    }
    redde opus->finitum;
}

vacuum
argon2id_digestum (
    Argon2idOpus* opus,
              i8* exitus)
{
    i64 c[VERBA_CAUDICIS];
     i8 o[OCTETI_CAUDICIS];
    i32 l;
    i32 i;

    si (!opus || !opus->finitum)
    {
        redde;
    }
    memcpy(c, _caudex(opus, 0, opus->q - I), magnitudo(c));
    per (l = I; l < opus->par.lanae; l++)
    {
        constans i64* u = _caudex(opus, l, opus->q - I);

        per (i = 0; i < VERBA_CAUDICIS; i++)
        {
            c[i] ^= u[i];
        }
    }
    _caudex_ad_octetos(o, c);
    _digestum_varium(exitus, opus->par.longitudo, o, OCTETI_CAUDICIS,
        NIHIL, 0);
    /* memoria ex tessera derivata non manet */
    memset(opus->memoria, 0,
        (memoriae_index)opus->caudices * OCTETI_CAUDICIS);
    memset(c, 0, magnitudo(c));
    memset(o, 0, magnitudo(o));
}

b32
argon2id_computare (
    constans Argon2idParametri* parametri,
                   constans i8* tessera,
                           i32  tn,
                   constans i8* sal,
                           i32  sn,
                            i8* exitus,
                       Piscina* piscina)
{
    Argon2idOpus* o = argon2id_incipere(parametri, tessera, tn, sal, sn,
                          piscina);

    si (!o)
    {
        redde FALSUM;
    }
    (vacuum)argon2id_gradus(o, 0);
    argon2id_digestum(o, exitus);
    redde VERUM;
}


/* ==================================================
 * PHC
 * ================================================== */

/* base64 sine '=' */
interior chorda
_b64 (
    constans i8* d,
            i32  n,
        Piscina* p)
{
    chorda c = base64_codificare(d, n, p);

    dum (c.mensura > 0 && c.datum[c.mensura - I] == '=')
    {
        c.mensura--;
    }
    redde c;
}

chorda
argon2id_phc_scribere (
    constans Argon2idParametri* parametri,
                   constans i8* sal,
                           i32  sn,
                   constans i8* digestum,
                       Piscina* piscina)
{
    character  caput[XCVI];
       chorda  s;
       chorda  d;
       chorda  vacua;
           i8* o;
          i32  n;

    vacua.datum    = NIHIL;
    vacua.mensura  = 0;
    si (   !piscina || !_parametri_validi(parametri, sn) || !sal
        || !digestum)
    {
        redde vacua;
    }
    sprintf(caput, "$argon2id$v=19$m=%u,t=%u,p=%u$",
        parametri->m_cost_kib, parametri->t_cost, parametri->lanae);
    s = _b64(sal, sn, piscina);
    d = _b64(digestum, parametri->longitudo, piscina);
    n = (i32)strlen(caput) + s.mensura + I + d.mensura;
    o = (i8*)piscina_allocare(piscina, (i64)n + I);
    memcpy(o, caput, strlen(caput));
    memcpy(o + strlen(caput), s.datum, (size_t)s.mensura);
    o[(i32)strlen(caput) + s.mensura] = '$';
    memcpy(o + (i32)strlen(caput) + s.mensura + I, d.datum,
        (size_t)d.mensura);
    o[n] = '\0';
    {
        chorda r;

        r.datum    = o;
        r.mensura  = n;
        redde r;
    }
}

/* praefixum literale ad *i */
interior b32
_consumere (
                chorda  s,
                   i32* i,
    constans character* l)
{
    i32 n = (i32)strlen(l);

    si (s.mensura - *i < n || memcmp(s.datum + *i, l, (size_t)n) != 0)
    {
        redde FALSUM;
    }
    *i += n;
    redde VERUM;
}

/* numerus decimalis 1..IX signorum, > 0 */
interior b32
_numerum (
    chorda  s,
       i32* i,
       i32* v)
{
    i32 k = 0;

    *v = 0;
    dum (   *i < s.mensura && s.datum[*i] >= '0' && s.datum[*i] <= '9'
         && k < IX)
    {
        *v = *v * X + (i32)(s.datum[*i] - '0');
        (*i)++;
        k++;
    }
    redde k > 0 && *v > 0
        && !(*i < s.mensura && s.datum[*i] >= '0'
        && s.datum[*i] <= '9');
}

/* campus base64 (sine '=') usque ad '$' aut finem; decodificatus in
 * exitus (<= maximum) */
interior b32
_b64_legere (
    chorda  s,
       i32* i,
        i8* exitus,
       i32* n,
       i32  maximum)
{
          i32  initium = *i;
          i32  m;
    character  copia[CXXXII];
      Piscina* p;
          b32  bonum = FALSUM;

    dum (*i < s.mensura && s.datum[*i] != '$')
    {
        i8 c = s.datum[*i];

        si (!(   (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
              || (c >= '0' && c <= '9') || c == '+' || c == '/'))
        {
            redde FALSUM;
        }
        (*i)++;
    }
    m = *i - initium;
    si (m == 0 || m % IV == I || m > CXXVIII)
    {
        redde FALSUM;
    }
    memcpy(copia, s.datum + initium, (size_t)m);
    dum (m % IV != 0)
    {
        copia[m++] = '=';
    }
    p = piscina_generare_dynamicum("argon2id_phc", CCLVI);
    si (p)
    {
        Base64Fructus f = base64_decodificare(chorda_ex_buffer(
            (i8*)copia, m), p);

        si (f.datum && f.mensura >= I && f.mensura <= maximum)
        {
            memcpy(exitus, f.datum, (size_t)f.mensura);
            *n     = f.mensura;
            bonum  = VERUM;
        }
        piscina_destruere(p);
    }
    redde bonum;
}

b32
argon2id_phc_legere (
                chorda  phc,
     Argon2idParametri* parametri,
                    i8* sal,
                   i32* sn,
                    i8* digestum,
                   i32* dn)
{
    i32 i = 0;
    i32 v;
    i32 m;
    i32 t;
    i32 p;

    si (!parametri || !sal || !sn || !digestum || !dn || !phc.datum)
    {
        redde FALSUM;
    }
    si (   !_consumere(phc, &i, "$argon2id$v=")
        || !_numerum(phc, &i, &v)
        || v != XIX
        || !_consumere(phc, &i, "$m=") || !_numerum(phc, &i, &m)
        || !_consumere(phc, &i, ",t=") || !_numerum(phc, &i, &t)
        || !_consumere(phc, &i, ",p=") || !_numerum(phc, &i, &p)
        || !_consumere(phc, &i, "$")
        || !_b64_legere(phc, &i, sal, sn, ARGON2ID_SAL_MAXIMUS)
        || !_consumere(phc, &i, "$")
        || !_b64_legere(phc, &i, digestum, dn,
        ARGON2ID_DIGESTUM_MAXIMUM)
        || i != phc.mensura)
    {
        redde FALSUM;
    }
    parametri->m_cost_kib  = m;
    parametri->t_cost      = t;
    parametri->lanae       = p;
    parametri->longitudo   = *dn;
    redde _parametri_validi(parametri, *sn);
}
