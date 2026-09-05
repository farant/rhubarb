/* briar_fabrica.c - Vide briar_fabrica.h. */

#include "postulata_posix.h"
#include "briar_fabrica.h"
#include "briar_arbor.h"
#include "briar_silva.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "sigillum.h"
#include "tabula_dispersa.h"
#include "via.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>


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

/* spatia finalia abscisa */
interior chorda
_detondere (
    chorda c)
{
    dum (   c.mensura > ZEPHYRUM
         && ((character)c.datum[c.mensura - I] == '\n'
            || (character)c.datum[c.mensura - I] == '\r'
            || (character)c.datum[c.mensura - I] == ' '
            || (character)c.datum[c.mensura - I] == '\t'))
    {
        c.mensura = c.mensura - I;
    }
    redde c;
}

/* linea (I-basata) in qua offset iacet */
interior i32
_linea_octeti (
    chorda textus,
       i32 offset)
{
    i32 linea = I;
    i32 i;

    per (i = ZEPHYRUM; i < offset && i < textus.mensura; i++)
    {
        si ((character)textus.datum[i] == '\n')
        {
            linea = linea + I;
        }
    }
    redde linea;
}

interior b32
_silva_chorda_est (
            SilvaChorda  s,
     constans character* literae)
{
    redde (b32)(s.mensura == (insignatus integer)strlen(literae)
        && memcmp(s.datum, literae, (size_t)s.mensura) == ZEPHYRUM);
}

interior vacuum
_recusare (
    BriarFabricaFructus* f,
                Piscina* piscina,
     constans character* causa,
                    i32  linea)
{
    f->successus     = FALSUM;
    f->causa         = _literae(piscina, causa);
    f->linea_causae  = linea;
    f->genitae       = NIHIL;
    f->clausura      = NIHIL;
}

interior vacuum
_genitam_addere (
               Piscina* piscina,
                   Xar* genitae,
    constans character* via,
                chorda  contentum)
{
    BriarPlagula* p = (BriarPlagula*)xar_addere(genitae);

    si (p != NIHIL)
    {
        p->via        = _literae(piscina, via);
        p->contentum  = contentum;
    }
}

interior vacuum
_lineam_appendere (
     ChordaAedificator* a,
                   i32  linea,
    constans character* via)
{
    character b[32];

    sprintf(b, "#line %d \"", (integer)linea);
    chorda_aedificator_appendere_literis(a, b);
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a, "\"\n");
}

b32
briar_directoria_creare (
               Piscina* piscina,
    constans character* via)
{
          i32  m = (i32)strlen(via);
    character* gradus = (character*)piscina_allocare(piscina,
        (memoriae_index)(m + I));
    i32 i;

    per (i = ZEPHYRUM; i < m; i++)
    {
        gradus[i] = via[i];
        si (via[i] == '/' && i > ZEPHYRUM)
        {
            gradus[i] = '\0';
            (vacuum)filum_directorium_creare_si_necesse(gradus);
            gradus[i] = '/';
        }
    }
    gradus[m] = '\0';
    redde filum_directorium_creare_si_necesse(gradus);
}


/* ==================================================
 * Titulus, vexilla, clavis
 * ================================================== */

constans character*
briar_fabrica_titulus (
               Piscina* piscina,
    constans character* via)
{
    constans character* basis = via;
    constans character* p;
             character* t;
                   i32  m;
                   i32  i;

    per (p = via; *p != '\0'; p++)
    {
        si (*p == '/')
        {
            basis = p + I;
        }
    }
    m = (i32)strlen(basis);
    si (m > VIII && strcmp(basis + (m - VIII), ".thistle") == ZEPHYRUM)
    {
        m = m - VIII;
    }
    si (m == ZEPHYRUM)
    {
        redde "thistle";
    }
    t = (character*)piscina_allocare(piscina, (memoriae_index)(m + I));
    per (i = ZEPHYRUM; i < m; i++)
    {
        character c = basis[i];
              b32 litera = (b32)((c >= 'a' && c <= 'z')
                  || (c >= 'A' && c <= 'Z')
                  || (c >= '0' && c <= '9') || c == '_');

        t[i] = litera ? c : '_';
    }
    t[m] = '\0';
    redde t;
}

constans character*
briar_fabrica_vexilla (
    BriarForma forma)
{
    si (forma == BRIAR_FORMA_VITREA)
    {
        redde SILEX_VEXILLA_VITREA " | " SILEX_VEXILLA_VENDITORIA;
    }
    redde SILEX_VEXILLA_COMPILATIONIS;
}

vacuum
briar_fabrica_clavem_computare (
    constans character* stampa,
    constans character* vexilla,
                chorda  octeti,
             character* sigillum_xvii)
{
    SigillumContextus ctx;
             Sigillum s;
            character hex[SIGILLUM_HEX_MENSURA];

    sigillum_incipere(&ctx);
    sigillum_addere(&ctx, stampa, (memoriae_index)strlen(stampa));
    sigillum_addere(&ctx, "\n", (memoriae_index)I);
    sigillum_addere(&ctx, vexilla, (memoriae_index)strlen(vexilla));
    sigillum_addere(&ctx, "\n", (memoriae_index)I);
    sigillum_addere(&ctx, octeti.datum, (memoriae_index)octeti.mensura);
    s = sigillum_finire(&ctx);
    sigillum_hex(&s, hex);
    memcpy(sigillum_xvii, hex, (size_t)16);
    sigillum_xvii[16] = '\0';
}


/* ==================================================
 * Inventarium regionum
 * ================================================== */

nomen structura {
     BriarNexusRes** app;          /* regiones C non-probatio, ordine */
               i32   numerus_app;
     BriarNexusRes*  probatio;     /* aut NIHIL */
     BriarNexusRes*  fenestra;     /* elementum */
     BriarNexusRes*  html;
     BriarNexusRes*  js;
     BriarNexusRes*  css;
} BriarInventarium;

interior b32
_inventarium_colligere (
               Piscina* piscina,
                   Xar* nexus,
      BriarInventarium* inv,
   BriarFabricaFructus* f)
{
    i32 i;
    i32 n = xar_numerus(nexus);

    memset(inv, 0, magnitudo(*inv));
    inv->app = (BriarNexusRes**)piscina_allocare(piscina,
        (memoriae_index)((n + I) * (i32)magnitudo(BriarNexusRes*)));
    per (i = ZEPHYRUM; i < n; i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

        si (   r->genus == BRIAR_NEXUS_REGIO
            && briar_nexus_titulus_est(r, "c"))
        {
            si (r->silva == NIHIL || r->silva->parsura == NIHIL)
            {
                _recusare(f, piscina, r->causa.mensura > ZEPHYRUM
                    ? chorda_ut_cstr(r->causa, piscina)
                    : "regio C non parsata (briar_silvam_texere ante)",
                    r->linea_erroris > ZEPHYRUM ? r->linea_erroris
                    : r->linea_initium);
                redde FALSUM;
            }
            si (r->linea_erroris > ZEPHYRUM)
            {
                _recusare(f, piscina, chorda_ut_cstr(r->causa, piscina),
                    r->linea_erroris);
                redde FALSUM;
            }
            si (chorda_aequalis_literis(briar_nexus_attributum(r,
                "munus"),
                "probatio"))
            {
                si (inv->probatio != NIHIL)
                {
                    _recusare(f, piscina,
                        "regio probationis iterata (una in plano I)",
                        r->linea_initium - I);
                    redde FALSUM;
                }
                inv->probatio = r;
            }
            alioquin
            {
                inv->app[inv->numerus_app]  = r;
                inv->numerus_app            = inv->numerus_app + I;
            }
        }
        alioquin si (   r->genus == BRIAR_NEXUS_STML
                     && briar_nexus_titulus_est(r, "fenestra"))
        {
            inv->fenestra = r;
        }
        alioquin si (r->genus == BRIAR_NEXUS_REGIO)
        {
            BriarNexusRes** sedes = briar_nexus_titulus_est(r, "html")
                ? &inv->html : briar_nexus_titulus_est(r,
                "js") ? &inv->js
                : briar_nexus_titulus_est(r, "css") ? &inv->css : NIHIL;

            si (sedes != NIHIL)
            {
                si (*sedes != NIHIL)
                {
                    character b[96];

                    sprintf(b, "regio %.*s iterata (prima linea %d)",
                        (integer)r->titulus.mensura,
                        (constans character*)r->titulus.datum,
                        (integer)((*sedes)->linea_initium - I));
                    _recusare(f, piscina, b, r->linea_initium - I);
                    redde FALSUM;
                }
                *sedes = r;
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Partitio unitatum (silva)
 * ================================================== */

nomen structura {
       i32 linea;    /* .thistle */
    chorda textus;
} BriarUnitas;

nomen structura {
                Xar* directivae;   /* lineae '#...' */
                Xar* typi;         /* unitates sine obiecto */
                Xar* prototypi;    /* 'caput;' definitionum */
                Xar* corpora;      /* obiecta + definitiones */
                Xar* derivata;     /* chorda: capita derivata */

    BriarUnitas princeps;
            i32 principalia;  /* numerus unitatum 'main' */
            i32 linea_principalis_secundi;
} BriarPartitio;

interior vacuum
_unitatem_addere (
       Xar* xar,
       i32  linea,
    chorda  textus)
{
    BriarUnitas* u = (BriarUnitas*)xar_addere(xar);

    si (u != NIHIL)
    {
        u->linea   = linea;
        u->textus  = textus;
    }
}

/* directivae textuales (lineae quarum character primus non albus '#',
 * cum continuationibus '\') - silva eas consumit, textus eas servat */
interior vacuum
_directivas_colligere (
    constans BriarNexusRes* r,
                       Xar* directivae)
{
    chorda c      = r->contentum;
       i32 i      = ZEPHYRUM;
       i32 linea  = r->linea_initium;

    dum (i < c.mensura)
    {
        i32 initium  = i;
        i32 p        = i;
        i32 finis;

        dum (   p < c.mensura && ((character)c.datum[p] == ' '
            || (character)c.datum[p] == '\t'))
        {
            p = p + I;
        }
        finis = initium;
        dum (finis < c.mensura && (character)c.datum[finis] != '\n')
        {
            finis = finis + I;
        }
        si (p < c.mensura && (character)c.datum[p] == '#')
        {
            i32 f       = finis;
            i32 lineae  = I;

            dum (   f > initium && f < c.mensura
                 && (character)c.datum[f - I] == '\\')
            {
                f = f + I;
                dum (f < c.mensura && (character)c.datum[f] != '\n')
                {
                    f = f + I;
                }
                lineae = lineae + I;
            }
            _unitatem_addere(directivae, linea, chorda_sectio(c,
                initium, f));
            finis = f;
            linea = linea + lineae - I;
        }
        i      = (finis < c.mensura) ? finis + I : finis;
        linea  = linea + I;
    }
}

interior constans SemanticaSymbolum*
_symbolum_definitionis (
    constans SilvaSemantica* sem,
        constans SilvaNodus* unitas)
{
    insignatus integer k;

    per (k = ZEPHYRUM; k < silva_c89_symbola_numerus(sem); k++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(sem,
            k);

        si (   s->declarans == unitas
            && s->genus     == (integer)SYMBOLUM_FUNCTIO)
        {
            redde s;
        }
    }
    redde NIHIL;
}

interior b32
_unitas_obiectum_declarat (
    constans SilvaSemantica* sem,
                    integer  fons_index,
                    integer  minimum,
                    integer  maximum)
{
    insignatus integer k;

    per (k = ZEPHYRUM; k < silva_c89_symbola_numerus(sem); k++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(sem,
            k);

        si (   s->genus               == (integer)SYMBOLUM_VARIABILE
            && s->profunditas         == (insignatus integer)ZEPHYRUM
            && s->lexema              != NIHIL
            && s->lexema->fons_index  == fons_index
            && s->lexema->byte_offset >= minimum
            && s->lexema->byte_offset < maximum)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* capita derivata regionis in xar (dedup, ordo primae visionis) */
interior vacuum
_derivata_addere (
                       Xar* derivata,
    constans BriarNexusRes* r)
{
    i32 k;

    si (r->silva == NIHIL || r->silva->capita_derivata == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(r->silva->capita_derivata); k++)
    {
        chorda c = *(chorda*)xar_obtinere(r->silva->capita_derivata, k);
           i32 i;
           b32 visa = FALSUM;

        per (i = ZEPHYRUM; i < xar_numerus(derivata); i++)
        {
            si (chorda_aequalis(*(chorda*)xar_obtinere(derivata, i), c))
            {
                visa = VERUM;
            }
        }
        si (!visa)
        {
            chorda* cella = (chorda*)xar_addere(derivata);

            *cella = c;
        }
    }
}

/* lineae '#include "x.h"' capitum derivatorum (pro capite genito,
 * unitate probationis, clausura) */
interior chorda
_inclusiones_derivatae (
    Piscina* piscina,
        Xar* derivata)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)256);
    i32 i;

    per (i = ZEPHYRUM; derivata != NIHIL
        && i < xar_numerus(derivata); i++)
    {
        chorda_aedificator_appendere_literis(a, "#include \"");
        chorda_aedificator_appendere_chorda(a,
            *(chorda*)xar_obtinere(derivata, i));
        chorda_aedificator_appendere_literis(a, "\"\n");
    }
    redde chorda_aedificator_finire(a);
}

interior b32
_regionem_partiri (
                   Piscina* piscina,
    constans BriarNexusRes* r,
             BriarPartitio* part,
       BriarFabricaFructus* f)
{
              SilvaValor  radix = r->silva->parsura->commissio->radix;
                SilvaXar* liberi = NIHIL;
      insignatus integer  numerus = ZEPHYRUM;
      insignatus integer  k;
                 integer  fons_index = r->silva->parsura->fons_princeps;

        _directivas_colligere(r, part->directivae);
    _derivata_addere(part->derivata, r);

    /* radix commissionis: LISTA unitatum (parsura sana) aut NODUS -
     * utraque forma ambulatur */
    si (radix.genus == SILVA_VALOR_LISTA)
    {
        numerus = silva_valor_lista_numerus(radix);
    }
    alioquin si (radix.genus == SILVA_VALOR_NODUS)
    {
        liberi = silva_nodus_liberi(r->silva->piscina,
            radix.datum.nodus);
        numerus = (liberi
            != NIHIL) ? silva_xar_numerus(liberi) : ZEPHYRUM;
    }
    alioquin
    {
        _recusare(f, piscina, "regio C: arbor silvae sine radice",
            r->linea_initium);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans SilvaNodus* u;
                    integer  minimum = -I;
                    integer  maximum = ZEPHYRUM;
                     chorda  textus;
                        i32  linea;

        si (liberi != NIHIL)
        {
            u = *(SilvaNodus**)silva_xar_obtinere(liberi, k);
        }
        alioquin
        {
            SilvaValor* e = silva_valor_lista_obtinere(radix, k);

            si (e == NIHIL || e->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            u = e->datum.nodus;
        }
        silva_nodus_extensionem(u, fons_index, &minimum, &maximum);
        si (   minimum < ZEPHYRUM
            || minimum < (integer)r->praeludium_octeti)
        {
            perge;   /* syntheticum aut praeludium (exemplar) */
        }
        textus = chorda_sectio(r->textus_silvae, (i32)minimum,
            (i32)maximum);
        linea  = briar_nexus_linea_silvae(r,
            _linea_octeti(r->textus_silvae, (i32)minimum));
        si (u->genus == (integer)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
        {
            constans SemanticaSymbolum* s = _symbolum_definitionis(
                r->silva->semantica, u);
            SilvaValor corpus =
                silva_c89_definitio_functionis_corpus(u);
               integer initium_corporis  = -I;
               integer finis_corporis    = ZEPHYRUM;

            si (s != NIHIL && _silva_chorda_est(s->titulus, "main"))
            {
                part->principalia = part->principalia + I;
                si (part->principalia == I)
                {
                    part->princeps.linea   = linea;
                    part->princeps.textus  = textus;
                }
                alioquin
                {
                    part->linea_principalis_secundi = linea;
                }
                perge;
            }
            si (corpus.genus == SILVA_VALOR_NODUS)
            {
                silva_nodus_extensionem(corpus.datum.nodus, fons_index,
                    &initium_corporis, &finis_corporis);
            }
            si (initium_corporis > minimum)
            {
                ChordaAedificator* a =
                    chorda_aedificator_creare(piscina,
                    (memoriae_index)256);

                chorda_aedificator_appendere_chorda(a, _detondere(
                    chorda_sectio(r->textus_silvae, (i32)minimum,
                        (i32)initium_corporis)));
                chorda_aedificator_appendere_literis(a, ";");
                _unitatem_addere(part->prototypi, linea,
                    chorda_aedificator_finire(a));
            }
            _unitatem_addere(part->corpora, linea, textus);
        }
        alioquin si (_unitas_obiectum_declarat(r->silva->semantica,
                     fons_index, minimum, maximum))
        {
            _unitatem_addere(part->corpora, linea, textus);
        }
        alioquin
        {
            _unitatem_addere(part->typi, linea, textus);
        }
    }
    redde VERUM;
}


/* ==================================================
 * Textus geniti
 * ================================================== */

interior vacuum
_unitates_appendere (
     ChordaAedificator* a,
                   Xar* unitates,
    constans character* via)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(unitates); i++)
    {
        constans BriarUnitas* u = (constans BriarUnitas*)xar_obtinere(
            unitates, i);

        _lineam_appendere(a, u->linea, via);
        chorda_aedificator_appendere_chorda(a, u->textus);
        chorda_aedificator_appendere_literis(a, "\n");
    }
}

interior constans character*
_custos (
               Piscina* piscina,
    constans character* titulus)
{
          i32  m = (i32)strlen(titulus);
    character* c = (character*)piscina_allocare(piscina,
        (memoriae_index)(m + 12));
    i32 i;

    per (i = ZEPHYRUM; i < m; i++)
    {
        character x = titulus[i];

        c[i] = (x >= 'a' && x <= 'z') ? (character)(x - 'a' + 'A') : x;
    }
    c[m] = '\0';
    strcat(c, "_REGIONES_H");
    redde c;
}

interior chorda
_caput_fingere (
                   Piscina* piscina,
        constans character* titulus,
        constans character* via,
    constans BriarPartitio* part)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)4096);
    constans character* custos = _custos(piscina, titulus);

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "_regiones.h - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": directivae, typi, prototypi regionum */\n#ifndef ");
    chorda_aedificator_appendere_literis(a, custos);
    chorda_aedificator_appendere_literis(a, "\n#define ");
    chorda_aedificator_appendere_literis(a, custos);
    /* capita implicita: latina.h + trias vulgaris (stdio/stdlib/string)
     * - plagula thistle scriptum est; imprimere sine stdio.h non
     * compilat */
    chorda_aedificator_appendere_literis(a,
        "\n#include \"latina.h\"\n"
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
                "#include <string.h>\n");
    /* capita DERIVATA ex usu symbolorum (briar_silva) - sine #line */
    chorda_aedificator_appendere_chorda(a,
        _inclusiones_derivatae(piscina, part->derivata));
    _unitates_appendere(a, part->directivae, via);
    _unitates_appendere(a, part->typi, via);
    _unitates_appendere(a, part->prototypi, via);
    chorda_aedificator_appendere_literis(a, "#endif /* ");
    chorda_aedificator_appendere_literis(a, custos);
    chorda_aedificator_appendere_literis(a, " */\n");
    redde chorda_aedificator_finire(a);
}

interior chorda
_corpus_fingere (
                   Piscina* piscina,
        constans character* titulus,
        constans character* via,
    constans BriarPartitio* part)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)4096);

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "_regiones.c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": obiecta et definitiones regionum */\n"
        "#include \"latina.h\"\n#include \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_regiones.h\"\n");
    _unitates_appendere(a, part->corpora, via);
    redde chorda_aedificator_finire(a);
}

interior chorda
_principem_fingere (
                 Piscina* piscina,
      constans character* titulus,
      constans character* via,
    constans BriarUnitas* princeps)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)2048);

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": principale */\n#include \"latina.h\"\n#include \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_regiones.h\"\n");
    _lineam_appendere(a, princeps->linea, via);
    chorda_aedificator_appendere_chorda(a, princeps->textus);
    chorda_aedificator_appendere_literis(a, "\n");
    redde chorda_aedificator_finire(a);
}

interior chorda
_probationem_fingere (
                   Piscina* piscina,
        constans character* titulus,
        constans character* via,
    constans BriarNexusRes* probatio,
                       Xar* derivata)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)(probatio->contentum.mensura + 256));

    chorda_aedificator_appendere_literis(a, "/* probatio_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": regio munus=\"probatio\" */\n#include \"latina.h\"\n"
        "#include \"");
    chorda_aedificator_appendere_literis(a, titulus);
        chorda_aedificator_appendere_literis(a, "_regiones.h\"\n");
    chorda_aedificator_appendere_chorda(a,
        _inclusiones_derivatae(piscina, derivata));
    _lineam_appendere(a, probatio->linea_initium, via);
    chorda_aedificator_appendere_chorda(a, probatio->contentum);
    redde chorda_aedificator_finire(a);
}


/* ==================================================
 * Forma vitrea: <fenestra/>, methodi, principale genitum, assets
 * ================================================== */

interior b32
_numerum_legere (
    chorda  c,
       i32* valor)
{
    i32 i;
    i32 v = ZEPHYRUM;

    si (c.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        character d = (character)c.datum[i];

        si (d < '0' || d > '9')
        {
            redde FALSUM;
        }
        v = v * X + (i32)(d - '0');
    }
    *valor = v;
    redde VERUM;
}

/* methodus="nomen": functio 'nomen' in regione definita et cum
 * exemplari tractatoris compatibilis; recusatio nominat lineam tagi */
interior b32
_methodum_probare (
                   Piscina* piscina,
    constans BriarNexusRes* r,
                    chorda  methodus,
       BriarFabricaFructus* f)
{
       constans SilvaSemantica* sem       = r->silva->semantica;
    constans SemanticaSymbolum* functio   = NIHIL;
    constans SemanticaSymbolum* exemplar  = NIHIL;
            insignatus integer  k;
                     character  b[160];
                           i32  linea_tagi = r->linea_initium - I;

    per (k = ZEPHYRUM; k < silva_c89_symbola_numerus(sem); k++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(sem,
            k);

        si (s->profunditas != (insignatus integer)ZEPHYRUM)
        {
            perge;
        }
        si (   s->genus == (integer)SYMBOLUM_FUNCTIO
            && s->titulus.mensura
                == (insignatus integer)methodus.mensura
            && memcmp(s->titulus.datum, methodus.datum,
                (size_t)methodus.mensura) == ZEPHYRUM
            && silva_c89_definitio_functionis_corpus(s->declarans).genus
                != SILVA_VALOR_NIHIL)
        {
            functio = s;
        }
        si (_silva_chorda_est(s->titulus, "briar_tractator_exemplar"))
        {
            exemplar = s;
        }
    }
    si (functio == NIHIL)
    {
        sprintf(b, "methodus '%.*s' (linea %d): functio non definita in"
            " regione", (integer)methodus.mensura,
            (constans character*)methodus.datum, (integer)linea_tagi);
        _recusare(f, piscina, b, linea_tagi);
        redde FALSUM;
    }
    si (   exemplar               == NIHIL
        || exemplar->typus->genus != (integer)TYPUS_C89_MONSTRATOR
        || !silva_c89_typi_compatibiles(
            exemplar->typus->datum.monstrator.internum, functio->typus))
    {
        sprintf(b,
            "methodus '%.*s' (linea %d): signatura aliena; exspectata"
            " JsonValor* f(JsonValor*, Piscina*, vacuum*, chorda*)",
            (integer)methodus.mensura,
            (constans character*)methodus.datum,
            (integer)linea_tagi);
        _recusare(f, piscina, b, linea_tagi);
        redde FALSUM;
    }
    redde VERUM;
}

interior chorda
_principem_vitreum_fingere (
                      Piscina* piscina,
           constans character* titulus,
           constans character* via,
    constans BriarInventarium* inv,
          BriarFabricaFructus* f)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)4096);
    chorda fenestrae_titulus = briar_nexus_attributum(inv->fenestra,
        "titulus");
    chorda latitudo = briar_nexus_attributum(inv->fenestra,
        "latitudo");
    chorda altitudo = briar_nexus_attributum(inv->fenestra,
        "altitudo");
          i32 numerus;
          i32 i;
    character b[96];

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": fenestra vitrea, methodi regionum praebitae.\n"
        " * Gyrus atrii hic (vide include/atrium.h); status usoris"
        " NIHIL (plan 2). */\n"
        "#include \"latina.h\"\n"
        "#include \"piscina.h\"\n"
        "#include \"chorda.h\"\n"
        "#include \"json.h\"\n"
        "#include \"atrium.h\"\n"
        "#include \"internuntius.h\"\n"
        "#include \"capsula_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".h\"\n#include \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "_regiones.h\"\n"
        "#include <stdio.h>\n"
        "#include <string.h>\n"
        "\n"
        "s32\n"
        "principale (integer argc, character** argv)\n"
        "{\n"
        "    Piscina*            piscina;\n"
        "    AtriumConfiguratio  figura;\n"
        "    Atrium*             atrium;\n"
        "    chorda              causa;\n"
        "\n"
        "    piscina = piscina_generare_dynamicum(\"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "\", 16777216);\n"
        "    si (piscina == NIHIL)\n"
        "    {\n"
        "        redde I;\n"
        "    }\n"
        "    memset(&figura, 0, magnitudo(figura));\n"
        "    figura.titulus  = \"");
    si (fenestrae_titulus.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, fenestrae_titulus);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, titulus);
    }
    chorda_aedificator_appendere_literis(a, "\";\n");
    si (latitudo.mensura > ZEPHYRUM)
    {
        si (!_numerum_legere(latitudo, &numerus))
        {
            sprintf(b, "<fenestra> (linea %d): latitudo non numerus",
                (integer)inv->fenestra->linea_initium);
            _recusare(f, piscina, b, inv->fenestra->linea_initium);
            redde _vacua();
        }
        sprintf(b, "    figura.latitudo = %d;\n", (integer)numerus);
        chorda_aedificator_appendere_literis(a, b);
    }
    si (altitudo.mensura > ZEPHYRUM)
    {
        si (!_numerum_legere(altitudo, &numerus))
        {
            sprintf(b, "<fenestra> (linea %d): altitudo non numerus",
                (integer)inv->fenestra->linea_initium);
            _recusare(f, piscina, b, inv->fenestra->linea_initium);
            redde _vacua();
        }
        sprintf(b, "    figura.altitudo = %d;\n", (integer)numerus);
        chorda_aedificator_appendere_literis(a, b);
    }
    chorda_aedificator_appendere_literis(a,
        "    figura.capsula  = &capsula_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        ";\n"
        "    atrium_vexilla_legere(&figura, argc, argv);\n"
        "\n"
        "    atrium = atrium_creare(piscina, &figura, &causa);\n"
        "    si (atrium == NIHIL)\n"
        "    {\n"
        "        imprimere(\"FRACTA: %.*s\\n\","
        " (integer)causa.mensura,\n"
        "            (constans character*)causa.datum);\n"
        "        redde I;\n"
        "    }\n");
    per (i = ZEPHYRUM; i < inv->numerus_app; i++)
    {
        chorda methodus = briar_nexus_attributum(inv->app[i],
            "methodus");

        si (methodus.mensura == ZEPHYRUM)
        {
            perge;
        }
        si (!_methodum_probare(piscina, inv->app[i], methodus, f))
        {
            redde _vacua();
        }
        chorda_aedificator_appendere_literis(a,
            "    (vacuum)internuntius_praebere("
            "atrium_internuntius(atrium),\n"
            "        \"");
        chorda_aedificator_appendere_chorda(a, methodus);
        chorda_aedificator_appendere_literis(a, "\", ");
        chorda_aedificator_appendere_chorda(a, methodus);
        chorda_aedificator_appendere_literis(a, ", NIHIL);\n");
    }
    chorda_aedificator_appendere_literis(a,
        "\n"
        "    imprimere(\"[");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "] fenestra aperta\\n\");\n"
        "    si (atrium_portus(atrium) != ZEPHYRUM)\n"
        "    {\n"
        "        imprimere(\"[");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "] imperium: http://127.0.0.1:%d/imperium\\n\",\n"
        "            (integer)atrium_portus(atrium));\n"
        "    }\n"
        "    fflush(stdout);\n"
        "    atrium_monstrare(atrium);\n"
        "    dum (atrium_currendum(atrium))\n"
        "    {\n"
        "        (vacuum)atrium_gressus(atrium);\n"
        "    }\n"
        "    atrium_destruere(atrium);\n"
        "    piscina_destruere(piscina);\n"
        "    redde ZEPHYRUM;\n"
        "}\n");
    redde chorda_aedificator_finire(a);
}

interior chorda
_toml_fingere (
                      Piscina* piscina,
           constans character* titulus,
    constans BriarInventarium* inv)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)512);

    chorda_aedificator_appendere_literis(a,
        "# Capsula frontis (a briar genita) - aedificare.sh eam omni"
        " aedificatione regenerat\n\n");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_files = [\"index.html\"");
    si (inv->js != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, ", \"");
        chorda_aedificator_appendere_literis(a, titulus);
        chorda_aedificator_appendere_literis(a, ".js\"");
    }
    si (inv->css != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, ", \"");
        chorda_aedificator_appendere_literis(a, titulus);
        chorda_aedificator_appendere_literis(a, ".css\"");
    }
    chorda_aedificator_appendere_literis(a, "]\n");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_compress = false\n");
    redde chorda_aedificator_finire(a);
}

/* unio clausurarum per viam (dedup, ordo primae visionis) */
interior Xar*
_clausuras_fundere (
    Piscina* piscina,
        Xar* a,
        Xar* b,
        Xar* c)
{
               Xar* omnes;
    TabulaDispersa* viae;
               Xar* fontes[3];
               i32  j;

    omnes  = xar_creare(piscina, (i32)magnitudo(SilexRes));
    viae   = tabula_dispersa_creare_chorda(piscina, 64);

    fontes[0] = a;
    fontes[1] = b;
    fontes[2] = c;
    per (j = ZEPHYRUM; j < III; j++)
    {
        i32 i;

        si (fontes[j] == NIHIL)
        {
            perge;
        }
        per (i = ZEPHYRUM; i < xar_numerus(fontes[j]); i++)
        {
            SilexRes* r = (SilexRes*)xar_obtinere(fontes[j], i);

            si (tabula_dispersa_continet(viae, r->via))
            {
                perge;
            }
            tabula_dispersa_inserere(viae, r->via, (vacuum*)r);
            {
                SilexRes* cella = (SilexRes*)xar_addere(omnes);

                si (cella != NIHIL)
                {
                    *cella = *r;
                }
            }
        }
    }
    redde omnes;
}


/* ==================================================
 * Fabricare
 * ================================================== */

BriarFabricaFructus
briar_fabricare (
                          Piscina* piscina,
            constans MateriaNodus* documentum,
                              Xar* nexus,
               constans SilexFons* fons,
    constans BriarFabricaOptiones* optiones,
                           chorda  octeti)
{
        BriarFabricaFructus  f;
           BriarInventarium  inv;
              BriarPartitio  part;
                        i32  i;
         constans character* via;
         constans character* fontes_app[2];
         constans character* fontes_prob[2];
                        Xar* derivata_probationis;
                     chorda  inclusiones_derivatae;
                     chorda  inclusiones_probationis;


    memset(&f, 0, magnitudo(f));
    si (   piscina == NIHIL || documentum == NIHIL || nexus == NIHIL
        || fons    == NIHIL || optiones == NIHIL)
    {
        _recusare(&f, piscina, "argumenta nulla", ZEPHYRUM);
        redde f;
    }
    via        = optiones->via_thistle;
    f.titulus  = briar_fabrica_titulus(piscina, via);
    si (!_inventarium_colligere(piscina, nexus, &inv, &f))
    {
        redde f;
    }
    f.regiones_c      = inv.numerus_app;
    f.probatio_adest  = (b32)(inv.probatio != NIHIL);

    memset(&part, 0, magnitudo(part));
    part.directivae = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.typi = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.prototypi = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
        part.corpora = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.derivata = xar_creare(piscina, (i32)magnitudo(chorda));

    per (i = ZEPHYRUM; i < inv.numerus_app; i++)
    {
        si (!_regionem_partiri(piscina, inv.app[i], &part, &f))
        {
            redde f;
        }
    }

    /* regula principalis */
    si (part.principalia > I)
    {
        character b[96];

        sprintf(b, "duo principalia: lineae %d et %d",
            (integer)part.princeps.linea,
            (integer)part.linea_principalis_secundi);
        _recusare(&f, piscina, b, part.linea_principalis_secundi);
        redde f;
    }
    si (part.principalia == I && inv.fenestra != NIHIL)
    {
        character b[96];

        sprintf(b,
            "<fenestra> (linea %d) et principale (linea %d): unum"
            " elige",
            (integer)inv.fenestra->linea_initium,
            (integer)part.princeps.linea);
        _recusare(&f, piscina, b, inv.fenestra->linea_initium);
        redde f;
    }
    si (part.principalia == ZEPHYRUM && inv.fenestra == NIHIL)
    {
        _recusare(&f, piscina,
            "nec principale in regione C nec <fenestra/>: nihil"
            " currendum",
            ZEPHYRUM);
        redde f;
    }
    f.forma = (part.principalia
        == I) ? BRIAR_FORMA_PLANA : BRIAR_FORMA_VITREA;

        /* capita derivata probationis: unitas sua ea includit */
    derivata_probationis = xar_creare(piscina, (i32)magnitudo(chorda));
    si (inv.probatio != NIHIL)
    {
        _derivata_addere(derivata_probationis, inv.probatio);
    }
    inclusiones_derivatae = _inclusiones_derivatae(piscina,
        part.derivata);
    inclusiones_probationis = _inclusiones_derivatae(piscina,
        derivata_probationis);
    f.genitae = xar_creare(piscina, (i32)magnitudo(BriarPlagula));
    /* caput in include/: -Iinclude ordinum id omnibus unitatibus
     * praebet (probationes/ quoque), fontes/ soli non */
    _genitam_addere(piscina, f.genitae,
        _texere(piscina, "include/", f.titulus, "_regiones.h"),
        _caput_fingere(piscina, f.titulus, via, &part));
    _genitam_addere(piscina, f.genitae,
        _texere(piscina, "fontes/", f.titulus, "_regiones.c"),
        _corpus_fingere(piscina, f.titulus, via, &part));
    si (inv.probatio != NIHIL)
    {
        _genitam_addere(piscina, f.genitae,
            _texere(piscina, "probationes/probatio_", f.titulus, ".c"),
                        _probationem_fingere(piscina, f.titulus, via,
                        inv.probatio,
                derivata_probationis));
    }

    fontes_app[0] = _texere(piscina, "fontes/", f.titulus, ".c");
    fontes_app[1] = _texere(piscina, "fontes/", f.titulus,
        "_regiones.c");
    fontes_prob[0] = _texere(piscina, "probationes/probatio_",
        f.titulus,
        ".c");
    fontes_prob[1] = fontes_app[1];

    si (f.forma == BRIAR_FORMA_PLANA)
    {
        chorda* contenta;
           i32  n = ZEPHYRUM;

        _genitam_addere(piscina, f.genitae, fontes_app[0],
            _principem_fingere(piscina, f.titulus, via,
            &part.princeps));
        /* clausura: regiones omnes (probatio inclusa - credo.h) */
                contenta = (chorda*)piscina_allocare(piscina,
                    (memoriae_index)((inv.numerus_app + III)
                    * (i32)magnitudo(chorda)));
        per (i = ZEPHYRUM; i < inv.numerus_app; i++)
        {
            contenta[n]  = inv.app[i]->contentum;
            n            = n + I;
        }
                si (inv.probatio != NIHIL)
                {
            contenta[n]  = inv.probatio->contentum;
            n            = n + I;
                }
        contenta[n]      = inclusiones_derivatae;
        contenta[n + I]  = inclusiones_probationis;
        n                = n + II;
        f.clausura = silex_clausuram_e_contentis(piscina, fons,
            contenta, n);
        si (f.clausura == NIHIL)
        {
            _recusare(&f, piscina, "clausura bibliothecarum fracta",
                ZEPHYRUM);
            redde f;
        }
        _genitam_addere(piscina, f.genitae, "aedificare.sh",
            silex_ordinem_fingere(piscina, f.titulus, fontes_app, II));
        si (inv.probatio != NIHIL)
        {
            _genitam_addere(piscina, f.genitae, "probare.sh",
                silex_ordinem_probandi_fingere(piscina, f.titulus,
                    fontes_prob, II));
        }
    }
    alioquin
    {
        chorda princeps = _principem_vitreum_fingere(piscina,
            f.titulus,
            via, &inv, &f);
        chorda  instrumentum;
        chorda* contenta_app;
        chorda* contenta_prob;
           Xar* clausura_app;
           Xar* clausura_instrumenti;
           Xar* clausura_prob  = NIHIL;
           b32  inventum       = FALSUM;
           i32  n;

        si (f.causa.mensura > ZEPHYRUM)
        {
            redde f;   /* recusatio methodi / attributi */
        }
        si (inv.html == NIHIL)
        {
            _recusare(&f, piscina,
                "<fenestra/> sine regione html: nihil monstrandum",
                inv.fenestra->linea_initium);
            redde f;
        }
        _genitam_addere(piscina, f.genitae, fontes_app[0], princeps);
        _genitam_addere(piscina, f.genitae, "assets/index.html",
            inv.html->contentum);
        si (inv.js != NIHIL)
        {
            _genitam_addere(piscina, f.genitae,
                _texere(piscina, "assets/", f.titulus, ".js"),
                inv.js->contentum);
        }
        si (inv.css != NIHIL)
        {
            _genitam_addere(piscina, f.genitae,
                _texere(piscina, "assets/", f.titulus, ".css"),
                inv.css->contentum);
        }
        _genitam_addere(piscina, f.genitae,
            _texere(piscina, "assets/", f.titulus, ".toml"),
            _toml_fingere(piscina, f.titulus, &inv));
        instrumentum = silex_fons_legere(fons,
            "tools/capsula_generare.c",
            piscina, &inventum);
        si (!inventum)
        {
            _recusare(&f, piscina,
                "tools/capsula_generare.c in fonte silicis deest",
                ZEPHYRUM);
            redde f;
        }
        _genitam_addere(piscina, f.genitae,
            "instrumenta/capsula_generare.c",
            instrumentum);

                contenta_app = (chorda*)piscina_allocare(piscina,
                    (memoriae_index)((inv.numerus_app + III)
                    * (i32)magnitudo(chorda)));
        contenta_prob = (chorda*)piscina_allocare(piscina,
            (memoriae_index)((inv.numerus_app + IV)
                * (i32)magnitudo(chorda)));
        per (i = ZEPHYRUM; i < inv.numerus_app; i++)
        {
            contenta_app[i]   = inv.app[i]->contentum;
            contenta_prob[i]  = inv.app[i]->contentum;
        }
                contenta_app[inv.numerus_app]  = princeps;
        contenta_app[inv.numerus_app + I]      = inclusiones_derivatae;
        clausura_app = silex_clausuram_e_contentis(piscina, fons,
            contenta_app, inv.numerus_app + II);
        clausura_instrumenti = silex_clausuram_e_contentis(piscina,
            fons,
            &instrumentum, I);
        n = inv.numerus_app;
        si (inv.probatio != NIHIL)
        {
                        contenta_prob[n]  = inv.probatio->contentum;
            contenta_prob[n + I]          = inclusiones_derivatae;
            contenta_prob[n + II]         = inclusiones_probationis;
            n                             = n + III;
            clausura_prob = silex_clausuram_e_contentis(piscina, fons,
                contenta_prob, n);
        }
        si (   clausura_app == NIHIL || clausura_instrumenti == NIHIL
            || (inv.probatio != NIHIL && clausura_prob == NIHIL))
        {
            _recusare(&f, piscina, "clausura bibliothecarum fracta",
                ZEPHYRUM);
            redde f;
        }
        _genitam_addere(piscina, f.genitae, "aedificare.sh",
            silex_ordinem_vitreum_fingere(piscina, f.titulus,
            fontes_app, II,
                clausura_app, clausura_instrumenti,
                optiones->fons_titulus));
        si (inv.probatio != NIHIL)
        {
            _genitam_addere(piscina, f.genitae, "probare.sh",
                silex_ordinem_probandi_vitreum_fingere(piscina,
                f.titulus,
                    fontes_prob, II, clausura_prob,
                    optiones->fons_titulus));
        }
        f.clausura = _clausuras_fundere(piscina, clausura_app,
            clausura_instrumenti, clausura_prob);
    }

    briar_fabrica_clavem_computare(optiones->stampa,
        briar_fabrica_vexilla(f.forma), octeti, f.sigillum);
    f.successus = VERUM;
    redde f;
}


/* ==================================================
 * Scriptor
 * ================================================== */

interior b32
_plagulam_scribere (
               Piscina* piscina,
    constans character* radix,
                chorda  via,
                chorda  contentum,
                chorda* causa)
{
    constans character* plena = _texere(piscina, radix, "/",
        chorda_ut_cstr(via, piscina));
                chorda parens = via_directorium(_literae(piscina,
                    plena),
                    piscina);

    (vacuum)briar_directoria_creare(piscina, chorda_ut_cstr(parens,
        piscina));
    si (!filum_scribere(plena, contentum))
    {
        *causa = _literae(piscina, _texere(piscina, "non scripta: ",
            plena,
            NIHIL));
        redde FALSUM;
    }
    redde VERUM;
}

b32
briar_fabricam_scribere (
                         Piscina* piscina,
    constans BriarFabricaFructus* fructus,
              constans character* radix,
                          chorda* causa)
{
    i32 i;

    *causa = _vacua();
    si (fructus == NIHIL || !fructus->successus || radix == NIHIL)
    {
        *causa = _literae(piscina, "fructus non sanus");
        redde FALSUM;
    }
    si (!briar_directoria_creare(piscina, radix))
    {
        *causa = _literae(piscina, _texere(piscina,
            "directorium non creatum: ", radix, NIHIL));
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->clausura); i++)
    {
        constans SilexRes* r = (constans SilexRes*)xar_obtinere(
            fructus->clausura, i);

        si (!_plagulam_scribere(piscina, radix, r->via, r->contentum,
            causa))
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->genitae); i++)
    {
        constans BriarPlagula* p = (constans BriarPlagula*)xar_obtinere(
            fructus->genitae, i);

        si (!_plagulam_scribere(piscina, radix, p->via, p->contentum,
            causa))
        {
            redde FALSUM;
        }
        si (   p->contentum.mensura > II
            && memcmp(p->contentum.datum, "#!", (size_t)II) == ZEPHYRUM)
        {
            (vacuum)chmod(_texere(piscina, radix, "/",
                chorda_ut_cstr(p->via, piscina)), (mode_t)0755);
        }
    }
    redde VERUM;
}
