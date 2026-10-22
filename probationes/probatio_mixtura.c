/* probatio_mixtura.c - Probationes mixturae (include/mixtura.h)
 *
 * Tabula fixa (DC = 1.0) ut lucra legantur directe: positio media
 * potentiae constantis = cos(pi/4) = 0.70710677 utrimque. Quadra
 * numerata: vox ad quadrum n incipit = exemplum primum non nullum ad
 * indicem n. Vide lib/mixtura.worklog.md.
 */
#include "latina.h"
#include "credo.h"
#include "sonitus.h"
#include "mixtura.h"

#define MEDIUM 0.70710677f
/* XVI milia quadra = 0.33 s: radius (0.23 s) et fragor (0.30 s) toti */
#define QUADRA_MAXIMA (XVI * M)

hic_manens f32 exitus_a[II * QUADRA_MAXIMA];
hic_manens f32 exitus_b[II * QUADRA_MAXIMA];
hic_manens f32 tabula_unius[C];    /* C exempla 1.0 */
hic_manens f32 sonitus_tabula[QUADRA_MAXIMA];

interior vacuum
_parare (
    CredoContextus* c)
{
    i32 k;

    (vacuum)c;
    per (k = ZEPHYRUM; k < II * QUADRA_MAXIMA; k++)
    {
        exitus_a[k] = 0.0f;
        exitus_b[k] = 0.0f;
    }
    per (k = ZEPHYRUM; k < C; k++)
    {
        tabula_unius[k] = 1.0f;
    }
    per (k = ZEPHYRUM; k < QUADRA_MAXIMA; k++)
    {
        sonitus_tabula[k] = 0.0f;
    }
}

interior Mixtura*
_mixtura (
    CredoContextus* c,
               i32  voces,
               i32  iussa,
               i32  nuntii)
{
    MixturaForma forma;

    forma.voces   = voces;
    forma.iussa   = iussa;
    forma.nuntii  = nuntii;
    redde mixtura_creare(c->piscina, &forma);
}

interior MixturaAnsa
_ansa (
    i32 sedes,
    i32 generatio)
{
    MixturaAnsa a;

    a.sedes      = sedes;
    a.generatio  = generatio;
    redde a;
}

/* vox tabulae unius (C exempla 1.0) ad momentum, positio media */
interior b32
_tabulam_incipere (
        Mixtura* m,
    MixturaAnsa  ansa,
            s64  momentum,
            b32  iterans)
{
    MixturaIussum i;

    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_TABULAM, momentum);
    i.ansa       = ansa;
    i.exempla    = tabula_unius;
    i.longitudo  = C;
    i.iterans    = iterans;
    redde mixtura_iubere(m, &i);
}

interior MixturaCensus
_census (
    Mixtura* m)
{
     MixturaIussum i;
    MixturaNuntius n;
     MixturaCensus nullus;
               f32 tabula[II];

    nullus.voces_activae  = ZEPHYRUM;
    nullus.iussa_tarda    = ZEPHYRUM;
    nullus.iussa_irrita   = ZEPHYRUM;
    nullus.iussa_amissa   = ZEPHYRUM;
    nullus.nuntii_amissi  = ZEPHYRUM;
    nullus.apex           = -1.0f;
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_CENSUM_PETERE,
                          mixtura_momentum(m));
    (vacuum)mixtura_iubere(m, &i);
    /* agmen unius quadri: iussum agit */
    mixtura_reddere(m, tabula, I);
    dum (mixtura_nuntium_legere(m, &n))
    {
        si (n.genus == MIXTURA_NUNTIUS_CENSUS)
        {
            redde n.census;
        }
    }
    redde nullus;
}


    /* ==================================================
     * CREATIO: forma invalida, silentium, horologium
     * ================================================== */

interior vacuum
_sectio_creatio (
    CredoContextus* c)
{
     Mixtura* m;
         i32  k;
         b32  tacita = VERUM;

    CREDO_VERUM (_mixtura(c, ZEPHYRUM, VIII, VIII) == NIHIL);
    CREDO_VERUM (_mixtura(c, IV, ZEPHYRUM, VIII) == NIHIL);
    CREDO_VERUM (_mixtura(c, IV, VIII, ZEPHYRUM) == NIHIL);
    m = _mixtura(c, IV, VIII, VIII);
    CREDO_VERUM (m != NIHIL);
    si (m == NIHIL)
    {
        redde;
    }
    CREDO_VERUM (mixtura_momentum(m) == ZEPHYRUM);
    exitus_a[ZEPHYRUM] = 9.0f;
    mixtura_reddere(m, exitus_a, LXIV);
    per (k = ZEPHYRUM; k < II * LXIV; k++)
    {
        si (exitus_a[k] != 0.0f)
        {
            tacita = FALSUM;
        }
    }
    CREDO_VERUM (tacita);
    CREDO_VERUM (mixtura_momentum(m) == LXIV);
    CREDO_VERUM (mixtura_ansa_nulla(_ansa(ZEPHYRUM, ZEPHYRUM)));
    CREDO_VERUM (!mixtura_ansa_nulla(_ansa(ZEPHYRUM, I)));
}


    /* ==================================================
     * INITIUM EXACTUM: quadrum incipiendi, finis, nuntius
     * ================================================== */

interior vacuum
_sectio_initium (
    CredoContextus* c)
{
           Mixtura* m = _mixtura(c, IV, VIII, VIII);
    MixturaNuntius  n;
               i32  k;
               b32  bene = VERUM;

    si (m == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    /* ad quadrum X, C exempla: [X, CX) sonant */
    CREDO_VERUM (_tabulam_incipere(m, _ansa(ZEPHYRUM, I), X, FALSUM));
    mixtura_reddere(m, exitus_a, CXXVIII);
    per (k = ZEPHYRUM; k < CXXVIII; k++)
    {
        f32 exspectatum = (k >= X && k < CX) ? MEDIUM : 0.0f;

        si (   exitus_a[II * k]     != exspectatum
            || exitus_a[II * k + I] != exspectatum)
        {
            bene = FALSUM;
        }
    }
    CREDO_VERUM (bene);
    CREDO_VERUM (mixtura_nuntium_legere(m, &n));
    CREDO_AEQUALIS_I32 ((i32)n.genus, (i32)MIXTURA_NUNTIUS_VOX_FINITA);
    CREDO_AEQUALIS_I32 ((i32)n.causa, (i32)MIXTURA_FINIS_NATURALIS);
    CREDO_VERUM (n.momentum == CX);
    CREDO_AEQUALIS_I32 (n.ansa.generatio, I);
    CREDO_VERUM (n.exempla == tabula_unius);
    CREDO_VERUM (!mixtura_nuntium_legere(m, &n));

    /* momentum in agmine sequenti: CC = quadrum LXXII agminis II */
    CREDO_VERUM (_tabulam_incipere(m, _ansa(I, I), CC, FALSUM));
    mixtura_reddere(m, exitus_a, CXXVIII);
    CREDO_F32_PROXIMUS (exitus_a[II * LXXI], 0.0f, 0.0f);
    CREDO_F32_PROXIMUS (exitus_a[II * LXXII], MEDIUM, 0.0f);

    /* quadra CONTIGUA: sinistra ad CCC, dextra ad CCCI - segmentum
     * incipit ad momentum iussi, ergo iussum quadro uno praecox solum
     * hic videtur (planta 'iussa quadro uno praecocia', 2026-10-20) */
    {
        MixturaIussum i;

        mixtura_iussum_parare(&i, MIXTURA_IUSSUM_TABULAM, CCC);
        i.ansa       = _ansa(II, I);
        i.exempla    = tabula_unius;
        i.longitudo  = C;
        i.positio    = -1.0f;
        (vacuum)mixtura_iubere(m, &i);
        i.momentum  = CCCI;
        i.ansa      = _ansa(III, I);
        i.positio   = 1.0f;
        (vacuum)mixtura_iubere(m, &i);
        mixtura_reddere(m, exitus_a, CXXVIII);   /* CCLVI .. CCCLXXXIII */
        CREDO_F32_PROXIMUS (exitus_a[II * XLIV], 1.0f, 0.000001f);
        CREDO_F32_PROXIMUS (exitus_a[II * XLIV + I], 0.0f, 0.000001f);
        CREDO_F32_PROXIMUS (exitus_a[II * XLV + I], 1.0f, 0.000001f);
    }
}


    /* ==================================================
     * TARDA et IRRITA: momentum praeteritum, ansa stala
     * ================================================== */

interior vacuum
_sectio_tarda (
    CredoContextus* c)
{
          Mixtura* m = _mixtura(c, IV, VIII, VIII);
    MixturaIussum  i;
    MixturaCensus  census;

    si (m == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    mixtura_reddere(m, exitus_a, LXIV);
    /* momentum X iam praeteritum: initio agminis (quadro LXIV) agit */
    CREDO_VERUM (_tabulam_incipere(m, _ansa(ZEPHYRUM, I), X, FALSUM));
    mixtura_reddere(m, exitus_a, LXIV);
    CREDO_F32_PROXIMUS (exitus_a[ZEPHYRUM], MEDIUM, 0.0f);

    /* ansa stala: generatio aliena ignoratur */
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SISTERE,
        mixtura_momentum(m));
    i.ansa = _ansa(ZEPHYRUM, II);
    (vacuum)mixtura_iubere(m, &i);
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_VOCEM_PONERE,
                          mixtura_momentum(m));
    i.ansa    = _ansa(ZEPHYRUM, VII);
    i.lucrum  = 0.0f;
    (vacuum)mixtura_iubere(m, &i);
    /* sedes extra fines */
    CREDO_VERUM (_tabulam_incipere(m, _ansa(IX, I), mixtura_momentum(m),
                                   FALSUM));
    mixtura_reddere(m, exitus_a, VIII);
    /* vox prior intacta sonat */
    CREDO_F32_PROXIMUS (exitus_a[ZEPHYRUM], MEDIUM, 0.0f);

    census = _census(m);
    CREDO_AEQUALIS_I32 (census.iussa_tarda, I);
    CREDO_AEQUALIS_I32 (census.iussa_irrita, III);
    CREDO_AEQUALIS_I32 (census.voces_activae, I);
}


    /* ==================================================
     * POSITIO: potentia constans L^2 + R^2 = 1
     * ================================================== */

interior vacuum
_sectio_positio (
    CredoContextus* c)
{
    interior constans f32 positiones[V] = {
        -1.0f, -0.4f, 0.0f, 0.3f, 1.0f };
          Mixtura* m = _mixtura(c, VIII, XVI, XVI);
    MixturaIussum  i;
              i32  k;
              b32  potentia = VERUM;

    si (m == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    per (k = ZEPHYRUM; k < V; k++)
    {
        f32 l;
        f32 r;

        mixtura_iussum_parare(&i, MIXTURA_IUSSUM_TABULAM,
                              mixtura_momentum(m));
        i.ansa       = _ansa(k, I);
        i.exempla    = tabula_unius;
        i.longitudo  = C;
        i.positio    = positiones[k];
        (vacuum)mixtura_iubere(m, &i);
        mixtura_reddere(m, exitus_a, I);
        l = exitus_a[ZEPHYRUM];
        r = exitus_a[I];
        si (l * l + r * r < 0.999999f || l * l + r * r > 1.000001f)
        {
            potentia = FALSUM;
        }
        si (k == ZEPHYRUM)
        {
            CREDO_F32_PROXIMUS (l, 1.0f, 0.000001f);
            CREDO_F32_PROXIMUS (r, 0.0f, 0.000001f);
        }
        si (k == IV)
        {
            CREDO_F32_PROXIMUS (l, 0.0f, 0.000001f);
            CREDO_F32_PROXIMUS (r, 1.0f, 0.000001f);
        }
        /* vox sistitur statim, ne summa sequentis misceatur */
        mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SISTERE,
                              mixtura_momentum(m));
        i.ansa = _ansa(k, I);
        (vacuum)mixtura_iubere(m, &i);
        mixtura_reddere(m, exitus_a, I);
    }
    CREDO_VERUM (potentia);
}


    /* ==================================================
     * CLIVUS: lucrum sine saltu, evanescentia, sistere
     * ================================================== */

interior vacuum
_sectio_clivus (
          CredoContextus* c)
{
           Mixtura* m = _mixtura(c, IV, VIII, VIII);
     MixturaIussum  i;
    MixturaNuntius  n;
               i32  k;
               b32  lenis    = VERUM;
               b32  inventa  = FALSUM;

    si (m == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    CREDO_VERUM (_tabulam_incipere(m, _ansa(ZEPHYRUM, I), ZEPHYRUM,
        VERUM));
    /* lucrum 1 -> 0 per C quadra a quadro X */
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_VOCEM_PONERE, X);
    i.ansa    = _ansa(ZEPHYRUM, I);
    i.lucrum  = 0.0f;
    i.clivus  = C;
    (vacuum)mixtura_iubere(m, &i);
    mixtura_reddere(m, exitus_a, CC);
    per (k = I; k < CC; k++)
    {
        f32 d = exitus_a[II * k] - exitus_a[II * (k - I)];

        si (d < -(MEDIUM / 100.0f) - 0.000001f || d > 0.000001f)
        {
            lenis = FALSUM;
        }
    }
    CREDO_VERUM (lenis);
    CREDO_F32_PROXIMUS (exitus_a[II * IX], MEDIUM, 0.0f);
    CREDO_F32_PROXIMUS (exitus_a[II * CX], 0.0f, 0.0f);

    /* lucrum 0 -> 1 per L: ascendit, deinde plenum */
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_VOCEM_PONERE,
                          mixtura_momentum(m));
    i.ansa    = _ansa(ZEPHYRUM, I);
    i.lucrum  = 1.0f;
    i.clivus  = L;
    (vacuum)mixtura_iubere(m, &i);
    mixtura_reddere(m, exitus_a, C);
    CREDO_VERUM (exitus_a[ZEPHYRUM] > 0.0f
        && exitus_a[ZEPHYRUM] < 0.1f);
    CREDO_F32_PROXIMUS (exitus_a[II * LXX], MEDIUM, 0.000001f);

    /* sistere per XLVIII quadra: evanescit, deinde FINIS_IUSSU */
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SISTERE,
        mixtura_momentum(m));
    i.ansa    = _ansa(ZEPHYRUM, I);
    i.clivus  = XLVIII;
    (vacuum)mixtura_iubere(m, &i);
    mixtura_reddere(m, exitus_a, C);
    CREDO_VERUM (exitus_a[ZEPHYRUM] > 0.6f);
    CREDO_F32_PROXIMUS (exitus_a[II * L], 0.0f, 0.0f);
    dum (mixtura_nuntium_legere(m, &n))
    {
        si (   n.genus == MIXTURA_NUNTIUS_VOX_FINITA
            && n.causa == MIXTURA_FINIS_IUSSU)
        {
            inventa = VERUM;
        }
    }
    CREDO_VERUM (inventa);
    CREDO_AEQUALIS_I32 (_census(m).voces_activae, ZEPHYRUM);
}


    /* ==================================================
     * GREGES: mutus tempus servat, lucrum summae, separatio
     * ================================================== */

interior vacuum
_sectio_greges (
    CredoContextus* c)
{
           Mixtura* m = _mixtura(c, IV, XVI, XVI);
     MixturaIussum  i;
    MixturaNuntius  n;
               b32  finita = FALSUM;

    si (m == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    /* effectus mutus ab initio, apertus ad L: tabula C ad 0 incepta
     * sonat [L, C) et finitur ad C (tempus servatum) */
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_GREGEM_PONERE, ZEPHYRUM);
    i.grex   = MIXTURA_GREX_EFFECTUS;
    i.mutus  = VERUM;
    (vacuum)mixtura_iubere(m, &i);
    CREDO_VERUM (_tabulam_incipere(m, _ansa(ZEPHYRUM, I), ZEPHYRUM,
        FALSUM));
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_GREGEM_PONERE, L);
    i.grex   = MIXTURA_GREX_EFFECTUS;
    i.mutus  = FALSUM;
    (vacuum)mixtura_iubere(m, &i);
    mixtura_reddere(m, exitus_a, CL);
    CREDO_F32_PROXIMUS (exitus_a[II * XLIX], 0.0f, 0.0f);
    CREDO_F32_PROXIMUS (exitus_a[II * L], MEDIUM, 0.0f);
    CREDO_F32_PROXIMUS (exitus_a[II * XCIX], MEDIUM, 0.0f);
    CREDO_F32_PROXIMUS (exitus_a[II * C], 0.0f, 0.0f);
    dum (mixtura_nuntium_legere(m, &n))
    {
        si (n.genus == MIXTURA_NUNTIUS_VOX_FINITA && n.momentum == C)
        {
            finita = VERUM;
        }
    }
    CREDO_VERUM (finita);

    /* summa lucro 0.5; effectus mutus non tangit interfaciem */
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_GREGEM_PONERE,
                          mixtura_momentum(m));
    i.grex    = MIXTURA_GREX_SUMMA;
    i.lucrum  = 0.5f;
    (vacuum)mixtura_iubere(m, &i);
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_GREGEM_PONERE,
                          mixtura_momentum(m));
    i.grex   = MIXTURA_GREX_EFFECTUS;
    i.mutus  = VERUM;
    (vacuum)mixtura_iubere(m, &i);
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_TABULAM,
        mixtura_momentum(m));
    i.ansa       = _ansa(I, I);
    i.grex       = MIXTURA_GREX_INTERFACIES;
    i.exempla    = tabula_unius;
    i.longitudo  = C;
    (vacuum)mixtura_iubere(m, &i);
    CREDO_VERUM (_tabulam_incipere(m, _ansa(II, I), mixtura_momentum(m),
                                   FALSUM));
    mixtura_reddere(m, exitus_a, X);
    CREDO_F32_PROXIMUS (exitus_a[ZEPHYRUM], 0.5f * MEDIUM, 0.000001f);

    /* summa muta: silentium magistrum */
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_GREGEM_PONERE,
                          mixtura_momentum(m));
    i.grex    = MIXTURA_GREX_SUMMA;
    i.lucrum  = 0.5f;
    i.mutus   = VERUM;
    (vacuum)mixtura_iubere(m, &i);
    mixtura_reddere(m, exitus_a, X);
    CREDO_F32_PROXIMUS (exitus_a[ZEPHYRUM], 0.0f, 0.0f);

    /* iussa eiusdem momenti ORDINE ADVENTUS: posterius vincit */
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_GREGEM_PONERE,
                          mixtura_momentum(m));
    i.grex    = MIXTURA_GREX_SUMMA;
    i.lucrum  = 0.25f;
    (vacuum)mixtura_iubere(m, &i);
    i.lucrum = 1.0f;
    (vacuum)mixtura_iubere(m, &i);
    mixtura_reddere(m, exitus_a, X);
    CREDO_F32_PROXIMUS (exitus_a[ZEPHYRUM], MEDIUM, 0.000001f);
}


    /* ==================================================
     * SURREPTIO: rector et nuntius SURREPTA
     * ================================================== */

interior vacuum
_sectio_surreptio (
    CredoContextus* c)
{
           Mixtura* m    = _mixtura(c, I, VIII, VIII);
     MixturaRector* r    = mixtura_rectorem_creare(c->piscina, I);
     MixturaRector* duo  = mixtura_rectorem_creare(c->piscina, II);
       MixturaAnsa  a;
       MixturaAnsa  b;
       MixturaAnsa  d;
    MixturaNuntius  n;
               b32  surrepta = FALSUM;

    si (m == NIHIL || r == NIHIL || duo == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    a = mixtura_rector_capere(r, I);
    CREDO_VERUM (!mixtura_ansa_nulla(a));
    CREDO_VERUM (_tabulam_incipere(m, a, ZEPHYRUM, VERUM));
    mixtura_reddere(m, exitus_a, X);
    /* prioritas inferior non surripit */
    CREDO_VERUM (mixtura_ansa_nulla(mixtura_rector_capere(r,
        ZEPHYRUM)));
    /* aequalis surripit: sedes eadem, generatio nova */
    b = mixtura_rector_capere(r, I);
    CREDO_VERUM (!mixtura_ansa_nulla(b));
    CREDO_AEQUALIS_I32 (b.sedes, a.sedes);
    CREDO_VERUM (b.generatio != a.generatio);
    CREDO_VERUM (_tabulam_incipere(m, b, mixtura_momentum(m), FALSUM));
    mixtura_reddere(m, exitus_a, X);
    dum (mixtura_nuntium_legere(m, &n))
    {
        si (   n.genus          == MIXTURA_NUNTIUS_VOX_FINITA
            && n.causa          == MIXTURA_FINIS_SURREPTA
            && n.ansa.generatio == a.generatio)
        {
            surrepta = VERUM;
        }
        mixtura_rector_tractare(r, &n);
    }
    CREDO_VERUM (surrepta);
    /* SURREPTA veteris sedem novam non liberat */
    CREDO_VERUM (mixtura_ansa_nulla(mixtura_rector_capere(r,
        ZEPHYRUM)));
    /* b finitur naturaliter: sedes libera */
    mixtura_reddere(m, exitus_a, CC);
    dum (mixtura_nuntium_legere(m, &n))
    {
        mixtura_rector_tractare(r, &n);
    }
    CREDO_VERUM (!mixtura_ansa_nulla(mixtura_rector_capere(r,
        ZEPHYRUM)));

    /* aequales: vetustissima surripitur */
    a = mixtura_rector_capere(duo, II);
    b = mixtura_rector_capere(duo, II);
    d = mixtura_rector_capere(duo, II);
    CREDO_VERUM (a.sedes != b.sedes);
    CREDO_AEQUALIS_I32 (d.sedes, a.sedes);
}


    /* ==================================================
     * AGMINA PLENA: iussa et nuntii amissi numerantur
     * ================================================== */

interior vacuum
_sectio_agmina (
    CredoContextus* c)
{
          Mixtura* m = _mixtura(c, IV, II, I);
    MixturaIussum  i;
    MixturaCensus  census;

    si (m == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    CREDO_VERUM (_tabulam_incipere(m, _ansa(ZEPHYRUM, I), ZEPHYRUM,
        FALSUM));
    CREDO_VERUM (_tabulam_incipere(m, _ansa(I, I), ZEPHYRUM, FALSUM));
    CREDO_VERUM (!_tabulam_incipere(m, _ansa(II, I), ZEPHYRUM, FALSUM));
    /* duae finiuntur, agmen nuntiorum I: unus amittitur */
    mixtura_reddere(m, exitus_a, CC);
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_CENSUM_PETERE,
                          mixtura_momentum(m));
    (vacuum)mixtura_iubere(m, &i);
    {
        MixturaNuntius n;

        /* nuntius VOX_FINITA unus manet; legitur, deinde census */
        CREDO_VERUM (mixtura_nuntium_legere(m, &n));
        census = _census(m);
    }
    CREDO_AEQUALIS_I32 (census.iussa_amissa, I);
    CREDO_VERUM (census.nuntii_amissi >= I);
}


    /* ==================================================
     * SONITUS: vox viva = sonitus_reddere x positio media
     * ================================================== */

interior vacuum
_sectio_sonitus (
    CredoContextus* c)
{
     constans SonitusParametra* p = sonitus_praeparatum("radius");
                       Mixtura* m = _mixtura(c, IV, VIII, VIII);
                 MixturaIussum  i;
                           i32  n;
                           i32  k;
                           b32  pares = VERUM;

    si (m == NIHIL || p == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    n = sonitus_reddere(p, VII, sonitus_tabula, QUADRA_MAXIMA);
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SONITUM, ZEPHYRUM);
    i.ansa       = _ansa(ZEPHYRUM, I);
    i.parametra  = *p;
    i.semen      = VII;
    (vacuum)mixtura_iubere(m, &i);
    mixtura_reddere(m, exitus_a, QUADRA_MAXIMA);
    per (k = ZEPHYRUM; k < QUADRA_MAXIMA; k++)
    {
        f32 exspectatum = k < n ? sonitus_tabula[k] * MEDIUM : 0.0f;

        si (   exitus_a[II * k]     != exspectatum
            || exitus_a[II * k + I] != exspectatum)
        {
            pares = FALSUM;
        }
    }
    CREDO_VERUM (n < QUADRA_MAXIMA);
    CREDO_VERUM (pares);
}


    /* ==================================================
     * DETERMINISMUS et CENSUS
     * ================================================== */

interior vacuum
_iussa_mittere (
    Mixtura* m)
{
     constans SonitusParametra* p = sonitus_praeparatum("fragor");
                 MixturaIussum  i;

    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SONITUM, V);
    i.ansa       = _ansa(ZEPHYRUM, I);
    i.parametra  = *p;
    i.semen      = III;
    i.positio    = -0.5f;
    (vacuum)mixtura_iubere(m, &i);
    (vacuum)_tabulam_incipere(m, _ansa(I, I), CCC, VERUM);
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_VOCEM_PONERE, D);
    i.ansa     = _ansa(I, I);
    i.lucrum   = 0.25f;
    i.positio  = 0.8f;
    i.clivus   = CC;
    (vacuum)mixtura_iubere(m, &i);
}

interior vacuum
_sectio_determinismus (
    CredoContextus* c)
{
          Mixtura* a = _mixtura(c, IV, VIII, VIII);
          Mixtura* b = _mixtura(c, IV, VIII, VIII);
    MixturaCensus  census;
              i32  k;
              b32  pares  = VERUM;
              f32  apex   = 0.0f;

    si (   a                             == NIHIL || b == NIHIL
        || sonitus_praeparatum("fragor") == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    _iussa_mittere(a);
    _iussa_mittere(b);
    /* agminibus diversis: CCLVI + reliquum contra unum totum */
    mixtura_reddere(a, exitus_a, CCLVI);
    mixtura_reddere(a, exitus_a + II * CCLVI, QUADRA_MAXIMA - CCLVI);
    mixtura_reddere(b, exitus_b, QUADRA_MAXIMA);
    per (k = ZEPHYRUM; k < II * QUADRA_MAXIMA; k++)
    {
        f32 v = exitus_a[k] < 0.0f ? -exitus_a[k] : exitus_a[k];

        si (exitus_a[k] != exitus_b[k])
        {
            pares = FALSUM;
        }
        si (v > apex)
        {
            apex = v;
        }
    }
    CREDO_VERUM (pares);
    census = _census(a);
    CREDO_F32_PROXIMUS (census.apex, apex, 0.0f);
    CREDO_AEQUALIS_I32 (census.voces_activae, I);   /* tabula iterans */
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "creatio", _sectio_creatio, _parare, NIHIL, NIHIL, NIHIL },
    { "initium", _sectio_initium, _parare, NIHIL, NIHIL, NIHIL },
    { "tarda", _sectio_tarda, _parare, NIHIL, NIHIL, NIHIL },
    { "positio", _sectio_positio, _parare, NIHIL, NIHIL, NIHIL },
    { "clivus", _sectio_clivus, _parare, NIHIL, NIHIL, NIHIL },
    { "greges", _sectio_greges, _parare, NIHIL, NIHIL, NIHIL },
    { "surreptio", _sectio_surreptio, _parare, NIHIL, NIHIL, NIHIL },
    { "agmina", _sectio_agmina, _parare, NIHIL, NIHIL, NIHIL },
    { "sonitus", _sectio_sonitus, _parare, NIHIL, NIHIL, NIHIL },
    { "determinismus", _sectio_determinismus, _parare, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mixtura", SECTIONES);
}
