/* probatio_crusta_effectus_dialectus.c - dialectus 'effectus' et
 * canon eius (effectus-plan.md T1)
 *
 * Contractus ANTE emissorem: summaria manu scripta
 * (probationes/fixa/effectus/) contra effectus.canon iudicantur, et
 * dialectus per elementum radicis in canones.registrum invenitur.
 * Quaeque assertio CONTRARIUM quoque fert: documentum cum valore
 * electionis alieno et situs sine sedibus VITIUM NOMINATUM dare
 * debent - canon qui omnia accipit mutus est, non sanus.
 * omnia.stml elementum omne et valorem omnem electionum principalium
 * continere debet (custos: fixum quod vocabularium non tegit
 * contractum non probat).
 */

#include "latina.h"
#include "credo.h"
#include "canon.h"
#include "chorda.h"
#include "filum.h"
#include "internamentum.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FIXURA SUITAE: internamentum et effectus.canon semel lecta;
 * sectiones LEGUNT (canon per monstratorem constantem; internatio in
 * internamentum fixurae sola licet). Lectio canonis in sectione I
 * ASSERITUR (instantia sua); fixura sine assertione - canon NIHIL =
 * sectiones quae eo egent VACUAE, suita FRACTA. */
nomen structura {
    InternamentumChorda* intern;
                  Canon* canon;
} FixuraCanonis;

interior constans character*
_radix_viae (vacuum)
{
    constans character* radix_viae = getenv("RHUBARB_RADIX");

    redde radix_viae != NIHIL ? radix_viae : ".";
}

interior chorda
_legere (
               Piscina* piscina,
    constans character* relativa)
{
    character via[DXII];

    sprintf(via, "%s/%s", _radix_viae(), relativa);
    redde filum_legere_totum(via, piscina);
}

/* Documentum iudicare; vitia imprimuntur ut defectus legi possit */
interior Xar*
_iudicare (
         constans Canon* canon,
                 chorda  fons,
               Piscina*  piscina,
    InternamentumChorda* intern)
{
    StmlResultus  res;
             Xar* vitia;
             i32  i;

    res = stml_legere(fons, piscina, intern);
    CREDO_VERUM (res.successus);
    si (!res.successus)
    {
        redde NIHIL;
    }
    vitia = canon_iudicare(canon, res.radix, piscina);
    per (i = ZEPHYRUM; vitia && i < xar_numerus(vitia); i++)
    {
        CanonVitium* v;

        v = (CanonVitium*)xar_obtinere(vitia, i);
        imprimere("    vitium: %s <%.*s>\n", canon_nuntius(v->genus),
            v->elementum ? (integer)v->elementum->mensura : 0,
            v->elementum ? (constans character*)v->elementum->datum
                         : "");
    }
    redde vitia;
}

/* Elementa nominata in subarbore numerare */
interior i32
_elementa_numerare (
               StmlNodus* nodus,
      constans character* titulus)
{
    i32 summa;
    i32 i;

    summa = ZEPHYRUM;
    si (nodus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (   nodus->genus   == STML_NODUS_ELEMENTUM
        && nodus->titulus != NIHIL
        && chorda_aequalis_literis(*nodus->titulus, titulus))
    {
        summa++;
    }
    per (i = ZEPHYRUM; nodus->liberi && i < xar_numerus(nodus->liberi);
         i++)
    {
        summa += _elementa_numerare(
            *(StmlNodus**)xar_obtinere(nodus->liberi, i), titulus);
    }
    redde summa;
}

/* Valorem attributi alicubi in subarbore occurrere */
interior b32
_valor_adest (
               StmlNodus* nodus,
      constans character* attributum,
      constans character* valor)
{
     chorda* v;
        i32  i;

    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    si (nodus->genus == STML_NODUS_ELEMENTUM)
    {
        v = stml_attributum_capere(nodus, attributum);
        si (v != NIHIL && chorda_aequalis_literis(*v, valor))
        {
            redde VERUM;
        }
    }
    per (i = ZEPHYRUM; nodus->liberi && i < xar_numerus(nodus->liberi);
         i++)
    {
        si (_valor_adest(*(StmlNodus**)xar_obtinere(nodus->liberi, i),
                attributum, valor))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Vitium unum generis dati exacte */
interior b32
_vitium_unum (
                 Xar* vitia,
    CanonVitiumGenus  genus)
{
    CanonVitium* v;

    si (vitia == NIHIL || xar_numerus(vitia) != I)
    {
        redde FALSUM;
    }
    v = (CanonVitium*)xar_obtinere(vitia, ZEPHYRUM);
    redde v->genus == genus;
}

hic_manens constans character* FIXA[] = {
    "crusta/probationes/fixa/effectus/catena.stml",
    "crusta/probationes/fixa/effectus/cursor.stml",
    "crusta/probationes/fixa/effectus/omnia.stml",
    NIHIL
};

hic_manens constans character* ELEMENTA_SITUUM[] = {
    "lectio", "scriptura", "exsecutio", "fontatio", "enumeratio",
    "probatio", "ambitus_lectio", "ignotum", NIHIL
};

/* attributum, valor - electiones quas omnia.stml tegere debet */
hic_manens constans character* ELECTIONES[] = {
    "forma", "via",         "forma", "globus",
    "forma", "praefixum",
    "resolutio", "plena",   "resolutio", "partialis",
    "resolutio", "nulla",
    "classis", "arbor",     "classis", "build",
    "classis", "instrumentum_domus",
    "classis", "externa",   "classis", "systema",
    "per", "redirectio",    "per", "aedificium",
    "per", "mandatum",      "per", "observatum",
    "scripta_in_ambitu", "verum", "scripta_in_ambitu", "falsum",
    "assignatum", "verum",  "assignatum", "falsum",
    NIHIL
};

hic_manens constans character* ALIENUM =
    "<effectus lingua=\"bash\" radix=\"x.sh\">\n"
    "  <processus radix=\"x.sh\">\n"
    "    <lectio via=\"a\" forma=\"via\" resolutio=\"plena\"\n"
    "            classis=\"aliena\" per=\"redirectio\"\n"
    "            plagula=\"x.sh\" sedes=\"1:1-1:4\" octeti=\"0-3\"/>\n"
    "  </processus>\n"
    "</effectus>\n";

hic_manens constans character* SINE_SEDIBUS =
    "<effectus lingua=\"bash\" radix=\"x.sh\">\n"
    "  <processus radix=\"x.sh\">\n"
    "    <probatio via=\"a\" forma=\"via\" resolutio=\"plena\"\n"
    "              per=\"aedificium\" plagula=\"x.sh\"\n"
    "              octeti=\"0-3\"/>\n"
    "  </processus>\n"
    "</effectus>\n";

hic_manens constans character* TABULA_ALIENA =
    "<mandata lingua=\"bash\">\n"
    "  <mandatum titulus=\"cat\" argumenta=\"legere\"/>\n"
    "</mandata>\n";

/* duo vitia exacte: 'cat' bis, 'wc' sine munere ullo */
hic_manens constans character* TABULA_PRAVA =
    "<mandata lingua=\"bash\">\n"
    "  <mandatum titulus=\"cat\" argumenta=\"lectio\"/>\n"
    "  <mandatum titulus=\"cat\" purum=\"verum\"/>\n"
    "  <mandatum titulus=\"wc\"/>\n"
    "</mandata>\n";

hic_manens constans character* MUNERA[] = {
    "argumenta", "primum", "ultimum", "optio_lectio", "optio_scriptura",
    NIHIL
};

/* Quae canon dicere nequit: titulus unicus; ordo quisque aut purus,
 * aut ignotus (cum causa), aut munus unum saltem fert - nec purus et
 * munera simul. Vitia imprimuntur et numerantur. */
interior i32
_tabulae_vitia (
      Piscina* piscina,
    StmlNodus* radix)
{
    Xar* visa   = xar_creare(piscina, (i32)magnitudo(chorda*));
    i32  vitia  = ZEPHYRUM;
    i32  i;
    i32  j;
    i32  k;

    per (i = ZEPHYRUM; radix && radix->liberi
                       && i < xar_numerus(radix->liberi); i++)
    {
        StmlNodus* m = *(StmlNodus**)xar_obtinere(radix->liberi, i);

        si (   m->genus == STML_NODUS_ELEMENTUM && m->titulus != NIHIL
            && chorda_aequalis_literis(*m->titulus, "mandata"))
        {
            per (j = ZEPHYRUM; m->liberi && j < xar_numerus(m->liberi);
                 j++)
            {
                StmlNodus* o = *(StmlNodus**)xar_obtinere(m->liberi, j);
                   chorda* t;
                   chorda* purum;
                   chorda* ignotum;
                       b32 munus = FALSUM;

                si (o->genus != STML_NODUS_ELEMENTUM)
                {
                    perge;
                }
                t        = stml_attributum_capere(o, "titulus");
                purum    = stml_attributum_capere(o, "purum");
                ignotum  = stml_attributum_capere(o, "ignotum");
                per (k = ZEPHYRUM; t && k < xar_numerus(visa); k++)
                {
                    si (chorda_aequalis(*t,
                            **(chorda**)xar_obtinere(visa, k)))
                    {
                        imprimere("    titulus bis: %.*s\n",
                            (integer)t->mensura,
                            (constans character*)t->datum);
                        vitia++;
                    }
                }
                si (t)
                {
                    *(chorda**)xar_addere(visa) = t;
                }
                per (k = ZEPHYRUM; MUNERA[k] != NIHIL; k++)
                {
                    si (stml_attributum_capere(o, MUNERA[k]) != NIHIL)
                    {
                        munus = VERUM;
                    }
                }
                si (purum && (munus || ignotum))
                {
                    imprimere("    purum cum munere aut ignoto\n");
                    vitia++;
                }
                alioquin si (   ignotum
                             && stml_attributum_capere(o, "causa")
                                 == NIHIL)
                {
                    imprimere("    ignotum sine causa\n");
                    vitia++;
                }
                alioquin si (!purum && !ignotum && !munus)
                {
                    imprimere("    ordo sine munere: %.*s\n",
                        t ? (integer)t->mensura : 0,
                        t ? (constans character*)t->datum : "");
                    vitia++;
                }
            }
        }
    }
    redde vitia;
}

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    FixuraCanonis* fx;
           chorda  causa;

    fx = (FixuraCanonis*)piscina_allocare(c->piscina,
        magnitudo(FixuraCanonis));
    si (fx == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    fx->intern     = internamentum_creare(c->piscina);
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    fx->canon      = canon_legere(_legere(c->piscina, "effectus.canon"),
        c->piscina, fx->intern, &causa);
    c->datum = fx;
}

/* I. canon legitur */
interior vacuum
_canonem_probare (
    CredoContextus* c)
{
                Piscina* piscina  = c->piscina;
    InternamentumChorda* intern   = internamentum_creare(c->piscina);
                  Canon* canon;
                 chorda  fons;
                 chorda  causa;

    fons = _legere(piscina, "effectus.canon");
    CREDO_MAIOR_I32 (fons.mensura, ZEPHYRUM);
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    canon          = canon_legere(fons, piscina, intern, &causa);
    CREDO_NON_NIHIL (canon);
    si (canon == NIHIL)
    {
        imprimere("  CAUSA: %.*s\n", (integer)causa.mensura,
            causa.datum ? (constans character*)causa.datum : "");
        redde;
    }
}

/* II. fixa sana */
interior vacuum
_fixa_probare (
    CredoContextus* c)
{
                   Piscina* piscina = c->piscina;
    constans FixuraCanonis* fx = (constans FixuraCanonis*)c->suita;
                    chorda  fons;
                       i32  i;

    si (fx == NIHIL || fx->canon == NIHIL)
    {
        redde;   /* canon absens: sectio I eum nominat */
    }
    per (i = ZEPHYRUM; FIXA[i] != NIHIL; i++)
    {
        Xar* vitia;

        imprimere("  %s\n", FIXA[i]);
        fons = _legere(piscina, FIXA[i]);
        CREDO_MAIOR_I32 (fons.mensura, ZEPHYRUM);
        vitia = _iudicare(fx->canon, fons, piscina, fx->intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_S32 (vitia ? (s32)xar_numerus(vitia) : -I,
            ZEPHYRUM);
    }
}

/* III. omnia.stml vocabularium totum tegit */
interior vacuum
_omnia_probare (
    CredoContextus* c)
{
                   Piscina* piscina = c->piscina;
    constans FixuraCanonis* fx = (constans FixuraCanonis*)c->suita;
                       i32  i;

    si (fx == NIHIL)
    {
        redde;
    }
    {
        StmlResultus res;

        res = stml_legere(_legere(piscina,
            "crusta/probationes/fixa/effectus/omnia.stml"),
            piscina, fx->intern);
        CREDO_VERUM (res.successus);
        per (i = ZEPHYRUM; ELEMENTA_SITUUM[i] != NIHIL; i++)
        {
            i32 quot;

            quot = _elementa_numerare(res.radix, ELEMENTA_SITUUM[i]);
            si (quot == ZEPHYRUM)
            {
                imprimere("  elementum absens: %s\n",
                    ELEMENTA_SITUUM[i]);
            }
            CREDO_MAIOR_I32 (quot, ZEPHYRUM);
        }
        per (i = ZEPHYRUM; ELECTIONES[i] != NIHIL; i += II)
        {
            b32 adest;

            adest = _valor_adest(res.radix, ELECTIONES[i],
                ELECTIONES[i + I]);
            si (!adest)
            {
                imprimere("  valor absens: %s=%s\n", ELECTIONES[i],
                    ELECTIONES[i + I]);
            }
            CREDO_VERUM (adest);
        }
    }
}

/* IV. contraria: canon recusat, vitio nominato */
interior vacuum
_contraria_probare (
    CredoContextus* c)
{
                   Piscina* piscina = c->piscina;
    constans FixuraCanonis* fx = (constans FixuraCanonis*)c->suita;

    si (fx == NIHIL || fx->canon == NIHIL)
    {
        redde;   /* canon absens: sectio I eum nominat */
    }
    {
        Xar* vitia;

        imprimere("  classis aliena:\n");
        vitia = _iudicare(fx->canon,
            chorda_ex_literis(ALIENUM, piscina), piscina, fx->intern);
        CREDO_VERUM (_vitium_unum(vitia, CANON_VALOR_MALUS));

        imprimere("  situs sine sedibus:\n");
        vitia = _iudicare(fx->canon,
            chorda_ex_literis(SINE_SEDIBUS, piscina), piscina,
            fx->intern);
        CREDO_VERUM (_vitium_unum(vitia, CANON_ATTRIBUTUM_DEEST));
    }
}

/* V. registrum: dialectus per elementum radicis */
interior vacuum
_registrum_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
        chorda catalogus;
        chorda titulus;
        chorda via_canonis;

        catalogus = _legere(piscina, "canones.registrum");
        CREDO_MAIOR_I32 (catalogus.mensura, ZEPHYRUM);
        titulus = chorda_ex_literis("effectus", piscina);
        via_canonis = canon_registrum_quaerere_radice(catalogus,
            &titulus, piscina);
        imprimere("  <effectus> -> '%.*s'\n",
            (integer)via_canonis.mensura,
            via_canonis.datum ? (constans character*)via_canonis.datum
                              : "");
        CREDO_VERUM (chorda_aequalis_literis(via_canonis,
            "effectus.canon"));
    }
}

/* VI. tabula mandatorum (planum T2): canon suus, registrum per
 * radicem <mandata>, deinde quae canon dicere nequit - titulus
 * unicus, ordo omnis significans, purum et ignotum exclusiva */
interior vacuum
_tabulam_probare (
    CredoContextus* c)
{
                   Piscina* piscina = c->piscina;
    constans FixuraCanonis* fx = (constans FixuraCanonis*)c->suita;
                    chorda  fons;
                    chorda  causa;

    si (fx == NIHIL)
    {
        redde;
    }
    causa.datum    = NIHIL;   /* ut olim: causa ex sectione I vacua */
    causa.mensura  = ZEPHYRUM;
    {
                  Canon* canon_tabulae;
                 chorda  catalogus;
                 chorda  titulus;
                 chorda  via_canonis;
           StmlResultus  res;
                    Xar* vitia;
                    i32  ordines;

        catalogus  = _legere(piscina, "canones.registrum");
        titulus    = chorda_ex_literis("mandata", piscina);
        via_canonis = canon_registrum_quaerere_radice(catalogus,
            &titulus, piscina);
        CREDO_VERUM (chorda_aequalis_literis(via_canonis,
            "crusta/effectus_mandata.canon"));
        canon_tabulae = canon_legere(_legere(piscina,
            "crusta/effectus_mandata.canon"), piscina, fx->intern,
            &causa);
        CREDO_NON_NIHIL (canon_tabulae);
        si (canon_tabulae != NIHIL)
        {
            fons = _legere(piscina, "crusta/effectus_mandata.stml");
            CREDO_MAIOR_I32 (fons.mensura, ZEPHYRUM);
            vitia = _iudicare(canon_tabulae, fons, piscina, fx->intern);
            CREDO_AEQUALIS_S32 (vitia ? (s32)xar_numerus(vitia) : -I,
                ZEPHYRUM);

            res      = stml_legere(fons, piscina, fx->intern);
            ordines  = _elementa_numerare(res.radix, "mandatum");
            imprimere("  ordines: %u\n", (insignatus integer)ordines);
            CREDO_MAIOR_I32 (ordines, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (_tabulae_vitia(piscina, res.radix),
                ZEPHYRUM);

            imprimere("  contrarium: munus alienum\n");
            vitia = _iudicare(canon_tabulae,
                chorda_ex_literis(TABULA_ALIENA, piscina), piscina,
                fx->intern);
            CREDO_VERUM (_vitium_unum(vitia, CANON_VALOR_MALUS));

            imprimere("  contrarium: titulus bis, ordo vacuus\n");
            res = stml_legere(chorda_ex_literis(TABULA_PRAVA, piscina),
                piscina, fx->intern);
            CREDO_VERUM (res.successus);
            CREDO_AEQUALIS_I32 (_tabulae_vitia(piscina, res.radix), II);
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. effectus.canon",
        _canonem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "II. fixa contra canonem",
        _fixa_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "III. omnia.stml tegit",
        _omnia_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "IV. contraria", _contraria_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "V. canones.registrum",
        _registrum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "VI. tabula mandatorum",
        _tabulam_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("crusta_effectus_dialectus",
        SECTIONES, _suitam_parare, NIHIL);
}
