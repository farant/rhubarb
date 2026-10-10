/* vicus_terminalis.c - vicus in TERMINALI (insula-rami-plan T4)
 *
 * Compositio communis (vicus_applicatio) + glutinum tesserae
 * (ludus_tessera): terminalis terminalem possidet, tessera pingit.
 * Superficies = amplitudo terminalis x modulus VI x VIII; linea
 * tabularum = linea prima. Tabulae ictu solo (Cmd per terminalem
 * non transit; Ctrl-A ad latus focatum).
 * Ctrl-C exit. -fumus: volumen temporarium, XXX quadra, exitus.
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
#include "vicus_applicatio.h"
#include "terminale.h"
#include <stdio.h>

#define QUADRA_FUMI  XXX

/* tabulae vivae (terminale) in omni quadro pulsantur, etiam in fundo
 * (vicus-latera S1c) */
interior b32
vicum_pulsare (
    vacuum* ctx)
{
    redde vicus_pulsare((Vicus*)ctx);
}

/* terminale montatum: ornamenta ut cellulae, non pixela */
interior vacuum
ornamenta_exstinguere (
    VicusLatus* l)
{
    si (l->montata && chorda_aequalis_literis(l->genus, "terminale"))
    {
        ((TerminaleApplicatio*)l->montatio)->ornamenta_pixelorum =
            FALSUM;
    }
}

s32
principale (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
        VicusApplicatio  app;
            TesseraPons  pons;
            TesseraOpus* opus;
           LudusTessera* lt;
                    b32  fumus;
                    s32  exitus;
            VicusTabula* t;
                    i32  k;
                    i32  j;

    si (!terminalis_adest())
    {
        fprintf(stderr, "vicus_terminalis: non terminalis\n");
        redde I;
    }
    piscina = piscina_generare_dynamicum("vicus_terminalis",
        VIII * M * M);
    si (!piscina)
    {
        redde I;
    }
    intern = internamentum_creare(piscina);
    thema_initiare();

    vol = vicus_volumen_aperire(piscina, (s32)argc, argv, &fumus);
    si (!vol)
    {
        fprintf(stderr,
            "vicus_terminalis: volumen aperiri non potuit\n");
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

    si (!vicus_applicatio_aedificare(&app, piscina, intern, vol, NIHIL,
            (i32)tessera_latitudo(opus) * VICUS_CELLULA_LATITUDO,
            (i32)tessera_altitudo(opus) * VICUS_CELLULA_ALTITUDO))
    {
        redde I;
    }
    /* S4: horologium locale in linea tabularum */
    vicus_horologium_ponere(app.vicus, vicus_horologium_locale, NIHIL);
    /* terminalia montata: ornamenta ut cellulae, non pixela (ut
     * terminale_terminalis) - tessellatio lineas in cellulas
     * verteret */
    per (k = ZEPHYRUM; k < vicus_numerus_tabularum(app.vicus); k++)
    {
        t = vicus_tabula(app.vicus, k);
        ornamenta_exstinguere(&t->sinistrum);
        per (j = ZEPHYRUM; j < xar_numerus(t->acervus); j++)
        {
            ornamenta_exstinguere((VicusLatus*)xar_obtinere(t->acervus,
                j));
        }
    }
    lt = ludus_tessera_creare(piscina, app.d, vicus_figurae(app.vicus),
        ZEPHYRUM, vicus_imago_fons, app.vicus, opus,
        VICUS_CELLULA_LATITUDO, VICUS_CELLULA_ALTITUDO);
    si (!lt)
    {
        redde I;
    }
    ludus_tessera_pulsum_ponere(lt, vicum_pulsare, app.vicus);
    exitus = ludus_tessera_currere(lt, fumus ? QUADRA_FUMI : ZEPHYRUM);
    volumen_claudere(vol);
    piscina_destruere(piscina);
    redde exitus;
}
