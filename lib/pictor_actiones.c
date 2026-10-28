/* pictor_actiones.c - tractatores pictoris */

#include "pictor_actiones.h"
#include "pictor_componentia.h"
#include "elenchus.h"
#include "moneta.h"
#include "communio_cliens.h"
#include "xar.h"
#include "exemplaria.h"

#include <stdio.h>
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

/* linea: punctum secundum (finis praevisionis) ponere aut addere */
interior vacuum
punctum_finale_ponere (
     Motus* motus,
    vacuum* ctx)
{
    si (xar_numerus(motus->ictus_pendens) >= II)
    {
        *(Punctum*)xar_obtinere(motus->ictus_pendens, I) =
            *(Punctum*)ctx;
        redde;
    }
    *(Punctum*)xar_addere(motus->ictus_pendens) = *(Punctum*)ctx;
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
hic_manens character litterae_linea[]       = "linea";
hic_manens character litterae_glutinatio_a[] = "A";
hic_manens character litterae_glutinatio_b[] = "B";

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

interior b32
palettam_claudere (
    constans InsulaRamus* ramus)
{
    redde mutare_ramum(ramus, palettam_mutator, NIHIL);
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

    a = insula_ramus_attributum(ramus, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

/* L3: stratum currens - ephemera stratum_activum si in documento est,
 * aliter summum */
interior s32
stratum_currens (
         constans InsulaRamus* ramus,
    constans PictorDocumentum* doc)
{
    chorda* a;
       s32  id;
       i32  k;
       i32  n;

    n = pictor_documentum_numerus_stratorum(doc);
    a = insula_ramus_attributum(ramus,
        "stratum_activum");
    si (a && chorda_ut_s32(*a, &id))
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (pictor_documentum_stratum(doc, k)->id == id)
            {
                redde id;
            }
        }
    }
    redde n > ZEPHYRUM ? pictor_documentum_stratum(doc, n - I)->id : I;
}

/* actum ictus pendentis instrumenti; pa (aspergillum solum): semen, t
 * cuiusque puncti ab initio ictus; spongia sine colore */
interior chorda
ictum_scribere (
       constans InsulaRamus* ramus,
             constans Motus* motus,
         constans character* instrumentum,
    constans PictorActiones* pa,
  constans PictorDocumentum* doc,
                    Piscina* p)
{
         s32  stratum;
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
        /* P3: non ordinaria sola - acta vetera idem significant */
        si (attributum_s32(ramus, "color_secundus", -I) != -I)
        {
            s = chorda_concatenare(s, chorda_ex_literis(
                "\" color_secundus=\"", p), p);
            s = chorda_concatenare(s,
                chorda_ex_s32(attributum_s32(ramus,
                "color_secundus", -I), p), p);
        }
        si (attributum_s32(ramus, "exemplar", ZEPHYRUM) != ZEPHYRUM)
        {
            s = chorda_concatenare(s,
                chorda_ex_literis("\" exemplar=\"",
                p), p);
            s = chorda_concatenare(s,
                chorda_ex_s32(attributum_s32(ramus,
                "exemplar", ZEPHYRUM), p), p);
        }
    }
    /* L3: stratum currens (I absens - acta vetera idem significant) */
    stratum = stratum_currens(ramus, doc);
    si (stratum != I)
    {
        s = chorda_concatenare(s, chorda_ex_literis("\" stratum=\"", p),
            p);
        s = chorda_concatenare(s, chorda_ex_s32(stratum, p), p);
    }
    s = chorda_concatenare(s, chorda_ex_literis("\" magnitudo=\"", p),
        p);
    s = chorda_concatenare(s,
        chorda_ex_s32(instrumentum == litterae_penicillus
            ? attributum_s32(ramus, "magnitudo_penicilli", I)
            : instrumentum == litterae_aspergillum
            ? attributum_s32(ramus, "magnitudo_aspergilli", I)
            : instrumentum == litterae_linea
            ? attributum_s32(ramus, "magnitudo_lineae", I) : I, p),
        p);
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
            /* P1b: palette aperta - ictus eam solum claudit: systema
             * dimissio (S5c) ictum ante actionem deglutit */
            ramus = ramus_pictoris(repo, ctx);
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
                (vacuum)mutare_ramum(&ramus,
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
                aspergillum ? pa : NIHIL, pa->doc, pa->doc->piscina));
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

/* linea pendens abicitur: puncta, captura */
interior vacuum
lineam_abicere (
     Motus* motus,
       s64  tempus)
{
    mutare_motum(motus, puncta_vacare, NIHIL, tempus);
    motus->sordida = FALSUM;
    motus_captura_tollere(motus);
}

/* <tractator/> */
b32
pictor_linea_ictus (
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
               b32  pendens;

    pa = (PictorActiones*)ctx;
    si (!repo || !motus || !destinatio || !nodus || !ev || !pa)
    {
        redde FALSUM;
    }
    pendens  = !chorda_vacua(motus->captura);
    p.x      = (s32)ev->datum.mus.x;
    p.y      = (s32)ev->datum.mus.y;
    p        = destinatio_ad_locale(nodus, p);
    commutatio (ev->genus)
    {
        casus EVENTUS_MUS_DEPRESSUS:
            ramus = ramus_pictoris(repo, ctx);
            /* palette aperta: dimissio (S5c) ictum deglutit */
            si (!pendens)
            {
                /* ictus primus: initium */
                motus_captura_ponere(motus, nodus->id);
                mutare_motum(motus, puncta_vacare, NIHIL, ev->tempus);
                mutare_motum(motus, punctum_addere, &p, ev->tempus);
                redde VERUM;
            }
            /* captura: ictus extra tabulam (geometrice) abicit */
            si (!chorda_aequalis(destinatio->id_geometricum, nodus->id))
            {
                lineam_abicere(motus, ev->tempus);
                redde VERUM;
            }
            mutare_motum(motus, punctum_finale_ponere, &p, ev->tempus);
            pictor_documentum_actum(pa->doc, ictum_scribere(&ramus,
                motus, litterae_linea, NIHIL, pa->doc,
                pa->doc->piscina));
            mutare_motum(motus, puncta_vacare, NIHIL, ev->tempus);
            si ((ev->datum.mus.modificantes & MOD_SHIFT) != ZEPHYRUM)
            {
                /* series: finis initium novum */
                mutare_motum(motus, punctum_addere, &p, ev->tempus);
                motus->sordida = FALSUM;
                redde VERUM;
            }
            lineam_abicere(motus, ev->tempus);
            redde VERUM;
        casus EVENTUS_MUS_MOTUS:
            si (!pendens)
            {
                redde FALSUM;
            }
            mutare_motum(motus, punctum_finale_ponere, &p, ev->tempus);
            redde VERUM;
        casus EVENTUS_MUS_LIBERATUS:
            /* solutio ictus primi: captura manet */
            redde pendens;
        casus EVENTUS_CLAVIS_DEPRESSUS:
            si (!pendens || ev->datum.clavis.clavis != CLAVIS_EFFUGIUM)
            {
                redde FALSUM;
            }
            lineam_abicere(motus, ev->tempus);
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* glutinatio pendens: campus 'glutinatio' ("A"/"B"; ctx NIHIL =
 * tollitur) */
interior vacuum
glutinationem_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    si (!ctx)
    {
        (vacuum)insula_attributum_tollere(radix, "glutinatio");
        redde;
    }
    insula_attributum_ponere(radix, p, in, "glutinatio",
                             (constans character*)ctx);
}

/* campus 'glutinatio' sub nomine domini - vocatur etiam ex clavibus
 * (actio instrumentum.eligere), ergo scriptor ponitur, deinde prior */
interior b32
glutinationem_scribere (
    InsulaRepositorium* repo,
                vacuum* ctx,
             character* valor)
{
    PictorActiones* pa;
       InsulaRamus  ramus;
            chorda  prior;
               b32  scriptum;

    pa     = (PictorActiones*)ctx;
    ramus  = ramus_pictoris(repo, ctx);
    prior  = repo->scriptor;
    insula_scriptorem_ponere(repo, chorda_ex_literis("glutinatio.agere",
        pa->doc->piscina));
    scriptum = mutare_ramum(&ramus, glutinationem_mutator, valor);
    insula_scriptorem_ponere(repo, prior);
    redde scriptum;
}

/* L5 + glutinatio pendens (Franus 2026-10-13): imago aptatur et bis
 * ditheratur in documento, EXTRA acta; campus 'glutinatio' = "B"
 * (exemplaria). Iterum: prior abicitur. Stratum nascitur solum per
 * glutinationem_confirmare. Commune Cmd+V et depositioni (C1b T1):
 * parare quod servat in piscinam DOCUMENTI exscribit - fons ipse post
 * reditum liberari potest */
interior b32
_imaginem_glutinare_ex (
    InsulaRepositorium* repo,
                vacuum* ctx,
        constans Imago* imago)
{
    PictorActiones* pa;

    pa = (PictorActiones*)ctx;
    si (   !pa || !pa->doc
        || !pictor_documentum_glutinationem_parare(pa->doc, imago))
    {
        redde FALSUM;
    }
    redde glutinationem_scribere(repo, ctx, litterae_glutinatio_b);
}

/* Cmd+V - imago e porta (clipboard) */
interior b32
imaginem_glutinare (
    InsulaRepositorium* repo,
                vacuum* ctx)
{
    PictorActiones* pa;
      ImagoFructus  f;

    pa = (PictorActiones*)ctx;
    si (!pa || !pa->doc || !pa->imago_capere)
    {
        redde FALSUM;
    }
    f = pa->imago_capere(pa->imago_ctx, pa->doc->piscina);
    si (!f.successus)
    {
        redde FALSUM;
    }
    redde _imaginem_glutinare_ex(repo, ctx, &f.imago);
}

/* depositio viarum (testa-ios C1b T1: photographia electa in
 * telephono = plagula deposita, D2): prima via quae ut imago legitur
 * glutinatur, ut Cmd+V. Viae chorda sunt ('\n' inter eas, sine
 * NUL): quaeque in buffer NUL-terminatum exscribitur. Imago in
 * piscina TEMPORARIA decoditur (photographia MXXIV ~ IV MiB) et post
 * parare destruitur. Nulla legibilis -> FALSUM, nihil mutatur. */
interior b32
imaginem_depositam_glutinare (
    InsulaRepositorium* repo,
                vacuum* ctx,
      constans Eventus* ev)
{
    PictorActiones* pa;
           Piscina* temporaria;
            chorda  viae;
      ImagoFructus  f;
         character  via[MXXIV];
               i32  initium;
               i32  finis;
               b32  lecta;
               b32  factum;

    pa = (PictorActiones*)ctx;
    si (!pa || !pa->doc || !ev)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("pictor_depositio",
        (memoriae_index)(M * M));
    si (!temporaria)
    {
        redde FALSUM;
    }
    viae     = ev->datum.depositio.viae;
    lecta    = FALSUM;
    factum   = FALSUM;
    initium  = ZEPHYRUM;
    dum (!lecta && viae.datum && initium < viae.mensura)
    {
        finis = initium;
        dum (finis < viae.mensura && viae.datum[finis] != '\n')
        {
            finis++;
        }
        si (   finis > initium
            && (memoriae_index)(finis - initium) < magnitudo(via))
        {
            memcpy(via, viae.datum + initium, (size_t)(finis
                - initium));
            via[finis - initium] = '\0';
            f = imago_caricare_ex_file(via, temporaria);
            si (f.successus)
            {
                lecta   = VERUM;
                factum  = _imaginem_glutinare_ex(repo, ctx, &f.imago);
            }
        }
        initium = finis + I;
    }
    piscina_destruere(temporaria);
    redde factum;
}

/* OK: modus campi in stratum novum supra currens, quod currens fit
 * (stratum_activum dominum 'strata.agere' habet: scriptura sub nomine
 * eius, deinde prior); campus tollitur */
interior b32
glutinationem_confirmare (
    InsulaRepositorium* repo,
                vacuum* ctx)
{
       PictorActiones* pa;
          InsulaRamus  ramus;
        ColorPonendus  cp;
               chorda  prior;
               chorda* modus;
                  s32  id;

    pa = (PictorActiones*)ctx;
    si (!pa || !pa->doc)
    {
        redde FALSUM;
    }
    ramus = ramus_pictoris(repo, ctx);
    modus = insula_ramus_attributum(&ramus, "glutinatio");
    id     = pictor_documentum_glutinationem_confirmare(pa->doc,
        modus && chorda_aequalis_literis(*modus, "A")
            ? PICTOR_GLUTINATIO_ATKINSON : PICTOR_GLUTINATIO_EXEMPLARIA,
        stratum_currens(&ramus, pa->doc));
    si (id == ZEPHYRUM)
    {
        redde FALSUM;
    }
    cp.attributum  = "stratum_activum";
    cp.valor       = id;
    prior          = repo->scriptor;
    insula_scriptorem_ponere(repo, chorda_ex_literis("strata.agere",
        pa->doc->piscina));
    (vacuum)mutare_ramum(&ramus, colorem_mutator, &cp);
    insula_scriptorem_ponere(repo, prior);
    redde glutinationem_scribere(repo, ctx, NIHIL);
}

/* X / Esc: abicitur, acta intacta */
interior b32
glutinationem_abicere (
    InsulaRepositorium* repo,
                vacuum* ctx)
{
    PictorActiones* pa;

    pa = (PictorActiones*)ctx;
    si (!pa || !pa->doc)
    {
        redde FALSUM;
    }
    pictor_documentum_glutinationem_abicere(pa->doc);
    redde glutinationem_scribere(repo, ctx, NIHIL);
}

/* tractus in tabula pendente: radere a puncto ultimo ad praesens
 * (pressio: punctum solum); Optio = restituere. Magnitudo =
 * penicilli */
interior b32
glutinationem_radere (
    InsulaRepositorium* repo,
                 Motus* motus,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    PictorActiones* pa;
       InsulaRamus  ramus;
           Punctum  p;
               s32  n;
               b32  restituere;

    pa = (PictorActiones*)ctx;
    si (!pa || !pa->doc || !motus || !pa->doc->glutinatio.pendens)
    {
        redde FALSUM;
    }
    /* punctum in spatio tabulae (ut ictum_tractare) */
    p.x         = (s32)ev->datum.mus.x;
    p.y         = (s32)ev->datum.mus.y;
    p           = destinatio_ad_locale(nodus, p);
    ramus       = ramus_pictoris(repo, ctx);
    n           = attributum_s32(&ramus, "magnitudo_penicilli", I);
    restituere  = (ev->datum.mus.modificantes & MOD_ALT) != ZEPHYRUM;
    commutatio (ev->genus)
    {
        casus EVENTUS_MUS_DEPRESSUS:
            motus_captura_ponere(motus, nodus->id);
            pa->rasura = p;
            (vacuum)pictor_documentum_glutinationem_radere(pa->doc, p.x,
                p.y, p.x, p.y, n, restituere);
            redde VERUM;
        casus EVENTUS_MUS_MOTUS:
        casus EVENTUS_MUS_LIBERATUS:
            si (chorda_vacua(motus->captura))
            {
                redde FALSUM;
            }
            (vacuum)pictor_documentum_glutinationem_radere(pa->doc,
                pa->rasura.x, pa->rasura.y, p.x, p.y, n, restituere);
            pa->rasura = p;
            si (ev->genus == EVENTUS_MUS_LIBERATUS)
            {
                motus_captura_tollere(motus);
            }
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* <tractator/> (vide caput) */
b32
pictor_glutinatio_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    (vacuum)destinatio;
    si (!repo || !ev || !nodus)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(nodus->id, "tabula"))
    {
        redde glutinationem_radere(repo, motus, nodus, ev, ctx);
    }
    si (ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(nodus->id, "glutinatio.A"))
    {
        redde glutinationem_scribere(repo, ctx, litterae_glutinatio_a);
    }
    si (chorda_aequalis_literis(nodus->id, "glutinatio.B"))
    {
        redde glutinationem_scribere(repo, ctx, litterae_glutinatio_b);
    }
    si (chorda_aequalis_literis(nodus->id, "glutinatio.confirmare"))
    {
        redde glutinationem_confirmare(repo, ctx);
    }
    si (chorda_aequalis_literis(nodus->id, "glutinatio.abicere"))
    {
        redde glutinationem_abicere(repo, ctx);
    }
    redde FALSUM;
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
    si (chorda_aequalis_literis(nomen_instrumenti, "linea"))
    {
        redde litterae_linea;
    }
    si (chorda_aequalis_literis(nomen_instrumenti, "spongia"))
    {
        redde litterae_spongia;
    }
    redde litterae_penicillus;
}

/* plan-5 T7: campus inscriptionis per claves (definitio infra) */
interior b32
inscriptionis_clavem_tractare (
    InsulaRepositorium* repo,
                vacuum* ctx,
      constans Eventus* ev);

/* plan-5 T6: spectator per claves (definitio infra) */
interior b32
spectatam_movere (
    InsulaRepositorium* repo,
                vacuum* ctx,
                   s32  gradus);

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
        /* palette: optio 'claudit' - dimissio post actionem (S5c) */
        /* linea pendens instrumento mutato abicitur */
        si (motus && !chorda_vacua(motus->captura))
        {
            lineam_abicere(motus, ev->tempus);
        }
        redde mutare_ramum(&ramus, instrumentum_ponere,
            litterae_instrumenti(nomen_instrumenti));
    }
    /* C1b T1: depositio (radici venit - nec indicator nec focus) =
     * glutinatio imaginis depositae */
    si (ev->genus == EVENTUS_DEPOSITIO)
    {
        redde imaginem_depositam_glutinare(repo, ctx, ev);
    }
    /* plan-5 T7: dialogus inscriptionis apertus - textus et claves
     * campi; claves ceterae consumuntur (nulla instrumenta dum
     * scribitur), Esc ad dimissionem */
    si (   ctx && (   ev->genus == EVENTUS_TEXTUS
                   || ev->genus == EVENTUS_CLAVIS_DEPRESSUS))
    {
        InsulaRamus  r;
             chorda* palette;

        r        = ramus_pictoris(repo, ctx);
        palette  = insula_ramus_attributum(&r, "palette");
        si (palette && chorda_aequalis_literis(*palette, "inscriptio"))
        {
            redde inscriptionis_clavem_tractare(repo, ctx, ev);
        }
    }
    si (ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    /* glutinatio pendens: Esc abicit, Reditus confirmat, Cmd+V
     * reponit; claves ceterae (instrumenta, revocatio) nihil agunt */
    si (   ctx && ((PictorActiones*)ctx)->doc
        && ((PictorActiones*)ctx)->doc->glutinatio.pendens)
    {
        si (ev->datum.clavis.clavis == CLAVIS_EFFUGIUM)
        {
            redde glutinationem_abicere(repo, ctx);
        }
        si (ev->datum.clavis.clavis == CLAVIS_REDITUS)
        {
            redde glutinationem_confirmare(repo, ctx);
        }
        si (   ev->datum.clavis.runa                       == 'v'
            && (ev->datum.clavis.modificantes & MOD_SUPER) != ZEPHYRUM)
        {
            redde imaginem_glutinare(repo, ctx);
        }
        redde FALSUM;
    }
    /* P1b: Esc palettam apertam claudit - systema dimissio (S5c) eum
     * ante actionem consumit; hic Esc nihil agit */
    si (ev->datum.clavis.clavis == CLAVIS_EFFUGIUM)
    {
        redde FALSUM;
    }
    /* plan-5 T6: spectator - ← → (Mac) ut tractus */
    si (   ctx && (   ev->datum.clavis.clavis == CLAVIS_SINISTER
                   || ev->datum.clavis.clavis == CLAVIS_DEXTER))
    {
        InsulaRamus  r;
             chorda* palette;

        r        = ramus_pictoris(repo, ctx);
        palette  = insula_ramus_attributum(&r, "palette");
        si (palette && chorda_aequalis_literis(*palette, "spectator"))
        {
            redde spectatam_movere(repo, ctx, ev->datum.clavis.clavis
                == CLAVIS_DEXTER ? I : -I);
        }
    }
    /* Cmd+Z revocat, Cmd+Shift+Z reficit (historia: gradus multi);
     * linea pendens (captura) abicitur solum */
    si (   ev->datum.clavis.runa                       == 'z'
        && (ev->datum.clavis.modificantes & MOD_SUPER) != ZEPHYRUM)
    {
        PictorActiones* pa;

        pa = (PictorActiones*)ctx;
        si (motus && !chorda_vacua(motus->captura))
        {
            lineam_abicere(motus, ev->tempus);
            redde VERUM;
        }
        si (!pa || !pa->doc)
        {
            redde FALSUM;
        }
        si ((ev->datum.clavis.modificantes & MOD_SHIFT) != ZEPHYRUM)
        {
            (vacuum)pictor_documentum_reficere(pa->doc);
        }
        alioquin
        {
            (vacuum)pictor_documentum_revocare(pa->doc);
        }
        redde VERUM;
    }
    /* L5: Cmd+V - imago glutinata */
    si (   ev->datum.clavis.runa                       == 'v'
        && (ev->datum.clavis.modificantes & MOD_SUPER) != ZEPHYRUM)
    {
        redde imaginem_glutinare(repo, ctx);
    }
    /* clavis LOGICA (runa sine maiuscula, dispositionis praesentis);
     * sub Cmd/Ctrl brevitas est (Cmd+P imprimere), non 'p' (A5) */
    si (   (ev->datum.clavis.runa == 'p'
        || ev->datum.clavis.runa == 'a'
        || ev->datum.clavis.runa == 'e'
        || ev->datum.clavis.runa == 'l')
        && (ev->datum.clavis.modificantes & (MOD_SUPER | MOD_IMPERIUM))
               == ZEPHYRUM)
    {
        ramus = ramus_pictoris(repo, ctx);
        /* linea pendens instrumento mutato abicitur */
        si (motus && !chorda_vacua(motus->captura))
        {
            lineam_abicere(motus, ev->tempus);
        }
        redde mutare_ramum(&ramus, instrumentum_ponere,
            ev->datum.clavis.runa == 'p' ? litterae_penicillus
            : ev->datum.clavis.runa == 'a' ? litterae_aspergillum
            : ev->datum.clavis.runa == 'l' ? litterae_linea
                                           : litterae_spongia);
    }
    redde FALSUM;
}

/* <tractator/> */
b32
pictor_imaginem_eligere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    PictorActiones* pa;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)destinatio;
    (vacuum)nodus;
    pa = (PictorActiones*)ctx;
    si (   !ev || ev->genus != EVENTUS_MUS_DEPRESSUS || !pa
        || !pa->imaginem_eligere)
    {
        redde FALSUM;
    }
    redde pa->imaginem_eligere(pa->imaginem_eligere_ctx);
}

/* plan-5 T3 (communio D9/D12): spatium picturae novae = ULID - unicum
 * sine coordinatione inter machinas, ordine temporis; picturae
 * veteres "pictura-<n>" nomen suum servant */
interior vacuum
spatium_novum (
    character exitus[XXXII])
{
    moneta_ulid(exitus);
}

/* campus 'palette' sub nomine domini (palette.aperire) - res menu
 * eum ex actione 'elenchus.agere' scribunt; NIHIL claudit */
interior b32
palettam_scribere (
    InsulaRepositorium* repo,
                vacuum* ctx,
             character* valor)
{
    PictorActiones* pa;
       InsulaRamus  ramus;
            chorda  prior;
               b32  scriptum;

    pa     = (PictorActiones*)ctx;
    ramus  = ramus_pictoris(repo, ctx);
    prior  = repo->scriptor;
    insula_scriptorem_ponere(repo, chorda_ex_literis("palette.aperire",
        pa->doc->piscina));
    scriptum = mutare_ramum(&ramus, palettam_mutator, valor);
    insula_scriptorem_ponere(repo, prior);
    redde scriptum;
}

/* campus 'pagina_pinacothecae' (dominus pinacotheca.agere); valor
 * <= 0 tollit (absens = 0) */
interior vacuum
paginam_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
          s32 valor;
    character t[XVI];

    valor = ctx ? *(s32*)ctx : ZEPHYRUM;
    si (valor <= ZEPHYRUM)
    {
        (vacuum)insula_attributum_tollere(radix, "pagina_pinacothecae");
        redde;
    }
    sprintf(t, "%ld", (longus)valor);
    insula_attributum_ponere(radix, p, in, "pagina_pinacothecae", t);
}

interior b32
paginam_scribere (
     InsulaRepositorium* repo,
                 vacuum* ctx,
                    s32  valor)
{
    PictorActiones* pa;
       InsulaRamus  ramus;
            chorda  prior;
               b32  scriptum;

    pa     = (PictorActiones*)ctx;
    ramus  = ramus_pictoris(repo, ctx);
    prior  = repo->scriptor;
    insula_scriptorem_ponere(repo, chorda_ex_literis(
        "pinacotheca.agere", pa->doc->piscina));
    scriptum = mutare_ramum(&ramus, paginam_mutator, &valor);
    insula_scriptorem_ponere(repo, prior);
    redde scriptum;
}

/* plan-5 T6: campus 'pagina_communium' (dominus communia.agere) */
interior vacuum
paginam_communium_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
          s32 valor;
    character t[XVI];

    valor = ctx ? *(s32*)ctx : ZEPHYRUM;
    si (valor <= ZEPHYRUM)
    {
        (vacuum)insula_attributum_tollere(radix, "pagina_communium");
        redde;
    }
    sprintf(t, "%ld", (longus)valor);
    insula_attributum_ponere(radix, p, in, "pagina_communium", t);
}

interior b32
communium_paginam_scribere (
     InsulaRepositorium* repo,
                 vacuum* ctx,
                    s32  valor)
{
    PictorActiones* pa;
       InsulaRamus  ramus;
            chorda  prior;
               b32  scriptum;

    pa     = (PictorActiones*)ctx;
    ramus  = ramus_pictoris(repo, ctx);
    prior  = repo->scriptor;
    insula_scriptorem_ponere(repo, chorda_ex_literis("communia.agere",
        pa->doc->piscina));
    scriptum = mutare_ramum(&ramus, paginam_communium_mutator, &valor);
    insula_scriptorem_ponere(repo, prior);
    redde scriptum;
}

/* campus 'spectata' = "replica:spatium" (dominus spectator.agere);
 * NIHIL tollit */
interior vacuum
spectatam_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    si (!ctx)
    {
        (vacuum)insula_attributum_tollere(radix, "spectata");
        redde;
    }
    insula_attributum_ponere(radix, p, in, "spectata",
        (constans character*)ctx);
}

interior b32
spectatam_scribere (
    InsulaRepositorium* repo,
                vacuum* ctx,
             character* valor)
{
    PictorActiones* pa;
       InsulaRamus  ramus;
            chorda  prior;
               b32  scriptum;

    pa     = (PictorActiones*)ctx;
    ramus  = ramus_pictoris(repo, ctx);
    prior  = repo->scriptor;
    insula_scriptorem_ponere(repo, chorda_ex_literis("spectator.agere",
        pa->doc->piscina));
    scriptum = mutare_ramum(&ramus, spectatam_mutator, valor);
    insula_scriptorem_ponere(repo, prior);
    redde scriptum;
}

/* "replica:spatium" picturae k indicis communis (in piscina) */
interior character*
identitas_communis (
    constans PictorCommunia* pc,
                        s32  k,
                    Piscina* piscina)
{
    PictorPicturaAliena* a;

    a = (PictorPicturaAliena*)xar_obtinere(pc->picturae, (i32)k);
    redde chorda_ut_cstr(chorda_concatenare(chorda_concatenare(a->replica,
        chorda_ex_literis(":", piscina), piscina), a->spatium, piscina),
        piscina);
}

/* index picturae spectatae in indice communi; -1 si abest */
interior s32
index_spectatae (
    InsulaRepositorium* repo,
                vacuum* ctx)
{
    PictorActiones* pa;
       InsulaRamus  ramus;
            chorda* s;
            chorda  replica;
            chorda  spatium;
               i32  i;

    pa     = (PictorActiones*)ctx;
    ramus  = ramus_pictoris(repo, ctx);
    s      = insula_ramus_attributum(&ramus, "spectata");
    si (!s || !pa->communia)
    {
        redde -I;
    }
    per (i = ZEPHYRUM; i < s->mensura && s->datum[i] != ':'; i++)
    {
    }
    si (i == ZEPHYRUM || i >= s->mensura)
    {
        redde -I;
    }
    replica.datum    = s->datum;
    replica.mensura  = i;
    spatium.datum    = s->datum + i + I;
    spatium.mensura  = s->mensura - i - I;
    redde pictor_communia_index(pa->communia, replica, spatium);
}

/* spectator: gradus +1 / -1 circulariter (spec IX.6: post ultimam
 * prima) */
interior b32
spectatam_movere (
    InsulaRepositorium* repo,
                vacuum* ctx,
                   s32  gradus)
{
    PictorActiones* pa;
               s32  k;
               s32  n;

    pa  = (PictorActiones*)ctx;
    k   = index_spectatae(repo, ctx);
    n  = (pa->communia && pa->communia->picturae)
        ? (s32)xar_numerus(pa->communia->picturae) : ZEPHYRUM;
    si (k < ZEPHYRUM || n == ZEPHYRUM)
    {
        redde FALSUM;
    }
    k = ((k + gradus) % n + n) % n;
    redde spectatam_scribere(repo, ctx, identitas_communis(pa->communia,
        k, pa->doc->piscina));
}

/* ===== plan-5 T7: salutatio et inscriptio ===== */

#define CAMPUS_MAXIMUS  CCLIV

/* gradus inscriptionis per compositionem (-1 nulla) */
interior s32
gradus_inscriptionis (
    constans PictorActiones* pa)
{
    redde (pa->compositio && pa->compositio->inscriptionis_gradus)
        ? pa->compositio->inscriptionis_gradus(pa->compositio->auctor_ctx)
        : -I;
}

interior b32
gradus_exspectans (
    s32 g)
{
    redde g == (s32)INSCRIPTIO_PETIT || g == (s32)INSCRIPTIO_EXSPECTAT
        || g == (s32)INSCRIPTIO_CONFIRMAT
        || g == (s32)INSCRIPTIO_CODEX_FALSUS;
}

interior vacuum
claviaturam_ponere (
    PictorActiones* pa,
               i32  genus)
{
    si (pa->claviaturam_ponere)
    {
        pa->claviaturam_ponere(pa->claviatura_ctx, genus);
    }
}

/* claviatura secundum gradum: epistula, codex, aut nulla (inscripta) */
interior vacuum
claviaturam_gradui_ponere (
    PictorActiones* pa)
{
    s32 g;

    g = gradus_inscriptionis(pa);
    claviaturam_ponere(pa, g == (s32)INSCRIPTIO_INSCRIPTA
        ? (i32)TESTA_CLAVIATURA_NULLA : gradus_exspectans(g)
        ? (i32)TESTA_CLAVIATURA_CODEX : (i32)TESTA_CLAVIATURA_EPISTULA);
}

/* campi 'inscriptio_textus' / 'inscriptio_error' (dominus
 * inscriptio.agere); valor NIHIL tollit */
nomen structura {
    constans character* attributum;
                chorda  valor;
                   b32  tollere;
} CampusInscriptionis;

interior vacuum
campum_inscriptionis_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    CampusInscriptionis* k = (CampusInscriptionis*)ctx;

    si (k->tollere)
    {
        (vacuum)insula_attributum_tollere(radix, k->attributum);
        redde;
    }
    insula_attributum_ponere(radix, p, in, k->attributum,
        chorda_ut_cstr(k->valor, p));
}

interior b32
campum_inscriptionis_scribere (
    InsulaRepositorium* repo,
                vacuum* ctx,
    constans character* attributum,
                chorda  valor,
                   b32  tollere)
{
         PictorActiones* pa;
            InsulaRamus  ramus;
                 chorda  prior;
                    b32  scriptum;
    CampusInscriptionis  k;

    pa            = (PictorActiones*)ctx;
    ramus         = ramus_pictoris(repo, ctx);
    prior         = repo->scriptor;
    k.attributum  = attributum;
    k.valor       = valor;
    k.tollere     = tollere || valor.mensura == ZEPHYRUM;
    insula_scriptorem_ponere(repo, chorda_ex_literis("inscriptio.agere",
        pa->doc->piscina));
    scriptum = mutare_ramum(&ramus, campum_inscriptionis_mutator, &k);
    insula_scriptorem_ponere(repo, prior);
    redde scriptum;
}

interior chorda
textus_inscriptionis (
    InsulaRepositorium* repo,
                vacuum* ctx,
               Piscina* piscina)
{
    InsulaRamus  r;
         chorda* t;

    r = ramus_pictoris(repo, ctx);
    t = insula_ramus_attributum(&r, "inscriptio_textus");
    redde t ? chorda_ex_literis(chorda_ut_cstr(*t, piscina), piscina)
            : chorda_ex_literis("", piscina);
}

interior vacuum
campos_inscriptionis_purgare (
    InsulaRepositorium* repo,
                vacuum* ctx)
{
    chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    (vacuum)campum_inscriptionis_scribere(repo, ctx,
        "inscriptio_textus",
        vacua, VERUM);
    (vacuum)campum_inscriptionis_scribere(repo, ctx, "inscriptio_error",
        vacua, VERUM);
}

/* Send (aut REDITUS): gradus I epistulam mittit, gradus II codicem */
interior b32
inscriptionem_mittere (
    InsulaRepositorium* repo,
                vacuum* ctx)
{
    PictorActiones* pa;
            chorda  textus;
               s32  g;

    pa      = (PictorActiones*)ctx;
    textus  = textus_inscriptionis(repo, ctx, pa->doc->piscina);
    g       = gradus_inscriptionis(pa);
    /* epistula in itinere (CCL ms ad summum): nondum codex accipitur -
     * nihil, sine errore */
    si (g == (s32)INSCRIPTIO_PETIT || g == (s32)INSCRIPTIO_CONFIRMAT)
    {
        redde VERUM;
    }
    si (gradus_exspectans(g))
    {
        si (!pa->codicem_dare || !pa->codicem_dare(pa->inscriptio_ctx,
                textus))
        {
            redde campum_inscriptionis_scribere(repo, ctx,
                "inscriptio_error", chorda_ex_literis("codex",
                pa->doc->piscina), FALSUM);
        }
        campos_inscriptionis_purgare(repo, ctx);
        redde VERUM;
    }
    si (   g == (s32)INSCRIPTIO_INSCRIPTA || textus.mensura == ZEPHYRUM
        || !pa->inscriptionem_incipere)
    {
        redde FALSUM;
    }
    si (!pa->inscriptionem_incipere(pa->inscriptio_ctx, textus))
    {
        redde FALSUM;
    }
    campos_inscriptionis_purgare(repo, ctx);
    claviaturam_ponere(pa, (i32)TESTA_CLAVIATURA_CODEX);
    redde VERUM;
}

/* textus commissus addit (ad CAMPUS_MAXIMUS), RETRORSUM signum ultimum
 * (UTF-8 integrum) tollit, REDITUS mittit; ceterae claves consumuntur,
 * Esc non (dimissio dialogum claudit) */
interior b32
inscriptionis_clavem_tractare (
    InsulaRepositorium* repo,
                vacuum* ctx,
      constans Eventus* ev)
{
    PictorActiones* pa;
            chorda  textus;
            chorda  novum;
               i32  n;

    pa = (PictorActiones*)ctx;
    si (ev->genus == EVENTUS_TEXTUS)
    {
        si (ev->datum.textus.genus != EVENTUS_TEXTUS_COMMISSUM)
        {
            redde VERUM;
        }
        textus  = textus_inscriptionis(repo, ctx, pa->doc->piscina);
        n       = ev->datum.textus.contentum.mensura;
        si (textus.mensura + n > CAMPUS_MAXIMUS)
        {
            n = CAMPUS_MAXIMUS > textus.mensura
                ? CAMPUS_MAXIMUS - textus.mensura : ZEPHYRUM;
        }
        si (n == ZEPHYRUM)
        {
            redde VERUM;
        }
        novum.datum    = ev->datum.textus.contentum.datum;
        novum.mensura  = n;
        redde campum_inscriptionis_scribere(repo, ctx,
            "inscriptio_textus", chorda_concatenare(textus, novum,
            pa->doc->piscina), FALSUM);
    }
    si (ev->datum.clavis.clavis == CLAVIS_EFFUGIUM)
    {
        redde FALSUM;
    }
    si (ev->datum.clavis.clavis == CLAVIS_RETRORSUM)
    {
        textus = textus_inscriptionis(repo, ctx, pa->doc->piscina);
        si (textus.mensura == ZEPHYRUM)
        {
            redde VERUM;
        }
        n = textus.mensura - I;
        dum (n > ZEPHYRUM && ((i8)textus.datum[n] & 0xC0) == 0x80)
        {
            n--;
        }
        textus.mensura = n;
        redde campum_inscriptionis_scribere(repo, ctx,
            "inscriptio_textus",
            textus, FALSUM);
    }
    si (ev->datum.clavis.clavis == CLAVIS_REDITUS)
    {
        (vacuum)inscriptionem_mittere(repo, ctx);
    }
    redde VERUM;
}

/* praesentia "pictor/salutatio" = "visa" */
interior vacuum
salutationem_notare (
    PictorActiones* pa)
{
    (vacuum)volumen_praesens_ponere(pa->doc->volumen, chorda_ex_literis(
        "pictor/salutatio", pa->doc->piscina), chorda_ex_literis("visa",
        pa->doc->piscina));
}

b32
pictor_salutationem_ponere_in (
    InsulaRepositorium* repo,
        PictorActiones* pa)
{
    hic_manens character SALUTATIO[]   = "salutatio";
    hic_manens character INSCRIPTIO[]  = "inscriptio";
                  chorda visa;
                     b32 inventum;
                     s32 g;

    si (   !repo || !pa || !pa->doc || !pa->compositio
        || !pa->compositio->communio_praesto)
    {
        redde FALSUM;
    }
    /* inscriptio pendens (applicatio necata medio): dialogus redit */
    g = gradus_inscriptionis(pa);
    si (gradus_exspectans(g))
    {
        claviaturam_ponere(pa, (i32)TESTA_CLAVIATURA_CODEX);
        redde palettam_scribere(repo, pa, INSCRIPTIO);
    }
    visa = volumen_praesens_legere(pa->doc->volumen, chorda_ex_literis(
        "pictor/salutatio", pa->doc->piscina), pa->doc->piscina,
        &inventum);
    si (   pa->compositio->inscripta
        || (inventum && chorda_aequalis_literis(visa, "visa")))
    {
        redde FALSUM;
    }
    redde palettam_scribere(repo, pa, SALUTATIO);
}

/* <tractator/> */
b32
pictor_elenchum_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
              PictorActiones* pa;
                    Elenchus  e;
                 ElenchusRes  res[V];
    PictorElenchiCondiciones  condiciones;
                         s32  k;
          constans character* titulus;
                   character  spatium[XXXII];

    (vacuum)motus;
    (vacuum)destinatio;
    pa = (PictorActiones*)ctx;
    si (   !repo || !ev || ev->genus != EVENTUS_MUS_DEPRESSUS || !pa
        || !pa->doc)
    {
        redde FALSUM;
    }
    /* plan-5 T6: condiciones ut compositio (sine ea: importare ex
     * servitio, nulla communio) */
    condiciones = pictor_elenchi_condiciones(pa->compositio);
    si (!pa->compositio)
    {
        condiciones.importare = pa->imaginem_eligere != NIHIL;
    }
    pictor_elenchum_parare(&condiciones, res, &e);
    k = elenchus_index(nodus);
    si (k < ZEPHYRUM || k >= (s32)e.numerus)
    {
        redde FALSUM;
    }
    titulus = e.res[k].titulus;
    si (strcmp(titulus, "pinacotheca.aperire") == ZEPHYRUM)
    {
        hic_manens character PINACOTHECA[] = "pinacotheca";

        /* T5: status parandus ante compositionem (picturae, scheda) */
        si (   pa->pinacotheca && !pictor_pinacothecam_parare(
                pa->pinacotheca))
        {
            redde FALSUM;
        }
        (vacuum)paginam_scribere(repo, ctx, ZEPHYRUM);
        redde palettam_scribere(repo, ctx, PINACOTHECA);
    }
    /* plan-5 T6: Shared pictures (status paratur ante compositionem) */
    si (strcmp(titulus, "communia.aperire") == ZEPHYRUM)
    {
        hic_manens character COMMUNIA[] = "communia";

        si (pa->communia && !pictor_communia_parare(pa->communia))
        {
            redde FALSUM;
        }
        (vacuum)communium_paginam_scribere(repo, ctx, ZEPHYRUM);
        redde palettam_scribere(repo, ctx, COMMUNIA);
    }
    /* plan-5 T6/T7: Sign in... - dialogus inscriptionis (T7 componit) */
    si (strcmp(titulus, "inscriptio.aperire") == ZEPHYRUM)
    {
        hic_manens character INSCRIPTIO[] = "inscriptio";

        claviaturam_gradui_ponere(pa);
        redde palettam_scribere(repo, ctx, INSCRIPTIO);
    }
    (vacuum)palettam_scribere(repo, ctx, NIHIL);
    si (strcmp(titulus, "pictura.nova") == ZEPHYRUM)
    {
        si (!pa->picturam_condere)
        {
            redde FALSUM;
        }
        spatium_novum(spatium);
        redde pa->picturam_condere(pa->montatio, spatium);
    }
    redde pa->imaginem_eligere
        && pa->imaginem_eligere(pa->imaginem_eligere_ctx);
}

/* "pinax:<lat>x<alt>:<seq>:<spatium>[:electum]" -> spatium */
interior chorda
spatium_ex_pinace (
    chorda titulus)
{
    chorda s;
       i32 i;
       i32 signa;

    s.datum    = titulus.datum;
    s.mensura  = ZEPHYRUM;
    signa      = ZEPHYRUM;
    per (i = ZEPHYRUM; i < titulus.mensura; i++)
    {
        si (titulus.datum[i] == ':' && ++signa == III)
        {
            s.datum    = titulus.datum + i + I;
            s.mensura  = titulus.mensura - i - I;
            frange;
        }
    }
    si (   s.mensura >= VIII
        && memcmp(s.datum + s.mensura - VIII, ":electum", VIII)
           == ZEPHYRUM)
    {
        s.mensura -= VIII;
    }
    redde s;
}

/* <tractator/> */
b32
pictor_pinacothecam_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    PictorActiones* pa;
       InsulaRamus  ramus;
            chorda* a;
               s32  pagina;
               s32  paginae;

    (vacuum)motus;
    (vacuum)destinatio;
    pa = (PictorActiones*)ctx;
    si (   !repo || !ev || ev->genus != EVENTUS_MUS_DEPRESSUS || !pa
        || !pa->doc || !nodus)
    {
        redde FALSUM;
    }
    si (post_praefixum(nodus, "pinax.").mensura > ZEPHYRUM)
    {
        chorda spatium;

        spatium = spatium_ex_pinace(nodus->titulus);
        (vacuum)paginam_scribere(repo, ctx, ZEPHYRUM);
        (vacuum)palettam_scribere(repo, ctx, NIHIL);
        redde pa->picturam_ponere && pa->picturam_ponere(pa->montatio,
            chorda_ut_cstr(spatium, pa->doc->piscina));
    }
    si (chorda_aequalis_literis(nodus->id, "pinacotheca.claudere"))
    {
        (vacuum)paginam_scribere(repo, ctx, ZEPHYRUM);
        redde palettam_scribere(repo, ctx, NIHIL);
    }
    ramus   = ramus_pictoris(repo, ctx);
    a       = insula_ramus_attributum(&ramus, "pagina_pinacothecae");
    pagina  = ZEPHYRUM;
    si (a)
    {
        (vacuum)chorda_ut_s32(*a, &pagina);
    }
    paginae  = (pa->pinacotheca && pa->pinacotheca->picturae)
        ? ((s32)xar_numerus(pa->pinacotheca->picturae) + XI) / XII : I;
    si (chorda_aequalis_literis(nodus->id, "pinacotheca.prior"))
    {
        redde paginam_scribere(repo, ctx, pagina > ZEPHYRUM
            ? pagina - I : ZEPHYRUM);
    }
    si (chorda_aequalis_literis(nodus->id, "pinacotheca.posterior"))
    {
        redde paginam_scribere(repo, ctx, pagina + I < paginae
            ? pagina + I : pagina);
    }
    redde FALSUM;
}

/* <tractator/> Shared pictures (spec IX.5): 'communia.pinax.<k>'
 * spectatorem eius picturae aperit; prior / posterior paginam; X
 * claudit */
b32
pictor_communia_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    hic_manens character  SPECTATOR[] = "spectator";
          PictorActiones* pa;
             InsulaRamus  ramus;
                  chorda* a;
                  chorda  k;
                     s32  pagina;
                     s32  paginae;
                     s32  index;

    (vacuum)motus;
    (vacuum)destinatio;
    pa = (PictorActiones*)ctx;
    si (   !repo || !ev || ev->genus != EVENTUS_MUS_DEPRESSUS || !pa
        || !pa->doc || !nodus || !pa->communia)
    {
        redde FALSUM;
    }
    ramus   = ramus_pictoris(repo, ctx);
    a       = insula_ramus_attributum(&ramus, "pagina_communium");
    pagina  = ZEPHYRUM;
    si (a)
    {
        (vacuum)chorda_ut_s32(*a, &pagina);
    }
    paginae = pa->communia->picturae ? ((s32)xar_numerus(
        pa->communia->picturae) + XI) / XII : I;
    k = post_praefixum(nodus, "communia.pinax.");
    si (k.mensura > ZEPHYRUM && chorda_ut_s32(k, &index))
    {
        index += pagina * XII;
        si (   !pa->communia->picturae || index < ZEPHYRUM
            || index >= (s32)xar_numerus(pa->communia->picturae))
        {
            redde FALSUM;
        }
        (vacuum)spectatam_scribere(repo, ctx, identitas_communis(
            pa->communia, index, pa->doc->piscina));
        redde palettam_scribere(repo, ctx, SPECTATOR);
    }
    si (chorda_aequalis_literis(nodus->id, "communia.claudere"))
    {
        (vacuum)communium_paginam_scribere(repo, ctx, ZEPHYRUM);
        redde palettam_scribere(repo, ctx, NIHIL);
    }
    si (chorda_aequalis_literis(nodus->id, "communia.prior"))
    {
        redde communium_paginam_scribere(repo, ctx, pagina > ZEPHYRUM
            ? pagina - I : ZEPHYRUM);
    }
    si (chorda_aequalis_literis(nodus->id, "communia.posterior"))
    {
        redde communium_paginam_scribere(repo, ctx, pagina + I < paginae
            ? pagina + I : pagina);
    }
    redde FALSUM;
}

/* <tractator/> spectator (spec IX.6): tractus in 'spectator.imago'
 * (|dx| >= quarta latitudinis, |dx| > |dy|) sinister proximam,
 * dexter priorem; ‹ › idem; "Edit a copy" exemplum facit et ostendit;
 * X aut ictus extra imaginem ad rete redit (pagina eadem). Lectio
 * sola: nihil in documenta scribit. */
b32
pictor_spectatorem_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    hic_manens character  COMMUNIA[] = "communia";
          PictorActiones* pa;
                 Punctum  p;
                     s32  dx;
                     s32  dy;
                     s32  k;

    (vacuum)motus;
    (vacuum)destinatio;
    pa = (PictorActiones*)ctx;
    si (!repo || !ev || !pa || !pa->doc || !nodus)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(nodus->id, "spectator.imago"))
    {
        p.x  = (s32)ev->datum.mus.x;
        p.y  = (s32)ev->datum.mus.y;
        p    = destinatio_ad_locale(nodus, p);
        si (ev->genus == EVENTUS_TRACTUS_INCIPIT)
        {
            pa->origo_tractus = p;
            redde VERUM;
        }
        si (ev->genus == EVENTUS_TRACTUS_FINIT)
        {
            dx = p.x - pa->origo_tractus.x;
            dy = p.y - pa->origo_tractus.y;
            si (   (dx < ZEPHYRUM ? -dx : dx) * IV
                < nodus->fines.latitudo
                || (dx < ZEPHYRUM ? -dx : dx) <= (dy
                    < ZEPHYRUM ? -dy : dy))
            {
                redde VERUM;
            }
            redde spectatam_movere(repo, ctx, dx < ZEPHYRUM ? I : -I);
        }
        /* pressio in imagine: consumitur (non claudit) */
        redde ev->genus == EVENTUS_MUS_DEPRESSUS
            || ev->genus == EVENTUS_MUS_LIBERATUS
            || ev->genus == EVENTUS_TRACTUS;
    }
    si (ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(nodus->id, "spectator.prior"))
    {
        redde spectatam_movere(repo, ctx, -I);
    }
    si (chorda_aequalis_literis(nodus->id, "spectator.posterior"))
    {
        redde spectatam_movere(repo, ctx, I);
    }
    si (chorda_aequalis_literis(nodus->id, "spectator.exemplum"))
    {
        PictorPicturaAliena* a;

        k = index_spectatae(repo, ctx);
        si (k < ZEPHYRUM || !pa->exemplum_facere)
        {
            redde FALSUM;
        }
        a = (PictorPicturaAliena*)xar_obtinere(pa->communia->picturae,
            (i32)k);
        (vacuum)spectatam_scribere(repo, ctx, NIHIL);
        (vacuum)communium_paginam_scribere(repo, ctx, ZEPHYRUM);
        (vacuum)palettam_scribere(repo, ctx, NIHIL);
        redde pa->exemplum_facere(pa->montatio, a->replica,
            chorda_ut_cstr(a->spatium, pa->doc->piscina));
    }
    /* X aut ictus extra imaginem (dialogus ipse): ad rete */
    (vacuum)spectatam_scribere(repo, ctx, NIHIL);
    redde palettam_scribere(repo, ctx, COMMUNIA);
}

/* <tractator/> salutatio (spec IX.1): Sign in… -> dialogus
 * inscriptionis; Draw without signing in -> tabula. Utrumque
 * salutationem notat (numquam iterum). */
b32
pictor_salutationem_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    hic_manens character  INSCRIPTIO[] = "inscriptio";
          PictorActiones* pa;

    (vacuum)motus;
    (vacuum)destinatio;
    pa = (PictorActiones*)ctx;
    si (   !repo || !ev || ev->genus != EVENTUS_MUS_DEPRESSUS || !pa
        || !pa->doc || !nodus)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(nodus->id, "salutatio.inscribere"))
    {
        salutationem_notare(pa);
        claviaturam_gradui_ponere(pa);
        redde palettam_scribere(repo, ctx, INSCRIPTIO);
    }
    si (chorda_aequalis_literis(nodus->id, "salutatio.pingere"))
    {
        salutationem_notare(pa);
        redde palettam_scribere(repo, ctx, NIHIL);
    }
    redde FALSUM;
}

/* <tractator/> inscriptio (spec IX.2): Send, Cancel (abicit, tessera
 * deleta), OK (post inscriptionem), campus (claviatura iterum) */
b32
pictor_inscriptionem_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    PictorActiones* pa;

    (vacuum)motus;
    (vacuum)destinatio;
    pa = (PictorActiones*)ctx;
    si (   !repo || !ev || ev->genus != EVENTUS_MUS_DEPRESSUS || !pa
        || !pa->doc || !nodus)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(nodus->id, "inscriptio.mittere"))
    {
        redde inscriptionem_mittere(repo, ctx);
    }
    si (chorda_aequalis_literis(nodus->id, "inscriptio.campus"))
    {
        claviaturam_gradui_ponere(pa);
        redde VERUM;
    }
    si (   chorda_aequalis_literis(nodus->id, "inscriptio.abicere")
        || chorda_aequalis_literis(nodus->id, "inscriptio.confirmata"))
    {
        si (pa->inscriptionem_abicere)
        {
            pa->inscriptionem_abicere(pa->inscriptio_ctx);
        }
        campos_inscriptionis_purgare(repo, ctx);
        claviaturam_ponere(pa, (i32)TESTA_CLAVIATURA_NULLA);
        redde palettam_scribere(repo, ctx, NIHIL);
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
    aperta = insula_ramus_attributum(&ramus,
        "palette");
    si (aperta && chorda_aequalis(*aperta, genus))
    {
        redde palettam_claudere(&ramus);
    }
    redde mutare_ramum(&ramus, palettam_mutator,
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
    redde mutare_ramum(&ramus, colorem_mutator, &cp);
}

/* <tractator/> */
b32
pictor_exemplar_ponere (
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
    valor          = post_praefixum(nodus, "optio.exemplar.");
    cp.attributum  = "exemplar";
    si (   valor.mensura == ZEPHYRUM || !chorda_ut_s32(valor, &cp.valor)
        || cp.valor < ZEPHYRUM || cp.valor >= (s32)EXEMPLAR_NUMERUS)
    {
        redde FALSUM;
    }
    ramus = ramus_pictoris(repo, ctx);
    redde mutare_ramum(&ramus, colorem_mutator, &cp);
}

/* P4a: magnitudines optionum (pixela penicilli) */
hic_manens constans s32 magnitudines[X] = {
    I, II, III, IV, VI, VIII, XII, XVI, XXXII, LXIV
};

/* P4b: magnitudines aspergilli (multiplex radii) */
hic_manens constans s32 magnitudines_aspergilli[V] = {
    I, II, IV, VIII, XVI
};

/* linea: latitudines (pixela) */
hic_manens constans s32 magnitudines_lineae[V] = {
    I, II, IV, VIII, XVI
};

/* <tractator/> */
b32
pictor_magnitudinem_ponere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
         InsulaRamus  ramus;
       ColorPonendus  cp;
              chorda  valor;
              chorda* instrumentum;
              chorda  penicillus;
        constans s32* tabula;
                 i32  numerus;
                 i32  k;
                 b32  nota;

    (vacuum)motus;
    (vacuum)destinatio;
    si (!repo || !ev || ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    valor = post_praefixum(nodus, "optio.magnitudo.");
    si (valor.mensura == ZEPHYRUM || !chorda_ut_s32(valor, &cp.valor))
    {
        redde FALSUM;
    }
    /* P4b: instrumentum currens - penicillus diametrum, aspergillum
     * multiplicem; ceteri (spongia) nihil. Glutinatio pendens:
     * penicillus semper (magnitudo radendi, Franus 2026-10-13) */
    ramus         = ramus_pictoris(repo, ctx);
    instrumentum  = insula_ramus_attributum(&ramus,
        "instrumentum");
    si (   ctx && ((PictorActiones*)ctx)->doc
        && ((PictorActiones*)ctx)->doc->glutinatio.pendens)
    {
        penicillus    = chorda_ex_literis(litterae_penicillus,
            ((PictorActiones*)ctx)->doc->piscina);
        instrumentum  = &penicillus;
    }
    si (   instrumentum && chorda_aequalis_literis(*instrumentum,
        "aspergillum"))
    {
        cp.attributum  = "magnitudo_aspergilli";
        tabula         = magnitudines_aspergilli;
        numerus        = V;
    }
    alioquin si (instrumentum && chorda_aequalis_literis(*instrumentum,
                 "linea"))
    {
        cp.attributum  = "magnitudo_lineae";
        tabula         = magnitudines_lineae;
        numerus        = V;
    }
    alioquin si (instrumentum && chorda_aequalis_literis(*instrumentum,
                 "penicillus"))
    {
        cp.attributum  = "magnitudo_penicilli";
        tabula         = magnitudines;
        numerus        = X;
    }
    alioquin
    {
        redde FALSUM;
    }
    nota = FALSUM;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (tabula[k] == cp.valor)
        {
            nota = VERUM;
        }
    }
    si (!nota)
    {
        redde FALSUM;
    }
    redde mutare_ramum(&ramus, colorem_mutator, &cp);
}

/* L4: ordo trahendi - ordo destinatus (summo primo) strati tracti ex
 * dy (pixela), ordines XX + II alti; eadem regula in compositore
 * (index insertionis) */
interior s32
ordo_destinatus (
    s32 ordo_tracti,
    s32 dy,
    i32 n)
{
    s32 gradus;
    s32 r;

    gradus  = dy >= ZEPHYRUM ? (dy + XI) / (XX + II)
                             : -((-dy + XI) / (XX + II));
    r = ordo_tracti + gradus;
    r = r < ZEPHYRUM ? ZEPHYRUM : r;
    r = r > (s32)n - I ? (s32)n - I : r;
    redde r;
}

/* L4: solutio intra palettam stratorum (nodus geometricus) */
interior b32
intra_palettam (
    constans Destinatio* destinatio,
                Piscina* p)
{
    chorda g;

    g = destinatio->id_geometricum;
    redde chorda_aequalis_literis(g, "palette")
        || chorda_incipit(g, chorda_ex_literis("stratum.", p))
        || chorda_incipit(g, chorda_ex_literis("oculus.", p))
        || chorda_incipit(g, chorda_ex_literis("strata.", p));
}

/* L4: solutio tractus: ordo novus si motus > III et intra palettam */
interior vacuum
tractum_solvere (
        PictorActiones* pa,
                 Motus* motus,
   constans Destinatio* destinatio,
                   s32  id,
                   s64  tempus)
{
    Punctum p0;
    Punctum p1;
        s32 dx;
        s32 dy;
        s32 index;
        s32 r_tracti;
        s32 r;
        i32 n;
        i32 k;
        s32 summo_primo[PICTOR_STRATA_MAXIMA];
        i32 m;
     chorda actum;

    n = pictor_documentum_numerus_stratorum(pa->doc);
    si (xar_numerus(motus->ictus_pendens) < II)
    {
        lineam_abicere(motus, tempus);
        redde;
    }
    p0 = *(Punctum*)xar_obtinere(motus->ictus_pendens, ZEPHYRUM);
    p1 = *(Punctum*)xar_obtinere(motus->ictus_pendens, I);
    dx = p1.x - p0.x;
    dy = p1.y - p0.y;
    lineam_abicere(motus, tempus);
    si (   (dx * dx + dy * dy <= IX)
        || !intra_palettam(destinatio, pa->doc->piscina))
    {
        redde;
    }
    index = -I;
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (pictor_documentum_stratum(pa->doc, k)->id == id)
        {
            index = (s32)k;
        }
    }
    si (index < ZEPHYRUM)
    {
        redde;
    }
    r_tracti  = (s32)n - I - index;
    r         = ordo_destinatus(r_tracti, dy, n);
    si (r == r_tracti)
    {
        redde;
    }
    /* summo primo sine tracto, tractum in r inserere */
    m = ZEPHYRUM;
    per (k = ZEPHYRUM; k < n; k++)
    {
        s32 alius;

        alius = pictor_documentum_stratum(pa->doc, n - I - k)->id;
        si (alius != id)
        {
            summo_primo[m] = alius;
            m++;
        }
    }
    per (k = m; (s32)k > r; k--)
    {
        summo_primo[k] = summo_primo[k - I];
    }
    summo_primo[r] = id;
    /* ab imo */
    actum = chorda_ex_literis("<stratum actio=\"ordo\" ids=\"",
        pa->doc->piscina);
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (k > ZEPHYRUM)
        {
            actum = chorda_concatenare(actum, chorda_ex_literis(" ",
                pa->doc->piscina), pa->doc->piscina);
        }
        actum = chorda_concatenare(actum, chorda_ex_s32(
            summo_primo[n - I - k], pa->doc->piscina),
            pa->doc->piscina);
    }
    actum = chorda_concatenare(actum, chorda_ex_literis("\"/>",
        pa->doc->piscina), pa->doc->piscina);
    pictor_documentum_actum(pa->doc, actum);
}

/* <tractator/> */
b32
pictor_strata_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
             InsulaRamus  ramus;
          PictorActiones* pa;
           ColorPonendus  cp;
                  chorda  valor;
                     s32  id;
                     s32  currens;
                     s32  maximum;
                     s32  index;
                     i32  k;
                     i32  n;
               character  actum[XCVI];
  constans PictorStratum* s;

    pa = (PictorActiones*)ctx;
    si (!repo || !ev || !pa || !motus || !destinatio)
    {
        redde FALSUM;
    }
    ramus          = ramus_pictoris(repo, ctx);
    currens        = stratum_currens(&ramus, pa->doc);
    n              = pictor_documentum_numerus_stratorum(pa->doc);
    cp.attributum  = "stratum_activum";
    /* 'stratum.<id>': ictus eligit et trahere incipit (L4: captura,
     * punctum absolutum pressionis); motus, solutio */
    valor = post_praefixum(nodus, "stratum.");
    si (valor.mensura > ZEPHYRUM && chorda_ut_s32(valor, &id))
    {
        Punctum q;

        q.x = (s32)ev->datum.mus.x;
        q.y = (s32)ev->datum.mus.y;
        si (ev->genus == EVENTUS_MUS_DEPRESSUS)
        {
            motus_captura_ponere(motus, nodus->id);
            mutare_motum(motus, puncta_vacare, NIHIL, ev->tempus);
            mutare_motum(motus, punctum_addere, &q, ev->tempus);
            cp.valor = id;
            redde mutare_ramum(&ramus, colorem_mutator,
                &cp);
        }
        si (chorda_vacua(motus->captura))
        {
            redde FALSUM;
        }
        si (ev->genus == EVENTUS_MUS_MOTUS)
        {
            mutare_motum(motus, punctum_finale_ponere, &q, ev->tempus);
            redde VERUM;
        }
        si (ev->genus == EVENTUS_MUS_LIBERATUS)
        {
            tractum_solvere(pa, motus, destinatio, id, ev->tempus);
            redde VERUM;
        }
        redde FALSUM;
    }
    si (ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    /* 'oculus.<id>': visibilitas in actis */
    valor = post_praefixum(nodus, "oculus.");
    si (valor.mensura > ZEPHYRUM && chorda_ut_s32(valor, &id))
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            s = pictor_documentum_stratum(pa->doc, k);
            si (s->id == id)
            {
                sprintf(actum, "<stratum actio=\"visibile\" id=\"%d\""
                    " valor=\"%d\"/>", (integer)id,
                    s->visibile ? ZEPHYRUM : I);
                pictor_documentum_actum(pa->doc,
                    chorda_ex_literis(actum,
                    pa->doc->piscina));
            }
        }
        redde VERUM;
    }
    si (nodus && chorda_aequalis_literis(nodus->id, "strata.novum"))
    {
        si (n >= (i32)PICTOR_STRATA_MAXIMA)
        {
            redde VERUM;
        }
        maximum = ZEPHYRUM;
        per (k = ZEPHYRUM; k < n; k++)
        {
            s        = pictor_documentum_stratum(pa->doc, k);
            maximum  = s->id > maximum ? s->id : maximum;
        }
        sprintf(actum,
            "<stratum actio=\"novum\" id=\"%d\" supra=\"%d\"/>",
            (integer)(maximum + I), (integer)currens);
        pictor_documentum_actum(pa->doc, chorda_ex_literis(actum,
            pa->doc->piscina));
        cp.valor = maximum + I;
        redde mutare_ramum(&ramus, colorem_mutator,
            &cp);
    }
    si (nodus && chorda_aequalis_literis(nodus->id, "strata.deletum"))
    {
        si (n <= I)
        {
            redde VERUM;
        }
        index = -I;
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (pictor_documentum_stratum(pa->doc, k)->id == currens)
            {
                index = (s32)k;
            }
        }
        /* inferius currens fit (imum: superius) */
        cp.valor = pictor_documentum_stratum(pa->doc, (i32)(index
            > ZEPHYRUM
            ? index - I : index + I))->id;
        sprintf(actum, "<stratum actio=\"deletum\" id=\"%d\"/>",
            (integer)currens);
        pictor_documentum_actum(pa->doc, chorda_ex_literis(actum,
            pa->doc->piscina));
        redde mutare_ramum(&ramus, colorem_mutator,
            &cp);
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
    actio_registrare(reg, "aspergillum.ictus", pictor_aspergillum_ictus,
        ctx);
    actio_registrare(reg, "spongia.ictus", pictor_spongia_ictus, ctx);
    actio_registrare(reg, "linea.ictus", pictor_linea_ictus, ctx);
    actio_registrare(reg, "strata.agere", pictor_strata_agere, ctx);
    actio_registrare(reg, "palette.aperire", pictor_palettam_aperire,
        ctx);
    actio_registrare(reg, "color_primus.ponere", pictor_colorem_ponere,
        ctx);
    actio_registrare(reg, "color_secundus.ponere",
        pictor_colorem_ponere,
        ctx);
    actio_registrare(reg, "exemplar.ponere", pictor_exemplar_ponere,
        ctx);
    actio_registrare(reg, "magnitudo.ponere",
        pictor_magnitudinem_ponere, ctx);
    actio_registrare(reg, "instrumentum.eligere",
                     pictor_instrumentum_eligere, ctx);
    actio_registrare(reg, "glutinatio.agere", pictor_glutinatio_agere,
        ctx);
    actio_registrare(reg, "imago.eligere", pictor_imaginem_eligere,
        ctx);
    actio_registrare(reg, "elenchus.agere", pictor_elenchum_agere, ctx);
    actio_registrare(reg, "pinacotheca.agere",
        pictor_pinacothecam_agere,
        ctx);
    actio_registrare(reg, "communia.agere", pictor_communia_agere, ctx);
    actio_registrare(reg, "spectator.agere", pictor_spectatorem_agere,
        ctx);
    actio_registrare(reg, "salutatio.agere", pictor_salutationem_agere,
        ctx);
    actio_registrare(reg, "inscriptio.agere",
        pictor_inscriptionem_agere,
        ctx);
}
