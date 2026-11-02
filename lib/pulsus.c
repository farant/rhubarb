/* pulsus.c - notificationes domus in servo communionis (vide
 * pulsus.h) */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "volumen.h"
#include "scrinium.h"
#include "cives.h"
#include "communio_servus.h"
#include "pulsus_forma.h"
#include "pulsus.h"
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

structura Pulsus {
           Piscina* piscina;
           Volumen* volumen;
          Scrinium* scrinium;
    CommunioServus* servus;
             Cives* cives;
               s64 (*nunc_ms)(vacuum);
     PulsusMittere  mittere;
            vacuum* sui;
           Piscina* tempus;     /* vacatur post quamque vocationem */
};

/* clavis gregis (chordae in tempore) */
nomen structura {
    chorda civis;
    chorda spatium;
    chorda clavis;
} Clavis;

/* inscriptio destinatarii */
nomen structura {
    chorda apparatus;
    chorda tessera;
    chorda ambitus;
} Destinatarius;

hic_manens constans character* constans SCHEMA[] = {
    "CREATE TABLE IF NOT EXISTS pulsus_inscriptiones"
    " (apparatus TEXT NOT NULL, thema TEXT NOT NULL,"
    " tessera TEXT NOT NULL, ambitus TEXT NOT NULL,"
    " creatus_ms INTEGER NOT NULL,"
    " usus_ultimus_ms INTEGER NOT NULL DEFAULT 0,"
    " PRIMARY KEY (apparatus, thema))",
    "CREATE TABLE IF NOT EXISTS pulsus_pendentes"
    " (civis TEXT NOT NULL, spatium TEXT NOT NULL,"
    " clavis TEXT NOT NULL, thema TEXT NOT NULL, res TEXT NOT NULL,"
    " omissa INTEGER NOT NULL DEFAULT 0, unus TEXT NOT NULL,"
    " plures TEXT NOT NULL, primus_ms INTEGER NOT NULL,"
    " ultimus_ms INTEGER NOT NULL, mora_ms INTEGER NOT NULL,"
    " PRIMARY KEY (civis, spatium, clavis))",
    NIHIL
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior s64
_nunc (
    Pulsus* p)
{
    structura timeval tv;

    si (p->nunc_ms)
    {
        redde p->nunc_ms();
    }
    si (gettimeofday(&tv, NIHIL) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s64)tv.tv_sec * (s64)M + (s64)(tv.tv_usec / M);
}

/* "earth.golden.pictor" -> "pictor" */
interior chorda
_titulus_thematis (
    chorda thema)
{
    i32 i;

    per (i = thema.mensura; i > ZEPHYRUM; i--)
    {
        si (thema.datum[i - I] == '.')
        {
            redde chorda_sectio(thema, i, thema.mensura);
        }
    }
    redde thema;
}

/* VIII signa prima (nuntii) */
interior chorda
_breviter (
    chorda tessera)
{
    redde tessera.mensura > VIII ? chorda_sectio(tessera, ZEPHYRUM,
        VIII)
                                 : tessera;
}

/* res JSON -> Xar de chorda (in tempore); vacua si mala */
interior Xar*
_res_legere (
     chorda  json,
    Piscina* piscina)
{
    JsonResultus  r;
             Xar* x;
             i32  i;
       JsonValor* v;

    x = xar_creare(piscina, magnitudo(chorda));
    si (!x)
    {
        redde NIHIL;
    }
    r = json_legere(json, piscina);
    per (i = ZEPHYRUM; r.successus && r.radix
        && json_est_tabulatum(r.radix)
        && i < json_tabulatum_numerus(r.radix); i++)
    {
        v = json_tabulatum_obtinere(r.radix, i);
        si (v && json_est_chorda(v))
        {
            *(chorda*)xar_addere(x) = json_ad_chorda(v);
        }
    }
    redde x;
}

interior chorda
_res_scribere (
    constans Xar* res,
         Piscina* piscina)
{
    JsonValor* t;
          i32  i;

    t = json_tabulatum_creare(piscina);
    per (i = ZEPHYRUM; t && i < xar_numerus(res); i++)
    {
        json_tabulatum_addere(t, json_chorda_creare(piscina,
            *(chorda*)xar_obtinere(res, i)));
    }
    redde json_scribere(t, piscina);
}

interior b32
_continet (
    constans Xar* res,
          chorda  id)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(res); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(res, i), id))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* apparatus per id (non revocatus); FALSUM si ignotus aut revocatus */
interior b32
_apparatum_quaerere (
               Pulsus* p,
               chorda  id,
    CommunioApparatus* exitus)
{
    Xar* l;
    i32  i;

    l = communio_apparatus_enumerare(p->servus, p->tempus);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        CommunioApparatus* a = (CommunioApparatus*)xar_obtinere(l, i);

        si (chorda_aequalis(a->id, id))
        {
            *exitus = *a;
            redde !a->revocatus;
        }
    }
    redde FALSUM;
}

interior b32
_persona_activa (
     Pulsus* p,
     chorda  civis)
{
    Civis c;

    redde cives_per_id(p->cives, civis, &c, p->tempus)
        && c.status == CIVIS_ACTIVUS;
}

interior vacuum
_inscriptiones_delere (
     Pulsus* p,
     chorda  apparatus)
{
    ScriniumEnuntiatum* e;

    e = scrinium_praeparare(p->scrinium,
        "DELETE FROM pulsus_inscriptiones WHERE apparatus = ?");
    si (e)
    {
        scrinium_ligare_textum(e, I, apparatus);
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
    }
}

/* inscriptiones (apparatus, thema) -> Xar de Destinatarius additae */
interior vacuum
_destinatarios_addere (
     Pulsus* p,
     chorda  apparatus,
     chorda  thema,
        Xar* exitus)
{
    ScriniumEnuntiatum* e;
         Destinatarius* d;

    e = scrinium_praeparare(p->scrinium,
        "SELECT tessera, ambitus FROM pulsus_inscriptiones"
        " WHERE apparatus = ? AND thema = ?");
    si (!e)
    {
        redde;
    }
    scrinium_ligare_textum(e, I, apparatus);
    scrinium_ligare_textum(e, II, thema);
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        d = (Destinatarius*)xar_addere(exitus);
        si (!d)
        {
            frange;
        }
        d->apparatus  = chorda_transcribere(apparatus, p->tempus);
        d->tessera    = scrinium_columna_textus(e, ZEPHYRUM, p->tempus);
        d->ambitus    = scrinium_columna_textus(e, I, p->tempus);
    }
    scrinium_finire(e);
}


/* ==================================================
 * Missio
 * ================================================== */

interior vacuum
_finis (
                   vacuum* ctx,
    constans PulsusMissio* m,
             PulsusExitus  exitus,
                   chorda  ratio)
{
                Pulsus* p;
    ScriniumEnuntiatum* e;

    p = (Pulsus*)ctx;
    si (!p || !m)
    {
        redde;
    }
    si (exitus == PULSUS_TRADITUM)
    {
        e = scrinium_praeparare(p->scrinium,
            "UPDATE pulsus_inscriptiones SET usus_ultimus_ms = ?"
            " WHERE apparatus = ? AND thema = ?");
        si (e)
        {
            scrinium_ligare_numerum(e, I, _nunc(p));
            scrinium_ligare_textum(e, II, m->apparatus);
            scrinium_ligare_textum(e, III, m->thema);
            (vacuum)scrinium_gradi(e);
            scrinium_finire(e);
        }
    }
    alioquin si (exitus == PULSUS_SIGNUM_MORTUUM)
    {
        e = scrinium_praeparare(p->scrinium,
            "DELETE FROM pulsus_inscriptiones"
            " WHERE apparatus = ? AND thema = ? AND tessera = ?");
        si (e)
        {
            scrinium_ligare_textum(e, I, m->apparatus);
            scrinium_ligare_textum(e, II, m->thema);
            scrinium_ligare_textum(e, III, m->signum);
            (vacuum)scrinium_gradi(e);
            scrinium_finire(e);
        }
        fprintf(stderr,
            "pulsus: signum %.*s mortuum (%.*s) - deletum\n",
            (int)_breviter(m->signum).mensura,
            (constans character*)_breviter(m->signum).datum,
            (int)ratio.mensura, (constans character*)ratio.datum);
    }
    alioquin
    {
        fprintf(stderr, "pulsus: missio ad %.*s defecit (%.*s)\n",
            (int)_breviter(m->signum).mensura,
            (constans character*)_breviter(m->signum).datum,
            (int)ratio.mensura, (constans character*)ratio.datum);
    }
}

/* corpus ad omnes destinatarios: apparatus spatii (non revocati,
 * persona activa, non mittens si mittens non vacuus) - aut, si
 * solus_civis non vacuus, apparatus eius personae soli. Revocatorum
 * inscriptiones purgantur. */
interior vacuum
_corpus_mittere (
     Pulsus* p,
     chorda  spatium,
     chorda  mittens,
     chorda  solus_civis,
     chorda  thema,
     chorda  corpus)
{
               Xar* l;
               Xar* dest;
               i32  i;
      PulsusMissio  m;

    l     = communio_apparatus_enumerare(p->servus, p->tempus);
    dest  = xar_creare(p->tempus, magnitudo(Destinatarius));
    per (i = ZEPHYRUM; l && dest && i < xar_numerus(l); i++)
    {
        CommunioApparatus* a = (CommunioApparatus*)xar_obtinere(l, i);

        si (a->revocatus)
        {
            _inscriptiones_delere(p, a->id);
            perge;
        }
        si (solus_civis.mensura > ZEPHYRUM)
        {
            si (!chorda_aequalis(a->civis, solus_civis))
            {
                perge;
            }
        }
        alioquin si (   !chorda_aequalis(a->spatium, spatium)
                     || chorda_aequalis(a->civis, mittens))
        {
            perge;
        }
        si (!_persona_activa(p, a->civis))
        {
            perge;
        }
        _destinatarios_addere(p, a->id, thema, dest);
    }
    per (i = ZEPHYRUM; dest && i < xar_numerus(dest); i++)
    {
        Destinatarius* d = (Destinatarius*)xar_obtinere(dest, i);

        si (!p->mittere)
        {
            fprintf(stderr,
                "pulsus: apns absens - non mittitur (%.*s)\n",
                (int)_breviter(d->tessera).mensura,
                (constans character*)_breviter(d->tessera).datum);
            perge;
        }
        m.signum     = d->tessera;
        m.ambitus    = d->ambitus;
        m.thema      = thema;
        m.corpus     = corpus;
        m.apparatus  = d->apparatus;
        si (!p->mittere(p->sui, &m, _finis, p))
        {
            fprintf(stderr,
                "pulsus: mittens missionem recusavit (%.*s)\n",
                (int)_breviter(d->tessera).mensura,
                (constans character*)_breviter(d->tessera).datum);
        }
    }
}

/* grex per clavem mittitur et deletur; FALSUM si nullus */
interior b32
_gregem_mittere (
             Pulsus* p,
    constans Clavis* k)
{
    ScriniumEnuntiatum* e;
                chorda  thema;
                chorda  res_json;
                chorda  unus;
                chorda  plures;
                   s64  omissa;
                   Xar* res;
                   i32  numerus;
                 Civis  c;
                chorda  persona;
                chorda  textus;
                chorda  corpus;

    e = scrinium_praeparare(p->scrinium,
        "SELECT thema, res, omissa, unus, plures FROM pulsus_pendentes"
        " WHERE civis = ? AND spatium = ? AND clavis = ?");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, k->civis);
    scrinium_ligare_textum(e, II, k->spatium);
    scrinium_ligare_textum(e, III, k->clavis);
    si (scrinium_gradi(e) != SCRINIUM_ORDO)
    {
        scrinium_finire(e);
        redde FALSUM;
    }
    thema     = scrinium_columna_textus(e, ZEPHYRUM, p->tempus);
    res_json  = scrinium_columna_textus(e, I, p->tempus);
    omissa    = scrinium_columna_numerus(e, II);
    unus      = scrinium_columna_textus(e, III, p->tempus);
    plures    = scrinium_columna_textus(e, IV, p->tempus);
    scrinium_finire(e);

    /* deletur ANTE missionem: finis synchronus scripturas suas facit,
     * grex numquam bis mittitur */
    e = scrinium_praeparare(p->scrinium,
        "DELETE FROM pulsus_pendentes"
        " WHERE civis = ? AND spatium = ? AND clavis = ?");
    si (e)
    {
        scrinium_ligare_textum(e, I, k->civis);
        scrinium_ligare_textum(e, II, k->spatium);
        scrinium_ligare_textum(e, III, k->clavis);
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
    }

    res      = _res_legere(res_json, p->tempus);
    numerus  = (res ? xar_numerus(res) : ZEPHYRUM) + (i32)omissa;
    persona  = k->civis;
    si (cives_per_id(p->cives, k->civis, &c, p->tempus))
    {
        persona = c.titulus.mensura > ZEPHYRUM ? c.titulus
                                               : c.nomen_usoris;
    }
    textus = pulsus_textum_facere(numerus == I ? unus : plures, persona,
        numerus, p->tempus);
    corpus = pulsus_corpus_facere(_titulus_thematis(thema), textus,
        k->clavis, k->civis, res, p->tempus);
    _corpus_mittere(p, k->spatium, k->civis, chorda_ex_literis("",
        p->tempus), thema, corpus);
    redde VERUM;
}


/* ==================================================
 * Vita
 * ================================================== */

Pulsus*
pulsus_creare (
           Piscina* piscina,
           Volumen* volumen,
    CommunioServus* servus,
             Cives* cives,
               s64  (*nunc_ms)(vacuum))
{
    Pulsus* p;
       i32  k;

    si (!piscina || !volumen || !servus || !cives)
    {
        redde NIHIL;
    }
    p = (Pulsus*)piscina_allocare(piscina, magnitudo(Pulsus));
    si (!p)
    {
        redde NIHIL;
    }
    memset(p, ZEPHYRUM, magnitudo(*p));
    p->piscina   = piscina;
    p->volumen   = volumen;
    p->scrinium  = volumen_scrinium(volumen);
    p->servus    = servus;
    p->cives     = cives;
    p->nunc_ms   = nunc_ms;
    p->tempus   = piscina_generare_dynamicum("pulsus",
        (memoriae_index)M * IV);
    si (!p->scrinium || !p->tempus)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; SCHEMA[k]; k++)
    {
        si (!scrinium_exsequi(p->scrinium, SCHEMA[k]))
        {
            piscina_destruere(p->tempus);
            p->tempus = NIHIL;
            redde NIHIL;
        }
    }
    redde p;
}

vacuum
pulsus_destruere (
    Pulsus* pulsus)
{
    si (pulsus && pulsus->tempus)
    {
        piscina_destruere(pulsus->tempus);
        pulsus->tempus = NIHIL;
    }
}

vacuum
pulsus_mittentem_ponere (
           Pulsus* pulsus,
    PulsusMittere  mittere,
           vacuum* sui)
{
    si (pulsus)
    {
        pulsus->mittere  = mittere;
        pulsus->sui      = sui;
    }
}


/* ==================================================
 * Inscriptiones
 * ================================================== */

b32
pulsus_inscribere (
     Pulsus* pulsus,
     chorda  apparatus,
     chorda  tessera,
     chorda  ambitus,
     chorda  thema)
{
    ScriniumEnuntiatum* e;
                   b32  bene;

    si (   !pulsus || apparatus.mensura == ZEPHYRUM
        || tessera.mensura == ZEPHYRUM || ambitus.mensura == ZEPHYRUM
        || thema.mensura   == ZEPHYRUM)
    {
        redde FALSUM;
    }
    e = scrinium_praeparare(pulsus->scrinium,
        "INSERT OR REPLACE INTO pulsus_inscriptiones"
        " (apparatus, thema, tessera, ambitus, creatus_ms,"
        " usus_ultimus_ms) VALUES (?, ?, ?, ?, ?, 0)");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, apparatus);
    scrinium_ligare_textum(e, II, thema);
    scrinium_ligare_textum(e, III, tessera);
    scrinium_ligare_textum(e, IV, ambitus);
    scrinium_ligare_numerum(e, V, _nunc(pulsus));
    bene = scrinium_gradi(e) != SCRINIUM_ERROR;
    scrinium_finire(e);
    redde bene;
}

Xar*
pulsus_inscriptiones_enumerare (
     Pulsus* pulsus,
    Piscina* piscina)
{
    ScriniumEnuntiatum* e;
                   Xar* l;
      PulsusInscriptio* r;

    si (!pulsus || !piscina)
    {
        redde NIHIL;
    }
    l = xar_creare(piscina, magnitudo(PulsusInscriptio));
    e = scrinium_praeparare(pulsus->scrinium,
        "SELECT apparatus, thema, tessera, ambitus, creatus_ms,"
        " usus_ultimus_ms FROM pulsus_inscriptiones"
        " ORDER BY creatus_ms, rowid");
    si (!l || !e)
    {
        si (e)
        {
            scrinium_finire(e);
        }
        redde NIHIL;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        r = (PulsusInscriptio*)xar_addere(l);
        si (!r)
        {
            frange;
        }
        r->apparatus  = scrinium_columna_textus(e, ZEPHYRUM,
            piscina);
        r->thema      = scrinium_columna_textus(e, I, piscina);
        r->tessera         = _breviter(scrinium_columna_textus(e, II,
            piscina));
        r->ambitus          = scrinium_columna_textus(e, III, piscina);
        r->creatus_ms       = scrinium_columna_numerus(e, IV);
        r->usus_ultimus_ms  = scrinium_columna_numerus(e, V);
    }
    scrinium_finire(e);
    redde l;
}


/* ==================================================
 * Eventus et greges
 * ================================================== */

b32
pulsus_eventum_addere (
                    Pulsus* pulsus,
                    chorda  apparatus,
    constans PulsusEventus* eventus)
{
     CommunioApparatus  a;
    ScriniumEnuntiatum* e;
                Clavis  k;
                   Xar* res;
                   s64  omissa;
                   s64  primus;
                   s64  nunc;
                   b32  erat;
                   i32  i;
                   i32  numerus_novorum;

    si (!pulsus || !eventus || !eventus->res)
    {
        redde FALSUM;
    }
    si (!_apparatum_quaerere(pulsus, apparatus, &a))
    {
        piscina_vacare(pulsus->tempus);
        redde FALSUM;
    }
    k.civis          = a.civis;
    k.spatium        = a.spatium;
    k.clavis         = eventus->clavis;
    nunc             = _nunc(pulsus);
    numerus_novorum  = xar_numerus(eventus->res);

    /* grex exstans */
    res     = NIHIL;
    omissa  = ZEPHYRUM;
    primus  = nunc;
    erat    = FALSUM;
    e = scrinium_praeparare(pulsus->scrinium,
        "SELECT res, omissa, primus_ms FROM pulsus_pendentes"
        " WHERE civis = ? AND spatium = ? AND clavis = ?");
    si (e)
    {
        scrinium_ligare_textum(e, I, k.civis);
        scrinium_ligare_textum(e, II, k.spatium);
        scrinium_ligare_textum(e, III, k.clavis);
        si (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            erat   = VERUM;
            res    = _res_legere(scrinium_columna_textus(e, ZEPHYRUM,
                pulsus->tempus), pulsus->tempus);
            omissa = scrinium_columna_numerus(e, I);
            primus = scrinium_columna_numerus(e, II);
        }
        scrinium_finire(e);
    }

    si (numerus_novorum > ZEPHYRUM)
    {
        si (!res)
        {
            res = xar_creare(pulsus->tempus, magnitudo(chorda));
        }
        per (i = ZEPHYRUM; res && i < numerus_novorum; i++)
        {
            chorda id = *(chorda*)xar_obtinere(eventus->res, i);

            si (_continet(res, id))
            {
                perge;
            }
            si (xar_numerus(res) >= PULSUS_RES_PER_GREGEM)
            {
                omissa++;
                perge;
            }
            *(chorda*)xar_addere(res) = id;
        }
        e = scrinium_praeparare(pulsus->scrinium,
            "INSERT OR REPLACE INTO pulsus_pendentes"
            " (civis, spatium, clavis, thema, res, omissa, unus, plures,"
            " primus_ms, ultimus_ms, mora_ms)"
            " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        si (!e || !res)
        {
            si (e)
            {
                scrinium_finire(e);
            }
            piscina_vacare(pulsus->tempus);
            redde FALSUM;
        }
        scrinium_ligare_textum(e, I, k.civis);
        scrinium_ligare_textum(e, II, k.spatium);
        scrinium_ligare_textum(e, III, k.clavis);
        scrinium_ligare_textum(e, IV, eventus->thema);
        scrinium_ligare_textum(e, V, _res_scribere(res,
            pulsus->tempus));
        scrinium_ligare_numerum(e, VI, omissa);
        scrinium_ligare_textum(e, VII, eventus->unus);
        scrinium_ligare_textum(e, VIII, eventus->plures);
        scrinium_ligare_numerum(e, IX, primus);
        scrinium_ligare_numerum(e, X, nunc);
        scrinium_ligare_numerum(e, XI, (s64)eventus->mora_s * (s64)M);
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
        erat = VERUM;
    }

    si (eventus->statim && erat)
    {
        (vacuum)_gregem_mittere(pulsus, &k);
    }
    piscina_vacare(pulsus->tempus);
    redde VERUM;
}

i32
pulsus_maturos_mittere (
    Pulsus* pulsus)
{
    ScriniumEnuntiatum* e;
                   Xar* claves;
                Clavis* k;
                   i32  i;
                   i32  missi;

    si (!pulsus)
    {
        redde ZEPHYRUM;
    }
    claves = xar_creare(pulsus->tempus, magnitudo(Clavis));
    e = scrinium_praeparare(pulsus->scrinium,
        "SELECT civis, spatium, clavis FROM pulsus_pendentes"
        " WHERE ultimus_ms + mora_ms <= ? ORDER BY ultimus_ms, rowid");
    si (!e || !claves)
    {
        si (e)
        {
            scrinium_finire(e);
        }
        piscina_vacare(pulsus->tempus);
        redde ZEPHYRUM;
    }
    scrinium_ligare_numerum(e, I, _nunc(pulsus));
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        k = (Clavis*)xar_addere(claves);
        si (!k)
        {
            frange;
        }
        k->civis = scrinium_columna_textus(e, ZEPHYRUM,
            pulsus->tempus);
        k->spatium  = scrinium_columna_textus(e, I, pulsus->tempus);
        k->clavis   = scrinium_columna_textus(e, II, pulsus->tempus);
    }
    scrinium_finire(e);
    missi = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(claves); i++)
    {
        si (_gregem_mittere(pulsus, (Clavis*)xar_obtinere(claves, i)))
        {
            missi++;
        }
    }
    piscina_vacare(pulsus->tempus);
    redde missi;
}

s64
pulsus_proximum_ms (
    Pulsus* pulsus)
{
    ScriniumEnuntiatum* e;
                   s64  proximum;

    si (!pulsus)
    {
        redde (s64)-I;
    }
    proximum = (s64)-I;
    e = scrinium_praeparare(pulsus->scrinium,
        "SELECT MIN(ultimus_ms + mora_ms) FROM pulsus_pendentes");
    si (e)
    {
        si (   scrinium_gradi(e) == SCRINIUM_ORDO
            && !scrinium_columna_nihil_est(e, ZEPHYRUM))
        {
            proximum = scrinium_columna_numerus(e, ZEPHYRUM);
        }
        scrinium_finire(e);
    }
    redde proximum;
}

b32
pulsus_probare (
     Pulsus* pulsus,
     chorda  civis)
{
                 Civis  c;
                   Xar* l;
                   Xar* themata;
                   i32  i;
    ScriniumEnuntiatum* e;
                chorda  corpus;

    si (   !pulsus
        || !cives_per_id(pulsus->cives, civis, &c, pulsus->tempus))
    {
        si (pulsus)
        {
            piscina_vacare(pulsus->tempus);
        }
        redde FALSUM;
    }
    /* themata omnia inscripta */
    themata = xar_creare(pulsus->tempus, magnitudo(chorda));
    e = scrinium_praeparare(pulsus->scrinium,
        "SELECT DISTINCT thema FROM pulsus_inscriptiones ORDER BY thema");
    si (e && themata)
    {
        dum (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            *(chorda*)xar_addere(themata) = scrinium_columna_textus(e,
                ZEPHYRUM, pulsus->tempus);
        }
    }
    si (e)
    {
        scrinium_finire(e);
    }
    l = xar_creare(pulsus->tempus, magnitudo(chorda));
    per (i = ZEPHYRUM; themata && l && i < xar_numerus(themata); i++)
    {
        chorda thema = *(chorda*)xar_obtinere(themata, i);

        corpus = pulsus_corpus_facere(_titulus_thematis(thema),
            chorda_ex_literis("pulsus: probatio", pulsus->tempus),
            chorda_ex_literis("pulsus.probatio", pulsus->tempus), civis,
            l, pulsus->tempus);
        _corpus_mittere(pulsus, chorda_ex_literis("", pulsus->tempus),
            chorda_ex_literis("", pulsus->tempus), civis, thema,
            corpus);
    }
    piscina_vacare(pulsus->tempus);
    redde VERUM;
}
