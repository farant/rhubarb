/* tabularium.c - servus MCP tabularii (implementatio; vide
 * tabularium.h + gesta/phase-log.md chunk C INTENTIO).
 * Sceletum vecturae ex legato copiatum-adaptatum (legatus.c
 * 4941-5153, 6526-6638, 6795-6832) - semen lib/mcp_servus. */

#include "tabularium.h"
#include "json.h"
#include "tabellarius.h"
#include "tabula_dispersa.h"
#include "xar.h"
#include "similitudo.h"
#include "sigillum.h"
#include "chorda_aedificator.h"
#include "vigilia.h"
#include <string.h>

#define TABULARII_PROTOCOLLUM_PINNATUM "2025-06-18"

/* doctrina: in contextum agentis VERBATIM cadit (inventum legati) -
 * contractus scribae hic VIVIT ut sessiones futurae eum hereditent */
interior constans character* constans TABULARII_DOCTRINA =
    "TABULARIUM: acta rerum repositorii (quaestiones/parca/decreta/"
    "notae/desiderata) super acta gestarum. CONTRACTUS SCRIBAE: "
    "Claude notas/parca/decreta per debriefs SPONTE scribit (sine "
    "confirmatione singula - recensio per quaestiones, non portas). "
    "addere {genus, titulus, corpus?, tags? (commatibus), ancorae? "
    "(JSON), actor?} = res nova. gerere {res (id aut titulus), "
    "actus: nota|status|nexus|denexus|mutatio|remotio, textus?/"
    "novus?/verbum?/alterum?/clavis?/valor?/datum?} = eventus unus. "
    "quaerere {textus, genus?, status?, tag?} = FTS (idioma "
    "Latinum: praefixa 'parsur*' - stemmata Anglica sola). res "
    "{res} = status + ancorae (CAUTIO si inresolutae) + actiones "
    "affordatae + annales. "
    "census {} = genera x status + tags + arretrata. "
    "agere {actio?|processus?, ligamina? (JSON: ops->res_id|"
    "titulus), argumenta? (JSON), actor?} = K3: actionem exsequi "
    "(porta obstat - recusata causam nominat; effectus atomice, "
    "actio-facta in flumine actionis) AUT processum incipere "
    "(instantia photographat, opera cursuum generantur). Opus "
    "processus: susceptio = gerere status susceptum; perfectio = "
    "gerere mutatio {effectus} + status perfectum -> provectio "
    "SPONTE sequitur (gradus proximi/portae/actiones). RECEPTA "
    "SUNT CODEX: definitiones actionum/processuum per semen aut "
    "canalem solum - agere exsequitur, numquam definit. Genera: "
    "quaestio (apertum->laborans->clausum|relictum), parcum "
    "(parcatum->tractum->clausum), decretum, nota, desideratum "
    "(apertum->impletum), opus (pendens->susceptum?->perfectum|"
    "omissum), actiones (claudere-cum-decreto), processus "
    "(ritus-signaculi). Violationes machinae NON obstant - nota "
    "custodiae appenditur (iudicat, non obstat). "
    "RAMI (K4): ramus {actus: creare|enumerare|comparare|fundere|"
    "abicere, titulus?, parens?, res?, vis?} = lineae temporis "
    "parallelae intra tabularium. truncus = veritas, rami = "
    "hypothetica; ABICERE GRATIS - fingere audacter (fusio casus "
    "rarus, non finis debitus). parametrum 'ramus' in addere/"
    "gerere/res = scriptura/lectio in ramo; IN RAMO res_id "
    "REQUIRITUR (resolutio tituli truncalis est). fusio = replay "
    "in truncum attributione servata; confligentia obstat nisi "
    "vis; nidificata nisi parens fusus recusatur. genera/census/"
    "quaerere/motor truncalia manent. MORES: quaere ANTE "
    "filationem (addere titulum duplicatum CAUTIONE monet; "
    "resolutio tituli ambigui candidatos nominat - res_id "
    "discernit); cum res parcata "
    "trahitur, status->tractum statim, cum perficitur ->clausum "
    "(tabula mendax peior prosa); orientatio post-compactionem = "
    "census + quaerere in area laboris; divisio actorum: vita-"
    "cyclica HIC, narratio aedificandi in phase-log, inventa "
    "codicis in worklog, MEMORY.md = reflexus solum. Via frigida "
    "(residente absente): ./gesta/frigida.sh";

nomen structura {
    constans character* titulus;
    constans character* datum;
} TabulariumSemen;

/* semina generum (INTENTIO C decisio 2: ad initium, idempotens) */
interior constans TabulariumSemen SEMINA_GENERUM[] = {
    { "quaestio",
      "{\"titulus\":\"quaestio\",\"status_initialis\":\"apertum\","
      "\"machina\":[[\"apertum\",\"laborans\"],[\"laborans\","
      "\"clausum\"],[\"apertum\",\"clausum\"],[\"laborans\","
      "\"apertum\"],[\"apertum\",\"relictum\"],[\"laborans\","
      "\"relictum\"]],\"reducer\":\"ordinarius\"}" },
    { "parcum",
      "{\"titulus\":\"parcum\",\"status_initialis\":\"parcatum\","
      "\"machina\":[[\"parcatum\",\"tractum\"],[\"tractum\","
      "\"clausum\"],[\"parcatum\",\"clausum\"]],\"reducer\":"
      "\"ordinarius\"}" },
    { "decretum",
      "{\"titulus\":\"decretum\",\"reducer\":\"ordinarius\"}" },
    { "nota",
      "{\"titulus\":\"nota\",\"reducer\":\"ordinarius\"}" },
    { "desideratum",
      "{\"titulus\":\"desideratum\",\"status_initialis\":"
      "\"apertum\",\"machina\":[[\"apertum\",\"impletum\"],"
      "[\"apertum\",\"relictum\"]],\"reducer\":\"ordinarius\"}" },
    /* K2: genus vinculi ipsum - species nexus, partes a/b unicus
     * incoercitae, verbum necessarium, vigens->solutum */
    { "nexus",
      "{\"titulus\":\"nexus\",\"species\":\"nexus\",\"partes\":"
      "[{\"titulus\":\"a\",\"genera_licita\":[],\"cardinalitas\":"
      "\"unicus\"},{\"titulus\":\"b\",\"genera_licita\":[],"
      "\"cardinalitas\":\"unicus\"}],\"attributa\":[{\"titulus\":"
      "\"verbum\",\"typus\":\"textus\",\"necessarium\":true}],"
      "\"status_initialis\":\"vigens\",\"machina\":[[\"vigens\","
      "\"solutum\"]],\"reducer\":\"ordinarius\"}" },
    /* ---- semen v3 (K3): infra hunc locum genera NON-tabulae -
     * fusio ATTRIBUTA_V2 supra SEMINA_BOARD_NUMERUS terminatur
     * (emendatio E2-B2: attributa tabulae numquam in opus/actio/
     * processum stampantur) ---- */
    { "opus",
      "{\"titulus\":\"opus\",\"status_initialis\":\"pendens\","
      "\"machina\":[[\"pendens\",\"susceptum\"],[\"susceptum\","
      "\"perfectum\"],[\"pendens\",\"perfectum\"],[\"pendens\","
      "\"omissum\"],[\"susceptum\",\"omissum\"]],\"attributa\":"
      "[{\"titulus\":\"titulus\",\"typus\":\"textus\","
      "\"necessarium\":true},{\"titulus\":\"assignatum\","
      "\"typus\":\"textus\"},{\"titulus\":\"prioritas\","
      "\"typus\":\"textus\"},{\"titulus\":\"effectus\","
      "\"typus\":\"textus\"}],\"reducer\":\"ordinarius\"}" },
    /* actio princeps (spec-v2 par IV; genus vinculi VIVUM =
     * 'nexus'): $novus bis, obumbrae ter - sex effectus atomice */
    { "claudere-cum-decreto",
      "{\"titulus\":\"claudere-cum-decreto\",\"species\":"
      "\"actio\",\"opes\":[{\"titulus\":\"parcum\",\"genus\":"
      "\"parcum\",\"status_necessarius\":\"tractum\"}],"
      "\"argumenta\":[{\"titulus\":\"cur\",\"typus\":\"textus\","
      "\"necessarium\":true}],\"effectus\":["
      "{\"verbum\":\"creatio\",\"ut\":\"decretum_novum\","
      "\"datum\":{\"genus\":\"decretum\",\"titulus\":"
      "\"$arg.cur\"}},"
      "{\"verbum\":\"creatio\",\"ut\":\"vinculum\",\"datum\":"
      "{\"genus\":\"nexus\",\"verbum\":\"natum-de\"}},"
      "{\"verbum\":\"membrum-additum\",\"res\":"
      "\"$novus.vinculum\",\"datum\":{\"pars\":\"a\","
      "\"membrum\":\"$novus.decretum_novum\"}},"
      "{\"verbum\":\"membrum-additum\",\"res\":"
      "\"$novus.vinculum\",\"datum\":{\"pars\":\"b\","
      "\"membrum\":\"$res.parcum\"}},"
      "{\"verbum\":\"nota\",\"res\":\"$res.parcum\",\"datum\":"
      "{\"textus\":\"clausum: $arg.cur\"}},"
      "{\"verbum\":\"status\",\"res\":\"$res.parcum\",\"datum\":"
      "{\"novus\":\"clausum\"}}]}" },
    /* processus ritūs milestonis (spec-v2 par V, porta-sola;
     * divergentia: cur_sigilli NECESSARIUM - gradus sigillare
     * $arg.cur_sigilli postulat, absentia = defectus mechanicus
     * post ritum totum) */
    { "ritus-signaculi",
      "{\"titulus\":\"ritus-signaculi\",\"species\":"
      "\"processus\",\"status_initialis\":\"currens\","
      "\"machina\":[[\"currens\",\"perfectus\"],[\"currens\","
      "\"defectus\"],[\"currens\",\"omissus\"]],\"opes\":"
      "[{\"titulus\":\"propositum\",\"genus\":\"parcum\"}],"
      "\"argumenta\":[{\"titulus\":\"cur_sigilli\",\"typus\":"
      "\"textus\",\"necessarium\":true}],\"ordines\":"
      "[\"consilium\",\"aedificatio\",\"sigillum\"],\"gradus\":["
      "{\"titulus\":\"interrogatio\",\"ordo\":\"consilium\","
      "\"genus_gradus\":\"opus\",\"positio\":0},"
      "{\"titulus\":\"spec\",\"ordo\":\"consilium\","
      "\"genus_gradus\":\"opus\",\"positio\":1},"
      "{\"titulus\":\"exploratores\",\"ordo\":\"consilium\","
      "\"genus_gradus\":\"opus\",\"positio\":2},"
      "{\"titulus\":\"porta-spec\",\"ordo\":\"aedificatio\","
      "\"genus_gradus\":\"porta\",\"positio\":0,\"condiciones\":"
      "[\"spec\"]},"
      "{\"titulus\":\"frusta\",\"ordo\":\"aedificatio\","
      "\"genus_gradus\":\"opus\",\"positio\":1},"
      "{\"titulus\":\"porta-sigilli\",\"ordo\":\"sigillum\","
      "\"genus_gradus\":\"porta\",\"positio\":0,\"condiciones\":"
      "[\"exploratores\",\"frusta\"]},"
      "{\"titulus\":\"sigillare\",\"ordo\":\"sigillum\","
      "\"genus_gradus\":\"actio\",\"positio\":1,\"actio\":"
      "\"claudere-cum-decreto\",\"ligamina\":{\"parcum\":"
      "\"$ops.propositum\"},\"argumenta\":{\"cur\":"
      "\"sigillum ritus: $arg.cur_sigilli\"}}]}" },
    /* ---- semen v4 (K4): genus rami - metadata linearum temporis
     * parallelarum, SEMPER truncale (decisio 6). parens NON
     * necessarium: radix fert "" et lex chordae vacuae (K2 salus)
     * absentem numeraret - divergentia a spec possessa ---- */
    { "ramus",
      "{\"titulus\":\"ramus\",\"status_initialis\":\"activus\","
      "\"machina\":[[\"activus\",\"fusus\"],[\"activus\","
      "\"abiectus\"]],\"attributa\":[{\"titulus\":\"titulus\","
      "\"typus\":\"textus\",\"necessarium\":true},{\"titulus\":"
      "\"parens\",\"typus\":\"textus\"},{\"titulus\":\"punctum\","
      "\"typus\":\"numerus\"}],\"reducer\":\"ordinarius\"}" }
};

/* scopus fusionis v2 (genera tabulae + nexus); genera K3 infra
 * attributa propria ferunt (emendatio E2-B2) */
#define SEMINA_BOARD_NUMERUS VI
#define SEMINA_NUMERUS X

/* semen v2 (K2 decisio Q9): attributa in genera VIVA - emendatio
 * integra-substitutio ex definitione currenti + attributa (fusio
 * additiva = comitas clientis, decretum Q2). Idempotens: clavis
 * 'attributa' praesens = iam factum. */
interior constans character* constans ATTRIBUTA_V2 =
    "[{\"titulus\":\"titulus\",\"typus\":\"textus\","
    "\"necessarium\":true},"
    "{\"titulus\":\"corpus\",\"typus\":\"textus\"},"
    "{\"titulus\":\"tags\",\"typus\":\"tabulatum\"},"
    "{\"titulus\":\"ancorae\",\"typus\":\"tabulatum\"}]";

interior constans character* constans VOCABULARIUM_TAGORUM =
    "{\"genus\":\"nota\",\"titulus\":\"vocabularium tagorum\","
    "\"corpus\":\"semina: silva officina gesta tessera legatus "
    "vectis lsp mcp perf corpus-law examen vindex colloquium "
    "excubitor aedilis urgens\",\"tags\":[\"vocabularium\"]}";

structura Tabularium {
    Piscina*     piscina;
    GestaMundus* mundus;
    b32          initiatum;
    constans character* radix;
    constans character* via_scrinii;
    constans character* via_annalium;
    constans character* via_nexus;
    constans character* via_tabulae;
    /* index ancorarum (pigre; INTENTIO C decisio 3) */
    b32             index_temptatus;
    TabulaDispersa* sedes_index;     /* titulus -> character* "via:linea" */
    Xar*            sedes_tituli;    /* chorda (valore) */
    /* vigilia (lib/vigilia, K2.1): disci + fontium; glutinosa */
    Vigilia*        vigilia;
};

/* ==================================================
 * auxilia
 * ================================================== */

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

interior constans character*
_litterae (Piscina* piscina, chorda c)
{
    character* l = (character*)piscina_allocare(piscina,
        (memoriae_index)c.mensura + I);

    si (l == NIHIL)
    {
        redde "";
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(l, c.datum, (memoriae_index)c.mensura);
    }
    l[c.mensura] = '\0';
    redde l;
}

interior b32
_chorda_est (chorda c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (memoriae_index)c.mensura == m
        && (m == ZEPHYRUM
            || memcmp(c.datum, litterae, m) == ZEPHYRUM);
}

interior chorda
_arg (JsonValor* argumenta, constans character* titulus)
{
    chorda vacua;
    JsonValor* v;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (argumenta == NIHIL)
    {
        redde vacua;
    }
    v = json_objectum_capere(argumenta, titulus);
    si (v == NIHIL || !json_est_chorda(v))
    {
        redde vacua;
    }
    redde json_ad_chorda(v);
}

interior vacuum
_respondere (FILE* effusio, chorda corpus)
{
    (vacuum)tabellarius_lineam_scribere(effusio, corpus);
}

/* effectus textus instrumenti (forma MCP; ex legato 5112-5153).
 * Infundibulum OMNIUM responsorum textualium - cautio vigiliae
 * hic appenditur (impossibile eam praeterire legendo). */
interior vacuum
_textum_respondere (Tabularium* t, Piscina* pn, FILE* effusio,
    JsonValor* id, chorda textus, b32 error_est)
{
    JsonValor* resultatum = json_objectum_creare(pn);
    JsonValor* contentus = json_tabulatum_creare(pn);
    JsonValor* fragmentum = json_objectum_creare(pn);

    si (t != NIHIL && vigilia_cautio(t->vigilia) != NIHIL)
    {
        ChordaAedificator* aed = chorda_aedificator_creare(pn,
            textus.mensura + CCLVI);

        chorda_aedificator_appendere_chorda(aed, textus);
        chorda_aedificator_appendere_literis(aed, "\n\n");
        chorda_aedificator_appendere_literis(aed,
            vigilia_cautio(t->vigilia));
        textus = chorda_aedificator_finire(aed);
    }
    json_objectum_ponere(fragmentum, "type",
        json_chorda_creare_literis(pn, "text"));
    json_objectum_ponere(fragmentum, "text",
        json_chorda_creare(pn, textus));
    json_tabulatum_addere(contentus, fragmentum);
    json_objectum_ponere(resultatum, "content", contentus);
    si (error_est)
    {
        json_objectum_ponere(resultatum, "isError",
            json_boolean_creare(pn, VERUM));
    }
    _respondere(effusio, tabellarius_responsum(pn, id,
        resultatum));
}

/* ==================================================
 * index ancorarum (nexus.tsv pigre)
 * ================================================== */

interior character*
_plagulam_legere (Piscina* piscina, constans character* via,
    i32* mensura_out)
{
    FILE* pl = fopen(via, "rb");
    long mensura_l;
    character* textus;

    *mensura_out = ZEPHYRUM;
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_l > 0L ? mensura_l + 1L : I));
    si (textus == NIHIL
        || (mensura_l > 0L
            && fread(textus, I, (memoriae_index)mensura_l, pl)
                != (memoriae_index)mensura_l))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    textus[mensura_l] = '\0';
    *mensura_out = (i32)mensura_l;
    redde textus;
}

/* nexus.tsv: titulus \t sedes|usus \t genus \t via \t linea \t ...
 * (forma confirmata; onerator legati 782-819 exemplar). Sedes
 * PRIMA vincit (praebere speculo). */
interior vacuum
_indicem_onerare (Tabularium* t)
{
    character* textus;
    i32 mensura = ZEPHYRUM;
    i32 cursor = ZEPHYRUM;

    t->index_temptatus = VERUM;
    si (t->via_nexus == NIHIL)
    {
        redde;
    }
    textus = _plagulam_legere(t->piscina, t->via_nexus, &mensura);
    si (textus == NIHIL)
    {
        redde;
    }
    t->sedes_index = tabula_dispersa_creare_chorda(t->piscina,
        16384);
    t->sedes_tituli = xar_creare(t->piscina,
        (i32)magnitudo(chorda));
    si (t->sedes_index == NIHIL || t->sedes_tituli == NIHIL)
    {
        t->sedes_index = NIHIL;
        t->sedes_tituli = NIHIL;
        redde;
    }
    dum (cursor < mensura)
    {
        i32 initium = cursor;
        i32 finis;
        chorda campi[VI];
        i32 n_campi = ZEPHYRUM;
        i32 c;

        dum (cursor < mensura && textus[cursor] != '\n')
        {
            cursor++;
        }
        finis = cursor;
        si (cursor < mensura)
        {
            cursor++;
        }
        si (finis == initium || textus[initium] == '#')
        {
            perge;
        }
        /* campos VI primos secare */
        c = initium;
        dum (c <= finis && n_campi < VI)
        {
            i32 campus_initium = c;

            dum (c < finis && textus[c] != '\t')
            {
                c++;
            }
            campi[n_campi].datum = (i8*)(textus + campus_initium);
            campi[n_campi].mensura = (i32)(c - campus_initium);
            n_campi++;
            c++;
        }
        si (n_campi < V || !_chorda_est(campi[I], "sedes"))
        {
            perge;
        }
        si (tabula_dispersa_continet(t->sedes_index, campi[0]))
        {
            perge;   /* sedes prima vicit */
        }
        {
            /* "via:linea" NUL-terminatum in piscina */
            ChordaAedificator* aed = chorda_aedificator_creare(
                t->piscina, LXIV);
            chorda sedes_textus;

            chorda_aedificator_appendere_chorda(aed, campi[III]);
            chorda_aedificator_appendere_literis(aed, ":");
            chorda_aedificator_appendere_chorda(aed, campi[IV]);
            sedes_textus = chorda_aedificator_finire(aed);
            {
                unio { constans character* l; vacuum* v; } u;

                u.l = _litterae(t->piscina, sedes_textus);
                (vacuum)tabula_dispersa_inserere(t->sedes_index,
                    campi[0], u.v);
            }
            {
                chorda* locus = (chorda*)xar_addere(
                    t->sedes_tituli);

                si (locus != NIHIL)
                {
                    *locus = campi[0];
                }
            }
        }
    }
}

/* ancoram symboli solvere: litterae "via:linea" aut NIHIL */
interior constans character*
_sedem_invenire (Tabularium* t, chorda scopus)
{
    vacuum* valor;

    si (!t->index_temptatus)
    {
        _indicem_onerare(t);
    }
    si (t->sedes_index == NIHIL)
    {
        redde NIHIL;
    }
    si (tabula_dispersa_invenire(t->sedes_index, scopus, &valor))
    {
        redde (constans character*)valor;
    }
    redde NIHIL;
}

/* simillima ex titulis sedum (acies plana - Xar segmentatus,
 * similitudo contiguam vult; lectio hover-vicinitatis) */
interior vacuum
_simillima_appendere (Tabularium* t, ChordaAedificator* aed,
    chorda quaestio, Piscina* pn)
{
    i32 n;
    chorda* plana;
    SimilitudoFructus fructus[III];
    i32 inventa;
    i32 i;

    si (t->sedes_tituli == NIHIL)
    {
        redde;
    }
    n = xar_numerus(t->sedes_tituli);
    si (n == ZEPHYRUM)
    {
        redde;
    }
    plana = (chorda*)piscina_allocare_ordinatum(pn,
        (memoriae_index)n * magnitudo(chorda),
        (memoriae_index)magnitudo(vacuum*));
    si (plana == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        plana[i] = *(chorda*)xar_obtinere(t->sedes_tituli, i);
    }
    inventa = similitudo_optima(quaestio, plana, n, fructus, III);
    si (inventa > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aed, "; simillima:");
        per (i = ZEPHYRUM; i < inventa; i++)
        {
            chorda_aedificator_appendere_literis(aed,
                i > ZEPHYRUM ? ", " : " ");
            chorda_aedificator_appendere_chorda(aed,
                plana[fructus[i].index]);
        }
    }
}

/* ancoras entis reddere (lineae "ancora ..." + CAUTIO) */
interior vacuum
_ancoras_reddere (Tabularium* t, ChordaAedificator* aed,
    JsonValor* status_obiectum, Piscina* pn)
{
    JsonValor* ancorae = json_objectum_capere(status_obiectum,
        "ancorae");
    i32 i;

    si (ancorae == NIHIL || !json_est_tabulatum(ancorae))
    {
        redde;
    }
    per (i = ZEPHYRUM; i < json_tabulatum_numerus(ancorae); i++)
    {
        JsonValor* a = json_tabulatum_obtinere(ancorae, i);
        chorda genus;
        chorda scopus;
        chorda sig;

        si (a == NIHIL || !json_est_objectum(a))
        {
            perge;
        }
        genus = _arg(a, "genus");
        scopus = _arg(a, "scopus");
        sig = _arg(a, "sigillum");
        chorda_aedificator_appendere_literis(aed, "\nancora ");
        chorda_aedificator_appendere_chorda(aed, genus);
        chorda_aedificator_appendere_literis(aed, " '");
        chorda_aedificator_appendere_chorda(aed, scopus);
        chorda_aedificator_appendere_literis(aed, "'");
        si (_chorda_est(genus, "symbolum"))
        {
            constans character* sedes = _sedem_invenire(t, scopus);

            si (sedes != NIHIL)
            {
                chorda_aedificator_appendere_literis(aed, " -> ");
                chorda_aedificator_appendere_literis(aed, sedes);
            }
            alioquin
            {
                chorda_aedificator_appendere_literis(aed,
                    " - CAUTIO: inresoluta (symbolum in indice"
                    " non est)");
                _simillima_appendere(t, aed, scopus, pn);
            }
        }
        alioquin si (_chorda_est(genus, "via"))
        {
            constans character* via_l = _litterae(pn, scopus);
            i32 m = ZEPHYRUM;
            character* octeti = _plagulam_legere(pn, via_l, &m);

            si (octeti == NIHIL)
            {
                chorda_aedificator_appendere_literis(aed,
                    " - CAUTIO: plagula abest");
            }
            alioquin si (sig.mensura > ZEPHYRUM)
            {
                Sigillum s = sigillum_computare(octeti,
                    (memoriae_index)m);
                character hex[SIGILLUM_HEX_MENSURA];

                sigillum_hex(&s, hex);
                si (!_chorda_est(sig, hex))
                {
                    chorda_aedificator_appendere_literis(aed,
                        " - CAUTIO: contentum motum (sigillum"
                        " differt)");
                }
                alioquin
                {
                    chorda_aedificator_appendere_literis(aed,
                        " (sigillum congruit)");
                }
            }
        }
    }
}

/* ==================================================
 * lectiones entis (res_id aut titulus)
 * ================================================== */

/* res_id per titulum exactum; chorda vacua = absens */
interior chorda
_res_per_titulum (Tabularium* t, chorda titulus, Piscina* pn)
{
    ScriniumEnuntiatum* e;
    chorda fructus;

    fructus.mensura = ZEPHYRUM;
    fructus.datum = NIHIL;
    e = scrinium_praeparare(gesta_scrinium(t->mundus),
        "SELECT res_id FROM res WHERE titulus = ? LIMIT 1");
    si (e == NIHIL)
    {
        redde fructus;
    }
    scrinium_ligare_textum(e, I, titulus);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        fructus = scrinium_columna_textus(e, 0, pn);
    }
    scrinium_finire(e);
    redde fructus;
}

/* numerus rerum eodem titulo (acies titulorum duplicatorum) */
interior s64
_tituli_numerus (Tabularium* t, chorda titulus)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "SELECT COUNT(*) FROM res WHERE titulus = ?");
    s64 n = ZEPHYRUM;

    si (e == NIHIL)
    {
        redde ZEPHYRUM;
    }
    scrinium_ligare_textum(e, I, titulus);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        n = scrinium_columna_numerus(e, 0);
    }
    scrinium_finire(e);
    redde n;
}

/* clavem (id aut titulum) ad res_id solvere; titulus pluribus rebus
 * = AMBIGUUS (vacua + vexillum - exemplar legati multi-definitorum;
 * LIMIT-1 tacitum erat acies, quaestio 'Tituli duplicati') */
interior chorda
_res_solvere (Tabularium* t, chorda clavis, Piscina* pn,
    b32* ambiguum_out)
{
    chorda d = gesta_res_datum(t->mundus,
        _litterae(pn, clavis), pn);
    chorda vacua;

    si (ambiguum_out != NIHIL)
    {
        *ambiguum_out = FALSUM;
    }
    si (d.mensura > ZEPHYRUM)
    {
        redde clavis;   /* res_id directum */
    }
    si (_tituli_numerus(t, clavis) > (s64)I)
    {
        si (ambiguum_out != NIHIL)
        {
            *ambiguum_out = VERUM;
        }
        vacua.mensura = ZEPHYRUM;
        vacua.datum = NIHIL;
        redde vacua;
    }
    redde _res_per_titulum(t, clavis, pn);
}

/* candidatos ambiguitatis nominare (res_id discernit) */
interior vacuum
_ambiguitatem_respondere (Tabularium* t, Piscina* pn, JsonValor* id,
    chorda titulus, FILE* effusio)
{
    ChordaAedificator* aed = chorda_aedificator_creare(pn, CCLVI);
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "SELECT res_id, genus, status FROM res WHERE titulus = ?"
        " ORDER BY res_id LIMIT 5");

    chorda_aedificator_appendere_literis(aed, "titulus ambiguus '");
    chorda_aedificator_appendere_chorda(aed, titulus);
    chorda_aedificator_appendere_literis(aed,
        "' - res_id adhibe:");
    si (e != NIHIL)
    {
        scrinium_ligare_textum(e, I, titulus);
        dum (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            chorda_aedificator_appendere_literis(aed, "\n  ");
            chorda_aedificator_appendere_chorda(aed,
                scrinium_columna_textus(e, 0, pn));
            chorda_aedificator_appendere_literis(aed, " (");
            chorda_aedificator_appendere_chorda(aed,
                scrinium_columna_textus(e, I, pn));
            chorda_aedificator_appendere_literis(aed, "/");
            chorda_aedificator_appendere_chorda(aed,
                scrinium_columna_textus(e, II, pn));
            chorda_aedificator_appendere_literis(aed, ")");
        }
        scrinium_finire(e);
    }
    _textum_respondere(t, pn, effusio, id,
        chorda_aedificator_finire(aed), VERUM);
}

/* ==================================================
 * vincula (K2) - saccharum super res nexus-speciei
 * ================================================== */

/* titulus rei aut textus crudus (membra tolerantia textum crudum
 * ferunt - solutio deest, titulus nullus) */
interior chorda
_titulus_membri (Tabularium* t, chorda membrum, Piscina* pn)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "SELECT titulus FROM res WHERE res_id = ?");
    chorda fructus = membrum;

    si (e == NIHIL)
    {
        redde fructus;
    }
    scrinium_ligare_textum(e, I, membrum);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        chorda tt = scrinium_columna_textus(e, 0, pn);

        si (tt.mensura > ZEPHYRUM)
        {
            fructus = tt;
        }
    }
    scrinium_finire(e);
    redde fructus;
}

/* verbum vinculi ex statu rei (vacua si abest) */
interior chorda
_verbum_vinculi (Tabularium* t, chorda vinculum, Piscina* pn)
{
    chorda d = gesta_res_datum(t->mundus,
        _litterae(pn, vinculum), pn);
    chorda vacua;
    JsonResultus r;
    JsonValor* v;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (d.mensura == ZEPHYRUM)
    {
        redde vacua;
    }
    r = json_legere(d, pn);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde vacua;
    }
    v = json_objectum_capere(r.radix, "verbum");
    si (v == NIHIL || !json_est_chorda(v))
    {
        redde vacua;
    }
    redde json_ad_chorda(v);
}

interior b32
_membrum_scribere (Tabularium* t, Piscina* pn,
    constans character* vinculum, constans character* pars,
    chorda membrum, constans character* actor_l)
{
    JsonValor* d = json_objectum_creare(pn);
    GestaEventum ev;

    json_objectum_ponere(d, "pars",
        json_chorda_creare_literis(pn, pars));
    json_objectum_ponere(d, "membrum",
        json_chorda_creare(pn, membrum));
    ev.res_id = vinculum;
    ev.genus_eventus = "membrum-additum";
    ev.datum = _litterae(pn, json_scribere(d, pn));
    ev.actor = actor_l;
    ev.origo = "mcp";
    redde gesta_scribere(t->mundus, &ev, NIHIL);
}

/* ==================================================
 * tabula.md - proiectio status legibilis (K1.1)
 *
 * Plicatura in textum: res APERTAE per genus, decreta, nexus.
 * CONTENT-DETERMINISTICA (caput = seq + creatum eventi ultimi,
 * SINE horologio - plagula mutatur solum cum acta mutantur, diffs
 * git honestae manent). Clausa/relicta/impleta OMISSA: tabula =
 * nunc, historia = quaestiones.
 * ================================================== */

/* sectionem generis appendere: ordines status/titulus/tags */
interior vacuum
_tabulae_sectionem (Tabularium* t, ChordaAedificator* aed,
    constans character* genus, constans character* titulus_sectionis,
    b32 cum_statu, Piscina* pn)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "SELECT titulus, status, datum FROM res WHERE genus = ?"
        " AND status NOT IN ('clausum','relictum','impletum')"
        " ORDER BY status, titulus");
    b32 caput_scriptum = FALSUM;

    si (e == NIHIL)
    {
        redde;
    }
    scrinium_ligare_textum(e, I, _ch(genus));
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        chorda titulus = scrinium_columna_textus(e, 0, pn);
        chorda status = scrinium_columna_textus(e, I, pn);
        chorda datum = scrinium_columna_textus(e, II, pn);

        si (!caput_scriptum)
        {
            chorda_aedificator_appendere_literis(aed, "\n## ");
            chorda_aedificator_appendere_literis(aed,
                titulus_sectionis);
            chorda_aedificator_appendere_literis(aed, "\n\n");
            caput_scriptum = VERUM;
        }
        chorda_aedificator_appendere_literis(aed, "- ");
        si (cum_statu && status.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed, "[");
            chorda_aedificator_appendere_chorda(aed, status);
            chorda_aedificator_appendere_literis(aed, "] ");
        }
        chorda_aedificator_appendere_chorda(aed, titulus);
        /* tags ex dato */
        {
            JsonResultus r = json_legere(datum, pn);

            si (r.successus && json_est_objectum(r.radix))
            {
                JsonValor* tags = json_objectum_capere(r.radix,
                    "tags");

                si (tags != NIHIL && json_est_tabulatum(tags)
                    && json_tabulatum_numerus(tags) > ZEPHYRUM)
                {
                    i32 i;

                    chorda_aedificator_appendere_literis(aed,
                        "  `");
                    per (i = ZEPHYRUM;
                         i < json_tabulatum_numerus(tags); i++)
                    {
                        JsonValor* tg = json_tabulatum_obtinere(
                            tags, i);

                        si (tg == NIHIL || !json_est_chorda(tg))
                        {
                            perge;
                        }
                        si (i > ZEPHYRUM)
                        {
                            chorda_aedificator_appendere_literis(
                                aed, " ");
                        }
                        chorda_aedificator_appendere_chorda(aed,
                            json_ad_chorda(tg));
                    }
                    chorda_aedificator_appendere_literis(aed, "`");
                }
            }
        }
        chorda_aedificator_appendere_literis(aed, "\n");
    }
    scrinium_finire(e);
}

/* sectio processuum (K3): instantiae non-terminatae cum cursu
 * graduum - defectae VISIBILES manent (attentionem petunt) */
interior vacuum
_tabulae_processus (Tabularium* t, ChordaAedificator* aed,
    Piscina* pn)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "SELECT r.res_id, r.genus, r.status, r.datum FROM res r"
        " JOIN genera g ON g.titulus = r.genus"
        " WHERE r.status NOT IN ('perfectus','omissus')"
        " AND g.datum LIKE '%\"species\":\"processus\"%'"
        " ORDER BY r.genus, r.res_id");
    b32 caput_scriptum = FALSUM;

    si (e == NIHIL)
    {
        redde;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        chorda genus = scrinium_columna_textus(e, I, pn);
        chorda status = scrinium_columna_textus(e, II, pn);
        chorda datum = scrinium_columna_textus(e, III, pn);

        si (!caput_scriptum)
        {
            chorda_aedificator_appendere_literis(aed,
                "\n## PROCESSUS\n\n");
            caput_scriptum = VERUM;
        }
        chorda_aedificator_appendere_literis(aed, "- [");
        chorda_aedificator_appendere_chorda(aed, status);
        chorda_aedificator_appendere_literis(aed, "] ");
        chorda_aedificator_appendere_chorda(aed, genus);
        {
            JsonResultus r = json_legere(datum, pn);
            JsonValor* cursus = (r.successus
                && json_est_objectum(r.radix))
                ? json_objectum_capere(r.radix, "cursus")
                : NIHIL;

            si (cursus != NIHIL && json_est_objectum(cursus))
            {
                JsonObjectumIterator iter =
                    json_objectum_iterator(cursus);
                chorda k;
                JsonValor* v;
                b32 primus = VERUM;

                chorda_aedificator_appendere_literis(aed, "  (");
                dum (json_objectum_iterator_proxima(&iter, &k,
                    &v))
                {
                    si (!primus)
                    {
                        chorda_aedificator_appendere_literis(aed,
                            "; ");
                    }
                    primus = FALSUM;
                    chorda_aedificator_appendere_chorda(aed, k);
                    chorda_aedificator_appendere_literis(aed,
                        " ");
                    si (v != NIHIL && json_est_chorda(v))
                    {
                        chorda_aedificator_appendere_chorda(aed,
                            json_ad_chorda(v));
                    }
                }
                chorda_aedificator_appendere_literis(aed, ")");
            }
        }
        chorda_aedificator_appendere_literis(aed, "\n");
    }
    scrinium_finire(e);
}

/* prototypum (definitio post _tab_agere) */
interior s64
_rami_eventa (Tabularium* t, chorda ramus_id, Piscina* pn);

/* sectio RAMI tabulae (K4): rami ACTIVI soli - lineae temporis
 * apertae quas sessio sequens videre debet (fusi/abiecti = annales,
 * enumerare eos refert) */
interior vacuum
_tabulae_rami (Tabularium* t, ChordaAedificator* aed, Piscina* pn)
{
    Xar* rami = gesta_ramos_enumerare(t->mundus, pn);
    b32 caput_scriptum = FALSUM;
    character numeri[LXIV];
    i32 i;

    si (rami == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(rami); i++)
    {
        GestaRamusOrdo* o = (GestaRamusOrdo*)xar_obtinere(rami, i);

        si (o == NIHIL || !_chorda_est(o->status, "activus"))
        {
            perge;
        }
        si (!caput_scriptum)
        {
            chorda_aedificator_appendere_literis(aed,
                "\n## RAMI\n\n");
            caput_scriptum = VERUM;
        }
        chorda_aedificator_appendere_literis(aed, "- ");
        chorda_aedificator_appendere_chorda(aed, o->titulus);
        sprintf(numeri, "  punctum %d  eventa %d",
            (int)o->punctum,
            (int)_rami_eventa(t, o->res_id, pn));
        chorda_aedificator_appendere_literis(aed, numeri);
        chorda_aedificator_appendere_literis(aed, "\n");
    }
}

interior vacuum
_tabulam_scribere (Tabularium* t, Piscina* pn)
{
    ChordaAedificator* aed;
    FILE* pl;
    chorda textus;

    si (t->via_tabulae == NIHIL || t->mundus == NIHIL)
    {
        redde;
    }
    aed = chorda_aedificator_creare(pn, 8192);
    si (aed == NIHIL)
    {
        redde;
    }
    chorda_aedificator_appendere_literis(aed,
        "# TABULA (proiectio tabularii - GENERATUM, noli manu"
        " edere)\n\n");
    /* caput deterministicum: seq + creatum ultimi eventi */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT seq, creatum FROM tessellae ORDER BY seq DESC"
            " LIMIT 1");

        si (e != NIHIL)
        {
            si (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                character numeri[XXXII];

                sprintf(numeri, "seq %d",
                    (int)scrinium_columna_numerus(e, 0));
                chorda_aedificator_appendere_literis(aed, numeri);
                chorda_aedificator_appendere_literis(aed, " - ");
                chorda_aedificator_appendere_chorda(aed,
                    scrinium_columna_textus(e, I, pn));
                chorda_aedificator_appendere_literis(aed, "\n");
            }
            scrinium_finire(e);
        }
    }
    _tabulae_sectionem(t, aed, "quaestio", "QUAESTIONES", VERUM,
        pn);
    _tabulae_sectionem(t, aed, "parcum", "PARCA", VERUM, pn);
    _tabulae_sectionem(t, aed, "desideratum", "DESIDERATA", FALSUM,
        pn);
    _tabulae_sectionem(t, aed, "decretum", "DECRETA", FALSUM, pn);
    /* K3: opera aperta (actionabilia) + instantiae processuum */
    _tabulae_sectionem(t, aed, "opus", "OPERA", VERUM, pn);
    _tabulae_processus(t, aed, pn);
    _tabulae_rami(t, aed, pn);
    /* nexus (vincula = res nexus-speciei per indicem membra; K2
     * cutover). Soluta sponte absunt - plicatura membra eorum
     * ordines purgat. Bi-partia cum verbo = forma sagittae; alia
     * = index crudus (parcum 'multi-partium redditio'). */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT DISTINCT res_id FROM membra ORDER BY res_id");
        b32 caput_scriptum = FALSUM;

        si (e != NIHIL)
        {
            dum (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                chorda vinculum = scrinium_columna_textus(e, 0,
                    pn);
                chorda d = gesta_res_datum(t->mundus,
                    _litterae(pn, vinculum), pn);
                JsonResultus r;
                JsonValor* membra = NIHIL;
                JsonValor* verbum = NIHIL;
                JsonValor* acies_a = NIHIL;
                JsonValor* acies_b = NIHIL;

                si (d.mensura > ZEPHYRUM)
                {
                    r = json_legere(d, pn);
                    si (r.successus && json_est_objectum(r.radix))
                    {
                        membra = json_objectum_capere(r.radix,
                            "membra");
                        verbum = json_objectum_capere(r.radix,
                            "verbum");
                    }
                }
                si (membra != NIHIL && json_est_objectum(membra))
                {
                    acies_a = json_objectum_capere(membra, "a");
                    acies_b = json_objectum_capere(membra, "b");
                }
                si (!caput_scriptum)
                {
                    chorda_aedificator_appendere_literis(aed,
                        "\n## NEXUS\n\n");
                    caput_scriptum = VERUM;
                }
                chorda_aedificator_appendere_literis(aed, "- ");
                si (verbum != NIHIL && json_est_chorda(verbum)
                    && acies_a != NIHIL
                    && json_est_tabulatum(acies_a)
                    && json_tabulatum_numerus(acies_a) == I
                    && acies_b != NIHIL
                    && json_est_tabulatum(acies_b)
                    && json_tabulatum_numerus(acies_b) == I)
                {
                    chorda_aedificator_appendere_chorda(aed,
                        _titulus_membri(t, json_ad_chorda(
                            json_tabulatum_obtinere(acies_a,
                                ZEPHYRUM)), pn));
                    chorda_aedificator_appendere_literis(aed,
                        " --");
                    chorda_aedificator_appendere_chorda(aed,
                        json_ad_chorda(verbum));
                    chorda_aedificator_appendere_literis(aed,
                        "--> ");
                    chorda_aedificator_appendere_chorda(aed,
                        _titulus_membri(t, json_ad_chorda(
                            json_tabulatum_obtinere(acies_b,
                                ZEPHYRUM)), pn));
                }
                alioquin
                {
                    /* forma indicis: vinculum + partes */
                    chorda_aedificator_appendere_chorda(aed,
                        vinculum);
                    si (membra != NIHIL
                        && json_est_objectum(membra))
                    {
                        JsonObjectumIterator iter =
                            json_objectum_iterator(membra);
                        chorda k;
                        JsonValor* v;

                        chorda_aedificator_appendere_literis(aed,
                            ":");
                        dum (json_objectum_iterator_proxima(&iter,
                            &k, &v))
                        {
                            i32 j;

                            si (v == NIHIL
                                || !json_est_tabulatum(v))
                            {
                                perge;
                            }
                            per (j = ZEPHYRUM;
                                 j < json_tabulatum_numerus(v);
                                 j++)
                            {
                                chorda_aedificator_appendere_literis(
                                    aed, " ");
                                chorda_aedificator_appendere_chorda(
                                    aed, k);
                                chorda_aedificator_appendere_literis(
                                    aed, "=");
                                chorda_aedificator_appendere_chorda(
                                    aed, _titulus_membri(t,
                                        json_ad_chorda(
                                            json_tabulatum_obtinere(
                                                v, j)), pn));
                            }
                        }
                    }
                }
                chorda_aedificator_appendere_literis(aed, "\n");
            }
            scrinium_finire(e);
        }
    }
    textus = chorda_aedificator_finire(aed);
    pl = fopen(t->via_tabulae, "wb");
    si (pl == NIHIL)
    {
        redde;
    }
    si (textus.mensura > ZEPHYRUM)
    {
        (vacuum)fwrite(textus.datum, I,
            (memoriae_index)textus.mensura, pl);
    }
    fclose(pl);
}

/* ==================================================
 * instrumenta
 * ================================================== */

/* prototypum resolutionis ramorum (definitio post _tab_agere -
 * addere/gerere/res parametrum 'ramus' eam postulant) */
interior chorda
_ramum_solvere (Tabularium* t, chorda clavis, Piscina* pn);

interior vacuum
_tab_addere (Tabularium* t, Piscina* pn, JsonValor* id,
    JsonValor* argumenta, FILE* effusio)
{
    chorda genus = _arg(argumenta, "genus");
    chorda titulus = _arg(argumenta, "titulus");
    chorda corpus = _arg(argumenta, "corpus");
    chorda tags = _arg(argumenta, "tags");
    chorda ancorae = _arg(argumenta, "ancorae");
    chorda actor = _arg(argumenta, "actor");
    chorda ramus_arg = _arg(argumenta, "ramus");
    chorda ramus_id;
    JsonValor* datum;
    GestaEventum e;
    character res_id[GESTA_RES_ID_MENSURA];
    chorda datum_textus;

    ramus_id.mensura = ZEPHYRUM;
    ramus_id.datum = NIHIL;
    si (genus.mensura == ZEPHYRUM || titulus.mensura == ZEPHYRUM)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("genus et titulus requiruntur"), VERUM);
        redde;
    }
    si (ramus_arg.mensura > ZEPHYRUM)
    {
        ramus_id = _ramum_solvere(t, ramus_arg, pn);
        si (ramus_id.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("ramus ignotus"), VERUM);
            redde;
        }
    }
    datum = json_objectum_creare(pn);
    json_objectum_ponere(datum, "genus",
        json_chorda_creare(pn, genus));
    json_objectum_ponere(datum, "titulus",
        json_chorda_creare(pn, titulus));
    si (corpus.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(datum, "corpus",
            json_chorda_creare(pn, corpus));
    }
    si (tags.mensura > ZEPHYRUM)
    {
        /* commatibus secare */
        JsonValor* tabulatum = json_tabulatum_creare(pn);
        i32 i = ZEPHYRUM;

        dum (i < tags.mensura)
        {
            i32 initium = i;
            chorda tag;

            dum (i < tags.mensura && tags.datum[i] != (i8)',')
            {
                i++;
            }
            tag.datum = tags.datum + initium;
            tag.mensura = (i32)(i - initium);
            /* spatia marginum tondere */
            dum (tag.mensura > ZEPHYRUM
                && tag.datum[0] == (i8)' ')
            {
                tag.datum++;
                tag.mensura--;
            }
            dum (tag.mensura > ZEPHYRUM
                && tag.datum[tag.mensura - I] == (i8)' ')
            {
                tag.mensura--;
            }
            si (tag.mensura > ZEPHYRUM)
            {
                json_tabulatum_addere(tabulatum,
                    json_chorda_creare(pn, tag));
            }
            i++;
        }
        json_objectum_ponere(datum, "tags", tabulatum);
    }
    si (ancorae.mensura > ZEPHYRUM)
    {
        JsonResultus r = json_legere(ancorae, pn);

        si (!r.successus || !json_est_tabulatum(r.radix))
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("ancorae: tabulatum JSON requiritur"), VERUM);
            redde;
        }
        json_objectum_ponere(datum, "ancorae", r.radix);
    }
    datum_textus = json_scribere(datum, pn);

    e.res_id = NIHIL;
    e.genus_eventus = "creatio";
    e.datum = _litterae(pn, datum_textus);
    e.actor = actor.mensura > ZEPHYRUM
        ? _litterae(pn, actor) : "claude";
    e.origo = "mcp";
    /* in ramo: scriptura iudicata in linea parallela - tabula/FTS
     * trunci intacta (invisibilis usque ad fusionem) */
    si (ramus_id.mensura > ZEPHYRUM)
    {
        si (!gesta_in_ramo_scribere(t->mundus, &e,
                _litterae(pn, ramus_id), res_id))
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed,
                "scriptura recusata: ");
            chorda_aedificator_appendere_literis(aed,
                gesta_error(t->mundus));
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), VERUM);
            redde;
        }
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed, "res ");
            chorda_aedificator_appendere_literis(aed, res_id);
            chorda_aedificator_appendere_literis(aed,
                " creata: ");
            chorda_aedificator_appendere_chorda(aed, titulus);
            chorda_aedificator_appendere_literis(aed, " (");
            chorda_aedificator_appendere_chorda(aed, genus);
            chorda_aedificator_appendere_literis(aed,
                ", in ramo ");
            chorda_aedificator_appendere_chorda(aed, ramus_arg);
            chorda_aedificator_appendere_literis(aed,
                ") - trunco invisibilis usque ad fusionem;"
                " res_id ad lectiones/scripturas ramales adhibe");
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
        }
        redde;
    }
    si (!gesta_scribere(t->mundus, &e, res_id))
    {
        ChordaAedificator* aed = chorda_aedificator_creare(pn,
            CCLVI);

        chorda_aedificator_appendere_literis(aed,
            "scriptura recusata: ");
        chorda_aedificator_appendere_literis(aed,
            gesta_error(t->mundus));
        _textum_respondere(t, pn, effusio, id,
            chorda_aedificator_finire(aed), VERUM);
        redde;
    }
    _tabulam_scribere(t, pn);
    {
        ChordaAedificator* aed = chorda_aedificator_creare(pn,
            CCLVI);
        chorda status = gesta_res_status(t->mundus, res_id, pn);
        s64 eodem_titulo = _tituli_numerus(t, titulus);

        chorda_aedificator_appendere_literis(aed, "res ");
        chorda_aedificator_appendere_literis(aed, res_id);
        chorda_aedificator_appendere_literis(aed, " creata: ");
        chorda_aedificator_appendere_chorda(aed, titulus);
        chorda_aedificator_appendere_literis(aed, " (");
        chorda_aedificator_appendere_chorda(aed, genus);
        si (status.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed, ", status ");
            chorda_aedificator_appendere_chorda(aed, status);
        }
        chorda_aedificator_appendere_literis(aed, ")");
        si (eodem_titulo > (s64)I)
        {
            character cautio[CXXVIII];

            sprintf(cautio, "\nCAUTIO: titulus iam exsistit (%d"
                " res eodem titulo) - res_id ad discretionem"
                " adhibe", (int)eodem_titulo);
            chorda_aedificator_appendere_literis(aed, cautio);
        }
        _textum_respondere(t, pn, effusio, id,
            chorda_aedificator_finire(aed), FALSUM);
    }
}

interior vacuum
_tab_gerere (Tabularium* t, Piscina* pn, JsonValor* id,
    JsonValor* argumenta, FILE* effusio)
{
    chorda clavis = _arg(argumenta, "res");
    chorda actus = _arg(argumenta, "actus");
    chorda actor = _arg(argumenta, "actor");
    chorda ramus_arg = _arg(argumenta, "ramus");
    chorda ramus_id;
    chorda res_id;
    JsonValor* datum = json_objectum_creare(pn);
    constans character* genus_eventus = NIHIL;
    GestaEventum e;

    ramus_id.mensura = ZEPHYRUM;
    ramus_id.datum = NIHIL;
    si (clavis.mensura == ZEPHYRUM || actus.mensura == ZEPHYRUM)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("res et actus requiruntur"), VERUM);
        redde;
    }
    si (ramus_arg.mensura > ZEPHYRUM)
    {
        ramus_id = _ramum_solvere(t, ramus_arg, pn);
        si (ramus_id.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("ramus ignotus"), VERUM);
            redde;
        }
    }
    si (ramus_id.mensura > ZEPHYRUM)
    {
        /* LEX E2-B1: in ramo res_id requiritur - resolutio tituli
         * statum TRUNCI legit et male solveret (res solum-ramales
         * titulis invisibiles) */
        res_id = clavis;
        si (gesta_res_in_ramo_datum(t->mundus,
                _litterae(pn, clavis), _litterae(pn, ramus_id),
                pn).mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("res in ramo ignota (in ramo res_id"
                    " requiritur - resolutio tituli truncalis"
                    " est)"), VERUM);
            redde;
        }
    }
    alioquin
    {
        b32 ambiguum = FALSUM;

        res_id = _res_solvere(t, clavis, pn, &ambiguum);
        si (ambiguum)
        {
            _ambiguitatem_respondere(t, pn, id, clavis, effusio);
            redde;
        }
        si (res_id.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("res ignota (id aut titulus exactus)"), VERUM);
            redde;
        }
    }
    si (_chorda_est(actus, "nota"))
    {
        chorda textus = _arg(argumenta, "textus");

        si (textus.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("nota: textus requiritur"), VERUM);
            redde;
        }
        genus_eventus = "nota";
        json_objectum_ponere(datum, "textus",
            json_chorda_creare(pn, textus));
    }
    alioquin si (_chorda_est(actus, "status"))
    {
        chorda novus = _arg(argumenta, "novus");

        si (novus.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("status: novus requiritur"), VERUM);
            redde;
        }
        genus_eventus = "status";
        json_objectum_ponere(datum, "novus",
            json_chorda_creare(pn, novus));
    }
    alioquin si (_chorda_est(actus, "nexus"))
    {
        /* saccharum K2 (spec par VIII): grammatica eadem,
         * machinamentum novum - res vinculi propria (creatio +
         * membrum a + membrum b). Alterum insolutum = textus
         * crudus (tolerans - custodia notat, non obstat). */
        chorda verbum = _arg(argumenta, "verbum");
        chorda alterum = _arg(argumenta, "alterum");
        chorda alterum_id;
        chorda membrum_b;
        constans character* actor_l;
        character vinculum_id[GESTA_RES_ID_MENSURA];
        JsonValor* d;
        GestaEventum ev;

        si (ramus_id.mensura > ZEPHYRUM)
        {
            /* saccharum vinculi = scripturae plures + resolutio
             * alterius truncalis (fovea E2-B1) - in ramo nondum */
            _textum_respondere(t, pn, effusio, id,
                _ch("nexus/denexus in ramo nondum sustentus"
                    " (parcum)"), VERUM);
            redde;
        }
        si (verbum.mensura == ZEPHYRUM
            || alterum.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("nexus: verbum et alterum requiruntur"),
                VERUM);
            redde;
        }
        alterum_id = _res_solvere(t, alterum, pn, NIHIL);
        membrum_b = alterum_id.mensura > ZEPHYRUM
            ? alterum_id : alterum;
        actor_l = actor.mensura > ZEPHYRUM
            ? _litterae(pn, actor) : "claude";
        d = json_objectum_creare(pn);
        json_objectum_ponere(d, "genus",
            json_chorda_creare_literis(pn, "nexus"));
        json_objectum_ponere(d, "verbum",
            json_chorda_creare(pn, verbum));
        ev.res_id = NIHIL;
        ev.genus_eventus = "creatio";
        ev.datum = _litterae(pn, json_scribere(d, pn));
        ev.actor = actor_l;
        ev.origo = "mcp";
        si (!gesta_scribere(t->mundus, &ev, vinculum_id)
            || !_membrum_scribere(t, pn, vinculum_id, "a",
                   res_id, actor_l)
            || !_membrum_scribere(t, pn, vinculum_id, "b",
                   membrum_b, actor_l))
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("scriptura vinculi fracta"), VERUM);
            redde;
        }
        _tabulam_scribere(t, pn);
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed, "nexus ");
            chorda_aedificator_appendere_literis(aed, vinculum_id);
            chorda_aedificator_appendere_literis(aed,
                " creatum: ");
            chorda_aedificator_appendere_chorda(aed,
                _titulus_membri(t, res_id, pn));
            chorda_aedificator_appendere_literis(aed, " --");
            chorda_aedificator_appendere_chorda(aed, verbum);
            chorda_aedificator_appendere_literis(aed, "--> ");
            chorda_aedificator_appendere_chorda(aed,
                _titulus_membri(t, membrum_b, pn));
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
        }
        redde;
    }
    alioquin si (_chorda_est(actus, "denexus"))
    {
        /* vinculum a/b + verbo congruens invenire -> status
         * solutum (index purgatur, res et historia manent) */
        chorda verbum = _arg(argumenta, "verbum");
        chorda alterum = _arg(argumenta, "alterum");
        chorda alterum_id;
        chorda membrum_b;
        chorda vinculum;
        constans character* actor_l;
        ScriniumEnuntiatum* sel;
        GestaEventum ev;

        si (ramus_id.mensura > ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("nexus/denexus in ramo nondum sustentus"
                    " (parcum)"), VERUM);
            redde;
        }
        si (verbum.mensura == ZEPHYRUM
            || alterum.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("denexus: verbum et alterum requiruntur"),
                VERUM);
            redde;
        }
        alterum_id = _res_solvere(t, alterum, pn, NIHIL);
        membrum_b = alterum_id.mensura > ZEPHYRUM
            ? alterum_id : alterum;
        vinculum.mensura = ZEPHYRUM;
        vinculum.datum = NIHIL;
        sel = scrinium_praeparare(gesta_scrinium(t->mundus),
            "SELECT m1.res_id FROM membra m1"
            " JOIN membra m2 ON m2.res_id = m1.res_id"
            " WHERE m1.pars = 'a' AND m1.membrum = ?1"
            " AND m2.pars = 'b' AND m2.membrum = ?2"
            " ORDER BY m1.res_id");
        si (sel != NIHIL)
        {
            scrinium_ligare_textum(sel, I, res_id);
            scrinium_ligare_textum(sel, II, membrum_b);
            dum (scrinium_gradi(sel) == SCRINIUM_ORDO
                && vinculum.mensura == ZEPHYRUM)
            {
                chorda cand = scrinium_columna_textus(sel, 0, pn);
                chorda v = _verbum_vinculi(t, cand, pn);

                si (v.mensura == verbum.mensura
                    && (v.mensura == ZEPHYRUM
                        || memcmp(v.datum, verbum.datum,
                               (memoriae_index)v.mensura)
                            == ZEPHYRUM))
                {
                    vinculum = cand;
                }
            }
            scrinium_finire(sel);
        }
        si (vinculum.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("nexus ignotus (a/verbum/b non congruunt)"),
                VERUM);
            redde;
        }
        actor_l = actor.mensura > ZEPHYRUM
            ? _litterae(pn, actor) : "claude";
        ev.res_id = _litterae(pn, vinculum);
        ev.genus_eventus = "status";
        ev.datum = "{\"novus\":\"solutum\"}";
        ev.actor = actor_l;
        ev.origo = "mcp";
        si (!gesta_scribere(t->mundus, &ev, NIHIL))
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("scriptura solutionis fracta"), VERUM);
            redde;
        }
        _tabulam_scribere(t, pn);
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed, "nexus ");
            chorda_aedificator_appendere_chorda(aed, vinculum);
            chorda_aedificator_appendere_literis(aed, " solutum");
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
        }
        redde;
    }
    alioquin si (_chorda_est(actus, "mutatio"))
    {
        chorda crudum = _arg(argumenta, "datum");
        chorda clavis_m = _arg(argumenta, "clavis");
        chorda valor = _arg(argumenta, "valor");

        genus_eventus = "mutatio";
        si (crudum.mensura > ZEPHYRUM)
        {
            JsonResultus r = json_legere(crudum, pn);

            si (!r.successus || !json_est_objectum(r.radix))
            {
                _textum_respondere(t, pn, effusio, id,
                    _ch("mutatio: datum obiectum JSON"
                        " requiritur"), VERUM);
                redde;
            }
            datum = r.radix;
        }
        alioquin si (clavis_m.mensura > ZEPHYRUM)
        {
            json_objectum_ponere_chorda(datum, clavis_m,
                json_chorda_creare(pn, valor));
        }
        alioquin
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("mutatio: datum aut clavis+valor"), VERUM);
            redde;
        }
    }
    alioquin si (_chorda_est(actus, "remotio"))
    {
        chorda clavis_r = _arg(argumenta, "clavis");

        si (clavis_r.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("remotio: clavis requiritur"), VERUM);
            redde;
        }
        genus_eventus = "remotio";
        json_objectum_ponere(datum, "clavis",
            json_chorda_creare(pn, clavis_r));
    }
    alioquin
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("actus ignotus (nota|status|nexus|denexus|"
                "mutatio|remotio)"), VERUM);
        redde;
    }

    e.res_id = _litterae(pn, res_id);
    e.genus_eventus = genus_eventus;
    e.datum = _litterae(pn, json_scribere(datum, pn));
    e.actor = actor.mensura > ZEPHYRUM
        ? _litterae(pn, actor) : "claude";
    e.origo = "mcp";
    /* in ramo: eventus iudicatus in linea parallela (custodia
     * contra statum RAMI); tabula trunci intacta */
    si (ramus_id.mensura > ZEPHYRUM)
    {
        si (!gesta_in_ramo_scribere(t->mundus, &e,
                _litterae(pn, ramus_id), NIHIL))
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed,
                "scriptura recusata: ");
            chorda_aedificator_appendere_literis(aed,
                gesta_error(t->mundus));
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), VERUM);
            redde;
        }
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);
            chorda status = gesta_res_in_ramo_status(t->mundus,
                _litterae(pn, res_id), _litterae(pn, ramus_id),
                pn);

            chorda_aedificator_appendere_literis(aed, "eventum ");
            chorda_aedificator_appendere_literis(aed,
                genus_eventus);
            chorda_aedificator_appendere_literis(aed,
                " scriptum in ");
            chorda_aedificator_appendere_chorda(aed, res_id);
            chorda_aedificator_appendere_literis(aed,
                " (in ramo ");
            chorda_aedificator_appendere_chorda(aed, ramus_arg);
            si (status.mensura > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(aed,
                    ", status ");
                chorda_aedificator_appendere_chorda(aed, status);
            }
            chorda_aedificator_appendere_literis(aed, ")");
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
        }
        redde;
    }
    si (!gesta_scribere(t->mundus, &e, NIHIL))
    {
        ChordaAedificator* aed = chorda_aedificator_creare(pn,
            CCLVI);

        chorda_aedificator_appendere_literis(aed,
            "scriptura recusata: ");
        chorda_aedificator_appendere_literis(aed,
            gesta_error(t->mundus));
        _textum_respondere(t, pn, effusio, id,
            chorda_aedificator_finire(aed), VERUM);
        redde;
    }
    _tabulam_scribere(t, pn);
    {
        ChordaAedificator* aed = chorda_aedificator_creare(pn,
            CCLVI);
        chorda status = gesta_res_status(t->mundus,
            _litterae(pn, res_id), pn);

        chorda_aedificator_appendere_literis(aed, "eventum ");
        chorda_aedificator_appendere_literis(aed, genus_eventus);
        chorda_aedificator_appendere_literis(aed, " scriptum in ");
        chorda_aedificator_appendere_chorda(aed, res_id);
        si (status.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed,
                " (status ");
            chorda_aedificator_appendere_chorda(aed, status);
            chorda_aedificator_appendere_literis(aed, ")");
        }
        _textum_respondere(t, pn, effusio, id,
            chorda_aedificator_finire(aed), FALSUM);
    }
}

interior vacuum
_tab_quaerere (Tabularium* t, Piscina* pn, JsonValor* id,
    JsonValor* argumenta, FILE* effusio)
{
    chorda textus = _arg(argumenta, "textus");
    chorda genus = _arg(argumenta, "genus");
    chorda status = _arg(argumenta, "status");
    chorda tag = _arg(argumenta, "tag");
    constans character* quaestio;
    Xar* inventa;
    ChordaAedificator* aed;
    i32 i;

    si (textus.mensura == ZEPHYRUM && tag.mensura == ZEPHYRUM)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("textus (aut tag) requiritur"), VERUM);
        redde;
    }
    /* tag = terminus FTS additus (columna corpus tags fert) */
    {
        ChordaAedificator* qa = chorda_aedificator_creare(pn,
            CXXVIII);

        chorda_aedificator_appendere_chorda(qa, textus);
        si (tag.mensura > ZEPHYRUM)
        {
            si (textus.mensura > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(qa, " ");
            }
            chorda_aedificator_appendere_chorda(qa, tag);
        }
        quaestio = _litterae(pn, chorda_aedificator_finire(qa));
    }
    inventa = gesta_quaerere(t->mundus, quaestio,
        genus.mensura > ZEPHYRUM ? _litterae(pn, genus) : NIHIL,
        status.mensura > ZEPHYRUM ? _litterae(pn, status) : NIHIL,
        pn);
    si (inventa == NIHIL)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("apparatus quaestionis fractus"), VERUM);
        redde;
    }
    si (xar_numerus(inventa) == ZEPHYRUM)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("nihil inventum (praefixa 'termin*' adiuvant -"
                " stemmata Latina absunt)"), FALSUM);
        redde;
    }
    aed = chorda_aedificator_creare(pn, 4096);
    per (i = ZEPHYRUM; i < xar_numerus(inventa); i++)
    {
        GestaInventum* inv = (GestaInventum*)xar_obtinere(inventa,
            i);

        si (inv == NIHIL)
        {
            perge;
        }
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed, "\n");
        }
        chorda_aedificator_appendere_chorda(aed, inv->res_id);
        chorda_aedificator_appendere_literis(aed, "  ");
        chorda_aedificator_appendere_chorda(aed, inv->genus);
        chorda_aedificator_appendere_literis(aed, "/");
        chorda_aedificator_appendere_chorda(aed, inv->status);
        chorda_aedificator_appendere_literis(aed, "  ");
        chorda_aedificator_appendere_chorda(aed, inv->titulus);
    }
    _textum_respondere(t, pn, effusio, id,
        chorda_aedificator_finire(aed), FALSUM);
}

interior vacuum
_tab_res (Tabularium* t, Piscina* pn, JsonValor* id,
    JsonValor* argumenta, FILE* effusio)
{
    chorda clavis = _arg(argumenta, "res");
    chorda ramus_arg = _arg(argumenta, "ramus");
    chorda res_id;
    chorda datum;
    JsonValor* st = NIHIL;
    ChordaAedificator* aed;

    si (clavis.mensura == ZEPHYRUM)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("res requiritur (id aut titulus)"), VERUM);
        redde;
    }
    /* in ramo: lectio plicaturae catenae - redditio macra (ancorae/
     * actiones/annales = proiectiones truncales). LEX E2-B1: res =
     * res_id (resolutio tituli truncalis est). */
    si (ramus_arg.mensura > ZEPHYRUM)
    {
        chorda ramus_id = _ramum_solvere(t, ramus_arg, pn);
        chorda status_rami;
        chorda datum_rami;

        si (ramus_id.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("ramus ignotus"), VERUM);
            redde;
        }
        datum_rami = gesta_res_in_ramo_datum(t->mundus,
            _litterae(pn, clavis), _litterae(pn, ramus_id), pn);
        si (datum_rami.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("res in ramo ignota (in ramo res_id"
                    " requiritur - resolutio tituli truncalis"
                    " est)"), VERUM);
            redde;
        }
        status_rami = gesta_res_in_ramo_status(t->mundus,
            _litterae(pn, clavis), _litterae(pn, ramus_id), pn);
        aed = chorda_aedificator_creare(pn, 2048);
        chorda_aedificator_appendere_chorda(aed, clavis);
        chorda_aedificator_appendere_literis(aed, " (in ramo ");
        chorda_aedificator_appendere_chorda(aed, ramus_arg);
        chorda_aedificator_appendere_literis(aed, ")");
        si (status_rami.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed,
                "\nstatus ");
            chorda_aedificator_appendere_chorda(aed, status_rami);
        }
        chorda_aedificator_appendere_literis(aed, "\ndatum ");
        chorda_aedificator_appendere_chorda(aed, datum_rami);
        _textum_respondere(t, pn, effusio, id,
            chorda_aedificator_finire(aed), FALSUM);
        redde;
    }
    {
        b32 ambiguum = FALSUM;

        res_id = _res_solvere(t, clavis, pn, &ambiguum);
        si (ambiguum)
        {
            _ambiguitatem_respondere(t, pn, id, clavis, effusio);
            redde;
        }
    }
    si (res_id.mensura == ZEPHYRUM)
    {
        /* simillima ex titulis rerum */
        ChordaAedificator* miss = chorda_aedificator_creare(pn,
            CCLVI);
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT titulus FROM res");
        Xar* tituli = xar_creare(pn, (i32)magnitudo(chorda));

        chorda_aedificator_appendere_literis(miss, "res ignota '");
        chorda_aedificator_appendere_chorda(miss, clavis);
        chorda_aedificator_appendere_literis(miss, "'");
        si (e != NIHIL && tituli != NIHIL)
        {
            i32 n;

            dum (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                chorda* locus = (chorda*)xar_addere(tituli);

                si (locus != NIHIL)
                {
                    *locus = scrinium_columna_textus(e, 0, pn);
                }
            }
            scrinium_finire(e);
            n = xar_numerus(tituli);
            si (n > ZEPHYRUM)
            {
                chorda* plana = (chorda*)
                    piscina_allocare_ordinatum(pn,
                        (memoriae_index)n * magnitudo(chorda),
                        (memoriae_index)magnitudo(vacuum*));
                SimilitudoFructus fr[III];
                i32 inventa;
                i32 i;

                si (plana != NIHIL)
                {
                    per (i = ZEPHYRUM; i < n; i++)
                    {
                        plana[i] = *(chorda*)xar_obtinere(tituli,
                            i);
                    }
                    inventa = similitudo_optima(clavis, plana, n,
                        fr, III);
                    si (inventa > ZEPHYRUM)
                    {
                        chorda_aedificator_appendere_literis(miss,
                            "; simillima:");
                        per (i = ZEPHYRUM; i < inventa; i++)
                        {
                            chorda_aedificator_appendere_literis(
                                miss, i > ZEPHYRUM ? ", " : " ");
                            chorda_aedificator_appendere_chorda(
                                miss, plana[fr[i].index]);
                        }
                    }
                }
            }
        }
        alioquin si (e != NIHIL)
        {
            scrinium_finire(e);
        }
        _textum_respondere(t, pn, effusio, id,
            chorda_aedificator_finire(miss), VERUM);
        redde;
    }
    datum = gesta_res_datum(t->mundus, _litterae(pn, res_id), pn);
    {
        JsonResultus r = json_legere(datum, pn);

        si (r.successus && json_est_objectum(r.radix))
        {
            st = r.radix;
        }
    }
    aed = chorda_aedificator_creare(pn, 4096);
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT genus, titulus, status, creatum, mutatum"
            " FROM res WHERE res_id = ?");

        si (e != NIHIL)
        {
            scrinium_ligare_textum(e, I, res_id);
            si (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                chorda_aedificator_appendere_chorda(aed,
                    scrinium_columna_textus(e, I, pn));
                chorda_aedificator_appendere_literis(aed, " (");
                chorda_aedificator_appendere_chorda(aed,
                    scrinium_columna_textus(e, 0, pn));
                chorda_aedificator_appendere_literis(aed, ", ");
                chorda_aedificator_appendere_chorda(aed,
                    scrinium_columna_textus(e, II, pn));
                chorda_aedificator_appendere_literis(aed,
                    ")\nres_id ");
                chorda_aedificator_appendere_chorda(aed, res_id);
                chorda_aedificator_appendere_literis(aed,
                    "\ncreatum ");
                chorda_aedificator_appendere_chorda(aed,
                    scrinium_columna_textus(e, III, pn));
                chorda_aedificator_appendere_literis(aed,
                    "  mutatum ");
                chorda_aedificator_appendere_chorda(aed,
                    scrinium_columna_textus(e, IV, pn));
            }
            scrinium_finire(e);
        }
    }
    chorda_aedificator_appendere_literis(aed, "\ndatum ");
    chorda_aedificator_appendere_chorda(aed, datum);
    si (st != NIHIL)
    {
        _ancoras_reddere(t, aed, st, pn);
    }
    /* vincula (superficies passiva K2 - socii per membra; sagitta
     * ex parte socii: b = exiens, a = iniens) */
    {
        Xar* socii = gesta_socii_rei(t->mundus,
            _litterae(pn, res_id), pn);

        si (socii != NIHIL && xar_numerus(socii) > ZEPHYRUM)
        {
            i32 i;

            chorda_aedificator_appendere_literis(aed, "\nnexus:");
            per (i = ZEPHYRUM; i < xar_numerus(socii); i++)
            {
                GestaSocius* s = (GestaSocius*)xar_obtinere(socii,
                    i);
                chorda verbum;

                si (s == NIHIL)
                {
                    perge;
                }
                verbum = _verbum_vinculi(t, s->nexus_res, pn);
                chorda_aedificator_appendere_literis(aed, "\n  ");
                si (_chorda_est(s->pars, "b"))
                {
                    chorda_aedificator_appendere_literis(aed,
                        "--");
                    chorda_aedificator_appendere_chorda(aed,
                        verbum);
                    chorda_aedificator_appendere_literis(aed,
                        "--> ");
                }
                alioquin si (_chorda_est(s->pars, "a"))
                {
                    chorda_aedificator_appendere_literis(aed,
                        "<--");
                    chorda_aedificator_appendere_chorda(aed,
                        verbum);
                    chorda_aedificator_appendere_literis(aed,
                        "-- ");
                }
                alioquin
                {
                    chorda_aedificator_appendere_literis(aed,
                        "(pars ");
                    chorda_aedificator_appendere_chorda(aed,
                        s->pars);
                    chorda_aedificator_appendere_literis(aed,
                        ") ");
                }
                chorda_aedificator_appendere_chorda(aed,
                    _titulus_membri(t, s->membrum, pn));
            }
        }
    }
    /* salus (superficies passiva K2 - querelae si insana) */
    {
        GestaSalus salus;

        si (gesta_salutem_aestimare(t->mundus,
                _litterae(pn, res_id), pn, &salus)
            && !salus.sanus)
        {
            i32 i;

            chorda_aedificator_appendere_literis(aed, "\nsalus:");
            per (i = ZEPHYRUM; i < salus.numerus; i++)
            {
                chorda_aedificator_appendere_literis(aed, "\n  [");
                chorda_aedificator_appendere_literis(aed,
                    salus.querelae[i].gravis
                        ? "erratum" : "cautio");
                chorda_aedificator_appendere_literis(aed, "] ");
                chorda_aedificator_appendere_chorda(aed,
                    salus.querelae[i].nuntius);
            }
        }
    }
    /* affordantiae (K3 - superficies passiva): actiones quae rem
     * NUNC ligare possunt */
    {
        Xar* act = gesta_actiones_rei(t->mundus,
            _litterae(pn, res_id), pn);

        si (act != NIHIL && xar_numerus(act) > ZEPHYRUM)
        {
            i32 i;

            chorda_aedificator_appendere_literis(aed,
                "\nactiones:");
            per (i = ZEPHYRUM; i < xar_numerus(act); i++)
            {
                chorda* c = (chorda*)xar_obtinere(act, i);

                si (c == NIHIL)
                {
                    perge;
                }
                chorda_aedificator_appendere_literis(aed,
                    i > ZEPHYRUM ? ", " : " ");
                chorda_aedificator_appendere_chorda(aed, *c);
            }
        }
    }
    /* annales entis (XV eventa recentissima) */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT genus_eventus, actor, creatum FROM tessellae"
            " WHERE res_id = ? ORDER BY seq DESC LIMIT 15");

        si (e != NIHIL)
        {
            scrinium_ligare_textum(e, I, res_id);
            chorda_aedificator_appendere_literis(aed,
                "\nannales (recentissima primum):");
            dum (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                chorda_aedificator_appendere_literis(aed, "\n  ");
                chorda_aedificator_appendere_chorda(aed,
                    scrinium_columna_textus(e, II, pn));
                chorda_aedificator_appendere_literis(aed, "  ");
                chorda_aedificator_appendere_chorda(aed,
                    scrinium_columna_textus(e, 0, pn));
                chorda_aedificator_appendere_literis(aed, " (");
                chorda_aedificator_appendere_chorda(aed,
                    scrinium_columna_textus(e, I, pn));
                chorda_aedificator_appendere_literis(aed, ")");
            }
            scrinium_finire(e);
        }
    }
    _textum_respondere(t, pn, effusio, id,
        chorda_aedificator_finire(aed), FALSUM);
}

interior vacuum
_tab_census (Tabularium* t, Piscina* pn, JsonValor* id,
    FILE* effusio)
{
    ChordaAedificator* aed = chorda_aedificator_creare(pn, 4096);
    Xar* cg = gesta_census_generum(t->mundus, pn);
    Xar* ct = gesta_census_tagorum(t->mundus, pn);
    character numeri[LXIV];
    i32 i;

    chorda_aedificator_appendere_literis(aed, "genera x status:");
    si (cg != NIHIL)
    {
        per (i = ZEPHYRUM; i < xar_numerus(cg); i++)
        {
            GestaCensusOrdo* o = (GestaCensusOrdo*)xar_obtinere(cg,
                i);

            si (o == NIHIL)
            {
                perge;
            }
            chorda_aedificator_appendere_literis(aed, "\n  ");
            chorda_aedificator_appendere_chorda(aed, o->genus);
            chorda_aedificator_appendere_literis(aed, "/");
            chorda_aedificator_appendere_chorda(aed, o->status);
            sprintf(numeri, "  %d", (int)o->numerus);
            chorda_aedificator_appendere_literis(aed, numeri);
        }
    }
    chorda_aedificator_appendere_literis(aed, "\ntags:");
    si (ct != NIHIL)
    {
        per (i = ZEPHYRUM; i < xar_numerus(ct); i++)
        {
            GestaTagNumerus* tn = (GestaTagNumerus*)xar_obtinere(
                ct, i);

            si (tn == NIHIL)
            {
                perge;
            }
            chorda_aedificator_appendere_literis(aed, "\n  ");
            chorda_aedificator_appendere_chorda(aed, tn->tag);
            sprintf(numeri, "  %d", (int)tn->numerus);
            chorda_aedificator_appendere_literis(aed, numeri);
        }
    }
    sprintf(numeri, "\nseq ultima %d; hwm res %d",
        (int)gesta_seq_ultima(t->mundus),
        (int)gesta_hwm(t->mundus, "res"));
    chorda_aedificator_appendere_literis(aed, numeri);
    si (vigilia_signum_breve(t->vigilia)[ZEPHYRUM] != '\0')
    {
        chorda_aedificator_appendere_literis(aed, "\nbinarium ");
        chorda_aedificator_appendere_literis(aed,
            vigilia_signum_breve(t->vigilia));
    }
    /* salus copiae (superficies passiva K2; percursus plenus -
     * parcum 'salus per sordidas' ad copias maiores) */
    {
        Xar* ins = gesta_insalubres_enumerare(t->mundus, NIHIL,
            pn);

        si (ins != NIHIL)
        {
            sprintf(numeri, "\ninsalubres %d",
                (int)xar_numerus(ins));
            chorda_aedificator_appendere_literis(aed, numeri);
        }
    }
    /* K4: rami activi (proiectio truncalis - res generis ramus) */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT COUNT(*) FROM res WHERE genus = 'ramus'"
            " AND status = 'activus'");

        si (e != NIHIL)
        {
            si (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                sprintf(numeri, "\nrami activi %d",
                    (int)scrinium_columna_numerus(e, 0));
                chorda_aedificator_appendere_literis(aed, numeri);
            }
            scrinium_finire(e);
        }
    }
    _textum_respondere(t, pn, effusio, id,
        chorda_aedificator_finire(aed), FALSUM);
}

/* agere (K3): actio exsequi aut processum incipere. Ligamina
 * valores = res_id AUT titulus exactus (resolutio tabularii ante
 * nucleum; ambiguitas candidatos nominat). RECEPTA SUNT CODEX:
 * definitiones per semen/canalem solum - hoc instrumentum
 * exsequitur, numquam definit (decisio 13). */
interior vacuum
_tab_agere (Tabularium* t, Piscina* pn, JsonValor* id,
    JsonValor* argumenta, FILE* effusio)
{
    chorda actio = _arg(argumenta, "actio");
    chorda processus = _arg(argumenta, "processus");
    chorda lig = _arg(argumenta, "ligamina");
    chorda arg_j = _arg(argumenta, "argumenta");
    chorda actor = _arg(argumenta, "actor");
    constans character* actor_l;
    constans character* lig_l = "{}";
    constans character* arg_l = "{}";

    si ((actio.mensura == ZEPHYRUM)
        == (processus.mensura == ZEPHYRUM))
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("actio AUT processus requiritur (unum ex"
                " ambobus)"), VERUM);
        redde;
    }
    actor_l = actor.mensura > ZEPHYRUM
        ? _litterae(pn, actor) : "claude";
    si (lig.mensura > ZEPHYRUM)
    {
        JsonResultus r = json_legere(lig, pn);
        JsonValor* solutum;
        JsonObjectumIterator iter;
        chorda k;
        JsonValor* v;

        si (!r.successus || !json_est_objectum(r.radix))
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("ligamina: obiectum JSON requiritur"), VERUM);
            redde;
        }
        solutum = json_objectum_creare(pn);
        iter = json_objectum_iterator(r.radix);
        dum (json_objectum_iterator_proxima(&iter, &k, &v))
        {
            si (v != NIHIL && json_est_chorda(v))
            {
                b32 ambiguum = FALSUM;
                chorda rid = _res_solvere(t, json_ad_chorda(v),
                    pn, &ambiguum);

                si (ambiguum)
                {
                    _ambiguitatem_respondere(t, pn, id,
                        json_ad_chorda(v), effusio);
                    redde;
                }
                /* insolutum = crudum manet (porta nuclei honeste
                 * recusat) */
                json_objectum_ponere_chorda(solutum, k,
                    json_chorda_creare(pn,
                        rid.mensura > ZEPHYRUM
                            ? rid : json_ad_chorda(v)));
            }
            alioquin
            {
                json_objectum_ponere_chorda(solutum, k, v);
            }
        }
        lig_l = _litterae(pn, json_scribere(solutum, pn));
    }
    si (arg_j.mensura > ZEPHYRUM)
    {
        arg_l = _litterae(pn, arg_j);
    }

    si (actio.mensura > ZEPHYRUM)
    {
        GestaActioFructus fr;

        si (!gesta_agere(t->mundus, _litterae(pn, actio), lig_l,
                arg_l, actor_l, pn, &fr))
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed,
                "agere fractum: ");
            chorda_aedificator_appendere_literis(aed,
                gesta_error(t->mundus));
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), VERUM);
            redde;
        }
        si (!fr.facta)
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed,
                "actio recusata: ");
            chorda_aedificator_appendere_literis(aed, fr.causa);
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
            redde;
        }
        _tabulam_scribere(t, pn);
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);
            i32 i;

            chorda_aedificator_appendere_literis(aed,
                "actio facta: ");
            chorda_aedificator_appendere_chorda(aed, actio);
            per (i = ZEPHYRUM; i < fr.novae_numerus; i++)
            {
                chorda_aedificator_appendere_literis(aed,
                    "\n  res nova ");
                chorda_aedificator_appendere_chorda(aed,
                    fr.res_novae[i]);
                chorda_aedificator_appendere_literis(aed, "  ");
                chorda_aedificator_appendere_chorda(aed,
                    _titulus_membri(t, fr.res_novae[i], pn));
            }
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
        }
        redde;
    }
    {
        GestaProcessusFructus pf;

        si (!gesta_processum_incipere(t->mundus,
                _litterae(pn, processus), lig_l, arg_l, actor_l,
                pn, &pf))
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed,
                "incipere fractum: ");
            chorda_aedificator_appendere_literis(aed,
                gesta_error(t->mundus));
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), VERUM);
            redde;
        }
        si (!pf.facta)
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed,
                "processus recusatus: ");
            chorda_aedificator_appendere_literis(aed, pf.causa);
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
            redde;
        }
        _tabulam_scribere(t, pn);
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);
            chorda d = gesta_res_datum(t->mundus, pf.instantia,
                pn);

            chorda_aedificator_appendere_literis(aed,
                "processus ");
            chorda_aedificator_appendere_chorda(aed, processus);
            chorda_aedificator_appendere_literis(aed,
                " inceptus: instantia ");
            chorda_aedificator_appendere_literis(aed,
                pf.instantia);
            /* cursus graduum generatorum */
            si (d.mensura > ZEPHYRUM)
            {
                JsonResultus r = json_legere(d, pn);
                JsonValor* cursus = (r.successus
                    && json_est_objectum(r.radix))
                    ? json_objectum_capere(r.radix, "cursus")
                    : NIHIL;

                si (cursus != NIHIL && json_est_objectum(cursus))
                {
                    JsonObjectumIterator iter =
                        json_objectum_iterator(cursus);
                    chorda k;
                    JsonValor* v;

                    dum (json_objectum_iterator_proxima(&iter, &k,
                        &v))
                    {
                        chorda_aedificator_appendere_literis(aed,
                            "\n  ");
                        chorda_aedificator_appendere_chorda(aed,
                            k);
                        chorda_aedificator_appendere_literis(aed,
                            ": ");
                        si (v != NIHIL && json_est_chorda(v))
                        {
                            chorda_aedificator_appendere_chorda(
                                aed, json_ad_chorda(v));
                        }
                    }
                }
            }
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
        }
    }
}

/* ramum solvere: titulus inter ACTIVOS (unicus per legem nuclei)
 * aut res_id directum generis ramus (archaeologia fusorum/
 * abiectorum per res_id). Chorda vacua = ignotus. */
interior chorda
_ramum_solvere (Tabularium* t, chorda clavis, Piscina* pn)
{
    ScriniumEnuntiatum* e;
    chorda fructus;

    fructus.mensura = ZEPHYRUM;
    fructus.datum = NIHIL;
    e = scrinium_praeparare(gesta_scrinium(t->mundus),
        "SELECT res_id FROM res WHERE genus = 'ramus'"
        " AND status = 'activus' AND titulus = ?1");
    si (e != NIHIL)
    {
        scrinium_ligare_textum(e, I, clavis);
        si (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            fructus = scrinium_columna_textus(e, 0, pn);
        }
        scrinium_finire(e);
    }
    si (fructus.mensura > ZEPHYRUM)
    {
        redde fructus;
    }
    e = scrinium_praeparare(gesta_scrinium(t->mundus),
        "SELECT res_id FROM res WHERE genus = 'ramus'"
        " AND res_id = ?1");
    si (e != NIHIL)
    {
        scrinium_ligare_textum(e, I, clavis);
        si (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            fructus = scrinium_columna_textus(e, 0, pn);
        }
        scrinium_finire(e);
    }
    redde fructus;
}

/* numerus eventuum rami */
interior s64
_rami_eventa (Tabularium* t, chorda ramus_id, Piscina* pn)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "SELECT COUNT(*) FROM tessellae WHERE branch_id = ?1");
    s64 n = ZEPHYRUM;

    (vacuum)pn;
    si (e == NIHIL)
    {
        redde ZEPHYRUM;
    }
    scrinium_ligare_textum(e, I, ramus_id);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        n = scrinium_columna_numerus(e, 0);
    }
    scrinium_finire(e);
    redde n;
}

/* ramus (K4 frustum C): vita linearum temporis parallelarum.
 * truncus = veritas, rami = hypothetica; ABICERE GRATIS
 * (flagship - charta E3). Resolutio per titulum inter activos;
 * scripturae/lectiones in ramo per parametrum 'ramus' in
 * addere/gerere/res (lex E2-B1 ibi). */
interior vacuum
_tab_ramus (Tabularium* t, Piscina* pn, JsonValor* id,
    JsonValor* argumenta, FILE* effusio)
{
    chorda actus = _arg(argumenta, "actus");
    chorda titulus = _arg(argumenta, "titulus");
    chorda parens = _arg(argumenta, "parens");
    chorda res_clavis = _arg(argumenta, "res");
    chorda vis = _arg(argumenta, "vis");
    chorda actor = _arg(argumenta, "actor");
    constans character* actor_l = actor.mensura > ZEPHYRUM
        ? _litterae(pn, actor) : "claude";

    si (actus.mensura == ZEPHYRUM)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("actus requiritur (creare|enumerare|comparare|"
                "fundere|abicere)"), VERUM);
        redde;
    }
    si (_chorda_est(actus, "creare"))
    {
        character rid[GESTA_RES_ID_MENSURA];
        chorda parens_id;

        parens_id.mensura = ZEPHYRUM;
        parens_id.datum = NIHIL;
        si (titulus.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("creare: titulus requiritur"), VERUM);
            redde;
        }
        si (parens.mensura > ZEPHYRUM)
        {
            parens_id = _ramum_solvere(t, parens, pn);
            si (parens_id.mensura == ZEPHYRUM)
            {
                _textum_respondere(t, pn, effusio, id,
                    _ch("parens ramus ignotus"), VERUM);
                redde;
            }
        }
        si (!gesta_ramum_creare(t->mundus, _litterae(pn, titulus),
                parens_id.mensura > ZEPHYRUM
                    ? _litterae(pn, parens_id) : "",
                actor_l, pn, rid))
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed,
                "creare recusatum: ");
            chorda_aedificator_appendere_literis(aed,
                gesta_error(t->mundus));
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), VERUM);
            redde;
        }
        _tabulam_scribere(t, pn);
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed, "ramus ");
            chorda_aedificator_appendere_literis(aed, rid);
            chorda_aedificator_appendere_literis(aed,
                " creatus: ");
            chorda_aedificator_appendere_chorda(aed, titulus);
            chorda_aedificator_appendere_literis(aed,
                " (scripturae: parametrum 'ramus' in addere/"
                "gerere; abicere gratis)");
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
        }
        redde;
    }
    si (_chorda_est(actus, "enumerare"))
    {
        Xar* rami = gesta_ramos_enumerare(t->mundus, pn);
        ChordaAedificator* aed = chorda_aedificator_creare(pn,
            2048);
        character numeri[LXIV];
        i32 i;

        si (rami == NIHIL || xar_numerus(rami) == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("rami nulli"), FALSUM);
            redde;
        }
        chorda_aedificator_appendere_literis(aed, "rami:");
        per (i = ZEPHYRUM; i < xar_numerus(rami); i++)
        {
            GestaRamusOrdo* o = (GestaRamusOrdo*)xar_obtinere(
                rami, i);

            si (o == NIHIL)
            {
                perge;
            }
            chorda_aedificator_appendere_literis(aed, "\n  ");
            chorda_aedificator_appendere_chorda(aed, o->titulus);
            chorda_aedificator_appendere_literis(aed, " [");
            chorda_aedificator_appendere_chorda(aed, o->status);
            chorda_aedificator_appendere_literis(aed, "]");
            sprintf(numeri, "  punctum %d  eventa %d",
                (int)o->punctum,
                (int)_rami_eventa(t, o->res_id, pn));
            chorda_aedificator_appendere_literis(aed, numeri);
        }
        _textum_respondere(t, pn, effusio, id,
            chorda_aedificator_finire(aed), FALSUM);
        redde;
    }
    si (_chorda_est(actus, "comparare"))
    {
        chorda ramus_id;
        chorda res_id;
        chorda st_trunci;
        chorda st_rami;
        chorda d_rami;
        ChordaAedificator* aed;

        si (titulus.mensura == ZEPHYRUM
            || res_clavis.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("comparare: titulus (rami) et res"
                    " requiruntur"), VERUM);
            redde;
        }
        ramus_id = _ramum_solvere(t, titulus, pn);
        si (ramus_id.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("ramus ignotus"), VERUM);
            redde;
        }
        /* res: res_id aut titulus TRUNCALIS (dimidium truncale
         * comparationis titulum sine periculo solvit) */
        res_id = _res_solvere(t, res_clavis, pn, NIHIL);
        si (res_id.mensura == ZEPHYRUM)
        {
            res_id = res_clavis;
        }
        st_trunci = gesta_res_status(t->mundus,
            _litterae(pn, res_id), pn);
        st_rami = gesta_res_in_ramo_status(t->mundus,
            _litterae(pn, res_id), _litterae(pn, ramus_id), pn);
        d_rami = gesta_res_in_ramo_datum(t->mundus,
            _litterae(pn, res_id), _litterae(pn, ramus_id), pn);
        aed = chorda_aedificator_creare(pn, 2048);
        chorda_aedificator_appendere_literis(aed, "truncus: ");
        si (gesta_res_datum(t->mundus, _litterae(pn, res_id),
                pn).mensura == ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed, "(absens)");
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(aed, "[");
            chorda_aedificator_appendere_chorda(aed, st_trunci);
            chorda_aedificator_appendere_literis(aed, "] ");
            chorda_aedificator_appendere_chorda(aed,
                gesta_res_datum(t->mundus, _litterae(pn, res_id),
                    pn));
        }
        chorda_aedificator_appendere_literis(aed, "\nramus ");
        chorda_aedificator_appendere_chorda(aed, titulus);
        chorda_aedificator_appendere_literis(aed, ": ");
        si (d_rami.mensura == ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed, "(absens)");
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(aed, "[");
            chorda_aedificator_appendere_chorda(aed, st_rami);
            chorda_aedificator_appendere_literis(aed, "] ");
            chorda_aedificator_appendere_chorda(aed, d_rami);
        }
        _textum_respondere(t, pn, effusio, id,
            chorda_aedificator_finire(aed), FALSUM);
        redde;
    }
    si (_chorda_est(actus, "fundere"))
    {
        chorda ramus_id;
        GestaFusioFructus ff;
        b32 vi = _chorda_est(vis, "verum")
            || _chorda_est(vis, "true") || _chorda_est(vis, "ita");

        si (titulus.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("fundere: titulus requiritur"), VERUM);
            redde;
        }
        ramus_id = _ramum_solvere(t, titulus, pn);
        si (ramus_id.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("ramus ignotus"), VERUM);
            redde;
        }
        si (!gesta_ramum_fundere(t->mundus,
                _litterae(pn, ramus_id), vi, actor_l, pn, &ff))
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed,
                "fusio fracta: ");
            chorda_aedificator_appendere_literis(aed,
                gesta_error(t->mundus));
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), VERUM);
            redde;
        }
        si (!ff.fusa)
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                2048);
            character numeri[LXIV];

            chorda_aedificator_appendere_literis(aed,
                "fusio recusata: ");
            chorda_aedificator_appendere_literis(aed, ff.causa);
            si (ff.confligentia_numerus > ZEPHYRUM)
            {
                Xar* conf = gesta_confligentia(t->mundus,
                    _litterae(pn, ramus_id), pn);
                i32 i;

                sprintf(numeri, "\nconfligentiae %d:",
                    (int)ff.confligentia_numerus);
                chorda_aedificator_appendere_literis(aed, numeri);
                per (i = ZEPHYRUM;
                     conf != NIHIL && i < xar_numerus(conf); i++)
                {
                    GestaConfligentia* c = (GestaConfligentia*)
                        xar_obtinere(conf, i);

                    si (c == NIHIL)
                    {
                        perge;
                    }
                    chorda_aedificator_appendere_literis(aed,
                        "\n  ");
                    chorda_aedificator_appendere_chorda(aed,
                        c->res_id);
                    chorda_aedificator_appendere_literis(aed,
                        "\n    truncus: ");
                    chorda_aedificator_appendere_chorda(aed,
                        c->status_trunci);
                    chorda_aedificator_appendere_literis(aed,
                        "\n    ramus:   ");
                    chorda_aedificator_appendere_chorda(aed,
                        c->status_rami);
                }
                chorda_aedificator_appendere_literis(aed,
                    "\n(vis: \"verum\" cogit)");
            }
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
            redde;
        }
        _tabulam_scribere(t, pn);
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);
            character numeri[LXIV];

            chorda_aedificator_appendere_literis(aed,
                "fusio perfecta: ");
            sprintf(numeri, "%d eventa in truncum copiata",
                (int)ff.copiata);
            chorda_aedificator_appendere_literis(aed, numeri);
            chorda_aedificator_appendere_literis(aed,
                " (origo merge:");
            chorda_aedificator_appendere_chorda(aed, titulus);
            chorda_aedificator_appendere_literis(aed,
                ", attributio servata)");
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
        }
        redde;
    }
    si (_chorda_est(actus, "abicere"))
    {
        chorda ramus_id;

        si (titulus.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("abicere: titulus requiritur"), VERUM);
            redde;
        }
        ramus_id = _ramum_solvere(t, titulus, pn);
        si (ramus_id.mensura == ZEPHYRUM)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("ramus ignotus"), VERUM);
            redde;
        }
        si (!gesta_ramum_abicere(t->mundus,
                _litterae(pn, ramus_id), actor_l))
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed,
                "abicere recusatum: ");
            chorda_aedificator_appendere_literis(aed,
                gesta_error(t->mundus));
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), VERUM);
            redde;
        }
        _tabulam_scribere(t, pn);
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                CCLVI);

            chorda_aedificator_appendere_literis(aed, "ramus ");
            chorda_aedificator_appendere_chorda(aed, titulus);
            chorda_aedificator_appendere_literis(aed,
                " abiectus (acta manent, lectio libera -"
                " fingere audacter, abicere gratis)");
            _textum_respondere(t, pn, effusio, id,
                chorda_aedificator_finire(aed), FALSUM);
        }
        redde;
    }
    _textum_respondere(t, pn, effusio, id,
        _ch("actus ignotus (creare|enumerare|comparare|fundere|"
            "abicere)"), VERUM);
}

/* ==================================================
 * protocollum (sceletum legati)
 * ================================================== */

/* fabrum instrumenti: argumenta usque ad VI, necessaria signata */
nomen structura {
    constans character* titulus;
    constans character* descriptio;
    b32                 necessarium;
} TabArgumentum;

interior JsonValor*
_instrumentum (Piscina* pn, constans character* titulus,
    constans character* descriptio,
    constans TabArgumentum* argumenta, i32 numerus)
{
    JsonValor* instrumentum = json_objectum_creare(pn);
    JsonValor* schema = json_objectum_creare(pn);
    JsonValor* proprietates = json_objectum_creare(pn);
    JsonValor* necessaria = json_tabulatum_creare(pn);
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        JsonValor* arg = json_objectum_creare(pn);

        json_objectum_ponere(arg, "type",
            json_chorda_creare_literis(pn, "string"));
        json_objectum_ponere(arg, "description",
            json_chorda_creare_literis(pn,
                argumenta[i].descriptio));
        json_objectum_ponere(proprietates, argumenta[i].titulus,
            arg);
        si (argumenta[i].necessarium)
        {
            json_tabulatum_addere(necessaria,
                json_chorda_creare_literis(pn,
                    argumenta[i].titulus));
        }
    }
    json_objectum_ponere(schema, "type",
        json_chorda_creare_literis(pn, "object"));
    json_objectum_ponere(schema, "properties", proprietates);
    json_objectum_ponere(schema, "required", necessaria);
    json_objectum_ponere(instrumentum, "name",
        json_chorda_creare_literis(pn, titulus));
    json_objectum_ponere(instrumentum, "description",
        json_chorda_creare_literis(pn, descriptio));
    json_objectum_ponere(instrumentum, "inputSchema", schema);
    redde instrumentum;
}

interior vacuum
_toolslist_tractare (Piscina* pn, JsonValor* id, FILE* effusio)
{
    JsonValor* resultatum = json_objectum_creare(pn);
    JsonValor* instrumenta = json_tabulatum_creare(pn);
    interior constans TabArgumentum ARG_ADDERE[] = {
        { "genus", "quaestio|parcum|decretum|nota|desideratum",
          VERUM },
        { "titulus", "titulus brevis entis", VERUM },
        { "corpus", "textus corporis (quaesibilis)", FALSUM },
        { "tags", "tags commatibus separata", FALSUM },
        { "ancorae", "tabulatum JSON: [{\"genus\":\"symbolum|via\","
          "\"scopus\":\"...\",\"sigillum\":\"hex?\"}]", FALSUM },
        { "actor", "fran|claude|machina (ordinarius claude)",
          FALSUM },
        { "ramus", "titulus rami activi - creatio in ramo (trunco"
          " invisibilis usque ad fusionem)", FALSUM }
    };
    interior constans TabArgumentum ARG_GERERE[] = {
        { "res", "res_id aut titulus exactus (in ramo: res_id"
          " SOLUM)", VERUM },
        { "actus", "nota|status|nexus|denexus|mutatio|remotio",
          VERUM },
        { "textus", "pro nota", FALSUM },
        { "novus", "pro statu (status novus)", FALSUM },
        { "verbum", "pro nexu (e.g. impedit)", FALSUM },
        { "alterum", "pro nexu (res altera: id aut titulus)",
          FALSUM },
        { "clavis", "pro mutatione/remotione", FALSUM },
        { "valor", "pro mutatione simplici (chorda)", FALSUM },
        { "datum", "pro mutatione: obiectum JSON crudum", FALSUM },
        { "actor", "fran|claude|machina (ordinarius claude)",
          FALSUM },
        { "ramus", "titulus rami activi - eventus in ramo"
          " (custodia contra statum rami)", FALSUM }
    };
    interior constans TabArgumentum ARG_QUAERERE[] = {
        { "textus", "quaestio FTS (praefixa 'termin*')", VERUM },
        { "genus", "filtrum generis", FALSUM },
        { "status", "filtrum statûs", FALSUM },
        { "tag", "filtrum tagi (terminus FTS additus)", FALSUM }
    };
    interior constans TabArgumentum ARG_RES[] = {
        { "res", "res_id aut titulus exactus (in ramo: res_id"
          " SOLUM)", VERUM },
        { "ramus", "titulus rami - lectio plicaturae ramalis",
          FALSUM }
    };
    interior constans TabArgumentum ARG_RAMUS[] = {
        { "actus", "creare|enumerare|comparare|fundere|abicere",
          VERUM },
        { "titulus", "titulus rami (creare/comparare/fundere/"
          "abicere)", FALSUM },
        { "parens", "ramus parens (creare; absens = truncus)",
          FALSUM },
        { "res", "res_id comparandae (comparare)", FALSUM },
        { "vis", "\"verum\" = confligentiaas cogere (fundere)",
          FALSUM },
        { "actor", "fran|claude|machina (ordinarius claude)",
          FALSUM }
    };
    interior constans TabArgumentum ARG_AGERE[] = {
        { "actio", "titulus actionis exsequendae (aut"
          " 'processus' adhibe - unum ex ambobus)", FALSUM },
        { "processus", "titulus processus incipiendi", FALSUM },
        { "ligamina", "obiectum JSON: titulus opis -> res_id aut"
          " titulus exactus", FALSUM },
        { "argumenta", "obiectum JSON: titulus -> valor", FALSUM },
        { "actor", "fran|claude|machina (ordinarius claude)",
          FALSUM }
    };

    json_tabulatum_addere(instrumenta, _instrumentum(pn, "addere",
        "Rem novam creare (quaestio/parcum/decretum/nota/"
        "desideratum) cum tags et ancoris optionalibus.",
        ARG_ADDERE, VII));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "gerere",
        "Eventum unum in rem exsistentem scribere: nota, status,"
        " nexus/denexus (ligamina), mutatio, remotio. Violationes"
        " machinae notantur, non obstant.",
        ARG_GERERE, XI));
    json_tabulatum_addere(instrumenta, _instrumentum(pn,
        "quaerere",
        "Quaestio FTS super statum materializatum (titulus/corpus/"
        "tags/notae). Idioma Latinum: praefixa 'parsur*'.",
        ARG_QUAERERE, IV));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "res",
        "Rem unam reddere: status + datum + ancorae (resolutae per"
        " indicem; CAUTIO si inresolutae) + actiones affordatae +"
        " annales recentes.",
        ARG_RES, II));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "census",
        "Census: genera x status, tags, seq/hwm.",
        NIHIL, ZEPHYRUM));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "agere",
        "Actionem exsequi (porta obstat, effectus atomice - fascis"
        " unus) aut processum incipere (instantia definitionem"
        " photographat; opera generantur; perfectio operum"
        " processum sponte provehit). Recepta sunt codex -"
        " definitiones per semen solum.",
        ARG_AGERE, V));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "ramus",
        "K4: lineae temporis parallelae. creare (furca a trunco"
        " aut parente), enumerare, comparare (res in trunco et"
        " ramo), fundere (replay in truncum, confligentia obstat"
        " nisi vis), ABICERE (flagship - gratis, acta manent)."
        " truncus = veritas, rami = hypothetica; fingere audacter."
        " Scripturae/lectiones in ramo: parametrum 'ramus' in"
        " addere/gerere/res (ibi res_id requiritur).",
        ARG_RAMUS, VI));
    json_objectum_ponere(resultatum, "tools", instrumenta);
    _respondere(effusio, tabellarius_responsum(pn, id,
        resultatum));
}

/* seminatio idempotens (INTENTIO C decisio 2; semen v2 K2 Q9) */
interior vacuum
_seminare (Tabularium* t, Piscina* pn)
{
    i32 i;

    per (i = ZEPHYRUM; i < SEMINA_NUMERUS; i++)
    {
        chorda gd = gesta_genus_datum(t->mundus,
            SEMINA_GENERUM[i].titulus, pn);

        si (gd.mensura == ZEPHYRUM)
        {
            GestaEventum e;

            e.res_id = NIHIL;
            e.genus_eventus = "definitio-generis";
            e.datum = SEMINA_GENERUM[i].datum;
            e.actor = "machina";
            e.origo = "seminatio";
            (vacuum)gesta_scribere(t->mundus, &e, NIHIL);
        }
    }
    /* semen v2: generibus TABULAE sine 'attributa' schema additur
     * (definitio currens + attributa -> emendatio integra).
     * SCOPUS: SEMINA_BOARD_NUMERUS - genera K3 (opus/actio/
     * processus) attributa tabulae NON accipiunt (E2-B2) */
    per (i = ZEPHYRUM; i < SEMINA_BOARD_NUMERUS; i++)
    {
        chorda gd = gesta_genus_datum(t->mundus,
            SEMINA_GENERUM[i].titulus, pn);
        JsonResultus r;

        si (gd.mensura == ZEPHYRUM)
        {
            perge;
        }
        r = json_legere(gd, pn);
        si (!r.successus || !json_est_objectum(r.radix)
            || json_objectum_habet(r.radix, "attributa"))
        {
            perge;
        }
        {
            JsonResultus ra = json_legere_literis(ATTRIBUTA_V2,
                pn);
            GestaEventum e;

            si (!ra.successus)
            {
                perge;
            }
            json_objectum_ponere(r.radix, "attributa", ra.radix);
            e.res_id = NIHIL;
            e.genus_eventus = "emendatio-generis";
            e.datum = _litterae(pn, json_scribere(r.radix, pn));
            e.actor = "machina";
            e.origo = "seminatio";
            (vacuum)gesta_scribere(t->mundus, &e, NIHIL);
        }
    }
    {
        chorda d = gesta_res_datum(t->mundus,
            "vocabularium-tagorum", pn);

        si (d.mensura == ZEPHYRUM)
        {
            GestaEventum e;

            e.res_id = "vocabularium-tagorum";
            e.genus_eventus = "creatio";
            e.datum = VOCABULARIUM_TAGORUM;
            e.actor = "machina";
            e.origo = "seminatio";
            (vacuum)gesta_scribere(t->mundus, &e, NIHIL);
        }
    }
}

interior vacuum
_initialize_tractare (Tabularium* t, Piscina* pn, JsonValor* id,
    JsonValor* params, FILE* effusio)
{
    si (t->initiatum)
    {
        _respondere(effusio, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_PETITIO_INVALIDA, "iam initiatum"));
        redde;
    }
    t->mundus = gesta_aperire(t->piscina, t->via_scrinii,
        t->via_annalium);
    si (t->mundus == NIHIL)
    {
        _respondere(effusio, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_INTERNUS,
            "scrinium aperiri non potuit"));
        redde;
    }
    si (!gesta_plicare(t->mundus))
    {
        _respondere(effusio, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_INTERNUS, "plicatura fracta"));
        redde;
    }
    _seminare(t, pn);
    _tabulam_scribere(t, pn);
    t->initiatum = VERUM;
    {
        JsonValor* resultatum = json_objectum_creare(pn);
        JsonValor* caps = json_objectum_creare(pn);
        JsonValor* instrumenta_caps = json_objectum_creare(pn);
        JsonValor* servus = json_objectum_creare(pn);
        JsonValor* versio_v = params != NIHIL
            ? json_objectum_capere(params, "protocolVersion")
            : NIHIL;
        chorda versio = json_ad_chorda(versio_v);

        json_objectum_ponere(resultatum, "protocolVersion",
            versio.mensura > ZEPHYRUM
                ? json_chorda_creare(pn, versio)
                : json_chorda_creare_literis(pn,
                      TABULARII_PROTOCOLLUM_PINNATUM));
        json_objectum_ponere(caps, "tools", instrumenta_caps);
        json_objectum_ponere(resultatum, "capabilities", caps);
        json_objectum_ponere(servus, "name",
            json_chorda_creare_literis(pn, "tabularii"));
        /* versio = signum binarii (K2.1) - "quale binarium es?"
         * uno vocamine; "0" si vigilia tacet */
        json_objectum_ponere(servus, "version",
            json_chorda_creare_literis(pn,
                vigilia_signum_breve(t->vigilia)[ZEPHYRUM] != '\0'
                    ? vigilia_signum_breve(t->vigilia) : "0"));
        json_objectum_ponere(resultatum, "serverInfo", servus);
        json_objectum_ponere(resultatum, "instructions",
            json_chorda_creare_literis(pn, TABULARII_DOCTRINA));
        _respondere(effusio, tabellarius_responsum(pn, id,
            resultatum));
    }
}

interior b32
_methodus_est (chorda methodus, constans character* titulus)
{
    redde _chorda_est(methodus, titulus);
}

interior vacuum
_toolscall_tractare (Tabularium* t, Piscina* pn, JsonValor* id,
    JsonValor* params, FILE* effusio)
{
    chorda titulus;
    JsonValor* argumenta;

    si (params == NIHIL)
    {
        _respondere(effusio, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_PARAMETRA_INVALIDA,
            "params desunt"));
        redde;
    }
    (vacuum)vigilia_inspicere(t->vigilia, pn);
    titulus = json_ad_chorda(json_objectum_capere(params, "name"));
    argumenta = json_objectum_capere(params, "arguments");
    si (_chorda_est(titulus, "addere"))
    {
        _tab_addere(t, pn, id, argumenta, effusio);
    }
    alioquin si (_chorda_est(titulus, "gerere"))
    {
        _tab_gerere(t, pn, id, argumenta, effusio);
    }
    alioquin si (_chorda_est(titulus, "quaerere"))
    {
        _tab_quaerere(t, pn, id, argumenta, effusio);
    }
    alioquin si (_chorda_est(titulus, "res"))
    {
        _tab_res(t, pn, id, argumenta, effusio);
    }
    alioquin si (_chorda_est(titulus, "census"))
    {
        _tab_census(t, pn, id, effusio);
    }
    alioquin si (_chorda_est(titulus, "agere"))
    {
        _tab_agere(t, pn, id, argumenta, effusio);
    }
    alioquin si (_chorda_est(titulus, "ramus"))
    {
        _tab_ramus(t, pn, id, argumenta, effusio);
    }
    alioquin
    {
        _respondere(effusio, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_PARAMETRA_INVALIDA,
            "instrumentum ignotum"));
    }
}

b32
tabularium_tractare (Tabularium* t, Piscina* pn, chorda corpus,
    FILE* effusio)
{
    TabellariusNuntius n;

    si (t == NIHIL || pn == NIHIL)
    {
        redde FALSUM;
    }
    n = tabellarius_nuntium_legere(corpus, pn);
    si (n.genus == TABELLARIUS_PRAVUM)
    {
        si (n.radix == NIHIL)
        {
            _respondere(effusio, tabellarius_errorem(pn, NIHIL,
                TABELLARIUS_ERROR_ANALYSIS, "analysis fracta"));
        }
        alioquin
        {
            _respondere(effusio, tabellarius_errorem(pn, n.id,
                TABELLARIUS_ERROR_PETITIO_INVALIDA,
                "petitio invalida"));
        }
        redde VERUM;
    }
    si (n.genus == TABELLARIUS_RESPONSUM)
    {
        redde VERUM;   /* numquam petimus */
    }
    si (n.genus == TABELLARIUS_PETITIO)
    {
        si (_methodus_est(n.methodus, "initialize"))
        {
            _initialize_tractare(t, pn, n.id, n.params, effusio);
        }
        alioquin si (_methodus_est(n.methodus, "ping"))
        {
            _respondere(effusio, tabellarius_responsum(pn, n.id,
                json_objectum_creare(pn)));
        }
        alioquin si (!t->initiatum)
        {
            _respondere(effusio, tabellarius_errorem(pn, n.id,
                TABELLARIUS_ERROR_NONDUM_INITIATUM,
                "nondum initiatum"));
        }
        alioquin si (_methodus_est(n.methodus, "tools/list"))
        {
            _toolslist_tractare(pn, n.id, effusio);
        }
        alioquin si (_methodus_est(n.methodus, "tools/call"))
        {
            _toolscall_tractare(t, pn, n.id, n.params, effusio);
        }
        alioquin
        {
            _respondere(effusio, tabellarius_errorem(pn, n.id,
                TABELLARIUS_ERROR_METHODUS_IGNOTA,
                "methodus ignota"));
        }
        redde VERUM;
    }
    /* nuntiationes tacite omissae */
    redde VERUM;
}

Tabularium*
tabularium_creare (Piscina* piscina,
    constans TabulariumConfiguratio* cfg)
{
    Tabularium* t;

    si (piscina == NIHIL || cfg == NIHIL
        || cfg->via_scrinii == NIHIL
        || cfg->via_annalium == NIHIL)
    {
        redde NIHIL;
    }
    t = (Tabularium*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(Tabularium),
        (memoriae_index)magnitudo(vacuum*));
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    memset(t, ZEPHYRUM, magnitudo(Tabularium));
    t->piscina = piscina;
    t->radix = cfg->radix != NIHIL
        ? _litterae(piscina, _ch(cfg->radix)) : "";
    t->via_scrinii = _litterae(piscina, _ch(cfg->via_scrinii));
    t->via_annalium = _litterae(piscina, _ch(cfg->via_annalium));
    t->via_nexus = cfg->via_nexus != NIHIL
        ? _litterae(piscina, _ch(cfg->via_nexus)) : NIHIL;
    t->via_tabulae = cfg->via_tabulae != NIHIL
        ? _litterae(piscina, _ch(cfg->via_tabulae)) : NIHIL;
    /* vigilia (lib/vigilia): configuratio manca = quieta */
    {
        VigiliaConfiguratio vc;

        vc.signum = cfg->signum;
        vc.via_binarii = cfg->via_binarii;
        vc.via_manifesti = cfg->via_manifesti;
        t->vigilia = vigilia_creare(piscina, &vc);
    }
    redde t;
}

integer
tabularium_currere (constans TabulariumConfiguratio* cfg,
    FILE* intra, FILE* extra)
{
    Piscina* piscina = piscina_generare_dynamicum("tabularium",
        268435456);
    Tabularium* t;
    integer exitus = I;

    si (piscina == NIHIL)
    {
        redde I;
    }
    t = tabularium_creare(piscina, cfg);
    si (t == NIHIL)
    {
        piscina_destruere(piscina);
        redde I;
    }
    per (;;)
    {
        Piscina* pn = piscina_generare_dynamicum(
            "tabularium_nuntius", LXIV * 1024);
        b32 finitus = FALSUM;
        chorda corpus;

        si (pn == NIHIL)
        {
            frange;
        }
        corpus = tabellarius_lineam_legere(intra, pn, &finitus);
        si (finitus)
        {
            piscina_destruere(pn);
            exitus = ZEPHYRUM;   /* cliens claudit = ordinatus */
            frange;
        }
        (vacuum)tabularium_tractare(t, pn, corpus, extra);
        piscina_destruere(pn);
    }
    si (t->mundus != NIHIL)
    {
        gesta_claudere(t->mundus);
    }
    piscina_destruere(piscina);
    redde exitus;
}
