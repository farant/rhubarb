/* nuntium_schema.c - Implementatio Linguae Schematis Nuntii
 *
 * Usat Lapifex ad parsandum definitionem schematis declarativam.
 * Construit AST nodorum ex reductionibus parsatoris.
 */

#include "nuntium_schema.h"
#include "lapifex_generare.h"
#include "lapifex_parsere.h"
#include "lapifex_lexere.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "piscina.h"
#include <string.h>

/* ================================================
 * Grammatica STML pro Schemate Nuntii
 * ================================================
 *
 * Productiones:
 *   P0:  schema         -> definitio_list
 *   P1:  definitio_list -> definitio
 *   P2:  definitio_list -> definitio_list definitio
 *   P3:  definitio      -> NUNTIUM IDENT LBRACE campus_list RBRACE
 *   P4:  campus_list    -> campus
 *   P5:  campus_list    -> campus_list campus
 *   P6:  campus         -> genus_campi IDENT AEQUALIS NUMERUS SEMICOLON
 *   P7:  genus_campi    -> TEXTUS
 *   P8:  genus_campi    -> VARINT
 *   P9:  genus_campi    -> SVARINT
 *   P10: genus_campi    -> FIXUM32
 *   P11: genus_campi    -> FIXUM64
 *   P12: genus_campi    -> BYTES
 *   P13: genus_campi    -> F32
 *   P14: genus_campi    -> F64
 *   P15: __initium__    -> schema EOF   (augmentata automatice)
 */

hic_manens constans character* NUNTIUM_SCHEMA_GRAMMATICA =
    "<grammatica>"
    "  <terminalia>"
    "    <terminalis titulus=\"NUNTIUM\" exemplum=\"nuntium\"/>"
    "    <terminalis titulus=\"TEXTUS\" exemplum=\"textus\"/>"
    "    <terminalis titulus=\"VARINT\" exemplum=\"varint\"/>"
    "    <terminalis titulus=\"SVARINT\" exemplum=\"svarint\"/>"
    "    <terminalis titulus=\"FIXUM32\" exemplum=\"fixum32\"/>"
    "    <terminalis titulus=\"FIXUM64\" exemplum=\"fixum64\"/>"
    "    <terminalis titulus=\"BYTES\" exemplum=\"bytes\"/>"
    "    <terminalis titulus=\"F32\" exemplum=\"f32\"/>"
    "    <terminalis titulus=\"F64\" exemplum=\"f64\"/>"
    "    <terminalis titulus=\"IDENT\" exemplum=\"$identificator\"/>"
    "    <terminalis titulus=\"NUMERUS\" exemplum=\"$numerus\"/>"
    "    <terminalis titulus=\"LBRACE\" exemplum=\"{\"/>"
    "    <terminalis titulus=\"RBRACE\" exemplum=\"}\"/>"
    "    <terminalis titulus=\"AEQUALIS\" exemplum=\"=\"/>"
    "    <terminalis titulus=\"SEMICOLON\" exemplum=\";\"/>"
    "    <terminalis titulus=\"EOF\"/>"
    "  </terminalia>"
    "  <regulae>"
    "    <regula titulus=\"schema\">"
    "      <productio>definitio_list</productio>"
    "    </regula>"
    "    <regula titulus=\"definitio_list\">"
    "      <productio>definitio</productio>"
    "      <productio>definitio_list definitio</productio>"
    "    </regula>"
    "    <regula titulus=\"definitio\">"
    "      <productio>NUNTIUM IDENT LBRACE campus_list RBRACE</productio>"
    "    </regula>"
    "    <regula titulus=\"campus_list\">"
    "      <productio>campus</productio>"
    "      <productio>campus_list campus</productio>"
    "    </regula>"
    "    <regula titulus=\"campus\">"
    "      <productio>genus_campi IDENT AEQUALIS NUMERUS SEMICOLON</productio>"
    "    </regula>"
    "    <regula titulus=\"genus_campi\">"
    "      <productio>TEXTUS</productio>"
    "      <productio>VARINT</productio>"
    "      <productio>SVARINT</productio>"
    "      <productio>FIXUM32</productio>"
    "      <productio>FIXUM64</productio>"
    "      <productio>BYTES</productio>"
    "      <productio>F32</productio>"
    "      <productio>F64</productio>"
    "    </regula>"
    "  </regulae>"
    "  <initium>schema</initium>"
    "</grammatica>";

/* ================================================
 * Functiones Auxiliares
 * ================================================ */

/* Ponere nodum in tabula nodorum, reddere indicem ut s64 */
hic_manens s64
_ponere_nodum(NuntiumSchemaContextus* ctx, NuntiumSchemaNodus* nodus)
{
    NuntiumSchemaNodus** locus;
    s64 index;

    index = (s64)xar_numerus(ctx->nodi);
    locus = (NuntiumSchemaNodus**)xar_addere(ctx->nodi);
    si (locus) *locus = nodus;

    redde index;
}

/* Capere nodum ex tabula nodorum per indicem s64 */
hic_manens NuntiumSchemaNodus*
_capere_nodum(NuntiumSchemaContextus* ctx, s64 index)
{
    NuntiumSchemaNodus** locus;

    si (index < 0) redde NIHIL;

    locus = (NuntiumSchemaNodus**)xar_obtinere(ctx->nodi, (i32)index);
    si (!locus) redde NIHIL;

    redde *locus;
}

/* Extrahere identificatorem ex valor compactato (offset<<32 | longitudo) */
hic_manens chorda*
_extrahere_identificatorem(NuntiumSchemaContextus* ctx, s64 valor)
{
    s32     offset;
    s32     longitudo_id;
    chorda* copia;
    i8*     dest;

    offset       = (s32)(valor >> XXXII);
    longitudo_id = (s32)(valor & (s64)0xFFFFFFFF);

    /* Copiare in piscinam (datum fontis potest esse temporaneum) */
    copia = (chorda*)piscina_allocare(ctx->piscina, (memoriae_index)magnitudo(chorda));
    si (!copia) redde NIHIL;

    dest = (i8*)piscina_allocare(
        ctx->piscina, (memoriae_index)longitudo_id);
    si (!dest) redde NIHIL;

    memcpy(dest, ctx->fons + offset, (memoriae_index)longitudo_id);
    copia->datum = dest;
    copia->mensura = (i32)longitudo_id;

    redde copia;
}

/* ================================================
 * Callback Reductionis
 * ================================================ */

hic_manens s64
_nuntium_schema_reductio(
    s32 productio_index,
    constans s64* valori,
    s32 numerus_dextrum,
    vacuum* contextus)
{
    NuntiumSchemaContextus* ctx;
    (vacuum)numerus_dextrum;

    ctx = (NuntiumSchemaContextus*)contextus;

    commutatio (productio_index)
    {
        /* P0: schema -> definitio_list */
        casus ZEPHYRUM:
        {
            NuntiumSchemaNodus* nodus;
            NuntiumSchemaNodus* lista;

            nodus = (NuntiumSchemaNodus*)piscina_allocare(
                ctx->piscina, (memoriae_index)magnitudo(NuntiumSchemaNodus));
            nodus->genus = NUNTIUM_SCHEMA_NODUS_SCHEMA;

            /* definitio_list iam est nodus cum Xar* definitiones */
            lista = _capere_nodum(ctx, valori[0]);
            si (lista && lista->genus == NUNTIUM_SCHEMA_NODUS_SCHEMA)
            {
                nodus->datum.schema.definitiones = lista->datum.schema.definitiones;
            }
            alioquin
            {
                /* lista est definitio singularis — creare Xar */
                nodus->datum.schema.definitiones = xar_creare(
                    ctx->piscina, (i32)magnitudo(NuntiumSchemaNodus*));
            }

            redde _ponere_nodum(ctx, nodus);
        }

        /* P1: definitio_list -> definitio */
        casus I:
        {
            NuntiumSchemaNodus* wrapper;
            NuntiumSchemaNodus* def_nodus;
            NuntiumSchemaNodus** locus;

            wrapper = (NuntiumSchemaNodus*)piscina_allocare(
                ctx->piscina, (memoriae_index)magnitudo(NuntiumSchemaNodus));
            wrapper->genus = NUNTIUM_SCHEMA_NODUS_SCHEMA;
            wrapper->datum.schema.definitiones = xar_creare(
                ctx->piscina, (i32)magnitudo(NuntiumSchemaNodus*));

            def_nodus = _capere_nodum(ctx, valori[0]);
            locus = (NuntiumSchemaNodus**)xar_addere(
                wrapper->datum.schema.definitiones);
            si (locus) *locus = def_nodus;

            redde _ponere_nodum(ctx, wrapper);
        }

        /* P2: definitio_list -> definitio_list definitio */
        casus II:
        {
            NuntiumSchemaNodus* lista;
            NuntiumSchemaNodus* def_nodus;
            NuntiumSchemaNodus** locus;

            lista = _capere_nodum(ctx, valori[0]);
            def_nodus = _capere_nodum(ctx, valori[1]);

            locus = (NuntiumSchemaNodus**)xar_addere(
                lista->datum.schema.definitiones);
            si (locus) *locus = def_nodus;

            /* Reddere eundem indicem */
            redde valori[0];
        }

        /* P3: definitio -> NUNTIUM IDENT LBRACE campus_list RBRACE */
        casus III:
        {
            NuntiumSchemaNodus* nodus;
            NuntiumSchemaNodus* campus_lista;

            nodus = (NuntiumSchemaNodus*)piscina_allocare(
                ctx->piscina, (memoriae_index)magnitudo(NuntiumSchemaNodus));
            nodus->genus = NUNTIUM_SCHEMA_NODUS_DEFINITIO;
            nodus->datum.definitio.titulus = _extrahere_identificatorem(ctx, valori[1]);

            /* campus_list est wrapper cum Xar* */
            campus_lista = _capere_nodum(ctx, valori[3]);
            si (campus_lista && campus_lista->genus == NUNTIUM_SCHEMA_NODUS_SCHEMA)
            {
                /* Reusamus schema wrapper pro campus_list */
                nodus->datum.definitio.campi = campus_lista->datum.schema.definitiones;
            }
            alioquin
            {
                nodus->datum.definitio.campi = xar_creare(
                    ctx->piscina, (i32)magnitudo(NuntiumSchemaNodus*));
            }

            redde _ponere_nodum(ctx, nodus);
        }

        /* P4: campus_list -> campus */
        casus IV:
        {
            NuntiumSchemaNodus* wrapper;
            NuntiumSchemaNodus* campus_nodus;
            NuntiumSchemaNodus** locus;

            wrapper = (NuntiumSchemaNodus*)piscina_allocare(
                ctx->piscina, (memoriae_index)magnitudo(NuntiumSchemaNodus));
            /* Reusare SCHEMA genus ut wrapper pro lista */
            wrapper->genus = NUNTIUM_SCHEMA_NODUS_SCHEMA;
            wrapper->datum.schema.definitiones = xar_creare(
                ctx->piscina, (i32)magnitudo(NuntiumSchemaNodus*));

            campus_nodus = _capere_nodum(ctx, valori[0]);
            locus = (NuntiumSchemaNodus**)xar_addere(
                wrapper->datum.schema.definitiones);
            si (locus) *locus = campus_nodus;

            redde _ponere_nodum(ctx, wrapper);
        }

        /* P5: campus_list -> campus_list campus */
        casus V:
        {
            NuntiumSchemaNodus* lista;
            NuntiumSchemaNodus* campus_nodus;
            NuntiumSchemaNodus** locus;

            lista = _capere_nodum(ctx, valori[0]);
            campus_nodus = _capere_nodum(ctx, valori[1]);

            locus = (NuntiumSchemaNodus**)xar_addere(
                lista->datum.schema.definitiones);
            si (locus) *locus = campus_nodus;

            redde valori[0];
        }

        /* P6: campus -> genus_campi IDENT AEQUALIS NUMERUS SEMICOLON */
        casus VI:
        {
            NuntiumSchemaNodus* nodus;

            nodus = (NuntiumSchemaNodus*)piscina_allocare(
                ctx->piscina, (memoriae_index)magnitudo(NuntiumSchemaNodus));
            nodus->genus = NUNTIUM_SCHEMA_NODUS_CAMPUS;
            nodus->datum.campus.campus_genus = (NuntiumCampusGenus)valori[0];
            nodus->datum.campus.titulus = _extrahere_identificatorem(ctx, valori[1]);
            nodus->datum.campus.tag = (s32)valori[3];

            redde _ponere_nodum(ctx, nodus);
        }

        /* P7-P14: genus_campi -> TEXTUS | VARINT | ... */
        casus VII:  redde (s64)NUNTIUM_CAMPUS_TEXTUS;
        casus VIII: redde (s64)NUNTIUM_CAMPUS_VARINT;
        casus IX:   redde (s64)NUNTIUM_CAMPUS_SVARINT;
        casus X:    redde (s64)NUNTIUM_CAMPUS_FIXUM32;
        casus XI:   redde (s64)NUNTIUM_CAMPUS_FIXUM64;
        casus XII:  redde (s64)NUNTIUM_CAMPUS_BYTES;
        casus XIII: redde (s64)NUNTIUM_CAMPUS_F32;
        casus XIV:  redde (s64)NUNTIUM_CAMPUS_F64;

        /* P15: __initium__ -> schema EOF */
        casus XV:   redde valori[0];

        ordinarius: redde 0;
    }
}

/* ================================================
 * Functio Principalis
 * ================================================ */

NuntiumSchemaNodus*
nuntium_schema_legere(
    Piscina*             piscina,
    constans character*  fons,
    s32                  longitudo)
{
    InternamentumChorda*    intern;
    LapifexGrammatica*      grammatica;
    LapifexCollectio*       collectio;
    LapifexTabula*          tabula;
    LapifexParsaturaFructus fructus;
    NuntiumSchemaContextus  ctx;

    si (!piscina || !fons || longitudo <= ZEPHYRUM) redde NIHIL;

    /* Creare internamentum pro hac parsatione */
    intern = internamentum_creare(piscina);
    si (!intern) redde NIHIL;

    /* Legere grammaticam ex STML */
    grammatica = lapifex_grammaticam_legere(piscina, intern, NUNTIUM_SCHEMA_GRAMMATICA);
    si (!grammatica) redde NIHIL;

    /* Computare FIRST coniuncta */
    si (!lapifex_first_computare(grammatica)) redde NIHIL;

    /* Computare FOLLOW coniuncta */
    si (!lapifex_follow_computare(grammatica)) redde NIHIL;

    /* Construere collectionem canonicam LR(1) */
    collectio = lapifex_collectio_construere(grammatica);
    si (!collectio) redde NIHIL;

    /* Construere tabulam ACTION/GOTO */
    tabula = lapifex_tabulam_construere(collectio);
    si (!tabula) redde NIHIL;

    /* Praeparare contextum */
    ctx.fons = fons;
    ctx.longitudo_fontis = longitudo;
    ctx.piscina = piscina;
    ctx.nodi = xar_creare(piscina, (i32)magnitudo(NuntiumSchemaNodus*));

    /* Lexere et parsare */
    fructus = lapifex_lexere_et_parsare(
        tabula, fons, longitudo,
        _nuntium_schema_reductio, &ctx);

    si (!fructus.successus) redde NIHIL;

    /* Fructus.valor est index nodi radicis */
    redde _capere_nodum(&ctx, fructus.valor);
}
