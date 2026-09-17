/* diagnostica.c - Diagnostica plagularum per clientes materiae
 * (tools/diagnostica.sh; materia-sedes-spec par. XI)
 *
 * Usus: diagnostica <plagula...> [-machina] [-sine-excerpto]
 * Suffixum clientem eligit (.sh crusta, .css css). Quaeque plagula:
 * parsura, diagnostica derivata (et emissa), deinde proiectio una -
 * refutatio scriptoris ut materia:scriptura. Exitus: 0 nullum
 * erratum, 1 erratum, 2 nihil iudicatum.
 *
 * Ordo TSV (XII campi): via linea columna linea_finis columna_finis
 * initium finis gravitas codex causa | sedes_relatae nota_primaria.
 * Campus XI sedes relatas fert ut 'L:C-L:C|nota' per ';' iunctas
 * (vacuus si nullae); campus XII notam sedis primariae (vacuus si
 * NIHIL). Campi ADDITI sunt, numquam ordines novi: ordo unus
 * diagnosticum unum manet. Codex CRUDUS in machina; linea humana
 * '<grammatica>:' praefigit codici sine ':' (codices substrati eum
 * iam ferunt).
 */

#include "latina.h"
#include "piscina.h"
#include "chorda_aedificator.h"
#include "excerptum.h"
#include "materia_arbor.h"
#include "materia_diagnostica.h"
#include "materia_lexicon.h"
#include "crusta_arbor.h"
#include "crusta_diagnostica.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include "css_arbor.h"
#include "css_lexicon.h"
#include "css_registrum.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    i32 errata;
    i32 monita;
    i32 plagulae;
} Summa;

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior b32
_suffixum (
    constans character* via,
    constans character* suffixum)
{
    size_t longitudo_viae = strlen(via);
    size_t longitudo_suff = strlen(suffixum);

    redde (b32)(   longitudo_viae >= longitudo_suff
                && strcmp(via + longitudo_viae - longitudo_suff,
                       suffixum) == ZEPHYRUM);
}

/* Nota in TSV: '|', ';', TAB et NOVA LINEA spatio uno mutantur.
 * Damnosum quidem (nota mutilatur), corrumpens numquam: campus
 * numquam in duos scinditur. Notae substrati nihil horum ferunt; nota
 * a regula lintris data quidlibet ferre potest. */
interior vacuum
_notam_scribere (
    constans character* nota)
{
    i32 i;

    si (nota == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; nota[i] != '\0'; i++)
    {
        character c = nota[i];

        si (   c == '|' || c == ';' || c == '\t' || c == '\n'
            || c == '\r')
        {
            c = ' ';
        }
        putchar((integer)c);
    }
}

/* Pictor seriem ORDINATAM poscit et eam non ordinat; recordum sedem
 * primariam seorsum a relatis servat, ergo ordo per constructionem
 * non datur. Numerus minimus est (hodie II), ergo insertio. */
interior vacuum
_sedes_ordinare (
    ExcerptumSedes* sedes,
               i32  numerus)
{
    i32 i;

    per (i = I; i < numerus; i++)
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

interior vacuum
_diagnosticum_imprimere (
                     Piscina* piscina,
          constans character* via,
          constans character* grammatica,
          constans character* fons,
                         i32  mensura,
constans MateriaDiagnosticum* d,
                         b32  machina,
                         b32  excerptum,
                       Summa* summa)
{
    b32 monitum = (b32)(d->gravitas == (s32)MATERIA_GRAVITAS_MONITUM);
    b32 substrati = (b32)(strchr(d->codex, ':') != NIHIL);

    si (monitum)
    {
        summa->monita++;
    }
    alioquin
    {
        summa->errata++;
    }
    si (machina)
    {
        i32 r;

        imprimere("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t", via,
            (integer)d->tractus.linea, (integer)d->tractus.columna,
            (integer)d->tractus.linea_finis,
            (integer)d->tractus.columna_finis,
            (integer)d->tractus.initium, (integer)d->tractus.finis,
            monitum ? "monitum" : "erratum", d->codex, d->causa);
        /* XI: sedes relatae, 'L:C-L:C|nota' per ';' iunctae. ORDINES
         * NOVI NON: ordo unus diagnosticum unum manet, aliter numerus
         * quisque qui TSV quaerit tacite cresceret. */
        per (r = ZEPHYRUM; r < d->numerus_relatorum; r++)
        {
            constans MateriaTractus* t = &d->relata[r].tractus;

            si (r > ZEPHYRUM)
            {
                putchar((integer)';');
            }
            imprimere("%d:%d-%d:%d", (integer)t->linea,
                (integer)t->columna, (integer)t->linea_finis,
                (integer)t->columna_finis);
            si (d->relata[r].nota != NIHIL)
            {
                putchar((integer)'|');
                _notam_scribere(d->relata[r].nota);
            }
        }
        /* XII: nota sedis primariae */
        putchar((integer)'\t');
        _notam_scribere(d->nota);
        putchar((integer)'\n');
        redde;
    }
    imprimere("%s:%d:%d: [%s] %s%s%s\n  %s\n", via,
        (integer)d->tractus.linea, (integer)d->tractus.columna,
        monitum ? "monitum" : "erratum",
        substrati ? "" : grammatica, substrati ? "" : ":", d->codex,
        d->causa);
    si (excerptum && d->tractus.initium >= ZEPHYRUM)
    {
        ChordaAedificator* aedificator;
           ExcerptumSedes* sedes;
                      i32  numerus = I;
                      i32  r;

        sedes = (ExcerptumSedes*)piscina_allocare(piscina,
            (memoriae_index)(d->numerus_relatorum + I)
                * magnitudo(ExcerptumSedes));
        si (sedes == NIHIL)
        {
            redde;
        }
        sedes[ZEPHYRUM].initium  = d->tractus.initium;
        sedes[ZEPHYRUM].finis    = d->tractus.finis;
        sedes[ZEPHYRUM].linea    = d->tractus.linea;
        sedes[ZEPHYRUM].nota     = d->nota;
        per (r = ZEPHYRUM; r < d->numerus_relatorum; r++)
        {
            constans MateriaTractus* t = &d->relata[r].tractus;

            /* Sedes relata insana excerptum TOTUM perderet, quia
             * pictor seriem totam recusat - ergo praetermittitur et
             * primaria superest, ut ante hoc opus. */
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
        aedificator = chorda_aedificator_creare(piscina,
            (memoriae_index)CCLVI);
        si (   aedificator != NIHIL
            && excerptum_scribere_multa(aedificator, fons, mensura,
                   sedes, numerus))
        {
            chorda textus = chorda_aedificator_finire(aedificator);

            fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
        }
    }
}

interior b32
_plagulam_iudicare (
    constans character* via,
                   b32  machina,
                   b32  excerptum,
                 Summa* summa)
{
                   Piscina* piscina;
                 character* fons;
                       i32  mensura = ZEPHYRUM;
       MateriaLexiconRatum  ratum;
        MateriaLexIudicium  iudicium;
     MateriaArborConsilium  consilium;
     MateriaArborScriptura  scriptura;
             CrustaParsura  relatio;
              MateriaNodus* radix        = NIHIL;
                       Xar* diagnostica  = NIHIL;
        constans character* grammatica   = NIHIL;
                       i32  k;

    piscina = piscina_generare_dynamicum("diagnostica", 4194304);
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }
    fons = _plagulam_legere(piscina, via, &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "diagnostica: plagula illegibilis: %s\n", via);
        piscina_destruere(piscina);
        redde FALSUM;
    }
    si (_suffixum(via, ".sh"))
    {
        grammatica = "crusta";
        si (materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON,
                &iudicium))
        {
            materia_arbor_consilium_nudum(&consilium,
                &CRUSTA_REGISTRUM, &ratum, grammatica);
            memset(&relatio, ZEPHYRUM, magnitudo(relatio));
            radix = crusta_arbor_parsare(piscina, fons, mensura,
                &CRUSTA_BASH, &relatio);
            diagnostica = radix != NIHIL
                ? crusta_diagnostica(piscina, radix, &relatio) : NIHIL;
        }
    }
    alioquin si (_suffixum(via, ".css"))
    {
        grammatica = "css";
        si (materia_lexicon_ratum_facere(&ratum, &CSS_LEXICON,
                &iudicium))
        {
            materia_arbor_consilium_nudum(&consilium, &CSS_REGISTRUM,
                &ratum, grammatica);
            radix = css_arbor_parsare(piscina, fons, mensura);
            diagnostica = radix != NIHIL
                ? materia_diagnostica_derivare(piscina, radix,
                      &CSS_REGISTRUM, &CSS_DIAGNOSTICA, NIHIL, NIHIL)
                : NIHIL;
        }
    }
    alioquin
    {
        fprintf(stderr, "diagnostica: suffixum sine cliente: %s\n",
            via);
        piscina_destruere(piscina);
        redde FALSUM;
    }
    si (radix == NIHIL || diagnostica == NIHIL)
    {
        fprintf(stderr,
            "diagnostica: parsura aut derivatio fracta: %s\n", via);
        piscina_destruere(piscina);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(diagnostica); k++)
    {
        _diagnosticum_imprimere(piscina, via, grammatica, fons,
            mensura,
            (constans MateriaDiagnosticum*)xar_obtinere(diagnostica,
                k), machina, excerptum, summa);
    }
    scriptura = materia_arbor_proicere_nodum(piscina, radix,
        &consilium);
    si (!scriptura.successus)
    {
        MateriaDiagnosticum refutatio;

        memset(&refutatio, ZEPHYRUM, magnitudo(refutatio));
        refutatio.gravitas  = (s32)MATERIA_GRAVITAS_ERRATUM;
        refutatio.codex     = MATERIA_CODEX_SCRIPTURA;
        refutatio.causa     = scriptura.causa != NIHIL
            ? scriptura.causa : "scriptura fracta";
        refutatio.tractus   = scriptura.tractus;
        _diagnosticum_imprimere(piscina, via, grammatica, fons,
            mensura, &refutatio, machina, excerptum, summa);
    }
    summa->plagulae++;
    piscina_destruere(piscina);
    redde VERUM;
}

integer
principale (
      integer   argc,
    character** argv)
{
      Summa summa;
        b32 machina    = FALSUM;
        b32 excerptum  = VERUM;
    integer i;

    memset(&summa, ZEPHYRUM, magnitudo(summa));
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[i], "-sine-excerpto") == ZEPHYRUM)
        {
            excerptum = FALSUM;
        }
    }
    per (i = I; i < argc; i++)
    {
        si (argv[i][0] != '-')
        {
            (vacuum)_plagulam_iudicare(argv[i], machina, excerptum,
                &summa);
        }
    }
    fprintf(stderr, "%d diagnostica (erratum %d, monitum %d) in %d "
        "plagulis\n", (integer)(summa.errata + summa.monita),
        (integer)summa.errata, (integer)summa.monita,
        (integer)summa.plagulae);
    si (summa.plagulae == ZEPHYRUM)
    {
        fprintf(stderr, "usus: diagnostica <plagula...> [-machina] "
            "[-sine-excerpto] (nihil iudicatum)\n");
        redde II;
    }
    redde summa.errata > ZEPHYRUM ? I : ZEPHYRUM;
}
