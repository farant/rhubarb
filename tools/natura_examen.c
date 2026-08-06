/* natura_examen.c - iudicium bibliothecae semanticae (facies CLI
 * oneratoris lib/natura.c).
 *
 * RATIO: natura_visus.sh XIV secundis currit - nimis tarde ut
 * uncus post-editionem sit. Onerator XXIX MILLESIMIS currit, id
 * est CCCCLXXX vicibus celerius, ET plus videt (regulae IX-XIII
 * resolutionem hereditatis poscunt quam tabula shell praestare
 * non potest). Unde hoc: porta velox quae in unco vivere potest.
 *
 * MOMENTUM: exemplaria OMNIA semper onerantur, etiam cum una
 * plagula sola iudicatur - regulae II/III/IV trans exemplaria
 * transeunt, et iudicium plagulae solius eas videre NON POTEST.
 * Filtrum (-plagula) post nexuram applicatur, non ante.
 *
 * Usus:
 *   bin/natura_examen                     omnia (natura/)
 *   bin/natura_examen -plagula VIA        diagnostica huius solius
 *   bin/natura_examen -machina            TSV purum
 *   bin/natura_examen -radix DIR          alia sedes
 *
 * Exitus: 0 = sanum; 1 = VULNERA; 2 = NIHIL CURSUM EST
 *   (exitus II disciplina domus: nihil onerari NON est successus,
 *    et porta quae in solo zephyro pendet mendacium tacitum est)
 */
#include "postulata_posix.h"

#include "latina.h"
#include "natura.h"
#include "filum.h"
#include "iter_directoria.h"
#include <stdio.h>
#include <string.h>

#define EXTENSIO       ".genera"
#define EXTENSIO_LONGA VII

interior b32 extensionem_habet(constans chorda* titulus);
interior vacuum stirpem_scribere(constans chorda* titulus,
                                 character* exitus, i32 tectum);
interior b32 viam_congruere(constans chorda* modulus,
                            constans character* plagula);

/* an titulus in ".genera" desinat */
interior b32
extensionem_habet(
    constans chorda* titulus)
{
    si (titulus->mensura <= EXTENSIO_LONGA)
    {
        redde FALSUM;
    }

    redde (b32)(memcmp(titulus->datum +
                       (titulus->mensura - EXTENSIO_LONGA),
                       EXTENSIO,
                       (memoriae_index)EXTENSIO_LONGA) == ZEPHYRUM);
}

/* titulus sine extensione - regula XIV eam contra attributum
 * modulus iudicat */
interior vacuum
stirpem_scribere(
    constans chorda*  titulus,
    character*        exitus,
    i32               tectum)
{
    i32 mensura;

    mensura = titulus->mensura - EXTENSIO_LONGA;
    si (mensura >= tectum)
    {
        mensura = tectum - I;
    }

    memcpy(exitus, titulus->datum, (memoriae_index)mensura);
    exitus[mensura] = '\0';
}

/* an via data hoc exemplar nominet (suffixum sufficit:
 * 'natura/mensura.genera', 'mensura.genera', 'mensura') */
interior b32
viam_congruere(
    constans chorda*     modulus,
    constans character*  plagula)
{
    constans character* basis;
    constans character* punctum;
    size_t              mensura;

    basis = strrchr(plagula, '/');
    basis = basis ? basis + I : plagula;
    punctum = strrchr(basis, '.');
    mensura = punctum ? (size_t)(punctum - basis) : strlen(basis);

    si (mensura != (size_t)modulus->mensura)
    {
        redde FALSUM;
    }

    redde (b32)(memcmp(basis, modulus->datum, mensura) == ZEPHYRUM);
}

s32
principale(
    s32          numerus,
    character**  argumenta)
{
    Piscina*              piscina;
    NaturaBibliotheca*    bib;
    DirectoriumIterator*  iter;
    DirectoriumIntroitus* introitus;
    constans character*   radix;
    constans character*   plagula;
    b32                   machina;
    s32                   i;
    i32                   onerata;
    i32                   vulnera;
    i32                   monita;
    i32                   ostensa;
    i32                   j;

    radix   = "natura";
    plagula = NIHIL;
    machina = FALSUM;

    per (i = I; i < numerus; i++)
    {
        si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-radix") == ZEPHYRUM &&
                     i + I < numerus)
        {
            radix = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-plagula") == ZEPHYRUM &&
                     i + I < numerus)
        {
            plagula = argumenta[++i];
        }
        alioquin
        {
            fprintf(stderr,
                "usus: natura_examen [-plagula VIA] [-machina] [-radix DIR]\n");
            redde II;
        }
    }

    piscina = piscina_generare_dynamicum("natura_examen", 1048576);
    bib     = natura_bibliotheca_creare(piscina);
    si (!bib)
    {
        fprintf(stderr, "natura_examen: bibliotheca creari nequit\n");
        redde II;
    }

    iter = directorium_iterator_aperire(radix, piscina);
    si (!iter)
    {
        fprintf(stderr, "natura_examen: '%s' aperiri nequit\n",
                radix);
        redde II;
    }

    onerata = ZEPHYRUM;
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        character via[DXII];
        character stirps[CCLVI];
        chorda    fons;

        si (introitus->genus != INTROITUS_FILUM ||
            !extensionem_habet(&introitus->titulus))
        {
            perge;
        }

        si ((size_t)introitus->titulus.mensura + strlen(radix) + II >
            magnitudo(via))
        {
            perge;
        }

        sprintf(via, "%s/%.*s", radix,
                (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        stirpem_scribere(&introitus->titulus, stirps,
                         (i32)magnitudo(stirps));

        fons = filum_legere_totum(via, piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "natura_examen: '%s' legi nequit\n", via);
            perge;
        }

        natura_legere(bib, fons, stirps);
        onerata++;
    }
    directorium_iterator_claudere(iter);

    /* NIHIL CURSUM EST - non successus */
    si (onerata == ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_examen: NULLUM exemplar in '%s' inventum "
            "(extensio '%s')\n", radix, EXTENSIO);
        redde II;
    }

    vulnera = natura_nectere(bib);

    /* filtrum POST nexuram: regulae trans exemplaria transeunt */
    monita  = ZEPHYRUM;
    ostensa = ZEPHYRUM;
    per (j = ZEPHYRUM; j < xar_numerus(bib->diagnostica); j++)
    {
        NaturaDiagnosticum* d;
        constans character* gradus;

        d = (NaturaDiagnosticum*)xar_obtinere(bib->diagnostica, j);
        si (d->gradus == NATURA_GRADUS_MONITUM)
        {
            monita++;
        }

        si (plagula && d->modulus &&
            !viam_congruere(d->modulus, plagula))
        {
            perge;
        }
        ostensa++;

        gradus = (d->gradus == NATURA_GRADUS_VULNUS) ?
                 "VULNUS" : "MONITUM";

        si (machina)
        {
            imprimere("%s\t%u\t%.*s\t%.*s\t%s\n", gradus, d->regula,
                d->modulus ? (integer)d->modulus->mensura : 1,
                d->modulus ?
                    (constans character*)d->modulus->datum : "-",
                d->ens ? (integer)d->ens->mensura : 1,
                d->ens ? (constans character*)d->ens->datum : "-",
                d->nuntius);
        }
        alioquin
        {
            imprimere("%-7s regula %2u  [%.*s] %.*s: %s\n", gradus,
                d->regula,
                d->modulus ? (integer)d->modulus->mensura : 1,
                d->modulus ?
                    (constans character*)d->modulus->datum : "-",
                d->ens ? (integer)d->ens->mensura : 1,
                d->ens ? (constans character*)d->ens->datum : "-",
                d->nuntius);
        }
    }

    si (!machina)
    {
        si (ostensa == ZEPHYRUM)
        {
            imprimere("natura_examen: SANUM");
            si (plagula)
            {
                imprimere(" (%s)", plagula);
            }
            imprimere("\n");
        }
        imprimere("  exemplaria %u / genera %u / res %u"
                  " / VULNERA %u / MONITA %u\n",
                  onerata, xar_numerus(bib->genera_omnia),
                  xar_numerus(bib->res_omnes), vulnera, monita);
    }

    redde vulnera > ZEPHYRUM ? I : ZEPHYRUM;
}
