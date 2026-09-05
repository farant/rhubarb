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
            Xar* directivae;   /* BriarUnitas: lineae '#...' regionum */
            Xar* typi;         /* BriarUnitas: unitates sine obiecto */
            Xar* prototypi;    /* BriarUnitas: 'caput;' definitionum */
            Xar* corpora;      /* BriarUnitas: obiecta + definitiones */
    BriarUnitas  princeps;
            i32  principalia;  /* numerus unitatum 'main' */
            i32  linea_principalis_secundi;
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
               integer cmin = -I;
               integer cmax = ZEPHYRUM;

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
                    &cmin, &cmax);
            }
            si (cmin > minimum)
            {
                ChordaAedificator* a =
                    chorda_aedificator_creare(piscina,
                    (memoriae_index)256);

                chorda_aedificator_appendere_chorda(a, _detondere(
                    chorda_sectio(r->textus_silvae, (i32)minimum,
                        (i32)cmin)));
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
    constans BriarNexusRes* probatio)
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
    _lineam_appendere(a, probatio->linea_initium, via);
    chorda_aedificator_appendere_chorda(a, probatio->contentum);
    redde chorda_aedificator_finire(a);
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
    part.directivae  = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.typi        = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.prototypi   = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.corpora     = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
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
            inv.probatio));
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
            (memoriae_index)((inv.numerus_app + I)
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
        _recusare(&f, piscina, "forma vitrea nondum (Task 4)",
            inv.fenestra->linea_initium);
        redde f;
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
