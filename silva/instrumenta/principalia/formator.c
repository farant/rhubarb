/* formator.c - lint et scriptura formae domus (G1 + G2)
 *
 * Usus:
 *   formator <via.c> [viae ...] [-machina] [-scribere]
 *
 * -scribere: emendationes fixabiles applicat et plagulam IN SITU
 * superscribit (portae in machina: series lexematum, custodia
 * spatialis, punctum fixum). Ordines divergentiarum tunc super
 * FRUCTUM referunt (residuae = gradus LINT + non-fixabiles).
 *
 * Regulae: project-specs/formator-regulae.md. Machina in
 * instrumenta/silva_formator.{h,c} (probabilis, silici
 * portabilis) - hic modi et CLI soli, exemplar differre.
 *
 * Exitus: 0 conformis | 1 divergentiae (residuae) | 2 usus,
 * plagula illegibilis, aut scriptura recusata
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "iter_directoria.h"
#include "silva_formator.h"

#include <stdio.h>
#include <string.h>

/* capita directorii praebere (typedef alieni resolvuntur -
 * sine hoc R7 ordines typorum ignotorum exemptos facit) */
interior vacuum
_capita_praebere (
     SilvaContextus* contextus,
           Piscina* piscina,
    constans character* directorium)
{
    DirectoriumIterator*  iter;
    DirectoriumIntroitus* introitus;

    iter = directorium_iterator_aperire(directorium, piscina);
    si (!iter) redde;

    dum ((introitus = directorium_iterator_proximum(iter))
        != NIHIL)
    {
        character via_plena[512];
        character titulus[256];
           chorda textus;
              i32 n;

        si (introitus->genus != INTROITUS_FILUM) perge;
        n = introitus->titulus.mensura;
        si (n < (i32)II || n >= (i32)255
            || introitus->titulus.datum[n - II] != '.'
            || introitus->titulus.datum[n - I] != 'h')
        {
            perge;
        }
        sprintf(titulus, "%.*s", (integer)n,
            (constans character*)introitus->titulus.datum);
        sprintf(via_plena, "%s/%s", directorium, titulus);
        textus = filum_legere_totum(via_plena, piscina);
        si (textus.mensura == (i32)ZEPHYRUM) perge;
        si (strcmp(titulus, "latina.h") == ZEPHYRUM)
        {
            /* custos neutralizatus - typi lexici oraculo
             * visibiles fiunt (vide silva_formator.h) */
            formator_latinam_praebere(contextus, piscina,
                (constans character*)textus.datum,
                textus.mensura);
            perge;
        }
        silva_contextus_praebere(contextus, titulus,
            (constans character*)textus.datum, textus.mensura);
    }
    directorium_iterator_claudere(iter);
}

#define EXCLUSIONES_VIA \
    "silva/probationes/fixa/formatoris/exclusiones.txt"

/* exclusiones: via<TAB>causa (radici-relativae); '#' et lineae
 * vacuae omissae. Exemptio numquam tacita - vocator clamat. */
interior b32
_exempta (
              chorda  exclusiones,
    constans character* via,
              chorda* causa_exitus)
{
    i8* datum;
    i32 n;
    i32 i;
    i32 via_mensura;

    dum (via[ZEPHYRUM] == '.' && via[I] == '/') via += II;
    via_mensura = (i32)strlen(via);

    datum = exclusiones.datum;
    n     = exclusiones.mensura;
    i     = ZEPHYRUM;

    dum (i < n)
    {
        i32 initium;
        i32 finis_lineae;
        i32 positus_tab;

        initium = i;
        dum (i < n && datum[i] != '\n') i += I;
        finis_lineae = i;
        i += I;

        si (finis_lineae == initium) perge;
        si (datum[initium] == '#') perge;

        positus_tab = initium;
        dum (positus_tab < finis_lineae
            && datum[positus_tab] != '\t')
        {
            positus_tab += I;
        }
        si (positus_tab == finis_lineae) perge;

        si (positus_tab - initium == via_mensura
            && memcmp(datum + initium, via,
                (memoriae_index)via_mensura) == ZEPHYRUM)
        {
            causa_exitus->datum   = datum + positus_tab + I;
            causa_exitus->mensura = finis_lineae - positus_tab
                - I;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ordines divergentiarum imprimere; reddit numerum */
interior i32
_divergentias_imprimere (
    constans character* via,
                   Xar* divergentiae,
                   b32  machina)
{
    i32 n;
    i32 j;

    n = divergentiae ? xar_numerus(divergentiae)
        : (i32)ZEPHYRUM;
    per (j = ZEPHYRUM; j < n; j += I)
    {
        FormatorDivergentia* d;

        d = (FormatorDivergentia*)xar_obtinere(divergentiae, j);
        si (machina)
        {
            imprimere("%s\t%u\t%u\t%s\t%d\t%d\t%s\n", via,
                (insignatus integer)d->linea,
                (insignatus integer)d->columna, d->regula,
                (integer)d->inventum,
                (integer)d->exspectatum, d->nuntius);
        }
        alioquin
        {
            imprimere("%s:%u:%u\t%s\t%s (%d pro %d)\n",
                via, (insignatus integer)d->linea,
                (insignatus integer)d->columna, d->regula,
                d->nuntius, (integer)d->inventum,
                (integer)d->exspectatum);
        }
    }
    redde n;
}

integer
principale (
     integer  numerus,
    character** argumenta)
{
    Piscina*        piscina;
    SilvaContextus* contextus;
    chorda          exclusiones;
    b32             machina;
    b32             scriptura;
    b32             recusatio;
    b32             ulla_plagula;
    i32             summa;
    integer         i;

    machina      = FALSUM;
    scriptura    = FALSUM;
    recusatio    = FALSUM;
    ulla_plagula = FALSUM;
    summa        = ZEPHYRUM;

    per (i = I; i < numerus; i += I)
    {
        si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        si (strcmp(argumenta[i], "-scribere") == ZEPHYRUM)
        {
            scriptura = VERUM;
        }
    }

    piscina = piscina_generare_dynamicum("formator", 67108864);
    si (!piscina)
    {
        fprintf(stderr, "formator: piscina fracta\n");
        redde II;
    }

    exclusiones.mensura = ZEPHYRUM;
    exclusiones.datum   = NIHIL;
    si (filum_existit(EXCLUSIONES_VIA))
    {
        exclusiones = filum_legere_totum(EXCLUSIONES_VIA,
            piscina);
    }

    contextus = silva_contextus_creare(piscina);
    si (contextus)
    {
        silva_contextus_latinam_addere(contextus);
        _capita_praebere(contextus, piscina, "include");
        _capita_praebere(contextus, piscina, "silva/fontes");
        _capita_praebere(contextus, piscina,
            "silva/instrumenta");
    }

    si (machina)
    {
        imprimere("# via\tlinea\tcolumna\tregula\tinventum\t"
            "exspectatum\tnuntius\n");
    }

    per (i = I; i < numerus; i += I)
    {
        constans character* via;
                    chorda  textus;

        via = argumenta[i];
        si (via[ZEPHYRUM] == '-') perge;

        si (exclusiones.mensura != (i32)ZEPHYRUM)
        {
            chorda causa;

            si (_exempta(exclusiones, via, &causa))
            {
                fprintf(stderr, "formator: exempta %s (%.*s)\n",
                    via, (integer)causa.mensura,
                    (constans character*)causa.datum);
                ulla_plagula = VERUM;
                perge;
            }
        }

        si (!filum_existit(via))
        {
            fprintf(stderr,
                "formator: plagula illegibilis: %s\n", via);
            piscina_destruere(piscina);
            redde II;
        }
        textus = filum_legere_totum(via, piscina);
        ulla_plagula = VERUM;

        si (scriptura)
        {
            Piscina*         opus;
            FormatorScriptum s;

            opus = piscina_generare_dynamicum("formator-opus",
                67108864);
            si (!opus)
            {
                fprintf(stderr, "formator: piscina fracta\n");
                piscina_destruere(piscina);
                redde II;
            }
            s = formator_scribere(opus, contextus,
                (constans character*)textus.datum,
                textus.mensura);
            si (!s.successus)
            {
                fprintf(stderr,
                    "formator: recusatum %s (%s)\n", via,
                    s.querela);
                recusatio = VERUM;
                piscina_destruere(opus);
                perge;
            }
            si (s.mutatum)
            {
                si (!filum_scribere(via, s.textus))
                {
                    fprintf(stderr,
                        "formator: scriptura fracta %s\n",
                        via);
                    recusatio = VERUM;
                    piscina_destruere(opus);
                    perge;
                }
                fprintf(stderr,
                    "formator: scriptum %s (%u emendationes,"
                    " %u iterationes)\n", via,
                    (insignatus integer)s.applicatae,
                    (insignatus integer)s.iterationes);
            }
            summa += _divergentias_imprimere(via,
                formator_lint(opus, contextus,
                    (constans character*)s.textus.datum,
                    s.textus.mensura), machina);
            piscina_destruere(opus);
        }
        alioquin
        {
            summa += _divergentias_imprimere(via,
                formator_lint(piscina, contextus,
                    (constans character*)textus.datum,
                    textus.mensura), machina);
        }
    }

    si (!ulla_plagula)
    {
        fprintf(stderr, "usus: formator <via.c> [viae ...]"
            " [-machina] [-scribere]\n");
        piscina_destruere(piscina);
        redde II;
    }

    si (!machina)
    {
        si (summa == (i32)ZEPHYRUM)
        {
            imprimere("CONFORMIS\n");
        }
        alioquin
        {
            imprimere("DIVERGENTIAE\t%u\n",
                (insignatus integer)summa);
        }
    }

    piscina_destruere(piscina);
    si (recusatio) redde II;
    redde summa != (i32)ZEPHYRUM ? I : ZEPHYRUM;
}
