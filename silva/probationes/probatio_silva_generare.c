/* probatio_silva_generare.c - Probationes generatoris (Phase 3 Chunk A)
 *
 * Lector grammaticae annotatae (forma v0 congelata, spec-v2 §9.3 +
 * silva-simulatio-3.md): symbolum@locus, genus/id/modus/manu,
 * <genera-extra>, epsilon, validatio (S19: generator est schema).
 * Grammatica exemplaris = ipsa forma congelata ex simulatione ③.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "silva_generare.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Grammatica congelata v0 (sim ③ exit deliverable) */
interior constans character* GRAMMATICA_V0 =
    "<grammatica>"
    "  <terminalia>"
    "    <terminalis titulus=\"NUMBER\" genus=\"SILVA_LEX_INTEGER\"/>"
    "    <terminalis titulus=\"PLUS\"   genus=\"SILVA_LEX_PLUS\"/>"
    "    <terminalis titulus=\"STAR\"   genus=\"SILVA_LEX_STAR\"/>"
    "    <terminalis titulus=\"LPAREN\" genus=\"SILVA_LEX_PAREN_APERTA\"/>"
    "    <terminalis titulus=\"RPAREN\" genus=\"SILVA_LEX_PAREN_CLAUSA\"/>"
    "    <terminalis titulus=\"EOF\"    genus=\"SILVA_LEX_EOF\"/>"
    "  </terminalia>"
    "  <regulae>"
    "    <regula titulus=\"expr\">"
    "      <productio genus=\"binarium\" id=\"expr-plus\">"
    "        expr@sinister PLUS@tok_operator term@dexter"
    "      </productio>"
    "      <productio>term</productio>"
    "    </regula>"
    "    <regula titulus=\"term\">"
    "      <productio genus=\"binarium\" id=\"term-star\">"
    "        term@sinister STAR@tok_operator factor@dexter"
    "      </productio>"
    "      <productio>factor</productio>"
    "    </regula>"
    "    <regula titulus=\"factor\">"
    "      <productio genus=\"parenthesis\" id=\"factor-paren\">"
    "        LPAREN@tok_apertum expr@internum RPAREN@tok_clausum"
    "      </productio>"
    "      <productio genus=\"folium-numerus\" id=\"factor-num\">"
    "        NUMBER@tok_valor"
    "      </productio>"
    "    </regula>"
    "  </regulae>"
    "  <genera-extra>"
    "    <genus titulus=\"error\"    slots=\"tokens:lista-token\"/>"
    "    <genus titulus=\"ambiguus\" slots=\"interpretationes:lista-nodus canonica:index\"/>"
    "  </genera-extra>"
    "  <initium>expr</initium>"
    "</grammatica>";

interior b32
_chorda_est (
                chorda* c,
    constans character* literis)
{
    si (c == NIHIL)
    {
        redde FALSUM;
    }
    si (c->mensura != (i32)strlen(literis))
    {
        redde FALSUM;
    }
    redde (memcmp(c->datum, literis, (memoriae_index)c->mensura)
        == ZEPHYRUM)
        ? VERUM : FALSUM;
}

/* Invenire symbolum per titulum (scansio linearis - probatio) */
interior s32
_symbolum (
    SilvaGenGrammatica* g,
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(g->symbola); i++)
    {
        SilvaGenSymbolum* sym;

        sym = (SilvaGenSymbolum*)xar_obtinere(g->symbola, i);
        si (sym != NIHIL && _chorda_est(sym->titulus, titulus))
        {
            redde sym->index;
        }
    }
    redde -I;
}

s32 principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_silva_generare",
        8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: lectio grammaticae congelatae v0
     * ======================================================== */

    {
        SilvaGenGrammatica* g;
         SilvaGenProductio* prod;
        SilvaGenLocusMappa* mappa;

        imprimere("\n--- Probans lectionem formae v0 ---\n");

        g = silva_gen_grammaticam_legere(piscina, intern,
            GRAMMATICA_V0);
        CREDO_NON_NIHIL (g);

        /* 6 terminalia, 3 non-terminalia + __initium__ */
        CREDO_AEQUALIS_I32 (g->numerus_terminalium, VI);
        CREDO_AEQUALIS_I32 (g->numerus_non_terminalium, IV);
        /* 6 productiones + augmentata */
        CREDO_AEQUALIS_I32 (xar_numerus(g->productiones), VII);

        /* P0: expr -> expr@sinister PLUS@tok_operator term@dexter */
        prod = (SilvaGenProductio*)xar_obtinere(g->productiones,
            ZEPHYRUM);
        CREDO_VERUM (_chorda_est(prod->genus, "binarium"));
        CREDO_VERUM (_chorda_est(prod->id, "expr-plus"));
        CREDO_NIHIL (prod->modus);
        CREDO_NIHIL (prod->manu);
        CREDO_AEQUALIS_I32 (xar_numerus(prod->dextrum), III);
        CREDO_AEQUALIS_I32 (xar_numerus(prod->loci), III);

        mappa = (SilvaGenLocusMappa*)xar_obtinere(prod->loci, ZEPHYRUM);
        CREDO_VERUM (_chorda_est(mappa->titulus, "sinister"));
        CREDO_FALSUM (mappa->appendere);
        mappa = (SilvaGenLocusMappa*)xar_obtinere(prod->loci, I);
        CREDO_VERUM (_chorda_est(mappa->titulus, "tok_operator"));
        mappa = (SilvaGenLocusMappa*)xar_obtinere(prod->loci, II);
        CREDO_VERUM (_chorda_est(mappa->titulus, "dexter"));

        /* P1: expr -> term (pass-through: sine genere, sine loco) */
        prod = (SilvaGenProductio*)xar_obtinere(g->productiones, I);
        CREDO_NIHIL (prod->genus);
        CREDO_NIHIL (prod->id);
        mappa = (SilvaGenLocusMappa*)xar_obtinere(prod->loci, ZEPHYRUM);
        CREDO_NIHIL (mappa->titulus);

        /* genera-extra: error + ambiguus */
        CREDO_AEQUALIS_I32 (xar_numerus(g->genera_extra), II);
        {
            SilvaGenGenusExtra* extra;

            extra = (SilvaGenGenusExtra*)xar_obtinere(g->genera_extra,
                ZEPHYRUM);
            CREDO_VERUM (_chorda_est(extra->titulus, "error"));
            CREDO_VERUM (_chorda_est(extra->loci_descriptio,
                "tokens:lista-token"));
        }
    }


    /* ========================================================
     * PROBARE: FIRST/FOLLOW + tabulae LALR (grammatica v0)
     * ======================================================== */

    {
        SilvaGenGrammatica* g;
         SilvaGenCollectio* collectio;
            SilvaGenTabula* tabula;

        imprimere("\n--- Probans FIRST/FOLLOW/LALR ---\n");

        g = silva_gen_grammaticam_legere(piscina, intern,
            GRAMMATICA_V0);
        CREDO_NON_NIHIL (g);
        CREDO_VERUM (silva_gen_first_computare(g));
        CREDO_VERUM (silva_gen_follow_computare(g));

        /* FIRST(expr) = {NUMBER, LPAREN} */
        {
            Xar* first_expr;

            first_expr = silva_gen_first_obtinere(g,
                (i32)_symbolum(g, "expr"));
            CREDO_NON_NIHIL (first_expr);
            CREDO_AEQUALIS_I32 (xar_numerus(first_expr), II);
        }

        collectio = silva_gen_collectio_lalr_construere(g);
        CREDO_NON_NIHIL (collectio);
        CREDO_VERUM (xar_numerus(collectio->status_omnes) > ZEPHYRUM);

        tabula = silva_gen_tabulam_construere(collectio);
        CREDO_NON_NIHIL (tabula);
        /* grammatica non-ambigua: nulli conflictus */
        CREDO_AEQUALIS_S32 (tabula->numerus_conflictuum, ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: conservatio conflictuum (grammatica ambigua)
     * E -> E PLUS E | NUMBER: shift/reduce in tabula MANET
     * ======================================================== */

    {
        SilvaGenGrammatica* g;
         SilvaGenCollectio* collectio;
            SilvaGenTabula* tabula;
        constans character* fons =
            "<grammatica>"
            "  <terminalia>"
            "    <terminalis titulus=\"NUMBER\" genus=\"SILVA_LEX_INTEGER\"/>"
            "    <terminalis titulus=\"PLUS\"   genus=\"SILVA_LEX_PLUS\"/>"
            "    <terminalis titulus=\"EOF\"    genus=\"SILVA_LEX_EOF\"/>"
            "  </terminalia>"
            "  <regulae>"
            "    <regula titulus=\"E\">"
            "      <productio genus=\"binarium\" id=\"e-plus\">"
            "        E@sinister PLUS@tok_operator E@dexter"
            "      </productio>"
            "      <productio>NUMBER</productio>"
            "    </regula>"
            "  </regulae>"
            "  <initium>E</initium>"
            "</grammatica>";

        imprimere("\n--- Probans conservationem conflictuum ---\n");

        g = silva_gen_grammaticam_legere(piscina, intern, fons);
        CREDO_NON_NIHIL (g);
        CREDO_VERUM (silva_gen_first_computare(g));
        CREDO_VERUM (silva_gen_follow_computare(g));

        collectio = silva_gen_collectio_lalr_construere(g);
        CREDO_NON_NIHIL (collectio);
        tabula = silva_gen_tabulam_construere(collectio);
        CREDO_NON_NIHIL (tabula);

        /* Ambiguitas deliberata: conflictus adsunt et SERVANTUR */
        CREDO_VERUM (tabula->numerus_conflictuum > ZEPHYRUM);

        /* Invenire cellam multi-actionis: aliquis status habet
         * SHIFT et REDUCE in eodem terminali (PLUS) */
        {
            b32 cella_duplex_inventa;
            i32 s;
            s32 plus_idx;

            cella_duplex_inventa  = FALSUM;
            plus_idx              = _symbolum(g, "PLUS");

            per (s = ZEPHYRUM;
                 s < xar_numerus(tabula->status_tabulae)
                     && !cella_duplex_inventa;
                 s++)
            {
                Xar* actiones;

                actiones = silva_gen_actiones_quaerere(tabula, (s32)s,
                    plus_idx);
                si (actiones != NIHIL && xar_numerus(actiones) >= II)
                {
                    cella_duplex_inventa = VERUM;
                }
            }
            CREDO_VERUM (cella_duplex_inventa);
        }
    }


    /* ========================================================
     * PROBARE: productio epsilon (silva additum - lapifex carebat)
     * ======================================================== */

    {
        SilvaGenGrammatica* g;
          SilvaGenSymbolum* sym;
        constans character* fons =
            "<grammatica>"
            "  <terminalia>"
            "    <terminalis titulus=\"ITEM\" genus=\"SILVA_LEX_IDENTIFICATOR\"/>"
            "    <terminalis titulus=\"EOF\"  genus=\"SILVA_LEX_EOF\"/>"
            "  </terminalia>"
            "  <regulae>"
            "    <regula titulus=\"lista\">"
            "      <productio></productio>"
            "      <productio genus=\"lista-item\" id=\"lista-app\" modus=\"lista-appendere\">"
            "        lista@elementa+ ITEM@tok_item"
            "      </productio>"
            "    </regula>"
            "  </regulae>"
            "  <initium>lista</initium>"
            "</grammatica>";

        imprimere("\n--- Probans epsilon ---\n");

        g = silva_gen_grammaticam_legere(piscina, intern, fons);
        CREDO_NON_NIHIL (g);

        /* P0 = epsilon: dextrum vacuum */
        {
            SilvaGenProductio* prod;

            prod = (SilvaGenProductio*)xar_obtinere(g->productiones,
                ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(prod->dextrum), ZEPHYRUM);
        }

        /* @locus+ forma appendendi lecta */
        {
             SilvaGenProductio* prod;
            SilvaGenLocusMappa* mappa;

            prod = (SilvaGenProductio*)xar_obtinere(g->productiones, I);
            CREDO_VERUM (_chorda_est(prod->modus, "lista-appendere"));
            mappa = (SilvaGenLocusMappa*)xar_obtinere(prod->loci,
                ZEPHYRUM);
            CREDO_VERUM (_chorda_est(mappa->titulus, "elementa"));
            CREDO_VERUM (mappa->appendere);
        }

        /* FIRST: lista habet epsilon */
        CREDO_VERUM (silva_gen_first_computare(g));
        sym = (SilvaGenSymbolum*)xar_obtinere(g->symbola,
            (i32)_symbolum(g, "lista"));
        CREDO_VERUM (sym->habet_epsilon);

        /* tabulae construuntur sine fragore */
        CREDO_VERUM (silva_gen_follow_computare(g));
        {
            SilvaGenCollectio* collectio;

            collectio = silva_gen_collectio_lalr_construere(g);
            CREDO_NON_NIHIL (collectio);
            CREDO_NON_NIHIL (silva_gen_tabulam_construere(collectio));
        }
    }


    /* ========================================================
     * PROBARE: productio multilinearis (2026-07-04)
     * Linea nova intra textum productionis atomos NON amittit.
     * Classis inventa per c89.stml: stml_textus_internus '\n'
     * servat, fissio per ' ' solum dividit -> "gamma@c\nDELTA@d"
     * UNUS atomus fiebat, symbolum DELTA TACITE evanescebat
     * (85 conflictus spurii). Nunc: albispatia normantur ante
     * fissionem + atomi corrupti clamant.
     * ======================================================== */

    {
        SilvaGenGrammatica* g;
         SilvaGenProductio* prod;
        SilvaGenLocusMappa* mappa;

        imprimere("\n--- Probans productionem multilinearem ---\n");

        g = silva_gen_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"B\" genus=\"GB\"/>"
            "<terminalis titulus=\"C\" genus=\"GC\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"r\">"
            "<productio genus=\"g\" id=\"r-multilinearis\">\n"
            "    A@primus B@secundus\n"
            "    C@tertius\n"
            "</productio>"
            "</regula>"
            "</regulae><initium>r</initium></grammatica>");
        CREDO_NON_NIHIL (g);

        prod = (SilvaGenProductio*)xar_obtinere(g->productiones,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(prod->dextrum), III);
        mappa = (SilvaGenLocusMappa*)xar_obtinere(prod->loci, II);
        CREDO_VERUM (_chorda_est(mappa->titulus, "tertius"));
    }


    /* ========================================================
     * PROBARE: validatio (generator est schema - R4/S19/S31)
     * Omnes casus mali: legere reddit NIHIL, nullus fragor.
     * ======================================================== */

    {
        imprimere("\n--- Probans validationem ---\n");

        /* atomum corruptum: '@' intra nomen loci (classis
         * multilinearis ante normationem) -> clamat */
        CREDO_NIHIL (silva_gen_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"r\">"
            "<productio genus=\"g\" id=\"r-corruptum\">"
            "A@primus@secundus</productio></regula>"
            "</regulae><initium>r</initium></grammatica>"));

        /* multi-symbola sine genere */
        CREDO_NIHIL (silva_gen_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"r\"><productio>r A@t</productio></regula>"
            "</regulae><initium>r</initium></grammatica>"));

        /* genus sine id */
        CREDO_NIHIL (silva_gen_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"r\">"
            "<productio genus=\"g\">A@t</productio></regula>"
            "</regulae><initium>r</initium></grammatica>"));

        /* id duplicatum */
        CREDO_NIHIL (silva_gen_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"r\">"
            "<productio genus=\"g\" id=\"x\">A@t</productio>"
            "<productio genus=\"g\" id=\"x\">A@t</productio>"
            "</regula>"
            "</regulae><initium>r</initium></grammatica>"));

        /* terminale sine @loco in productione cum genere (S19) */
        CREDO_NIHIL (silva_gen_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"B\" genus=\"GB\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"r\">"
            "<productio genus=\"g\" id=\"x\">A@t B</productio></regula>"
            "</regulae><initium>r</initium></grammatica>"));

        /* terminale ut LHS regulae */
        CREDO_NIHIL (silva_gen_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"A\"><productio>A</productio></regula>"
            "</regulae><initium>A</initium></grammatica>"));

        /* manu = effugium: multi-symbola sine genere ACCIPITUR */
        CREDO_NON_NIHIL (silva_gen_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"r\">"
            "<productio manu=\"construere_r\">r A</productio></regula>"
            "</regulae><initium>r</initium></grammatica>"));

        /* NON-terminale sine @loco in productione cum genere =
         * error (Phase 5: totalitas emissionis - subarbor non
         * mappata octetos perderet) */
        CREDO_NIHIL (silva_gen_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"r\">"
            "<productio genus=\"g\" id=\"x\">r A@t</productio></regula>"
            "</regulae><initium>r</initium></grammatica>"));
    }


    /* ========================================================
     * PROBARE: validatio ordinis locorum (Phase 5, scribere)
     * Emissio generica ordinem layout sequitur - productiones
     * eiusdem generis ordine consentire debent.
     * ======================================================== */

    {
        SilvaGenGrammatica* g;

        imprimere("\n--- Probans ordinem locorum ---\n");

        /* Ordo divergens: p1 mappat x y, p2 mappat y x - error
         * in registro computando */
        g = silva_gen_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"B\" genus=\"GB\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"r\">"
            "<productio genus=\"g\" id=\"p1\">A@x B@y</productio>"
            "<productio genus=\"g\" id=\"p2\">B@y A@x</productio>"
            "</regula>"
            "</regulae>"
            "<genera-extra>"
            "<genus titulus=\"error\" slots=\"tokens:lista-token\"/>"
            "<genus titulus=\"ambiguus\" slots=\"interpretationes:lista-nodus canonica:index\"/>"
            "<genus titulus=\"conditionalis\" slots=\"rami:lista-nodus finis:lista-token\"/>"
            "<genus titulus=\"ramus-sumptus\" slots=\"directiva:lista-token contentum:lista-nodus conditio_id:index\"/>"
            "<genus titulus=\"ramus-omissus\" slots=\"directiva:lista-token cruda:lista-token conditio_id:index\"/>"
            "</genera-extra>"
            "<initium>r</initium></grammatica>");
        CREDO_NON_NIHIL (g);
        CREDO_NIHIL (silva_gen_registrum_computare(g));

        /* Ordo consentiens: idem sed p2 quoque x y - acceptum */
        g = silva_gen_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia>"
            "<terminalis titulus=\"A\" genus=\"GA\"/>"
            "<terminalis titulus=\"B\" genus=\"GB\"/>"
            "<terminalis titulus=\"EOF\" genus=\"GE\"/>"
            "</terminalia><regulae>"
            "<regula titulus=\"r\">"
            "<productio genus=\"g\" id=\"p1\">A@x B@y</productio>"
            "<productio genus=\"g\" id=\"p2\">A@x B@y B@y2</productio>"
            "</regula>"
            "</regulae>"
            "<genera-extra>"
            "<genus titulus=\"error\" slots=\"tokens:lista-token\"/>"
            "<genus titulus=\"ambiguus\" slots=\"interpretationes:lista-nodus canonica:index\"/>"
            "<genus titulus=\"conditionalis\" slots=\"rami:lista-nodus finis:lista-token\"/>"
            "<genus titulus=\"ramus-sumptus\" slots=\"directiva:lista-token contentum:lista-nodus conditio_id:index\"/>"
            "<genus titulus=\"ramus-omissus\" slots=\"directiva:lista-token cruda:lista-token conditio_id:index\"/>"
            "</genera-extra>"
            "<initium>r</initium></grammatica>");
        CREDO_NON_NIHIL (g);
        CREDO_NON_NIHIL (silva_gen_registrum_computare(g));
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
