/* mcp_servus.c - nucleus servi MCP (vide include/mcp_servus.h)
 *
 * Extractus ex sceletis copiatis (officina/instrumenta/legatus.c,
 * gesta/fontes/tabularium.c: initialize, dispatch, schemata ex
 * tabulis); illi servi in sub-projecto B intacti manent.
 *
 * Vocatio quaeque locum in tabula fixa servi occupat (LXIV), etiam
 * synchrona: locus liberatur cum responsum redditur, generatio
 * augetur - manubria vetera irrita fiunt. Loci pleni -> error
 * internus (-32603), non ruina.
 */

#include "mcp_servus.h"
#include "chorda_aedificator.h"
#include "tabellarius.h"

#include <string.h>

structura McpVocatio {
                    McpServus* servus;
                          i32  locus;
                          b32  activa;
                          b32  dilata;
                          b32  responsa;
                    JsonValor* id;
                    JsonValor* argumenta;   /* objectum iudicatum */
     constans McpInstrumentum* instrumentum;
                   McpPersona  persona;
                      Piscina* piscina;
                   McpReddere  reddere;
                       vacuum* datum_reddendi;
};

structura McpServus {
    McpConfiguratio cfg;
                b32 initiatum;
         McpVocatio loci[MCP_VOCATIONES_MAXIMAE];
                i32 generationes[MCP_VOCATIONES_MAXIMAE];
};

/* versiones protocolli quas loquimur (aeram initialize) */
hic_manens constans character* constans VERSIONES[] = {
    "2025-03-26", "2025-06-18", "2025-11-25", NIHIL
};


/* ==================================================
 * auxilia
 * ================================================== */

interior b32
_est (
                 chorda  c,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde c.mensura == n
        && (n == ZEPHYRUM || memcmp(c.datum, literae, (size_t)n)
                == ZEPHYRUM);
}

interior vacuum
_reddere (
             McpReddere  reddere,
                 vacuum* datum,
        McpReditusGenus  genus,
                 chorda  corpus,
     constans character* scopus,
     constans character* instrumentum,
     constans character* exitus)
{
    McpReditus r;

    r.genus              = genus;
    r.corpus             = corpus;
    r.scopus_requisitus  = scopus;
    r.instrumentum       = instrumentum;
    r.exitus             = exitus;
    si (reddere)
    {
        reddere(datum, &r);
    }
}

interior vacuum
_errorem_reddere (
             McpReddere  reddere,
                 vacuum* datum,
                Piscina* p,
              JsonValor* id,
                 longus  codex,
                 chorda  nuntius,
     constans character* instrumentum)
{
    _reddere(reddere, datum, MCP_REDITUS_RESPONSUM,
        tabellarius_errorem(p, id, codex, chorda_ut_cstr(nuntius, p)),
        NIHIL, instrumentum, "error");
}

interior chorda
_nuntius (
                Piscina* p,
     constans character* initium,
                 chorda  medium,
     constans character* finis)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, LXIV);

    chorda_aedificator_appendere_literis(a, initium);
    chorda_aedificator_appendere_chorda(a, medium);
    chorda_aedificator_appendere_literis(a, finis);
    redde chorda_aedificator_finire(a);
}

interior chorda
_literae (
    constans character* s)
{
    chorda c;

    c.datum    = (i8*)(vacuum*)(memoriae_index)s;
    c.mensura  = (i32)strlen(s);
    redde c;
}

interior constans character*
_genus_titulus (
    McpGenus genus)
{
    commutatio (genus)
    {
        casus MCP_INTEGER:
            redde "integer";
        casus MCP_BOOLEAN:
            redde "boolean";
        casus MCP_CHORDA:
        ordinarius:
            redde "string";
    }
}


/* ==================================================
 * tools/list - schemata ex tabula
 * ================================================== */

interior JsonValor*
_instrumentum (
    constans McpInstrumentum* m,
                     Piscina* p)
{
    JsonValor* instr         = json_objectum_creare(p);
    JsonValor* schema        = json_objectum_creare(p);
    JsonValor* proprietates  = json_objectum_creare(p);
    JsonValor* necessaria    = json_tabulatum_creare(p);
    JsonValor* annotationes  = json_objectum_creare(p);
          i32  k;

    per (k = ZEPHYRUM; k < m->numerus; k++)
    {
        constans McpArgumentum* a    = &m->argumenta[k];
                     JsonValor* arg  = json_objectum_creare(p);

        json_objectum_ponere(arg, "type",
            json_chorda_creare_literis(p, _genus_titulus(a->genus)));
        si (a->descriptio)
        {
            json_objectum_ponere(arg, "description",
                json_chorda_creare_literis(p, a->descriptio));
        }
        si (a->electiones)
        {
            JsonValor* electiones = json_tabulatum_creare(p);
                  i32  e;

            per (e = ZEPHYRUM; a->electiones[e]; e++)
            {
                json_tabulatum_addere(electiones,
                    json_chorda_creare_literis(p, a->electiones[e]));
            }
            json_objectum_ponere(arg, "enum", electiones);
        }
        json_objectum_ponere(proprietates, a->titulus, arg);
        si (a->necessarium)
        {
            json_tabulatum_addere(necessaria,
                json_chorda_creare_literis(p, a->titulus));
        }
    }
    json_objectum_ponere(schema, "type",
        json_chorda_creare_literis(p, "object"));
    json_objectum_ponere(schema, "properties", proprietates);
    /* "required" vacuum omissum (schemata vetera minItems I) */
    si (json_tabulatum_numerus(necessaria) > ZEPHYRUM)
    {
        json_objectum_ponere(schema, "required", necessaria);
    }
    json_objectum_ponere(schema, "additionalProperties",
        json_boolean_creare(p, FALSUM));
    json_objectum_ponere(annotationes, "readOnlyHint",
        json_boolean_creare(p, m->legere_solum));
    json_objectum_ponere(annotationes, "destructiveHint",
        json_boolean_creare(p, m->delens));
    json_objectum_ponere(instr, "name",
        json_chorda_creare_literis(p, m->titulus));
    json_objectum_ponere(instr, "description",
        json_chorda_creare_literis(p, m->descriptio ? m->descriptio
            : ""));
    json_objectum_ponere(instr, "inputSchema", schema);
    json_objectum_ponere(instr, "annotations", annotationes);
    redde instr;
}

interior JsonValor*
_index (
    McpServus* s,
      Piscina* p)
{
    JsonValor* resultatum  = json_objectum_creare(p);
    JsonValor* tabula      = json_tabulatum_creare(p);
          i32  i;

    per (i = ZEPHYRUM; i < s->cfg.numerus; i++)
    {
        json_tabulatum_addere(tabula,
            _instrumentum(&s->cfg.instrumenta[i], p));
    }
    json_objectum_ponere(resultatum, "tools", tabula);
    redde resultatum;
}


/* ==================================================
 * iudicium argumentorum (ante tractatorem)
 * ================================================== */

interior constans McpArgumentum*
_argumentum (
    constans McpInstrumentum* m,
                      chorda  titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < m->numerus; k++)
    {
        si (_est(titulus, m->argumenta[k].titulus))
        {
            redde &m->argumenta[k];
        }
    }
    redde NIHIL;
}

interior b32
_genus_congruit (
      McpGenus  genus,
     JsonValor* v)
{
    commutatio (genus)
    {
        casus MCP_INTEGER:
            redde json_est_integer(v);
        casus MCP_BOOLEAN:
            redde json_est_boolean(v);
        casus MCP_CHORDA:
        ordinarius:
            redde json_est_chorda(v);
    }
}

/* chorda vacua = bene; aliter causa argumentum nominans */
interior chorda
_argumenta_iudicare (
    constans McpInstrumentum* m,
                   JsonValor* argumenta,
                     Piscina* p)
{
    JsonObjectumIterator  iter = json_objectum_iterator(argumenta);
                  chorda  clavis;
               JsonValor* valor;
                  chorda  bene;
                     i32  k;

    bene.datum    = NIHIL;
    bene.mensura  = ZEPHYRUM;
    dum (json_objectum_iterator_proxima(&iter, &clavis, &valor))
    {
        si (!_argumentum(m, clavis))
        {
            redde _nuntius(p, "argumentum ignotum: ", clavis, "");
        }
    }
    per (k = ZEPHYRUM; k < m->numerus; k++)
    {
        constans McpArgumentum* a = &m->argumenta[k];

        valor = json_objectum_capere(argumenta, a->titulus);
        si (!valor || json_est_nullum(valor))
        {
            si (a->necessarium)
            {
                redde _nuntius(p, "argumentum necessarium deest: ",
                    _literae(a->titulus), "");
            }
            perge;
        }
        si (!_genus_congruit(a->genus, valor))
        {
            ChordaAedificator* t = chorda_aedificator_creare(p, LXIV);

            chorda_aedificator_appendere_literis(t, " ");
            chorda_aedificator_appendere_literis(t,
                _genus_titulus(a->genus));
            chorda_aedificator_appendere_literis(t, " exspectatur");
            redde _nuntius(p, "argumentum ", _literae(a->titulus),
                chorda_ut_cstr(chorda_aedificator_finire(t), p));
        }
        si (a->electiones)
        {
            ChordaAedificator* t;
                       chorda  v = json_ad_chorda(valor);
                          i32  e;
                          b32  inventa = FALSUM;

            per (e = ZEPHYRUM; a->electiones[e]; e++)
            {
                si (_est(v, a->electiones[e]))
                {
                    inventa = VERUM;
                }
            }
            si (inventa)
            {
                perge;
            }
            t = chorda_aedificator_creare(p, LXIV);
            chorda_aedificator_appendere_literis(t,
                ": valor non inter electiones (");
            per (e = ZEPHYRUM; a->electiones[e]; e++)
            {
                si (e > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_literis(t, ", ");
                }
                chorda_aedificator_appendere_literis(t,
                    a->electiones[e]);
            }
            chorda_aedificator_appendere_literis(t, ")");
            redde _nuntius(p, "argumentum ", _literae(a->titulus),
                chorda_ut_cstr(chorda_aedificator_finire(t), p));
        }
    }
    redde bene;
}


/* ==================================================
 * scopi
 * ================================================== */

/* signum integrum in indice spatiis separato */
interior b32
_scopum_habet (
    constans McpPersona* persona,
     constans character* scopus)
{
    i32 n;
    i32 i = ZEPHYRUM;

    si (!scopus)
    {
        redde VERUM;
    }
    si (!persona)
    {
        redde FALSUM;
    }
    n = (i32)strlen(scopus);
    dum (i < persona->scopi.mensura)
    {
        i32 initium;

        dum (   i < persona->scopi.mensura
             && persona->scopi.datum[i] == ' ')
        {
            i++;
        }
        initium = i;
        dum (   i < persona->scopi.mensura
             && persona->scopi.datum[i] != ' ')
        {
            i++;
        }
        si (   i - initium == n && n > ZEPHYRUM
            && memcmp(persona->scopi.datum + initium, scopus, (size_t)n)
                == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * responsa vocationum
 * ================================================== */

/* locus liberatur ANTE reddere: reddere vocationem novam incipere
 * potest */
interior vacuum
_finire (
            McpVocatio* v,
       McpReditusGenus  genus,
                chorda  corpus,
    constans character* exitus)
{
    McpReddere  reddere  = v->reddere;
        vacuum* datum    = v->datum_reddendi;

    v->responsa  = VERUM;
    v->activa    = FALSUM;
    v->servus->generationes[v->locus]++;
    _reddere(reddere, datum, genus, corpus, NIHIL,
        v->instrumentum->titulus, exitus);
}

interior b32
_respondere_potest (
    McpVocatio* v)
{
    redde v != NIHIL && v->activa && !v->responsa;
}

interior vacuum
_resultatum (
    McpVocatio* v,
        chorda  textus,
           b32  vitium,
     JsonValor* valor)
{
       Piscina* p           = v->piscina;
     JsonValor* resultatum  = json_objectum_creare(p);
     JsonValor* contentus   = json_tabulatum_creare(p);
     JsonValor* fragmentum  = json_objectum_creare(p);

    json_objectum_ponere(fragmentum, "type",
        json_chorda_creare_literis(p, "text"));
    json_objectum_ponere(fragmentum, "text", json_chorda_creare(p,
        textus));
    json_tabulatum_addere(contentus, fragmentum);
    json_objectum_ponere(resultatum, "content", contentus);
    si (valor)
    {
        json_objectum_ponere(resultatum, "structuredContent", valor);
    }
    json_objectum_ponere(resultatum, "isError", json_boolean_creare(p,
        vitium));
    _finire(v, MCP_REDITUS_RESPONSUM, tabellarius_responsum(p, v->id,
        resultatum), vitium ? "vitium" : "bene");
}

vacuum
mcp_respondere_textum (
    McpVocatio* vocatio,
        chorda  textus)
{
    si (_respondere_potest(vocatio))
    {
        _resultatum(vocatio, textus, FALSUM, NIHIL);
    }
}

vacuum
mcp_respondere_vitium (
    McpVocatio* vocatio,
        chorda  textus)
{
    si (_respondere_potest(vocatio))
    {
        _resultatum(vocatio, textus, VERUM, NIHIL);
    }
}

vacuum
mcp_respondere_json (
    McpVocatio* vocatio,
     JsonValor* valor)
{
    si (_respondere_potest(vocatio) && valor)
    {
        _resultatum(vocatio, json_scribere(valor, vocatio->piscina),
            FALSUM, valor);
    }
}

McpDilatum
mcp_differre (
    McpVocatio* vocatio)
{
    McpDilatum d;

    d.servus     = NIHIL;
    d.locus      = ZEPHYRUM;
    d.generatio  = ZEPHYRUM;
    si (_respondere_potest(vocatio))
    {
        vocatio->dilata  = VERUM;
        d.servus         = vocatio->servus;
        d.locus          = vocatio->locus;
        d.generatio =
            vocatio->servus->generationes[vocatio->locus];
    }
    redde d;
}

McpVocatio*
mcp_dilatum_vocatio (
    McpDilatum dilatum)
{
    McpVocatio* v;

    si (!dilatum.servus || dilatum.locus >= MCP_VOCATIONES_MAXIMAE)
    {
        redde NIHIL;
    }
    v = &dilatum.servus->loci[dilatum.locus];
    si (   !v->activa || !v->dilata
        || dilatum.servus->generationes[dilatum.locus]
               != dilatum.generatio)
    {
        redde NIHIL;
    }
    redde v;
}


/* ==================================================
 * vocatio (accessus tractatoris)
 * ================================================== */

constans McpPersona*
mcp_vocatio_persona (
    McpVocatio* vocatio)
{
    redde vocatio ? &vocatio->persona : NIHIL;
}

vacuum*
mcp_vocatio_datum (
    McpVocatio* vocatio)
{
    redde vocatio ? vocatio->servus->cfg.datum : NIHIL;
}

Piscina*
mcp_vocatio_piscina (
    McpVocatio* vocatio)
{
    redde vocatio ? vocatio->piscina : NIHIL;
}

/* valor praesens (non null) argumenti iudicati */
interior JsonValor*
_valor (
             McpVocatio* vocatio,
     constans character* titulus)
{
    JsonValor* v;

    si (!vocatio || !vocatio->argumenta)
    {
        redde NIHIL;
    }
    v = json_objectum_capere(vocatio->argumenta, titulus);
    redde (v && !json_est_nullum(v)) ? v : NIHIL;
}

b32
mcp_argumentum_chorda (
             McpVocatio* vocatio,
     constans character* titulus,
                 chorda* exitus)
{
    JsonValor* v = _valor(vocatio, titulus);

    si (!v || !json_est_chorda(v))
    {
        redde FALSUM;
    }
    si (exitus)
    {
        *exitus = json_ad_chorda(v);
    }
    redde VERUM;
}

b32
mcp_argumentum_integer (
             McpVocatio* vocatio,
     constans character* titulus,
                    s64* exitus)
{
    JsonValor* v = _valor(vocatio, titulus);

    si (!v || !json_est_integer(v))
    {
        redde FALSUM;
    }
    si (exitus)
    {
        *exitus = json_ad_integer(v);
    }
    redde VERUM;
}

b32
mcp_argumentum_boolean (
             McpVocatio* vocatio,
     constans character* titulus,
                    b32* exitus)
{
    JsonValor* v = _valor(vocatio, titulus);

    si (!v || !json_est_boolean(v))
    {
        redde FALSUM;
    }
    si (exitus)
    {
        *exitus = json_ad_boolean(v);
    }
    redde VERUM;
}


/* ==================================================
 * methodi
 * ================================================== */

interior vacuum
_initialize (
     McpServus* s,
     JsonValor* id,
     JsonValor* params,
    McpReddere  reddere,
        vacuum* datum,
       Piscina* p)
{
             JsonValor* resultatum  = json_objectum_creare(p);
             JsonValor* caps        = json_objectum_creare(p);
             JsonValor* servus      = json_objectum_creare(p);
                chorda  petita      = json_ad_chorda(
                    json_objectum_capere(params, "protocolVersion"));
    constans character* versio      = MCP_PROTOCOLLUM_NOVISSIMUM;
                   i32  k;

    per (k = ZEPHYRUM; VERSIONES[k]; k++)
    {
        si (_est(petita, VERSIONES[k]))
        {
            versio = VERSIONES[k];
        }
    }
    json_objectum_ponere(resultatum, "protocolVersion",
        json_chorda_creare_literis(p, versio));
    json_objectum_ponere(caps, "tools", json_objectum_creare(p));
    json_objectum_ponere(resultatum, "capabilities", caps);
    json_objectum_ponere(servus, "name", json_chorda_creare_literis(p,
        s->cfg.titulus ? s->cfg.titulus : ""));
    json_objectum_ponere(servus, "version",
        json_chorda_creare_literis(p, s->cfg.versio ? s->cfg.versio
            : "0"));
    json_objectum_ponere(resultatum, "serverInfo", servus);
    si (s->cfg.instructiones)
    {
        json_objectum_ponere(resultatum, "instructions",
            json_chorda_creare_literis(p, s->cfg.instructiones));
    }
    s->initiatum = VERUM;
    _reddere(reddere, datum, MCP_REDITUS_RESPONSUM,
        tabellarius_responsum(p, id, resultatum), NIHIL, NIHIL, "bene");
}

interior b32
_vocare (
              McpServus* s,
             JsonValor* id,
             JsonValor* params,
    constans McpPersona* persona,
             McpReddere  reddere,
                 vacuum* datum,
               Piscina* p)
{
     constans McpInstrumentum* m = NIHIL;
                       chorda  titulus;
                    JsonValor* argumenta;
                       chorda  causa;
                   McpVocatio* v;
                          i32  i;
                          i32  locus = MCP_VOCATIONES_MAXIMAE;

    si (!params || !json_est_objectum(params))
    {
        _errorem_reddere(reddere, datum, p, id,
            TABELLARIUS_ERROR_PARAMETRA_INVALIDA,
            _literae("params desunt"), NIHIL);
        redde VERUM;
    }
    titulus = json_ad_chorda(json_objectum_capere(params, "name"));
    per (i = ZEPHYRUM; i < s->cfg.numerus; i++)
    {
        si (_est(titulus, s->cfg.instrumenta[i].titulus))
        {
            m = &s->cfg.instrumenta[i];
        }
    }
    si (!m)
    {
        _errorem_reddere(reddere, datum, p, id,
            TABELLARIUS_ERROR_PARAMETRA_INVALIDA,
            _nuntius(p, "instrumentum ignotum: ", titulus, ""), NIHIL);
        redde VERUM;
    }
    /* scopus ANTE argumenta: sine scopo nihil de argumentis dicitur */
    si (!_scopum_habet(persona, m->scopus))
    {
        _reddere(reddere, datum, MCP_REDITUS_SCOPUS,
            tabellarius_errorem(p, id, MCP_ERROR_SCOPUS,
                chorda_ut_cstr(_nuntius(p, "scopus deest: ",
                    _literae(m->scopus), ""), p)), m->scopus,
                    m->titulus, "scopus");
        redde VERUM;
    }
    argumenta = json_objectum_capere(params, "arguments");
    si (!argumenta || json_est_nullum(argumenta))
    {
        argumenta = json_objectum_creare(p);
    }
    si (!json_est_objectum(argumenta))
    {
        _errorem_reddere(reddere, datum, p, id,
            TABELLARIUS_ERROR_PARAMETRA_INVALIDA,
            _literae("arguments objectum esse debet"), m->titulus);
        redde VERUM;
    }
    causa = _argumenta_iudicare(m, argumenta, p);
    si (causa.mensura > ZEPHYRUM)
    {
        _errorem_reddere(reddere, datum, p, id,
            TABELLARIUS_ERROR_PARAMETRA_INVALIDA, causa, m->titulus);
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < MCP_VOCATIONES_MAXIMAE; i++)
    {
        si (!s->loci[i].activa)
        {
            locus = i;
            frange;
        }
    }
    si (locus == MCP_VOCATIONES_MAXIMAE)
    {
        _errorem_reddere(reddere, datum, p, id,
            TABELLARIUS_ERROR_INTERNUS,
            _literae("nimis multae vocationes pendentes"), m->titulus);
        redde VERUM;
    }
    v = &s->loci[locus];
    memset(v, ZEPHYRUM, magnitudo(*v));
    v->servus          = s;
    v->locus           = locus;
    v->activa          = VERUM;
    v->id              = id;
    v->argumenta       = argumenta;
    v->instrumentum    = m;
    v->piscina         = p;
    v->reddere         = reddere;
    v->datum_reddendi  = datum;
    si (persona)
    {
        v->persona.usor    = chorda_transcribere(persona->usor, p);
        v->persona.cliens  = chorda_transcribere(persona->cliens, p);
        v->persona.scopi   = chorda_transcribere(persona->scopi, p);
    }
    m->tractator(v);
    si (v->responsa)
    {
        redde VERUM;
    }
    si (v->dilata)
    {
        redde FALSUM;
    }
    _finire(v, MCP_REDITUS_RESPONSUM, tabellarius_errorem(p, id,
        TABELLARIUS_ERROR_INTERNUS, "tractator nihil respondit"),
        "error");
    redde VERUM;
}


/* ==================================================
 * servus
 * ================================================== */

McpServus*
mcp_servus_creare (
    constans McpConfiguratio* configuratio,
                     Piscina* piscina)
{
    McpServus* s;

    si (   !configuratio || !piscina
        || (configuratio->numerus > ZEPHYRUM
            && !configuratio->instrumenta))
    {
        redde NIHIL;
    }
    s = (McpServus*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(McpServus),
        (memoriae_index)magnitudo(vacuum*));
    si (!s)
    {
        redde NIHIL;
    }
    memset(s, ZEPHYRUM, magnitudo(*s));
    s->cfg = *configuratio;
    redde s;
}

b32
mcp_servus_tractare (
              McpServus* servus,
                 chorda  corpus,
    constans McpPersona* persona,
                    b32  initium_postulatum,
             McpReddere  reddere,
                 vacuum* datum_reddendi,
               Piscina* piscina)
{
    TabellariusNuntius n;
                chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (!servus || !piscina)
    {
        redde VERUM;
    }
    n = tabellarius_nuntium_legere(corpus, piscina);
    si (n.genus == TABELLARIUS_PRAVUM)
    {
        si (n.radix == NIHIL)
        {
            _reddere(reddere, datum_reddendi, MCP_REDITUS_PRAVUM,
                tabellarius_errorem(piscina, NIHIL,
                    TABELLARIUS_ERROR_ANALYSIS, "analysis fracta"),
                NIHIL, NIHIL, "pravum");
        }
        alioquin
        {
            _reddere(reddere, datum_reddendi, MCP_REDITUS_PRAVUM,
                tabellarius_errorem(piscina, n.id,
                    TABELLARIUS_ERROR_PETITIO_INVALIDA,
                    json_est_tabulatum(n.radix)
                        ? "fasces non admittuntur"
                        : "petitio invalida"), NIHIL, NIHIL, "pravum");
        }
        redde VERUM;
    }
    si (n.genus != TABELLARIUS_PETITIO)
    {
        /* nuntiationes et responsa (numquam petimus): omissa */
        _reddere(reddere, datum_reddendi, MCP_REDITUS_NUNTIATIO, vacua,
            NIHIL, NIHIL, "nuntiatio");
        redde VERUM;
    }
    si (_est(n.methodus, "initialize"))
    {
        _initialize(servus, n.id, n.params, reddere, datum_reddendi,
            piscina);
        redde VERUM;
    }
    si (_est(n.methodus, "ping"))
    {
        _reddere(reddere, datum_reddendi, MCP_REDITUS_RESPONSUM,
            tabellarius_responsum(piscina, n.id,
                json_objectum_creare(piscina)), NIHIL, NIHIL, "bene");
        redde VERUM;
    }
    si (initium_postulatum && !servus->initiatum)
    {
        _reddere(reddere, datum_reddendi, MCP_REDITUS_RESPONSUM,
            tabellarius_errorem(piscina, n.id,
                TABELLARIUS_ERROR_NONDUM_INITIATUM, "nondum initiatum"),
            NIHIL, NIHIL, "error");
        redde VERUM;
    }
    si (_est(n.methodus, "tools/list"))
    {
        _reddere(reddere, datum_reddendi, MCP_REDITUS_RESPONSUM,
            tabellarius_responsum(piscina, n.id, _index(servus,
                piscina)), NIHIL, NIHIL, "bene");
        redde VERUM;
    }
    si (_est(n.methodus, "tools/call"))
    {
        redde _vocare(servus, n.id, n.params, persona, reddere,
            datum_reddendi, piscina);
    }
    _reddere(reddere, datum_reddendi, MCP_REDITUS_RESPONSUM,
        tabellarius_errorem(piscina, n.id,
        TABELLARIUS_ERROR_METHODUS_IGNOTA,
            "methodus ignota"), NIHIL, NIHIL, "error");
    redde VERUM;
}


/* ==================================================
 * stdio
 * ================================================== */

/* status lineae: piscina sua; dilata -> reddere eam destruit */
nomen structura {
        FILE* extra;
     Piscina* piscina;
         b32  dilata;
         s32* status;    /* NIHIL post ansam (dilata) */
} McpLineaStdio;

interior vacuum
_stdio_reddere (
                 vacuum* datum,
    constans McpReditus* r)
{
    McpLineaStdio* l = (McpLineaStdio*)datum;

    si (   r->corpus.mensura > ZEPHYRUM
        && !tabellarius_lineam_scribere(l->extra, r->corpus)
        && l->status)
    {
        *l->status = I;
    }
    si (l->dilata)
    {
        piscina_destruere(l->piscina);
    }
}

s32
mcp_servus_stdio_currere (
              McpServus* servus,
                   FILE* intra,
                   FILE* extra,
    constans McpPersona* persona)
{
    s32 status = ZEPHYRUM;

    si (!servus || !intra || !extra)
    {
        redde I;
    }
    dum (VERUM)
    {
              Piscina* p = piscina_generare_dynamicum("mcp_stdio",
                  (memoriae_index)65536);
        McpLineaStdio* l;
               chorda  linea;
                  b32  finitus = FALSUM;

        si (!p)
        {
            redde I;
        }
        l = (McpLineaStdio*)piscina_allocare_ordinatum(p,
            (memoriae_index)magnitudo(McpLineaStdio),
            (memoriae_index)magnitudo(vacuum*));
        linea = tabellarius_lineam_legere(intra, p, &finitus);
        si (finitus || !l)
        {
            piscina_destruere(p);
            redde status;
        }
        l->extra    = extra;
        l->piscina  = p;
        l->dilata   = FALSUM;
        l->status   = &status;
        si (mcp_servus_tractare(servus, linea, persona, VERUM,
                _stdio_reddere, l, p))
        {
            piscina_destruere(p);
        }
        alioquin
        {
            l->dilata = VERUM;
            l->status = NIHIL;
        }
    }
}
