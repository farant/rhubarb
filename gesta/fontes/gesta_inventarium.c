/* gesta_inventarium.c - inventaria: plicatura et validatio (PURAE)
 *
 * Vide gesta_inventarium.h et gesta_inventarium.worklog.md.
 */
#include "gesta_inventarium.h"
#include "chorda_aedificator.h"

#define GENUS_ITA_NON  "ita-non"
#define GENUS_TEXTUS   "textus"

interior b32
_est (
                 chorda  c,
     constans character* litterae)
{
    redde chorda_aequalis_literis(c, litterae);
}

/* chorda campi datum[clavis]; vacua si abest aut non chorda */
interior chorda
_campus (
             JsonValor* obiectum,
    constans character* clavis)
{
    JsonValor* v = obiectum != NIHIL
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
gesta_inventarium_eventus_est (
    chorda genus_eventus)
{
    redde _est(genus_eventus, "ordo-additus")
        || _est(genus_eventus, "ordo-remotus")
        || _est(genus_eventus, "lens-addita")
        || _est(genus_eventus, "cella-posita");
}

/* acies status[clavis], creata si deest */
interior JsonValor*
_acies (
             JsonValor* status,
    constans character* clavis,
               Piscina* pn)
{
    JsonValor* a = json_objectum_capere(status, clavis);

    si (a == NIHIL || !json_est_tabulatum(a))
    {
        a = json_tabulatum_creare(pn);
        json_objectum_ponere(status, clavis, a);
    }
    redde a;
}

/* obiectum status['cellae'], creatum si deest */
interior JsonValor*
_cellae (
    JsonValor* status,
      Piscina* pn)
{
    JsonValor* c = json_objectum_capere(status, "cellae");

    si (c == NIHIL || !json_est_objectum(c))
    {
        c = json_objectum_creare(pn);
        json_objectum_ponere(status, "cellae", c);
    }
    redde c;
}

/* index ordinis per clavem; -I si abest */
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

/* lens per nomen; NIHIL si abest */
interior JsonValor*
_lens (
     JsonValor* lentes,
        chorda  appellatio)
{
    i32 k;

    si (lentes == NIHIL || !json_est_tabulatum(lentes))
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(lentes); k++)
    {
        JsonValor* l = json_tabulatum_obtinere(lentes, k);

        si (chorda_aequalis(_campus(l, "nomen"), appellatio))
        {
            redde l;
        }
    }
    redde NIHIL;
}

/* clavis in acie chordarum? */
interior b32
_in_acie (
     JsonValor* acies,
        chorda  clavis)
{
    i32 k;

    si (acies == NIHIL || !json_est_tabulatum(acies))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(acies); k++)
    {
        JsonValor* v = json_tabulatum_obtinere(acies, k);

        si (   v != NIHIL && json_est_chorda(v)
            && chorda_aequalis(json_ad_chorda(v), clavis))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * PLICATURA (defensiva - replay numquam fallit)
 * ================================================== */

interior b32
_ordines_addere (
     JsonValor* status,
     JsonValor* datum,
        chorda  actor,
        chorda  creatum,
       Piscina* pn)
{
    JsonValor* nova     = json_objectum_capere(datum, "ordines");
    JsonValor* ordines  = _acies(status, "ordines", pn);
          b32  mutatum  = FALSUM;
          i32  k;

    si (nova == NIHIL || !json_est_tabulatum(nova))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(nova); k++)
    {
        JsonValor* v = json_tabulatum_obtinere(nova, k);
        JsonValor* o;
           chorda  clavis;

        si (v == NIHIL || !json_est_chorda(v))
        {
            perge;
        }
        clavis = json_ad_chorda(v);
        si (   clavis.mensura               == ZEPHYRUM
            || _ordo_index(ordines, clavis) >= ZEPHYRUM)
        {
            perge;
        }
        o = json_objectum_creare(pn);
        json_objectum_ponere(o, "clavis", json_chorda_creare(pn,
            clavis));
        json_objectum_ponere(o, "additus",
            json_chorda_creare(pn, creatum));
        json_objectum_ponere(o, "actor", json_chorda_creare(pn, actor));
        json_tabulatum_addere(ordines, o);
        mutatum = VERUM;
    }
    redde mutatum;
}

interior b32
_ordines_removere (
    JsonValor* status,
    JsonValor* datum,
      Piscina* pn)
{
               JsonValor* removenda = json_objectum_capere(datum,
                   "ordines");
               JsonValor* ordines   = json_objectum_capere(status,
                   "ordines");
               JsonValor* cellae    = json_objectum_capere(status,
                   "cellae");
               JsonValor* nova     = json_tabulatum_creare(pn);
                     b32  mutatum  = FALSUM;
                     i32  k;

    si (   removenda == NIHIL || !json_est_tabulatum(removenda)
        || ordines   == NIHIL || !json_est_tabulatum(ordines))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(ordines); k++)
    {
        JsonValor* o = json_tabulatum_obtinere(ordines, k);

        si (_in_acie(removenda, _campus(o, "clavis")))
        {
            mutatum = VERUM;
            perge;
        }
        json_tabulatum_addere(nova, o);
    }
    json_objectum_ponere(status, "ordines", nova);
    si (cellae != NIHIL && json_est_objectum(cellae))
    {
                   JsonValor* reliquae =
                       json_objectum_creare(pn);
        JsonObjectumIterator  iter = json_objectum_iterator(cellae);
                      chorda  clavis;
                   JsonValor* linea;

        dum (json_objectum_iterator_proxima(&iter, &clavis, &linea))
        {
            si (!_in_acie(removenda, clavis))
            {
                json_objectum_ponere_chorda(reliquae, clavis, linea);
            }
        }
        json_objectum_ponere(status, "cellae", reliquae);
    }
    redde mutatum;
}

interior b32
_lentem_addere (
    JsonValor* status,
    JsonValor* datum,
      Piscina* pn)
{
    JsonValor* lentes      = _acies(status, "lentes", pn);
       chorda  appellatio  = _campus(datum, "nomen");
       chorda  genus       = _campus(datum, "genus_valoris");
       chorda  corpus      = _campus(datum, "corpus");
    JsonValor* l;

    si (   appellatio.mensura        == ZEPHYRUM
        || _lens(lentes, appellatio) != NIHIL)
    {
        redde FALSUM;
    }
    l = json_objectum_creare(pn);
    json_objectum_ponere(l, "nomen", json_chorda_creare(pn,
        appellatio));
    json_objectum_ponere(l, "genus_valoris",
        json_chorda_creare(pn, genus));
    si (corpus.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(l, "corpus", json_chorda_creare(pn,
            corpus));
    }
    json_tabulatum_addere(lentes, l);
    redde VERUM;
}

interior b32
_cellas_ponere (
     JsonValor* status,
     JsonValor* datum,
        chorda  actor,
        chorda  creatum,
       Piscina* pn)
{
    JsonValor* cellae_in  = json_objectum_capere(datum, "cellae");
    JsonValor* ordines    = json_objectum_capere(status, "ordines");
    JsonValor* lentes     = json_objectum_capere(status, "lentes");
       chorda  fons       = _campus(datum, "fons");
       chorda  per_quid   = _campus(datum, "per");
    JsonValor* cellae;
          b32  mutatum    = FALSUM;
          i32  k;

    si (cellae_in == NIHIL || !json_est_tabulatum(cellae_in))
    {
        redde FALSUM;
    }
    cellae = _cellae(status, pn);
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(cellae_in); k++)
    {
        JsonValor* c     = json_tabulatum_obtinere(cellae_in, k);
           chorda  ordo  = _campus(c, "ordo");
           chorda  lens  = _campus(c, "lens");
        JsonValor* valor  = c != NIHIL
            ? json_objectum_capere(c, "valor") : NIHIL;
        JsonValor* linea;
        JsonValor* cella;

        si (   _ordo_index(ordines, ordo) < ZEPHYRUM
            || _lens(lentes, lens) == NIHIL
            || valor == NIHIL || !json_est_objectum(valor))
        {
            perge;
        }
        linea = json_objectum_capere_chorda(cellae, ordo);
        si (linea == NIHIL || !json_est_objectum(linea))
        {
            linea = json_objectum_creare(pn);
            json_objectum_ponere_chorda(cellae, ordo, linea);
        }
        cella = json_objectum_creare(pn);
        json_objectum_ponere(cella, "genus",
            json_chorda_creare(pn, _campus(valor, "genus")));
        json_objectum_ponere(cella, "valor",
            json_chorda_creare(pn, _campus(valor, "valor")));
        json_objectum_ponere(cella, "actor", json_chorda_creare(pn,
            actor));
        json_objectum_ponere(cella, "creatum",
            json_chorda_creare(pn, creatum));
        json_objectum_ponere(cella, "fons", json_chorda_creare(pn,
            fons));
        si (per_quid.mensura > ZEPHYRUM)
        {
            json_objectum_ponere(cella, "per",
                json_chorda_creare(pn, per_quid));
        }
        json_objectum_ponere_chorda(linea, lens, cella);
        mutatum = VERUM;
    }
    redde mutatum;
}

b32
gesta_inventarium_applicare (
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
    si (_est(genus_eventus, "ordo-additus"))
    {
        redde _ordines_addere(status, datum, actor, creatum, pn);
    }
    si (_est(genus_eventus, "ordo-remotus"))
    {
        redde _ordines_removere(status, datum, pn);
    }
    si (_est(genus_eventus, "lens-addita"))
    {
        redde _lentem_addere(status, datum, pn);
    }
    si (_est(genus_eventus, "cella-posita"))
    {
        redde _cellas_ponere(status, datum, actor, creatum, pn);
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

interior vacuum
_ordines_validare (
             JsonValor* status,
             JsonValor* datum,
                   b32  addere,
     ChordaAedificator* causae,
                   i32* numerus)
{
    JsonValor* acies    = json_objectum_capere(datum, "ordines");
    JsonValor* ordines  = json_objectum_capere(status, "ordines");
          i32  k;

    si (   acies == NIHIL || !json_est_tabulatum(acies)
        || json_tabulatum_numerus(acies) == ZEPHYRUM)
    {
        _causa(causae, numerus, "ordines: acies non vacua requiritur",
            _nulla(), "");
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
            _causa(causae, numerus,
                "ordines: clavis vacua aut non chorda",
                _nulla(), "");
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
        alioquin si (addere && _ordo_index(ordines, clavis) >= ZEPHYRUM)
        {
            _causa(causae, numerus, "ordo '", clavis, "' iam adest");
        }
        alioquin si (!addere && _ordo_index(ordines, clavis) < ZEPHYRUM)
        {
            _causa(causae, numerus, "ordo '", clavis, "' non adest");
        }
    }
    si (!addere && _campus(datum, "causa").mensura == ZEPHYRUM)
    {
        _causa(causae, numerus, "causa remotionis requiritur", _nulla(),
            "");
    }
}

interior vacuum
_lentem_validare (
             JsonValor* status,
             JsonValor* datum,
     ChordaAedificator* causae,
                   i32* numerus)
{
    chorda appellatio  = _campus(datum, "nomen");
    chorda genus       = _campus(datum, "genus_valoris");

    si (appellatio.mensura == ZEPHYRUM)
    {
        _causa(causae, numerus, "lens: nomen requiritur", _nulla(), "");
    }
    alioquin si (_lens(json_objectum_capere(status, "lentes"),
                 appellatio)
                 != NIHIL)
    {
        _causa(causae, numerus, "lens '", appellatio, "' iam adest");
    }
    si (!_est(genus, GENUS_ITA_NON) && !_est(genus, GENUS_TEXTUS))
    {
        _causa(causae, numerus, "genus_valoris '", genus,
            "' ignotum (ita-non | textus)");
    }
}

interior vacuum
_cellas_validare (
             JsonValor* status,
             JsonValor* datum,
     ChordaAedificator* causae,
                   i32* numerus)
{
    JsonValor* cellae   = json_objectum_capere(datum, "cellae");
    JsonValor* ordines  = json_objectum_capere(status, "ordines");
    JsonValor* lentes   = json_objectum_capere(status, "lentes");
       chorda  fons     = _campus(datum, "fons");
          i32  k;

    si (!_est(fons, "manu") && !_est(fons, "derivatum"))
    {
        _causa(causae, numerus, "fons '", fons,
            "' ignotus (manu | derivatum)");
    }
    si (   cellae == NIHIL || !json_est_tabulatum(cellae)
        || json_tabulatum_numerus(cellae) == ZEPHYRUM)
    {
        _causa(causae, numerus, "cellae: acies non vacua requiritur",
            _nulla(), "");
        redde;
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(cellae); k++)
    {
        JsonValor* c           = json_tabulatum_obtinere(cellae, k);
           chorda  ordo        = _campus(c, "ordo");
           chorda  appellatio  = _campus(c, "lens");
        JsonValor* valor  = c != NIHIL
            ? json_objectum_capere(c, "valor") : NIHIL;
        JsonValor* lens   = _lens(lentes, appellatio);
           chorda  genus;
           chorda  v;

        si (_ordo_index(ordines, ordo) < ZEPHYRUM)
        {
            _causa(causae, numerus, "cella: ordo '", ordo,
                "' non adest");
        }
        si (lens == NIHIL)
        {
            _causa(causae, numerus, "cella: lens '", appellatio,
                "' non adest");
        }
        si (valor == NIHIL || !json_est_objectum(valor))
        {
            _causa(causae, numerus, "cella '", ordo,
                "': valor obiectum {genus, valor} requiritur");
            perge;
        }
        genus  = _campus(valor, "genus");
        v      = _campus(valor, "valor");
        si (   lens != NIHIL
            && !chorda_aequalis(genus, _campus(lens, "genus_valoris")))
        {
            _causa(causae, numerus, "cella '", ordo,
                "': genus valoris lenti non congruit");
        }
        si (_est(genus, GENUS_ITA_NON))
        {
            si (   !_est(v, "ita") && !_est(v, "non")
                && !_est(v, "ignotum"))
            {
                _causa(causae, numerus, "cella '", ordo,
                    "': valor ita-non debet esse ita | non | ignotum");
            }
        }
        alioquin si (!_est(genus, GENUS_TEXTUS))
        {
            _causa(causae, numerus, "cella '", ordo,
                "': genus valoris ignotum (ita-non | textus)");
        }
    }
}

constans character*
gesta_inventarium_validare (
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
    alioquin si (_est(genus_eventus, "ordo-additus"))
    {
        _ordines_validare(status, datum, VERUM, causae, &numerus);
    }
    alioquin si (_est(genus_eventus, "ordo-remotus"))
    {
        _ordines_validare(status, datum, FALSUM, causae, &numerus);
    }
    alioquin si (_est(genus_eventus, "lens-addita"))
    {
        _lentem_validare(status, datum, causae, &numerus);
    }
    alioquin si (_est(genus_eventus, "cella-posita"))
    {
        _cellas_validare(status, datum, causae, &numerus);
    }
    alioquin
    {
        _causa(causae, &numerus, "genus eventus '", genus_eventus,
            "' non est inventarii");
    }
    si (numerus == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(causae), pn);
}
