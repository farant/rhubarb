#include "postulata_posix.h"
#include "latina.h"
#include "imperium.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "piscina.h"
#include "xar.h"
#include "json.h"
#include "http.h"

#include <stdio.h>
#include <string.h>

/* ========================================================================
 * TYPI
 * ======================================================================== */

#define IMPERIUM_PENDENS    ZEPHYRUM
#define IMPERIUM_PERFECTUM  I
#define IMPERIUM_CULPA      II

nomen structura {
    i32    tessera;
    i32    status;
    chorda valor;    /* JSON serializatum (perfectum) aut nuntius (culpa) */
} Iussum;

structura Imperium {
    Piscina*           piscina;
    ImperiumAestimator aestimator;
    vacuum*            datum;
    Xar*               iussa;
    i32                tessera_proxima;
    i32                id_fenestrae;
    ImperiumFructus    fructus;
};

/* ========================================================================
 * INTERNA
 * ======================================================================== */

interior Iussum*
_iussum_invenire (
    Imperium* imperium,
    i32       tessera)
{
    i32 index;

    per (index = ZEPHYRUM; index < xar_numerus(imperium->iussa);
        index = index + I)
    {
        Iussum* i = (Iussum*)xar_obtinere(imperium->iussa, index);

        si (i != NIHIL && i->tessera == tessera)
        {
            redde i;
        }
    }
    redde NIHIL;
}

/* JS involucrum: valorem (aut promissum solutum) per pontem remittit.
 *
 * CUR PROMISSUM TRACTANDUM: operationes interfaciei fere semper
 * async sunt. Sine hac diramatione quaeque earum 'null' statim
 * redderet - et probatio quae 'null' accipit et transit MENDAX est,
 * non fracta. */
interior chorda
_involvere (
    Imperium*           imperium,
    i32                 tessera,
    chorda              js,
    Piscina*            piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, 1024);
    character          numerus[XXXII];

    sprintf(numerus, "%d", (integer)tessera);
    (vacuum)imperium;

    chorda_aedificator_appendere_literis(a, "(function(){var t=");
    chorda_aedificator_appendere_literis(a, numerus);
    chorda_aedificator_appendere_literis(a,
        ";function n(o){o.tessera=t;"
        "internuntius.vocare('imperium.responsum',o);}"
        "function s(v){try{return JSON.stringify(v===undefined"
        "?null:v);}catch(e){return JSON.stringify(String(v));}}"
        "try{var r=(0,eval)(");
    /* JS utentis ut CHORDA effugiata, deinde eval INDIRECTUS.
     *
     * CUR EVAL ET NON CORPUS FUNCTIONIS: corpus functionis
     * '(function(){1+1})()' UNDEFINED reddit - sententia enim est,
     * non redditio. Prima forma huius bibliothecae id faciebat et
     * OMNIA 'null' reddebat: instrumentum quod semper 'perfectum'
     * dicit et nihil umquam narrat - genus pessimum defectus, quia
     * viride apparet. Eval valorem COMPLETIONIS reddit (semantica
     * consolae): '1+1' -> 2, 'var x=1; x+1' -> 2. Indirectus
     * '(0,eval)' scopum globalem tenet. */
    chorda_aedificator_appendere_character(a, '"');
    chorda_aedificator_appendere_evasus_json(a, js);
    chorda_aedificator_appendere_character(a, '"');
    chorda_aedificator_appendere_literis(a,
        ");"
        "if(r&&typeof r.then==='function'){"
        "r.then(function(v){n({valor:s(v)});},"
        "function(e){n({culpa:String(e&&e.message?e.message:e)});});"
        "}else{n({valor:s(r)});}"
        "}catch(e){n({culpa:String(e&&e.message?e.message:e)});}})();");

    redde chorda_aedificator_finire(a);
}

interior vacuum
_respondere_json (
    HospitiumColloquium* colloquium,
    i32                  status,
    chorda               corpus)
{
    colloquium_respondere(colloquium, status, "application/json",
        corpus);
}

/* ========================================================================
 * TRACTATORES HTTP
 * ======================================================================== */

/* POST /imperium - corpus = JS crudum */
interior vacuum
_iubere (
    HospitiumColloquium* colloquium)
{
    Imperium*                 imperium;
    constans HttpPetitioServeri* petitio;
    Piscina*                  p;
    Iussum                    novum;
    Iussum*                   sedes;
    chorda                    involutum;
    ChordaAedificator*        a;
    character                 numerus[XXXII];

    imperium = (Imperium*)colloquium_datum(colloquium);
    petitio  = colloquium_petitio(colloquium);
    p        = colloquium_piscina(colloquium);

    si (imperium == NIHIL || petitio == NIHIL)
    {
        _respondere_json(colloquium, D,
            chorda_ex_literis("{\"culpa\":\"imperium abest\"}", p));
        redde;
    }
    si (petitio->corpus.mensura == ZEPHYRUM)
    {
        _respondere_json(colloquium, CD,
            chorda_ex_literis("{\"culpa\":\"corpus vacuum\"}", p));
        redde;
    }

    novum.tessera = imperium->tessera_proxima;
    novum.status  = IMPERIUM_PENDENS;
    novum.valor.datum   = NIHIL;
    novum.valor.mensura = ZEPHYRUM;
    imperium->tessera_proxima = imperium->tessera_proxima + I;

    sedes = (Iussum*)xar_addere(imperium->iussa);
    si (sedes == NIHIL)
    {
        _respondere_json(colloquium, D,
            chorda_ex_literis("{\"culpa\":\"tabula plena\"}", p));
        redde;
    }
    *sedes = novum;

    /* involucrum in piscina IMPERII (non petitionis): aestimator
     * asynchronus est, et piscina petitionis post responsum moritur */
    involutum = _involvere(imperium, novum.tessera, petitio->corpus,
        imperium->piscina);
    imperium->aestimator(imperium->datum, involutum);
    imperium->fructus.iussa_missa = imperium->fructus.iussa_missa + I;

    a = chorda_aedificator_creare(p, LXIV);
    sprintf(numerus, "%d", (integer)novum.tessera);
    chorda_aedificator_appendere_literis(a, "{\"tessera\":");
    chorda_aedificator_appendere_literis(a, numerus);
    chorda_aedificator_appendere_literis(a, "}");
    _respondere_json(colloquium, CCII, chorda_aedificator_finire(a));
}

/* GET /imperium/:tessera  (et /imperium/species) */
interior vacuum
_quaerere (
    HospitiumColloquium* colloquium)
{
    Imperium*          imperium;
    Piscina*           p;
    chorda             param;
    Iussum*            iussum;
    ChordaAedificator* a;
    character          numerus[XXXII];
    s32                tessera;

    imperium = (Imperium*)colloquium_datum(colloquium);
    p        = colloquium_piscina(colloquium);
    param    = colloquium_param(colloquium, "tessera");

    si (imperium == NIHIL)
    {
        _respondere_json(colloquium, D,
            chorda_ex_literis("{\"culpa\":\"imperium abest\"}", p));
        redde;
    }
    /* /imperium/species - id fenestrae, ut instrumentum exterius
     * SOLAM hanc fenestram capiat */
    si (chorda_aequalis_literis(param, "species"))
    {
        a = chorda_aedificator_creare(p, LXIV);
        sprintf(numerus, "%d", (integer)imperium->id_fenestrae);
        chorda_aedificator_appendere_literis(a, "{\"fenestra\":");
        chorda_aedificator_appendere_literis(a, numerus);
        chorda_aedificator_appendere_literis(a, "}");
        _respondere_json(colloquium, CC, chorda_aedificator_finire(a));
        redde;
    }

    si (!chorda_ut_s32(param, &tessera))
    {
        _respondere_json(colloquium, CD,
            chorda_ex_literis("{\"culpa\":\"tessera non numerus\"}",
                p));
        redde;
    }
    iussum = _iussum_invenire(imperium, (i32)tessera);
    si (iussum == NIHIL)
    {
        _respondere_json(colloquium, CDIV,
            chorda_ex_literis("{\"culpa\":\"tessera ignota\"}", p));
        redde;
    }

    a = chorda_aedificator_creare(p, CCLVI);
    si (iussum->status == IMPERIUM_PENDENS)
    {
        chorda_aedificator_appendere_literis(a,
            "{\"status\":\"pendens\"}");
    }
    alioquin si (iussum->status == IMPERIUM_PERFECTUM)
    {
        chorda_aedificator_appendere_literis(a,
            "{\"status\":\"perfectum\",\"valor\":");
        chorda_aedificator_appendere_chorda(a, iussum->valor);
        chorda_aedificator_appendere_literis(a, "}");
    }
    alioquin
    {
        /* effugium contentum solum effugit - virgulae NOSTRAE sunt */
        chorda_aedificator_appendere_literis(a,
            "{\"status\":\"culpa\",\"nuntius\":\"");
        chorda_aedificator_appendere_evasus_json(a, iussum->valor);
        chorda_aedificator_appendere_literis(a, "\"}");
    }
    _respondere_json(colloquium, CC, chorda_aedificator_finire(a));
}

/* ========================================================================
 * TRACTATOR PONTIS - 'imperium.responsum'
 * ======================================================================== */

interior JsonValor*
_responsum (
    JsonValor* argumenta,
    Piscina*   piscina,
    vacuum*    datum,
    chorda*    culpa_exitus)
{
    Imperium*  imperium = (Imperium*)datum;
    JsonValor* j_tessera;
    JsonValor* j_valor;
    JsonValor* j_culpa;
    Iussum*    iussum;

    (vacuum)culpa_exitus;

    si (imperium == NIHIL || argumenta == NIHIL)
    {
        redde json_objectum_creare(piscina);
    }
    j_tessera = json_objectum_capere(argumenta, "tessera");
    si (j_tessera == NIHIL)
    {
        redde json_objectum_creare(piscina);
    }
    iussum = _iussum_invenire(imperium,
        (i32)json_ad_integer(j_tessera));
    si (iussum == NIHIL)
    {
        redde json_objectum_creare(piscina);
    }

    j_culpa = json_objectum_capere(argumenta, "culpa");
    j_valor = json_objectum_capere(argumenta, "valor");

    si (j_culpa != NIHIL)
    {
        iussum->status = IMPERIUM_CULPA;
        iussum->valor  = chorda_transcribere(json_ad_chorda(j_culpa),
            imperium->piscina);
        imperium->fructus.culpae = imperium->fructus.culpae + I;
    }
    alioquin
    {
        iussum->status = IMPERIUM_PERFECTUM;
        iussum->valor  = chorda_transcribere(json_ad_chorda(j_valor),
            imperium->piscina);
    }
    imperium->fructus.responsa_recepta =
        imperium->fructus.responsa_recepta + I;

    redde json_objectum_creare(piscina);
}

/* ========================================================================
 * API PUBLICA
 * ======================================================================== */

Imperium*
imperium_creare (
    Piscina*           piscina,
    ImperiumAestimator aestimator,
    vacuum*            datum)
{
    Imperium* imperium;

    si (piscina == NIHIL || aestimator == NIHIL)
    {
        redde NIHIL;
    }
    imperium = (Imperium*)piscina_allocare(piscina,
        (i32)magnitudo(Imperium));
    si (imperium == NIHIL)
    {
        redde NIHIL;
    }
    imperium->piscina         = piscina;
    imperium->aestimator      = aestimator;
    imperium->datum           = datum;
    imperium->iussa           = xar_creare(piscina,
        (i32)magnitudo(Iussum));
    imperium->tessera_proxima = I;
    imperium->id_fenestrae    = ZEPHYRUM;
    imperium->fructus.iussa_missa      = ZEPHYRUM;
    imperium->fructus.responsa_recepta = ZEPHYRUM;
    imperium->fructus.culpae           = ZEPHYRUM;
    imperium->fructus.pendentia        = ZEPHYRUM;

    si (imperium->iussa == NIHIL)
    {
        redde NIHIL;
    }
    redde imperium;
}

b32
imperium_praebere (
    Imperium*     imperium,
    Hospitium*    hospitium,
    Internuntius* internuntius)
{
    si (imperium == NIHIL || hospitium == NIHIL
        || internuntius == NIHIL)
    {
        redde FALSUM;
    }
    si (!hospitium_praebere(hospitium, HTTP_POST, "/imperium",
        _iubere, imperium))
    {
        redde FALSUM;
    }
    si (!hospitium_praebere(hospitium, HTTP_GET, "/imperium/:tessera",
        _quaerere, imperium))
    {
        redde FALSUM;
    }
    si (!internuntius_praebere(internuntius, "imperium.responsum",
        _responsum, imperium))
    {
        redde FALSUM;
    }
    redde VERUM;
}

vacuum
imperium_fenestram_ponere (
    Imperium* imperium,
    i32       id_fenestrae)
{
    si (imperium != NIHIL)
    {
        imperium->id_fenestrae = id_fenestrae;
    }
}

ImperiumFructus
imperium_fructus (
    constans Imperium* imperium)
{
    ImperiumFructus vacua;

    vacua.iussa_missa      = ZEPHYRUM;
    vacua.responsa_recepta = ZEPHYRUM;
    vacua.culpae           = ZEPHYRUM;
    vacua.pendentia        = ZEPHYRUM;

    si (imperium == NIHIL)
    {
        redde vacua;
    }
    {
        ImperiumFructus f = imperium->fructus;

        f.pendentia = f.iussa_missa - f.responsa_recepta;
        redde f;
    }
}
