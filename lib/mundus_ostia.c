/*
 * mundus_ostia.c - ostia loci (vide mundus_ostia.h)
 *
 * probare: quodque ostium ordine - titulus, radius, locus in tabula,
 * solum intra radium, punctum iteratum; primum culpatum vincit.
 * volumen: actum unum per positionem fert indicem TOTUM loci; lector
 * acta ordine seq percurrit et novissimum bonum loci tenet.
 */

#include "mundus_ostia.h"
#include "scaena_repositorium.h"
#include "json.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define ERRORIS_MENSURA  CCLVI
#define MILLE            1000.0

interior s32
_millesimae (
    f32 x)
{
    redde (s32)floor((f64)x * MILLE + 0.5);
}

interior MundusOstiaFructus
_recusare (
               Piscina* piscina,
                   i32  linea,
    constans character* nuntius)
{
    MundusOstiaFructus f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.linea = linea;
    si (piscina)
    {
        f.error = chorda_ex_literis(nuntius, piscina);
    }
    redde f;
}

/* est cella in qua corpus stare potest et cuius centrum intra
 * radium a (x, z) iacet? */
interior b32
_solum_intra_radium (
    constans MundusSolum* s,
                     f32  x,
                     f32  z,
                     f32  radius,
                     f32  corpus)
{
    f32 cella  = 1.0f / (f32)MUNDUS_CELLAE_PER_UNITATEM;
    s32 i0     = (s32)floor((f64)((x - radius - s->origo_x) / cella));
    s32 i1     = (s32)floor((f64)((x + radius - s->origo_x) / cella));
    s32 j0     = (s32)floor((f64)((z - radius - s->origo_z) / cella));
    s32 j1     = (s32)floor((f64)((z + radius - s->origo_z) / cella));
    s32 i;
    s32 j;

    si (i0 < ZEPHYRUM)
    {
        i0 = ZEPHYRUM;
    }
    si (j0 < ZEPHYRUM)
    {
        j0 = ZEPHYRUM;
    }
    si (i1 >= (s32)s->latitudo)
    {
        i1 = (s32)s->latitudo - I;
    }
    si (j1 >= (s32)s->altitudo)
    {
        j1 = (s32)s->altitudo - I;
    }
    per (j = j0; j <= j1; j++)
    {
        per (i = i0; i <= i1; i++)
        {
            f32 medium_x   = s->origo_x + ((f32)i + 0.5f) * cella;
            f32 medium_z   = s->origo_z + ((f32)j + 0.5f) * cella;
            f32 spatium_x  = medium_x - x;
            f32 spatium_z  = medium_z - z;

            si (   spatium_x * spatium_x + spatium_z * spatium_z
                <= radius * radius
                && mundus_solum_stare(s, medium_x, medium_z, corpus))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

MundusOstiaFructus
mundus_ostia_probare (
    constans MundusLocusOstium* ostia,
                           i32  numerus,
          constans MundusSolum* solum,
                           f32  corpus,
                       Piscina* piscina)
{
    MundusOstiaFructus f;
             character nuntius[ERRORIS_MENSURA];
                   i32 k;
                   i32 j;

    memset(&f, ZEPHYRUM, magnitudo(f));
    si (!piscina || !solum || (numerus > ZEPHYRUM && !ostia))
    {
        redde _recusare(piscina, ZEPHYRUM, "nothing to check");
    }
    si (numerus > MUNDUS_OSTIA_MAXIMA)
    {
        redde _recusare(piscina, ostia[MUNDUS_OSTIA_MAXIMA].linea,
            "too many doorways in one place (more than 64)");
    }
    si (numerus == ZEPHYRUM)
    {
        f.successus = VERUM;
        redde f;
    }
    f.ostia = (MundusOstium*)piscina_allocare(piscina,
        numerus * magnitudo(MundusOstium));
    si (!f.ostia)
    {
        redde _recusare(piscina, ZEPHYRUM, "out of memory");
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans MundusLocusOstium* o = &ostia[k];
                      MundusOstium* e = &f.ostia[k];

        si (!scaena_titulus_validus(o->scaena))
        {
            /* titulus ut scriptus (non NUL terminatus) */
            {
                i32 mensura = o->scaena.mensura > LXIV
                    ? LXIV : o->scaena.mensura;

                sprintf(nuntius, "<ostium> scene '%.*s' is not a scene"
                    " view title (1 to 64 of a-z, 0-9, '-', '_'; not"
                    " 'mundus')", (integer)mensura,
                    o->scaena.datum
                        ? (constans character*)o->scaena.datum : "");
            }
            redde _recusare(piscina, o->linea, nuntius);
        }
        si (   !(o->radius >= MUNDUS_OSTII_RADIUS_MINIMUS)
            || !(o->radius <= MUNDUS_OSTII_RADIUS_MAXIMUS))
        {
            redde _recusare(piscina, o->linea,
                "<ostium> attribute 'radius' must be a number from"
                " 0.5 to 8");
        }
        si (mundus_solum_genus(solum, o->x, o->z)
            == MUNDUS_SOLUM_NIHIL)
        {
            redde _recusare(piscina, o->linea,
                "<ostium> is outside the place");
        }
        si (!_solum_intra_radium(solum, o->x, o->z, o->radius,
            corpus))
        {
            redde _recusare(piscina, o->linea,
                "<ostium> has no ground to stand on within its"
                " radius: its prompt could never appear");
        }
        e->scaena             = chorda_transcribere(o->scaena,
            piscina);
        e->x_millesimae       = _millesimae(o->x);
        e->z_millesimae       = _millesimae(o->z);
        e->radius_millesimae  = _millesimae(o->radius);
        per (j = ZEPHYRUM; j < k; j++)
        {
            si (   f.ostia[j].x_millesimae == e->x_millesimae
                && f.ostia[j].z_millesimae == e->z_millesimae)
            {
                redde _recusare(piscina, o->linea,
                    "<ostium> is at the same spot as an earlier"
                    " doorway");
            }
        }
    }
    f.numerus    = numerus;
    f.successus  = VERUM;
    redde f;
}

/* acta propria ostiorum, ordine seq */
interior Xar*
_acta (
    Volumen* volumen,
    Piscina* piscina)
{
    hic_manens constans character* constans GENERA[] = {
        MUNDUS_OSTIA_ACTUM, NIHIL
    };
    chorda replica = volumen_replica(volumen);

    si (replica.mensura > ZEPHYRUM)
    {
        redde volumen_acta_generum_replicae_legere(volumen, replica,
            ZEPHYRUM, GENERA, piscina);
    }
    redde volumen_acta_generum_legere(volumen, ZEPHYRUM, GENERA,
        piscina);
}

/* numerus integer campi in fines s32; FALSUM si abest aut alius */
interior b32
_campus_integer (
             JsonValor* objectum,
    constans character* clavis,
                   s32* exitus)
{
    JsonValor* valor = json_objectum_capere(objectum, clavis);
          s64  n;

    si (!valor || !json_est_integer(valor))
    {
        redde FALSUM;
    }
    n = json_ad_integer(valor);
    si (n < -2000000000 || n > 2000000000)
    {
        redde FALSUM;
    }
    *exitus = (s32)n;
    redde VERUM;
}

/* tabulatum ostiorum actus in indicem novum; NIHIL si quid malum */
interior Xar*
_indicem_legere (
    JsonValor* tabulatum,
      Piscina* piscina)
{
    Xar* index = xar_creare(piscina, magnitudo(MundusOstium));
    i32  numerus;
    i32  k;

    si (!index || !tabulatum || !json_est_tabulatum(tabulatum))
    {
        redde NIHIL;
    }
    numerus = json_tabulatum_numerus(tabulatum);
    si (numerus > MUNDUS_OSTIA_MAXIMA)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
           JsonValor* o = json_tabulatum_obtinere(tabulatum, k);
           JsonValor* titulus;
        MundusOstium  e;
        MundusOstium* locus;

        si (!o || !json_est_objectum(o))
        {
            redde NIHIL;
        }
        titulus = json_objectum_capere(o, "scaena");
        si (   !titulus || !json_est_chorda(titulus)
            || !scaena_titulus_validus(json_ad_chorda(titulus))
            || !_campus_integer(o, "x", &e.x_millesimae)
            || !_campus_integer(o, "z", &e.z_millesimae)
            || !_campus_integer(o, "radius", &e.radius_millesimae)
            || e.radius_millesimae
                < _millesimae(MUNDUS_OSTII_RADIUS_MINIMUS)
            || e.radius_millesimae
                > _millesimae(MUNDUS_OSTII_RADIUS_MAXIMUS))
        {
            redde NIHIL;
        }
        e.scaena  = chorda_transcribere(json_ad_chorda(titulus),
            piscina);
        locus     = (MundusOstium*)xar_addere(index);
        si (!locus)
        {
            redde NIHIL;
        }
        *locus = e;
    }
    redde index;
}

Xar*
mundus_ostia_legere (
    Volumen* volumen,
     chorda  locus,
    Piscina* piscina)
{
    Xar* index;
    Xar* acta;
    i32  k;

    si (!piscina)
    {
        redde NIHIL;
    }
    index = xar_creare(piscina, magnitudo(MundusOstium));
    si (!volumen || !index)
    {
        redde index;
    }
    acta = _acta(volumen, piscina);
    per (k = ZEPHYRUM; acta && k < xar_numerus(acta); k++)
    {
        VolumenActum* actum  = (VolumenActum*)xar_obtinere(acta, k);
        JsonResultus  r      = json_legere(actum->datum, piscina);
           JsonValor* titulus;
                 Xar* novus;

        si (!r.successus || !r.radix || !json_est_objectum(r.radix))
        {
            perge;
        }
        titulus = json_objectum_capere(r.radix, "locus");
        si (   !titulus || !json_est_chorda(titulus)
            || !chorda_aequalis(json_ad_chorda(titulus), locus))
        {
            perge;
        }
        novus = _indicem_legere(json_objectum_capere(r.radix,
            "ostia"), piscina);
        si (novus)
        {
            index = novus;
        }
    }
    redde index;
}

interior b32
_pares (
                      Xar* index,
    constans MundusOstium* ostia,
                      i32  numerus)
{
    i32 k;

    si (!index || xar_numerus(index) != numerus)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans MundusOstium* a =
            (constans MundusOstium*)xar_obtinere(index, k);

        si (   !chorda_aequalis(a->scaena, ostia[k].scaena)
            || a->x_millesimae      != ostia[k].x_millesimae
            || a->z_millesimae      != ostia[k].z_millesimae
            || a->radius_millesimae != ostia[k].radius_millesimae)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
mundus_ostia_ponere (
                  Volumen* volumen,
                   chorda  locus,
    constans MundusOstium* ostia,
                      i32  numerus)
{
      Piscina* piscina;
    JsonValor* objectum;
    JsonValor* tabulatum;
          b32  bene = VERUM;
          i32  k;

    si (   !volumen || locus.mensura == ZEPHYRUM || !locus.datum
        || numerus > MUNDUS_OSTIA_MAXIMA
        || (numerus > ZEPHYRUM && !ostia))
    {
        redde FALSUM;
    }
    piscina = piscina_generare_dynamicum("mundus_ostia", 0x10000);
    si (!piscina)
    {
        redde FALSUM;
    }
    /* index idem iam loci est: nihil scribendum */
    si (!_pares(mundus_ostia_legere(volumen, locus, piscina), ostia,
        numerus))
    {
        objectum   = json_objectum_creare(piscina);
        tabulatum  = json_tabulatum_creare(piscina);
        bene       = objectum && tabulatum;
        per (k = ZEPHYRUM; bene && k < numerus; k++)
        {
            JsonValor* o = json_objectum_creare(piscina);

            si (!o)
            {
                bene = FALSUM;
                frange;
            }
            json_objectum_ponere(o, "x", json_integer_creare(piscina,
                (s64)ostia[k].x_millesimae));
            json_objectum_ponere(o, "z", json_integer_creare(piscina,
                (s64)ostia[k].z_millesimae));
            json_objectum_ponere(o, "radius",
                json_integer_creare(piscina,
                    (s64)ostia[k].radius_millesimae));
            json_objectum_ponere(o, "scaena",
                json_chorda_creare(piscina, ostia[k].scaena));
            json_tabulatum_addere(tabulatum, o);
        }
        si (bene)
        {
            json_objectum_ponere(objectum, "locus",
                json_chorda_creare(piscina, locus));
            json_objectum_ponere(objectum, "ostia", tabulatum);
            bene = volumen_actum_appendere(volumen,
                MUNDUS_OSTIA_ACTUM, json_scribere(objectum, piscina))
                > ZEPHYRUM;
        }
    }
    piscina_destruere(piscina);
    redde bene;
}
