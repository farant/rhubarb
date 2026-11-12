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
#include <string.h>

#define ERRORIS_MENSURA  CCLVI
/* summum rei solidae tantum infra solum iacere potest et tamen
 * cellam claudere (altitudine pari non gressibile vincit) */
#define PARITAS          1.0e-4f

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

interior b32
_rem_legere (
                Lector* l,
    constans StmlNodus* nodus,
        MundusLocusRes* res)
{
    hic_manens constans character* constans LICITA[] = {
        "fons", "locus", "gyrus", NIHIL
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
    redde VERUM;
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
    f.res  = (MundusLocusRes*)piscina_allocare(piscina,
        (liberi + I) * magnitudo(MundusLocusRes));
    si (!f.res)
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
        si (   liber->genus != STML_NODUS_ELEMENTUM || !liber->titulus
            || !chorda_aequalis_literis(*liber->titulus, "res"))
        {
            (vacuum)_recusare(&l, liber->linea, "unknown element <",
                liber->titulus, "> (only <res> is allowed here)");
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
        iungendae[k].gyrus   = res[k].gyrus;
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
