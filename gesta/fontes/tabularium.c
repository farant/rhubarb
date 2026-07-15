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
    "{res} = status + ancorae (CAUTIO si inresolutae) + annales. "
    "census {} = genera x status + tags + arretrata. Genera: "
    "quaestio (apertum->laborans->clausum|relictum), parcum "
    "(parcatum->tractum->clausum), decretum, nota, desideratum "
    "(apertum->impletum). Violationes machinae NON obstant - nota "
    "custodiae appenditur (iudicat, non obstat).";

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
      "[\"apertum\",\"relictum\"]],\"reducer\":\"ordinarius\"}" }
};

#define SEMINA_NUMERUS V

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
    /* index ancorarum (pigre; INTENTIO C decisio 3) */
    b32             index_temptatus;
    TabulaDispersa* sedes_index;     /* titulus -> character* "via:linea" */
    Xar*            sedes_tituli;    /* chorda (valore) */
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

/* effectus textus instrumenti (forma MCP; ex legato 5112-5153,
 * sine vigilia - index tabularii = scrinium ipsum, semper recens) */
interior vacuum
_textum_respondere (Piscina* pn, FILE* effusio, JsonValor* id,
    chorda textus, b32 error_est)
{
    JsonValor* resultatum = json_objectum_creare(pn);
    JsonValor* contentus = json_tabulatum_creare(pn);
    JsonValor* fragmentum = json_objectum_creare(pn);

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

/* clavem (id aut titulum) ad res_id solvere */
interior chorda
_res_solvere (Tabularium* t, chorda clavis, Piscina* pn)
{
    chorda d = gesta_res_datum(t->mundus,
        _litterae(pn, clavis), pn);

    si (d.mensura > ZEPHYRUM)
    {
        redde clavis;   /* res_id directum */
    }
    redde _res_per_titulum(t, clavis, pn);
}

/* ==================================================
 * instrumenta
 * ================================================== */

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
    JsonValor* datum;
    GestaEventum e;
    character res_id[GESTA_RES_ID_MENSURA];
    chorda datum_textus;

    si (genus.mensura == ZEPHYRUM || titulus.mensura == ZEPHYRUM)
    {
        _textum_respondere(pn, effusio, id,
            _ch("genus et titulus requiruntur"), VERUM);
        redde;
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
            _textum_respondere(pn, effusio, id,
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
    si (!gesta_scribere(t->mundus, &e, res_id))
    {
        ChordaAedificator* aed = chorda_aedificator_creare(pn,
            CCLVI);

        chorda_aedificator_appendere_literis(aed,
            "scriptura recusata: ");
        chorda_aedificator_appendere_literis(aed,
            gesta_error(t->mundus));
        _textum_respondere(pn, effusio, id,
            chorda_aedificator_finire(aed), VERUM);
        redde;
    }
    {
        ChordaAedificator* aed = chorda_aedificator_creare(pn,
            CCLVI);
        chorda status = gesta_res_status(t->mundus, res_id, pn);

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
        _textum_respondere(pn, effusio, id,
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
    chorda res_id;
    JsonValor* datum = json_objectum_creare(pn);
    constans character* genus_eventus = NIHIL;
    GestaEventum e;

    si (clavis.mensura == ZEPHYRUM || actus.mensura == ZEPHYRUM)
    {
        _textum_respondere(pn, effusio, id,
            _ch("res et actus requiruntur"), VERUM);
        redde;
    }
    res_id = _res_solvere(t, clavis, pn);
    si (res_id.mensura == ZEPHYRUM)
    {
        _textum_respondere(pn, effusio, id,
            _ch("res ignota (id aut titulus exactus)"), VERUM);
        redde;
    }
    si (_chorda_est(actus, "nota"))
    {
        chorda textus = _arg(argumenta, "textus");

        si (textus.mensura == ZEPHYRUM)
        {
            _textum_respondere(pn, effusio, id,
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
            _textum_respondere(pn, effusio, id,
                _ch("status: novus requiritur"), VERUM);
            redde;
        }
        genus_eventus = "status";
        json_objectum_ponere(datum, "novus",
            json_chorda_creare(pn, novus));
    }
    alioquin si (_chorda_est(actus, "nexus")
        || _chorda_est(actus, "denexus"))
    {
        chorda verbum = _arg(argumenta, "verbum");
        chorda alterum = _arg(argumenta, "alterum");
        chorda alterum_id;

        si (verbum.mensura == ZEPHYRUM
            || alterum.mensura == ZEPHYRUM)
        {
            _textum_respondere(pn, effusio, id,
                _ch("nexus: verbum et alterum requiruntur"),
                VERUM);
            redde;
        }
        alterum_id = _res_solvere(t, alterum, pn);
        genus_eventus = _chorda_est(actus, "nexus")
            ? "nexus" : "denexus";
        json_objectum_ponere(datum, "verbum",
            json_chorda_creare(pn, verbum));
        json_objectum_ponere(datum, "alterum",
            json_chorda_creare(pn,
                alterum_id.mensura > ZEPHYRUM
                    ? alterum_id : alterum));
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
                _textum_respondere(pn, effusio, id,
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
            _textum_respondere(pn, effusio, id,
                _ch("mutatio: datum aut clavis+valor"), VERUM);
            redde;
        }
    }
    alioquin si (_chorda_est(actus, "remotio"))
    {
        chorda clavis_r = _arg(argumenta, "clavis");

        si (clavis_r.mensura == ZEPHYRUM)
        {
            _textum_respondere(pn, effusio, id,
                _ch("remotio: clavis requiritur"), VERUM);
            redde;
        }
        genus_eventus = "remotio";
        json_objectum_ponere(datum, "clavis",
            json_chorda_creare(pn, clavis_r));
    }
    alioquin
    {
        _textum_respondere(pn, effusio, id,
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
    si (!gesta_scribere(t->mundus, &e, NIHIL))
    {
        ChordaAedificator* aed = chorda_aedificator_creare(pn,
            CCLVI);

        chorda_aedificator_appendere_literis(aed,
            "scriptura recusata: ");
        chorda_aedificator_appendere_literis(aed,
            gesta_error(t->mundus));
        _textum_respondere(pn, effusio, id,
            chorda_aedificator_finire(aed), VERUM);
        redde;
    }
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
        _textum_respondere(pn, effusio, id,
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
        _textum_respondere(pn, effusio, id,
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
        _textum_respondere(pn, effusio, id,
            _ch("apparatus quaestionis fractus"), VERUM);
        redde;
    }
    si (xar_numerus(inventa) == ZEPHYRUM)
    {
        _textum_respondere(pn, effusio, id,
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
    _textum_respondere(pn, effusio, id,
        chorda_aedificator_finire(aed), FALSUM);
}

interior vacuum
_tab_res (Tabularium* t, Piscina* pn, JsonValor* id,
    JsonValor* argumenta, FILE* effusio)
{
    chorda clavis = _arg(argumenta, "res");
    chorda res_id;
    chorda datum;
    JsonValor* st = NIHIL;
    ChordaAedificator* aed;

    si (clavis.mensura == ZEPHYRUM)
    {
        _textum_respondere(pn, effusio, id,
            _ch("res requiritur (id aut titulus)"), VERUM);
        redde;
    }
    res_id = _res_solvere(t, clavis, pn);
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
        _textum_respondere(pn, effusio, id,
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
    _textum_respondere(pn, effusio, id,
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
    _textum_respondere(pn, effusio, id,
        chorda_aedificator_finire(aed), FALSUM);
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
          FALSUM }
    };
    interior constans TabArgumentum ARG_GERERE[] = {
        { "res", "res_id aut titulus exactus", VERUM },
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
          FALSUM }
    };
    interior constans TabArgumentum ARG_QUAERERE[] = {
        { "textus", "quaestio FTS (praefixa 'termin*')", VERUM },
        { "genus", "filtrum generis", FALSUM },
        { "status", "filtrum statûs", FALSUM },
        { "tag", "filtrum tagi (terminus FTS additus)", FALSUM }
    };
    interior constans TabArgumentum ARG_RES[] = {
        { "res", "res_id aut titulus exactus", VERUM }
    };

    json_tabulatum_addere(instrumenta, _instrumentum(pn, "addere",
        "Rem novam creare (quaestio/parcum/decretum/nota/"
        "desideratum) cum tags et ancoris optionalibus.",
        ARG_ADDERE, VI));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "gerere",
        "Eventum unum in rem exsistentem scribere: nota, status,"
        " nexus/denexus (ligamina), mutatio, remotio. Violationes"
        " machinae notantur, non obstant.",
        ARG_GERERE, X));
    json_tabulatum_addere(instrumenta, _instrumentum(pn,
        "quaerere",
        "Quaestio FTS super statum materializatum (titulus/corpus/"
        "tags/notae). Idioma Latinum: praefixa 'parsur*'.",
        ARG_QUAERERE, IV));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "res",
        "Rem unam reddere: status + datum + ancorae (resolutae per"
        " indicem; CAUTIO si inresolutae) + annales recentes.",
        ARG_RES, I));
    json_tabulatum_addere(instrumenta, _instrumentum(pn, "census",
        "Census: genera x status, tags, seq/hwm.",
        NIHIL, ZEPHYRUM));
    json_objectum_ponere(resultatum, "tools", instrumenta);
    _respondere(effusio, tabellarius_responsum(pn, id,
        resultatum));
}

/* seminatio idempotens (INTENTIO C decisio 2) */
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
        json_objectum_ponere(servus, "version",
            json_chorda_creare_literis(pn, "0"));
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
