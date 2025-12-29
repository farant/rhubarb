/* capsula_caudae_adiungere.c - Build Tool for Tail-Appended Assets
 *
 * Legit TOML config, comprimit files, adiungit ad caudam binarii.
 *
 * USUS:
 *   ./bin/capsula_caudae_adiungere config.toml target_binary
 *
 * CONFIG FORMAT (same as capsula_generare):
 *   libri_files = ["*.txt", "librarium.stml"]
 *   libri_compress = true
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "toml.h"
#include "filum.h"
#include "flatura.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>


/* ========================================================================
 * CONSTANTIAE
 * ======================================================================== */

#define CAPSULA_MAGICA           0x53504143  /* "CAPS" */
#define CAPSULA_VERSIO           1

/* Footer magic: "CAPSCLDA" */
#define CAPSULA_CAUDAE_MAGICA_0  0x53504143  /* "CAPS" */
#define CAPSULA_CAUDAE_MAGICA_1  0x4144434C  /* "LCDA" */


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* File entry for building */
nomen structura {
    chorda via;              /* Relative path */
    chorda datum;            /* File contents (original) */
    chorda compressa;        /* Compressed data (or original if no compression) */
    b32    est_compressa;    /* Was compression applied? */
} CaudaeFileEntry;


/* ========================================================================
 * HELPER: GLOB MATCHING
 * ======================================================================== */

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
                redde VERUM;
            }

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

    dum (*pattern == '*')
    {
        pattern++;
    }

    redde *pattern == '\0' && *str == '\0';
}


/* ========================================================================
 * HELPER: EXPAND GLOB
 * ======================================================================== */

interior vacuum
_expand_glob(
    constans character* pattern,
    constans character* base_dir,
    Xar*                files_out,
    Piscina*            piscina)
{
    DIR*                dir;
    structura dirent*   entry;
    character           path_buffer[1024];
    constans character* slash;
    character           dir_part[512];
    constans character* file_pattern;
    i32                 dir_len;

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
            perge;
        }

        si (_glob_congruit(file_pattern, entry->d_name))
        {
            character full_path[1024];
            chorda*   path_chorda;

            si (strcmp(dir_part, ".") == 0)
            {
                snprintf(full_path, sizeof(full_path), "%s", entry->d_name);
            }
            alioquin
            {
                snprintf(full_path, sizeof(full_path), "%s/%s", dir_part, entry->d_name);
            }

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
 * HELPER: WRITE INTEGERS LITTLE-ENDIAN
 * ======================================================================== */

interior vacuum
_write_i32_le(i8* buf, i32 value)
{
    buf[0] = (i8)(value & 0xFF);
    buf[1] = (i8)((value >> VIII) & 0xFF);
    buf[2] = (i8)((value >> XVI) & 0xFF);
    buf[3] = (i8)((value >> XXIV) & 0xFF);
}


interior vacuum
_write_i64_le(i8* buf, i64 value)
{
    buf[0] = (i8)(value & 0xFF);
    buf[1] = (i8)((value >> VIII) & 0xFF);
    buf[2] = (i8)((value >> XVI) & 0xFF);
    buf[3] = (i8)((value >> XXIV) & 0xFF);
    buf[4] = (i8)((value >> 32) & 0xFF);
    buf[5] = (i8)((value >> 40) & 0xFF);
    buf[6] = (i8)((value >> 48) & 0xFF);
    buf[7] = (i8)((value >> 56) & 0xFF);
}


/* ========================================================================
 * APPEND ASSETS TO BINARY
 * ======================================================================== */

interior b32
_append_assets(
    constans character* section_name,
    Xar*                file_patterns,
    b32                 compress,
    constans character* config_dir,
    constans character* target_path,
    Piscina*            piscina)
{
    Xar*        file_paths;
    Xar*        entries;
    FILE*       target_file;
    i64         asset_offset;
    i32         i;
    i32         num_files;
    i32         toc_size;
    i32         string_table_size;
    i32         current_string_offset;
    i32         current_data_offset;
    i32         string_table_offset;
    i32         data_offset;

    /* Collect file paths from glob patterns */
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
        fprintf(stderr, "Error: nullum filum inventum\n");
        redde FALSUM;
    }

    printf("  Sectio '%s': %d fila\n", section_name, num_files);

    /* Load and compress files */
    entries = xar_creare(piscina, (i32)magnitudo(CaudaeFileEntry));

    per (i = 0; i < num_files; i++)
    {
        chorda*          path;
        CaudaeFileEntry* entry;
        character        full_path[1024];
        i32              path_len;

        path = (chorda*)xar_obtinere(file_paths, i);
        si (path == NIHIL)
        {
            perge;
        }

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

        entry = (CaudaeFileEntry*)xar_addere(entries);
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
        }

        /* Compress if requested */
        si (compress && entry->datum.mensura > 0)
        {
            FlaturaFructus res;

            res = flatura_gzip_deflare(entry->datum.datum,
                                       (i32)entry->datum.mensura,
                                       FLATURA_COMPRESSIO_RAPIDA,
                                       piscina);

            si (res.status == FLATURA_STATUS_OK)
            {
                entry->compressa.datum = res.datum;
                entry->compressa.mensura = res.mensura;
                entry->est_compressa = VERUM;
            }
            alioquin
            {
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

    /* Calculate offsets and total sizes */
    toc_size = num_files * XX;
    string_table_offset = XII + toc_size;

    string_table_size = 0;
    per (i = 0; i < xar_numerus(entries); i++)
    {
        CaudaeFileEntry* entry;

        entry = (CaudaeFileEntry*)xar_obtinere(entries, i);
        string_table_size += (i32)entry->via.mensura;
    }

    data_offset = string_table_offset + string_table_size;

    /* Calculate total compressed data size */
    {
        i64 total_data_size;
        i64 total_blob_size;
        i8* blob;
        i8* cursor;

        total_data_size = 0;
        per (i = 0; i < xar_numerus(entries); i++)
        {
            CaudaeFileEntry* entry;

            entry = (CaudaeFileEntry*)xar_obtinere(entries, i);
            total_data_size += (i64)entry->compressa.mensura;
        }

        /* Total blob = header + TOC + strings + data + footer */
        total_blob_size = (i64)XII + (i64)toc_size + (i64)string_table_size +
                          total_data_size + (i64)XVI;

        /* Allocate single buffer pro toto blob */
        blob = (i8*)piscina_allocare(piscina, (memoriae_index)total_blob_size);
        si (blob == NIHIL)
        {
            fprintf(stderr, "Error: allocatio blob fallita\n");
            redde FALSUM;
        }

        cursor = blob;

        /* Open target binary to get current size */
        target_file = fopen(target_path, "rb");
        si (target_file == NIHIL)
        {
            fprintf(stderr, "Error: non possum aperire '%s'\n", target_path);
            redde FALSUM;
        }

        si (fseek(target_file, 0, SEEK_END) != 0)
        {
            fprintf(stderr, "Error: fseek fallita\n");
            fclose(target_file);
            redde FALSUM;
        }

        asset_offset = (i64)ftell(target_file);
        fclose(target_file);

        printf("    Asset offset: %lld bytes\n", (longus longus)asset_offset);

        /* Build blob in memory: CAPS header */
        _write_i32_le(cursor, (i32)CAPSULA_MAGICA);
        _write_i32_le(cursor + IV, (i32)CAPSULA_VERSIO);
        _write_i32_le(cursor + VIII, num_files);
        cursor += XII;

        /* Build TOC entries */
        current_string_offset = string_table_offset;
        current_data_offset = data_offset;

        per (i = 0; i < xar_numerus(entries); i++)
        {
            CaudaeFileEntry* entry;
            i32              comp_size;
            i32              raw_size;

            entry = (CaudaeFileEntry*)xar_obtinere(entries, i);

            comp_size = (i32)entry->compressa.mensura;
            raw_size = (i32)entry->datum.mensura;

            si (!entry->est_compressa)
            {
                comp_size = raw_size;
            }

            _write_i32_le(cursor, current_string_offset);
            _write_i32_le(cursor + IV, (i32)entry->via.mensura);
            _write_i32_le(cursor + VIII, current_data_offset);
            _write_i32_le(cursor + XII, comp_size);
            _write_i32_le(cursor + XVI, raw_size);
            cursor += XX;

            current_string_offset += (i32)entry->via.mensura;
            current_data_offset += comp_size;
        }

        /* Build string table */
        per (i = 0; i < xar_numerus(entries); i++)
        {
            CaudaeFileEntry* entry;

            entry = (CaudaeFileEntry*)xar_obtinere(entries, i);
            memcpy(cursor, entry->via.datum, (size_t)entry->via.mensura);
            cursor += entry->via.mensura;
        }

        /* Build compressed file data */
        per (i = 0; i < xar_numerus(entries); i++)
        {
            CaudaeFileEntry* entry;

            entry = (CaudaeFileEntry*)xar_obtinere(entries, i);

            si (entry->compressa.mensura > 0)
            {
                memcpy(cursor, entry->compressa.datum, (size_t)entry->compressa.mensura);
                cursor += entry->compressa.mensura;
            }
        }

        /* Build footer */
        _write_i64_le(cursor, asset_offset);
        _write_i32_le(cursor + VIII, (i32)CAPSULA_CAUDAE_MAGICA_0);
        _write_i32_le(cursor + XII, (i32)CAPSULA_CAUDAE_MAGICA_1);
        cursor += XVI;

        /* Now write entire blob in one operation */
        target_file = fopen(target_path, "ab");
        si (target_file == NIHIL)
        {
            fprintf(stderr, "Error: non possum aperire '%s' pro appendere\n", target_path);
            redde FALSUM;
        }

        si (fwrite(blob, I, (size_t)total_blob_size, target_file) != (size_t)total_blob_size)
        {
            fprintf(stderr, "Error: fwrite blob fallita\n");
            fclose(target_file);
            redde FALSUM;
        }

        fclose(target_file);
    }

    /* Report statistics */
    {
        i64 total_raw;
        i64 total_compressed;

        total_raw = 0;
        total_compressed = 0;

        per (i = 0; i < xar_numerus(entries); i++)
        {
            CaudaeFileEntry* entry;

            entry = (CaudaeFileEntry*)xar_obtinere(entries, i);
            total_raw += (i64)entry->datum.mensura;
            total_compressed += (i64)entry->compressa.mensura;
        }

        printf("    Raw data:        %lld bytes\n", (longus longus)total_raw);
        printf("    Compressed data: %lld bytes\n", (longus longus)total_compressed);
        printf("    Ratio:           %.1f%%\n",
               total_raw > 0 ? (duplex)total_compressed / (duplex)total_raw * 100.0 : 0.0);
    }

    redde VERUM;
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(integer argc, character** argv)
{
    Piscina*            piscina;
    chorda              config_content;
    TomlDocumentum*     doc;
    constans character* config_path;
    constans character* target_path;
    character           config_dir[1024];
    constans character* last_slash;
    i32                 i;
    b32                 found_section;

    si (argc < III)
    {
        fprintf(stderr, "Usus: %s <config.toml> <target_binary>\n", argv[0]);
        fprintf(stderr, "\nAdiungit assets ad caudam binarii.\n");
        fprintf(stderr, "\nConfig format:\n");
        fprintf(stderr, "  section_files = [\"*.txt\", \"data/*.dat\"]\n");
        fprintf(stderr, "  section_compress = true\n");
        redde I;
    }

    config_path = argv[I];
    target_path = argv[II];

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

    piscina = piscina_generare_dynamicum("capsula_caudae_adiungere", CXXVIII * M);

    /* Read config */
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

    printf("Capsula Caudae Adiungere\n");
    printf("========================\n");
    printf("Config: %s\n", config_path);
    printf("Target: %s\n\n", target_path);

    /* Process sections */
    found_section = FALSUM;

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
                found_section = VERUM;

                si (!_append_assets(sections[i], files, compress,
                                    config_dir[0] ? config_dir : NIHIL,
                                    target_path, piscina))
                {
                    piscina_destruere(piscina);
                    redde I;
                }
            }
        }
    }

    si (!found_section)
    {
        fprintf(stderr, "Error: nulla sectio inventa in config\n");
        piscina_destruere(piscina);
        redde I;
    }

    printf("\nFactum!\n");

    piscina_destruere(piscina);
    redde 0;
}
