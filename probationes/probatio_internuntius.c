/* probatio_internuntius.c - Phasis A vitreae (spec-v2 par XI):
 * involucrum, registrum, viae culpae, pulsus, introspectio,
 * numeratores, effugator (probationes aureae). Missor memoriae =
 * Xar chordarum COPIATARUM (contractus missoris: textus validus
 * solum intra vocationem). Sine WebKit toto. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "internuntius.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * missor memoriae
 * ================================================== */

nomen structura {
    Piscina* piscina;
    Xar*     capta;   /* chorda (valore) - copiae */
} MissorMemoriae;

interior vacuum
_missor_memoriae (vacuum* datum, chorda textus)
{
    MissorMemoriae* m = (MissorMemoriae*)datum;
    chorda copia;
    chorda* locus;

    copia.mensura = textus.mensura;
    copia.datum = NIHIL;
    si (textus.mensura > ZEPHYRUM)
    {
        copia.datum = (i8*)piscina_allocare(m->piscina,
            (memoriae_index)textus.mensura);
        si (copia.datum == NIHIL)
        {
            copia.mensura = ZEPHYRUM;
        }
        alioquin
        {
            memcpy(copia.datum, textus.datum,
                (memoriae_index)textus.mensura);
        }
    }
    locus = (chorda*)xar_addere(m->capta);
    si (locus != NIHIL)
    {
        *locus = copia;
    }
}

/* captum ultimum ut litterae NUL-terminatae ("" si nullum) */
interior constans character*
_ultimum (MissorMemoriae* m)
{
    i32 n = xar_numerus(m->capta);
    chorda* c;
    character* l;

    si (n == ZEPHYRUM)
    {
        redde "";
    }
    c = (chorda*)xar_obtinere(m->capta, n - I);
    l = (character*)piscina_allocare(m->piscina,
        (memoriae_index)c->mensura + I);
    si (l == NIHIL)
    {
        redde "";
    }
    si (c->mensura > ZEPHYRUM)
    {
        memcpy(l, c->datum, (memoriae_index)c->mensura);
    }
    l[c->mensura] = '\0';
    redde l;
}

/* ==================================================
 * tractatores probationis
 * ================================================== */

/* resonans: fructus = {"vox": argumenta.vox} */
interior JsonValor*
_tractator_resonare (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    JsonValor* fructus = json_objectum_creare(piscina);
    JsonValor* vox = NIHIL;

    (vacuum)datum;
    (vacuum)culpa;
    si (argumenta != NIHIL)
    {
        vox = json_objectum_capere(argumenta, "vox");
    }
    json_objectum_ponere(fructus, "vox",
        vox != NIHIL ? vox : json_nullum_creare(piscina));
    redde fructus;
}

/* additor cum dato usoris (basis addenda - probat datum) */
interior JsonValor*
_tractator_addere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    s64* basis = (s64*)datum;
    s64 a = ZEPHYRUM;
    s64 b = ZEPHYRUM;
    JsonValor* fructus = json_objectum_creare(piscina);

    (vacuum)culpa;
    si (argumenta != NIHIL)
    {
        a = json_ad_integer(json_objectum_capere(argumenta, "a"));
        b = json_ad_integer(json_objectum_capere(argumenta, "b"));
    }
    json_objectum_ponere(fructus, "summa",
        json_integer_creare(piscina, a + b + *basis));
    redde fructus;
}

/* culpans: semper recusat */
interior JsonValor*
_tractator_culpans (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    (vacuum)argumenta;
    (vacuum)datum;
    *culpa = chorda_ex_literis("res prohibita", piscina);
    redde NIHIL;
}

/* tacens: NIHIL sine culpa -> fructus null */
interior JsonValor*
_tractator_tacens (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    (vacuum)argumenta;
    (vacuum)piscina;
    (vacuum)datum;
    (vacuum)culpa;
    redde NIHIL;
}

/* ==================================================
 * auxilia
 * ================================================== */

interior vacuum
_tractare_literis (Internuntius* inx, constans character* nuntium,
    Piscina* pv)
{
    internuntius_tractare(inx, chorda_ex_literis(nuntium, pv), pv);
}

/* effugator: fons -> exspectatum, byte-exactum */
interior b32
_effugium_aequale (Piscina* piscina, constans character* fons,
    constans character* exspectatum)
{
    chorda f = chorda_ex_literis(fons, piscina);
    chorda e = internuntius_effugere_js(f, piscina);
    memoriae_index m = strlen(exspectatum);

    redde (memoriae_index)e.mensura == m
        && (m == ZEPHYRUM
            || memcmp(e.datum, exspectatum, m) == ZEPHYRUM);
}

s32 principale (vacuum)
{
    Piscina* piscina;
    Piscina* pv;
    MissorMemoriae missor;
    Internuntius* inx;
    s64 basis = C;   /* 100 - datum usoris */
    b32 praeteritus;
    constans character* r;

    piscina = piscina_generare_dynamicum("probatio_inx",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    missor.piscina = piscina;
    missor.capta = xar_creare(piscina, (i32)magnitudo(chorda));

    /* I. creare + registrum */
    inx = internuntius_creare(piscina, _missor_memoriae, &missor);
    CREDO_NON_NIHIL (inx);
    CREDO_VERUM (internuntius_praebere(inx, "resonare",
        _tractator_resonare, NIHIL));
    CREDO_VERUM (internuntius_praebere(inx, "addere",
        _tractator_addere, &basis));
    CREDO_VERUM (internuntius_praebere(inx, "culpans",
        _tractator_culpans, NIHIL));
    CREDO_VERUM (internuntius_praebere(inx, "tacens",
        _tractator_tacens, NIHIL));
    /* duplicatum recusatur - registrum codex */
    CREDO_VERUM (!internuntius_praebere(inx, "resonare",
        _tractator_tacens, NIHIL));
    /* vacuum/NIHIL recusantur */
    CREDO_VERUM (!internuntius_praebere(inx, "",
        _tractator_tacens, NIHIL));
    CREDO_VERUM (!internuntius_praebere(inx, "x", NIHIL, NIHIL));

    /* introspectio: ordo praebitionis */
    {
        Xar* methodi = internuntius_methodi(inx, piscina);

        CREDO_NON_NIHIL (methodi);
        CREDO_AEQUALIS_I32 (xar_numerus(methodi), IV);
        {
            chorda* primus = (chorda*)xar_obtinere(methodi,
                ZEPHYRUM);

            CREDO_VERUM (primus != NIHIL
                && primus->mensura == VIII
                && memcmp(primus->datum, "resonare", VIII)
                    == ZEPHYRUM);
        }
    }

    pv = piscina_generare_dynamicum("probatio_inx_vocatio",
        1048576);
    CREDO_NON_NIHIL (pv);

    /* II. vocatio plena: id + methodus + argumenta */
    _tractare_literis(inx, "{\"id\":7,\"methodus\":\"resonare\","
        "\"argumenta\":{\"vox\":\"salve\"}}", pv);
    r = _ultimum(&missor);
    CREDO_VERUM (strstr(r, "\"id\":7") != NIHIL);
    CREDO_VERUM (strstr(r, "\"fructus\"") != NIHIL);
    CREDO_VERUM (strstr(r, "\"vox\":\"salve\"") != NIHIL);
    CREDO_VERUM (strstr(r, "culpa") == NIHIL);

    /* III. datum usoris vivit (basis 100) */
    _tractare_literis(inx, "{\"id\":8,\"methodus\":\"addere\","
        "\"argumenta\":{\"a\":2,\"b\":3}}", pv);
    r = _ultimum(&missor);
    CREDO_VERUM (strstr(r, "\"summa\":105") != NIHIL);

    /* IV. sine id (ignis-et-obliviscere): responsum sine id */
    _tractare_literis(inx, "{\"methodus\":\"resonare\","
        "\"argumenta\":{\"vox\":\"echo\"}}", pv);
    r = _ultimum(&missor);
    CREDO_VERUM (strstr(r, "\"id\"") == NIHIL);
    CREDO_VERUM (strstr(r, "\"vox\":\"echo\"") != NIHIL);

    /* V. methodus ignota -> culpa cum id et titulo */
    _tractare_literis(inx, "{\"id\":9,\"methodus\":\"absens\"}",
        pv);
    r = _ultimum(&missor);
    CREDO_VERUM (strstr(r, "\"id\":9") != NIHIL);
    CREDO_VERUM (strstr(r, "\"culpa\"") != NIHIL);
    CREDO_VERUM (strstr(r, "methodus ignota: absens") != NIHIL);

    /* VI. json fractum -> culpa ORBA (sine id) */
    _tractare_literis(inx, "{nequaquam json", pv);
    r = _ultimum(&missor);
    CREDO_VERUM (strstr(r, "\"id\"") == NIHIL);
    CREDO_VERUM (strstr(r, "malformatum") != NIHIL);

    /* VII. methodus deest -> culpa cum id */
    _tractare_literis(inx, "{\"id\":10}", pv);
    r = _ultimum(&missor);
    CREDO_VERUM (strstr(r, "\"id\":10") != NIHIL);
    CREDO_VERUM (strstr(r, "methodus deest") != NIHIL);

    /* VIII. culpa tractatoris */
    _tractare_literis(inx, "{\"id\":11,\"methodus\":\"culpans\"}",
        pv);
    r = _ultimum(&missor);
    CREDO_VERUM (strstr(r, "\"id\":11") != NIHIL);
    CREDO_VERUM (strstr(r, "res prohibita") != NIHIL);
    CREDO_VERUM (strstr(r, "fructus") == NIHIL);

    /* IX. tacens -> fructus null (vocatio felix sine valore) */
    _tractare_literis(inx, "{\"id\":12,\"methodus\":\"tacens\"}",
        pv);
    r = _ultimum(&missor);
    CREDO_VERUM (strstr(r, "\"fructus\":null") != NIHIL);

    /* X. pulsus eventuum (cum dato et sine) */
    {
        JsonValor* d = json_objectum_creare(pv);

        json_objectum_ponere(d, "hora",
            json_integer_creare(pv, XII));
        internuntius_eventum_mittere(inx, "horologium", d, pv);
        r = _ultimum(&missor);
        CREDO_VERUM (strstr(r, "\"eventus\":\"horologium\"")
            != NIHIL);
        CREDO_VERUM (strstr(r, "\"hora\":12") != NIHIL);
        CREDO_VERUM (strstr(r, "\"id\"") == NIHIL);

        internuntius_eventum_mittere(inx, "pulsus_nudus", NIHIL,
            pv);
        r = _ultimum(&missor);
        CREDO_VERUM (strstr(r, "\"datum\":null") != NIHIL);
    }

    /* XI. numeratores fructus: felices II/III/IV/IX = 4;
     * culpae V/VI/VII/VIII = 4; eventus X = 2 */
    {
        InternuntiusFructus fr = internuntius_fructus(inx);

        CREDO_VERUM (fr.vocationes_tractatae == (i64)IV);
        CREDO_VERUM (fr.culpae == (i64)IV);
        CREDO_VERUM (fr.eventus_missi == (i64)II);
    }

    /* XII. separatio piscinae vocationis: piscina destructa,
     * registrum vivit - vocatio proxima ex piscina recenti */
    piscina_destruere(pv);
    pv = piscina_generare_dynamicum("probatio_inx_vocatio2",
        1048576);
    CREDO_NON_NIHIL (pv);
    _tractare_literis(inx, "{\"id\":13,\"methodus\":\"resonare\","
        "\"argumenta\":{\"vox\":\"post\"}}", pv);
    r = _ultimum(&missor);
    CREDO_VERUM (strstr(r, "\"vox\":\"post\"") != NIHIL);

    /* XIII. effugator - aureae byte-exactae */
    CREDO_VERUM (_effugium_aequale(piscina,
        "salve mundi", "salve mundi"));
    CREDO_VERUM (_effugium_aequale(piscina,
        "a\"b\\c", "a\\\"b\\\\c"));
    CREDO_VERUM (_effugium_aequale(piscina,
        "l1\nl2\tt\rr", "l1\\nl2\\tt\\rr"));
    CREDO_VERUM (_effugium_aequale(piscina,
        "pre\x01post", "pre\\u0001post"));
    CREDO_VERUM (_effugium_aequale(piscina,
        "<script>", "\\u003Cscript>"));
    CREDO_VERUM (_effugium_aequale(piscina,
        "a\xE2\x80\xA8" "b", "a\\u2028b"));
    CREDO_VERUM (_effugium_aequale(piscina,
        "\xE2\x80\xA9", "\\u2029"));
    /* geminum: E2 80 93 (lineola longa) TRANSIT intacta */
    CREDO_VERUM (_effugium_aequale(piscina,
        "a\xE2\x80\x93" "b", "a\xE2\x80\x93" "b"));
    /* multibyte ceterum transit (pi = CF 80) */
    CREDO_VERUM (_effugium_aequale(piscina,
        "\xCF\x80", "\xCF\x80"));
    /* triplum truncatum ad finem: E2 80 sine tertio - transit */
    CREDO_VERUM (_effugium_aequale(piscina,
        "\xE2\x80", "\xE2\x80"));
    /* vacua -> vacua */
    CREDO_VERUM (_effugium_aequale(piscina, "", ""));

    piscina_destruere(pv);
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
