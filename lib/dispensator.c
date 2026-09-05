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


/* Eventus derivatus differendus ad limen (T5) */
nomen structura {
             chorda id;
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
    redde componens_invenire_per_id(d->arbor, id);
}


/* ==================================================
 * Focus in insula ephemerarum
 * ================================================== */

nomen structura {
    constans character* titulus;
                chorda  valor;
} AttrCtx;

interior vacuum
attr_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    AttrCtx* a;

    a = (AttrCtx*)ctx;
    insula_attributum_ponere(radix, p, in, a->titulus,
                             chorda_ut_cstr(a->valor, p));
}

interior chorda
attr_legere (
           Dispensator* d,
    constans character* titulus)
{
    chorda* c;

    c = insula_attributum(d->repo, INSULA_EPHEMERA, titulus);
    si (c)
    {
        redde *c;
    }
    redde chorda_nulla();
}

interior vacuum
attr_scribere (
           Dispensator* d,
    constans character* titulus,
                chorda  valor)
{
    AttrCtx a;

    a.titulus  = titulus;
    a.valor    = valor;
    mutare_ephemera(d->repo, attr_ponere, &a);
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

/* Ascensum ambulare: prima actio quae consumit sistit. */
interior b32
mittere (
            Dispensator* d,
    constans Destinatio* des,
       constans Eventus* e)
{
     Componens* c;
       ActioFn  fn;
        vacuum* ctx;
           i32  i;
           i32  n;

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
        si (!actio_invenire(d->actiones, c->actio, &fn, &ctx))
        {
            perge;
        }
        si (fn(d->repo, &d->motus, des, c, e, ctx))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
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
    dispensator_addressare(d, c->id, genus, tempus);
}

vacuum
dispensator_addressare (
     Dispensator* d,
          chorda  id,
 eventus_genus_t  genus,
             s64  tempus)
{
    Differendum* x;

    si (!d || chorda_vacua(id))
    {
        redde;
    }
    x          = (Differendum*)xar_addere(d->differenda);
    x->id      = id;
    x->genus   = genus;
    x->tempus  = tempus;
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
        c = invenire_id(d, x->id);
        si (!c)
        {
            perge;
        }
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus   = x->genus;
        e.tempus  = x->tempus;
        des       = destinatio_ex_componente(c, d->scratch);
        mittere(d, &des, &e);
    }
    dispensator_recomponere(d);
}

interior vacuum
tractare_unum (
         Dispensator* d,
    constans Eventus* e)
{
    PiscinaNotatio  nota;
        Destinatio  des;
            chorda  focus;
            chorda  focus_novus;
         Componens* destinatum;
               b32  consumptus;

    nota = piscina_notare(d->scratch);
    focus = dispensator_focus(d);
    des = d->strategia(d->arbor, &d->motus, focus, e, d->scratch);
    destinatum = destinatio_componens(&des);

    /* super derivatur */
    si (   e->genus == EVENTUS_MUS_MOTUS
        && !chorda_aequalis(des.id_geometricum, d->super))
    {
        mittere_ad(d, invenire_id(d, d->super), EVENTUS_MUS_EXIIT,
                   e->tempus);
        d->super = des.id_geometricum;
        mittere_ad(d, invenire_id(d, d->super), EVENTUS_MUS_INTRAVIT,
                   e->tempus);
    }

    /* ictus dat focum */
    si (   e->genus == EVENTUS_MUS_DEPRESSUS && destinatum
        && destinatum->focusabilis
        && !chorda_aequalis(destinatum->id, focus))
    {
        mittere_ad(d, invenire_id(d, focus), EVENTUS_FOCUS_AMISSUS,
                   e->tempus);
        dispensator_focus_ponere(d, destinatum->id);
        mittere_ad(d, destinatum, EVENTUS_FOCUS_CAPTUS, e->tempus);
        focus = destinatum->id;
    }

    consumptus = mittere(d, &des, e);

    /* Tab innatus: modificans I (shift) = praecedens */
    si (   !consumptus && e->genus == EVENTUS_CLAVIS_DEPRESSUS
        && e->datum.clavis.typus == '\t')
    {
        focus_novus = (e->datum.clavis.modificantes & I)
            ? destinatio_focus_praecedens(d->arbor, focus, d->scratch)
            : destinatio_focus_proximum(d->arbor, focus, d->scratch);
        si (!chorda_vacua(focus_novus))
        {
            mittere_ad(d, invenire_id(d, focus), EVENTUS_FOCUS_AMISSUS,
                       e->tempus);
            dispensator_focus_ponere(d, focus_novus);
            mittere_ad(d, invenire_id(d, focus_novus),
                       EVENTUS_FOCUS_CAPTUS, e->tempus);
        }
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
    /* sedes quietis */
    si (motus_quies(&d->motus, ev->tempus, d->quies_ms))
    {
        motus_effundere(&d->motus, d->repo);
    }
    xar_vacare(d->effusio);
    derivare(&d->derivator, ev, d->effusio);
    n = xar_numerus(d->effusio);
    per (i = ZEPHYRUM; i < n; i++)
    {
        tractare_unum(d,
            (constans Eventus*)xar_obtinere(d->effusio, i));
    }
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
