/* xml_nomina.c - Characteres et tituli XML (vide xml_nomina.h)
 *
 * Intervalla ordinata, quaesitio binaria. Tabulae ex XML 1.0 ed. V
 * par. 2.2 et 2.3 verbatim transcriptae (probatio_xml_nomina fines
 * omnes et vicinos extra probat).
 */

#include "xml_nomina.h"
#include "utf8.h"

nomen structura {
    s32 ab;
    s32 ad;   /* inclusivum */
} IntervallumRunarum;

hic_manens constans IntervallumRunarum INITIALES[] = {
    { 0x3A, 0x3A },         /* ':' */
    { 0x41, 0x5A },         /* A-Z */
    { 0x5F, 0x5F },         /* '_' */
    { 0x61, 0x7A },         /* a-z */
    { 0xC0, 0xD6 },
    { 0xD8, 0xF6 },
    { 0xF8, 0x2FF },
    { 0x370, 0x37D },
    { 0x37F, 0x1FFF },
    { 0x200C, 0x200D },
    { 0x2070, 0x218F },
    { 0x2C00, 0x2FEF },
    { 0x3001, 0xD7FF },
    { 0xF900, 0xFDCF },
    { 0xFDF0, 0xFFFD },
    { 0x10000, 0xEFFFF }
};

/* NameChar praeter NameStartChar */
hic_manens constans IntervallumRunarum SEQUENTES[] = {
    { 0x2D, 0x2E },         /* '-' '.' */
    { 0x30, 0x39 },         /* 0-9 */
    { 0xB7, 0xB7 },
    { 0x300, 0x36F },
    { 0x203F, 0x2040 }
};

interior b32
_in (
    constans IntervallumRunarum* tabula,
                            s32  numerus,
                            s32  runa)
{
    s32 infra = ZEPHYRUM;
    s32 supra = numerus - I;

    dum (infra <= supra)
    {
        s32 medius = (infra + supra) / II;

        si (runa < tabula[medius].ab)
        {
            supra = medius - I;
        }
        alioquin si (runa > tabula[medius].ad)
        {
            infra = medius + I;
        }
        alioquin
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

#define NUMERUS(t) ((s32)(magnitudo(t) / magnitudo((t)[0])))

b32
xml_runa_licita (
    s32 runa)
{
    redde runa == 0x9 || runa == 0xA || runa == 0xD
        || (runa >= 0x20 && runa <= 0xD7FF)
        || (runa >= 0xE000 && runa <= 0xFFFD)
        || (runa >= 0x10000 && runa <= 0x10FFFF);
}

b32
xml_runa_initialis (
    s32 runa)
{
    redde _in(INITIALES, NUMERUS(INITIALES), runa);
}

b32
xml_runa_tituli (
    s32 runa)
{
    redde _in(INITIALES, NUMERUS(INITIALES), runa)
        || _in(SEQUENTES, NUMERUS(SEQUENTES), runa);
}

b32
xml_runa_pubid (
    s32 runa)
{
    si (   (runa >= 'a' && runa <= 'z') || (runa >= 'A' && runa <= 'Z')
        || (runa >= '0' && runa <= '9'))
    {
        redde VERUM;
    }
    commutatio (runa)
    {
        casus 0x20: casus 0xD: casus 0xA:
        casus '-': casus '\'': casus '(': casus ')': casus '+':
        casus ',': casus '.': casus '/': casus ':': casus '=':
        casus '?': casus ';': casus '!': casus '*': casus '#':
        casus '@': casus '$': casus '_': casus '%':
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

b32
xml_spatium (
    s32 runa)
{
    redde runa == 0x20 || runa == 0x9 || runa == 0xD || runa == 0xA;
}

s32
xml_titulum_metiri (
    constans character* octeti,
                   s32  mensura,
                   b32  nmtoken)
{
    constans i8* p;
    constans i8* finis;
    constans i8* initium;

    si (octeti == NIHIL || mensura <= ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    p        = (constans i8*)octeti;
    initium  = p;
    finis    = p + mensura;
    dum (p < finis)
    {
        constans i8* ante = p;
                s32  runa = utf8_decodere(&p, finis);
                b32  bona;

        /* prima Name: NameStartChar; ceterae et Nmtoken NameChar */
        bona = (ante == initium && !nmtoken) ? xml_runa_initialis(runa)
                                             : xml_runa_tituli(runa);
        si (runa < ZEPHYRUM || !bona)
        {
            redde (s32)(ante - initium);
        }
    }
    redde (s32)(p - initium);
}
