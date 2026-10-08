/* vates_fumus.c - fumus vivus vatis (vates-plan-2 T7): vocatio una,
 * cache lectum in vocatione secunda eiusdem praefixi, reditus
 * cogitationis cum signatura.
 *
 * NON PORTA: API vera, centesimae pecuniae - manu curritur.
 *
 * Curre per: ./tools/vates_fumus.sh [-exemplar ID]
 * (ordinarium claude-sonnet-5-5). Exitus 0 = omnes exspectationes.
 *
 * Clavis: ~/.rhubarb/anthropic.clavis - in processu legitur, Vati
 * traditur (copia UNA ibi), deinde buffera nostra zephyrantur.
 * Rationarium: build/vates_fumus.rationarium.jsonl; herbarium:
 * build/vates_fumus_herbarium. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "json.h"
#include "vates.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RATIONARIUM "build/vates_fumus.rationarium.jsonl"
#define HERBARIUM   "build/vates_fumus_herbarium"

hic_manens i32 _fracti = 0;

interior vacuum
_nuntiare (
    constans character* titulus,
                   b32  bene,
    constans character* nota)
{
    imprimere("%s  %-46s %s\n", bene ? "BENE " : "FRACT", titulus,
              nota ? nota : "");
    si (!bene)
    {
        _fracti++;
    }
}

/* Pretia (micro-dollaria per MTok), tabula skill 2026-10-06; scriptura
 * cache 1.25x (5m) / 2x (1h) inputi. */
hic_manens constans VatesPretium _pretia[] = {
    { "claude-sonnet-5-5", 2000000, 10000000, 200000, 2500000,
        4000000 },
    { "claude-opus-5-5",   4000000, 20000000, 200000, 5000000,
        8000000 },
    { "claude-haiku-5-5",   100000,   500000,  10000,  125000,  200000 }
};

interior chorda
_systema_longum (
    Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, IV * M);
            character  numerus[XXXII];
                  i32  i;

    chorda_aedificator_appendere_literis(aed,
        "Es auxiliarius brevis. Responde semper quam brevissime.\n");
    per (i = I; i <= XII; i++)
    {
        sprintf(numerus, "Pars %u. ", i);
        chorda_aedificator_appendere_literis(aed, numerus);
        chorda_aedificator_appendere_literis(aed,
            "Pater noster, qui es in caelis, sanctificetur nomen tuum. "
            "Adveniat regnum tuum. Fiat voluntas tua, sicut in caelo et "
            "in terra. Panem nostrum quotidianum da nobis hodie, et "
            "dimitte nobis debita nostra sicut et nos dimittimus "
            "debitoribus nostris. Et ne nos inducas in tentationem, sed "
            "libera nos a malo. Amen.\n");
    }
    redde chorda_aedificator_finire(aed);
}

interior VatesPetitio*
_petitio_cache (
     Piscina* piscina,
      chorda  exemplar,
      chorda  systema)
{
    VatesPetitio* p = vates_petitio_creare(piscina, exemplar);

    vates_petitio_maxima_ponere(p, MXXIV);
    vates_petitio_conatum_ponere(p, chorda_ex_literis("low", piscina));
    vates_systema_addere(p, systema, VERUM);
    vates_textum_addere(vates_nuntium_addere(p, VATES_USOR),
        chorda_ex_literis("Responde uno verbo: salve.", piscina));
    redde p;
}

interior vacuum
_imprimere (
    constans character* titulus,
        VatesResponsum* r)
{
       i32 i;
    chorda textus;

    textus.datum    = NIHIL;
    textus.mensura  = 0;
    per (i = 0; r->bloci && i < xar_numerus(r->bloci); i++)
    {
        VatesBlocus* b = (VatesBlocus*)xar_obtinere(r->bloci, i);

        si (b->genus == VATES_TEXTUS)
        {
            textus = b->textus;
        }
    }
    imprimere("        %-3s status %u, finis %.*s, conatus %u | in %ld, "
              "cache lectum %ld, cache scr. %ld, out %ld | %ld udollaria, %ld ms\n",
              titulus, r->status_http,
              (integer)r->causa_finis_cruda.mensura,
              (constans character*)r->causa_finis_cruda.datum,
              r->conatus,
              (longus)r->usus.input, (longus)r->usus.cache_lectum,
              (longus)(r->usus.cache_scriptum_5m
                  + r->usus.cache_scriptum_1h),
              (longus)r->usus.output, (longus)r->usus.pretium,
              (longus)r->usus.mora_ms);
    si (textus.mensura > 0)
    {
        imprimere("            textus: %.*s\n",
                  (integer)(textus.mensura
                      > LXXX ? LXXX : textus.mensura),
                  (constans character*)textus.datum);
    }
    si (!r->successus)
    {
        imprimere("            error: %s %.*s %.*s\n",
                  vates_error_descriptio(r->error),
                  (integer)r->error_genus.mensura,
                  (constans character*)r->error_genus.datum,
                  (integer)r->error_nuntius.mensura,
                  (constans character*)r->error_nuntius.datum);
    }
}

interior b32
_cogitationem_habet (
    VatesResponsum* r)
{
       i32 i;
    chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = 0;

    per (i = 0; r->bloci && i < xar_numerus(r->bloci); i++)
    {
        VatesBlocus* b = (VatesBlocus*)xar_obtinere(r->bloci, i);

        si (b->genus == VATES_OPACUM && b->crudum)
        {
            chorda typus = json_capere_chorda(b->crudum, "type",
                                              vacua);

            si (   chorda_aequalis_literis(typus, "thinking")
                || chorda_aequalis_literis(typus, "redacted_thinking"))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

interior i32
_lineae (
    Piscina* piscina)
{
    chorda t = filum_legere_totum(RATIONARIUM, piscina);
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

s32
principale (
      integer   argc,
    character** argv)
{
               Piscina* piscina;
    constans character* exemplar_literae = "claude-sonnet-5-5";
    constans character* domus;
             character  via[MXXIV];
                chorda  crudum;
                chorda  clavis;
         VatesOptiones  o;
                 Vates* v;
                chorda  exemplar;
                chorda  systema;
          VatesPetitio* p;
        VatesResponsum* a;
        VatesResponsum* b;
        VatesResponsum* c;
        VatesResponsum* d = NIHIL;
             character  nota[CCLVI];
                   i32  conatus_omnes;
                   s64  pretium_omne;
               integer  i;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-exemplar") == 0 && i + I < argc)
        {
            exemplar_literae = argv[i + I];
            i++;
        }
        alioquin
        {
            fprintf(stderr, "usus: vates_fumus [-exemplar ID]\n");
            redde II;
        }
    }
    piscina  = piscina_generare_dynamicum("vates_fumus", M * M);
    domus    = getenv("HOME");
    si (!piscina || !domus)
    {
        redde II;
    }
    sprintf(via, "%s/.rhubarb/anthropic.clavis", domus);
    si (!filum_existit(via))
    {
        _nuntiare("clavis", FALSUM,
            "~/.rhubarb/anthropic.clavis abest");
        redde I;
    }
    crudum = filum_legere_totum(via, piscina);
    clavis = chorda_praecidere(crudum);

    (vacuum)unlink(RATIONARIUM);
    o                  = vates_optiones_ordinariae();
    o.rationarium_via  = RATIONARIUM;
    o.herbarium_via    = HERBARIUM;
    o.pretia           = _pretia;
    o.pretia_numerus = (i32)(magnitudo(_pretia)
        / magnitudo(_pretia[0]));
    v = vates_aperire(piscina, chorda_ex_literis("anthropic", piscina),
        clavis, &o);
    /* Vates copiam suam tenet; nostras zephyrare */
    si (crudum.datum)
    {
        memset(crudum.datum, 0, (size_t)crudum.mensura);
    }
    si (!v)
    {
        _nuntiare("vates_aperire", FALSUM, "NIHIL");
        redde I;
    }
    exemplar  = chorda_ex_literis(exemplar_literae, piscina);
    systema   = _systema_longum(piscina);
    imprimere("exemplar %s, systema %u octeti\n\n", exemplar_literae,
              systema.mensura);

    /* I-II: idem praefixum bis -> cache lectum in secunda */
    a = vates_mittere(v, _petitio_cache(piscina, exemplar, systema),
                      chorda_ex_literis("fumus", piscina), piscina);
    _imprimere("A", a);
    b = vates_mittere(v, _petitio_cache(piscina, exemplar, systema),
                      chorda_ex_literis("fumus", piscina), piscina);
    _imprimere("B", b);
    _nuntiare("vocatio A", a->successus,
        a->successus ? "" : "vide errorem supra");
    sprintf(nota, "%ld signa ex cache", (longus)b->usus.cache_lectum);
    _nuntiare("vocatio B: cache lectum > 0", b->successus
        && b->usus.cache_lectum > 0,
              nota);

    /* III-IV: cogitatio, deinde reditus verbatim */
    p = vates_petitio_creare(piscina, exemplar);
    vates_petitio_maxima_ponere(p, IV * M);
    vates_petitio_conatum_ponere(p, chorda_ex_literis("high", piscina));
    vates_petitio_cogitationem_monstrare(p, VERUM);
    vates_textum_addere(vates_nuntium_addere(p, VATES_USOR),
        chorda_ex_literis(
        "Quot sunt numeri primi inter C et CL? Responde numero solo.",
        piscina));
    c = vates_mittere(v, p, chorda_ex_literis("fumus", piscina),
        piscina);
    _imprimere("C", c);
    _nuntiare("vocatio C (cogitatio)", c->successus, "");
    si (c->successus && _cogitationem_habet(c))
    {
        vates_responsum_addere(p, c);
        vates_textum_addere(vates_nuntium_addere(p, VATES_USOR),
            chorda_ex_literis(
            "Et quot inter CL et CC? Responde numero solo.", piscina));
        d = vates_mittere(v, p, chorda_ex_literis("fumus", piscina),
            piscina);
        _imprimere("D", d);
        _nuntiare("vocatio D: cogitatio verbatim reddita", d->successus,
                  d->successus ? "signatura accepta" : "vide errorem supra");
    }
    alioquin si (c->successus)
    {
        imprimere("NOTA   nulla cogitatio reddita - reditus non probatus\n");
    }

    conatus_omnes = a->conatus + b->conatus + c->conatus
        + (d ? d->conatus : 0);
    pretium_omne  = a->usus.pretium + b->usus.pretium + c->usus.pretium
                  + (d ? d->usus.pretium : 0);
    sprintf(nota, "%u lineae, %u conatus", _lineae(piscina),
        conatus_omnes);
    _nuntiare("rationarium: linea per conatum", _lineae(piscina)
        == conatus_omnes, nota);
    imprimere("\npretium totum: %ld micro-dollaria ($%.4f)\n",
              (longus)pretium_omne, (f64)pretium_omne / 1000000.0);
    imprimere("fumus vatis: %s (%u fracti)\n",
              _fracti == 0 ? "sanum" : "FRACTUM", _fracti);
    piscina_destruere(piscina);
    redde _fracti == 0 ? 0 : I;
}
