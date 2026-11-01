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

/* occursus ad arma j intrans? infra X0; supra X3 (positivus) aut X1
 * (negativus) */
interior b32
_intrans (
    constans i32* x,
             i32  j,
             i32  m)
{
    s32 signum = _signum(x, m);

    redde j == ZEPHYRUM || (j == III && signum > ZEPHYRUM)
        || (j == I && signum < ZEPHYRUM);
}

/* summa connexa K1 # K2: os ultimum utriusque secatur - occursus
 * intrans m1 in K1 -> m1 + m2, occursus intrans m2 in K2 -> m1,
 * ceteri K2 + m1. Jones(K1 # K2) = Jones(K1) Jones(K2). */
interior vacuum
_summam_connexam (
    constans i32* pd_sinister,
             i32  c1,
    constans i32* pd_dexter,
             i32  c2,
             i32* novum)
{
    i32 m1 = II * c1;
    i32 m2 = II * c2;
    i32 k;

    per (k = ZEPHYRUM; k < IV * c1; k++)
    {
        novum[k] = pd_sinister[k] == m1
            && _intrans(pd_sinister + IV * (k / IV), k % IV,
            m1) ? m1 + m2 : pd_sinister[k];
    }
    per (k = ZEPHYRUM; k < IV * c2; k++)
    {
        novum[IV * c1 + k] = pd_dexter[k] == m2
            && _intrans(pd_dexter + IV * (k / IV),
            k % IV, m2) ? m1 : pd_dexter[k] + m1;
    }
}

/* speculum: positivus [a,b,c,d] -> [d,a,b,c], negativus -> [b,c,d,a]
 * (filum supra fit infra); Jones(K*)(t) = Jones(K)(1/t) */
interior vacuum
_speculum (
    constans i32* pd,
             i32  c,
             i32* novum)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < c; k++)
    {
        constans i32* x      = pd + IV * k;
                 i32  motus  = _signum(x, II * c) > ZEPHYRUM ? III : I;

        per (j = ZEPHYRUM; j < IV; j++)
        {
            novum[IV * k + j] = x[(j + motus) % IV];
        }
    }
}

/* ora rotata (l -> l + r modulo m) et transitus permixti: idem nodus,
 * ansa quaelibet ad transitum 0 venire potest */
interior vacuum
_permiscere (
        i32* pd,
        i32  c,
       Sors* s)
{
    i32 m = II * c;
    i32 r = sors_intra(s, m);
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < IV * c; k++)
    {
        pd[k] = (pd[k] - I + r) % m + I;
    }
    per (k = c; k > I; k--)
    {
        i32 q = sors_intra(s, k);

        per (j = ZEPHYRUM; j < IV; j++)
        {
            i32 x = pd[IV * (k - I) + j];

            pd[IV * (k - I) + j]  = pd[IV * q + j];
            pd[IV * q + j]        = x;
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


/* summa valorum absolutorum coefficientium */
interior Magnus
_summa_absoluta (
    Polynomium  q,
       Piscina* piscina)
{
    Magnus summa = magnus_ex_s64((s64)ZEPHYRUM);
       s32 e;

    si (polynomium_est_nullum(q))
    {
        redde summa;
    }
    per (e = polynomium_gradus_imus(q); e
        <= polynomium_gradus_summus(q);
         e++)
    {
        summa = magnus_adde(summa, magnus_absolutum(
            polynomium_coefficiens(q, e), piscina), piscina);
    }
    redde summa;
}

/* Sigma |c_k| (Jones = uncinus in valoribus absolutis) <= B, limes =
 * bita(B) <= 2c; si limes_exspectatus != 0, aequalis */
interior vacuum
_limitem_probare (
    constans i32* pd,
             i32  c,
       Polynomium jones,
             i32  limes_exspectatus,
             s64  b_exspectatum,
         Piscina* piscina)
{
    LaqueusModulusVertex* mv = laqueus_modulus_vertex_creare(pd, c,
        OCTETI_MAXIMI, piscina);
    Magnus b;
       i32 limes;

    CREDO_NECESSE_NON_NIHIL(mv);
    CREDO_NECESSE(laqueus_modulus_vertex_limes_exactus(mv, &b));
    limes = laqueus_modulus_vertex_limes(mv);
    CREDO_AEQUALIS_I32(limes, magnus_bitorum(b));
    CREDO_VERUM(magnus_compara(_summa_absoluta(jones, piscina), b)
        <= ZEPHYRUM);
    CREDO_VERUM(limes <= II * c);
    si (c >= III)
    {
        CREDO_VERUM(limes < II * c);
    }
    si (limes_exspectatus != ZEPHYRUM)
    {
        CREDO_AEQUALIS_I32(limes, limes_exspectatus);
        CREDO_VERUM(magnus_aequalis(b, magnus_ex_s64(b_exspectatum)));
    }
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
    constans i32  u3[]     = { II, I, III, II, IV, III, I, IV };  /* facies 0
                                                          * monogonum */
    constans i32 t1[] = { I, VII, II, VI, VII, III, VIII, II, III, I,
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
    _uncinum_comparare(u3, II, piscina);
    _uncinum_comparare(t1, IV, piscina);
    _uncinum_comparare(t2, IV, piscina);
    _jones_comparare(u1, II, unum, piscina);
    _jones_comparare(u2, II, unum, piscina);
    _jones_comparare(u3, II, unum, piscina);
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
        _permiscere(pd, transitus, &s);
        _jones_comparare(pd, transitus, _polynomium_textus(n->jones,
            piscina), piscina);
        _limitem_probare(pd, transitus, _polynomium_textus(n->jones,
            piscina), ZEPHYRUM, ZEPHYRUM, piscina);
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


/* nodus tabulae fortuitus cum transitibus in [imus, summus] */
interior constans NodusTabulae*
_nodum_eligere (
     Sors* s,
      i32  imus,
      i32  summus)
{
    constans NodusTabulae* n;

    fac
    {
        n = tabula_nodorum_nodus(sors_intra(s,
            tabula_nodorum_numerus()));
    }
    dum (n->transitus < imus || n->transitus > summus);
    redde n;
}

/* summae connexae: XXX summae II..V nodorum (III..IX transitus), una
 * XII nodorum (IX..XIII, c >= C); quisque fortuito speculatus, ansa
 * una, ora rotata et transitus permixti. Jones = productum Jones
 * partium (speculum: t -> 1/t). Summa magna coefficientem > 31
 * bitorum habet: restitutio trans primos plures vere probatur. */
interior vacuum
_probare_summas (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;

    sors_seminare(&s, (i64)MMXXX, ZEPHYRUM);
    per (t = ZEPHYRUM; t <= XXX; t++)
    {
        PiscinaNotatio nota   = piscina_notare(piscina);
                   b32 magna  = t == XXX;
                   i32 partes = magna ? XII : II
                       + sors_intra(&s, IV);
          constans i32* pd         = NIHIL;
                   i32  transitus  = ZEPHYRUM;
            Polynomium  exspectatum =
                polynomium_constans(magnus_ex_s64(I),
                piscina);
        Polynomium  jones;
               i32* novum;
               i32  q;

        per (q = ZEPHYRUM; q < partes; q++)
        {
            constans NodusTabulae* n = _nodum_eligere(&s,
                magna ? IX : III,
                magna ? XIII : IX);
            constans i32* pars  = tabula_nodorum_pd(n, piscina);
                     i32  ct    = n->transitus;
              Polynomium  j = _polynomium_textus(n->jones,
                  piscina);

            si (sors_intra(&s, II) == ZEPHYRUM)
            {
                i32* speculum = (i32*)piscina_allocare(piscina,
                    (memoriae_index)IV * ct * magnitudo(i32));

                _speculum(pars, ct, speculum);
                pars = speculum;
                CREDO_NECESSE(polynomium_dilata(j, -(s32)I, piscina,
                    &j));
            }
            si (pd == NIHIL)
            {
                pd         = pars;
                transitus  = ct;
            }
            alioquin
            {
                novum = (i32*)piscina_allocare(piscina,
                    (memoriae_index)IV * (transitus
                        + ct) * magnitudo(i32));
                _summam_connexam(pd, transitus, pars, ct, novum);
                pd         = novum;
                transitus  += ct;
            }
            CREDO_NECESSE(polynomium_multiplica(exspectatum, j, piscina,
                &exspectatum));
        }
        novum = (i32*)piscina_allocare(piscina,
            (memoriae_index)IV * (transitus + I) * magnitudo(i32));
        _ansam_inserere(pd, transitus, I + sors_intra(&s,
            II * transitus),
            sors_intra(&s, IV), novum);
        transitus++;
        _permiscere(novum, transitus, &s);
        CREDO_NECESSE(laqueus_jones_ex_pd_rete(novum, transitus,
            OCTETI_MAXIMI, piscina, &jones, NIHIL));
        CREDO_VERUM(polynomium_aequalis(jones, exspectatum));
        _limitem_probare(novum, transitus, jones, ZEPHYRUM, ZEPHYRUM,
            piscina);
        si (magna)
        {
            i32 bita = ZEPHYRUM;
            s32 e;

            CREDO_VERUM(transitus >= C);
            per (e = polynomium_gradus_imus(jones);
                 e <= polynomium_gradus_summus(jones); e++)
            {
                i32 b = magnus_bitorum(polynomium_coefficiens(jones,
                    e));

                si (b > bita)
                {
                    bita = b;
                }
            }
            CREDO_VERUM(bita > XXXI);
        }
        piscina_reficere(piscina, nota);
    }
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

/* filius CREDO_NON_PENDET: modulus compositus prope 2^32 */
interior vacuum
_compositum_magnum (
    LaqueusModulusVertex* mv)
{
    i32 a;
    i32 v;

    (vacuum)laqueus_modulus_vertex_aestimare(mv, II, (i32)4294967289UL,
        &a, &v);
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
    /* moduli compositi 1 mod 4: 85 = 5 * 17 (g = 13 olim falso
     * acceptum), 2^32 - 7 = 3 * ... (nullum g: olim ~p conatus) */
    CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv, V, LXXXV,
        &a,
        &v), INTERPOLATIO_ERRATUM);
    CREDO_NON_PENDET(_compositum_magnum(mv), MM);
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
 * Limes arborum (contractio-plan T5a)
 * ================================================== */

/* clausura tricae per bibliothecam (laqueus_pd_ex_plexu); NIHIL si
 * refutata */
interior i32*
_plexus_pd (
    constans s32* verbum,
             i32  longitudo,
             i32  n,
         Piscina* piscina)
{
    i32* pd;
    i32  transitus;

    redde laqueus_pd_ex_plexu(verbum, longitudo, n, piscina, &pd,
        &transitus) ? pd : NIHIL;
}

/* torus T(2, n): clausura sigma_1^n (n impar; n < 0: sigma_1^-|n|) */
interior i32*
_torus_duo (
         s32  n,
     Piscina* piscina)
{
    i32  longitudo = (i32)(n > ZEPHYRUM ? n : -n);
    s32* verbum    = (s32*)piscina_allocare(piscina,
        (memoriae_index)longitudo * magnitudo(s32));
    i32 k;

    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        verbum[k] = n > ZEPHYRUM ? (s32)I : -(s32)I;
    }
    redde _plexus_pd(verbum, longitudo, II, piscina);
}

/* valores manu (recensio T5a): stricti T(2,31) et T(2,7) # T(2,-9),
 * ubi Sigma |c| = B = 2^limes - 1; nodi parvi; ansa; summa trifolii */
interior vacuum
_probare_limitem_manu (
    CredoContextus* c)
{
         Piscina* piscina  = c->piscina;
    constans i32  u1[]     = { I, I, II, IV, II, IV, III, III };
    constans i32  t1[] = { I, VII, II, VI, VII, III, VIII, II, III, I,
        IV,
        VIII, IV, VI, V, V };
    constans character* nomina[] = { "3_1", "4_1", "5_1", "5_2",
        "7_1" };
             constans s64  b_tabulae[]  = { III, VI, V, VIII, VII };
             constans i32  limites[]    = { II, III, III, IV, III };
               Polynomium  jones;
                      i32* pd;
                      i32* septem;
                      i32* novem;
    constans NodusTabulae* trifolium = tabula_nodorum_quaere("3_1");
                      i32  k;

    pd = _torus_duo((s32)XXXI, piscina);
    CREDO_NECESSE_NON_NIHIL(pd);
    CREDO_NECESSE(laqueus_jones_ex_pd_rete(pd, XXXI, OCTETI_MAXIMI,
        piscina, &jones, NIHIL));
    CREDO_VERUM(magnus_aequalis(_summa_absoluta(jones, piscina),
        magnus_ex_s64((s64)XXXI)));
    _limitem_probare(pd, XXXI, jones, V, (s64)XXXI, piscina);

    septem  = _torus_duo((s32)VII, piscina);
    novem   = _torus_duo(-(s32)IX, piscina);
    CREDO_NECESSE_NON_NIHIL(septem);
    CREDO_NECESSE_NON_NIHIL(novem);
    pd = (i32*)piscina_allocare(piscina, (memoriae_index)IV * XVI
        * magnitudo(i32));
    _summam_connexam(septem, VII, novem, IX, pd);
    CREDO_NECESSE(laqueus_jones_ex_pd_rete(pd, XVI, OCTETI_MAXIMI,
        piscina, &jones, NIHIL));
    CREDO_VERUM(magnus_aequalis(_summa_absoluta(jones, piscina),
        magnus_ex_s64((s64)LXIII)));
    _limitem_probare(pd, XVI, jones, VI, (s64)LXIII, piscina);

    _limitem_probare(u1, II, polynomium_constans(magnus_ex_s64(I),
        piscina), I, (s64)I, piscina);
    _limitem_probare(t1, IV, _polynomium_textus(trifolium->jones,
        piscina), II, (s64)III, piscina);
    per (k = ZEPHYRUM; k < V; k++)
    {
        constans NodusTabulae* n = tabula_nodorum_quaere(nomina[k]);

        _limitem_probare(tabula_nodorum_pd(n, piscina), n->transitus,
            _polynomium_textus(n->jones, piscina), limites[k],
            b_tabulae[k],
            piscina);
    }
    {
        constans i32* p3 = tabula_nodorum_pd(trifolium, piscina);

        pd = (i32*)piscina_allocare(piscina, (memoriae_index)IV * VI
            * magnitudo(i32));
        _summam_connexam(p3, III, p3, III, pd);
        CREDO_NECESSE(polynomium_multiplica(_polynomium_textus(
            trifolium->jones, piscina), _polynomium_textus(
            trifolium->jones, piscina), piscina, &jones));
        _limitem_probare(pd, VI, jones, IV, (s64)IX, piscina);
    }
}

/* omnes nodi tabulae: Sigma |c| <= B, limes < 2c */
interior vacuum
_probare_limitem_tabulae (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        i32  k;

    per (k = ZEPHYRUM; k < tabula_nodorum_numerus(); k++)
    {
        constans NodusTabulae* n = tabula_nodorum_nodus(k);
               PiscinaNotatio  nota;

        si (n->transitus < II)
        {
            perge;
        }
        nota = piscina_notare(piscina);
        _limitem_probare(tabula_nodorum_pd(n, piscina), n->transitus,
            _polynomium_textus(n->jones, piscina), ZEPHYRUM, ZEPHYRUM,
            piscina);
        piscina_reficere(piscina, nota);
    }
}

/* clausurae tricarum fortuitae (3..6 fila, 8..82 transitus): Sigma
 * |c| <= B; usque ad XVI transitus uncinus = summa statuum (oraculum
 * alienum); limes VERE in usu: primi = ceil((limes + 1)/31) + 1
 * (omnes primi > 2^31, unus additus), pauciores quam 2c poscit ubi
 * c >= XL */
interior vacuum
_probare_limitem_tricarum (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;

    sors_seminare(&s, (i64)MMXXXI, ZEPHYRUM);
    per (t = ZEPHYRUM; t < XL; t++)
    {
        PiscinaNotatio  nota       = piscina_notare(piscina);
                   i32  fila       = III + sors_intra(&s, IV);
                   i32  longitudo  = VIII + sors_intra(&s, LXXV);
                   s32* verbum;
                   i32* pd = NIHIL;
                   i32  conatus;
                   i32  k;

        si ((longitudo + fila - I) % II != ZEPHYRUM)
        {
            longitudo++;
        }
        verbum = (s32*)piscina_allocare(piscina,
            (memoriae_index)longitudo * magnitudo(s32));
        per (conatus = ZEPHYRUM; conatus < M && pd == NIHIL; conatus++)
        {
            per (k = ZEPHYRUM; k < longitudo; k++)
            {
                s32 g = (s32)(I + sors_intra(&s, fila - I));

                verbum[k] = sors_intra(&s, II) == ZEPHYRUM ? g : -g;
            }
            pd = _plexus_pd(verbum, longitudo, fila, piscina);
        }
        CREDO_NECESSE_NON_NIHIL(pd);
        {
            LaqueusModulusVertex* mv = laqueus_modulus_vertex_creare(pd,
                longitudo, OCTETI_MAXIMI, piscina);
            Polynomium uncinus;
                   i32 limes;
                   i32 primi;

            CREDO_NECESSE_NON_NIHIL(mv);
            CREDO_NECESSE(laqueus_modulus_vertex_uncinus(mv, piscina,
                &uncinus));
            limes = laqueus_modulus_vertex_limes(mv);
            laqueus_modulus_vertex_vocationes(mv, NIHIL, &primi);
            CREDO_AEQUALIS_I32(primi, (limes + I + XXX) / XXXI + I);
            CREDO_VERUM(primi <= (II * longitudo + I + XXX) / XXXI + I);
            si (longitudo >= XL)
            {
                CREDO_VERUM(primi < (II * longitudo + I + XXX) / XXXI
                    + I);
            }
            _limitem_probare(pd, longitudo, uncinus, ZEPHYRUM, ZEPHYRUM,
                piscina);
            si (longitudo <= XVI)
            {
                _uncinum_comparare(pd, longitudo, piscina);
            }
        }
        piscina_reficere(piscina, nota);
    }
}


/* ordo optimus VERE in usu (contractio-plan T5b): clausura tricae VIII
 * filorum supra limen - victor non avidus, sumptus retis = simulatus
 * victoris < avidi; valores uncinus in punctis = valores copiae ora
 * rotata et transitibus permixtis (rete aliud, ordo alius) */
interior vacuum
_probare_ordinem_optimum (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        s32  verbum[CLI];
        i32* pd = NIHIL;
        i32  conatus;
        i32  k;

    sors_seminare(&s, (i64)MMXXXV, ZEPHYRUM);
    per (conatus = ZEPHYRUM; conatus < M && pd == NIHIL; conatus++)
    {
        per (k = ZEPHYRUM; k < CLI; k++)
        {
            s32 g = (s32)(I + sors_intra(&s, VII));

            verbum[k] = sors_intra(&s, II) == ZEPHYRUM ? g : -g;
        }
        pd = _plexus_pd(verbum, CLI, VIII, piscina);
    }
    CREDO_NECESSE_NON_NIHIL(pd);
    {
        LaqueusModulusVertex* mv = laqueus_modulus_vertex_creare(pd,
            CLI,
            OCTETI_MAXIMI, piscina);
        constans ContractioOrdo* ordo;
              ContractioSumptus  avidus;
              ContractioSumptus  victoris;
                            i32  victor;

        CREDO_NECESSE_NON_NIHIL(mv);
        ordo    = laqueus_modulus_vertex_ordo(mv);
        victor  = contractio_ordo_candidatus(ordo);
        CREDO_NECESSE(contractio_ordo_candidati(ordo,
            CONTRACTIO_CANDIDATUS_AVIDUS, &avidus));
        CREDO_NECESSE(avidus.multiplicationes
            >= CONTRACTIO_LIMEN_ORDINARIUM);
        CREDO_VERUM(victor != CONTRACTIO_CANDIDATUS_AVIDUS);
        CREDO_NECESSE(contractio_ordo_candidati(ordo, victor,
            &victoris));
        CREDO_AEQUALIS_I64(laqueus_modulus_vertex_sumptus(mv).multiplicationes,
            victoris.multiplicationes);
        CREDO_VERUM(victoris.multiplicationes
            < avidus.multiplicationes);
    }
    {
        /* copia: ora rotata et transitus permixti - idem diagramma, idem
         * uncinus, rete et ordo alii. Valores in punctis aequales. */
        i32* copia = (i32*)piscina_allocare(piscina,
            (memoriae_index)IV * CLI * magnitudo(i32));
        LaqueusModulusVertex* mv = laqueus_modulus_vertex_creare(pd,
            CLI,
            OCTETI_MAXIMI, piscina);
        LaqueusModulusVertex* mv_copiae;
                         i32  omega;

        per (k = ZEPHYRUM; k < IV * CLI; k++)
        {
            copia[k] = pd[k];
        }
        _permiscere(copia, CLI, &s);
        mv_copiae = laqueus_modulus_vertex_creare(copia, CLI,
            OCTETI_MAXIMI,
            piscina);
        CREDO_NECESSE_NON_NIHIL(mv);
        CREDO_NECESSE_NON_NIHIL(mv_copiae);
        per (omega = II; omega < VII; omega++)
        {
            i32 a1;
            i32 v1;
            i32 a2;
            i32 v2;

            CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv,
                omega,
                (i32)4294967197UL, &a1, &v1), INTERPOLATIO_UTILE);
            CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv_copiae,
                omega, (i32)4294967197UL, &a2, &v2),
                INTERPOLATIO_UTILE);
            CREDO_AEQUALIS_I32(a1, a2);
            CREDO_AEQUALIS_I32(v1, v2);
        }
    }
}

/* nodus toroidalis T(7, 8) ut clausura (sigma_1 .. sigma_6)^8 (XLVIII
 * transitus positivi): Jones = t^((p-1)(q-1)/2) (1 - t^(p+1) - t^(q+1)
 * + t^(p+q)) / (1 - t^2) - oraculum clausum ultra summam statuum;
 * conventio signi laqueus_pd_ex_plexu ita probatur */
interior vacuum
_probare_torum_septem_octo (
    CredoContextus* c)
{
       Piscina* piscina = c->piscina;
           s32  verbum[XLVIII];
           i32* pd;
    Polynomium  numerator    = polynomium_nullum();
    Polynomium  denominator  = polynomium_nullum();
    Polynomium  monomium     = polynomium_nullum();
    Polynomium  quotiens;
    Polynomium  formula;
    Polynomium  speculum;
    Polynomium  jones;
        Magnus  coef[XVI];
           i32  k;

    per (k = ZEPHYRUM; k < XLVIII; k++)
    {
        verbum[k] = (s32)(k % VI + I);
    }
    pd = _plexus_pd(verbum, XLVIII, VII, piscina);
    CREDO_NECESSE_NON_NIHIL(pd);
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        coef[k] = magnus_ex_s64((s64)ZEPHYRUM);
    }
    coef[0]     = magnus_ex_s64((s64)I);
    coef[VIII]  = magnus_ex_s64(-(s64)I);
    coef[IX]    = magnus_ex_s64(-(s64)I);
    coef[XV]    = magnus_ex_s64((s64)I);
    CREDO_NECESSE(polynomium_ex_coefficientibus(coef, XVI, ZEPHYRUM,
        piscina, &numerator));
    coef[0]   = magnus_ex_s64((s64)I);
    coef[I]   = magnus_ex_s64((s64)ZEPHYRUM);
    coef[II]  = magnus_ex_s64(-(s64)I);
    CREDO_NECESSE(polynomium_ex_coefficientibus(coef, III, ZEPHYRUM,
        piscina, &denominator));
    CREDO_NECESSE(polynomium_divide_exacte(numerator, denominator,
        piscina,
        &quotiens));
    CREDO_NECESSE(polynomium_monomium(magnus_ex_s64((s64)I), (s32)XXI,
        piscina, &monomium));
    CREDO_NECESSE(polynomium_multiplica(quotiens, monomium, piscina,
        &formula));
    CREDO_NECESSE(polynomium_dilata(formula, -(s32)I, piscina,
        &speculum));
    CREDO_NECESSE(laqueus_jones_ex_pd_rete(pd, XLVIII, OCTETI_MAXIMI,
        piscina, &jones, NIHIL));
    CREDO_VERUM(polynomium_aequalis(jones, formula));
    CREDO_FALSUM(polynomium_aequalis(jones, speculum));
}


/* Jones clausurae tricae */
interior Polynomium
_jones_plexus (
    constans s32* verbum,
             i32  longitudo,
             i32  fila,
         Piscina* piscina)
{
           i32* pd;
           i32  transitus;
    Polynomium  jones = polynomium_nullum();

    CREDO_NECESSE(laqueus_pd_ex_plexu(verbum, longitudo, fila, piscina,
        &pd,
        &transitus));
    CREDO_AEQUALIS_I32(transitus, longitudo);
    CREDO_NECESSE(laqueus_jones_ex_pd_rete(pd, transitus, OCTETI_MAXIMI,
        piscina, &jones, NIHIL));
    redde jones;
}

/* laqueus_pd_ex_plexu: trifolium dextrum sigma_1^3 = 3_1 tabulae,
 * sinistrum speculum; motus Markov (coniugatio, rotatio, stabilizatio
 * +-) et insertio sigma sigma^-1 Jones non mutant; usque ad XVI
 * transitus uncinus = summa statuum; refutationes */
interior vacuum
_probare_plexum (
    CredoContextus* c)
{
         Piscina* piscina      = c->piscina;
    constans s32  dextrum[]    = { 1, 1, 1 };
    constans s32  sinistrum[]  = { -1, -1, -1 };
    constans s32  hopf[]       = { 1, 1 };
    constans s32  triplex[]    = { 1, 2, 1, 2, 1, 2 };
    constans s32  intactum[]   = { 1, 1, 1 };
    constans s32  nulla[]      = { 0, 1, 1 };
    constans s32  ultra[]      = { 1, 3, 1 };
    constans s32  unus[]       = { 1 };
      Polynomium  trifolium = _polynomium_textus(
          tabula_nodorum_quaere("3_1")->jones, piscina);
    Polynomium  speculum;
           i32  signum     = (i32)0xBEEFUL;
           i32* pd         = &signum;
           i32  transitus  = (i32)0xDEADUL;
          Sors  s;
           i32  t;

    CREDO_VERUM(polynomium_aequalis(_jones_plexus(dextrum, III, II,
        piscina), trifolium));
    CREDO_NECESSE(polynomium_dilata(trifolium, -(s32)I, piscina,
        &speculum));
    CREDO_VERUM(polynomium_aequalis(_jones_plexus(sinistrum, III, II,
        piscina), speculum));

    /* refutationes: exitus intacti */
    CREDO_FALSUM(laqueus_pd_ex_plexu(hopf, II, II, piscina, &pd,
        &transitus));
    CREDO_FALSUM(laqueus_pd_ex_plexu(triplex, VI, III, piscina, &pd,
        &transitus));
    CREDO_FALSUM(laqueus_pd_ex_plexu(intactum, III, III, piscina, &pd,
        &transitus));
    CREDO_FALSUM(laqueus_pd_ex_plexu(nulla, III, II, piscina, &pd,
        &transitus));
    CREDO_FALSUM(laqueus_pd_ex_plexu(ultra, III, III, piscina, &pd,
        &transitus));
    CREDO_FALSUM(laqueus_pd_ex_plexu(dextrum, III, ZEPHYRUM, piscina,
        &pd,
        &transitus));
    CREDO_FALSUM(laqueus_pd_ex_plexu(NIHIL, ZEPHYRUM, II, piscina, &pd,
        &transitus));
    CREDO_FALSUM(laqueus_pd_ex_plexu(NIHIL, III, II, piscina, &pd,
        &transitus));
    CREDO_AEQUALIS_I32(transitus, (i32)0xDEADUL);
    CREDO_VERUM(pd == &signum);

    /* nodus trivialis: filum unum, verbum vacuum; transitus unus */
    CREDO_VERUM(laqueus_pd_ex_plexu(NIHIL, ZEPHYRUM, I, piscina, &pd,
        &transitus));
    CREDO_AEQUALIS_I32(transitus, ZEPHYRUM);
    CREDO_NIHIL(pd);
    CREDO_VERUM(laqueus_pd_ex_plexu(unus, I, II, piscina, &pd,
        &transitus));
    CREDO_AEQUALIS_I32(transitus, I);

    /* signa mixta in sigma_2 (recensio): 5_1 = sigma_1^5, 5_2 =
     * sigma_1^3 sigma_2 sigma_1^-1 sigma_2, ambo tabulae sine speculo */
    {
        constans s32 quinque[]    = { 1, 1, 1, 1, 1 };
        constans s32 quinque_b[]  = { 1, 1, 1, 2, -1, 2 };

        CREDO_VERUM(polynomium_aequalis(_jones_plexus(quinque, V, II,
            piscina),
            _polynomium_textus(tabula_nodorum_quaere("5_1")->jones,
            piscina)));
        CREDO_VERUM(polynomium_aequalis(_jones_plexus(quinque_b, VI,
            III,
            piscina),
            _polynomium_textus(tabula_nodorum_quaere("5_2")->jones,
            piscina)));
    }

    /* transitus unus: codices exacti */
    {
        constans s32 negativus[] = { -1 };

        CREDO_NECESSE(laqueus_pd_ex_plexu(unus, I, II, piscina, &pd,
            &transitus));
        CREDO_AEQUALIS_I32(pd[0], I);
        CREDO_AEQUALIS_I32(pd[I], I);
        CREDO_AEQUALIS_I32(pd[II], II);
        CREDO_AEQUALIS_I32(pd[III], II);
        CREDO_NECESSE(laqueus_pd_ex_plexu(negativus, I, II, piscina,
            &pd,
            &transitus));
        CREDO_AEQUALIS_I32(pd[0], II);
        CREDO_AEQUALIS_I32(pd[I], I);
        CREDO_AEQUALIS_I32(pd[II], I);
        CREDO_AEQUALIS_I32(pd[III], II);
    }

    /* fila absurda (olim ossa involvebantur) et littera INT_MIN */
    {
        constans s32 angustum[] = { 1, 2 };
                 s32 minimum[I];

        minimum[0]  = -(s32)0x7FFFFFFFL - (s32)I;
        signum      = (i32)0xBEEFUL;
        pd          = &signum;
        transitus   = (i32)0xDEADUL;
        CREDO_FALSUM(laqueus_pd_ex_plexu(unus, I, (i32)0xFFFFFFFFUL,
            piscina,
            &pd, &transitus));
        CREDO_FALSUM(laqueus_pd_ex_plexu(angustum, II, V, piscina, &pd,
            &transitus));
        CREDO_FALSUM(laqueus_pd_ex_plexu(minimum, I, III, piscina, &pd,
            &transitus));
        CREDO_AEQUALIS_I32(transitus, (i32)0xDEADUL);
        CREDO_VERUM(pd == &signum);
    }

    /* relationes: R3 sigma_i sigma_i+1 sigma_i = sigma_i+1 sigma_i
     * sigma_i+1, commutatio longinqua sigma_i sigma_j = sigma_j sigma_i
     * (|i - j| >= 2); fila IV..VIII */
    sors_seminare(&s, (i64)MMXXXVII, ZEPHYRUM);
    per (t = ZEPHYRUM; t < XX; t++)
    {
        PiscinaNotatio  nota       = piscina_notare(piscina);
                   i32  fila       = IV + sors_intra(&s, V);
                   i32  longitudo  = XVI + sors_intra(&s, XVI);
                   s32  primum[XL];
                   s32  secundum[XL];
                   i32* probatum  = NIHIL;
                   i32  probati;
                   i32  conatus;
                   i32  r;
                   i32  k;
                   s32  g;
                   s32  h;
                   b32  tria = t % II == ZEPHYRUM;

        /* longitudo totalis = longitudo + 3 (R3) aut + 2: paritas */
        si ((longitudo + (tria ? III : II) + fila - I) % II != ZEPHYRUM)
        {
            longitudo++;
        }
        per (conatus = ZEPHYRUM; conatus < M
            && probatum == NIHIL; conatus++)
        {
            i32 gi;
            i32 hi;

            r = sors_intra(&s, longitudo + I);
            si (tria)
            {
                gi = I + sors_intra(&s, fila - II);       /* 1..fila-2 */
                hi = gi + I;
            }
            alioquin
            {
                gi = I + sors_intra(&s, fila - III);      /* 1..fila-3 */
                hi = gi + II + sors_intra(&s, fila - gi - II);
            }
            g = (s32)gi;
            h = (s32)hi;
            per (k = ZEPHYRUM; k < longitudo; k++)
            {
                s32 x = (s32)(I + sors_intra(&s, fila - I));

                primum[k < r ? k : k + (tria ? III : II)] =
                    sors_intra(&s, II) == ZEPHYRUM ? x : -x;
            }
            si (tria)
            {
                primum[r] = g; primum[r + I] = h; primum[r + II] = g;
            }
            alioquin
            {
                primum[r] = g; primum[r + I] = h;
            }
            per (k = ZEPHYRUM; k < longitudo + (tria ? III : II); k++)
            {
                secundum[k] = primum[k];
            }
            si (tria)
            {
                secundum[r] = h; secundum[r + I] = g; secundum[r + II] =
                                                          h;
            }
            alioquin
            {
                secundum[r] = h; secundum[r + I] = g;
            }
            si (!laqueus_pd_ex_plexu(primum, longitudo
                + (tria ? III : II),
                fila, piscina, &probatum, &probati))
            {
                probatum = NIHIL;
            }
        }
        CREDO_NECESSE_NON_NIHIL(probatum);
        CREDO_VERUM(polynomium_aequalis(_jones_plexus(primum,
            longitudo + (tria ? III : II), fila, piscina),
            _jones_plexus(
            secundum, longitudo + (tria ? III : II), fila, piscina)));
        piscina_reficere(piscina, nota);
    }

    /* motus Markov */
    sors_seminare(&s, (i64)MMXXXVI, ZEPHYRUM);
    per (t = ZEPHYRUM; t < XL; t++)
    {
        PiscinaNotatio  nota       = piscina_notare(piscina);
                   i32  fila       = III + sors_intra(&s, IV);
                   i32  longitudo  = X + sors_intra(&s, XXXI);
                   s32  verbum[XLIV];
                   s32  alterum[XLIV];
                   i32* probatum  = NIHIL;
                   i32  probati;
                   i32  conatus;
                   i32  k;
                   i32  r;
                   s32  g;
            Polynomium  jones;

        si ((longitudo + fila - I) % II != ZEPHYRUM)
        {
            longitudo++;
        }
        per (conatus = ZEPHYRUM; conatus < M
            && probatum == NIHIL; conatus++)
        {
            per (k = ZEPHYRUM; k < longitudo; k++)
            {
                g          = (s32)(I + sors_intra(&s, fila - I));
                verbum[k]  = sors_intra(&s, II) == ZEPHYRUM ? g : -g;
            }
            si (!laqueus_pd_ex_plexu(verbum, longitudo, fila, piscina,
                &probatum, &probati))
            {
                probatum = NIHIL;
            }
        }
        CREDO_NECESSE_NON_NIHIL(probatum);
        jones = _jones_plexus(verbum, longitudo, fila, piscina);
        si (longitudo <= XVI)
        {
            _uncinum_comparare(probatum, longitudo, piscina);
        }

        /* coniugatio: g w g^-1 */
        g           = (s32)(I + sors_intra(&s, fila - I));
        alterum[0]  = g;
        per (k = ZEPHYRUM; k < longitudo; k++)
        {
            alterum[k + I] = verbum[k];
        }
        alterum[longitudo + I] = -g;
        CREDO_VERUM(polynomium_aequalis(_jones_plexus(alterum,
            longitudo + II, fila, piscina), jones));

        /* rotatio cyclica */
        r = sors_intra(&s, longitudo);
        per (k = ZEPHYRUM; k < longitudo; k++)
        {
            alterum[k] = verbum[(k + r) % longitudo];
        }
        CREDO_VERUM(polynomium_aequalis(_jones_plexus(alterum,
            longitudo,
            fila, piscina), jones));

        /* stabilizatio +- : w sigma_fila^(+-1) in fila + 1 */
        per (k = ZEPHYRUM; k < longitudo; k++)
        {
            alterum[k] = verbum[k];
        }
        alterum[longitudo] = sors_intra(&s, II) == ZEPHYRUM ? (s32)fila
            : -(s32)fila;
        CREDO_VERUM(polynomium_aequalis(_jones_plexus(alterum,
            longitudo + I, fila + I, piscina), jones));

        /* insertio sigma sigma^-1 (R2) */
        r = sors_intra(&s, longitudo + I);
        g = (s32)(I + sors_intra(&s, fila - I));
        per (k = ZEPHYRUM; k < longitudo + II; k++)
        {
            alterum[k] = k < r ? verbum[k] : (k == r ? g : (k == r
                + I ? -g
                : verbum[k - II]));
        }
        CREDO_VERUM(polynomium_aequalis(_jones_plexus(alterum,
            longitudo + II, fila, piscina), jones));
        piscina_reficere(piscina, nota);
    }
}


/* ==================================================
 * Fenestra et limes aurei (temperley-lieb-plan T2a)
 *
 * Valores capti ex codice ANTE refactionem (be6be402: fenestra et
 * limes ex structura moduli vertex lecti, lib/laqueus.c inclusa in
 * programmate scabro); laqueus_pd_fenestra eosdem reddere debet. Tricae
 * magnae = tricae D136 (semen MMXXVIII, paritas aucta, generator R0),
 * fenestrae et limites ut in effusione D136.
 * ================================================== */

interior i32
_misce (
    i32 h,
    s32 v)
{
    i32 u = (i32)v;
    i32 b;

    per (b = ZEPHYRUM; b < IV; b++)
    {
        h ^= (u >> (VIII * b)) & (i32)0xFFUL;
        h *= (i32)16777619UL;
    }
    redde h;
}

/* generator R0 (tools/uncinus_mensura.c, _plexus_fortuitus) */
interior i32*
_plexus_aureus (
        Sors*  s,
         i32   fila,
         i32*  longitudo,
     Piscina*  piscina,
         s32** verbum_exitus)
{
    s32* verbum;
    i32  conatus;

    si ((*longitudo + fila - I) % II != ZEPHYRUM)
    {
        (*longitudo)++;
    }
    verbum = (s32*)piscina_allocare(piscina, (memoriae_index)*longitudo
        * magnitudo(s32));
    per (conatus = ZEPHYRUM; conatus < M; conatus++)
    {
        PiscinaNotatio  nota = piscina_notare(piscina);
                   i32* pd;
                   i32  transitus;
                   i32  k;

        per (k = ZEPHYRUM; k < *longitudo; k++)
        {
            s32 g = (s32)(I + sors_intra(s, fila - I));

            verbum[k] = sors_intra(s, II) == ZEPHYRUM ? g : -g;
        }
        si (laqueus_pd_ex_plexu(verbum, *longitudo, fila, piscina, &pd,
            &transitus))
        {
            *verbum_exitus = verbum;
            redde pd;
        }
        piscina_reficere(piscina, nota);
    }
    redde NIHIL;
}

nomen structura {
    constans character* titulus;
                   s32  imus;
                   s32  summus;
                   i32  limes;
                   s64  exactus;
} FenestraAurea;

nomen structura {
    i32 fila;
    i32 longitudo;          /* petita; paritas augetur */
    i32 vestigium_verbi;    /* FNV verbi, ne sors mutata tricam mutet */
    s32 imus;
    s32 summus;
    i32 limes;
    i32 exactus_modulo;     /* B modulo 4294967291 */
} TricaAurea;

interior vacuum
_probare_fenestram_auream (
    CredoContextus* c)
{
                   Piscina* piscina = c->piscina;
    constans FenestraAurea  nodi[] = {
        { "3_1", -VII, V, II, III },
        { "4_1", -VIII, VIII, III, VI },
        { "5_1", -XIII, VII, III, V },
        { "5_2", -IX, XI, IV, VIII },
        { "6_1", -X, XIV, IV, X },
        { "6_2", -XIV, X, IV, XII },
        { "6_3", -XII, XII, V, XVIII },
        { "7_1", -XIX, IX, III, VII },
        { "7_4", -XI, XVII, V, XVI },
        { "7_7", -XV, XIII, V, XXVII },
        { "8_17", -XVI, XVI, VII, LXXII },
        { "8_19", -XVI, XII, VI, XL },
        { "8_20", -XIV, XIV, VI, LIV },
        { "9_35", -XIII, XXIII, VI, XXXVI },
        { "9_42", -XV, XVII, VII, LXXV },
        { "10_124", -XXII, XIV, VI, LVI },
        { "10_132", -XX, XVI, VII, XCVI },
        { "10_161", -XIV, XVIII, VIII, CXCII },
        { "13n_5108", -XXIX, XIX, X, (s64)864 },
        { "13n_5109", -XXV, XXIII, XI, (s64)1125 },
        { "13n_5110", -XVII, XXXI, IX, (s64)432 }
    };
    constans TricaAurea tricae[] = {
        { VIII, CCI, (i32)3522731884UL, -(s32)283, (s32)273, (i32)166,
          (i32)3487703463UL },
        { XII, CCI, (i32)1771876111UL, -(s32)299, (s32)269, (i32)165,
          (i32)3103119802UL },
        { XII, CDI, (i32)96994982UL, -(s32)557, (s32)547, (i32)333,
          (i32)2518497856UL },
        { VIII, DCCCI, (i32)1632908588UL, -(s32)1125, (s32)1203,
          (i32)642, (i32)3562495677UL },
        { XII, DCCCI, (i32)964510034UL, -(s32)1109, (s32)1131,
          (i32)654, (i32)2962172050UL }
    };
    i32 h        = (i32)2166136261UL;
    i32 numerus  = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < (i32)(magnitudo(nodi)
        / magnitudo(nodi[ZEPHYRUM]));
        k++)
    {
        constans NodusTabulae* n =
            tabula_nodorum_quaere(nodi[k].titulus);
                          s32 imus    = ZEPHYRUM;
                          s32 summus  = ZEPHYRUM;
                          i32 limes   = ZEPHYRUM;
                       Magnus exactus;
                          s64 b = ZEPHYRUM;

        CREDO_NECESSE_NON_NIHIL(n);
        CREDO_NECESSE(laqueus_pd_fenestra(tabula_nodorum_pd(n, piscina),
            n->transitus, piscina, &imus, &summus, &limes, &exactus));
        CREDO_AEQUALIS_S32(imus, nodi[k].imus);
        CREDO_AEQUALIS_S32(summus, nodi[k].summus);
        CREDO_AEQUALIS_I32(limes, nodi[k].limes);
        CREDO_VERUM(magnus_ad_s64(exactus, &b));
        CREDO_AEQUALIS_S64(b, nodi[k].exactus);
    }

    /* tabula tota: vestigium (imus, summus, limes, B mod p) */
    per (k = ZEPHYRUM; k < tabula_nodorum_numerus(); k++)
    {
        constans NodusTabulae* n = tabula_nodorum_nodus(k);
               PiscinaNotatio  nota;
                          s32  imus;
                          s32  summus;
                          i32  limes;
                       Magnus  exactus;

        si (n->transitus < II)
        {
            perge;
        }
        nota = piscina_notare(piscina);
        si (laqueus_pd_fenestra(tabula_nodorum_pd(n, piscina),
            n->transitus, piscina, &imus, &summus, &limes, &exactus))
        {
            h = _misce(h, imus);
            h = _misce(h, summus);
            h = _misce(h, (s32)limes);
            h = _misce(h, (s32)magnus_residuum_parvum(exactus,
                (i32)4294967291UL));
            numerus++;
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32(numerus, (i32)12965UL);
    CREDO_AEQUALIS_I32(h, (i32)655766105UL);

    /* tricae D136 */
    per (k = ZEPHYRUM; k < (i32)(magnitudo(tricae)
        / magnitudo(tricae[ZEPHYRUM])); k++)
    {
        PiscinaNotatio  nota = piscina_notare(piscina);
                  Sors  s;
                   i32  longitudo  = tricae[k].longitudo;
                   s32* verbum     = NIHIL;
                   i32* pd;
                   i32  v       = (i32)2166136261UL;
                   s32  imus    = ZEPHYRUM;
                   s32  summus  = ZEPHYRUM;
                   i32  limes   = ZEPHYRUM;
                Magnus  exactus;
                   i32  j;

        sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
        pd = _plexus_aureus(&s, tricae[k].fila, &longitudo, piscina,
            &verbum);
        CREDO_NECESSE_NON_NIHIL(pd);
        per (j = ZEPHYRUM; j < longitudo; j++)
        {
            v = _misce(v, verbum[j]);
        }
        CREDO_AEQUALIS_I32(v, tricae[k].vestigium_verbi);
        CREDO_NECESSE(laqueus_pd_fenestra(pd, longitudo, piscina, &imus,
            &summus, &limes, &exactus));
        CREDO_AEQUALIS_S32(imus, tricae[k].imus);
        CREDO_AEQUALIS_S32(summus, tricae[k].summus);
        CREDO_AEQUALIS_I32(limes, tricae[k].limes);
        CREDO_AEQUALIS_I32(magnus_residuum_parvum(exactus,
            (i32)4294967291UL), tricae[k].exactus_modulo);
        piscina_reficere(piscina, nota);
    }
}

/* laqueus_pd_fenestra = modulus vertex in eodem codice (limes et B);
 * refutationes sine exitu tacto */
interior vacuum
_probare_fenestram_et_verticem (
    CredoContextus* c)
{
                  Piscina* piscina = c->piscina;
                     Sors  s;
                      i32  t;
    constans NodusTabulae* trifolium = tabula_nodorum_quaere("3_1");
                      i32  malus[XII];
                      s32  imus    = -(s32)DCCCLXXXVIII;
                      s32  summus  = -(s32)DCCCLXXXVIII;
                      i32  limes   = (i32)DCCCLXXXVIII;

    sors_seminare(&s, (i64)MMXXXVIII, ZEPHYRUM);
    per (t = ZEPHYRUM; t < CC; t++)
    {
               PiscinaNotatio  nota = piscina_notare(piscina);
        constans NodusTabulae* n = tabula_nodorum_nodus(sors_intra(&s,
            tabula_nodorum_numerus()));
                constans i32* pd;
        LaqueusModulusVertex* mv;
                         s32  infima;
                         s32  summa;
                         i32  bita;
                      Magnus  exactus;
                      Magnus  exactus_verticis;

        si (n->transitus < II)
        {
            piscina_reficere(piscina, nota);
            perge;
        }
        pd = tabula_nodorum_pd(n, piscina);
        mv = laqueus_modulus_vertex_creare(pd, n->transitus,
            OCTETI_MAXIMI, piscina);
        CREDO_NECESSE_NON_NIHIL(mv);
        CREDO_NECESSE(laqueus_pd_fenestra(pd, n->transitus, piscina,
            &infima,
            &summa, &bita, &exactus));
        CREDO_AEQUALIS_I32(bita, laqueus_modulus_vertex_limes(mv));
        CREDO_VERUM(laqueus_modulus_vertex_limes_exactus(mv,
            &exactus_verticis));
        CREDO_VERUM(magnus_aequalis(exactus, exactus_verticis));
        piscina_reficere(piscina, nota);
    }

    /* refutationes: exitus non tacti */
    CREDO_FALSUM(laqueus_pd_fenestra(NIHIL, III, piscina, &imus,
        &summus,
        &limes, NIHIL));
    CREDO_FALSUM(laqueus_pd_fenestra(tabula_nodorum_pd(trifolium,
        piscina), I, piscina, &imus, &summus, &limes, NIHIL));
    per (t = ZEPHYRUM; t < XII; t++)
    {
        malus[t] = tabula_nodorum_pd(trifolium, piscina)[t];
    }
    malus[ZEPHYRUM] = XIII;                    /* os extra 1..2c */
    CREDO_FALSUM(laqueus_pd_fenestra(malus, III, piscina, &imus,
        &summus,
        &limes, NIHIL));
    CREDO_AEQUALIS_S32(imus, -(s32)DCCCLXXXVIII);
    CREDO_AEQUALIS_S32(summus, -(s32)DCCCLXXXVIII);
    CREDO_AEQUALIS_I32(limes, (i32)DCCCLXXXVIII);
}


/* ==================================================
 * Via Temperley-Lieb (temperley-lieb-plan T2b)
 *
 * Oracula: summa statuum (c <= XXIV), modulus vertex (uncinus et
 * puncta: punctum ET valor), tabula nodorum (Jones), uncini D136 a
 * recensore T3 certificati (oraculum regularis repraesentationis
 * proprium), motus Markov et braid ad puncta ubi nullum aliud oraculum
 * pervenit (10 x 401, XI fila).
 * ================================================== */

#define OCTETI_TEMPERLEY ((i64)0x40000000UL)

interior LaqueusModulusTemperley*
_temperley (
    constans s32* verbum,
             i32  longitudo,
             i32  fila,
         Piscina* piscina)
{
    redde laqueus_modulus_temperley_creare(verbum, longitudo, fila,
        OCTETI_TEMPERLEY, piscina);
}

/* uncinus -> Jones: (-A^3)^-w <D>, A = t^(-1/4) */
interior Polynomium
_jones_ex_uncino_probationis (
    Polynomium  uncinus,
           s32  scriptura,
       Piscina* piscina)
{
    Polynomium f = polynomium_nullum();
    Polynomium g = polynomium_nullum();
           s32 w = scriptura
               < ZEPHYRUM ? -scriptura : scriptura;

    CREDO_NECESSE(polynomium_translata(uncinus, -(s32)III * scriptura,
        piscina, &f));
    si (w % II != ZEPHYRUM)
    {
        f = polynomium_nega(f, piscina);
    }
    CREDO_NECESSE(polynomium_contrahe(f, -(s32)IV, piscina, &g));
    redde g;
}

interior s32
_scriptura_verbi (
    constans s32* verbum,
             i32  longitudo)
{
    s32 w = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        w += verbum[k] > ZEPHYRUM ? I : -(s32)I;
    }
    redde w;
}

interior vacuum
_probare_temperley_statuum (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;

    sors_seminare(&s, (i64)MMXL, ZEPHYRUM);
    per (t = ZEPHYRUM; t < XL; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
        i32 fila             = II + sors_intra(&s, VII);
        i32 longitudo        = II + sors_intra(&s, XXI);
        s32* verbum          = NIHIL;
        i32* pd = _plexus_aureus(&s, fila, &longitudo, piscina,
            &verbum);
        LaqueusModulusTemperley* mt;
                     Polynomium  tl      = polynomium_nullum();
                     Polynomium  status  = polynomium_nullum();

        si (pd == NIHIL || longitudo > XXIV)
        {
            piscina_reficere(piscina, nota);
            perge;
        }
        mt = _temperley(verbum, longitudo, fila, piscina);
        CREDO_NECESSE_NON_NIHIL(mt);
        CREDO_NECESSE(laqueus_modulus_temperley_uncinus(mt, piscina,
            &tl));
        CREDO_NECESSE(laqueus_uncinus_ex_pd(pd, longitudo, piscina,
            &status));
        CREDO_VERUM(polynomium_aequalis(tl, status));
        piscina_reficere(piscina, nota);
    }
}

interior vacuum
_probare_temperley_verticis (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;

    sors_seminare(&s, (i64)MMXLI, ZEPHYRUM);
    per (t = ZEPHYRUM; t < XXX; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
        i32 fila             = III + sors_intra(&s, VI);
        i32 longitudo        = XXV + sors_intra(&s, LXXV);
        s32* verbum          = NIHIL;
        i32* pd = _plexus_aureus(&s, fila, &longitudo, piscina,
            &verbum);
        LaqueusModulusTemperley* mt;
           LaqueusModulusVertex* mv;
                     Polynomium  tl = polynomium_nullum();
                     Polynomium  uncinus_verticis = polynomium_nullum();

        CREDO_NECESSE_NON_NIHIL(pd);
        mt = _temperley(verbum, longitudo, fila, piscina);
        mv = laqueus_modulus_vertex_creare(pd, longitudo, OCTETI_MAXIMI,
            piscina);
        CREDO_NECESSE_NON_NIHIL(mt);
        CREDO_NECESSE_NON_NIHIL(mv);
        CREDO_AEQUALIS_I32(laqueus_modulus_temperley_limes(mt),
            laqueus_modulus_vertex_limes(mv));
        CREDO_NECESSE(laqueus_modulus_temperley_uncinus(mt, piscina,
            &tl));
        CREDO_NECESSE(laqueus_modulus_vertex_uncinus(mv, piscina,
            &uncinus_verticis));
        CREDO_VERUM(polynomium_aequalis(tl, uncinus_verticis));
        piscina_reficere(piscina, nota);
    }
}

interior vacuum
_probare_temperley_puncta (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    Sors s;
    i32 longitudo  = CI;
    s32* verbum    = NIHIL;
    i32* pd;
    LaqueusModulusTemperley* mt;
    LaqueusModulusVertex* mv;
    constans i32 primi[III] = { (i32)998244353UL, (i32)2147483629UL,
        (i32)998244353UL };
               i32 valores_primi[XL];
               i32 a = (i32)0xDEADUL;
               i32 v = (i32)0xDEADUL;
               i32 q;
               i32 omega;
    memoriae_index numerus_ante;

    sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
    pd = _plexus_aureus(&s, VIII, &longitudo, piscina, &verbum);
    CREDO_NECESSE_NON_NIHIL(pd);
    /* scriptura non nulla (celeritas H2: A^w omissum hic non lateat) */
    CREDO_VERUM(_scriptura_verbi(verbum, longitudo) != ZEPHYRUM);
    mt = _temperley(verbum, longitudo, VIII, piscina);
    mv = laqueus_modulus_vertex_creare(pd, longitudo, OCTETI_MAXIMI,
        piscina);
    CREDO_NECESSE_NON_NIHIL(mt);
    CREDO_NECESSE_NON_NIHIL(mv);

    /* primus p, deinde q, deinde p: puncta et valores eadem ac vertex,
     * valores p iterum idem; aestimare nihil allocat */
    numerus_ante = piscina_numerus_allocationum(piscina);
    per (q = ZEPHYRUM; q < III; q++)
    {
        per (omega = II; omega < XL; omega++)
        {
            i32 at = ZEPHYRUM;
            i32 vt = ZEPHYRUM;
            i32 av = ZEPHYRUM;
            i32 vv = ZEPHYRUM;
            s32 st = laqueus_modulus_temperley_aestimare(mt, omega,
                primi[q], &at, &vt);
            s32 sv = laqueus_modulus_vertex_aestimare(mv, omega,
                primi[q],
                &av, &vv);

            CREDO_AEQUALIS_S32(st, sv);
            CREDO_AEQUALIS_I32(at, av);
            CREDO_AEQUALIS_I32(vt, vv);
            si (q == ZEPHYRUM)
            {
                valores_primi[omega] = vt;
            }
            alioquin si (q == II)
            {
                CREDO_AEQUALIS_I32(vt, valores_primi[omega]);
            }
        }
    }
    CREDO_AEQUALIS_I64((i64)piscina_numerus_allocationum(piscina),
        (i64)numerus_ante);

    /* radix i (signum ex g minimo) in duobus primis: p = 17, omega = 2 ->
     * i = 3^4 = 13, A = 13 x 4 = 1; p = 998244353 -> g = 3, i =
     * 911660635, A = 4 i = 651909481. (g ipse non figitur: non residua
     * eiusdem signi idem i dant.) */
    CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(mt, II, XVII,
        &a, &v), INTERPOLATIO_UTILE);
    CREDO_AEQUALIS_I32(a, I);
    CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(mt, II,
        (i32)998244353UL, &a, &v), INTERPOLATIO_UTILE);
    CREDO_AEQUALIS_I32(a, (i32)651909481UL);

    /* celeritas G1: via TL primos >= 2^31 refutat (exitus non tacti),
     * modulus vertex eos accipit; 2147483629 (primus maximus = 1 mod 4
     * infra 2^31) culpam multiplicationis 32 bitorum adhuc capit */
    a = (i32)0xDEADUL;
    v = (i32)0xDEADUL;
    CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(mt, VII,
        (i32)4294967029UL, &a, &v), INTERPOLATIO_ERRATUM);
    CREDO_AEQUALIS_I32(a, (i32)0xDEADUL);
    CREDO_AEQUALIS_I32(v, (i32)0xDEADUL);
    CREDO_AEQUALIS_S32(laqueus_modulus_vertex_aestimare(mv, VII,
        (i32)4294967029UL, &a, &v), INTERPOLATIO_UTILE);

    /* custodes: exitus non tacti */
    a = (i32)0xDEADUL;
    v = (i32)0xDEADUL;
    CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(mt, V, XVII,
        &a, &v), INTERPOLATIO_INUTILE);
    CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(mt, ZEPHYRUM,
        primi[ZEPHYRUM], &a, &v), INTERPOLATIO_INUTILE);
    CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(mt, II, VII,
        &a, &v), INTERPOLATIO_ERRATUM);
    CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(mt, II, XXI,
        &a, &v), INTERPOLATIO_ERRATUM);
    CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(mt, V, LXXXV,
        &a, &v), INTERPOLATIO_ERRATUM);
    CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(mt, II,
        (i32)4294967291UL, &a, &v), INTERPOLATIO_ERRATUM);
    CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(NIHIL, II,
        primi[ZEPHYRUM], &a, &v), INTERPOLATIO_ERRATUM);
    CREDO_AEQUALIS_I32(a, (i32)0xDEADUL);
    CREDO_AEQUALIS_I32(v, (i32)0xDEADUL);
}

interior vacuum
_probare_temperley_notos (
    CredoContextus* c)
{
                    Piscina* piscina         = c->piscina;
               constans s32  trifolium[III]  = { I, I, I };
               constans s32  octo[IV] = { I, -(s32)II, I,
                   -(s32)II };
                        s32  torus_tria[VIII];
                        s32  torus_quattuor[XV];
         constans character* nomina[III] = { "3_1", "4_1", "8_19" };
               constans s32* verba[III];
                        i32  longitudines[III]  = { III, IV, VIII };
                        i32  fila[III]          = { II, III, III };
                        i32  k;
    LaqueusModulusTemperley* mt;
                 Polynomium  tl = polynomium_nullum();

    per (k = ZEPHYRUM; k < VIII; k++)
    {
        torus_tria[k] = (s32)(k % II + I);
    }
    per (k = ZEPHYRUM; k < XV; k++)
    {
        torus_quattuor[k] = (s32)(k % III + I);
    }
    verba[ZEPHYRUM]  = trifolium;
    verba[I]         = octo;
    verba[II]        = torus_tria;
    per (k = ZEPHYRUM; k < III; k++)
    {
        constans NodusTabulae* n = tabula_nodorum_quaere(nomina[k]);

        CREDO_NECESSE_NON_NIHIL(n);
        mt = _temperley(verba[k], longitudines[k], fila[k], piscina);
        CREDO_NECESSE_NON_NIHIL(mt);
        CREDO_NECESSE(laqueus_modulus_temperley_uncinus(mt, piscina,
            &tl));
        CREDO_VERUM(polynomium_aequalis(_jones_ex_uncino_probationis(tl,
            _scriptura_verbi(verba[k], longitudines[k]), piscina),
            _polynomium_textus(n->jones, piscina)));
    }
    /* T(4,5): XV transitus, extra tabulam - Jones = modulus vertex */
    mt = _temperley(torus_quattuor, XV, IV, piscina);
    CREDO_NECESSE_NON_NIHIL(mt);
    CREDO_NECESSE(laqueus_modulus_temperley_uncinus(mt, piscina, &tl));
    CREDO_VERUM(polynomium_aequalis(_jones_ex_uncino_probationis(tl, XV,
        piscina), _jones_plexus(torus_quattuor, XV, IV, piscina)));
}

/* uncini D136 (knotapel/demo_136_tl_braid_closures/brackets.txt), a
 * recensore T3 per oraculum proprium certificati: coefficientes A^(imus
 * + 4k) */
interior constans s64 UNCINUS_3_50[] = { -1, 3, -6, 11, -15, 21, -24,
    25, -24, 20, -15, 10, -6, 3, -1 };
interior constans s64 UNCINUS_8_101[] = { 1, -25, 239, -1382, 5594,
    -16876, 38538, -63522, 56339, 63682, -412521, 1075781, -1985533,
    2794618, -2850483, 1356335, 2284376, -8068156, 15140330, -21867198,
    26274328, -26719418, 22535235, -14358472, 3978509, 6249031,
    -14179663, 18524415, -19118989, 16765182, -12780037, 8482149,
    -4818019, 2223823, -698096, -13976, 232973, -222484, 145361, -75684,
    32646, -11729, 3464, -815, 144, -17, 1 };
interior constans s64 UNCINUS_8_201[] = { 1, -32, 491, -4903, 36121,
    -209888, 1001297, -4022751, 13829715, -41044537, 105356095,
    -232222949, 429238763, -622129746, 540170191, 413673285,
    -3183514275L, 8752266673L, -17287055302L, 26761300855L,
    -31603595402L, 22529224755L, 11122222034L, -76064042165L,
    168215278269L, -267944865861L, 341721283127L, -352339522502L,
    275214787298L, -113489275101L, -96718295492L, 295843478580L,
    -422670031046L, 438938651651L, -345131071952L, 179793018306L,
    -2780427390L, -129475143256L, 185135612716L, -165405818488L,
    97899493472L, -20050873397L, -38124297024L, 63805996604L,
    -60911780461L, 42466229554L, -21690227404L, 6518673415L, 1172147008,
    -3282376246L, 2674897030L, -1449232449, 546038561, -109663707,
    -24979329, 34853559, -17334761, 4743282, -6942, -786723, 505111,
    -207656, 64352, -15601, 2944, -416, 40, -2 };

nomen structura {
             i32  fila;
             i32  longitudo;
             i32  vestigium_verbi;
             s32  imus;
    constans s64* coefficientes;
             i32  numerus;
} UncinusCertus;

interior vacuum
_probare_temperley_certos (
    CredoContextus* c)
{
                   Piscina* piscina = c->piscina;
    constans UncinusCertus  certi[III] = {
        { III, L, (i32)2213358261UL, -(s32)XXII, UNCINUS_3_50,
          (i32)(magnitudo(UNCINUS_3_50) / magnitudo(s64)) },
        { VIII, CI, (i32)2594227919UL, -(s32)LXIX, UNCINUS_8_101,
          (i32)(magnitudo(UNCINUS_8_101) / magnitudo(s64)) },
        { VIII, CCI, (i32)3522731884UL, -(s32)CXXXV, UNCINUS_8_201,
          (i32)(magnitudo(UNCINUS_8_201) / magnitudo(s64)) }
    };
    i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        PiscinaNotatio nota = piscina_notare(piscina);
        Sors s;
        i32 longitudo  = certi[k].longitudo;
        s32* verbum    = NIHIL;
        i32* pd;
        i32 h        = (i32)2166136261UL;
        i32 numerus  = IV * (certi[k].numerus - I) + I;
        Magnus* coefficientes = (Magnus*)piscina_allocare(piscina,
            (memoriae_index)numerus * magnitudo(Magnus));
                     Polynomium  exspectatum  = polynomium_nullum();
                     Polynomium  tl           = polynomium_nullum();
        LaqueusModulusTemperley* mt;
                            i32  j;

        sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
        pd = _plexus_aureus(&s, certi[k].fila, &longitudo, piscina,
            &verbum);
        CREDO_NECESSE_NON_NIHIL(pd);
        per (j = ZEPHYRUM; j < longitudo; j++)
        {
            h = _misce(h, verbum[j]);
        }
        CREDO_AEQUALIS_I32(h, certi[k].vestigium_verbi);
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            coefficientes[j] = magnus_ex_s64(j % IV == ZEPHYRUM
                ? certi[k].coefficientes[j / IV] : ZEPHYRUM);
        }
        CREDO_NECESSE(polynomium_ex_coefficientibus(coefficientes,
            numerus,
            certi[k].imus, piscina, &exspectatum));
        mt = _temperley(verbum, longitudo, certi[k].fila, piscina);
        CREDO_NECESSE_NON_NIHIL(mt);
        CREDO_NECESSE(laqueus_modulus_temperley_uncinus(mt, piscina,
            &tl));
        CREDO_VERUM(polynomium_aequalis(tl, exspectatum));
        piscina_reficere(piscina, nota);
    }
}

/* motus ad puncta in trica D136 10 x 401 (nullum aliud oraculum in
 * suita pervenit): stabilizatio retro et ante in XI filis (x -A^(+-3)),
 * relationes braid et R2 inserta (idem valor) */
interior vacuum
_probare_temperley_motus (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                       Sors  s;
                        i32  longitudo  = CDI;
                        s32* verbum     = NIHIL;
                        i32* pd;
                        s32* retro;
                        s32* ante;
                        s32* insertum;
    LaqueusModulusTemperley* mt;
    LaqueusModulusTemperley* retro_structurae[II];
    LaqueusModulusTemperley* ante_structurae[II];
    LaqueusModulusTemperley* inserta_structura;
                        i32  p = (i32)998244353UL;
                        i32  signum;
                        i32  omega;
                        i32  j;

    sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
    pd = _plexus_aureus(&s, X, &longitudo, piscina, &verbum);
    CREDO_NECESSE_NON_NIHIL(pd);
    {
        i32 h = (i32)2166136261UL;

        per (j = ZEPHYRUM; j < longitudo; j++)
        {
            h = _misce(h, verbum[j]);
        }
        CREDO_AEQUALIS_I32(h, (i32)2869638322UL);
    }
    mt = _temperley(verbum, longitudo, X, piscina);
    CREDO_NECESSE_NON_NIHIL(mt);
    retro = (s32*)piscina_allocare(piscina, (memoriae_index)(longitudo
        + I) * magnitudo(s32));
    ante = (s32*)piscina_allocare(piscina, (memoriae_index)(longitudo
        + I) * magnitudo(s32));
    per (signum = ZEPHYRUM; signum < II; signum++)
    {
        s32 littera = signum == ZEPHYRUM ? (s32)I : -(s32)I;

        per (j = ZEPHYRUM; j < longitudo; j++)
        {
            retro[j] = verbum[j];
            ante[j + I] = verbum[j] > ZEPHYRUM ? verbum[j] + I
                : verbum[j] - I;
        }
        retro[longitudo]  = littera * (s32)X;
        ante[ZEPHYRUM]    = littera;
        retro_structurae[signum] = _temperley(retro, longitudo + I, XI,
            piscina);
        ante_structurae[signum] = _temperley(ante, longitudo + I, XI,
            piscina);
        CREDO_NECESSE_NON_NIHIL(retro_structurae[signum]);
        CREDO_NECESSE_NON_NIHIL(ante_structurae[signum]);
        /* nova structura pro signo quoque: verba copiantur */
    }
    /* sigma_1 sigma_2 sigma_1 sigma_2^-1 sigma_1^-1 sigma_2^-1 = 1 et
     * sigma_5 sigma_5^-1 = 1, in medio verbo inserta */
    insertum = (s32*)piscina_allocare(piscina,
        (memoriae_index)(longitudo
        + VIII) * magnitudo(s32));
    per (j = ZEPHYRUM; j < longitudo + VIII; j++)
    {
        constans s32 inserta[VIII] = { I, II, I, -(s32)II, -(s32)I,
            -(s32)II, V, -(s32)V };
        i32 medium = longitudo / II;

        insertum[j] = j < medium ? verbum[j] : (j < medium + VIII
            ? inserta[j - medium] : verbum[j - VIII]);
    }
    inserta_structura = _temperley(insertum, longitudo + VIII, X,
        piscina);
    CREDO_NECESSE_NON_NIHIL(inserta_structura);

    per (omega = II; omega < V; omega++)
    {
        i32 a   = ZEPHYRUM;
        i32 v   = ZEPHYRUM;
        i32 at  = ZEPHYRUM;
        i32 vt  = ZEPHYRUM;
        i32 a_cubus;
        i32 a_cubus_inversus;

        CREDO_NECESSE(laqueus_modulus_temperley_aestimare(mt, omega, p,
            &a, &v) == INTERPOLATIO_UTILE);
        a_cubus = congruentia_multiplica(congruentia_multiplica(a, a,
            p), a,
            p);
        (vacuum)congruentia_inversa(a_cubus, p, &a_cubus_inversus);
        per (signum = ZEPHYRUM; signum < II; signum++)
        {
            /* <b sigma_n^(+-1)> = -A^(+-3) <b> */
            i32 factor = congruentia_subtrahe(ZEPHYRUM, signum
                == ZEPHYRUM
                ? a_cubus : a_cubus_inversus, p);
            i32 exspectatum = congruentia_multiplica(factor, v, p);

            CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(
                retro_structurae[signum], omega, p, &at, &vt),
                INTERPOLATIO_UTILE);
            CREDO_AEQUALIS_I32(at, a);
            CREDO_AEQUALIS_I32(vt, exspectatum);
            CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(
                ante_structurae[signum], omega, p, &at, &vt),
                INTERPOLATIO_UTILE);
            CREDO_AEQUALIS_I32(vt, exspectatum);
        }
        CREDO_AEQUALIS_S32(laqueus_modulus_temperley_aestimare(inserta_structura,
            omega, p,
            &at, &vt), INTERPOLATIO_UTILE);
        CREDO_AEQUALIS_I32(vt, v);
    }
}

interior vacuum
_probare_temperley_refutationes (
    CredoContextus* c)
{
                    Piscina* piscina         = c->piscina;
               constans s32  hopf[II]        = { I, I };
               constans s32  trifolium[III]  = { I, I, I };
               constans s32  unum[I]         = { I };
               constans s32  nullum[III]     = { I, ZEPHYRUM, I };
               constans s32  nimium[III]     = { I, III, I };
    LaqueusTemperleySumptus  sumptus;
    LaqueusTemperleySumptus  vacuus;
                 Polynomium  f = polynomium_nullum();

    CREDO_NIHIL(_temperley(hopf, II, II, piscina));           /* catena */
    CREDO_NIHIL(_temperley(trifolium, III, III, piscina));    /* filum intactum */
    CREDO_NIHIL(_temperley(trifolium, ZEPHYRUM, II, piscina));
    CREDO_NIHIL(_temperley(unum, I, II, piscina));            /* c = 1 */
    CREDO_NIHIL(_temperley(nullum, III, II, piscina));
    CREDO_NIHIL(_temperley(nimium, III, II, piscina));
    CREDO_NIHIL(_temperley(trifolium, III, XVII, piscina));
    CREDO_NIHIL(_temperley(NIHIL, III, II, piscina));
    CREDO_NIHIL(laqueus_modulus_temperley_creare(trifolium, III, II,
        OCTETI_TEMPERLEY, NIHIL));
    CREDO_FALSUM(laqueus_modulus_temperley_uncinus(NIHIL, piscina, &f));
    CREDO_AEQUALIS_I32(laqueus_modulus_temperley_limes(NIHIL),
        ZEPHYRUM);

    /* limes octetorum in margine: exactus accipitur, uno minus refutatur */
    sumptus = laqueus_temperley_sumptus(II, III);
    CREDO_VERUM(sumptus.octeti > ZEPHYRUM);
    CREDO_NON_NIHIL(laqueus_modulus_temperley_creare(trifolium, III, II,
        sumptus.octeti, piscina));
    CREDO_NIHIL(laqueus_modulus_temperley_creare(trifolium, III, II,
        sumptus.octeti - I, piscina));
    /* sumptus: multiplicationes = longitudo x Catalan(fila) */
    CREDO_AEQUALIS_I64(laqueus_temperley_sumptus(XII, DCCCI)
        .multiplicationes, (i64)DCCCI * (i64)208012UL);
    CREDO_AEQUALIS_I64(laqueus_temperley_sumptus(XVI, I)
        .multiplicationes, (i64)35357670UL);
    vacuus = laqueus_temperley_sumptus(XVII, X);
    CREDO_AEQUALIS_I64(vacuus.octeti, (i64)ZEPHYRUM);
    CREDO_AEQUALIS_I64(laqueus_temperley_sumptus(ZEPHYRUM, X).octeti,
        (i64)ZEPHYRUM);
}


/* ==================================================
 * Via electa (temperley-lieb-plan T3)
 *
 * Electiones ex sumptu solo (nulla aestimatio): tricae D136 cum vestigio
 * verbi; limites octetorum in margine utriusque viae; c <= 1 forma
 * clausa; refutationes ante pretium; uncinus et Jones per viam = via
 * temperley = modulus vertex.
 * ================================================== */

nomen structura {
           i32 fila;
           i32 longitudo;
           i32 vestigium_verbi;
    LaqueusVia via;
} ElectioCerta;

/* trica D136 (semen MMXXVIII) cum vestigio probato */
interior s32*
_trica_certa (
         i32  fila,
         i32* longitudo,
         i32  vestigium,
     Piscina* piscina)
{
    Sors  s;
     s32* verbum = NIHIL;
     i32* pd;
     i32  h = (i32)2166136261UL;
     i32  j;

    sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
    pd = _plexus_aureus(&s, fila, longitudo, piscina, &verbum);
    si (pd == NIHIL)
    {
        redde NIHIL;
    }
    per (j = ZEPHYRUM; j < *longitudo; j++)
    {
        h = _misce(h, verbum[j]);
    }
    redde h == vestigium ? verbum : NIHIL;
}

interior vacuum
_probare_viam_electam (
    CredoContextus* c)
{
                  Piscina* piscina = c->piscina;
    constans ElectioCerta  electiones[VIII] = {
        { XII, CI, (i32)136900930UL, LAQUEUS_VIA_VERTEX },
        { XII, CCI, (i32)1771876111UL, LAQUEUS_VIA_VERTEX },
        { VIII, CCI, (i32)3522731884UL, LAQUEUS_VIA_TEMPERLEY },
        { X, CCI, (i32)1191330UL, LAQUEUS_VIA_TEMPERLEY },
        { X, CDI, (i32)2869638322UL, LAQUEUS_VIA_TEMPERLEY },
        { XII, CDI, (i32)96994982UL, LAQUEUS_VIA_TEMPERLEY },
        { VIII, DCCCI, (i32)1632908588UL, LAQUEUS_VIA_TEMPERLEY },
        { XII, DCCCI, (i32)964510034UL, LAQUEUS_VIA_TEMPERLEY }
    };
               i32 k;

    per (k = ZEPHYRUM; k < VIII; k++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
        i32 longitudo        = electiones[k].longitudo;
        s32* verbum = _trica_certa(electiones[k].fila, &longitudo,
            electiones[k].vestigium_verbi, piscina);
                     LaqueusVia via = LAQUEUS_VIA_NULLA;
              ContractioSumptus sv;
        LaqueusTemperleySumptus st;

        CREDO_NECESSE_NON_NIHIL(verbum);
        CREDO_NECESSE(laqueus_via_eligere(verbum, longitudo,
            electiones[k].fila, CONTRACTIO_SATURATUM, piscina, &via,
            &sv,
            &st));
        CREDO_AEQUALIS_I32((i32)via, (i32)electiones[k].via);
        CREDO_AEQUALIS_I64(st.multiplicationes,
            laqueus_temperley_sumptus(
            electiones[k].fila, longitudo).multiplicationes);
        CREDO_VERUM(sv.multiplicationes > ZEPHYRUM);
        piscina_reficere(piscina, nota);
    }

    /* limites in margine: vertex (12 x 201: officina ipsa accipitur;
     * infra eam ordo minor quaeritur, donec nullus capit - temperley
     * quoque nimius) et temperley (8 x 201: octeti ipsi, deinde uno minus -
     * vertex quoque nimius). Limes electionem invertere POTEST ubi vertex
     * sine_limite plus memoriae poscit quam temperley: vide inversionem infra
     * (recensio T4, M1). */
    {
        i32  longitudo = CCI;
        s32* verbum = _trica_certa(XII, &longitudo, (i32)1771876111UL,
            piscina);
                     LaqueusVia via = LAQUEUS_VIA_NULLA;
              ContractioSumptus sv;
        LaqueusTemperleySumptus st;

        CREDO_NECESSE_NON_NIHIL(verbum);
        CREDO_NECESSE(laqueus_via_eligere(verbum, longitudo, XII,
            CONTRACTIO_SATURATUM, piscina, &via, &sv, &st));
        CREDO_VERUM(sv.octeti_officinae < st.octeti);
        CREDO_NECESSE(laqueus_via_eligere(verbum, longitudo, XII,
            sv.octeti_officinae, piscina, &via, NIHIL, NIHIL));
        CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_VERTEX);
        /* limes infra officinam: ordo alius minor eligi potest (ordo
         * optimus candidatos nimios omittit) - limes descendit infra
         * officinam quamque relatam donec nullus ordo capit */
        {
                          i64 limes   = sv.octeti_officinae - I;
                          i32 gradus  = ZEPHYRUM;
            ContractioSumptus minor;

            dum (   gradus < XXX
                 && laqueus_via_eligere(verbum, longitudo, XII,
                limes, piscina, &via, &minor, NIHIL))
            {
                CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_VERTEX);
                CREDO_VERUM(minor.octeti_officinae <= limes);
                limes = minor.octeti_officinae - I;
                gradus++;
            }
            CREDO_VERUM(gradus < XXX);
            via = (LaqueusVia)XLII;
            CREDO_FALSUM(laqueus_via_eligere(verbum, longitudo, XII,
                limes,
                piscina, &via, NIHIL, NIHIL));
            CREDO_AEQUALIS_I32((i32)via, XLII);
        }
    }
    {
        i32  longitudo = CCI;
        s32* verbum = _trica_certa(VIII, &longitudo, (i32)3522731884UL,
            piscina);
                     LaqueusVia via = LAQUEUS_VIA_NULLA;
              ContractioSumptus sv;
        LaqueusTemperleySumptus st;

        CREDO_NECESSE_NON_NIHIL(verbum);
        CREDO_NECESSE(laqueus_via_eligere(verbum, longitudo, VIII,
            CONTRACTIO_SATURATUM, piscina, &via, &sv, &st));
        CREDO_VERUM(st.octeti < sv.octeti_officinae);
        CREDO_NECESSE(laqueus_via_eligere(verbum, longitudo, VIII,
            st.octeti, piscina, &via, NIHIL, NIHIL));
        CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_TEMPERLEY);
        CREDO_FALSUM(laqueus_via_eligere(verbum, longitudo, VIII,
            st.octeti - I, piscina, &via, NIHIL, NIHIL));
    }

    /* inversio per limitem (recensio T4, M1): vertex pretio vilior sed
     * officina maior quam octeti temperley; limes in [octeti temperley,
     * officina) -> temperley, et uncinus idem. Trica 11 x 134 (celeritas
     * T3: K* = 15.7, infra constantem temperley refectam; inventa
     * quaerendo 400.000 tricas usque ad 14 fila et 200 transitus -
     * inversiones memoriae solae rarae fiunt quo via TL vilior est;
     * priores 7 x 32 (K* 130.45) et 9 x 74 (K* 47.8) constanti refecta
     * pretio iam invertuntur) */
    {
        constans s32 verbum[CXXXIV] = {
            -V, -VIII, IX, VIII, VIII, -IV, IV, III, X, VII, -X, -VI,
                -II,
            III, -V, -IV, VII, -VI, -IX, -X, X, -IV, VII, VI, -IV, II,
                I,
            VII, I, VI, V, III, VIII, II, IV, -VI, -IV, -I, -II, -IV,
                III,
            V, -I, -V, IV, -X, X, II, IV, -I, VI, III, -II, -VI, -VIII,
                V,
            -VI, VI, -III, III, II, -II, VII, -V, -I, I, -VII, VIII, II,
            VI, II, -VI, VIII, IX, VIII, -III, IX, -IV, -IX, IV, I, X,
            -III, VIII, -III, VIII, I, IX, IX, VI, VIII, VIII, X, -I, V,
            IV, -VI, II, -VI, VIII, -IX, -VIII, II, -I, -IV, V, IX,
                VIII,
            -IV, -I, III, X, V, -V, -VIII, -III, VII, I, V, IX, -VIII,
            VIII, -VI, -IV, II, III, IV, II, VIII, -X, -I, IX, VIII,
                -IV };
                     LaqueusVia via = LAQUEUS_VIA_NULLA;
              ContractioSumptus sv;
        LaqueusTemperleySumptus st;
                     Polynomium sine_limite  = polynomium_nullum();
                     Polynomium limitatus    = polynomium_nullum();

        CREDO_NECESSE(laqueus_via_eligere(verbum, CXXXIV, XI,
            CONTRACTIO_SATURATUM, piscina, &via, &sv, &st));
        CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_VERTEX);
        CREDO_VERUM(st.octeti < sv.octeti_officinae);
        CREDO_NECESSE(laqueus_via_eligere(verbum, CXXXIV, XI, st.octeti,
            piscina, &via, NIHIL, NIHIL));
        CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_TEMPERLEY);
        CREDO_NECESSE(laqueus_uncinus_ex_plexu(verbum, CXXXIV, XI,
            CONTRACTIO_SATURATUM, piscina, &sine_limite, &via));
        CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_VERTEX);
        CREDO_NECESSE(laqueus_uncinus_ex_plexu(verbum, CXXXIV, XI,
            st.octeti, piscina, &limitatus, &via));
        CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_TEMPERLEY);
        CREDO_VERUM(polynomium_aequalis(sine_limite, limitatus));
    }

    /* XVI fila (sigma_1 ... sigma_15: nodus trivialis, XV transitus):
     * temperley octeti ~107 MB, vertex sine_limite; limes infra octetos
     * temperley vertex servat (via limitis temperley exercita) */
    {
                            s32 verbum[XV];
                     LaqueusVia via = LAQUEUS_VIA_NULLA;
        LaqueusTemperleySumptus st;

        per (k = ZEPHYRUM; k < XV; k++)
        {
            verbum[k] = (s32)(k + I);
        }
        CREDO_NECESSE(laqueus_via_eligere(verbum, XV, XVI,
            CONTRACTIO_SATURATUM, piscina, &via, NIHIL, &st));
        CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_VERTEX);
        CREDO_AEQUALIS_I64(st.multiplicationes, (i64)XV
            * (i64)35357670UL);
        via = LAQUEUS_VIA_NULLA;
        CREDO_NECESSE(laqueus_via_eligere(verbum, XV, XVI, st.octeti
            - I,
            piscina, &via, NIHIL, NIHIL));
        CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_VERTEX);
    }

    /* XVII fila (sigma_1 ... sigma_16): temperley non eligibilis (fila >
     * XVI, sumptus nullus) - vertex, numquam temperley pretio nullo */
    {
                            s32 verbum[XVI];
                     LaqueusVia via = LAQUEUS_VIA_NULLA;
        LaqueusTemperleySumptus st;

        per (k = ZEPHYRUM; k < XVI; k++)
        {
            verbum[k] = (s32)(k + I);
        }
        CREDO_NECESSE(laqueus_via_eligere(verbum, XVI, XVII,
            CONTRACTIO_SATURATUM, piscina, &via, NIHIL, &st));
        CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_VERTEX);
        CREDO_AEQUALIS_I64(st.octeti, (i64)ZEPHYRUM);
    }

    /* pretium nullam officinam allocat: piscina RECENS, apex eius (non
     * usus currens, quem reficere restituit) longe infra officinam
     * vertex 12 x 801 (~557 MB) - recensio T4, M2 */
    {
        i32  longitudo = DCCCI;
        s32* verbum = _trica_certa(XII, &longitudo, (i32)964510034UL,
            piscina);
        Piscina* recens = piscina_generare_dynamicum("via_pretium",
            (memoriae_index)1048576UL);
               LaqueusVia via = LAQUEUS_VIA_NULLA;
        ContractioSumptus sv;

        CREDO_NECESSE_NON_NIHIL(verbum);
        CREDO_NECESSE_NON_NIHIL(recens);
        CREDO_VERUM(laqueus_via_eligere(verbum, longitudo, XII,
            CONTRACTIO_SATURATUM, recens, &via, &sv, NIHIL));
        CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_TEMPERLEY);
        CREDO_VERUM(sv.octeti_officinae > (i64)100000000UL);
        CREDO_VERUM((i64)piscina_summa_apex_usus(recens)
            < sv.octeti_officinae / (i64)C);
        piscina_destruere(recens);
    }
}

interior vacuum
_probare_viam_clausam (
    CredoContextus* c)
{
         Piscina* piscina         = c->piscina;
    constans s32  positivum[I]    = { I };
    constans s32  negativum[I]    = { -(s32)I };
    constans s32  hopf[II]        = { I, I };
    constans s32  trifolium[III]  = { I, I, I };
    constans s32  nimium[III]     = { I, III, I };
      Polynomium  f               = polynomium_nullum();
      Polynomium  exspectatum     = polynomium_nullum();
      Polynomium  unitas = polynomium_constans(magnus_ex_s64((s64)I),
          piscina);
    LaqueusVia via = LAQUEUS_VIA_VERTEX;

    /* c = 1: -A^3, -A^-3; Jones 1 */
    CREDO_NECESSE(laqueus_uncinus_ex_plexu(positivum, I, II,
        CONTRACTIO_SATURATUM, piscina, &f, &via));
    CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_NULLA);
    CREDO_NECESSE(polynomium_monomium(magnus_ex_s64(-(s64)I), III,
        piscina, &exspectatum));
    CREDO_VERUM(polynomium_aequalis(f, exspectatum));
    CREDO_NECESSE(laqueus_uncinus_ex_plexu(negativum, I, II,
        CONTRACTIO_SATURATUM, piscina, &f, NIHIL));
    CREDO_NECESSE(polynomium_monomium(magnus_ex_s64(-(s64)I), -(s32)III,
        piscina, &exspectatum));
    CREDO_VERUM(polynomium_aequalis(f, exspectatum));
    CREDO_NECESSE(laqueus_jones_ex_plexu(negativum, I, II,
        CONTRACTIO_SATURATUM, piscina, &f, NIHIL));
    CREDO_VERUM(polynomium_aequalis(f, unitas));

    /* c = 0, I filum: 1 */
    via = LAQUEUS_VIA_VERTEX;
    CREDO_NECESSE(laqueus_uncinus_ex_plexu(NIHIL, ZEPHYRUM, I,
        CONTRACTIO_SATURATUM, piscina, &f, &via));
    CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_NULLA);
    CREDO_VERUM(polynomium_aequalis(f, unitas));

    /* refutationes: exitus non tacti */
    via  = (LaqueusVia)XLII;
    f    = unitas;
    CREDO_FALSUM(laqueus_uncinus_ex_plexu(hopf, II, II,
        CONTRACTIO_SATURATUM, piscina, &exspectatum, &via));
    CREDO_FALSUM(laqueus_uncinus_ex_plexu(trifolium, III, III,
        CONTRACTIO_SATURATUM, piscina, &exspectatum, &via));
    CREDO_FALSUM(laqueus_uncinus_ex_plexu(nimium, III, II,
        CONTRACTIO_SATURATUM, piscina, &exspectatum, &via));
    CREDO_FALSUM(laqueus_uncinus_ex_plexu(NIHIL, ZEPHYRUM, II,
        CONTRACTIO_SATURATUM, piscina, &exspectatum, &via));
    CREDO_FALSUM(laqueus_via_eligere(hopf, II, II, CONTRACTIO_SATURATUM,
        piscina, &via, NIHIL, NIHIL));
    CREDO_FALSUM(laqueus_jones_ex_plexu(trifolium, III, II, I, piscina,
        &exspectatum, &via));
    CREDO_AEQUALIS_I32((i32)via, XLII);
}

/* uncinus et Jones per viam = via temperley = modulus vertex = Jones
 * plexus (vertex), in tricis fortuitis utriusque viae */
interior vacuum
_probare_per_viam (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  visae[III] = { ZEPHYRUM, ZEPHYRUM, ZEPHYRUM };

    sors_seminare(&s, (i64)MMXLII, ZEPHYRUM);
    per (t = ZEPHYRUM; t < XX; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
        i32 fila             = III + sors_intra(&s, VII);
        i32 longitudo        = XX + sors_intra(&s, LXXX);
        s32* verbum          = NIHIL;
        i32* pd = _plexus_aureus(&s, fila, &longitudo, piscina,
            &verbum);
        LaqueusModulusTemperley* mt;
        Polynomium per_viam          = polynomium_nullum();
        Polynomium tl                = polynomium_nullum();
        Polynomium uncinus_verticis  = polynomium_nullum();
        Polynomium jones             = polynomium_nullum();
        LaqueusVia via               = LAQUEUS_VIA_NULLA;

        CREDO_NECESSE_NON_NIHIL(pd);
        CREDO_NECESSE(laqueus_uncinus_ex_plexu(verbum, longitudo, fila,
            CONTRACTIO_SATURATUM, piscina, &per_viam, &via));
        mt = _temperley(verbum, longitudo, fila, piscina);
        CREDO_NECESSE_NON_NIHIL(mt);
        CREDO_NECESSE(laqueus_modulus_temperley_uncinus(mt, piscina,
            &tl));
        CREDO_NECESSE(laqueus_uncinus_ex_pd_rete(pd, longitudo,
            OCTETI_MAXIMI, piscina, &uncinus_verticis, NIHIL));
        CREDO_VERUM(polynomium_aequalis(per_viam, tl));
        CREDO_VERUM(polynomium_aequalis(per_viam, uncinus_verticis));
        CREDO_NECESSE(laqueus_jones_ex_plexu(verbum, longitudo, fila,
            CONTRACTIO_SATURATUM, piscina, &jones, NIHIL));
        CREDO_VERUM(polynomium_aequalis(jones, _jones_plexus(verbum,
            longitudo, fila, piscina)));
        visae[(i32)via]++;
        piscina_reficere(piscina, nota);
    }
    /* XVII fila per viam: sigma_1 ... sigma_16 = nodus trivialis, uncinus
     * (-A^3)^16 = A^48 */
    {
               s32 verbum[XVI];
        Polynomium f            = polynomium_nullum();
        Polynomium exspectatum  = polynomium_nullum();
        LaqueusVia via          = LAQUEUS_VIA_NULLA;

        per (t = ZEPHYRUM; t < XVI; t++)
        {
            verbum[t] = (s32)(t + I);
        }
        CREDO_NECESSE(laqueus_uncinus_ex_plexu(verbum, XVI, XVII,
            CONTRACTIO_SATURATUM, piscina, &f, &via));
        CREDO_AEQUALIS_I32((i32)via, (i32)LAQUEUS_VIA_VERTEX);
        CREDO_NECESSE(polynomium_monomium(magnus_ex_s64((s64)I), XLVIII,
            piscina, &exspectatum));
        CREDO_VERUM(polynomium_aequalis(f, exspectatum));
    }
    /* ambae viae exercitae */
    CREDO_VERUM(visae[LAQUEUS_VIA_VERTEX] > ZEPHYRUM);
    CREDO_VERUM(visae[LAQUEUS_VIA_TEMPERLEY] > ZEPHYRUM);
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
    { "summae connexae", _probare_summas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "polygona fortuita", _probare_laqueos_fortuitos, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "gyrationes", _probare_gyrationes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "puncta", _probare_puncta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "refutationes", _probare_refutationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "limes manu", _probare_limitem_manu, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limes tabulae", _probare_limitem_tabulae, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "limes tricarum", _probare_limitem_tricarum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "ordo optimus", _probare_ordinem_optimum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "torus T(7,8)", _probare_torum_septem_octo, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "plexus", _probare_plexum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fenestra aurea", _probare_fenestram_auream, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "fenestra et vertex", _probare_fenestram_et_verticem, NIHIL,
        NIHIL,
      NIHIL, NIHIL },
    { "temperley summa statuum", _probare_temperley_statuum, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "temperley vertex", _probare_temperley_verticis, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "temperley puncta", _probare_temperley_puncta, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "temperley nodi noti", _probare_temperley_notos, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "temperley uncini certi", _probare_temperley_certos, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "temperley motus", _probare_temperley_motus, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "temperley refutationes", _probare_temperley_refutationes, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "via electa", _probare_viam_electam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "via clausa", _probare_viam_clausam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "per viam", _probare_per_viam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tabula tota", _probare_tabulam_totam, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("laqueus_rete", SECTIONES);
}
