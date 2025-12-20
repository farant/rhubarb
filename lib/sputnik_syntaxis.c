#include "sputnik_syntaxis.h"
#include "thema.h"
#include "color.h"
#include "delineare.h"
#include "chorda.h"
#include <string.h>

/* ==================================================
 * Constantae
 * ================================================== */

#define CHAR_LATITUDO   VI      /* 6 pixels per character */
#define CHAR_ALTITUDO   VIII    /* 8 pixels per character */
#define PADDING         II      /* 2 characters padding */


/* ==================================================
 * Verba Clausa (Keywords)
 * ================================================== */

nomen structura {
    constans character* verbum;
    constans character* significatio;
} VerbumClausum;

hic_manens constans VerbumClausum verba_clausa[] = {
    { "sit",       "let (mutable)" },
    { "constans",  "const" },
    { "si",        "if" },
    { "alioquin",  "else" },
    { "dum",       "while" },
    { "per",       "for" },
    { "frange",    "break" },
    { "perge",     "continue" },
    { "redde",     "return" },
    { "functio",   "function" },
    { "entitas",   "entity decl" },
    { "verum",     "true" },
    { "falsum",    "false" },
    { "nihil",     "null" }
};

#define NUMERUS_VERBORUM (magnitudo(verba_clausa) / magnitudo(verba_clausa[0]))


/* ==================================================
 * Literae Speciales
 * ================================================== */

nomen structura {
    constans character* exemplum;
    constans character* descriptio;
} LiteraSpecialis;

hic_manens constans LiteraSpecialis literae_speciales[] = {
    { "9.99$",           "Currency (exact decimal)" },
    { "#tag",            "Tag literal" },
    { "#Namespace::tag", "Namespaced tag" }
};

#define NUMERUS_LITERARUM (magnitudo(literae_speciales) / magnitudo(literae_speciales[0]))


/* ==================================================
 * REPO Methodi
 * ================================================== */

nomen structura {
    constans character* methodus;
    constans character* descriptio;
} MethodusRepo;

hic_manens constans MethodusRepo methodi_repo[] = {
    { "REPO.get(id)",        "Entity by ID" },
    { "REPO.query(genus)",   "Entities by type" },
    { "REPO.tagged(nota)",   "Entities with tag" },
    { "REPO.search(text)",   "Search entities" },
    { "REPO.roots()",        "Root entities" },
    { "REPO.scaffold(g,t)",  "Create/get entity" }
};

#define NUMERUS_METHODI_REPO (magnitudo(methodi_repo) / magnitudo(methodi_repo[0]))


/* ==================================================
 * Methodi Entitatis
 * ================================================== */

nomen structura {
    constans character* methodus;
    constans character* descriptio;
} MethodusEntitatis;

hic_manens constans MethodusEntitatis methodi_entitatis[] = {
    { "add_relation(type, id)",   "Add relation" },
    { "remove_relation(type,id)", "Remove relation" },
    { "related(type)",            "Get related" },
    { "related_all(type)",        "Get all related" },
    { "related_from(type)",       "Reverse relation" },
    { "related_from_all(type)",   "All reverse" },
    { "relation_types()",         "Outgoing types" },
    { "relation_types_from()",    "Incoming types" },
    { "relations()",              "Grouped rels" },
    { "relations_raw()",          "Raw rel data" },
    { "add_tag(nota)",            "Add tag" },
    { "has_tag(nota)",            "Check tag" },
    { "remove_tag(nota)",         "Remove tag" }
};

#define NUMERUS_METHODI_ENTITATIS (magnitudo(methodi_entitatis) / magnitudo(methodi_entitatis[0]))


/* ==================================================
 * Functiones Auxiliares
 * ================================================== */

/* Creare chorda ex C string (view, non allocatio)
 * NOTA: Hic facit view in stringam staticam - non modificare!
 */
hic_manens chorda
_chorda_ex_cstr(
    constans character* cstr)
{
    chorda fructus;
    /* Caute: haec est view, non copia. Chorda non debet modificari. */
    union { constans character* c; i8* m; } u;
    u.c = cstr;

    fructus.datum = u.m;
    fructus.mensura = (i32)strlen(cstr);

    redde fructus;
}


/* ==================================================
 * Creatio
 * ================================================== */

SputnikSyntaxis*
sputnik_syntaxis_creare(
    Piscina* piscina)
{
    SputnikSyntaxis* syntaxis;

    si (!piscina)
    {
        redde NIHIL;
    }

    syntaxis = (SputnikSyntaxis*)piscina_allocare(piscina, magnitudo(SputnikSyntaxis));
    si (!syntaxis)
    {
        redde NIHIL;
    }

    syntaxis->piscina = piscina;

    redde syntaxis;
}


/* ==================================================
 * Reddere
 * ================================================== */

vacuum
sputnik_syntaxis_reddere(
    SputnikSyntaxis* syntaxis,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    ContextusDelineandi* ctx;
    i32 char_lat;
    i32 char_alt;
    i32 px_x;
    i32 px_y;
    i32 linea;
    i32 i;
    Color color_background;
    Color color_text;
    Color color_text_dim;
    Color color_accent;
    i32 pixelum_text;
    i32 pixelum_text_dim;
    i32 pixelum_accent;
    chorda textus;

    (vacuum)syntaxis;
    (vacuum)latitudo;
    (vacuum)altitudo;
    (vacuum)focused;

    si (!tabula)
    {
        redde;
    }

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(syntaxis->piscina, tabula);
    si (!ctx)
    {
        redde;
    }

    char_lat = CHAR_LATITUDO * scala;
    char_alt = CHAR_ALTITUDO * scala;

    /* Colores */
    color_background = thema_color(COLOR_BACKGROUND);
    color_text = thema_color(COLOR_TEXT);
    color_text_dim = thema_color(COLOR_TEXT_DIM);
    color_accent = color_ex_palette(PALETTE_DARK_LEAF);

    pixelum_text = color_ad_pixelum(color_text);
    pixelum_text_dim = color_ad_pixelum(color_text_dim);
    pixelum_accent = color_ad_pixelum(color_accent);

    /* Pingere fondum */
    px_x = x * char_lat;
    px_y = y * char_alt;
    delineare_rectangulum_plenum(ctx, px_x, px_y,
        latitudo * char_lat, altitudo * char_alt, color_background);

    /* === Titulus === */
    linea = y + I;

    textus = _chorda_ex_cstr("=== Sputnik Syntaxis ===");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        textus, pixelum_text, scala);
    linea += II;

    /* === Sectio: Verba Clausa === */
    textus = _chorda_ex_cstr("--- Verba Clausa ---");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        textus, pixelum_text_dim, scala);
    linea++;

    per (i = ZEPHYRUM; i < (i32)NUMERUS_VERBORUM; i++)
    {
        /* Verbum */
        textus = _chorda_ex_cstr(verba_clausa[i].verbum);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + II) * char_lat, linea * char_alt,
            textus, pixelum_accent, scala);

        /* Significatio */
        textus = _chorda_ex_cstr(verba_clausa[i].significatio);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + XIV) * char_lat, linea * char_alt,
            textus, pixelum_text, scala);

        linea++;
    }

    linea++;

    /* === Sectio: Literae Speciales === */
    textus = _chorda_ex_cstr("--- Literae Speciales ---");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        textus, pixelum_text_dim, scala);
    linea++;

    per (i = ZEPHYRUM; i < (i32)NUMERUS_LITERARUM; i++)
    {
        /* Exemplum */
        textus = _chorda_ex_cstr(literae_speciales[i].exemplum);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + II) * char_lat, linea * char_alt,
            textus, pixelum_accent, scala);

        /* Descriptio */
        textus = _chorda_ex_cstr(literae_speciales[i].descriptio);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + XX) * char_lat, linea * char_alt,
            textus, pixelum_text, scala);

        linea++;
    }

    linea++;

    /* === Sectio: REPO === */
    textus = _chorda_ex_cstr("--- REPO ---");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        textus, pixelum_text_dim, scala);
    linea++;

    per (i = ZEPHYRUM; i < (i32)NUMERUS_METHODI_REPO; i++)
    {
        /* Methodus */
        textus = _chorda_ex_cstr(methodi_repo[i].methodus);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + II) * char_lat, linea * char_alt,
            textus, pixelum_accent, scala);

        /* Descriptio */
        textus = _chorda_ex_cstr(methodi_repo[i].descriptio);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + XXVI) * char_lat, linea * char_alt,
            textus, pixelum_text, scala);

        linea++;
    }

    linea++;

    /* === Sectio: Methodi Entitatis === */
    textus = _chorda_ex_cstr("--- Methodi Entitatis ---");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        textus, pixelum_text_dim, scala);
    linea++;

    per (i = ZEPHYRUM; i < (i32)NUMERUS_METHODI_ENTITATIS; i++)
    {
        /* Methodus */
        textus = _chorda_ex_cstr(methodi_entitatis[i].methodus);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + II) * char_lat, linea * char_alt,
            textus, pixelum_accent, scala);

        /* Descriptio */
        textus = _chorda_ex_cstr(methodi_entitatis[i].descriptio);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + XXVIII) * char_lat, linea * char_alt,
            textus, pixelum_text, scala);

        linea++;
    }
}


/* ==================================================
 * Eventus
 * ================================================== */

b32
sputnik_syntaxis_tractare_eventum(
    SputnikSyntaxis*  syntaxis,
    constans Eventus* eventus)
{
    (vacuum)syntaxis;
    (vacuum)eventus;

    /* Widget est read-only - non tractat eventus */
    redde FALSUM;
}
