/* kalendarium.h - calendarium: pagina mensis (agenda-plan A2; genus
 * "calendarium", praefixum kalendarium_ quia calendarium_ liturgici
 * est)
 *
 * Montatio in ramo <calendarium id> per organum (manifestum infixum:
 * canon, domini, status). Status visus in insula (visus persistens,
 * dies_electus transiens - absens = hodie); contentum ex documento
 * agendae (ligamen a parente, commune lateribus). Hodie ex porta
 * (vicus: tempus_* radicis; probationes: dies fixa), numquam ex
 * systemate.
 *
 * ARBOR (visus mensis): radix "kalendarium" (PARTES_TABULA,
 * focusabilis, actio calendarium.agere) > "titulus" (PARTES_TITULUS)
 * + cellulae XLII (PARTES_ITEM; VI hebdomades x VII, dominica prima)
 * quarum id = dies ISO ("2026-10-14"), actio eadem. Cellula prima =
 * dominica ante aut in die I mensis electi.
 *
 * DISPOSITIO (V0b): arbor dispositio.h in cellulis (VI x VIII): radix
 * columna (margo cellulae unius = mensa) > barra (II), titulus (II;
 * dies IV), corpus crescens (mensis: VI x VII crescentes; ordines II
 * + I per notam), prompta (II); fines per dispositio_ad_pixela.
 *
 * MENSA (ut scriba et pictor): radix totum latus tenet, superficies
 * colore COLOR_SUPERFICIES et, dum latus focatum est (titulus radicis
 * "focatum"), exemplar foci; visus omnes intra marginem (VI x VIII
 * pixela, cellula una) componuntur - fines liberorum in spatio
 * radicis margine includunt.
 *
 * FIGURAE (A2b; registrum montationis, spatium lateris): tabula =
 * fundus ET cellulae omnes - figura una, dies liturgici et signa
 * semel per quadrum computantur (cellulae ipsae figuram non habent;
 * figurae purae manent); titulus = mensis et annus (Anglice, ut
 * horologium vici) et tempus liturgicum, deinde litterae dierum (S M
 * T W T F S); in cellula: numerus diei (mensis alieni obscurus,
 * hodie inversus), quadratum coloris liturgici iuxta numerum,
 * festum (titulus brevis: memoria aut maius; dominica nuda nihil) ad
 * latitudinem praecisum, signa rerum infra (quadrata plena = apertae,
 * vacua = perfectae; IV summum, deinde '+'), dies electa margine
 * activo.
 *
 * FIGURAE (visus diei, A3c): titulus = dies plena Anglice ("Wednesday,
 * October 14, 2026"), deinde quadratum coloris et celebratio
 * principalis (titulus plenus), deinde tempus liturgicum; ordo = "[ ]"
 * aut "[x]" et titulus (res alibi assignata obscura, cum die sua),
 * notae infra ("Oct 13  textus"), ordo electus fundo selectionis;
 * prompta (PARTES_CAMPUS) = genus ("new:", "note:", "title:",
 * "date:"), textus et cursor.
 *
 * VISUS DIEI (A3; visus="dies"): radix "kalendarium" (PARTES_TABULA)
 * > "titulus" (PARTES_TITULUS: dies plena, celebratio, tempus) +
 * ordines rerum (PARTES_ITEM, id "r<id>"; ictus eligit): primum res
 * diei assignatae (ordine id), deinde res eo die perfectae sed alibi
 * assignatae; notae sub quaque re. "prompta" (PARTES_CAMPUS) solum
 * dum prompta aperta est (A3b).
 *
 * CLAVES (mensis): h/l dies -1/+1, k/j hebdomas -1/+1, H/L mensis
 * -1/+1 (dies ad finem mensis praecisa), t hodie, Enter diem aperit.
 * BARRA (M1b): supra omnes visus, globuli (PARTES_BOTTONE) "praecedens"
 * "<", "sequens" ">", "hodie" "Today" ad sinistrum (mensis, dies) et
 * "visus-mensis" "visus-dies" "visus-agenda" ad dextrum (visus
 * currens fundo selectionis); ictus mandatum nominatum currit.
 * ICTUS in cellula diem eligit; DUPLEX in cellula diem aperit (M1a).
 * In ordine (dies, agenda): ictus eligit; ictus in capsa "[ ]" rem
 * insuper perficit aut aperit (M1a).
 *
 * CLAVES (dies): j/k res proxima/prior, h/l dies -1/+1, x perficere
 * aut aperire (dies perfectionis = hodie ex porta), </> res ad diem
 * priorem/proximum (visus manet), X delere, u/Ctrl-R revocare/
 * reficere (agenda), n res nova, a nota, c titulus, D dies scripta
 * (vacua = sine die), r rubricae (A4b) - prompta (A3b); Esc ad mensem
 * (aut promptam claudit).
 * Claves omnes per tabulam mandatorum (lib: titulus, descriptio,
 * clavis, visus, requisita); littera cum Cmd aut Ctrl nullum mandatum
 * est. Clavis visus cuius mandatum non praesto: consumpta, nisi Esc.
 * Argumentum "YYYY-MM": dies I eius mensis (aut hodie si in eo);
 * "agenda": visus agendae.
 *
 * VISUS HEBDOMADIS (visus="hebdomas"; globulus "Week" inter Month et
 * Day): hebdomas diei electi, dominica prima - radix > "titulus"
 * ("Week of October 18, 2026") + dies VII (PARTES_ITEM, id = dies
 * ISO, lineae altitudine aequa). In die: linea prima "Wed 14" (hodie
 * inversus), quadratum coloris, celebratio principalis (memoria ad
 * libitum aut maior); deinde natales (fontes dierum), res apertae
 * diei ("[ ] titulus"; perfectae non), lineae libri diei
 * (scriba_liber_lineas_legere - LECTIO SOLA, vacuae omissae; paginae
 * ordine; indentatio communis notarum remota, relativa manet).
 * Plures quam capiuntur: ultima linea "+n" ad dextrum. Electa fundo
 * selectionis. CLAVES: j/k dies +1/-1, J/K hebdomas +1/-1, t
 * hodie, Enter diem aperit (visus diei), Esc ad mensem; < > hebdomas.
 * ICTUS diem eligit, DUPLEX aperit (ut cellula mensis).
 *
 * FIGURAE (visus agendae, A4c): titulus = "Agenda: <n> open" (et
 * "#rubrica" sub filtro); titulus gregis "#rubrica" aut "(none)";
 * ordo = "[ ]" et titulus, dies ad dextrum ("Oct 14" aut "-"); res
 * electa fundo selectionis et notis suis infra.
 *
 * VISUS AGENDAE (A4; visus="agenda", argumentum "agenda", '$agenda'):
 * res apertae (quocumque die, sine die incluse) per rubricas ordine
 * canonico, deinde sine rubrica; intra gregem dies, sine die ultimae;
 * res rubricarum plurium in quoque grege. radix > "titulus" + per
 * gregem "g<k>" (PARTES_INDEX) et ordines "r<id>-<k>" (PARTES_ITEM).
 * Claves ut in visu diei (j/k iterata praetereunt; x rem perficit -
 * abit; n rem SINE die creat, rubrica filtri si posita), sine h/l et
 * </>; f filtrum (prompta; vacuum tollit, rubrica persistens - A4b);
 * Esc filtrum tollit.
 */

#ifndef KALENDARIUM_H
#define KALENDARIUM_H

/* <aedilis corpus="lib/kalendarium.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "fasti.h"
#include "agenda.h"
#include "fasti_fontes.h"
#include "organum_montatio.h"

/* porta 'hodie': VERUM et *hodie posita, aut FALSUM (nulla lectio -
 * tunc dies electa vel I Ianuarii MMXXVI) */
nomen b32 (*KalendariumHodie)(
    vacuum* ctx,
      Dies* hodie);

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
        AgendaDocumentum* documentum;   /* ligamen (a parente) */
        KalendariumHodie  hodie;        /* porta */
                  vacuum* hodie_ctx;
                 Piscina* piscina;
     InternamentumChorda* intern;
                  vacuum* figurae_ctx;   /* A2b: privatum (liturgia) */
} KalendariumMontatio;

/* montare in repositorio dato: id NIHIL = radix; aliter liberum
 * <calendarium id> (creatur si abest). argumentum "YYYY-MM" (mensis),
 * "YYYY-MM-DD" (P6c: visus diei in eo die), "agenda" aut NIHIL.
 * systemata ut pictor (conservatio: visus; NIHIL = nulla). FALSUM si
 * argumentum malum aut ramus recusatur. P6: fontes dierum
 * signatorum (natales ...) - signum in cellula mensis, ordines in visu
 * diei; NIHIL = nulli. */
b32
kalendarium_montare (
     KalendariumMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
      constans character* argumentum,
        AgendaDocumentum* documentum,
    constans FastiFontes* fontes,      /* P6 (NIHIL: nulli) */
                 Volumen* volumen,     /* A6d: libri dierum (NIHIL:
                                        * sine notis) */
                     i32  latitudo,    /* A6e: superficies in
                                        * montatione (0 = ignota) */
                     i32  altitudo,
        KalendariumHodie  hodie,
                  vacuum* hodie_ctx,
         StatusSystemata* systemata);

/* A6e: pulsus (vicus): notae diei refectae (indicium paginae post
 * navigationem in notis); VERUM si mutatum (pingendum) */
b32
kalendarium_reficere (
    KalendariumMontatio* m);

/* A6d: gestus (vicus T3a): notae diei montatae - gestus scribae
 * notarum (liber diei refectus); aliter nullus */
vacuum
kalendarium_gestum_ponere (
     Motus* motus,
    vacuum* ctx);

/* argumentum novum (vicus_acervo_mutare; semitae R4): semita
 * calendarii - URI ("calendarium/dies/2026-10-19") aut cognomen:
 * "YYYY-MM" -> calendarium/mensis/<dies>, "YYYY-MM-DD" ->
 * calendarium/dies/<dies>, "agenda" -> calendarium/agenda. FALSUM si
 * malum (nihil mutatur). */
b32
kalendarium_argumentum_ponere (
    KalendariumMontatio* m,
     constans character* argumentum);

/* semitae R4 (facies.semitam_secuta): post semitam a vico applicatam
 * (historia) liber notarum diem electum sequitur; FALSUM si liber
 * aptari non potuit */
b32
kalendarium_semitam_secuta (
    vacuum* ctx);

/* dies electa: campus dies_electus, absens = hodie */
Dies
kalendarium_dies_electus (
    constans KalendariumMontatio* m);

/* componere hospitis (dispensator.h Componere), ctx =
 * KalendariumMontatio* - purum */
Componens*
kalendarium_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

#endif /* KALENDARIUM_H */
