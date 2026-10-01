/* latina_datum.c - silva/fontes/silva_latina_datum.{h,c} ex
 * include/latina.h generare
 *
 * Copia compilata textus latina.h (silva_contextus_latinam_addere eam
 * praebet). Olim 'passus 0' amalgamatoris silvae, qui has plagulas
 * COMMISSAS in arbore scribebat in omni cursu - etiam sub iudice
 * fabricae et in quoque gyro excludendorum (fabrica 1b T5, D5). Nunc
 * actio sua 'latina_datum' (silva/aedificatio.stml); amalgamator eas
 * solum legit.
 *
 * Usus: latina_datum <radix_lectionis> <radix_scripturae>
 *   lectio: <radix_lectionis>/include/latina.h
 *   scriptura: <radix_scripturae>/silva/fontes/silva_latina_datum.{h,c}
 * Exitus: 0 scriptum, 1 fractum, 2 usus.
 */
#include "latina.h"
#include <stdio.h>
#include <stdlib.h>

#define VIA_MAXIMA 4096

interior constans character* constans PROOEMIUM =
    "/* silva_latina_datum.h - Textus latina.h ut datum (Phase 7"
    " Chunk A)\n"
    " *\n"
    " * GENERATUM ex include/latina.h per"
    " silva/latina_datum_generare.sh\n"
    " * (actio fabricae 'latina_datum') - NE MANU MUTES. Copia"
    " compilata\n"
    " * definitionum latinarum: silva_contextus_latinam_addere eam"
    " praebet\n"
    " * - \"compiled-in defaults\" interview ad litteram, sine fonte\n"
    " * veritatis secundo (datum IPSA plagula vendicata est).\n"
    " */\n";

/* plagulam totam legere (octeti), mensura in *mensura_out */
interior i8*
_legere (
    constans character* via,
                   i32* mensura_out)
{
    FILE* plagula;
    long  mensura;
     i8* textus;

    plagula = fopen(via, "rb");
    si (plagula == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(plagula, 0L, SEEK_END) != 0)
    {
        fclose(plagula);
        redde NIHIL;
    }
    mensura = ftell(plagula);
    si (mensura < 0L || fseek(plagula, 0L, SEEK_SET) != 0)
    {
        fclose(plagula);
        redde NIHIL;
    }
    textus = (i8*)malloc((memoriae_index)mensura + 1);
    si (textus == NIHIL)
    {
        fclose(plagula);
        redde NIHIL;
    }
    si (fread(textus, 1, (memoriae_index)mensura, plagula)
        != (memoriae_index)mensura)
    {
        free(textus);
        fclose(plagula);
        redde NIHIL;
    }
    fclose(plagula);
    *mensura_out = (i32)mensura;
    redde textus;
}

interior b32
_caput_scribere (
    constans character* via)
{
    FILE* plagula;

    plagula = fopen(via, "wb");
    si (plagula == NIHIL)
    {
        fprintf(stderr, "latina_datum: %s non apertum\n", via);
        redde FALSUM;
    }
    fprintf(plagula, "%s", PROOEMIUM);
    fprintf(plagula,
        "\n"
        "#ifndef SILVA_LATINA_DATUM_H\n"
        "#define SILVA_LATINA_DATUM_H\n"
        "\n"
        "#include \"latina.h\"\n"
        "\n"
        "externus constans character silva_latina_textus[];\n"
        "externus constans i32       silva_latina_mensura;\n"
        "\n"
        "#endif /* SILVA_LATINA_DATUM_H */\n");
    fclose(plagula);
    redde VERUM;
}

interior b32
_corpus_scribere (
    constans character* via,
           constans i8* textus,
                   i32  mensura)
{
    FILE* plagula;
      i32 k;

    plagula = fopen(via, "wb");
    si (plagula == NIHIL)
    {
        fprintf(stderr, "latina_datum: %s non apertum\n", via);
        redde FALSUM;
    }
    fprintf(plagula,
        "/* silva_latina_datum.c - GENERATUM ex include/latina.h - NE"
        " MANU MUTES */\n"
        "\n"
        "#include \"silva_latina_datum.h\"\n"
        "\n"
        "constans character silva_latina_textus[] = {\n");
    per (k = ZEPHYRUM; k < mensura; k += XII)
    {
        i32 finis;
        i32 j;

        finis = (k + XII < mensura) ? (k + XII) : mensura;
        fprintf(plagula, "    ");
        per (j = k; j < finis; j++)
        {
            fprintf(plagula, (j == k) ? "%d" : ", %d", (int)textus[j]);
        }
        fprintf(plagula, (finis < mensura) ? ",\n" : "\n");
    }
    fprintf(plagula,
        "};\n"
        "\n"
        "constans i32 silva_latina_mensura = %d;\n", (int)mensura);
    fclose(plagula);
    redde VERUM;
}

s32
principale (
          s32   argc,
    character** argv)
{
    character  via[VIA_MAXIMA];
           i8* textus;
          i32  mensura;

    si (argc != III)
    {
        fprintf(stderr,
            "usus: latina_datum <radix_lectionis>"
            " <radix_scripturae>\n");
        redde II;
    }
    sprintf(via, "%s/include/latina.h", argv[I]);
    mensura  = ZEPHYRUM;
    textus   = _legere(via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "latina_datum: %s non lecta\n", via);
        redde I;
    }
    sprintf(via, "%s/silva/fontes/silva_latina_datum.h", argv[II]);
    si (!_caput_scribere(via))
    {
        free(textus);
        redde I;
    }
    sprintf(via, "%s/silva/fontes/silva_latina_datum.c", argv[II]);
    si (!_corpus_scribere(via, textus, mensura))
    {
        free(textus);
        redde I;
    }
    free(textus);
    redde ZEPHYRUM;
}
