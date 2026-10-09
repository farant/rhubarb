/* pictor_actiones.c - tractatores pictoris */

#include "pictor_actiones.h"
#include "xar.h"

#include <string.h>


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

hic_manens character litterae_penicillus[]  = "penicillus";
hic_manens character litterae_aspergillum[] = "aspergillum";
hic_manens character litterae_spongia[]     = "spongia";

/* aspergillum: semen ictus in ephemera (praevisio guttarum) */
interior vacuum
semen_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(radix, p, in, "semen",
        chorda_ut_cstr(chorda_ex_s64(*(constans s64*)ctx, p), p));
}

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

/* P1b: palette aperta (ctx = nomen) aut clausa (ctx NIHIL: tollitur) */
interior vacuum
palettam_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    si (!ctx)
    {
        (vacuum)insula_attributum_tollere(radix, "palette");
        redde;
    }
    insula_attributum_ponere(radix, p, in, "palette",
                             (constans character*)ctx);
}

/* P1b: colorem (ctx = ColorPonendus) ponere */
nomen structura {
     constans character* attributum;
                    s32  valor;
} ColorPonendus;

interior vacuum
colorem_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans ColorPonendus* cp;

    cp = (constans ColorPonendus*)ctx;
    insula_attributum_ponere(radix, p, in, cp->attributum,
        chorda_ut_cstr(chorda_ex_s32(cp->valor, p), p));
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

/* P1b: palette aperta? */
interior b32
palette_aperta (
    constans InsulaRamus* ramus)
{
    redde insula_ramus_attributum(ramus, INSULA_EPHEMERA, "palette")
        != NIHIL;
}

interior b32
palettam_claudere (
    constans InsulaRamus* ramus)
{
    redde mutare_ramum(ramus, INSULA_EPHEMERA, palettam_mutator, NIHIL);
}

/* id nodi post praefixum ('optio.color_primus.' -> '7'); vacua si
 * praefixum abest */
interior chorda
post_praefixum (
    constans Componens* nodus,
    constans character* praefixum)
{
    chorda c;
       i32 n;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    n          = (i32)strlen(praefixum);
    si (   nodus && nodus->id.mensura > n
        && memcmp(nodus->id.datum, praefixum, (size_t)n) == ZEPHYRUM)
    {
        c.datum    = nodus->id.datum + n;
        c.mensura  = nodus->id.mensura - n;
    }
    redde c;
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

/* actum ictus pendentis instrumenti; pa (aspergillum solum): semen, t
 * cuiusque puncti ab initio ictus; spongia sine colore */
interior chorda
ictum_scribere (
       constans InsulaRamus* ramus,
             constans Motus* motus,
         constans character* instrumentum,
    constans PictorActiones* pa,
                    Piscina* p)
{
     chorda  s;
    Punctum* q;
        i32  i;
        i32  n;
        s64  t0;

    s = chorda_ex_literis("<ictus instrumentum=\"", p);
    s = chorda_concatenare(s, chorda_ex_literis(instrumentum, p), p);
    si (instrumentum != litterae_spongia)
    {
        s = chorda_concatenare(s, chorda_ex_literis("\" color=\"", p),
            p);
        s = chorda_concatenare(s,
            chorda_ex_s32(attributum_s32(ramus, "color_primus",
            ZEPHYRUM),
            p), p);
    }
    s = chorda_concatenare(s, chorda_ex_literis("\" magnitudo=\"", p),
        p);
    s = chorda_concatenare(s,
        chorda_ex_s32(attributum_s32(ramus, "magnitudo", I), p), p);
    si (pa)
    {
        s = chorda_concatenare(s, chorda_ex_literis("\" semen=\"", p),
            p);
        s = chorda_concatenare(s, chorda_ex_s64(pa->semen, p), p);
    }
    s = chorda_concatenare(s, chorda_ex_literis("\">", p), p);
    n = xar_numerus(motus->ictus_pendens);
    t0 = (pa && pa->tempora && xar_numerus(pa->tempora) > ZEPHYRUM)
        ? *(s64*)xar_obtinere(pa->tempora, ZEPHYRUM) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        q = (Punctum*)xar_obtinere(motus->ictus_pendens, i);
        s = chorda_concatenare(s, chorda_ex_literis("<punctum x=\"", p),
            p);
        s = chorda_concatenare(s, chorda_ex_s32(q->x, p), p);
        s = chorda_concatenare(s, chorda_ex_literis("\" y=\"", p), p);
        s = chorda_concatenare(s, chorda_ex_s32(q->y, p), p);
        si (pa && pa->tempora && i < xar_numerus(pa->tempora))
        {
            s = chorda_concatenare(s, chorda_ex_literis("\" t=\"", p),
                p);
            s = chorda_concatenare(s, chorda_ex_s64(
                *(s64*)xar_obtinere(pa->tempora, i) - t0, p), p);
        }
        s = chorda_concatenare(s, chorda_ex_literis("\"/>", p), p);
    }
    s = chorda_concatenare(s, chorda_ex_literis("</ictus>", p), p);
    redde s;
}


/* ==================================================
 * Tractatores
 * ================================================== */

/* tempus puncti (aspergillum) */
interior vacuum
tempus_addere (
     PictorActiones* pa,
                s64  tempus)
{
    si (!pa->tempora)
    {
        pa->tempora = xar_creare(pa->doc->piscina,
            (i32)magnitudo(s64));
    }
    si (pa->tempora)
    {
        *(s64*)xar_addere(pa->tempora) = tempus;
    }
}

/* ictus communis penicilli, aspergilli, spongiae (actio instrumentum
 * figit - tractator de statu instrumenti non ramificat) */
interior b32
ictum_tractare (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx,
    constans character* instrumentum)
{
       InsulaRamus  ramus;
    PictorActiones* pa;
           Punctum  p;
               b32  aspergillum;

    pa           = (PictorActiones*)ctx;
    aspergillum  = instrumentum == litterae_aspergillum;
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
            /* P1b: palette aperta - ictus eam solum claudit */
            ramus = ramus_pictoris(repo, ctx);
            si (palette_aperta(&ramus))
            {
                (vacuum)palettam_claudere(&ramus);
                redde VERUM;
            }
            motus_captura_ponere(motus, nodus->id);
            mutare_motum(motus, puncta_vacare, NIHIL, ev->tempus);
            mutare_motum(motus, punctum_addere, &p, ev->tempus);
            si (aspergillum)
            {
                /* semen ex tempore et cursore historiae: ictus diversi
                 * (etiam eodem loco) guttas diversas */
                pa->semen = ((s64)ev->tempus
                             + (s64)pictor_documentum_cursor(pa->doc)
                               * ((s64)M * (s64)M + (s64)III))
                          & (s64)0x7FFFFFFF;
                si (pa->tempora)
                {
                    xar_vacare(pa->tempora);
                }
                tempus_addere(pa, ev->tempus);
                ramus = ramus_pictoris(repo, ctx);
                (vacuum)mutare_ramum(&ramus, INSULA_EPHEMERA,
                    semen_ponere, &pa->semen);
            }
            redde VERUM;
        casus EVENTUS_MUS_MOTUS:
            si (chorda_vacua(motus->captura))
            {
                redde FALSUM;
            }
            mutare_motum(motus, punctum_addere, &p, ev->tempus);
            si (aspergillum)
            {
                tempus_addere(pa, ev->tempus);
            }
            redde VERUM;
        casus EVENTUS_MUS_LIBERATUS:
            si (chorda_vacua(motus->captura))
            {
                redde FALSUM;
            }
            /* aspergillum: punctum solutionis - mora ante eam guttas
             * addit */
            si (aspergillum)
            {
                mutare_motum(motus, punctum_addere, &p, ev->tempus);
                tempus_addere(pa, ev->tempus);
            }
            ramus = ramus_pictoris(repo, ctx);
            pictor_documentum_actum(pa->doc,
                ictum_scribere(&ramus, motus, instrumentum,
                aspergillum ? pa : NIHIL, pa->doc->piscina));
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
pictor_penicillus_ictus (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    redde ictum_tractare(repo, motus, destinatio, nodus, ev, ctx,
        litterae_penicillus);
}

/* <tractator/> */
b32
pictor_aspergillum_ictus (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    redde ictum_tractare(repo, motus, destinatio, nodus, ev, ctx,
        litterae_aspergillum);
}

/* <tractator/> */
b32
pictor_spongia_ictus (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    redde ictum_tractare(repo, motus, destinatio, nodus, ev, ctx,
        litterae_spongia);
}

/* nomen instrumenti -> litterae (penicillus si ignotum) */
interior character*
litterae_instrumenti (
    chorda nomen_instrumenti)
{
    si (chorda_aequalis_literis(nomen_instrumenti, "aspergillum"))
    {
        redde litterae_aspergillum;
    }
    si (chorda_aequalis_literis(nomen_instrumenti, "spongia"))
    {
        redde litterae_spongia;
    }
    redde litterae_penicillus;
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
         chorda nomen_instrumenti;
    (vacuum)motus;
    (vacuum)destinatio;
    si (!repo || !ev)
    {
        redde FALSUM;
    }
    /* P1b: optio instrumenti in palette */
    nomen_instrumenti = post_praefixum(nodus, "optio.instrumentum.");
    si (   ev->genus == EVENTUS_MUS_DEPRESSUS
        && nomen_instrumenti.mensura > ZEPHYRUM)
    {
        ramus = ramus_pictoris(repo, ctx);
        (vacuum)palettam_claudere(&ramus);
        redde mutare_ramum(&ramus, INSULA_EPHEMERA, instrumentum_ponere,
            litterae_instrumenti(nomen_instrumenti));
    }
    si (ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    /* P1b: Esc palettam apertam claudit */
    si (ev->datum.clavis.clavis == CLAVIS_EFFUGIUM)
    {
        ramus = ramus_pictoris(repo, ctx);
        si (palette_aperta(&ramus))
        {
            redde palettam_claudere(&ramus);
        }
        redde FALSUM;
    }
    /* clavis LOGICA (runa sine maiuscula, dispositionis praesentis);
     * sub Cmd/Ctrl brevitas est (Cmd+P imprimere), non 'p' (A5) */
    si (   (ev->datum.clavis.runa == 'p'
        || ev->datum.clavis.runa == 'a'
        || ev->datum.clavis.runa == 'e')
        && (ev->datum.clavis.modificantes & (MOD_SUPER | MOD_IMPERIUM))
               == ZEPHYRUM)
    {
        ramus = ramus_pictoris(repo, ctx);
        redde mutare_ramum(&ramus, INSULA_EPHEMERA, instrumentum_ponere,
            ev->datum.clavis.runa == 'p' ? litterae_penicillus
            : ev->datum.clavis.runa == 'a' ? litterae_aspergillum
                                           : litterae_spongia);
    }
    redde FALSUM;
}

/* <tractator/> */
b32
pictor_palettam_aperire (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    InsulaRamus  ramus;
         chorda  genus;
         chorda* aperta;

    (vacuum)motus;
    (vacuum)destinatio;
    si (!repo || !ev || ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    genus = post_praefixum(nodus, "quadratum.");
    si (genus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    ramus = ramus_pictoris(repo, ctx);
    aperta = insula_ramus_attributum(&ramus, INSULA_EPHEMERA,
        "palette");
    si (aperta && chorda_aequalis(*aperta, genus))
    {
        redde palettam_claudere(&ramus);
    }
    redde mutare_ramum(&ramus, INSULA_EPHEMERA, palettam_mutator,
        (vacuum*)chorda_ut_cstr(genus, ((PictorActiones*)ctx)->doc
            ->piscina));
}

/* <tractator/> */
b32
pictor_colorem_ponere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
      InsulaRamus ramus;
    ColorPonendus cp;
           chorda valor;

    (vacuum)motus;
    (vacuum)destinatio;
    si (!repo || !ev || ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    valor          = post_praefixum(nodus, "optio.color_primus.");
    cp.attributum  = "color_primus";
    si (valor.mensura == ZEPHYRUM)
    {
        valor          = post_praefixum(nodus, "optio.color_secundus.");
        cp.attributum  = "color_secundus";
    }
    si (   valor.mensura == ZEPHYRUM || !chorda_ut_s32(valor, &cp.valor)
        || cp.valor < -I || cp.valor > XV)
    {
        redde FALSUM;
    }
    ramus = ramus_pictoris(repo, ctx);
    (vacuum)palettam_claudere(&ramus);
    redde mutare_ramum(&ramus, INSULA_EPHEMERA, colorem_mutator, &cp);
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
    actio_registrare(reg, "aspergillum.ictus", pictor_aspergillum_ictus,
        ctx);
    actio_registrare(reg, "spongia.ictus", pictor_spongia_ictus, ctx);
    actio_registrare(reg, "palette.aperire", pictor_palettam_aperire,
        ctx);
    actio_registrare(reg, "color_primus.ponere", pictor_colorem_ponere,
        ctx);
    actio_registrare(reg, "color_secundus.ponere",
        pictor_colorem_ponere,
        ctx);
    actio_registrare(reg, "instrumentum.eligere",
                     pictor_instrumentum_eligere, ctx);
}
