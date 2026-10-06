/* polynomium.c - Polynomia Laurentiana exacta super magnum
 *
 * Densa: coefficientes[i] pro t^(imus + i). Fines exponentium
 * (|e| <= 2^30 - 1) efficiunt ut omnis amplitudo (summus - imus + 1)
 * in i32 capiat et omne productum exponentis in s64 - ergo probationes
 * finium ipsae sine exundatione computantur. Divisio exacta: longa ex
 * summo, quotiens partialis per coefficientem ducem divisibilis esse
 * debet, residuum nullum. Vide lib/polynomium.worklog.md.
 */
#include "polynomium.h"
#include "chorda_aedificator.h"


/* ==================================================
 * Auxilia
 * ================================================== */

interior b32
_intra (
    s64 exponens)
{
    redde exponens >= -(s64)POLYNOMIUM_EXPONENS_MAXIMUS
        && exponens <= (s64)POLYNOMIUM_EXPONENS_MAXIMUS;
}

interior s64
_summus (
    Polynomium p)
{
    redde (s64)p.imus + (s64)p.numerus - I;
}

/* numerus coefficientium nullorum */
interior Magnus*
_alveus (
    Piscina* piscina,
        i32  numerus)
{
    Magnus* c = (Magnus*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(Magnus));
       i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        c[k] = magnus_ex_s64(ZEPHYRUM);
    }
    redde c;
}

/* forma canonica: zephyra extrema absciduntur (imus crescit) */
interior Polynomium
_ex_alveo (
     constans Magnus* c,
                 i32  numerus,
                 s32  imus)
{
    Polynomium p;
           i32 primus = ZEPHYRUM;

    dum (primus < numerus && magnus_signum(c[primus]) == ZEPHYRUM)
    {
        primus++;
    }
    si (primus == numerus)
    {
        redde polynomium_nullum();
    }
    dum (magnus_signum(c[numerus - I]) == ZEPHYRUM)
    {
        numerus--;
    }
    p.coefficientes  = c + primus;
    p.numerus        = numerus - primus;
    p.imus           = (s32)((s64)imus + (s64)primus);
    redde p;
}

interior b32
_est_digitus (
    i8 c)
{
    redde c >= '0' && c <= '9';
}

interior i32
_transili (
    chorda textus,
       i32 k)
{
    dum (   k < textus.mensura && (textus.datum[k] == ' '
        || textus.datum[k] == '\t'))
    {
        k++;
    }
    redde k;
}


/* ==================================================
 * Constructio et textus
 * ================================================== */

Polynomium
polynomium_nullum (
    vacuum)
{
    Polynomium p;

    p.coefficientes  = NIHIL;
    p.numerus        = ZEPHYRUM;
    p.imus           = ZEPHYRUM;
    redde p;
}

Polynomium
polynomium_constans (
      Magnus  c,
     Piscina* piscina)
{
    Polynomium p = polynomium_nullum();

    (vacuum)polynomium_monomium(c, ZEPHYRUM, piscina, &p);
    redde p;
}

b32
polynomium_monomium (
        Magnus  c,
           s32  exponens,
       Piscina* piscina,
    Polynomium* exitus)
{
    Magnus* alveus;

    si (!_intra((s64)exponens))
    {
        redde FALSUM;
    }
    si (magnus_signum(c) == ZEPHYRUM)
    {
        *exitus = polynomium_nullum();
        redde VERUM;
    }
    alveus                 = _alveus(piscina, I);
    alveus[ZEPHYRUM]       = c;
    exitus->coefficientes  = alveus;
    exitus->numerus        = I;
    exitus->imus           = exponens;
    redde VERUM;
}

b32
polynomium_ex_coefficientibus (
     constans Magnus* c,
                 i32  numerus,
                 s32  imus,
             Piscina* piscina,
          Polynomium* exitus)
{
       i32  primus = ZEPHYRUM;
       i32  ultimus;
       i32  k;
    Magnus* alveus;

    dum (primus < numerus && magnus_signum(c[primus]) == ZEPHYRUM)
    {
        primus++;
    }
    si (primus == numerus)
    {
        *exitus = polynomium_nullum();
        redde VERUM;
    }
    ultimus = numerus - I;
    dum (magnus_signum(c[ultimus]) == ZEPHYRUM)
    {
        ultimus--;
    }
    si (   !_intra((s64)imus + (s64)primus)
        || !_intra((s64)imus + (s64)ultimus))
    {
        redde FALSUM;
    }
    alveus = _alveus(piscina, ultimus - primus + I);
    per (k = primus; k <= ultimus; k++)
    {
        alveus[k - primus] = c[k];
    }
    exitus->coefficientes  = alveus;
    exitus->numerus        = ultimus - primus + I;
    exitus->imus           = (s32)((s64)imus + (s64)primus);
    redde VERUM;
}

b32
polynomium_ex_chorda (
        chorda  textus,
     character  littera,
       Piscina* piscina,
    Polynomium* exitus)
{
    Magnus* valores;
       s64* exponentes;
       i32  numerus_terminorum = ZEPHYRUM;
       i32  k;
       s64  imus;
       s64  summus;
    Magnus* alveus;

    si (textus.datum == NIHIL || textus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (   (littera >= '0' && littera <= '9') || littera == '+'
        || littera == '-' || littera == '^' || littera == ' '
        || littera == '\t')
    {
        redde FALSUM;
    }
    /* quisque terminus saltem characterem unum consumit */
    valores     = _alveus(piscina, textus.mensura);
    exponentes  = (s64*)piscina_allocare(piscina,
        (memoriae_index)textus.mensura * magnitudo(s64));

    k = _transili(textus, ZEPHYRUM);
    dum (VERUM)
    {
           s32 signum               = I;
           b32 habet_coefficientem  = FALSUM;
           b32 habet_litteram       = FALSUM;
        Magnus valor                = magnus_ex_s64(I);
           s64 exponens             = ZEPHYRUM;
           i32 initium;

        si (numerus_terminorum > ZEPHYRUM)
        {
            si (k == textus.mensura)
            {
                frange;
            }
            si (textus.datum[k] == '-')
            {
                signum = -I;
            }
            alioquin si (textus.datum[k] != '+')
            {
                redde FALSUM;
            }
            k = _transili(textus, k + I);
        }
        alioquin si (k < textus.mensura && textus.datum[k] == '-')
        {
            signum  = -I;
            k       = _transili(textus, k + I);
        }

        initium = k;
        dum (k < textus.mensura && _est_digitus(textus.datum[k]))
        {
            k++;
        }
        si (k > initium)
        {
            si (!magnus_ex_chorda(chorda_sectio(textus, initium, k),
                piscina, &valor))
            {
                redde FALSUM;
            }
            habet_coefficientem  = VERUM;
            k                    = _transili(textus, k);
        }

        si (k < textus.mensura && textus.datum[k] == littera)
        {
            habet_litteram  = VERUM;
            exponens        = I;
            k++;
            si (k < textus.mensura && textus.datum[k] == '^')
            {
                b32 negativus = FALSUM;

                k++;
                si (k < textus.mensura && textus.datum[k] == '-')
                {
                    negativus = VERUM;
                    k++;
                }
                initium   = k;
                exponens  = ZEPHYRUM;
                dum (   k < textus.mensura
                     && _est_digitus(textus.datum[k]))
                {
                    exponens = exponens * X
                        + (s64)(textus.datum[k] - '0');
                    si (exponens > (s64)POLYNOMIUM_EXPONENS_MAXIMUS)
                    {
                        redde FALSUM;
                    }
                    k++;
                }
                si (k == initium)
                {
                    redde FALSUM;
                }
                si (negativus)
                {
                    exponens = -exponens;
                }
            }
            k = _transili(textus, k);
        }

        si (!habet_coefficientem && !habet_litteram)
        {
            redde FALSUM;
        }
        si (signum < ZEPHYRUM)
        {
            valor = magnus_nega(valor, piscina);
        }
        valores[numerus_terminorum]     = valor;
        exponentes[numerus_terminorum]  = exponens;
        numerus_terminorum++;
    }

    /* termini in alveum densum; exponentes iterati coniunguntur */
    imus    = exponentes[ZEPHYRUM];
    summus  = exponentes[ZEPHYRUM];
    per (k = I; k < numerus_terminorum; k++)
    {
        si (exponentes[k] < imus)
        {
            imus = exponentes[k];
        }
        si (exponentes[k] > summus)
        {
            summus = exponentes[k];
        }
    }
    alveus = _alveus(piscina, (i32)(summus - imus + I));
    per (k = ZEPHYRUM; k < numerus_terminorum; k++)
    {
        i32 locus = (i32)(exponentes[k] - imus);

        alveus[locus] = magnus_adde(alveus[locus], valores[k], piscina);
    }
    *exitus = _ex_alveo(alveus, (i32)(summus - imus + I), (s32)imus);
    redde VERUM;
}

chorda
polynomium_ad_chordam (
    Polynomium  p,
     character  littera,
       Piscina* piscina)
{
     ChordaAedificator* scriba;
                   i32  k;
                   b32  primus  = VERUM;
                Magnus  unum    = magnus_ex_s64(I);

    si (p.numerus == ZEPHYRUM)
    {
        redde chorda_ex_literis("0", piscina);
    }
    scriba = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);
    per (k = p.numerus; k-- > ZEPHYRUM;)
    {
        Magnus c         = p.coefficientes[k];
           s32 exponens  = (s32)((s64)p.imus + (s64)k);
        Magnus modulus;

        si (magnus_signum(c) == ZEPHYRUM)
        {
            perge;
        }
        si (primus)
        {
            si (magnus_signum(c) < ZEPHYRUM)
            {
                (vacuum)chorda_aedificator_appendere_character(scriba,
                    '-');
            }
            primus = FALSUM;
        }
        alioquin
        {
            (vacuum)chorda_aedificator_appendere_literis(scriba,
                magnus_signum(c) < ZEPHYRUM ? " - " : " + ");
        }
        modulus = magnus_absolutum(c, piscina);
        si (exponens == ZEPHYRUM || !magnus_aequalis(modulus, unum))
        {
            (vacuum)chorda_aedificator_appendere_chorda(scriba,
                magnus_ad_chordam(modulus, piscina));
        }
        si (exponens != ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_character(scriba,
                littera);
            si (exponens != I)
            {
                (vacuum)chorda_aedificator_appendere_character(scriba,
                    '^');
                (vacuum)chorda_aedificator_appendere_s32(scriba,
                    exponens);
            }
        }
    }
    redde chorda_aedificator_finire(scriba);
}


/* ==================================================
 * Lectio
 * ================================================== */

b32
polynomium_est_nullum (
    Polynomium p)
{
    redde p.numerus == ZEPHYRUM;
}

s32
polynomium_gradus_imus (
    Polynomium p)
{
    redde p.imus;
}

s32
polynomium_gradus_summus (
    Polynomium p)
{
    si (p.numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s32)_summus(p);
}

Magnus
polynomium_coefficiens (
    Polynomium p,
           s32 exponens)
{
    s64 locus = (s64)exponens - (s64)p.imus;

    si (locus < ZEPHYRUM || locus >= (s64)p.numerus)
    {
        redde magnus_ex_s64(ZEPHYRUM);
    }
    redde p.coefficientes[(i32)locus];
}

b32
polynomium_aequalis (
    Polynomium a,
    Polynomium b)
{
    i32 k;

    si (a.numerus != b.numerus || a.imus != b.imus)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        si (!magnus_aequalis(a.coefficientes[k], b.coefficientes[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

Magnus
polynomium_contentum (
    Polynomium  p,
       Piscina* piscina)
{
    Magnus g = magnus_ex_s64(ZEPHYRUM);
       i32 k;

    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        g = magnus_divisor_communis(g, p.coefficientes[k], piscina);
    }
    redde g;
}


/* ==================================================
 * Arithmetica
 * ================================================== */

Polynomium
polynomium_nega (
    Polynomium  a,
       Piscina* piscina)
{
     Magnus* alveus;
        i32  k;

    si (a.numerus == ZEPHYRUM)
    {
        redde a;
    }
    alveus = _alveus(piscina, a.numerus);
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        alveus[k] = magnus_nega(a.coefficientes[k], piscina);
    }
    redde _ex_alveo(alveus, a.numerus, a.imus);
}

/* a + signum * b */
interior Polynomium
_summa (
    Polynomium  a,
    Polynomium  b,
           s32  signum,
       Piscina* piscina)
{
     Magnus* alveus;
        s64  imus;
        s64  summus;
        i32  numerus;
        i32  k;

    si (b.numerus == ZEPHYRUM)
    {
        redde a;
    }
    si (a.numerus == ZEPHYRUM)
    {
        redde signum > ZEPHYRUM ? b : polynomium_nega(b, piscina);
    }
    imus     = a.imus < b.imus ? (s64)a.imus : (s64)b.imus;
    summus   = _summus(a) > _summus(b) ? _summus(a) : _summus(b);
    numerus  = (i32)(summus - imus + I);
    alveus   = _alveus(piscina, numerus);
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        alveus[(i32)((s64)a.imus - imus) + k] = a.coefficientes[k];
    }
    per (k = ZEPHYRUM; k < b.numerus; k++)
    {
        i32 locus = (i32)((s64)b.imus - imus) + k;

        alveus[locus] = signum > ZEPHYRUM
            ? magnus_adde(alveus[locus], b.coefficientes[k], piscina)
            : magnus_subtrahe(alveus[locus], b.coefficientes[k],
            piscina);
    }
    redde _ex_alveo(alveus, numerus, (s32)imus);
}

Polynomium
polynomium_adde (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina)
{
    redde _summa(a, b, I, piscina);
}

Polynomium
polynomium_subtrahe (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina)
{
    redde _summa(a, b, -I, piscina);
}

Polynomium
polynomium_multiplica_scalari (
    Polynomium  p,
        Magnus  c,
       Piscina* piscina)
{
     Magnus* alveus;
        i32  k;

    si (p.numerus == ZEPHYRUM || magnus_signum(c) == ZEPHYRUM)
    {
        redde polynomium_nullum();
    }
    alveus = _alveus(piscina, p.numerus);
    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        alveus[k] = magnus_multiplica(p.coefficientes[k], c, piscina);
    }
    redde _ex_alveo(alveus, p.numerus, p.imus);
}

b32
polynomium_multiplica (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* exitus)
{
     Magnus* alveus;
        s64  imus;
        i32  numerus;
        i32  i;
        i32  j;

    si (a.numerus == ZEPHYRUM || b.numerus == ZEPHYRUM)
    {
        *exitus = polynomium_nullum();
        redde VERUM;
    }
    imus = (s64)a.imus + (s64)b.imus;
    si (!_intra(imus) || !_intra(_summus(a) + _summus(b)))
    {
        redde FALSUM;
    }
    numerus  = a.numerus + b.numerus - I;
    alveus   = _alveus(piscina, numerus);
    per (i = ZEPHYRUM; i < a.numerus; i++)
    {
        per (j = ZEPHYRUM; j < b.numerus; j++)
        {
            alveus[i + j] = magnus_adde(alveus[i + j],
                magnus_multiplica(a.coefficientes[i],
                b.coefficientes[j],
                    piscina), piscina);
        }
    }
    *exitus = _ex_alveo(alveus, numerus, (s32)imus);
    redde VERUM;
}

b32
polynomium_potentia (
    Polynomium  p,
           i32  n,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium effectus;
    Polynomium basis     = p;
           i32 reliquum  = n;

    si (n == ZEPHYRUM)
    {
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    /* |exponens| <= 2^30, n < 2^32: productum in s64 capit */
    si (   !_intra((s64)p.imus * (s64)n)
        || !_intra(_summus(p) * (s64)n))
    {
        redde FALSUM;
    }
    /* quadrata intermedia p^(2^k), 2^k <= n: intra fines */
    effectus = polynomium_constans(magnus_ex_s64(I), piscina);
    dum (VERUM)
    {
        si (reliquum & I)
        {
            si (!polynomium_multiplica(effectus, basis, piscina,
                &effectus))
            {
                redde FALSUM;
            }
        }
        reliquum >>= I;
        si (reliquum == ZEPHYRUM)
        {
            frange;
        }
        si (!polynomium_multiplica(basis, basis, piscina, &basis))
        {
            redde FALSUM;
        }
    }
    *exitus = effectus;
    redde VERUM;
}

b32
polynomium_divide_exacte (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* quotiens)
{
     Magnus* residua;
     Magnus* partes;
     Magnus  dux;
        s64  imus;
        i32  numerus;
        i32  k;
        i32  j;

    si (b.numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (a.numerus == ZEPHYRUM)
    {
        *quotiens = a;
        redde VERUM;
    }
    si (a.numerus < b.numerus)
    {
        redde FALSUM;
    }
    imus = (s64)a.imus - (s64)b.imus;
    si (!_intra(imus) || !_intra(_summus(a) - _summus(b)))
    {
        redde FALSUM;
    }

    /* A = a t^-imus(a), B = b t^-imus(b): termini constantes non
     * nulli (t unitas est); divisio longa in Z[t] ex summo */
    numerus  = a.numerus - b.numerus + I;
    residua  = _alveus(piscina, a.numerus);
    partes   = _alveus(piscina, numerus);
    dux      = b.coefficientes[b.numerus - I];
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        residua[k] = a.coefficientes[k];
    }
    per (k = numerus; k-- > ZEPHYRUM;)
    {
        Magnus residuum;

        (vacuum)magnus_divide(residua[k + b.numerus - I], dux, piscina,
            &partes[k], &residuum);
        si (magnus_signum(residuum) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (magnus_signum(partes[k]) == ZEPHYRUM)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < b.numerus; j++)
        {
            residua[k + j] = magnus_subtrahe(residua[k + j],
                magnus_multiplica(partes[k], b.coefficientes[j],
                piscina),
                piscina);
        }
    }
    /* residuum infra gradum B nullum esse debet */
    per (k = ZEPHYRUM; k + I < b.numerus; k++)
    {
        si (magnus_signum(residua[k]) != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    *quotiens = _ex_alveo(partes, numerus, (s32)imus);
    redde VERUM;
}


/* ==================================================
 * Substitutiones
 * ================================================== */

b32
polynomium_translata (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus)
{
    (vacuum)piscina;
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    si (   !_intra((s64)p.imus + (s64)k)
        || !_intra(_summus(p) + (s64)k))
    {
        redde FALSUM;
    }
    *exitus       = p;
    exitus->imus  = (s32)((s64)p.imus + (s64)k);
    redde VERUM;
}

b32
polynomium_dilata (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus)
{
     Magnus* alveus;
        s64  imus;
        s64  summus;
        i32  i;

    si (k == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    /* |exponens| < 2^30, |k| <= 2^31: productum in s64 capit */
    imus    = (s64)p.imus * (s64)k;
    summus  = _summus(p) * (s64)k;
    si (k < ZEPHYRUM)
    {
        s64 t = imus;

        imus    = summus;
        summus  = t;
    }
    si (!_intra(imus) || !_intra(summus))
    {
        redde FALSUM;
    }
    alveus = _alveus(piscina, (i32)(summus - imus + I));
    per (i = ZEPHYRUM; i < p.numerus; i++)
    {
        s64 exponens = ((s64)p.imus + (s64)i) * (s64)k;

        alveus[(i32)(exponens - imus)] = p.coefficientes[i];
    }
    *exitus = _ex_alveo(alveus, (i32)(summus - imus + I), (s32)imus);
    redde VERUM;
}

b32
polynomium_contrahe (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus)
{
     Magnus* alveus;
        s64  modulus_k;
        s64  imus;
        s64  summus;
        i32  i;

    si (k == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    /* divisibilitas per modulos non negativos (C89 '%' cum negativis
     * definitionem non habet) */
    modulus_k = k < ZEPHYRUM ? -(s64)k : (s64)k;
    per (i = ZEPHYRUM; i < p.numerus; i++)
    {
        s64 exponens  = (s64)p.imus + (s64)i;
        s64 modulus   = exponens < ZEPHYRUM ? -exponens : exponens;

        si (   magnus_signum(p.coefficientes[i]) != ZEPHYRUM
            && modulus % modulus_k               != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    /* quotientes exacti: e / k = signum * (|e| / |k|); |e/k| <= |e|,
     * ergo intra fines */
    imus    = (s64)p.imus;
    summus  = _summus(p);
    imus    = (imus < ZEPHYRUM ? -((-imus) / modulus_k) : imus
        / modulus_k);
    summus  = (summus < ZEPHYRUM ? -((-summus) / modulus_k) : summus
        / modulus_k);
    si (k < ZEPHYRUM)
    {
        s64 t = -imus;

        imus    = -summus;
        summus  = t;
    }
    alveus = _alveus(piscina, (i32)(summus - imus + I));
    per (i = ZEPHYRUM; i < p.numerus; i++)
    {
        s64 exponens  = (s64)p.imus + (s64)i;
        s64 modulus   = exponens < ZEPHYRUM ? -exponens : exponens;
        s64 novus;

        si (magnus_signum(p.coefficientes[i]) == ZEPHYRUM)
        {
            perge;
        }
        novus = modulus / modulus_k;
        si ((exponens < ZEPHYRUM) != (k < ZEPHYRUM))
        {
            novus = -novus;
        }
        alveus[(i32)(novus - imus)] = p.coefficientes[i];
    }
    *exitus = _ex_alveo(alveus, (i32)(summus - imus + I), (s32)imus);
    redde VERUM;
}

b32
polynomium_normale (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium q;

    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    si (!polynomium_translata(p, -p.imus, piscina, &q))
    {
        redde FALSUM;
    }
    si (magnus_signum(q.coefficientes[ZEPHYRUM]) < ZEPHYRUM)
    {
        q = polynomium_nega(q, piscina);
    }
    *exitus = q;
    redde VERUM;
}


/* ==================================================
 * Valor
 * ================================================== */

b32
polynomium_valor (
    Polynomium  p,
       Fractio  x,
       Piscina* piscina,
       Fractio* exitus)
{
    Fractio valor = fractio_ex_s64(ZEPHYRUM);
    Fractio factor_imus;
        i32 k;

    si (p.numerus == ZEPHYRUM)
    {
        *exitus = valor;
        redde VERUM;
    }
    si (!fractio_potentia(x, p.imus, piscina, &factor_imus))
    {
        redde FALSUM;   /* x = 0, exponens negativus */
    }
    per (k = p.numerus; k-- > ZEPHYRUM;)
    {
        valor = fractio_adde(fractio_multiplica(valor, x, piscina),
            fractio_ex_magno(p.coefficientes[k]), piscina);
    }
    *exitus = fractio_multiplica(valor, factor_imus, piscina);
    redde VERUM;
}
