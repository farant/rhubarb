/* pictor_actiones.c - tractatores pictoris */

#include "pictor_actiones.h"
#include "xar.h"


/* ==================================================
 * Mutatores motus
 * ================================================== */

interior vacuum
puncta_vacare (
     Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    xar_vacare(motus->ictus_pendens);
}

interior vacuum
punctum_addere (
     Motus* motus,
    vacuum* ctx)
{
    Punctum* sedes;

    sedes   = (Punctum*)xar_addere(motus->ictus_pendens);
    *sedes  = *(Punctum*)ctx;
}


/* ==================================================
 * Mutator insulae
 * ================================================== */

hic_manens character litterae_penicillus[] = "penicillus";

interior vacuum
instrumentum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(radix, p, in, "instrumentum",
                             (constans character*)ctx);
}


/* ==================================================
 * Actum ictus
 * ================================================== */

interior s32
attributum_s32 (
    InsulaRepositorium* repo,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_attributum(repo, INSULA_EPHEMERA, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

interior chorda
ictum_scribere (
    InsulaRepositorium* repo,
        constans Motus* motus,
               Piscina* p)
{
     chorda  s;
    Punctum* q;
        i32  i;
        i32  n;

    s = chorda_ex_literis("<ictus instrumentum=\"penicillus\" color=\"",
        p);
    s = chorda_concatenare(s,
        chorda_ex_s32(attributum_s32(repo, "color_primus", ZEPHYRUM),
        p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\" magnitudo=\"", p),
        p);
    s = chorda_concatenare(s,
        chorda_ex_s32(attributum_s32(repo, "magnitudo", I), p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\">", p), p);
    n = xar_numerus(motus->ictus_pendens);
    per (i = ZEPHYRUM; i < n; i++)
    {
        q = (Punctum*)xar_obtinere(motus->ictus_pendens, i);
        s = chorda_concatenare(s, chorda_ex_literis("<punctum x=\"", p),
            p);
        s = chorda_concatenare(s, chorda_ex_s32(q->x, p), p);
        s = chorda_concatenare(s, chorda_ex_literis("\" y=\"", p), p);
        s = chorda_concatenare(s, chorda_ex_s32(q->y, p), p);
        s = chorda_concatenare(s, chorda_ex_literis("\"/>", p), p);
    }
    s = chorda_concatenare(s, chorda_ex_literis("</ictus>", p), p);
    redde s;
}


/* ==================================================
 * Tractatores
 * ================================================== */

/* <tractator/> */
b32
pictor_penicillus_ictus (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    PictorActiones* pa;
           Punctum  p;

    pa = (PictorActiones*)ctx;
    si (!repo || !motus || !destinatio || !nodus || !ev || !pa)
    {
        redde FALSUM;
    }
    p = destinatio->punctum_locale;
    commutatio (ev->genus)
    {
        casus EVENTUS_MUS_DEPRESSUS:
            motus_captura_ponere(motus, nodus->id);
            mutare_motum(motus, puncta_vacare, NIHIL, ev->tempus);
            mutare_motum(motus, punctum_addere, &p, ev->tempus);
            redde VERUM;
        casus EVENTUS_MUS_MOTUS:
            si (chorda_vacua(motus->captura))
            {
                redde FALSUM;
            }
            mutare_motum(motus, punctum_addere, &p, ev->tempus);
            redde VERUM;
        casus EVENTUS_MUS_LIBERATUS:
            si (chorda_vacua(motus->captura))
            {
                redde FALSUM;
            }
            pictor_documentum_actum(pa->doc,
                ictum_scribere(repo, motus, pa->doc->piscina));
            mutare_motum(motus, puncta_vacare, NIHIL, ev->tempus);
            /* ictus finitus ephemera non tangit */
            motus->sordida = FALSUM;
            motus_captura_tollere(motus);
            redde VERUM;
        casus EVENTUS_CLAVIS_DEPRESSUS:
            si (   chorda_vacua(motus->captura)
                || ev->datum.clavis.typus != (character)XXVII)
            {
                redde FALSUM;
            }
            mutare_motum(motus, puncta_vacare, NIHIL, ev->tempus);
            motus->sordida = FALSUM;
            motus_captura_tollere(motus);
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* <tractator/> */
b32
pictor_instrumentum_eligere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    (vacuum)motus;
    (vacuum)destinatio;
    (vacuum)nodus;
    (vacuum)ctx;
    si (!repo || !ev || ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    si (ev->datum.clavis.typus == 'p')
    {
        redde mutare_ephemera(repo, instrumentum_ponere,
            litterae_penicillus);
    }
    redde FALSUM;
}

vacuum
pictor_actiones_registrare (
    ActioRegistrum* reg,
    PictorActiones* ctx)
{
    si (!reg || !ctx)
    {
        redde;
    }
    actio_registrare(reg, "penicillus.ictus", pictor_penicillus_ictus,
        ctx);
    actio_registrare(reg, "instrumentum.eligere",
                     pictor_instrumentum_eligere, ctx);
}
