/* probatio_pictor_strata.c - strata in documento pictoris (L2)
 *
 * Documentum XL x XXX, intervallum IV (checkpoints crebri). I: stratum
 * unum (id I) visibile, compositum = fundus. II: stratum novum supra
 * I; ictus in strato suo; perspicua strata inferiora ostendunt. III:
 * occultare (et revocare / reficere). IV: ordo. V: spongia stratum
 * suum solum purgat (perspicuum). VI: delere; ultimum numquam. VII:
 * XVI maxima. VIII: reapertio per checkpoint codificatum - sigillum,
 * pixela, strata eadem; verificare. IX: checkpoint VETUS (massa cruda)
 * recusatur, reproiectio - compositum rectum. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "color.h"
#include "thema.h"
#include "volumen.h"
#include "sigillum.h"
#include "xar.h"
#include "pictor_documentum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior b32
actum (
    PictorDocumentum* doc,
  constans character* stml)
{
    redde pictor_documentum_actum(doc, chorda_ex_literis(stml, piscina))
        > ZEPHYRUM;
}

interior i32
pixelum (
    PictorDocumentum* doc,
                 s32  x,
                 s32  y)
{
    redde (i32)tabula_pixelorum_obtinere_pixelum(doc->tabula, (i32)x,
        (i32)y);
}

interior i32
palettae (
    s32 index)
{
    redde (i32)color_ad_pixelum(color_ex_palette((i32)index));
}

interior s32
id_strati (
    PictorDocumentum* doc,
                 i32  index)
{
    constans PictorStratum* s;

    s = pictor_documentum_stratum(doc, index);
    redde s ? s->id : -I;
}

interior b32
photographia_eadem (
    PictorDocumentum* a,
    PictorDocumentum* b)
{
    redde memcmp(a->tabula->pixela, b->tabula->pixela,
        (size_t)(a->latitudo * a->altitudo) * magnitudo(i32))
        == ZEPHYRUM;
}

s32 principale (vacuum)
{
             Volumen* vol;
    PictorDocumentum* doc;
                 i32  fundus;
                 i32  c0;
                 i32  c3;

    piscina = piscina_generare_dynamicum("probatio_pictor_strata",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_strata");
    doc = pictor_documentum_creare(piscina, intern, vol, "", XL, XXX,
        IV);
    CREDO_NON_NIHIL(doc);
    si (!doc)
    {
        credo_imprimere_compendium();
        redde I;
    }
    fundus  = (i32)color_ad_pixelum(thema_color(COLOR_BACKGROUND));
    c0      = palettae(ZEPHYRUM);
    c3      = palettae(III);

    imprimere("\n--- I: stratum unum, compositum = fundus ---\n");
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
    CREDO_AEQUALIS_S32(id_strati(doc, ZEPHYRUM), I);
    CREDO_VERUM(pictor_documentum_stratum(doc, ZEPHYRUM)
        && pictor_documentum_stratum(doc, ZEPHYRUM)->visibile);
    CREDO_AEQUALIS_I32(pixelum(doc, X, X), fundus);

    imprimere("\n--- II: stratum novum, perspicuitas ---\n");
    CREDO_VERUM(actum(doc,
        "<ictus instrumentum=\"penicillus\" color=\"0\""
        " magnitudo=\"1\"><punctum x=\"0\" y=\"2\"/><punctum x=\"39\""
        " y=\"2\"/></ictus>"));
    CREDO_AEQUALIS_I32(pixelum(doc, V, II), c0);
    CREDO_VERUM(actum(doc,
        "<stratum actio=\"novum\" id=\"2\" supra=\"1\"/>"));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), II);
    CREDO_AEQUALIS_S32(id_strati(doc, I), II);
    CREDO_VERUM(actum(doc,
        "<ictus instrumentum=\"penicillus\" color=\"3\""
        " magnitudo=\"1\" stratum=\"2\"><punctum x=\"5\" y=\"0\"/>"
        "<punctum x=\"5\" y=\"29\"/></ictus>"));
    CREDO_AEQUALIS_I32(pixelum(doc, V, II), c3);
    CREDO_AEQUALIS_I32(pixelum(doc, VI, II), c0);
    CREDO_AEQUALIS_I32(pixelum(doc, V, X), c3);
    CREDO_AEQUALIS_I32(pixelum(doc, X, X), fundus);

    imprimere("\n--- III: occultare, revocare, reficere ---\n");
    CREDO_VERUM(actum(doc,
        "<stratum actio=\"visibile\" id=\"2\" valor=\"0\"/>"));
    CREDO_AEQUALIS_I32(pixelum(doc, V, II), c0);
    CREDO_AEQUALIS_I32(pixelum(doc, V, X), fundus);
    CREDO_VERUM(pictor_documentum_revocare(doc));
    CREDO_AEQUALIS_I32(pixelum(doc, V, II), c3);
    CREDO_VERUM(pictor_documentum_reficere(doc));
    CREDO_AEQUALIS_I32(pixelum(doc, V, X), fundus);
    CREDO_VERUM(actum(doc,
        "<stratum actio=\"visibile\" id=\"2\" valor=\"1\"/>"));
    CREDO_AEQUALIS_I32(pixelum(doc, V, X), c3);

    imprimere("\n--- IV: ordo ---\n");
    CREDO_VERUM(actum(doc, "<stratum actio=\"ordo\" ids=\"2 1\"/>"));
    CREDO_AEQUALIS_S32(id_strati(doc, I), I);
    CREDO_AEQUALIS_I32(pixelum(doc, V, II), c0);
    CREDO_AEQUALIS_I32(pixelum(doc, V, X), c3);
    /* ordo non permutatio: ignoratur */
    CREDO_VERUM(actum(doc, "<stratum actio=\"ordo\" ids=\"1 1\"/>"));
    CREDO_AEQUALIS_S32(id_strati(doc, ZEPHYRUM), II);
    CREDO_AEQUALIS_S32(id_strati(doc, I), I);
    CREDO_VERUM(actum(doc, "<stratum actio=\"ordo\" ids=\"1 2\"/>"));
    CREDO_AEQUALIS_I32(pixelum(doc, V, II), c3);

    imprimere("\n--- V: spongia stratum suum solum ---\n");
    /* quadratum XVI centratum (5, 10): y II..XVII in strato 2 */
    CREDO_VERUM(actum(doc, "<ictus instrumentum=\"spongia\""
        " magnitudo=\"1\" stratum=\"2\"><punctum x=\"5\" y=\"10\"/>"
        "</ictus>"));
    CREDO_AEQUALIS_I32(pixelum(doc, V, II), c0);
    CREDO_AEQUALIS_I32(pixelum(doc, V, X), fundus);
    CREDO_AEQUALIS_I32(pixelum(doc, V, XX), c3);
    CREDO_AEQUALIS_I32(pixelum(doc, VI, II), c0);

    imprimere("\n--- VI: delere, ultimum numquam ---\n");
    CREDO_VERUM(actum(doc, "<stratum actio=\"deletum\" id=\"2\"/>"));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
    CREDO_AEQUALIS_I32(pixelum(doc, V, XX), fundus);
    CREDO_VERUM(actum(doc, "<stratum actio=\"deletum\" id=\"1\"/>"));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
    CREDO_AEQUALIS_I32(pixelum(doc, V, II), c0);

    imprimere("\n--- VII: XVI maxima ---\n");
    {
        character t[LXIV];
              s32 k;

        per (k = III; k <= XX; k++)
        {
            sprintf(t,
                "<stratum actio=\"novum\" id=\"%d\" supra=\"1\"/>",
                (integer)k);
            (vacuum)actum(doc, t);
        }
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc),
            (i32)PICTOR_STRATA_MAXIMA);
    }

    imprimere("\n--- VIII: reapertio per checkpoint codificatum ---\n");
    {
        PictorDocumentum* alter;
                     i32  k;
                     b32  idem;
                     Xar* plagulae;
          VolumenPlagula* pl;
                  chorda  hex;
                  chorda  massa;
                     b32  inv;
                     i32  codices;

        /* stratum occultum per checkpoint (IV acta post): visibilitas
         * codificata, non solum reproiecta */
        CREDO_VERUM(actum(doc,
            "<stratum actio=\"visibile\" id=\"3\" valor=\"0\"/>"));
        per (k = ZEPHYRUM; k < IV; k++)
        {
            (vacuum)actum(doc, "<ictus instrumentum=\"penicillus\""
                " color=\"0\" magnitudo=\"1\"><punctum x=\"1\""
                " y=\"20\"/></ictus>");
        }
        codices   = ZEPHYRUM;
        plagulae  = volumen_plagulas_enumerare(vol, piscina);
        per (k = ZEPHYRUM; k < xar_numerus(plagulae); k++)
        {
            pl = (VolumenPlagula*)xar_obtinere(plagulae, k);
            si (!chorda_incipit(pl->via,
                chorda_ex_literis("checkpoint/",
                    piscina)))
            {
                perge;
            }
            hex   = volumen_plagulam_promere(vol, pl->via, piscina,
                &inv);
            massa = inv ? volumen_massam_promere(vol, hex, piscina,
                &inv)
                        : hex;
            si (   inv && massa.mensura > VIII
                && memcmp(massa.datum, "STRATA1 ", VIII) == ZEPHYRUM)
            {
                codices++;
            }
        }
        CREDO_VERUM(codices > ZEPHYRUM);

        alter = pictor_documentum_aperire(piscina, intern, vol, "");
        CREDO_NON_NIHIL(alter);
        si (alter)
        {
            CREDO_CHORDA_AEQUALIS(pictor_documentum_sigillum_hex(alter,
                piscina), pictor_documentum_sigillum_hex(doc, piscina));
            CREDO_VERUM(photographia_eadem(alter, doc));
            CREDO_AEQUALIS_I32(
                pictor_documentum_numerus_stratorum(alter),
                pictor_documentum_numerus_stratorum(doc));
            idem = VERUM;
            per (k = ZEPHYRUM; k < pictor_documentum_numerus_stratorum(
                doc); k++)
            {
                si (id_strati(alter, k) != id_strati(doc, k))
                {
                    idem = FALSUM;
                }
            }
            CREDO_VERUM(idem);
            CREDO_VERUM(pictor_documentum_verificare(alter));
        }
        CREDO_VERUM(pictor_documentum_verificare(doc));
    }

    imprimere("\n--- IX: checkpoint vetus recusatur ---\n");
    {
              Volumen* vv;
     PictorDocumentum* d;
     PictorDocumentum* alter;
               chorda  massa;
               chorda  clavis;
            character  hex[SIGILLUM_HEX_MENSURA];
            character  via[XXXII];

        vv = volumen_temporarium(piscina, "probatio_pictor_strata_v");
        d  = pictor_documentum_creare(piscina, intern, vv, "", XL, XXX,
            LXIV);
        CREDO_NON_NIHIL(d);
        si (d)
        {
            (vacuum)actum(d, "<ictus instrumentum=\"penicillus\""
                " color=\"3\" magnitudo=\"1\"><punctum x=\"0\""
                " y=\"5\"/><punctum x=\"39\" y=\"5\"/></ictus>");
            /* massa cruda formae veteris (pixela XL x XXX, 0x11) in
             * checkpoint actus ultimi */
            massa.mensura  = XL * XXX * (i32)magnitudo(i32);
            massa.datum    = (i8*)piscina_allocare(piscina,
                (memoriae_index)massa.mensura);
            memset(massa.datum, XVII, (size_t)massa.mensura);
            CREDO_VERUM(volumen_massam_condere(vv, massa, hex));
            sprintf(via, "checkpoint/%ld",
                (longus)pictor_documentum_finis(d));
            clavis = chorda_ex_literis(via, piscina);
            volumen_plagulam_condere(vv, clavis, chorda_ex_literis(hex,
                piscina), "pictor:checkpoint");
            alter = pictor_documentum_aperire(piscina, intern, vv, "");
            CREDO_NON_NIHIL(alter);
            CREDO_VERUM(alter && photographia_eadem(alter, d));
            CREDO_VERUM(alter && pixelum(alter, X, V) == c3);
        }
        volumen_claudere(vv);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
