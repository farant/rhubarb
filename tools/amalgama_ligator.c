/* amalgama_ligator.c - ligator genericus (dev-time, Phasis D)
 *
 * Fasciculum domesticum ex clausura derivata ligat: manifestum
 * DEGENERATUM (sine capite manuali, sine renominatione, sine
 * cadendis) in mechanismum communem (silva_amalgama) bis fundit -
 * cursus capitum solorum -> <titulus>.h (concatenatio topologica,
 * latina inclusa - se-contenta), cursus plenus -> <titulus>.c
 * (implementatio se-contenta). Detectio S41 (statica duplicata
 * trans corpora) gratis venit.
 *
 * Usus: amalgama_ligator <radix> <lista.tsv> <titulus>
 *                        <exitus.h> <exitus.c>
 * lista.tsv: lineae "C\t<via>" (capita ORDINE topologico) et
 * "O\t<via>" (corpora). Politica nulla - vectis (amalgama_ligare.sh)
 * clausuram derivat, ordinat, recusationes facit.
 */
#include "latina.h"
#include "piscina.h"
#include "silva_amalgama.h"
#include <stdio.h>
#include <string.h>

#define LIGATOR_MAXIMAE 512

interior constans character* constans VACUA[] = { NIHIL };

interior constans character*
_chordam_figere (Piscina* piscina, constans character* fons,
    memoriae_index mensura)
{
    character* nova;

    nova = (character*)piscina_allocare(piscina, mensura + 1);
    si (nova == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(nova, fons, mensura);
    nova[mensura] = '\0';
    redde nova;
}

interior vacuum
_plagulam_ponere (AmalgamaPlagula* plagula, constans character* via,
    b32 est_corpus)
{
    plagula->via = via;
    plagula->servanda = NIHIL;
    plagula->excludenda = NIHIL;
    plagula->est_corpus = est_corpus;
    plagula->est_vendicata = FALSUM;  /* sine static, sine rename */
}

s32 principale (s32 argc, character** argv)
{
    Piscina* piscina;
    FILE* lista;
    interior AmalgamaPlagula capita[LIGATOR_MAXIMAE];
    interior AmalgamaPlagula corpora[LIGATOR_MAXIMAE];
    i32 numerus_capitum;
    i32 numerus_corporum;
    character linea[1024];
    AmalgamaManifestum manifestum;
    character prooemium[512];

    si (argc < VI)
    {
        fprintf(stderr, "usus: amalgama_ligator <radix> <lista.tsv>"
            " <titulus> <exitus.h> <exitus.c>\n");
        redde I;
    }
    piscina = piscina_generare_dynamicum("amalgama_ligator",
        33554432);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "amalgama_ligator: piscina non generata\n");
        redde I;
    }

    lista = fopen(argv[II], "r");
    si (lista == NIHIL)
    {
        fprintf(stderr, "amalgama_ligator: lista non lecta: %s\n",
            argv[II]);
        redde I;
    }
    numerus_capitum = ZEPHYRUM;
    numerus_corporum = ZEPHYRUM;
    dum (fgets(linea, (s32)magnitudo(linea), lista) != NIHIL)
    {
        memoriae_index mensura;
        constans character* via;

        mensura = strlen(linea);
        dum (mensura > ZEPHYRUM && (linea[mensura - 1] == '\n'
            || linea[mensura - 1] == '\r'))
        {
            linea[--mensura] = '\0';
        }
        si (mensura < III || linea[I] != '\t')
        {
            perge;
        }
        via = _chordam_figere(piscina, linea + II, mensura - II);
        si (via == NIHIL)
        {
            fprintf(stderr, "amalgama_ligator: memoria\n");
            redde I;
        }
        si (linea[ZEPHYRUM] == 'C')
        {
            si (numerus_capitum >= LIGATOR_MAXIMAE)
            {
                fprintf(stderr, "amalgama_ligator: capita ultra"
                    " modum (%d)\n", LIGATOR_MAXIMAE);
                redde I;
            }
            _plagulam_ponere(&capita[numerus_capitum++], via,
                FALSUM);
        }
        alioquin si (linea[ZEPHYRUM] == 'O')
        {
            si (numerus_corporum >= LIGATOR_MAXIMAE)
            {
                fprintf(stderr, "amalgama_ligator: corpora ultra"
                    " modum (%d)\n", LIGATOR_MAXIMAE);
                redde I;
            }
            _plagulam_ponere(&corpora[numerus_corporum++], via,
                VERUM);
        }
    }
    fclose(lista);
    si (numerus_capitum == ZEPHYRUM
        || numerus_corporum == ZEPHYRUM)
    {
        fprintf(stderr,
            "amalgama_ligator: lista vacua (capita %d, corpora %d)\n",
            (s32)numerus_capitum, (s32)numerus_corporum);
        redde I;
    }

    memset(&manifestum, ZEPHYRUM, magnitudo(manifestum));
    manifestum.capita_propria = capita;
    manifestum.numerus_capitum_propriorum = numerus_capitum;
    manifestum.cadenda_typedef = VACUA;
    manifestum.cadenda_definitio = VACUA;
    manifestum.non_statica = VACUA;
    /* via_capitis NIHIL = sine capite manuali (memset);
     * renominationes/latina-datum: NIHIL/zephyrum (memset) */

    /* cursus I: capita sola -> <titulus>.h */
    sprintf(prooemium,
        "/* %s.h - FASCICULUS GENERATUS (amalgama_ligare) - NE MANU"
        " EDITES\n * Concatenatio capitum clausurae ordine"
        " topologico, latina inclusa\n * (se-contenta). Regeneratio:"
        " ./tools/amalgama_ligare.sh %s ...\n */\n\n",
        argv[III], argv[III]);
    manifestum.prooemium = prooemium;
    si (!silva_amalgama_currere(piscina, argv[I], argv[IV],
            &manifestum))
    {
        fprintf(stderr, "amalgama_ligator: cursus capitum fractus\n");
        redde I;
    }

    /* cursus II: capita + corpora -> <titulus>.c (se-contentus) */
    sprintf(prooemium,
        "/* %s.c - FASCICULUS GENERATUS (amalgama_ligare) - NE MANU"
        " EDITES\n * Implementatio se-contenta (capita + corpora"
        " clausurae). Compila\n * severitate plena; nulla"
        " renominatio - API originalis. Regeneratio:\n *"
        " ./tools/amalgama_ligare.sh %s ...\n */\n\n",
        argv[III], argv[III]);
    manifestum.prooemium = prooemium;
    manifestum.corpora_propria = corpora;
    manifestum.numerus_corporum_propriorum = numerus_corporum;
    si (!silva_amalgama_currere(piscina, argv[I], argv[V],
            &manifestum))
    {
        fprintf(stderr, "amalgama_ligator: cursus plenus fractus\n");
        redde I;
    }

    imprimere("amalgama_ligator: %s (%d capita) + %s (%d corpora)\n",
        argv[IV], (s32)numerus_capitum, argv[V],
        (s32)numerus_corporum);
    redde ZEPHYRUM;
}
