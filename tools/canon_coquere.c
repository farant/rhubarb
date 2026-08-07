/* canon_coquere.c - generatio codicis C ex canone (deserialisatio)
 *
 * Motus silva_coquere in strato documentorum: canon (systema
 * generum PARVUM consulto - sex genera, sacci cardinales) in
 * structuras C typatas et lectores generatos vertitur. Paupertas
 * deliberata canonis est quod generationem mundam sinit - via
 * qua JAXB periit (systema generum XSD nimis dives) hic clausa.
 *
 * Usus:
 *   bin/canon_coquere <via.canon> -praefixum <Pascal>
 *       -caput <include/x.h> -corpus <lib/x.c>
 *
 * QUOD GENERATUR (per elementum canonis):
 *   - structura typata: chorda* pro nomen/compositum/textus/dies,
 *     s64+praesens pro numero, b32+praesens pro veritate,
 *     enumeratio generata pro electione; monstrator pro libero
 *     0..1, Xar* pro 0..inf; textus normalizatus; StmlNodus*
 *     SEMPER retentum (profunditas hybrida - effugium ad DOM,
 *     linea gratis)
 *   - lector <praef>_<elem>_legere_nodum per elementum, et
 *     <praef>_<radix>_legere (parsatio + lectio) pro radice
 *   - praestituta (ordinarius=) APPLICANTUR: canon dicit, lector
 *     generatus applicat
 *
 * LEGES LECTORIS GENERATI:
 *   - folium: involve, numquam emenda (regeneratio eum vincit)
 *   - lectio sola (scriptura = foedus aliud, files=veritas)
 *   - plagulam CANONE SANAM praesumit: ignota tacite praeterit
 *   - identificatores reservati (macra latina, verba C89)
 *     suffixo '_v' sanantur: attributum 'per=' campus 'per_v' fit
 *
 * Definitiones intra= nomina qualificata accipiunt:
 *   <elementum nomen="regula" intra="nexus"> -> PraefNexusRegula.
 */
#include "postulata_posix.h"

#include "latina.h"
#include "canon.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Exemplar canonis in memoria (ordine documenti)
 * ================================================== */

nomen structura {
    chorda*            titulus;
    CanonGenusValoris  genus;
    b32                necessarium;
    chorda*            praestitutum;
    Xar*               optiones;     /* chorda* */
} CqAttributum;

nomen structura {
    chorda*  titulus;
    b32      unicum;   /* maximum == 1 */
} CqLiberum;

nomen structura {
    chorda*            titulus;
    chorda*            intra;        /* NIHIL = globale */
    b32                radix;
    b32                textus_licet;
    CanonGenusValoris  textus_genus;
    Xar*               attributa;    /* CqAttributum */
    Xar*               liberi;       /* CqLiberum */
} CqElementum;

/* ==================================================
 * Prototypa
 * ================================================== */

interior CanonGenusValoris _genus_ex(chorda* s);
interior b32 _reservatum(constans character* s);
interior vacuum _campus_scribere(FILE* f, constans chorda* titulus);
interior vacuum _camelus_scribere(FILE* f, constans chorda* t);
interior vacuum _typus_scribere(FILE* f,
    constans character* praefixum, CqElementum* e);
interior vacuum _functio_scribere(FILE* f,
    constans character* praefixum_min, CqElementum* e,
    constans character* suffixum);
interior vacuum _maiusculum_scribere(FILE* f, constans chorda* t);
interior vacuum _membrum_electionis_scribere(FILE* f,
    constans character* praefixum, CqElementum* e,
    CqAttributum* a, constans chorda* optio);
interior CqElementum* _def_invenire(Xar* elementa,
    constans chorda* parens, constans chorda* titulus);
interior Xar* _canonem_colligere(StmlNodus* radix_canonis,
    Piscina* piscina);
interior vacuum _caput_emittere(FILE* f, Xar* elementa,
    constans character* praefixum, constans character* praefixum_min,
    constans character* custos, constans character* via_canonis,
    constans character* iussum);
interior vacuum _corpus_emittere(FILE* f, Xar* elementa,
    constans character* praefixum, constans character* praefixum_min,
    constans character* caput_basis, constans character* via_canonis);
interior vacuum _lectorem_emittere(FILE* f, Xar* elementa,
    CqElementum* e, constans character* praefixum,
    constans character* praefixum_min);

/* ==================================================
 * Auxilia nominum
 * ================================================== */

interior constans character* constans RESERVATA[] = {
    /* macra latina - quodlibet verbum in latina.h definitum */
    "character", "brevis", "integer", "longus", "fluitans",
    "duplex", "vacuum", "signatus", "insignatus", "constans",
    "volatilis", "sponte", "registrum", "staticus", "externus",
    "si", "alioquin", "commutatio", "casus", "ordinarius", "per",
    "dum", "fac", "frange", "perge", "salta", "redde", "structura",
    "unio", "enumeratio", "nomen", "magnitudo", "principale",
    "imprimere", "interior", "hic_manens", "universalis", "exire",
    /* verba C89 */
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern", "float",
    "for", "goto", "if", "int", "long", "register", "return",
    "short", "signed", "sizeof", "static", "struct", "switch",
    "typedef", "union", "unsigned", "void", "volatile", "while",
    /* campi quos structurae generatae ipsae ferunt */
    "nodus", "textus", "textus_numerus", "textus_praesens",
    NIHIL
};

interior b32
_reservatum(
    constans character* s)
{
    i32 i;

    per (i = ZEPHYRUM; RESERVATA[i]; i++)
    {
        si (strcmp(s, RESERVATA[i]) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

/* nomen campi: minusculae, lineola -> subvirga; reservatum
 * suffixo '_v' sanatur */
interior vacuum
_campus_scribere(
    FILE*             f,
    constans chorda*  titulus)
{
    character buffer[CXXVIII];
    i32       i;
    i32       j;

    j = ZEPHYRUM;
    per (i = ZEPHYRUM;
         i < titulus->mensura && j < (i32)magnitudo(buffer) - III;
         i++)
    {
        character c;

        c = (character)titulus->datum[i];
        si (c == '-')
        {
            c = '_';
        }
        buffer[j++] = c;
    }
    buffer[j] = '\0';

    si (_reservatum(buffer))
    {
        buffer[j]     = '_';
        buffer[j + I] = 'v';
        buffer[j + II] = '\0';
    }

    fprintf(f, "%s", buffer);
}

/* CamelCase: littera prima cuiusque segmenti (-/_) maiuscula */
interior vacuum
_camelus_scribere(
    FILE*             f,
    constans chorda*  t)
{
    i32 i;
    b32 initium;

    initium = VERUM;
    per (i = ZEPHYRUM; i < t->mensura; i++)
    {
        character c;

        c = (character)t->datum[i];
        si (c == '-' || c == '_')
        {
            initium = VERUM;
            perge;
        }
        si (initium && c >= 'a' && c <= 'z')
        {
            c = (character)(c - 'a' + 'A');
        }
        putc(c, f);
        initium = FALSUM;
    }
}

interior vacuum
_typus_scribere(
    FILE*                f,
    constans character*  praefixum,
    CqElementum*         e)
{
    fprintf(f, "%s", praefixum);
    si (e->intra)
    {
        _camelus_scribere(f, e->intra);
    }
    _camelus_scribere(f, e->titulus);
}

interior vacuum
_functio_scribere(
    FILE*                f,
    constans character*  praefixum_min,
    CqElementum*         e,
    constans character*  suffixum)
{
    fprintf(f, "%s_", praefixum_min);
    si (e->intra)
    {
        _campus_scribere(f, e->intra);
        fprintf(f, "_");
    }
    _campus_scribere(f, e->titulus);
    fprintf(f, "%s", suffixum);
}

interior vacuum
_maiusculum_scribere(
    FILE*             f,
    constans chorda*  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t->mensura; i++)
    {
        character c;

        c = (character)t->datum[i];
        si (c == '-')
        {
            c = '_';
        }
        si (c >= 'a' && c <= 'z')
        {
            c = (character)(c - 'a' + 'A');
        }
        putc(c, f);
    }
}

/* membrum enumerationis: PRAEF_ELEM_ATTR_OPTIO (optio NIHIL =
 * caput sine optione) */
interior vacuum
_membrum_electionis_scribere(
    FILE*                f,
    constans character*  praefixum,
    CqElementum*         e,
    CqAttributum*        a,
    constans chorda*     optio)
{
    i32 i;

    per (i = ZEPHYRUM; praefixum[i]; i++)
    {
        character c;

        c = praefixum[i];
        si (c >= 'a' && c <= 'z')
        {
            c = (character)(c - 'a' + 'A');
        }
        putc(c, f);
    }
    putc('_', f);
    si (e->intra)
    {
        _maiusculum_scribere(f, e->intra);
        putc('_', f);
    }
    _maiusculum_scribere(f, e->titulus);
    putc('_', f);
    _maiusculum_scribere(f, a->titulus);
    si (optio)
    {
        putc('_', f);
        _maiusculum_scribere(f, optio);
    }
}

/* ==================================================
 * Lectio canonis in exemplar
 * ================================================== */

interior CanonGenusValoris
_genus_ex(
    chorda* s)
{
    si (!s)
    {
        redde CANON_GENUS_TEXTUS;
    }
    si (chorda_aequalis_literis(*s, "nomen"))
        redde CANON_GENUS_NOMEN;
    si (chorda_aequalis_literis(*s, "numerus"))
        redde CANON_GENUS_NUMERUS;
    si (chorda_aequalis_literis(*s, "veritas"))
        redde CANON_GENUS_VERITAS;
    si (chorda_aequalis_literis(*s, "dies"))
        redde CANON_GENUS_DIES;
    si (chorda_aequalis_literis(*s, "electio"))
        redde CANON_GENUS_ELECTIO;
    si (chorda_aequalis_literis(*s, "compositum"))
        redde CANON_GENUS_COMPOSITUM;
    /* signa (spec canon-referentia par. 7): valor VERBATIM manet,
     * signo incluso - chorda* ut textus, CONSULTO (degradatio
     * tacita vetita: nominatim hic, non per casum ordinarium) */
    si (chorda_aequalis_literis(*s, "identitas"))
        redde CANON_GENUS_TEXTUS;
    si (chorda_aequalis_literis(*s, "referentia"))
        redde CANON_GENUS_TEXTUS;

    redde CANON_GENUS_TEXTUS;
}

interior Xar*
_canonem_colligere(
    StmlNodus*  radix_canonis,
    Piscina*    piscina)
{
    Xar* elementa;
    i32  n;
    i32  i;

    elementa = xar_creare(piscina, (i32)magnitudo(CqElementum));
    n = stml_numerus_liberorum(radix_canonis);

    per (i = ZEPHYRUM; i < n; i++)
    {
        StmlNodus*   en;
        CqElementum* e;
        chorda*      attr;
        i32          nl;
        i32          j;

        en = stml_liberum_ad_indicem(radix_canonis, i);
        si (!en || en->genus != STML_NODUS_ELEMENTUM ||
            !chorda_aequalis_literis(*en->titulus, "elementum"))
        {
            perge;
        }

        e = (CqElementum*)xar_addere(elementa);
        e->titulus = stml_attributum_capere(en, "nomen");
        e->intra   = stml_attributum_capere(en, "intra");
        attr = stml_attributum_capere(en, "radix");
        e->radix = (b32)(attr &&
            chorda_aequalis_literis(*attr, "verum"));

        e->textus_licet = FALSUM;
        e->textus_genus = CANON_GENUS_TEXTUS;
        attr = stml_attributum_capere(en, "textus");
        si (attr)
        {
            si (chorda_aequalis_literis(*attr, "verum"))
            {
                e->textus_licet = VERUM;
            }
            alioquin
            {
                CanonGenusValoris g;

                g = _genus_ex(attr);
                si (g != CANON_GENUS_TEXTUS &&
                    g != CANON_GENUS_ELECTIO)
                {
                    e->textus_licet = VERUM;
                    e->textus_genus = g;
                }
            }
        }

        e->attributa = xar_creare(piscina,
                                  (i32)magnitudo(CqAttributum));
        e->liberi    = xar_creare(piscina,
                                  (i32)magnitudo(CqLiberum));

        nl = stml_numerus_liberorum(en);
        per (j = ZEPHYRUM; j < nl; j++)
        {
            StmlNodus* l;
            chorda*    lt;

            l = stml_liberum_ad_indicem(en, j);
            si (!l || l->genus != STML_NODUS_ELEMENTUM)
            {
                perge;
            }
            lt = stml_attributum_capere(l, "nomen");
            si (!lt)
            {
                perge;
            }

            si (chorda_aequalis_literis(*l->titulus, "attributum"))
            {
                CqAttributum* a;
                i32           no;
                i32           k;

                a = (CqAttributum*)xar_addere(e->attributa);
                a->titulus = lt;
                a->genus = _genus_ex(
                    stml_attributum_capere(l, "genus"));
                attr = stml_attributum_capere(l, "necessarium");
                a->necessarium = (b32)(attr &&
                    chorda_aequalis_literis(*attr, "verum"));
                a->praestitutum =
                    stml_attributum_capere(l, "ordinarius");
                a->optiones = xar_creare(piscina,
                                         (i32)magnitudo(chorda*));

                no = stml_numerus_liberorum(l);
                per (k = ZEPHYRUM; k < no; k++)
                {
                    StmlNodus* o;
                    chorda*    valor;
                    chorda**   locus;
                    chorda     t;

                    o = stml_liberum_ad_indicem(l, k);
                    si (!o || o->genus != STML_NODUS_ELEMENTUM ||
                        !chorda_aequalis_literis(*o->titulus,
                                                 "optio"))
                    {
                        perge;
                    }
                    t = chorda_praecidere(
                        stml_textus_normalizatus(o, piscina));
                    valor = (chorda*)piscina_allocare(piscina,
                        magnitudo(chorda));
                    *valor = t;
                    locus = (chorda**)xar_addere(a->optiones);
                    *locus = valor;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                                                 "liberum"))
            {
                CqLiberum* lb;
                chorda*    m;

                lb = (CqLiberum*)xar_addere(e->liberi);
                lb->titulus = lt;
                lb->unicum  = FALSUM;
                m = stml_attributum_capere(l, "maximum");
                si (m && chorda_aequalis_literis(*m, "1"))
                {
                    lb->unicum = VERUM;
                }
            }
        }
    }

    redde elementa;
}

/* definitionem liberi solvere: adstricta (intra==parens) vincit,
 * globalis cadit - speculum elementum_quaerere canonis */
interior CqElementum*
_def_invenire(
    Xar*              elementa,
    constans chorda*  parens,
    constans chorda*  titulus)
{
    i32          i;
    CqElementum* globale;

    globale = NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(elementa); i++)
    {
        CqElementum* e;

        e = (CqElementum*)xar_obtinere(elementa, i);
        si (!e->titulus || !chorda_aequalis(*e->titulus, *titulus))
        {
            perge;
        }
        si (e->intra && parens &&
            chorda_aequalis(*e->intra, *parens))
        {
            redde e;
        }
        si (!e->intra)
        {
            globale = e;
        }
    }

    redde globale;
}

/* ==================================================
 * Emissio capitis
 * ================================================== */

interior vacuum
_caput_emittere(
    FILE*                f,
    Xar*                 elementa,
    constans character*  praefixum,
    constans character*  praefixum_min,
    constans character*  custos,
    constans character*  via_canonis,
    constans character*  iussum)
{
    i32 i;
    i32 j;

    fprintf(f,
        "/* GENERATUM a canon_coquere e %s - NOLI MANU MUTARE.\n"
        " * (signum generata-custodis: editio manualis vetatur)\n"
        " * Lex folii: involve, numquam emenda - regeneratio\n"
        " * emendationes vincit. Lectio sola; plagulam canone\n"
        " * sanam praesumit (ignota tacite praeterit).\n"
        " * Regeneratio: %s */\n"
        "#ifndef %s\n"
        "#define %s\n\n"
        "#include \"latina.h\"\n"
        "#include \"chorda.h\"\n"
        "#include \"piscina.h\"\n"
        "#include \"xar.h\"\n"
        "#include \"internamentum.h\"\n"
        "#include \"stml.h\"\n\n",
        via_canonis, iussum, custos, custos);

    /* praenuntiationes */
    per (i = ZEPHYRUM; i < xar_numerus(elementa); i++)
    {
        CqElementum* e;

        e = (CqElementum*)xar_obtinere(elementa, i);
        fprintf(f, "nomen structura ");
        _typus_scribere(f, praefixum, e);
        fprintf(f, " ");
        _typus_scribere(f, praefixum, e);
        fprintf(f, ";\n");
    }
    fprintf(f, "\n");

    /* enumerationes electionum */
    per (i = ZEPHYRUM; i < xar_numerus(elementa); i++)
    {
        CqElementum* e;

        e = (CqElementum*)xar_obtinere(elementa, i);
        per (j = ZEPHYRUM; j < xar_numerus(e->attributa); j++)
        {
            CqAttributum* a;
            i32           k;

            a = (CqAttributum*)xar_obtinere(e->attributa, j);
            si (a->genus != CANON_GENUS_ELECTIO)
            {
                perge;
            }

            fprintf(f, "nomen enumeratio {\n    ");
            _membrum_electionis_scribere(f, praefixum, e, a, NIHIL);
            fprintf(f, "_ABSENS = ZEPHYRUM");
            per (k = ZEPHYRUM; k < xar_numerus(a->optiones); k++)
            {
                chorda** o;

                o = (chorda**)xar_obtinere(a->optiones, k);
                fprintf(f, ",\n    ");
                _membrum_electionis_scribere(f, praefixum, e, a,
                                             *o);
            }
            fprintf(f, "\n} ");
            _typus_scribere(f, praefixum, e);
            _camelus_scribere(f, a->titulus);
            fprintf(f, ";\n\n");
        }
    }

    /* structurae */
    per (i = ZEPHYRUM; i < xar_numerus(elementa); i++)
    {
        CqElementum* e;

        e = (CqElementum*)xar_obtinere(elementa, i);
        fprintf(f, "structura ");
        _typus_scribere(f, praefixum, e);
        fprintf(f, " {\n    StmlNodus*  nodus;\n");

        per (j = ZEPHYRUM; j < xar_numerus(e->attributa); j++)
        {
            CqAttributum* a;

            a = (CqAttributum*)xar_obtinere(e->attributa, j);
            commutatio (a->genus)
            {
                casus CANON_GENUS_NUMERUS:
                    fprintf(f, "    s64         ");
                    _campus_scribere(f, a->titulus);
                    fprintf(f, ";\n    b32         ");
                    _campus_scribere(f, a->titulus);
                    fprintf(f, "_praesens;\n");
                    frange;
                casus CANON_GENUS_VERITAS:
                    fprintf(f, "    b32         ");
                    _campus_scribere(f, a->titulus);
                    fprintf(f, ";\n    b32         ");
                    _campus_scribere(f, a->titulus);
                    fprintf(f, "_praesens;\n");
                    frange;
                casus CANON_GENUS_ELECTIO:
                    fprintf(f, "    ");
                    _typus_scribere(f, praefixum, e);
                    _camelus_scribere(f, a->titulus);
                    fprintf(f, " ");
                    _campus_scribere(f, a->titulus);
                    fprintf(f, ";\n");
                    frange;
                ordinarius:
                    /* nomen/compositum/textus/dies: chorda*
                     * (NIHIL = absens) */
                    fprintf(f, "    chorda*     ");
                    _campus_scribere(f, a->titulus);
                    fprintf(f, ";\n");
                    frange;
            }
        }

        per (j = ZEPHYRUM; j < xar_numerus(e->liberi); j++)
        {
            CqLiberum*   lb;
            CqElementum* def;

            lb = (CqLiberum*)xar_obtinere(e->liberi, j);
            def = _def_invenire(elementa, e->titulus, lb->titulus);
            si (!def)
            {
                perge;   /* liberum sine definitione: DOM solum */
            }
            si (lb->unicum)
            {
                fprintf(f, "    ");
                _typus_scribere(f, praefixum, def);
                fprintf(f, "* ");
                _campus_scribere(f, lb->titulus);
                fprintf(f, ";\n");
            }
            alioquin
            {
                fprintf(f, "    Xar*        ");
                _campus_scribere(f, lb->titulus);
                fprintf(f, ";   /* ");
                _typus_scribere(f, praefixum, def);
                fprintf(f, "* */\n");
            }
        }

        si (e->textus_licet)
        {
            fprintf(f, "    chorda      textus;\n");
            si (e->textus_genus == CANON_GENUS_NUMERUS)
            {
                fprintf(f, "    s64         textus_numerus;\n"
                           "    b32         textus_praesens;\n");
            }
            alioquin si (e->textus_genus == CANON_GENUS_VERITAS)
            {
                fprintf(f, "    b32         textus_veritas;\n"
                           "    b32         textus_praesens;\n");
            }
        }

        fprintf(f, "};\n\n");
    }

    /* prototypa */
    per (i = ZEPHYRUM; i < xar_numerus(elementa); i++)
    {
        CqElementum* e;

        e = (CqElementum*)xar_obtinere(elementa, i);
        _typus_scribere(f, praefixum, e);
        fprintf(f, "* ");
        _functio_scribere(f, praefixum_min, e, "_legere_nodum");
        fprintf(f, "(\n    StmlNodus* nodus, Piscina* piscina,\n"
                   "    InternamentumChorda* intern);\n");
        si (e->radix)
        {
            _typus_scribere(f, praefixum, e);
            fprintf(f, "* ");
            _functio_scribere(f, praefixum_min, e, "_legere");
            fprintf(f,
                "(\n    chorda fons, Piscina* piscina,\n"
                "    InternamentumChorda* intern, chorda* causa);\n");
        }
    }

    fprintf(f, "\n#endif /* %s */\n", custos);
}

/* ==================================================
 * Emissio corporis
 * ================================================== */

interior vacuum
_lectorem_emittere(
    FILE*                f,
    Xar*                 elementa,
    CqElementum*         e,
    constans character*  praefixum,
    constans character*  praefixum_min)
{
    i32 j;
    b32 liberi_definiti;

    liberi_definiti = FALSUM;
    per (j = ZEPHYRUM; j < xar_numerus(e->liberi); j++)
    {
        CqLiberum* lb;

        lb = (CqLiberum*)xar_obtinere(e->liberi, j);
        si (_def_invenire(elementa, e->titulus, lb->titulus))
        {
            liberi_definiti = VERUM;
        }
    }

    _typus_scribere(f, praefixum, e);
    fprintf(f, "*\n");
    _functio_scribere(f, praefixum_min, e, "_legere_nodum");
    fprintf(f, "(\n    StmlNodus*           nodus,\n"
               "    Piscina*             piscina,\n"
               "    InternamentumChorda* intern)\n{\n    ");
    _typus_scribere(f, praefixum, e);
    fprintf(f, "* res;\n\n"
               "    (vacuum)intern;\n"
               "    res = (");
    _typus_scribere(f, praefixum, e);
    fprintf(f, "*)piscina_allocare(piscina,\n"
               "        magnitudo(");
    _typus_scribere(f, praefixum, e);
    fprintf(f, "));\n"
               "    si (!res)\n    {\n        redde NIHIL;\n    }\n"
               "    res->nodus = nodus;\n");

    /* attributa */
    per (j = ZEPHYRUM; j < xar_numerus(e->attributa); j++)
    {
        CqAttributum* a;

        a = (CqAttributum*)xar_obtinere(e->attributa, j);
        fprintf(f, "\n");

        si (a->genus == CANON_GENUS_NUMERUS)
        {
            fprintf(f, "    res->");
            _campus_scribere(f, a->titulus);
            si (a->praestitutum)
            {
                fprintf(f, " = %.*s;",
                        (integer)a->praestitutum->mensura,
                        (constans character*)
                            a->praestitutum->datum);
            }
            alioquin
            {
                fprintf(f, " = ZEPHYRUM;");
            }
            fprintf(f, "\n    res->");
            _campus_scribere(f, a->titulus);
            fprintf(f, "_praesens = FALSUM;\n"
                "    {\n        chorda* v;\n        s32 tv;\n\n"
                "        v = stml_attributum_capere(nodus, \"%.*s\");\n"
                "        si (v && chorda_ut_s32(*v, &tv))\n"
                "        {\n            res->",
                (integer)a->titulus->mensura,
                (constans character*)a->titulus->datum);
            _campus_scribere(f, a->titulus);
            fprintf(f, " = (s64)tv;\n            res->");
            _campus_scribere(f, a->titulus);
            fprintf(f, "_praesens = VERUM;\n        }\n    }\n");
        }
        alioquin si (a->genus == CANON_GENUS_VERITAS)
        {
            fprintf(f, "    res->");
            _campus_scribere(f, a->titulus);
            fprintf(f, " = %s;\n    res->",
                (a->praestitutum &&
                 chorda_aequalis_literis(*a->praestitutum,
                                         "verum"))
                    ? "VERUM" : "FALSUM");
            _campus_scribere(f, a->titulus);
            fprintf(f, "_praesens = FALSUM;\n"
                "    {\n        chorda* v;\n\n"
                "        v = stml_attributum_capere(nodus, \"%.*s\");\n"
                "        si (v)\n        {\n            res->",
                (integer)a->titulus->mensura,
                (constans character*)a->titulus->datum);
            _campus_scribere(f, a->titulus);
            fprintf(f,
                " = (b32)chorda_aequalis_literis(*v, \"verum\");\n"
                "            res->");
            _campus_scribere(f, a->titulus);
            fprintf(f, "_praesens = VERUM;\n        }\n    }\n");
        }
        alioquin si (a->genus == CANON_GENUS_ELECTIO)
        {
            i32 k;

            fprintf(f, "    res->");
            _campus_scribere(f, a->titulus);
            fprintf(f, " = ");
            si (a->praestitutum)
            {
                _membrum_electionis_scribere(f, praefixum, e, a,
                                             a->praestitutum);
            }
            alioquin
            {
                _membrum_electionis_scribere(f, praefixum, e, a,
                                             NIHIL);
                fprintf(f, "_ABSENS");
            }
            fprintf(f, ";\n"
                "    {\n        chorda* v;\n\n"
                "        v = stml_attributum_capere(nodus, \"%.*s\");\n"
                "        si (v)\n        {\n",
                (integer)a->titulus->mensura,
                (constans character*)a->titulus->datum);
            per (k = ZEPHYRUM; k < xar_numerus(a->optiones); k++)
            {
                chorda** o;

                o = (chorda**)xar_obtinere(a->optiones, k);
                fprintf(f, "            %ssi "
                    "(chorda_aequalis_literis(*v, \"%.*s\"))\n"
                    "            {\n                res->",
                    k == ZEPHYRUM ? "" : "alioquin ",
                    (integer)(*o)->mensura,
                    (constans character*)(*o)->datum);
                _campus_scribere(f, a->titulus);
                fprintf(f, " = ");
                _membrum_electionis_scribere(f, praefixum, e, a,
                                             *o);
                fprintf(f, ";\n            }\n");
            }
            fprintf(f, "        }\n    }\n");
        }
        alioquin
        {
            /* chorda*: nomen/compositum/textus/dies */
            fprintf(f, "    res->");
            _campus_scribere(f, a->titulus);
            fprintf(f,
                " = stml_attributum_capere(nodus, \"%.*s\");\n",
                (integer)a->titulus->mensura,
                (constans character*)a->titulus->datum);
            si (a->praestitutum)
            {
                fprintf(f, "    si (!res->");
                _campus_scribere(f, a->titulus);
                fprintf(f, ")\n    {\n        res->");
                _campus_scribere(f, a->titulus);
                fprintf(f,
                    " = chorda_internare_ex_literis(intern,\n"
                    "            \"%.*s\");\n    }\n",
                    (integer)a->praestitutum->mensura,
                    (constans character*)a->praestitutum->datum);
            }
        }
    }

    /* liberi: initia */
    per (j = ZEPHYRUM; j < xar_numerus(e->liberi); j++)
    {
        CqLiberum*   lb;
        CqElementum* def;

        lb = (CqLiberum*)xar_obtinere(e->liberi, j);
        def = _def_invenire(elementa, e->titulus, lb->titulus);
        si (!def)
        {
            perge;
        }
        si (lb->unicum)
        {
            fprintf(f, "    res->");
            _campus_scribere(f, lb->titulus);
            fprintf(f, " = NIHIL;\n");
        }
        alioquin
        {
            fprintf(f, "    res->");
            _campus_scribere(f, lb->titulus);
            fprintf(f, " = xar_creare(piscina, (i32)magnitudo(");
            _typus_scribere(f, praefixum, def);
            fprintf(f, "*));\n");
        }
    }

    /* liberi: percursus */
    si (liberi_definiti)
    {
        fprintf(f,
            "    {\n        i32 n;\n        i32 i;\n\n"
            "        n = stml_numerus_liberorum(nodus);\n"
            "        per (i = ZEPHYRUM; i < n; i++)\n"
            "        {\n            StmlNodus* l;\n\n"
            "            l = stml_liberum_ad_indicem(nodus, i);\n"
            "            si (!l || l->genus != "
            "STML_NODUS_ELEMENTUM ||\n"
            "                !l->titulus)\n"
            "            {\n                perge;\n            }\n");

        {
            b32 primus;

            primus = VERUM;
            per (j = ZEPHYRUM; j < xar_numerus(e->liberi); j++)
            {
                CqLiberum*   lb;
                CqElementum* def;

                lb = (CqLiberum*)xar_obtinere(e->liberi, j);
                def = _def_invenire(elementa, e->titulus,
                                    lb->titulus);
                si (!def)
                {
                    perge;
                }

                fprintf(f, "            %ssi "
                    "(chorda_aequalis_literis(*l->titulus,\n"
                    "                    \"%.*s\"))\n"
                    "            {\n",
                    primus ? "" : "alioquin ",
                    (integer)lb->titulus->mensura,
                    (constans character*)lb->titulus->datum);
                primus = FALSUM;

                si (lb->unicum)
                {
                    fprintf(f, "                si (!res->");
                    _campus_scribere(f, lb->titulus);
                    fprintf(f, ")\n                {\n"
                               "                    res->");
                    _campus_scribere(f, lb->titulus);
                    fprintf(f, " =\n                        ");
                    _functio_scribere(f, praefixum_min, def,
                                      "_legere_nodum");
                    fprintf(f, "(\n                            "
                               "l, piscina, intern);\n"
                               "                }\n");
                }
                alioquin
                {
                    fprintf(f, "                ");
                    _typus_scribere(f, praefixum, def);
                    fprintf(f, "* filius;\n                ");
                    _typus_scribere(f, praefixum, def);
                    fprintf(f, "** locus;\n\n"
                               "                filius = ");
                    _functio_scribere(f, praefixum_min, def,
                                      "_legere_nodum");
                    fprintf(f, "(l, piscina, intern);\n"
                        "                si (filius)\n"
                        "                {\n"
                        "                    locus = (");
                    _typus_scribere(f, praefixum, def);
                    fprintf(f, "**)xar_addere(res->");
                    _campus_scribere(f, lb->titulus);
                    fprintf(f, ");\n"
                        "                    *locus = filius;\n"
                        "                }\n");
                }
                fprintf(f, "            }\n");
            }
        }

        fprintf(f, "        }\n    }\n");
    }

    /* textus */
    si (e->textus_licet)
    {
        fprintf(f, "    res->textus = chorda_praecidere(\n"
            "        stml_textus_normalizatus(nodus, piscina));\n");
        si (e->textus_genus == CANON_GENUS_NUMERUS)
        {
            fprintf(f,
                "    res->textus_numerus = ZEPHYRUM;\n"
                "    res->textus_praesens = FALSUM;\n"
                "    {\n        s32 tv;\n\n"
                "        si (res->textus.mensura > ZEPHYRUM &&\n"
                "            chorda_ut_s32(res->textus, &tv))\n"
                "        {\n"
                "            res->textus_numerus = (s64)tv;\n"
                "            res->textus_praesens = VERUM;\n"
                "        }\n    }\n");
        }
        alioquin si (e->textus_genus == CANON_GENUS_VERITAS)
        {
            fprintf(f,
                "    res->textus_veritas = (b32)\n"
                "        chorda_aequalis_literis(res->textus, "
                "\"verum\");\n"
                "    res->textus_praesens =\n"
                "        (b32)(res->textus.mensura > ZEPHYRUM);\n");
        }
    }

    fprintf(f, "\n    redde res;\n}\n\n");

    /* radix: parsatio + lectio */
    si (e->radix)
    {
        _typus_scribere(f, praefixum, e);
        fprintf(f, "*\n");
        _functio_scribere(f, praefixum_min, e, "_legere");
        fprintf(f, "(\n    chorda               fons,\n"
            "    Piscina*             piscina,\n"
            "    InternamentumChorda* intern,\n"
            "    chorda*              causa)\n{\n"
            "    StmlResultus r;\n\n"
            "    si (causa)\n    {\n"
            "        causa->datum   = NIHIL;\n"
            "        causa->mensura = ZEPHYRUM;\n    }\n\n"
            "    r = stml_legere(fons, piscina, intern);\n"
            "    si (!r.successus || !r.elementum_radix)\n"
            "    {\n        si (causa)\n        {\n"
            "            *causa = chorda_ex_literis(\n"
            "                \"stml parsari nequit\", piscina);\n"
            "        }\n        redde NIHIL;\n    }\n"
            "    si (!r.elementum_radix->titulus ||\n"
            "        !chorda_aequalis_literis("
            "*r.elementum_radix->titulus,\n"
            "                                 \"%.*s\"))\n"
            "    {\n        si (causa)\n        {\n"
            "            *causa = chorda_ex_literis(\n"
            "                \"radix non est <%.*s>\", piscina);\n"
            "        }\n        redde NIHIL;\n    }\n\n"
            "    redde ",
            (integer)e->titulus->mensura,
            (constans character*)e->titulus->datum,
            (integer)e->titulus->mensura,
            (constans character*)e->titulus->datum);
        _functio_scribere(f, praefixum_min, e, "_legere_nodum");
        fprintf(f, "(r.elementum_radix, piscina,\n"
                   "        intern);\n}\n\n");
    }
}

interior vacuum
_corpus_emittere(
    FILE*                f,
    Xar*                 elementa,
    constans character*  praefixum,
    constans character*  praefixum_min,
    constans character*  caput_basis,
    constans character*  via_canonis)
{
    i32 i;

    fprintf(f,
        "/* GENERATUM a canon_coquere e %s - NOLI MANU MUTARE.\n"
        " * Vide caput pro legibus folii. */\n"
        "#include \"%s\"\n\n",
        via_canonis, caput_basis);

    per (i = ZEPHYRUM; i < xar_numerus(elementa); i++)
    {
        CqElementum* e;

        e = (CqElementum*)xar_obtinere(elementa, i);
        _lectorem_emittere(f, elementa, e, praefixum,
                           praefixum_min);
    }
}

/* ==================================================
 * Principale
 * ================================================== */

s32
principale(
    s32          numerus,
    character**  argumenta)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    constans character*  via_canonis;
    constans character*  praefixum;
    constans character*  via_capitis;
    constans character*  via_corporis;
    character            praefixum_min[XXXII];
    character            custos[CXXVIII];
    character            iussum[DXII];
    chorda               fons;
    StmlResultus         r;
    Xar*                 elementa;
    FILE*                f;
    s32                  i;

    via_canonis  = NIHIL;
    praefixum    = NIHIL;
    via_capitis  = NIHIL;
    via_corporis = NIHIL;

    per (i = I; i < numerus; i++)
    {
        si (strcmp(argumenta[i], "-praefixum") == ZEPHYRUM &&
            i + I < numerus)
        {
            praefixum = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-caput") == ZEPHYRUM &&
                     i + I < numerus)
        {
            via_capitis = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-corpus") == ZEPHYRUM &&
                     i + I < numerus)
        {
            via_corporis = argumenta[++i];
        }
        alioquin
        {
            via_canonis = argumenta[i];
        }
    }

    si (!via_canonis || !praefixum || !via_capitis || !via_corporis)
    {
        fprintf(stderr,
            "usus: canon_coquere <via.canon> -praefixum <Pascal> "
            "-caput <x.h> -corpus <x.c>\n");
        redde II;
    }

    piscina = piscina_generare_dynamicum("canon_coquere", 1048576);
    intern  = internamentum_creare(piscina);

    fons = filum_legere_totum(via_canonis, piscina);
    si (fons.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "canon_coquere: '%s' legi nequit\n",
                via_canonis);
        redde II;
    }
    r = stml_legere(fons, piscina, intern);
    si (!r.successus || !r.elementum_radix ||
        !chorda_aequalis_literis(*r.elementum_radix->titulus,
                                 "canon"))
    {
        fprintf(stderr,
            "canon_coquere: '%s' non est canon parsabilis\n",
            via_canonis);
        redde II;
    }

    elementa = _canonem_colligere(r.elementum_radix, piscina);
    si (xar_numerus(elementa) == ZEPHYRUM)
    {
        fprintf(stderr,
            "canon_coquere: canon sine elementis - NIHIL coctum\n");
        redde II;
    }

    /* praefixum minusculum + custos includendi */
    {
        i32 j;

        per (j = ZEPHYRUM; praefixum[j] &&
             j < (i32)magnitudo(praefixum_min) - I; j++)
        {
            character c;

            c = praefixum[j];
            si (c >= 'A' && c <= 'Z')
            {
                c = (character)(c - 'A' + 'a');
            }
            praefixum_min[j] = c;
        }
        praefixum_min[j] = '\0';
    }
    {
        constans character* basis;
        i32                 j;
        i32                 k;

        basis = strrchr(via_capitis, '/');
        basis = basis ? basis + I : via_capitis;
        k = ZEPHYRUM;
        per (j = ZEPHYRUM; basis[j] &&
             k < (i32)magnitudo(custos) - I; j++)
        {
            character c;

            c = basis[j];
            si (c >= 'a' && c <= 'z')
            {
                c = (character)(c - 'a' + 'A');
            }
            alioquin si (c == '.' || c == '-')
            {
                c = '_';
            }
            custos[k++] = c;
        }
        custos[k] = '\0';
    }
    sprintf(iussum,
        "bin/canon_coquere %s -praefixum %s -caput %s -corpus %s",
        via_canonis, praefixum, via_capitis, via_corporis);

    f = fopen(via_capitis, "w");
    si (!f)
    {
        fprintf(stderr, "canon_coquere: '%s' scribi nequit\n",
                via_capitis);
        redde II;
    }
    _caput_emittere(f, elementa, praefixum, praefixum_min,
                    custos, via_canonis, iussum);
    fclose(f);

    f = fopen(via_corporis, "w");
    si (!f)
    {
        fprintf(stderr, "canon_coquere: '%s' scribi nequit\n",
                via_corporis);
        redde II;
    }
    {
        constans character* basis;

        basis = strrchr(via_capitis, '/');
        basis = basis ? basis + I : via_capitis;
        _corpus_emittere(f, elementa, praefixum, praefixum_min,
                         basis, via_canonis);
    }
    fclose(f);

    fprintf(stderr, "canon_coquere: %s + %s cocta (%u elementa)\n",
            via_capitis, via_corporis, xar_numerus(elementa));

    redde ZEPHYRUM;
}
