/* apps/villa/villa.c - villa: fenestra vitreae in servos remotos.
 *
 * V4b-2: stratum tractatorum plenum; facies (index.html) adhuc
 * placeholder - V4b-3 eam aedificat. Indicem servorum FACIES
 * possidet (entia 'servus' ex mundo per transmittere), et horaria
 * probationum quoque: C-latus nullam auto-probationem habet praeter
 * primam in initio (facies aliquid statim videt).
 *
 * OMNE OPUS VERUM IN lib/villa_agens.c HABITAT. Haec plagula
 * nectit, non agit - ideo probationes radicis agentem custodire
 * possunt quamvis apps/ a nullo cursore verratur.
 *
 * TICTUS NUMQUAM OBSTAT: villa_agens_pulsare gradum unum omnium
 * probationum ET actionum currentium agit et statim redit. Vide
 * processus.h de causa (domus filis CARET).
 *
 * Tractatores IS (clavis absens = servus praetermissionis, pro
 * evolutione et fumo):
 *   villa_status {}                    -> summarium omnium sediumm
 *   villa_probatio {clavis?}           -> photographia plena una
 *   villa_probare {clavis?, alias?, unitates?[]} -> probatio nova
 *   villa_agere {clavis?, alias?, res?, imperium} -> actio nova
 *   villa_actio {clavis?}              -> status + fructus actionis
 *   villa_abrumpere {clavis?}          -> actionem desistere
 *   transmittere {...}                 -> mundus (allowlist)
 *
 * Vexillum -fumus: sine oculis - configuratio, semina, probatio
 * una per stipitem, numeri impressi, exitus.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "fenestra.h"
#include "capsula.h"
#include "vitrea.h"
#include "internuntius.h"
#include "speculum.h"
#include "cliens_tabularii.h"
#include "villa_agens.h"
#include "villa_lectores.h"
#include "capsula_villa.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* capsula fontium speculi - externus directus (mos consumptoris:
 * caput generatum non includitur, silva ".." non resolvit) */
/* <aedilis obiectum="build/speculum/villa/capsula_speculi_villa.c"/> */
externus constans CapsulaEmbed capsula_speculi_villa;

#define VILLA_PORTUS_ORDINARIUS 8753
#define VILLA_TICTUS_MS         CC
/* fumus faciei: CCC ticti x ~CC ms = ~LX s. Catena villae (status
 * -> index -> detalia -> actio) longior quam fori, ergo budgetum
 * maius quam CL fori. */
#define VILLA_FUMUS_GRESSUS_MAXIMI 300

nomen structura {
      ClientTabularii  cliens;
    VillaConfiguratio  configuratio;
           VillaAgens* agens;
                  Xar* unitates;    /* chorda - praetermissio */
               chorda  alias;       /* praetermissio (-alias, fumus) */
               chorda  clavis;
                  i32  cycli;

    /* fumus faciei (-fumus-facies): fenestra vera, IS choreographiam
	 * agit et fumus_perfectus vocat; C verdictum exspectat */
          b32 fumus_facies;
          b32 fumus_perfectus_est;
          b32 fumus_bene;
    character fumus_nuntius[CCLVI];
} VillaStatus;

interior chorda
_ch (
    constans character* litterae)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l        = litterae;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(litterae);
    redde c;
}

/* adiutores serendi - dimidium strepitus json tollunt */
interior vacuum
_pone_ch (
             JsonValor* o,
    constans character* clavis,
                chorda  valor,
               Piscina* pn)
{
    json_objectum_ponere(o, clavis, json_chorda_creare(pn, valor));
}

interior vacuum
_pone_n (
             JsonValor* o,
    constans character* clavis,
                   s64  valor,
               Piscina* pn)
{
    json_objectum_ponere(o, clavis, json_integer_creare(pn, valor));
}

interior vacuum
_pone_b (
             JsonValor* o,
    constans character* clavis,
                   b32  valor,
               Piscina* pn)
{
    json_objectum_ponere(o, clavis, json_boolean_creare(pn, valor));
}

/* chordam ex argumentis capere (vacua si clavis abest) */
interior chorda
_arg_ch (
             JsonValor* argumenta,
    constans character* clavis)
{
    si (argumenta == NIHIL)
    {
        chorda vacua;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde vacua;
    }
    redde json_ad_chorda(json_objectum_capere(argumenta, clavis));
}


/* ========================================================================
 * TRACTATORES INTERNUNTII
 * ======================================================================== */

/* villa_status {} -> configuratio + summarium omnium sediumm.
 * Indicem servorum FACIES possidet (entia 'servus' ex mundo per
 * transmittere legere); hic solum status probationum per clavem.
 * Tempus probationis ultimae JS notat (photographia horologium non
 * fert - JS horaria possidet, ergo et horologia). */
interior JsonValor*
_villa_status (
    JsonValor* argumenta,
      Piscina* piscina,
       vacuum* datum,
       chorda* culpa)
{
    VillaStatus* v      = (VillaStatus*)datum;
      JsonValor* r      = json_objectum_creare(piscina);
      JsonValor* sedes  = json_tabulatum_creare(piscina);
            i32  i;

    (vacuum)argumenta;
    (vacuum)culpa;

    _pone_ch(r, "alias", v->alias, piscina);
    json_objectum_ponere(r, "via_ssh",
        json_chorda_creare_literis(piscina,
            (v->configuratio.via_ssh != NIHIL)
                ? v->configuratio.via_ssh : "ssh"));
    _pone_ch(r, "via_moderandi", v->configuratio.via_moderandi,
        piscina);
    _pone_n(r, "currentes",
        (s64)villa_probationes_currentes(v->agens), piscina);
    _pone_n(r, "cycli", (s64)v->cycli, piscina);

    per (i = ZEPHYRUM; i < villa_sedes_numerus(v->agens); i++)
    {
                        chorda clavis =
                            villa_sedes_clavis(v->agens, i);
                     JsonValor* o = json_objectum_creare(piscina);
        constans ProbatioServi* p =
            villa_probatio_ultima(v->agens, clavis);

        _pone_ch(o, "clavis", clavis, piscina);
        json_objectum_ponere(o, "iudicium",
            json_chorda_creare_literis(piscina,
                villa_iudicium_nomen(villa_iudicare(p))));
        _pone_b(o, "probatio_currit",
            villa_probatio_currit(v->agens, clavis), piscina);
        _pone_b(o, "actio_currit",
            villa_actio_currit(v->agens, clavis), piscina);
        si (p != NIHIL)
        {
            _pone_b(o, "felix", p->felix, piscina);
            _pone_n(o, "mora_ms", (s64)p->mora_ms, piscina);
            _pone_ch(o, "querelae", p->querelae, piscina);
            _pone_n(o, "servitia", (s64)((p->servitia != NIHIL)
                ? xar_numerus(p->servitia) : ZEPHYRUM), piscina);
            _pone_n(o, "situs", (s64)((p->situs != NIHIL)
                ? xar_numerus(p->situs) : ZEPHYRUM), piscina);
            _pone_n(o, "discus_capacitas",
                (s64)p->discus.capacitas, piscina);
        }
        json_tabulatum_addere(sedes, o);
    }
    json_objectum_ponere(r, "sedes", sedes);
    redde r;
}

/* villa_probatio {clavis?} -> photographia plena parsata unius
 * servi - fundamentum visus detaliorum */
interior JsonValor*
_villa_probatio (
    JsonValor* argumenta,
      Piscina* piscina,
       vacuum* datum,
       chorda* culpa)
{
               VillaStatus* v       = (VillaStatus*)datum;
                    chorda  clavis  = _arg_ch(argumenta, "clavis");
                 JsonValor* r       = json_objectum_creare(piscina);
    constans ProbatioServi* p;
                       i32  i;

    (vacuum)culpa;
    si (clavis.mensura == ZEPHYRUM)
    {
        clavis = v->clavis;
    }
    p = villa_probatio_ultima(v->agens, clavis);
    _pone_ch(r, "clavis", clavis, piscina);
    json_objectum_ponere(r, "iudicium",
        json_chorda_creare_literis(piscina,
            villa_iudicium_nomen(villa_iudicare(p))));
    _pone_b(r, "probatio_currit",
        villa_probatio_currit(v->agens, clavis), piscina);
    si (p == NIHIL)
    {
        redde r;
    }
    _pone_b(r, "felix", p->felix, piscina);
    _pone_n(r, "mora_ms", (s64)p->mora_ms, piscina);
    json_objectum_ponere(r, "causa",
        json_chorda_creare_literis(piscina,
            villa_exitus_nomen(p->causa.genus)));
    _pone_ch(r, "causa_textus", p->causa.causa, piscina);
    _pone_ch(r, "querelae", p->querelae, piscina);
    _pone_n(r, "tempus_activum", (s64)p->tempus_activum, piscina);

    {
        JsonValor* t = json_tabulatum_creare(piscina);

        si (p->unitates != NIHIL)
        {
            per (i = ZEPHYRUM; i < xar_numerus(p->unitates); i++)
            {
                constans UnitasCursoria* u =
                    (constans UnitasCursoria*)
                    xar_obtinere(p->unitates, i);
                JsonValor* o = json_objectum_creare(piscina);

                si (u == NIHIL)
                {
                    perge;
                }
                _pone_ch(o, "unitas", u->unitas, piscina);
                _pone_ch(o, "vita", u->vita, piscina);
                _pone_ch(o, "sub", u->sub, piscina);
                _pone_ch(o, "descriptio", u->descriptio, piscina);
                json_tabulatum_addere(t, o);
            }
        }
        json_objectum_ponere(r, "unitates", t);
    }
    {
        JsonValor* t = json_tabulatum_creare(piscina);

        si (p->servitia != NIHIL)
        {
            per (i = ZEPHYRUM; i < xar_numerus(p->servitia); i++)
            {
                constans StatusServitii* s =
                    (constans StatusServitii*)
                    xar_obtinere(p->servitia, i);
                JsonValor* o = json_objectum_creare(piscina);

                si (s == NIHIL)
                {
                    perge;
                }
                _pone_ch(o, "id", s->id, piscina);
                _pone_ch(o, "descriptio", s->descriptio, piscina);
                _pone_ch(o, "vita", s->status_vitae, piscina);
                _pone_ch(o, "sub", s->sub_status, piscina);
                _pone_ch(o, "tempus_initii", s->tempus_initii,
                    piscina);
                _pone_b(o, "inventa", s->inventa, piscina);
                _pone_b(o, "currit", s->currit, piscina);
                _pone_b(o, "fracta", s->fracta, piscina);
                /* causa_finis nihil significat nisi fracta - lex
				 * systemd: 'Result=success' etiam mortuis et
				 * ignotis redditur. Non emissa = non ostensa. */
                si (s->fracta)
                {
                    _pone_ch(o, "causa_finis", s->causa_finis,
                        piscina);
                    _pone_n(o, "codex", (s64)s->codex_exitus,
                        piscina);
                }
                _pone_n(o, "pid", (s64)s->pid, piscina);
                _pone_n(o, "restitutiones", (s64)s->restitutiones,
                    piscina);
                json_tabulatum_addere(t, o);
            }
        }
        json_objectum_ponere(r, "servitia", t);
    }
    {
        JsonValor* t = json_tabulatum_creare(piscina);

        si (p->situs != NIHIL)
        {
            per (i = ZEPHYRUM; i < xar_numerus(p->situs); i++)
            {
                constans SitusNginx* s = (constans SitusNginx*)
                    xar_obtinere(p->situs, i);
                JsonValor* o = json_objectum_creare(piscina);

                si (s == NIHIL)
                {
                    perge;
                }
                _pone_ch(o, "hospes", s->hospes, piscina);
                _pone_ch(o, "hospites", s->hospites, piscina);
                _pone_ch(o, "destinatio", s->destinatio, piscina);
                _pone_n(o, "destinationes", (s64)s->destinationes,
                    piscina);
                _pone_ch(o, "certificatum", s->certificatum,
                    piscina);
                _pone_ch(o, "radix", s->radix, piscina);
                _pone_b(o, "ssl", s->ssl, piscina);
                _pone_b(o, "redirectio", s->habet_redirectionem,
                    piscina);
                json_tabulatum_addere(t, o);
            }
        }
        json_objectum_ponere(r, "situs", t);
    }
    {
        JsonValor* d = json_objectum_creare(piscina);

        _pone_ch(d, "systema", p->discus.systema, piscina);
        _pone_ch(d, "punctum", p->discus.punctum, piscina);
        _pone_n(d, "frusta", (s64)p->discus.frusta, piscina);
        _pone_n(d, "usa", (s64)p->discus.usa, piscina);
        _pone_n(d, "praesto", (s64)p->discus.praesto, piscina);
        _pone_n(d, "capacitas", (s64)p->discus.capacitas, piscina);
        json_objectum_ponere(r, "discus", d);
    }
    {
        JsonValor* m = json_objectum_creare(piscina);

        _pone_n(m, "summa_kb", (s64)p->memoria.summa_kb, piscina);
        _pone_n(m, "libera_kb", (s64)p->memoria.libera_kb, piscina);
        _pone_n(m, "praesto_kb", (s64)p->memoria.praesto_kb,
            piscina);
        json_objectum_ponere(r, "memoria", m);
    }
    redde r;
}

/* villa_probare {clavis?, alias?, unitates?[]} - probationem
 * incipere (non obstat). Sine argumentis: servus praetermissionis
 * (-alias) - semita evolutionis et fumi immutata. */
interior JsonValor*
_villa_probare (
    JsonValor* argumenta,
      Piscina* piscina,
       vacuum* datum,
       chorda* culpa)
{
     VillaStatus* v         = (VillaStatus*)datum;
       JsonValor* r         = json_objectum_creare(piscina);
          chorda  clavis    = _arg_ch(argumenta, "clavis");
          chorda  alias     = _arg_ch(argumenta, "alias");
          chorda  causa     = _ch("");
    constans Xar* unitates  = v->unitates;
             b32  bene;

    (vacuum)culpa;
    si (clavis.mensura == ZEPHYRUM)
    {
        clavis = v->clavis;
    }
    si (alias.mensura == ZEPHYRUM)
    {
        alias = v->alias;
    }
    /* unitates propriae ex JS (tabulatum chordarum) - chordae in
	 * piscinam vocationis monstrant, quod sufficit: scriptum eas
	 * intra hanc vocationem transcribit */
    si (argumenta != NIHIL)
    {
        JsonValor* t = json_objectum_capere(argumenta, "unitates");

        si (t != NIHIL && json_est_tabulatum(t))
        {
            Xar* propriae = xar_creare(piscina,
                (i32)magnitudo(chorda));
            i32 i;

            per (i = ZEPHYRUM; i < json_tabulatum_numerus(t); i++)
            {
                JsonValor* e = json_tabulatum_obtinere(t, i);

                si (e != NIHIL && json_est_chorda(e))
                {
                    chorda* n = (chorda*)xar_addere(propriae);

                    si (n != NIHIL)
                    {
                        *n = json_ad_chorda(e);
                    }
                }
            }
            unitates = propriae;
        }
    }
    bene = villa_probationem_incipere(v->agens, clavis, alias,
        unitates, &causa);
    _pone_b(r, "incepta", bene, piscina);
    _pone_ch(r, "causa", causa, piscina);
    redde r;
}

/* villa_agere {clavis?, alias?, res?, imperium} - actio
 * incrementalis (non obstat); fructus per villa_actio legitur */
interior JsonValor*
_villa_agere (
    JsonValor* argumenta,
      Piscina* piscina,
       vacuum* datum,
       chorda* culpa)
{
    VillaStatus* v         = (VillaStatus*)datum;
      JsonValor* r         = json_objectum_creare(piscina);
         chorda  clavis    = _arg_ch(argumenta, "clavis");
         chorda  alias     = _arg_ch(argumenta, "alias");
         chorda  res       = _arg_ch(argumenta, "res");
         chorda  imperium  = _arg_ch(argumenta, "imperium");
         chorda  causa     = _ch("");
            b32  bene;

    (vacuum)culpa;
    si (clavis.mensura == ZEPHYRUM)
    {
        clavis = v->clavis;
    }
    si (alias.mensura == ZEPHYRUM)
    {
        alias = v->alias;
    }
    bene = villa_actionem_incipere(v->agens, clavis, alias, res,
        imperium, &causa);
    _pone_b(r, "incepta", bene, piscina);
    _pone_ch(r, "causa", causa, piscina);
    redde r;
}

/* villa_actio {clavis?} - status actionis + fructus ultimus */
interior JsonValor*
_villa_actio (
    JsonValor* argumenta,
      Piscina* piscina,
       vacuum* datum,
       chorda* culpa)
{
                  VillaStatus* v = (VillaStatus*)datum;
                    JsonValor* r =
                        json_objectum_creare(piscina);
                       chorda clavis = _arg_ch(argumenta,
                           "clavis");
    constans ResultusActionis* f;

    (vacuum)culpa;
    si (clavis.mensura == ZEPHYRUM)
    {
        clavis = v->clavis;
    }
    _pone_b(r, "currit", villa_actio_currit(v->agens, clavis),
        piscina);
    f = villa_actio_ultima(v->agens, clavis);
    si (f == NIHIL)
    {
        json_objectum_ponere(r, "fructus",
            json_nullum_creare(piscina));
        redde r;
    }
    {
        JsonValor* o = json_objectum_creare(piscina);

        _pone_b(o, "successus", f->successus, piscina);
        json_objectum_ponere(o, "causa",
            json_chorda_creare_literis(piscina,
                villa_exitus_nomen(f->causa.genus)));
        _pone_ch(o, "causa_textus", f->causa.causa, piscina);
        _pone_n(o, "codex", (s64)f->causa.codex, piscina);
        _pone_ch(o, "effusio", f->effusio, piscina);
        _pone_ch(o, "erratum", f->erratum, piscina);
        _pone_n(o, "mora_ms", (s64)f->mora_ms, piscina);
        _pone_b(o, "eventus_scriptus", f->eventus_scriptus,
            piscina);
        json_objectum_ponere(r, "fructus", o);
    }
    redde r;
}

/* fumus_modus {} -> {facies} - IS choreographiam fumi rogaverit */
interior JsonValor*
_fumus_modus (
    JsonValor* argumenta,
      Piscina* piscina,
       vacuum* datum,
       chorda* culpa)
{
    VillaStatus* v = (VillaStatus*)datum;
      JsonValor* r = json_objectum_creare(piscina);

    (vacuum)argumenta;
    (vacuum)culpa;
    _pone_b(r, "facies", v->fumus_facies, piscina);
    redde r;
}

/* fumus_perfectus {bene, nuntius?} - IS choreographiam finivit;
 * verdictum (etiam malum) statim refertur ne budgetum exspectemus */
interior JsonValor*
_fumus_perfectus (
    JsonValor* argumenta,
      Piscina* piscina,
       vacuum* datum,
       chorda* culpa)
{
    VillaStatus* v = (VillaStatus*)datum;
      JsonValor* r = json_objectum_creare(piscina);
      JsonValor* b = (argumenta != NIHIL)
          ? json_objectum_capere(argumenta, "bene") : NIHIL;
            chorda n = _arg_ch(argumenta, "nuntius");
    memoriae_index m;

    (vacuum)culpa;
    v->fumus_perfectus_est  = VERUM;
    v->fumus_bene           = (b32)(b != NIHIL && json_ad_boolean(b));
    m                       = (memoriae_index)n.mensura;
    si (m > (memoriae_index)(CCLVI - I))
    {
        m = (memoriae_index)(CCLVI - I);
    }
    si (m > ZEPHYRUM)
    {
        memcpy(v->fumus_nuntius, n.datum, m);
    }
    v->fumus_nuntius[m] = '\0';
    _pone_b(r, "acceptum", VERUM, piscina);
    redde r;
}

/* villa_abrumpere {clavis?} - actionem currentem desistere */
interior JsonValor*
_villa_abrumpere (
    JsonValor* argumenta,
      Piscina* piscina,
       vacuum* datum,
       chorda* culpa)
{
    VillaStatus* v       = (VillaStatus*)datum;
      JsonValor* r       = json_objectum_creare(piscina);
         chorda  clavis  = _arg_ch(argumenta, "clavis");

    (vacuum)culpa;
    si (clavis.mensura == ZEPHYRUM)
    {
        clavis = v->clavis;
    }
    villa_actionem_abrumpere(v->agens, clavis);
    _pone_b(r, "factum", VERUM, piscina);
    redde r;
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

s32
principale (
      integer   argc,
    character** argv)
{
    Piscina* piscina = piscina_generare_dynamicum("villa",
        16777216);
    Piscina* piscina_vocationis = piscina_generare_dynamicum(
        "villa_vocationes", 8388608);
             VillaStatus  villa;
                  chorda  causa;
                     b32  fumus = FALSUM;
                 integer  k;
    FenestraConfiguratio  figura_fenestrae;
      VitreaConfiguratio  figura_vitreae;
                Fenestra* fenestra;
                 Capsula* capsula;
                  Vitrea* vitrea;
            Internuntius* inx;

    si (piscina == NIHIL || piscina_vocationis == NIHIL)
    {
        redde I;
    }
    memset(&villa, 0, magnitudo(VillaStatus));
    cliens_tabularii_incipere(&villa.cliens,
        (i32)VILLA_PORTUS_ORDINARIUS);
    villa.cliens.praefixum  = "[villa]";
    villa.alias             = _ch("smaragda");
    villa.clavis            = _ch("primus");
    causa                   = _ch("");

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-portus") == ZEPHYRUM && k + I < argc)
        {
            villa.cliens.portus = (i32)atoi(argv[k + I]);
            k++;
        }
        alioquin si (   strcmp(argv[k], "-alias") == ZEPHYRUM
                     && k + I < argc)
        {
            villa.alias = _ch(argv[k + I]);
            k++;
        }
        alioquin si (strcmp(argv[k], "-fumus") == ZEPHYRUM)
        {
            fumus = VERUM;
        }
        alioquin si (strcmp(argv[k], "-fumus-facies") == ZEPHYRUM)
        {
            villa.fumus_facies = VERUM;
        }
    }

    /* configuratio: VILLA_SSH semel, directorium moderandi 0700 */
    si (!villa_configurationem_parare(&villa.configuratio, piscina,
        &causa))
    {
        imprimere("[villa] configuratio FRACTA: %.*s\n",
            (int)causa.mensura, (constans character*)causa.datum);
        redde I;
    }
    villa.agens = villa_agens_creare(&villa.configuratio,
        &villa.cliens, piscina);
    si (villa.agens == NIHIL)
    {
        imprimere("[villa] agens FRACTUS\n");
        redde I;
    }
    villa.unitates = xar_creare(piscina, (i32)magnitudo(chorda));
    {
        chorda* e = (chorda*)xar_addere(villa.unitates);

        si (e != NIHIL)
        {
            *e = _ch("nginx.service");
        }
    }

    si (fumus)
    {
        /* PORTA SINE OCULIS: catena tota sine fenestra. Semina
		 * omittuntur si daemon abest - fumus stratum agentis
		 * probat, non daemonem. */
        constans ProbatioServi* p;
                           i32  gressus = ZEPHYRUM;

        imprimere("[villa] fumus: ssh=%s\n",
            (villa.configuratio.via_ssh != NIHIL)
                ? villa.configuratio.via_ssh : "ssh");
        si (!villa_probationem_incipere(villa.agens, villa.clavis,
            villa.alias, villa.unitates, &causa))
        {
            imprimere("[villa] fumus FRACTUS (initium): %.*s\n",
                (int)causa.mensura,
                (constans character*)causa.datum);
            redde I;
        }
        /* TERMINUS VERUS EST MORA PROBATIONIS (processus eam
		 * custodit), non hic numerus: numerus gradum aeternum
		 * solum arcet. Prius II miliones erant, quod conexionem
		 * ssh FRIGIDAM (salutatio TCP + auctoritas + magister
		 * novus) non capiebat dum calidam capiebat - id est,
		 * terminus TEMPORIS unitate falsa expressus, qui cursum
		 * primum solum frangebat. */
        dum (   villa_probationes_currentes(villa.agens) > ZEPHYRUM
             && gressus < 200000000)
        {
            (vacuum)villa_agens_pulsare(villa.agens);
            gressus++;
        }
        p = villa_probatio_ultima(villa.agens, villa.clavis);
        si (p == NIHIL)
        {
            imprimere("[villa] fumus FRACTUS: nulla probatio\n");
            redde I;
        }
        imprimere("[villa] fumus: felix=%d causa=%s pulsus=%d\n",
            (int)p->felix, villa_exitus_nomen(p->causa.genus),
            (int)gressus);
        imprimere("[villa] fumus: unitates=%d servitia=%d situs=%d"
            " querelae='%.*s'\n",
            (int)((p->unitates != NIHIL)
                ? xar_numerus(p->unitates) : ZEPHYRUM),
            (int)((p->servitia != NIHIL)
                ? xar_numerus(p->servitia) : ZEPHYRUM),
            (int)((p->situs != NIHIL)
                ? xar_numerus(p->situs) : ZEPHYRUM),
            (int)p->querelae.mensura,
            (constans character*)p->querelae.datum);
        piscina_destruere(piscina_vocationis);
        piscina_destruere(piscina);
        redde p->felix ? ZEPHYRUM : I;
    }

    /* semina generum: idempotens, custodia collisionis intus */
    si (!villa_genera_seminare(villa.agens, &causa))
    {
        /* NON fatale: daemon abesse potest, aut clavis ab alia app
		 * occupata esse. Nominamus et pergimus - facies sine
		 * generibus adhuc probare potest. */
        imprimere("[villa] semina omissa: %.*s\n",
            (int)causa.mensura, (constans character*)causa.datum);
    }

    figura_fenestrae.titulus   = "villa";
    figura_fenestrae.x         = CC;
    figura_fenestrae.y         = CC;
    figura_fenestrae.latitudo  = 1000;
    figura_fenestrae.altitudo  = 900;
    figura_fenestrae.vexilla = FENESTRA_CLAUDIBILIS
        | FENESTRA_MUTABILIS | FENESTRA_CENTRATA;
    fenestra = fenestra_creare(piscina, &figura_fenestrae);
    si (fenestra == NIHIL)
    {
        imprimere("FRACTA: fenestra\n");
        redde I;
    }
    capsula = capsula_aperire(&capsula_villa, piscina);
    si (capsula == NIHIL)
    {
        imprimere("FRACTA: capsula\n");
        redde I;
    }
    figura_vitreae.origo = VITREA_ORIGO_CAPSULA;
    figura_vitreae.capsula = capsula;
    figura_vitreae.via_initialis = "index.html";
    figura_vitreae.url = NIHIL;
    figura_vitreae.inspectabilis = VERUM;
    vitrea = vitrea_creare(piscina, fenestra, &figura_vitreae);
    si (vitrea == NIHIL)
    {
        imprimere("FRACTA: vitrea\n");
        redde I;
    }
    inx = internuntius_creare(piscina, vitrea_missor, vitrea);
    si (inx == NIHIL)
    {
        imprimere("FRACTA: internuntius\n");
        redde I;
    }
    (vacuum)internuntius_praebere(inx, "villa_status",
        _villa_status, &villa);
    (vacuum)internuntius_praebere(inx, "villa_probatio",
        _villa_probatio, &villa);
    (vacuum)internuntius_praebere(inx, "villa_probare",
        _villa_probare, &villa);
    (vacuum)internuntius_praebere(inx, "villa_agere",
        _villa_agere, &villa);
    (vacuum)internuntius_praebere(inx, "villa_actio",
        _villa_actio, &villa);
    (vacuum)internuntius_praebere(inx, "villa_abrumpere",
        _villa_abrumpere, &villa);
    (vacuum)internuntius_praebere(inx, "fumus_modus",
        _fumus_modus, &villa);
    (vacuum)internuntius_praebere(inx, "fumus_perfectus",
        _fumus_perfectus, &villa);
    (vacuum)internuntius_praebere(inx, "transmittere",
        cliens_tabularii_transmittere, &villa.cliens);

    {
        Speculum* speculum = speculum_creare(piscina,
            &capsula_speculi_villa, inx, vitrea_aestimator, vitrea);
        i32 gressus_fumi = ZEPHYRUM;

        si (speculum == NIHIL)
        {
            imprimere("FRACTA: speculum\n");
            redde I;
        }
        /* probatio prima statim (non obstat): facies aliquid videt
		 * antequam JS horaria sua instituat. Horaria PERIODICA JS
		 * possidet quia indicem servorum possidet (V4b decisum -
		 * auto-probatio C-latus remota est). */
        (vacuum)villa_probationem_incipere(villa.agens, villa.clavis,
            villa.alias, villa.unitates, &causa);
        imprimere("[villa] fenestra aperta (alias %.*s, daemon"
            " portus %d)\n", (int)villa.alias.mensura,
            (constans character*)villa.alias.datum,
            (int)villa.cliens.portus);
        fflush(stdout);

        dum (!fenestra_debet_claudere(fenestra))
        {
                       Eventus eventus;
                        chorda nuntium;
            VitreaNuntiusGenus genus;
                PiscinaNotatio nota;

            fenestra_expectare_eventus(fenestra, VILLA_TICTUS_MS);
            dum (fenestra_obtinere_eventus(fenestra, &eventus))
            {
                (vacuum)speculum_tangere(speculum, &eventus);
            }
            nota = piscina_notare(piscina_vocationis);
            dum (vitrea_obtinere_nuntium(vitrea, &nuntium, &genus))
            {
                si (genus == VITREA_NUNTIUS_PONS)
                {
                    internuntius_tractare(inx, nuntium,
                        piscina_vocationis);
                }
                alioquin
                {
                    vitrea_recargare(vitrea);
                }
            }

            /* SLOT PROBATIONIS: gradus unus, numquam obstans -
			 * probationes ET actiones provehit. Photographiae in
			 * arenis agentis vivunt, NON in piscina_vocationis
			 * quae mox reficitur. */
            si (villa_agens_pulsare(villa.agens) > ZEPHYRUM)
            {
                villa.cycli++;
            }
            si (villa.fumus_facies)
            {
                si (villa.fumus_perfectus_est)
                {
                    imprimere(villa.fumus_bene
                        ? "FUMUS FACIEI PLENUS: %s\n"
                        : "FUMUS FACIEI FRACTUS: %s\n",
                        villa.fumus_nuntius);
                    fflush(stdout);
                    piscina_reficere(piscina_vocationis, nota);
                    frange;
                }
                gressus_fumi++;
                si (gressus_fumi > (i32)VILLA_FUMUS_GRESSUS_MAXIMI)
                {
                    imprimere("FUMUS FACIEI FRACTUS: tempus"
                        " excessum\n");
                    redde I;
                }
            }
            piscina_reficere(piscina_vocationis, nota);
        }
    }
    vitrea_destruere(vitrea);
    fenestra_destruere(fenestra);
    piscina_destruere(piscina_vocationis);
    piscina_destruere(piscina);
    si (villa.fumus_facies)
    {
        redde villa.fumus_bene ? ZEPHYRUM : I;
    }
    redde ZEPHYRUM;
}
