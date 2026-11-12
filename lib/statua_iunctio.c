/*
 * statua_iunctio.c - statuas in unam iungere (statua_iunctio.h)
 *
 * Transitus primus numerat: quae addendae NOVAE sunt (index statuae
 * nondum visus), ubi tabulae cuiusque incipiunt, quae texturae
 * novae. Transitus alter tabulas implet: basis, deinde statua
 * quaeque nova semel, deinde nodi cuiusque addendae.
 */

#include "statua_iunctio.h"
#include <math.h>
#include <string.h>

/* ubi tabulae statuae additae in statua nova incipiunt */
nomen structura {
    constans Statua* statua;
                i32  retia;
                i32  partes;
                i32  vertices;
                i32  indices;
                i32  materiae;
                s32* texturae;    /* index texturae eius -> novus */
} Sedes;

interior b32
_finitus (
    f32 x)
{
    redde x == x && x <= 3.0e38f && x >= -3.0e38f;
}

interior vacuum*
_locus (
           Piscina* piscina,
    memoriae_index  mensura)
{
    vacuum* p = piscina_allocare(piscina, mensura ? mensura : I);

    si (p)
    {
        memset(p, ZEPHYRUM, mensura ? mensura : I);
    }
    redde p;
}

interior b32
_texturae_pares (
    constans Imago* a,
    constans Imago* b)
{
    redde a->latitudo == b->latitudo && a->altitudo == b->altitudo
        && (   a->pixela == b->pixela
            || memcmp(a->pixela, b->pixela,
                   (memoriae_index)a->latitudo * a->altitudo * IV)
                   == ZEPHYRUM);
}

/* texturam in tabula nova invenire aut addere; index eius */
interior s32
_texturam_ponere (
            Statua* s,
    constans Imago* textura)
{
    i32 k;

    per (k = ZEPHYRUM; k < s->texturarum; k++)
    {
        si (_texturae_pares(&s->texturae[k], textura))
        {
            redde (s32)k;
        }
    }
    s->texturae[s->texturarum] = *textura;
    redde (s32)s->texturarum++;
}

/* tabulas statuae additae post tabulas iam positas copiare, indicibus
 * motis */
interior vacuum
_tabulas_addere (
              Statua* s,
     constans Statua* a,
      constans Sedes* sedes)
{
    i32 k;

    per (k = ZEPHYRUM; k < a->retium; k++)
    {
        s->retia[sedes->retia + k]             = a->retia[k];
        s->retia[sedes->retia + k].pars_prima  += sedes->partes;
    }
    per (k = ZEPHYRUM; k < a->partium; k++)
    {
        StatuaPars* pars = &s->partes[sedes->partes + k];

        *pars               = a->partes[k];
        pars->index_primus  += sedes->indices;
        si (pars->materia >= ZEPHYRUM)
        {
            pars->materia += (s32)sedes->materiae;
        }
    }
    si (a->verticum)
    {
        memcpy(s->positiones + sedes->vertices * III, a->positiones,
            a->verticum * III * magnitudo(f32));
        memcpy(s->normales + sedes->vertices * III, a->normales,
            a->verticum * III * magnitudo(f32));
        memcpy(s->coordinatae + sedes->vertices * II, a->coordinatae,
            a->verticum * II * magnitudo(f32));
    }
    per (k = ZEPHYRUM; k < a->indicum; k++)
    {
        s->indices[sedes->indices + k] = a->indices[k]
            + sedes->vertices;
    }
    per (k = ZEPHYRUM; k < a->materiarum; k++)
    {
        StatuaMateria* materia = &s->materiae[sedes->materiae + k];

        *materia = a->materiae[k];
        si (materia->textura >= ZEPHYRUM)
        {
            materia->textura = sedes->texturae[materia->textura];
        }
    }
}

Statua*
statua_coniungere (
            constans Statua* basis,
    constans StatuaIungenda* addendae,
                        i32  numerus,
                    Piscina* piscina)
{
    Statua* s;
     Sedes* sedes;        /* per addendam NOVAM */
       i32* sedes_addendae;
     Sedes  summa;
       i32  novarum    = ZEPHYRUM;
       i32  nodorum;
       i32  texturarum;
       i32  nodus;
       i32  n;
       i32  k;

    si (!basis || !piscina || (numerus && !addendae))
    {
        redde NIHIL;
    }
    per (n = ZEPHYRUM; n < numerus; n++)
    {
        si (   !addendae[n].statua || !_finitus(addendae[n].locus.x)
            || !_finitus(addendae[n].locus.y)
            || !_finitus(addendae[n].locus.z)
            || !_finitus(addendae[n].gyrus))
        {
            redde NIHIL;
        }
    }
    s               = (Statua*)_locus(piscina, magnitudo(Statua));
    sedes           = (Sedes*)_locus(piscina,
        (numerus + I) * magnitudo(Sedes));
    sedes_addendae  = (i32*)_locus(piscina,
        (numerus + I) * magnitudo(i32));
    si (!s || !sedes || !sedes_addendae)
    {
        redde NIHIL;
    }
    /* transitus primus: summae et sedes */
    memset(&summa, ZEPHYRUM, magnitudo(summa));
    summa.retia     = basis->retium;
    summa.partes    = basis->partium;
    summa.vertices  = basis->verticum;
    summa.indices   = basis->indicum;
    summa.materiae  = basis->materiarum;
    nodorum         = basis->nodorum;
    texturarum      = basis->texturarum;
    per (n = ZEPHYRUM; n < numerus; n++)
    {
        constans Statua* a = addendae[n].statua;

        nodorum += I + a->nodorum;
        per (k = ZEPHYRUM; k < novarum; k++)
        {
            si (sedes[k].statua == a)
            {
                frange;
            }
        }
        sedes_addendae[n] = k;
        si (k < novarum)
        {
            perge;
        }
        sedes[k]         = summa;
        sedes[k].statua  = a;
        sedes[k].texturae  = (s32*)_locus(piscina,
            (a->texturarum + I) * magnitudo(s32));
        si (!sedes[k].texturae)
        {
            redde NIHIL;
        }
        novarum++;
        summa.retia     += a->retium;
        summa.partes    += a->partium;
        summa.vertices  += a->verticum;
        summa.indices   += a->indicum;
        summa.materiae  += a->materiarum;
        texturarum      += a->texturarum;
    }
    s->nodi         = (StatuaNodus*)_locus(piscina,
        nodorum * magnitudo(StatuaNodus));
    s->retia        = (StatuaRete*)_locus(piscina,
        summa.retia * magnitudo(StatuaRete));
    s->partes       = (StatuaPars*)_locus(piscina,
        summa.partes * magnitudo(StatuaPars));
    s->positiones   = (f32*)_locus(piscina,
        (memoriae_index)summa.vertices * III * magnitudo(f32));
    s->normales     = (f32*)_locus(piscina,
        (memoriae_index)summa.vertices * III * magnitudo(f32));
    s->coordinatae  = (f32*)_locus(piscina,
        (memoriae_index)summa.vertices * II * magnitudo(f32));
    s->indices      = (i32*)_locus(piscina,
        (memoriae_index)summa.indices * magnitudo(i32));
    s->materiae     = (StatuaMateria*)_locus(piscina,
        summa.materiae * magnitudo(StatuaMateria));
    s->texturae     = (Imago*)_locus(piscina,
        texturarum * magnitudo(Imago));
    si (   !s->nodi || !s->retia || !s->partes || !s->positiones
        || !s->normales || !s->coordinatae || !s->indices
        || !s->materiae || !s->texturae)
    {
        redde NIHIL;
    }
    s->nodorum     = nodorum;
    s->retium      = summa.retia;
    s->partium     = summa.partes;
    s->verticum    = summa.vertices;
    s->indicum     = summa.indices;
    s->materiarum  = summa.materiae;
    /* basis: tabulae ut sunt, indicibus intactis */
    memcpy(s->nodi, basis->nodi,
        basis->nodorum * magnitudo(StatuaNodus));
    memset(&summa, ZEPHYRUM, magnitudo(summa));
    per (k = ZEPHYRUM; k < basis->texturarum; k++)
    {
        s->texturae[k] = basis->texturae[k];
    }
    s->texturarum  = basis->texturarum;
    summa.texturae = (s32*)_locus(piscina,
        (basis->texturarum + I) * magnitudo(s32));
    si (!summa.texturae)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < basis->texturarum; k++)
    {
        summa.texturae[k] = (s32)k;
    }
    _tabulas_addere(s, basis, &summa);
    s->cycli     = basis->cycli;
    s->cyclorum  = basis->cyclorum;
    /* statuae novae: texturae (pares semel), deinde tabulae */
    per (k = ZEPHYRUM; k < novarum; k++)
    {
        constans Statua* a = sedes[k].statua;

        per (n = ZEPHYRUM; n < a->texturarum; n++)
        {
            sedes[k].texturae[n] =
                _texturam_ponere(s, &a->texturae[n]);
        }
        _tabulas_addere(s, a, &sedes[k]);
    }
    /* nodi addendarum: radix nova cum loco et gyro, deinde nodi
     * statuae sub ea */
    nodus = basis->nodorum;
    per (n = ZEPHYRUM; n < numerus; n++)
    {
        constans Statua* a       = addendae[n].statua;
         constans Sedes* locata  = &sedes[sedes_addendae[n]];
            StatuaNodus* radix   = &s->nodi[nodus];
                    f32  dimidium = addendae[n].gyrus
                        * STEREOMETRIA_PI / 360.0f;

        radix->parens      = -I;
        radix->rete        = -I;
        radix->signa       = ZEPHYRUM;
        radix->translatio  = addendae[n].locus;
        radix->rotatio.x   = 0.0f;
        radix->rotatio.y   = (f32)sin((f64)dimidium);
        radix->rotatio.z   = 0.0f;
        radix->rotatio.w   = (f32)cos((f64)dimidium);
        radix->scala       = stereometria_vector(1.0f, 1.0f, 1.0f);
        per (k = ZEPHYRUM; k < a->nodorum; k++)
        {
            StatuaNodus* novus = &s->nodi[nodus + I + k];

            *novus = a->nodi[k];
            novus->parens = novus->parens < ZEPHYRUM ? (s32)nodus
                : novus->parens + (s32)nodus + I;
            si (novus->rete >= ZEPHYRUM)
            {
                novus->rete += (s32)locata->retia;
            }
        }
        nodus += I + a->nodorum;
    }
    redde statua_finire(s, piscina) ? s : NIHIL;
}
