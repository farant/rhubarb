/* natura_examen.c - iudicium bibliothecae semanticae (facies CLI
 * oneratoris lib/natura.c).
 *
 * RATIO: natura_visus.sh XIV secundis currit - nimis tarde ut
 * uncus post-editionem sit. Onerator XXIX MILLESIMIS currit, id
 * est CCCCLXXX vicibus celerius, ET plus videt (regulae IX-XIII
 * resolutionem hereditatis poscunt quam tabula shell praestare
 * non potest). Unde hoc: porta velox quae in unco vivere potest.
 *
 * MOMENTUM: exemplaria OMNIA semper onerantur, etiam cum una
 * plagula sola iudicatur - regulae II/III/IV trans exemplaria
 * transeunt, et iudicium plagulae solius eas videre NON POTEST.
 * Filtrum (-plagula) post nexuram applicatur, non ante.
 *
 * Usus:
 *   bin/natura_examen                     omnia (natura/)
 *   bin/natura_examen -plagula VIA        diagnostica huius solius
 *   bin/natura_examen -machina            TSV purum
 *   bin/natura_examen -radix DIR          alia sedes
 *   bin/natura_examen -tabulae DIR        tabulas extractionis scribere
 *
 * TABULAE (gradus II): natura_visus.sh eas prius per XLVIII
 * vocationes xmllint aedificabat - id est PARSATIO ALTERA eiusdem
 * corporis, cum interpretatione altera. Onerator ea omnia iam in
 * memoria tenet; ergo ea EMITTIT et visus solam praesentationem
 * agit. Una parsatio, una veritas, nullum xmllint.
 *
 * Exitus: 0 = sanum; 1 = VULNERA; 2 = NIHIL CURSUM EST
 *   (exitus II disciplina domus: nihil onerari NON est successus,
 *    et porta quae in solo zephyro pendet mendacium tacitum est)
 */
#include "postulata_posix.h"

#include "latina.h"
#include "natura.h"
#include "filum.h"
#include "iter_directoria.h"
#include <stdio.h>
#include <string.h>

#define EXTENSIO       ".genera"
#define EXTENSIO_LONGA VII

interior b32 extensionem_habet(constans chorda* titulus);
interior vacuum tabulas_scribere(NaturaBibliotheca* bib,
                                 constans character* dir);
interior vacuum corpus_scribere(NaturaBibliotheca* bib,
                                constans character* via);
interior vacuum stirpem_scribere(constans chorda* titulus,
                                 character* exitus, i32 tectum);
interior b32 viam_congruere(constans chorda* modulus,
                            constans character* plagula);

/* an titulus in ".genera" desinat */
interior b32
extensionem_habet(
    constans chorda* titulus)
{
    si (titulus->mensura <= EXTENSIO_LONGA)
    {
        redde FALSUM;
    }

    redde (b32)(memcmp(titulus->datum +
                       (titulus->mensura - EXTENSIO_LONGA),
                       EXTENSIO,
                       (memoriae_index)EXTENSIO_LONGA) == ZEPHYRUM);
}

/* titulus sine extensione - regula XIV eam contra attributum
 * modulus iudicat */
interior vacuum
stirpem_scribere(
    constans chorda*  titulus,
    character*        exitus,
    i32               tectum)
{
    i32 mensura;

    mensura = titulus->mensura - EXTENSIO_LONGA;
    si (mensura >= tectum)
    {
        mensura = tectum - I;
    }

    memcpy(exitus, titulus->datum, (memoriae_index)mensura);
    exitus[mensura] = '\0';
}

/* an via data hoc exemplar nominet (suffixum sufficit:
 * 'natura/mensura.genera', 'mensura.genera', 'mensura') */
interior b32
viam_congruere(
    constans chorda*     modulus,
    constans character*  plagula)
{
    constans character* basis;
    constans character* punctum;
    size_t              mensura;

    basis = strrchr(plagula, '/');
    basis = basis ? basis + I : plagula;
    punctum = strrchr(basis, '.');
    mensura = punctum ? (size_t)(punctum - basis) : strlen(basis);

    si (mensura != (size_t)modulus->mensura)
    {
        redde FALSUM;
    }

    redde (b32)(memcmp(basis, modulus->datum, mensura) == ZEPHYRUM);
}

/* ==================================================
 * Tabulae extractionis (gradus II)
 * ================================================== */

/* omnes posteros cum titulo dato, ORDINE DOCUMENTI */
interior vacuum
_colligere(
    StmlNodus*           nodus,
    constans character*  titulus,
    Xar*                 fructus)
{
    i32 numerus;
    i32 i;

    numerus = stml_numerus_liberorum(nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liberum;

        liberum = stml_liberum_ad_indicem(nodus, i);
        si (!liberum || liberum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (chorda_aequalis_literis(*liberum->titulus, titulus))
        {
            StmlNodus** locus;

            locus = (StmlNodus**)xar_addere(fructus);
            *locus = liberum;
        }
        _colligere(liberum, titulus, fructus);
    }
}

/* omnia elementa, ordine documenti - pro attributis quae in
 * elemento quolibet sedere possunt (fons, certitudo, valens_*,
 * externum) */
interior vacuum
_colligere_omnia_elementa(
    StmlNodus* nodus,
    Xar*       fructus)
{
    i32 numerus;
    i32 i;

    numerus = stml_numerus_liberorum(nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus*  liberum;
        StmlNodus** locus;

        liberum = stml_liberum_ad_indicem(nodus, i);
        si (!liberum || liberum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }

        locus = (StmlNodus**)xar_addere(fructus);
        *locus = liberum;
        _colligere_omnia_elementa(liberum, fructus);
    }
}

/* liberos DIRECTOS numerare */
interior i32
_numerare_liberos(
    StmlNodus*           nodus,
    constans character*  titulus)
{
    i32 numerus;
    i32 i;
    i32 summa;

    summa   = ZEPHYRUM;
    numerus = stml_numerus_liberorum(nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liberum;

        liberum = stml_liberum_ad_indicem(nodus, i);
        si (liberum && liberum->genus == STML_NODUS_ELEMENTUM &&
            chorda_aequalis_literis(*liberum->titulus, titulus))
        {
            summa++;
        }
    }

    redde summa;
}

/* attributum ut literae (buffer vocantis); vacuum si abest */
interior vacuum
_attr(
    StmlNodus*           nodus,
    constans character*  titulus,
    character*           exitus,
    i32                  tectum)
{
    chorda* v;

    exitus[ZEPHYRUM] = '\0';
    v = stml_attributum_capere(nodus, titulus);
    si (v && v->mensura > ZEPHYRUM)
    {
        i32 m;

        m = v->mensura < tectum - I ? v->mensura : tectum - I;
        memcpy(exitus, v->datum, (memoriae_index)m);
        exitus[m] = '\0';
    }
}

/* prosa ad campum unius lineae: '\n'->' ', '|'->'/', spatia
 * coartata, spatium primum abscisum - idem quod tr et sed in
 * natura_visus.sh agebant. Separator in prosa campum TACITE
 * corrumpit (vitium quod domus bis mensuravit).
 *
 * CAVE: expressio sed originalis hic NON citatur, quia stellam
 * ante virgulam fert et commentarium ANTE TEMPUS clauderet -
 * quod uncus examinis statim deprehendit. */
interior vacuum
_prosa_ex(
    chorda      textus,
    character*  exitus,
    i32         tectum,
    b32         virgulam_mutare)
{
    i32 i;
    i32 j;
    b32 spatium_prius;

    j = ZEPHYRUM;
    spatium_prius = VERUM;   /* spatium primum abscindit */

    per (i = ZEPHYRUM; i < textus.mensura && j < tectum - I; i++)
    {
        character c;

        c = (character)textus.datum[i];
        si (c == '\n' || c == '\r' || c == '\t')
        {
            c = ' ';
        }
        si (c == '|' && virgulam_mutare)
        {
            c = '/';
        }

        si (c == ' ')
        {
            si (spatium_prius)
            {
                perge;
            }
            spatium_prius = VERUM;
        }
        alioquin
        {
            spatium_prius = FALSUM;
        }

        exitus[j++] = c;
    }

    exitus[j] = '\0';
}

/* tabulae visus virgulam (|) separatorem habent, ergo eam mutant;
 * corpus quaerentis TABULATIS separatur, ergo virgulam SERVAT -
 * mutatio ibi contentum falsaret */
interior vacuum
_prosa(
    chorda      textus,
    character*  exitus,
    i32         tectum)
{
    _prosa_ex(textus, exitus, tectum, VERUM);
}

/* ordo alphabeticus exemplarium - glob conchae eum praestat, et
 * tabulae conferri debent */
interior s32
_comparare_exemplaria(
    constans vacuum* a,
    constans vacuum* b)
{
    NaturaExemplar* ea;
    NaturaExemplar* eb;

    ea = *(NaturaExemplar* constans*)a;
    eb = *(NaturaExemplar* constans*)b;

    redde chorda_comparare(*ea->stirps, *eb->stirps);
}

interior vacuum
tabulas_scribere(
    NaturaBibliotheca*   bib,
    constans character*  dir)
{
    FILE*     f_genera;
    FILE*     f_res;
    FILE*     f_arcus;
    FILE*     f_umbrae;
    FILE*     f_dubia;
    FILE*     f_fontes;
    FILE*     f_citationes;
    FILE*     f_assensus;
    FILE*     f_transradices;
    FILE*     f_validitas;
    FILE*     f_glossae;
    FILE*     f_resgen;
    FILE*     f_moduli;
    character via[DXII];
    i32       ie;

#define APERIRE(var, nomen_tabulae)                                \
    sprintf(via, "%s/%s", dir, nomen_tabulae);                     \
    var = fopen(via, "w");                                         \
    si (!var) { fprintf(stderr, "natura_examen: %s scribi nequit\n", via); redde; }

    APERIRE(f_genera,       "genera.txt")
    APERIRE(f_res,          "res.txt")
    APERIRE(f_arcus,        "arcus.txt")
    APERIRE(f_umbrae,       "umbrae.txt")
    APERIRE(f_dubia,        "dubia.txt")
    APERIRE(f_fontes,       "fontes.txt")
    APERIRE(f_citationes,   "citationes.txt")
    APERIRE(f_assensus,     "assensus.txt")
    APERIRE(f_transradices, "transradices.txt")
    APERIRE(f_validitas,    "validitas.txt")
    APERIRE(f_glossae,      "glossae.txt")
    APERIRE(f_resgen,       "resgen.txt")
    APERIRE(f_moduli,       "moduli.txt")
#undef APERIRE

    xar_ordinare(bib->exemplaria, _comparare_exemplaria);

    per (ie = ZEPHYRUM; ie < xar_numerus(bib->exemplaria); ie++)
    {
        NaturaExemplar* ex;
        Piscina*        p;
        PiscinaNotatio  nota;
        Xar*            nodi;
        i32             i;
        character       mod[CCLVI];
        constans character* gradus_nomina[III];
        i32             ig;

        ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria, ie);
        p  = bib->piscina;
        nota = piscina_notare(p);

        _attr(ex->radix, "modulus", mod, (i32)magnitudo(mod));
        {
            character ver[LXIV];

            _attr(ex->radix, "versio", ver, (i32)magnitudo(ver));
            fprintf(f_moduli, "%s:%s\n", mod, ver);
        }

        /* ---- genera ---- */
        nodi = xar_creare(p, (i32)magnitudo(StmlNodus*));
        _colligere(ex->radix, "genus", nodi);
        per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
        {
            StmlNodus* g;
            character  gn[CCLVI];
            character  sub[CCLVI];
            character  submod[CCLVI];
            character  glossa[DXII];
            StmlNodus* def;
            Xar*       dubia_sub;
            i32        npa;
            StmlNodus* partes;

            g = *(StmlNodus**)xar_obtinere(nodi, i);
            _attr(g, "nomen",   gn,     (i32)magnitudo(gn));
            _attr(g, "sub",     sub,    (i32)magnitudo(sub));
            _attr(g, "modulus", submod, (i32)magnitudo(submod));

            /* subordinatio TRANS EXEMPLARIA: arcus, et radix in
             * silva sua (mechanismus rotae XVII) */
            si (sub[ZEPHYRUM] && submod[ZEPHYRUM] &&
                strcmp(submod, mod) != ZEPHYRUM)
            {
                fprintf(f_arcus, "%s|%s|sub|%s|%s|%s\n",
                        mod, gn, submod, sub, gn);
                fprintf(f_transradices, "%s|%s|%s.%s\n",
                        mod, gn, submod, sub);
                sub[ZEPHYRUM] = '\0';
            }

            dubia_sub = xar_creare(p, (i32)magnitudo(StmlNodus*));
            _colligere(g, "dubium", dubia_sub);

            partes = stml_invenire_liberum(g, "partes");
            npa = partes ? _numerare_liberos(partes, "pars")
                         : ZEPHYRUM;

            fprintf(f_genera, "%s|%s|%s|%u|%u|%u|%u|%u\n",
                    mod, gn, sub,
                    _numerare_liberos(g, "species"),
                    _numerare_liberos(g, "individuum"),
                    xar_numerus(dubia_sub),
                    _numerare_liberos(g, "machina_statuum"),
                    npa);

            /* glossa: sententia prima definitionis (aut
             * differentiae, qua sub-genera saepe sola utuntur) */
            def = stml_invenire_liberum(g, "definitio");
            si (!def)
            {
                def = stml_invenire_liberum(g, "differentia");
            }
            si (def)
            {
                character bruta[MM];
                i32       k;

                _prosa(stml_textus_internus(def, p), bruta,
                       (i32)magnitudo(bruta));
                /* cut -d'.' -f1 deinde cut -c1-118 */
                per (k = ZEPHYRUM; bruta[k]; k++)
                {
                    si (bruta[k] == '.')
                    {
                        bruta[k] = '\0';
                        frange;
                    }
                }
                bruta[CXVIII] = '\0';
                memcpy(glossa, bruta, strlen(bruta) + I);
            }
            alioquin
            {
                glossa[ZEPHYRUM] = '\0';
            }
            fprintf(f_glossae, "%s|%s|%s\n", mod, gn, glossa);
        }

        /* ---- res dictionarii (ordine graduum, ut visus) ---- */
        gradus_nomina[ZEPHYRUM] = "species";
        gradus_nomina[I]        = "individuum";
        gradus_nomina[II]       = "cultivar";
        per (ig = ZEPHYRUM; ig < III; ig++)
        {
            Xar* rr;

            rr = xar_creare(p, (i32)magnitudo(StmlNodus*));
            _colligere(ex->radix, gradus_nomina[ig], rr);
            per (i = ZEPHYRUM; i < xar_numerus(rr); i++)
            {
                StmlNodus* r;
                StmlNodus* maior;
                character  rn[CCLVI];
                character  rg[CCLVI];

                r = *(StmlNodus**)xar_obtinere(rr, i);
                _attr(r, "nomen", rn, (i32)magnitudo(rn));
                si (!rn[ZEPHYRUM])
                {
                    perge;
                }

                rg[ZEPHYRUM] = '\0';
                per (maior = r->parens; maior; maior = maior->parens)
                {
                    si (maior->genus == STML_NODUS_ELEMENTUM &&
                        chorda_aequalis_literis(*maior->titulus,
                                                "genus"))
                    {
                        _attr(maior, "nomen", rg,
                              (i32)magnitudo(rg));
                        frange;
                    }
                }

                fprintf(f_res, "%s|%s\n", mod, rn);
                fprintf(f_resgen, "%s|%s|%s%s\n", mod, rg,
                        ig == I ? ":" : "", rn);
            }
        }

        /* ---- arcus: relatio/relatum ---- */
        {
            constans character* elementa[II];
            constans character* attributa[II];
            i32                 ielem;
            i32                 iattr;

            elementa[ZEPHYRUM]  = "relatio";
            elementa[I]         = "relatum";
            attributa[ZEPHYRUM] = "ad";
            attributa[I]        = "a";

            per (ielem = ZEPHYRUM; ielem < II; ielem++)
            {
                per (iattr = ZEPHYRUM; iattr < II; iattr++)
                {
                    Xar* rr;

                    rr = xar_creare(p, (i32)magnitudo(StmlNodus*));
                    _colligere(ex->radix, elementa[ielem], rr);
                    per (i = ZEPHYRUM; i < xar_numerus(rr); i++)
                    {
                        StmlNodus* r;
                        StmlNodus* maior;
                        character  rel[CCLVI];
                        character  tgt[CCLVI];
                        character  tm[CCLVI];
                        character  fg[CCLVI];
                        character  fr[CCLVI];

                        r = *(StmlNodus**)xar_obtinere(rr, i);
                        _attr(r, attributa[iattr], tgt,
                              (i32)magnitudo(tgt));
                        /* [@attr][not(@externum)][@attr!='*'] */
                        si (!tgt[ZEPHYRUM] ||
                            strcmp(tgt, "*") == ZEPHYRUM ||
                            stml_attributum_capere(r, "externum"))
                        {
                            perge;
                        }

                        _attr(r, "nomen",   rel, (i32)magnitudo(rel));
                        _attr(r, "modulus", tm,  (i32)magnitudo(tm));
                        si (!tm[ZEPHYRUM])
                        {
                            memcpy(tm, mod, strlen(mod) + I);
                        }

                        /* genus PROXIMUM (ancestor::genus[1]) */
                        fg[ZEPHYRUM] = '\0';
                        per (maior = r->parens; maior;
                             maior = maior->parens)
                        {
                            si (maior->genus ==
                                    STML_NODUS_ELEMENTUM &&
                                chorda_aequalis_literis(
                                    *maior->titulus, "genus"))
                            {
                                _attr(maior, "nomen", fg,
                                      (i32)magnitudo(fg));
                                frange;
                            }
                        }

                        /* FONS PRECISUS: ancestor::*[@nomen][1] -
                         * relatum intra speciem a SPECIE asseritur */
                        fr[ZEPHYRUM] = '\0';
                        per (maior = r->parens; maior;
                             maior = maior->parens)
                        {
                            si (maior->genus ==
                                    STML_NODUS_ELEMENTUM &&
                                stml_attributum_capere(maior,
                                                       "nomen"))
                            {
                                _attr(maior, "nomen", fr,
                                      (i32)magnitudo(fr));
                                frange;
                            }
                        }

                        fprintf(f_arcus, "%s|%s|%s|%s|%s|%s\n",
                                mod, fg, rel, tm, tgt, fr);
                    }
                }
            }
        }

        /* ---- arcus: proprietates typo bibliothecae ---- */
        nodi = xar_creare(p, (i32)magnitudo(StmlNodus*));
        _colligere(ex->radix, "proprietas", nodi);
        per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
        {
            StmlNodus* pr;
            StmlNodus* maior;
            character  pn[CCLVI];
            character  pg[CCLVI];
            character  pm[CCLVI];
            character  pf[CCLVI];

            pr = *(StmlNodus**)xar_obtinere(nodi, i);
            si (!stml_attributum_capere(pr, "modulus"))
            {
                perge;
            }

            _attr(pr, "nomen",   pn, (i32)magnitudo(pn));
            _attr(pr, "genus",   pg, (i32)magnitudo(pg));
            _attr(pr, "modulus", pm, (i32)magnitudo(pm));

            pf[ZEPHYRUM] = '\0';
            per (maior = pr->parens; maior; maior = maior->parens)
            {
                si (maior->genus == STML_NODUS_ELEMENTUM &&
                    chorda_aequalis_literis(*maior->titulus,
                                            "genus"))
                {
                    _attr(maior, "nomen", pf, (i32)magnitudo(pf));
                    frange;
                }
            }

            fprintf(f_arcus, "%s|%s|proprietas:%s|%s|%s|%s\n",
                    mod, pf, pn, pm, pg, pf);
        }

        /* ---- fontes declarati ---- */
        {
            StmlNodus* fontes_n;

            fontes_n = stml_invenire_liberum(ex->radix, "fontes");
            si (fontes_n)
            {
                nodi = xar_creare(p, (i32)magnitudo(StmlNodus*));
                _colligere(fontes_n, "fons", nodi);
                per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
                {
                    character fc[CCLVI];

                    _attr(*(StmlNodus**)xar_obtinere(nodi, i),
                          "clavis", fc, (i32)magnitudo(fc));
                    fprintf(f_fontes, "%s|%s\n", mod, fc);
                }
            }
        }

        /* ---- citationes (omne @fons), assensus (@certitudo),
         *      validitas (@valens_*), umbrae (@externum) ----
         * uno percursu, quia omnia elementa quaelibet tangunt */
        {
            Xar* omnia;

            omnia = xar_creare(p, (i32)magnitudo(StmlNodus*));
            _colligere_omnia_elementa(ex->radix, omnia);

            per (i = ZEPHYRUM; i < xar_numerus(omnia); i++)
            {
                StmlNodus* n;
                chorda*    v;

                n = *(StmlNodus**)xar_obtinere(omnia, i);

                v = stml_attributum_capere(n, "fons");
                si (v)
                {
                    character fu[CCLVI];
                    character fn[CCLVI];

                    _attr(n, "fons",  fu, (i32)magnitudo(fu));
                    _attr(n, "nomen", fn, (i32)magnitudo(fn));
                    si (!fn[ZEPHYRUM])
                    {
                        _attr(n, "quando", fn, (i32)magnitudo(fn));
                    }
                    si (!fn[ZEPHYRUM])
                    {
                        memcpy(fn, "(innominatum)", XIV);
                    }
                    fprintf(f_citationes, "%s|%s|%s\n", mod, fu, fn);
                }

                v = stml_attributum_capere(n, "certitudo");
                si (v)
                {
                    character cv[CCLVI];

                    _attr(n, "certitudo", cv, (i32)magnitudo(cv));
                    fprintf(f_assensus, "%s|%s\n", mod, cv);
                }

                si (stml_attributum_capere(n, "valens_a") ||
                    stml_attributum_capere(n, "valens_ad"))
                {
                    character va[CCLVI];
                    character vd[CCLVI];
                    character vn[CCLVI];

                    _attr(n, "valens_a",  va, (i32)magnitudo(va));
                    _attr(n, "valens_ad", vd, (i32)magnitudo(vd));
                    _attr(n, "nomen",     vn, (i32)magnitudo(vn));
                    fprintf(f_validitas, "%s|%s|%s|%s\n",
                            mod, vn, va, vd);
                }

                v = stml_attributum_capere(n, "externum");
                si (v && chorda_aequalis_literis(*v, "verum"))
                {
                    character u[CCLVI];

                    _attr(n, "ad", u, (i32)magnitudo(u));
                    si (u[ZEPHYRUM])
                    {
                        fprintf(f_umbrae, "%s|%s|%.*s\n", u, mod,
                            (integer)n->titulus->mensura,
                            (constans character*)n->titulus->datum);
                    }
                }
            }
        }

        /* ---- dubia cum contextu ET TEXTU ---- */
        nodi = xar_creare(p, (i32)magnitudo(StmlNodus*));
        _colligere(ex->radix, "dubium", nodi);
        per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
        {
            StmlNodus* d;
            StmlNodus* maior;
            character  ctx[CCLVI];
            character  dtx[MM];

            d = *(StmlNodus**)xar_obtinere(nodi, i);

            /* ancestor-or-self::*[@nomen][1] */
            ctx[ZEPHYRUM] = '\0';
            per (maior = d; maior; maior = maior->parens)
            {
                si (maior->genus == STML_NODUS_ELEMENTUM &&
                    stml_attributum_capere(maior, "nomen"))
                {
                    _attr(maior, "nomen", ctx, (i32)magnitudo(ctx));
                    frange;
                }
            }
            si (!ctx[ZEPHYRUM])
            {
                memcpy(ctx, "(modulus)", X);
            }

            _prosa(stml_textus_internus(d, p), dtx,
                   (i32)magnitudo(dtx));
            dtx[CLX] = '\0';   /* cut -c1-160 */
            fprintf(f_dubia, "%s|%s|%s\n", mod, ctx, dtx);
        }

        piscina_reficere(p, nota);
    }

    fclose(f_genera);       fclose(f_res);
    fclose(f_arcus);        fclose(f_umbrae);
    fclose(f_dubia);        fclose(f_fontes);
    fclose(f_citationes);   fclose(f_assensus);
    fclose(f_transradices); fclose(f_validitas);
    fclose(f_glossae);      fclose(f_resgen);
    fclose(f_moduli);
}

/* ==================================================
 * Corpus quaerentis (gradus II) - forma TSV, ordines
 * G/P/M/A/R/S/U quos natura_quaere.sh consumit.
 *
 * P/M/A/R e CONTINENTIBUS DIRECTIS generis sumuntur (non e
 * posteris): genus nidificatum apparatum suum ipsum fert.
 * ================================================== */

interior vacuum
corpus_scribere(
    NaturaBibliotheca*   bib,
    constans character*  via)
{
    FILE*     f;
    i32       ie;

    f = fopen(via, "w");
    si (!f)
    {
        fprintf(stderr, "natura_examen: %s scribi nequit\n", via);
        redde;
    }

    xar_ordinare(bib->exemplaria, _comparare_exemplaria);

    per (ie = ZEPHYRUM; ie < xar_numerus(bib->exemplaria); ie++)
    {
        NaturaExemplar* ex;
        Piscina*        p;
        PiscinaNotatio  nota;
        Xar*            nodi;
        character       mod[CCLVI];
        i32             i;
        i32             ig;
        constans character* gradus_nomina[II];

        ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria, ie);
        p  = bib->piscina;
        nota = piscina_notare(p);
        _attr(ex->radix, "modulus", mod, (i32)magnitudo(mod));

        nodi = xar_creare(p, (i32)magnitudo(StmlNodus*));
        _colligere(ex->radix, "genus", nodi);

        per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
        {
            StmlNodus* g;
            character  gn[CCLVI];
            character  sb[CCLVI];
            character  sm[CCLVI];
            character  de[MM];
            character  di[MM];
            StmlNodus* n2;
            i32        ik;
            constans character* invol[IV];
            constans character* membra[IV];

            g = *(StmlNodus**)xar_obtinere(nodi, i);
            _attr(g, "nomen",   gn, (i32)magnitudo(gn));
            _attr(g, "sub",     sb, (i32)magnitudo(sb));
            _attr(g, "modulus", sm, (i32)magnitudo(sm));
            si (!sm[ZEPHYRUM])
            {
                memcpy(sm, mod, strlen(mod) + I);
            }
            si (!sb[ZEPHYRUM])
            {
                sm[ZEPHYRUM] = '\0';
            }

            de[ZEPHYRUM] = '\0';
            n2 = stml_invenire_liberum(g, "definitio");
            si (n2)
            {
                _prosa_ex(stml_textus_internus(n2, p), de,
                          (i32)magnitudo(de), FALSUM);
            }
            di[ZEPHYRUM] = '\0';
            n2 = stml_invenire_liberum(g, "differentia");
            si (n2)
            {
                _prosa_ex(stml_textus_internus(n2, p), di,
                          (i32)magnitudo(di), FALSUM);
            }

            fprintf(f, "G\t%s\t%s\t%s\t%s\t%s\t%s\n",
                    mod, gn, sm, sb, de, di);

            /* machina_statuum liberum DIRECTUM est; cetera in
             * involucris habitant */
            invol[ZEPHYRUM] = "proprietates";  membra[ZEPHYRUM] = "proprietas";
            invol[I]        = NIHIL;           membra[I]        = "machina_statuum";
            invol[II]       = "partes";        membra[II]       = "pars";
            invol[III]      = "relationes";    membra[III]      = "relatio";

            per (ik = ZEPHYRUM; ik < IV; ik++)
            {
                StmlNodus* sedes;
                i32        numerus_l;
                i32        j;

                sedes = invol[ik]
                        ? stml_invenire_liberum(g, invol[ik]) : g;
                si (!sedes)
                {
                    perge;
                }

                numerus_l = stml_numerus_liberorum(sedes);
                per (j = ZEPHYRUM; j < numerus_l; j++)
                {
                    StmlNodus* m;
                    character  n[CCLVI];

                    m = stml_liberum_ad_indicem(sedes, j);
                    si (!m || m->genus != STML_NODUS_ELEMENTUM ||
                        !chorda_aequalis_literis(*m->titulus,
                                                 membra[ik]))
                    {
                        perge;
                    }

                    _attr(m, "nomen", n, (i32)magnitudo(n));

                    si (ik == ZEPHYRUM)
                    {
                        character ty[CCLVI];
                        character tm[CCLVI];
                        character op[MM];
                        i32       k;

                        _attr(m, "genus",   ty, (i32)magnitudo(ty));
                        _attr(m, "modulus", tm, (i32)magnitudo(tm));

                        /* CIRCUMVENTIO SUBLATA (2026-08-06):
                         * optiones hic manu iungebantur quia stml
                         * nodos spatii albi INTER elementa
                         * abiciebat, unde 'disciplinastructura...'.
                         * Vitium in bibliotheca emendatum est
                         * (normalizatio a parsatione ad lectionem
                         * migravit), ergo textus internus rursus
                         * verbatim est et _prosa_ex sufficit. */
                        _prosa_ex(stml_textus_internus(m, p), op,
                                  (i32)magnitudo(op), FALSUM);
                        /* sed 's/ $//' */
                        k = (i32)strlen(op);
                        dum (k > ZEPHYRUM && op[k - I] == ' ')
                        {
                            op[--k] = '\0';
                        }
                        fprintf(f, "P\t%s\t%s\t%s\t%s\t%s\t%s\n",
                                mod, gn, n, ty, tm, op);
                    }
                    alioquin si (ik == I)
                    {
                        character st[MM];
                        i32       numerus_s;
                        i32       k;
                        i32       pos;

                        st[ZEPHYRUM] = '\0';
                        pos = ZEPHYRUM;
                        numerus_s = stml_numerus_liberorum(m);
                        per (k = ZEPHYRUM; k < numerus_s; k++)
                        {
                            StmlNodus* s;
                            character  sn[CCLVI];

                            s = stml_liberum_ad_indicem(m, k);
                            si (!s ||
                                s->genus != STML_NODUS_ELEMENTUM ||
                                !chorda_aequalis_literis(
                                    *s->titulus, "status"))
                            {
                                perge;
                            }
                            _attr(s, "nomen", sn,
                                  (i32)magnitudo(sn));
                            si (!sn[ZEPHYRUM])
                            {
                                perge;
                            }
                            si (pos > ZEPHYRUM)
                            {
                                pos += (i32)sprintf(st + pos, ", ");
                            }
                            pos += (i32)sprintf(st + pos, "%s", sn);
                        }
                        fprintf(f, "M\t%s\t%s\t%s\t%s\n",
                                mod, gn, n, st);
                    }
                    alioquin si (ik == II)
                    {
                        character ne[CCLVI];

                        _attr(m, "necessaria", ne,
                              (i32)magnitudo(ne));
                        fprintf(f, "A\t%s\t%s\t%s\t%s\n",
                                mod, gn, n, ne);
                    }
                    alioquin
                    {
                        character ad[CCLVI];
                        character rm[CCLVI];

                        si (!n[ZEPHYRUM])
                        {
                            perge;
                        }
                        _attr(m, "ad",      ad, (i32)magnitudo(ad));
                        _attr(m, "modulus", rm, (i32)magnitudo(rm));
                        si (!rm[ZEPHYRUM])
                        {
                            memcpy(rm, mod, strlen(mod) + I);
                        }
                        fprintf(f, "R\t%s\t%s\t%s\t%s\t%s\n",
                                mod, gn, n, rm, ad);
                    }
                }
            }
        }

        /* species et individua (cultivar NON - quaere ea omittit) */
        gradus_nomina[ZEPHYRUM] = "species";
        gradus_nomina[I]        = "individuum";
        per (ig = ZEPHYRUM; ig < II; ig++)
        {
            Xar* rr;

            rr = xar_creare(p, (i32)magnitudo(StmlNodus*));
            _colligere(ex->radix, gradus_nomina[ig], rr);
            per (i = ZEPHYRUM; i < xar_numerus(rr); i++)
            {
                StmlNodus* r;
                StmlNodus* maior;
                StmlNodus* def;
                character  rn[CCLVI];
                character  rg[CCLVI];
                character  rd[MM];

                r = *(StmlNodus**)xar_obtinere(rr, i);
                _attr(r, "nomen", rn, (i32)magnitudo(rn));
                si (!rn[ZEPHYRUM])
                {
                    perge;
                }

                rg[ZEPHYRUM] = '\0';
                per (maior = r->parens; maior; maior = maior->parens)
                {
                    si (maior->genus == STML_NODUS_ELEMENTUM &&
                        chorda_aequalis_literis(*maior->titulus,
                                                "genus"))
                    {
                        _attr(maior, "nomen", rg,
                              (i32)magnitudo(rg));
                        frange;
                    }
                }

                rd[ZEPHYRUM] = '\0';
                def = stml_invenire_liberum(r, "definitio");
                si (def)
                {
                    _prosa_ex(stml_textus_internus(def, p), rd,
                              (i32)magnitudo(rd), FALSUM);
                }

                fprintf(f, "S\t%s\t%s\t%s\t%s\t%s\n",
                        mod, rg, gradus_nomina[ig], rn, rd);
            }
        }

        /* umbrae */
        {
            Xar* omnia;

            omnia = xar_creare(p, (i32)magnitudo(StmlNodus*));
            _colligere_omnia_elementa(ex->radix, omnia);
            per (i = ZEPHYRUM; i < xar_numerus(omnia); i++)
            {
                StmlNodus* n;
                chorda*    v;

                n = *(StmlNodus**)xar_obtinere(omnia, i);
                v = stml_attributum_capere(n, "externum");
                si (v && chorda_aequalis_literis(*v, "verum"))
                {
                    character u[CCLVI];

                    _attr(n, "ad", u, (i32)magnitudo(u));
                    si (u[ZEPHYRUM])
                    {
                        fprintf(f, "U\t%s\t%s\n", mod, u);
                    }
                }
            }
        }

        piscina_reficere(p, nota);
    }

    fclose(f);
}

s32
principale(
    s32          numerus,
    character**  argumenta)
{
    Piscina*              piscina;
    NaturaBibliotheca*    bib;
    DirectoriumIterator*  iter;
    DirectoriumIntroitus* introitus;
    constans character*   radix;
    constans character*   plagula;
    constans character*   tabulae;
    constans character*   corpus;
    b32                   machina;
    s32                   i;
    i32                   onerata;
    i32                   vulnera;
    i32                   monita;
    i32                   ostensa;
    i32                   j;

    radix   = "natura";
    plagula = NIHIL;
    tabulae = NIHIL;
    corpus  = NIHIL;
    machina = FALSUM;

    per (i = I; i < numerus; i++)
    {
        si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-radix") == ZEPHYRUM &&
                     i + I < numerus)
        {
            radix = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-plagula") == ZEPHYRUM &&
                     i + I < numerus)
        {
            plagula = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-tabulae") == ZEPHYRUM &&
                     i + I < numerus)
        {
            tabulae = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-corpus") == ZEPHYRUM &&
                     i + I < numerus)
        {
            corpus = argumenta[++i];
        }
        alioquin
        {
            fprintf(stderr,
                "usus: natura_examen [-plagula VIA] [-machina] "
                "[-radix DIR] [-tabulae DIR] [-corpus VIA]\n");
            redde II;
        }
    }

    piscina = piscina_generare_dynamicum("natura_examen", 1048576);
    bib     = natura_bibliotheca_creare(piscina);
    si (!bib)
    {
        fprintf(stderr, "natura_examen: bibliotheca creari nequit\n");
        redde II;
    }

    iter = directorium_iterator_aperire(radix, piscina);
    si (!iter)
    {
        fprintf(stderr, "natura_examen: '%s' aperiri nequit\n",
                radix);
        redde II;
    }

    onerata = ZEPHYRUM;
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        character via[DXII];
        character stirps[CCLVI];
        chorda    fons;

        si (introitus->genus != INTROITUS_FILUM ||
            !extensionem_habet(&introitus->titulus))
        {
            perge;
        }

        si ((size_t)introitus->titulus.mensura + strlen(radix) + II >
            magnitudo(via))
        {
            perge;
        }

        sprintf(via, "%s/%.*s", radix,
                (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        stirpem_scribere(&introitus->titulus, stirps,
                         (i32)magnitudo(stirps));

        fons = filum_legere_totum(via, piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "natura_examen: '%s' legi nequit\n", via);
            perge;
        }

        natura_legere(bib, fons, stirps);
        onerata++;
    }
    directorium_iterator_claudere(iter);

    /* NIHIL CURSUM EST - non successus */
    si (onerata == ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_examen: NULLUM exemplar in '%s' inventum "
            "(extensio '%s')\n", radix, EXTENSIO);
        redde II;
    }

    vulnera = natura_nectere(bib);

    si (tabulae)
    {
        tabulas_scribere(bib, tabulae);
    }
    si (corpus)
    {
        corpus_scribere(bib, corpus);
    }

    /* filtrum POST nexuram: regulae trans exemplaria transeunt */
    monita  = ZEPHYRUM;
    ostensa = ZEPHYRUM;
    per (j = ZEPHYRUM; j < xar_numerus(bib->diagnostica); j++)
    {
        NaturaDiagnosticum* d;
        constans character* gradus;

        d = (NaturaDiagnosticum*)xar_obtinere(bib->diagnostica, j);
        si (d->gradus == NATURA_GRADUS_MONITUM)
        {
            monita++;
        }

        si (plagula && d->modulus &&
            !viam_congruere(d->modulus, plagula))
        {
            perge;
        }
        ostensa++;

        gradus = (d->gradus == NATURA_GRADUS_VULNUS) ?
                 "VULNUS" : "MONITUM";

        si (machina)
        {
            imprimere("%s\t%u\t%.*s\t%.*s\t%s\n", gradus, d->regula,
                d->modulus ? (integer)d->modulus->mensura : 1,
                d->modulus ?
                    (constans character*)d->modulus->datum : "-",
                d->ens ? (integer)d->ens->mensura : 1,
                d->ens ? (constans character*)d->ens->datum : "-",
                d->nuntius);
        }
        alioquin
        {
            imprimere("%-7s regula %2u  [%.*s] %.*s: %s\n", gradus,
                d->regula,
                d->modulus ? (integer)d->modulus->mensura : 1,
                d->modulus ?
                    (constans character*)d->modulus->datum : "-",
                d->ens ? (integer)d->ens->mensura : 1,
                d->ens ? (constans character*)d->ens->datum : "-",
                d->nuntius);
        }
    }

    si (!machina)
    {
        si (ostensa == ZEPHYRUM)
        {
            imprimere("natura_examen: SANUM");
            si (plagula)
            {
                imprimere(" (%s)", plagula);
            }
            imprimere("\n");
        }
        imprimere("  exemplaria %u / genera %u / res %u"
                  " / VULNERA %u / MONITA %u\n",
                  onerata, xar_numerus(bib->genera_omnia),
                  xar_numerus(bib->res_omnes), vulnera, monita);
    }

    redde vulnera > ZEPHYRUM ? I : ZEPHYRUM;
}
