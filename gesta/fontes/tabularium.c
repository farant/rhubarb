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
#include "filum.h"
#include "via.h"
#include "iter_directoria.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>   /* horologium politicae tacendi (sutura POSIX) */

#define TABULARII_PROTOCOLLUM_PINNATUM "2025-06-18"

/* doctrina: in contextum agentis VERBATIM cadit (inventum legati) -
 * contractus scribae hic VIVIT ut sessiones futurae eum hereditent */
interior constans character* constans TABULARII_DOCTRINA =
    "TABULARIUM: acta rerum repositorii (quaestiones/parca/decreta/"
    "notae/desiderata) super acta gestarum. CONTRACTUS SCRIBAE: "
    "Claude notas/parca/decreta per debriefs SPONTE scribit (sine "
    "confirmatione singula - recensio per quaestiones, non portas). "
    "addere {genus, titulus, corpus?, tags? (commatibus), ancorae? "
    "(JSON), actor?, origo?} = res nova (similia FTS in responso - "
    "custos duplicationum). gerere {res (id, praefixum ULID"
    " inambiguum, aut titulus), "
    "actus: nota|ictus|status|nexus|denexus|mutatio|remotio, "
    "textus?/"
    "novus?/verbum?/alterum?/clavis?/valor?/datum?/origo?} = "
    "eventus unus. "
    "quaerere {textus, genus?, status?, tag?} = FTS (idioma "
    "Latinum: praefixa 'parsur*' - stemmata Anglica sola). res "
    "{res, breviter?} = status + ancorae (CAUTIO si inresolutae) + "
    "actiones affordatae + annales; breviter \"verum\" = compendium "
    "(corpus + notae ultimae III + actiones, sine dato crudo). "
    "census {} = genera x status + tags + res saepissime ICTAE "
    "(apertae, ordine ictuum - pretium MENSURATUM, non "
    "aestimatum). ictus: gerere {actus:ictus, textus?} = "
    "'haec res me ITERUM momordit' - signum quod campum "
    "prioritatis vincit quia a rebus ipsis ponitur. "
    "acta {quantum? (XXV), genus?, actor?} = cauda fluminis"
    " eventuum trans res omnes (truncus, recentissima primum) -"
    " recensio scribae (quid hodie scriptum est). "
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
    "(residente absente): ./gesta/frigida.sh "
    "CONTRACTUS CORRESPONDENTIS (mundi scriptorum - forum): "
    "salutatio NOVA -> acta {ab_lecto} in calefactione; LECTUM "
    "EST PROMISSUM - responde quod aperis, opera in tabulam "
    "converte (cursor non iterum monebit); articulus adveniens = "
    "epistula lectorio, lege totum et responde; pipa solum quod "
    "LECTOREM vult, non retractorem - tacere licitum, plerumque "
    "rectum; forum = stratum opinionum repositorii (codex = quid "
    "EST, tabula = quid DECISUM, worklog = quid INVENTUM, forum = "
    "quid PENSATUR) - quaere id in labore, noli percurrere in "
    "calefactione; annales fori in fine sessionis committe; "
    "signatura = vox tua propria (nomen exemplaris) - posteris "
    "scribis.";

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
      "\"typus\":\"numerus\"}],\"reducer\":\"ordinarius\"}" },
    /* ---- semen v5 (F0 forum): genera mundi scriptorum - ambo
     * mundi ea vident (tabula in machina cocta; acceptum spec-v2
     * par III). Post SEMINA_BOARD_NUMERUS: attributa INLINE
     * necessaria (fusio ATTRIBUTA_V2 hos non tangit). Fila per
     * nexum 'respondet-ad' (verbum liberum - mutationes nullae) */
    { "articulus",
      "{\"titulus\":\"articulus\",\"status_initialis\":"
      "\"conditum\",\"machina\":[[\"conditum\",\"publicatum\"],"
      "[\"publicatum\",\"retractum\"]],\"attributa\":"
      "[{\"titulus\":\"titulus\",\"typus\":\"textus\","
      "\"necessarium\":true},{\"titulus\":\"corpus\",\"typus\":"
      "\"textus\"},{\"titulus\":\"tags\",\"typus\":"
      "\"tabulatum\"}],\"reducer\":\"ordinarius\"}" },
    { "commentarium",
      "{\"titulus\":\"commentarium\",\"attributa\":[{\"titulus\":"
      "\"titulus\",\"typus\":\"textus\",\"necessarium\":true},"
      "{\"titulus\":\"corpus\",\"typus\":\"textus\"}],"
      "\"reducer\":\"ordinarius\"}" },
    { "pipatum",
      "{\"titulus\":\"pipatum\",\"attributa\":[{\"titulus\":"
      "\"titulus\",\"typus\":\"textus\",\"necessarium\":true},"
      "{\"titulus\":\"corpus\",\"typus\":\"textus\"}],"
      "\"reducer\":\"ordinarius\"}" },
    /* ---- semen v6 (genera G0): meta-genus definitionis - genera
     * ab usore per app definita. Ens definitionis plicatum ->
     * ordo registri (usor=1, gesta.c _definitionem_proicere);
     * campi = [{clavis, monstrans, typus: textus|area|dies|annus|
     * numerus|relatio, ad?, cardinalitas?}], campus_tituli,
     * ordo_ordinarius {campus, directio} - forma libera, iudicium
     * ad scripturam (_campos_iudicare). clavis immutabilis,
     * emendatio additiva sola. ---- */
    { "definitio",
      "{\"titulus\":\"definitio\",\"attributa\":[{\"titulus\":"
      "\"titulus\",\"typus\":\"textus\",\"necessarium\":true},"
      "{\"titulus\":\"clavis\",\"typus\":\"textus\","
      "\"necessarium\":true},{\"titulus\":\"campi\",\"typus\":"
      "\"tabulatum\"},{\"titulus\":\"campus_tituli\",\"typus\":"
      "\"textus\"}],\"reducer\":\"ordinarius\"}" }
};

/* scopus fusionis v2 (genera tabulae + nexus); genera K3 infra
 * attributa propria ferunt (emendatio E2-B2) */
#define SEMINA_BOARD_NUMERUS VI
#define SEMINA_NUMERUS XIV

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

/* frustum D: ordines indicum identitatum et citationum ex codice */
nomen structura {
    chorda              nid;    /* ULID plenus ex indice */
    constans character* sedes;  /* "via:linea (genus unitatis)" */
} _NidOrdo;

nomen structura {
    chorda              praefixum; /* citatio res= ex codice */
    constans character* sedes;     /* "via:linea" */
} _CitatioOrdo;

structura Tabularium {
    Piscina*     piscina;
    GestaMundus* mundus;
    b32          initiatum;
    constans character* radix;
    constans character* via_scrinii;
    constans character* via_annalium;
    constans character* via_nexus;
    constans character* via_tabulae;
    constans character* via_entitatum;
    /* index ancorarum (pigre; INTENTIO C decisio 3) */
    b32             index_temptatus;
    TabulaDispersa* sedes_index;     /* titulus -> character* "via:linea" */
    Xar*            sedes_tituli;    /* chorda (valore) */
    /* identitates nid + citationes ex codice (frustum D, pigre) */
    constans character* via_identitatum;
    constans character* via_citationum;
    b32             nides_temptatae;
    Xar*            nides;           /* _NidOrdo (valore) */
    b32             citationes_temptatae;
    Xar*            citationes;      /* _CitatioOrdo (valore) */
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

    si (t != NIHIL)
    {
        /* politica tacendi (2026-07-17): dicenda pro cruda -
         * agnitio per instrumentum tacere suppressa, re-armationes
         * in lib/vigilia */
        constans character* cautio = vigilia_cautio_dicenda(
            t->vigilia, (s64)time(NIHIL));

        si (cautio != NIHIL)
        {
            ChordaAedificator* aed = chorda_aedificator_creare(pn,
                textus.mensura + CCLVI);

            chorda_aedificator_appendere_chorda(aed, textus);
            chorda_aedificator_appendere_literis(aed, "\n\n");
            chorda_aedificator_appendere_literis(aed, cautio);
            textus = chorda_aedificator_finire(aed);
        }
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

/* ==================================================
 * identitates nid + citationes ex codice (frustum D 01KY3D7EJP):
 * gemini indicis nexus - pigre, absentia = sine resolutione.
 * identitates.tsv: nid via linea modus elementum genus_u linea_u...
 * citationes.tsv: praefixum via linea
 * ================================================== */

interior vacuum
_nides_onerare (Tabularium* t)
{
    character* textus;
    i32 mensura = ZEPHYRUM;
    i32 cursor = ZEPHYRUM;

    si (t->via_identitatum == NIHIL)
    {
        t->nides_temptatae = VERUM;   /* numquam retempta */
        redde;
    }
    textus = _plagulam_legere(t->piscina, t->via_identitatum,
        &mensura);
    si (textus == NIHIL)
    {
        redde;   /* plagula abest - retempta postea (index post
                  * ortum residentis apparere potest) */
    }
    t->nides = xar_creare(t->piscina, (i32)magnitudo(_NidOrdo));
    si (t->nides == NIHIL)
    {
        redde;
    }
    t->nides_temptatae = VERUM;
    dum (cursor < mensura)
    {
        i32 initium = cursor;
        i32 finis;
        chorda campi[VII];
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
        c = initium;
        dum (c <= finis && n_campi < VII)
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
        si (n_campi < VII)
        {
            perge;
        }
        {
            ChordaAedificator* aed = chorda_aedificator_creare(
                t->piscina, LXIV);
            _NidOrdo* ordo;

            chorda_aedificator_appendere_chorda(aed, campi[I]);
            chorda_aedificator_appendere_literis(aed, ":");
            si (campi[VI].mensura == I
                && campi[VI].datum[ZEPHYRUM] == '0')
            {
                chorda_aedificator_appendere_chorda(aed,
                    campi[II]);
            }
            alioquin
            {
                chorda_aedificator_appendere_chorda(aed,
                    campi[VI]);
            }
            chorda_aedificator_appendere_literis(aed, " (");
            chorda_aedificator_appendere_chorda(aed, campi[V]);
            chorda_aedificator_appendere_literis(aed, ")");
            ordo = (_NidOrdo*)xar_addere(t->nides);
            si (ordo != NIHIL)
            {
                ordo->nid = campi[ZEPHYRUM];
                ordo->sedes = _litterae(t->piscina,
                    chorda_aedificator_finire(aed));
            }
        }
    }
}

/* nid per praefixum: sedes primae congruentis; numerus_out =
 * quot congruunt (0 = absens, >1 = ambiguum) */
interior constans character*
_nid_invenire (Tabularium* t, chorda praefixum, i32* numerus_out)
{
    constans character* sedes = NIHIL;
    i32 i;

    *numerus_out = ZEPHYRUM;
    si (!t->nides_temptatae)
    {
        _nides_onerare(t);
    }
    si (t->nides == NIHIL || praefixum.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(t->nides); i++)
    {
        constans _NidOrdo* ordo = (constans _NidOrdo*)xar_obtinere(
            t->nides, i);

        si (ordo->nid.mensura >= praefixum.mensura
            && memcmp(ordo->nid.datum, praefixum.datum,
                   (memoriae_index)praefixum.mensura) == ZEPHYRUM)
        {
            si (*numerus_out == ZEPHYRUM)
            {
                sedes = ordo->sedes;
            }
            (*numerus_out)++;
        }
    }
    redde sedes;
}

interior vacuum
_citationes_onerare (Tabularium* t)
{
    character* textus;
    i32 mensura = ZEPHYRUM;
    i32 cursor = ZEPHYRUM;

    si (t->via_citationum == NIHIL)
    {
        t->citationes_temptatae = VERUM;   /* numquam retempta */
        redde;
    }
    textus = _plagulam_legere(t->piscina, t->via_citationum,
        &mensura);
    si (textus == NIHIL)
    {
        redde;   /* plagula abest - retempta postea */
    }
    t->citationes = xar_creare(t->piscina,
        (i32)magnitudo(_CitatioOrdo));
    si (t->citationes == NIHIL)
    {
        redde;
    }
    t->citationes_temptatae = VERUM;
    dum (cursor < mensura)
    {
        i32 initium = cursor;
        i32 finis;
        chorda campi[III];
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
        c = initium;
        dum (c <= finis && n_campi < III)
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
        si (n_campi < III)
        {
            perge;
        }
        {
            ChordaAedificator* aed = chorda_aedificator_creare(
                t->piscina, LXIV);
            _CitatioOrdo* ordo;

            chorda_aedificator_appendere_chorda(aed, campi[I]);
            chorda_aedificator_appendere_literis(aed, ":");
            chorda_aedificator_appendere_chorda(aed, campi[II]);
            ordo = (_CitatioOrdo*)xar_addere(t->citationes);
            si (ordo != NIHIL)
            {
                ordo->praefixum = campi[ZEPHYRUM];
                ordo->sedes = _litterae(t->piscina,
                    chorda_aedificator_finire(aed));
            }
        }
    }
}

/* nexus reversi: citationes ex codice quarum praefixum res_id
 * huius praefigit */
interior vacuum
_citationes_reddere (Tabularium* t, ChordaAedificator* aed,
    chorda res_id)
{
    i32 i;

    si (!t->citationes_temptatae)
    {
        _citationes_onerare(t);
    }
    si (t->citationes == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(t->citationes); i++)
    {
        constans _CitatioOrdo* ordo = (constans _CitatioOrdo*)
            xar_obtinere(t->citationes, i);

        si (ordo->praefixum.mensura > ZEPHYRUM
            && res_id.mensura >= ordo->praefixum.mensura
            && memcmp(res_id.datum, ordo->praefixum.datum,
                   (memoriae_index)ordo->praefixum.mensura)
                == ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed,
                "\ncitata ex codice: ");
            chorda_aedificator_appendere_literis(aed, ordo->sedes);
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
        alioquin si (_chorda_est(genus, "nid"))
        {
            i32 congruentes = ZEPHYRUM;
            constans character* sedes = _nid_invenire(t, scopus,
                &congruentes);

            si (congruentes == I && sedes != NIHIL)
            {
                chorda_aedificator_appendere_literis(aed, " -> ");
                chorda_aedificator_appendere_literis(aed, sedes);
            }
            alioquin si (congruentes > I)
            {
                chorda_aedificator_appendere_literis(aed,
                    " - CAUTIO: praefixum ambiguum in indice"
                    " identitatum");
            }
            alioquin
            {
                chorda_aedificator_appendere_literis(aed,
                    " - CAUTIO: inresoluta (nid in indice non est"
                    " - annotatio deleta aut index vetus; curre"
                    " ./silva/identitates.sh -renovare)");
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

/* praefixum ULID validum? 6..25 characterum alphabeti Crockford
 * (0-9, A-Z sine I L O U) - reconstructio praefixi ex memoria post
 * compactionem 'res ignota' dabat (ergonomia 2026-07-17). Sub VI
 * characteribus periculum congruentiae fortuitae; XXVI = id plenum
 * (via directa iam tegit). */
interior b32
_ulid_praefixus_est (chorda clavis)
{
    i32 i;

    si (clavis.mensura < VI || clavis.mensura >= XXVI)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < clavis.mensura; i++)
    {
        character c = (character)clavis.datum[i];
        b32 cifra = c >= '0' && c <= '9';
        b32 littera = c >= 'A' && c <= 'Z'
            && c != 'I' && c != 'L' && c != 'O' && c != 'U';

        si (!cifra && !littera)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* res per praefixum res_id: unicum resolvit, plura = ambiguum.
 * Alphabetum ULID '%'/'_' non continet - LIKE tutum. */
interior chorda
_res_per_praefixum (Tabularium* t, chorda praefixum, Piscina* pn,
    b32* ambiguum_out)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "SELECT res_id FROM res WHERE res_id LIKE ? || '%'"
        " ORDER BY res_id LIMIT 2");
    chorda inventum;

    inventum.mensura = ZEPHYRUM;
    inventum.datum = NIHIL;
    si (e == NIHIL)
    {
        redde inventum;
    }
    scrinium_ligare_textum(e, I, praefixum);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        inventum = scrinium_columna_textus(e, 0, pn);
        si (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            /* plura congruunt - ambiguum */
            si (ambiguum_out != NIHIL)
            {
                *ambiguum_out = VERUM;
            }
            inventum.mensura = ZEPHYRUM;
            inventum.datum = NIHIL;
        }
    }
    scrinium_finire(e);
    redde inventum;
}

/* clavem (id aut titulum) ad res_id solvere; titulus pluribus rebus
 * = AMBIGUUS (vacua + vexillum - exemplar legati multi-definitorum;
 * LIMIT-1 tacitum erat acies, quaestio 'Tituli duplicati').
 * Recessus ultimus: praefixum ULID inambiguum (2026-07-17). */
interior chorda
_res_solvere (Tabularium* t, chorda clavis, Piscina* pn,
    b32* ambiguum_out)
{
    chorda d = gesta_res_datum(t->mundus,
        _litterae(pn, clavis), pn);
    chorda vacua;
    chorda per_titulum;

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
    per_titulum = _res_per_titulum(t, clavis, pn);
    si (per_titulum.mensura > ZEPHYRUM)
    {
        redde per_titulum;
    }
    /* recessus ultimus: praefixum ULID inambiguum (ordo servatus -
     * id exactum, titulus exactus, TUM praefixum; semantica
     * exsistens intacta). Trunci solum - in ramo lex E2-B1 res_id
     * plenum tenet. */
    si (_ulid_praefixus_est(clavis))
    {
        redde _res_per_praefixum(t, clavis, pn, ambiguum_out);
    }
    redde per_titulum;
}

/* candidatos ambiguitatis nominare (res_id discernit). Modus per
 * fontem: titulus pluribus rebus = quaestio tituli; alioquin
 * praefixum ULID pluribus congruens = quaestio praefixi. */
interior vacuum
_ambiguitatem_respondere (Tabularium* t, Piscina* pn, JsonValor* id,
    chorda titulus, FILE* effusio)
{
    ChordaAedificator* aed = chorda_aedificator_creare(pn, CCLVI);
    b32 per_praefixum = _tituli_numerus(t, titulus) <= (s64)I
        && _ulid_praefixus_est(titulus);
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        per_praefixum
        ? "SELECT res_id, genus, status FROM res WHERE res_id"
          " LIKE ? || '%' ORDER BY res_id LIMIT 5"
        : "SELECT res_id, genus, status FROM res WHERE titulus = ?"
          " ORDER BY res_id LIMIT 5");

    chorda_aedificator_appendere_literis(aed, per_praefixum
        ? "praefixum ambiguum '" : "titulus ambiguus '");
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
    chorda membrum, constans character* actor_l,
    constans character* origo_l)
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
    ev.origo = origo_l;
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
 * entitates: proiectio una-plagula-per-rem (GENERATUM per tag -
 * archivum planum legibile ex quovis editore/instrumento quod
 * folders navigat). Spec: project-specs/gesta-entitates-spec.md.
 * ================================================== */

/* titulum in slug URL-aptum vertere: minusculae ASCII, quisque
 * cursus non-[a-z0-9] fit lineola una, lineolae extremae
 * praecisae, ad XL octetos truncatum (sine lineola pendente post
 * truncationem). Titulus vacuus/sine-alnum -> "sine-titulo" */
interior chorda
_titulum_ad_slug (chorda titulus, Piscina* pn)
{
    ChordaAedificator* aed = chorda_aedificator_creare(pn, LXIV);
    b32 lineola_pendens = FALSUM;   /* an ultimum emissum fuit '-' */
    b32 aliquid = FALSUM;           /* an character alphanumericus emissus */
    i32 emissa = ZEPHYRUM;          /* longitudo currens */
    i32 i;
    character unum[II];

    unum[I] = '\0';
    per (i = ZEPHYRUM; i < titulus.mensura && emissa < XL; i++)
    {
        i8 c = titulus.datum[i];

        si (c >= (i8)'A' && c <= (i8)'Z')
        {
            c = (i8)(c + (i8)('a' - 'A'));
        }
        si ((c >= (i8)'a' && c <= (i8)'z')
            || (c >= (i8)'0' && c <= (i8)'9'))
        {
            /* lineola pendens + alnum = duo octeti hoc gradu -
             * capacitas ANTE quamque appositionem probanda (non
             * semel per gradum solum) ne emissa summam XL superet */
            b32 lineola_hic = lineola_pendens && aliquid;
            i32 necessaria = lineola_hic ? II : I;

            si (emissa + necessaria > XL)
            {
                frange;   /* nulla capacitas - slug hic terminatur */
            }
            si (lineola_hic)
            {
                chorda_aedificator_appendere_literis(aed, "-");
                emissa++;
                lineola_pendens = FALSUM;
            }
            unum[0] = (character)c;
            chorda_aedificator_appendere_literis(aed, unum);
            emissa++;
            aliquid = VERUM;
        }
        alioquin
        {
            lineola_pendens = VERUM;   /* mora - lineolam solum si sequitur alnum */
        }
    }
    si (!aliquid)
    {
        redde _ch("sine-titulo");
    }
    redde chorda_aedificator_finire(aed);
}

/* "<genus>-<slug>-<res_id>.md" */
interior chorda
_entitatem_nomen_plagulae (chorda genus, chorda slug,
    constans character* res_id, Piscina* pn)
{
    ChordaAedificator* aed = chorda_aedificator_creare(pn, LXIV);

    chorda_aedificator_appendere_chorda(aed, genus);
    chorda_aedificator_appendere_literis(aed, "-");
    chorda_aedificator_appendere_chorda(aed, slug);
    chorda_aedificator_appendere_literis(aed, "-");
    chorda_aedificator_appendere_literis(aed, res_id);
    chorda_aedificator_appendere_literis(aed, ".md");
    redde chorda_aedificator_finire(aed);
}

/* ==================================================
 * auxilia entitatum (via.h/filum.h/iter_directoria.h - proiectio
 * non est via critica: errores tacite ignorantur, archivum
 * derivatum regenerabile per initialize)
 * ================================================== */

/* duas partes viae per via_iungere componere (saccharum super API
 * array-based via.h - vocatum crebro hic) */
interior chorda
_duas_iungere (chorda a, chorda b, Piscina* pn)
{
    chorda partes[II];

    partes[0] = a;
    partes[I] = b;
    redde via_iungere(partes, II, pn);
}

/* trans omnia subdirectoria tagorum, quaeque plagula terminans
 * "-<res_id>.md" delere. Directoria tagi vacua NON purgantur (nulla
 * machina domestica pro rmdir - relinquere acceptabile pro v1). */
interior vacuum
_entis_plagulas_omnes_delere (Tabularium* t, constans character* res_id,
    Piscina* pn)
{
    DirectoriumIterator* radix;
    DirectoriumIntroitus* e;
    character acus[XL + IV];   /* "-<ulid>.md" */

    si (t->via_entitatum == NIHIL)
    {
        redde;
    }
    radix = directorium_iterator_aperire(t->via_entitatum, pn);
    si (radix == NIHIL)
    {
        redde;
    }
    sprintf(acus, "-%s.md", res_id);
    dum ((e = directorium_iterator_proximum(radix)) != NIHIL)
    {
        DirectoriumIterator* sub;
        DirectoriumIntroitus* f;
        chorda semita_dir;

        si (e->genus != INTROITUS_DIRECTORIUM
            || (e->titulus.mensura > ZEPHYRUM
                && e->titulus.datum[0] == '.'))
        {
            perge;
        }
        semita_dir = _duas_iungere(_ch(t->via_entitatum), e->titulus,
            pn);
        sub = directorium_iterator_aperire(_litterae(pn, semita_dir),
            pn);
        si (sub == NIHIL)
        {
            perge;   /* non directorium */
        }
        dum ((f = directorium_iterator_proximum(sub)) != NIHIL)
        {
            memoriae_index ln;
            memoriae_index la = strlen(acus);

            si (f->genus != INTROITUS_FILUM
                || (f->titulus.mensura > ZEPHYRUM
                    && f->titulus.datum[0] == '.'))
            {
                perge;
            }
            ln = (memoriae_index)f->titulus.mensura;
            si (ln >= la
                && memcmp(f->titulus.datum + (ln - la), acus, la)
                    == ZEPHYRUM)
            {
                chorda semita_plag = _duas_iungere(semita_dir,
                    f->titulus, pn);

                (vacuum)filum_delere(_litterae(pn, semita_plag));
            }
        }
        directorium_iterator_claudere(sub);
    }
    directorium_iterator_claudere(radix);
}

/* ==================================================
 * entitas -> chorda semitae relativae / markdown
 * ================================================== */

/* genus + nomen plagulae rei per res_id legere: SELECT genus,titulus
 * -> _titulum_ad_slug -> _entitatem_nomen_plagulae, sequentia
 * communis inter semitam relativam et reconciliationem incrementalem
 * (emendatio DRY). genus_out optionalis (NIHIL si vocans genus non
 * curat - nomen_out iam genus incorporat). Redde FALSUM si res_id in
 * tabula res non invenitur (nomen_out adhuc scriptum - "sine-titulo"
 * pro slug - vocans decernit an id abstinendum sit). */
interior b32
_entitatem_genus_nomen (Tabularium* t, constans character* res_id,
    Piscina* pn, chorda* genus_out, chorda* nomen_out)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "SELECT genus, titulus FROM res WHERE res_id = ?");
    chorda genus_ch = _ch("");
    chorda titulus_ch = _ch("");
    b32 inventa = FALSUM;

    si (e != NIHIL)
    {
        scrinium_ligare_textum(e, I, _ch(res_id));
        si (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            genus_ch = scrinium_columna_textus(e, ZEPHYRUM, pn);
            titulus_ch = scrinium_columna_textus(e, I, pn);
            inventa = VERUM;
        }
        scrinium_finire(e);
    }
    si (genus_out != NIHIL)
    {
        *genus_out = genus_ch;
    }
    *nomen_out = _entitatem_nomen_plagulae(genus_ch,
        _titulum_ad_slug(titulus_ch, pn), res_id, pn);
    redde inventa;
}

/* semita relativa "../<primum-tag>/<genus>-<slug>-<id>.md" ad
 * membrum ligatum; chorda vacua si membrum_id non est res (tunc
 * vocans textum planum reddit) */
interior chorda
_entitatem_semita_relativa (Tabularium* t, chorda membrum_id, Piscina* pn)
{
    constans character* membrum_l = _litterae(pn, membrum_id);
    chorda nomen_plagulae;
    chorda datum;
    chorda primum_tag = _ch("_sine_tag");
    ChordaAedificator* aed;

    si (!_entitatem_genus_nomen(t, membrum_l, pn, NIHIL,
            &nomen_plagulae))
    {
        redde _ch("");   /* membrum non est res -> sine ligamine */
    }
    datum = gesta_res_datum(t->mundus, membrum_l, pn);
    {
        JsonResultus r = json_legere(datum, pn);

        si (r.successus && json_est_objectum(r.radix))
        {
            JsonValor* tags = json_objectum_capere(r.radix, "tags");

            si (tags != NIHIL && json_est_tabulatum(tags)
                && json_tabulatum_numerus(tags) > ZEPHYRUM)
            {
                JsonValor* t0 = json_tabulatum_obtinere(tags, ZEPHYRUM);

                si (t0 != NIHIL && json_est_chorda(t0))
                {
                    primum_tag = json_ad_chorda(t0);
                }
            }
        }
    }
    aed = chorda_aedificator_creare(pn, LXIV);
    chorda_aedificator_appendere_literis(aed, "../");
    chorda_aedificator_appendere_chorda(aed, primum_tag);
    chorda_aedificator_appendere_literis(aed, "/");
    chorda_aedificator_appendere_chorda(aed, nomen_plagulae);
    redde chorda_aedificator_finire(aed);
}

/* corpus plenus plagulae unius entitatis: frontmatter + titulus +
 * corpus + ## Status + ## Notae + ## Nexus + ## Ancorae. Assumit
 * rem exsistere (vocans gesta_res_datum non-vacuum iam probavit) */
interior chorda
_entitatem_ad_markdown (Tabularium* t, constans character* res_id,
    Piscina* pn)
{
    ChordaAedificator* aed = chorda_aedificator_creare(pn,
        2048);   /* MDXX/MMXL desunt in latina.h - numerale
                  * decimum hic manet propter hanc lacunam */
    chorda datum = gesta_res_datum(t->mundus, res_id, pn);
    JsonValor* st = NIHIL;
    chorda genus_ch = _ch("");
    chorda titulus_ch = _ch("");
    chorda status_ch = _ch("");
    chorda creatum_ch = _ch("");
    JsonResultus r;

    /* res tabula: genus/titulus/status/creatum */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT genus, titulus, status, creatum FROM res"
            " WHERE res_id = ?");

        si (e != NIHIL)
        {
            scrinium_ligare_textum(e, I, _ch(res_id));
            si (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                genus_ch = scrinium_columna_textus(e, ZEPHYRUM, pn);
                titulus_ch = scrinium_columna_textus(e, I, pn);
                status_ch = scrinium_columna_textus(e, II, pn);
                creatum_ch = scrinium_columna_textus(e, III, pn);
            }
            scrinium_finire(e);
        }
    }
    r = json_legere(datum, pn);
    si (r.successus && json_est_objectum(r.radix))
    {
        st = r.radix;
    }
    /* frontmatter */
    chorda_aedificator_appendere_literis(aed, "---\nid: ");
    chorda_aedificator_appendere_literis(aed, res_id);
    chorda_aedificator_appendere_literis(aed, "\ngenus: ");
    chorda_aedificator_appendere_chorda(aed, genus_ch);
    chorda_aedificator_appendere_literis(aed, "\nstatus: ");
    chorda_aedificator_appendere_chorda(aed, status_ch);
    chorda_aedificator_appendere_literis(aed, "\ncreatum: ");
    chorda_aedificator_appendere_chorda(aed, creatum_ch);
    chorda_aedificator_appendere_literis(aed, "\ntags: ");
    si (st != NIHIL)
    {
        JsonValor* tags = json_objectum_capere(st, "tags");
        i32 i;
        i32 n = (tags != NIHIL && json_est_tabulatum(tags))
            ? json_tabulatum_numerus(tags) : ZEPHYRUM;

        per (i = ZEPHYRUM; i < n; i++)
        {
            JsonValor* tg = json_tabulatum_obtinere(tags, i);

            si (tg == NIHIL || !json_est_chorda(tg))
            {
                perge;
            }
            si (i > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(aed, ", ");
            }
            chorda_aedificator_appendere_chorda(aed, json_ad_chorda(tg));
        }
    }
    chorda_aedificator_appendere_literis(aed,
        "\n---\n\n> GENERATUM (proiectio entitatum - noli manu"
        " edere)\n\n# ");
    chorda_aedificator_appendere_chorda(aed, titulus_ch);
    chorda_aedificator_appendere_literis(aed, "\n\n");
    /* corpus */
    si (st != NIHIL)
    {
        JsonValor* corpus = json_objectum_capere(st, "corpus");

        si (corpus != NIHIL && json_est_chorda(corpus))
        {
            chorda_aedificator_appendere_chorda(aed,
                json_ad_chorda(corpus));
            chorda_aedificator_appendere_literis(aed, "\n");
        }
    }
    /* ## Status - chronologia ex eventis creatio/status. TRUNCALIA
     * SOLA (branch_id vacuum): eventus ramales trunco invisibiles
     * manent, et post fusionem originalia ramalia cum copiis
     * truncalibus coexsistunt (res_id idem) - sine filtro quaeque
     * linea bis redderetur. */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT genus_eventus, datum, creatum FROM tessellae"
            " WHERE res_id = ? AND branch_id = ''"
            " AND genus_eventus IN ('creatio','status')"
            " ORDER BY seq");

        si (e != NIHIL)
        {
            scrinium_ligare_textum(e, I, _ch(res_id));
            chorda_aedificator_appendere_literis(aed, "\n## Status\n\n");
            dum (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                chorda ge = scrinium_columna_textus(e, ZEPHYRUM, pn);
                chorda ed = scrinium_columna_textus(e, I, pn);
                chorda ec = scrinium_columna_textus(e, II, pn);

                chorda_aedificator_appendere_literis(aed, "- ");
                si (_chorda_est(ge, "creatio"))
                {
                    chorda_aedificator_appendere_literis(aed, "creatum");
                }
                alioquin
                {
                    JsonResultus rd = json_legere(ed, pn);

                    si (rd.successus && json_est_objectum(rd.radix))
                    {
                        JsonValor* nv = json_objectum_capere(rd.radix,
                            "novus");

                        si (nv != NIHIL && json_est_chorda(nv))
                        {
                            chorda_aedificator_appendere_chorda(aed,
                                json_ad_chorda(nv));
                        }
                    }
                }
                chorda_aedificator_appendere_literis(aed,
                    " \xE2\x80\x94 ");
                chorda_aedificator_appendere_chorda(aed, ec);
                chorda_aedificator_appendere_literis(aed, "\n");
            }
            scrinium_finire(e);
        }
    }
    /* ## Notae - omnes, vetustissima primum (truncalia sola - vide
     * commentarium Status supra) */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT datum, actor, creatum FROM tessellae"
            " WHERE res_id = ? AND branch_id = ''"
            " AND genus_eventus = 'nota' ORDER BY seq");
        b32 caput = FALSUM;

        si (e != NIHIL)
        {
            scrinium_ligare_textum(e, I, _ch(res_id));
            dum (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                chorda ed = scrinium_columna_textus(e, ZEPHYRUM, pn);
                chorda ea = scrinium_columna_textus(e, I, pn);
                chorda ec = scrinium_columna_textus(e, II, pn);
                JsonResultus rd = json_legere(ed, pn);

                si (!caput)
                {
                    chorda_aedificator_appendere_literis(aed,
                        "\n## Notae\n\n");
                    caput = VERUM;
                }
                chorda_aedificator_appendere_literis(aed, "- ");
                chorda_aedificator_appendere_chorda(aed, ec);
                chorda_aedificator_appendere_literis(aed, " (");
                chorda_aedificator_appendere_chorda(aed, ea);
                chorda_aedificator_appendere_literis(aed, "): ");
                si (rd.successus && json_est_objectum(rd.radix))
                {
                    JsonValor* tx = json_objectum_capere(rd.radix,
                        "textus");

                    si (tx != NIHIL && json_est_chorda(tx))
                    {
                        chorda_aedificator_appendere_chorda(aed,
                            json_ad_chorda(tx));
                    }
                }
                chorda_aedificator_appendere_literis(aed, "\n");
            }
            scrinium_finire(e);
        }
    }
    /* ## Nexus - socii ut ligamina relativa */
    {
        Xar* socii = gesta_socii_rei(t->mundus, res_id, pn);
        i32 i;
        i32 n = (socii != NIHIL) ? xar_numerus(socii) : ZEPHYRUM;

        si (n > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed, "\n## Nexus\n\n");
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            GestaSocius* s = (GestaSocius*)xar_obtinere(socii, i);
            chorda verbum;
            chorda titulus_m;
            chorda semita;

            si (s == NIHIL)
            {
                perge;
            }
            verbum = _verbum_vinculi(t, s->nexus_res, pn);
            titulus_m = _titulus_membri(t, s->membrum, pn);
            semita = _entitatem_semita_relativa(t, s->membrum, pn);
            chorda_aedificator_appendere_literis(aed, "- ");
            chorda_aedificator_appendere_chorda(aed, verbum);
            chorda_aedificator_appendere_literis(aed,
                " \xE2\x86\x92 ");
            si (semita.mensura > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(aed, "[");
                chorda_aedificator_appendere_chorda(aed, titulus_m);
                chorda_aedificator_appendere_literis(aed, "](");
                chorda_aedificator_appendere_chorda(aed, semita);
                chorda_aedificator_appendere_literis(aed, ")");
            }
            alioquin
            {
                chorda_aedificator_appendere_chorda(aed, titulus_m);
            }
            chorda_aedificator_appendere_literis(aed, "\n");
        }
    }
    /* ## Ancorae */
    si (st != NIHIL)
    {
        JsonValor* anc = json_objectum_capere(st, "ancorae");
        i32 i;
        i32 n = (anc != NIHIL && json_est_tabulatum(anc))
            ? json_tabulatum_numerus(anc) : ZEPHYRUM;

        si (n > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed, "\n## Ancorae\n\n");
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            JsonValor* a = json_tabulatum_obtinere(anc, i);
            JsonValor* ag;
            JsonValor* as;

            si (a == NIHIL || !json_est_objectum(a))
            {
                perge;
            }
            ag = json_objectum_capere(a, "genus");
            as = json_objectum_capere(a, "scopus");
            chorda_aedificator_appendere_literis(aed, "- ");
            si (ag != NIHIL && json_est_chorda(ag))
            {
                chorda_aedificator_appendere_chorda(aed,
                    json_ad_chorda(ag));
                chorda_aedificator_appendere_literis(aed, " ");
            }
            si (as != NIHIL && json_est_chorda(as))
            {
                chorda_aedificator_appendere_literis(aed, "`");
                chorda_aedificator_appendere_chorda(aed,
                    json_ad_chorda(as));
                chorda_aedificator_appendere_literis(aed, "`");
            }
            chorda_aedificator_appendere_literis(aed, "\n");
        }
    }
    redde chorda_aedificator_finire(aed);
}

/* ==================================================
 * reconciliatio - una res (incrementalis)
 * ================================================== */

/* plagulam entitatis in omne directorium tagi (vel _sine_tag)
 * scribere, prioribus copiis prius deletis. Res remota/absens
 * (gesta_res_datum vacuum) -> omnes copiae delentur, nihil
 * scribitur. */
interior vacuum
_entitatem_reconciliare (Tabularium* t, constans character* res_id,
    Piscina* pn)
{
    chorda datum;
    chorda nomen_plagulae;
    chorda md;
    JsonValor* st = NIHIL;
    JsonValor* tags = NIHIL;
    i32 n_tags = ZEPHYRUM;
    i32 i;

    si (t->via_entitatum == NIHIL)
    {
        redde;
    }
    /* semper primo reliquas plagulas huius entis purgare (tags/slug
     * mutati, remotio) - systema plagularum = memoria status veteris */
    _entis_plagulas_omnes_delere(t, res_id, pn);

    datum = gesta_res_datum(t->mundus, res_id, pn);
    si (datum.mensura == ZEPHYRUM)
    {
        redde;   /* remotum/absens -> nullae plagulae optatae */
    }
    {
        JsonResultus r = json_legere(datum, pn);

        si (r.successus && json_est_objectum(r.radix))
        {
            st = r.radix;
        }
    }
    (vacuum)_entitatem_genus_nomen(t, res_id, pn, NIHIL, &nomen_plagulae);
    md = _entitatem_ad_markdown(t, res_id, pn);

    si (st != NIHIL)
    {
        tags = json_objectum_capere(st, "tags");
        si (tags != NIHIL && json_est_tabulatum(tags))
        {
            n_tags = json_tabulatum_numerus(tags);
        }
    }
    (vacuum)filum_directorium_creare_si_necesse(t->via_entitatum);

    si (n_tags == ZEPHYRUM)
    {
        chorda dir = _duas_iungere(_ch(t->via_entitatum),
            _ch("_sine_tag"), pn);
        chorda plag;

        (vacuum)filum_directorium_creare_si_necesse(_litterae(pn, dir));
        plag = _duas_iungere(dir, nomen_plagulae, pn);
        (vacuum)filum_scribere(_litterae(pn, plag), md);
        redde;
    }
    per (i = ZEPHYRUM; i < n_tags; i++)
    {
        JsonValor* tg = json_tabulatum_obtinere(tags, i);
        chorda dir;
        chorda plag;

        si (tg == NIHIL || !json_est_chorda(tg))
        {
            perge;
        }
        dir = _duas_iungere(_ch(t->via_entitatum), json_ad_chorda(tg),
            pn);
        (vacuum)filum_directorium_creare_si_necesse(_litterae(pn, dir));
        plag = _duas_iungere(dir, nomen_plagulae, pn);
        (vacuum)filum_scribere(_litterae(pn, plag), md);
    }
}

/* ==================================================
 * reconciliatio - omnes res (transitus plenus / reformatio)
 * ================================================== */

/* directorium unius gradus purgare: quaeque plagula in quoque
 * subdirectorio delere. Directoria NON removentur (nulla machina
 * domestica pro rmdir) - reconciliatio sequens eas rescribit. */
interior vacuum
_directorium_purgare (constans character* via, Piscina* pn)
{
    DirectoriumIterator* radix = directorium_iterator_aperire(via, pn);
    DirectoriumIntroitus* e;

    si (radix == NIHIL)
    {
        redde;
    }
    dum ((e = directorium_iterator_proximum(radix)) != NIHIL)
    {
        DirectoriumIterator* sub;
        DirectoriumIntroitus* f;
        chorda semita_dir;

        si (e->genus != INTROITUS_DIRECTORIUM
            || (e->titulus.mensura > ZEPHYRUM
                && e->titulus.datum[0] == '.'))
        {
            perge;
        }
        semita_dir = _duas_iungere(_ch(via), e->titulus, pn);
        sub = directorium_iterator_aperire(_litterae(pn, semita_dir),
            pn);
        si (sub == NIHIL)
        {
            perge;
        }
        dum ((f = directorium_iterator_proximum(sub)) != NIHIL)
        {
            chorda semita_plag;

            si (f->genus != INTROITUS_FILUM
                || (f->titulus.mensura > ZEPHYRUM
                    && f->titulus.datum[0] == '.'))
            {
                perge;
            }
            semita_plag = _duas_iungere(semita_dir, f->titulus, pn);
            (vacuum)filum_delere(_litterae(pn, semita_plag));
        }
        directorium_iterator_claudere(sub);
    }
    directorium_iterator_claudere(radix);
}

/* directorium entitatum purgare + omnem rem in scrinio
 * reconciliare (initialize + operationes multi-rei) */
interior vacuum
_entitates_reconciliare_omnes (Tabularium* t, Piscina* pn)
{
    ScriniumEnuntiatum* e;

    si (t->via_entitatum == NIHIL)
    {
        redde;
    }
    _directorium_purgare(t->via_entitatum, pn);
    (vacuum)filum_directorium_creare_si_necesse(t->via_entitatum);
    e = scrinium_praeparare(gesta_scrinium(t->mundus),
        "SELECT res_id FROM res ORDER BY res_id");
    si (e == NIHIL)
    {
        redde;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        chorda rid = scrinium_columna_textus(e, ZEPHYRUM, pn);

        _entitatem_reconciliare(t, _litterae(pn, rid), pn);
    }
    scrinium_finire(e);
}

/* ==================================================
 * instrumenta
 * ================================================== */

/* prototypum resolutionis ramorum (definitio post _tab_agere -
 * addere/gerere/res parametrum 'ramus' eam postulant) */
interior chorda
_ramum_solvere (Tabularium* t, chorda clavis, Piscina* pn);

interior b32
_littera_aut_cifra (i8 octetus)
{
    character c = (character)octetus;

    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9');
}

/* similia post creationem (K4.2): vicini FTS tituli novi - custos
 * duplicationum ipso momento scripturae (quaere-ante saepe
 * praetermittitur). Termini = verba tituli (>= III char., usque
 * VIII) praefixata '*' disiuncta OR - idioma Latinum; bm25
 * ordinat, IDF verba communia deprimit. Res nova ipsa excluditur
 * (exhaustio pigra eam ante lectionem indicavit). Error syntaxis
 * MATCH = fructus vacuus = nihil appenditur (honestum). */
interior vacuum
_similia_reddere (Tabularium* t, ChordaAedificator* aed,
    chorda titulus, constans character* res_id_nova, Piscina* pn)
{
    ChordaAedificator* qa = chorda_aedificator_creare(pn, CCLVI);
    Xar* inventa;
    i32 i = ZEPHYRUM;
    i32 termini = ZEPHYRUM;
    i32 reddita = ZEPHYRUM;

    si (qa == NIHIL)
    {
        redde;
    }
    dum (i < titulus.mensura && termini < VIII)
    {
        i32 initium;

        dum (i < titulus.mensura
            && !_littera_aut_cifra(titulus.datum[i]))
        {
            i++;
        }
        initium = i;
        dum (i < titulus.mensura
            && _littera_aut_cifra(titulus.datum[i]))
        {
            i++;
        }
        si (i - initium >= III)
        {
            chorda verbum;

            verbum.datum = titulus.datum + initium;
            verbum.mensura = i - initium;
            si (termini > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(qa, " OR ");
            }
            chorda_aedificator_appendere_chorda(qa, verbum);
            chorda_aedificator_appendere_literis(qa, "*");
            termini++;
        }
    }
    si (termini == ZEPHYRUM)
    {
        redde;
    }
    inventa = gesta_quaerere(t->mundus,
        _litterae(pn, chorda_aedificator_finire(qa)), NIHIL, NIHIL,
        pn);
    si (inventa == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM;
         i < xar_numerus(inventa) && reddita < III; i++)
    {
        GestaInventum* inv = (GestaInventum*)xar_obtinere(inventa,
            i);

        si (inv == NIHIL || _chorda_est(inv->res_id, res_id_nova))
        {
            perge;
        }
        chorda_aedificator_appendere_literis(aed,
            reddita == ZEPHYRUM ? "\nsimilia:\n  " : "\n  ");
        chorda_aedificator_appendere_chorda(aed, inv->res_id);
        chorda_aedificator_appendere_literis(aed, "  ");
        chorda_aedificator_appendere_chorda(aed, inv->genus);
        chorda_aedificator_appendere_literis(aed, "/");
        chorda_aedificator_appendere_chorda(aed, inv->status);
        chorda_aedificator_appendere_literis(aed, "  ");
        chorda_aedificator_appendere_chorda(aed, inv->titulus);
        reddita++;
    }
}

/* codicillos UTF-8 numerare (bytes non-continuationis) - limes
 * pipati CCXL CHARACTERES significat, non bytes (decisio Franis) */
interior i32
_codicillos_numerare (chorda textus)
{
    i32 n = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si (((insignatus character)textus.datum[i] & 0xC0u)
            != 0x80u)
        {
            n++;
        }
    }
    redde n;
}

/* titulum ex corpore derivare: praefixum XL codicillorum, ad
 * lineam novam primam sectum (titulus optionalis - F4 ergonomia:
 * finis tituli-fabricandi pro pipatis/commentariis) */
interior chorda
_titulum_derivare (chorda corpus)
{
    i32 codicilli = ZEPHYRUM;
    i32 i = ZEPHYRUM;
    chorda t;

    dum (i < corpus.mensura && codicilli < XL
        && corpus.datum[i] != (i8)'\n')
    {
        i++;
        dum (i < corpus.mensura
            && ((insignatus character)corpus.datum[i] & 0xC0u)
                == 0x80u)
        {
            i++;
        }
        codicilli++;
    }
    t.datum = corpus.datum;
    t.mensura = i;
    redde t;
}

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
    chorda origo = _arg(argumenta, "origo");
    chorda signatura = _arg(argumenta, "signatura");
    chorda ad = _arg(argumenta, "ad");
    chorda ramus_arg = _arg(argumenta, "ramus");
    chorda datum_arg = _arg(argumenta, "datum");
    chorda ramus_id;
    JsonValor* datum;
    GestaEventum e;
    character res_id[GESTA_RES_ID_MENSURA];
    chorda datum_textus;

    ramus_id.mensura = ZEPHYRUM;
    ramus_id.datum = NIHIL;
    si (genus.mensura == ZEPHYRUM)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("genus requiritur"), VERUM);
        redde;
    }
    si (titulus.mensura == ZEPHYRUM && corpus.mensura > ZEPHYRUM)
    {
        titulus = _titulum_derivare(corpus);
    }
    si (titulus.mensura == ZEPHYRUM)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("titulus (aut corpus unde derivetur) requiritur"),
            VERUM);
        redde;
    }
    si (ad.mensura > ZEPHYRUM && ramus_arg.mensura > ZEPHYRUM)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("ad in ramo nondum sustentum (parcum)"), VERUM);
        redde;
    }
    /* custos pipati (F0 forum): limes CCXL codicillorum DURUS -
     * limes ipse genus est; latus daemonis = MCP et app aequaliter
     * ligati (spec-v2 par III.2) */
    si (_chorda_est(genus, "pipatum")
        && _codicillos_numerare(corpus) > CCXL)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("pipatum nimis longum (limes CCXL codicilli)"),
            VERUM);
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
    /* signatura (F0 forum): vox individualis scribentis (nomen
     * exemplaris, e.g. "Fable 5") - clavis dati liberi, mutatio
     * schematis nulla; actor = stratum identitatis manet */
    si (signatura.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(datum, "signatura",
            json_chorda_creare(pn, signatura));
    }
    /* datum (genera G0): campi entis generis usoris - obiectum
     * JSON ut CHORDA escapata (exemplar gerere-mutationis). Claves
     * summae in datum eventus mersae: reductor (mersio
     * superficialis), legere (embed totum) et iudicium camporum ad
     * scripturam gratis sequuntur. genus/titulus protecta -
     * identitas per parametra sola. */
    si (datum_arg.mensura > ZEPHYRUM)
    {
        JsonResultus r = json_legere(datum_arg, pn);

        si (!r.successus || !json_est_objectum(r.radix))
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("datum: obiectum JSON requiritur"), VERUM);
            redde;
        }
        {
            JsonObjectumIterator iter =
                json_objectum_iterator(r.radix);
            chorda k;
            JsonValor* v;

            dum (json_objectum_iterator_proxima(&iter, &k, &v))
            {
                si (_chorda_est(k, "genus")
                    || _chorda_est(k, "titulus"))
                {
                    perge;
                }
                json_objectum_ponere_chorda(datum, k, v);
            }
        }
    }
    datum_textus = json_scribere(datum, pn);

    e.res_id = NIHIL;
    e.genus_eventus = "creatio";
    e.datum = _litterae(pn, datum_textus);
    e.actor = actor.mensura > ZEPHYRUM
        ? _litterae(pn, actor) : "claude";
    e.origo = origo.mensura > ZEPHYRUM
        ? _litterae(pn, origo) : "mcp";
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
    _entitatem_reconciliare(t, res_id, pn);
    /* ad (F4 ergonomia): nexus respondet-ad sponte - responsum
     * filo uno vocamine, non duobus (exemplar bracchii nexus in
     * _tab_gerere) */
    si (ad.mensura > ZEPHYRUM)
    {
        chorda alterum_id = _res_solvere(t, ad, pn, NIHIL);
        chorda membrum_b = alterum_id.mensura > ZEPHYRUM
            ? alterum_id : ad;
        constans character* actor_l = actor.mensura > ZEPHYRUM
            ? _litterae(pn, actor) : "claude";
        constans character* origo_l = origo.mensura > ZEPHYRUM
            ? _litterae(pn, origo) : "mcp";
        JsonValor* d = json_objectum_creare(pn);
        GestaEventum ev;
        character vinculum_id[GESTA_RES_ID_MENSURA];

        json_objectum_ponere(d, "genus",
            json_chorda_creare_literis(pn, "nexus"));
        json_objectum_ponere(d, "verbum",
            json_chorda_creare_literis(pn, "respondet-ad"));
        ev.res_id = NIHIL;
        ev.genus_eventus = "creatio";
        ev.datum = _litterae(pn, json_scribere(d, pn));
        ev.actor = actor_l;
        ev.origo = origo_l;
        si (!gesta_scribere(t->mundus, &ev, vinculum_id)
            || !_membrum_scribere(t, pn, vinculum_id, "a",
                   _ch(res_id), actor_l, origo_l)
            || !_membrum_scribere(t, pn, vinculum_id, "b",
                   membrum_b, actor_l, origo_l))
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("res creata sed nexus respondet-ad fractus"),
                VERUM);
            redde;
        }
        _entitatem_reconciliare(t, vinculum_id, pn);
    }
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
        si (ad.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed,
                " --respondet-ad--> ");
            chorda_aedificator_appendere_chorda(aed, ad);
        }
        si (eodem_titulo > (s64)I)
        {
            character cautio[CXXVIII];

            sprintf(cautio, "\nCAUTIO: titulus iam exsistit (%d"
                " res eodem titulo) - res_id ad discretionem"
                " adhibe", (int)eodem_titulo);
            chorda_aedificator_appendere_literis(aed, cautio);
        }
        _similia_reddere(t, aed, titulus, res_id, pn);
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
    chorda origo = _arg(argumenta, "origo");
    chorda signatura = _arg(argumenta, "signatura");
    chorda ramus_arg = _arg(argumenta, "ramus");
    constans character* origo_l = origo.mensura > ZEPHYRUM
        ? _litterae(pn, origo) : "mcp";
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
                _ch("res ignota (id, praefixum inambiguum, aut titulus exactus)"), VERUM);
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
    alioquin si (_chorda_est(actus, "ictus"))
    {
        /* 'haec res me ITERUM momordit'. Textus OPTIONALIS - ictus
         * sine verbis adhuc numerat, et gradus qui verba postulat
         * gradus est qui non fit. */
        chorda textus = _arg(argumenta, "textus");

        genus_eventus = "ictus";
        si (textus.mensura > ZEPHYRUM)
        {
            json_objectum_ponere(datum, "textus",
                json_chorda_creare(pn, textus));
        }
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
        ev.origo = origo_l;
        si (!gesta_scribere(t->mundus, &ev, vinculum_id)
            || !_membrum_scribere(t, pn, vinculum_id, "a",
                   res_id, actor_l, origo_l)
            || !_membrum_scribere(t, pn, vinculum_id, "b",
                   membrum_b, actor_l, origo_l))
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("scriptura vinculi fracta"), VERUM);
            redde;
        }
        _tabulam_scribere(t, pn);
        _entitatem_reconciliare(t, _litterae(pn, res_id), pn);
        _entitatem_reconciliare(t, _litterae(pn, membrum_b), pn);
        _entitatem_reconciliare(t, vinculum_id, pn);
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
        ev.origo = origo_l;
        si (!gesta_scribere(t->mundus, &ev, NIHIL))
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("scriptura solutionis fracta"), VERUM);
            redde;
        }
        _tabulam_scribere(t, pn);
        _entitatem_reconciliare(t, _litterae(pn, res_id), pn);
        _entitatem_reconciliare(t, _litterae(pn, membrum_b), pn);
        _entitatem_reconciliare(t, _litterae(pn, vinculum), pn);
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
            _ch("actus ignotus (nota|ictus|status|nexus|"
                "denexus|mutatio|remotio)"), VERUM);
        redde;
    }

    /* signatura (F0 forum): POST ramos actuum - mutatio cruda
     * datum substituit, ergo hic sola sedes tuta. Nexus/denexus
     * supra redeunt sine ea (acceptum: responsa eam in creatione
     * commentarii ferunt) */
    si (signatura.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(datum, "signatura",
            json_chorda_creare(pn, signatura));
    }
    e.res_id = _litterae(pn, res_id);
    e.genus_eventus = genus_eventus;
    e.datum = _litterae(pn, json_scribere(datum, pn));
    e.actor = actor.mensura > ZEPHYRUM
        ? _litterae(pn, actor) : "claude";
    e.origo = origo_l;
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
    _entitatem_reconciliare(t, _litterae(pn, res_id), pn);
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

/* caput rei (linea prima + res_id + tempora) - commune redditione
 * plena et breviario (K4.2: ne formae divergant) */
interior vacuum
_caput_rei_reddere (Tabularium* t, ChordaAedificator* aed,
    chorda res_id, Piscina* pn)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "SELECT genus, titulus, status, creatum, mutatum"
        " FROM res WHERE res_id = ?");

    si (e == NIHIL)
    {
        redde;
    }
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

/* affordantiae (K3 superficies passiva) - commune */
interior vacuum
_actiones_reddere (Tabularium* t, ChordaAedificator* aed,
    chorda res_id, Piscina* pn)
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

/* compendium rei (K4.2 'breviter'): corpus ut textus purus + notae
 * ultimae III (plicatura eas in statum iam fert) + affordantiae.
 * OMISSA: datum crudum, ancorae (IO plagularum + sigilla), socii,
 * salus, annales - lectio parcorum vili pro lectore
 * context-budgetato. */
interior vacuum
_breviarium_reddere (Tabularium* t, ChordaAedificator* aed,
    chorda res_id, JsonValor* st, Piscina* pn)
{
    _caput_rei_reddere(t, aed, res_id, pn);
    si (st != NIHIL)
    {
        JsonValor* corpus_v = json_objectum_capere(st, "corpus");
        JsonValor* tags_v = json_objectum_capere(st, "tags");
        JsonValor* notae_v = json_objectum_capere(st, "notae");

        si (corpus_v != NIHIL && json_est_chorda(corpus_v))
        {
            chorda_aedificator_appendere_literis(aed,
                "\ncorpus ");
            chorda_aedificator_appendere_chorda(aed,
                json_ad_chorda(corpus_v));
        }
        si (tags_v != NIHIL && json_est_tabulatum(tags_v)
            && json_tabulatum_numerus(tags_v) > ZEPHYRUM)
        {
            i32 i;

            chorda_aedificator_appendere_literis(aed, "\ntags ");
            per (i = ZEPHYRUM;
                 i < json_tabulatum_numerus(tags_v); i++)
            {
                JsonValor* tg = json_tabulatum_obtinere(tags_v,
                    i);

                si (tg == NIHIL || !json_est_chorda(tg))
                {
                    perge;
                }
                si (i > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_literis(aed,
                        " ");
                }
                chorda_aedificator_appendere_chorda(aed,
                    json_ad_chorda(tg));
            }
        }
        si (notae_v != NIHIL && json_est_tabulatum(notae_v)
            && json_tabulatum_numerus(notae_v) > ZEPHYRUM)
        {
            i32 n = json_tabulatum_numerus(notae_v);
            i32 ostendendae = n > III ? III : n;
            character numeri[LXIV];
            i32 i;

            sprintf(numeri, "\nnotae (%d ex %d, recentissima"
                " primum):", (int)ostendendae, (int)n);
            chorda_aedificator_appendere_literis(aed, numeri);
            per (i = ZEPHYRUM; i < ostendendae; i++)
            {
                JsonValor* nv = json_tabulatum_obtinere(notae_v,
                    n - I - i);
                JsonValor* tx;
                JsonValor* cr;
                JsonValor* ac;

                si (nv == NIHIL || !json_est_objectum(nv))
                {
                    perge;
                }
                tx = json_objectum_capere(nv, "textus");
                cr = json_objectum_capere(nv, "creatum");
                ac = json_objectum_capere(nv, "actor");
                chorda_aedificator_appendere_literis(aed,
                    "\n  ");
                si (cr != NIHIL && json_est_chorda(cr))
                {
                    chorda_aedificator_appendere_chorda(aed,
                        json_ad_chorda(cr));
                }
                si (ac != NIHIL && json_est_chorda(ac))
                {
                    chorda_aedificator_appendere_literis(aed,
                        " (");
                    chorda_aedificator_appendere_chorda(aed,
                        json_ad_chorda(ac));
                    chorda_aedificator_appendere_literis(aed,
                        ")");
                }
                chorda_aedificator_appendere_literis(aed, "  ");
                si (tx != NIHIL && json_est_chorda(tx))
                {
                    chorda_aedificator_appendere_chorda(aed,
                        json_ad_chorda(tx));
                }
            }
        }
    }
    _actiones_reddere(t, aed, res_id, pn);
}

/* Tectum campi singularis in impressione dati crudi. Campi ordinarii
 * (titulus, siglum, status) longe infra stant; documenta integra
 * longe supra. */
#define DATUM_CAMPUS_MAXIMUS 2048

/* Datum ad IMPRESSIONEM temperare: campos chordarum immanes
 * marcatore substituere.
 *
 * CUR (mensuratum 2026-07-25 in conditorio vivo): unus 'liber' cum
 * de-imagine.md in campo 'fons' vocationem 'res' simplicem in XLV KB
 * contextus vertit. Non est limes conditorii - nulla mensura in
 * validatione camporum, SQLite ad GB, transportus ad LXIV MB - sed
 * laqueus LECTORIS: agens post compactionem orientationem suam uno
 * vocamine comedit, eo ipso momento quo contextus carissimus est.
 *
 * MARCATOR CLAMAT: nomen campi manet et numerus octetorum omissorum
 * nominatur, ergo lector scit et QUID absit et QUANTUM, et campum
 * per viam suam petere potest. Truncatio TACITA hic vitium peius
 * esset quam effusio ipsa - lector 'fons' absentem pro 'fons'
 * vacuo legeret.
 *
 * SOLA impressio 'res' temperatur. '_tab_legere' CONSULTO intacta
 * manet: applicatio fori 'fons' ex lectione LISTAE hodie sumit
 * (apps/forum/assets/index.html: legere('liber') -> ens.datum.fons),
 * ergo suppressio ibi lectorem sententiarum frangeret. Illud latus
 * mutationem BILATERAM poscit (conditorium tacet + applicatio corpus
 * in apertione petit) - res propria, non haec.
 *
 * Fallback: si datum objectum non est, aut parsari non potest, aut
 * nihil immane continet, chorda ORIGINALIS redditur - mos vetus
 * intactus et nulla copia frustra facta. */
interior chorda
_datum_temperatum (chorda datum, Piscina* pn)
{
    JsonResultus r;
    JsonValor* novum;
    JsonObjectumIterator iter;
    chorda clavis;
    JsonValor* valor;
    b32 mutatum = FALSUM;

    si (datum.mensura <= (i32)DATUM_CAMPUS_MAXIMUS) redde datum;
    r = json_legere(datum, pn);
    si (!r.successus || !json_est_objectum(r.radix)) redde datum;
    novum = json_objectum_creare(pn);
    si (novum == NIHIL) redde datum;

    iter = json_objectum_iterator(r.radix);
    dum (json_objectum_iterator_proxima(&iter, &clavis, &valor))
    {
        si (json_est_chorda(valor)
            && json_ad_chorda(valor).mensura
                > (i32)DATUM_CAMPUS_MAXIMUS)
        {
            character nota[128];

            sprintf(nota, "<OMISSUM: %lu octeti - campus immanis>",
                (insignatus longus)json_ad_chorda(valor).mensura);
            json_objectum_ponere_chorda(novum, clavis,
                json_chorda_creare_literis(pn, nota));
            mutatum = VERUM;
        }
        alioquin
        {
            json_objectum_ponere_chorda(novum, clavis, valor);
        }
    }
    si (!mutatum) redde datum;
    redde json_scribere(novum, pn);
}

interior vacuum
_tab_res (Tabularium* t, Piscina* pn, JsonValor* id,
    JsonValor* argumenta, FILE* effusio)
{
    chorda clavis = _arg(argumenta, "res");
    chorda ramus_arg = _arg(argumenta, "ramus");
    chorda breviter = _arg(argumenta, "breviter");
    b32 breviter_est = _chorda_est(breviter, "verum")
        || _chorda_est(breviter, "true")
        || _chorda_est(breviter, "ita");
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
    si (breviter_est)
    {
        _breviarium_reddere(t, aed, res_id, st, pn);
        _textum_respondere(t, pn, effusio, id,
            chorda_aedificator_finire(aed), FALSUM);
        redde;
    }
    _caput_rei_reddere(t, aed, res_id, pn);
    chorda_aedificator_appendere_literis(aed, "\ndatum ");
    chorda_aedificator_appendere_chorda(aed,
        _datum_temperatum(datum, pn));
    si (st != NIHIL)
    {
        _ancoras_reddere(t, aed, st, pn);
    }
    _citationes_reddere(t, aed, res_id);
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
    _actiones_reddere(t, aed, res_id, pn);
    /* annales entis (XV eventa recentissima; truncalia sola - eadem
     * puritas trunci ac proiectio entitatum, alias eventus ramales
     * et duplicata post-fusionem apparerent) */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT genus_eventus, actor, creatum FROM tessellae"
            " WHERE res_id = ? AND branch_id = ''"
            " ORDER BY seq DESC LIMIT 15");

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

/* tacere: cautionem vigiliae per N responsa supprimere - agnitio
 * explicita (politica in lib/vigilia; consilium Fran 2026-07-17).
 * Responsum confirmationis ipsum per infundibulum fluit et unum e
 * numero absumit - consulto, simplicitas vincit. */
interior vacuum
_tab_tacere (Tabularium* t, Piscina* pn, JsonValor* id,
    JsonValor* argumenta, FILE* effusio)
{
    JsonValor* v = (argumenta != NIHIL)
        ? json_objectum_capere(argumenta, "responsa") : NIHIL;
    s64 responsa = ZEPHYRUM;
    character buf[CXCII];

    si (v != NIHIL && json_est_integer(v))
    {
        responsa = json_ad_integer(v);
    }
    alioquin si (v != NIHIL && json_est_chorda(v))
    {
        responsa = (s64)atoi(_litterae(pn, json_ad_chorda(v)));
    }
    si (responsa <= (s64)ZEPHYRUM)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("responsa (numerus positivus) requiritur"), VERUM);
        redde;
    }
    si (!vigilia_tacere(t->vigilia, (i32)responsa,
            (s64)time(NIHIL)))
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("vigilia recens - nihil tacendum"), FALSUM);
        redde;
    }
    sprintf(buf, "vigilia tacet per %ld responsa (re-armabitur:"
        " numero exhausto, quiete %d s sine petitionibus,"
        " commissione, causa nova)", (longus)responsa,
        (int)VIGILIA_QUIES_SECUNDA);
    {
        chorda textus;
        unio { character* b; i8* m; } u;

        u.b = buf;
        textus.datum = u.m;
        textus.mensura = (i32)strlen(buf);
        _textum_respondere(t, pn, effusio, id, textus, FALSUM);
    }
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
    /* RES SAEPISSIME ICTAE: pretium MENSURATUM (quotiens res
     * iterum momordit), non momentum aestimatum. Tacet omnino si
     * nulla res icta est - sectio vacua tantum spatium consumit. */
    {
        Xar* ci = gesta_census_ictuum(t->mundus, (i32)X, pn);

        si (ci != NIHIL && xar_numerus(ci) > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed,
                "\nres saepissime ICTAE (apertae):");
            per (i = ZEPHYRUM; i < xar_numerus(ci); i++)
            {
                GestaIctusOrdo* o = (GestaIctusOrdo*)xar_obtinere(
                    ci, i);

                si (o == NIHIL)
                {
                    perge;
                }
                sprintf(numeri, "\n  %dx  ", (int)o->ictus);
                chorda_aedificator_appendere_literis(aed, numeri);
                chorda_aedificator_appendere_chorda(aed, o->res_id);
                chorda_aedificator_appendere_literis(aed, "  ");
                chorda_aedificator_appendere_chorda(aed, o->titulus);
            }
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
    /* frustum D: salus citationum ex codice (res= in annotationibus
     * contra tabulam - dimidium code->board putredinis
     * bidirectionalis; inresolutae nominatae) */
    {
        i32 i;
        i32 summa_citationum = ZEPHYRUM;
        i32 inresolutae = ZEPHYRUM;

        si (!t->citationes_temptatae)
        {
            _citationes_onerare(t);
        }
        si (t->citationes != NIHIL
            && xar_numerus(t->citationes) > ZEPHYRUM)
        {
            per (i = ZEPHYRUM; i < xar_numerus(t->citationes); i++)
            {
                constans _CitatioOrdo* ordo =
                    (constans _CitatioOrdo*)xar_obtinere(
                        t->citationes, i);
                b32 ambiguum = FALSUM;
                chorda inventa;

                summa_citationum++;
                inventa = _res_per_praefixum(t, ordo->praefixum,
                    pn, &ambiguum);
                si (inventa.mensura == ZEPHYRUM && !ambiguum)
                {
                    inresolutae++;
                    chorda_aedificator_appendere_literis(aed,
                        i == ZEPHYRUM || inresolutae == I
                            ? "\n" : "");
                    chorda_aedificator_appendere_literis(aed,
                        "\nCAUTIO citatio ex codice inresoluta: ");
                    chorda_aedificator_appendere_chorda(aed,
                        ordo->praefixum);
                    chorda_aedificator_appendere_literis(aed,
                        " (");
                    chorda_aedificator_appendere_literis(aed,
                        ordo->sedes);
                    chorda_aedificator_appendere_literis(aed, ")");
                }
            }
            sprintf(numeri, "\ncitationes ex codice %d"
                " (inresolutae %d)", (int)summa_citationum,
                (int)inresolutae);
            chorda_aedificator_appendere_literis(aed, numeri);
        }
    }
    _textum_respondere(t, pn, effusio, id,
        chorda_aedificator_finire(aed), FALSUM);
}

/* summarium eventus: campus primus textualis ex dato (ordo fixus -
 * notae textum, status novum, creationes titulum ostendunt) */
/* cursor lectionis (F4 forum): titulus fixus in tabula
 * consumptorum exsistente (INSERT OR REPLACE - clavis primaria).
 * NB: scrinium proiectio rescribilis est - cursor perditus =
 * salutatio iterata, non damnum (acceptum). */
#define TABULARIUM_LECTOR "claude-lectum"

interior s64
_lectum_capere (Tabularium* t, b32* exstat)
{
    ScriniumEnuntiatum* e;
    s64 seq = ZEPHYRUM;

    *exstat = FALSUM;
    e = scrinium_praeparare(gesta_scrinium(t->mundus),
        "SELECT seq FROM consumptores WHERE titulus = ?");
    si (e == NIHIL)
    {
        redde ZEPHYRUM;
    }
    scrinium_ligare_textum(e, I, _ch(TABULARIUM_LECTOR));
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        seq = scrinium_columna_numerus(e, 0);
        *exstat = VERUM;
    }
    scrinium_finire(e);
    redde seq;
}

interior vacuum
_lectum_ponere (Tabularium* t, s64 seq)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "INSERT OR REPLACE INTO consumptores(titulus, seq)"
        " VALUES(?, ?)");

    si (e == NIHIL)
    {
        redde;
    }
    scrinium_ligare_textum(e, I, _ch(TABULARIUM_LECTOR));
    scrinium_ligare_numerum(e, II, seq);
    (vacuum)scrinium_gradi(e);
    scrinium_finire(e);
}

interior s64
_seq_maximum (Tabularium* t)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "SELECT COALESCE(MAX(seq), 0) FROM tessellae"
        " WHERE branch_id = ''");
    s64 m = ZEPHYRUM;

    si (e == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        m = scrinium_columna_numerus(e, 0);
    }
    scrinium_finire(e);
    redde m;
}

/* numerus eventuum non lectorum - EODEM praedicato ac lectio
 * ab_lecto (genera G0): eventa datorum exclusa, ne salutatio NOVA
 * aut "plura restant" epistulas promittat quas lectio negabit
 * (inventum portae vivae G1: promissum falsum = fossa vana) */
interior s64
_nova_numerare (Tabularium* t, s64 post)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(
        gesta_scrinium(t->mundus),
        "SELECT COUNT(*) FROM tessellae t"
        " LEFT JOIN res r ON r.res_id = t.res_id"
        " WHERE t.branch_id = '' AND t.seq > ?"
        " AND NOT EXISTS (SELECT 1 FROM genera g"
        "  WHERE g.titulus = r.genus AND g.usor = 1)"
        " AND NOT EXISTS (SELECT 1 FROM membra ma"
        "  JOIN res ra ON ra.res_id = ma.membrum"
        "  JOIN genera ga ON ga.titulus = ra.genus"
        "  WHERE ma.res_id = t.res_id AND ma.pars = 'a'"
        "  AND ga.usor = 1)");
    s64 n = ZEPHYRUM;

    si (e == NIHIL)
    {
        redde ZEPHYRUM;
    }
    scrinium_ligare_numerum(e, I, post);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        n = scrinium_columna_numerus(e, 0);
    }
    scrinium_finire(e);
    redde n;
}

interior chorda
_actum_summarium (Piscina* pn, chorda datum)
{
    chorda vacua;
    JsonResultus r;
    interior constans character* constans CLAVES[] = {
        "textus", "novus", "corpus", "titulus", "actio",
        "verbum", "clavis", "valor", NIHIL
    };
    i32 k;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (datum.mensura == ZEPHYRUM)
    {
        redde vacua;
    }
    r = json_legere(datum, pn);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde vacua;
    }
    per (k = ZEPHYRUM; CLAVES[k] != NIHIL; k++)
    {
        JsonValor* v = json_objectum_capere(r.radix, CLAVES[k]);

        si (v != NIHIL && json_est_chorda(v))
        {
            redde json_ad_chorda(v);
        }
    }
    redde vacua;
}

/* acta (K4.5 frustum 1): cauda fluminis eventuum globalis trans
 * res omnes - superficies recensionis contractus scribae ("quid
 * hodie scriptum est" una quaestione). Truncus solus (branch_id
 * vacuum; param ramus = futurum nominatum). */
interior vacuum
_tab_acta (Tabularium* t, Piscina* pn, JsonValor* argumenta,
    JsonValor* id, FILE* effusio)
{
    ChordaAedificator* aed = chorda_aedificator_creare(pn, 4096);
    ScriniumEnuntiatum* e;
    s64 quantum = XXV;
    chorda genus_f = _arg(argumenta, "genus");
    chorda actor_f = _arg(argumenta, "actor");
    chorda ab_lecto = _arg(argumenta, "ab_lecto");
    b32 modus_lecti = _chorda_est(ab_lecto, "verum");
    s64 solum = ZEPHYRUM;
    s64 maximum_visum = ZEPHYRUM;
    character buf[CXCII];
    i32 numerus = ZEPHYRUM;

    si (argumenta != NIHIL)
    {
        JsonValor* v = json_objectum_capere(argumenta, "quantum");

        si (v != NIHIL && json_est_integer(v))
        {
            quantum = json_ad_integer(v);
        }
        alioquin si (v != NIHIL && json_est_chorda(v))
        {
            quantum = (s64)atoi(_litterae(pn, json_ad_chorda(v)));
        }
    }
    si (quantum < (s64)I) quantum = (s64)XXV;
    si (quantum > (s64)CC) quantum = (s64)CC;
    si (genus_f.datum == NIHIL) genus_f = _ch("");
    si (actor_f.datum == NIHIL) actor_f = _ch("");

    si (modus_lecti)
    {
        /* F4: eventa non lecta VETUSTISSIMA PRIMUM (ordo
         * lectionis); cursor provehitur ad maximum redditum */
        b32 exstat = FALSUM;

        solum = _lectum_capere(t, &exstat);
        si (!exstat)
        {
            solum = _seq_maximum(t);
            _lectum_ponere(t, solum);
        }
        maximum_visum = solum;
        /* genera G0: eventa rerum generum usoris (usor=1) et
         * nexuum quorum membrum-a res usoris est EXCLUSA -
         * epistulae = colloquium, non res datorum; creatio
         * definitionis (genus systematis) epistula manet. Acta
         * plena (sine ab_lecto) omnia retinent. */
        e = scrinium_praeparare(gesta_scrinium(t->mundus),
            "SELECT t.seq, t.creatum, t.actor, t.genus_eventus,"
            " t.res_id, t.datum, r.titulus"
            " FROM tessellae t"
            " LEFT JOIN res r ON r.res_id = t.res_id"
            " WHERE t.branch_id = ''"
            " AND t.seq > ?"
            " AND (? = '' OR t.genus_eventus = ?)"
            " AND (? = '' OR t.actor = ?)"
            " AND NOT EXISTS (SELECT 1 FROM genera g"
            "  WHERE g.titulus = r.genus AND g.usor = 1)"
            " AND NOT EXISTS (SELECT 1 FROM membra ma"
            "  JOIN res ra ON ra.res_id = ma.membrum"
            "  JOIN genera ga ON ga.titulus = ra.genus"
            "  WHERE ma.res_id = t.res_id AND ma.pars = 'a'"
            "  AND ga.usor = 1)"
            " ORDER BY t.seq ASC LIMIT ?");
        si (e == NIHIL)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("acta: praeparatio scrinii fracta"), VERUM);
            redde;
        }
        scrinium_ligare_numerum(e, I, solum);
        scrinium_ligare_textum(e, II, genus_f);
        scrinium_ligare_textum(e, III, genus_f);
        scrinium_ligare_textum(e, IV, actor_f);
        scrinium_ligare_textum(e, V, actor_f);
        scrinium_ligare_numerum(e, VI, quantum);
        chorda_aedificator_appendere_literis(aed,
            "acta non lecta (vetustissima primum):");
    }
    alioquin
    {
        e = scrinium_praeparare(gesta_scrinium(t->mundus),
            "SELECT t.seq, t.creatum, t.actor, t.genus_eventus,"
            " t.res_id, t.datum, r.titulus"
            " FROM tessellae t"
            " LEFT JOIN res r ON r.res_id = t.res_id"
            " WHERE t.branch_id = ''"
            " AND (? = '' OR t.genus_eventus = ?)"
            " AND (? = '' OR t.actor = ?)"
            " ORDER BY t.seq DESC LIMIT ?");
        si (e == NIHIL)
        {
            _textum_respondere(t, pn, effusio, id,
                _ch("acta: praeparatio scrinii fracta"), VERUM);
            redde;
        }
        scrinium_ligare_textum(e, I, genus_f);
        scrinium_ligare_textum(e, II, genus_f);
        scrinium_ligare_textum(e, III, actor_f);
        scrinium_ligare_textum(e, IV, actor_f);
        scrinium_ligare_numerum(e, V, quantum);
        chorda_aedificator_appendere_literis(aed,
            "acta recentia (recentissima primum):");
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        s64 seq = (s64)scrinium_columna_numerus(e, 0);
        chorda creatum = scrinium_columna_textus(e, I, pn);
        chorda actor = scrinium_columna_textus(e, II, pn);
        chorda genus_ev = scrinium_columna_textus(e, III, pn);
        chorda res_id = scrinium_columna_textus(e, IV, pn);
        chorda datum = scrinium_columna_textus(e, V, pn);
        chorda titulus = scrinium_columna_textus(e, VI, pn);
        chorda summarium = _actum_summarium(pn, datum);

        sprintf(buf, "\n%ld  ", (longus)seq);
        chorda_aedificator_appendere_literis(aed, buf);
        si (creatum.mensura > XVI) creatum.mensura = XVI;
        chorda_aedificator_appendere_chorda(aed, creatum);
        chorda_aedificator_appendere_literis(aed, "  ");
        chorda_aedificator_appendere_chorda(aed, actor);
        chorda_aedificator_appendere_literis(aed, "  ");
        chorda_aedificator_appendere_chorda(aed, genus_ev);
        chorda_aedificator_appendere_literis(aed, "  ");
        si (titulus.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_chorda(aed, titulus);
        }
        alioquin
        {
            /* res sine plicatura (genus-eventus systematis) -
             * praefixum res_id */
            si (res_id.mensura > X) res_id.mensura = X;
            chorda_aedificator_appendere_chorda(aed, res_id);
        }
        si (summarium.mensura > ZEPHYRUM
            && !chorda_aequalis(summarium, titulus))
        {
            i32 finis = summarium.mensura;
            i32 j;
            b32 sectum = FALSUM;

            per (j = ZEPHYRUM; j < finis; j++)
            {
                si (summarium.datum[j] == '\n')
                {
                    finis = j;
                    sectum = VERUM;
                    frange;
                }
            }
            si (finis > LXXX)
            {
                finis = LXXX;
                sectum = VERUM;
            }
            summarium.mensura = finis;
            chorda_aedificator_appendere_literis(aed, " - ");
            chorda_aedificator_appendere_chorda(aed, summarium);
            si (sectum)
            {
                chorda_aedificator_appendere_literis(aed, "...");
            }
        }
        si (seq > maximum_visum)
        {
            maximum_visum = seq;
        }
        numerus++;
    }
    scrinium_finire(e);
    si (modus_lecti)
    {
        si (numerus == ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed,
                "\n  (nihil novi)");
        }
        alioquin
        {
            s64 restant;

            _lectum_ponere(t, maximum_visum);
            restant = _nova_numerare(t, maximum_visum);
            si (restant > (s64)ZEPHYRUM)
            {
                sprintf(buf, "\n  ... et %ld plura restant"
                    " (acta ab_lecto iterum)", (longus)restant);
                chorda_aedificator_appendere_literis(aed, buf);
            }
        }
    }
    alioquin si (numerus == ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aed,
            "\n  (nulla - filtra nimis arta?)");
    }
    _textum_respondere(t, pn, effusio, id,
        chorda_aedificator_finire(aed), FALSUM);
}

/* legere (F2 forum): lectio structurata pro apps - textus responsi
 * = TABULATUM JSON rerum recentissimarum primum. Datum plicatum
 * INSERTUM ut obiectum (corpus/signatura/tags gratis); actor ex
 * eventu creationis (subquaestio); respondet_ad ex membris (a ->
 * b) - verbum per LIKE in dato nexus (cruditas v1 commentata:
 * verbum fori unicum, json_extract vitatum). */
interior vacuum
_tab_legere (Tabularium* t, Piscina* pn, JsonValor* argumenta,
    JsonValor* id, FILE* effusio)
{
    ScriniumEnuntiatum* e;
    s64 quantum = L;
    chorda genus_f = _arg(argumenta, "genus");
    chorda status_f = _arg(argumenta, "status");
    JsonValor* tabulatum = json_tabulatum_creare(pn);

    si (argumenta != NIHIL)
    {
        JsonValor* v = json_objectum_capere(argumenta, "quantum");

        si (v != NIHIL && json_est_integer(v))
        {
            quantum = json_ad_integer(v);
        }
        alioquin si (v != NIHIL && json_est_chorda(v))
        {
            quantum = (s64)atoi(_litterae(pn, json_ad_chorda(v)));
        }
    }
    si (quantum < (s64)I) quantum = (s64)L;
    si (quantum > (s64)CC) quantum = (s64)CC;
    si (genus_f.datum == NIHIL) genus_f = _ch("");
    si (status_f.datum == NIHIL) status_f = _ch("");

    e = scrinium_praeparare(gesta_scrinium(t->mundus),
        "SELECT r.res_id, r.genus, r.titulus, r.status, r.datum,"
        " r.creatum, r.mutatum,"
        " (SELECT tc.actor FROM tessellae tc"
        "  WHERE tc.res_id = r.res_id"
        "  AND tc.genus_eventus = 'creatio' AND tc.branch_id = ''"
        "  ORDER BY tc.seq LIMIT 1),"
        " (SELECT mb.membrum FROM membra ma"
        "  JOIN res n ON n.res_id = ma.res_id"
        "   AND n.genus = 'nexus' AND n.status = 'vigens'"
        "   AND n.datum LIKE '%\"verbum\":\"respondet-ad\"%'"
        "  JOIN membra mb ON mb.res_id = ma.res_id"
        "   AND mb.pars = 'b'"
        "  WHERE ma.pars = 'a' AND ma.membrum = r.res_id"
        "  LIMIT 1)"
        " FROM res r"
        " WHERE (? = '' OR r.genus = ?)"
        " AND (? = '' OR r.status = ?)"
        " ORDER BY r.creatum DESC, r.res_id DESC LIMIT ?");
    si (e == NIHIL)
    {
        _textum_respondere(t, pn, effusio, id,
            _ch("legere: praeparatio scrinii fracta"), VERUM);
        redde;
    }
    scrinium_ligare_textum(e, I, genus_f);
    scrinium_ligare_textum(e, II, genus_f);
    scrinium_ligare_textum(e, III, status_f);
    scrinium_ligare_textum(e, IV, status_f);
    scrinium_ligare_numerum(e, V, quantum);

    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        JsonValor* res = json_objectum_creare(pn);
        chorda rid = scrinium_columna_textus(e, 0, pn);
        chorda datum = scrinium_columna_textus(e, IV, pn);
        chorda actor = scrinium_columna_textus(e, VII, pn);
        chorda respondet = scrinium_columna_textus(e, VIII, pn);

        json_objectum_ponere(res, "res_id",
            json_chorda_creare(pn, rid));
        json_objectum_ponere(res, "genus",
            json_chorda_creare(pn,
                scrinium_columna_textus(e, I, pn)));
        json_objectum_ponere(res, "titulus",
            json_chorda_creare(pn,
                scrinium_columna_textus(e, II, pn)));
        json_objectum_ponere(res, "status",
            json_chorda_creare(pn,
                scrinium_columna_textus(e, III, pn)));
        json_objectum_ponere(res, "creatum",
            json_chorda_creare(pn,
                scrinium_columna_textus(e, V, pn)));
        json_objectum_ponere(res, "mutatum",
            json_chorda_creare(pn,
                scrinium_columna_textus(e, VI, pn)));
        json_objectum_ponere(res, "actor",
            json_chorda_creare(pn, actor));
        si (respondet.mensura > ZEPHYRUM)
        {
            json_objectum_ponere(res, "respondet_ad",
                json_chorda_creare(pn, respondet));
        }
        si (datum.mensura > ZEPHYRUM)
        {
            JsonResultus r = json_legere(datum, pn);

            si (r.successus)
            {
                json_objectum_ponere(res, "datum", r.radix);
            }
        }
        /* nexus (genera G0): tabulatum relationum embeddatum per
         * ordinem - verbum ex dato rei nexus (LIKE respondet-ad
         * NON extenditur: quaesitum indicatum, tituli
         * destinationis resoluti, multa verba multae destinationes;
         * respondet_ad supra pro filis fori manet) */
        {
            ScriniumEnuntiatum* en = scrinium_praeparare(
                gesta_scrinium(t->mundus),
                "SELECT n.datum, mb.membrum, tgt.titulus,"
                " tgt.status"
                " FROM membra ma"
                " JOIN res n ON n.res_id = ma.res_id"
                "  AND n.genus = 'nexus' AND n.status = 'vigens'"
                " JOIN membra mb ON mb.res_id = ma.res_id"
                "  AND mb.pars = 'b'"
                " LEFT JOIN res tgt ON tgt.res_id = mb.membrum"
                " WHERE ma.pars = 'a' AND ma.membrum = ?");

            si (en != NIHIL)
            {
                JsonValor* nexus_tab = NIHIL;

                scrinium_ligare_textum(en, I, rid);
                dum (scrinium_gradi(en) == SCRINIUM_ORDO)
                {
                    chorda n_datum = scrinium_columna_textus(en,
                        0, pn);
                    chorda ad_id = scrinium_columna_textus(en, I,
                        pn);
                    chorda ad_titulus = scrinium_columna_textus(en,
                        II, pn);
                    chorda ad_status = scrinium_columna_textus(en,
                        III, pn);
                    JsonResultus rn = json_legere(n_datum, pn);
                    JsonValor* v_verbum;
                    JsonValor* unus;

                    si (!rn.successus
                        || !json_est_objectum(rn.radix))
                    {
                        perge;
                    }
                    v_verbum = json_objectum_capere(rn.radix,
                        "verbum");
                    si (v_verbum == NIHIL
                        || !json_est_chorda(v_verbum))
                    {
                        perge;
                    }
                    si (nexus_tab == NIHIL)
                    {
                        nexus_tab = json_tabulatum_creare(pn);
                    }
                    unus = json_objectum_creare(pn);
                    json_objectum_ponere(unus, "verbum", v_verbum);
                    json_objectum_ponere(unus, "ad",
                        json_chorda_creare(pn, ad_id));
                    si (ad_titulus.mensura > ZEPHYRUM)
                    {
                        json_objectum_ponere(unus, "ad_titulus",
                            json_chorda_creare(pn, ad_titulus));
                    }
                    si (ad_status.mensura > ZEPHYRUM)
                    {
                        json_objectum_ponere(unus, "ad_status",
                            json_chorda_creare(pn, ad_status));
                    }
                    json_tabulatum_addere(nexus_tab, unus);
                }
                scrinium_finire(en);
                si (nexus_tab != NIHIL)
                {
                    json_objectum_ponere(res, "nexus", nexus_tab);
                }
            }
        }
        json_tabulatum_addere(tabulatum, res);
    }
    scrinium_finire(e);
    _textum_respondere(t, pn, effusio, id,
        json_scribere(tabulatum, pn), FALSUM);
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
        _entitates_reconciliare_omnes(t, pn);
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
        _entitates_reconciliare_omnes(t, pn);
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
        _entitates_reconciliare_omnes(t, pn);
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
        _entitates_reconciliare_omnes(t, pn);
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
        _entitates_reconciliare_omnes(t, pn);
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
        { "ancorae", "tabulatum JSON: [{\"genus\":\"symbolum|via"
          "|nid\",\"scopus\":\"...\",\"sigillum\":\"hex?\"}] -"
          " nid = identitas annotationis (praefixum >= VI;"
          " frustum D)", FALSUM },
        { "actor", "fran|claude|machina (ordinarius claude)",
          FALSUM },
        { "origo", "provenientia eventus (ordinarius mcp; e.g."
          " messis-2026-07, sessio)", FALSUM },
        { "signatura", "nomen exemplaris scribentis (e.g. Fable"
          " 5) - linea auctoris in foro; in dato eventus conditur",
          FALSUM },
        { "ad", "res cui respondetur - nexus respondet-ad sponte"
          " creatur (filum uno vocamine); titulus absens ex"
          " corpore derivatur", FALSUM },
        { "ramus", "titulus rami activi - creatio in ramo (trunco"
          " invisibilis usque ad fusionem)", FALSUM },
        { "datum", "obiectum JSON camporum ut CHORDA escapata -"
          " entia generum per definitionem creata (claves contra"
          " campos iudicatae, notae custodiae)", FALSUM }
    };
    interior constans TabArgumentum ARG_GERERE[] = {
        { "res", "res_id (aut praefixum ULID inambiguum >= 6 char.)"
          " aut titulus exactus (in ramo: res_id SOLUM)", VERUM },
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
        { "origo", "provenientia eventus (ordinarius mcp; e.g."
          " messis-2026-07, sessio)", FALSUM },
        { "signatura", "nomen exemplaris scribentis (e.g. Fable"
          " 5) - in dato eventus conditur (nota/status/mutatio/"
          "remotio)", FALSUM },
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
        { "res", "res_id (aut praefixum ULID inambiguum >= 6 char.)"
          " aut titulus exactus (in ramo: res_id SOLUM)", VERUM },
        { "breviter", "\"verum\" = compendium (corpus + notae"
          " ultimae III + actiones; sine dato crudo/ancoris/"
          "annalibus)", FALSUM },
        { "ramus", "titulus rami - lectio plicaturae ramalis",
          FALSUM }
    };
    interior constans TabArgumentum ARG_TACERE[] = {
        { "responsa", "numerus responsorum supprimendorum"
          " (positivus; limen 500)", VERUM }
    };
    interior constans TabArgumentum ARG_ACTA[] = {
        { "quantum", "numerus actorum (ordinarius XXV, tectum CC)",
          FALSUM },
        { "genus", "filtrum generis eventus (creatio|nota|status|"
          "mutatio|nexus|...)", FALSUM },
        { "actor", "filtrum actoris (fran|claude|machina)",
          FALSUM },
        { "ab_lecto", "\"verum\" = solum non lecta, vetustissima"
          " primum; cursor claude-lectum provehitur (optime sine"
          " filtris - provectio filtra ignorat)", FALSUM }
    };
    interior constans TabArgumentum ARG_LEGERE[] = {
        { "genus", "filtrum generis rerum (e.g. pipatum|articulus|"
          "commentarium)", FALSUM },
        { "status", "filtrum statûs", FALSUM },
        { "quantum", "numerus rerum (ordinarius L, tectum CC)",
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
        "desideratum) cum tags et ancoris optionalibus; similia"
        " FTS in responso (custos duplicationum).",
        ARG_ADDERE, XI));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "gerere",
        "Eventum unum in rem exsistentem scribere: nota, status,"
        " nexus/denexus (ligamina), mutatio, remotio. Violationes"
        " machinae notantur, non obstant.",
        ARG_GERERE, XIII));
    json_tabulatum_addere(instrumenta, _instrumentum(pn,
        "quaerere",
        "Quaestio FTS super statum materializatum (titulus/corpus/"
        "tags/notae). Idioma Latinum: praefixa 'parsur*'.",
        ARG_QUAERERE, IV));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "res",
        "Rem unam reddere: status + datum + ancorae (resolutae per"
        " indicem; CAUTIO si inresolutae) + actiones affordatae +"
        " annales recentes. breviter \"verum\" = compendium.",
        ARG_RES, III));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "census",
        "Census: genera x status, tags, seq/hwm.",
        NIHIL, ZEPHYRUM));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "acta",
        "Cauda fluminis eventuum globalis trans res omnes"
        " (truncus, recentissima primum) - superficies recensionis"
        " (quid hodie scriptum est).",
        ARG_ACTA, IV));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "legere",
        "Lectio structurata pro apps: textus = tabulatum JSON"
        " rerum (recentissima primum) cum dato plicato inserto,"
        " actore creationis, et respondet_ad (filum). Fundamentum"
        " tabulae fori.",
        ARG_LEGERE, III));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "tacere",
        "Cautionem vigiliae (residens obsoletus) per N responsa"
        " supprimere - agnitio explicita. Re-armatur: numero"
        " exhausto, quiete 300 s sine petitionibus, commissione"
        " git, causa staleness nova.",
        ARG_TACERE, I));
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
    /* IDEMPOTENS (F0 forum): salutatio iterata responsum idem
     * reddit - daemon se praeinitiat, clientes per-petitionem
     * innoxie salutant (mutatio machinae unica spec-v2 par II) */
    si (!t->initiatum)
    {
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
        _entitates_reconciliare_omnes(t, pn);
        t->initiatum = VERUM;
    }
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
        /* salutatio lectoris (F4): cursor absens -> conditus ad
         * hwm (silentium primi conventus); retardatio > 0 ->
         * "NOVA" in instructionibus - nuntiatio mitis, non
         * pulsus: salutatio */
        {
            constans character* instructiones = TABULARII_DOCTRINA;
            b32 exstat = FALSUM;
            s64 lectum = _lectum_capere(t, &exstat);

            si (!exstat)
            {
                _lectum_ponere(t, _seq_maximum(t));
            }
            alioquin
            {
                s64 nova = _nova_numerare(t, lectum);

                si (nova > (s64)ZEPHYRUM)
                {
                    ChordaAedificator* aed =
                        chorda_aedificator_creare(pn, 8192);
                    character buf[CXCII];

                    chorda_aedificator_appendere_literis(aed,
                        TABULARII_DOCTRINA);
                    sprintf(buf, "\n\nNOVA: %ld eventa ab ultima"
                        " lectione - acta {\"ab_lecto\":"
                        "\"verum\"} legenda.", (longus)nova);
                    chorda_aedificator_appendere_literis(aed,
                        buf);
                    instructiones = _litterae(pn,
                        chorda_aedificator_finire(aed));
                }
            }
            json_objectum_ponere(resultatum, "instructions",
                json_chorda_creare_literis(pn, instructiones));
        }
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
    alioquin si (_chorda_est(titulus, "acta"))
    {
        _tab_acta(t, pn, argumenta, id, effusio);
    }
    alioquin si (_chorda_est(titulus, "legere"))
    {
        _tab_legere(t, pn, argumenta, id, effusio);
    }
    alioquin si (_chorda_est(titulus, "tacere"))
    {
        _tab_tacere(t, pn, id, argumenta, effusio);
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
    t->via_identitatum = cfg->via_identitatum != NIHIL
        ? _litterae(piscina, _ch(cfg->via_identitatum)) : NIHIL;
    t->via_citationum = cfg->via_citationum != NIHIL
        ? _litterae(piscina, _ch(cfg->via_citationum)) : NIHIL;
    t->via_tabulae = cfg->via_tabulae != NIHIL
        ? _litterae(piscina, _ch(cfg->via_tabulae)) : NIHIL;
    t->via_entitatum = cfg->via_entitatum != NIHIL
        ? _litterae(piscina, _ch(cfg->via_entitatum)) : NIHIL;
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
