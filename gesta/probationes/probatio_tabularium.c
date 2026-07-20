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
#include "iter_directoria.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>   /* system - purgatio recursiva arboris entitatum */

#define VIA_DB "gesta/build/probatio_tab.db"
#define VIA_AN "gesta/build/probatio_tab.jsonl"
#define VIA_NX "gesta/probationes/fixa/nexus_specimen.tsv"
#define VIA_TB "gesta/build/probatio_tabula.md"
#define VIA_BN "gesta/build/probatio_binarium_fictum.txt"
#define VIA_MN "gesta/build/probatio_manifestum_fictum"
#define VIA_FN "gesta/build/probatio_fons_fictus.c"
#define VIA_ENT "gesta/build/probatio_entities"

/* mensura bufferi semitae probationis (MMXL deest in latina.h) */
#define PROBATIO_SEMITA_MENSURA 2048

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
    (vacuum)system("rm -rf " VIA_ENT);
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

/* prima plagula .md in directorio (recens tagi) - textus totus,
 * vacuum "" si directorium abest aut nulla plagula .md continet */
interior constans character*
_prima_plagula_md (Piscina* piscina, constans character* dir)
{
    DirectoriumIterator* it = directorium_iterator_aperire(dir, piscina);
    DirectoriumIntroitus* e;
    chorda exemplar = chorda_ex_literis("*.md", piscina);

    si (it == NIHIL)
    {
        redde "";
    }
    dum ((e = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (e->genus == INTROITUS_FILUM
            && e->titulus.mensura > ZEPHYRUM
            && e->titulus.datum[0] != '.'
            && directorium_titulus_congruit(e->titulus, exemplar))
        {
            character semita[PROBATIO_SEMITA_MENSURA];

            sprintf(semita, "%s/%s", dir,
                chorda_ut_cstr(e->titulus, piscina));
            directorium_iterator_claudere(it);
            redde _plagula_litterae(piscina, semita);
        }
    }
    directorium_iterator_claudere(it);
    redde "";
}

/* plagulam .md in directorio quaerere cuius contentus substantiam
 * continet - fructus = contentus totus illius plagulae (vacuum ""
 * si nulla congruit). Folder (praesertim _sine_tag) crebras res
 * insertas per probationes antecedentes fert, ordo iteratoris
 * directorii non certus; ergo _prima_plagula_md fragile hic est ubi
 * plura entia coexsistunt (probatum empirice: XXIIIb infra sine hac
 * functione fracta) */
interior constans character*
_plagula_cum_continente (Piscina* piscina, constans character* dir,
    constans character* substantia)
{
    DirectoriumIterator* it = directorium_iterator_aperire(dir, piscina);
    DirectoriumIntroitus* e;
    chorda exemplar = chorda_ex_literis("*.md", piscina);

    si (it == NIHIL)
    {
        redde "";
    }
    dum ((e = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (e->genus == INTROITUS_FILUM
            && e->titulus.mensura > ZEPHYRUM
            && e->titulus.datum[0] != '.'
            && directorium_titulus_congruit(e->titulus, exemplar))
        {
            character semita[PROBATIO_SEMITA_MENSURA];
            constans character* contentum;

            sprintf(semita, "%s/%s", dir,
                chorda_ut_cstr(e->titulus, piscina));
            contentum = _plagula_litterae(piscina, semita);
            si (strstr(contentum, substantia) != NIHIL)
            {
                directorium_iterator_claudere(it);
                redde contentum;
            }
        }
    }
    directorium_iterator_claudere(it);
    redde "";
}

/* an ULLA plagula .md in directorio substantiam continet (saccharum
 * super _plagula_cum_continente pro assertionibus booleis) */
interior b32
_ulla_plagula_continet (Piscina* piscina, constans character* dir,
    constans character* substantia)
{
    redde _plagula_cum_continente(piscina, dir, substantia)[0]
        != '\0';
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

/* res_id ex responso "res <ID> creata" extrahere (primum "res "
 * in textu responsi = nostrum) */
interior vacuum
_res_id_ex_responso (constans character* r, character* quaternio)
{
    constans character* p = strstr(r, "res ");
    i32 i;

    quaternio[0] = '\0';
    si (p == NIHIL)
    {
        redde;
    }
    p += IV;
    per (i = ZEPHYRUM; i < (i32)(GESTA_RES_ID_MENSURA - I)
        && p[i] != '\0' && p[i] != ' '; i++)
    {
        quaternio[i] = p[i];
    }
    quaternio[i] = '\0';
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
    cfg.via_entitatum = VIA_ENT;
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

    /* review fix 1: vinculum ipsum (res genus 'nexus', sine tags) -
     * antea numquam reconciliatus incrementaliter, tantum in
     * transitu pleno rarissimo - accipit proiectionem propriam */
    {
        constans character* p = strstr(r, "nexus ");

        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            character vid[GESTA_RES_ID_MENSURA];
            i32 k;

            p += VI;
            per (k = ZEPHYRUM; k < (i32)(GESTA_RES_ID_MENSURA - I)
                && p[k] != '\0' && p[k] != ' '; k++)
            {
                vid[k] = p[k];
            }
            vid[k] = '\0';
            CREDO_VERUM (_ulla_plagula_continet(piscina,
                VIA_ENT "/_sine_tag", vid));
        }
    }

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
    /* review fix 1 (continued): vinculum quoque renovatur - status
     * solutum apparet in eius proiectione propria (antea manebat
     * vetus 'vigens' usque ad transitum plenum rarissimum) */
    {
        constans character* p = strstr(r, "nexus ");

        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            character vid[GESTA_RES_ID_MENSURA];
            i32 k;
            constans character* md;

            p += VI;
            per (k = ZEPHYRUM; k < (i32)(GESTA_RES_ID_MENSURA - I)
                && p[k] != '\0' && p[k] != ' '; k++)
            {
                vid[k] = p[k];
            }
            vid[k] = '\0';
            md = _plagula_cum_continente(piscina, VIA_ENT "/_sine_tag",
                vid);
            CREDO_VERUM (strstr(md, "status: solutum") != NIHIL);
        }
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

            /* tacere: agnitio explicita - confirmatio ipsa per
             * infundibulum fluit et unum absumit; duo sequentia
             * tacita; tum numero exhausto loquitur iterum */
            r = _mitte(t3, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":65,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"tacere\",\"arguments\":"
                "{\"responsa\":3}}}");
            CREDO_VERUM (strstr(r, "vigilia tacet per 3")
                != NIHIL);
            CREDO_VERUM (strstr(r, "CAUTIO VIGILIAE") == NIHIL);
            r = _mitte(t3, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":66,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"census\",\"arguments\":{}}}");
            CREDO_VERUM (strstr(r, "CAUTIO VIGILIAE") == NIHIL);
            r = _mitte(t3, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":67,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"census\",\"arguments\":{}}}");
            CREDO_VERUM (strstr(r, "CAUTIO VIGILIAE") == NIHIL);
            r = _mitte(t3, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":68,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"census\",\"arguments\":{}}}");
            CREDO_VERUM (strstr(r, "CAUTIO VIGILIAE") != NIHIL);

            /* responsa absens -> error */
            r = _mitte(t3, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":69,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"tacere\",\"arguments\":{}}}");
            CREDO_VERUM (strstr(r, "requiritur") != NIHIL);
        }
    }

    /* X.b tacere in residente sine vigilia armata: nihil tacendum */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":70,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"tacere\",\"arguments\":{\"responsa\":5}}}");
    CREDO_VERUM (strstr(r, "nihil tacendum") != NIHIL);

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

    /* XX. G17 (K4): instrumentum ramus - vita completa via MCP;
     * parametrum ramus in addere/gerere/res; lex res_id in ramo
     * (E2-B1); truncus caecus usque ad fusionem */
    {
        character rid[GESTA_RES_ID_MENSURA];

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":70,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "\"ramus\"") != NIHIL);
        CREDO_VERUM (strstr(r, "ABICERE") != NIHIL);

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":71,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"ramus\",\"arguments\":{\"actus\":\"creare\","
            "\"titulus\":\"speculatio\",\"actor\":\"fran\"}}}");
        CREDO_VERUM (strstr(r, "creatus") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":72,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"ramus\",\"arguments\":{\"actus\":\"creare\","
            "\"titulus\":\"speculatio\"}}}");
        CREDO_VERUM (strstr(r, "recusatum") != NIHIL);

        /* addere in ramo - trunco invisibilis */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":73,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"cogitatio ramalis\",\"ramus\":"
            "\"speculatio\",\"actor\":\"fran\"}}}");
        CREDO_VERUM (strstr(r, "in ramo speculatio") != NIHIL);
        _res_id_ex_responso(r, rid);
        CREDO_VERUM (rid[0] != '\0');

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":74,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":"
            "\"cogitatio\"}}}");
        CREDO_VERUM (strstr(r, "nihil inventum") != NIHIL);
        {
            character linea[CCLVI];

            sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":75,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"res\",\"arguments\":{\"res\":\"%s\"}}}", rid);
            r = _mitte(t, piscina, linea);
            CREDO_VERUM (strstr(r, "res ignota") != NIHIL);

            /* res cum ramo (res_id) - plicatura ramalis redditur */
            sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":76,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"res\",\"arguments\":{\"res\":\"%s\",\"ramus\":"
                "\"speculatio\"}}}", rid);
            r = _mitte(t, piscina, linea);
            CREDO_VERUM (strstr(r, "in ramo speculatio") != NIHIL);
            CREDO_VERUM (strstr(r, "cogitatio ramalis") != NIHIL);

            /* LEX E2-B1: titulus in ramo recusatur */
            r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":77,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"res\",\"arguments\":{\"res\":"
                "\"cogitatio ramalis\",\"ramus\":"
                "\"speculatio\"}}}");
            CREDO_VERUM (strstr(r, "res_id requiritur") != NIHIL);

            /* gerere in ramo (nota) */
            sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":78,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"gerere\",\"arguments\":{\"res\":\"%s\","
                "\"actus\":\"nota\",\"textus\":\"adnotatio"
                " ramalis\",\"ramus\":\"speculatio\",\"actor\":"
                "\"fran\"}}}", rid);
            r = _mitte(t, piscina, linea);
            CREDO_VERUM (strstr(r, "in ramo speculatio") != NIHIL);

            /* nexus in ramo nondum */
            sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":79,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"gerere\",\"arguments\":{\"res\":\"%s\","
                "\"actus\":\"nexus\",\"verbum\":\"impedit\","
                "\"alterum\":\"x\",\"ramus\":\"speculatio\"}}}",
                rid);
            r = _mitte(t, piscina, linea);
            CREDO_VERUM (strstr(r, "nondum sustentus") != NIHIL);

            /* comparare: truncus absens, ramus praesens */
            sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":80,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"ramus\",\"arguments\":{\"actus\":\"comparare\","
                "\"titulus\":\"speculatio\",\"res\":\"%s\"}}}",
                rid);
            r = _mitte(t, piscina, linea);
            CREDO_VERUM (strstr(r, "(absens)") != NIHIL);
            CREDO_VERUM (strstr(r, "ramus speculatio") != NIHIL);

            /* fundere - copiae in truncum, FTS nunc videt */
            r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":81,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"ramus\",\"arguments\":{\"actus\":"
                "\"fundere\",\"titulus\":\"speculatio\","
                "\"actor\":\"fran\"}}}");
            CREDO_VERUM (strstr(r, "fusio perfecta") != NIHIL);
            r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":82,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"quaerere\",\"arguments\":{\"textus\":"
                "\"cogitatio\"}}}");
            CREDO_VERUM (strstr(r, "cogitatio ramalis") != NIHIL);
            sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":83,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"res\",\"arguments\":{\"res\":\"%s\"}}}", rid);
            r = _mitte(t, piscina, linea);
            CREDO_VERUM (strstr(r, "res ignota") == NIHIL);
        }

        /* abicere flagship: vilis, resolutio titulorum activos
         * solos videt - scriptura in abiectum iam per titulum
         * inattingibilis */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":84,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"ramus\",\"arguments\":{\"actus\":\"creare\","
            "\"titulus\":\"vitrina\"}}}");
        CREDO_VERUM (strstr(r, "creatus") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":85,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"ramus\",\"arguments\":{\"actus\":\"abicere\","
            "\"titulus\":\"vitrina\"}}}");
        CREDO_VERUM (strstr(r, "abiectus") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":86,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"post mortem\",\"ramus\":"
            "\"vitrina\"}}}");
        CREDO_VERUM (strstr(r, "ramus ignotus") != NIHIL);
    }

    /* XXI. G18 (K4): census 'rami activi'; tabula sectio RAMI
     * (activi soli); semen v4 genus ramus praesens */
    {
        constans character* tb;

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":87,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"ramus\",\"arguments\":{\"actus\":\"creare\","
            "\"titulus\":\"manens\"}}}");
        CREDO_VERUM (strstr(r, "creatus") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":88,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"census\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "rami activi 1") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":89,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"ramus\",\"arguments\":{\"actus\":"
            "\"enumerare\"}}}");
        CREDO_VERUM (strstr(r, "manens [activus]") != NIHIL);
        CREDO_VERUM (strstr(r, "speculatio [fusus]") != NIHIL);
        CREDO_VERUM (strstr(r, "vitrina [abiectus]") != NIHIL);
        tb = _plagula_litterae(piscina, VIA_TB);
        CREDO_VERUM (strstr(tb, "## RAMI") != NIHIL);
        CREDO_VERUM (strstr(tb, "manens") != NIHIL);
        {
            GestaMundus* sonda = gesta_aperire(piscina, VIA_DB,
                VIA_AN);

            CREDO_NON_NIHIL (sonda);
            si (sonda != NIHIL)
            {
                chorda c = gesta_genus_datum(sonda, "ramus",
                    piscina);

                CREDO_VERUM (c.mensura > ZEPHYRUM);
                gesta_claudere(sonda);
            }
        }
    }

    /* ================================================
     * praefixum ULID inambiguum (ergonomia 2026-07-17):
     * recessus ultimus post id exactum + titulum
     * ================================================ */
    {
        character id_a[27];
        character id_b[27];
        character praefixum[27];
        character vocatio[512];
        constans character* p;
        i32 k;

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":90,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"res\","
            "\"arguments\":{\"res\":\"Parsura lenta\"}}}");
        p = strstr(r, "res_id ");
        CREDO_NON_NIHIL (p);
        si (p == NIHIL)
        {
            redde I;
        }
        memcpy(id_a, p + VII, XXVI);
        id_a[XXVI] = '\0';

        /* praefixum XXV characterum (unicum) resolvit ad id plenum */
        memcpy(praefixum, id_a, XXV);
        praefixum[XXV] = '\0';
        sprintf(vocatio, "{\"jsonrpc\":\"2.0\",\"id\":91,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"res\","
            "\"arguments\":{\"res\":\"%s\"}}}", praefixum);
        r = _mitte(t, piscina, vocatio);
        CREDO_VERUM (strstr(r, id_a) != NIHIL);
        CREDO_VERUM (strstr(r, "res ignota") == NIHIL);

        /* gerere per praefixum: nota scribitur */
        sprintf(vocatio, "{\"jsonrpc\":\"2.0\",\"id\":92,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"nota\",\"textus\":\"per praefixum scripta\"}}}",
            praefixum);
        r = _mitte(t, piscina, vocatio);
        CREDO_VERUM (strstr(r, "nota") != NIHIL);
        CREDO_VERUM (strstr(r, "ignota") == NIHIL);

        /* res altera - praefixum COMMUNE ambiguum fit */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":93,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Probatio praefixi ambigui\"}}}");
        p = strstr(r, "res 01");
        CREDO_NON_NIHIL (p);
        si (p == NIHIL)
        {
            redde I;
        }
        memcpy(id_b, p + IV, XXVI);
        id_b[XXVI] = '\0';
        per (k = ZEPHYRUM; k < XXVI && id_a[k] == id_b[k]; k++)
        {
        }
        /* ULID eiusdem cursus partem temporis communem habent */
        CREDO_VERUM (k >= VI);
        si (k >= VI)
        {
            memcpy(praefixum, id_a, (memoriae_index)k);
            praefixum[k] = '\0';
            sprintf(vocatio, "{\"jsonrpc\":\"2.0\",\"id\":94,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"res\",\"arguments\":{\"res\":\"%s\"}}}",
                praefixum);
            r = _mitte(t, piscina, vocatio);
            CREDO_VERUM (strstr(r, "praefixum ambiguum") != NIHIL);
        }

        /* praefixum validum nulli congruens = res ignota */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":95,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"res\","
            "\"arguments\":{\"res\":\"7ZZZZZZZZZ\"}}}");
        CREDO_VERUM (strstr(r, "res ignota") != NIHIL);
    }

    /* ================================================
     * XXII. K4.2 ergonomia: origo + similia + breviter
     * ================================================ */
    {
        /* schemata nova in tools/list */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":100,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "\"origo\"") != NIHIL);
        CREDO_VERUM (strstr(r, "\"breviter\"") != NIHIL);

        /* origo in addere: eventus provenientiam datam fert
         * (annales JSONL = via probationis vilissima - totum iter
         * scripturae tegit) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":101,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Messis probationis\",\"origo\":"
            "\"messis-probatio\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        {
            constans character* an = _plagula_litterae(piscina,
                VIA_AN);

            CREDO_VERUM (strstr(an, "messis-probatio") != NIHIL);
        }

        /* origo in gerere (nota) - eadem via */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":102,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Parsura lenta\","
            "\"actus\":\"nota\",\"textus\":\"adnotatio"
            " provenientiae\",\"origo\":\"sessio-42\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
        {
            constans character* an = _plagula_litterae(piscina,
                VIA_AN);

            CREDO_VERUM (strstr(an, "sessio-42") != NIHIL);
        }

        /* similia: titulus verba cum 'Parsura lenta' communia -
         * vicinus in responso creationis nominatur */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":103,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Parsura tarda\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        CREDO_VERUM (strstr(r, "similia:") != NIHIL);
        CREDO_VERUM (strstr(r, "Parsura lenta") != NIHIL);

        /* titulus alienus - nihil simile, nihil appensum */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":104,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Zyx wvut\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        CREDO_VERUM (strstr(r, "similia:") == NIHIL);

        /* breviter: corpus + notae ultimae (III ex III) +
         * caput; datum crudum, ancorae, annales ABSENT */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":105,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Parsura lenta\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "res_id") != NIHIL);
        CREDO_VERUM (strstr(r, "corpus expansio macronis tarda")
            != NIHIL);
        CREDO_VERUM (strstr(r, "notae (3 ex 3") != NIHIL);
        CREDO_VERUM (strstr(r, "adnotatio provenientiae")
            != NIHIL);
        CREDO_VERUM (strstr(r, "datum") == NIHIL);
        CREDO_VERUM (strstr(r, "annales") == NIHIL);
        CREDO_VERUM (strstr(r, "ancora") == NIHIL);

        /* breviter cum affordantiis (Affordata tractum manet) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":106,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Affordata\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "actiones: claudere-cum-decreto")
            != NIHIL);
        CREDO_VERUM (strstr(r, "datum") == NIHIL);

        /* res plena immutata (datum + annales adhuc redduntur) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":107,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Parsura lenta\"}}}");
        CREDO_VERUM (strstr(r, "datum") != NIHIL);
        CREDO_VERUM (strstr(r, "annales") != NIHIL);
    }

    /* ================================================
     * XXIII. proiectio entitatum (per rem, per tag) - vita
     * completa: creatio cum duobus tags, nota, mutatio status,
     * retag (folder vetus purgatur, novum apparet)
     * ================================================ */
    {
        constans character* md;

        /* creare rem cum duobus tags */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":200,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"addere\","
            "\"arguments\":{\"genus\":\"parcum\",\"titulus\":"
            "\"Probatio Entitatum Alpha\",\"corpus\":\"corpus alpha\","
            "\"tags\":\"silva, examen\"}}}");

        /* plagula in utroque folder tagi, corpus praesens */
        md = _prima_plagula_md(piscina, VIA_ENT "/silva");
        CREDO_VERUM (strstr(md, "genus: parcum") != NIHIL);
        CREDO_VERUM (strstr(md, "# Probatio Entitatum Alpha") != NIHIL);
        CREDO_VERUM (strstr(md, "corpus alpha") != NIHIL);
        CREDO_VERUM (strstr(md, "GENERATUM") != NIHIL);
        md = _prima_plagula_md(piscina, VIA_ENT "/examen");
        CREDO_VERUM (strstr(md, "Probatio Entitatum Alpha") != NIHIL);

        /* addere nota -> nota apparet, eadem plagula */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":201,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Probatio Entitatum"
            " Alpha\",\"actus\":\"nota\",\"textus\":\"nota prima"
            " hic\"}}}");
        md = _prima_plagula_md(piscina, VIA_ENT "/silva");
        CREDO_VERUM (strstr(md, "## Notae") != NIHIL);
        CREDO_VERUM (strstr(md, "nota prima hic") != NIHIL);

        /* status -> lineae status, plagula manet (archivum planum) */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":202,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Probatio Entitatum"
            " Alpha\",\"actus\":\"status\",\"novus\":\"tractum\"}}}");
        md = _prima_plagula_md(piscina, VIA_ENT "/silva");
        CREDO_VERUM (strstr(md, "status: tractum") != NIHIL);

        /* retag: mutatio datum obiectum crudum -> tabulatum reale.
         * NOTA (probatum empirice): gerere mutatio clavis+valor
         * pono 'tags' ut chorda simplicem (json_objectum_ponere_
         * chorda - tabularium.c 1919), non tabulatum; mersio
         * superficialis gestae (gesta.h:12-14) verbatim substituit
         * - reconciliatio json_est_tabulatum requirit, ergo forma
         * clavis+valor foliret in _sine_tag (probatum: assertio
         * infra fracta cum ea forma adhibita). Forma datum crudum
         * sola tabulatum verum ponit. */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":203,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Probatio Entitatum"
            " Alpha\",\"actus\":\"mutatio\",\"datum\":"
            "\"{\\\"tags\\\":[\\\"mcp\\\"]}\"}}}");
        md = _prima_plagula_md(piscina, VIA_ENT "/mcp");
        CREDO_VERUM (strstr(md, "Probatio Entitatum Alpha") != NIHIL);
        CREDO_VERUM (_prima_plagula_md(piscina, VIA_ENT "/silva")[0]
            == '\0');   /* folder silva purgatum (vacuum aut abest) */
    }

    /* ================================================
     * XXIIIb. _sine_tag saccharum: ens sine argumento 'tags' - nulla
     * clavis 'tags' in dato, ergo n_tags == 0 in
     * _entitatem_reconciliare - plagula cadit in folder _sine_tag.
     * NOTA: _ulla_plagula_continet adhibetur (non _prima_plagula_md)
     * quia _sine_tag iam multas res antecedentes fert (parca/notae
     * sine tag + vincula nexus/denexus post review fix 1) - ordo
     * iteratoris non certus, "prima" plagula alia esse potest */
    {
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":204,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"addere\","
            "\"arguments\":{\"genus\":\"nota\",\"titulus\":"
            "\"Probatio Sine Tag\",\"corpus\":\"corpus sine tag\"}}}");
        CREDO_VERUM (_ulla_plagula_continet(piscina,
            VIA_ENT "/_sine_tag", "Probatio Sine Tag"));
    }

    /* ================================================
     * XXIIIc. reconciliatio omnium (transitus plenus): actus 'ramus
     * creare' _entitates_reconciliare_omnes ciet (purgatio +
     * rescriptio directorii totius). "Probatio Entitatum Alpha" -
     * FACTA ANTE hunc transitum (retagata in 'mcp' supra XXIII) -
     * plagulam suam POST transitum RETINET: hoc probat rescriptionem
     * plenam ex scrinio, non modo scripturam incrementalem (iam
     * probatam in XXIII per singula actus) */
    {
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":205,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"ramus\","
            "\"arguments\":{\"actus\":\"creare\",\"titulus\":"
            "\"renovatio-plena\"}}}");
        CREDO_VERUM (strstr(r, "creatus") != NIHIL);
        CREDO_VERUM (_ulla_plagula_continet(piscina, VIA_ENT "/mcp",
            "Probatio Entitatum Alpha"));
    }

    /* ================================================
     * XXIIId (review fix 2): _titulum_ad_slug numquam summam XL
     * (40) octetorum slug superat - etiam cum lineola pendens +
     * character alphanumericus in eodem gradu incidunt (ante
     * fixuram: XXXIX alnum + confinium + alnum unum = slug XLI
     * octetorum, pactum documentatum violans) */
    {
        DirectoriumIterator* it;
        DirectoriumIntroitus* e;
        chorda exemplar = chorda_ex_literis("*.md", piscina);
        i32 longitudo_nominis = ZEPHYRUM;

        /* titulus: XXXIX 'a' + spatium (confinium) + 'b' - gradus
         * ubi lineola pendens + 'b' simul incidunt post XXXIX octeti
         * iam emissi */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":206,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"addere\","
            "\"arguments\":{\"genus\":\"nota\",\"titulus\":"
            "\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa b\","
            "\"tags\":\"capacitastest\"}}}");
        it = directorium_iterator_aperire(
            VIA_ENT "/capacitastest", piscina);
        CREDO_NON_NIHIL (it);
        si (it != NIHIL)
        {
            dum ((e = directorium_iterator_proximum(it)) != NIHIL)
            {
                si (e->genus == INTROITUS_FILUM
                    && e->titulus.mensura > ZEPHYRUM
                    && e->titulus.datum[0] != '.'
                    && directorium_titulus_congruit(e->titulus,
                        exemplar))
                {
                    longitudo_nominis = e->titulus.mensura;
                }
            }
            directorium_iterator_claudere(it);
        }
        CREDO_VERUM (longitudo_nominis > ZEPHYRUM);
        /* "nota-" (IV+I) + slug (<= XL) + "-" (I) + res_id (XXVI) +
         * ".md" (III); ante fixuram slug XLI dabat, summa +I */
        CREDO_VERUM (longitudo_nominis
            <= (i32)(IV + I + XL + I + XXVI + III));
    }

    /* ================================================
     * XXIIIe. nexus inter duas entitates -> ## Nexus in plagula
     * socii redditur ut LIGAMEN RELATIVUM markdown (non modo
     * textus planus): "](../<tag>/<genus>-..." (spec: semita
     * relativa _entitatem_semita_relativa, tabularium.c:1550)
     * ================================================ */
    {
        constans character* md;

        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":207,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"addere\","
            "\"arguments\":{\"genus\":\"parcum\",\"titulus\":"
            "\"Probatio Entitatum Gamma\",\"corpus\":\"corpus gamma"
            " nexus\",\"tags\":\"silva\"}}}");
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":208,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"addere\","
            "\"arguments\":{\"genus\":\"nota\",\"titulus\":"
            "\"Probatio Entitatum Delta\",\"tags\":\"silva\"}}}");
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":209,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Probatio Entitatum"
            " Gamma\",\"actus\":\"nexus\",\"verbum\":\"nectitur-cum\","
            "\"alterum\":\"Probatio Entitatum Delta\"}}}");

        /* NOTA: locandum per corpus proprium ("corpus gamma nexus"),
         * non per titulum - Delta (socius symmetricus, cf. gesta_
         * socii_rei) fert IDEM titulus "Probatio Entitatum Gamma"
         * ut textus sui ligaminis Nexus reditus; titulus solus ergo
         * ambiguus est inter binas plagulas, ordo iteratoris incertus
         * (probatum empirice: sine hac praecautione infra flaccidum) */
        md = _plagula_cum_continente(piscina, VIA_ENT "/silva",
            "corpus gamma nexus");
        CREDO_VERUM (strstr(md, "## Nexus") != NIHIL);
        CREDO_VERUM (strstr(md, "](../silva/nota-") != NIHIL);
    }

    /* ================================================
     * XXIIIf. remotio: clavis 'corpus' delet EX DATO REI (non rem
     * ipsam - nullum actus totam entitatem delet in instrumentis
     * expositis) -> plagula ipsa manet (titulus adhuc invenitur),
     * corpus solum evanescit
     * ================================================ */
    {
        constans character* md;

        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":210,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"addere\","
            "\"arguments\":{\"genus\":\"nota\",\"titulus\":"
            "\"Probatio Entitatum Epsilon\",\"corpus\":"
            "\"corpus removendum epsilon\",\"tags\":\"silva\"}}}");
        md = _plagula_cum_continente(piscina, VIA_ENT "/silva",
            "corpus removendum epsilon");
        CREDO_VERUM (md[0] != '\0');   /* corpus praesens ante remotionem */

        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":211,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Probatio Entitatum"
            " Epsilon\",\"actus\":\"remotio\",\"clavis\":"
            "\"corpus\"}}}");
        md = _plagula_cum_continente(piscina, VIA_ENT "/silva",
            "Probatio Entitatum Epsilon");
        CREDO_VERUM (md[0] != '\0');   /* plagula manet - ens NON deletum */
        CREDO_VERUM (strstr(md, "corpus removendum epsilon") == NIHIL);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
