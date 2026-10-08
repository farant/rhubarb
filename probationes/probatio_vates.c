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

#include <stdio.h>
#include <string.h>
#include <unistd.h>

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


s32
principale (vacuum)
{
     Piscina* piscina;
         b32  successus;

    piscina = piscina_generare_dynamicum("probatio_vates", M * M);
    credo_aperire(piscina);

    /* T3 */
    probatio_corpus_minimum(piscina);
    probatio_corpus_plenum(piscina);
    probatio_opaca(piscina);
    /* T4 */
    probatio_legere(piscina);
    /* T5, T6 addunt hic vocationes suas */

    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde successus ? 0 : I;
}
