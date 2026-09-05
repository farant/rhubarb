/* probatio_pictor_actiones.c - gradus II: ictus scriptus UNUM <ictus>
 * cum n punctis parit, insula ephemera intacta (praeter focum);
 * Escape medio ictu nihil parit; 'p' instrumentum eligit */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "motus.h"
#include "actio.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_actiones.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

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

interior i32
numerare_puncta (
                 chorda  actum,
                Piscina* p,
    InternamentumChorda* in)
{
    StmlResultus res;

    res = stml_legere_ex_literis(chorda_ut_cstr(actum, p), p, in);
    si (!res.successus || !res.elementum_radix)
    {
        redde ZEPHYRUM;
    }
    redde stml_numerus_liberorum(res.elementum_radix);
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
       PictorDocumentum* doc;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
         PictorActiones  actiones;
       PictorCompositio  cfg;
            Dispensator* d;
             ManusLudus* m;
                Punctum  via[III];
                    Xar* acta;
           VolumenActum* a;
                    i32  versio_ante;
                 chorda  causa;
                 chorda  fons;
                  Canon* ephemera;
              character  nihil_instr[] = "nihil";

    piscina = piscina_generare_dynamicum("probatio_pictor_actiones",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_actiones");
    doc = pictor_documentum_creare(piscina, intern, vol, CCCXX, CC,
        LXIV);
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum latitudo=\"320\" altitudo=\"200\"/>",
        "<ephemera instrumentum=\"penicillus\" color_primus=\"0\""
        " magnitudo=\"1\" zoom=\"1\"/>");
    fons = filum_legere_totum("apps/pictor/canones/ephemera.canon",
        piscina);
    ephemera = canon_legere(fons, piscina, intern, &causa);
    CREDO_NON_NIHIL(ephemera);
    insula_ponere_canonem(repo, INSULA_EPHEMERA, ephemera);
    {
              chorda domini;
        StmlResultus res;
        domini = filum_legere_totum("apps/pictor/canones/domini.stml",
                                    piscina);
        res = stml_legere_ex_literis(chorda_ut_cstr(domini, piscina),
                                     piscina, intern);
        CREDO_VERUM(res.successus);
        insula_dominos_legere(repo, INSULA_EPHEMERA,
            res.elementum_radix);
    }
    reg           = actio_registrum_creare(piscina, intern);
    actiones.doc  = doc;
    pictor_actiones_registrare(reg, &actiones);
    cfg.fenestra_latitudo  = CCCXX;
    cfg.fenestra_altitudo  = CCXII;
    cfg.status_altitudo    = XII;
    d = dispensator_creare(piscina, intern, repo, reg, pictor_componere,
                           &cfg, CCC);
    CREDO_NON_NIHIL(d);
    m = manus_ludus_creare(piscina, d);
    CREDO_MANUS_LUDUS_EXISTIT(m, "#tabula");
    CREDO_MANUS_LUDUS_EXISTIT(m, "[actio=penicillus.ictus]");
    CREDO_AEQUALIS_I32(xar_numerus(actio_non_registratae(reg,
        dispensator_arbor(d), piscina)), ZEPHYRUM);          /* L10 */

    imprimere("\n--- Ictus scriptus: UNUM <ictus>, III puncta ---\n");
    versio_ante      = insula_versio(repo, INSULA_EPHEMERA);
    via[ZEPHYRUM].x  = X;
    via[ZEPHYRUM].y  = XX;
    via[I].x         = XV;
    via[I].y         = XXV;
    via[II].x        = XX;
    via[II].y        = XXX;
    CREDO_VERUM(manus_ludus_trahere(m, "#tabula", via, III));
    acta = volumen_acta_legere(vol, ZEPHYRUM, piscina);
    {
        i32 i;
        i32 n;
        i32 ictus;
        ictus  = ZEPHYRUM;
        n      = xar_numerus(acta);
        per (i = ZEPHYRUM; i < n; i++)
        {
            a = (VolumenActum*)xar_obtinere(acta, i);
            si (chorda_aequalis_literis(a->genus, "ictus"))
            {
                ictus++;
                CREDO_AEQUALIS_I32(numerare_puncta(a->datum, piscina,
                    intern),
                                   III);
                CREDO_CHORDA_CONTINET(a->datum,
                    chorda_ex_literis("instrumentum=\"penicillus\"",
                    piscina));
            }
        }
        CREDO_AEQUALIS_I32(ictus, I);
    }
        CREDO_VERUM(pictor_documentum_cursor(doc) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(doc->numerus_vivorum, I);
    /* motus purgatus, captura soluta, nihil sordidum */
    CREDO_AEQUALIS_I32(xar_numerus(dispensator_motus(d)->ictus_pendens),
                       ZEPHYRUM);
    CREDO_VERUM(chorda_vacua(dispensator_motus(d)->captura));
    CREDO_FALSUM(dispensator_motus(d)->sordida);
    /* ephemera: focus solus scriptus (versio + I), pan absens */
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), versio_ante
        + I);
    CREDO_MANUS_LUDUS_FOCUS(m, "tabula");
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA, "pan_x"));
    CREDO_VERUM(insula_restituere(repo));
    /* proiectio: pixelum sub (15,25) nigrum */
    {
        constans Imago* im;
           constans i8* px;
                 Color  niger_c;
                   i32  niger_r;
        im = pictor_documentum_proiectio(doc);
        px = im->pixela + (XXV * im->latitudo + XV) * IV;
        niger_c = thema_color_ex_indice_colorationis((i8)PALETTE_BLACK);
        niger_r = (i32)color_obtinere_r(niger_c);
        CREDO_AEQUALIS_I32((i32)px[ZEPHYRUM], niger_r);
        CREDO_AEQUALIS_I32((i32)px[III], CCLV);
    }

    imprimere("\n--- Escape medio ictu: nihil paritur ---\n");
    /* premere in tabula = ictus puncti unius */
    manus_ludus_premere_ad(m, XL, XL);
        CREDO_AEQUALIS_I32(doc->numerus_vivorum, II);
    {
        Eventus e;
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus        = EVENTUS_MUS_DEPRESSUS;
        e.tempus       = m->tempus;
        e.datum.mus.x  = L;
        e.datum.mus.y  = L;
        dispensator_tractare(d, &e);
        CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_motus(d)->captura,
            "tabula");
        e.genus        = EVENTUS_MUS_MOTUS;
        e.datum.mus.x  = LV;
        dispensator_tractare(d, &e);
        CREDO_AEQUALIS_I32(
            xar_numerus(dispensator_motus(d)->ictus_pendens), II);
    }
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(chorda_vacua(dispensator_motus(d)->captura));
    CREDO_AEQUALIS_I32(xar_numerus(dispensator_motus(d)->ictus_pendens),
                       ZEPHYRUM);
    /* cursor immotus */
        CREDO_AEQUALIS_I32(doc->numerus_vivorum, II);

    imprimere("\n--- 'p' instrumentum eligit (dominus = actio) ---\n");
    /* instrumentum aliud sub domino suo, deinde 'p' per manum */
    insula_scriptorem_ponere(repo,
        chorda_ex_literis("instrumentum.eligere", piscina));
    CREDO_VERUM(mutare_ephemera(repo, instrumentum_ponere,
        nihil_instr));
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));
    dispensator_recomponere(d);
    CREDO_MANUS_LUDUS_ABEST(m, "[actio=penicillus.ictus]");
    CREDO_VERUM(manus_ludus_clavem(m, 'p', ZEPHYRUM));
    {
        chorda* a2;
        a2 = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
        CREDO_NON_NIHIL(a2);
        CREDO_CHORDA_AEQUALIS_LITERIS(*a2, "penicillus");
    }
    CREDO_MANUS_LUDUS_EXISTIT(m, "[actio=penicillus.ictus]");
    CREDO_VERUM(insula_restituere(repo));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
