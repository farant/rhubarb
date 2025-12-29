/* librarium_collector.c - Colligit libros ex librarium.stml
 *
 * Legit librarium.stml, copiat libros in folderum book_assets,
 * generat STML modificatum et TOML config pro capsula.
 *
 * USUS:
 *   ./bin/librarium_collector librarium.stml book_assets/ [--limit 100]
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "stml.h"
#include "filum.h"
#include "via.h"
#include "internamentum.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>


/* ========================================================================
 * CONSTANTIAE
 * ======================================================================== */

#define LIMIT_DEFECTUS  100


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Informationes de libro collectae */
nomen structura {
    chorda numerus;
    chorda titulus;
    chorda auctor;
    chorda annus;
    chorda summarium;
    chorda notae;
    chorda via_originalis;
    chorda status;
    Xar*   tags;          /* Xar de chorda */
    b32    successus;     /* Copia successit? */
} LibrumCollectum;


/* ========================================================================
 * HELPER: MKDIR
 * ======================================================================== */

interior b32
_creare_directorium(constans character* via)
{
    si (mkdir(via, 0755) == 0)
    {
        redde VERUM;
    }

    /* Iam existit? */
    si (errno == EEXIST)
    {
        redde VERUM;
    }

    redde FALSUM;
}


/* ========================================================================
 * HELPER: COPY FILE
 * ======================================================================== */

interior b32
_copiare_filum(constans character* fons, constans character* destinatio, Piscina* piscina)
{
    chorda contentum;
    FILE*  out;

    contentum = filum_legere_totum(fons, piscina);

    si (contentum.mensura == 0)
    {
        redde FALSUM;
    }

    out = fopen(destinatio, "wb");
    si (out == NIHIL)
    {
        redde FALSUM;
    }

    fwrite(contentum.datum, 1, contentum.mensura, out);
    fclose(out);

    redde VERUM;
}


/* ========================================================================
 * HELPER: CHORDA AD C STRING
 * ======================================================================== */

interior constans character*
_chorda_ad_cstr(chorda ch, Piscina* piscina)
{
    character* buf;

    buf = (character*)piscina_allocare(piscina, ch.mensura + I);
    memcpy(buf, ch.datum, ch.mensura);
    buf[ch.mensura] = '\0';

    redde buf;
}


/* ========================================================================
 * HELPER: EXTRACT TAGS
 * ======================================================================== */

interior Xar*
_extrahere_tags(StmlNodus* nodus_liber, Piscina* piscina)
{
    StmlNodus* nodus_tags;
    Xar*       tags;
    Xar*       tag_nodes;
    i32        i;

    tags = xar_creare(piscina, (i32)magnitudo(chorda));

    nodus_tags = stml_invenire_liberum(nodus_liber, "tags");
    si (nodus_tags == NIHIL)
    {
        redde tags;
    }

    tag_nodes = stml_invenire_omnes_liberos(nodus_tags, "tag", piscina);

    per (i = 0; i < xar_numerus(tag_nodes); i++)
    {
        StmlNodus* tag_nodus;
        chorda*    tag_chorda;

        tag_nodus = *(StmlNodus**)xar_obtinere(tag_nodes, i);
        tag_chorda = (chorda*)xar_addere(tags);

        si (tag_chorda != NIHIL)
        {
            *tag_chorda = stml_textus_internus(tag_nodus, piscina);
        }
    }

    redde tags;
}


/* ========================================================================
 * GENERARE STML
 * ======================================================================== */

interior chorda
_generare_stml(Xar* libri, Piscina* piscina)
{
    ChordaAedificator* aed;
    i32                i;

    aed = chorda_aedificator_creare(piscina, XXXII * M);

    chorda_aedificator_appendere_literis(aed, "<librarium>\n");

    per (i = 0; i < xar_numerus(libri); i++)
    {
        LibrumCollectum* liber;
        i32              j;

        liber = (LibrumCollectum*)xar_obtinere(libri, i);

        si (!liber->successus)
        {
            perge;
        }

        /* Aperire liber elementum */
        chorda_aedificator_appendere_literis(aed, "  <liber numerus=\"");
        chorda_aedificator_appendere_chorda(aed, liber->numerus);
        chorda_aedificator_appendere_literis(aed, "\" status=\"");
        chorda_aedificator_appendere_chorda(aed, liber->status);
        chorda_aedificator_appendere_literis(aed, "\">\n");

        /* Titulus */
        chorda_aedificator_appendere_literis(aed, "    <titulus>");
        chorda_aedificator_appendere_chorda(aed, liber->titulus);
        chorda_aedificator_appendere_literis(aed, "</titulus>\n");

        /* Auctor */
        chorda_aedificator_appendere_literis(aed, "    <auctor>");
        chorda_aedificator_appendere_chorda(aed, liber->auctor);
        chorda_aedificator_appendere_literis(aed, "</auctor>\n");

        /* Annus */
        chorda_aedificator_appendere_literis(aed, "    <annus>");
        chorda_aedificator_appendere_chorda(aed, liber->annus);
        chorda_aedificator_appendere_literis(aed, "</annus>\n");

        /* Summarium */
        chorda_aedificator_appendere_literis(aed, "    <summarium>");
        chorda_aedificator_appendere_chorda(aed, liber->summarium);
        chorda_aedificator_appendere_literis(aed, "</summarium>\n");

        /* Notae */
        si (liber->notae.mensura > 0)
        {
            chorda_aedificator_appendere_literis(aed, "    <notae>");
            chorda_aedificator_appendere_chorda(aed, liber->notae);
            chorda_aedificator_appendere_literis(aed, "</notae>\n");
        }

        /* Via - nunc {numerus}.txt */
        chorda_aedificator_appendere_literis(aed, "    <via>");
        chorda_aedificator_appendere_chorda(aed, liber->numerus);
        chorda_aedificator_appendere_literis(aed, ".txt</via>\n");

        /* Tags */
        si (liber->tags != NIHIL && xar_numerus(liber->tags) > 0)
        {
            chorda_aedificator_appendere_literis(aed, "    <tags>\n");

            per (j = 0; j < xar_numerus(liber->tags); j++)
            {
                chorda* tag;

                tag = (chorda*)xar_obtinere(liber->tags, j);
                chorda_aedificator_appendere_literis(aed, "      <tag>");
                chorda_aedificator_appendere_chorda(aed, *tag);
                chorda_aedificator_appendere_literis(aed, "</tag>\n");
            }

            chorda_aedificator_appendere_literis(aed, "    </tags>\n");
        }

        /* Claudere liber */
        chorda_aedificator_appendere_literis(aed, "  </liber>\n");
    }

    chorda_aedificator_appendere_literis(aed, "</librarium>\n");

    redde chorda_aedificator_finire(aed);
}


/* ========================================================================
 * GENERARE TOML
 * ======================================================================== */

interior chorda
_generare_toml(Piscina* piscina)
{
    ChordaAedificator* aed;

    aed = chorda_aedificator_creare(piscina, M);

    chorda_aedificator_appendere_literis(aed,
        "# Capsula config pro libri\n"
        "# Generatum a librarium_collector\n\n"
        "libri_files = [\"*.txt\", \"librarium.stml\"]\n"
        "libri_compress = true\n");

    redde chorda_aedificator_finire(aed);
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(integer argc, character** argv)
{
    Piscina*            piscina;
    InternamentumChorda* intern;
    chorda              config_content;
    StmlResultus        resultus;
    Xar*                libri_stml;
    Xar*                libri_collecta;
    constans character* input_via;
    constans character* output_dir;
    i32                 limit;
    i32                 i;
    i32                 successus_count;
    i32                 fallita_count;
    chorda              input_dir;
    character           output_stml_via[1024];
    character           output_toml_via[1024];
    FILE*               out_file;
    chorda              stml_output;
    chorda              toml_output;

    /* Parse argumenta */
    si (argc < III)
    {
        fprintf(stderr, "Usus: %s <librarium.stml> <output_dir> [--limit N]\n", argv[0]);
        fprintf(stderr, "\nExempla:\n");
        fprintf(stderr, "  %s librarium.stml book_assets/\n", argv[0]);
        fprintf(stderr, "  %s librarium.stml book_assets/ --limit 100\n", argv[0]);
        redde I;
    }

    input_via = argv[I];
    output_dir = argv[II];
    limit = LIMIT_DEFECTUS;

    /* Check pro --limit */
    per (i = III; i < (i32)argc; i++)
    {
        si (strcmp(argv[i], "--limit") == 0 && (i32)(i + I) < (i32)argc)
        {
            limit = (i32)atoi(argv[i + I]);
            i++;
        }
    }

    printf("Librarium Collector\n");
    printf("===================\n");
    printf("Input:  %s\n", input_via);
    printf("Output: %s\n", output_dir);
    printf("Limit:  %d\n\n", limit);

    piscina = piscina_generare_dynamicum("librarium_collector", CXXVIII * M);
    intern = internamentum_creare(piscina);

    /* Creare output directorium */
    si (!_creare_directorium(output_dir))
    {
        fprintf(stderr, "Error: non possum creare directorium '%s'\n", output_dir);
        piscina_destruere(piscina);
        redde I;
    }

    /* Capere directorium input fili */
    input_dir = via_directorium(chorda_ex_literis(input_via, piscina), piscina);

    /* Legere STML */
    config_content = filum_legere_totum(input_via, piscina);
    si (config_content.mensura == 0)
    {
        fprintf(stderr, "Error: non possum legere '%s'\n", input_via);
        piscina_destruere(piscina);
        redde I;
    }

    /* Parse STML */
    resultus = stml_legere(config_content, piscina, intern);
    si (!resultus.successus)
    {
        fprintf(stderr, "Error: STML parsing fallita\n");
        piscina_destruere(piscina);
        redde I;
    }

    /* Invenire omnes libros */
    libri_stml = stml_invenire_omnes_liberos(resultus.elementum_radix, "liber", piscina);

    printf("Inventa %d libri in STML\n", xar_numerus(libri_stml));
    printf("Processurus %d libri (limit)\n\n",
           limit < xar_numerus(libri_stml) ? limit : xar_numerus(libri_stml));

    /* Colligere libros */
    libri_collecta = xar_creare(piscina, (i32)magnitudo(LibrumCollectum));
    successus_count = 0;
    fallita_count = 0;

    per (i = 0; i < xar_numerus(libri_stml) && i < limit; i++)
    {
        StmlNodus*       nodus_liber;
        StmlNodus*       nodus_temp;
        LibrumCollectum* liber;
        chorda           via_relativa;
        chorda           partes[II];
        chorda           via_absoluta;
        character        output_file_via[1024];
        constans character* via_abs_cstr;
        constans character* numerus_cstr;

        nodus_liber = *(StmlNodus**)xar_obtinere(libri_stml, i);

        liber = (LibrumCollectum*)xar_addere(libri_collecta);
        si (liber == NIHIL)
        {
            perge;
        }

        /* Capere numerus attributum */
        {
            chorda* attr_numerus;
            chorda* attr_status;

            attr_numerus = stml_attributum_capere(nodus_liber, "numerus");
            attr_status = stml_attributum_capere(nodus_liber, "status");

            liber->numerus = attr_numerus ? *attr_numerus :
                                            chorda_ex_literis("", piscina);
            liber->status = attr_status ? *attr_status :
                                          chorda_ex_literis("enriched", piscina);
        }

        /* Capere elementa */
        nodus_temp = stml_invenire_liberum(nodus_liber, "titulus");
        liber->titulus = nodus_temp ? stml_textus_internus(nodus_temp, piscina) :
                                       chorda_ex_literis("", piscina);

        nodus_temp = stml_invenire_liberum(nodus_liber, "auctor");
        liber->auctor = nodus_temp ? stml_textus_internus(nodus_temp, piscina) :
                                      chorda_ex_literis("", piscina);

        nodus_temp = stml_invenire_liberum(nodus_liber, "annus");
        liber->annus = nodus_temp ? stml_textus_internus(nodus_temp, piscina) :
                                     chorda_ex_literis("", piscina);

        nodus_temp = stml_invenire_liberum(nodus_liber, "summarium");
        liber->summarium = nodus_temp ? stml_textus_internus(nodus_temp, piscina) :
                                         chorda_ex_literis("", piscina);

        nodus_temp = stml_invenire_liberum(nodus_liber, "notae");
        liber->notae = nodus_temp ? stml_textus_internus(nodus_temp, piscina) :
                                     chorda_ex_literis("", piscina);

        nodus_temp = stml_invenire_liberum(nodus_liber, "via");
        liber->via_originalis = nodus_temp ? stml_textus_internus(nodus_temp, piscina) :
                                              chorda_ex_literis("", piscina);

        liber->tags = _extrahere_tags(nodus_liber, piscina);

        /* Resolvere via */
        si (liber->via_originalis.mensura == 0)
        {
            printf("  [%d] %.*s - via vacua, saliens\n",
                   i, (i32)liber->numerus.mensura, liber->numerus.datum);
            liber->successus = FALSUM;
            fallita_count++;
            perge;
        }

        /* Iungere input_dir + via_originalis */
        partes[0] = input_dir;
        partes[I] = liber->via_originalis;
        via_relativa = via_iungere(partes, II, piscina);
        via_absoluta = via_normalizare(via_relativa, piscina);

        via_abs_cstr = _chorda_ad_cstr(via_absoluta, piscina);
        numerus_cstr = _chorda_ad_cstr(liber->numerus, piscina);

        /* Creare output via */
        snprintf(output_file_via, sizeof(output_file_via),
                 "%s/%s.txt", output_dir, numerus_cstr);

        /* Copiare filum */
        si (_copiare_filum(via_abs_cstr, output_file_via, piscina))
        {
            printf("  [%d] %s -> %s.txt\n", i, numerus_cstr, numerus_cstr);
            liber->successus = VERUM;
            successus_count++;
        }
        alioquin
        {
            printf("  [%d] %s - NON INVENTUM: %s\n",
                   i, numerus_cstr, via_abs_cstr);
            liber->successus = FALSUM;
            fallita_count++;
        }
    }

    printf("\n");
    printf("Copiata: %d libri\n", successus_count);
    printf("Fallita: %d libri\n", fallita_count);
    printf("\n");

    /* Generare STML */
    snprintf(output_stml_via, sizeof(output_stml_via), "%s/librarium.stml", output_dir);
    stml_output = _generare_stml(libri_collecta, piscina);

    out_file = fopen(output_stml_via, "w");
    si (out_file != NIHIL)
    {
        fwrite(stml_output.datum, 1, stml_output.mensura, out_file);
        fclose(out_file);
        printf("Generated: %s\n", output_stml_via);
    }
    alioquin
    {
        fprintf(stderr, "Error: non possum scribere '%s'\n", output_stml_via);
    }

    /* Generare TOML */
    snprintf(output_toml_via, sizeof(output_toml_via), "%s/libri.toml", output_dir);
    toml_output = _generare_toml(piscina);

    out_file = fopen(output_toml_via, "w");
    si (out_file != NIHIL)
    {
        fwrite(toml_output.datum, 1, toml_output.mensura, out_file);
        fclose(out_file);
        printf("Generated: %s\n", output_toml_via);
    }
    alioquin
    {
        fprintf(stderr, "Error: non possum scribere '%s'\n", output_toml_via);
    }

    printf("\nFactum!\n");
    printf("\nProximi passi:\n");
    printf("  ./bin/capsula_generare %s\n", output_toml_via);

    piscina_destruere(piscina);
    redde 0;
}
