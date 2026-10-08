/* fictio.c - valores ficti sine JSON (norma-spec par. IV; norma-plan-2 N1)
 *
 * Corpus: XLVIII sententiae VERBATIM ex M. Tullii Ciceronis In Catilinam
 * (Project Gutenberg EBook #226, 'Cicero's Orations', Language: Latin;
 * textus in dominio publico), lectae ex ../gutenberg-mirror/2/2/226/226.txt
 * 2026-10-08, ex orationibus ipsis (non ex argumento editoris). */
#include "fictio.h"
#include "chorda_aedificator.h"
#include "utf8.h"

#include <stdio.h>
#include <string.h>

hic_manens constans character* constans _praenomina[] = {
    "Marcus", "Gaius", "Lucius", "Publius", "Quintus", "Titus",
    "Gnaeus", "Sextus", "Aulus", "Decimus", NIHIL
};
hic_manens constans character* constans _nomina[] = {
    "Tullius", "Iulius", "Cornelius", "Claudius", "Valerius",
        "Aemilius",
    "Fabius", "Antonius", "Licinius", "Porcius", NIHIL
};
hic_manens constans character* constans _ordines_feminarum[] = {
    "Maior", "Minor", "Tertia", NIHIL
};
hic_manens constans character* constans _dominia[] = {
    "example.org", "example.com", "example.net", NIHIL
};
hic_manens constans character* constans _corpus[] = {
    "Quo usque tandem abutere, Catilina, patientia nostra?",
    "Patere tua consilia non sentis, constrictam iam horum omnium scientia teneri coniurationem tuam non vides?",
    "Nos autem fortes viri satis facere rei publicae videmur, si istius furorem ac tela vitemus.",
    "Fuit, fuit ista quondam in hac re publica virtus, ut viri fortes acrioribus suppliciis civem perniciosum quam acerbissimum hostem coercerent.",
    "Vivis, et vivis non ad deponendam, sed ad confirmandam audaciam.",
    "Cupio, patres conscripti, me esse clementem, cupio in tantis rei publicae periculis me non dissolutum videri, sed iam me ipse inertiae nequitiaeque condemno.",
    "Verum ego hoc, quod iam pridem factum esse oportuit, certa de causa nondum adducor ut faciam.",
    "Multorum te etiam oculi et aures non sentientem, sicut adhuc fecerunt, speculabuntur atque custodient.",
    "Muta iam istam mentem, mihi crede, obliviscere caedis atque incendiorum.",
    "Teneris undique; luce sunt clariora nobis tua consilia omnia; quae iam mecum licet recognoscas.",
    "Nihil agis, nihil moliris, nihil cogitas, quod non ego non modo audiam, sed etiam videam planeque sentiam.",
    "Recognosce tandem mecum noctem illam superiorem; iam intelleges multo me vigilare acrius ad salutem quam te ad perniciem rei publicae.",
    "Video enim esse hic in senatu quosdam, qui tecum una fuerunt.",
    "Quae cum ita sint, Catilina, perge, quo coepisti, egredere aliquando ex urbe; patent portae; proficiscere.",
    "Nimium diu te imperatorem tua illa Manliana castra desiderant.",
    "Educ tecum etiam omnes tuos, si minus, quam plurimos; purga urbem.",
    "Magno me metu liberabis, dum modo inter me atque te murus intersit.",
    "Nobiscum versari iam diutius non potes; non feram, non patiar, non sinam.",
    "Non est saepius in uno homine summa salus periclitanda rei publicae.",
    "Exire ex urbe iubet consul hostem.",
    "Interrogas me, num in exilium; non iubeo, sed, si me consulis, suadeo.",
    "Quid est enim, Catilina, quod te iam in hac urbe delectare possit?",
    "Nunc vero quae tua est ista vita?",
    "Quis te ex hac tanta frequentia totque tuis amicis ac necessariis salutavit?",
    "Quam ob rem discede atque hunc mihi timorem eripe; si est verus, ne opprimar, sin falsus, ut tandem aliquando timere desinam.",
    "Quid exspectas auctoritatem loquentium, quorum voluntatem tacitorum perspicis?",
    "Habes, ubi ostentes tuam illam praeclaram patientiam famis, frigoris, inopiae rerum omnium, quibus te brevi tempore confectum esse senties.",
    "At persaepe etiam privati in hac re publica perniciosos cives morte multarunt.",
    "At numquam in hac urbe, qui a re publica defecerunt, civium iura tenuerunt.",
    "His ego sanctissimis rei publicae vocibus et eorum hominum, qui hoc idem sentiunt, mentibus pauca respondebo.",
    "Hoc autem uno interfecto intellego hanc rei publicae pestem paulisper reprimi, non in perpetuum comprimi posse.",
    "Abiit, excessit, evasit, erupit.",
    "Nulla iam pernicies a monstro illo atque prodigio moenibus ipsis intra moenia comparabitur.",
    "Atque hunc quidem unum huius belli domestici ducem sine controversia vicimus.",
    "Non enim iam inter latera nostra sica illa versabitur, non in campo, non in foro, non in curia, non denique intra domesticos parietes pertimescemus.",
    "Loco ille motus est, cum est ex urbe depulsus.",
    "Palam iam cum hoste nullo inpediente bellum iustum geremus.",
    "Utinam ille omnis secum suas copias eduxisset!",
    "Ne illi vehementer errant, si illam meam pristinam lenitatem perpetuam sperant futuram.",
    "Non est iam lenitati locus; severitatem res ipsa flagitat.",
    "Demonstrabo iter: Aurelia via profectus est; si accelerare volent, ad vesperam consequentur.",
    "O fortunatam rem publicam, si quidem hanc sentinam urbis eiecerit!",
    "Uno mehercule Catilina exhausto levata mihi et recreata res publica videtur.",
    "Quid enim mali aut sceleris fingi aut cogitari potest, quod non ille conceperit?",
    "Non enim iam sunt mediocres hominum lubidines, non humanae ac tolerandae audaciae; nihil cogitant nisi caedem, nisi incendia, nisi rapinas.",
    "Sic enim iam tecum loquar, non ut odio permotus esse videar, quo debeo, sed ut misericordia, quae tibi nulla debetur.",
    "Quae nota domesticae turpitudinis non inusta vitae tuae est?",
    "Sed quam longe videtur a carcere atque a vinculis abesse debere, qui se ipse iam dignum custodia iudicarit!",
    NIHIL
};

/* scrinium: (datum, mensura) - mensura explicita quia U+0000 */
nomen structura {
    constans character* datum;
                   i32  mensura;
} FictioDifficile;

hic_manens constans FictioDifficile _difficilia[] = {
    { "", 0 },                                                 /* vacua */
    { "e\xcc\x81t\xc3\xa9", 6 },                               /* nota combinans + praecomposita */
    { "\xf0\x9d\x94\x99 \xf0\x9f\x98\x80", 9 },                /* non-BMP */
    { "\xd7\xa9\xd7\x9c\xd7\x95\xd7\x9d abc", 12 },            /* dextrorsum + sinistrorsum */
    { "\"virgulae\" \\obliqua\\", 21 },                        /* " et \ */
    { "a\0b", 3 },                                             /* U+0000 */
    { "  \t \n ", 6 },                                         /* spatia sola */
    { "\xf0\x9f\x91\x8d\xf0\x9f\x8f\xbd", 8 },                 /* emoji + modificator */
    { "\xef\xbb\xbf" "BOM", 6 },                               /* U+FEFF */
    { NIHIL, 0 }                                               /* longus: computatus */
};
#define FICTIO_DIFFICILIA  X
#define FICTIO_LONGUS_RUNAE MM

/* ---- sors in LXIV bits ---- */

interior i64
_u64 (
    Sors* s)
{
    i64 alta   = (i64)sors_proximum(s);
    i64 bassa  = (i64)sors_proximum(s);

    redde (alta << XXXII) | bassa;
}

s64
fictio_integer (
    Sors* sors,
     s64  minimum,
     s64  maximum)
{
    i64 spatium;
    i64 limen;
    i64 r;

    si (minimum >= maximum)
    {
        redde minimum;
    }
    spatium = (i64)maximum - (i64)minimum + I;   /* 0 = totum spatium */
    si (spatium == 0)
    {
        redde (s64)_u64(sors);
    }
    limen = (0 - spatium) % spatium;              /* Lemire/OpenBSD */
    fac
    {
        r = _u64(sors);
    } dum (r < limen);
    redde (s64)((i64)minimum + (r % spatium));
}

f64
fictio_numerus (
    Sors* sors,
     f64  minimum,
     f64  maximum)
{
    si (minimum >= maximum)
    {
        redde minimum;
    }
    redde minimum + (maximum - minimum) * sors_f64(sors);
}

constans character*
fictio_eligere (
    Sors* sors,
    constans character* constans* optiones)
{
    i32 numerus = 0;

    si (!optiones)
    {
        redde NIHIL;
    }
    dum (optiones[numerus])
    {
        numerus++;
    }
    si (numerus == 0)
    {
        redde NIHIL;
    }
    redde optiones[sors_intra(sors, numerus)];
}

/* ---- nomina et inscriptiones ---- */

chorda
fictio_nomen (
       Sors* sors,
    Piscina* piscina)
{
     ChordaAedificator* aed = chorda_aedificator_creare(piscina, LXIV);
    constans character* gens = fictio_eligere(sors, _nomina);

    si (sors_casu(sors, II, III))
    {
        chorda_aedificator_appendere_literis(aed, fictio_eligere(sors,
            _praenomina));
        chorda_aedificator_appendere_character(aed, ' ');
        chorda_aedificator_appendere_literis(aed, gens);
    }
    alioquin
    {
        /* femina: nomen gentile in -a + ordo ("Tullia Minor") */
        i32 m = (i32)strlen(gens);
        i32 k;

        per (k = 0; k + II < m; k++)
        {
            chorda_aedificator_appendere_character(aed, gens[k]);
        }
        chorda_aedificator_appendere_literis(aed, "a ");
        chorda_aedificator_appendere_literis(aed,
            fictio_eligere(sors, _ordines_feminarum));
    }
    redde chorda_aedificator_finire(aed);
}

chorda
fictio_email (
       Sors* sors,
    Piscina* piscina)
{
               chorda appellatio = fictio_nomen(sors,
                   piscina);
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, LXIV);
                  i32  i;

    per (i = 0; i < appellatio.mensura; i++)
    {
        character c = (character)appellatio.datum[i];

        si (c == ' ')
        {
            c = '.';
        }
        alioquin si (c >= 'A' && c <= 'Z')
        {
            c = (character)(c - 'A' + 'a');
        }
        chorda_aedificator_appendere_character(aed, c);
    }
    chorda_aedificator_appendere_character(aed, '@');
    chorda_aedificator_appendere_literis(aed, fictio_eligere(sors,
        _dominia));
    redde chorda_aedificator_finire(aed);
}

chorda
fictio_uuid (
       Sors* sors,
    Piscina* piscina)
{
           i8 o[XVI];
    character buffer[XXXVII];
          i32 i;
          i32 k = 0;

    per (i = 0; i < XVI; i++)
    {
        o[i] = (i8)(sors_proximum(sors) & 0xFF);
    }
    o[VI]    = (i8)((o[VI] & 0x0F) | 0x40);         /* versio IV */
    o[VIII]  = (i8)((o[VIII] & 0x3F) | 0x80);     /* variatio RFC 4122 */
    per (i = 0; i < XVI; i++)
    {
        si (i == IV || i == VI || i == VIII || i == X)
        {
            buffer[k++] = '-';
        }
        sprintf(buffer + k, "%02x", (insignatus integer)o[i]);
        k += II;
    }
    buffer[k] = '\0';
    redde chorda_ex_literis(buffer, piscina);
}

/* dies civiles ex diebus epochae (Hinnant, days_from_civil inversum) */
interior vacuum
_dies_civiles (
    s64  z,
    s64* annus,
    s64* mensis,
    s64* dies)
{
    s64 era;
    s64 dies_aerae;
    s64 annus_aerae;
    s64 dies_anni;
    s64 mp;

    z           += 719468;
    era         = (z >= 0 ? z : z - 146096) / 146097;
    dies_aerae  = z - era * 146097;
    annus_aerae = (dies_aerae - dies_aerae / 1460 + dies_aerae / 36524
        - dies_aerae / 146096) / 365;
    dies_anni = dies_aerae - (365 * annus_aerae + annus_aerae / 4
        - annus_aerae / 100);
    mp       = (5 * dies_anni + 2) / 153;
    *dies    = dies_anni - (153 * mp + 2) / 5 + 1;
    *mensis  = mp < 10 ? mp + 3 : mp - 9;
    *annus   = annus_aerae + era * 400 + (*mensis <= 2 ? 1 : 0);
}

chorda
fictio_tempus (
       Sors* sors,
        s64  ab,
        s64  ad,
    Piscina* piscina)
{
          s64 t        = fictio_integer(sors, ab, ad);
          s64 dies_ep  = t >= 0 ? t / 86400 : (t - 86399) / 86400;
          s64 sec      = t - dies_ep * 86400;
          s64 annus;
          s64 mensis;
          s64 dies;
    character buffer[LXIV];

    _dies_civiles(dies_ep, &annus, &mensis, &dies);
    sprintf(buffer, "%04ld-%02ld-%02ldT%02ld:%02ld:%02ldZ",
            (longus)annus, (longus)mensis, (longus)dies,
            (longus)(sec / 3600), (longus)((sec % 3600) / 60),
            (longus)(sec % 60));
    redde chorda_ex_literis(buffer, piscina);
}

chorda
fictio_textus_latinus (
       Sors* sors,
        i32  sententiae,
    Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, CCLVI);
                  i32  i;

    per (i = 0; i < (sententiae < I ? I : sententiae); i++)
    {
        si (i > 0)
        {
            chorda_aedificator_appendere_character(aed, ' ');
        }
        chorda_aedificator_appendere_literis(aed, fictio_eligere(sors,
            _corpus));
    }
    redde chorda_aedificator_finire(aed);
}

chorda
fictio_textus (
                  Sors* sors,
                   i32  minimum,
                   i32  maximum,
    constans character* alphabetum,
               Piscina* piscina)
{
    constans character* litterae = alphabetum ? alphabetum
                                          : "abcdefghijklmnopqrstuvwxyz";
                  i32  runae_initia[CCLVI];
                  i32  runae_longitudo[CCLVI];
                  i32  numerus  = 0;
                  i32  m        = (i32)strlen(litterae);
                  i32  i        = 0;
                  i32  longitudo;
    ChordaAedificator* aed;

    dum (i < m && numerus < CCLVI)
    {
        s32 l = utf8_longitudo_byte((i8)litterae[i]);

        si (l < I)
        {
            l = I;
        }
        runae_initia[numerus]     = i;
        runae_longitudo[numerus]  = (i32)l;
        numerus++;
        i += (i32)l;
    }
    longitudo = (i32)fictio_integer(sors, (s64)minimum, (s64)maximum);
    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(longitudo * IV + I));
    per (i = 0; numerus > 0 && i < longitudo; i++)
    {
        i32 r = sors_intra(sors, numerus);
        i32 k;

        per (k = 0; k < runae_longitudo[r]; k++)
        {
            chorda_aedificator_appendere_character(aed,
                litterae[runae_initia[r] + k]);
        }
    }
    redde chorda_aedificator_finire(aed);
}

i32
fictio_difficilia_numerus (vacuum)
{
    redde FICTIO_DIFFICILIA;
}

chorda
fictio_difficile (
        i32  index,
    Piscina* piscina)
{
    chorda c;

    si (index >= FICTIO_DIFFICILIA)
    {
        index = 0;
    }
    si (_difficilia[index].datum == NIHIL)
    {
        /* longus: sententia prima repetita usque ad MM runas */
        ChordaAedificator* aed = chorda_aedificator_creare(piscina, MM
            + C);
        constans character* fons = _corpus[0];
                       i32  i;

        per (i = 0; i < FICTIO_LONGUS_RUNAE; i++)
        {
            chorda_aedificator_appendere_character(aed, fons[i
                % (i32)strlen(fons)]);
        }
        redde chorda_aedificator_finire(aed);
    }
    c.mensura  = _difficilia[index].mensura;
    c.datum    = (i8*)piscina_allocare(piscina, (i64)c.mensura + I);
    si (c.mensura > 0)
    {
        memcpy(c.datum, _difficilia[index].datum, (size_t)c.mensura);
    }
    redde c;
}

chorda
fictio_textus_difficilis (
       Sors* sors,
    Piscina* piscina)
{
    redde fictio_difficile(sors_intra(sors, FICTIO_DIFFICILIA),
        piscina);
}
