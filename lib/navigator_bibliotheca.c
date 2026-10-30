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
#include "scriba_liber.h"
#include "scriba_documentum.h"
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


/* ==================================================
 * Comites: locus canonicus et sanatio
 * ================================================== */

chorda
navigator_locum_formare (
      chorda  articulus,
      chorda  versio,
         b32  sententia,
         s32  positio,
         s32  columna,
     Piscina* piscina)
{
    ChordaAedificator* a;

    a = chorda_aedificator_creare(piscina, (memoriae_index)CXXVIII);
    chorda_aedificator_appendere_literis(a, sententia
        ? "navigator/sententia/" : "navigator/pagina/");
    chorda_aedificator_appendere_chorda(a, articulus);
    chorda_aedificator_appendere_literis(a, "?versio=");
    chorda_aedificator_appendere_chorda(a, versio);
    chorda_aedificator_appendere_literis(a, sententia ? "&sententia="
        : "&pagina=");
    chorda_aedificator_appendere_s32(a, positio);
    si (!sententia && columna >= ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "&columna=");
        chorda_aedificator_appendere_s32(a, columna);
    }
    redde chorda_aedificator_finire(a);
}

/* valor parametri in quaestione ("a=1&b=2"); FALSUM si abest */
interior b32
parametrum (
                chorda  quaestio,
    constans character* titulus,
                chorda* valor)
{
    i32 i;
    i32 n = (i32)strlen(titulus);

    i = ZEPHYRUM;
    dum (i < quaestio.mensura)
    {
        i32 finis = i;

        dum (finis < quaestio.mensura && quaestio.datum[finis] != '&')
        {
            finis++;
        }
        si (   finis - i > n
            && memcmp(quaestio.datum + i, titulus, (size_t)n)
               == ZEPHYRUM
            && quaestio.datum[i + n] == '=')
        {
            valor->datum    = quaestio.datum + i + n + I;
            valor->mensura  = finis - i - n - I;
            redde VERUM;
        }
        i = finis + I;
    }
    redde FALSUM;
}

interior s32
parametrum_s32 (
                chorda  quaestio,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda v;
       s32 n;

    redde parametrum(quaestio, titulus, &v) && chorda_ut_s32(v, &n)
        ? n : praestitutum;
}

/* locus canonicus loci veteris ("navigator/pagina/<art>?..."); vacua
 * si forma ignota aut articulus ignotus */
interior chorda
locum_canonicum (
    NavigatorBibliotheca* b,
                  chorda  locus,
                 Piscina* piscina)
{
    hic_manens constans character pagina[] = "navigator/pagina/";
    hic_manens constans character sententia[] =
        "navigator/sententia/";
                               b32  in_sententia;
                               i32  p;
                               i32  q;
                            chorda  articulus;
                            chorda  quaestio;
                            chorda  versio;
                NavigatorArticulus* a;

    p             = (i32)strlen(pagina);
    in_sententia  = FALSUM;
    si (   locus.mensura > (i32)strlen(sententia)
        && memcmp(locus.datum, sententia, strlen(sententia))
            == ZEPHYRUM)
    {
        p             = (i32)strlen(sententia);
        in_sententia  = VERUM;
    }
    alioquin si (   locus.mensura                          <= p
                 || memcmp(locus.datum, pagina, (size_t)p) != ZEPHYRUM)
    {
        redde vacua();
    }
    q = p;
    dum (q < locus.mensura && locus.datum[q] != '?')
    {
        q++;
    }
    articulus.datum    = locus.datum + p;
    articulus.mensura  = q - p;
    quaestio.datum     = locus.datum + q + (q < locus.mensura ? I : 0);
    quaestio.mensura   = q < locus.mensura ? locus.mensura - q - I
                                           : ZEPHYRUM;
    a = articulum_per_id(b, articulus);
    si (!a || xar_numerus(a->versiones) == ZEPHYRUM)
    {
        redde vacua();
    }
    si (!parametrum(quaestio, "versio", &versio) || versio.mensura == 0)
    {
        versio = ((NavigatorVersio*)xar_obtinere(a->versiones,
            xar_numerus(a->versiones) - I))->identitas;
    }
    redde navigator_locum_formare(articulus, versio, in_sententia,
        parametrum_s32(quaestio, in_sententia ? "sententia" : "pagina",
        ZEPHYRUM), in_sententia ? -I : parametrum_s32(quaestio,
        "columna", -I), piscina);
}

/* liber vetus in canonicum: absens - paginae omnes (nomina, nota
 * comitis); praesens - paginae non vacuae ad finem */
interior b32
librum_transferre (
    NavigatorBibliotheca* b,
     InternamentumChorda* intern,
      constans character* vetus,
      constans character* canon,
                 Piscina* piscina)
{
         ScribaLiber* fons;
         ScribaLiber* meta;
                 Xar* nomina;
                 b32  novus;
              chorda  titulus;
              chorda  semita;
                 i32  i;

    nomina = xar_creare(piscina, (i32)magnitudo(chorda));
    si (   !nomina
        || !scriba_liber_nomina_legere(piscina, intern, b->volumen,
               vetus, nomina))
    {
        redde FALSUM;
    }
    novus = !scriba_liber_exstat(b->volumen, canon, piscina);
    fons  = scriba_liber_aperire_in_spatio(piscina, intern, b->volumen,
        vetus, LXXX, XX);
    meta  = NIHIL;
    per (i = ZEPHYRUM; fons && i < xar_numerus(nomina); i++)
    {
                   chorda  titulus_paginae;
         ScribaDocumentum* a;
         ScribaDocumentum* d;
                      Xar* lineae;

        titulus_paginae  = *(chorda*)xar_obtinere(nomina, i);
        a                = scriba_liber_pagina(fons, titulus_paginae);
        lineae           = xar_creare(piscina, (i32)magnitudo(chorda));
        si (!a || !lineae)
        {
            redde FALSUM;
        }
        (vacuum)scriba_liber_paginam_legere_omnes(piscina, intern,
            b->volumen, vetus, titulus_paginae, lineae);
        si (!novus && xar_numerus(lineae) == ZEPHYRUM)
        {
            perge;
        }
        si (!meta)
        {
            meta = scriba_liber_aperire_in_spatio(piscina, intern,
                b->volumen, canon, a->latitudo, a->altitudo);
            si (!meta)
            {
                redde FALSUM;
            }
        }
        si (!novus)
        {
            chorda nova = scriba_liber_pagina_nova(meta);

            d = nova.mensura > ZEPHYRUM ? scriba_liber_pagina(meta,
                nova)
                                        : NIHIL;
        }
        alioquin si (chorda_aequalis_literis(titulus_paginae, "1"))
        {
            d = scriba_liber_pagina(meta, titulus_paginae);
        }
        alioquin
        {
            d = scriba_liber_paginam_condere(meta, titulus_paginae);
        }
        si (   !d
            || scriba_documentum_committere(d,
                   scriba_documentum_tabula(a)) < ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    /* nota comitis: canonicus sine nota eam a vetere accipit */
    si (   !scriba_liber_comitem_legere(piscina, intern, b->volumen,
               canon, &titulus, &semita)
        && scriba_liber_comitem_legere(piscina, intern, b->volumen,
               vetus, &titulus, &semita))
    {
        (vacuum)scriba_liber_comitem_notare(b->volumen, intern, canon,
            titulus, semita, piscina);
    }
    redde VERUM;
}

/* plagulae omnes sub "<praefixum>/" removere, transactione una */
interior b32
librum_removere (
    NavigatorBibliotheca* b,
                  chorda  praefixum,
                 Piscina* piscina)
{
       Xar* plagulae;
    chorda  sub;
       b32  bene;
       i32  i;

    sub = chorda_concatenare(praefixum, chorda_ex_literis("/", piscina),
        piscina);
    plagulae = volumen_plagulas_enumerare(b->volumen, piscina);
    si (!plagulae || !volumen_transactionem_incipere(b->volumen))
    {
        redde FALSUM;
    }
    bene = VERUM;
    per (i = ZEPHYRUM; bene && i < xar_numerus(plagulae); i++)
    {
        chorda via = ((VolumenPlagula*)xar_obtinere(plagulae, i))->via;

        si (chorda_incipit(via, sub))
        {
            bene = volumen_plagulam_removere(b->volumen, via);
        }
    }
    redde bene ? volumen_transactionem_committere(b->volumen)
               : (volumen_transactionem_revolvere(b->volumen), FALSUM);
}

b32
navigator_comites_sanare (
    NavigatorBibliotheca* b,
     InternamentumChorda* intern,
                 Piscina* piscina,
                     i32* translati)
{
    hic_manens constans character  praefixum[]  = "comes/navigator/";
    hic_manens constans character  cauda[]      = "/paginae";
                          Piscina* tp;
                              Xar* plagulae;
                              Xar* loci;
                              i32  i;
                              i32  p;
                              i32  c;
                              b32  bene;

    si (translati)
    {
        *translati = ZEPHYRUM;
    }
    si (!b || !intern || !piscina)
    {
        redde FALSUM;
    }
    tp = piscina_generare_dynamicum("navigator_comites", IV * MXXIV);
    si (!tp)
    {
        redde FALSUM;
    }
    p         = (i32)strlen("comes/");
    c         = (i32)strlen(cauda);
    loci      = xar_creare(tp, (i32)magnitudo(chorda));
    plagulae  = volumen_plagulas_enumerare(b->volumen, tp);
    per (i = ZEPHYRUM; loci && plagulae && i < xar_numerus(plagulae);
         i++)
    {
        chorda via = ((VolumenPlagula*)xar_obtinere(plagulae, i))->via;
        chorda locus;

        si (   via.mensura <= (i32)strlen(praefixum) + c
            || memcmp(via.datum, praefixum, strlen(praefixum))
               != ZEPHYRUM
            || memcmp(via.datum + via.mensura - c, cauda, (size_t)c)
               != ZEPHYRUM)
        {
            perge;
        }
        locus.datum                 = via.datum + p;
        locus.mensura               = via.mensura - p - c;
        *(chorda*)xar_addere(loci)  = chorda_transcribere(locus, tp);
    }
    bene = loci && plagulae;
    per (i = ZEPHYRUM; bene && i < xar_numerus(loci); i++)
    {
        chorda locus = *(chorda*)xar_obtinere(loci, i);
        chorda canon = locum_canonicum(b, locus, tp);
        chorda vetus;

        si (canon.mensura == ZEPHYRUM || chorda_aequalis(canon, locus))
        {
            perge;
        }
        vetus = chorda_concatenare(chorda_ex_literis("comes/", tp),
            locus,
            tp);
        canon = chorda_concatenare(chorda_ex_literis("comes/", tp),
            canon,
            tp);
        bene = librum_transferre(b, intern, chorda_ut_cstr(vetus, tp),
                   chorda_ut_cstr(canon, tp), tp)
            && librum_removere(b, vetus, tp);
        si (bene && translati)
        {
            (*translati)++;
        }
    }
    piscina_destruere(tp);
    redde bene;
}
