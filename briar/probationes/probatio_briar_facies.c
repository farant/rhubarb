/* probatio_briar_facies.c - Pagina litterata (spec par. 4.6): corpus
 * genitum contra AURUM (fixa/facies/) octetim ET invariantia
 * structuralia super lexin HTML veram (html_lexare, non grep).
 *
 * Vestis PROBATORIA hic datur - notae solae, sine stylis, sine
 * scripto: ergo aurea CORPUS tenent, et CSS verum (briar/facies/)
 * ea movere non potest.
 *
 * Regeneratio auri: BRIAR_FACIES_SCRIBERE=1 cum causa nominata.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_contextus.h"
#include "briar_fabrica.h"
#include "briar_facies.h"
#include "briar_nexus.h"
#include "briar_silva.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "html_lexema.h"
#include "internamentum.h"
#include "piscina.h"
#include "silex.h"
#include "via.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* AURUM =
    "briar/probationes/fixa/facies/";
hic_manens constans character* FIXA =
    "briar/probationes/fixa/thistle/";

/* vestis probatoria: notae solae */
hic_manens constans character* INVOLUCRUM_PROBATORIUM =
    "<!DOCTYPE html>\n"
    "<title><!--@titulus--></title>\n"
    "<style><!--@styli--></style>\n"
    "<!--@caput-->\n"
    "<!--@corpus-->\n"
    "<!--@symbola-->\n"
    "<script><!--@scriptum--></script>\n";


/* ==================================================
 * Adiumenta
 * ================================================== */

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior constans character*
_texere (
               Piscina* piscina,
    constans character* a,
    constans character* b)
{
    redde chorda_ut_cstr(chorda_concatenare(
        chorda_ex_literis(a, piscina),
        chorda_ex_literis(b, piscina), piscina), piscina);
}

/* causam ut in pagina apparet: evasa. Causae '->' et '<<#x>>' ferunt,
 * ergo comparatio cruda semper falleret - et evasio ipsa probanda
 * est, non praetereunda */
interior constans character*
_evasum (
               Piscina* piscina,
                chorda  t)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)256);
                   i32 k;

    per (k = ZEPHYRUM; k < t.mensura; k++)
    {
        character c = (character)t.datum[k];

        si (c == '&')
        {
            chorda_aedificator_appendere_literis(a, "&amp;");
        }
        alioquin si (c == '<')
        {
            chorda_aedificator_appendere_literis(a, "&lt;");
        }
        alioquin si (c == '>')
        {
            chorda_aedificator_appendere_literis(a, "&gt;");
        }
        alioquin si (c == '"')
        {
            chorda_aedificator_appendere_literis(a, "&quot;");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, c);
        }
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(a), piscina);
}

interior s32
_numerare (
               Piscina* piscina,
                chorda  fenum,
    constans character* acus)
{
     constans character* c        = chorda_ut_cstr(fenum, piscina);
                    s32  n        = ZEPHYRUM;
                 size_t  mensura  = strlen(acus);

    dum ((c = strstr(c, acus)) != NIHIL)
    {
        n = n + I;
        c = c + mensura;
    }
    redde n;
}

interior b32
_continet (
               Piscina* piscina,
                chorda  fenum,
    constans character* acus)
{
    redde (b32)(strstr(chorda_ut_cstr(fenum, piscina), acus) != NIHIL);
}

/* lineae 'CASUS' plagulae exemplaris, ordine, detonsae */
interior Xar*
_lineae_casuum (
    Piscina* piscina,
     chorda  textus)
{
    Xar* lineae = xar_creare(piscina, magnitudo(chorda));
    i32  cursor = ZEPHYRUM;

    dum (cursor < textus.mensura)
    {
           i32 finis = cursor;
        chorda linea;

        dum (finis < textus.mensura && textus.datum[finis] != (i8)'\n')
        {
            finis = finis + I;
        }
        linea.datum    = textus.datum + cursor;
        linea.mensura  = finis - cursor;
        dum (linea.mensura > ZEPHYRUM && linea.datum[0] == (i8)' ')
        {
            linea.datum    = linea.datum + I;
            linea.mensura  = linea.mensura - I;
        }
        si (_continet(piscina, linea, "<CASUS"))
        {
            *(chorda*)xar_addere(lineae) = linea;
        }
        cursor = (finis < textus.mensura) ? finis + I : finis;
    }
    redde lineae;
}

/* plagulam thistle in paginam vertere; nexus/fragmenta redduntur */
interior chorda
_paginam_fingere (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans SilexFons* fons,
     constans character* via,
                   Xar** nexus_out,
                   Xar** fragmenta_out,
                 chorda* causa)
{
     BriarFabricaFructus  fructus;
    BriarFabricaOptiones  optiones;
             BriarVestis  vestis;
               character* textus;
                     i32  mensura = ZEPHYRUM;
           MateriaNodus* doc;
                    Xar* nexus;
                    Xar* fragmenta = NIHIL;
                  chorda octeti;
                  chorda vacua;

    vacua.datum     = NIHIL;
    vacua.mensura   = ZEPHYRUM;
    *nexus_out      = NIHIL;
    *fragmenta_out  = NIHIL;
    causa->datum    = NIHIL;
    causa->mensura  = ZEPHYRUM;
    textus          = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        *causa = chorda_ex_literis("plagula non lecta", piscina);
        redde vacua;
    }
    doc    = briar_arbor_parsare(piscina, textus, mensura);
    nexus  = briar_nexus_texere(piscina, doc, intern);
    (vacuum)briar_contexere(piscina, nexus, &fragmenta);
    (vacuum)briar_silvam_texere(piscina, nexus, fons);
    optiones.via_thistle   = via;
    optiones.stampa        = "probatio";
    optiones.fons_titulus  = "probatio";
    octeti.datum           = (i8*)textus;
    octeti.mensura         = mensura;
    fructus = briar_fabricare(piscina, doc, nexus, fons, &optiones,
        octeti);
    *nexus_out      = nexus;
    *fragmenta_out  = fragmenta;

    vestis.involucrum = chorda_ex_literis(INVOLUCRUM_PROBATORIUM,
        piscina);
    vestis.styli     = vacua;
    vestis.scriptum  = vacua;
    vestis.exemplar   = filum_legere_totum(
        "briar/facies/md-html-facies.stml", piscina);
    CREDO_VERUM (vestis.exemplar.mensura > ZEPHYRUM);
    redde briar_faciem_fingere(piscina, intern, nexus, fragmenta,
        &fructus, octeti, via, &vestis, causa);
}


/* ==================================================
 * Census structuralis (super lexin HTML veram)
 * ================================================== */

nomen structura {
    i32 ids;                /* id "l" cum numero lineae */
    i32 ids_gemini;         /* idem id bis */
    i32 sedes_fragmentorum;         /* id "frag-" cum titulo */
    i32 nexus_fragmentorum;         /* href ad "#frag-" */
    i32 nexus_pendentes;    /* sine sede pari */
    i32 data_s;             /* attributum data-s */
    i32 data_s_absentia;    /* clavis in insula symbolorum deest */
    i32 contexta;            /* elementa details: textus contexti */
} FaciesCensus;

/* citationes demere: valor lexematis eas visibiles fert */
interior chorda
_valorem_nudare (
    chorda v)
{
    si (   v.mensura >= II
        && (v.datum[0] == (i8)'"' || v.datum[0] == (i8)'\''))
    {
        v.datum    = v.datum + I;
        v.mensura  = v.mensura - II;
    }
    redde v;
}

interior b32
_xar_habet (
     constans Xar* xar,
           chorda  clavis)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(xar); i++)
    {
        si (chorda_aequalis(*(constans chorda*)xar_obtinere(xar, i),
            clavis))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior FaciesCensus
_censere (
    Piscina* piscina,
     chorda  pagina)
{
    FaciesCensus  c;
             Xar* lexemata;
             Xar* ids;
             Xar* sedes;
             Xar* nexus;
             i32  i;
          chorda  nomen_ultimum;

    memset(&c, ZEPHYRUM, magnitudo(c));
    nomen_ultimum.datum    = NIHIL;
    nomen_ultimum.mensura  = ZEPHYRUM;
    ids                    = xar_creare(piscina, magnitudo(chorda));
    sedes                  = xar_creare(piscina, magnitudo(chorda));
    nexus                  = xar_creare(piscina, magnitudo(chorda));
    lexemata = html_lexare(piscina,
        (constans character*)pagina.datum, pagina.mensura);
    si (lexemata == NIHIL)
    {
        redde c;
    }
    per (i = ZEPHYRUM; i < xar_numerus(lexemata); i++)
    {
        constans HtmlLexema* l = (constans HtmlLexema*)xar_obtinere(
            lexemata, i);

        si (l->genus == HTML_LEX_TAG_APERTURA)
        {
            si (chorda_aequalis_literis(l->valor, "<details"))
            {
                c.contexta = c.contexta + I;
            }
        }
        alioquin si (l->genus == HTML_LEX_ATTRIBUTUM_NOMEN)
        {
            nomen_ultimum = l->valor;
        }
        alioquin si (l->genus == HTML_LEX_ATTRIBUTUM_VALOR)
        {
            chorda v = _valorem_nudare(l->valor);

            si (chorda_aequalis_literis(nomen_ultimum, "id"))
            {
                si (v.mensura > I && v.datum[0] == (i8)'l')
                {
                    si (_xar_habet(ids, v))
                    {
                        c.ids_gemini = c.ids_gemini + I;
                    }
                    *(chorda*)xar_addere(ids)  = v;
                    c.ids                      = c.ids + I;
                }
                alioquin si (chorda_incipit(v,
                             chorda_ex_literis("frag-", piscina)))
                {
                    *(chorda*)xar_addere(sedes) = v;
                    c.sedes_fragmentorum =
                        c.sedes_fragmentorum + I;
                }
            }
            alioquin si (chorda_aequalis_literis(nomen_ultimum, "href")
                         && chorda_incipit(v,
                         chorda_ex_literis("#frag-",
                         piscina)))
            {
                v.datum                      = v.datum + I;
                v.mensura                    = v.mensura - I;
                *(chorda*)xar_addere(nexus)  = v;
                c.nexus_fragmentorum         =
                    c.nexus_fragmentorum + I;
            }
            alioquin si (chorda_aequalis_literis(nomen_ultimum,
                         "data-s"))
            {
                c.data_s = c.data_s + I;
            }
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        si (!_xar_habet(sedes, *(chorda*)xar_obtinere(nexus, i)))
        {
            c.nexus_pendentes = c.nexus_pendentes + I;
        }
    }
    redde c;
}

/* pagina contra aurum; BRIAR_FACIES_SCRIBERE=1 aurum scribit */
interior vacuum
_aurum_conferre (
                Piscina* piscina,
                 chorda  pagina,
     constans character* titulus)
{
    constans character* via_auri = _texere(piscina, AURUM, titulus);

    si (getenv("BRIAR_FACIES_SCRIBERE") != NIHIL)
    {
        si (filum_scribere(via_auri, pagina))
        {
            imprimere("  AURUM SCRIPTUM: %s\n", via_auri);
        }
        alioquin
        {
            CREDO_CULPA ("aurum non scriptum");
        }
        redde;
    }
    {
        chorda aurum = filum_legere_totum(via_auri, piscina);

        CREDO_VERUM (aurum.mensura > ZEPHYRUM);
        si (!chorda_aequalis(aurum, pagina))
        {
            imprimere("  DIFFERT: %s (genitum %d octeti, aurum %d)\n",
                via_auri, (integer)pagina.mensura,
                (integer)aurum.mensura);
        }
        CREDO_CHORDA_AEQUALIS (pagina, aurum);
    }
}


/* ==================================================
 * Principale
 * ================================================== */

s32
principale (vacuum)
{
                  b32  praeteritus;
              Piscina* piscina;
  InternamentumChorda* intern;
   constans character* radix;
   constans SilexFons* fons;

    piscina = piscina_generare_dynamicum("probatio_briar_facies",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern  = internamentum_creare(piscina);
    radix   = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    fons = silex_fons_disci(piscina, radix);
    CREDO_NON_NIHIL (fons);

    imprimere("\n--- Probans paginam salve.thistle ---\n");
    {
         Xar* nexus;
         Xar* fragmenta;
      chorda  causa;
      chorda  pagina = _paginam_fingere(piscina, intern, fons,
          _texere(piscina, FIXA, "salve.thistle"), &nexus,
          &fragmenta, &causa);
      FaciesCensus c;

        CREDO_CHORDA_VACUA (causa);
        CREDO_VERUM (pagina.mensura > ZEPHYRUM);
        CREDO_CHORDA_INCIPIT (pagina,
            chorda_ex_literis("<!DOCTYPE html>", piscina));

        /* omnis nota consumpta - nulla '<!--@' superest */
        CREDO_FALSUM (_continet(piscina, pagina, "<!--@"));

        /* caput: via fontis nominata */
        CREDO_VERUM (_continet(piscina, pagina, "salve.thistle"));

        /* prosa per md_html: paragraphus et codex inlineus */
        CREDO_VERUM (_continet(piscina, pagina, "<p>"));
        CREDO_VERUM (_continet(piscina, pagina, "A plain program"));
        CREDO_VERUM (_continet(piscina, pagina,
            "<code>principale</code>"));

        /* lineae C: contentum regionis primae VII..XIV, secundae
         * XVIII..XXIX; tagi ipsi lineas non habent */
        CREDO_VERUM (_continet(piscina, pagina, "id=\"l7\""));
        CREDO_VERUM (_continet(piscina, pagina, "id=\"l14\""));
        CREDO_VERUM (_continet(piscina, pagina, "id=\"l18\""));
        CREDO_VERUM (_continet(piscina, pagina, "id=\"l29\""));
        CREDO_FALSUM (_continet(piscina, pagina, "id=\"l15\""));

        /* evasio: '"salve, munde\n"' citationes evasae */
        CREDO_VERUM (_continet(piscina, pagina, "&quot;"));

        /* census: XX lineae codicis, nulla gemina, nullus nexus
         * pendens; contexta et data-s opera II et III sunt */
        c = _censere(piscina, pagina);
        CREDO_AEQUALIS_S32 ((s32)c.ids, (s32)20);
        CREDO_AEQUALIS_S32 ((s32)c.ids_gemini, (s32)0);
        CREDO_AEQUALIS_S32 ((s32)c.nexus_pendentes, (s32)0);

        _aurum_conferre(piscina, pagina, "salve.html");
    }

    imprimere("\n--- Probans evasionem anguli (fragmenta) ---\n");
    {
         Xar* nexus;
         Xar* fragmenta;
      chorda  causa;
      chorda  pagina = _paginam_fingere(piscina, intern, fons,
          _texere(piscina, FIXA, "fragmenta.thistle"), &nexus,
          &fragmenta, &causa);
      FaciesCensus c;

        CREDO_CHORDA_VACUA (causa);
        /* 'per (i = ZEPHYRUM; i < n; i++)' angulum fert */
        CREDO_VERUM (_continet(piscina, pagina, "&lt;"));
        CREDO_FALSUM (_continet(piscina, pagina, "i < n"));
        c = _censere(piscina, pagina);
        CREDO_MAIOR_S32 ((s32)c.ids, (s32)30);
        CREDO_AEQUALIS_S32 ((s32)c.ids_gemini, (s32)0);
    }

    imprimere("\n--- Probans colorem (salve) ---\n");
    {
         Xar* nexus;
         Xar* fragmenta;
      chorda  causa;
      chorda  pagina = _paginam_fingere(piscina, intern, fons,
          _texere(piscina, FIXA, "salve.thistle"), &nexus,
          &fragmenta, &causa);

        CREDO_CHORDA_VACUA (causa);
        /* linea directivi tota praeprocessor est */
        CREDO_VERUM (_continet(piscina, pagina,
            "ca-sy-praeprocessor"));
        /* literale chordae, clavis Latina, genus domus */
        CREDO_VERUM (_continet(piscina, pagina, "ca-sy-chorda"));
        CREDO_VERUM (_continet(piscina, pagina, "ca-sy-clavis"));
        CREDO_VERUM (_continet(piscina, pagina, "ca-sy-genus"));
        /* 'redde' clavis, non identificator nudus */
        CREDO_VERUM (_continet(piscina, pagina,
            "<span class=\"ca-sy-clavis\">redde</span>"));
        /* 's32' genus */
        CREDO_VERUM (_continet(piscina, pagina,
            "<span class=\"ca-sy-genus\">s32</span>"));
    }

    imprimere("\n--- Probans commentarium bilineum et utf8 ---\n");
    {
         Xar* nexus;
         Xar* fragmenta;
      chorda  causa;
      chorda  pagina = _paginam_fingere(piscina, intern, fons,
          _texere(piscina, FIXA, "facies_utf8.thistle"), &nexus,
          &fragmenta, &causa);

        CREDO_CHORDA_VACUA (causa);
        /* tectum per lineas SECTUM, non demissum: utraque linea
         * commentarii classem suam fert */
        CREDO_AEQUALIS_S32 (_numerare(piscina, pagina,
            "ca-sy-commentarium"), (s32)2);
        /* octeti 'ae' ligati adiacentes manent - nullum tag inter
         * eos (sectio media characteris utf8 eos separaret) */
        CREDO_VERUM (_continet(piscina, pagina,
            "non-ASCII: \303\246 */"));
    }

    imprimere("\n--- Probans grafum fragmentorum ---\n");
    {
         Xar* nexus;
         Xar* fragmenta;
      chorda  causa;
      chorda  pagina = _paginam_fingere(piscina, intern, fons,
          _texere(piscina, FIXA, "fragmenta.thistle"), &nexus,
          &fragmenta, &causa);
      FaciesCensus c;

        CREDO_CHORDA_VACUA (causa);
        /* sedes definitionum quinque */
        CREDO_VERUM (_continet(piscina, pagina,
            "id=\"frag-incrementum\""));
        CREDO_VERUM (_continet(piscina, pagina, "id=\"frag-summa\""));
        CREDO_VERUM (_continet(piscina, pagina, "id=\"frag-capita\""));
        CREDO_VERUM (_continet(piscina, pagina,
            "id=\"frag-salutatio\""));
        CREDO_VERUM (_continet(piscina, pagina,
            "id=\"frag-otiosum\""));
        /* transclusiones nexus sunt: linea XV ad #incrementum */
        CREDO_VERUM (_continet(piscina, pagina,
            "href=\"#frag-incrementum\""));
        /* fragmentum non adhibitum ita dicitur */
        CREDO_VERUM (_continet(piscina, pagina, "non adhibitum"));
        /* radices duae, ergo duo textus contexti */
        c = _censere(piscina, pagina);
        CREDO_AEQUALIS_S32 ((s32)c.contexta, (s32)2);
        CREDO_AEQUALIS_S32 ((s32)c.ids_gemini, (s32)0);
        CREDO_AEQUALIS_S32 ((s32)c.nexus_pendentes, (s32)0);
        /* nexus quinque: capita summa incrementum salutatio bis */
        CREDO_AEQUALIS_S32 ((s32)c.nexus_fragmentorum, (s32)5);
        CREDO_AEQUALIS_S32 ((s32)c.sedes_fragmentorum, (s32)5);

        /* MARGO TEXTI: linea contexta 'summa = summa + numeri[i];'
         * lineam FRAGMENTI (IX) nominat, non indicem compositum -
         * hoc est totum consilium visus contexti */
        CREDO_VERUM (_continet(piscina, pagina,
            "<a class=\"fr-numerus\" href=\"#l9\" data-n=\"9\">"));
        /* et praefixum octo spatiorum (regula noweb) servatur */
        CREDO_VERUM (_continet(piscina, pagina,
            "data-n=\"9\"></a>        summa = summa"));

        _aurum_conferre(piscina, pagina, "fragmenta.html");

        /* CONSENSUS: pagina et '-partes' idem dicunt. Una veritas
         * (grex fragmentorum), duae redditiones; discordia nominat
         * utra mentiatur. */
        {
            i32 j;

            CREDO_AEQUALIS_S32 ((s32)c.sedes_fragmentorum,
                (s32)xar_numerus(fragmenta));
            per (j = ZEPHYRUM; j < xar_numerus(fragmenta); j++)
            {
                constans BriarFragmentum* f =
                    (constans BriarFragmentum*)xar_obtinere(
                    fragmenta, j);
                character b[160];
                      i32 u;

                sprintf(b, "id=\"frag-%.*s\"", (integer)f->id.mensura,
                    (constans character*)f->id.datum);
                CREDO_VERUM (_continet(piscina, pagina, b));
                per (u = ZEPHYRUM;
                    u < ((f->usus != NIHIL)
                        ? xar_numerus(f->usus) : ZEPHYRUM); u++)
                {
                    sprintf(b, "<a href=\"#l%d\">%d</a>",
                        (integer)*(constans i32*)xar_obtinere(f->usus,
                        u),
                        (integer)*(constans i32*)xar_obtinere(f->usus,
                        u));
                    CREDO_VERUM (_continet(piscina, pagina, b));
                }
            }
        }
    }

    imprimere("\n--- Probans symbola ---\n");
    {
         Xar* nexus;
         Xar* fragmenta;
      chorda  causa;
      chorda  pagina = _paginam_fingere(piscina, intern, fons,
          _texere(piscina, FIXA, "fragmenta_derivata.thistle"), &nexus,
          &fragmenta, &causa);
      FaciesCensus c;

        CREDO_CHORDA_VACUA (causa);
        c = _censere(piscina, pagina);
        /* insula NON vacua: sine hoc 'omne data-s in insula est'
         * nihil probat (zephyrum ex zephyro congruit) */
        CREDO_MAIOR_S32 ((s32)c.data_s, (s32)0);
        CREDO_AEQUALIS_S32 ((s32)c.data_s_absentia, (s32)0);
        /* symbolum DERIVATUM: piscina_generare_dynamicum caput suum
         * nusquam in plagula habet - hoc est pretium quod pagina
         * reddit */
        CREDO_VERUM (_continet(piscina, pagina,
            "data-s=\"piscina_generare_dynamicum\""));
        CREDO_VERUM (_continet(piscina, pagina,
            "\"piscina_generare_dynamicum\":{\"caput\":\"piscina.h\""));
    }

    imprimere("\n--- Probans symbola domestica ---\n");
    {
         Xar* nexus;
         Xar* fragmenta;
      chorda  causa;
      chorda  pagina = _paginam_fingere(piscina, intern, fons,
          _texere(piscina, FIXA, "fragmenta.thistle"), &nexus,
          &fragmenta, &causa);

        CREDO_CHORDA_VACUA (causa);
        /* 'summare' linea XXXV definitum, linea LVI adhibitum ->
         * ancora ad sedem, non data-s (plagula eam ipsa definit) */
        CREDO_VERUM (_continet(piscina, pagina,
            "<a class=\"fr-sym\" href=\"#l35\">summare</a>"));
    }

    imprimere("\n--- Probans evasionem ---\n");
    {
         Xar* nexus;
         Xar* fragmenta;
      chorda  causa;
      chorda  pagina = _paginam_fingere(piscina, intern, fons,
          _texere(piscina, FIXA, "facies_evasio.thistle"), &nexus,
          &fragmenta, &causa);

        CREDO_CHORDA_VACUA (causa);
        /* nullum '<script' praeter involucri proprium (vestis
         * probatoria unum fert) */
        /* unum '<script>' nudum (involucri); insula '<script type=
         * "application/json"' est - iners, nihil exsequitur */
        CREDO_AEQUALIS_S32 (_numerare(piscina, pagina, "<script>"),
            (s32)1);
        CREDO_AEQUALIS_S32 (_numerare(piscina, pagina,
            "<script type=\"application/json\""), (s32)1);
        CREDO_FALSUM (_continet(piscina, pagina, "alert(1)</script>"));
        CREDO_VERUM (_continet(piscina, pagina, "&lt;/script&gt;"));
        CREDO_VERUM (_continet(piscina, pagina, "&amp; &lt; &gt;"));
    }

    imprimere("\n--- Probans recusationes (adversa) ---\n");
    {
        constans character* adversa[6];
                       i32  v;

        adversa[0] = "adversa/fragmentum_absens.thistle";
        adversa[1] = "adversa/fragmentum_circulus.thistle";
        adversa[2] = "adversa/fragmentum_iteratum.thistle";
        adversa[3] = "adversa/fragmentum_munus.thistle";
        adversa[4] = "adversa/fragmentum_sine_id.thistle";
        adversa[5] = "adversa/transclusio_malformata.thistle";
        per (v = ZEPHYRUM; v < (i32)6; v++)
        {
             Xar* nexus;
             Xar* fragmenta;
          chorda  causa;
          chorda  pagina = _paginam_fingere(piscina, intern, fons,
              _texere(piscina, FIXA, adversa[v]), &nexus, &fragmenta,
              &causa);
             i32 recusationes = ZEPHYRUM;
             i32 j;

            /* LEX F4: pagina SEMPER redditur */
            CREDO_CHORDA_VACUA (causa);
            CREDO_VERUM (pagina.mensura > ZEPHYRUM);
            /* causa cuiusque regionis fractae in pagina stat, ad
             * lineam suam affixa */
            per (j = ZEPHYRUM; j < xar_numerus(nexus); j++)
            {
                constans BriarNexusRes* r = (constans BriarNexusRes*)
                    xar_obtinere(nexus, j);
                character b[64];

                si (r->linea_erroris <= ZEPHYRUM)
                {
                    perge;
                }
                recusationes = recusationes + I;
                CREDO_VERUM (_continet(piscina, pagina,
                    _evasum(piscina, r->causa)));
                sprintf(b, "data-linea=\"%d\"",
                    (integer)r->linea_erroris);
                CREDO_VERUM (_continet(piscina, pagina, b));
            }
            /* numerare, non absentiam: fixtura sine recusatione
             * hanc sectionem tacite praeteriret */
            CREDO_MAIOR_S32 ((s32)recusationes, (s32)0);
            /* unum aurum repraesentativum: lex F4 concrete pinnata
             * (ceterae quinque per causam+lineam ex briar ipso
             * probatae - aurea sex markup mutato omnia caderent) */
            si (v == I)
            {
                _aurum_conferre(piscina, pagina, "circulus.html");
            }
        }
    }

    imprimere("\n--- Probans derivam exemplarium ---\n");
    {
        /* duo exemplaria per annos taciti divergent: porta differentiam
         * ad tria bracchia html CONSTRINGIT. Lineae 'CASUS' ambarum
         * comparantur; quae differunt 'html' nominare DEBENT. */
        chorda domus  = filum_legere_totum("md/html/md-html.stml",
            piscina);
        chorda facies = filum_legere_totum(
            "briar/facies/md-html-facies.stml", piscina);
            Xar* ld;
            Xar* lf;
            i32  i;
            i32  divergentes = ZEPHYRUM;

        CREDO_VERUM (domus.mensura  > ZEPHYRUM);
        CREDO_VERUM (facies.mensura > ZEPHYRUM);
        ld = _lineae_casuum(piscina, domus);
        lf = _lineae_casuum(piscina, facies);
        CREDO_AEQUALIS_S32 ((s32)xar_numerus(ld),
            (s32)xar_numerus(lf));
        CREDO_MAIOR_S32 ((s32)xar_numerus(ld), (s32)30);
        per (i = ZEPHYRUM; i < xar_numerus(ld)
            && i < xar_numerus(lf); i++)
        {
            chorda a = *(chorda*)xar_obtinere(ld, i);
            chorda b = *(chorda*)xar_obtinere(lf, i);

            si (chorda_aequalis(a, b))
            {
                perge;
            }
            divergentes = divergentes + I;
            /* linea divergens html nominet - aliter deriva est */
            CREDO_VERUM (_continet(piscina, a, "html"));
            CREDO_VERUM (_continet(piscina, b, "html"));
        }
        CREDO_AEQUALIS_S32 ((s32)divergentes, (s32)3);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
