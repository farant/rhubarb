/* exemplar.c - Implementatio Bibliothecae Exemplarium */

#include "exemplar.h"
#include <string.h>

/* ==================================================
 * Genera Statuum NFA
 * ================================================== */

nomen enumeratio {
    NFA_LITTERA,    /* Congruit character specifico */
    NFA_PUNCTUM,    /* Congruit quolibet charactere (.) */
    NFA_CLASSIS,    /* Congruit classi characterum */
    NFA_DIVISIO,    /* Divisio epsilon (pro *, +, ?) */
    NFA_FINIS       /* Status finalis acceptans */
} NFAGenus;


/* ==================================================
 * Structurae Internae
 * ================================================== */

/* Status singularis NFA */
nomen structura {
    s32 genus;          /* NFAGenus */
    s32 valor;          /* Character vel index classis */
    s32 transitus_1;    /* Proximus status (-1 = nullus) */
    s32 transitus_2;    /* Secundus transitus pro divisione (-1 = nullus) */
} NFAStatus;

/* Classis characterum (bitmap pro ASCII) */
nomen structura {
    i8  mappa[32];  /* 256 bits = 32 bytes */
    b32 negata;     /* Classis negata? */
} Classis;

/* Exemplar compilatum */
structura Exemplar {
    NFAStatus*  status_array;
    i32         status_numerus;
    Classis*    classes;
    i32         classes_numerus;
    b32         ancoratus_initio;   /* ^ ancora */
    b32         ancoratus_fine;     /* $ ancora */
    s32         status_initialis;   /* Index status initialis */
};

/* Contextus compilationis */
nomen structura {
    constans i8* datum;
    i32          mensura;
    i32          positio;
    NFAStatus*   status_array;
    i32          status_numerus;
    Classis*     classes;
    i32          classes_numerus;
    ExemplarStatus error;
    Piscina*     piscina;
} CompilatorContextus;

/* Set statuum pro simulatione NFA (bitmap) */
nomen structura {
    i8  mappa[32];  /* 256 bits = 32 bytes, sufficiens pro EXEMPLAR_STATUS_MAXIMUS */
} StatusSet;


/* ==================================================
 * Functiones Auxiliares: Classis Characterum
 * ================================================== */

hic_manens vacuum
_classis_initiare(Classis* c)
{
    memset(c->mappa, 0, 32);
    c->negata = FALSUM;
}

hic_manens vacuum
_classis_addere(Classis* c, i32 ch)
{
    si (ch >= 0 && ch < 256) {
        c->mappa[ch / 8] = (i8)(c->mappa[ch / 8] | (1 << (ch % 8)));
    }
}

hic_manens vacuum
_classis_addere_intervallum(Classis* c, i32 a, i32 b)
{
    i32 i;
    per (i = a; i <= b && i < 256; i++) {
        _classis_addere(c, i);
    }
}

hic_manens b32
_classis_continet(constans Classis* c, i32 ch)
{
    b32 in_mappa;
    si (ch < 0 || ch >= 256) {
        redde FALSUM;
    }
    in_mappa = (c->mappa[ch / 8] & (1 << (ch % 8))) != 0;
    redde c->negata ? !in_mappa : in_mappa;
}


/* ==================================================
 * Functiones Auxiliares: Status Set
 * ================================================== */

hic_manens vacuum
_set_purgare(StatusSet* s)
{
    memset(s->mappa, 0, 32);
}

hic_manens vacuum
_set_addere(StatusSet* s, s32 index)
{
    si (index >= 0 && index < 256) {
        s->mappa[index / 8] = (i8)(s->mappa[index / 8] | (1 << (index % 8)));
    }
}

hic_manens b32
_set_continet(constans StatusSet* s, s32 index)
{
    si (index < 0 || index >= 256) {
        redde FALSUM;
    }
    redde (s->mappa[index / 8] & (1 << (index % 8))) != 0;
}

hic_manens b32
_set_vacua(constans StatusSet* s)
{
    i32 i;
    per (i = 0; i < 32; i++) {
        si (s->mappa[i] != 0) {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Compilator: Functiones Auxiliares
 * ================================================== */

hic_manens s32
_addere_status(CompilatorContextus* ctx, s32 genus, s32 valor)
{
    s32 index;
    si (ctx->status_numerus >= EXEMPLAR_STATUS_MAXIMUS) {
        ctx->error = EXEMPLAR_ERROR_SYNTAXIS;
        redde (s32)-1;
    }
    index = (s32)ctx->status_numerus;
    ctx->status_array[index].genus = genus;
    ctx->status_array[index].valor = valor;
    ctx->status_array[index].transitus_1 = (s32)-1;
    ctx->status_array[index].transitus_2 = (s32)-1;
    ctx->status_numerus++;
    redde index;
}

hic_manens s32
_addere_classem(CompilatorContextus* ctx)
{
    s32 index;
    si (ctx->classes_numerus >= EXEMPLAR_CLASSES_MAXIMAE) {
        ctx->error = EXEMPLAR_ERROR_SYNTAXIS;
        redde (s32)-1;
    }
    index = (s32)ctx->classes_numerus;
    _classis_initiare(&ctx->classes[index]);
    ctx->classes_numerus++;
    redde index;
}

hic_manens character
_legere_character(CompilatorContextus* ctx)
{
    si (ctx->positio >= ctx->mensura) {
        redde '\0';
    }
    redde (character)ctx->datum[ctx->positio++];
}

hic_manens character
_spectare_character(CompilatorContextus* ctx)
{
    si (ctx->positio >= ctx->mensura) {
        redde '\0';
    }
    redde (character)ctx->datum[ctx->positio];
}

/* ==================================================
 * Compilator: Parsere Classem Characterum
 * ================================================== */

hic_manens s32
_parsere_classem(CompilatorContextus* ctx)
{
    s32 class_index;
    character c;
    character prev;
    b32 in_range;

    class_index = _addere_classem(ctx);
    si (class_index < 0) {
        redde (s32)-1;
    }

    /* Verificare negationem */
    c = _spectare_character(ctx);
    si (c == '^') {
        ctx->classes[class_index].negata = VERUM;
        _legere_character(ctx);
    }

    prev = '\0';
    in_range = FALSUM;

    dum (ctx->positio < ctx->mensura) {
        c = _legere_character(ctx);

        si (c == ']') {
            si (in_range) {
                /* Hyphen ad finem est literalis */
                _classis_addere(&ctx->classes[class_index], '-');
            }
            redde class_index;
        }

        si (c == '\\' && ctx->positio < ctx->mensura) {
            c = _legere_character(ctx);
        }

        si (c == '-' && prev != '\0' && _spectare_character(ctx) != ']') {
            in_range = VERUM;
            perge;
        }

        si (in_range) {
            si ((i32)(i8)c < (i32)(i8)prev) {
                ctx->error = EXEMPLAR_ERROR_ORDO_MALUS;
                redde (s32)-1;
            }
            _classis_addere_intervallum(&ctx->classes[class_index], (i32)(i8)prev, (i32)(i8)c);
            in_range = FALSUM;
            prev = '\0';
        } alioquin {
            si (prev != '\0') {
                _classis_addere(&ctx->classes[class_index], (i32)(i8)prev);
            }
            prev = c;
        }
    }

    ctx->error = EXEMPLAR_ERROR_CLASSIS_NON_CLAUSA;
    redde (s32)-1;
}


/* ==================================================
 * Compilator: Fragmentum NFA
 *
 * Fragmentum habet statum initialem et "dangling" transitum
 * qui debet connecti ad proximum fragmentum.
 * ================================================== */

nomen structura {
    s32 initium;        /* Status initialis fragmenti */
    s32* caudae;        /* Array transitionum dangling */
    s32  caudae_num;    /* Numerus caudarum */
} NFAFragmentum;

/* Array pro caudis (usus simplex) */
hic_manens s32 _caudae_buffer[256];
hic_manens s32 _caudae_index = 0;

hic_manens s32*
_allocare_caudas(s32 numerus)
{
    s32* result = &_caudae_buffer[_caudae_index];
    _caudae_index += numerus;
    redde result;
}

hic_manens vacuum
_reset_caudas(vacuum)
{
    _caudae_index = 0;
}

hic_manens vacuum
_connectere_caudas(CompilatorContextus* ctx, NFAFragmentum* frag, s32 target)
{
    s32 i;
    per (i = 0; i < frag->caudae_num; i++) {
        s32 status_idx = frag->caudae[i] / 2;
        s32 trans_idx = frag->caudae[i] % 2;
        si (trans_idx == 0) {
            ctx->status_array[status_idx].transitus_1 = target;
        } alioquin {
            ctx->status_array[status_idx].transitus_2 = target;
        }
    }
}


/* ==================================================
 * Compilator: Construere NFA
 *
 * Usus: Stack-based algorithm pro Thompson construction
 * ================================================== */

hic_manens NFAFragmentum
_compilare_atomum(CompilatorContextus* ctx)
{
    NFAFragmentum frag;
    character c;
    s32 s;

    frag.initium = (s32)-1;
    frag.caudae = NIHIL;
    frag.caudae_num = 0;

    c = _legere_character(ctx);

    si (c == '\\') {
        /* Character effugitus */
        si (ctx->positio >= ctx->mensura) {
            ctx->error = EXEMPLAR_ERROR_EFFUGIUM_FINIS;
            redde frag;
        }
        c = _legere_character(ctx);
        s = _addere_status(ctx, (s32)NFA_LITTERA, (s32)(i8)c);
        si (s < 0) redde frag;
        frag.initium = s;
        frag.caudae = _allocare_caudas(1);
        frag.caudae[0] = s * 2;  /* transitus_1 */
        frag.caudae_num = 1;
    }
    alioquin si (c == '.') {
        /* Quilibet character */
        s = _addere_status(ctx, (s32)NFA_PUNCTUM, 0);
        si (s < 0) redde frag;
        frag.initium = s;
        frag.caudae = _allocare_caudas(1);
        frag.caudae[0] = s * 2;
        frag.caudae_num = 1;
    }
    alioquin si (c == '[') {
        /* Classis characterum */
        s32 class_idx = _parsere_classem(ctx);
        si (class_idx < 0) redde frag;
        s = _addere_status(ctx, (s32)NFA_CLASSIS, class_idx);
        si (s < 0) redde frag;
        frag.initium = s;
        frag.caudae = _allocare_caudas(1);
        frag.caudae[0] = s * 2;
        frag.caudae_num = 1;
    }
    alioquin {
        /* Character literalis */
        s = _addere_status(ctx, (s32)NFA_LITTERA, (s32)(i8)c);
        si (s < 0) redde frag;
        frag.initium = s;
        frag.caudae = _allocare_caudas(1);
        frag.caudae[0] = s * 2;
        frag.caudae_num = 1;
    }

    redde frag;
}

hic_manens NFAFragmentum
_applicare_quantificatorem(CompilatorContextus* ctx, NFAFragmentum frag, character quant)
{
    s32 split;
    NFAFragmentum novum;

    si (frag.initium < 0) {
        redde frag;
    }

    si (quant == '*') {
        /* Zero vel plus: split -> (frag -> split) | exit */
        split = _addere_status(ctx, (s32)NFA_DIVISIO, 0);
        si (split < 0) redde frag;
        ctx->status_array[split].transitus_1 = frag.initium;
        _connectere_caudas(ctx, &frag, split);

        novum.initium = split;
        novum.caudae = _allocare_caudas(1);
        novum.caudae[0] = split * 2 + 1;  /* transitus_2 */
        novum.caudae_num = 1;
        redde novum;
    }
    alioquin si (quant == '+') {
        /* Unus vel plus: frag -> split -> (frag | exit) */
        split = _addere_status(ctx, (s32)NFA_DIVISIO, 0);
        si (split < 0) redde frag;
        ctx->status_array[split].transitus_1 = frag.initium;
        _connectere_caudas(ctx, &frag, split);

        novum.initium = frag.initium;
        novum.caudae = _allocare_caudas(1);
        novum.caudae[0] = split * 2 + 1;
        novum.caudae_num = 1;
        redde novum;
    }
    alioquin si (quant == '?') {
        /* Zero vel unus: split -> (frag | exit) */
        split = _addere_status(ctx, (s32)NFA_DIVISIO, 0);
        si (split < 0) redde frag;
        ctx->status_array[split].transitus_1 = frag.initium;

        novum.initium = split;
        novum.caudae = _allocare_caudas(1 + frag.caudae_num);
        novum.caudae[0] = split * 2 + 1;
        memcpy(&novum.caudae[1], frag.caudae, (size_t)frag.caudae_num * sizeof(s32));
        novum.caudae_num = 1 + frag.caudae_num;
        redde novum;
    }

    redde frag;
}


/* ==================================================
 * Compilator: Principale
 * ================================================== */

Exemplar*
exemplar_compilare(
    chorda          exemplar_textus,
    ExemplarStatus* status,
    Piscina*        piscina)
{
    CompilatorContextus ctx;
    Exemplar* exemplar;
    NFAFragmentum stack[64];
    s32 stack_depth;
    character c;
    s32 finis;

    si (piscina == NIHIL) {
        si (status) *status = EXEMPLAR_ERROR_SYNTAXIS;
        redde NIHIL;
    }

    /* Initiare contextum */
    ctx.datum = exemplar_textus.datum;
    ctx.mensura = exemplar_textus.mensura;
    ctx.positio = 0;
    ctx.status_array = (NFAStatus*)piscina_allocare(piscina,
        (memoriae_index)(EXEMPLAR_STATUS_MAXIMUS * (i32)sizeof(NFAStatus)));
    ctx.status_numerus = 0;
    ctx.classes = (Classis*)piscina_allocare(piscina,
        (memoriae_index)(EXEMPLAR_CLASSES_MAXIMAE * (i32)sizeof(Classis)));
    ctx.classes_numerus = 0;
    ctx.error = EXEMPLAR_OK;
    ctx.piscina = piscina;

    _reset_caudas();

    /* Allocare exemplar */
    exemplar = (Exemplar*)piscina_allocare(piscina, sizeof(Exemplar));
    exemplar->ancoratus_initio = FALSUM;
    exemplar->ancoratus_fine = FALSUM;

    /* Verificare ancora initii */
    si (ctx.positio < ctx.mensura && (character)ctx.datum[ctx.positio] == '^') {
        exemplar->ancoratus_initio = VERUM;
        ctx.positio++;
    }

    /* Parsere exemplar */
    stack_depth = 0;

    dum (ctx.positio < ctx.mensura && ctx.error == EXEMPLAR_OK) {
        c = _spectare_character(&ctx);

        /* Verificare ancora finis */
        si (c == '$' && ctx.positio == ctx.mensura - 1) {
            exemplar->ancoratus_fine = VERUM;
            ctx.positio++;
            frange;
        }

        /* Quantificatores non possunt esse primi */
        si (c == '*' || c == '+' || c == '?') {
            ctx.error = EXEMPLAR_ERROR_SYNTAXIS;
            frange;
        }

        /* Parsere atomum */
        si (stack_depth >= 64) {
            ctx.error = EXEMPLAR_ERROR_SYNTAXIS;
            frange;
        }
        stack[stack_depth] = _compilare_atomum(&ctx);
        si (stack[stack_depth].initium < 0) {
            frange;
        }

        /* Verificare quantificatorem POST atomum */
        c = _spectare_character(&ctx);
        si (c == '*' || c == '+' || c == '?') {
            ctx.positio++;
            stack[stack_depth] = _applicare_quantificatorem(&ctx, stack[stack_depth], c);
        }

        stack_depth++;

        /* Concatenare cum praecedenti si existit */
        dum (stack_depth >= 2) {
            /* Connectere fragmenta */
            _connectere_caudas(&ctx, &stack[stack_depth - 2], stack[stack_depth - 1].initium);
            stack[stack_depth - 2].caudae = stack[stack_depth - 1].caudae;
            stack[stack_depth - 2].caudae_num = stack[stack_depth - 1].caudae_num;
            stack_depth--;
        }
    }

    si (ctx.error != EXEMPLAR_OK) {
        si (status) *status = ctx.error;
        redde NIHIL;
    }

    /* Addere status finalis */
    finis = _addere_status(&ctx, (s32)NFA_FINIS, 0);
    si (stack_depth > 0) {
        _connectere_caudas(&ctx, &stack[0], finis);
        exemplar->status_initialis = stack[0].initium;
    } alioquin {
        /* Exemplar vacuum - congruere nihil */
        exemplar->status_initialis = finis;
    }

    exemplar->status_array = ctx.status_array;
    exemplar->status_numerus = ctx.status_numerus;
    exemplar->classes = ctx.classes;
    exemplar->classes_numerus = ctx.classes_numerus;

    si (status) *status = EXEMPLAR_OK;
    redde exemplar;
}


/* ==================================================
 * Simulatio NFA: Epsilon Clausura
 * ================================================== */

hic_manens vacuum
_epsilon_clausura(constans Exemplar* ex, StatusSet* set)
{
    s32 mutatum;
    s32 i;

    /* Iterare usque ad punctum fixum */
    fac {
        mutatum = FALSUM;
        per (i = 0; i < (s32)ex->status_numerus; i++) {
            si (_set_continet(set, i)) {
                constans NFAStatus* s = &ex->status_array[i];
                si (s->genus == (s32)NFA_DIVISIO) {
                    si (s->transitus_1 >= 0 && !_set_continet(set, s->transitus_1)) {
                        _set_addere(set, s->transitus_1);
                        mutatum = VERUM;
                    }
                    si (s->transitus_2 >= 0 && !_set_continet(set, s->transitus_2)) {
                        _set_addere(set, s->transitus_2);
                        mutatum = VERUM;
                    }
                }
            }
        }
    } dum (mutatum);
}

hic_manens b32
_set_continet_finem(constans Exemplar* ex, constans StatusSet* set)
{
    s32 i;
    per (i = 0; i < (s32)ex->status_numerus; i++) {
        si (_set_continet(set, i) && ex->status_array[i].genus == (s32)NFA_FINIS) {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Simulatio NFA: Transire
 * ================================================== */

hic_manens vacuum
_transire(constans Exemplar* ex, constans StatusSet* nunc, StatusSet* proximus, character c)
{
    s32 i;

    _set_purgare(proximus);

    per (i = 0; i < (s32)ex->status_numerus; i++) {
        si (_set_continet(nunc, i)) {
            constans NFAStatus* s = &ex->status_array[i];

            si (s->genus == (s32)NFA_LITTERA) {
                si ((character)s->valor == c && s->transitus_1 >= 0) {
                    _set_addere(proximus, s->transitus_1);
                }
            }
            alioquin si (s->genus == (s32)NFA_PUNCTUM) {
                /* . non congruit cum newline in modo default */
                si (c != '\n' && s->transitus_1 >= 0) {
                    _set_addere(proximus, s->transitus_1);
                }
            }
            alioquin si (s->genus == (s32)NFA_CLASSIS) {
                si (_classis_continet(&ex->classes[s->valor], (i32)(i8)c) && s->transitus_1 >= 0) {
                    _set_addere(proximus, s->transitus_1);
                }
            }
        }
    }
}


/* ==================================================
 * Functiones Publicae: Congruere
 * ================================================== */

b32
exemplar_congruit_totum(
    constans Exemplar* exemplar,
    chorda             textus)
{
    StatusSet nunc;
    StatusSet proximus;
    StatusSet* temp;
    StatusSet* p_nunc;
    StatusSet* p_proximus;
    i32 i;

    si (exemplar == NIHIL) {
        redde FALSUM;
    }

    _set_purgare(&nunc);
    _set_purgare(&proximus);

    _set_addere(&nunc, exemplar->status_initialis);
    _epsilon_clausura(exemplar, &nunc);

    p_nunc = &nunc;
    p_proximus = &proximus;

    per (i = 0; i < textus.mensura; i++) {
        _transire(exemplar, p_nunc, p_proximus, (character)textus.datum[i]);
        _epsilon_clausura(exemplar, p_proximus);

        /* Swap */
        temp = p_nunc;
        p_nunc = p_proximus;
        p_proximus = temp;
    }

    redde _set_continet_finem(exemplar, p_nunc);
}


ExemplarFructus
exemplar_invenire(
    constans Exemplar* exemplar,
    chorda             textus)
{
    ExemplarFructus fructus;
    StatusSet nunc;
    StatusSet proximus;
    StatusSet* temp;
    StatusSet* p_nunc;
    StatusSet* p_proximus;
    i32 initium_pos;
    i32 i;
    i32 finis_max;

    fructus.congruit = FALSUM;
    fructus.initium = 0;
    fructus.longitudo = 0;

    si (exemplar == NIHIL) {
        redde fructus;
    }

    finis_max = exemplar->ancoratus_initio ? 1 : textus.mensura;

    per (initium_pos = 0; initium_pos < finis_max || (finis_max == 0 && initium_pos == 0); initium_pos++) {
        _set_purgare(&nunc);
        _set_purgare(&proximus);

        _set_addere(&nunc, exemplar->status_initialis);
        _epsilon_clausura(exemplar, &nunc);

        /* Verificare congruentiam vacuam */
        si (_set_continet_finem(exemplar, &nunc)) {
            fructus.congruit = VERUM;
            fructus.initium = initium_pos;
            fructus.longitudo = 0;
            /* Perge ad invenire congruentiam longiorem */
        }

        p_nunc = &nunc;
        p_proximus = &proximus;

        per (i = initium_pos; i < textus.mensura; i++) {
            _transire(exemplar, p_nunc, p_proximus, (character)textus.datum[i]);
            _epsilon_clausura(exemplar, p_proximus);

            si (_set_vacua(p_proximus)) {
                frange;
            }

            si (_set_continet_finem(exemplar, p_proximus)) {
                /* Verificare ancora finis */
                si (!exemplar->ancoratus_fine || i == textus.mensura - 1) {
                    fructus.congruit = VERUM;
                    fructus.initium = initium_pos;
                    fructus.longitudo = i - initium_pos + 1;
                    /* Perge ad invenire congruentiam longiorem (greedy) */
                }
            }

            /* Swap */
            temp = p_nunc;
            p_nunc = p_proximus;
            p_proximus = temp;
        }

        si (fructus.congruit) {
            redde fructus;
        }
    }

    redde fructus;
}


i32
exemplar_invenire_omnes(
    constans Exemplar*  exemplar,
    chorda              textus,
    ExemplarFructus**   fructus_out,
    Piscina*            piscina)
{
    ExemplarFructus* fructus_array;
    i32 numerus;
    i32 capacitas;
    i32 positio;
    ExemplarFructus f;
    chorda sub;

    si (exemplar == NIHIL || fructus_out == NIHIL || piscina == NIHIL) {
        redde 0;
    }

    capacitas = 16;
    fructus_array = (ExemplarFructus*)piscina_allocare(piscina,
        (memoriae_index)(capacitas * (i32)sizeof(ExemplarFructus)));
    numerus = 0;
    positio = 0;

    dum (positio < textus.mensura) {
        sub.datum = textus.datum + positio;
        sub.mensura = textus.mensura - positio;

        f = exemplar_invenire(exemplar, sub);

        si (!f.congruit) {
            frange;
        }

        /* Addere ad array */
        si (numerus >= capacitas) {
            /* Reallocare (simpliciter allocare novum) */
            ExemplarFructus* novum = (ExemplarFructus*)piscina_allocare(piscina,
                (memoriae_index)(capacitas * 2 * (i32)sizeof(ExemplarFructus)));
            memcpy(novum, fructus_array, (size_t)numerus * sizeof(ExemplarFructus));
            fructus_array = novum;
            capacitas *= 2;
        }

        fructus_array[numerus].congruit = VERUM;
        fructus_array[numerus].initium = positio + f.initium;
        fructus_array[numerus].longitudo = f.longitudo;
        numerus++;

        /* Avanzare positio */
        positio += f.initium + (f.longitudo > 0 ? f.longitudo : 1);
    }

    *fructus_out = fructus_array;
    redde numerus;
}


/* ==================================================
 * Functiones Commodae
 * ================================================== */

b32
exemplar_congruit_literis(
    constans character* exemplar_cstr,
    chorda              textus,
    Piscina*            piscina)
{
    Exemplar* ex;
    ExemplarFructus f;

    si (exemplar_cstr == NIHIL || piscina == NIHIL) {
        redde FALSUM;
    }

    ex = exemplar_compilare(chorda_ex_literis(exemplar_cstr, piscina), NIHIL, piscina);
    si (ex == NIHIL) {
        redde FALSUM;
    }

    f = exemplar_invenire(ex, textus);
    redde f.congruit;
}


constans character*
exemplar_status_descriptio(
    ExemplarStatus status)
{
    commutatio (status) {
        casus EXEMPLAR_OK:
            redde "OK";
        casus EXEMPLAR_ERROR_SYNTAXIS:
            redde "Syntaxis mala";
        casus EXEMPLAR_ERROR_CLASSIS_NON_CLAUSA:
            redde "Classis non clausa ([ sine ])";
        casus EXEMPLAR_ERROR_EFFUGIUM_FINIS:
            redde "Effugium ad finem (\\ sine charactere)";
        casus EXEMPLAR_ERROR_CLASSIS_VACUA:
            redde "Classis vacua ([])";
        casus EXEMPLAR_ERROR_ORDO_MALUS:
            redde "Ordo malus in intervallo ([z-a])";
        ordinarius:
            redde "Error ignotus";
    }
}
