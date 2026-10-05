/* scriba_terminalis.c - scriba in TERMINALI (scriba-plan S3)
 *
 * Compositio communis (scriba_applicatio) + glutinum tesserae
 * (ludus_tessera): terminalis terminalem possidet, tessera pingit.
 * Superficies = amplitudo terminalis x modulus VI x VIII; folium
 * maius quam terminalis volvitur (cursor centratus). Ctrl-C exit.
 * -fumus: volumen temporarium, XXX quadra, exitus.
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "terminalis.h"
#include "tessera_pons.h"
#include "tessera_opus.h"
#include "ludus_tessera_pons.h"
#include "ludus_tessera.h"
#include "scriba_applicatio.h"
#include <stdio.h>

#define CELLULA_LATITUDO  VI
#define CELLULA_ALTITUDO  VIII
#define QUADRA_FUMI       XXX

s32
principale (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
       ScribaApplicatio  app;
            TesseraPons  pons;
            TesseraOpus* opus;
           LudusTessera* lt;
                    b32  fumus;
                    s32  exitus;

    si (!terminalis_adest())
    {
        fprintf(stderr, "scriba_terminalis: non terminalis\n");
        redde I;
    }
    piscina = piscina_generare_dynamicum("scriba_terminalis",
        IV * M * M);
    si (!piscina)
    {
        redde I;
    }
    intern = internamentum_creare(piscina);
    thema_initiare();

    vol = scriba_volumen_aperire(piscina, (s32)argc, argv, &fumus);
    si (!vol)
    {
        fprintf(stderr,
            "scriba_terminalis: volumen aperiri non potuit\n");
        redde I;
    }

    ludus_tessera_pontem_initiare(&pons);
    opus = tessera_aperire(piscina, &pons);
    si (!opus)
    {
        redde I;
    }
    tessera_colores_ponere(opus, tessera_colores_ambitus());
    tessera_politicam_ponere(opus, tessera_politica_ambitus());

    si (!scriba_applicatio_aedificare(&app, piscina, intern, vol, NIHIL,
            (i32)tessera_latitudo(opus) * CELLULA_LATITUDO,
            (i32)tessera_altitudo(opus) * CELLULA_ALTITUDO))
    {
        redde I;
    }
    lt = ludus_tessera_creare(piscina, app.d, app.figurae, ZEPHYRUM,
        NIHIL, NIHIL, opus, CELLULA_LATITUDO, CELLULA_ALTITUDO);
    si (!lt)
    {
        redde I;
    }
    exitus = ludus_tessera_currere(lt, fumus ? QUADRA_FUMI : ZEPHYRUM);
    volumen_claudere(vol);
    piscina_destruere(piscina);
    redde exitus;
}
