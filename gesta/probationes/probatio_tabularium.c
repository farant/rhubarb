/* probatio_tabularium.c - K1 chunk C: servus MCP per tractare
 * (lineae JSON praescriptae -> tmpfile captum). Fixtura nexus =
 * fixa/nexus_specimen.tsv (resolutio ancorarum sine tsv vero). */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabularium.h"
#include "gesta.h"
#include "sigillum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_DB "gesta/build/probatio_tab.db"
#define VIA_AN "gesta/build/probatio_tab.jsonl"
#define VIA_NX "gesta/probationes/fixa/nexus_specimen.tsv"
#define VIA_TB "gesta/build/probatio_tabula.md"
#define VIA_BN "gesta/build/probatio_binarium_fictum.txt"
#define VIA_MN "gesta/build/probatio_manifestum_fictum"
#define VIA_FN "gesta/build/probatio_fons_fictus.c"

interior vacuum
_purgare (vacuum)
{
    remove(VIA_DB);
    remove(VIA_DB "-wal");
    remove(VIA_DB "-shm");
    remove(VIA_AN);
    remove(VIA_TB);
    remove(VIA_BN);
    remove(VIA_MN);
    remove(VIA_FN);
}

/* plagulam scribere (fixtura vigiliae) */
interior b32
_plagulam_scribere (constans character* via,
    constans character* contentum)
{
    FILE* pl = fopen(via, "wb");

    si (pl == NIHIL)
    {
        redde FALSUM;
    }
    fputs(contentum, pl);
    fclose(pl);
    redde VERUM;
}

/* plagulam totam ut litterae (vacuae si abest) */
interior constans character*
_plagula_litterae (Piscina* piscina, constans character* via)
{
    FILE* pl = fopen(via, "rb");
    long mensura;
    character* textus;

    si (pl == NIHIL)
    {
        redde "";
    }
    fseek(pl, 0L, SEEK_END);
    mensura = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura > 0L ? mensura + 1L : I));
    si (textus == NIHIL
        || (mensura > 0L
            && fread(textus, I, (memoriae_index)mensura, pl)
                != (memoriae_index)mensura))
    {
        fclose(pl);
        redde "";
    }
    textus[mensura] = '\0';
    fclose(pl);
    redde textus;
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
    cfg.via_tabulae = VIA_TB;
    cfg.signum = NIHIL;
    cfg.via_binarii = NIHIL;
    cfg.via_manifesti = NIHIL;
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

    /* X. nexus inter res (saccharum K2: res vinculi propria -
     * creatio + membra duo; grammatica gerere eadem, G15) */
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
    CREDO_VERUM (strstr(r, "creatum") != NIHIL);
    CREDO_VERUM (strstr(r, "--impeditur-a-->") != NIHIL);
    CREDO_VERUM (strstr(r, "Cache calida") != NIHIL);

    /* XI-b. tabula.md: res apertae praesentes, clausae absentes,
     * nexus redditus (proiectio plicata; INTENTIO K1.1) */
    {
        constans character* tabula = _plagula_litterae(piscina,
            VIA_TB);

        CREDO_VERUM (strstr(tabula, "QUAESTIONES") != NIHIL);
        CREDO_VERUM (strstr(tabula, "Parsura lenta") != NIHIL);
        CREDO_VERUM (strstr(tabula, "Cache calida") != NIHIL);
        CREDO_VERUM (strstr(tabula, "impeditur-a") != NIHIL);

        /* clausa e tabula evanescit */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":20,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Cache calida\","
            "\"actus\":\"status\",\"novus\":\"clausum\"}}}");
        CREDO_VERUM (strstr(r, "status clausum") != NIHIL);
        tabula = _plagula_litterae(piscina, VIA_TB);
        /* titulus in sectione PARCA abest; in NEXU manere licet
         * (ligamen ipsum vivit) - probamus sectionem */
        CREDO_VERUM (strstr(tabula, "[parcatum] Cache calida")
            == NIHIL);
        CREDO_VERUM (strstr(tabula, "Parsura lenta") != NIHIL);
    }

    /* XI-c. denexus (G15): vinculum solvitur - status solutum,
     * plicatura membra indicem purgat, sagitta e tabula evanescit
     * (res vinculi et historia manent) */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":21,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Parsura lenta\","
        "\"actus\":\"denexus\",\"verbum\":\"impeditur-a\","
        "\"alterum\":\"Cache calida\"}}}");
    CREDO_VERUM (strstr(r, "solutum") != NIHIL);
    {
        constans character* tabula = _plagula_litterae(piscina,
            VIA_TB);

        CREDO_VERUM (strstr(tabula, "impeditur-a") == NIHIL);
    }
    /* denexus iterum = ignotus (iam solutum, index vacuus) */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":22,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Parsura lenta\","
        "\"actus\":\"denexus\",\"verbum\":\"impeditur-a\","
        "\"alterum\":\"Cache calida\"}}}");
    CREDO_VERUM (strstr(r, "nexus ignotus") != NIHIL);

    /* XII. tituli duplicati: addere monet, resolutio ambigua
     * candidatos nominat, res_id discernit (quaestio 'Tituli
     * duplicati: acies tacita' - sanatio) */
    {
        /* duplicatum "Cache calida" (parcum iam clausum supra sed
         * ordo res manet - titulus idem, res altera) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":30,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Cache calida\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        CREDO_VERUM (strstr(r, "CAUTIO: titulus iam exsistit")
            != NIHIL);

        /* resolutio per titulum nunc AMBIGUA - candidati nominati */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":31,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Cache calida\"}}}");
        CREDO_VERUM (strstr(r, "titulus ambiguus") != NIHIL);
        CREDO_VERUM (strstr(r, "res_id adhibe") != NIHIL);
        CREDO_VERUM (strstr(r, "parcum/clausum") != NIHIL);
        CREDO_VERUM (strstr(r, "nota/") != NIHIL);

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":32,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Cache calida\","
            "\"actus\":\"nota\",\"textus\":\"x\"}}}");
        CREDO_VERUM (strstr(r, "titulus ambiguus") != NIHIL);

        /* titulus unicus adhuc solvit */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":33,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Parsura lenta\"}}}");
        CREDO_VERUM (strstr(r, "annales") != NIHIL);
    }

    /* XIII. salus in superficiebus (K2 passiva): schema v2 in
     * generibus VIVIS (tags = tabulatum) - valor chorda querelam
     * parit; census insalubres numerat; res querelam ostendit;
     * sanatio ambas purgat */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":40,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Parsura lenta\","
        "\"actus\":\"mutatio\",\"clavis\":\"tags\",\"valor\":"
        "\"pravum-textus\"}}}");
    CREDO_VERUM (strstr(r, "eventum mutatio scriptum") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":41,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"census\",\"arguments\":{}}}");
    CREDO_VERUM (strstr(r, "insalubres 1") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":42,"
        "\"method\":\"tools/call\",\"params\":{\"name\":\"res\","
        "\"arguments\":{\"res\":\"Parsura lenta\"}}}");
    CREDO_VERUM (strstr(r, "salus:") != NIHIL);
    CREDO_VERUM (strstr(r, "[cautio]") != NIHIL);
    CREDO_VERUM (strstr(r, "tags") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":43,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Parsura lenta\","
        "\"actus\":\"mutatio\",\"datum\":\"{\\\"tags\\\":"
        "[\\\"perf\\\",\\\"lsp\\\"]}\"}}}");
    CREDO_VERUM (strstr(r, "eventum mutatio scriptum") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":44,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"census\",\"arguments\":{}}}");
    CREDO_VERUM (strstr(r, "insalubres 0") != NIHIL);

    /* XIV. semen v2 idempotens (G14): initialize alterum NIHIL
     * scribit - tabula content-deterministica octetim aequalis
     * manet (caput = seq eventi ultimi) */
    {
        constans character* tabula_ante = _plagula_litterae(
            piscina, VIA_TB);
        Tabularium* t2 = tabularium_creare(piscina, &cfg);

        CREDO_NON_NIHIL (t2);
        si (t2 != NIHIL)
        {
            r = _mitte(t2, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":50,\"method\":\"initialize\",\"params\":"
                "{\"protocolVersion\":\"2025-06-18\"}}");
            CREDO_VERUM (strstr(r, "CONTRACTUS SCRIBAE") != NIHIL);
            {
                constans character* tabula_post =
                    _plagula_litterae(piscina, VIA_TB);

                CREDO_VERUM (strcmp(tabula_ante, tabula_post)
                    == ZEPHYRUM);
            }
        }
    }

    /* ========================================================
     * K3 CHUNK C (aureae G20-G23)
     * ======================================================== */

    /* XVI. G20: semen v3 - genera K3 praesentia; fusio v2 in
     * genera TABULAE terminata (E2-B2): quaestio attributa tabulae
     * fert, opus/actio/processus EA NON ferunt (idempotentia iam
     * in XIV probata - tabula octetim aequalis post initialize
     * alterum CUM seminibus v3) */
    {
        GestaMundus* sonda = gesta_aperire(piscina, VIA_DB,
            VIA_AN);

        CREDO_NON_NIHIL (sonda);
        si (sonda != NIHIL)
        {
            /* chorda non NUL-terminata - copiae NUL-terminatae
             * pro strstr */
            {
                chorda c = gesta_genus_datum(sonda, "quaestio",
                    piscina);
                character* buf = (character*)piscina_allocare(
                    piscina, (memoriae_index)c.mensura + I);

                CREDO_VERUM (c.mensura > ZEPHYRUM);
                si (buf != NIHIL && c.mensura > ZEPHYRUM)
                {
                    memcpy(buf, c.datum,
                        (memoriae_index)c.mensura);
                    buf[c.mensura] = '\0';
                    CREDO_VERUM (strstr(buf, "\"ancorae\"")
                        != NIHIL);
                }
            }
            {
                chorda c = gesta_genus_datum(sonda,
                    "claudere-cum-decreto", piscina);
                character* buf = (character*)piscina_allocare(
                    piscina, (memoriae_index)c.mensura + I);

                CREDO_VERUM (c.mensura > ZEPHYRUM);
                si (buf != NIHIL && c.mensura > ZEPHYRUM)
                {
                    memcpy(buf, c.datum,
                        (memoriae_index)c.mensura);
                    buf[c.mensura] = '\0';
                    CREDO_VERUM (strstr(buf,
                        "\"species\":\"actio\"") != NIHIL);
                    CREDO_VERUM (strstr(buf, "\"ancorae\"")
                        == NIHIL);
                }
            }
            {
                chorda c = gesta_genus_datum(sonda,
                    "ritus-signaculi", piscina);
                character* buf = (character*)piscina_allocare(
                    piscina, (memoriae_index)c.mensura + I);

                CREDO_VERUM (c.mensura > ZEPHYRUM);
                si (buf != NIHIL && c.mensura > ZEPHYRUM)
                {
                    memcpy(buf, c.datum,
                        (memoriae_index)c.mensura);
                    buf[c.mensura] = '\0';
                    CREDO_VERUM (strstr(buf,
                        "\"species\":\"processus\"") != NIHIL);
                    CREDO_VERUM (strstr(buf, "\"ancorae\"")
                        == NIHIL);
                }
            }
            {
                chorda c = gesta_genus_datum(sonda, "opus",
                    piscina);
                character* buf = (character*)piscina_allocare(
                    piscina, (memoriae_index)c.mensura + I);

                CREDO_VERUM (c.mensura > ZEPHYRUM);
                si (buf != NIHIL && c.mensura > ZEPHYRUM)
                {
                    memcpy(buf, c.datum,
                        (memoriae_index)c.mensura);
                    buf[c.mensura] = '\0';
                    CREDO_VERUM (strstr(buf, "\"assignatum\"")
                        != NIHIL);
                    CREDO_VERUM (strstr(buf, "\"ancorae\"")
                        == NIHIL);
                }
            }
            gesta_claudere(sonda);
        }
    }

    /* XVII. G21: agere - receptum princeps felix (ligamina PER
     * TITULUM soluta) + recusata causam nominat */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":70,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"parcum\","
        "\"titulus\":\"Migratio K3\"}}}");
    CREDO_VERUM (strstr(r, "creata") != NIHIL);
    /* recusata: parcum parcatum, actio tractum postulat */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":71,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"agere\",\"arguments\":{\"actio\":"
        "\"claudere-cum-decreto\",\"ligamina\":\"{\\\"parcum\\\":"
        "\\\"Migratio K3\\\"}\",\"argumenta\":\"{\\\"cur\\\":"
        "\\\"probatio\\\"}\"}}}");
    CREDO_VERUM (strstr(r, "actio recusata") != NIHIL);
    CREDO_VERUM (strstr(r, "status opis") != NIHIL);
    /* tractum -> felix: sex effectus atomice, res novae nominatae */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":72,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Migratio K3\","
        "\"actus\":\"status\",\"novus\":\"tractum\"}}}");
    CREDO_VERUM (strstr(r, "status tractum") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":73,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"agere\",\"arguments\":{\"actio\":"
        "\"claudere-cum-decreto\",\"ligamina\":\"{\\\"parcum\\\":"
        "\\\"Migratio K3\\\"}\",\"argumenta\":\"{\\\"cur\\\":"
        "\\\"K3 probatum est\\\"}\",\"actor\":\"fran\"}}}");
    CREDO_VERUM (strstr(r, "actio facta") != NIHIL);
    CREDO_VERUM (strstr(r, "res nova") != NIHIL);
    CREDO_VERUM (strstr(r, "K3 probatum est") != NIHIL);
    /* parcum clausum + decretum natum + vinculum in tabula */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":74,"
        "\"method\":\"tools/call\",\"params\":{\"name\":\"res\","
        "\"arguments\":{\"res\":\"Migratio K3\"}}}");
    CREDO_VERUM (strstr(r, "clausum") != NIHIL);
    CREDO_VERUM (strstr(r, "clausum: K3 probatum est") != NIHIL);
    CREDO_VERUM (strstr(r, "natum-de") != NIHIL);
    {
        constans character* tabula = _plagula_litterae(piscina,
            VIA_TB);

        CREDO_VERUM (strstr(tabula, "K3 probatum est") != NIHIL);
        CREDO_VERUM (strstr(tabula, "natum-de") != NIHIL);
    }

    /* XVIII. G22: affordantiae in res (actiones: linea) - tractum
     * affert, quaestio non */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":75,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"parcum\","
        "\"titulus\":\"Affordata\"}}}");
    CREDO_VERUM (strstr(r, "creata") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":76,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Affordata\","
        "\"actus\":\"status\",\"novus\":\"tractum\"}}}");
    CREDO_VERUM (strstr(r, "status tractum") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":77,"
        "\"method\":\"tools/call\",\"params\":{\"name\":\"res\","
        "\"arguments\":{\"res\":\"Affordata\"}}}");
    CREDO_VERUM (strstr(r, "actiones: claudere-cum-decreto")
        != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":78,"
        "\"method\":\"tools/call\",\"params\":{\"name\":\"res\","
        "\"arguments\":{\"res\":\"Parsura lenta\"}}}");
    CREDO_VERUM (strstr(r, "actiones:") == NIHIL);
    /* tools/list nunc sex instrumenta */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":79,"
        "\"method\":\"tools/list\"}");
    CREDO_VERUM (strstr(r, "\"agere\"") != NIHIL);

    /* XIX. G23: ritus-signaculi incipit - opus primum cursus
     * consilii generatur (in-cursu sequentia: spec NONDUM);
     * tabula instantiam + opus ostendit; perfectio operis per
     * gerere provectionem SPONTE ciet */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":80,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"parcum\","
        "\"titulus\":\"Parcum ritus\"}}}");
    CREDO_VERUM (strstr(r, "creata") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":81,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"agere\",\"arguments\":{\"processus\":"
        "\"ritus-signaculi\",\"ligamina\":\"{\\\"propositum\\\":"
        "\\\"Parcum ritus\\\"}\",\"argumenta\":"
        "\"{\\\"cur_sigilli\\\":\\\"probatio ritus\\\"}\"}}}");
    CREDO_VERUM (strstr(r, "inceptus: instantia") != NIHIL);
    CREDO_VERUM (strstr(r, "interrogatio: incohatus") != NIHIL);
    CREDO_VERUM (strstr(r, "spec:") == NIHIL);
    {
        constans character* tabula = _plagula_litterae(piscina,
            VIA_TB);

        CREDO_VERUM (strstr(tabula, "PROCESSUS") != NIHIL);
        CREDO_VERUM (strstr(tabula, "ritus-signaculi") != NIHIL);
        CREDO_VERUM (strstr(tabula, "OPERA") != NIHIL);
        CREDO_VERUM (strstr(tabula, "interrogatio") != NIHIL);
    }
    /* perfectio operis 'interrogatio' -> spec generatur (motor
     * per superficiem MCP ipsam) */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":82,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"interrogatio\","
        "\"actus\":\"status\",\"novus\":\"perfectum\"}}}");
    CREDO_VERUM (strstr(r, "status perfectum") != NIHIL);
    {
        constans character* tabula = _plagula_litterae(piscina,
            VIA_TB);

        CREDO_VERUM (strstr(tabula, "spec") != NIHIL);
    }

    /* XV. vigilia (lib/vigilia): signum in serverInfo.version +
     * censu; fontes superantes -> CAUTIO; binarium motum ->
     * promotio + CAUTIO glutinosa. Semantica profunda in
     * probatio_vigilia (unitas); hic = integratio superficierum. */
    {
        Tabularium* t3;
        TabulariumConfiguratio cfg3 = cfg;
        Sigillum s;
        character hex[SIGILLUM_HEX_MENSURA];
        character breve[XVII];
        constans character* contentum = "binarium fictum primum";

        CREDO_VERUM (_plagulam_scribere(VIA_BN, contentum));
        CREDO_VERUM (_plagulam_scribere(VIA_MN, VIA_FN "\n"));
        s = sigillum_computare(contentum, strlen(contentum));
        sigillum_hex(&s, hex);
        memcpy(breve, hex, XVI);
        breve[XVI] = '\0';
        cfg3.signum = hex;
        cfg3.via_binarii = VIA_BN;
        cfg3.via_manifesti = VIA_MN;
        t3 = tabularium_creare(piscina, &cfg3);
        CREDO_NON_NIHIL (t3);
        si (t3 != NIHIL)
        {
            r = _mitte(t3, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":60,\"method\":\"initialize\",\"params\":"
                "{\"protocolVersion\":\"2025-06-18\"}}");
            CREDO_VERUM (strstr(r, breve) != NIHIL);

            r = _mitte(t3, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":61,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"census\",\"arguments\":{}}}");
            CREDO_VERUM (strstr(r, "binarium ") != NIHIL);
            CREDO_VERUM (strstr(r, breve) != NIHIL);
            CREDO_VERUM (strstr(r, "CAUTIO VIGILIAE") == NIHIL);

            /* fons post ortum scriptus (lex ns - etiam eodem
             * secundo detegitur) -> vigilia fontium */
            CREDO_VERUM (_plagulam_scribere(VIA_FN,
                "integer x;\n"));
            r = _mitte(t3, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":62,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"census\",\"arguments\":{}}}");
            CREDO_VERUM (strstr(r, "CAUTIO VIGILIAE: fontes me"
                " superant") != NIHIL);

            /* binarium 'reaedificatum' contento alio -> promotio
             * ad nuntium disci */
            CREDO_VERUM (_plagulam_scribere(VIA_BN,
                "binarium fictum alterum - contentum longius"));
            r = _mitte(t3, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":63,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"census\",\"arguments\":{}}}");
            CREDO_VERUM (strstr(r, "CAUTIO VIGILIAE: binarium in"
                " disco novius me") != NIHIL);

            /* glutinosa: vocatio proxima etiam monet */
            r = _mitte(t3, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":64,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"quaerere\",\"arguments\":"
                "{\"textus\":\"parsur*\"}}}");
            CREDO_VERUM (strstr(r, "CAUTIO VIGILIAE") != NIHIL);
        }
    }

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
