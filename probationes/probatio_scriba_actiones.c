/* probatio_scriba_actiones.c - pagina.clavis per dispensatorem verum
 * (scriba-plan S1c): canones scribae veri, manus ludus ut fons verus
 * (clavis + textus), arbor minima (S2 compositionem veram facit).
 *
 * Probat: littera semel (non bis); insertio + Esc = actum unum;
 * insertio cum pausa = frusta coniuncta, 'u' totam revocat; dd
 * statim; dd -> capsa, p glutinat; fd ex tempore eventus; finire
 * insertionem apertam servat; nullus valor vacuus in insula.
 * Exportatio (capsa -> clipboard systematis per portam): yy, y
 * visualis, dd exportant (iterum idem quoque), dedentatio communis,
 * linea vacua servatur, linea longa (XC) integra; motus nihil. */
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
#include "eventus.h"
#include "manus_ludus.h"
#include "scriba_documentum.h"
#include "scriba_actiones.h"
#include "tabula_characterum.h"
#include "credo.h"
#include "organum_probationis.h"
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

    a = insula_attributum(repo, titulus);
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

/* piscina et intern = sectionis (adiutores supra eas legunt); purgare
 * reponit ne status inter sectiones maneat */
interior vacuum
_parare (
    CredoContextus* c)
{
    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina  = NIHIL;
    intern   = NIHIL;
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
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

    (vacuum)c;
    vol     = volumen_temporarium(piscina, "probatio_scriba_actiones");
    doc = scriba_documentum_creare(piscina, intern, vol, "", LAT, ALT,
        IV);
    CREDO_NON_NIHIL(doc);
    t    = scriba_documentum_tabula(doc);
    repo = insula_repositorium_creare(piscina, intern,
        "<scriba latitudo=\"16\" altitudo=\"6\""
        " focus=\"pagina\"/>");
    {
            Canon* canon;
        StmlNodus* domini;

        /* S6b: ex manifesto scribae */
        CREDO_VERUM(organum_probationis_parare(piscina, intern,
            "apps/scriba/organum.stml", &canon, &domini));
        insula_ponere_canonem(repo, canon);
        CREDO_VERUM(insula_dominos_legere(repo, domini) > ZEPHYRUM);
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

    CREDO_GRADUS("I: insertio + Esc = actum unum; littera semel");
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
    CREDO_NON_NIHIL(insula_attributum(repo,
        "cursor_linea"));
    CREDO_NON_NIHIL(insula_attributum(repo,
        "visualis_genus"));
    CREDO_NON_NIHIL(insula_attributum(repo,
        "selectio_linea"));
    CREDO_NON_NIHIL(insula_attributum(repo,
        "fd_exspectans"));
    /* S7b: registrum solum mutatum scribitur - vacuum numquam */
    CREDO_NIHIL(insula_attributum(repo,
        "capsa_lineae"));
    /* nullus valor vacuus: capsa et clavis praecedens ABSUNT - in
     * textu scripto quaeritur (a="" capere NIHIL reddit ut absentia;
     * scriptor pulcher a="" ut a nudum scribit, relectum "true") */
    CREDO_FALSUM(textus_habet(insula_scribere(repo,
        piscina), "capsa="));
    CREDO_FALSUM(textus_habet(insula_scribere(repo,
        piscina), "clavis_praecedens"));

    CREDO_GRADUS("II: pausa = frusta coniuncta; 'u' insertionem totam");
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

    CREDO_GRADUS("III: dd statim; clavis praecedens, vacuitas");
    CREDO_VERUM(manus_ludus_scribere(m, "ofoo"));
    effugium(m);
    CREDO_VERUM(linea_est(t, I, "foo"));
    mutationes = acta_generis(vol, "mutatio");
    CREDO_VERUM(manus_ludus_scribere(m, "d"));
    CREDO_NON_NIHIL(insula_attributum(repo,
        "clavis_praecedens"));
    CREDO_VERUM(manus_ludus_scribere(m, "d"));
    CREDO_NIHIL(insula_attributum(repo,
        "clavis_praecedens"));
    CREDO_VERUM(linea_est(t, I, ""));
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), mutationes + I);

    CREDO_GRADUS("IV: capsa ex dd; g p glutinat");
    /* dd capsam implet (ut yy) */
    CREDO_VERUM(ephemera_est(repo, "capsa", "foo"));
    CREDO_VERUM(ephemera_est(repo, "capsa_lineae", "1"));
    CREDO_VERUM(manus_ludus_scribere(m, "gp"));
    CREDO_VERUM(linea_est(t, ZEPHYRUM, "hello world"));
    CREDO_VERUM(linea_est(t, I, "foo"));

    CREDO_GRADUS("V: fd ex tempore eventus; nullum '\\0'");
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
    /* Ctrl-[ = Esc (Franus): runa '[' (manus) et character productus
     * ESC solus (fenestra: dispositio ubi '[' alibi) */
    CREDO_VERUM(manus_ludus_scribere(m, "i"));
    CREDO_VERUM(ephemera_est(repo, "modus", "inserere"));
    CREDO_VERUM(manus_ludus_clavem(m, '[', MOD_IMPERIUM));
    CREDO_VERUM(ephemera_est(repo, "modus", "normalis"));
    CREDO_VERUM(manus_ludus_scribere(m, "i"));
    {
        Eventus e;

        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
        e.tempus                     = M * M;
        e.datum.clavis.producta      = XXVII;
        e.datum.clavis.modificantes  = MOD_IMPERIUM;
        dispensator_tractare(d, &e);
    }
    CREDO_VERUM(ephemera_est(repo, "modus", "normalis"));
    CREDO_VERUM(memchr(t->cellulae, '[', (size_t)(LAT * ALT)) == NIHIL);

    CREDO_GRADUS("VI: finire insertionem apertam servat");
    mutationes = acta_generis(vol, "mutatio");
    CREDO_VERUM(manus_ludus_scribere(m, "Azz"));
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), mutationes);
    CREDO_VERUM(memcmp(t->cellulae, sa.laboris.cellulae,
        (size_t)(LAT * ALT)) != ZEPHYRUM);
    dispensator_finire(d);
    CREDO_AEQUALIS_I32(acta_generis(vol, "mutatio"), mutationes + I);
    CREDO_VERUM(memcmp(t->cellulae, sa.laboris.cellulae,
        (size_t)(LAT * ALT)) == ZEPHYRUM);

    CREDO_GRADUS("VII: insula honesta");
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));
    CREDO_VERUM(scriba_documentum_verificare(doc));

    volumen_claudere(vol);
}

/* porta exportationis ficta: textus ultimus et numerus */
nomen structura {
          i32 numerus;
    character textus[DXII];
} Exportata;

interior b32
exportare_fictum (
     vacuum* ctx,
     chorda  textus)
{
    Exportata* e;

    e = (Exportata*)ctx;
    e->numerus++;
    sprintf(e->textus, "%.*s", (integer)(textus.mensura < DXI
        ? textus.mensura : DXI), (constans character*)textus.datum);
    redde VERUM;
}

/* capsa -> clipboard systematis (porta): yy, V..y, dd; dedentatio
 * communis (lineae vacuae non numerant); linea longa integra; motus
 * sine scriptura capsae nihil exportat */
interior vacuum
_sectio_exportandi (
    CredoContextus* c)
{
               Volumen* vol;
      ScribaDocumentum* doc;
    InsulaRepositorium* repo;
        ActioRegistrum* reg;
        ScribaActiones  sa;
           Dispensator* d;
            ManusLudus* m;
     TabulaCharacterum  folium;
             Exportata  ex;
             character  longa[XCI];
             character  textus[CCLVI];

    (vacuum)c;
    memset(&ex, ZEPHYRUM, magnitudo(ex));
    memset(longa, 'a', XC);
    longa[XC] = '\0';
    sprintf(textus, "    def foo():\n        return 1\n\n    pass\n%s",
        longa);
    vol = volumen_temporarium(piscina, "probatio_scriba_actiones");
    doc = scriba_documentum_creare(piscina, intern, vol, "", C, VIII,
        IV);
    CREDO_NECESSE_NON_NIHIL(doc);
    tabula_ex_literis_cum_dimensionibus(&folium, piscina, C, VIII,
        textus);
    CREDO_VERUM(scriba_documentum_committere(doc, &folium) > ZEPHYRUM);
    repo = insula_repositorium_creare(piscina, intern,
        "<scriba latitudo=\"100\" altitudo=\"8\" focus=\"pagina\"/>");
    {
            Canon* canon;
        StmlNodus* domini;

        CREDO_VERUM(organum_probationis_parare(piscina, intern,
            "apps/scriba/organum.stml", &canon, &domini));
        insula_ponere_canonem(repo, canon);
        CREDO_VERUM(insula_dominos_legere(repo, domini) > ZEPHYRUM);
    }
    reg = actio_registrum_creare(piscina, intern);
    scriba_actiones_initiare(&sa, doc, piscina);
    sa.textum_exportare  = exportare_fictum;
    sa.exportare_ctx     = &ex;
    scriba_actiones_registrare(reg, &sa);
    d = dispensator_creare(piscina, intern, repo, reg,
        componere_minimum,
        NIHIL, CCC);
    CREDO_NECESSE_NON_NIHIL(d);
    scriba_gestum_ponere(dispensator_motus(d), &sa);
    m = manus_ludus_creare(piscina, d);

    CREDO_GRADUS("yy: linea sine indentatione");
    CREDO_VERUM(manus_ludus_scribere(m, "yy"));
    CREDO_AEQUALIS_I32(ex.numerus, I);
    CREDO_VERUM(strcmp(ex.textus, "def foo():") == ZEPHYRUM);

    CREDO_GRADUS("yy iterum idem: iterum exportat; motus nihil");
    ex.textus[ZEPHYRUM] = '\0';
    CREDO_VERUM(manus_ludus_scribere(m, "yy"));
    CREDO_AEQUALIS_I32(ex.numerus, II);
    CREDO_VERUM(strcmp(ex.textus, "def foo():") == ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "jk"));
    CREDO_AEQUALIS_I32(ex.numerus, II);

    CREDO_GRADUS("V y: dedentatio communis, linea vacua servatur");
    CREDO_VERUM(manus_ludus_scribere(m, "Vjjjy"));
    CREDO_AEQUALIS_I32(ex.numerus, III);
    CREDO_VERUM(strcmp(ex.textus,
        "def foo():\n    return 1\n\npass") == ZEPHYRUM);

    CREDO_GRADUS("linea longa (XC) integra; dd exportat");
    CREDO_VERUM(manus_ludus_scribere(m, "jjjjyy"));
    CREDO_AEQUALIS_I32(ex.numerus, IV);
    CREDO_VERUM(strcmp(ex.textus, longa) == ZEPHYRUM);
    ex.textus[ZEPHYRUM] = '\0';
    CREDO_VERUM(manus_ludus_scribere(m, "dd"));
    CREDO_AEQUALIS_I32(ex.numerus, V);
    CREDO_VERUM(strcmp(ex.textus, longa) == ZEPHYRUM);
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "pagina.clavis per dispensatorem verum", _sectio_prima, _parare,
        _purgare, NIHIL, NIHIL },
    { "exportatio", _sectio_exportandi, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("scriba_actiones", SECTIONES);
}
