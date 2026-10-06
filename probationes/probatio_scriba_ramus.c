/* probatio_scriba_ramus.c - duae scribae in ramis (insula-rami-plan R4)
 *
 * Repositorium hospitis cum duabus scribis montatis (s1, s2), canones
 * et domini veri in utroque ramo; documenta in volumine UNO (spatia
 * s1, s2 - R2); dispensator et manus pro utraque. Probat: scriptio in
 * una ramum et documentum suum solum mutat; revocatio independens;
 * documenta ex volumine reaperta; radix statum scribae non fert. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "volumen.h"
#include "insula.h"
#include "actio.h"
#include "componens.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "scriba_documentum.h"
#include "scriba_actiones.h"
#include "scriba_componentia.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior Canon*
canonem_legere (
    constans character* via)
{
    chorda causa;

    redde canon_legere(filum_legere_totum(via, piscina), piscina,
        intern,
                       &causa);
}

/* ramum ramum_parare: canones veri, domini (ephemera) */
interior vacuum
ramum_parare (
    constans InsulaRamus* ramus)
{
    StmlResultus res;

    insula_ramus_canonem_ponere(ramus, INSULA_DURABILIS,
        canonem_legere("apps/scriba/canones/durabilis.canon"));
    insula_ramus_canonem_ponere(ramus, INSULA_EPHEMERA,
        canonem_legere("apps/scriba/canones/ephemera.canon"));
    res = stml_legere_ex_literis(chorda_ut_cstr(filum_legere_totum(
        "apps/scriba/canones/domini.stml", piscina), piscina), piscina,
        intern);
    (vacuum)insula_ramus_dominos_legere(ramus, INSULA_EPHEMERA,
        res.elementum_radix);
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
    redde n == finis
        && memcmp(&tabula_cellula(t, l, ZEPHYRUM), expectata, (size_t)n)
           == ZEPHYRUM;
}

interior b32
valor_est (
                chorda* a,
    constans character* v)
{
    redde a ? chorda_aequalis_literis(*a, v) : FALSUM;
}

s32 principale (vacuum)
{
               Volumen* vol;
      ScribaDocumentum* documentum_primum;
      ScribaDocumentum* documentum_secundum;
    InsulaRepositorium* repo;
           InsulaRamus  s1;
           InsulaRamus  s2;
        ActioRegistrum* index_primus;
        ActioRegistrum* index_secundus;
        ScribaActiones  actiones_primae;
        ScribaActiones  actiones_secundae;
      ScribaCompositio  compositio_prima;
      ScribaCompositio  compositio_secunda;
           Dispensator* d1;
           Dispensator* d2;
            ManusLudus* m1;
            ManusLudus* m2;

    piscina = piscina_generare_dynamicum("probatio_scriba_ramus",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern  = internamentum_creare(piscina);
    vol     = volumen_temporarium(piscina, "probatio_scriba_ramus");
    documentum_primum = scriba_documentum_creare(piscina, intern, vol,
        "s1", XVI, VI,
                                    IV);
    documentum_secundum = scriba_documentum_creare(piscina, intern, vol,
        "s2", XVI, VI,
                                    IV);
    repo = insula_repositorium_creare(piscina, intern,
        "<hospes><scriba id=\"s1\" latitudo=\"16\" altitudo=\"6\"/>"
        "<scriba id=\"s2\" latitudo=\"16\" altitudo=\"6\"/></hospes>",
        "<hospes focus=\"pagina\">"
        "<scriba id=\"s1\" modus=\"normalis\"/>"
        "<scriba id=\"s2\" modus=\"normalis\"/></hospes>");
    CREDO_NON_NIHIL(repo);
    s1 = insula_ramus(repo, "scriba", "s1");
    s2 = insula_ramus(repo, "scriba", "s2");
    ramum_parare(&s1);
    ramum_parare(&s2);

    scriba_actiones_initiare(&actiones_primae, documentum_primum,
        piscina);
    scriba_actiones_initiare(&actiones_secundae, documentum_secundum,
        piscina);
    actiones_primae.ramus    = s1;
    actiones_secundae.ramus  = s2;
    index_primus             = actio_registrum_creare(piscina, intern);
    index_secundus           = actio_registrum_creare(piscina, intern);
    scriba_actiones_registrare(index_primus, &actiones_primae);
    scriba_actiones_registrare(index_secundus, &actiones_secundae);
    memset(&compositio_prima, ZEPHYRUM, magnitudo(compositio_prima));
    compositio_prima.fenestra_latitudo  = CDLXXX;
    compositio_prima.fenestra_altitudo  = CDLXXX;
    compositio_prima.cellula_latitudo   = VI;
    compositio_prima.cellula_altitudo   = VIII;
    compositio_prima.status_lineae      = I;
    compositio_secunda                  = compositio_prima;
    compositio_prima.ramus              = s1;
    compositio_secunda.ramus            = s2;
    d1 = dispensator_creare(piscina, intern, repo, index_primus,
        scriba_componere,
                            &compositio_prima, CCC);
    d2 = dispensator_creare(piscina, intern, repo, index_secundus,
        scriba_componere,
                            &compositio_secunda, CCC);
    scriba_gestum_ponere(dispensator_motus(d1), &actiones_primae);
    scriba_gestum_ponere(dispensator_motus(d2), &actiones_secundae);
    m1 = manus_ludus_creare(piscina, d1);
    m2 = manus_ludus_creare(piscina, d2);

    imprimere("\n--- I: scriptio in s1 - ramus et documentum eius\n");
    CREDO_VERUM(manus_ludus_scribere(m1, "isalve"));
    CREDO_VERUM(manus_ludus_clavem(m1, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(linea_est(scriba_documentum_tabula(documentum_primum),
        ZEPHYRUM,
        "salve"));
    CREDO_VERUM(linea_est(scriba_documentum_tabula(documentum_secundum),
        ZEPHYRUM,
        ""));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&s1, INSULA_EPHEMERA,
        "cursor_columna"), "5"));
    CREDO_NIHIL(insula_ramus_attributum(&s2, INSULA_EPHEMERA,
        "cursor_columna"));
    /* arbor d1 ex ramo s1 (cursor, linea status); d2 suum */
    {
        Componens* pg;
        Componens* st;

        pg = componens_invenire_per_id(dispensator_arbor(d1),
            chorda_ex_literis("pagina", piscina));
        st = componens_invenire_per_id(dispensator_arbor(d1),
            chorda_ex_literis("status", piscina));
        CREDO_NON_NIHIL(pg);
        CREDO_NON_NIHIL(st);
        si (pg && st)
        {
            CREDO_AEQUALIS_S32(pg->puncta[ZEPHYRUM].x, V);
            CREDO_CHORDA_AEQUALIS_LITERIS(st->titulus, "NORMALIS 1:6");
        }
        pg = componens_invenire_per_id(dispensator_arbor(d2),
            chorda_ex_literis("pagina", piscina));
        CREDO_NON_NIHIL(pg);
        si (pg)
        {
            CREDO_AEQUALIS_S32(pg->puncta[ZEPHYRUM].x, ZEPHYRUM);
        }
    }
    /* radix statum scribae non fert */
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA,
        "cursor_columna"));
    CREDO_FALSUM(insula_mendacium(repo));

    imprimere("\n--- II: scriptio in s2 ---\n");
    CREDO_VERUM(manus_ludus_scribere(m2, "imunde"));
    CREDO_VERUM(manus_ludus_clavem(m2, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(linea_est(scriba_documentum_tabula(documentum_secundum),
        ZEPHYRUM,
        "munde"));
    CREDO_VERUM(linea_est(scriba_documentum_tabula(documentum_primum),
        ZEPHYRUM,
        "salve"));

    imprimere("\n--- III: revocatio independens ---\n");
    CREDO_VERUM(manus_ludus_scribere(m1, "u"));
    CREDO_VERUM(linea_est(scriba_documentum_tabula(documentum_primum),
        ZEPHYRUM,
        ""));
    CREDO_VERUM(linea_est(scriba_documentum_tabula(documentum_secundum),
        ZEPHYRUM,
        "munde"));

    imprimere("\n--- IV: documenta ex volumine reaperta\n");
    {
        ScribaDocumentum* r1;
        ScribaDocumentum* r2;

        r1 = scriba_documentum_aperire(piscina, intern, vol, "s1");
        r2 = scriba_documentum_aperire(piscina, intern, vol, "s2");
        CREDO_NON_NIHIL(r1);
        CREDO_NON_NIHIL(r2);
        /* cursor revocandi in memoria solo (historia): reapertio ad
         * finem caudae - 'salve' redit (quaestio Franco posita) */
        CREDO_VERUM(linea_est(scriba_documentum_tabula(r1), ZEPHYRUM,
            "salve"));
        CREDO_VERUM(linea_est(scriba_documentum_tabula(r2), ZEPHYRUM,
            "munde"));
    }
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
