/* probatio_silva_scribere.c - Emissio arborum (Phase 5 Chunk A)
 *
 * Fidelitas arboris: parsare(x) -> scribere == x, pro inputis sine
 * directivis/macris (limes expansionis = Chunk B). Formae sepulcreti
 * v1 (spatia irregularia, lineae multae, tabulae, commenta, CRLF,
 * scissurae), nodi ERROR (totalitas recuperationis), AMBIGUUS
 * (canonica sola - octeti aequales ante et post collapsum ET post
 * recanonicationem), fracturae CLARAE (genus ignotum, origo non
 * FONS), corpus syntaxis v1 totum (125 lineae).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_sceleti.h"
#include "silva_tabulae_imparilis.h"
#include "silva_glr.h"
#include "silva_expandere.h"
#include "silva_commissio.h"
#include "silva_parsare.h"
#include "silva_scribere.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA 1024
#define LINEA_MAXIMA 4096

hic_manens constans SilvaGrammatica GRAMMATICA_SCELETI = {
    &SILVA_SCELETUM_TABULA,
    &SILVA_SCELETUM_REGISTRUM,
    silva_sceletum_construere,
    silva_sceletum_ambiguum_fabricare,
    NIHIL
};

hic_manens constans SilvaGrammatica GRAMMATICA_IMPARILIS = {
    &SILVA_IMPARILIS_TABULA,
    &SILVA_IMPARILIS_REGISTRUM,
    silva_imparilis_construere,
    silva_imparilis_ambiguum_fabricare,
    NIHIL
};

interior SilvaParsura*
_parsare (
               Piscina* piscina,
    constans character* fons)
{
    redde silva_parsare(piscina, "probatio.c", fons,
        (i32)strlen(fons), &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
}

/* Plagulam integram legere (exemplar mensurae) */
interior i8*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_out)
{
    FILE* pl;
    i8* buffer;
    signatus longus mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);

    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (   mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);

    *mensura_out = (i32)mensura;
    redde buffer;
}

interior b32
_scriptura_aequat (
    SilvaScriptura  scriptura,
       constans i8* octeti,
               i32  mensura)
{
    si (!scriptura.successus || scriptura.textus.mensura != mensura)
    {
        redde FALSUM;
    }
    si (mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (memcmp(scriptura.textus.datum, octeti,
        (memoriae_index)mensura) == ZEPHYRUM) ? VERUM : FALSUM;
}

interior SilvaNodus*
_elementum (
    SilvaValor lista,
           i32 index)
{
    SilvaValor* elem;

    elem = silva_valor_lista_obtinere(lista, index);
    si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde elem->datum.nodus;
}

/* Fidelitas arboris per grammaticam datam: parsare -> scribere == fons */
interior b32
_fidelis_gram (
                     Piscina* piscina,
          constans character* fons,
    constans SilvaGrammatica* grammatica)
{
      SilvaParsura* parsura;
    SilvaScriptura  scriptura;
               i32  m = (i32)strlen(fons);

    parsura = silva_parsare(piscina, "probatio.c", fons, m,
        grammatica, NIHIL, NIHIL, NIHIL);
    si (parsura == NIHIL || !parsura->successus)
    {
        redde FALSUM;
    }
    scriptura = silva_scribere_fontem(piscina, parsura,
        grammatica->tabularium, parsura->fons_princeps);
    si (!scriptura.successus)
    {
        redde FALSUM;
    }
    si (scriptura.textus.mensura != m)
    {
        redde FALSUM;
    }
    si (   m > ZEPHYRUM
        && memcmp(scriptura.textus.datum, fons,
               (memoriae_index)m) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_fidelis (
               Piscina* piscina,
    constans character* fons)
{
    redde _fidelis_gram(piscina, fons, &GRAMMATICA_SCELETI);
}

/* Resolutor sceleti (ut in probatio_silva_parsare) */
interior vacuum
_resolutor_sceleti (
        constans SilvaNodus* ambiguum,
     constans SilvaOraculum* oraculum,
                     vacuum* contextus,
    SilvaResolutioResponsum* responsum)
{
    SilvaValor interps =
        silva_sceletum_ambiguus_interpretationes(ambiguum);
    i32 i;

    (vacuum)contextus;
    per (i = ZEPHYRUM; i < silva_valor_lista_numerus(interps); i++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(interps, i);
        SilvaNodus* nodus;
        SilvaValor  typus;

        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS) perge;
        nodus = elem->datum.nodus;
        si (   nodus        == NIHIL
            || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATIO)
        {
            perge;
        }
        typus = silva_sceletum_declaratio_typus(nodus);
        si (   typus.genus != SILVA_VALOR_NODUS
            || typus.datum.nodus->genus
                != (s32)SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS)
        {
            perge;
        }
        {
            SilvaValor titulus =
                silva_sceletum_typus_nominatus_tok_titulus(
                    typus.datum.nodus);

            si (   titulus.genus == SILVA_VALOR_TOKEN
                && silva_oraculum_typum_novit(oraculum,
                       titulus.datum.token->valor))
            {
                responsum->victor        = (s32)i;
                responsum->discriminans  = titulus.datum.token;
                redde;
            }
        }
    }
}

/* Resolutor fictus: expressionem SEMPER praefert (probatio
 * recanonicationis - octeti mutari non debent) */
interior vacuum
_resolutor_praeferens_expressionem (
        constans SilvaNodus* ambiguum,
     constans SilvaOraculum* oraculum,
                     vacuum* contextus,
    SilvaResolutioResponsum* responsum)
{
    SilvaValor interps =
        silva_sceletum_ambiguus_interpretationes(ambiguum);
    i32 i;

    (vacuum)oraculum;
    (vacuum)contextus;
    per (i = ZEPHYRUM; i < silva_valor_lista_numerus(interps); i++)
    {
        SilvaNodus* nodus = _elementum(interps, i);

        si (   nodus != NIHIL
            && nodus->genus
                == (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS)
        {
            responsum->victor = (s32)i;
            redde;
        }
    }
}

s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silva_scribere",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: fidelitas basica (formae sepulcreti v1)
     * ======================================================== */

    {
        hic_manens constans character* FIXA[] = {
            "int x;",
            "int  x ;",
            "x + 1;",
            "x+1;",
            "int x;\n    x + 1;\n",
            "int\tx;",
            "/* c */ int x; /* d */",
            "",
            "  \n",
            "(x + 1) * 2;",
            "int *p;",
            "int **pp;",
            "1 + 2 * 3;",
            "int x;\r\n",
            "int x; /* eadem linea */",
            "int x;\n/* cauda propria */\n",
            "\n\n\nint x;\n\n\n"
        };
        i32 i;

        imprimere("\n--- Probans fidelitatem basicam ---\n");

        per (i = ZEPHYRUM;
             i < (i32)(magnitudo(FIXA)
                 / magnitudo(FIXA[ZEPHYRUM])); i++)
        {
            CREDO_VERUM (_fidelis(piscina, FIXA[i]));
        }
    }


    /* ========================================================
     * PROBARE: scissurae (lamina intra lexema reinseritur)
     * ======================================================== */

    {
        imprimere("\n--- Probans scissuras ---\n");

        CREDO_VERUM (_fidelis(piscina, "ab\\\ncd + 1;"));
        CREDO_VERUM (_fidelis(piscina, "ab\\\r\ncd;"));
    }


    /* ========================================================
     * PROBARE: fidelitas per nodos ERROR (totalitas
     * recuperationis - octeti numquam perduntur)
     * ======================================================== */

    {
        imprimere("\n--- Probans fidelitatem nodorum ERROR ---\n");

        CREDO_VERUM (_fidelis(piscina, "int int;"));
        CREDO_VERUM (_fidelis(piscina, "@ $ ^;"));
        CREDO_VERUM (_fidelis(piscina, "\"abc"));
        CREDO_VERUM (_fidelis(piscina, "int x"));
        CREDO_VERUM (_fidelis(piscina, "int a; @@@; x + 1;"));
    }


    /* ========================================================
     * PROBARE: AMBIGUUS - canonica sola, octeti stabiles
     * trans collapsum et recanonicationem
     * ======================================================== */

    {
          SilvaParsura* parsura;
        SilvaScriptura  ante;
        SilvaScriptura  post;
         SilvaOraculum* oraculum;
                   i32  versae;

        imprimere("\n--- Probans ambigua (canonica sola) ---\n");

        /* Sine oraculo: AMBIGUUS superstes, octeti exacti */
        CREDO_VERUM (_fidelis(piscina, "foo * bar;"));

        /* Cum oraculo: collapsus, octeti exacti */
        {
            SilvaParsura* p2;

            oraculum = silva_oraculum_creare(piscina);
            CREDO_VERUM (silva_oraculum_typum_addere_literis(oraculum,
                "foo"));
            p2 = silva_parsare(piscina, "probatio.c", "foo * bar;", X,
                &GRAMMATICA_SCELETI, oraculum, _resolutor_sceleti,
                NIHIL);
            CREDO_VERUM (p2->successus);
            CREDO_AEQUALIS_I32 (xar_numerus(p2->commissio->ambigui),
                ZEPHYRUM);
            post = silva_scribere_fontem(piscina, p2,
                &SILVA_SCELETUM_REGISTRUM, ZEPHYRUM);
            CREDO_VERUM (post.successus);
            CREDO_VERUM (chorda_aequalis_literis(post.textus,
                "foo * bar;"));
        }

        /* Recanonicatio: canonica versa, octeti IIDEM (lexemata
         * communia - emissio a canonica independens esse debet) */
        parsura = _parsare(piscina, "foo * bar;");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            I);

        ante = silva_scribere_fontem(piscina, parsura,
            &SILVA_SCELETUM_REGISTRUM, ZEPHYRUM);
        CREDO_VERUM (ante.successus);

        versae = silva_recanonicare(parsura->commissio, oraculum,
            _resolutor_praeferens_expressionem, NIHIL);
        CREDO_AEQUALIS_I32 (versae, I);

        post = silva_scribere_fontem(piscina, parsura,
            &SILVA_SCELETUM_REGISTRUM, ZEPHYRUM);
        CREDO_VERUM (post.successus);
        CREDO_AEQUALIS_I32 (ante.textus.mensura, post.textus.mensura);
        CREDO_VERUM (memcmp(ante.textus.datum, post.textus.datum,
            (memoriae_index)ante.textus.mensura) == ZEPHYRUM);
        CREDO_VERUM (chorda_aequalis_literis(post.textus,
            "foo * bar;"));
    }


    /* ========================================================
     * PROBARE: grammatica imparilis (via transmutationis)
     * ======================================================== */

    {
        SilvaParsura* parsura;

        imprimere("\n--- Probans imparilem (transmutatio) ---\n");

        parsura = silva_parsare(piscina, "probatio.c", "foo * bar;", X,
            &GRAMMATICA_IMPARILIS, NIHIL, NIHIL, NIHIL);
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->transmutationes, I);
        CREDO_VERUM (_fidelis_gram(piscina, "foo * bar;",
            &GRAMMATICA_IMPARILIS));
        CREDO_VERUM (_fidelis_gram(piscina, "int x;\nfoo * bar;\n",
            &GRAMMATICA_IMPARILIS));
    }


    /* ========================================================
     * PROBARE: subarbores (ingressus transformationum)
     * ======================================================== */

    {
          SilvaParsura* parsura;
            SilvaNodus* declaratio;
        SilvaScriptura  scriptura;

        imprimere("\n--- Probans subarbores ---\n");

        parsura = _parsare(piscina, "int x;");
        CREDO_VERUM (parsura->successus);
        declaratio = _elementum(parsura->commissio->radix, ZEPHYRUM);
        CREDO_NON_NIHIL (declaratio);

        scriptura = silva_scribere_nodum(piscina, declaratio,
            &SILVA_SCELETUM_REGISTRUM, parsura->expansio);
        CREDO_VERUM (scriptura.successus);
        CREDO_VERUM (chorda_aequalis_literis(scriptura.textus,
            "int x;"));

        /* Valor radicis integer */
        scriptura = silva_scribere_valorem(piscina,
            parsura->commissio->radix, &SILVA_SCELETUM_REGISTRUM,
            parsura->expansio);
        CREDO_VERUM (scriptura.successus);
        CREDO_VERUM (chorda_aequalis_literis(scriptura.textus,
            "int x;"));

        /* Subarbor secunda seriei - octeti SUI tantum. Regula
         * attachmenti: "\n" post primum ";" trivia trailing EIUS est,
         * ergo subarbor secunda sine eo incipit sed "\n" suum fert. */
        parsura = _parsare(piscina, "int x;\nint y;\n");
        CREDO_VERUM (parsura->successus);
        scriptura = silva_scribere_nodum(piscina,
            _elementum(parsura->commissio->radix, I),
            &SILVA_SCELETUM_REGISTRUM, parsura->expansio);
        CREDO_VERUM (scriptura.successus);
        CREDO_VERUM (chorda_aequalis_literis(scriptura.textus,
            "int y;\n"));
    }


    /* ========================================================
     * PROBARE: fracturae CLARAE (numquam octeti falsi taciti)
     * ======================================================== */

    {
            SilvaNodus* nodus;
        SilvaScriptura  scriptura;

        imprimere("\n--- Probans fracturas claras ---\n");

        /* Genus ignotum */
        nodus = silva_nodus_creare(piscina, 9999, I);
        CREDO_NON_NIHIL (nodus);
        scriptura = silva_scribere_nodum(piscina, nodus,
            &SILVA_SCELETUM_REGISTRUM, NIHIL);
        CREDO_FALSUM (scriptura.successus);
        CREDO_VERUM (scriptura.causa != NIHIL);

        /* AMBIGUUS canonica extra fines */
        nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_AMBIGUUS, II);
        CREDO_NON_NIHIL (nodus);
        {
              SilvaValor  lista  = silva_valor_lista_nova(piscina);
            SilvaParsura* p      = _parsare(piscina, "int x;");

            lista = silva_valor_lista_appendere(piscina, lista,
                silva_valor_nodus(_elementum(p->commissio->radix,
                    ZEPHYRUM)));
            CREDO_VERUM (silva_nodus_ponere(nodus, ZEPHYRUM, lista,
                SILVA_LOCUS_LISTA_NODUS));
            CREDO_VERUM (silva_nodus_ponere(nodus, I,
                silva_valor_index(V), SILVA_LOCUS_INDEX));
        }
        scriptura = silva_scribere_nodum(piscina, nodus,
            &SILVA_SCELETUM_REGISTRUM, NIHIL);
        CREDO_FALSUM (scriptura.successus);

        /* Sine contextu expansionis: lexema expansum in subarbore =
         * fractura clara; cum contextu subarbor ipsa fidelis (Chunk
         * A fractura in fidelitatem versa - sim ⑥) */
        {
            SilvaParsura* p = _parsare(piscina,
                "#define T int\nT x;");

            CREDO_VERUM (p->successus);
            scriptura = silva_scribere_nodum(piscina,
                _elementum(p->commissio->radix, ZEPHYRUM),
                &SILVA_SCELETUM_REGISTRUM, NIHIL);
            CREDO_FALSUM (scriptura.successus);
            CREDO_VERUM (scriptura.causa != NIHIL);

            scriptura = silva_scribere_nodum(piscina,
                _elementum(p->commissio->radix, ZEPHYRUM),
                &SILVA_SCELETUM_REGISTRUM, p->expansio);
            CREDO_VERUM (scriptura.successus);
            CREDO_VERUM (chorda_aequalis_literis(scriptura.textus,
                "T x;"));
        }
    }


    /* ========================================================
     * PROBARE: limes expansionis (obiectum-simile) - fidelitas
     * fontis per macros expansa (sim ⑥ C4)
     * ======================================================== */

    {
        imprimere("\n--- Probans limitem expansionis ---\n");

        CREDO_VERUM (_fidelis(piscina, "#define T int\nT x;"));
        CREDO_VERUM (_fidelis(piscina, "#define DUO 1 + 2\nDUO;"));
        CREDO_VERUM (_fidelis(piscina, "#define A 7\nA + A;"));
        CREDO_VERUM (_fidelis(piscina,
            "#define A B\n#define B 3\nA;"));
        CREDO_VERUM (_fidelis(piscina,
            "#define X 1\nX;\n#undef X\nX;"));
        CREDO_VERUM (_fidelis(piscina,
            "#define T int\n  T  x ;  /* trivia circa */\n"));
    }


    /* ========================================================
     * PROBARE: extenta functio-similium (sim ⑥ C5)
     * ======================================================== */

    {
        imprimere("\n--- Probans extenta functio-similium ---\n");

        CREDO_VERUM (_fidelis(piscina,
            "#define ADDERE(a, b) a + b\nADDERE(1, 2);"));
        CREDO_VERUM (_fidelis(piscina,
            "#define ADDERE(a,b) a+b\nADDERE( 1 , 2 );"));
        CREDO_VERUM (_fidelis(piscina,
            "#define F(x) x\n#define G 9\nF(G);"));
        CREDO_VERUM (_fidelis(piscina,
            "#define F(x) x\nF(1) + F(2);"));
        CREDO_VERUM (_fidelis(piscina,
            "#define PAR(x) (x)\nPAR((1 + 2));"));

        /* Limes CHORDA (stringificatio - Phase 7 Chunk C: corpus
         * solarii deferral coegit, vectis maximalista): radix per
         * primus + extentum per continentiam. Corpus solo #x,
         * #x medio corporis (forma CHECK solarii), argumentum
         * multi-lexematis, argumentum vacuum. */
        CREDO_VERUM (_fidelis(piscina,
            "#define STR(x) #x\nSTR(abc);"));
        CREDO_VERUM (_fidelis(piscina,
            "#define STR(x) #x\nSTR(a + b);"));
        CREDO_VERUM (_fidelis(piscina,
            "#define CH(c) probe(#c, 1)\nCH(x + y);"));
        CREDO_VERUM (_fidelis(piscina,
            "#define STR(x) #x\nSTR();"));
        CREDO_VERUM (_fidelis(piscina,
            "#define STR(x) 1 + #x + 2\nSTR(m);"));
    }


    /* ========================================================
     * PROBARE: regiones et directivae (sim ⑥ C1/C2/C3)
     * ======================================================== */

    {
        imprimere("\n--- Probans regiones et directivas ---\n");

        /* ramus non sumptus reinseritur (C2) */
        CREDO_VERUM (_fidelis(piscina,
            "#if 0\nint a;\n#else\nint b;\n#endif\n"));
        CREDO_VERUM (_fidelis(piscina,
            "#if 1\nint a;\n#else\nint b;\n#endif\n"));
        CREDO_VERUM (_fidelis(piscina,
            "#ifdef ABSENS\nint a;\n#endif\nint c;\n"));
        CREDO_VERUM (_fidelis(piscina,
            "#if 1\n#if 0\nint a;\n#endif\nint b;\n#endif\n"));
        /* regio imperfecta (EOF ante #endif) */
        CREDO_VERUM (_fidelis(piscina, "#if 0\nint a;\n"));
        /* directiva ignota transit (in arbore, non reinserenda) */
        CREDO_VERUM (_fidelis(piscina, "#pragma semel\nint x;"));
        /* includendum ignotum: linea capta et reinserta */
        CREDO_VERUM (_fidelis(piscina,
            "#include \"ignota.h\"\nint x;"));
        /* orphanum #endif transit */
        CREDO_VERUM (_fidelis(piscina, "#endif\nint x;"));
        /* forma preprocessor_test: omnia simul */
        CREDO_VERUM (_fidelis(piscina,
            "#define MAGNITUDO 100\n"
            "#define VACUUM_PR\n"
            "#undef VACUUM_PR\n"
            "#pragma semel\n"
            "int g;\n"
            "#ifdef ABSENS\nint a;\n#else\nint b;\n#endif\n"
            "#ifndef ABSENS\nint c;\n#endif\n"
            "MAGNITUDO;\n"));
    }


    /* ========================================================
     * PROBARE: includenda - fontes plures (sim ⑥ C1/C6)
     * ======================================================== */

    {
        hic_manens constans character* MODULUS =
            "#ifndef MODULUS_H\n#define MODULUS_H\nint m;\n#endif\n";
             SilvaExpansio* exp;
              SilvaParsura* parsura;
            SilvaScriptura  scriptura;
                       s32  fons_moduli;
        constans character* fons =
            "#include \"modulus.h\"\nint x;\n";

        imprimere("\n--- Probans includenda (fontes plures) ---\n");

        exp = silva_expansio_creare(piscina);
        fons_moduli = silva_includendum_praebere(exp, "modulus.h",
            MODULUS, (i32)strlen(MODULUS));
        CREDO_VERUM (fons_moduli >= ZEPHYRUM);

        parsura = silva_parsare_cum_expansione(piscina, exp,
            "probatio.c", fons, (i32)strlen(fons),
            &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
        CREDO_VERUM (parsura->successus);

        /* plagula includens: linea #include reinserta, lexemata
         * moduli praetermissa (NB fons princeps NON 0 est - praebere
         * indices priores capit) */
        scriptura = silva_scribere_fontem(piscina, parsura,
            &SILVA_SCELETUM_REGISTRUM, parsura->fons_princeps);
        CREDO_VERUM (scriptura.successus);
        CREDO_VERUM (chorda_aequalis_literis(scriptura.textus, fons));

        /* plagula inclusa: custodes + interior + EOF caudae ex
         * includenda (C6) */
        scriptura = silva_scribere_fontem(piscina, parsura,
            &SILVA_SCELETUM_REGISTRUM, fons_moduli);
        CREDO_VERUM (scriptura.successus);
        CREDO_VERUM (chorda_aequalis_literis(scriptura.textus,
            MODULUS));

        /* inclusio bis: secunda praetermissa (custos), ambae lineae
         * reinsertae */
        exp = silva_expansio_creare(piscina);
        CREDO_VERUM (silva_includendum_praebere(exp, "modulus.h",
            MODULUS, (i32)strlen(MODULUS)) >= ZEPHYRUM);
        {
            constans character* fons_bis =
                "#include \"modulus.h\"\n#include \"modulus.h\"\n"
                "int x;\n";

            parsura = silva_parsare_cum_expansione(piscina, exp,
                "probatio.c", fons_bis, (i32)strlen(fons_bis),
                &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
            CREDO_VERUM (parsura->successus);
            scriptura = silva_scribere_fontem(piscina, parsura,
                &SILVA_SCELETUM_REGISTRUM, parsura->fons_princeps);
            CREDO_VERUM (scriptura.successus);
            CREDO_VERUM (chorda_aequalis_literis(scriptura.textus,
                fons_bis));
        }
    }


    /* ========================================================
     * PROBARE: corpus syntaxis v1 totum - fidelitas arboris
     * (125 lineae, omnes sine directivis - Chunk A tota)
     * ======================================================== */

    {
        constans character* radix_env;
                 character  via[VIA_MAXIMA];
                 character  linea[LINEA_MAXIMA];
        FILE* corpus;
        i32 lineae = ZEPHYRUM;
        i32 fideles = ZEPHYRUM;

        imprimere("\n--- Probans corpus syntaxis v1 (fidelitas) ---\n");

        radix_env = getenv("RHUBARB_RADIX");
        si (radix_env == NIHIL)
        {
            radix_env = "..";
        }
        sprintf(via, "%s/silva/probationes/fixa/syntaxis_v1_corpus.txt",
            radix_env);
        corpus = fopen(via, "rb");
        CREDO_NON_NIHIL (corpus);

        si (corpus != NIHIL)
        {
            dum (fgets(linea, LINEA_MAXIMA, corpus) != NIHIL)
            {
                       Piscina* piscina_lineae;
                memoriae_index  m = strlen(linea);

                si (   m == ZEPHYRUM || linea[ZEPHYRUM] == '#'
                    || linea[ZEPHYRUM] == '\n')
                {
                    perge;
                }
                lineae++;

                piscina_lineae = piscina_generare_dynamicum(
                    "corpus_lineae", 4194304);
                si (piscina_lineae == NIHIL)
                {
                    perge;
                }
                si (_fidelis(piscina_lineae, linea))
                {
                    fideles++;
                }
                piscina_destruere(piscina_lineae);
            }
            fclose(corpus);
        }

        imprimere("  lineae: %d, fideles: %d\n", (int)lineae,
            (int)fideles);
        CREDO_AEQUALIS_I32 (lineae, CXXV);
        CREDO_AEQUALIS_I32 (fideles, lineae);
    }


    /* ========================================================
     * PROBARE: cursor.c clausura (VECTIS M1 par 6) - plagula vera
     * cum latina.h praebita: verba clausa latina EXPANDUNTUR et
     * reconstructio strati 0 octetim exacta manet; latina.h et
     * cursor.h quoque ut fontes SUI reconstruuntur
     * ======================================================== */

    {
        constans character* radix_env;
                 character  via[VIA_MAXIMA];
                   Piscina* piscina_cursoris;
                        i8* cursor_c;
                        i8* cursor_h;
                        i8* color_h;
                        i8* latina_h;
                       i32  m_cursor_c;
                       i32  m_cursor_h;
                       i32  m_color_h;
                       i32  m_latina_h;

        imprimere("\n--- Probans clausuram cursor.c (vectis M1) ---\n");

        radix_env = getenv("RHUBARB_RADIX");
        si (radix_env == NIHIL)
        {
            radix_env = "..";
        }

        piscina_cursoris = piscina_generare_dynamicum(
            "clausura_cursoris", 33554432);
        CREDO_NON_NIHIL (piscina_cursoris);

        sprintf(via, "%s/probationes/fixa/roundtrip/cursor.c",
            radix_env);
        cursor_c = _plagulam_legere(piscina_cursoris, via, &m_cursor_c);
        sprintf(via, "%s/probationes/fixa/roundtrip/cursor.h",
            radix_env);
        cursor_h = _plagulam_legere(piscina_cursoris, via, &m_cursor_h);
        sprintf(via, "%s/probationes/fixa/roundtrip/color.h",
            radix_env);
        color_h = _plagulam_legere(piscina_cursoris, via, &m_color_h);
        sprintf(via, "%s/probationes/fixa/roundtrip/latina.h",
            radix_env);
        latina_h = _plagulam_legere(piscina_cursoris, via, &m_latina_h);
        CREDO_NON_NIHIL (cursor_c);
        CREDO_NON_NIHIL (cursor_h);
        CREDO_NON_NIHIL (color_h);
        CREDO_NON_NIHIL (latina_h);

        si (cursor_c != NIHIL && cursor_h != NIHIL && color_h != NIHIL
            && latina_h != NIHIL)
        {
             SilvaExpansio* exp;
              SilvaParsura* parsura;
            SilvaScriptura  scriptura;
                       s32  fons_cursor_h;
                       s32  fons_latina_h;

            exp = silva_expansio_creare(piscina_cursoris);
            fons_cursor_h = silva_includendum_praebere(exp, "cursor.h",
                (constans character*)cursor_h, m_cursor_h);
            CREDO_VERUM (silva_includendum_praebere(exp, "color.h",
                (constans character*)color_h, m_color_h) >= ZEPHYRUM);
            fons_latina_h = silva_includendum_praebere(exp, "latina.h",
                (constans character*)latina_h, m_latina_h);
            CREDO_VERUM (fons_cursor_h >= ZEPHYRUM);
            CREDO_VERUM (fons_latina_h >= ZEPHYRUM);

            parsura = silva_parsare_cum_expansione(piscina_cursoris,
                exp, "cursor.c", (constans character*)cursor_c,
                m_cursor_c, &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
            CREDO_NON_NIHIL (parsura);
            CREDO_VERUM (parsura->successus);

            /* verba clausa latina vere expansa sunt? (aliter vectis
             * vacua esset) */
            CREDO_NON_NIHIL (silva_expansio_quaerere(exp,
                chorda_ex_literis("si", piscina_cursoris)));

            /* cursor.c: fons princeps octetim exactus (praebere
             * indices priores cepit - princeps est ultimus) */
            scriptura = silva_scribere_fontem(piscina_cursoris,
                parsura, &SILVA_SCELETUM_REGISTRUM,
                parsura->fons_princeps);
            CREDO_VERUM (_scriptura_aequat(scriptura, cursor_c,
                m_cursor_c));

            /* cursor.h: fons inclusus custoditus */
            scriptura = silva_scribere_fontem(piscina_cursoris,
                parsura, &SILVA_SCELETUM_REGISTRUM, fons_cursor_h);
            CREDO_VERUM (_scriptura_aequat(scriptura, cursor_h,
                m_cursor_h));

            /* latina.h: fons inclusus - solae directivae + EOF ex
             * includenda (sim ⑥ C6) */
            scriptura = silva_scribere_fontem(piscina_cursoris,
                parsura, &SILVA_SCELETUM_REGISTRUM, fons_latina_h);
            CREDO_VERUM (_scriptura_aequat(scriptura, latina_h,
                m_latina_h));
        }

        piscina_destruere(piscina_cursoris);
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
