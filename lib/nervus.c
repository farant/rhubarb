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

/* forma utilis: non NIHIL, bitus non NIHIL */
interior b32
_forma_valida (
    constans NervusForma* forma)
{
    redde forma != NIHIL && forma->bitus != NIHIL;
}

interior b32
_bitus (
    constans NervusForma* forma,
                     i32  k)
{
    redde ((forma->bitus[k >> III] >> (k & VII)) & I) != ZEPHYRUM;
}

interior vacuum
_bitum_ponere (
    NervusForma* forma,
            i32  k,
            b32  activa)
{
    i8 larva = (i8)(I << (k & VII));

    si (activa)
    {
        forma->bitus[k >> III] = (i8)(forma->bitus[k >> III] | larva);
    }
    alioquin
    {
        forma->bitus[k >> III] = (i8)(forma->bitus[k >> III]
            & (i8)~larva);
    }
}

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
    si (!_forma_valida(forma) || k >= forma->numerus)
    {
        redde FALSUM;
    }
    redde _bitus(forma, k);
}

/* aestimatio communis: scripta (NIHIL licet) bitus accipit; coacta
 * (NIHIL licet) ReLU regit */
interior b32
_aestimare (
         constans Nervus* rete,
         constans Magnus* introitus,
                  Magnus* exitus,
             NervusForma* scripta,
    constans NervusForma* coacta,
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
    si (   scripta != NIHIL
        && (scripta->numerus != rete->relu || scripta->bitus == NIHIL))
    {
        redde FALSUM;
    }
    si (   coacta != NIHIL
        && (coacta->numerus != rete->relu || coacta->bitus == NIHIL))
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
                valores[instructio->fontes[j]], piscina), piscina);
        }
        si (instructio->genus == NERVUS_RELU)
        {
            b32 activa = coacta != NIHIL ? _bitus(coacta, r)
                         : magnus_signum(summa) > ZEPHYRUM;

            si (!activa)
            {
                summa = magnus_ex_s64(ZEPHYRUM);
            }
            si (scripta != NIHIL)
            {
                _bitum_ponere(scripta, r, activa);
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
nervus_aestimare (
    constans Nervus* rete,
    constans Magnus* introitus,
             Magnus* exitus,
        NervusForma* forma,
            Piscina* piscina)
{
    redde _aestimare(rete, introitus, exitus, forma, NIHIL, piscina);
}

b32
nervus_aestimare_coacte (
         constans Nervus* rete,
    constans NervusForma* forma,
         constans Magnus* introitus,
                  Magnus* exitus,
                 Piscina* piscina)
{
    si (forma == NIHIL)
    {
        redde FALSUM;
    }
    redde _aestimare(rete, introitus, exitus, NIHIL, forma, piscina);
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


/* ==================================================
 * Formae
 * ================================================== */

b32
nervus_forma_ponere (
    NervusForma* forma,
            i32  k,
            b32  activa)
{
    si (!_forma_valida(forma) || k >= forma->numerus)
    {
        redde FALSUM;
    }
    _bitum_ponere(forma, k, activa);
    redde VERUM;
}

i32
nervus_forma_activae (
    constans NervusForma* forma)
{
    i32 k;
    i32 numerus = ZEPHYRUM;

    si (!_forma_valida(forma))
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < forma->numerus; k++)
    {
        si (_bitus(forma, k))
        {
            numerus++;
        }
    }
    redde numerus;
}

b32
nervus_formae_aequales (
    constans NervusForma* a,
    constans NervusForma* b)
{
    i32 k;

    si (   !_forma_valida(a) || !_forma_valida(b)
        || a->numerus != b->numerus)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < a->numerus; k++)
    {
        si (_bitus(a, k) != _bitus(b, k))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
nervus_forma_clavis (
    constans NervusForma* forma,
                 Piscina* piscina,
                  chorda* exitus)
{
    i32  octeti;
    i32  k;
     i8* datum;

    si (!_forma_valida(forma) || piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    octeti  = (forma->numerus + VII) / VIII;
    datum   = (i8*)piscina_allocare(piscina,
        (memoriae_index)(octeti > ZEPHYRUM ? octeti : I));
    per (k = ZEPHYRUM; k < octeti; k++)
    {
        datum[k] = forma->bitus[k];
    }
    /* bitus ultra numerum nulli */
    si ((forma->numerus & VII) != ZEPHYRUM)
    {
        datum[octeti - I] = (i8)(datum[octeti - I]
            & (i8)((I << (forma->numerus & VII)) - I));
    }
    exitus->mensura  = octeti;
    exitus->datum    = datum;
    redde VERUM;
}


/* ==================================================
 * Mappa affinis formae
 * ================================================== */

b32
nervus_forma_affinis (
         constans Nervus*  rete,
    constans NervusForma*  forma,
                 Piscina*  piscina,
                  Matrix*  m,
                  Magnus** c)
{
    Piscina* temporaria;
     Matrix  matrix_effectus;
     Magnus* cc;
     Magnus* formae;     /* registra x (introitus + 1): coef..., c */
        i32  latus;
        i32  registra;
        i32  k;
        i32  j;
        i32  t;
        i32  r = ZEPHYRUM;

    si (   !nervus_validus(rete) || !_forma_valida(forma)
        || forma->numerus != rete->relu || piscina == NIHIL
        || m              == NIHIL
        || c              == NIHIL)
    {
        redde FALSUM;
    }
    latus     = rete->introitus + I;
    registra  = rete->introitus + rete->numerus;
    /* formae registra x latus Magnus; indices in i32 */
    si ((memoriae_index)registra * (memoriae_index)latus
        > ((memoriae_index)I << XXVII))
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("nervus_affinis",
        IV * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    formae    = (Magnus*)piscina_allocare(temporaria,
        (memoriae_index)registra * (memoriae_index)latus
        * magnitudo(Magnus));
    per (k = ZEPHYRUM; k < rete->introitus; k++)
    {
        per (t = ZEPHYRUM; t < latus; t++)
        {
            formae[k * latus + t] = magnus_ex_s64(t
                == k ? I : ZEPHYRUM);
        }
    }
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
        constans NervusInstructio* instructio;
                           Magnus* ipsa;

        instructio  = &rete->instructiones[k];
        ipsa        = &formae[(rete->introitus + k) * latus];
        per (t = ZEPHYRUM; t < rete->introitus; t++)
        {
            ipsa[t] = magnus_ex_s64(ZEPHYRUM);
        }
        ipsa[rete->introitus] = instructio->addendum;
        per (j = ZEPHYRUM; j < instructio->termini; j++)
        {
            Magnus* fons = &formae[(i32)instructio->fontes[j] * latus];

            per (t = ZEPHYRUM; t < latus; t++)
            {
                ipsa[t] = magnus_adde(ipsa[t], magnus_multiplica(
                    instructio->coefficientes[j], fons[t], temporaria),
                    temporaria);
            }
        }
        si (instructio->genus == NERVUS_RELU)
        {
            si (!_bitus(forma, r))
            {
                per (t = ZEPHYRUM; t < latus; t++)
                {
                    ipsa[t] = magnus_ex_s64(ZEPHYRUM);
                }
            }
            r++;
        }
    }
    si (!matrix_nulla(&ANULUS_INTEGRORUM, rete->exitus_numerus,
        rete->introitus, piscina, &matrix_effectus))
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
    cc = NIHIL;
    si (rete->exitus_numerus > ZEPHYRUM)
    {
        cc = (Magnus*)piscina_allocare(piscina,
            (memoriae_index)rete->exitus_numerus * magnitudo(Magnus));
    }
    per (k = ZEPHYRUM; k < rete->exitus_numerus; k++)
    {
        Magnus* ipsa = &formae[(i32)rete->exitus[k] * latus];

        per (t = ZEPHYRUM; t < rete->introitus; t++)
        {
            Magnus v = magnus_transcribe(ipsa[t], piscina);

            matrix_pone(&matrix_effectus, k, t, &v);
        }
        cc[k] = magnus_transcribe(ipsa[rete->introitus], piscina);
    }
    piscina_destruere(temporaria);
    *m = matrix_effectus;
    *c = cc;
    redde VERUM;
}


/* ==================================================
 * Compositio
 * ================================================== */

b32
nervus_componere (
    constans Nervus*  a,
    constans Nervus*  b,
            Piscina*  piscina,
             Nervus** exitus)
{
    Piscina* temporaria;
     Nervus* rete;
        s32* mappa;      /* registra b -> registra compositi */
        s32* fontes;
        s32* exitus_novi;
        i32  k;
        i32  j;
        b32  bene = VERUM;

    si (   !nervus_validus(a) || !nervus_validus(b) || piscina == NIHIL
        || exitus == NIHIL || a->exitus_numerus != b->introitus)
    {
        redde FALSUM;
    }
    si (a->introitus + a->numerus + b->numerus > NERVUS_REGISTRA_MAXIMA)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("nervus_componere", IV
        * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    mappa = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(b->introitus + b->numerus
            + I) * magnitudo(s32));
    fontes = (s32*)piscina_allocare(temporaria,
        (memoriae_index)NERVUS_TERMINI_MAXIMI * magnitudo(s32));
    exitus_novi = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(b->exitus_numerus + I) * magnitudo(s32));
    (vacuum)nervus_creare(a->introitus, piscina, &rete);
    per (k = ZEPHYRUM; k < a->numerus && bene; k++)
    {
         constans NervusInstructio* instructio = &a->instructiones[k];
                               s32  novum;

        bene = _instructionem_addere(rete, instructio->genus,
            instructio->termini, instructio->fontes,
            instructio->coefficientes, instructio->addendum, &novum);
    }
    per (k = ZEPHYRUM; k < b->introitus; k++)
    {
        mappa[k] = a->exitus[k];
    }
    per (k = ZEPHYRUM; k < b->numerus && bene; k++)
    {
         constans NervusInstructio* instructio  = &b->instructiones[k];
                               s32  novum       = -I;

        per (j = ZEPHYRUM; j < instructio->termini; j++)
        {
            fontes[j] = mappa[instructio->fontes[j]];
        }
        bene = _instructionem_addere(rete, instructio->genus,
            instructio->termini, fontes, instructio->coefficientes,
            instructio->addendum, &novum);
        mappa[b->introitus + k] = novum;
    }
    per (k = ZEPHYRUM; k < b->exitus_numerus && bene; k++)
    {
        exitus_novi[k] = mappa[b->exitus[k]];
    }
    si (bene)
    {
        bene = nervus_exitus(rete, b->exitus_numerus, exitus_novi);
    }
    piscina_destruere(temporaria);
    si (!bene)
    {
        redde FALSUM;
    }
    *exitus = rete;
    redde VERUM;
}
