/* probatio_manus_ludus.c - manus in processu super applicationem
 * ludicram: selectores, ad_schirmum, premere, trahere, clavem,
 * exspectare, causa */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "xar.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* scenarium unum: gradus mundum eundem (repositorium, dispensator,
 * manus) ordine mutant - focus, puncta, tempus a gradu priore pendent -
 * ergo una sectio cum gradibus nominatis */
interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
              ToyStatus  toy;
            Dispensator* d;
             ManusLudus* m;
                 chorda* a;
                Punctum  via[III];
                Punctum  p;
              Componens* pannus;
              Componens* filius;
                  Fines  f;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern,
        "<insula/>");
    reg = actio_registrum_creare(piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere,
                           &toy, CCC);
    CREDO_NON_NIHIL(d);
    m = manus_ludus_creare(piscina, d);
    CREDO_NON_NIHIL(m);
    CREDO_VERUM(chorda_vacua(manus_ludus_causa(m)));

    CREDO_GRADUS("Selectores");
    CREDO_MANUS_LUDUS_EXISTIT(m, "#b1");
    CREDO_MANUS_LUDUS_EXISTIT(m, "#radix");
    CREDO_MANUS_LUDUS_EXISTIT(m, "[partes=tabula]");
    CREDO_MANUS_LUDUS_EXISTIT(m, "[actio=numerare]");
    CREDO_MANUS_LUDUS_EXISTIT(m, "[titulus=Numerare]");
    CREDO_MANUS_LUDUS_ABEST(m, "#nemo");
    CREDO_MANUS_LUDUS_ABEST(m, "[partes=dialogus]");
    CREDO_MANUS_LUDUS_ABEST(m, "[actio=ignota]");
    CREDO_VERUM(manus_ludus_invenire(m, "[actio=numerare]")
                == manus_ludus_invenire(m, "#b1"));
    /* selectores mali: causa nominatur */
    CREDO_MANUS_LUDUS_ABEST(m, "b1");
    CREDO_CHORDA_CONTINET(manus_ludus_causa(m),
                          chorda_ex_literis("selector", piscina));
    CREDO_MANUS_LUDUS_ABEST(m, "[partes=tabula");
    CREDO_MANUS_LUDUS_ABEST(m, "[color=ruber]");

    CREDO_GRADUS("ad_schirmum");
    p.x = ZEPHYRUM;
    p.y = ZEPHYRUM;
    p = manus_ludus_ad_schirmum(m, manus_ludus_invenire(m, "#tabula"),
        p);
    CREDO_AEQUALIS_S32(p.x, LXX);
    CREDO_AEQUALIS_S32(p.y, X);
    p.x = V;
    p.y = VI;
    p = manus_ludus_ad_schirmum(m, manus_ludus_invenire(m, "#b1"), p);
    CREDO_AEQUALIS_S32(p.x, XV);
    CREDO_AEQUALIS_S32(p.y, XVI);
    /* transformatio nidificata sine dispensatore: pannus (70,0)
     * translatio (0,50) scala II; filius (5,5) -> schirmo (80,60) */
    pannus = componens_creare(piscina, intern, "pannus", PARTES_NULLUM);
    f.x = LXX;
    f.y = ZEPHYRUM;
    f.latitudo = C;
    f.altitudo = C;
    componens_ponere_fines(pannus, f);
    componens_ponere_transformatio(pannus, ZEPHYRUM, L, II);
    filius = componens_creare(piscina, intern, "filius", PARTES_NULLUM);
    f.x = V;
    f.y = V;
    f.latitudo = X;
    f.altitudo = X;
    componens_ponere_fines(filius, f);
    componens_addere_liberum(pannus, filius);
    p.x  = ZEPHYRUM;
    p.y  = ZEPHYRUM;
    p    = manus_ludus_ad_schirmum(m, filius, p);
    CREDO_AEQUALIS_S32(p.x, LXXX);
    CREDO_AEQUALIS_S32(p.y, LX);

    CREDO_GRADUS("premere");
    CREDO_VERUM(manus_ludus_premere(m, "[titulus=Numerare]"));
    a = insula_attributum(repo, "numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "1");
    CREDO_MANUS_LUDUS_FOCUS(m, "b1");
    /* premere_ad in radicem: focus immotus (radix non focusabilis) */
    CREDO_VERUM(manus_ludus_premere_ad(m, V, XC));
    CREDO_MANUS_LUDUS_FOCUS(m, "b1");

    CREDO_GRADUS("movere: super derivatur");
    CREDO_VERUM(manus_ludus_movere(m, CXX, L));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_super(d), "tabula");

    CREDO_GRADUS("trahere in tabula");
    via[ZEPHYRUM].x  = V;
    via[ZEPHYRUM].y  = V;
    via[I].x         = X;
    via[I].y         = X;
    via[II].x        = XV;
    via[II].y        = XV;
    CREDO_VERUM(manus_ludus_trahere(m, "#tabula", via, III));
    a = insula_attributum(repo, "puncta");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "3");
    CREDO_MANUS_LUDUS_FOCUS(m, "tabula");
    CREDO_VERUM(chorda_vacua(dispensator_motus(d)->captura));
    CREDO_FALSUM(manus_ludus_trahere(m, "#tabula", via, ZEPHYRUM));

    CREDO_GRADUS("clavem");
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    a = insula_attributum(repo, "fuga");
    CREDO_NON_NIHIL(a);
    CREDO_VERUM(manus_ludus_clavem(m, '\t', ZEPHYRUM));
    CREDO_MANUS_LUDUS_FOCUS(m, "b1");
    /* shift-Tab retro - vexilla vera macOS (A5: olim 'I', qui
     * bitus lateris Ctrl est) */
    CREDO_VERUM(manus_ludus_clavem(m, '\t',
        MOD_SHIFT | MOD_SHIFT_SINISTER));
    CREDO_MANUS_LUDUS_FOCUS(m, "tabula");

    CREDO_GRADUS("scribere: clavis + textus eodem tempore");
    {
                     Xar* notata;
        constans Eventus* e0;
        constans Eventus* e1;

        notata = xar_creare(piscina, (i32)magnitudo(EventusNotatum));
        dispensator_notarium_ponere(d, notata);
        CREDO_VERUM(manus_ludus_scribere(m, "a\r"));
        dispensator_notarium_ponere(d, NIHIL);
        /* 'a': DEPRESSUS, TEXTUS, LIBERATUS;
         * '\r': DEPRESSUS, LIBERATUS (regimen textum non fert) */
        CREDO_AEQUALIS_I32(xar_numerus(notata), V);
        e0 = &((EventusNotatum*)xar_obtinere(notata,
            ZEPHYRUM))->eventus;
        e1 = &((EventusNotatum*)xar_obtinere(notata, I))->eventus;
        CREDO_AEQUALIS_I32((i32)e0->genus,
            (i32)EVENTUS_CLAVIS_DEPRESSUS);
        CREDO_AEQUALIS_I32((i32)e1->genus, (i32)EVENTUS_TEXTUS);
        CREDO_AEQUALIS_S64(e1->tempus, e0->tempus);
        CREDO_AEQUALIS_I32(e1->datum.textus.contentum.mensura, I);
        CREDO_VERUM(e1->datum.textus.contentum.datum[ZEPHYRUM] == 'a');
        CREDO_AEQUALIS_I32((i32)((EventusNotatum*)xar_obtinere(notata,
            III))->eventus.genus, (i32)EVENTUS_CLAVIS_DEPRESSUS);
    }

    CREDO_GRADUS("exspectare movet tempus (quies)");
    dispensator_motus(d)->pan.x                      = III;
    dispensator_motus(d)->sordida                    = VERUM;
    dispensator_motus(d)->tempus_ultimae_mutationis  = m->tempus;
    CREDO_NIHIL(insula_attributum(repo, "pan_x"));
    manus_ludus_exspectare(m, C);
    CREDO_NIHIL(insula_attributum(repo, "pan_x"));
    manus_ludus_exspectare(m, D);
    a = insula_attributum(repo, "pan_x");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "3");

    CREDO_GRADUS("botton ut fons verus (insula-rami T4)");
    /* ictus et tractus bottonem sinistrum ferunt, motus nudus nullum -
     * codificator terminalis pressionem sine bottone abicit */
    {
         Xar* notata;
         i32  i;

        notata = xar_creare(piscina, (i32)magnitudo(EventusNotatum));
        dispensator_notarium_ponere(d, notata);
        CREDO_VERUM(manus_ludus_premere_ad(m, V, XC));
        CREDO_VERUM(manus_ludus_movere(m, CXX, L));
        CREDO_VERUM(manus_ludus_trahere(m, "#tabula", via, III));
        dispensator_notarium_ponere(d, NIHIL);
        CREDO_AEQUALIS_I32(xar_numerus(notata), VII);
        per (i = ZEPHYRUM; i < xar_numerus(notata); i++)
        {
            constans Eventus* e;

            e = &((EventusNotatum*)xar_obtinere(notata, i))->eventus;
            CREDO_AEQUALIS_I32((i32)e->datum.mus.botton,
                (i32)(i == II ? ZEPHYRUM : MUS_SINISTER));
        }
    }

    CREDO_GRADUS("selector malus nominat causam");
    CREDO_FALSUM(manus_ludus_premere(m, "#nemo"));
    CREDO_CHORDA_NON_VACUA(manus_ludus_causa(m));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "manus super applicationem ludicram (scenarium)", _sectio_prima,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("manus_ludus", SECTIONES);
}
