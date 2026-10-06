/* situs.c - Praedicata geometrica exacta super fractionem
 *
 * Omnia signa determinantium: orientatio plana = productum crucis,
 * orientatio spatii = productum triplex det[u, v, w] = u . (v x w).
 * Casus degeneres per probationes "inter" (coordinatae aut producta
 * scalaria) nominantur. Transitus sine coordinatis proiectis: in
 * proiectione secundum v, crux plana fit det[x, y, v] et scalare planum
 * (x.y)(v.v) - (x.v)(y.v) - utrumque per factorem positivum
 * multiplicatum (|v|, |v|^2), ergo signa exacta. Functiones publicae
 * quae signum aut genus reddunt piscinam notant et reficiunt.
 * Vide lib/situs.worklog.md.
 */
#include "situs.h"


/* ==================================================
 * Auxilia vectorum (piscina sine refectione)
 * ================================================== */

/* a - b */
interior Punctum
_minus (
     Punctum  a,
     Punctum  b,
     Piscina* piscina)
{
    Punctum r;

    r.x = fractio_subtrahe(a.x, b.x, piscina);
    r.y = fractio_subtrahe(a.y, b.y, piscina);
    r.z = fractio_subtrahe(a.z, b.z, piscina);
    redde r;
}

interior Fractio
_scalare (
     Punctum  u,
     Punctum  v,
     Piscina* piscina)
{
    redde fractio_adde(fractio_adde(
        fractio_multiplica(u.x, v.x, piscina),
        fractio_multiplica(u.y, v.y, piscina), piscina),
        fractio_multiplica(u.z, v.z, piscina), piscina);
}

interior Punctum
_crux (
     Punctum  u,
     Punctum  v,
     Piscina* piscina)
{
    Punctum r;

    r.x = fractio_subtrahe(fractio_multiplica(u.y, v.z, piscina),
        fractio_multiplica(u.z, v.y, piscina), piscina);
    r.y = fractio_subtrahe(fractio_multiplica(u.z, v.x, piscina),
        fractio_multiplica(u.x, v.z, piscina), piscina);
    r.z = fractio_subtrahe(fractio_multiplica(u.x, v.y, piscina),
        fractio_multiplica(u.y, v.x, piscina), piscina);
    redde r;
}

/* det[u, v, w] = u . (v x w) */
interior Fractio
_triplex (
     Punctum  u,
     Punctum  v,
     Punctum  w,
     Piscina* piscina)
{
    redde _scalare(u, _crux(v, w, piscina), piscina);
}

interior s32
_signum_triplex (
     Punctum  u,
     Punctum  v,
     Punctum  w,
     Piscina* piscina)
{
    redde fractio_signum(_triplex(u, v, w, piscina));
}

interior b32
_nullus (
    Punctum u)
{
    redde fractio_signum(u.x) == ZEPHYRUM
        && fractio_signum(u.y) == ZEPHYRUM
        && fractio_signum(u.z) == ZEPHYRUM;
}

/* coordinata k (0 x, 1 y, 2 z) */
interior Fractio
_coordinata (
    Punctum a,
        s32 k)
{
    si (k == ZEPHYRUM)
    {
        redde a.x;
    }
    si (k == I)
    {
        redde a.y;
    }
    redde a.z;
}

/* proiectio in planum coordinatarum omissa coordinata k */
interior PunctumPlani
_omissa (
    Punctum a,
        s32 k)
{
    PunctumPlani r;

    r.x = (k == ZEPHYRUM) ? a.y : a.x;
    r.y = (k == II) ? a.y : a.z;
    redde r;
}

/* axis ubi componens vectoris non nulla est (-1 si nullus) */
interior s32
_axis_non_nulla (
    Punctum u)
{
    s32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        si (fractio_signum(_coordinata(u, k)) != ZEPHYRUM)
        {
            redde k;
        }
    }
    redde -I;
}


/* ==================================================
 * Planum
 * ================================================== */

interior s32
_orientatio_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
         Piscina* piscina)
{
    Fractio crux = fractio_subtrahe(
        fractio_multiplica(fractio_subtrahe(b.x, a.x, piscina),
            fractio_subtrahe(c.y, a.y, piscina), piscina),
        fractio_multiplica(fractio_subtrahe(b.y, a.y, piscina),
            fractio_subtrahe(c.x, a.x, piscina), piscina), piscina);

    redde fractio_signum(crux);
}

/* r inter extrema p et q (singulis coordinatis; r collineare datum) */
interior b32
_inter_plana (
    PunctumPlani  p,
    PunctumPlani  q,
    PunctumPlani  r,
         Piscina* piscina)
{
    redde fractio_compara(r.x, p.x, piscina) *
              fractio_compara(r.x, q.x, piscina) <= ZEPHYRUM
        && fractio_compara(r.y, p.y, piscina) *
              fractio_compara(r.y, q.y, piscina) <= ZEPHYRUM;
}

interior SitusContactus
_segmenta_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
    PunctumPlani  d,
         Piscina* piscina)
{
    s32 o1 = _orientatio_plana(a, b, c, piscina);
    s32 o2 = _orientatio_plana(a, b, d, piscina);
    s32 o3 = _orientatio_plana(c, d, a, piscina);
    s32 o4 = _orientatio_plana(c, d, b, piscina);

    si (o1 * o2 < ZEPHYRUM && o3 * o4 < ZEPHYRUM)
    {
        redde SITUS_SECANT;
    }
    si (   (o1 == ZEPHYRUM && _inter_plana(a, b, c, piscina))
        || (o2 == ZEPHYRUM && _inter_plana(a, b, d, piscina))
        || (o3 == ZEPHYRUM && _inter_plana(c, d, a, piscina))
        || (o4 == ZEPHYRUM && _inter_plana(c, d, b, piscina)))
    {
        redde SITUS_TANGUNT;
    }
    redde SITUS_DISIUNCTA;
}


/* ==================================================
 * Spatium
 * ================================================== */

interior SitusContactus
_segmenta (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina)
{
    Punctum ab = _minus(b, a, piscina);
    Punctum normalis;
        s32 k;

    si (_signum_triplex(ab, _minus(c, a, piscina), _minus(d, a,
        piscina),
            piscina) != ZEPHYRUM)
    {
        redde SITUS_DISIUNCTA;
    }

    /* coplanaria: normalis non nulla ex tribus punctis non
     * collinearibus */
    normalis = _crux(ab, _minus(c, a, piscina), piscina);
    si (_nullus(normalis))
    {
        normalis = _crux(ab, _minus(d, a, piscina), piscina);
    }
    si (_nullus(normalis))
    {
        Punctum cd = _minus(d, c, piscina);

        normalis = _crux(cd, _minus(a, c, piscina), piscina);
        si (_nullus(normalis))
        {
            normalis = _crux(cd, _minus(b, c, piscina), piscina);
        }
    }
    k = _axis_non_nulla(normalis);
    si (k >= ZEPHYRUM)
    {
        /* proiectio omissa axe ubi normalis non nulla: iniectiva in
         * plano */
        redde _segmenta_plana(_omissa(a, k), _omissa(b, k), _omissa(c,
            k),
            _omissa(d, k), piscina);
    }

    /* omnia collinearia: intervalla in coordinata ubi recta non est
     * perpendicularis */
    {
        Punctum directio = ab;
        Fractio imum_primum;
        Fractio summum_primum;
        Fractio imum_alterum;
        Fractio summum_alterum;

        si (_nullus(directio))
        {
            directio = _minus(d, c, piscina);
        }
        si (_nullus(directio))
        {
            directio = _minus(c, a, piscina);
        }
        k = _axis_non_nulla(directio);
        si (k < ZEPHYRUM)
        {
            redde SITUS_TANGUNT;   /* puncta omnia eadem */
        }
        imum_primum    = _coordinata(a, k);
        summum_primum  = _coordinata(b, k);
        si (fractio_compara(imum_primum, summum_primum, piscina)
            > ZEPHYRUM)
        {
            Fractio t = imum_primum;

            imum_primum    = summum_primum;
            summum_primum  = t;
        }
        imum_alterum    = _coordinata(c, k);
        summum_alterum  = _coordinata(d, k);
        si (fractio_compara(imum_alterum, summum_alterum, piscina)
            > ZEPHYRUM)
        {
            Fractio t = imum_alterum;

            imum_alterum    = summum_alterum;
            summum_alterum  = t;
        }
        si (   fractio_compara(imum_primum, summum_alterum, piscina)
            <= ZEPHYRUM
            && fractio_compara(imum_alterum, summum_primum, piscina)
                <= ZEPHYRUM)
        {
            redde SITUS_TANGUNT;
        }
        redde SITUS_DISIUNCTA;
    }
}

/* [a,b] et [b,c] vertice b communi: aliud punctum commune sse
 * collinearia eadem directione ab b (u x w nullum, u . w > 0) */
interior SitusContactus
_segmenta_vicina (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Piscina* piscina)
{
    Punctum u = _minus(a, b, piscina);
    Punctum w = _minus(c, b, piscina);

    si (_nullus(u) || _nullus(w))
    {
        redde SITUS_TANGUNT;   /* segmentum nullum: punctum ipsum b */
    }
    si (   _nullus(_crux(u, w, piscina))
        && fractio_signum(_scalare(u, w, piscina)) > ZEPHYRUM)
    {
        redde SITUS_TANGUNT;
    }
    redde SITUS_DISIUNCTA;
}

/* p coplanare cum triangulo non degeneri: intra triangulum clausum? */
interior b32
_in_triangulo (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  normalis,
     Punctum  p,
     Piscina* piscina)
{
             s32 k   = _axis_non_nulla(normalis);
    PunctumPlani a   = _omissa(t0, k);
    PunctumPlani b   = _omissa(t1, k);
    PunctumPlani c   = _omissa(t2, k);
    PunctumPlani r   = _omissa(p, k);
             s32 o1  = _orientatio_plana(a, b, r, piscina);
             s32 o2  = _orientatio_plana(b, c, r, piscina);
             s32 o3  = _orientatio_plana(c, a, r, piscina);

    redde (o1 >= ZEPHYRUM && o2 >= ZEPHYRUM && o3 >= ZEPHYRUM)
        || (o1 <= ZEPHYRUM && o2 <= ZEPHYRUM && o3 <= ZEPHYRUM);
}

interior SitusContactus
_triangulum_segmentum (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  p,
     Punctum  q,
     Piscina* piscina)
{
    Punctum e1        = _minus(t1, t0, piscina);
    Punctum e2        = _minus(t2, t0, piscina);
    Punctum normalis  = _crux(e1, e2, piscina);
        s32 latus_p;
        s32 latus_q;
        s32 s0;
        s32 s1;
        s32 s2;
    Punctum directio_segmenti;

    /* triangulum degener (collineare): contactus cum marginibus
     * solus */
    si (_nullus(normalis))
    {
        si (   _segmenta(t0, t1, p, q, piscina) != SITUS_DISIUNCTA
            || _segmenta(t1, t2, p, q, piscina) != SITUS_DISIUNCTA
            || _segmenta(t2, t0, p, q, piscina) != SITUS_DISIUNCTA)
        {
            redde SITUS_TANGUNT;
        }
        redde SITUS_DISIUNCTA;
    }

    latus_p = fractio_signum(_scalare(normalis, _minus(p, t0, piscina),
        piscina));
    latus_q = fractio_signum(_scalare(normalis, _minus(q, t0, piscina),
        piscina));
    si (latus_p * latus_q > ZEPHYRUM)
    {
        redde SITUS_DISIUNCTA;   /* ex eadem parte plani */
    }

    si (latus_p == ZEPHYRUM && latus_q == ZEPHYRUM)
    {
        /* segmentum coplanare: contactus = margo tacta aut extremum
         * intra (tunc segmentum totum intra esse potest) */
        si (   _segmenta(t0, t1, p, q, piscina) != SITUS_DISIUNCTA
            || _segmenta(t1, t2, p, q, piscina) != SITUS_DISIUNCTA
            || _segmenta(t2, t0, p, q, piscina) != SITUS_DISIUNCTA
            || _in_triangulo(t0, t1, t2, normalis, p, piscina))
        {
            redde SITUS_TANGUNT;
        }
        redde SITUS_DISIUNCTA;
    }

    /* recta pq planum in puncto uno secat (intra segmentum, quia
     * latus_p * latus_q <= 0): intra triangulum clausum sse s0, s1,
     * s2 signis strictis non discrepant */
    directio_segmenti = _minus(q, p, piscina);
    s0 = _signum_triplex(directio_segmenti, _minus(t0, p, piscina),
        _minus(t1, p,
        piscina), piscina);
    s1 = _signum_triplex(directio_segmenti, _minus(t1, p, piscina),
        _minus(t2, p,
        piscina), piscina);
    s2 = _signum_triplex(directio_segmenti, _minus(t2, p, piscina),
        _minus(t0, p,
        piscina), piscina);
    si (   (s0 > ZEPHYRUM || s1 > ZEPHYRUM || s2 > ZEPHYRUM)
        && (s0 < ZEPHYRUM || s1 < ZEPHYRUM || s2 < ZEPHYRUM))
    {
        redde SITUS_DISIUNCTA;
    }
    si (   latus_p == ZEPHYRUM || latus_q == ZEPHYRUM
        || s0      == ZEPHYRUM || s1 == ZEPHYRUM || s2 == ZEPHYRUM)
    {
        /* extremum in plano, aut margo/vertex */
        redde SITUS_TANGUNT;
    }
    redde SITUS_SECANT;
}

/* segmentum [t0,x] et triangulum [t0,t1,t2] vertice t0 communi.
 * Triangulum convexum t0 continet, ergo aliud punctum commune sse
 * d = x - t0 coplanaris et in cono clauso {alpha e1 + beta e2: alpha,
 * beta >= 0}. Pro d = alpha e1 + beta e2: (e1 x d) . n = beta |n|^2 et
 * (d x e2) . n = alpha |n|^2, ergo signa coefficientes nominant. */
interior SitusContactus
_triangulum_vicinum (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  x,
     Piscina* piscina)
{
    Punctum e1        = _minus(t1, t0, piscina);
    Punctum e2        = _minus(t2, t0, piscina);
    Punctum d         = _minus(x, t0, piscina);
    Punctum normalis  = _crux(e1, e2, piscina);

    si (_nullus(d) || _nullus(normalis))
    {
        /* segmentum nullum aut triangulum degener */
        redde SITUS_TANGUNT;
    }
    si (fractio_signum(_scalare(normalis, d, piscina)) != ZEPHYRUM)
    {
        redde SITUS_DISIUNCTA;   /* planum in t0 solo transit */
    }
    si (   fractio_signum(_scalare(_crux(e1, d, piscina), normalis,
               piscina)) >= ZEPHYRUM
        && fractio_signum(_scalare(_crux(d, e2, piscina), normalis,
               piscina)) >= ZEPHYRUM)
    {
        redde SITUS_TANGUNT;
    }
    redde SITUS_DISIUNCTA;
}


/* ==================================================
 * Proiectio secundum v
 * ================================================== */

/* scalare in proiectione (multiplicatum per v.v > 0) */
interior Fractio
_scalare_proiectum (
     Punctum  x,
     Punctum  y,
     Punctum  v,
     Piscina* piscina)
{
    redde fractio_subtrahe(
        fractio_multiplica(_scalare(x, y, piscina),
            _scalare(v, v, piscina), piscina),
        fractio_multiplica(_scalare(x, v, piscina),
            _scalare(y, v, piscina), piscina), piscina);
}

/* r (in proiectione collineare cum pq) inter extrema proiecta? Si pq in
 * punctum proiicitur, r illud punctum esse debet. */
interior b32
_inter_proiectum (
     Punctum  p,
     Punctum  q,
     Punctum  r,
     Punctum  v,
     Piscina* piscina)
{
    Punctum directio_segmenti  = _minus(q, p, piscina);
    Punctum pr                 = _minus(r, p, piscina);

    si (fractio_signum(_scalare_proiectum(directio_segmenti,
        directio_segmenti, v, piscina))
        == ZEPHYRUM)
    {
        redde fractio_signum(_scalare_proiectum(pr, pr, v, piscina))
            == ZEPHYRUM;
    }
    redde fractio_signum(_scalare_proiectum(pr, directio_segmenti, v,
        piscina))
        >= ZEPHYRUM
        && fractio_signum(_scalare_proiectum(_minus(r, q, piscina),
            _minus(p, q, piscina), v, piscina)) >= ZEPHYRUM;
}

interior SitusContactus
_transitus (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Punctum  v,
     Piscina* piscina,
         s32* superius,
         s32* signum)
{
    Punctum u = _minus(b, a, piscina);
    Punctum w = _minus(d, c, piscina);
    Punctum ca;
        s32 o1;
        s32 o2;
        s32 o3;
        s32 o4;

    si (_nullus(v))
    {
        redde SITUS_TANGUNT;
    }
    ca = _minus(c, a, piscina);
    o1 = _signum_triplex(u, ca, v, piscina);
    o2 = _signum_triplex(u, _minus(d, a, piscina), v, piscina);
    o3 = _signum_triplex(w, _minus(a, c, piscina), v, piscina);
    o4 = _signum_triplex(w, _minus(b, c, piscina), v, piscina);

    si (o1 * o2 < ZEPHYRUM && o3 * o4 < ZEPHYRUM)
    {
        /* s u - t w = (c - a) + lambda v; Cramer cum v: det[u, w, v]
         * non nullum quia proiectiones non parallelae */
        Fractio determinans  = _triplex(u, w, v, piscina);
        Fractio s            = fractio_ex_s64(ZEPHYRUM);
        Fractio t            = fractio_ex_s64(ZEPHYRUM);
        Punctum pa;
        Punctum pc;
            s32 ordo;

        (vacuum)fractio_divide(_triplex(ca, w, v, piscina), determinans,
            piscina, &s);
        (vacuum)fractio_divide(_triplex(ca, u, v, piscina), determinans,
            piscina, &t);
        pa.x = fractio_adde(a.x, fractio_multiplica(s, u.x, piscina),
            piscina);
        pa.y = fractio_adde(a.y, fractio_multiplica(s, u.y, piscina),
            piscina);
        pa.z = fractio_adde(a.z, fractio_multiplica(s, u.z, piscina),
            piscina);
        pc.x = fractio_adde(c.x, fractio_multiplica(t, w.x, piscina),
            piscina);
        pc.y = fractio_adde(c.y, fractio_multiplica(t, w.y, piscina),
            piscina);
        pc.z = fractio_adde(c.z, fractio_multiplica(t, w.z, piscina),
            piscina);
        ordo = fractio_compara(_scalare(v, pa, piscina),
            _scalare(v, pc, piscina), piscina);
        si (ordo == ZEPHYRUM)
        {
            redde SITUS_TANGUNT;   /* segmenta in spatio se secant */
        }
        si (superius)
        {
            *superius = (ordo > ZEPHYRUM) ? ZEPHYRUM : I;
        }
        si (signum)
        {
            /* det[o, u, v]: o = u si [a,b] supra, aliter o = w */
            s32 signum_determinantis = fractio_signum(determinans);

            *signum = (ordo > ZEPHYRUM) ? signum_determinantis
                : -signum_determinantis;
        }
        redde SITUS_SECANT;
    }
    si (   (o1 == ZEPHYRUM && _inter_proiectum(a, b, c, v, piscina))
        || (o2 == ZEPHYRUM && _inter_proiectum(a, b, d, v, piscina))
        || (o3 == ZEPHYRUM && _inter_proiectum(c, d, a, v, piscina))
        || (o4 == ZEPHYRUM && _inter_proiectum(c, d, b, v, piscina)))
    {
        redde SITUS_TANGUNT;
    }
    redde SITUS_DISIUNCTA;
}

/* proiectiones [a,b] et [b,c] secundum v, vertice b communi: aliud
 * punctum commune sse aliqua in punctum proiicitur aut proiectiones
 * parallelae (det[u, w, v] nullum) eadem directione ab b */
interior SitusContactus
_transitus_vicinus (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  v,
     Piscina* piscina)
{
    Punctum u = _minus(a, b, piscina);
    Punctum w = _minus(c, b, piscina);

    si (   _nullus(v)
        || fractio_signum(_scalare_proiectum(u, u, v, piscina))
            == ZEPHYRUM
        || fractio_signum(_scalare_proiectum(w, w, v, piscina))
            == ZEPHYRUM)
    {
        redde SITUS_TANGUNT;
    }
    si (   _signum_triplex(u, w, v, piscina) == ZEPHYRUM
        && fractio_signum(_scalare_proiectum(u, w, v, piscina))
            > ZEPHYRUM)
    {
        redde SITUS_TANGUNT;
    }
    redde SITUS_DISIUNCTA;
}


/* ==================================================
 * Publica
 * ================================================== */

PunctumPlani
situs_punctum_plani (
    s64 x,
    s64 y)
{
    PunctumPlani p;

    p.x = fractio_ex_s64(x);
    p.y = fractio_ex_s64(y);
    redde p;
}

Punctum
situs_punctum (
    s64 x,
    s64 y,
    s64 z)
{
    Punctum p;

    p.x = fractio_ex_s64(x);
    p.y = fractio_ex_s64(y);
    p.z = fractio_ex_s64(z);
    redde p;
}

b32
situs_puncta_aequalia (
    Punctum a,
    Punctum b)
{
    redde fractio_aequalis(a.x, b.x) && fractio_aequalis(a.y, b.y)
        && fractio_aequalis(a.z, b.z);
}

s32
situs_orientatio_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
         Piscina* piscina)
{
    PiscinaNotatio nota          = piscina_notare(piscina);
               s32 r  = _orientatio_plana(a, b, c,
                   piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

s32
situs_orientatio (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina)
{
    PiscinaNotatio nota = piscina_notare(piscina);
               s32 r    = _signum_triplex(_minus(b, a,
                   piscina),
                   _minus(c, a, piscina), _minus(d, a, piscina),
                   piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

Fractio
situs_volumen_sexies (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina)
{
    redde _triplex(_minus(b, a, piscina), _minus(c, a, piscina),
        _minus(d, a, piscina), piscina);
}

SitusContactus
situs_segmenta_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
    PunctumPlani  d,
         Piscina* piscina)
{
    PiscinaNotatio nota  = piscina_notare(piscina);
    SitusContactus r     = _segmenta_plana(a, b, c, d, piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_segmenta (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina)
{
    PiscinaNotatio nota  = piscina_notare(piscina);
    SitusContactus r     = _segmenta(a, b, c, d, piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_triangulum_segmentum (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  p,
     Punctum  q,
     Piscina* piscina)
{
    PiscinaNotatio nota = piscina_notare(piscina);
    SitusContactus r    = _triangulum_segmentum(t0, t1, t2, p, q,
        piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_transitus (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Punctum  v,
     Piscina* piscina,
         s32* superius,
         s32* signum)
{
    PiscinaNotatio nota = piscina_notare(piscina);
    SitusContactus r    = _transitus(a, b, c, d, v, piscina, superius,
        signum);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_segmenta_vicina (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Piscina* piscina)
{
    PiscinaNotatio nota  = piscina_notare(piscina);
    SitusContactus r     = _segmenta_vicina(a, b, c, piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_triangulum_vicinum (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  x,
     Piscina* piscina)
{
    PiscinaNotatio nota  = piscina_notare(piscina);
    SitusContactus r     = _triangulum_vicinum(t0, t1, t2, x, piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_transitus_vicinus (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  v,
     Piscina* piscina)
{
    PiscinaNotatio nota  = piscina_notare(piscina);
    SitusContactus r     = _transitus_vicinus(a, b, c, v, piscina);

    piscina_reficere(piscina, nota);
    redde r;
}
