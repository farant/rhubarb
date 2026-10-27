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
#include "magnus.h"
#include "polynomium.h"
#include "congruentia.h"
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
        constans i32* lista;
                 i32  longitudo;

        /* initiales: indices ordine vocantis */
        CREDO_NECESSE(contractio_ordo_indices(ordo, k, &lista,
            &longitudo));
        CREDO_AEQUALIS_I32(longitudo, f->longitudines[k]);
        masca[k] = ZEPHYRUM;
        per (j = ZEPHYRUM; j < f->longitudines[k]; j++)
        {
            masca[k] |= (i32)I << f->plani[initium + j];
            CREDO_AEQUALIS_I32(lista[j], f->plani[initium + j]);
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

        /* intermedius: differentia symmetrica, ordine id ascendente */
        {
            constans i32* lista;
                     i32  longitudo;
                     i32  masca_listae = ZEPHYRUM;

            CREDO_NECESSE(contractio_ordo_indices(ordo, e, &lista,
                &longitudo));
            per (j = ZEPHYRUM; j < longitudo; j++)
            {
                masca_listae |= (i32)I << lista[j];
                si (j > ZEPHYRUM)
                {
                    CREDO_VERUM(lista[j - I] < lista[j]);
                }
            }
            CREDO_AEQUALIS_I32(masca_listae, masca[e]);
        }
    }
    {
        constans i32* lista      = NIHIL;
                 i32  longitudo  = (i32)0xDEADUL;

        CREDO_FALSUM(contractio_ordo_indices(ordo, n + gradus, &lista,
            &longitudo));
        CREDO_AEQUALIS_I32(longitudo, (i32)0xDEADUL);
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

    CREDO_NIHIL(contractio_officina_creare(ordo, XXVII, piscina));
    CREDO_NIHIL(contractio_officina_creare(NIHIL, C, piscina));
    CREDO_NIHIL(contractio_officina_creare(ordo, C, NIHIL));
    officina = contractio_officina_creare(ordo, XXVIII, piscina);
    CREDO_NON_NIHIL(officina);
}

/* cauda scalarium: ordo septem (paria id minimorum) et 4000 scalares
 * sine scansione O(L^2) per gradum (ante: ~14 s) */
interior vacuum
_ordinare_multos_vacuos (
    Piscina* piscina)
{
    ContractioRete* rete = contractio_rete_creare(piscina);
               i32  id;
               i32  k;

    per (k = ZEPHYRUM; k < (i32)4000UL; k++)
    {
        (vacuum)contractio_tensor_addere(rete, NIHIL, ZEPHYRUM, &id);
    }
    (vacuum)_ordinare(rete, piscina);
}

interior vacuum
_probare_vacuos_multos (
    CredoContextus* c)
{
         Piscina* piscina  = c->piscina;
    constans i32  nulli[]  = { 0 };
    constans i32  l[]      = { 0, 0, 0, 0, 0, 0, 0 };
    constans i32  g[]     = { 0, I, VII, II, III, VIII, IV, V, IX, VI,
        VII,
        X, VIII, IX, XI, X, XI, XII };
    ContractioOrdo* ordo;

    ordo = _ordinare(_rete(piscina, nulli, 0, nulli, l, VII), piscina);
    _gradus_probare(ordo, g, VI);
    _sumptus_probare(ordo, I, XVI, VI, 0);
    CREDO_NON_PENDET(_ordinare_multos_vacuos(piscina), MM);
}


/* ==================================================
 * Exsecutio (T3)
 * ================================================== */

#define PARVI_TENSORES VII
#define PARVI_INDICES  IX

/* rete parvum pro vi bruta: n tensores (1..7), m indices (0..9; n ==
 * 1 -> 0) dimensionis 1..3 */
interior vacuum
_fortuitum_parvum (
         Sors* s,
    Fortuitum* f)
{
    i32 tabula[PARVI_TENSORES][II * PARVI_INDICES];
    i32 numeri[PARVI_TENSORES];
    i32 j;
    i32 k;
    i32 initium = ZEPHYRUM;

    f->numerus_tensorum = I + sors_intra(s, PARVI_TENSORES);
    f->numerus_indicum  = f->numerus_tensorum == I ? ZEPHYRUM
        : sors_intra(s, PARVI_INDICES + I);
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
        f->dimensiones[j]       = I + sors_intra(s, V);
        tabula[a][numeri[a]++]  = j;
        tabula[b][numeri[b]++]  = j;
    }

    /* dimensiones 1..5, sed assignationes vis brutae <= 2^16:
     * dimensio maxima minuitur donec productum capit */
    per (;;)
    {
        i64 productum  = I;
        i32 maxima     = ZEPHYRUM;

        per (j = ZEPHYRUM; j < f->numerus_indicum; j++)
        {
            productum *= f->dimensiones[j];
            si (f->dimensiones[j] > f->dimensiones[maxima])
            {
                maxima = j;
            }
        }
        si (productum <= (i64)0x10000UL)
        {
            frange;
        }
        f->dimensiones[maxima]--;
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

/* elementa tensoris k */
interior i64
_elementa_tensoris (
    constans Fortuitum* f,
                   i32  k)
{
    i64 e        = I;
    i32 initium  = ZEPHYRUM;
    i32 j;

    per (j = ZEPHYRUM; j < k; j++)
    {
        initium += f->longitudines[j];
    }
    per (j = ZEPHYRUM; j < f->longitudines[k]; j++)
    {
        e *= f->dimensiones[f->plani[initium + j]];
    }
    redde e;
}

/* tabulae valorum (vacuae) pro omnibus tensoribus, elementa i32 */
interior i32**
_valores_parare (
    constans Fortuitum* f,
               Piscina* piscina)
{
    i32** v = (i32**)piscina_allocare(piscina,
        (memoriae_index)(f->numerus_tensorum + I) * magnitudo(i32*));
    i32 k;

    per (k = ZEPHYRUM; k < f->numerus_tensorum; k++)
    {
        v[k] = (i32*)piscina_allocare(piscina,
            (memoriae_index)_elementa_tensoris(f, k) * magnitudo(i32));
    }
    redde v;
}

interior vacuum
_valores_implere (
                  Sors*  s,
    constans Fortuitum*  f,
                   i32** valores,
                   i32   modulus)
{
    i32 k;
    i64 j;

    per (k = ZEPHYRUM; k < f->numerus_tensorum; k++)
    {
        per (j = ZEPHYRUM; j < _elementa_tensoris(f, k); j++)
        {
            valores[k][j] = sors_intra(s, modulus);
        }
    }
}

/* vis bruta: summa super OMNES assignationes indicum producti
 * elementorum, modulo p (oraculum sine ordine et sine tabula) */
interior i32
_vis_bruta (
    constans Fortuitum* f,
           i32* constans* valores,
                    i32  modulus)
{
    i32 assignatio[FORTUITI_INDICES];
    i64 summa     = ZEPHYRUM;
    b32 reliquum  = VERUM;
    i32 j;

    per (j = ZEPHYRUM; j < f->numerus_indicum; j++)
    {
        assignatio[j] = ZEPHYRUM;
    }
    dum (reliquum)
    {
        i64 productum  = I % (i64)modulus;
        i32 initium    = ZEPHYRUM;
        i32 k;

        per (k = ZEPHYRUM; k < f->numerus_tensorum; k++)
        {
            i64 offset = ZEPHYRUM;

            per (j = ZEPHYRUM; j < f->longitudines[k]; j++)
            {
                i32 index = f->plani[initium + j];

                offset = offset * f->dimensiones[index]
                    + assignatio[index];
            }
            productum  = productum * valores[k][offset] % modulus;
            initium    += f->longitudines[k];
        }
        summa = (summa + productum) % modulus;

        reliquum  = FALSUM;
        j         = f->numerus_indicum;
        dum (j > ZEPHYRUM)
        {
            j--;
            assignatio[j]++;
            si (assignatio[j] < f->dimensiones[j])
            {
                reliquum = VERUM;
                frange;
            }
            assignatio[j] = ZEPHYRUM;
        }
    }
    redde (i32)summa;
}

/* CC retia parva: gradus calidus = vis bruta = gradus referentiae
 * super Z/n, n a 2 usque ad 2^32 - 1 (non primus quoque) */
interior vacuum
_probare_vim_brutam (
    CredoContextus* c)
{
         Piscina* piscina = c->piscina;
    constans i32  moduli[] = { II, XVII, (i32)998244353UL,
        (i32)4294967291UL, (i32)0xFFFFFFFFUL };
    Sors s;
     i32 t;

    sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
    per (t = ZEPHYRUM; t < CC; t++)
    {
                 Fortuitum  f;
                       i32  p = moduli[t % V];
           constans Anulus* anulus_modulo = anulus_residuorum(p,
               piscina);
            ContractioRete*  rete;
            ContractioOrdo*  ordo;
        ContractioOfficina*  officina;
                       i32** valores;
                       i32   calidum     = (i32)0xDEADUL;
                       i32   referentia  = (i32)0xDEADUL;

        _fortuitum_parvum(&s, &f);
        rete = _rete_fortuitum(piscina, &f);
        ordo = _ordinare(rete, piscina);
        officina = contractio_officina_creare(ordo,
            CONTRACTIO_SATURATUM,
            piscina);
        CREDO_NECESSE_NON_NIHIL(officina);
        valores = _valores_parare(&f, piscina);
        _valores_implere(&s, &f, valores, p);
        si (t % III == ZEPHYRUM)
        {
            /* tertium quodque rete: omnia n - 1 (portatio maxima) */
            i32 k;
            i64 j;

            per (k = ZEPHYRUM; k < f.numerus_tensorum; k++)
            {
                per (j = ZEPHYRUM; j < _elementa_tensoris(&f, k); j++)
                {
                    valores[k][j] = p - I;
                }
            }
        }
        CREDO_NECESSE(contractio_exsequi_modulo(rete, ordo, officina,
            (constans i32* constans*)valores, p, &calidum));
        CREDO_AEQUALIS_I32(calidum, _vis_bruta(&f, valores, p));
        CREDO_NECESSE(contractio_exsequi_anulo(rete, ordo,
            (constans vacuum* constans*)valores, anulus_modulo, piscina,
            &referentia));
        CREDO_AEQUALIS_I32(referentia, calidum);
    }
}

/* polynomium Laurentianum modulo p in x (x invertibilis) */
interior i32
_aestimare_modulo (
    Polynomium q,
           i32 x,
           i32 p)
{
    i32 summa = ZEPHYRUM;
    i32 inversa;
    s32 e;

    si (polynomium_est_nullum(q))
    {
        redde ZEPHYRUM;
    }
    CREDO_NECESSE(congruentia_inversa(x, p, &inversa));
    per (e = polynomium_gradus_imus(q); e
        <= polynomium_gradus_summus(q);
         e++)
    {
        i32 coefficiens =
            magnus_residuum_parvum(polynomium_coefficiens(q,
            e), p);
        i32 potentia = e >= ZEPHYRUM
            ? congruentia_potentia(x, (i64)e, p)
            : congruentia_potentia(inversa, (i64)(-e), p);

        summa = congruentia_adde(summa,
            congruentia_multiplica(coefficiens,
            potentia, p), p);
    }
    redde summa;
}

/* gradus referentiae super Z et Z[t, t^-1]: effectus exactus
 * reductus (aut aestimatus in x) modulo p = gradus calidus super
 * valores reductos */
interior vacuum
_probare_anulos (
    CredoContextus* c)
{
    Piscina* piscina  = c->piscina;
        i32  p        = (i32)998244353UL;
        i32  x        = (i32)12345UL;
       Sors  s;
        i32  t;

    sors_seminare(&s, (i64)MMXXIX, ZEPHYRUM);
    per (t = ZEPHYRUM; t < LX; t++)
    {
                 Fortuitum   f;
            ContractioRete*  rete;
            ContractioOrdo*  ordo;
        ContractioOfficina*  officina;
                    Magnus** integri;
                Polynomium** polynomia;
                       i32** residua;
                    Magnus   effectus_z;
                Polynomium   effectus_t;
                       i32   calidum;
                       i32   k;
                       i64   j;

        _fortuitum_parvum(&s, &f);
        rete = _rete_fortuitum(piscina, &f);
        ordo = _ordinare(rete, piscina);
        officina = contractio_officina_creare(ordo,
            CONTRACTIO_SATURATUM,
            piscina);
        CREDO_NECESSE_NON_NIHIL(officina);
        residua   = _valores_parare(&f, piscina);
        integri   = (Magnus**)piscina_allocare(piscina,
            (memoriae_index)f.numerus_tensorum * magnitudo(Magnus*));
        polynomia = (Polynomium**)piscina_allocare(piscina,
            (memoriae_index)f.numerus_tensorum * magnitudo(Polynomium*));

        /* Z: valores in [-50, 50] */
        per (k = ZEPHYRUM; k < f.numerus_tensorum; k++)
        {
            i64 e = _elementa_tensoris(&f, k);

            integri[k] = (Magnus*)piscina_allocare(piscina,
                (memoriae_index)e * magnitudo(Magnus));
            per (j = ZEPHYRUM; j < e; j++)
            {
                s64 v = (s64)sors_intra(&s, CI) - (s64)L;

                integri[k][j] = magnus_ex_s64(v);
                residua[k][j] = (i32)(v < ZEPHYRUM ? (s64)p + v : v);
            }
        }
        CREDO_NECESSE(contractio_exsequi_anulo(rete, ordo,
            (constans vacuum* constans*)integri, &ANULUS_INTEGRORUM,
            piscina, &effectus_z));
        CREDO_NECESSE(contractio_exsequi_modulo(rete, ordo, officina,
            (constans i32* constans*)residua, p, &calidum));
        CREDO_AEQUALIS_I32(magnus_residuum_parvum(effectus_z, p),
            calidum);

        /* Z[t, t^-1]: coefficientes in [-3, 3] ad t^-2 .. t^2 */
        per (k = ZEPHYRUM; k < f.numerus_tensorum; k++)
        {
            i64 e = _elementa_tensoris(&f, k);

            polynomia[k] = (Polynomium*)piscina_allocare(piscina,
                (memoriae_index)e * magnitudo(Polynomium));
            per (j = ZEPHYRUM; j < e; j++)
            {
                Magnus coefficientes[V];
                   i32 i;

                per (i = ZEPHYRUM; i < V; i++)
                {
                    coefficientes[i] = magnus_ex_s64(
                        (s64)sors_intra(&s, VII) - (s64)III);
                }
                CREDO_NECESSE(polynomium_ex_coefficientibus(coefficientes,
                    V, -(s32)II, piscina, &polynomia[k][j]));
                residua[k][j] = _aestimare_modulo(polynomia[k][j], x,
                    p);
            }
        }
        CREDO_NECESSE(contractio_exsequi_anulo(rete, ordo,
            (constans vacuum* constans*)polynomia, &ANULUS_POLYNOMIORUM,
            piscina, &effectus_t));
        CREDO_NECESSE(contractio_exsequi_modulo(rete, ordo, officina,
            (constans i32* constans*)residua, p, &calidum));
        CREDO_AEQUALIS_I32(_aestimare_modulo(effectus_t, x, p),
            calidum);
    }
}

/* omnia valores n - 1 ((n-1)^2 = 2^64 - 2^33 + 1 ad n = 2^32 - 1):
 * portatio in omni fere addito. v(a) w(a), a = 4096: 4096; v(a)
 * M(a,b) N(b,c) u(c), 3 x 2048 x 5: 30720 */
interior vacuum
_probare_modulos_magnos (
    CredoContextus* c)
{
         Piscina* piscina   = c->piscina;
    constans i32  moduli[]  = { (i32)4294967291UL, (i32)0xFFFFFFFFUL };
    constans i32  d1[]      = { (i32)4096UL };
    constans i32  p1[]      = { 0, 0 };
    constans i32  l1[]      = { I, I };
    constans i32  d2[]      = { III, (i32)2048UL, V };
    constans i32  p2[]      = { 0, 0, I, I, II, II };
    constans i32  l2[]      = { I, II, II, I };
    constans i32  e2[]      = { III, (i32)6144UL, (i32)10240UL, V };
             i32  t;

    per (t = ZEPHYRUM; t < II; t++)
    {
                       i32  n = moduli[t];
           constans Anulus* anulus_modulo = anulus_residuorum(n,
               piscina);
            ContractioRete* rete;
            ContractioOrdo* ordo;
        ContractioOfficina* officina;
                       i32* valores[IV];
                       i32  calidum;
                       i32  referentia;
                       i32  k;
                       i32  j;

        rete      = _rete(piscina, d1, I, p1, l1, II);
        ordo      = _ordinare(rete, piscina);
        officina  = contractio_officina_creare(ordo, C, piscina);
        CREDO_NECESSE_NON_NIHIL(officina);
        per (k = ZEPHYRUM; k < II; k++)
        {
            valores[k] = (i32*)piscina_allocare(piscina,
                (memoriae_index)4096UL * magnitudo(i32));
            per (j = ZEPHYRUM; j < (i32)4096UL; j++)
            {
                valores[k][j] = n - I;
            }
        }
        CREDO_NECESSE(contractio_exsequi_modulo(rete, ordo, officina,
            (constans i32* constans*)valores, n, &calidum));
        CREDO_AEQUALIS_I32(calidum, (i32)4096UL);
        CREDO_NECESSE(contractio_exsequi_anulo(rete, ordo,
            (constans vacuum* constans*)valores, anulus_modulo, piscina,
            &referentia));
        CREDO_AEQUALIS_I32(referentia, (i32)4096UL);

        rete = _rete(piscina, d2, III, p2, l2, IV);
        ordo = _ordinare(rete, piscina);
        officina = contractio_officina_creare(ordo,
            CONTRACTIO_SATURATUM,
            piscina);
        CREDO_NECESSE_NON_NIHIL(officina);
        per (k = ZEPHYRUM; k < IV; k++)
        {
            valores[k] = (i32*)piscina_allocare(piscina,
                (memoriae_index)e2[k] * magnitudo(i32));
            per (j = ZEPHYRUM; j < e2[k]; j++)
            {
                valores[k][j] = n - I;
            }
        }
        CREDO_NECESSE(contractio_exsequi_modulo(rete, ordo, officina,
            (constans i32* constans*)valores, n, &calidum));
        CREDO_AEQUALIS_I32(calidum, (i32)30720UL);
        CREDO_NECESSE(contractio_exsequi_anulo(rete, ordo,
            (constans vacuum* constans*)valores, anulus_modulo, piscina,
            &referentia));
        CREDO_AEQUALIS_I32(referentia, (i32)30720UL);
    }
}

/* reductio prope 2^64: n = 3 * 2^30 (2^64 mod n = 2^30), v . w cum
 * w[3] = 1789569713: summa = 2^65 - 715827886, portatio 1, humile
 * intra 2^30 sub 2^64 - humile ante additionem reducendum est
 * (exspectatum 1431655762, Python exacte) */
interior vacuum
_probare_marginem_verbi (
    CredoContextus* c)
{
               Piscina* piscina  = c->piscina;
          constans i32  n        = (i32)3221225472UL;
          constans i32  d[]      = { IV };
          constans i32  p[]      = { 0, 0 };
          constans i32  l[]      = { I, I };
                   i32  v[IV];
                   i32  w[IV];
                   i32* valores[II];
                   i32  calidum;
                   i32  referentia;
                   i32  k;
        ContractioRete* rete = _rete(piscina, d, I, p, l, II);
        ContractioOrdo* ordo = _ordinare(rete, piscina);
    ContractioOfficina* officina = contractio_officina_creare(ordo, C,
        piscina);

    CREDO_NECESSE_NON_NIHIL(officina);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        v[k] = n - I;
        w[k] = n - I;
    }
    w[III]      = (i32)1789569713UL;
    valores[0]  = v;
    valores[I]  = w;
    CREDO_NECESSE(contractio_exsequi_modulo(rete, ordo, officina,
        (constans i32* constans*)valores, n, &calidum));
    CREDO_AEQUALIS_I32(calidum, (i32)1431655762UL);
    CREDO_NECESSE(contractio_exsequi_anulo(rete, ordo,
        (constans vacuum* constans*)valores, anulus_residuorum(n,
        piscina),
        piscina, &referentia));
    CREDO_AEQUALIS_I32(referentia, (i32)1431655762UL);
}

/* officina reutilis: C exsecutiones, piscina vocantis non crescit;
 * refutationes (ordo alieni retis, officina alieni ordinis, modulus,
 * valores, exitus) exitum non tangunt */
interior vacuum
_probare_iterationem (
    CredoContextus* c)
{
               Piscina*  piscina = c->piscina;
             Fortuitum   f;
                  Sors   s;
        ContractioRete*  rete;
        ContractioRete*  rete_alterum;
        ContractioOrdo*  ordo;
        ContractioOrdo*  ordo_alter;
    ContractioOfficina*  officina;
    ContractioOfficina*  officina_altera;
                   i32** valores;
                   i32*  servatum;
                   i32   calidum  = ZEPHYRUM;
                   i32   exitus   = (i32)0xDEADUL;
        memoriae_index   usus;
                   i32   t;

    /* catena v0(a) M1(a,b) M2(b,c) v3(c), dimensiones 2, 3, 4 */
    f.numerus_indicum    = III;
    f.numerus_tensorum   = IV;
    f.dimensiones[0]     = II;
    f.dimensiones[I]     = III;
    f.dimensiones[II]    = IV;
    f.plani[0]           = 0;
    f.plani[I]           = 0;
    f.plani[II]          = I;
    f.plani[III]         = I;
    f.plani[IV]          = II;
    f.plani[V]           = II;
    f.longitudines[0]    = I;
    f.longitudines[I]    = II;
    f.longitudines[II]   = II;
    f.longitudines[III]  = I;

    rete      = _rete_fortuitum(piscina, &f);
    ordo      = _ordinare(rete, piscina);
    officina  = contractio_officina_creare(ordo, XXVIII, piscina);
    CREDO_NECESSE_NON_NIHIL(officina);
    valores = _valores_parare(&f, piscina);
    sors_seminare(&s, (i64)MMXXX, ZEPHYRUM);

    usus = piscina_summa_usus(piscina);
    per (t = ZEPHYRUM; t < C; t++)
    {
        _valores_implere(&s, &f, valores, XVII);
        CREDO_NECESSE(contractio_exsequi_modulo(rete, ordo, officina,
            (constans i32* constans*)valores, XVII, &calidum));
        CREDO_AEQUALIS_I32(calidum, _vis_bruta(&f, valores, XVII));
    }
    CREDO_AEQUALIS_I64((i64)piscina_summa_usus(piscina), (i64)usus);

    rete_alterum  = _rete_fortuitum(piscina, &f);
    ordo_alter    = _ordinare(rete_alterum, piscina);
    officina_altera = contractio_officina_creare(ordo_alter, XXVIII,
        piscina);
    CREDO_NECESSE_NON_NIHIL(officina_altera);
    CREDO_FALSUM(contractio_exsequi_modulo(rete_alterum, ordo, officina,
        (constans i32* constans*)valores, XVII, &exitus));
    CREDO_FALSUM(contractio_exsequi_modulo(rete, ordo, officina_altera,
        (constans i32* constans*)valores, XVII, &exitus));
    CREDO_FALSUM(contractio_exsequi_modulo(rete, ordo, officina,
        (constans i32* constans*)valores, I, &exitus));
    CREDO_FALSUM(contractio_exsequi_modulo(rete, ordo, officina,
        (constans i32* constans*)valores, ZEPHYRUM, &exitus));
    CREDO_FALSUM(contractio_exsequi_modulo(rete, ordo, officina, NIHIL,
        XVII, &exitus));
    CREDO_FALSUM(contractio_exsequi_modulo(rete, ordo, officina,
        (constans i32* constans*)valores, XVII, NIHIL));
    CREDO_FALSUM(contractio_exsequi_modulo(rete, ordo, NIHIL,
        (constans i32* constans*)valores, XVII, &exitus));
    servatum     = valores[II];
    valores[II]  = NIHIL;
    CREDO_FALSUM(contractio_exsequi_modulo(rete, ordo, officina,
        (constans i32* constans*)valores, XVII, &exitus));
    CREDO_FALSUM(contractio_exsequi_anulo(rete, ordo,
        (constans vacuum* constans*)valores,
        anulus_residuorum(XVII, piscina), piscina, &exitus));
    valores[II] = servatum;
    CREDO_AEQUALIS_I32(exitus, (i32)0xDEADUL);

    CREDO_FALSUM(contractio_exsequi_anulo(rete_alterum, ordo,
        (constans vacuum* constans*)valores,
        anulus_residuorum(XVII, piscina), piscina, &exitus));
    CREDO_FALSUM(contractio_exsequi_anulo(rete, ordo,
        (constans vacuum* constans*)valores, NIHIL, piscina, &exitus));
    CREDO_FALSUM(contractio_exsequi_anulo(rete, ordo,
        (constans vacuum* constans*)valores,
        anulus_residuorum(XVII, piscina), NIHIL, &exitus));
    CREDO_FALSUM(contractio_exsequi_anulo(rete, ordo, NIHIL,
        anulus_residuorum(XVII, piscina), piscina, &exitus));
    CREDO_AEQUALIS_I32(exitus, (i32)0xDEADUL);
}

/* C indices dimensionis 1 communes et unus dimensionis 3: odometrum
 * solum super indicem dimensionis 3 (1*4 + 2*5 + 3*6 = 32) */
interior vacuum
_probare_dimensiones_unas (
    CredoContextus* c)
{
               Piscina* piscina  = c->piscina;
        ContractioRete* rete     = contractio_rete_creare(piscina);
        ContractioOrdo* ordo;
    ContractioOfficina* officina;
                   i32  sinister[CI];
                   i32  dexter[CI];
                   i32  v0[] = { I, II, III };
                   i32  v1[] = { IV, V, VI };
                   i32* valores[II];
                   i32  calidum;
                   i32  referentia;
                   i32  id;
                   i32  j;

    per (j = ZEPHYRUM; j < CI; j++)
    {
        CREDO_NECESSE(contractio_index_addere(rete, j == L ? III : I,
            &id));
        sinister[j]    = j;
        dexter[C - j]  = j;
    }
    CREDO_NECESSE(contractio_tensor_addere(rete, sinister, CI, &id));
    CREDO_NECESSE(contractio_tensor_addere(rete, dexter, CI, &id));
    ordo      = _ordinare(rete, piscina);
    officina  = contractio_officina_creare(ordo, C, piscina);
    CREDO_NECESSE_NON_NIHIL(officina);
    valores[0] = v0;
    valores[I] = v1;
    CREDO_NECESSE(contractio_exsequi_modulo(rete, ordo, officina,
        (constans i32* constans*)valores, XCVII, &calidum));
    CREDO_AEQUALIS_I32(calidum, XXXII);
    CREDO_NECESSE(contractio_exsequi_anulo(rete, ordo,
        (constans vacuum* constans*)valores,
        anulus_residuorum(XCVII, piscina), piscina, &referentia));
    CREDO_AEQUALIS_I32(referentia, XXXII);
}

/* rete vacuum: 1; tensor solus: valor eius */
interior vacuum
_probare_retia_minima (
    CredoContextus* c)
{
               Piscina* piscina  = c->piscina;
          constans i32  nulli[]  = { 0 };
          constans i32  l1[]     = { 0 };
        ContractioRete* rete;
        ContractioOrdo* ordo;
    ContractioOfficina* officina;
                   i32  septem   = VII;
                   i32* valores[I];
                Magnus  septem_z = magnus_ex_s64((s64)VII);
                Magnus* valores_z[I];
                Magnus  effectus_z;
                   i32  calidum;

    rete      = contractio_rete_creare(piscina);
    ordo      = _ordinare(rete, piscina);
    officina  = contractio_officina_creare(ordo, 0, piscina);
    CREDO_NECESSE(contractio_exsequi_modulo(rete, ordo, officina, NIHIL,
        XVII, &calidum));
    CREDO_AEQUALIS_I32(calidum, I);
    CREDO_NECESSE(contractio_exsequi_anulo(rete, ordo, NIHIL,
        &ANULUS_INTEGRORUM, piscina, &effectus_z));
    CREDO_VERUM(magnus_aequalis(effectus_z, magnus_ex_s64((s64)I)));

    rete        = _rete(piscina, nulli, 0, nulli, l1, I);
    ordo        = _ordinare(rete, piscina);
    officina    = contractio_officina_creare(ordo, 0, piscina);
    valores[0]  = &septem;
    CREDO_NECESSE(contractio_exsequi_modulo(rete, ordo, officina,
        (constans i32* constans*)valores, XVII, &calidum));
    CREDO_AEQUALIS_I32(calidum, VII);
    CREDO_NECESSE(contractio_exsequi_modulo(rete, ordo, officina,
        (constans i32* constans*)valores, V, &calidum));
    CREDO_AEQUALIS_I32(calidum, II);
    valores_z[0] = &septem_z;
    CREDO_NECESSE(contractio_exsequi_anulo(rete, ordo,
        (constans vacuum* constans*)valores_z, &ANULUS_INTEGRORUM,
        piscina, &effectus_z));
    CREDO_VERUM(magnus_aequalis(effectus_z, septem_z));
}

/* gradus referentiae sine officina: sumptus saturati refutantur ante
 * omnem lectionem valorum (tabulae hic unius elementi tantum) */
interior vacuum
_probare_anulum_saturatum (
    CredoContextus* c)
{
           Piscina* piscina  = c->piscina;
      constans i32  t        = (i32)3486784401UL;
      constans i32  tricies  = (i32)0x40000000UL;
      constans i32  tres[]   = { t, t, t, t };
      constans i32  ps[]     = { 0, I, II, III, 0, I, II, III };
      constans i32  ls[]     = { IV, I, I, I, I };
      constans i32  dn[]     = { tricies, tricies, tricies };
      constans i32  pn[]     = { 0, I, II, 0, I, II };
      constans i32  ln[]     = { III, III };
               i32  unum     = I;
               i32* valores[V];
               i32  exitus = (i32)0xDEADUL;
               i32  k;
    ContractioRete* rete;

    per (k = ZEPHYRUM; k < V; k++)
    {
        valores[k] = &unum;
    }
    /* multiplicationes 2^90, apex 1 */
    rete = _rete(piscina, dn, III, pn, ln, II);
    CREDO_FALSUM(contractio_exsequi_anulo(rete, _ordinare(rete,
        piscina),
        (constans vacuum* constans*)valores,
        anulus_residuorum(XVII, piscina), piscina, &exitus));
    /* apex 3^60 */
    rete = _rete(piscina, tres, IV, ps, ls, V);
    CREDO_FALSUM(contractio_exsequi_anulo(rete, _ordinare(rete,
        piscina),
        (constans vacuum* constans*)valores,
        anulus_residuorum(XVII, piscina), piscina, &exitus));
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
    { "scalares multi", _probare_vacuos_multos, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "vis bruta", _probare_vim_brutam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "anuli Z et Z[t]", _probare_anulos, NIHIL, NIHIL, NIHIL, NIHIL },
    { "moduli magni", _probare_modulos_magnos, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "reductio prope 2^64", _probare_marginem_verbi, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "reusus et refutationes", _probare_iterationem, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "dimensiones unae", _probare_dimensiones_unas, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "retia minima", _probare_retia_minima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "anulus saturatus", _probare_anulum_saturatum, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("contractio", SECTIONES);
}
