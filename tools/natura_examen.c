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
#include "canon.h"
#include "filum.h"
#include "iter_directoria.h"
#include <stdio.h>
#include <string.h>

#define EXTENSIO       ".genera"
#define EXTENSIO_LONGA VII

interior b32
extensionem_habet (
    constans chorda* titulus);
interior vacuum
tabulas_scribere (
                                  NaturaBibliotheca* bib,
                                 constans character* dir);
interior vacuum
corpus_scribere (
                                 NaturaBibliotheca* bib,
                                constans character* via);
interior vacuum
stirpem_scribere (
                           constans chorda* titulus,
                                 character* exitus,
                                       i32  tectum);
interior b32
viam_congruere (
                               constans chorda* modulus,
                            constans character* plagula);

/* an titulus in ".genera" desinat */
interior b32
extensionem_habet (
    constans chorda* titulus)
{
    si (titulus->mensura <= EXTENSIO_LONGA)
    {
        redde FALSUM;
    }

    redde (b32)(memcmp(titulus->datum
        + (titulus->mensura - EXTENSIO_LONGA),
                       EXTENSIO,
                       (memoriae_index)EXTENSIO_LONGA) == ZEPHYRUM);
}

/* titulus sine extensione - regula XIV eam contra attributum
 * modulus iudicat */
interior vacuum
stirpem_scribere (
    constans chorda* titulus,
          character* exitus,
                i32  tectum)
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
viam_congruere (
       constans chorda* modulus,
    constans character* plagula)
{
    constans character* basis;
    constans character* punctum;
                size_t  mensura;

    basis    = strrchr(plagula, '/');
    basis    = basis ? basis + I : plagula;
    punctum  = strrchr(basis, '.');
    mensura  = punctum ? (size_t)(punctum - basis) : strlen(basis);

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
_colligere (
             StmlNodus* nodus,
    constans character* titulus,
                   Xar* fructus)
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

            locus   = (StmlNodus**)xar_addere(fructus);
            *locus  = liberum;
        }
        _colligere(liberum, titulus, fructus);
    }
}

/* omnia elementa, ordine documenti - pro attributis quae in
 * elemento quolibet sedere possunt (fons, certitudo, valens_*,
 * externum) */
interior vacuum
_colligere_omnia_elementa (
    StmlNodus* nodus,
          Xar* fructus)
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

        locus   = (StmlNodus**)xar_addere(fructus);
        *locus  = liberum;
        _colligere_omnia_elementa(liberum, fructus);
    }
}

/* titulus elementi ordo generum est? (.genus/.species/.cultivar -
 * arbor porphyriana: munera tria, genus unum) */
interior b32
_kind_titulus_est (
    constans chorda* titulus)
{
    redde chorda_aequalis_literis(*titulus, ".genus")
        || chorda_aequalis_literis(*titulus, ".species")
        || chorda_aequalis_literis(*titulus, ".cultivar");
}

/* liberos DIRECTOS numerare */
interior i32
_numerare_liberos (
             StmlNodus* nodus,
    constans character* titulus)
{
    i32 numerus;
    i32 i;
    i32 summa;

    summa    = ZEPHYRUM;
    numerus  = stml_numerus_liberorum(nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liberum;

        liberum = stml_liberum_ad_indicem(nodus, i);
        si (   liberum && liberum->genus == STML_NODUS_ELEMENTUM
            && chorda_aequalis_literis(*liberum->titulus, titulus))
        {
            summa++;
        }
    }

    redde summa;
}

/* attributum ut literae (buffer vocantis); vacuum si abest */
interior vacuum
_attr (
             StmlNodus* nodus,
    constans character* titulus,
             character* exitus,
                   i32  tectum)
{
    chorda* v;

    exitus[ZEPHYRUM]  = '\0';
    v                 = stml_attributum_capere(nodus, titulus);
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
_prosa_ex (
       chorda  textus,
    character* exitus,
          i32  tectum,
          b32  virgulam_mutare)
{
    i32 i;
    i32 j;
    b32 spatium_prius;

    j              = ZEPHYRUM;
    spatium_prius  = VERUM;   /* spatium primum abscindit */

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
_prosa (
       chorda  textus,
    character* exitus,
          i32  tectum)
{
    _prosa_ex(textus, exitus, tectum, VERUM);
}

/* ordo alphabeticus exemplarium - glob conchae eum praestat, et
 * tabulae conferri debent */
interior s32
_comparare_exemplaria (
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
tabulas_scribere (
     NaturaBibliotheca* bib,
    constans character* dir)
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
    FILE*     f_necessitudines;
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
    APERIRE(f_necessitudines, "necessitudines.txt")
#undef APERIRE

    xar_ordinare(bib->exemplaria, _comparare_exemplaria);

    per (ie = ZEPHYRUM; ie < xar_numerus(bib->exemplaria); ie++)
    {
        NaturaExemplar* ex;
               Piscina* p;
        PiscinaNotatio  nota;
                   Xar* nodi;
                   i32  i;
             character  mod[CCLVI];

        ex    = *(NaturaExemplar**)xar_obtinere(bib->exemplaria, ie);
        p     = bib->piscina;
        nota  = piscina_notare(p);

        _attr(ex->radix, "modulus", mod, (i32)magnitudo(mod));
        {
            character ver[LXIV];

            _attr(ex->radix, "versio", ver, (i32)magnitudo(ver));
            fprintf(f_moduli, "%s:%s\n", mod, ver);
        }

        /* ---- necessitudines (declarationes) ---- */
        nodi = xar_creare(p, (i32)magnitudo(StmlNodus*));
        _colligere(ex->radix, "necessitudo", nodi);
        per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
        {
            StmlNodus* nx;
            character  nn[CCLVI];
            character  cv[CCLVI];
            character  sb[CCLVI];
            character  fa[CCLVI];
            character  fad[CCLVI];
            character  ca[CCLVI];
            character  scr[DXII];
                  i32  j;
                  i32  n_l;
                  i32  pos;

            nx = *(StmlNodus**)xar_obtinere(nodi, i);
            _attr(nx, "nomen",     nn,  (i32)magnitudo(nn));
            _attr(nx, "conversum", cv,  (i32)magnitudo(cv));
            _attr(nx, "sub",       sb,  (i32)magnitudo(sb));
            _attr(nx, "a",         fa,  (i32)magnitudo(fa));
            _attr(nx, "ad",        fad, (i32)magnitudo(fad));
            _attr(nx, "causa",     ca,  (i32)magnitudo(ca));

            scr[ZEPHYRUM]  = '\0';
            pos            = ZEPHYRUM;
            n_l            = stml_numerus_liberorum(nx);
            per (j = ZEPHYRUM; j < n_l; j++)
            {
                StmlNodus* s;
                character  unum[CCLVI];

                s = stml_liberum_ad_indicem(nx, j);
                si (   !s || s->genus != STML_NODUS_ELEMENTUM
                    || !chorda_aequalis_literis(*s->titulus,
                                             "scriptio"))
                {
                    perge;
                }
                _prosa_ex(stml_textus_internus(s, p), unum,
                          (i32)magnitudo(unum), FALSUM);
                si (   !unum[ZEPHYRUM]
                    || pos + (i32)strlen(unum) + II
                    > (i32)magnitudo(scr))
                {
                    perge;
                }
                si (pos > ZEPHYRUM)
                {
                    pos += (i32)sprintf(scr + pos, " ");
                }
                pos += (i32)sprintf(scr + pos, "%s", unum);
            }

            /* columna VIII causa= APPENSA (stratum causale
             * 2026-08-11) - lectores priores columnas I-VII
             * intactas vident */
            fprintf(f_necessitudines, "%s|%s|%s|%s|%s|%s|%s|%s\n",
                    mod, nn, cv, sb, fa, fad, scr, ca);
        }

        /* ---- genera (arbor porphyriana: .genus/.species/.cultivar
         * ordines OMNES generum sunt; munus = columna IX; species
         * parentem e nidificatione in columna sub ferunt) ---- */
        {
            constans character* kind_nomina[III];
            constans character* kind_munera[III];
                           i32  ik;

            kind_nomina[ZEPHYRUM]  = ".genus";
            kind_nomina[I]         = ".species";
            kind_nomina[II]        = ".cultivar";
            kind_munera[ZEPHYRUM]  = "genus";
            kind_munera[I]         = "species";
            kind_munera[II]        = "cultivar";

            per (ik = ZEPHYRUM; ik < III; ik++)
            {
            nodi = xar_creare(p, (i32)magnitudo(StmlNodus*));
            _colligere(ex->radix, kind_nomina[ik], nodi);
            per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
            {
                StmlNodus* g;
                character  gn[CCLVI];
                character  sub[CCLVI];
                character  submod[CCLVI];
                character  glossa[DXII];
                StmlNodus* def;
                      Xar* dubia_sub;
                      i32  npa;
                StmlNodus* partes;

                g = *(StmlNodus**)xar_obtinere(nodi, i);
                _attr(g, "nomen",   gn,     (i32)magnitudo(gn));
                _attr(g, "sub",     sub,    (i32)magnitudo(sub));
                _attr(g, "modulus", submod, (i32)magnitudo(submod));

                /* subordinatio TRANS EXEMPLARIA: arcus, et radix in
                 * silva sua (mechanismus rotae XVII) */
                si (   sub[ZEPHYRUM] && submod[ZEPHYRUM]
                    && strcmp(submod, mod) != ZEPHYRUM)
                {
                    fprintf(f_arcus, "%s|%s|sub|%s|%s|%s\n",
                            mod, gn, submod, sub, gn);
                    fprintf(f_transradices, "%s|%s|%s.%s\n",
                            mod, gn, submod, sub);
                    sub[ZEPHYRUM] = '\0';
                }

                /* species/cultivar: parens e nidificatione (sub=
                 * eis vetitum est) - columna sub impletur ut
                 * tabula PARENS visus genera omnia tegat */
                si (!sub[ZEPHYRUM] && ik > ZEPHYRUM)
                {
                    StmlNodus* maior;

                    per (maior = g->parens; maior;
                         maior = maior->parens)
                    {
                        si (   maior->genus == STML_NODUS_ELEMENTUM
                            && _kind_titulus_est(maior->titulus))
                        {
                            _attr(maior, "nomen", sub,
                                  (i32)magnitudo(sub));
                            frange;
                        }
                    }
                }

                dubia_sub = xar_creare(p, (i32)magnitudo(StmlNodus*));
                _colligere(g, "dubium", dubia_sub);

                partes = stml_invenire_liberum(g, "partes");
                npa = partes ? _numerare_liberos(partes, "pars")
                             : ZEPHYRUM;

                fprintf(f_genera, "%s|%s|%s|%u|%u|%u|%u|%u|%s\n",
                        mod, gn, sub,
                        _numerare_liberos(g, ".species"),
                        _numerare_liberos(g, "individuum"),
                        xar_numerus(dubia_sub),
                        _numerare_liberos(g, "machina_statuum"),
                        npa,
                        kind_munera[ik]);

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
                          i32 k;

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
            }
        }

        /* ---- res dictionarii = INDIVIDUA SOLA (arbor
         * porphyriana; maior = genus proximus CUIUSVIS ordinis) */
        {
            Xar* rr;

            rr = xar_creare(p, (i32)magnitudo(StmlNodus*));
            _colligere(ex->radix, "individuum", rr);
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
                    si (   maior->genus == STML_NODUS_ELEMENTUM
                        && _kind_titulus_est(maior->titulus))
                    {
                        _attr(maior, "nomen", rg,
                              (i32)magnitudo(rg));
                        frange;
                    }
                }

                fprintf(f_res, "%s|%s\n", mod, rn);
                fprintf(f_resgen, "%s|%s|:%s\n", mod, rg, rn);
            }
        }

        /* ---- arcus: relatio/relatum/terminus ---- */
        {
            constans character* elementa[III];
            constans character* attributa[II];
                           i32  ielem;
                           i32  iattr;

            elementa[ZEPHYRUM]   = "relatio";
            elementa[I]          = "relatum";
            elementa[II]         = "terminus";
            attributa[ZEPHYRUM]  = "ad";
            attributa[I]         = "a";

            per (ielem = ZEPHYRUM; ielem < III; ielem++)
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
                        si (   !tgt[ZEPHYRUM]
                            || strcmp(tgt, "*") == ZEPHYRUM
                            || stml_attributum_capere(r, "externum"))
                        {
                            perge;
                        }

                        /* terminus munere nominatur, non nomine */
                        _attr(r, ielem == II ? "munus" : "nomen",
                              rel, (i32)magnitudo(rel));
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
                            si (   maior->genus
                                == STML_NODUS_ELEMENTUM
                                && _kind_titulus_est(maior->titulus))
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
                            si (   maior->genus
                                == STML_NODUS_ELEMENTUM
                                && stml_attributum_capere(maior,
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
                si (   maior->genus == STML_NODUS_ELEMENTUM
                    && _kind_titulus_est(maior->titulus))
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
                   chorda* v;

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

                si (   stml_attributum_capere(n, "valens_a")
                    || stml_attributum_capere(n, "valens_ad"))
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
                si (   maior->genus == STML_NODUS_ELEMENTUM
                    && stml_attributum_capere(maior, "nomen"))
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
    fclose(f_moduli);       fclose(f_necessitudines);
}


/* ==================================================
 * Corpus quaerentis (gradus II) - forma TSV, ordines
 * G/P/M/A/R/S/U quos natura_quaere.sh consumit.
 *
 * P/M/A/R e CONTINENTIBUS DIRECTIS generis sumuntur (non e
 * posteris): genus nidificatum apparatum suum ipsum fert.
 * ================================================== */

interior vacuum
corpus_scribere (
     NaturaBibliotheca* bib,
    constans character* via)
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
               Piscina* p;
        PiscinaNotatio  nota;
                   Xar* nodi;
             character  mod[CCLVI];
                   i32  i;

        ex    = *(NaturaExemplar**)xar_obtinere(bib->exemplaria, ie);
        p     = bib->piscina;
        nota  = piscina_notare(p);
        _attr(ex->radix, "modulus", mod, (i32)magnitudo(mod));

        nodi = xar_creare(p, (i32)magnitudo(StmlNodus*));
        /* ordines generum OMNES (arbor porphyriana) - umbra
         * apparatus inde genera CDLX tegit */
        _colligere(ex->radix, ".genus", nodi);
        _colligere(ex->radix, ".species", nodi);
        _colligere(ex->radix, ".cultivar", nodi);

        per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
        {
                     StmlNodus* g;
                     character  gn[CCLVI];
                     character  sb[CCLVI];
                     character  sm[CCLVI];
                     character  de[MM];
                     character  di[MM];
                     StmlNodus* n2;
                           i32  ik;
            constans character* invol[V];
            constans character* membra[V];

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

            de[ZEPHYRUM]  = '\0';
            n2            = stml_invenire_liberum(g, "definitio");
            si (n2)
            {
                _prosa_ex(stml_textus_internus(n2, p), de,
                          (i32)magnitudo(de), FALSUM);
            }
            di[ZEPHYRUM]  = '\0';
            n2            = stml_invenire_liberum(g, "differentia");
            si (n2)
            {
                _prosa_ex(stml_textus_internus(n2, p), di,
                          (i32)magnitudo(di), FALSUM);
            }

            fprintf(f, "G\t%s\t%s\t%s\t%s\t%s\t%s\n",
                    mod, gn, sm, sb, de, di);

            /* machina_statuum liberum DIRECTUM est; cetera in
             * involucris habitant */
            invol[ZEPHYRUM] = "proprietates";  membra[ZEPHYRUM] =
                                                   "proprietas";
            invol[I]        = NIHIL;           membra[I]        =
                                                   "machina_statuum";
            invol[II]       = "partes";        membra[II]       =
                                                   "pars";
            invol[III]      = "relationes";    membra[III]      =
                                                   "relatio";
            invol[IV]       = "termini";       membra[IV]       =
                                                   "terminus";

            per (ik = ZEPHYRUM; ik < V; ik++)
            {
                StmlNodus* sedes;
                      i32  numerus_l;
                      i32  j;

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
                    si (   !m || m->genus != STML_NODUS_ELEMENTUM
                        || !chorda_aequalis_literis(*m->titulus,
                                                 membra[ik]))
                    {
                        perge;
                    }

                    /* terminus munere nominatur, non nomine */
                    _attr(m, ik == IV ? "munus" : "nomen", n,
                          (i32)magnitudo(n));

                    si (ik == ZEPHYRUM)
                    {
                        character ty[CCLVI];
                        character tm[CCLVI];
                        character op[MM];
                              i32 k;

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
                              i32 numerus_s;
                              i32 k;
                              i32 pos;

                        st[ZEPHYRUM]  = '\0';
                        pos           = ZEPHYRUM;
                        numerus_s     = stml_numerus_liberorum(m);
                        per (k = ZEPHYRUM; k < numerus_s; k++)
                        {
                            StmlNodus* s;
                            character  sn[CCLVI];

                            s = stml_liberum_ad_indicem(m, k);
                            si (   !s
                                || s->genus != STML_NODUS_ELEMENTUM
                                || !chorda_aequalis_literis(
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
                        fprintf(f, "%s\t%s\t%s\t%s\t%s\t%s\n",
                                ik == IV ? "T" : "R",
                                mod, gn, n, rm, ad);
                    }
                }
            }
        }

        /* individua sola (arbor porphyriana: species in flumine G
         * supra; maior = genus proximus CUIUSVIS ordinis) */
        {
            Xar* rr;

            rr = xar_creare(p, (i32)magnitudo(StmlNodus*));
            _colligere(ex->radix, "individuum", rr);
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
                    si (   maior->genus == STML_NODUS_ELEMENTUM
                        && _kind_titulus_est(maior->titulus))
                    {
                        _attr(maior, "nomen", rg,
                              (i32)magnitudo(rg));
                        frange;
                    }
                }

                rd[ZEPHYRUM]  = '\0';
                def           = stml_invenire_liberum(r, "definitio");
                si (def)
                {
                    _prosa_ex(stml_textus_internus(def, p), rd,
                              (i32)magnitudo(rd), FALSUM);
                }

                fprintf(f, "S\t%s\t%s\tindividuum\t%s\t%s\n",
                        mod, rg, rn, rd);
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
                   chorda* v;

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

/* relatio sedium unius familiae: quis dependet, qua directione,
 * quo verdicto - sedes probatae ET praetereuntes (silentium non
 * fides est). Sub -angustatio eadem relatio hypothesi servit. */
interior vacuum
sedes_referre (
     NaturaBibliotheca* bib,
     NaturaNecessitudo* decl,
    constans character* familia,
                   b32  machina)
{
    chorda* finis_a;
    chorda* finis_ad;
       i32  i;
       i32  summa_s;
       i32  conversae;
       i32  intra_n;
       i32  excedit_n;
       i32  aperta_n;

    finis_a   = natura_finem_effectivum(decl, "a");
    finis_ad  = natura_finem_effectivum(decl, "ad");

    si (!machina)
    {
        imprimere("\nsedes familiae '%s'", familia);
        si (decl->conversum)
        {
            imprimere(" (conversum '%.*s')",
                (integer)decl->conversum->mensura,
                (constans character*)decl->conversum->datum);
        }
        imprimere("  a=%.*s ad=%.*s\n",
            finis_a ? (integer)finis_a->mensura : I,
            finis_a ? (constans character*)finis_a->datum : "*",
            finis_ad ? (integer)finis_ad->mensura : I,
            finis_ad ? (constans character*)finis_ad->datum : "*");
    }

    summa_s    = ZEPHYRUM;
    conversae  = ZEPHYRUM;
    intra_n    = ZEPHYRUM;
    excedit_n  = ZEPHYRUM;
    aperta_n   = ZEPHYRUM;

    per (i = ZEPHYRUM; i < xar_numerus(bib->sedes_ligatae); i++)
    {
         NaturaSedesLigata* s;
        constans character* directio;
        constans character* verdictum;

        s = (NaturaSedesLigata*)xar_obtinere(bib->sedes_ligatae, i);
        si (s->ligata != decl)
        {
            perge;
        }
        summa_s++;
        si (s->conversa)
        {
            conversae++;
        }
        commutatio (s->verdictum)
        {
            casus NATURA_SEDES_INTRA:
                verdictum = "INTRA";   intra_n++;   frange;
            casus NATURA_SEDES_EXCEDIT:
                verdictum = "EXCEDIT"; excedit_n++; frange;
            ordinarius:
                verdictum = "APERTA";  aperta_n++;  frange;
        }
        directio = s->conversa ? "conversa" : "recta";

        si (machina)
        {
            imprimere("SEDES\t%s\t%.*s\t%.*s\t%.*s\t%.*s\t%s\t%.*s\t%s\n",
                familia,
                (integer)s->exemplar->stirps->mensura,
                (constans character*)s->exemplar->stirps->datum,
                s->possessor ?
                    (integer)s->possessor->titulus->mensura : I,
                s->possessor ?
                    (constans character*)s->possessor->titulus->datum
                    : "-",
                s->titulus ? (integer)s->titulus->mensura : I,
                s->titulus ?
                    (constans character*)s->titulus->datum : "-",
                (integer)s->nodus->titulus->mensura,
                (constans character*)s->nodus->titulus->datum,
                directio,
                s->ad_attr ? (integer)s->ad_attr->mensura : I,
                s->ad_attr ?
                    (constans character*)s->ad_attr->datum : "-",
                verdictum);
        }
        alioquin
        {
            imprimere("  %-16.*s %-22.*s %-20.*s %-8s %-20.*s %s\n",
                (integer)s->exemplar->stirps->mensura,
                (constans character*)s->exemplar->stirps->datum,
                s->possessor ?
                    (integer)s->possessor->titulus->mensura : I,
                s->possessor ?
                    (constans character*)s->possessor->titulus->datum
                    : "-",
                s->titulus ? (integer)s->titulus->mensura : I,
                s->titulus ?
                    (constans character*)s->titulus->datum : "-",
                directio,
                s->ad_attr ? (integer)s->ad_attr->mensura : I,
                s->ad_attr ?
                    (constans character*)s->ad_attr->datum : "-",
                verdictum);

            /* excedenti catenam scopi monstrare - iudicium
             * humanum materia sua eget */
            si (s->verdictum == NATURA_SEDES_EXCEDIT && s->ad_attr)
            {
                     chorda* modulus_attr;
                NaturaGenus* g;

                modulus_attr = stml_attributum_capere(s->nodus,
                                                      "modulus");
                g = natura_genus_in(bib,
                    modulus_attr ?
                        chorda_ut_cstr(*modulus_attr, bib->piscina)
                        : chorda_ut_cstr(*s->exemplar->stirps,
                                         bib->piscina),
                    chorda_ut_cstr(*s->ad_attr, bib->piscina));
                si (g)
                {
                    imprimere("      catena scopi:");
                    dum (g)
                    {
                        imprimere(" %.*s",
                            (integer)g->titulus->mensura,
                            (constans character*)g->titulus->datum);
                        g = g->parens;
                        si (g)
                        {
                            imprimere(" <");
                        }
                    }
                    imprimere("\n");
                }
            }
        }
    }

    si (!machina)
    {
        imprimere("  summa: sedes %u (rectae %u / conversae %u)"
                  " - INTRA %u / EXCEDIT %u / APERTA %u\n",
                  summa_s, summa_s - conversae, conversae,
                  intra_n, excedit_n, aperta_n);
    }
}

s32
principale (
          s32   numerus,
    character** argumenta)
{
                 Piscina* piscina;
       NaturaBibliotheca* bib;
     DirectoriumIterator* iter;
    DirectoriumIntroitus* introitus;
      constans character* radix;
      constans character* plagula;
      constans character* tabulae;
      constans character* corpus;
      constans character* familia_sedes;
      constans character* angustatio_par;
                     b32  machina;
                     s32  i;
                     i32  onerata;
                     i32  vulnera;
                     i32  monita;
                     i32  ostensa;
                     i32  j;

    radix           = "natura";
    plagula         = NIHIL;
    tabulae         = NIHIL;
    corpus          = NIHIL;
    machina         = FALSUM;
    familia_sedes   = NIHIL;
    angustatio_par  = NIHIL;

    per (i = I; i < numerus; i++)
    {
        si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (   strcmp(argumenta[i], "-sedes") == ZEPHYRUM
                     && i + I < numerus)
        {
            familia_sedes = argumenta[++i];
        }
        alioquin si (   strcmp(argumenta[i], "-angustatio") == ZEPHYRUM
                     && i + II < numerus)
        {
            familia_sedes   = argumenta[++i];
            angustatio_par  = argumenta[++i];
        }
        alioquin si (   strcmp(argumenta[i], "-radix") == ZEPHYRUM
                     && i + I < numerus)
        {
            radix = argumenta[++i];
        }
        alioquin si (   strcmp(argumenta[i], "-plagula") == ZEPHYRUM
                     && i + I < numerus)
        {
            plagula = argumenta[++i];
        }
        alioquin si (   strcmp(argumenta[i], "-tabulae") == ZEPHYRUM
                     && i + I < numerus)
        {
            tabulae = argumenta[++i];
        }
        alioquin si (   strcmp(argumenta[i], "-corpus") == ZEPHYRUM
                     && i + I < numerus)
        {
            corpus = argumenta[++i];
        }
        alioquin
        {
            fprintf(stderr,
                "usus: natura_examen [-plagula VIA] [-machina] "
                "[-radix DIR] [-tabulae DIR] [-corpus VIA]\n"
                "      [-sedes FAMILIA] "
                "[-angustatio FAMILIA a|ad=GENUS]\n");
            redde II;
        }
    }

    piscina  = piscina_generare_dynamicum("natura_examen", 1048576);
    bib      = natura_bibliotheca_creare(piscina);
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
           chorda fons;

        si (   introitus->genus != INTROITUS_FILUM
            || !extensionem_habet(&introitus->titulus))
        {
            perge;
        }

        si ((size_t)introitus->titulus.mensura + strlen(radix) + II
            > magnitudo(via))
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

    /* superpositio finis ANTE nexuram: hypothesis in memoria
     * sola, plagulae intactae; regulae omnes sub ea currunt */
    si (angustatio_par)
    {
                 character  finis_b[IV];
        constans character* aequale;
                    size_t  mensura_f;

        aequale = strchr(angustatio_par, '=');
        si (   !aequale || aequale == angustatio_par
            || *(aequale + I) == '\0')
        {
            fprintf(stderr,
                "natura_examen: -angustatio postulat a|ad=GENUS"
                " (datum '%s') - NIHIL probatum\n", angustatio_par);
            redde II;
        }
        mensura_f = (size_t)(aequale - angustatio_par);
        si (mensura_f >= magnitudo(finis_b))
        {
            fprintf(stderr,
                "natura_examen: finis '%s' ignotus (a aut ad)"
                " - NIHIL probatum\n", angustatio_par);
            redde II;
        }
        memcpy(finis_b, angustatio_par, mensura_f);
        finis_b[mensura_f] = '\0';

        si (!natura_finem_superponere(bib, familia_sedes, finis_b,
                                      aequale + I))
        {
            fprintf(stderr,
                "natura_examen: superpositio recusata (familia"
                " '%s' ignota aut finis '%s' pravus) - NIHIL"
                " probatum\n", familia_sedes, finis_b);
            redde II;
        }
        si (!machina)
        {
            imprimere("HYPOTHESIS: %s %s -> %s (plagulae"
                      " intactae)\n", familia_sedes, finis_b,
                      aequale + I);
        }
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

    monita   = ZEPHYRUM;
    ostensa  = ZEPHYRUM;

    /* ---- gradus II: canon (regula VIII MIGRATA 2026-08-06) ----
     * Vocabularium clausum natura/natura.canon solum dicit; via
     * per canones.registrum solvitur ne ligamen bis stet. Canon
     * absens = VULNUS clamans, non silentium (gradus II mortuus
     * sanus non videtur). Regulae migratae: V/VIII/XII/XV et
     * dimidium formae VII - canon eas dicit, hic praesentantur
     * numeris historicis. */
    {
        chorda  catalogus;
        chorda  via_canonis;
         Canon* canon;

        canon      = NIHIL;
        catalogus  = filum_legere_totum("canones.registrum", piscina);
        via_canonis = canon_registrum_quaerere(catalogus,
                                               "x.genera", piscina);
        si (via_canonis.mensura > ZEPHYRUM)
        {
            chorda fons_canonis;
            chorda causa;

            fons_canonis = filum_legere_totum(
                chorda_ut_cstr(via_canonis, piscina), piscina);
            si (fons_canonis.mensura > ZEPHYRUM)
            {
                canon = canon_legere(fons_canonis, piscina,
                                     bib->intern, &causa);
            }
        }

        si (!canon)
        {
            vulnera++;
            ostensa++;
            si (machina)
            {
                imprimere("VULNUS\t8\t-\t-\tcanon '.genera' legi "
                          "nequit - gradus II NON iudicatus "
                          "(canones.registrum)\n");
            }
            alioquin
            {
                imprimere("VULNUS  regula  8  [-] -: canon "
                          "'.genera' legi nequit - gradus II NON "
                          "iudicatus (canones.registrum)\n");
            }
        }
        alioquin
        {
            i32 ie;

            per (ie = ZEPHYRUM; ie < xar_numerus(bib->exemplaria);
                 ie++)
            {
                NaturaExemplar* ex;
                           Xar* vitia;
                           i32  iv;

                ex = *(NaturaExemplar**)xar_obtinere(
                    bib->exemplaria, ie);
                vitia = canon_iudicare(canon, ex->radix, piscina);

                per (iv = ZEPHYRUM; iv < xar_numerus(vitia); iv++)
                {
                    CanonVitium* v;
                      character  campus[CCLVI];
                      character  nuntius[DXII];
                            i32  regula;

                    v = (CanonVitium*)xar_obtinere(vitia, iv);
                    vulnera++;

                    /* numerus regulae praesentationis: vitia
                     * canonis regulas migratas gerunt - citationes
                     * V/XII (transitus = XII), NOMEN_BIS = XV
                     * (unicitas 'entia'), cetera = VIII */
                    regula = VIII;
                    si (v->genus == CANON_CITATIO_IRRITA)
                    {
                        regula = (v->elementum
                            && chorda_aequalis_literis(*v->elementum,
                                                    "transitus"))
                            ? XII : V;
                    }
                    alioquin si (v->genus == CANON_NOMEN_BIS)
                    {
                        regula = XV;
                    }

                    si (   plagula && ex->stirps
                        && !viam_congruere(ex->stirps, plagula))
                    {
                        perge;
                    }
                    ostensa++;

                    campus[ZEPHYRUM] = '\0';
                    si (v->detail)
                    {
                        i32 m;

                        m = v->detail->mensura
                            < (i32)magnitudo(campus) - I
                            ? v->detail->mensura
                            : (i32)magnitudo(campus) - I;
                        memcpy(campus, v->detail->datum,
                               (memoriae_index)m);
                        campus[m] = '\0';
                    }
                    si (   v->genus == CANON_LIBERI_PAUCI
                        || v->genus == CANON_LIBERI_MULTI)
                    {
                        sprintf(nuntius, "%s: %s (%u, limes %u)",
                                canon_nuntius(v->genus), campus,
                                v->numerus, v->limes);
                    }
                    alioquin si (campus[ZEPHYRUM])
                    {
                        sprintf(nuntius, "%s: %s",
                                canon_nuntius(v->genus), campus);
                    }
                    alioquin
                    {
                        sprintf(nuntius, "%s",
                                canon_nuntius(v->genus));
                    }
                    /* linea in nuntio, non columna nova: forma
                     * V-columnaris quam visus et uncus legunt
                     * stabilis manet */
                    si (v->nodus && v->nodus->linea > ZEPHYRUM)
                    {
                        sprintf(nuntius + strlen(nuntius),
                                " (linea %u)", v->nodus->linea);
                    }

                    si (machina)
                    {
                        imprimere("VULNUS\t%u\t%.*s\t%.*s\t%s\n",
                            regula,
                            ex->stirps
                                ? (integer)ex->stirps->mensura : 1,
                            ex->stirps
                                ? (constans character*)
                                      ex->stirps->datum : "-",
                            v->elementum
                                ? (integer)v->elementum->mensura : 1,
                            v->elementum
                                ? (constans character*)
                                      v->elementum->datum : "-",
                            nuntius);
                    }
                    alioquin
                    {
                        imprimere("VULNUS  regula %2u  [%.*s] %.*s:"
                                  " %s\n",
                            regula,
                            ex->stirps
                                ? (integer)ex->stirps->mensura : 1,
                            ex->stirps
                                ? (constans character*)
                                      ex->stirps->datum : "-",
                            v->elementum
                                ? (integer)v->elementum->mensura : 1,
                            v->elementum
                                ? (constans character*)
                                      v->elementum->datum : "-",
                            nuntius);
                    }
                }
            }
        }
    }

    /* filtrum POST nexuram: regulae trans exemplaria transeunt */
    per (j = ZEPHYRUM; j < xar_numerus(bib->diagnostica); j++)
    {
        NaturaDiagnosticum* d;
        constans character* gradus;

        d = (NaturaDiagnosticum*)xar_obtinere(bib->diagnostica, j);
        si (d->gradus == NATURA_GRADUS_MONITUM)
        {
            monita++;
        }

        si (   plagula && d->modulus
            && !viam_congruere(d->modulus, plagula))
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

    /* relatio sedium (post diagnostica, ante summam) */
    si (familia_sedes)
    {
        NaturaNecessitudo* decl;

        decl = natura_necessitudo(bib, familia_sedes);
        si (!decl)
        {
            fprintf(stderr,
                "natura_examen: familia '%s' ignota - NIHIL"
                " relatum\n", familia_sedes);
            redde II;
        }
        sedes_referre(bib, decl, familia_sedes, machina);
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
                  " / necessitudines %u / VULNERA %u / MONITA %u\n",
                  onerata, xar_numerus(bib->genera_omnia),
                  xar_numerus(bib->res_omnes),
                  xar_numerus(bib->necessitudines_omnes),
                  vulnera, monita);
    }

    redde vulnera > ZEPHYRUM ? I : ZEPHYRUM;
}
