/* exemplar_administratio.c - tabula administrationis (vide
 * exemplar_administratio.h; canalis T4)
 *
 * Responsa JSON (mcp_respondere_json: structuredContent + textus).
 * Errores ut isError cum causa (nomen ignotum nominatur). Codex
 * invitationis solum in responso 'invitare' (P7); nihil in actis. */
#include "postulata_posix.h"
#include "exemplar_administratio.h"
#include "json.h"
#include "cives.h"
#include "invitationes.h"
#include "admissio.h"
#include "communio_servus.h"
#include "moneta.h"

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define SCOPUS_ADMINISTRATIONIS "administratio"
#define DIES_MS                 ((s64)XXIV * LX * LX * M)
#define DIES_ORDINARII          VII
#define DIES_MAXIMI             XXX

interior Exemplar*
_e (
    McpVocatio* v)
{
    redde (Exemplar*)mcp_vocatio_datum(v);
}

interior s64
_nunc_ms (vacuum)
{
    structura timeval t;

    gettimeofday(&t, NIHIL);
    redde (s64)t.tv_sec * M + (s64)(t.tv_usec / M);
}

interior vacuum
_vitium (
                McpVocatio* v,
        constans character* causa)
{
    mcp_respondere_vitium(v, chorda_ex_literis(causa,
        mcp_vocatio_piscina(v)));
}

interior constans character*
_status_titulus (
    CivisStatus s)
{
    commutatio (s)
    {
        casus CIVIS_ACTIVUS:   redde "activus";
        casus CIVIS_SUSPENSUS: redde "suspensus";
        casus CIVIS_DELETUS:   redde "deletus";
        ordinarius:            redde "ignotus";
    }
}

/* civis ex argumento 'nomen' (normatum, P11); aliter isError nominans */
interior b32
_civem_invenire (
    McpVocatio* v,
         Civis* x)
{
    Piscina* p = mcp_vocatio_piscina(v);
     chorda  appellatio;
  character  causa[CXXVIII];

    (vacuum)mcp_argumentum_chorda(v, "nomen", &appellatio);
    appellatio = cives_nomen_normare(appellatio, p);
    si (cives_per_nomen(ianua_cives(_e(v)->ianua), appellatio, x, p))
    {
        redde VERUM;
    }
    sprintf(causa, "civis ignotus: %.*s", (integer)(appellatio.mensura
        < LXIV
        ? appellatio.mensura : LXIV),
        (constans character*)appellatio.datum);
    _vitium(v, causa);
    redde FALSUM;
}

/* vivane? non revocata, non expirata, usus reliquus */
interior b32
_viva (
     constans Invitatio* x,
                    s64  nunc)
{
    redde    x->revocata_ms == ZEPHYRUM
          && x->expirat_ms > nunc
          && x->usus_numerus < x->usus_max;
}

/* auctor ostensus (P13): "" -> administratio, id -> nomen praesens */
interior chorda
_auctor (
     Exemplar* e,
       chorda  auctor,
      Piscina* p)
{
    Civis x;

    si (auctor.mensura == ZEPHYRUM)
    {
        redde chorda_ex_literis("administratio", p);
    }
    si (cives_per_id(ianua_cives(e->ianua), auctor, &x, p))
    {
        redde x.nomen_usoris;
    }
    redde auctor;
}

/* ---- instrumenta ---- */

interior vacuum
_status (
    McpVocatio* v)
{
      Exemplar* e      = _e(v);
       Piscina* p      = mcp_vocatio_piscina(v);
           Xar* cives  = cives_enumerare(ianua_cives(e->ianua), p);
           Xar* inv = invitationes_omnes_enumerare(ianua_invitationes(
               e->ianua), p);
     JsonValor* r;
     JsonValor* n;
           s64  numeri[III];
           s64  vivae  = ZEPHYRUM;
           s64  nunc   = _nunc_ms();
           i32  k;

    si (!cives || !inv)
    {
        _vitium(v, "volumen legi nequit");
        redde;
    }
    numeri[ZEPHYRUM] = numeri[I] = numeri[II] = ZEPHYRUM;
    per (k = ZEPHYRUM; k < xar_numerus(cives); k++)
    {
        CivisStatus s = ((Civis*)xar_obtinere(cives, k))->status;

        si ((i32)s <= (i32)CIVIS_DELETUS)
        {
            numeri[s]++;
        }
    }
    per (k = ZEPHYRUM; k < xar_numerus(inv); k++)
    {
        si (_viva((Invitatio*)xar_obtinere(inv, k), nunc))
        {
            vivae++;
        }
    }
    r = json_objectum_creare(p);
    n = json_objectum_creare(p);
    json_objectum_ponere(r, "salus", json_chorda_creare_literis(p,
        "ok"));
    json_objectum_ponere(r, "acta", json_integer_creare(p,
        (s64)volumen_summa_actorum(e->volumen)));
    json_objectum_ponere(n, "activi", json_integer_creare(p,
        numeri[CIVIS_ACTIVUS]));
    json_objectum_ponere(n, "suspensi", json_integer_creare(p,
        numeri[CIVIS_SUSPENSUS]));
    json_objectum_ponere(n, "deleti", json_integer_creare(p,
        numeri[CIVIS_DELETUS]));
    json_objectum_ponere(r, "cives", n);
    json_objectum_ponere(r, "invitationes_vivae", json_integer_creare(p,
        vivae));
    mcp_respondere_json(v, r);
}

interior vacuum
_invitare (
    McpVocatio* v)
{
          Exemplar* e    = _e(v);
           Piscina* p    = mcp_vocatio_piscina(v);
      Invitationes* inv  = ianua_invitationes(e->ianua);
            chorda  nota;
            chorda  codex;
            chorda  id;
         Invitatio  x;
               s64  dies = DIES_ORDINARII;
               b32  bene;
         JsonValor* r;
         character* nexus;

    (vacuum)mcp_argumentum_chorda(v, "nota", &nota);
    si (   mcp_argumentum_integer(v, "dies", &dies)
        && (dies < I || dies > DIES_MAXIMI))
    {
        _vitium(v, "dies extra I..XXX");
        redde;
    }
    si (!volumen_transactionem_incipere(e->volumen))
    {
        _vitium(v, "volumen occupatum");
        redde;
    }
    bene = invitationes_creare(inv, chorda_ex_literis("", p), nota,
        dies * DIES_MS, ZEPHYRUM, &codex, &id, p)
        && volumen_transactionem_committere(e->volumen);
    si (!bene)
    {
        (vacuum)volumen_transactionem_revolvere(e->volumen);
        _vitium(v, "invitatio creari nequit");
        redde;
    }
    memset(&x, ZEPHYRUM, magnitudo(x));
    (vacuum)invitationes_inspicere(inv, codex, &x, p);
    nexus = (character*)piscina_allocare(p,
        (memoriae_index)(EXEMPLAR_VIA_MAXIMA
        + LXIV));
    sprintf(nexus, "%s/inscribere?invitatio=%.*s", e->basis,
        (integer)codex.mensura, (constans character*)codex.datum);
    r = json_objectum_creare(p);
    json_objectum_ponere(r, "id", json_chorda_creare(p, id));
    json_objectum_ponere(r, "codex", json_chorda_creare(p, codex));
    json_objectum_ponere(r, "expirat_ms", json_integer_creare(p,
        x.expirat_ms));
    json_objectum_ponere(r, "nexus", json_chorda_creare_literis(p,
        nexus));
    mcp_respondere_json(v, r);
}

interior vacuum
_invitationes_enumerare (
    McpVocatio* v)
{
     Exemplar* e = _e(v);
      Piscina* p = mcp_vocatio_piscina(v);
          Xar* l = invitationes_omnes_enumerare(ianua_invitationes(
              e->ianua), p);
    JsonValor* r;
    JsonValor* t;
          s64  nunc = _nunc_ms();
          i32  k;

    si (!l)
    {
        _vitium(v, "volumen legi nequit");
        redde;
    }
    t = json_tabulatum_creare(p);
    per (k = ZEPHYRUM; k < xar_numerus(l); k++)
    {
        Invitatio* x = (Invitatio*)xar_obtinere(l, k);
        JsonValor* o = json_objectum_creare(p);

        json_objectum_ponere(o, "id", json_chorda_creare(p, x->id));
        json_objectum_ponere(o, "auctor", json_chorda_creare(p,
            _auctor(e, x->auctor, p)));
        json_objectum_ponere(o, "nota", json_chorda_creare(p, x->nota));
        json_objectum_ponere(o, "creata_ms", json_integer_creare(p,
            x->creata_ms));
        json_objectum_ponere(o, "expirat_ms", json_integer_creare(p,
            x->expirat_ms));
        json_objectum_ponere(o, "usus_max", json_integer_creare(p,
            (s64)x->usus_max));
        json_objectum_ponere(o, "usus_numerus", json_integer_creare(p,
            (s64)x->usus_numerus));
        json_objectum_ponere(o, "revocata", json_boolean_creare(p,
            x->revocata_ms != ZEPHYRUM));
        json_objectum_ponere(o, "viva", json_boolean_creare(p, _viva(x,
            nunc)));
        json_tabulatum_addere(t, o);
    }
    r = json_objectum_creare(p);
    json_objectum_ponere(r, "invitationes", t);
    mcp_respondere_json(v, r);
}

interior vacuum
_invitationem_revocare (
    McpVocatio* v)
{
      Exemplar* e = _e(v);
       Piscina* p = mcp_vocatio_piscina(v);
        chorda  id;
           b32  bene;
     JsonValor* r;

    (vacuum)mcp_argumentum_chorda(v, "id", &id);
    si (!volumen_transactionem_incipere(e->volumen))
    {
        _vitium(v, "volumen occupatum");
        redde;
    }
    bene = invitationes_revocare_quamlibet(ianua_invitationes(e->ianua),
        id) && volumen_transactionem_committere(e->volumen);
    si (!bene)
    {
        (vacuum)volumen_transactionem_revolvere(e->volumen);
        _vitium(v, "invitatio ignota aut iam revocata");
        redde;
    }
    r = json_objectum_creare(p);
    json_objectum_ponere(r, "id", json_chorda_creare(p, id));
    json_objectum_ponere(r, "revocata", json_boolean_creare(p, VERUM));
    mcp_respondere_json(v, r);
}

interior vacuum
_cives_enumerare (
    McpVocatio* v)
{
     Exemplar* e = _e(v);
      Piscina* p = mcp_vocatio_piscina(v);
          Xar* l = cives_enumerare(ianua_cives(e->ianua), p);
    JsonValor* r;
    JsonValor* t;
          i32  k;

    si (!l)
    {
        _vitium(v, "volumen legi nequit");
        redde;
    }
    t = json_tabulatum_creare(p);
    per (k = ZEPHYRUM; k < xar_numerus(l); k++)
    {
             Civis* x = (Civis*)xar_obtinere(l, k);
         JsonValor* o = json_objectum_creare(p);

        json_objectum_ponere(o, "id", json_chorda_creare(p, x->id));
        json_objectum_ponere(o, "nomen_usoris", json_chorda_creare(p,
            x->nomen_usoris));
        json_objectum_ponere(o, "titulus", json_chorda_creare(p,
            x->titulus));
        json_objectum_ponere(o, "status", json_chorda_creare_literis(p,
            _status_titulus(x->status)));
        json_objectum_ponere(o, "creatus_ms", json_integer_creare(p,
            x->creatus_ms));
        json_tabulatum_addere(t, o);
    }
    r = json_objectum_creare(p);
    json_objectum_ponere(r, "cives", t);
    mcp_respondere_json(v, r);
}

/* responsum commune mutationis statuum */
interior vacuum
_statum_reddere (
     McpVocatio* v,
          Civis* x,
    CivisStatus  status,
            b32  mutatum,
            s32  sessiones,
            s32  concessiones)
{
       Piscina* p = mcp_vocatio_piscina(v);
     JsonValor* r = json_objectum_creare(p);

    json_objectum_ponere(r, "nomen_usoris", json_chorda_creare(p,
        x->nomen_usoris));
    json_objectum_ponere(r, "status", json_chorda_creare_literis(p,
        _status_titulus(status)));
    json_objectum_ponere(r, "mutatum", json_boolean_creare(p, mutatum));
    json_objectum_ponere(r, "sessiones", json_integer_creare(p,
        (s64)sessiones));
    json_objectum_ponere(r, "concessiones", json_integer_creare(p,
        (s64)concessiones));
    mcp_respondere_json(v, r);
}

/* P10: status + sessiones + concessiones, transactione una */
interior vacuum
_civem_suspendere (
    McpVocatio* v)
{
     Exemplar* e = _e(v);
      Piscina* p = mcp_vocatio_piscina(v);
        Civis  x;
          Xar* concessiones;
          s32  sessiones = ZEPHYRUM;
          s32  revocatae = ZEPHYRUM;
          i32  k;
          b32  bene;

    si (!_civem_invenire(v, &x))
    {
        redde;
    }
    si (x.status == CIVIS_DELETUS)
    {
        _vitium(v, "civis deletus - status non mutatur");
        redde;
    }
    si (x.status == CIVIS_SUSPENSUS)
    {
        _statum_reddere(v, &x, CIVIS_SUSPENSUS, FALSUM, ZEPHYRUM,
            ZEPHYRUM);
        redde;
    }
    si (!volumen_transactionem_incipere(e->volumen))
    {
        _vitium(v, "volumen occupatum");
        redde;
    }
    bene = cives_statum_ponere(ianua_cives(e->ianua), x.id,
        CIVIS_SUSPENSUS);
    si (bene)
    {
        sessiones  = ianua_sessiones_civis_delere(e->ianua, x.id);
        bene       = sessiones >= ZEPHYRUM;
    }
    concessiones = bene ? admissio_concessiones(e->admissio, x.id, p)
        : NIHIL;
    bene = bene && concessiones;
    per (k = ZEPHYRUM; bene && k < xar_numerus(concessiones); k++)
    {
        bene = admissio_revocare(e->admissio, x.id,
            ((AdmissioConcessio*)xar_obtinere(concessiones, k))->id);
        revocatae++;
    }
    si (!bene || !volumen_transactionem_committere(e->volumen))
    {
        (vacuum)volumen_transactionem_revolvere(e->volumen);
        _vitium(v, "suspensio servari nequit - nihil mutatum");
        redde;
    }
    _statum_reddere(v, &x, CIVIS_SUSPENSUS, VERUM, sessiones,
        revocatae);
}

interior vacuum
_civem_restituere (
    McpVocatio* v)
{
     Exemplar* e = _e(v);
        Civis  x;
          b32  bene;

    si (!_civem_invenire(v, &x))
    {
        redde;
    }
    si (x.status == CIVIS_DELETUS)
    {
        _vitium(v, "civis deletus - status non mutatur");
        redde;
    }
    si (x.status == CIVIS_ACTIVUS)
    {
        _statum_reddere(v, &x, CIVIS_ACTIVUS, FALSUM, ZEPHYRUM,
            ZEPHYRUM);
        redde;
    }
    si (!volumen_transactionem_incipere(e->volumen))
    {
        _vitium(v, "volumen occupatum");
        redde;
    }
    bene = cives_statum_ponere(ianua_cives(e->ianua), x.id,
        CIVIS_ACTIVUS)
        && volumen_transactionem_committere(e->volumen);
    si (!bene)
    {
        (vacuum)volumen_transactionem_revolvere(e->volumen);
        _vitium(v, "restitutio servari nequit");
        redde;
    }
    _statum_reddere(v, &x, CIVIS_ACTIVUS, VERUM, ZEPHYRUM, ZEPHYRUM);
}

/* ---- communio (plan-2 T7): spatia et apparatus. Digestum tesserae
 * solum intrat (apparatus tesseram ipse cudit: communio tessera);
 * nec tessera nec digestum umquam redditur ---- */

interior vacuum
_communio_membrum_addere (
    McpVocatio* v)
{
     Exemplar* e = _e(v);
      Piscina* p = mcp_vocatio_piscina(v);
        Civis  x;
       chorda  spatium;
    JsonValor* r;

    si (!_civem_invenire(v, &x))
    {
        redde;
    }
    (vacuum)mcp_argumentum_chorda(v, "spatium", &spatium);
    si (!communio_membrum_addere(e->communio, spatium, x.id))
    {
        _vitium(v, "membrum addi nequit (persona non activa aut spatium"
            " vacuum)");
        redde;
    }
    r = json_objectum_creare(p);
    json_objectum_ponere(r, "spatium", json_chorda_creare(p, spatium));
    json_objectum_ponere(r, "civis", json_chorda_creare(p, x.id));
    json_objectum_ponere(r, "nomen", json_chorda_creare(p,
        x.nomen_usoris));
    mcp_respondere_json(v, r);
}

interior vacuum
_communio_apparatum_addere (
    McpVocatio* v)
{
     Exemplar* e = _e(v);
      Piscina* p = mcp_vocatio_piscina(v);
        Civis  x;
       chorda  spatium;
       chorda  titulus;
       chorda  digestum;
    character  id[MONETA_ULID_MENSURA];
    JsonValor* r;

    si (!_civem_invenire(v, &x))
    {
        redde;
    }
    (vacuum)mcp_argumentum_chorda(v, "spatium", &spatium);
    (vacuum)mcp_argumentum_chorda(v, "titulus", &titulus);
    (vacuum)mcp_argumentum_chorda(v, "digestum", &digestum);
    si (!communio_apparatum_addere(e->communio, x.id, spatium, titulus,
            digestum, id))
    {
        _vitium(v, "apparatus addi nequit (persona non activa, non"
            " membrum spatii, aut digestum malum vel iam usum)");
        redde;
    }
    r = json_objectum_creare(p);
    json_objectum_ponere(r, "id", json_chorda_creare_literis(p, id));
    json_objectum_ponere(r, "spatium", json_chorda_creare(p, spatium));
    json_objectum_ponere(r, "titulus", json_chorda_creare(p, titulus));
    mcp_respondere_json(v, r);
}

interior vacuum
_communio_apparatum_revocare (
    McpVocatio* v)
{
     Exemplar* e = _e(v);
      Piscina* p = mcp_vocatio_piscina(v);
       chorda  id;
    JsonValor* r;

    (vacuum)mcp_argumentum_chorda(v, "id", &id);
    si (!communio_apparatum_revocare(e->communio, id))
    {
        _vitium(v, "apparatus ignotus");
        redde;
    }
    r = json_objectum_creare(p);
    json_objectum_ponere(r, "id", json_chorda_creare(p, id));
    json_objectum_ponere(r, "revocatus", json_boolean_creare(p, VERUM));
    mcp_respondere_json(v, r);
}

interior vacuum
_communio_apparatus_enumerare (
    McpVocatio* v)
{
     Exemplar* e = _e(v);
      Piscina* p = mcp_vocatio_piscina(v);
          Xar* l;
    JsonValor* r;
    JsonValor* t;
          i32  i;

    l = communio_apparatus_enumerare(e->communio, p);
    si (!l)
    {
        _vitium(v, "registrum legi nequit");
        redde;
    }
    t = json_tabulatum_creare(p);
    per (i = ZEPHYRUM; i < xar_numerus(l); i++)
    {
        CommunioApparatus* a = (CommunioApparatus*)xar_obtinere(l, i);
                JsonValor* o = json_objectum_creare(p);
                    Civis  x;

        json_objectum_ponere(o, "id", json_chorda_creare(p, a->id));
        json_objectum_ponere(o, "civis", json_chorda_creare(p,
            a->civis));
        si (cives_per_id(ianua_cives(e->ianua), a->civis, &x, p))
        {
            json_objectum_ponere(o, "nomen", json_chorda_creare(p,
                x.nomen_usoris));
        }
        json_objectum_ponere(o, "spatium", json_chorda_creare(p,
            a->spatium));
        json_objectum_ponere(o, "titulus", json_chorda_creare(p,
            a->titulus));
        json_objectum_ponere(o, "replica", json_chorda_creare(p,
            a->replica));
        json_objectum_ponere(o, "revocatus", json_boolean_creare(p,
            a->revocatus));
        json_tabulatum_addere(t, o);
    }
    r = json_objectum_creare(p);
    json_objectum_ponere(r, "apparatus", t);
    mcp_respondere_json(v, r);
}

/* ---- tabula ---- */

hic_manens constans McpArgumentum ARG_INVITARE[] = {
    { "nota", "pro quo invitatio (videtur in enumeratione)", MCP_CHORDA,
        VERUM, NIHIL },
    { "dies", "vita in diebus, I..XXX (ordinarie VII)", MCP_INTEGER,
        FALSUM, NIHIL }
};

hic_manens constans McpArgumentum ARG_ID_INVITATIONIS[] = {
    { "id", "id invitationis (ex invitationes_enumerare)", MCP_CHORDA,
        VERUM, NIHIL }
};

hic_manens constans McpArgumentum ARG_NOMEN[] = {
    { "nomen", "nomen usoris (handle)", MCP_CHORDA, VERUM, NIHIL }
};

hic_manens constans McpArgumentum ARG_COMMUNIO_MEMBRUM[] = {
    { "spatium", "spatium (e.g. familia)", MCP_CHORDA, VERUM, NIHIL },
    { "nomen", "nomen usoris (handle)", MCP_CHORDA, VERUM, NIHIL }
};

hic_manens constans McpArgumentum ARG_COMMUNIO_APPARATUS[] = {
    { "nomen", "nomen usoris cuius apparatus est", MCP_CHORDA, VERUM,
        NIHIL },
    { "spatium", "spatium apparatus (membrum esse debet)", MCP_CHORDA,
        VERUM, NIHIL },
    { "titulus", "nomen apparatus (e.g. Macintosh)", MCP_CHORDA, VERUM,
        NIHIL },
    { "digestum", "SHA-256 hex tesserae (ex 'communio tessera'; numquam"
        " tessera ipsa)", MCP_CHORDA, VERUM, NIHIL }
};

hic_manens constans McpArgumentum ARG_ID_APPARATUS[] = {
    { "id", "id apparatus (ex communio_apparatus_enumerare)",
        MCP_CHORDA, VERUM, NIHIL }
};

hic_manens constans McpInstrumentum INSTRUMENTA_ADMINISTRATIONIS[] = {
    { "status", "Salus servi, numerus actorum, cives per statum,"
        " invitationes vivae.", NIHIL, ZEPHYRUM,
            SCOPUS_ADMINISTRATIONIS,
        VERUM, FALSUM, _status },
    { "invitare", "Invitationem novam creat; codicem et nexum SEMEL"
        " reddit.", ARG_INVITARE, II, SCOPUS_ADMINISTRATIONIS, FALSUM,
        FALSUM, _invitare },
    { "invitationes_enumerare", "Invitationes omnium auctorum,"
        " novissimas primas (sine codicibus).", NIHIL, ZEPHYRUM,
        SCOPUS_ADMINISTRATIONIS, VERUM, FALSUM,
            _invitationes_enumerare },
    { "invitationem_revocare", "Invitationem quamlibet revocat.",
        ARG_ID_INVITATIONIS, I, SCOPUS_ADMINISTRATIONIS, FALSUM, VERUM,
        _invitationem_revocare },
    { "cives_enumerare",
        "Cives omnes: nomen, titulus, status, creatio.",
        NIHIL, ZEPHYRUM, SCOPUS_ADMINISTRATIONIS, VERUM, FALSUM,
        _cives_enumerare },
    { "civem_suspendere", "Civem suspendit: sessiones et concessiones"
        " OAuth statim necantur.", ARG_NOMEN, I,
            SCOPUS_ADMINISTRATIONIS,
        FALSUM, VERUM, _civem_suspendere },
    { "civem_restituere", "Civem suspensum restituit (intrare et"
        " Claudium reconnectere debet).", ARG_NOMEN, I,
        SCOPUS_ADMINISTRATIONIS, FALSUM, FALSUM, _civem_restituere },
    { "communio_membrum_addere", "Civem in spatium communionis addit"
        " (iteratum idem).", ARG_COMMUNIO_MEMBRUM, II,
        SCOPUS_ADMINISTRATIONIS, FALSUM, FALSUM,
        _communio_membrum_addere },
    { "communio_apparatum_addere", "Apparatum per digestum tesserae"
        " inscribit; id reddit.", ARG_COMMUNIO_APPARATUS, IV,
        SCOPUS_ADMINISTRATIONIS, FALSUM, FALSUM,
        _communio_apparatum_addere },
    { "communio_apparatum_revocare", "Apparatum revocat: communicare"
        " desinit, acta eius manent.", ARG_ID_APPARATUS, I,
        SCOPUS_ADMINISTRATIONIS, FALSUM, VERUM,
        _communio_apparatum_revocare },
    { "communio_apparatus_enumerare",
        "Apparatus omnes: persona, spatium,"
        " titulus, replica, revocatus (sine digestis).", NIHIL,
            ZEPHYRUM,
        SCOPUS_ADMINISTRATIONIS, VERUM, FALSUM,
        _communio_apparatus_enumerare }
};

McpServus*
exemplar_administrationem_creare (
    Exemplar* e,
     Piscina* piscina)
{
    McpConfiguratio mc;

    si (!e || !piscina)
    {
        redde NIHIL;
    }
    memset(&mc, ZEPHYRUM, magnitudo(mc));
    mc.titulus  = "exemplar-administratio";
    mc.versio   = "1";
    mc.instructiones  =
        "Administratio exemplaris per canalem localem (uid servi): "
        "status, invitationes, cives (suspendere, restituere), "
        "communio (membra, apparatus).";
    mc.instrumenta  = INSTRUMENTA_ADMINISTRATIONIS;
    mc.numerus      = XI;
    mc.datum        = e;
    redde mcp_servus_creare(&mc, piscina);
}
