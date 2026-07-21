/* aedilis.c - AEDILIS: stratum constructionis (spica, phasis 0)
 *
 * Magistratus operum publicorum: plagulae datae clausuram
 * dependentiarum veram derivare et scripta fabricae emittere.
 * Spica haec plagulam UNAM parsat (silva) et imprimit:
 *   - directivas inclusionum (quales expansio vidit)
 *   - annotationes "aedilis:" (ex fluxu crudo lexematum)
 *   - tempora (parsatio, lexatio cruda)
 *
 * Spec: project-specs/aedilis-spec-v2.md; parcum 01KXJ2HV.
 * Postura defectus: RECUSARE CLAMOSE (nihil emissum, causa
 * nominata, exitus 1).
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"

#include "silva.h"

#include <stdio.h>
#include <time.h>

/* Annotatio ANCORATA: commentum post delimitatorem et spatia cum
 * "aedilis:" INCIPERE debet - quaestio substring commenta quae de
 * annotationibus loquuntur (prosa) falso caperet (inventum spicae:
 * fixtura ipsa se cepit) */
interior b32
_annotatio_est (constans i8* datum, i32 mensura)
{
    constans character* signum = "aedilis:";
    i32 i;
    i32 j;

    i = ZEPHYRUM;
    si (mensura >= II && datum[ZEPHYRUM] == (i8)'/'
        && (datum[I] == (i8)'*' || datum[I] == (i8)'/'))
    {
        i = II;
    }
    dum (i < mensura
        && (datum[i] == (i8)' ' || datum[i] == (i8)'\t'))
    {
        i++;
    }
    per (j = ZEPHYRUM; signum[j] != '\0'; j++)
    {
        si (i + j >= mensura || datum[i + j] != (i8)signum[j])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior f64
_horologium_ms (clock_t initium, clock_t finis)
{
    redde (f64)(finis - initium) * 1000.0 / (f64)CLOCKS_PER_SEC;
}

s32
principale (s32 numerus_argumentorum, character** argumenta)
{
    Piscina*            piscina;
    SilvaPiscina*       arboris;
    SilvaContextus*     contextus;
    SilvaParsura*       parsura;
    SilvaXar*           cruda;
    chorda              fons;
    constans character* via;
    clock_t             initium;
    clock_t             finis;
    f64                 tempus_parsandi;
    f64                 tempus_crudae;
    f64                 chiliocteti;
    insignatus integer  n;
    insignatus integer  k;
    insignatus integer  numeratae;

    si (numerus_argumentorum < II)
    {
        fprintf(stderr, "usus: aedilis <plagula.c>\n");
        redde I;
    }
    via = argumenta[I];

    piscina = piscina_generare_dynamicum("aedilis", 4194304);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "AEDILIS RECUSAT: piscina deest\n");
        redde I;
    }
    fons = filum_legere_totum(via, piscina);
    si (fons.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "AEDILIS RECUSAT: plagula non lecta: %s\n",
            via);
        redde I;
    }

    arboris = silva_piscina_generare_dynamicum("aedilis_arbor",
        8388608);
    si (arboris == NIHIL)
    {
        fprintf(stderr, "AEDILIS RECUSAT: piscina silvae deest\n");
        redde I;
    }
    contextus = silva_contextus_creare(arboris);
    si (contextus == NIHIL
        || !silva_contextus_latinam_addere(contextus))
    {
        fprintf(stderr, "AEDILIS RECUSAT: contextus deest\n");
        redde I;
    }

    initium = clock();
    parsura = silva_c89_parsare_cum_contextu(arboris, contextus,
        via, (constans character*)fons.datum, fons.mensura, NIHIL);
    finis = clock();
    tempus_parsandi = _horologium_ms(initium, finis);

    si (parsura == NIHIL || parsura->expansio == NIHIL)
    {
        fprintf(stderr, "AEDILIS RECUSAT: parsura defecit: %s\n",
            via);
        redde I;
    }

    chiliocteti = (f64)fons.mensura / 1024.0;
    imprimere("AEDILIS spica 0\n");
    imprimere("plagula: %s (%.1f KB)\n", via, chiliocteti);
    imprimere("parsura: successus=%d errores=%u"
        " tempus=%.2f ms (%.3f ms/KB)\n",
        parsura->successus, parsura->numerus_errorum,
        tempus_parsandi, tempus_parsandi / chiliocteti);

    imprimere("directivae inclusionum:\n");
    numeratae = ZEPHYRUM;
    n = silva_inclusiones_numerus(parsura->expansio);
    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaInclusioVista vista;

        si (!silva_inclusio_vista(parsura->expansio, k, &vista))
        {
            perge;
        }
        si (vista.fons_ex != parsura->fons_princeps
            || vista.via == NIHIL)
        {
            perge;
        }
        imprimere("  %.*s%s\n", (s32)vista.via->mensura,
            (constans character*)vista.via->datum,
            vista.est_praetermissa ? "  [praetermissa]" : "");
        numeratae++;
    }
    imprimere("  summa: %u\n", numeratae);

    initium = clock();
    cruda = silva_lexare_cruda(arboris,
        (constans character*)fons.datum, fons.mensura, ZEPHYRUM);
    finis = clock();
    tempus_crudae = _horologium_ms(initium, finis);
    si (cruda == NIHIL)
    {
        fprintf(stderr, "AEDILIS RECUSAT: lexatio cruda defecit\n");
        redde I;
    }

    imprimere("annotationes aedilis:\n");
    numeratae = ZEPHYRUM;
    n = silva_xar_numerus(cruda);
    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaToken* lexema;

        lexema = *(SilvaToken**)silva_xar_obtinere(cruda, k);
        si (lexema == NIHIL
            || (lexema->genus != SILVA_LEX_COMMENTUM_CLAUSUM
                && lexema->genus != SILVA_LEX_COMMENTUM_LINEA))
        {
            perge;
        }
        si (!_annotatio_est(lexema->valor.datum,
                lexema->valor.mensura))
        {
            perge;
        }
        imprimere("  linea %u: %.*s\n", lexema->linea,
            (s32)lexema->valor.mensura,
            (constans character*)lexema->valor.datum);
        numeratae++;
    }
    imprimere("  summa: %u\n", numeratae);
    imprimere("lexatio cruda: %.2f ms (%u lexemata)\n",
        tempus_crudae, silva_xar_numerus(cruda));

    redde ZEPHYRUM;
}
