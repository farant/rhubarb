/* probatio_vicus.c - repositorium hospitis cum tabulis
 * (insula-rami-plan T1b)
 *
 * Genera applicationum a principali (hic: probatione) registrantur;
 * vicus nullam applicationem novit. Volumen novum: index ordinarius;
 * tabula addita; reapertio: tabulae, ordo, tituli, activa, documenta
 * eadem; genus ignotum praeteritur cum causa; id geminum recusatur. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "stml.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "pictor_applicatio.h"
#include "scriba_applicatio.h"
#include "vicus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior b32
scribam_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* v,
     InsulaRepositorium* r,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
    redde scriba_montare((ScribaMontatio*)sedes, p, in, v, r, id, radix,
                         latitudo, altitudo);
}

interior b32
pictorem_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* v,
     InsulaRepositorium* r,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
    redde pictor_montare((PictorMontatio*)sedes, p, in, v, r, id, radix,
                         latitudo, altitudo);
}

interior Vicus*
vicum_aperire (
    Volumen* vol)
{
    Vicus* v;

    v = vicus_creare(piscina, intern, vol, NIHIL, CDLXXX, CDLXXX);
    CREDO_NON_NIHIL(v);
    CREDO_VERUM(vicus_genus_addere(v, "scriba",
        magnitudo(ScribaMontatio),
        scribam_montare));
    CREDO_VERUM(vicus_genus_addere(v, "pictor",
        magnitudo(PictorMontatio),
        pictorem_montare));
    CREDO_VERUM(vicus_aperire(v,
        "<tabulae activa=\"s1\">"
        "<tabula id=\"s1\" genus=\"scriba\" titulus=\"scriba\"/>"
        "<tabula id=\"p1\" genus=\"pictor\" titulus=\"pictor\"/>"
        "</tabulae>"));
    redde v;
}

interior b32
titulus_est (
    constans VicusTabula* t,
      constans character* id,
      constans character* genus,
      constans character* titulus)
{
    redde t && chorda_aequalis_literis(t->id, id)
        && chorda_aequalis_literis(t->genus, genus)
        && chorda_aequalis_literis(t->titulus, titulus);
}

interior vacuum
ignotum_ponere (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(nodus, p, in, "ignotum", "1");
}

s32 principale (vacuum)
{
             Volumen* vol;
               Vicus* v;
      ScribaMontatio* sm;
         InsulaRamus  radix;

    piscina = piscina_generare_dynamicum("probatio_vicus", LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_vicus");

    imprimere("\n--- I: volumen novum - index ordinarius ---\n");
    v = vicum_aperire(vol);
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), II);
    CREDO_VERUM(titulus_est(vicus_tabula(v, ZEPHYRUM), "s1", "scriba",
        "scriba"));
    CREDO_VERUM(titulus_est(vicus_tabula(v, I), "p1", "pictor",
        "pictor"));
    CREDO_VERUM(vicus_tabula(v, ZEPHYRUM)->montata);
    CREDO_VERUM(vicus_tabula(v, I)->montata);
    CREDO_VERUM(titulus_est(vicus_activa(v), "s1", "scriba", "scriba"));
    CREDO_AEQUALIS_I32(stml_numerus_liberorum(insula_radix(v->repo,
        INSULA_DURABILIS)), II);

    imprimere("\n--- II: tabula addita; id geminum recusatur ---\n");
    CREDO_VERUM(vicus_tabulam_addere(v, "scriba", "s2", "secunda"));
    CREDO_FALSUM(vicus_tabulam_addere(v, "scriba", "s2", "iterum"));
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), III);
    /* additio ipsa indicem servat (non scriptura posterior) */
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(vicum_aperire(vol)),
        III);
    CREDO_VERUM(vicus_activam_ponere(v, "s2"));
    sm = (ScribaMontatio*)vicus_tabula(v, II)->montatio;

    tabula_cellula(&sm->actiones_ctx.laboris, ZEPHYRUM, ZEPHYRUM) = 'q';
    CREDO_VERUM(scriba_documentum_committere(sm->doc,
        &sm->actiones_ctx.laboris) > ZEPHYRUM);

    imprimere("\n--- III: canon hospitis ---\n");
    radix = insula_ramus_radix(v->repo);
    CREDO_FALSUM(mutare_ramum(&radix, INSULA_EPHEMERA, ignotum_ponere,
        NIHIL));

    imprimere("\n--- IV: reapertio - tabulae, activa, documenta ---\n");
    v = vicum_aperire(vol);
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), III);
    CREDO_VERUM(titulus_est(vicus_tabula(v, II), "s2", "scriba",
        "secunda"));
    CREDO_VERUM(titulus_est(vicus_activa(v), "s2", "scriba",
        "secunda"));
    sm = (ScribaMontatio*)vicus_tabula(v, II)->montatio;
    CREDO_VERUM(tabula_cellula(scriba_documentum_tabula(sm->doc),
        ZEPHYRUM, ZEPHYRUM) == 'q');

    imprimere("\n--- V: genus ignotum - addere recusat ---\n");
    CREDO_FALSUM(vicus_tabulam_addere(v, "lyra", "l1", "lyra"));
    /* volumen ab hospite recentiore: tabula generis ignoti in indice */
    CREDO_VERUM(volumen_plagulam_condere(vol,
        chorda_ex_literis("vicus/tabulae", piscina), chorda_ex_literis(
        "<tabulae activa=\"s2\">"
        "<tabula id=\"s1\" genus=\"scriba\" titulus=\"scriba\"/>"
        "<tabula id=\"p1\" genus=\"pictor\" titulus=\"pictor\"/>"
        "<tabula id=\"s2\" genus=\"scriba\" titulus=\"secunda\"/>"
        "<tabula id=\"l1\" genus=\"lyra\" titulus=\"lyra\"/>"
        "</tabulae>", piscina), "probatio"));
    v = vicum_aperire(vol);
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), IV);
    CREDO_FALSUM(vicus_tabula(v, III)->montata);
    CREDO_VERUM(vicus_causa(v).mensura > ZEPHYRUM);
    CREDO_VERUM(vicus_tabula(v, ZEPHYRUM)->montata);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
