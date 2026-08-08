/* ==================================================
 * natura_quaesitor.c - interrogator bibliothecae semanticae
 *
 * bin/natura <mandatum> [argumenta]
 *
 *   quaere <terminus>   - NOMEN OCCUPATUM an LIBERUM? (custos
 *                         collisionum) + congruentia in prosa
 *   apparatus <genus>   - quae rogare debeas, cum hereditate
 *   maiores <genus>     - catena parentum ad radicem
 *   index               - inventarium generum omnium
 *
 * CUR HOC INSTRUMENTUM (occasio, 2026-08-08): candidatus
 * 'mensuratio' liber visus est quia inspectio mea per 'head -c'
 * truncata tag '[species:...]' absciderat - nomen apud
 * fundamentum sedebat. Custos generum id invenit POST genus
 * scriptum. Emendatio in memoria scripta erat 'lege totam
 * lineam', id est DISCIPLINA - quod foramen instrumenti est.
 * Ideo hic MANDATUM UNUM quaestioni verae respondet et
 * SENTENTIAM reddit, non lineas quas quisque truncare possit.
 *
 * Exitus: 0 = cursum; 2 = NIHIL CURSUM (corpus onerari nequit,
 * mandatum ignotum, genus non inventum). Quaestio sine
 * congruentia exitum 0 reddit - 'nihil inventum' responsum est,
 * non defectus.
 * ================================================== */

#include "postulata_posix.h"

#include "latina.h"
#include "natura.h"
#include "filum.h"
#include "iter_directoria.h"

#include <stdio.h>
#include <string.h>

#define EXTENSIO      ".genera"
#define RADIX_MORIS   "natura"
#define PROSAE_TECTUM 512
#define NOMINIS_TECTUM 256

/* ==================================================
 * Adiutores
 * ================================================== */

interior b32
_extensionem_habet(
    constans chorda* titulus)
{
    i32 m;

    m = (i32)strlen(EXTENSIO);
    si (titulus->mensura < m)
    {
        redde FALSUM;
    }
    redde (b32)(memcmp((constans character*)titulus->datum +
                       (titulus->mensura - m),
                       EXTENSIO, (memoriae_index)m) == ZEPHYRUM);
}

interior vacuum
_stirpem_scribere(
    constans chorda*  titulus,
    character*        exitus,
    i32               tectum)
{
    i32 m;

    m = titulus->mensura - (i32)strlen(EXTENSIO);
    si (m > tectum - I)
    {
        m = tectum - I;
    }
    memcpy(exitus, titulus->datum, (memoriae_index)m);
    exitus[m] = '\0';
}

/* prosa in unam lineam: spatia coguntur, nova linea perit */
interior vacuum
_prosa(
    chorda      fons,
    character*  exitus,
    i32         tectum)
{
    i32 i;
    i32 k;
    b32 spatium;

    k       = ZEPHYRUM;
    spatium = VERUM;   /* initium: spatia praecedentia vorat */

    per (i = ZEPHYRUM; i < fons.mensura && k < tectum - I; i++)
    {
        character c;

        c = ((constans character*)fons.datum)[i];
        si (c == ' ' || c == '\t' || c == '\n' || c == '\r')
        {
            si (!spatium)
            {
                exitus[k++] = ' ';
                spatium     = VERUM;
            }
        }
        alioquin
        {
            exitus[k++] = c;
            spatium     = FALSUM;
        }
    }
    dum (k > ZEPHYRUM && exitus[k - I] == ' ')
    {
        k--;
    }
    exitus[k] = '\0';
}

/* definitio, aut differentia si illa abest (sub-genera saepe
 * sola differentia utuntur - lex glossarum) */
interior vacuum
_sensum_scribere(
    StmlNodus*  nodus,
    Piscina*    piscina,
    character*  exitus,
    i32         tectum)
{
    StmlNodus* def;

    exitus[ZEPHYRUM] = '\0';
    si (!nodus)
    {
        redde;
    }
    def = stml_invenire_liberum(nodus, "definitio");
    si (!def)
    {
        def = stml_invenire_liberum(nodus, "differentia");
    }
    si (def)
    {
        _prosa(stml_textus_internus(def, piscina), exitus, tectum);
    }
}

interior vacuum
_chordam_scribere(
    constans chorda*  fons,
    character*        exitus,
    i32               tectum)
{
    i32 m;

    exitus[ZEPHYRUM] = '\0';
    si (!fons)
    {
        redde;
    }
    m = fons->mensura < tectum - I ? fons->mensura : tectum - I;
    memcpy(exitus, fons->datum, (memoriae_index)m);
    exitus[m] = '\0';
}

/* congruentia insensibilis capitis */
interior b32
_continet(
    constans character*  acervus,
    constans character*  acus)
{
    i32 la;
    i32 lb;
    i32 i;

    la = (i32)strlen(acervus);
    lb = (i32)strlen(acus);
    si (lb == ZEPHYRUM || lb > la)
    {
        redde (b32)(lb == ZEPHYRUM);
    }
    per (i = ZEPHYRUM; i + lb <= la; i++)
    {
        i32 k;
        b32 par;

        par = VERUM;
        per (k = ZEPHYRUM; k < lb; k++)
        {
            character x;
            character y;

            x = acervus[i + k];
            y = acus[k];
            si (x >= 'A' && x <= 'Z')
            {
                x = (character)(x + ('a' - 'A'));
            }
            si (y >= 'A' && y <= 'Z')
            {
                y = (character)(y + ('a' - 'A'));
            }
            si (x != y)
            {
                par = FALSUM;
                frange;
            }
        }
        si (par)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Corpus onerare (mos natura_examen)
 * ================================================== */

interior i32
_corpus_onerare(
    NaturaBibliotheca*   bib,
    constans character*  radix,
    Piscina*             piscina)
{
    DirectoriumIterator*  iter;
    DirectoriumIntroitus* introitus;
    i32                   onerata;

    iter = directorium_iterator_aperire(radix, piscina);
    si (!iter)
    {
        redde ZEPHYRUM;
    }

    onerata = ZEPHYRUM;
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        character via[DXII];
        character stirps[NOMINIS_TECTUM];
        chorda    fons;

        si (introitus->genus != INTROITUS_FILUM ||
            !_extensionem_habet(&introitus->titulus))
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
        _stirpem_scribere(&introitus->titulus, stirps,
                          (i32)magnitudo(stirps));

        fons = filum_legere_totum(via, piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            perge;
        }
        natura_legere(bib, fons, stirps);
        onerata++;
    }
    directorium_iterator_claudere(iter);
    redde onerata;
}

/* ==================================================
 * quaere - custos collisionum
 * ================================================== */

interior vacuum
_quaerere(
    NaturaBibliotheca*   bib,
    constans character*  terminus,
    b32                  machina,
    Piscina*             piscina)
{
    i32 i;
    i32 exacta;
    i32 congrua;

    exacta  = ZEPHYRUM;
    congrua = ZEPHYRUM;

    si (!machina)
    {
        imprimere("quaesitum: '%s'\n\n", terminus);
    }

    per (i = ZEPHYRUM; i < xar_numerus(bib->genera_omnia); i++)
    {
        NaturaGenus* g;
        character    tit[NOMINIS_TECTUM];
        character    mod[NOMINIS_TECTUM];
        character    sensus[PROSAE_TECTUM];
        b32          nomine;

        g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
        _chordam_scribere(g->titulus, tit, (i32)magnitudo(tit));
        _chordam_scribere(g->modulus, mod, (i32)magnitudo(mod));
        _sensum_scribere(g->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));

        nomine = (b32)(strcmp(tit, terminus) == ZEPHYRUM);
        si (nomine)
        {
            exacta++;
        }
        alioquin si (!_continet(tit, terminus) &&
                     !_continet(sensus, terminus))
        {
            perge;
        }
        congrua++;

        si (machina)
        {
            imprimere("GENUS\t%s\t%s\t%s\t%s\n", mod, tit,
                      nomine ? "EXACTUM" : "-",
                      g->parens && g->parens->titulus ?
                          (constans character*)g->parens->titulus->datum
                          : "-");
        }
        alioquin
        {
            imprimere("  GENUS%s   /%s/%s\n",
                      nomine ? " [NOMEN IPSUM]" : "        ",
                      mod, tit);
            si (sensus[ZEPHYRUM])
            {
                imprimere("             %.100s%s\n", sensus,
                          strlen(sensus) > 100 ? "..." : "");
            }
        }
    }

    per (i = ZEPHYRUM; i < xar_numerus(bib->res_omnes); i++)
    {
        NaturaRes* r;
        character  tit[NOMINIS_TECTUM];
        character  mod[NOMINIS_TECTUM];
        character  sensus[PROSAE_TECTUM];
        character  sub[NOMINIS_TECTUM];
        b32        nomine;

        r = *(NaturaRes**)xar_obtinere(bib->res_omnes, i);
        _chordam_scribere(r->titulus, tit, (i32)magnitudo(tit));
        _chordam_scribere(r->modulus, mod, (i32)magnitudo(mod));
        _sensum_scribere(r->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));
        sub[ZEPHYRUM] = '\0';
        si (r->genus_suum)
        {
            _chordam_scribere(r->genus_suum->titulus, sub,
                              (i32)magnitudo(sub));
        }

        nomine = (b32)(strcmp(tit, terminus) == ZEPHYRUM);
        si (nomine)
        {
            exacta++;
        }
        alioquin si (!_continet(tit, terminus) &&
                     !_continet(sensus, terminus))
        {
            perge;
        }
        congrua++;

        si (machina)
        {
            imprimere("RES\t%s\t%s\t%s\t%s\n", mod, tit,
                      nomine ? "EXACTUM" : "-", sub);
        }
        alioquin
        {
            imprimere("  RES%s     /%s/%s   (in genere %s)\n",
                      nomine ? " [NOMEN IPSUM]" : "     ",
                      mod, tit, sub[ZEPHYRUM] ? sub : "-");
            si (sensus[ZEPHYRUM])
            {
                imprimere("             %.100s%s\n", sensus,
                          strlen(sensus) > 100 ? "..." : "");
            }
        }
    }

    /* SENTENTIA - quaestio vera, non lineae solae. Haec linea
     * causa huius instrumenti est: truncari non potest sine
     * responso ipso amisso. */
    si (machina)
    {
        imprimere("SENTENTIA\t%s\t%d\t%d\n",
                  exacta > ZEPHYRUM ? "OCCUPATUM" : "LIBERUM",
                  (integer)exacta, (integer)congrua);
    }
    alioquin
    {
        si (congrua == ZEPHYRUM)
        {
            imprimere("  (nihil congruit)\n");
        }
        imprimere("\n  SENTENTIA: nomen '%s' %s", terminus,
                  exacta > ZEPHYRUM ?
                      "OCCUPATUM EST" : "LIBERUM est");
        si (exacta > ZEPHYRUM)
        {
            imprimere(" (%d entia nomine ipso)", (integer)exacta);
        }
        imprimere("  [congruentia %d]\n", (integer)congrua);
        si (exacta == ZEPHYRUM)
        {
            /* SENTENTIA FINES SUOS DICIT: negativum quod
             * inspectionem suam non nominat plus promittit quam
             * praestat - id est ipsum vitium quod hoc
             * instrumentum vitare debet */
            imprimere("  (inspecta: genera et res. UMBRAE NON "
                      "INSPECTAE - './tools/natura_quaere.sh %s' "
                      "eas videt; desideratum 01KZH0ZBVX)\n",
                      terminus);
        }
    }
}

/* ==================================================
 * apparatus / maiores
 * ================================================== */

interior NaturaGenus*
_genus_solvere(
    NaturaBibliotheca*   bib,
    constans character*  petitum)
{
    constans character* sol;

    sol = strchr(petitum, '/');
    si (sol)
    {
        character mod[NOMINIS_TECTUM];
        i32       m;

        m = (i32)(sol - petitum);
        si (m > (i32)magnitudo(mod) - I)
        {
            m = (i32)magnitudo(mod) - I;
        }
        memcpy(mod, petitum, (memoriae_index)m);
        mod[m] = '\0';
        redde natura_genus_in(bib, mod, sol + I);
    }
    redde natura_genus(bib, petitum);
}

interior vacuum
_apparatum_scribere(
    NaturaBibliotheca*   bib,
    NaturaGenus*         genus,
    b32                  machina,
    Piscina*             piscina)
{
    Xar* membra;
    i32  i;

    membra = natura_apparatus(bib, genus, piscina);

    si (!machina)
    {
        character sensus[PROSAE_TECTUM];

        imprimere("apparatus /%.*s/%.*s\n",
                  (integer)genus->modulus->mensura,
                  (constans character*)genus->modulus->datum,
                  (integer)genus->titulus->mensura,
                  (constans character*)genus->titulus->datum);
        _sensum_scribere(genus->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));
        si (sensus[ZEPHYRUM])
        {
            imprimere("  %s\n", sensus);
        }
        imprimere("\n--- QUAE ROGARE DEBEAS ---\n");
    }

    per (i = ZEPHYRUM; i < xar_numerus(membra); i++)
    {
        NaturaApparatusMembrum* m;
        chorda*                 titulus;
        constans character*     genus_membri;
        constans character*     auctor;

        m            = (NaturaApparatusMembrum*)xar_obtinere(membra, i);
        genus_membri = m->nodus->titulus ?
                           (constans character*)m->nodus->titulus->datum
                           : "?";
        titulus      = stml_attributum_capere(m->nodus, "nomen");
        auctor       = (m->auctor && m->auctor->titulus) ?
                           (constans character*)m->auctor->titulus->datum
                           : "-";

        si (machina)
        {
            imprimere("%s\t%.*s\t%s\n", genus_membri,
                      titulus ? (integer)titulus->mensura : I,
                      titulus ? (constans character*)titulus->datum
                              : "-",
                      auctor);
        }
        alioquin
        {
            b32 hereditarium;

            hereditarium = (b32)(m->auctor != genus);
            imprimere("  ? %-14s %-28.*s%s\n", genus_membri,
                      titulus ? (integer)titulus->mensura : I,
                      titulus ? (constans character*)titulus->datum
                              : "-",
                      hereditarium ? " [hereditarium]" : "");
        }
    }
}

interior vacuum
_maiores_scribere(
    NaturaGenus*  genus,
    b32           machina,
    Piscina*      piscina)
{
    Xar* catena;
    i32  i;

    catena = natura_maiores(genus, piscina);

    si (!machina)
    {
        imprimere("catena /%.*s/%.*s\n",
                  (integer)genus->modulus->mensura,
                  (constans character*)genus->modulus->datum,
                  (integer)genus->titulus->mensura,
                  (constans character*)genus->titulus->datum);
    }
    per (i = ZEPHYRUM; i < xar_numerus(catena); i++)
    {
        NaturaGenus* g;

        g = *(NaturaGenus**)xar_obtinere(catena, i);
        si (machina)
        {
            imprimere("MAIOR\t%.*s\t%.*s\n",
                      (integer)g->modulus->mensura,
                      (constans character*)g->modulus->datum,
                      (integer)g->titulus->mensura,
                      (constans character*)g->titulus->datum);
        }
        alioquin
        {
            imprimere("  %*s^ /%.*s/%.*s\n", (integer)(i * II), "",
                      (integer)g->modulus->mensura,
                      (constans character*)g->modulus->datum,
                      (integer)g->titulus->mensura,
                      (constans character*)g->titulus->datum);
        }
    }
    si (!machina && xar_numerus(catena) == ZEPHYRUM)
    {
        imprimere("  (radix - nullum maiorem habet)\n");
    }
}

interior vacuum
_indicem_scribere(
    NaturaBibliotheca* bib,
    b32                machina,
    Piscina*           piscina)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(bib->genera_omnia); i++)
    {
        NaturaGenus* g;
        character    sensus[PROSAE_TECTUM];

        g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
        _sensum_scribere(g->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));

        si (machina)
        {
            imprimere("%.*s\t%.*s\t%.*s\n",
                      (integer)g->modulus->mensura,
                      (constans character*)g->modulus->datum,
                      (integer)g->titulus->mensura,
                      (constans character*)g->titulus->datum,
                      (integer)strlen(sensus), sensus);
        }
        alioquin
        {
            imprimere("  /%.*s/%-26.*s %.60s\n",
                      (integer)g->modulus->mensura,
                      (constans character*)g->modulus->datum,
                      (integer)g->titulus->mensura,
                      (constans character*)g->titulus->datum,
                      sensus);
        }
    }
}

/* ==================================================
 * principale
 * ================================================== */

interior vacuum
_usum_scribere(vacuum)
{
    fprintf(stderr,
        "usus: natura <mandatum> [argumenta] [-machina] "
        "[-radix DIR]\n"
        "  quaere <terminus>   nomen occupatum an liberum "
        "(custos collisionum)\n"
        "  apparatus <genus>   quae rogare debeas, cum "
        "hereditate\n"
        "  maiores <genus>     catena parentum ad radicem\n"
        "  index               inventarium generum omnium\n"
        "genus: 'planta' aut '/modulus/planta' (homonyma "
        "modulum poscunt)\n");
}

integer
main(
    integer argumentorum_numerus,
    character** argumenta)
{
    Piscina*            piscina;
    NaturaBibliotheca*  bib;
    constans character* radix;
    constans character* mandatum;
    constans character* petitum;
    b32                 machina;
    integer             i;
    i32                 onerata;

    radix    = RADIX_MORIS;
    mandatum = NIHIL;
    petitum  = NIHIL;
    machina  = FALSUM;

    per (i = I; i < argumentorum_numerus; i++)
    {
        si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-radix") == ZEPHYRUM &&
                     i + I < argumentorum_numerus)
        {
            radix = argumenta[++i];
        }
        alioquin si (!mandatum)
        {
            mandatum = argumenta[i];
        }
        alioquin si (!petitum)
        {
            petitum = argumenta[i];
        }
        alioquin
        {
            _usum_scribere();
            redde II;
        }
    }

    si (!mandatum)
    {
        _usum_scribere();
        redde II;
    }

    piscina = piscina_generare_dynamicum("natura_quaesitor", 1048576);
    bib     = natura_bibliotheca_creare(piscina);
    si (!bib)
    {
        fprintf(stderr, "natura: bibliotheca creari nequit\n");
        redde II;
    }

    onerata = _corpus_onerare(bib, radix, piscina);
    si (onerata == ZEPHYRUM)
    {
        /* NIHIL CURSUM EST - non successus */
        fprintf(stderr,
            "natura: NULLUM exemplar in '%s' inventum "
            "(extensio '%s')\n", radix, EXTENSIO);
        redde II;
    }
    (vacuum)natura_nectere(bib);

    si (strcmp(mandatum, "index") == ZEPHYRUM)
    {
        _indicem_scribere(bib, machina, piscina);
        redde ZEPHYRUM;
    }

    si (!petitum)
    {
        _usum_scribere();
        redde II;
    }

    si (strcmp(mandatum, "quaere") == ZEPHYRUM)
    {
        _quaerere(bib, petitum, machina, piscina);
        redde ZEPHYRUM;
    }

    si (strcmp(mandatum, "apparatus") == ZEPHYRUM ||
        strcmp(mandatum, "maiores") == ZEPHYRUM)
    {
        NaturaGenus* genus;

        genus = _genus_solvere(bib, petitum);
        si (!genus)
        {
            /* nomen ignotum aut HOMONYMUM (tunc modulus
             * poscitur) - utrumque 'nihil cursum est' */
            fprintf(stderr,
                "natura: genus '%s' non inventum (aut homonymum: "
                "'/modulus/%s' tempta)\n", petitum, petitum);
            redde II;
        }
        si (strcmp(mandatum, "apparatus") == ZEPHYRUM)
        {
            _apparatum_scribere(bib, genus, machina, piscina);
        }
        alioquin
        {
            _maiores_scribere(genus, machina, piscina);
        }
        redde ZEPHYRUM;
    }

    fprintf(stderr, "natura: mandatum ignotum '%s'\n", mandatum);
    _usum_scribere();
    redde II;
}
