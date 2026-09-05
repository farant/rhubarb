/* probatio_briar_amalgama.c - Plagula una (-amalgama): fabrica FICTA
 * (fixa/amalgama/fabrica - alpha/beta cum staticis collidentibus,
 * postulata_posix, ordo dependentiae) contra AURUM gamma.c octetim;
 * corpus VERUM (salve, derivatum, punctum) STRUCTURALITER: plagula
 * quaeque clausurae semel per '#line 1', nulla inclusio localis
 * relicta, statica involuta (#define/#undef paria), postulata_posix.h
 * prima, probatio plagula altera; recusationes (vitrea, venditorium);
 * scriptor (superscriptio genitorum solum).
 * Regeneratio auri: BRIAR_AMALGAMA_SCRIBERE=1 cum causa nominata.
 * Nativitas: gamma.c manu compilatum (clang vexilla gamma.c) -> '8'.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "briar_amalgama.h"
#include "briar_arbor.h"
#include "briar_contextus.h"
#include "briar_fabrica.h"
#include "briar_nexus.h"
#include "briar_silva.h"
#include "filum.h"
#include "internamentum.h"
#include "piscina.h"
#include "silex.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* FABRICA_FICTA =
    "briar/probationes/fixa/amalgama/fabrica";
hic_manens constans character* AURUM_GAMMA =
    "briar/probationes/fixa/amalgama/gamma.c";
hic_manens constans character* AREA = "briar/build/amalgama_area";

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior constans character*
_texere (
               Piscina* piscina,
    constans character* a,
    constans character* b,
    constans character* c)
{
    chorda ab = chorda_concatenare(chorda_ex_literis(a, piscina),
        chorda_ex_literis(b, piscina), piscina);

    si (c != NIHIL)
    {
        ab = chorda_concatenare(ab, chorda_ex_literis(c, piscina),
            piscina);
    }
    redde chorda_ut_cstr(ab, piscina);
}

interior BriarFabricaFructus
_fabricare (
               Piscina*  piscina,
   InternamentumChorda*  intern,
    constans SilexFons*  fons,
    constans character*  via,
                   Xar** nexus_out)
{
     BriarFabricaFructus  fructus;
    BriarFabricaOptiones  optiones;
               character* textus;
                     i32  mensura = ZEPHYRUM;
           MateriaNodus* doc;
                    Xar* nexus;
                  chorda octeti;

    memset(&fructus, 0, magnitudo(fructus));
    *nexus_out  = NIHIL;
    textus      = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fructus.causa = chorda_ex_literis("plagula non lecta", piscina);
        redde fructus;
    }
        doc  = briar_arbor_parsare(piscina, textus, mensura);
    nexus    = briar_nexus_texere(piscina, doc, intern);
    (vacuum)briar_contexere(piscina, nexus, NIHIL);
    (vacuum)briar_silvam_texere(piscina, nexus, fons);
    optiones.via_thistle   = via;
    optiones.stampa        = "probatio";
    optiones.fons_titulus  = "probatio";
    octeti.datum           = (i8*)textus;
    octeti.mensura         = mensura;
    fructus = briar_fabricare(piscina, doc, nexus, fons, &optiones,
        octeti);
    *nexus_out = nexus;
    redde fructus;
}

interior constans BriarPlagula*
_plagula (
    constans BriarAmalgamaFructus* f,
               constans character* via)
{
    i32 i;

    si (f->plagulae == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(f->plagulae); i++)
    {
        constans BriarPlagula* p = (constans BriarPlagula*)xar_obtinere(
            f->plagulae, i);

        si (chorda_aequalis_literis(p->via, via))
        {
            redde p;
        }
    }
    redde NIHIL;
}

interior b32
_continet (
               Piscina* piscina,
                chorda  fenum,
    constans character* acus)
{
    redde (b32)(strstr(chorda_ut_cstr(fenum, piscina), acus) != NIHIL);
}

/* index primae occurrentiae aut -I */
interior s32
_index (
               Piscina* piscina,
                chorda  fenum,
    constans character* acus)
{
    constans character* c = chorda_ut_cstr(fenum, piscina);
    constans character* p = strstr(c, acus);

    redde p == NIHIL ? -I : (s32)(p - c);
}

interior i32
_numerare (
               Piscina* piscina,
                chorda  fenum,
    constans character* acus)
{
     constans character* c = chorda_ut_cstr(fenum, piscina);
                    i32  n = ZEPHYRUM;
                 size_t  m = strlen(acus);

    dum ((c = strstr(c, acus)) != NIHIL)
    {
        n = n + I;
        c = c + m;
    }
    redde n;
}

/* '#line 1 "via"\n' semel */
interior vacuum
_semel_probare (
               Piscina* piscina,
                chorda  textus,
                chorda  via)
{
    constans character* acus = _texere(piscina, "#line 1 \"",
        chorda_ut_cstr(via, piscina), "\"\n");
    i32 n = _numerare(piscina, textus, acus);

    si (n != I)
    {
        imprimere("  '%s' %d vicibus\n", acus, (integer)n);
    }
    CREDO_AEQUALIS_I32 (n, I);
}

/* ulla linea inclusionis localis relicta? */
interior b32
_inclusio_relicta (
    chorda textus)
{
    i32 i = ZEPHYRUM;

    dum (i < textus.mensura)
    {
           i32 f = i;
        chorda n;

        dum (f < textus.mensura && (character)textus.datum[f] != '\n')
        {
            f = f + I;
        }
        si (briar_amalgama_inclusio_localis(chorda_sectio(textus, i, f),
            &n))
        {
            imprimere("  inclusio relicta: %.*s\n", (integer)(f - i),
                (constans character*)textus.datum + i);
            redde VERUM;
        }
        i = f + I;
    }
    redde FALSUM;
}

/* omne '#define A A_...' columna 0 habet '#undef A\n' suum */
interior vacuum
_paria_probare (
    Piscina* piscina,
     chorda  textus)
{
    i32 i          = ZEPHYRUM;
    i32 definita   = ZEPHYRUM;
    i32 sine_pari  = ZEPHYRUM;

    dum (i < textus.mensura)
    {
        i32 f = i;

        dum (f < textus.mensura && (character)textus.datum[f] != '\n')
        {
            f = f + I;
        }
        si (   f - i > VIII
            && memcmp(textus.datum + i, "#define ", (size_t)VIII)
                == ZEPHYRUM)
        {
            chorda linea  = chorda_sectio(textus, i + VIII, f);
               i32 s      = ZEPHYRUM;

            dum (s < linea.mensura && (character)linea.datum[s] != ' ')
            {
                s = s + I;
            }
            /* forma 'A A_': involucrum staticum (non macro regionis) */
            si (   s + I < linea.mensura && linea.mensura > s + s + I
                && memcmp(linea.datum + s + I, linea.datum, (size_t)s)
                    == ZEPHYRUM
                && (character)linea.datum[s + s + I] == '_')
            {
                                constans character* nomen_symboli =
                                    chorda_ut_cstr(
                                    chorda_sectio(linea, ZEPHYRUM, s),
                                    piscina);
                constans character* sublatio = _texere(piscina,
                    "#undef ",
                    nomen_symboli, "\n");
                constans character* positio = _texere(piscina,
                    "#define ",
                    nomen_symboli, _texere(piscina, " ", nomen_symboli,
                    "_"));

                /* idem nomen in fontibus pluribus: paria totidem */
                definita = definita + I;
                si (_numerare(piscina, textus, sublatio)
                    != _numerare(piscina, textus, positio))
                {
                    imprimere("  sine pari: %s", sublatio);
                    sine_pari = sine_pari + I;
                }
            }
        }
        i = f + I;
    }
    imprimere("  involucra %d\n", (integer)definita);
    CREDO_VERUM (definita > ZEPHYRUM);
    CREDO_AEQUALIS_I32 (sine_pari, ZEPHYRUM);
}

/* structura amalgamae super corpus verum */
interior vacuum
_structuram_probare (
                          Piscina* piscina,
    constans BriarFabricaFructus* fabrica,
                  constans chorda  textus)
{
    i32 i;
    s32 primum;
    s32 posix;

    per (i = ZEPHYRUM; i < xar_numerus(fabrica->clausura); i++)
    {
        constans SilexRes* r = (constans SilexRes*)xar_obtinere(
            fabrica->clausura, i);

        _semel_probare(piscina, textus, r->via);
    }
    CREDO_FALSUM (_inclusio_relicta(textus));
    _paria_probare(piscina, textus);
    primum  = _index(piscina, textus, "#line 1 \"");
    posix   = _index(piscina, textus,
        "#line 1 \"include/postulata_posix.h\"\n");
    CREDO_VERUM (posix >= ZEPHYRUM);
    CREDO_AEQUALIS_S32 (primum, posix);
    /* linea II = ordo compilationis */
    CREDO_VERUM (_index(piscina, textus, "\n * clang -std=c89 ")
        > ZEPHYRUM);
}

interior vacuum
_aurum_conferre (
       Piscina* piscina,
        chorda  genitum,
    constans character* via_auri)
{
    si (getenv("BRIAR_AMALGAMA_SCRIBERE") != NIHIL)
    {
        si (filum_scribere(via_auri, genitum))
        {
            imprimere("  AURUM SCRIPTUM: %s\n", via_auri);
        }
        alioquin
        {
            CREDO_CULPA ("aurum non scriptum");
        }
        redde;
    }
    {
        chorda aurum = filum_legere_totum(via_auri, piscina);

        CREDO_VERUM (aurum.mensura > ZEPHYRUM);
        si (!chorda_aequalis(aurum, genitum))
        {
            imprimere("  DIFFERT: %s\n--- genitum ---\n%.*s\n"
                "--- aurum ---\n%.*s\n", via_auri,
                (integer)genitum.mensura,
                (constans character*)genitum.datum,
                (integer)aurum.mensura,
                (constans character*)aurum.datum);
        }
        CREDO_CHORDA_AEQUALIS (genitum, aurum);
    }
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    InternamentumChorda* intern;
     constans character* radix;
              SilexFons* fons;
              SilexFons* fons_fictus;

    piscina = piscina_generare_dynamicum("probatio_briar_amalgama",
        134217728);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    silex_monitiones_tacere(VERUM);
    intern  = internamentum_creare(piscina);
    radix   = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    fons         = silex_fons_disci(piscina, radix);
    fons_fictus  = silex_fons_disci(piscina, FABRICA_FICTA);
    CREDO_NON_NIHIL (fons);
    CREDO_NON_NIHIL (fons_fictus);

    imprimere("\n--- Probans inclusio localis (lector linearum) ---\n");
    {
        chorda n;

        CREDO_VERUM (briar_amalgama_inclusio_localis(
            chorda_ex_literis("#include \"chorda.h\"", piscina), &n));
        CREDO_CHORDA_AEQUALIS (n, chorda_ex_literis("chorda.h",
            piscina));
        CREDO_VERUM (briar_amalgama_inclusio_localis(
            chorda_ex_literis("  #  include  \"a/b.h\"  /* c */",
            piscina), &n));
        CREDO_CHORDA_AEQUALIS (n, chorda_ex_literis("a/b.h", piscina));
        CREDO_FALSUM (briar_amalgama_inclusio_localis(
            chorda_ex_literis("#include <stdio.h>", piscina), &n));
        CREDO_FALSUM (briar_amalgama_inclusio_localis(
            chorda_ex_literis("#include \"x.c\"", piscina), &n));
        CREDO_FALSUM (briar_amalgama_inclusio_localis(
            chorda_ex_literis(" * #include \"x.h\"", piscina), &n));
        CREDO_FALSUM (briar_amalgama_inclusio_localis(
            chorda_ex_literis("#define X", piscina), &n));
        CREDO_FALSUM (briar_amalgama_inclusio_localis(
            chorda_ex_literis("", piscina), &n));
    }

    imprimere("\n--- Probans gamma (fabrica ficta): aurum, ordo,"
        " involucra ---\n");
    {
                         Xar* nexus;
         BriarFabricaFructus  fab = _fabricare(piscina, intern,
             fons_fictus,
             "briar/probationes/fixa/amalgama/gamma.thistle", &nexus);
        BriarAmalgamaFructus  am;
       constans BriarPlagula* p;

        si (!fab.successus)
        {
            imprimere("  fabrica: %.*s\n", (integer)fab.causa.mensura,
                (constans character*)fab.causa.datum);
        }
        CREDO_VERUM (fab.successus);
        am = briar_amalgamare(piscina, &fab, fons_fictus,
            "briar/probationes/fixa/amalgama/gamma.thistle");
        si (!am.successus)
        {
            imprimere("  amalgama: %.*s\n", (integer)am.causa.mensura,
                (constans character*)am.causa.datum);
        }
        CREDO_VERUM (am.successus);
        CREDO_AEQUALIS_I32 (xar_numerus(am.plagulae), I);
        p = _plagula(&am, "gamma.c");
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            chorda t = p->contentum;

            _aurum_conferre(piscina, t, AURUM_GAMMA);
            /* statica utriusque fontis, typedef, variabile, macro */
            CREDO_VERUM (_continet(piscina, t,
                "#define _est_spatium _est_spatium_alpha\n"));
            CREDO_VERUM (_continet(piscina, t,
                "#define _est_spatium _est_spatium_beta\n"));
            CREDO_VERUM (_continet(piscina, t,
                "#define Cella Cella_beta\n"));
            CREDO_VERUM (_continet(piscina, t,
                "#define _cella _cella_beta\n"));
            CREDO_FALSUM (_continet(piscina, t, "#define ALPHA_MAXIMUM "
                "ALPHA_MAXIMUM_"));
            CREDO_VERUM (_continet(piscina, t,
                "#undef ALPHA_MAXIMUM\n"));
            CREDO_AEQUALIS_I32 (_numerare(piscina, t,
                "#undef _est_spatium\n"), II);
            /* ordo: posix, latina, alpha.h, beta.h, caput genitum,
             * alpha.c, beta.c, regiones, principale */
            CREDO_VERUM (_index(piscina, t,
                "#line 1 \"include/postulata_posix.h\"")
                < _index(piscina, t, "#line 1 \"include/latina.h\""));
            CREDO_VERUM (_index(piscina, t,
                "#line 1 \"include/latina.h\"")
                < _index(piscina, t, "#line 1 \"include/alpha.h\""));
            CREDO_VERUM (_index(piscina, t,
                "#line 1 \"include/alpha.h\"")
                < _index(piscina, t, "#line 1 \"include/beta.h\""));
            CREDO_VERUM (_index(piscina, t,
                "#line 1 \"include/beta.h\"")
                < _index(piscina, t,
                    "#line 1 \"include/gamma_regiones.h\""));
            CREDO_VERUM (_index(piscina, t,
                "#line 1 \"include/gamma_regiones.h\"")
                < _index(piscina, t, "#line 1 \"lib/alpha.c\""));
            CREDO_VERUM (_index(piscina, t, "#line 1 \"lib/alpha.c\"")
                < _index(piscina, t, "#line 1 \"lib/beta.c\""));
            CREDO_VERUM (_index(piscina, t, "#line 1 \"lib/beta.c\"")
                < _index(piscina, t,
                    "#line 1 \"fontes/gamma_regiones.c\""));
            CREDO_VERUM (_index(piscina, t,
                "#line 1 \"fontes/gamma_regiones.c\"")
                < _index(piscina, t, "#line 1 \"fontes/gamma.c\""));
            /* caput derivatum (beta.h) in capite genito VACUATUM */
            CREDO_FALSUM (_inclusio_relicta(t));
            /* '#line' regionis in .thistle servatur */
            CREDO_VERUM (_continet(piscina, t,
                "#line 8 \"briar/probationes/fixa/amalgama/"
                "gamma.thistle\"\n"));
            _structuram_probare(piscina, &fab, t);
        }
        briar_silvam_solvere(nexus);
    }

    imprimere("\n--- Probans corpus verum: salve (+ probatio),"
        " derivatum, punctum ---\n");
    {
        constans character* fixa[3];
                       i32  k;

        fixa[0] = "salve";
        fixa[1] = "derivatum";
        fixa[2] = "punctum";
        per (k = ZEPHYRUM; k < III; k++)
        {
                             Xar* nexus;
             BriarFabricaFructus  fab = _fabricare(piscina, intern,
                 fons,
                 _texere(piscina, "briar/probationes/fixa/thistle/",
                 fixa[k], ".thistle"), &nexus);
            BriarAmalgamaFructus  am;
           constans BriarPlagula* p;
           constans BriarPlagula* prob;

            imprimere("  %s\n", fixa[k]);
            CREDO_VERUM (fab.successus);
            am = briar_amalgamare(piscina, &fab, fons, fixa[k]);
            CREDO_VERUM (am.successus);
            CREDO_AEQUALIS_I32 (xar_numerus(am.plagulae), II);
            p     = _plagula(&am, _texere(piscina, fixa[k], ".c",
                NIHIL));
            prob  = _plagula(&am, _texere(piscina, "probatio_", fixa[k],
                ".c"));
            CREDO_NON_NIHIL (p);
            CREDO_NON_NIHIL (prob);
            si (p != NIHIL && prob != NIHIL)
            {
                constans character* signum_probationis =
                    _texere(piscina,
                    "#line 1 \"probationes/probatio_", fixa[k],
                    ".c\"\n");
                constans character* signum_principalis =
                    _texere(piscina,
                    "#line 1 \"fontes/", fixa[k], ".c\"\n");

                _structuram_probare(piscina, &fab, p->contentum);
                _structuram_probare(piscina, &fab, prob->contentum);
                CREDO_VERUM (_continet(piscina, p->contentum,
                    signum_principalis));
                CREDO_FALSUM (_continet(piscina, p->contentum,
                    signum_probationis));
                CREDO_VERUM (_continet(piscina, prob->contentum,
                    signum_probationis));
                CREDO_FALSUM (_continet(piscina, prob->contentum,
                    signum_principalis));
                CREDO_VERUM (_continet(piscina, prob->contentum,
                    "#line 1 \"lib/credo.c\"\n"));
                CREDO_VERUM (_continet(piscina, p->contentum,
                    "#define _allocare_interna"
                    " _allocare_interna_piscina\n"));
                CREDO_VERUM (_continet(piscina, p->contentum,
                    "#undef _allocare_interna\n"));
                CREDO_VERUM (_continet(piscina, prob->contentum,
                    "#undef _credo_piscina\n"));
            }
            briar_silvam_solvere(nexus);
        }
    }

    imprimere("\n--- Probans recusationes: vitrea, venditorium ---\n");
    {
                         Xar* nexus;
         BriarFabricaFructus  fab = _fabricare(piscina, intern, fons,
             "briar/probationes/fixa/thistle/salve_vitreum.thistle",
             &nexus);
        BriarAmalgamaFructus am;

        CREDO_VERUM (fab.successus);
        am = briar_amalgamare(piscina, &fab, fons, "salve_vitreum");
        imprimere("  vitrea: %.*s\n", (integer)am.causa.mensura,
            (constans character*)am.causa.datum);
        CREDO_FALSUM (am.successus);
        CREDO_VERUM (_continet(piscina, am.causa, "forma vitrea"));
        CREDO_VERUM (am.plagulae == NIHIL);
        briar_silvam_solvere(nexus);

        fab = _fabricare(piscina, intern, fons,
            "briar/probationes/fixa/thistle/adversa/"
            "amalgama_venditorium.thistle",
            &nexus);
        si (!fab.successus)
        {
            imprimere("  fabrica: %.*s\n", (integer)fab.causa.mensura,
                (constans character*)fab.causa.datum);
        }
        CREDO_VERUM (fab.successus);
        am = briar_amalgamare(piscina, &fab, fons, "venditorium");
        imprimere("  venditorium: %.*s\n", (integer)am.causa.mensura,
            (constans character*)am.causa.datum);
        CREDO_FALSUM (am.successus);
        CREDO_VERUM (_continet(piscina, am.causa,
            "venditorium in clausura: vendor/"));
        briar_silvam_solvere(nexus);
    }

    imprimere("\n--- Probans scriptor: genita superscribitur, aliena"
        " non ---\n");
    {
                         Xar* nexus;
         BriarFabricaFructus  fab = _fabricare(piscina, intern, fons,
             "briar/probationes/fixa/thistle/salve.thistle", &nexus);
        BriarAmalgamaFructus am = briar_amalgamare(piscina, &fab, fons,
            "salve");
                      chorda  causa;
          constans character* via = _texere(piscina, AREA, "/salve.c",
              NIHIL);

        CREDO_VERUM (am.successus);
        CREDO_VERUM (briar_directoria_creare(piscina, AREA));
        (vacuum)filum_delere(via);
        CREDO_VERUM (briar_amalgama_scribere(piscina, &am, AREA,
            &causa));
        CREDO_VERUM (filum_existit(via));
        CREDO_VERUM (filum_existit(_texere(piscina, AREA,
            "/probatio_salve.c", NIHIL)));
        CREDO_CHORDA_AEQUALIS (filum_legere_totum(via, piscina),
            _plagula(&am, "salve.c")->contentum);
        /* iterum: genita superscribitur */
        CREDO_VERUM (briar_amalgama_scribere(piscina, &am, AREA,
            &causa));
        /* aliena: recusatio, intacta */
        CREDO_VERUM (filum_scribere_literis(via, "int main(void){}\n"));
        CREDO_FALSUM (briar_amalgama_scribere(piscina, &am, AREA,
            &causa));
        imprimere("  %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        CREDO_VERUM (_continet(piscina, causa, "NON a briar genita"));
        CREDO_CHORDA_AEQUALIS (filum_legere_totum(via, piscina),
            chorda_ex_literis("int main(void){}\n", piscina));
        (vacuum)filum_delere(via);
        briar_silvam_solvere(nexus);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
