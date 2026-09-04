/* html.c - markdown -> html per compositionem STML (md/html.sh)
 *
 * Usus: html <plagula.md> -programma <md-html.stml> [-stml]
 * Legit plagulam et programma, md_html_reddere vocat, html in stdout
 * scribit; -stml: documentum compositum pro programmate (inspectio
 * per 'stml expandere'). Exitus: 0 sanum, 1 fractum (causa in
 * stderr), 2 usus/plagula absens.
 */

#include "latina.h"
#include "md_html.h"
#include "piscina.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

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

integer
principale (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
    InternamentumChorda* intern;
              character* textus;
              character* programma_textus;
                    i32  mensura            = ZEPHYRUM;
                    i32  programma_mensura  = ZEPHYRUM;
                 chorda  programma;
         MdHtmlResultus  r;
                    b32  stml = FALSUM;
                integer  i;
     constans character* via            = NIHIL;
     constans character* programma_via  = NIHIL;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-stml") == ZEPHYRUM)
        {
            stml = VERUM;
        }
        alioquin si (   strcmp(argv[i], "-programma") == ZEPHYRUM
                     && i + I < argc)
        {
            programma_via = argv[++i];
        }
        alioquin
        {
            via = argv[i];
        }
    }
    si (via == NIHIL || programma_via == NIHIL)
    {
        fprintf(stderr,
            "usus: html <plagula.md> -programma <md-html.stml> [-stml]\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("md_html_instrumentum",
        16777216);
    intern = internamentum_creare(piscina);
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "html: plagula absens: %s\n", via);
        redde II;
    }
    programma_textus = _plagulam_legere(piscina, programma_via,
        &programma_mensura);
    si (programma_textus == NIHIL)
    {
        fprintf(stderr, "html: programma absens: %s\n", programma_via);
        redde II;
    }
    programma.datum = (i8*)programma_textus;
    programma.mensura = programma_mensura;
    r = md_html_reddere(piscina, intern, textus, mensura, programma);
    si (stml)
    {
        fwrite(r.stml.datum, I, (size_t)r.stml.mensura, stdout);
    }
    si (!r.successus)
    {
        fprintf(stderr, "html: %.*s\n", (integer)r.causa.mensura,
            (constans character*)r.causa.datum);
        piscina_destruere(piscina);
        redde I;
    }
    si (!stml)
    {
        fwrite(r.html.datum, I, (size_t)r.html.mensura, stdout);
    }
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
