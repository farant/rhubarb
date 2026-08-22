/* generator.c - Principale generatoris (dev-time)
 *
 * Usus: generator <grammatica.stml> <PRAEFIXUM> <basis_exitus>
 * e.g.: generator grammatica/sceletum.stml SILVA_SCELETUM \
 *           fontes/silva_tabulae_sceleti
 *
 * Legit grammaticam annotatam, construit tabulas LALR conflictus
 * servantes, coquit in <basis_exitus>.{h,c}. Curritur per
 * silva/generare.sh; exitus committitur (pars futuri silva.c).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "silva_generare.h"
#include "silva_coquere.h"
#include "silva_canon_coquere.h"
#include <stdio.h>
#include <string.h>

interior character*
_plagulam_legere (Piscina* piscina, constans character* via)
{
    FILE* pl;
    character* buffer;
    signatus longus mensura;

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

    buffer = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    buffer[mensura] = '\0';
    redde buffer;
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    character* fons;
    SilvaGenGrammatica* grammatica;
    SilvaGenCollectio* collectio;
    SilvaGenTabula* tabula;
    constans character* via_grammaticae;
    b32 canon_solus;

    /* MODUS CANONIS SOLIUS: porta recentiae canonem in locum
     * TEMPORARIUM scribere debet ut exitum conferat, et tabulas
     * interea NON tangere. Tabulae LALR huic nihil conferunt
     * (proiectio grammaticam ipsam legit, non tabulas coctas),
     * ergo hic modus collectionem omnino praeterit - quod eum
     * etiam CELEREM facit, id quod porta saepe currenda poscit. */
    canon_solus = (b32)(argc == V
        && strcmp(argv[I], "-canon") == ZEPHYRUM);

    si (!canon_solus && argc != IV && argc != VI && argc != VIII)
    {
        fprintf(stderr,
            "usus: generator <grammatica.stml> <PRAEFIXUM> <basis_exitus>"
            " [<caput_publicum> <hospes> [<canon> <dialectus>]]\n"
            "      generator -canon <grammatica.stml> <dialectus>"
            " <exitus.canon>\n");
        redde I;
    }
    via_grammaticae = canon_solus ? argv[II] : argv[I];

    piscina = piscina_generare_dynamicum("silva_generator", 33554432);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "generator: piscina non generata\n");
        redde I;
    }
    intern = internamentum_creare(piscina);

    fons = _plagulam_legere(piscina, via_grammaticae);
    si (fons == NIHIL)
    {
        fprintf(stderr, "generator: grammatica non lecta: %s\n",
            via_grammaticae);
        redde I;
    }

    grammatica = silva_gen_grammaticam_legere(piscina, intern, fons);
    si (grammatica == NIHIL)
    {
        fprintf(stderr, "generator: grammatica invalida\n");
        redde I;
    }

    si (canon_solus)
    {
        si (!silva_gen_canonem_scribere(grammatica, argv[III], argv[IV],
                 via_grammaticae))
        {
            fprintf(stderr, "generator: canon scribi non potuit\n");
            redde I;
        }
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }

    si (!silva_gen_first_computare(grammatica)
        || !silva_gen_follow_computare(grammatica))
    {
        fprintf(stderr, "generator: FIRST/FOLLOW defecerunt\n");
        redde I;
    }

    collectio = silva_gen_collectio_lalr_construere(grammatica);
    si (collectio == NIHIL)
    {
        fprintf(stderr, "generator: collectio LALR defecit\n");
        redde I;
    }

    tabula = silva_gen_tabulam_construere(collectio);
    si (tabula == NIHIL)
    {
        fprintf(stderr, "generator: tabula defecit\n");
        redde I;
    }

    si (!silva_gen_coquere(grammatica, tabula, argv[II], argv[III], argv[I]))
    {
        fprintf(stderr, "generator: coctio defecit\n");
        redde I;
    }

    /* Superficies publica (officina pre-M1): splicis in silva.h +
     * hospes.c - solum si viae datae (grammatica c89 sola) */
    si (argc >= VI
        && !silva_gen_splicere_publica(piscina, grammatica, argv[II],
               argv[IV], argv[V]))
    {
        fprintf(stderr, "generator: splicis publicae defecit\n");
        redde I;
    }

    /* Canon documentorum arboris (arbor M2 par. 3) - grammatica c89
     * sola, quae sola documenta parsurae gignit */
    si (argc == VIII
        && !silva_gen_canonem_scribere(grammatica, argv[VII], argv[VI],
               argv[I]))
    {
        fprintf(stderr, "generator: canon scribi non potuit\n");
        redde I;
    }

    imprimere("generator: %s -> %s.{h,c}\n", argv[I], argv[III]);
    imprimere("  symbola: %d (%d terminalia), productiones: %d\n",
        (int)xar_numerus(grammatica->symbola),
        (int)grammatica->numerus_terminalium,
        (int)xar_numerus(grammatica->productiones));
    imprimere("  status: %d, conflictus servati: %d\n",
        (int)xar_numerus(collectio->status_omnes),
        (int)tabula->numerus_conflictuum);

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
