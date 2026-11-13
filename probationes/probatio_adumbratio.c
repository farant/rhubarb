/* probatio_adumbratio.c - forma adumbrationum (negotia-paginae T2):
 * textus in cellula sua, fragmenta (macrones) ordine, globulus,
 * <mandata> translata in capsam suam (nomina colorum), vitia nominata
 * cum pagina vitii, PNG magnitudinis declaratae. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "mandatum.h"
#include "thema.h"
#include "imago.h"
#include "adumbratio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior AdumbratioResultus
reddere (
         CredoContextus* c,
     constans character* documentum)
{
    thema_initiare();
    redde adumbrationem_reddere(chorda_ex_literis(documentum,
        c->piscina), CCLXXX, CCLVI, c->piscina, internamentum_creare(
        c->piscina));
}

/* mandatum textus aequalis; NIHIL si nullum */
interior constans Mandatum*
textus_invenire (
    constans Mandata* md,
  constans character* textus)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; md && i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_aequalis_literis(x->textus, textus))
        {
            redde x;
        }
    }
    redde NIHIL;
}

interior b32
textus_continens (
    constans Mandata* md,
  constans character* fragmentum,
             Piscina* p)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; md && i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_continet(x->textus, chorda_ex_literis(fragmentum,
                   p)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
vitium_continet (
    AdumbratioResultus  r,
    constans character* fragmentum,
               Piscina* p)
{
    redde chorda_continet(r.vitium, chorda_ex_literis(fragmentum, p));
}

/* textus in cellula (1, 1) spatii: pixela (6, 8); color obscurus */
interior vacuum
_probare_textum (
    CredoContextus* c)
{
     AdumbratioResultus  r;
      constans Mandatum* x;

    r = reddere(c, "<adumbratio latitudo=\"120\" altitudo=\"64\">"
        "<columna spatium=\"1\"><textus>salve</textus>"
        "<textus color=\"obscurus\">vale</textus></columna>"
        "</adumbratio>");
    CREDO_AEQUALIS_I32(r.vitium.mensura, ZEPHYRUM);
    CREDO_AEQUALIS_S32(r.latitudo, CXX);
    CREDO_AEQUALIS_S32(r.altitudo, LXIV);
    x = textus_invenire(r.mandata, "salve");
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_S32(x->fines.x, VI);
    CREDO_AEQUALIS_S32(x->fines.y, VIII);
    CREDO_AEQUALIS_I32(x->color.valor, (i32)COLOR_TEXT);
    x = textus_invenire(r.mandata, "vale");
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_S32(x->fines.y, XVI);
    CREDO_AEQUALIS_I32(x->color.valor, (i32)COLOR_TEXT_DIM);
}

/* fragmentum semel definitum, ter vocatum: ordo et lineae */
interior vacuum
_probare_fragmenta (
    CredoContextus* c)
{
     AdumbratioResultus  r;
      constans Mandatum* a;
      constans Mandatum* b;
      constans Mandatum* g;

    r = reddere(c, "<adumbratio>"
        "<#@ordo t=\"@t\"><linea altitudo=\"2\"><textus>&@t;</textus>"
        "</linea></#>"
        "<columna><<#@ordo t=\"alpha\">><<#@ordo t=\"beta\">>"
        "<<#@ordo t=\"gamma\">></columna></adumbratio>");
    CREDO_AEQUALIS_I32(r.vitium.mensura, ZEPHYRUM);
    a = textus_invenire(r.mandata, "alpha");
    b = textus_invenire(r.mandata, "beta");
    g = textus_invenire(r.mandata, "gamma");
    CREDO_NECESSE(a && b && g);
    CREDO_AEQUALIS_S32(b->fines.y - a->fines.y, XVI);
    CREDO_AEQUALIS_S32(g->fines.y - b->fines.y, XVI);
}

interior vacuum
_probare_globulum (
    CredoContextus* c)
{
     AdumbratioResultus  r;
      constans Mandatum* x;

    r = reddere(c, "<adumbratio><linea intervallum=\"1\">"
        "<globulum status=\"shipped\"/>"
        "<globulum textus=\"Resume (r)\" obscurum=\"verum\"/>"
        "</linea></adumbratio>");
    CREDO_AEQUALIS_I32(r.vitium.mensura, ZEPHYRUM);
    CREDO_NON_NIHIL(textus_invenire(r.mandata, "shipped"));
    x = textus_invenire(r.mandata, "Resume (r)");
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_I32(x->color.valor, (i32)COLOR_TEXT_DIM);
}

/* textus in linea altitudinem lineae implet et verticaliter
 * centratur (Franus 2026-11-06: tituli ad summum lineae haerebant):
 * linea II, fons parvus (8 px) -> +4; in columna non movetur; linea
 * apta cum textu solo altitudinem I servat; altitudo expressa vincit;
 * titulus (16 px) in linea II -> +0; audax quoque centratur;
 * columna crescens cum spatio: textus eius non crescit */
interior vacuum
_probare_centratum (
    CredoContextus* c)
{
     AdumbratioResultus  r;
      constans Mandatum* x;

    r = reddere(c, "<adumbratio latitudo=\"120\" altitudo=\"128\">"
        "<columna altitudo=\"crescens\">"
        "<linea altitudo=\"2\"><globulum status=\"shipped\"/>"
        "<textus>library</textus></linea>"
        "<textus>infra</textus>"
        "<linea><textus>sola</textus></linea>"
        "<textus>post</textus>"
        "<linea altitudo=\"2\"><globulum status=\"planned\"/>"
        "<textus altitudo=\"1\">summa</textus></linea>"
        "<linea altitudo=\"2\"><textus fons=\"titulus\">T</textus>"
        "</linea>"
        "<linea altitudo=\"2\"><globulum status=\"shipped\"/>"
        "<textus audax=\"verum\">fortis</textus></linea>"
        "<spatium/></columna></adumbratio>");
    CREDO_AEQUALIS_I32(r.vitium.mensura, ZEPHYRUM);
    CREDO_GRADUS("linea II: +4");
    x = textus_invenire(r.mandata, "library");
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_S32(x->fines.y, IV);
    CREDO_GRADUS("columna: sine translatione");
    x = textus_invenire(r.mandata, "infra");
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_S32(x->fines.y, XVI);
    CREDO_GRADUS("linea apta: altitudo I");
    x = textus_invenire(r.mandata, "sola");
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_S32(x->fines.y, XXIV);
    x = textus_invenire(r.mandata, "post");
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_S32(x->fines.y, XXXII);
    CREDO_GRADUS("altitudo expressa vincit");
    x = textus_invenire(r.mandata, "summa");
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_S32(x->fines.y, XL);
    CREDO_GRADUS("titulus in linea II");
    x = textus_invenire(r.mandata, "T");
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_S32(x->fines.y, LVI);
    CREDO_GRADUS("audax");
    x = textus_invenire(r.mandata, "fortis");
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_S32(x->fines.y, LXXVI);
}

/* mandata in capsa spatio II: coetus cum origine (12, 16) et
 * translatione nulla (origo = fines + translatio: bis numerata
 * primitiva extra sectionem iacerent), primitiva coordinatis suis;
 * nomen coloris -> gradus thematis */
interior vacuum
_probare_mandata (
    CredoContextus* c)
{
     AdumbratioResultus  r;
      constans Mandatum* x;
      constans Mandatum* coetus  = NIHIL;
      constans Mandatum* rect    = NIHIL;
                    i32  i;

    r = reddere(c, "<adumbratio><columna spatium=\"2\">"
        "<mandata latitudo=\"4\" altitudo=\"2\">"
        "<rectangulum x=\"1\" y=\"1\" latitudo=\"5\" altitudo=\"3\""
        " color=\"accentus\" impletum=\"verum\"/></mandata>"
        "</columna></adumbratio>");
    CREDO_AEQUALIS_I32(r.vitium.mensura, ZEPHYRUM);
    per (i = ZEPHYRUM; i < xar_numerus(r.mandata->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(r.mandata->elementa, i);
        si (   x->genus        == MANDATUM_COETUS && x->fines.x == XII
            && x->fines.y      == XVI && x->translatio.x == ZEPHYRUM
            && x->translatio.y == ZEPHYRUM)
        {
            coetus = x;
        }
        si (   x->genus == MANDATUM_RECTANGULUM && x->fines.x == I
            && x->fines.latitudo == V)
        {
            rect = x;
        }
    }
    CREDO_NON_NIHIL(coetus);
    CREDO_NECESSE_NON_NIHIL(rect);
    CREDO_AEQUALIS_I32(rect->color.valor, (i32)COLOR_ACCENT_PRIMARY);
}

/* vitia: nominata cum linea; Mandata = pagina vitii */
interior vacuum
_probare_vitia (
    CredoContextus* c)
{
    AdumbratioResultus r;

    CREDO_GRADUS("fragmentum ignotum");
    r = reddere(c, "<adumbratio>\n<columna>\n<<#@nullus>>\n</columna>\n"
        "</adumbratio>");
    CREDO_VERUM(vitium_continet(r, "nullus", c->piscina));
    CREDO_VERUM(vitium_continet(r, "linea", c->piscina));
    CREDO_VERUM(textus_invenire(r.mandata, "Mock-up error") != NIHIL);

    CREDO_GRADUS("loculus non impletus");
    r = reddere(c, "<adumbratio><#@f t=\"@t\"><textus>&@t;</textus></#>"
        "<columna><<#@f>></columna></adumbratio>");
    CREDO_VERUM(r.vitium.mensura > ZEPHYRUM);

    CREDO_GRADUS("sine '@': transclusio contenti, vitium nominatum");
    r = reddere(c, "<adumbratio><#f t=\"@t\"><textus>&@t;</textus></#>"
        "<columna><<#f t=\"x\">></columna></adumbratio>");
    CREDO_VERUM(vitium_continet(r, "#@", c->piscina));

    CREDO_GRADUS("elementum ignotum");
    r = reddere(c, "<adumbratio><rota/></adumbratio>");
    CREDO_VERUM(vitium_continet(r, "rota", c->piscina));
    CREDO_VERUM(textus_continens(r.mandata, "rota", c->piscina));

    CREDO_GRADUS("attributum ignotum");
    r = reddere(c, "<adumbratio><columna rubor=\"x\"/></adumbratio>");
    CREDO_VERUM(vitium_continet(r, "rubor", c->piscina));

    CREDO_GRADUS("valor malus");
    r = reddere(c, "<adumbratio><columna latitudo=\"multum\"/>"
        "</adumbratio>");
    CREDO_VERUM(vitium_continet(r, "latitudo", c->piscina));
    r = reddere(c, "<adumbratio><textus color=\"purpura\">x</textus>"
        "</adumbratio>");
    CREDO_VERUM(vitium_continet(r, "purpura", c->piscina));
    r = reddere(c, "<adumbratio><globulum status=\"fortasse\"/>"
        "</adumbratio>");
    CREDO_VERUM(vitium_continet(r, "fortasse", c->piscina));

    CREDO_GRADUS("documentum malum, radix aliena");
    r = reddere(c, "<adumbratio><columna>");
    CREDO_VERUM(r.vitium.mensura > ZEPHYRUM);
    CREDO_VERUM(textus_invenire(r.mandata, "Mock-up error") != NIHIL);
    r = reddere(c, "<pagina/>");
    CREDO_VERUM(vitium_continet(r, "adumbratio", c->piscina));
}

/* PNG: scriptum (in area sectionis) et relectum magnitudine
 * declarata */
interior vacuum
_probare_png (
    CredoContextus* c)
{
    AdumbratioResultus  r;
          ImagoFructus  f;
    constans character* area;
             character  via[CCLVI];

    area = credo_area(c);
    CREDO_NECESSE_NON_NIHIL(area);
    sprintf(via, "%.200s/adumbratio.png", area);
    r = reddere(c, "<adumbratio latitudo=\"96\" altitudo=\"40\">"
        "<columna spatium=\"1\"><textus>png</textus></columna>"
        "</adumbratio>");
    CREDO_VERUM(adumbrationem_png_scribere(&r, via, c->piscina));
    f = imago_caricare_ex_file(via, c->piscina);
    CREDO_NECESSE(f.successus);
    CREDO_AEQUALIS_I32(f.imago.latitudo, XCVI);
    CREDO_AEQUALIS_I32(f.imago.altitudo, XL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "textus", _probare_textum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fragmenta", _probare_fragmenta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "globulus", _probare_globulum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "centratum", _probare_centratum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mandata", _probare_mandata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _probare_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "png", _probare_png, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("adumbratio", SECTIONES);
}
