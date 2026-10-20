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
#include "color.h"
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
#include "pictor_organum_probationis.h"
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

interior vacuum
_sectio_prima (
    CredoContextus* c)
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
                  Canon* ephemera;
              character  nihil_instr[] = "nihil";

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_actiones");
    doc = pictor_documentum_creare(piscina, intern, vol, "", CCCXX, CC,
        LXIV);
    repo = insula_repositorium_creare(piscina, intern,
        "<pictor latitudo=\"320\" altitudo=\"200\""
        " instrumentum=\"penicillus\" color_primus=\"0\""
        " magnitudo_penicilli=\"1\" zoom=\"1\"/>");
    {
        StmlNodus* domini;

        CREDO_VERUM(pictor_organum_parare(piscina, intern, &ephemera,
            &domini));
        CREDO_NON_NIHIL(ephemera);
        insula_ponere_canonem(repo, ephemera);
        insula_dominos_legere(repo, domini);
    }
    reg           = actio_registrum_creare(piscina, intern);
    memset(&actiones, ZEPHYRUM, magnitudo(actiones));
    actiones.doc  = doc;
    pictor_actiones_registrare(reg, &actiones);
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.fenestra_latitudo  = CCCXX;
    cfg.fenestra_altitudo  = CCXII;
    cfg.cellula_latitudo   = VI;
    cfg.cellula_altitudo   = VIII;
    cfg.status_lineae      = I;
    d = dispensator_creare(piscina, intern, repo, reg, pictor_componere,
                           &cfg, CCC);
    CREDO_NON_NIHIL(d);
    m = manus_ludus_creare(piscina, d);
    CREDO_MANUS_LUDUS_EXISTIT(m, "#tabula");
    CREDO_MANUS_LUDUS_EXISTIT(m, "[actio=penicillus.ictus]");
    CREDO_AEQUALIS_I32(xar_numerus(actio_non_registratae(reg,
        dispensator_arbor(d), piscina)), ZEPHYRUM);          /* L10 */

    versio_ante      = insula_versio(repo);
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
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(doc), I);
    /* motus purgatus, captura soluta, nihil sordidum */
    CREDO_AEQUALIS_I32(xar_numerus(dispensator_motus(d)->ictus_pendens),
                       ZEPHYRUM);
    CREDO_VERUM(chorda_vacua(dispensator_motus(d)->captura));
    CREDO_FALSUM(dispensator_motus(d)->sordida);
    /* ephemera: focus solus scriptus (versio + I), pan absens */
    CREDO_AEQUALIS_I32(insula_versio(repo), versio_ante
        + I);
    CREDO_MANUS_LUDUS_FOCUS(m, "tabula");
    CREDO_NIHIL(insula_attributum(repo, "pan_x"));
    CREDO_VERUM(insula_restituere(repo));
    /* proiectio: pixelum sub (15,25) nigrum */
    {
        constans Imago* im;
           constans i8* px;
                 Color  niger_c;
                   i32  niger_r;
        im       = pictor_documentum_proiectio(doc);
        px       = im->pixela + (XXV * im->latitudo + XV) * IV;
        niger_c  = color_ex_palette(PALETTE_BLACK);
        niger_r  = (i32)color_obtinere_r(niger_c);
        CREDO_AEQUALIS_I32((i32)px[ZEPHYRUM], niger_r);
        CREDO_AEQUALIS_I32((i32)px[III], CCLV);
    }

    CREDO_GRADUS("Escape medio ictu: nihil paritur");
    /* premere in tabula = ictus puncti unius */
    manus_ludus_premere_ad(m, XL, XL);
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(doc), II);
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
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(doc), II);

    CREDO_GRADUS("'p' instrumentum eligit (dominus = actio)");
    /* instrumentum aliud sub domino suo, deinde 'p' per manum */
    insula_scriptorem_ponere(repo,
        chorda_ex_literis("instrumentum.eligere", piscina));
    CREDO_VERUM(insula_mutare(repo, instrumentum_ponere,
        nihil_instr));
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));
    dispensator_recomponere(d);
    CREDO_MANUS_LUDUS_ABEST(m, "[actio=penicillus.ictus]");
    /* A5: Ctrl+P et Cmd+P (imprimere!) brevitates sunt, non 'p' */
    CREDO_VERUM(manus_ludus_clavem(m, 'p',
        MOD_IMPERIUM | MOD_IMPERIUM_SINISTER));
    CREDO_MANUS_LUDUS_ABEST(m, "[actio=penicillus.ictus]");
    CREDO_VERUM(manus_ludus_clavem(m, 'p',
        MOD_SUPER | MOD_SUPER_SINISTER));
    CREDO_MANUS_LUDUS_ABEST(m, "[actio=penicillus.ictus]");
    CREDO_VERUM(manus_ludus_clavem(m, 'p', ZEPHYRUM));
    {
        chorda* a2;
        a2 = insula_attributum(repo, "instrumentum");
        CREDO_NON_NIHIL(a2);
        CREDO_CHORDA_AEQUALIS_LITERIS(*a2, "penicillus");
    }
    CREDO_MANUS_LUDUS_EXISTIT(m, "[actio=penicillus.ictus]");
    CREDO_VERUM(insula_restituere(repo));

    CREDO_GRADUS("Captura: punctum in spatio TABULAE");
    /* Franus 2026-10-08: tractus extra tabulam (super lineam status aut
     * latus alterum) ad marginem oppositum saliebat - punctum_locale
     * ad ictum GEOMETRICUM pertinet (linea status: y ~ 3), non ad
     * componentem captum */
    {
           Xar* omnia;
           i32  k;
        chorda  ultimum;

        via[ZEPHYRUM].x  = X;
        via[ZEPHYRUM].y  = CXC;
        via[I].x         = X;
        via[I].y         = CXCIX;
        via[II].x        = X;
        via[II].y        = CCVII;
        CREDO_VERUM(manus_ludus_trahere(m, "#tabula", via, III));
        omnia    = volumen_acta_legere(vol, ZEPHYRUM, piscina);
        ultimum  = chorda_ex_literis("", piscina);
        per (k = ZEPHYRUM; k < xar_numerus(omnia); k++)
        {
            a = (VolumenActum*)xar_obtinere(omnia, k);
            si (chorda_aequalis_literis(a->genus, "ictus"))
            {
                ultimum = a->datum;
            }
        }
        /* punctum ultimum in spatio tabulae (CCVII), non status */
        CREDO_CHORDA_CONTINET(ultimum, chorda_ex_literis("y=\"207\"",
            piscina));
    }

    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "Ictus scriptus: UNUM <ictus>, III puncta", _sectio_prima, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_actiones", SECTIONES);
}
