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
    /* T4, T5, T6 addunt hic vocationes suas */

    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde successus ? 0 : I;
}
