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

hic_manens constans character* radix_viae;

interior chorda
_legere (
               Piscina* piscina,
    constans character* relativa)
{
    character via[DXII];

    sprintf(via, "%s/%s", radix_viae, relativa);
    redde filum_legere_totum(via, piscina);
}

/* Documentum iudicare; vitia imprimuntur ut defectus legi possit */
interior Xar*
_iudicare (
                  Canon* canon,
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

s32 principale (vacuum)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
                   Canon* canon;
                  chorda  fons;
                  chorda  causa;
                     b32  praeteritus;
                     i32  i;

    piscina = piscina_generare_dynamicum(
        "probatio_crusta_effectus_dialectus", (memoriae_index)M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern      = internamentum_creare(piscina);
    radix_viae  = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }

    /* I. canon legitur */
    imprimere("\n--- I. effectus.canon ---\n");
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
        credo_imprimere_compendium();
        credo_claudere();
        redde I;
    }

    /* II. fixa sana */
    imprimere("\n--- II. fixa contra canonem ---\n");
    per (i = ZEPHYRUM; FIXA[i] != NIHIL; i++)
    {
        Xar* vitia;

        imprimere("  %s\n", FIXA[i]);
        fons = _legere(piscina, FIXA[i]);
        CREDO_MAIOR_I32 (fons.mensura, ZEPHYRUM);
        vitia = _iudicare(canon, fons, piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_S32 (vitia ? (s32)xar_numerus(vitia) : -I,
            ZEPHYRUM);
    }

    /* III. omnia.stml vocabularium totum tegit */
    imprimere("\n--- III. omnia.stml tegit ---\n");
    {
        StmlResultus res;

        res = stml_legere(_legere(piscina,
            "crusta/probationes/fixa/effectus/omnia.stml"),
            piscina, intern);
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

    /* IV. contraria: canon recusat, vitio nominato */
    imprimere("\n--- IV. contraria ---\n");
    {
        Xar* vitia;

        imprimere("  classis aliena:\n");
        vitia = _iudicare(canon, chorda_ex_literis(ALIENUM, piscina),
            piscina, intern);
        CREDO_VERUM (_vitium_unum(vitia, CANON_VALOR_MALUS));

        imprimere("  situs sine sedibus:\n");
        vitia = _iudicare(canon,
            chorda_ex_literis(SINE_SEDIBUS, piscina), piscina, intern);
        CREDO_VERUM (_vitium_unum(vitia, CANON_ATTRIBUTUM_DEEST));
    }

    /* V. registrum: dialectus per elementum radicis */
    imprimere("\n--- V. canones.registrum ---\n");
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

    /* VI. tabula mandatorum (planum T2): canon suus, registrum per
     * radicem <mandata>, deinde quae canon dicere nequit - titulus
     * unicus, ordo omnis significans, purum et ignotum exclusiva */
    imprimere("\n--- VI. tabula mandatorum ---\n");
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
            "crusta/effectus_mandata.canon"), piscina, intern, &causa);
        CREDO_NON_NIHIL (canon_tabulae);
        si (canon_tabulae != NIHIL)
        {
            fons = _legere(piscina, "crusta/effectus_mandata.stml");
            CREDO_MAIOR_I32 (fons.mensura, ZEPHYRUM);
            vitia = _iudicare(canon_tabulae, fons, piscina, intern);
            CREDO_AEQUALIS_S32 (vitia ? (s32)xar_numerus(vitia) : -I,
                ZEPHYRUM);

            res      = stml_legere(fons, piscina, intern);
            ordines  = _elementa_numerare(res.radix, "mandatum");
            imprimere("  ordines: %u\n", (insignatus integer)ordines);
            CREDO_MAIOR_I32 (ordines, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (_tabulae_vitia(piscina, res.radix),
                ZEPHYRUM);

            imprimere("  contrarium: munus alienum\n");
            vitia = _iudicare(canon_tabulae,
                chorda_ex_literis(TABULA_ALIENA, piscina), piscina,
                intern);
            CREDO_VERUM (_vitium_unum(vitia, CANON_VALOR_MALUS));

            imprimere("  contrarium: titulus bis, ordo vacuus\n");
            res = stml_legere(chorda_ex_literis(TABULA_PRAVA, piscina),
                piscina, intern);
            CREDO_VERUM (res.successus);
            CREDO_AEQUALIS_I32 (_tabulae_vitia(piscina, res.radix), II);
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
