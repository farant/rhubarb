/* probatio_laqueus_rete.c - Uncinus et Jones per modulum vertex
 *
 * Oracula: summa statuum laqueus (laqueus_uncinus_ex_pd, usque ad
 * XXIV transitus) et Jones tabulae nodorum (KnotInfo collatum) - hic
 * solum _pd_legere et _pd_planus communicantur. Codices cum ansis R1
 * manu et fortuito insertis, polygona fortuita, nodi ultra XXIV
 * transitus (ansis additis: Jones idem). Gyrationes per ansas statuum
 * fortuitorum probantur (quaeque +-4, sine solutore). Punctum d = 0
 * (p = 17, A = 2). Vide project-specs/contractio-plan.md (T4).
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "chorda.h"
#include "sors.h"
#include "fractio.h"
#include "situs.h"
#include "magnus.h"
#include "polynomium.h"
#include "congruentia.h"
#include "interpolatio.h"
#include "contractio.h"
#include "laqueus.h"
#include "tabula_nodorum.h"

/* officina satis pro omnibus retibus hic (MB) */
#define OCTETI_MAXIMI ((i64)0x10000000UL)


/* ==================================================
 * Auxilia
 * ================================================== */

/* signum transitus codicis 1-basati: +1 si X1 = X3 + 1, -1 si X3 = X1
 * + 1 (modulo m) */
interior s32
_signum (
    constans i32* x,
             i32  m)
{
    redde x[I] == x[III] % m + I ? (s32)I : -(s32)I;
}

/* ansa R1 in os e (1..m) inserta, genus 0..3 (infra/supra primum,
 * positiva/negativa): ora > e + 2, occursus intrans ossis e -> e + 2,
 * transitus novus in fine. novum: IV (c + 1) elementa. */
interior vacuum
_ansam_inserere (
    constans i32* pd,
             i32  c,
             i32  e,
             i32  genus,
             i32* novum)
{
    i32 m = II * c;
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < c; k++)
    {
        constans i32* x       = pd + IV * k;
                 s32  signum  = _signum(x, m);

        per (j = ZEPHYRUM; j < IV; j++)
        {
            i32 l = x[j];
            b32 intrans = j == ZEPHYRUM
                || (j == III && signum > ZEPHYRUM)
                || (j == I && signum < ZEPHYRUM);

            si (l > e)
            {
                l += II;
            }
            alioquin si (l == e && intrans)
            {
                l = e + II;
            }
            novum[IV * k + j] = l;
        }
    }
    {
        i32* x = novum + IV * c;

        commutatio (genus)
        {
            casus ZEPHYRUM:
                x[0] = e; x[I] = e + II; x[II] = e + I; x[III] = e + I;
                frange;
            casus I:
                x[0] = e; x[I] = e + I; x[II] = e + I; x[III] = e + II;
                frange;
            casus II:
                x[0] = e + I; x[I] = e + I; x[II] = e + II; x[III] = e;
                frange;
            ordinarius:
                x[0] = e + I; x[I] = e; x[II] = e + II; x[III] = e + I;
                frange;
        }
    }
}

/* polynomium tabulae ("..." in t) */
interior Polynomium
_polynomium_textus (
    constans character* textus,
               Piscina* piscina)
{
    Polynomium p = polynomium_nullum();

    CREDO_NECESSE(polynomium_ex_chorda(chorda_ex_literis(textus,
        piscina),
        't', piscina, &p));
    redde p;
}

/* rete et summa statuum: uncinus aequalis; exponentes summae in classe
 * 3w modulo 4 */
interior vacuum
_uncinum_comparare (
    constans i32* pd,
             i32  c,
         Piscina* piscina)
{
    Polynomium status;
    Polynomium rete;
           s32 w = ZEPHYRUM;
           s32 e;
           i32 k;

    CREDO_NECESSE(laqueus_uncinus_ex_pd(pd, c, piscina, &status));
    CREDO_NECESSE(laqueus_uncinus_ex_pd_rete(pd, c, OCTETI_MAXIMI,
        piscina, &rete, NIHIL));
    CREDO_VERUM(polynomium_aequalis(rete, status));
    per (k = ZEPHYRUM; k < c; k++)
    {
        w += _signum(pd + IV * k, II * c);
    }
    per (e = polynomium_gradus_imus(status);
         e <= polynomium_gradus_summus(status); e++)
    {
        si (magnus_signum(polynomium_coefficiens(status, e))
            != ZEPHYRUM)
        {
            CREDO_VERUM(((e - III * w) % (s32)IV + (s32)IV) % (s32)IV
                == ZEPHYRUM);
        }
    }
}

/* Jones per rete = Jones exspectatum */
interior vacuum
_jones_comparare (
    constans i32* pd,
             i32  c,
       Polynomium exspectatum,
         Piscina* piscina)
{
    Polynomium rete;

    CREDO_NECESSE(laqueus_jones_ex_pd_rete(pd, c, OCTETI_MAXIMI,
        piscina,
        &rete, NIHIL));
    CREDO_VERUM(polynomium_aequalis(rete, exspectatum));
}


/* ==================================================
 * Codices manu et tabula
 * ================================================== */

/* nodi triviales duorum transituum cum ansis; trifolium cum
 * stabilizatione (ansa positiva et negativa) */
interior vacuum
_probare_codices_manu (
    CredoContextus* c)
{
         Piscina* piscina  = c->piscina;
    constans i32  u1[]     = { I, I, II, IV, II, IV, III, III };
    constans i32  u2[]     = { IV, I, I, II, II, IV, III, III };
    constans i32  t1[] = { I, VII, II, VI, VII, III, VIII, II, III, I,
        IV,
        VIII, IV, VI, V, V };
    constans i32 t2[] = { I, VII, II, VI, VII, III, VIII, II, III, I,
        IV,
        VIII, IV, V, V, VI };
    Polynomium unum = polynomium_constans(magnus_ex_s64(I), piscina);
    Polynomium trifolium = _polynomium_textus(
        tabula_nodorum_quaere("3_1")->jones, piscina);

    _uncinum_comparare(u1, II, piscina);
    _uncinum_comparare(u2, II, piscina);
    _uncinum_comparare(t1, IV, piscina);
    _uncinum_comparare(t2, IV, piscina);
    _jones_comparare(u1, II, unum, piscina);
    _jones_comparare(u2, II, unum, piscina);
    _jones_comparare(t1, IV, trifolium, piscina);
    _jones_comparare(t2, IV, trifolium, piscina);
}

/* omnes nodi tabulae usque ad X transitus: uncinus rete = summa
 * statuum */
interior vacuum
_probare_tabulam_x (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        i32  numerus = ZEPHYRUM;
        i32  k;

    per (k = ZEPHYRUM; k < tabula_nodorum_numerus(); k++)
    {
        constans NodusTabulae* n = tabula_nodorum_nodus(k);
               PiscinaNotatio  nota;

        si (n->transitus < II || n->transitus > X)
        {
            perge;
        }
        nota = piscina_notare(piscina);
        _uncinum_comparare(tabula_nodorum_pd(n, piscina), n->transitus,
            piscina);
        piscina_reficere(piscina, nota);
        numerus++;
    }
    CREDO_AEQUALIS_I32(numerus, (i32)249UL);
}

/* omnes XIIDCCCCLXVI nodi tabulae: Jones rete = Jones tabulae */
interior vacuum
_probare_tabulam_totam (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        i32  k;

    CREDO_AEQUALIS_I32(tabula_nodorum_numerus(), (i32)12966UL);
    per (k = ZEPHYRUM; k < tabula_nodorum_numerus(); k++)
    {
        constans NodusTabulae* n     = tabula_nodorum_nodus(k);
               PiscinaNotatio  nota  = piscina_notare(piscina);
                 constans i32* pd   = n->transitus
                     == ZEPHYRUM ? NIHIL
                     : tabula_nodorum_pd(n, piscina);

        _jones_comparare(pd, n->transitus, _polynomium_textus(n->jones,
            piscina), piscina);
        piscina_reficere(piscina, nota);
    }
}


/* ==================================================
 * Ansae R1, polygona, ultra XXIV
 * ================================================== */

/* nodi tabulae (2..9 transitus) cum 1..3 ansis fortuitis: Jones
 * tabulae, uncinus = summa statuum, simplificatio ad c reddit */
interior vacuum
_probare_ansas (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;

    sors_seminare(&s, (i64)MMXXVI, ZEPHYRUM);
    per (t = ZEPHYRUM; t < CC; t++)
    {
               PiscinaNotatio  nota = piscina_notare(piscina);
        constans NodusTabulae* n;
                          i32* pd;
                          i32  transitus;
                          i32  ansae = I + sors_intra(&s, III);
                          i32* simplex;
                          i32  transitus_simplex;
                          i32  a;

        fac
        {
            n = tabula_nodorum_nodus(sors_intra(&s,
                tabula_nodorum_numerus()));
        }
        dum (n->transitus < II || n->transitus > IX);
        transitus = n->transitus;
        pd        = (i32*)piscina_allocare(piscina,
            (memoriae_index)IV * transitus * magnitudo(i32));
        {
            constans i32* fons = tabula_nodorum_pd(n, piscina);
                     i32  j;

            per (j = ZEPHYRUM; j < IV * transitus; j++)
            {
                pd[j] = fons[j];
            }
        }
        per (a = ZEPHYRUM; a < ansae; a++)
        {
            i32* novum = (i32*)piscina_allocare(piscina,
                (memoriae_index)IV * (transitus + I) * magnitudo(i32));

            _ansam_inserere(pd, transitus, I + sors_intra(&s, II
                * transitus), sors_intra(&s, IV), novum);
            pd = novum;
            transitus++;
        }
        _jones_comparare(pd, transitus, _polynomium_textus(n->jones,
            piscina), piscina);
        _uncinum_comparare(pd, transitus, piscina);
        CREDO_NECESSE(laqueus_pd_simplificare(pd, transitus, piscina,
            &simplex, &transitus_simplex));
        CREDO_VERUM(transitus_simplex <= n->transitus);
        piscina_reficere(piscina, nota);
    }
}

/* ultra XXIV: nodus XIII transituum cum XV ansis (XXVIII transitus) -
 * summa statuum refutat, rete Jones tabulae reddit */
interior vacuum
_probare_ultra_limitem (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;

    sors_seminare(&s, (i64)MMXXVII, ZEPHYRUM);
    per (t = ZEPHYRUM; t < IV; t++)
    {
        constans NodusTabulae* n;
                 constans i32* pd;
                          i32  transitus;
                          i32  a;
                   Polynomium  jones;
            ContractioSumptus  sumptus;

        fac
        {
            n = tabula_nodorum_nodus(sors_intra(&s,
                tabula_nodorum_numerus()));
        }
        dum (n->transitus != XIII);
        pd         = tabula_nodorum_pd(n, piscina);
        transitus  = n->transitus;
        per (a = ZEPHYRUM; a < XV; a++)
        {
            i32* novum = (i32*)piscina_allocare(piscina,
                (memoriae_index)IV * (transitus + I) * magnitudo(i32));

            _ansam_inserere(pd, transitus, I + sors_intra(&s, II
                * transitus), sors_intra(&s, IV), novum);
            pd = novum;
            transitus++;
        }
        CREDO_AEQUALIS_I32(transitus, XXVIII);
        CREDO_FALSUM(laqueus_jones_ex_pd(pd, transitus, piscina,
            &jones));
        CREDO_NECESSE(laqueus_jones_ex_pd_rete(pd, transitus,
            OCTETI_MAXIMI, piscina, &jones, &sumptus));
        CREDO_VERUM(polynomium_aequalis(jones, _polynomium_textus(
            n->jones, piscina)));
        CREDO_VERUM(sumptus.apex > ZEPHYRUM);
        CREDO_VERUM(sumptus.octeti_officinae <= OCTETI_MAXIMI);
    }
}

/* polygona fortuita (VIII..XI vertices in [-12, 12]^3): diagramma
 * genericum, codex PD; Jones et uncinus rete = summa statuum */
interior vacuum
_probare_laqueos_fortuitos (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  probata = ZEPHYRUM;

    sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
    per (t = ZEPHYRUM; t < CCC; t++)
    {
        PiscinaNotatio  nota = piscina_notare(piscina);
               Punctum  puncta[XII];
                   i32  initia[II];
                   i32  n = VIII + sors_intra(&s, IV);
                   i32  k;
               Laqueus  l;
             Diagramma  d;
                   i32* pd;
                   i32  transitus;

        per (k = ZEPHYRUM; k < n; k++)
        {
            puncta[k].x = fractio_ex_s64((s64)sors_intra(&s, XXV)
                - XII);
            puncta[k].y = fractio_ex_s64((s64)sors_intra(&s, XXV)
                - XII);
            puncta[k].z = fractio_ex_s64((s64)sors_intra(&s, XXV)
                - XII);
        }
        initia[0] = ZEPHYRUM;
        initia[I] = n;
        si (   laqueus_ex_punctis(puncta, initia, I, piscina, &l)
            && laqueus_simplex(l, piscina)
            && laqueus_diagramma_genericum(l, piscina, &d))
        {
            transitus = diagramma_numerus(d);
            si (   transitus >= II && transitus <= XVI
                && diagramma_pd(d, piscina, &pd))
            {
                Polynomium jones;

                CREDO_NECESSE(diagramma_jones(d, piscina, &jones));
                _jones_comparare(pd, transitus, jones, piscina);
                _uncinum_comparare(pd, transitus, piscina);
                probata++;
            }
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_MAIOR_I64((i64)probata, (i64)L);
}


/* ==================================================
 * Gyrationes, puncta, refutationes
 * ================================================== */

/* ansae statuum fortuitorum: summa gyrationum ossium (+t a -> b, -t
 * b -> a) et angulorum (arma p -> p + 1: -1, aliter +1) = +-4 -
 * proprietas Umlaufsatz, non aequationes solutoris */
interior vacuum
_ansas_statuum_probare (
    constans i32* pd,
             i32  c,
            Sors* s,
         Piscina* piscina)
{
    LaqueusModulusVertex* mv = laqueus_modulus_vertex_creare(pd, c,
        OCTETI_MAXIMI, piscina);
    constans s32* gyrationes;
             i32  n = IV * c;
             i32* finis_a;
             i32* alter;
             b32* visa;
             i32  t;
             i32  k;

    CREDO_NECESSE_NON_NIHIL(mv);
    gyrationes    = laqueus_modulus_vertex_gyrationes(mv);
    finis_a = (i32*)piscina_allocare(piscina, (memoriae_index)(II * c
        + I)
        * magnitudo(i32));
    alter  = (i32*)piscina_allocare(piscina, (memoriae_index)n
        * magnitudo(i32));
    visa   = (b32*)piscina_allocare(piscina, (memoriae_index)n
        * magnitudo(b32));
    per (k = ZEPHYRUM; k <= II * c; k++)
    {
        finis_a[k] = n;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (finis_a[pd[k]] == n)
        {
            finis_a[pd[k]] = k;
        }
        alioquin
        {
            alter[k]               = finis_a[pd[k]];
            alter[finis_a[pd[k]]]  = k;
        }
    }
    per (t = ZEPHYRUM; t < XX; t++)
    {
        i64 status = ZEPHYRUM;
        i32 d;

        per (k = ZEPHYRUM; k < c; k++)
        {
            status |= (i64)sors_intra(s, II) << k;
        }
        per (d = ZEPHYRUM; d < n; d++)
        {
            visa[d] = FALSUM;
        }
        per (d = ZEPHYRUM; d < n; d++)
        {
            s64 summa    = ZEPHYRUM;
            i32 currens  = d;

            si (visa[d])
            {
                perge;
            }
            fac
            {
                i32 o          = alter[currens];
                i32 transitus  = o / IV;
                i32 in         = o % IV;
                b32 b_status   = ((status >> transitus) & (i64)I) != 0;
                i32 ex;

                summa += finis_a[pd[currens]] == currens
                    ? (s64)gyrationes[pd[currens]
                        - I] : -(s64)gyrationes[pd[currens]
                    - I];
                si (!b_status)
                {
                    ex = in ^ I;                       /* (0 1)(2 3) */
                }
                alioquin
                {
                    ex = III - in;                     /* (0 3)(1 2) */
                }
                summa += (ex + IV - in) % IV == I ? -(s64)I : (s64)I;
                visa[currens] = VERUM;
                visa[o] = VERUM;
                currens = IV * transitus + ex;
            }
            dum (currens != d);
            CREDO_VERUM(summa == (s64)IV || summa == -(s64)IV);
        }
    }
}

interior vacuum
_probare_gyrationes (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;

    sors_seminare(&s, (i64)MMXXIX, ZEPHYRUM);
    per (t = ZEPHYRUM; t < C; t++)
    {
               PiscinaNotatio  nota = piscina_notare(piscina);
        constans NodusTabulae* n;
                 constans i32* pd;
                          i32  transitus;
                          i32  ansae = sors_intra(&s, III);
                          i32  a;

        fac
        {
            n = tabula_nodorum_nodus(sors_intra(&s,
                tabula_nodorum_numerus()));
        }
        dum (n->transitus < II);
        pd         = tabula_nodorum_pd(n, piscina);
        transitus  = n->transitus;
        per (a = ZEPHYRUM; a < ansae; a++)
        {
            i32* novum = (i32*)piscina_allocare(piscina,
                (memoriae_index)IV * (transitus + I) * magnitudo(i32));

            _ansam_inserere(pd, transitus, I + sors_intra(&s, II
                * transitus), sors_intra(&s, IV), novum);
            pd = novum;
            transitus++;
        }
        _ansas_statuum_probare(pd, transitus, &s, piscina);
        piscina_reficere(piscina, nota);
    }
}

/* polynomium Laurentianum modulo p in a */
interior i32
_aestimare_modulo (
    Polynomium q,
           i32 a,
           i32 p)
{
    i32 summa = ZEPHYRUM;
    i32 inversa;
    s32 e;

    si (polynomium_est_nullum(q))
    {
        redde ZEPHYRUM;
    }
    CREDO_NECESSE(congruentia_inversa(a, p, &inversa));
    per (e = polynomium_gradus_imus(q); e
        <= polynomium_gradus_summus(q);
         e++)
    {
        i32 coefficiens =
            magnus_residuum_parvum(polynomium_coefficiens(q,
            e), p);

        summa = congruentia_adde(summa,
            congruentia_multiplica(coefficiens,
            e >= ZEPHYRUM ? congruentia_potentia(a, (i64)e, p)
            : congruentia_potentia(inversa, (i64)(-e), p), p), p);
    }
    redde summa;
}

/* puncta singula: valor = summa statuum in A modulo p (sine
 * interpolatione); d = 0 ad p = 17, omega = 5 (A = 2, A^4 = -1);
 * omega 0; primi inepti */
interior vacuum
_probare_puncta (
    CredoContextus* c)
{
                  Piscina* piscina  = c->piscina;
    constans NodusTabulae* n        = tabula_nodorum_quaere("8_19");
             constans i32* pd       = tabula_nodorum_pd(n, piscina);
     LaqueusModulusVertex* mv = laqueus_modulus_vertex_creare(pd,
         n->transitus, OCTETI_MAXIMI, piscina);
    Polynomium status;
           i32 p = (i32)998244353UL;
           i32 a = ZEPHYRUM;
           i32 v = ZEPHYRUM;
           i32 omega;

    CREDO_NECESSE_NON_NIHIL(mv);
    CREDO_NECESSE(laqueus_uncinus_ex_pd(pd, n->transitus, piscina,
        &status));
    per (omega = II; omega < XL; omega++)
    {
        CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv, omega,
            p,
            &a, &v), INTERPOLATIO_UTILE);
        CREDO_AEQUALIS_I32(v, _aestimare_modulo(status, a, p));
    }
    /* primus alius (i recomputatur), deinde redi */
    CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv, VII,
        (i32)4294967029UL, &a, &v), INTERPOLATIO_UTILE);
    CREDO_AEQUALIS_I32(v, _aestimare_modulo(status, a,
        (i32)4294967029UL));
    CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv, VII, p, &a,
        &v), INTERPOLATIO_UTILE);
    CREDO_AEQUALIS_I32(v, _aestimare_modulo(status, a, p));

    a = (i32)0xDEADUL;
    CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv, V, XVII, &a,
        &v), INTERPOLATIO_INUTILE);
    CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv, ZEPHYRUM, p,
        &a, &v), INTERPOLATIO_INUTILE);
    CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv, II, VII, &a,
        &v), INTERPOLATIO_ERRATUM);
    CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv, II,
        (i32)4294967291UL, &a, &v), INTERPOLATIO_ERRATUM);
    CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv, II, XXI, &a,
        &v), INTERPOLATIO_ERRATUM);
    CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(NIHIL, II, p,
        &a,
        &v), INTERPOLATIO_ERRATUM);
    CREDO_AEQUALIS_I32(a, (i32)0xDEADUL);

    /* p = 17, omega = 2: A = 13 * 4 = 1, d = 16 + 16^-1 = 16 + 16 = 15
     * != 0 -> valor summae statuum in A = 1 */
    CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv, II, XVII,
        &a,
        &v), INTERPOLATIO_UTILE);
    CREDO_AEQUALIS_I32(v, _aestimare_modulo(status, a, XVII));
}

/* codices invalidi et non plani refutati; c = 0 -> 1; officina
 * nimia refutata; sumptus */
interior vacuum
_probare_refutationes (
    CredoContextus* c)
{
         Piscina* piscina  = c->piscina;
    constans i32  unus[]   = { I, I, II, II };
    constans i32  virtualis[] = { I, III, II, IV, II, V, III, VI, IV, I,
        V, VI };
    constans i32 malus[] = { I, V, II, IV, III, I, IV, VII, V, III, VI,
        II };
    constans NodusTabulae* n  = tabula_nodorum_quaere("7_4");
    constans i32* pd          = tabula_nodorum_pd(n, piscina);
    Polynomium exitus         = polynomium_nullum();
    Polynomium signum = polynomium_constans(magnus_ex_s64(XLII),
        piscina);
    ContractioSumptus sumptus;

    exitus = signum;
    CREDO_FALSUM(laqueus_uncinus_ex_pd_rete(unus, I, OCTETI_MAXIMI,
        piscina, &exitus, NIHIL));
    CREDO_FALSUM(laqueus_uncinus_ex_pd_rete(virtualis, III,
        OCTETI_MAXIMI, piscina, &exitus, NIHIL));
    CREDO_FALSUM(laqueus_jones_ex_pd_rete(malus, III, OCTETI_MAXIMI,
        piscina, &exitus, NIHIL));
    CREDO_FALSUM(laqueus_jones_ex_pd_rete(pd, n->transitus, IV, piscina,
        &exitus, NIHIL));
    CREDO_FALSUM(laqueus_jones_ex_pd_rete(pd, n->transitus,
        OCTETI_MAXIMI, piscina, NIHIL, NIHIL));
    CREDO_VERUM(polynomium_aequalis(exitus, signum));
    CREDO_NIHIL(laqueus_modulus_vertex_creare(unus, I, OCTETI_MAXIMI,
        piscina));
    CREDO_NIHIL(laqueus_modulus_vertex_creare(virtualis, III,
        OCTETI_MAXIMI, piscina));
    CREDO_NIHIL(laqueus_modulus_vertex_creare(pd, n->transitus, IV,
        piscina));

    CREDO_VERUM(laqueus_uncinus_ex_pd_rete(NIHIL, ZEPHYRUM,
        OCTETI_MAXIMI, piscina, &exitus, &sumptus));
    CREDO_VERUM(polynomium_aequalis(exitus, polynomium_constans(
        magnus_ex_s64(I), piscina)));
    CREDO_AEQUALIS_I64(sumptus.apex, ZEPHYRUM);
    CREDO_VERUM(laqueus_jones_ex_pd_rete(NIHIL, ZEPHYRUM, OCTETI_MAXIMI,
        piscina, &exitus, NIHIL));

    CREDO_VERUM(laqueus_jones_ex_pd_rete(pd, n->transitus,
        OCTETI_MAXIMI,
        piscina, &exitus, &sumptus));
    CREDO_VERUM(polynomium_aequalis(exitus, _polynomium_textus(n->jones,
        piscina)));
    CREDO_VERUM(sumptus.apex > ZEPHYRUM);
    CREDO_VERUM(sumptus.multiplicationes > ZEPHYRUM);
}


/* ==================================================
 * Suita
 * ================================================== */

interior constans CredoSectio SECTIONES[] = {
    { "codices manu", _probare_codices_manu, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "tabula usque ad X", _probare_tabulam_x, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "ansae R1", _probare_ansas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ultra XXIV", _probare_ultra_limitem, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "polygona fortuita", _probare_laqueos_fortuitos, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "gyrationes", _probare_gyrationes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "puncta", _probare_puncta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "refutationes", _probare_refutationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "tabula tota", _probare_tabulam_totam, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("laqueus_rete", SECTIONES);
}
