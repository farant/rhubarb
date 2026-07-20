/* speculum.c - Modus-debug se-fontis pro apps vitreis
 *
 * Vide speculum.h pro usu; project-specs/speculum-spec-v2.md pro
 * decisionibus. Corpora fontium in piscinam speculi PIGRE inflantur
 * (primum obtinere) et retinentur; arbor JSON responsi in piscina
 * vocationis per json_chorda_creare (zero-copia) super ea struitur.
 */

#include "speculum.h"
#include "json.h"
#include "xar.h"
#include "chorda_aedificator.h"

#include <string.h>


/* ========================================================================
 * TYPI INTERNI
 * ======================================================================== */

nomen structura {
    chorda via;
    chorda corpus;
} SpeculumIntroitus;

structura Speculum {
    Piscina*           piscina;
    Capsula*           capsula;
    SpeculumAestimator aestimator;
    vacuum*            aestimator_datum;
    chorda             stipes;    /* evaluatio se-sanans, semel structa */
    Xar*               cache;     /* SpeculumIntroitus; NIHIL donec primum obtinere */
};


/* ========================================================================
 * ADIUTORES CHORDARUM
 * ======================================================================== */

interior b32
_aequalis_literis(chorda textus, constans character* literis)
{
    i32 mensura;

    mensura = (i32)strlen(literis);
    si (textus.mensura != mensura)
    {
        redde FALSUM;
    }
    redde memcmp(textus.datum, literis, (size_t)mensura) == 0;
}

interior b32
_terminatur(chorda textus, constans character* suffixum)
{
    i32 mensura;

    mensura = (i32)strlen(suffixum);
    si (textus.mensura < mensura)
    {
        redde FALSUM;
    }
    redde memcmp(textus.datum + (textus.mensura - mensura), suffixum,
        (size_t)mensura) == 0;
}


/* ========================================================================
 * CACHE FONTIUM (piger)
 * ======================================================================== */

interior vacuum
_cache_implere(Speculum* s)
{
    i32 i;
    i32 numerus;

    si (s->cache != NIHIL)
    {
        redde;
    }
    s->cache = xar_creare(s->piscina, (i32)magnitudo(SpeculumIntroitus));
    si (s->cache == NIHIL)
    {
        redde;
    }
    numerus = capsula_numerus(s->capsula);
    per (i = 0; i < numerus; i++)
    {
        CapsulaIndexum*    tabella;
        CapsulaFructus     fructus;
        SpeculumIntroitus* intro;

        tabella = capsula_indexum(s->capsula, i);
        si (tabella == NIHIL)
        {
            perge;
        }
        fructus = capsula_legere_chorda(s->capsula, tabella->via,
            s->piscina);
        si (fructus.status != CAPSULA_OK)
        {
            perge;
        }
        intro = (SpeculumIntroitus*)xar_addere(s->cache);
        si (intro == NIHIL)
        {
            redde;
        }
        intro->via = tabella->via;
        intro->corpus = fructus.datum;
    }
}


/* ========================================================================
 * STRUCTIO RESPONSI
 * ======================================================================== */

interior JsonValor*
_introitus_struere(constans SpeculumIntroitus* intro, Piscina* pv)
{
    JsonValor* obiectum;

    obiectum = json_objectum_creare(pv);
    json_objectum_ponere(obiectum, "via", json_chorda_creare(pv, intro->via));
    json_objectum_ponere(obiectum, "magnitudo",
        json_integer_creare(pv, (s64)intro->corpus.mensura));
    json_objectum_ponere(obiectum, "corpus",
        json_chorda_creare(pv, intro->corpus));
    redde obiectum;
}

/* proventus.txt: lineae clavis=valor -> obiectum planum */
interior vacuum
_proventus_parsare(chorda corpus, JsonValor* obiectum, Piscina* pv)
{
    i32 initium;
    i32 i;

    initium = 0;
    per (i = 0; i <= corpus.mensura; i++)
    {
        si (i == corpus.mensura || corpus.datum[i] == '\n')
        {
            chorda linea;
            i32    j;

            linea.datum = corpus.datum + initium;
            linea.mensura = i - initium;
            per (j = 0; j < linea.mensura; j++)
            {
                si (linea.datum[j] == '=')
                {
                    frange;
                }
            }
            si (j > 0 && j < linea.mensura)
            {
                chorda clavis;
                chorda valor;

                clavis.datum = linea.datum;
                clavis.mensura = j;
                valor.datum = linea.datum + j + I;
                valor.mensura = linea.mensura - j - I;
                json_objectum_ponere_chorda(obiectum, clavis,
                    json_chorda_creare(pv, valor));
            }
            initium = i + I;
        }
    }
}

/* exclusa.txt: lineae via<TAB>magnitudo<TAB>digestum<TAB>causa ->
 * tabulatum obiectorum (campi ut chordae; causa spatiis tolerans) */
interior JsonValor*
_exclusa_parsare(chorda corpus, Piscina* pv)
{
    JsonValor* tabulatum;
    i32        initium;
    i32        i;

    tabulatum = json_tabulatum_creare(pv);
    initium = 0;
    per (i = 0; i <= corpus.mensura; i++)
    {
        si (i == corpus.mensura || corpus.datum[i] == '\n')
        {
            chorda linea;

            linea.datum = corpus.datum + initium;
            linea.mensura = i - initium;
            si (linea.mensura > 0)
            {
                JsonValor* obiectum;
                chorda     campi[IV];
                i32        numerus_camporum;
                i32        j;
                i32        campus_initium;
                constans character* claves[IV];

                claves[0] = "via";
                claves[I] = "magnitudo";
                claves[II] = "digestum";
                claves[III] = "causa";

                numerus_camporum = 0;
                campus_initium = 0;
                per (j = 0; j <= linea.mensura && numerus_camporum < IV; j++)
                {
                    b32 finis_campi;

                    finis_campi = (j == linea.mensura
                        || (linea.datum[j] == '\t'
                            && numerus_camporum < III));
                    si (finis_campi)
                    {
                        campi[numerus_camporum].datum =
                            linea.datum + campus_initium;
                        campi[numerus_camporum].mensura =
                            j - campus_initium;
                        numerus_camporum++;
                        campus_initium = j + I;
                    }
                }
                obiectum = json_objectum_creare(pv);
                per (j = 0; j < numerus_camporum; j++)
                {
                    json_objectum_ponere(obiectum, claves[j],
                        json_chorda_creare(pv, campi[j]));
                }
                json_tabulatum_addere(tabulatum, obiectum);
            }
            initium = i + I;
        }
    }
    redde tabulatum;
}

interior JsonValor*
_speculum_obtinere(JsonValor* argumenta, Piscina* pv, vacuum* datum,
    chorda* culpa)
{
    Speculum*  s;
    JsonValor* radix;
    JsonValor* fontes;
    JsonValor* documenta;
    JsonValor* proventus;
    JsonValor* exclusa;
    i32        i;

    (vacuum)argumenta;
    (vacuum)culpa;
    s = (Speculum*)datum;
    _cache_implere(s);

    radix = json_objectum_creare(pv);
    fontes = json_tabulatum_creare(pv);
    documenta = json_tabulatum_creare(pv);
    proventus = json_objectum_creare(pv);
    exclusa = json_tabulatum_creare(pv);

    per (i = 0; i < xar_numerus(s->cache); i++)
    {
        SpeculumIntroitus* intro;

        intro = (SpeculumIntroitus*)xar_obtinere(s->cache, i);
        si (intro == NIHIL)
        {
            perge;
        }
        si (_aequalis_literis(intro->via, "speculum/proventus.txt"))
        {
            _proventus_parsare(intro->corpus, proventus, pv);
        }
        alioquin si (_aequalis_literis(intro->via, "speculum/exclusa.txt"))
        {
            exclusa = _exclusa_parsare(intro->corpus, pv);
        }
        alioquin si (_terminatur(intro->via, ".worklog.md"))
        {
            json_tabulatum_addere(documenta, _introitus_struere(intro, pv));
        }
        alioquin
        {
            json_tabulatum_addere(fontes, _introitus_struere(intro, pv));
        }
    }

    json_objectum_ponere(radix, "proventus", proventus);
    json_objectum_ponere(radix, "fontes", fontes);
    json_objectum_ponere(radix, "documenta", documenta);
    json_objectum_ponere(radix, "exclusa", exclusa);
    redde radix;
}


/* ========================================================================
 * STIPES EVALUATIONIS (se-sanans)
 * ======================================================================== */

interior chorda
_stipes_struere(Speculum* s)
{
    CapsulaFructus     scriptum;
    CapsulaFructus     stylus;
    ChordaAedificator* aed;
    memoriae_index     capacitas;

    scriptum = capsula_legere(s->capsula,
        "lib/speculum_assets/speculum.js", s->piscina);
    stylus = capsula_legere(s->capsula,
        "lib/speculum_assets/speculum.css", s->piscina);

    capacitas = 4096;
    si (scriptum.status == CAPSULA_OK)
    {
        capacitas += (memoriae_index)scriptum.datum.mensura;
    }
    si (stylus.status == CAPSULA_OK)
    {
        capacitas += (memoriae_index)stylus.datum.mensura * II;
    }
    aed = chorda_aedificator_creare(s->piscina, capacitas);
    si (aed == NIHIL)
    {
        redde chorda_ex_literis("", s->piscina);
    }

    chorda_aedificator_appendere_literis(aed, "(function () {\n");
    chorda_aedificator_appendere_literis(aed,
        "if (window.speculum_praesens) { window.speculum_commutare(); return; }\n");
    si (stylus.status == CAPSULA_OK)
    {
        chorda effugatum;

        effugatum = internuntius_effugere_js(stylus.datum, s->piscina);
        chorda_aedificator_appendere_literis(aed,
            "window.speculum_stylus_fons = \"");
        chorda_aedificator_appendere_chorda(aed, effugatum);
        chorda_aedificator_appendere_literis(aed, "\";\n");
    }
    si (scriptum.status == CAPSULA_OK)
    {
        chorda_aedificator_appendere_chorda(aed, scriptum.datum);
        chorda_aedificator_appendere_literis(aed, "\n");
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(aed,
            "console.log(\"[speculum] bona desunt in capsula\");\n");
    }
    chorda_aedificator_appendere_literis(aed, "})();");
    redde chorda_aedificator_finire(aed);
}


/* ========================================================================
 * API PUBLICA
 * ======================================================================== */

Speculum*
speculum_creare(Piscina* piscina, constans CapsulaEmbed* fontes,
    Internuntius* inx, SpeculumAestimator aestimator,
    vacuum* aestimator_datum)
{
    Speculum* s;

    si (piscina == NIHIL || fontes == NIHIL || inx == NIHIL)
    {
        redde NIHIL;
    }
    s = (Speculum*)piscina_allocare(piscina, magnitudo(Speculum));
    memset(s, 0, magnitudo(Speculum));
    s->piscina = piscina;
    s->aestimator = aestimator;
    s->aestimator_datum = aestimator_datum;
    s->capsula = capsula_aperire(fontes, piscina);
    si (s->capsula == NIHIL)
    {
        redde NIHIL;
    }
    si (!internuntius_praebere(inx, "speculum_obtinere",
        _speculum_obtinere, s))
    {
        redde NIHIL;
    }
    s->stipes = _stipes_struere(s);
    redde s;
}

b32
speculum_tangere(Speculum* speculum, constans Eventus* eventus)
{
    si (speculum == NIHIL || eventus == NIHIL)
    {
        redde FALSUM;
    }
    si (eventus->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    si (eventus->datum.clavis.clavis != 'D')
    {
        redde FALSUM;
    }
    si ((eventus->datum.clavis.modificantes & (i32)MOD_SUPER) == 0)
    {
        redde FALSUM;
    }
    si ((eventus->datum.clavis.modificantes & (i32)MOD_SHIFT) == 0)
    {
        redde FALSUM;
    }
    si (speculum->aestimator != NIHIL)
    {
        speculum->aestimator(speculum->aestimator_datum, speculum->stipes);
    }
    redde VERUM;
}
