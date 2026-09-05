/* briar_amalgama.c - Vide briar_amalgama.h. */

#include "briar_amalgama.h"
#include "chorda_aedificator.h"
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

/* columna quota (0-basata) lineae per tabulatores */
interior chorda
_columna (
    chorda linea,
       i32 quota)
{
    i32 i;
    i32 initium  = ZEPHYRUM;
    i32 n        = ZEPHYRUM;

    per (i = ZEPHYRUM; i <= linea.mensura; i++)
    {
        si (i == linea.mensura || (character)linea.datum[i] == '\t')
        {
            si (n == quota)
            {
                redde chorda_sectio(linea, initium, i);
            }
            n        = n + I;
            initium  = i + I;
        }
    }
    redde _vacua();
}

b32
briar_amalgama_inclusio_localis (
    chorda  linea,
    chorda* nomen_capitis)
{
    i32 p = ZEPHYRUM;
    i32 initium;

    dum (   p < linea.mensura
         && (linea.datum[p] == ' ' || linea.datum[p] == '\t'))
    {
        p = p + I;
    }
    si (p >= linea.mensura || linea.datum[p] != '#')
    {
        redde FALSUM;
    }
    p = p + I;
    dum (   p < linea.mensura
         && (linea.datum[p] == ' ' || linea.datum[p] == '\t'))
    {
        p = p + I;
    }
    si (   p + VII > linea.mensura
        || memcmp(linea.datum + p, "include", (size_t)VII) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    p = p + VII;
    dum (   p < linea.mensura
         && (linea.datum[p] == ' ' || linea.datum[p] == '\t'))
    {
        p = p + I;
    }
    si (p >= linea.mensura || linea.datum[p] != '"')
    {
        redde FALSUM;
    }
    p        = p + I;
    initium  = p;
    dum (p < linea.mensura && linea.datum[p] != '"')
    {
        p = p + I;
    }
    si (   p                   >= linea.mensura || p - initium < II
        || linea.datum[p - II] != '.' || linea.datum[p - I] != 'h')
    {
        redde FALSUM;
    }
    *nomen_capitis = chorda_sectio(linea, initium, p);
    redde VERUM;
}

/* textus lineatim, inclusiones locales lineis vacuis substitutae;
 * linea ultima semper '\n' terminata */
interior vacuum
_textum_purgatum_appendere (
    ChordaAedificator* a,
               chorda  textus)
{
    i32 i = ZEPHYRUM;

    dum (i < textus.mensura)
    {
           i32 f = i;
        chorda linea;
        chorda ignotum;

        dum (f < textus.mensura && textus.datum[f] != '\n')
        {
            f = f + I;
        }
        linea = chorda_sectio(textus, i, f);
        si (!briar_amalgama_inclusio_localis(linea, &ignotum))
        {
            chorda_aedificator_appendere_chorda(a, linea);
        }
        chorda_aedificator_appendere_literis(a, "\n");
        i = f + I;
    }
}

interior vacuum
_lineam_appendere (
     ChordaAedificator* a,
                chorda  via)
{
    chorda_aedificator_appendere_literis(a, "#line 1 \"");
    chorda_aedificator_appendere_chorda(a, via);
    chorda_aedificator_appendere_literis(a, "\"\n");
}


/* ==================================================
 * Statica: ordines 'lib/' tabulae symbolorum, per plagulam
 * ================================================== */

nomen structura {
    chorda symbolum;
    chorda genus;
} BriarStaticum;

nomen structura {
    TabulaDispersa* per_plagulam;   /* via -> Xar de BriarStaticum */
               b32  adest;          /* tabula lecta */
} BriarStatica;

interior BriarStatica
_statica_legere (
               Piscina* piscina,
    constans SilexFons* fons)
{
    BriarStatica t;
             b32 inventum = FALSUM;
          chorda textus;
             i32 i = ZEPHYRUM;

    t.per_plagulam  = tabula_dispersa_creare_chorda(piscina, 256);
    t.adest         = FALSUM;
    textus = silex_fons_legere(fons, "corpus.symbola.tsv", piscina,
        &inventum);
    si (!inventum || t.per_plagulam == NIHIL)
    {
        redde t;
    }
    t.adest = VERUM;
    dum (i < textus.mensura)
    {
           i32 f = i;
        chorda linea;

        dum (f < textus.mensura && (character)textus.datum[f] != '\n')
        {
            f = f + I;
        }
        linea  = chorda_sectio(textus, i, f);
        i      = f + I;
        si (   linea.mensura             == ZEPHYRUM
            || (character)linea.datum[0] == '#')
        {
            perge;
        }
        {
                   chorda  symbolum  = _columna(linea, ZEPHYRUM);
                   chorda  genus     = _columna(linea, I);
                   chorda  plagula   = _columna(linea, II);
                   vacuum* prior     = NIHIL;
                      Xar* ordines;
            BriarStaticum* s;

            si (   symbolum.mensura == ZEPHYRUM
                || genus.mensura    == ZEPHYRUM
                || !_incipit(plagula, "lib/"))
            {
                perge;
            }
            si (tabula_dispersa_invenire(t.per_plagulam, plagula,
                &prior))
            {
                ordines = (Xar*)prior;
            }
            alioquin
            {
                ordines = xar_creare(piscina,
                    (i32)magnitudo(BriarStaticum));
                tabula_dispersa_inserere(t.per_plagulam, plagula,
                    (vacuum*)ordines);
            }
            s = (BriarStaticum*)xar_addere(ordines);
            si (s != NIHIL)
            {
                s->symbolum  = symbolum;
                s->genus     = genus;
            }
        }
    }
    redde t;
}

/* 'lib/x.c' -> 'x' */
interior chorda
_stirps (
    chorda via)
{
    i32 initium  = ZEPHYRUM;
    i32 finis    = via.mensura;
    i32 i;

    per (i = ZEPHYRUM; i < via.mensura; i++)
    {
        si ((character)via.datum[i] == '/')
        {
            initium = i + I;
        }
    }
    per (i = finis - I; i > initium; i--)
    {
        si ((character)via.datum[i] == '.')
        {
            finis = i;
            frange;
        }
    }
    redde chorda_sectio(via, initium, finis);
}


/* ==================================================
 * Ordo capitum: profunditate prima, post-ordine
 * ================================================== */

nomen structura {
           Piscina* piscina;
    TabulaDispersa* per_viam;   /* via -> SilexRes* (clausura) */
    TabulaDispersa* visa;
               Xar* ordo;       /* constans SilexRes* */
} BriarOrdinatio;

interior vacuum
_caput_visitare (
       BriarOrdinatio* o,
    constans SilexRes* res)
{
    chorda c = res->contentum;
       i32 i = ZEPHYRUM;

    si (tabula_dispersa_continet(o->visa, res->via))
    {
        redde;
    }
    tabula_dispersa_inserere(o->visa, res->via, (vacuum*)o);
    dum (i < c.mensura)
    {
           i32 f = i;
        chorda linea;
        chorda nomen_capitis;

        dum (f < c.mensura && c.datum[f] != '\n')
        {
            f = f + I;
        }
        linea  = chorda_sectio(c, i, f);
        i      = f + I;
        si (briar_amalgama_inclusio_localis(linea, &nomen_capitis))
        {
            chorda via = chorda_concatenare(_literae(o->piscina,
                "include/"), nomen_capitis, o->piscina);
            vacuum* v = NIHIL;

            si (tabula_dispersa_invenire(o->per_viam, via, &v))
            {
                _caput_visitare(o, (constans SilexRes*)v);
            }
        }
    }
    {
        constans SilexRes** cella = (constans SilexRes**)xar_addere(
            o->ordo);

        si (cella != NIHIL)
        {
            *cella = res;
        }
    }
}

/* capita clausurae ordine dependentiae; postulata_posix.h primum */
interior Xar*
_capita_ordinare (
               Piscina* piscina,
                   Xar* clausura,
        TabulaDispersa* per_viam)
{
      BriarOrdinatio  o;
              vacuum* v = NIHIL;
                 i32  i;

    o.piscina   = piscina;
    o.per_viam  = per_viam;
    o.visa      = tabula_dispersa_creare_chorda(piscina, 128);
    o.ordo      = xar_creare(piscina, (i32)magnitudo(SilexRes*));
    si (tabula_dispersa_invenire(per_viam,
        _literae(piscina, "include/postulata_posix.h"), &v))
    {
        _caput_visitare(&o, (constans SilexRes*)v);
    }
    per (i = ZEPHYRUM; i < xar_numerus(clausura); i++)
    {
        constans SilexRes* r = (constans SilexRes*)xar_obtinere(
            clausura, i);

        si (_incipit(r->via, "include/"))
        {
            _caput_visitare(&o, r);
        }
    }
    redde o.ordo;
}

/* fontes lib ordine capitum (gemellus 'lib/<stirps>.c' cuiusque
 * capitis 'include/<stirps>.h' post caput suum - bibliothecae
 * infimae primae, ordo legibilis), reliqui ordine clausurae */
interior Xar*
_fontes_ordinare (
           Piscina* piscina,
               Xar* clausura,
               Xar* capita,
    TabulaDispersa* per_viam)
{
               Xar* ordo = xar_creare(piscina,
                   (i32)magnitudo(SilexRes*));
    TabulaDispersa* visa = tabula_dispersa_creare_chorda(piscina, 128);
               i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(capita); i++)
    {
        constans SilexRes* c =
            *(constans SilexRes**)xar_obtinere(capita,
            i);
        chorda via = chorda_concatenare(chorda_concatenare(
            _literae(piscina, "lib/"), _stirps(c->via), piscina),
            _literae(piscina, ".c"), piscina);
        vacuum* v = NIHIL;

        si (   tabula_dispersa_invenire(per_viam, via, &v)
            && !tabula_dispersa_continet(visa, via))
        {
            constans SilexRes** cella = (constans SilexRes**)xar_addere(
                ordo);

            *cella = (constans SilexRes*)v;
            tabula_dispersa_inserere(visa, via, v);
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(clausura); i++)
    {
        constans SilexRes* r = (constans SilexRes*)xar_obtinere(
            clausura, i);

        si (_incipit(r->via, "lib/") && !tabula_dispersa_continet(visa,
            r->via))
        {
            constans SilexRes** cella = (constans SilexRes**)xar_addere(
                ordo);

            *cella = r;
            tabula_dispersa_inserere(visa, r->via, (vacuum*)cella);
        }
    }
    redde ordo;
}


/* ==================================================
 * Textus amalgamae
 * ================================================== */

interior vacuum
_fontem_emittere (
        ChordaAedificator* a,
        constans SilexRes* res,
    constans BriarStatica* statica)
{
    vacuum* v        = NIHIL;
       Xar* ordines  = NIHIL;
    chorda  stirps   = _stirps(res->via);
       i32  k;

    si (tabula_dispersa_invenire(statica->per_plagulam, res->via, &v))
    {
        ordines = (Xar*)v;
    }
    si (ordines != NIHIL && xar_numerus(ordines) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "/* ");
        chorda_aedificator_appendere_chorda(a, res->via);
        chorda_aedificator_appendere_literis(a,
            ": statica per plagulam renominata */\n");
        per (k = ZEPHYRUM; k < xar_numerus(ordines); k++)
        {
            constans BriarStaticum* s =
                (constans BriarStaticum*)xar_obtinere(ordines, k);

            si (chorda_aequalis_literis(s->genus, "macro"))
            {
                perge;   /* macra: #undef post solum */
            }
            chorda_aedificator_appendere_literis(a, "#define ");
            chorda_aedificator_appendere_chorda(a, s->symbolum);
            chorda_aedificator_appendere_literis(a, " ");
            chorda_aedificator_appendere_chorda(a, s->symbolum);
            chorda_aedificator_appendere_literis(a, "_");
            chorda_aedificator_appendere_chorda(a, stirps);
            chorda_aedificator_appendere_literis(a, "\n");
        }
    }
    _lineam_appendere(a, res->via);
    _textum_purgatum_appendere(a, res->contentum);
    si (ordines != NIHIL)
    {
        per (k = ZEPHYRUM; k < xar_numerus(ordines); k++)
        {
            constans BriarStaticum* s =
                (constans BriarStaticum*)xar_obtinere(ordines, k);

            chorda_aedificator_appendere_literis(a, "#undef ");
            chorda_aedificator_appendere_chorda(a, s->symbolum);
            chorda_aedificator_appendere_literis(a, "\n");
        }
    }
}

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
    _lineam_appendere(a, p->via);
    _textum_purgatum_appendere(a, p->contentum);
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
           constans BriarStatica* statica,
           constans BriarPlagula* caput_genitum,
           constans BriarPlagula* regiones,
           constans BriarPlagula* princeps)
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
        constans SilexRes* r = *(constans SilexRes**)xar_obtinere(
            capita, i);

        _lineam_appendere(a, r->via);
        _textum_purgatum_appendere(a, r->contentum);
    }
        _genitam_emittere(a, caput_genitum);
    per (i = ZEPHYRUM; i < xar_numerus(fontes); i++)
    {
        _fontem_emittere(a, *(constans SilexRes**)xar_obtinere(fontes,
            i), statica);
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
        BriarAmalgamaFructus  f;
              TabulaDispersa* per_viam;
                         Xar* capita;
                         Xar* fontes;
                BriarStatica  statica;
       constans BriarPlagula* caput_genitum;
       constans BriarPlagula* regiones;
       constans BriarPlagula* princeps;
       constans BriarPlagula* probatio;
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
    per_viam = tabula_dispersa_creare_chorda(piscina, 256);
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
        si (!_incipit(r->via, "include/") && !_incipit(r->via, "lib/"))
        {
            _recusare(&f, piscina, _texere(piscina,
                "plagula clausurae ignota: ", chorda_ut_cstr(r->via,
                piscina), NIHIL));
            redde f;
        }
        tabula_dispersa_inserere(per_viam, r->via, (vacuum*)r);
    }
    statica = _statica_legere(piscina, fons);
    si (!statica.adest)
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
        capita      = _capita_ordinare(piscina, fructus->clausura,
            per_viam);
    fontes      = _fontes_ordinare(piscina, fructus->clausura, capita,
        per_viam);
    f.plagulae  = xar_creare(piscina, (i32)magnitudo(BriarPlagula));
    _plagulam_addere(piscina, f.plagulae,
        _texere(piscina, fructus->titulus, ".c", NIHIL),
        _plagulam_fingere(piscina, fructus, via_thistle,
            _texere(piscina, fructus->titulus, ".c", NIHIL),
            fructus->titulus, capita, fontes, &statica, caput_genitum,
            regiones, princeps));
    si (fructus->probatio_adest && probatio != NIHIL)
    {
        constans character* titulus_probationis = _texere(piscina,
            "probatio_", fructus->titulus, NIHIL);

        _plagulam_addere(piscina, f.plagulae,
            _texere(piscina, titulus_probationis, ".c", NIHIL),
                        _plagulam_fingere(piscina, fructus, via_thistle,
                _texere(piscina, titulus_probationis, ".c", NIHIL),
                titulus_probationis, capita, fontes, &statica,
                caput_genitum,
                regiones, probatio));
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
