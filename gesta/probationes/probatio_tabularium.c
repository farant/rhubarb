/* probatio_tabularium.c - K1 chunk C: servus MCP per tractare
 * (lineae JSON praescriptae -> tmpfile captum). Fixtura nexus =
 * fixa/nexus_specimen.tsv (resolutio ancorarum sine tsv vero). */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabularium.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_DB "gesta/build/probatio_tab.db"
#define VIA_AN "gesta/build/probatio_tab.jsonl"
#define VIA_NX "gesta/probationes/fixa/nexus_specimen.tsv"

interior vacuum
_purgare (vacuum)
{
    remove(VIA_DB);
    remove(VIA_DB "-wal");
    remove(VIA_DB "-shm");
    remove(VIA_AN);
}

/* lineam mittere, responsum totum (litterae) recipere */
interior constans character*
_mitte (Tabularium* t, Piscina* pn, constans character* linea)
{
    FILE* effusio = tmpfile();
    chorda corpus;
    unio { constans character* l; i8* m; } u;
    long mensura;
    character* fructus;

    si (effusio == NIHIL)
    {
        redde "";
    }
    u.l = linea;
    corpus.datum = u.m;
    corpus.mensura = (i32)strlen(linea);
    (vacuum)tabularium_tractare(t, pn, corpus, effusio);
    fseek(effusio, 0L, SEEK_END);
    mensura = ftell(effusio);
    fseek(effusio, 0L, SEEK_SET);
    fructus = (character*)piscina_allocare(pn,
        (memoriae_index)(mensura > 0L ? mensura + 1L : I));
    si (fructus == NIHIL)
    {
        fclose(effusio);
        redde "";
    }
    si (mensura > 0L
        && fread(fructus, I, (memoriae_index)mensura, effusio)
            != (memoriae_index)mensura)
    {
        fclose(effusio);
        redde "";
    }
    fructus[mensura] = '\0';
    fclose(effusio);
    redde fructus;
}

s32 principale (vacuum)
{
    Piscina* piscina;
    Tabularium* t;
    TabulariumConfiguratio cfg;
    b32 praeteritus;
    constans character* r;

    piscina = piscina_generare_dynamicum("probatio_tab",
        134217728);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    _purgare();

    cfg.radix = ".";
    cfg.via_scrinii = VIA_DB;
    cfg.via_annalium = VIA_AN;
    cfg.via_nexus = VIA_NX;
    t = tabularium_creare(piscina, &cfg);
    CREDO_NON_NIHIL (t);
    si (t == NIHIL)
    {
        redde I;
    }

    /* I. ante initialize: instrumenta recusantur */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":1,"
        "\"method\":\"tools/list\"}");
    CREDO_VERUM (strstr(r, "-32002") != NIHIL);

    /* II. initialize: doctrina (contractus scribae) in
     * instructions cadit; genera seminantur */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":2,"
        "\"method\":\"initialize\",\"params\":{"
        "\"protocolVersion\":\"2025-06-18\"}}");
    CREDO_VERUM (strstr(r, "CONTRACTUS SCRIBAE") != NIHIL);
    CREDO_VERUM (strstr(r, "tabularii") != NIHIL);
    CREDO_VERUM (strstr(r, "2025-06-18") != NIHIL);

    /* iterum initialize = recusatum */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":3,"
        "\"method\":\"initialize\",\"params\":{}}");
    CREDO_VERUM (strstr(r, "iam initiatum") != NIHIL);

    /* III. tools/list: quinque instrumenta */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":4,"
        "\"method\":\"tools/list\"}");
    CREDO_VERUM (strstr(r, "\"addere\"") != NIHIL);
    CREDO_VERUM (strstr(r, "\"gerere\"") != NIHIL);
    CREDO_VERUM (strstr(r, "\"quaerere\"") != NIHIL);
    CREDO_VERUM (strstr(r, "\"res\"") != NIHIL);
    CREDO_VERUM (strstr(r, "\"census\"") != NIHIL);

    /* IV. addere: quaestio cum tags et ancoris (una resoluta, una
     * inresoluta - fixtura nexus_specimen.tsv) */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
        "\"titulus\":\"Parsura lenta\",\"corpus\":\"expansio"
        " macronis tarda\",\"tags\":\"perf, lsp\","
        "\"ancorae\":\"[{\\\"genus\\\":\\\"symbolum\\\","
        "\\\"scopus\\\":\\\"chorda_secare\\\"},{\\\"genus\\\":"
        "\\\"symbolum\\\",\\\"scopus\\\":\\\"chorda_secre\\\"}]"
        "\"}}}");
    /* NB 'chorda_secre' = typus DELETIONIS ('a' abest) - similitudo
     * subsequentiam postulat; substitutiones numquam congruunt
     * (lex documentata, aurea legati eadem) */
    CREDO_VERUM (strstr(r, "creata") != NIHIL);
    CREDO_VERUM (strstr(r, "status apertum") != NIHIL);

    /* V. res per titulum: ancora resoluta -> lib/chorda.c:42
     * (sedes PRIMA vicit, non lib/copia.c); inresoluta -> CAUTIO +
     * simillima (chorda_secare propinquum) */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":6,"
        "\"method\":\"tools/call\",\"params\":{\"name\":\"res\","
        "\"arguments\":{\"res\":\"Parsura lenta\"}}}");
    CREDO_VERUM (strstr(r, "lib/chorda.c:42") != NIHIL);
    CREDO_VERUM (strstr(r, "CAUTIO: inresoluta") != NIHIL);
    CREDO_VERUM (strstr(r, "simillima") != NIHIL);
    CREDO_VERUM (strstr(r, "annales") != NIHIL);

    /* VI. gerere: status + nota; violatio machinae notatur */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":7,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Parsura lenta\","
        "\"actus\":\"status\",\"novus\":\"laborans\"}}}");
    CREDO_VERUM (strstr(r, "status laborans") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":8,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Parsura lenta\","
        "\"actus\":\"nota\",\"textus\":\"radix in praeparatore"
        " inventa\"}}}");
    CREDO_VERUM (strstr(r, "eventum nota scriptum") != NIHIL);

    /* VII. quaerere: prosa, praefixum, tag */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":9,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"quaerere\",\"arguments\":{\"textus\":\"praeparatore\""
        "}}}");
    CREDO_VERUM (strstr(r, "Parsura lenta") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":10,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"quaerere\",\"arguments\":{\"textus\":\"pars*\","
        "\"tag\":\"perf\"}}}");
    CREDO_VERUM (strstr(r, "Parsura lenta") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":11,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"quaerere\",\"arguments\":{\"textus\":\"nusquam"
        "-invenietur\"}}}");
    CREDO_VERUM (strstr(r, "nihil inventum") != NIHIL);

    /* VIII. res ignota -> simillima ex titulis rerum (typus
     * deletionis - lex subsequentiae similitudinis) */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":12,"
        "\"method\":\"tools/call\",\"params\":{\"name\":\"res\","
        "\"arguments\":{\"res\":\"Parsura lnta\"}}}");
    CREDO_VERUM (strstr(r, "res ignota") != NIHIL);
    CREDO_VERUM (strstr(r, "Parsura lenta") != NIHIL);

    /* IX. census: genera seminata + tags + vocabularium */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":13,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"census\",\"arguments\":{}}}");
    CREDO_VERUM (strstr(r, "quaestio/laborans") != NIHIL);
    CREDO_VERUM (strstr(r, "nota/") != NIHIL);
    CREDO_VERUM (strstr(r, "perf") != NIHIL);
    CREDO_VERUM (strstr(r, "vocabularium") != NIHIL);

    /* X. nexus inter res (gerere nexus + titulus alterius) */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":14,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"parcum\","
        "\"titulus\":\"Cache calida\"}}}");
    CREDO_VERUM (strstr(r, "status parcatum") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":15,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Parsura lenta\","
        "\"actus\":\"nexus\",\"verbum\":\"impeditur-a\","
        "\"alterum\":\"Cache calida\"}}}");
    CREDO_VERUM (strstr(r, "eventum nexus scriptum") != NIHIL);

    /* XI. nuntiatio tacite omissa; ping ante omnia licitum;
     * analysis fracta -> -32700 */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"method\":"
        "\"notifications/initialized\"}");
    CREDO_AEQUALIS_I32 ((i32)strlen(r), ZEPHYRUM);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":16,"
        "\"method\":\"ping\"}");
    CREDO_VERUM (strstr(r, "\"result\"") != NIHIL);
    r = _mitte(t, piscina, "{nequaquam json");
    CREDO_VERUM (strstr(r, "-32700") != NIHIL);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
