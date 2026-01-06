/* arbor_index.c - Project-Wide Symbol Index */

#include "arbor_index.h"
#include "arbor_lexema.h"
#include "arbor_syntaxis.h"
#include "arbor_typus.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Internal Structure
 * ================================================== */

structura ArborProiectum {
    Piscina*              piscina;
    InternamentumChorda*  intern;

    /* Fila */
    Xar*                  fila;              /* Xar de ArborFilum* */
    TabulaDispersa*       fila_per_viam;     /* via -> ArborFilum* */

    /* Viae include */
    Xar*                  viae_include;      /* Xar de chorda */

    /* Tabulae symbolorum */
    TabulaDispersa*       symbola_externa;   /* titulus -> ArborIndexSymbolum* */
    TabulaDispersa*       symbola_per_filum; /* via -> TabulaDispersa* (file-local) */

    /* Type tags (struct/union/enum) */
    TabulaDispersa*       tag_externa;       /* tag titulus -> ArborIndexSymbolum* */
};

/* ==================================================
 * Helper Functions - Forward Declarations
 * ================================================== */

interior vacuum _extrahere_symbola(ArborProiectum* proj, ArborFilum* filum,
                                   ArborNodus* nodus, b32 in_scopo_fili);
interior vacuum _colligere_referentias(ArborProiectum* proj, ArborFilum* filum,
                                       ArborNodus* nodus);
interior ArborNexus _determinare_nexum(ArborNodus* declaratio);
interior chorda* _obtinere_nomen_declaratoris(ArborNodus* declarator);
interior ArborNodus* _invenire_nodum_ad_punctum(ArborNodus* radix,
                                                i32 linea, i32 columna);
interior ArborFilum* _invenire_filum(ArborProiectum* proj, constans character* via);

/* ==================================================
 * API Implementation
 * ================================================== */

ArborProiectum*
arbor_index_creare(Piscina* piscina, InternamentumChorda* intern)
{
    ArborProiectum* proj;

    proj = piscina_allocare(piscina, magnitudo(ArborProiectum));
    si (proj == NIHIL) redde NIHIL;

    proj->piscina = piscina;
    proj->intern = intern;
    proj->fila = xar_creare(piscina, magnitudo(ArborFilum*));
    proj->fila_per_viam = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    proj->viae_include = xar_creare(piscina, magnitudo(chorda));
    proj->symbola_externa = tabula_dispersa_creare_chorda(piscina, CCLVI);
    proj->symbola_per_filum = tabula_dispersa_creare_chorda(piscina, LXIV);
    proj->tag_externa = tabula_dispersa_creare_chorda(piscina, CXXVIII);

    redde proj;
}

vacuum
arbor_index_addere_filum(ArborProiectum* proj, constans character* via)
{
    ArborFilum* filum;
    ArborFilum** slot;
    chorda fons;
    chorda via_chorda;
    ArborSyntaxis* syn;
    ArborSyntaxisResultus res;
    i32 via_len;
    TabulaDispersa* tabula_fili;

    si (proj == NIHIL || via == NIHIL) redde;

    /* Legere filum */
    fons = filum_legere_totum(via, proj->piscina);
    si (fons.mensura == ZEPHYRUM && !filum_existit(via))
    {
        imprimere("arbor_index: non potest legere filum: %s\n", via);
        redde;
    }

    /* Creare ArborFilum */
    filum = piscina_allocare(proj->piscina, magnitudo(ArborFilum));
    si (filum == NIHIL) redde;

    via_len = (i32)strlen(via);
    via_chorda.datum = piscina_allocare(proj->piscina, (memoriae_index)via_len);
    memcpy(via_chorda.datum, via, (size_t)via_len);
    via_chorda.mensura = via_len;

    filum->via = via_chorda;
    filum->fons = fons;
    filum->tempus_mutatum = ZEPHYRUM;
    filum->radix = NIHIL;

    /* Parsere fontem */
    syn = arbor_syntaxis_creare(proj->piscina, proj->intern);
    si (syn != NIHIL)
    {
        res = arbor_syntaxis_parsere_fontem(syn, (constans character*)fons.datum,
                                            (i32)fons.mensura, via);
        si (res.successus)
        {
            filum->radix = res.radix;
        }
        alioquin
        {
            imprimere("arbor_index: error parsendi filum: %s\n", via);
        }
    }

    /* Addere ad fila */
    slot = xar_addere(proj->fila);
    si (slot != NIHIL)
    {
        *slot = filum;
    }

    /* Addere ad tabulam per viam */
    tabula_dispersa_inserere(proj->fila_per_viam, via_chorda, filum);

    /* Creare tabulam symbolorum pro hoc filo */
    tabula_fili = tabula_dispersa_creare_chorda(proj->piscina, LXIV);
    tabula_dispersa_inserere(proj->symbola_per_filum, via_chorda, tabula_fili);
}

vacuum
arbor_index_addere_folder(ArborProiectum* proj, constans character* via,
                          constans character* pattern)
{
    /* TODO: Implementare cum glob pattern matching */
    /* Pro nunc, non implementatum */
    (vacuum)proj;
    (vacuum)via;
    (vacuum)pattern;
}

vacuum
arbor_index_addere_via_include(ArborProiectum* proj, constans character* via)
{
    chorda via_chorda;
    chorda* slot;
    i32 via_len;

    si (proj == NIHIL || via == NIHIL) redde;

    via_len = (i32)strlen(via);
    via_chorda.datum = piscina_allocare(proj->piscina, (memoriae_index)via_len);
    memcpy(via_chorda.datum, via, (size_t)via_len);
    via_chorda.mensura = via_len;

    slot = xar_addere(proj->viae_include);
    si (slot != NIHIL)
    {
        *slot = via_chorda;
    }
}

vacuum
arbor_index_aedificare(ArborProiectum* proj)
{
    i32 i;
    i32 numerus;

    si (proj == NIHIL) redde;

    numerus = xar_numerus(proj->fila);

    /* Primus passus: extrahere symbola */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        ArborFilum** f_ptr;
        ArborFilum* f;

        f_ptr = xar_obtinere(proj->fila, i);
        si (f_ptr == NIHIL) perge;
        f = *f_ptr;

        si (f->radix != NIHIL)
        {
            _extrahere_symbola(proj, f, f->radix, VERUM);
        }
    }

    /* Secundus passus: colligere referentias */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        ArborFilum** f_ptr;
        ArborFilum* f;

        f_ptr = xar_obtinere(proj->fila, i);
        si (f_ptr == NIHIL) perge;
        f = *f_ptr;

        si (f->radix != NIHIL)
        {
            _colligere_referentias(proj, f, f->radix);
        }
    }
}

ArborIndexSymbolum*
arbor_index_invenire(ArborProiectum* proj, chorda* titulus)
{
    vacuum* res;
    b32 inventum;

    si (proj == NIHIL || titulus == NIHIL) redde NIHIL;

    /* Quaerere in symbolis externis */
    inventum = tabula_dispersa_invenire(proj->symbola_externa, *titulus, &res);
    si (inventum)
    {
        redde (ArborIndexSymbolum*)res;
    }

    redde NIHIL;
}

Xar*
arbor_index_referentiae(ArborProiectum* proj, chorda* titulus)
{
    ArborIndexSymbolum* sym;

    si (proj == NIHIL || titulus == NIHIL) redde NIHIL;

    sym = arbor_index_invenire(proj, titulus);
    si (sym == NIHIL) redde NIHIL;

    redde sym->referentiae;
}

ArborNodus*
arbor_index_definitio_ad_punctum(ArborProiectum* proj, constans character* via,
                                  i32 linea, i32 columna)
{
    ArborIndexSymbolum* sym;

    sym = arbor_index_symbolum_ad_punctum(proj, via, linea, columna);
    si (sym == NIHIL) redde NIHIL;

    /* Reddere definitionem si disponibilis, alioquin declarationem */
    si (sym->definitio != NIHIL)
    {
        redde sym->definitio;
    }
    redde sym->declaratio;
}

ArborIndexSymbolum*
arbor_index_symbolum_ad_punctum(ArborProiectum* proj, constans character* via,
                                 i32 linea, i32 columna)
{
    ArborFilum* filum;
    ArborNodus* nodus;

    si (proj == NIHIL || via == NIHIL) redde NIHIL;

    /* Invenire filum */
    filum = _invenire_filum(proj, via);
    si (filum == NIHIL) redde NIHIL;

    /* Invenire nodum ad punctum */
    nodus = _invenire_nodum_ad_punctum(filum->radix, linea, columna);
    si (nodus == NIHIL) redde NIHIL;

    /* Si identificator, quaerere symbolum */
    si (nodus->genus == ARBOR_NODUS_IDENTIFIER)
    {
        redde arbor_index_invenire(proj, nodus->datum.folium.valor);
    }

    redde NIHIL;
}

constans character*
arbor_symbolum_genus_nomen(ArborSymbolumGenus genus)
{
    commutatio (genus)
    {
        casus ARBOR_SYMBOLUM_FUNCTIO:    redde "functio";
        casus ARBOR_SYMBOLUM_VARIABILIS: redde "variabilis";
        casus ARBOR_SYMBOLUM_TYPUS:      redde "typus";
        casus ARBOR_SYMBOLUM_MACRO:      redde "macro";
        casus ARBOR_SYMBOLUM_ENUM_VALOR: redde "enum_valor";
        ordinarius:                      redde "ignotum";
    }
}

constans character*
arbor_nexus_nomen(ArborNexus nexus)
{
    commutatio (nexus)
    {
        casus ARBOR_NEXUS_EXTERNUS:  redde "externus";
        casus ARBOR_NEXUS_INTERNUS:  redde "internus";
        casus ARBOR_NEXUS_NULLUS:    redde "nullus";
        ordinarius:                  redde "ignotum";
    }
}

/* ==================================================
 * Helper Functions - Implementation
 * ================================================== */

interior ArborFilum*
_invenire_filum(ArborProiectum* proj, constans character* via)
{
    chorda via_chorda;
    vacuum* res;
    b32 inventum;
    i32 via_len;

    /* Creare chorda temporanea pro lookup - allocare et copiare */
    via_len = (i32)strlen(via);
    via_chorda.datum = piscina_allocare(proj->piscina, (memoriae_index)via_len);
    memcpy(via_chorda.datum, via, (size_t)via_len);
    via_chorda.mensura = via_len;

    inventum = tabula_dispersa_invenire(proj->fila_per_viam, via_chorda, &res);
    si (inventum)
    {
        redde (ArborFilum*)res;
    }

    redde NIHIL;
}

interior ArborNexus
_determinare_nexum(ArborNodus* declaratio)
{
    Xar* specifiers;
    i32 i;

    si (declaratio == NIHIL) redde ARBOR_NEXUS_NULLUS;

    /* Pro function definition */
    si (declaratio->genus == ARBOR_NODUS_FUNCTION_DEFINITION)
    {
        specifiers = declaratio->datum.functio.specifiers;
        si (specifiers != NIHIL)
        {
            per (i = ZEPHYRUM; i < xar_numerus(specifiers); i++)
            {
                ArborNodus** spec_ptr;
                ArborNodus* spec;

                spec_ptr = xar_obtinere(specifiers, i);
                si (spec_ptr == NIHIL) perge;
                spec = *spec_ptr;

                si (spec->genus == ARBOR_NODUS_STORAGE_CLASS &&
                    spec->datum.folium.valor != NIHIL)
                {
                    si (spec->datum.folium.valor->mensura == VI &&
                        memcmp(spec->datum.folium.valor->datum, "static", VI) == ZEPHYRUM)
                    {
                        redde ARBOR_NEXUS_INTERNUS;
                    }
                }
            }
        }
        /* Function definitions default to external linkage */
        redde ARBOR_NEXUS_EXTERNUS;
    }

    /* Pro declaration */
    si (declaratio->genus == ARBOR_NODUS_DECLARATION)
    {
        specifiers = declaratio->datum.declaratio.specifiers;
        si (specifiers != NIHIL)
        {
            per (i = ZEPHYRUM; i < xar_numerus(specifiers); i++)
            {
                ArborNodus** spec_ptr;
                ArborNodus* spec;

                spec_ptr = xar_obtinere(specifiers, i);
                si (spec_ptr == NIHIL) perge;
                spec = *spec_ptr;

                si (spec->genus == ARBOR_NODUS_STORAGE_CLASS)
                {
                    /* Check for static or extern */
                    si (spec->datum.folium.valor != NIHIL)
                    {
                        si (spec->datum.folium.valor->mensura == VI &&
                            memcmp(spec->datum.folium.valor->datum, "static", VI) == ZEPHYRUM)
                        {
                            redde ARBOR_NEXUS_INTERNUS;
                        }
                        si (spec->datum.folium.valor->mensura == VI &&
                            memcmp(spec->datum.folium.valor->datum, "extern", VI) == ZEPHYRUM)
                        {
                            redde ARBOR_NEXUS_EXTERNUS;
                        }
                    }
                }
            }
        }

        /* File scope declarations default to external */
        redde ARBOR_NEXUS_EXTERNUS;
    }

    redde ARBOR_NEXUS_NULLUS;
}

interior b32
_est_declarator_functionis(ArborNodus* declarator)
{
    Xar* liberi;
    i32 i;

    si (declarator == NIHIL) redde FALSUM;

    si (declarator->genus == ARBOR_NODUS_FUNCTION_DECLARATOR)
    {
        redde VERUM;
    }

    si (declarator->genus == ARBOR_NODUS_INIT_DECLARATOR)
    {
        redde _est_declarator_functionis(declarator->datum.init_decl.declarator);
    }

    si (declarator->genus == ARBOR_NODUS_POINTER)
    {
        /* Pointer to function - still check inner */
        redde _est_declarator_functionis(declarator->datum.pointer.inner);
    }

    /* Check children of DECLARATOR node */
    si (declarator->genus == ARBOR_NODUS_DECLARATOR)
    {
        liberi = declarator->datum.genericum.liberi;
        si (liberi != NIHIL)
        {
            per (i = ZEPHYRUM; i < xar_numerus(liberi); i++)
            {
                ArborNodus** child_ptr = xar_obtinere(liberi, i);
                si (child_ptr != NIHIL)
                {
                    si (_est_declarator_functionis(*child_ptr))
                    {
                        redde VERUM;
                    }
                }
            }
        }
    }

    redde FALSUM;
}

interior chorda*
_obtinere_nomen_declaratoris(ArborNodus* declarator)
{
    Xar* liberi;
    i32 i;

    si (declarator == NIHIL) redde NIHIL;

    /* Identifier direct */
    si (declarator->genus == ARBOR_NODUS_IDENTIFIER)
    {
        redde declarator->datum.folium.valor;
    }

    /* Init declarator - habet structuram specificam */
    si (declarator->genus == ARBOR_NODUS_INIT_DECLARATOR)
    {
        redde _obtinere_nomen_declaratoris(declarator->datum.init_decl.declarator);
    }

    /* Pointer declarator */
    si (declarator->genus == ARBOR_NODUS_POINTER)
    {
        redde _obtinere_nomen_declaratoris(declarator->datum.pointer.inner);
    }

    /* Array declarator */
    si (declarator->genus == ARBOR_NODUS_ARRAY_DECLARATOR)
    {
        redde _obtinere_nomen_declaratoris(declarator->datum.array.inner);
    }

    /* Search in children for identifier (DECLARATOR, FUNCTION_DECLARATOR) */
    si (declarator->genus == ARBOR_NODUS_DECLARATOR ||
        declarator->genus == ARBOR_NODUS_FUNCTION_DECLARATOR)
    {
        liberi = declarator->datum.genericum.liberi;
        si (liberi != NIHIL)
        {
            per (i = ZEPHYRUM; i < xar_numerus(liberi); i++)
            {
                ArborNodus** child_ptr;
                ArborNodus* child;
                chorda* result;

                child_ptr = xar_obtinere(liberi, i);
                si (child_ptr == NIHIL) perge;
                child = *child_ptr;

                result = _obtinere_nomen_declaratoris(child);
                si (result != NIHIL)
                {
                    redde result;
                }
            }
        }
    }

    redde NIHIL;
}

interior vacuum
_registrare_symbolum(ArborProiectum* proj, ArborFilum* filum,
                     chorda* titulus, ArborSymbolumGenus genus,
                     ArborNexus nexus, ArborNodus* declaratio,
                     ArborNodus* definitio)
{
    ArborIndexSymbolum* sym;
    ArborIndexSymbolum* existens;
    vacuum* res;
    b32 inventum;

    si (titulus == NIHIL) redde;

    /* Check if symbol already exists */
    inventum = tabula_dispersa_invenire(proj->symbola_externa, *titulus, &res);
    si (inventum)
    {
        existens = (ArborIndexSymbolum*)res;
        /* Update definition if we have one and existing doesn't */
        si (definitio != NIHIL && existens->definitio == NIHIL)
        {
            existens->definitio = definitio;
        }
        redde;
    }

    /* Creare novum symbolum */
    sym = piscina_allocare(proj->piscina, magnitudo(ArborIndexSymbolum));
    si (sym == NIHIL) redde;

    sym->titulus = titulus;
    sym->genus = genus;
    sym->nexus = nexus;
    sym->filum = filum;
    sym->declaratio = declaratio;
    sym->definitio = definitio;
    sym->referentiae = xar_creare(proj->piscina, magnitudo(ArborIndexReferentia));

    /* Inserere in tabulam appropriatam */
    si (nexus == ARBOR_NEXUS_EXTERNUS)
    {
        tabula_dispersa_inserere(proj->symbola_externa, *titulus, sym);
    }
    alioquin si (nexus == ARBOR_NEXUS_INTERNUS)
    {
        /* Inserere in tabulam fili */
        TabulaDispersa* tabula_fili;
        inventum = tabula_dispersa_invenire(proj->symbola_per_filum, filum->via, &res);
        si (inventum)
        {
            tabula_fili = (TabulaDispersa*)res;
            tabula_dispersa_inserere(tabula_fili, *titulus, sym);
        }
        /* Also insert in global for lookup */
        tabula_dispersa_inserere(proj->symbola_externa, *titulus, sym);
    }
}

interior vacuum
_extrahere_symbola(ArborProiectum* proj, ArborFilum* filum,
                   ArborNodus* nodus, b32 in_scopo_fili)
{
    si (nodus == NIHIL) redde;

    commutatio (nodus->genus)
    {
        casus ARBOR_NODUS_TRANSLATION_UNIT:
        {
            /* Process children at file scope */
            Xar* liberi = nodus->datum.genericum.liberi;
            si (liberi != NIHIL)
            {
                i32 i;
                per (i = ZEPHYRUM; i < xar_numerus(liberi); i++)
                {
                    ArborNodus** child_ptr = xar_obtinere(liberi, i);
                    si (child_ptr != NIHIL)
                    {
                        _extrahere_symbola(proj, filum, *child_ptr, VERUM);
                    }
                }
            }
            frange;
        }

        casus ARBOR_NODUS_FUNCTION_DEFINITION:
        {
            /* Registrare functionem */
            ArborNodus* declarator = nodus->datum.functio.declarator;
            chorda* func_titulus = _obtinere_nomen_declaratoris(declarator);
            ArborNexus nexus = _determinare_nexum(nodus);

            _registrare_symbolum(proj, filum, func_titulus,
                                 ARBOR_SYMBOLUM_FUNCTIO, nexus, nodus, nodus);

            /* Recurse into function body but not at file scope */
            si (nodus->datum.functio.corpus != NIHIL)
            {
                _extrahere_symbola(proj, filum, nodus->datum.functio.corpus, FALSUM);
            }
            frange;
        }

        casus ARBOR_NODUS_DECLARATION:
        {
            si (in_scopo_fili)
            {
                /* File-scope declarations: global variables, typedefs */
                Xar* specifiers = nodus->datum.declaratio.specifiers;
                Xar* declaratores = nodus->datum.declaratio.declaratores;
                ArborNexus nexus = _determinare_nexum(nodus);
                b32 est_typedef = FALSUM;
                i32 i;

                /* Verificare si est typedef */
                si (specifiers != NIHIL)
                {
                    per (i = ZEPHYRUM; i < xar_numerus(specifiers); i++)
                    {
                        ArborNodus** spec_ptr = xar_obtinere(specifiers, i);
                        si (spec_ptr != NIHIL)
                        {
                            ArborNodus* spec = *spec_ptr;
                            si (spec->genus == ARBOR_NODUS_STORAGE_CLASS &&
                                spec->datum.folium.valor != NIHIL &&
                                spec->datum.folium.valor->mensura == VII &&
                                memcmp(spec->datum.folium.valor->datum, "typedef", VII) == ZEPHYRUM)
                            {
                                est_typedef = VERUM;
                                frange;
                            }
                        }
                    }

                    /* Etiam processare struct/union/enum specifiers in declaratione */
                    per (i = ZEPHYRUM; i < xar_numerus(specifiers); i++)
                    {
                        ArborNodus** spec_ptr = xar_obtinere(specifiers, i);
                        si (spec_ptr != NIHIL)
                        {
                            ArborNodus* spec = *spec_ptr;
                            si (spec->genus == ARBOR_NODUS_STRUCT_SPECIFIER ||
                                spec->genus == ARBOR_NODUS_UNION_SPECIFIER ||
                                spec->genus == ARBOR_NODUS_ENUM_SPECIFIER)
                            {
                                _extrahere_symbola(proj, filum, spec, VERUM);
                            }
                        }
                    }
                }

                si (declaratores != NIHIL)
                {
                    per (i = ZEPHYRUM; i < xar_numerus(declaratores); i++)
                    {
                        ArborNodus** decl_ptr = xar_obtinere(declaratores, i);
                        si (decl_ptr != NIHIL)
                        {
                            ArborNodus* decl = *decl_ptr;
                            chorda* var_titulus = _obtinere_nomen_declaratoris(decl);

                            si (est_typedef)
                            {
                                _registrare_symbolum(proj, filum, var_titulus,
                                                     ARBOR_SYMBOLUM_TYPUS, ARBOR_NEXUS_EXTERNUS,
                                                     nodus, NIHIL);
                            }
                            alioquin si (_est_declarator_functionis(decl))
                            {
                                /* Declaratio functionis (e.g., int foo(void);) */
                                _registrare_symbolum(proj, filum, var_titulus,
                                                     ARBOR_SYMBOLUM_FUNCTIO, nexus,
                                                     nodus, NIHIL);
                            }
                            alioquin
                            {
                                _registrare_symbolum(proj, filum, var_titulus,
                                                     ARBOR_SYMBOLUM_VARIABILIS, nexus,
                                                     nodus, NIHIL);
                            }
                        }
                    }
                }
            }
            frange;
        }

        casus ARBOR_NODUS_STRUCT_SPECIFIER:
        casus ARBOR_NODUS_UNION_SPECIFIER:
        {
            /* Register struct/union tag */
            chorda* tag_titulus = nodus->datum.aggregatum.titulus;
            si (tag_titulus != NIHIL && in_scopo_fili)
            {
                _registrare_symbolum(proj, filum, tag_titulus,
                                     ARBOR_SYMBOLUM_TYPUS, ARBOR_NEXUS_EXTERNUS,
                                     nodus, nodus);
            }
            frange;
        }

        casus ARBOR_NODUS_ENUM_SPECIFIER:
        {
            /* Register enum tag and values */
            chorda* enum_titulus = nodus->datum.enum_spec.titulus;
            Xar* enumeratores = nodus->datum.enum_spec.enumeratores;

            si (enum_titulus != NIHIL && in_scopo_fili)
            {
                _registrare_symbolum(proj, filum, enum_titulus,
                                     ARBOR_SYMBOLUM_TYPUS, ARBOR_NEXUS_EXTERNUS,
                                     nodus, nodus);
            }

            /* Register enum values */
            si (enumeratores != NIHIL && in_scopo_fili)
            {
                i32 i;
                per (i = ZEPHYRUM; i < xar_numerus(enumeratores); i++)
                {
                    ArborNodus** enum_ptr = xar_obtinere(enumeratores, i);
                    si (enum_ptr != NIHIL)
                    {
                        ArborNodus* enumerator = *enum_ptr;

                        si (enumerator->genus == ARBOR_NODUS_ENUMERATOR ||
                            enumerator->genus == ARBOR_NODUS_IDENTIFIER)
                        {
                            chorda* val_titulus = enumerator->datum.folium.valor;
                            si (val_titulus != NIHIL)
                            {
                                _registrare_symbolum(proj, filum, val_titulus,
                                                     ARBOR_SYMBOLUM_ENUM_VALOR,
                                                     ARBOR_NEXUS_EXTERNUS,
                                                     enumerator, enumerator);
                            }
                        }
                    }
                }
            }
            frange;
        }

        casus ARBOR_NODUS_COMPOUND_STATEMENT:
        {
            /* Recurse into compound statements but not at file scope */
            Xar* sententiae = nodus->datum.compositum.sententiae;
            si (sententiae != NIHIL)
            {
                i32 i;
                per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
                {
                    ArborNodus** sent_ptr = xar_obtinere(sententiae, i);
                    si (sent_ptr != NIHIL)
                    {
                        _extrahere_symbola(proj, filum, *sent_ptr, FALSUM);
                    }
                }
            }
            frange;
        }

        ordinarius:
        {
            /* For other nodes, try to recurse into children */
            Xar* liberi = nodus->datum.genericum.liberi;
            si (liberi != NIHIL)
            {
                i32 i;
                per (i = ZEPHYRUM; i < xar_numerus(liberi); i++)
                {
                    ArborNodus** child_ptr = xar_obtinere(liberi, i);
                    si (child_ptr != NIHIL)
                    {
                        _extrahere_symbola(proj, filum, *child_ptr, in_scopo_fili);
                    }
                }
            }
            frange;
        }
    }
}

interior vacuum
_colligere_referentias(ArborProiectum* proj, ArborFilum* filum, ArborNodus* nodus)
{
    si (nodus == NIHIL) redde;

    /* Si identifier, try to add reference */
    si (nodus->genus == ARBOR_NODUS_IDENTIFIER)
    {
        chorda* id_titulus = nodus->datum.folium.valor;
        ArborIndexSymbolum* sym;

        si (id_titulus != NIHIL)
        {
            sym = arbor_index_invenire(proj, id_titulus);
            si (sym != NIHIL && sym->referentiae != NIHIL)
            {
                ArborIndexReferentia* ref_slot;
                ref_slot = xar_addere(sym->referentiae);
                si (ref_slot != NIHIL)
                {
                    ref_slot->filum = filum;
                    ref_slot->linea = nodus->linea_initium;
                    ref_slot->columna = nodus->columna_initium;
                    ref_slot->nodus = nodus;
                }
            }
        }
    }

    /* Recurse based on node type */
    commutatio (nodus->genus)
    {
        casus ARBOR_NODUS_TRANSLATION_UNIT:
        {
            Xar* liberi = nodus->datum.genericum.liberi;
            si (liberi != NIHIL)
            {
                i32 i;
                per (i = ZEPHYRUM; i < xar_numerus(liberi); i++)
                {
                    ArborNodus** child_ptr = xar_obtinere(liberi, i);
                    si (child_ptr != NIHIL)
                    {
                        _colligere_referentias(proj, filum, *child_ptr);
                    }
                }
            }
            frange;
        }

        casus ARBOR_NODUS_FUNCTION_DEFINITION:
        {
            /* Recurse into function body */
            _colligere_referentias(proj, filum, nodus->datum.functio.corpus);
            frange;
        }

        casus ARBOR_NODUS_COMPOUND_STATEMENT:
        {
            Xar* sententiae = nodus->datum.compositum.sententiae;
            si (sententiae != NIHIL)
            {
                i32 i;
                per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
                {
                    ArborNodus** sent_ptr = xar_obtinere(sententiae, i);
                    si (sent_ptr != NIHIL)
                    {
                        _colligere_referentias(proj, filum, *sent_ptr);
                    }
                }
            }
            frange;
        }

        casus ARBOR_NODUS_BINARY_EXPRESSION:
        {
            _colligere_referentias(proj, filum, nodus->datum.binarium.sinister);
            _colligere_referentias(proj, filum, nodus->datum.binarium.dexter);
            frange;
        }

        casus ARBOR_NODUS_UNARY_EXPRESSION:
        {
            _colligere_referentias(proj, filum, nodus->datum.unarium.operandum);
            frange;
        }

        casus ARBOR_NODUS_CALL_EXPRESSION:
        {
            _colligere_referentias(proj, filum, nodus->datum.vocatio.callee);
            si (nodus->datum.vocatio.argumenta != NIHIL)
            {
                i32 i;
                per (i = ZEPHYRUM; i < xar_numerus(nodus->datum.vocatio.argumenta); i++)
                {
                    ArborNodus** arg_ptr = xar_obtinere(nodus->datum.vocatio.argumenta, i);
                    si (arg_ptr != NIHIL)
                    {
                        _colligere_referentias(proj, filum, *arg_ptr);
                    }
                }
            }
            frange;
        }

        casus ARBOR_NODUS_IF_STATEMENT:
        {
            _colligere_referentias(proj, filum, nodus->datum.conditionale.conditio);
            _colligere_referentias(proj, filum, nodus->datum.conditionale.consequens);
            _colligere_referentias(proj, filum, nodus->datum.conditionale.alternans);
            frange;
        }

        casus ARBOR_NODUS_WHILE_STATEMENT:
        casus ARBOR_NODUS_DO_STATEMENT:
        {
            _colligere_referentias(proj, filum, nodus->datum.iteratio.conditio);
            _colligere_referentias(proj, filum, nodus->datum.iteratio.corpus);
            frange;
        }

        casus ARBOR_NODUS_FOR_STATEMENT:
        {
            _colligere_referentias(proj, filum, nodus->datum.circuitus.init);
            _colligere_referentias(proj, filum, nodus->datum.circuitus.conditio);
            _colligere_referentias(proj, filum, nodus->datum.circuitus.post);
            _colligere_referentias(proj, filum, nodus->datum.circuitus.corpus);
            frange;
        }

        casus ARBOR_NODUS_RETURN_STATEMENT:
        {
            _colligere_referentias(proj, filum, nodus->datum.reditio.valor);
            frange;
        }

        casus ARBOR_NODUS_EXPRESSION_STATEMENT:
        {
            /* Expression statements have a single child expression */
            Xar* liberi = nodus->datum.genericum.liberi;
            si (liberi != NIHIL && xar_numerus(liberi) > ZEPHYRUM)
            {
                ArborNodus** expr_ptr = xar_obtinere(liberi, ZEPHYRUM);
                si (expr_ptr != NIHIL)
                {
                    _colligere_referentias(proj, filum, *expr_ptr);
                }
            }
            frange;
        }

        casus ARBOR_NODUS_MEMBER_EXPRESSION:
        {
            _colligere_referentias(proj, filum, nodus->datum.membrum.objectum);
            frange;
        }

        casus ARBOR_NODUS_SUBSCRIPT_EXPRESSION:
        {
            _colligere_referentias(proj, filum, nodus->datum.subscriptum.array);
            _colligere_referentias(proj, filum, nodus->datum.subscriptum.index);
            frange;
        }

        casus ARBOR_NODUS_ASSIGNMENT_EXPRESSION:
        {
            /* Assignments are binary expressions */
            _colligere_referentias(proj, filum, nodus->datum.binarium.sinister);
            _colligere_referentias(proj, filum, nodus->datum.binarium.dexter);
            frange;
        }

        casus ARBOR_NODUS_CONDITIONAL_EXPRESSION:
        {
            _colligere_referentias(proj, filum, nodus->datum.ternarium.conditio);
            _colligere_referentias(proj, filum, nodus->datum.ternarium.verum);
            _colligere_referentias(proj, filum, nodus->datum.ternarium.falsum);
            frange;
        }

        ordinarius:
        {
            /* Generic fallback: try to iterate children */
            Xar* liberi = nodus->datum.genericum.liberi;
            si (liberi != NIHIL)
            {
                i32 i;
                per (i = ZEPHYRUM; i < xar_numerus(liberi); i++)
                {
                    ArborNodus** child_ptr = xar_obtinere(liberi, i);
                    si (child_ptr != NIHIL)
                    {
                        _colligere_referentias(proj, filum, *child_ptr);
                    }
                }
            }
            frange;
        }
    }
}

interior ArborNodus*
_invenire_nodum_ad_punctum(ArborNodus* radix, i32 linea, i32 columna)
{
    ArborNodus* optimum = NIHIL;
    Xar* liberi;
    i32 i;

    si (radix == NIHIL) redde NIHIL;

    /* Check if this node contains the position */
    si (radix->linea_initium <= linea && linea <= radix->linea_finis)
    {
        /* Position is within line range */
        si (radix->linea_initium == radix->linea_finis)
        {
            /* Single line node - check columns */
            si (radix->columna_initium <= columna && columna <= radix->columna_finis)
            {
                optimum = radix;
            }
        }
        alioquin si (linea == radix->linea_initium)
        {
            /* First line - check start column */
            si (columna >= radix->columna_initium)
            {
                optimum = radix;
            }
        }
        alioquin si (linea == radix->linea_finis)
        {
            /* Last line - check end column */
            si (columna <= radix->columna_finis)
            {
                optimum = radix;
            }
        }
        alioquin
        {
            /* Middle line - definitely inside */
            optimum = radix;
        }
    }

    /* Try to find a more specific (deeper) match in children */
    commutatio (radix->genus)
    {
        casus ARBOR_NODUS_TRANSLATION_UNIT:
            liberi = radix->datum.genericum.liberi;
            frange;
        casus ARBOR_NODUS_COMPOUND_STATEMENT:
            liberi = radix->datum.compositum.sententiae;
            frange;
        casus ARBOR_NODUS_FUNCTION_DEFINITION:
            {
                ArborNodus* res;
                res = _invenire_nodum_ad_punctum(radix->datum.functio.corpus, linea, columna);
                si (res != NIHIL) redde res;
            }
            redde optimum;
        ordinarius:
            liberi = radix->datum.genericum.liberi;
            frange;
    }

    si (liberi != NIHIL)
    {
        per (i = ZEPHYRUM; i < xar_numerus(liberi); i++)
        {
            ArborNodus** child_ptr = xar_obtinere(liberi, i);
            si (child_ptr != NIHIL)
            {
                ArborNodus* child_result = _invenire_nodum_ad_punctum(*child_ptr, linea, columna);
                si (child_result != NIHIL)
                {
                    redde child_result;
                }
            }
        }
    }

    redde optimum;
}
