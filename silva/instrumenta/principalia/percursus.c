/* percursus.c - PERCURSUS REPOSITORII (M2d Chunk C): omnem
 * plagulam .c/.h monorepositorii per fistulam plenam (contextus
 * latinus + grammatica c89 + saltatio) parsare et censum referre:
 *
 *   - totalitas: omnis plagula arborem dat (norma tree-sitter)
 *   - fidelitas: scriptura == fons octetim (oraculum implacabile)
 *   - nodi ERROR per plagulam (metrum comprehensionis M2d)
 *   - NUMERATIONES EVIDENTIAE (portae decisionum nominatarum):
 *       typedef intra corpora functionum (decisiones 13)
 *       CASE intra nodos ERROR (machina Duffii, pretium gregum)
 *       identificatores "__" intra ERROR (extensiones gcc)
 *
 * Instrumentum dev-time (non probatio - percursus 38MB minuta
 * capit); numeri in phase-log / mensura historia referuntur.
 * Usus: ./silva/percursus.sh [radix] [-v]
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_scribere.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include "silva_lexicon.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens b32 verbosa = FALSUM;
hic_manens i32 mensura_maxima = 4194304;  /* -omnia = sine tecto */
hic_manens i32 praetermissae = ZEPHYRUM;

hic_manens i32 plagulae = ZEPHYRUM;
hic_manens i32 arbores = ZEPHYRUM;
hic_manens i32 fideles = ZEPHYRUM;
hic_manens i32 summa_errorum = ZEPHYRUM;
hic_manens i32 plagulae_cum_erroribus = ZEPHYRUM;
hic_manens i32 typedef_in_corpore = ZEPHYRUM;
hic_manens i32 duffiana = ZEPHYRUM;
hic_manens i32 gcc_in_errore = ZEPHYRUM;
hic_manens duplex summa_octetorum = 0.0;
hic_manens duplex summa_ms = 0.0;
hic_manens duplex apex_maximus = 0.0;
hic_manens character plagula_apicis[1024];
hic_manens i32 capita_praebita = ZEPHYRUM;
hic_manens i32 capita_collisiones = ZEPHYRUM;
hic_manens i32 fines_tactae_plagulae = ZEPHYRUM;
hic_manens i32 summa_ambiguorum = ZEPHYRUM;
hic_manens i32 plagulae_cum_ambiguis = ZEPHYRUM;

/* -semantica (M0a Chunk B; DEFALTUM DEMPTUM - basis pristina) */
hic_manens b32 cum_semantica = FALSUM;
hic_manens i32 summa_symbolorum = ZEPHYRUM;
hic_manens i32 summa_diagnosticorum = ZEPHYRUM;
hic_manens i32 census_codicum[EXAMEN_CODEX_NUMERUS];
hic_manens i32 verdicta_reice = ZEPHYRUM;
hic_manens i32 summa_typationum = ZEPHYRUM;      /* M0b */
hic_manens i32 summa_expr_visorum = ZEPHYRUM;    /* M0b C: coopertura */
hic_manens i32 summa_expr_typatorum = ZEPHYRUM;
hic_manens i32 plagula_expr_visa = ZEPHYRUM;     /* per plagulam */
hic_manens i32 plagula_expr_typata = ZEPHYRUM;
hic_manens i32 plagulae_cum_diagnosticis = ZEPHYRUM;
/* Chunk C -> DESIGN B (2026-07-27): systema non iam UNUM semel
 * parsatum, sed praeparatio (ctx + parsura systematis + semantica)
 * per SIGNATURAM - textus systematis compositus (ISO + sectiones
 * POSIX ex inclusionibus plagulae derivatae + bloci externa
 * plagulae; silva_lexicon_componere, EADEM compositio quam examen
 * per plagulam facit) clavis cache est. Plagulae sine POSIX ad ISO
 * purum componunt ergo praeparationem UNAM communicant; typi
 * cuiusque praeparationis trans plagulas vivunt (piscina
 * praeparationis longaeva - lex 'systema semel parsatum' vetus,
 * nunc per praeparationem). */
nomen structura {
    SilvaContextus* ctx;
      SilvaParsura* parsura;
    SilvaSemantica* semantica;
           Piscina* piscina;
} PraeparatioLexici;

/* caput semel lectum (praebetur deinde in contextus plures) */
nomen structura {
    constans character* via;   /* via plena - clavis praebitionis */
             character* textus;
                   i32  mensura;
} CaputLectum;

hic_manens Xar* capita_lecta = NIHIL;
hic_manens TabulaDispersa* praeparationes_tabula = NIHIL;
hic_manens Xar* praeparationes_index = NIHIL;  /* pro summa */
hic_manens SilvaContextus* ctx_basis = NIHIL;  /* sine -semantica */
hic_manens character* fons_iso = NIHIL;
hic_manens i32 mensura_iso = ZEPHYRUM;
hic_manens character* fons_posix = NIHIL;
hic_manens i32 mensura_posix = ZEPHYRUM;
hic_manens i32 plagulae_infra = ZEPHYRUM;
hic_manens i32 summa_versorum = ZEPHYRUM;
hic_manens i32 summa_indecisorum = ZEPHYRUM;

hic_manens b32
_praetermittendum (
    constans character* titulus);

/* typedef intra corpus definitionis (ambulatio generica: nodi
 * definitio-functionis -> subarbor corporis -> declarationes cum
 * lexemate TYPEDEF in specificatoribus) */
hic_manens vacuum
_typedefs_numerare (
            SilvaValor  valor,
                   b32  intra_corpus,
    constans character* via,
                   i32  profunditas)
{
    si (profunditas > LXIV) redde;

    si (valor.genus == SILVA_VALOR_LISTA)
    {
        i32 i;

        per (i = ZEPHYRUM;
             i < silva_valor_lista_numerus(valor); i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            si (elem != NIHIL)
            {
                _typedefs_numerare(*elem, intra_corpus, via,
                    profunditas + I);
            }
        }
        redde;
    }
    si (   valor.genus       != SILVA_VALOR_NODUS
        || valor.datum.nodus == NIHIL)
    {
        redde;
    }

    {
        constans SilvaNodus* nodus = valor.datum.nodus;
                        i32  i;

        si (   intra_corpus
            && nodus->genus == (s32)SILVA_C89_GENUS_DECLARATIO)
        {
            SilvaValor specificatores =
                silva_c89_declaratio_specificatores(nodus);
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(specificatores); k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(
                    specificatores, k);

                si (   elem              != NIHIL
                    && elem->genus       == SILVA_VALOR_TOKEN
                    && elem->datum.token != NIHIL
                    && elem->datum.token->genus
                        == SILVA_LEX_TYPEDEF)
                {
                    typedef_in_corpore++;
                    imprimere("  [typedef in corpore] %s:%u\n",
                        via, elem->datum.token->linea);
                }
            }
        }
        si (nodus->genus
            == (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
        {
            intra_corpus = VERUM;
        }
        per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
        {
            _typedefs_numerare(nodus->loci[i], intra_corpus, via,
                profunditas + I);
        }
    }
}

/* M0b C: genera expressionum quae tabula typationum tenere debet
 * (parenthesis + congeries EXCLUSA - positio contextualis) */
hic_manens b32
_est_genus_expressionis (
    s32 genus)
{
    commutatio (genus)
    {
        casus (s32)SILVA_C89_GENUS_VIRGULA:
        casus (s32)SILVA_C89_GENUS_ASSIGNATIO:
        casus (s32)SILVA_C89_GENUS_TERNARIUS:
        casus (s32)SILVA_C89_GENUS_BINARIUM:
        casus (s32)SILVA_C89_GENUS_CONVERSIO:
        casus (s32)SILVA_C89_GENUS_UNARIUM:
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS:
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI:
        casus (s32)SILVA_C89_GENUS_SUBSCRIPTIO:
        casus (s32)SILVA_C89_GENUS_VOCATIO:
        casus (s32)SILVA_C89_GENUS_ACCESSUS:
        casus (s32)SILVA_C89_GENUS_POSTCREMENTUM:
        casus (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR:
        casus (s32)SILVA_C89_GENUS_FOLIUM_INTEGER:
        casus (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS:
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER:
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHORDA:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* Coopertura typationis: nodi expressionum visitati/typati.
 * AMBIGUUS -> lectio canonica SOLA (aliae numquam typantur);
 * RAMUS_OMISSUS (cruda) + ERROR praetermissi. Post clausuram
 * mensurata - lacuna canonicae-relativae (INTENTIO DECISUS 2)
 * hic apparet donec Chunk D bis analysat. */
hic_manens vacuum
_expressiones_numerare (
    constans SilvaSemantica* sem,
                 SilvaValor  valor,
                        i32  profunditas)
{
    constans SilvaNodus* nodus;
                    i32  i;

    si (profunditas > CXXVIII)
    {
        redde;
    }
    si (valor.genus == SILVA_VALOR_LISTA)
    {
        per (i = ZEPHYRUM;
             i < silva_valor_lista_numerus(valor); i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            si (elem != NIHIL)
            {
                _expressiones_numerare(sem, *elem, profunditas + I);
            }
        }
        redde;
    }
    si (   valor.genus       != SILVA_VALOR_NODUS
        || valor.datum.nodus == NIHIL)
    {
        redde;
    }
    nodus = valor.datum.nodus;
    si (nodus->genus == (s32)SILVA_C89_GENUS_AMBIGUUS)
    {
        SilvaValor interp = silva_c89_ambiguus_interpretationes(
            nodus);
        SilvaValor canonica = silva_c89_ambiguus_canonica(nodus);

        si (canonica.genus == SILVA_VALOR_INDEX)
        {
            SilvaValor* lectio = silva_valor_lista_obtinere(interp,
                (i32)canonica.datum.index);

            si (lectio != NIHIL)
            {
                _expressiones_numerare(sem, *lectio,
                    profunditas + I);
            }
        }
        redde;
    }
    si (   nodus->genus == (s32)SILVA_C89_GENUS_RAMUS_OMISSUS
        || nodus->genus == (s32)SILVA_C89_GENUS_ERROR)
    {
        redde;
    }
    si (_est_genus_expressionis(nodus->genus))
    {
        plagula_expr_visa++;
        si (silva_c89_typus_expressionis(sem, nodus) != NIHIL)
        {
            plagula_expr_typata++;
        }
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        _expressiones_numerare(sem, nodus->loci[i], profunditas + I);
    }
}

/* Lexemata nodorum ERROR inspicere: CASE (Duffii) + "__" (gcc) */
hic_manens vacuum
_errores_inspicere (
            SilvaValor  valor,
    constans character* via,
                   i32  profunditas)
{
    si (profunditas > LXIV) redde;

    si (valor.genus == SILVA_VALOR_LISTA)
    {
        i32 i;

        per (i = ZEPHYRUM;
             i < silva_valor_lista_numerus(valor); i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            si (elem != NIHIL)
            {
                _errores_inspicere(*elem, via, profunditas + I);
            }
        }
        redde;
    }
    si (   valor.genus       != SILVA_VALOR_NODUS
        || valor.datum.nodus == NIHIL)
    {
        redde;
    }

    {
        constans SilvaNodus* nodus = valor.datum.nodus;
                        i32  i;

        si (nodus->genus == (s32)SILVA_C89_GENUS_ERROR)
        {
            per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
            {
                SilvaValor lexemata = nodus->loci[i];
                       i32 k;

                si (lexemata.genus != SILVA_VALOR_LISTA) perge;
                per (k = ZEPHYRUM;
                     k < silva_valor_lista_numerus(lexemata); k++)
                {
                    SilvaValor* elem = silva_valor_lista_obtinere(
                        lexemata, k);
                    SilvaToken* t;

                    si (   elem        == NIHIL
                        || elem->genus != SILVA_VALOR_TOKEN)
                    {
                        perge;
                    }
                    t = elem->datum.token;
                    si (t == NIHIL) perge;
                    si (t->genus == SILVA_LEX_CASE)
                    {
                        duffiana++;
                        imprimere("  [casus in errore] %s:%u\n",
                            via, t->linea);
                    }
                    si (   t->genus == SILVA_LEX_IDENTIFICATOR
                        && t->valor.mensura >= II
                        && t->valor.datum[ZEPHYRUM] == '_'
                        && t->valor.datum[I] == '_')
                    {
                        gcc_in_errore++;
                        si (verbosa)
                        {
                            imprimere("  [gcc in errore] %s:%u "
                                "%.*s\n", via, t->linea,
                                (int)t->valor.mensura,
                                (constans character*)
                                    t->valor.datum);
                        }
                    }
                }
            }
            redde;  /* intra errorem non descendimus ultra */
        }
        per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
        {
            _errores_inspicere(nodus->loci[i], via,
                profunditas + I);
        }
    }
}

/* Plagulam totam legere: textus in piscina, '\0' additum (pro
 * capitibus ET fontibus systematis) */
hic_manens character*
_plagulam_totam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_out)
{
    FILE* pl;
    long mensura_l;
    i32 mensura;
    character* textus;

    *mensura_out  = ZEPHYRUM;
    pl            = fopen(via, "rb");
    si (pl == NIHIL) redde NIHIL;
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = (i32)mensura_l;
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + I));
    si (   textus == NIHIL || (mensura > ZEPHYRUM
        && fread(textus, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    textus[mensura]  = '\0';
    *mensura_out     = mensura;
    redde textus;
}

/* EXPANSIO PER INCLUSIONEM VERAM (M2d Chunk D): praepassus omne
 * caput (.h) repositorii sub VIA PLENA COLLIGIT (01KYJ6740K) -
 * textus SEMEL lectus (piscina longaeva), deinde in contextus
 * praebetur (_capita_praebere_in); expansor clavem canonicam +
 * basename seponit (primus vincit) et inclusiones citatas
 * includenti-relative primum resolvit. DESIGN B: contextus plures
 * (praeparatio per signaturam), textus capitum communes. Quaeque
 * plagula suam catenam inclusionum VERAM sequitur (transitive,
 * custodes honorati) - plagulae latina.h includentes latinam
 * accipiunt, ceterae (hospes canariae! knotapel! raqiya) lexica
 * SUA VERA, et gemini basename (fontes_generata.h!) vicinos SUOS
 * capiunt, non alienos. */
hic_manens vacuum
_caput_legere (
               Piscina* piscina,
        TabulaDispersa* visa,
    constans character* via)
{
            chorda  clavis;
         character* textus;
         character* via_copia;
               i32  mensura = ZEPHYRUM;
    memoriae_index  m;
       CaputLectum* introitus;

    clavis = chorda_ex_literis(via, piscina);
    si (tabula_dispersa_continet(visa, clavis))
    {
        capita_collisiones++;
        si (verbosa)
        {
            imprimere("  [collisio capitis] %s\n", via);
        }
        redde;
    }
    textus = _plagulam_totam_legere(piscina, via, &mensura);
    si (textus == NIHIL) redde;
    /* via_plena effimera est - copianda (vita = piscina) */
    m          = strlen(via);
    via_copia  = (character*)piscina_allocare(piscina, m + I);
    si (via_copia == NIHIL) redde;
    memcpy(via_copia, via, m + I);
    introitus = (CaputLectum*)xar_addere(capita_lecta);
    si (introitus == NIHIL) redde;
    introitus->via      = via_copia;
    introitus->textus   = textus;
    introitus->mensura  = mensura;
    (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);
    capita_praebita++;
}

hic_manens vacuum
_capita_praeparare (
               Piscina* piscina,
        TabulaDispersa* visa,
    constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL) redde;
    dum ((introitus = readdir(dir)) != NIHIL)
    {
             character via_plena[1024];
        memoriae_index m;

        si (introitus->d_name[ZEPHYRUM] == '.') perge;
        si (_praetermittendum(introitus->d_name)) perge;
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);

        si (introitus->d_type == DT_DIR)
        {
            _capita_praeparare(piscina, visa, via_plena);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (   m >= III && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I] == 'h')
            {
                _caput_legere(piscina, visa, via_plena);
            }
        }
    }
    closedir(dir);
}

/* Capita collecta in contextum praebere (basis aut praeparatio) */
hic_manens vacuum
_capita_praebere_in (
    SilvaContextus* ctx)
{
    i32 n = xar_numerus(capita_lecta);
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        constans CaputLectum* cl = (constans CaputLectum*)
            xar_obtinere(capita_lecta, i);

        si (cl != NIHIL)
        {
            (vacuum)silva_contextus_praebere(ctx, cl->via,
                cl->textus, cl->mensura);
        }
    }
}

/* Praeparationem pro textu systematis composito capere: cache
 * clave CONTENTO - plagulae sine POSIX ad ISO purum componunt ergo
 * praeparationem unam communicant, sine casu speciali. Fractura
 * quaevis NIHIL reddit (vocator plagulam INFRA nominat, verdictum
 * mundum numquam fingit); piscina praeparationis in fractura
 * destruitur. */
hic_manens PraeparatioLexici*
_praeparationem_capere (
    character* compositum,
          i32  mensura_comp)
{
               chorda  clavis;
               vacuum* valor = NIHIL;
              Piscina* pp;
    PraeparatioLexici* praep;
            character* copia;

    clavis.mensura  = mensura_comp;
    clavis.datum    = (i8*)compositum;
    si (tabula_dispersa_invenire(praeparationes_tabula, clavis,
            &valor))
    {
        redde (PraeparatioLexici*)valor;
    }

    pp = piscina_generare_dynamicum("percursus_praep", 8388608);
    si (pp == NIHIL) redde NIHIL;
    praep = (PraeparatioLexici*)piscina_allocare(pp,
        (memoriae_index)magnitudo(PraeparatioLexici));
    copia = (character*)piscina_allocare(pp,
        (memoriae_index)(mensura_comp + I));
    si (praep == NIHIL || copia == NIHIL)
    {
        piscina_destruere(pp);
        redde NIHIL;
    }
    memcpy(copia, compositum, (memoriae_index)mensura_comp);
    copia[mensura_comp]  = '\0';
    praep->piscina       = pp;
    praep->ctx           = silva_contextus_creare(pp);
    si (praep->ctx == NIHIL)
    {
        piscina_destruere(pp);
        redde NIHIL;
    }
    si (mensura_maxima == ZEPHYRUM)
    {
        /* -omnia: fines ut in contextu basis */
        praep->ctx->fines.lexemata = ZEPHYRUM;
    }
    si (!silva_contextus_lexicon_addere(praep->ctx,
            "systema_c89.h", copia, mensura_comp))
    {
        piscina_destruere(pp);
        redde NIHIL;
    }
    _capita_praebere_in(praep->ctx);
    praep->parsura = silva_c89_parsare(pp, "systema_c89.h", copia,
        mensura_comp, NIHIL);
    si (   praep->parsura == NIHIL
        || praep->parsura->numerus_errorum > ZEPHYRUM)
    {
        piscina_destruere(pp);
        redde NIHIL;
    }
    praep->semantica = silva_c89_semantica_analysare(pp,
        praep->parsura);
    si (praep->semantica == NIHIL)
    {
        piscina_destruere(pp);
        redde NIHIL;
    }
    /* clavis in copiam piscinae praeparationis monstrat (vita
     * = vita cache) */
    clavis.datum = (i8*)copia;
    si (!tabula_dispersa_inserere(praeparationes_tabula, clavis,
            praep))
    {
        piscina_destruere(pp);
        redde NIHIL;
    }
    {
        PraeparatioLexici** situs = (PraeparatioLexici**)
            xar_addere(praeparationes_index);

        si (situs != NIHIL)
        {
            *situs = praep;
        }
    }
    redde praep;
}

hic_manens vacuum
_plagulam_percurrere (
    constans character* via)
{
    Piscina* piscina;
    FILE* pl;
    i8* fons;
    long mensura_l;
    i32 mensura;
    SilvaParsura* parsura;
    SilvaOraculum* oraculum_clausurae = NIHIL;
    PraeparatioLexici* praep = NIHIL;
    clock_t c0;
    clock_t c1;

    pl = fopen(via, "rb");
    si (pl == NIHIL) redde;
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde;
    }
    mensura = (i32)mensura_l;

    /* TECTUM MENSURAE (inventum M2d C): plagulae datorum densorum
     * (biblia_dr.c 6MB) arenam ad 49 GB inflant - multiplicator
     * ~8000x, pathologia NOMINATA (investigatio propria; lustrum
     * telemetria per subsystema). Praetermissio ALTA voce. */
    si (mensura_maxima > ZEPHYRUM && mensura > mensura_maxima)
    {
        imprimere("[PRAETERMISSA mensura %.1f MB] %s\n",
            (duplex)mensura / 1048576.0, via);
        praetermissae++;
        fclose(pl);
        redde;
    }

    piscina = piscina_generare_dynamicum("percursus", 8388608);
    si (piscina == NIHIL)
    {
        fclose(pl);
        redde;
    }
    fons = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (   fons == NIHIL || (mensura > ZEPHYRUM
        && fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        piscina_destruere(piscina);
        redde;
    }
    fclose(pl);

    /* DESIGN B: praeparatio ANTE numerationem - fractura plagulam
     * INFRA facit (nominata alta voce, non numerata), verdictum
     * mundum numquam fingit (contractus examinis) */
    si (cum_semantica)
    {
        character* compositum;
              i32  mensura_comp  = ZEPHYRUM;
              b32  fractum       = FALSUM;

        compositum = silva_lexicon_componere(fons_iso, mensura_iso,
            fons_posix, mensura_posix, (constans character*)fons,
            mensura, FALSUM, piscina, &mensura_comp, via, &fractum);
        si (fractum)
        {
            imprimere("[externa fracta] %s\n", via);
            plagulae_infra++;
            piscina_destruere(piscina);
            redde;
        }
        si (compositum == NIHIL)
        {
            imprimere("[compositio fracta] %s\n", via);
            plagulae_infra++;
            piscina_destruere(piscina);
            redde;
        }
        praep = _praeparationem_capere(compositum, mensura_comp);
        si (praep == NIHIL)
        {
            imprimere("[praeparatio fracta] %s\n", via);
            plagulae_infra++;
            piscina_destruere(piscina);
            redde;
        }
    }

    plagulae++;
    summa_octetorum += (duplex)mensura;

    {
                  SilvaOraculum* oraculum_plagulae = NIHIL;
        constans SilvaContextus* ctx = cum_semantica
            ? praep->ctx : ctx_basis;

        si (cum_semantica)
        {
            oraculum_plagulae = silva_oraculum_creare(piscina);
            si (   oraculum_plagulae != NIHIL
                && praep->semantica  != NIHIL)
            {
                (vacuum)silva_c89_semantica_oraculum_augere(
                    praep->semantica, oraculum_plagulae);
            }
        }
        c0 = clock();
        parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
            (constans character*)fons, mensura, oraculum_plagulae);
        c1                  = clock();
        oraculum_clausurae  = oraculum_plagulae;
    }
    summa_ms += (duplex)(c1 - c0) * 1000.0
        / (duplex)CLOCKS_PER_SEC;

    si (parsura != NIHIL && parsura->successus)
    {
        SilvaScriptura scriptura;

        arbores++;
        /* degradatio picta, non silens (praetermissio ALTA voce):
         * fines_tactae = limen aliquod tactum, expansio decisa */
        si (parsura->fines_tactae)
        {
            fines_tactae_plagulae++;
            imprimere("[fines tactae] %s\n", via);
        }
        si (parsura->numerus_errorum > ZEPHYRUM)
        {
            plagulae_cum_erroribus++;
            summa_errorum += (i32)parsura->numerus_errorum;
            imprimere("[errores %d] %s\n",
                (int)parsura->numerus_errorum, via);
            _errores_inspicere(parsura->commissio->radix, via,
                ZEPHYRUM);
        }
        _typedefs_numerare(parsura->commissio->radix, FALSUM, via,
            ZEPHYRUM);
        {
            i32 amb = xar_numerus(parsura->commissio->ambigui);

            si (amb > ZEPHYRUM)
            {
                summa_ambiguorum += amb;
                plagulae_cum_ambiguis++;
                imprimere("[ambigui %d] %s\n", (int)amb, via);
            }
        }
        si (cum_semantica)
        {
            /* BIS-ANALYSIS (M0b D, INTENTIO DECISUS 2): typatio
             * RELATIVA CANONICAE est. sem1 = pabulum clausurae
             * (annotationes eius abiciuntur); post recanonicare
             * sem2 AUCTORITATIVA contra canonicas versas - eius
             * numeri referuntur, eius tabula quaeritur. */
            SilvaSemantica* sem =
                silva_c89_semantica_analysare_cum_systemate(
                    piscina, parsura, praep->parsura);

            si (sem != NIHIL)
            {
                /* clausura: oraculum auctum symbolis plagulae
                 * (localibus!) -> recanonicare + residuum */
                si (oraculum_clausurae != NIHIL)
                {
                    i32 versae;
                    i32 indecisa;

                    (vacuum)silva_c89_semantica_oraculum_augere(
                        sem, oraculum_clausurae);
                    silva_oraculum_responsa_vacare(
                        oraculum_clausurae);
                    versae = (i32)silva_recanonicare(
                        parsura->commissio, oraculum_clausurae,
                        silva_c89_resolutor, NIHIL);
                    indecisa = (i32)
                        silva_c89_ambigua_indecisa_numerare(
                            parsura->commissio, oraculum_clausurae);
                    summa_versorum     += versae;
                    summa_indecisorum  += indecisa;
                    si (indecisa > ZEPHYRUM)
                    {
                        imprimere("[indecisa %d] %s\n",
                            (int)indecisa, via);
                    }
                    /* analysi secunda contra arborem versam */
                    sem = silva_c89_semantica_analysare_cum_systemate(
                        piscina, parsura, praep->parsura);
                }
            }
            si (sem != NIHIL)
            {
                i32 diag = xar_numerus(sem->diagnostica);

                summa_symbolorum += (i32)xar_numerus(sem->symbola);
                summa_typationum +=
                    silva_c89_typationes_numerus(sem);
                si (diag > ZEPHYRUM)
                {
                    i32 di;
                    b32 reice = parsura->numerus_errorum
                        > ZEPHYRUM;

                    summa_diagnosticorum += diag;
                    plagulae_cum_diagnosticis++;
                    imprimere("[semantica diagnostica %d] %s\n",
                        (int)diag, via);
                    per (di = ZEPHYRUM; di < diag; di++)
                    {
                        constans SemanticaDiagnosticum* dg =
                            silva_c89_diagnosticum_per_indicem(sem,
                                di);

                        si (   dg != NIHIL && dg->codex >= ZEPHYRUM
                            && dg->codex
                                < (s32)EXAMEN_CODEX_NUMERUS)
                        {
                            census_codicum[dg->codex]++;
                            si (   dg->severitas
                                    == (s32)EXAMEN_VIOLATIO
                                && !dg->provisionale)
                            {
                                reice = VERUM;
                            }
                        }
                    }
                    /* columna verdicti (examen chunk D,
                     * DECISUS 12) */
                    si (reice)
                    {
                        verdicta_reice++;
                        imprimere("[verdictum REICE] %s\n", via);
                    }
                }
                /* coopertura POST clausuram contra sem2 - lacuna
                 * canonicae CLAUSA esse debet (proba acceptionis) */
                plagula_expr_visa    = ZEPHYRUM;
                plagula_expr_typata  = ZEPHYRUM;
                _expressiones_numerare(sem,
                    parsura->commissio->radix, ZEPHYRUM);
                summa_expr_visorum    += plagula_expr_visa;
                summa_expr_typatorum  += plagula_expr_typata;
                si (plagula_expr_typata < plagula_expr_visa)
                {
                    imprimere("[expr coopertura %d/%d] %s\n",
                        (int)plagula_expr_typata,
                        (int)plagula_expr_visa, via);
                }
            }
            alioquin
            {
                imprimere("[SEMANTICA FRACTA] %s\n", via);
            }
        }

        scriptura = silva_scribere_fontem(piscina, parsura,
            &SILVA_C89_REGISTRUM, parsura->fons_princeps);
        si (   scriptura.successus
            && scriptura.textus.mensura == mensura
            && (mensura == ZEPHYRUM
                || memcmp(scriptura.textus.datum, fons,
                       (memoriae_index)mensura) == ZEPHYRUM))
        {
            fideles++;
        }
        alioquin si (!scriptura.successus)
        {
            /* Fractura CLARA (deferral nominatum, e.g. pasta) -
             * non divergentia octetim; discrimen refert (M2d C:
             * arbor2_glr_tabula.c "INFIDELIS" investigationem
             * integram consumpsit quae vere deferral notum erat) */
            imprimere("[SCRIPTURA FRACTA: %s] %s\n",
                scriptura.causa ? scriptura.causa : "?", via);
        }
        alioquin
        {
            imprimere("[INFIDELIS octetim] %s\n", via);
        }
    }
    alioquin
    {
        imprimere("[SINE ARBORE] %s\n", via);
    }

    {
        duplex apex = (duplex)piscina_summa_apex_usus(piscina);

        si (apex > apex_maximus)
        {
            apex_maximus = apex;
            strncpy(plagula_apicis, via,
                magnitudo(plagula_apicis) - I);
            plagula_apicis[magnitudo(plagula_apicis) - I] = '\0';
        }
    }
    si (verbosa)
    {
        imprimere("  %s: %d octeti\n", via, (int)mensura);
    }
    piscina_destruere(piscina);
}

hic_manens b32
_est_fons_c (
    constans character* titulus)
{
    memoriae_index m = strlen(titulus);

    si (m < III) redde FALSUM;
    si (titulus[m - II] != '.') redde FALSUM;
    redde (titulus[m - I] == 'c' || titulus[m - I] == 'h')
        ? VERUM : FALSUM;
}

hic_manens b32
_praetermittendum (
    constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

hic_manens vacuum
_directorium_percurrere (
    constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL) redde;
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        character via_plena[1024];

        si (introitus->d_name[ZEPHYRUM] == '.') perge;
        si (_praetermittendum(introitus->d_name)) perge;
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);

        si (introitus->d_type == DT_DIR)
        {
            _directorium_percurrere(via_plena);
        }
        alioquin si (_est_fons_c(introitus->d_name))
        {
            _plagulam_percurrere(via_plena);
        }
    }
    closedir(dir);
}

s32
principale (
      integer   argc,
    character** argv)
{
               Piscina* piscina_ctx;
    constans character* radix = ".";
               integer  k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-v") == ZEPHYRUM)
        {
            verbosa = VERUM;
        }
        alioquin si (strcmp(argv[k], "-omnia") == ZEPHYRUM)
        {
            mensura_maxima = ZEPHYRUM;  /* sine tecto - cave! */
        }
        alioquin si (strcmp(argv[k], "-semantica") == ZEPHYRUM)
        {
            cum_semantica = VERUM;
        }
        alioquin
        {
            radix = argv[k];
        }
    }

    piscina_ctx = piscina_generare_dynamicum("percursus_ctx",
        8388608);
    si (piscina_ctx == NIHIL)
    {
        fprintf(stderr, "percursus: piscina deest\n");
        redde I;
    }
    plagula_apicis[ZEPHYRUM] = '\0';

    /* DESIGN B (2026-07-27): fontes systematis SEMEL lecti;
     * praeparationes (ctx + parsura + semantica) per plagulam ex
     * textu composito cache-antur (_praeparationem_capere) - eadem
     * compositio quam examen per plagulam facit. Macra systematis
     * (NULL, EOF, INT_MAX...) per canalem LEXICI cuiusque
     * praeparationis fluunt, ut ante (M0b Chunk A); sub -semantica
     * solum - basis pristina manet. */
    si (cum_semantica)
    {
        fons_iso = _plagulam_totam_legere(piscina_ctx,
            "silva/fontes/systema_c89.h", &mensura_iso);
        si (fons_iso == NIHIL)
        {
            fprintf(stderr, "percursus: systema_c89.h deest"
                " (curre ex radice repositorii)\n");
            redde I;
        }
        fons_posix = _plagulam_totam_legere(piscina_ctx,
            "silva/fontes/systema_posix.h", &mensura_posix);
        si (fons_posix == NIHIL)
        {
            fprintf(stderr, "percursus: systema_posix.h deest\n");
            redde I;
        }
        praeparationes_tabula = tabula_dispersa_creare_chorda(
            piscina_ctx, LXIV);
        praeparationes_index = xar_creare(piscina_ctx,
            (i32)magnitudo(PraeparatioLexici*));
        si (   praeparationes_tabula == NIHIL
            || praeparationes_index  == NIHIL)
        {
            fprintf(stderr, "percursus: cache praeparationum"
                " deest\n");
            redde I;
        }
    }

    /* Praepassus: omne caput repositorii COLLIGERE (textus semel
     * lectus) - expansio per inclusionem VERAM (nullum lexicon
     * incondicionale) */
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        capita_lecta = xar_creare(piscina_ctx,
            (i32)magnitudo(CaputLectum));
        si (visa == NIHIL || capita_lecta == NIHIL)
        {
            fprintf(stderr, "percursus: tabula deest\n");
            redde I;
        }
        /* SEMPER a radice repositorii (cwd), non a radice
         * percursus: plagulae sub radix capita EXTRA radicem
         * includunt (silva/fontes -> include/latina.h) */
        _capita_praeparare(piscina_ctx, visa, ".");
    }

    /* contextus basis (sine -semantica): capita, nullum lexicon -
     * basis pristina. Sub -semantica contextus omnes per
     * praeparationem veniunt (_praeparationem_capere). */
    si (!cum_semantica)
    {
        ctx_basis = silva_contextus_creare(piscina_ctx);
        si (ctx_basis == NIHIL)
        {
            fprintf(stderr, "percursus: contextus deest\n");
            redde I;
        }
        si (mensura_maxima == ZEPHYRUM)
        {
            /* -omnia = sine tecto, ETIAM fluxus expansus (defaltum
             * 1M lexemata expansionem in plagulis giganteis decidit
             * - capsula_libri 3.7M lexemata, inventum Chunk D);
             * idem in fabrica praeparationum */
            ctx_basis->fines.lexemata = ZEPHYRUM;
        }
        _capita_praebere_in(ctx_basis);
    }

    _directorium_percurrere(radix);

    imprimere("\n=== PERCURSUS REPOSITORII (c89 + inclusio"
        " vera) ===\n");
    imprimere("capita:    %d praebita", (int)capita_praebita);
    si (capita_collisiones > ZEPHYRUM)
    {
        imprimere("  [%d collisiones basename - primus vicit]",
            (int)capita_collisiones);
    }
    imprimere("\n");
    imprimere("plagulae:  %d (%.1f MB)", (int)plagulae,
        summa_octetorum / 1048576.0);
    si (praetermissae > ZEPHYRUM)
    {
        imprimere("  [+%d praetermissae super tectum]",
            (int)praetermissae);
    }
    imprimere("\n");
    imprimere("arbores:   %d (totalitas %s)\n", (int)arbores,
        (arbores == plagulae) ? "TENET" : "FRACTA!");
    imprimere("fideles:   %d/%d octetim%s\n", (int)fideles,
        (int)plagulae,
        (fideles == plagulae) ? "" : "  <- INSPICE");
    imprimere("errores:   %d nodi in %d plagulis\n",
        (int)summa_errorum, (int)plagulae_cum_erroribus);
    imprimere("ambigui:   %d retenti in %d plagulis\n",
        (int)summa_ambiguorum, (int)plagulae_cum_ambiguis);
    si (cum_semantica)
    {
        imprimere("semantica: %d symbola; %d typationes;"
            " %d diagnostica in %d plagulis\n",
            (int)summa_symbolorum, (int)summa_typationum,
            (int)summa_diagnosticorum,
            (int)plagulae_cum_diagnosticis);
        {
            /* census codicum (examen chunk C): quae classes
             * flagrant - fons veritatis pro columna verdicti */
            s32 cc;

            per (cc = ZEPHYRUM; cc < (s32)EXAMEN_CODEX_NUMERUS;
                 cc++)
            {
                si (census_codicum[cc] > ZEPHYRUM)
                {
                    imprimere("  [codex %2d sev %d] %6d  %s\n",
                        (int)cc,
                        (int)silva_c89_codicis_severitas(cc),
                        (int)census_codicum[cc],
                        silva_c89_codicis_causa(cc));
                }
            }
        }
        imprimere("verdicta:  %d REICE (examen; cetera ACCIPE)\n",
            (int)verdicta_reice);
        imprimere("clausura:  %d versae; %d indecisa (residuum)\n",
            (int)summa_versorum, (int)summa_indecisorum);
        si (summa_expr_visorum > ZEPHYRUM)
        {
            imprimere("coopertura: %d/%d nodi expressionum typati"
                " (%.2f%%)\n",
                (int)summa_expr_typatorum, (int)summa_expr_visorum,
                100.0 * (duplex)summa_expr_typatorum
                    / (duplex)summa_expr_visorum);
        }
    }
    alioquin
    {
        /* silentium non est successus: absentia iudicii NOMINATUR
         * ne "corpus-zero" vacue verum legatur (01KXQ3FY0Q) */
        imprimere("modus:     parse-only (iudicium semanticum"
            " NON CURSUM - adde -semantica)\n");
    }
    si (fines_tactae_plagulae > ZEPHYRUM)
    {
        imprimere("fines:     %d plagulae limen tactae\n",
            (int)fines_tactae_plagulae);
    }
    imprimere("tempus:    %.0f ms (%.2f ms/KB)\n", summa_ms,
        summa_octetorum > 0.0
            ? summa_ms / (summa_octetorum / 1024.0) : 0.0);
    imprimere("apex:      %.1f MB (%s)\n",
        apex_maximus / 1048576.0, plagula_apicis);
    si (cum_semantica)
    {
        /* memoria praeparationum = factum impressum, non
         * aestimatum (INTENTIO design B) */
        duplex octeti_praep  = 0.0;
           i32 np            = xar_numerus(praeparationes_index);
           i32 pi;

        per (pi = ZEPHYRUM; pi < np; pi++)
        {
            PraeparatioLexici** situs = (PraeparatioLexici**)
                xar_obtinere(praeparationes_index, pi);

            si (situs != NIHIL && *situs != NIHIL)
            {
                octeti_praep += (duplex)piscina_summa_apex_usus(
                    (*situs)->piscina);
            }
        }
        imprimere("praeparationes: %d (%.1f MB)\n", (int)np,
            octeti_praep / 1048576.0);
    }
    si (plagulae_infra > ZEPHYRUM)
    {
        imprimere("INFRA:     %d plagulae NON iudicatae"
            " (annotatio/compositio/praeparatio fracta)\n",
            (int)plagulae_infra);
    }
    imprimere("\n--- numerationes evidentiae ---\n");
    imprimere("typedef in corpore functionis: %d (decisiones 13)\n",
        (int)typedef_in_corpore);
    imprimere("casus intra ERROR (Duffiana):  %d\n",
        (int)duffiana);
    imprimere("\"__\" intra ERROR (gcc):        %d\n",
        (int)gcc_in_errore);
    redde (arbores == plagulae) ? ZEPHYRUM : I;
}
