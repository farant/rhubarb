/* navigator_bibliotheca.c - articuli servati et versiones (navigator
 * N2.4; vide caput). Index = Xar articulorum in piscina bibliothecae,
 * ex actis propriis refectus: apertio omnia legit, scriptura acta sua
 * post seq ultimum lectum iterum legit - via una, ergo index et acta
 * numquam dissentiunt. Extractio (HTML magnum -> arbor) in piscina
 * temporaria, deleta post oblationem. */
#include "navigator_bibliotheca.h"
#include "chorda_aedificator.h"
#include "json.h"
#include "sigillum.h"
#include <stdio.h>
#include <string.h>

#define GENUS_ARTICULI   "navigator-articulus"
#define GENUS_VERSIONIS  "navigator-versio"
#define GENUS_FAVORIS    "navigator-favor"     /* N3.4 */
#define ALVEUS_EXTRACTIONIS ((memoriae_index)(IV * M * M))

structura NavigatorBibliotheca {
       Volumen* volumen;
       Piscina* piscina;
    MonetaFons  fons;
        vacuum* fontis_ctx;
           Xar* articuli;     /* NavigatorArticulus */
           s64  seq;          /* actum ultimum lectum */
};

interior chorda
vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

interior NavigatorArticulus*
articulum_per_id (
    NavigatorBibliotheca* b,
                  chorda  identitas)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(b->articuli); i++)
    {
        NavigatorArticulus* a = (NavigatorArticulus*)xar_obtinere(
            b->articuli, i);

        si (chorda_aequalis(a->identitas, identitas))
        {
            redde a;
        }
    }
    redde NIHIL;
}

/* actum unum in indicem */
interior vacuum
actum_applicare (
         NavigatorBibliotheca* b,
        constans VolumenActum* actum)
{
          JsonResultus  r;
    NavigatorArticulus* a;
       NavigatorVersio* v;
                chorda  identitas;

    r = json_legere(actum->datum, b->piscina);
    si (!r.successus || !r.radix)
    {
        redde;
    }
    si (chorda_aequalis_literis(actum->genus, GENUS_FAVORIS))
    {
        /* N3.4: favor articuli - actum ultimum vincit */
        a = articulum_per_id(b, json_capere_chorda(r.radix, "articulus",
            vacua()));
        si (a)
        {
            a->favor = json_capere_boolean(r.radix, "favor", FALSUM);
        }
        redde;
    }
    identitas = json_capere_chorda(r.radix, "id", vacua());
    si (identitas.mensura == ZEPHYRUM)
    {
        redde;
    }
    si (chorda_aequalis_literis(actum->genus, GENUS_ARTICULI))
    {
        si (articulum_per_id(b, identitas))
        {
            redde;
        }
        a = (NavigatorArticulus*)xar_addere(b->articuli);
        si (!a)
        {
            redde;
        }
        a->identitas  = identitas;
        a->url        = json_capere_chorda(r.radix, "url", vacua());
        a->situs      = json_capere_chorda(r.radix, "situs", vacua());
        a->versiones = xar_creare(b->piscina,
            magnitudo(NavigatorVersio));
        a->favor      = FALSUM;
        redde;
    }
    a = articulum_per_id(b, json_capere_chorda(r.radix, "articulus",
        vacua()));
    si (!a)
    {
        redde;
    }
    v = (NavigatorVersio*)xar_addere(a->versiones);
    si (!v)
    {
        redde;
    }
    v->identitas  = identitas;
    v->momentum   = actum->momentum;
    v->url        = json_capere_chorda(r.radix, "url", vacua());
    v->titulus    = json_capere_chorda(r.radix, "titulus", vacua());
    v->massa      = json_capere_chorda(r.radix, "massa", vacua());
}

/* acta propria post b->seq in indicem */
interior vacuum
acta_legere (
    NavigatorBibliotheca* b)
{
     constans character* genera[IV];
                 chorda  replica;
                    Xar* acta;
                    i32  i;

    genera[ZEPHYRUM]  = GENUS_ARTICULI;
    genera[I]         = GENUS_VERSIONIS;
    genera[II]        = GENUS_FAVORIS;
    genera[III]       = NIHIL;
    replica           = volumen_replica(b->volumen);
    acta = replica.mensura > ZEPHYRUM
        ? volumen_acta_generum_replicae_legere(b->volumen, replica,
            b->seq, genera, b->piscina)
        : volumen_acta_generum_legere(b->volumen, b->seq, genera,
            b->piscina);
    per (i = ZEPHYRUM; acta && i < xar_numerus(acta); i++)
    {
        constans VolumenActum* actum = (constans VolumenActum*)
            xar_obtinere(acta, i);

        si (actum->seq > b->seq)
        {
            b->seq = actum->seq;
        }
        actum_applicare(b, actum);
    }
}

NavigatorBibliotheca*
navigator_bibliotheca_aperire (
       Volumen* volumen,
    MonetaFons  fons,
        vacuum* fontis_ctx,
       Piscina* piscina)
{
    NavigatorBibliotheca* b;

    si (!volumen || !piscina)
    {
        redde NIHIL;
    }
    b = (NavigatorBibliotheca*)piscina_allocare(piscina,
        magnitudo(NavigatorBibliotheca));
    si (!b)
    {
        redde NIHIL;
    }
    memset(b, ZEPHYRUM, magnitudo(NavigatorBibliotheca));
    b->volumen     = volumen;
    b->piscina     = piscina;
    b->fons        = fons;
    b->fontis_ctx  = fontis_ctx;
    b->articuli    = xar_creare(piscina, magnitudo(NavigatorArticulus));
    acta_legere(b);
    redde b;
}

i32
navigator_articulorum_numerus (
    constans NavigatorBibliotheca* b)
{
    redde b ? xar_numerus(b->articuli) : ZEPHYRUM;
}

constans NavigatorArticulus*
navigator_articulus (
    constans NavigatorBibliotheca* b,
                              i32  k)
{
    si (!b || k >= xar_numerus(b->articuli))
    {
        redde NIHIL;
    }
    redde (constans NavigatorArticulus*)xar_obtinere(b->articuli, k);
}

constans NavigatorArticulus*
navigator_articulum_invenire (
    constans NavigatorBibliotheca* b,
                           chorda  url)
{
    i32 i;

    per (i = ZEPHYRUM; b && i < xar_numerus(b->articuli); i++)
    {
        constans NavigatorArticulus* a = (constans NavigatorArticulus*)
            xar_obtinere(b->articuli, i);

        si (chorda_aequalis(a->url, url))
        {
            redde a;
        }
    }
    redde NIHIL;
}

/* ULID novum (aut fons probationis) in piscina */
interior chorda
identitatem_cudere (
    NavigatorBibliotheca* b,
                 Piscina* piscina)
{
    character effusio[MONETA_ULID_MENSURA];

    memset(effusio, ZEPHYRUM, magnitudo(effusio));
    si (b->fons)
    {
        b->fons(b->fontis_ctx, effusio);
    }
    alioquin
    {
        moneta_ulid(effusio);
    }
    effusio[MONETA_ULID_MENSURA - I] = '\0';
    redde chorda_ex_literis(effusio, piscina);
}

/* "clavis":"valor" (cum comma praecedente nisi primum) */
interior vacuum
campum_scribere (
     ChordaAedificator* aed,
    constans character* clavis,
                chorda  valor,
                   b32  primum)
{
    si (!primum)
    {
        chorda_aedificator_appendere_character(aed, ',');
    }
    chorda_aedificator_appendere_character(aed, '"');
    chorda_aedificator_appendere_literis(aed, clavis);
    chorda_aedificator_appendere_literis(aed, "\":\"");
    chorda_aedificator_appendere_evasus_json(aed, valor);
    chorda_aedificator_appendere_character(aed, '"');
}

interior b32
actum_articuli_scribere (
    NavigatorBibliotheca* b,
                  chorda  identitas,
                  chorda  url,
      constans character* situs,
                 Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, CCLVI);

    chorda_aedificator_appendere_character(aed, '{');
    campum_scribere(aed, "id", identitas, VERUM);
    campum_scribere(aed, "url", url, FALSUM);
    campum_scribere(aed, "situs", chorda_ex_literis(situs, piscina),
        FALSUM);
    chorda_aedificator_appendere_character(aed, '}');
    redde volumen_actum_appendere(b->volumen, GENUS_ARTICULI,
        chorda_aedificator_finire(aed)) > ZEPHYRUM;
}

interior b32
actum_versionis_scribere (
    NavigatorBibliotheca* b,
                  chorda  identitas,
                  chorda  articulus,
      constans character* massa,
                  chorda  url_finalis,
                  chorda  titulus,
                 Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, CCLVI);

    chorda_aedificator_appendere_character(aed, '{');
    campum_scribere(aed, "id", identitas, VERUM);
    campum_scribere(aed, "articulus", articulus, FALSUM);
    campum_scribere(aed, "massa", chorda_ex_literis(massa, piscina),
        FALSUM);
    campum_scribere(aed, "url", url_finalis, FALSUM);
    campum_scribere(aed, "titulus", titulus, FALSUM);
    chorda_aedificator_appendere_character(aed, '}');
    redde volumen_actum_appendere(b->volumen, GENUS_VERSIONIS,
        chorda_aedificator_finire(aed)) > ZEPHYRUM;
}

/* contentum versionis ultimae articuli idem ac sigillum datum? FALSUM
 * etiam si massa abest aut extractio deficit (comparari non potest ->
 * versio nova servatur) */
interior b32
ultima_eadem (
           NavigatorBibliotheca* b,
    constans NavigatorArticulus* a,
                         chorda  url,
             NavigatorExtrahere  extrahere,
             constans character* sigillum_novum,
                        Piscina* tp)
{
    constans NavigatorVersio* ultima;
                      chorda  html;
                         b32  inventum = FALSUM;
                   Articulus* vetus;
                   character  hex[SIGILLUM_HEX_MENSURA];

    si (xar_numerus(a->versiones) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    ultima = (constans NavigatorVersio*)xar_obtinere(a->versiones,
        xar_numerus(a->versiones) - I);
    html = volumen_massam_promere(b->volumen, ultima->massa, tp,
        &inventum);
    si (!inventum)
    {
        redde FALSUM;
    }
    vetus = extrahere(html, chorda_ut_cstr(url, tp), tp);
    si (!vetus)
    {
        redde FALSUM;
    }
    articulus_sigillum(vetus, hex);
    redde strcmp(hex, sigillum_novum) == ZEPHYRUM;
}

interior constans NavigatorVersio*
versio_ultima (
    constans NavigatorArticulus* a)
{
    si (!a || xar_numerus(a->versiones) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde (constans NavigatorVersio*)xar_obtinere(a->versiones,
        xar_numerus(a->versiones) - I);
}

NavigatorExitus
navigator_versionem_offerre (
              NavigatorBibliotheca* b,
                            chorda  url,
                            chorda  url_finalis,
               constans character* situs,
                            chorda  html,
                NavigatorExtrahere  extrahere,
    constans NavigatorVersio**      versio)
{
    constans NavigatorArticulus* a;
                        Piscina* tp;
                      Articulus* novus;
                      character  sigillum[SIGILLUM_HEX_MENSURA];
                      character  massa[SIGILLUM_HEX_MENSURA];
                         chorda  id_articuli;
                            b32  bene;
                NavigatorExitus  exitus;

    si (   !b || !extrahere || !situs || url.mensura == ZEPHYRUM
        || html.mensura == ZEPHYRUM)
    {
        redde NAVIGATOR_VITIUM;
    }
    tp = piscina_generare_dynamicum("navigator_extractio",
        ALVEUS_EXTRACTIONIS);
    si (!tp)
    {
        redde NAVIGATOR_VITIUM;
    }
    /* fons = URL canonica in utraque extractione: nexus eodem modo
     * solvuntur quamvis URL petitionis differat */
    novus = extrahere(html, chorda_ut_cstr(url, tp), tp);
    si (!novus)
    {
        piscina_destruere(tp);
        redde NAVIGATOR_VITIUM;
    }
    articulus_sigillum(novus, sigillum);
    a = navigator_articulum_invenire(b, url);
    si (a && ultima_eadem(b, a, url, extrahere, sigillum, tp))
    {
        piscina_destruere(tp);
        si (versio)
        {
            *versio = versio_ultima(a);
        }
        redde NAVIGATOR_IDEM;
    }

    bene = volumen_transactionem_incipere(b->volumen)
        && volumen_massam_condere(b->volumen, html, massa);
    id_articuli = a ? a->identitas : identitatem_cudere(b, tp);
    si (bene && !a)
    {
        bene = actum_articuli_scribere(b, id_articuli, url, situs, tp);
    }
    si (bene)
    {
        bene = actum_versionis_scribere(b, identitatem_cudere(b, tp),
            id_articuli, massa, url_finalis.mensura > ZEPHYRUM
            ? url_finalis : url, novus->titulus, tp);
    }
    bene = bene && volumen_transactionem_committere(b->volumen);
    si (!bene)
    {
        si (volumen_in_transactione(b->volumen))
        {
            (vacuum)volumen_transactionem_revolvere(b->volumen);
        }
        piscina_destruere(tp);
        redde NAVIGATOR_VITIUM;
    }
    exitus = a ? NAVIGATOR_VERSIO_NOVA : NAVIGATOR_NOVUM;
    piscina_destruere(tp);
    acta_legere(b);
    si (versio)
    {
        *versio = versio_ultima(navigator_articulum_invenire(b, url));
    }
    redde exitus;
}

b32
navigator_favorem_ponere (
    NavigatorBibliotheca* b,
                  chorda  articulus,
                     b32  favor)
{
    NavigatorArticulus* a;
     ChordaAedificator* aed;
               Piscina* tp;
                   b32  bene;

    a = b ? articulum_per_id(b, articulus) : NIHIL;
    si (!a)
    {
        redde FALSUM;
    }
    favor = favor != FALSUM;
    si (a->favor == favor)
    {
        redde VERUM;   /* status idem: nihil scriptum */
    }
    tp = piscina_generare_dynamicum("navigator_favor", MXXIV);
    si (!tp)
    {
        redde FALSUM;
    }
    aed = chorda_aedificator_creare(tp, CXXVIII);
    chorda_aedificator_appendere_character(aed, '{');
    campum_scribere(aed, "articulus", a->identitas, VERUM);
    chorda_aedificator_appendere_literis(aed, favor ? ",\"favor\":true}"
        : ",\"favor\":false}");
    bene = volumen_actum_appendere(b->volumen, GENUS_FAVORIS,
        chorda_aedificator_finire(aed)) > ZEPHYRUM;
    piscina_destruere(tp);
    si (bene)
    {
        acta_legere(b);
    }
    redde bene;
}

/* clavis praesentiae: "navigator/positio/<ULID>" (ULID tutus in via) */
interior chorda
clavem_positionis (
     chorda  articulus,
    Piscina* piscina)
{
    redde chorda_concatenare(chorda_ex_literis("navigator/positio/",
        piscina), articulus, piscina);
}

b32
navigator_positionem_scribere (
              NavigatorBibliotheca* b,
                            chorda  articulus,
         constans NavigatorPositio* positio)
{
    ChordaAedificator* aed;
              Piscina* tp;
            character  numerus[XXIV];
                  b32  bene;

    si (!b || !positio || !articulum_per_id(b, articulus))
    {
        redde FALSUM;
    }
    tp = piscina_generare_dynamicum("navigator_positio", MXXIV);
    si (!tp)
    {
        redde FALSUM;
    }
    aed = chorda_aedificator_creare(tp, CXXVIII);
    chorda_aedificator_appendere_character(aed, '{');
    campum_scribere(aed, "versio", positio->versio, VERUM);
    sprintf(numerus, ",\"offset\":%u", (insignatus integer)
        positio->offset);
    chorda_aedificator_appendere_literis(aed, numerus);
    chorda_aedificator_appendere_literis(aed, positio->in_nexibus
        ? ",\"in_nexibus\":true}" : ",\"in_nexibus\":false}");
    bene = volumen_praesens_ponere(b->volumen, clavem_positionis(
        articulus, tp), chorda_aedificator_finire(aed));
    piscina_destruere(tp);
    redde bene;
}

b32
navigator_positionem_legere (
    NavigatorBibliotheca* b,
                  chorda  articulus,
        NavigatorPositio* positio,
                 Piscina* piscina)
{
          chorda valor;
             b32 inventum;
    JsonResultus r;
             s64 offset;

    si (!b || !positio || !piscina || !articulum_per_id(b, articulus))
    {
        redde FALSUM;
    }
    valor = volumen_praesens_legere(b->volumen, clavem_positionis(
        articulus, piscina), piscina, &inventum);
    si (!inventum)
    {
        redde FALSUM;
    }
    r = json_legere(valor, piscina);
    si (!r.successus || !r.radix)
    {
        redde FALSUM;
    }
    offset = json_capere_integer(r.radix, "offset", -I);
    si (offset < ZEPHYRUM)
    {
        redde FALSUM;
    }
    positio->versio      = json_capere_chorda(r.radix, "versio",
        vacua());
    positio->offset      = (i32)offset;
    positio->in_nexibus  = json_capere_boolean(r.radix, "in_nexibus",
        FALSUM);
    redde VERUM;
}

#define CLAVIS_RECENTIUM "navigator/recentes"

/* linea proxima ab *i in valore ('\n' separat); *i post '\n'
 * promovetur (semper progreditur dum *i < mensura) */
interior chorda
lineam_proximam (
    chorda  valor,
       i32* i)
{
    i32 a;
    i32 b;

    a = *i;
    dum (*i < valor.mensura && valor.datum[*i] != '\n')
    {
        (*i)++;
    }
    b = *i;
    si (*i < valor.mensura)
    {
        (*i)++;   /* '\n' praeteritur */
    }
    redde chorda_sectio(valor, a, b);
}

b32
navigator_recentem_notare (
    NavigatorBibliotheca* b,
                  chorda  articulus)
{
    ChordaAedificator* aed;
              Piscina* tp;
               chorda  vetus;
               chorda  id;
                  b32  inventum;
                  b32  bene;
                  i32  i;
                  i32  n;

    si (!b || !articulum_per_id(b, articulus))
    {
        redde FALSUM;
    }
    tp = piscina_generare_dynamicum("navigator_recentes", MMXLVIII);
    si (!tp)
    {
        redde FALSUM;
    }
    vetus = volumen_praesens_legere(b->volumen, chorda_ex_literis(
        CLAVIS_RECENTIUM, tp), tp, &inventum);
    aed = chorda_aedificator_creare(tp, DXII);
    chorda_aedificator_appendere_chorda(aed, articulus);
    n = I;
    i = ZEPHYRUM;
    dum (inventum && i < vetus.mensura && n < NAVIGATOR_RECENTES_MAXIMI)
    {
        id = lineam_proximam(vetus, &i);
        si (id.mensura > ZEPHYRUM && !chorda_aequalis(id, articulus))
        {
            chorda_aedificator_appendere_character(aed, '\n');
            chorda_aedificator_appendere_chorda(aed, id);
            n++;
        }
    }
    bene = volumen_praesens_ponere(b->volumen, chorda_ex_literis(
        CLAVIS_RECENTIUM, tp), chorda_aedificator_finire(aed));
    piscina_destruere(tp);
    redde bene;
}

Xar*
navigator_recentes (
    NavigatorBibliotheca* b,
                 Piscina* piscina)
{
                            Xar*  l;
                         chorda   valor;
                         chorda   id;
                            b32   inventum;
                            i32   i;
    constans NavigatorArticulus** e;
    constans NavigatorArticulus*  a;

    l = piscina ? xar_creare(piscina,
        (i32)magnitudo(constans NavigatorArticulus*)) : NIHIL;
    si (!l || !b)
    {
        redde l;
    }
    valor = volumen_praesens_legere(b->volumen, chorda_ex_literis(
        CLAVIS_RECENTIUM, piscina), piscina, &inventum);
    i = ZEPHYRUM;
    dum (inventum && i < valor.mensura)
    {
        id  = lineam_proximam(valor, &i);
        a   = id.mensura > ZEPHYRUM ? articulum_per_id(b, id) : NIHIL;
        e   = a ? (constans NavigatorArticulus**)xar_addere(l) : NIHIL;
        si (e)
        {
            *e = a;
        }
    }
    redde l;
}

chorda
navigator_versionis_html (
           NavigatorBibliotheca* b,
       constans NavigatorVersio* versio,
                        Piscina* piscina)
{
       b32 inventum = FALSUM;
    chorda html;

    si (!b || !versio || !piscina)
    {
        redde vacua();
    }
    html = volumen_massam_promere(b->volumen, versio->massa, piscina,
        &inventum);
    redde inventum ? html : vacua();
}
