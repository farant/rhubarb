/* probatio_scriba_liber.c - liber paginarum scribae (vicus-latera S2b)
 *
 * Volumen novum: pagina "1" sola, index scriptus. Idem nomen = idem
 * documentum (visus omnes unum tenent). Nomen ignotum: NIHIL, index
 * -1. Pagina nova numero proximo, ordo et textus per reaperturam
 * servantur; nomen novum numquam iteratur. Sine limite (decisio XIV):
 * CXX paginae novae. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "volumen.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "scriba_liber.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior chorda
lit (
    constans character* s)
{
    redde chorda_ex_literis(s, piscina);
}

s32 principale (vacuum)
{
              Volumen* vol;
          ScribaLiber* l;
     ScribaDocumentum* d;
     ScribaDocumentum* d2;
    TabulaCharacterum  folium;
               chorda  index;
               chorda  n;
                  b32  inventum;
                  i32  k;

    piscina = piscina_generare_dynamicum("probatio_scriba_liber",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern  = internamentum_creare(piscina);
    vol     = volumen_temporarium(piscina, "probatio_scriba_liber");

    imprimere("\n--- I: volumen novum - pagina '1' ---\n");
    l = scriba_liber_aperire(piscina, intern, vol, XL, XX);
    CREDO_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(scriba_liber_numerus(l), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(scriba_liber_nomen(l, ZEPHYRUM), "1");
    index = volumen_plagulam_promere(vol, lit("scriba/paginae"),
        piscina,
        &inventum);
    CREDO_VERUM(inventum);
    CREDO_CHORDA_CONTINET(index, lit("<pagina nomen=\"1\"/>"));

    imprimere("\n--- II: idem nomen = idem documentum ---\n");
    d = scriba_liber_pagina(l, lit("1"));
    CREDO_NON_NIHIL(d);
    d2 = scriba_liber_pagina(l, lit("1"));
    CREDO_VERUM(d == d2);
    CREDO_AEQUALIS_I32(d ? d->latitudo : ZEPHYRUM, XL);
    CREDO_AEQUALIS_I32(d ? d->altitudo : ZEPHYRUM, XX);

    imprimere("\n--- III: nomen ignotum ---\n");
    CREDO_NIHIL(scriba_liber_pagina(l, lit("nusquam")));
    CREDO_AEQUALIS_S32(scriba_liber_index(l, lit("nusquam")), -I);
    CREDO_AEQUALIS_S32(scriba_liber_index(l, lit("1")), ZEPHYRUM);

    imprimere("\n--- IV: pagina nova, reapertio ---\n");
    n = scriba_liber_pagina_nova(l);
    CREDO_CHORDA_AEQUALIS_LITERIS(n, "2");
    CREDO_AEQUALIS_I32(scriba_liber_numerus(l), II);
    /* textus in pagina '1' committitur */
    si (d)
    {
        tabula_initiare(&folium, piscina, d->latitudo, d->altitudo);
        memcpy(folium.cellulae, scriba_documentum_tabula(d)->cellulae,
            (memoriae_index)(d->latitudo * d->altitudo));
        tabula_cellula(&folium, ZEPHYRUM, ZEPHYRUM) = 'q';
        CREDO_VERUM(scriba_documentum_committere(d, &folium)
            > ZEPHYRUM);
    }
    l = scriba_liber_aperire(piscina, intern, vol, XL, XX);
    CREDO_AEQUALIS_I32(scriba_liber_numerus(l), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(scriba_liber_nomen(l, I), "2");
    d = scriba_liber_pagina(l, lit("1"));
    CREDO_NON_NIHIL(d);
    CREDO_VERUM(d
        && tabula_cellula(scriba_documentum_tabula(d), ZEPHYRUM,
        ZEPHYRUM) == 'q');

    imprimere("\n--- V: nomen novum numquam iteratur ---\n");
    CREDO_VERUM(volumen_plagulam_condere(vol, lit("scriba/paginae"),
        lit(
        "<paginae><pagina nomen=\"1\"/>"
        "<pagina nomen=\"3\"/></paginae>"),
        "probatio"));
    l = scriba_liber_aperire(piscina, intern, vol, XL, XX);
    CREDO_AEQUALIS_I32(scriba_liber_numerus(l), II);
    /* numerus + I = 3 iam adest: proximus */
    CREDO_CHORDA_AEQUALIS_LITERIS(scriba_liber_pagina_nova(l), "4");

    imprimere("\n--- VI: sine limite (decisio XIV) ---\n");
    per (k = ZEPHYRUM; k < CXX; k++)
    {
        (vacuum)scriba_liber_pagina_nova(l);
    }
    CREDO_AEQUALIS_I32(scriba_liber_numerus(l), CXXIII);
    CREDO_NON_NIHIL(scriba_liber_pagina(l, scriba_liber_nomen(l,
        CXXII)));

    imprimere("\n--- VII: pagina nominata (S3c) ---\n");
    {
        ScribaDocumentum* d;

        d = scriba_liber_paginam_condere(l, lit("notae"));
        CREDO_NON_NIHIL(d);
        CREDO_AEQUALIS_I32(scriba_liber_numerus(l), CXXIV);
        CREDO_AEQUALIS_S32(scriba_liber_index(l, lit("notae")), CXXIII);
        /* iterum: eadem, nulla nova */
        CREDO_VERUM(scriba_liber_paginam_condere(l, lit("notae")) == d);
        CREDO_VERUM(scriba_liber_pagina(l, lit("notae")) == d);
        CREDO_AEQUALIS_I32(scriba_liber_numerus(l), CXXIV);
        /* exstans numerata */
        CREDO_VERUM(scriba_liber_paginam_condere(l, lit("1"))
            == scriba_liber_pagina(l, lit("1")));
        CREDO_AEQUALIS_I32(scriba_liber_numerus(l), CXXIV);
        /* invalida */
        CREDO_NIHIL(scriba_liber_paginam_condere(l, lit("")));
        CREDO_NIHIL(scriba_liber_paginam_condere(l, lit("Notae")));
        CREDO_NIHIL(scriba_liber_paginam_condere(l, lit("a b")));
        CREDO_NIHIL(scriba_liber_paginam_condere(l, lit("a/b")));
        CREDO_NIHIL(scriba_liber_paginam_condere(NIHIL, lit("x")));
        CREDO_AEQUALIS_I32(scriba_liber_numerus(l), CXXIV);
        /* reapertio: index servatus */
        l = scriba_liber_aperire(piscina, intern, vol, XL, XX);
        CREDO_AEQUALIS_S32(scriba_liber_index(l, lit("notae")), CXXIII);
        CREDO_NON_NIHIL(scriba_liber_paginam_condere(l, lit("x-1_b")));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
