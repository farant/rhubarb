/* probatio_silva_forma.c - formae (fragmenta C cum lacunis):
 * transformare T1 - parsura, lacunae, recusationes nominatae */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "credo.h"
#include "silva_forma.h"
#include "materia_nodus.h"
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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("silva_forma", SECTIONES);
}
