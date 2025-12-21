/* librarium_merge.c - Merge LLM metadata into librarium.stml */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "stml.h"
#include "toml.h"
#include "xar.h"
#include "internamentum.h"
#include "iter_directoria.h"
#include <stdio.h>
#include <stdlib.h>

/* ==================================================
 * Configuratio
 * ================================================== */

#define VIA_RAW      "gutenberg-data/raw"
#define VIA_OUTPUT   "librarium.stml"

/* ==================================================
 * Contextus
 * ================================================== */

nomen structura {
    Piscina*              piscina;
    InternamentumChorda*  intern;
    StmlNodus*            radix;           /* Radix librarium */
    i32                   libri_processati;
    i32                   libri_errores;
} MergeContextus;

/* ==================================================
 * Auxiliares
 * ================================================== */

/* Convertere chorda ad C string (null-terminated) */
hic_manens character*
_chorda_ad_cstr(chorda ch, Piscina* piscina)
{
    character* fructus;
    i32 i;

    fructus = (character*)piscina_allocare(piscina, ch.mensura + 1);
    per (i = 0; i < ch.mensura; i++) {
        fructus[i] = (character)ch.datum[i];
    }
    fructus[ch.mensura] = '\0';

    redde fructus;
}

/* Extrahere numerum libri ex titulo fili (e.g., "10553.txt" -> 10553) */
hic_manens s32
_extrahere_numerum(chorda titulus)
{
    s32 numerus = 0;
    i32 i;

    per (i = 0; i < titulus.mensura; i++) {
        character c = (character)titulus.datum[i];
        si (c >= '0' && c <= '9') {
            numerus = numerus * 10 + (c - '0');
        } alioquin {
            frange;
        }
    }

    redde numerus;
}

/* Invenire ultimum <result> blocum in textu */
hic_manens chorda
_invenire_ultimum_result(chorda textus, Piscina* piscina)
{
    chorda fructus;
    constans character* tag_init = "<result>";
    constans character* tag_finis = "</result>";
    s32 init_len = 8;  /* length of "<result>" */
    s32 finis_len = 9; /* length of "</result>" */
    s32 ultima_initium = -1;
    s32 ultima_finis = -1;
    s32 i;

    fructus.datum = NIHIL;
    fructus.mensura = 0;

    /* Quaerere omnes <result> blocos, recordare ultimum */
    per (i = 0; i <= (s32)textus.mensura - init_len; i++) {
        b32 congruit = VERUM;
        s32 j;
        per (j = 0; j < init_len; j++) {
            si ((character)textus.datum[(i32)(i + j)] != tag_init[j]) {
                congruit = FALSUM;
                frange;
            }
        }
        si (congruit) {
            ultima_initium = (s32)i + init_len;
        }
    }

    si (ultima_initium < 0) {
        redde fructus;
    }

    /* Quaerere </result> post ultimum initium */
    per (i = ultima_initium; i <= (s32)textus.mensura - finis_len; i++) {
        b32 congruit = VERUM;
        s32 j;
        per (j = 0; j < finis_len; j++) {
            si ((character)textus.datum[(i32)(i + j)] != tag_finis[j]) {
                congruit = FALSUM;
                frange;
            }
        }
        si (congruit) {
            ultima_finis = (s32)i;
            frange;
        }
    }

    si (ultima_finis < 0) {
        redde fructus;
    }

    /* Copiare contentum inter tags */
    {
        s32 len = ultima_finis - ultima_initium;
        s32 k;
        fructus.datum = (i8*)piscina_allocare(piscina, (i32)len);
        fructus.mensura = (i32)len;
        per (k = 0; k < len; k++) {
            fructus.datum[k] = textus.datum[(i32)(ultima_initium + k)];
        }
    }

    redde fructus;
}

/* Extrahere TOML inter ``` fences, vel totum contentum si non sunt fences */
hic_manens chorda
_extrahere_toml(chorda result_content, Piscina* piscina)
{
    chorda fructus;
    s32 initium = -1;
    s32 finis = -1;
    s32 i;
    s32 fence_count = 0;

    fructus.datum = NIHIL;
    fructus.mensura = 0;

    /* Quaerere primum ``` (post "toml" si praesens) */
    per (i = 0; i < (s32)result_content.mensura - 2; i++) {
        si ((character)result_content.datum[(i32)i] == '`' &&
            (character)result_content.datum[(i32)i + 1] == '`' &&
            (character)result_content.datum[(i32)i + 2] == '`') {
            si (fence_count == 0) {
                /* Saltare ad finem lineae */
                initium = i + 3;
                dum (initium < (s32)result_content.mensura &&
                     (character)result_content.datum[(i32)initium] != '\n') {
                    initium++;
                }
                si (initium < (s32)result_content.mensura) {
                    initium++; /* Saltare \n */
                }
                fence_count++;
            } alioquin {
                finis = i;
                frange;
            }
        }
    }

    /* Si non inventae fences, uti toto contentu */
    si (initium < 0 || finis < 0 || finis <= initium) {
        s32 start = 0;
        s32 end = (s32)result_content.mensura;
        s32 k;

        /* Saltare spatium initiale */
        dum (start < end) {
            character c = (character)result_content.datum[(i32)start];
            si (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                start++;
            } alioquin {
                frange;
            }
        }

        /* Saltare spatium finale */
        dum (end > start) {
            character c = (character)result_content.datum[(i32)(end - 1)];
            si (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                end--;
            } alioquin {
                frange;
            }
        }

        si (end <= start) {
            redde fructus;
        }

        fructus.datum = (i8*)piscina_allocare(piscina, (i32)(end - start));
        fructus.mensura = (i32)(end - start);
        per (k = 0; k < end - start; k++) {
            fructus.datum[k] = result_content.datum[(i32)(start + k)];
        }

        redde fructus;
    }

    /* Copiare TOML inter fences */
    {
        s32 len = finis - initium;
        s32 k;
        fructus.datum = (i8*)piscina_allocare(piscina, (i32)len);
        fructus.mensura = (i32)len;
        per (k = 0; k < len; k++) {
            fructus.datum[k] = result_content.datum[(i32)(initium + k)];
        }
    }

    redde fructus;
}

/* Convertere s32 ad chorda */
hic_manens chorda
_s32_ad_chorda(s32 numerus, Piscina* piscina)
{
    character buffer[32];
    i32 i = 0;
    s32 n = numerus;
    i32 start = 0;
    i32 len;
    i32 j;
    chorda fructus;

    si (n < 0) {
        buffer[i++] = '-';
        n = -n;
        start = 1;
    }

    si (n == 0) {
        buffer[i++] = '0';
    } alioquin {
        dum (n > 0) {
            buffer[i++] = (character)('0' + n % 10);
            n /= 10;
        }
    }

    len = i;
    /* Reverse numerus part */
    per (j = start; j < start + (len - start) / 2; j++) {
        character temp = buffer[j];
        buffer[j] = buffer[len - 1 - (j - start)];
        buffer[len - 1 - (j - start)] = temp;
    }

    fructus.datum = (i8*)piscina_allocare(piscina, (i32)len);
    fructus.mensura = (i32)len;
    per (j = 0; j < len; j++) {
        fructus.datum[j] = (i8)buffer[j];
    }

    redde fructus;
}

/* Extrahere annus ex liber nodo */
hic_manens s32
_extrahere_annus(StmlNodus* liber)
{
    StmlNodus* annus_nodus;
    StmlNodus* textus;
    chorda* valor;
    s32 annus = 0;
    i32 i;

    annus_nodus = stml_invenire_liberum(liber, "annus");
    si (annus_nodus == NIHIL) {
        redde 9999; /* Libri sine anno ad finem */
    }

    textus = stml_primus_liberum(annus_nodus);
    si (textus == NIHIL) {
        redde 9999;
    }

    valor = textus->valor; /* Textus nodi valor est chorda* */
    si (valor == NIHIL || valor->datum == NIHIL || valor->mensura == 0) {
        redde 9999;
    }

    /* Parsare numerum */
    per (i = 0; i < valor->mensura; i++) {
        character c = (character)valor->datum[i];
        si (c >= '0' && c <= '9') {
            annus = annus * 10 + (c - '0');
        } alioquin si (c == '-' && annus == 0) {
            /* Saltare signum negativum initiale */
        } alioquin {
            frange;
        }
    }

    redde annus;
}

/* Ordinare liberos per annum (insertion sort) */
hic_manens vacuum
_ordinare_per_annum(StmlNodus* radix, Piscina* piscina)
{
    StmlNodus** libri;
    i32 numerus = 0;
    i32 capacitas = 128;
    StmlNodus* liber;
    s32 i;
    s32 j;

    /* Colligere omnes liberos */
    libri = (StmlNodus**)piscina_allocare(piscina, (i32)sizeof(StmlNodus*) * capacitas);

    liber = stml_primus_liberum(radix);
    dum (liber != NIHIL) {
        si (numerus >= capacitas) {
            frange;
        }
        libri[numerus++] = liber;
        liber = stml_frater_proximus(liber);
    }

    si (numerus <= 1) {
        redde;
    }

    /* Insertion sort per annum */
    per (i = 1; i < (s32)numerus; i++) {
        StmlNodus* insertandus = libri[i];
        s32 annus_insertandi = _extrahere_annus(insertandus);
        j = i - 1;

        dum (j >= 0 && _extrahere_annus(libri[j]) > annus_insertandi) {
            libri[j + 1] = libri[j];
            j--;
        }
        libri[j + 1] = insertandus;
    }

    /* Vacare liberos et readdere in ordine */
    stml_vacare_liberos(radix);

    per (i = 0; i < (s32)numerus; i++) {
        stml_liberum_addere(radix, libri[i]);
    }
}

/* Invenire liber nodum pro numero */
hic_manens StmlNodus*
_invenire_liber(StmlNodus* radix, s32 numerus)
{
    StmlNodus* liber;
    chorda numerus_str;
    character buffer[16];
    i32 i = 0;
    s32 n = numerus;
    i32 len;
    i32 j;

    /* Convertere numerum ad buffer */
    si (n == 0) {
        buffer[i++] = '0';
    } alioquin {
        dum (n > 0) {
            buffer[i++] = (character)('0' + n % 10);
            n /= 10;
        }
    }
    len = i;
    /* Reverse */
    per (j = 0; j < len / 2; j++) {
        character temp = buffer[j];
        buffer[j] = buffer[len - 1 - j];
        buffer[len - 1 - j] = temp;
    }

    numerus_str.datum = (i8*)buffer;
    numerus_str.mensura = (i32)len;

    /* Quaerere existentem liber */
    liber = stml_invenire_liberum(radix, "liber");
    dum (liber != NIHIL) {
        chorda* attr_numerus = stml_attributum_capere(liber, "numerus");
        si (attr_numerus != NIHIL && attr_numerus->mensura == numerus_str.mensura) {
            b32 aequalis = VERUM;
            s32 k;
            per (k = 0; k < (s32)numerus_str.mensura; k++) {
                si (attr_numerus->datum[k] != numerus_str.datum[k]) {
                    aequalis = FALSUM;
                    frange;
                }
            }
            si (aequalis) {
                redde liber;
            }
        }
        liber = stml_frater_proximus(liber);
    }

    redde NIHIL;
}

/* Addere vel actualizare elementum textus */
hic_manens vacuum
_addere_elementum_textus(MergeContextus* ctx, StmlNodus* liber,
                         constans character* titulus, chorda valor)
{
    StmlNodus* existens;
    StmlNodus* novum;
    StmlNodus* textus;

    si (valor.datum == NIHIL || valor.mensura == 0) {
        redde;
    }

    /* Verificare si iam existit */
    existens = stml_invenire_liberum(liber, titulus);
    si (existens != NIHIL) {
        redde;
    }

    /* Creare novum elementum */
    novum = stml_elementum_creare(ctx->piscina, ctx->intern, titulus);
    textus = stml_textum_creare_ex_chorda(ctx->piscina, ctx->intern, valor);
    stml_liberum_addere(novum, textus);
    stml_liberum_addere(liber, novum);
}

/* Addere tags */
hic_manens vacuum
_addere_tags(MergeContextus* ctx, StmlNodus* liber, Xar* tags)
{
    StmlNodus* tags_nodus;
    i32 num;
    i32 i;

    si (tags == NIHIL) {
        redde;
    }

    num = xar_numerus(tags);
    si (num == 0) {
        redde;
    }

    /* Verificare si tags iam existit */
    si (stml_invenire_liberum(liber, "tags") != NIHIL) {
        redde;
    }

    tags_nodus = stml_elementum_creare(ctx->piscina, ctx->intern, "tags");

    per (i = 0; i < num; i++) {
        chorda* tag = (chorda*)xar_obtinere(tags, i);
        StmlNodus* tag_elem = stml_elementum_creare(ctx->piscina, ctx->intern, "tag");
        StmlNodus* tag_text = stml_textum_creare_ex_chorda(ctx->piscina, ctx->intern, *tag);
        stml_liberum_addere(tag_elem, tag_text);
        stml_liberum_addere(tags_nodus, tag_elem);
    }

    stml_liberum_addere(liber, tags_nodus);
}

/* Processare unum filum LLM */
hic_manens s32
_processare_filum(chorda via_plena, constans DirectoriumIntroitus* introitus,
                  vacuum* contextus)
{
    MergeContextus* ctx = (MergeContextus*)contextus;
    chorda titulus = introitus->titulus;
    s32 numerus;
    character* via_cstr;
    chorda contentum;
    chorda result_block;
    chorda toml_text;
    TomlDocumentum* doc;
    StmlNodus* liber;
    chorda valor_titulus;
    chorda valor_auctor;
    s32 valor_annus;
    Xar* valor_tags;
    chorda valor_summarium;
    chorda valor_notae;

    /* Ignorare si non est .txt */
    si (titulus.mensura < 4) {
        redde 0;
    }
    si ((character)titulus.datum[titulus.mensura - 4] != '.' ||
        (character)titulus.datum[titulus.mensura - 3] != 't' ||
        (character)titulus.datum[titulus.mensura - 2] != 'x' ||
        (character)titulus.datum[titulus.mensura - 1] != 't') {
        redde 0;
    }

    numerus = _extrahere_numerum(titulus);
    si (numerus == 0) {
        redde 0;
    }

    imprimere("Processando %d...\n", numerus);

    /* Legere filum */
    via_cstr = _chorda_ad_cstr(via_plena, ctx->piscina);
    contentum = filum_legere_totum(via_cstr, ctx->piscina);
    si (contentum.datum == NIHIL || contentum.mensura == 0) {
        imprimere("  Error: non potest legere filum\n");
        ctx->libri_errores++;
        redde 0;
    }

    /* Extrahere ultimum <result> */
    result_block = _invenire_ultimum_result(contentum, ctx->piscina);
    si (result_block.datum == NIHIL) {
        imprimere("  Error: non inventum <result>\n");
        ctx->libri_errores++;
        redde 0;
    }

    /* Extrahere TOML */
    toml_text = _extrahere_toml(result_block, ctx->piscina);
    si (toml_text.datum == NIHIL) {
        imprimere("  Error: non inventum TOML\n");
        ctx->libri_errores++;
        redde 0;
    }

    /* Parsare TOML */
    doc = toml_legere(toml_text, ctx->piscina);
    si (!toml_successus(doc)) {
        imprimere("  Error: TOML parsatio fallavit\n");
        ctx->libri_errores++;
        redde 0;
    }

    /* Invenire vel creare liber */
    liber = _invenire_liber(ctx->radix, numerus);
    si (liber == NIHIL) {
        chorda numerus_ch = _s32_ad_chorda(numerus, ctx->piscina);
        character* numerus_cstr = _chorda_ad_cstr(numerus_ch, ctx->piscina);
        liber = stml_elementum_creare(ctx->piscina, ctx->intern, "liber");
        stml_attributum_addere(liber, ctx->piscina, ctx->intern, "numerus", numerus_cstr);
        stml_liberum_addere(ctx->radix, liber);
    }

    /* Extrahere valores */
    valor_titulus = toml_capere_chorda(doc, "Title");
    valor_auctor = toml_capere_chorda(doc, "Author");
    valor_annus = toml_capere_numerum(doc, "Year");
    valor_tags = toml_capere_tabulatum(doc, "Tags");
    valor_summarium = toml_capere_chorda(doc, "Summary");
    valor_notae = toml_capere_chorda(doc, "Notes");

    /* Addere metadata */
    _addere_elementum_textus(ctx, liber, "titulus", valor_titulus);
    _addere_elementum_textus(ctx, liber, "auctor", valor_auctor);

    si (valor_annus != 0) {
        chorda annus_ch = _s32_ad_chorda(valor_annus, ctx->piscina);
        _addere_elementum_textus(ctx, liber, "annus", annus_ch);
    }

    _addere_tags(ctx, liber, valor_tags);
    _addere_elementum_textus(ctx, liber, "summarium", valor_summarium);
    _addere_elementum_textus(ctx, liber, "notae", valor_notae);

    /* Actualizare status */
    stml_attributum_addere(liber, ctx->piscina, ctx->intern, "status", "enriched");

    ctx->libri_processati++;
    imprimere("  OK\n");

    redde 0;
}

/* ==================================================
 * Principale
 * ================================================== */

s32 principale(s32 argc, character** argv)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    MergeContextus ctx;
    DirectoriumFiltrum filtrum;
    ChordaAedificator* aedificator;
    chorda output;

    (vacuum)argc;
    (vacuum)argv;

    imprimere("=== LIBRARIUM MERGE ===\n\n");

    /* Creare piscina et internamentum */
    piscina = piscina_generare_dynamicum("merge", MMMMXCVI * LXIV);
    intern = internamentum_creare(piscina);

    /* Initialisare contextum */
    ctx.piscina = piscina;
    ctx.intern = intern;
    ctx.radix = stml_elementum_creare(piscina, intern, "librarium");
    ctx.libri_processati = 0;
    ctx.libri_errores = 0;

    /* Ambulare per raw directoria */
    imprimere("Legendo fila ex %s...\n\n", VIA_RAW);

    filtrum = directorium_filtrum_omnia();
    filtrum.profunditas_max = 1; /* Unum nivellum */

    directorium_ambulare(VIA_RAW, &filtrum, _processare_filum, &ctx, piscina);

    /* Ordinare per annum */
    imprimere("\nOrdinando libros per annum...\n");
    _ordinare_per_annum(ctx.radix, piscina);

    /* Scribere output */
    imprimere("Scribendo %s...\n", VIA_OUTPUT);

    aedificator = chorda_aedificator_creare(piscina, MMMMXCVI * XVI);
    stml_scribere_ad_aedificator(ctx.radix, aedificator, VERUM, 0);
    output = chorda_aedificator_finire(aedificator);

    si (filum_scribere(VIA_OUTPUT, output)) {
        imprimere("Scriptum cum successu.\n");
    } alioquin {
        imprimere("Error: non potest scribere %s\n", VIA_OUTPUT);
    }

    /* Compendium */
    imprimere("\n=== COMPENDIUM ===\n");
    imprimere("Libri processati: %d\n", ctx.libri_processati);
    imprimere("Errores:          %d\n", ctx.libri_errores);

    piscina_destruere(piscina);

    redde 0;
}
