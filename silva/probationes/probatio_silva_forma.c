/* probatio_silva_forma.c - formae (fragmenta C cum lacunis):
 * transformare T1 - parsura, lacunae, recusationes nominatae */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "credo.h"
#include "silva_forma.h"
#include "materia_nodus.h"
#include "silva_iudicium.h"
#include "silva_emendatio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior SilvaForma*
_forma (
          CredoContextus* c,
      constans character* textus,
         SilvaFormaGenus  genus,
                  chorda* causa)
{
    *causa = chorda_ex_literis("", c->piscina);
    redde silva_formam_parsare(c->piscina, textus, (i32)strlen(textus),
        genus, causa);
}

/* lacuna k: nomen et reliquum */
interior b32
_lacuna (
    constans SilvaForma* f,
                    i32  k,
     constans character* titulus,
                    b32  reliquum)
{
    constans SilvaLacuna* l;

    si (f == NIHIL || k >= xar_numerus(f->lacunae))
    {
        redde FALSUM;
    }
    l = (constans SilvaLacuna*)xar_obtinere(f->lacunae, k);
    redde chorda_aequalis_literis(l->titulus, titulus)
        && l->reliquum == reliquum;
}

/* nodus loci NODUS (NIHIL aliter) */
interior constans SilvaNodus*
_filius (
    constans SilvaNodus* n,
     constans character* locus)
{
    constans SilvaValor* v = silva_nodi_locus(n, locus);

    redde (v != NIHIL && v->genus == MATERIA_VALOR_NODUS)
        ? v->datum.nodus : NIHIL;
}

/* I. expressio cum lacuna: binarium, sinister vocatio, argumentum
 * lacuna 'via'; ZEPHYRUM latinum expansum (folium-integer) */
interior vacuum
_sectio_expressio (
    CredoContextus* c)
{
                 chorda  causa;
             SilvaForma* f;
    constans SilvaNodus* vocatio;
    constans SilvaValor* argumenta;
  constans MateriaValor* primum;
                 chorda  titulus;
                    b32  reliquum = VERUM;

    f = _forma(c, "remove($via) != ZEPHYRUM", SILVA_FORMA_EXPRESSIO,
        &causa);
    CREDO_NECESSE(f != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_literis(
        silva_nodi_genus_titulus(f->nodus), c->piscina), "binarium");
    CREDO_AEQUALIS_I32(xar_numerus(f->lacunae), I);
    CREDO_VERUM(_lacuna(f, ZEPHYRUM, "via", FALSUM));
    vocatio = _filius(f->nodus, "sinister");
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_literis(
        silva_nodi_genus_titulus(vocatio), c->piscina), "vocatio");
    /* argumenta = LISTA (nodi et commata alternantes) */
    argumenta = silva_nodi_locus(vocatio, "argumenta");
    CREDO_NECESSE(argumenta != NIHIL
        && argumenta->genus == MATERIA_VALOR_LISTA
        && materia_valor_lista_numerus(*argumenta) == I);
    primum = materia_valor_lista_obtinere(*argumenta, ZEPHYRUM);
    CREDO_NECESSE(primum != NIHIL
        && primum->genus == MATERIA_VALOR_NODUS);
    CREDO_VERUM(silva_lacuna_est(primum->datum.nodus, &titulus,
        &reliquum));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus, "via");
    CREDO_FALSUM(reliquum);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_literis(
        silva_nodi_genus_titulus(_filius(f->nodus, "dexter")),
        c->piscina), "folium-integer");
    /* folium non lacuna */
    CREDO_FALSUM(silva_lacuna_est(_filius(f->nodus, "dexter"), NIHIL,
        NIHIL));
}

/* II. reliquum '$...', lacuna repetita semel notata, '$' in littera
 * chordae et commentario non lacuna */
interior vacuum
_sectio_lacunae (
    CredoContextus* c)
{
         chorda  causa;
     SilvaForma* f;

    f = _forma(c, "fopen($p, $...r)", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_NECESSE(f != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(f->lacunae), II);
    CREDO_VERUM(_lacuna(f, ZEPHYRUM, "p", FALSUM));
    CREDO_VERUM(_lacuna(f, I, "r", VERUM));

    f = _forma(c, "$a + $a", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_NECESSE(f != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(f->lacunae), I);

    f = _forma(c, "fopen($p, \"$x\") /* $y */", SILVA_FORMA_EXPRESSIO,
        &causa);
    CREDO_NECESSE(f != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(f->lacunae), I);
    CREDO_VERUM(_lacuna(f, ZEPHYRUM, "p", FALSUM));
}

/* III. sententia: 'si' latinum, nodus generis si */
interior vacuum
_sectio_sententia (
    CredoContextus* c)
{
         chorda  causa;
     SilvaForma* f;

    f = _forma(c, "si (remove($v) != ZEPHYRUM) { redde; }",
        SILVA_FORMA_SENTENTIA, &causa);
    CREDO_NECESSE(f != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_literis(
        silva_nodi_genus_titulus(f->nodus), c->piscina), "si");
    f = _forma(c, "(vacuum)remove($v);", SILVA_FORMA_SENTENTIA, &causa);
    CREDO_NECESSE(f != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_literis(
        silva_nodi_genus_titulus(f->nodus), c->piscina),
        "sententia-expressionis");
}

/* IV. recusationes nominatae */
interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    chorda causa;

    CREDO_NIHIL(_forma(c, "f($)", SILVA_FORMA_EXPRESSIO, &causa));
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis(
        "'$' sine nomine", c->piscina)));
    CREDO_NIHIL(_forma(c, "f($...)", SILVA_FORMA_EXPRESSIO, &causa));
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis(
        "'$...' sine nomine", c->piscina)));
    CREDO_NIHIL(_forma(c, "f($a, $...a)", SILVA_FORMA_EXPRESSIO,
        &causa));
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis(
        "eadem ut '$' et '$...'", c->piscina)));
    CREDO_NIHIL(_forma(c, "remove(", SILVA_FORMA_EXPRESSIO, &causa));
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis(
        "non parsatur", c->piscina)));
    CREDO_NIHIL(_forma(c, "remove(", SILVA_FORMA_SENTENTIA, &causa));
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis(
        "non parsatur ut sententia", c->piscina)));
    CREDO_NIHIL(_forma(c, "f($a); g($a);", SILVA_FORMA_SENTENTIA,
        &causa));
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis(
        "plures sententias", c->piscina)));
    CREDO_NIHIL(_forma(c, "si ($a) { redde; }", SILVA_FORMA_EXPRESSIO,
        &causa));
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis(
        "non parsatur ut expressio", c->piscina)));
}

/* captura nominata (NIHIL si absens) */
interior constans SilvaCaptura*
_capta (
                   Xar* capturae,
    constans character* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(capturae); k++)
    {
        constans SilvaCaptura* c = (constans SilvaCaptura*)xar_obtinere(
            capturae, k);

        si (chorda_aequalis_literis(c->titulus, titulus))
        {
            redde c;
        }
    }
    redde NIHIL;
}

/* valor lexematis folii (nodus 'folium-*' cum tok_valor) */
interior chorda
_folium (
    constans SilvaNodus* n,
                Piscina* piscina)
{
    constans SilvaValor* v = silva_nodi_locus(n, "tok_valor");

    redde (v != NIHIL && v->genus == MATERIA_VALOR_TOKEN
           && v->datum.token != NIHIL)
        ? v->datum.token->valor : chorda_ex_literis("", piscina);
}

/* V. congruentia: lacuna capit nodum fontis ipsum; spatia et
 * commentaria neglecta; ZEPHYRUM formae == 0 fontis */
interior vacuum
_sectio_congruentia (
    CredoContextus* c)
{
                 chorda  causa;
             SilvaForma* forma;
             SilvaForma* fons;
                    Xar* capturae = xar_creare(c->piscina,
                        (i32)magnitudo(SilvaCaptura));
    constans SilvaCaptura* via;

    forma = _forma(c, "remove($via) != ZEPHYRUM", SILVA_FORMA_EXPRESSIO,
        &causa);
    fons  = _forma(c, "remove( via_a )!=0 /* nota */",
        SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_NECESSE(forma != NIHIL && fons != NIHIL);
    CREDO_VERUM(silva_forma_congruit(c->piscina, forma, fons->nodus,
        capturae));
    via = _capta(capturae, "via");
    CREDO_NECESSE(via != NIHIL && via->nodus != NIHIL);
    CREDO_FALSUM(via->reliquum);
    CREDO_CHORDA_AEQUALIS_LITERIS(_folium(via->nodus, c->piscina),
        "via_a");
    /* nodus fontis IPSE captus (non copia): argumentum primum
     * vocationis in arbore fontis */
    {
        constans SilvaValor* args = silva_nodi_locus(
            _filius(fons->nodus, "sinister"), "argumenta");
        constans MateriaValor* primum;

        CREDO_NECESSE(args != NIHIL
            && args->genus == MATERIA_VALOR_LISTA);
        primum = materia_valor_lista_obtinere(*args, ZEPHYRUM);
        CREDO_NECESSE(primum != NIHIL
            && primum->genus == MATERIA_VALOR_NODUS);
        CREDO_VERUM(via->nodus == primum->datum.nodus);
    }

    /* non congruit: operator alius, functio alia */
    fons = _forma(c, "remove(x) == 0", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_FALSUM(silva_forma_congruit(c->piscina, forma, fons->nodus,
        capturae));
    fons = _forma(c, "unlink(x) != 0", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_FALSUM(silva_forma_congruit(c->piscina, forma, fons->nodus,
        capturae));
    /* expressio composita in lacuna */
    fons = _forma(c, "remove(via + 1) != 0", SILVA_FORMA_EXPRESSIO,
        &causa);
    CREDO_VERUM(silva_forma_congruit(c->piscina, forma, fons->nodus,
        capturae));
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_literis(
        silva_nodi_genus_titulus(_capta(capturae, "via")->nodus),
        c->piscina), "binarium");
}

/* VI. lacuna iterata: subarbores aequales poscit */
interior vacuum
_sectio_iterata (
    CredoContextus* c)
{
         chorda  causa;
     SilvaForma* forma;
     SilvaForma* fons;

    forma  = _forma(c, "$a + $a", SILVA_FORMA_EXPRESSIO, &causa);
    fons   = _forma(c, "x[i] + x[ i ]", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_NECESSE(forma != NIHIL && fons != NIHIL);
    CREDO_VERUM(silva_forma_congruit(c->piscina, forma, fons->nodus,
        NIHIL));
    fons = _forma(c, "x[i] + x[j]", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_FALSUM(silva_forma_congruit(c->piscina, forma, fons->nodus,
        NIHIL));
}

/* VII. reliquum '$...': unum, nullum (comma quoque), at prima lacuna
 * deesse non potest */
interior vacuum
_sectio_reliquum (
    CredoContextus* c)
{
                 chorda  causa;
             SilvaForma* forma;
             SilvaForma* fons;
                    Xar* capturae = xar_creare(c->piscina,
                        (i32)magnitudo(SilvaCaptura));
    constans SilvaCaptura* r;

    forma = _forma(c, "fopen($p, $...r)", SILVA_FORMA_EXPRESSIO,
        &causa);
    fons  = _forma(c, "fopen(via, \"wb\")", SILVA_FORMA_EXPRESSIO,
        &causa);
    CREDO_NECESSE(forma != NIHIL && fons != NIHIL);
    CREDO_VERUM(silva_forma_congruit(c->piscina, forma, fons->nodus,
        capturae));
    r = _capta(capturae, "r");
    CREDO_NECESSE(r != NIHIL && r->reliquum && r->elementa != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(r->elementa), I);

    fons = _forma(c, "fopen(via)", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_NECESSE(fons != NIHIL);
    CREDO_VERUM(silva_forma_congruit(c->piscina, forma, fons->nodus,
        capturae));
    r = _capta(capturae, "r");
    CREDO_NECESSE(r != NIHIL && r->elementa != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(r->elementa), ZEPHYRUM);

    fons = _forma(c, "fopen()", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_NECESSE(fons != NIHIL);
    CREDO_FALSUM(silva_forma_congruit(c->piscina, forma, fons->nodus,
        capturae));
    fons = _forma(c, "fopen(a, b, d)", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_NECESSE(fons != NIHIL);
    CREDO_VERUM(silva_forma_congruit(c->piscina, forma, fons->nodus,
        capturae));
    CREDO_AEQUALIS_I32(xar_numerus(_capta(capturae, "r")->elementa),
        III);
}

/* VIII. sententia latina contra fontem anglicum; aequalitas pura */
interior vacuum
_sectio_latina (
    CredoContextus* c)
{
         chorda  causa;
     SilvaForma* forma;
     SilvaForma* fons;
     SilvaForma* alter;

    forma = _forma(c, "si (remove($v) != ZEPHYRUM) { redde; }",
        SILVA_FORMA_SENTENTIA, &causa);
    fons  = _forma(c, "if (remove(p) != 0) { return; }",
        SILVA_FORMA_SENTENTIA, &causa);
    CREDO_NECESSE(forma != NIHIL && fons != NIHIL);
    CREDO_VERUM(silva_forma_congruit(c->piscina, forma, fons->nodus,
        NIHIL));
    /* aequalitas: spatia neglecta, nomen aliud non */
    fons   = _forma(c, "f(a ,b)", SILVA_FORMA_EXPRESSIO, &causa);
    alter  = _forma(c, "f( a, b )", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_VERUM(silva_nodi_aequales(fons->nodus, alter->nodus));
    alter = _forma(c, "f(a, c)", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_FALSUM(silva_nodi_aequales(fons->nodus, alter->nodus));
    /* aequalitas non lacunas agnoscit: '$x' contra 'y' non aequales */
    fons   = _forma(c, "f($x)", SILVA_FORMA_EXPRESSIO, &causa);
    alter  = _forma(c, "f(y)", SILVA_FORMA_EXPRESSIO, &causa);
    CREDO_FALSUM(silva_nodi_aequales(fons->nodus, alter->nodus));
}

/* IX. situs (T3): vocationes per consumentem. Recipe examinis
 * (lexicon systematis + latina.h + oraculum) - sine oraculo
 * 'remove(v);' declaratio legeretur. Lineae fixturae numeratae. */
hic_manens constans character* FIXTURA_SITUUM =
    "#include \"latina.h\"\n"                                 /*  1 */
    "#include <stdio.h>\n"                                    /*  2 */
    "#define DELE(v) remove(v)\n"                             /*  3 */
    "#define VOCA(x) x\n"                                     /*  4 */
    "s32 g(s32 a);\n"                                         /*  5 */
    "vacuum f(constans character* v)\n"                       /*  6 */
    "{\n"                                                     /*  7 */
    "    s32 r = remove(v);\n"                                /*  8 */
    "    remove(v);\n"                                        /*  9 */
    "    (vacuum)remove(v);\n"                                /* 10 */
    "    si (remove(v) != ZEPHYRUM) { redde; }\n"             /* 11 */
    "    si (!(remove( v /* nota */ ))) { r = I; }\n"         /* 12 */
    "    r = remove(v);\n"                                    /* 13 */
    "    r = g(remove(v));\n"                                 /* 14 */
    "    dum (remove(v)) { frange; }\n"                       /* 15 */
    "    si (remove(v)) { r = I; } alioquin { r = II; }\n"    /* 16 */
    "    (vacuum)DELE(v);\n"                                  /* 17 */
    "    (vacuum)VOCA(remove(v));\n"                          /* 18 */
    "    (vacuum)g(r);\n"                                     /* 19 */
    "}\n"                                                     /* 20 */
    "s32 h(constans character* v)\n"                          /* 21 */
    "{\n"                                                     /* 22 */
    "    redde remove(v);\n"                                  /* 23 */
    "}\n"                                                     /* 24 */
    "vacuum m(s32 v)\n"                                       /* 25 */
    "{\n"                                                     /* 26 */
    "    ignota(v);\n"                                        /* 27 */
    "}\n";

/* clavis NIHIL = in macro (scriptura = nomen macro) */
nomen structura {
                    i32  linea;
     constans character* clavis;
     constans character* scriptura;
} SitusExspectatus;

hic_manens constans SitusExspectatus SITUS_REMOVE[] = {
    { VIII,   "declaratio|s32 r = remove ( $1 ) ;",
        "s32 r = remove($1);" },
    { IX,     "sententia-expressionis|remove ( $1 ) ;", "remove($1);" },
    { X,      "sententia-expressionis|( void ) remove ( $1 ) ;",
              "(vacuum)remove($1);" },
    { XI,     "binarium|remove ( $1 ) != 0", "remove($1) != ZEPHYRUM" },
    { XII,    "unarium|! ( remove ( $1 ) )", "!(remove( $1 ))" },
    { XIII,   "assignatio|r = remove ( $1 )", "r = remove($1)" },
    { XIV,    "vocatio|g ( remove ( $1 ) )", "g(remove($1))" },
    { XV,     "dum|while ( remove ( $1 ) ) ...",
        "dum (remove($1)) ..." },
    { XVI,    "si|if ( remove ( $1 ) ) ...", "si (remove($1)) ..." },
    { XVII,   NIHIL, "DELE" },
    { XVIII,  NIHIL, "VOCA" },
    { XXIII,  "redde|return remove ( $1 ) ;", "redde remove($1);" }
};

interior b32
_apparatum_parare (
       CredoContextus* c,
    IudiciumApparatus* app)
{
     constans character* radix = getenv("RHUBARB_RADIX");
              character  via[DXII];
          IudiciumCaput* caput;

    si (radix == NIHIL)
    {
        radix = "..";
    }
    app->piscina = c->piscina;
    app->capita = xar_creare(c->piscina,
        (i32)magnitudo(IudiciumCaput));
    sprintf(via, "%s/silva/fontes/systema_c89.h", radix);
    app->fons_iso = silva_iudicium_plagulam_legere(c->piscina, via,
        &app->mensura_iso);
    sprintf(via, "%s/silva/fontes/systema_posix.h", radix);
    app->fons_px = silva_iudicium_plagulam_legere(c->piscina, via,
        &app->mensura_px);
    caput = app->capita != NIHIL
        ? (IudiciumCaput*)xar_addere(app->capita) : NIHIL;
    si (   caput        == NIHIL || app->fons_iso == NIHIL
        || app->fons_px == NIHIL)
    {
        redde FALSUM;
    }
    caput->via = (character*)piscina_allocare(c->piscina, XXXII);
    si (caput->via == NIHIL)
    {
        redde FALSUM;
    }
    strcpy(caput->via, "./include/latina.h");
    sprintf(via, "%s/include/latina.h", radix);
    caput->textus = silva_iudicium_plagulam_legere(c->piscina, via,
        &caput->mensura);
    redde caput->textus != NIHIL;
}

interior vacuum
_sectio_situs (
    CredoContextus* c)
{
    IudiciumApparatus  app;
      IudiciumFructus  fr;
                  Xar* situs;
                  i32  mensura = (i32)strlen(FIXTURA_SITUUM);
                  i32  k;
                  i32  numerus = (i32)(magnitudo(SITUS_REMOVE)
                      / magnitudo(SITUS_REMOVE[ZEPHYRUM]));

    CREDO_NECESSE(_apparatum_parare(c, &app));
    CREDO_NECESSE(silva_iudicium_iudicare(&app, c->piscina,
        "fixa_situum.c", FIXTURA_SITUUM, mensura, &fr));
    situs = silva_situs_invenire(c->piscina, fr.parsura,
        chorda_ex_literis("remove", c->piscina));
    CREDO_NECESSE(situs != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(situs), numerus);
    per (k = ZEPHYRUM; k < numerus && k < xar_numerus(situs); k++)
    {
        constans SilvaSitus* s =
            (constans SilvaSitus*)xar_obtinere(situs,
            k);
                     chorda clavis;
                     chorda scriptura;

        CREDO_AEQUALIS_I32(s->linea, SITUS_REMOVE[k].linea);
        si (SITUS_REMOVE[k].clavis == NIHIL)
        {
            CREDO_VERUM(s->in_macro);
            CREDO_CHORDA_AEQUALIS_LITERIS(s->macro,
                SITUS_REMOVE[k].scriptura);
            CREDO_FALSUM(silva_situs_forma(c->piscina, fr.parsura, s,
                FIXTURA_SITUUM, mensura, &clavis, &scriptura));
            perge;
        }
        CREDO_FALSUM(s->in_macro);
        CREDO_NECESSE(silva_situs_forma(c->piscina, fr.parsura, s,
            FIXTURA_SITUUM, mensura, &clavis, &scriptura));
        CREDO_CHORDA_AEQUALIS_LITERIS(clavis, SITUS_REMOVE[k].clavis);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptura,
            SITUS_REMOVE[k].scriptura);
    }

    /* functio alia: vocatio interior ipsa lacuna fit */
    situs = silva_situs_invenire(c->piscina, fr.parsura,
        chorda_ex_literis("g", c->piscina));
    CREDO_NECESSE(situs != NIHIL && xar_numerus(situs) == II);
    {
        constans SilvaSitus* s =
            (constans SilvaSitus*)xar_obtinere(situs,
            ZEPHYRUM);
                     chorda clavis;
                     chorda scriptura;

        CREDO_NECESSE(silva_situs_forma(c->piscina, fr.parsura, s,
            FIXTURA_SITUUM, mensura, &clavis, &scriptura));
        CREDO_CHORDA_AEQUALIS_LITERIS(clavis,
            "assignatio|r = g ( $1 )");
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptura, "r = g($1)");
    }
    /* ambiguum superstes (functio sine prototypo): lectio canonica
     * (politica X8: expressio) sola ambulatur - lectio prima
     * (declaratio) vocationem non habet */
    situs = silva_situs_invenire(c->piscina, fr.parsura,
        chorda_ex_literis("ignota", c->piscina));
    CREDO_NECESSE(situs != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(situs), I);
    si (xar_numerus(situs) == I)
    {
        constans SilvaSitus* s =
            (constans SilvaSitus*)xar_obtinere(situs,
            ZEPHYRUM);
                     chorda clavis;
                     chorda scriptura;

        CREDO_AEQUALIS_I32(s->linea, XXVII);
        CREDO_NECESSE(silva_situs_forma(c->piscina, fr.parsura, s,
            FIXTURA_SITUUM, mensura, &clavis, &scriptura));
        CREDO_CHORDA_AEQUALIS_LITERIS(clavis,
            "sententia-expressionis|ignota ( $1 ) ;");
        /* congruentia per lectionem canonicam: forma 'ignota($v);'
         * contra nodum ambiguum ipsum */
        {
                 chorda  causa;
             SilvaForma* f = _forma(c, "ignota($v);",
                 SILVA_FORMA_SENTENTIA, &causa);
            constans SilvaNodus* amb = s->vocatio->pater != NIHIL
                ? s->vocatio->pater->pater : NIHIL;

            CREDO_NECESSE(f != NIHIL && amb != NIHIL);
            CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_literis(
                silva_nodi_genus_titulus(amb), c->piscina), "ambiguus");
            CREDO_VERUM(silva_forma_congruit(c->piscina, f, amb,
                NIHIL));
        }
    }
    /* functio absens: nulli situs */
    situs = silva_situs_invenire(c->piscina, fr.parsura,
        chorda_ex_literis("unlink", c->piscina));
    CREDO_NECESSE(situs != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(situs), ZEPHYRUM);
}

/* X. regulae et rescriptio (T4): ordo regularum, ascensus ad
 * sententiam, octeti capturarum, recusationes nominatae */
hic_manens constans character* FIXTURA_REGULARUM =
    "#include \"latina.h\"\n"                                 /*  1 */
    "#include <stdio.h>\n"                                    /*  2 */
    "#define DELE(v) remove(v)\n"                             /*  3 */
    "#define VIA(x) x\n"                                      /*  4 */
    "s32 g();\n"                                              /*  5 */
    "vacuum f(constans character* v, s32 r)\n"                /*  6 */
    "{\n"                                                     /*  7 */
    "    s32 q = remove(v);\n"                                /*  8 */
    "    remove(v);\n"                                        /*  9 */
    "    (vacuum)remove( v /* via */ );\n"                    /* 10 */
    "    si (remove(v) != ZEPHYRUM) { redde; }\n"             /* 11 */
    "    si (remove(v) != 0) { redde; }\n"                    /* 12 */
    "    r = remove(v);\n"                                    /* 13 */
    "    si (remove(VIA(v)) != ZEPHYRUM) { redde; }\n"        /* 14 */
    "    (vacuum)DELE(v);\n"                                  /* 15 */
    "    (vacuum)q;\n"                                        /* 16 */
    "    g(v);\n"                                             /* 17 */
    "    g(v, 1, \"a,b\");\n"                                 /* 18 */
    "}\n"                                                     /* 19 */
    "b32 filum_delere(constans character* v)\n"               /* 20 */
    "{\n"                                                     /* 21 */
    "    remove(v);\n"                                        /* 22 */
    "    redde VERUM;\n"                                      /* 23 */
    "}\n";

hic_manens constans character* FIXTURA_REGULARUM_POST =
    "#include \"latina.h\"\n"
    "#include <stdio.h>\n"
    "#define DELE(v) remove(v)\n"
    "#define VIA(x) x\n"
    "s32 g();\n"
    "vacuum f(constans character* v, s32 r)\n"
    "{\n"
    "    s32 q = remove(v);\n"
    "    (vacuum)filum_delere(v);\n"
    "    (vacuum)remove( v /* via */ );\n"
    "    si (!filum_delere(v)) { redde; }\n"
    "    si (!filum_delere(v)) { redde; }\n"
    "    r = filum_delere(v) ? ZEPHYRUM : -I;\n"
    "    si (remove(VIA(v)) != ZEPHYRUM) { redde; }\n"
    "    (vacuum)DELE(v);\n"
    "    (vacuum)q;\n"
    "    g(v);\n"
    "    g(v, 1, \"a,b\");\n"
    "}\n"
    "b32 filum_delere(constans character* v)\n"
    "{\n"
    "    remove(v);\n"
    "    redde VERUM;\n"
    "}\n";

nomen structura {
                         i32 linea;
    SilvaRescriptioVerdictum verdictum;
                         s32 regula;
} RescriptioExspectata;

interior Xar*
_regulas_parare (
         CredoContextus* c,
    constans character* constans* paria,
                    i32  numerus)
{
    Xar* regulae = xar_creare(c->piscina, (i32)magnitudo(SilvaRegula*));
    i32  k;

    per (k = ZEPHYRUM; regulae != NIHIL && k < numerus; k++)
    {
             chorda  causa;
        SilvaRegula* r = silva_regulam_parare(c->piscina,
            chorda_ex_literis(paria[II * k], c->piscina),
            chorda_ex_literis(paria[II * k + I], c->piscina), &causa);

        si (r == NIHIL)
        {
            redde NIHIL;
        }
        *(SilvaRegula**)xar_addere(regulae) = r;
    }
    redde regulae;
}

interior vacuum
_sectio_regulae (
    CredoContextus* c)
{
    hic_manens constans character* constans REGULAE_REMOVE[] = {
        "(vacuum)remove($v);",    "(vacuum)filum_delere($v);",
        "remove($v);",            "(vacuum)filum_delere($v);",
        "remove($v) != ZEPHYRUM", "!filum_delere($v)",
        "$r = remove($v)",
            "$r = filum_delere($v) ? ZEPHYRUM : -I"
    };
    hic_manens constans character* constans REGULAE_G[] = {
        "g($a, $...r);", "(vacuum)h($a, $...r);"
    };
    hic_manens constans RescriptioExspectata EXSPECTATAE[] = {
        { VIII,  SILVA_RESCRIPTIO_SINE_REGULA, -I },
        { IX,    SILVA_RESCRIPTIO_FACTA,       I },
        { X,     SILVA_RESCRIPTIO_TRIVIA,      ZEPHYRUM },
        { XI,    SILVA_RESCRIPTIO_FACTA,       II },
        { XII,   SILVA_RESCRIPTIO_FACTA,       II },
        { XIII,  SILVA_RESCRIPTIO_FACTA,       III },
        { XIV,   SILVA_RESCRIPTIO_NON_FIDA,    II },
        { XV,    SILVA_RESCRIPTIO_IN_MACRO,    -I },
        { XXII,  SILVA_RESCRIPTIO_RECURSIO,    I }
    };
    IudiciumApparatus  app;
      IudiciumFructus  fr;
                  Xar* regulae;
                  Xar* rescriptiones;
               chorda  causa;
               chorda  novum;
                  i32  mensura = (i32)strlen(FIXTURA_REGULARUM);
                  i32  numerus = (i32)(magnitudo(EXSPECTATAE)
                      / magnitudo(EXSPECTATAE[ZEPHYRUM]));
                  i32 k;

    /* regulae malae: causae nominatae */
    CREDO_VERUM(silva_regulam_parare(c->piscina,
        chorda_ex_literis("remove($v)", c->piscina),
        chorda_ex_literis("f($w)", c->piscina), &causa) == NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(causa, "fac: lacuna non in si: $w");
    CREDO_VERUM(silva_regulam_parare(c->piscina,
        chorda_ex_literis("remove($v", c->piscina),
        chorda_ex_literis("f($v)", c->piscina), &causa) == NIHIL);
    CREDO_VERUM(causa.mensura > IV
        && memcmp(causa.datum, "si: ", IV) == ZEPHYRUM);
    /* sententia nuda 'remove($v);' = lectio canonica vocationis */
    {
        SilvaForma* f = _forma(c, "remove($v);", SILVA_FORMA_SENTENTIA,
            &causa);

        CREDO_NECESSE(f != NIHIL);
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_literis(
            silva_nodi_genus_titulus(f->nodus), c->piscina),
            "sententia-expressionis");
    }

    CREDO_NECESSE(_apparatum_parare(c, &app));
    CREDO_NECESSE(silva_iudicium_iudicare(&app, c->piscina,
        "fixa_regularum.c", FIXTURA_REGULARUM, mensura, &fr));
    regulae = _regulas_parare(c, REGULAE_REMOVE, IV);
    CREDO_NECESSE(regulae != NIHIL);
    rescriptiones = silva_rescriptiones_parare(c->piscina, fr.parsura,
        FIXTURA_REGULARUM, mensura, chorda_ex_literis("remove",
        c->piscina), regulae);
    CREDO_NECESSE(rescriptiones != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(rescriptiones), numerus);
    per (k = ZEPHYRUM; k < numerus
        && k < xar_numerus(rescriptiones); k++)
    {
        constans SilvaRescriptio* r = (constans SilvaRescriptio*)
            xar_obtinere(rescriptiones, k);

        CREDO_AEQUALIS_I32(r->situs.linea, EXSPECTATAE[k].linea);
        CREDO_AEQUALIS_I32((i32)r->verdictum,
            (i32)EXSPECTATAE[k].verdictum);
        CREDO_VERUM(r->regula == EXSPECTATAE[k].regula);
    }
    novum = silva_rescriptiones_applicare(c->piscina, FIXTURA_REGULARUM,
        mensura, rescriptiones);
    CREDO_CHORDA_AEQUALIS_LITERIS(novum, FIXTURA_REGULARUM_POST);

    /* '$...r': nullum (comma quoque sublatum) et plura verbatim */
    regulae = _regulas_parare(c, REGULAE_G, I);
    CREDO_NECESSE(regulae != NIHIL);
    rescriptiones = silva_rescriptiones_parare(c->piscina, fr.parsura,
        FIXTURA_REGULARUM, mensura, chorda_ex_literis("g", c->piscina),
        regulae);
    CREDO_NECESSE(rescriptiones != NIHIL && xar_numerus(rescriptiones)
        == II);
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans SilvaRescriptio*)
        xar_obtinere(rescriptiones, ZEPHYRUM))->textus,
        "(vacuum)h(v);");
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans SilvaRescriptio*)
        xar_obtinere(rescriptiones, I))->textus,
        "(vacuum)h(v, 1, \"a,b\");");
}

/* XI. custodia (T5): iudicium textus novi et clang - fixtura
 * functionem substitutam ipsa declarat (capita: latina.h sola) */
hic_manens constans character* FIXTURA_CUSTODIAE =
    "#include \"latina.h\"\n"
    "#include <stdio.h>\n"
    "interior b32 nova_delere(constans character* v)\n"
    "{\n"
    "    (vacuum)v;\n"
    "    redde VERUM;\n"
    "}\n"
    "interior vacuum f(constans character* v)\n"
    "{\n"
    "    remove(v);\n"
    "    remove(v);\n"
    "    (vacuum)nova_delere(v);\n"
    "}\n"
    "vacuum g(constans character* v);\n"
    "vacuum g(constans character* v) { f(v); }\n";

interior chorda
_custodiae_textus (
        CredoContextus* c,
    constans character* vetus,
    constans character* novum)
{
    redde chorda_substituere(chorda_ex_literis(FIXTURA_CUSTODIAE,
        c->piscina), chorda_ex_literis(vetus, c->piscina),
        chorda_ex_literis(novum, c->piscina), c->piscina);
}

interior vacuum
_sectio_custodia (
    CredoContextus* c)
{
     constans character* radix = getenv("RHUBARB_RADIX");
              character  include[DXII];
     constans character* vexilla[VII];
      IudiciumApparatus  app;
        IudiciumFructus  ante;
     EmendatioVerdictum  v;
                 chorda  novum;
                 chorda  erratum;
                    i32  mensura = (i32)strlen(FIXTURA_CUSTODIAE);

    CREDO_NECESSE(_apparatum_parare(c, &app));
    CREDO_NECESSE(silva_iudicium_iudicare(&app, c->piscina,
        "fixa_custodiae.c", FIXTURA_CUSTODIAE, mensura, &ante));
    /* rescriptio bona: codex 97 exacte II perdit, nihil crescit */
    novum = _custodiae_textus(c, "    remove(v);\n",
        "    (vacuum)nova_delere(v);\n");
    v = silva_emendationem_iudicare(&app, c->piscina,
        "fixa_custodiae.c",
        &ante, (constans character*)chorda_ut_cstr(novum, c->piscina),
        novum.mensura, (s32)EXAMEN_CODEX_SCRIPTURA_PRAETERITA, II);
    CREDO_VERUM(v.bene);
    CREDO_AEQUALIS_I32(v.ante, II);
    CREDO_AEQUALIS_I32(v.post, ZEPHYRUM);
    /* decrementum falsum: nominatur */
    v = silva_emendationem_iudicare(&app, c->piscina,
        "fixa_custodiae.c",
        &ante, (constans character*)chorda_ut_cstr(novum, c->piscina),
        novum.mensura, (s32)EXAMEN_CODEX_SCRIPTURA_PRAETERITA, I);
    CREDO_FALSUM(v.bene);
    CREDO_VERUM(chorda_incipit(v.causa, chorda_ex_literis("codex 97:",
        c->piscina)));
    /* numerus argumentorum falsus: codex alius crescit */
    novum = _custodiae_textus(c, "    remove(v);\n",
        "    (vacuum)nova_delere(v, I);\n");
    v = silva_emendationem_iudicare(&app, c->piscina,
        "fixa_custodiae.c",
        &ante, (constans character*)chorda_ut_cstr(novum, c->piscina),
        novum.mensura, (s32)EXAMEN_CODEX_SCRIPTURA_PRAETERITA, II);
    CREDO_FALSUM(v.bene);
    CREDO_VERUM(chorda_continet(v.causa, chorda_ex_literis("crevit",
        c->piscina)));
    /* syntaxis fracta */
    novum = _custodiae_textus(c, "    remove(v);\n", "    remove(v;\n");
    v = silva_emendationem_iudicare(&app, c->piscina,
        "fixa_custodiae.c",
        &ante, (constans character*)chorda_ut_cstr(novum, c->piscina),
        novum.mensura, -I, ZEPHYRUM);
    CREDO_FALSUM(v.bene);

    /* clang: textus bonus tacet, numerus argumentorum falsus non */
    si (radix == NIHIL)
    {
        radix = "..";
    }
    sprintf(include, "-I%s/include", radix);
    vexilla[ZEPHYRUM]  = "-std=c89";
    vexilla[I]         = "-pedantic";
    vexilla[II]        = "-Wall";
    vexilla[III]       = "-Werror";
    vexilla[IV]        = "-Wno-long-long";
    vexilla[V]         = include;
    vexilla[VI]        = NIHIL;
    CREDO_VERUM(silva_emendationem_compilare(c->piscina,
        "fixa_custodiae.c",
        FIXTURA_CUSTODIAE, mensura, vexilla, &erratum));
    novum = _custodiae_textus(c, "    remove(v);\n",
        "    (vacuum)nova_delere(v, I);\n");
    CREDO_FALSUM(silva_emendationem_compilare(c->piscina,
        "fixa_custodiae.c", (constans character*)novum.datum,
        novum.mensura, vexilla, &erratum));
    CREDO_VERUM(chorda_continet(erratum, chorda_ex_literis("arguments",
        c->piscina)));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. expressio cum lacuna", _sectio_expressio, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "II. lacunae: reliquum, repetita, litterae", _sectio_lacunae,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "III. sententia", _sectio_sententia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "IV. recusationes nominatae", _sectio_recusationes, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "V. congruentia et captura", _sectio_congruentia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "VI. lacuna iterata", _sectio_iterata, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "VII. reliquum '$...'", _sectio_reliquum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "VIII. latina et aequalitas", _sectio_latina, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "IX. situs per consumentem", _sectio_situs, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "X. regulae et rescriptio", _sectio_regulae, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "XI. custodia rescriptionis", _sectio_custodia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("silva_forma", SECTIONES);
}
