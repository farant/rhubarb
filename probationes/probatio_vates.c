/* probatio_vates.c - vates: corpus Anthropic (octeti exacti),
 * lectio responsorum, mittere (iterationes, rationarium, herbarium),
 * fictus, specimina commissa (vates-plan-2 T3-T6). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "http.h"
#include "filum.h"
#include "herbarium.h"
#include "vates.h"
#include "norma.h"
#include "norma_gignere.h"
#include "norma_stml.h"
#include "vates_responsum_norma.h"
#include "internamentum.h"
#include "stml.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>

interior chorda
_c (
    constans character* s,
               Piscina* piscina)
{
    redde chorda_ex_literis(s, piscina);
}

interior vacuum
probatio_corpus_minimum(Piscina* piscina)
{
    VatesPetitio* p;
    VatesNuntius* n;

    imprimere("\n--- Probans corpus minimum (octeti exacti) ---\n");
    p = vates_petitio_creare(piscina, _c("claude-opus-5-5", piscina));
    vates_systema_addere(p, _c("Sis brevis.", piscina), VERUM);
    n = vates_nuntium_addere(p, VATES_USOR);
    vates_textum_addere(n, _c("Salve", piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(vates_anthropic_corpus(p, piscina),
        "{\"model\":\"claude-opus-5-5\",\"max_tokens\":16000,"
        "\"system\":[{\"type\":\"text\",\"text\":\"Sis brevis.\","
        "\"cache_control\":{\"type\":\"ephemeral\"}}],"
        "\"messages\":[{\"role\":\"user\",\"content\":"
        "[{\"type\":\"text\",\"text\":\"Salve\"}]}]}");
}

interior vacuum
probatio_corpus_plenum(Piscina* piscina)
{
         VatesPetitio* p;
         VatesNuntius* n;
          VatesBlocus* b;
    VatesInstrumentum  t;
               chorda  primum;

    imprimere("\n--- Probans corpus plenum + determinismus + unicode ---\n");
    p = vates_petitio_creare(piscina, _c("claude-sonnet-5-5", piscina));
    vates_petitio_maxima_ponere(p, MM);
    vates_petitio_conatum_ponere(p, _c("high", piscina));
    vates_petitio_cogitationem_monstrare(p, VERUM);
    vates_petitio_caudam_signare(p, VERUM);
    t.titulus     = _c("zoom", piscina);
    t.descriptio  = _c("Aperit lineam.", piscina);
    t.schema = json_legere_literis("{\"type\":\"object\"}",
        piscina).radix;
    vates_instrumentum_addere(p, &t);
    n = vates_nuntium_addere(p, VATES_USOR);
    b = vates_textum_addere(n,
        _c("Ave, c\xc4\x81rissime \xe2\x9c\x9d \xf0\x9d\x94\x99",
        piscina));
    b->signum_thesauri  = VERUM;
    n                   = vates_nuntium_addere(p, VATES_USOR);
    vates_effectum_addere(n, _c("toolu_1", piscina), _c("nihil",
        piscina), VERUM);
    vates_extra_ponere(p, "anthropic", "fallbacks",
                       json_chorda_creare_literis(piscina, "default"));
    vates_extra_ponere(p, "openai", "store",
                       json_boolean_creare(piscina, FALSUM));
    primum = vates_anthropic_corpus(p, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(primum,
        "{\"model\":\"claude-sonnet-5-5\",\"max_tokens\":2000,"
        "\"thinking\":{\"type\":\"adaptive\",\"display\":\"summarized\"},"
        "\"output_config\":{\"effort\":\"high\"},"
        "\"cache_control\":{\"type\":\"ephemeral\"},"
        "\"tools\":[{\"name\":\"zoom\",\"description\":\"Aperit lineam.\","
        "\"input_schema\":{\"type\":\"object\"}}],"
        "\"messages\":[{\"role\":\"user\",\"content\":[{\"type\":\"text\","
        "\"text\":\"Ave, c\xc4\x81rissime \xe2\x9c\x9d \xf0\x9d\x94\x99\","
        "\"cache_control\":{\"type\":\"ephemeral\"}}]},"
        "{\"role\":\"user\",\"content\":[{\"type\":\"tool_result\","
        "\"tool_use_id\":\"toolu_1\",\"content\":\"nihil\",\"is_error\":true}]}],"
        "\"fallbacks\":\"default\"}");
    /* idem corpus bis -> octeti idem */
    CREDO_CHORDA_AEQUALIS(primum, vates_anthropic_corpus(p, piscina));
}

interior vacuum
probatio_opaca(Piscina* piscina)
{
    VatesPetitio* p;
    VatesNuntius* n;
     VatesBlocus* b;
          chorda  corpus;

    imprimere("\n--- Probans OPACUM: verbatim, alienum omissum ---\n");
    p = vates_petitio_creare(piscina, _c("claude-opus-5-5", piscina));
    n = vates_nuntium_addere(p, VATES_ASSISTENS);
    b = vates_textum_addere(n, _c("", piscina));
    b->genus = VATES_OPACUM;
    b->crudum = json_legere_literis(
        "{\"type\":\"thinking\",\"thinking\":\"\",\"signature\":\"sig123\"}",
        piscina).radix;
    b->provisor         = _c("anthropic", piscina);
    b->signum_thesauri  = VERUM;              /* in OPACO neglegitur */
    /* nuntius cuius soli bloci aliena OPACA sunt -> omittitur */
    n         = vates_nuntium_addere(p, VATES_ASSISTENS);
    b         = vates_textum_addere(n, _c("", piscina));
    b->genus  = VATES_OPACUM;
    b->crudum =
        json_legere_literis("{\"type\":\"reasoning\",\"encrypted\":\"x\"}",
                                    piscina).radix;
    b->provisor  = _c("openai", piscina);
    n            = vates_nuntium_addere(p, VATES_USOR);
    vates_textum_addere(n, _c("Et nunc?", piscina));
    corpus = vates_anthropic_corpus(p, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(corpus,
        "{\"model\":\"claude-opus-5-5\",\"max_tokens\":16000,"
        "\"messages\":[{\"role\":\"assistant\",\"content\":[{\"type\":\"thinking\","
        "\"thinking\":\"\",\"signature\":\"sig123\"}]},"
        "{\"role\":\"user\",\"content\":[{\"type\":\"text\",\"text\":\"Et nunc?\"}]}]}");
}

interior VatesBlocus*
_blocus (
    VatesResponsum* r,
               i32  i)
{
    redde (VatesBlocus*)xar_obtinere(r->bloci, i);
}

interior vacuum
probatio_legere(Piscina* piscina)
{
    VatesResponsum* r;
      VatesPetitio* p;
      VatesNuntius* n;

    imprimere("\n--- Probans vates_anthropic_legere ---\n");

    /* textus + usus cum cache_creation */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"msg_1\",\"type\":\"message\",\"role\":\"assistant\","
        "\"model\":\"claude-opus-5-5\",\"content\":[{\"type\":\"text\",\"text\":\"Salve!\"}],"
        "\"stop_reason\":\"end_turn\",\"stop_sequence\":null,\"usage\":{\"input_tokens\":12,"
        "\"cache_read_input_tokens\":3000,\"cache_creation_input_tokens\":400,"
        "\"cache_creation\":{\"ephemeral_5m_input_tokens\":300,\"ephemeral_1h_input_tokens\":100},"
        "\"output_tokens\":5}}", piscina), piscina);
    CREDO_VERUM(r->successus);
    CREDO_VERUM(r->causa_finis == VATES_FINIS);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->id, "msg_1");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->exemplar, "claude-opus-5-5");
    CREDO_AEQUALIS_I32(xar_numerus(r->bloci), I);
    CREDO_VERUM(_blocus(r, 0)->genus == VATES_TEXTUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(_blocus(r, 0)->textus, "Salve!");
    CREDO_AEQUALIS_S64(r->usus.input, 12);
    CREDO_AEQUALIS_S64(r->usus.cache_lectum, 3000);
    CREDO_AEQUALIS_S64(r->usus.cache_scriptum_5m, 300);
    CREDO_AEQUALIS_S64(r->usus.cache_scriptum_1h, 100);
    CREDO_AEQUALIS_S64(r->usus.output, 5);

    /* usus sine cache_creation: totum in 5m */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[],\"stop_reason\":\"end_turn\",\"stop_sequence\":null,"
        "\"usage\":{\"input_tokens\":1,\"cache_creation_input_tokens\":77,\"output_tokens\":1}}",
        piscina), piscina);
    CREDO_AEQUALIS_S64(r->usus.cache_scriptum_5m, 77);

    /* cogitatio + tool_use: OPACUM et PETITUM; reditus verbatim */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m2\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[{\"type\":\"thinking\",\"thinking\":\"\",\"signature\":\"SIG\"},"
        "{\"type\":\"tool_use\",\"id\":\"toolu_9\",\"name\":\"zoom\",\"input\":{\"id\":0,\"n\":8}}],"
        "\"stop_reason\":\"tool_use\",\"stop_sequence\":null,\"usage\":{\"input_tokens\":1,\"output_tokens\":1}}",
        piscina), piscina);
    CREDO_VERUM(r->causa_finis == VATES_FINIS_INSTRUMENTUM);
    CREDO_VERUM(_blocus(r, 0)->genus == VATES_OPACUM);
    CREDO_VERUM(_blocus(r, I)->genus == VATES_INSTRUMENTUM_PETITUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_blocus(r, I)->id, "toolu_9");
    CREDO_CHORDA_AEQUALIS_LITERIS(_blocus(r, I)->titulus, "zoom");
    CREDO_AEQUALIS_S64(json_capere_integer(_blocus(r, I)->input, "n",
        0), 8);
    p = vates_petitio_creare(piscina, _c("x", piscina));
    vates_responsum_addere(p, r);
    n = vates_nuntium_addere(p, VATES_USOR);
    vates_effectum_addere(n, _c("toolu_9", piscina), _c("linea",
        piscina), FALSUM);
    CREDO_CHORDA_CONTINET(vates_anthropic_corpus(p, piscina), _c(
        "{\"role\":\"assistant\",\"content\":[{\"type\":\"thinking\",\"thinking\":\"\","
        "\"signature\":\"SIG\"},{\"type\":\"tool_use\",\"id\":\"toolu_9\",\"name\":\"zoom\","
        "\"input\":{\"id\":0,\"n\":8}}]}", piscina));

    /* citationes: TEXTUS cum crudo - redditur cum citationibus */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m3\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[{\"type\":\"text\",\"text\":\"t\",\"citations\":[{\"x\":1}]}],"
        "\"stop_reason\":\"end_turn\",\"stop_sequence\":null,\"usage\":{\"input_tokens\":1,\"output_tokens\":1}}",
        piscina), piscina);
    CREDO_VERUM(_blocus(r, 0)->genus == VATES_TEXTUS);
    p = vates_petitio_creare(piscina, _c("x", piscina));
    vates_responsum_addere(p, r);
    CREDO_CHORDA_CONTINET(vates_anthropic_corpus(p, piscina),
                          _c("\"citations\":[{\"x\":1}]", piscina));

    /* recusatio cum categoria */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m4\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[],\"stop_reason\":\"refusal\",\"stop_sequence\":null,"
        "\"stop_details\":{\"type\":\"refusal\",\"category\":\"cyber\",\"explanation\":\"e\"},"
        "\"usage\":{\"input_tokens\":1,\"output_tokens\":0}}", piscina),
        piscina);
    CREDO_VERUM(r->successus);
    CREDO_VERUM(r->causa_finis == VATES_FINIS_RECUSATIO);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->recusatio_categoria, "cyber");

    /* max_tokens */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m5\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[{\"type\":\"text\",\"text\":\"trunc\"}],\"stop_reason\":\"max_tokens\","
        "\"stop_sequence\":null,\"usage\":{\"input_tokens\":1,\"output_tokens\":9}}",
        piscina), piscina);
    CREDO_VERUM(r->causa_finis == VATES_FINIS_MAXIMUM);

    /* errores provisoris */
    r = vates_anthropic_legere(DXXIX, _c(
        "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\",\"message\":\"Overloaded\"},"
        "\"request_id\":\"req_1\"}", piscina), piscina);
    CREDO_FALSUM(r->successus);
    CREDO_VERUM(r->error == VATES_ERROR_STATUS);
    CREDO_AEQUALIS_I32(r->status_http, DXXIX);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->error_genus, "overloaded_error");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->error_nuntius, "Overloaded");
    r = vates_anthropic_legere(DII, _c("<html>bad gateway</html>",
        piscina), piscina);
    CREDO_VERUM(r->error == VATES_ERROR_STATUS);
    CREDO_CHORDA_CONTINET(r->error_nuntius, _c("bad gateway", piscina));

    /* CC sed non legibile */
    r = vates_anthropic_legere(CC, _c("{\"id\":", piscina), piscina);
    CREDO_FALSUM(r->successus);
    CREDO_VERUM(r->error == VATES_ERROR_PARSE);

    /* blocus ignotus -> OPACUM, non abicitur */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m6\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[{\"type\":\"server_tool_use\",\"id\":\"s1\"},{\"type\":\"text\",\"text\":\"ok\"}],"
        "\"stop_reason\":\"end_turn\",\"stop_sequence\":null,\"usage\":{\"input_tokens\":1,\"output_tokens\":1}}",
        piscina), piscina);
    CREDO_AEQUALIS_I32(xar_numerus(r->bloci), II);
    CREDO_VERUM(_blocus(r, 0)->genus == VATES_OPACUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_blocus(r, 0)->provisor, "anthropic");
}


/* ---- vectura scripta pro provisore Anthropic: capita capit ---- */
nomen structura {
                   i32  status;          /* 0 = HTTP_ERROR_TIMEOUT */
    constans character* corpus;
    constans character* retry_after; /* NIHIL = sine */
} Scriptum;

nomen structura {
    constans Scriptum* scripta;
                  i32  numerus;
                  i32  index;
            character  capita_visa[MMMCMXCIX];
} ScriptorVatis;

interior HttpResultus
_scriptor_exsequi (
    HttpPetitio* petitio,
        Piscina* piscina,
         vacuum* datum)
{
        ScriptorVatis* s      = (ScriptorVatis*)datum;
     HttpPetitioVisus  visus  = http_petitio_visus(petitio);
         HttpResultus  res;
        HttpResponsum* resp;
    constans Scriptum* sc;
                  i32  i;
               size_t  longitudo = 0;

    s->capita_visa[0] = '\0';
    per (i = 0; i < visus.capita_numerus; i++)
    {
        longitudo = strlen(s->capita_visa);
        sprintf(s->capita_visa + longitudo, "%.*s: %.*s\n",
                (integer)visus.capita[i].titulus.mensura,
                (constans character*)visus.capita[i].titulus.datum,
                (integer)visus.capita[i].valor.mensura,
                (constans character*)visus.capita[i].valor.datum);
    }
    memset(&res, 0, magnitudo(res));
    si (s->index >= s->numerus)
    {
        res.error = HTTP_ERROR_CONNEXIO;
        redde res;
    }
    sc = &s->scripta[s->index];
    s->index++;
    si (sc->status == 0)
    {
        res.error = HTTP_ERROR_TIMEOUT;
        res.error_descriptio = chorda_ex_literis("tempus scriptum",
            piscina);
        redde res;
    }
    resp = (HttpResponsum*)piscina_allocare(piscina,
        (i64)magnitudo(HttpResponsum));
    memset(resp, 0, magnitudo(*resp));
    resp->status = sc->status;
    resp->corpus = chorda_ex_literis(sc->corpus, piscina);
    resp->capita = (HttpCaput*)piscina_allocare(piscina,
        (i64)magnitudo(HttpCaput));
    si (sc->retry_after)
    {
        resp->capita[0].titulus = chorda_ex_literis("retry-after",
            piscina);
        resp->capita[0].valor = chorda_ex_literis(sc->retry_after,
            piscina);
        resp->capita_numerus = I;
    }
    res.successus = VERUM;
    res.responsum = resp;
    redde res;
}

hic_manens constans character* _textus_bonus =
    "{\"id\":\"msg_b\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"m\","
    "\"content\":[{\"type\":\"text\",\"text\":\"bene\"}],\"stop_reason\":\"end_turn\","
    "\"stop_sequence\":null,\"usage\":{\"input_tokens\":10,\"output_tokens\":2}}";
hic_manens constans character* _onustus =
    "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\",\"message\":\"Overloaded\"}}";

interior VatesPetitio*
_petitio_simplex (
    Piscina* piscina)
{
    VatesPetitio* p = vates_petitio_creare(piscina, _c("m", piscina));

    vates_textum_addere(vates_nuntium_addere(p, VATES_USOR), _c("salve",
        piscina));
    redde p;
}

interior i32
_lineae (
    constans character* via,
               Piscina* piscina)
{
    chorda t = filum_legere_totum(via, piscina);
       i32 i;
       i32 n = 0;

    per (i = 0; i < t.mensura; i++)
    {
        si (t.datum[i] == '\n')
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
probatio_provisores(Piscina* piscina)
{
    VatesOptiones  o = vates_optiones_ordinariae();
            Vates* v;

    imprimere("\n--- Probans vates_aperire per nomen ---\n");
    v = vates_aperire(piscina, _c("anthropic", piscina), _c("k",
        piscina), &o);
    CREDO_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(vates_provisor(v), "anthropic");
    v = vates_aperire(piscina, _c("fictus", piscina), _c("", piscina),
        &o);
    CREDO_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(vates_provisor(v), "fictus");
    CREDO_NIHIL(vates_aperire(piscina, _c("openai", piscina), _c("k",
        piscina), &o));
}

interior vacuum
probatio_fictus_et_rationarium(Piscina* piscina)
{
     VatesOptiones  o = vates_optiones_ordinariae();
      VatesPretium  pretium;
         VatesUsus  u;
             Vates* v;
      VatesPetitio* p;
    VatesResponsum* r;
         character  via[CCLVI];
            chorda  linea;

    imprimere("\n--- Probans fictus + pretium + rationarium ---\n");
    sprintf(via, "/tmp/probatio_vates_rationarium_%ld.jsonl",
        (longus)getpid());
    (vacuum)unlink(via);
    pretium.exemplar      = "fictus";
    pretium.input         = IV * M * M;      /* $4 / MTok */
    pretium.output        = XX * M * M;
    pretium.cache_lectum  = CC * M;
    pretium.cache_5m      = V * M * M;
    pretium.cache_1h      = VIII * M * M;
    o.pretia              = &pretium;
    o.pretia_numerus      = I;
    o.rationarium_via     = via;
    v                     = vates_fictus_aperire(piscina, &o);
    memset(&u, 0, magnitudo(u));
    u.input              = M;
    u.cache_lectum       = MM;
    u.cache_scriptum_5m  = CCC;
    u.output             = L;
    vates_fictus_textum(v, _c("responsum fictum", piscina), u);
    p = _petitio_simplex(piscina);
    r = vates_mittere(v, p, _c("probatio", piscina), piscina);
    CREDO_VERUM(r->successus);
    CREDO_AEQUALIS_I32(r->conatus, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(((VatesBlocus*)xar_obtinere(r->bloci,
        0))->textus,
                                  "responsum fictum");
    /* (1000*4e6 + 2000*2e5 + 300*5e6 + 50*2e7) / 1e6 = 6900 */
    CREDO_AEQUALIS_S64(r->usus.pretium, 6900);
    CREDO_AEQUALIS_I32(_lineae(via, piscina), I);
    linea = filum_legere_totum(via, piscina);
    CREDO_CHORDA_CONTINET(linea, _c("\"propositum\":\"probatio\"",
        piscina));
    CREDO_CHORDA_CONTINET(linea, _c("\"pretium\":6900", piscina));
    /* fictus: corpus missum = corpus purum */
    CREDO_AEQUALIS_I32(vates_fictus_petitiones_numerus(v), I);
    CREDO_CHORDA_AEQUALIS(vates_fictus_petitio(v, 0),
        vates_anthropic_corpus(p, piscina));
    (vacuum)unlink(via);
}

interior vacuum
probatio_iterationes(Piscina* piscina)
{
     VatesOptiones  o = vates_optiones_ordinariae();
         VatesUsus  u;
             Vates* v;
    VatesResponsum* r;
         character  via[CCLVI];

    imprimere("\n--- Probans iterationes ---\n");
    sprintf(via, "/tmp/probatio_vates_iter_%ld.jsonl",
        (longus)getpid());
    (vacuum)unlink(via);
    o.mora_iterandi_ms  = I;
    o.rationarium_via   = via;
    memset(&u, 0, magnitudo(u));
    u.input   = X;
    u.output  = II;

    /* 429 deinde bene: conatus II, lineae II, usus summatus */
    v = vates_fictus_aperire(piscina, &o);
    vates_fictus_crudum(v, CDXXIX,
        "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"x\"}}");
    vates_fictus_textum(v, _c("bene", piscina), u);
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina),
        piscina);
    CREDO_VERUM(r->successus);
    CREDO_AEQUALIS_I32(r->conatus, II);
    CREDO_AEQUALIS_I32(_lineae(via, piscina), II);
    CREDO_AEQUALIS_S64(r->usus.input, X);

    /* 400: non iteratur */
    v = vates_fictus_aperire(piscina, &o);
    vates_fictus_crudum(v, CD,
        "{\"type\":\"error\",\"error\":{\"type\":\"invalid_request_error\",\"message\":\"y\"}}");
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina),
        piscina);
    CREDO_FALSUM(r->successus);
    CREDO_AEQUALIS_I32(r->conatus, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->error_genus,
        "invalid_request_error");

    /* 529 ter: conatus III, STATUS */
    v = vates_fictus_aperire(piscina, &o);
    vates_fictus_crudum(v, DXXIX, _onustus);
    vates_fictus_crudum(v, DXXIX, _onustus);
    vates_fictus_crudum(v, DXXIX, _onustus);
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina),
        piscina);
    CREDO_VERUM(r->error == VATES_ERROR_STATUS);
    CREDO_AEQUALIS_I32(r->conatus, III);

    /* fictus exhaustus: RETE, iteratur usque ad III */
    v = vates_fictus_aperire(piscina, &o);
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina),
        piscina);
    CREDO_VERUM(r->error == VATES_ERROR_RETE);
    CREDO_AEQUALIS_I32(r->conatus, III);
    (vacuum)unlink(via);
}

interior vacuum
probatio_limes(Piscina* piscina)
{
     VatesOptiones  o = vates_optiones_ordinariae();
             Vates* v = vates_fictus_aperire(piscina, &o);
      VatesPetitio* p = vates_petitio_creare(piscina, _c("m",
          piscina));
      VatesNuntius* n = vates_nuntium_addere(p, VATES_USOR);
    VatesResponsum* r;
               i32  i;

    imprimere("\n--- Probans limes punctorum cache ---\n");
    vates_petitio_caudam_signare(p, VERUM);
    per (i = 0; i < IV; i++)
    {
        vates_textum_addere(n, _c("x", piscina))->signum_thesauri =
            VERUM;
    }
    r = vates_mittere(v, p, _c("t", piscina), piscina);
    CREDO_VERUM(r->error == VATES_ERROR_LIMES);
    CREDO_AEQUALIS_I32(vates_fictus_petitiones_numerus(v), 0);
}

interior vacuum
probatio_tempus_et_mora_iterandi(Piscina* piscina)
{
     VatesOptiones  o = vates_optiones_ordinariae();
     ScriptorVatis  s;
          Scriptum  scripta[II];
             Vates* v;
    VatesResponsum* r;
            time_t  initium;

    imprimere("\n--- Probans retry-after hostile + TEMPUS semel ---\n");
    scripta[0].status = CDXXIX;
    scripta[0].corpus =
        "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"z\"}}";
    scripta[0].retry_after  = "3600";
    scripta[I].status       = CC;
    scripta[I].corpus       = _textus_bonus;
    scripta[I].retry_after  = NIHIL;
    memset(&s, 0, magnitudo(s));
    s.scripta = scripta;
    s.numerus = II;
    o.mora_iterandi_maxima_ms = V;
    o.vectura.exsequi = _scriptor_exsequi;
    o.vectura.datum = &s;
    v = vates_anthropic_aperire(piscina, _c("k", piscina), &o);
    initium = time(NIHIL);
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina),
        piscina);
    CREDO_VERUM(r->successus);
    CREDO_VERUM(time(NIHIL) - initium < II);

    /* TEMPUS bis: iteratur semel tantum */
    scripta[0].status = 0;
    scripta[I].status = 0;
    s.index = 0;
    v = vates_anthropic_aperire(piscina, _c("k", piscina), &o);
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina),
        piscina);
    CREDO_VERUM(r->error == VATES_ERROR_TEMPUS);
    CREDO_AEQUALIS_I32(r->conatus, II);
}

interior vacuum
probatio_clavis_non_effunditur(Piscina* piscina)
{
     VatesOptiones  o = vates_optiones_ordinariae();
     ScriptorVatis  s;
          Scriptum  scripta[II];
             Vates* v;
      VatesPetitio* p;
    VatesResponsum* r;
         character  via_rationarii[CCLVI];
         character  via_herbarii[CCLVI];
         character  via[DXII];
            chorda  clavis = _c("CLAVIS_SECRETA_PROBATIONIS",
                piscina);

    imprimere("\n--- Probans clavis numquam effunditur ---\n");
    sprintf(via_rationarii, "/tmp/probatio_vates_clavis_%ld.jsonl",
        (longus)getpid());
    sprintf(via_herbarii, "/tmp/probatio_vates_herb_%ld",
        (longus)getpid());
    (vacuum)unlink(via_rationarii);
    scripta[0].status       = DXXIX;
    scripta[0].corpus       = _onustus;
    scripta[0].retry_after  = NIHIL;
    scripta[I].status       = CC;
    scripta[I].corpus       = _textus_bonus;
    scripta[I].retry_after  = NIHIL;
    memset(&s, 0, magnitudo(s));
    s.scripta           = scripta;
    s.numerus           = II;
    o.mora_iterandi_ms  = I;
    o.rationarium_via   = via_rationarii;
    o.herbarium_via     = via_herbarii;
    o.vectura.exsequi   = _scriptor_exsequi;
    o.vectura.datum     = &s;
    v                   = vates_anthropic_aperire(piscina, clavis, &o);
    p                   = _petitio_simplex(piscina);
    vates_caput_addere(p, "anthropic", "anthropic-beta",
        "beta-probationis");
    vates_caput_addere(p, "openai", "openai-beta", "alienum");
    r = vates_mittere(v, p, _c("t", piscina), piscina);
    CREDO_VERUM(r->successus);
    /* clavis in capite petitionis - et solum ibi */
    CREDO_VERUM(strstr(s.capita_visa,
        "x-api-key: CLAVIS_SECRETA_PROBATIONIS") != NIHIL);
    CREDO_VERUM(strstr(s.capita_visa,
        "anthropic-beta: beta-probationis") != NIHIL);
    CREDO_VERUM(strstr(s.capita_visa, "openai-beta") == NIHIL);
    CREDO_FALSUM(chorda_continet(filum_legere_totum(via_rationarii,
        piscina), clavis));
    sprintf(via, "%s/index.jsonl", via_herbarii);
    CREDO_FALSUM(chorda_continet(filum_legere_totum(via, piscina),
        clavis));
    {
        Xar* sp = herbarium_enumerare(piscina, via_herbarii);
        i32  i;

        CREDO_AEQUALIS_I32(xar_numerus(sp), I);
        per (i = 0; i < xar_numerus(sp); i++)
        {
            HerbariumSpecimen* h = (HerbariumSpecimen*)xar_obtinere(sp,
                i);

            CREDO_FALSUM(chorda_continet(h->corpus, clavis));
            CREDO_CHORDA_INCIPIT(h->clavis, _c("529:overloaded_error:",
                piscina));
            sprintf(via, "%s/specimina/%.*s-%u.json", via_herbarii,
                    (integer)h->sigillum.mensura,
                    (constans character*)h->sigillum.datum,
                    h->variantes_index);
            CREDO_FALSUM(chorda_continet(filum_legere_totum(via,
                piscina), clavis));
            (vacuum)unlink(via);
        }
    }
    sprintf(via, "%s/index.jsonl", via_herbarii);
    (vacuum)unlink(via);
    sprintf(via, "%s/specimina", via_herbarii);
    (vacuum)rmdir(via);
    (vacuum)rmdir(via_herbarii);
    (vacuum)unlink(via_rationarii);
}

interior vacuum
probatio_novitas_pressa(Piscina* piscina)
{
     VatesOptiones  o = vates_optiones_ordinariae();
             Vates* v;
    VatesResponsum* r;
         character  via_herbarii[CCLVI];
         character  via[DXII];
               Xar* sp;
               i32  i;
               b32  inventa = FALSUM;

    imprimere("\n--- Probans novitas in herbarium premitur ---\n");
    sprintf(via_herbarii, "/tmp/probatio_vates_novitas_%ld",
        (longus)getpid());
    o.herbarium_via  = via_herbarii;
    v                = vates_fictus_aperire(piscina, &o);
    vates_fictus_crudum(v, CC,
        "{\"id\":\"m\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"m\","
        "\"content\":[{\"type\":\"server_tool_use\",\"id\":\"s\"}],\"stop_reason\":\"end_turn\","
        "\"stop_sequence\":null,\"usage\":{\"input_tokens\":1,\"output_tokens\":1}}");
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina),
        piscina);
    CREDO_VERUM(r->successus);
    sp = herbarium_enumerare(piscina, via_herbarii);
    per (i = 0; i < xar_numerus(sp); i++)
    {
        HerbariumSpecimen* h = (HerbariumSpecimen*)xar_obtinere(sp, i);

        si (chorda_aequalis_literis(h->causa,
            "blocus ignotus: server_tool_use"))
        {
            inventa = VERUM;
        }
        sprintf(via, "%s/specimina/%.*s-%u.json", via_herbarii,
            (integer)h->sigillum.mensura,
                (constans character*)h->sigillum.datum,
                h->variantes_index);
        (vacuum)unlink(via);
    }
    CREDO_VERUM(inventa);
    sprintf(via, "%s/index.jsonl", via_herbarii);
    (vacuum)unlink(via);
    sprintf(via, "%s/specimina", via_herbarii);
    (vacuum)rmdir(via);
    (vacuum)rmdir(via_herbarii);
}


interior vacuum
probatio_specimina_commissa(Piscina* piscina)
{
    Xar* s = herbarium_enumerare(piscina,
        "probationes/fixa/vates/herbarium");
    HttpVectura reddens;
            i32 i;

    imprimere("\n--- Probans specimina commissa (exitus NOMINATUS) ---\n");
    /* praesentia > 0, non numerus fixus: semina crescunt */
    CREDO_MAIOR_I32(xar_numerus(s), 0);
    reddens = herbarium_reddens(piscina, s);
    per (i = 0; i < xar_numerus(s); i++)
    {
        HerbariumSpecimen* sp = (HerbariumSpecimen*)xar_obtinere(s, i);
             HttpResultus  resultus_http = http_vectura_exsequi(reddens,
                 NIHIL,
                 piscina);
           VatesResponsum* r =
               vates_anthropic_legere(resultus_http.responsum->status,
                                                      resultus_http.responsum->corpus,
                                                      piscina);

        imprimere("  %.*s\n", (integer)sp->sigillum.mensura,
                  (constans character*)sp->sigillum.datum);
        si (sp->status != CC)
        {
            CREDO_VERUM(r->error == VATES_ERROR_STATUS);
            CREDO_CHORDA_NON_VACUA(r->error_genus);
        }
        alioquin
        {
            CREDO_VERUM(r->successus);
            CREDO_VERUM(r->causa_finis != VATES_FINIS_ALIA);
        }
    }
}


interior vacuum
probatio_forma_viva(Piscina* piscina)
{
     VatesOptiones  o = vates_optiones_ordinariae();
             Vates* v;
    VatesResponsum* r;
         character  via_herbarii[CCLVI];
         character  via[DXII];

    /* corpus VERUM ex fumo vivo T7 (2026-10-08): 'container' et
     * 'diagnostics' in summo noti esse debent - nulla novitas */
    imprimere("\n--- Probans forma viva API: nulla novitas ---\n");
    sprintf(via_herbarii, "/tmp/probatio_vates_forma_%ld",
        (longus)getpid());
    o.herbarium_via  = via_herbarii;
    v                = vates_fictus_aperire(piscina, &o);
    vates_fictus_crudum(v, CC,
        "{\"model\":\"claude-sonnet-5-5\",\"id\":\"msg_011CfpvgCh25e34mA"
        "ahvG2wb\",\"type\":\"message\",\"role\":\"assistant\",\"content\":["
        "{\"type\":\"text\",\"text\":\"Salve.\"}],\"container\":null,\"stop_"
        "reason\":\"end_turn\",\"stop_sequence\":null,\"stop_details\":n"
        "ull,\"usage\":{\"input_tokens\":18,\"cache_creation_input_tok"
        "ens\":1839,\"cache_read_input_tokens\":0,\"cache_creation\":{"
        "\"ephemeral_5m_input_tokens\":1839,\"ephemeral_1h_input_tok"
        "ens\":0},\"output_tokens\":6,\"output_tokens_details\":{\"thin"
        "king_tokens\":0},\"service_tier\":\"standard\",\"inference_geo"
        "\":\"global\"},\"diagnostics\":null}");
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina),
        piscina);
    CREDO_VERUM(r->successus);
    CREDO_AEQUALIS_I32(xar_numerus(herbarium_enumerare(piscina,
        via_herbarii)), 0);
    sprintf(via, "%s/index.jsonl", via_herbarii);
    (vacuum)unlink(via);
    sprintf(via, "%s/specimina", via_herbarii);
    (vacuum)rmdir(via);
    (vacuum)rmdir(via_herbarii);
}


/* norma-plan-3 A6: aedificator VETUS (copia verbatim lib/vates.c ante
 * migrationem) - oraculum solum: schema ex .norma idem dicere debet */
interior Norma*
_forma_responsi_vetus (
    Piscina* p)
{
    Norma* textus     = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* petitum    = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* cogitatio  = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* redacta    = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* blocus = norma_modus(norma_discrimen(p, "type"),
        NORMA_NOTANDUM);
    Norma* cc    = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* usus  = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* r     = norma_modus(norma_objectum(p), NORMA_NOTANDUM);

    norma_campus(textus, "text", norma_textus(p), VERUM);
    norma_campus(textus, "citations", norma_liberum(p), FALSUM);
    norma_campus(petitum, "id", norma_textus(p), VERUM);
    norma_campus(petitum, "name", norma_textus(p), VERUM);
    norma_campus(petitum, "input", norma_liberum(p), VERUM);
    norma_campus(cogitatio, "thinking", norma_textus(p), VERUM);
    norma_campus(cogitatio, "signature", norma_textus(p), VERUM);
    norma_campus(redacta, "data", norma_textus(p), VERUM);
    norma_variatio(blocus, "text", textus);
    norma_variatio(blocus, "tool_use", petitum);
    norma_variatio(blocus, "thinking", cogitatio);
    norma_variatio(blocus, "redacted_thinking", redacta);
    norma_campus(cc, "ephemeral_5m_input_tokens", norma_integer(p),
        FALSUM);
    norma_campus(cc, "ephemeral_1h_input_tokens", norma_integer(p),
        FALSUM);
    norma_campus(usus, "input_tokens", norma_integer(p), VERUM);
    norma_campus(usus, "output_tokens", norma_integer(p), VERUM);
    norma_campus(usus, "cache_read_input_tokens", norma_integer(p),
        FALSUM);
    norma_campus(usus, "cache_creation_input_tokens", norma_integer(p),
        FALSUM);
    norma_campus(usus, "cache_creation", cc, FALSUM);
    norma_campus(usus, "output_tokens_details", norma_liberum(p),
        FALSUM);
    norma_campus(usus, "service_tier",
        norma_aut_nullum(norma_textus(p)), FALSUM);
    norma_campus(usus, "inference_geo",
        norma_aut_nullum(norma_textus(p)), FALSUM);
    norma_campus(r, "id", norma_textus(p), VERUM);
    norma_campus(r, "type", norma_textus(p), VERUM);
    norma_campus(r, "role", norma_textus(p), VERUM);
    norma_campus(r, "model", norma_textus(p), VERUM);
    norma_campus(r, "content", norma_tabulatum(p, blocus), VERUM);
    norma_campus(r, "stop_reason", norma_aut_nullum(norma_textus(p)),
        VERUM);
    norma_campus(r, "stop_sequence", norma_aut_nullum(norma_textus(p)),
        FALSUM);
    norma_campus(r, "stop_details", norma_liberum(p), FALSUM);
    norma_campus(r, "usage", usus, VERUM);
    norma_campus(r, "container", norma_liberum(p), FALSUM);
    norma_campus(r, "diagnostics", norma_liberum(p), FALSUM);
    redde r;
}

interior b32
_iudicia_aequalia (
    NormaIudicium a,
    NormaIudicium b)
{
    i32 i;

    si (   a.validum            != b.validum
        || xar_numerus(a.vitia) != xar_numerus(b.vitia)
        || xar_numerus(a.notae) != xar_numerus(b.notae))
    {
        redde FALSUM;
    }
    per (i = 0; i < xar_numerus(a.vitia) + xar_numerus(a.notae); i++)
    {
                b32  vit  = i < xar_numerus(a.vitia);
                i32  k    = vit ? i : i - xar_numerus(a.vitia);
        NormaVitium* x =
            (NormaVitium*)xar_obtinere(vit ? a.vitia : a.notae, k);
        NormaVitium* y =
            (NormaVitium*)xar_obtinere(vit ? b.vitia : b.notae, k);

        si (x->causa != y->causa || !chorda_aequalis(x->via, y->via))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* norma-plan-3 A6: schema ex .norma generatum idem dicit ac aedificator
 * vetus (exportatio + iudicia super valores ab utroque genitos) */
interior vacuum
probatio_forma_ex_norma(Piscina* p)
{
    Norma* vetus  = _forma_responsi_vetus(p);
    Norma* nova   = vates_norma_responsum(p);
    i32 diversa   = 0;
    s64 semen;
    i32 m;
    i32 q;
    chorda fons = filum_legere_totum("lib/vates_responsum.norma", p);
    StmlResultus r = stml_legere(fons, p, internamentum_creare(p));

    imprimere("\n--- Probans formam responsi ex .norma ---\n");
    CREDO_CHORDA_AEQUALIS(json_scribere(norma_json_schema(vetus, p), p),
                          json_scribere(norma_json_schema(nova, p), p));
    per (q = 0; q < II; q++)
    {
        per (semen = 0; semen < L; semen++)
        {
            per (m = (i32)NORMA_TYPICA; m <= (i32)NORMA_INVALIDA; m++)
            {
                NormaGenitum g = norma_gignere(q == 0 ? vetus : nova,
                    (NormaModusGignendi)m, semen, p);

                si (g.valor && !_iudicia_aequalia(norma_iudicare(vetus,
                        g.valor, p), norma_iudicare(nova, g.valor, p)))
                {
                    diversa++;
                }
            }
        }
    }
    CREDO_AEQUALIS_I32(diversa, 0);
    /* oraculum V: plagula forma pulchra */
    CREDO_VERUM(r.successus);
    CREDO_CHORDA_AEQUALIS(stml_scribere(r.radix, p, VERUM), fons);
    /* lector eam sine vitio legit, sine notis (nullus gignens) */
    CREDO_VERUM(norma_stml_legere(fons, NIHIL, 0, p).successus);
}

/* ---- herbarium-spec-2 H2: custos ante usum ---- */

hic_manens character _sedes_temporaria[CCLVI];

/* specimina et index directorii <radix>/<hospes> delere */
interior vacuum
_acervum_purgare (
    constans character* radix,
    constans character* hospes,
               Piscina* piscina)
{
    character  via[DXII];
          Xar* sp;
          i32  i;

    sprintf(via, "%s/%s", radix, hospes);
    sp = herbarium_enumerare(piscina, via);
    per (i = 0; i < xar_numerus(sp); i++)
    {
         HerbariumSpecimen* h = (HerbariumSpecimen*)xar_obtinere(sp, i);
                 character  plena[DXII];

        sprintf(plena, "%s/specimina/%.*s-%u.json", via,
            (integer)h->sigillum.mensura,
            (constans character*)h->sigillum.datum, h->variantes_index);
        (vacuum)unlink(plena);
    }
    sprintf(via, "%s/%s/index.jsonl", radix, hospes);
    (vacuum)unlink(via);
    sprintf(via, "%s/%s/specimina", radix, hospes);
    (vacuum)rmdir(via);
    sprintf(via, "%s/%s", radix, hospes);
    (vacuum)rmdir(via);
}

/* causa inter specimina acervi */
interior b32
_causa_in_acervo (
    constans character* acervus,
    constans character* causa,
               Piscina* piscina)
{
    Xar* sp = herbarium_enumerare(piscina, acervus);
    i32  i;

    per (i = 0; i < xar_numerus(sp); i++)
    {
        si (chorda_continet(((HerbariumSpecimen*)xar_obtinere(sp,
                i))->causa, chorda_ex_literis(causa, piscina)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* casus unus: Vates fictus, sedes ORDINARIA (herbarium_via NIHIL),
 * conatus unus; reddit errorem vatis */
interior VatesError
_casus (
                   i32  status,
    constans character* corpus,
               Piscina* piscina)
{
     VatesOptiones  o = vates_optiones_ordinariae();
             Vates* v;
    VatesResponsum* r;

    o.conatus_maximi  = I;
    v                 = vates_fictus_aperire(piscina, &o);
    vates_fictus_crudum(v, status, corpus);
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina),
        piscina);
    redde r->error;
}

interior vacuum
probatio_custos_ante_usum(Piscina* piscina)
{
        character  acervus[DXII];
        character  nunquam[CCLVI];
    VatesOptiones  o = vates_optiones_ordinariae();
            Vates* v;

    imprimere("\n--- Probans custodem ante usum ---\n");
    sprintf(acervus, "%s/fictus", _sedes_temporaria);
    /* inexspectata: omnia in disco, errores vatis immutati */
    CREDO_VERUM(_casus(CC, "non est JSON", piscina)
        == VATES_ERROR_PARSE);
    CREDO_VERUM(_casus(CC, "[1,2]", piscina) == VATES_ERROR_PARSE);
    CREDO_VERUM(_casus(CCIV, "", piscina) == VATES_ERROR_STATUS);
    CREDO_VERUM(_casus(CCCII, "", piscina) == VATES_ERROR_STATUS);
    (vacuum)_casus(CC,
        "{\"id\":\"m\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"m\","
        "\"content\":[],\"stop_reason\":\"end_turn\",\"novum_campum\":1,"
        "\"usage\":{\"input_tokens\":1,\"output_tokens\":1}}", piscina);
    (vacuum)_casus(CC,
        "{\"id\":\"m\",\"type\":\"message\",\"role\":\"assistant\","
        "\"content\":[],\"stop_reason\":\"end_turn\","
        "\"usage\":{\"input_tokens\":1,\"output_tokens\":1}}", piscina);
    (vacuum)_casus(CC,
        "{\"id\":\"m\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"m\","
        "\"content\":[],\"stop_reason\":\"mysterium\","
        "\"usage\":{\"input_tokens\":1,\"output_tokens\":2}}", piscina);
    CREDO_VERUM(_causa_in_acervo(acervus, "corpus non JSON", piscina));
    CREDO_VERUM(_causa_in_acervo(acervus, "radix non objectum",
        piscina));
    CREDO_VERUM(_causa_in_acervo(acervus, "status inexspectatus: 204",
        piscina));
    CREDO_VERUM(_causa_in_acervo(acervus, "status inexspectatus: 302",
        piscina));
    CREDO_VERUM(_causa_in_acervo(acervus, "novum_campum", piscina));
    CREDO_VERUM(_causa_in_acervo(acervus, "model", piscina));
    CREDO_VERUM(_causa_in_acervo(acervus,
        "stop_reason ignota: mysterium",
        piscina));
    /* exspectatum: nihil premitur */
    _acervum_purgare(_sedes_temporaria, "fictus", piscina);
    CREDO_VERUM(_casus(CC, _textus_bonus, piscina) == VATES_OK);
    CREDO_AEQUALIS_I32(xar_numerus(herbarium_enumerare(piscina,
        acervus)),
        0);
    /* status >= CD: premitur ut olim ('status'), iudex non vocatur */
    (vacuum)_casus(CDXXIX,
        "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"x\"}}",
        piscina);
    CREDO_VERUM(_causa_in_acervo(acervus, "status", piscina));
    _acervum_purgare(_sedes_temporaria, "fictus", piscina);
    /* provisor Anthropic: acervus per hospitem URL (api.anthropic.com) */
    {
        VatesOptiones oa = vates_optiones_ordinariae();
        ScriptorVatis sv;
             Scriptum scriptum;
            character acervus_anthropic[DXII];

        scriptum.status       = CC;
        scriptum.corpus       = "<html>non est JSON</html>";
        scriptum.retry_after  = NIHIL;
        memset(&sv, 0, magnitudo(sv));
        sv.scripta = &scriptum;
        sv.numerus = I;
        oa.conatus_maximi = I;
        oa.vectura.exsequi = _scriptor_exsequi;
        oa.vectura.datum = &sv;
        v = vates_anthropic_aperire(piscina, _c("k", piscina), &oa);
        (vacuum)vates_mittere(v, _petitio_simplex(piscina), _c("t",
            piscina), piscina);
        sprintf(acervus_anthropic, "%s/api.anthropic.com",
            _sedes_temporaria);
        CREDO_VERUM(_causa_in_acervo(acervus_anthropic,
            "corpus non JSON",
            piscina));
    }
    /* sine_herbario: nihil, ne in sede ordinaria quidem */
    o.conatus_maximi  = I;
    o.sine_herbario   = VERUM;
    v                 = vates_fictus_aperire(piscina, &o);
    vates_fictus_crudum(v, CC, "non est JSON");
    (vacuum)vates_mittere(v, _petitio_simplex(piscina), _c("t",
        piscina),
        piscina);
    CREDO_AEQUALIS_I32(xar_numerus(herbarium_enumerare(piscina,
        acervus)),
        0);
    /* sedes ordinaria irrita ($RHUBARB_HERBARIUM non exstans): vates
     * sine captura pergit, vocatio non frangitur */
    sprintf(nunquam, "%s_nunquam", _sedes_temporaria);
    (vacuum)setenv("RHUBARB_HERBARIUM", nunquam, I);
    CREDO_VERUM(_casus(CC, _textus_bonus, piscina) == VATES_OK);
    (vacuum)setenv("RHUBARB_HERBARIUM", _sedes_temporaria, I);
}

s32
principale (vacuum)
{
     Piscina* piscina;
         b32  successus;

    piscina = piscina_generare_dynamicum("probatio_vates", M * M);
    credo_aperire(piscina);
    /* herbarium-spec-2: captura nunc ORDINARIA - probationes numquam in
     * ~/.rhubarb/herbarium veram scribunt: sedes temporaria per ambitum */
    sprintf(_sedes_temporaria, "/tmp/probatio_vates_sedes_%ld",
        (longus)getpid());
    (vacuum)mkdir(_sedes_temporaria, 0755);
    (vacuum)setenv("RHUBARB_HERBARIUM", _sedes_temporaria, I);

    /* T3 */
    probatio_corpus_minimum(piscina);
    probatio_corpus_plenum(piscina);
    probatio_opaca(piscina);
    /* T4 */
    probatio_legere(piscina);
    /* T5 */
    probatio_provisores(piscina);
    probatio_fictus_et_rationarium(piscina);
    probatio_iterationes(piscina);
    probatio_limes(piscina);
    probatio_tempus_et_mora_iterandi(piscina);
    probatio_clavis_non_effunditur(piscina);
    probatio_novitas_pressa(piscina);
    /* T6 */
    probatio_specimina_commissa(piscina);
    /* T7b */
    probatio_forma_viva(piscina);
    /* norma-plan-3 A6 */
    probatio_forma_ex_norma(piscina);
    /* herbarium-spec-2 H2 */
    probatio_custos_ante_usum(piscina);
    _acervum_purgare(_sedes_temporaria, "fictus", piscina);
    _acervum_purgare(_sedes_temporaria, "api.anthropic.com", piscina);
    (vacuum)rmdir(_sedes_temporaria);

    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde successus ? 0 : I;
}
