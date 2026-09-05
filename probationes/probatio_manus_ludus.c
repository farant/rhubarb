/* probatio_manus_ludus.c - manus in processu super applicationem
 * ludicram: selectores, ad_schirmum, premere, trahere, clavem,
 * exspectare, causa */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
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

    piscina = piscina_generare_dynamicum("probatio_manus_ludus",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum/>", "<ephemera/>");
    reg = actio_registrum_creare(piscina, intern);
    toy.compositiones = ZEPHYRUM;
    toy.derivata = ZEPHYRUM;
    toy.compositiones_in_traditione = ZEPHYRUM;
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere,
                           &toy, CCC);
    CREDO_NON_NIHIL(d);
    m = manus_ludus_creare(piscina, d);
    CREDO_NON_NIHIL(m);
    CREDO_VERUM(chorda_vacua(manus_ludus_causa(m)));

    imprimere("\n--- Selectores ---\n");
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

    imprimere("\n--- ad_schirmum ---\n");
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

    imprimere("\n--- premere ---\n");
    CREDO_VERUM(manus_ludus_premere(m, "[titulus=Numerare]"));
    a = insula_attributum(repo, INSULA_EPHEMERA, "numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "1");
    CREDO_MANUS_LUDUS_FOCUS(m, "b1");
    /* premere_ad in radicem: focus immotus (radix non focusabilis) */
    CREDO_VERUM(manus_ludus_premere_ad(m, V, XC));
    CREDO_MANUS_LUDUS_FOCUS(m, "b1");

    imprimere("\n--- movere: super derivatur ---\n");
    CREDO_VERUM(manus_ludus_movere(m, CXX, L));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_super(d), "tabula");

    imprimere("\n--- trahere in tabula ---\n");
    via[ZEPHYRUM].x  = V;
    via[ZEPHYRUM].y  = V;
    via[I].x         = X;
    via[I].y         = X;
    via[II].x        = XV;
    via[II].y        = XV;
    CREDO_VERUM(manus_ludus_trahere(m, "#tabula", via, III));
    a = insula_attributum(repo, INSULA_DURABILIS, "puncta");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "3");
    CREDO_MANUS_LUDUS_FOCUS(m, "tabula");
    CREDO_VERUM(chorda_vacua(dispensator_motus(d)->captura));
    CREDO_FALSUM(manus_ludus_trahere(m, "#tabula", via, ZEPHYRUM));

    imprimere("\n--- clavem ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    a = insula_attributum(repo, INSULA_EPHEMERA, "fuga");
    CREDO_NON_NIHIL(a);
    CREDO_VERUM(manus_ludus_clavem(m, '\t', ZEPHYRUM));
    CREDO_MANUS_LUDUS_FOCUS(m, "b1");
    /* shift-Tab retro */
    CREDO_VERUM(manus_ludus_clavem(m, '\t', I));
    CREDO_MANUS_LUDUS_FOCUS(m, "tabula");

    imprimere("\n--- exspectare movet tempus (quies) ---\n");
    dispensator_motus(d)->pan.x                      = III;
    dispensator_motus(d)->sordida                    = VERUM;
    dispensator_motus(d)->tempus_ultimae_mutationis  = m->tempus;
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA, "pan_x"));
    manus_ludus_exspectare(m, C);
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA, "pan_x"));
    manus_ludus_exspectare(m, D);
    a = insula_attributum(repo, INSULA_EPHEMERA, "pan_x");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "3");

    imprimere("\n--- selector malus nominat causam ---\n");
    CREDO_FALSUM(manus_ludus_premere(m, "#nemo"));
    CREDO_CHORDA_NON_VACUA(manus_ludus_causa(m));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
