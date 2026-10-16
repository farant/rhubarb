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
    exitus->nullae   = ZEPHYRUM;
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
       i32  r       = ZEPHYRUM;
       i32  nullae  = ZEPHYRUM;
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

            si (magnus_signum(summa) == ZEPHYRUM)
            {
                nullae++;
            }
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
    si (scripta != NIHIL)
    {
        scripta->nullae = nullae;
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
         constans Magnus* introitus,
                  Magnus* exitus,
    constans NervusForma* forma,
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
    /* alveus magnus: conari, ut defectus FALSUM reddat, non abortum */
    formae    = (Magnus*)piscina_conari_allocare(temporaria,
        (memoriae_index)registra * (memoriae_index)latus
        * magnitudo(Magnus));
    si (formae == NIHIL)
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
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
    /* alveus fontium ad terminos plurimos instructionis b */
    {
        i32 plurimi = I;

        per (k = ZEPHYRUM; k < b->numerus; k++)
        {
            si (b->instructiones[k].termini > plurimi)
            {
                plurimi = b->instructiones[k].termini;
            }
        }
        fontes = (s32*)piscina_allocare(temporaria,
            (memoriae_index)plurimi * magnitudo(s32));
    }
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


/* ==================================================
 * Regiones plani
 * ================================================== */

/* arcus elementarius: ab initio ad initium arcus sequentis, angulus
 * < pi. formae: registra x II, forma linearis (coefficientes x, y)
 * cuiusque registri iam computati in arcu */
nomen structura {
    NervusRadius  initium;
          Magnus* formae;
              i8* bitus;
             i32  nullae;
} ArcusPlani;

/* g(u) = g_x u_x + g_y u_y */
interior Magnus
_applicare (
     constans Magnus* g,
        NervusRadius  u,
             Piscina* piscina)
{
    redde magnus_adde(magnus_multiplica(g[ZEPHYRUM], u.x, piscina),
        magnus_multiplica(g[I], u.y, piscina), piscina);
}

/* radius primitivus ex vectore non nullo */
interior NervusRadius
_primitivus (
      Magnus  x,
      Magnus  y,
     Piscina* piscina)
{
    NervusRadius radius;
          Magnus d = magnus_divisor_communis(x, y, piscina);

    (vacuum)magnus_divide(x, d, piscina, &radius.x, NIHIL);
    (vacuum)magnus_divide(y, d, piscina, &radius.y, NIHIL);
    redde radius;
}

interior NervusRadius
_radium_transcribere (
    NervusRadius  radius,
         Piscina* piscina)
{
    NervusRadius copia;

    copia.x = magnus_transcribe(radius.x, piscina);
    copia.y = magnus_transcribe(radius.y, piscina);
    redde copia;
}

/* arcus novus ab initio; formae registrorum 0 .. definita-1, bitus et
 * nullae ex exemplari copiantur */
interior b32
_arcum_creare (
             ArcusPlani* arcus,
           NervusRadius  initium,
    constans ArcusPlani* exemplar,
                    i32  registra,
                    i32  definita,
                    i32  octeti,
                Piscina* piscina)
{
    i32 k;

    arcus->formae = (Magnus*)piscina_conari_allocare(piscina,
        (memoriae_index)registra * II * magnitudo(Magnus));
    arcus->bitus = (i8*)piscina_conari_allocare(piscina,
        (memoriae_index)octeti);
    si (arcus->formae == NIHIL || arcus->bitus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < II * definita; k++)
    {
        arcus->formae[k] = exemplar->formae[k];
    }
    per (k = ZEPHYRUM; k < octeti; k++)
    {
        arcus->bitus[k] = exemplar->bitus[k];
    }
    arcus->initium  = initium;
    arcus->nullae   = exemplar->nullae;
    redde VERUM;
}

/* forma registri ipse = summa c_j formae[f_j] (sine addendo) */
interior vacuum
_formam_computare (
                   ArcusPlani* arcus,
    constans NervusInstructio* instructio,
                          i32  ipse,
                      Piscina* piscina)
{
    Magnus summa_x = magnus_ex_s64(ZEPHYRUM);
    Magnus summa_y = magnus_ex_s64(ZEPHYRUM);
       i32 j;

    per (j = ZEPHYRUM; j < instructio->termini; j++)
    {
        constans Magnus* fons = &arcus->formae[II
            * (i32)instructio->fontes[j]];

        summa_x = magnus_adde(summa_x, magnus_multiplica(
            instructio->coefficientes[j], fons[ZEPHYRUM], piscina),
            piscina);
        summa_y = magnus_adde(summa_y, magnus_multiplica(
            instructio->coefficientes[j], fons[I], piscina), piscina);
    }
    arcus->formae[II * ipse]      = summa_x;
    arcus->formae[II * ipse + I]  = summa_y;
}

/* ReLU r in arcu: activa -> forma manet, aliter 0; bitus r */
interior vacuum
_relu_in_arcu (
    ArcusPlani* arcus,
           i32  ipse,
           i32  r,
           b32  activa)
{
    NervusForma forma;

    si (!activa)
    {
        arcus->formae[II * ipse]      = magnus_ex_s64(ZEPHYRUM);
        arcus->formae[II * ipse + I]  = magnus_ex_s64(ZEPHYRUM);
    }
    forma.bitus = arcus->bitus;
    _bitum_ponere(&forma, r, activa);
}

interior b32
_bitus_aequales (
    constans i8* a,
    constans i8* b,
            i32  octeti)
{
    i32 k;

    per (k = ZEPHYRUM; k < octeti; k++)
    {
        si (a[k] != b[k])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* arcus post omnes instructiones (temporaria); FALSUM si memoria
 * deficit */
interior b32
_arcus_plani (
    constans Nervus*  rete,
                i32   octeti,
            Piscina*  temporaria,
         ArcusPlani** exitus,
                i32*  numerus_exitus)
{
    constans s64 axes[IV][II] = {
        { I, ZEPHYRUM }, { ZEPHYRUM, I }, { -I, ZEPHYRUM },
        { ZEPHYRUM, -I }
    };
          ArcusPlani* vetera;
          ArcusPlani* nova;
          ArcusPlani  initialis;
                 i32  numerus            = IV;
                 i32  capacitas_veterum  = VIII;
                 i32  capacitas_novorum  = VIII;
                 i32  registra           = II + rete->numerus;
                 i32  k;
                 i32  a;
                 i32  r          = ZEPHYRUM;

    vetera = (ArcusPlani*)piscina_conari_allocare(temporaria,
        (memoriae_index)capacitas_veterum * magnitudo(ArcusPlani));
    nova = (ArcusPlani*)piscina_conari_allocare(temporaria,
        (memoriae_index)capacitas_novorum * magnitudo(ArcusPlani));
    initialis.formae = (Magnus*)piscina_conari_allocare(temporaria,
        IV * magnitudo(Magnus));
    initialis.bitus = (i8*)piscina_conari_allocare(temporaria,
        (memoriae_index)octeti);
    si (   vetera == NIHIL || nova == NIHIL || initialis.formae == NIHIL
        || initialis.bitus == NIHIL)
    {
        redde FALSUM;
    }
    /* introitus: x -> (1, 0), y -> (0, 1) */
    initialis.formae[ZEPHYRUM]  = magnus_ex_s64(I);
    initialis.formae[I]         = magnus_ex_s64(ZEPHYRUM);
    initialis.formae[II]        = magnus_ex_s64(ZEPHYRUM);
    initialis.formae[III]       = magnus_ex_s64(I);
    per (k = ZEPHYRUM; k < octeti; k++)
    {
        initialis.bitus[k] = ZEPHYRUM;
    }
    initialis.nullae = ZEPHYRUM;
    per (a = ZEPHYRUM; a < IV; a++)
    {
        NervusRadius axis;

        axis.x = magnus_ex_s64(axes[a][ZEPHYRUM]);
        axis.y = magnus_ex_s64(axes[a][I]);
        si (!_arcum_creare(&vetera[a], axis, &initialis, registra, II,
            octeti, temporaria))
        {
            redde FALSUM;
        }
    }
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
        constans NervusInstructio* instructio =
            &rete->instructiones[k];
                               i32 ipse  = II + k;
                               i32 m     = ZEPHYRUM;

        si (instructio->genus == NERVUS_AFFINIS)
        {
            per (a = ZEPHYRUM; a < numerus; a++)
            {
                _formam_computare(&vetera[a], instructio, ipse,
                    temporaria);
            }
            perge;
        }
        /* quisque arcus in duos ad summum */
        si (II * numerus > capacitas_novorum)
        {
            capacitas_novorum = II * numerus;
            nova = (ArcusPlani*)piscina_conari_allocare(temporaria,
                (memoriae_index)capacitas_novorum
                * magnitudo(ArcusPlani));
            si (nova == NIHIL)
            {
                redde FALSUM;
            }
        }
        per (a = ZEPHYRUM; a < numerus; a++)
        {
              ArcusPlani* arcus  = &vetera[a];
            NervusRadius  v      = vetera[(a + I) % numerus].initium;
                  Magnus  ad_initium;
                  Magnus  ad_finem;
                     s32  su;
                     s32  sv;

            _formam_computare(arcus, instructio, ipse, temporaria);
            ad_initium  = _applicare(&arcus->formae[II * ipse],
                arcus->initium,
                temporaria);
            ad_finem = _applicare(&arcus->formae[II * ipse], v,
                temporaria);
            su = magnus_signum(ad_initium);
            sv = magnus_signum(ad_finem);
            si (su * sv < ZEPHYRUM)
            {
                /* radius zero: |g(v)| u + |g(u)| v, intra arcum */
                      Magnus au = magnus_absolutum(ad_initium,
                          temporaria);
                      Magnus av = magnus_absolutum(ad_finem,
                          temporaria);
                NervusRadius z;

                z = _primitivus(
                    magnus_adde(magnus_multiplica(av, arcus->initium.x,
                    temporaria), magnus_multiplica(au, v.x, temporaria),
                    temporaria),
                    magnus_adde(magnus_multiplica(av, arcus->initium.y,
                    temporaria), magnus_multiplica(au, v.y, temporaria),
                    temporaria), temporaria);
                si (!_arcum_creare(&nova[m + I], z, arcus, registra,
                    ipse + I, octeti, temporaria))
                {
                    redde FALSUM;
                }
                nova[m] = *arcus;
                _relu_in_arcu(&nova[m], ipse, r, su > ZEPHYRUM);
                _relu_in_arcu(&nova[m + I], ipse, r, sv > ZEPHYRUM);
                m += II;
            }
            alioquin
            {
                si (su == ZEPHYRUM && sv == ZEPHYRUM)
                {
                    arcus->nullae++;
                }
                nova[m] = *arcus;
                _relu_in_arcu(&nova[m], ipse, r, su + sv > ZEPHYRUM);
                m++;
            }
        }
        {
            ArcusPlani* permutata = vetera;
                   i32  capacitas = capacitas_veterum;

            vetera             = nova;
            nova               = permutata;
            capacitas_veterum  = capacitas_novorum;
            capacitas_novorum  = capacitas;
            numerus            = m;
        }
        r++;
    }
    *exitus          = vetera;
    *numerus_exitus  = numerus;
    redde VERUM;
}

b32
nervus_regiones_plani (
    constans Nervus* rete,
            Piscina* piscina,
       NervusPlanum* exitus)
{
         Piscina* temporaria;
      ArcusPlani* arcus;
    NervusSector* regiones;
             i32* muri;
             i32  numerus_arcuum;
             i32  numerus_murorum  = ZEPHYRUM;
             i32  numerus;
             i32  octeti;
             i32  frusta;
             i32  k;
             i32  a;
             i32  o;

    si (   !nervus_validus(rete) || rete->introitus != II
        || piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
        si (magnus_signum(rete->instructiones[k].addendum) != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    octeti = (rete->relu + VII) / VIII;
    si (octeti == ZEPHYRUM)
    {
        octeti = I;
    }
    temporaria = piscina_generare_dynamicum("nervus_regiones_plani",
        IV * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    si (!_arcus_plani(rete, octeti, temporaria, &arcus,
        &numerus_arcuum))
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
    /* murus ante arcum a: forma arcus prioris differt */
    muri = (i32*)piscina_conari_allocare(temporaria,
        (memoriae_index)numerus_arcuum * magnitudo(i32));
    si (muri == NIHIL)
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
    per (a = ZEPHYRUM; a < numerus_arcuum; a++)
    {
        i32 prior = (a + numerus_arcuum - I) % numerus_arcuum;

        si (!_bitus_aequales(arcus[prior].bitus, arcus[a].bitus,
            octeti))
        {
            muri[numerus_murorum++] = a;
        }
    }
    /* murus unus impossibilis (forma circum rediret); nulli: sector
     * unus ab arcu 0, cuius initium (1, 0) est */
    numerus = numerus_murorum > ZEPHYRUM ? numerus_murorum : I;
    si (numerus_murorum == ZEPHYRUM)
    {
        muri[ZEPHYRUM] = ZEPHYRUM;
    }
    regiones = (NervusSector*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(NervusSector));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        NervusSector* sector = &regiones[k];
          ArcusPlani* primus = &arcus[muri[k]];
        NervusRadius  sequens  = arcus[(muri[k] + I)
                                 % numerus_arcuum].initium;
                 i8* bitus;

        sector->initium = _radium_transcribere(primus->initium,
            piscina);
        sector->finis = _radium_transcribere(arcus[muri[(k + I)
            % numerus]].initium, piscina);
        /* arcus primus < pi: u + v intra eum */
        sector->medius = _radium_transcribere(_primitivus(
            magnus_adde(primus->initium.x, sequens.x, temporaria),
            magnus_adde(primus->initium.y, sequens.y, temporaria),
            temporaria), piscina);
        bitus = (i8*)piscina_allocare(piscina, (memoriae_index)octeti);
        per (o = ZEPHYRUM; o < octeti; o++)
        {
            bitus[o] = primus->bitus[o];
        }
        sector->forma.numerus  = rete->relu;
        sector->forma.bitus    = bitus;
        sector->forma.nullae   = primus->nullae;
        si (!matrix_nulla(&ANULUS_INTEGRORUM, rete->exitus_numerus, II,
            piscina, &sector->m))
        {
            piscina_destruere(temporaria);
            redde FALSUM;
        }
        per (o = ZEPHYRUM; o < rete->exitus_numerus; o++)
        {
            constans Magnus* forma;
                     Magnus  valor_x;
                     Magnus  valor_y;

            forma    = &primus->formae[II * (i32)rete->exitus[o]];
            valor_x  = magnus_transcribe(forma[ZEPHYRUM], piscina);
            valor_y  = magnus_transcribe(forma[I], piscina);
            matrix_pone(&sector->m, o, ZEPHYRUM, &valor_x);
            matrix_pone(&sector->m, o, I, &valor_y);
        }
    }
    piscina_destruere(temporaria);
    /* frusta: regiones contiguae matrice eadem; cursus ultimus cum
     * primo coniungitur si matrix eadem (circulus) */
    regiones[ZEPHYRUM].frustum = ZEPHYRUM;
    per (k = I; k < numerus; k++)
    {
        regiones[k].frustum = regiones[k - I].frustum
            + (matrix_aequalis(regiones[k].m, regiones[k - I].m)
            ? ZEPHYRUM : I);
    }
    frusta = regiones[numerus - I].frustum + I;
    si (   frusta > I
        && matrix_aequalis(regiones[numerus - I].m,
        regiones[ZEPHYRUM].m))
    {
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            si (regiones[k].frustum == frusta - I)
            {
                regiones[k].frustum = ZEPHYRUM;
            }
        }
        frusta--;
    }
    exitus->numerus   = numerus;
    exitus->regiones  = regiones;
    exitus->frusta    = frusta;
    redde VERUM;
}
