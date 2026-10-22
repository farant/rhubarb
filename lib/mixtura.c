/* mixtura.c - Mixtura vocum (vide include/mixtura.h)
 *
 * Agmen redditur per SEGMENTA: iussa ordine temporis servantur
 * (insertio stabilis - eiusdem momenti ordine adventus), et inter
 * momenta duo iussorum voces quadratim redduntur. Iussum ad quadrum f
 * ANTE quadrum f reddendum agit. Summa: voces -> greges (lucrum,
 * mutus) -> summa (lucrum, mutus) -> exitus. Vide
 * lib/mixtura.worklog.md.
 */
#include "mixtura.h"
#include <math.h>

#define MIXTURA_QUADRANS_PI 0.78539816339744830962   /* pi / IV */

/* valor qui ad metam per quadra 'reliqua' linealiter it */
nomen structura {
    f32 valor;
    f32 meta;
    f32 passus;
    i32 reliqua;
} MixturaClivus;

nomen enumeratio {
    MIXTURA_VOX_VACUA = 0,
    MIXTURA_VOX_TABULAE,
    MIXTURA_VOX_SONITUS,
    MIXTURA_VOX_TIBIAE
} MixturaGenusVocis;

nomen structura {
    MixturaGenusVocis genus;
                  i32 generatio;
          MixturaGrex grex;
        MixturaClivus lucrum;
        MixturaClivus positio;
                  f32 sinistrum;    /* lucra potentiae constantis ex */
                  f32 dextrum;      /* positione (renovata dum movetur) */
                  b32 evanescens;   /* SISTERE: finis cum lucrum 0 */
        constans f32* exempla;
                  i32 longitudo;
                  i32 locus;
                  b32 iterans;
           SonitusVox sonitus;
             TibiaVox tibia;
} MixturaVox;

nomen structura {
    MixturaClivus lucrum;
              b32 mutus;
} MixturaGregis;

structura Mixtura {
       MixturaVox* voces;
              i32  numerus_vocum;
    MixturaIussum* iussa;          /* ordine momenti */
              i32  numerus_iussorum;
              i32  capacitas_iussorum;
   MixturaNuntius* nuntii;         /* circulus */
              i32  caput_nuntiorum;
              i32  numerus_nuntiorum;
              i32  capacitas_nuntiorum;
    MixturaGregis  greges[MIXTURA_GREGES];
              s64  momentum;
    MixturaCensus  census;
};

nomen structura {
    i32 generatio;
    b32 occupata;
    i32 prioritas;
    i32 aetas;
} MixturaSedes;

structura MixturaRector {
    MixturaSedes* sedes;
             i32  numerus;
             i32  aetas;
};


    /* ==================================================
     * CLIVUS
     * ================================================== */

interior vacuum
_clivum_statuere (
    MixturaClivus* k,
              f32  valor)
{
    k->valor    = valor;
    k->meta     = valor;
    k->passus   = 0.0f;
    k->reliqua  = ZEPHYRUM;
}

interior vacuum
_clivum_ponere (
    MixturaClivus* k,
              f32  meta,
              i32  quadra)
{
    si (quadra == ZEPHYRUM)
    {
        _clivum_statuere(k, meta);
        redde;
    }
    k->meta     = meta;
    k->passus   = (meta - k->valor) / (f32)quadra;
    k->reliqua  = quadra;
}

/* valor huius quadri; ultimo passu meta EXACTA (sine residuo
 * fluitante) */
interior f32
_clivum_provehere (
    MixturaClivus* k)
{
    si (k->reliqua > ZEPHYRUM)
    {
        k->reliqua--;
        k->valor = k->reliqua == ZEPHYRUM ? k->meta
                                          : k->valor + k->passus;
    }
    redde k->valor;
}

interior vacuum
_positionem_computare (
    MixturaVox* v)
{
    f64 p = (f64)v->positio.valor;

    si (p < -1.0)
    {
        p = -1.0;
    }
    si (p > 1.0)
    {
        p = 1.0;
    }
    v->sinistrum  = (f32)cos((p + 1.0) * MIXTURA_QUADRANS_PI);
    v->dextrum    = (f32)sin((p + 1.0) * MIXTURA_QUADRANS_PI);
}


    /* ==================================================
     * NUNTII
     * ================================================== */

interior vacuum
_nuntiare (
                    Mixtura* m,
    constans MixturaNuntius* n)
{
    i32 locus;

    si (m->numerus_nuntiorum >= m->capacitas_nuntiorum)
    {
        m->census.nuntii_amissi++;
        redde;
    }
    locus = (m->caput_nuntiorum + m->numerus_nuntiorum)
          % m->capacitas_nuntiorum;
    m->nuntii[locus] = *n;
    m->numerus_nuntiorum++;
}

interior vacuum
_vocem_finire (
              Mixtura* m,
                  i32  sedes,
    MixturaCausaFinis  causa,
                  s64  momentum)
{
        MixturaVox* v = &m->voces[sedes];
    MixturaNuntius  n;

    n.genus           = MIXTURA_NUNTIUS_VOX_FINITA;
    n.momentum        = momentum;
    n.ansa.sedes      = sedes;
    n.ansa.generatio  = v->generatio;
    n.causa           = causa;
    n.exempla = v->genus
        == MIXTURA_VOX_TABULAE ? v->exempla : NIHIL;
    n.census = m->census;
    v->genus = MIXTURA_VOX_VACUA;
    _nuntiare(m, &n);
}


    /* ==================================================
     * IUSSA
     * ================================================== */

/* vox viva ansae, aliter NIHIL (irrita) */
interior MixturaVox*
_vox_ansae (
        Mixtura* m,
    MixturaAnsa  ansa)
{
    MixturaVox* v;

    si (ansa.sedes >= m->numerus_vocum || ansa.generatio == ZEPHYRUM)
    {
        redde NIHIL;
    }
    v = &m->voces[ansa.sedes];
    si (v->genus == MIXTURA_VOX_VACUA || v->generatio != ansa.generatio)
    {
        redde NIHIL;
    }
    redde v;
}

interior vacuum
_incipere (
                   Mixtura* m,
    constans MixturaIussum* i,
                       s64  momentum)
{
    MixturaVox* v;

    si (   i->ansa.sedes     >= m->numerus_vocum
        || i->ansa.generatio == ZEPHYRUM
        || (i->grex >= MIXTURA_GREGES || i->grex == MIXTURA_GREX_SUMMA))
    {
        m->census.iussa_irrita++;
        redde;
    }
    v = &m->voces[i->ansa.sedes];
    si (v->genus != MIXTURA_VOX_VACUA)
    {
        si (v->generatio == i->ansa.generatio)
        {
            m->census.iussa_irrita++;   /* initium duplex */
            redde;
        }
        _vocem_finire(m, i->ansa.sedes, MIXTURA_FINIS_SURREPTA,
            momentum);
    }
    v->generatio   = i->ansa.generatio;
    v->grex        = i->grex;
    v->evanescens  = FALSUM;
    /* clivus initii = introitus a silentio; 0 = plenum statim
     * (impetus servatur) */
    _clivum_statuere(&v->lucrum, i->clivus
        > ZEPHYRUM ? 0.0f : i->lucrum);
    _clivum_ponere(&v->lucrum, i->lucrum, i->clivus);
    _clivum_statuere(&v->positio, i->positio);
    _positionem_computare(v);
    si (i->genus == MIXTURA_IUSSUM_TABULAM)
    {
        v->genus      = MIXTURA_VOX_TABULAE;
        v->exempla    = i->exempla;
        v->longitudo  = i->exempla == NIHIL ? ZEPHYRUM : i->longitudo;
        v->locus      = ZEPHYRUM;
        v->iterans    = i->iterans;
    }
    alioquin si (i->genus == MIXTURA_IUSSUM_TIBIAM)
    {
        v->genus    = MIXTURA_VOX_TIBIAE;
        v->exempla  = NIHIL;
        tibia_vocem_parare(&v->tibia, &i->tibia, i->nota);
    }
    alioquin
    {
        v->genus    = MIXTURA_VOX_SONITUS;
        v->exempla  = NIHIL;
        sonitus_vocem_parare(&v->sonitus, &i->parametra, i->semen);
    }
}

/* vox tibiae ansae; NIHIL (iussum irritum) si stala aut generis alii */
interior MixturaVox*
_tibia_ansae (
        Mixtura* m,
    MixturaAnsa  ansa)
{
    MixturaVox* v = _vox_ansae(m, ansa);

    si (v == NIHIL || v->genus != MIXTURA_VOX_TIBIAE)
    {
        m->census.iussa_irrita++;
        redde NIHIL;
    }
    redde v;
}

interior i32
_voces_activae (
    constans Mixtura* m)
{
    i32 k;
    i32 n = ZEPHYRUM;

    per (k = ZEPHYRUM; k < m->numerus_vocum; k++)
    {
        si (m->voces[k].genus != MIXTURA_VOX_VACUA)
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
_agere (
                   Mixtura* m,
    constans MixturaIussum* i,
                       s64  momentum)
{
    MixturaVox* v;

    commutatio (i->genus)
    {
        casus MIXTURA_IUSSUM_TABULAM:
        casus MIXTURA_IUSSUM_SONITUM:
        casus MIXTURA_IUSSUM_TIBIAM:
            _incipere(m, i, momentum);
            frange;
        casus MIXTURA_IUSSUM_NOTAM_PONERE:
            v = _tibia_ansae(m, i->ansa);
            si (v != NIHIL)
            {
                tibia_notam_ponere(&v->tibia, i->nota, i->clivus);
            }
            frange;
        casus MIXTURA_IUSSUM_PORTAM_CLAUDERE:
            v = _tibia_ansae(m, i->ansa);
            si (v != NIHIL)
            {
                tibia_portam_claudere(&v->tibia);
            }
            frange;
        casus MIXTURA_IUSSUM_SISTERE:
            v = _vox_ansae(m, i->ansa);
            si (v == NIHIL)
            {
                m->census.iussa_irrita++;
            }
            alioquin si (i->clivus == ZEPHYRUM)
            {
                _vocem_finire(m, i->ansa.sedes, MIXTURA_FINIS_IUSSU,
                              momentum);
            }
            alioquin
            {
                v->evanescens = VERUM;
                _clivum_ponere(&v->lucrum, 0.0f, i->clivus);
            }
            frange;
        casus MIXTURA_IUSSUM_VOCEM_PONERE:
            v = _vox_ansae(m, i->ansa);
            si (v == NIHIL || v->evanescens)
            {
                m->census.iussa_irrita++;
            }
            alioquin
            {
                _clivum_ponere(&v->lucrum, i->lucrum, i->clivus);
                _clivum_ponere(&v->positio, i->positio, i->clivus);
                _positionem_computare(v);
            }
            frange;
        casus MIXTURA_IUSSUM_GREGEM_PONERE:
            si (i->grex >= MIXTURA_GREGES)
            {
                m->census.iussa_irrita++;
            }
            alioquin
            {
                _clivum_ponere(&m->greges[i->grex].lucrum, i->lucrum,
                               i->clivus);
                m->greges[i->grex].mutus = i->mutus;
            }
            frange;
        casus MIXTURA_IUSSUM_CENSUM_PETERE:
        {
            MixturaNuntius n;

            m->census.voces_activae  = _voces_activae(m);
            n.genus                  = MIXTURA_NUNTIUS_CENSUS;
            n.momentum               = momentum;
            n.ansa.sedes             = ZEPHYRUM;
            n.ansa.generatio         = ZEPHYRUM;
            n.causa                  = MIXTURA_FINIS_NATURALIS;
            n.exempla                = NIHIL;
            n.census                 = m->census;
            _nuntiare(m, &n);
            m->census.apex = 0.0f;
        }
            frange;
        ordinarius:
            m->census.iussa_irrita++;
            frange;
    }
}


    /* ==================================================
     * REDDERE
     * ================================================== */

/* exemplum vocis huius quadri; VERUM si vox post hoc quadrum finitur */
interior f32
_vocis_exemplum (
    MixturaVox* v,
           b32* finita)
{
    f32 x = 0.0f;

    *finita = FALSUM;
    si (v->genus == MIXTURA_VOX_TABULAE)
    {
        si (v->locus < v->longitudo)
        {
            x = v->exempla[v->locus];
            v->locus++;
        }
        si (v->locus >= v->longitudo)
        {
            si (v->iterans && v->longitudo > ZEPHYRUM)
            {
                v->locus = ZEPHYRUM;
            }
            alioquin
            {
                *finita = VERUM;
            }
        }
    }
    alioquin si (v->genus == MIXTURA_VOX_TIBIAE)
    {
        x        = tibia_vocis_proximum(&v->tibia);
        *finita  = tibia_vox_finita(&v->tibia);
    }
    alioquin
    {
        x        = sonitus_vocis_proximum(&v->sonitus);
        *finita  = sonitus_vox_finita(&v->sonitus);
    }
    redde x;
}

interior vacuum
_segmentum_reddere (
    Mixtura* m,
        f32* exitus,
        i32  quadra)
{
    i32 q;

    per (q = ZEPHYRUM; q < quadra; q++)
    {
        f32 summae[MIXTURA_GREGES][II];
        f32 l = 0.0f;
        f32 r = 0.0f;
        f32 lucrum_summae;
        i32 g;
        i32 k;

        per (g = ZEPHYRUM; g < MIXTURA_GREGES; g++)
        {
            summae[g][ZEPHYRUM]  = 0.0f;
            summae[g][I]         = 0.0f;
        }
        per (k = ZEPHYRUM; k < m->numerus_vocum; k++)
        {
             MixturaVox* v = &m->voces[k];
                    f32  x;
                    f32  lucrum;
                    b32  finita;

            si (v->genus == MIXTURA_VOX_VACUA)
            {
                perge;
            }
            lucrum = _clivum_provehere(&v->lucrum);
            si (v->positio.reliqua > ZEPHYRUM)
            {
                (vacuum)_clivum_provehere(&v->positio);
                _positionem_computare(v);
            }
            x = _vocis_exemplum(v, &finita) * lucrum;
            summae[v->grex][ZEPHYRUM] += x * v->sinistrum;
            summae[v->grex][I] += x * v->dextrum;
            si (finita)
            {
                _vocem_finire(m, k, MIXTURA_FINIS_NATURALIS,
                              m->momentum + (s64)q + I);
            }
            alioquin si (v->evanescens && v->lucrum.reliqua == ZEPHYRUM)
            {
                _vocem_finire(m, k, MIXTURA_FINIS_IUSSU,
                              m->momentum + (s64)q + I);
            }
        }
        per (g = I; g < MIXTURA_GREGES; g++)
        {
            f32 lucrum = _clivum_provehere(&m->greges[g].lucrum);

            si (!m->greges[g].mutus)
            {
                l += summae[g][ZEPHYRUM] * lucrum;
                r += summae[g][I] * lucrum;
            }
        }
        lucrum_summae = _clivum_provehere(
            &m->greges[MIXTURA_GREX_SUMMA].lucrum);
        si (m->greges[MIXTURA_GREX_SUMMA].mutus)
        {
            lucrum_summae = 0.0f;
        }
        l                   *= lucrum_summae;
        r                   *= lucrum_summae;
        exitus[II * q]      = l;
        exitus[II * q + I]  = r;
        si (l < 0.0f) l = -l;
        si (r < 0.0f) r = -r;
        si (l > m->census.apex) m->census.apex = l;
        si (r > m->census.apex) m->census.apex = r;
    }
    m->momentum += (s64)quadra;
}

vacuum
mixtura_reddere (
    Mixtura* m,
        f32* exitus,
        i32  quadra)
{
    s64 initium  = m->momentum;
    s64 finis    = m->momentum + (s64)quadra;
    i32 positus  = ZEPHYRUM;

    dum (positus < quadra)
    {
        s64 proximum  = finis;
        i32 acta      = ZEPHYRUM;
        i32 k;

        /* iussa quorum momentum advenit (aut praeteriit) agunt */
        dum (   acta < m->numerus_iussorum
             && m->iussa[acta].momentum <= m->momentum)
        {
            si (m->iussa[acta].momentum < initium)
            {
                m->census.iussa_tarda++;
            }
            _agere(m, &m->iussa[acta], m->momentum);
            acta++;
        }
        si (acta > ZEPHYRUM)
        {
            per (k = acta; k < m->numerus_iussorum; k++)
            {
                m->iussa[k - acta] = m->iussa[k];
            }
            m->numerus_iussorum -= acta;
        }
        si (   m->numerus_iussorum > ZEPHYRUM
            && m->iussa[ZEPHYRUM].momentum < finis)
        {
            proximum = m->iussa[ZEPHYRUM].momentum;
        }
        _segmentum_reddere(m, exitus + II * positus,
                           (i32)(proximum - m->momentum));
        positus = (i32)(m->momentum - initium);
    }
}


    /* ==================================================
     * INTERFACIES PUBLICA
     * ================================================== */

b32
mixtura_ansa_nulla (
    MixturaAnsa ansa)
{
    redde ansa.generatio == ZEPHYRUM;
}

vacuum
mixtura_iussum_parare (
          MixturaIussum* iussum,
     MixturaIussumGenus  genus,
                    s64  momentum)
{
    i32 k;

    iussum->genus           = genus;
    iussum->momentum        = momentum;
    iussum->ansa.sedes      = ZEPHYRUM;
    iussum->ansa.generatio  = ZEPHYRUM;
    iussum->grex            = MIXTURA_GREX_EFFECTUS;
    iussum->lucrum          = 1.0f;
    iussum->positio         = 0.0f;
    iussum->mutus           = FALSUM;
    iussum->clivus          = ZEPHYRUM;
    iussum->exempla         = NIHIL;
    iussum->longitudo       = ZEPHYRUM;
    iussum->iterans         = FALSUM;
    per (k = ZEPHYRUM; k < (i32)sonitus_campi_numerus(); k++)
    {
        *(f32*)((character*)&iussum->parametra
                + sonitus_campus(k)->offset) = 0.0f;
    }
    iussum->semen = ZEPHYRUM;
}

Mixtura*
mixtura_creare (
                  Piscina* piscina,
    constans MixturaForma* forma)
{
     Mixtura* m;
         i32  k;

    si (   forma        == NIHIL || forma->voces == ZEPHYRUM
        || forma->iussa == ZEPHYRUM || forma->nuntii == ZEPHYRUM)
    {
        redde NIHIL;
    }
    m = (Mixtura*)piscina_allocare(piscina, magnitudo(Mixtura));
    si (m == NIHIL)
    {
        redde NIHIL;
    }
    m->voces = (MixturaVox*)piscina_allocare(
        piscina, (memoriae_index)forma->voces * magnitudo(MixturaVox));
    m->iussa = (MixturaIussum*)piscina_allocare(
        piscina,
        (memoriae_index)forma->iussa * magnitudo(MixturaIussum));
    m->nuntii = (MixturaNuntius*)piscina_allocare(
        piscina,
        (memoriae_index)forma->nuntii * magnitudo(MixturaNuntius));
    si (m->voces == NIHIL || m->iussa == NIHIL || m->nuntii == NIHIL)
    {
        redde NIHIL;
    }
    m->numerus_vocum        = forma->voces;
    m->numerus_iussorum     = ZEPHYRUM;
    m->capacitas_iussorum   = forma->iussa;
    m->caput_nuntiorum      = ZEPHYRUM;
    m->numerus_nuntiorum    = ZEPHYRUM;
    m->capacitas_nuntiorum  = forma->nuntii;
    m->momentum             = ZEPHYRUM;
    per (k = ZEPHYRUM; k < forma->voces; k++)
    {
        m->voces[k].genus      = MIXTURA_VOX_VACUA;
        m->voces[k].generatio  = ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < MIXTURA_GREGES; k++)
    {
        _clivum_statuere(&m->greges[k].lucrum, 1.0f);
        m->greges[k].mutus = FALSUM;
    }
    m->census.voces_activae  = ZEPHYRUM;
    m->census.iussa_tarda    = ZEPHYRUM;
    m->census.iussa_irrita   = ZEPHYRUM;
    m->census.iussa_amissa   = ZEPHYRUM;
    m->census.nuntii_amissi  = ZEPHYRUM;
    m->census.apex           = 0.0f;
    redde m;
}

/* insertio stabilis: post omnia iussa eiusdem aut prioris momenti */
b32
mixtura_iubere (
                   Mixtura* m,
    constans MixturaIussum* iussum)
{
    i32 k;

    si (m->numerus_iussorum >= m->capacitas_iussorum)
    {
        m->census.iussa_amissa++;
        redde FALSUM;
    }
    k = m->numerus_iussorum;
    dum (k > ZEPHYRUM && m->iussa[k - I].momentum > iussum->momentum)
    {
        m->iussa[k] = m->iussa[k - I];
        k--;
    }
    m->iussa[k] = *iussum;
    m->numerus_iussorum++;
    redde VERUM;
}

b32
mixtura_nuntium_legere (
           Mixtura* m,
    MixturaNuntius* nuntius)
{
    si (m->numerus_nuntiorum == ZEPHYRUM)
    {
        redde FALSUM;
    }
    *nuntius = m->nuntii[m->caput_nuntiorum];
    m->caput_nuntiorum = (m->caput_nuntiorum + I)
        % m->capacitas_nuntiorum;
    m->numerus_nuntiorum--;
    redde VERUM;
}

s64
mixtura_momentum (
    constans Mixtura* m)
{
    redde m->momentum;
}


    /* ==================================================
     * RECTOR
     * ================================================== */

MixturaRector*
mixtura_rectorem_creare (
    Piscina* piscina,
        i32  voces)
{
    MixturaRector* r;
              i32  k;

    si (voces == ZEPHYRUM)
    {
        redde NIHIL;
    }
    r = (MixturaRector*)piscina_allocare(piscina,
        magnitudo(MixturaRector));
    si (r == NIHIL)
    {
        redde NIHIL;
    }
    r->sedes = (MixturaSedes*)piscina_allocare(
        piscina, (memoriae_index)voces * magnitudo(MixturaSedes));
    si (r->sedes == NIHIL)
    {
        redde NIHIL;
    }
    r->numerus  = voces;
    r->aetas    = ZEPHYRUM;
    per (k = ZEPHYRUM; k < voces; k++)
    {
        r->sedes[k].generatio  = ZEPHYRUM;
        r->sedes[k].occupata   = FALSUM;
        r->sedes[k].prioritas  = ZEPHYRUM;
        r->sedes[k].aetas      = ZEPHYRUM;
    }
    redde r;
}

MixturaAnsa
mixtura_rector_capere (
    MixturaRector* r,
              i32  prioritas)
{
    MixturaAnsa ansa;
            i32 k;
            i32 electa = r->numerus;

    ansa.sedes      = ZEPHYRUM;
    ansa.generatio  = ZEPHYRUM;
    per (k = ZEPHYRUM; k < r->numerus; k++)
    {
        si (!r->sedes[k].occupata)
        {
            electa = k;
            frange;
        }
    }
    si (electa == r->numerus)
    {
        /* surreptio: prioritas infima, aequalibus vetustissima */
        electa = ZEPHYRUM;
        per (k = I; k < r->numerus; k++)
        {
            MixturaSedes* s = &r->sedes[k];
            MixturaSedes* e = &r->sedes[electa];

            si (   s->prioritas < e->prioritas
                || (s->prioritas == e->prioritas
                    && s->aetas < e->aetas))
            {
                electa = k;
            }
        }
        si (r->sedes[electa].prioritas > prioritas)
        {
            redde ansa;
        }
    }
    r->sedes[electa].generatio++;
    si (r->sedes[electa].generatio == ZEPHYRUM)
    {
        r->sedes[electa].generatio = I;   /* 0 = ansa nulla */
    }
    r->aetas++;
    r->sedes[electa].occupata   = VERUM;
    r->sedes[electa].prioritas  = prioritas;
    r->sedes[electa].aetas      = r->aetas;
    ansa.sedes                  = electa;
    ansa.generatio              = r->sedes[electa].generatio;
    redde ansa;
}

vacuum
mixtura_rector_tractare (
              MixturaRector* r,
    constans MixturaNuntius* nuntius)
{
    MixturaSedes* s;

    si (   nuntius->genus      != MIXTURA_NUNTIUS_VOX_FINITA
        || nuntius->ansa.sedes >= r->numerus)
    {
        redde;
    }
    s = &r->sedes[nuntius->ansa.sedes];
    si (s->occupata && s->generatio == nuntius->ansa.generatio)
    {
        s->occupata = FALSUM;
    }
}
