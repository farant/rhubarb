/* internuntius.c - implementatio (vide internuntius.h; spec
 * project-specs/vitrea-spec-v2.md par VI). */

#include "internuntius.h"
#include "tabula_dispersa.h"
#include <string.h>
#include <stdio.h>

/* praebitum unum registri */
nomen structura {
    InternuntiusTractator  tractator;
                   vacuum* datum;
} InternuntiusPraebitum;

structura Internuntius {
                Piscina* piscina;
         TabulaDispersa* methodi_tabula;   /* titulus -> Praebitum* */
                    Xar* methodi_ordo;     /* chorda (valore) */
     InternuntiusMissor  mittere;
                 vacuum* mittere_datum;
    InternuntiusFructus  fructus;
};


/* ==================================================
 * auxilia
 * ================================================== */

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

/* copia chordae in piscinam (datum novum, non NUL-terminatum) */
interior chorda
_copiare (
    Piscina* piscina,
     chorda  c)
{
    chorda nova;

    nova.mensura  = c.mensura;
    nova.datum    = NIHIL;
    si (c.mensura > ZEPHYRUM)
    {
        nova.datum = (i8*)piscina_allocare(piscina,
            (memoriae_index)c.mensura);
        si (nova.datum == NIHIL)
        {
            nova.mensura = ZEPHYRUM;
            redde nova;
        }
        memcpy(nova.datum, c.datum, (memoriae_index)c.mensura);
    }
    redde nova;
}

interior vacuum
_emittere (
    Internuntius* inx,
       JsonValor* responsum,
         Piscina* pv)
{
    chorda textus = json_scribere(responsum, pv);

    si (inx->mittere != NIHIL)
    {
        inx->mittere(inx->mittere_datum, textus);
    }
}

/* responsum culpae construere et mittere (id solum si habetur) */
interior vacuum
_culpam_mittere (
    Internuntius* inx,
             b32  habet_id,
             s64  id,
          chorda  nuntius,
         Piscina* pv)
{
    JsonValor* responsum  = json_objectum_creare(pv);
    JsonValor* culpa      = json_objectum_creare(pv);

    si (habet_id)
    {
        json_objectum_ponere(responsum, "id",
            json_integer_creare(pv, id));
    }
    json_objectum_ponere(culpa, "nuntius",
        json_chorda_creare(pv, nuntius));
    json_objectum_ponere(responsum, "culpa", culpa);
    inx->fructus.culpae++;
    _emittere(inx, responsum, pv);
}


/* ==================================================
 * vita
 * ================================================== */

Internuntius*
internuntius_creare (
               Piscina* piscina,
    InternuntiusMissor  mittere,
                vacuum* mittere_datum)
{
    Internuntius* inx;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    inx = (Internuntius*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(Internuntius),
        (memoriae_index)magnitudo(vacuum*));
    si (inx == NIHIL)
    {
        redde NIHIL;
    }
    memset(inx, ZEPHYRUM, magnitudo(Internuntius));
    inx->piscina = piscina;
    inx->methodi_tabula = tabula_dispersa_creare_chorda(piscina,
        CXXVIII);
    inx->methodi_ordo = xar_creare(piscina,
        (i32)magnitudo(chorda));
    si (inx->methodi_tabula == NIHIL || inx->methodi_ordo == NIHIL)
    {
        redde NIHIL;
    }
    inx->mittere        = mittere;
    inx->mittere_datum  = mittere_datum;
    redde inx;
}

b32
internuntius_praebere (
             Internuntius* inx,
       constans character* methodus,
    InternuntiusTractator  tractator,
                   vacuum* datum)
{
                   chorda  titulus;
    InternuntiusPraebitum* praebitum;

    si (inx == NIHIL || methodus == NIHIL || tractator == NIHIL)
    {
        redde FALSUM;
    }
    titulus = _ch(methodus);
    si (   titulus.mensura == ZEPHYRUM
        || tabula_dispersa_continet(inx->methodi_tabula, titulus))
    {
        redde FALSUM;   /* vacuum aut duplicatum - registrum codex */
    }
    titulus = _copiare(inx->piscina, titulus);
    praebitum = (InternuntiusPraebitum*)piscina_allocare_ordinatum(
        inx->piscina,
        (memoriae_index)magnitudo(InternuntiusPraebitum),
        (memoriae_index)magnitudo(vacuum*));
    si (   praebitum == NIHIL
        || (titulus.mensura == ZEPHYRUM && methodus[0] != '\0'))
    {
        redde FALSUM;
    }
    praebitum->tractator  = tractator;
    praebitum->datum      = datum;
    si (!tabula_dispersa_inserere(inx->methodi_tabula, titulus,
            praebitum))
    {
        redde FALSUM;
    }
    {
        chorda* locus = (chorda*)xar_addere(inx->methodi_ordo);

        si (locus != NIHIL)
        {
            *locus = titulus;
        }
    }
    redde VERUM;
}


/* ==================================================
 * dispatch
 * ================================================== */

vacuum
internuntius_tractare (
    Internuntius* inx,
          chorda  nuntium,
         Piscina* pv)
{
             JsonResultus  r;
                JsonValor* id_v;
                JsonValor* methodus_v;
                JsonValor* argumenta_v;
                      b32  habet_id  = FALSUM;
                      s64  id        = ZEPHYRUM;
                   chorda  methodus;
                   vacuum* valor = NIHIL;
    InternuntiusPraebitum* praebitum;

    si (inx == NIHIL || pv == NIHIL)
    {
        redde;
    }
    r = json_legere(nuntium, pv);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        /* sine id recuperabili - culpa orba (JS orbas queritur) */
        _culpam_mittere(inx, FALSUM, ZEPHYRUM,
            _ch("nuntium malformatum (json fractum aut non"
                " obiectum)"), pv);
        redde;
    }
    id_v = json_objectum_capere(r.radix, "id");
    si (id_v != NIHIL && json_est_integer(id_v))
    {
        habet_id  = VERUM;
        id        = json_ad_integer(id_v);
    }
    methodus_v = json_objectum_capere(r.radix, "methodus");
    si (methodus_v == NIHIL || !json_est_chorda(methodus_v))
    {
        _culpam_mittere(inx, habet_id, id,
            _ch("methodus deest"), pv);
        redde;
    }
    methodus = json_ad_chorda(methodus_v);
    si (!tabula_dispersa_invenire(inx->methodi_tabula, methodus,
            &valor))
    {
        ChordaAedificator* aed = chorda_aedificator_creare(pv,
            LXIV);

        chorda_aedificator_appendere_literis(aed,
            "methodus ignota: ");
        chorda_aedificator_appendere_chorda(aed, methodus);
        _culpam_mittere(inx, habet_id, id,
            chorda_aedificator_finire(aed), pv);
        redde;
    }
    praebitum    = (InternuntiusPraebitum*)valor;
    argumenta_v  = json_objectum_capere(r.radix, "argumenta");
    {
           chorda  culpa;
        JsonValor* fructus_v;
        JsonValor* responsum;

        culpa.mensura  = ZEPHYRUM;
        culpa.datum    = NIHIL;
        fructus_v = praebitum->tractator(argumenta_v, pv,
            praebitum->datum, &culpa);
        si (culpa.mensura > ZEPHYRUM)
        {
            _culpam_mittere(inx, habet_id, id, culpa, pv);
            redde;
        }
        responsum = json_objectum_creare(pv);
        si (habet_id)
        {
            json_objectum_ponere(responsum, "id",
                json_integer_creare(pv, id));
        }
        json_objectum_ponere(responsum, "fructus",
            fructus_v != NIHIL ? fructus_v
                : json_nullum_creare(pv));
        inx->fructus.vocationes_tractatae++;
        _emittere(inx, responsum, pv);
    }
}

vacuum
internuntius_eventum_mittere (
          Internuntius* inx,
    constans character* eventus,
             JsonValor* datum,
               Piscina* pv)
{
    JsonValor* responsum;

    si (inx == NIHIL || eventus == NIHIL || pv == NIHIL)
    {
        redde;
    }
    responsum = json_objectum_creare(pv);
    json_objectum_ponere(responsum, "eventus",
        json_chorda_creare_literis(pv, eventus));
    json_objectum_ponere(responsum, "datum",
        datum != NIHIL ? datum : json_nullum_creare(pv));
    inx->fructus.eventus_missi++;
    _emittere(inx, responsum, pv);
}


/* ==================================================
 * introspectio + fructus
 * ================================================== */

Xar*
internuntius_methodi (
    Internuntius* inx,
         Piscina* piscina)
{
    Xar* index;
    i32  i;

    si (inx == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    index = xar_creare(piscina, (i32)magnitudo(chorda));
    si (index == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(inx->methodi_ordo); i++)
    {
        chorda* fons   = (chorda*)xar_obtinere(inx->methodi_ordo, i);
        chorda* locus  = (chorda*)xar_addere(index);

        si (fons != NIHIL && locus != NIHIL)
        {
            *locus = *fons;
        }
    }
    redde index;
}

InternuntiusFructus
internuntius_fructus (
    constans Internuntius* inx)
{
    InternuntiusFructus vacua;

    si (inx == NIHIL)
    {
        memset(&vacua, ZEPHYRUM, magnitudo(vacua));
        redde vacua;
    }
    redde inx->fructus;
}


/* ==================================================
 * effugator litterae JS
 * ================================================== */

/* cursum planum [initium, finis) appendere */
interior vacuum
_cursum_appendere (
    ChordaAedificator* aed,
               chorda  textus,
                  i32  initium,
                  i32  finis)
{
    chorda pars;

    si (finis <= initium)
    {
        redde;
    }
    pars.datum    = textus.datum + initium;
    pars.mensura  = finis - initium;
    chorda_aedificator_appendere_chorda(aed, pars);
}

chorda
internuntius_effugere_js (
     chorda  textus,
    Piscina* piscina)
{
    ChordaAedificator* aed;
                  i32  i               = ZEPHYRUM;
                  i32  initium_cursus  = ZEPHYRUM;
               chorda  vacua;

    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;
    si (piscina == NIHIL)
    {
        redde vacua;
    }
    aed = chorda_aedificator_creare(piscina,
        textus.mensura + XVI);
    si (aed == NIHIL)
    {
        redde vacua;
    }
    dum (i < textus.mensura)
    {
        insignatus character o =
            (insignatus character)textus.datum[i];
        constans character* effugium = NIHIL;
                 character  quattuor[VIII];
                       i32  saltus = I;

        si (o == (insignatus character)'\\')
        {
            effugium = "\\\\";
        }
        alioquin si (o == (insignatus character)'"')
        {
            effugium = "\\\"";
        }
        alioquin si (o == (insignatus character)'\n')
        {
            effugium = "\\n";
        }
        alioquin si (o == (insignatus character)'\r')
        {
            effugium = "\\r";
        }
        alioquin si (o == (insignatus character)'\t')
        {
            effugium = "\\t";
        }
        alioquin si (o < 0x20)
        {
            sprintf(quattuor, "\\u%04X", (insignatus)o);
            effugium = quattuor;
        }
        alioquin si (o == (insignatus character)'<')
        {
            /* contra '</script' formas intra HTML intextum */
            effugium = "\\u003C";
        }
        alioquin si (   o == 0xE2
                     && i + II < textus.mensura
                     && (insignatus character)textus.datum[i + I] == 0x80
                     && ((insignatus character)textus.datum[i + II] == 0xA8
                     || (insignatus character)textus.datum[i + II]
                     == 0xA9))
        {
            /* U+2028/U+2029: in JSON licita, in littera JS cruda
             * illicita - classicum quod homines praetermittunt */
            effugium = ((insignatus character)textus.datum[i + II]
                == 0xA8) ? "\\u2028" : "\\u2029";
            saltus = III;
        }
        si (effugium != NIHIL)
        {
            _cursum_appendere(aed, textus, initium_cursus, i);
            chorda_aedificator_appendere_literis(aed, effugium);
            i               += saltus;
            initium_cursus  = i;
        }
        alioquin
        {
            i++;
        }
    }
    _cursum_appendere(aed, textus, initium_cursus, i);
    redde chorda_aedificator_finire(aed);
}
