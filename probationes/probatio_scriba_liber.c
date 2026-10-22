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

/* piscina et intern plagulae: sectionis propria (parare ponit) */
interior vacuum
_parare (
    CredoContextus* c)
{
    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
}

/* SECTIO UNA (credo II): scaena gradibus I-IX nominatis
 * (CREDO_GRADUS) - volumen et liber per gradus crescunt, ergo gradus
 * sectiones liberae esse non possunt */
interior vacuum
_sectio_prima (
    CredoContextus* c)
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

    (vacuum)c;
    vol = volumen_temporarium(piscina, "probatio_scriba_liber");

    CREDO_GRADUS("I: volumen novum - pagina '1'");
    l = scriba_liber_aperire(piscina, intern, vol, XL, XX);
    CREDO_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(scriba_liber_numerus(l), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(scriba_liber_nomen(l, ZEPHYRUM), "1");
    index = volumen_plagulam_promere(vol, lit("scriba/paginae"),
        piscina,
        &inventum);
    CREDO_VERUM(inventum);
    CREDO_CHORDA_CONTINET(index, lit("<pagina nomen=\"1\"/>"));

    CREDO_GRADUS("II: idem nomen = idem documentum");
    d = scriba_liber_pagina(l, lit("1"));
    CREDO_NON_NIHIL(d);
    d2 = scriba_liber_pagina(l, lit("1"));
    CREDO_VERUM(d == d2);
    CREDO_AEQUALIS_I32(d ? d->latitudo : ZEPHYRUM, XL);
    CREDO_AEQUALIS_I32(d ? d->altitudo : ZEPHYRUM, XX);

    CREDO_GRADUS("III: nomen ignotum");
    CREDO_NIHIL(scriba_liber_pagina(l, lit("nusquam")));
    CREDO_AEQUALIS_S32(scriba_liber_index(l, lit("nusquam")), -I);
    CREDO_AEQUALIS_S32(scriba_liber_index(l, lit("1")), ZEPHYRUM);

    CREDO_GRADUS("IV: pagina nova, reapertio");
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

    CREDO_GRADUS("V: nomen novum numquam iteratur");
    CREDO_VERUM(volumen_plagulam_condere(vol, lit("scriba/paginae"),
        lit(
        "<paginae><pagina nomen=\"1\"/>"
        "<pagina nomen=\"3\"/></paginae>"),
        "probatio"));
    l = scriba_liber_aperire(piscina, intern, vol, XL, XX);
    CREDO_AEQUALIS_I32(scriba_liber_numerus(l), II);
    /* numerus + I = 3 iam adest: proximus */
    CREDO_CHORDA_AEQUALIS_LITERIS(scriba_liber_pagina_nova(l), "4");

    CREDO_GRADUS("VI: sine limite (decisio XIV)");
    per (k = ZEPHYRUM; k < CXX; k++)
    {
        (vacuum)scriba_liber_pagina_nova(l);
    }
    CREDO_AEQUALIS_I32(scriba_liber_numerus(l), CXXIII);
    CREDO_NON_NIHIL(scriba_liber_pagina(l, scriba_liber_nomen(l,
        CXXII)));

    CREDO_GRADUS("VII: pagina nominata (S3c)");
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

    CREDO_GRADUS("VIII: liber in spatio (A6c, liber diei)");
    {
             ScribaLiber* ld;
        ScribaDocumentum* dd;

        ld = scriba_liber_aperire_in_spatio(piscina, intern, vol,
            "dies/2026-10-14", XL, X);
        CREDO_NON_NIHIL(ld);
        CREDO_AEQUALIS_I32(scriba_liber_numerus(ld), I);
        index = volumen_plagulam_promere(vol,
            lit("dies/2026-10-14/paginae"), piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_CHORDA_CONTINET(index, lit("<pagina nomen=\"1\"/>"));
        dd = scriba_liber_pagina(ld, lit("1"));
        CREDO_NON_NIHIL(dd);
        CREDO_AEQUALIS_I32(dd ? dd->altitudo : ZEPHYRUM, X);
        /* textus diei: pagina '1' communis ('q') non mutatur */
        si (dd)
        {
            tabula_initiare(&folium, piscina, dd->latitudo,
                dd->altitudo);
            memcpy(folium.cellulae, scriba_documentum_tabula(dd)
                ->cellulae, (memoriae_index)(dd->latitudo
                * dd->altitudo));
            tabula_cellula(&folium, ZEPHYRUM, ZEPHYRUM) = 'z';
            CREDO_VERUM(scriba_documentum_committere(dd, &folium)
                > ZEPHYRUM);
        }
        ld = scriba_liber_aperire_in_spatio(piscina, intern, vol,
            "dies/2026-10-14", XL, X);
        dd = scriba_liber_pagina(ld, lit("1"));
        CREDO_VERUM(dd && tabula_cellula(scriba_documentum_tabula(dd),
            ZEPHYRUM, ZEPHYRUM) == 'z');
        l = scriba_liber_aperire(piscina, intern, vol, XL, XX);
        d = scriba_liber_pagina(l, lit("1"));
        CREDO_VERUM(d && tabula_cellula(scriba_documentum_tabula(d),
            ZEPHYRUM, ZEPHYRUM) == 'q');
        /* dies alius: liber suus, vacuus */
        ld = scriba_liber_aperire_in_spatio(piscina, intern, vol,
            "dies/2026-10-15", XL, X);
        dd = scriba_liber_pagina(ld, lit("1"));
        CREDO_VERUM(dd && tabula_cellula(scriba_documentum_tabula(dd),
            ZEPHYRUM, ZEPHYRUM) != 'z');
    }

    CREDO_GRADUS("IX: lineae libri, lectio sola");
    {
             ScribaLiber* ld;
        ScribaDocumentum* dd;
                     Xar* x;

        /* liber absens: nullae lineae, nihil scriptum */
        x = xar_creare(piscina, (i32)magnitudo(chorda));
        CREDO_VERUM(scriba_liber_lineas_legere(piscina, intern, vol,
            "dies/2026-10-20", ZEPHYRUM, x));
        CREDO_AEQUALIS_I32(xar_numerus(x), ZEPHYRUM);
        (vacuum)volumen_plagulam_promere(vol,
            lit("dies/2026-10-20/paginae"), piscina, &inventum);
        CREDO_FALSUM(inventum);
        /* paginae duae; lineae vacuae et spatiorum solae omissae,
         * spatia initialia servata; pagina tertia in indice sine
         * documento non creatur */
        ld = scriba_liber_aperire_in_spatio(piscina, intern, vol,
            "dies/2026-10-21", XL, X);
        dd = scriba_liber_pagina(ld, lit("1"));
        CREDO_NECESSE_NON_NIHIL(dd);
        tabula_ex_literis_cum_dimensionibus(&folium, piscina, XL, X,
            "alpha\n\n   \n\t\n  beta");
        CREDO_VERUM(scriba_documentum_committere(dd, &folium)
            > ZEPHYRUM);
        dd = scriba_liber_pagina(ld, scriba_liber_pagina_nova(ld));
        CREDO_NECESSE_NON_NIHIL(dd);
        tabula_ex_literis_cum_dimensionibus(&folium, piscina, XL, X,
            "gamma");
        CREDO_VERUM(scriba_documentum_committere(dd, &folium)
            > ZEPHYRUM);
        n = scriba_liber_pagina_nova(ld);
        CREDO_CHORDA_AEQUALIS_LITERIS(n, "3");
        x = xar_creare(piscina, (i32)magnitudo(chorda));
        CREDO_VERUM(scriba_liber_lineas_legere(piscina, intern, vol,
            "dies/2026-10-21", ZEPHYRUM, x));
        CREDO_AEQUALIS_I32(xar_numerus(x), III);
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(x,
            ZEPHYRUM),
            "alpha");
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(x, I),
            "  beta");
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(x, II),
            "gamma");
        CREDO_VERUM(scriba_documentum_aperire(piscina, intern, vol,
            "dies/2026-10-21/paginae/3") == NIHIL);
        /* tabula ('\t' + continuatio, II cellulae) = spatia duo */
        ld = scriba_liber_aperire_in_spatio(piscina, intern, vol,
            "dies/2026-10-22", XL, X);
        dd = scriba_liber_pagina(ld, lit("1"));
        CREDO_NECESSE_NON_NIHIL(dd);
        tabula_ex_literis_cum_dimensionibus(&folium, piscina, XL, X,
            "a\tb");
        CREDO_VERUM(scriba_documentum_committere(dd, &folium)
            > ZEPHYRUM);
        x = xar_creare(piscina, (i32)magnitudo(chorda));
        CREDO_VERUM(scriba_liber_lineas_legere(piscina, intern, vol,
            "dies/2026-10-22", ZEPHYRUM, x));
        CREDO_AEQUALIS_I32(xar_numerus(x), I);
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(x,
            ZEPHYRUM),
            "a  b");
        /* maximum */
        x = xar_creare(piscina, (i32)magnitudo(chorda));
        CREDO_VERUM(scriba_liber_lineas_legere(piscina, intern, vol,
            "dies/2026-10-21", II, x));
        CREDO_AEQUALIS_I32(xar_numerus(x), II);
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(x, I),
            "  beta");
    }

    CREDO_GRADUS("X: nomina et pagina una, lectio sola (D5)");
    {
        Xar* x;
        s64  acta;
        s64  plagulae;

        acta      = volumen_summa_actorum(vol);
        plagulae  = volumen_summa_plagularum(vol);
        /* liber communis: praefixum NIHIL */
        x = xar_creare(piscina, (i32)magnitudo(chorda));
        CREDO_VERUM(scriba_liber_nomina_legere(piscina, intern, vol,
            NIHIL, x));
        CREDO_AEQUALIS_I32(xar_numerus(x), CXXV);
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(x,
            ZEPHYRUM), "1");
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(x,
            CXXIV), "x-1_b");
        x = xar_creare(piscina, (i32)magnitudo(chorda));
        CREDO_VERUM(scriba_liber_paginam_legere(piscina, intern, vol,
            NIHIL, lit("1"), x));
        CREDO_AEQUALIS_I32(xar_numerus(x), I);
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(x,
            ZEPHYRUM), "q");
        /* liber diei: index ordine, paginae singulae */
        x = xar_creare(piscina, (i32)magnitudo(chorda));
        CREDO_VERUM(scriba_liber_nomina_legere(piscina, intern, vol,
            "dies/2026-10-21", x));
        CREDO_AEQUALIS_I32(xar_numerus(x), III);
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(x, II),
            "3");
        x = xar_creare(piscina, (i32)magnitudo(chorda));
        CREDO_VERUM(scriba_liber_paginam_legere(piscina, intern, vol,
            "dies/2026-10-21", lit("1"), x));
        CREDO_AEQUALIS_I32(xar_numerus(x), II);
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(x, I),
            "  beta");
        x = xar_creare(piscina, (i32)magnitudo(chorda));
        CREDO_VERUM(scriba_liber_paginam_legere(piscina, intern, vol,
            "dies/2026-10-21", lit("2"), x));
        CREDO_AEQUALIS_I32(xar_numerus(x), I);
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(x,
            ZEPHYRUM), "gamma");
        /* pagina in indice sine documento, pagina ignota, liber
         * absens: nihil, nihil creatum */
        x = xar_creare(piscina, (i32)magnitudo(chorda));
        CREDO_VERUM(scriba_liber_paginam_legere(piscina, intern, vol,
            "dies/2026-10-21", lit("3"), x));
        CREDO_VERUM(scriba_liber_paginam_legere(piscina, intern, vol,
            "dies/2026-10-21", lit("nusquam"), x));
        CREDO_VERUM(scriba_liber_paginam_legere(piscina, intern, vol,
            "dies/2026-10-30", lit("1"), x));
        CREDO_VERUM(scriba_liber_nomina_legere(piscina, intern, vol,
            "dies/2026-10-30", x));
        CREDO_AEQUALIS_I32(xar_numerus(x), ZEPHYRUM);
        CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), acta);
        CREDO_AEQUALIS_S64(volumen_summa_plagularum(vol), plagulae);
        CREDO_VERUM(scriba_documentum_aperire(piscina, intern, vol,
            "dies/2026-10-21/paginae/3") == NIHIL);
        /* index malus */
        CREDO_VERUM(volumen_plagulam_condere(vol,
            lit("dies/2026-10-23/paginae"), lit("<paginae"),
            "probatio"));
        CREDO_FALSUM(scriba_liber_nomina_legere(piscina, intern, vol,
            "dies/2026-10-23", x));
    }
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "liber paginarum (scenarium I-IX)", _sectio_prima, _parare,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("scriba_liber", SECTIONES);
}
