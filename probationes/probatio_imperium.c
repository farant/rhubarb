/* probatio_imperium.c - Probationes ansae imperandi
 *
 * SINE FENESTRA: imperium aestimatorem ut SUTURAM accipit, ergo
 * paginam falsam supponere licet et totam ansam sine vitrea probare.
 * Ea est ratio propter quam sutura exstat.
 *
 * Exemplar clientis: probatio_hospitium (cliens obstruens + pumpatio
 * manualis gressuum in eodem filo - nulla fila in hac domo).
 *
 * ITER PROBATUM: POST -> aestimator JS involutum capit -> pons
 * responsum refert (internuntius_tractare, ut si pagina vocasset)
 * -> GET valorem reddit.
 */

#include "imperium.h"
#include "hospitium.h"
#include "internuntius.h"
#include "tcp.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>

/* ========================================================================
 * SUTURAE FALSAE
 * ======================================================================== */

nomen structura {
    character ultimum[4096];
    i32       numerus;
} AestimatorFalsus;

interior AestimatorFalsus aestimator_falsus;

/* sutura: JS quod imperium mitteret capit loco vitreae */
interior vacuum
_aestimator_capiens (
    vacuum* datum,
    chorda  js)
{
    i32 m;

    (vacuum)datum;
    m = js.mensura < (i32)magnitudo(aestimator_falsus.ultimum) - I
        ? js.mensura
        : (i32)magnitudo(aestimator_falsus.ultimum) - I;
    memcpy(aestimator_falsus.ultimum, js.datum, (memoriae_index)m);
    aestimator_falsus.ultimum[m] = '\0';
    aestimator_falsus.numerus = aestimator_falsus.numerus + I;
}

/* imaginator falsus: viam servat et STATIM perficit (vera vitrea
 * gyro proximo redderet - hic sine mora, ut probatio deterministica
 * maneat) */
interior character imaginator_via[512];
interior i32       imaginator_numerus;

interior b32
_imaginator_falsus (
    vacuum*             datum,
    constans character* via,
    ImperiumImagoFacta  facta,
    vacuum*             facta_datum)
{
    (vacuum)datum;
    strncpy(imaginator_via, via, magnitudo(imaginator_via) - I);
    imaginator_via[magnitudo(imaginator_via) - I] = '\0';
    imaginator_numerus = imaginator_numerus + I;
    si (facta != NIHIL)
    {
        facta(facta_datum, VERUM);
    }
    redde VERUM;
}

/* missor internuntii: responsa in vacuum eunt (pagina non adest) */
interior vacuum
_missor_mutus (
    vacuum* datum,
    chorda  textus)
{
    (vacuum)datum;
    (vacuum)textus;
}

/* ========================================================================
 * AUXILIA CLIENTIS
 * ======================================================================== */

interior vacuum
_pumpare (Hospitium* h, i32 vices)
{
    i32 i;

    per (i = ZEPHYRUM; i < vices; i = i + I)
    {
        hospitium_gressus(h, X);
    }
}

interior s32
_commercium (
    Hospitium*          h,
    Piscina*            piscina,
    constans character* petitio,
    character*          buffer,
    i32                 capacitas)
{
    TcpOptiones  opt = tcp_optiones_default();
    TcpResultus  res;
    TcpConnexio* cliens;
    s32          n;

    opt.timeout_ms = MM;
    res = tcp_connectere_cum_optionibus("127.0.0.1",
        hospitium_portus(h), &opt, piscina);
    si (!res.successus)
    {
        redde -I;
    }
    cliens = res.connexio;
    si (!tcp_mittere_omnia(cliens, (constans i8*)petitio,
        (i32)strlen(petitio)))
    {
        tcp_claudere(cliens);
        redde -I;
    }
    _pumpare(h, X);
    n = tcp_recipere(cliens, (i8*)buffer, capacitas - I);
    si (n > ZEPHYRUM)
    {
        buffer[n] = '\0';
    }
    alioquin
    {
        buffer[ZEPHYRUM] = '\0';
    }
    tcp_claudere(cliens);
    redde n;
}

/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

s32 principale (vacuum);

s32
principale (vacuum)
{
    b32                   praeteritus;
    Piscina*              piscina;
    Piscina*              pv;
    HospitiumConfiguratio figura;
    Hospitium*            hospitium;
    Internuntius*         inx;
    Imperium*             imperium;
    ImperiumFructus       fructus;
    character             buffer[8192];

    piscina = piscina_generare_dynamicum("probatio_imperium", 4194304);
    pv      = piscina_generare_dynamicum("probatio_imperium_v", 1048576);
    si (!piscina || !pv)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    aestimator_falsus.numerus = ZEPHYRUM;
    aestimator_falsus.ultimum[ZEPHYRUM] = '\0';

    memset(&figura, 0, magnitudo(figura));
    figura.portus = ZEPHYRUM;
    hospitium = hospitium_creare(piscina, &figura);
    inx = internuntius_creare(piscina, _missor_mutus, NIHIL);

    CREDO_NON_NIHIL(hospitium);
    CREDO_NON_NIHIL(inx);

    /* --- I. creatio + custodes argumentorum --- */
    CREDO_NIHIL(imperium_creare(NIHIL, _aestimator_capiens, NIHIL));
    CREDO_NIHIL(imperium_creare(piscina, NIHIL, NIHIL));

    imperium = imperium_creare(piscina, _aestimator_capiens, NIHIL);
    CREDO_NON_NIHIL(imperium);
    CREDO_FALSUM(imperium_praebere(imperium, NIHIL, inx));
    CREDO_VERUM(imperium_praebere(imperium, hospitium, inx));

    imperium_fenestram_ponere(imperium, 4242);

    /* --- II. iussum missum: tessera redditur, aestimator vocatur --- */
    _commercium(hospitium, pv,
        "POST /imperium HTTP/1.1\r\nHost: x\r\n"
        "Content-Length: 3\r\n\r\n1+1", buffer, (i32)magnitudo(buffer));
    CREDO_VERUM(strstr(buffer, "\"tessera\":1") != NIHIL);
    CREDO_AEQUALIS_I32(aestimator_falsus.numerus, I);

    /* JS involutum tesseram ET iussum utentis fert; et EVAL adhibet,
     * non corpus functionis (illud undefined redderet - defectus qui
     * omnia 'perfectum/null' pareret) */
    CREDO_VERUM(strstr(aestimator_falsus.ultimum, "var t=1") != NIHIL);
    CREDO_VERUM(strstr(aestimator_falsus.ultimum, "1+1") != NIHIL);
    CREDO_VERUM(strstr(aestimator_falsus.ultimum, "eval") != NIHIL);
    CREDO_VERUM(strstr(aestimator_falsus.ultimum,
        "imperium.responsum") != NIHIL);

    /* --- III. ante responsum: PENDENS (non 'perfectum/null') --- */
    _commercium(hospitium, pv,
        "GET /imperium/1 HTTP/1.1\r\nHost: x\r\n\r\n",
        buffer, (i32)magnitudo(buffer));
    CREDO_VERUM(strstr(buffer, "pendens") != NIHIL);

    /* --- IV. pons responsum refert (ut si pagina vocasset) --- */
    internuntius_tractare(inx, chorda_ex_literis(
        "{\"id\":1,\"methodus\":\"imperium.responsum\","
        "\"argumenta\":{\"tessera\":1,\"valor\":\"2\"}}", pv), pv);

    _commercium(hospitium, pv,
        "GET /imperium/1 HTTP/1.1\r\nHost: x\r\n\r\n",
        buffer, (i32)magnitudo(buffer));
    CREDO_VERUM(strstr(buffer, "perfectum") != NIHIL);
    CREDO_VERUM(strstr(buffer, "\"valor\":2") != NIHIL);

    /* --- V. iter culpae: nuntius VIRGULIS cinctus (JSON validum) --- */
    _commercium(hospitium, pv,
        "POST /imperium HTTP/1.1\r\nHost: x\r\n"
        "Content-Length: 5\r\n\r\nfrang", buffer,
        (i32)magnitudo(buffer));
    CREDO_VERUM(strstr(buffer, "\"tessera\":2") != NIHIL);

    internuntius_tractare(inx, chorda_ex_literis(
        "{\"id\":2,\"methodus\":\"imperium.responsum\","
        "\"argumenta\":{\"tessera\":2,\"culpa\":\"rupta est\"}}",
        pv), pv);

    _commercium(hospitium, pv,
        "GET /imperium/2 HTTP/1.1\r\nHost: x\r\n\r\n",
        buffer, (i32)magnitudo(buffer));
    CREDO_VERUM(strstr(buffer, "culpa") != NIHIL);
    CREDO_VERUM(strstr(buffer, "\"nuntius\":\"rupta est\"") != NIHIL);

    /* --- VI. species: id fenestrae pro captura extrinseca --- */
    _commercium(hospitium, pv,
        "GET /imperium/species HTTP/1.1\r\nHost: x\r\n\r\n",
        buffer, (i32)magnitudo(buffer));
    CREDO_VERUM(strstr(buffer, "\"fenestra\":4242") != NIHIL);

    /* --- VII. tessera ignota: CDIV, non transitus tacitus --- */
    _commercium(hospitium, pv,
        "GET /imperium/999 HTTP/1.1\r\nHost: x\r\n\r\n",
        buffer, (i32)magnitudo(buffer));
    CREDO_VERUM(strstr(buffer, "404") != NIHIL);

    /* --- VIII. imago SINE imaginatore: CDIV, recusatio APERTA
     *      (non tessera quae numquam perficitur) --- */
    _commercium(hospitium, pv,
        "POST /imperium/imago HTTP/1.1\r\nHost: x\r\n"
        "Content-Length: 0\r\n\r\n", buffer, (i32)magnitudo(buffer));
    CREDO_VERUM(strstr(buffer, "404") != NIHIL);
    CREDO_AEQUALIS_I32(imaginator_numerus, ZEPHYRUM);

    /* --- IX. imago CUM imaginatore: tessera + via in valore --- */
    imperium_imaginatorem_ponere(imperium, _imaginator_falsus, NIHIL);
    _commercium(hospitium, pv,
        "POST /imperium/imago HTTP/1.1\r\nHost: x\r\n"
        "Content-Length: 14\r\n\r\n/tmp/probo.png", buffer,
        (i32)magnitudo(buffer));
    CREDO_VERUM(strstr(buffer, "\"tessera\":3") != NIHIL);
    CREDO_AEQUALIS_I32(imaginator_numerus, I);
    CREDO_VERUM(strstr(imaginator_via, "/tmp/probo.png") != NIHIL);

    _commercium(hospitium, pv,
        "GET /imperium/3 HTTP/1.1\r\nHost: x\r\n\r\n",
        buffer, (i32)magnitudo(buffer));
    CREDO_VERUM(strstr(buffer, "perfectum") != NIHIL);
    CREDO_VERUM(strstr(buffer, "/tmp/probo.png") != NIHIL);

    /* --- X. numeratores --- */
    fructus = imperium_fructus(imperium);
    CREDO_AEQUALIS_I32((i32)fructus.iussa_missa, III);
    CREDO_AEQUALIS_I32((i32)fructus.responsa_recepta, III);
    CREDO_AEQUALIS_I32((i32)fructus.culpae, I);
    CREDO_AEQUALIS_I32((i32)fructus.pendentia, ZEPHYRUM);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    hospitium_destruere(hospitium);
    piscina_destruere(pv);
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
