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

/* ramus status pictoris (R3): contextus eum fert; sine eo radix */
interior InsulaRamus
ramus_pictoris (
    InsulaRepositorium* repo,
                vacuum* ctx)
{
    PictorActiones* pa;

    pa = (PictorActiones*)ctx;
    si (pa && pa->ramus.repo)
    {
        redde pa->ramus;
    }
    redde insula_ramus_radix(repo);
}

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_ramus_attributum(ramus, INSULA_EPHEMERA, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

interior chorda
ictum_scribere (
    constans InsulaRamus* ramus,
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
        chorda_ex_s32(attributum_s32(ramus, "color_primus", ZEPHYRUM),
        p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\" magnitudo=\"", p),
        p);
    s = chorda_concatenare(s,
        chorda_ex_s32(attributum_s32(ramus, "magnitudo", I), p), p);
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
       InsulaRamus  ramus;
    PictorActiones* pa;
           Punctum  p;

    pa = (PictorActiones*)ctx;
    si (!repo || !motus || !destinatio || !nodus || !ev || !pa)
    {
        redde FALSUM;
    }
    /* punctum in spatio NODI (tabulae), non ictus geometrici: dum
     * captura tenet, mus super aliud jacet (linea status, latus
     * alterum) et punctum_locale ad id pertinet - tractus ad marginem
     * oppositum saliebat (Franus 2026-10-08; destinatio.h) */
    p.x  = (s32)ev->datum.mus.x;
    p.y  = (s32)ev->datum.mus.y;
    p    = destinatio_ad_locale(nodus, p);
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
            ramus = ramus_pictoris(repo, ctx);
            pictor_documentum_actum(pa->doc,
                ictum_scribere(&ramus, motus, pa->doc->piscina));
            mutare_motum(motus, puncta_vacare, NIHIL, ev->tempus);
            /* ictus finitus ephemera non tangit */
            motus->sordida = FALSUM;
            motus_captura_tollere(motus);
            redde VERUM;
        casus EVENTUS_CLAVIS_DEPRESSUS:
            si (   chorda_vacua(motus->captura)
                || ev->datum.clavis.clavis != CLAVIS_EFFUGIUM)
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
    InsulaRamus ramus;
    (vacuum)motus;
    (vacuum)destinatio;
    (vacuum)nodus;
    si (!repo || !ev || ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    /* clavis LOGICA (runa sine maiuscula, dispositionis praesentis);
     * sub Cmd/Ctrl brevitas est (Cmd+P imprimere), non 'p' (A5) */
    si (   ev->datum.clavis.runa == 'p'
        && (ev->datum.clavis.modificantes & (MOD_SUPER | MOD_IMPERIUM))
               == ZEPHYRUM)
    {
        ramus = ramus_pictoris(repo, ctx);
        redde mutare_ramum(&ramus, INSULA_EPHEMERA, instrumentum_ponere,
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
