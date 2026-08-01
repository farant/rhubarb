/* vitrea_servus.c - transportus HTTP applicationis vitreae
 *
 * Vide vitrea_servus.h pro consilio. Hic tres res tantum fiunt:
 * (1) sutura missoris super colloquium hospitii, (2) scriptum
 * pontis in paginam insertum dum servitur, (3) via una per filum
 * capsulae.
 */

#include "vitrea_servus.h"
#include "hospitium.h"
#include "http.h"
#include "mimen.h"

#include <string.h>

/* ========================================================================
 * SCRIPTUM PONTIS - gemellum VITREA_CURSUS_JS pro transportu HTTP
 * ========================================================================
 *
 * Vitrea suum scriptum per WKUserScript ad documenti-initium
 * iniicit; nos nostrum in <head> inserimus dum paginam servimus.
 * Semantica eadem: 'window.internuntius' adest antequam pagina
 * currat, et applicatio definitionem nusquam scribit.
 *
 * SIMPLICIUS quam gemellum WebKit de industria: tabula
 * 'pendentia' et numerator 'proximus' illic exsistunt quia fistula
 * UNA omnia miscet et responsa per id itineranda sunt. HTTP
 * petitionem responso ipse copulat, ergo strato itinerandi opus
 * non est. */
interior constans character* constans VITREA_SERVUS_SCRIPTUM =
"<script>\n"
"(function(){\n"
"window.internuntius = {\n"
"  vocare: function (methodus, argumenta) {\n"
"    return fetch('/internuntius', {\n"
"      method: 'POST',\n"
"      headers: { 'Content-Type': 'application/json' },\n"
"      body: JSON.stringify({ id: 1, methodus: methodus,\n"
"                             argumenta: argumenta || {} })\n"
"    }).then(function (r) { return r.json(); })\n"
"      .then(function (n) {\n"
"        if (n.culpa) { throw new Error(n.culpa.nuntius); }\n"
"        return n.fructus;\n"
"      });\n"
"  },\n"
"  audire: function (eventus) {\n"
"    console.error('internuntius: pulsus non transeunt super'\n"
"      + ' transportum HTTP (eventus: ' + eventus + ').'\n"
"      + ' AUDITOR HIC NUMQUAM VOCABITUR - SSE nondum'\n"
"      + ' aedificatum.');\n"
"  }\n"
"};\n"
"})();\n"
"</script>\n";

/* ========================================================================
 * STATUS
 * ======================================================================== */

structura VitreaServus {
    Piscina*      piscina;
    Hospitium*    hospitium;
    Internuntius* inx;
    Capsula*      capsula;
    /* sedes responsi petitionis currentis: tuta quia hospitium filo
     * uno currit et tractator ad finem currit ante petitionem
     * proximam (vide notam in capite) */
    Piscina*      piscina_petitionis;
    chorda        responsum;
    b32           responsum_positum;
};

/* ligamen per assetum registratum (datum viae hospitii) */
nomen structura {
    VitreaServus*       servus;
    constans character* via;         /* via in capsula, terminata */
    b32                 est_pagina;  /* .html -> scriptum inseritur */
} AssetumLigamen;

/* ========================================================================
 * AUXILIA
 * ======================================================================== */

/* chorda -> litterae terminatae in piscina (capsula et mimen
 * chordas C exspectant) */
interior character*
_litterae_ex_chorda (chorda c, Piscina* piscina)
{
    character* s = (character*)piscina_allocare(piscina,
        (memoriae_index)(c.mensura + I));

    si (s == NIHIL)
    {
        redde NIHIL;
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(s, c.datum, (size_t)c.mensura);
    }
    s[c.mensura] = '\0';
    redde s;
}

/* litterae -> chorda sine allocatione (mos domus: unio constantiam
 * lavat - vide gesta/fontes/tabularium.c:345). Chordae ita factae
 * litteras STATICAS solum monstrent. */
interior chorda
_ch (constans character* litterae)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l = litterae;
    c.datum = u.m;
    c.mensura = (i32)strlen(litterae);
    redde c;
}

/* scriptum pontis post <head> inserere; sine <head>, ante omnia
 * (pagina fragmentaria adhuc laborat - scriptum ante usum currit) */
interior chorda
_scriptum_inserere (chorda pagina, Piscina* piscina)
{
    chorda acus = _ch("<head>");
    chorda scriptum = _ch(VITREA_SERVUS_SCRIPTUM);
    chorda ante;
    chorda post;
    s32    index;
    i32    scissura;

    index = chorda_invenire_index(pagina, acus);
    si (index < 0)
    {
        redde chorda_concatenare(scriptum, pagina, piscina);
    }
    scissura = (i32)index + acus.mensura;
    ante = chorda_sectio(pagina, ZEPHYRUM, scissura);
    post = chorda_sectio(pagina, scissura, pagina.mensura);
    redde chorda_concatenare(chorda_concatenare(ante, scriptum,
        piscina), post, piscina);
}

/* ========================================================================
 * SUTURA MISSORIS
 * ======================================================================== */

interior vacuum
_missor (vacuum* datum, chorda textus)
{
    VitreaServus* s = (VitreaServus*)datum;

    si (s->responsum_positum)
    {
        redde;   /* primum vincit - responsum unum per petitionem */
    }
    /* CONTRACTUS MISSORIS: textus valet intra vocationem SOLUM */
    s->responsum = chorda_transcribere(textus, s->piscina_petitionis);
    s->responsum_positum = VERUM;
}

interior vacuum
_pons_tractator (HospitiumColloquium* colloquium)
{
    VitreaServus* s  = (VitreaServus*)colloquium_datum(colloquium);
    Piscina*      pn = colloquium_piscina(colloquium);
    constans HttpPetitioServeri* petitio =
        colloquium_petitio(colloquium);

    s->piscina_petitionis = pn;
    s->responsum_positum  = FALSUM;
    s->responsum.mensura  = ZEPHYRUM;
    s->responsum.datum    = NIHIL;

    internuntius_tractare(s->inx, petitio->corpus, pn);

    si (!s->responsum_positum)
    {
        /* internuntius SEMPER respondet (culpae numquam frangunt) -
         * ergo silentium vitium nostrum est, non petentis */
        colloquium_respondere(colloquium, DII, "application/json",
            _ch("{\"culpa\":{\"nuntius\":\"internuntius tacuit\"}}"));
        redde;
    }
    colloquium_respondere(colloquium, CC, "application/json",
        s->responsum);
}

/* ========================================================================
 * ASSETA
 * ======================================================================== */

interior vacuum
_assetum_tractator (HospitiumColloquium* colloquium)
{
    AssetumLigamen* lig =
        (AssetumLigamen*)colloquium_datum(colloquium);
    Piscina*        pn  = colloquium_piscina(colloquium);
    CapsulaFructus  res;
    chorda          corpus;

    res = capsula_legere(lig->servus->capsula, lig->via, pn);
    si (res.status != CAPSULA_OK)
    {
        colloquium_respondere(colloquium, CDIV, "text/plain",
            _ch("assetum absens"));
        redde;
    }
    corpus = res.datum;
    si (lig->est_pagina)
    {
        corpus = _scriptum_inserere(corpus, pn);
    }
    colloquium_respondere(colloquium, CC,
        mimen_pro_via(lig->via), corpus);
}

interior b32
_est_pagina (chorda via)
{
    redde chorda_terminatur(via, _ch(".html"))
        || chorda_terminatur(via, _ch(".htm"));
}

/* viam URL ex via capsulae: "/" praefixum */
interior character*
_url_ex_via (chorda via, Piscina* piscina)
{
    character* s = (character*)piscina_allocare(piscina,
        (memoriae_index)(via.mensura + II));

    si (s == NIHIL)
    {
        redde NIHIL;
    }
    s[ZEPHYRUM] = '/';
    si (via.mensura > ZEPHYRUM)
    {
        memcpy(s + I, via.datum, (size_t)via.mensura);
    }
    s[via.mensura + I] = '\0';
    redde s;
}

/* ========================================================================
 * CYCLUS VITAE
 * ======================================================================== */

VitreaServus*
vitrea_servus_creare (Piscina* piscina,
    constans VitreaServusConfiguratio* configuratio)
{
    VitreaServus*         servus;
    HospitiumConfiguratio cfg;
    constans character*   via_initialis;
    i32                   numerus;
    i32                   i;

    si (piscina == NIHIL || configuratio == NIHIL
        || configuratio->capsula == NIHIL
        || configuratio->praebitor == NIHIL)
    {
        redde NIHIL;
    }

    servus = (VitreaServus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(VitreaServus));
    si (servus == NIHIL)
    {
        redde NIHIL;
    }
    memset(servus, ZEPHYRUM, magnitudo(VitreaServus));
    servus->piscina = piscina;
    servus->capsula = configuratio->capsula;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.hospes        = configuratio->hospes;   /* NIHIL = loopback */
    cfg.portus        = configuratio->portus;
    cfg.acta_accessus = configuratio->acta_accessus;
    servus->hospitium = hospitium_creare(piscina, &cfg);
    si (servus->hospitium == NIHIL)
    {
        redde NIHIL;
    }

    servus->inx = internuntius_creare(piscina, _missor, servus);
    si (servus->inx == NIHIL)
    {
        hospitium_destruere(servus->hospitium);
        redde NIHIL;
    }
    /* modus PUBLICUS: applicatio methodos harnesii excludit */
    configuratio->praebitor(servus->inx, INTERNUNTIUS_MODUS_PUBLICUS,
        configuratio->praebitor_datum);

    si (!hospitium_praebere(servus->hospitium, HTTP_POST,
            "/internuntius", _pons_tractator, servus))
    {
        hospitium_destruere(servus->hospitium);
        redde NIHIL;
    }

    via_initialis = configuratio->via_initialis;
    si (via_initialis == NIHIL)
    {
        via_initialis = "index.html";
    }

    /* via una per filum capsulae: superficies servita = contenta
     * capsulae ex constructione. Vias petitionis in systema
     * filorum numquam vertimus, ergo traversalis non est res. */
    numerus = capsula_numerus(configuratio->capsula);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        CapsulaIndexum*  ix  = capsula_indexum(configuratio->capsula, i);
        AssetumLigamen*  lig;
        character*       url;

        si (ix == NIHIL)
        {
            perge;
        }
        lig = (AssetumLigamen*)piscina_allocare(piscina,
            (memoriae_index)magnitudo(AssetumLigamen));
        url = _url_ex_via(ix->via, piscina);
        si (lig == NIHIL || url == NIHIL)
        {
            hospitium_destruere(servus->hospitium);
            redde NIHIL;
        }
        lig->servus     = servus;
        lig->via        = _litterae_ex_chorda(ix->via, piscina);
        lig->est_pagina = _est_pagina(ix->via);
        si (lig->via == NIHIL)
        {
            hospitium_destruere(servus->hospitium);
            redde NIHIL;
        }
        (vacuum)hospitium_praebere(servus->hospitium, HTTP_GET, url,
            _assetum_tractator, lig);

        /* radix -> pagina initialis */
        si (chorda_aequalis_literis(ix->via, via_initialis))
        {
            (vacuum)hospitium_praebere(servus->hospitium, HTTP_GET,
                "/", _assetum_tractator, lig);
        }
    }
    redde servus;
}

vacuum
vitrea_servus_currere (VitreaServus* servus)
{
    si (servus != NIHIL)
    {
        hospitium_currere(servus->hospitium);
    }
}

b32
vitrea_servus_gressus (VitreaServus* servus, s32 ms_maximae)
{
    si (servus == NIHIL)
    {
        redde FALSUM;
    }
    redde hospitium_gressus(servus->hospitium, ms_maximae);
}

vacuum
vitrea_servus_sistere (VitreaServus* servus)
{
    si (servus != NIHIL)
    {
        hospitium_sistere(servus->hospitium);
    }
}

vacuum
vitrea_servus_destruere (VitreaServus* servus)
{
    si (servus != NIHIL && servus->hospitium != NIHIL)
    {
        hospitium_destruere(servus->hospitium);
        servus->hospitium = NIHIL;
    }
}

i32
vitrea_servus_portus (constans VitreaServus* servus)
{
    si (servus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde hospitium_portus(servus->hospitium);
}

Internuntius*
vitrea_servus_internuntius (VitreaServus* servus)
{
    si (servus == NIHIL)
    {
        redde NIHIL;
    }
    redde servus->inx;
}
