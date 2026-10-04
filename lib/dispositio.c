/* dispositio.c - dispositio pura in cellulis (D1). Ratio et
 * divergentiae a Clay in capite; algorithmus Clay @ e6cc369
 * translatus: (1) apta a foliis sursum (Clay__CloseElement), (2)
 * mensurae per axem a radice deorsum (Clay__SizeContainersAlongAxis;
 * crescens secundum axem et contractio D2), (3) positiones
 * (Clay__CalculateFinalLayout). */

#include "dispositio.h"
#include "xar.h"
#include <string.h>

#define SINE_FINE  0x3FFFFFFF

nomen structura {
    DispositioForma forma;
                s32 parens;
                s32 primus;      /* liber primus; -1 nullus */
                s32 ultimus;
                s32 proximus;    /* frater sequens; -1 nullus */
                s32 mensura[II]; /* latitudo, altitudo */
                s32 minima[II];  /* minimum contenti per axem */
                s32 positio[II]; /* x, y */
                b32 habet_textum; /* D3: liber primus virtualis */
                s32 textus[II];   /* latitudo mensa x I */
                s32 textus_minimum[II];  /* verbum latissimum x I */
} Nodus;

structura Dispositio {
    Piscina* piscina;
        Xar* nodi;               /* Xar de Nodus */
        Xar* membra;             /* s32: liberi mutabiles (D2); -1 =
                                  * exemptus, ordo arboris servatur */
        s32 radix_primus;       /* liberi radicis implicitae */
        s32 radix_ultimus;
        s32 superficies[II];
    /* mensor textus (D3): intra computare tantum */
    DispositioMensor  mensor;
              vacuum* mensor_ctx;
};

/* Divisio cum pavimento (b > 0): C89 directionem negativae
 * implementationi relinquit */
interior s32
_pavimentum (
    s32 a,
    s32 b)
{
    redde (a >= ZEPHYRUM) ? a / b : -((-a + b - I) / b);
}

interior s32
_maximum (
    s32 a,
    s32 b)
{
    redde (a > b) ? a : b;
}

interior s32
_minimum (
    s32 a,
    s32 b)
{
    redde (a < b) ? a : b;
}

interior Nodus*
_nodus (
    constans Dispositio* d,
                    s32  index)
{
    redde (Nodus*)xar_obtinere(d->nodi, (i32)index);
}

/* Accessus per axem (0 = x, 1 = y): mensura formae */
interior constans DispositioMensura*
_mensura_formae (
    constans Nodus* n,
               s32  axis)
{
    redde (axis == ZEPHYRUM) ? &n->forma.latitudo : &n->forma.altitudo;
}

interior s32
_spatium (
    constans Nodus* n,
               s32  axis)
{
    redde (axis == ZEPHYRUM)
        ? n->forma.spatium_sinistrum + n->forma.spatium_dextrum
        : n->forma.spatium_superum + n->forma.spatium_inferum;
}

interior s32
_spatium_initii (
    constans Nodus* n,
               s32  axis)
{
    redde (axis == ZEPHYRUM) ? n->forma.spatium_sinistrum
                             : n->forma.spatium_superum;
}

interior b32
_praecidit (
    constans Nodus* n,
               s32  axis)
{
    redde (axis == ZEPHYRUM) ? n->forma.praecidere_x
                             : n->forma.praecidere_y;
}

interior DispositioAllineatio
_allineatio (
    constans Nodus* n,
               s32  axis)
{
    redde (axis == ZEPHYRUM) ? n->forma.allineatio_x
                             : n->forma.allineatio_y;
}

/* axis directionis: linea -> x, columna -> y */
interior s32
_axis_directionis (
    constans Nodus* n)
{
    redde (n->forma.directio == DISPOSITIO_COLUMNA) ? I : ZEPHYRUM;
}

/* minimum et maximum formae (FIXA: valor utrumque) */
interior vacuum
_limites (
    constans DispositioMensura* m,
                           s32* minimum,
                           s32* maximum)
{
    si (m->genus == DISPOSITIO_FIXA)
    {
        *minimum = m->valor;
        *maximum = m->valor;
        redde;
    }
    *minimum = m->minimum;
    *maximum = (m->maximum <= ZEPHYRUM) ? SINE_FINE : m->maximum;
}

Dispositio*
dispositio_creare (
    Piscina* piscina)
{
    Dispositio* d;

    si (!piscina)
    {
        redde NIHIL;
    }
    d = (Dispositio*)piscina_allocare(piscina, magnitudo(Dispositio));
    si (!d)
    {
        redde NIHIL;
    }
    memset(d, ZEPHYRUM, magnitudo(Dispositio));
    d->piscina        = piscina;
    d->nodi           = xar_creare(piscina, (i32)magnitudo(Nodus));
    d->membra         = xar_creare(piscina, (i32)magnitudo(s32));
    d->radix_primus   = -I;
    d->radix_ultimus  = -I;
    redde (d->nodi && d->membra) ? d : NIHIL;
}

vacuum
dispositio_vacare (
    Dispositio* d)
{
    si (!d)
    {
        redde;
    }
    xar_vacare(d->nodi);
    d->radix_primus   = -I;
    d->radix_ultimus  = -I;
}

vacuum
dispositio_formam_initiare (
    DispositioForma* forma)
{
    si (!forma)
    {
        redde;
    }
    memset(forma, ZEPHYRUM, magnitudo(DispositioForma));
    forma->latitudo.genus  = DISPOSITIO_APTA;
    forma->altitudo.genus  = DISPOSITIO_APTA;
    forma->directio        = DISPOSITIO_LINEA;
    forma->allineatio_x    = DISPOSITIO_INITIUM;
    forma->allineatio_y    = DISPOSITIO_INITIUM;
}

s32
dispositio_addere (
                  Dispositio* d,
                         s32  parens,
    constans DispositioForma* forma)
{
    Nodus* n;
      s32  index;

    si (   !d || !forma || parens < -I
        || parens >= (s32)xar_numerus(d->nodi))
    {
        redde -I;
    }
    n = (Nodus*)xar_addere(d->nodi);
    si (!n)
    {
        redde -I;
    }
    memset(n, ZEPHYRUM, magnitudo(Nodus));
    n->forma     = *forma;
    n->parens    = parens;
    n->primus    = -I;
    n->ultimus   = -I;
    n->proximus  = -I;
    index        = (s32)xar_numerus(d->nodi) - I;
    si (parens < ZEPHYRUM)
    {
        si (d->radix_ultimus >= ZEPHYRUM)
        {
            _nodus(d, d->radix_ultimus)->proximus = index;
        }
        alioquin
        {
            d->radix_primus = index;
        }
        d->radix_ultimus = index;
    }
    alioquin
    {
        Nodus* p = _nodus(d, parens);

        si (p->ultimus >= ZEPHYRUM)
        {
            _nodus(d, p->ultimus)->proximus = index;
        }
        alioquin
        {
            p->primus = index;
        }
        p->ultimus = index;
    }
    redde index;
}


/* ==================================================
 * (0) Textus (D3): liber primus VIRTUALIS mensurae fixae (latitudo
 *     mensa x I), ut Clay textum liberum primum aperit (sine
 *     involutione). Minimum = verbum latissimum (spatiis divisum) - ut
 *     Clay, contractio parentis infra textum totum licet.
 * ================================================== */

interior vacuum
_textum_metiri (
    Dispositio* d,
         Nodus* n)
{
    chorda t = n->forma.textus;
       s32 i;
       s32 initium     = ZEPHYRUM;
       s32 latissimum  = ZEPHYRUM;

    n->habet_textum = (t.mensura > ZEPHYRUM && d->mensor) ? VERUM
                                                          : FALSUM;
    n->textus[0]          = ZEPHYRUM;
    n->textus[1]          = ZEPHYRUM;
    n->textus_minimum[0]  = ZEPHYRUM;
    n->textus_minimum[1]  = ZEPHYRUM;
    si (!n->habet_textum)
    {
        redde;
    }
    n->textus[0] = d->mensor(t, d->mensor_ctx);
    n->textus[1] = I;
    per (i = ZEPHYRUM; i <= (s32)t.mensura; i++)
    {
        si (i == (s32)t.mensura || t.datum[i] == ' ')
        {
            si (i > initium)
            {
                chorda verbum;

                verbum.datum    = t.datum + initium;
                verbum.mensura  = (i32)(i - initium);
                latissimum = _maximum(latissimum,
                    d->mensor(verbum, d->mensor_ctx));
            }
            initium = i + I;
        }
    }
    n->textus_minimum[0] = latissimum;
    n->textus_minimum[1] = I;
}


/* ==================================================
 * (1) Apta: a foliis sursum
 * ================================================== */

interior vacuum
_aptare (
    Dispositio* d,
           s32  index)
{
    Nodus* n       = _nodus(d, index);
      s32  a       = _axis_directionis(n);   /* secundum axem */
      s32  t       = I - a;                  /* transversus */
      s32  liberi  = ZEPHYRUM;
      s32  c;
      s32  axis;

    n->mensura[a]  = _spatium(n, a);
    n->minima[a]   = _spatium(n, a);
    n->mensura[t]  = ZEPHYRUM;
    n->minima[t]   = ZEPHYRUM;
    si (n->habet_textum)
    {
        n->mensura[a] += n->textus[a];
        n->mensura[t] = n->textus[t] + _spatium(n, t);
        si (!_praecidit(n, a))
        {
            n->minima[a] += n->textus_minimum[a];
        }
        si (!_praecidit(n, t))
        {
            n->minima[t] = n->textus_minimum[t] + _spatium(n, t);
        }
        liberi++;
    }
    per (c = n->primus; c >= ZEPHYRUM; c = _nodus(d, c)->proximus)
    {
        Nodus* l = _nodus(d, c);

        n->mensura[a] += l->mensura[a];
        n->mensura[t]  = _maximum(n->mensura[t],
            l->mensura[t] + _spatium(n, t));
        si (!_praecidit(n, a))
        {
            n->minima[a] += l->minima[a];
        }
        si (!_praecidit(n, t))
        {
            n->minima[t] = _maximum(n->minima[t],
                l->minima[t] + _spatium(n, t));
        }
        liberi++;
    }
    si (liberi > I)
    {
        n->mensura[a] += (liberi - I) * n->forma.intervallum;
        si (!_praecidit(n, a))
        {
            n->minima[a] += (liberi - I) * n->forma.intervallum;
        }
    }
    per (axis = ZEPHYRUM; axis < II; axis++)
    {
        constans DispositioMensura* m = _mensura_formae(n, axis);
                               s32  mi;
                               s32  ma;

        si (m->genus == DISPOSITIO_PARS)
        {
            n->mensura[axis] = ZEPHYRUM;
            perge;
        }
        _limites(m, &mi, &ma);
        n->mensura[axis] = _minimum(_maximum(n->mensura[axis], mi), ma);
        n->minima[axis] = _minimum(_maximum(n->minima[axis], mi), ma);
    }
}


/* ==================================================
 * (2a) Crescens et contractio secundum axem (D2)
 *
 * Aequatio Clay: crescentes MINIMI primum ad minimum proximum, deinde
 * aequaliter, quisque maximo suo tectus; contractio (APTA et CRESCENS)
 * MAXIMI primum, minimo contenti pavimentata. Divisio ultima integra:
 * ORA QUAEQUE pavimentum orae exactae - membrum j (ordine arboris)
 * accipit floor((j+1)S/k) - floor(jS/k), amittit ceil((j+1)D/k) -
 * ceil(jD/k). Residuum ita aequaliter sparsum (non primis datum: ea
 * regula oras usque ad k/4 cellulas promoveret).
 * ================================================== */

interior s32
_lacunar (
    s32 a,
    s32 b)
{
    /* ceil(a / b), a >= 0, b > 0 */
    redde (a + b - I) / b;
}

/* membra: liberi generis dati (aut omnes mutabiles) ordine arboris */
interior s32
_membra_colligere (
    Dispositio* d,
           s32  primus,
           s32  axis,
           b32  solum_crescentes)
{
    s32 c;
    s32 n = ZEPHYRUM;

    xar_vacare(d->membra);
    per (c = primus; c >= ZEPHYRUM; c = _nodus(d, c)->proximus)
    {
        DispositioGenus g = _mensura_formae(_nodus(d, c), axis)->genus;

        si (g == DISPOSITIO_PARS || g == DISPOSITIO_FIXA)
        {
            perge;
        }
        si (solum_crescentes && g != DISPOSITIO_CRESCENS)
        {
            perge;
        }
        *(s32*)xar_addere(d->membra) = c;
        n++;
    }
    redde n;
}

interior s32
_membrum (
    Dispositio* d,
           s32  i)
{
    redde *(s32*)xar_obtinere(d->membra, (i32)i);
}

interior vacuum
_eximere (
    Dispositio* d,
           s32  i)
{
    *(s32*)xar_obtinere(d->membra, (i32)i) = -I;
}

interior vacuum
_crescere (
    Dispositio* d,
           s32  primus,
           s32  axis,
           s32  reliquum)
{
    s32 n = _membra_colligere(d, primus, axis, VERUM);

    dum (reliquum > ZEPHYRUM && n > ZEPHYRUM)
    {
        s32 minimum   = SINE_FINE;
        s32 secundum  = SINE_FINE;
        s32 k         = ZEPHYRUM;
        s32 j         = ZEPHYRUM;
        s32 datum     = ZEPHYRUM;
        b32 ultima;
        s32 i;

        per (i = ZEPHYRUM; i < (s32)xar_numerus(d->membra); i++)
        {
            s32 c = _membrum(d, i);

            si (c >= ZEPHYRUM)
            {
                minimum = _minimum(minimum, _nodus(d,
                    c)->mensura[axis]);
            }
        }
        per (i = ZEPHYRUM; i < (s32)xar_numerus(d->membra); i++)
        {
            s32 c = _membrum(d, i);
            s32 m;

            si (c < ZEPHYRUM)
            {
                perge;
            }
            m = _nodus(d, c)->mensura[axis];
            si (m == minimum)
            {
                k++;
            }
            alioquin si (m < secundum)
            {
                secundum = m;
            }
        }
        /* gradus integer ad secundum, aut divisio ultima */
        ultima = (secundum == SINE_FINE
            || (secundum - minimum) * k > reliquum) ? VERUM : FALSUM;
        per (i = ZEPHYRUM; i < (s32)xar_numerus(d->membra); i++)
        {
              s32  c = _membrum(d, i);
            Nodus* l;
              s32  additum;
              s32  mi;
              s32  ma;
              s32  vetus;

            si (c < ZEPHYRUM)
            {
                perge;
            }
            l = _nodus(d, c);
            si (l->mensura[axis] != minimum)
            {
                perge;
            }
            additum = ultima ? (((j + I) * reliquum) / k
                - (j * reliquum) / k) : secundum - minimum;
            j++;
            _limites(_mensura_formae(l, axis), &mi, &ma);
            vetus             = l->mensura[axis];
            l->mensura[axis]  = _minimum(vetus + additum, ma);
            si (l->mensura[axis] >= ma)
            {
                _eximere(d, i);
                n--;
            }
            datum += l->mensura[axis] - vetus;
        }
        reliquum -= datum;
        si (datum == ZEPHYRUM)
        {
            frange;   /* custodia: nihil datum (omnes tecti) */
        }
    }
}

interior vacuum
_contrahere (
    Dispositio* d,
           s32  primus,
           s32  axis,
           s32  defectus)
{
    s32 n = _membra_colligere(d, primus, axis, FALSUM);

    dum (defectus > ZEPHYRUM && n > ZEPHYRUM)
    {
        s32 maximum   = -I;
        s32 secundum  = -I;
        s32 k         = ZEPHYRUM;
        s32 j         = ZEPHYRUM;
        s32 ablatum   = ZEPHYRUM;
        b32 ultima;
        s32 i;

        per (i = ZEPHYRUM; i < (s32)xar_numerus(d->membra); i++)
        {
            s32 c = _membrum(d, i);

            si (c >= ZEPHYRUM)
            {
                maximum = _maximum(maximum, _nodus(d,
                    c)->mensura[axis]);
            }
        }
        per (i = ZEPHYRUM; i < (s32)xar_numerus(d->membra); i++)
        {
            s32 c = _membrum(d, i);
            s32 m;

            si (c < ZEPHYRUM)
            {
                perge;
            }
            m = _nodus(d, c)->mensura[axis];
            si (m == maximum)
            {
                k++;
            }
            alioquin si (m > secundum)
            {
                secundum = m;
            }
        }
        ultima = (secundum < ZEPHYRUM
            || (maximum - secundum) * k > defectus) ? VERUM : FALSUM;
        per (i = ZEPHYRUM; i < (s32)xar_numerus(d->membra); i++)
        {
              s32  c = _membrum(d, i);
            Nodus* l;
              s32  amissum;
              s32  vetus;

            si (c < ZEPHYRUM)
            {
                perge;
            }
            l = _nodus(d, c);
            si (l->mensura[axis] != maximum)
            {
                perge;
            }
            amissum = ultima ? (_lacunar((j + I) * defectus, k)
                - _lacunar(j * defectus, k)) : maximum - secundum;
            j++;
            vetus = l->mensura[axis];
            l->mensura[axis] = _maximum(vetus - amissum,
                l->minima[axis]);
            si (l->mensura[axis] <= l->minima[axis])
            {
                _eximere(d, i);
                n--;
            }
            ablatum += vetus - l->mensura[axis];
        }
        defectus -= ablatum;
        si (ablatum == ZEPHYRUM)
        {
            frange;
        }
    }
}


/* ==================================================
 * (2) Mensurae per axem: liberi parentis unius
 * ================================================== */

/* parens: nodus aut NIHIL (radix implicita: linea, mensura
 * superficiei, sine spatio) */
interior vacuum
_liberos_metiri (
    Dispositio* d,
         Nodus* parens,
           s32  primus,
           s32  axis)
{
    s32 magnitudo_parentis;
    s32 spatium_parentis;
    s32 contentum = ZEPHYRUM;
    s32 spatia_et_intervalla;
    b32 secundum;
    b32 praecidit;
    s32 intervallum;
    b32 primus_liber  = VERUM;
    s32 crescentes    = ZEPHYRUM;
    s32 c;

    si (parens)
    {
        magnitudo_parentis  = parens->mensura[axis];
        spatium_parentis    = _spatium(parens, axis);
        secundum            = (_axis_directionis(parens) == axis);
        praecidit           = _praecidit(parens, axis);
        intervallum         = parens->forma.intervallum;
    }
    alioquin
    {
        magnitudo_parentis  = d->superficies[axis];
        spatium_parentis    = ZEPHYRUM;
        secundum            = (axis == ZEPHYRUM);
        praecidit           = FALSUM;
        intervallum         = ZEPHYRUM;
    }
    spatia_et_intervalla = spatium_parentis;
    si (parens && parens->habet_textum)
    {
        /* textus: liber primus, mensura fixa */
        si (secundum)
        {
            contentum += parens->textus[axis];
        }
        alioquin
        {
            contentum = parens->textus[axis];
        }
        primus_liber = FALSUM;
    }

    per (c = primus; c >= ZEPHYRUM; c = _nodus(d, c)->proximus)
    {
                  Nodus* l = _nodus(d, c);
        DispositioGenus  g = _mensura_formae(l, axis)->genus;

        si (secundum)
        {
            contentum += (g == DISPOSITIO_PARS) ? ZEPHYRUM
                                                : l->mensura[axis];
            si (g == DISPOSITIO_CRESCENS)
            {
                crescentes++;
            }
            si (!primus_liber)
            {
                contentum             += intervallum;
                spatia_et_intervalla  += intervallum;
            }
        }
        alioquin
        {
            contentum = _maximum(l->mensura[axis], contentum);
        }
        primus_liber = FALSUM;
    }

    /* PARS: spatium parentis (minus spatiis et intervallis) */
    per (c = primus; c >= ZEPHYRUM; c = _nodus(d, c)->proximus)
    {
                             Nodus* l = _nodus(d,
                                 c);
        constans DispositioMensura* m = _mensura_formae(l, axis);

        si (m->genus == DISPOSITIO_PARS)
        {
            l->mensura[axis] = _pavimentum((magnitudo_parentis
                - spatia_et_intervalla) * m->valor, C);
            si (secundum)
            {
                contentum += l->mensura[axis];
            }
        }
    }

    si (secundum)
    {
        s32 reliquum = magnitudo_parentis - spatium_parentis
            - contentum;

        si (reliquum < ZEPHYRUM && !praecidit)
        {
            _contrahere(d, primus, axis, -reliquum);
        }
        alioquin si (reliquum > ZEPHYRUM && crescentes > ZEPHYRUM)
        {
            _crescere(d, primus, axis, reliquum);
        }
        redde;
    }

    /* transversus: APTA et CRESCENS intra spatium parentis */
    per (c = primus; c >= ZEPHYRUM; c = _nodus(d, c)->proximus)
    {
                             Nodus* l;
        constans DispositioMensura* m;
                               s32  maximum;
                               s32  mi;
                               s32  ma;

        l        = _nodus(d, c);
        m        = _mensura_formae(l, axis);
        maximum  = magnitudo_parentis - spatium_parentis;
        si (m->genus == DISPOSITIO_PARS || m->genus == DISPOSITIO_FIXA)
        {
            perge;
        }
        si (praecidit)
        {
            maximum = _maximum(maximum, contentum);
        }
        si (m->genus == DISPOSITIO_CRESCENS)
        {
            _limites(m, &mi, &ma);
            l->mensura[axis] = _minimum(maximum, ma);
        }
        l->mensura[axis] = _maximum(l->minima[axis],
            _minimum(l->mensura[axis], maximum));
    }
}


/* ==================================================
 * (3) Positiones liberorum parentis unius
 * ================================================== */

interior vacuum
_liberos_ponere (
    Dispositio* d,
         Nodus* parens,
           s32  primus)
{
                     s32 a;
                     s32 t;
                     s32 origo[II];
                     s32 interior_[II];
                     s32 initium[II];
                     s32 intervallum;
                     s32 contentum  = ZEPHYRUM;
                     s32 liberi     = ZEPHYRUM;
                     s32 reliquum;
                     s32 cursor;
    DispositioAllineatio allin_a;
    DispositioAllineatio allin_t;
                     s32 c;

    si (parens)
    {
        a         = _axis_directionis(parens);
        origo[0]  = parens->positio[0];
        origo[1]  = parens->positio[1];
        interior_[0]  = parens->mensura[0] - _spatium(parens,
            ZEPHYRUM);
        interior_[1]  = parens->mensura[1] - _spatium(parens, I);
        initium[0]    = _spatium_initii(parens, ZEPHYRUM);
        initium[1]    = _spatium_initii(parens, I);
        intervallum   = parens->forma.intervallum;
        allin_a       = _allineatio(parens, a);
        allin_t       = _allineatio(parens, I - a);
    }
    alioquin
    {
        a             = ZEPHYRUM;
        origo[0]      = ZEPHYRUM;
        origo[1]      = ZEPHYRUM;
        interior_[0]  = d->superficies[0];
        interior_[1]  = d->superficies[1];
        initium[0]    = ZEPHYRUM;
        initium[1]    = ZEPHYRUM;
        intervallum   = ZEPHYRUM;
        allin_a       = DISPOSITIO_INITIUM;
        allin_t       = DISPOSITIO_INITIUM;
    }
    t = I - a;

    si (parens && parens->habet_textum)
    {
        contentum += parens->textus[a];
        liberi++;
    }
    per (c = primus; c >= ZEPHYRUM; c = _nodus(d, c)->proximus)
    {
        contentum += _nodus(d, c)->mensura[a];
        liberi++;
    }
    si (liberi > I)
    {
        contentum += (liberi - I) * intervallum;
    }
    reliquum = interior_[a] - contentum;
    si (allin_a == DISPOSITIO_INITIUM)
    {
        reliquum = ZEPHYRUM;
    }
    alioquin si (allin_a == DISPOSITIO_MEDIUM)
    {
        reliquum = _pavimentum(reliquum, II);
    }
    reliquum  = _maximum(ZEPHYRUM, reliquum);
    cursor    = initium[a] + reliquum;
    si (parens && parens->habet_textum)
    {
        cursor += parens->textus[a] + intervallum;
    }

    per (c = primus; c >= ZEPHYRUM; c = _nodus(d, c)->proximus)
    {
        Nodus* l            = _nodus(d, c);
          s32  album        = interior_[t] - l->mensura[t];
          s32  transversus  = initium[t];

        si (allin_t == DISPOSITIO_MEDIUM)
        {
            transversus += _pavimentum(album, II);
        }
        alioquin si (allin_t == DISPOSITIO_FINIS)
        {
            transversus += album;
        }
        l->positio[a]  = origo[a] + cursor;
        l->positio[t]  = origo[t] + transversus;
        cursor         += l->mensura[a] + intervallum;
    }
}

vacuum
dispositio_computare (
          Dispositio* d,
                 s32  latitudo,
                 s32  altitudo,
    DispositioMensor  mensor,
              vacuum* ctx)
{
    s32 n;
    s32 i;
    s32 axis;

    si (!d)
    {
        redde;
    }
    d->mensor          = mensor;
    d->mensor_ctx      = ctx;
    d->superficies[0]  = latitudo;
    d->superficies[1]  = altitudo;
    n                  = (s32)xar_numerus(d->nodi);

    /* (0) textus; (1) apta: parens semper ante liberos additus -
     * ordine inverso */
    per (i = ZEPHYRUM; i < n; i++)
    {
        _textum_metiri(d, _nodus(d, i));
    }
    per (i = n - I; i >= ZEPHYRUM; i--)
    {
        _aptare(d, i);
    }
    /* (2) per axem, x deinde y: radix, deinde parentes ordine */
    per (axis = ZEPHYRUM; axis < II; axis++)
    {
        _liberos_metiri(d, NIHIL, d->radix_primus, axis);
        per (i = ZEPHYRUM; i < n; i++)
        {
            Nodus* p = _nodus(d, i);

            si (p->primus >= ZEPHYRUM)
            {
                _liberos_metiri(d, p, p->primus, axis);
            }
        }
    }
    /* (3) positiones */
    _liberos_ponere(d, NIHIL, d->radix_primus);
    per (i = ZEPHYRUM; i < n; i++)
    {
        Nodus* p = _nodus(d, i);

        si (p->primus >= ZEPHYRUM)
        {
            _liberos_ponere(d, p, p->primus);
        }
    }
}

Fines
dispositio_fines (
    constans Dispositio* d,
                    s32  index)
{
    Fines f;

    memset(&f, ZEPHYRUM, magnitudo(Fines));
    si (d && index >= ZEPHYRUM && index < (s32)xar_numerus(d->nodi))
    {
        Nodus* n = _nodus(d, index);

        f.x         = n->positio[0];
        f.y         = n->positio[1];
        f.latitudo  = n->mensura[0];
        f.altitudo  = n->mensura[1];
    }
    redde f;
}

s32
dispositio_numerus (
    constans Dispositio* d)
{
    redde d ? (s32)xar_numerus(d->nodi) : ZEPHYRUM;
}
