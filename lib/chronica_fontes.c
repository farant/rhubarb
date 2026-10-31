/* chronica_fontes.c - eventa omnium librorum in tabulam unam
 * (chronica C2). Vide include/chronica_fontes.h. */

#include "chronica_fontes.h"
#include "chorda_aedificator.h"
#include "conspectus_infixa.h"
#include "scriba_liber.h"

#include <string.h>

/* conspectus libri unius: eventa (ChronicaRes) et declarationes
 * (ChronicaNomenRes, C5a) in piscina conspectus */
nomen structura {
    Xar* res;
    Xar* declarationes;
} EventaLibri;

structura ChronicaFontes {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
    ConspectusRegistrum* repertorium;
                Piscina* piscina_tabulae;
         ChronicaTabula* tabula;
                    b32  stalum;
};

hic_manens constans character  titulus_comitis[]  = "eventa_comitis";
hic_manens constans character  titulus_libri[]    = "eventa_libri";
hic_manens constans character  titulus_communis[] = "Notebook";


interior chorda
vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}


/* ==================================================
 * Computatio libri
 * ================================================== */

/* eventa paginae unius ad res (praefixum NIHIL = communis); eventa
 * vitiosa omittuntur */
interior vacuum
paginam_addere (
         ChronicaFontes* f,
     constans character* praefixum,
                 chorda  liber,
                 chorda  pagina,
                 chorda  titulus,
                Piscina* piscina,
            EventaLibri* v)
{
    Xar* lineae;
    Xar* eventa;
    Xar* declarationes;
    Xar* vitia;
    i32  i;

    lineae = xar_creare(piscina, (i32)magnitudo(chorda));
    eventa = xar_creare(piscina,
        (i32)magnitudo(ChronicaEventus));
    declarationes  = xar_creare(piscina,
        (i32)magnitudo(ChronicaDeclaratio));
    vitia          = xar_creare(piscina,
        (i32)magnitudo(ChronicaVitium));
    si (   !lineae || !eventa || !declarationes || !vitia
        || !scriba_liber_paginam_legere_omnes(piscina, f->intern,
               f->volumen, praefixum, pagina, lineae)
        || !chronica_eventa_legere(lineae, piscina, eventa, vitia)
        || !chronica_nomina_legere(lineae, piscina, f->intern,
               declarationes, vitia))
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(eventa); i++)
    {
        ChronicaRes* r = (ChronicaRes*)xar_addere(v->res);

        r->eventus        = *(ChronicaEventus*)xar_obtinere(eventa, i);
        r->liber          = liber;
        r->pagina         = pagina;
        r->titulus_libri  = titulus;
    }
    per (i = ZEPHYRUM; i < xar_numerus(declarationes); i++)
    {
        ChronicaNomenRes* r = (ChronicaNomenRes*)xar_addere(
            v->declarationes);

        r->declaratio     = *(ChronicaDeclaratio*)xar_obtinere(
            declarationes, i);
        r->liber          = liber;
        r->pagina         = pagina;
        r->titulus_libri  = titulus;
    }
}

interior EventaLibri*
eventa_nova (
    Piscina* piscina)
{
    EventaLibri* v;

    v = (EventaLibri*)piscina_allocare(piscina, magnitudo(EventaLibri));
    v->res = xar_creare(piscina, (i32)magnitudo(ChronicaRes));
    v->declarationes = xar_creare(piscina,
        (i32)magnitudo(ChronicaNomenRes));
    redde v->res && v->declarationes ? v : NIHIL;
}

/* liber comitis: clavis = locus; paginae ordine indicis */
interior EventaLibri*
comitem_computare (
    ChronicaFontes* f,
            chorda  clavis,
           Piscina* piscina)
{
           EventaLibri* v;
                   Xar* nomina;
                chorda  liber;
                chorda  titulus;
                chorda  semita;
    constans character* praefixum;
                   i32  i;

    v          = eventa_nova(piscina);
    liber      = chorda_concatenare(chorda_ex_literis("comes/",
        piscina),
        clavis, piscina);
    praefixum  = chorda_ut_cstr(liber, piscina);
    nomina     = xar_creare(piscina, (i32)magnitudo(chorda));
    si (   !v || !nomina
        || !scriba_liber_nomina_legere(piscina, f->intern, f->volumen,
               praefixum, nomina))
    {
        redde v;
    }
    si (   !scriba_liber_comitem_legere(piscina, f->intern, f->volumen,
               praefixum, &titulus, &semita)
        || titulus.mensura == ZEPHYRUM)
    {
        titulus = chorda_transcribere(clavis, piscina);
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        paginam_addere(f, praefixum, liber,
            *(chorda*)xar_obtinere(nomina, i), titulus, piscina,
            v);
    }
    redde v;
}

/* pagina libri communis: clavis = nomen paginae */
interior EventaLibri*
communem_computare (
    ChronicaFontes* f,
            chorda  clavis,
           Piscina* piscina)
{
    EventaLibri* v;

    v = eventa_nova(piscina);
    si (v)
    {
        paginam_addere(f, NIHIL, vacua(),
            chorda_transcribere(clavis, piscina),
            chorda_ex_literis(titulus_communis, piscina), piscina,
            v);
    }
    redde v;
}


/* ==================================================
 * Opera conspectus
 * ================================================== */

interior vacuum*
comitis_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
    (vacuum)repertorium;
    redde comitem_computare((ChronicaFontes*)ctx, clavis, piscina);
}

interior vacuum*
libri_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
    (vacuum)repertorium;
    redde communem_computare((ChronicaFontes*)ctx, clavis, piscina);
}

/* codex canonicus: linea una per eventum - positio, data, campi,
 * descriptio (JSON evasa) */
interior chorda
eventa_codificare (
             vacuum* ctx,
    constans vacuum* conspectus,
            Piscina* piscina)
{
    constans EventaLibri* v;
       ChordaAedificator* a;
                     i32  i;
                     i32  k;

    (vacuum)ctx;
    v = (constans EventaLibri*)conspectus;
    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    per (i = ZEPHYRUM; v && i < xar_numerus(v->res); i++)
    {
        constans ChronicaRes* r = (constans ChronicaRes*)xar_obtinere(
            v->res, i);
        constans ChronicaEventus* e = &r->eventus;

        chorda_aedificator_appendere_chorda(a, r->pagina);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_s32(a, (s32)e->linea);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_s32(a, (s32)e->columna);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_s32(a, e->annus);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_s32(a, e->mensis);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_s32(a, e->dies);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_evasus_json(a, e->locus);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_evasus_json(a, e->genus);
        per (k = ZEPHYRUM; e->personae && k < xar_numerus(e->personae);
            k++)
        {
            chorda_aedificator_appendere_character(a, ' ');
            chorda_aedificator_appendere_evasus_json(a,
                *(chorda*)xar_obtinere(e->personae, k));
        }
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_evasus_json(a, e->descriptio);
        chorda_aedificator_appendere_character(a, '\n');
    }
    /* C5a: declarationes - "D <pagina> <linea> <genus> <titulus>" */
    per (i = ZEPHYRUM; v && v->declarationes
        && i < xar_numerus(v->declarationes); i++)
    {
        constans ChronicaNomenRes* r = (constans ChronicaNomenRes*)
            xar_obtinere(v->declarationes, i);

        chorda_aedificator_appendere_literis(a, "D ");
        chorda_aedificator_appendere_chorda(a, r->pagina);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_s32(a, (s32)r->declaratio.linea);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_s32(a, (s32)r->declaratio.genus);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_evasus_json(a,
            r->declaratio.titulus);
        chorda_aedificator_appendere_character(a, '\n');
    }
    redde chorda_aedificator_finire(a);
}

interior vacuum
audire (
    vacuum* ctx,
    chorda  clavis)
{
    (vacuum)clavis;
    ((ChronicaFontes*)ctx)->stalum = VERUM;
}


/* ==================================================
 * Tabula
 * ================================================== */

interior chorda
copiare (
    chorda  c,
   Piscina* piscina)
{
    redde c.mensura > ZEPHYRUM ? chorda_transcribere(c, piscina)
                               : vacua();
}

/* res in tabulam copiare (chordae in piscinam tabulae) */
interior vacuum
res_copiare (
    constans EventaLibri* v,
                     Xar* exitus,
                     Xar* declarationes,
                 Piscina* piscina)
{
    i32 i;
    i32 k;

    per (i = ZEPHYRUM; v && v->declarationes
        && i < xar_numerus(v->declarationes); i++)
    {
        constans ChronicaNomenRes* r = (constans ChronicaNomenRes*)
            xar_obtinere(v->declarationes, i);
        ChronicaNomenRes* n = (ChronicaNomenRes*)xar_addere(
            declarationes);

        *n = *r;
        n->declaratio.titulus = copiare(r->declaratio.titulus,
            piscina);
        n->liber          = copiare(r->liber, piscina);
        n->pagina         = copiare(r->pagina, piscina);
        n->titulus_libri  = copiare(r->titulus_libri, piscina);
        n->declaratio.attributa = xar_creare(piscina,
            (i32)magnitudo(ChronicaAttributum));
        per (k = ZEPHYRUM; r->declaratio.attributa
            && k < xar_numerus(r->declaratio.attributa); k++)
        {
            constans ChronicaAttributum* s =
                (constans ChronicaAttributum*)
                xar_obtinere(r->declaratio.attributa, k);
            ChronicaAttributum* d = (ChronicaAttributum*)xar_addere(
                n->declaratio.attributa);

            d->titulus  = copiare(s->titulus, piscina);
            d->valor    = copiare(s->valor, piscina);
        }
    }
    per (i = ZEPHYRUM; v && i < xar_numerus(v->res); i++)
    {
        constans ChronicaRes* r = (constans ChronicaRes*)xar_obtinere(
            v->res, i);
        ChronicaRes* n = (ChronicaRes*)xar_addere(exitus);

        *n = *r;
        n->eventus.descriptio = copiare(r->eventus.descriptio,
            piscina);
        n->eventus.locus  = copiare(r->eventus.locus, piscina);
        n->eventus.genus  = copiare(r->eventus.genus, piscina);
        n->liber          = copiare(r->liber, piscina);
        n->pagina         = copiare(r->pagina, piscina);
        n->titulus_libri  = copiare(r->titulus_libri, piscina);
        n->eventus.personae    = xar_creare(piscina,
            (i32)magnitudo(chorda));
        per (k = ZEPHYRUM; r->eventus.personae
            && k < xar_numerus(r->eventus.personae); k++)
        {
            *(chorda*)xar_addere(n->eventus.personae) = copiare(
                *(chorda*)xar_obtinere(r->eventus.personae, k),
                piscina);
        }
    }
}

/* libri comites: plagulae "comes/<locus>/paginae", ordine loci */
interior s32
chordas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
     chorda x = *(constans chorda*)a;
     chorda y = *(constans chorda*)b;
        i32 n = x.mensura < y.mensura ? x.mensura : y.mensura;
    integer d = n > ZEPHYRUM
              ? memcmp(x.datum, y.datum, (size_t)n) : ZEPHYRUM;

    si (d != ZEPHYRUM)
    {
        redde d < ZEPHYRUM ? -I : I;
    }
    si (x.mensura != y.mensura)
    {
        redde x.mensura < y.mensura ? -I : I;
    }
    redde ZEPHYRUM;
}

interior Xar*
comites_enumerare (
    ChronicaFontes* f,
           Piscina* piscina)
{
    hic_manens constans character  praefixum[]  = "comes/";
    hic_manens constans character  cauda[]      = "/paginae";
                              Xar* plagulae;
                              Xar* claves;
                              i32  p;
                              i32  c;
                              i32  i;

    p         = (i32)(magnitudo(praefixum) - I);
    c         = (i32)(magnitudo(cauda) - I);
    claves    = xar_creare(piscina, (i32)magnitudo(chorda));
    plagulae  = volumen_plagulas_enumerare(f->volumen, piscina);
    per (i = ZEPHYRUM; claves && plagulae
        && i < xar_numerus(plagulae); i++)
    {
        chorda via = ((VolumenPlagula*)xar_obtinere(plagulae, i))->via;
        chorda clavis;

        si (   via.mensura                             <= p + c
            || memcmp(via.datum, praefixum, (size_t)p) != ZEPHYRUM
            || memcmp(via.datum + via.mensura - c, cauda, (size_t)c)
               != ZEPHYRUM)
        {
            perge;
        }
        clavis.datum                  = via.datum + p;
        clavis.mensura                = via.mensura - p - c;
        *(chorda*)xar_addere(claves)  = clavis;
    }
    si (claves)
    {
        xar_ordinare(claves, chordas_comparare);
    }
    redde claves;
}

/* ordo intra annum: datae primum (mensis, dies), ceterae aequales -
 * fusio stabilis ordinem libri servat */
interior s32
res_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans ChronicaEventus* x = &((constans ChronicaRes*)a)->eventus;
    constans ChronicaEventus* y = &((constans ChronicaRes*)b)->eventus;

    si (x->annus != y->annus)
    {
        redde x->annus < y->annus ? -I : I;
    }
    si ((x->mensis > ZEPHYRUM) != (y->mensis > ZEPHYRUM))
    {
        redde x->mensis > ZEPHYRUM ? -I : I;
    }
    si (x->mensis != y->mensis)
    {
        redde x->mensis < y->mensis ? -I : I;
    }
    si (x->dies != y->dies)
    {
        redde x->dies < y->dies ? -I : I;
    }
    redde ZEPHYRUM;
}

/* conspectus libri; clavis ultra inscriptionem (conspectus_legere
 * NIHIL): computatio directa in piscina temporaria */
interior constans EventaLibri*
librum_legere (
         ChronicaFontes* f,
     constans character* titulus,
                 chorda  clavis,
                Piscina* tempus)
{
    constans EventaLibri* v;

    v = (constans EventaLibri*)conspectus_legere(f->repertorium,
        titulus, clavis);
    si (v)
    {
        redde v;
    }
    redde titulus == titulus_comitis
        ? comitem_computare(f, clavis, tempus)
        : communem_computare(f, clavis, tempus);
}

/* C5a: entitates ordine octetorum nominis */
interior s32
entitates_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chordas_comparare(&((constans ChronicaEntitas*)a)->titulus,
        &((constans ChronicaEntitas*)b)->titulus);
}

interior vacuum
entitatem_addere (
                     Xar* x,
    ChronicaGenusNominis  genus,
                  chorda  titulus,
                     i32  eventa,
                     i32  declarationes)
{
    ChronicaEntitas* e;

    si (titulus.mensura == ZEPHYRUM)
    {
        redde;
    }
    e                 = (ChronicaEntitas*)xar_addere(x);
    e->genus          = genus;
    e->titulus        = titulus;
    e->eventa         = eventa;
    e->declarationes  = declarationes;
}

/* nomina declarata et ex eventis (people= aut place=), iuncta:
 * eventa et declarationes numerata; chordae iam in tabula */
interior Xar*
entitates_colligere (
    constans ChronicaTabula* t,
       ChronicaGenusNominis  genus,
                    Piscina* piscina,
                    Piscina* tempus)
{
    Xar* omnes;
    Xar* exitus;
    i32  i;
    i32  k;

    omnes   = xar_creare(tempus, (i32)magnitudo(ChronicaEntitas));
    exitus  = xar_creare(piscina, (i32)magnitudo(ChronicaEntitas));
    si (!omnes || !exitus)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(t->res); i++)
    {
        constans ChronicaEventus* e = &((constans ChronicaRes*)
            xar_obtinere(t->res, i))->eventus;

        si (genus == CHRONICA_LOCUS)
        {
            entitatem_addere(omnes, genus, e->locus, I, ZEPHYRUM);
            perge;
        }
        per (k = ZEPHYRUM; e->personae && k < xar_numerus(e->personae);
             k++)
        {
            entitatem_addere(omnes, genus, *(chorda*)xar_obtinere(
                e->personae, k), I, ZEPHYRUM);
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(t->declarationes); i++)
    {
        constans ChronicaDeclaratio* d = &((constans ChronicaNomenRes*)
            xar_obtinere(t->declarationes, i))->declaratio;

        si (d->genus == genus)
        {
            entitatem_addere(omnes, genus, d->titulus, ZEPHYRUM, I);
        }
    }
    xar_ordinare(omnes, entitates_comparare);
    per (i = ZEPHYRUM; i < xar_numerus(omnes); i++)
    {
        constans ChronicaEntitas* e = (constans ChronicaEntitas*)
            xar_obtinere(omnes, i);
        ChronicaEntitas* u = xar_numerus(exitus) > ZEPHYRUM
            ? (ChronicaEntitas*)xar_obtinere(exitus,
                  xar_numerus(exitus) - I) : NIHIL;

        si (u && chorda_aequalis(u->titulus, e->titulus))
        {
            u->eventa         += e->eventa;
            u->declarationes  += e->declarationes;
        }
        alioquin
        {
            *(ChronicaEntitas*)xar_addere(exitus) = *e;
        }
    }
    redde exitus;
}

interior ChronicaTabula*
tabulam_aedificare (
    ChronicaFontes* f,
           Piscina* piscina,
           Piscina* tempus)
{
    ChronicaTabula* t;
               Xar* nomina;
               Xar* loci;
               i32  i;

    t = (ChronicaTabula*)piscina_allocare(piscina,
        magnitudo(ChronicaTabula));
    t->res   = xar_creare(piscina,
        (i32)magnitudo(ChronicaRes));
    t->anni  = xar_creare(piscina, (i32)magnitudo(s32));
    t->declarationes  = xar_creare(piscina,
        (i32)magnitudo(ChronicaNomenRes));
    si (!t->res || !t->anni || !t->declarationes)
    {
        redde NIHIL;
    }
    nomina = xar_creare(tempus, (i32)magnitudo(chorda));
    si (   nomina
        && scriba_liber_nomina_legere(tempus, f->intern, f->volumen,
               NIHIL, nomina))
    {
        per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
        {
            res_copiare(librum_legere(f, titulus_libri,
                *(chorda*)xar_obtinere(nomina, i), tempus), t->res,
                t->declarationes, piscina);
        }
    }
    loci = comites_enumerare(f, tempus);
    per (i = ZEPHYRUM; loci && i < xar_numerus(loci); i++)
    {
        res_copiare(librum_legere(f, titulus_comitis,
            *(chorda*)xar_obtinere(loci, i), tempus), t->res,
            t->declarationes, piscina);
    }
    xar_ordinare(t->res, res_comparare);
    per (i = ZEPHYRUM; i < xar_numerus(t->res); i++)
    {
        s32 annus = ((ChronicaRes*)xar_obtinere(t->res, i))->eventus
            .annus;

        si (   xar_numerus(t->anni) == ZEPHYRUM
            || *(s32*)xar_obtinere(t->anni, xar_numerus(t->anni) - I)
               != annus)
        {
            *(s32*)xar_addere(t->anni) = annus;
        }
    }
    t->personae  = entitates_colligere(t, CHRONICA_PERSONA, piscina,
        tempus);
    t->loca      = entitates_colligere(t, CHRONICA_LOCUS, piscina,
        tempus);
    redde t->personae && t->loca ? t : NIHIL;
}


/* ==================================================
 * Interfacies
 * ================================================== */

ChronicaFontes*
chronica_fontes_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen)
{
    ChronicaFontes* f;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    f = (ChronicaFontes*)piscina_allocare(piscina,
        magnitudo(ChronicaFontes));
    memset(f, ZEPHYRUM, magnitudo(ChronicaFontes));
    f->piscina  = piscina;
    f->intern   = intern;
    f->volumen  = volumen;
    f->stalum   = VERUM;
    redde f;
}

interior b32
declarare (
    ConspectusRegistrum*  repertorium,
         ChronicaFontes*  fontes,
     constans character*  titulus,
                  vacuum* (*computare)(vacuum*, ConspectusRegistrum*,
                                       chorda, Piscina*),
                    Xar** vitia)
{
    ConspectusOpera  opera;
            Piscina* p;
                b32  sana;

    p = piscina_generare_dynamicum("chronica_declaratio", MXXIV);
    si (!p)
    {
        redde FALSUM;
    }
    sana = conspectus_declarare(repertorium,
        conspectus_declaratio_infixa(titulus, p), vitia);
    piscina_destruere(p);
    si (!sana)
    {
        redde FALSUM;
    }
    opera.computare   = computare;
    opera.codificare  = eventa_codificare;
    opera.ctx         = fontes;
    redde conspectus_opera_ponere(repertorium, titulus, opera)
        && conspectus_auditorem_ponere(repertorium, titulus, audire,
               fontes);
}

b32
chronica_registrare (
    ConspectusRegistrum*  repertorium,
         ChronicaFontes*  fontes,
                    Xar** vitia)
{
    si (vitia)
    {
        *vitia = NIHIL;
    }
    si (!repertorium || !fontes)
    {
        redde FALSUM;
    }
    fontes->repertorium = repertorium;
    redde declarare(repertorium, fontes, titulus_comitis,
               comitis_computare, vitia)
        && declarare(repertorium, fontes, titulus_libri,
               libri_computare, vitia);
}

constans ChronicaTabula*
chronica_tabula (
    ChronicaFontes* fontes)
{
           Piscina* p;
           Piscina* tempus;
    ChronicaTabula* t;

    si (!fontes || !fontes->repertorium)
    {
        redde NIHIL;
    }
    (vacuum)conspectus_renovare(fontes->repertorium);
    si (fontes->tabula && !fontes->stalum)
    {
        redde fontes->tabula;
    }
    /* piscina nova ANTE veterem destructam: tabula nova sedem novam
     * habet (eadem sedes = eadem tabula) */
    p       = piscina_generare_dynamicum("chronica_tabula", IV * MXXIV);
    tempus  = piscina_generare_dynamicum("chronica_tempus", IV * MXXIV);
    si (!p || !tempus)
    {
        si (p)
        {
            piscina_destruere(p);
        }
        si (tempus)
        {
            piscina_destruere(tempus);
        }
        redde fontes->tabula;
    }
    fontes->stalum  = FALSUM;
    t               = tabulam_aedificare(fontes, p, tempus);
    piscina_destruere(tempus);
    si (!t)
    {
        piscina_destruere(p);
        fontes->stalum = VERUM;
        redde fontes->tabula;
    }
    si (fontes->piscina_tabulae)
    {
        piscina_destruere(fontes->piscina_tabulae);
    }
    fontes->piscina_tabulae  = p;
    fontes->tabula           = t;
    redde t;
}

interior Xar*
res_quaerere (
    constans ChronicaTabula* tabula,
                     chorda  titulus,
                        b32  personae,
                    Piscina* piscina)
{
    Xar* exitus;
    i32  i;
    i32  k;

    si (!tabula || !piscina)
    {
        redde NIHIL;
    }
    exitus   = xar_creare(piscina, (i32)magnitudo(ChronicaRes));
    titulus  = chorda_praecidere(titulus);
    per (i = ZEPHYRUM; exitus && titulus.mensura > ZEPHYRUM
        && i < xar_numerus(tabula->res); i++)
    {
        constans ChronicaRes* r = (constans ChronicaRes*)xar_obtinere(
            tabula->res, i);
        b32 congruit = FALSUM;

        si (!personae)
        {
            congruit = chorda_aequalis(r->eventus.locus, titulus);
        }
        per (k = ZEPHYRUM; personae && !congruit && r->eventus.personae
            && k < xar_numerus(r->eventus.personae); k++)
        {
            congruit = chorda_aequalis(*(chorda*)xar_obtinere(
                r->eventus.personae, k), titulus);
        }
        si (congruit)
        {
            *(ChronicaRes*)xar_addere(exitus) = *r;
        }
    }
    redde exitus;
}

Xar*
chronica_res_personae (
    constans ChronicaTabula* tabula,
                     chorda  titulus,
                    Piscina* piscina)
{
    redde res_quaerere(tabula, titulus, VERUM, piscina);
}

Xar*
chronica_res_loci (
    constans ChronicaTabula* tabula,
                     chorda  titulus,
                    Piscina* piscina)
{
    redde res_quaerere(tabula, titulus, FALSUM, piscina);
}

Xar*
chronica_declarationes (
    constans ChronicaTabula* tabula,
       ChronicaGenusNominis  genus,
                     chorda  titulus,
                    Piscina* piscina)
{
    Xar* exitus;
    i32  i;

    si (!tabula || !piscina)
    {
        redde NIHIL;
    }
    exitus   = xar_creare(piscina, (i32)magnitudo(ChronicaNomenRes));
    titulus  = chorda_praecidere(titulus);
    per (i = ZEPHYRUM; exitus && tabula->declarationes
        && i < xar_numerus(tabula->declarationes); i++)
    {
        constans ChronicaNomenRes* r = (constans ChronicaNomenRes*)
            xar_obtinere(tabula->declarationes, i);

        si (   r->declaratio.genus == genus
            && chorda_aequalis(r->declaratio.titulus, titulus))
        {
            *(ChronicaNomenRes*)xar_addere(exitus) = *r;
        }
    }
    redde exitus;
}

vacuum
chronica_fontes_destruere (
    ChronicaFontes* fontes)
{
    si (fontes && fontes->piscina_tabulae)
    {
        piscina_destruere(fontes->piscina_tabulae);
        fontes->piscina_tabulae  = NIHIL;
        fontes->tabula           = NIHIL;
    }
}
