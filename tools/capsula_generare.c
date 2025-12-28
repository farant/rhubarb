/* capsula_generare.c - Build Tool for File Embedding
 *
 * Legit TOML config, comprimit files, generat C headers.
 *
 * USUS:
 *   ./bin/capsula_generare config.toml
 *
 * CONFIG FORMAT:
 *   [libri]
 *   files = ["assets/books/pattern.txt"]
 *   compress = true
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "toml.h"
#include "filum.h"
#include "flatura.h"
#include "via.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>


/* ========================================================================
 * CONSTANTIAE
 * ======================================================================== */

#define CAPSULA_MAGICA     0x53504143  /* "CAPS" */
#define CAPSULA_VERSIO     1
#define BYTES_PER_LINE     16


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* File entry for building */
nomen structura {
    chorda via;              /* Relative path */
    chorda datum;            /* File contents (original) */
    chorda compressa;        /* Compressed data (or empty if no compression) */
    b32    est_compressa;    /* Was compression applied? */
} FileEntry;


/* ========================================================================
 * HELPER: GLOB MATCHING
 * ======================================================================== */

/* Simple glob match - supports * for any characters */
interior b32
_glob_congruit(constans character* pattern, constans character* str)
{
    dum (*pattern && *str)
    {
        si (*pattern == '*')
        {
            pattern++;
            si (*pattern == '\0')
            {
                redde VERUM;  /* Trailing * matches everything */
            }

            /* Try to match remaining pattern at each position */
            dum (*str)
            {
                si (_glob_congruit(pattern, str))
                {
                    redde VERUM;
                }
                str++;
            }
            redde FALSUM;
        }
        alioquin si (*pattern == *str)
        {
            pattern++;
            str++;
        }
        alioquin
        {
            redde FALSUM;
        }
    }

    /* Handle trailing * */
    dum (*pattern == '*')
    {
        pattern++;
    }

    redde *pattern == '\0' && *str == '\0';
}


/* ========================================================================
 * HELPER: EXPAND GLOB
 * ======================================================================== */

/* Expand a glob pattern to list of file paths */
interior vacuum
_expand_glob(
    constans character* pattern,
    constans character* base_dir,
    Xar*                files_out,
    Piscina*            piscina)
{
    DIR*            dir;
    structura dirent* entry;
    character       path_buffer[1024];
    constans character* slash;
    character       dir_part[512];
    constans character* file_pattern;
    i32             dir_len;

    /* Find the directory part and file pattern */
    slash = strrchr(pattern, '/');
    si (slash != NIHIL)
    {
        dir_len = (i32)(slash - pattern);
        si (dir_len >= 512) dir_len = 511;
        memcpy(dir_part, pattern, (size_t)dir_len);
        dir_part[dir_len] = '\0';
        file_pattern = slash + 1;
    }
    alioquin
    {
        strcpy(dir_part, ".");
        file_pattern = pattern;
    }

    /* Build full directory path */
    si (base_dir != NIHIL && strlen(base_dir) > 0)
    {
        snprintf(path_buffer, sizeof(path_buffer), "%s/%s", base_dir, dir_part);
    }
    alioquin
    {
        strncpy(path_buffer, dir_part, sizeof(path_buffer) - 1);
        path_buffer[sizeof(path_buffer) - 1] = '\0';
    }

    dir = opendir(path_buffer);
    si (dir == NIHIL)
    {
        fprintf(stderr, "Error: non possum aperire directorium '%s'\n", path_buffer);
        redde;
    }

    dum ((entry = readdir(dir)) != NIHIL)
    {
        si (entry->d_name[0] == '.')
        {
            perge;  /* Skip hidden files and . / .. */
        }

        si (_glob_congruit(file_pattern, entry->d_name))
        {
            character full_path[1024];
            chorda*   path_chorda;

            /* Build full path */
            si (strcmp(dir_part, ".") == 0)
            {
                snprintf(full_path, sizeof(full_path), "%s", entry->d_name);
            }
            alioquin
            {
                snprintf(full_path, sizeof(full_path), "%s/%s", dir_part, entry->d_name);
            }

            /* Add to output */
            path_chorda = (chorda*)xar_addere(files_out);
            si (path_chorda != NIHIL)
            {
                *path_chorda = chorda_ex_literis(full_path, piscina);
            }
        }
    }

    closedir(dir);
}


/* ========================================================================
 * HELPER: WRITE BYTES AS C ARRAY
 * ======================================================================== */

interior vacuum
_write_bytes(FILE* out, constans i8* data, i32 size, b32 est_ultimus)
{
    i32 i;

    per (i = 0; i < size; i++)
    {
        si (i > 0)
        {
            si (i % BYTES_PER_LINE == 0)
            {
                fprintf(out, ",\n    ");
            }
            alioquin
            {
                fprintf(out, ", ");
            }
        }
        alioquin
        {
            fprintf(out, "    ");
        }

        fprintf(out, "0x%02X", (i32)((i8)data[i]) & 0xFF);
    }

    si (size > 0)
    {
        /* Adde comma nisi est ultimus sectio */
        si (!est_ultimus)
        {
            fprintf(out, ",\n");
        }
        alioquin
        {
            fprintf(out, "\n");
        }
    }
}


/* ========================================================================
 * HELPER: WRITE i32 LITTLE-ENDIAN
 * ======================================================================== */

interior vacuum
_write_i32_le(i8* buf, i32 value)
{
    buf[0] = (i8)(value & 0xFF);
    buf[1] = (i8)((value >> VIII) & 0xFF);
    buf[2] = (i8)((value >> XVI) & 0xFF);
    buf[3] = (i8)((value >> XXIV) & 0xFF);
}


/* ========================================================================
 * PROCESS SECTION
 * ======================================================================== */

interior b32
_process_section(
    constans character* section_name,
    Xar*                file_patterns,
    b32                 compress,
    constans character* config_dir,
    Piscina*            piscina)
{
    Xar*        file_paths;
    Xar*        entries;
    i32         i;
    i32         num_files;
    i32         toc_size;
    i32         string_table_offset;
    i32         string_table_size;
    i32         data_offset;
    i32         current_string_offset;
    i32         current_data_offset;
    character   header_path[1024];
    character   source_path[1024];
    FILE*       header_file;
    FILE*       source_file;
    i8          header_buf[XII];
    i8          toc_entry[XX];

    /* Collect all file paths from glob patterns */
    file_paths = xar_creare(piscina, (i32)magnitudo(chorda));

    per (i = 0; i < xar_numerus(file_patterns); i++)
    {
        chorda* pattern;

        pattern = (chorda*)xar_obtinere(file_patterns, i);
        si (pattern != NIHIL)
        {
            character pattern_cstr[512];
            i32       len;

            len = (i32)pattern->mensura;
            si (len > 511) len = 511;
            memcpy(pattern_cstr, pattern->datum, (size_t)len);
            pattern_cstr[len] = '\0';

            _expand_glob(pattern_cstr, config_dir, file_paths, piscina);
        }
    }

    num_files = xar_numerus(file_paths);
    si (num_files == 0)
    {
        fprintf(stderr, "Warning: nullum filum inventum pro sectione '%s'\n", section_name);
        redde VERUM;  /* Not an error, just no files */
    }

    printf("  Sectio '%s': %d fila\n", section_name, num_files);

    /* Load and optionally compress each file */
    entries = xar_creare(piscina, (i32)magnitudo(FileEntry));

    per (i = 0; i < num_files; i++)
    {
        chorda*    path;
        FileEntry* entry;
        character  full_path[1024];
        i32        path_len;

        path = (chorda*)xar_obtinere(file_paths, i);
        si (path == NIHIL)
        {
            perge;
        }

        /* Build full path */
        path_len = (i32)path->mensura;
        si (path_len > 1000) path_len = 1000;

        si (config_dir != NIHIL && strlen(config_dir) > 0)
        {
            snprintf(full_path, sizeof(full_path), "%s/%.*s",
                     config_dir, path_len, path->datum);
        }
        alioquin
        {
            memcpy(full_path, path->datum, (size_t)path_len);
            full_path[path_len] = '\0';
        }

        entry = (FileEntry*)xar_addere(entries);
        si (entry == NIHIL)
        {
            fprintf(stderr, "Error: allocatio fallita\n");
            redde FALSUM;
        }

        entry->via = *path;
        entry->datum = filum_legere_totum(full_path, piscina);

        si (entry->datum.mensura == 0)
        {
            fprintf(stderr, "Warning: non possum legere '%s'\n", full_path);
            /* Continue anyway - might be empty file */
        }

        /* Compress if requested */
        si (compress && entry->datum.mensura > 0)
        {
            FlaturaFructus res;

            res = flatura_gzip_deflare(entry->datum.datum,
                                       (i32)entry->datum.mensura,
                                       FLATURA_COMPRESSIO_ORDINARIA,
                                       piscina);

            si (res.status == FLATURA_STATUS_OK)
            {
                entry->compressa.datum = res.datum;
                entry->compressa.mensura = res.mensura;
                entry->est_compressa = VERUM;
            }
            alioquin
            {
                /* Compression failed - use uncompressed */
                entry->compressa = entry->datum;
                entry->est_compressa = FALSUM;
            }
        }
        alioquin
        {
            entry->compressa = entry->datum;
            entry->est_compressa = FALSUM;
        }
    }

    /* Calculate offsets
     *
     * Layout:
     *   Header: 12 bytes (magic, version, count)
     *   TOC: num_files * 20 bytes
     *   String table: all paths concatenated (NOT null-terminated in our format)
     *   Data: all file data concatenated
     */
    toc_size = num_files * XX;
    string_table_offset = XII + toc_size;

    /* Calculate string table size */
    string_table_size = 0;
    per (i = 0; i < xar_numerus(entries); i++)
    {
        FileEntry* entry;

        entry = (FileEntry*)xar_obtinere(entries, i);
        string_table_size += (i32)entry->via.mensura;
    }

    data_offset = string_table_offset + string_table_size;

    /* Generate output file paths */
    si (config_dir != NIHIL && strlen(config_dir) > 0)
    {
        snprintf(header_path, sizeof(header_path), "%s/capsula_%s.h",
                 config_dir, section_name);
        snprintf(source_path, sizeof(source_path), "%s/capsula_%s.c",
                 config_dir, section_name);
    }
    alioquin
    {
        snprintf(header_path, sizeof(header_path), "capsula_%s.h", section_name);
        snprintf(source_path, sizeof(source_path), "capsula_%s.c", section_name);
    }

    /* Write header file */
    header_file = fopen(header_path, "w");
    si (header_file == NIHIL)
    {
        fprintf(stderr, "Error: non possum creare '%s'\n", header_path);
        redde FALSUM;
    }

    fprintf(header_file, "/* capsula_%s.h - Generated by capsula_generare */\n\n",
            section_name);
    fprintf(header_file, "#ifndef CAPSULA_%s_H\n", section_name);
    fprintf(header_file, "#define CAPSULA_%s_H\n\n", section_name);
    fprintf(header_file, "#include \"capsula.h\"\n\n");
    fprintf(header_file, "externus constans CapsulaEmbed capsula_%s;\n\n", section_name);
    fprintf(header_file, "#endif /* CAPSULA_%s_H */\n", section_name);

    fclose(header_file);
    printf("    Generated: %s\n", header_path);

    /* Write source file */
    source_file = fopen(source_path, "w");
    si (source_file == NIHIL)
    {
        fprintf(stderr, "Error: non possum creare '%s'\n", source_path);
        redde FALSUM;
    }

    fprintf(source_file, "/* capsula_%s.c - Generated by capsula_generare */\n\n",
            section_name);
    fprintf(source_file, "#include \"capsula_%s.h\"\n\n", section_name);
    fprintf(source_file, "hic_manens constans i8 _capsula_%s_data[] = {\n", section_name);

    /* Write header: magic, version, count */
    _write_i32_le(header_buf, (i32)CAPSULA_MAGICA);
    _write_i32_le(header_buf + IV, (i32)CAPSULA_VERSIO);
    _write_i32_le(header_buf + VIII, num_files);
    _write_bytes(source_file, header_buf, XII, FALSUM);

    /* Write TOC entries */
    current_string_offset = string_table_offset;
    current_data_offset = data_offset;

    per (i = 0; i < xar_numerus(entries); i++)
    {
        FileEntry* entry;
        i32        comp_size;
        i32        raw_size;

        entry = (FileEntry*)xar_obtinere(entries, i);

        comp_size = (i32)entry->compressa.mensura;
        raw_size = (i32)entry->datum.mensura;

        /* If not compressed, sizes are equal */
        si (!entry->est_compressa)
        {
            comp_size = raw_size;
        }

        _write_i32_le(toc_entry, current_string_offset);
        _write_i32_le(toc_entry + IV, (i32)entry->via.mensura);
        _write_i32_le(toc_entry + VIII, current_data_offset);
        _write_i32_le(toc_entry + XII, comp_size);
        _write_i32_le(toc_entry + XVI, raw_size);

        fprintf(source_file, "    /* TOC[%d]: %.*s */\n", i,
                (i32)entry->via.mensura, entry->via.datum);
        _write_bytes(source_file, toc_entry, XX, FALSUM);

        current_string_offset += (i32)entry->via.mensura;
        current_data_offset += comp_size;
    }

    /* Write string table */
    fprintf(source_file, "    /* String table */\n");
    per (i = 0; i < xar_numerus(entries); i++)
    {
        FileEntry* entry;

        entry = (FileEntry*)xar_obtinere(entries, i);
        _write_bytes(source_file, entry->via.datum, (i32)entry->via.mensura, FALSUM);
    }

    /* Write file data */
    fprintf(source_file, "    /* File data */\n");
    per (i = 0; i < xar_numerus(entries); i++)
    {
        FileEntry* entry;
        b32        est_ultimus;

        entry = (FileEntry*)xar_obtinere(entries, i);
        est_ultimus = (i == xar_numerus(entries) - I);

        fprintf(source_file, "    /* [%d] %.*s (%d bytes%s) */\n", i,
                (i32)entry->via.mensura, entry->via.datum,
                (i32)entry->compressa.mensura,
                entry->est_compressa ? ", gzip" : "");

        _write_bytes(source_file, entry->compressa.datum,
                     (i32)entry->compressa.mensura, est_ultimus);
    }

    fprintf(source_file, "};\n\n");

    /* Write CapsulaEmbed struct */
    fprintf(source_file, "constans CapsulaEmbed capsula_%s = {\n", section_name);
    fprintf(source_file, "    _capsula_%s_data,\n", section_name);
    fprintf(source_file, "    (i32)magnitudo(_capsula_%s_data)\n", section_name);
    fprintf(source_file, "};\n");

    fclose(source_file);
    printf("    Generated: %s\n", source_path);

    redde VERUM;
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(integer argc, character** argv)
{
    Piscina*        piscina;
    chorda          config_content;
    TomlDocumentum* doc;
    constans character* config_path;
    character       config_dir[1024];
    constans character* last_slash;
    i32             i;

    si (argc < II)
    {
        fprintf(stderr, "Usus: %s <config.toml>\n", argv[0]);
        fprintf(stderr, "\nConfig format:\n");
        fprintf(stderr, "  [section_name]\n");
        fprintf(stderr, "  files = [\"path/*.txt\", \"other/*.dat\"]\n");
        fprintf(stderr, "  compress = true\n");
        redde I;
    }

    config_path = argv[I];

    /* Extract directory from config path */
    last_slash = strrchr(config_path, '/');
    si (last_slash != NIHIL)
    {
        i32 dir_len;
        dir_len = (i32)(last_slash - config_path);
        si (dir_len >= 1024) dir_len = 1023;
        memcpy(config_dir, config_path, (size_t)dir_len);
        config_dir[dir_len] = '\0';
    }
    alioquin
    {
        config_dir[0] = '\0';
    }

    piscina = piscina_generare_dynamicum("capsula_generare", CXXVIII * M);

    /* Read config file */
    config_content = filum_legere_totum(config_path, piscina);
    si (config_content.mensura == 0)
    {
        fprintf(stderr, "Error: non possum legere '%s'\n", config_path);
        piscina_destruere(piscina);
        redde I;
    }

    /* Parse TOML */
    doc = toml_legere(config_content, piscina);
    si (!toml_successus(doc))
    {
        fprintf(stderr, "Error: TOML parsing fallita: %.*s\n",
                (i32)toml_error(doc).mensura, toml_error(doc).datum);
        piscina_destruere(piscina);
        redde I;
    }

    printf("Capsula Generare\n");
    printf("================\n");
    printf("Config: %s\n\n", config_path);

    /* Process each section
     *
     * For now, we iterate through known sections by checking for
     * section_name.files patterns in the TOML.
     *
     * TODO: This is a simplified approach. A full implementation would
     * need to enumerate all sections in the TOML document.
     */

    /* For now, let's look for common section names and process them */
    {
        constans character* sections[] = {
            "libri", "fontes", "assets", "templates", "data",
            "books", "fonts", "images", "sounds", "scripts",
            NIHIL
        };

        per (i = 0; sections[i] != NIHIL; i++)
        {
            character key_files[128];
            character key_compress[128];
            Xar*      files;
            b32       compress;

            snprintf(key_files, sizeof(key_files), "%s_files", sections[i]);
            snprintf(key_compress, sizeof(key_compress), "%s_compress", sections[i]);

            files = toml_capere_tabulatum(doc, key_files);

            si (files != NIHIL && xar_numerus(files) > 0)
            {
                compress = toml_capere_boolean(doc, key_compress);

                si (!_process_section(sections[i], files, compress,
                                      config_dir[0] ? config_dir : NIHIL, piscina))
                {
                    piscina_destruere(piscina);
                    redde I;
                }
            }
        }
    }

    printf("\nFactum!\n");

    piscina_destruere(piscina);
    redde 0;
}
