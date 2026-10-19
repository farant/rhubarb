/* pecunia.c - pecunia ut quantitates decimales exactae (vide
 * include/pecunia.h; spec project-specs/pecunia-spec.md) */
#include "pecunia.h"
#include "pecunia_monetae.h"
#include <string.h>

hic_manens constans character* VITIA_TITULI[] = {
    "bene",
    "forma mala",
    "plus quam IV decimales",
    "superfluxus (extra s64)",
    "moneta ignota",
    "moneta sine unitate minore",
    "monetae diversae",
    "totum non solvibile in unitates",
    "rationes malae (negativae aut summa nulla)"
};

constans character*
pecunia_vitium_titulus (
    PecuniaVitium vitium)
{
    si (   (s32)vitium < ZEPHYRUM
        || (s32)vitium > (s32)PECUNIA_VITIUM_RATIONES)
    {
        redde "vitium ignotum";
    }
    redde VITIA_TITULI[vitium];
}


/* ==================================================
 * Monetae
 * ================================================== */

/* index ordinis in tabula, aut -I (quaestio binaria; codex
 * majusculis, NUL terminatus) */
interior s32
_ordo (
    constans character* codex)
{
    s32 infra = ZEPHYRUM;
    s32 supra = (s32)PECUNIA_MONETAE_NUMERUS - I;

    dum (infra <= supra)
    {
        s32 medium  = infra + (supra - infra) / II;
        s32 c       = strcmp(codex, PECUNIA_MONETAE[medium].codex);

        si (c == ZEPHYRUM)
        {
            redde medium;
        }
        si (c < ZEPHYRUM)
        {
            supra = medium - I;
        }
        alioquin
        {
            infra = medium + I;
        }
    }
    redde -I;
}

PecuniaVitium
pecunia_moneta_ex_codice (
           chorda  codex,
    PecuniaMoneta* exitus)
{
    character c[IV];
          s32 i;

    si (exitus == NIHIL || codex.mensura != III || codex.datum == NIHIL)
    {
        redde PECUNIA_VITIUM_MONETA_IGNOTA;
    }
    per (i = ZEPHYRUM; i < III; i++)
    {
        character l = (character)codex.datum[i];

        si (l >= 'a' && l <= 'z')
        {
            l = (character)(l - 'a' + 'A');
        }
        si (l < 'A' || l > 'Z')
        {
            redde PECUNIA_VITIUM_MONETA_IGNOTA;
        }
        c[i] = l;
    }
    c[III] = '\0';
    si (_ordo(c) < ZEPHYRUM)
    {
        per (i = ZEPHYRUM; i
            < (s32)PECUNIA_MONETAE_SINE_UNITATE_NUMERUS;
             i++)
        {
            si (strcmp(c, PECUNIA_MONETAE_SINE_UNITATE[i]) == ZEPHYRUM)
            {
                redde PECUNIA_VITIUM_MONETA_SINE_UNITATE;
            }
        }
        redde PECUNIA_VITIUM_MONETA_IGNOTA;
    }
    memcpy(exitus->codex, c, IV);
    redde PECUNIA_BENE;
}

constans PecuniaMonetaInformatio*
pecunia_moneta_informatio (
    PecuniaMoneta m)
{
    s32 k;

    si (m.codex[III] != '\0')
    {
        redde NIHIL;
    }
    k = _ordo(m.codex);
    redde k < ZEPHYRUM ? NIHIL : &PECUNIA_MONETAE[k];
}

s32
pecunia_moneta_exponens (
    PecuniaMoneta m)
{
    constans PecuniaMonetaInformatio* i = pecunia_moneta_informatio(m);

    redde i ? i->exponens : -I;
}

b32
pecunia_moneta_aequalis (
    PecuniaMoneta a,
    PecuniaMoneta b)
{
    redde memcmp(a.codex, b.codex, IV) == ZEPHYRUM;
}


/* ==================================================
 * Valor et arithmetica
 * ================================================== */

/* decimillesimae per unitatem minorem: 10^(IV - exponens) */
interior s64
_factor_unitatis (
    s32 exponens)
{
    commutatio (exponens)
    {
        casus ZEPHYRUM: redde (s64)(X * M);
        casus I:        redde (s64)M;
        casus II:       redde (s64)C;
        casus III:      redde (s64)X;
        ordinarius:     redde (s64)I;
    }
}

/* a + b sine circumvolutione; FALSUM si extra s64 */
interior b32
_adde_tutum (
     s64  a,
     s64  b,
    s64* exitus)
{
    si (b > ZEPHYRUM && a > PECUNIA_QUANTITAS_MAXIMA - b)
    {
        redde FALSUM;
    }
    si (b < ZEPHYRUM && a < PECUNIA_QUANTITAS_MINIMA - b)
    {
        redde FALSUM;
    }
    *exitus = a + b;
    redde VERUM;
}

PecuniaVitium
pecunia_ex_unitatibus (
              s64  unitates,
    PecuniaMoneta  m,
         Pecunia* exitus)
{
    s32 exponens = pecunia_moneta_exponens(m);
    s64 factor;
    s64 limes;

    si (exponens < ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_MONETA_IGNOTA;
    }
    factor = _factor_unitatis(exponens);
    /* |unitates * factor| <= MAXIMA; inferne idem limes valet quia
     * factor (> I) 2^63 non dividit:
     * ceil(MINIMA / f) = -(MAXIMA / f) */
    limes = PECUNIA_QUANTITAS_MAXIMA / factor;
    si (factor > I && (unitates > limes || unitates < -limes))
    {
        redde PECUNIA_VITIUM_SUPERFLUXUS;
    }
    exitus->quantitas  = unitates * factor;
    exitus->moneta     = m;
    redde PECUNIA_BENE;
}

Pecunia
pecunia_nihil (
    PecuniaMoneta m)
{
    Pecunia x;

    x.quantitas  = ZEPHYRUM;
    x.moneta     = m;
    redde x;
}

PecuniaVitium
pecunia_adde (
     Pecunia  a,
     Pecunia  b,
     Pecunia* exitus)
{
    s64 r;

    si (!pecunia_moneta_aequalis(a.moneta, b.moneta))
    {
        redde PECUNIA_VITIUM_MONETAE_DIVERSAE;
    }
    si (!_adde_tutum(a.quantitas, b.quantitas, &r))
    {
        redde PECUNIA_VITIUM_SUPERFLUXUS;
    }
    exitus->quantitas  = r;
    exitus->moneta     = a.moneta;
    redde PECUNIA_BENE;
}

PecuniaVitium
pecunia_subtrahe (
     Pecunia  a,
     Pecunia  b,
    Pecunia* exitus)
{
    si (!pecunia_moneta_aequalis(a.moneta, b.moneta))
    {
        redde PECUNIA_VITIUM_MONETAE_DIVERSAE;
    }
    /* a - b: b > 0 infra, b < 0 supra (etiam b = MINIMA) */
    si (   b.quantitas > ZEPHYRUM
        && a.quantitas < PECUNIA_QUANTITAS_MINIMA + b.quantitas)
    {
        redde PECUNIA_VITIUM_SUPERFLUXUS;
    }
    si (   b.quantitas < ZEPHYRUM
        && a.quantitas > PECUNIA_QUANTITAS_MAXIMA + b.quantitas)
    {
        redde PECUNIA_VITIUM_SUPERFLUXUS;
    }
    exitus->quantitas  = a.quantitas - b.quantitas;
    exitus->moneta     = a.moneta;
    redde PECUNIA_BENE;
}

PecuniaVitium
pecunia_nega (
     Pecunia  a,
    Pecunia* exitus)
{
    si (a.quantitas == PECUNIA_QUANTITAS_MINIMA)
    {
        redde PECUNIA_VITIUM_SUPERFLUXUS;
    }
    exitus->quantitas  = -a.quantitas;
    exitus->moneta     = a.moneta;
    redde PECUNIA_BENE;
}

PecuniaVitium
pecunia_summa (
     constans Pecunia* a,
       memoriae_index  n,
              Pecunia* exitus)
{
    memoriae_index i;
               s64 r = ZEPHYRUM;

    si (a == NIHIL || n == ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_FORMA;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (!pecunia_moneta_aequalis(a[i].moneta, a[ZEPHYRUM].moneta))
        {
            redde PECUNIA_VITIUM_MONETAE_DIVERSAE;
        }
        si (!_adde_tutum(r, a[i].quantitas, &r))
        {
            redde PECUNIA_VITIUM_SUPERFLUXUS;
        }
    }
    exitus->quantitas  = r;
    exitus->moneta     = a[ZEPHYRUM].moneta;
    redde PECUNIA_BENE;
}

PecuniaVitium
pecunia_compara (
    Pecunia  a,
    Pecunia  b,
        s32* exitus)
{
    si (!pecunia_moneta_aequalis(a.moneta, b.moneta))
    {
        redde PECUNIA_VITIUM_MONETAE_DIVERSAE;
    }
    *exitus = a.quantitas < b.quantitas ? -I
        : a.quantitas > b.quantitas ? I : ZEPHYRUM;
    redde PECUNIA_BENE;
}

s32
pecunia_signum (
    Pecunia a)
{
    redde a.quantitas < ZEPHYRUM ? -I : a.quantitas > ZEPHYRUM ? I
        : ZEPHYRUM;
}

b32
pecunia_aequalis (
    Pecunia a,
    Pecunia b)
{
    redde a.quantitas == b.quantitas
        && pecunia_moneta_aequalis(a.moneta, b.moneta);
}


/* ==================================================
 * Rotundatio, multiplicatio, unitates, cambium
 * ================================================== */

/* Decisio rotundationis: valor = quotiens (solum) + r/den, r >= 0.
 * VERUM = quotiens + 1. dimidium = signum(2r - den). Communis viae
 * Magni (multiplica) et viae s64 (ad_unitatem). */
interior b32
_sursum (
    PecuniaModus modus,
             b32 residuum_nullum,
             s32 dimidium,
             b32 quotiens_negativus,
             b32 quotiens_impar)
{
    si (residuum_nullum)
    {
        redde FALSUM;
    }
    commutatio (modus)
    {
        casus PECUNIA_SOLUM:
            redde FALSUM;
        casus PECUNIA_TECTUM:
            redde VERUM;
        casus PECUNIA_VERSUS_NIHIL:
            /* negativus: versus nihil = sursum */
            redde quotiens_negativus;
        casus PECUNIA_DIMIDIUM_SURSUM:
            redde dimidium > ZEPHYRUM
                || (dimidium == ZEPHYRUM && !quotiens_negativus);
        ordinarius:   /* PECUNIA_PAR */
            redde dimidium > ZEPHYRUM
                || (dimidium == ZEPHYRUM && quotiens_impar);
    }
}

/* numerator / denominator (> 0) ad integrum per modum, EXACTE:
 * magnus_divide Euclideum (0 <= r < den), ergo quotiens iam solum
 * est. FALSUM si effectus extra s64. */
interior b32
_rotundare (
          Magnus  numerator,
          Magnus  denominator,
    PecuniaModus  modus,
         Piscina* p,
             s64* exitus)
{
    Magnus quotiens;
    Magnus residuum;

    si (!magnus_divide(numerator, denominator, p, &quotiens, &residuum))
    {
        redde FALSUM;
    }
    si (_sursum(modus, magnus_signum(residuum) == ZEPHYRUM,
        magnus_compara(magnus_adde(residuum, residuum, p), denominator),
        magnus_signum(quotiens) < ZEPHYRUM,
        magnus_residuum_parvum(quotiens, II) == I))
    {
        quotiens = magnus_adde(quotiens, magnus_ex_s64(I), p);
    }
    redde magnus_ad_s64(quotiens, exitus);
}

PecuniaVitium
pecunia_multiplica (
         Pecunia  a,
         Fractio  f,
    PecuniaModus  modus,
         Piscina* piscina,
         Pecunia* exitus)
{
    s64 r;

    si (!_rotundare(magnus_multiplica(magnus_ex_s64(a.quantitas),
        fractio_numerator(f), piscina), fractio_denominator(f), modus,
        piscina, &r))
    {
        redde PECUNIA_VITIUM_SUPERFLUXUS;
    }
    exitus->quantitas  = r;
    exitus->moneta     = a.moneta;
    redde PECUNIA_BENE;
}

PecuniaVitium
pecunia_ad_unitatem (
         Pecunia  a,
    PecuniaModus  modus,
        Pecunia* exitus)
{
    s32 exponens = pecunia_moneta_exponens(a.moneta);
    s64 factor;
    s64 quotiens;
    s64 residuum;
    s64 limes;

    si (exponens < ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_MONETA_IGNOTA;
    }
    factor = _factor_unitatis(exponens);
    /* via s64 (sine piscina): divisio Euclidea manu - '/' et '%'
     * negativorum C89 incerta, sed a == q*f + r semper; r < 0
     * corrigitur. |2r| < 2f <= 20000: nullus superfluxus. */
    quotiens = a.quantitas / factor;
    residuum = a.quantitas % factor;
    si (residuum < ZEPHYRUM)
    {
        quotiens -= I;
        residuum += factor;
    }
    si (_sursum(modus, residuum == ZEPHYRUM,
        II * residuum < factor ? -I : II * residuum > factor ? I
            : ZEPHYRUM,
        quotiens < ZEPHYRUM, (quotiens % II) != ZEPHYRUM))
    {
        quotiens += I;    /* quotiens <= MAXIMA / f: capit */
    }
    /* rotundatio sursum prope margines: quotiens * factor extra s64 */
    limes = PECUNIA_QUANTITAS_MAXIMA / factor;
    si (factor > I && (quotiens > limes || quotiens < -limes))
    {
        redde PECUNIA_VITIUM_SUPERFLUXUS;
    }
    exitus->quantitas  = quotiens * factor;
    exitus->moneta     = a.moneta;
    redde PECUNIA_BENE;
}

b32
pecunia_solvibilis (
    Pecunia a)
{
    s32 exponens = pecunia_moneta_exponens(a.moneta);

    si (exponens < ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* % negativi C89 signo incerto, sed nihil manet nihil */
    redde a.quantitas % _factor_unitatis(exponens) == ZEPHYRUM;
}

PecuniaVitium
pecunia_convertere (
          Pecunia  a,
          Fractio  cambium,
    PecuniaMoneta  ad,
     PecuniaModus  modus,
          Piscina* piscina,
          Pecunia* exitus)
{
    PecuniaVitium v;
          Pecunia r;

    si (fractio_signum(cambium) <= ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_FORMA;
    }
    si (pecunia_moneta_exponens(ad) < ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_MONETA_IGNOTA;
    }
    v = pecunia_multiplica(a, cambium, modus, piscina, &r);
    si (v != PECUNIA_BENE)
    {
        redde v;
    }
    r.moneta  = ad;
    *exitus   = r;
    redde PECUNIA_BENE;
}
