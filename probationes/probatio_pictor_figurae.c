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

s32 principale (vacuum)
{
                                 Piscina* piscina;
                     InternamentumChorda* intern;
                                 Volumen* vol;
                        PictorDocumentum* doc;
                      InsulaRepositorium* repo;
                                   Motus  motus;
                        PictorCompositio  cfg;
                           PictorFigurae  pf;
                         FiguraRegistrum* reg;
                               Componens* arbor;
                                 Mandata* m;
                                Mandatum* x;
                         TabulaPixelorum* t;
                                   Imago  captura;
                         SpecimenFructus  sf;
                                 Punctum  p;
                                     i32  i;
                                     i32  n;
                                     i32  imagines;
                                     i32  lineae;
                                     i32  accentus;
                                     i32  textus;
                                  chorda  sigillum;
                                     i32  niger;
                                     i32  fundus;

    piscina = piscina_generare_dynamicum("probatio_pictor_figurae",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_figurae");
    doc = pictor_documentum_creare(piscina, intern, vol, "", XL, XXX,
        LXIV);
    /* actum: linea nigra horizontalis y=2, x 0..39 */
    pictor_documentum_actum(doc, chorda_ex_literis(
                "<ictus instrumentum=\"penicillus\" color=\"0\""
        " magnitudo=\"1\"><punctum x=\"0\" y=\"2\"/>"
        "<punctum x=\"39\" y=\"2\"/></ictus>",
        piscina));
    repo = insula_repositorium_creare(piscina, intern,
        "<pictor latitudo=\"40\" altitudo=\"30\"/>",
        "<pictor instrumentum=\"penicillus\" zoom=\"1\"/>");
    motus_initiare(&motus, piscina);
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

    imprimere("\n--- Gradus VI: figurae -> mandata ---\n");
    pf.doc               = doc;
    pf.cellula_latitudo  = VI;
    pf.cellula_altitudo  = VIII;
    reg                  = figura_registrum_creare(piscina);
    pictor_figurae_registrare(reg, ZEPHYRUM, &pf);
    m = mandata_creare(piscina, intern);
    pingere(arbor, reg, ZEPHYRUM, m);
    sigillum  = pictor_documentum_sigillum_hex(doc, piscina);
    imagines  = ZEPHYRUM;
    lineae    = ZEPHYRUM;
    accentus  = ZEPHYRUM;
    textus    = ZEPHYRUM;
    n         = mandata_numerus(m);
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = mandata_obtinere(m, i);
        si (x->genus == MANDATUM_IMAGO)
        {
            imagines++;
            CREDO_VERUM(chorda_aequalis(x->textus, sigillum));
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

    imprimere("\n--- Fons imaginum: sigillum -> proiectio ---\n");
    CREDO_VERUM(pictor_imago_fons(sigillum, &pf)
                == pictor_documentum_proiectio(doc));
    CREDO_NIHIL(pictor_imago_fons(chorda_ex_literis("alienum", piscina),
        &pf));

    imprimere("\n--- Gradus VII: pixela, deinde specimen ---\n");
    t = tabula_pixelorum_creare_nuda(piscina, LXIV, XLVIII);
    tabula_pixelorum_vacare(t,
        color_ad_pixelum(thema_color(COLOR_BACKGROUND)));
    delineare_mandata(m, t, pictor_imago_fons, &pf);
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
        piscina);
    si (sf.sententia != SPECIMEN_CONGRUIT)
    {
        imprimere("SPECIMEN %s: %.*s\n",
                  specimen_sententia_nomen(sf.sententia),
                  (int)sf.causa.mensura, sf.causa.datum);
    }
    CREDO_VERUM(sf.sententia == SPECIMEN_CONGRUIT);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
