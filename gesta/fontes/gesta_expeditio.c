/* gesta_expeditio.c - expeditiones: plicatura et validatio (PURAE)
 *
 * Vide gesta_expeditio.h et gesta_expeditio.worklog.md.
 */
#include "gesta_expeditio.h"
#include "chorda_aedificator.h"

interior b32
_est (
                 chorda  c,
     constans character* litterae)
{
    redde chorda_aequalis_literis(c, litterae);
}

/* chorda campi obiecti[clavis]; vacua si abest aut non chorda */
interior chorda
_campus (
             JsonValor* obiectum,
    constans character* clavis)
{
    JsonValor* v = obiectum != NIHIL && json_est_objectum(obiectum)
        ? json_objectum_capere(obiectum, clavis) : NIHIL;
    chorda vacua;

    si (v != NIHIL && json_est_chorda(v))
    {
        redde json_ad_chorda(v);
    }
    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;
    redde vacua;
}

b32
gesta_expeditio_eventus_est (
    chorda genus_eventus)
{
    redde _est(genus_eventus, "expeditio-photographia")
        || _est(genus_eventus, "rubrica-mutata")
        || _est(genus_eventus, "gradus-positus")
        || _est(genus_eventus, "gradus-promotus")
        || _est(genus_eventus, "ordines-additi");
}

/* index ordinis in photographia per clavem; -I si abest */
interior s32
_ordo_index (
     JsonValor* ordines,
        chorda  clavis)
{
    i32 k;

    si (ordines == NIHIL || !json_est_tabulatum(ordines))
    {
        redde -I;
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(ordines); k++)
    {
        si (chorda_aequalis(_campus(json_tabulatum_obtinere(ordines, k),
                "clavis"), clavis))
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* photographia iam facta? (acies 'ordines' in statu) */
interior b32
_photographata (
    JsonValor* status)
{
    JsonValor* o = json_objectum_capere(status, "ordines");

    redde o != NIHIL && json_est_tabulatum(o);
}

/* versio rubricae currens (0 ante photographiam) */
interior s64
_versio (
    JsonValor* status)
{
    JsonValor* r = json_objectum_capere(status, "rubrica");
    JsonValor* v = r != NIHIL && json_est_objectum(r)
        ? json_objectum_capere(r, "versio") : NIHIL;

    redde v != NIHIL && json_est_integer(v) ? json_ad_integer(v)
        : (s64)ZEPHYRUM;
}

/* obiectum status['gradus'], creatum si deest */
interior JsonValor*
_gradus (
    JsonValor* status,
      Piscina* pn)
{
    JsonValor* g = json_objectum_capere(status, "gradus");

    si (g == NIHIL || !json_est_objectum(g))
    {
        g = json_objectum_creare(pn);
        json_objectum_ponere(status, "gradus", g);
    }
    redde g;
}

/* obiectum copiatum (campi chordae soli) - filtrum/implenda */
interior JsonValor*
_copia_chordarum (
    JsonValor* fons,
      Piscina* pn)
{
               JsonValor* c = json_objectum_creare(pn);
    JsonObjectumIterator  iter;
                  chorda  clavis;
               JsonValor* v;

    iter = json_objectum_iterator(fons);
    dum (json_objectum_iterator_proxima(&iter, &clavis, &v))
    {
        si (v != NIHIL && json_est_chorda(v))
        {
            json_objectum_ponere_chorda(c, clavis,
                json_chorda_creare(pn, json_ad_chorda(v)));
        }
    }
    redde c;
}

/* ordinem in aciem ordinum addere {clavis, additus} */
interior vacuum
_ordinem_addere (
    JsonValor* ordines,
       chorda  clavis,
       chorda  creatum,
      Piscina* pn)
{
    JsonValor* o = json_objectum_creare(pn);

    json_objectum_ponere(o, "clavis", json_chorda_creare(pn, clavis));
    json_objectum_ponere(o, "additus", json_chorda_creare(pn, creatum));
    json_tabulatum_addere(ordines, o);
}


/* ==================================================
 * PLICATURA (defensiva - replay numquam fallit)
 * ================================================== */

interior b32
_photographiam_plicare (
    JsonValor* status,
    JsonValor* datum,
       chorda  creatum,
      Piscina* pn)
{
    JsonValor* acies     = json_objectum_capere(datum, "ordines");
    JsonValor* filtrum   = json_objectum_capere(datum, "filtrum");
    JsonValor* implenda  = json_objectum_capere(datum, "implenda");
    JsonValor* ordines;
    JsonValor* rubrica;
          i32  k;

    si (   _photographata(status) || acies == NIHIL
        || !json_est_tabulatum(acies))
    {
        redde FALSUM;
    }
    ordines = json_tabulatum_creare(pn);
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(acies); k++)
    {
        JsonValor* v = json_tabulatum_obtinere(acies, k);

        si (   v != NIHIL && json_est_chorda(v)
            && json_ad_chorda(v).mensura > ZEPHYRUM
            && _ordo_index(ordines, json_ad_chorda(v)) < ZEPHYRUM)
        {
            _ordinem_addere(ordines, json_ad_chorda(v), creatum, pn);
        }
    }
    json_objectum_ponere(status, "ordines", ordines);
    json_objectum_ponere(status, "inventarium", json_chorda_creare(pn,
        _campus(datum, "inventarium")));
    rubrica = json_objectum_creare(pn);
    json_objectum_ponere(rubrica, "textus", json_chorda_creare(pn,
        _campus(datum, "rubrica")));
    json_objectum_ponere(rubrica, "versio", json_integer_creare(pn,
        (s64)I));
    json_objectum_ponere(status, "rubrica", rubrica);
    si (filtrum != NIHIL && json_est_objectum(filtrum))
    {
        json_objectum_ponere(status, "filtrum",
            _copia_chordarum(filtrum, pn));
    }
    si (implenda != NIHIL && json_est_objectum(implenda))
    {
        json_objectum_ponere(status, "implenda",
            _copia_chordarum(implenda, pn));
    }
    redde VERUM;
}

interior b32
_rubricam_mutare (
    JsonValor* status,
    JsonValor* datum,
      Piscina* pn)
{
    JsonValor* rubrica;
       chorda  textus = _campus(datum, "textus");

    si (!_photographata(status) || textus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    rubrica = json_objectum_creare(pn);
    json_objectum_ponere(rubrica, "textus", json_chorda_creare(pn,
        textus));
    json_objectum_ponere(rubrica, "versio", json_integer_creare(pn,
        _versio(status) + (s64)I));
    json_objectum_ponere(status, "rubrica", rubrica);
    redde VERUM;
}

/* gradum ordinis scribere; campus 'opus' prior servatur */
interior vacuum
_gradum_ponere (
    JsonValor* status,
       chorda  clavis,
       chorda  status_gradus,
       chorda  per_quid,
       chorda  nota,
       chorda  opus,
       chorda  actor,
       chorda  creatum,
      Piscina* pn)
{
    JsonValor* gradus = _gradus(status, pn);
    JsonValor* prior = json_objectum_capere_chorda(gradus, clavis);
    JsonValor* g = json_objectum_creare(pn);
       chorda  opus_prior = _campus(prior, "opus");

    json_objectum_ponere(g, "status", json_chorda_creare(pn,
        status_gradus));
    json_objectum_ponere(g, "versio", json_integer_creare(pn,
        _versio(status)));
    json_objectum_ponere(g, "actor", json_chorda_creare(pn, actor));
    json_objectum_ponere(g, "creatum", json_chorda_creare(pn, creatum));
    si (per_quid.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(g, "per", json_chorda_creare(pn,
            per_quid));
    }
    si (nota.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(g, "nota", json_chorda_creare(pn, nota));
    }
    si (opus.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(g, "opus", json_chorda_creare(pn, opus));
    }
    alioquin si (opus_prior.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(g, "opus", json_chorda_creare(pn,
            opus_prior));
    }
    json_objectum_ponere_chorda(gradus, clavis, g);
}

interior b32
_gradus_ponere (
    JsonValor* status,
    JsonValor* datum,
       chorda  actor,
       chorda  creatum,
      Piscina* pn)
{
    JsonValor* acies    = json_objectum_capere(datum, "ordines");
    JsonValor* ordines  = json_objectum_capere(status, "ordines");
       chorda  st       = _campus(datum, "status");
       chorda  nulla;
          b32  mutatum  = FALSUM;
          i32  k;

    nulla.mensura  = ZEPHYRUM;
    nulla.datum    = NIHIL;
    si (   acies == NIHIL || !json_est_tabulatum(acies)
        || (!_est(st, "factum") && !_est(st, "omissum")
            && !_est(st, "apertum")))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(acies); k++)
    {
        JsonValor* v = json_tabulatum_obtinere(acies, k);

        si (   v == NIHIL || !json_est_chorda(v)
            || _ordo_index(ordines, json_ad_chorda(v)) < ZEPHYRUM)
        {
            perge;
        }
        _gradum_ponere(status, json_ad_chorda(v), st,
            _campus(datum, "per"), _campus(datum, "nota"), nulla, actor,
            creatum, pn);
        mutatum = VERUM;
    }
    redde mutatum;
}

interior b32
_gradum_promovere (
    JsonValor* status,
    JsonValor* datum,
       chorda  actor,
       chorda  creatum,
      Piscina* pn)
{
    chorda ordo = _campus(datum, "ordo");
    chorda opus = _campus(datum, "opus");
    chorda nulla;

    nulla.mensura  = ZEPHYRUM;
    nulla.datum    = NIHIL;
    si (   opus.mensura == ZEPHYRUM
        || _ordo_index(json_objectum_capere(status, "ordines"), ordo)
           < ZEPHYRUM)
    {
        redde FALSUM;
    }
    _gradum_ponere(status, ordo, chorda_ex_literis("promotum", pn),
        nulla,
        nulla, opus, actor, creatum, pn);
    redde VERUM;
}

interior b32
_ordines_addere (
    JsonValor* status,
    JsonValor* datum,
       chorda  creatum,
      Piscina* pn)
{
    JsonValor* acies    = json_objectum_capere(datum, "ordines");
    JsonValor* ordines  = json_objectum_capere(status, "ordines");
          b32  mutatum  = FALSUM;
          i32  k;

    si (   acies   == NIHIL || !json_est_tabulatum(acies)
        || ordines == NIHIL || !json_est_tabulatum(ordines))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(acies); k++)
    {
        JsonValor* v = json_tabulatum_obtinere(acies, k);

        si (   v != NIHIL && json_est_chorda(v)
            && json_ad_chorda(v).mensura > ZEPHYRUM
            && _ordo_index(ordines, json_ad_chorda(v)) < ZEPHYRUM)
        {
            _ordinem_addere(ordines, json_ad_chorda(v), creatum, pn);
            mutatum = VERUM;
        }
    }
    redde mutatum;
}

b32
gesta_expeditio_applicare (
     JsonValor* status,
        chorda  genus_eventus,
     JsonValor* datum,
        chorda  actor,
        chorda  creatum,
       Piscina* pn)
{
    si (   status == NIHIL || !json_est_objectum(status)
        || datum  == NIHIL || !json_est_objectum(datum))
    {
        redde FALSUM;
    }
    si (_est(genus_eventus, "expeditio-photographia"))
    {
        redde _photographiam_plicare(status, datum, creatum, pn);
    }
    si (_est(genus_eventus, "rubrica-mutata"))
    {
        redde _rubricam_mutare(status, datum, pn);
    }
    si (_est(genus_eventus, "gradus-positus"))
    {
        redde _gradus_ponere(status, datum, actor, creatum, pn);
    }
    si (_est(genus_eventus, "gradus-promotus"))
    {
        redde _gradum_promovere(status, datum, actor, creatum, pn);
    }
    si (_est(genus_eventus, "ordines-additi"))
    {
        redde _ordines_addere(status, datum, creatum, pn);
    }
    redde FALSUM;
}


/* ==================================================
 * VALIDATIO (stricta - causae omnes simul)
 * ================================================== */

interior vacuum
_causa (
      ChordaAedificator* causae,
                    i32* numerus,
     constans character* ante,
                 chorda  clavis,
     constans character* post)
{
    chorda_aedificator_appendere_literis(causae,
        *numerus > ZEPHYRUM ? "\n  - " : "  - ");
    chorda_aedificator_appendere_literis(causae, ante);
    si (clavis.datum != NIHIL)
    {
        chorda_aedificator_appendere_chorda(causae, clavis);
    }
    chorda_aedificator_appendere_literis(causae, post);
    (*numerus)++;
}

interior chorda
_nulla (vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

/* acies clavium: non vacua, chordae non vacuae, nulla bis; si
 * 'in_photographia' datur, quaeque ibi esse (VERUM) aut NON esse
 * (FALSUM) debet */
interior vacuum
_aciem_validare (
              JsonValor* acies,
              JsonValor* ordines,
                    b32  debet_adesse,
     constans character* titulus,
      ChordaAedificator* causae,
                    i32* numerus)
{
    i32 k;

    si (   acies == NIHIL || !json_est_tabulatum(acies)
        || json_tabulatum_numerus(acies) == ZEPHYRUM)
    {
        _causa(causae, numerus, titulus, _nulla(),
            ": acies non vacua requiritur");
        redde;
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(acies); k++)
    {
        JsonValor* v = json_tabulatum_obtinere(acies, k);
           chorda  clavis;
              i32  j;
              b32  bis = FALSUM;

        si (   v                         == NIHIL || !json_est_chorda(v)
            || json_ad_chorda(v).mensura == ZEPHYRUM)
        {
            _causa(causae, numerus, titulus, _nulla(),
                ": clavis vacua aut non chorda");
            perge;
        }
        clavis = json_ad_chorda(v);
        per (j = ZEPHYRUM; j < k; j++)
        {
            JsonValor* w = json_tabulatum_obtinere(acies, j);

            si (   w != NIHIL && json_est_chorda(w)
                && chorda_aequalis(json_ad_chorda(w), clavis))
            {
                bis = VERUM;
            }
        }
        si (bis)
        {
            _causa(causae, numerus, "ordo '", clavis,
                "' bis in eodem eventu");
        }
        alioquin si (   ordines != NIHIL && debet_adesse
                     && _ordo_index(ordines, clavis) < ZEPHYRUM)
        {
            _causa(causae, numerus, "ordo '", clavis,
                "' non in photographia expeditionis");
        }
        alioquin si (   ordines != NIHIL && !debet_adesse
                     && _ordo_index(ordines, clavis) >= ZEPHYRUM)
        {
            _causa(causae, numerus, "ordo '", clavis,
                "' iam in photographia");
        }
    }
}

interior vacuum
_photographiam_validare (
             JsonValor* status,
             JsonValor* datum,
     ChordaAedificator* causae,
                   i32* numerus)
{
    JsonValor* filtrum   = json_objectum_capere(datum, "filtrum");
    JsonValor* implenda  = json_objectum_capere(datum, "implenda");

    si (_photographata(status))
    {
        _causa(causae, numerus, "photographia iam facta - expeditio"
            " semel photographatur ('ordines-additi' pro incremento)",
            _nulla(), "");
    }
    si (_campus(datum, "inventarium").mensura == ZEPHYRUM)
    {
        _causa(causae, numerus, "inventarium requiritur", _nulla(), "");
    }
    si (_campus(datum, "rubrica").mensura == ZEPHYRUM)
    {
        _causa(causae, numerus, "rubrica requiritur", _nulla(), "");
    }
    _aciem_validare(json_objectum_capere(datum, "ordines"), NIHIL,
        VERUM,
        "ordines", causae, numerus);
    si (filtrum != NIHIL)
    {
        chorda op = _campus(filtrum, "op");

        si (   _campus(filtrum, "lens").mensura == ZEPHYRUM
            || (!_est(op, "=") && !_est(op, "!=")))
        {
            _causa(causae, numerus, "filtrum: {lens, op: = | !=, valor}"
                " requiritur", _nulla(), "");
        }
    }
    si (   implenda != NIHIL
        && (   _campus(implenda, "lens").mensura == ZEPHYRUM
            || _campus(implenda, "genus").mensura == ZEPHYRUM
            || _campus(implenda, "valor").mensura == ZEPHYRUM))
    {
        _causa(causae, numerus, "implenda: {lens, genus, valor}"
            " requiritur", _nulla(), "");
    }
}

interior vacuum
_gradus_validare (
             JsonValor* status,
             JsonValor* datum,
     ChordaAedificator* causae,
                   i32* numerus)
{
    chorda st = _campus(datum, "status");

    _aciem_validare(json_objectum_capere(datum, "ordines"),
        json_objectum_capere(status, "ordines"), VERUM, "ordines",
        causae,
        numerus);
    si (   !_est(st, "factum") && !_est(st, "omissum")
        && !_est(st, "apertum"))
    {
        _causa(causae, numerus, "status '", st,
            "' ignotus (factum | omissum | apertum)");
    }
    si (   _est(st, "omissum")
        && _campus(datum, "nota").mensura == ZEPHYRUM)
    {
        _causa(causae, numerus,
            "omissum causam poscit: nota requiritur",
            _nulla(), "");
    }
}

interior vacuum
_promotionem_validare (
             JsonValor* status,
             JsonValor* datum,
     ChordaAedificator* causae,
                   i32* numerus)
{
       chorda  ordo    = _campus(datum, "ordo");
    JsonValor* gradus  = json_objectum_capere(status, "gradus");
       chorda  prior   = _campus(gradus != NIHIL
           ? json_objectum_capere_chorda(gradus, ordo) : NIHIL,
           "status");

    si (_ordo_index(json_objectum_capere(status, "ordines"), ordo)
        < ZEPHYRUM)
    {
        _causa(causae, numerus, "ordo '", ordo,
            "' non in photographia expeditionis");
    }
    alioquin si (_est(prior, "factum") || _est(prior, "promotum"))
    {
        _causa(causae, numerus, "ordo '", ordo,
            "' iam factus aut promotus");
    }
    si (_campus(datum, "opus").mensura == ZEPHYRUM)
    {
        _causa(causae, numerus, "opus requiritur", _nulla(), "");
    }
}

constans character*
gesta_expeditio_validare (
     JsonValor* status,
        chorda  genus_eventus,
     JsonValor* datum,
       Piscina* pn)
{
    ChordaAedificator* causae   = chorda_aedificator_creare(pn, DXII);
                  i32  numerus  = ZEPHYRUM;

    si (datum == NIHIL || !json_est_objectum(datum))
    {
        _causa(causae, &numerus, "datum: obiectum requiritur", _nulla(),
            "");
    }
    alioquin si (_est(genus_eventus, "expeditio-photographia"))
    {
        _photographiam_validare(status, datum, causae, &numerus);
    }
    alioquin si (!gesta_expeditio_eventus_est(genus_eventus))
    {
        _causa(causae, &numerus, "genus eventus '", genus_eventus,
            "' non est expeditionis");
    }
    alioquin si (!_photographata(status))
    {
        _causa(causae, &numerus, "photographia nondum facta", _nulla(),
            "");
    }
    alioquin si (_est(genus_eventus, "rubrica-mutata"))
    {
        si (_campus(datum, "textus").mensura == ZEPHYRUM)
        {
            _causa(causae, &numerus, "rubrica: textus requiritur",
                _nulla(), "");
        }
    }
    alioquin si (_est(genus_eventus, "gradus-positus"))
    {
        _gradus_validare(status, datum, causae, &numerus);
    }
    alioquin si (_est(genus_eventus, "gradus-promotus"))
    {
        _promotionem_validare(status, datum, causae, &numerus);
    }
    alioquin
    {
        _aciem_validare(json_objectum_capere(datum, "ordines"),
            json_objectum_capere(status, "ordines"), FALSUM, "ordines",
            causae, &numerus);
    }
    si (numerus == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(causae), pn);
}
