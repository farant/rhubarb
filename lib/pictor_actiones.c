/* pictor_actiones.c - tractatores pictoris */

#include "pictor_actiones.h"
#include "pictor_componentia.h"
#include "elenchus.h"
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

/* pinacotheca D6: spatium picturae novae "pictura-<n>", n = I +
 * maximum numerorum exstantium (nomen internum, numquam ostensum) */
interior vacuum
spatium_novum (
      Volumen* volumen,
      Piscina* piscina,
    character  exitus[XXXII])
{
       Xar* l;
    chorda  s;
       i32  i;
       i32  j;
       i32  n;
       i32  maximum;
       b32  numeri;

    maximum  = ZEPHYRUM;
    l        = pictor_documenta_enumerare(volumen, piscina);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        s = *(chorda*)xar_obtinere(l, i);
        si (   s.mensura <= VIII || s.mensura > VIII + IX
            || memcmp(s.datum, "pictura-", VIII) != ZEPHYRUM)
        {
            perge;
        }
        n       = ZEPHYRUM;
        numeri  = VERUM;
        per (j = VIII; j < s.mensura; j++)
        {
            si (s.datum[j] < '0' || s.datum[j] > '9')
            {
                numeri = FALSUM;
                frange;
            }
            n = n * X + (i32)(s.datum[j] - '0');
        }
        si (numeri && n > maximum)
        {
            maximum = n;
        }
    }
    sprintf(exitus, "pictura-%lu", (insignatus longus)(maximum + I));
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
            ElenchusRes  res[III];
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
    pictor_elenchum_parare(pa->imaginem_eligere != NIHIL, res, &e);
    k = elenchus_index(nodus);
    si (k < ZEPHYRUM || k >= (s32)e.numerus)
    {
        redde FALSUM;
    }
    titulus = e.res[k].titulus;
    si (strcmp(titulus, "pinacotheca.aperire") == ZEPHYRUM)
    {
        hic_manens character PINACOTHECA[] = "pinacotheca";

        redde palettam_scribere(repo, ctx, PINACOTHECA);
    }
    (vacuum)palettam_scribere(repo, ctx, NIHIL);
    si (strcmp(titulus, "pictura.nova") == ZEPHYRUM)
    {
        si (!pa->picturam_condere)
        {
            redde FALSUM;
        }
        spatium_novum(pa->doc->volumen, pa->doc->piscina, spatium);
        redde pa->picturam_condere(pa->montatio, spatium);
    }
    redde pa->imaginem_eligere
        && pa->imaginem_eligere(pa->imaginem_eligere_ctx);
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
}
