/* arbor.c - plagulam C in STML canonicum vertere, in exitum standardem
 *
 * Usus:
 *   ./silva/arbor.sh <plagula.c> [-nudum] [-tacitus]
 *
 * Clausura inclusionum PRAEBETUR ex 'bin/aedilis --partes' nisi
 * '-nudum' datur. Id refert: sine clausura latina.h numquam videtur,
 * 'si' identificator MANET, et documentum codicis latinizati nihil
 * de expansione dicit (vide portam apparatus in
 * probatio_silva_arbor_plagula.c - haec ipsa fallacia bis incendit).
 *
 * ORACULUM NIHIL est, sicut in porta M2: ergo ambiguitas MAXIMA
 * retinetur. Documentum plagulae oraculo instructae multo minus
 * ambiguorum fert (mensuratum: lib/chorda.c CCCXCII contra III).
 *
 * Exit: 0 scriptum | 1 fractum | 2 usus
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_c89.h"
#include "silva_parsare.h"
#include "silva_expandere.h"
#include "silva_scribere.h"
#include "silva_c89_oraculum.h"
#include "silva_arbor.h"
/* Dependentiae NEXUS obiectorum silvae (per aedilem in indicem
 * fontium fluunt - vide emitte.c pro eodem more). Hic non
 * adhibentur; silva ipsa eis nititur. */
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "friatio.h"
#include "selectio.h"
#include "similitudo.h"
#include "stml.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLAUSURA_MAXIMA 96
#define VIA_MAXIMA 512

interior i8*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_exitus)
{
              FILE* pl;
                i8* buffer;
    signatus longus  mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);
    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (   mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
               != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    *mensura_exitus = (i32)mensura;
    redde buffer;
}

/* Clausuram capitum domesticorum ex aedile petere; numerum reddit. */
interior i32
_clausuram_petere (
    constans character* via_relativa,
              character series[CLAUSURA_MAXIMA][VIA_MAXIMA])
{
    character mandatum[1024];
    character linea[VIA_MAXIMA + 64];
        FILE* tubus;
         i32  numerus;

    numerus = ZEPHYRUM;
    sprintf(mandatum, "./bin/aedilis '%s' --partes 2>/dev/null",
        via_relativa);
    tubus = popen(mandatum, "r");
    si (tubus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    dum (fgets(linea, (integer)magnitudo(linea), tubus) != NIHIL)
    {
        memoriae_index m;

        si (linea[ZEPHYRUM] != 'C' || linea[I] != '\t')
        {
            perge;
        }
        m = strlen(linea);
        dum (m > ZEPHYRUM
             && (linea[m - I] == '\n' || linea[m - I] == '\r'))
        {
            linea[m - I] = '\0';
            m--;
        }
        si (m <= II || numerus >= CLAUSURA_MAXIMA
            || m - II >= VIA_MAXIMA)
        {
            perge;
        }
        strcpy(series[numerus], linea + II);
        numerus++;
    }
    pclose(tubus);
    redde numerus;
}

s32
principale (
             integer  numerus_argumentorum,
    constans character* argumenta[])
{
                Piscina* piscina;
     constans character* via;
                     b32 nudum;
                     b32 tacitus;
                     i32 i;
                     i8* fons;
                     i32 mensura;
           SilvaParsura* parsura;
    SilvaArborScriptura  scriptura;
               character clausura[CLAUSURA_MAXIMA][VIA_MAXIMA];
                     i32 numerus_clausurae;
                     i32 expansa;

    via      = NIHIL;
    nudum    = FALSUM;
    tacitus  = FALSUM;

    per (i = I; i < (i32)numerus_argumentorum; i++)
    {
        si (strcmp(argumenta[i], "-nudum") == ZEPHYRUM)
        {
            nudum = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-tacitus") == ZEPHYRUM)
        {
            tacitus = VERUM;
        }
        alioquin si (via == NIHIL)
        {
            via = argumenta[i];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr,
            "usus: arbor <plagula.c> [-nudum] [-tacitus]\n"
            "  -nudum    clausuram inclusionum NON praebere\n"
            "  -tacitus  compendium in errorem non scribere\n");
        redde II;
    }

    piscina = piscina_generare_dynamicum("arbor", 67108864);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "arbor: piscina creari non potuit\n");
        redde I;
    }

    fons = _plagulam_legere(piscina, via, &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "arbor: plagula non lecta: %s\n", via);
        piscina_destruere(piscina);
        redde I;
    }

    numerus_clausurae = ZEPHYRUM;
    si (nudum)
    {
        parsura = silva_parsare(piscina, via,
            (constans character*)fons, mensura, &SILVA_C89_GRAMMATICA,
            NIHIL, NIHIL, NIHIL);
    }
    alioquin
    {
        SilvaExpansio* expansio;

        expansio = silva_expansio_creare(piscina);
        numerus_clausurae = _clausuram_petere(via, clausura);
        per (i = ZEPHYRUM; i < numerus_clausurae; i++)
        {
            i8* textus;
            i32 m_caput;

            textus = _plagulam_legere(piscina, clausura[i], &m_caput);
            si (textus == NIHIL)
            {
                perge;
            }
            silva_includendum_praebere(expansio, clausura[i],
                (constans character*)textus, m_caput);
        }
        parsura = silva_parsare_cum_expansione(piscina, expansio, via,
            (constans character*)fons, mensura, &SILVA_C89_GRAMMATICA,
            NIHIL, NIHIL, NIHIL);
    }

    si (parsura == NIHIL || parsura->commissio == NIHIL)
    {
        fprintf(stderr, "arbor: parsura fracta: %s\n", via);
        piscina_destruere(piscina);
        redde I;
    }

    scriptura = silva_arbor_scribere_parsuram(piscina, parsura,
        &SILVA_C89_REGISTRUM, "c89", parsura->fons_princeps, NIHIL);
    si (!scriptura.successus)
    {
        fprintf(stderr, "arbor: scriptura recusata: %s\n",
            scriptura.causa ? scriptura.causa : "(sine causa)");
        piscina_destruere(piscina);
        redde I;
    }

    fwrite(scriptura.textus.datum, I,
        (memoriae_index)scriptura.textus.mensura, stdout);

    si (!tacitus)
    {
        expansa = ZEPHYRUM;
        si (parsura->lexemata != NIHIL)
        {
            i32 quantum = xar_numerus(parsura->lexemata);

            per (i = ZEPHYRUM; i < quantum; i++)
            {
                SilvaToken** sedes = (SilvaToken**)
                    xar_obtinere(parsura->lexemata, i);

                si (   sedes != NIHIL && *sedes != NIHIL
                    && (*sedes)->origo.genus != SILVA_ORIGO_FONS)
                {
                    expansa++;
                }
            }
        }
        fprintf(stderr,
            "arbor: %s -> %d octeti (fons %d, %.1fx)\n"
            "  capita praebita:  %d%s\n"
            "  lexemata:         %d (expansa %d)\n"
            "  ambigui:          %d\n"
            "  errores:          %d  (nodi ERROR in arbore)\n"
            "  compressio:       spatia %d | folia %d def / %d voc"
            " | parametra %d/%d\n",
            via, (integer)scriptura.textus.mensura, (integer)mensura,
            mensura > ZEPHYRUM
                ? (duplex)scriptura.textus.mensura / (duplex)mensura
                : 0.0,
            (integer)numerus_clausurae,
            nudum ? " (-nudum: NIHIL praebitum)" : "",
            (integer)(parsura->lexemata
                ? xar_numerus(parsura->lexemata) : ZEPHYRUM),
            (integer)expansa,
            (integer)(parsura->commissio->ambigui
                ? xar_numerus(parsura->commissio->ambigui)
                : ZEPHYRUM),
            (integer)parsura->numerus_errorum,
            (integer)scriptura.census.spatia_vocationes,
            (integer)scriptura.census.folia_formae,
            (integer)scriptura.census.folia_vocationes,
            (integer)scriptura.census.parametra_compressa,
            (integer)scriptura.census.parametra_visa);
    }

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
