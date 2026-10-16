/* probatio_unitates.c - UNITATES CODICIS (spec unitates-codicis, U3
 * gradus 2b): lector politicae et declarationum, unitas intima,
 * hereditas curae et vitae, munera ordinata, provenientia pigra,
 * iudex, recusationes - per suturam fictam (plagulae in memoria).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "credo.h"
#include "unitates.h"
#include <stdio.h>
#include <string.h>

/* credo CONTINET/INCIPIT chordam poscunt: litterae per piscinam
 * sectionis */
#define CONTINET_LITERIS(s, litterae) \
    CREDO_CHORDA_CONTINET((s), \
        chorda_ex_literis((litterae), c->piscina))
#define INCIPIT_LITERIS(s, litterae) \
    CREDO_CHORDA_INCIPIT((s), \
        chorda_ex_literis((litterae), c->piscina))

/* plagula ficta: textus brevis AUT lineae (litterae C89 > DIX
 * octetis vetitae - politica per lineas componitur) */
nomen structura {
             constans character* via;
             constans character* textus;
    constans character* constans* lineae;
} PlagulaFicta;

nomen structura {
    constans PlagulaFicta* plagulae;   /* terminus via NIHIL */
} MundusUnitatum;

interior Xar*
_enumerare (
     vacuum* datum,
    Piscina* piscina)
{
    constans MundusUnitatum* m;
                        Xar* viae;
                        i32  i;

    m     = (constans MundusUnitatum*)datum;
    viae  = xar_creare(piscina, (i32)magnitudo(chorda));

    per (i = ZEPHYRUM; m->plagulae[i].via != NIHIL; i++)
    {
        *(chorda*)xar_addere(viae) = chorda_ex_literis(
            m->plagulae[i].via, piscina);
    }
    redde viae;
}

interior b32
_legere (
      vacuum* datum,
      chorda  via,
     Piscina* piscina,
      chorda* textus)
{
    constans MundusUnitatum* m = (constans MundusUnitatum*)datum;
                        i32  i;
                        i32  j;

    per (i = ZEPHYRUM; m->plagulae[i].via != NIHIL; i++)
    {
        constans PlagulaFicta* f = &m->plagulae[i];
            ChordaAedificator* a;

        si (!chorda_aequalis_literis(via, f->via))
        {
            perge;
        }
        si (f->lineae == NIHIL)
        {
            *textus = chorda_ex_literis(f->textus, piscina);
            redde VERUM;
        }
        a = chorda_aedificator_creare(piscina, (memoriae_index)MXXIV);
        per (j = ZEPHYRUM; f->lineae[j] != NIHIL; j++)
        {
            chorda_aedificator_appendere_literis(a, f->lineae[j]);
            chorda_aedificator_appendere_character(a, '\n');
        }
        *textus = chorda_aedificator_finire(a);
        redde VERUM;
    }
    redde FALSUM;
}

hic_manens constans character* constans POLITICA[] = {
    "<unitates>",
    "  <cura titulus=\"domestica\" descriptio=\"d\"/>",
    "  <cura titulus=\"remissa\" descriptio=\"r\"/>",
    "  <cura titulus=\"aliena\" descriptio=\"a\"/>",
    "  <vita titulus=\"viva\" descriptio=\"v\"/>",
    "  <vita titulus=\"vetusta\" descriptio=\"v\"/>",
    "  <vita titulus=\"archivata\" descriptio=\"a\"/>",
    "  <usus titulus=\"homo\" descriptio=\"h\"/>",
    "  <usus titulus=\"agens\" descriptio=\"a\"/>",
    "  <genus via=\"/elementa_programmatis/bibliotheca\""
        " cura=\"domestica\"/>",
    "  <genus via=\"/elementa_programmatis/applicatio\""
        " cura=\"domestica\"/>",
    "  <genus via=\"/elementa_programmatis/subsystema\""
        " cura=\"domestica\"/>",
    "  <genus via=\"/elementa_programmatis/opus_explorationis\""
        " cura=\"remissa\"/>",
    "  <genus via=\"/fabricatio/adscitio\" cura=\"aliena\"/>",
    "  <munus titulus=\"fixtura\" segmentum=\"fixa\"/>",
    "  <munus titulus=\"assets\" segmentum=\"assets\""
        " segmenti_suffixum=\"_assets\"/>",
    "  <munus titulus=\"probatio\" segmentum=\"probationes\""
        " extensiones=\".c .m .h\"/>",
    "  <munus titulus=\"fixtura\" segmentum=\"probationes\"/>",
    "  <munus titulus=\"caput\" extensiones=\".h\"/>",
    "  <munus titulus=\"fons\" extensiones=\".c .m\"/>",
    "  <munus titulus=\"documentum\" extensiones=\".md .html\"/>",
    "  <munus titulus=\"declaratio\" extensiones=\".stml\"/>",
    "  <munus titulus=\"scriptum\" extensiones=\".sh\"/>",
    "  <lingua titulus=\"c89\" extensiones=\".c .h\"/>",
    "  <lingua titulus=\"objc\" extensiones=\".m\"/>",
    "  <lingua titulus=\"md\" extensiones=\".md\"/>",
    "  <lingua titulus=\"sh\" extensiones=\".sh\"/>",
    "  <lingua titulus=\"stml\" extensiones=\".stml\"/>",
    "  <iudex titulus=\"lint\" curae=\"domestica\""
        " vitae=\"viva vetusta\" munera=\"fons caput probatio\"",
    "         provenientiae=\"manu\" linguae=\"c89 objc\""
        " descriptio=\"l\"/>",
    "  <bibliothecae capita=\"include\" fontes=\"lib\""
        " probationes=\"probationes\" praefixum=\"probatio_\"",
    "                partes=\"_macos _posix\""
        " paginae=\"docs/bibliothecae\" bona=\"_assets\"/>",
    "  <iudex titulus=\"omnia\" curae=\"domestica\""
        " vitae=\"viva\" munera=\"fons\"",
    "         provenientiae=\"manu generatum\" linguae=\"c89\""
        " descriptio=\"o\"/>",
    "  <exspectatio genus=\"/elementa_programmatis/bibliotheca\""
        " munus=\"probatio\" minimum=\"1\" descriptio=\"probatio\"/>",
    "  <exspectatio genus=\"/elementa_programmatis/applicatio\""
        " attributum=\"usus\" descriptio=\"usus\"/>",
    "  <familia radix=\"imago\"/>",
    "</unitates>",
    NIHIL
};

#define SUB "/elementa_programmatis/subsystema"
#define APP "/elementa_programmatis/applicatio"

/* mundus sanus: radix, duae applicationes, vendor, knotapel,
 * archivum, bibliothecae cum partibus et familia */
hic_manens constans PlagulaFicta MUNDUS[] = {
    { "unitates.stml", NIHIL, POLITICA },
    { "unitas.stml", "<unitas titulus=\"rhubarb\" genus=\"" SUB "\""
        " descriptio=\"r\"/>", NIHIL },
    { "apps/vicus/unitas.stml", "<unitas titulus=\"vicus\" genus=\""
        APP "\" usus=\"homo\" descriptio=\"v\"/>", NIHIL },
    { "apps/vicus/vicus.c", "/* vicus */\n", NIHIL },
    { "apps/pictor/unitas.stml", "<unitas titulus=\"pictor\" genus=\""
        APP "\" descriptio=\"p\"/>", NIHIL },
    { "vendor/unitas.stml", "<unitas titulus=\"vendor\""
        " genus=\"/fabricatio/adscitio\" descriptio=\"v\">\n"
        "  <origo via=\"sqlite3.c\" fons=\"f\" versio=\"I\""
        " licentia=\"l\"/>\n</unitas>", NIHIL },
    { "vendor/sqlite3.c", "/* alienum */\n", NIHIL },
    { "knotapel/unitas.stml", "<unitas titulus=\"knotapel\""
        " genus=\"/elementa_programmatis/opus_explorationis\""
        " descriptio=\"k\"/>", NIHIL },
    { "knotapel/demo.c", "/* demo */\n", NIHIL },
    { "archivum/unitas.stml", "<unitas titulus=\"archivum\" genus=\""
        SUB "\" vita=\"archivata\" descriptio=\"a\"/>", NIHIL },
    { "archivum/probationes/probatio_vetus.c", "/* vetus */\n", NIHIL },
    { "archivum/materia/unitas.stml", "<unitas titulus=\"materia\""
        " genus=\"" SUB "\" descriptio=\"m\"/>", NIHIL },
    { "archivum/materia/vetus.c", "/* vetus */\n", NIHIL },
    { "archivum/app/unitas.stml", "<unitas titulus=\"app\" genus=\"" APP
        "\" descriptio=\"a\"/>", NIHIL },
    { "ora/unitas.stml", "<unitas titulus=\"ora\" genus=\"" SUB "\""
        " cura=\"aliena\" descriptio=\"o\"/>", NIHIL },
    { "ora/clay/unitas.stml", "<unitas titulus=\"clay\" genus=\"" SUB
        "\" descriptio=\"c\"/>", NIHIL },
    { "ora/clay/vinculum.c", "/* c99 */\n", NIHIL },
    { "include/xar.h", "/* xar */\n", NIHIL },
    { "lib/xar.c", "/* xar.c - manu */\n", NIHIL },
    { "lib/xar.worklog.md", "# xar\n", NIHIL },
    { "lib/energia.phase-log.md", "# energia\n", NIHIL },
    { "lib/xar.interrogationes.stml", "<interrogationes/>\n", NIHIL },
    { "lib/xar.stml", "<x/>\n", NIHIL },
    { "docs/bibliothecae/xar.html", "<!doctype html>\n<p>xar</p>\n",
        NIHIL },
    { "docs/bibliothecae/index.html", "<!doctype html>\n<p>i</p>\n",
        NIHIL },
    { "lib/xar_assets/a.js", "var a;\n", NIHIL },
    { "probationes/q_assets/c.c", "/* capsula */\n", NIHIL },
    { "probationes/probatio_xar.c", "/* p */\n", NIHIL },
    { "probationes/fixa/xar/datum.c", "/* fixum */\n", NIHIL },
    { "probationes/pictor_commune.h", "/* auxilium */\n", NIHIL },
    { "include/energia.h", "/* e */\n", NIHIL },
    { "lib/energia_macos.c", "/* e */\n", NIHIL },
    { "lib/energia_posix.c", "/* e */\n", NIHIL },
    { "include/imago.h", "/* i */\n", NIHIL },
    { "include/imago_png.h", "/* i */\n", NIHIL },
    { "lib/imago_png.c", "/* i */\n", NIHIL },
    { "include/vicus.h", "/* bibliotheca vicus */\n", NIHIL },
    { "probationes/probatio_vicus_quaestio.c", "/* p */\n", NIHIL },
    { "probationes/probatio_pictor_strata.c", "/* p */\n", NIHIL },
    { "probationes/probatio_combinado.c", "/* p */\n", NIHIL },
    { "silva/amalgama/silva.c", "/* GENERATUM - noli edere */\n"
        "int x;\n", NIHIL },
    { "tools/x.sh", "#!/bin/sh\n", NIHIL },
    { "docs/pagina.html", "<!doctype html>\n<!-- GENERATUM: g.py -->\n"
        "<html>\n", NIHIL },
    { "natura/cocta/a.canon",
        "<?xml version=\"1.0\"?>\n<!-- GENERATUM a "
        "c -->\n<canon/>\n", NIHIL },
    { "tools/norma_canon.h", "/* norma_canon.h - caput manu\n * Corpus "
        "lib/norma_canon.c GENERATUM est per bin/norma */\n", NIHIL },
    { "tools/coctum.c",
        "/*\n * coctum.c - GENERATUM, NE MANU EDITES\n */\n",
        NIHIL },
    { "media/imago.png", "PNG", NIHIL },
    { NIHIL, NIHIL, NIHIL }
};

interior Unitates*
_mundum_legere (
             CredoContextus*  c,
      constans PlagulaFicta*  plagulae,
                     chorda*  causa,
                        Xar** recusationes)
{
    /* in piscina sectionis: Unitates suturam SERVAT (provenientia
     * pigre per eam legitur) - locale pendens esset, staticum status
     * inter sectiones */
    MundusUnitatum* m;
    UnitatesSutura* s;

    m = (MundusUnitatum*)piscina_allocare(c->piscina,
        (memoriae_index)magnitudo(MundusUnitatum));
    s = (UnitatesSutura*)piscina_allocare(c->piscina,
        (memoriae_index)magnitudo(UnitatesSutura));
    m->plagulae            = plagulae;
    s->datum               = m;
    s->plagulas_enumerare  = _enumerare;
    s->plagulam_legere     = _legere;
    redde unitates_legere(s, c->piscina, causa, recusationes);
}

interior Unitates*
_sanum (
    CredoContextus* c)
{
      chorda  causa;
    Unitates* u = _mundum_legere(c, MUNDUS, &causa, NIHIL);

    si (u == NIHIL)
    {
        imprimere("recusatio inexspectata:\n%.*s\n",
            (integer)causa.mensura,
            (character*)causa.datum);
    }
    redde u;
}

interior UnitatisResponsum
_quid (
          CredoContextus* c,
    constans Unitates*    u,
     constans character*  via)
{
    UnitatisResponsum r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    CREDO_VERUM(unitates_quid(u, chorda_ex_literis(via, c->piscina),
        c->piscina, &r));
    redde r;
}

/* I. lectio sana: XV unitates (X directorii, V bibliothecae; partes
 * energia_macos/posix non unitates) */
interior vacuum
_sectio_lectio (
    CredoContextus* c)
{
        Unitates* u = _sanum(c);
    constans Xar* omnes;
             i32  directorii    = ZEPHYRUM;
             i32  bibliothecae  = ZEPHYRUM;
             i32  i;

    CREDO_NECESSE_NON_NIHIL(u);
    omnes = unitates_omnes(u);
    per (i = ZEPHYRUM; i < xar_numerus(omnes); i++)
    {
        constans Unitas* x = *(Unitas**)xar_obtinere(omnes, i);

        si (x->fons == UNITAS_DIRECTORII)
        {
            directorii++;
        }
        alioquin
        {
            bibliothecae++;
            CREDO_FALSUM(chorda_aequalis_literis(x->titulus,
                "energia_macos"));
        }
    }
    CREDO_AEQUALIS_I32(directorii, X);
    CREDO_AEQUALIS_I32(bibliothecae, V);
}

/* II. unitas intima: directorium, stemma, pars, familia, probatio per
 * praefixum, applicatio et bibliotheca eiusdem tituli */
interior vacuum
_sectio_intima (
    CredoContextus* c)
{
             Unitates* u = _sanum(c);
    UnitatisResponsum  r;
      constans Unitas* bib_vicus;

    CREDO_NECESSE_NON_NIHIL(u);

    r = _quid(c, u, "apps/vicus/vicus.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "vicus");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->genus, APP);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->parens->titulus, "rhubarb");
    CONTINET_LITERIS(r.unitatis_causa,
        "directorium apps/vicus (apps/vicus/unitas.stml)");

    r = _quid(c, u, "include/xar.h");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "xar");
    CREDO_AEQUALIS_I32((i32)r.unitas->fons, (i32)UNITAS_BIBLIOTHECAE);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->declaratio,
        "stemma include/xar.h");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->parens->titulus, "rhubarb");

    r = _quid(c, u, "lib/xar.worklog.md");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "xar");

    r = _quid(c, u, "./probationes/probatio_xar.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "xar");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitatis_causa,
        "probatio stemmatis xar");

    r = _quid(c, u, "lib/energia_macos.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "energia");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitatis_causa,
        "stemma energia_macos pars bibliothecae energia");

    r = _quid(c, u, "lib/imago_png.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "imago_png");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->parens->titulus, "imago");

    /* bibliotheca vicus vincit applicationem vicus (praefixum
     * aequale), unitas alia */
    r          = _quid(c, u, "probationes/probatio_vicus_quaestio.c");
    bib_vicus  = r.unitas;
    CREDO_CHORDA_AEQUALIS_LITERIS(bib_vicus->titulus, "vicus");
    CREDO_AEQUALIS_I32((i32)bib_vicus->fons, (i32)UNITAS_BIBLIOTHECAE);
    r = _quid(c, u, "apps/vicus/vicus.c");
    CREDO_VERUM(r.unitas != bib_vicus);

    r = _quid(c, u, "probationes/probatio_pictor_strata.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "pictor");
    INCIPIT_LITERIS(r.unitatis_causa, "probatio unitatis pictor");

    /* FONTES/S.<quidvis>.md, PAGINAE/S.html, FONTES/S_assets/ ad S;
     * index.html nullius stemmatis radici cadit */
    r = _quid(c, u, "lib/energia.phase-log.md");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "energia");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.valor, "documentum");
    /* FONTES/S.<quidvis>.stml (interrogationes) ad S; S.stml sine
     * medio stemma non est */
    r = _quid(c, u, "lib/xar.interrogationes.stml");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "xar");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.valor, "declaratio");
    r = _quid(c, u, "lib/xar.stml");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "rhubarb");
    r = _quid(c, u, "docs/bibliothecae/xar.html");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "xar");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.valor, "documentum");
    r = _quid(c, u, "docs/bibliothecae/index.html");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "rhubarb");
    r = _quid(c, u, "lib/xar_assets/a.js");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "xar");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.valor, "assets");

    /* suffixum segmenti: probationes/q_assets/ assets, non probatio */
    r = _quid(c, u, "probationes/q_assets/c.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.valor, "assets");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.causa,
        "regula munus 2 (segmentum assets|*_assets)");

    r = _quid(c, u, "probationes/probatio_combinado.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "rhubarb");
    r = _quid(c, u, "tools/x.sh");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "rhubarb");

    /* declaratio unitatis continentis est: vendor/unitas.stml nostra
     * (canon eam iudicat), radicis declaratio radicis manet */
    r = _quid(c, u, "vendor/unitas.stml");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "rhubarb");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.cura.valor, "domestica");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitatis_causa,
        "declaratio unitatis vendor - continentis rhubarb");
    r = _quid(c, u, "unitas.stml");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "rhubarb");

    /* via absoluta aut vacua: FALSUM */
    CREDO_FALSUM(unitates_quid(u, chorda_ex_literis("/etc/x",
        c->piscina), c->piscina, &r));
    CREDO_FALSUM(unitates_quid(u, chorda_ex_literis("", c->piscina),
        c->piscina, &r));
}

/* III. hereditas: cura generis (vendor, knotapel), vita et cura
 * MAIORIS (unitas intra archivum, intra ora), praestituta (viva) */
interior vacuum
_sectio_hereditas (
    CredoContextus* c)
{
             Unitates* u = _sanum(c);
    UnitatisResponsum  r;

    CREDO_NECESSE_NON_NIHIL(u);

    r = _quid(c, u, "vendor/sqlite3.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.cura.valor, "aliena");
    INCIPIT_LITERIS(r.cura.causa, "genus /fabricatio/adscitio");

    r = _quid(c, u, "knotapel/demo.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.cura.valor, "remissa");

    r = _quid(c, u, "archivum/probationes/probatio_vetus.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.vita.valor, "archivata");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.vita.causa, "archivum/unitas.stml");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.cura.valor, "domestica");

    /* unitas intra archivum: vita MAIORIS (nulla sua) */
    r = _quid(c, u, "archivum/materia/vetus.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "materia");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.vita.valor, "archivata");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.vita.causa, "archivum/unitas.stml");

    /* cura declarata maioris vincit praestitutum generis intimae */
    r = _quid(c, u, "ora/clay/vinculum.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "clay");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->parens->titulus, "ora");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.cura.valor, "aliena");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.cura.causa, "ora/unitas.stml");

    r = _quid(c, u, "lib/xar.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.cura.valor, "domestica");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.vita.valor, "viva");
    INCIPIT_LITERIS(r.vita.causa, "praestitutum");
}

/* IV. munera ordine (fixa ante probationes), linguae per extensionem,
 * extensio ignota nominata */
interior vacuum
_sectio_munera (
    CredoContextus* c)
{
             Unitates* u = _sanum(c);
    UnitatisResponsum  r;

    CREDO_NECESSE_NON_NIHIL(u);

    r = _quid(c, u, "probationes/fixa/xar/datum.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.valor, "fixtura");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.causa,
        "regula munus 1 (segmentum fixa)");

    r = _quid(c, u, "probationes/probatio_xar.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.valor, "probatio");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.causa,
        "regula munus 3 (segmentum probationes, .c .m .h)");

    /* caput auxiliare probationum = probatio, non fixtura (lint
     * hodie iudicat; regula prima solum .c .m dabat - inventum
     * contra arborem veram) */
    r = _quid(c, u, "probationes/pictor_commune.h");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.valor, "probatio");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.lingua.valor, "c89");

    r = _quid(c, u, "include/xar.h");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.valor, "caput");
    r = _quid(c, u, "lib/xar.worklog.md");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.valor, "documentum");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.lingua.valor, "md");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.lingua.causa, "extensio .md");
    r = _quid(c, u, "unitas.stml");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.valor, "declaratio");

    r = _quid(c, u, "media/imago.png");
    CREDO_CHORDA_VACUA(r.munus.valor);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.munus.causa,
        "nulla regula munus congruit");
    CREDO_CHORDA_VACUA(r.lingua.valor);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.lingua.causa, "extensio ignota");
}

/* V. provenientia ex linea prima; plagula absens nominata */
interior vacuum
_sectio_provenientia (
    CredoContextus* c)
{
             Unitates* u = _sanum(c);
    UnitatisResponsum  r;

    CREDO_NECESSE_NON_NIHIL(u);

    r = _quid(c, u, "silva/amalgama/silva.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.provenientia.valor, "generatum");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.provenientia.causa,
        "linea I: GENERATUM");

    r = _quid(c, u, "lib/xar.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.provenientia.valor, "manu");

    /* post declarationem quae lineam primam poscit: linea secunda */
    r = _quid(c, u, "docs/pagina.html");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.provenientia.valor, "generatum");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.provenientia.causa,
        "linea II (post declarationem): GENERATUM");
    r = _quid(c, u, "natura/cocta/a.canon");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.provenientia.valor, "generatum");

    /* positio FIXA, non fenestra: caput quod GENERATUM in prosa linea
     * II nominat manu est; signum linea III (commentum solum in linea
     * I) NON valet - generator lineam primam signare debet */
    r = _quid(c, u, "tools/norma_canon.h");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.provenientia.valor, "manu");
    r = _quid(c, u, "tools/coctum.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.provenientia.valor, "manu");

    r = _quid(c, u, "lib/nusquam.c");
    CREDO_CHORDA_VACUA(r.provenientia.valor);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.provenientia.causa,
        "plagula legi nequit");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.unitas->titulus, "rhubarb");
}

/* VI. iudex: iudicat, proprietas prima excludens cum causa sua,
 * iudex ignotus cum notis */
interior vacuum
_sectio_iudex (
    CredoContextus* c)
{
    Unitates* u = _sanum(c);
      chorda  lint;
      chorda  causa;

    CREDO_NECESSE_NON_NIHIL(u);
    lint = chorda_ex_literis("lint", c->piscina);

    CREDO_AEQUALIS_I32((i32)unitates_iudicat(u, lint, chorda_ex_literis(
        "lib/xar.c", c->piscina), c->piscina, &causa),
        (i32)UNITATES_IUDICAT);
    CREDO_CHORDA_VACUA(causa);

    CREDO_AEQUALIS_I32((i32)unitates_iudicat(u, lint, chorda_ex_literis(
        "vendor/sqlite3.c", c->piscina), c->piscina, &causa),
        (i32)UNITATES_NON_IUDICAT);
    CREDO_CHORDA_AEQUALIS_LITERIS(causa, "cura aliena (genus "
        "/fabricatio/adscitio (unitates.stml)) non in curae "
        "iudicis lint");

    CREDO_AEQUALIS_I32((i32)unitates_iudicat(u, lint, chorda_ex_literis(
        "archivum/probationes/probatio_vetus.c", c->piscina),
        c->piscina,
        &causa), (i32)UNITATES_NON_IUDICAT);
    INCIPIT_LITERIS(causa, "vita archivata (archivum/unitas.stml)");

    CREDO_AEQUALIS_I32((i32)unitates_iudicat(u, lint, chorda_ex_literis(
        "probationes/fixa/xar/datum.c", c->piscina), c->piscina,
        &causa),
        (i32)UNITATES_NON_IUDICAT);
    INCIPIT_LITERIS(causa, "munus fixtura");

    CREDO_AEQUALIS_I32((i32)unitates_iudicat(u, lint, chorda_ex_literis(
        "silva/amalgama/silva.c", c->piscina), c->piscina, &causa),
        (i32)UNITATES_NON_IUDICAT);
    INCIPIT_LITERIS(causa, "provenientia generatum");

    CREDO_AEQUALIS_I32((i32)unitates_iudicat(u, lint, chorda_ex_literis(
        "media/imago.png", c->piscina), c->piscina, &causa),
        (i32)UNITATES_NON_IUDICAT);
    INCIPIT_LITERIS(causa, "munus nullum (nulla regula");

    CREDO_AEQUALIS_I32((i32)unitates_iudicat(u, chorda_ex_literis(
        "formator", c->piscina), chorda_ex_literis("lib/xar.c",
        c->piscina), c->piscina, &causa), (i32)UNITATES_IUDEX_IGNOTUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(causa,
        "iudex 'formator' ignotus (noti: lint, omnia)");

    /* iudex provenientias omnes admittens plagulam NON legit: via
     * illegibilis iudicatur (lint indicem MMC viarum sine lectione
     * filtrat) */
    CREDO_AEQUALIS_I32((i32)unitates_iudicat(u, chorda_ex_literis(
        "omnia", c->piscina), chorda_ex_literis("lib/nusquam.c",
        c->piscina), c->piscina, &causa), (i32)UNITATES_IUDICAT);
    CREDO_AEQUALIS_I32((i32)unitates_iudicat(u, lint, chorda_ex_literis(
        "lib/nusquam.c", c->piscina), c->piscina, &causa),
        (i32)UNITATES_NON_IUDICAT);
    INCIPIT_LITERIS(causa, "provenientia nullum (plagula legi nequit)");
}

/* IX. lista viarum filtrata (corpus_tractatae.txt): eadem forma, '\r'
 * et lineae vacuae tolerantur, iudex ignotus nominatur */
interior vacuum
_sectio_viae_iudicatae (
    CredoContextus* c)
{
    Unitates* u = _sanum(c);
      chorda  r;
         b32  ignotus = VERUM;

    CREDO_NECESSE_NON_NIHIL(u);
    r = unitates_viae_iudicatae(u, chorda_ex_literis("lint",
        c->piscina),
        chorda_ex_literis("vendor/sqlite3.c\nlib/xar.c\r\n\n"
            "probationes/fixa/xar/datum.c\ninclude/xar.h", c->piscina),
        c->piscina, &ignotus);
    CREDO_FALSUM(ignotus);
    CREDO_CHORDA_AEQUALIS_LITERIS(r, "lib/xar.c\ninclude/xar.h\n");

    r = unitates_viae_iudicatae(u, chorda_ex_literis("nusquam",
        c->piscina), chorda_ex_literis("lib/xar.c\n", c->piscina),
        c->piscina, &ignotus);
    CREDO_VERUM(ignotus);
    CREDO_CHORDA_VACUA(r);
}

/* VII. recusationes OMNES simul, cum valoribus notis */
interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    hic_manens constans character* constans politica_mala[] = {
        "<unitates>",
        "  <cura titulus=\"domestica\" descriptio=\"d\"/>",
        "  <vita titulus=\"viva\" descriptio=\"v\"/>",
        "  <vita titulus=\"vetusta\" descriptio=\"v\"/>",
        "  <usus titulus=\"homo\" descriptio=\"h\"/>",
        "  <genus via=\"" SUB "\" cura=\"domestica\"/>",
        "  <genus via=\"" APP "\" cura=\"stricta\"/>",
        "  <munus titulus=\"fons\" extensiones=\".c\"/>",
        "  <munus titulus=\"nihil\"/>",
        "  <lingua titulus=\"c89\" extensiones=\".c\"/>",
        "  <iudex titulus=\"lint\" curae=\"domestica\" vitae=\"viva\""
            " munera=\"caput\" provenientiae=\"manu\"",
        "         linguae=\"c89\" descriptio=\"l\"/>",
        "  <bibliothecae capita=\"include\" fontes=\"lib\""
            " probationes=\"probationes\" praefixum=\"probatio_\""
            " partes=\"_macos\"/>",
        "  <familia radix=\"nusquam\"/>",
        "  <exspectatio genus=\"/x\" munus=\"nihil\""
            " attributum=\"usus\" descriptio=\"d\"/>",
        "  <ignotum/>",
        "</unitates>",
        NIHIL
    };
    hic_manens constans PlagulaFicta mundus[] = {
        { "unitates.stml", NIHIL, politica_mala },
        { "unitas.stml", "<unitas titulus=\"rhubarb\" genus=\"" SUB "\""
            " descriptio=\"r\"/>", NIHIL },
        { "a/unitas.stml", "<unitas titulus=\"a\" genus=\"/x/ignotum\""
            " descriptio=\"a\"/>", NIHIL },
        { "b/unitas.stml", "<unitas titulus=\"b\" genus=\"" SUB "\""
            " cura=\"laxa\" vita=\"vetusta\" usus=\"robot\""
            " colores=\"rubri\" descriptio=\"b\"/>", NIHIL },
        { "c/unitas.stml", "<unitas titulus=\"b\" genus=\"" SUB "\""
            " descriptio=\"c\"/>", NIHIL },
        { "d/unitas.stml", "<unitas titulus=\"d\" genus=\"" SUB
            "\"/>", NIHIL },
        { "e/unitas.stml", "<unitas titulus=\"e\" genus=\"" SUB
            "\" descriptio=\"e\"", NIHIL },
        { "include/xar.h", "/* xar */\n", NIHIL },
        { NIHIL, NIHIL, NIHIL }
    };
    hic_manens constans character* constans exspectatae[] = {
        "cura 'stricta' ignotus (noti: domestica)",
        "regula munus 'nihil' vacua",
        "elementum ignotum <ignotum>",
        "munus 'caput' ignotus (noti: fons, nihil)",
        "a/unitas.stml:1: genus '/x/ignotum' ignotus (noti: " SUB ", "
            APP ")",
        "b/unitas.stml:1: attributum ignotum 'colores'",
        "b/unitas.stml:1: cura 'laxa' ignotus (noti: domestica)",
        "b/unitas.stml:1: usus 'robot' ignotus (noti: homo)",
        "b/unitas.stml:1: vita vetusta et successor simul",
        "c/unitas.stml:1: unitas 'b' bis declarata in eodem genere",
        "d/unitas.stml:1: attributum necessarium 'descriptio' abest",
        "e/unitas.stml",
        "genus '/elementa_programmatis/bibliotheca' non citabile",
        "familia 'nusquam' sine bibliotheca propria",
        "exspectatio generis non citabilis '/x'",
        "exspectatio munus AUT attributum nominat",
        NIHIL
    };
      chorda  causa;
         Xar* recusationes = NIHIL;
    Unitates* u;
         i32  i;

    u = _mundum_legere(c, mundus, &causa, &recusationes);
    CREDO_NIHIL(u);
    CREDO_NECESSE_NON_NIHIL(recusationes);
    per (i = ZEPHYRUM; exspectatae[i] != NIHIL; i++)
    {
        CONTINET_LITERIS(causa, exspectatae[i]);
    }
    CREDO_AEQUALIS_I32(xar_numerus(recusationes), (i32)i);
}

/* VIII. radix sine unitate: recusatio nominata, non responsum NIHIL */
interior vacuum
_sectio_radix_absens (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta mundus[] = {
        { "unitates.stml", NIHIL, POLITICA },
        { "apps/vicus/unitas.stml", "<unitas titulus=\"vicus\" genus=\""
            APP "\" descriptio=\"v\"/>", NIHIL },
        { NIHIL, NIHIL, NIHIL }
    };
      chorda  causa;
    Unitates* u = _mundum_legere(c, mundus, &causa, NIHIL);

    CREDO_NIHIL(u);
    CONTINET_LITERIS(causa, "unitas radicis abest");
}

/* X. census: exspectationes per genus (bibliotheca sine probatione,
 * applicatio sine usu), archivata non iudicata, probatio orphana,
 * directorium cum codice indeclaratum */
interior vacuum
_sectio_census (
    CredoContextus* c)
{
             Unitates* u = _sanum(c);
       UnitatesCensus* k;
    ChordaAedificator* a;
                  i32  i;
                  i32  j;

    CREDO_NECESSE_NON_NIHIL(u);
    k = unitates_census(u, c->piscina);
    CREDO_NECESSE_NON_NIHIL(k);
    a = chorda_aedificator_creare(c->piscina, (memoriae_index)CCLVI);
    per (i = ZEPHYRUM; i < xar_numerus(k->unitates); i++)
    {
        constans UnitatisCensus* summarium = (constans UnitatisCensus*)
            xar_obtinere(k->unitates, i);

        per (j = ZEPHYRUM; j < xar_numerus(summarium->desunt); j++)
        {
            chorda_aedificator_appendere_chorda(a,
                summarium->unitas->titulus);
            chorda_aedificator_appendere_character(a, ':');
            chorda_aedificator_appendere_chorda(a,
                *(chorda*)xar_obtinere(summarium->desunt, j));
            chorda_aedificator_appendere_character(a, ' ');
        }
        /* numeri munerum: xar probationem unam habet */
        si (chorda_aequalis_literis(summarium->unitas->titulus, "xar"))
        {
            CREDO_AEQUALIS_I32(xar_numerus(summarium->numeri)
                > ZEPHYRUM, I);
        }
    }
    /* ordine unitates_omnes: directorii (pictor sine usu; app
     * archivata NON), deinde bibliothecae (energia imago imago_png
     * sine probatione; xar et vicus habent) */
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(a),
        "pictor:usus energia:probatio imago:probatio "
        "imago_png:probatio ");

    CREDO_AEQUALIS_I32(xar_numerus(k->orphani), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(k->orphani,
        ZEPHYRUM), "probationes/probatio_combinado.c");

    /* ordine enumerationis: silva/amalgama (politica probationis munus
     * amalgama non habet - fons) et tools (x.sh, coctum.c,
     * norma_canon.h) */
    CREDO_AEQUALIS_I32(xar_numerus(k->indeclarata), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(((UnitatesDirectorium*)xar_obtinere(
        k->indeclarata, ZEPHYRUM))->via, "silva/amalgama");
    CREDO_CHORDA_AEQUALIS_LITERIS(((UnitatesDirectorium*)xar_obtinere(
        k->indeclarata, I))->via, "tools");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "lectio sana", _sectio_lectio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unitas intima", _sectio_intima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "hereditas", _sectio_hereditas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "munera", _sectio_munera, NIHIL, NIHIL, NIHIL, NIHIL },
    { "provenientia", _sectio_provenientia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "iudex", _sectio_iudex, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "radix absens", _sectio_radix_absens, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "viae iudicatae", _sectio_viae_iudicatae, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "census", _sectio_census, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("unitates", SECTIONES);
}
