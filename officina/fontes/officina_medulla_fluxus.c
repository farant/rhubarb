/* officina_medulla_fluxus.c - analyses medullae (M5 G24)
 *
 * Vide officina_medulla_fluxus.h. Ordo operum in computare:
 *   1. successores (operanda BLOCCUS terminatoris), praedecessores;
 *   2. DFS iterativa: attingibilitas, ordo_finitorum, arcus retro;
 *   3. ansae naturales (caput -> corpus, eiusdem capitis unitae),
 *      profunditas;
 *   4. ordo dorsi: RPO, corpore ansae contiguo (regio per caput);
 *   5. vita: usus/definitio per bloccum, iteratio in postordine;
 *   6. intervalla super positiones in ordine dorsi; vocationes.
 * Functio numquam scribitur.
 */

#include "officina_medulla_fluxus.h"
#include <string.h>

#define FLUXUS_NULLUM 0xFFFFFFFF

nomen structura {
    s32  caput;
     i8* corpus;                  /* larva bloccorum (0/1) */
} FluxusAnsa;

structura MedullaFluxus {
    constans MedullaFunctio* functio;
                        s32  numerus_bloccorum;
                        s32  numerus_registrorum;
                        s32* successores;        /* II per bloccum */
                        s32* numerus_successorum;
                        s32* praedecessores;     /* contigue */
                        s32* praedecessores_initium; /* n + 1 */
                        b32* attingibilis;
                        s32* ordo;
                        s32  numerus_ordinis;
                        s32* profunditas;
                        i32* plani;              /* per k in ordine */
                        i32  numerus_instructionum;
                        i32* initium;            /* per registrum */
                        i32* finis;
                        i32* vocationes;         /* positiones usus */
                        i32  numerus_vocationum;
};


    /* ==================================================
     * Adiumenta
     * ================================================== */

interior vacuum*
_allocare (
           Piscina* piscina,
    memoriae_index  numerus,
    memoriae_index  magnitudo_elementi)
{
    vacuum* p;

    p = piscina_allocare(piscina,
        (numerus == ZEPHYRUM ? I : numerus) * magnitudo_elementi);
    si (p != NIHIL)
    {
        memset(p, ZEPHYRUM,
            (numerus == ZEPHYRUM ? I : numerus) * magnitudo_elementi);
    }
    redde p;
}

interior constans MedullaBloccus*
_bloccus (
    constans MedullaFunctio* functio,
                        s32  index)
{
    redde (constans MedullaBloccus*)xar_obtinere(functio->blocci,
        (i32)index);
}

interior constans MedullaInstructio*
_instructio (
    constans MedullaBloccus* bloccus,
                        i32  index)
{
    redde (constans MedullaInstructio*)xar_obtinere(
        bloccus->instructiones, index);
}

interior b32
_bitum (
    constans i32* vector,
             s32  index)
{
    redde (vector[index >> V] & ((i32)I << (index & 0x1F)))
        != ZEPHYRUM;
}

interior vacuum
_bitum_ponere (
    i32* vector,
    s32  index)
{
    vector[index >> V] |= (i32)I << (index & 0x1F);
}

/* registra quae instructio legit: operanda a, b, c (REGISTRUM) et
 * argumenta vocationis in stiva. Reddit numerum in lecta (VI +
 * argumenta summum; capacitas a vocante data). */
interior s32
_lectiones (
       constans MedullaFunctio* functio,
    constans MedullaInstructio* instructio,
                           s32* lecta,
                           s32  capacitas)
{
    s32 numerus;
    s32 k;

    numerus = ZEPHYRUM;
    si (   instructio->a.genus == MEDULLA_OPERANDUM_REGISTRUM
        && numerus < capacitas)
    {
        lecta[numerus++] = instructio->a.datum.index;
    }
    si (   instructio->b.genus == MEDULLA_OPERANDUM_REGISTRUM
        && numerus < capacitas)
    {
        lecta[numerus++] = instructio->b.datum.index;
    }
    si (   instructio->c.genus == MEDULLA_OPERANDUM_REGISTRUM
        && numerus < capacitas)
    {
        lecta[numerus++] = instructio->c.datum.index;
    }
    si (   instructio->op          == MEDULLA_OP_VOCARE
        && instructio->extra_index >= ZEPHYRUM)
    {
        per (k = ZEPHYRUM; k < instructio->extra_numerus; k++)
        {
            constans MedullaOperandum* argumentum;

            argumentum = (constans MedullaOperandum*)xar_obtinere(
                functio->operanda,
                (i32)(instructio->extra_index + k));
            si (   argumentum        != NIHIL
                && argumentum->genus == MEDULLA_OPERANDUM_REGISTRUM
                && numerus < capacitas)
            {
                lecta[numerus++] = argumentum->datum.index;
            }
        }
    }
    redde numerus;
}

interior s32
_lectionum_capacitas (
    constans MedullaInstructio* instructio)
{
    redde III + ((instructio->op == MEDULLA_OP_VOCARE
        && instructio->extra_numerus > ZEPHYRUM)
        ? instructio->extra_numerus : ZEPHYRUM);
}


    /* ==================================================
     * 1. Successores et praedecessores
     * ================================================== */

interior vacuum
_successores (
    MedullaFluxus* f)
{
    s32 b;

    per (b = ZEPHYRUM; b < f->numerus_bloccorum; b++)
    {
           constans MedullaBloccus* bloccus;
        constans MedullaInstructio* ultima;
                               i32  numerus;

        bloccus = _bloccus(f->functio, b);
        numerus = xar_numerus(bloccus->instructiones);
        si (numerus == ZEPHYRUM)
        {
            perge;
        }
        ultima = _instructio(bloccus, numerus - I);
        si (ultima->a.genus == MEDULLA_OPERANDUM_BLOCCUS)
        {
            f->successores[b * II + f->numerus_successorum[b]++] =
                ultima->a.datum.index;
        }
        si (ultima->b.genus == MEDULLA_OPERANDUM_BLOCCUS)
        {
            f->successores[b * II + f->numerus_successorum[b]++] =
                ultima->b.datum.index;
        }
        si (   ultima->c.genus == MEDULLA_OPERANDUM_BLOCCUS
            && f->numerus_successorum[b] < II)
        {
            f->successores[b * II + f->numerus_successorum[b]++] =
                ultima->c.datum.index;
        }
    }
}

/* ordine fontis crescente, sine duplicibus (ramus ad idem bis) */
interior b32
_praedecessores (
    MedullaFluxus* f,
          Piscina* piscina)
{
    s32* numeri;
    s32* cursores;
    s32  b;
    s32  k;
    s32  summa;

    numeri = (s32*)_allocare(piscina,
        (memoriae_index)f->numerus_bloccorum, magnitudo(s32));
    cursores = (s32*)_allocare(piscina,
        (memoriae_index)f->numerus_bloccorum, magnitudo(s32));
    f->praedecessores_initium = (s32*)_allocare(piscina,
        (memoriae_index)f->numerus_bloccorum + I, magnitudo(s32));
    si (   numeri                    == NIHIL || cursores == NIHIL
        || f->praedecessores_initium == NIHIL)
    {
        redde FALSUM;
    }
    per (b = ZEPHYRUM; b < f->numerus_bloccorum; b++)
    {
        per (k = ZEPHYRUM; k < f->numerus_successorum[b]; k++)
        {
            s32 s;

            s = f->successores[b * II + k];
            si (k == I && s == f->successores[b * II])
            {
                perge;
            }
            numeri[s]++;
        }
    }
    summa = ZEPHYRUM;
    per (b = ZEPHYRUM; b < f->numerus_bloccorum; b++)
    {
        f->praedecessores_initium[b]  = summa;
        cursores[b]                   = summa;
        summa                         += numeri[b];
    }
    f->praedecessores_initium[f->numerus_bloccorum] = summa;
    f->praedecessores = (s32*)_allocare(piscina,
        (memoriae_index)summa, magnitudo(s32));
    si (f->praedecessores == NIHIL)
    {
        redde FALSUM;
    }
    per (b = ZEPHYRUM; b < f->numerus_bloccorum; b++)
    {
        per (k = ZEPHYRUM; k < f->numerus_successorum[b]; k++)
        {
            s32 s;

            s = f->successores[b * II + k];
            si (k == I && s == f->successores[b * II])
            {
                perge;
            }
            f->praedecessores[cursores[s]++] = b;
        }
    }
    redde VERUM;
}


    /* ==================================================
     * 2-3. DFS, ansae, profunditas
     * ================================================== */

/* DFS iterativa ex bloccus 0 (successores ordine b, c). ordo_finitorum
 * implet; arcus retro (u -> caput in stiva) in ansas convertit. */
interior b32
_percurrere (
    MedullaFluxus* f,
          Piscina* piscina,
              s32* ordo_finitorum,
              s32* numerus_finitorum,
       FluxusAnsa* ansae,
              s32* numerus_ansarum)
{
    s32* stiva;
    s32* proximus;
     i8* status;                  /* 0 novus, 1 in stiva, 2 finitus */
    s32  altitudo;
    s32  n;

    n        = f->numerus_bloccorum;
    stiva    = (s32*)_allocare(piscina, (memoriae_index)n,
        magnitudo(s32));
    proximus = (s32*)_allocare(piscina, (memoriae_index)n,
        magnitudo(s32));
    status   = (i8*)_allocare(piscina, (memoriae_index)n, I);
    si (stiva == NIHIL || proximus == NIHIL || status == NIHIL)
    {
        redde FALSUM;
    }
    *numerus_finitorum  = ZEPHYRUM;
    *numerus_ansarum    = ZEPHYRUM;
    altitudo            = ZEPHYRUM;
    stiva[altitudo++]   = ZEPHYRUM;
    status[0]           = I;
    f->attingibilis[0]  = VERUM;
    dum (altitudo > ZEPHYRUM)
    {
        s32 u;

        u = stiva[altitudo - I];
        si (proximus[u] < f->numerus_successorum[u])
        {
            s32 v;

            v = f->successores[u * II + proximus[u]];
            proximus[u]++;
            si (v < ZEPHYRUM || v >= n)
            {
                perge;
            }
            si (status[v] == ZEPHYRUM)
            {
                status[v]           = I;
                f->attingibilis[v]  = VERUM;
                stiva[altitudo++]   = v;
            }
            alioquin si (status[v] == I)
            {
                /* arcus retro u -> v: ansa capitis v */
                s32 a;

                per (a = ZEPHYRUM; a < *numerus_ansarum; a++)
                {
                    si (ansae[a].caput == v)
                    {
                        frange;
                    }
                }
                si (a == *numerus_ansarum)
                {
                    ansae[a].caput  = v;
                    ansae[a].corpus = (i8*)_allocare(piscina,
                        (memoriae_index)n, I);
                    si (ansae[a].corpus == NIHIL)
                    {
                        redde FALSUM;
                    }
                    ansae[a].corpus[v] = I;
                    (*numerus_ansarum)++;
                }
                /* corpus: ex u retro per praedecessores usque ad
                 * caput (stiva implendi = stiva DFS iam vacua non
                 * est; tabula propria) */
                {
                    s32* opus;
                    s32  numerus_operis;

                    opus = (s32*)_allocare(piscina, (memoriae_index)n,
                        magnitudo(s32));
                    si (opus == NIHIL)
                    {
                        redde FALSUM;
                    }
                    numerus_operis = ZEPHYRUM;
                    si (ansae[a].corpus[u] == ZEPHYRUM)
                    {
                        ansae[a].corpus[u]      = I;
                        opus[numerus_operis++]  = u;
                    }
                    dum (numerus_operis > ZEPHYRUM)
                    {
                        s32 w;
                        s32 p;

                        w = opus[--numerus_operis];
                        per (p = f->praedecessores_initium[w];
                             p < f->praedecessores_initium[w + I]; p++)
                        {
                            s32 x;

                            x = f->praedecessores[p];
                            si (ansae[a].corpus[x] == ZEPHYRUM)
                            {
                                ansae[a].corpus[x]      = I;
                                opus[numerus_operis++]  = x;
                            }
                        }
                    }
                }
            }
        }
        alioquin
        {
            status[u]                               = II;
            ordo_finitorum[(*numerus_finitorum)++]  = u;
            altitudo--;
        }
    }
    redde VERUM;
}


    /* ==================================================
     * 4. Ordo dorsi
     * ================================================== */

interior b32
_regionem_emittere (
          MedullaFluxus* f,
                Piscina* piscina,
    constans FluxusAnsa* ansae,
                    s32  numerus_ansarum,
                    s32  caput,
            constans i8* larva,
                     i8* emissi,
           constans s32* ordo_inversus,
                    s32  numerus_inversi)
{
    s32 k;

    per (k = ZEPHYRUM; k < numerus_inversi; k++)
    {
        s32 b;
        s32 a;

        b = ordo_inversus[k];
        si (larva[b] == ZEPHYRUM || emissi[b] != ZEPHYRUM)
        {
            perge;
        }
        a = -I;
        si (b != caput)
        {
            s32 j;

            per (j = ZEPHYRUM; j < numerus_ansarum; j++)
            {
                si (ansae[j].caput == b)
                {
                    a = j;
                    frange;
                }
            }
        }
        si (a >= ZEPHYRUM)
        {
             i8* sub;
            s32  i;

            sub = (i8*)_allocare(piscina,
                (memoriae_index)f->numerus_bloccorum, I);
            si (sub == NIHIL)
            {
                redde FALSUM;
            }
            per (i = ZEPHYRUM; i < f->numerus_bloccorum; i++)
            {
                sub[i] = (i8)(ansae[a].corpus[i] && larva[i]);
            }
            si (!_regionem_emittere(f, piscina, ansae, numerus_ansarum,
                    b, sub, emissi, ordo_inversus, numerus_inversi))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            f->ordo[f->numerus_ordinis++]  = b;
            emissi[b]                      = I;
        }
    }
    redde VERUM;
}


    /* ==================================================
     * 5-6. Vita et intervalla
     * ================================================== */

interior vacuum
_extendere (
    MedullaFluxus* f,
              s32  r,
              i32  positio)
{
    si (r < ZEPHYRUM || r >= f->numerus_registrorum)
    {
        redde;
    }
    si (f->initium[r] == FLUXUS_NULLUM)
    {
        f->initium[r]  = positio;
        f->finis[r]    = positio;
        redde;
    }
    si (positio < f->initium[r])
    {
        f->initium[r] = positio;
    }
    si (positio > f->finis[r])
    {
        f->finis[r] = positio;
    }
}

interior b32
_vitam_computare (
     MedullaFluxus* f,
           Piscina* piscina,
      constans s32* ordo_finitorum,
               s32  numerus_finitorum)
{
    s32  verba;
    i32* usus;
    i32* definitio;
    i32* vivi_in;
    i32* vivi_ex;
    s32* lecta;
    s32  capacitas;
    s32  n;
    s32  b;
    b32  mutatum;

    n      = f->numerus_bloccorum;
    verba  = (f->numerus_registrorum + XXXI) / XXXII;
    si (verba == ZEPHYRUM)
    {
        verba = I;
    }
    usus      = (i32*)_allocare(piscina,
        (memoriae_index)(n * verba), magnitudo(i32));
    definitio = (i32*)_allocare(piscina,
        (memoriae_index)(n * verba), magnitudo(i32));
    vivi_in   = (i32*)_allocare(piscina,
        (memoriae_index)(n * verba), magnitudo(i32));
    vivi_ex   = (i32*)_allocare(piscina,
        (memoriae_index)(n * verba), magnitudo(i32));
    capacitas = CCLVI;
    lecta     = (s32*)_allocare(piscina, (memoriae_index)capacitas,
        magnitudo(s32));
    si (   usus    == NIHIL || definitio == NIHIL || vivi_in == NIHIL
        || vivi_ex == NIHIL || lecta == NIHIL)
    {
        redde FALSUM;
    }

    /* usus (lectum ante definitionem in bloccum) et definitio */
    per (b = ZEPHYRUM; b < n; b++)
    {
        constans MedullaBloccus* bloccus;
                            i32  k;

        si (!f->attingibilis[b])
        {
            perge;
        }
        bloccus = _bloccus(f->functio, b);
        per (k = ZEPHYRUM; k < xar_numerus(bloccus->instructiones);
             k++)
        {
            constans MedullaInstructio* instructio;
                                   s32  numerus;
                                   s32  j;

            instructio = _instructio(bloccus, k);
            si (_lectionum_capacitas(instructio) > capacitas)
            {
                capacitas = _lectionum_capacitas(instructio);
                lecta     = (s32*)_allocare(piscina,
                    (memoriae_index)capacitas, magnitudo(s32));
                si (lecta == NIHIL)
                {
                    redde FALSUM;
                }
            }
            numerus = _lectiones(f->functio, instructio, lecta,
                capacitas);
            per (j = ZEPHYRUM; j < numerus; j++)
            {
                s32 r;

                r = lecta[j];
                si (   r >= ZEPHYRUM && r < f->numerus_registrorum
                    && !_bitum(&definitio[b * verba], r))
                {
                    _bitum_ponere(&usus[b * verba], r);
                }
            }
            si (   instructio->destinatio >= ZEPHYRUM
                && instructio->destinatio < f->numerus_registrorum)
            {
                _bitum_ponere(&definitio[b * verba],
                    instructio->destinatio);
            }
        }
    }

    /* iteratio in postordine usque ad punctum fixum */
    fac
    {
        s32 k;

        mutatum = FALSUM;
        per (k = ZEPHYRUM; k < numerus_finitorum; k++)
        {
            s32 j;
            s32 w;

            b = ordo_finitorum[k];
            per (j = ZEPHYRUM; j < f->numerus_successorum[b]; j++)
            {
                s32 s;

                s = f->successores[b * II + j];
                si (s < ZEPHYRUM || s >= n || !f->attingibilis[s])
                {
                    perge;
                }
                per (w = ZEPHYRUM; w < verba; w++)
                {
                    vivi_ex[b * verba + w] |= vivi_in[s * verba + w];
                }
            }
            per (w = ZEPHYRUM; w < verba; w++)
            {
                i32 novum;

                novum = usus[b * verba + w]
                    | (vivi_ex[b * verba + w]
                       & ~definitio[b * verba + w]);
                si (novum != vivi_in[b * verba + w])
                {
                    vivi_in[b * verba + w]  = novum;
                    mutatum                 = VERUM;
                }
            }
        }
    }
    dum (mutatum);

    /* intervalla in ordine dorsi */
    {
        constans MedullaParametrum* parametrum;
                               i32* basis_plana;
                               i32  summa;
                               i32  positio_k;
                               s32  o;
                               i32  i;

        basis_plana = (i32*)_allocare(piscina, (memoriae_index)n,
            magnitudo(i32));
        si (basis_plana == NIHIL)
        {
            redde FALSUM;
        }
        summa = ZEPHYRUM;
        per (b = ZEPHYRUM; b < n; b++)
        {
            basis_plana[b] = summa;
            summa += xar_numerus(_bloccus(f->functio,
                b)->instructiones);
        }
        f->plani = (i32*)_allocare(piscina, (memoriae_index)summa,
            magnitudo(i32));
        f->vocationes = (i32*)_allocare(piscina,
            (memoriae_index)summa, magnitudo(i32));
        si (f->plani == NIHIL || f->vocationes == NIHIL)
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < xar_numerus(f->functio->parametra); i++)
        {
            parametrum = (constans MedullaParametrum*)xar_obtinere(
                f->functio->parametra, i);
            _extendere(f, parametrum->index, ZEPHYRUM);
        }
        positio_k = ZEPHYRUM;
        per (o = ZEPHYRUM; o < f->numerus_ordinis; o++)
        {
            constans MedullaBloccus* bloccus;
                                i32  numerus;
                                i32  k;
                                s32  r;

            b        = f->ordo[o];
            bloccus  = _bloccus(f->functio, b);
            numerus  = xar_numerus(bloccus->instructiones);
            si (numerus == ZEPHYRUM)
            {
                perge;
            }
            per (r = ZEPHYRUM; r < f->numerus_registrorum; r++)
            {
                si (_bitum(&vivi_in[b * verba], r))
                {
                    _extendere(f, r, positio_k * II);
                }
                si (_bitum(&vivi_ex[b * verba], r))
                {
                    _extendere(f, r, (positio_k + numerus) * II - I);
                }
            }
            per (k = ZEPHYRUM; k < numerus; k++)
            {
                constans MedullaInstructio* instructio;
                                       s32  lectorum;
                                       s32  j;

                instructio = _instructio(bloccus, k);
                lectorum   = _lectiones(f->functio, instructio, lecta,
                    capacitas);
                per (j = ZEPHYRUM; j < lectorum; j++)
                {
                    _extendere(f, lecta[j], positio_k * II);
                }
                si (instructio->destinatio >= ZEPHYRUM)
                {
                    _extendere(f, instructio->destinatio,
                        positio_k * II + I);
                }
                si (instructio->op == MEDULLA_OP_VOCARE)
                {
                    f->vocationes[f->numerus_vocationum++] =
                        positio_k * II;
                }
                f->plani[positio_k] = basis_plana[b] + k;
                positio_k++;
            }
        }
        f->numerus_instructionum = positio_k;
    }
    redde VERUM;
}


    /* ==================================================
     * Computare
     * ================================================== */

MedullaFluxus*
medulla_fluxus_computare (
                    Piscina* piscina,
    constans MedullaFunctio* functio)
{
    MedullaFluxus* f;
              s32* ordo_finitorum;
              s32  numerus_finitorum;
       FluxusAnsa* ansae;
              s32  numerus_ansarum;
              s32* ordo_inversus;
               i8* larva;
               i8* emissi;
              s32  n;
              s32  k;
              s32  a;

    si (   piscina                      == NIHIL || functio == NIHIL
        || xar_numerus(functio->blocci) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    f = (MedullaFluxus*)_allocare(piscina, I, magnitudo(MedullaFluxus));
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    n                       = (s32)xar_numerus(functio->blocci);
    f->functio              = functio;
    f->numerus_bloccorum    = n;
    f->numerus_registrorum  = (s32)xar_numerus(functio->registra);
    f->successores = (s32*)_allocare(piscina,
        (memoriae_index)(n * II), magnitudo(s32));
    f->numerus_successorum = (s32*)_allocare(piscina,
        (memoriae_index)n, magnitudo(s32));
    f->attingibilis = (b32*)_allocare(piscina, (memoriae_index)n,
        magnitudo(b32));
    f->ordo        = (s32*)_allocare(piscina, (memoriae_index)n,
        magnitudo(s32));
    f->profunditas = (s32*)_allocare(piscina, (memoriae_index)n,
        magnitudo(s32));
    f->initium = (i32*)_allocare(piscina,
        (memoriae_index)f->numerus_registrorum, magnitudo(i32));
    f->finis   = (i32*)_allocare(piscina,
        (memoriae_index)f->numerus_registrorum, magnitudo(i32));
    ordo_finitorum = (s32*)_allocare(piscina, (memoriae_index)n,
        magnitudo(s32));
    ansae    = (FluxusAnsa*)_allocare(piscina, (memoriae_index)n,
        magnitudo(FluxusAnsa));
    ordo_inversus      = (s32*)_allocare(piscina, (memoriae_index)n,
        magnitudo(s32));
    larva   = (i8*)_allocare(piscina, (memoriae_index)n, I);
    emissi  = (i8*)_allocare(piscina, (memoriae_index)n, I);
    si (   f->successores == NIHIL || f->numerus_successorum == NIHIL
        || f->attingibilis == NIHIL || f->ordo == NIHIL
        || f->profunditas == NIHIL || f->initium == NIHIL
        || f->finis == NIHIL || ordo_finitorum == NIHIL
        || ansae == NIHIL
        || ordo_inversus == NIHIL || larva == NIHIL || emissi == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < f->numerus_registrorum; k++)
    {
        f->initium[k]  = FLUXUS_NULLUM;
        f->finis[k]    = FLUXUS_NULLUM;
    }

    _successores(f);
    si (   !_praedecessores(f, piscina)
        || !_percurrere(f, piscina, ordo_finitorum, &numerus_finitorum,
               ansae, &numerus_ansarum))
    {
        redde NIHIL;
    }
    per (a = ZEPHYRUM; a < numerus_ansarum; a++)
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (ansae[a].corpus[k] != ZEPHYRUM && f->attingibilis[k])
            {
                f->profunditas[k]++;
            }
        }
    }
    per (k = ZEPHYRUM; k < numerus_finitorum; k++)
    {
        ordo_inversus[k] = ordo_finitorum[numerus_finitorum - I
            - k];
        larva[ordo_inversus[k]] = I;
    }
    si (   !_regionem_emittere(f, piscina, ansae, numerus_ansarum, -I,
            larva, emissi, ordo_inversus, numerus_finitorum)
        || !_vitam_computare(f, piscina, ordo_finitorum,
               numerus_finitorum))
    {
        redde NIHIL;
    }
    redde f;
}


    /* ==================================================
     * Interrogationes
     * ================================================== */

s32
medulla_fluxus_praedecessores (
    constans MedullaFluxus*  f,
                       s32   bloccus,
              constans s32** indices_out)
{
    si (   f       == NIHIL || bloccus < ZEPHYRUM
        || bloccus >= f->numerus_bloccorum)
    {
        redde ZEPHYRUM;
    }
    si (indices_out != NIHIL)
    {
        *indices_out =
            &f->praedecessores[f->praedecessores_initium[bloccus]];
    }
    redde f->praedecessores_initium[bloccus + I]
        - f->praedecessores_initium[bloccus];
}

b32
medulla_fluxus_attingibilis (
    constans MedullaFluxus* f,
                       s32  bloccus)
{
    si (   f       == NIHIL || bloccus < ZEPHYRUM
        || bloccus >= f->numerus_bloccorum)
    {
        redde FALSUM;
    }
    redde f->attingibilis[bloccus];
}

constans s32*
medulla_fluxus_ordo (
    constans MedullaFluxus* f,
                       s32* numerus_out)
{
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (numerus_out != NIHIL)
    {
        *numerus_out = f->numerus_ordinis;
    }
    redde f->ordo;
}

s32
medulla_fluxus_profunditas_ansae (
    constans MedullaFluxus* f,
                       s32  bloccus)
{
    si (   f       == NIHIL || bloccus < ZEPHYRUM
        || bloccus >= f->numerus_bloccorum)
    {
        redde ZEPHYRUM;
    }
    redde f->profunditas[bloccus];
}

i32
medulla_fluxus_index_planus (
    constans MedullaFluxus* f,
                       i32  positio)
{
    si (f == NIHIL || positio / II >= f->numerus_instructionum)
    {
        redde FLUXUS_NULLUM;
    }
    redde f->plani[positio / II];
}

b32
medulla_fluxus_intervallum (
    constans MedullaFluxus* f,
                       s32  index_registri,
                       i32* initium_out,
                       i32* finis_out)
{
    si (   f == NIHIL || index_registri < ZEPHYRUM
        || index_registri >= f->numerus_registrorum
        || f->initium[index_registri] == FLUXUS_NULLUM)
    {
        redde FALSUM;
    }
    si (initium_out != NIHIL)
    {
        *initium_out = f->initium[index_registri];
    }
    si (finis_out != NIHIL)
    {
        *finis_out = f->finis[index_registri];
    }
    redde VERUM;
}

b32
medulla_fluxus_transit_vocationem (
    constans MedullaFluxus* f,
                       s32  index_registri)
{
    i32 i;
    i32 initium;
    i32 finis;

    si (!medulla_fluxus_intervallum(f, index_registri, &initium,
            &finis))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < f->numerus_vocationum; i++)
    {
        si (   initium < f->vocationes[i]
            && finis > f->vocationes[i] + I)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}
