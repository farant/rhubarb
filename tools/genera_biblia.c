/* genera_biblia.c - Generate biblia_dr.c from SQLite database
 *
 * Compile: gcc -o genera_biblia tools/genera_biblia.c -lsqlite3
 * Usage:   ./genera_biblia events.db > lib/biblia_dr.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

/* Book info structure */
typedef struct {
    const char* abbr;
    const char* name_latin;
    const char* name_english;
    int enum_index;
} BookInfo;

/* Douay-Rheims canonical order with enum indices */
static const BookInfo books[] = {
    { "GEN", "Genesis", "Genesis", 0 },
    { "EXO", "Exodus", "Exodus", 1 },
    { "LEV", "Leviticus", "Leviticus", 2 },
    { "NUM", "Numeri", "Numbers", 3 },
    { "DEU", "Deuteronomium", "Deuteronomy", 4 },
    { "JOS", "Josue", "Josue", 5 },
    { "JDG", "Judicum", "Judges", 6 },
    { "RUT", "Ruth", "Ruth", 7 },
    { "1SA", "I Regum", "1 Samuel", 8 },
    { "2SA", "II Regum", "2 Samuel", 9 },
    { "1KI", "III Regum", "1 Kings", 10 },
    { "2KI", "IV Regum", "2 Kings", 11 },
    { "1CH", "I Paralipomenon", "1 Chronicles", 12 },
    { "2CH", "II Paralipomenon", "2 Chronicles", 13 },
    { "EZR", "I Esdrae", "Ezra", 14 },
    { "NEH", "II Esdrae", "Nehemiah", 15 },
    { "TOB", "Tobiae", "Tobias", 16 },
    { "JDT", "Judith", "Judith", 17 },
    { "EST", "Esther", "Esther", 18 },
    { "JOB", "Job", "Job", 19 },
    { "PSA", "Psalmorum", "Psalms", 20 },
    { "PRO", "Proverbiorum", "Proverbs", 21 },
    { "ECC", "Ecclesiastes", "Ecclesiastes", 22 },
    { "SOL", "Canticum", "Canticle of Canticles", 23 },
    { "WIS", "Sapientiae", "Wisdom", 24 },
    { "SIR", "Ecclesiasticus", "Ecclesiasticus", 25 },
    { "ISA", "Isaiae", "Isaias", 26 },
    { "JER", "Jeremiae", "Jeremias", 27 },
    { "LAM", "Lamentationes", "Lamentations", 28 },
    { "BAR", "Baruch", "Baruch", 29 },
    { "EZE", "Ezechielis", "Ezechiel", 30 },
    { "DAN", "Danielis", "Daniel", 31 },
    { "HOS", "Osee", "Osee", 32 },
    { "JOE", "Joel", "Joel", 33 },
    { "AMO", "Amos", "Amos", 34 },
    { "OBA", "Abdiae", "Abdias", 35 },
    { "JON", "Jonae", "Jonas", 36 },
    { "MIC", "Michaeae", "Micheas", 37 },
    { "NAH", "Nahum", "Nahum", 38 },
    { "HAB", "Habacuc", "Habacuc", 39 },
    { "ZEP", "Sophoniae", "Sophonias", 40 },
    { "HAG", "Aggaei", "Aggeus", 41 },
    { "ZEC", "Zachariae", "Zacharias", 42 },
    { "MAL", "Malachiae", "Malachias", 43 },
    { "1MA", "I Machabaeorum", "1 Machabees", 44 },
    { "2MA", "II Machabaeorum", "2 Machabees", 45 },
    { "MAT", "Matthaeus", "Matthew", 46 },
    { "MAR", "Marcus", "Mark", 47 },
    { "LUK", "Lucas", "Luke", 48 },
    { "JOH", "Joannes", "John", 49 },
    { "ACT", "Actus", "Acts", 50 },
    { "ROM", "Romanos", "Romans", 51 },
    { "1CO", "I Corinthios", "1 Corinthians", 52 },
    { "2CO", "II Corinthios", "2 Corinthians", 53 },
    { "GAL", "Galatas", "Galatians", 54 },
    { "EPH", "Ephesios", "Ephesians", 55 },
    { "PHI", "Philippenses", "Philippians", 56 },
    { "COL", "Colossenses", "Colossians", 57 },
    { "1TH", "I Thessalonicenses", "1 Thessalonians", 58 },
    { "2TH", "II Thessalonicenses", "2 Thessalonians", 59 },
    { "1TI", "I Timotheum", "1 Timothy", 60 },
    { "2TI", "II Timotheum", "2 Timothy", 61 },
    { "TIT", "Titum", "Titus", 62 },
    { "PHM", "Philemonem", "Philemon", 63 },
    { "HEB", "Hebraeos", "Hebrews", 64 },
    { "JAM", "Jacobi", "James", 65 },
    { "1PE", "I Petri", "1 Peter", 66 },
    { "2PE", "II Petri", "2 Peter", 67 },
    { "1JO", "I Joannis", "1 John", 68 },
    { "2JO", "II Joannis", "2 John", 69 },
    { "3JO", "III Joannis", "3 John", 70 },
    { "JUD", "Judae", "Jude", 71 },
    { "REV", "Apocalypsis", "Apocalypse", 72 },
    { NULL, NULL, NULL, -1 }
};

#define NUM_BOOKS 73

typedef struct {
    int book_idx;
    int chapter;
    int verse;
    int offset;
    int length;
} VerseInfo;

typedef struct {
    int versus_initium;
    int versus_numerus;
} ChapterInfo;

typedef struct {
    int capitula_initium;
    int capitula_numerus;
} BookChapterInfo;

static int find_book_index(const char* abbr) {
    int i;
    for (i = 0; books[i].abbr != NULL; i++) {
        if (strcmp(books[i].abbr, abbr) == 0) {
            return books[i].enum_index;
        }
    }
    return -1;
}

/* Calculate output length after transformation (smart quotes -> ASCII) */
static int calc_output_length(const char* s) {
    const unsigned char* p;
    int len = 0;
    for (p = (const unsigned char*)s; *p; p++) {
        /* UTF-8 smart apostrophe: 3 bytes -> 1 byte */
        if (p[0] == 0xE2 && p[1] == 0x80 && p[2] == 0x99) {
            len++;
            p += 2;
            continue;
        }
        if (*p != '\r') {  /* skip CR */
            len++;
        }
    }
    return len;
}

static void print_escaped_string(const char* s) {
    const unsigned char* p;
    for (p = (const unsigned char*)s; *p; p++) {
        /* Check for UTF-8 smart apostrophe: E2 80 99 -> ASCII ' */
        if (p[0] == 0xE2 && p[1] == 0x80 && p[2] == 0x99) {
            putchar('\'');
            p += 2;  /* skip next 2 bytes (loop will skip 3rd) */
            continue;
        }
        switch (*p) {
            case '\\': printf("\\\\"); break;
            case '"':  printf("\\\""); break;
            case '\n': printf("\\n"); break;
            case '\r': break;  /* skip CR */
            case '\t': printf("\\t"); break;
            default:   putchar((int)*p); break;
        }
    }
}

int main(int argc, char** argv) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;
    const char* db_path;
    int i;

    VerseInfo* verses = NULL;
    int verse_count = 0;
    int verse_capacity = 0;
    int text_offset = 0;

    ChapterInfo* chapters = NULL;
    int chapter_count = 0;
    int chapter_capacity = 0;

    BookChapterInfo book_chapters[NUM_BOOKS];

    int prev_book = -1;
    int prev_chapter = -1;
    int current_chapter_start = 0;
    int current_chapter_verses = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <database.db>\n", argv[0]);
        return 1;
    }

    db_path = argv[1];

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    /* Initialize book_chapters */
    for (i = 0; i < NUM_BOOKS; i++) {
        book_chapters[i].capitula_initium = 0;
        book_chapters[i].capitula_numerus = 0;
    }

    /* Header */
    printf("/* biblia_dr.c - Douay-Rheims Bible (Generated - DO NOT EDIT) */\n");
    printf("/* Generated by tools/genera_biblia */\n");
    printf("\n");
    printf("#include \"biblia.h\"\n");
    printf("#include <string.h>\n");
    printf("\n");

    /* ============================================================
     * Pass 1: Generate text blob and collect verse offsets
     * ============================================================ */

    printf("/* ==================================================\n");
    printf(" * Textus Totalis\n");
    printf(" * ================================================== */\n");
    printf("\n");
    printf("hic_manens constans character textus_dr[] =\n");

    /* Order by Douay-Rheims canonical order, not database id */
    rc = sqlite3_prepare_v2(db,
        "SELECT book, chapter, verse, text FROM bible_dra "
        "ORDER BY CASE book "
        "WHEN 'GEN' THEN 0 WHEN 'EXO' THEN 1 WHEN 'LEV' THEN 2 WHEN 'NUM' THEN 3 "
        "WHEN 'DEU' THEN 4 WHEN 'JOS' THEN 5 WHEN 'JDG' THEN 6 WHEN 'RUT' THEN 7 "
        "WHEN '1SA' THEN 8 WHEN '2SA' THEN 9 WHEN '1KI' THEN 10 WHEN '2KI' THEN 11 "
        "WHEN '1CH' THEN 12 WHEN '2CH' THEN 13 WHEN 'EZR' THEN 14 WHEN 'NEH' THEN 15 "
        "WHEN 'TOB' THEN 16 WHEN 'JDT' THEN 17 WHEN 'EST' THEN 18 WHEN 'JOB' THEN 19 "
        "WHEN 'PSA' THEN 20 WHEN 'PRO' THEN 21 WHEN 'ECC' THEN 22 WHEN 'SOL' THEN 23 "
        "WHEN 'WIS' THEN 24 WHEN 'SIR' THEN 25 WHEN 'ISA' THEN 26 WHEN 'JER' THEN 27 "
        "WHEN 'LAM' THEN 28 WHEN 'BAR' THEN 29 WHEN 'EZE' THEN 30 WHEN 'DAN' THEN 31 "
        "WHEN 'HOS' THEN 32 WHEN 'JOE' THEN 33 WHEN 'AMO' THEN 34 WHEN 'OBA' THEN 35 "
        "WHEN 'JON' THEN 36 WHEN 'MIC' THEN 37 WHEN 'NAH' THEN 38 WHEN 'HAB' THEN 39 "
        "WHEN 'ZEP' THEN 40 WHEN 'HAG' THEN 41 WHEN 'ZEC' THEN 42 WHEN 'MAL' THEN 43 "
        "WHEN '1MA' THEN 44 WHEN '2MA' THEN 45 WHEN 'MAT' THEN 46 WHEN 'MAR' THEN 47 "
        "WHEN 'LUK' THEN 48 WHEN 'JOH' THEN 49 WHEN 'ACT' THEN 50 WHEN 'ROM' THEN 51 "
        "WHEN '1CO' THEN 52 WHEN '2CO' THEN 53 WHEN 'GAL' THEN 54 WHEN 'EPH' THEN 55 "
        "WHEN 'PHI' THEN 56 WHEN 'COL' THEN 57 WHEN '1TH' THEN 58 WHEN '2TH' THEN 59 "
        "WHEN '1TI' THEN 60 WHEN '2TI' THEN 61 WHEN 'TIT' THEN 62 WHEN 'PHM' THEN 63 "
        "WHEN 'HEB' THEN 64 WHEN 'JAM' THEN 65 WHEN '1PE' THEN 66 WHEN '2PE' THEN 67 "
        "WHEN '1JO' THEN 68 WHEN '2JO' THEN 69 WHEN '3JO' THEN 70 WHEN 'JUD' THEN 71 "
        "WHEN 'REV' THEN 72 ELSE 99 END, chapter, verse",
        -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char* book = (const char*)sqlite3_column_text(stmt, 0);
        int chapter = sqlite3_column_int(stmt, 1);
        int verse = sqlite3_column_int(stmt, 2);
        const char* text = (const char*)sqlite3_column_text(stmt, 3);
        int text_len = calc_output_length(text);

        int book_idx = find_book_index(book);

        /* Grow verse array if needed */
        if (verse_count >= verse_capacity) {
            verse_capacity = verse_capacity ? verse_capacity * 2 : 1024;
            verses = realloc(verses, (size_t)verse_capacity * sizeof(VerseInfo));
        }

        /* Track chapter boundaries */
        if (book_idx != prev_book || chapter != prev_chapter) {
            /* Save previous chapter if exists */
            if (prev_book >= 0 && current_chapter_verses > 0) {
                if (chapter_count >= chapter_capacity) {
                    chapter_capacity = chapter_capacity ? chapter_capacity * 2 : 256;
                    chapters = realloc(chapters, (size_t)chapter_capacity * sizeof(ChapterInfo));
                }
                chapters[chapter_count].versus_initium = current_chapter_start;
                chapters[chapter_count].versus_numerus = current_chapter_verses;
                chapter_count++;
            }

            /* Track first chapter index for new book */
            if (book_idx != prev_book && book_idx >= 0 && book_idx < NUM_BOOKS) {
                book_chapters[book_idx].capitula_initium = chapter_count;
            }

            /* Start new chapter */
            current_chapter_start = verse_count;
            current_chapter_verses = 0;
            prev_book = book_idx;
            prev_chapter = chapter;
        }

        verses[verse_count].book_idx = book_idx;
        verses[verse_count].chapter = chapter;
        verses[verse_count].verse = verse;
        verses[verse_count].offset = text_offset;
        verses[verse_count].length = text_len;
        verse_count++;
        current_chapter_verses++;

        /* Update book chapter count */
        if (book_idx >= 0 && book_idx < NUM_BOOKS) {
            if (chapter > book_chapters[book_idx].capitula_numerus) {
                book_chapters[book_idx].capitula_numerus = chapter;
            }
        }

        /* Print text line */
        printf("    \"");
        print_escaped_string(text);
        printf("\\n\"\n");

        text_offset += text_len + 1; /* +1 for newline */
    }

    /* Save final chapter */
    if (prev_book >= 0 && current_chapter_verses > 0) {
        if (chapter_count >= chapter_capacity) {
            chapter_capacity = chapter_capacity ? chapter_capacity * 2 : 256;
            chapters = realloc(chapters, (size_t)chapter_capacity * sizeof(ChapterInfo));
        }
        chapters[chapter_count].versus_initium = current_chapter_start;
        chapters[chapter_count].versus_numerus = current_chapter_verses;
        chapter_count++;
    }

    sqlite3_finalize(stmt);

    printf(";\n");
    printf("\n");
    printf("#define TEXTUS_LONGITUDO %d\n", text_offset);
    printf("\n");

    /* ============================================================
     * Versus Array
     * ============================================================ */

    printf("/* ==================================================\n");
    printf(" * Versus Array\n");
    printf(" * ================================================== */\n");
    printf("\n");
    printf("hic_manens constans Versus versus_dr[] = {\n");

    for (i = 0; i < verse_count; i++) {
        printf("    { %d, %d, %d, %d, %d },\n",
            verses[i].book_idx,
            verses[i].chapter,
            verses[i].verse,
            verses[i].offset,
            verses[i].length);
    }

    printf("};\n");
    printf("\n");
    printf("#define VERSUS_NUMERUS %d\n", verse_count);
    printf("\n");

    /* ============================================================
     * Capitula Array
     * ============================================================ */

    printf("/* ==================================================\n");
    printf(" * Capitula Array\n");
    printf(" * ================================================== */\n");
    printf("\n");
    printf("hic_manens constans Capitulum capitula_dr[] = {\n");

    for (i = 0; i < chapter_count; i++) {
        printf("    { %d, %d },\n",
            chapters[i].versus_numerus,
            chapters[i].versus_initium);
    }

    printf("};\n");
    printf("\n");
    printf("#define CAPITULA_NUMERUS %d\n", chapter_count);
    printf("\n");

    /* ============================================================
     * Libri Array
     * ============================================================ */

    printf("/* ==================================================\n");
    printf(" * Libri Array\n");
    printf(" * ================================================== */\n");
    printf("\n");
    printf("hic_manens constans Liber libri_dr[] = {\n");

    for (i = 0; books[i].abbr != NULL; i++) {
        printf("    { \"%s\", \"%s\", \"%s\", %d, %d },\n",
            books[i].name_latin,
            books[i].name_english,
            books[i].abbr,
            book_chapters[i].capitula_numerus,
            book_chapters[i].capitula_initium);
    }

    printf("};\n");
    printf("\n");
    printf("#define LIBRI_NUMERUS_DR 73\n");
    printf("\n");

    /* ============================================================
     * Biblia Structure
     * ============================================================ */

    printf("/* ==================================================\n");
    printf(" * Biblia Structure\n");
    printf(" * ================================================== */\n");
    printf("\n");
    printf("hic_manens constans Biblia biblia_dr = {\n");
    printf("    /* titulus */ \"Douay-Rheims\",\n");
    printf("    \"1899\",\n");
    printf("    textus_dr,\n");
    printf("    TEXTUS_LONGITUDO,\n");
    printf("    LIBRI_NUMERUS_DR,\n");
    printf("    libri_dr,\n");
    printf("    CAPITULA_NUMERUS,\n");
    printf("    capitula_dr,\n");
    printf("    VERSUS_NUMERUS,\n");
    printf("    versus_dr\n");
    printf("};\n");
    printf("\n");

    /* ============================================================
     * API Functions
     * ============================================================ */

    printf("/* ==================================================\n");
    printf(" * API Functions\n");
    printf(" * ================================================== */\n");
    printf("\n");
    printf("#if defined(__clang__)\n");
    printf("#pragma clang diagnostic push\n");
    printf("#pragma clang diagnostic ignored \"-Wcast-qual\"\n");
    printf("#endif\n");
    printf("\n");
    printf("constans Biblia*\n");
    printf("biblia_obtinere_dr(vacuum)\n");
    printf("{\n");
    printf("    redde &biblia_dr;\n");
    printf("}\n");
    printf("\n");

    /* Optimized biblia_versus using chapter index */
    printf("chorda\n");
    printf("biblia_versus(\n");
    printf("    constans Biblia* biblia,\n");
    printf("    i32 liber,\n");
    printf("    i32 capitulum,\n");
    printf("    i32 versus_num)\n");
    printf("{\n");
    printf("    chorda vacua;\n");
    printf("    constans Liber* lib;\n");
    printf("    constans Capitulum* cap;\n");
    printf("    i32 versus_idx;\n");
    printf("    i32 i;\n");
    printf("\n");
    printf("    vacua.datum = NIHIL;\n");
    printf("    vacua.mensura = 0;\n");
    printf("\n");
    printf("    si (!biblia || !biblia->capitula)\n");
    printf("    {\n");
    printf("        redde vacua;\n");
    printf("    }\n");
    printf("\n");
    printf("    si (liber >= biblia->libri_numerus)\n");
    printf("    {\n");
    printf("        redde vacua;\n");
    printf("    }\n");
    printf("\n");
    printf("    lib = &biblia->libri[liber];\n");
    printf("\n");
    printf("    si (capitulum < 1 || capitulum > lib->capitula_numerus)\n");
    printf("    {\n");
    printf("        redde vacua;\n");
    printf("    }\n");
    printf("\n");
    printf("    cap = &biblia->capitula[lib->capitula_initium + capitulum - 1];\n");
    printf("\n");
    printf("    /* Quaerere solum in hoc capitulo */\n");
    printf("    per (i = 0; i < cap->versus_numerus; i++)\n");
    printf("    {\n");
    printf("        versus_idx = cap->versus_initium + i;\n");
    printf("        si (biblia->versus[versus_idx].versus == versus_num)\n");
    printf("        {\n");
    printf("            chorda result;\n");
    printf("            result.datum = (i8*)(biblia->textus + biblia->versus[versus_idx].textus_initium);\n");
    printf("            result.mensura = biblia->versus[versus_idx].textus_longitudo;\n");
    printf("            redde result;\n");
    printf("        }\n");
    printf("    }\n");
    printf("\n");
    printf("    redde vacua;\n");
    printf("}\n");
    printf("\n");

    printf("constans character*\n");
    printf("biblia_nomen_libri(\n");
    printf("    constans Biblia* biblia,\n");
    printf("    i32 liber)\n");
    printf("{\n");
    printf("    si (!biblia || liber >= biblia->libri_numerus)\n");
    printf("    {\n");
    printf("        redde NIHIL;\n");
    printf("    }\n");
    printf("    redde biblia->libri[liber].nomen_anglicum;\n");
    printf("}\n");
    printf("\n");

    printf("i32\n");
    printf("biblia_capitula_in_libro(\n");
    printf("    constans Biblia* biblia,\n");
    printf("    i32 liber)\n");
    printf("{\n");
    printf("    si (!biblia || liber >= biblia->libri_numerus)\n");
    printf("    {\n");
    printf("        redde 0;\n");
    printf("    }\n");
    printf("    redde biblia->libri[liber].capitula_numerus;\n");
    printf("}\n");
    printf("\n");

    /* Optimized biblia_versus_in_capitulo using chapter index */
    printf("i32\n");
    printf("biblia_versus_in_capitulo(\n");
    printf("    constans Biblia* biblia,\n");
    printf("    i32 liber,\n");
    printf("    i32 capitulum)\n");
    printf("{\n");
    printf("    constans Liber* lib;\n");
    printf("    constans Capitulum* cap;\n");
    printf("\n");
    printf("    si (!biblia || !biblia->capitula)\n");
    printf("    {\n");
    printf("        redde 0;\n");
    printf("    }\n");
    printf("\n");
    printf("    si (liber >= biblia->libri_numerus)\n");
    printf("    {\n");
    printf("        redde 0;\n");
    printf("    }\n");
    printf("\n");
    printf("    lib = &biblia->libri[liber];\n");
    printf("\n");
    printf("    si (capitulum < 1 || capitulum > lib->capitula_numerus)\n");
    printf("    {\n");
    printf("        redde 0;\n");
    printf("    }\n");
    printf("\n");
    printf("    cap = &biblia->capitula[lib->capitula_initium + capitulum - 1];\n");
    printf("    redde cap->versus_numerus;\n");
    printf("}\n");
    printf("\n");
    printf("#if defined(__clang__)\n");
    printf("#pragma clang diagnostic pop\n");
    printf("#endif\n");

    free(verses);
    free(chapters);
    sqlite3_close(db);

    return 0;
}
