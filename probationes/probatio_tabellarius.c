/* probatio_tabellarius.c - Probationes cursoris JSON-RPC
 * (LEGATUS chunk A: framing + envelope + fabri)
 */

#include "tabellarius.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>
#include <string.h>

/* chorda == litterae (octetim) */
interior b32
_aequalis (chorda c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (c.mensura == (i32)m && c.datum != NIHIL
        && memcmp(c.datum, litterae, m) == ZEPHYRUM) ? VERUM : FALSUM;
}

/* forma canonica: scribere(legere(litterae)) - asserta objectorum
 * independentia formae exactae json_scribere */
interior chorda
_canonica (constans character* litterae, Piscina* piscina)
{
    JsonResultus res = json_legere_literis(litterae, piscina);

    si (!res.successus || res.radix == NIHIL)
    {
        chorda vacua;

        vacua.mensura = ZEPHYRUM;
        vacua.datum = NIHIL;
        redde vacua;
    }
    redde json_scribere(res.radix, piscina);
}

interior b32
_aequalis_canonicae (chorda c, constans character* litterae,
    Piscina* piscina)
{
    chorda canonica = _canonica(litterae, piscina);

    redde (c.mensura == canonica.mensura && c.datum != NIHIL
        && canonica.datum != NIHIL
        && memcmp(c.datum, canonica.datum,
               (memoriae_index)c.mensura) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

/* FILE* effimerum cum contentis datis, retro volutum */
interior FILE*
_fluxus_ex_literis (constans character* litterae)
{
    FILE* pl = tmpfile();

    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fwrite(litterae, I, strlen(litterae), pl);
    rewind(pl);
    redde pl;
}

/* ==================================================
 * EPISTULA (framing)
 * ================================================== */

interior vacuum
probatio_epistula_scribere (Piscina* piscina)
{
    FILE* pl = tmpfile();
    chorda corpus = chorda_ex_literis("{}", piscina);
    character receptum[LXIV];
    memoriae_index m;

    imprimere("--- Probans epistulam scribere ---\n");
    CREDO_VERUM(pl != NIHIL);
    tabellarius_epistulam_scribere(pl, corpus);
    rewind(pl);
    m = fread(receptum, I, magnitudo(receptum), pl);
    CREDO_VERUM(m == strlen("Content-Length: 2\r\n\r\n{}"));
    CREDO_VERUM(memcmp(receptum, "Content-Length: 2\r\n\r\n{}", m)
        == ZEPHYRUM);
    fclose(pl);
}

interior vacuum
probatio_epistula_legere (Piscina* piscina)
{
    FILE* pl = _fluxus_ex_literis(
        "Content-Length: 5\r\n\r\nsalveContent-Length: 2\r\n\r\n{}");
    b32 finitus = FALSUM;
    chorda corpus;

    imprimere("--- Probans epistulam legere ---\n");
    CREDO_VERUM(pl != NIHIL);

    corpus = tabellarius_epistulam_legere(pl, piscina, &finitus);
    CREDO_VERUM(!finitus);
    CREDO_VERUM(_aequalis(corpus, "salve"));

    /* epistula secunda ex eodem fluxu */
    corpus = tabellarius_epistulam_legere(pl, piscina, &finitus);
    CREDO_VERUM(!finitus);
    CREDO_VERUM(_aequalis(corpus, "{}"));

    /* fluxus exhaustus */
    corpus = tabellarius_epistulam_legere(pl, piscina, &finitus);
    CREDO_VERUM(finitus);
    CREDO_VERUM(corpus.mensura == ZEPHYRUM);
    fclose(pl);
}

interior vacuum
probatio_epistula_tolerantia (Piscina* piscina)
{
    /* caput sine casu + caput alienum ignoratum */
    FILE* pl = _fluxus_ex_literis(
        "content-length: 4\r\n"
        "Content-Type: application/vscode-jsonrpc\r\n"
        "\r\n"
        "abcd");
    b32 finitus = FALSUM;
    chorda corpus;

    imprimere("--- Probans tolerantiam capitum ---\n");
    CREDO_VERUM(pl != NIHIL);
    corpus = tabellarius_epistulam_legere(pl, piscina, &finitus);
    CREDO_VERUM(!finitus);
    CREDO_VERUM(_aequalis(corpus, "abcd"));
    fclose(pl);
}

interior vacuum
probatio_epistula_prava (Piscina* piscina)
{
    b32 finitus;
    chorda corpus;
    FILE* pl;

    imprimere("--- Probans epistulas pravas ---\n");

    /* caput sine content-length */
    pl = _fluxus_ex_literis("Nusquam: 3\r\n\r\nabc");
    CREDO_VERUM(pl != NIHIL);
    finitus = FALSUM;
    corpus = tabellarius_epistulam_legere(pl, piscina, &finitus);
    CREDO_VERUM(finitus);
    CREDO_VERUM(corpus.mensura == ZEPHYRUM);
    fclose(pl);

    /* fluxus vacuus */
    pl = _fluxus_ex_literis("");
    CREDO_VERUM(pl != NIHIL);
    finitus = FALSUM;
    corpus = tabellarius_epistulam_legere(pl, piscina, &finitus);
    CREDO_VERUM(finitus);
    fclose(pl);

    /* corpus brevius quam promissum */
    pl = _fluxus_ex_literis("Content-Length: 99\r\n\r\nbrevis");
    CREDO_VERUM(pl != NIHIL);
    finitus = FALSUM;
    corpus = tabellarius_epistulam_legere(pl, piscina, &finitus);
    CREDO_VERUM(finitus);
    fclose(pl);
}

/* framing lineis-delimitatum (MCP, LEGATI pars 1) */
interior vacuum
probatio_linea (Piscina* piscina)
{
    imprimere("--- Probans framing lineis ---\n");

    /* legere: lineae plures, '\r' detractum, vacuae praetermissae,
     * EOF finitus */
    {
        FILE* pl = _fluxus_ex_literis(
            "{\"a\":1}\r\n"
            "\n"
            "{\"b\":2}\n");
        b32 finitus = FALSUM;
        chorda corpus;

        CREDO_VERUM(pl != NIHIL);
        corpus = tabellarius_lineam_legere(pl, piscina, &finitus);
        CREDO_VERUM(!finitus && _aequalis(corpus, "{\"a\":1}"));
        corpus = tabellarius_lineam_legere(pl, piscina, &finitus);
        CREDO_VERUM(!finitus && _aequalis(corpus, "{\"b\":2}"));
        corpus = tabellarius_lineam_legere(pl, piscina, &finitus);
        CREDO_VERUM(finitus && corpus.mensura == ZEPHYRUM);
        fclose(pl);
    }
    /* EOF ante '\n' = finitus (nuntius truncatus) */
    {
        FILE* pl = _fluxus_ex_literis("{\"c\":3}");
        b32 finitus = FALSUM;
        chorda corpus;

        CREDO_VERUM(pl != NIHIL);
        corpus = tabellarius_lineam_legere(pl, piscina, &finitus);
        CREDO_VERUM(finitus && corpus.mensura == ZEPHYRUM);
        fclose(pl);
    }
    /* scribere: corpus + '\n'; '\n' crudum RECUSATUM */
    {
        FILE* pl = tmpfile();
        character receptum[LXIV];
        memoriae_index m;

        CREDO_VERUM(pl != NIHIL);
        CREDO_VERUM(tabellarius_lineam_scribere(pl,
            chorda_ex_literis("{}", piscina)));
        CREDO_VERUM(!tabellarius_lineam_scribere(pl,
            chorda_ex_literis("{\n}", piscina)));
        rewind(pl);
        m = fread(receptum, I, magnitudo(receptum), pl);
        CREDO_VERUM(m == (memoriae_index)III
            && memcmp(receptum, "{}\n", III) == ZEPHYRUM);
        fclose(pl);
    }
    /* lex serializationis: '\t'/'\n' in valore chordae effugiuntur
     * compacte -> linea una tuta -> circulus integer */
    {
        JsonValor* obj = json_objectum_creare(piscina);
        chorda serialis;
        FILE* pl = tmpfile();
        b32 finitus = FALSUM;
        chorda relecta;
        i32 i;

        CREDO_VERUM(pl != NIHIL);
        json_objectum_ponere(obj, "nuntius",
            json_chorda_creare(piscina, chorda_ex_literis(
                "linea\nsecunda\tcum tab", piscina)));
        serialis = json_scribere(obj, piscina);
        per (i = ZEPHYRUM; i < serialis.mensura; i++)
        {
            CREDO_VERUM(serialis.datum[i] != (i8)'\n');
        }
        CREDO_VERUM(tabellarius_lineam_scribere(pl, serialis));
        rewind(pl);
        relecta = tabellarius_lineam_legere(pl, piscina, &finitus);
        CREDO_VERUM(!finitus);
        {
            JsonResultus iterum = json_legere(relecta, piscina);
            chorda valor;

            CREDO_VERUM(iterum.successus);
            valor = json_ad_chorda(json_objectum_capere(
                iterum.radix, "nuntius"));
            CREDO_VERUM(_aequalis(valor,
                "linea\nsecunda\tcum tab"));
        }
        fclose(pl);
    }
}

/* ==================================================
 * NUNTIUS (envelope)
 * ================================================== */

interior vacuum
probatio_nuntius_petitio (Piscina* piscina)
{
    chorda corpus = chorda_ex_literis(
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"processId\":7}}", piscina);
    TabellariusNuntius n = tabellarius_nuntium_legere(corpus,
        piscina);

    imprimere("--- Probans petitionem ---\n");
    CREDO_VERUM(n.genus == TABELLARIUS_PETITIO);
    CREDO_VERUM(_aequalis(n.methodus, "initialize"));
    CREDO_VERUM(n.id != NIHIL);
    CREDO_VERUM(json_est_integer(n.id));
    CREDO_VERUM(json_ad_integer(n.id) == I);
    CREDO_VERUM(n.params != NIHIL);
    CREDO_VERUM(json_est_objectum(n.params));
    CREDO_VERUM(n.radix != NIHIL);
}

interior vacuum
probatio_nuntius_nuntiatio (Piscina* piscina)
{
    chorda corpus = chorda_ex_literis(
        "{\"jsonrpc\":\"2.0\",\"method\":\"initialized\","
        "\"params\":{}}", piscina);
    TabellariusNuntius n = tabellarius_nuntium_legere(corpus,
        piscina);

    imprimere("--- Probans nuntiationem ---\n");
    CREDO_VERUM(n.genus == TABELLARIUS_NUNTIATIO);
    CREDO_VERUM(_aequalis(n.methodus, "initialized"));
    CREDO_VERUM(n.id == NIHIL);
}

interior vacuum
probatio_nuntius_id_chorda (Piscina* piscina)
{
    /* id chorda - VERBATIM referendum (C1) */
    chorda corpus = chorda_ex_literis(
        "{\"jsonrpc\":\"2.0\",\"id\":\"abc-7\","
        "\"method\":\"shutdown\"}", piscina);
    TabellariusNuntius n = tabellarius_nuntium_legere(corpus,
        piscina);

    imprimere("--- Probans id chordam ---\n");
    CREDO_VERUM(n.genus == TABELLARIUS_PETITIO);
    CREDO_VERUM(n.id != NIHIL);
    CREDO_VERUM(json_est_chorda(n.id));
    CREDO_VERUM(_aequalis(json_ad_chorda(n.id), "abc-7"));
}

interior vacuum
probatio_nuntius_pravum (Piscina* piscina)
{
    TabellariusNuntius n;

    imprimere("--- Probans nuntios pravos ---\n");

    /* fascis (tabulatum) - LSP numquam */
    n = tabellarius_nuntium_legere(
        chorda_ex_literis("[{\"id\":1}]", piscina), piscina);
    CREDO_VERUM(n.genus == TABELLARIUS_PRAVUM);

    /* quisquiliae */
    n = tabellarius_nuntium_legere(
        chorda_ex_literis("quisquiliae", piscina), piscina);
    CREDO_VERUM(n.genus == TABELLARIUS_PRAVUM);
    CREDO_VERUM(n.radix == NIHIL);

    /* neque methodus neque responsum */
    n = tabellarius_nuntium_legere(
        chorda_ex_literis("{\"id\":3}", piscina), piscina);
    CREDO_VERUM(n.genus == TABELLARIUS_PRAVUM);

    /* responsum verum */
    n = tabellarius_nuntium_legere(
        chorda_ex_literis("{\"id\":3,\"result\":null}", piscina),
        piscina);
    CREDO_VERUM(n.genus == TABELLARIUS_RESPONSUM);
    CREDO_VERUM(n.id != NIHIL);
}

/* ==================================================
 * FABRI
 * ================================================== */

interior vacuum
probatio_fabri (Piscina* piscina)
{
    chorda corpus;
    JsonValor* id_septem = json_integer_creare(piscina, VII);

    imprimere("--- Probans fabros ---\n");

    /* responsum cum resultato nullo */
    corpus = tabellarius_responsum(piscina, id_septem, NIHIL);
    CREDO_VERUM(_aequalis_canonicae(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":7,\"result\":null}", piscina));

    /* error sine id (id -> null) */
    corpus = tabellarius_errorem(piscina, NIHIL,
        TABELLARIUS_ERROR_METHODUS_IGNOTA, "methodus ignota");
    CREDO_VERUM(_aequalis_canonicae(corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":"
        "{\"code\":-32601,\"message\":\"methodus ignota\"}}",
        piscina));

    /* nuntiatio cum params */
    {
        JsonValor* params = json_objectum_creare(piscina);

        json_objectum_ponere(params, "uri",
            json_chorda_creare_literis(piscina, "file:///a.c"));
        corpus = tabellarius_nuntiationem(piscina,
            "textDocument/publishDiagnostics", params);
        CREDO_VERUM(_aequalis_canonicae(corpus,
            "{\"jsonrpc\":\"2.0\","
            "\"method\":\"textDocument/publishDiagnostics\","
            "\"params\":{\"uri\":\"file:///a.c\"}}", piscina));
    }

    /* nuntiatio sine params - campus omissus */
    corpus = tabellarius_nuntiationem(piscina, "exit", NIHIL);
    CREDO_VERUM(_aequalis_canonicae(corpus,
        "{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}", piscina));
}

/* ==================================================
 * CIRCULUS PLENUS (legere -> tractare -> scribere -> legere)
 * ================================================== */

interior vacuum
probatio_circulus (Piscina* piscina)
{
    FILE* intra = _fluxus_ex_literis(
        "Content-Length: 44\r\n\r\n"
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"shutdown\"}");
    FILE* extra = tmpfile();
    b32 finitus = FALSUM;
    chorda corpus;
    TabellariusNuntius n;

    imprimere("--- Probans circulum plenum ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    corpus = tabellarius_epistulam_legere(intra, piscina, &finitus);
    CREDO_VERUM(!finitus);
    n = tabellarius_nuntium_legere(corpus, piscina);
    CREDO_VERUM(n.genus == TABELLARIUS_PETITIO);
    CREDO_VERUM(_aequalis(n.methodus, "shutdown"));

    /* responsum cum id VERBATIM relato */
    corpus = tabellarius_responsum(piscina, n.id, NIHIL);
    tabellarius_epistulam_scribere(extra, corpus);

    rewind(extra);
    corpus = tabellarius_epistulam_legere(extra, piscina, &finitus);
    CREDO_VERUM(!finitus);
    n = tabellarius_nuntium_legere(corpus, piscina);
    CREDO_VERUM(n.genus == TABELLARIUS_RESPONSUM);
    CREDO_VERUM(n.id != NIHIL && json_est_integer(n.id));
    CREDO_VERUM(json_ad_integer(n.id) == I);

    fclose(intra);
    fclose(extra);
}

/* ================================================== */

integer
principale (vacuum)
{
    Piscina* piscina;

    imprimere("\n========================================\n");
    imprimere("PROBATIO TABELLARIUS\n");
    imprimere("========================================\n\n");

    piscina = piscina_generare_dynamicum("probatio_tabellarius",
        CDLVI);
    si (piscina == NIHIL)
    {
        imprimere("piscina generari non potuit\n");
        redde I;
    }
    credo_aperire(piscina);

    probatio_epistula_scribere(piscina);
    probatio_epistula_legere(piscina);
    probatio_epistula_tolerantia(piscina);
    probatio_epistula_prava(piscina);
    probatio_linea(piscina);
    probatio_nuntius_petitio(piscina);
    probatio_nuntius_nuntiatio(piscina);
    probatio_nuntius_id_chorda(piscina);
    probatio_nuntius_pravum(piscina);
    probatio_fabri(piscina);
    probatio_circulus(piscina);

    credo_imprimere_compendium();

    {
        b32 omnia = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        imprimere("========================================\n\n");
        redde omnia ? ZEPHYRUM : I;
    }
}
