/* probatio_silva_constructio.c - Constructio generata (Phase 3 Chunk D)
 *
 * Forma annotationum v0 EXSEQUITUR: silva_sceletum_construere manu
 * agitur (nullus GLR adhuc - Phase 4 eum nectet) per AMBAS lectiones
 * "foo * bar ;" - expressionem et declarationem - quas Phase 4 in
 * AMBIGUUS colliget. Registrum (S20 unificatio + errores), accessores
 * probati, children(), listae, et violationes S32.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_sceleti.h"
#include "silva_generare.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA 1024

interior character*
_plagulam_legere (Piscina* piscina, constans character* via)
{
    FILE* pl;
    character* buffer;
    signatus longus mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);
    buffer = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer != NIHIL && mensura > 0L)
    {
        si (fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
        {
            fclose(pl);
            redde NIHIL;
        }
    }
    fclose(pl);
    si (buffer != NIHIL)
    {
        buffer[mensura] = '\0';
    }
    redde buffer;
}

/* Productio cocta per id */
interior s32
_prod_per_id (constans character* id)
{
    i32 i;

    per (i = ZEPHYRUM; i < SILVA_SCELETUM_TABULA.numerus_productionum; i++)
    {
        constans character* pid = SILVA_SCELETUM_TABULA.productiones[i].id;

        si (pid != NIHIL && strcmp(pid, id) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* Symbolum coctum per titulum */
interior s32
_sym_per_titulum (constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < SILVA_SCELETUM_TABULA.numerus_symbolorum; i++)
    {
        si (strcmp(SILVA_SCELETUM_TABULA.symbola[i].titulus, titulus)
            == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* Productio transitus (id NIHIL) per LHS + longitudinem */
interior s32
_prod_transitus (constans character* lhs, i32 longitudo)
{
    s32 lhs_idx = _sym_per_titulum(lhs);
    i32 i;

    per (i = ZEPHYRUM; i < SILVA_SCELETUM_TABULA.numerus_productionum; i++)
    {
        constans SilvaTabProductio* p =
            &SILVA_SCELETUM_TABULA.productiones[i];

        si (p->id == NIHIL && p->sinistrum == lhs_idx
            && p->longitudo == longitudo)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    InternamentumChorda* intern;
    SilvaToken* tok_foo;
    SilvaToken* tok_star;
    SilvaToken* tok_bar;
    SilvaToken* tok_semi;

    piscina = piscina_generare_dynamicum("probatio_silva_constructio",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* Lexemata vera pro constructione */
    {
        Xar* lexemata = silva_lexare(piscina, "foo * bar ;",
            (i32)strlen("foo * bar ;"), ZEPHYRUM);

        tok_foo = *(SilvaToken**)xar_obtinere(lexemata, 0);
        tok_star = *(SilvaToken**)xar_obtinere(lexemata, I);
        tok_bar = *(SilvaToken**)xar_obtinere(lexemata, II);
        tok_semi = *(SilvaToken**)xar_obtinere(lexemata, III);
    }


    /* ========================================================
     * PROBARE: registrum recens (S20 unificatio)
     * ======================================================== */

    {
        constans character* radix;
        character via[VIA_MAXIMA];
        character* fons;
        SilvaGenGrammatica* g;
        Xar* genera;

        imprimere("\n--- Probans registrum (S20) ---\n");

        radix = getenv("RHUBARB_RADIX");
        si (radix == NIHIL)
        {
            radix = "..";
        }
        sprintf(via, "%s/silva/grammatica/sceletum.stml", radix);
        fons = _plagulam_legere(piscina, via);
        g = (fons != NIHIL)
            ? silva_gen_grammaticam_legere(piscina, intern, fons) : NIHIL;
        CREDO_NON_NIHIL (g);

        genera = silva_gen_registrum_computare(g);
        CREDO_NON_NIHIL (genera);
        /* 10 genera grammaticae + 3 extra */
        CREDO_AEQUALIS_I32 (xar_numerus(genera), XV);

        /* binarium: loci unificati trans expressio-plus/terminus-star = 3 */
        {
            i32 i;
            SilvaGenGenusDef* binarium = NIHIL;

            per (i = ZEPHYRUM; i < xar_numerus(genera); i++)
            {
                SilvaGenGenusDef* def =
                    (SilvaGenGenusDef*)xar_obtinere(genera, i);

                si (def->titulus->mensura == VIII
                    && memcmp(def->titulus->datum, "binarium", VIII)
                        == ZEPHYRUM)
                {
                    binarium = def;
                }
            }
            CREDO_NON_NIHIL (binarium);
            CREDO_AEQUALIS_I32 (xar_numerus(binarium->loci), III);
        }
    }


    /* ========================================================
     * PROBARE: conflictus specierum S20 = error generationis
     * ======================================================== */

    {
        SilvaGenGrammatica* g;
        constans character* fons =
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"r\">"
            "<productio genus=\"g\" id=\"g1\">A@x</productio>"
            "<productio genus=\"g\" id=\"g2\">r@x</productio>"
            "</regula>"
            "</regulae><initium>r</initium></grammatica>";

        imprimere("\n--- Probans conflictum S20 ---\n");

        g = silva_gen_grammaticam_legere(piscina, intern, fons);
        CREDO_NON_NIHIL (g);  /* grammatica valida... */
        CREDO_NIHIL (silva_gen_registrum_computare(g));  /* ...registrum non */
    }


    /* ========================================================
     * PROBARE: registrum coctum
     * ======================================================== */

    {
        i32 i;
        s32 binarium_idx;

        imprimere("\n--- Probans registrum coctum ---\n");

        CREDO_AEQUALIS_I32 (SILVA_SCELETUM_REGISTRUM.numerus_generum, XV);

        binarium_idx = -I;
        per (i = ZEPHYRUM; i < SILVA_SCELETUM_REGISTRUM.numerus_generum; i++)
        {
            si (strcmp(SILVA_SCELETUM_REGISTRUM.genera[i].titulus,
                    "binarium") == ZEPHYRUM)
            {
                binarium_idx = (s32)i;
            }
        }
        CREDO_VERUM (binarium_idx >= ZEPHYRUM);
        CREDO_AEQUALIS_I32 (
            SILVA_SCELETUM_REGISTRUM.genera[binarium_idx].loci_numerus, III);
        CREDO_AEQUALIS_S32 ((s32)binarium_idx,
            (s32)SILVA_SCELETUM_GENUS_BINARIUM);

        /* species loci: sinister NODUS, tok_operator TOKEN */
        {
            i32 off = SILVA_SCELETUM_REGISTRUM
                .genera[binarium_idx].loci_offset;

            CREDO_AEQUALIS_I32 ((i32)strcmp(
                SILVA_SCELETUM_REGISTRUM.loci[off].titulus, "sinister"),
                ZEPHYRUM);
            CREDO_AEQUALIS_S32 (SILVA_SCELETUM_REGISTRUM.loci[off].species,
                (s32)SILVA_LOCUS_NODUS);
            CREDO_AEQUALIS_S32 (
                SILVA_SCELETUM_REGISTRUM.loci[off + I].species,
                (s32)SILVA_LOCUS_TOKEN);
        }

        /* genus extra: ambiguus cum canonica:index */
        {
            b32 index_inventus = FALSUM;

            per (i = ZEPHYRUM; i < SILVA_SCELETUM_REGISTRUM.numerus_locorum;
                 i++)
            {
                si (strcmp(SILVA_SCELETUM_REGISTRUM.loci[i].titulus,
                        "canonica") == ZEPHYRUM
                    && SILVA_SCELETUM_REGISTRUM.loci[i].species
                        == (s32)SILVA_LOCUS_INDEX)
                {
                    index_inventus = VERUM;
                }
            }
            CREDO_VERUM (index_inventus);
        }
    }


    /* ========================================================
     * PROBARE: lectio expressionis "foo * bar ;" manu constructa
     * ======================================================== */

    {
        SilvaValor v_foo;
        SilvaValor v_bar;
        SilvaValor v_binarium;
        SilvaValor v_sententia;
        SilvaValor valores[3];

        imprimere("\n--- Probans lectionem expressionis ---\n");

        /* folium-identificator(foo) */
        valores[0] = silva_valor_token(tok_foo);
        v_foo = silva_sceletum_construere(piscina,
            _prod_per_id("factor-identificator"), valores);
        CREDO_AEQUALIS_I32 ((i32)v_foo.genus, (i32)SILVA_VALOR_NODUS);
        CREDO_AEQUALIS_S32 (v_foo.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_FOLIUM_IDENTIFICATOR);

        /* transitus terminus -> factor: valor idem */
        valores[0] = v_foo;
        v_foo = silva_sceletum_construere(piscina,
            _prod_transitus("terminus", I), valores);
        CREDO_AEQUALIS_I32 ((i32)v_foo.genus, (i32)SILVA_VALOR_NODUS);

        /* folium-identificator(bar) */
        valores[0] = silva_valor_token(tok_bar);
        v_bar = silva_sceletum_construere(piscina,
            _prod_per_id("factor-identificator"), valores);

        /* binarium: terminus STAR factor */
        valores[0] = v_foo;
        valores[I] = silva_valor_token(tok_star);
        valores[II] = v_bar;
        v_binarium = silva_sceletum_construere(piscina,
            _prod_per_id("terminus-star"), valores);
        CREDO_AEQUALIS_I32 ((i32)v_binarium.genus, (i32)SILVA_VALOR_NODUS);
        CREDO_AEQUALIS_S32 (v_binarium.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_BINARIUM);

        /* accessores probati */
        {
            SilvaValor sin = silva_sceletum_binarium_sinister(
                v_binarium.datum.nodus);
            SilvaValor op = silva_sceletum_binarium_tok_operator(
                v_binarium.datum.nodus);
            SilvaValor dex = silva_sceletum_binarium_dexter(
                v_binarium.datum.nodus);

            CREDO_AEQUALIS_I32 ((i32)sin.genus, (i32)SILVA_VALOR_NODUS);
            CREDO_AEQUALIS_I32 ((i32)op.genus, (i32)SILVA_VALOR_TOKEN);
            CREDO_CHORDA_AEQUALIS_LITERIS (op.datum.token->valor, "*");
            CREDO_AEQUALIS_I32 ((i32)dex.genus, (i32)SILVA_VALOR_NODUS);

            /* accessor generis alieni: nihil */
            CREDO_AEQUALIS_I32 ((i32)silva_sceletum_declaratio_typus(
                v_binarium.datum.nodus).genus, (i32)SILVA_VALOR_NIHIL);
        }

        /* children(): 2 nodi (token non liber) */
        {
            Xar* liberi = silva_nodus_liberi(piscina,
                v_binarium.datum.nodus);

            CREDO_AEQUALIS_I32 (xar_numerus(liberi), II);
        }

        /* sententia-expressionis + lista */
        valores[0] = v_binarium;
        valores[I] = silva_valor_token(tok_semi);
        v_sententia = silva_sceletum_construere(piscina,
            _prod_per_id("sententia-expr"), valores);
        CREDO_AEQUALIS_I32 ((i32)v_sententia.genus, (i32)SILVA_VALOR_NODUS);

        {
            SilvaValor lista;
            s32 prod_initium = _prod_transitus("sententiae", ZEPHYRUM);

            lista = silva_sceletum_construere(piscina, prod_initium, NIHIL);
            CREDO_AEQUALIS_I32 ((i32)lista.genus, (i32)SILVA_VALOR_LISTA);
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(lista), ZEPHYRUM);

            valores[0] = lista;
            valores[I] = v_sententia;
            lista = silva_sceletum_construere(piscina,
                _prod_per_id("sententiae-appendere"), valores);
            CREDO_AEQUALIS_I32 ((i32)lista.genus, (i32)SILVA_VALOR_LISTA);
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(lista), I);
        }
    }


    /* ========================================================
     * PROBARE: prospectus listarum (A½) - appendere purum,
     * copia-in-divergentia, immunitas prospectuum alienorum
     * ======================================================== */

    {
        SilvaValor basis;
        SilvaValor furca_a;
        SilvaValor furca_b;
        SilvaValor elem_i;
        SilvaValor elem_ii;
        SilvaValor elem_iii;

        imprimere("\n--- Probans prospectus listarum (A-dimidium) ---\n");

        elem_i = silva_valor_token(tok_foo);
        elem_ii = silva_valor_token(tok_bar);
        elem_iii = silva_valor_token(tok_semi);

        /* Via velox: prospectus ad finem vivum -> idem repositorium */
        basis = silva_valor_lista_nova(piscina);
        basis = silva_valor_lista_appendere(piscina, basis, elem_i);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(basis), I);
        {
            SilvaValor porro = silva_valor_lista_appendere(piscina,
                basis, elem_ii);

            /* In loco: idem xar, mensura crescit */
            CREDO_AEQUALIS_PTR (porro.datum.lista.xar,
                basis.datum.lista.xar);
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(porro), II);
            /* Prospectus basis IMMOTUS (mensura sua) */
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(basis), I);

            /* DIVERGENTIA: furca altera ex EODEM prospectu basis
             * appendit - repositorium iam ultra basis scriptum est,
             * ergo copia recens (numerus repositorii = numerator
             * versionum) */
            furca_a = porro;
            furca_b = silva_valor_lista_appendere(piscina, basis,
                elem_iii);
            CREDO_INAEQUALITAS_PTR (furca_b.datum.lista.xar,
                basis.datum.lista.xar);
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(furca_b), II);

            /* Ambae furcae praefixum commune, caudas proprias vident */
            CREDO_AEQUALIS_PTR (
                silva_valor_lista_obtinere(furca_a, ZEPHYRUM)->datum.token,
                silva_valor_lista_obtinere(furca_b, ZEPHYRUM)->datum.token);
            CREDO_AEQUALIS_PTR (
                silva_valor_lista_obtinere(furca_a, I)->datum.token,
                tok_bar);
            CREDO_AEQUALIS_PTR (
                silva_valor_lista_obtinere(furca_b, I)->datum.token,
                tok_semi);
        }

        /* Fines prospectus: extra mensuram -> NIHIL (etiam si
         * repositorium longius est) */
        CREDO_NIHIL (silva_valor_lista_obtinere(basis, I));
        CREDO_NIHIL (silva_valor_lista_obtinere(basis, C));

        /* Inputa mala: non-lista -> nihil-valor / zephyrum / NIHIL */
        {
            SilvaValor malum = silva_valor_lista_appendere(piscina,
                elem_i, elem_ii);

            CREDO_AEQUALIS_I32 ((i32)malum.genus,
                (i32)SILVA_VALOR_NIHIL);
        }
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(elem_i), ZEPHYRUM);
        CREDO_NIHIL (silva_valor_lista_obtinere(elem_i, ZEPHYRUM));
    }


    /* ========================================================
     * PROBARE: lectio declarationis "foo * bar ;" manu constructa
     * ======================================================== */

    {
        SilvaValor v_typus;
        SilvaValor v_declarator;
        SilvaValor v_declaratio;
        SilvaValor valores[3];

        imprimere("\n--- Probans lectionem declarationis ---\n");

        /* typus-nominatus(foo) - foo ut nomen typi (typedef!) */
        valores[0] = silva_valor_token(tok_foo);
        v_typus = silva_sceletum_construere(piscina,
            _prod_per_id("typus-nomen"), valores);
        CREDO_AEQUALIS_S32 (v_typus.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS);

        /* declarator-titulus(bar), tum declarator-monstrator(* bar) */
        valores[0] = silva_valor_token(tok_bar);
        v_declarator = silva_sceletum_construere(piscina,
            _prod_per_id("declarator-nomen"), valores);

        valores[0] = silva_valor_token(tok_star);
        valores[I] = v_declarator;
        v_declarator = silva_sceletum_construere(piscina,
            _prod_per_id("declarator-stella"), valores);
        CREDO_AEQUALIS_S32 (v_declarator.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATOR_MONSTRATOR);

        /* declaratio: typus declarator ; */
        valores[0] = v_typus;
        valores[I] = v_declarator;
        valores[II] = silva_valor_token(tok_semi);
        v_declaratio = silva_sceletum_construere(piscina,
            _prod_per_id("declaratio-simplex"), valores);
        CREDO_AEQUALIS_I32 ((i32)v_declaratio.genus, (i32)SILVA_VALOR_NODUS);
        CREDO_AEQUALIS_S32 (v_declaratio.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO);

        /* accessores + children */
        {
            SilvaValor typus = silva_sceletum_declaratio_typus(
                v_declaratio.datum.nodus);
            SilvaValor term = silva_sceletum_declaratio_tok_terminator(
                v_declaratio.datum.nodus);
            Xar* liberi;

            CREDO_AEQUALIS_I32 ((i32)typus.genus, (i32)SILVA_VALOR_NODUS);
            CREDO_CHORDA_AEQUALIS_LITERIS (term.datum.token->valor, ";");

            liberi = silva_nodus_liberi(piscina, v_declaratio.datum.nodus);
            CREDO_AEQUALIS_I32 (xar_numerus(liberi), II);
        }

        /* pater NUMQUAM in constructione assignatus (S27) */
        CREDO_NIHIL (v_declaratio.datum.nodus->pater);
        CREDO_NIHIL (v_typus.datum.nodus->pater);
    }


    /* ========================================================
     * PROBARE: violationes S32 (signum + dominus duplex)
     * ======================================================== */

    {
        SilvaNodus* nodus;

        imprimere("\n--- Probans violationes S32 (errores expectati) ---\n");

        nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_BINARIUM, III);

        /* signum falsum: token in locum nodi */
        CREDO_FALSUM (silva_nodus_ponere(nodus, ZEPHYRUM,
            silva_valor_token(tok_foo), SILVA_LOCUS_NODUS));

        /* scriptio valida, tum duplex */
        CREDO_VERUM (silva_nodus_ponere(nodus, I,
            silva_valor_token(tok_star), SILVA_LOCUS_TOKEN));
        CREDO_FALSUM (silva_nodus_ponere(nodus, I,
            silva_valor_token(tok_star), SILVA_LOCUS_TOKEN));

        /* extra fines */
        CREDO_FALSUM (silva_nodus_ponere(nodus, X,
            silva_valor_token(tok_star), SILVA_LOCUS_TOKEN));
    }


    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
