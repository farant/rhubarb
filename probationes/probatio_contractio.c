/* probatio_contractio.c - Probationes structurae, ordinis avidi et
 * tabulae
 *
 * Retia manu facta (catena, anulus, rete toroidale 2x2, stella,
 * disiuncta, scalares) cum ordinibus, sumptibus et tabulis manu
 * computatis. Retia fortuita cum computatione INDEPENDENTI in
 * probatione (indices ut mascae bitorum): quisque gradus clavem
 * avidam minimam habet, sumptus recomputantur, tabula per vitas
 * disiuncta est et primo apto posita. Renumeratio indicum ordinem non
 * mutat; idem rete bis aedificatum idem dat. Saturatio ultra 2^64 et
 * latitudo exacta. Vide lib/contractio.worklog.md.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "sors.h"
#include "contractio.h"


/* ==================================================
 * Auxilia
 * ================================================== */

/* rete ex dimensionibus et tensoribus: tensor k = plani[initium_k ..
 * initium_k + longitudines[k]) */
interior ContractioRete*
_rete (
         Piscina* piscina,
    constans i32* dimensiones,
             i32  numerus_indicum,
    constans i32* plani,
    constans i32* longitudines,
             i32  numerus_tensorum)
{
    ContractioRete* rete     = contractio_rete_creare(piscina);
               i32  initium  = ZEPHYRUM;
               i32  j;
               i32  id;

    CREDO_NECESSE_NON_NIHIL(rete);
    per (j = ZEPHYRUM; j < numerus_indicum; j++)
    {
        CREDO_NECESSE(contractio_index_addere(rete, dimensiones[j],
            &id));
        CREDO_NECESSE(id == j);
    }
    per (j = ZEPHYRUM; j < numerus_tensorum; j++)
    {
        CREDO_NECESSE(contractio_tensor_addere(rete, plani + initium,
            longitudines[j], &id));
        CREDO_NECESSE(id == j);
        initium += longitudines[j];
    }
    redde rete;
}

interior ContractioOrdo*
_ordinare (
    ContractioRete* rete,
          Piscina* piscina)
{
    ContractioOptiones  o = contractio_optiones_ordinariae();
        ContractioOrdo* ordo = contractio_ordinare(rete, &o,
            piscina);

    CREDO_NECESSE_NON_NIHIL(ordo);
    redde ordo;
}

/* gradus exspectati: terni (sinister, dexter, effectus) */
interior vacuum
_gradus_probare (
    constans ContractioOrdo* ordo,
              constans i32* exspectati,
                       i32  numerus)
{
    i32 k;
    i32 a;
    i32 b;
    i32 e;

    CREDO_AEQUALIS_I32(contractio_ordo_gradus_numerus(ordo), numerus);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        CREDO_NECESSE(contractio_ordo_gradus(ordo, k, &a, &b, &e));
        CREDO_AEQUALIS_I32(a, exspectati[III * k]);
        CREDO_AEQUALIS_I32(b, exspectati[III * k + I]);
        CREDO_AEQUALIS_I32(e, exspectati[III * k + II]);
    }
    CREDO_FALSUM(contractio_ordo_gradus(ordo, numerus, &a, &b, &e));
}

interior vacuum
_sumptus_probare (
    constans ContractioOrdo* ordo,
                       i64  apex,
                       i64  octeti,
                       i64  multiplicationes,
                       i32  latitudo)
{
    ContractioSumptus s = contractio_sumptus(ordo);

    CREDO_AEQUALIS_I64(s.apex, apex);
    CREDO_AEQUALIS_I64(s.octeti_officinae, octeti);
    CREDO_AEQUALIS_I64(s.multiplicationes, multiplicationes);
    CREDO_AEQUALIS_I32(s.latitudo, latitudo);
}

/* sedes exspectatae intermediorum primus, primus + 1, ...: bini
 * (offset, elementa); initiales et ultra finem refutantur */
interior vacuum
_sedes_probare (
    constans ContractioOrdo* ordo,
                       i32  primus,
              constans i64* exspectatae,
                       i32  numerus)
{
    i64 offset;
    i64 elementa;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        CREDO_NECESSE(contractio_ordo_sedes(ordo, primus + k, &offset,
            &elementa));
        CREDO_AEQUALIS_I64(offset, exspectatae[II * k]);
        CREDO_AEQUALIS_I64(elementa, exspectatae[II * k + I]);
    }
    CREDO_FALSUM(contractio_ordo_sedes(ordo, primus + numerus, &offset,
        &elementa));
    si (primus > ZEPHYRUM)
    {
        CREDO_FALSUM(contractio_ordo_sedes(ordo, primus - I, &offset,
            &elementa));
        CREDO_FALSUM(contractio_ordo_sedes(ordo, ZEPHYRUM, &offset,
            &elementa));
    }
}


/* ==================================================
 * Retia manu computata
 * ================================================== */

interior vacuum
_probare_optiones (
    CredoContextus* c)
{
    ContractioOptiones o = contractio_optiones_ordinariae();

    (vacuum)c;
    CREDO_AEQUALIS_I32(o.methodus, CONTRACTIO_AVIDUS);
}

/* v0(a) M1(a,b) M2(b,c) v3(c), dimensiones 2, 3, 4: paria (0,1) et
 * (2,3) ambo 3 elementa - id minor vincit. Catena quinque (omnia 2):
 * (0,1) et (3,4) aequales; deinde (2,5) ante (3,4) per id minorem;
 * deinde (3,4) ante (3,6) per id maiorem. Tabula: 7 in offset 0
 * (spatium a 5 liberatum) reutitur. */
interior vacuum
_probare_catenam (
    CredoContextus* c)
{
         Piscina* piscina  = c->piscina;
    constans i32  d3[]     = { II, III, IV };
    constans i32  p3[]     = { 0, 0, I, I, II, II };
    constans i32  l3[]     = { I, II, II, I };
    constans i32  g3[]     = { 0, I, IV, II, III, V, IV, V, VI };
    constans i64  s3[]     = { 0, III, III, III, VI, I };
    constans i32  d5[]     = { II, II, II, II };
    constans i32  p5[]     = { 0, 0, I, I, II, II, III, III };
    constans i32  l5[]     = { I, II, II, II, I };
    constans i32  g5[] = { 0, I, V, II, V, VI, III, IV, VII, VI, VII,
        VIII };
      constans i64  s5[] = { 0, II, II, II, 0, II, IV, I };
    ContractioOrdo* ordo;

    ordo = _ordinare(_rete(piscina, d3, III, p3, l3, IV), piscina);
    _gradus_probare(ordo, g3, III);
    _sumptus_probare(ordo, III, XXVIII, XXI, II);
    _sedes_probare(ordo, IV, s3, III);

    ordo = _ordinare(_rete(piscina, d5, IV, p5, l5, V), piscina);
    _gradus_probare(ordo, g5, IV);
    _sumptus_probare(ordo, II, XX, XIV, I);
    _sedes_probare(ordo, V, s5, IV);
}

/* T0(a,b) T1(b,c) T2(c,d) T3(d,a), dimensiones 2, 3, 5, 7 */
interior vacuum
_probare_anulum (
    CredoContextus* c)
{
           Piscina* piscina  = c->piscina;
      constans i32  d[]      = { II, III, V, VII };
      constans i32  p[]      = { 0, I, I, II, II, III, III, 0 };
      constans i32  l[]      = { II, II, II, II };
      constans i32  g[]      = { 0, I, IV, II, III, V, IV, V, VI };
      constans i64  s[]      = { 0, X, X, X, XX, I };
    ContractioOrdo* ordo;

    ordo = _ordinare(_rete(piscina, d, IV, p, l, IV), piscina);
    _gradus_probare(ordo, g, III);
    _sumptus_probare(ordo, X, LXXXIV, CX, IV);
    _sedes_probare(ordo, IV, s, III);
}

/* rete toroidale 2x2: inter vicinos DUO indices (h, h' dimensionis 2;
 * v, v' dimensionis 3) - paria plures indices communes habent */
interior vacuum
_probare_torum (
    CredoContextus* c)
{
         Piscina* piscina  = c->piscina;
    constans i32  d[]      = { II, II, II, II, III, III, III, III };
    constans i32  p[] = { 0, I, IV, V, 0, I, VI, VII, II, III, IV, V,
        II,
        III, VI, VII };
      constans i32  l[] = { IV, IV, IV, IV };
      constans i32  g[] = { 0, II, IV, I, III, V, IV, V, VI };
      constans i64  s[] = { 0, XVI, XVI, XVI, XXXII, I };
    ContractioOrdo* ordo;

    ordo = _ordinare(_rete(piscina, d, VIII, p, l, IV), piscina);
    _gradus_probare(ordo, g, III);
    _sumptus_probare(ordo, XVI, CXXXII, CCCIV, IV);
    _sedes_probare(ordo, IV, s, III);
}

/* centrum C(a,b,c,d) cum quattuor foliis, dimensiones 2, 3, 4, 5:
 * folium maximum primum; 7 in offset 0 reutitur, 8 in 2 */
interior vacuum
_probare_stellam (
    CredoContextus* c)
{
         Piscina* piscina  = c->piscina;
    constans i32  d[]      = { II, III, IV, V };
    constans i32  p[]      = { 0, I, II, III, 0, I, II, III };
    constans i32  l[]      = { IV, I, I, I, I };
    constans i32  g[] = { 0, IV, V, III, V, VI, II, VI, VII, I, VII,
        VIII };
      constans i64  s[] = { 0, XXIV, XXIV, VI, 0, II, II, I };
    ContractioOrdo* ordo;

    ordo = _ordinare(_rete(piscina, d, IV, p, l, V), piscina);
    _gradus_probare(ordo, g, IV);
    _sumptus_probare(ordo, XXIV, CXX, CLII, V);
    _sedes_probare(ordo, V, s, IV);
}

/* componentes disiunctae (productum externum in fine) et tensores
 * sine indicibus */
interior vacuum
_probare_disiunctas (
    CredoContextus* c)
{
         Piscina* piscina  = c->piscina;
    constans i32  d1[]     = { II, III, V };
    constans i32  p1[]     = { 0, I, 0, I, II, II };
    constans i32  l1[]     = { II, II, I, I };
    constans i32  g1[]     = { 0, I, IV, II, III, V, IV, V, VI };
    constans i64  s1[]     = { 0, I, I, I, II, I };
    constans i32  d2[]     = { VII };
    constans i32  p2[]     = { 0, 0 };
    constans i32  l2[]     = { 0, 0, I, I, 0 };
    constans i32  g2[] = { II, III, V, 0, I, VI, IV, V, VII, VI, VII,
        VIII };
      constans i64  s2[] = { 0, I, I, I, II, I, 0, I };
    ContractioOrdo* ordo;

    ordo = _ordinare(_rete(piscina, d1, III, p1, l1, IV), piscina);
    _gradus_probare(ordo, g1, III);
    _sumptus_probare(ordo, I, XII, XII, 0);
    _sedes_probare(ordo, IV, s1, III);

    ordo = _ordinare(_rete(piscina, d2, I, p2, l2, V), piscina);
    _gradus_probare(ordo, g2, IV);
    _sumptus_probare(ordo, I, XII, X, 0);
    _sedes_probare(ordo, V, s2, IV);
}

/* rete vacuum, scalaris solus, duo scalares */
interior vacuum
_probare_vacuum (
    CredoContextus* c)
{
           Piscina* piscina  = c->piscina;
      constans i32  nulli[]  = { 0 };
      constans i32  l1[]     = { 0 };
      constans i32  l2[]     = { 0, 0 };
      constans i32  g2[]     = { 0, I, II };
      constans i64  s2[]     = { 0, I };
    ContractioRete* rete;
    ContractioOrdo* ordo;
               i64  offset;
               i64  elementa;

    rete = contractio_rete_creare(piscina);
    CREDO_VERUM(contractio_rete_clausum(rete));
    ordo = _ordinare(rete, piscina);
    _gradus_probare(ordo, NIHIL, 0);
    _sumptus_probare(ordo, 0, 0, 0, 0);
    CREDO_FALSUM(contractio_ordo_sedes(ordo, 0, &offset, &elementa));
    CREDO_NON_NIHIL(contractio_officina_creare(ordo, 0, piscina));

    ordo = _ordinare(_rete(piscina, nulli, 0, nulli, l1, I), piscina);
    _gradus_probare(ordo, NIHIL, 0);
    _sumptus_probare(ordo, 0, 0, 0, 0);
    CREDO_FALSUM(contractio_ordo_sedes(ordo, 0, &offset, &elementa));
    CREDO_NON_NIHIL(contractio_officina_creare(ordo, 0, piscina));

    ordo = _ordinare(_rete(piscina, nulli, 0, nulli, l2, II), piscina);
    _gradus_probare(ordo, g2, I);
    _sumptus_probare(ordo, I, IV, I, 0);
    _sedes_probare(ordo, II, s2, I);

    _sumptus_probare(NIHIL, 0, 0, 0, 0);
    CREDO_AEQUALIS_I32(contractio_ordo_gradus_numerus(NIHIL), 0);
}


/* ==================================================
 * Saturatio et latitudo exacta
 * ================================================== */

interior vacuum
_probare_saturata (
    CredoContextus* c)
{
         Piscina* piscina  = c->piscina;
    constans i32  t        = (i32)3486784401UL;              /* 3^20 */
    constans i32  vicies   = (i32)0x100000UL;           /* 2^20 */
    constans i32  tricies  = (i32)0x40000000UL;        /* 2^30 */
    constans i32  tres[]   = { t, t, t, t };
    constans i32  ps[]     = { 0, I, II, III, 0, I, II, III };
    constans i32  ls[]     = { IV, I, I, I, I };
    constans i32  gs[] = { 0, I, V, II, V, VI, III, VI, VII, IV, VII,
        VIII };
    constans i32 dv[] = { vicies, vicies, vicies, vicies, vicies,
        vicies };
    constans i32 pv[] = { 0, I, II, III, II, III, IV, V, IV, V, 0, I };
    constans i32 lv[] = { IV, IV, IV };
    constans i32 pt[] = { 0, I, I, II, II, 0 };
    constans i32 lt[] = { II, II, II };
    constans i32 ge[] = { 0, I, III, II, III, IV };
    constans i32 de[] = { (i32)42007935UL, (i32)0xFFFFFFFFUL,
        (i32)0xFFFFFFFFUL, (i32)65537UL, (i32)6700417UL };
    constans i32  pe[]  = { 0, I, II, I, II, III, IV, III, IV, 0 };
    constans i32  le[]  = { III, IV, III };
    constans i32  dn[]  = { tricies, tricies, tricies };
    constans i32  pn[]  = { 0, I, II, 0, I, II };
    constans i32  ln[]  = { III, III };
    constans i32  gn[]  = { 0, I, II };
    constans i64  tt    = (i64)t * (i64)t;               /* 3^40 < 2^64 */
             i64  sedes_t[IV];
    ContractioOrdo* ordo;
               i64  offset;
               i64  elementa;

    /* stella 3^20: primus effectus 3^60 (saturatus), latitudo
     * ceil(60 log2 3) = 96; omnes claves saturatae -> id minimi */
    ordo = _ordinare(_rete(piscina, tres, IV, ps, ls, V), piscina);
    _gradus_probare(ordo, gs, IV);
    _sumptus_probare(ordo, CONTRACTIO_SATURATUM, CONTRACTIO_SATURATUM,
        CONTRACTIO_SATURATUM, XCVI);
    CREDO_FALSUM(contractio_ordo_sedes(ordo, V, &offset, &elementa));
    CREDO_NIHIL(contractio_officina_creare(ordo, CONTRACTIO_SATURATUM,
        piscina));

    /* trigonum indicum binorum 2^20: intermedius 2^80, latitudo 80 */
    ordo = _ordinare(_rete(piscina, dv, VI, pv, lv, III), piscina);
    _gradus_probare(ordo, ge, II);
    _sumptus_probare(ordo, CONTRACTIO_SATURATUM, CONTRACTIO_SATURATUM,
        CONTRACTIO_SATURATUM, LXXX);

    /* trigonum 3^20: intermedius 3^40 CAPIT (latitudo 64), sed tabula
     * 3^40 + 1 elementorum in octetis saturatur; sedes tamen notae */
    ordo = _ordinare(_rete(piscina, tres, III, pt, lt, III), piscina);
    _gradus_probare(ordo, ge, II);
    _sumptus_probare(ordo, tt, CONTRACTIO_SATURATUM,
        CONTRACTIO_SATURATUM,
        LXIV);
    sedes_t[0]    = 0;
    sedes_t[I]    = tt;
    sedes_t[II]   = tt;
    sedes_t[III]  = I;
    _sedes_probare(ordo, III, sedes_t, II);

    /* intermedius EXACTE 2^64 - 1 = 42007935 * 65537 * 6700417:
     * valor saturato aequalis, latitudo 64 per magnum */
    ordo = _ordinare(_rete(piscina, de, V, pe, le, III), piscina);
    _gradus_probare(ordo, ge, II);
    _sumptus_probare(ordo, CONTRACTIO_SATURATUM, CONTRACTIO_SATURATUM,
        CONTRACTIO_SATURATUM, LXIV);

    /* multiplicationes 2^90 saturatae, tabula unius elementi:
     * officina tamen refutatur */
    ordo = _ordinare(_rete(piscina, dn, III, pn, ln, II), piscina);
    _gradus_probare(ordo, gn, I);
    _sumptus_probare(ordo, I, IV, CONTRACTIO_SATURATUM, 0);
    CREDO_NIHIL(contractio_officina_creare(ordo, CONTRACTIO_SATURATUM,
        piscina));
}


/* ==================================================
 * Retia fortuita: computatio independens
 * ================================================== */

#define FORTUITI_TENSORES XII
#define FORTUITI_INDICES  XX

nomen structura {
    i32 numerus_indicum;
    i32 numerus_tensorum;
    i32 dimensiones[FORTUITI_INDICES];
    i32 plani[II * FORTUITI_INDICES];
    i32 longitudines[FORTUITI_TENSORES];
} Fortuitum;

/* n tensores (2..12), m indices (0..20) dimensionis 1..4 inter duos
 * tensores distinctos fortuitos; ordo indicum in tensore permixtus */
interior vacuum
_fortuitum (
         Sors* s,
    Fortuitum* f)
{
    i32 tabula[FORTUITI_TENSORES][II * FORTUITI_INDICES];
    i32 numeri[FORTUITI_TENSORES];
    i32 j;
    i32 k;
    i32 initium = ZEPHYRUM;

    f->numerus_tensorum  = II + sors_intra(s, XI);
    f->numerus_indicum   = sors_intra(s, XXI);
    per (k = ZEPHYRUM; k < f->numerus_tensorum; k++)
    {
        numeri[k] = ZEPHYRUM;
    }
    per (j = ZEPHYRUM; j < f->numerus_indicum; j++)
    {
        i32 a = sors_intra(s, f->numerus_tensorum);
        i32 b = sors_intra(s, f->numerus_tensorum - I);

        si (b >= a)
        {
            b++;
        }
        f->dimensiones[j]       = I + sors_intra(s, IV);
        tabula[a][numeri[a]++]  = j;
        tabula[b][numeri[b]++]  = j;
    }
    per (k = ZEPHYRUM; k < f->numerus_tensorum; k++)
    {
        per (j = numeri[k]; j > I; j--)
        {
            i32 r = sors_intra(s, j);
            i32 x = tabula[k][j - I];

            tabula[k][j - I]  = tabula[k][r];
            tabula[k][r]      = x;
        }
        f->longitudines[k] = numeri[k];
        per (j = ZEPHYRUM; j < numeri[k]; j++)
        {
            f->plani[initium++] = tabula[k][j];
        }
    }
}

interior ContractioRete*
_rete_fortuitum (
               Piscina* piscina,
    constans Fortuitum* f)
{
    redde _rete(piscina, f->dimensiones, f->numerus_indicum, f->plani,
        f->longitudines, f->numerus_tensorum);
}

interior i64
_productum (
    constans Fortuitum* f,
                   i32  masca)
{
    i64 p = I;
    i32 j;

    per (j = ZEPHYRUM; j < f->numerus_indicum; j++)
    {
        si ((masca >> j) & I)
        {
            p *= f->dimensiones[j];
        }
    }
    redde p;
}

interior i32
_bita (
    i64 x)
{
    i32 n = ZEPHYRUM;

    dum (x != ZEPHYRUM)
    {
        n++;
        x >>= I;
    }
    redde n;
}

/* ordo et tabula contra computationem independentem */
interior vacuum
_independens_probare (
    constans ContractioOrdo* ordo,
          constans Fortuitum* f)
{
                  i32 masca[II * FORTUITI_TENSORES];
                  b32 vivus[II * FORTUITI_TENSORES];
                  i32 natus[II * FORTUITI_TENSORES];
                  i32 mortuus[II * FORTUITI_TENSORES];
                  i64 offsets[II * FORTUITI_TENSORES];
                  i64 magnitudines[II * FORTUITI_TENSORES];
                  i32 n                 = f->numerus_tensorum;
                  i32 gradus            = n - I;
                  i64 multiplicationes  = ZEPHYRUM;
                  i64 apex              = ZEPHYRUM;
                  i64 summum            = ZEPHYRUM;
                  i32 latitudo          = ZEPHYRUM;
                  i32 initium           = ZEPHYRUM;
                  i32 s;
                  i32 j;
                  i32 k;
                  i32 x;
                  i32 y;
    ContractioSumptus sumptus = contractio_sumptus(ordo);

    per (k = ZEPHYRUM; k < n; k++)
    {
        masca[k] = ZEPHYRUM;
        per (j = ZEPHYRUM; j < f->longitudines[k]; j++)
        {
            masca[k] |= (i32)I << f->plani[initium + j];
        }
        initium   += f->longitudines[k];
        vivus[k]  = VERUM;
    }
    CREDO_AEQUALIS_I32(contractio_ordo_gradus_numerus(ordo), gradus);
    per (s = ZEPHYRUM; s < gradus; s++)
    {
        i32 a;
        i32 b;
        i32 e;
        i64 clavis;
        b32 vicini = FALSUM;

        CREDO_NECESSE(contractio_ordo_gradus(ordo, s, &a, &b, &e));
        CREDO_NECESSE(e == n + s && a < b && b < e);
        CREDO_NECESSE(vivus[a] && vivus[b]);

        /* clavis avida: nullum par candidatum minus */
        clavis = _productum(f, masca[a] ^ masca[b]);
        per (x = ZEPHYRUM; x < e; x++)
        {
            per (y = x + I; y < e; y++)
            {
                si (vivus[x] && vivus[y] && (masca[x] & masca[y]) != 0)
                {
                    vicini = VERUM;
                }
            }
        }
        si (vicini)
        {
            CREDO_VERUM((masca[a] & masca[b]) != 0);
        }
        per (x = ZEPHYRUM; x < e; x++)
        {
            per (y = x + I; y < e; y++)
            {
                i64 k2;

                si (   !vivus[x] || !vivus[y]
                    || (vicini && (masca[x] & masca[y]) == 0))
                {
                    perge;
                }
                k2 = _productum(f, masca[x] ^ masca[y]);
                CREDO_VERUM(k2 > clavis
                    || (k2 == clavis && (x > a || (x == a && y >= b))));
            }
        }

        masca[e]          = masca[a] ^ masca[b];
        multiplicationes  += _productum(f, masca[a] | masca[b]);
        si (clavis > apex)
        {
            apex = clavis;
        }
        si (_bita(clavis - I) > latitudo)
        {
            latitudo = _bita(clavis - I);
        }
        vivus[a]    = FALSUM;
        vivus[b]    = FALSUM;
        vivus[e]    = VERUM;
        natus[e]    = s;
        mortuus[e]  = gradus;
        si (a >= n)
        {
            mortuus[a] = s;
        }
        si (b >= n)
        {
            mortuus[b] = s;
        }
        CREDO_NECESSE(contractio_ordo_sedes(ordo, e, &offsets[e],
            &magnitudines[e]));
        CREDO_AEQUALIS_I64(magnitudines[e], clavis);
        si (offsets[e] + magnitudines[e] > summum)
        {
            summum = offsets[e] + magnitudines[e];
        }
    }
    CREDO_AEQUALIS_I64(sumptus.apex, apex);
    CREDO_AEQUALIS_I64(sumptus.multiplicationes, multiplicationes);
    CREDO_AEQUALIS_I32(sumptus.latitudo, latitudo);
    CREDO_AEQUALIS_I64(sumptus.octeti_officinae, summum * IV);

    /* tabula: vitae superpositae -> intervalla disiuncta; offset
     * primum aptum inter vivos ad natum */
    per (x = n; x < n + gradus; x++)
    {
        per (y = n; y < n + gradus; y++)
        {
            si (   x == y || natus[x] > mortuus[y]
                || natus[y] > mortuus[x])
            {
                perge;
            }
            CREDO_VERUM(offsets[x] + magnitudines[x] <= offsets[y]
                || offsets[y] + magnitudines[y] <= offsets[x]);
        }
        per (y = n - I; y < n + gradus; y++)
        {
            i64 locus;
            b32 aptus = VERUM;

            si (y == n - I)
            {
                locus = ZEPHYRUM;
            }
            alioquin
            {
                si (natus[y] >= natus[x] || mortuus[y] < natus[x])
                {
                    perge;
                }
                locus = offsets[y] + magnitudines[y];
            }
            si (locus >= offsets[x])
            {
                perge;
            }
            per (k = n; k < n + gradus; k++)
            {
                si (   natus[k] < natus[x] && mortuus[k] >= natus[x]
                    && locus < offsets[k] + magnitudines[k]
                    && offsets[k] < locus + magnitudines[x])
                {
                    aptus = FALSUM;
                }
            }
            CREDO_FALSUM(aptus);
        }
    }
}

interior vacuum
_probare_fortuita (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;

    sors_seminare(&s, (i64)MMXXVI, ZEPHYRUM);
    per (t = ZEPHYRUM; t < CCC; t++)
    {
        Fortuitum f;

        _fortuitum(&s, &f);
        _independens_probare(_ordinare(_rete_fortuitum(piscina, &f),
            piscina), &f);
    }
}

/* idem rete bis; deinde indices renumerati: gradus, sumptus, sedes
 * aequales */
interior vacuum
_ordines_aequales (
    constans ContractioOrdo* o1,
    constans ContractioOrdo* o2)
{
    ContractioSumptus s1 = contractio_sumptus(o1);
    ContractioSumptus s2 = contractio_sumptus(o2);
                  i32 numerus  =
                      contractio_ordo_gradus_numerus(o1);
                  i32 k;

    CREDO_AEQUALIS_I32(contractio_ordo_gradus_numerus(o2), numerus);
    CREDO_AEQUALIS_I64(s1.apex, s2.apex);
    CREDO_AEQUALIS_I64(s1.octeti_officinae, s2.octeti_officinae);
    CREDO_AEQUALIS_I64(s1.multiplicationes, s2.multiplicationes);
    CREDO_AEQUALIS_I32(s1.latitudo, s2.latitudo);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        i32 a1;
        i32 b1;
        i32 e1;
        i32 a2;
        i32 b2;
        i32 e2;
        i64 f1;
        i64 m1;
        i64 f2;
        i64 m2;

        CREDO_NECESSE(contractio_ordo_gradus(o1, k, &a1, &b1, &e1));
        CREDO_NECESSE(contractio_ordo_gradus(o2, k, &a2, &b2, &e2));
        CREDO_AEQUALIS_I32(a1, a2);
        CREDO_AEQUALIS_I32(b1, b2);
        CREDO_AEQUALIS_I32(e1, e2);
        CREDO_NECESSE(contractio_ordo_sedes(o1, e1, &f1, &m1));
        CREDO_NECESSE(contractio_ordo_sedes(o2, e2, &f2, &m2));
        CREDO_AEQUALIS_I64(f1, f2);
        CREDO_AEQUALIS_I64(m1, m2);
    }
}

interior vacuum
_probare_permutationem (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;

    sors_seminare(&s, (i64)MMXXVII, ZEPHYRUM);
    per (t = ZEPHYRUM; t < CC; t++)
    {
             Fortuitum  f;
             Fortuitum  g;
                   i32  permutatio[FORTUITI_INDICES];
                   i32  j;
                   i32  initium = ZEPHYRUM;
        ContractioOrdo* o1;
        ContractioOrdo* o2;
        ContractioOrdo* o3;

        _fortuitum(&s, &f);
        per (j = ZEPHYRUM; j < f.numerus_indicum; j++)
        {
            permutatio[j] = j;
        }
        per (j = f.numerus_indicum; j > I; j--)
        {
            i32 r = sors_intra(&s, j);
            i32 x = permutatio[j - I];

            permutatio[j - I]  = permutatio[r];
            permutatio[r]      = x;
        }
        g = f;
        per (j = ZEPHYRUM; j < f.numerus_indicum; j++)
        {
            g.dimensiones[permutatio[j]] = f.dimensiones[j];
        }
        per (j = ZEPHYRUM; j < f.numerus_tensorum; j++)
        {
            initium += f.longitudines[j];
        }
        per (j = ZEPHYRUM; j < initium; j++)
        {
            g.plani[j] = permutatio[f.plani[j]];
        }
        o1 = _ordinare(_rete_fortuitum(piscina, &f), piscina);
        o2 = _ordinare(_rete_fortuitum(piscina, &f), piscina);
        o3 = _ordinare(_rete_fortuitum(piscina, &g), piscina);
        _ordines_aequales(o1, o2);
        _ordines_aequales(o1, o3);
    }
}


/* ==================================================
 * Refutationes et officina
 * ================================================== */

interior vacuum
_probare_refutationes (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
        ContractioRete* rete;
    ContractioOptiones  o = contractio_optiones_ordinariae();
                   i32  a;
                   i32  b;
                   i32  d;
                   i32  id = (i32)0xDEADUL;
                   i32  indices[III];

    CREDO_NIHIL(contractio_rete_creare(NIHIL));
    rete = contractio_rete_creare(piscina);
    CREDO_FALSUM(contractio_index_addere(rete, 0, &id));
    CREDO_AEQUALIS_I32(id, (i32)0xDEADUL);
    CREDO_FALSUM(contractio_index_addere(rete, II, NIHIL));
    CREDO_FALSUM(contractio_index_addere(NIHIL, II, &id));
    CREDO_VERUM(contractio_index_addere(rete, II, &a));
    CREDO_VERUM(contractio_index_addere(rete, (i32)0xFFFFFFFFUL, &b));
    CREDO_AEQUALIS_I32(a, 0);
    CREDO_AEQUALIS_I32(b, I);

    /* index bis, index ignotus, indices NIHIL: rete non mutatur */
    indices[0]   = a;
    indices[I]   = b;
    indices[II]  = b;
    CREDO_FALSUM(contractio_tensor_addere(rete, indices, III, &id));
    indices[II] = a;
    CREDO_FALSUM(contractio_tensor_addere(rete, indices, III, &id));
    indices[I] = VII;
    CREDO_FALSUM(contractio_tensor_addere(rete, indices, II, &id));
    CREDO_FALSUM(contractio_tensor_addere(rete, NIHIL, I, &id));
    CREDO_AEQUALIS_I32(id, (i32)0xDEADUL);

    /* apertum: ordinare refutat, rete mutabile manet */
    indices[I] = b;
    CREDO_VERUM(contractio_tensor_addere(rete, indices, II, &id));
    CREDO_AEQUALIS_I32(id, 0);
    CREDO_FALSUM(contractio_rete_clausum(rete));
    CREDO_NIHIL(contractio_ordinare(rete, &o, piscina));
    CREDO_VERUM(contractio_tensor_addere(rete, indices, II, &id));
    CREDO_AEQUALIS_I32(id, I);
    CREDO_VERUM(contractio_rete_clausum(rete));

    /* usus tertius */
    CREDO_FALSUM(contractio_tensor_addere(rete, indices, I, &id));
    CREDO_AEQUALIS_I32(id, I);

    /* index sine usu: non clausum */
    CREDO_VERUM(contractio_index_addere(rete, III, &d));
    CREDO_FALSUM(contractio_rete_clausum(rete));
    indices[0] = d;
    CREDO_VERUM(contractio_tensor_addere(rete, indices, I, &id));
    CREDO_VERUM(contractio_tensor_addere(rete, indices, I, &id));
    CREDO_VERUM(contractio_rete_clausum(rete));

    /* optiones invalidae: refutatae ante congelationem */
    o.methodus = 0;
    CREDO_NIHIL(contractio_ordinare(rete, &o, piscina));
    o.methodus = II;
    CREDO_NIHIL(contractio_ordinare(rete, &o, piscina));
    CREDO_NIHIL(contractio_ordinare(rete, NIHIL, piscina));
    o = contractio_optiones_ordinariae();
    CREDO_NIHIL(contractio_ordinare(rete, &o, NIHIL));
    CREDO_NIHIL(contractio_ordinare(NIHIL, &o, piscina));
    CREDO_VERUM(contractio_index_addere(rete, II, &id));
    indices[0] = id;
    CREDO_VERUM(contractio_tensor_addere(rete, indices, I, &id));
    CREDO_VERUM(contractio_tensor_addere(rete, indices, I, &id));

    /* post ordinare: immutabile; ordinare iterum licet */
    CREDO_NON_NIHIL(contractio_ordinare(rete, &o, piscina));
    CREDO_FALSUM(contractio_index_addere(rete, II, &id));
    CREDO_FALSUM(contractio_tensor_addere(rete, NIHIL, 0, &id));
    CREDO_NON_NIHIL(contractio_ordinare(rete, &o, piscina));
}

interior vacuum
_probare_officinam (
    CredoContextus* c)
{
               Piscina* piscina  = c->piscina;
          constans i32  d[]      = { II, III, IV };
          constans i32  p[]      = { 0, 0, I, I, II, II };
          constans i32  l[]      = { I, II, II, I };
        ContractioRete* rete     = _rete(piscina, d, III, p, l, IV);
        ContractioOrdo* ordo     = _ordinare(rete, piscina);
    ContractioOfficina* officina;
                   i32  exitus = (i32)0xDEADUL;

    CREDO_NIHIL(contractio_officina_creare(ordo, XXVII, piscina));
    CREDO_NIHIL(contractio_officina_creare(NIHIL, C, piscina));
    CREDO_NIHIL(contractio_officina_creare(ordo, C, NIHIL));
    officina = contractio_officina_creare(ordo, XXVIII, piscina);
    CREDO_NON_NIHIL(officina);

    /* T2: exsecutio nondum - refutatur, exitus intactus */
    CREDO_FALSUM(contractio_exsequi_modulo(rete, ordo, officina, NIHIL,
        XVII, &exitus));
    CREDO_FALSUM(contractio_exsequi_anulo(rete, ordo, NIHIL, NIHIL,
        piscina, &exitus));
    CREDO_AEQUALIS_I32(exitus, (i32)0xDEADUL);
}


/* ==================================================
 * Suita
 * ================================================== */

interior constans CredoSectio SECTIONES[] = {
    { "optiones ordinariae", _probare_optiones, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "catena", _probare_catenam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "anulus", _probare_anulum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "torus", _probare_torum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stella", _probare_stellam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "disiuncta et scalares", _probare_disiunctas, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "vacuum", _probare_vacuum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "saturatio", _probare_saturata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fortuita independenter", _probare_fortuita, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "renumeratio et determinatio", _probare_permutationem, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "refutationes", _probare_refutationes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "officina", _probare_officinam, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("contractio", SECTIONES);
}
