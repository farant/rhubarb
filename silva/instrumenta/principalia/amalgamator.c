/* amalgamator.c - Amalgamator silvae (dev-time, Phase 6)
 *
 * Usus: amalgamator <radix_repositorii> <via_exitus>
 * e.g.: amalgamator .. amalgama/silva.c
 *
 * Emittit silva.c unicum ex manifesto (sim ④ / spec-v2 par 11.1):
 *   [includenda systematis sublata] [silva.h - Chunk B] [latina.h
 *   verbatim] [capita vendicata renominata] [corpora vendicata
 *   renominata + static iniecta].
 *
 * Renominator lexemate-conscius LEXATORE SILVAE IPSIUS utitur
 * (bootstrap: instrumentum dev-time fontes ordinarie nectit).
 * Identificatores exacti: typi exacti + praefixa functionum
 * (longissimum primum). Commenta/chordae numquam tanguntur (trivia
 * verbatim transeunt). Custodiae capitum verbatim manent (S46).
 *
 * Manifestum = ordines expliciti in hoc fonte (manifestum EST codex):
 * capita ordine topologico capitum, corpora ordine corporum (S42),
 * servanda per plagulam (S44: friatio tres functiones servat -
 * unitates ceterae cadunt, prototypa inclusa).
 *
 * Unitas = declaratio/definitio suprema una (ad ';' in profunditate
 * 0, aut ad '}' parem definitionis functionis). Functiones vendicatae
 * nondum staticae 'static' accipiunt (S44 - superficies exportata =
 * API publica sola). Definitiones staticae duplicatae trans corpora
 * = error (S41, impositum non praesumptum).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include <stdio.h>
#include <string.h>

#define VIA_MAXIMA 1024

/* ==================================================
 * Manifestum
 * ================================================== */

nomen structura {
    constans character*  via;      /* relativa ad radicem */
    constans character* constans* servanda;   /* NIHIL = omnia */
    constans character* constans* excludenda; /* cadenda; NIHIL = nulla */
    b32                  est_corpus;    /* definitiones (S41) */
    b32                  est_vendicata; /* static iniectio + rename */
} AmalgamaPlagula;

interior constans character* constans SERVANDA_FRIATIONIS[] = {
    "friatio_fnv1a", NIHIL
};

/* S44 (finis): static internalizatio x -Werror functiones inusitatas
 * in defectus compilationis vertit - manifestum functiones NON
 * consumptas nominatim demittit. Indices ex compilatore ipso lectae
 * (-Wunused-function harvest); functio nova consumpta = introitum
 * delere (compilatio id clamat). */
interior constans character* constans EXCLUDENDA_PISCINAE[] = {
    "piscina_conari_allocare", "piscina_conari_allocare_ordinatum",
    "piscina_generare_certae_magnitudinis", "piscina_notare",
    "piscina_potesne_allocare", "piscina_reficere",
    "piscina_reliqua_antequam_cresca_alvei",
    "piscina_summa_apex_usus", "piscina_summa_inutilis_allocatus",
    "piscina_summa_usus", "piscina_vacare", "_catena_alveus_vacare",
    NIHIL
};

interior constans character* constans EXCLUDENDA_CHORDAE[] = {
    "chorda_aequalis", "chorda_aequalis_case_insensitivus",
    "chorda_aequalis_literis", "chorda_camelus",
    "chorda_character_ad", "chorda_concatenare", "chorda_continet",
    "chorda_duplicare", "chorda_ex_buffer",
    "chorda_ex_bytes_legibilis", "chorda_ex_f64", "chorda_ex_s32",
    "chorda_fissio", "chorda_fissio_chorda", "chorda_friare",
    "chorda_incipit", "chorda_invenire", "chorda_invenire_index",
    "chorda_invenire_ultimum", "chorda_invenire_ultimum_index",
    "chorda_invertere", "chorda_iungere", "chorda_kebab",
    "chorda_maiuscula", "chorda_minuscula", "chorda_pascalis",
    "chorda_pascalis_serpens", "chorda_praecidere",
    "chorda_praecidere_dextram", "chorda_praecidere_sinistram",
    "chorda_praecidi_laterale", "chorda_serpens",
    "chorda_substituere", "chorda_terminatur", "chorda_ut_cstr",
    "chorda_ut_f64", "chorda_ut_i32", "chorda_ut_s32",
    "chorda_vacua", "chorda_numerare_occurrentia", "chorda_sectio",
    "_extrahere_verba", NIHIL
};

interior constans character* constans EXCLUDENDA_AEDIFICATORIS[] = {
    "chorda_aedificator_appendere_f64",
    "chorda_aedificator_appendere_hex_i32",
    "chorda_aedificator_appendere_i32",
    "chorda_aedificator_appendere_indentationem",
    "chorda_aedificator_appendere_lineam_novam",
    "chorda_aedificator_appendere_literis_evasus_json",
    "chorda_aedificator_appendere_repetita",
    "chorda_aedificator_appendere_s32",
    "chorda_aedificator_destruere",
    "chorda_aedificator_indentatio_gradus",
    "chorda_aedificator_longitudo",
    "chorda_aedificator_pop_indentationem",
    "chorda_aedificator_push_indentationem",
    "chorda_aedificator_reset", "chorda_aedificator_spectare",
    "chorda_aedificator_appendere_evasus_json",
    "_format_integer_s32", "_format_integer_i32", "_format_duplex",
    "_evadere_json", NIHIL
};

interior constans character* constans EXCLUDENDA_TABULAE[] = {
    "tabula_dispersa_continet_literis",
    "tabula_dispersa_iterator_initium",
    "tabula_dispersa_iterator_proximum", "tabula_dispersa_numerus",
    "tabula_dispersa_status_imprimere", "tabula_dispersa_vacare",
    "tabula_friare_multiplicatio",
    "tabula_dispersa_invenire_literis", "_friare_literis",
    "_invenire_slotum_literis", "_aequalis_literis_chorda", NIHIL
};

interior constans character* constans EXCLUDENDA_XARIS[] = {
    "xar_addere_multos", "xar_capacitas", "xar_continet",
    "xar_copiare_ad_tabulam", "xar_creare_cum_magnitudine",
    "xar_destruere", "xar_invenire", "xar_invertere",
    "xar_iterator_finis", "xar_iterator_initium",
    "xar_iterator_proximum", "xar_obtinere_s", "xar_ponere",
    "xar_ponere_vexilla", "xar_quaerere_binarie",
    "xar_quaerere_binarie_index", "xar_removere_cum_ultimo",
    "xar_removere_ultimum", "xar_status_imprimere", "xar_tollere",
    "xar_vacuum_est", "xar_obtinere_vel_creare", "xar_invenire_index",
    NIHIL
};

/* Ordo capitum (S42: topologia capitum != topologia corporum) */
interior constans AmalgamaPlagula CAPITA_VENDICATA[] = {
    { "include/piscina.h",            NIHIL, EXCLUDENDA_PISCINAE,      FALSUM, VERUM },
    { "include/chorda.h",             NIHIL, EXCLUDENDA_CHORDAE,       FALSUM, VERUM },
    { "include/chorda_aedificator.h", NIHIL, EXCLUDENDA_AEDIFICATORIS, FALSUM, VERUM },
    { "include/friatio.h",            SERVANDA_FRIATIONIS, NIHIL,     FALSUM, VERUM },
    { "include/tabula_dispersa.h",    NIHIL, EXCLUDENDA_TABULAE,       FALSUM, VERUM },
    { "include/xar.h",                NIHIL, EXCLUDENDA_XARIS,         FALSUM, VERUM }
};

interior constans AmalgamaPlagula CORPORA_VENDICATA[] = {
    { "lib/piscina.c",            NIHIL, EXCLUDENDA_PISCINAE,      VERUM, VERUM },
    { "lib/chorda.c",             NIHIL, EXCLUDENDA_CHORDAE,       VERUM, VERUM },
    { "lib/chorda_aedificator.c", NIHIL, EXCLUDENDA_AEDIFICATORIS, VERUM, VERUM },
    { "lib/friatio.c",            SERVANDA_FRIATIONIS, NIHIL,     VERUM, VERUM },
    { "lib/tabula_dispersa.c",    NIHIL, EXCLUDENDA_TABULAE,       VERUM, VERUM },
    { "lib/xar.c",                NIHIL, EXCLUDENDA_XARIS,         VERUM, VERUM }
};

/* Capita silvae: post vendicata (typi vendicati retenti - e.g.
 * SilvaTabulaDispersa in definitionibus SilvaOraculum/SilvaExpansio -
 * ante usum stare debent). silva.h typos CADENDA possidet; cetera
 * (typi interni, prototypa, macros) transeunt. */
interior constans AmalgamaPlagula CAPITA_SILVAE[] = {
    { "silva/fontes/silva_token.h",           NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_lexema.h",          NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_expandere.h",       NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_conditio.h",        NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_nodus.h",           NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_tabulae.h",         NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_tabulae_sceleti.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_glr.h",             NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_commissio.h",       NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_parsare.h",         NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_scribere.h",        NIHIL, NIHIL, FALSUM, FALSUM }
};

interior constans AmalgamaPlagula CORPORA_SILVAE[] = {
    { "silva/fontes/silva_token.c",           NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_lexema.c",          NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_expandere.c",       NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_conditio.c",        NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_nodus.c",           NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_tabulae_sceleti.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_glr.c",             NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_commissio.c",       NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_parsare.c",         NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_scribere.c",        NIHIL, NIHIL, VERUM, FALSUM }
};

/* CADENDA: tituli typorum quos silva.h POSSIDET - unitates typedef
 * eorundem titulorum ex capitibus internis cadunt (definitio una per
 * TU - C89 typedef duplicata vetat; prototypa duplicata et macros
 * identica LICENT, ergo solum typi cadunt). */
interior constans character* constans CADENDA_TYPEDEF[] = {
    "Piscina", "chorda", "Xar",
    "SilvaCaecatio", "SilvaOrigoGenus", "SilvaOrigo", "SilvaToken",
    "SilvaLexemaGenus", "SilvaLocusSpecies", "SilvaValorGenus",
    "SilvaListaProspectus", "SilvaValor", "SilvaNodus",
    "SilvaTabActioGenus", "SilvaTabActio", "SilvaTabGoto",
    "SilvaTabStatus", "SilvaTabSymbolum", "SilvaTabProductio",
    "SilvaTabLocus", "SilvaTabGenus", "SilvaRegistrumCoctum",
    "SilvaTabulaCocta", "SilvaGLRConstructor",
    "SilvaGLRFabricaAmbigui", "SilvaOraculum",
    "SilvaResolutioResponsum", "SilvaResolutor",
    "SilvaResolutioGenus", "SilvaResolutioEventum", "SilvaCommissio",
    "SilvaExpansio", "SilvaGrammatica", "SilvaParsura",
    "SilvaScriptura", NIHIL
};

/* Definitiones tag quas silva.h PLENE possidet (definitio structurae
 * interna quoque cadit). SilvaOraculum/SilvaExpansio NON hic: silva.h
 * eas solum praenuntiat - definitiones internae manent (interna eas
 * dereferunt). */
interior constans character* constans CADENDA_DEFINITIO[] = {
    "SilvaToken", "SilvaValor", "SilvaNodus", "SilvaCaecatio", NIHIL
};

/* Functiones vendicatae quae PUBLICAE manent (extern, in silva.h) */
interior constans character* constans NON_STATICA[] = {
    "piscina_generare_dynamicum", "piscina_destruere",
    "xar_numerus", "xar_obtinere", NIHIL
};

/* Renominationes typorum (exactae). Typi symbola nexus non creant,
 * sed silva.h eosdem SilvaXxx nominabit - corpus et caput consentire
 * debent. */
nomen structura {
    constans character* vetus;
    constans character* novum;
} Renominatio;

interior constans Renominatio TYPI_EXACTI[] = {
    { "Piscina",               "SilvaPiscina" },
    { "PiscinaNotatio",        "SilvaPiscinaNotatio" },
    { "chorda",                "SilvaChorda" },
    { "chorda_fissio_fructus", "silva_chorda_fissio_fructus" },
    { "ChordaAedificator",     "SilvaChordaAedificator" },
    { "TabulaDispersa",        "SilvaTabulaDispersa" },
    { "TabulaIterator",        "SilvaTabulaIterator" },
    { "Slotus",                "SilvaSlotus" },
    { "SlotusStatus",          "SilvaSlotusStatus" },
    { "Xar",                   "SilvaXar" },
    { "XarIterator",           "SilvaXarIterator" },
    { "XarComparator",         "SilvaXarComparator" }
};

/* Praefixa functionum - longissimum primum (chorda_aedificator_
 * ante chorda_!) */
interior constans Renominatio PRAEFIXA_FUNCTIONUM[] = {
    { "chorda_aedificator_", "silva_chorda_aedificator_" },
    { "tabula_dispersa_",    "silva_tabula_dispersa_" },
    { "friatio_",            "silva_friatio_" },
    { "piscina_",            "silva_piscina_" },
    { "chorda_",             "silva_chorda_" },
    { "xar_",                "silva_xar_" }
};

#define NUMERUS(series) ((i32)(magnitudo(series) / magnitudo((series)[0])))

/* ==================================================
 * Status amalgamatoris
 * ================================================== */

nomen structura {
    Piscina*            piscina;
    ChordaAedificator*  aed;
    constans character* radix;
    Xar*                systematis;  /* chorda - includenda <...> */
    TabulaDispersa*     statica;     /* titulus -> character* (via) */
    b32                 successus;
} Amalgamator;

/* ==================================================
 * Auxiliares
 * ================================================== */

interior i8*
_plagulam_legere (Piscina* piscina, constans character* via,
    i32* mensura_out)
{
    FILE* pl;
    i8* buffer;
    signatus longus mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);

    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    *mensura_out = (i32)mensura;
    redde buffer;
}

interior character*
_literis_figere (Piscina* piscina, constans character* literis)
{
    memoriae_index m = strlen(literis) + I;
    character* fixum = (character*)piscina_allocare(piscina, m);

    si (fixum != NIHIL)
    {
        memcpy(fixum, literis, m);
    }
    redde fixum;
}

interior b32
_chorda_est (chorda c, constans character* literis)
{
    si (c.mensura != (i32)strlen(literis))
    {
        redde FALSUM;
    }
    si (c.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (memcmp(c.datum, literis, (memoriae_index)c.mensura)
        == ZEPHYRUM) ? VERUM : FALSUM;
}

interior SilvaToken*
_ad (Xar* lexemata, i32 i)
{
    redde *(SilvaToken**)xar_obtinere(lexemata, i);
}

interior vacuum
_trivia_emittere (Amalgamator* am, Xar* spatia)
{
    i32 i;

    si (spatia == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(spatia); i++)
    {
        chorda_aedificator_appendere_chorda(am->aed,
            (*(SilvaToken**)xar_obtinere(spatia, i))->valor);
    }
}

/* Emittere lexema, renominatum si tabulae congruunt.
 * NB scissurae hic ignorantur (valor mundus emittitur) - laminae
 * intra lexemata in bibliothecis vendicatis non exsistunt, et textus
 * mundus semper compilat. */
interior vacuum
_lexema_emittere (Amalgamator* am, SilvaToken* t, b32 sine_ante)
{
    i32 k;

    si (!sine_ante)
    {
        _trivia_emittere(am, t->spatia_ante);
    }

    si (t->genus == SILVA_LEX_IDENTIFICATOR)
    {
        per (k = ZEPHYRUM; k < NUMERUS(TYPI_EXACTI); k++)
        {
            si (_chorda_est(t->valor, TYPI_EXACTI[k].vetus))
            {
                chorda_aedificator_appendere_literis(am->aed,
                    TYPI_EXACTI[k].novum);
                _trivia_emittere(am, t->spatia_post);
                redde;
            }
        }
        per (k = ZEPHYRUM; k < NUMERUS(PRAEFIXA_FUNCTIONUM); k++)
        {
            i32 m = (i32)strlen(PRAEFIXA_FUNCTIONUM[k].vetus);

            si (t->valor.mensura > m
                && memcmp(t->valor.datum,
                       PRAEFIXA_FUNCTIONUM[k].vetus,
                       (memoriae_index)m) == ZEPHYRUM)
            {
                chorda cauda;

                chorda_aedificator_appendere_literis(am->aed,
                    PRAEFIXA_FUNCTIONUM[k].novum);
                cauda.datum = t->valor.datum + m;
                cauda.mensura = t->valor.mensura - m;
                chorda_aedificator_appendere_chorda(am->aed, cauda);
                _trivia_emittere(am, t->spatia_post);
                redde;
            }
        }
    }

    chorda_aedificator_appendere_chorda(am->aed, t->valor);
    _trivia_emittere(am, t->spatia_post);
}

/* ==================================================
 * Scansio unitatum
 * ================================================== */

interior i32
_lineam_finire (Xar* lexemata, i32 i, i32 n)
{
    i32 j = i + I;

    dum (j < n)
    {
        SilvaToken* t = _ad(lexemata, j);

        si (t->initium_lineae || t->genus == SILVA_LEX_EOF)
        {
            frange;
        }
        j++;
    }
    redde j;
}

/* Unitas suprema: [i, finis). titulus_out = identificator ante '('
 * primum (functio) aut ante '='/'[' aut ultimus ante ';' (datum,
 * typus) aut post structura/unio/enumeratio (definitio tag) aut in
 * '(*IDENT)' (typedef indicis functionis). est_definitio_out:
 * corpus functionis {...} habet. */
interior i32
_unitatem_finire (Xar* lexemata, i32 i, i32 n, chorda* titulus_out,
    b32* est_functio_out, b32* est_statica_out, b32* est_definitio_out,
    b32* est_typedef_out, b32* est_tag_def_out)
{
    i32 pp = ZEPHYRUM;
    i32 pb = ZEPHYRUM;
    b32 vidit_clausam = FALSUM;
    b32 est_typedef = FALSUM;
    b32 titulus_fixus = FALSUM;
    chorda ultimus;
    SilvaLexemaGenus prius = SILVA_LEX_EOF;
    SilvaLexemaGenus prius_prius = SILVA_LEX_EOF;
    i32 j;

    titulus_out->datum = NIHIL;
    titulus_out->mensura = ZEPHYRUM;
    ultimus.datum = NIHIL;
    ultimus.mensura = ZEPHYRUM;
    *est_functio_out = FALSUM;
    *est_statica_out = FALSUM;
    *est_definitio_out = FALSUM;
    *est_tag_def_out = FALSUM;

    {
        SilvaToken* primum = _ad(lexemata, i);

        si (primum->genus == SILVA_LEX_STATIC)
        {
            *est_statica_out = VERUM;
        }
        alioquin si (primum->genus == SILVA_LEX_IDENTIFICATOR
            && (_chorda_est(primum->valor, "interior")
                || _chorda_est(primum->valor, "hic_manens")
                || _chorda_est(primum->valor, "staticus")
                || _chorda_est(primum->valor, "universalis")))
        {
            *est_statica_out = VERUM;
        }
        si (primum->genus == SILVA_LEX_TYPEDEF
            || (primum->genus == SILVA_LEX_IDENTIFICATOR
                && _chorda_est(primum->valor, "nomen")))
        {
            est_typedef = VERUM;
        }

        /* Definitio tag (structura X {...};) - titulus = tag ipse */
        si (!est_typedef
            && (primum->genus == SILVA_LEX_STRUCT
                || primum->genus == SILVA_LEX_UNION
                || primum->genus == SILVA_LEX_ENUM
                || (primum->genus == SILVA_LEX_IDENTIFICATOR
                    && (_chorda_est(primum->valor, "structura")
                        || _chorda_est(primum->valor, "unio")
                        || _chorda_est(primum->valor, "enumeratio"))))
            && i + I < n)
        {
            SilvaToken* secundum = _ad(lexemata, i + I);

            si (secundum->genus == SILVA_LEX_IDENTIFICATOR)
            {
                *titulus_out = secundum->valor;
                titulus_fixus = VERUM;
                *est_tag_def_out = VERUM;
            }
        }
    }
    *est_typedef_out = est_typedef;

    per (j = i; j < n; j++)
    {
        SilvaToken* t = _ad(lexemata, j);

        commutatio (t->genus)
        {
            casus SILVA_LEX_EOF:
                redde j;
            casus SILVA_LEX_PAREN_APERTA:
                si (pp == ZEPHYRUM && pb == ZEPHYRUM && !est_typedef
                    && !titulus_fixus && ultimus.datum != NIHIL)
                {
                    *titulus_out = ultimus;
                    *est_functio_out = VERUM;
                    titulus_fixus = VERUM;
                }
                pp++;
                frange;
            casus SILVA_LEX_PAREN_CLAUSA:
                pp--;
                si (pp == ZEPHYRUM && pb == ZEPHYRUM)
                {
                    vidit_clausam = VERUM;
                }
                frange;
            casus SILVA_LEX_BRACE_APERTA:
                si (pb == ZEPHYRUM && pp == ZEPHYRUM && vidit_clausam
                    && *est_functio_out)
                {
                    *est_definitio_out = VERUM;
                }
                pb++;
                frange;
            casus SILVA_LEX_BRACE_CLAUSA:
                pb--;
                si (pb == ZEPHYRUM && *est_definitio_out)
                {
                    redde j + I;
                }
                frange;
            casus SILVA_LEX_SEMICOLON:
                si (pp == ZEPHYRUM && pb == ZEPHYRUM)
                {
                    si (titulus_out->datum == NIHIL)
                    {
                        *titulus_out = ultimus;
                    }
                    redde j + I;
                }
                frange;
            casus SILVA_LEX_ASSIGNATIO:
            casus SILVA_LEX_QUADRA_APERTA:
                si (pp == ZEPHYRUM && pb == ZEPHYRUM && !titulus_fixus
                    && ultimus.datum != NIHIL)
                {
                    *titulus_out = ultimus;
                    titulus_fixus = VERUM;
                }
                frange;
            casus SILVA_LEX_IDENTIFICATOR:
                /* typedef indicis functionis: nomen ... (*IDENT)(...) */
                si (est_typedef && !titulus_fixus
                    && prius == SILVA_LEX_STAR
                    && prius_prius == SILVA_LEX_PAREN_APERTA)
                {
                    *titulus_out = t->valor;
                    titulus_fixus = VERUM;
                }
                ultimus = t->valor;
                frange;
            ordinarius:
                frange;
        }
        prius_prius = prius;
        prius = t->genus;
    }
    redde n;
}

interior b32
_in_indice (chorda titulus, constans character* constans* index)
{
    i32 k;

    si (titulus.datum == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; index[k] != NIHIL; k++)
    {
        si (_chorda_est(titulus, index[k]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_in_servandis (chorda titulus,
    constans character* constans* servanda)
{
    i32 k;

    si (servanda == NIHIL)
    {
        redde VERUM;
    }
    si (titulus.datum == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; servanda[k] != NIHIL; k++)
    {
        si (_chorda_est(titulus, servanda[k]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Processio plagulae
 * ================================================== */

interior vacuum
_includendum_systematis_subferre (Amalgamator* am, Xar* lexemata,
    i32 i, i32 lf)
{
    ChordaAedificator* titulus_aed;
    chorda titulus;
    i32 j;
    b32 novum = VERUM;

    si (i + II >= lf
        || _ad(lexemata, i + II)->genus != SILVA_LEX_MINOR)
    {
        redde;  /* "proiectum.h" - cadit sine sublatione */
    }
    titulus_aed = chorda_aedificator_creare(am->piscina, XXXII);
    per (j = i + III; j < lf; j++)
    {
        SilvaToken* u = _ad(lexemata, j);

        si (u->genus == SILVA_LEX_MAIOR)
        {
            frange;
        }
        chorda_aedificator_appendere_chorda(titulus_aed, u->valor);
    }
    titulus = chorda_aedificator_finire(titulus_aed);
    per (j = ZEPHYRUM; j < xar_numerus(am->systematis); j++)
    {
        chorda* c = (chorda*)xar_obtinere(am->systematis, j);

        si (c->mensura == titulus.mensura
            && (titulus.mensura == ZEPHYRUM
                || memcmp(c->datum, titulus.datum,
                       (memoriae_index)c->mensura) == ZEPHYRUM))
        {
            novum = FALSUM;
            frange;
        }
    }
    si (novum)
    {
        chorda* locus = (chorda*)xar_addere(am->systematis);

        si (locus != NIHIL)
        {
            *locus = titulus;
        }
    }
}

interior vacuum
_plagulam_processare (Amalgamator* am, constans AmalgamaPlagula* pl)
{
    character via[VIA_MAXIMA];
    character* via_fixa;
    i8* fons;
    i32 mensura;
    Xar* lexemata;
    i32 n;
    i32 i;

    sprintf(via, "%s/%s", am->radix, pl->via);
    fons = _plagulam_legere(am->piscina, via, &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "amalgamator: plagula non lecta: %s\n", via);
        am->successus = FALSUM;
        redde;
    }
    via_fixa = _literis_figere(am->piscina, pl->via);

    chorda_aedificator_appendere_literis(am->aed,
        "\n/* ================= ex ");
    chorda_aedificator_appendere_literis(am->aed, pl->via);
    chorda_aedificator_appendere_literis(am->aed,
        " ================= */\n");

    lexemata = silva_lexare(am->piscina, (constans character*)fons,
        mensura, ZEPHYRUM);
    n = xar_numerus(lexemata);
    i = ZEPHYRUM;

    dum (i < n)
    {
        SilvaToken* t = _ad(lexemata, i);

        si (t->genus == SILVA_LEX_EOF)
        {
            _trivia_emittere(am, t->spatia_ante);
            frange;
        }

        /* Directivae: includenda systematis sublata, proiecti remota;
         * ceterae (custodiae, defines) verbatim renominatae */
        si (t->genus == SILVA_LEX_CANCELLUM && t->initium_lineae)
        {
            i32 lf = _lineam_finire(lexemata, i, n);
            b32 est_includendum = FALSUM;

            si (i + I < lf)
            {
                SilvaToken* nomen_dir = _ad(lexemata, i + I);

                si (nomen_dir->genus == SILVA_LEX_IDENTIFICATOR
                    && _chorda_est(nomen_dir->valor, "include"))
                {
                    est_includendum = VERUM;
                }
            }
            si (est_includendum)
            {
                _includendum_systematis_subferre(am, lexemata, i, lf);
                i = lf;
                perge;
            }
            {
                i32 j;

                per (j = i; j < lf; j++)
                {
                    _lexema_emittere(am, _ad(lexemata, j), FALSUM);
                }
            }
            i = lf;
            perge;
        }

        /* Unitas suprema */
        {
            chorda titulus;
            b32 est_functio;
            b32 est_statica;
            b32 est_definitio;
            b32 est_typedef;
            b32 est_tag_def;
            b32 inicere;
            i32 finis = _unitatem_finire(lexemata, i, n, &titulus,
                &est_functio, &est_statica, &est_definitio,
                &est_typedef, &est_tag_def);
            i32 j;

            si (!_in_servandis(titulus, pl->servanda))
            {
                i = finis;  /* unitas exclusa cadit (S44) */
                perge;
            }
            si (pl->excludenda != NIHIL
                && _in_indice(titulus, pl->excludenda))
            {
                i = finis;  /* inusitata demissa (S44 finis) */
                perge;
            }

            /* CADENDA: typi quos silva.h possidet cadunt ex capitibus
             * (definitio una per TU). Typedef unitates per titulum;
             * definitiones tag solum si silva.h structuram PLENE
             * possidet. */
            si (!pl->est_corpus
                && ((est_typedef
                        && _in_indice(titulus, CADENDA_TYPEDEF))
                    || (est_tag_def
                        && _in_indice(titulus, CADENDA_DEFINITIO))))
            {
                i = finis;
                perge;
            }

            /* S41: definitiones staticae duplicatae trans corpora =
             * error in UNO TU (vendicatae iniectae + interiores
             * silvae aeque examinantur) */
            si (pl->est_corpus && est_functio && est_definitio
                && titulus.datum != NIHIL
                && (est_statica || pl->est_vendicata))
            {
                vacuum* valor_prior = NIHIL;

                si (tabula_dispersa_invenire(am->statica, titulus,
                        &valor_prior))
                {
                    fprintf(stderr, "amalgamator: definitio duplicata "
                        "'%.*s' (%s et %s)\n",
                        (int)titulus.mensura,
                        (constans character*)titulus.datum,
                        (character*)valor_prior, pl->via);
                    am->successus = FALSUM;
                }
                alioquin
                {
                    chorda clavis;
                    i8* datum_fixum = (i8*)piscina_allocare(
                        am->piscina,
                        (memoriae_index)(titulus.mensura > ZEPHYRUM
                            ? titulus.mensura : I));

                    si (datum_fixum != NIHIL
                        && titulus.mensura > ZEPHYRUM)
                    {
                        memcpy(datum_fixum, titulus.datum,
                            (memoriae_index)titulus.mensura);
                    }
                    clavis.datum = datum_fixum;
                    clavis.mensura = titulus.mensura;
                    tabula_dispersa_inserere(am->statica, clavis,
                        (vacuum*)via_fixa);
                }
            }

            /* static inicere in functiones vendicatas nondum staticas
             * (S44); publicae vendicatae (NON_STATICA) et functiones
             * silvae (API - extern) intactae */
            inicere = (pl->est_vendicata && est_functio && !est_statica
                && !_in_indice(titulus, NON_STATICA)) ? VERUM : FALSUM;
            si (inicere)
            {
                SilvaToken* primum = _ad(lexemata, i);

                _trivia_emittere(am, primum->spatia_ante);
                chorda_aedificator_appendere_literis(am->aed,
                    "static ");
                _lexema_emittere(am, primum, VERUM);
                per (j = i + I; j < finis; j++)
                {
                    _lexema_emittere(am, _ad(lexemata, j), FALSUM);
                }
            }
            alioquin
            {
                per (j = i; j < finis; j++)
                {
                    _lexema_emittere(am, _ad(lexemata, j), FALSUM);
                }
            }
            i = finis;
        }
    }
}

/* ==================================================
 * Principale
 * ================================================== */

s32 principale (s32 argc, character** argv)
{
    Piscina* piscina;
    Amalgamator am;
    character via_latinae[VIA_MAXIMA];
    i8* latina;
    i32 mensura_latinae;
    chorda corpus;
    i32 k;

    si (argc < III)
    {
        fprintf(stderr,
            "usus: amalgamator <radix_repositorii> <via_exitus>\n");
        redde I;
    }

    piscina = piscina_generare_dynamicum("amalgamator", 33554432);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "amalgamator: piscina non generata\n");
        redde I;
    }

    am.piscina = piscina;
    am.aed = chorda_aedificator_creare(piscina, 262144);
    am.radix = argv[I];
    am.systematis = xar_creare(piscina, magnitudo(chorda));
    am.statica = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    am.successus = VERUM;

    /* latina.h verbatim (numquam renominata - S43) */
    sprintf(via_latinae, "%s/include/latina.h", am.radix);
    latina = _plagulam_legere(piscina, via_latinae, &mensura_latinae);
    si (latina == NIHIL)
    {
        fprintf(stderr, "amalgamator: latina.h non lecta\n");
        piscina_destruere(piscina);
        redde I;
    }

    /* Corpus primum aedificatur (includenda systematis inter
     * processionem colliguntur), prooemium postea praeponitur */
    per (k = ZEPHYRUM; k < NUMERUS(CAPITA_VENDICATA); k++)
    {
        _plagulam_processare(&am, &CAPITA_VENDICATA[k]);
    }

    /* Assertio derivae (C89 more): silva.h magnitudinem seriei
     * segmentorum SilvaXar 64 codificat - macro internum consentire
     * DEBET aut compilatio frangitur */
    chorda_aedificator_appendere_literis(am.aed,
        "\n/* assertio derivae: silva.h SilvaXar.segmenta[64] ==\n"
        " * XAR_MAXIMUS_SEGMENTORUM internum */\n"
        "typedef char silva_assertio_xar_segmentorum[\n"
        "    (XAR_MAXIMUS_SEGMENTORUM == 64) ? 1 : -1];\n");

    per (k = ZEPHYRUM; k < NUMERUS(CAPITA_SILVAE); k++)
    {
        _plagulam_processare(&am, &CAPITA_SILVAE[k]);
    }
    per (k = ZEPHYRUM; k < NUMERUS(CORPORA_VENDICATA); k++)
    {
        _plagulam_processare(&am, &CORPORA_VENDICATA[k]);
    }
    per (k = ZEPHYRUM; k < NUMERUS(CORPORA_SILVAE); k++)
    {
        _plagulam_processare(&am, &CORPORA_SILVAE[k]);
    }
    corpus = chorda_aedificator_finire(am.aed);

    si (!am.successus)
    {
        piscina_destruere(piscina);
        redde I;
    }

    {
        ChordaAedificator* totum =
            chorda_aedificator_creare(piscina, 262144);
        chorda latina_chorda;

        chorda_aedificator_appendere_literis(totum,
            "/* silva.c - GENERATUM (amalgamator) - NE MANU EDITES\n"
            " *\n"
            " * Substratum silvae in plagula una (SQLite modo).\n"
            " * Fons veritatis: silva/fontes/ + bibliothecae\n"
            " * vendicatae in lib/. Regenerare: silva/amalgamare.sh\n"
            " */\n\n");
        per (k = ZEPHYRUM; k < xar_numerus(am.systematis); k++)
        {
            chorda* c = (chorda*)xar_obtinere(am.systematis, k);

            chorda_aedificator_appendere_literis(totum, "#include <");
            chorda_aedificator_appendere_chorda(totum, *c);
            chorda_aedificator_appendere_literis(totum, ">\n");
        }
        /* silva.h VERBATIM (manu scriptum, vanilla C89 - S43) */
        {
            character via_capitis[VIA_MAXIMA];
            i8* caput;
            i32 mensura_capitis;
            chorda caput_chorda;

            sprintf(via_capitis, "%s/silva/amalgama/silva.h",
                am.radix);
            caput = _plagulam_legere(piscina, via_capitis,
                &mensura_capitis);
            si (caput == NIHIL)
            {
                fprintf(stderr,
                    "amalgamator: silva.h non lectum: %s\n",
                    via_capitis);
                piscina_destruere(piscina);
                redde I;
            }
            chorda_aedificator_appendere_literis(totum,
                "\n/* ================= silva.h (verbatim) "
                "================= */\n");
            caput_chorda.datum = caput;
            caput_chorda.mensura = mensura_capitis;
            chorda_aedificator_appendere_chorda(totum, caput_chorda);
        }
        chorda_aedificator_appendere_literis(totum,
            "\n/* ================= ex include/latina.h "
            "================= */\n");
        latina_chorda.datum = latina;
        latina_chorda.mensura = mensura_latinae;
        chorda_aedificator_appendere_chorda(totum, latina_chorda);
        chorda_aedificator_appendere_chorda(totum, corpus);

        {
            chorda exitus = chorda_aedificator_finire(totum);
            FILE* pl = fopen(argv[II], "wb");

            si (pl == NIHIL)
            {
                fprintf(stderr,
                    "amalgamator: exitus non apertus: %s\n", argv[II]);
                piscina_destruere(piscina);
                redde I;
            }
            si (exitus.mensura > ZEPHYRUM
                && fwrite(exitus.datum, I,
                       (memoriae_index)exitus.mensura, pl)
                    != (memoriae_index)exitus.mensura)
            {
                fprintf(stderr, "amalgamator: scriptio fracta\n");
                fclose(pl);
                piscina_destruere(piscina);
                redde I;
            }
            fclose(pl);
            imprimere("amalgamator: %s scriptum (%d octeti, "
                "%d includenda systematis)\n",
                argv[II], (int)exitus.mensura,
                (int)xar_numerus(am.systematis));
        }
    }

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
