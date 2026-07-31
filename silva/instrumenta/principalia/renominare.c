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
 *                    [-via <plagula>] (-lista | plagulae...)
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

/* dedup sedium trans TU: clavis "via|offset" */
hic_manens TabulaDispersa* sedes_visae = NIHIL;

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

/* symbolum ligatum entitati nostrae? (discretio staticorum
 * homonymorum - nucleus correctitudinis) */
interior b32
_symbolum_entitatis (constans AnalysisPlagulae* an,
    constans SemanticaSymbolum* s)
{
    si (s == NIHIL || s->ex_systemate || s->profunditas != ZEPHYRUM)
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

                    si (lectio != NIHIL)
                    {
                        GradusAmbulationis* novus =
                            (GradusAmbulationis*)xar_addere(
                                series);

                        si (novus != NIHIL)
                        {
                            novus->v = *lectio;
                            novus->in_membro = in_membro;
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
                    _classem_ponere(classes, tok, CLASSIS_ALIENA);
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
                    == (s32)SILVA_C89_GENUS_DECLARATOR_TITULUS
                && in_membro)
            {
                SilvaToken* tok = _tok_valoris(
                    silva_c89_declarator_titulus_tok_titulus(
                        nodus));

                si (_tok_vetus_scribit(tok))
                {
                    _classem_ponere(classes, tok, CLASSIS_ALIENA);
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

interior b32
_verbum_in_intervallo (constans character* fons, s32 initium,
    s32 finis, s32* sedes_out)
{
    s32 i = initium;
    memoriae_index m = strlen(vetus_l);

    si (initium < ZEPHYRUM || finis <= initium)
    {
        redde FALSUM;
    }
    dum (i + (s32)m <= finis)
    {
        si (memcmp(fons + i, vetus_l, m) == ZEPHYRUM)
        {
            b32 ante_bene = (i == ZEPHYRUM
                || !_littera_identificatoris(fons[i - I], FALSUM));
            b32 post_bene = (i + (s32)m >= finis
                || !_littera_identificatoris(fons[i + (s32)m],
                       FALSUM));

            si (ante_bene && post_bene)
            {
                *sedes_out = i;
                redde VERUM;
            }
        }
        i++;
    }
    redde FALSUM;
}

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
 * collectio plagulae: classes -> sedes + rationarium
 * ================================================== */

interior constans character*
_via_lexematis (constans AnalysisPlagulae* an, SilvaToken* tok)
{
    constans chorda* via = silva_fons_via(an->parsura->expansio,
        tok->fons_index);

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
            /* expansio: sedes invocationis manualis nominatur */
            SilvaToken* radix = silva_token_radix(tok);

            si (radix != NIHIL && radix->byte_offset >= ZEPHYRUM)
            {
                constans character* via_r = _via_lexematis(an,
                    radix);

                si (via_r != NIHIL)
                {
                    _sedem_addere(CLASSIS_MANUALIS, via_r,
                        radix->linea, radix->columna,
                        radix->byte_offset,
                        "per expansionem macronis");
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

    /* corpora macronum: scansio textualis (lexemata corporum in
     * flumine absunt - definitio acta est, non parsata) */
    n = silva_macros_numerus(an->parsura->expansio);
    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaMacroVista vista;

        si (!silva_macro_vista(an->parsura->expansio, k, &vista))
        {
            perge;
        }
        si (vista.corpus_initium < ZEPHYRUM
            || vista.fons_index
                != an->parsura->fons_princeps)
        {
            perge;   /* corpora capitum: TU capitis ea colligit */
        }
        {
            s32 sedes_b;

            si (_verbum_in_intervallo(an->fons,
                    vista.corpus_initium, vista.corpus_finis,
                    &sedes_b))
            {
                character nota_l[CCLVI];
                int scripti = sprintf(nota_l,
                    "in corpore macronis %.*s",
                    (int)(vista.titulus->mensura < CC
                        ? vista.titulus->mensura : CC),
                    (constans character*)vista.titulus->datum);

                _sedem_addere(CLASSIS_MANUALIS, an->via,
                    vista.linea, I, sedes_b,
                    scripti > ZEPHYRUM
                        ? _litterae(_ch(nota_l))
                        : "in corpore macronis");
            }
        }
    }

    /* rami omissi: scansio textualis laminarum non sumptarum */
    n = silva_rami_numerus(an->parsura->expansio);
    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaRamusVista vista;

        si (!silva_ramus_vista(an->parsura->expansio, k, &vista))
        {
            perge;
        }
        si (vista.est_sumptum || vista.fons_index
                != an->parsura->fons_princeps
            || vista.corpus_initium < ZEPHYRUM)
        {
            perge;
        }
        {
            s32 sedes_b;

            si (_verbum_in_intervallo(an->fons,
                    vista.corpus_initium, vista.corpus_finis,
                    &sedes_b))
            {
                _sedem_addere(CLASSIS_MANUALIS, an->via,
                    vista.linea, I, sedes_b,
                    "in ramo praeprocessoris omisso");
            }
        }
    }

    /* porta collisionis: registratio quaevis 'novum' titulata in
     * plagula affecta */
    n = (i32)silva_c89_symbola_numerus(an->sem);
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

    si (_entitatem_resolvere())
    {
        per (j = ZEPHYRUM; j < xar_numerus(analyses); j++)
        {
            _plagulam_colligere((AnalysisPlagulae*)xar_obtinere(
                analyses, j));
        }
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
                    errores_ante =
                        (i32)an->parsura->numerus_errorum;
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
