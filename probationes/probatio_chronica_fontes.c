/* probatio_chronica_fontes.c - chronica C2: eventa omnium librorum
 * (comites et liber communis) in tabulam unam - origo (liber, pagina,
 * linea), titulus libri, ordo anni et intra annum, mutatio paginae,
 * liber novus, tabula recens sine mutatione, personae et loci, clavis
 * longa (ultra inscriptionem conspectus), lineae vacuae servatae. */

#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "scriba_liber.h"
#include "conspectus.h"
#include "chronica_eventus.h"
#include "chronica_fontes.h"

#include <stdio.h>
#include <string.h>


/* ==================================================
 * Mundus
 * ================================================== */

nomen structura {
                Volumen* volumen;
    InternamentumChorda* intern;
    ConspectusRegistrum* repertorium;
         ChronicaFontes* fontes;
                Piscina* piscina;
} Mundus;

interior chorda
lit (
    constans character* s,
               Piscina* piscina)
{
    redde chorda_ex_literis(s, piscina);
}

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* m;
       Xar* vitia;

    m = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    c->datum    = m;
    m->piscina  = c->piscina;
    m->intern   = internamentum_creare(c->piscina);
    m->volumen  = volumen_temporarium(c->piscina,
        "probatio_chronica_fontes");
    si (!m->volumen)
    {
        redde;
    }
    m->repertorium = conspectus_registrum_creare(m->piscina, m->intern,
        m->volumen, ZEPHYRUM);
    m->fontes = chronica_fontes_creare(m->piscina, m->intern,
        m->volumen);
    si (   !m->repertorium || !m->fontes
        || !chronica_registrare(m->repertorium, m->fontes, &vitia)
        || !conspectus_parare(m->repertorium, &vitia))
    {
        m->fontes = NIHIL;
    }
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    si (m && m->fontes)
    {
        chronica_fontes_destruere(m->fontes);
    }
    si (m && m->repertorium)
    {
        conspectus_registrum_destruere(m->repertorium);
    }
    si (m && m->volumen)
    {
        volumen_claudere(m->volumen);
    }
}


/* ==================================================
 * Auxilia
 * ================================================== */

/* pagina libri (praefixum NIHIL = communis) cum textu toto; condita
 * si deest */
interior b32
paginam_scribere (
                 Mundus* m,
     constans character* praefixum,
     constans character* titulus,
     constans character* textus)
{
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    l = praefixum ? scriba_liber_aperire_in_spatio(m->piscina,
        m->intern, m->volumen, praefixum, LXXX, XX)
        : scriba_liber_aperire(m->piscina, m->intern, m->volumen,
        LXXX, XX);
    d = l ? scriba_liber_paginam_condere(l, lit(titulus, m->piscina))
        : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, m->piscina,
        d->latitudo, d->altitudo, textus);
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

interior constans ChronicaRes*
res_ad (
    constans ChronicaTabula* t,
                        i32  index)
{
    redde (constans ChronicaRes*)xar_obtinere(t->res, index);
}

interior s32
annus_ad (
    constans ChronicaTabula* t,
                        i32  index)
{
    redde *(s32*)xar_obtinere(t->anni, index);
}

/* res prima cuius descriptio datur; NIHIL si nulla */
interior constans ChronicaRes*
res_descriptionis (
    constans ChronicaTabula* t,
         constans character* descriptio)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(t->res); i++)
    {
        si (chorda_aequalis_literis(res_ad(t, i)->eventus.descriptio,
                descriptio))
        {
            redde res_ad(t, i);
        }
    }
    redde NIHIL;
}


/* ==================================================
 * Sectiones
 * ================================================== */

/* liber comitis: origo, titulus ex nota, vitiosa omissa */
interior vacuum
_probare_comitem (
    CredoContextus* c)
{
                      Mundus* m;
     constans ChronicaTabula* t;
        constans ChronicaRes* r;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->fontes);
    CREDO_VERUM(paginam_scribere(m,
        "comes/navigator/en.wikipedia.org/Thomas_Aquinas", "1",
        "Doctor angelicus.\n"
        "\n"
        "<event year=\"1274\">Aquinas dies</event>\n"
        "<event month=\"3\">sine anno</event>"));
    CREDO_VERUM(scriba_liber_comitem_notare(m->volumen, m->intern,
        "comes/navigator/en.wikipedia.org/Thomas_Aquinas",
        lit("Notes \302\267 Thomas Aquinas", c->piscina),
        lit("navigator/en.wikipedia.org/Thomas_Aquinas", c->piscina),
        c->piscina));

    CREDO_GRADUS("res una, origo");
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_NECESSE(xar_numerus(t->res) == I);
    r = res_ad(t, ZEPHYRUM);
    CREDO_AEQUALIS_S32(r->eventus.annus, (s32)MCCLXXIV);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->eventus.descriptio,
        "Aquinas dies");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->liber,
        "comes/navigator/en.wikipedia.org/Thomas_Aquinas");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->pagina, "1");
    CREDO_AEQUALIS_I32(r->eventus.linea, II);
    CREDO_AEQUALIS_I32(r->eventus.columna, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus_libri,
        "Notes \302\267 Thomas Aquinas");
    CREDO_AEQUALIS_I32(xar_numerus(t->anni), I);
    CREDO_AEQUALIS_S32(annus_ad(t, ZEPHYRUM), (s32)MCCLXXIV);

    CREDO_GRADUS("nota absens: locus titulus");
    CREDO_VERUM(paginam_scribere(m, "comes/terminale/1_dextrum", "1",
        "<event year=\"1900\">nota sine titulo</event>"));
    t = chronica_tabula(m->fontes);
    r = res_descriptionis(t, "nota sine titulo");
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus_libri,
        "terminale/1_dextrum");
}

/* liber communis: liber vacua, titulus "Notebook" */
interior vacuum
_probare_communem (
    CredoContextus* c)
{
                      Mundus* m;
     constans ChronicaTabula* t;
        constans ChronicaRes* r;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->fontes);
    CREDO_VERUM(paginam_scribere(m, NIHIL, "historia",
        "<event year=\"-1446\" people=\"Moses\">Exodus</event>"));
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_NECESSE(xar_numerus(t->res) == I);
    r = res_ad(t, ZEPHYRUM);
    CREDO_AEQUALIS_S32(r->eventus.annus, -(s32)MCDXLVI);
    CREDO_CHORDA_VACUA(r->liber);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->pagina, "historia");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus_libri, "Notebook");
}

/* anni distincti ascendentes; intra annum datae primum (mensis, dies;
 * dies absens ante diem), deinde ordine libri: communis primum */
interior vacuum
_probare_ordinem (
    CredoContextus* c)
{
                     Mundus* m;
    constans ChronicaTabula* t;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->fontes);
    CREDO_VERUM(paginam_scribere(m, "comes/sancti/sanctus/1", "1",
        "<event year=\"1274\">B sine die</event>\n"
        "<event year=\"1274\" month=\"7\" day=\"15\">Bonaventura</>\n"
        "<event year=\"1225\">Thomas nascitur</event>\n"
        "<event year=\"1274\" month=\"3\" day=\"7\">Thomas</event>\n"
        "<event year=\"1274\" month=\"7\">Lugdunum</event>"));
    CREDO_VERUM(paginam_scribere(m, NIHIL, "historia",
        "<event year=\"1274\">A sine die</event>\n"
        "<event year=\"-1446\">Exodus</event>"));
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE_NON_NIHIL(t);

    CREDO_GRADUS("anni");
    CREDO_NECESSE(xar_numerus(t->anni) == III);
    CREDO_AEQUALIS_S32(annus_ad(t, ZEPHYRUM), -(s32)MCDXLVI);
    CREDO_AEQUALIS_S32(annus_ad(t, I), (s32)MCCXXV);
    CREDO_AEQUALIS_S32(annus_ad(t, II), (s32)MCCLXXIV);

    CREDO_GRADUS("ordo rerum");
    CREDO_NECESSE(xar_numerus(t->res) == VII);
    CREDO_CHORDA_AEQUALIS_LITERIS(res_ad(t,
        ZEPHYRUM)->eventus.descriptio,
        "Exodus");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_ad(t, I)->eventus.descriptio,
        "Thomas nascitur");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_ad(t, II)->eventus.descriptio,
        "Thomas");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_ad(t, III)->eventus.descriptio,
        "Lugdunum");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_ad(t, IV)->eventus.descriptio,
        "Bonaventura");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_ad(t, V)->eventus.descriptio,
        "A sine die");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_ad(t, VI)->eventus.descriptio,
        "B sine die");
}

/* pagina mutata; liber post aedificationem natus */
interior vacuum
_probare_mutationem (
    CredoContextus* c)
{
                     Mundus* m;
    constans ChronicaTabula* t;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->fontes);
    CREDO_VERUM(paginam_scribere(m, NIHIL, "historia",
        "<event year=\"1274\">Aquinas dies</event>"));
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE(t && xar_numerus(t->res) == I);

    CREDO_GRADUS("pagina mutata");
    CREDO_VERUM(paginam_scribere(m, NIHIL, "historia",
        "<event year=\"800\">Carolus coronatur</event>"));
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE(t && xar_numerus(t->res) == I);
    CREDO_AEQUALIS_S32(res_ad(t, ZEPHYRUM)->eventus.annus, (s32)DCCC);
    CREDO_NIHIL(res_descriptionis(t, "Aquinas dies"));
    CREDO_AEQUALIS_I32(xar_numerus(t->anni), I);
    CREDO_AEQUALIS_S32(annus_ad(t, ZEPHYRUM), (s32)DCCC);

    CREDO_GRADUS("liber comitis novus");
    CREDO_VERUM(paginam_scribere(m, "comes/calendarium/dies/2026-10-30",
        "1", "<event year=\"1517\">Theses</event>"));
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE(t && xar_numerus(t->res) == II);
    CREDO_NON_NIHIL(res_descriptionis(t, "Theses"));
}

/* sine mutatione: eadem tabula, inscriptiones RECENTES */
interior vacuum
_probare_recentem (
    CredoContextus* c)
{
                     Mundus* m;
    constans ChronicaTabula* t;
    constans ChronicaTabula* u;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->fontes);
    CREDO_VERUM(paginam_scribere(m, "comes/sancti/sanctus/1", "1",
        "<event year=\"1225\">Thomas nascitur</event>"));
    CREDO_VERUM(paginam_scribere(m, "comes/sancti/sanctus/2", "1",
        "<event year=\"1221\">Bonaventura nascitur</event>"));
    CREDO_VERUM(paginam_scribere(m, NIHIL, "historia",
        "<event year=\"-1446\">Exodus</event>"));
    t = chronica_tabula(m->fontes);
    u = chronica_tabula(m->fontes);
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_AEQUALIS_PTR(t, u);
    CREDO_VERUM(conspectus_status(m->repertorium, "eventa_comitis",
        lit("sancti/sanctus/1", c->piscina)) == CONSPECTUS_RECENS);
    CREDO_VERUM(conspectus_status(m->repertorium, "eventa_comitis",
        lit("sancti/sanctus/2", c->piscina)) == CONSPECTUS_RECENS);
    CREDO_VERUM(conspectus_status(m->repertorium, "eventa_libri",
        lit("historia", c->piscina)) == CONSPECTUS_RECENS);

    CREDO_GRADUS("liber alius mutatus: hic RECENS manet");
    CREDO_VERUM(paginam_scribere(m, "comes/sancti/sanctus/2", "1",
        "<event year=\"1274\">Bonaventura moritur</event>"));
    CREDO_VERUM(conspectus_status(m->repertorium, "eventa_comitis",
        lit("sancti/sanctus/2", c->piscina)) == CONSPECTUS_STALUM);
    CREDO_VERUM(conspectus_status(m->repertorium, "eventa_comitis",
        lit("sancti/sanctus/1", c->piscina)) == CONSPECTUS_RECENS);
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_NON_NIHIL(res_descriptionis(t, "Bonaventura moritur"));
    CREDO_NIHIL(res_descriptionis(t, "Bonaventura nascitur"));
    CREDO_AEQUALIS_PTR(chronica_tabula(m->fontes), t);
}

/* personae et loci: nomen praecisum, exactum, trans libros */
interior vacuum
_probare_personas (
    CredoContextus* c)
{
                     Mundus* m;
    constans ChronicaTabula* t;
                        Xar* r;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->fontes);
    CREDO_VERUM(paginam_scribere(m, NIHIL, "historia",
        "<event year=\"-1446\" people=\"Moses\" place=\"Egypt\">"
        "Exodus</event>"));
    CREDO_VERUM(paginam_scribere(m, "comes/sancti/sanctus/1", "1",
        "<event year=\"-1406\" people=\"Moses\">"
        "Moses moritur</event>"));
    CREDO_VERUM(paginam_scribere(m, "comes/sancti/sanctus/2", "1",
        "<event year=\"-1491\" people=\"Aaron; Moses \" "
        "place=\"Egypt\">Moses nascitur</event>"));
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE_NON_NIHIL(t);

    CREDO_GRADUS("persona");
    r = chronica_res_personae(t, lit(" Moses ", c->piscina),
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(xar_numerus(r) == III);
    CREDO_CHORDA_AEQUALIS_LITERIS(
        ((ChronicaRes*)xar_obtinere(r, ZEPHYRUM))->eventus.descriptio,
        "Moses nascitur");
    CREDO_AEQUALIS_I32(xar_numerus(chronica_res_personae(t,
        lit("Aaron", c->piscina), c->piscina)), I);
    CREDO_AEQUALIS_I32(xar_numerus(chronica_res_personae(t,
        lit("moses", c->piscina), c->piscina)), ZEPHYRUM);

    CREDO_GRADUS("locus");
    CREDO_AEQUALIS_I32(xar_numerus(chronica_res_loci(t,
        lit("Egypt", c->piscina), c->piscina)), II);
    CREDO_AEQUALIS_I32(xar_numerus(chronica_res_loci(t,
        lit("Sinai", c->piscina), c->piscina)), ZEPHYRUM);
}

/* locus longior quam inscriptio conspectus (CCLVI): eventa tamen
 * leguntur et mutationem sequuntur */
interior vacuum
_probare_clavem_longam (
    CredoContextus* c)
{
                     Mundus* m;
    constans ChronicaTabula* t;
                  character  praefixum[CCCXX];
                        i32  i;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->fontes);
    strcpy(praefixum, "comes/navigator/en.wikipedia.org/");
    per (i = (i32)strlen(praefixum); i < CCLXXX; i++)
    {
        praefixum[i] = 'x';
    }
    praefixum[i] = '\0';
    CREDO_VERUM(paginam_scribere(m, praefixum, "1",
        "<event year=\"1054\">Schisma</event>"));
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_NON_NIHIL(res_descriptionis(t, "Schisma"));

    CREDO_GRADUS("mutatio sequitur");
    CREDO_VERUM(paginam_scribere(m, praefixum, "1",
        "<event year=\"1378\">Schisma occidentale</event>"));
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_NIHIL(res_descriptionis(t, "Schisma"));
    CREDO_NON_NIHIL(res_descriptionis(t, "Schisma occidentale"));
}

/* nulla eventa: tabula vacua, non NIHIL */
interior vacuum
_probare_vacuam (
    CredoContextus* c)
{
                     Mundus* m;
    constans ChronicaTabula* t;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->fontes);
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_AEQUALIS_I32(xar_numerus(t->res), ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(t->anni), ZEPHYRUM);
}

/* scriba_liber_paginam_legere_omnes: vacuae servatae, finales
 * demptae; pagina absens nihil */
interior vacuum
_probare_lineas_omnes (
    CredoContextus* c)
{
    Mundus* m;
       Xar* lineae;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->fontes);
    CREDO_VERUM(paginam_scribere(m, "comes/sancti/sanctus/1", "1",
        "a\n\n   \nb\n\n"));
    lineae = xar_creare(c->piscina, (i32)magnitudo(chorda));
    CREDO_VERUM(scriba_liber_paginam_legere_omnes(c->piscina,
        m->intern, m->volumen, "comes/sancti/sanctus/1",
        lit("1", c->piscina), lineae));
    CREDO_NECESSE(xar_numerus(lineae) == IV);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(lineae,
        ZEPHYRUM), "a");
    CREDO_CHORDA_VACUA(*(chorda*)xar_obtinere(lineae, I));
    CREDO_CHORDA_VACUA(*(chorda*)xar_obtinere(lineae, II));
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(lineae, III),
        "b");

    CREDO_GRADUS("pagina absens");
    lineae = xar_creare(c->piscina, (i32)magnitudo(chorda));
    CREDO_VERUM(scriba_liber_paginam_legere_omnes(c->piscina,
        m->intern, m->volumen, "comes/sancti/sanctus/1",
        lit("nulla", c->piscina), lineae));
    CREDO_AEQUALIS_I32(xar_numerus(lineae), ZEPHYRUM);
}

/* C5a: declarationes (person, place) cum eventis unitae */
interior constans ChronicaEntitas*
entitas (
                        Xar* x,
         constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; x && i < xar_numerus(x); i++)
    {
        constans ChronicaEntitas* e = (constans ChronicaEntitas*)
            xar_obtinere(x, i);

        si (chorda_aequalis_literis(e->titulus, titulus))
        {
            redde e;
        }
    }
    redde NIHIL;
}

interior vacuum
_probare_nomina (
    CredoContextus* c)
{
                     Mundus* m;
    constans ChronicaTabula* t;
                        Xar* d;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->fontes);
    CREDO_VERUM(paginam_scribere(m, NIHIL, "historia",
        "<person>Moses</person>\n"
        "<place>Sinai</place>\n"
        "<event year=\"-1446\" people=\"Moses; Aaron\" "
        "place=\"Egypt\">Exodus</event>"));
    CREDO_VERUM(paginam_scribere(m, "comes/sancti/sanctus/1", "1",
        "<person>\n  <>Moses</>\n  <>Miriam</>\n</>"));
    t = chronica_tabula(m->fontes);
    CREDO_NECESSE_NON_NIHIL(t);

    CREDO_GRADUS("personae: declaratae et ex eventis, ordine");
    CREDO_NECESSE(t->personae && xar_numerus(t->personae) == III);
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans ChronicaEntitas*)
        xar_obtinere(t->personae, ZEPHYRUM))->titulus, "Aaron");
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans ChronicaEntitas*)
        xar_obtinere(t->personae, I))->titulus, "Miriam");
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans ChronicaEntitas*)
        xar_obtinere(t->personae, II))->titulus, "Moses");
    CREDO_AEQUALIS_I32(entitas(t->personae, "Moses")->eventa, I);
    CREDO_AEQUALIS_I32(entitas(t->personae, "Moses")->declarationes,
        II);
    CREDO_AEQUALIS_I32(entitas(t->personae, "Miriam")->eventa,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(entitas(t->personae, "Aaron")->declarationes,
        ZEPHYRUM);

    CREDO_GRADUS("loca");
    CREDO_NECESSE(t->loca && xar_numerus(t->loca) == II);
    CREDO_AEQUALIS_I32(entitas(t->loca, "Sinai")->declarationes, I);
    CREDO_AEQUALIS_I32(entitas(t->loca, "Egypt")->eventa, I);

    CREDO_GRADUS("declarationes nominis cum origine");
    d = chronica_declarationes(t, CHRONICA_PERSONA, lit(" Moses ",
        c->piscina), c->piscina);
    CREDO_NECESSE(d && xar_numerus(d) == II);
    CREDO_CHORDA_VACUA(((constans ChronicaNomenRes*)xar_obtinere(d,
        ZEPHYRUM))->liber);
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans ChronicaNomenRes*)
        xar_obtinere(d, I))->liber, "comes/sancti/sanctus/1");
    CREDO_AEQUALIS_I32(((constans ChronicaNomenRes*)xar_obtinere(d, I))
        ->declaratio.linea, I);

    CREDO_GRADUS("mutatio: declaratio deleta abit");
    CREDO_VERUM(paginam_scribere(m, "comes/sancti/sanctus/1", "1",
        "nihil"));
    t = chronica_tabula(m->fontes);
    CREDO_NIHIL(entitas(t->personae, "Miriam"));
    CREDO_AEQUALIS_I32(entitas(t->personae, "Moses")->declarationes, I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "comes",          _probare_comitem,        mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "communis",       _probare_communem,       mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "ordo",           _probare_ordinem,        mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "mutatio",        _probare_mutationem,     mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "recens",         _probare_recentem,       mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "personae",       _probare_personas,       mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "clavis longa",   _probare_clavem_longam,  mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "vacua",          _probare_vacuam,         mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "lineae omnes",   _probare_lineas_omnes,   mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "nomina",         _probare_nomina,         mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("chronica_fontes", SECTIONES);
}
