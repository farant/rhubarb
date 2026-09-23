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
#include "json.h"
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
#define VIA_ID "gesta/probationes/fixa/identitates_specimen.tsv"
#define VIA_CIT "gesta/build/probatio_citationes.tsv"
/* fixturae renovationis (sectio XVI) */
#define VIA_BN2 "gesta/build/probatio_binarium_renovandum.txt"
#define VIA_MN2 "gesta/build/probatio_manifestum_renovandum"
#define VIA_FN2 "gesta/build/probatio_fons_renovandus.c"
#define VIA_STIPULA_SANA "gesta/build/probatio_renovator_sanus.sh"
#define VIA_STIPULA_FRACTA "gesta/build/probatio_renovator_fractus.sh"

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
    remove(VIA_CIT);
    remove(VIA_BN2);
    remove(VIA_MN2);
    remove(VIA_FN2);
    remove(VIA_STIPULA_SANA);
    remove(VIA_STIPULA_FRACTA);
    (vacuum)system("rm -rf " VIA_ENT);
}

/* plagulam scribere (fixtura vigiliae) */
interior b32
_plagulam_scribere (
    constans character* via,
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
_plagula_litterae (
               Piscina* piscina,
    constans character* via)
{
         FILE* pl = fopen(via, "rb");
         long  mensura;
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
    si (   textus == NIHIL
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
_prima_plagula_md (
               Piscina* piscina,
    constans character* dir)
{
    DirectoriumIterator* it = directorium_iterator_aperire(dir,
        piscina);
    DirectoriumIntroitus* e;
                  chorda  exemplar = chorda_ex_literis("*.md", piscina);

    si (it == NIHIL)
    {
        redde "";
    }
    dum ((e = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (   e->genus            == INTROITUS_FILUM
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
_plagula_cum_continente (
               Piscina* piscina,
    constans character* dir,
    constans character* substantia)
{
    DirectoriumIterator* it = directorium_iterator_aperire(dir,
        piscina);
    DirectoriumIntroitus* e;
                  chorda  exemplar = chorda_ex_literis("*.md", piscina);

    si (it == NIHIL)
    {
        redde "";
    }
    dum ((e = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (   e->genus            == INTROITUS_FILUM
            && e->titulus.mensura > ZEPHYRUM
            && e->titulus.datum[0] != '.'
            && directorium_titulus_congruit(e->titulus, exemplar))
        {
                     character  semita[PROBATIO_SEMITA_MENSURA];
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
_ulla_plagula_continet (
               Piscina* piscina,
    constans character* dir,
    constans character* substantia)
{
    redde _plagula_cum_continente(piscina, dir, substantia)[0]
        != '\0';
}

/* quoties acus in textu occurrit (occurrentiae non superpositae) */
interior i32
_quoties_continet (
    constans character* textus,
    constans character* acus)
{
    constans character* cursor   = textus;
                   i32  numerus  = ZEPHYRUM;
        memoriae_index  passus   = strlen(acus);

    si (passus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    dum ((cursor = strstr(cursor, acus)) != NIHIL)
    {
        numerus++;
        cursor += passus;
    }
    redde numerus;
}

/* estne acus INTER signum initii et signum finis (finis NIHIL =
 * usque ad finem)? Sectiones responsi discernit - 'adest alicubi'
 * non sufficit cum quaeritur IN QUA sectione res stet. */
interior b32
_inter (
    constans character* textus,
    constans character* initium,
    constans character* finis,
    constans character* acus)
{
    constans character* a = strstr(textus, initium);
    constans character* b;
    constans character* locus;

    si (a == NIHIL)
    {
        redde FALSUM;
    }
    b = finis != NIHIL ? strstr(a + strlen(initium), finis) : NIHIL;
    locus = strstr(a, acus);
    si (locus == NIHIL)
    {
        redde FALSUM;
    }
    redde b == NIHIL || locus < b;
}

/* locus ordinis: textus a titulo rei (intra sectionem) usque ad
 * finem - ut causa EIUS ordinis probetur, non alterius */
interior constans character*
_inter_locus (
    constans character* textus,
    constans character* initium,
    constans character* acus)
{
    constans character* a      = strstr(textus, initium);
    constans character* locus  = a != NIHIL ? strstr(a, acus) : NIHIL;

    redde locus != NIHIL ? locus : "";
}

/* longitudo 'instructions' in responso initialize (octeti; doctrina
 * ASCII est, ergo et characteres) - ZEPHYRUM = non inventum */
interior i32
_instructiones_mensura (
    constans character* r,
               Piscina* pn)
{
    JsonResultus  res = json_legere_literis(r, pn);
       JsonValor* resultatum;
       JsonValor* instructiones;

    si (!res.successus)
    {
        redde ZEPHYRUM;
    }
    resultatum    = json_objectum_capere(res.radix, "result");
    instructiones = resultatum != NIHIL
        ? json_objectum_capere(resultatum, "instructions") : NIHIL;
    si (instructiones == NIHIL || !json_est_chorda(instructiones))
    {
        redde ZEPHYRUM;
    }
    redde json_ad_chorda(instructiones).mensura;
}

/* an instrumentum 'instrumentum' in responso tools/list
 * proprietatem 'proprietas' in inputSchema publicet */
interior b32
_proprietas_publicata (
    constans character* r,
               Piscina* pn,
    constans character* instrumentum,
    constans character* proprietas)
{
    JsonResultus  res = json_legere_literis(r, pn);
       JsonValor* resultatum;
       JsonValor* tabula;
             i32  k;

    si (!res.successus)
    {
        redde FALSUM;
    }
    resultatum = json_objectum_capere(res.radix, "result");
    tabula     = resultatum != NIHIL
        ? json_objectum_capere(resultatum, "tools") : NIHIL;
    per (k = ZEPHYRUM; tabula != NIHIL
         && k < json_tabulatum_numerus(tabula); k++)
    {
        JsonValor* instr = json_tabulatum_obtinere(tabula, k);
        JsonValor* schema;
        JsonValor* proprietates;

        si (!chorda_aequalis_literis(json_ad_chorda(
                json_objectum_capere(instr, "name")), instrumentum))
        {
            perge;
        }
        schema       = json_objectum_capere(instr, "inputSchema");
        proprietates = schema != NIHIL
            ? json_objectum_capere(schema, "properties") : NIHIL;
        redde proprietates != NIHIL
            && json_objectum_capere(proprietates, proprietas) != NIHIL;
    }
    redde FALSUM;
}

/* lineam mittere, responsum totum (litterae) recipere */
interior constans character*
_mitte (
            Tabularium* t,
               Piscina* pn,
    constans character* linea)
{
      FILE* effusio = tmpfile();
    chorda  corpus;
    unio { constans character* l; i8* m; } u;
         long  mensura;
    character* fructus;

    si (effusio == NIHIL)
    {
        redde "";
    }
    u.l             = linea;
    corpus.datum    = u.m;
    corpus.mensura  = (i32)strlen(linea);
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
    si (   mensura > 0L
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

/* stampam primam ex responso extrahere ('<#' sequitur usque ad
 * spatium; captura fragmentorum) */
interior vacuum
_stampa_ex_responso (
    constans character* r,
             character* quaternio)
{
    constans character* p = strstr(r, "<#");
                   i32  i;

    quaternio[0] = '\0';
    si (p == NIHIL)
    {
        redde;
    }
    p += II;
    per (i = ZEPHYRUM; i < XXVI && p[i] != '\0' && p[i] != ' ';
        i++)
    {
        quaternio[i] = p[i];
    }
    quaternio[i] = '\0';
}

/* res_id ex responso "res <ID> creata" extrahere (primum "res "
 * in textu responsi = nostrum) */
interior vacuum
_res_id_ex_responso (
    constans character* r,
             character* quaternio)
{
    constans character* p = strstr(r, "res ");
                   i32  i;

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

s32
principale (vacuum)
{
                   Piscina* piscina;
                Tabularium* t;
    TabulariumConfiguratio  cfg;
                       b32  praeteritus;
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

    cfg.radix             = ".";
    cfg.via_scrinii       = VIA_DB;
    cfg.via_annalium      = VIA_AN;
    cfg.via_nexus         = VIA_NX;
    cfg.via_identitatum   = VIA_ID;
    cfg.via_citationum    = VIA_CIT;
    cfg.via_tabulae       = VIA_TB;
    cfg.signum            = NIHIL;
    cfg.via_binarii       = NIHIL;
    cfg.via_manifesti     = NIHIL;
    cfg.via_entitatum     = VIA_ENT;
    cfg.via_renovatoris   = NIHIL;
    cfg.renovatio_exitus  = FALSUM;
    cfg.renatus           = FALSUM;
    t                     = tabularium_creare(piscina, &cfg);
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
    /* LIMES HOSPITIS (01M32WHJ1Q): hospes instructions ad
     * TABULARII_LIMES_INSTRUCTIONUM abscindit - doctrina cum
     * salutatione NOVA longissima TOTA legi debet (olim 4790 ex
     * 2048: MORES, contractus fori et salutatio ipsa numquam visa) */
    {
        i32 mensura_instructionum = _instructiones_mensura(r, piscina);

        CREDO_MAIOR_I32 (mensura_instructionum, ZEPHYRUM);
        CREDO_MINOR_AUT_AEQUALIS_I32 (
            mensura_instructionum + TABULARII_NOVA_MAXIMA,
            TABULARII_LIMES_INSTRUCTIONUM);
    }

    /* iterum initialize = IDEMPOTENS (F0 forum): responsum idem,
     * non recusatio - clientes daemonis per-petitionem innoxie
     * salutant */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":3,"
        "\"method\":\"initialize\",\"params\":{}}");
    CREDO_VERUM (strstr(r, "iam initiatum") == NIHIL);
    CREDO_VERUM (strstr(r, "CONTRACTUS SCRIBAE") != NIHIL);

    /* III. tools/list: quinque instrumenta */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":4,"
        "\"method\":\"tools/list\"}");
    CREDO_VERUM (strstr(r, "\"addere\"") != NIHIL);
    CREDO_VERUM (strstr(r, "\"gerere\"") != NIHIL);
    CREDO_VERUM (strstr(r, "\"quaerere\"") != NIHIL);
    CREDO_VERUM (strstr(r, "\"res\"") != NIHIL);
    CREDO_VERUM (strstr(r, "\"census\"") != NIHIL);
    /* argumentum ULTIMUM tabulae publicatur: numerus olim manu
     * scriptus (XII pro XIII, XIII pro XIV) 'datum' et 'ramus'
     * tacite abscidebat (01M37AY25M) */
    CREDO_VERUM (_proprietas_publicata(r, piscina, "addere", "intra"));
    CREDO_VERUM (_proprietas_publicata(r, piscina, "addere", "datum"));
    CREDO_VERUM (_proprietas_publicata(r, piscina, "gerere", "ramus"));

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

    /* VI-bis. ICTUS: 'haec res me ITERUM momordit'.
     * Numerus in censu apparet SOLUM cum aliquid ictum est - sectio
     * vacua spatium consumeret sine nuntio. */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":701,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"census\",\"arguments\":{}}}");
    CREDO_VERUM (strstr(r, "saepissime ICTAE") == NIHIL);

    /* textus OPTIONALIS - gradus qui verba postulat gradus est
     * qui non fit */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":702,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Parsura lenta\","
        "\"actus\":\"ictus\"}}}");
    CREDO_VERUM (strstr(r, "eventum ictus scriptum") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":703,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Parsura lenta\","
        "\"actus\":\"ictus\",\"textus\":\"iterum in messe\"}}}");
    CREDO_VERUM (strstr(r, "eventum ictus scriptum") != NIHIL);

    /* census nunc numerum fert (II ictus) */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":704,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"census\",\"arguments\":{}}}");
    CREDO_VERUM (strstr(r, "saepissime ICTAE") != NIHIL);
    CREDO_VERUM (strstr(r, "2x") != NIHIL);
    CREDO_VERUM (strstr(r, "Parsura lenta") != NIHIL);

    /* ictus in dato rei manent (series appensa, ut notae) */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":705,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"res\",\"arguments\":{\"res\":\"Parsura lenta\"}}}");
    CREDO_VERUM (strstr(r, "iterum in messe") != NIHIL);

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
    /* verbum VALIDUM absens: nihil inventum */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":11,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"quaerere\",\"arguments\":{\"textus\":\"nusquam"
        "invenietur\"}}}");
    CREDO_VERUM (strstr(r, "nihil inventum") != NIHIL);
    /* quaestio INVALIDA: 'nusquam-invenietur' FTS5 ut exclusionem
     * columnae legit ('no such column: invenietur'; '(' nudum =
     * 'syntax error') - causa SQLite nominatur, numquam 'nihil
     * inventum'. Pinna prior hic 'nihil inventum' asserebat et
     * viridis MENTIEBATUR (vitium 01M350VMNF). */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":611,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"quaerere\",\"arguments\":{\"textus\":\"nusquam"
        "-invenietur\"}}}");
    CREDO_VERUM (strstr(r, "quaestio FTS invalida") != NIHIL);
    CREDO_VERUM (strstr(r, "no such column: invenietur") != NIHIL);
    CREDO_VERUM (strstr(r, "nihil inventum") == NIHIL);
    /* TAG CUM HYPHEN (vitium 01M350VEHA): 'messis-2026-07' LIII res
     * ferebant et filtrum eum numquam invenit - tag crudus in
     * quaestionem FTS ibat. Nunc phrasis citata in columna corpus. */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":612,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"parcum\","
        "\"titulus\":\"Tg parcum regionis\",\"tags\":"
        "\"regio-probationis,perf\"}}}");
    CREDO_VERUM (strstr(r, "creata") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":613,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"quaerere\",\"arguments\":{\"textus\":\"parcum\","
        "\"tag\":\"regio-probationis\"}}}");
    CREDO_VERUM (strstr(r, "Tg parcum regionis") != NIHIL);
    CREDO_VERUM (strstr(r, "invalida") == NIHIL);
    /* tag solus (textus vacuus) quoque */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":614,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"quaerere\",\"arguments\":{\"tag\":"
        "\"regio-probationis\"}}}");
    CREDO_VERUM (strstr(r, "Tg parcum regionis") != NIHIL);
    /* tag in COLUMNA CORPUS quaeritur: titulus qui verbum tagi fert
     * tag non est */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":615,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"parcum\","
        "\"titulus\":\"Tg titulus cum verbo lignum\"}}}");
    CREDO_VERUM (strstr(r, "creata") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":616,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"quaerere\",\"arguments\":{\"tag\":\"lignum\"}}}");
    CREDO_VERUM (strstr(r, "Tg titulus cum verbo lignum") == NIHIL);

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

    /* IX-b. acta (K4.5 frustum 1): cauda fluminis - eventus
     * recentes trans res, filtrum generis excludit cetera */
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":113,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"acta\",\"arguments\":{}}}");
    CREDO_VERUM (strstr(r, "acta recentia") != NIHIL);
    CREDO_VERUM (strstr(r, "creatio") != NIHIL);
    r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":114,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"acta\",\"arguments\":{\"genus\":\"creatio\","
        "\"quantum\":2}}}");
    CREDO_VERUM (strstr(r, "creatio") != NIHIL);
    CREDO_VERUM (strstr(r, "  nota  ") == NIHIL);

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
                     character  vid[GESTA_RES_ID_MENSURA];
                           i32  k;
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
            /* semen v7 (verba canonica) IDEMPOTENS: ortus alter
             * emendationem alteram NON scribit - versio monotona
             * custodit (binaria duo seminibus diversis mundum
             * eundem servientia genus alternatim reverterent) */
            {
                constans character* an = _plagula_litterae(piscina,
                    VIA_AN);

                CREDO_AEQUALIS_I32 (_quoties_continet(an,
                    "\"verba_versio\""), I);
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
                    /* OLIM hic: 'ancorae ABSUNT' ut custos E2-B2.
                     * Assertio illa VACUA erat a natu: fusio v2
                     * genera solum SINE clave 'attributa' tangit,
                     * et opus attributa SUA semper habuit - ergo
                     * fusio caeca opus numquam tetigisset, custode
                     * aut sine. Custodes veri E2-B2 sunt actio et
                     * processus supra (attributis carent), et
                     * manent. 2026-09-21 semen v8 ancoras in opus
                     * CONSULTO declarat; quod nunc custodiendum est:
                     * emendatio definitionem TOTAM rescribit, ergo
                     * NIHIL pristinum amittere debet. */
                    CREDO_VERUM (strstr(buf, "\"ancorae\"")
                        != NIHIL);
                    CREDO_VERUM (strstr(buf, "\"corpus\"")
                        != NIHIL);
                    CREDO_VERUM (strstr(buf, "\"tags\"") != NIHIL);
                    CREDO_VERUM (strstr(buf, "\"prioritas\"")
                        != NIHIL);
                    CREDO_VERUM (strstr(buf, "\"effectus\"")
                        != NIHIL);
                    CREDO_VERUM (strstr(buf, "\"pendens\"")
                        != NIHIL);
                    CREDO_VERUM (strstr(buf, "\"omissum\"")
                        != NIHIL);
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
        TabulariumConfiguratio  cfg3 = cfg;
                      Sigillum  s;
                     character  hex[SIGILLUM_HEX_MENSURA];
                     character  breve[XVII];
            constans character* contentum = "binarium fictum primum";

        CREDO_VERUM (_plagulam_scribere(VIA_BN, contentum));
        CREDO_VERUM (_plagulam_scribere(VIA_MN, VIA_FN "\n"));
        s = sigillum_computare(contentum, strlen(contentum));
        sigillum_hex(&s, hex);
        memcpy(breve, hex, XVI);
        breve[XVI]          = '\0';
        cfg3.signum         = hex;
        cfg3.via_binarii    = VIA_BN;
        cfg3.via_manifesti  = VIA_MN;
        t3                  = tabularium_creare(piscina, &cfg3);
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
                 character  id_a[27];
                 character  id_b[27];
                 character  praefixum[27];
                 character  vocatio[512];
        constans character* p;
                       i32  k;

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
            CREDO_VERUM (strstr(r, "fragmentum ambiguum") != NIHIL);
            /* recusatio finem BREVEM unicum offert (01M37KBAWV) */
            sprintf(vocatio, "[%s]", id_a + XX);
            CREDO_VERUM (strstr(r, vocatio) != NIHIL);
        }

        /* FINIS (01M37KBAWV): fragmentum finale VI characterum
         * resolvit - entropia ULID in fine stat */
        sprintf(vocatio, "{\"jsonrpc\":\"2.0\",\"id\":951,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"res\","
            "\"arguments\":{\"res\":\"%s\"}}}", id_a + XX);
        r = _mitte(t, piscina, vocatio);
        CREDO_VERUM (strstr(r, id_a) != NIHIL);
        CREDO_VERUM (strstr(r, "res ignota") == NIHIL);
        /* litterae PARVAE (Crockford sine respectu litterarum):
         * initium et id plenum */
        {
            character minuscula[27];

            per (k = ZEPHYRUM; k < XXVI; k++)
            {
                minuscula[k] = (id_a[k] >= 'A' && id_a[k] <= 'Z')
                    ? (character)(id_a[k] - 'A' + 'a') : id_a[k];
            }
            minuscula[XXVI] = '\0';
            sprintf(vocatio, "{\"jsonrpc\":\"2.0\",\"id\":952,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"res\",\"arguments\":{\"res\":\"%s\"}}}",
                minuscula);
            r = _mitte(t, piscina, vocatio);
            CREDO_VERUM (strstr(r, id_a) != NIHIL);
            minuscula[XII] = '\0';
            sprintf(vocatio, "{\"jsonrpc\":\"2.0\",\"id\":953,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"res\",\"arguments\":{\"res\":\"%s\"}}}",
                minuscula);
            r = _mitte(t, piscina, vocatio);
            CREDO_VERUM (strstr(r, id_a) != NIHIL);
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


    /* ==================================================
     * XXII bis. breviarium VINCULA monstrat, cum statu socii
     * (2026-09-21; parcum K4.5 01KY57P2WD). Antea breviarium
     * socios OMITTEBAT: lector qui breviario solo utitur
     * impedientia NUMQUAM videbat. Et redditio plena titulum socii
     * sine statu dabat - 'estne impediens adhuc apertum?' sine
     * apertione altera sciri non poterat.
     * ================================================== */

    {
        character nuntius[DXII];
              i32 k;

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":140,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Vinculum dependens\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":141,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"parcum\","
            "\"titulus\":\"Vinculum impediens\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":142,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Vinculum dependens\",\"actus\":\"nexus\","
            "\"verbum\":\"impeditur-a\",\"alterum\":"
            "\"Vinculum impediens\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);

        /* breviarium dependentis: sagitta EXIENS + genus et status
         * socii; breviarium manet (datum crudum abest) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":143,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":"
            "\"Vinculum dependens\",\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "nexus:") != NIHIL);
        CREDO_VERUM (strstr(r, "--impeditur-a--> Vinculum impediens"
            " (parcum, parcatum)") != NIHIL);
        CREDO_VERUM (strstr(r, "datum") == NIHIL);

        /* latus alterum: sagitta INIENS */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":144,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":"
            "\"Vinculum impediens\",\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "<--impeditur-a-- Vinculum dependens"
            " (quaestio, apertum)") != NIHIL);

        /* impediens clauditur: status in breviario DEPENDENTIS
         * sequitur - hoc est quod lector scire vult */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":145,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Vinculum impediens\",\"actus\":\"status\","
            "\"novus\":\"clausum\"}}}");
        CREDO_VERUM (strstr(r, "status clausum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":146,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":"
            "\"Vinculum dependens\",\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "Vinculum impediens (parcum, clausum)")
            != NIHIL);

        /* redditio PLENA eandem sectionem fert (functio communis -
         * ne formae divergant) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":147,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":"
            "\"Vinculum dependens\"}}}");
        CREDO_VERUM (strstr(r, "Vinculum impediens (parcum, clausum)")
            != NIHIL);
        CREDO_VERUM (strstr(r, "annales") != NIHIL);

        /* genus SINE VITA: status vacuus omittitur, comma nullum */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":148,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Vinculum notatum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":149,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Vinculum dependens\",\"actus\":\"nexus\","
            "\"verbum\":\"sequitur\",\"alterum\":"
            "\"Vinculum notatum\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":150,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":"
            "\"Vinculum dependens\",\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "--sequitur--> Vinculum notatum (nota)")
            != NIHIL);

        /* TECTUM: res centralis XIII vincula trahit - XII ostensa,
         * reliqua NUMERATA (numquam tacite cadunt) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":151,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"parcum\","
            "\"titulus\":\"Vinculum centrale\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        per (k = ZEPHYRUM; k < XIII; k++)
        {
            sprintf(nuntius, "{\"jsonrpc\":\"2.0\",\"id\":%d,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"addere\",\"arguments\":{\"genus\":\"nota\","
                "\"titulus\":\"Vinculum radius %d\"}}}",
                (int)(160 + k), (int)k);
            r = _mitte(t, piscina, nuntius);
            sprintf(nuntius, "{\"jsonrpc\":\"2.0\",\"id\":%d,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"gerere\",\"arguments\":{\"res\":"
                "\"Vinculum centrale\",\"actus\":\"nexus\","
                "\"verbum\":\"sequitur\",\"alterum\":"
                "\"Vinculum radius %d\"}}}",
                (int)(180 + k), (int)k);
            r = _mitte(t, piscina, nuntius);
            CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        }
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":199,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":"
            "\"Vinculum centrale\",\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "(et alia 1)") != NIHIL);
    }


    /* ==================================================
     * XXII ter. VERBA NEXUS CANONICA (decretum 01M32TEK3K,
     * 2026-09-21): quinque verba a machina lecta, directio una (a
     * dependente ad id cui innititur). Synonymum NOTUM recusatur
     * CLARE canonicum nominans; verba cetera LIBERA transeunt.
     * Mensuratum ante decretum: XXXVII verba in CIII vinculis, et
     * impedimentum tribus orthographiis duabus directionibus
     * scriptum - visus PARATA super id aedificari non poterat.
     * ================================================== */

    {
        /* semen: genus nexus verba canonica ut DATUM fert */
        {
            constans character* an = _plagula_litterae(piscina,
                VIA_AN);

            CREDO_VERUM (strstr(an, "\"verba_canonica\"") != NIHIL);
            /* clavis 'titulus' discernit: verbum nudum iam a
             * vinculis probationis X in annalibus stat */
            CREDO_VERUM (strstr(an,
                "\"titulus\":\"impeditur-a\"") != NIHIL);
        }

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":210,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"parcum\","
            "\"titulus\":\"Verbum propositum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":211,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Verbum gradus\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);

        /* SYNONYMUM directionis EIUSDEM: recusatur, canonicum
         * nominatur, NIHIL scribitur */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":212,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Verbum gradus\",\"actus\":\"nexus\","
            "\"verbum\":\"pendet-ex\",\"alterum\":"
            "\"Verbum propositum\"}}}");
        CREDO_VERUM (strstr(r, "\"isError\":true") != NIHIL);
        CREDO_VERUM (strstr(r, "synonymum") != NIHIL);
        CREDO_VERUM (strstr(r, "impeditur-a") != NIHIL);
        CREDO_VERUM (strstr(r, "creatum") == NIHIL);

        /* verbum INVERSUM: recusatur et directionem docet */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":213,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Verbum propositum\",\"actus\":\"nexus\","
            "\"verbum\":\"impedit\",\"alterum\":"
            "\"Verbum gradus\"}}}");
        CREDO_VERUM (strstr(r, "\"isError\":true") != NIHIL);
        CREDO_VERUM (strstr(r, "inversum") != NIHIL);
        CREDO_VERUM (strstr(r, "impeditur-a") != NIHIL);
        CREDO_VERUM (strstr(r, "creatum") == NIHIL);

        /* RECUSATIO DOCET (Fran 2026-09-21): scripturam VALIDAM
         * ostendit, non solum vitium nominat. Glossa titulis
         * partes COMMUTATAS probat: initus erat 'propositum impedit
         * gradus', scriptura valida 'gradus impeditur-a propositum'
         * - et forma non commutata ABESSE debet. */
        CREDO_VERUM (strstr(r, "SCRIPTURA VALIDA") != NIHIL);
        CREDO_VERUM (strstr(r, "Verbum gradus --impeditur-a-->"
            " Verbum propositum") != NIHIL);
        CREDO_VERUM (strstr(r, "Verbum propositum --impeditur-a-->")
            == NIHIL);
        CREDO_VERUM (strstr(r, "verbum: \\\"impeditur-a\\\"")
            != NIHIL);

        /* CAUSAE OMNES SIMUL, numquam guttatim (Fran 2026-09-21):
         * synonymum ET alterum absens - ambo in responso uno, ne
         * scriptor vitium unum sanet et altero statim obstetur */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":220,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Verbum gradus\",\"actus\":\"nexus\","
            "\"verbum\":\"pendet-ex\"}}}");
        CREDO_VERUM (strstr(r, "\"isError\":true") != NIHIL);
        CREDO_VERUM (strstr(r, "(2 causae)") != NIHIL);
        CREDO_VERUM (strstr(r, "synonymum") != NIHIL);
        CREDO_VERUM (strstr(r, "alterum deest") != NIHIL);
        CREDO_VERUM (strstr(r, "SCRIPTURA VALIDA") != NIHIL);

        /* utrumque absens: duae causae + tabula canonicorum, ut
         * scriptor verbum eligere possit sine vocatione altera */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":221,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Verbum gradus\",\"actus\":\"nexus\"}}}");
        CREDO_VERUM (strstr(r, "(2 causae)") != NIHIL);
        CREDO_VERUM (strstr(r, "verbum deest") != NIHIL);
        CREDO_VERUM (strstr(r, "alterum deest") != NIHIL);
        CREDO_VERUM (strstr(r, "impeditur-a | intra | natum-de |"
            " sequitur | respondet-ad") != NIHIL);

        /* inversum CUM altero insolubili: post commutationem
         * alterum 'res' fieret, et res solvi DEBET - vitium quod
         * scriptura 'valida' ingenua postea demum ostenderet */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":222,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Verbum propositum\",\"actus\":\"nexus\","
            "\"verbum\":\"impedit\",\"alterum\":"
            "\"Res nusquam inventa\"}}}");
        CREDO_VERUM (strstr(r, "(2 causae)") != NIHIL);
        CREDO_VERUM (strstr(r, "inversum") != NIHIL);
        CREDO_VERUM (strstr(r, "rem non solvit") != NIHIL);
        CREDO_VERUM (strstr(r, "creatum") == NIHIL);

        /* RES IGNOTA inter ceteras causas: olim tractator ad rem
         * insolubilem PRIMUM redibat, ergo 'res ignota + verbum
         * pravum' itinera duo poscebat. Nunc praeiudicium rem
         * ipsum solvit et causam INTER ceteras refert. */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":223,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Res nusquam inventa\",\"actus\":\"nexus\","
            "\"verbum\":\"pendet-ex\",\"alterum\":"
            "\"Verbum propositum\"}}}");
        CREDO_VERUM (strstr(r, "(2 causae)") != NIHIL);
        CREDO_VERUM (strstr(r, "ignota") != NIHIL);
        CREDO_VERUM (strstr(r, "synonymum") != NIHIL);

        /* nihil scriptum: breviarium vincula nulla monstrat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":214,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":"
            "\"Verbum gradus\",\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "nexus:") == NIHIL);

        /* CANONICUM transit */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":215,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Verbum gradus\",\"actus\":\"nexus\","
            "\"verbum\":\"intra\",\"alterum\":"
            "\"Verbum propositum\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        CREDO_VERUM (strstr(r, "--intra-->") != NIHIL);

        /* verbum LIBERUM transit (decretum: canonicum fit solum
         * cum machina id legit; cetera libera manent) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":216,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Verbum gradus\",\"actus\":\"nexus\","
            "\"verbum\":\"cognatum\",\"alterum\":"
            "\"Verbum propositum\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);

        /* schema verba canonica DOCET (doctrina auto-onerata ab
         * hospite ad MMXLVIII characteres truncatur - mensuratum
         * 2026-09-21; schema est ubi scriptor ea videt) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":217,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "impeditur-a") != NIHIL);
        CREDO_VERUM (strstr(r, "e.g. impedit)") == NIHIL);
    }


    /* ==================================================
     * XXII quater. OPERA PLANORUM (2026-09-21, parcum K4.5):
     * genus 'opus' corpus/tags/ancoras DECLARAT (semen v8), et
     * quaesitio opera ORDINARIE excludit - numquam tacite.
     * MENSURATUM: res exsistentes ~CCL verba scientiae ferunt,
     * opera ZEPHYRUM (tituli IX characterum: 'spec'). Basis
     * scientiae a catena operum non polluatur.
     * ================================================== */

    {
        /* semen v8: declaratio in annalibus, semel */
        {
            constans character* an = _plagula_litterae(piscina,
                VIA_AN);

            CREDO_AEQUALIS_I32 (_quoties_continet(an,
                "\"attributa_versio\""), I);
        }

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":230,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Harundo scientiae vera\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":231,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"opus\","
            "\"titulus\":\"planum I.1: harundo secanda\","
            "\"corpus\":\"Perfectum cum porta transit.\","
            "\"tags\":\"planum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        /* custos duplicationum: opus NOVUM scientiam non monstrat
         * ut 'simile' (opus alterius operis simile esse potest,
         * non quaestionis) */
        CREDO_VERUM (strstr(r, "Harundo scientiae vera") == NIHIL);

        /* quaerere ORDINARIE: scientia adest, opus ABEST, et
         * exclusio NUMERATA nominatur cum via ad eam tollendam */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":232,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":"
            "\"harund*\"}}}");
        CREDO_VERUM (strstr(r, "Harundo scientiae vera") != NIHIL);
        CREDO_VERUM (strstr(r, "harundo secanda") == NIHIL);
        CREDO_VERUM (strstr(r, "opera exclusa: 1") != NIHIL);
        CREDO_VERUM (strstr(r, "genus") != NIHIL);

        /* genus expresse petitum: opus redditur */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":233,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":"
            "\"harund*\",\"genus\":\"opus\"}}}");
        CREDO_VERUM (strstr(r, "harundo secanda") != NIHIL);
        CREDO_VERUM (strstr(r, "opera exclusa") == NIHIL);

        /* OPERA SOLA congruunt: 'nihil inventum' MENTIRETUR -
         * responsum exclusionem nominat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":234,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":"
            "\"secand*\"}}}");
        CREDO_VERUM (strstr(r, "opera exclusa: 1") != NIHIL);

        /* scientia NOVA opus ut 'simile' non accipit */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":235,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Harundo secanda altera\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        CREDO_VERUM (strstr(r, "planum I.1") == NIHIL);
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
        chorda exemplar        = chorda_ex_literis("*.md", piscina);
        i32 longitudo_nominis  = ZEPHYRUM;

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
                si (   e->genus            == INTROITUS_FILUM
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


    /* ================================================
     * XXIIIg. puritas trunci in proiectione: res nata in ramo et
     * fusa (XX: 'cogitatio ramalis' cum nota 'adnotatio ramalis'
     * scripta IN RAMO, deinde fundere) fert eventus BIS in
     * tessellae - originalia ramalia + copiae truncales fusionis,
     * res_id eodem. Sine filtro branch_id='' quaeque linea Status/
     * Notae bis redderetur (insectum repertum in recensione
     * 2026-07-20 - artefactum generatum ipsum id monstravit).
     * ================================================ */

    {
        constans character* md = _plagula_cum_continente(piscina,
            VIA_ENT "/_sine_tag", "cogitatio ramalis");
        constans character* p;

        CREDO_VERUM (md[0] != '\0');
        /* nota SEMEL sola (copia truncalis; originale ramale
         * filtratum) */
        p = strstr(md, "adnotatio ramalis");
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            CREDO_VERUM (strstr(p + I, "adnotatio ramalis")
                == NIHIL);
        }
        /* linea 'creatum' SEMEL sola in ## Status */
        p = strstr(md, "- creatum \xE2\x80\x94");
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            CREDO_VERUM (strstr(p + I, "- creatum \xE2\x80\x94")
                == NIHIL);
        }
    }


    /* ================================================
     * XXIIIh. DETERMINISMUS OCTETORUM (spec gesta-entitates,
     * sectio Testing): reconciliatio BIS facta octeta PARIA
     * relinquit - nullae rescripturae spuriae.
     *
     * Haec probatio vitium praesens NON persequitur: proprietas per
     * constructionem tenet, quia nullum horologium in via
     * redditionis est. Custodit ne horologium parietis FUTURUM
     * irrepat - 'mutatum <hora>' in redditione scriptum silenter
     * transiret, et deinde omnis reconciliatio omnem plagulam
     * rescriberet (proiectio quae numquam quiescit).
     *
     * initialize scobem plenam ciet (_seminare + _tabulam_scribere
     * + _entitates_reconciliare_omnes) et IDEMPOTENS est (supra II),
     * ergo nihil status inter duos cursus mutatur: differentia ulla
     * octetorum non ambigua esset.
     *
     * Ens electum est 'cogitatio ramalis' - res in ramo nata et fusa,
     * ergo via redditionis MAXIME implicata (eventus duplices,
     * filtrum trunci XXIIIg supra). Si quid non-deterministicum
     * usquam irrepit, hic primum apparebit.
     * ================================================ */

    {
        constans character* ante;
        constans character* post;

        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":260,"
            "\"method\":\"initialize\",\"params\":{}}");
        ante = _plagula_cum_continente(piscina, VIA_ENT "/_sine_tag",
            "cogitatio ramalis");
        CREDO_VERUM (ante[0] != '\0');

        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":261,"
            "\"method\":\"initialize\",\"params\":{}}");
        post = _plagula_cum_continente(piscina, VIA_ENT "/_sine_tag",
            "cogitatio ramalis");
        CREDO_VERUM (post[0] != '\0');

        CREDO_VERUM (strcmp(ante, post) == ZEPHYRUM);
    }

    /* FRUSTUM D (01KY3D7EJP): ancorae nid + citationes ex codice */
    {
                 character  rid[27];
        constans character* p;
                       i32  k;

        /* res cum ancoris nid: una resoluta (fixtura
         * identitates_specimen), una inresoluta */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":900,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Putredo bidirectionalis probanda\","
            "\"corpus\":\"frustum D\",\"ancorae\":"
            "\"[{\\\"genus\\\":\\\"nid\\\",\\\"scopus\\\":"
            "\\\"01KY3TESTAA\\\"},{\\\"genus\\\":\\\"nid\\\","
            "\\\"scopus\\\":\\\"01KY3TESTZZ\\\"}]\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        rid[ZEPHYRUM]  = '\0';
        p              = strstr(r, "res 01");
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            per (k = ZEPHYRUM; k < XXVI; k++)
            {
                rid[k] = p[IV + k];
            }
            rid[XXVI] = '\0';
        }

        /* resolutio: fixtura -> lib/exemplum.c:12 (genus); absens
         * -> CAUTIO cum mandato renovationis */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":901,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Putredo"
            " bidirectionalis probanda\"}}}");
        CREDO_VERUM (strstr(r,
            "lib/exemplum.c:12 (definitio-functionis)") != NIHIL);
        CREDO_VERUM (strstr(r, "CAUTIO: inresoluta (nid") != NIHIL);

        /* citationes ex codice: plagula runtime-scripta (praefixum
         * verum rei + praefixum orbum) - lector cache-in-successu,
         * ergo scriptio media probationis visibilis fit */
        {
            FILE* cit = fopen(VIA_CIT, "w");

            CREDO_NON_NIHIL (cit);
            si (cit != NIHIL)
            {
                fprintf(cit, "# citationes specimen\n");
                fprintf(cit, "%.10s\tlib/exemplum.c\t42\n", rid);
                fprintf(cit,
                    "01KY3ZZZZZ\tlib/exemplum.c\t99\n");
                fclose(cit);
            }
        }

        /* nexus reversus in lectione rei */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":902,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Putredo"
            " bidirectionalis probanda\"}}}");
        CREDO_VERUM (strstr(r,
            "citata ex codice: lib/exemplum.c:42") != NIHIL);

        /* census: salus citationum - 2 summae, 1 inresoluta
         * nominata */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":903,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"census\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r,
            "citationes ex codice 2 (inresolutae 1)") != NIHIL);
        CREDO_VERUM (strstr(r,
            "CAUTIO citatio ex codice inresoluta: 01KY3ZZZZZ")
            != NIHIL);
    }

    /* XVI. F0 FORUM: genera fori seminata + custos CCXL +
     * signatura (spec-v2 par III) */
    {
        character arti[GESTA_RES_ID_MENSURA];
        character pipi[GESTA_RES_ID_MENSURA];
        character linea[PROBATIO_SEMITA_MENSURA];
        character longum[CCXLII];
        character multibyte[DXII];
              i32 i;

        /* articulus: status initialis conditum, machina plena */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":950,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"articulus\","
            "\"titulus\":\"articulus probationis fori\","
            "\"corpus\":\"paragraphus prima.\",\"actor\":\"fran\""
            "}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        CREDO_VERUM (strstr(r, "status conditum") != NIHIL);
        _res_id_ex_responso(r, arti);
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":951,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"status\",\"novus\":\"publicatum\"}}}", arti);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "status publicatum") != NIHIL);
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":952,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"status\",\"novus\":\"retractum\"}}}", arti);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "status retractum") != NIHIL);

        /* pipatum CCXL exacte = acceptum (limes inclusivus) */
        per (i = ZEPHYRUM; i < CCXL; i++)
        {
            longum[i] = 'a';
        }
        longum[CCXL] = '\0';
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":953,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"pipatum\","
            "\"titulus\":\"pipatum plenum\",\"corpus\":\"%s\","
            "\"signatura\":\"Fable 5\"}}}", longum);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        _res_id_ex_responso(r, pipi);

        /* CCXLI = recusatio nominata */
        longum[CCXL]   = 'a';
        longum[CCXLI]  = '\0';
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":954,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"pipatum\","
            "\"titulus\":\"pipatum nimium\",\"corpus\":\"%s\"}}}",
            longum);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r,
            "pipatum nimis longum (limes CCXL codicilli)")
            != NIHIL);

        /* CCXL characteres multibyte (480 bytes) = acceptum -
         * CODICILLI numerantur, non bytes (decisio Franis) */
        per (i = ZEPHYRUM; i < CCXL; i++)
        {
            multibyte[i * II]      = (character)0xC3;
            multibyte[i * II + I]  = (character)0xA9; /* é */
        }
        multibyte[CDLXXX] = '\0';
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":955,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"pipatum\","
            "\"titulus\":\"pipatum multibyte\",\"corpus\":"
            "\"%s\"}}}", multibyte);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "creata") != NIHIL);

        /* signatura in creatione condita (datum eventus) */
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":956,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}", pipi);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "Fable 5") != NIHIL);

        /* signatura in mutatione (post ramos actuum - mutatio
         * cruda datum substituit); annales veritas */
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":957,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"mutatio\",\"clavis\":\"corpus\",\"valor\":"
            "\"emendatum\",\"signatura\":\"Opus 4.8\"}}}", arti);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "eventum mutatio scriptum")
            != NIHIL);
        CREDO_VERUM (strstr(_plagula_litterae(piscina, VIA_AN),
            "Opus 4.8") != NIHIL);

        /* commentarium + filum respondet-ad (verbum liberum -
         * mutationes nullae, spec-v2 par III.4) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":958,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"commentarium\","
            "\"titulus\":\"commentarium probationis\",\"corpus\":"
            "\"respondeo.\",\"signatura\":\"Fable 5\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        {
            character comm[GESTA_RES_ID_MENSURA];

            _res_id_ex_responso(r, comm);
            sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":959,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"gerere\",\"arguments\":{\"res\":\"%s\","
                "\"actus\":\"nexus\",\"verbum\":\"respondet-ad\","
                "\"alterum\":\"%s\"}}}", comm, arti);
            r = _mitte(t, piscina, linea);
            CREDO_VERUM (strstr(r, "--respondet-ad-->") != NIHIL);
        }

        /* legere (F2): lectio structurata - filtrum generis,
         * datum insertum (signatura), respondet_ad resolutum */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":960,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":\"pipatum\"}}}");
        CREDO_VERUM (strstr(r, "pipatum plenum") != NIHIL);
        CREDO_VERUM (strstr(r, "Fable 5") != NIHIL);
        CREDO_VERUM (strstr(r, "articulus probationis fori")
            == NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":961,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":"
            "\"commentarium\"}}}");
        CREDO_VERUM (strstr(r, "respondet_ad") != NIHIL);
        CREDO_VERUM (strstr(r, arti) != NIHIL);
        CREDO_VERUM (strstr(r, "respondeo.") != NIHIL);

        /* F4: ad = responsum uno vocamine + titulus derivatus */
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":962,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"pipatum\","
            "\"corpus\":\"responsum sine titulo, uno vocamine.\","
            "\"ad\":\"%s\"}}}", pipi);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "--respondet-ad-->") != NIHIL);
        CREDO_VERUM (strstr(r, "creata: responsum sine titulo,")
            != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":963,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":\"pipatum\"}}}");
        CREDO_VERUM (strstr(r, pipi) != NIHIL);
        CREDO_VERUM (strstr(r, "respondet_ad") != NIHIL);
        /* ad IGNOTUM: totum recusatur, nihil scriptum (01M37JYNMT) -
         * olim textus crudus ut membrum cadebat et res sine vinculo
         * vero nascebatur; LXXII vincula respondet-ad in annalibus
         * (tabularium + forum) nullum textum crudum ferunt */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":9621,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"pipatum\","
            "\"titulus\":\"Pp responsum orbum\",\"corpus\":"
            "\"ad nihil.\",\"ad\":\"Pipatum nullum\"}}}");
        CREDO_VERUM (strstr(r, "creata") == NIHIL);
        CREDO_VERUM (strstr(r, "nihil scriptum") != NIHIL);
        CREDO_VERUM (strstr(r, "Pipatum nullum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":9622,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":"
            "\"Pp responsum orbum\"}}}");
        CREDO_VERUM (strstr(r, "res ignota") != NIHIL);

        /* diurnum (acta diurna): genus SEMINATUM - creatio nullam
         * notam custodiae trahit. Regressio mensurata 2026-08-01:
         * genus in codice natum sed sine ordine seminis omnem
         * creationem violatione 'genus ignotum' notabat (IX
         * diurna, IX violationes) - haec porta id iterum caperet */
        {
            character diur[GESTA_RES_ID_MENSURA];

            r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":968,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"addere\",\"arguments\":{\"genus\":"
                "\"diurnum\",\"titulus\":\"dies probationis\","
                "\"corpus\":\"hodie scripsi.\",\"actor\":"
                "\"fran\"}}}");
            CREDO_VERUM (strstr(r, "creata") != NIHIL);
            _res_id_ex_responso(r, diur);
            sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":969,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"res\",\"arguments\":{\"res\":\"%s\"}}}", diur);
            r = _mitte(t, piscina, linea);
            CREDO_VERUM (strstr(r, "hodie scripsi.") != NIHIL);
            CREDO_VERUM (strstr(r, "genus ignotum") == NIHIL);
            CREDO_VERUM (strstr(r, "nota (machina)") == NIHIL);
        }

        /* F4: salutatio lectoris + acta ab_lecto (cursor conditus
         * in initialize primo ad hwm tunc - omnia posteriora non
         * lecta) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":964,"
            "\"method\":\"initialize\",\"params\":{}}");
        CREDO_VERUM (strstr(r, "NOVA:") != NIHIL);
        /* salutatio PRIMA stat, ante doctrinam - in fine
         * abscindebatur et numquam legebatur (01M32WHJ1Q) */
        CREDO_NON_NIHIL (strstr(r, "TABULARIUM:"));
        CREDO_VERUM (strstr(r, "NOVA:") < strstr(r, "TABULARIUM:"));
        CREDO_MINOR_AUT_AEQUALIS_I32 (_instructiones_mensura(r,
            piscina), TABULARII_LIMES_INSTRUCTIONUM);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":965,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"acta\",\"arguments\":{\"ab_lecto\":\"verum\","
            "\"quantum\":\"200\"}}}");
        CREDO_VERUM (strstr(r, "acta non lecta") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":966,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"acta\",\"arguments\":{\"ab_lecto\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "(nihil novi)") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":967,"
            "\"method\":\"initialize\",\"params\":{}}");
        CREDO_VERUM (strstr(r, "NOVA:") == NIHIL);
    }


    /* ========================================================
     * XVII. GENERA PER APP DEFINITA (G0): registrum dynamicum ex
     * entibus definitionis, iudicium camporum (iudicat-non-
     * obstat), cardinalitas unum, emendatio additiva sola,
     * ab_lecto discriminat (entia = data, definitiones =
     * epistulae)
     * ======================================================== */

    {
        character def_auctor[GESTA_RES_ID_MENSURA];
        character auctor_a[GESTA_RES_ID_MENSURA];
        character auctor_b[GESTA_RES_ID_MENSURA];
        character liber_a[GESTA_RES_ID_MENSURA];
        character linea[1400];

        /* definitio auctoris: campus unus textus */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":970,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"definitio\","
            "\"titulus\":\"Auctor\",\"datum\":\"{\\\"clavis\\\":"
            "\\\"auctor\\\",\\\"campi\\\":[{\\\"clavis\\\":"
            "\\\"appellatio\\\",\\\"typus\\\":\\\"textus\\\"}],"
            "\\\"campus_tituli\\\":\\\"appellatio\\\"}\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        _res_id_ex_responso(r, def_auctor);

        /* definitio libri: textus + annus + numerus + relatio
         * unum ad auctorem */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":971,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"definitio\","
            "\"titulus\":\"Liber\",\"datum\":\"{\\\"clavis\\\":"
            "\\\"liber\\\",\\\"campi\\\":[{\\\"clavis\\\":"
            "\\\"titulus_operis\\\",\\\"typus\\\":\\\"textus\\\"},"
            "{\\\"clavis\\\":\\\"annus\\\",\\\"typus\\\":"
            "\\\"annus\\\"},{\\\"clavis\\\":\\\"paginae\\\","
            "\\\"typus\\\":\\\"numerus\\\"},{\\\"clavis\\\":"
            "\\\"lectus\\\",\\\"typus\\\":\\\"veritas\\\"},"
            "{\\\"clavis\\\":\\\"status_lecturae\\\",\\\"typus\\\":"
            "\\\"electio\\\",\\\"optiones\\\":[\\\"desideratus\\\","
            "\\\"possessus\\\",\\\"lectus\\\"]},{\\\"clavis\\\":"
            "\\\"auctor\\\",\\\"typus\\\":\\\"relatio\\\","
            "\\\"ad\\\":\\\"auctor\\\",\\\"cardinalitas\\\":"
            "\\\"unum\\\"}],\\\"campus_tituli\\\":"
            "\\\"titulus_operis\\\"}\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);

        /* registrum novit ambo (sonda gestarum, exemplar G20) */
        {
            GestaMundus* sonda = gesta_aperire(piscina, VIA_DB,
                VIA_AN);

            CREDO_NON_NIHIL (sonda);
            si (sonda != NIHIL)
            {
                chorda c = gesta_genus_datum(sonda, "liber",
                    piscina);
                character* buf = (character*)piscina_allocare(
                    piscina, (memoriae_index)c.mensura + I);

                CREDO_VERUM (c.mensura > ZEPHYRUM);
                si (buf != NIHIL && c.mensura > ZEPHYRUM)
                {
                    memcpy(buf, c.datum,
                        (memoriae_index)c.mensura);
                    buf[c.mensura] = '\0';
                    CREDO_VERUM (strstr(buf, "\"campi\"")
                        != NIHIL);
                    CREDO_VERUM (strstr(buf, "\"annus\"")
                        != NIHIL);
                }
                gesta_claudere(sonda);
            }
        }

        /* entia auctorum (genus modo definitum accipitur - ipsum
         * registrum probat: ante G0 'genus ignotum' notaretur) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":972,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"auctor\","
            "\"titulus\":\"Gaius Iulius Caesar\",\"datum\":"
            "\"{\\\"appellatio\\\":\\\"Gaius Iulius Caesar\\\"}\""
            "}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        _res_id_ex_responso(r, auctor_a);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":973,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"auctor\","
            "\"titulus\":\"Plinius Maior\",\"datum\":"
            "\"{\\\"appellatio\\\":\\\"Plinius Maior\\\"}\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        _res_id_ex_responso(r, auctor_b);

        /* ens libri mundum: annus signatus negativus (a.C.n.) -
         * scriptura munda, nulla nota camporum adhuc */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":974,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"liber\","
            "\"titulus\":\"Commentarii de Bello Gallico\","
            "\"datum\":\"{\\\"titulus_operis\\\":\\\"Commentarii"
            " de Bello Gallico\\\",\\\"annus\\\":-52,"
            "\\\"paginae\\\":240,\\\"lectus\\\":true,"
            "\\\"status_lecturae\\\":\\\"possessus\\\"}\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        _res_id_ex_responso(r, liber_a);
        CREDO_VERUM (strstr(_plagula_litterae(piscina, VIA_AN),
            "violatio camporum") == NIHIL);

        /* datum malformatum = recusatio clara (non scriptum) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":975,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"liber\","
            "\"titulus\":\"Malformatus\",\"datum\":\"non json\""
            "}}}");
        CREDO_VERUM (strstr(r, "datum: obiectum JSON requiritur")
            != NIHIL);

        /* violatio typi: annus ut chorda + clavis ignota -
         * scriptura PROCEDIT, nota custodiae composita in annales
         * (iudicat, non obstat) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":976,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"liber\","
            "\"titulus\":\"Liber Pravus\",\"datum\":"
            "\"{\\\"annus\\\":\\\"-52\\\",\\\"color\\\":"
            "\\\"ruber\\\",\\\"lectus\\\":\\\"ita\\\","
            "\\\"status_lecturae\\\":\\\"perditus\\\"}\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        {
            constans character* an = _plagula_litterae(piscina,
                VIA_AN);

            CREDO_VERUM (strstr(an, "violatio camporum") != NIHIL);
            CREDO_VERUM (strstr(an, "integer expectatus")
                != NIHIL);
            CREDO_VERUM (strstr(an, "clavis extra campos")
                != NIHIL);
            CREDO_VERUM (strstr(an, "veritas (boolean) expectata")
                != NIHIL);
            CREDO_VERUM (strstr(an, "valor extra optiones"
                " electionis") != NIHIL);
        }

        /* relatio unum: vinculum primum mundum, alterum notatum */
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":977,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"nexus\",\"verbum\":\"auctor\",\"alterum\":\"%s\""
            "}}}", liber_a, auctor_a);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "--auctor-->") != NIHIL);
        CREDO_VERUM (strstr(_plagula_litterae(piscina, VIA_AN),
            "cardinalitas unum excessa") == NIHIL);
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":978,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"nexus\",\"verbum\":\"auctor\",\"alterum\":\"%s\""
            "}}}", liber_a, auctor_b);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "--auctor-->") != NIHIL);
        CREDO_VERUM (strstr(_plagula_litterae(piscina, VIA_AN),
            "cardinalitas unum excessa") != NIHIL);

        /* legere: datum insertum (annus signatus) + tabulatum
         * nexus cum titulo destinationis resoluto */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":979,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":\"liber\"}}}");
        /* textus instrumenti = JSON intra JSON - virgulae in
         * responso crudo escapatae (\") */
        CREDO_VERUM (strstr(r, "\\\"annus\\\":-52") != NIHIL);
        CREDO_VERUM (strstr(r, "\\\"nexus\\\"") != NIHIL);
        CREDO_VERUM (strstr(r, "\\\"verbum\\\":\\\"auctor\\\"")
            != NIHIL);
        CREDO_VERUM (strstr(r, "\\\"ad_titulus\\\":\\\"Gaius"
            " Iulius Caesar\\\"") != NIHIL);


        /* ================================================
         * 'sine_campis' + 'res' in legere (01KYCMQMED): lista
         * tacere potest, apertio complet.
         *
         * Cur AMBO simul et non suppressio per MENSURAM: campi
         * longi legitimi ubique sunt - articuli, scholia, codices
         * omnes 'corpus' ex hac ipsa via reddunt, ergo regula
         * mensurae caeca faciem fori truncaret. Suppressio ergo
         * NOMINATA est et vocatoris electio; et vocator viam ad
         * campum recipiendum habere DEBET, alioquin sartura datum
         * simpliciter perdit - inde 'res'.
         * ================================================ */

        {
            constans character* s;
            constans character* p;
                     character  rid_l[27];
                           i32  numerus = ZEPHYRUM;

            /* (a) campus nominatus TOLLITUR; mensura in clave nova
             * manet; ceteri INTACTI */
            s = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":981,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"legere\",\"arguments\":{\"genus\":\"liber\","
                "\"sine_campis\":\"annus\"}}}");
            CREDO_VERUM (strstr(s, "\\\"annus\\\":-52") == NIHIL);
            CREDO_VERUM (strstr(s, "\\\"annus_omissus\\\"") != NIHIL);
            CREDO_VERUM (strstr(s, "\\\"nexus\\\"") != NIHIL);

            /* (b) nomen SIMILE non capitur: comparatio TOTA est,
             * non substantia ('ann' 'annus' non tollat) */
            s = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":982,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"legere\",\"arguments\":{\"genus\":\"liber\","
                "\"sine_campis\":\"ann\"}}}");
            CREDO_VERUM (strstr(s, "\\\"annus\\\":-52") != NIHIL);

            /* (c) 'res' UNUM ens reddit (apertio dato pleno) */
            s = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":983,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"addere\",\"arguments\":{\"genus\":\"nota\","
                "\"titulus\":\"Unica Lectio Probanda\",\"tags\":"
                "\"lectioprobanda\"}}}");
            _res_id_ex_responso(s, rid_l);
            sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":984,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"legere\",\"arguments\":{\"res\":\"%s\"}}}",
                rid_l);
            s = _mitte(t, piscina, linea);
            CREDO_VERUM (strstr(s, "Unica Lectio Probanda") != NIHIL);
            p = s;
            dum ((p = strstr(p, "\\\"res_id\\\"")) != NIHIL)
            {
                numerus++;
                p++;
            }
            CREDO_VERUM (numerus == I);
        }

        /* emendatio definitionis (G2.2 - emendatio PLENA licita):
         * additiva munda; destructiva SINE nota scripturae
         * (evolutio legitima - orphani per salutem apparent);
         * clavis generis sola immutabilis */
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":980,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"mutatio\",\"datum\":\"{\\\"campi\\\":"
            "[{\\\"clavis\\\":\\\"appellatio\\\",\\\"typus\\\":"
            "\\\"textus\\\"},{\\\"clavis\\\":\\\"floruit\\\","
            "\\\"typus\\\":\\\"annus\\\"}]}\"}}}", def_auctor);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "eventum mutatio scriptum")
            != NIHIL);
        CREDO_VERUM (strstr(_plagula_litterae(piscina, VIA_AN),
            "emendatio destructiva") == NIHIL);
        /* ante emendationem destructivam: insalubris unus solus
         * (Liber Pravus - typi violati) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":990,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"census\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "insalubres 1") != NIHIL);
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":981,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"mutatio\",\"datum\":\"{\\\"campi\\\":"
            "[{\\\"clavis\\\":\\\"floruit\\\",\\\"typus\\\":"
            "\\\"annus\\\"}]}\"}}}", def_auctor);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "eventum mutatio scriptum")
            != NIHIL);
        CREDO_VERUM (strstr(_plagula_litterae(piscina, VIA_AN),
            "emendatio destructiva") == NIHIL);
        /* post remotionem campi appellatio: auctores II orphani
         * (salus campi - superficies reparationis) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":991,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"census\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "insalubres 3") != NIHIL);
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":992,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}", auctor_a);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "[cautio]") != NIHIL);
        CREDO_VERUM (strstr(r, "extra campos definitionis")
            != NIHIL);
        sprintf(linea, "{\"jsonrpc\":\"2.0\",\"id\":982,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"mutatio\",\"datum\":\"{\\\"clavis\\\":"
            "\\\"scriptor\\\"}\"}}}", def_auctor);
        r = _mitte(t, piscina, linea);
        CREDO_VERUM (strstr(r, "eventum mutatio scriptum")
            != NIHIL);
        CREDO_VERUM (strstr(_plagula_litterae(piscina, VIA_AN),
            "clavis immutabilis") != NIHIL);

        /* collisio clavis: definitio super genus systematis -
         * notata, registrum systematis NON clobberatum */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":983,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"definitio\","
            "\"titulus\":\"Quaestio Falsa\",\"datum\":"
            "\"{\\\"clavis\\\":\\\"quaestio\\\",\\\"campi\\\":[]"
            "}\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        CREDO_VERUM (strstr(_plagula_litterae(piscina, VIA_AN),
            "clavis generis iam occupata") != NIHIL);
        {
            GestaMundus* sonda = gesta_aperire(piscina, VIA_DB,
                VIA_AN);

            CREDO_NON_NIHIL (sonda);
            si (sonda != NIHIL)
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
                    CREDO_VERUM (strstr(buf, "\"machina\"")
                        != NIHIL);
                    CREDO_VERUM (strstr(buf, "\"campi\"")
                        == NIHIL);
                }
                gesta_claudere(sonda);
            }
        }

        /* ab_lecto discriminat: entia generum usoris = data (non
         * epistulae), definitiones = epistulae; nota custodiae
         * nexus datorum quoque exclusa (in annalibus manet) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":984,"
            "\"method\":\"initialize\",\"params\":{}}");
        CREDO_VERUM (strstr(r, "NOVA:") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":985,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"acta\",\"arguments\":{\"ab_lecto\":\"verum\","
            "\"quantum\":\"200\"}}}");
        CREDO_VERUM (strstr(r, "acta non lecta") != NIHIL);
        CREDO_VERUM (strstr(r, "Quaestio Falsa") != NIHIL);
        CREDO_VERUM (strstr(r, "Commentarii") == NIHIL);
        CREDO_VERUM (strstr(r, "Gaius Iulius Caesar") == NIHIL);
        CREDO_VERUM (strstr(r, "cardinalitas unum excessa")
            == NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":986,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"acta\",\"arguments\":{\"ab_lecto\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "(nihil novi)") != NIHIL);

        /* numeratio novorum eodem praedicato (G1): ens usoris
         * novum SOLUM -> NOVA tacet, lectio nihil - salutatio
         * numquam promittit quod lectio negabit */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":987,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"auctor\","
            "\"titulus\":\"Titus Livius\",\"datum\":"
            "\"{\\\"floruit\\\":-10}\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":988,"
            "\"method\":\"initialize\",\"params\":{}}");
        CREDO_VERUM (strstr(r, "NOVA:") == NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":989,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"acta\",\"arguments\":{\"ab_lecto\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "(nihil novi)") != NIHIL);
        CREDO_VERUM (strstr(r, "plura restant") == NIHIL);
    }

    /* XXII. captura fragmentorum (01KYRGEGV4): bloci <genus> in
     * articulis - lineae '<# (>' / '<(>' res gignunt, stampantur
     * praefixo ULID mintae fortuitae; genus natum tacite; editio
     * titulum renovat; deletio orphanam signat. */
    {
        character articulus_id[GESTA_RES_ID_MENSURA];
        character stampa[XXVII];
        character imperium[PROBATIO_SEMITA_MENSURA];

        /* creatio cum bloco: linea hash + linea sacchari */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":800,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"articulus\","
            "\"titulus\":\"cogitationes hodiernae\",\"corpus\":"
            "\"prooemium liberum\\n<ideas>\\n<# (> eat a cake\\n"
            "<(> alter cibus\\n</ideas>\\npostscriptum\","
            "\"actor\":\"fran\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        CREDO_VERUM (strstr(r, "capturae:") != NIHIL);
        CREDO_VERUM (strstr(r, "res novae 2 stampatae") != NIHIL);
        CREDO_VERUM (strstr(r, "genus natum: ideas") != NIHIL);
        _res_id_ex_responso(r, articulus_id);
        CREDO_VERUM (articulus_id[0] != '\0');

        /* corpus stampatum: formae authoris abierunt, stampae
         * adsunt; textus circumiacens intactus */
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":801,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}",
            articulus_id);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "<#01") != NIHIL);
        CREDO_VERUM (strstr(r, "<# (>") == NIHIL);
        CREDO_VERUM (strstr(r, "<(>") == NIHIL);
        CREDO_VERUM (strstr(r, "prooemium liberum") != NIHIL);
        CREDO_VERUM (strstr(r, "postscriptum") != NIHIL);
        _stampa_ex_responso(r, stampa);
        CREDO_VERUM ((i32)strlen(stampa) >= XII);

        /* definitio nata in registrum plicata (clavis ideas) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":802,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":"
            "\"definitio\"}}}");
        CREDO_VERUM (strstr(r, "\\\"clavis\\\":\\\"ideas\\\"")
            != NIHIL || strstr(r, "\"clavis\":\"ideas\"")
            != NIHIL);

        /* res capta per stampam resolvitur; titulus = textus
         * lineae; campus textus (definitionis natae) idem */
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":803,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}", stampa);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "eat a cake") != NIHIL);
        CREDO_VERUM (strstr(r, "ideas") != NIHIL);
        /* genus in fasce ipso natum = notum validationi (obumbra
         * definitionis) - NULLA nota custodiae 'genus ignotum' */
        CREDO_VERUM (strstr(r, "nota (machina)") == NIHIL);

        /* editio lineae stampatae -> titulus rei renovatur */
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":804,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"mutatio\",\"datum\":\"{\\\"corpus\\\":"
            "\\\"<ideas>\\\\n<#%s (> eat a cake NOW\\\\n"
            "</ideas>\\\"}\",\"actor\":\"fran\"}}}",
            articulus_id, stampa);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "eventum mutatio scriptum")
            != NIHIL);
        CREDO_VERUM (strstr(r, "tituli renovati 1") != NIHIL);
        /* linea sacchari abiit -> res altera orphana */
        CREDO_VERUM (strstr(r, "orphanae 1") != NIHIL);
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":805,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}", stampa);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "eat a cake NOW") != NIHIL);

        /* deletio lineae restantis -> orphana; res vivit */
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":806,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"mutatio\",\"datum\":\"{\\\"corpus\\\":"
            "\\\"<ideas>\\\\n</ideas>\\\"}\",\"actor\":"
            "\"fran\"}}}", articulus_id);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "orphanae 1") != NIHIL);
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":807,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}", stampa);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "eat a cake NOW") != NIHIL);
        CREDO_VERUM (strstr(r, "orphanus") != NIHIL);

        /* linea rediviva -> signum orphanae tollitur */
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":808,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"mutatio\",\"datum\":\"{\\\"corpus\\\":"
            "\\\"<ideas>\\\\n<#%s (> eat a cake NOW\\\\n"
            "</ideas>\\\"}\",\"actor\":\"fran\"}}}",
            articulus_id, stampa);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "eventum mutatio scriptum")
            != NIHIL);
        CREDO_VERUM (strstr(r, "redivivae 1") != NIHIL);
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":809,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}", stampa);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "orphanus") == NIHIL);

        /* extra blocum nihil capitur; stampa insoluta praeteritur */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":810,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"articulus\","
            "\"titulus\":\"margines\",\"corpus\":"
            "\"<# (> extra blocum\\n<ideas>\\n"
            "<#01AAAAAAAAAA (> phantasma\\n</ideas>\","
            "\"actor\":\"fran\"}}}");
        CREDO_VERUM (strstr(r, "lineae praeteritae 1") != NIHIL);
        _res_id_ex_responso(r, articulus_id);
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":811,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}",
            articulus_id);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "<# (> extra blocum") != NIHIL);
        CREDO_VERUM (strstr(r, "01AAAAAAAAAA") != NIHIL);

        /* pipatum: transitus quiescit (articuli soli) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":812,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"pipatum\","
            "\"titulus\":\"pipatum cum bloco\",\"corpus\":"
            "\"<ideas>\\n<# (> non capienda\\n</ideas>\","
            "\"actor\":\"fran\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        CREDO_VERUM (strstr(r, "capturae:") == NIHIL);

        /* clausura anonyma </> blocum claudit (01KYSPRF9R) -
         * linea capturae POST eam extra blocum est (fuga quam
         * accidens Franis paene fecit) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":813,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"articulus\","
            "\"titulus\":\"clausura anonyma\",\"corpus\":"
            "\"<ideas>\\n<# (> intra blocum\\n</>\\n"
            "<# (> post clausuram\\n\",\"actor\":\"fran\"}}}");
        CREDO_VERUM (strstr(r, "res novae 1 stampatae") != NIHIL);
        _res_id_ex_responso(r, articulus_id);
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":814,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}",
            articulus_id);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "<# (> post clausuram") != NIHIL);

        /* legere per praefixum (01KYR7XMM9: praefixum antea []
         * tacite) - stampa XII signorum ens plenum reddit */
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":815,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"res\":\"%s\"}}}", stampa);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "eat a cake NOW") != NIHIL);
        CREDO_VERUM (strstr(r, "\\\"genus\\\":\\\"ideas\\\"")
            != NIHIL || strstr(r, "\"genus\":\"ideas\"")
            != NIHIL);
    }

    /* XV bis. legere: filtrum nexus (01KYZG0V0K). Sine eo consumptor
     * genus TOTUM legere et clientis-latere seligere debet, ergo
     * tectum CC per res OMNES generis partitur - capitula omnium
     * librorum uno pretio, et truncatio ut 'liber hic pauca capitula
     * habet' legitur. */
    {
        character liber_a[GESTA_RES_ID_MENSURA];
        character liber_b[GESTA_RES_ID_MENSURA];
        character praefixum[XI];
        character imperium[PROBATIO_SEMITA_MENSURA];
        character capitulum_a[GESTA_RES_ID_MENSURA];
        character capitulum_b[GESTA_RES_ID_MENSURA];
              i32 k;

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":820,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"decretum\","
            "\"titulus\":\"liber alpha\"}}}");
        _res_id_ex_responso(r, liber_a);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":821,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"decretum\","
            "\"titulus\":\"liber beta\"}}}");
        _res_id_ex_responso(r, liber_b);

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":822,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"capitulum alpha\"}}}");
        _res_id_ex_responso(r, capitulum_a);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":823,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"capitulum beta\"}}}");
        _res_id_ex_responso(r, capitulum_b);
        /* nota SINE ullo nexu - custos regressionis infra */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":824,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"nota solitaria\"}}}");

        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":825,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"nexus\",\"verbum\":\"liber\",\"alterum\":\"%s\"}}}",
            capitulum_a, liber_a);
        r = _mitte(t, piscina, imperium);
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":826,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"%s\",\"actus\":"
            "\"nexus\",\"verbum\":\"liber\",\"alterum\":\"%s\"}}}",
            capitulum_b, liber_b);
        r = _mitte(t, piscina, imperium);

        /* verbum + destinatio: capitula UNIUS libri */
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":827,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":\"nota\","
            "\"nexus_verbum\":\"liber\",\"nexus_ad\":\"%s\"}}}",
            liber_a);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "capitulum alpha") != NIHIL);
        CREDO_VERUM (strstr(r, "capitulum beta") == NIHIL);
        CREDO_VERUM (strstr(r, "nota solitaria") == NIHIL);

        /* CUSTOS REGRESSIONIS: lectio SINE filtro res sine nexu
         * ferre DEBET. Si custodia utrumque-vacuum caderet, EXISTS
         * ad 'nexum quemlibet habet' degeneraret et omnis lectio
         * fori res nexu carentes TACITE amitteret - damnum longe
         * maius quam functio quam addimus. */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":828,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":\"nota\"}}}");
        CREDO_VERUM (strstr(r, "nota solitaria") != NIHIL);
        CREDO_VERUM (strstr(r, "capitulum alpha") != NIHIL);
        CREDO_VERUM (strstr(r, "capitulum beta") != NIHIL);

        /* destinatio sola (verbum quodlibet) */
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":829,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":\"nota\","
            "\"nexus_ad\":\"%s\"}}}", liber_b);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "capitulum beta") != NIHIL);
        CREDO_VERUM (strstr(r, "capitulum alpha") == NIHIL);

        /* verbum solum: ambo capitula, nota solitaria non */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":830,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":\"nota\","
            "\"nexus_verbum\":\"liber\"}}}");
        CREDO_VERUM (strstr(r, "capitulum alpha") != NIHIL);
        CREDO_VERUM (strstr(r, "capitulum beta") != NIHIL);
        CREDO_VERUM (strstr(r, "nota solitaria") == NIHIL);

        /* verbum ignotum: nihil (congruentia EXACTA, non substantia
         * - LIKE hic characterem vagum ab usore acciperet) */
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":831,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":\"nota\","
            "\"nexus_verbum\":\"lib\",\"nexus_ad\":\"%s\"}}}",
            liber_a);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "capitulum alpha") == NIHIL);

        /* character vagus LIKE ut verbum litterale tractatur */
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":832,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":\"nota\","
            "\"nexus_verbum\":\"%%\",\"nexus_ad\":\"%s\"}}}",
            liber_a);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "capitulum alpha") == NIHIL);

        /* praefixum ULID in destinatione resolvitur (comitas eadem
         * quam 'res' habet) */
        per (k = ZEPHYRUM; k < X; k++) praefixum[k] = liber_a[k];
        praefixum[X] = '\0';
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":833,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"legere\",\"arguments\":{\"genus\":\"nota\","
            "\"nexus_verbum\":\"liber\",\"nexus_ad\":\"%s\"}}}",
            praefixum);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "capitulum alpha") != NIHIL);
        CREDO_VERUM (strstr(r, "capitulum beta") == NIHIL);
    }

    /* XV ter. TYPUS 'pagina': designatio, non numerus.
     * Liber duas series fert (prooemium i-xlii, corpus 1-380) et
     * 'xii' et '12' paginae DIVERSAE sunt - integer solus eas
     * exprimere nequit. Machina hoc iudicat, ne designatio prava
     * per MCP intret quam applicatio numquam scriberet. */
    {
        character imperium[PROBATIO_SEMITA_MENSURA];
        character ens_id[GESTA_RES_ID_MENSURA];

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":840,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"definitio\","
            "\"titulus\":\"Marginale\",\"datum\":\"{\\\"clavis\\\":"
            "\\\"marginale\\\",\\\"campi\\\":[{\\\"clavis\\\":"
            "\\\"nota\\\",\\\"typus\\\":\\\"textus\\\"},"
            "{\\\"clavis\\\":\\\"pag\\\",\\\"typus\\\":"
            "\\\"pagina\\\"}],\\\"campus_tituli\\\":\\\"nota\\\"}\""
            "}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);

        /* Romana minuscula: prooemium */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":841,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"marginale\","
            "\"titulus\":\"in praefatione\",\"datum\":"
            "\"{\\\"nota\\\":\\\"in praefatione\\\",\\\"pag\\\":"
            "\\\"xii\\\"}\"}}}");
        _res_id_ex_responso(r, ens_id);
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":842,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}", ens_id);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "xii") != NIHIL);
        CREDO_VERUM (strstr(r, "invalida") == NIHIL);

        /* INTEGER quoque licet: typus superset est, ergo paginae
         * iam scriptae (numerus) valent et retypatio orphanos non
         * parit */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":843,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"marginale\","
            "\"titulus\":\"in corpore\",\"datum\":"
            "\"{\\\"nota\\\":\\\"in corpore\\\",\\\"pag\\\":42}\"}}}");
        _res_id_ex_responso(r, ens_id);
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":844,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}", ens_id);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "invalida") == NIHIL);
        CREDO_VERUM (strstr(r, "expectata") == NIHIL);

        /* PRAVA nominatim recusantur - sed NOTA CUSTODIAE in ente,
         * non in responso 'addere' (responsum successum mundum
         * reddit; violatio in notis sepelitur). Ergo per 'res'
         * quaerendum est, aliter probatio semper viridis esset. */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":845,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"marginale\","
            "\"titulus\":\"prava\",\"datum\":"
            "\"{\\\"nota\\\":\\\"prava\\\",\\\"pag\\\":"
            "\\\"p. 12\\\"}\"}}}");
        _res_id_ex_responso(r, ens_id);
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":8451,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}", ens_id);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "invalida") != NIHIL);

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":846,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"marginale\","
            "\"titulus\":\"nulla\",\"datum\":"
            "\"{\\\"nota\\\":\\\"nulla\\\",\\\"pag\\\":0}\"}}}");
        _res_id_ex_responso(r, ens_id);
        sprintf(imperium, "{\"jsonrpc\":\"2.0\",\"id\":8461,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"%s\"}}}", ens_id);
        r = _mitte(t, piscina, imperium);
        CREDO_VERUM (strstr(r, "positiva") != NIHIL);
    }

    /* XVI. renovatio sui (01KYQ4T5EE): explorator praevius =
     * launcher -struere ut infans; defectus = residens vivus;
     * successus = renovandum (stdio: exec in currere; daemon:
     * exitus). Stipulae pro launchero - semantica launcheri veri
     * in fumo vivo probatur. */
    {
                    Tabularium* tr;
        TabulariumConfiguratio  cfgr = cfg;
                      Sigillum  s;
                     character  hex[SIGILLUM_HEX_MENSURA];
            constans character* contentum = "binarium renovandum";

        /* recens (vigilia quieta): nihil agendum */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":990,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"renovare\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "nihil agendum") != NIHIL);
        CREDO_VERUM (!tabularium_renovandum(t));

        /* tools/list instrumentum fert */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":991,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "\"renovare\"") != NIHIL);

        /* stipulae launcheri */
        CREDO_VERUM (_plagulam_scribere(VIA_STIPULA_FRACTA,
            "#!/bin/sh\nexit 1\n"));
        CREDO_VERUM (_plagulam_scribere(VIA_STIPULA_SANA,
            "#!/bin/sh\necho cafe1234deadbeef\nexit 0\n"));
        (vacuum)system("chmod +x " VIA_STIPULA_FRACTA " "
            VIA_STIPULA_SANA);

        /* fixtura vigiliae stalandae (exemplar XV) */
        CREDO_VERUM (_plagulam_scribere(VIA_BN2, contentum));
        CREDO_VERUM (_plagulam_scribere(VIA_MN2, VIA_FN2 "\n"));
        s = sigillum_computare(contentum, strlen(contentum));
        sigillum_hex(&s, hex);
        cfgr.signum         = hex;
        cfgr.via_binarii    = VIA_BN2;
        cfgr.via_manifesti  = VIA_MN2;

        /* stipula fracta: RECUSATA, residens vivus */
        cfgr.via_renovatoris  = VIA_STIPULA_FRACTA;
        tr                    = tabularium_creare(piscina, &cfgr);
        CREDO_NON_NIHIL (tr);
        si (tr != NIHIL)
        {
            r = _mitte(tr, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":992,\"method\":\"initialize\",\"params\":"
                "{}}");
            CREDO_VERUM (_plagulam_scribere(VIA_FN2,
                "integer x;\n"));
            r = _mitte(tr, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":993,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"renovare\",\"arguments\":{}}}");
            CREDO_VERUM (strstr(r, "RECUSATA") != NIHIL);
            CREDO_VERUM (strstr(r, "residens vivus") != NIHIL);
            CREDO_VERUM (!tabularium_renovandum(tr));
        }

        /* stipula sana, modus stdio: parata + transformatio */
        cfgr.via_renovatoris  = VIA_STIPULA_SANA;
        tr                    = tabularium_creare(piscina, &cfgr);
        CREDO_NON_NIHIL (tr);
        si (tr != NIHIL)
        {
            r = _mitte(tr, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":994,\"method\":\"initialize\",\"params\":"
                "{}}");
            CREDO_VERUM (_plagulam_scribere(VIA_FN2,
                "integer y;\n"));
            r = _mitte(tr, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":995,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"renovare\",\"arguments\":{}}}");
            CREDO_VERUM (strstr(r, "renovatio parata") != NIHIL);
            CREDO_VERUM (strstr(r, "cafe1234deadbeef") != NIHIL);
            CREDO_VERUM (strstr(r, "me transformo") != NIHIL);
            CREDO_VERUM (tabularium_renovandum(tr));
        }

        /* stipula sana, modus daemon: exitus pro exec */
        cfgr.renovatio_exitus  = VERUM;
        tr                     = tabularium_creare(piscina, &cfgr);
        CREDO_NON_NIHIL (tr);
        si (tr != NIHIL)
        {
            r = _mitte(tr, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":996,\"method\":\"initialize\",\"params\":"
                "{}}");
            CREDO_VERUM (_plagulam_scribere(VIA_FN2,
                "integer z;\n"));
            r = _mitte(tr, piscina, "{\"jsonrpc\":\"2.0\","
                "\"id\":997,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"renovare\",\"arguments\":{}}}");
            CREDO_VERUM (strstr(r, "exeo") != NIHIL);
            CREDO_VERUM (tabularium_renovandum(tr));
        }
    }


    /* ==================================================
     * XXXI. VISUS PARATA (2026-09-21, parcum K4.5): quid NUNC agi
     * potest, ex grapho DERIVATUM (impeditur-a + intra), numquam
     * status declaratus. Mensuratum ante: ex IX vinculis impediendi
     * in IV impediens iam clausum erat dum dependens apertum manebat,
     * et nemo sciebat.
     * Mundus: propositum -> gradus unus (opera I.1, I.2 <- I.1, I.3
     * Frano assignatum), gradus duo <- gradus unus (opus II.1 nepos);
     * res solitaria (extra graphum); res extra scopum cuius impediens
     * clausum est.
     * ================================================== */

    {
        constans character* sectio;

        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":301,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"parcum\",\"titulus\":\"Pa propositum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":302,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"parcum\",\"titulus\":\"Pa gradus unus\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":303,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"parcum\",\"titulus\":\"Pa gradus duo\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":304,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"opus\",\"titulus\":\"Pa I.1 primum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":305,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"opus\",\"titulus\":\"Pa I.2 secundum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":306,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"opus\",\"titulus\":\"Pa I.3 Frani\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":307,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"opus\",\"titulus\":\"Pa II.1 nepos\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":308,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"desideratum\",\"titulus\":\"Pa solitarium\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":309,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"desideratum\",\"titulus\":\"Pa extra scopum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":310,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"quaestio\",\"titulus\":\"Pa quaestio clausa\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":311,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa gradus unus\",\"actus\":\"nexus\",\"verbum\":\"int"
            "ra\",\"alterum\":\"Pa propositum\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":312,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa gradus duo\",\"actus\":\"nexus\",\"verbum\":\"intr"
            "a\",\"alterum\":\"Pa propositum\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":313,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa gradus duo\",\"actus\":\"nexus\",\"verbum\":\"impe"
            "ditur-a\",\"alterum\":\"Pa gradus unus\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":314,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa I.1 primum\",\"actus\":\"nexus\",\"verbum\":\"intr"
            "a\",\"alterum\":\"Pa gradus unus\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":315,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa I.2 secundum\",\"actus\":\"nexus\",\"verbum\":\"in"
            "tra\",\"alterum\":\"Pa gradus unus\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":316,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa I.3 Frani\",\"actus\":\"nexus\",\"verbum\":\"intra"
            "\",\"alterum\":\"Pa gradus unus\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":317,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa II.1 nepos\",\"actus\":\"nexus\",\"verbum\":\"intr"
            "a\",\"alterum\":\"Pa gradus duo\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":318,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa I.2 secundum\",\"actus\":\"nexus\",\"verbum\":\"im"
            "peditur-a\",\"alterum\":\"Pa I.1 primum\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":319,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa extra scopum\",\"actus\":\"nexus\",\"verbum\":\"im"
            "peditur-a\",\"alterum\":\"Pa quaestio clausa\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":320,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa quaestio clausa\",\"actus\":\"status\",\"novus\":"
            "\"clausum\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":321,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa I.3 Frani\",\"actus\":\"mutatio\",\"clavis\":\"ass"
            "ignatum\",\"valor\":\"fran\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);

        /* STATUS I: sectiones per classem */
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":322,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "AD LABOREM") != NIHIL);
        CREDO_VERUM (strstr(r, "Pa I.1 primum") != NIHIL);
        CREDO_VERUM (strstr(r, "EXSPECTANT FRANUM") != NIHIL);
        CREDO_VERUM (strstr(r, "AD CONSILIUM") != NIHIL);
        CREDO_VERUM (strstr(r, "IMPEDITA") != NIHIL);

        /* opus paratum in AD LABOREM; potentia eius nominatur */
        sectio = strstr(r, "AD LABOREM");
        CREDO_NON_NIHIL (sectio);
        CREDO_VERUM (_inter(r, "AD LABOREM", "AD CONSILIUM",
            "Pa I.1 primum"));
        CREDO_VERUM (strstr(r, "impedit 1") != NIHIL);
        /* opus impeditum NON in AD LABOREM, sed in IMPEDITA cum
         * impediente nominato */
        CREDO_FALSUM (_inter(r, "AD LABOREM", "AD CONSILIUM",
            "Pa I.2 secundum"));
        CREDO_VERUM (_inter(r, "IMPEDITA", NIHIL,
            "Pa I.2 secundum"));
        /* HEREDITAS: nepos gradus IMPEDITI paratus non est, etsi
         * ipse impedientia nulla habet */
        CREDO_FALSUM (_inter(r, "AD LABOREM", "AD CONSILIUM",
            "Pa II.1 nepos"));
        CREDO_VERUM (_inter(r, "IMPEDITA", NIHIL, "Pa II.1 nepos"));
        CREDO_VERUM (strstr(r, "parens") != NIHIL);
        /* assignatum fran: EXSPECTANT FRANUM, non AD LABOREM */
        CREDO_FALSUM (_inter(r, "AD LABOREM", "AD CONSILIUM",
            "Pa I.3 Frani"));
        CREDO_VERUM (_inter(r, "EXSPECTANT FRANUM", "IMPEDITA",
            "Pa I.3 Frani"));
        /* impediens clausum: res in AD CONSILIUM cum CAUSA */
        CREDO_VERUM (_inter(r, "AD CONSILIUM", "AD CLAUSURAM",
            "Pa extra scopum"));
        CREDO_VERUM (strstr(r, "Pa quaestio clausa") != NIHIL);
        /* res EXTRA GRAPHUM numquam: tabularium basis scientiae est,
         * catena = quod in graphum positum est */
        CREDO_VERUM (strstr(r, "Pa solitarium") == NIHIL);
        /* gradus cum filiis apertis = continens: in NULLA classe
         * (nec consilio nec - quod planta ostendit - clausurae) */
        CREDO_FALSUM (_inter(r, "AD CONSILIUM", "AD CLAUSURAM",
            "parcum/parcatum  Pa gradus unus"));
        CREDO_FALSUM (_inter(r, "AD CLAUSURAM", "EXSPECTANT FRANUM",
            "parcum/parcatum  Pa gradus unus"));

        /* SCOPUS: intra propositum - res extra id abest */
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":323,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"parata\",\"arguments\":{\"intra"
            "\":\"Pa propositum\"}}}");
        CREDO_VERUM (strstr(r, "Pa I.1 primum") != NIHIL);
        CREDO_VERUM (strstr(r, "Pa extra scopum") == NIHIL);

        /* STATUS II: I.1 perficitur -> I.2 paratum, causa nominata */
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":324,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa I.1 primum\",\"actus\":\"status\",\"novus\":\"perf"
            "ectum\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":325,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"parata\",\"arguments\":{\"intra"
            "\":\"Pa propositum\"}}}");
        CREDO_VERUM (_inter(r, "AD LABOREM", "AD CONSILIUM",
            "Pa I.2 secundum"));
        CREDO_VERUM (_inter(r, "AD LABOREM", "AD CONSILIUM",
            "clausa"));
        /* opus PERFECTUM ordinem non habet (titulus eius in CAUSA
         * successoris legitime stat - ergo ordo per statum
         * quaeritur, non per titulum nudum) */
        CREDO_FALSUM (_inter(r, "AD LABOREM", "AD CONSILIUM",
            "opus/perfectum"));
        /* tabula.md: sectio PARATA adest; opus PERFECTUM e sectione
         * OPERA abest (status finales ex MACHINA generis derivati,
         * non index manu scriptus qui 'perfectum' nesciebat) */
        {
            constans character* tabula = _plagula_litterae(piscina,
                VIA_TB);

            CREDO_VERUM (strstr(tabula, "## PARATA") != NIHIL);
            CREDO_VERUM (strstr(tabula, "Pa I.2 secundum") != NIHIL);
            CREDO_VERUM (strstr(tabula,
                "[perfectum] Pa I.1 primum") == NIHIL);
        }

        /* STATUS III: omnia opera gradus unius perfecta -> gradus AD
         * CLAUSURAM (parens probationem exitus SUAM poscit: index
         * filiorum numquam completus scitur) */
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":326,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa I.2 secundum\",\"actus\":\"status\",\"novus\":\"pe"
            "rfectum\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":327,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa I.3 Frani\",\"actus\":\"status\",\"novus\":\"perfe"
            "ctum\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":328,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"parata\",\"arguments\":{\"intra"
            "\":\"Pa propositum\"}}}");
        CREDO_VERUM (_inter(r, "AD CLAUSURAM", "EXSPECTANT FRANUM",
            "Pa gradus unus"));

        /* STATUS IV: gradus unus clauditur -> hereditas solvitur,
         * nepos paratus */
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":329,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa gradus unus\",\"actus\":\"status\",\"novus\":\"cla"
            "usum\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":330,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"parata\",\"arguments\":{\"intra"
            "\":\"Pa propositum\"}}}");
        CREDO_VERUM (_inter(r, "AD LABOREM", "AD CONSILIUM",
            "Pa II.1 nepos"));

        /* impediens RELICTUM: solutum sed SIGNATUM (decretum
         * 01M32TEK3K) - dependens recogitandum, non tacite paratum */
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":331,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"desideratum\",\"titulus\":\"Pa post relictum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":332,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"addere\",\"arguments\":{\"genus"
            "\":\"quaestio\",\"titulus\":\"Pa quaestio relicta\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":333,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa post relictum\",\"actus\":\"nexus\",\"verbum\":\"i"
            "mpeditur-a\",\"alterum\":\"Pa quaestio relicta\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":334,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"gerere\",\"arguments\":{\"res\""
            ":\"Pa quaestio relicta\",\"actus\":\"status\",\"novus\":"
            "\"relictum\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
        r = _mitte(t, piscina,
            "{\"jsonrpc\":\"2.0\",\"id\":335,\"method\":\"tools/call"
            "\",\"params\":{\"name\":\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "Pa post relictum") != NIHIL);
        CREDO_VERUM (strstr(r, "RELICTUM") != NIHIL);

        /* ASSIGNATIO EXPRESSA rem in catenam ponit, ut vinculum:
         * quaestio SOLITARIA Frano assignata in EXSPECTANT FRANUM
         * stat (cursus vivus primus: quaestio doctrinae Frano
         * assignata ABERAT quia vinculum nullum ferebat) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":380,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Pa decisio solitaria\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":381,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Pa decisio solitaria\",\"actus\":\"mutatio\","
            "\"clavis\":\"assignatum\",\"valor\":\"fran\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":382,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (_inter(r, "EXSPECTANT FRANUM", "IMPEDITA",
            "quaestio/apertum  Pa decisio solitaria"));
        /* res solitaria NON assignata adhuc abest */
        CREDO_VERUM (strstr(r, "Pa solitarium") == NIHIL);

        /* TITULUS LONGUS decurtatur IN LIMITE CHARACTERIS: titulus
         * XCV octetorum ASCII + 'é' (II octeti, 0xC3 0xA9) limitem
         * XCVI in MEDIO characteris ponit - sectio ingenua octetum
         * 0xC3 nudum relinqueret (UTF-8 fractum in responso JSON) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":390,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"opus\","
            "\"titulus\":\"Pa longum aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "\xc3\xa9 cauda quae abesse debet\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":391,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "Pa longum aaaa") != NIHIL);
        CREDO_VERUM (strstr(r, "cauda quae abesse debet") == NIHIL);
        CREDO_VERUM (strstr(r, "a...") != NIHIL);
        CREDO_VERUM (strstr(r, "\xc3...") == NIHIL);

        /* schema instrumentum nominat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":399,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "\"parata\"") != NIHIL);
    }


    /* ==================================================
     * XXXII. CENSUS: numeri visus PARATA + tags TRUNCATA
     * (2026-09-21, opus 01M335DSK5). Mensuratum in conditorio vivo:
     * DXI tags distincta, CCLII semel usa - ~CDL lineae per
     * vocationem census, eo ipso momento (orientatio post
     * compactionem) quo contextus carissimus est.
     * ================================================== */

    {
        constans character* nomina[VI];
                       i32  numeri_visus[VI];
                 character  speratum[CCLVI];
                       i32  k;

        nomina[ZEPHYRUM]  = "AD LABOREM (";
        nomina[I]         = "AD CONSILIUM (";
        nomina[II]        = "AD CLAUSURAM (";
        nomina[III]       = "EXSPECTANT FRANUM (";
        nomina[IV]        = "AD COLLOQUIUM (";
        nomina[V]         = "IMPEDITA (";

        /* XXV tags singularia + unum FREQUENS quod alphabetice
         * ULTIMUM est (ordo per numerum id servare debet) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":400,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Census tagorum multorum\",\"tags\":"
            "\"tg01,tg02,tg03,tg04,tg05,tg06,tg07,tg08,tg09,tg10,tg11,tg12,tg13,tg14,tg15,tg16,tg17,tg18,tg19,tg20,tg21,tg22,tg23,tg24,tg25,zzfrequens\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":401,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Census frequens alter\",\"tags\":"
            "\"zzfrequens\"}}}");
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":402,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Census frequens tertius\",\"tags\":"
            "\"zzfrequens\"}}}");

        /* numeri visus ex INSTRUMENTO parata ipso sumuntur: census
         * eosdem ferre debet (fons unus, non numeratio altera) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":403,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        per (k = ZEPHYRUM; k < VI; k++)
        {
            constans character* locus = strstr(r, nomina[k]);

            CREDO_NON_NIHIL (locus);
            numeri_visus[k] = locus != NIHIL
                ? (i32)atoi(locus + strlen(nomina[k])) : ZEPHYRUM;
        }
        CREDO_VERUM (numeri_visus[ZEPHYRUM] > ZEPHYRUM);
        CREDO_AEQUALIS_I32 (numeri_visus[III], I);

        /* census ORDINARIUS */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":404,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"census\",\"arguments\":{}}}");
        sprintf(speratum, "parata: ad laborem %d, ad consilium %d,"
            " ad clausuram %d, exspectant Franum %d, ad colloquium"
            " %d, impedita %d",
            (int)numeri_visus[ZEPHYRUM], (int)numeri_visus[I],
            (int)numeri_visus[II], (int)numeri_visus[III],
            (int)numeri_visus[IV], (int)numeri_visus[V]);
        CREDO_VERUM (strstr(r, speratum) != NIHIL);
        /* tag FREQUENS adest etsi alphabetice ultimum */
        CREDO_VERUM (strstr(r, "zzfrequens  3") != NIHIL);
        /* singulare XXV-um truncatum est, et truncatio NUMERATUR
         * cum via ad omnia videnda */
        CREDO_VERUM (strstr(r, "tg25") == NIHIL);
        CREDO_VERUM (strstr(r, "et alia ") != NIHIL);
        CREDO_VERUM (strstr(r, "semel usa") != NIHIL);
        CREDO_VERUM (strstr(r, "omnia") != NIHIL);
        /* sectiones ceterae intactae */
        CREDO_VERUM (strstr(r, "genera x status:") != NIHIL);
        CREDO_VERUM (strstr(r, "seq ultima") != NIHIL);

        /* tags: "omnia" - nihil truncatur */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":405,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"census\",\"arguments\":{\"tags\":\"omnia\"}}}");
        CREDO_VERUM (strstr(r, "tg25") != NIHIL);
        CREDO_VERUM (strstr(r, "tg01") != NIHIL);
        CREDO_VERUM (strstr(r, "et alia ") == NIHIL);

        /* schema parametrum nominat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":406,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "semel usa") != NIHIL
            || strstr(r, "tags omnia") != NIHIL
            || strstr(r, "\\\"omnia\\\"") != NIHIL);
    }


    /* ==================================================
     * XXXIII. RECUSATIONES STATUS ET MUTATIONIS sub legibus decreti
     * 01M32X21NR (opus 01M335DZJM). Mensuratum ante: transitio
     * illegalis SCRIBEBATUR, nota custodiae in re, responsum
     * 'eventum status scriptum' SINE signo erroris - scriptor nihil
     * discebat, res in statu quem machina sua nescit sedebat,
     * insalubres 0. Nunc: recusatio ad ianuam, status legales
     * proximi ex MACHINA generis, scriptura valida, et 'vis' ut
     * lex 'scribe, ne obsta' ingenii CONSULTO invocari possit.
     * ================================================== */

    {
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":420,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"parcum\","
            "\"titulus\":\"Status sonda\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);

        /* TRANSITIO ILLEGALIS: recusata, nihil scriptum, status
         * currens et LEGALES PROXIMI nominati, scriptura valida */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":421,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Status sonda\","
            "\"actus\":\"status\",\"novus\":\"apertum\"}}}");
        CREDO_VERUM (strstr(r, "\"isError\":true") != NIHIL);
        /* 'nihil scriptum' in recusatione legitime stat - forma
         * successus quaeritur */
        CREDO_VERUM (strstr(r, "eventum status scriptum") == NIHIL);
        CREDO_VERUM (strstr(r, "'parcatum'") != NIHIL);
        CREDO_VERUM (strstr(r, "tractum | clausum") != NIHIL);
        CREDO_VERUM (strstr(r, "SCRIPTURA VALIDA") != NIHIL);
        CREDO_VERUM (strstr(r, "vis") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":422,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Status sonda\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "(parcum, parcatum)") != NIHIL);
        CREDO_VERUM (strstr(r, "violatio machinae") == NIHIL);

        /* STATUS IDEM: recusatur (eventus inanis) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":423,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Status sonda\","
            "\"actus\":\"status\",\"novus\":\"parcatum\"}}}");
        CREDO_VERUM (strstr(r, "\"isError\":true") != NIHIL);
        CREDO_VERUM (strstr(r, "iam") != NIHIL);

        /* NOVUS DEEST: causa cum legalibus (scriptor eligere potest
         * sine vocatione altera) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":424,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Status sonda\","
            "\"actus\":\"status\"}}}");
        CREDO_VERUM (strstr(r, "\"isError\":true") != NIHIL);
        CREDO_VERUM (strstr(r, "novus deest") != NIHIL);
        CREDO_VERUM (strstr(r, "tractum | clausum") != NIHIL);

        /* VIS: lex ingenii 'scribe, ne obsta' CONSULTO invocata -
         * scribitur, nota custodiae ut olim */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":425,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Status sonda\","
            "\"actus\":\"status\",\"novus\":\"apertum\","
            "\"vis\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "status apertum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":426,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Status sonda\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "violatio machinae") != NIHIL);

        /* GENUS SINE MACHINA (nota): status quilibet transit ut
         * olim - lex progressiva, nihil novi recusatur */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":427,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Status nota sonda\"}}}");
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":428,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Status nota sonda\",\"actus\":\"status\","
            "\"novus\":\"impletum\"}}}");
        CREDO_VERUM (strstr(r, "status impletum") != NIHIL);

        /* MUTATIO: causae omnes simul + scriptura valida */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":429,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Status sonda\","
            "\"actus\":\"mutatio\"}}}");
        CREDO_VERUM (strstr(r, "\"isError\":true") != NIHIL);
        CREDO_VERUM (strstr(r, "SCRIPTURA VALIDA") != NIHIL);
        CREDO_VERUM (strstr(r, "<clavis>") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":430,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Status sonda\","
            "\"actus\":\"mutatio\",\"datum\":\"[1,2]\"}}}");
        CREDO_VERUM (strstr(r, "\"isError\":true") != NIHIL);
        CREDO_VERUM (strstr(r, "obiectum") != NIHIL);
        CREDO_VERUM (strstr(r, "SCRIPTURA VALIDA") != NIHIL);
        /* clavis reservata 'status' per mutationem: causa nominata
         * (via recta = actus status) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":431,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Status sonda\","
            "\"actus\":\"mutatio\",\"clavis\":\"status\","
            "\"valor\":\"clausum\"}}}");
        CREDO_VERUM (strstr(r, "\"isError\":true") != NIHIL);
        CREDO_VERUM (strstr(r, "actus: \\\"status\\\"") != NIHIL);

        /* RES IGNOTA + NOVUS ILLEGALIS: causae DUAE responso uno
         * (numquam guttatim) - sine re nota legales dari nequeunt,
         * sed res ignota et novus absens simul nominantur */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":432,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Res nusquam\","
            "\"actus\":\"status\"}}}");
        CREDO_VERUM (strstr(r, "(2 causae)") != NIHIL);
        CREDO_VERUM (strstr(r, "ignota") != NIHIL);
        CREDO_VERUM (strstr(r, "novus deest") != NIHIL);

        /* schema 'vis' nominat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":433,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "\"vis\"") != NIHIL);
    }


    /* ==================================================
     * XXXIV. BREVIARIUM: effectus operis + filii parati parentis
     * (opus 01M34RACAD). Commissum effectum scribit ('hash: linea')
     * sed breviarium eum non monstrabat (in dato solo). Et vocatio
     * una in propositum arborem dabat, catenam non.
     * ================================================== */

    {
        /* effectus absens: nulla linea. NB responsum textus JSON
         * est: linea nova ut '\\n' (characteres duo) advenit -
         * assertiones id spectant */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":440,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Pa I.1 primum\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "\\neffectus ") == NIHIL);

        /* effectus positus (ritus perfectionis): linea adest */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":441,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Pa I.1 primum\","
            "\"actus\":\"mutatio\",\"clavis\":\"effectus\","
            "\"valor\":\"abc1234: res gesta est\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":442,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Pa I.1 primum\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "\\neffectus abc1234: res gesta est")
            != NIHIL);
        /* forma plena eandem lineam fert (functio communis) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":443,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Pa I.1 primum\"}}}");
        CREDO_VERUM (strstr(r, "\\neffectus abc1234: res gesta est")
            != NIHIL);

        /* effectus LONGUS decurtatur (breviarium lectori
         * context-budgetato servit) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":444,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Pa I.2 secundum\","
            "\"actus\":\"mutatio\",\"clavis\":\"effectus\","
            "\"valor\":\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"}}}");
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":445,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Pa I.2 secundum\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "\\neffectus aaaa") != NIHIL);
        CREDO_VERUM (strstr(r, "a...") != NIHIL);
        CREDO_VERUM (strstr(r,
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
                == NIHIL);

        /* PARENS: breviarium filios PARATOS enumerat per classem -
         * 'Pa propositum': gradus duo ('AD CLAUSURAM' non - gradus
         * unus clausus est, nepos II.1 paratus, gradus duo filium
         * apertum habet ergo continens); res extra propositum
         * absunt */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":446,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Pa propositum\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "parata sub hac re") != NIHIL);
        CREDO_VERUM (_inter(r, "parata sub hac re", "nexus:",
            "AD LABOREM"));
        CREDO_VERUM (_inter(r, "parata sub hac re", "nexus:",
            "Pa II.1 nepos"));
        CREDO_FALSUM (_inter(r, "parata sub hac re", "nexus:",
            "Pa extra scopum"));
        CREDO_FALSUM (_inter(r, "parata sub hac re", "nexus:",
            "Pa I.1 primum"));

        /* res SINE filiis: sectio abest omnino (spatium nullum) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":447,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Pa II.1 nepos\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "parata sub hac re") == NIHIL);

        /* parens cuius filii OMNES clausi: sectio dicit id (non
         * tacet - 'nihil paratum' a 'nihil sub ea' distinguitur) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":448,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Pa gradus unus\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "parata sub hac re") != NIHIL);
        CREDO_VERUM (strstr(r, "nihil paratum") != NIHIL);
    }


    /* ==================================================
     * XXXV. QUAERERE 'intra' (opus 01M335DW3M): quaesitio FTS intra
     * subarborem rei (ipsa + quae sub ea per 'intra' stant, ad
     * profunditatem). Mundus XXXI: Pa propositum > gradus unus/duo >
     * opera; 'Pa extra scopum' EXTRA. Verbum commune in corpore
     * ubique ponitur ut FTS omnes inveniat sine scopo.
     * ================================================== */

    {
        constans character* tituli[] = { "Pa propositum",
            "Pa gradus unus", "Pa I.1 primum", "Pa II.1 nepos",
            "Pa extra scopum" };
                 character nuntius[DXII];
                       i32 k;

        per (k = ZEPHYRUM; k < V; k++)
        {
            sprintf(nuntius, "{\"jsonrpc\":\"2.0\",\"id\":%d,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"gerere\",\"arguments\":{\"res\":\"%s\","
                "\"actus\":\"nota\",\"textus\":\"verbum xylophonum"
                " hic\"}}}", (int)(450 + k), tituli[k]);
            r = _mitte(t, piscina, nuntius);
            CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
        }

        /* sine scopo: omnes V (opera quoque, quia genus petitur
         * ... non: sine genere opera EXCLUDUNTUR - ergo III res
         * non-opera + numerus operum exclusorum) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":460,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":"
            "\"xylophonum\"}}}");
        CREDO_VERUM (strstr(r, "Pa propositum") != NIHIL);
        CREDO_VERUM (strstr(r, "Pa extra scopum") != NIHIL);
        CREDO_VERUM (strstr(r, "opera exclusa: 2") != NIHIL);

        /* INTRA propositum: res extra scopum ABEST; gradus (filius)
         * et res ipsa ADSUNT; opera sub ea numerantur ut exclusa
         * (exclusio operum servatur intra scopum quoque) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":461,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":"
            "\"xylophonum\",\"intra\":\"Pa propositum\"}}}");
        CREDO_VERUM (strstr(r, "Pa propositum") != NIHIL);
        CREDO_VERUM (strstr(r, "Pa gradus unus") != NIHIL);
        CREDO_VERUM (strstr(r, "Pa extra scopum") == NIHIL);
        CREDO_VERUM (strstr(r, "opera exclusa: 2") != NIHIL);
        CREDO_VERUM (strstr(r, "intra '") != NIHIL);

        /* intra + genus opus: opera sub propositum, PROFUNDITAS II
         * (nepos sub gradu duo) - opus extra scopum nullum */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":462,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":"
            "\"xylophonum\",\"intra\":\"Pa propositum\","
            "\"genus\":\"opus\"}}}");
        CREDO_VERUM (strstr(r, "Pa I.1 primum") != NIHIL);
        CREDO_VERUM (strstr(r, "Pa II.1 nepos") != NIHIL);
        /* caput scopum nominat ('intra ...') - ORDO propositi
         * abesse debet, non titulus */
        CREDO_VERUM (strstr(r, "parcum/parcatum  Pa propositum")
            == NIHIL);

        /* scopus angustior: intra gradus unus - nepos (sub gradu
         * DUO) abest */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":463,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":"
            "\"xylophonum\",\"intra\":\"Pa gradus unus\","
            "\"genus\":\"opus\"}}}");
        CREDO_VERUM (strstr(r, "Pa I.1 primum") != NIHIL);
        CREDO_VERUM (strstr(r, "Pa II.1 nepos") == NIHIL);

        /* nihil in scopo: responsum scopum nominat (ne 'nihil
         * inventum' nudum de tabulario toto legatur) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":464,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":"
            "\"xylophonum\",\"intra\":\"Pa solitarium\"}}}");
        CREDO_VERUM (strstr(r, "nihil inventum") != NIHIL);
        CREDO_VERUM (strstr(r, "intra '") != NIHIL);

        /* intra ignotum: recusatio clara */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":465,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":"
            "\"xylophonum\",\"intra\":\"Res nusquam\"}}}");
        CREDO_VERUM (strstr(r, "\"isError\":true") != NIHIL);
        CREDO_VERUM (strstr(r, "intra") != NIHIL);

        /* schema parametrum nominat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":466,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "SUBARBOREM") != NIHIL);
    }


    /* ==================================================
     * XXXVI. QUAESTIONES CONSILII (opus 01M34TE9Y5): quaestio cum
     * 'natura: consilium' = quaestio DESIGNI (non vitium), quae
     * propositum impedire potest. Visus parata sectionem AD
     * COLLOQUIUM fert: quaestiones consilii apertae ordine
     * potentiae, impeditis nominatis - catena colloquiorum, ut
     * catena operum. Mensuratum ante: XL quaestiones apertae,
     * XXXVIII vitia; quaestiones designi VERAE in prosa planorum
     * (XXI sectiones AUDIENDA) - a nullo instrumento legibiles.
     * Et parca aperta sine vinculo ullo (LXVIII) visui invisibilia
     * erant - 'visa, non fixa' nunc numerantur.
     * ================================================== */

    {
        /* propositum fictum + quaestio consilii quae id impedit +
         * quaestio consilii SOLITARIA (impedit nihil) + vitium
         * ordinarium (quaestio SINE natura) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":470,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"parcum\","
            "\"titulus\":\"Qc propositum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":471,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Qc nomina stratorum?\",\"natura\":"
            "\"consilium\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":472,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Qc gamma?\",\"natura\":"
            "\"consilium\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":473,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Qc vitium ordinarium\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":474,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Qc propositum\","
            "\"actus\":\"nexus\",\"verbum\":\"impeditur-a\","
            "\"alterum\":\"Qc nomina stratorum?\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);

        /* natura in breviario visibilis */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":475,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":"
            "\"Qc nomina stratorum?\",\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "natura consilium") != NIHIL);

        /* AD COLLOQUIUM: ambae quaestiones consilii (solitaria
         * quoque - natura eam in graphum ponit ut assignatio),
         * impediens PRIMA (potentia), impedita nominata; vitium
         * ordinarium ABEST; propositum in IMPEDITA cum quaestione
         * nominata */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":476,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "AD COLLOQUIUM (") != NIHIL);
        CREDO_VERUM (_inter(r, "AD COLLOQUIUM", "IMPEDITA",
            "Qc nomina stratorum?"));
        CREDO_VERUM (_inter(r, "AD COLLOQUIUM", "IMPEDITA",
            "Qc gamma?"));
        CREDO_VERUM (strstr(_inter_locus(r, "AD COLLOQUIUM",
            "Qc nomina stratorum?"), "impedit 1") != NIHIL);
        CREDO_VERUM (strstr(r, "Qc vitium ordinarium") == NIHIL);
        CREDO_VERUM (_inter(r, "IMPEDITA", NIHIL, "Qc propositum"));
        /* ORDO: impediens ante solitariam */
        CREDO_VERUM (strstr(r, "Qc nomina stratorum?")
            < strstr(r, "Qc gamma?"));

        /* quaestio consilii in NULLA classe ALIA (non 'ad
         * consilium' - illa classis parcis est) */
        CREDO_FALSUM (_inter(r, "AD CONSILIUM", "AD CLAUSURAM",
            "Qc gamma?"));

        /* VISA NON FIXA: parca aperta sine vinculo numerantur, non
         * enumerantur (LXVIII in conditorio vivo) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":477,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"parcum\","
            "\"titulus\":\"Qc parcum solitarium\"}}}");
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":478,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "parca visa, non fixa") != NIHIL);
        CREDO_VERUM (strstr(r, "Qc parcum solitarium") == NIHIL);

        /* colloquium CLAUSUM: quaestio clausa -> propositum AD
         * CONSILIUM, causa nominata */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":479,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Qc nomina stratorum?\",\"actus\":\"status\","
            "\"novus\":\"clausum\"}}}");
        CREDO_VERUM (strstr(r, "status clausum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":480,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (_inter(r, "AD CONSILIUM", "AD CLAUSURAM",
            "Qc propositum"));
        CREDO_FALSUM (_inter(r, "AD COLLOQUIUM", "IMPEDITA",
            "Qc nomina stratorum?"));

        /* census numerum fert; tabula.md sectionem */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":481,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"census\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "ad colloquium 1") != NIHIL);
        CREDO_VERUM (strstr(_plagula_litterae(piscina, VIA_TB),
            "### AD COLLOQUIUM") != NIHIL);

        /* natura per mutationem in quaestione exsistente */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":482,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Qc vitium ordinarium\",\"actus\":\"mutatio\","
            "\"clavis\":\"natura\",\"valor\":\"consilium\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":483,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (_inter(r, "AD COLLOQUIUM", "IMPEDITA",
            "Qc vitium ordinarium"));

        /* schema natura nominat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":484,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "\"natura\"") != NIHIL);
    }


    /* ==================================================
     * XXXVII. VISUS PARATA UT RELATIO COTIDIANA (opus 01M335E2KY):
     * aetas per ordinem, dies clausurae impedientis, forma brevis,
     * ordo 'sequitur', orientatio in doctrina. Ex colloquio
     * 2026-09-22: relatio quam Claudius manu ex visu composuit uno
     * vocamine dari debet - iudicium lectori, evidentia (aetas) in
     * ordine.
     * ================================================== */

    {
        constans character* locus;

        /* AETAS: quisque ordo 'natus YYYY-MM-DD, tactus YYYY-MM-DD'
         * fert (mundus probationis hodie natus - dies idem; forma
         * probatur, non valor) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":490,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        locus = _inter_locus(r, "AD COLLOQUIUM", "Qc gamma?");
        CREDO_VERUM (strstr(locus, "natus 20") != NIHIL);
        CREDO_VERUM (strstr(locus, ", tactus 20") != NIHIL);

        /* DIES CLAUSURAE impedientis in causa 'impedientia clausa':
         * 'Pa I.2 secundum' impedientem 'Pa I.1 primum' habet
         * (perfectum in XXXI) - causa diem fert */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":491,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Qc propositum\","
            "\"breviter\":\"verum\"}}}");
        /* Qc propositum: impediens 'Qc nomina stratorum?' clausum in
         * XXXVI -> in visu toto AD CONSILIUM cum 'clausum 20..' */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":492,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        locus = _inter_locus(r, "AD CONSILIUM", "Qc propositum");
        CREDO_VERUM (strstr(locus, "impedientia clausa") != NIHIL);
        CREDO_VERUM (strstr(locus, "clausum 20") != NIHIL);

        /* FORMA BREVIS: numeri sex in capite, summi TRES per
         * sectionem, residuum numeratum, parca visa non fixa */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":493,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "ad laborem ") != NIHIL);
        CREDO_VERUM (strstr(r, "ad colloquium ") != NIHIL);
        CREDO_VERUM (strstr(r, "parca visa, non fixa") != NIHIL);
        /* AD CONSILIUM in mundo probationis > III res: tres
         * ostensae, reliquae numeratae */
        locus = _inter_locus(r, "AD CONSILIUM (", "(et alia ");
        CREDO_VERUM (locus[0] != '\0');
        /* aetas in forma brevi ABEST (brevis = brevis) */
        CREDO_VERUM (strstr(r, "natus 20") == NIHIL);

        /* ORDO 'sequitur': in AD LABOREM opus quod alterum
         * SEQUITUR post illud stat, quidquid res_id dicit. Opera
         * duo nova sine impedientibus; 'Sq alpha' (res_id minor)
         * sequitur 'Sq beta' -> beta ANTE alpha */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":494,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"opus\","
            "\"titulus\":\"Sq alpha\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":495,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"opus\","
            "\"titulus\":\"Sq beta\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":496,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "Sq alpha") < strstr(r, "Sq beta"));
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":497,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Sq alpha\","
            "\"actus\":\"nexus\",\"verbum\":\"sequitur\","
            "\"alterum\":\"Sq beta\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":498,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "Sq beta") < strstr(r, "Sq alpha"));
        /* MOLLE: alpha adhuc AD LABOREM (sequitur numquam obstat),
         * et causa ordinem nominat */
        CREDO_VERUM (_inter(r, "AD LABOREM", "AD CONSILIUM",
            "Sq alpha"));
        locus = _inter_locus(r, "AD LABOREM", "Sq alpha");
        CREDO_VERUM (strstr(locus, "sequitur 'Sq beta'") != NIHIL);

        /* DOCTRINA: orientatio in parte NON truncata (ante
         * MMXLVIII characteres) - hospes id abscindit */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":499,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Sq alpha\","
            "\"breviter\":\"verum\"}}}");
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":500,"
            "\"method\":\"initialize\",\"params\":{}}");
        locus = strstr(r, "ORIENTATIO: parata");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && (locus - strstr(r, "TABULARIUM:")) < 2048);
        /* stratum mappae quoque intra caput (mensuratum 2026-09-22:
         * REGIO 910, mappa 1022, PRINCIPIUM 1079 ex 4790) - sessio
         * recens regiones ex doctrina sola discat */
        locus = strstr(r, "REGIO = locus");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && (locus - strstr(r, "TABULARIUM:")) < 2048);
        locus = strstr(r, "mappa {} = arbor");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && (locus - strstr(r, "TABULARIUM:")) < 2048);
        locus = strstr(r, "PRINCIPIUM = decretum");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && (locus - strstr(r, "TABULARIUM:")) < 2048);
    }


    /* ==================================================
     * XXXVIII. QUAESTIO IMPEDIMENTUM PARENTIS NON HEREDITAT (vitium
     * 01M350V79N, usu primo inventum): parcum mappae cum quaestionibus
     * consilii INTRA id ET id impedientibus - visus omnes ut IMPEDITA
     * dabat ('parens impeditur a: <quaestio ipsa>'), AD COLLOQUIUM
     * vacuum. Regula: quaestio semper colloqui potest; impeditur-a
     * EXPRESSUM inter quaestiones manet; opus/parcum hereditant.
     * ================================================== */

    {
        constans character* locus;

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":510,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"parcum\","
            "\"titulus\":\"Hq propositum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":511,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Hq impediens?\",\"natura\":"
            "\"consilium\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":512,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Hq soror?\",\"natura\":\"consilium\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":513,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"opus\","
            "\"titulus\":\"Hq opus sub proposito\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        /* ambae quaestiones et opus INTRA propositum; propositum
         * impeditur-a 'Hq impediens?' */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":514,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Hq impediens?\","
            "\"actus\":\"nexus\",\"verbum\":\"intra\",\"alterum\":"
            "\"Hq propositum\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":515,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Hq soror?\","
            "\"actus\":\"nexus\",\"verbum\":\"intra\",\"alterum\":"
            "\"Hq propositum\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":516,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Hq opus sub proposito\",\"actus\":\"nexus\","
            "\"verbum\":\"intra\",\"alterum\":\"Hq propositum\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":517,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Hq propositum\","
            "\"actus\":\"nexus\",\"verbum\":\"impeditur-a\","
            "\"alterum\":\"Hq impediens?\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":518,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{\"intra\":"
            "\"Hq propositum\"}}}");
        /* impediens ipsa: AD COLLOQUIUM, numquam IMPEDITA (ORDO
         * eius quaeritur - titulus nudus in causa operis legitime
         * stat: 'parens ... impeditur a: Hq impediens?') */
        CREDO_VERUM (_inter(r, "AD COLLOQUIUM", "IMPEDITA",
            "quaestio/apertum  Hq impediens?"));
        CREDO_FALSUM (_inter(r, "IMPEDITA", NIHIL,
            "quaestio/apertum  Hq impediens?"));
        /* soror sub eodem parente impedito: colloqui potest quoque
         * (impeditur-a inter quaestiones expressum esset) */
        CREDO_VERUM (_inter(r, "AD COLLOQUIUM", "IMPEDITA",
            "Hq soror?"));
        /* OPUS sub proposito impedito hereditat ut antea */
        CREDO_VERUM (_inter(r, "IMPEDITA", NIHIL,
            "Hq opus sub proposito"));
        locus = _inter_locus(r, "IMPEDITA", "Hq opus sub proposito");
        CREDO_VERUM (strstr(locus, "parens") != NIHIL);
        /* impeditur-a EXPRESSUM inter quaestiones adhuc impedit */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":519,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Hq soror?\","
            "\"actus\":\"nexus\",\"verbum\":\"impeditur-a\","
            "\"alterum\":\"Hq impediens?\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":520,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{\"intra\":"
            "\"Hq propositum\"}}}");
        CREDO_VERUM (_inter(r, "IMPEDITA", NIHIL,
            "quaestio/apertum  Hq soror?"));
        CREDO_FALSUM (_inter(r, "AD COLLOQUIUM", "IMPEDITA",
            "quaestio/apertum  Hq soror?"));
    }


    /* ==================================================
     * XXXIX. GENUS 'REGIO' (opus 01M3567242, mappa I; decreta
     * 01M35650Z4 collocatio per intra, 01M35656PV regio sine statu):
     * nodus mappae - locus, non res vitae. Semen v9; parata et
     * quaerere {intra: regio} SINE codice novo; porta recusat
     * impeditur-a cum regione utrimque et status in regione;
     * collocatio in regione NON fixat ('parca visa, non fixa').
     * ================================================== */

    {
        constans character* locus;
                       s64  visa_ante;
                       s64  visa_post;

        /* SEMEN v9: genus DECLARATUM (lex progressiva rem generis
         * ignoti creare sinit - declaratio = honestas schematis:
         * corpus/tags nominata, machina NULLA) */
        {
            GestaMundus* sonda = gesta_aperire(piscina, VIA_DB,
                VIA_AN);

            CREDO_NON_NIHIL (sonda);
            si (sonda != NIHIL)
            {
                chorda c = gesta_genus_datum(sonda, "regio", piscina);
                character* buf = (character*)piscina_allocare(
                    piscina, (memoriae_index)c.mensura + I);

                CREDO_VERUM (c.mensura > ZEPHYRUM);
                si (buf != NIHIL && c.mensura > ZEPHYRUM)
                {
                    memcpy(buf, c.datum, (memoriae_index)c.mensura);
                    buf[c.mensura] = '\0';
                    CREDO_VERUM (strstr(buf, "\"corpus\"") != NIHIL);
                    CREDO_VERUM (strstr(buf, "status_initialis")
                        == NIHIL);
                    CREDO_VERUM (strstr(buf, "\"machina\"") == NIHIL);
                }
                gesta_claudere(sonda);
            }
        }
        /* schema addere.genus regionem nominat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":529,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "|regio.") != NIHIL);

        /* regio creatur SINE statu (ut decretum); census eam numerat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":530,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"regio\","
            "\"titulus\":\"Rg textus\",\"corpus\":\"fontes, PDF,"
            " Unicode - quid haec area sit\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        CREDO_VERUM (strstr(r, "(regio") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":531,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"regio\","
            "\"titulus\":\"Rg fontes\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":532,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"census\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "regio/  2") != NIHIL);

        /* 'parca visa, non fixa' ANTE collocationem */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":533,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        locus = strstr(r, "parca visa, non fixa: ");
        CREDO_NON_NIHIL (locus);
        visa_ante = locus != NIHIL
            ? (s64)atol(locus + strlen("parca visa, non fixa: "))
            : (s64)-I;

        /* arbor UNA: regio intra regionem, parcum intra regionem,
         * opus intra parcum - omnia per 'intra' */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":534,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Rg fontes\","
            "\"actus\":\"nexus\",\"verbum\":\"intra\",\"alterum\":"
            "\"Rg textus\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":535,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"parcum\","
            "\"titulus\":\"Rg parsator sfnt\",\"corpus\":"
            "\"glyphae ex tabulis\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":536,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Rg parsator sfnt\","
            "\"actus\":\"nexus\",\"verbum\":\"intra\",\"alterum\":"
            "\"Rg fontes\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);

        /* COLLOCATIO NON FIXAT: parcum intra regionem, sine filio,
         * sine impedimento, adhuc 'visum, non fixum' (locus non est
         * planum) - numerus crescit uno */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5361,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        locus = strstr(r, "parca visa, non fixa: ");
        CREDO_NON_NIHIL (locus);
        visa_post = locus != NIHIL
            ? (s64)atol(locus + strlen("parca visa, non fixa: "))
            : (s64)-II;
        CREDO_AEQUALIS_S64 (visa_post, visa_ante + (s64)I);

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":537,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"opus\","
            "\"titulus\":\"Rg opus tabulae cmap\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":538,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Rg opus tabulae cmap\",\"actus\":\"nexus\","
            "\"verbum\":\"intra\",\"alterum\":"
            "\"Rg parsator sfnt\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);

        /* FILIUS FIXAT: opus intra parcum = ansa - parcum iam
         * fixum, numerus ad priorem redit */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":539,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        locus = strstr(r, "parca visa, non fixa: ");
        CREDO_NON_NIHIL (locus);
        visa_post = locus != NIHIL
            ? (s64)atol(locus + strlen("parca visa, non fixa: "))
            : (s64)-II;
        CREDO_AEQUALIS_S64 (visa_post, visa_ante);

        /* parata {intra: REGIO} sine codice novo: opus sub parco sub
         * regione AD LABOREM; regio ipsa in nulla classe */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":540,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{\"intra\":\"Rg textus\"}}}");
        CREDO_VERUM (_inter(r, "AD LABOREM", "AD CONSILIUM",
            "Rg opus tabulae cmap"));
        CREDO_VERUM (strstr(r, "regio/") == NIHIL);
        CREDO_VERUM (strstr(r, "Hq opus sub proposito") == NIHIL);

        /* quaerere {intra: REGIO} sine codice novo */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":541,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":\"glyph*\","
            "\"intra\":\"Rg textus\"}}}");
        CREDO_VERUM (strstr(r, "Rg parsator sfnt") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":542,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"quaerere\",\"arguments\":{\"textus\":\"glyph*\","
            "\"intra\":\"Hq propositum\"}}}");
        CREDO_VERUM (strstr(r, "Rg parsator sfnt") == NIHIL);

        /* PORTA: impeditur-a cum regione UTRIMQUE recusatur, causis
         * omnibus + scriptura valida (lex recusationis) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":543,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Rg parsator sfnt\","
            "\"actus\":\"nexus\",\"verbum\":\"impeditur-a\","
            "\"alterum\":\"Rg textus\"}}}");
        CREDO_VERUM (strstr(r, "RECUSATUS") != NIHIL);
        CREDO_VERUM (strstr(r, "regio") != NIHIL);
        CREDO_VERUM (strstr(r, "locus") != NIHIL);
        CREDO_VERUM (strstr(r, "creatum") == NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":544,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Rg textus\","
            "\"actus\":\"nexus\",\"verbum\":\"impeditur-a\","
            "\"alterum\":\"Rg parsator sfnt\"}}}");
        CREDO_VERUM (strstr(r, "RECUSATUS") != NIHIL);
        CREDO_VERUM (strstr(r, "creatum") == NIHIL);
        /* verbum inversum + regio: AMBAE causae simul (numquam
         * guttatim) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":545,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Rg textus\","
            "\"actus\":\"nexus\",\"verbum\":\"impedit\","
            "\"alterum\":\"Rg parsator sfnt\"}}}");
        CREDO_VERUM (strstr(r, "RECUSATUS (2 causae)") != NIHIL);
        CREDO_VERUM (strstr(r, "inversum") != NIHIL);
        CREDO_VERUM (strstr(r, "locus") != NIHIL);

        /* PORTA: status in regione recusatur (lex progressiva
         * generibus sine machina statum sinit - regio EXCIPITUR:
         * numquam clauditur) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":546,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Rg textus\","
            "\"actus\":\"status\",\"novus\":\"clausum\"}}}");
        CREDO_VERUM (strstr(r, "RECUSATUS") != NIHIL);
        CREDO_VERUM (strstr(r, "regio statum non habet") != NIHIL);
        CREDO_VERUM (strstr(r, "eventum status scriptum") == NIHIL);
        /* mutatio corporis in regione LICET (synthesis viva) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":547,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Rg textus\","
            "\"actus\":\"mutatio\",\"clavis\":\"corpus\",\"valor\":"
            "\"fontes, PDF, Unicode, dispositio\"}}}");
        CREDO_VERUM (strstr(r, "scriptum") != NIHIL);
    }


    /* ==================================================
     * XL. STAGE II MAPPAE (opus 01M35A3SSP, decretum 01M3565C2A):
     * visio (desideratum natura:visio) intra regionem in NULLA
     * classe visus parata; pagina regionis = corpus + visiones +
     * numeri subarboris; breviarium parci 'quaestiones consilii:
     * decisae N, apertae M'. Fixtura: Rg textus > Rg fontes > Rg
     * parsator sfnt > Rg opus tabulae cmap (ex XXXIX).
     * ================================================== */

    {
        constans character* locus;

        /* visio Frani intra regionem */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":550,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"desideratum\","
            "\"titulus\":\"Rv volo librum Lapidis in PDF\","
            "\"natura\":\"visio\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":551,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Rv volo librum Lapidis in PDF\",\"actus\":\"nexus\","
            "\"verbum\":\"intra\",\"alterum\":\"Rg textus\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);

        /* VISIO NUMQUAM PARATA: nec AD CONSILIUM nec usquam in visu
         * (toto et intra regionem) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":552,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "Rv volo librum") == NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":553,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{\"intra\":\"Rg textus\"}}}");
        CREDO_VERUM (strstr(r, "Rv volo librum") == NIHIL);
        /* desideratum ORDINARIUM intra regionem VISIBILE manet -
         * signum visionis solum findit (visio nusquam, ordinarium in
         * scopo regionis). Sectio mutata 2026-09-23 (Fran, optio A):
         * olim AD CONSILIUM, nunc 'VISA, NON FIXA (in hac regione)' -
         * collocatio non fixat (decretum 01M35650Z4) etiam in
         * sectionibus; haec assertio lectionem veterem pinnabat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":554,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"desideratum\","
            "\"titulus\":\"Rv desideratum ordinarium\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":555,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Rv desideratum ordinarium\",\"actus\":\"nexus\","
            "\"verbum\":\"intra\",\"alterum\":\"Rg textus\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":556,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{\"intra\":\"Rg textus\"}}}");
        CREDO_VERUM (strstr(r, "VISA, NON FIXA (in hac regione)")
            != NIHIL
            && strstr(strstr(r, "VISA, NON FIXA (in hac regione)"),
                "Rv desideratum ordinarium") != NIHIL);
        CREDO_FALSUM (_inter(r, "AD CONSILIUM", "AD CLAUSURAM",
            "Rv desideratum ordinarium"));

        /* quaestio consilii intra parcum (pro numeris et breviario) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":557,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Rq forma tabulae glypharum?\","
            "\"natura\":\"consilium\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":558,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Rq forma tabulae glypharum?\",\"actus\":\"nexus\","
            "\"verbum\":\"intra\",\"alterum\":"
            "\"Rg parsator sfnt\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        /* vitium ordinarium intra parcum */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":559,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Rq vitium cmap ruit\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":560,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Rq vitium cmap ruit\",\"actus\":\"nexus\","
            "\"verbum\":\"intra\",\"alterum\":"
            "\"Rg parsator sfnt\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);

        /* PAGINA REGIONIS (breviter): visiones primae, deinde numeri
         * subarboris (forma UNA cum mappa) - parca I (dormientia 0:
         * parcum filios habet), quaestiones consilii I, vitia I,
         * opera I, tactus hodie */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":561,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Rg textus\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "visiones (1)") != NIHIL);
        CREDO_VERUM (strstr(r, "Rv volo librum Lapidis in PDF")
            != NIHIL);
        /* visio ANTE numeros et ante nexus */
        CREDO_VERUM (strstr(r, "numeri intra regionem") != NIHIL
            && strstr(r, "visiones (1)")
                < strstr(r, "numeri intra regionem"));
        locus = strstr(r, "numeri intra regionem");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "parca 1 (dormientia 0)") != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "quaestiones consilii 1")
                != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "vitia 1") != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "opera 1") != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "tactus 20") != NIHIL);
        /* forma plena eadem fert */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":562,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Rg textus\"}}}");
        CREDO_VERUM (strstr(r, "visiones (1)") != NIHIL);
        CREDO_VERUM (strstr(r, "numeri intra regionem") != NIHIL);
        /* sub-regio vacua: numeri cum nihilo (mappa 'nihil hic
         * adhuc' dicit), visiones absunt */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":563,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"regio\","
            "\"titulus\":\"Rg horizon vacuus\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":564,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Rg horizon vacuus\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "visiones (") == NIHIL);
        CREDO_VERUM (strstr(r, "parca 0 (dormientia 0)")
            != NIHIL);
        /* parcum non-regio: numeri regionis ABSUNT */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":565,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Rg parsator sfnt\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "numeri intra regionem") == NIHIL);

        /* BREVIARIUM PARCI: quaestiones consilii decisae/apertae
         * (vitium ordinarium NON numeratur) */
        CREDO_VERUM (strstr(r, "quaestiones consilii: decisae 0,"
            " apertae 1") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":566,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Rq forma tabulae glypharum?\",\"actus\":\"status\","
            "\"novus\":\"clausum\"}}}");
        CREDO_VERUM (strstr(r, "status clausum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":567,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Rg parsator sfnt\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "quaestiones consilii: decisae 1,"
            " apertae 0") != NIHIL);
        /* parcum sine quaestionibus consilii: linea TACET */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":568,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Qc parcum solitarium\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "quaestiones consilii:") == NIHIL);
        /* regio post clausuram: quaestiones consilii apertae 0 */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":569,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Rg textus\","
            "\"breviter\":\"verum\"}}}");
        locus = strstr(r, "numeri intra regionem");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "quaestiones consilii 0")
                != NIHIL);
    }


    /* ==================================================
     * XLI. INSTRUMENTUM 'MAPPA' (opus 01M35APSXM, mappa III): arbor
     * regionum cum numeris subarboris per nodum + SALUS mappae (menu
     * activitatis ordinandi) + sectio '## MAPPA' in tabula.md.
     * Fixtura ex XXXIX/XL: Rg textus > Rg fontes > Rg parsator sfnt;
     * Rg horizon vacuus (sine corpore, sine re); visio Rv intra Rg
     * textus.
     * ================================================== */

    {
        constans character* locus;
                       i32  k;

        /* X notae cum tags 'typographia' et 'ligatura' - tags
         * recurrentes sine regione (ligatura: regio eam TAGO solo
         * nominabit, infra) */
        per (k = ZEPHYRUM; k < X; k++)
        {
            character petitio[CCLVI];

            sprintf(petitio, "{\"jsonrpc\":\"2.0\",\"id\":%d,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"addere\",\"arguments\":{\"genus\":\"nota\","
                "\"titulus\":\"Mt nota typographica %d\",\"tags\":"
                "\"typographia,ligatura\"}}}", (int)(570 + k), (int)k);
            r = _mitte(t, piscina, petitio);
            CREDO_VERUM (strstr(r, "creata") != NIHIL);
        }

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":580,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "MAPPA") != NIHIL);
        /* ARBOR: radix ante filiam; numeri subarboris in linea */
        CREDO_VERUM (strstr(r, "Rg textus") != NIHIL);
        CREDO_VERUM (strstr(r, "Rg fontes") != NIHIL);
        CREDO_VERUM (strstr(r, "Rg textus") < strstr(r, "Rg fontes"));
        locus = strstr(r, "Rg textus");
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "parca 1 (dormientia 0)") != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "visiones 1") != NIHIL);
        locus = strstr(r, "Rg horizon vacuus");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "parca 0 (dormientia 0)") != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "visiones 0") != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "tactus 20") != NIHIL);
        /* parca ipsa in arbore NON enumerantur (regiones solae) */
        CREDO_FALSUM (_inter(r, "MAPPA", "SALUS", "Rg parsator sfnt"));

        /* SALUS: sine regione numerata per genus, prima nominata;
         * parcum in regione ABSENS */
        CREDO_VERUM (strstr(r, "SALUS") != NIHIL);
        CREDO_VERUM (strstr(r, "sine regione: parca ") != NIHIL);
        CREDO_FALSUM (_inter(r, "sine regione", "regiones sine corpore",
            "Rg parsator sfnt"));
        /* regiones sine corpore: horizon et fontes, non textus */
        CREDO_VERUM (_inter(r, "regiones sine corpore",
            "regiones supra limen", "Rg horizon vacuus"));
        CREDO_VERUM (_inter(r, "regiones sine corpore",
            "regiones supra limen", "Rg fontes"));
        CREDO_FALSUM (_inter(r, "regiones sine corpore",
            "regiones supra limen", "Rg textus"));
        /* regiones vacuae: horizon sola */
        CREDO_VERUM (_inter(r, "regiones vacuae",
            "regiones sine visione", "Rg horizon vacuus"));
        CREDO_FALSUM (_inter(r, "regiones vacuae",
            "regiones sine visione", "Rg textus"));
        /* tag recurrens nullam regionem nominans */
        CREDO_VERUM (strstr(r, "typographia (10)") != NIHIL);
        CREDO_VERUM (strstr(r, "ligatura (10)") != NIHIL);
        /* parca quietissima: caput et tactus */
        locus = strstr(r, "parca quietissima");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "(tactus 20") != NIHIL);

        /* regio cum titulo tagi: tag non amplius candidata */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":581,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"regio\","
            "\"titulus\":\"Typographia\",\"corpus\":\"ars"
            " litterarum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":582,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "typographia (10)") == NIHIL);

        /* regio cum TAGO (non titulo) tagi: tag quoque non candidata
         * (01M35C05AD) - olim tituli soli conferebantur, ergo 'silva'
         * sub 'C89 development stack' tag gerente et 'regio' ipsum
         * (tag quem regiones solae ferunt) candidatae manebant */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5823,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"regio\","
            "\"titulus\":\"Rg glyphae\",\"corpus\":\"formae"
            " litterarum\",\"tags\":\"ligatura\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5824,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "ligatura (") == NIHIL);

        /* ADDERE {intra} (01M37AY25M): res ad ORTUM collocata -
         * creatio et vinculum 'intra' uno fasce; parens ignotus
         * totum recusat, nihil scriptum (olim collocatio vocatio
         * altera erat, saepe omissa: LXX parca sine regione) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5825,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Mt nota collocata\",\"intra\":"
            "\"Rg glyphae\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        CREDO_VERUM (strstr(r, "--intra--> Rg glyphae") != NIHIL);
        /* res nata cum vinculo per FINEM resolvitur (01M37KBAWV).
         * Ambiguitas initii X characterum inter rem et vinculum eius
         * a TEMPORE pendet (vinculum ad solutionem cuditur, res ad
         * scripturam - millisecundum idem saepe, non semper), ergo hic
         * non asseritur; ambiguitas initii certa in sectione
         * 'praefixum ULID' probatur */
        {
            character nata[GESTA_RES_ID_MENSURA];
            character vocatio_finis[CCLVI];

            _res_id_ex_responso(r, nata);
            CREDO_VERUM (strlen(nata) == XXVI);
            sprintf(vocatio_finis, "{\"jsonrpc\":\"2.0\","
                "\"id\":58252,\"method\":\"tools/call\",\"params\":"
                "{\"name\":\"res\",\"arguments\":{\"res\":\"%s\"}}}",
                nata + XX);
            r = _mitte(t, piscina, vocatio_finis);
            CREDO_VERUM (strstr(r, "Mt nota collocata") != NIHIL);
        }
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5826,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Mt nota collocata\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "--intra--> Rg glyphae (regio)")
            != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5827,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Mt nota orba\",\"intra\":"
            "\"Rg regio nulla\"}}}");
        CREDO_VERUM (strstr(r, "creata") == NIHIL);
        CREDO_VERUM (strstr(r, "nihil scriptum") != NIHIL);
        CREDO_VERUM (strstr(r, "Rg regio nulla") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5828,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Mt nota orba\"}}}");
        CREDO_VERUM (strstr(r, "res ignota") != NIHIL);
        /* intra ET ad ignota: causae AMBAE simul (numquam guttatim) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5829,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Mt nota bis orba\",\"intra\":"
            "\"Rg regio nulla\",\"ad\":\"Res nulla\"}}}");
        CREDO_VERUM (strstr(r, "(2 causae)") != NIHIL);
        CREDO_VERUM (strstr(r, "intra: 'Rg regio nulla'") != NIHIL);
        CREDO_VERUM (strstr(r, "ad: 'Res nulla'") != NIHIL);

        /* ARGUMENTA IGNOTA (01M37JYP2W): clavis non publicata
         * recusatur, nihil actum, proxima suggeritur - olim tacite
         * neglegebatur ('intr' -> res sine parente nascebatur) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5830,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"nota\","
            "\"titulus\":\"Mt nota typo\",\"intr\":"
            "\"Rg glyphae\"}}}");
        CREDO_VERUM (strstr(r, "creata") == NIHIL);
        CREDO_VERUM (strstr(r, "argumenta RECUSATA (1 causa)")
            != NIHIL);
        CREDO_VERUM (strstr(r, "'intr' ignotum - fortasse 'intra'?")
            != NIHIL);
        CREDO_VERUM (strstr(r, "ARGUMENTA 'addere':") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5831,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Mt nota typo\"}}}");
        CREDO_VERUM (strstr(r, "res ignota") != NIHIL);
        /* causae omnes simul; clavis longinqua sine suggestione */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5832,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Rg glyphae\","
            "\"actus\":\"nota\",\"textum\":\"x\","
            "\"zzzzzzzz\":\"y\"}}}");
        CREDO_VERUM (strstr(r, "(2 causae)") != NIHIL);
        CREDO_VERUM (strstr(r, "'textum' ignotum - fortasse 'textus'?")
            != NIHIL);
        CREDO_VERUM (strstr(r, "'zzzzzzzz' ignotum\\n") != NIHIL
            || strstr(r, "'zzzzzzzz' ignotum\\nARGUMENTA") != NIHIL);

        /* FORMA NOMINA: arbor nominum sola, ut 'tree' - sine id, sine
         * numeris, sine salute; filia indentata */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5821,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{\"forma\":\"nomina\"}}}");
        CREDO_VERUM (strstr(r, "\\nRg textus") != NIHIL);
        CREDO_VERUM (strstr(r, "\\n  Rg fontes") != NIHIL);
        CREDO_VERUM (strstr(r, "parca ") == NIHIL);
        CREDO_VERUM (strstr(r, "SALUS") == NIHIL);
        CREDO_VERUM (strstr(r, "01M") == NIHIL);

        /* BREVITER: arbor sola */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":583,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "Rg textus") != NIHIL);
        CREDO_VERUM (strstr(r, "SALUS") == NIHIL);
        /* REGIO: subarbor sola */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":584,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{\"regio\":\"Rg fontes\"}}}");
        CREDO_VERUM (strstr(r, "Rg fontes") != NIHIL);
        CREDO_VERUM (strstr(r, "Rg textus") == NIHIL);
        CREDO_VERUM (strstr(r, "Rg horizon vacuus") == NIHIL);
        /* res quae regio non est: recusatio clara */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":585,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{\"regio\":"
            "\"Rg parsator sfnt\"}}}");
        CREDO_VERUM (strstr(r, "regio non est") != NIHIL);

        /* tabula.md: sectio MAPPA post PARATA cum arbore et sine
         * regione */
        {
            constans character* tabula = _plagula_litterae(piscina,
                VIA_TB);

            CREDO_VERUM (strstr(tabula, "## MAPPA") != NIHIL);
            CREDO_VERUM (_inter(tabula, "## MAPPA", "## QUAESTIONES",
                "Rg textus"));
            CREDO_VERUM (_inter(tabula, "## MAPPA", "## QUAESTIONES",
                "sine regione"));
            CREDO_VERUM (strstr(tabula, "## MAPPA") != NIHIL
                && strstr(tabula, "## PARATA")
                    < strstr(tabula, "## MAPPA"));
        }

        /* schema instrumentum nominat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":586,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "\"mappa\"") != NIHIL);
    }


    /* ==================================================
     * XLII. FRONS REGIONIS (opus 01M35P4EFV, mappa IV): salus 'sine
     * visione / sine consilio / sine opere'; linea arboris cum
     * classibus paratis et numeris cumulativis (decisae, perfecta);
     * pagina regionis ORDINANDA + impedita; AD CLAUSURAM 'aut gradum
     * proximum file'. Fixtura ex XXXIX-XLI.
     * ================================================== */

    {
        constans character* locus;
        constans character* finis_lineae;
        constans character* classis;

        /* parcum DORMIENS intra Rg fontes (sine filio, sine
         * impedimento, sine quaestione); quaestio consilii aperta
         * intra Typographia */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":590,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"parcum\","
            "\"titulus\":\"Fr parcum dormiens\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":591,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Fr parcum dormiens\","
            "\"actus\":\"nexus\",\"verbum\":\"intra\",\"alterum\":"
            "\"Rg fontes\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":592,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Fr quaestio typographiae?\",\"natura\":"
            "\"consilium\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":593,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Fr quaestio typographiae?\",\"actus\":\"nexus\","
            "\"verbum\":\"intra\",\"alterum\":\"Typographia\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);

        /* COLLOCATIO NON FIXAT - etiam in SECTIONIBUS parata (decretum
         * 01M35650Z4; 2026-09-23): 'intra' ad regionem locus est, non
         * planum. Olim sectiones omne 'intra' ut fixationem legebant
         * (pes solus eum excludebat) - post sessionem ordinandi LXII
         * parca dormientia AD CONSILIUM inundabant (XIII -> LXXVII) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":59350,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_FALSUM (_inter(r, "AD CONSILIUM", "AD CLAUSURAM",
            "Fr parcum dormiens"));
        CREDO_VERUM (strstr(r, "Fr parcum dormiens") == NIHIL);
        /* scopo regionis: sectio propria, non AD CONSILIUM */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":59351,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{\"intra\":\"Rg fontes\"}}}");
        CREDO_VERUM (strstr(r, "VISA, NON FIXA (in hac regione)")
            != NIHIL);
        CREDO_VERUM (_inter(r, "VISA, NON FIXA (in hac regione)",
            "\\n\\n", "Fr parcum dormiens")
            || (strstr(r, "VISA, NON FIXA (in hac regione)") != NIHIL
                && strstr(strstr(r, "VISA, NON FIXA (in hac regione)"),
                    "Fr parcum dormiens") != NIHIL));
        CREDO_FALSUM (_inter(r, "AD CONSILIUM", "AD CLAUSURAM",
            "Fr parcum dormiens"));
        /* pes: alia genera in regionibus collocata numerantur */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":59352,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"desideratum\","
            "\"titulus\":\"Fr desideratum collocatum\",\"intra\":"
            "\"Rg fontes\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":59353,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "Fr desideratum collocatum") == NIHIL);
        CREDO_VERUM (strstr(r,
            "alia in regionibus collocata, non fixa:")
            != NIHIL);
        CREDO_VERUM (strstr(r, "desiderata ") != NIHIL);

        /* MAPPA: linea Rg textus - cumulativa et classes non-nullae.
         * Finis lineae = sequentia '\\n' DUORUM characterum (responsum
         * JSON lineas novas ita fert - laqueus notus) */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":594,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{}}}");
        locus = strstr(r, "Rg textus");
        CREDO_NON_NIHIL (locus);
        finis_lineae = locus != NIHIL ? strstr(locus, "\\n") : NIHIL;
        CREDO_NON_NIHIL (finis_lineae);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "quaestiones consilii 0 (decisae 1)")
                != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "opera 1 (perfecta 0)") != NIHIL);
        classis = locus != NIHIL ? strstr(locus,
            "ad laborem 1") : NIHIL;
        CREDO_VERUM (classis != NIHIL && classis < finis_lineae);
        /* regio vacua: nulla classis in linea */
        locus = strstr(r, "Rg horizon vacuus");
        CREDO_NON_NIHIL (locus);
        finis_lineae  = locus != NIHIL ? strstr(locus, "\\n") : NIHIL;
        classis       = locus != NIHIL ? strstr(locus, "ad ") : NIHIL;
        CREDO_VERUM (classis == NIHIL || finis_lineae == NIHIL
            || classis > finis_lineae);

        /* SALUS: tres lineae novae. (Olim 'Rg horizon vacuus' hic
         * NOMINATA asserebatur - ordine creationis inter tres primas
         * cadebat. Ex 2026-09-23 linea per ACTIVITATEM ordinatur:
         * regio vacua infra activas descendit et numerata manet.) */
        CREDO_VERUM (strstr(r, "regiones sine visione (quid area velit"
            " nondum dictum): ") != NIHIL);
        CREDO_FALSUM (_inter(r, "regiones sine visione",
            "regiones sine consilio", "Rg textus"));
        CREDO_VERUM (_inter(r, "regiones sine consilio",
            "regiones sine opere", "Rg textus"));
        CREDO_FALSUM (_inter(r, "regiones sine consilio",
            "regiones sine opere", "Typographia"));
        CREDO_VERUM (_inter(r, "regiones sine opere",
            "tags recurrentes", "Typographia"));
        CREDO_FALSUM (_inter(r, "regiones sine opere",
            "tags recurrentes", "Rg textus"));

        /* FRONS ORDINANDI ORDINATA (2026-09-23): 'sine visione' per
         * activitatem subarboris (parca + quaestiones consilii + vitia
         * + opera aperta), non per ordinem creationis. Regio NOVISSIMA
         * cum VI parcis: ordine creationis ultima (non nominata),
         * activitate prima - pondere monstrato */
        {
            constans character* linea_visionis;
            constans character* finis_visionis;
                           i32  k;

            r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":59360,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"addere\",\"arguments\":{\"genus\":\"regio\","
                "\"titulus\":\"Rg activa sine visione\",\"corpus\":"
                "\"multa sed nulla visio\"}}}");
            CREDO_VERUM (strstr(r, "creata") != NIHIL);
            per (k = ZEPHYRUM; k < VI; k++)
            {
                character petitio[CCLVI];

                sprintf(petitio, "{\"jsonrpc\":\"2.0\",\"id\":%d,"
                    "\"method\":\"tools/call\",\"params\":{\"name\":"
                    "\"addere\",\"arguments\":{\"genus\":\"parcum\","
                    "\"titulus\":\"Ra parcum %d\",\"intra\":"
                    "\"Rg activa sine visione\"}}}", (int)(59361 + k),
                    (int)k);
                r = _mitte(t, piscina, petitio);
                CREDO_VERUM (strstr(r, "creata") != NIHIL);
            }
            r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":59369,"
                "\"method\":\"tools/call\",\"params\":{\"name\":"
                "\"mappa\",\"arguments\":{}}}");
            linea_visionis = strstr(r, "regiones sine visione");
            finis_visionis = linea_visionis != NIHIL
                ? strstr(linea_visionis, "\\n") : NIHIL;
            CREDO_NON_NIHIL (finis_visionis);
            locus = linea_visionis != NIHIL
                ? strstr(linea_visionis, "Rg activa sine visione")
                : NIHIL;
            CREDO_VERUM (locus != NIHIL && finis_visionis != NIHIL
                && locus < finis_visionis);
            locus = linea_visionis != NIHIL
                ? strstr(linea_visionis, "(aperta 6)") : NIHIL;
            CREDO_VERUM (locus != NIHIL && finis_visionis != NIHIL
                && locus < finis_visionis);
        }

        /* PAGINA REGIONIS: ORDINANDA post 'parata sub hac re' */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":595,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Rg textus\","
            "\"breviter\":\"verum\"}}}");
        locus = strstr(r, "ordinanda");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "parca dormientia (1)") != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "Fr parcum dormiens") != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "quaestio consilii nulla") != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "visio nulla") == NIHIL);
        CREDO_VERUM (strstr(r, "parata sub hac re") != NIHIL
            && strstr(r, "parata sub hac re") < locus);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":596,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Rg horizon vacuus\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "visio nulla") != NIHIL);
        CREDO_VERUM (strstr(r, "quaestio consilii nulla") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":597,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Typographia\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "quaestio consilii nulla") == NIHIL);

        /* IMPEDITA in pagina et in linea: opus impeditur a vitio */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":598,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Rg opus tabulae cmap\",\"actus\":\"nexus\",\"verbum\":"
            "\"impeditur-a\",\"alterum\":\"Rq vitium cmap ruit\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":599,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Rg textus\","
            "\"breviter\":\"verum\"}}}");
        CREDO_VERUM (strstr(r, "impedita 1 (parata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":600,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{\"breviter\":\"verum\"}}}");
        locus = strstr(r, "Rg textus");
        CREDO_NON_NIHIL (locus);
        finis_lineae = locus != NIHIL ? strstr(locus, "\\n") : NIHIL;
        classis = locus != NIHIL ? strstr(locus, "impedita 1") : NIHIL;
        CREDO_VERUM (classis != NIHIL && finis_lineae != NIHIL
            && classis < finis_lineae);
        classis = locus != NIHIL ? strstr(locus, "ad laborem") : NIHIL;
        CREDO_VERUM (classis == NIHIL || finis_lineae == NIHIL
            || classis > finis_lineae);

        /* AD CLAUSURAM: causa gradum proximum quoque nominat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":601,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"parcum\","
            "\"titulus\":\"Fr parcum decisum\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":602,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
            "\"titulus\":\"Fr decisa?\",\"natura\":\"consilium\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":603,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Fr decisa?\","
            "\"actus\":\"nexus\",\"verbum\":\"intra\",\"alterum\":"
            "\"Fr parcum decisum\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":604,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":\"Fr decisa?\","
            "\"actus\":\"status\",\"novus\":\"clausum\"}}}");
        CREDO_VERUM (strstr(r, "status clausum") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":605,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        locus = _inter_locus(r, "AD CLAUSURAM", "Fr parcum decisum");
        CREDO_VERUM (locus[0] != '\0');
        CREDO_VERUM (strstr(locus, "gradum proximum") != NIHIL);
    }


    /* ==================================================
     * XLIII. PRINCIPIA (opus 01M35QWCWQ, mappa V): decretum natura:
     * principium = prior cui visio nititur, cum campo 'refutatio'
     * (quid id everteret). Pagina regionis: 'principia (N)' post
     * visiones, ante numeros; refutatio in linea. Decretum sine
     * regione = domus totius (non 'sine regione' flagatum).
     * ================================================== */

    {
        constans character* locus;

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":610,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"decretum\","
            "\"titulus\":\"Pr fontes variabiles rari sunt\","
            "\"natura\":\"principium\",\"refutatio\":\"si corpus"
            " Lapidis fontem variabilem poscit\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":611,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"gerere\",\"arguments\":{\"res\":"
            "\"Pr fontes variabiles rari sunt\",\"actus\":\"nexus\","
            "\"verbum\":\"intra\",\"alterum\":\"Rg fontes\"}}}");
        CREDO_VERUM (strstr(r, "creatum") != NIHIL);
        /* principium DOMUS: sine regione */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":612,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"addere\",\"arguments\":{\"genus\":\"decretum\","
            "\"titulus\":\"Pr programmata non senescunt\","
            "\"natura\":\"principium\"}}}");
        CREDO_VERUM (strstr(r, "creata") != NIHIL);

        /* pagina regionis (subarbor: principium in Rg fontes sub Rg
         * textus): principia post visiones, ante numeros; refutatio
         * in linea; principium domus ABEST */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":613,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Rg textus\","
            "\"breviter\":\"verum\"}}}");
        locus = strstr(r, "principia (1)");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "Pr fontes variabiles rari sunt")
                != NIHIL);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus, "refutatio: si corpus Lapidis") != NIHIL);
        CREDO_VERUM (strstr(r, "visiones (1)") != NIHIL
            && locus != NIHIL && strstr(r, "visiones (1)") < locus);
        CREDO_VERUM (locus != NIHIL
            && locus < strstr(r, "numeri intra regionem"));
        CREDO_VERUM (strstr(r, "Pr programmata non senescunt")
            == NIHIL);
        /* forma plena quoque */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":614,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":\"Rg fontes\"}}}");
        CREDO_VERUM (strstr(r, "principia (1)") != NIHIL);

        /* res decreti: natura et refutatio lineis suis */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":615,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"res\",\"arguments\":{\"res\":"
            "\"Pr fontes variabiles rari sunt\",\"breviter\":"
            "\"verum\"}}}");
        CREDO_VERUM (strstr(r, "natura principium") != NIHIL);
        CREDO_VERUM (strstr(r, "\\nrefutatio si corpus Lapidis")
            != NIHIL);

        /* visus parata principia numquam videt; salus mappae
         * decretum sine regione non flagat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":616,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"parata\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "Pr fontes variabiles") == NIHIL);
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":617,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{}}}");
        CREDO_VERUM (strstr(r, "Pr programmata") == NIHIL);

        /* schema refutationem nominat */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":618,"
            "\"method\":\"tools/list\"}");
        CREDO_VERUM (strstr(r, "\"refutatio\"") != NIHIL);
    }


    /* ==================================================
     * XLIV. FORMA PLENA (opus mappa V.b): arbor nominum cum
     * visionibus et principiis sub regione sua ut lineae captae STML
     * ('<principium (> t', '<visio (> t'), filiae directae solae,
     * gradu uno altius, ante sub-regiones; domus totius ante radices.
     * Fixtura: Rg textus ⊃ visio Rv; Rg fontes ⊃ principium Pr fontes;
     * Pr programmata sine regione.
     * ================================================== */

    {
        constans character* locus;
        constans character* filia;

        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":620,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{\"forma\":\"plena\"}}}");
        CREDO_VERUM (strstr(r, "MAPPA (plena)") != NIHIL);
        /* domus totius ante radices, gradu 0 */
        locus = strstr(r,
            "\\n<principium (> Pr programmata non senescunt");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && locus < strstr(r, "\\nRg textus"));
        /* visio sub Rg textus, gradu uno altius, ANTE filiam Rg fontes */
        locus = strstr(r, "\\nRg textus");
        CREDO_NON_NIHIL (locus);
        CREDO_VERUM (locus != NIHIL
            && strstr(locus,
            "\\n  <visio (> Rv volo librum Lapidis in PDF")
                != NIHIL);
        filia = locus != NIHIL ? strstr(locus,
            "\\n  Rg fontes") : NIHIL;
        CREDO_NON_NIHIL (filia);
        CREDO_VERUM (locus != NIHIL && filia != NIHIL
            && strstr(locus, "\\n  <visio (> Rv volo") < filia);
        /* principium sub Rg fontes, gradu duobus, NON sub Rg textus */
        CREDO_VERUM (filia != NIHIL
            && strstr(filia,
            "\\n    <principium (> Pr fontes variabiles")
                != NIHIL);
        CREDO_VERUM (strstr(r, "\\n  <principium (> Pr fontes")
            == NIHIL);
        /* sine numeris, sine salute, sine id */
        CREDO_VERUM (strstr(r, "parca ") == NIHIL);
        CREDO_VERUM (strstr(r, "SALUS") == NIHIL);
        CREDO_VERUM (strstr(r, "01M") == NIHIL);
        /* forma nomina lineas captas NON fert */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":621,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{\"forma\":\"nomina\"}}}");
        CREDO_VERUM (strstr(r, "<visio (>") == NIHIL);
        CREDO_VERUM (strstr(r, "<principium (>") == NIHIL);
        /* forma plena intra regionem unam: domus totius ABEST */
        r = _mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":622,"
            "\"method\":\"tools/call\",\"params\":{\"name\":"
            "\"mappa\",\"arguments\":{\"forma\":\"plena\",\"regio\":"
            "\"Rg textus\"}}}");
        CREDO_VERUM (strstr(r, "<visio (> Rv volo") != NIHIL);
        CREDO_VERUM (strstr(r, "Pr programmata") == NIHIL);
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
