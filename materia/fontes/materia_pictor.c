/* materia_pictor.c - Compositor textus diagnostici
 *
 * Ex tools/diagnostica.c motum (E4 specificationis exemplariorum):
 * illic 'interior' sedebat et nihil clientis proprium accipiebat,
 * ergo facies quaeque eum exscriberet - forma vitii 'recentius_ex'
 * quod domus modo quattuor plagulis emendavit.
 *
 * Numeri (summa errata/monita), forma TSV, et scriptura ad stdout in
 * instrumento MANENT: officium instrumenti sunt, non forma.
 */

#include "materia_pictor.h"
#include "chorda_aedificator.h"
#include "excerptum.h"
#include <string.h>


/* ==================================================
 * Sedes ordinare
 * ================================================== */

/* Insertio: sedes paucae sunt (primaria + relatae), et excerptum
 * seriem ORDINATAM poscit - ordinem non reparat, recusat. */
interior vacuum
_sedes_ordinare (
    ExcerptumSedes* sedes,
               i32  numerus)
{
    i32 i;

    per (i = (i32)I; i < numerus; i++)
    {
        ExcerptumSedes cella  = sedes[i];
                   i32 j      = i;

        dum (j > ZEPHYRUM && sedes[j - I].initium > cella.initium)
        {
            sedes[j] = sedes[j - I];
            j--;
        }
        sedes[j] = cella;
    }
}


/* ==================================================
 * Caput
 * ================================================== */

/* 'via:linea:columna: [gravitas] grammatica:codex\n  causa\n'
 *
 * Grammatica codici sine ':' PRAEFIGITUR: codices substrati
 * ('materia:...') et lintris ('lint:...') eum iam ferunt, ergo
 * intacti transeunt et 'crusta:materia:...' numquam nascitur. */
interior b32
_caput_scribere (
           ChordaAedificator* a,
constans MateriaDiagnosticum* d,
          constans character* via,
          constans character* grammatica,
                         b32  monitum)
{
    b32 substrati = (b32)(strchr(d->codex, ':') != NIHIL);

    si (   !chorda_aedificator_appendere_literis(a, via)
        || !chorda_aedificator_appendere_character(a, ':')
        || !chorda_aedificator_appendere_i32(a, d->tractus.linea)
        || !chorda_aedificator_appendere_character(a, ':')
        || !chorda_aedificator_appendere_i32(a, d->tractus.columna)
        || !chorda_aedificator_appendere_literis(a, ": [")
        || !chorda_aedificator_appendere_literis(a,
               monitum ? "monitum" : "erratum")
        || !chorda_aedificator_appendere_literis(a, "] "))
    {
        redde FALSUM;
    }
    si (!substrati && grammatica != NIHIL)
    {
        si (   !chorda_aedificator_appendere_literis(a, grammatica)
            || !chorda_aedificator_appendere_character(a, ':'))
        {
            redde FALSUM;
        }
    }
    redde (b32)(
           chorda_aedificator_appendere_literis(a, d->codex)
        && chorda_aedificator_appendere_literis(a, "\n  ")
        && chorda_aedificator_appendere_literis(a,
               d->causa != NIHIL ? d->causa : "")
        && chorda_aedificator_appendere_character(a, '\n'));
}


/* ==================================================
 * Compositio
 * ================================================== */

chorda
materia_pictor_scribere (
                     Piscina* piscina,
constans MateriaDiagnosticum* d,
          constans character* via,
          constans character* grammatica,
          constans character* fons,
                         i32  mensura,
                         b32  excerptum)
{
    ChordaAedificator* a;
       ExcerptumSedes* sedes;
                  b32  monitum;
                  i32  numerus = (i32)I;
                  i32  r;
               chorda  vacua;

    memset(&vacua, ZEPHYRUM, magnitudo(vacua));
    si (piscina == NIHIL || d == NIHIL || via == NIHIL)
    {
        redde vacua;
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    si (a == NIHIL)
    {
        redde vacua;
    }
    monitum = (b32)(d->gravitas == (s32)MATERIA_GRAVITAS_MONITUM);
    si (!_caput_scribere(a, d, via, grammatica, monitum))
    {
        redde vacua;
    }
    si (   !excerptum || fons == NIHIL
        || d->tractus.initium < ZEPHYRUM)
    {
        redde chorda_aedificator_finire(a);
    }
    sedes = (ExcerptumSedes*)piscina_allocare(piscina,
        (memoriae_index)(d->numerus_relatorum + I)
            * magnitudo(ExcerptumSedes));
    si (sedes == NIHIL)
    {
        redde chorda_aedificator_finire(a);
    }
    sedes[ZEPHYRUM].initium  = d->tractus.initium;
    sedes[ZEPHYRUM].finis    = d->tractus.finis;
    sedes[ZEPHYRUM].linea    = d->tractus.linea;
    sedes[ZEPHYRUM].nota     = d->nota;
    per (r = ZEPHYRUM; r < d->numerus_relatorum; r++)
    {
        constans MateriaTractus* t = &d->relata[r].tractus;

        /* Sedes relata INSANA excerptum TOTUM perderet, quia pictor
         * seriem totam recusat - ergo praetermittitur et primaria
         * superest. */
        si (   t->initium < ZEPHYRUM || t->initium > (s32)mensura
            || t->linea == ZEPHYRUM)
        {
            perge;
        }
        sedes[numerus].initium  = t->initium;
        sedes[numerus].finis    = t->finis;
        sedes[numerus].linea    = t->linea;
        sedes[numerus].nota     = d->relata[r].nota;
        numerus++;
    }
    _sedes_ordinare(sedes, numerus);
    (vacuum)excerptum_scribere_multa(a, fons, mensura, sedes, numerus);
    redde chorda_aedificator_finire(a);
}
