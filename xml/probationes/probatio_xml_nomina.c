/* probatio_xml_nomina.c - Characteres et tituli XML (ed. V)
 *
 * Fines intervallorum omnium et vicini extra, ex textu XML 1.0 ed. V
 * par. 2.2 et 2.3 SEORSUM transcripti (non ex tabula xml_nomina.c):
 * error unius in fine (off-by-one) capitur.
 */

#include "latina.h"
#include "credo.h"
#include "xml_nomina.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    s32 runa;
    b32 initialis;   /* NameStartChar */
    b32 tituli;      /* NameChar */
} CasusRunae;

hic_manens constans CasusRunae CASUS[] = {
    /* ASCII */
    { 0x2C, FALSUM, FALSUM },   /* ',' */
    { 0x2D, FALSUM, VERUM },    /* '-' */
    { 0x2E, FALSUM, VERUM },    /* '.' */
    { 0x2F, FALSUM, FALSUM },   /* '/' */
    { 0x30, FALSUM, VERUM },    /* '0' */
    { 0x39, FALSUM, VERUM },    /* '9' */
    { 0x3A, VERUM,  VERUM },    /* ':' */
    { 0x3B, FALSUM, FALSUM },
    { 0x40, FALSUM, FALSUM },   /* '@' */
    { 0x41, VERUM,  VERUM },    /* 'A' */
    { 0x5A, VERUM,  VERUM },    /* 'Z' */
    { 0x5B, FALSUM, FALSUM },
    { 0x5E, FALSUM, FALSUM },
    { 0x5F, VERUM,  VERUM },    /* '_' */
    { 0x60, FALSUM, FALSUM },
    { 0x61, VERUM,  VERUM },    /* 'a' */
    { 0x7A, VERUM,  VERUM },    /* 'z' */
    { 0x7B, FALSUM, FALSUM },
    /* B7 et Latina */
    { 0xB6, FALSUM, FALSUM },
    { 0xB7, FALSUM, VERUM },
    { 0xB8, FALSUM, FALSUM },
    { 0xBF, FALSUM, FALSUM },
    { 0xC0, VERUM,  VERUM },
    { 0xD6, VERUM,  VERUM },
    { 0xD7, FALSUM, FALSUM },   /* multiplicatio */
    { 0xD8, VERUM,  VERUM },
    { 0xF6, VERUM,  VERUM },
    { 0xF7, FALSUM, FALSUM },   /* divisio */
    { 0xF8, VERUM,  VERUM },
    { 0x2FF, VERUM, VERUM },
    /* signa combinantia: NameChar solum */
    { 0x300, FALSUM, VERUM },
    { 0x36F, FALSUM, VERUM },
    { 0x370, VERUM,  VERUM },
    { 0x37D, VERUM,  VERUM },
    { 0x37E, FALSUM, FALSUM },  /* punctum interrogationis Graecum */
    { 0x37F, VERUM,  VERUM },
    { 0x1FFF, VERUM, VERUM },
    { 0x2000, FALSUM, FALSUM },
    { 0x200B, FALSUM, FALSUM },
    { 0x200C, VERUM, VERUM },
    { 0x200D, VERUM, VERUM },
    { 0x200E, FALSUM, FALSUM },
    { 0x203E, FALSUM, FALSUM },
    { 0x203F, FALSUM, VERUM },
    { 0x2040, FALSUM, VERUM },
    { 0x2041, FALSUM, FALSUM },
    { 0x206F, FALSUM, FALSUM },
    { 0x2070, VERUM, VERUM },
    { 0x218F, VERUM, VERUM },
    { 0x2190, FALSUM, FALSUM },
    { 0x2BFF, FALSUM, FALSUM },
    { 0x2C00, VERUM, VERUM },
    { 0x2FEF, VERUM, VERUM },
    { 0x2FF0, FALSUM, FALSUM },
    { 0x3000, FALSUM, FALSUM },  /* spatium ideographicum */
    { 0x3001, VERUM, VERUM },
    { 0xD7FF, VERUM, VERUM },
    { 0xD800, FALSUM, FALSUM },  /* surrogata */
    { 0xF8FF, FALSUM, FALSUM },
    { 0xF900, VERUM, VERUM },
    { 0xFDCF, VERUM, VERUM },
    { 0xFDD0, FALSUM, FALSUM },  /* non-characteres */
    { 0xFDEF, FALSUM, FALSUM },
    { 0xFDF0, VERUM, VERUM },
    { 0xFFFD, VERUM, VERUM },
    { 0xFFFE, FALSUM, FALSUM },
    { 0xFFFF, FALSUM, FALSUM },
    { 0x10000, VERUM, VERUM },
    { 0xEFFFF, VERUM, VERUM },
    { 0xF0000, FALSUM, FALSUM },
    { 0x10FFFF, FALSUM, FALSUM },
    { 0x110000, FALSUM, FALSUM },
    { -1, FALSUM, FALSUM }
};

interior vacuum
_titulos_probare (
    CredoContextus* c)
{
    i32 i;
    i32 n = (i32)(magnitudo(CASUS) / magnitudo(CASUS[0]));

    (vacuum)c;
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (   xml_runa_initialis(CASUS[i].runa) != CASUS[i].initialis
            || xml_runa_tituli(CASUS[i].runa)    != CASUS[i].tituli)
        {
            imprimere("    runa U+%04X: initialis %d (exsp. %d), tituli"
                " %d (exsp. %d)\n", (insignatus integer)CASUS[i].runa,
                (integer)xml_runa_initialis(CASUS[i].runa),
                (integer)CASUS[i].initialis,
                (integer)xml_runa_tituli(CASUS[i].runa),
                (integer)CASUS[i].tituli);
        }
        CREDO_AEQUALIS_S32 ((s32)xml_runa_initialis(CASUS[i].runa),
            (s32)CASUS[i].initialis);
        CREDO_AEQUALIS_S32 ((s32)xml_runa_tituli(CASUS[i].runa),
            (s32)CASUS[i].tituli);
    }
}

interior vacuum
_characteres_probare (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_FALSUM (xml_runa_licita(0x0));
    CREDO_FALSUM (xml_runa_licita(0x8));
    CREDO_VERUM  (xml_runa_licita(0x9));
    CREDO_VERUM  (xml_runa_licita(0xA));
    CREDO_FALSUM (xml_runa_licita(0xB));
    CREDO_FALSUM (xml_runa_licita(0xC));
    CREDO_VERUM  (xml_runa_licita(0xD));
    CREDO_FALSUM (xml_runa_licita(0xE));
    CREDO_FALSUM (xml_runa_licita(0x1F));
    CREDO_VERUM  (xml_runa_licita(0x20));
    CREDO_VERUM  (xml_runa_licita(0x7F));   /* DEL licitum in 1.0 */
    CREDO_VERUM  (xml_runa_licita(0xD7FF));
    CREDO_FALSUM (xml_runa_licita(0xD800));
    CREDO_FALSUM (xml_runa_licita(0xDFFF));
    CREDO_VERUM  (xml_runa_licita(0xE000));
    CREDO_VERUM  (xml_runa_licita(0xFFFD));
    CREDO_FALSUM (xml_runa_licita(0xFFFE));
    CREDO_FALSUM (xml_runa_licita(0xFFFF));
    CREDO_VERUM  (xml_runa_licita(0x10000));
    CREDO_VERUM  (xml_runa_licita(0x10FFFF));
    CREDO_FALSUM (xml_runa_licita(0x110000));
    CREDO_FALSUM (xml_runa_licita(-1));

    CREDO_VERUM  (xml_spatium(0x20));
    CREDO_VERUM  (xml_spatium(0x9));
    CREDO_VERUM  (xml_spatium(0xA));
    CREDO_VERUM  (xml_spatium(0xD));
    CREDO_FALSUM (xml_spatium(0xA0));      /* NBSP non S */
    CREDO_FALSUM (xml_spatium(0xC));

    CREDO_VERUM  (xml_runa_pubid('a'));
    CREDO_VERUM  (xml_runa_pubid('%'));
    CREDO_VERUM  (xml_runa_pubid('\''));
    CREDO_VERUM  (xml_runa_pubid(0xD));
    CREDO_FALSUM (xml_runa_pubid('"'));
    CREDO_FALSUM (xml_runa_pubid('&'));
    CREDO_FALSUM (xml_runa_pubid('<'));
    CREDO_FALSUM (xml_runa_pubid(0x9));    /* TAB non PubidChar */
    CREDO_FALSUM (xml_runa_pubid(0xE9));
}

interior vacuum
_metiri_probare (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_AEQUALIS_S32 (xml_titulum_metiri("abc def", VII, FALSUM),
        (s32)III);
    CREDO_AEQUALIS_S32 (xml_titulum_metiri("a:b-c.d_e9=", XI, FALSUM),
        (s32)X);
    CREDO_AEQUALIS_S32 (xml_titulum_metiri("9abc", IV, FALSUM),
        ZEPHYRUM);
    CREDO_AEQUALIS_S32 (xml_titulum_metiri("9abc", IV, VERUM), (s32)IV);
    CREDO_AEQUALIS_S32 (xml_titulum_metiri("-x", II, FALSUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (xml_titulum_metiri(".x", II, VERUM), (s32)II);
    /* e acutum (C3 A9) initialis; signum combinans (CC 81) tituli */
    CREDO_AEQUALIS_S32 (xml_titulum_metiri("\303\251t\314\201>", VI,
        FALSUM), (s32)V);
    CREDO_AEQUALIS_S32 (xml_titulum_metiri("\314\201a", III, FALSUM),
        ZEPHYRUM);
    /* UTF-8 invalida titulum finit */
    CREDO_AEQUALIS_S32 (xml_titulum_metiri("ab\377c", IV, FALSUM),
        (s32)II);
    /* truncata in fine */
    CREDO_AEQUALIS_S32 (xml_titulum_metiri("ab\303", III, FALSUM),
        (s32)II);
    CREDO_AEQUALIS_S32 (xml_titulum_metiri("", ZEPHYRUM, FALSUM),
        ZEPHYRUM);
    CREDO_AEQUALIS_S32 (xml_titulum_metiri(NIHIL, III, FALSUM),
        ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "tituli: NameStartChar et NameChar ad fines", _titulos_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "characteres: Char, S, PubidChar", _characteres_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "titulum metiri: Name, Nmtoken, UTF-8", _metiri_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("xml_nomina", SECTIONES, NIHIL,
        NIHIL);
}
