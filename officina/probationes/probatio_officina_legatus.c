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
#include "tabellarius.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

interior b32
_chorda_est (chorda c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (c.mensura == (i32)m && c.datum != NIHIL
        && memcmp(c.datum, litterae, m) == ZEPHYRUM) ? VERUM : FALSUM;
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

    /* hover nusquam (ultra textum) - nullum */
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

    n = _lege(extra, p, &bene);   /* hover nusquam */
    CREDO_VERUM(bene);
    CREDO_VERUM(_resultatum_nullum(&n));

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

    n = _lege(extra, p, &bene);   /* shutdown */
    CREDO_VERUM(bene);
    CREDO_VERUM(_resultatum_nullum(&n));

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
    probatio_utf16(piscina);
    probatio_syntaxis_posita(piscina);
    probatio_caput_servatum(piscina);
    probatio_ante_initium(piscina);
    probatio_fluxus_vacuus();
    probatio_quisquiliae(piscina);

    credo_imprimere_compendium();

    {
        b32 omnia = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        imprimere("========================================\n\n");
        redde omnia ? ZEPHYRUM : I;
    }
}
