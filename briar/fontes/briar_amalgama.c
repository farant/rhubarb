/* briar_amalgama.c - Vide briar_amalgama.h. */

#include "briar_amalgama.h"
#include "chorda_aedificator.h"
#include "silva_conflatio.h"
#include "filum.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior chorda
_literae (
               Piscina* piscina,
    constans character* literae)
{
    redde chorda_ex_literis(literae, piscina);
}

interior constans character*
_texere (
               Piscina* piscina,
    constans character* a,
    constans character* b,
    constans character* c)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)128);

    chorda_aedificator_appendere_literis(aed, a);
    chorda_aedificator_appendere_literis(aed, b);
    si (c != NIHIL)
    {
        chorda_aedificator_appendere_literis(aed, c);
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}

interior b32
_incipit (
                chorda  c,
    constans character* praefixum)
{
    i32 m = (i32)strlen(praefixum);

    redde (b32)(c.mensura >= m
        && memcmp(c.datum, praefixum, (size_t)m) == ZEPHYRUM);
}

interior b32
_terminatur (
                chorda  c,
    constans character* suffixum)
{
    i32 m = (i32)strlen(suffixum);

    redde (b32)(c.mensura >= m
        && memcmp(c.datum + (c.mensura - m), suffixum, (size_t)m)
            == ZEPHYRUM);
}

interior vacuum
_recusare (
    BriarAmalgamaFructus* f,
                 Piscina* piscina,
      constans character* causa)
{
    f->successus  = FALSUM;
    f->causa      = _literae(piscina, causa);
    f->plagulae   = NIHIL;
}

b32
briar_amalgama_inclusio_localis (
    chorda  linea,
    chorda* nomen_capitis)
{
    redde conflatio_inclusio_localis(linea, nomen_capitis);
}


/* ==================================================
 * Textus amalgamae (mechanismus: silva_conflatio)
 * ================================================== */

interior constans BriarPlagula*
_genita (
    constans BriarFabricaFructus* f,
              constans character* via)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(f->genitae); i++)
    {
        constans BriarPlagula* p = (constans BriarPlagula*)xar_obtinere(
            f->genitae, i);

        si (chorda_aequalis_literis(p->via, via))
        {
            redde p;
        }
    }
    redde NIHIL;
}

interior vacuum
_genitam_emittere (
        ChordaAedificator* a,
    constans BriarPlagula* p)
{
    ConflatioPlagula pl;

    pl.via        = p->via;
    pl.contentum  = p->contentum;
    conflatio_plagulam_emittere(a, &pl);
}

/* membrum bibliothecae in amalgama: plagulae eius genitae */
nomen structura {
    constans BriarMembrum* membrum;
    constans BriarPlagula* caput;    /* include/<m>_regiones.h */
    constans BriarPlagula* corpus;   /* fontes/<m>_regiones.c */
} BriarMembrumAmalgamae;

/* corpus membri: statica OMNIA (functiones, variabilia) suffixo
 * tituli renominata - nulla exceptione, ergo privatum publico alieno
 * aequale (classis Gradus) numquam omittitur */
interior vacuum
_corpus_membri_emittere (
                             Piscina* piscina,
                   ChordaAedificator* a,
      constans BriarMembrumAmalgamae* unitas)
{
                  Xar* nomina   = unitas->membrum->statica;
                  Xar* statica  = NIHIL;
     ConflatioPlagula  pl;
                  i32  k;

    si (nomina != NIHIL)
    {
        statica = xar_creare(piscina,
            (i32)magnitudo(ConflatioStaticum));
        per (k = ZEPHYRUM; k < xar_numerus(nomina); k++)
        {
            constans BriarNomenPublicum* n =
                (constans BriarNomenPublicum*)xar_obtinere(nomina, k);
            ConflatioStaticum* s = (ConflatioStaticum*)xar_addere(
                statica);

            si (s != NIHIL)
            {
                s->symbolum  = n->titulus;
                s->genus     = _vacua();   /* omnia renominantur */
            }
        }
    }
    pl.via        = unitas->corpus->via;
    pl.contentum  = unitas->corpus->contentum;
    conflatio_renominatam_emittere(a, &pl, statica, _literae(piscina,
        unitas->membrum->titulus), "statica membri renominata");
}

/* plagula una: titulus_plagulae 'salve.c' / 'probatio_salve.c',
 * binarium 'salve' / 'probatio_salve', princeps = unitas principalis */
interior chorda
_plagulam_fingere (
                         Piscina* piscina,
    constans BriarFabricaFructus* fructus,
              constans character* via_thistle,
              constans character* titulus_plagulae,
              constans character* binarium,
                             Xar* capita,
                             Xar* fontes,
     constans ConflatioContextus* contextus,
           constans BriarPlagula* caput_genitum,
           constans BriarPlagula* regiones,
           constans BriarPlagula* princeps,
                             Xar* membra)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)65536);
    i32 i;

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus_plagulae);
    chorda_aedificator_appendere_literis(a,
        " - AMALGAMA a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via_thistle);
    chorda_aedificator_appendere_literis(a,
        ": plagula UNA, sola compilanda (effugium):\n * clang ");
    chorda_aedificator_appendere_literis(a,
        briar_fabrica_vexilla(fructus->forma));
    chorda_aedificator_appendere_literis(a, " ");
    chorda_aedificator_appendere_literis(a, titulus_plagulae);
    chorda_aedificator_appendere_literis(a, " -o ");
    chorda_aedificator_appendere_literis(a, binarium);
    chorda_aedificator_appendere_literis(a,
        "\n * Ordo: capita clausurae ordine dependentiae"
        " (postulata_posix.h primum), caput regionum,\n"
        " * fontes bibliothecarum (statica per plagulam renominata:"
        " #define ante, #undef post),\n"
        " * regiones, principale. Inclusiones locales lineis vacuis"
        " substitutae (numeri linearum\n"
        " * servati); '#line 1' plagulam originalem nominat. */\n");
    per (i = ZEPHYRUM; i < xar_numerus(capita); i++)
    {
        conflatio_plagulam_emittere(a,
            *(constans ConflatioPlagula**)xar_obtinere(capita, i));
    }
    si (xar_numerus(membra) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a,
            "/* bibliothecae (membra thistle, post-ordine): capita hic,"
            " corpora post fontes\n * corporis, statica per membrum"
            " renominata */\n");
    }
    per (i = ZEPHYRUM; i < xar_numerus(membra); i++)
    {
        _genitam_emittere(a, ((constans BriarMembrumAmalgamae*)
            xar_obtinere(membra, i))->caput);
    }
        _genitam_emittere(a, caput_genitum);
    per (i = ZEPHYRUM; i < xar_numerus(fontes); i++)
    {
        conflatio_fontem_emittere(a, contextus,
            *(constans ConflatioPlagula**)xar_obtinere(fontes, i));
    }
    per (i = ZEPHYRUM; i < xar_numerus(membra); i++)
    {
        _corpus_membri_emittere(piscina, a,
            (constans BriarMembrumAmalgamae*)xar_obtinere(membra, i));
    }
    _genitam_emittere(a, regiones);
    _genitam_emittere(a, princeps);
    redde chorda_aedificator_finire(a);
}

interior vacuum
_plagulam_addere (
               Piscina* piscina,
                   Xar* plagulae,
    constans character* via,
                chorda  contentum)
{
    BriarPlagula* p = (BriarPlagula*)xar_addere(plagulae);

    si (p != NIHIL)
    {
        p->via        = _literae(piscina, via);
        p->contentum  = contentum;
    }
}


/* ==================================================
 * Amalgamare, scribere
 * ================================================== */

BriarAmalgamaFructus
briar_amalgamare (
                         Piscina* piscina,
    constans BriarFabricaFructus* fructus,
              constans SilexFons* fons,
              constans character* via_thistle)
{
    redde briar_amalgamare_cum_membris(piscina, fructus, fons,
        via_thistle, NIHIL);
}

/* membra -> Xar de BriarMembrumAmalgamae (post-ordine); plagula
 * genita membri ignoti (fabrica cum membris, amalgama sine eis) aut
 * membrum sine plagulis suis -> NIHIL cum causa */
interior Xar*
_membra_amalgamae (
                                Piscina*  piscina,
           constans BriarFabricaFructus*  fructus,
                                    Xar*  membra,
                     constans character** causa)
{
    Xar* ordo_membrorum = xar_creare(piscina,
        (i32)magnitudo(BriarMembrumAmalgamae));
    i32 i;

    *causa = NIHIL;
    per (i = ZEPHYRUM; membra != NIHIL && i < xar_numerus(membra); i++)
    {
        constans BriarMembrum* m = (constans BriarMembrum*)xar_obtinere(
            membra, i);
        BriarMembrumAmalgamae* cella = (BriarMembrumAmalgamae*)
            xar_addere(ordo_membrorum);

        cella->membrum  = m;
        cella->caput    = _genita(fructus, _texere(piscina, "include/",
            m->titulus, "_regiones.h"));
        cella->corpus   = _genita(fructus, _texere(piscina, "fontes/",
            m->titulus, "_regiones.c"));
        si (cella->caput == NIHIL || cella->corpus == NIHIL)
        {
            *causa = _texere(piscina,
                "plagulae genitae membri desunt: ",
                m->titulus, NIHIL);
            redde NIHIL;
        }
    }
    /* fontes/<x>_regiones.c alienus (non radix, non membrum datum) */
    per (i = ZEPHYRUM; i < xar_numerus(fructus->genitae); i++)
    {
        constans BriarPlagula* p = (constans BriarPlagula*)xar_obtinere(
            fructus->genitae, i);
                        chorda titulus;
                           b32 notum;
                           i32 k;

        si (   !_incipit(p->via, "fontes/")
            || !_terminatur(p->via, "_regiones.c"))
        {
            perge;
        }
        titulus  = chorda_sectio(p->via, VII, p->via.mensura - XI);
        notum    = chorda_aequalis_literis(titulus, fructus->titulus);
        per (k = ZEPHYRUM; !notum
            && k < xar_numerus(ordo_membrorum); k++)
        {
            constans BriarMembrumAmalgamae* u =
                (constans BriarMembrumAmalgamae*)xar_obtinere(
                ordo_membrorum, k);

            notum = chorda_aequalis_literis(titulus,
                u->membrum->titulus);
        }
        si (!notum)
        {
            *causa = _texere(piscina, "membrum '", chorda_ut_cstr(
                titulus, piscina),
                "' in fabrica sed amalgamae non datum -"
                " briar_amalgamare_cum_membris");
            redde NIHIL;
        }
    }
    redde ordo_membrorum;
}

BriarAmalgamaFructus
briar_amalgamare_cum_membris (
                         Piscina* piscina,
    constans BriarFabricaFructus* fructus,
              constans SilexFons* fons,
              constans character* via_thistle,
                             Xar* membra)
{
        BriarAmalgamaFructus  f;
          ConflatioContextus  contextus;
                         Xar* clausura;
                         Xar* capita;
                         Xar* fontes;
                      chorda  tabula;
                         b32  inventum = FALSUM;
       constans BriarPlagula* caput_genitum;
       constans BriarPlagula* regiones;
       constans BriarPlagula* princeps;
       constans BriarPlagula* probatio;
                         Xar* ordo_membrorum;
          constans character* causa_membrorum;
                         i32  i;

    memset(&f, 0, magnitudo(f));
    si (   piscina == NIHIL || fructus == NIHIL || fons == NIHIL
        || !fructus->successus || fructus->clausura == NIHIL
        || fructus->genitae == NIHIL)
    {
        _recusare(&f, piscina, "fructus fabricae non sanus");
        redde f;
    }
    si (fructus->forma == BRIAR_FORMA_VITREA)
    {
        _recusare(&f, piscina,
            "forma vitrea: amalgama nondum (capsula, Objective-C,"
            " frameworks - spec par. 9)");
        redde f;
    }
    contextus.radices_clientium = SILEX_RADICES_CLIENTIUM;
    contextus.statica = NIHIL;
    clausura = xar_creare(piscina, (i32)magnitudo(ConflatioPlagula));
    per (i = ZEPHYRUM; i < xar_numerus(fructus->clausura); i++)
    {
                SilexRes* r = (SilexRes*)xar_obtinere(fructus->clausura,
                    i);

        si (_incipit(r->via, "vendor/"))
        {
            _recusare(&f, piscina, _texere(piscina,
                "venditorium in clausura: ", chorda_ut_cstr(r->via,
                piscina),
                " (sub vexillis severis non compilat) - amalgama v1"
                " recusat"));
            redde f;
        }
        si (_terminatur(r->via, ".m"))
        {
            _recusare(&f, piscina, _texere(piscina,
                "Objective-C in clausura: ", chorda_ut_cstr(r->via,
                piscina),
                " - amalgama v1 recusat"));
            redde f;
        }
        si (   !conflatio_caput_est(&contextus, r->via)
            && !conflatio_fons_est(&contextus, r->via))
        {
            _recusare(&f, piscina, _texere(piscina,
                "plagula clausurae ignota: ", chorda_ut_cstr(r->via,
                piscina), NIHIL));
            redde f;
        }
        {
            ConflatioPlagula* pl = (ConflatioPlagula*)xar_addere(
                clausura);

            pl->via        = r->via;
            pl->contentum  = r->contentum;
        }
    }
    tabula = silex_fons_legere(fons, "corpus.symbola.tsv", piscina,
        &inventum);
    si (   !inventum || !conflatio_statica_legere(piscina, tabula,
        &contextus))
    {
        _recusare(&f, piscina,
            "corpus.symbola.tsv in fonte deest: statica renominari"
            " nequeunt");
        redde f;
    }
    caput_genitum = _genita(fructus, _texere(piscina, "include/",
        fructus->titulus, "_regiones.h"));
    regiones = _genita(fructus, _texere(piscina, "fontes/",
        fructus->titulus, "_regiones.c"));
    princeps = _genita(fructus, _texere(piscina, "fontes/",
        fructus->titulus, ".c"));
    probatio = _genita(fructus, _texere(piscina,
        "probationes/probatio_", fructus->titulus, ".c"));
    si (   caput_genitum == NIHIL || regiones == NIHIL
        || princeps      == NIHIL)
    {
        _recusare(&f, piscina, "plagulae genitae fabricae desunt");
        redde f;
    }
    ordo_membrorum = _membra_amalgamae(piscina, fructus, membra,
        &causa_membrorum);
    si (ordo_membrorum == NIHIL)
    {
        _recusare(&f, piscina, causa_membrorum);
        redde f;
    }
    capita      = conflatio_capita_ordinare(piscina, &contextus,
        clausura);
    fontes      = conflatio_fontes_ordinare(piscina, &contextus,
        clausura, capita);
    f.plagulae  = xar_creare(piscina, (i32)magnitudo(BriarPlagula));
    _plagulam_addere(piscina, f.plagulae,
        _texere(piscina, fructus->titulus, ".c", NIHIL),
        _plagulam_fingere(piscina, fructus, via_thistle,
            _texere(piscina, fructus->titulus, ".c", NIHIL),
            fructus->titulus, capita, fontes, &contextus, caput_genitum,
            regiones, princeps, ordo_membrorum));
    si (fructus->probatio_adest && probatio != NIHIL)
    {
        constans character* titulus_probationis = _texere(piscina,
            "probatio_", fructus->titulus, NIHIL);

        _plagulam_addere(piscina, f.plagulae,
            _texere(piscina, titulus_probationis, ".c", NIHIL),
                        _plagulam_fingere(piscina, fructus, via_thistle,
                _texere(piscina, titulus_probationis, ".c", NIHIL),
                titulus_probationis, capita, fontes, &contextus,
                caput_genitum,
                regiones, probatio, ordo_membrorum));
    }
    f.successus = VERUM;
    redde f;
}

b32
briar_amalgama_scribere (
                          Piscina* piscina,
    constans BriarAmalgamaFructus* fructus,
               constans character* directorium,
                           chorda* causa)
{
    i32 i;

    *causa = _vacua();
    si (fructus == NIHIL || !fructus->successus || directorium == NIHIL)
    {
        *causa = _literae(piscina, "fructus amalgamae non sanus");
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->plagulae); i++)
    {
        constans BriarPlagula* p = (constans BriarPlagula*)xar_obtinere(
            fructus->plagulae, i);
        constans character* plena = _texere(piscina, directorium, "/",
            chorda_ut_cstr(p->via, piscina));

        si (filum_existit(plena))
        {
                chorda  vetus;
    constans character* signum;

            vetus   = filum_legere_totum(plena, piscina);
            signum  = _texere(piscina, "/* ",
                chorda_ut_cstr(p->via, piscina),
                " - AMALGAMA a briar genitum");

            si (!_incipit(vetus, signum))
            {
                *causa = _literae(piscina, _texere(piscina,
                    "plagula exsistens NON a briar genita, non"
                    " superscribitur: ", plena, " - move eam"));
                redde FALSUM;
            }
        }
        si (!filum_scribere(plena, p->contentum))
        {
            *causa = _literae(piscina, _texere(piscina, "non scripta: ",
                plena, NIHIL));
            redde FALSUM;
        }
    }
    redde VERUM;
}
