/* ambigua.c - census ambiguitatis retentae trans corpus
 *
 * Instrumentum gradus II oraculi classificationis (01M0D4QV5S):
 * fidelitas byte-exacta classificationem non videt - arbor male
 * classificata bytes intactos emittit. Hic numerus ambiguorum
 * RETENTORUM (post resolutionem, commissio->ambigui) per plagulam
 * mensuratur, contextu domus (lexicon latinae + capita praebita) -
 * salus classificationis ut numerus, regressus contra pinnam
 * CLAMAT.
 *
 * Usus: ambigua <via.c> [viae ...] [-machina] [-loca]
 *               [-pinna <via.tsv>]
 * Exitus: 0 mensuratum (sine regressu) | 1 regressus contra pinnam
 *         | 2 usus / apparatus
 *
 * Columnae (-machina): via, ambigua (extenta in plagula IPSA),
 * aliena (in contento incluso - per includentem repetita!),
 * errores (nodi erroris trans unitatem totam). -loca: ordines
 * per ambiguum singulum (via, linea, columna) pro inspectione.
 * Pinna: ordines 'via<TAB>ambigua' ('#' omissa); via mensurata
 * absens a pinna = fundamentum nullum (ambigua ulla = REGRESSUS
 * NOVUS).
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "iter_directoria.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_commissio.h"
#include "silva_c89_oraculum.h"

#include <stdio.h>
#include <string.h>

/* capita directorii praebere (exemplar formator.c CLI) */
interior vacuum
_capita_praebere (
        SilvaContextus* contextus,
               Piscina* piscina,
    constans character* directorium)
{
     DirectoriumIterator* iter;
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
        si (   n < (i32)II || n >= (i32)255
            || introitus->titulus.datum[n - II] != '.'
            || introitus->titulus.datum[n - I]  != 'h')
        {
            perge;
        }
        sprintf(titulus, "%.*s", (integer)n,
            (constans character*)introitus->titulus.datum);
        sprintf(via_plena, "%s/%s", directorium, titulus);
        textus = filum_legere_totum(via_plena, piscina);
        si (textus.mensura == (i32)ZEPHYRUM) perge;
        silva_contextus_praebere(contextus, titulus,
            (constans character*)textus.datum, textus.mensura);
    }
    directorium_iterator_claudere(iter);
}

/* numerus decimalis ex [a, b) - cifrae ducentes solae */
interior i32
_numerum_legere (
     constans i8* datum,
             i32  a,
             i32  b)
{
    i32 n;

    n = ZEPHYRUM;
    dum (a < b && datum[a] >= '0' && datum[a] <= '9')
    {
        n = n * (i32)10 + (i32)(datum[a] - '0');
        a += I;
    }
    redde n;
}

/* numerus pinnatus viae; -1 = via in pinna absens. Ordines
 * 'via<TAB>numerus...' lineatim, '#' ducens = commentarium. */
interior s32
_pinna_numerus (
                 chorda  pinna,
     constans character* via)
{
    i32 mensura_viae;
    i32 a;

    mensura_viae  = (i32)strlen(via);
    a             = ZEPHYRUM;
    dum (a < pinna.mensura)
    {
        i32 b;

        b = a;
        dum (b < pinna.mensura && pinna.datum[b] != '\n') b += I;
        si (   pinna.datum[a]                != '#'
            && b - a > mensura_viae
            && pinna.datum[a + mensura_viae] == '\t'
            && memcmp(pinna.datum + a, via,
                   (memoriae_index)mensura_viae) == ZEPHYRUM)
        {
            redde (s32)_numerum_legere(pinna.datum,
                a + mensura_viae + I, b);
        }
        a = b + I;
    }
    redde -I;
}

integer
principale (
               integer   numerus,
    constans character** argumenta)
{
               Piscina* piscina;
        SilvaContextus* contextus;
                   b32  machina;
                   b32  loca;
    constans character* pinna_via;
                chorda  pinna;
                   b32  ulla_plagula;
                   b32  regressus;
                   i32  plagulae;
                   i32  ambigua_summa;
                   i32  aliena_summa;
                   i32  errores_summa;
               integer  i;

    machina    = FALSUM;
    loca       = FALSUM;
    pinna_via  = NIHIL;
    per (i = I; i < numerus; i += I)
    {
        si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-loca") == ZEPHYRUM)
        {
            loca = VERUM;
        }
        alioquin si (   strcmp(argumenta[i], "-pinna") == ZEPHYRUM
                     && i + I < numerus)
        {
            i          += I;
            pinna_via  = argumenta[i];
        }
    }

    piscina = piscina_generare_dynamicum("ambigua", 67108864);
    si (!piscina)
    {
        fprintf(stderr, "ambigua: piscina fracta\n");
        redde II;
    }

    pinna.datum    = NIHIL;
    pinna.mensura  = ZEPHYRUM;
    si (pinna_via != NIHIL)
    {
        si (!filum_existit(pinna_via))
        {
            fprintf(stderr,
                "ambigua: pinna illegibilis: %s\n", pinna_via);
            piscina_destruere(piscina);
            redde II;
        }
        pinna = filum_legere_totum(pinna_via, piscina);
    }

    contextus = silva_contextus_creare(piscina);
    si (contextus)
    {
        /* lexicon systematis ANTE latinam: typedefs systematis
         * (size_t...) et functiones (free...) praeonerantur -
         * classis 'liberare(x);' T(x)-ambigua solvitur, et
         * 'nomen size_t memoriae_index;' latinae typum notum
         * videt (cascata ordine) */
        chorda systema;
        chorda posix;

        systema = filum_legere_totum(
            "silva/fontes/systema_c89.h", piscina);
        si (systema.mensura > (i32)ZEPHYRUM)
        {
            silva_contextus_lexicon_addere(contextus,
                "systema_c89.h",
                (constans character*)systema.datum,
                systema.mensura);
        }
        /* posix quoque - census scientiam MAXIMAM vult (residuum
         * = quod lexicon sanare NEQUIT); examen contra per
         * plagulam componit (postulata) - iudicia diversa,
         * missiones diversae */
        posix = filum_legere_totum(
            "silva/fontes/systema_posix.h", piscina);
        si (posix.mensura > (i32)ZEPHYRUM)
        {
            silva_contextus_lexicon_addere(contextus,
                "systema_posix.h",
                (constans character*)posix.datum,
                posix.mensura);
        }
        silva_contextus_latinam_addere(contextus);
        _capita_praebere(contextus, piscina, "include");
        _capita_praebere(contextus, piscina, "silva/fontes");
        _capita_praebere(contextus, piscina,
            "silva/instrumenta");
    }

    si (machina)
    {
        si (loca)
        {
            imprimere("# via\tlinea\tcolumna\n");
        }
        alioquin
        {
            imprimere("# via\tambigua\taliena\terrores\n");
        }
    }

    ulla_plagula   = FALSUM;
    regressus      = FALSUM;
    plagulae       = ZEPHYRUM;
    ambigua_summa  = ZEPHYRUM;
    aliena_summa   = ZEPHYRUM;
    errores_summa  = ZEPHYRUM;

    per (i = I; i < numerus; i += I)
    {
        constans character* via;

        via = argumenta[i];
        si (via[ZEPHYRUM] == '-')
        {
            si (strcmp(via, "-pinna") == ZEPHYRUM) i += I;
            perge;
        }
        si (!filum_existit(via))
        {
            fprintf(stderr,
                "ambigua: plagula illegibilis: %s\n", via);
            piscina_destruere(piscina);
            redde II;
        }
        ulla_plagula = VERUM;

        /* piscina operis PER PLAGULAM - lectio formatoris
         * 2026-08-19 (cursus corporis piscinam longaevam
         * exhauriebat, mors tacita) */
        {
                 Piscina* opus;
                  chorda  textus;
            SilvaParsura* parsura;
                     i32  propria;
                     i32  aliena;
                     i32  m;
                     i32  k;

            opus = piscina_generare_dynamicum("ambigua-opus",
                67108864);
            si (!opus)
            {
                fprintf(stderr, "ambigua: piscina fracta\n");
                piscina_destruere(piscina);
                redde II;
            }
            textus = filum_legere_totum(via, opus);
            parsura = silva_c89_parsare_cum_contextu(opus,
                contextus, via,
                (constans character*)textus.datum,
                textus.mensura, NIHIL);
            si (parsura == NIHIL || parsura->commissio == NIHIL)
            {
                fprintf(stderr,
                    "ambigua: parsura fracta: %s\n", via);
                piscina_destruere(opus);
                piscina_destruere(piscina);
                redde II;
            }

            propria  = ZEPHYRUM;
            aliena   = ZEPHYRUM;
            m        = xar_numerus(parsura->commissio->ambigui);
            per (k = ZEPHYRUM; k < m; k += I)
            {
                SilvaNodus* ambiguum =
                    *(SilvaNodus**)xar_obtinere(
                        parsura->commissio->ambigui, k);
                i32 la;
                i32 ca;
                i32 lb;
                i32 cb;

                silva_nodus_extensionem_lineis(ambiguum,
                    parsura->fons_princeps, &la, &ca, &lb, &cb);
                si (la != (i32)ZEPHYRUM)
                {
                    propria += I;
                    si (loca)
                    {
                        si (machina)
                        {
                            imprimere("%s\t%u\t%u\n", via,
                                (insignatus integer)la,
                                (insignatus integer)ca);
                        }
                        alioquin
                        {
                            imprimere("    %s:%u:%u\n", via,
                                (insignatus integer)la,
                                (insignatus integer)ca);
                        }
                    }
                }
                alioquin
                {
                    aliena += I;
                }
            }

            si (machina && !loca)
            {
                imprimere("%s\t%u\t%u\t%u\n", via,
                    (insignatus integer)propria,
                    (insignatus integer)aliena,
                    (insignatus integer)
                        parsura->numerus_errorum);
            }
            alioquin si (   !machina
                         && (propria > (i32)ZEPHYRUM
                         || parsura->numerus_errorum
                         > (i32)ZEPHYRUM))
            {
                imprimere("%s\tambigua %u\terrores %u\n", via,
                    (insignatus integer)propria,
                    (insignatus integer)
                        parsura->numerus_errorum);
            }

            si (pinna_via != NIHIL)
            {
                s32 pinnatum;

                pinnatum = _pinna_numerus(pinna, via);
                si (pinnatum < (s32)ZEPHYRUM) pinnatum = ZEPHYRUM;
                si ((s32)propria > pinnatum)
                {
                    fprintf(stderr,
                        "ambigua: REGRESSUS %s (%u pro %d"
                        " pinnatis)\n", via,
                        (insignatus integer)propria,
                        (integer)pinnatum);
                    regressus = VERUM;
                }
                alioquin si ((s32)propria < pinnatum)
                {
                    fprintf(stderr,
                        "ambigua: melior %s (%u pro %d pinnatis"
                        " - pinnam renovare licet)\n", via,
                        (insignatus integer)propria,
                        (integer)pinnatum);
                }
            }

            plagulae       += I;
            ambigua_summa  += propria;
            aliena_summa   += aliena;
            errores_summa  += parsura->numerus_errorum;
            piscina_destruere(opus);
        }
    }

    si (!ulla_plagula)
    {
        fprintf(stderr, "usus: ambigua <via.c> [viae ...]"
            " [-machina] [-loca] [-pinna <via.tsv>]\n");
        piscina_destruere(piscina);
        redde II;
    }

    si (machina)
    {
        imprimere("# summa\t%u\t%u\t%u\t%u\n",
            (insignatus integer)plagulae,
            (insignatus integer)ambigua_summa,
            (insignatus integer)aliena_summa,
            (insignatus integer)errores_summa);
    }
    alioquin
    {
        imprimere("plagulae %u\tambigua %u\taliena %u\t"
            "errores %u\n",
            (insignatus integer)plagulae,
            (insignatus integer)ambigua_summa,
            (insignatus integer)aliena_summa,
            (insignatus integer)errores_summa);
    }

    piscina_destruere(piscina);
    si (regressus) redde I;
    redde ZEPHYRUM;
}
