/* sonda: thesaurus_aperire admittit radicem CCCLXXXIII literarum;
 * THESAURUS_GENERATIO CXX literarum -> via generationis 521+NUL in
 * generatio[512] (membrum ultimum structurae). Canarium post
 * Thesaurum in piscina eadem positum inspicitur. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "thesaurus.h"
#include "sigillum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

integer
principale (vacuum)
{
    Piscina*   piscina;
    Thesaurus* thesaurus;
    character  radix[1024];
    character  titulus[200];
    character* canarium;
    Sigillum   clavis;
    integer    longitudo;
    integer    k;
    integer    corrupta;

    piscina = piscina_generare_dynamicum("sonda", 65536);
    si (getcwd(radix, 600) == NIHIL) { redde 1; }
    strcat(radix, "/build/sonda/thesaurus_r1_dir/");
    longitudo = (integer)strlen(radix);
    /* complere ad 383 literas, segmentis < 200 */
    dum (longitudo < 383)
    {
        integer seg = 383 - longitudo;
        si (seg > 150) { seg = 150; }
        per (k = 0; k < seg; k++) { radix[longitudo + k] = 'r'; }
        longitudo += seg;
        radix[longitudo] = '\0';
        si (longitudo < 383) { radix[longitudo++] = '/'; radix[longitudo] = '\0'; }
    }
    si (radix[longitudo - 1] == '/') { radix[longitudo - 1] = 'r'; }
    printf("strlen(radix) = %lu (limes: +128 < 512 -> <= 383)\n",
        (unsigned long)strlen(radix));

    thesaurus = thesaurus_aperire(radix, piscina);
    printf("thesaurus_aperire -> %s\n", thesaurus ? "Thesaurus" : "NIHIL");
    si (thesaurus == NIHIL) { redde 1; }

    canarium = (character*)piscina_allocare(piscina, 64);
    memset(canarium, 'Q', 64);
    printf("canarium - thesaurus = %ld octeti\n",
        (long)(canarium - (character*)thesaurus));

    per (k = 0; k < 120; k++) { titulus[k] = 'g'; }
    titulus[120] = '\0';
    setenv("THESAURUS_GENERATIO", titulus, 1);

    memset(&clavis, 0xab, sizeof(clavis));
    thesaurus_generationem_notare(thesaurus, &clavis);

    printf("via generationis expectata: %lu literae + NUL in [512]\n",
        (unsigned long)(strlen(radix) + strlen("/generationes/") + 120 + 4));
    corrupta = 0;
    per (k = 0; k < 64; k++) { si (canarium[k] != 'Q') { corrupta++; } }
    printf("canarium: %d octeti corrupti; primi: ", corrupta);
    per (k = 0; k < 8; k++) { printf("%c", canarium[k] ? canarium[k] : '0'); }
    printf("\n");
    piscina_destruere(piscina);
    redde corrupta ? 3 : 0;
}
