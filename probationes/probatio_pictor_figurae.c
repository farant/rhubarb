/* probatio_pictor_figurae.c - gradus VI (figurae pictoris -> mandata)
 * et VII (specimen pictor_prima): arbor vera, documentum verum */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "color.h"
#include "thema.h"
#include "volumen.h"
#include "fenestra.h"
#include "insula.h"
#include "motus.h"
#include "componens.h"
#include "mandatum.h"
#include "figura.h"
#include "delineare_mandata.h"
#include "specimen.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_figurae.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior vacuum
punctum_addere (
     Motus* motus,
    vacuum* ctx)
{
    Punctum* sedes;

    sedes   = (Punctum*)xar_addere(motus->ictus_pendens);
    *sedes  = *(Punctum*)ctx;
}

interior vacuum
pan_ponere (
     Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    motus->pan.x = -V;
    motus->pan.y = III;
}

/* fixura suitae: documentum, figurae et mandata semel picta;
 * sectiones LEGUNT */
nomen structura {
     PictorDocumentum* doc;
              Volumen* vol;
        PictorFigurae* pf;
              Mandata* m;
               chorda  sigillum;
} Mundus;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
                                 Piscina* piscina;
                     InternamentumChorda* intern;
                                 Volumen* vol;
                        PictorDocumentum* doc;
                      InsulaRepositorium* repo;
                                   Motus  motus;
                        PictorCompositio  cfg;
                           PictorFigurae* pf;
                         FiguraRegistrum* reg;
                               Componens* arbor;
                                 Mandata* m;
                                  Mundus* ctx;
                                 Punctum  p;

    piscina  = c->piscina;
    ctx      = (Mundus*)piscina_allocare(piscina, magnitudo(Mundus));
    memset(ctx, ZEPHYRUM, magnitudo(Mundus));
    c->datum  = ctx;
    intern    = internamentum_creare(piscina);
    thema_initiare();
    vol       = volumen_temporarium(piscina, "probatio_pictor_figurae");
    ctx->vol  = vol;
    doc = pictor_documentum_creare(piscina, intern, vol, "", XL, XXX,
        LXIV);
    /* actum: linea nigra horizontalis y=2, x 0..39 */
    pictor_documentum_actum(doc, chorda_ex_literis(
                "<ictus instrumentum=\"penicillus\" color=\"0\""
        " magnitudo=\"1\"><punctum x=\"0\" y=\"2\"/>"
        "<punctum x=\"39\" y=\"2\"/></ictus>",
        piscina));
    repo = insula_repositorium_creare(piscina, intern,
        "<pictor latitudo=\"40\" altitudo=\"30\""
        " instrumentum=\"penicillus\" zoom=\"1\"/>");
    motus_initiare(&motus, piscina);
    /* ictus pendens = captura tabulae (ut penicillus in pressione):
     * compositor puncta sola tabulae capturae ostendit (L4) */
    motus_captura_ponere(&motus, chorda_ex_literis("tabula", piscina));
    mutare_motum(&motus, pan_ponere, NIHIL, M);
    p.x = X;
    p.y = X;
    mutare_motum(&motus, punctum_addere, &p, M);
    p.x = XX;
    p.y = XV;
    mutare_motum(&motus, punctum_addere, &p, M);
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.fenestra_latitudo = LXIV;
    cfg.fenestra_altitudo = XLVIII;
    cfg.cellula_latitudo = VI;
    cfg.cellula_altitudo = VIII;
    cfg.status_lineae = I;
    arbor = pictor_componere(repo, &motus, piscina, intern, &cfg);
    CREDO_NON_NIHIL(arbor);

    /* gradus VI: figurae -> mandata (semel picta; sectiones legunt) */
    pf = (PictorFigurae*)piscina_allocare(piscina,
        magnitudo(PictorFigurae));
    pf->doc               = doc;
    pf->cellula_latitudo  = VI;
    pf->cellula_altitudo  = VIII;
    reg                   = figura_registrum_creare(piscina);
    pictor_figurae_registrare(reg, ZEPHYRUM, pf);
    m = mandata_creare(piscina, intern);
    pingere(arbor, reg, ZEPHYRUM, m);
    ctx->doc       = doc;
    ctx->pf        = pf;
    ctx->m         = m;
    ctx->sigillum  = pictor_documentum_sigillum_hex(doc, piscina);
}

interior vacuum
_suitam_purgare (
    CredoContextus* c)
{
    constans Mundus* ctx = (constans Mundus*)c->datum;

    si (ctx != NIHIL)
    {
        volumen_claudere(ctx->vol);
    }
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
     constans Mundus* ctx = (constans Mundus*)c->suita;
            Mandatum* x;
                 i32  i;
                 i32  n;
                 i32  imagines;
                 i32  lineae;
                 i32  accentus;
                 i32  textus;

    imagines  = ZEPHYRUM;
    lineae    = ZEPHYRUM;
    accentus  = ZEPHYRUM;
    textus    = ZEPHYRUM;
    n         = mandata_numerus(ctx->m);
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = mandata_obtinere(ctx->m, i);
        si (x->genus == MANDATUM_IMAGO)
        {
            imagines++;
            CREDO_VERUM(chorda_aequalis(x->textus, ctx->sigillum));
            CREDO_AEQUALIS_S32(x->fines.latitudo, XL);
        }
        /* ictus pendens (P4a): disci I colore vero (palette 0), non
         * linea accentus */
        si (   x->genus          == MANDATUM_RECTANGULUM
            && x->fines.latitudo == I && x->fines.altitudo == I
            && x->color.genus    == COLOR_MANDATI_RGBA
            && x->color.valor
               == color_ad_pixelum(color_ex_palette(ZEPHYRUM)))
        {
            lineae++;
        }
        si (   x->genus       == MANDATUM_LINEA
            && x->color.genus == COLOR_MANDATI_THEMA
            && x->color.valor == (i32)COLOR_ACCENT_PRIMARY)
        {
            accentus++;
        }
        si (x->genus == MANDATUM_TEXTUS)
        {
            textus++;
            CREDO_CHORDA_AEQUALIS_LITERIS(x->textus, "penicillus");
        }
    }
    CREDO_AEQUALIS_I32(imagines, I);
    /* (X, X) -> (XX, XV): punctum primum + X gradus lineae */
    CREDO_AEQUALIS_I32(lineae, XI);
    CREDO_AEQUALIS_I32(accentus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(textus, I);
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    constans Mundus* ctx = (constans Mundus*)c->suita;

    CREDO_VERUM(pictor_imago_fons(ctx->sigillum, ctx->pf)
                == pictor_documentum_proiectio(ctx->doc));
    CREDO_NIHIL(pictor_imago_fons(chorda_ex_literis("alienum",
        c->piscina), ctx->pf));
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
      constans Mundus* ctx = (constans Mundus*)c->suita;
      TabulaPixelorum* t;
                Imago  captura;
      SpecimenFructus  sf;
                  i32  niger;
                  i32  fundus;

    t = tabula_pixelorum_creare_nuda(c->piscina, LXIV, XLVIII);
    tabula_pixelorum_vacare(t,
        color_ad_pixelum(thema_color(COLOR_BACKGROUND)));
    delineare_mandata(ctx->m, t, pictor_imago_fons, ctx->pf);
    /* linea documenti y=2 sub pan (-5,3) et margine cellulae (VI,
     * VIII - tabula ad cellulam I, ut folium scribae): schirmo y=13,
     * documentum x 1..40 */
        niger = color_ad_pixelum(
            color_ex_palette(PALETTE_BLACK));
    /* vacatio: color fundi thematis (Franus 2026-10-09) */
    fundus = color_ad_pixelum(thema_color(COLOR_BACKGROUND));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XX, XIII),
                       niger);
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XX, XIV),
                       fundus);
    /* extra documentum dextrorsum (x=41): MENSA in cellula marginis */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XLI, XVIII),
        color_ad_pixelum(thema_color(COLOR_SUPERFICIES)));
    /* margo paginae: cellula TOTA extra documentum (schirmo documentum
     * x 1..40, y 11..40 -> margo x -5..46, y 3..48): dexter x=46 et
     * SUMMUS y=3 nunc videntur (olim summus extra prospectum - margo
     * nusquam in vico visus, Franus 2026-10-08) */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XLVI,
        XVIII),
        color_ad_pixelum(thema_color(COLOR_BORDER)));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XX, III),
        color_ad_pixelum(thema_color(COLOR_BORDER)));
    captura = imago_ex_tabula(t);
    sf = specimen_iudicare(&captura, "pictor_prima",
        specimen_regula_solita("probationes/pictor/specimina"),
        c->piscina);
    si (sf.sententia != SPECIMEN_CONGRUIT)
    {
        imprimere("SPECIMEN %s: %.*s\n",
                  specimen_sententia_nomen(sf.sententia),
                  (int)sf.causa.mensura, sf.causa.datum);
    }
    CREDO_VERUM(sf.sententia == SPECIMEN_CONGRUIT);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "Gradus VI: figurae -> mandata", _sectio_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "Fons imaginum: sigillum -> proiectio", _sectio_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Gradus VII: pixela, deinde specimen", _sectio_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("pictor_figurae", SECTIONES,
        _suitam_parare, _suitam_purgare);
}
