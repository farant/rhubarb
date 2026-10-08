/* vicus.c - hospes applicationum (insula-rami-plan T1b) */

#include "vicus.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "thema.h"
#include "mandatum.h"
#include "componens.h"

#include <stdio.h>
#include <string.h>

hic_manens constans character clavis_indicis[] = "vicus/tabulae";
/* tabula finita (vicus-latera S1c): post titulum in linea, numquam in
 * indice */
hic_manens constans character suffixum_finitae[] = " [exitus]";


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
chorda_nulla_vici (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

/* radix + "/" + via (radix NIHIL aut vacua: via sola) */
interior constans character*
via_plena (
               Piscina* p,
    constans character* radix,
    constans character* via)
{
    chorda c;

    si (!radix || radix[ZEPHYRUM] == '\0')
    {
        redde via;
    }
    c = chorda_concatenare(chorda_ex_literis(radix, p),
        chorda_ex_literis("/", p), p);
    c = chorda_concatenare(c, chorda_ex_literis(via, p), p);
    redde chorda_ut_cstr(c, p);
}

interior Canon*
canonem_legere (
                  Vicus* v,
     constans character* via)
{
    chorda causa;

    redde canon_legere(filum_legere_totum(via_plena(v->piscina,
        v->radix,
        via), v->piscina), v->piscina, v->intern, &causa);
}

interior constans VicusGenus*
genus_invenire (
    constans Vicus* v,
            chorda  titulus)
{
    VicusGenus* g;
           i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(v->genera); i++)
    {
        g = (VicusGenus*)xar_obtinere(v->genera, i);
        si (chorda_aequalis(g->titulus, titulus))
        {
            redde g;
        }
    }
    redde NIHIL;
}

interior VicusTabula*
tabula_invenire (
    constans Vicus* v,
            chorda  id)
{
    VicusTabula* t;
            i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, i);
        si (chorda_aequalis(t->id, id))
        {
            redde t;
        }
    }
    redde NIHIL;
}

interior chorda
internare (
                  Vicus* v,
     constans character* litterae)
{
    chorda* c;

    c = chorda_internare_ex_literis(v->intern, litterae);
    redde c ? *c : chorda_nulla_vici();
}

/* index in volumen (superscribitur) */
interior b32
indicem_scribere (
    Vicus* v)
{
    ChordaAedificator* a;
          VicusTabula* t;
                  i32  i;

    a = chorda_aedificator_creare(v->piscina, (memoriae_index)CCLVI);
    chorda_aedificator_appendere_literis(a, "<tabulae activa=\"");
    chorda_aedificator_appendere_chorda(a, v->activa);
    chorda_aedificator_appendere_literis(a, "\">");
    per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, i);
        chorda_aedificator_appendere_literis(a, "<tabula id=\"");
        chorda_aedificator_appendere_chorda(a, t->id);
        chorda_aedificator_appendere_literis(a, "\" genus=\"");
        chorda_aedificator_appendere_chorda(a, t->genus);
        chorda_aedificator_appendere_literis(a, "\" titulus=\"");
        chorda_aedificator_appendere_chorda(a, t->titulus);
        chorda_aedificator_appendere_literis(a, "\"/>");
    }
    chorda_aedificator_appendere_literis(a, "</tabulae>");
    redde volumen_plagulam_condere(v->volumen,
        chorda_ex_literis(clavis_indicis, v->piscina),
        chorda_aedificator_finire(a), "vicus:tabulae");
}

/* tabulam montare (descriptio NIHIL: praeteritur, causa) */
interior vacuum
tabulam_montare (
          Vicus* v,
    VicusTabula* t)
{
    si (!t->descriptio)
    {
        v->causa = chorda_concatenare(chorda_ex_literis(
            "genus ignotum, tabula non montata: ", v->piscina),
            t->genus,
            v->piscina);
        redde;
    }
    t->montatio = piscina_allocare(v->piscina, t->descriptio->mensura);
    si (!t->montatio)
    {
        redde;
    }
    memset(t->montatio, ZEPHYRUM, t->descriptio->mensura);
    t->montata = t->descriptio->montare(t->montatio, v->piscina,
        v->intern,
        v->volumen, v->repo, chorda_ut_cstr(t->id, v->piscina),
        v->radix,
        v->latitudo, v->altitudo);
    si (!t->montata)
    {
        v->causa = chorda_concatenare(chorda_ex_literis(
            "montatio defecit: ", v->piscina), t->id, v->piscina);
        redde;
    }
    memset(&t->facies, ZEPHYRUM, magnitudo(VicusFacies));
    t->descriptio->describere(t->montatio, &t->facies);
}

/* superficies montationum (T2b): applicatio superficiem ex ramo suo
 * legit; hospes eam scribit nomine 'dispensator' (domini
 * applicationum superficiem dispensatori addicunt - in vico hospes
 * dispensator montationum est) */
nomen structura {
    s32 latitudo;
    s32 altitudo;
} Superficies;

interior vacuum
superficiem_mutator (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans Superficies* sp;

    sp = (constans Superficies*)ctx;
    insula_attributum_ponere(nodus, p, in, "superficies_latitudo",
        chorda_ut_cstr(chorda_ex_s32(sp->latitudo, p), p));
    insula_attributum_ponere(nodus, p, in, "superficies_altitudo",
        chorda_ut_cstr(chorda_ex_s32(sp->altitudo, p), p));
}

interior vacuum
superficies_scribere (
    Vicus* v,
      s32  latitudo,
      s32  altitudo)
{
     VicusTabula* t;
     InsulaRamus  ramus;
     Superficies  sp;
          chorda  prior;
             i32  i;

    sp.latitudo = latitudo;
    sp.altitudo = altitudo - VICUS_ALTITUDO_TABULARUM;
    si (sp.altitudo < ZEPHYRUM)
    {
        sp.altitudo = ZEPHYRUM;
    }
    prior = v->repo->scriptor;
    insula_scriptorem_ponere(v->repo, chorda_ex_literis("dispensator",
        v->piscina));
    per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, i);
        si (!t->montata)
        {
            perge;
        }
        ramus = insula_ramus(v->repo, chorda_ut_cstr(t->genus,
            v->piscina),
                             chorda_ut_cstr(t->id, v->piscina));
        (vacuum)mutare_ramum(&ramus, INSULA_EPHEMERA,
            superficiem_mutator,
                             &sp);
    }
    insula_scriptorem_ponere(v->repo, prior);
}


/* ==================================================
 * Praefixum (T3b): Ctrl-A, deinde n / p / 1-9 / Ctrl-A
 * ================================================== */

interior b32
est_imperium_a (
    constans Eventus* ev)
{
    redde ev
        && (   ev->genus == EVENTUS_CLAVIS_DEPRESSUS
            || ev->genus == EVENTUS_CLAVIS_LIBERATUS)
        && (ev->datum.clavis.modificantes & MOD_IMPERIUM)
        && ev->datum.clavis.runa == 'a';
}

/* arbor praefixum pendens notat (vicus_componere: titulus radicis) */
interior b32
arbor_pendet (
    constans Componens* arbor)
{
    redde arbor && chorda_aequalis_literis(arbor->titulus, "praefixum");
}

/* DestinatioStrategia hospitis: Ctrl-A et, dum pendet, claves et
 * textus ad radicem; cetera geometrica */
interior Destinatio
destinare (
           Componens* arbor,
      constans Motus* motus,
              chorda  focus,
    constans Eventus* ev,
             Piscina* piscina)
{
    si (   arbor && ev
        && (est_imperium_a(ev)
            || (   arbor_pendet(arbor)
                && (   ev->genus == EVENTUS_CLAVIS_DEPRESSUS
                    || ev->genus == EVENTUS_CLAVIS_LIBERATUS
                    || ev->genus == EVENTUS_TEXTUS))))
    {
        redde destinatio_ex_componente(arbor, piscina);
    }
    redde destinatio_geometrica(arbor, motus, focus, ev, piscina);
}

interior vacuum
praefixum_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(radix, p, in, "praefixum",
        *(constans b32*)ctx ? "1" : "0");
}

interior b32
praefixum_pendet (
    Vicus* v)
{
    chorda* a;

    a = insula_attributum(v->repo, INSULA_EPHEMERA, "praefixum");
    redde a && chorda_aequalis_literis(*a, "1");
}

interior vacuum
praefixum_ponere (
    Vicus* v,
      b32  pendens)
{
    InsulaRamus radix;

    radix = insula_ramus_radix(v->repo);
    (vacuum)mutare_ramum(&radix, INSULA_EPHEMERA, praefixum_mutator,
        &pendens);
}

/* tabula montata gradu dato ab activa (circulus); VERUM si mutata */
interior b32
vicinam_ponere (
    Vicus* v,
      s32  gradus)
{
     VicusTabula* t;
             s32  n;
             s32  i;
             s32  k;
             s32  j;

    n = (s32)xar_numerus(v->tabulae);
    i = -I;
    per (k = ZEPHYRUM; k < n; k++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, (i32)k);
        si (chorda_aequalis(t->id, v->activa))
        {
            i = k;
        }
    }
    si (i < ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = I; k < n; k++)
    {
        j = ((i + gradus * k) % n + n) % n;
        t = (VicusTabula*)xar_obtinere(v->tabulae, (i32)j);
        si (t->montata)
        {
            redde vicus_activam_ponere(v,
                chorda_ut_cstr(t->id, v->piscina));
        }
    }
    redde FALSUM;
}

/* praefixum perficitur littera textus */
interior vacuum
praefixum_perficere (
    Vicus* v,
      s32  littera)
{
     VicusTabula* t;

    si (littera == 'n')
    {
        (vacuum)vicinam_ponere(v, I);
    }
    alioquin si (littera == 'p')
    {
        (vacuum)vicinam_ponere(v, -I);
    }
    alioquin si (   littera >= '1' && littera <= '9'
                 && littera - '1' < (s32)xar_numerus(v->tabulae))
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae,
            (i32)(littera - '1'));
        si (t->montata)
        {
            (vacuum)vicus_activam_ponere(v,
                chorda_ut_cstr(t->id, v->piscina));
        }
    }
}

/* <tractator/> radicis (T2b, T3b): mutatio magnitudinis ->
 * superficies montationum; Ctrl-A et praefixum pendens. Clavis
 * imprimibilis pendens devoratur - littera in TEXTU agitur (fontes
 * veri clavem et textum mittunt); Esc et cetera abolent. */
interior b32
radicem_tractare (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
     Vicus* v;
    chorda* prior;
       s32  r;
       b32  pendens;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)destinatio;
    (vacuum)nodus;
    v = (Vicus*)ctx;
    si (!ev || !v)
    {
        redde FALSUM;
    }
    si (ev->genus == EVENTUS_MUTARE_MAGNITUDINEM)
    {
        superficies_scribere(v,
            (s32)ev->datum.mutare_magnitudinem.latitudo,
            (s32)ev->datum.mutare_magnitudinem.altitudo);
        redde FALSUM;
    }
    pendens = praefixum_pendet(v);
    si (est_imperium_a(ev))
    {
        si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
        {
            praefixum_ponere(v, !pendens);
            prior = insula_attributum(v->repo, INSULA_EPHEMERA,
                "prior");
            si (pendens && prior)
            {
                (vacuum)vicus_activam_ponere(v,
                    chorda_ut_cstr(*prior, v->piscina));
            }
        }
        redde VERUM;
    }
    si (!pendens)
    {
        redde FALSUM;
    }
    commutatio (ev->genus)
    {
        casus EVENTUS_CLAVIS_LIBERATUS:
            redde VERUM;
        casus EVENTUS_CLAVIS_DEPRESSUS:
            r = ev->datum.clavis.runa;
            si (   r < XXXII || r >= CXXVII
                || (ev->datum.clavis.modificantes
                    & (MOD_IMPERIUM | MOD_ALT | MOD_SUPER)))
            {
                praefixum_ponere(v, FALSUM);
            }
            redde VERUM;
        casus EVENTUS_TEXTUS:
            praefixum_ponere(v, FALSUM);
            si (ev->datum.textus.contentum.mensura > ZEPHYRUM)
            {
                praefixum_perficere(v,
                    (s32)ev->datum.textus.contentum.datum[ZEPHYRUM]);
            }
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* <tractator/> tabulae (T3b): ictus = activa (titulus = id) */
interior b32
tabulam_tractare (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    (vacuum)repo;
    (vacuum)motus;
    (vacuum)destinatio;
    si (!ev || !ctx || !nodus || ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    (vacuum)vicus_activam_ponere((Vicus*)ctx,
        chorda_ut_cstr(nodus->titulus, ((Vicus*)ctx)->piscina));
    redde VERUM;
}

/* latitudo tabulae in pixelis: titulus (+ suffixum finitae) +
 * cellula utrimque - zona ictus et figura eandem legunt */
interior s32
latitudo_tabulae (
    constans VicusTabula* t)
{
    s32 n;

    n = (s32)t->titulus.mensura + II;
    si (t->finita)
    {
        n += (s32)(magnitudo(suffixum_finitae) - I);
    }
    redde n * VICUS_CELLULA_LATITUDO;
}

interior ColorMandati
color_thematis (
    ColorThema c)
{
    ColorMandati cm;

    cm.genus = COLOR_MANDATI_THEMA;
    cm.valor = (i32)c;
    redde cm;
}

/* <purus/> linea tabularum: tituli, activa inversa (ctx = Vicus*) */
interior vacuum
figura_tabularum (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    constans Vicus* v;
       VicusTabula* t;
             Fines  f;
               s32  x;
               s32  lat;
               i32  i;
               b32  activa;

    (vacuum)thema;
    v           = (constans Vicus*)ctx;
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    /* praefixum pendens: linea tincta (titulus lineae notat) */
    mandata_rectangulum(m, f, color_thematis(
        chorda_aequalis_literis(c->titulus, "praefixum")
            ? COLOR_ACCENT_PRIMARY : COLOR_BACKGROUND), VERUM);
    x = ZEPHYRUM;
    per (i = ZEPHYRUM; v && i < xar_numerus(v->tabulae); i++)
    {
        t       = (VicusTabula*)xar_obtinere(v->tabulae, i);
        lat     = latitudo_tabulae(t);
        activa  = chorda_aequalis(t->id, v->activa);
        si (activa)
        {
            f.x         = x;
            f.y         = ZEPHYRUM;
            f.latitudo  = lat;
            f.altitudo  = c->fines.altitudo;
            mandata_rectangulum(m, f, color_thematis(COLOR_SELECTION),
                                VERUM);
        }
        mandata_textus(m, x + VICUS_CELLULA_LATITUDO, ZEPHYRUM,
            t->titulus,
            ZEPHYRUM, color_thematis(activa ? COLOR_BACKGROUND
                                            : COLOR_TEXT));
        si (t->finita)
        {
            mandata_textus(m, x + ((s32)t->titulus.mensura + I)
                * VICUS_CELLULA_LATITUDO, ZEPHYRUM, chorda_ex_literis(
                suffixum_finitae, m->piscina), ZEPHYRUM,
                color_thematis(activa ? COLOR_BACKGROUND : COLOR_TEXT));
        }
        x += lat;
    }
}

/* Motus ligatus (T3a): ramus et gestus activae. Sine activa montata:
 * radix, nullus gestus. */
interior vacuum
motum_aptare (
    Vicus* v)
{
    VicusTabula* t;

    si (!v->motus)
    {
        redde;
    }
    memset(&v->motus->ramus, ZEPHYRUM, magnitudo(InsulaRamus));
    motus_gestum_ponere(v->motus, NIHIL, NIHIL, NIHIL, ZEPHYRUM);
    t = vicus_activa(v);
    si (!t || !t->montata)
    {
        redde;
    }
    v->motus->ramus = insula_ramus(v->repo,
        chorda_ut_cstr(t->genus, v->piscina),
        chorda_ut_cstr(t->id, v->piscina));
    si (t->facies.gestum_ponere)
    {
        t->facies.gestum_ponere(v->motus, t->facies.gestum_ctx);
    }
}

/* relinquens (T3a): gestus et pan/zoom in ramum ADHUC activum
 * effunduntur; gestus sordidus non effusus commutationem recusat
 * (textus perderetur). Pan/zoom recusata abiciuntur - aliter in ramum
 * advenientis effunderentur. Captura et ictus pendens abiciuntur. */
interior b32
motum_relinquere (
    Vicus* v)
{
    si (!v->motus)
    {
        redde VERUM;
    }
    si (   v->motus->gestus.sordidus
        && !motus_gestum_effundere(v->motus, v->repo))
    {
        v->causa = chorda_ex_literis(
            "gestus relinquentis non effusus: commutatio recusata",
            v->piscina);
        redde FALSUM;
    }
    si (v->motus->sordida)
    {
        (vacuum)motus_effundere(v->motus, v->repo);
        v->motus->sordida = FALSUM;
    }
    motus_captura_tollere(v->motus);
    xar_vacare(v->motus->ictus_pendens);
    redde VERUM;
}

/* registra hospitis: vacua, deinde introitus activae (T2a) */
interior vacuum
registra_reficere (
    Vicus* v)
{
    VicusTabula* t;

    actio_registrum_vacare(v->actiones);
    figura_registrum_vacare(v->figurae);
    /* hospitis primum (T2b), deinde activae */
    (vacuum)actio_registrare(v->actiones, "vicus.radix",
                             radicem_tractare, v);
    (vacuum)actio_registrare(v->actiones, "vicus.tabula",
                             tabulam_tractare, v);
    (vacuum)figura_registrare(v->figurae, PARTES_INDEX, ZEPHYRUM,
                              figura_tabularum, v);
    t = tabula_invenire(v, v->activa);
    si (!t || !t->montata)
    {
        redde;
    }
    si (t->facies.actiones)
    {
        (vacuum)actio_registrum_miscere(v->actiones,
            t->facies.actiones);
    }
    si (t->facies.figurae)
    {
        (vacuum)figura_registrum_miscere(v->figurae, t->facies.figurae);
    }
}

/* activa et prior (T3b; prior vacua = non scribitur - valor vacuus
 * numquam scribitur) */
nomen structura {
    chorda activa;
    chorda prior;
} Activa;

interior vacuum
activam_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans Activa* a;

    a = (constans Activa*)ctx;
    insula_attributum_ponere(radix, p, in, "activa",
        chorda_ut_cstr(a->activa, p));
    si (a->prior.mensura > ZEPHYRUM)
    {
        insula_attributum_ponere(radix, p, in, "prior",
            chorda_ut_cstr(a->prior, p));
    }
}


/* ==================================================
 * Vita
 * ================================================== */

Vicus*
vicus_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
    Vicus* v;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    v = (Vicus*)piscina_allocare(piscina, magnitudo(Vicus));
    si (!v)
    {
        redde NIHIL;
    }
    memset(v, ZEPHYRUM, magnitudo(Vicus));
    v->piscina   = piscina;
    v->intern    = intern;
    v->volumen   = volumen;
    v->radix     = radix;
    v->latitudo  = latitudo;
    v->altitudo  = altitudo;
    v->genera    = xar_creare(piscina, (i32)magnitudo(VicusGenus));
    v->actiones  = actio_registrum_creare(piscina, intern);
    v->figurae   = figura_registrum_creare(piscina);
    v->tabulae   = xar_creare(piscina, (i32)magnitudo(VicusTabula));
    redde v;
}

b32
vicus_genus_addere (
                  Vicus* v,
     constans character* titulus,
         memoriae_index  mensura,
          VicusMontator  montare,
        VicusDescriptor  describere)
{
     VicusGenus* g;
         chorda  t;

    si (   !v || !titulus || !montare || !describere
        || mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    t = internare(v, titulus);
    si (chorda_vacua(t) || genus_invenire(v, t))
    {
        redde FALSUM;
    }
    g              = (VicusGenus*)xar_addere(v->genera);
    g->titulus     = t;
    g->mensura     = mensura;
    g->montare     = montare;
    g->describere  = describere;
    redde VERUM;
}

b32
vicus_aperire (
                  Vicus* v,
     constans character* index_ordinarius)
{
          chorda  index;
             b32  inventum;
    StmlResultus  res;
       StmlNodus* n;
     VicusTabula* t;
          chorda* a;
             i32  i;
     InsulaRamus  radix;

    si (!v || !index_ordinarius)
    {
        redde FALSUM;
    }
    v->tabulae  = xar_creare(v->piscina, (i32)magnitudo(VicusTabula));
    v->causa    = chorda_nulla_vici();
    v->repo    = insula_repositorium_creare(v->piscina, v->intern,
        "<vicus/>", "<vicus/>");
    si (!v->repo)
    {
        redde FALSUM;
    }
    insula_ponere_canonem(v->repo, INSULA_DURABILIS,
        canonem_legere(v, "apps/vicus/canones/durabilis.canon"));
    insula_ponere_canonem(v->repo, INSULA_EPHEMERA,
        canonem_legere(v, "apps/vicus/canones/ephemera.canon"));

    /* index: e volumine aut ordinarius (scribitur) */
    index = volumen_plagulam_promere(v->volumen,
        chorda_ex_literis(clavis_indicis, v->piscina), v->piscina,
        &inventum);
    si (!inventum)
    {
        index = chorda_ex_literis(index_ordinarius, v->piscina);
    }
    res = stml_legere_ex_literis(chorda_ut_cstr(index, v->piscina),
        v->piscina, v->intern);
    si (!res.successus || !res.elementum_radix)
    {
        v->causa = chorda_ex_literis("index tabularum malus",
            v->piscina);
        redde FALSUM;
    }
    a          = stml_attributum_capere(res.elementum_radix, "activa");
    v->activa  = a ? *a : chorda_nulla_vici();
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(res.elementum_radix);
         i++)
    {
        n = stml_liberum_ad_indicem(res.elementum_radix, i);
        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        t = (VicusTabula*)xar_addere(v->tabulae);
        memset(t, ZEPHYRUM, magnitudo(VicusTabula));
        a              = stml_attributum_capere(n, "id");
        t->id          = a ? *a : chorda_nulla_vici();
        a              = stml_attributum_capere(n, "genus");
        t->genus       = a ? *a : chorda_nulla_vici();
        a              = stml_attributum_capere(n, "titulus");
        t->titulus     = a ? *a : t->id;
        t->descriptio  = genus_invenire(v, t->genus);
        tabulam_montare(v, t);
    }
    /* activa: data aut prima */
    si (   !tabula_invenire(v, v->activa)
        && xar_numerus(v->tabulae) > ZEPHYRUM)
    {
        v->activa = ((VicusTabula*)xar_obtinere(v->tabulae,
            ZEPHYRUM))->id;
    }
    radix = insula_ramus_radix(v->repo);
    si (!chorda_vacua(v->activa))
    {
        Activa a;

        a.activa         = v->activa;
        a.prior.mensura  = ZEPHYRUM;
        a.prior.datum    = NIHIL;
        (vacuum)mutare_ramum(&radix, INSULA_EPHEMERA, activam_mutator,
                             &a);
    }
    si (!inventum)
    {
        (vacuum)indicem_scribere(v);
    }
    superficies_scribere(v, (s32)v->latitudo, (s32)v->altitudo);
    registra_reficere(v);
    redde VERUM;
}

b32
vicus_tabulam_addere (
                  Vicus* v,
     constans character* genus,
     constans character* id,
     constans character* titulus)
{
    constans VicusGenus* g;
            VicusTabula* t;

    si (   !v || !v->repo || !genus || !id || !titulus
        || strchr(titulus, '"') || strchr(id, '"'))
    {
        redde FALSUM;
    }
    g = genus_invenire(v, internare(v, genus));
    si (!g || tabula_invenire(v, internare(v, id)))
    {
        redde FALSUM;
    }
    t = (VicusTabula*)xar_addere(v->tabulae);
    memset(t, ZEPHYRUM, magnitudo(VicusTabula));
    t->id          = internare(v, id);
    t->genus       = g->titulus;
    t->titulus     = internare(v, titulus);
    t->descriptio  = g;
    tabulam_montare(v, t);
    si (!t->montata)
    {
        xar_truncare(v->tabulae, xar_numerus(v->tabulae) - I);
        redde FALSUM;
    }
    redde indicem_scribere(v);
}


/* ==================================================
 * Lectio et activa
 * ================================================== */

i32
vicus_numerus_tabularum (
    constans Vicus* v)
{
    redde v ? xar_numerus(v->tabulae) : ZEPHYRUM;
}

VicusTabula*
vicus_tabula (
    constans Vicus* v,
               i32  index)
{
    si (!v || index >= xar_numerus(v->tabulae))
    {
        redde NIHIL;
    }
    redde (VicusTabula*)xar_obtinere(v->tabulae, index);
}

VicusTabula*
vicus_activa (
    constans Vicus* v)
{
    redde v ? tabula_invenire(v, v->activa) : NIHIL;
}

b32
vicus_activam_ponere (
                  Vicus* v,
     constans character* id)
{
    VicusTabula* t;
    InsulaRamus  radix;
         Activa  a;

    si (!v || !v->repo || !id)
    {
        redde FALSUM;
    }
    t = tabula_invenire(v, internare(v, id));
    si (!t)
    {
        redde FALSUM;
    }
    a.activa         = t->id;
    a.prior.mensura  = ZEPHYRUM;
    a.prior.datum    = NIHIL;
    si (!chorda_aequalis(t->id, v->activa))
    {
        si (!motum_relinquere(v))
        {
            redde FALSUM;
        }
        a.prior = v->activa;
    }
    v->activa  = t->id;
    radix      = insula_ramus_radix(v->repo);
    si (!mutare_ramum(&radix, INSULA_EPHEMERA, activam_mutator, &a))
    {
        redde FALSUM;
    }
    registra_reficere(v);
    motum_aptare(v);
    redde indicem_scribere(v);
}

ActioRegistrum*
vicus_actiones (
    constans Vicus* v)
{
    redde v ? v->actiones : NIHIL;
}

FiguraRegistrum*
vicus_figurae (
    constans Vicus* v)
{
    redde v ? v->figurae : NIHIL;
}

constans Imago*
vicus_imago_fons (
     chorda  provenientia,
     vacuum* ctx)
{
    VicusTabula* t;

    t = vicus_activa((constans Vicus*)ctx);
    si (!t || !t->montata || !t->facies.fons)
    {
        redde NIHIL;
    }
    redde t->facies.fons(provenientia, t->facies.fons_ctx);
}

/* tabulae ut zonae ictus (PARTES_NULLUM: linea eas pingit) */
interior vacuum
tabulas_componere (
                  Vicus* v,
              Componens* linea,
                Piscina* piscina,
    InternamentumChorda* intern)
{
      VicusTabula* t;
        Componens* c;
            Fines  f;
              i32  i;

    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.altitudo  = VICUS_ALTITUDO_TABULARUM;
    per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, i);
        c = componens_creare(piscina, intern, chorda_ut_cstr(
            chorda_concatenare(chorda_ex_literis("vicus.tabula.",
            piscina), t->id, piscina), piscina), PARTES_NULLUM);
        f.latitudo = latitudo_tabulae(t);
        componens_ponere_fines(c, f);
        componens_ponere_actio(c, "vicus.tabula");
        componens_ponere_titulum(c, chorda_ut_cstr(t->id, piscina));
        componens_addere_liberum(linea, c);
        f.x += f.latitudo;
    }
}

interior s32
attributum_radicis (
    InsulaRepositorium* repo,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a;
       s32  n;

    a = insula_attributum(repo, INSULA_EPHEMERA, titulus);
    si (a && chorda_ut_s32(*a, &n))
    {
        redde n;
    }
    redde praestitutum;
}

/* <componens/> <purus/> */
Componens*
vicus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
            Vicus* v;
        Componens* radix;
        Componens* linea;
        Componens* app;
      VicusTabula* t;
            Fines  f;
              s32  latitudo;
              s32  altitudo;

    v = (Vicus*)ctx;
    si (!v || !repo || !piscina || !intern)
    {
        redde NIHIL;
    }
    latitudo = attributum_radicis(repo, "superficies_latitudo",
        (s32)v->latitudo);
    altitudo = attributum_radicis(repo, "superficies_altitudo",
        (s32)v->altitudo);
    radix = componens_creare(piscina, intern, "vicus", PARTES_NULLUM);
    f.x = ZEPHYRUM;
    f.y = ZEPHYRUM;
    f.latitudo = latitudo;
    f.altitudo = altitudo;
    componens_ponere_fines(radix, f);
    componens_ponere_actio(radix, "vicus.radix");
    linea = componens_creare(piscina, intern, "vicus.tabulae",
                             PARTES_INDEX);
    f.altitudo = VICUS_ALTITUDO_TABULARUM;
    componens_ponere_fines(linea, f);
    componens_addere_liberum(radix, linea);
    /* T3b: praefixum pendens in arbore notatur (destinatio, figura) */
    si (praefixum_pendet(v))
    {
        componens_ponere_titulum(radix, "praefixum");
        componens_ponere_titulum(linea, "praefixum");
    }
    tabulas_componere(v, linea, piscina, intern);
    t = vicus_activa(v);
    si (t && t->montata && t->facies.componere)
    {
        app = t->facies.componere(repo, motus, piscina, intern,
                                  t->facies.componere_ctx);
        si (app)
        {
            /* infra lineam tabularum */
            f    = app->fines;
            f.y  += VICUS_ALTITUDO_TABULARUM;
            componens_ponere_fines(app, f);
            componens_addere_liberum(radix, app);
        }
    }
    redde radix;
}

vacuum
vicus_dispensatorem_ligare (
          Vicus* v,
    Dispensator* d)
{
    si (!v || !d)
    {
        redde;
    }
    v->motus = dispensator_motus(d);
    motum_aptare(v);
    dispensator_ponere_strategiam(d, destinare);
}

b32
vicus_pulsare (
    Vicus* v)
{
    VicusTabula* t;
    VicusPulsus  p;
            i32  i;
            b32  activa;
            b32  pingendum;

    si (!v)
    {
        redde FALSUM;
    }
    pingendum = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
    {
        t       = (VicusTabula*)xar_obtinere(v->tabulae, i);
        activa  = chorda_aequalis(t->id, v->activa);
        si (   !t->montata || t->finita || !t->facies.pulsare
            || (!activa && !t->facies.vivit_in_fundo))
        {
            perge;
        }
        p = t->facies.pulsare(t->facies.pulsare_ctx);
        si (p.mutatum && activa)
        {
            pingendum = VERUM;
        }
        si (p.finitus)
        {
            /* linea tabularum mutatur, quaecumque activa */
            t->finita = VERUM;
            pingendum = VERUM;
        }
    }
    redde pingendum;
}

chorda
vicus_causa (
    constans Vicus* v)
{
    redde v ? v->causa : chorda_nulla_vici();
}
