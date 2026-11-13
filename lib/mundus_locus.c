/*
 * mundus_locus.c - dispositio et locus instructus (mundus_locus.h)
 *
 * legere: arbor STML semel ambulata. struere: tabula gressus loci
 * nudi altitudinem cuique rei dat; res solida per tabulam SUAM
 * (res sola, in loco suo) probatur: cella eius solida numeratur si
 * locus nudus ibi gressibilis est et summum rei solum attingit.
 * Craticulae omnium tabularum congruunt (origo unitas integra).
 */

#include "mundus_locus.h"
#include "mundus_solum.h"
#include "statua_iunctio.h"
#include "stereometria.h"
#include "stml.h"
#include "internamentum.h"
#include "xar.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define ERRORIS_MENSURA  CCLVI
/* summum rei solidae tantum infra solum iacere potest et tamen
 * cellam claudere (altitudine pari non gressibile vincit) */
#define PARITAS          1.0e-4f
/* fines attributorum */
#define SCALA_MINIMA     0.25f
#define SCALA_MAXIMA     4.0f
#define SEMEN_MAXIMUM    (M * M)
#define SPARSIO_MAXIMA   CCLVI
#define VARIETATES_MAXIMAE XVI
#define DISTANTIA_ORDINARIA 1.5f
/* quot puncta sparsio pro quoque exemplari temptat */
#define TENTAMINA        XL

nomen structura {
      Piscina* piscina;
          b32  fractum;
          i32  linea;
    character  error[ERRORIS_MENSURA];
} Lector;


/* ==================================================
 * Recusatio
 * ================================================== */

interior vacuum
_appone (
             character* alveus,
                   i32* longitudo,
    constans character* fons,
                   i32  mensura)
{
    i32 k;

    per (k = ZEPHYRUM;
         k < mensura && *longitudo + I < ERRORIS_MENSURA; k++)
    {
        character littera = fons[k];

        si (littera == '\n' || littera == '\r' || littera == '\t')
        {
            littera = ' ';
        }
        alveus[(*longitudo)++] = littera;
    }
    alveus[*longitudo] = '\0';
}

/* nuntius = ante + medium + post; prima recusatio manet */
interior b32
_recusare (
                Lector* l,
                   i32  linea,
    constans character* ante,
       constans chorda* medium,
    constans character* post)
{
    i32 longitudo = ZEPHYRUM;

    si (l->fractum)
    {
        redde FALSUM;
    }
    l->fractum  = VERUM;
    l->linea    = linea;
    si (ante)
    {
        _appone(l->error, &longitudo, ante, (i32)strlen(ante));
    }
    si (medium && medium->datum)
    {
        _appone(l->error, &longitudo,
            (constans character*)medium->datum,
            medium->mensura > LXIV ? LXIV : medium->mensura);
    }
    si (post)
    {
        _appone(l->error, &longitudo, post, (i32)strlen(post));
    }
    redde FALSUM;
}


/* ==================================================
 * Legere
 * ================================================== */

interior b32
_spatium (
    i8 littera)
{
    redde littera == ' ' || littera == '\t' || littera == '\n'
        || littera == '\r';
}

/* 'numerus' numeri finiti spatiis separati, nec plus nec minus */
interior b32
_numeri (
    constans chorda* valor,
                f32* exitus,
                i32  numerus)
{
    i32 k = ZEPHYRUM;
    i32 n = ZEPHYRUM;

    dum (VERUM)
    {
        f64 x;
        i32 initium;

        dum (k < valor->mensura && _spatium(valor->datum[k]))
        {
            k++;
        }
        si (k >= valor->mensura)
        {
            frange;
        }
        initium = k;
        dum (k < valor->mensura && !_spatium(valor->datum[k]))
        {
            k++;
        }
        si (   n >= numerus
            || !chorda_ut_f64(chorda_sectio(*valor, initium, k), &x)
            || !(x == x) || x > 1.0e6 || x < -1.0e6)
        {
            redde FALSUM;
        }
        exitus[n++] = (f32)x;
    }
    redde n == numerus;
}

/* omne attributum nodi in indice 'licita' (NIHIL terminato) est? */
interior b32
_attributa_licita (
                           Lector* l,
              constans StmlNodus* nodus,
    constans character* constans* licita)
{
    i32 numerus = nodus->attributa ? xar_numerus(nodus->attributa)
        : ZEPHYRUM;
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans StmlAttributum* a =
            (constans StmlAttributum*)xar_obtinere(nodus->attributa,
                k);
                            b32 notum = FALSUM;

        per (j = ZEPHYRUM; a && licita[j]; j++)
        {
            si (   a->titulus
                && chorda_aequalis_literis(*a->titulus, licita[j]))
            {
                notum = VERUM;
                frange;
            }
        }
        si (!notum)
        {
            redde _recusare(l, nodus->linea, "unknown attribute '",
                a ? a->titulus : NIHIL, "'");
        }
    }
    redde VERUM;
}

/* attributum requisitum non vacuum, in piscinam copiatum */
interior b32
_chorda_requisita (
                Lector* l,
    constans StmlNodus* nodus,
    constans character* titulus,
                chorda* exitus)
{
    constans chorda* valor = stml_attributum_capere(nodus, titulus);
                i32  k;

    per (k = ZEPHYRUM; valor && k < valor->mensura; k++)
    {
        si (!_spatium(valor->datum[k]))
        {
            *exitus = chorda_transcribere(*valor, l->piscina);
            redde VERUM;
        }
    }
    {
        chorda attributum = chorda_ex_literis(titulus, l->piscina);

        redde _recusare(l, nodus->linea,
            "<locus> needs a non-empty attribute '", &attributum, "'");
    }
}

/* "<elementum> attribute 'titulus' quid" */
interior b32
_recusare_attributum (
                Lector* l,
    constans StmlNodus* nodus,
    constans character* titulus,
    constans character* quid)
{
    character alveus[CXXVIII];
       chorda nomen_elementi;

    nomen_elementi.datum    = nodus->titulus ? nodus->titulus->datum
                                             : NIHIL;
    nomen_elementi.mensura  = nodus->titulus ? nodus->titulus->mensura
                                             : ZEPHYRUM;
    sprintf(alveus, "> attribute '%.40s' %.70s", titulus, quid);
    redde _recusare(l, nodus->linea, "<", &nomen_elementi, alveus);
}

/* attributum integrum intra [minimum, maximum]; absens: ordinarius
 * (aut recusatur si requisitum) */
interior b32
_integer (
                Lector* l,
    constans StmlNodus* nodus,
    constans character* titulus,
                   b32  requisitum,
                   i32  ordinarius_valor,
                   i32  minimum,
                   i32  maximum,
    constans character* quid,
                   i32* exitus)
{
    constans chorda* valor = stml_attributum_capere(nodus, titulus);
                f32  x;

    *exitus = ordinarius_valor;
    si (!valor)
    {
        redde requisitum
            ? _recusare_attributum(l, nodus, titulus, "is required")
            : VERUM;
    }
    si (   !_numeri(valor, &x, I) || x < 0.0f
        || x != (f32)floor((f64)x) || (i32)x < minimum
        || (i32)x > maximum)
    {
        redde _recusare_attributum(l, nodus, titulus, quid);
    }
    *exitus = (i32)x;
    redde VERUM;
}

/* attributum reale intra [minimum, maximum]; absens: ordinarius */
interior b32
_realis (
                Lector* l,
    constans StmlNodus* nodus,
    constans character* titulus,
                   f32  ordinarius_valor,
                   f32  minimum,
                   f32  maximum,
    constans character* quid,
                   f32* exitus)
{
    constans chorda* valor = stml_attributum_capere(nodus, titulus);

    *exitus = ordinarius_valor;
    si (!valor)
    {
        redde VERUM;
    }
    si (   !_numeri(valor, exitus, I) || *exitus < minimum
        || *exitus > maximum)
    {
        redde _recusare_attributum(l, nodus, titulus, quid);
    }
    redde VERUM;
}

/* elementum liberos elementares habet? */
interior b32
_habet_elementa (
    constans StmlNodus* nodus)
{
    i32 liberi = nodus->liberi ? xar_numerus(nodus->liberi) : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < liberi; k++)
    {
        StmlNodus** locus =
            (StmlNodus**)xar_obtinere(nodus->liberi, k);

        si (   locus && *locus
            && (*locus)->genus == STML_NODUS_ELEMENTUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_sparsionem_legere (
                Lector* l,
    constans StmlNodus* nodus,
    MundusLocusSparsio* sparsio)
{
    hic_manens constans character* constans LICITA[] = {
        "fons", "numerus", "semen", "area", "distantia",
        "magnitudo_varia", "gyrus_varius", "varietates", NIHIL
    };
    constans chorda* valor;

    si (!_attributa_licita(l, nodus, LICITA))
    {
        redde FALSUM;
    }
    si (_habet_elementa(nodus))
    {
        redde _recusare(l, nodus->linea,
            "<sparsio> cannot have children", NIHIL, NIHIL);
    }
    valor = stml_attributum_capere(nodus, "fons");
    si (!valor || valor->mensura == ZEPHYRUM)
    {
        redde _recusare_attributum(l, nodus, "fons",
            "is required (path of the asset's description)");
    }
    sparsio->fons   = chorda_transcribere(*valor, l->piscina);
    sparsio->linea  = nodus->linea;
    si (   !_integer(l, nodus, "numerus", VERUM, I, I, SPARSIO_MAXIMA,
               "must be a whole number from 1 to 256",
               &sparsio->numerus)
        || !_integer(l, nodus, "semen", FALSUM, ZEPHYRUM, ZEPHYRUM,
               SEMEN_MAXIMUM,
               "must be a whole number from 0 to 1000000",
               &sparsio->semen)
        || !_integer(l, nodus, "varietates", FALSUM, I, I,
               VARIETATES_MAXIMAE,
               "must be a whole number from 1 to 16",
               &sparsio->varietates)
        || !_realis(l, nodus, "distantia", DISTANTIA_ORDINARIA, 0.0f,
               1.0e6f, "must be a number that is not negative",
               &sparsio->distantia)
        || !_realis(l, nodus, "magnitudo_varia", 0.0f, 0.0f, 0.999f,
               "must be at least 0 and less than 1",
               &sparsio->magnitudo_varia))
    {
        redde FALSUM;
    }
    valor = stml_attributum_capere(nodus, "area");
    si (   !valor || !_numeri(valor, sparsio->area, IV)
        || !(sparsio->area[II] > sparsio->area[ZEPHYRUM])
        || !(sparsio->area[III] > sparsio->area[I]))
    {
        redde _recusare_attributum(l, nodus, "area",
            "needs 4 numbers (x0 z0 x1 z1) with x1 > x0 and z1 > z0");
    }
    sparsio->gyrus_varius  = FALSUM;
    valor                  =
        stml_attributum_capere(nodus, "gyrus_varius");
    si (valor)
    {
        si (!chorda_aequalis_literis(*valor, "verum"))
        {
            redde _recusare_attributum(l, nodus, "gyrus_varius",
                "must be 'verum' or absent");
        }
        sparsio->gyrus_varius = VERUM;
    }
    redde VERUM;
}

interior b32
_rem_legere (
                Lector* l,
    constans StmlNodus* nodus,
        MundusLocusRes* res)
{
    hic_manens constans character* constans LICITA[] = {
        "fons", "locus", "gyrus", "magnitudo", "semen", NIHIL
    };
    constans chorda* valor;
                f32  n[II];
                i32  liberi = nodus->liberi
                    ? xar_numerus(nodus->liberi) : ZEPHYRUM;
                i32 k;

    si (!_attributa_licita(l, nodus, LICITA))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < liberi; k++)
    {
        StmlNodus** locus =
            (StmlNodus**)xar_obtinere(nodus->liberi, k);

        si (   locus && *locus
            && (*locus)->genus == STML_NODUS_ELEMENTUM)
        {
            redde _recusare(l, nodus->linea,
                "<res> cannot have children", NIHIL, NIHIL);
        }
    }
    valor = stml_attributum_capere(nodus, "fons");
    si (!valor || valor->mensura == ZEPHYRUM)
    {
        redde _recusare(l, nodus->linea,
            "<res> needs the attribute 'fons' (path of the asset's"
            " description)", NIHIL, NIHIL);
    }
    res->fons   = chorda_transcribere(*valor, l->piscina);
    res->linea  = nodus->linea;
    valor       = stml_attributum_capere(nodus, "locus");
    si (!valor || !_numeri(valor, n, II))
    {
        redde _recusare(l, nodus->linea,
            "<res> attribute 'locus' needs 2 numbers (x z)", NIHIL,
            NIHIL);
    }
    res->x      = n[ZEPHYRUM];
    res->z      = n[I];
    res->gyrus  = 0.0f;
    valor       = stml_attributum_capere(nodus, "gyrus");
    si (valor && !_numeri(valor, &res->gyrus, I))
    {
        redde _recusare(l, nodus->linea,
            "<res> attribute 'gyrus' needs 1 number (degrees)", NIHIL,
            NIHIL);
    }
    redde _realis(l, nodus, "magnitudo", 1.0f, SCALA_MINIMA,
            SCALA_MAXIMA, "must be a number from 0.25 to 4",
            &res->scala)
        && _integer(l, nodus, "semen", FALSUM, ZEPHYRUM, ZEPHYRUM,
            SEMEN_MAXIMUM, "must be a whole number from 0 to 1000000",
            &res->semen);
}

interior b32
_ostium_legere (
                Lector* l,
    constans StmlNodus* nodus,
     MundusLocusOstium* ostium)
{
    hic_manens constans character* constans LICITA[] = {
        "locus", "radius", "scaena", NIHIL
    };
    constans chorda* valor;
                f32  n[II];
                i32  k;

    si (!_attributa_licita(l, nodus, LICITA))
    {
        redde FALSUM;
    }
    si (_habet_elementa(nodus))
    {
        redde _recusare(l, nodus->linea,
            "<ostium> cannot have children", NIHIL, NIHIL);
    }
    ostium->linea  = nodus->linea;
    valor          = stml_attributum_capere(nodus, "locus");
    si (!valor || !_numeri(valor, n, II))
    {
        redde _recusare_attributum(l, nodus, "locus",
            "needs 2 numbers (x z)");
    }
    ostium->x       = n[ZEPHYRUM];
    ostium->z       = n[I];
    ostium->radius  = MUNDUS_LOCUS_OSTII_RADIUS;
    valor           = stml_attributum_capere(nodus, "radius");
    si (valor && !_numeri(valor, &ostium->radius, I))
    {
        redde _recusare_attributum(l, nodus, "radius",
            "needs 1 number");
    }
    valor = stml_attributum_capere(nodus, "scaena");
    per (k = ZEPHYRUM; valor && k < valor->mensura; k++)
    {
        si (!_spatium(valor->datum[k]))
        {
            ostium->scaena = chorda_transcribere(*valor, l->piscina);
            redde VERUM;
        }
    }
    redde _recusare_attributum(l, nodus, "scaena",
        "is required (title of the scene view it leads to)");
}

interior MundusLocusFructus
_fructus (
    constans Lector* l)
{
    MundusLocusFructus f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.error = chorda_ex_literis(l->error, l->piscina);
    f.linea = l->linea;
    redde f;
}

MundusLocusFructus
mundus_locus_legere (
     chorda  textus,
    Piscina* piscina)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "basis", NIHIL
    };
    MundusLocusFructus  f;
                Lector  l;
          StmlResultus  r;
    constans StmlNodus* radix;
                   i32  liberi;
                   i32  k;

    memset(&f, ZEPHYRUM, magnitudo(f));
    memset(&l, ZEPHYRUM, magnitudo(l));
    si (!piscina)
    {
        redde f;
    }
    l.piscina = piscina;
    r = stml_legere(textus, piscina, internamentum_creare(piscina));
    si (!r.successus || !r.elementum_radix)
    {
        chorda causa = r.error;

        (vacuum)_recusare(&l, r.successus ? ZEPHYRUM : r.linea_erroris,
            "the layout is not valid STML: ",
            causa.mensura ? &causa : NIHIL,
            causa.mensura ? NIHIL : "no root element");
        redde _fructus(&l);
    }
    radix = r.elementum_radix;
    si (   !radix->titulus
        || !chorda_aequalis_literis(*radix->titulus, "locus"))
    {
        (vacuum)_recusare(&l, radix->linea,
            "the root element must be <locus>, got <", radix->titulus,
            ">");
        redde _fructus(&l);
    }
    si (   !_attributa_licita(&l, radix, LICITA)
        || !_chorda_requisita(&l, radix, "titulus", &f.titulus)
        || !_chorda_requisita(&l, radix, "basis", &f.basis))
    {
        redde _fructus(&l);
    }
    liberi = radix->liberi ? xar_numerus(radix->liberi) : ZEPHYRUM;
    f.res         = (MundusLocusRes*)piscina_allocare(piscina,
        (liberi + I) * magnitudo(MundusLocusRes));
    f.sparsiones  = (MundusLocusSparsio*)piscina_allocare(piscina,
        (liberi + I) * magnitudo(MundusLocusSparsio));
    f.ostia       = (MundusLocusOstium*)piscina_allocare(piscina,
        (liberi + I) * magnitudo(MundusLocusOstium));
    si (!f.res || !f.sparsiones || !f.ostia)
    {
        (vacuum)_recusare(&l, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
        redde _fructus(&l);
    }
    per (k = ZEPHYRUM; k < liberi; k++)
    {
                StmlNodus** locus =
                    (StmlNodus**)xar_obtinere(radix->liberi, k);
        constans StmlNodus* liber = locus ? *locus : NIHIL;
                       i32  j;

        si (!liber || liber->genus == STML_NODUS_COMMENTUM)
        {
            perge;
        }
        si (liber->genus == STML_NODUS_TEXTUS)
        {
            per (j = ZEPHYRUM; liber->valor
                && j < liber->valor->mensura;
                j++)
            {
                si (!_spatium(liber->valor->datum[j]))
                {
                    (vacuum)_recusare(&l, liber->linea,
                        "unexpected text between elements", NIHIL,
                        NIHIL);
                    redde _fructus(&l);
                }
            }
            perge;
        }
        si (   liber->genus == STML_NODUS_ELEMENTUM && liber->titulus
            && chorda_aequalis_literis(*liber->titulus, "sparsio"))
        {
            si (!_sparsionem_legere(&l, liber,
                &f.sparsiones[f.sparsionum]))
            {
                redde _fructus(&l);
            }
            f.sparsionum++;
            perge;
        }
        si (   liber->genus == STML_NODUS_ELEMENTUM && liber->titulus
            && chorda_aequalis_literis(*liber->titulus, "ostium"))
        {
            si (!_ostium_legere(&l, liber, &f.ostia[f.ostiorum]))
            {
                redde _fructus(&l);
            }
            f.ostiorum++;
            perge;
        }
        si (   liber->genus != STML_NODUS_ELEMENTUM || !liber->titulus
            || !chorda_aequalis_literis(*liber->titulus, "res"))
        {
            (vacuum)_recusare(&l, liber->linea, "unknown element <",
                liber->titulus,
                "> (only <res>, <sparsio> and <ostium> are allowed"
                " here)");
            redde _fructus(&l);
        }
        si (f.numerus >= MUNDUS_LOCUS_RES_MAXIMAE)
        {
            (vacuum)_recusare(&l, liber->linea,
                "the layout has too many placements (more than 4096)",
                NIHIL, NIHIL);
            redde _fructus(&l);
        }
        si (!_rem_legere(&l, liber, &f.res[f.numerus]))
        {
            redde _fructus(&l);
        }
        f.numerus++;
    }
    f.successus = VERUM;
    redde f;
}


/* ==================================================
 * Explicare
 * ================================================== */

interior i32
_miscere (
    i32 x)
{
    x ^= x >> XVI;
    x *= 0x85EBCA6Bu;
    x ^= x >> XIII;
    x *= 0xC2B2AE35u;
    x ^= x >> XVI;
    redde x;
}

/* [0, 1) ex semine et duobus indicibus (integer solus: eadem in
 * omni machina) */
interior f32
_fortuna (
    i32 semen,
    i32 a,
    i32 b)
{
    i32 x = _miscere(semen * 0x9E3779B1u + 0x7F4A7C15u);

    x = _miscere(x ^ (a * 0x85EBCA77u + I));
    x = _miscere(x ^ (b * 0xC2B2AE3Du + II));
    redde (f32)(x >> VIII) / 16777216.0f;
}

interior MundusLocusExplicatus
_explicatus_recusatus (
    constans character* causa,
                   i32  linea,
               Piscina* piscina)
{
    MundusLocusExplicatus e;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.linea = linea;
    si (piscina)
    {
        e.error = chorda_ex_literis(causa, piscina);
    }
    redde e;
}

MundusLocusExplicatus
mundus_locus_explicare (
    constans MundusLocusFructus* d,
                constans Statua* basis,
                        Piscina* piscina)
{
    MundusLocusExplicatus  e;
              MundusSolum* solum = NIHIL;
                character  causa[CC];
                      i32  summa;
                      i32  k;
                      i32  j;
                      i32  t;

    si (!d || !piscina || !d->successus)
    {
        redde _explicatus_recusatus("the layout was not read",
            ZEPHYRUM, piscina);
    }
    summa = d->numerus;
    per (k = ZEPHYRUM; k < d->sparsionum; k++)
    {
        summa += d->sparsiones[k].numerus;
    }
    si (summa > MUNDUS_LOCUS_RES_MAXIMAE)
    {
        redde _explicatus_recusatus("the layout places too many"
            " assets (more than 4096)", ZEPHYRUM, piscina);
    }
    memset(&e, ZEPHYRUM, magnitudo(e));
    e.res = (MundusLocusRes*)piscina_allocare(piscina,
        (summa + I) * magnitudo(MundusLocusRes));
    si (d->sparsionum)
    {
        solum = basis ? mundus_solum_struere(basis, piscina) : NIHIL;
        si (!solum)
        {
            redde _explicatus_recusatus("the level is missing or has"
                " no walk map", ZEPHYRUM, piscina);
        }
    }
    si (!e.res)
    {
        redde _explicatus_recusatus("out of memory", ZEPHYRUM,
            piscina);
    }
    per (k = ZEPHYRUM; k < d->numerus; k++)
    {
        e.res[e.numerus++] = d->res[k];
    }
    per (k = ZEPHYRUM; k < d->sparsionum; k++)
    {
        constans MundusLocusSparsio* s        = &d->sparsiones[k];
                                i32  positae  = ZEPHYRUM;

        per (t = ZEPHYRUM;
            positae < s->numerus && t < s->numerus * TENTAMINA; t++)
        {
            MundusLocusRes* nova = &e.res[e.numerus];
                       f32  x = s->area[ZEPHYRUM]
                           + _fortuna(s->semen, t, ZEPHYRUM)
                           * (s->area[II] - s->area[ZEPHYRUM]);
                       f32 z = s->area[I] + _fortuna(s->semen, t, I)
                           * (s->area[III] - s->area[I]);
                       f32 y;
                       b32 liber = VERUM;

            si (   mundus_solum_genus(solum, x, z)
                       != MUNDUS_SOLUM_GRESSIBILE
                || !mundus_solum_altitudo(solum, x, z, &y))
            {
                perge;
            }
            per (j = ZEPHYRUM; liber && j < e.numerus; j++)
            {
                f32 spatium_x = x - e.res[j].x;
                f32 spatium_z = z - e.res[j].z;

                liber = spatium_x * spatium_x + spatium_z * spatium_z
                    >= s->distantia * s->distantia;
            }
            si (!liber)
            {
                perge;
            }
            /* sortes II et III semper trahuntur: exemplar idem manet
             * cum attributa alia mutantur */
            nova->fons  = s->fons;
            nova->x     = x;
            nova->z     = z;
            nova->gyrus  = s->gyrus_varius
                ? _fortuna(s->semen, t, II) * 360.0f : 0.0f;
            nova->scala  = 1.0f + s->magnitudo_varia
                * (_fortuna(s->semen, t, III) * 2.0f - 1.0f);
            nova->semen = positae % s->varietates;
            nova->linea = s->linea;
            e.numerus++;
            positae++;
        }
        si (positae < s->numerus)
        {
            sprintf(causa, "only %u of %u copies could be placed (make"
                " the area larger, or the number or the spacing"
                " smaller)", (insignatus)positae,
                (insignatus)s->numerus);
            redde _explicatus_recusatus(causa, s->linea, piscina);
        }
    }
    e.successus = VERUM;
    redde e;
}


/* ==================================================
 * Struere
 * ================================================== */

interior MundusLocusInstructus
_recusatus (
    constans character* causa,
                   i32  linea,
               Piscina* piscina)
{
    MundusLocusInstructus r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.linea = linea;
    si (piscina)
    {
        r.error = chorda_ex_literis(causa, piscina);
    }
    redde r;
}

/* statua nodum solidum (sine gressibili) habet? */
interior b32
_habet_solidum (
    constans Statua* s)
{
    i32 n;

    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        si (   (s->nodi[n].signa & STATUA_SIGNUM_SOLIDUM)
            && !(s->nodi[n].signa & STATUA_SIGNUM_GRESSIBILE)
            && s->nodi[n].rete >= ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* cellae gressibiles loci nudi quas res sola (in loco suo) claudit;
 * -1 si memoria deficit */
interior s32
_cellae_clausae (
         constans MundusSolum* solum,
              constans Statua* vacua,
      constans StatuaIungenda* iungenda,
                      Piscina* piscina)
{
         Statua* sola = statua_coniungere(vacua, iungenda, I, piscina);
    MundusSolum* tabula;
            s32  clausae = ZEPHYRUM;
            i32  cella_x;
            i32  cella_z;

    tabula = sola ? mundus_solum_struere(sola, piscina) : NIHIL;
    si (!tabula)
    {
        redde -I;
    }
    per (cella_z = ZEPHYRUM; cella_z < tabula->altitudo; cella_z++)
    {
        per (cella_x = ZEPHYRUM; cella_x < tabula->latitudo; cella_x++)
        {
            memoriae_index locus = (memoriae_index)cella_z
                * tabula->latitudo + cella_x;
                       f32 x = tabula->origo_x
                           + ((f32)cella_x + 0.5f)
                           / (f32)MUNDUS_CELLAE_PER_UNITATEM;
                       f32 z = tabula->origo_z
                           + ((f32)cella_z + 0.5f)
                           / (f32)MUNDUS_CELLAE_PER_UNITATEM;
                       f32 y;

            si (   tabula->genera[locus] != MUNDUS_SOLUM_SOLIDUM
                || mundus_solum_genus(solum, x, z)
                       != MUNDUS_SOLUM_GRESSIBILE
                || !mundus_solum_altitudo(solum, x, z, &y))
            {
                perge;
            }
            clausae += (s32)(tabula->altitudines[locus]
                >= y - PARITAS);
        }
    }
    redde clausae;
}

MundusLocusInstructus
mundus_locus_struere (
              constans Statua* basis,
      constans MundusLocusRes* res,
    constans Statua* constans* statuae,
                          i32  numerus,
                      Piscina* piscina)
{
    MundusLocusInstructus  r;
              MundusSolum* solum;
           StatuaIungenda* iungendae;
                   Statua* vacua;
                      i32  k;

    si (!basis || !piscina || (numerus && (!res || !statuae)))
    {
        redde _recusatus("the level or the placements are missing",
            ZEPHYRUM, piscina);
    }
    solum      = mundus_solum_struere(basis, piscina);
    iungendae  = (StatuaIungenda*)piscina_allocare(piscina,
        (numerus + I) * magnitudo(StatuaIungenda));
    vacua      = (Statua*)piscina_allocare(piscina, magnitudo(Statua));
    si (!solum || !iungendae || !vacua)
    {
        redde _recusatus("out of memory", ZEPHYRUM, piscina);
    }
    memset(vacua, ZEPHYRUM, magnitudo(Statua));
    si (!statua_finire(vacua, piscina))
    {
        redde _recusatus("out of memory", ZEPHYRUM, piscina);
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        MundusSoliGenus genus;
                    f32 y;
                    s32 clausae;

        si (!statuae[k])
        {
            redde _recusatus("the asset was not compiled",
                res[k].linea, piscina);
        }
        si (!(res[k].scala > 0.0f))
        {
            redde _recusatus("the scale of the asset must be"
                " positive", res[k].linea, piscina);
        }
        genus = mundus_solum_genus(solum, res[k].x, res[k].z);
        si (genus == MUNDUS_SOLUM_NIHIL)
        {
            redde _recusatus("the spot lies outside the level",
                res[k].linea, piscina);
        }
        si (genus == MUNDUS_SOLUM_AQUA)
        {
            redde _recusatus("the spot is in water", res[k].linea,
                piscina);
        }
        si (   genus != MUNDUS_SOLUM_GRESSIBILE
            || !mundus_solum_altitudo(solum, res[k].x, res[k].z, &y))
        {
            redde _recusatus("the spot is not walkable ground",
                res[k].linea, piscina);
        }
        iungendae[k].statua  = statuae[k];
        iungendae[k].locus   = stereometria_vector(res[k].x, y,
            res[k].z);
        iungendae[k].gyrus = res[k].gyrus;
        iungendae[k].scala = res[k].scala;
        si (!_habet_solidum(statuae[k]))
        {
            perge;
        }
        clausae = _cellae_clausae(solum, vacua, &iungendae[k], piscina);
        si (clausae < ZEPHYRUM)
        {
            redde _recusatus("out of memory", res[k].linea, piscina);
        }
        si (clausae == ZEPHYRUM)
        {
            redde _recusatus("the asset is marked solid but blocks no"
                " cell (make its solid part at least 0.5 units wide,"
                " or add an unseen blocker)", res[k].linea, piscina);
        }
    }
    memset(&r, ZEPHYRUM, magnitudo(r));
    r.statua = statua_coniungere(basis, iungendae, numerus, piscina);
    si (!r.statua)
    {
        redde _recusatus("the level and its assets could not be"
            " joined", ZEPHYRUM, piscina);
    }
    r.successus = VERUM;
    redde r;
}
