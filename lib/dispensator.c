/* dispensator.c - ansa eventuum */

#include "dispensator.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
chorda_nulla (vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

/* spatia vacua aequalia sine memcmp (datum NIHIL) */
interior b32
spatia_aequalia (
    chorda a,
    chorda b)
{
    si (a.mensura == ZEPHYRUM && b.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde chorda_aequalis(a, b);
}

/* Eventus derivatus differendus ad limen (T5) */
nomen structura {
             chorda id;
             chorda spatium;    /* S2a: id intra id quaeritur */
    eventus_genus_t genus;
                s64 tempus;
} Differendum;

/* Id vacuum numquam quaeritur (radix anonyma congrueret) */
interior Componens*
invenire_id (
     Dispensator* d,
          chorda  id)
{
    si (!d->arbor || chorda_vacua(id))
    {
        redde NIHIL;
    }
    /* S2a, S8c: intra spatium intimum (vacuum = arbor tota) */
    redde componens_invenire_in_spatio(d->arbor,
        d->motus.spatium_intimum,
        id);
}

interior Componens*
radicem_spatii_quaerere (
    Componens* c,
       chorda  spatium)
{
          i32  i;
          i32  n;
    Componens* inventus;

    si (   !chorda_vacua(c->spatium)
        && chorda_aequalis(c->spatium, spatium))
    {
        redde c;
    }
    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++)
    {
        inventus = radicem_spatii_quaerere(componens_liberum(c, i),
            spatium);
        si (inventus)
        {
            redde inventus;
        }
    }
    redde NIHIL;
}

/* radix spatii activi (Tab circuit intra eam); vacuum = arbor */
interior Componens*
radix_spatii (
     Componens* arbor,
        chorda  spatium)
{
    si (!arbor || chorda_vacua(spatium))
    {
        redde arbor;
    }
    redde radicem_spatii_quaerere(arbor, spatium);
}


/* ==================================================
 * Focus in insula ephemerarum
 * ================================================== */

nomen structura {
    constans character* titulus;
                chorda  valor;
} AttrCtx;

/* S8c: focus segmentum liberi est ("elementum:id")? Ids componentium
 * ':' numquam habent */
interior b32
segmentum_est (
    chorda focus)
{
    i32 i;

    per (i = ZEPHYRUM; i < focus.mensura; i++)
    {
        si (focus.datum[i] == ':')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* S8c: ramus liberi ex segmento ("elementum:id"); repo NIHIL si
 * segmentum malum aut nimis longum */
interior InsulaRamus
liberum_ex_segmento (
    constans InsulaRamus* parens,
                  chorda  segmentum)
{
    character elementum[LXIV];
    character id[CXXVIII];
          i32 i;
          i32 colon;

    colon = ZEPHYRUM;
    dum (colon < segmentum.mensura && segmentum.datum[colon] != ':')
    {
        colon++;
    }
    i = segmentum.mensura - colon - I;
    si (   colon == ZEPHYRUM || colon >= segmentum.mensura
        || colon >= (i32)magnitudo(elementum) || i <= ZEPHYRUM
        || i     >= (i32)magnitudo(id))
    {
        redde insula_ramus_radix(NIHIL);
    }
    memcpy(elementum, segmentum.datum, (size_t)colon);
    elementum[colon] = '\0';
    memcpy(id, segmentum.datum + colon + I, (size_t)i);
    id[i] = '\0';
    redde insula_ramus_liberi(parens, elementum, id);
}

/* S8c: catena foci - a capite (Motus.ramus; nullus = radix) per
 * segmenta 'focus' descendit usque ad ramum cuius focus id est, aut
 * vacuus, aut cuius liberum abest. Spatium capitis = Motus.spatium
 * (ab hospite datum), inferiorum = via sua. */
interior vacuum
catenam_sequi (
    Dispensator* d)
{
    InsulaRamus  r;
    InsulaRamus  liber;
         chorda  spatium;
         chorda* f;
            i32  gradus;

    r        = d->motus.ramus.repo ? d->motus.ramus
                                   : insula_ramus_radix(d->repo);
    spatium  = d->motus.spatium;
    per (gradus = ZEPHYRUM; gradus < XVI; gradus++)
    {
        f = insula_ramus_attributum(&r, "focus");
        si (!f || !segmentum_est(*f))
        {
            frange;
        }
        liber = liberum_ex_segmento(&r, *f);
        si (!liber.repo || !insula_ramus_nodus(&liber))
        {
            frange;
        }
        r        = liber;
        spatium  = liber.via;
    }
    d->motus.ramus_intimus    = r;
    d->motus.spatium_intimum  = spatium;
}

interior vacuum
attr_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    AttrCtx* a;

    a = (AttrCtx*)ctx;
    /* valor vacuus TOLLITUR: "" internari non potest (internamentum
     * recusat) - ponere eum valorem veterem relinquebat, focus numquam
     * tollebatur (inventum S2a-1) */
    si (chorda_vacua(a->valor))
    {
        (vacuum)insula_attributum_tollere(radix, a->titulus);
        redde;
    }
    insula_attributum_ponere(radix, p, in, a->titulus,
                             chorda_ut_cstr(a->valor, p));
}

interior chorda
attr_legere (
           Dispensator* d,
    constans character* titulus)
{
    chorda* c;

    /* T3a, S8c: focus in ramo intimo catenae */
    catenam_sequi(d);
    c = insula_ramus_attributum(&d->motus.ramus_intimus, titulus);
    si (c)
    {
        redde *c;
    }
    redde chorda_nulla();
}

/* attributum in ramo dato, scriptore "dispensator" */
interior vacuum
attr_scribere_in (
             Dispensator* d,
    constans InsulaRamus* ramus,
      constans character* titulus,
                  chorda  valor)
{
    AttrCtx a;

    a.titulus  = titulus;
    a.valor    = valor;
    insula_scriptorem_ponere(d->repo, chorda_ex_literis("dispensator",
        d->piscina));
    (vacuum)mutare_ramum(ramus, attr_ponere, &a);
    insula_scriptorem_ponere(d->repo, chorda_nulla());
}

interior vacuum
attr_scribere (
           Dispensator* d,
    constans character* titulus,
                chorda  valor)
{
    /* S8c: in ramo intimo catenae */
    catenam_sequi(d);
    attr_scribere_in(d, &d->motus.ramus_intimus, titulus, valor);
}

/* S8c: focus in componentem spatii P (P = caput aut sub eo): quisque
 * ramus a capite usque ad parentem segmentum proximum viae P, ramus P
 * id. FALSUM si P extra caput (nihil scribitur). Pan/zoom sordida
 * ante in intimum relictum effunduntur si intimus mutatur. */
interior b32
catenam_scribere (
    Dispensator* d,
         chorda  spatium,
         chorda  id)
{
    InsulaRamus r;
         chorda segmentum;
         chorda via;
            i32 initium;
            i32 finis;

    si (!insula_via_continet(d->motus.spatium, spatium))
    {
        redde FALSUM;
    }
    catenam_sequi(d);
    si (   d->motus.sordida
        && !spatia_aequalia(d->motus.spatium_intimum, spatium))
    {
        (vacuum)motus_effundere(&d->motus, d->repo);
    }
    r        = d->motus.ramus.repo ? d->motus.ramus
                                   : insula_ramus_radix(d->repo);
    initium  = d->motus.spatium.mensura == ZEPHYRUM ? ZEPHYRUM
             : d->motus.spatium.mensura + I;
    dum (initium < spatium.mensura)
    {
        finis = initium;
        dum (finis < spatium.mensura && spatium.datum[finis] != '/')
        {
            finis++;
        }
        segmentum.datum    = spatium.datum + initium;
        segmentum.mensura  = finis - initium;
        via.datum          = spatium.datum;
        via.mensura        = finis;
        attr_scribere_in(d, &r, "focus", segmentum);
        r = insula_ramus_ex_via(d->repo, via);
        si (!r.repo)
        {
            redde FALSUM;
        }
        initium = finis + I;
    }
    attr_scribere_in(d, &r, "focus", id);
    catenam_sequi(d);
    redde VERUM;
}

/* Superficies (modulus 013 B1): magnitudo superficiei STATUS est -
 * attributa ephemera a dispensatore scripta (ut focus), mutatione una
 * (ambo simul, canone semel iudicata). Canon applicationis ea declaret
 * (superficies_latitudo/_altitudo, numerus; dominus "dispensator") -
 * alioquin insula scripturam recusat. Pixela nostra. */
nomen structura {
    s32 latitudo;
    s32 altitudo;
} SuperficiesCtx;

interior vacuum
superficiem_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    SuperficiesCtx* s;

    s = (SuperficiesCtx*)ctx;
    insula_attributum_ponere(radix, p, in, "superficies_latitudo",
        chorda_ut_cstr(chorda_ex_s32(s->latitudo, p), p));
    insula_attributum_ponere(radix, p, in, "superficies_altitudo",
        chorda_ut_cstr(chorda_ex_s32(s->altitudo, p), p));
}

interior vacuum
superficiem_scribere (
          Dispensator* d,
     constans Eventus* e)
{
    SuperficiesCtx s;

    s.latitudo = (s32)e->datum.mutare_magnitudinem.latitudo;
    s.altitudo = (s32)e->datum.mutare_magnitudinem.altitudo;
    insula_scriptorem_ponere(d->repo,
                             chorda_ex_literis("dispensator",
                             d->piscina));
    insula_mutare(d->repo, superficiem_ponere, &s);
    insula_scriptorem_ponere(d->repo, chorda_nulla());
}

chorda
dispensator_focus (
    Dispensator* d)
{
    si (!d)
    {
        redde chorda_nulla();
    }
    redde attr_legere(d, "focus");
}

vacuum
dispensator_focus_ponere (
     Dispensator* d,
          chorda  id)
{
    si (!d)
    {
        redde;
    }
    attr_scribere(d, "focus", id);
}

vacuum
dispensator_focus_impellere (
     Dispensator* d,
          chorda  id)
{
    chorda acervus;
    chorda currens;
    chorda comma;

    si (!d)
    {
        redde;
    }
    currens  = dispensator_focus(d);
    acervus  = attr_legere(d, "focus_acervus");
    comma    = chorda_ex_literis(",", d->piscina);
    si (!chorda_vacua(acervus))
    {
        acervus = chorda_concatenare(acervus, comma, d->piscina);
    }
    acervus = chorda_concatenare(acervus, currens, d->piscina);
    attr_scribere(d, "focus_acervus", acervus);
    dispensator_focus_ponere(d, id);
}

vacuum
dispensator_focus_extrahere (
    Dispensator* d)
{
    chorda_fissio_fructus partes;
                   chorda acervus;
                   chorda ultimus;
                   chorda reliquum;
                   chorda comma;
                      i32 i;

    si (!d)
    {
        redde;
    }
    acervus = attr_legere(d, "focus_acervus");
    si (chorda_vacua(acervus))
    {
        redde;
    }
    partes = chorda_fissio(acervus, ',', d->piscina);
    si (partes.numerus == ZEPHYRUM)
    {
        redde;
    }
    ultimus   = partes.elementa[partes.numerus - I];
    reliquum  = chorda_nulla();
    comma     = chorda_ex_literis(",", d->piscina);
    per (i = ZEPHYRUM; i + I < partes.numerus; i++)
    {
        si (i > ZEPHYRUM)
        {
            reliquum = chorda_concatenare(reliquum, comma, d->piscina);
        }
        reliquum = chorda_concatenare(reliquum, partes.elementa[i],
                                      d->piscina);
    }
    attr_scribere(d, "focus_acervus", reliquum);
    dispensator_focus_ponere(d, ultimus);
}


/* ==================================================
 * Compositio (ping-pong)
 * ================================================== */

vacuum
dispensator_recomponere (
    Dispensator* d)
{
         i32  alia;
     Piscina* p;

    si (!d || !d->componere)
    {
        redde;
    }
    alia  = I - d->arbor_activa;
    p     = d->piscinae_arboris[alia];
    piscina_vacare(p);
    d->arbor = d->componere(d->repo, &d->motus, p, d->intern,
                            d->componere_ctx);
    d->arbor_activa = alia;
    d->compositiones++;
}


/* ==================================================
 * Vita
 * ================================================== */

Dispensator*
dispensator_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo,
         ActioRegistrum* actiones,
              Componere  componere,
                 vacuum* componere_ctx,
                    s64  quies_ms)
{
    Dispensator* d;

    si (!piscina || !intern || !repo || !actiones || !componere)
    {
        redde NIHIL;
    }
    d = (Dispensator*)piscina_allocare(piscina, magnitudo(Dispensator));
    si (!d)
    {
        redde NIHIL;
    }
    memset(d, ZEPHYRUM, magnitudo(Dispensator));
    d->piscina  = piscina;
    d->intern   = intern;
    d->piscinae_arboris[ZEPHYRUM] =
        piscina_generare_dynamicum("arbor_a", LXIV * M);
    d->piscinae_arboris[I] =
        piscina_generare_dynamicum("arbor_b", LXIV * M);
    d->scratch = piscina_generare_dynamicum("destinatio_scratch",
                                            XVI * M);
    si (   !d->piscinae_arboris[ZEPHYRUM] || !d->piscinae_arboris[I]
        || !d->scratch)
    {
        redde NIHIL;
    }
    d->repo = repo;
    d->actiones = actiones;
    d->strategia = destinatio_geometrica;
    d->componere = componere;
    d->componere_ctx = componere_ctx;
    d->super = chorda_nulla();
    d->quies_ms = quies_ms;
    d->effusio = xar_creare(piscina, (i32)magnitudo(Eventus));
    d->differenda = xar_creare(piscina, (i32)magnitudo(Differendum));
    d->notata = NIHIL;
    motus_initiare(&d->motus, piscina);
    derivator_initiare(&d->derivator, CCC, IV);
    d->arbor_activa = I;          /* recomponere permutat ad 0 */
    dispensator_recomponere(d);   /* quadrum 0 ante initum */
    redde d;
}

vacuum
dispensator_ponere_strategiam (
            Dispensator* d,
    DestinatioStrategia  strategia)
{
    si (!d || !strategia)
    {
        redde;
    }
    d->strategia = strategia;
}


/* ==================================================
 * Dispensatio
 * ================================================== */

/* Ascensum ambulare: prima actio quae consumit sistit. S5c: *actor =
 * componens cuius actio consumpsit (NIHIL si nulla) */
interior b32
mittere (
            Dispensator*  d,
    constans Destinatio*  des,
       constans Eventus*  e,
              Componens** actor)
{
     Componens* c;
       ActioFn  fn;
        vacuum* ctx;
           i32  i;
           i32  n;
           b32  consumptus;

    *actor = NIHIL;
    si (!des->ascensus)
    {
        redde FALSUM;
    }
    n = xar_numerus(des->ascensus);
    per (i = ZEPHYRUM; i < n; i++)
    {
        c = *(Componens**)xar_obtinere(des->ascensus, i);
        si (chorda_vacua(c->actio))
        {
            perge;
        }
        /* S2a: actio in spatio COMPONENTIS */
        si (!actio_invenire_in_spatio(d->actiones, componens_spatium(c),
                c->actio, &fn, &ctx))
        {
            perge;
        }
        insula_scriptorem_ponere(d->repo, c->actio);
        consumptus = fn(d->repo, &d->motus, des, c, e, ctx);
        insula_scriptorem_ponere(d->repo, chorda_nulla());
        si (consumptus)
        {
            *actor = c;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* differendum in spatio dato (S2a) */
interior vacuum
differre (
     Dispensator* d,
          chorda  spatium,
          chorda  id,
 eventus_genus_t  genus,
             s64  tempus)
{
    Differendum* x;

    si (chorda_vacua(id))
    {
        redde;
    }
    x           = (Differendum*)xar_addere(d->differenda);
    x->id       = id;
    x->spatium  = spatium;
    x->genus    = genus;
    x->tempus   = tempus;
}

/* Eventus derivatus ad nodum certum */
/* Eventus derivatus: numquam statim - differtur ad limen */
interior vacuum
mittere_ad (
        Dispensator* d,
          Componens* c,
    eventus_genus_t  genus,
                s64  tempus)
{
    si (!c)
    {
        redde;
    }
    /* S2a: spatium COMPONENTIS - radix hospitis et radix montationis
     * idem id habere possunt */
    differre(d, componens_spatium(c), c->id, genus, tempus);
}

/* A6d: focusabiles sub c ordine profunditatis (Componens*) */
interior vacuum
focusabiles_colligere (
    Componens* c,
          Xar* l)
{
    i32 i;

    si (!c || !l)
    {
        redde;
    }
    si (c->focusabilis)
    {
        *(Componens**)xar_addere(l) = c;
    }
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        focusabiles_colligere(componens_liberum(c, i), l);
    }
}

/* focus ad componentem c (spatii sui): catena si spatium nidificatum
 * sub capite, aliter focus in ramo intimo */
interior vacuum
focum_ad_componentem (
    Dispensator* d,
         chorda  focus,
      Componens* c,
            s64  tempus)
{
    mittere_ad(d, invenire_id(d, focus), EVENTUS_FOCUS_AMISSUS, tempus);
    si (!catenam_scribere(d, componens_spatium(c), c->id))
    {
        dispensator_focus_ponere(d, c->id);
    }
    mittere_ad(d, c, EVENTUS_FOCUS_CAPTUS, tempus);
}

/* A6d: Tab - focusabiles sub radice spatii lateris (nidificati
 * inclusi); currens = id focus in spatio intimo; proximus (gradus I)
 * aut praecedens (-I), involvit */
interior vacuum
tabulam_circumire (
    Dispensator* d,
         chorda  focus,
            s32  gradus,
            s64  tempus)
{
    Componens* radix;
          Xar* l;
          s32  n;
          s32  k;
          s32  i;
    Componens* c;

    radix  = radix_spatii(d->arbor, d->motus.spatium);
    l      = xar_creare(d->scratch, (i32)magnitudo(Componens*));
    focusabiles_colligere(radix, l);
    n = l ? (s32)xar_numerus(l) : ZEPHYRUM;
    si (n == ZEPHYRUM)
    {
        redde;
    }
    k = -I;
    per (i = ZEPHYRUM; i < n; i++)
    {
        c = *(Componens**)xar_obtinere(l, (i32)i);
        si (   chorda_aequalis(c->id, focus)
            && spatia_aequalia(componens_spatium(c),
                   d->motus.spatium_intimum))
        {
            k = i;
            frange;
        }
    }
    k = k < ZEPHYRUM ? (gradus > ZEPHYRUM ? ZEPHYRUM : n - I)
                     : (k + gradus + n) % n;
    focum_ad_componentem(d, focus, *(Componens**)xar_obtinere(l,
        (i32)k), tempus);
}

/* A6d: Esc ex spatio nidificato (intimum non caput): focus ad
 * focusabilem primum gradus continentis proximi - spatium eius
 * intimum continet sed non aequat; longissimum (proximum) eligitur.
 * Radix lateris sine signo (hospes ipse) spatium "" habet - continet
 * omnia */
interior vacuum
gradum_ascendere (
    Dispensator* d,
         chorda  focus,
            s64  tempus)
{
          Xar* l;
          i32  i;
          s32  longitudo;
    Componens* c;
    Componens* electus;
       chorda  sp;

    si (   chorda_vacua(d->motus.spatium_intimum)
        || spatia_aequalia(d->motus.spatium_intimum, d->motus.spatium))
    {
        redde;
    }
    l = xar_creare(d->scratch, (i32)magnitudo(Componens*));
    focusabiles_colligere(radix_spatii(d->arbor, d->motus.spatium), l);
    electus    = NIHIL;
    longitudo  = -I;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        c   = *(Componens**)xar_obtinere(l, i);
        sp  = componens_spatium(c);
        si (   !spatia_aequalia(sp, d->motus.spatium_intimum)
            && insula_via_continet(sp, d->motus.spatium_intimum)
            && (s32)sp.mensura > longitudo)
        {
            electus    = c;
            longitudo  = (s32)sp.mensura;
        }
    }
    si (electus)
    {
        focum_ad_componentem(d, focus, electus, tempus);
    }
}

vacuum
dispensator_addressare (
     Dispensator* d,
          chorda  id,
 eventus_genus_t  genus,
             s64  tempus)
{
    si (!d)
    {
        redde;
    }
    /* id nudum: spatium intimum (S2a, S8c) */
    catenam_sequi(d);
    differre(d, d->motus.spatium_intimum, id, genus, tempus);
}

/* Limen: differenda TUNC praesentia traduntur contra arborem novam;
 * quae in traditione nascuntur, ad limen proximum manent. Copia in
 * scratch: addressare in traditione tutum. */
interior vacuum
limen_transire (
    Dispensator* d)
{
    Differendum* x;
     Destinatio  des;
        Eventus  e;
      Componens* c;
      Componens* actor;
            i32  i;
            i32  n;
            Xar* praesentia;

    n = xar_numerus(d->differenda);
    si (n == ZEPHYRUM)
    {
        redde;
    }
    praesentia = xar_creare(d->scratch, (i32)magnitudo(Differendum));
    per (i = ZEPHYRUM; i < n; i++)
    {
        x   = (Differendum*)xar_addere(praesentia);
        *x  = *(Differendum*)xar_obtinere(d->differenda, i);
    }
    xar_vacare(d->differenda);
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = (Differendum*)xar_obtinere(praesentia, i);
        c = componens_invenire_in_spatio(d->arbor, x->spatium, x->id);
        si (!c)
        {
            perge;
        }
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus   = x->genus;
        e.tempus  = x->tempus;
        des       = destinatio_ex_componente(c, d->scratch);
        /* derivata hamos medios non transeunt (dimissio = input
         * crudum) */
        (vacuum)mittere(d, &des, &e, &actor);
    }
    dispensator_recomponere(d);
}

/* Notarius (A6): eventum crudum cum destinato suo notare. Destinatum
 * ex arbore VETERE (ante traditionem): id in piscinam dispensatoris
 * copiatur - piscinae arborum vicissim redduntur. */
interior vacuum
_notare (
          Dispensator* d,
     constans Eventus* e,
  constans Destinatio* des)
{
     EventusNotatum* en;
          Componens* destinatum;
            Punctum  p;

    en = (EventusNotatum*)xar_addere(d->notata);
    memset(en, ZEPHYRUM, magnitudo(EventusNotatum));
    en->eventus = *e;
    si (   e->genus != EVENTUS_MUS_DEPRESSUS
        && e->genus != EVENTUS_MUS_LIBERATUS
        && e->genus != EVENTUS_MUS_MOTUS)
    {
        redde;
    }
    destinatum = destinatio_componens(des);
    si (!destinatum)
    {
        redde;
    }
    p.x           = e->datum.mus.x;
    p.y           = e->datum.mus.y;
    p             = destinatio_ad_locale(destinatum, p);
    en->scopus    = chorda_transcribere(destinatum->id, d->piscina);
    en->scopus_x  = p.x;
    en->scopus_y  = p.y;
}

interior vacuum
tractare_unum (
         Dispensator* d,
    constans Eventus* e,
                 b32  crudus)
{
    PiscinaNotatio  nota;
        Destinatio  des;
            chorda  focus;
            chorda  spatium_super;
         Componens* destinatum;
         Componens* actor;
               b32  consumptus;

    nota = piscina_notare(d->scratch);
    /* superficies ante destinationem et compositionem: arbor nova
     * magnitudinem novam videt */
    si (crudus && e->genus == EVENTUS_MUTARE_MAGNITUDINEM)
    {
        superficiem_scribere(d, e);
    }
    focus = dispensator_focus(d);
    des = d->strategia(d->arbor, &d->motus, focus, e, d->scratch);
    destinatum = destinatio_componens(&des);
    si (crudus && d->notata)
    {
        _notare(d, e, &des);
    }

    /* super derivatur (S2a: cum spatio suo - idem id in latere altero
     * est componens alius) */
    spatium_super = (   destinatum
                     && chorda_aequalis(destinatum->id,
                            des.id_geometricum))
                  ? componens_spatium(destinatum)
                  : d->motus.spatium_intimum;
    si (   e->genus == EVENTUS_MUS_MOTUS
        && (   !chorda_aequalis(des.id_geometricum, d->super)
            || !spatia_aequalia(spatium_super, d->super_spatium)))
    {
        mittere_ad(d, componens_invenire_in_spatio(d->arbor,
            d->super_spatium, d->super), EVENTUS_MUS_EXIIT, e->tempus);
        d->super          = des.id_geometricum;
        d->super_spatium  = spatium_super;
        mittere_ad(d, componens_invenire_in_spatio(d->arbor,
            d->super_spatium, d->super), EVENTUS_MUS_INTRAVIT,
            e->tempus);
    }

    /* ictus dat focum (S8c: idem id in gradu alio = focus alius;
     * spatium nidificatum catenam scribit) */
    si (   e->genus == EVENTUS_MUS_DEPRESSUS && destinatum
        && destinatum->focusabilis
        && (   !chorda_aequalis(destinatum->id, focus)
            || !spatia_aequalia(componens_spatium(destinatum),
                   d->motus.spatium_intimum)))
    {
        mittere_ad(d, invenire_id(d, focus), EVENTUS_FOCUS_AMISSUS,
                   e->tempus);
        si (!catenam_scribere(d, componens_spatium(destinatum),
                destinatum->id))
        {
            dispensator_focus_ponere(d, destinatum->id);
        }
        mittere_ad(d, destinatum, EVENTUS_FOCUS_CAPTUS, e->tempus);
        focus = destinatum->id;
    }

    /* S5c: hami medii (dimissio) - ante actionem, post consumentem */
    actor = NIHIL;
    si (d->ante && d->ante(d->medium_ctx, d, &des, e))
    {
        consumptus = VERUM;
    }
    alioquin
    {
        consumptus = mittere(d, &des, e, &actor);
        si (consumptus && actor && d->post)
        {
            d->post(d->medium_ctx, d, actor, e);
        }
    }

    /* Tab innatus per clavem LOGICAM (eventus A5): typus '\t' etiam
     * Ctrl+I fert. Shift = MOD_SHIFT (olim '& I' - qui bitus lateris
     * Ctrl sinistri est: Ctrl+Tab retro, Shift+Tab ante ibat).
     * S2a + A6d: circuitus intra spatium LATERIS (motus.spatium),
     * focusabiles spatiorum nidificatorum inclusi - focus in spatium
     * aliud catenam scribit (Tab inter gradus, S8c-2 dilatum) */
    si (   !consumptus && e->genus == EVENTUS_CLAVIS_DEPRESSUS
        && e->datum.clavis.clavis == CLAVIS_TABULA)
    {
        tabulam_circumire(d, focus, (e->datum.clavis.modificantes
            & MOD_SHIFT) ? -I : I, e->tempus);
    }
    /* A6d: Esc a nemine consumptum, focus in spatio nidificato: gradus
     * unus sursum - focusabilis primus spatii parentis */
    si (   !consumptus && e->genus == EVENTUS_CLAVIS_DEPRESSUS
        && e->datum.clavis.clavis == CLAVIS_EFFUGIUM)
    {
        gradum_ascendere(d, focus, e->tempus);
    }

    /* regula staleness */
    dispensator_recomponere(d);

    /* limen: derivata contra arborem novam */
    limen_transire(d);

    /* focus ut petitio: id ex arbore nova absens -> radici (ad limen
     * proximum); si post id arbor id non reddit, focus tollitur */
    focus = dispensator_focus(d);
    si (!chorda_vacua(focus) && !invenire_id(d, focus))
    {
        mittere_ad(d, d->arbor, EVENTUS_FOCUS_PETITUS, e->tempus);
        limen_transire(d);
        si (!invenire_id(d, focus))
        {
            dispensator_focus_ponere(d, chorda_nulla());
        }
    }
    piscina_reficere(d->scratch, nota);
}

vacuum
dispensator_tractare (
         Dispensator* d,
    constans Eventus* ev)
{
    i32 i;
    i32 n;

    si (!d || !ev)
    {
        redde;
    }
    /* sedes quietis: pan/zoom (quies dispensatoris) et gestus
     * applicationis (quies sua, S1a) - tempore eventus, ergo
     * replicatio eadem */
    si (motus_quies(&d->motus, ev->tempus, d->quies_ms))
    {
        motus_effundere(&d->motus, d->repo);
    }
    si (motus_gestus_quies(&d->motus, ev->tempus))
    {
        (vacuum)motus_gestum_effundere(&d->motus, d->repo);
    }
    xar_vacare(d->effusio);
    derivare(&d->derivator, ev, d->effusio);
    n = xar_numerus(d->effusio);
    per (i = ZEPHYRUM; i < n; i++)
    {
        /* effusio[0] = eventus crudus (derivare eum primum ponit) */
        tractare_unum(d,
            (constans Eventus*)xar_obtinere(d->effusio, i),
            (b32)(i == ZEPHYRUM));
    }
}

vacuum
dispensator_notarium_ponere (
    Dispensator* d,
            Xar* notata)
{
    si (d)
    {
        d->notata = notata;
    }
}

vacuum
dispensator_medium_ponere (
        Dispensator* d,
    DispensatorAnte  ante,
    DispensatorPost  post,
             vacuum* ctx)
{
    si (!d)
    {
        redde;
    }
    d->ante        = ante;
    d->post        = post;
    d->medium_ctx  = ctx;
}

vacuum
dispensator_finire (
    Dispensator* d)
{
    si (!d)
    {
        redde;
    }
    si (d->motus.sordida)
    {
        (vacuum)motus_effundere(&d->motus, d->repo);
    }
    (vacuum)motus_gestum_effundere(&d->motus, d->repo);
}

vacuum
dispensator_pulsare (
    Dispensator* d,
            s64  tempus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus   = EVENTUS_NIHIL;
    e.tempus  = tempus;
    dispensator_tractare(d, &e);
}


/* ==================================================
 * Lectio
 * ================================================== */

Componens*
dispensator_arbor (
    constans Dispensator* d)
{
    redde d ? d->arbor : NIHIL;
}

chorda
dispensator_super (
    constans Dispensator* d)
{
    redde d ? d->super : chorda_nulla();
}

Motus*
dispensator_motus (
    Dispensator* d)
{
    redde d ? &d->motus : NIHIL;
}

i32
dispensator_numerus_compositionum (
    constans Dispensator* d)
{
    redde d ? d->compositiones : ZEPHYRUM;
}
