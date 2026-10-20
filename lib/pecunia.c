/* pecunia.c - pecunia ut quantitates decimales exactae (vide
 * include/pecunia.h; spec project-specs/pecunia-spec.md) */
#include "pecunia.h"
#include "pecunia_monetae.h"
#include "chorda_aedificator.h"
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

/* quantitas / factor ad integrum per modum, in s64 (sine piscina):
 * divisio Euclidea manu - '/' et '%' negativorum C89 incerta, sed
 * a == q*f + r semper; r < 0 corrigitur. |2r| < 2f <= 20000. Effectus
 * semper capit (|q| <= |a| / f + 1). */
interior s64
_unitates_rotundatae (
             s64 quantitas,
             s64 factor,
    PecuniaModus modus)
{
    s64 quotiens = quantitas / factor;
    s64 residuum = quantitas % factor;

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
        quotiens += I;
    }
    redde quotiens;
}

PecuniaVitium
pecunia_ad_unitatem (
         Pecunia  a,
    PecuniaModus  modus,
        Pecunia* exitus)
{
    s32 exponens = pecunia_moneta_exponens(a.moneta);
    s64 factor;
    s64 unitates;
    s64 limes;

    si (exponens < ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_MONETA_IGNOTA;
    }
    factor    = _factor_unitatis(exponens);
    unitates  = _unitates_rotundatae(a.quantitas, factor, modus);
    /* rotundatio sursum prope margines: unitates * factor extra s64 */
    limes = PECUNIA_QUANTITAS_MAXIMA / factor;
    si (factor > I && (unitates > limes || unitates < -limes))
    {
        redde PECUNIA_VITIUM_SUPERFLUXUS;
    }
    exitus->quantitas  = unitates * factor;
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


/* ==================================================
 * Partitio
 * ================================================== */

PecuniaVitium
pecunia_partiri (
           Pecunia  totum,
      constans s64* rationes,
    memoriae_index  n,
     PecuniaGranum  granum,
           Piscina* piscina,
           Pecunia* exitus)
{
               s32  exponens = pecunia_moneta_exponens(totum.moneta);
               s64  magnitudo_grani;
            Magnus  grana;
            Magnus  summa_rationum  = magnus_ex_s64(ZEPHYRUM);
            Magnus  summa_solorum   = magnus_ex_s64(ZEPHYRUM);
            Magnus* soli;
            Magnus* residua;
               b32* donati;
               s64* quantitates;
               s64  reliqua;
               s64  k;
               b32  negativum = totum.quantitas < ZEPHYRUM;
    memoriae_index  i;

    si (exponens < ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_MONETA_IGNOTA;
    }
    si (n == ZEPHYRUM || rationes == NIHIL || piscina == NIHIL)
    {
        redde PECUNIA_VITIUM_RATIONES;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (rationes[i] < ZEPHYRUM)
        {
            redde PECUNIA_VITIUM_RATIONES;
        }
        summa_rationum = magnus_adde(summa_rationum,
            magnus_ex_s64(rationes[i]), piscina);
    }
    si (magnus_signum(summa_rationum) == ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_RATIONES;
    }
    magnitudo_grani = granum == PECUNIA_GRANUM_UNITAS
        ? _factor_unitatis(exponens) : (s64)I;
    si (totum.quantitas % magnitudo_grani != ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_NON_SOLVIBILIS;
    }
    /* amplitudo in Magno: |MINIMA| s64 non capit */
    si (!magnus_divide(magnus_absolutum(magnus_ex_s64(totum.quantitas),
        piscina), magnus_ex_s64(magnitudo_grani), piscina, &grana,
        NIHIL))
    {
        redde PECUNIA_VITIUM_RATIONES;
    }
    soli    = (Magnus*)piscina_allocare(piscina,
        n * (memoriae_index)magnitudo(Magnus));
    residua = (Magnus*)piscina_allocare(piscina,
        n * (memoriae_index)magnitudo(Magnus));
    donati  = (b32*)piscina_allocare(piscina,
        n * (memoriae_index)magnitudo(b32));
    si (soli == NIHIL || residua == NIHIL || donati == NIHIL)
    {
        redde PECUNIA_VITIUM_RATIONES;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        (vacuum)magnus_divide(magnus_multiplica(grana,
            magnus_ex_s64(rationes[i]), piscina), summa_rationum,
            piscina, &soli[i], &residua[i]);
        summa_solorum  = magnus_adde(summa_solorum, soli[i], piscina);
        donati[i]      = FALSUM;
    }
    /* reliqua < n: summa partium fractarum, quaeque < I */
    si (!magnus_ad_s64(magnus_subtrahe(grana, summa_solorum, piscina),
        &reliqua))
    {
        redde PECUNIA_VITIUM_RATIONES;
    }
    /* singula grana residuo maximo; paria ad indicem minorem (primus
     * maximus inventus vincit). O(n * reliqua), reliqua < n. */
    per (k = ZEPHYRUM; k < reliqua; k++)
    {
        memoriae_index optimus = n;

        per (i = ZEPHYRUM; i < n; i++)
        {
            si (   !donati[i] && (optimus == n
                || magnus_compara(residua[i], residua[optimus])
                   > ZEPHYRUM))
            {
                optimus = i;
            }
        }
        donati[optimus] = VERUM;
    }
    /* signum in Magno restituitur ANTE conversionem: amplitudo partis
     * unicae ex MINIMA (2^63) s64 non capit, pars signata semper capit
     * (signum totius, |pars| <= |totum|). Prius omnes computantur,
     * deinde exitus scribuntur - intacti si quid deficit. */
    per (i = ZEPHYRUM; i < n; i++)
    {
        Magnus g = donati[i]
            ? magnus_adde(soli[i], magnus_ex_s64(I), piscina) : soli[i];

        g = magnus_multiplica(g, magnus_ex_s64(magnitudo_grani),
            piscina);
        si (negativum)
        {
            g = magnus_nega(g, piscina);
        }
        soli[i] = g;
    }
    quantitates = (s64*)piscina_allocare(piscina,
        n * (memoriae_index)magnitudo(s64));
    si (quantitates == NIHIL)
    {
        redde PECUNIA_VITIUM_RATIONES;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (!magnus_ad_s64(soli[i], &quantitates[i]))
        {
            redde PECUNIA_VITIUM_SUPERFLUXUS;   /* numquam accidit */
        }
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        exitus[i].quantitas  = quantitates[i];
        exitus[i].moneta     = totum.moneta;
    }
    redde PECUNIA_BENE;
}

PecuniaVitium
pecunia_dividere (
           Pecunia  totum,
    memoriae_index  n,
     PecuniaGranum  granum,
           Piscina* piscina,
           Pecunia* exitus)
{
               s64* rationes;
    memoriae_index  i;

    si (n == ZEPHYRUM || piscina == NIHIL)
    {
        redde PECUNIA_VITIUM_RATIONES;
    }
    rationes = (s64*)piscina_allocare(piscina,
        n * (memoriae_index)magnitudo(s64));
    si (rationes == NIHIL)
    {
        redde PECUNIA_VITIUM_RATIONES;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        rationes[i] = I;
    }
    redde pecunia_partiri(totum, rationes, n, granum, piscina, exitus);
}


/* ==================================================
 * Textus
 * ================================================== */

/* amplitudo |MINIMA| = 2^63 in i64 (insignatum) capit */
#define PECUNIA_MAGNITUDO_MINIMAE ((i64)PECUNIA_QUANTITAS_MAXIMA + I)

/* quantitas: signum, pars integra, '.', IV decimales. Amplitudo ut
 * i64 sumitur (MINIMA sine superfluxu); nihil numquam signum fert. */
interior vacuum
_quantitatem_scribere (
    ChordaAedificator* a,
                  s64  quantitas)
{
          i64 amplitudo;
          i64 integra;
          i64 fracta;
    character cifrae[XXIV];
          s32 k = ZEPHYRUM;
          s32 j;

    si (quantitas < ZEPHYRUM)
    {
        amplitudo = (i64)(-(quantitas + I)) + I;
        chorda_aedificator_appendere_character(a, '-');
    }
    alioquin
    {
        amplitudo = (i64)quantitas;
    }
    integra  = amplitudo / (i64)PECUNIA_SCALA;
    fracta   = amplitudo % (i64)PECUNIA_SCALA;
    fac
    {
        cifrae[k++]  = (character)('0' + (s32)(integra % X));
        integra      /= X;
    } dum (integra > ZEPHYRUM);
    per (j = k - I; j >= ZEPHYRUM; j--)
    {
        chorda_aedificator_appendere_character(a, cifrae[j]);
    }
    chorda_aedificator_appendere_character(a, '.');
    chorda_aedificator_appendere_character(a,
        (character)('0' + (s32)(fracta / M)));
    chorda_aedificator_appendere_character(a,
        (character)('0' + (s32)(fracta / C % X)));
    chorda_aedificator_appendere_character(a,
        (character)('0' + (s32)(fracta / X % X)));
    chorda_aedificator_appendere_character(a,
        (character)('0' + (s32)(fracta % X)));
}

chorda
pecunia_ad_textum (
     Pecunia  a,
     Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, XXXII);

    chorda_aedificator_appendere_literis(aed, a.moneta.codex);
    chorda_aedificator_appendere_character(aed, ' ');
    _quantitatem_scribere(aed, a.quantitas);
    redde chorda_aedificator_finire(aed);
}

chorda
pecunia_numerus (
     Pecunia  a,
     Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, XXIV);

    _quantitatem_scribere(aed, a.quantitas);
    redde chorda_aedificator_finire(aed);
}

interior b32
_cifra (
    character c)
{
    redde c >= '0' && c <= '9';
}

/* Quantitas ex textu[initium..mensura). canonica: pars integra sine
 * zephyris ducentibus, IV decimales exacte, "-0.0000" vetitum;
 * aliter forma Lunch Money (zephyra ducentia, 0..IV decimales post
 * '.' qui saltem unam cifram postulat). FORMA prius tota iudicatur,
 * deinde valor (superfluxus contra spatium reliquum). */
interior PecuniaVitium
_quantitatem_legere (
    chorda  textus,
       s32  initium,
       b32  canonica,
      s64* exitus)
{
    s32 i          = initium;
    s32 n          = (s32)textus.mensura;
    b32 negativum  = FALSUM;
    s32 integra_initium;
    s32 integra_finis;
    s32 fracta_initium  = -I;
    s32 fracta_numerus  = ZEPHYRUM;
    i64 integra         = ZEPHYRUM;
    i64 fracta          = ZEPHYRUM;
    i64 amplitudo;
    s32 k;

    si (i < n && textus.datum[i] == '-')
    {
        negativum = VERUM;
        i++;
    }
    integra_initium = i;
    dum (i < n && _cifra((character)textus.datum[i]))
    {
        i++;
    }
    integra_finis = i;
    si (integra_finis == integra_initium)
    {
        redde PECUNIA_VITIUM_FORMA;
    }
    si (i < n && textus.datum[i] == '.')
    {
        i++;
        fracta_initium = i;
        dum (i < n && _cifra((character)textus.datum[i]))
        {
            i++;
        }
        fracta_numerus = i - fracta_initium;
        si (fracta_numerus == ZEPHYRUM)
        {
            redde PECUNIA_VITIUM_FORMA;
        }
    }
    si (i != n)
    {
        redde PECUNIA_VITIUM_FORMA;
    }
    si (canonica)
    {
        si (   fracta_numerus != IV
            || (textus.datum[integra_initium] == '0'
                && integra_finis - integra_initium > I))
        {
            redde PECUNIA_VITIUM_FORMA;
        }
    }
    alioquin si (fracta_numerus > IV)
    {
        redde PECUNIA_VITIUM_DECIMALES;
    }

    /* valor: pars integra <= 2^63 / 10^4 (= 922337203685477) */
    per (k = integra_initium; k < integra_finis; k++)
    {
        integra = integra * X + (i64)(textus.datum[k] - '0');
        si (integra > PECUNIA_MAGNITUDO_MINIMAE / (i64)PECUNIA_SCALA)
        {
            redde PECUNIA_VITIUM_SUPERFLUXUS;
        }
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        fracta = fracta * X;
        si (k < fracta_numerus)
        {
            fracta += (i64)(textus.datum[fracta_initium + k] - '0');
        }
    }
    amplitudo = integra * (i64)PECUNIA_SCALA + fracta;
    si (negativum && canonica && amplitudo == ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_FORMA;           /* "-0.0000" */
    }
    si (amplitudo > (negativum ? PECUNIA_MAGNITUDO_MINIMAE
                               : (i64)PECUNIA_QUANTITAS_MAXIMA))
    {
        redde PECUNIA_VITIUM_SUPERFLUXUS;
    }
    si (!negativum)
    {
        *exitus = (s64)amplitudo;
    }
    alioquin si (amplitudo == PECUNIA_MAGNITUDO_MINIMAE)
    {
        *exitus = PECUNIA_QUANTITAS_MINIMA;
    }
    alioquin
    {
        *exitus = -(s64)amplitudo;
    }
    redde PECUNIA_BENE;
}

PecuniaVitium
pecunia_ex_textu (
      chorda  textus,
     Pecunia* exitus)
{
    PecuniaMoneta m;
    PecuniaVitium v;
              s64 q;
              s32 i;

    /* "CCC " + saltem "0.0000" */
    si (   textus.datum      == NIHIL || textus.mensura < X
        || textus.datum[III] != ' ')
    {
        redde PECUNIA_VITIUM_FORMA;
    }
    per (i = ZEPHYRUM; i < III; i++)
    {
        si (textus.datum[i] < 'A' || textus.datum[i] > 'Z')
        {
            redde PECUNIA_VITIUM_FORMA;     /* canonica: majusculae */
        }
    }
    v = _quantitatem_legere(textus, IV, VERUM, &q);
    si (v != PECUNIA_BENE)
    {
        redde v;
    }
    v = pecunia_moneta_ex_codice(chorda_sectio(textus, ZEPHYRUM, III),
        &m);
    si (v != PECUNIA_BENE)
    {
        redde v;
    }
    exitus->quantitas  = q;
    exitus->moneta     = m;
    redde PECUNIA_BENE;
}

PecuniaVitium
pecunia_ex_numero (
           chorda  textus,
    PecuniaMoneta  m,
          Pecunia* exitus)
{
    PecuniaVitium v;
              s64 q;

    si (pecunia_moneta_exponens(m) < ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_MONETA_IGNOTA;
    }
    si (textus.datum == NIHIL)
    {
        redde PECUNIA_VITIUM_FORMA;
    }
    v = _quantitatem_legere(textus, ZEPHYRUM, FALSUM, &q);
    si (v != PECUNIA_BENE)
    {
        redde v;
    }
    exitus->quantitas  = q;
    exitus->moneta     = m;
    redde PECUNIA_BENE;
}


/* ==================================================
 * Ostensio et ingressus humanus
 * ================================================== */

/* separatores ordinarii: octeti statici (litterae "," sub
 * -Wwrite-strings constantes sunt - i8* ex eis qualificationem
 * abiceret) */
hic_manens i8 PECUNIA_COMMA[I]  = { ',' };
hic_manens i8 PECUNIA_PUNCTUM[I] = { '.' };

vacuum
pecunia_forma_ordinaria (
    PecuniaForma* forma)
{
    forma->plena              = FALSUM;
    forma->modus              = PECUNIA_PAR;
    forma->coetus.datum       = PECUNIA_COMMA;
    forma->coetus.mensura     = I;
    forma->decimalis.datum    = PECUNIA_PUNCTUM;
    forma->decimalis.mensura  = I;
    forma->rationaria         = FALSUM;
    forma->signum.datum       = NIHIL;
    forma->signum.mensura     = ZEPHYRUM;
    forma->nota_post          = FALSUM;
}

/* numerus ostensus: cifrae integrae cum coetibus, decimalis, cifrae
 * fractae (decimales numero) - ex amplitudine i64 */
interior vacuum
_numerum_ostendere (
        ChordaAedificator* a,
                      i64  amplitudo,
                      s32  decimales,
    constans PecuniaForma* f)
{
          i64 divisor = I;
          i64 integra;
          i64 fracta;
    character cifrae[XXIV];
          s32 k = ZEPHYRUM;
          s32 j;

    per (j = ZEPHYRUM; j < decimales; j++)
    {
        divisor *= X;
    }
    integra  = amplitudo / divisor;
    fracta   = amplitudo % divisor;
    fac
    {
        cifrae[k++]  = (character)('0' + (s32)(integra % X));
        integra      /= X;
    } dum (integra > ZEPHYRUM);
    per (j = k - I; j >= ZEPHYRUM; j--)
    {
        chorda_aedificator_appendere_character(a, cifrae[j]);
        si (   j > ZEPHYRUM && j % III == ZEPHYRUM
            && f->coetus.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_chorda(a, f->coetus);
        }
    }
    si (decimales > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, f->decimalis);
        per (j = decimales - I; j >= ZEPHYRUM; j--)
        {
            i64 d = I;
            s32 e;

            per (e = ZEPHYRUM; e < j; e++)
            {
                d *= X;
            }
            chorda_aedificator_appendere_character(a,
                (character)('0' + (s32)(fracta / d % X)));
        }
    }
}

chorda
pecunia_formare (
                   Pecunia  a,
     constans PecuniaForma* forma,
                   Piscina* piscina)
{
     ChordaAedificator* aed;
                   s32  exponens = pecunia_moneta_exponens(a.moneta);
                   s32  decimales;
                   s64  unitates;
                   i64  amplitudo;
                   b32  negativum;
                   b32  symbolum = forma->signum.mensura > ZEPHYRUM;

    si (forma->plena || exponens < ZEPHYRUM)
    {
        decimales  = IV;
        unitates   = a.quantitas;
    }
    alioquin
    {
        decimales = exponens;
        unitates  = _unitates_rotundatae(a.quantitas,
            _factor_unitatis(exponens), forma->modus);
    }
    aed        = chorda_aedificator_creare(piscina, XLVIII);
    negativum  = unitates < ZEPHYRUM;
    amplitudo  = negativum ? (i64)(-(unitates + I)) + I : (i64)unitates;

    /* codex ante: "USD " extra signum; symbolum intra signum */
    si (!forma->nota_post && !symbolum)
    {
        chorda_aedificator_appendere_literis(aed, a.moneta.codex);
        chorda_aedificator_appendere_character(aed, ' ');
    }
    si (negativum)
    {
        chorda_aedificator_appendere_character(aed,
            forma->rationaria ? '(' : '-');
    }
    si (!forma->nota_post && symbolum)
    {
        chorda_aedificator_appendere_chorda(aed, forma->signum);
    }
    _numerum_ostendere(aed, amplitudo, decimales, forma);
    si (forma->nota_post && symbolum)
    {
        chorda_aedificator_appendere_chorda(aed, forma->signum);
    }
    si (negativum && forma->rationaria)
    {
        chorda_aedificator_appendere_character(aed, ')');
    }
    si (forma->nota_post && !symbolum)
    {
        chorda_aedificator_appendere_character(aed, ' ');
        chorda_aedificator_appendere_literis(aed, a.moneta.codex);
    }
    redde chorda_aedificator_finire(aed);
}

interior b32
_incipit_cum (
    chorda t,
       s32 positio,
    chorda praefixum)
{
    redde praefixum.mensura > ZEPHYRUM
        && positio + (s32)praefixum.mensura <= (s32)t.mensura
        && memcmp(t.datum + positio, praefixum.datum,
                  praefixum.mensura) == ZEPHYRUM;
}

interior b32
_littera (
    i8 c)
{
    redde (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

/* spatia ASCII extrema [a, b) detrahere */
interior vacuum
_spatia_detrahere (
    chorda  t,
       s32* a,
       s32* b)
{
    dum (*a < *b && t.datum[*a] == ' ')
    {
        (*a)++;
    }
    dum (*b > *a && t.datum[*b - I] == ' ')
    {
        (*b)--;
    }
}

/* codex trium litterarum in [a, a+III): moneta m esse debet */
interior PecuniaVitium
_codex_congruit (
           chorda t,
              s32 a,
    PecuniaMoneta m)
{
    PecuniaMoneta inventa;
    PecuniaVitium v = pecunia_moneta_ex_codice(chorda_sectio(t, (i32)a,
                          (i32)(a + III)), &inventa);

    si (v != PECUNIA_BENE)
    {
        redde v;
    }
    redde pecunia_moneta_aequalis(inventa, m) ? PECUNIA_BENE
        : PECUNIA_VITIUM_MONETAE_DIVERSAE;
}

PecuniaVitium
pecunia_ex_ingressu (
                    chorda  textus,
             PecuniaMoneta  m,
     constans PecuniaForma* forma,
                   Pecunia* exitus)
{
              s32 a          = ZEPHYRUM;
              s32 b          = (s32)textus.mensura;
              b32 negativum  = FALSUM;
              b32 nota       = FALSUM;
    PecuniaVitium v;
        character canonica[LXIV];
              s32 k = ZEPHYRUM;
              s32 i;
              s64 q;

    si (pecunia_moneta_exponens(m) < ZEPHYRUM)
    {
        redde PECUNIA_VITIUM_MONETA_IGNOTA;
    }
    si (textus.datum == NIHIL)
    {
        redde PECUNIA_VITIUM_FORMA;
    }
    _spatia_detrahere(textus, &a, &b);
    /* rationaria: parentheses totum amplectuntur */
    si (   b - a               >= II && textus.datum[a] == '('
        && textus.datum[b - I] == ')')
    {
        negativum = VERUM;
        a++;
        b--;
        _spatia_detrahere(textus, &a, &b);
    }
    si (a < b && textus.datum[a] == '-')
    {
        si (negativum)
        {
            redde PECUNIA_VITIUM_FORMA;
        }
        negativum = VERUM;
        a++;
        _spatia_detrahere(textus, &a, &b);
    }
    /* nota ante: symbolum formae aut codex + spatium */
    si (_incipit_cum(textus, a, forma->signum))
    {
        a     += (s32)forma->signum.mensura;
        nota  = VERUM;
    }
    alioquin si (   b - a > III && _littera(textus.datum[a])
                 && _littera(textus.datum[a + I])
                 && _littera(textus.datum[a + II])
                 && textus.datum[a + III] == ' ')
    {
        v = _codex_congruit(textus, a, m);
        si (v != PECUNIA_BENE)
        {
            redde v;
        }
        a     += III;
        nota  = VERUM;
    }
    _spatia_detrahere(textus, &a, &b);
    /* nota post: symbolum aut spatium + codex */
    si (   !nota && forma->signum.mensura > ZEPHYRUM
        && b - (s32)forma->signum.mensura >= a
        && _incipit_cum(textus, b - (s32)forma->signum.mensura,
                        forma->signum))
    {
        b -= (s32)forma->signum.mensura;
    }
    alioquin si (   !nota && b - a > IV && _littera(textus.datum[b - I])
                 && _littera(textus.datum[b - II])
                 && _littera(textus.datum[b - III])
                 && textus.datum[b - IV] == ' ')
    {
        v = _codex_congruit(textus, b - III, m);
        si (v != PECUNIA_BENE)
        {
            redde v;
        }
        b -= III;
    }
    _spatia_detrahere(textus, &a, &b);
    si (a < b && textus.datum[a] == '-')
    {
        si (negativum)
        {
            redde PECUNIA_VITIUM_FORMA;
        }
        negativum = VERUM;
        a++;
    }

    /* numerus: cifrae (cum coetibus in locis veris), decimalis, cifrae
     * -> forma nuda "-d.d" pro _quantitatem_legere */
    si (negativum)
    {
        canonica[k++] = '-';
    }
    {
        s32 coetus_primus  = ZEPHYRUM;   /* cifrae ante coetum primum */
        b32 coetus_visus   = FALSUM;
        s32 in_coetu       = ZEPHYRUM;

        si (a >= b || !_cifra((character)textus.datum[a]))
        {
            redde PECUNIA_VITIUM_FORMA;
        }
        dum (a < b)
        {
            si (_cifra((character)textus.datum[a]))
            {
                si (k >= LX)
                {
                    redde PECUNIA_VITIUM_SUPERFLUXUS;
                }
                canonica[k++] = (character)textus.datum[a];
                a++;
                si (coetus_visus)
                {
                    in_coetu++;
                }
                alioquin
                {
                    coetus_primus++;
                }
            }
            alioquin si (_incipit_cum(textus, a, forma->coetus))
            {
                /* primus 1..III, ceteri exacte III */
                si (   (!coetus_visus && coetus_primus > III)
                    || (coetus_visus && in_coetu != III))
                {
                    redde PECUNIA_VITIUM_FORMA;
                }
                coetus_visus  = VERUM;
                in_coetu      = ZEPHYRUM;
                a             += (s32)forma->coetus.mensura;
            }
            alioquin
            {
                frange;
            }
        }
        si (coetus_visus && in_coetu != III)
        {
            redde PECUNIA_VITIUM_FORMA;
        }
    }
    si (a < b && _incipit_cum(textus, a, forma->decimalis))
    {
        a              += (s32)forma->decimalis.mensura;
        canonica[k++]  = '.';
        per (i = a; i < b && _cifra((character)textus.datum[i]); i++)
        {
            si (k >= LXIII)
            {
                redde PECUNIA_VITIUM_DECIMALES;
            }
            canonica[k++] = (character)textus.datum[i];
        }
        a = i;
    }
    si (a != b)
    {
        redde PECUNIA_VITIUM_FORMA;
    }
    v = _quantitatem_legere(chorda_ex_buffer((i8*)canonica, (i32)k),
        ZEPHYRUM, FALSUM, &q);
    si (v != PECUNIA_BENE)
    {
        redde v;
    }
    exitus->quantitas  = q;
    exitus->moneta     = m;
    redde PECUNIA_BENE;
}
