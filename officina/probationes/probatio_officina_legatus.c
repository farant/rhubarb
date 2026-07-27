/* probatio_officina_legatus.c - Transcripta aurea machinae legati
 * (LEGATUS chunk B: ansa, initialize, lamina diagnosticorum,
 * exclusiones, ordo vitae)
 *
 * Machinam IN-PROCESSU agit super FILE* effimeris (exemplar
 * sessionis - C6): epistulae scriptae in intra, currere, exitus
 * lecti ex extra. Radix vera repositorii ex RHUBARB_RADIX.
 *
 * Plagulae probationum = PHANTASMATA: viae sub ./lib/ quae in
 * disco NON exsistunt - didOpen textum buffer fert, discus numquam
 * legitur. Ita nec fixa/ (exclusa!) nec inquinatio repositorii.
 */

#include "legatus.h"
#include "praeparator.h"   /* probatio_capita_nova: inventio
                            * capitum novorum */
#include "tabula_dispersa.h"
#include "tabellarius.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utime.h>   /* probatio vigiliae: mtime binarii ficti */

interior constans character*
_radix (vacuum)
{
    constans character* r = getenv("RHUBARB_RADIX");

    redde r != NIHIL ? r : ".";
}

interior vacuum
_scribe (FILE* pl, Piscina* p, constans character* corpus)
{
    tabellarius_epistulam_scribere(pl, chorda_ex_literis(corpus, p));
}

interior TabellariusNuntius
_lege (FILE* pl, Piscina* p, b32* bene)
{
    b32 finitus = FALSUM;
    chorda corpus = tabellarius_epistulam_legere(pl, p, &finitus);
    TabellariusNuntius n;

    memset(&n, ZEPHYRUM, magnitudo(TabellariusNuntius));
    si (finitus)
    {
        *bene = FALSUM;
        redde n;
    }
    *bene = VERUM;
    redde tabellarius_nuntium_legere(corpus, p);
}

/* framing lineis (modus MCP, LEGATI) */
interior vacuum
_scribe_lineam (FILE* pl, Piscina* p, constans character* corpus)
{
    (vacuum)tabellarius_lineam_scribere(pl,
        chorda_ex_literis(corpus, p));
}

interior TabellariusNuntius
_lege_lineam (FILE* pl, Piscina* p, b32* bene)
{
    b32 finitus = FALSUM;
    chorda corpus = tabellarius_lineam_legere(pl, p, &finitus);
    TabellariusNuntius n;

    memset(&n, ZEPHYRUM, magnitudo(TabellariusNuntius));
    si (finitus)
    {
        *bene = FALSUM;
        redde n;
    }
    *bene = VERUM;
    redde tabellarius_nuntium_legere(corpus, p);
}

interior b32
_chorda_est (chorda c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (c.mensura == (i32)m && c.datum != NIHIL
        && memcmp(c.datum, litterae, m) == ZEPHYRUM) ? VERUM : FALSUM;
}

interior b32
_chorda_desinit (chorda c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (c.datum != NIHIL && (memoriae_index)c.mensura >= m
        && memcmp(c.datum + ((memoriae_index)c.mensura - m),
               litterae, m) == ZEPHYRUM) ? VERUM : FALSUM;
}

/* textus effectus instrumenti MCP: result.content[0].text */
interior chorda
_mcp_textus (TabellariusNuntius* n)
{
    redde json_ad_chorda(json_objectum_capere(
        json_tabulatum_obtinere(json_objectum_capere(
            json_objectum_capere(n->radix, "result"), "content"),
            ZEPHYRUM), "text"));
}

interior b32
_chorda_continet (chorda c, constans character* particula)
{
    memoriae_index m = strlen(particula);
    memoriae_index i;

    si (c.datum == NIHIL || (memoriae_index)c.mensura < m)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i + m <= (memoriae_index)c.mensura; i++)
    {
        si (memcmp(c.datum + i, particula, m) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* numerus diagnosticorum publicationis (-1 = non publicatio) */
interior s32
_diagnostica_numerus (TabellariusNuntius* n)
{
    JsonValor* lista;

    si (n->genus != TABELLARIUS_NUNTIATIO
        || !_chorda_est(n->methodus,
               "textDocument/publishDiagnostics")
        || n->params == NIHIL)
    {
        redde -I;
    }
    lista = json_objectum_capere(n->params, "diagnostics");
    si (lista == NIHIL || !json_est_tabulatum(lista))
    {
        redde -I;
    }
    redde (s32)json_tabulatum_numerus(lista);
}

interior s64
_error_codex (TabellariusNuntius* n)
{
    JsonValor* error_v;
    JsonValor* codex_v;

    si (n->radix == NIHIL)
    {
        redde ZEPHYRUM;
    }
    error_v = json_objectum_capere(n->radix, "error");
    si (error_v == NIHIL)
    {
        redde ZEPHYRUM;
    }
    codex_v = json_objectum_capere(error_v, "code");
    redde json_ad_integer(codex_v);
}

/* verum si aliquod diagnosticum publicationis particulam in
 * nuntio fert */
interior b32
_diagnosticum_continet (TabellariusNuntius* n,
    constans character* particula)
{
    JsonValor* lista;
    i32 k;
    memoriae_index pm = strlen(particula);

    si (n->genus != TABELLARIUS_NUNTIATIO || n->params == NIHIL)
    {
        redde FALSUM;
    }
    lista = json_objectum_capere(n->params, "diagnostics");
    si (lista == NIHIL || !json_est_tabulatum(lista))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < (i32)json_tabulatum_numerus(lista); k++)
    {
        JsonValor* d = json_tabulatum_obtinere(lista, k);
        JsonValor* nuntius_v = d != NIHIL
            ? json_objectum_capere(d, "message") : NIHIL;
        chorda nuntius;

        si (nuntius_v == NIHIL)
        {
            perge;
        }
        nuntius = json_ad_chorda(nuntius_v);
        si (nuntius.datum != NIHIL && nuntius.mensura >= (i32)pm)
        {
            i32 j;

            per (j = ZEPHYRUM; j + (i32)pm <= nuntius.mensura; j++)
            {
                si (memcmp(nuntius.datum + j, particula, pm)
                    == ZEPHYRUM)
                {
                    redde VERUM;
                }
            }
        }
    }
    redde FALSUM;
}

/* ==================================================
 * ORDO PLENUS: initialize -> didOpen violatio -> didChange purum
 * -> exclusum -> $/tacitum -> methodus ignota -> shutdown -> exit
 * ================================================== */

interior vacuum
probatio_ordo_plenus (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    character corpus[2048];
    LegatusConfiguratio cfg;
    s32 exitus;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    imprimere("--- Probans ordinem plenum ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    /* 1: initialize (utf-8 oblatum) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"rootUri\":\"file://%s\",\"capabilities\":"
        "{\"general\":{\"positionEncodings\":[\"utf-8\","
        "\"utf-16\"]}}}}", _radix());
    _scribe(intra, p, corpus);

    /* 2: initialized */
    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"method\":\"initialized\","
        "\"params\":{}}");

    /* 3: didOpen phantasma cum violatione (chorda -> integer) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didOpen\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma.c\",\"version\":1,"
        "\"languageId\":\"c\",\"text\":"
        "\"int x = \\\"salve\\\";\\n\"}}}", _radix());
    _scribe(intra, p, corpus);

    /* 4: didChange ad textum purum - undulae purgandae */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didChange\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma.c\",\"version\":2},"
        "\"contentChanges\":[{\"text\":\"int x;\\n\"}]}}",
        _radix());
    _scribe(intra, p, corpus);

    /* 5: didOpen plagulae EXCLUSAE (lib/uuid.c pinnata) cum textu
     * fracto - nihilominus vacua publicanda */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didOpen\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/uuid.c\",\"version\":1,"
        "\"languageId\":\"c\",\"text\":\"quisquiliae (((\"}}}",
        _radix());
    _scribe(intra, p, corpus);

    /* 6: $/nuntiatio - tacite omittenda */
    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"method\":\"$/setTrace\","
        "\"params\":{\"value\":\"off\"}}");

    /* 7: petitio methodi ignotae */
    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":2,"
        "\"method\":\"workspace/executeCommand\",\"params\":{}}");

    /* 8+9: shutdown + exit */
    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"shutdown\"}");
    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");

    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    exitus = legatus_currere(intra, extra, &cfg);
    CREDO_VERUM(exitus == ZEPHYRUM);   /* exit post shutdown */

    rewind(extra);

    /* R1: responsio initialize */
    n = _lege(extra, p, &bene);
    CREDO_VERUM(bene);
    CREDO_VERUM(n.genus == TABELLARIUS_RESPONSUM);
    CREDO_VERUM(json_ad_integer(n.id) == I);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");
        JsonValor* caps;
        JsonValor* v;

        CREDO_VERUM(resultatum != NIHIL);
        caps = json_objectum_capere(resultatum, "capabilities");
        CREDO_VERUM(caps != NIHIL);
        v = json_objectum_capere(caps, "positionEncoding");
        CREDO_VERUM(v != NIHIL
            && _chorda_est(json_ad_chorda(v), "utf-8"));
        v = json_objectum_capere(caps, "textDocumentSync");
        CREDO_VERUM(v != NIHIL);
        CREDO_VERUM(json_ad_integer(
            json_objectum_capere(v, "change")) == I);
        CREDO_VERUM(json_ad_boolean(
            json_objectum_capere(v, "save")) == VERUM);
        v = json_objectum_capere(caps, "hoverProvider");
        CREDO_VERUM(v != NIHIL && json_ad_boolean(v) == VERUM);
        v = json_objectum_capere(caps, "documentSymbolProvider");
        CREDO_VERUM(v != NIHIL && json_ad_boolean(v) == VERUM);
        v = json_objectum_capere(resultatum, "serverInfo");
        CREDO_VERUM(v != NIHIL && _chorda_est(json_ad_chorda(
            json_objectum_capere(v, "name")), "legatus"));
    }

    /* R2: publicatio violationis - saltem una, severitas 1,
     * linea 0, extensio vera (finis > initium) */
    n = _lege(extra, p, &bene);
    CREDO_VERUM(bene);
    {
        s32 numerus = _diagnostica_numerus(&n);
        JsonValor* lista;
        b32 violatio_inventa = FALSUM;
        s32 i;

        CREDO_VERUM(numerus >= (s32)I);
        CREDO_VERUM(json_ad_integer(json_objectum_capere(n.params,
            "version")) == I);
        lista = json_objectum_capere(n.params, "diagnostics");
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            JsonValor* diag = json_tabulatum_obtinere(lista,
                (i32)i);
            JsonValor* regio = json_objectum_capere(diag, "range");
            JsonValor* initium = json_objectum_capere(regio,
                "start");
            JsonValor* finis_r = json_objectum_capere(regio, "end");

            si (json_ad_integer(json_objectum_capere(diag,
                    "severity")) == I
                && json_ad_integer(json_objectum_capere(initium,
                       "line")) == ZEPHYRUM
                && json_ad_integer(json_objectum_capere(finis_r,
                       "character"))
                    > json_ad_integer(json_objectum_capere(initium,
                       "character")))
            {
                violatio_inventa = VERUM;
            }
            CREDO_VERUM(_chorda_est(json_ad_chorda(
                json_objectum_capere(diag, "source")), "silva"));
        }
        CREDO_VERUM(violatio_inventa);
    }

    /* R3: publicatio post didChange purum - VACUA (undulae
     * purgatae), versio 2 */
    n = _lege(extra, p, &bene);
    CREDO_VERUM(bene);
    CREDO_VERUM(_diagnostica_numerus(&n) == (s32)ZEPHYRUM);
    CREDO_VERUM(json_ad_integer(json_objectum_capere(n.params,
        "version")) == II);

    /* R4: plagula exclusa - vacua quamvis textus fractus */
    n = _lege(extra, p, &bene);
    CREDO_VERUM(bene);
    CREDO_VERUM(_diagnostica_numerus(&n) == (s32)ZEPHYRUM);

    /* R5: methodus ignota -> -32601 (post $/ tacitum - ordo
     * probat $/ nihil emisisse) */
    n = _lege(extra, p, &bene);
    CREDO_VERUM(bene);
    CREDO_VERUM(n.genus == TABELLARIUS_RESPONSUM);
    CREDO_VERUM(json_ad_integer(n.id) == II);
    CREDO_VERUM(_error_codex(&n) == -32601);

    /* R6: shutdown -> result null */
    n = _lege(extra, p, &bene);
    CREDO_VERUM(bene);
    CREDO_VERUM(n.genus == TABELLARIUS_RESPONSUM);
    CREDO_VERUM(json_ad_integer(n.id) == III);
    CREDO_VERUM(json_est_nullum(json_objectum_capere(n.radix,
        "result")));

    /* nihil ultra */
    {
        b32 finitus = FALSUM;

        (vacuum)tabellarius_epistulam_legere(extra, p, &finitus);
        CREDO_VERUM(finitus);
    }

    fclose(intra);
    fclose(extra);
}

interior b32
_resultatum_nullum (TabellariusNuntius* n)
{
    JsonValor* resultatum;

    si (n->radix == NIHIL)
    {
        redde FALSUM;
    }
    resultatum = json_objectum_capere(n->radix, "result");
    redde (resultatum != NIHIL && json_est_nullum(resultatum))
        ? VERUM : FALSUM;
}

interior b32
_hover_valor_est (TabellariusNuntius* n,
    constans character* litterae)
{
    JsonValor* resultatum;
    JsonValor* contenta;

    si (n->radix == NIHIL)
    {
        redde FALSUM;
    }
    resultatum = json_objectum_capere(n->radix, "result");
    si (resultatum == NIHIL || json_est_nullum(resultatum))
    {
        redde FALSUM;
    }
    contenta = json_objectum_capere(resultatum, "contents");
    si (contenta == NIHIL)
    {
        redde FALSUM;
    }
    redde _chorda_est(json_ad_chorda(
        json_objectum_capere(contenta, "value")), litterae);
}

/* valor hoveris particulam continet? (vicinitas: indicium, non
 * aequalitas exacta) */
interior b32
_hover_valor_continet (TabellariusNuntius* n,
    constans character* particula)
{
    JsonValor* resultatum;
    JsonValor* contenta;
    chorda valor;
    memoriae_index pm = strlen(particula);
    i32 j;

    si (n->radix == NIHIL)
    {
        redde FALSUM;
    }
    resultatum = json_objectum_capere(n->radix, "result");
    si (resultatum == NIHIL || json_est_nullum(resultatum))
    {
        redde FALSUM;
    }
    contenta = json_objectum_capere(resultatum, "contents");
    si (contenta == NIHIL)
    {
        redde FALSUM;
    }
    valor = json_ad_chorda(json_objectum_capere(contenta, "value"));
    si (valor.datum == NIHIL || valor.mensura < (i32)pm)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j + (i32)pm <= valor.mensura; j++)
    {
        si (memcmp(valor.datum + j, particula, pm) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* definitio: Location unum AUT tabulatum (gradus corporis v0.1b)
 * -> primum */
interior JsonValor*
_sedes_prima (TabellariusNuntius* n)
{
    JsonValor* resultatum;

    si (n->radix == NIHIL)
    {
        redde NIHIL;
    }
    resultatum = json_objectum_capere(n->radix, "result");
    si (resultatum == NIHIL || json_est_nullum(resultatum))
    {
        redde NIHIL;
    }
    si (json_est_tabulatum(resultatum))
    {
        redde json_tabulatum_obtinere(resultatum, ZEPHYRUM);
    }
    redde resultatum;
}

/* ==================================================
 * HOVER + DOCUMENTSYMBOL (utf-8): sedes usus, litterale, nihil,
 * symbola plagulae
 * ================================================== */

interior vacuum
probatio_hover_symbola (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    character corpus[2048];
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    imprimere("--- Probans hover + documentSymbol ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"rootUri\":\"file://%s\",\"capabilities\":"
        "{\"general\":{\"positionEncodings\":[\"utf-8\"]}}}}",
        _radix());
    _scribe(intra, p, corpus);

    /* functio: usus parametrorum + localium (nexus = sedes USUS -
     * hover super nominibus declaratis nullum reddit, parca) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didOpen\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\",\"version\":1,"
        "\"languageId\":\"c\",\"text\":"
        "\"int probatio_functio(int a)\\n{\\n"
        "    int b = a + 1;\\n    return b;\\n}\\n"
        "int probatio_vocans(void)\\n{\\n"
        "    return probatio_functio(7);\\n}\\n\"}}}", _radix());
    _scribe(intra, p, corpus);

    /* hover VOCATI (linea 7, char 15) - signatura functionis */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":8,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":7,\"character\":15}}}", _radix());
    _scribe(intra, p, corpus);

    /* hover usus 'a' (linea 2, char 12) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":2,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":2,\"character\":12}}}", _radix());
    _scribe(intra, p, corpus);

    /* hover litterale '1' (linea 2, char 16) - ramus typi */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":3,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":2,\"character\":16}}}", _radix());
    _scribe(intra, p, corpus);

    /* hover usus 'b' (linea 3, char 11) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":4,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":3,\"character\":11}}}", _radix());
    _scribe(intra, p, corpus);

    /* hover nusquam (ultra textum) - olim nullum, nunc VICINITAS
     * (sedes proximae; politio post-debrief) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":5,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":9,\"character\":0}}}", _radix());
    _scribe(intra, p, corpus);

    /* documentSymbol */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":6,"
        "\"method\":\"textDocument/documentSymbol\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"}}}", _radix());
    _scribe(intra, p, corpus);

    /* hover nominis DECLARATI 'b' (linea 2, char 8) - parca soluta */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":9,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":2,\"character\":8}}}", _radix());
    _scribe(intra, p, corpus);

    /* definitio ex USU parametri 'a' (linea 2 char 12) ->
     * declaratio {0,25} */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":10,"
        "\"method\":\"textDocument/definition\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":2,\"character\":12}}}", _radix());
    _scribe(intra, p, corpus);

    /* definitio ex VOCATIONE (linea 7 char 15) -> nomen {0,4} */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":11,"
        "\"method\":\"textDocument/definition\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":7,\"character\":15}}}", _radix());
    _scribe(intra, p, corpus);

    /* references parametri 'a' (localis - plagula sola, sine
     * declaratione) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":12,"
        "\"method\":\"textDocument/references\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":2,\"character\":12},"
        "\"context\":{\"includeDeclaration\":false}}}", _radix());
    _scribe(intra, p, corpus);

    /* references functionis (globalis, cum declaratione - ordines
     * ex SUPERPOSITIONE: phantasma in tsv numquam fuit) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":13,"
        "\"method\":\"textDocument/references\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":7,\"character\":15},"
        "\"context\":{\"includeDeclaration\":true}}}", _radix());
    _scribe(intra, p, corpus);

    /* workspaceSymbol - quaestio exacta */
    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":14,"
        "\"method\":\"workspace/symbol\",\"params\":"
        "{\"query\":\"probatio_functio\"}}");

    /* prepareCallHierarchy + incomingCalls (via documenti aperti) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":15,"
        "\"method\":\"textDocument/prepareCallHierarchy\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":7,\"character\":15}}}", _radix());
    _scribe(intra, p, corpus);
    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":16,"
        "\"method\":\"callHierarchy/incomingCalls\",\"params\":"
        "{\"item\":{\"name\":\"probatio_functio\"}}}");

    /* outgoingCalls: quas functiones probatio_vocans vocat
     * (inversum - item sine range: quaestio tituli in extentis) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":17,"
        "\"method\":\"callHierarchy/outgoingCalls\",\"params\":"
        "{\"item\":{\"name\":\"probatio_vocans\",\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"}}}", _radix());
    _scribe(intra, p, corpus);

    /* implementation = alias definitionis (nona operationis, 9/9) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":18,"
        "\"method\":\"textDocument/implementation\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_f.c\"},"
        "\"position\":{\"line\":7,\"character\":15}}}", _radix());
    _scribe(intra, p, corpus);

    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":7,\"method\":\"shutdown\"}");
    _scribe(intra, p, "{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");

    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);
    rewind(extra);

    n = _lege(extra, p, &bene);   /* initialize */
    CREDO_VERUM(bene);
    n = _lege(extra, p, &bene);   /* publicatio (pura) */
    CREDO_VERUM(bene);
    CREDO_VERUM(_diagnostica_numerus(&n) == (s32)ZEPHYRUM);

    n = _lege(extra, p, &bene);   /* hover vocati: SIGNATURA */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_est(&n,
        "probatio_functio : integer(integer)"));

    n = _lege(extra, p, &bene);   /* hover a */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_est(&n, "a : integer"));

    n = _lege(extra, p, &bene);   /* hover 1 */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_est(&n, "integer"));

    n = _lege(extra, p, &bene);   /* hover b */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_est(&n, "b : integer"));

    n = _lege(extra, p, &bene);   /* hover nusquam -> vicinitas */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_continet(&n, "sedes proximae"));

    n = _lege(extra, p, &bene);   /* documentSymbol */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");
        JsonValor* symbolum_v;

        CREDO_VERUM(resultatum != NIHIL
            && json_est_tabulatum(resultatum));
        CREDO_VERUM(json_tabulatum_numerus(resultatum) == II);
        CREDO_VERUM(_chorda_est(json_ad_chorda(
            json_objectum_capere(json_tabulatum_obtinere(
                resultatum, I), "name")), "probatio_vocans"));
        symbolum_v = json_tabulatum_obtinere(resultatum, ZEPHYRUM);
        CREDO_VERUM(_chorda_est(json_ad_chorda(
            json_objectum_capere(symbolum_v, "name")),
            "probatio_functio"));
        CREDO_VERUM(json_ad_integer(json_objectum_capere(
            symbolum_v, "kind")) == XII);
        {
            JsonValor* sedes_v = json_objectum_capere(symbolum_v,
                "location");
            JsonValor* regio = json_objectum_capere(sedes_v,
                "range");

            CREDO_VERUM(json_ad_integer(json_objectum_capere(
                json_objectum_capere(regio, "start"), "line"))
                == ZEPHYRUM);
        }
    }

    n = _lege(extra, p, &bene);   /* hover DECLARATI b */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_est(&n, "b : integer"));

    n = _lege(extra, p, &bene);   /* definitio parametri a */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");
        JsonValor* initium;

        CREDO_VERUM(resultatum != NIHIL
            && !json_est_nullum(resultatum));
        initium = json_objectum_capere(json_objectum_capere(
            resultatum, "range"), "start");
        CREDO_VERUM(json_ad_integer(json_objectum_capere(initium,
            "line")) == ZEPHYRUM);
        CREDO_VERUM(json_ad_integer(json_objectum_capere(initium,
            "character")) == XXV);
    }

    n = _lege(extra, p, &bene);   /* definitio ex vocatione */
    CREDO_VERUM(bene);
    {
        /* corpus primum (v0.1b) - phantasma per SUPERPOSITIONEM
         * in indicem intravit (in tsv numquam fuit!) */
        JsonValor* sedes_v = _sedes_prima(&n);
        JsonValor* initium;

        CREDO_VERUM(sedes_v != NIHIL);
        initium = json_objectum_capere(json_objectum_capere(
            sedes_v, "range"), "start");
        CREDO_VERUM(json_ad_integer(json_objectum_capere(initium,
            "line")) == ZEPHYRUM);
        CREDO_VERUM(json_ad_integer(json_objectum_capere(initium,
            "character")) == IV);
    }

    n = _lege(extra, p, &bene);   /* references 'a' (localis) */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");

        CREDO_VERUM(resultatum != NIHIL
            && json_est_tabulatum(resultatum));
        CREDO_VERUM(json_tabulatum_numerus(resultatum) == I);
    }

    n = _lege(extra, p, &bene);   /* references functionis:
                                   * sedes + usus = 2 (superpositio!) */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");

        CREDO_VERUM(resultatum != NIHIL
            && json_est_tabulatum(resultatum));
        CREDO_VERUM(json_tabulatum_numerus(resultatum) == II);
    }

    n = _lege(extra, p, &bene);   /* workspaceSymbol */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");
        JsonValor* primum;

        CREDO_VERUM(resultatum != NIHIL
            && json_est_tabulatum(resultatum)
            && json_tabulatum_numerus(resultatum) >= I);
        primum = json_tabulatum_obtinere(resultatum, ZEPHYRUM);
        CREDO_VERUM(_chorda_est(json_ad_chorda(
            json_objectum_capere(primum, "name")),
            "probatio_functio"));
    }

    n = _lege(extra, p, &bene);   /* prepareCallHierarchy */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");

        CREDO_VERUM(resultatum != NIHIL
            && json_est_tabulatum(resultatum)
            && json_tabulatum_numerus(resultatum) == I);
    }

    n = _lege(extra, p, &bene);   /* incomingCalls: probatio_vocans */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");
        JsonValor* introitus;
        JsonValor* ab;

        CREDO_VERUM(resultatum != NIHIL
            && json_est_tabulatum(resultatum)
            && json_tabulatum_numerus(resultatum) == I);
        introitus = json_tabulatum_obtinere(resultatum, ZEPHYRUM);
        ab = json_objectum_capere(introitus, "from");
        CREDO_VERUM(_chorda_est(json_ad_chorda(
            json_objectum_capere(ab, "name")), "probatio_vocans"));
        CREDO_VERUM(json_ad_integer(json_objectum_capere(
            json_objectum_capere(json_tabulatum_obtinere(
                json_objectum_capere(introitus, "fromRanges"),
                ZEPHYRUM), "start"), "line")) == VII);
    }

    n = _lege(extra, p, &bene);   /* outgoingCalls: probatio_functio */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");
        JsonValor* introitus;
        JsonValor* ad;

        CREDO_VERUM(resultatum != NIHIL
            && json_est_tabulatum(resultatum)
            && json_tabulatum_numerus(resultatum) == I);
        introitus = json_tabulatum_obtinere(resultatum, ZEPHYRUM);
        ad = json_objectum_capere(introitus, "to");
        CREDO_VERUM(_chorda_est(json_ad_chorda(
            json_objectum_capere(ad, "name")), "probatio_functio"));
        CREDO_VERUM(_chorda_desinit(json_ad_chorda(
            json_objectum_capere(ad, "uri")),
            "legatus_phantasma_f.c"));
        /* sedes vocati: definitio linea 0 (0-basata) */
        CREDO_VERUM(json_ad_integer(json_objectum_capere(
            json_objectum_capere(json_objectum_capere(ad, "range"),
                "start"), "line")) == (s64)ZEPHYRUM);
        /* sedes vocationis: linea VII (0-basata) */
        CREDO_VERUM(json_ad_integer(json_objectum_capere(
            json_objectum_capere(json_tabulatum_obtinere(
                json_objectum_capere(introitus, "fromRanges"),
                ZEPHYRUM), "start"), "line")) == VII);
    }

    n = _lege(extra, p, &bene);   /* implementation = definitio */
    CREDO_VERUM(bene);
    {
        JsonValor* sedes_v = _sedes_prima(&n);
        JsonValor* initium;

        CREDO_VERUM(sedes_v != NIHIL);
        initium = json_objectum_capere(json_objectum_capere(
            sedes_v, "range"), "start");
        CREDO_VERUM(json_ad_integer(json_objectum_capere(initium,
            "line")) == ZEPHYRUM);
        CREDO_VERUM(json_ad_integer(json_objectum_capere(initium,
            "character")) == IV);
    }

    n = _lege(extra, p, &bene);   /* shutdown */
    CREDO_VERUM(bene);
    CREDO_VERUM(_resultatum_nullum(&n));

    fclose(intra);
    fclose(extra);
}

/* ==================================================
 * DEFINITIO IN CAPUT (saltus trans-plagularis primus): usus
 * functionis capitis -> Location in include/piscina.h
 * ================================================== */

interior vacuum
probatio_definitio_capitis (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    character corpus[1024];
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    imprimere("--- Probans definitionem in caput ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"rootUri\":\"file://%s\",\"capabilities\":"
        "{\"general\":{\"positionEncodings\":[\"utf-8\"]}}}}",
        _radix());
    _scribe(intra, p, corpus);

    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didOpen\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_d.c\",\"version\":1,"
        "\"languageId\":\"c\",\"text\":"
        "\"#include \\\"piscina.h\\\"\\n"
        "void* g(Piscina* p) { return piscina_allocare(p, 4); }"
        "\\n\"}}}", _radix());
    _scribe(intra, p, corpus);

    /* definitio ex usu piscina_allocare (linea 1 char 30) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":2,"
        "\"method\":\"textDocument/definition\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_d.c\"},"
        "\"position\":{\"line\":1,\"character\":30}}}", _radix());
    _scribe(intra, p, corpus);

    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"shutdown\"}");
    _scribe(intra, p, "{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");

    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);
    rewind(extra);

    n = _lege(extra, p, &bene);   /* initialize */
    CREDO_VERUM(bene);
    n = _lege(extra, p, &bene);   /* publicatio (pura) */
    CREDO_VERUM(bene);
    CREDO_VERUM(_diagnostica_numerus(&n) == (s32)ZEPHYRUM);

    n = _lege(extra, p, &bene);   /* definitio -> CORPUS .c primum
                                   * (v0.1b), caput in lista */
    CREDO_VERUM(bene);
    {
        JsonValor* sedes_v = _sedes_prima(&n);
        chorda uri;
        /* cum tsv: corpus .c primum; sine tsv (arbor recens):
         * gradus declarationis solus -> caput */
        FILE* tsv = fopen("build/nexus.tsv", "rb");
        constans character* suffixum = tsv != NIHIL
            ? "lib/piscina.c" : "include/piscina.h";
        memoriae_index m = strlen(suffixum);

        si (tsv != NIHIL)
        {
            fclose(tsv);
        }

        CREDO_VERUM(sedes_v != NIHIL);
        uri = json_ad_chorda(json_objectum_capere(sedes_v, "uri"));
        CREDO_VERUM(uri.mensura >= (i32)m
            && memcmp(uri.datum + (uri.mensura - (i32)m), suffixum,
                   m) == ZEPHYRUM);
        CREDO_VERUM(json_ad_integer(json_objectum_capere(
            json_objectum_capere(json_objectum_capere(sedes_v,
                "range"), "start"), "line")) > ZEPHYRUM);
    }

    fclose(intra);
    fclose(extra);
}

/* ==================================================
 * UTF-16: remappa exiens (columnae diagnostici post BMP-signum)
 * + iniens (positio hover post signum)
 * ================================================== */

interior vacuum
probatio_utf16 (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    character corpus[2048];
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    imprimere("--- Probans remappam utf-16 ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    /* initialize SINE positionEncodings -> utf-16 obligatorius */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"rootUri\":\"file://%s\"}}", _radix());
    _scribe(intra, p, corpus);

    /* linea 0: violatio post signum BMP (2 octeti, 1 unitas);
     * linea 1: usus 'a' post signum - remappa iniens */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didOpen\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_u.c\",\"version\":1,"
        "\"languageId\":\"c\",\"text\":"
        "\"/* \xC2\xA7 */ int x = \\\"s\\\";\\n"
        "/* \xC2\xA7 */ int f(int a) { return a; }\\n\"}}}",
        _radix());
    _scribe(intra, p, corpus);

    /* hover 'a' in usu: octetum 31, unitas utf-16 = 30 */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":2,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_u.c\"},"
        "\"position\":{\"line\":1,\"character\":30}}}", _radix());
    _scribe(intra, p, corpus);

    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"shutdown\"}");
    _scribe(intra, p, "{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");

    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);
    rewind(extra);

    n = _lege(extra, p, &bene);   /* initialize: utf-16 */
    CREDO_VERUM(bene);
    {
        JsonValor* caps = json_objectum_capere(
            json_objectum_capere(n.radix, "result"),
            "capabilities");

        CREDO_VERUM(_chorda_est(json_ad_chorda(
            json_objectum_capere(caps, "positionEncoding")),
            "utf-16"));
    }

    n = _lege(extra, p, &bene);   /* publicatio: columnae remappatae */
    CREDO_VERUM(bene);
    {
        JsonValor* lista;
        JsonValor* diag;
        JsonValor* regio;

        CREDO_VERUM(_diagnostica_numerus(&n) == (s32)I);
        lista = json_objectum_capere(n.params, "diagnostics");
        diag = json_tabulatum_obtinere(lista, ZEPHYRUM);
        regio = json_objectum_capere(diag, "range");
        CREDO_VERUM(json_ad_integer(json_objectum_capere(
            json_objectum_capere(regio, "start"), "line"))
            == ZEPHYRUM);
        /* octetum 17 - signum bi-octetum = unitas 16 */
        CREDO_VERUM(json_ad_integer(json_objectum_capere(
            json_objectum_capere(regio, "start"), "character"))
            == XVI);
        /* finis: octetum 20 -> unitas 19 */
        CREDO_VERUM(json_ad_integer(json_objectum_capere(
            json_objectum_capere(regio, "end"), "character"))
            == XIX);
    }

    n = _lege(extra, p, &bene);   /* hover a (remappa iniens) */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_est(&n, "a : integer"));

    n = _lege(extra, p, &bene);   /* shutdown */
    CREDO_VERUM(bene);
    CREDO_VERUM(_resultatum_nullum(&n));

    fclose(intra);
    fclose(extra);
}

/* ==================================================
 * SYNTAXIS POSITA (chunk D): nodus erroris post declarationem
 * validam -> linea 1 (degenerata daret 0)
 * ================================================== */

interior vacuum
probatio_syntaxis_posita (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    character corpus[1024];
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    imprimere("--- Probans syntaxim positam ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"rootUri\":\"file://%s\",\"capabilities\":"
        "{\"general\":{\"positionEncodings\":[\"utf-8\"]}}}}",
        _radix());
    _scribe(intra, p, corpus);

    /* linea 0 valida; linea 1 fracta - positio discriminat */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didOpen\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_s.c\",\"version\":1,"
        "\"languageId\":\"c\",\"text\":"
        "\"int x;\\nint f( {\\n\"}}}", _radix());
    _scribe(intra, p, corpus);

    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"shutdown\"}");
    _scribe(intra, p, "{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");

    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);
    rewind(extra);

    n = _lege(extra, p, &bene);   /* initialize */
    CREDO_VERUM(bene);
    n = _lege(extra, p, &bene);   /* publicatio */
    CREDO_VERUM(bene);
    {
        s32 numerus = _diagnostica_numerus(&n);
        JsonValor* lista = json_objectum_capere(n.params,
            "diagnostics");
        b32 posita_inventa = FALSUM;
        s32 i;

        CREDO_VERUM(numerus >= (s32)I);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            JsonValor* diag = json_tabulatum_obtinere(lista,
                (i32)i);
            chorda nuntius = json_ad_chorda(
                json_objectum_capere(diag, "message"));

            si (nuntius.mensura >= (i32)VIII
                && memcmp(nuntius.datum, "syntaxis", VIII)
                    == ZEPHYRUM)
            {
                JsonValor* initium = json_objectum_capere(
                    json_objectum_capere(diag, "range"), "start");

                /* POSITA: linea 1, non degenerata 0:0 */
                si (json_ad_integer(json_objectum_capere(initium,
                        "line")) == I)
                {
                    posita_inventa = VERUM;
                }
            }
        }
        CREDO_VERUM(posita_inventa);
    }

    fclose(intra);
    fclose(extra);
}

/* ==================================================
 * LAMINA .h (chunk D): didSave capitis -> reaedificatio ordine
 * C11 + re-publicatio documentorum apertorum; didSave .c tacitum
 * ================================================== */

interior vacuum
probatio_caput_servatum (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    character corpus[1024];
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    imprimere("--- Probans caput servatum ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"rootUri\":\"file://%s\",\"capabilities\":"
        "{\"general\":{\"positionEncodings\":[\"utf-8\"]}}}}",
        _radix());
    _scribe(intra, p, corpus);

    /* documentum apertum cum violatione (1 ordo) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didOpen\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_h.c\",\"version\":1,"
        "\"languageId\":\"c\",\"text\":"
        "\"int x = \\\"salve\\\";\\n\"}}}", _radix());
    _scribe(intra, p, corpus);

    /* didSave capitis VERI (nulla scriptura disci - nuntiatio
     * sola!) -> reaedificatio + re-publicatio eiusdem iudicii */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didSave\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/piscina.h\"}}}", _radix());
    _scribe(intra, p, corpus);

    /* didSave plagulae .c - NIHIL reaedificatur, nihil emittitur */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didSave\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_h.c\"}}}", _radix());
    _scribe(intra, p, corpus);

    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"shutdown\"}");
    _scribe(intra, p, "{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");

    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);
    rewind(extra);

    n = _lege(extra, p, &bene);   /* initialize */
    CREDO_VERUM(bene);

    n = _lege(extra, p, &bene);   /* publicatio prima: 1 ordo */
    CREDO_VERUM(bene);
    CREDO_VERUM(_diagnostica_numerus(&n) == (s32)I);

    /* post didSave capitis: RE-publicatio, idem iudicium, eadem
     * versio (superstes reaedificationis = decipula C11 vitata) */
    n = _lege(extra, p, &bene);
    CREDO_VERUM(bene);
    CREDO_VERUM(_diagnostica_numerus(&n) == (s32)I);
    CREDO_VERUM(json_ad_integer(json_objectum_capere(n.params,
        "version")) == I);

    /* didSave .c NIHIL emisit: proximus exitus = shutdown */
    n = _lege(extra, p, &bene);
    CREDO_VERUM(bene);
    CREDO_VERUM(n.genus == TABELLARIUS_RESPONSUM);
    CREDO_VERUM(json_ad_integer(n.id) == II);
    CREDO_VERUM(_resultatum_nullum(&n));

    fclose(intra);
    fclose(extra);
}

/* ==================================================
 * ANTE INITIALIZE: petitio -> -32002; EOF sine exit -> 1
 * ================================================== */

interior vacuum
probatio_ante_initium (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    LegatusConfiguratio cfg;
    s32 exitus;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    imprimere("--- Probans ante initium ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":9,"
        "\"method\":\"textDocument/hover\",\"params\":{}}");
    /* etiam shutdown ante initialize = -32002 */
    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":10,\"method\":\"shutdown\"}");
    rewind(intra);

    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    exitus = legatus_currere(intra, extra, &cfg);
    CREDO_VERUM(exitus == I);   /* EOF sine exit = abnormis */

    rewind(extra);
    n = _lege(extra, p, &bene);
    CREDO_VERUM(bene);
    CREDO_VERUM(json_ad_integer(n.id) == IX);
    CREDO_VERUM(_error_codex(&n) == -32002);
    n = _lege(extra, p, &bene);
    CREDO_VERUM(bene);
    CREDO_VERUM(json_ad_integer(n.id) == X);
    CREDO_VERUM(_error_codex(&n) == -32002);

    fclose(intra);
    fclose(extra);
}

/* ==================================================
 * FLUXUS VACUUS: exitus 1 statim
 * ================================================== */

interior vacuum
probatio_fluxus_vacuus (vacuum)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    LegatusConfiguratio cfg;

    imprimere("--- Probans fluxum vacuum ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == I);
    fclose(intra);
    fclose(extra);
}

/* ==================================================
 * QUISQUILIAE: analysis fracta -> -32700 cum id nullo
 * ================================================== */

interior vacuum
probatio_quisquiliae (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    imprimere("--- Probans quisquilias ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    _scribe(intra, p, "hoc non est json");
    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    (vacuum)legatus_currere(intra, extra, &cfg);

    rewind(extra);
    n = _lege(extra, p, &bene);
    CREDO_VERUM(bene);
    CREDO_VERUM(_error_codex(&n) == -32700);
    CREDO_VERUM(json_est_nullum(json_objectum_capere(n.radix,
        "id")));

    fclose(intra);
    fclose(extra);
}

/* ================================================== */

/* ==================================================
 * MACRA (v0.2): hover invocationis (gradus originis VINCIT typum
 * expansionis), hover directivae (verbum crudum), definitio,
 * references, workspaceSymbol, documentSymbol cum #define
 * ================================================== */

interior vacuum
probatio_macra (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    character corpus[2048];
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    imprimere("--- Probans macra (index v0.2) ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"rootUri\":\"file://%s\",\"capabilities\":"
        "{\"general\":{\"positionEncodings\":[\"utf-8\"]}}}}",
        _radix());
    _scribe(intra, p, corpus);

    /* phantasma: macra obiecti et functionis, in EADEM plagula
     * definita (lexemata corporis = fons princeps - casus durus) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didOpen\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_m.c\",\"version\":1,"
        "\"languageId\":\"c\",\"text\":"
        "\"#define GRADUS 7\\n"
        "#define DUPLICARE(x) ((x) * 2)\\n"
        "int probatio_macro_functio(int a)\\n{\\n"
        "    return DUPLICARE(a) + GRADUS;\\n}\\n"
        "#define SUMMA(x, y) \\\\\\n"
        "    ((x) + \\\\\\n"
        "     (y))\\n"
        "int probatio_macro_summa(int a)\\n{\\n"
        "    return SUMMA(a, 2);\\n}\\n\"}}}", _radix());
    _scribe(intra, p, corpus);

    /* hover usus GRADUS (linea 4, char 26) - expansio est litterale
     * 7; invocatio scripta vincere debet */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":20,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_m.c\"},"
        "\"position\":{\"line\":4,\"character\":26}}}", _radix());
    _scribe(intra, p, corpus);

    /* hover usus DUPLICARE (linea 4, char 11) - functionis */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":21,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_m.c\"},"
        "\"position\":{\"line\":4,\"character\":11}}}", _radix());
    _scribe(intra, p, corpus);

    /* hover in DIRECTIVA ipsa (linea 0, char 8) - verbum crudum
     * (directivae consumptae, nulla lexemata expansa ibi) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":22,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_m.c\"},"
        "\"position\":{\"line\":0,\"character\":8}}}", _radix());
    _scribe(intra, p, corpus);

    /* definitio ex usu GRADUS -> linea 0 */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":23,"
        "\"method\":\"textDocument/definition\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_m.c\"},"
        "\"position\":{\"line\":4,\"character\":26}}}", _radix());
    _scribe(intra, p, corpus);

    /* references GRADUS cum declaratione: sedes + usus = 2 */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":24,"
        "\"method\":\"textDocument/references\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_m.c\"},"
        "\"position\":{\"line\":4,\"character\":26},"
        "\"context\":{\"includeDeclaration\":true}}}", _radix());
    _scribe(intra, p, corpus);

    /* workspaceSymbol - macro inveniendum, species Constant */
    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":25,"
        "\"method\":\"workspace/symbol\",\"params\":"
        "{\"query\":\"GRADUS\"}}");

    /* documentSymbol - functio + duo macra */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":26,"
        "\"method\":\"textDocument/documentSymbol\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_m.c\"}}}", _radix());
    _scribe(intra, p, corpus);

    /* hover usus SUMMA (linea 11, char 11) - corpus MULTILINEARE
     * per extenta vistae (nulla scansio '\') */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":28,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_m.c\"},"
        "\"position\":{\"line\":11,\"character\":11}}}", _radix());
    _scribe(intra, p, corpus);

    /* hover in '{' (linea 3, char 0) - nihil sub cursore ->
     * VICINITAS: sedes proximae eiusdem viae (politio
     * post-debrief) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":29,"
        "\"method\":\"textDocument/hover\",\"params\":"
        "{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_m.c\"},"
        "\"position\":{\"line\":3,\"character\":0}}}", _radix());
    _scribe(intra, p, corpus);

    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":27,\"method\":\"shutdown\"}");
    _scribe(intra, p, "{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");

    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);
    rewind(extra);

    n = _lege(extra, p, &bene);   /* initialize */
    CREDO_VERUM(bene);
    n = _lege(extra, p, &bene);   /* publicatio (pura) */
    CREDO_VERUM(bene);
    CREDO_VERUM(_diagnostica_numerus(&n) == (s32)ZEPHYRUM);

    n = _lege(extra, p, &bene);   /* hover GRADUS */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_est(&n, "#define GRADUS 7"));

    n = _lege(extra, p, &bene);   /* hover DUPLICARE */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_est(&n,
        "#define DUPLICARE(x) ((x) * 2)"));

    n = _lege(extra, p, &bene);   /* hover directivae */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_est(&n, "#define GRADUS 7"));

    n = _lege(extra, p, &bene);   /* definitio -> linea 0 */
    CREDO_VERUM(bene);
    {
        JsonValor* sedes_v = _sedes_prima(&n);
        JsonValor* initium;

        CREDO_VERUM(sedes_v != NIHIL);
        initium = json_objectum_capere(json_objectum_capere(
            sedes_v, "range"), "start");
        CREDO_VERUM(json_ad_integer(json_objectum_capere(initium,
            "line")) == ZEPHYRUM);
        CREDO_VERUM(json_ad_integer(json_objectum_capere(initium,
            "character")) == ZEPHYRUM);
    }

    n = _lege(extra, p, &bene);   /* references: sedes + usus */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");

        CREDO_VERUM(resultatum != NIHIL
            && json_est_tabulatum(resultatum));
        CREDO_VERUM(json_tabulatum_numerus(resultatum) == II);
    }

    n = _lege(extra, p, &bene);   /* workspaceSymbol */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");
        JsonValor* primum;

        CREDO_VERUM(resultatum != NIHIL
            && json_est_tabulatum(resultatum)
            && json_tabulatum_numerus(resultatum) >= I);
        primum = json_tabulatum_obtinere(resultatum, ZEPHYRUM);
        CREDO_VERUM(_chorda_est(json_ad_chorda(
            json_objectum_capere(primum, "name")), "GRADUS"));
        CREDO_VERUM(json_ad_integer(json_objectum_capere(primum,
            "kind")) == XIV);
    }

    n = _lege(extra, p, &bene);   /* documentSymbol: II functiones
                                   * + III macra = V */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");
        JsonValor* macro_v;

        CREDO_VERUM(resultatum != NIHIL
            && json_est_tabulatum(resultatum));
        CREDO_VERUM(json_tabulatum_numerus(resultatum) == V);
        macro_v = json_tabulatum_obtinere(resultatum, II);
        CREDO_VERUM(_chorda_est(json_ad_chorda(
            json_objectum_capere(macro_v, "name")), "GRADUS"));
        CREDO_VERUM(json_ad_integer(json_objectum_capere(macro_v,
            "kind")) == XIV);
    }

    n = _lege(extra, p, &bene);   /* hover SUMMA: corpus totum
                                   * multilineare, verbatim */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_est(&n,
        "#define SUMMA(x, y) \\\n    ((x) + \\\n     (y))"));

    n = _lege(extra, p, &bene);   /* hover '{': vicinitas - sedes
                                   * proximae, non NIHIL nudum */
    CREDO_VERUM(bene);
    CREDO_VERUM(_hover_valor_continet(&n, "sedes proximae"));
    CREDO_VERUM(_hover_valor_continet(&n, "probatio_macro_functio"));

    n = _lege(extra, p, &bene);   /* shutdown */
    CREDO_VERUM(bene);
    CREDO_VERUM(_resultatum_nullum(&n));

    fclose(intra);
    fclose(extra);
}

/* ==================================================
 * VIGILIA SUI (lib/vigilia): binarium residens fontibus
 * posterius -> diagnosticum in publicatione. Manifestum fictum
 * (invarians launcheri: aedificator indicem scribit - graphus
 * inclusionum supervacuus) + binarium fictum mtempore antiquo
 * (utime).
 * ================================================== */

interior vacuum
_vigilia_agere (Piscina* p, constans character* via_binarii,
    constans character* via_manifesti, b32 stalus_expectatus)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    character corpus[2048];
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    CREDO_VERUM(intra != NIHIL && extra != NIHIL);
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"rootUri\":\"file://%s\",\"capabilities\":"
        "{\"general\":{\"positionEncodings\":[\"utf-8\"]}}}}",
        _radix());
    _scribe(intra, p, corpus);
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didOpen\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_v.c\",\"version\":1,"
        "\"languageId\":\"c\",\"text\":\"int x;\\n\"}}}", _radix());
    _scribe(intra, p, corpus);
    /* phantasma alterum EXTRA clausuram: monitum semel per epocham
     * (debrief 2026-07-14) - publicatio secunda quieta */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didOpen\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/lib/legatus_phantasma_v2.c\",\"version\":1,"
        "\"languageId\":\"c\",\"text\":\"int y;\\n\"}}}", _radix());
    _scribe(intra, p, corpus);
    /* plagula IN clausura propria: monitum SEMPER (textus fictus -
     * superpositio processus-localis, innocua) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/didOpen\","
        "\"params\":{\"textDocument\":{\"uri\":"
        "\"file://%s/officina/instrumenta/legatus.c\",\"version\":1,"
        "\"languageId\":\"c\",\"text\":\"int z;\\n\"}}}", _radix());
    _scribe(intra, p, corpus);
    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"shutdown\"}");
    _scribe(intra, p, "{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");

    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    cfg.binarium_via = via_binarii;
    cfg.via_manifesti = via_manifesti;
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);
    rewind(extra);

    n = _lege(extra, p, &bene);   /* initialize */
    CREDO_VERUM(bene);
    n = _lege(extra, p, &bene);   /* publicatio phantasmatis */
    CREDO_VERUM(bene);
    si (stalus_expectatus)
    {
        CREDO_VERUM(_diagnostica_numerus(&n) == (s32)I);
        CREDO_VERUM(_diagnosticum_continet(&n,
            "VIGILIA LEGATI"));
        /* fons "excubitor" (non "silva") - monitum vigiliae ab
         * origine sua nominatur */
        {
            JsonValor* lista = json_objectum_capere(n.params,
                "diagnostics");
            JsonValor* d = json_tabulatum_obtinere(lista,
                ZEPHYRUM);

            CREDO_VERUM(d != NIHIL && _chorda_est(
                json_ad_chorda(json_objectum_capere(d, "source")),
                "excubitor"));
        }
    }
    alioquin
    {
        CREDO_VERUM(_diagnostica_numerus(&n) == (s32)ZEPHYRUM);
    }
    n = _lege(extra, p, &bene);   /* phantasma v2 (extra clausuram) */
    CREDO_VERUM(bene);
    /* semel per epocham: monitum NON repetitur extra clausuram */
    CREDO_VERUM(_diagnostica_numerus(&n) == (s32)ZEPHYRUM);
    n = _lege(extra, p, &bene);   /* legatus.c (in clausura) */
    CREDO_VERUM(bene);
    si (stalus_expectatus)
    {
        /* clausura propria SEMPER monetur */
        CREDO_VERUM(_diagnostica_numerus(&n) == (s32)I);
        CREDO_VERUM(_diagnosticum_continet(&n,
            "VIGILIA LEGATI"));
    }
    alioquin
    {
        CREDO_VERUM(_diagnostica_numerus(&n) == (s32)ZEPHYRUM);
    }
    fclose(intra);
    fclose(extra);
}

interior vacuum
probatio_vigilia (Piscina* p)
{
    constans character* via_binarii =
        "officina/build/probatio_vigilia_binarium";
    constans character* via_manifesti =
        "officina/build/probatio_vigilia_manifestum";

    imprimere("--- Probans vigiliam sui (excubitor chunk 3) ---\n");

    /* manifestum fictum: fons verus legati (recens per se) */
    {
        FILE* pl = fopen(via_manifesti, "wb");

        CREDO_NON_NIHIL(pl);
        fputs("# probatio vigiliae\n"
            "officina/instrumenta/legatus.c\n", pl);
        fclose(pl);
    }

    /* casus stalus: binarium antiquum - quivis fons clausurae
     * recentior est */
    {
        FILE* pl = fopen(via_binarii, "wb");
        structura utimbuf tempora;

        CREDO_NON_NIHIL(pl);
        fputc('x', pl);
        fclose(pl);
        tempora.actime = 1000000L;
        tempora.modtime = 1000000L;
        CREDO_VERUM(utime(via_binarii, &tempora) == ZEPHYRUM);
    }
    _vigilia_agere(p, via_binarii, via_manifesti, VERUM);

    /* casus recens: binarium nunc scriptum - vigilia quieta
     * (phantasma extra clausuram = via membri quoque probata) */
    {
        FILE* pl = fopen(via_binarii, "wb");

        CREDO_NON_NIHIL(pl);
        fputc('x', pl);
        fclose(pl);
    }
    _vigilia_agere(p, via_binarii, via_manifesti, FALSUM);

    remove(via_manifesti);
    remove(via_binarii);
}

/* derivatio lexici (design B 2026-07-27): praeparatio cum
 * cfg.fons_plagulae sectiones POSIX ex inclusionibus plagulae
 * derivat - eadem plagula sine derivatione diagnostica dat, cum
 * derivatione munda est. Externa fracta = praeparatio fracta (0).
 * NB: sequentia aperiens tagi externa per %c construitur - littera
 * cruda excerptorem in HANC plagulam evocaret (lex scanner-
 * textualis, quinta sedes vitata). */
interior vacuum
probatio_praeparatio_derivata (Piscina* p)
{
    constans character* fons_posix_utens =
        "#include <sys/time.h>\n"
        "\n"
        "static long secundae(void)\n"
        "{\n"
        "    struct timeval t;\n"
        "\n"
        "    t.tv_sec = 0;\n"
        "    t.tv_usec = 0;\n"
        "    return (long)t.tv_sec;\n"
        "}\n";
    Praeparatio praeparatio;
    PraeparatorConfiguratio cfg;
    Piscina* piscina_capitum;

    (vacuum)p;
    imprimere("--- Probans derivationem lexici (design B) ---\n");

    piscina_capitum = piscina_generare_dynamicum(
        "probatio_derivationis", 8388608);
    CREDO_NON_NIHIL(piscina_capitum);

    /* sine derivatione: struct timeval ignotum -> diagnostica */
    memset(&cfg, ZEPHYRUM, magnitudo(PraeparatorConfiguratio));
    cfg.radix = _radix();
    cfg.sine_capitibus = I;   /* capita non attinent - velocius */
    CREDO_VERUM(praeparator_praeparare(&praeparatio,
        piscina_capitum, &cfg) != ZEPHYRUM);
    CREDO_NON_NIHIL(praeparatio.fons_posix);   /* SEMPER lectus */
    {
        SilvaPiscina* effimera = silva_piscina_generare_dynamicum(
            "probatio_deriv_a", 8388608);
        SilvaParsura* parsura = NIHIL;
        SilvaSemantica* sem;

        CREDO_NON_NIHIL(effimera);
        sem = praeparator_analysare(&praeparatio, effimera,
            "specimen.c", fons_posix_utens,
            (insignatus integer)strlen(fons_posix_utens), &parsura);
        CREDO_NON_NIHIL(sem);
        si (sem != NIHIL)
        {
            CREDO_VERUM(silva_c89_diagnostica_numerus(sem)
                > ZEPHYRUM);
        }
        silva_piscina_destruere(effimera);
    }
    praeparator_destruere(&praeparatio);

    /* cum derivatione: sectio sys/time.h intrat -> munda */
    memset(&cfg, ZEPHYRUM, magnitudo(PraeparatorConfiguratio));
    cfg.radix = _radix();
    cfg.sine_capitibus = I;
    cfg.fons_plagulae = fons_posix_utens;
    cfg.mensura_plagulae =
        (insignatus integer)strlen(fons_posix_utens);
    cfg.via_plagulae = "specimen.c";
    CREDO_VERUM(praeparator_praeparare(&praeparatio,
        piscina_capitum, &cfg) != ZEPHYRUM);
    {
        SilvaPiscina* effimera = silva_piscina_generare_dynamicum(
            "probatio_deriv_b", 8388608);
        SilvaParsura* parsura = NIHIL;
        SilvaSemantica* sem;

        CREDO_NON_NIHIL(effimera);
        sem = praeparator_analysare(&praeparatio, effimera,
            "specimen.c", fons_posix_utens,
            (insignatus integer)strlen(fons_posix_utens), &parsura);
        CREDO_NON_NIHIL(sem);
        si (sem != NIHIL)
        {
            CREDO_VERUM(silva_c89_diagnostica_numerus(sem)
                == ZEPHYRUM);
        }
        silva_piscina_destruere(effimera);
    }
    praeparator_destruere(&praeparatio);

    /* externa fracta (tagus apertus sine caput=) -> praeparatio
     * fracta, numquam iudicium mundum */
    {
        character fons_fractus[CXXVIII];

        sprintf(fons_fractus,
            "/* %cexterna%c\n * int x;\n */\n"
            "int proba_f(void) { return 0; }\n", '<', '>');
        memset(&cfg, ZEPHYRUM, magnitudo(PraeparatorConfiguratio));
        cfg.radix = _radix();
        cfg.sine_capitibus = I;
        cfg.fons_plagulae = fons_fractus;
        cfg.mensura_plagulae =
            (insignatus integer)strlen(fons_fractus);
        cfg.via_plagulae = "specimen_fractum.c";
        CREDO_VERUM(praeparator_praeparare(&praeparatio,
            piscina_capitum, &cfg) == ZEPHYRUM);
        praeparator_destruere(&praeparatio);
    }
    piscina_destruere(piscina_capitum);
}

/* inventio capitum NOVORUM (fallax iudicis 2026-07-14, tria
 * incendia): directoria in tempora_capitum - plagula nova mtime
 * parentis pulsat (POSIX), quod tempora capitum notorum sola
 * videre non possunt */
interior vacuum
probatio_capita_nova (Piscina* p)
{
    constans character* via_capitis =
        "officina/probationes/fixa/specimen_caput_novum.h";
    Praeparatio praeparatio;
    PraeparatorConfiguratio cfg;
    Piscina* piscina_capitum;

    (vacuum)p;
    imprimere("--- Probans inventionem capitum novorum ---\n");
    remove(via_capitis);

    piscina_capitum = piscina_generare_dynamicum(
        "probatio_capita_nova", 8388608);
    CREDO_NON_NIHIL(piscina_capitum);
    memset(&cfg, ZEPHYRUM, magnitudo(PraeparatorConfiguratio));
    cfg.radix = _radix();
    cfg.cum_latina = I;
    CREDO_VERUM(praeparator_praeparare(&praeparatio,
        piscina_capitum, &cfg) != ZEPHYRUM);
    /* mundus recens: nihil stalum */
    CREDO_VERUM(praeparator_caput_stalum(&praeparatio) == NIHIL);

    /* caput novum nascitur */
    {
        FILE* pl = fopen(via_capitis, "wb");

        CREDO_NON_NIHIL(pl);
        fputs("typedef int specimen_novum_t;\n", pl);
        fclose(pl);
    }
    /* utime directorii: granularitas secundi intra probationem
     * eundem secundum caderet - tempus antiquum differentiam vim
     * facit (exemplar vigiliae; identitas, non ordo) */
    {
        structura utimbuf tempora;

        tempora.actime = 1000000L;
        tempora.modtime = 1000000L;
        CREDO_VERUM(utime("officina/probationes/fixa", &tempora)
            == ZEPHYRUM);
    }
    /* stalum detectum (directorium mutatum = caput novum possibile) */
    CREDO_VERUM(praeparator_caput_stalum(&praeparatio) != NIHIL);

    /* reaedificatio: caput novum resolvitur */
    praeparator_destruere(&praeparatio);
    CREDO_VERUM(praeparator_praeparare(&praeparatio,
        piscina_capitum, &cfg) != ZEPHYRUM);
    {
        vacuum* valor = NIHIL;

        CREDO_VERUM(tabula_dispersa_invenire_literis(
            praeparatio.viae_capitum, "specimen_caput_novum.h",
            &valor));
    }
    /* gradus alter (venatio 2026-07-17, classis campi): caput
     * NOTUM contento mutato - stratum PLAGULAE detectoris, non
     * directorii (rescriptio mtime directorii non pulsat). In
     * campo: gesta.h mutatum sine eventu LSP -> praeparatio
     * mendax -> "numerus argumentorum discrepat" in vocationibus
     * rectis. Detector hic; filatio didChange custodita in
     * legatus.c (probatio viva per transcriptum LSP scriptum -
     * vide legatus.worklog.md; harnessum interpositum = parcum). */
    CREDO_VERUM(praeparator_caput_stalum(&praeparatio) == NIHIL);
    {
        FILE* pl = fopen(via_capitis, "wb");
        structura utimbuf tempora;

        CREDO_NON_NIHIL(pl);
        si (pl != NIHIL)
        {
            fputs("typedef long specimen_novum_t;\n", pl);
            fclose(pl);
        }
        /* tempus alterum deterministicum (granularitas secundi) */
        tempora.actime = 2000000L;
        tempora.modtime = 2000000L;
        CREDO_VERUM(utime(via_capitis, &tempora) == ZEPHYRUM);
    }
    {
        constans character* stalum =
            praeparator_caput_stalum(&praeparatio);

        CREDO_NON_NIHIL(stalum);
        CREDO_VERUM(stalum != NIHIL
            && strstr(stalum, "specimen_caput_novum.h") != NIHIL);
    }
    /* reaedificatio sanat */
    praeparator_destruere(&praeparatio);
    CREDO_VERUM(praeparator_praeparare(&praeparatio,
        piscina_capitum, &cfg) != ZEPHYRUM);
    CREDO_VERUM(praeparator_caput_stalum(&praeparatio) == NIHIL);
    praeparator_destruere(&praeparatio);
    piscina_destruere(piscina_capitum);
    remove(via_capitis);
}

/* ==================================================
 * RECENSIO (pars 2): plagula clausa numquam-in-tsv -> outgoingCalls
 * per superpositionem _recensere; editio + mtime alia -> iudicium
 * novum (gradus III)
 * ================================================== */

/* CUM prototypis praeviis - regressio umbrae prototypi: quaeque
 * declaratio registrationem PROPRIAM habet (prototypus [n,n] +
 * definitio [corpus]); primus-congruens prototypum capiebat,
 * vocationes corporis extra cadebant. Sanatio: est_definitio in
 * chartis (accessor corporis, contractus generis alieni) +
 * _extentum_tituli definitionem praefert. Vide legatus.worklog.md
 * 2026-07-14. */
interior vacuum
_stala_scribere (constans character* via, b32 cum_altero)
{
    FILE* pl = fopen(via, "wb");

    si (pl == NIHIL)
    {
        redde;
    }
    fputs("int adiutor_stalae(int a);\n"
          "int adiutor_stalae(int a) { return a + 1; }\n", pl);
    si (cum_altero)
    {
        fputs("int alter_stalae(int a);\n"
              "int alter_stalae(int a) { return a * 2; }\n", pl);
    }
    fputs("int probans_stala(void);\n"
          "int probans_stala(void)\n"
          "{\n", pl);
    fputs("    int x = adiutor_stalae(1);\n", pl);
    si (cum_altero)
    {
        fputs("    x = alter_stalae(x);\n", pl);
    }
    fputs("    return x;\n"
          "}\n", pl);
    fclose(pl);
}

interior s32
_vocata_numerus (TabellariusNuntius* n)
{
    JsonValor* resultatum = json_objectum_capere(n->radix,
        "result");

    si (resultatum == NIHIL || !json_est_tabulatum(resultatum))
    {
        redde -I;
    }
    redde (s32)json_tabulatum_numerus(resultatum);
}

interior vacuum
probatio_recensio (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    character corpus[1024];
    character via_stalae[DXII];
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    imprimere("--- Probans recensionem (gradus III) ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    sprintf(via_stalae, "%s/officina/build/probatio_stala_f.c",
        _radix());
    _stala_scribere(via_stalae, FALSUM);

    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"rootUri\":\"file://%s\",\"capabilities\":"
        "{\"general\":{\"positionEncodings\":[\"utf-8\"]}}}}",
        _radix());
    _scribe(intra, p, corpus);

    /* outgoingCalls SINE didOpen, SINE tsv: superpositio ex
     * _recensere (inventum heri [vacuum fail-clausum] obsoletum) */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":2,"
        "\"method\":\"callHierarchy/outgoingCalls\",\"params\":"
        "{\"item\":{\"name\":\"probans_stala\",\"uri\":"
        "\"file://%s\"}}}", via_stalae);
    _scribe(intra, p, corpus);

    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":7,\"method\":\"shutdown\"}");
    _scribe(intra, p, "{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");

    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);
    rewind(extra);

    n = _lege(extra, p, &bene);   /* initialize */
    CREDO_VERUM(bene);
    n = _lege(extra, p, &bene);   /* outgoingCalls: unum vocatum */
    CREDO_VERUM(bene);
    CREDO_VERUM(_vocata_numerus(&n) == (s32)I);

    fclose(intra);
    fclose(extra);

    /* sessio secunda: editio + mtime alia -> iudicium novum videt
     * vocatum alterum (in UNA sessione idem fluxus - hic sessio
     * nova quia fluxus praescripti; gradus III intra sessionem
     * per tempora_viarum agit: primum iudicium in petitione 2,
     * mtime mutatum ante petitionem 3) */
    intra = tmpfile();
    extra = tmpfile();
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"rootUri\":\"file://%s\",\"capabilities\":"
        "{\"general\":{\"positionEncodings\":[\"utf-8\"]}}}}",
        _radix());
    _scribe(intra, p, corpus);
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":2,"
        "\"method\":\"callHierarchy/outgoingCalls\",\"params\":"
        "{\"item\":{\"name\":\"probans_stala\",\"uri\":"
        "\"file://%s\"}}}", via_stalae);
    _scribe(intra, p, corpus);
    /* petitio eadem iterum: cache + _via_recens (identitas mtime)
     * -> responsum idem sine iudicio novo */
    sprintf(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":3,"
        "\"method\":\"callHierarchy/outgoingCalls\",\"params\":"
        "{\"item\":{\"name\":\"probans_stala\",\"uri\":"
        "\"file://%s\"}}}", via_stalae);
    _scribe(intra, p, corpus);
    _scribe(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":7,\"method\":\"shutdown\"}");
    _scribe(intra, p, "{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");

    /* editio ANTE sessionem: plagula cum vocato altero + mtime
     * vetus discretum */
    _stala_scribere(via_stalae, VERUM);
    {
        structura utimbuf tempora;

        tempora.actime = 1500000000L;
        tempora.modtime = 1500000000L;
        (vacuum)utime(via_stalae, &tempora);
    }

    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);
    rewind(extra);

    n = _lege(extra, p, &bene);   /* initialize */
    CREDO_VERUM(bene);
    n = _lege(extra, p, &bene);   /* outgoingCalls: DUO vocata */
    CREDO_VERUM(bene);
    CREDO_VERUM(_vocata_numerus(&n) == (s32)II);
    n = _lege(extra, p, &bene);   /* iterum: cache recens, idem */
    CREDO_VERUM(bene);
    CREDO_VERUM(_vocata_numerus(&n) == (s32)II);

    fclose(intra);
    fclose(extra);
    remove(via_stalae);
}

/* ==================================================
 * MODUS MCP (LEGATI pars 1): framing lineis + handshake +
 * tools/list + sceleta tools/call; EOF = exitus mundus 0
 * ================================================== */

interior vacuum
probatio_mcp (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;

    imprimere("--- Probans modum MCP (LEGATI) ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"protocolVersion\":\"2025-03-26\","
        "\"capabilities\":{},"
        "\"clientInfo\":{\"name\":\"probatio\"}}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"method\":"
        "\"notifications/initialized\"}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"ping\"}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"tools/list\"}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":4,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"diagnostica\",\"arguments\":"
        "{\"via\":\"lib/piscina.c\"}}}");
    /* acervus commatibus (desideratum 'Diagnostica multiplex'):
     * via secunda prava - linea erroris, non recusatio totius */
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":41,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"diagnostica\",\"arguments\":"
        "{\"via\":\"lib/piscina.c, lib/xar.c, nusquam.c\"}}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":5,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"ignotum\"}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":6,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"symbolum\",\"arguments\":"
        "{\"titulus\":\"piscina_generare_dynamicum\"}}}");
    /* typo DELETIONIS (subsequentia manet - similitudo subsequentiam
     * requirit; substitutio 'u'->'v' nihil inveniret) */
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":7,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"symbolum\",\"arguments\":"
        "{\"titulus\":\"piscina_generare_dynamicm\"}}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":8,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"vocata\",\"arguments\":"
        "{\"titulus\":\"machinula_currere\"}}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":9,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"vocantes\",\"arguments\":"
        "{\"titulus\":\"machinula_currere\"}}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":10,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"inclusiones\",\"arguments\":"
        "{\"via\":\"lib/piscina.c\"}}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":11,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"inclusiones\",\"arguments\":"
        "{\"via\":\"officina/instrumenta/legatus.h\"}}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":12,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"inclusiones\",\"arguments\":"
        "{\"via\":\"nusquam/phantasma.c\"}}}");
    /* commentarium ducens in charta: corpus sine commentario
     * (vacuum ante definitionem), caput cum contractu - via caput
     * ostenditur */
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":13,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"symbolum\",\"arguments\":"
        "{\"titulus\":\"nexus_ordines_fundere\"}}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":14,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"corpus\",\"arguments\":"
        "{\"titulus\":\"piscina_generare_dynamicum\"}}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":15,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"corpus\",\"arguments\":"
        "{\"titulus\":\"piscina_generare_dynamicm\"}}}");
    /* via disambiguat: _ch septemdecies definitum (statica per
     * plagulas probationum - NOTA: principale non idoneum, macro
     * est, ordines functionis 'main' titulantur) */
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":16,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"corpus\",\"arguments\":"
        "{\"titulus\":\"_ch\","
        "\"via\":\"probatio_scrinium.c\"}}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":17,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"corpus\",\"arguments\":"
        "{\"titulus\":\"_ch\","
        "\"via\":\"nusquam_xyz.c\"}}}");
    /* typedef-opacum (desideratum 01KXS3EXS6): typedef nudum ->
     * corpus STRUCTURAE redditur, non linea nominis sola */
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":18,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"corpus\",\"arguments\":"
        "{\"titulus\":\"SilvaSemantica\"}}}");
    /* idioma uni-declarationis (corpus intra radicem typedefi):
     * via repuncta radicem eandem videt - intactum */
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":19,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"corpus\",\"arguments\":"
        "{\"titulus\":\"TypusC89Membrum\"}}}");
    /* nullum "exit" - EOF fistulae = exitus ordinatus */

    rewind(intra);
    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    cfg.modus_mcp = VERUM;
    cfg.radix = _radix();
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);
    rewind(extra);

    n = _lege_lineam(extra, p, &bene);   /* initialize */
    CREDO_VERUM(bene);
    {
        JsonValor* resultatum = json_objectum_capere(n.radix,
            "result");

        CREDO_VERUM(resultatum != NIHIL);
        /* protocolVersion resonatum */
        CREDO_VERUM(_chorda_est(json_ad_chorda(json_objectum_capere(
            resultatum, "protocolVersion")), "2025-03-26"));
        CREDO_VERUM(_chorda_est(json_ad_chorda(json_objectum_capere(
            json_objectum_capere(resultatum, "serverInfo"),
            "name")), "legati"));
        /* doctrina praesens */
        CREDO_VERUM(json_ad_chorda(json_objectum_capere(resultatum,
            "instructions")).mensura > ZEPHYRUM);
        /* capabilities.tools praesens */
        CREDO_VERUM(json_objectum_capere(json_objectum_capere(
            resultatum, "capabilities"), "tools") != NIHIL);
    }

    n = _lege_lineam(extra, p, &bene);   /* ping */
    CREDO_VERUM(bene);
    CREDO_VERUM(json_objectum_capere(n.radix, "result") != NIHIL);

    n = _lege_lineam(extra, p, &bene);   /* tools/list */
    CREDO_VERUM(bene);
    {
        JsonValor* instrumenta = json_objectum_capere(
            json_objectum_capere(n.radix, "result"), "tools");

        CREDO_VERUM(instrumenta != NIHIL
            && json_est_tabulatum(instrumenta)
            && json_tabulatum_numerus(instrumenta) == VII);
        CREDO_VERUM(_chorda_est(json_ad_chorda(json_objectum_capere(
            json_tabulatum_obtinere(instrumenta, ZEPHYRUM),
            "name")), "diagnostica"));
        CREDO_VERUM(_chorda_est(json_ad_chorda(json_objectum_capere(
            json_tabulatum_obtinere(instrumenta, III),
            "name")), "vocata"));
        /* inputSchema.required[0] praesens */
        CREDO_VERUM(_chorda_est(json_ad_chorda(
            json_tabulatum_obtinere(json_objectum_capere(
                json_objectum_capere(json_tabulatum_obtinere(
                    instrumenta, ZEPHYRUM), "inputSchema"),
                "required"), ZEPHYRUM)), "via"));
    }

    n = _lege_lineam(extra, p, &bene);   /* diagnostica piscina.c */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus, "verdictum: ACCIPE"));
    }

    n = _lege_lineam(extra, p, &bene);   /* diagnostica acervus */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus,
            "== lib/piscina.c =="));
        CREDO_VERUM(_chorda_continet(textus, "== lib/xar.c =="));
        CREDO_VERUM(_chorda_continet(textus, "verdictum: ACCIPE"));
        CREDO_VERUM(_chorda_continet(textus, "== nusquam.c =="));
        CREDO_VERUM(_chorda_continet(textus,
            "non analysabilis aut illegibilis"));
    }

    n = _lege_lineam(extra, p, &bene);   /* instrumentum ignotum */
    CREDO_VERUM(bene);
    CREDO_VERUM(json_ad_integer(json_objectum_capere(
        json_objectum_capere(n.radix, "error"), "code"))
        == (s64)(-32602L));

    n = _lege_lineam(extra, p, &bene);   /* symbolum */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus, "signatura:"));
        CREDO_VERUM(_chorda_continet(textus, "lib/piscina.c"));
        CREDO_VERUM(_chorda_continet(textus, "usus "));
        /* vexillum (banner) supra definitionem linea vacua
         * separatum NON attachatur (regula arcte-supra) */
        CREDO_VERUM(!_chorda_continet(textus, "GENERATIO"));
    }

    n = _lege_lineam(extra, p, &bene);   /* symbolum: typo */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus, "simillima"));
        CREDO_VERUM(_chorda_continet(textus,
            "piscina_generare_dynamicum"));
    }

    n = _lege_lineam(extra, p, &bene);   /* vocata */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus, "machinula_aperire"));
    }

    n = _lege_lineam(extra, p, &bene);   /* vocantes */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus,
            "officina/instrumenta/sessio.c"));
    }

    n = _lege_lineam(extra, p, &bene);   /* inclusiones .c */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus, "includit ("));
        CREDO_VERUM(_chorda_continet(textus, "include/piscina.h"));
        CREDO_VERUM(_chorda_continet(textus, "stdlib.h"));
        /* .c a nemine inclusum */
        CREDO_VERUM(_chorda_continet(textus, "includitur ab (0)"));
    }

    n = _lege_lineam(extra, p, &bene);   /* inclusiones .h */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus, "includitur ab ("));
        CREDO_VERUM(_chorda_continet(textus,
            "officina/instrumenta/legatus.c"));
        /* clausura = includentes quattuor (folia - transitivum nihil
         * addit; sonda.c quartus, 6998d0d) */
        CREDO_VERUM(_chorda_continet(textus,
            "clausura reversa (4)"));
    }

    n = _lege_lineam(extra, p, &bene);   /* inclusiones ignota */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus,
            "in grapho inclusionum ignota"));
    }

    n = _lege_lineam(extra, p, &bene);   /* symbolum + commentarium */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        /* contractus capitis (nexus_ordines.h) in charta - sub
         * sede capitis, quia corpus commentarium ducens caret */
        CREDO_VERUM(_chorda_continet(textus,
            "Sedes + usus TU unius fundere"));
    }

    n = _lege_lineam(extra, p, &bene);   /* corpus */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus,
            "corpus piscina_generare_dynamicum"));
        CREDO_VERUM(_chorda_continet(textus, "lib/piscina.c"));
        /* corpus verum: definitio integra cum redde */
        CREDO_VERUM(_chorda_continet(textus,
            "piscina_generare_dynamicum ("));
        CREDO_VERUM(_chorda_continet(textus, "redde"));
    }

    n = _lege_lineam(extra, p, &bene);   /* corpus: typo */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus, "simillima"));
        CREDO_VERUM(_chorda_continet(textus,
            "piscina_generare_dynamicum"));
    }

    n = _lege_lineam(extra, p, &bene);   /* corpus + via */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus,
            "probationes/probatio_scrinium.c"));
        /* _ch HUIUS plagulae (strlen adhibet), non alterius */
        CREDO_VERUM(_chorda_continet(textus, "strlen"));
    }

    n = _lege_lineam(extra, p, &bene);   /* corpus + via ignota */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus, "sedes notae"));
    }

    n = _lege_lineam(extra, p, &bene);   /* corpus typedef-opacum */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus,
            "corpus SilvaSemantica"));
        /* membrum e CORPORE structurae - ante sanationem linea
         * typedefi sola reddebatur (nomen structura X X;) */
        CREDO_VERUM(_chorda_continet(textus, "scopus_currens"));
    }

    n = _lege_lineam(extra, p, &bene);   /* corpus uni-declarationis */
    CREDO_VERUM(bene);
    {
        chorda textus = _mcp_textus(&n);

        CREDO_VERUM(_chorda_continet(textus, "est_campus"));
    }

    fclose(intra);
    fclose(extra);
}

integer
principale (vacuum)
{
    Piscina* piscina;

    imprimere("\n========================================\n");
    imprimere("PROBATIO OFFICINA LEGATUS\n");
    imprimere("========================================\n\n");

    piscina = piscina_generare_dynamicum("probatio_legatus",
        CDLVI);
    si (piscina == NIHIL)
    {
        imprimere("piscina generari non potuit\n");
        redde I;
    }
    credo_aperire(piscina);

    probatio_ordo_plenus(piscina);
    probatio_hover_symbola(piscina);
    probatio_macra(piscina);
    probatio_vigilia(piscina);
    probatio_praeparatio_derivata(piscina);
    probatio_capita_nova(piscina);
    probatio_definitio_capitis(piscina);
    probatio_utf16(piscina);
    probatio_syntaxis_posita(piscina);
    probatio_caput_servatum(piscina);
    probatio_ante_initium(piscina);
    probatio_fluxus_vacuus();
    probatio_quisquiliae(piscina);
    probatio_recensio(piscina);
    probatio_mcp(piscina);

    credo_imprimere_compendium();

    {
        b32 omnia = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        imprimere("========================================\n\n");
        redde omnia ? ZEPHYRUM : I;
    }
}
