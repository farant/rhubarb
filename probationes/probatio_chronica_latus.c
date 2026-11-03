/* probatio_chronica_latus.c - chronica in latere (C3): visus anni
 * (annus magnus "1446 BC" / "AD 1274", linea parva), h l per annos
 * cum eventis, j k per paginas anni, descriptio longa fluit, positio
 * in praesentia (restituta; prima: annus hodie proximus), mutatio
 * dum aperta, vacua, mus (sagittae pedis, dimidia paginae), locus et
 * titulus loci, argumentum malum. C3b: visus fluminis (ordinarius) -
 * anni continui, columnae, annus iteratus in pagina sequenti, h l ad
 * paginas annorum, positio, v vertit, mus. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "fons.h"
#include "volumen.h"
#include "fasti.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "scriba_liber.h"
#include "conspectus.h"
#include "chronica_fontes.h"
#include "chronica.h"
#include "credo.h"

#include <stdio.h>
#include <string.h>

#define LATITUDO  CCCLX
#define ALTITUDO  CCXL


/* ==================================================
 * Mundus
 * ================================================== */

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
                 Volumen* volumen;
     ConspectusRegistrum* repertorium;
          ChronicaFontes* fontes;
        ChronicaMontatio  m;
             Dispensator* d;
                 Piscina* p;
                     s32  hodie;      /* annus hodiernus fictus */
                  chorda  aperta;     /* C4b: semita portae ultima */
                     i32  aperiones;
} Mundus;

interior b32
hodie_ficta (
    vacuum* ctx,
      Dies* hodie)
{
    *hodie = fasti_dies(((Mundus*)ctx)->hodie, X, XXX);
    redde VERUM;
}

/* C4b: porta domum ficta - semitam servat */
interior b32
aperire_fictum (
    vacuum* ctx,
    chorda  semita)
{
    Mundus* w = (Mundus*)ctx;

    w->aperta = chorda_transcribere(semita, w->p);
    w->aperiones++;
    redde VERUM;
}

interior chorda
lit (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

/* pagina libri communis cum textu toto (condita si deest) */
interior b32
paginam_scribere (
                 Mundus* w,
     constans character* titulus,
     constans character* textus)
{
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    l = scriba_liber_aperire(w->p, w->intern, w->volumen, CXX, LX);
    d = l ? scriba_liber_paginam_condere(l, lit(titulus, w->p))
          : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, w->p, d->latitudo,
        d->altitudo, textus);
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

/* pagina "1" libri comitis cum textu toto */
interior b32
paginam_comitis_scribere (
                 Mundus* w,
     constans character* praefixum,
     constans character* textus)
{
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    l = scriba_liber_aperire_in_spatio(w->p, w->intern, w->volumen,
        praefixum, CXX, LX);
    d = l ? scriba_liber_pagina(l, lit("1", w->p)) : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, w->p, d->latitudo,
        d->altitudo, textus);
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

/* volumen et fontes; latus nondum montatum */
interior Mundus*
mundum_creare (
    CredoContextus* c)
{
    Mundus* w;
       Xar* vitia;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    c->datum   = w;
    w->p       = c->piscina;
    w->hodie   = MMXXVI;
    w->intern  = internamentum_creare(c->piscina);
    w->volumen  = volumen_temporarium(c->piscina,
        "probatio_chronica_latus");
    si (!w->volumen)
    {
        redde NIHIL;
    }
    w->repertorium = conspectus_registrum_creare(w->p, w->intern,
        w->volumen, ZEPHYRUM);
    w->fontes = chronica_fontes_creare(w->p, w->intern, w->volumen);
    si (   !w->repertorium || !w->fontes
        || !chronica_registrare(w->repertorium, w->fontes, &vitia)
        || !conspectus_parare(w->repertorium, &vitia))
    {
        redde NIHIL;
    }
    redde w;
}

/* latus (iterum) montare: repositorium novum, volumen idem */
interior b32
montare (
                 Mundus* w,
     constans character* argumentum)
{
    Motus* motus;

    w->repo = insula_repositorium_creare(w->p, w->intern, "<hospes/>");
    si (   !w->repo
        || !chronica_montare(&w->m, w->p, w->intern, w->repo, "c1",
               w->fontes, w->volumen, LATITUDO, ALTITUDO, NIHIL))
    {
        redde FALSUM;
    }
    w->m.hodie        = hodie_ficta;
    w->m.hodie_ctx    = w;
    w->m.aperire      = aperire_fictum;
    w->m.aperire_ctx  = w;
    si (argumentum && !chronica_argumentum_ponere(&w->m, argumentum))
    {
        redde FALSUM;
    }
    w->d = dispensator_creare(w->p, w->intern, w->repo, w->m.actiones,
        chronica_componere, &w->m, CCC);
    si (!w->d)
    {
        redde FALSUM;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    redde VERUM;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* w = (Mundus*)c->datum;

    si (w && w->fontes)
    {
        chronica_fontes_destruere(w->fontes);
    }
    si (w && w->repertorium)
    {
        conspectus_registrum_destruere(w->repertorium);
    }
    si (w && w->volumen)
    {
        volumen_claudere(w->volumen);
    }
}


/* ==================================================
 * Auxilia
 * ================================================== */

interior vacuum
clavis (
       Mundus* w,
    character  typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.producta  = (s32)(insignatus character)typus;
    e.datum.clavis.clavis    = (clavis_t)typus;
    e.datum.clavis.runa      = (s32)typus;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior vacuum
clavis_nominata (
      Mundus* w,
    clavis_t  k)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis  = k;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior b32
campus_est (
                 Mundus* w,
     constans character* titulus,
     constans character* valor)
{
    chorda* a = insula_ramus_attributum(&w->m.ramus, titulus);

    redde a && chorda_aequalis_literis(*a, valor);
}

/* textus pictus continens fragmentum? */
interior b32
pictum (
                 Mundus* w,
     constans character* fragmentum)
{
               Mandata* md;
     constans Mandatum* x;
                   i32  i;

    md = mandata_creare(w->p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_continet(x->textus, lit(fragmentum, w->p)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
titulus_est (
                 Mundus* w,
     constans character* titulus)
{
    redde chorda_aequalis_literis(chronica_locus_titulus(&w->m, w->p),
        titulus);
}

/* ictus in angulo infero dextro componentis id (C4a: textus in
 * medio paginae eventum eligit - margo vacuus paginam vertit) */
interior b32
ictus_in (
                 Mundus* w,
     constans character* id)
{
    Componens* c;
      Eventus  e;

    c = componens_invenire_per_id(dispensator_arbor(w->d),
        lit(id, w->p));
    si (!c || c->fines.altitudo <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = c->fines.x + c->fines.latitudo - II;
    e.datum.mus.y       = c->fines.y + c->fines.altitudo - II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
    redde VERUM;
}

/* textus continens fragmentum colore thematis pictus? */
interior b32
pictum_colore (
                 Mundus* w,
     constans character* fragmentum,
             ColorThema  color)
{
               Mandata* md;
     constans Mandatum* x;
          ColorMandati  q;
                   i32  i;

    q   = figura_color_thematis(color);
    md  = mandata_creare(w->p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus       == MANDATUM_TEXTUS
            && x->color.genus == q.genus && x->color.valor == q.valor
            && chorda_continet(x->textus, lit(fragmentum, w->p)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ictus in medio runae primae fragmenti (textus pictus) */
interior b32
ictus_in_textu (
                 Mundus* w,
     constans character* fragmentum)
{
                Mandata* md;
      constans Mandatum* x;
                Eventus  e;
                    s32  k;
                    i32  i;
                    i32  j;

    md = mandata_creare(w->p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        k = x->genus == MANDATUM_TEXTUS ? chorda_invenire_index(
            x->textus, lit(fragmentum, w->p)) : -I;
        si (k < ZEPHYRUM)
        {
            perge;
        }
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus        = EVENTUS_MUS_DEPRESSUS;
        e.datum.mus.x  = x->fines.x + fons_latitudo(x->fons) / II;
        per (j = ZEPHYRUM; j < (i32)k; j++)
        {
            si ((x->textus.datum[j] & 0xC0) != 0x80)
            {
                e.datum.mus.x += fons_latitudo(x->fons);
            }
        }
        e.datum.mus.y       = x->fines.y + fons_altitudo(x->fons) / II;
        e.datum.mus.botton  = MUS_SINISTER;
        dispensator_tractare(w->d, &e);
        dispensator_recomponere(w->d);
        redde VERUM;
    }
    redde FALSUM;
}

interior vacuum
tabula (
    Mundus* w,
       b32  retro)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis        = CLAVIS_TABULA;
    e.datum.clavis.producta      = IX;
    e.datum.clavis.runa          = IX;
    e.datum.clavis.modificantes  = retro ? (i32)MOD_SHIFT : ZEPHYRUM;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

/* campus electus terminatur suffixo? (vacuum = eventus) */
interior b32
electus_est (
                 Mundus* w,
     constans character* cauda)
{
    chorda* a = insula_ramus_attributum(&w->m.ramus, "electus");

    si (!a || a->mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (cauda[ZEPHYRUM] == '\0')
    {
        redde !chorda_continet(*a, lit("#p", w->p))
            && !chorda_terminatur(*a, lit("#l", w->p));
    }
    redde chorda_terminatur(*a, lit(cauda, w->p));
}

interior b32
electus_nullus (
    Mundus* w)
{
    chorda* a = insula_ramus_attributum(&w->m.ramus, "electus");

    redde !a || a->mensura == ZEPHYRUM;
}

/* tres anni: -1446, 1225, 1274 */
interior b32
tres_annos (
    Mundus* w)
{
    redde paginam_scribere(w, "historia",
        "<event year=\"-1446\">Exodus</event>\n"
        "<event year=\"1225\">Thomas nascitur</event>\n"
        "<event year=\"1274\">Thomas moritur</event>");
}


/* ==================================================
 * Sectiones
 * ================================================== */

interior vacuum
_probare_annum (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && tres_annos(w)
        && montare(w, "chronica/annus"));

    CREDO_GRADUS("prima apertio: annus hodie proximus");
    CREDO_VERUM(pictum(w, "AD 1274"));
    CREDO_VERUM(pictum(w, "Thomas moritur"));
    CREDO_VERUM(titulus_est(w, "Timeline - AD 1274 - 1/1"));

    CREDO_GRADUS("ante Christum");
    CREDO_VERUM(chronica_argumentum_ponere(&w->m,
        "chronica/annus/-1446"));
    dispensator_recomponere(w->d);
    CREDO_VERUM(pictum(w, "1446 BC"));
    CREDO_VERUM(pictum(w, "Exodus"));
    CREDO_VERUM(campus_est(w, "annus", "-1446"));
    CREDO_FALSUM(pictum(w, "Thomas"));
}

interior vacuum
_probare_annos (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && tres_annos(w)
        && montare(w, "chronica/annus/1225"));
    CREDO_VERUM(pictum(w, "AD 1225"));

    CREDO_GRADUS("l: annus proximus cum eventis");
    clavis(w, 'l');
    CREDO_VERUM(campus_est(w, "annus", "1274"));
    CREDO_VERUM(pictum(w, "AD 1274"));
    clavis(w, 'l');
    CREDO_VERUM(campus_est(w, "annus", "1274"));

    CREDO_GRADUS("h: annus prior; in primo manet");
    clavis(w, 'h');
    CREDO_VERUM(campus_est(w, "annus", "1225"));
    clavis(w, 'h');
    CREDO_VERUM(campus_est(w, "annus", "-1446"));
    clavis(w, 'h');
    CREDO_VERUM(campus_est(w, "annus", "-1446"));

    CREDO_GRADUS("sagittae");
    clavis_nominata(w, CLAVIS_DEXTER);
    CREDO_VERUM(campus_est(w, "annus", "1225"));
    clavis_nominata(w, CLAVIS_SINISTER);
    CREDO_VERUM(campus_est(w, "annus", "-1446"));
}

/* XII eventa longa in anno 1274: paginae plures */
interior vacuum
_probare_paginas (
    CredoContextus* c)
{
         Mundus* w = mundum_creare(c);
      character  textus[IV * M];
            i32  i;
            i32  n;

    CREDO_NECESSE(w);
    textus[ZEPHYRUM] = '\0';
    per (i = ZEPHYRUM; i < XII; i++)
    {
        sprintf(textus + strlen(textus),
            "<event year=\"1274\">Eventus %02u: haec descriptio satis "
            "longa est ut lineas duas impleat in latere.</event>\n",
            (insignatus integer)(i + I));
    }
    CREDO_NECESSE(paginam_scribere(w, "historia", textus)
        && montare(w, "chronica/annus"));
    CREDO_VERUM(pictum(w, "Eventus 01"));
    CREDO_FALSUM(pictum(w, "Eventus 12"));
    CREDO_VERUM(chorda_continet(chronica_locus_titulus(&w->m, w->p),
        lit("Timeline - AD 1274 - 1/", w->p)));

    CREDO_GRADUS("j: pagina proxima; k: prior; k in prima manet");
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "pagina", "1"));
    CREDO_VERUM(chorda_continet(chronica_locus_titulus(&w->m, w->p),
        lit(" - 2/", w->p)));
    CREDO_FALSUM(pictum(w, "Eventus 01"));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "pagina", "0"));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "pagina", "0"));

    CREDO_GRADUS("ultima: Eventus 12; j ultra nihil");
    per (n = ZEPHYRUM; n < XXX; n++)
    {
        clavis(w, 'j');
    }
    CREDO_VERUM(pictum(w, "Eventus 12"));
    clavis_nominata(w, CLAVIS_SURSUM);
    CREDO_FALSUM(pictum(w, "Eventus 12"));
    clavis_nominata(w, CLAVIS_DEORSUM);
    CREDO_VERUM(pictum(w, "Eventus 12"));
}

/* descriptio pagina longior fluit, numquam praecisa */
interior vacuum
_probare_descriptionem_longam (
    CredoContextus* c)
{
       Mundus* w = mundum_creare(c);
    character  textus[IV * M];
          i32  i;

    CREDO_NECESSE(w);
    strcpy(textus, "<event year=\"1274\">PRINCIPIUM ");
    per (i = ZEPHYRUM; i < XXV; i++)
    {
        strcat(textus, "verbum medium verbum medium verbum medium\n");
    }
    strcat(textus, "TERMINUS</event>");
    CREDO_NECESSE(paginam_scribere(w, "historia", textus)
        && montare(w, "chronica/annus"));
    CREDO_VERUM(pictum(w, "PRINCIPIUM"));
    CREDO_FALSUM(pictum(w, "TERMINUS"));
    per (i = ZEPHYRUM; i < X; i++)
    {
        clavis(w, 'j');
    }
    CREDO_VERUM(pictum(w, "TERMINUS"));
    CREDO_FALSUM(pictum(w, "PRINCIPIUM"));
}

/* annus in praesentia; prima apertio: annus hodie proximus
 * (aequalitas: prior) */
interior vacuum
_probare_positionem (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && tres_annos(w)
        && montare(w, "chronica/annus"));
    clavis(w, 'h');
    CREDO_VERUM(campus_est(w, "annus", "1225"));

    CREDO_GRADUS("iterum montatum: annus servatus");
    CREDO_NECESSE(montare(w, "chronica/annus"));
    CREDO_VERUM(pictum(w, "AD 1225"));

    CREDO_GRADUS("argumentum vacuum: positio servata");
    clavis(w, 'h');
    CREDO_NECESSE(montare(w, "chronica/annus"));
    CREDO_VERUM(pictum(w, "1446 BC"));
}

interior vacuum
_probare_hodie (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && paginam_scribere(w, "historia",
        "<event year=\"1220\">A</event>\n"
        "<event year=\"1230\">B</event>\n"
        "<event year=\"1300\">C</event>"));
    w->hodie = MCCXXV;
    CREDO_NECESSE(montare(w, "chronica/annus"));
    CREDO_VERUM(pictum(w, "AD 1220"));
    CREDO_VERUM(chronica_argumentum_ponere(&w->m,
        "chronica/annus/1229"));
    dispensator_recomponere(w->d);
    CREDO_VERUM(pictum(w, "AD 1230"));
}

/* annus mutatione vacuus factus: proximus cum eventis */
interior vacuum
_probare_mutationem (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && tres_annos(w)
        && montare(w, "chronica/annus/1225"));
    CREDO_VERUM(pictum(w, "Thomas nascitur"));
    CREDO_VERUM(paginam_scribere(w, "historia",
        "<event year=\"-1446\">Exodus</event>\n"
        "<event year=\"1274\">Thomas moritur</event>\n"
        "<event year=\"1276\">Innocentius V</event>"));
    dispensator_recomponere(w->d);
    CREDO_VERUM(pictum(w, "AD 1274"));
    CREDO_FALSUM(pictum(w, "Thomas nascitur"));
    clavis(w, 'l');
    CREDO_VERUM(campus_est(w, "annus", "1276"));
}

interior vacuum
_probare_vacuam (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && montare(w, "chronica/annus"));
    CREDO_VERUM(pictum(w, "Write <event year=\"1274\">...</event> in a "
        "note"));
    CREDO_VERUM(titulus_est(w, "Timeline"));
    clavis(w, 'l');
    clavis(w, 'j');
    CREDO_VERUM(titulus_est(w, "Timeline"));
}

/* linea parva: dies, genus, personae, locus */
interior vacuum
_probare_lineam_parvam (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && paginam_scribere(w, "historia",
        "<event year=\"1274\" month=\"7\" day=\"15\" type=\"death\" "
        "people=\"Thomas; Bonaventura\" place=\"Lyon\">"
        "Bonaventura moritur</event>\n"
        "<event year=\"1274\" month=\"3\">Mense solo</event>")
        && montare(w, "chronica/annus"));
    CREDO_VERUM(pictum(w,
        "15 Jul \302\267 death \302\267 Thomas, Bonaventura \302\267 "
        "Lyon"));
    CREDO_VERUM(pictum(w, "Mar"));
}

interior vacuum
_probare_murem (
    CredoContextus* c)
{
         Mundus* w = mundum_creare(c);
      character  textus[IV * M];
            i32  i;

    CREDO_NECESSE(w);
    strcpy(textus, "<event year=\"1225\">Thomas nascitur</event>\n");
    per (i = ZEPHYRUM; i < XII; i++)
    {
        strcat(textus, "<event year=\"1274\">Haec descriptio satis "
            "longa est ut lineas duas impleat.</event>\n");
    }
    CREDO_NECESSE(paginam_scribere(w, "historia", textus)
        && montare(w, "chronica/annus/1225"));

    CREDO_GRADUS("sagittae pedis");
    CREDO_VERUM(ictus_in(w, "sagitta_proxima"));
    CREDO_VERUM(campus_est(w, "annus", "1274"));
    CREDO_VERUM(ictus_in(w, "sagitta_prior"));
    CREDO_VERUM(campus_est(w, "annus", "1225"));

    CREDO_GRADUS("dimidia paginae");
    clavis(w, 'l');
    CREDO_VERUM(ictus_in(w, "infra"));
    CREDO_VERUM(campus_est(w, "pagina", "1"));
    CREDO_VERUM(ictus_in(w, "supra"));
    CREDO_VERUM(campus_est(w, "pagina", "0"));
}

interior vacuum
_probare_locum (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && tres_annos(w)
        && montare(w, "chronica/annus"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(&w->m, w->p),
        "chronica/annus/1274");
    CREDO_VERUM(chronica_argumentum_ponere(&w->m,
        "chronica/annus/-1446"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(&w->m, w->p),
        "chronica/annus/-1446");
    CREDO_VERUM(titulus_est(w, "Timeline - 1446 BC - 1/1"));

    CREDO_GRADUS("argumentum malum: nihil mutatum");
    CREDO_FALSUM(chronica_argumentum_ponere(&w->m, "0"));
    CREDO_FALSUM(chronica_argumentum_ponere(&w->m, "abc"));
    CREDO_VERUM(campus_est(w, "annus", "-1446"));
}

/* x (px) textus primi qui fragmentum continet; -1 si nullum */
interior s32
x_picti (
                 Mundus* w,
     constans character* fragmentum)
{
               Mandata* md;
     constans Mandatum* x;
                   i32  i;

    md = mandata_creare(w->p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_continet(x->textus, lit(fragmentum, w->p)))
        {
            redde x->fines.x;
        }
    }
    redde -I;
}

/* textus pictus AEQUALIS (non solum continens: pes annos quoque
 * nominat) */
interior b32
pictum_exactum (
                 Mundus* w,
     constans character* textus)
{
               Mandata* md;
     constans Mandatum* x;
                   i32  i;

    md = mandata_creare(w->p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_aequalis_literis(x->textus, textus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* 1220 (unum), 1250 (XII longa: paginae plures), 1290 (unum) */
interior b32
annos_fluminis (
    Mundus* w)
{
    character textus[IV * M];
          i32 i;

    strcpy(textus, "<event year=\"1220\">Annus primus</event>\n");
    per (i = ZEPHYRUM; i < XII; i++)
    {
        sprintf(textus + strlen(textus),
            "<event year=\"1250\">Eventus %02u: haec descriptio satis "
            "longa est ut lineas duas impleat.</event>\n",
            (insignatus integer)(i + I));
    }
    strcat(textus, "<event year=\"1290\">Annus ultimus</event>");
    redde paginam_scribere(w, "historia", textus);
}

/* visus ordinarius: flumen - anni omnes in pagina, annus in columna
 * sinistra, descriptiones in dextra */
interior vacuum
_probare_flumen (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && tres_annos(w) && montare(w, NIHIL));
    CREDO_VERUM(pictum(w, "1446 BC"));
    CREDO_VERUM(pictum(w, "AD 1225"));
    CREDO_VERUM(pictum(w, "AD 1274"));
    CREDO_VERUM(pictum(w, "Exodus"));
    CREDO_VERUM(pictum(w, "Thomas moritur"));
    CREDO_VERUM(titulus_est(w, "Timeline \302\267 1446 BC \342\200\223 "
        "AD 1274 \302\267 1/1"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(&w->m, w->p),
        "chronica/flumen/-1446");

    CREDO_GRADUS("columnae: annus sinister, descriptio dextra");
    CREDO_VERUM(x_picti(w, "1446 BC") >= ZEPHYRUM);
    CREDO_VERUM(x_picti(w, "1446 BC") < x_picti(w, "Exodus"));
    CREDO_VERUM(x_picti(w, "Exodus") >= LATITUDO / V);
    CREDO_AEQUALIS_S32(x_picti(w, "AD 1225"), x_picti(w, "1446 BC"));

    CREDO_GRADUS("argumentum numerus: flumen ad annum");
    CREDO_VERUM(chronica_argumentum_ponere(&w->m, "1225"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(&w->m, w->p),
        "chronica/flumen/-1446");
}

/* paginae plenae; annus in paginam sequentem fluens iteratur */
interior vacuum
_probare_flumen_paginas (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && annos_fluminis(w));
    w->hodie = MCCXX;
    CREDO_NECESSE(montare(w, NIHIL));
    CREDO_VERUM(pictum_exactum(w, "AD 1220"));
    CREDO_VERUM(pictum(w, "Annus primus"));
    CREDO_VERUM(pictum_exactum(w, "AD 1250"));
    CREDO_VERUM(pictum(w, "Eventus 01"));
    CREDO_FALSUM(titulus_est(w,
        "Timeline \302\267 AD 1220 \342\200\223 "
        "AD 1250 \302\267 1/1"));

    CREDO_GRADUS("j: pagina secunda, annus iteratus in summo");
    clavis(w, 'j');
    CREDO_FALSUM(pictum(w, "Annus primus"));
    CREDO_FALSUM(pictum(w, "AD 1220"));
    CREDO_VERUM(pictum_exactum(w, "AD 1250"));
    CREDO_VERUM(chorda_continet(chronica_locus_titulus(&w->m, w->p),
        lit("Timeline \302\267 AD 1250 \302\267 2/", w->p)));
    CREDO_VERUM(campus_est(w, "annus", "1250"));
    CREDO_VERUM(campus_est(w, "pagina", "1"));

    CREDO_GRADUS("k: prima");
    clavis(w, 'k');
    CREDO_VERUM(pictum(w, "Annus primus"));
    clavis(w, 'k');
    CREDO_VERUM(pictum(w, "Annus primus"));
}

/* h / l: ad paginam ubi annus prior / proximus incipit */
interior vacuum
_probare_flumen_annos (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && annos_fluminis(w));
    w->hodie = MCCXX;
    CREDO_NECESSE(montare(w, NIHIL));

    CREDO_GRADUS("l: annus 1250 in eadem pagina incipit - ad 1290");
    clavis(w, 'l');
    CREDO_VERUM(pictum(w, "Annus ultimus"));
    CREDO_FALSUM(pictum(w, "Annus primus"));
    clavis(w, 'l');
    CREDO_VERUM(pictum(w, "Annus ultimus"));

    CREDO_GRADUS("h: ad paginam ubi annus summus incipit");
    clavis(w, 'h');
    CREDO_VERUM(pictum(w, "Annus primus"));
    clavis(w, 'h');
    CREDO_VERUM(pictum(w, "Annus primus"));
}

/* positio: annus summus et pagina inter paginas eius */
interior vacuum
_probare_flumen_positionem (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && annos_fluminis(w));
    w->hodie = MCCXX;
    CREDO_NECESSE(montare(w, NIHIL));
    clavis(w, 'j');
    CREDO_NECESSE(montare(w, NIHIL));
    CREDO_VERUM(chorda_continet(chronica_locus_titulus(&w->m, w->p),
        lit(" 2/", w->p)));
    CREDO_FALSUM(pictum(w, "Annus primus"));
}

/* v: flumen <-> annus; annus summus communis */
interior vacuum
_probare_vertere (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && tres_annos(w) && montare(w, NIHIL));
    clavis(w, 'v');
    CREDO_VERUM(campus_est(w, "visus", "annus"));
    CREDO_VERUM(pictum(w, "1446 BC"));
    CREDO_FALSUM(pictum(w, "Thomas"));
    CREDO_VERUM(titulus_est(w, "Timeline - 1446 BC - 1/1"));
    clavis(w, 'l');
    clavis(w, 'v');
    CREDO_VERUM(campus_est(w, "visus", "flumen"));
    CREDO_VERUM(pictum(w, "Thomas moritur"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(&w->m, w->p),
        "chronica/flumen/-1446");
}

interior vacuum
_probare_flumen_murem (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && annos_fluminis(w));
    w->hodie = MCCXX;
    CREDO_NECESSE(montare(w, NIHIL));
    CREDO_VERUM(ictus_in(w, "infra"));
    CREDO_FALSUM(pictum(w, "Annus primus"));
    CREDO_VERUM(ictus_in(w, "supra"));
    CREDO_VERUM(pictum(w, "Annus primus"));
    CREDO_VERUM(ictus_in(w, "sagitta_proxima"));
    CREDO_VERUM(pictum(w, "Annus ultimus"));
    CREDO_VERUM(ictus_in(w, "sagitta_prior"));
    CREDO_VERUM(pictum(w, "Annus primus"));
}

/* C4a: Tab per eventum, personas, locum; S-Tab; Esc; pes */
interior vacuum
_probare_electionem (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && paginam_scribere(w, "historia",
        "<event year=\"1274\" type=\"concilium\" "
        "people=\"Thomas; Bonaventura\" place=\"Lyon\">Concilium"
        "</event>\n"
        "<event year=\"1275\">Alterum</event>") && montare(w, NIHIL));
    CREDO_VERUM(electus_nullus(w));

    CREDO_GRADUS("Tab: eventus primus inversus, pes ad notam");
    tabula(w, FALSUM);
    CREDO_VERUM(electus_est(w, ""));
    CREDO_VERUM(pictum_colore(w, "Concilium", COLOR_BACKGROUND));
    CREDO_VERUM(pictum(w, "-> note: Notebook"));

    CREDO_GRADUS("Tab: personae ordine, deinde locus");
    tabula(w, FALSUM);
    CREDO_VERUM(electus_est(w, "#p0"));
    CREDO_VERUM(pictum_colore(w, "Thomas", COLOR_BACKGROUND));
    CREDO_VERUM(pictum(w, "-> person: Thomas"));
    tabula(w, FALSUM);
    CREDO_VERUM(electus_est(w, "#p1"));
    CREDO_VERUM(pictum(w, "-> person: Bonaventura"));
    tabula(w, FALSUM);
    CREDO_VERUM(electus_est(w, "#l"));
    CREDO_VERUM(pictum(w, "-> place: Lyon"));
    CREDO_VERUM(pictum_colore(w, "Lyon", COLOR_BACKGROUND));

    CREDO_GRADUS("Tab: eventus sequens (annus alius); S-Tab retro");
    tabula(w, FALSUM);
    CREDO_VERUM(electus_est(w, ""));
    CREDO_VERUM(pictum_colore(w, "Alterum", COLOR_BACKGROUND));
    tabula(w, VERUM);
    CREDO_VERUM(electus_est(w, "#l"));

    CREDO_GRADUS("Esc: electio tollitur, pes ordinarius");
    clavis_nominata(w, CLAVIS_EFFUGIUM);
    CREDO_VERUM(electus_nullus(w));
    CREDO_FALSUM(pictum(w, "-> "));
    CREDO_VERUM(pictum(w, "Timeline"));
}

/* Tab trans paginas (flumen); S-Tab redit */
interior vacuum
_probare_electionem_paginarum (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);
       i32  n;

    CREDO_NECESSE(w && annos_fluminis(w));
    w->hodie = MCCXX;
    CREDO_NECESSE(montare(w, NIHIL));
    per (n = ZEPHYRUM; n < XXX && chorda_continet(
        chronica_locus_titulus(&w->m, w->p), lit(" 1/", w->p)); n++)
    {
        tabula(w, FALSUM);
    }
    CREDO_VERUM(chorda_continet(chronica_locus_titulus(&w->m, w->p),
        lit(" 2/", w->p)));
    CREDO_VERUM(electus_est(w, ""));
    CREDO_VERUM(pictum_colore(w, "Eventus", COLOR_BACKGROUND));
    tabula(w, VERUM);
    CREDO_VERUM(chorda_continet(chronica_locus_titulus(&w->m, w->p),
        lit(" 1/", w->p)));
    CREDO_VERUM(electus_est(w, ""));
}

/* visus anni: Tab intra annum manet; motus anni electionem tollit */
interior vacuum
_probare_electionem_motus (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && tres_annos(w)
        && montare(w, "chronica/annus/1225"));
    tabula(w, FALSUM);
    CREDO_VERUM(pictum_colore(w, "Thomas nascitur", COLOR_BACKGROUND));
    tabula(w, FALSUM);
    CREDO_VERUM(pictum_colore(w, "Thomas nascitur", COLOR_BACKGROUND));
    CREDO_VERUM(campus_est(w, "annus", "1225"));

    CREDO_GRADUS("l: annus alius, electio nulla");
    clavis(w, 'l');
    CREDO_VERUM(campus_est(w, "annus", "1274"));
    CREDO_VERUM(electus_nullus(w));
    CREDO_FALSUM(pictum(w, "-> "));
}

/* electa mutatione deleta = nulla */
interior vacuum
_probare_electionem_mutationis (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && paginam_scribere(w, "historia",
        "<event year=\"1274\">Primum</event>\n"
        "<event year=\"1275\">Delendum</event>") && montare(w, NIHIL));
    tabula(w, FALSUM);
    tabula(w, FALSUM);
    CREDO_VERUM(pictum_colore(w, "Delendum", COLOR_BACKGROUND));
    CREDO_VERUM(paginam_scribere(w, "historia",
        "<event year=\"1274\">Primum</event>"));
    dispensator_recomponere(w->d);
    CREDO_FALSUM(pictum(w, "-> "));
    CREDO_FALSUM(pictum_colore(w, "Primum", COLOR_BACKGROUND));
    tabula(w, FALSUM);
    CREDO_VERUM(pictum_colore(w, "Primum", COLOR_BACKGROUND));
}

/* ictus in textu eligit; margo vacuus paginam vertit */
interior vacuum
_probare_electionem_muris (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && paginam_scribere(w, "historia",
        "<event year=\"1274\" people=\"Thomas; Bonaventura\" "
        "place=\"Lyon\">Concilium</event>") && montare(w, NIHIL));
    CREDO_VERUM(ictus_in_textu(w, "Bonaventura"));
    CREDO_VERUM(electus_est(w, "#p1"));
    CREDO_VERUM(ictus_in_textu(w, "Lyon"));
    CREDO_VERUM(electus_est(w, "#l"));
    CREDO_VERUM(ictus_in_textu(w, "Concilium"));
    CREDO_VERUM(electus_est(w, ""));
}

/* C4b: Enter domum - semita libri communis et comitis cum linea;
 * persona nihil; ictus in electo aperit */
interior vacuum
_probare_domum (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && paginam_scribere(w, "historia",
        "Prima linea\n"
        "<event year=\"1274\" people=\"Moses\">Communis</event>")
        && paginam_comitis_scribere(w,
        "comes/calendarium/dies/2026-10-30",
        "<event year=\"1100\">Comitis</event>")
        && montare(w, NIHIL));

    CREDO_GRADUS("Enter sine electa: nihil");
    clavis_nominata(w, CLAVIS_REDITUS);
    CREDO_AEQUALIS_I32(w->aperiones, ZEPHYRUM);

    CREDO_GRADUS("eventus comitis: liber post lineam");
    tabula(w, FALSUM);
    CREDO_VERUM(pictum_colore(w, "Comitis", COLOR_BACKGROUND));
    clavis_nominata(w, CLAVIS_REDITUS);
    CREDO_AEQUALIS_I32(w->aperiones, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(w->aperta,
        "scriba/1?linea=0&columna=0&liber=comes/calendarium/dies/"
        "2026-10-30");

    CREDO_GRADUS("eventus communis: pagina et linea");
    tabula(w, FALSUM);
    CREDO_VERUM(pictum_colore(w, "Communis", COLOR_BACKGROUND));
    clavis_nominata(w, CLAVIS_REDITUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(w->aperta,
        "scriba/historia?linea=1&columna=0");

    CREDO_GRADUS("persona electa: paginam suam aperit (C5b), porta "
        "nulla");
    tabula(w, FALSUM);
    CREDO_VERUM(electus_est(w, "#p0"));
    clavis_nominata(w, CLAVIS_REDITUS);
    CREDO_AEQUALIS_I32(w->aperiones, II);
    CREDO_VERUM(campus_est(w, "visus", "persona"));
    CREDO_VERUM(campus_est(w, "nomen", "Moses"));
    CREDO_VERUM(chronica_argumentum_ponere(&w->m, ""));
    dispensator_recomponere(w->d);

    CREDO_GRADUS("ictus in eventu electo aperit, in non electo eligit");
    CREDO_VERUM(ictus_in_textu(w, "Comitis"));
    CREDO_AEQUALIS_I32(w->aperiones, II);
    CREDO_VERUM(electus_est(w, ""));
    CREDO_VERUM(ictus_in_textu(w, "Comitis"));
    CREDO_AEQUALIS_I32(w->aperiones, III);
    CREDO_CHORDA_AEQUALIS_LITERIS(w->aperta,
        "scriba/1?linea=0&columna=0&liber=comes/calendarium/dies/"
        "2026-10-30");
}

/* C5b: personae et loca */
interior b32
nomina_scribere (
    Mundus* w)
{
    redde paginam_scribere(w, "historia",
        "<person>Miriam</person>\n"
        "<event year=\"-1446\" people=\"Moses; Aaron\" "
        "place=\"Egypt\">Exodus</event>\n"
        "<event year=\"-1406\" people=\"Moses\">Moses moritur</event>\n"
        "<person>Moses</person>");
}

/* p: index personarum; Tab, Enter: pagina nominis */
interior vacuum
_probare_personas (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && nomina_scribere(w) && montare(w, NIHIL));
    clavis(w, 'p');
    CREDO_VERUM(campus_est(w, "visus", "personae"));
    CREDO_VERUM(pictum_exactum(w, "People"));
    CREDO_VERUM(pictum(w, "Aaron \302\267 1 event"));
    CREDO_VERUM(pictum(w, "Miriam \302\267 declared 1"));
    CREDO_VERUM(pictum(w,
        "Moses \302\267 2 events \302\267 declared 1"));
    CREDO_VERUM(titulus_est(w, "People - 1/1"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(&w->m, w->p),
        "chronica/personae");

    CREDO_GRADUS("Tab eligit, pes ad personam; Enter aperit");
    tabula(w, FALSUM);
    CREDO_VERUM(pictum(w, "-> person: Aaron"));
    tabula(w, FALSUM);
    tabula(w, FALSUM);
    CREDO_VERUM(pictum_colore(w, "Moses", COLOR_BACKGROUND));
    clavis_nominata(w, CLAVIS_REDITUS);
    CREDO_VERUM(campus_est(w, "visus", "persona"));
    CREDO_VERUM(campus_est(w, "nomen", "Moses"));
    CREDO_AEQUALIS_I32(w->aperiones, ZEPHYRUM);
}

/* pagina nominis: declarationes, eventa; Enter domum ad lineam */
interior vacuum
_probare_paginam_nominis (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && nomina_scribere(w)
        && montare(w, "chronica/persona/Moses"));
    CREDO_VERUM(campus_est(w, "visus", "persona"));
    CREDO_VERUM(pictum_exactum(w, "Moses"));
    CREDO_VERUM(pictum(w, "Declared in"));
    CREDO_VERUM(pictum(w, "Notebook, page historia"));
    CREDO_VERUM(pictum(w, "1446 BC  Exodus"));
    CREDO_VERUM(pictum(w, "1406 BC  Moses moritur"));
    CREDO_FALSUM(pictum(w, "Miriam"));
    CREDO_VERUM(titulus_est(w, "Moses - 1/1"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(&w->m, w->p),
        "chronica/persona/Moses");

    CREDO_GRADUS("declaratio: domum ad lineam declarationis");
    tabula(w, FALSUM);
    CREDO_VERUM(pictum(w, "-> note: Notebook"));
    clavis_nominata(w, CLAVIS_REDITUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(w->aperta,
        "scriba/historia?linea=3&columna=0");

    CREDO_GRADUS("eventus: domum ad lineam eventus");
    tabula(w, FALSUM);
    CREDO_VERUM(pictum_colore(w, "Exodus", COLOR_BACKGROUND));
    clavis_nominata(w, CLAVIS_REDITUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(w->aperta,
        "scriba/historia?linea=1&columna=0");
}

/* P: loca; locus Egypt */
interior vacuum
_probare_loca (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && nomina_scribere(w) && montare(w, NIHIL));
    clavis(w, 'P');
    CREDO_VERUM(campus_est(w, "visus", "loca"));
    CREDO_VERUM(pictum_exactum(w, "Places"));
    CREDO_VERUM(pictum(w, "Egypt \302\267 1 event"));
    CREDO_VERUM(titulus_est(w, "Places - 1/1"));
    tabula(w, FALSUM);
    CREDO_VERUM(pictum(w, "-> place: Egypt"));
    clavis_nominata(w, CLAVIS_REDITUS);
    CREDO_VERUM(campus_est(w, "visus", "locus"));
    CREDO_VERUM(pictum(w, "1446 BC  Exodus"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(&w->m, w->p),
        "chronica/locus/Egypt");
}

/* nomen effugitum in loco et argumento; persona ex chronica */
interior vacuum
_probare_nomen_effugitum (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && paginam_scribere(w, "historia",
        "<event year=\"1274\" people=\"Tom\303\241s de Aquino\">"
        "Moritur</event>") && montare(w, NIHIL));

    CREDO_GRADUS("Enter in persona electa (flumen): pagina eius");
    tabula(w, FALSUM);
    tabula(w, FALSUM);
    CREDO_VERUM(electus_est(w, "#p0"));
    clavis_nominata(w, CLAVIS_REDITUS);
    CREDO_VERUM(campus_est(w, "nomen", "Tom\303\241s de Aquino"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(&w->m, w->p),
        "chronica/persona/Tom%C3%A1s%20de%20Aquino");

    CREDO_GRADUS("argumentum effugitum redit");
    CREDO_VERUM(chronica_argumentum_ponere(&w->m, ""));
    CREDO_VERUM(chronica_argumentum_ponere(&w->m,
        "chronica/persona/Tom%C3%A1s%20de%20Aquino"));
    dispensator_recomponere(w->d);
    CREDO_VERUM(campus_est(w, "nomen", "Tom\303\241s de Aquino"));
    CREDO_VERUM(pictum(w, "AD 1274  Moritur"));
}

/* mus in indice: ictus eligit, ictus in electo aperit */
interior vacuum
_probare_personas_mure (
    CredoContextus* c)
{
    Mundus* w = mundum_creare(c);

    CREDO_NECESSE(w && nomina_scribere(w)
        && montare(w, "chronica/personae"));
    CREDO_VERUM(campus_est(w, "visus", "personae"));
    CREDO_VERUM(ictus_in_textu(w, "Miriam"));
    CREDO_VERUM(pictum(w, "-> person: Miriam"));
    CREDO_VERUM(ictus_in_textu(w, "Miriam"));
    CREDO_VERUM(campus_est(w, "visus", "persona"));
    CREDO_VERUM(campus_est(w, "nomen", "Miriam"));
    CREDO_VERUM(pictum(w, "Notebook, page historia"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "annus",        _probare_annum,                NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "anni",         _probare_annos,                NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "paginae",      _probare_paginas,              NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "descriptio longa", _probare_descriptionem_longam, NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "positio",      _probare_positionem,           NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "hodie",        _probare_hodie,                NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "mutatio",      _probare_mutationem,           NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "vacua",        _probare_vacuam,               NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "linea parva",  _probare_lineam_parvam,        NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "mus",          _probare_murem,                NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "locus",        _probare_locum,                NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "flumen",       _probare_flumen,               NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "flumen paginae", _probare_flumen_paginas,     NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "flumen anni",  _probare_flumen_annos,         NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "flumen positio", _probare_flumen_positionem,  NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "vertere",      _probare_vertere,              NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "flumen mus",   _probare_flumen_murem,         NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "electio",      _probare_electionem,           NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "electio paginae", _probare_electionem_paginarum, NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "electio motus", _probare_electionem_motus,    NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "electio mutatio", _probare_electionem_mutationis, NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "electio mus",  _probare_electionem_muris,     NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "domum",        _probare_domum,                NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "personae",     _probare_personas,             NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "pagina nominis", _probare_paginam_nominis,    NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "loca",         _probare_loca,                 NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "nomen effugitum", _probare_nomen_effugitum,   NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "personae mus", _probare_personas_mure,        NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("chronica_latus", SECTIONES);
}
