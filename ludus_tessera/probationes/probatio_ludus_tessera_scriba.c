/* probatio_ludus_tessera_scriba.c - scriba verus per glutinum
 * tesserae, sine terminali (scriba-plan S3; exemplar A4 pictoris)
 *
 * Compositio EADEM ac principalia (scriba_applicatio; volumina
 * temporaria, canones e radice). Sessio fenestrae notata: insertio,
 * pausa (frustum servatur), insertio, Esc, u, Ctrl-R - per
 * manus_ludus_scribere (clavis + textus, ut fons verus). Eadem notata
 * per terminalem iterantur: codificator -> octeti -> rivus ->
 * ludus_tessera_tractare. Status idem: sigillum, cursor actorum,
 * acta et coniunctiones, insulae ambae.
 *
 * TRANSITUS: clavis cum textu suo PAR codificatur (codificator_eventa
 * cum II) - aliter littera bis scriberetur; ESC solus post moram
 * rivi exhaustam (rivus_moram) decernitur; pulsus (EVENTUS_NIHIL)
 * octetos non fert - glutino directe traditur (sedes quietis).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "xar.h"
#include "thema.h"
#include "volumen.h"
#include "eventus.h"
#include "insula.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "rivus_terminalis.h"
#include "codificator_terminalis.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include "ludus_tessera_pons.h"
#include "ludus_tessera.h"
#include "scriba_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLUMNAE  LXXX
#define LINEAE    XXX

interior b32
_continet (
                 chorda  captum,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);
    i32 k;

    per (k = ZEPHYRUM; k + n <= captum.mensura; k++)
    {
        si (memcmp(captum.datum + k, literae, (memoriae_index)n)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior chorda
_status (
    ScribaApplicatio* app,
             Piscina* piscina)
{
    chorda c;

    c = chorda_concatenare(insula_scribere(app->repo, INSULA_DURABILIS,
        piscina), insula_scribere(app->repo, INSULA_EPHEMERA, piscina),
        piscina);
    redde chorda_concatenare(c, scriba_documentum_sigillum_hex(app->doc,
        piscina), piscina);
}

interior i32
_acta (
               Volumen* vol,
    constans character* genus,
               Piscina* piscina)
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

/* Transitus terminalis: codificator -> rivus -> glutinum */
nomen structura {
    RivusTerminalis* rivus;
    CodificatorModi  modi;
       LudusTessera* lt;
            Piscina* piscina;
            Eventus  pendens;      /* clavis exspectans textum suum */
                b32  habet_pendentem;
} Transitus;

interior vacuum
_exhaurire (
     Transitus* t,
           s64  tempus)
{
    Eventus x;

    dum (rivus_eventum_coalitum(t->rivus, tempus, &x))
    {
        ludus_tessera_tractare(t->lt, &x, tempus);
    }
    /* ESC solus: silentium simulatum moram exhaurit */
    si (rivus_mora_ms(t->rivus) > ZEPHYRUM)
    {
        rivus_moram(t->rivus, tempus + rivus_mora_ms(t->rivus));
        dum (rivus_eventum_coalitum(t->rivus, tempus, &x))
        {
            ludus_tessera_tractare(t->lt, &x, tempus);
        }
    }
}

interior vacuum
_codificare (
              Transitus* t,
       constans Eventus* eventa,
                    i32  numerus)
{
    ChordaAedificator* a = chorda_aedificator_creare(t->piscina, LXIV);
               chorda  o;

    (vacuum)codificator_eventa(&t->modi, eventa, numerus, a);
    o = chorda_aedificator_finire(a);
    si (o.mensura > ZEPHYRUM)
    {
        (vacuum)rivus_tradere(t->rivus, o.datum, o.mensura);
    }
    _exhaurire(t, eventa[ZEPHYRUM].tempus);
}

interior vacuum
_pendentem_mittere (
    Transitus* t)
{
    si (t->habet_pendentem)
    {
        t->habet_pendentem = FALSUM;
        _codificare(t, &t->pendens, I);
    }
}

interior vacuum
_transire (
              vacuum* ctx,
         Dispensator* d,
    constans Eventus* e)
{
    Transitus* t = (Transitus*)ctx;
      Eventus  par[II];

    (vacuum)d;
    si (   t->habet_pendentem && e->genus == EVENTUS_TEXTUS
        && e->datum.textus.genus == EVENTUS_TEXTUS_COMMISSUM)
    {
        par[ZEPHYRUM]       = t->pendens;
        par[I]              = *e;
        t->habet_pendentem  = FALSUM;
        _codificare(t, par, II);
        redde;
    }
    _pendentem_mittere(t);
    si (e->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t->pendens          = *e;
        t->habet_pendentem  = VERUM;
        redde;
    }
    si (e->genus == EVENTUS_NIHIL)
    {
        ludus_tessera_tractare(t->lt, e, e->tempus);   /* pulsus */
        redde;
    }
    _codificare(t, e, I);
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    InternamentumChorda* intern;
     constans character* radix;
       ScribaApplicatio  f;
       ScribaApplicatio  t;
                Volumen* vol_f;
                Volumen* vol_t;
                    Xar* notata;
             ManusLudus* m;
    TesseraPonsMemoriae* pm;
            TesseraOpus* opus;
           LudusTessera* lt;
              Transitus  tr;
                     i8  intrandi[LUDUS_TESSERA_MODI_MAXIMI];
                     i8  exeundi[LUDUS_TESSERA_MODI_MAXIMI];
                    i32  mi;
                    i32  me;

    piscina =
        piscina_generare_dynamicum("probatio_ludus_tessera_scriba",
        IV * M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    radix = getenv("RHUBARB_RADIX");

    imprimere("\n--- I. sessio fenestrae: scriptio notata ---\n");
    vol_f = volumen_temporarium(piscina, "lt_scriba_f");
    CREDO_VERUM (scriba_applicatio_aedificare(&f, piscina, intern,
        vol_f,
        radix, COLUMNAE * VI, LINEAE * VIII));
    notata = xar_creare(piscina, (i32)magnitudo(EventusNotatum));
    dispensator_notarium_ponere(f.d, notata);
    {
        Eventus e;

        /* magnitudo initialis, ut glutinum fenestrae eam mittit */
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus = EVENTUS_MUTARE_MAGNITUDINEM;
        e.tempus = M - I;
        e.datum.mutare_magnitudinem.latitudo = COLUMNAE * VI;
        e.datum.mutare_magnitudinem.altitudo = LINEAE * VIII;
        dispensator_tractare(f.d, &e);
    }
    m = manus_ludus_creare(piscina, f.d);
    CREDO_VERUM (manus_ludus_scribere(m, "isalve"));
    manus_ludus_exspectare(m, MCC);              /* frustum servatur */
    CREDO_VERUM (manus_ludus_scribere(m, " munde"));
    CREDO_VERUM (manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM (manus_ludus_scribere(m, "u"));
    CREDO_VERUM (manus_ludus_clavem(m, 'r', MOD_IMPERIUM));
    dispensator_notarium_ponere(f.d, NIHIL);
    CREDO_VERUM (scriba_documentum_cursor(f.doc) > ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_acta(vol_f, "mutatio", piscina), II);
    CREDO_AEQUALIS_I32 (_acta(vol_f, "coniunctio", piscina), I);
    CREDO_VERUM (memcmp(scriba_documentum_tabula(f.doc)->cellulae,
        "salve munde", XI) == ZEPHYRUM);

    imprimere("\n--- II. eadem per terminalem (glutinum) ---\n");
    vol_t = volumen_temporarium(piscina, "lt_scriba_t");
    CREDO_VERUM (scriba_applicatio_aedificare(&t, piscina, intern,
        vol_t,
        radix, COLUMNAE * VI, LINEAE * VIII));
    pm    = tessera_pons_memoriae_creare(piscina, COLUMNAE, LINEAE);
    opus  = tessera_aperire(piscina, &pm->pons);
    lt    = ludus_tessera_creare(piscina, t.d, t.figurae, ZEPHYRUM,
        NIHIL,
        NIHIL, opus, VI, VIII);
    CREDO_NON_NIHIL (lt);
    memset(&tr, ZEPHYRUM, magnitudo(Transitus));
    tr.piscina                = piscina;
    tr.lt                     = lt;
    tr.rivus                  = rivus_creare(piscina, VI, VIII);
    tr.modi.mus               = CODIFICATOR_MUS_OMNIS;
    tr.modi.cellula_latitudo  = VI;
    tr.modi.cellula_altitudo  = VIII;
    CREDO_VERUM (ludus_tessera_modos_componere(tr.rivus,
        RIVUS_MODUS_MUS | RIVUS_MODUS_GLUTINUM, intrandi, &mi, exeundi,
        &me));
    (vacuum)manus_ludus_iterare_per(manus_ludus_creare(piscina, t.d),
        notata, MANUS_ITERATIO_SEMANTICA, NIHIL, _transire, &tr);
    _pendentem_mittere(&tr);
    CREDO_AEQUALIS_S64 (scriba_documentum_cursor(t.doc),
        scriba_documentum_cursor(f.doc));
    CREDO_AEQUALIS_I32 (_acta(vol_t, "mutatio", piscina), II);
    CREDO_AEQUALIS_I32 (_acta(vol_t, "coniunctio", piscina), I);
    CREDO_VERUM (chorda_aequalis(_status(&t, piscina),
        _status(&f, piscina)));

    imprimere("\n--- III. quadrum in cellulas ---\n");
    ludus_tessera_quadrum(lt, M * X);
    CREDO_VERUM (tessera_praesentare(opus));
    CREDO_VERUM (_continet(tessera_pons_memoriae_captum(pm),
        "salve munde"));
    CREDO_VERUM (_continet(tessera_pons_memoriae_captum(pm),
        "NORMALIS"));
    /* linea status in linea ULTIMA superficiei */
    CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, ZEPHYRUM,
        LINEAE - I).signum, (i32)'N');

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    volumen_claudere(vol_f);
    volumen_claudere(vol_t);
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
