/* probatio_scriba_actiones.c - pagina.clavis per dispensatorem verum
 * (scriba-plan S1c): canones scribae veri, manus ludus ut fons verus
 * (clavis + textus), arbor minima (S2 compositionem veram facit).
 *
 * Probat: littera semel (non bis); insertio + Esc = actum unum;
 * insertio cum pausa = frusta coniuncta, 'u' totam revocat; dd
 * statim; dd -> capsa, p glutinat; fd ex tempore eventus; finire
 * insertionem apertam servat; nullus valor vacuus in insula. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "volumen.h"
#include "insula.h"
#include "motus.h"
#include "actio.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "scriba_documentum.h"
#include "scriba_actiones.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LAT XVI
#define ALT VI

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

/* arbor minima: radix + pagina focusabilis cum actione */
interior Componens*
componere_minimum (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    Componens* radix;
    Componens* pagina;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)ctx;
    radix   = componens_creare(p, in, "radix", PARTES_NULLUM);
    pagina  = componens_creare(p, in, "pagina", PARTES_CAMPUS);
    componens_ponere_actio(pagina, "pagina.clavis");
    componens_ponere_focusabilis(pagina, VERUM);
    componens_addere_liberum(radix, pagina);
    redde radix;
}

interior Canon*
canonem_legere (
    constans character* via)
{
    chorda  fons;
    chorda  causa;
     Canon* c;

    fons  = filum_legere_totum(via, piscina);
    c     = canon_legere(fons, piscina, intern, &causa);
    si (!c)
    {
        imprimere("  canon malus %s: %.*s\n", via,
            (integer)causa.mensura,
                  causa.datum ? (constans character*)causa.datum : "");
    }
    redde c;
}

/* linea l folii (spatiis finalibus omissis) == expectata */
interior b32
linea_est (
    constans TabulaCharacterum* t,
                           i32  l,
           constans character* expectata)
{
    i32 finis;
    i32 n;

    finis = t->latitudo;
    dum (finis > ZEPHYRUM && tabula_cellula(t, l, finis - I) == ' ')
    {
        finis--;
    }
    n = (i32)strlen(expectata);
    si (   n != finis
        || memcmp(&tabula_cellula(t, l, ZEPHYRUM), expectata,
        (size_t)n))
    {
        imprimere("  linea %d: '%.*s' (expectata '%s')\n", (integer)l,
                  (integer)finis, &tabula_cellula(t, l, ZEPHYRUM),
                  expectata);
        redde FALSUM;
    }
    redde VERUM;
}

interior i32
acta_generis (
               Volumen* vol,
    constans character* genus)
{
             Xar* acta;
             i32  i;
             i32  n;

    acta  = volumen_acta_legere(vol, ZEPHYRUM, piscina);
    n     = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        si (chorda_aequalis_literis(
            ((VolumenActum*)xar_obtinere(acta, i))->genus, genus))
        {
            n++;
        }
    }
    redde n;
}

interior b32
ephemera_est (
     InsulaRepositorium* repo,
     constans character* titulus,
     constans character* valor)
{
    chorda* a;

    a = insula_attributum(repo, INSULA_EPHEMERA, titulus);
    si (!a || !chorda_aequalis_literis(*a, valor))
    {
        imprimere("  %s = '%.*s' (expectatum '%s')\n", titulus,
                  (integer)(a ? a->mensura : ZEPHYRUM),
                  a && a->datum ? (constans character*)a->datum : "",
                  valor);
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
textus_habet (
                 chorda  textus,
     constans character* quaesitum)
{
    redde strstr(chorda_ut_cstr(textus, piscina), quaesitum) != NIHIL;
}

interior vacuum
effugium (
    ManusLudus* m)
{
    (vacuum)manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM);
}

s32 principale (vacuum)
{
                       Volumen* vol;
              ScribaDocumentum* doc;
            InsulaRepositorium* repo;
                ActioRegistrum* reg;
                ScribaActiones  sa;
                   Dispensator* d;
                    ManusLudus* m;
    constans TabulaCharacterum* t;
                           i32  mutationes;

    piscina = piscina_generare_dynamicum("probatio_scriba_actiones",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern  = internamentum_creare(piscina);
    vol     = volumen_temporarium(piscina, "probatio_scriba_actiones");
    doc = scriba_documentum_creare(piscina, intern, vol, "", LAT, ALT,
        IV);
    CREDO_NON_NIHIL(doc);
    t    = scriba_documentum_tabula(doc);
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum latitudo=\"16\" altitudo=\"6\"/>",
        "<ephemera focus=\"pagina\"/>");
    insula_ponere_canonem(repo, INSULA_DURABILIS,
        canonem_legere("apps/scriba/canones/durabilis.canon"));
    insula_ponere_canonem(repo, INSULA_EPHEMERA,
        canonem_legere("apps/scriba/canones/ephemera.canon"));
    {
              chorda domini;
        StmlResultus res;

        domini = filum_legere_totum("apps/scriba/canones/domini.stml",
                                    piscina);
        res = stml_legere_ex_literis(chorda_ut_cstr(domini, piscina),
                                     piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_VERUM(insula_dominos_legere(repo, INSULA_EPHEMERA,
            res.elementum_radix) > ZEPHYRUM);
    }
    reg = actio_registrum_creare(piscina, intern);
    scriba_actiones_initiare(&sa, doc, piscina);
    scriba_actiones_registrare(reg, &sa);
    d = dispensator_creare(piscina, intern, repo, reg,
        componere_minimum,
                           NIHIL, CCC);
    CREDO_NON_NIHIL(d);
    scriba_gestum_ponere(dispensator_motus(d), &sa);
    m = manus_ludus_creare(piscina, d);
    CREDO_MANUS_LUDUS_FOCUS(m, "pagina");

    imprimere("\n--- I: insertio + Esc = actum unum; littera semel\n");
    CREDO_VERUM(manus_ludus_scribere(m, "ihello"));
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), ZEPHYRUM);
    CREDO_VERUM(ephemera_est(repo, "modus", "inserere"));
    effugium(m);
    CREDO_VERUM(linea_est(t, ZEPHYRUM, "hello"));
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), I);
    CREDO_VERUM(ephemera_est(repo, "modus", "normalis"));
    /* vim domus: Esc cursorem non retrahit (folium 2D) */
    CREDO_VERUM(ephemera_est(repo, "cursor_columna", "5"));
    /* canon omnia accepit (canones tacite recusant) */
    CREDO_NON_NIHIL(insula_attributum(repo, INSULA_EPHEMERA,
        "cursor_linea"));
    CREDO_NON_NIHIL(insula_attributum(repo, INSULA_EPHEMERA,
        "visualis_genus"));
    CREDO_NON_NIHIL(insula_attributum(repo, INSULA_EPHEMERA,
        "selectio_linea"));
    CREDO_NON_NIHIL(insula_attributum(repo, INSULA_EPHEMERA,
        "fd_exspectans"));
    CREDO_NON_NIHIL(insula_attributum(repo, INSULA_EPHEMERA,
        "capsa_lineae"));
    /* nullus valor vacuus: capsa et clavis praecedens ABSUNT - in
     * textu scripto quaeritur (a="" capere NIHIL reddit ut absentia;
     * scriptor pulcher a="" ut a nudum scribit, relectum "true") */
    CREDO_FALSUM(textus_habet(insula_scribere(repo, INSULA_EPHEMERA,
        piscina), "capsa="));
    CREDO_FALSUM(textus_habet(insula_scribere(repo, INSULA_EPHEMERA,
        piscina), "clavis_praecedens"));

    imprimere("\n--- II: pausa = frusta coniuncta; 'u' insertionem"
              " totam ---\n");
    CREDO_VERUM(manus_ludus_scribere(m, "A wor"));
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), I);   /* nondum */
    manus_ludus_exspectare(m, MCC);
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), II);
    CREDO_VERUM(linea_est(t, ZEPHYRUM, "hello wor"));
    CREDO_VERUM(manus_ludus_scribere(m, "ld"));
    manus_ludus_exspectare(m, MCC);
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), III);
    CREDO_AEQUALIS_I32(acta_generis(vol, "coniunctio"), I);
    effugium(m);
    CREDO_VERUM(linea_est(t, ZEPHYRUM, "hello world"));
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), III);
    CREDO_VERUM(manus_ludus_scribere(m, "u"));
    CREDO_VERUM(linea_est(t, ZEPHYRUM, "hello"));
    CREDO_VERUM(linea_est(&sa.laboris, ZEPHYRUM, "hello"));
    CREDO_VERUM(manus_ludus_clavem(m, 'r', MOD_IMPERIUM));
    CREDO_VERUM(linea_est(t, ZEPHYRUM, "hello world"));

    imprimere("\n--- III: dd statim; clavis praecedens, vacuitas\n");
    CREDO_VERUM(manus_ludus_scribere(m, "ofoo"));
    effugium(m);
    CREDO_VERUM(linea_est(t, I, "foo"));
    mutationes = acta_generis(vol, "mutatio");
    CREDO_VERUM(manus_ludus_scribere(m, "d"));
    CREDO_NON_NIHIL(insula_attributum(repo, INSULA_EPHEMERA,
        "clavis_praecedens"));
    CREDO_VERUM(manus_ludus_scribere(m, "d"));
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA,
        "clavis_praecedens"));
    CREDO_VERUM(linea_est(t, I, ""));
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), mutationes + I);

    imprimere("\n--- IV: capsa ex dd; g p glutinat ---\n");
    /* vim domus 'y' non habet: dd capsam implet */
    CREDO_VERUM(ephemera_est(repo, "capsa", "foo"));
    CREDO_VERUM(ephemera_est(repo, "capsa_lineae", "1"));
    CREDO_VERUM(manus_ludus_scribere(m, "gp"));
    CREDO_VERUM(linea_est(t, ZEPHYRUM, "hello world"));
    CREDO_VERUM(linea_est(t, I, "foo"));

    imprimere("\n--- V: fd ex tempore eventus; nullum '\\0' ---\n");
    /* 'i' indentationem lineae ponit (status verus, actum honestum);
     * f insertum et per fd deletum: textus idem, nullum '\0' (tabula
     * trahens sinistram '\0' scribit - albatur) */
    CREDO_VERUM(manus_ludus_scribere(m, "ifd"));
    CREDO_VERUM(ephemera_est(repo, "modus", "normalis"));
    CREDO_VERUM(linea_est(t, I, "foo"));
    CREDO_VERUM(memchr(t->cellulae, '\0', (size_t)(LAT * ALT))
        == NIHIL);
    CREDO_VERUM(manus_ludus_scribere(m, "if"));
    manus_ludus_exspectare(m, D);
    CREDO_VERUM(manus_ludus_scribere(m, "d"));
    CREDO_VERUM(ephemera_est(repo, "modus", "inserere"));
    effugium(m);
    CREDO_VERUM(linea_est(t, I, "fdfoo"));

    imprimere("\n--- VI: finire insertionem apertam servat ---\n");
    mutationes = acta_generis(vol, "mutatio");
    CREDO_VERUM(manus_ludus_scribere(m, "Azz"));
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), mutationes);
    CREDO_VERUM(memcmp(t->cellulae, sa.laboris.cellulae,
        (size_t)(LAT * ALT)) != ZEPHYRUM);
    dispensator_finire(d);
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), mutationes + I);
    CREDO_VERUM(memcmp(t->cellulae, sa.laboris.cellulae,
        (size_t)(LAT * ALT)) == ZEPHYRUM);

    imprimere("\n--- VII: insula honesta ---\n");
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));
    CREDO_VERUM(scriba_documentum_verificare(doc));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
