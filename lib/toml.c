/* toml.c - Parser pro Subset TOML */

#include "toml.h"
#include "chorda_aedificator.h"
#include <stdio.h>

/* ==================================================
 * Status Parsationis
 * ================================================== */

nomen enumeratio {
    TOML_STATUS_NORMALIS,
    TOML_STATUS_CHORDA_MULTILINEA,
    TOML_STATUS_TABULATUM
} TomlStatus;

/* Contextus parsationis */
nomen structura {
    constans i8* datum;
    i32          mensura;
    i32          positio;
    i32          linea;
    Piscina*     piscina;

    TomlStatus   status;
    chorda       clavis_currens;
    ChordaAedificator* aedificator;  /* Pro multilinea/tabulatum */
    Xar*         tabulatum_currens;  /* Pro tabulatum */
} TomlContextus;

/* ==================================================
 * Functiones Auxiliares
 * ================================================== */

/* Saltare spatia (non newlines) */
hic_manens vacuum
_saltare_spatia(TomlContextus* ctx)
{
    dum (ctx->positio < ctx->mensura) {
        character c = (character)ctx->datum[ctx->positio];
        si (c == ' ' || c == '\t' || c == '\r') {
            ctx->positio++;
        } alioquin {
            frange;
        }
    }
}

/* Saltare ad finem lineae */
hic_manens vacuum
_saltare_ad_finem_lineae(TomlContextus* ctx)
{
    dum (ctx->positio < ctx->mensura && ctx->datum[ctx->positio] != '\n') {
        ctx->positio++;
    }
}

/* Verificare si character est digitus */
hic_manens b32
_est_digitus(character c)
{
    redde c >= '0' && c <= '9';
}

/* Verificare si character est littera vel underscore */
hic_manens b32
_est_clavis_character(character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           c == '_' || c == '-';
}

/* Legere clavim */
hic_manens chorda
_legere_clavim(TomlContextus* ctx)
{
    chorda fructus;
    i32 initium;

    fructus.datum = NIHIL;
    fructus.mensura = 0;

    _saltare_spatia(ctx);

    initium = ctx->positio;
    dum (ctx->positio < ctx->mensura &&
         _est_clavis_character((character)ctx->datum[ctx->positio])) {
        ctx->positio++;
    }

    si (ctx->positio > initium) {
        i32 longitudo = ctx->positio - initium;
        i32 k;
        fructus.datum = (i8*)piscina_allocare(ctx->piscina, longitudo);
        fructus.mensura = longitudo;
        per (k = 0; k < longitudo; k++) {
            fructus.datum[k] = ctx->datum[initium + k];
        }
    }

    redde fructus;
}

/* Legere chordam inter quotes */
hic_manens chorda
_legere_chorda_simplex(TomlContextus* ctx)
{
    chorda fructus;
    i32 initium;

    fructus.datum = NIHIL;
    fructus.mensura = 0;

    /* Expectare " */
    si (ctx->positio >= ctx->mensura || ctx->datum[ctx->positio] != '"') {
        redde fructus;
    }
    ctx->positio++;  /* Saltare " */

    initium = ctx->positio;
    dum (ctx->positio < ctx->mensura && ctx->datum[ctx->positio] != '"') {
        /* Tractare escape sequences */
        si (ctx->datum[ctx->positio] == '\\' && ctx->positio + 1 < ctx->mensura) {
            ctx->positio += 2;
        } alioquin {
            ctx->positio++;
        }
    }

    si (ctx->positio > initium) {
        i32 longitudo = ctx->positio - initium;
        i32 k;
        fructus.datum = (i8*)piscina_allocare(ctx->piscina, longitudo);
        fructus.mensura = longitudo;
        per (k = 0; k < longitudo; k++) {
            fructus.datum[k] = ctx->datum[initium + k];
        }
    }

    /* Saltare closing " */
    si (ctx->positio < ctx->mensura && ctx->datum[ctx->positio] == '"') {
        ctx->positio++;
    }

    redde fructus;
}

/* Legere numerum */
hic_manens s32
_legere_numerum(TomlContextus* ctx)
{
    s32 fructus = 0;
    s32 signum = 1;

    _saltare_spatia(ctx);

    /* Tractare signum negativum */
    si (ctx->positio < ctx->mensura && ctx->datum[ctx->positio] == '-') {
        signum = -1;
        ctx->positio++;
    }

    dum (ctx->positio < ctx->mensura &&
         _est_digitus((character)ctx->datum[ctx->positio])) {
        fructus = fructus * 10 + (ctx->datum[ctx->positio] - '0');
        ctx->positio++;
    }

    redde fructus * signum;
}

/* Verificare si incipit cum """ */
hic_manens b32
_est_triplex_quote(TomlContextus* ctx)
{
    redde ctx->positio + 2 < ctx->mensura &&
           ctx->datum[ctx->positio] == '"' &&
           ctx->datum[ctx->positio + 1] == '"' &&
           ctx->datum[ctx->positio + 2] == '"';
}

/* Addere introitum ad documentum */
hic_manens vacuum
_addere_introitum(TomlDocumentum* doc, chorda clavis, TomlValor valor)
{
    TomlIntroitus* introitus = (TomlIntroitus*)xar_addere(doc->introitus);
    introitus->clavis = clavis;
    introitus->valor = valor;
}

/* ==================================================
 * Parsatio Lineae
 * ================================================== */

hic_manens b32
_parsare_lineam(TomlContextus* ctx, TomlDocumentum* doc)
{
    _saltare_spatia(ctx);

    /* Linea vacua vel commentum */
    si (ctx->positio >= ctx->mensura ||
        ctx->datum[ctx->positio] == '\n' ||
        ctx->datum[ctx->positio] == '#') {
        _saltare_ad_finem_lineae(ctx);
        redde VERUM;
    }

    /* Status: Chorda multilinea */
    si (ctx->status == TOML_STATUS_CHORDA_MULTILINEA) {
        /* Quaerere closing """ */
        dum (ctx->positio < ctx->mensura) {
            si (_est_triplex_quote(ctx)) {
                /* Finis chordae multilineae */
                TomlValor valor;
                valor.genus = TOML_CHORDA;
                valor.datum.chorda_valor = chorda_aedificator_finire(ctx->aedificator);
                _addere_introitum(doc, ctx->clavis_currens, valor);
                ctx->positio += 3;
                ctx->status = TOML_STATUS_NORMALIS;
                _saltare_ad_finem_lineae(ctx);
                redde VERUM;
            }

            /* Addere character ad aedificator */
            chorda_aedificator_appendere_character(ctx->aedificator,
                (character)ctx->datum[ctx->positio]);
            ctx->positio++;
        }
        redde VERUM;
    }

    /* Status: Tabulatum */
    si (ctx->status == TOML_STATUS_TABULATUM) {
        dum (ctx->positio < ctx->mensura && ctx->datum[ctx->positio] != '\n') {
            _saltare_spatia(ctx);

            si (ctx->datum[ctx->positio] == ']') {
                /* Finis tabulati */
                TomlValor valor;
                valor.genus = TOML_TABULATUM;
                valor.datum.tabulatum_valor = ctx->tabulatum_currens;
                _addere_introitum(doc, ctx->clavis_currens, valor);
                ctx->positio++;
                ctx->status = TOML_STATUS_NORMALIS;
                _saltare_ad_finem_lineae(ctx);
                redde VERUM;
            }

            si (ctx->datum[ctx->positio] == '"') {
                /* Legere chorda in tabulato */
                chorda* elem = (chorda*)xar_addere(ctx->tabulatum_currens);
                *elem = _legere_chorda_simplex(ctx);
                _saltare_spatia(ctx);

                /* Saltare comma si praesens */
                si (ctx->positio < ctx->mensura && ctx->datum[ctx->positio] == ',') {
                    ctx->positio++;
                }
            } alioquin si (ctx->datum[ctx->positio] == '\n' ||
                          ctx->datum[ctx->positio] == '#') {
                /* Continuare ad proximam lineam */
                frange;
            } alioquin {
                ctx->positio++;
            }
        }
        redde VERUM;
    }

    /* Status: Normalis - legere clavis = valor */
    {
        chorda clavis = _legere_clavim(ctx);
        si (clavis.datum == NIHIL) {
            _saltare_ad_finem_lineae(ctx);
            redde VERUM;
        }

        _saltare_spatia(ctx);

        /* Expectare = */
        si (ctx->positio >= ctx->mensura || ctx->datum[ctx->positio] != '=') {
            doc->successus = FALSUM;
            doc->error = chorda_ex_literis("Expectabatur '='", ctx->piscina);
            doc->linea_erroris = ctx->linea;
            redde FALSUM;
        }
        ctx->positio++;

        _saltare_spatia(ctx);

        /* Determinare genus valoris */
        si (_est_triplex_quote(ctx)) {
            /* Chorda multilinea */
            ctx->positio += 3;
            ctx->clavis_currens = clavis;
            ctx->status = TOML_STATUS_CHORDA_MULTILINEA;
            ctx->aedificator = chorda_aedificator_creare(ctx->piscina, CCLVI);

            /* Saltare newline post """ si praesens */
            si (ctx->positio < ctx->mensura && ctx->datum[ctx->positio] == '\n') {
                ctx->positio++;
                ctx->linea++;
            }
        } alioquin si (ctx->datum[ctx->positio] == '"') {
            /* Chorda simplex */
            TomlValor valor;
            valor.genus = TOML_CHORDA;
            valor.datum.chorda_valor = _legere_chorda_simplex(ctx);
            _addere_introitum(doc, clavis, valor);
            _saltare_ad_finem_lineae(ctx);
        } alioquin si (ctx->datum[ctx->positio] == '[') {
            /* Tabulatum */
            ctx->positio++;
            ctx->clavis_currens = clavis;
            ctx->status = TOML_STATUS_TABULATUM;
            ctx->tabulatum_currens = xar_creare(ctx->piscina, magnitudo(chorda));
        } alioquin si (_est_digitus((character)ctx->datum[ctx->positio]) ||
                      ctx->datum[ctx->positio] == '-') {
            /* Numerus */
            TomlValor valor;
            valor.genus = TOML_NUMERUS;
            valor.datum.numerus_valor = _legere_numerum(ctx);
            _addere_introitum(doc, clavis, valor);
            _saltare_ad_finem_lineae(ctx);
        } alioquin {
            doc->successus = FALSUM;
            doc->error = chorda_ex_literis("Valor ignotus", ctx->piscina);
            doc->linea_erroris = ctx->linea;
            redde FALSUM;
        }
    }

    redde VERUM;
}

/* ==================================================
 * API Publica
 * ================================================== */

TomlDocumentum*
toml_legere(chorda input, Piscina* piscina)
{
    TomlDocumentum* doc;
    TomlContextus ctx;

    doc = (TomlDocumentum*)piscina_allocare(piscina, magnitudo(TomlDocumentum));
    doc->introitus = xar_creare(piscina, magnitudo(TomlIntroitus));
    doc->piscina = piscina;
    doc->successus = VERUM;
    doc->error.datum = NIHIL;
    doc->error.mensura = 0;
    doc->linea_erroris = 0;

    ctx.datum = input.datum;
    ctx.mensura = input.mensura;
    ctx.positio = 0;
    ctx.linea = 1;
    ctx.piscina = piscina;
    ctx.status = TOML_STATUS_NORMALIS;
    ctx.clavis_currens.datum = NIHIL;
    ctx.clavis_currens.mensura = 0;
    ctx.aedificator = NIHIL;
    ctx.tabulatum_currens = NIHIL;

    dum (ctx.positio < ctx.mensura && doc->successus) {
        si (!_parsare_lineam(&ctx, doc)) {
            frange;
        }

        /* Movere ad proximam lineam */
        si (ctx.positio < ctx.mensura && ctx.datum[ctx.positio] == '\n') {
            ctx.positio++;
            ctx.linea++;
        }
    }

    /* Verificare si status non-normalis ad finem */
    si (doc->successus && ctx.status != TOML_STATUS_NORMALIS) {
        doc->successus = FALSUM;
        si (ctx.status == TOML_STATUS_CHORDA_MULTILINEA) {
            doc->error = chorda_ex_literis("Chorda multilinea non clausa", piscina);
        } alioquin {
            doc->error = chorda_ex_literis("Tabulatum non clausum", piscina);
        }
        doc->linea_erroris = ctx.linea;
    }

    redde doc;
}

TomlDocumentum*
toml_legere_literis(constans character* input, Piscina* piscina)
{
    chorda ch;
    i32 len = 0;

    si (input == NIHIL) {
        ch.datum = NIHIL;
        ch.mensura = 0;
    } alioquin {
        i32 k;
        dum (input[len] != '\0') {
            len++;
        }
        /* Transcribere ad piscinam pro evitando const issues */
        ch.datum = (i8*)piscina_allocare(piscina, (i32)len);
        per (k = 0; k < len; k++) {
            ch.datum[k] = (i8)input[k];
        }
        ch.mensura = len;
    }

    redde toml_legere(ch, piscina);
}

TomlValor*
toml_capere(TomlDocumentum* doc, constans character* clavis)
{
    i32 i;
    i32 num;
    i32 len = 0;

    si (doc == NIHIL || clavis == NIHIL || !doc->successus) {
        redde NIHIL;
    }

    /* Computare longitudinem clavis */
    dum (clavis[len] != '\0') {
        len++;
    }

    num = xar_numerus(doc->introitus);
    per (i = 0; i < num; i++) {
        TomlIntroitus* intr = (TomlIntroitus*)xar_obtinere(doc->introitus, i);
        /* Comparare directe sine creatione chorda */
        si (intr->clavis.mensura == len) {
            b32 aequalis = VERUM;
            i32 j;
            per (j = 0; j < len; j++) {
                si ((character)intr->clavis.datum[j] != clavis[j]) {
                    aequalis = FALSUM;
                    frange;
                }
            }
            si (aequalis) {
                redde &intr->valor;
            }
        }
    }

    redde NIHIL;
}

chorda
toml_capere_chorda(TomlDocumentum* doc, constans character* clavis)
{
    TomlValor* val = toml_capere(doc, clavis);
    chorda vacua;

    vacua.datum = NIHIL;
    vacua.mensura = 0;

    si (val != NIHIL && val->genus == TOML_CHORDA) {
        redde val->datum.chorda_valor;
    }

    redde vacua;
}

s32
toml_capere_numerum(TomlDocumentum* doc, constans character* clavis)
{
    TomlValor* val = toml_capere(doc, clavis);

    si (val != NIHIL && val->genus == TOML_NUMERUS) {
        redde val->datum.numerus_valor;
    }

    redde 0;
}

Xar*
toml_capere_tabulatum(TomlDocumentum* doc, constans character* clavis)
{
    TomlValor* val = toml_capere(doc, clavis);

    si (val != NIHIL && val->genus == TOML_TABULATUM) {
        redde val->datum.tabulatum_valor;
    }

    redde NIHIL;
}

b32
toml_habet(TomlDocumentum* doc, constans character* clavis)
{
    redde toml_capere(doc, clavis) != NIHIL;
}

i32
toml_numerus_introituum(TomlDocumentum* doc)
{
    si (doc == NIHIL || doc->introitus == NIHIL) {
        redde 0;
    }
    redde xar_numerus(doc->introitus);
}

b32
toml_successus(TomlDocumentum* doc)
{
    redde doc != NIHIL && doc->successus;
}

chorda
toml_error(TomlDocumentum* doc)
{
    chorda vacua;
    vacua.datum = NIHIL;
    vacua.mensura = 0;

    si (doc == NIHIL) {
        redde vacua;
    }

    redde doc->error;
}
