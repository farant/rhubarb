/* renominare.c - renominatio identificatorum (actio prima
 * transformationis; parcum 01KYWYQMJ7, INTENTIO 2026-07-31)
 *
 * Recipe examinis FIDELE per plagulam: lexicon compositum (ISO +
 * POSIX totum + bloci externa plagulae), capita repositorii
 * praebita, oraculum praeseminatum, BIS-analysis post recanonicare.
 *
 * LEX RATIONARII (invarians nucleus): omne lexema identificatoris
 * strati 0 quod vetus scribit AUT explicatum est (sedes scopi
 * plagulae / symbolum alienum / membrum / titulus salti / typus
 * nominatus) AUT manuale nominatur (expansio macronis, corpus
 * macronis, ramus omissus) AUT REFUSIO fit. Debilitas analyseos
 * numquam sedem tacite praeterit - refusio clara pro fractura muta.
 *
 * Ambitus v0: symbola scopi plagulae (functio/variabile/typedef/
 * constans; static + extern). Entitas: registrationes externae
 * omnium plagularum = entitas una; staticum homonymum -> AMBIGUUM,
 * refusio nisi -via plagulam definientem nominat (tunc sedes
 * alibi ligatae AUTOMATICE excluduntur - quod grep non potest).
 *
 * Usus:   renominare <vetus> <novum> [-scribere] [-machina] [-v]
 *                    [-via <plagula>] [-intra <functio> [-linea n]]
 *                    (-lista | plagulae...)
 * Exitus: 0 planum/scriptum | 1 refusio (nihil scriptum) |
 *         2 apparatus fractus | 3 RECUSO (fines tactae) |
 *         4 scriptum sed verificatio fracta (git checkout!)
 */

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_expandere.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include "silva_lexicon.h"

#include <stdio.h>
#include <string.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

/* ==================================================
 * status globalis
 * ================================================== */

hic_manens b32 machina = FALSUM;
hic_manens b32 verbosa = FALSUM;
hic_manens b32 scribere_modus = FALSUM;
hic_manens b32 recuso_flag = FALSUM;   /* fines tactae in analysi */

hic_manens constans character* vetus_l = NIHIL;
hic_manens constans character* novum_l = NIHIL;
hic_manens constans character* via_data = NIHIL;   /* -via */

/* modus localis (-intra): scopus = functio una; identitas =
 * REGISTRATIO (monstrator symboli - locales TU numquam
 * transgrediuntur, ergo aequalitas monstratorum exacta est) */
hic_manens constans character* intra_l = NIHIL;    /* -intra */
hic_manens i32 linea_data = ZEPHYRUM;              /* -linea */

/* modus membri (-membrum <typus>): vetus = titulus membri; typus
 * per TU nomine resolvitur (typedef primum - domus tags anonyma
 * titulis typedef baptizat - deinde tag scopi plagulae); intra TU
 * aequalitas monstratorum tag. Spatium nominum membrorum: symbola
 * omnia ALIENA sunt. */
hic_manens constans character* membrum_l = NIHIL;  /* -membrum */

hic_manens Piscina* piscina_magistra = NIHIL;

/* capita repositorii SEMEL lecta (textus in piscina magistra) */
nomen structura {
    constans character* via;
    character*          textus;
    i32                 mensura;
} CaputLectum;
hic_manens Xar* capita_lecta = NIHIL;

/* systema ISO + POSIX semel lectum (compositio per plagulam) */
hic_manens character* fons_iso = NIHIL;
hic_manens i32 mensura_iso = ZEPHYRUM;
hic_manens character* fons_px = NIHIL;
hic_manens i32 mensura_px = ZEPHYRUM;

/* analysis plagulae unius (piscina propria VIVIT ad finem) */
nomen structura {
    constans character* via;
    character*          fons;
    i32                 mensura;
    Piscina*            piscina;
    SilvaParsura*       parsura;
    SilvaSemantica*     sem;
    TypusC89*           tag_electus;   /* modus membri: typus
                                        * target huius TU; NIHIL =
                                        * TU typum nescit */
    i32                 errores;       /* numerus_errorum copia -
                                        * superstes destructioni
                                        * piscinae (fluxus modorum
                                        * membri/localis) */
} AnalysisPlagulae;
hic_manens Xar* analyses = NIHIL;    /* AnalysisPlagulae */

/* classes sedium */
#define CLASSIS_SEDES      0   /* registratio entitatis (splicenda) */
#define CLASSIS_USUS       1   /* usus ligatus entitati (splicenda) */
#define CLASSIS_ALIENA     2   /* symbolum aliud / membrum / titulus */
#define CLASSIS_MANUALIS   3   /* expansio / corpus macronis / ramus */
#define CLASSIS_REFUSIO    4   /* inexplicata - refusio */

nomen structura {
    s32                 classis;
    constans character* via;      /* copia in piscina magistra */
    i32                 linea;
    i32                 columna;
    s32                 offset;   /* -1 si non splicenda */
    constans character* nota;     /* causa/contextus; NIHIL licet */
} SedesInventa;
hic_manens Xar* sedes_omnes = NIHIL;      /* SedesInventa */
hic_manens Xar* culpae = NIHIL;           /* constans character* */

/* entitas: staticum-ne? (post resolutionem) */
hic_manens b32 entitas_statica = FALSUM;
hic_manens constans character* entitas_via_statica = NIHIL;
hic_manens s32 entitas_genus = -I;        /* SemanticaSymbolumGenus */

/* entitas localis (post _localem_resolvere) */
hic_manens constans SemanticaSymbolum* symbolum_localis = NIHIL;
hic_manens constans AnalysisPlagulae* analysis_localis = NIHIL;

/* dedup sedium trans TU: clavis "via|offset" */
hic_manens TabulaDispersa* sedes_visae = NIHIL;

/* v0.1: testimonium macronum (01KYX2DSKK) - corpora per lexemata
 * VERA (SilvaMacroDef.corpus), promotio probabilis: si OMNES
 * expansiones observatae lexematum vetus-scribentium ex corpore M
 * ad entitatem ligant, sedes corporis SPLICENDAE sunt. Clavis =
 * titulus macronis (definitiones homonymae plures = demotio). */
nomen structura {
    constans character* titulus;
    constans character* via_def;    /* definitionis primae */
    i32                 linea_def;
    i32                 defs;           /* definitiones distinctae */
    b32                 param_homonymum;
    i32                 ligata_entitati;
    i32                 ligata_aliis;
    i32                 non_ligata;
    Xar*                sedes_corporis;   /* SedesInventa */
    Xar*                invocationes;     /* SedesInventa (manualis) */
} MacroTestimonium;
hic_manens Xar* macro_testimonia = NIHIL;        /* MacroTestimonium */
hic_manens TabulaDispersa* macro_index = NIHIL;  /* titulus -> idx+1 */

/* ==================================================
 * auxilia parva
 * ================================================== */

interior chorda
_ch (constans character* litterae)
{
    redde chorda_ex_literis(litterae, piscina_magistra);
}

interior b32
_chordae_pares_lit (chorda a, constans character* b)
{
    memoriae_index m = strlen(b);

    si ((memoriae_index)a.mensura != m)
    {
        redde FALSUM;
    }
    redde memcmp(a.datum, b, m) == ZEPHYRUM ? VERUM : FALSUM;
}

interior constans character*
_litterae (chorda c)
{
    character* l = (character*)piscina_allocare(piscina_magistra,
        (memoriae_index)c.mensura + I);

    si (l == NIHIL)
    {
        redde "";
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(l, c.datum, (memoriae_index)c.mensura);
    }
    l[c.mensura] = '\0';
    redde l;
}

hic_manens TabulaDispersa* culpae_visae = NIHIL;

interior vacuum
_culpam_addere (constans character* culpa)
{
    constans character** cella;

    /* dedup litteralis: registratio capitis a TU quoque visa
     * culpam unam parit, non unam per includentem */
    si (culpae_visae != NIHIL)
    {
        chorda clavis = _ch(culpa);

        si (tabula_dispersa_continet(culpae_visae, clavis))
        {
            redde;
        }
        (vacuum)tabula_dispersa_inserere(culpae_visae, clavis,
            NIHIL);
    }
    cella = (constans character**)xar_addere(culpae);
    si (cella != NIHIL)
    {
        *cella = culpa;
    }
}

interior constans character*
_culpa_formata (constans character* praefixum, constans character* via,
    i32 linea, i32 columna)
{
    character buffer[DXII];
    int scripti = sprintf(buffer, "%s (%s:%d:%d)", praefixum,
        via != NIHIL ? via : "?", (int)linea, (int)columna);

    si (scripti <= ZEPHYRUM)
    {
        redde praefixum;
    }
    redde _litterae(_ch(buffer));
}

interior character*
_plagulam_legere (Piscina* piscina, constans character* via,
    i32* mensura_out)
{
    FILE* pl = fopen(via, "rb");
    long mensura_l;
    character* textus;

    *mensura_out = ZEPHYRUM;
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_l > 0L ? mensura_l + 1L : I));
    si (textus == NIHIL
        || (mensura_l > 0L
            && fread(textus, I, (memoriae_index)mensura_l, pl)
                != (memoriae_index)mensura_l))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    textus[mensura_l] = '\0';
    *mensura_out = (i32)mensura_l;
    redde textus;
}

/* ==================================================
 * validatio nominis novi
 * ================================================== */

interior b32
_littera_identificatoris (character c, b32 prima)
{
    si ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || c == '_')
    {
        redde VERUM;
    }
    si (!prima && c >= '0' && c <= '9')
    {
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
_verbum_c89 (constans character* v)
{
    hic_manens constans character* verba[] = {
        "auto", "break", "case", "char", "const", "continue",
        "default", "do", "double", "else", "enum", "extern",
        "float", "for", "goto", "if", "int", "long", "register",
        "return", "short", "signed", "sizeof", "static", "struct",
        "switch", "typedef", "union", "unsigned", "void",
        "volatile", "while", NIHIL
    };
    i32 k;

    per (k = ZEPHYRUM; verba[k] != NIHIL; k++)
    {
        si (strcmp(v, verba[k]) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * capita repositorii (semel lecta, per plagulam praebita)
 * ================================================== */

interior b32
_praetermittendum (constans character* titulus)
{
    redde strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM;
}

interior vacuum
_capita_colligere (constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL)
    {
        redde;
    }
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        character via_plena[1024];
        memoriae_index m;

        si (introitus->d_name[ZEPHYRUM] == '.')
        {
            perge;
        }
        si (_praetermittendum(introitus->d_name))
        {
            perge;
        }
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);
        si (introitus->d_type == DT_DIR)
        {
            _capita_colligere(via_plena);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (m >= III && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I] == 'h')
            {
                CaputLectum* caput;
                character* textus;
                i32 mensura;

                textus = _plagulam_legere(piscina_magistra,
                    via_plena, &mensura);
                si (textus == NIHIL)
                {
                    perge;
                }
                caput = (CaputLectum*)xar_addere(capita_lecta);
                si (caput == NIHIL)
                {
                    perge;
                }
                caput->via = _litterae(_ch(via_plena));
                caput->textus = textus;
                caput->mensura = mensura;
            }
        }
    }
    closedir(dir);
}

/* ==================================================
 * analysis plagulae unius (recipe examinis)
 * ================================================== */

interior AnalysisPlagulae*
_plagulam_analysare (constans character* via)
{
    AnalysisPlagulae* an;
    Piscina* pn;
    SilvaContextus* ctx;
    character* fons;
    i32 mensura;
    character* fons_sys;
    i32 mensura_sys = ZEPHYRUM;
    b32 ext_fractum = FALSUM;
    SilvaParsura* systema_parsura;
    SilvaSemantica* systema_semantica;
    SilvaOraculum* oraculum;
    SilvaParsura* parsura;
    SilvaSemantica* sem;
    i32 k;

    pn = piscina_generare_dynamicum("renominare_plagula", 268435456);
    si (pn == NIHIL)
    {
        redde NIHIL;
    }
    fons = _plagulam_legere(pn, via, &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "renominare: plagula illegibilis: %s\n",
            via);
        piscina_destruere(pn);
        redde NIHIL;
    }
    fons_sys = silva_lexicon_componere(fons_iso, mensura_iso,
        fons_px, mensura_px, fons, mensura, VERUM, pn,
        &mensura_sys, via, &ext_fractum);
    si (ext_fractum || fons_sys == NIHIL)
    {
        fprintf(stderr, "renominare: compositio lexici fracta:"
            " %s\n", via);
        piscina_destruere(pn);
        redde NIHIL;
    }
    ctx = silva_contextus_creare(pn);
    si (ctx == NIHIL
        || !silva_contextus_lexicon_addere(ctx, "systema_c89.h",
               fons_sys, mensura_sys))
    {
        piscina_destruere(pn);
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(capita_lecta); k++)
    {
        CaputLectum* caput = (CaputLectum*)xar_obtinere(
            capita_lecta, k);

        (vacuum)silva_contextus_praebere(ctx, caput->via,
            caput->textus, caput->mensura);
    }
    systema_parsura = silva_c89_parsare(pn, "systema_c89.h",
        fons_sys, mensura_sys, NIHIL);
    si (systema_parsura == NIHIL
        || systema_parsura->numerus_errorum > ZEPHYRUM)
    {
        fprintf(stderr, "renominare: systema non parsatum (%s)\n",
            via);
        piscina_destruere(pn);
        redde NIHIL;
    }
    systema_semantica = silva_c89_semantica_analysare(pn,
        systema_parsura);
    si (systema_semantica == NIHIL)
    {
        piscina_destruere(pn);
        redde NIHIL;
    }
    oraculum = silva_oraculum_creare(pn);
    si (oraculum != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(
            systema_semantica, oraculum);
    }
    parsura = silva_c89_parsare_cum_contextu(pn, ctx, via, fons,
        mensura, oraculum);
    si (parsura == NIHIL || !parsura->successus
        || parsura->commissio == NIHIL)
    {
        fprintf(stderr, "renominare: apparatus fractus: %s\n", via);
        piscina_destruere(pn);
        redde NIHIL;
    }
    si (parsura->fines_tactae)
    {
        imprimere("VERDICTUM\tRECUSO\t%s\n", via);
        recuso_flag = VERUM;
        piscina_destruere(pn);
        redde NIHIL;
    }
    sem = silva_c89_semantica_analysare_cum_systemate(pn, parsura,
        systema_parsura);
    si (sem != NIHIL && oraculum != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(sem, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        si (silva_recanonicare(parsura->commissio, oraculum,
                silva_c89_resolutor, NIHIL) > ZEPHYRUM)
        {
            sem = silva_c89_semantica_analysare_cum_systemate(pn,
                parsura, systema_parsura);
        }
    }
    si (sem == NIHIL)
    {
        fprintf(stderr, "renominare: semantica fracta: %s\n", via);
        piscina_destruere(pn);
        redde NIHIL;
    }
    an = (AnalysisPlagulae*)xar_addere(analyses);
    si (an == NIHIL)
    {
        piscina_destruere(pn);
        redde NIHIL;
    }
    an->via = via;
    an->fons = fons;
    an->mensura = mensura;
    an->piscina = pn;
    an->parsura = parsura;
    an->sem = sem;
    an->tag_electus = NIHIL;
    an->errores = (i32)parsura->numerus_errorum;
    redde an;
}

/* ==================================================
 * classificatio: tabula lexema -> classis (per plagulam)
 * ================================================== */

/* clavis tabulae = octeti monstratoris lexematis */
interior chorda
_clavis_lexematis (SilvaToken* tok)
{
    chorda c;
    character* l = (character*)piscina_allocare(piscina_magistra,
        magnitudo(SilvaToken*));

    memcpy(l, &tok, magnitudo(SilvaToken*));
    c.datum = (i8*)l;
    c.mensura = (i32)magnitudo(SilvaToken*);
    redde c;
}

interior vacuum
_classem_ponere (TabulaDispersa* classes, SilvaToken* tok,
    s32 classis)
{
    /* prima positio vincit (sedes iam explicata non degradatur) */
    chorda clavis = _clavis_lexematis(tok);
    vacuum* prior;

    si (tabula_dispersa_invenire(classes, clavis, &prior))
    {
        redde;
    }
    (vacuum)tabula_dispersa_inserere(classes, clavis,
        (vacuum*)(memoriae_index)(classis + I));
}

interior b32
_classem_capere (TabulaDispersa* classes, SilvaToken* tok,
    s32* classis_out)
{
    chorda clavis = _clavis_lexematis(tok);
    vacuum* valor;

    si (!tabula_dispersa_invenire(classes, clavis, &valor))
    {
        redde FALSUM;
    }
    *classis_out = (s32)((memoriae_index)valor - I);
    redde VERUM;
}

interior SilvaToken*
_tok_valoris (SilvaValor v)
{
    si (v.genus != SILVA_VALOR_TOKEN)
    {
        redde NIHIL;
    }
    redde v.datum.token;
}

interior b32
_tok_vetus_scribit (SilvaToken* tok)
{
    si (tok == NIHIL || tok->genus != SILVA_LEX_IDENTIFICATOR)
    {
        redde FALSUM;
    }
    redde _chordae_pares_lit(tok->valor, vetus_l);
}

/* basis accessus ad typum tag (recipe custodis chorda.datum in
 * semantica.c, per superficies publicas): typus naturalis basis
 * (canonicum-conscius), SAGITTA monstratorem unum exuit, qualia
 * exuuntur, STRUCTURA/UNIO redditur; NIHIL = basis sine typo aut
 * non-aggregata (consumptor refusionem parit - lex rationarii:
 * membrum numquam divinatur) */
interior constans TypusC89*
_tag_accessus (constans AnalysisPlagulae* an,
    constans SilvaNodus* accessus)
{
    SilvaValor b_v = silva_c89_accessus_basis(accessus);
    SilvaValor op_v = silva_c89_accessus_tok_operator(accessus);
    constans TypusC89* tb;

    si (b_v.genus != SILVA_VALOR_NODUS || b_v.datum.nodus == NIHIL
        || op_v.genus != SILVA_VALOR_TOKEN
        || op_v.datum.token == NIHIL)
    {
        redde NIHIL;
    }
    tb = silva_c89_typus_expressionis(an->sem, b_v.datum.nodus);
    si (tb == NIHIL)
    {
        redde NIHIL;
    }
    si (op_v.datum.token->genus == SILVA_LEX_SAGITTA)
    {
        dum (tb->genus == (s32)TYPUS_C89_QUALIFICATUS)
        {
            tb = tb->datum.qualificatus.internum;
        }
        si (tb->genus != (s32)TYPUS_C89_MONSTRATOR)
        {
            redde NIHIL;
        }
        tb = tb->datum.monstrator.internum;
        si (tb == NIHIL)
        {
            redde NIHIL;
        }
    }
    dum (tb->genus == (s32)TYPUS_C89_QUALIFICATUS)
    {
        tb = tb->datum.qualificatus.internum;
    }
    si (tb->genus != (s32)TYPUS_C89_STRUCTURA
        && tb->genus != (s32)TYPUS_C89_UNIO)
    {
        redde NIHIL;
    }
    redde tb;
}

/* symbolum ligatum entitati nostrae? (discretio staticorum
 * homonymorum - nucleus correctitudinis) */
interior b32
_symbolum_entitatis (constans AnalysisPlagulae* an,
    constans SemanticaSymbolum* s)
{
    si (s == NIHIL || s->ex_systemate)
    {
        redde FALSUM;
    }
    si (membrum_l != NIHIL)
    {
        /* spatium nominum membrorum: symbola OMNIA aliena */
        redde FALSUM;
    }
    si (intra_l != NIHIL)
    {
        /* modus localis: aequalitas monstratorum - exacta */
        redde (s == symbolum_localis && an == analysis_localis)
            ? VERUM : FALSUM;
    }
    si (s->profunditas != ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (!_chordae_pares_lit(s->titulus, vetus_l))
    {
        redde FALSUM;
    }
    si (entitas_statica)
    {
        /* entitas = staticum plagulae certae */
        si (strcmp(an->via, entitas_via_statica) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        redde VERUM;
    }
    /* entitas externa: statica homonyma alibi excluduntur */
    si (s->repositio & REPOSITIO_STATICA)
    {
        redde FALSUM;
    }
    redde VERUM;
}

/* ambulatio arboris: classes lexematum vetus-scribentium ponere.
 * in_membro: intra declarationem membrorum structurae/unionis. */
nomen structura {
    SilvaValor v;
    b32        in_membro;
    b32        in_structura_electa;   /* subarbor declarantis tag
                                       * target (modus membri) */
} GradusAmbulationis;

interior vacuum
_arborem_classificare (constans AnalysisPlagulae* an,
    TabulaDispersa* classes, Piscina* effimera)
{
    Xar* series = xar_creare(effimera,
        (i32)magnitudo(GradusAmbulationis));
    i32 cursor = ZEPHYRUM;

    si (series == NIHIL)
    {
        redde;
    }
    {
        GradusAmbulationis* radix =
            (GradusAmbulationis*)xar_addere(series);

        si (radix == NIHIL)
        {
            redde;
        }
        radix->v = an->parsura->commissio->radix;
        radix->in_membro = FALSUM;
        radix->in_structura_electa = FALSUM;
    }
    dum (cursor < xar_numerus(series))
    {
        GradusAmbulationis gradus =
            *(GradusAmbulationis*)xar_obtinere(series, cursor);
        SilvaValor v = gradus.v;

        cursor++;
        si (v.genus == SILVA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(v); k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v,
                    (insignatus integer)k);

                si (elem != NIHIL)
                {
                    GradusAmbulationis* novus =
                        (GradusAmbulationis*)xar_addere(series);

                    si (novus != NIHIL)
                    {
                        novus->v = *elem;
                        novus->in_membro = gradus.in_membro;
                        novus->in_structura_electa =
                            gradus.in_structura_electa;
                    }
                }
            }
            perge;
        }
        si (v.genus != SILVA_VALOR_NODUS || v.datum.nodus == NIHIL)
        {
            perge;
        }
        {
            constans SilvaNodus* nodus = v.datum.nodus;
            b32 in_membro = gradus.in_membro;
            b32 in_structura_electa = gradus.in_structura_electa;
            i32 k;

            /* modus membri: subarbor declarantis tag target */
            si (membrum_l != NIHIL && an->tag_electus != NIHIL
                && (nodus->genus == (s32)SILVA_C89_GENUS_STRUCTURA
                    || nodus->genus == (s32)SILVA_C89_GENUS_UNIO)
                && nodus == an->tag_electus->datum.tag.declarans)
            {
                in_structura_electa = VERUM;
            }

            si (nodus->genus == (s32)SILVA_C89_GENUS_AMBIGUUS)
            {
                SilvaValor interp =
                    silva_c89_ambiguus_interpretationes(nodus);
                SilvaValor canonica =
                    silva_c89_ambiguus_canonica(nodus);

                si (canonica.genus == SILVA_VALOR_INDEX)
                {
                    SilvaValor* lectio =
                        silva_valor_lista_obtinere(interp,
                            (insignatus integer)
                                canonica.datum.index);

                    si (lectio != NIHIL)
                    {
                        GradusAmbulationis* novus =
                            (GradusAmbulationis*)xar_addere(
                                series);

                        si (novus != NIHIL)
                        {
                            novus->v = *lectio;
                            novus->in_membro = in_membro;
                            novus->in_structura_electa =
                                in_structura_electa;
                        }
                    }
                }
                perge;
            }
            si (nodus->genus == (s32)SILVA_C89_GENUS_ERROR
                || nodus->genus
                    == (s32)SILVA_C89_GENUS_RAMUS_OMISSUS)
            {
                perge;
            }

            /* classes per genus nodi */
            si (nodus->genus
                == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
            {
                SilvaToken* tok = _tok_valoris(
                    silva_c89_folium_identificator_tok_valor(
                        nodus));

                si (_tok_vetus_scribit(tok))
                {
                    constans SemanticaSymbolum* s =
                        silva_c89_symbolum_nodi(an->sem, nodus);

                    si (s != NIHIL)
                    {
                        _classem_ponere(classes, tok,
                            _symbolum_entitatis(an, s)
                                ? CLASSIS_USUS : CLASSIS_ALIENA);
                    }
                }
            }
            alioquin si (nodus->genus
                == (s32)SILVA_C89_GENUS_ACCESSUS)
            {
                SilvaToken* tok = _tok_valoris(
                    silva_c89_accessus_tok_titulus(nodus));

                si (_tok_vetus_scribit(tok))
                {
                    si (membrum_l != NIHIL)
                    {
                        constans TypusC89* tag = _tag_accessus(an,
                            nodus);

                        si (tag == NIHIL)
                        {
                            /* basis sine typo - membrum numquam
                             * divinatur (lex rationarii) */
                            _classem_ponere(classes, tok,
                                CLASSIS_REFUSIO);
                        }
                        alioquin
                        {
                            _classem_ponere(classes, tok,
                                tag == an->tag_electus
                                    ? CLASSIS_USUS
                                    : CLASSIS_ALIENA);
                        }
                    }
                    alioquin
                    {
                        _classem_ponere(classes, tok,
                            CLASSIS_ALIENA);
                    }
                }
            }
            alioquin si (nodus->genus
                == (s32)SILVA_C89_GENUS_TITULATUM)
            {
                SilvaToken* tok = _tok_valoris(
                    silva_c89_titulatum_tok_titulus(nodus));

                si (_tok_vetus_scribit(tok))
                {
                    _classem_ponere(classes, tok, CLASSIS_ALIENA);
                }
            }
            alioquin si (nodus->genus == (s32)SILVA_C89_GENUS_SALTA)
            {
                SilvaToken* tok = _tok_valoris(
                    silva_c89_salta_tok_destinatio(nodus));

                si (_tok_vetus_scribit(tok))
                {
                    _classem_ponere(classes, tok, CLASSIS_ALIENA);
                }
            }
            alioquin si (nodus->genus
                == (s32)SILVA_C89_GENUS_TYPUS_NOMINATUS)
            {
                SilvaToken* tok = _tok_valoris(
                    silva_c89_typus_nominatus_tok_titulus(nodus));

                si (_tok_vetus_scribit(tok))
                {
                    /* usus typedef: entitas typedef -> splicenda
                     * (umbrae localis typedef refusione rationarii
                     * caventur - registratio profunda homonyma
                     * culpam infra parit) */
                    _classem_ponere(classes, tok,
                        entitas_genus == (s32)SYMBOLUM_TYPEDEF
                            ? CLASSIS_USUS : CLASSIS_ALIENA);
                }
            }
            alioquin si (nodus->genus
                == (s32)SILVA_C89_GENUS_DECLARATOR_TITULUS)
            {
                SilvaToken* tok = _tok_valoris(
                    silva_c89_declarator_titulus_tok_titulus(
                        nodus));

                si (_tok_vetus_scribit(tok))
                {
                    /* modus membri: declarator intra structuram
                     * ELECTAM = sedes. Ceteri declaratores sine
                     * registratione (parametra prototyporum,
                     * tituli parametrorum monstratorum functionum
                     * - mensuratum in tessera) = ALIENA; sedes
                     * registratae iam classificatae sunt (prima
                     * positio vincit). */
                    _classem_ponere(classes, tok,
                        (membrum_l != NIHIL && in_structura_electa)
                            ? CLASSIS_SEDES : CLASSIS_ALIENA);
                }
            }
            alioquin si (nodus->genus
                    == (s32)SILVA_C89_GENUS_MEMBRUM
                || nodus->genus == (s32)SILVA_C89_GENUS_CAMPUS)
            {
                in_membro = VERUM;
            }

            per (k = ZEPHYRUM; k < (i32)nodus->numerus_locorum; k++)
            {
                GradusAmbulationis* novus =
                    (GradusAmbulationis*)xar_addere(series);

                si (novus != NIHIL)
                {
                    novus->v = nodus->loci[
                        (insignatus integer)k];
                    novus->in_membro = in_membro;
                    novus->in_structura_electa =
                        in_structura_electa;
                }
            }
        }
    }
}

/* registrationes: lexemata declarationum symbolorum omnium (etiam
 * profundorum - homonyma localia ALIENA fiunt, entitas SEDES) */
interior vacuum
_registrationes_classificare (constans AnalysisPlagulae* an,
    TabulaDispersa* classes)
{
    i32 numerus = (i32)silva_c89_symbola_numerus(an->sem);
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(an->sem,
                (insignatus integer)k);

        si (s == NIHIL || s->lexema == NIHIL || s->ex_systemate
            || s->est_implicitum)
        {
            perge;
        }
        si (!_tok_vetus_scribit(s->lexema))
        {
            perge;
        }
        _classem_ponere(classes, s->lexema,
            _symbolum_entitatis(an, s)
                ? CLASSIS_SEDES : CLASSIS_ALIENA);
    }
}

/* ==================================================
 * intervalla textualia (corpora macronum + rami omissi)
 * ================================================== */

/* ==================================================
 * sedes addere (dedup trans TU per via|offset)
 * ================================================== */

interior vacuum
_sedem_addere (s32 classis, constans character* via, i32 linea,
    i32 columna, s32 offset, constans character* nota)
{
    SedesInventa* sedes;

    si (offset >= ZEPHYRUM)
    {
        character clavis_l[DXII];
        chorda clavis;
        int scripti = sprintf(clavis_l, "%s|%ld", via,
            (long)offset);

        si (scripti > ZEPHYRUM)
        {
            clavis = _ch(clavis_l);
            si (tabula_dispersa_continet(sedes_visae, clavis))
            {
                redde;
            }
            (vacuum)tabula_dispersa_inserere(sedes_visae, clavis,
                NIHIL);
        }
    }
    sedes = (SedesInventa*)xar_addere(sedes_omnes);
    si (sedes == NIHIL)
    {
        redde;
    }
    sedes->classis = classis;
    sedes->via = via;
    sedes->linea = linea;
    sedes->columna = columna;
    sedes->offset = offset;
    sedes->nota = nota;
}

/* ==================================================
 * testimonium macronum (v0.1)
 * ================================================== */

interior constans character* _via_lexematis (
    constans AnalysisPlagulae* an, SilvaToken* tok);

interior MacroTestimonium*
_testimonium_capere (constans chorda* titulus)
{
    chorda clavis;
    vacuum* valor;
    MacroTestimonium* t;

    clavis.datum = (i8*)titulus->datum;
    clavis.mensura = (i32)titulus->mensura;
    si (tabula_dispersa_invenire(macro_index, clavis, &valor))
    {
        redde (MacroTestimonium*)xar_obtinere(macro_testimonia,
            (i32)((memoriae_index)valor - I));
    }
    t = (MacroTestimonium*)xar_addere(macro_testimonia);
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    t->titulus = _litterae(chorda_transcribere(clavis,
        piscina_magistra));
    t->via_def = NIHIL;
    t->linea_def = ZEPHYRUM;
    t->defs = ZEPHYRUM;
    t->param_homonymum = FALSUM;
    t->ligata_entitati = ZEPHYRUM;
    t->ligata_aliis = ZEPHYRUM;
    t->non_ligata = ZEPHYRUM;
    t->sedes_corporis = xar_creare(piscina_magistra,
        (i32)magnitudo(SedesInventa));
    t->invocationes = xar_creare(piscina_magistra,
        (i32)magnitudo(SedesInventa));
    (vacuum)tabula_dispersa_inserere(macro_index,
        chorda_transcribere(clavis, piscina_magistra),
        (vacuum*)(memoriae_index)xar_numerus(macro_testimonia));
    redde t;
}

/* nomen macronis PRIMI gradus originis: macro ex cuius CORPORE
 * lexema hoc copiatum est (contra titulus_macronis nexus, qui ad
 * EXTIMUM ascendit) */
interior constans chorda*
_macro_primum (SilvaToken* tok)
{
    commutatio (tok->origo.genus)
    {
        casus SILVA_ORIGO_EXPANSIO:
            redde tok->origo.datum.expansio.nomen_macro;
        casus SILVA_ORIGO_PASTA:
            redde tok->origo.datum.pasta.nomen_macro;
        casus SILVA_ORIGO_CHORDA:
            redde tok->origo.datum.stringificatio.nomen_macro;
        casus SILVA_ORIGO_API:
            redde tok->origo.datum.api.nomen_macro;
        ordinarius:
            redde NIHIL;
    }
}

/* sedes in xar privatum testimonii (dedup differtur ad emissionem
 * per _sedem_addere) */
interior vacuum
_sedem_testimonii (Xar* quo, s32 classis, constans character* via,
    i32 linea, i32 columna, s32 offset, constans character* nota)
{
    SedesInventa* s = (SedesInventa*)xar_addere(quo);

    si (s == NIHIL)
    {
        redde;
    }
    s->classis = classis;
    s->via = via;
    s->linea = linea;
    s->columna = columna;
    s->offset = offset;
    s->nota = nota;
}

/* corpora macronum TU huius: candidata + parametra homonyma.
 * Definitiones ex fonte QUOVIS repositorii ('/'-via) - dedup per
 * via|offset in emissione. */
interior vacuum
_macros_colligere (constans AnalysisPlagulae* an)
{
    i32 n = xar_numerus(an->parsura->expansio->acta);
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaEventum* ev = (SilvaEventum*)xar_obtinere(
            an->parsura->expansio->acta, k);
        SilvaMacroDef* def;
        MacroTestimonium* t;
        constans character* via_def;
        i32 j;

        si (ev == NIHIL || ev->genus != SILVA_EVENTUM_DEFINITIO
            || ev->def == NIHIL || ev->def->ex_api)
        {
            perge;
        }
        def = ev->def;
        {
            constans chorda* via_ch = silva_fons_via(
                an->parsura->expansio, def->fons_index);
            b32 habet_sep = FALSUM;
            i32 c;

            si (via_ch == NIHIL)
            {
                perge;
            }
            per (c = ZEPHYRUM; c < via_ch->mensura; c++)
            {
                si (via_ch->datum[c] == '/')
                {
                    habet_sep = VERUM;
                    frange;
                }
            }
            si (!habet_sep)
            {
                perge;   /* copia basename praebita */
            }
        }
        /* interest solum si corpus aut parametra vetus tangunt */
        {
            b32 tangit = FALSUM;

            si (def->parametra != NIHIL)
            {
                per (j = ZEPHYRUM;
                     j < xar_numerus(def->parametra); j++)
                {
                    chorda** p = (chorda**)xar_obtinere(
                        def->parametra, j);

                    si (p != NIHIL && *p != NIHIL
                        && _chordae_pares_lit(**p, vetus_l))
                    {
                        tangit = VERUM;
                    }
                }
            }
            si (!tangit && def->corpus != NIHIL)
            {
                per (j = ZEPHYRUM; j < xar_numerus(def->corpus);
                     j++)
                {
                    SilvaToken** c = (SilvaToken**)xar_obtinere(
                        def->corpus, j);

                    si (c != NIHIL && _tok_vetus_scribit(*c))
                    {
                        tangit = VERUM;
                    }
                }
            }
            si (!tangit)
            {
                perge;
            }
        }
        si (def->titulus == NIHIL)
        {
            perge;
        }
        t = _testimonium_capere(def->titulus);
        si (t == NIHIL)
        {
            perge;
        }
        via_def = _via_lexematis(an,
            def->corpus != NIHIL && xar_numerus(def->corpus)
                    > ZEPHYRUM
                ? *(SilvaToken**)xar_obtinere(def->corpus,
                      ZEPHYRUM)
                : NIHIL);
        si (via_def == NIHIL)
        {
            /* via ex indice fontis definitionis */
            constans chorda* via_ch = silva_fons_via(
                an->parsura->expansio, def->fons_index);
            chorda c;

            c.datum = (i8*)via_ch->datum;
            c.mensura = (i32)via_ch->mensura;
            via_def = _litterae(c);
        }
        /* definitio distincta? (via|linea) - homonymae demotant */
        si (t->via_def == NIHIL)
        {
            t->via_def = via_def;
            t->linea_def = def->linea_def;
            t->defs = I;
        }
        alioquin si (strcmp(t->via_def, via_def) != ZEPHYRUM
            || t->linea_def != def->linea_def)
        {
            t->defs++;
        }
        /* parametrum homonymum: corpus refert PARAMETRUM, non
         * symbolum - sedes alienae */
        si (def->parametra != NIHIL)
        {
            per (j = ZEPHYRUM; j < xar_numerus(def->parametra);
                 j++)
            {
                chorda** p = (chorda**)xar_obtinere(
                    def->parametra, j);

                si (p != NIHIL && *p != NIHIL
                    && _chordae_pares_lit(**p, vetus_l))
                {
                    t->param_homonymum = VERUM;
                }
            }
        }
        si (def->corpus == NIHIL)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(def->corpus); j++)
        {
            SilvaToken** cella = (SilvaToken**)xar_obtinere(
                def->corpus, j);
            SilvaToken* tok;

            si (cella == NIHIL || !_tok_vetus_scribit(*cella))
            {
                perge;
            }
            tok = *cella;
            si (tok->byte_offset < ZEPHYRUM)
            {
                perge;
            }
            {
                constans character* via_t = _via_lexematis(an,
                    tok);

                si (via_t != NIHIL)
                {
                    _sedem_testimonii(t->sedes_corporis,
                        CLASSIS_USUS, via_t, tok->linea,
                        tok->columna, tok->byte_offset, NIHIL);
                }
            }
        }
    }
}

/* rami omissi + expressiones conditionum: lexemata VERA (chordae
 * et commenta non fallunt, quod scansio textualis faciebat) */
interior vacuum
_ramos_colligere (constans AnalysisPlagulae* an)
{
    i32 n = xar_numerus(an->parsura->expansio->rami);
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaRamus** cella = (SilvaRamus**)xar_obtinere(
            an->parsura->expansio->rami, k);
        SilvaRamus* ramus;
        i32 j;

        si (cella == NIHIL || *cella == NIHIL)
        {
            perge;
        }
        ramus = *cella;
        si (ramus->lexemata_cruda != NIHIL)
        {
            per (j = ZEPHYRUM;
                 j < xar_numerus(ramus->lexemata_cruda); j++)
            {
                SilvaToken** c = (SilvaToken**)xar_obtinere(
                    ramus->lexemata_cruda, j);

                si (c != NIHIL && _tok_vetus_scribit(*c)
                    && (*c)->byte_offset >= ZEPHYRUM)
                {
                    constans character* via_t = _via_lexematis(
                        an, *c);

                    si (via_t != NIHIL)
                    {
                        _sedem_addere(CLASSIS_MANUALIS, via_t,
                            (*c)->linea, (*c)->columna,
                            (*c)->byte_offset,
                            "in ramo praeprocessoris omisso");
                    }
                }
            }
        }
        si (ramus->expressio != NIHIL)
        {
            per (j = ZEPHYRUM;
                 j < xar_numerus(ramus->expressio); j++)
            {
                SilvaToken** c = (SilvaToken**)xar_obtinere(
                    ramus->expressio, j);

                si (c != NIHIL && _tok_vetus_scribit(*c)
                    && (*c)->byte_offset >= ZEPHYRUM)
                {
                    constans character* via_t = _via_lexematis(
                        an, *c);

                    si (via_t != NIHIL)
                    {
                        _sedem_addere(CLASSIS_MANUALIS, via_t,
                            (*c)->linea, (*c)->columna,
                            (*c)->byte_offset,
                            "in conditione praeprocessoris");
                    }
                }
            }
        }
    }
}

/* resolutio testimoniorum (post TUs omnes): promotio aut demotio */
interior vacuum
_testimonia_resolvere (vacuum)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < xar_numerus(macro_testimonia); k++)
    {
        MacroTestimonium* t = (MacroTestimonium*)xar_obtinere(
            macro_testimonia, k);
        b32 promotum = FALSUM;
        constans character* causa = NIHIL;

        si (t->param_homonymum)
        {
            /* corpus parametrum refert - sedes alienae */
            per (j = ZEPHYRUM; j < xar_numerus(t->sedes_corporis);
                 j++)
            {
                SedesInventa* s = (SedesInventa*)xar_obtinere(
                    t->sedes_corporis, j);

                _sedem_addere(CLASSIS_ALIENA, s->via, s->linea,
                    s->columna, s->offset,
                    "parametrum macronis homonymum");
            }
            perge;
        }
        si (t->defs > I)
        {
            causa = "corpus macronis - definitiones homonymae"
                " plures, promotio recusata";
        }
        alioquin si (t->ligata_aliis > ZEPHYRUM
            || t->non_ligata > ZEPHYRUM)
        {
            causa = "corpus macronis - ligamina expansionum"
                " divergentia";
        }
        alioquin si (t->ligata_entitati == ZEPHYRUM)
        {
            causa = "corpus macronis - macro numquam invocatum"
                " in plagulis analysatis";
        }
        alioquin
        {
            promotum = VERUM;
        }
        per (j = ZEPHYRUM; j < xar_numerus(t->sedes_corporis); j++)
        {
            SedesInventa* s = (SedesInventa*)xar_obtinere(
                t->sedes_corporis, j);

            si (promotum)
            {
                _sedem_addere(CLASSIS_USUS, s->via, s->linea,
                    s->columna, s->offset,
                    "corpus macronis - promotum per invocationes");
            }
            alioquin
            {
                _sedem_addere(CLASSIS_MANUALIS, s->via, s->linea,
                    s->columna, s->offset, causa);
            }
        }
        /* invocationes: promotione facta nihil manuale restat
         * (corpus editur, invocationes sequuntur); aliter rows
         * manuales manent. Testimonium sine sedibus corporis
         * (constructio per pastam) invocationes semper servat. */
        si (!promotum || xar_numerus(t->sedes_corporis)
                == ZEPHYRUM)
        {
            per (j = ZEPHYRUM; j < xar_numerus(t->invocationes);
                 j++)
            {
                SedesInventa* s = (SedesInventa*)xar_obtinere(
                    t->invocationes, j);

                _sedem_addere(CLASSIS_MANUALIS, s->via, s->linea,
                    s->columna, s->offset, s->nota);
            }
        }
    }
}

/* ==================================================
 * collectio plagulae: classes -> sedes + rationarium
 * ================================================== */

interior constans character*
_via_lexematis (constans AnalysisPlagulae* an, SilvaToken* tok)
{
    constans chorda* via;

    si (tok == NIHIL)
    {
        redde NIHIL;
    }
    via = silva_fons_via(an->parsura->expansio, tok->fons_index);
    si (via == NIHIL)
    {
        redde NIHIL;
    }
    /* basename praebitum (sine '/') = copia capitis - via vera ex
     * ambulatione directa venit; systema quoque excluditur */
    {
        b32 habet_separatorem = FALSUM;
        i32 k;

        per (k = ZEPHYRUM; k < via->mensura; k++)
        {
            si (via->datum[k] == '/')
            {
                habet_separatorem = VERUM;
                frange;
            }
        }
        si (!habet_separatorem)
        {
            redde NIHIL;
        }
    }
    {
        chorda c;

        c.datum = (i8*)via->datum;
        c.mensura = (i32)via->mensura;
        /* praefixum "./" ambulationis detractum - dedup trans TU
         * viam UNAM postulat (caput ex ambulatione "./x" contra
         * plagulam principalem "x") */
        si (c.mensura > II && c.datum[ZEPHYRUM] == (i8)'.'
            && c.datum[I] == (i8)'/')
        {
            c.datum += II;
            c.mensura -= II;
        }
        redde _litterae(c);
    }
}

/* lexema in linea directivae? (initium lineae logicae = '#') -
 * fons per fons_index: plagula principalis aut caput lectum */
interior b32
_in_directiva (constans AnalysisPlagulae* an, SilvaToken* tok)
{
    constans character* fons = NIHIL;
    s32 mensura = ZEPHYRUM;
    s32 i;

    si (tok->fons_index == an->parsura->fons_princeps)
    {
        fons = an->fons;
        mensura = (s32)an->mensura;
    }
    alioquin
    {
        constans chorda* via = silva_fons_via(
            an->parsura->expansio, tok->fons_index);
        i32 k;

        si (via == NIHIL)
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < xar_numerus(capita_lecta); k++)
        {
            CaputLectum* caput = (CaputLectum*)xar_obtinere(
                capita_lecta, k);

            si ((i32)strlen(caput->via) == (i32)via->mensura
                && memcmp(caput->via, via->datum,
                       (memoriae_index)via->mensura) == ZEPHYRUM)
            {
                fons = caput->textus;
                mensura = (s32)caput->mensura;
                frange;
            }
        }
        si (fons == NIHIL)
        {
            redde FALSUM;
        }
    }
    si (tok->byte_offset < ZEPHYRUM || tok->byte_offset >= mensura)
    {
        redde FALSUM;
    }
    /* retro ad initium lineae (continuationes '\' inclusae) */
    i = tok->byte_offset;
    dum (i > ZEPHYRUM)
    {
        si (fons[i - I] == '\n')
        {
            /* linea praecedens continuatione desinens? */
            s32 p = i - II;

            dum (p >= ZEPHYRUM && fons[p] == '\r')
            {
                p--;
            }
            si (p >= ZEPHYRUM && fons[p] == '\\')
            {
                i = p;
                perge;
            }
            frange;
        }
        i--;
    }
    dum (i < mensura && (fons[i] == ' ' || fons[i] == '\t'))
    {
        i++;
    }
    redde (i < mensura && fons[i] == '#') ? VERUM : FALSUM;
}

interior vacuum
_plagulam_colligere (constans AnalysisPlagulae* an)
{
    Piscina* effimera = piscina_generare_dynamicum(
        "renominare_effimera", 67108864);
    TabulaDispersa* classes;
    i32 n;
    i32 k;

    si (effimera == NIHIL)
    {
        redde;
    }
    classes = tabula_dispersa_creare_chorda(piscina_magistra, DXII);
    si (classes == NIHIL)
    {
        piscina_destruere(effimera);
        redde;
    }
    _registrationes_classificare(an, classes);
    _arborem_classificare(an, classes, effimera);

    /* rationarium: omne lexema identificatoris vetus-scribens */
    n = (i32)xar_numerus(an->parsura->lexemata);
    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaToken** cella = (SilvaToken**)xar_obtinere(
            an->parsura->lexemata, (insignatus integer)k);
        SilvaToken* tok;

        si (cella == NIHIL || *cella == NIHIL)
        {
            perge;
        }
        tok = *cella;
        si (!_tok_vetus_scribit(tok))
        {
            perge;
        }
        si (!silva_token_est_fons(tok))
        {
            /* expansio: testimonium macroni primi gradus (macro
             * ex cuius corpore lexema venit) - ligamen huius
             * instantiae = probatio promotionis corporis (v0.1) */
            constans chorda* nomen_m = _macro_primum(tok);
            SilvaToken* radix = silva_token_radix(tok);
            MacroTestimonium* t = nomen_m != NIHIL
                ? _testimonium_capere(nomen_m) : NIHIL;

            si (t != NIHIL)
            {
                s32 classis;

                si (_classem_capere(classes, tok, &classis))
                {
                    si (classis == CLASSIS_USUS
                        || classis == CLASSIS_SEDES)
                    {
                        t->ligata_entitati++;
                    }
                    alioquin
                    {
                        t->ligata_aliis++;
                    }
                }
                alioquin
                {
                    t->non_ligata++;
                }
                si (radix != NIHIL
                    && radix->byte_offset >= ZEPHYRUM)
                {
                    constans character* via_r = _via_lexematis(
                        an, radix);

                    si (via_r != NIHIL)
                    {
                        _sedem_testimonii(t->invocationes,
                            CLASSIS_MANUALIS, via_r, radix->linea,
                            radix->columna, radix->byte_offset,
                            "per expansionem macronis");
                    }
                }
            }
            perge;
        }
        si (tok->byte_offset < ZEPHYRUM)
        {
            perge;   /* syntheticum */
        }
        {
            constans character* via_t = _via_lexematis(an, tok);
            s32 classis;

            si (via_t == NIHIL)
            {
                perge;   /* systema/lexicon - non fons verus */
            }
            si (_classem_capere(classes, tok, &classis))
            {
                si (classis == CLASSIS_REFUSIO)
                {
                    /* basis accessus sine typo (modus membri) */
                    _sedem_addere(CLASSIS_REFUSIO, via_t,
                        tok->linea, tok->columna, -I,
                        "basis accessus sine typo");
                    _culpam_addere(_culpa_formata(
                        "basis accessus sine typo - membrum non"
                        " divinatur", via_t, tok->linea,
                        tok->columna));
                    perge;
                }
                /* offset etiam alienis: dedup trans TU + calculus
                 * superstitum post scriptionem */
                _sedem_addere(classis, via_t, tok->linea,
                    tok->columna, tok->byte_offset, NIHIL);
            }
            alioquin si (_in_directiva(an, tok))
            {
                /* lexema in linea directivae (#define corpus,
                 * #if...) - textus non arboris, manuale */
                _sedem_addere(CLASSIS_MANUALIS, via_t, tok->linea,
                    tok->columna, tok->byte_offset,
                    "in directiva praeprocessoris");
            }
            alioquin
            {
                /* inexplicatum - refusio (lex rationarii) */
                _sedem_addere(CLASSIS_REFUSIO, via_t, tok->linea,
                    tok->columna, -I, "sedes inexplicata");
                _culpam_addere(_culpa_formata(
                    "sedes inexplicata (analysis non ligavit)",
                    via_t, tok->linea, tok->columna));
            }
        }
    }

    /* v0.1 (01KYX2DSKK): corpora macronum + rami omissi per
     * lexemata VERA - substrata quae iam exstabant (scansio
     * textualis v0 = firma quinta legis subaestimationis) */
    _macros_colligere(an);
    _ramos_colligere(an);

    /* porta collisionis: registratio quaevis 'novum' titulata in
     * plagula affecta. Modo locali OMISSA - porta capturae (novum
     * intra functionem) instrumentum rectum est: localis 'k' in
     * functione ALIENA collisio non est */
    n = (intra_l != NIHIL || membrum_l != NIHIL) ? ZEPHYRUM
        : (i32)silva_c89_symbola_numerus(an->sem);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(an->sem,
                (insignatus integer)k);

        si (s == NIHIL || s->ex_systemate || s->est_implicitum)
        {
            perge;
        }
        si (_chordae_pares_lit(s->titulus, novum_l))
        {
            constans character* via_s = an->via;
            i32 linea_s = ZEPHYRUM;

            si (s->lexema != NIHIL)
            {
                constans character* via_t = _via_lexematis(an,
                    s->lexema);

                si (via_t != NIHIL)
                {
                    via_s = via_t;
                }
                linea_s = s->lexema->linea;
            }
            _culpam_addere(_culpa_formata(
                "collisio: nomen novum iam registratum", via_s,
                linea_s, ZEPHYRUM));
        }
    }

    /* porta macronis: 'novum' macro definitum in TU (latina.h per
     * derivationem capitur - lex lexici examinis) */
    n = silva_macros_numerus(an->parsura->expansio);
    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaMacroVista vista;

        si (!silva_macro_vista(an->parsura->expansio, k, &vista)
            || vista.titulus == NIHIL)
        {
            perge;
        }
        si (_chordae_pares_lit(*vista.titulus, novum_l))
        {
            character culpa_l[DXII];
            constans chorda* via_m = silva_fons_via(
                an->parsura->expansio, vista.fons_index);
            int scripti = sprintf(culpa_l,
                "nomen novum macro definitum est (%.*s:%d)",
                via_m != NIHIL ? (int)via_m->mensura : I,
                via_m != NIHIL
                    ? (constans character*)via_m->datum : "?",
                (int)vista.linea);

            si (scripti > ZEPHYRUM)
            {
                _culpam_addere(_litterae(_ch(culpa_l)));
            }
            frange;   /* culpa una sufficit per plagulam */
        }
    }

    piscina_destruere(effimera);
}

/* ==================================================
 * resolutio entitatis (trans plagulas)
 * ================================================== */

/* ==================================================
 * resolutio localis (-intra): functio -> subarbor -> registratio
 * ================================================== */

/* subarborem functionis ambulare: lexemata vetus-scribentia in
 * copiam (clavis = monstrator), lexemata NOVUM-scribentia = porta
 * capturae (utroque modo: umbra interior aut symbolum externum
 * umbratum - refusio conservativa). AMBIGUUS per canonicam. */
interior vacuum
_subarborem_lustrare (constans AnalysisPlagulae* an,
    SilvaNodus* functio, TabulaDispersa* copia_vetus,
    Piscina* effimera)
{
    Xar* series = xar_creare(effimera, (i32)magnitudo(SilvaValor));
    i32 cursor = ZEPHYRUM;

    si (series == NIHIL)
    {
        redde;
    }
    {
        SilvaValor* radix = (SilvaValor*)xar_addere(series);

        si (radix == NIHIL)
        {
            redde;
        }
        radix->genus = SILVA_VALOR_NODUS;
        radix->datum.nodus = functio;
    }
    dum (cursor < xar_numerus(series))
    {
        SilvaValor v = *(SilvaValor*)xar_obtinere(series, cursor);

        cursor++;
        si (v.genus == SILVA_VALOR_TOKEN && v.datum.token != NIHIL)
        {
            SilvaToken* tok = v.datum.token;

            si (_tok_vetus_scribit(tok))
            {
                _classem_ponere(copia_vetus, tok, ZEPHYRUM);
            }
            alioquin si (tok->genus == SILVA_LEX_IDENTIFICATOR
                && _chordae_pares_lit(tok->valor, novum_l))
            {
                constans character* via_t = _via_lexematis(an,
                    tok);

                _culpam_addere(_culpa_formata(
                    "captura: nomen novum intra functionem iam"
                    " apparet", via_t != NIHIL ? via_t : an->via,
                    tok->linea, tok->columna));
            }
            perge;
        }
        si (v.genus == SILVA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(v); k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v,
                    (insignatus integer)k);
                SilvaValor* novus;

                si (elem == NIHIL)
                {
                    perge;
                }
                novus = (SilvaValor*)xar_addere(series);
                si (novus != NIHIL)
                {
                    *novus = *elem;
                }
            }
            perge;
        }
        si (v.genus != SILVA_VALOR_NODUS || v.datum.nodus == NIHIL)
        {
            perge;
        }
        {
            constans SilvaNodus* nodus = v.datum.nodus;
            i32 k;

            si (nodus->genus == (s32)SILVA_C89_GENUS_AMBIGUUS)
            {
                SilvaValor interp =
                    silva_c89_ambiguus_interpretationes(nodus);
                SilvaValor canonica =
                    silva_c89_ambiguus_canonica(nodus);

                si (canonica.genus == SILVA_VALOR_INDEX)
                {
                    SilvaValor* lectio =
                        silva_valor_lista_obtinere(interp,
                            (insignatus integer)
                                canonica.datum.index);
                    SilvaValor* novus;

                    si (lectio != NIHIL)
                    {
                        novus = (SilvaValor*)xar_addere(series);
                        si (novus != NIHIL)
                        {
                            *novus = *lectio;
                        }
                    }
                }
                perge;
            }
            si (nodus->genus == (s32)SILVA_C89_GENUS_ERROR
                || nodus->genus
                    == (s32)SILVA_C89_GENUS_RAMUS_OMISSUS)
            {
                perge;
            }
            per (k = ZEPHYRUM; k < (i32)nodus->numerus_locorum;
                 k++)
            {
                SilvaValor* novus = (SilvaValor*)xar_addere(
                    series);

                si (novus != NIHIL)
                {
                    *novus = nodus->loci[(insignatus integer)k];
                }
            }
        }
    }
}

/* definitionem functionis intra_l titulatam in arbore invenire */
interior SilvaNodus*
_functionem_invenire (constans AnalysisPlagulae* an,
    Piscina* effimera)
{
    Xar* series = xar_creare(effimera, (i32)magnitudo(SilvaValor));
    i32 cursor = ZEPHYRUM;
    SilvaNodus* inventa = NIHIL;

    si (series == NIHIL)
    {
        redde NIHIL;
    }
    {
        SilvaValor* radix = (SilvaValor*)xar_addere(series);

        si (radix == NIHIL)
        {
            redde NIHIL;
        }
        *radix = an->parsura->commissio->radix;
    }
    dum (cursor < xar_numerus(series) && inventa == NIHIL)
    {
        SilvaValor v = *(SilvaValor*)xar_obtinere(series, cursor);

        cursor++;
        si (v.genus == SILVA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(v); k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v,
                    (insignatus integer)k);
                SilvaValor* novus;

                si (elem == NIHIL)
                {
                    perge;
                }
                novus = (SilvaValor*)xar_addere(series);
                si (novus != NIHIL)
                {
                    *novus = *elem;
                }
            }
            perge;
        }
        si (v.genus != SILVA_VALOR_NODUS || v.datum.nodus == NIHIL)
        {
            perge;
        }
        si (v.datum.nodus->genus
            == (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
        {
            SilvaValor decl =
                silva_c89_definitio_functionis_declarator(
                    v.datum.nodus);

            si (decl.genus == SILVA_VALOR_NODUS
                && decl.datum.nodus != NIHIL)
            {
                SilvaToken* titulus =
                    silva_c89_declaratoris_titulus(
                        decl.datum.nodus);

                si (titulus != NIHIL
                    && _chordae_pares_lit(titulus->valor,
                           intra_l))
                {
                    inventa = v.datum.nodus;
                }
            }
            perge;   /* corpora aliena non descendimus */
        }
        {
            constans SilvaNodus* nodus = v.datum.nodus;
            i32 k;

            per (k = ZEPHYRUM; k < (i32)nodus->numerus_locorum;
                 k++)
            {
                SilvaValor* novus = (SilvaValor*)xar_addere(
                    series);

                si (novus != NIHIL)
                {
                    *novus = nodus->loci[(insignatus integer)k];
                }
            }
        }
    }
    redde inventa;
}

interior b32
_localem_resolvere (vacuum)
{
    Piscina* effimera = piscina_generare_dynamicum(
        "renominare_localis", 67108864);
    constans AnalysisPlagulae* an_functionis = NIHIL;
    SilvaNodus* functio = NIHIL;
    TabulaDispersa* copia_vetus;
    i32 k;

    si (effimera == NIHIL)
    {
        redde FALSUM;
    }
    copia_vetus = tabula_dispersa_creare_chorda(piscina_magistra,
        LXIV);
    si (copia_vetus == NIHIL)
    {
        piscina_destruere(effimera);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(analyses); k++)
    {
        AnalysisPlagulae* an = (AnalysisPlagulae*)xar_obtinere(
            analyses, k);
        SilvaNodus* f;

        si (via_data != NIHIL && strcmp(an->via, via_data)
                != ZEPHYRUM)
        {
            perge;
        }
        f = _functionem_invenire(an, effimera);
        si (f == NIHIL)
        {
            perge;
        }
        si (functio != NIHIL)
        {
            _culpam_addere("functio -intra in plagulis pluribus"
                " definita - da -via plagulam certam");
            piscina_destruere(effimera);
            redde FALSUM;
        }
        functio = f;
        an_functionis = an;
    }
    si (functio == NIHIL)
    {
        _culpam_addere("functio -intra non inventa in plagulis"
            " datis");
        piscina_destruere(effimera);
        redde FALSUM;
    }
    _subarborem_lustrare(an_functionis, functio, copia_vetus,
        effimera);

    /* registrationes candidatae: prof > 0, vetus titulatae,
     * lexema intra subarborem */
    {
        i32 n = (i32)silva_c89_symbola_numerus(an_functionis->sem);
        i32 inventae = ZEPHYRUM;
        constans SemanticaSymbolum* electum = NIHIL;
        character lineae_l[CCLVI];
        i32 scripti_summa = ZEPHYRUM;
        i32 j;

        lineae_l[ZEPHYRUM] = '\0';
        per (j = ZEPHYRUM; j < n; j++)
        {
            constans SemanticaSymbolum* s =
                silva_c89_symbolum_per_indicem(an_functionis->sem,
                    (insignatus integer)j);
            s32 ignotum;

            si (s == NIHIL || s->ex_systemate || s->est_implicitum
                || s->profunditas == ZEPHYRUM
                || s->lexema == NIHIL)
            {
                perge;
            }
            si (!_chordae_pares_lit(s->titulus, vetus_l))
            {
                perge;
            }
            si (!_classem_capere(copia_vetus, s->lexema, &ignotum))
            {
                perge;   /* extra functionem */
            }
            si (linea_data > ZEPHYRUM
                && s->lexema->linea != linea_data)
            {
                perge;
            }
            inventae++;
            electum = s;
            si (scripti_summa < CC)
            {
                int scripti = sprintf(lineae_l + scripti_summa,
                    "%s%d", scripti_summa > ZEPHYRUM ? ", " : "",
                    (int)s->lexema->linea);

                si (scripti > ZEPHYRUM)
                {
                    scripti_summa += (i32)scripti;
                }
            }
        }
        si (inventae == ZEPHYRUM)
        {
            _culpam_addere(linea_data > ZEPHYRUM
                ? "localis vetus in linea data non inventa"
                : "localis vetus intra functionem non inventa");
            piscina_destruere(effimera);
            redde FALSUM;
        }
        si (inventae > I)
        {
            character culpa_l[DXII];
            int scripti = sprintf(culpa_l, "declarationes"
                " plures localis intra functionem (lineae %s) -"
                " da -linea", lineae_l);

            _culpam_addere(scripti > ZEPHYRUM
                ? _litterae(_ch(culpa_l))
                : "declarationes plures - da -linea");
            piscina_destruere(effimera);
            redde FALSUM;
        }
        symbolum_localis = electum;
        analysis_localis = an_functionis;
        entitas_genus = electum->genus;
    }
    piscina_destruere(effimera);
    redde VERUM;
}

/* ==================================================
 * resolutio membri (-membrum <typus>): typus nomine per TU
 * ================================================== */

interior b32
_membrum_resolvere (vacuum)
{
    chorda titulus_typi = _ch(membrum_l);
    i32 cum_tag = ZEPHYRUM;
    b32 vetus_inventum = FALSUM;
    b32 novum_inventum = FALSUM;
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(analyses); k++)
    {
        AnalysisPlagulae* an = (AnalysisPlagulae*)xar_obtinere(
            analyses, k);
        TypusC89* t = silva_c89_typedef_invenire(an->sem,
            titulus_typi);
        i32 j;

        si (t == NIHIL)
        {
            /* tag scopi plagulae (structura <titulus>) */
            vacuum* valor;

            si (an->sem->scopus_summus != NIHIL
                && an->sem->scopus_summus->tags != NIHIL
                && tabula_dispersa_invenire(
                       an->sem->scopus_summus->tags,
                       titulus_typi, &valor))
            {
                t = (TypusC89*)valor;
            }
        }
        si (t == NIHIL)
        {
            perge;   /* TU typum nescit - sedes target nullae */
        }
        dum (t->genus == (s32)TYPUS_C89_QUALIFICATUS)
        {
            t = t->datum.qualificatus.internum;
        }
        si (t->genus != (s32)TYPUS_C89_STRUCTURA
            && t->genus != (s32)TYPUS_C89_UNIO)
        {
            _culpam_addere("-membrum: typus nominatus non est"
                " structura/unio");
            redde FALSUM;
        }
        si (!t->datum.tag.completa)
        {
            perge;   /* declaratio incompleta - membra ignota */
        }
        an->tag_electus = t;
        cum_tag++;
        per (j = ZEPHYRUM; j < t->datum.tag.numerus_membrorum; j++)
        {
            constans TypusC89Membrum* m =
                &t->datum.tag.membra[j];

            si (_chordae_pares_lit(m->titulus, vetus_l))
            {
                vetus_inventum = VERUM;
            }
            si (_chordae_pares_lit(m->titulus, novum_l))
            {
                novum_inventum = VERUM;
            }
        }
    }
    si (cum_tag == ZEPHYRUM)
    {
        _culpam_addere("-membrum: typus in plagulis datis non"
            " inventus (typedef aut tag scopi plagulae)");
        redde FALSUM;
    }
    si (!vetus_inventum)
    {
        _culpam_addere("-membrum: membrum vetus in typo non"
            " exstat");
        redde FALSUM;
    }
    si (novum_inventum)
    {
        _culpam_addere("collisio: membrum novum iam in typo"
            " exstat");
        redde FALSUM;
    }
    redde VERUM;
}

/* registratio scopi plagulae vetus-titulata in analysi una?
 * fructus: numerus; genus_out = genus primae; statica_out */
interior i32
_registrationes_plagulae (constans AnalysisPlagulae* an,
    s32* genus_out, b32* statica_out)
{
    i32 n = (i32)silva_c89_symbola_numerus(an->sem);
    i32 inventae = ZEPHYRUM;
    i32 j;

    per (j = ZEPHYRUM; j < n; j++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(an->sem,
                (insignatus integer)j);

        si (s == NIHIL || s->ex_systemate || s->est_implicitum
            || s->profunditas != ZEPHYRUM)
        {
            perge;
        }
        si (!_chordae_pares_lit(s->titulus, vetus_l))
        {
            perge;
        }
        inventae++;
        si (genus_out != NIHIL && *genus_out < ZEPHYRUM)
        {
            *genus_out = s->genus;
        }
        si (statica_out != NIHIL
            && (s->repositio & REPOSITIO_STATICA))
        {
            *statica_out = VERUM;
        }
    }
    redde inventae;
}

interior b32
_entitatem_resolvere (vacuum)
{
    i32 registrationes = ZEPHYRUM;
    i32 viae_staticae = ZEPHYRUM;
    i32 externae = ZEPHYRUM;
    constans character* via_statica_prima = NIHIL;
    s32 genus_externum = -I;
    i32 k;

    entitas_statica = FALSUM;
    entitas_via_statica = NIHIL;
    entitas_genus = -I;

    /* -via: entitas = registratio plagulae nominatae, cetera
     * automatice aliena fiunt (ligamen discriminat) */
    si (via_data != NIHIL)
    {
        per (k = ZEPHYRUM; k < xar_numerus(analyses); k++)
        {
            AnalysisPlagulae* an = (AnalysisPlagulae*)xar_obtinere(
                analyses, k);

            si (strcmp(an->via, via_data) == ZEPHYRUM)
            {
                s32 genus_viae = -I;
                b32 statica_viae = FALSUM;

                si (_registrationes_plagulae(an, &genus_viae,
                        &statica_viae) == ZEPHYRUM)
                {
                    _culpam_addere("-via: plagula nominata"
                        " registrationem vetus non fert");
                    redde FALSUM;
                }
                entitas_statica = VERUM;
                entitas_via_statica = via_data;
                entitas_genus = genus_viae;
                redde VERUM;
            }
        }
        _culpam_addere("-via plagulam non in lista datam nominat");
        redde FALSUM;
    }

    per (k = ZEPHYRUM; k < xar_numerus(analyses); k++)
    {
        AnalysisPlagulae* an = (AnalysisPlagulae*)xar_obtinere(
            analyses, k);
        s32 genus_viae = -I;
        b32 statica_viae = FALSUM;
        i32 inventae = _registrationes_plagulae(an, &genus_viae,
            &statica_viae);

        si (inventae == ZEPHYRUM)
        {
            perge;
        }
        registrationes += inventae;
        si (statica_viae)
        {
            viae_staticae++;
            si (via_statica_prima == NIHIL)
            {
                via_statica_prima = an->via;
            }
        }
        alioquin
        {
            externae++;
            si (genus_externum < ZEPHYRUM)
            {
                genus_externum = genus_viae;
            }
            alioquin si (genus_externum != genus_viae)
            {
                _culpam_addere("genera registrationum externarum"
                    " discordant (typedef contra functionem etc.)"
                    " - renominatio ambigua");
                redde FALSUM;
            }
        }
    }
    si (registrationes == ZEPHYRUM)
    {
        _culpam_addere("symbolum scopi plagulae non inventum"
            " (vetus ignotum in plagulis datis)");
        redde FALSUM;
    }
    si (viae_staticae > I
        || (viae_staticae == I && externae > ZEPHYRUM))
    {
        _culpam_addere("statica homonyma (aut staticum externo"
            " mixtum) - da -via plagulam certam");
        redde FALSUM;
    }
    si (viae_staticae == I)
    {
        AnalysisPlagulae* an_s = NIHIL;
        s32 genus_viae = -I;
        b32 statica_viae = FALSUM;

        per (k = ZEPHYRUM; k < xar_numerus(analyses); k++)
        {
            AnalysisPlagulae* an = (AnalysisPlagulae*)xar_obtinere(
                analyses, k);

            si (strcmp(an->via, via_statica_prima) == ZEPHYRUM)
            {
                an_s = an;
            }
        }
        si (an_s != NIHIL)
        {
            (vacuum)_registrationes_plagulae(an_s, &genus_viae,
                &statica_viae);
        }
        entitas_statica = VERUM;
        entitas_via_statica = via_statica_prima;
        entitas_genus = genus_viae;
        redde VERUM;
    }
    entitas_genus = genus_externum;
    redde VERUM;
}

/* ==================================================
 * applicatio (splices retro, custodia octetorum)
 * ================================================== */

interior b32
_plagulam_scribere (constans character* via)
{
    /* sedes plagulae huius, offsets descendentes */
    Xar* mei = xar_creare(piscina_magistra,
        (i32)magnitudo(SedesInventa));
    character* fons;
    i32 mensura;
    memoriae_index m_vetus = strlen(vetus_l);
    memoriae_index m_novum = strlen(novum_l);
    i32 k;
    i32 j;

    si (mei == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(sedes_omnes); k++)
    {
        SedesInventa* s = (SedesInventa*)xar_obtinere(sedes_omnes,
            k);

        si ((s->classis == CLASSIS_SEDES
             || s->classis == CLASSIS_USUS)
            && s->offset >= ZEPHYRUM
            && strcmp(s->via, via) == ZEPHYRUM)
        {
            SedesInventa* cella = (SedesInventa*)xar_addere(mei);

            si (cella != NIHIL)
            {
                *cella = *s;
            }
        }
    }
    si (xar_numerus(mei) == ZEPHYRUM)
    {
        redde VERUM;
    }
    /* ordinatio offsets descendentes (insertio simplex) */
    per (k = I; k < xar_numerus(mei); k++)
    {
        per (j = k; j > ZEPHYRUM; j--)
        {
            SedesInventa* a = (SedesInventa*)xar_obtinere(mei,
                j - I);
            SedesInventa* b = (SedesInventa*)xar_obtinere(mei, j);

            si (a->offset < b->offset)
            {
                SedesInventa tmp = *a;

                *a = *b;
                *b = tmp;
            }
        }
    }
    fons = _plagulam_legere(piscina_magistra, via, &mensura);
    si (fons == NIHIL)
    {
        _culpam_addere(_culpa_formata("plagula scribenda"
            " illegibilis", via, ZEPHYRUM, ZEPHYRUM));
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(mei); k++)
    {
        SedesInventa* s = (SedesInventa*)xar_obtinere(mei, k);

        /* custodia octetorum: sedes DEBET vetus scribere */
        si (s->offset + (s32)m_vetus > (s32)mensura
            || memcmp(fons + s->offset, vetus_l, m_vetus)
                != ZEPHYRUM)
        {
            _culpam_addere(_culpa_formata("octeti sedis non"
                " congruunt (offset staluit?)", via, s->linea,
                s->columna));
            redde FALSUM;
        }
    }
    {
        /* aedificatio novi fontis */
        memoriae_index m_nova = (memoriae_index)mensura
            + (memoriae_index)xar_numerus(mei)
                * (m_novum > m_vetus ? m_novum - m_vetus
                                     : ZEPHYRUM)
            + I;
        character* novus_fons = (character*)piscina_allocare(
            piscina_magistra, m_nova);
        s32 cauda = (s32)mensura;
        memoriae_index scriptum = ZEPHYRUM;
        character* cursor;
        FILE* pl;

        si (novus_fons == NIHIL)
        {
            redde FALSUM;
        }
        cursor = novus_fons + m_nova - I;
        *cursor = '\0';
        /* retro: a fine ad initium, sedes descendentes */
        per (k = ZEPHYRUM; k < xar_numerus(mei); k++)
        {
            SedesInventa* s = (SedesInventa*)xar_obtinere(mei, k);
            s32 post = s->offset + (s32)m_vetus;
            memoriae_index m_post = (memoriae_index)(cauda - post);

            cursor -= m_post;
            memcpy(cursor, fons + post, m_post);
            cursor -= m_novum;
            memcpy(cursor, novum_l, m_novum);
            scriptum += m_post + m_novum;
            cauda = s->offset;
        }
        cursor -= cauda;
        memcpy(cursor, fons, (memoriae_index)cauda);
        scriptum += (memoriae_index)cauda;

        pl = fopen(via, "wb");
        si (pl == NIHIL)
        {
            _culpam_addere(_culpa_formata("plagula non"
                " scribibilis", via, ZEPHYRUM, ZEPHYRUM));
            redde FALSUM;
        }
        si (scriptum > ZEPHYRUM
            && fwrite(cursor, I, scriptum, pl) != scriptum)
        {
            fclose(pl);
            _culpam_addere(_culpa_formata("scriptio fracta", via,
                ZEPHYRUM, ZEPHYRUM));
            redde FALSUM;
        }
        fclose(pl);
    }
    redde VERUM;
}

/* ==================================================
 * effusio plani
 * ================================================== */

interior constans character*
_classis_titulus (s32 classis)
{
    commutatio (classis)
    {
        casus CLASSIS_SEDES:    redde "sedes";
        casus CLASSIS_USUS:     redde "usus";
        casus CLASSIS_ALIENA:   redde "aliena";
        casus CLASSIS_MANUALIS: redde "manualis";
        ordinarius:             redde "refusio";
    }
}

interior vacuum
_planum_imprimere (vacuum)
{
    i32 splicendae = ZEPHYRUM;
    i32 manuales = ZEPHYRUM;
    i32 alienae = ZEPHYRUM;
    i32 k;

    si (machina)
    {
        imprimere("#classis\tvia\tlinea\tcolumna\toffset\tnota\n");
    }
    per (k = ZEPHYRUM; k < xar_numerus(sedes_omnes); k++)
    {
        SedesInventa* s = (SedesInventa*)xar_obtinere(sedes_omnes,
            k);

        commutatio (s->classis)
        {
            casus CLASSIS_SEDES:
            casus CLASSIS_USUS:     splicendae++; frange;
            casus CLASSIS_MANUALIS: manuales++;   frange;
            casus CLASSIS_ALIENA:   alienae++;    frange;
            ordinarius: frange;
        }
        si (s->classis == CLASSIS_ALIENA && !machina && !verbosa)
        {
            perge;
        }
        si (machina)
        {
            imprimere("%s\t%s\t%d\t%d\t%ld\t%s\n",
                _classis_titulus(s->classis), s->via,
                (int)s->linea, (int)s->columna, (long)s->offset,
                s->nota != NIHIL ? s->nota : "");
        }
        alioquin
        {
            imprimere("[%s]  %s:%d:%d%s%s\n",
                _classis_titulus(s->classis), s->via,
                (int)s->linea, (int)s->columna,
                s->nota != NIHIL ? "  " : "",
                s->nota != NIHIL ? s->nota : "");
        }
    }
    si (!machina)
    {
        imprimere("summa: splicendae %d, manuales %d, alienae %d\n",
            (int)splicendae, (int)manuales, (int)alienae);
    }
}

/* ==================================================
 * principale
 * ================================================== */

interior vacuum
_usus_imprimere (vacuum)
{
    fprintf(stderr, "usus: renominare <vetus> <novum> [-scribere]"
        " [-machina] [-v] [-via <plagula>] (-lista |"
        " plagulae...)\n");
}

s32
principale (integer argc, character** argv)
{
    Xar* viae = NIHIL;
    b32 ex_stdin = FALSUM;
    integer k;
    i32 j;

    piscina_magistra = piscina_generare_dynamicum(
        "renominare_magistra", 536870912);
    si (piscina_magistra == NIHIL)
    {
        fprintf(stderr, "renominare: piscina deest\n");
        redde II;
    }
    viae = xar_creare(piscina_magistra,
        (i32)magnitudo(constans character*));
    analyses = xar_creare(piscina_magistra,
        (i32)magnitudo(AnalysisPlagulae));
    sedes_omnes = xar_creare(piscina_magistra,
        (i32)magnitudo(SedesInventa));
    culpae = xar_creare(piscina_magistra,
        (i32)magnitudo(constans character*));
    capita_lecta = xar_creare(piscina_magistra,
        (i32)magnitudo(CaputLectum));
    sedes_visae = tabula_dispersa_creare_chorda(piscina_magistra,
        2048);
    culpae_visae = tabula_dispersa_creare_chorda(piscina_magistra,
        LXIV);
    macro_testimonia = xar_creare(piscina_magistra,
        (i32)magnitudo(MacroTestimonium));
    macro_index = tabula_dispersa_creare_chorda(piscina_magistra,
        LXIV);
    si (viae == NIHIL || analyses == NIHIL || sedes_omnes == NIHIL
        || culpae == NIHIL || capita_lecta == NIHIL
        || sedes_visae == NIHIL)
    {
        redde II;
    }

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-scribere") == ZEPHYRUM)
        {
            scribere_modus = VERUM;
        }
        alioquin si (strcmp(argv[k], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[k], "-v") == ZEPHYRUM)
        {
            verbosa = VERUM;
        }
        alioquin si (strcmp(argv[k], "-lista") == ZEPHYRUM)
        {
            ex_stdin = VERUM;
        }
        alioquin si (strcmp(argv[k], "-via") == ZEPHYRUM
            && k + I < argc)
        {
            k++;
            via_data = argv[k];
        }
        alioquin si (strcmp(argv[k], "-intra") == ZEPHYRUM
            && k + I < argc)
        {
            k++;
            intra_l = argv[k];
        }
        alioquin si (strcmp(argv[k], "-membrum") == ZEPHYRUM
            && k + I < argc)
        {
            k++;
            membrum_l = argv[k];
        }
        alioquin si (strcmp(argv[k], "-linea") == ZEPHYRUM
            && k + I < argc)
        {
            int linea_i = ZEPHYRUM;

            k++;
            si (sscanf(argv[k], "%d", &linea_i) == I
                && linea_i > ZEPHYRUM)
            {
                linea_data = (i32)linea_i;
            }
        }
        alioquin si (vetus_l == NIHIL)
        {
            vetus_l = argv[k];
        }
        alioquin si (novum_l == NIHIL)
        {
            novum_l = argv[k];
        }
        alioquin
        {
            constans character** cella =
                (constans character**)xar_addere(viae);

            si (cella != NIHIL)
            {
                *cella = argv[k];
            }
        }
    }
    si (vetus_l == NIHIL || novum_l == NIHIL)
    {
        _usus_imprimere();
        redde II;
    }
    si (ex_stdin)
    {
        character linea[2048];

        dum (fgets(linea, (int)magnitudo(linea), stdin) != NIHIL)
        {
            memoriae_index m = strlen(linea);

            dum (m > ZEPHYRUM && (linea[m - I] == '\n'
                || linea[m - I] == '\r'))
            {
                linea[--m] = '\0';
            }
            si (m > ZEPHYRUM)
            {
                constans character** cella =
                    (constans character**)xar_addere(viae);

                si (cella != NIHIL)
                {
                    *cella = _litterae(_ch(linea));
                }
            }
        }
    }
    si (xar_numerus(viae) == ZEPHYRUM)
    {
        _usus_imprimere();
        redde II;
    }

    /* validatio nominis novi (charset + verbum clavium) */
    si (strcmp(vetus_l, novum_l) == ZEPHYRUM)
    {
        _culpam_addere("vetus et novum idem sunt");
    }
    {
        memoriae_index m = strlen(novum_l);
        memoriae_index i;
        b32 bene = m > ZEPHYRUM;

        per (i = ZEPHYRUM; i < m && bene; i++)
        {
            si (!_littera_identificatoris(novum_l[i],
                    i == ZEPHYRUM))
            {
                bene = FALSUM;
            }
        }
        si (!bene)
        {
            _culpam_addere("nomen novum non est identificator"
                " validus");
        }
    }
    si (_verbum_c89(novum_l))
    {
        _culpam_addere("nomen novum verbum clavium C89 est");
    }

    /* systema semel lectum */
    fons_iso = _plagulam_legere(piscina_magistra,
        "silva/fontes/systema_c89.h", &mensura_iso);
    fons_px = _plagulam_legere(piscina_magistra,
        "silva/fontes/systema_posix.h", &mensura_px);
    si (fons_iso == NIHIL || fons_px == NIHIL)
    {
        fprintf(stderr, "renominare: systema deest (curre ex"
            " radice repositorii)\n");
        redde II;
    }
    _capita_colligere(".");

    /* analysis omnium plagularum (etiam sub culpis praecoces -
     * planum plenum utile est) */
    per (j = ZEPHYRUM; j < xar_numerus(viae); j++)
    {
        constans character* via = *(constans character**)
            xar_obtinere(viae, j);

        si (verbosa)
        {
            fprintf(stderr, "renominare: analysans %s\n", via);
        }
        si (_plagulam_analysare(via) == NIHIL)
        {
            redde recuso_flag ? III : II;
        }
    }
    si (via_data != NIHIL)
    {
        b32 inventa = FALSUM;

        per (j = ZEPHYRUM; j < xar_numerus(analyses); j++)
        {
            AnalysisPlagulae* an = (AnalysisPlagulae*)xar_obtinere(
                analyses, j);

            si (strcmp(an->via, via_data) == ZEPHYRUM)
            {
                inventa = VERUM;
            }
        }
        si (!inventa)
        {
            _culpam_addere("-via plagulam non in lista datam"
                " nominat");
        }
    }

    si (intra_l != NIHIL && membrum_l != NIHIL)
    {
        _culpam_addere("-intra et -membrum simul dari non possunt");
    }
    si (xar_numerus(culpae) == ZEPHYRUM
        && (membrum_l != NIHIL ? _membrum_resolvere()
            : intra_l != NIHIL ? _localem_resolvere()
                               : _entitatem_resolvere()))
    {
        per (j = ZEPHYRUM; j < xar_numerus(analyses); j++)
        {
            AnalysisPlagulae* an = (AnalysisPlagulae*)xar_obtinere(
                analyses, j);

            _plagulam_colligere(an);
            /* modi membri/localis per TU se continent: piscina
             * post collectionem statim destruitur - clausurae
             * reversae magnae (CCXVI plagulae mensuratae) alioqui
             * memoriam trans finem trahunt (Killed: 9).
             * Comparationes monstratorum posteriores (symbolum/
             * tag) valorem solum comparant, numquam dereferunt. */
            si (membrum_l != NIHIL || intra_l != NIHIL)
            {
                piscina_destruere(an->piscina);
                an->piscina = NIHIL;
                an->parsura = NIHIL;
                an->sem = NIHIL;
                an->fons = NIHIL;
            }
        }
        /* v0.1: testimonia macronum post TUs OMNES (probatio
         * promotionis globalis est - invocationes trans plagulas) */
        _testimonia_resolvere();
    }

    _planum_imprimere();

    si (xar_numerus(culpae) > ZEPHYRUM)
    {
        per (j = ZEPHYRUM; j < xar_numerus(culpae); j++)
        {
            constans character* culpa = *(constans character**)
                xar_obtinere(culpae, j);

            si (machina)
            {
                imprimere("CULPA\t%s\n", culpa);
            }
            alioquin
            {
                imprimere("culpa: %s\n", culpa);
            }
        }
        imprimere(machina ? "VERDICTUM\tREFUSIO\n"
                          : "verdictum: REFUSIO (nihil scriptum)\n");
        redde I;
    }

    si (!scribere_modus)
    {
        imprimere(machina ? "VERDICTUM\tPLANUM\n"
                          : "verdictum: PLANUM (da -scribere ut"
                            " applicetur)\n");
        redde ZEPHYRUM;
    }

    /* applicatio */
    {
        Xar* viae_tactae = xar_creare(piscina_magistra,
            (i32)magnitudo(constans character*));
        TabulaDispersa* viae_visae = tabula_dispersa_creare_chorda(
            piscina_magistra, LXIV);

        si (viae_tactae == NIHIL || viae_visae == NIHIL)
        {
            redde II;
        }
        per (j = ZEPHYRUM; j < xar_numerus(sedes_omnes); j++)
        {
            SedesInventa* s = (SedesInventa*)xar_obtinere(
                sedes_omnes, j);

            si ((s->classis == CLASSIS_SEDES
                 || s->classis == CLASSIS_USUS)
                && s->offset >= ZEPHYRUM)
            {
                chorda clavis = _ch(s->via);

                si (!tabula_dispersa_continet(viae_visae, clavis))
                {
                    constans character** cella =
                        (constans character**)xar_addere(
                            viae_tactae);

                    (vacuum)tabula_dispersa_inserere(viae_visae,
                        clavis, NIHIL);
                    si (cella != NIHIL)
                    {
                        *cella = s->via;
                    }
                }
            }
        }
        per (j = ZEPHYRUM; j < xar_numerus(viae_tactae); j++)
        {
            constans character* via = *(constans character**)
                xar_obtinere(viae_tactae, j);

            si (!_plagulam_scribere(via))
            {
                imprimere(machina
                    ? "VERDICTUM\tFRACTUM\n"
                    : "verdictum: FRACTUM in scriptione -"
                      " git checkout -- <plagulae>!\n");
                redde IV;
            }
            si (!machina)
            {
                imprimere("scripta: %s\n", via);
            }
        }

        /* verificatio: plagulae scriptae re-analysatae - vetus
         * evanuit? errores novi? */
        per (j = ZEPHYRUM; j < xar_numerus(viae_tactae); j++)
        {
            constans character* via = *(constans character**)
                xar_obtinere(viae_tactae, j);
            i32 errores_ante = ZEPHYRUM;
            AnalysisPlagulae* nova;
            i32 i;

            per (i = ZEPHYRUM; i < xar_numerus(analyses); i++)
            {
                AnalysisPlagulae* an =
                    (AnalysisPlagulae*)xar_obtinere(analyses, i);

                si (strcmp(an->via, via) == ZEPHYRUM)
                {
                    errores_ante = an->errores;
                }
            }
            nova = _plagulam_analysare(via);
            si (nova == NIHIL
                || (i32)nova->parsura->numerus_errorum
                    > errores_ante)
            {
                si (machina)
                {
                    imprimere("VERDICTUM\tFRACTUM\n");
                }
                alioquin
                {
                    imprimere("verdictum: FRACTUM in verificatione"
                        " (%s) - git checkout -- <plagulae>!\n",
                        via);
                }
                redde IV;
            }
            /* calculus superstitum: lexemata vetus-scribentia
             * strati 0 quae SUPERESSE DEBENT = sedes alienae +
             * sedes in directivis (homonyma consulto manent).
             * Numerus alius = fractura. */
            {
                i32 n = (i32)xar_numerus(
                    nova->parsura->lexemata);
                i32 superstites = ZEPHYRUM;
                i32 exspectati = ZEPHYRUM;
                i32 t;

                per (t = ZEPHYRUM; t < n; t++)
                {
                    SilvaToken** cella = (SilvaToken**)
                        xar_obtinere(
                            nova->parsura->lexemata,
                            (insignatus integer)t);

                    si (cella == NIHIL || *cella == NIHIL)
                    {
                        perge;
                    }
                    si (_tok_vetus_scribit(*cella)
                        && silva_token_est_fons(*cella)
                        && (*cella)->byte_offset >= ZEPHYRUM
                        && (*cella)->fons_index
                            == nova->parsura->fons_princeps)
                    {
                        superstites++;
                    }
                }
                per (t = ZEPHYRUM; t < xar_numerus(sedes_omnes);
                     t++)
                {
                    SedesInventa* s = (SedesInventa*)xar_obtinere(
                        sedes_omnes, t);

                    si (strcmp(s->via, via) != ZEPHYRUM
                        || s->offset < ZEPHYRUM)
                    {
                        perge;
                    }
                    si (s->classis == CLASSIS_ALIENA
                        || (s->classis == CLASSIS_MANUALIS
                            && s->nota != NIHIL
                            && strcmp(s->nota, "in directiva"
                                   " praeprocessoris")
                                == ZEPHYRUM))
                    {
                        exspectati++;
                    }
                }
                si (superstites != exspectati)
                {
                    si (machina)
                    {
                        imprimere("VERDICTUM\tFRACTUM\n");
                    }
                    alioquin
                    {
                        imprimere("verdictum: FRACTUM -"
                            " superstites %d, exspectati %d (%s) -"
                            " git checkout -- <plagulae>!\n",
                            (int)superstites, (int)exspectati,
                            via);
                    }
                    redde IV;
                }
            }
        }
        imprimere(machina ? "VERDICTUM\tSCRIPTUM\n"
                          : "verdictum: SCRIPTUM\n");
    }
    redde ZEPHYRUM;
}
