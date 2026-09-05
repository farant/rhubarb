/* ludus_toy.h - applicatio ludicra pro probationibus substrati
 *
 * radix (0,0,200,100) actio "fugere"
 *   b1     (10,10,50,20)  bottone focusabilis actio "numerare"
 *   tabula (70,10,100,80) tabula PROPRIUS focusabilis actio "ictus"
 *
 * <componens/> <purus/>: toy_componere legit repo + motus, nihil
 * scribit. Functiones INTERIORES: quaeque probatio quae hoc caput
 * includit copiam suam accipit, et (-Wunused-function) AMBOS aditus
 * vocare debet - toy_componere et toy_registrare - qui ceteras
 * omnes trahunt.
 */
#ifndef LUDUS_TOY_H
#define LUDUS_TOY_H

#include "latina.h"
#include "chorda.h"
#include "xar.h"
#include "stml.h"
#include "componens.h"
#include "insula.h"
#include "motus.h"
#include "actio.h"
#include "destinatio.h"

nomen structura { i32 compositiones; } ToyStatus;

nomen structura {
    constans character* titulus;
                   i32  valor;
} ToyPositio;

/* tituli attributorum ut character[] (non literae: -Wwrite-strings) */
hic_manens character toy_attr_numerus[]       = "numerus";
hic_manens character toy_attr_ictus_numerus[] = "ictus_numerus";
hic_manens character toy_attr_fuga[]          = "fuga";


/* ==================================================
 * Compositio
 * ================================================== */

interior Componens*
toy_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
     Componens* radix;
     Componens* b1;
     Componens* tabula;
         Fines  f;

    (vacuum)repo;
    (vacuum)motus;
    ((ToyStatus*)ctx)->compositiones++;
    radix       = componens_creare(p, in, "radix", PARTES_NULLUM);
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = CC;
    f.altitudo  = C;
    componens_ponere_fines(radix, f);
    componens_ponere_actio(radix, "fugere");
    b1          = componens_creare(p, in, "b1", PARTES_BOTTONE);
    f.x         = X;
    f.y         = X;
    f.latitudo  = L;
    f.altitudo  = XX;
    componens_ponere_fines(b1, f);
    componens_ponere_actio(b1, "numerare");
    componens_ponere_titulum(b1, "Numerare");
    componens_ponere_focusabilis(b1, VERUM);
    componens_addere_liberum(radix, b1);
    tabula      = componens_creare(p, in, "tabula", PARTES_TABULA);
    f.x         = LXX;
    f.y         = X;
    f.latitudo  = C;
    f.altitudo  = LXXX;
    componens_ponere_fines(tabula, f);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_ponere_actio(tabula, "ictus");
    componens_ponere_focusabilis(tabula, VERUM);
    componens_addere_liberum(radix, tabula);
    redde radix;
}


/* ==================================================
 * Mutatores
 * ================================================== */

/* ctx = titulus attributi (character[]); valor + I */
interior vacuum
toy_incrementare (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    chorda* a;
       i32  v;

    a = stml_attributum_capere(radix, (constans character*)ctx);
    v = ZEPHYRUM;
    si (a)
    {
        chorda_ut_i32(*a, &v);
    }
    insula_attributum_ponere(radix, p, in, (constans character*)ctx,
        chorda_ut_cstr(chorda_ex_s32((s32)(v + I), p), p));
}

interior vacuum
toy_ponere_numerum (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    ToyPositio* tp;

    tp = (ToyPositio*)ctx;
    insula_attributum_ponere(radix, p, in, tp->titulus,
        chorda_ut_cstr(chorda_ex_s32((s32)tp->valor, p), p));
}

interior vacuum
toy_ponere_verum (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(radix, p, in, (constans character*)ctx,
                             "verum");
}

interior vacuum
toy_punctum_addere (
     Motus* motus,
    vacuum* ctx)
{
    Punctum* sedes;

    sedes   = (Punctum*)xar_addere(motus->ictus_pendens);
    *sedes  = *(Punctum*)ctx;
}

interior vacuum
toy_puncta_vacare (
     Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    xar_vacare(motus->ictus_pendens);
}


/* ==================================================
 * Actiones - <tractator/>
 * ================================================== */

interior b32
toy_numerare (
    InsulaRepositorium* r,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* c,
      constans Eventus* e,
                vacuum* ctx)
{
    (vacuum)motus;
    (vacuum)destinatio;
    (vacuum)c;
    (vacuum)ctx;
    si (e->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    mutare_ephemera(r, toy_incrementare, toy_attr_numerus);
    redde VERUM;
}

/* Ictus: depressus capit et incipit, motus addit, liberatus UNA
 * scriptura durabili finit - insulam ephemerarum NUMQUAM tangit. */
interior b32
toy_ictus (
    InsulaRepositorium* r,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* c,
      constans Eventus* e,
                vacuum* ctx)
{
       Punctum p;
    ToyPositio tp;

    (vacuum)ctx;
    p = destinatio->punctum_locale;
    commutatio (e->genus)
    {
        casus EVENTUS_MUS_DEPRESSUS:
            motus_captura_ponere(motus, c->id);
            mutare_motum(motus, toy_puncta_vacare, NIHIL, e->tempus);
            mutare_motum(motus, toy_punctum_addere, &p, e->tempus);
            redde VERUM;
        casus EVENTUS_MUS_MOTUS:
            si (chorda_vacua(motus->captura))
            {
                redde FALSUM;
            }
            mutare_motum(motus, toy_punctum_addere, &p, e->tempus);
            redde VERUM;
        casus EVENTUS_MUS_LIBERATUS:
            si (chorda_vacua(motus->captura))
            {
                redde FALSUM;
            }
            tp.titulus = "puncta";
            tp.valor   = xar_numerus(motus->ictus_pendens);
            mutare_durabile(r, toy_ponere_numerum, &tp);
            mutare_durabile(r, toy_incrementare,
                toy_attr_ictus_numerus);
            mutare_motum(motus, toy_puncta_vacare, NIHIL, e->tempus);
            /* ictus finitus insulam ephemerarum NON tangit */
            motus->sordida = FALSUM;
            motus_captura_tollere(motus);
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

interior b32
toy_fugere (
    InsulaRepositorium* r,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* c,
      constans Eventus* e,
                vacuum* ctx)
{
    (vacuum)motus;
    (vacuum)destinatio;
    (vacuum)c;
    (vacuum)ctx;
    si (   e->genus              == EVENTUS_CLAVIS_DEPRESSUS
        && e->datum.clavis.typus == (character)XXVII)
    {
        mutare_ephemera(r, toy_ponere_verum, toy_attr_fuga);
        redde VERUM;
    }
    redde FALSUM;
}

interior vacuum
toy_registrare (
    ActioRegistrum* reg,
         ToyStatus* toy)
{
    actio_registrare(reg, "numerare", toy_numerare, toy);
    actio_registrare(reg, "ictus", toy_ictus, toy);
    actio_registrare(reg, "fugere", toy_fugere, toy);
}

#endif /* LUDUS_TOY_H */
