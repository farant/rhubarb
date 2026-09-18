/* materia_excusatio.c - Excusationes declaratae super diagnostica
 *
 * Duo transitus: primus excusationes ex annotationibus legit, alter
 * diagnostica contra eas cribrat. Deinde excusationes quae nihil
 * absorbuerunt (aut sine causa erant, aut fractae, aut codicem
 * ignotum nominabant) diagnostica SUA pariunt.
 *
 * ORDO: exitus (initium, codex) ordinatur ut materia_diagnostica
 * derivare - comparator hic RENASCITUR quia ille 'interior' est;
 * X ordines, ergo ordinatio simplex sufficit.
 */

#include "materia_excusatio.h"
#include "chorda.h"
#include "stml.h"
#include <string.h>


/* ==================================================
 * Excusatio una, ex annotatione lecta
 * ================================================== */

nomen enumeratio {
    EX_VALIDA = 0,    /* tolera cum codice et causa */
    EX_SINE_CAUSA,
    EX_FRACTA,        /* parsari nequit */
    EX_IGNOTA         /* codicem non declaratum nominat */
} ExcusatioStatus;

nomen structura {
      constans MateriaAnnotatio* annotatio;
                constans chorda* codex;   /* NIHIL = attributum abest */
                ExcusatioStatus  status;
                            b32  usus;
} Excusatio;


/* ==================================================
 * Lectio
 * ================================================== */

interior b32
_causam_habet (
    StmlNodus* arbor,
      Piscina* piscina)
{
    chorda textus = stml_textus_normalizatus(arbor, piscina);
       i32 k;

    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (   textus.datum[k] != (i8)' '
            && textus.datum[k] != (i8)'\t'
            && textus.datum[k] != (i8)'\n')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* An codex in tabula declaratorum sit. Praefixum 'lint:' SEMPER
 * transit: registrum regularum nondum exsistit (EX8; signum = cursor
 * regularum), ergo iudicare esset RESPONSUM FALSUM FIDENTER DATUM. */
interior b32
_codex_notus (
                             Piscina* piscina,
    constans MateriaDiagnosticaCocta* declarata,
                     constans chorda* codex)
{
    i32 k;

    si (declarata == NIHIL || codex == NIHIL)
    {
        redde VERUM;
    }
    si (chorda_incipit(*codex, chorda_ex_literis("lint:", piscina)))
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < declarata->numerus; k++)
    {
        si (chorda_aequalis_literis(*codex,
                declarata->tabula[k].codex))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Annotationes -> excusationes. Familiae ALIAE (nid, intentio)
 * TACITE praetereuntur: annotatio inconsumpta excusatio mortua non
 * est. Sed annotatio FRACTA nominatur quaecumque familia futura
 * erat - tag aperuit et cecidit, quod vitium est cuiuscumque. */
interior Xar*
_excusationes_legere (
                             Piscina* piscina,
                        constans Xar* annotationes,
    constans MateriaDiagnosticaCocta* declarata)
{
    Xar* exitus = xar_creare(piscina, (i32)magnitudo(Excusatio));
    i32  k;

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(annotationes); k++)
    {
        constans MateriaAnnotatio* a =
            (constans MateriaAnnotatio*)xar_obtinere(annotationes, k);
        Excusatio* e;

        si (   a->parsata
            && (   a->arbor == NIHIL
                || a->arbor->titulus == NIHIL
                || !chorda_aequalis_literis(*a->arbor->titulus,
                       MATERIA_EXCUSATIO_TAG)))
        {
            perge;
        }
        e = (Excusatio*)xar_addere(exitus);
        si (e == NIHIL)
        {
            redde NIHIL;
        }
        memset(e, ZEPHYRUM, magnitudo(*e));
        e->annotatio = a;
        si (!a->parsata)
        {
            e->status = EX_FRACTA;
            perge;
        }
        e->codex = stml_attributum_capere(a->arbor, "codex");
        si (!_causam_habet(a->arbor, piscina))
        {
            e->status = EX_SINE_CAUSA;
        }
        alioquin si (!_codex_notus(piscina, declarata, e->codex))
        {
            e->status = EX_IGNOTA;
        }
    }
    redde exitus;
}


/* ==================================================
 * Cribratio
 * ================================================== */

/* An excusatio hoc diagnosticum tegat: sedes PRIMARIA intra scopum,
 * codices AEQUALES. Sedes relatae non participant (SM1). */
interior b32
_tegit (
          constans Excusatio* e,
constans MateriaDiagnosticum* d)
{
    constans MateriaTractus* s;

    si (   e->status != EX_VALIDA
        || e->codex  == NIHIL
        || d->codex  == NIHIL)
    {
        redde FALSUM;
    }
    s = &e->annotatio->scopus;
    si (s->initium < ZEPHYRUM || d->tractus.initium < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (   d->tractus.initium < s->initium
        || d->tractus.finis > s->finis)
    {
        redde FALSUM;
    }
    redde chorda_aequalis_literis(*e->codex, d->codex);
}

interior b32
_prius (
    constans MateriaDiagnosticum* a,
    constans MateriaDiagnosticum* b)
{
    si (a->tractus.initium != b->tractus.initium)
    {
        redde (b32)(a->tractus.initium < b->tractus.initium);
    }
    redde (b32)(strcmp(a->codex, b->codex) < ZEPHYRUM);
}

/* ordinatio stabilis: ordo inventionis paribus servatur */
interior vacuum
_ordinare (
    Xar* in)
{
    i32 k;

    per (k = (i32)I; k < xar_numerus(in); k++)
    {
        i32 j = k;

        dum (j > ZEPHYRUM)
        {
            MateriaDiagnosticum* a =
                (MateriaDiagnosticum*)xar_obtinere(in, j - I);
            MateriaDiagnosticum* b =
                (MateriaDiagnosticum*)xar_obtinere(in, j);
            MateriaDiagnosticum t;

            si (!_prius(b, a))
            {
                frange;
            }
            t   = *a;
            *a  = *b;
            *b  = t;
            j--;
        }
    }
}

interior constans character*
_codex_vitii (
    ExcusatioStatus status)
{
    commutatio (status)
    {
    casus EX_SINE_CAUSA:
        redde MATERIA_CODEX_EXCUSATIO_SINE_CAUSA;
    casus EX_FRACTA:
        redde MATERIA_CODEX_EXCUSATIO_FRACTA;
    casus EX_IGNOTA:
        redde MATERIA_CODEX_EXCUSATIO_IGNOTA;
    ordinarius:
        redde MATERIA_CODEX_EXCUSATIO_MORTUA;
    }
}

interior constans character*
_causa_vitii (
    ExcusatioStatus status)
{
    commutatio (status)
    {
    casus EX_SINE_CAUSA:
        redde "excusatio sine causa: causa necessaria est";
    casus EX_FRACTA:
        redde "excusatio parsari nequit";
    casus EX_IGNOTA:
        redde "excusatio codicem non declaratum nominat";
    ordinarius:
        redde "excusatio nihil absorbuit";
    }
}

Xar*
materia_excusatio_applicare (
                             Piscina* piscina,
                        constans Xar* diagnostica,
                        constans Xar* annotationes,
    constans MateriaDiagnosticaCocta* declarata)
{
    Xar* exitus;
    Xar* excusationes;
    i32  k;
    i32  j;

    si (piscina == NIHIL || diagnostica == NIHIL)
    {
        redde NIHIL;
    }
    exitus = xar_creare(piscina, (i32)magnitudo(MateriaDiagnosticum));
    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    si (annotationes == NIHIL || xar_numerus(annotationes) == ZEPHYRUM)
    {
        excusationes = xar_creare(piscina, (i32)magnitudo(Excusatio));
    }
    alioquin
    {
        excusationes = _excusationes_legere(piscina, annotationes,
            declarata);
    }
    si (excusationes == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(diagnostica); k++)
    {
        constans MateriaDiagnosticum* d =
            (constans MateriaDiagnosticum*)xar_obtinere(diagnostica,
                k);
        b32 excusatum = FALSUM;

        per (j = ZEPHYRUM; j < xar_numerus(excusationes); j++)
        {
            Excusatio* e = (Excusatio*)xar_obtinere(excusationes, j);

            si (_tegit(e, d))
            {
                e->usus    = VERUM;
                excusatum  = VERUM;
            }
        }
        si (!excusatum)
        {
            MateriaDiagnosticum* c =
                (MateriaDiagnosticum*)xar_addere(exitus);

            si (c == NIHIL)
            {
                redde NIHIL;
            }
            *c = *d;
        }
    }
    per (j = ZEPHYRUM; j < xar_numerus(excusationes); j++)
    {
        constans Excusatio* e =
            (constans Excusatio*)xar_obtinere(excusationes, j);
        MateriaDiagnosticum* c;

        si (e->status == EX_VALIDA && e->usus)
        {
            perge;
        }
        c = (MateriaDiagnosticum*)xar_addere(exitus);
        si (c == NIHIL)
        {
            redde NIHIL;
        }
        memset(c, ZEPHYRUM, magnitudo(*c));
        c->gravitas  = (s32)MATERIA_GRAVITAS_ERRATUM;
        c->codex     = _codex_vitii(e->status);
        c->causa     = _causa_vitii(e->status);
        c->nodus     = e->annotatio->nodus;
        c->tractus   = e->annotatio->commentarium;
    }
    _ordinare(exitus);
    redde exitus;
}
