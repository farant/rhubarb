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
 * Umbrae - superfecta, nondum descripta
 *
 * UMBRA = valor 'ad' elementi quod 'externum="verum"' fert:
 * nomen adhibitum quod corpus nondum describit. Ratio ex
 * natura_examen sumpta (ordines U corporis), NON divinata -
 * custos collisionum ex coniectura peior est quam nullus.
 * ================================================== */

interior vacuum
_elementa_colligere(
    StmlNodus*  nodus,
    Xar*        acervus)
{
    i32 n;
    i32 i;

    si (!nodus)
    {
        redde;
    }
    si (nodus->genus == STML_NODUS_ELEMENTUM)
    {
        *(StmlNodus**)xar_addere(acervus) = nodus;
    }
    n = stml_numerus_liberorum(nodus);
    per (i = ZEPHYRUM; i < n; i++)
    {
        _elementa_colligere(stml_liberum_ad_indicem(nodus, i),
                            acervus);
    }
}

/* Redde numerum umbrarum nomine IPSO congruentium; si 'scribere',
 * congruentia omnia imprimit. */
interior i32
_umbras_tractare(
    NaturaBibliotheca*   bib,
    constans character*  terminus,
    b32                  scribere,
    b32                  machina,
    Piscina*             piscina)
{
    Xar* visa;   /* nomina iam emissa - umbra eadem saepe multis
                  * locis citatur (shell 'sort -u' faciebat) */
    i32  e;
    i32  x;

    visa = xar_creare(piscina, (i32)magnitudo(character*));
    e    = ZEPHYRUM;

    per (x = ZEPHYRUM; x < xar_numerus(bib->exemplaria); x++)
    {
        NaturaExemplar* ex;
        Xar*            omnia;
        character       mod[NOMINIS_TECTUM];
        i32             i;

        ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria, x);
        _chordam_scribere(ex->stirps, mod, (i32)magnitudo(mod));

        omnia = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
        _elementa_colligere(ex->radix, omnia);

        per (i = ZEPHYRUM; i < xar_numerus(omnia); i++)
        {
            StmlNodus* n;
            chorda*    v;
            chorda*    ad;
            character  u[NOMINIS_TECTUM];

            n = *(StmlNodus**)xar_obtinere(omnia, i);
            v = stml_attributum_capere(n, "externum");
            si (!v || !chorda_aequalis_literis(*v, "verum"))
            {
                perge;
            }
            ad = stml_attributum_capere(n, "ad");
            si (!ad || ad->mensura == ZEPHYRUM)
            {
                perge;
            }
            _chordam_scribere(ad, u, (i32)magnitudo(u));
            si (!_continet(u, terminus))
            {
                perge;
            }

            {
                b32 iam;
                i32 k;

                iam = FALSUM;
                per (k = ZEPHYRUM; k < xar_numerus(visa); k++)
                {
                    si (strcmp(*(character**)xar_obtinere(visa, k),
                               u) == ZEPHYRUM)
                    {
                        iam = VERUM;
                        frange;
                    }
                }
                si (iam)
                {
                    perge;
                }
                *(character**)xar_addere(visa) =
                    (character*)chorda_ut_cstr(
                        chorda_ex_literis(u, piscina), piscina);
            }

            si (strcmp(u, terminus) == ZEPHYRUM)
            {
                e++;
            }
            si (scribere)
            {
                si (machina)
                {
                    imprimere("UMBRA\t%s\t%s\t%s\n", mod, u,
                              strcmp(u, terminus) == ZEPHYRUM ?
                                  "EXACTUM" : "-");
                }
                alioquin
                {
                    imprimere("  UMBRA%s   %s  (a %s superfectum)\n",
                              strcmp(u, terminus) == ZEPHYRUM ?
                                  " [NOMEN IPSUM]" : "       ",
                              u, mod);
                }
            }
        }
    }
    redde e;
}

/* ==================================================
 * quaere - custos collisionum
 * ================================================== */

/* punctum: nomen ipsum X, nomen continens VI, prosa III */
#define PUNCTUM_NOMEN_IPSUM  X
#define PUNCTUM_NOMEN        VI
#define PUNCTUM_PROSAE       III

/* discrimen expressum, non veritas monstratoris: 'si (c->genus)'
 * necessitudinem tacite ut rem tractaret (tertium genus additum
 * 2026-08-10) */
nomen enumeratio {
    QC_GENUS       = I,
    QC_RES         = II,
    QC_NECESSITUDO = III
} QuaesitiCongruentiaDiscrimen;

nomen structura {
    QuaesitiCongruentiaDiscrimen  discrimen;
    NaturaGenus*        genus;
    NaturaRes*          res;
    NaturaNecessitudo*  necessitudo;
    i32                 punctum;
    b32                 nomine;
} QuaesitiCongruentia;

interior vacuum
_congruentiam_scribere(
    QuaesitiCongruentia*  c,
    b32                   machina,
    Piscina*              piscina)
{
    character tit[NOMINIS_TECTUM];
    character mod[NOMINIS_TECTUM];
    character sensus[PROSAE_TECTUM];
    character apud[NOMINIS_TECTUM];

    apud[ZEPHYRUM] = '\0';

    si (c->discrimen == QC_GENUS)
    {
        _chordam_scribere(c->genus->titulus, tit,
                          (i32)magnitudo(tit));
        _chordam_scribere(c->genus->modulus, mod,
                          (i32)magnitudo(mod));
        _sensum_scribere(c->genus->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));
        si (c->genus->parens && c->genus->parens->titulus)
        {
            _chordam_scribere(c->genus->parens->titulus, apud,
                              (i32)magnitudo(apud));
        }
    }
    alioquin si (c->discrimen == QC_NECESSITUDO)
    {
        _chordam_scribere(c->necessitudo->titulus, tit,
                          (i32)magnitudo(tit));
        _chordam_scribere(c->necessitudo->modulus, mod,
                          (i32)magnitudo(mod));
        _sensum_scribere(c->necessitudo->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));
        /* apud = conversum (directio altera eiusdem identitatis) */
        si (c->necessitudo->conversum)
        {
            _chordam_scribere(c->necessitudo->conversum, apud,
                              (i32)magnitudo(apud));
        }
    }
    alioquin
    {
        _chordam_scribere(c->res->titulus, tit,
                          (i32)magnitudo(tit));
        _chordam_scribere(c->res->modulus, mod,
                          (i32)magnitudo(mod));
        _sensum_scribere(c->res->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));
        si (c->res->genus_suum)
        {
            _chordam_scribere(c->res->genus_suum->titulus, apud,
                              (i32)magnitudo(apud));
        }
    }

    si (machina)
    {
        imprimere("%s\t%s\t%s\t%s\t%s\n",
                  c->discrimen == QC_GENUS ? "GENUS" :
                  c->discrimen == QC_NECESSITUDO ? "NECESSITUDO" :
                      "RES",
                  mod, tit,
                  c->nomine ? "EXACTUM" : "-",
                  apud[ZEPHYRUM] ? apud : "-");
    }
    alioquin
    {
        si (c->discrimen == QC_GENUS)
        {
            imprimere("  GENUS%s   /%s/%s%s%s%s\n",
                      c->nomine ? " [NOMEN IPSUM]" : "        ",
                      mod, tit,
                      apud[ZEPHYRUM] ? "   (sub " : "",
                      apud[ZEPHYRUM] ? apud : "",
                      apud[ZEPHYRUM] ? ")" : "");
        }
        alioquin si (c->discrimen == QC_NECESSITUDO)
        {
            imprimere("  NECESSITUDO%s   /%s/%s%s%s%s\n",
                      c->nomine ? " [NOMEN IPSUM]" : "        ",
                      mod, tit,
                      apud[ZEPHYRUM] ? "   (conversum " : "",
                      apud[ZEPHYRUM] ? apud : "",
                      apud[ZEPHYRUM] ? ")" : "");
        }
        alioquin
        {
            character intra[NOMINIS_TECTUM];

            /* TAXINOMIA ante apparatum: continens PROXIMA prius,
             * quia lector 'ubi sedet?' quaerit, non 'unde
             * hereditat?' */
            intra[ZEPHYRUM] = '\0';
            si (c->res->continens)
            {
                _chordam_scribere(c->res->continens->titulus, intra,
                                  (i32)magnitudo(intra));
            }
            imprimere("  RES%s     /%s/%s   (",
                      c->nomine ? " [NOMEN IPSUM]" : "     ",
                      mod, tit);
            si (intra[ZEPHYRUM])
            {
                imprimere("intra %s, ", intra);
            }
            imprimere("in genere %s)\n", apud[ZEPHYRUM] ? apud : "-");
        }
        si (sensus[ZEPHYRUM])
        {
            imprimere("             %.100s%s\n", sensus,
                      strlen(sensus) > 100 ? "..." : "");
        }
    }
}

interior vacuum
_quaerere(
    NaturaBibliotheca*   bib,
    constans character*  terminus,
    b32                  machina,
    Piscina*             piscina)
{
    Xar* congruentiae;
    i32  i;
    i32  gradus;
    i32  exacta;
    i32  umbrae_exactae;

    congruentiae = xar_creare(piscina,
                              (i32)magnitudo(QuaesitiCongruentia));
    exacta       = ZEPHYRUM;

    si (!machina)
    {
        imprimere("quaesitum: '%s'\n\n", terminus);
    }

    per (i = ZEPHYRUM; i < xar_numerus(bib->genera_omnia); i++)
    {
        NaturaGenus* g;
        character    tit[NOMINIS_TECTUM];
        character    sensus[PROSAE_TECTUM];
        i32          punctum;

        g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
        _chordam_scribere(g->titulus, tit, (i32)magnitudo(tit));
        _sensum_scribere(g->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));

        punctum = ZEPHYRUM;
        si (strcmp(tit, terminus) == ZEPHYRUM)
        {
            punctum = PUNCTUM_NOMEN_IPSUM;
        }
        alioquin si (_continet(tit, terminus))
        {
            punctum = PUNCTUM_NOMEN;
        }
        alioquin si (_continet(sensus, terminus))
        {
            punctum = PUNCTUM_PROSAE;
        }
        si (punctum == ZEPHYRUM)
        {
            perge;
        }

        {
            QuaesitiCongruentia* c;

            c = (QuaesitiCongruentia*)xar_addere(congruentiae);
            c->discrimen   = QC_GENUS;
            c->genus       = g;
            c->res         = NIHIL;
            c->necessitudo = NIHIL;
            c->punctum     = punctum;
            c->nomine      = (b32)(punctum == PUNCTUM_NOMEN_IPSUM);
            si (c->nomine)
            {
                exacta++;
            }
        }
    }

    per (i = ZEPHYRUM; i < xar_numerus(bib->res_omnes); i++)
    {
        NaturaRes* r;
        character  tit[NOMINIS_TECTUM];
        character  sensus[PROSAE_TECTUM];
        i32        punctum;

        r = *(NaturaRes**)xar_obtinere(bib->res_omnes, i);
        _chordam_scribere(r->titulus, tit, (i32)magnitudo(tit));
        _sensum_scribere(r->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));

        punctum = ZEPHYRUM;
        si (strcmp(tit, terminus) == ZEPHYRUM)
        {
            punctum = PUNCTUM_NOMEN_IPSUM;
        }
        alioquin si (_continet(tit, terminus))
        {
            punctum = PUNCTUM_NOMEN;
        }
        alioquin si (_continet(sensus, terminus))
        {
            punctum = PUNCTUM_PROSAE;
        }
        si (punctum == ZEPHYRUM)
        {
            perge;
        }

        {
            QuaesitiCongruentia* c;

            c = (QuaesitiCongruentia*)xar_addere(congruentiae);
            c->discrimen   = QC_RES;
            c->genus       = NIHIL;
            c->res         = r;
            c->necessitudo = NIHIL;
            c->punctum     = punctum;
            c->nomine      = (b32)(punctum == PUNCTUM_NOMEN_IPSUM);
            si (c->nomine)
            {
                exacta++;
            }
        }
    }

    /* AMBULATIO QUARTA: necessitudines. Nomen primarium,
     * conversum, et scriptura quaevis IDENTITATEM UNAM occupant -
     * quaere quod tres inspicere debet, aliter 'pollinat' liber
     * videretur dum relatio viva est (caecitas mensurata
     * 2026-08-10, ante hanc ambulationem). */
    per (i = ZEPHYRUM;
         i < xar_numerus(bib->necessitudines_omnes); i++)
    {
        NaturaNecessitudo* nx;
        character          tit[NOMINIS_TECTUM];
        character          sensus[PROSAE_TECTUM];
        i32                punctum;

        nx = *(NaturaNecessitudo**)xar_obtinere(
            bib->necessitudines_omnes, i);
        _chordam_scribere(nx->titulus, tit, (i32)magnitudo(tit));
        _sensum_scribere(nx->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));

        punctum = ZEPHYRUM;
        si (strcmp(tit, terminus) == ZEPHYRUM)
        {
            punctum = PUNCTUM_NOMEN_IPSUM;
        }
        si (punctum == ZEPHYRUM && nx->conversum)
        {
            character conv[NOMINIS_TECTUM];

            _chordam_scribere(nx->conversum, conv,
                              (i32)magnitudo(conv));
            si (strcmp(conv, terminus) == ZEPHYRUM)
            {
                punctum = PUNCTUM_NOMEN_IPSUM;
            }
        }
        si (punctum == ZEPHYRUM && nx->nodus)
        {
            i32 n_s;
            i32 j;

            n_s = stml_numerus_liberorum(nx->nodus);
            per (j = ZEPHYRUM; j < n_s; j++)
            {
                StmlNodus* s;
                character  scr[NOMINIS_TECTUM];

                s = stml_liberum_ad_indicem(nx->nodus, j);
                si (!s || s->genus != STML_NODUS_ELEMENTUM ||
                    !chorda_aequalis_literis(*s->titulus,
                                             "scriptio"))
                {
                    perge;
                }
                _prosa(stml_textus_internus(s, piscina), scr,
                       (i32)magnitudo(scr));
                si (strcmp(scr, terminus) == ZEPHYRUM)
                {
                    punctum = PUNCTUM_NOMEN_IPSUM;
                    frange;
                }
            }
        }
        si (punctum == ZEPHYRUM && _continet(tit, terminus))
        {
            punctum = PUNCTUM_NOMEN;
        }
        si (punctum == ZEPHYRUM && _continet(sensus, terminus))
        {
            punctum = PUNCTUM_PROSAE;
        }
        si (punctum == ZEPHYRUM)
        {
            perge;
        }

        {
            QuaesitiCongruentia* c;

            c = (QuaesitiCongruentia*)xar_addere(congruentiae);
            c->discrimen   = QC_NECESSITUDO;
            c->genus       = NIHIL;
            c->res         = NIHIL;
            c->necessitudo = nx;
            c->punctum     = punctum;
            c->nomine      = (b32)(punctum == PUNCTUM_NOMEN_IPSUM);
            si (c->nomine)
            {
                exacta++;
            }
        }
    }

    /* ordo per gradus (tres soli - ordinatio vera superflua) */
    per (gradus = PUNCTUM_NOMEN_IPSUM; gradus > ZEPHYRUM; gradus--)
    {
        per (i = ZEPHYRUM; i < xar_numerus(congruentiae); i++)
        {
            QuaesitiCongruentia* c;

            c = (QuaesitiCongruentia*)xar_obtinere(congruentiae, i);
            si (c->punctum == gradus)
            {
                _congruentiam_scribere(c, machina, piscina);
            }
        }
    }

    /* UMBRAE: nomina adhibita quae corpus nondum describit */
    si (!machina && xar_numerus(congruentiae) == ZEPHYRUM)
    {
        imprimere("  (nihil congruit inter genera et res)\n");
    }
    umbrae_exactae = _umbras_tractare(bib, terminus, VERUM, machina,
                                      piscina);

    /* SENTENTIA - quaestio vera, non lineae solae. ULTIMA linea
     * consulto: responsum truncationem superest, quae causa huius
     * instrumenti fuit. TRIA sunt, non duo: umbra nomen TENET sed
     * describi EXSPECTAT - eam implere opus intentum est, non
     * collisio. */
    {
        constans character* sententia;

        si (exacta > ZEPHYRUM)
        {
            sententia = "OCCUPATUM";
        }
        alioquin si (umbrae_exactae > ZEPHYRUM)
        {
            sententia = "UMBRATUM";
        }
        alioquin
        {
            sententia = "LIBERUM";
        }

        si (machina)
        {
            imprimere("SENTENTIA\t%s\t%d\t%d\t%d\n", sententia,
                      (integer)exacta,
                      (integer)xar_numerus(congruentiae),
                      (integer)umbrae_exactae);
        }
        alioquin
        {
            imprimere("\n  SENTENTIA: nomen '%s' ", terminus);
            si (exacta > ZEPHYRUM)
            {
                imprimere("OCCUPATUM EST (%d entia nomine ipso)\n",
                          (integer)exacta);
            }
            alioquin si (umbrae_exactae > ZEPHYRUM)
            {
                imprimere("UMBRATUM est: nomen adhibetur nec "
                          "describitur.\n             Genus eo "
                          "nomine condere umbram IMPLET (opus "
                          "intentum), non collidit.\n");
            }
            alioquin
            {
                imprimere("LIBERUM est (genera, res, "
                          "necessitudines, umbrae omnia "
                          "inspecta).\n");
            }
            imprimere("  [congruentia %d]\n",
                      (integer)xar_numerus(congruentiae));
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

    /* AMBAE formae: '/modulus/genus' (mos censūs) et
     * 'modulus.genus' (mos natura_quaere.sh - memoria manuum
     * servanda est cum instrumentum migrat) */
    si (petitum[ZEPHYRUM] == '/')
    {
        petitum++;
    }
    sol = strchr(petitum, '/');
    si (!sol)
    {
        sol = strchr(petitum, '.');
    }
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

/* liberos nomine dato in unam lineam iungere (optiones, status) */
interior vacuum
_liberos_iungere(
    StmlNodus*           nodus,
    constans character*  genus_liberi,
    b32                  per_nomen,
    Piscina*             piscina,
    character*           exitus,
    i32                  tectum)
{
    i32 n;
    i32 i;
    i32 k;

    exitus[ZEPHYRUM] = '\0';
    k = ZEPHYRUM;
    n = stml_numerus_liberorum(nodus);

    per (i = ZEPHYRUM; i < n; i++)
    {
        StmlNodus* s;
        character  frustum[NOMINIS_TECTUM];

        s = stml_liberum_ad_indicem(nodus, i);
        si (!s || s->genus != STML_NODUS_ELEMENTUM || !s->titulus ||
            !chorda_aequalis_literis(*s->titulus, genus_liberi))
        {
            perge;
        }

        frustum[ZEPHYRUM] = '\0';
        si (per_nomen)
        {
            _chordam_scribere(stml_attributum_capere(s, "nomen"),
                              frustum, (i32)magnitudo(frustum));
        }
        alioquin
        {
            _prosa(stml_textus_internus(s, piscina), frustum,
                   (i32)magnitudo(frustum));
        }
        si (!frustum[ZEPHYRUM])
        {
            perge;
        }
        si (k > ZEPHYRUM && k < tectum - III)
        {
            exitus[k++] = '|';
        }
        {
            i32 m;

            m = (i32)strlen(frustum);
            si (k + m > tectum - I)
            {
                m = tectum - I - k;
            }
            si (m > ZEPHYRUM)
            {
                memcpy(exitus + k, frustum, (memoriae_index)m);
                k += m;
            }
        }
    }
    exitus[k] = '\0';
}

interior vacuum
_membrum_scribere(
    NaturaApparatusMembrum*  m,
    NaturaGenus*             genus,
    Piscina*                 piscina)
{
    character  nom[NOMINIS_TECTUM];
    character  praefixum[NOMINIS_TECTUM];
    character  kind[NOMINIS_TECTUM];

    /* titulus chordae NON terminatur - copia terminata, ne
     * dispatch ultra mensuram legat (laqueus mensuratus) */
    kind[ZEPHYRUM] = '?';
    kind[I]        = '\0';
    si (m->nodus->titulus)
    {
        _chordam_scribere(m->nodus->titulus, kind,
                          (i32)magnitudo(kind));
    }
    _chordam_scribere(stml_attributum_capere(m->nodus, "nomen"),
                      nom, (i32)magnitudo(nom));

    praefixum[ZEPHYRUM] = '\0';
    si (m->auctor != genus && m->auctor && m->auctor->titulus)
    {
        character am[NOMINIS_TECTUM];
        character ag[NOMINIS_TECTUM];

        _chordam_scribere(m->auctor->modulus, am, (i32)magnitudo(am));
        _chordam_scribere(m->auctor->titulus, ag, (i32)magnitudo(ag));
        sprintf(praefixum, "[a %s.%s] ", am, ag);
    }

    si (strcmp(kind, "terminus") == ZEPHYRUM)
    {
        /* in blocco TERMINI proprio scribitur, non hic */
        redde;
    }
    si (strcmp(kind, "nota") == ZEPHYRUM)
    {
        /* margo curatorialis, non quaestio apparatus */
        redde;
    }

    si (strcmp(kind, "proprietas") == ZEPHYRUM)
    {
        character ty[NOMINIS_TECTUM];
        character tm[NOMINIS_TECTUM];

        _chordam_scribere(stml_attributum_capere(m->nodus, "genus"),
                          ty, (i32)magnitudo(ty));
        _chordam_scribere(stml_attributum_capere(m->nodus, "modulus"),
                          tm, (i32)magnitudo(tm));

        imprimere("  ? %s%s", praefixum, nom);
        si (strcmp(ty, "electio") == ZEPHYRUM)
        {
            character op[PROSAE_TECTUM];

            _liberos_iungere(m->nodus, "optio", FALSUM, piscina, op,
                             (i32)magnitudo(op));
            si (op[ZEPHYRUM])
            {
                imprimere("  (%s)", op);
            }
            alioquin
            {
                imprimere("  [electio]");
            }
        }
        alioquin si (ty[ZEPHYRUM])
        {
            imprimere("  [%s%s%s]", tm[ZEPHYRUM] ? tm : "",
                      tm[ZEPHYRUM] ? "." : "", ty);
        }
        imprimere("\n");
    }
    alioquin si (strcmp(kind, "machina_statuum") == ZEPHYRUM)
    {
        character st[PROSAE_TECTUM];

        _liberos_iungere(m->nodus, "status", VERUM, piscina, st,
                         (i32)magnitudo(st));
        imprimere("  ? %squo statu (%s): %s\n", praefixum, nom,
                  st[ZEPHYRUM] ? st : "-");
    }
    alioquin si (strcmp(kind, "pars") == ZEPHYRUM)
    {
        character ne[NOMINIS_TECTUM];

        _chordam_scribere(
            stml_attributum_capere(m->nodus, "necessaria"), ne,
            (i32)magnitudo(ne));
        imprimere("  ? %sadestne pars '%s'%s\n", praefixum, nom,
                  strcmp(ne, "verum") == ZEPHYRUM ?
                      "  [NECESSARIA]" : "  (optio)");
    }
    alioquin si (strcmp(kind, "relatio") == ZEPHYRUM)
    {
        character ad[NOMINIS_TECTUM];
        character rm[NOMINIS_TECTUM];
        character mx[NOMINIS_TECTUM];

        _chordam_scribere(stml_attributum_capere(m->nodus, "ad"), ad,
                          (i32)magnitudo(ad));
        _chordam_scribere(stml_attributum_capere(m->nodus, "modulus"),
                          rm, (i32)magnitudo(rm));
        _chordam_scribere(
            stml_attributum_capere(m->nodus, "multiplex"), mx,
            (i32)magnitudo(mx));

        imprimere("  ? %s%s -> ", praefixum, nom);
        si (strcmp(ad, "*") == ZEPHYRUM)
        {
            imprimere("(quidlibet)");
        }
        alioquin
        {
            imprimere("%s%s%s", rm[ZEPHYRUM] ? rm : "",
                      rm[ZEPHYRUM] ? "." : "", ad);
        }
        si (strcmp(mx, "verum") == ZEPHYRUM)
        {
            imprimere("  [multiplex]");
        }
        imprimere("\n");
    }
    alioquin si (strcmp(kind, "actio") == ZEPHYRUM)
    {
        imprimere("  ? %sactio '%s'\n", praefixum, nom);
    }
    alioquin
    {
        imprimere("  ? %s%s %s\n", praefixum, kind, nom);
    }
}

/* res nidificatas RECURRENTER scribere (taxinomia Porphyriana:
 * <species isbn> speciem isbn_10 continet, <species
 * malus_domestica> cultivar granny_smith) */
interior vacuum
_nidificatas_scribere(
    NaturaRes*  parens,
    i32         gradus,
    Piscina*    piscina)
{
    i32 i;

    si (!parens->res_suae || gradus > VIII)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(parens->res_suae); i++)
    {
        NaturaRes*          n;
        character           tit[NOMINIS_TECTUM];
        character           sensus[PROSAE_TECTUM];

        n    = *(NaturaRes**)xar_obtinere(parens->res_suae, i);
        _chordam_scribere(n->titulus, tit, (i32)magnitudo(tit));
        _sensum_scribere(n->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));

        imprimere("  %*s: %-*s %.50s\n", (integer)(gradus * II), "",
                  (integer)(25 - gradus * II), tit, sensus);
        _nidificatas_scribere(n, gradus + I, piscina);
    }
}

/* genus dictionarii = subgenus inscriptum (.species/.cultivar) -
 * liberi taxinomici (.genus, sub=) alibi ostenduntur */
interior b32
_genus_dictionarii_est(NaturaGenus* g)
{
    constans character* kind;

    si (!g->nodus || !g->nodus->titulus)
    {
        redde FALSUM;
    }
    kind = (constans character*)g->nodus->titulus->datum;
    redde (b32)(strncmp(kind, ".species", VIII) == ZEPHYRUM ||
                strncmp(kind, ".cultivar", IX) == ZEPHYRUM);
}

/* subgenera dictionarii + individua generis, recursive (arbor
 * porphyriana: liberi generum, res_suae individuorum) */
interior vacuum
_nidificata_genera_scribere(
    NaturaGenus*  parens,
    i32           gradus,
    Piscina*      piscina)
{
    i32 i;

    si (gradus > VIII)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(parens->liberi); i++)
    {
        NaturaGenus* l;
        character    tit[NOMINIS_TECTUM];
        character    sensus[PROSAE_TECTUM];

        l = *(NaturaGenus**)xar_obtinere(parens->liberi, i);
        si (!_genus_dictionarii_est(l))
        {
            perge;
        }
        _chordam_scribere(l->titulus, tit, (i32)magnitudo(tit));
        _sensum_scribere(l->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));
        imprimere("  %*s> %-*s %.50s\n", (integer)(gradus * II), "",
                  (integer)(25 - gradus * II), tit, sensus);
        _nidificata_genera_scribere(l, gradus + I, piscina);
    }
    per (i = ZEPHYRUM; i < xar_numerus(parens->res_suae); i++)
    {
        NaturaRes* n;
        character  tit[NOMINIS_TECTUM];
        character  sensus[PROSAE_TECTUM];

        n = *(NaturaRes**)xar_obtinere(parens->res_suae, i);
        imprimere("  %*s", (integer)(gradus * II), "");
        _chordam_scribere(n->titulus, tit, (i32)magnitudo(tit));
        _sensum_scribere(n->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));
        imprimere(": %-*s %.50s\n",
                  (integer)(25 - gradus * II), tit, sensus);
        _nidificatas_scribere(n, gradus + I, piscina);
    }
}

interior vacuum
_species_scribere(
    NaturaGenus*  genus,
    Piscina*      piscina)
{
    i32 i;
    i32 quot;

    quot = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(genus->liberi); i++)
    {
        NaturaGenus* l;

        l = *(NaturaGenus**)xar_obtinere(genus->liberi, i);
        si (_genus_dictionarii_est(l))
        {
            quot++;
        }
    }
    quot += xar_numerus(genus->res_suae);
    si (quot == ZEPHYRUM)
    {
        redde;
    }

    imprimere("\n--- QUAE SPECIES SIT? ---\n");
    per (i = ZEPHYRUM; i < xar_numerus(genus->liberi); i++)
    {
        NaturaGenus* l;
        character    tit[NOMINIS_TECTUM];
        character    sensus[PROSAE_TECTUM];

        l = *(NaturaGenus**)xar_obtinere(genus->liberi, i);
        si (!_genus_dictionarii_est(l))
        {
            perge;
        }
        _chordam_scribere(l->titulus, tit, (i32)magnitudo(tit));
        _sensum_scribere(l->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));
        imprimere("   %-26s %.60s\n", tit, sensus);
        _nidificata_genera_scribere(l, I, piscina);
    }
    per (i = ZEPHYRUM; i < xar_numerus(genus->res_suae); i++)
    {
        NaturaRes* r;
        character  tit[NOMINIS_TECTUM];
        character  sensus[PROSAE_TECTUM];

        r = *(NaturaRes**)xar_obtinere(genus->res_suae, i);
        /* nidificatas hic PRAETERIT: sub continente sua infra
         * scribuntur, ne bis appareant */
        si (r->continens)
        {
            perge;
        }
        _chordam_scribere(r->titulus, tit, (i32)magnitudo(tit));
        _sensum_scribere(r->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));
        imprimere("  :%-26s %.60s\n", tit, sensus);
        _nidificatas_scribere(r, I, piscina);
    }
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

    si (machina)
    {
        per (i = ZEPHYRUM; i < xar_numerus(membra); i++)
        {
            NaturaApparatusMembrum* m;
            character               nom[NOMINIS_TECTUM];
            character               auc[NOMINIS_TECTUM];

            m = (NaturaApparatusMembrum*)xar_obtinere(membra, i);
            si (m->nodus->titulus &&
                chorda_aequalis_literis(*m->nodus->titulus, "nota"))
            {
                perge;  /* margo curatorialis, non membrum */
            }
            /* terminus munere nominatur, non nomine */
            _chordam_scribere(
                stml_attributum_capere(m->nodus,
                    (m->nodus->titulus &&
                     chorda_aequalis_literis(*m->nodus->titulus,
                                             "terminus"))
                        ? "munus" : "nomen"),
                nom, (i32)magnitudo(nom));
            auc[ZEPHYRUM] = '\0';
            si (m->auctor)
            {
                _chordam_scribere(m->auctor->titulus, auc,
                                  (i32)magnitudo(auc));
            }
            imprimere("%.*s\t%s\t%s\n",
                      m->nodus->titulus ?
                          (integer)m->nodus->titulus->mensura : I,
                      m->nodus->titulus ?
                          (constans character*)m->nodus->titulus->datum
                          : "?",
                      nom, auc);
        }
        redde;
    }

    {
        character sensus[PROSAE_TECTUM];
        Xar*      catena;

        imprimere("==================================================\n");
        imprimere("  /%.*s/%.*s\n",
                  (integer)genus->modulus->mensura,
                  (constans character*)genus->modulus->datum,
                  (integer)genus->titulus->mensura,
                  (constans character*)genus->titulus->datum);
        imprimere("==================================================\n");

        _sensum_scribere(genus->nodus, piscina, sensus,
                         (i32)magnitudo(sensus));
        si (sensus[ZEPHYRUM])
        {
            imprimere("\n%s\n", sensus);
        }

        /* IN ARBORE: a radice ad se ipsum */
        catena = natura_maiores(genus, piscina);
        si (xar_numerus(catena) > ZEPHYRUM)
        {
            s32 k;   /* DESCENDENS: i32 insignatus est, ergo
                      * 'k >= 0' semper verum esset (laqueus
                      * infinitus). xar_obtinere_s pro decrementis
                      * factum est. Custos hoc cepit dum scribo. */

            imprimere("\nIN ARBORE: ");
            per (k = (s32)xar_numerus(catena) - I; k >= ZEPHYRUM;
                 k--)
            {
                NaturaGenus* g;

                g = *(NaturaGenus**)xar_obtinere_s(catena, k);
                imprimere("%.*s.%.*s > ",
                          (integer)g->modulus->mensura,
                          (constans character*)g->modulus->datum,
                          (integer)g->titulus->mensura,
                          (constans character*)g->titulus->datum);
            }
            imprimere("%.*s.%.*s\n",
                      (integer)genus->modulus->mensura,
                      (constans character*)genus->modulus->datum,
                      (integer)genus->titulus->mensura,
                      (constans character*)genus->titulus->datum);
        }
    }

    imprimere("\n--- QUAE ROGARE DEBEAS (apparatus, cum "
              "hereditate) ---\n");
    per (i = ZEPHYRUM; i < xar_numerus(membra); i++)
    {
        _membrum_scribere(
            (NaturaApparatusMembrum*)xar_obtinere(membra, i),
            genus, piscina);
    }

    /* TERMINI - compages actus ut index integer legitur ("quis
     * emit? quis vendit? quid? quanti?"), ideo bloccus proprius,
     * non lineae inter cetera sparsae */
    {
        i32 quot_terminorum;

        quot_terminorum = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(membra); i++)
        {
            NaturaApparatusMembrum* m;

            m = (NaturaApparatusMembrum*)xar_obtinere(membra, i);
            si (m->nodus->titulus &&
                chorda_aequalis_literis(*m->nodus->titulus,
                                        "terminus"))
            {
                quot_terminorum++;
            }
        }

        si (quot_terminorum > ZEPHYRUM)
        {
            imprimere("\n--- TERMINI (compages actus) ---\n");
            per (i = ZEPHYRUM; i < xar_numerus(membra); i++)
            {
                NaturaApparatusMembrum* m;
                character               mun[NOMINIS_TECTUM];
                character               ad[NOMINIS_TECTUM];
                character               rm[NOMINIS_TECTUM];
                character               nex[NOMINIS_TECTUM];
                character               mx[NOMINIS_TECTUM];
                character               ne[NOMINIS_TECTUM];

                m = (NaturaApparatusMembrum*)xar_obtinere(membra,
                                                          i);
                si (!m->nodus->titulus ||
                    !chorda_aequalis_literis(*m->nodus->titulus,
                                             "terminus"))
                {
                    perge;
                }
                _chordam_scribere(
                    stml_attributum_capere(m->nodus, "munus"),
                    mun, (i32)magnitudo(mun));
                _chordam_scribere(
                    stml_attributum_capere(m->nodus, "ad"),
                    ad, (i32)magnitudo(ad));
                _chordam_scribere(
                    stml_attributum_capere(m->nodus, "modulus"),
                    rm, (i32)magnitudo(rm));
                _chordam_scribere(
                    stml_attributum_capere(m->nodus,
                                           "necessitudo"),
                    nex, (i32)magnitudo(nex));
                _chordam_scribere(
                    stml_attributum_capere(m->nodus, "multiplex"),
                    mx, (i32)magnitudo(mx));
                _chordam_scribere(
                    stml_attributum_capere(m->nodus, "necessaria"),
                    ne, (i32)magnitudo(ne));

                imprimere("  ? quis '%s' stet -> ", mun);
                si (!ad[ZEPHYRUM] ||
                    strcmp(ad, "*") == ZEPHYRUM)
                {
                    imprimere("(quidlibet)");
                }
                alioquin
                {
                    imprimere("%s%s%s", rm[ZEPHYRUM] ? rm : "",
                              rm[ZEPHYRUM] ? "." : "", ad);
                }
                si (nex[ZEPHYRUM])
                {
                    imprimere("  [necessitudo %s]", nex);
                }
                si (strcmp(mx, "verum") == ZEPHYRUM)
                {
                    imprimere("  [multiplex]");
                }
                si (strcmp(ne, "verum") == ZEPHYRUM)
                {
                    imprimere("  [NECESSARIA]");
                }
                si (m->auctor != genus && m->auctor &&
                    m->auctor->titulus)
                {
                    character ag[NOMINIS_TECTUM];

                    _chordam_scribere(m->auctor->titulus, ag,
                                      (i32)magnitudo(ag));
                    imprimere("  [a %s]", ag);
                }
                imprimere("\n");
            }
        }
    }

    _species_scribere(genus, piscina);
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

/* ==================================================
 * relationes / census - mensurae structurales
 *
 * CUR IN INSTRUMENTO, NON IN CRUSTA (occasio 2026-08-08):
 * mensurae hae per grep/awk/sed factae sunt, et TER mentitae:
 * (i) machina awk tag speciei se-claudente decepta est, bis;
 * (ii) 'cut -c9-' nomina truncavit quia campus numeri latitudine
 * variat. Nemo mendacium vidit nisi quia numeri absurdi erant.
 * Instrumentum arborem PARSATAM ambulat: eas culpas facere non
 * potest. Semantica (in quas familias nomina cadant) hominis
 * manet - hic numeri soli, sed VERI.
 * ================================================== */

/* membra directa unius blocci (NON recursive: genera nidificata
 * sua propria habent, aliter bis numerarentur) */
interior Xar*
_membra_directa(
    NaturaGenus*         genus,
    constans character*  bloccus,
    constans character*  elementum,
    Piscina*             piscina)
{
    Xar*        exitus;
    StmlNodus*  continens;
    i32         n;
    i32         i;

    exitus = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    si (!genus->nodus)
    {
        redde exitus;
    }
    continens = stml_invenire_liberum(genus->nodus, bloccus);
    si (!continens)
    {
        redde exitus;
    }
    n = stml_numerus_liberorum(continens);
    per (i = ZEPHYRUM; i < n; i++)
    {
        StmlNodus* m;

        m = stml_liberum_ad_indicem(continens, i);
        si (m && m->genus == STML_NODUS_ELEMENTUM && m->titulus &&
            chorda_aequalis_literis(*m->titulus, elementum))
        {
            *(StmlNodus**)xar_addere(exitus) = m;
        }
    }
    redde exitus;
}

/* sedes una (relatio aut terminus) ut ordo scribitur. Columna
 * petiti a= quoque legit (sedes inversae aliter petitum suum
 * celabant - cohors XVIII conversorum, metrum ligaminis primum);
 * columna VII 'ligata'/'soluta' APPENSA (formatum -machina
 * append-only: consumptores columnas priores asserunt). */
interior vacuum
_sedem_relationis_scribere(
    NaturaBibliotheca*   bib,
    b32                  machina,
    constans character*  gm,
    constans character*  gn,
    StmlNodus*           r,
    b32                  terminus_est)
{
    character           rn[NOMINIS_TECTUM];
    character           ad[NOMINIS_TECTUM];
    character           rm[NOMINIS_TECTUM];
    character           mx[NOMINIS_TECTUM];
    character           nex[NOMINIS_TECTUM];
    constans character* ligamen;

    _chordam_scribere(
        stml_attributum_capere(r, terminus_est ? "munus" : "nomen"),
        rn, (i32)magnitudo(rn));
    _chordam_scribere(stml_attributum_capere(r, "ad"), ad,
                      (i32)magnitudo(ad));
    si (!ad[ZEPHYRUM])
    {
        _chordam_scribere(stml_attributum_capere(r, "a"), ad,
                          (i32)magnitudo(ad));
    }
    _chordam_scribere(stml_attributum_capere(r, "modulus"),
                      rm, (i32)magnitudo(rm));
    _chordam_scribere(stml_attributum_capere(r, "multiplex"),
                      mx, (i32)magnitudo(mx));
    _chordam_scribere(stml_attributum_capere(r, "necessitudo"),
                      nex, (i32)magnitudo(nex));

    si (nex[ZEPHYRUM])
    {
        ligamen = natura_necessitudo(bib, nex) ? "ligata"
                                               : "soluta";
    }
    alioquin
    {
        ligamen = natura_necessitudo(bib, rn) ? "ligata"
                                              : "soluta";
    }

    si (machina)
    {
        imprimere("%s\t%s\t%s\t%s\t%s\t%s\t%s\n", gm, gn, rn,
                  strcmp(mx, "verum") == ZEPHYRUM ?
                      "multiplex" : "unum",
                  rm[ZEPHYRUM] ? rm : gm,
                  ad[ZEPHYRUM] ? ad : "-",
                  ligamen);
    }
    alioquin
    {
        imprimere("%-22s %-26s %-24s %-6s %-7s %s%s%s\n", gm, gn,
                  rn,
                  strcmp(mx, "verum") == ZEPHYRUM ?
                      "*" : "-",
                  ligamen,
                  rm[ZEPHYRUM] ? rm : "",
                  rm[ZEPHYRUM] ? "." : "",
                  ad[ZEPHYRUM] ? ad : "-");
    }
}

interior vacuum
_relationes_scribere(
    NaturaBibliotheca*  bib,
    b32                 machina,
    Piscina*            piscina)
{
    i32 g;

    si (!machina)
    {
        imprimere("%-22s %-26s %-24s %-6s %-7s %s\n", "MODULUS",
                  "GENUS", "RELATIO", "MULT", "LIG", "AD");
    }
    per (g = ZEPHYRUM; g < xar_numerus(bib->genera_omnia); g++)
    {
        NaturaGenus* genus;
        Xar*         sedes;
        i32          i;
        character    gm[NOMINIS_TECTUM];
        character    gn[NOMINIS_TECTUM];

        genus = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, g);
        _chordam_scribere(genus->modulus, gm, (i32)magnitudo(gm));
        _chordam_scribere(genus->titulus, gn, (i32)magnitudo(gn));

        sedes = _membra_directa(genus, "relationes", "relatio",
                                piscina);
        per (i = ZEPHYRUM; i < xar_numerus(sedes); i++)
        {
            _sedem_relationis_scribere(bib, machina, gm, gn,
                *(StmlNodus**)xar_obtinere(sedes, i), FALSUM);
        }

        sedes = _membra_directa(genus, "termini", "terminus",
                                piscina);
        per (i = ZEPHYRUM; i < xar_numerus(sedes); i++)
        {
            _sedem_relationis_scribere(bib, machina, gm, gn,
                *(StmlNodus**)xar_obtinere(sedes, i), VERUM);
        }
    }
}

interior vacuum
_censum_scribere(
    NaturaBibliotheca*  bib,
    b32                 machina,
    Piscina*            piscina)
{
    Xar* nomina_relationum;
    i32  g;
    i32  i;
    i32  quot_relationum;
    i32  quot_apertarum;
    i32  quot_inversarum;
    i32  quot_ligatarum;
    i32  quot_solutarum;
    i32  quot_terminorum;
    i32  quot_proprietatum;
    i32  quot_partium;
    i32  quot_actionum;
    i32  quot_machinarum;
    i32  quot_subgenerum;
    i32  quot_specierum;
    i32  quot_individuorum;
    i32  quot_cultivarum;
    i32  quot_nidificatarum;
    i32  distincta;
    i32  semel_adhibita;

    nomina_relationum = xar_creare(piscina,
                                   (i32)magnitudo(character*));
    quot_relationum   = ZEPHYRUM;
    quot_apertarum    = ZEPHYRUM;
    quot_inversarum   = ZEPHYRUM;
    quot_ligatarum    = ZEPHYRUM;
    quot_solutarum    = ZEPHYRUM;
    quot_terminorum   = ZEPHYRUM;
    quot_proprietatum = ZEPHYRUM;
    quot_partium      = ZEPHYRUM;
    quot_actionum     = ZEPHYRUM;
    quot_machinarum   = ZEPHYRUM;
    quot_subgenerum   = ZEPHYRUM;

    per (g = ZEPHYRUM; g < xar_numerus(bib->genera_omnia); g++)
    {
        NaturaGenus* genus;
        Xar*         membra;

        genus = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, g);
        si (genus->parens)
        {
            quot_subgenerum++;
        }

        membra = _membra_directa(genus, "relationes", "relatio",
                                 piscina);
        per (i = ZEPHYRUM; i < xar_numerus(membra); i++)
        {
            StmlNodus* r;
            character  rn[NOMINIS_TECTUM];
            chorda*    ad;
            chorda*    inv;

            r = *(StmlNodus**)xar_obtinere(membra, i);
            quot_relationum++;

            ad = stml_attributum_capere(r, "ad");
            si (ad && chorda_aequalis_literis(*ad, "*"))
            {
                quot_apertarum++;
            }
            inv = stml_attributum_capere(r, "inversa");
            si (inv && chorda_aequalis_literis(*inv, "verum"))
            {
                quot_inversarum++;
            }
            _chordam_scribere(stml_attributum_capere(r, "nomen"), rn,
                              (i32)magnitudo(rn));
            si (rn[ZEPHYRUM])
            {
                *(character**)xar_addere(nomina_relationum) =
                    (character*)chorda_ut_cstr(
                        chorda_ex_literis(rn, piscina), piscina);
            }

            /* metrum ligaminis (necessitudines): citatio
             * explicita aut praesumptio nomine */
            {
                character nex[NOMINIS_TECTUM];

                _chordam_scribere(
                    stml_attributum_capere(r, "necessitudo"), nex,
                    (i32)magnitudo(nex));
                si ((nex[ZEPHYRUM]
                         ? natura_necessitudo(bib, nex)
                         : natura_necessitudo(bib, rn)) != NIHIL)
                {
                    quot_ligatarum++;
                }
                alioquin
                {
                    quot_solutarum++;
                }
            }
        }

        /* termini sedes relationum quoque sunt (munere nominatae,
         * nomina eorum NON in vocabularium nominum - munus locale
         * compagi est) */
        membra = _membra_directa(genus, "termini", "terminus",
                                 piscina);
        per (i = ZEPHYRUM; i < xar_numerus(membra); i++)
        {
            StmlNodus* r;
            character  rn[NOMINIS_TECTUM];
            character  nex[NOMINIS_TECTUM];

            r = *(StmlNodus**)xar_obtinere(membra, i);
            quot_terminorum++;
            _chordam_scribere(stml_attributum_capere(r, "munus"),
                              rn, (i32)magnitudo(rn));
            _chordam_scribere(
                stml_attributum_capere(r, "necessitudo"), nex,
                (i32)magnitudo(nex));
            si ((nex[ZEPHYRUM]
                     ? natura_necessitudo(bib, nex)
                     : natura_necessitudo(bib, rn)) != NIHIL)
            {
                quot_ligatarum++;
            }
            alioquin
            {
                quot_solutarum++;
            }
        }

        quot_proprietatum += xar_numerus(
            _membra_directa(genus, "proprietates", "proprietas",
                            piscina));
        quot_partium += xar_numerus(
            _membra_directa(genus, "partes", "pars", piscina));
        quot_actionum += xar_numerus(
            _membra_directa(genus, "actiones", "actio", piscina));
        {
            i32 k;
            i32 n;

            n = genus->nodus ?
                    stml_numerus_liberorum(genus->nodus) : ZEPHYRUM;
            per (k = ZEPHYRUM; k < n; k++)
            {
                StmlNodus* m;

                m = stml_liberum_ad_indicem(genus->nodus, k);
                si (m && m->genus == STML_NODUS_ELEMENTUM &&
                    m->titulus &&
                    chorda_aequalis_literis(*m->titulus,
                                            "machina_statuum"))
                {
                    quot_machinarum++;
                }
            }
        }
    }

    /* nomina distincta et SEMEL adhibita (indicium 'plura
     * specialia, nullum generale' in stratum relationum) */
    distincta      = ZEPHYRUM;
    semel_adhibita = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(nomina_relationum); i++)
    {
        constans character* hoc;
        i32                 k;
        i32                 quot;
        b32                 primum;

        hoc    = *(character**)xar_obtinere(nomina_relationum, i);
        quot   = ZEPHYRUM;
        primum = VERUM;
        per (k = ZEPHYRUM; k < xar_numerus(nomina_relationum); k++)
        {
            si (strcmp(*(character**)xar_obtinere(
                           nomina_relationum, k), hoc) == ZEPHYRUM)
            {
                quot++;
                si (k < i)
                {
                    primum = FALSUM;
                }
            }
        }
        si (primum)
        {
            distincta++;
            si (quot == I)
            {
                semel_adhibita++;
            }
        }
    }

    quot_specierum     = ZEPHYRUM;
    quot_individuorum  = ZEPHYRUM;
    quot_cultivarum    = ZEPHYRUM;
    quot_nidificatarum = ZEPHYRUM;
    /* species/cultivares GENERA sunt post arborem porphyrianam -
     * numerantur e genera_omnia per titulum elementi */
    per (i = ZEPHYRUM; i < xar_numerus(bib->genera_omnia); i++)
    {
        NaturaGenus*        g;
        constans character* kind;

        g    = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
        kind = (g->nodus && g->nodus->titulus) ?
                   (constans character*)g->nodus->titulus->datum : "?";
        si (strncmp(kind, ".species", VIII) == ZEPHYRUM)
        {
            quot_specierum++;
        }
        alioquin si (strncmp(kind, ".cultivar", IX) == ZEPHYRUM)
        {
            quot_cultivarum++;
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(bib->res_omnes); i++)
    {
        NaturaRes*          r;
        constans character* kind;

        r    = *(NaturaRes**)xar_obtinere(bib->res_omnes, i);
        kind = (r->nodus && r->nodus->titulus) ?
                   (constans character*)r->nodus->titulus->datum : "?";
        si (strncmp(kind, "individuum", X) == ZEPHYRUM)
        {
            quot_individuorum++;
        }
        si (r->continens)
        {
            quot_nidificatarum++;
        }
    }

    si (machina)
    {
        imprimere("exemplaria\t%d\n",
                  (integer)xar_numerus(bib->exemplaria));
        imprimere("genera\t%d\n",
                  (integer)xar_numerus(bib->genera_omnia));
        imprimere("subgenera\t%d\n", (integer)quot_subgenerum);
        imprimere("species\t%d\n", (integer)quot_specierum);
        imprimere("individua\t%d\n", (integer)quot_individuorum);
        imprimere("cultivares\t%d\n", (integer)quot_cultivarum);
        imprimere("res_nidificatae\t%d\n",
                  (integer)quot_nidificatarum);
        imprimere("relationes\t%d\n", (integer)quot_relationum);
        imprimere("relationes_nomina_distincta\t%d\n",
                  (integer)distincta);
        imprimere("relationes_nomina_semel\t%d\n",
                  (integer)semel_adhibita);
        imprimere("relationes_apertae\t%d\n", (integer)quot_apertarum);
        imprimere("relationes_inversae\t%d\n",
                  (integer)quot_inversarum);
        imprimere("relationes_ligatae\t%d\n",
                  (integer)quot_ligatarum);
        imprimere("relationes_solutae\t%d\n",
                  (integer)quot_solutarum);
        imprimere("termini\t%d\n", (integer)quot_terminorum);
        imprimere("necessitudines\t%d\n",
                  (integer)xar_numerus(bib->necessitudines_omnes));
        imprimere("proprietates\t%d\n", (integer)quot_proprietatum);
        imprimere("partes\t%d\n", (integer)quot_partium);
        imprimere("actiones\t%d\n", (integer)quot_actionum);
        imprimere("machinae\t%d\n", (integer)quot_machinarum);
        redde;
    }

    imprimere("CENSUS BIBLIOTHECAE\n\n");
    imprimere("  exemplaria           %5d\n",
              (integer)xar_numerus(bib->exemplaria));
    imprimere("  genera               %5d  (sub alio: %d)\n",
              (integer)xar_numerus(bib->genera_omnia),
              (integer)quot_subgenerum);
    imprimere("  species              %5d\n", (integer)quot_specierum);
    imprimere("  individua            %5d\n",
              (integer)quot_individuorum);
    imprimere("  cultivares           %5d\n",
              (integer)quot_cultivarum);
    imprimere("  res nidificatae      %5d  (intra aliam rem)\n",
              (integer)quot_nidificatarum);
    imprimere("\n  APPARATUS\n");
    imprimere("  proprietates         %5d\n",
              (integer)quot_proprietatum);
    imprimere("  partes               %5d\n", (integer)quot_partium);
    imprimere("  actiones             %5d\n", (integer)quot_actionum);
    imprimere("  machinae statuum     %5d\n",
              (integer)quot_machinarum);
    imprimere("\n  RELATIONES\n");
    imprimere("  relationes           %5d\n",
              (integer)quot_relationum);
    imprimere("  nomina distincta     %5d\n", (integer)distincta);
    imprimere("  nomina SEMEL adhibita%5d", (integer)semel_adhibita);
    si (distincta > ZEPHYRUM)
    {
        imprimere("  (%d%% nominum)",
                  (integer)((semel_adhibita * C) / distincta));
    }
    imprimere("\n");
    imprimere("  clausurae APERTAE    %5d  (ad=\"*\")\n",
              (integer)quot_apertarum);
    imprimere("  inversae declaratae  %5d\n",
              (integer)quot_inversarum);
    imprimere("\n  NECESSITUDINES\n");
    imprimere("  declaratae           %5d\n",
              (integer)xar_numerus(bib->necessitudines_omnes));
    imprimere("  sedes ligatae        %5d\n",
              (integer)quot_ligatarum);
    imprimere("  sedes solutae        %5d  (metrum deustionis)\n",
              (integer)quot_solutarum);
    imprimere("  termini compagum     %5d\n",
              (integer)quot_terminorum);
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
        "  census              mensurae structurales corporis\n"
        "                      (+ ligatae/solutae, necessitudines)\n"
        "  relationes          sedes OMNES (relatio + terminus;\n"
        "                      columna VII ligata/soluta = metrum\n"
        "                      deustionis nominum insolutorum)\n"
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

    si (strcmp(mandatum, "census") == ZEPHYRUM)
    {
        _censum_scribere(bib, machina, piscina);
        redde ZEPHYRUM;
    }

    si (strcmp(mandatum, "relationes") == ZEPHYRUM)
    {
        _relationes_scribere(bib, machina, piscina);
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
