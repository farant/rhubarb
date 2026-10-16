/* nervus.c - retia exacta per partes linearia (vide include/nervus.h)
 *
 * Rete: introitus registra, deinde una per instructionem (assignatio
 * unica). Aestimatio ordine instructionum, valoribus Magnus. Figura per
 * transitum unum (profunditas) et intervalla vitae (latitudo).
 */
#include "nervus.h"


/* ==================================================
 * Auxilia
 * ================================================== */

/* caput retis: numeri et alvei quos constructio et aestimatio legunt.
 * O(1); instructiones singulas nervus_validus probat. */
interior b32
_caput_validum (
    constans Nervus* rete)
{
    si (rete == NIHIL)
    {
        redde FALSUM;
    }
    si (   rete->introitus > NERVUS_REGISTRA_MAXIMA
        || rete->numerus > rete->capacitas
        || rete->numerus > NERVUS_REGISTRA_MAXIMA
        || rete->introitus + rete->numerus > NERVUS_REGISTRA_MAXIMA
        || rete->relu > rete->numerus
        || rete->exitus_numerus > NERVUS_REGISTRA_MAXIMA)
    {
        redde FALSUM;
    }
    si (rete->capacitas > ZEPHYRUM && rete->instructiones == NIHIL)
    {
        redde FALSUM;
    }
    si (rete->exitus_numerus > ZEPHYRUM && rete->exitus == NIHIL)
    {
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_instructionem_addere (
                Nervus* rete,
           NervusGenus  genus,
                   i32  termini,
          constans s32* fontes,
       constans Magnus* coefficientes,
                Magnus  addendum,
                   s32* registrum_novum)
{
                 i32  registra;
                 i32  k;
    NervusInstructio* instructio;

    si (   !_caput_validum(rete) || rete->piscina == NIHIL
        || registrum_novum == NIHIL)
    {
        redde FALSUM;
    }
    registra = rete->introitus + rete->numerus;
    si (registra >= NERVUS_REGISTRA_MAXIMA)
    {
        redde FALSUM;
    }
    si (termini > NERVUS_TERMINI_MAXIMI)
    {
        redde FALSUM;
    }
    si (   termini > ZEPHYRUM
        && (fontes == NIHIL || coefficientes == NIHIL))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < termini; k++)
    {
        si (fontes[k] < ZEPHYRUM || (i32)fontes[k] >= registra)
        {
            redde FALSUM;
        }
    }
    /* crescere: alveus novus duplex, priores copiantur */
    si (rete->numerus == rete->capacitas)
    {
                     i32 nova = rete->capacitas > ZEPHYRUM
                                 ? II * rete->capacitas : XVI;
        NervusInstructio* alveus;

        alveus = (NervusInstructio*)piscina_allocare(rete->piscina,
            (memoriae_index)nova * magnitudo(NervusInstructio));
        per (k = ZEPHYRUM; k < rete->numerus; k++)
        {
            alveus[k] = rete->instructiones[k];
        }
        rete->instructiones  = alveus;
        rete->capacitas      = nova;
    }
    instructio                 = &rete->instructiones[rete->numerus];
    instructio->genus          = genus;
    instructio->termini        = termini;
    instructio->fontes         = NIHIL;
    instructio->coefficientes  = NIHIL;
    si (termini > ZEPHYRUM)
    {
        instructio->fontes = (s32*)piscina_allocare(rete->piscina,
            (memoriae_index)termini * magnitudo(s32));
        instructio->coefficientes =
            (Magnus*)piscina_allocare(rete->piscina,
            (memoriae_index)termini * magnitudo(Magnus));
        per (k = ZEPHYRUM; k < termini; k++)
        {
            instructio->fontes[k]         = fontes[k];
            instructio->coefficientes[k]  =
                magnus_transcribe(coefficientes[k],
                rete->piscina);
        }
    }
    instructio->addendum = magnus_transcribe(addendum, rete->piscina);
    rete->numerus++;
    si (genus == NERVUS_RELU)
    {
        rete->relu++;
    }
    *registrum_novum = (s32)registra;
    redde VERUM;
}


/* ==================================================
 * Constructio
 * ================================================== */

b32
nervus_creare (
         i32   introitus,
     Piscina*  piscina,
      Nervus** exitus)
{
    Nervus* rete;

    si (   introitus > NERVUS_REGISTRA_MAXIMA || piscina == NIHIL
        || exitus == NIHIL)
    {
        redde FALSUM;
    }
    rete = (Nervus*)piscina_allocare(piscina, magnitudo(Nervus));
    rete->introitus = introitus;
    rete->numerus = ZEPHYRUM;
    rete->capacitas = ZEPHYRUM;
    rete->instructiones = NIHIL;
    rete->relu = ZEPHYRUM;
    rete->exitus_numerus = ZEPHYRUM;
    rete->exitus = NIHIL;
    rete->piscina = piscina;
    *exitus = rete;
    redde VERUM;
}

b32
nervus_affinis (
               Nervus* rete,
                  i32  termini,
         constans s32* fontes,
      constans Magnus* coefficientes,
               Magnus  addendum,
                  s32* registrum_novum)
{
    redde _instructionem_addere(rete, NERVUS_AFFINIS, termini, fontes,
        coefficientes, addendum, registrum_novum);
}

b32
nervus_relu (
               Nervus* rete,
                  i32  termini,
         constans s32* fontes,
      constans Magnus* coefficientes,
               Magnus  addendum,
                  s32* registrum_novum)
{
    redde _instructionem_addere(rete, NERVUS_RELU, termini, fontes,
        coefficientes, addendum, registrum_novum);
}

b32
nervus_exitus (
          Nervus* rete,
             i32  numerus,
    constans s32* registra)
{
     i32  definita;
     i32  k;
     s32* alveus;

    si (!_caput_validum(rete) || rete->piscina == NIHIL)
    {
        redde FALSUM;
    }
    si (numerus > NERVUS_REGISTRA_MAXIMA)
    {
        redde FALSUM;
    }
    si (numerus > ZEPHYRUM && registra == NIHIL)
    {
        redde FALSUM;
    }
    definita = rete->introitus + rete->numerus;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (registra[k] < ZEPHYRUM || (i32)registra[k] >= definita)
        {
            redde FALSUM;
        }
    }
    alveus = NIHIL;
    si (numerus > ZEPHYRUM)
    {
        alveus = (s32*)piscina_allocare(rete->piscina,
            (memoriae_index)numerus * magnitudo(s32));
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            alveus[k] = registra[k];
        }
    }
    rete->exitus          = alveus;
    rete->exitus_numerus  = numerus;
    redde VERUM;
}

b32
nervus_validus (
    constans Nervus* rete)
{
    i32 k;
    i32 j;
    i32 relu = ZEPHYRUM;
    i32 definita;

    si (!_caput_validum(rete))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
        constans NervusInstructio* instructio = &rete->instructiones[k];

        definita = rete->introitus + k;
        si (   instructio->genus != NERVUS_AFFINIS
            && instructio->genus != NERVUS_RELU)
        {
            redde FALSUM;
        }
        si (instructio->termini > NERVUS_TERMINI_MAXIMI)
        {
            redde FALSUM;
        }
        si (   instructio->termini > ZEPHYRUM
            && (instructio->fontes == NIHIL
            || instructio->coefficientes == NIHIL))
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < instructio->termini; j++)
        {
            si (   instructio->fontes[j] < ZEPHYRUM
                || (i32)instructio->fontes[j] >= definita)
            {
                redde FALSUM;
            }
        }
        si (instructio->genus == NERVUS_RELU)
        {
            relu++;
        }
    }
    si (relu != rete->relu)
    {
        redde FALSUM;
    }
    definita = rete->introitus + rete->numerus;
    per (k = ZEPHYRUM; k < rete->exitus_numerus; k++)
    {
        si (   rete->exitus[k] < ZEPHYRUM
            || (i32)rete->exitus[k] >= definita)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Aestimatio
 * ================================================== */

b32
nervus_forma_creare (
    constans Nervus* rete,
            Piscina* piscina,
        NervusForma* exitus)
{
    i32  octeti;
    i32  k;
     i8* bitus;

    si (!nervus_validus(rete) || piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    octeti = (rete->relu + VII) / VIII;
    si (octeti == ZEPHYRUM)
    {
        octeti = I;     /* numquam NIHIL */
    }
    bitus = (i8*)piscina_allocare(piscina, (memoriae_index)octeti);
    per (k = ZEPHYRUM; k < octeti; k++)
    {
        bitus[k] = ZEPHYRUM;
    }
    exitus->numerus  = rete->relu;
    exitus->bitus    = bitus;
    redde VERUM;
}

b32
nervus_forma_bitus (
    constans NervusForma* forma,
                     i32  k)
{
    si (forma == NIHIL || forma->bitus == NIHIL || k >= forma->numerus)
    {
        redde FALSUM;
    }
    redde ((forma->bitus[k >> III] >> (k & VII)) & I) != ZEPHYRUM;
}

b32
nervus_aestimare (
    constans Nervus* rete,
    constans Magnus* introitus,
             Magnus* exitus,
        NervusForma* forma,
            Piscina* piscina)
{
       i32  registra;
       i32  k;
       i32  j;
       i32  r = ZEPHYRUM;
    Magnus* valores;

    si (!nervus_validus(rete) || piscina == NIHIL)
    {
        redde FALSUM;
    }
    si (rete->introitus > ZEPHYRUM && introitus == NIHIL)
    {
        redde FALSUM;
    }
    si (rete->exitus_numerus > ZEPHYRUM && exitus == NIHIL)
    {
        redde FALSUM;
    }
    si (   forma != NIHIL
        && (forma->numerus != rete->relu || forma->bitus == NIHIL))
    {
        redde FALSUM;
    }
    registra  = rete->introitus + rete->numerus;
    valores   = NIHIL;
    si (registra > ZEPHYRUM)
    {
        valores = (Magnus*)piscina_allocare(piscina,
            (memoriae_index)registra * magnitudo(Magnus));
    }
    per (k = ZEPHYRUM; k < rete->introitus; k++)
    {
        valores[k] = introitus[k];
    }
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
         constans NervusInstructio* instructio;
                            Magnus  summa;

        instructio  = &rete->instructiones[k];
        summa       = instructio->addendum;

        per (j = ZEPHYRUM; j < instructio->termini; j++)
        {
            summa = magnus_adde(summa, magnus_multiplica(
                instructio->coefficientes[j],
                valores[instructio->fontes[j]],
                piscina), piscina);
        }
        si (instructio->genus == NERVUS_RELU)
        {
            b32 activa = magnus_signum(summa) > ZEPHYRUM;

            si (!activa)
            {
                summa = magnus_ex_s64(ZEPHYRUM);
            }
            si (forma != NIHIL)
            {
                i8 larva = (i8)(I << (r & VII));

                si (activa)
                {
                    forma->bitus[r >> III] = (i8)(forma->bitus[r >> III]
                        | larva);
                }
                alioquin
                {
                    forma->bitus[r >> III] = (i8)(forma->bitus[r >> III]
                        & (i8)~larva);
                }
            }
            r++;
        }
        valores[rete->introitus + k] = summa;
    }
    per (k = ZEPHYRUM; k < rete->exitus_numerus; k++)
    {
        exitus[k] = valores[rete->exitus[k]];
    }
    redde VERUM;
}

b32
nervus_figura (
    constans Nervus* rete,
       NervusFigura* exitus)
{
         Piscina* temporaria;
             i32  registra;
             i32  k;
             i32  j;
             s32* profunditas;
             s32* profunditas_relu;
             s32* ultimus;       /* ultimus usus; numerus = exitus */
             s32* differentia;   /* tempora -1 .. numerus-1, +1 */
             s32  vivi     = ZEPHYRUM;
             s32  maximum  = ZEPHYRUM;
    NervusFigura  figura;

    si (!nervus_validus(rete) || exitus == NIHIL)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("nervus_figura",
        IV * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    registra = rete->introitus + rete->numerus;
    profunditas = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(registra + I) * magnitudo(s32));
    profunditas_relu = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(registra + I) * magnitudo(s32));
    ultimus = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(registra + I) * magnitudo(s32));
    differentia = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(rete->numerus + III) * magnitudo(s32));
    per (k = ZEPHYRUM; k < registra; k++)
    {
        profunditas[k]       = ZEPHYRUM;
        profunditas_relu[k]  = ZEPHYRUM;
        ultimus[k]           = -I;
    }
    per (k = ZEPHYRUM; k < rete->numerus + III; k++)
    {
        differentia[k] = ZEPHYRUM;
    }
    figura.instructiones     = rete->numerus;
    figura.relu              = rete->relu;
    figura.termini           = ZEPHYRUM;
    figura.profunditas       = ZEPHYRUM;
    figura.profunditas_relu  = ZEPHYRUM;
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
         constans NervusInstructio* instructio;
                               s32  p     = ZEPHYRUM;
                               s32  q     = ZEPHYRUM;
                               i32  ipse  = rete->introitus + k;

        instructio = &rete->instructiones[k];

        per (j = ZEPHYRUM; j < instructio->termini; j++)
        {
            s32 f = instructio->fontes[j];

            si (profunditas[f] > p)
            {
                p = profunditas[f];
            }
            si (profunditas_relu[f] > q)
            {
                q = profunditas_relu[f];
            }
            ultimus[f] = (s32)k;
        }
        profunditas[ipse] = p + I;
        profunditas_relu[ipse] = q + (instructio->genus
            == NERVUS_RELU ? I : ZEPHYRUM);
        si ((i32)profunditas[ipse] > figura.profunditas)
        {
            figura.profunditas = (i32)profunditas[ipse];
        }
        si ((i32)profunditas_relu[ipse] > figura.profunditas_relu)
        {
            figura.profunditas_relu = (i32)profunditas_relu[ipse];
        }
        figura.termini += (i64)instructio->termini;
    }
    per (k = ZEPHYRUM; k < rete->exitus_numerus; k++)
    {
        ultimus[rete->exitus[k]] = (s32)rete->numerus;
    }
    /* registrum r (definitum tempore d: -1 pro introitu, k pro
     * instructione k) vivit temporibus d .. u-1, u = ultimus usus */
    per (k = ZEPHYRUM; k < registra; k++)
    {
        s32 d = k < rete->introitus ? -I
                : (s32)(k - rete->introitus);
        s32 u = ultimus[k];

        si (u > d)
        {
            differentia[d + I]++;
            differentia[u + I]--;
        }
    }
    per (k = ZEPHYRUM; k < rete->numerus + II; k++)
    {
        vivi += differentia[k];
        si (vivi > maximum)
        {
            maximum = vivi;
        }
    }
    figura.latitudo = (i32)maximum;
    piscina_destruere(temporaria);
    *exitus = figura;
    redde VERUM;
}
