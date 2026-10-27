/* pecunia_monetae_generare.c - tabulam monetarum GENERARE
 *
 * Usus: pecunia_monetae_generare <directorium> <supplementum>
 *        <exitus.c>
 *
 * Legit <directorium>/list-one.xml (monetae currentes ISO 4217) et
 * <directorium>/list-three.xml (retractae) - indices SIX fixi - et
 * <supplementum> (STML: monetae quas indices non collocant, cum
 * fontibus), scribit lib/pecunia_monetae.c ordine codicum.
 *
 * REGULAE (pecunia-spec II.9): in indice uno ordo primus vincit
 * (EUR multis terris); codex repetitus cum exponente DIVERSO = vitium
 * (exitus II); "N.A." -> tabula sine unitate; supplementum: historica
 * debet esse in indice tertio et non in uno, extra_iso in neutro; ordo
 * supplementi quem index unus iam collocat = vitium (contradictio aut
 * superfluum) - numquam emendatio tacita. Instrumentum aetatis
 * evolutionis; tools/pecunia_monetae_generare.sh id struit et currit.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "xar.h"
#include "xml.h"
#include "stml.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDINES_MAXIMI  CCLVI
#define SINE_MAXIMI     XXXII
#define TITULUS_MAXIMUS CXXVIII
#define TERTII_MAXIMI   CCLVI

nomen structura {
    character codex[IV];
          s32 numerus;
          s32 exponens;
          s32 status;     /* 0 currens, 1 historica, 2 extra ISO */
    character titulus[TITULUS_MAXIMUS];
} Ordo;

hic_manens Ordo      ordines[ORDINES_MAXIMI];
hic_manens s32       numerus_ordinum = ZEPHYRUM;
hic_manens character sine_unitate[SINE_MAXIMI][IV];
hic_manens s32       numerus_sine = ZEPHYRUM;
hic_manens character tertii[TERTII_MAXIMI][IV];
hic_manens s32       tertii_numeri[TERTII_MAXIMI];
hic_manens s32       numerus_tertiorum = ZEPHYRUM;
hic_manens character publicatum_unum[XXXII];
hic_manens character publicatum_tertium[XXXII];

interior vacuum
_mori (
    constans character* nuntius,
    constans character* res)
{
    fprintf(stderr, "pecunia_monetae_generare: %s%s%s\n", nuntius,
        res ? ": " : "", res ? res : "");
    exit(II);
}

/* chorda -> literae NUL terminatae, spatiis extremis detractis */
interior vacuum
_ad_literas (
       chorda  c,
    character* exitus,
          s32  capacitas)
{
    s32 a = ZEPHYRUM;
    s32 b = (s32)c.mensura;

    dum (   a < b && (c.datum[a] == ' ' || c.datum[a] == '\t'
                   || c.datum[a] == '\r' || c.datum[a] == '\n'))
    {
        a++;
    }
    dum (   b > a && (c.datum[b - I] == ' ' || c.datum[b - I] == '\t'
                   || c.datum[b - I] == '\r' || c.datum[b - I] == '\n'))
    {
        b--;
    }
    si (b - a >= capacitas)
    {
        _mori("textus nimis longus", NIHIL);
    }
    si (b > a)
    {
        memcpy(exitus, c.datum + a, (memoriae_index)(b - a));
    }
    exitus[b - a] = '\0';
}

/* textus liberi <titulus> elementi, aut "" */
interior vacuum
_textus_liberi (
               XmlNodus* e,
     constans character* titulus,
              character* exitus,
                    s32  capacitas,
                Piscina* p)
{
    XmlNodus* l = xml_invenire_liberum(e, titulus);

    si (l == NIHIL)
    {
        exitus[ZEPHYRUM] = '\0';
        redde;
    }
    _ad_literas(xml_textus_internus(l, p), exitus, capacitas);
}

interior b32
_codex_validus (
    constans character* c)
{
    s32 i;

    per (i = ZEPHYRUM; i < III; i++)
    {
        si (c[i] < 'A' || c[i] > 'Z')
        {
            redde FALSUM;
        }
    }
    redde c[III] == '\0';
}

/* numerus decimalis ex literis; -I si non digiti */
interior s32
_numerus (
    constans character* s)
{
    s32 v = ZEPHYRUM;
    s32 i = ZEPHYRUM;

    si (s[ZEPHYRUM] == '\0')
    {
        redde -I;
    }
    dum (s[i] != '\0')
    {
        si (s[i] < '0' || s[i] > '9' || v > CM * M)
        {
            redde -I;
        }
        v = v * X + (s[i] - '0');
        i++;
    }
    redde v;
}

interior s32
_invenire (
    constans character* codex)
{
    s32 i;

    per (i = ZEPHYRUM; i < numerus_ordinum; i++)
    {
        si (strcmp(ordines[i].codex, codex) == ZEPHYRUM)
        {
            redde i;
        }
    }
    redde -I;
}

interior b32
_in_indice (
    constans character  index[][IV],
                   s32  numerus,
    constans character* codex)
{
    s32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (strcmp(index[i], codex) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* radix ISO_4217 (resultus aut documentum aut elementum) */
interior XmlNodus*
_radix_iso (
    constans character* via,
             character* publicatum,
               Piscina* p)
{
         chorda  textus = filum_legere_totum(via, p);
    XmlResultus  r;
         chorda* pb;

    si (textus.datum == NIHIL || textus.mensura == ZEPHYRUM)
    {
        _mori("legi non potest", via);
    }
    r = xml_legere(textus, p, internamentum_creare(p));
    si (!r.successus || r.radix == NIHIL)
    {
        _mori("XML malum", via);
    }
    si (   r.radix->titulus == NIHIL || !chorda_aequalis_literis(
        *r.radix->titulus, "ISO_4217"))
    {
        XmlNodus* iso = xml_invenire_liberum(r.radix, "ISO_4217");

        si (iso == NIHIL)
        {
            _mori("radix ISO_4217 deest", via);
        }
        r.radix = iso;
    }
    pb = xml_attributum_capere(r.radix, "Pblshd");
    si (pb == NIHIL)
    {
        _mori("Pblshd deest", via);
    }
    _ad_literas(*pb, publicatum, XXXII);
    redde r.radix;
}

interior vacuum
_legere_unum (
    constans character* via,
               Piscina* p)
{
     XmlNodus* radix   = _radix_iso(via, publicatum_unum, p);
     XmlNodus* tabula  = xml_invenire_liberum(radix, "CcyTbl");
          Xar* ingressus;
          s32  i;

    si (tabula == NIHIL)
    {
        _mori("CcyTbl deest", via);
    }
    ingressus = xml_invenire_omnes_liberos(tabula, "CcyNtry", p);
    per (i = ZEPHYRUM; ingressus
        && i < (s32)xar_numerus(ingressus); i++)
    {
         XmlNodus* e = *(XmlNodus**)xar_obtinere(ingressus, (i32)i);
        character  codex[VIII];
        character  unitates[XVI];
        character  numerus[XVI];
        character  titulus[TITULUS_MAXIMUS];
              s32  k;

        _textus_liberi(e, "Ccy", codex, VIII, p);
        si (codex[ZEPHYRUM] == '\0')
        {
            continue;          /* terra sine moneta (ANTARCTICA) */
        }
        si (!_codex_validus(codex))
        {
            _mori("codex malus in indice uno", codex);
        }
        _textus_liberi(e, "CcyMnrUnts", unitates, XVI, p);
        _textus_liberi(e, "CcyNbr", numerus, XVI, p);
        _textus_liberi(e, "CcyNm", titulus, TITULUS_MAXIMUS, p);
        si (strcmp(unitates, "N.A.") == ZEPHYRUM)
        {
            si (!_in_indice((constans character (*)[IV])sine_unitate,
                numerus_sine, codex))
            {
                si (numerus_sine >= SINE_MAXIMI)
                {
                    _mori("nimis multae sine unitate", NIHIL);
                }
                memcpy(sine_unitate[numerus_sine++], codex, IV);
            }
            continue;
        }
        k = _invenire(codex);
        si (k >= ZEPHYRUM)
        {
            si (ordines[k].exponens != _numerus(unitates))
            {
                _mori("codex repetitus cum exponente diverso", codex);
            }
            continue;          /* EUR multis terris: primus vincit */
        }
        si (_numerus(unitates) < ZEPHYRUM || _numerus(unitates) > IV)
        {
            _mori("unitas minor extra 0..IV", codex);
        }
        si (numerus_ordinum >= ORDINES_MAXIMI)
        {
            _mori("nimis multae monetae", NIHIL);
        }
        memcpy(ordines[numerus_ordinum].codex, codex, IV);
        ordines[numerus_ordinum].numerus   = _numerus(numerus);
        ordines[numerus_ordinum].exponens  = _numerus(unitates);
        ordines[numerus_ordinum].status    = ZEPHYRUM;
        strcpy(ordines[numerus_ordinum].titulus, titulus);
        si (ordines[numerus_ordinum].numerus < ZEPHYRUM)
        {
            _mori("numerus ISO malus", codex);
        }
        numerus_ordinum++;
    }
    si (numerus_ordinum == ZEPHYRUM)
    {
        _mori("index unus vacuus", via);
    }
}

interior vacuum
_legere_tertium (
    constans character* via,
               Piscina* p)
{
     XmlNodus* radix   = _radix_iso(via, publicatum_tertium, p);
     XmlNodus* tabula  = xml_invenire_liberum(radix, "HstrcCcyTbl");
          Xar* ingressus;
          s32  i;

    si (tabula == NIHIL)
    {
        _mori("HstrcCcyTbl deest", via);
    }
    ingressus = xml_invenire_omnes_liberos(tabula, "HstrcCcyNtry", p);
    per (i = ZEPHYRUM; ingressus
        && i < (s32)xar_numerus(ingressus); i++)
    {
         XmlNodus* e = *(XmlNodus**)xar_obtinere(ingressus, (i32)i);
        character  codex[VIII];
        character  numerus[XVI];

        _textus_liberi(e, "Ccy", codex, VIII, p);
        _textus_liberi(e, "CcyNbr", numerus, XVI, p);
        si (   !_codex_validus(codex) || _in_indice(
            (constans character (*)[IV])tertii, numerus_tertiorum,
            codex))
        {
            continue;
        }
        si (numerus_tertiorum >= TERTII_MAXIMI)
        {
            _mori("nimis multae retractae", NIHIL);
        }
        /* HRK bis (MCMXCIV, MMXXIII): ordo primus, numerus idem */
        tertii_numeri[numerus_tertiorum] = _numerus(numerus);
        memcpy(tertii[numerus_tertiorum++], codex, IV);
    }
}

interior vacuum
_attributum (
             StmlNodus* m,
    constans character* titulus,
             character* exitus,
                   s32  capacitas)
{
    chorda* v = stml_attributum_capere(m, titulus);

    si (v == NIHIL)
    {
        _mori("attributum deest in supplemento", titulus);
    }
    _ad_literas(*v, exitus, capacitas);
}

interior vacuum
_legere_supplementum (
    constans character* via,
               Piscina* p)
{
          chorda  textus = filum_legere_totum(via, p);
    StmlResultus  r;
             Xar* monetae;
             s32  i;

    si (textus.datum == NIHIL || textus.mensura == ZEPHYRUM)
    {
        _mori("legi non potest", via);
    }
    r = stml_legere(textus, p, internamentum_creare(p));
    si (!r.successus || r.elementum_radix == NIHIL)
    {
        _mori("STML malum", via);
    }
    monetae = stml_invenire_omnes_liberos(r.elementum_radix, "moneta",
        p);
    per (i = ZEPHYRUM; monetae && i < (s32)xar_numerus(monetae); i++)
    {
         StmlNodus* m = *(StmlNodus**)xar_obtinere(monetae, (i32)i);
         character  codex[VIII];
         character  exponens[VIII];
         character  status[XXXII];
         character  titulus[TITULUS_MAXIMUS];
         character  fons[DXII];
               s32  e;
               s32  st  = -I;   /* _mori exit vocat; clang id nescit */
               s32  n   = ZEPHYRUM;
               s32  k;

        _attributum(m, "codex", codex, VIII);
        _attributum(m, "exponens", exponens, VIII);
        _attributum(m, "status", status, XXXII);
        _attributum(m, "titulus", titulus, TITULUS_MAXIMUS);
        _attributum(m, "fons", fons, DXII);
        e = _numerus(exponens);
        si (   !_codex_validus(codex) || e < ZEPHYRUM || e > IV
            || fons[ZEPHYRUM] == '\0' || titulus[ZEPHYRUM] == '\0')
        {
            _mori("ordo supplementi malus", codex);
        }
        si (_invenire(codex) >= ZEPHYRUM)
        {
            s32 j = _invenire(codex);

            _mori(ordines[j].status != ZEPHYRUM
                ? "supplementum bis eundem codicem habet"
                : ordines[j].exponens == e
                ? "supplementum superfluum (index unus collocat)"
                : "supplementum contradicit indici uno", codex);
        }
        si (strcmp(status, "historica") == ZEPHYRUM)
        {
            per (k = ZEPHYRUM; k < numerus_tertiorum; k++)
            {
                si (strcmp(tertii[k], codex) == ZEPHYRUM)
                {
                    n = tertii_numeri[k];
                    frange;
                }
            }
            si (k == numerus_tertiorum || n <= ZEPHYRUM)
            {
                _mori("historica non in indice tertio (cum numero)",
                    codex);
            }
            st = I;
        }
        alioquin si (strcmp(status, "extra_iso") == ZEPHYRUM)
        {
            si (_in_indice((constans character (*)[IV])tertii,
                numerus_tertiorum, codex))
            {
                _mori("extra_iso tamen in indice tertio", codex);
            }
            st = II;
        }
        alioquin
        {
            _mori("status ignotus", status);
        }
        si (numerus_ordinum >= ORDINES_MAXIMI)
        {
            _mori("nimis multae monetae", NIHIL);
        }
        memcpy(ordines[numerus_ordinum].codex, codex, IV);
        ordines[numerus_ordinum].numerus   = n;
        ordines[numerus_ordinum].exponens  = e;
        ordines[numerus_ordinum].status    = st;
        strcpy(ordines[numerus_ordinum].titulus, titulus);
        numerus_ordinum++;
    }
}

interior integer
_comparare_ordines (
    constans vacuum* a,
    constans vacuum* b)
{
    redde strcmp(((constans Ordo*)a)->codex,
        ((constans Ordo*)b)->codex);
}

interior integer
_comparare_codices (
    constans vacuum* a,
    constans vacuum* b)
{
    redde strcmp((constans character*)a, (constans character*)b);
}

/* titulus ut litterae C: '"' et '\\' effugiuntur, octeti non ASCII
 * octaliter (tres digiti - effugium hex digitos sequentes devoraret) */
interior vacuum
_titulum_scribere (
     ChordaAedificator* a,
    constans character* t)
{
    s32 i;

    chorda_aedificator_appendere_character(a, '"');
    per (i = ZEPHYRUM; t[i] != '\0'; i++)
    {
        i32 o = (i32)(insignatus character)t[i];

        si (o == '"' || o == '\\')
        {
            chorda_aedificator_appendere_character(a, '\\');
            chorda_aedificator_appendere_character(a, t[i]);
        }
        alioquin si (o < 0x20 || o >= 0x7F)
        {
            character b[VIII];

            sprintf(b, "\\%03o", (insignatus integer)o);
            chorda_aedificator_appendere_literis(a, b);
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, t[i]);
        }
    }
    chorda_aedificator_appendere_character(a, '"');
}

hic_manens constans character* STATUS_NOMINA[] = {
    "PECUNIA_MONETA_CURRENS", "PECUNIA_MONETA_HISTORICA",
    "PECUNIA_MONETA_EXTRA_ISO"
};

interior chorda
_scribere (
    constans character* directorium,
    constans character* supplementum,
               Piscina* p)
{
     ChordaAedificator* a = chorda_aedificator_creare(p, LXIV * MXXIV);
             character  b[CCLVI];
                   s32  i;
                   s32  numeri[III] = { ZEPHYRUM, ZEPHYRUM, ZEPHYRUM };

    per (i = ZEPHYRUM; i < numerus_ordinum; i++)
    {
        numeri[ordines[i].status]++;
    }
    chorda_aedificator_appendere_literis(a,
        "/* pecunia_monetae.c - GENERATUM: "
        "tools/pecunia_monetae_generare.sh"
        " (NE EDITA MANU)\n *\n * Ex indicibus ISO 4217 fixis (");
    chorda_aedificator_appendere_literis(a, directorium);
    sprintf(b,
        "/:\n * list-one.xml Pblshd %s, list-three.xml Pblshd %s)"
        " et\n * ", publicatum_unum, publicatum_tertium);
    chorda_aedificator_appendere_literis(a, b);
    chorda_aedificator_appendere_literis(a, supplementum);
    sprintf(b, " (fontes ibi).\n * Ordines %d: currentes %d,"
        " historicae %d, extra ISO %d; sine unitate minore %d.\n */\n",
        (integer)numerus_ordinum, (integer)numeri[ZEPHYRUM],
        (integer)numeri[I], (integer)numeri[II], (integer)numerus_sine);
    chorda_aedificator_appendere_literis(a, b);
    chorda_aedificator_appendere_literis(a,
        "#include \"pecunia_monetae.h\"\n\n"
        "constans PecuniaMonetaInformatio PECUNIA_MONETAE[] = {\n");
    per (i = ZEPHYRUM; i < numerus_ordinum; i++)
    {
        sprintf(b, "    { \"%s\", %d, %d, %s, ", ordines[i].codex,
            (integer)ordines[i].numerus, (integer)ordines[i].exponens,
            STATUS_NOMINA[ordines[i].status]);
        chorda_aedificator_appendere_literis(a, b);
        _titulum_scribere(a, ordines[i].titulus);
        chorda_aedificator_appendere_literis(a,
            i + I < numerus_ordinum ? " },\n" : " }\n");
    }
    sprintf(b, "};\n\nconstans i32 PECUNIA_MONETAE_NUMERUS = %d;\n\n"
        "constans character* constans "
        "PECUNIA_MONETAE_SINE_UNITATE[] = {\n",
        (integer)numerus_ordinum);
    chorda_aedificator_appendere_literis(a, b);
    per (i = ZEPHYRUM; i < numerus_sine; i++)
    {
        sprintf(b, "    \"%s\"%s\n", sine_unitate[i],
            i + I < numerus_sine ? "," : "");
        chorda_aedificator_appendere_literis(a, b);
    }
    sprintf(b, "};\n\nconstans i32 PECUNIA_MONETAE_SINE_UNITATE_NUMERUS"
        " = %d;\n", (integer)numerus_sine);
    chorda_aedificator_appendere_literis(a, b);
    redde chorda_aedificator_finire(a);
}

s32
principale (
      integer   argc,
    character** argv)
{
     Piscina* p;
   character  via[DXII];

    si (argc != IV)
    {
        fprintf(stderr, "usus: pecunia_monetae_generare <directorium>"
            " <supplementum> <exitus.c>\n");
        redde II;
    }
    p = piscina_generare_dynamicum("pecunia_monetae_generare",
        CCLVI * MXXIV);
    si (strlen(argv[I]) > CDLXXX)
    {
        _mori("via nimis longa", argv[I]);
    }
    sprintf(via, "%s/list-one.xml", argv[I]);
    _legere_unum(via, p);
    sprintf(via, "%s/list-three.xml", argv[I]);
    _legere_tertium(via, p);
    _legere_supplementum(argv[II], p);
    qsort(ordines, (memoriae_index)numerus_ordinum, magnitudo(Ordo),
        _comparare_ordines);
    qsort(sine_unitate, (memoriae_index)numerus_sine, IV,
        _comparare_codices);
    si (!filum_scribere(argv[III], _scribere(argv[I], argv[II], p)))
    {
        _mori("scribi non potest", argv[III]);
    }
    redde ZEPHYRUM;
}
