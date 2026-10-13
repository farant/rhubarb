/* agenda_mensura.c - pretium sigilli agendae (agenda-plan V AUDIENDA;
 * A1b)
 *
 * Omne actum statum canonicum TOTUM scribit et sigillat (historia L1).
 * Hic per ordinem n rerum (quaeque cum die et nota): microsecundae
 * per actum (CC tituli mutati in mensura n) et tempus aperiendi ex
 * volumine (checkpoint + acta). Tempus per clock().
 *
 * Usus: ./bin/aedilis tools/agenda_mensura.c && bash
 *   build/aedilis/agenda_mensura/struere.sh &&
 *   ./build/aedilis/agenda_mensura/agenda_mensura
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "fasti.h"
#include "agenda.h"
#include <stdio.h>
#include <time.h>

#define MUTATIONES CC

interior duplex
tempus_transactum (
    clock_t initium)
{
    redde (duplex)(clock() - initium) * 1.0e3 / CLOCKS_PER_SEC;
}

interior vacuum
ordinem_metiri (
    i32 n)
{
                Piscina* p;
    InternamentumChorda* in;
                Volumen* vol;
       AgendaDocumentum* doc;
       AgendaDocumentum* altera;
                clock_t  initium;
                 duplex  struere;
                 duplex  per_actum;
                 duplex  aperire;
                    i32  i;
                    s32  id;
                   Dies  d;

    p    = piscina_generare_dynamicum("agenda_mensura", CCLVI * M);
    in   = internamentum_creare(p);
    vol  = volumen_temporarium(p, "agenda_mensura");
    doc  = vol ? agenda_aperire(p, in, vol, "", ZEPHYRUM) : NIHIL;
    si (!doc)
    {
        imprimere("%6u  PARARE DEFECIT\n", n);
        redde;
    }
    initium = clock();
    per (i = ZEPHYRUM; i < n; i++)
    {
        id = agenda_rem_creare(doc, chorda_ex_literis("res agenda",
            p));
        d  = fasti_dies(MMXXVI, (s32)(I + i % XII), (s32)(I + i
            % XXVIII));
        (vacuum)agenda_diem_ponere(doc, id, &d);
        (vacuum)agenda_notam_addere(doc, id, d, chorda_ex_literis(
            "nota brevis de re", p));
    }
    struere = tempus_transactum(initium);
    initium = clock();
    per (i = ZEPHYRUM; i < MUTATIONES; i++)
    {
        (vacuum)agenda_titulum_ponere(doc, (s32)(I + i % n),
            chorda_ex_literis((i & I) ? "titulus a" : "titulus b", p));
    }
    per_actum  = tempus_transactum(initium) * 1.0e3 / MUTATIONES;
    initium    = clock();
    altera     = agenda_aperire(p, in, vol, "", ZEPHYRUM);
    aperire    = tempus_transactum(initium);
    imprimere("%6u  res %6u  struere %8.1f ms  per actum %8.1f us  "
        "aperire %7.1f ms\n", n, altera ? agenda_numerus_rerum(altera)
        : ZEPHYRUM, struere, per_actum, aperire);
    volumen_claudere(vol);
    piscina_destruere(p);
}

s32
principale (
    vacuum)
{
    hic_manens constans i32 ordines[] = { C, D, M, MM };
                        i32 i;

    imprimere("     n                                (III acta per rem)"
        "\n");
    per (i = ZEPHYRUM; i < IV; i++)
    {
        ordinem_metiri(ordines[i]);
    }
    redde ZEPHYRUM;
}
