/* recensio_carminis.c - Carmen recensitum (vide
 * include/recensio_carminis.h)
 *
 * FACERE colligit (censura, auscultatio totius et partium solarum,
 * tacitae, partitura rogata); SCRIPTORES duo eandem structuram
 * legunt. Numeri in JSON rotundantur (millesimae, dBFS decimae) ut
 * responsa stabilia sint inter machinas.
 */

#include "recensio_carminis.h"
#include "partitura.h"
#include "harmonia.h"
#include <math.h>
#include <string.h>

/* capacitas inventorum prima; plura -> iterum cum numero toto */
#define RECENSIO_INVENTA LXIV

interior i32
_mensurae (
    constans Carmen* carmen)
{
    i32 m = I;

    dum (carmen_pulsus_mensurae(carmen, m + I) < carmen->pulsus)
    {
        m++;
    }
    redde m;
}

interior f64
_gradus_soni (
    f32 x)
{
    redde x > 1e-6f ? 20.0 * log10((f64)x) : -120.0;
}

/* ad 1/partes rotundat; divisio (non multiplicatio per unitatem)
 * duplex proximum dat, ut scriptor JSON '-12.2' scribat, non
 * '-12.200000000000001' */
interior f64
_rotundare (
    f64 x,
    f64 partes)
{
    redde floor(x * partes + 0.5) / partes;
}

vacuum
recensio_carminis_ordinaria (
    RecensioForma* forma)
{
    memset(forma, ZEPHYRUM, magnitudo(RecensioForma));
    censura_ordinaria(&forma->censura);
}

b32
recensio_carminis_facere (
             constans Carmen* carmen,
       constans RecensioForma* forma,
    constans CantorResolutor* resolutor,
                     Piscina* piscina,
            RecensioCarminis* recensio)
{
    RecensioCarminis* r = recensio;
                 f32* exempla;
                 i32  quadra;
                 i32  capacitas = RECENSIO_INVENTA;
                 i32  i;

    memset(r, ZEPHYRUM, magnitudo(RecensioCarminis));
    r->carmen = carmen;
    si (forma != NIHIL)
    {
        r->forma = *forma;
    }
    alioquin
    {
        recensio_carminis_ordinaria(&r->forma);
    }
    r->mensurae = _mensurae(carmen);
    r->secundae = (f64)cantus_quadrum_pulsus(carmen->tempora,
        carmen->numerus_temporum, carmen->pulsus)
        / (f64)SONUS_FREQUENTIA;

    /* censura: numerus totus, deinde capacitas exacta si plura */
    r->inventa = (CensuraInventum*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(CensuraInventum));
    si (r->inventa == NIHIL)
    {
        redde FALSUM;
    }
    r->numerus_inventorum = censura_iudicare(carmen, &r->forma.censura,
        r->inventa, capacitas);
    si (r->numerus_inventorum > capacitas)
    {
        capacitas   = r->numerus_inventorum;
        r->inventa  = (CensuraInventum*)piscina_allocare(piscina,
            (memoriae_index)capacitas * magnitudo(CensuraInventum));
        si (r->inventa == NIHIL)
        {
            redde FALSUM;
        }
        (vacuum)censura_iudicare(carmen, &r->forma.censura, r->inventa,
            capacitas);
    }

    /* sonus: totum, partes solae, tacitae */
    si (   !auscultatio_reddere(carmen, resolutor, piscina, &exempla,
                                &quadra)
        || !auscultatio_metiri(carmen, exempla, quadra, piscina,
                               &r->sonus))
    {
        redde FALSUM;
    }
    si (carmen->numerus_partium > ZEPHYRUM)
    {
        r->partes = (AuscultatioSummarium*)piscina_allocare(piscina,
            (memoriae_index)carmen->numerus_partium
                * magnitudo(AuscultatioSummarium));
        si (r->partes == NIHIL)
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        si (!auscultatio_partem(carmen, i, resolutor, piscina,
                                &r->partes[i]))
        {
            redde FALSUM;
        }
    }
    r->tacitae = (i32*)piscina_allocare(piscina,
        (memoriae_index)r->sonus.numerus * magnitudo(i32));
    si (r->tacitae == NIHIL)
    {
        redde FALSUM;
    }
    r->numerus_tacitarum = auscultatio_tacitae(carmen, &r->sonus,
        r->tacitae, r->sonus.numerus);

    /* partitura rogata */
    si (r->forma.partitura)
    {
           PartituraForma  pf;
        ChordaAedificator* a = chorda_aedificator_creare(piscina, M);

        memset(&pf, ZEPHYRUM, magnitudo(pf));
        si (r->forma.harmonia)
        {
            pf.additae[ZEPHYRUM].titulus   = "harmonia";
            pf.additae[ZEPHYRUM].scribere  = harmonia_lineam_scribere;
        }
        si (a == NIHIL || !partitura_scribere(carmen, &pf, a))
        {
            redde FALSUM;
        }
        r->partitura = chorda_aedificator_finire(a);
    }
    redde VERUM;
}

i32
recensio_carminis_monita (
    constans RecensioCarminis* recensio)
{
    redde recensio->numerus_inventorum + recensio->numerus_tacitarum
        + (recensio->sonus.tonsa > ZEPHYRUM ? I : ZEPHYRUM);
}


/* ==================================================
 * Notae partium (ambitus)
 * ================================================== */

/* nota scripta ("G#4"); '-' si NIHIL */
interior b32
_notam_scribere (
        constans Carmen* carmen,
    constans CarmenNota* nota,
                    s64  pulsus,
      ChordaAedificator* a)
{
    si (nota == NIHIL)
    {
        redde chorda_aedificator_appendere_character(a, '-');
    }
    redde partitura_notam_scribere(carmen, nota, pulsus, a);
}

/* nota ut chorda (e piscina) */
interior chorda
_nota (
        constans Carmen* carmen,
    constans CarmenNota* nota,
                    s64  pulsus,
                Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, XVI);
               chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (a == NIHIL || !_notam_scribere(carmen, nota, pulsus, a))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}

/* "d maior" in aedificatorem */
interior b32
_clavem_scribere (
      constans Carmen* carmen,
    ChordaAedificator* a)
{
    constans CarmenClavis* k = &carmen->claves[ZEPHYRUM];

    redde chorda_aedificator_appendere_literis(a,
              partitura_clavis_littera(k))
        && chorda_aedificator_appendere_literis(a,
              k->minor ? " minor" : " maior");
}


/* ==================================================
 * Textus
 * ================================================== */

interior b32
_numerum_scribere (
    ChordaAedificator* a,
                  f64  x,
                  i32  decimales)
{
    redde chorda_aedificator_appendere_f64(a, x, decimales);
}

b32
recensio_carminis_textus (
    constans RecensioCarminis* recensio,
            ChordaAedificator* a)
{
    constans RecensioCarminis* r = recensio;
              constans Carmen* k = r->carmen;
                          b32  bene;
                          i32  latitudo_tituli       = ZEPHYRUM;
                          i32  latitudo_instrumenti  = ZEPHYRUM;
                          i32  i;
                          b32  primum;

    bene = chorda_aedificator_appendere_literis(a, "recensio ");
    bene = chorda_aedificator_appendere_chorda(a, k->titulus) && bene;
    bene = chorda_aedificator_appendere_literis(a, ": ") && bene;
    bene = _clavem_scribere(k, a) && bene;
    bene = chorda_aedificator_appendere_literis(a, ", ") && bene;
    bene = chorda_aedificator_appendere_i32(a,
        k->metra[ZEPHYRUM].numerator) && bene;
    bene = chorda_aedificator_appendere_character(a, '/') && bene;
    bene = chorda_aedificator_appendere_i32(a,
        k->metra[ZEPHYRUM].denominator) && bene;
    bene = chorda_aedificator_appendere_literis(a, ", ") && bene;
    bene = chorda_aedificator_appendere_i32(a,
        k->tempora[ZEPHYRUM].tempus) && bene;
    bene = chorda_aedificator_appendere_literis(a, ", ") && bene;
    bene = chorda_aedificator_appendere_i32(a, r->mensurae) && bene;
    bene = chorda_aedificator_appendere_literis(a, " mensurae, ")
        && bene;
    bene = _numerum_scribere(a, r->secundae, III) && bene;
    bene = chorda_aedificator_appendere_literis(a, " s; monita ")
        && bene;
    bene = chorda_aedificator_appendere_i32(a,
        recensio_carminis_monita(r)) && bene;
    bene = chorda_aedificator_appendere_character(a, '\n') && bene;

    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        si (k->partes[i].titulus.mensura > latitudo_tituli)
        {
            latitudo_tituli = k->partes[i].titulus.mensura;
        }
        si (k->partes[i].instrumentum.mensura > latitudo_instrumenti)
        {
            latitudo_instrumenti = k->partes[i].instrumentum.mensura;
        }
    }
    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        constans CarmenPars* p    = &k->partes[i];
           PartituraAmbitus  amb  = partitura_ambitus(p);

        bene = chorda_aedificator_appendere_literis(a, "  pars ")
            && bene;
        bene = chorda_aedificator_appendere_chorda(a, p->titulus)
            && bene;
        bene = chorda_aedificator_appendere_repetita(a, ' ',
            latitudo_tituli - p->titulus.mensura + II) && bene;
        bene = chorda_aedificator_appendere_chorda(a, p->instrumentum)
            && bene;
        bene = chorda_aedificator_appendere_repetita(a, ' ',
            latitudo_instrumenti - p->instrumentum.mensura + II)
                && bene;
        bene = chorda_aedificator_appendere_literis(a, "notae ")
            && bene;
        bene = chorda_aedificator_appendere_i32(a, amb.numerus) && bene;
        bene = chorda_aedificator_appendere_literis(a, "  ") && bene;
        bene = _notam_scribere(k, amb.infima, amb.pulsus_infimae, a)
            && bene;
        bene = chorda_aedificator_appendere_literis(a, "..") && bene;
        bene = _notam_scribere(k, amb.summa, amb.pulsus_summae, a)
            && bene;
        bene = chorda_aedificator_appendere_literis(a, "  rms ")
            && bene;
        bene = _numerum_scribere(a, (f64)r->partes[i].rms, III) && bene;
        bene = chorda_aedificator_appendere_literis(a, " (") && bene;
        bene = _numerum_scribere(a, _gradus_soni(r->partes[i].rms), I)
            && bene;
        bene = chorda_aedificator_appendere_literis(a, " dBFS)  apex ")
            && bene;
        bene = _numerum_scribere(a, (f64)r->partes[i].apex, III)
            && bene;
        bene = chorda_aedificator_appendere_character(a, '\n') && bene;
    }

    bene = chorda_aedificator_appendere_literis(a, "  sonus: apex ")
        && bene;
    bene = _numerum_scribere(a, (f64)r->sonus.apex, III) && bene;
    bene = chorda_aedificator_appendere_literis(a, " (") && bene;
    bene = _numerum_scribere(a, _gradus_soni(r->sonus.apex), I) && bene;
    bene = chorda_aedificator_appendere_literis(a, " dBFS), tonsa ")
        && bene;
    bene = chorda_aedificator_appendere_i32(a, r->sonus.tonsa) && bene;
    bene = chorda_aedificator_appendere_literis(a, ", rms ") && bene;
    bene = _numerum_scribere(a, (f64)r->sonus.rms, III) && bene;
    bene = chorda_aedificator_appendere_literis(a, " (") && bene;
    bene = _numerum_scribere(a, _gradus_soni(r->sonus.rms), I) && bene;
    bene = chorda_aedificator_appendere_literis(a, " dBFS)\n") && bene;

    bene = chorda_aedificator_appendere_literis(a, "  censura (")
        && bene;
    primum = VERUM;
    per (i = ZEPHYRUM; i < (i32)CENSURA_REGULAE; i++)
    {
        si (r->forma.censura.activae[i])
        {
            si (!primum)
            {
                bene = chorda_aedificator_appendere_character(a, ' ')
                    && bene;
            }
            primum = FALSUM;
            bene = chorda_aedificator_appendere_literis(a,
                censura_titulus((CensuraRegula)i)) && bene;
        }
    }
    bene = chorda_aedificator_appendere_literis(a, "): ") && bene;
    bene = chorda_aedificator_appendere_i32(a, r->numerus_inventorum)
        && bene;
    bene = chorda_aedificator_appendere_literis(a, " inventa\n")
        && bene;
    per (i = ZEPHYRUM; i < r->numerus_inventorum; i++)
    {
        bene = chorda_aedificator_appendere_literis(a, "    ") && bene;
        bene = censura_inventum_scribere(k, &r->inventa[i], a) && bene;
    }

    bene = chorda_aedificator_appendere_literis(a, "  tacitae:")
        && bene;
    si (r->numerus_tacitarum == ZEPHYRUM)
    {
        bene = chorda_aedificator_appendere_literis(a, " nullae")
            && bene;
    }
    per (i = ZEPHYRUM; i < r->numerus_tacitarum; i++)
    {
        bene = chorda_aedificator_appendere_character(a, ' ') && bene;
        bene = chorda_aedificator_appendere_i32(a, r->tacitae[i])
            && bene;
    }
    bene = chorda_aedificator_appendere_character(a, '\n') && bene;
    si (r->partitura.mensura > ZEPHYRUM)
    {
        bene = chorda_aedificator_appendere_character(a, '\n') && bene;
        bene = chorda_aedificator_appendere_chorda(a, r->partitura)
            && bene;
    }
    redde bene;
}


/* ==================================================
 * JSON
 * ================================================== */

interior JsonValor*
_numerus_json (
    Piscina* piscina,
        f64  x,
        f64  partes)
{
    redde json_fluitans_creare(piscina, _rotundare(x, partes));
}

interior JsonValor*
_chorda_json (
    Piscina* piscina,
     chorda  c)
{
    redde json_chorda_creare(piscina, c);
}

/* notae inventi in tabulatum: prima et altera (si sunt) */
interior JsonValor*
_notae_json (
          constans Carmen* carmen,
      constans CarmenNota* a,
      constans CarmenNota* b,
                      s64  pulsus,
                  Piscina* piscina)
{
    JsonValor* t = json_tabulatum_creare(piscina);

    si (t != NIHIL && a != NIHIL)
    {
        json_tabulatum_addere(t, _chorda_json(piscina,
            _nota(carmen, a, pulsus, piscina)));
    }
    si (t != NIHIL && b != NIHIL)
    {
        json_tabulatum_addere(t, _chorda_json(piscina,
            _nota(carmen, b, pulsus, piscina)));
    }
    redde t;
}

interior JsonValor*
_inventum_json (
             constans Carmen* carmen,
    constans CensuraInventum* x,
                     Piscina* piscina)
{
            JsonValor* o = json_objectum_creare(piscina);
    ChordaAedificator* a = chorda_aedificator_creare(piscina, CXXVIII);
                  s64  pulsus;
               chorda  linea;

    si (   o == NIHIL || a == NIHIL
        || !censura_inventum_scribere(carmen, x,
                                                                a))
    {
        redde NIHIL;
    }
    linea = chorda_aedificator_finire(a);
    linea.mensura--;   /* sine '\n' */
    pulsus = carmen_pulsus_mensurae(carmen, x->mensura) + x->pulsus;
    json_objectum_ponere(o, "regula",
        json_chorda_creare_literis(piscina,
        censura_titulus(x->regula)));
    json_objectum_ponere(o, "mensura", json_integer_creare(piscina,
        (s64)x->mensura));
    json_objectum_ponere(o, "pulsus", json_integer_creare(piscina,
        x->pulsus));
    json_objectum_ponere(o, "pars", _chorda_json(piscina, x->pars));
    si (x->pars_altera.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(o, "pars_altera", _chorda_json(piscina,
            x->pars_altera));
    }
    json_objectum_ponere(o, "notae", _notae_json(carmen, x->nota,
        x->nota_altera, pulsus, piscina));
    si (x->nota_prior != NIHIL)
    {
        json_objectum_ponere(o, "priores", _notae_json(carmen,
            x->nota_prior, x->nota_altera_prior, pulsus, piscina));
    }
    json_objectum_ponere(o, "frustum", json_integer_creare(piscina,
        (s64)x->frustum));
    json_objectum_ponere(o, "offset", json_integer_creare(piscina,
        (s64)x->offset));
    json_objectum_ponere(o, "textus", _chorda_json(piscina, linea));
    redde o;
}

JsonValor*
recensio_carminis_json (
    constans RecensioCarminis* recensio,
                      Piscina* piscina)
{
    constans RecensioCarminis* r = recensio;
              constans Carmen* k = r->carmen;
                    JsonValor* o;
                    JsonValor* partes;
                    JsonValor* sonus;
                    JsonValor* mensurae;
                    JsonValor* censura;
                    JsonValor* regulae;
                    JsonValor* inventa;
            ChordaAedificator* a;
                          i32  i;
                          i32  t = ZEPHYRUM;

    o         = json_objectum_creare(piscina);
    partes    = json_tabulatum_creare(piscina);
    sonus     = json_objectum_creare(piscina);
    mensurae  = json_tabulatum_creare(piscina);
    censura   = json_objectum_creare(piscina);
    regulae   = json_tabulatum_creare(piscina);
    inventa   = json_tabulatum_creare(piscina);
    a         = chorda_aedificator_creare(piscina, LXIV);
    si (   o        == NIHIL || partes == NIHIL || sonus == NIHIL
        || mensurae == NIHIL || censura == NIHIL || regulae == NIHIL
        || inventa  == NIHIL || a == NIHIL)
    {
        redde NIHIL;
    }
    json_objectum_ponere(o, "carmen", _chorda_json(piscina,
        k->titulus));
    (vacuum)_clavem_scribere(k, a);
    json_objectum_ponere(o, "clavis", _chorda_json(piscina,
        chorda_aedificator_finire(a)));
    a = chorda_aedificator_creare(piscina, XVI);
    si (a == NIHIL)
    {
        redde NIHIL;
    }
    (vacuum)chorda_aedificator_appendere_i32(a,
        k->metra[ZEPHYRUM].numerator);
    (vacuum)chorda_aedificator_appendere_character(a, '/');
    (vacuum)chorda_aedificator_appendere_i32(a,
        k->metra[ZEPHYRUM].denominator);
    json_objectum_ponere(o, "metrum", _chorda_json(piscina,
        chorda_aedificator_finire(a)));
    json_objectum_ponere(o, "tempus", json_integer_creare(piscina,
        (s64)k->tempora[ZEPHYRUM].tempus));
    json_objectum_ponere(o, "mensurae", json_integer_creare(piscina,
        (s64)r->mensurae));
    json_objectum_ponere(o, "secundae", _numerus_json(piscina,
        r->secundae, 1000.0));
    json_objectum_ponere(o, "monita", json_integer_creare(piscina,
        (s64)recensio_carminis_monita(r)));

    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        constans CarmenPars* p    = &k->partes[i];
           PartituraAmbitus  amb  = partitura_ambitus(p);
                  JsonValor* x    = json_objectum_creare(piscina);

        si (x == NIHIL)
        {
            redde NIHIL;
        }
        json_objectum_ponere(x, "titulus", _chorda_json(piscina,
            p->titulus));
        json_objectum_ponere(x, "instrumentum", _chorda_json(piscina,
            p->instrumentum));
        json_objectum_ponere(x, "notae", json_integer_creare(piscina,
            (s64)amb.numerus));
        json_objectum_ponere(x, "infima", _chorda_json(piscina,
            _nota(k, amb.infima, amb.pulsus_infimae, piscina)));
        json_objectum_ponere(x, "summa", _chorda_json(piscina,
            _nota(k, amb.summa, amb.pulsus_summae, piscina)));
        json_objectum_ponere(x, "rms", _numerus_json(piscina,
            (f64)r->partes[i].rms, 1000.0));
        json_objectum_ponere(x, "dbfs", _numerus_json(piscina,
            _gradus_soni(r->partes[i].rms), 10.0));
        json_objectum_ponere(x, "apex", _numerus_json(piscina,
            (f64)r->partes[i].apex, 1000.0));
        json_tabulatum_addere(partes, x);
    }
    json_objectum_ponere(o, "partes", partes);

    json_objectum_ponere(sonus, "apex", _numerus_json(piscina,
        (f64)r->sonus.apex, 1000.0));
    json_objectum_ponere(sonus, "apex_dbfs", _numerus_json(piscina,
        _gradus_soni(r->sonus.apex), 10.0));
    json_objectum_ponere(sonus, "tonsa", json_integer_creare(piscina,
        (s64)r->sonus.tonsa));
    json_objectum_ponere(sonus, "rms", _numerus_json(piscina,
        (f64)r->sonus.rms, 1000.0));
    json_objectum_ponere(sonus, "dbfs", _numerus_json(piscina,
        _gradus_soni(r->sonus.rms), 10.0));
    per (i = ZEPHYRUM; i < r->sonus.numerus; i++)
    {
         JsonValor* x = json_objectum_creare(piscina);
               b32  tacita = t < r->numerus_tacitarum
                   && r->tacitae[t] == i + I;

        si (x == NIHIL)
        {
            redde NIHIL;
        }
        t += tacita ? I : ZEPHYRUM;
        json_objectum_ponere(x, "mensura", json_integer_creare(piscina,
            (s64)(i + I)));
        json_objectum_ponere(x, "rms", _numerus_json(piscina,
            (f64)r->sonus.mensurae[i], 1000.0));
        json_objectum_ponere(x, "dbfs", _numerus_json(piscina,
            _gradus_soni(r->sonus.mensurae[i]), 10.0));
        json_objectum_ponere(x, "tacita", json_boolean_creare(piscina,
            tacita));
        json_tabulatum_addere(mensurae, x);
    }
    json_objectum_ponere(sonus, "mensurae", mensurae);
    json_objectum_ponere(o, "sonus", sonus);

    per (i = ZEPHYRUM; i < (i32)CENSURA_REGULAE; i++)
    {
        si (r->forma.censura.activae[i])
        {
            json_tabulatum_addere(regulae, json_chorda_creare_literis(
                piscina, censura_titulus((CensuraRegula)i)));
        }
    }
    json_objectum_ponere(censura, "regulae", regulae);
    per (i = ZEPHYRUM; i < r->numerus_inventorum; i++)
    {
        JsonValor* x = _inventum_json(k, &r->inventa[i], piscina);

        si (x == NIHIL)
        {
            redde NIHIL;
        }
        json_tabulatum_addere(inventa, x);
    }
    json_objectum_ponere(censura, "inventa", inventa);
    json_objectum_ponere(o, "censura", censura);
    si (r->partitura.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(o, "partitura", _chorda_json(piscina,
            r->partitura));
    }
    redde o;
}
