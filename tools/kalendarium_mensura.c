/* kalendarium_mensura.c - pretium picturae calendarii (proiectiones
 * D4b: basis ANTE conspectum 'dies', et post)
 *
 * Volumen temporarium: res agendae L (dies per hebdomadem sparsae),
 * libri dierum VII hebdomadis (pagina una, lineae X, historia
 * mutationum M per paginam - replicatio vera), deinde visus hebdomadis
 * et mensis pinguntur N vicibus. Tempus per clock(): ms per picturam;
 * et lectio librorum VII sola (scriba_liber_lineas_legere) ut pars
 * eius nominetur.
 *
 * Usus: ./bin/aedilis tools/kalendarium_mensura.c &&
 *   bash build/aedilis/kalendarium_mensura/struere.sh &&
 *   build/aedilis/kalendarium_mensura/kalendarium_mensura [mutationes]
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "fasti.h"
#include "agenda.h"
#include "moneta.h"
#include "insula.h"
#include "kalendarium.h"
#include "conspectus.h"
#include "conspectus_dies.h"
#include "calendarium_liturgicum.h"
#include "dispensator.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "componens.h"
#include "scriba_liber.h"
#include "scriba_documentum.h"
#include "tabula_characterum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PICTURAE CC

interior b32
hodie_fixa (
    vacuum* ctx,
      Dies* hodie)
{
    (vacuum)ctx;
    *hodie = fasti_dies(MMXXVI, X, XIV);
    redde VERUM;
}

interior duplex
tempus_transactum (
    clock_t initium)
{
    redde (duplex)(clock() - initium) * 1.0e3 / CLOCKS_PER_SEC;
}

/* liber diei: pagina "1", lineae X, mutationes n (linea prima
 * variata) - historia vera quam aperire replicat */
interior b32
librum_scribere (
                Piscina* p,
    InternamentumChorda* in,
                Volumen* vol,
                   Dies  d,
                    i32  mutationes)
{
          ScribaLiber* l;
     ScribaDocumentum* doc;
    TabulaCharacterum  folium;
            character  praefixum[XXXII];
            character  textus[DXII];
                  i32  i;

    sprintf(praefixum, "dies/%04d-%02d-%02d", (integer)d.annus,
        (integer)d.mensis, (integer)d.dies);
    l = scriba_liber_aperire_in_spatio(p, in, vol, praefixum, XL, XL);
    doc = l ? scriba_liber_pagina(l, chorda_ex_literis("1", p)) : NIHIL;
    si (!doc)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < mutationes; i++)
    {
        sprintf(textus, "nota %u de die\nlinea secunda\nlinea tertia\n"
            "linea quarta\nlinea quinta\nlinea sexta\nlinea septima\n"
            "linea octava\nlinea nona\nlinea decima",
            (insignatus integer)i);
        tabula_ex_literis_cum_dimensionibus(&folium, p, doc->latitudo,
            doc->altitudo, textus);
        si (scriba_documentum_committere(doc, &folium) <= ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior duplex
picturas_metiri (
     KalendariumMontatio* m,
             Dispensator* d,
                 Piscina* p,
     InternamentumChorda* in)
{
     clock_t  initium;
     Mandata* md;
         i32  i;

    initium = clock();
    per (i = ZEPHYRUM; i < PICTURAE; i++)
    {
        md = mandata_creare(p, in);
        pingere(dispensator_arbor(d), m->figurae, ZEPHYRUM, md);
    }
    redde tempus_transactum(initium) / PICTURAE;
}

interior b32
montare (
     KalendariumMontatio*  m,
             Dispensator** d,
                 Piscina*  p,
     InternamentumChorda*  in,
                 Volumen*  vol,
        AgendaDocumentum*  doc,
     ConspectusRegistrum*  repertorium,
      constans character*  argumentum)
{
    InsulaRepositorium* repo;

    repo = insula_repositorium_creare(p, in, "<hospes/>");
    si (!repo || !kalendarium_montare(m, p, in, repo, "c1", argumentum,
            doc, NIHIL, vol, DCCC, DC, hodie_fixa, NIHIL, NIHIL))
    {
        redde FALSUM;
    }
    kalendarium_conspectum_ponere(m, repertorium);
    *d = dispensator_creare(p, in, repo, m->actiones,
        kalendarium_componere, m, CCC);
    si (!*d)
    {
        redde FALSUM;
    }
    dispensator_motus(*d)->ramus = m->ramus;
    dispensator_recomponere(*d);
    redde VERUM;
}

s32
principale (
               s32   argc,
         character** argv)
{
                 Piscina* p;
     InternamentumChorda* in;
                 Volumen* vol;
        AgendaDocumentum* doc;
     KalendariumMontatio  hebdomas;
     KalendariumMontatio  mensis;
             Dispensator* dh;
             Dispensator* dm;
                     i32  numerator;
                     i32  mutationes;
                     i32  i;
                  chorda  id;
                    Dies  d;
                 clock_t  initium;
                  duplex  libri;
                     Xar* lineae;
               character  praefixum[XXXII];
              DiesFontes  fontes;
     ConspectusRegistrum* r;
                     Xar* vitia;

    mutationes = argc > I ? (i32)atoi(argv[I]) : (i32)L;
    thema_initiare();
    p    = piscina_generare_dynamicum("kalendarium_mensura", CCLVI * M);
    in   = internamentum_creare(p);
    vol  = volumen_temporarium(p, "kalendarium_mensura");
    si (!vol)
    {
        imprimere("volumen temporarium deest\n");
        redde I;
    }
    numerator  = ZEPHYRUM;
    doc        = agenda_aperire(p, in, vol, "agenda", ZEPHYRUM);
    agenda_monetam_ponere(doc, moneta_numerator, &numerator);
    per (i = ZEPHYRUM; i < (i32)L; i++)
    {
        id = agenda_rem_creare(doc, chorda_ex_literis("res agenda", p));
        d = fasti_dies(MMXXVI, X, (s32)(VIII + i % XIV));
        (vacuum)agenda_diem_ponere(doc, id, &d);
    }
    per (i = ZEPHYRUM; i < VII; i++)
    {
        si (!librum_scribere(p, in, vol, fasti_dies(MMXXVI, X,
                (s32)(XI + i)), mutationes))
        {
            imprimere("liber non scriptus\n");
            volumen_claudere(vol);
            redde I;
        }
    }
    /* D4c: registrum commune ut in vico (unum per volumen) */
    memset(&fontes, ZEPHYRUM, magnitudo(fontes));
    fontes.agenda = doc;
    fontes.liturgia = calendarium_creare(p);
    fontes.volumen = vol;
    fontes.intern = in;
    r = conspectus_registrum_creare(p, in, vol, ZEPHYRUM);
    si (   !r || !conspectus_dies_registrare(r, &fontes, &vitia)
        || !conspectus_parare(r, &vitia))
    {
        imprimere("registrum fractum\n");
        volumen_claudere(vol);
        redde I;
    }
    si (   !montare(&hebdomas, &dh, p, in, vol, doc, r,
               "calendarium/hebdomas/2026-10-14")
        || !montare(&mensis, &dm, p, in, vol, doc, r,
               "calendarium/mensis/2026-10-14"))
    {
        imprimere("montatio fracta\n");
        volumen_claudere(vol);
        redde I;
    }
    si (!componens_invenire_per_id(dispensator_arbor(dh),
            chorda_ex_literis("2026-10-11", p)))
    {
        imprimere("visus hebdomadis non apertus\n");
        volumen_claudere(vol);
        redde I;
    }

    initium = clock();
    per (i = ZEPHYRUM; i < PICTURAE; i++)
    {
        s32 k;

        per (k = ZEPHYRUM; k < VII; k++)
        {
            sprintf(praefixum, "dies/2026-10-%02d", (integer)(XI + k));
            lineae = xar_creare(p, (i32)magnitudo(chorda));
            (vacuum)scriba_liber_lineas_legere(p, in, vol, praefixum,
                ZEPHYRUM, lineae);
        }
    }
    libri = tempus_transactum(initium) / PICTURAE;

    imprimere("res %u, libri VII x mutationes %u, acta %ld\n",
        (insignatus integer)L, (insignatus integer)mutationes,
        (longus)volumen_summa_actorum(vol));
    imprimere("hebdomas: %.3f ms per picturam\n",
        picturas_metiri(&hebdomas, dh, p, in));
    imprimere("  libri VII soli: %.3f ms\n", libri);
    imprimere("mensis:   %.3f ms per picturam\n",
        picturas_metiri(&mensis, dm, p, in));
    conspectus_registrum_destruere(r);
    volumen_claudere(vol);
    redde ZEPHYRUM;
}
