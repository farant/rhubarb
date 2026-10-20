/* ====================================================================
 * PROBATIO SENTENTIARUM
 *
 * Fixum principale documentum VERUM est (de-imagine.md, CCLXXIII
 * sententiae). Ideo numeri hic FIGUNTUR, non relaxantur: "sententiae
 * > ZEPHYRUM" nihil probat, "sententiae == CCLXXIII" omnia.
 *
 * Quinque decipulae in capite sententiae.h nominatae hic portas habent.
 * Praecipua: OCTO retractationes, non DUAE - extractor per lineas
 * operans duas invenit et successum nuntiavit.
 * ==================================================================== */

#include "latina.h"
#include "credo.h"
#include "sententiae.h"
#include "filum.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"

#include <stdio.h>
#include <string.h>

#define FX_RADIX "probationes/fixa/sententiae/"

/* PORTA: filum_legere_totum chordam VACUAM pro plagula absente reddit,
 * et lector chordae vacuae "nihil inventum" laete nuntiat - ergo via
 * mendosa probationem VIRIDEM sine ulla assertione pareret. Hic
 * frangimus. */
interior chorda
_fixum (Piscina* piscina, constans character* titulus)
{
    character via[CCLVI];
       chorda contentum;

    sprintf(via, "%s%s", FX_RADIX, titulus);
    contentum = filum_legere_totum(via, piscina);

    si (contentum.mensura == ZEPHYRUM)
    { imprimere("  FIXUM ABEST AUT VACUUM: %s\n", via);
    }
    CREDO_VERUM (contentum.mensura > ZEPHYRUM);
    redde contentum;
}

interior chorda
_ch (
    constans character* litterae)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l        = litterae;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(litterae);
    redde c;
}

interior i32
_gradus_numerare (
        constans Liber* liber,
    constans character* vocabulum)
{
    i32 k;
    i32 numerus = ZEPHYRUM;

    per (k = ZEPHYRUM; k < xar_numerus(liber->sententiae); k++)
    {
        Sententia* s = (Sententia*)xar_obtinere(liber->sententiae,
            (i32)k);
           Gradus* g;

        si (s == NIHIL || s->gradus < (s32)ZEPHYRUM)
        { perge;
        }
        g = (Gradus*)xar_obtinere(liber->vocabularium, (i32)s->gradus);
        si (   g != NIHIL
            && chorda_aequalis_literis(g->vocabulum, vocabulum))
        { numerus++;
        }
    }
    redde numerus;
}

interior i32
_anomalias_numerare (
    constans Liber* liber,
    GenusAnomaliae  genus)
{
    i32 k;
    i32 numerus = ZEPHYRUM;

    per (k = ZEPHYRUM; k < xar_numerus(liber->anomaliae); k++)
    {
        Anomalia* a = (Anomalia*)xar_obtinere(liber->anomaliae, (i32)k);
        si (a != NIHIL && a->genus == genus)
        { numerus++;
        }
    }
    redde numerus;
}

/* Fixura suitae: documentum verum SEMEL legitur (sine assertione);
 * sectiones II-X 'liber' (c->suita, copia vadosa) LEGUNT solum.
 * Sectio prima lectionem ipsam in piscina sua iterat et asserit. */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    Piscina* piscina;
      Liber* liber;
     chorda  fons;

    piscina   = c->piscina;
    fons      = filum_legere_totum(FX_RADIX "de_imagine.md", piscina);
    liber     = (Liber*)piscina_allocare(piscina, magnitudo(Liber));
    *liber    = sententiae_legere(fons, piscina);
    c->datum  = liber;
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;
      Liber  liber;

    piscina = c->piscina;
    {
        chorda fons = _fixum(piscina, "de_imagine.md");

        liber = sententiae_legere(fons, piscina);

        CREDO_VERUM (liber.successus);
        CREDO_AEQUALIS_I32 (liber.culpa, (i32)SENTENTIAE_BENE);

        /* NUMERI FIXI - decipula (b): grammatica laxa CCLXXV redderet */
        CREDO_AEQUALIS_I32 (xar_numerus(liber.sententiae), (i32)273);
        CREDO_AEQUALIS_I32 (xar_numerus(liber.partes), (i32)14);
    }
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
      Liber liber;

    liber = *(constans Liber*)c->suita;
    {
        CREDO_CHORDA_AEQUALIS_LITERIS (liber.titulus, "De Imagine");
        CREDO_CHORDA_AEQUALIS_LITERIS (liber.siglum, "DI");
        CREDO_CHORDA_AEQUALIS_LITERIS (liber.status, "DRAFT");

        /* tabula siglorum PARSATA, non solum reddita - resolutio
         * remissionum externarum ea eget */
        CREDO_AEQUALIS_I32 (xar_numerus(liber.auctoritates), (i32)4);
        {
            Auctoritas* a =
                (Auctoritas*)xar_obtinere(liber.auctoritates,
                                                      (i32)ZEPHYRUM);
            CREDO_NON_NIHIL (a);
            si (a != NIHIL)
            { CREDO_CHORDA_AEQUALIS_LITERIS (a->siglum, "DI");
            }
        }
    }
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
      Liber liber;

    liber = *(constans Liber*)c->suita;
    {
        /* prooemium ad primum SAEPTUM finit, non ad primam lineam locum
         * simulantem: regula ingenua ad lineam XV truncat et
         * vocabularium DESTRUIT dum successum nuntiat */
        CREDO_AEQUALIS_I32 (xar_numerus(liber.vocabularium), (i32)8);

        /* nullus gradus in corpore indeclaratus */
        CREDO_AEQUALIS_I32 (_anomalias_numerare(&liber,
                                ANOMALIA_GRADUS_IGNOTUS),
                                (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_anomalias_numerare(&liber,
                                ANOMALIA_VOCABULARIUM_ABEST),
                                (i32)ZEPHYRUM);
    }
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
      Liber liber;

    liber = *(constans Liber*)c->suita;
    {
        /* HAEC EST PORTA PRAECIPUA. SEPTEM sententiae retractae sunt
         * (14.1 + 29.1-29.6), quarum SEX per lineas extenduntur - ergo
         * extractor per lineas operans UNAM solam invenit.
         *
         * Cave numerum OCTO: octo lineae verbum 'retracted' ferunt, sed
         * linea XVII prooemii DECLARATIO vocabularii est, non gradus
         * applicatus. Distinctio ipsa est cur prooemium et corpus
         * separantur. */
        CREDO_AEQUALIS_I32 (_gradus_numerare(&liber, "retracted"),
            (i32)7);

        /* onus gradus captum: [retracted: <causa>] */
        {
            Sententia* s = sententiae_per_locum(&liber, _ch("29.3"));
            CREDO_NON_NIHIL (s);
            si (s != NIHIL)
            {
                CREDO_VERUM (s->retractum);
                CREDO_CHORDA_NON_VACUA (s->onus_gradus);
                CREDO_CHORDA_CONTINET (s->onus_gradus, _ch("14.obj.1"));
            }
        }
    }
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
      Liber liber;

    liber = *(constans Liber*)c->suita;
    {
        Sententia* s = sententiae_per_locum(&liber, _ch("4.obj.1.1"));

        CREDO_NON_NIHIL (s);
        si (s != NIHIL)
        {
            CREDO_AEQUALIS_I32 (s->profunditas, (i32)4);
            CREDO_CHORDA_INCIPIT (s->textus, _ch("Reply: granted"));
        }

        /* XXI loci segmentum 'obj' ferunt; grammatica solis digitis
         * omnes obiectiones amitteret */
        {
            i32 k;
            i32 numerus = ZEPHYRUM;

            per (k = ZEPHYRUM; k < xar_numerus(liber.sententiae); k++)
            {
                Sententia* t =
                    (Sententia*)xar_obtinere(liber.sententiae,
                                                        (i32)k);
                si (t != NIHIL && chorda_continet(t->locus, _ch("obj")))
                { numerus++;
                }
            }
            CREDO_AEQUALIS_I32 (numerus, (i32)21);
        }
    }
}

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
      Liber liber;

    liber = *(constans Liber*)c->suita;
    {
        /* novem loci parentem non habent, omnes formae X.obj.1 - si haec
         * culpa STRUCTURAE esset, documentum verum primo cursu
         * obstrueretur. Regula 'omnia segmenta numerica' ea accipit. */
        CREDO_NIHIL (sententiae_per_locum(&liber, _ch("4.obj")));
        CREDO_AEQUALIS_I32 (_anomalias_numerare(&liber,
                                ANOMALIA_PARENS_ABEST), (i32)ZEPHYRUM);
    }
}

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
      Liber liber;

    liber = *(constans Liber*)c->suita;
    {
        Sententia* s = sententiae_per_locum(&liber, _ch("15.1"));

        CREDO_NON_NIHIL (s);
        si (s != NIHIL)
        {
            Gradus* g;

            CREDO_MAIOR_AUT_AEQUALIS_S32 (s->gradus, (s32)ZEPHYRUM);
            g = (Gradus*)xar_obtinere(liber.vocabularium,
                (i32)s->gradus);
            CREDO_NON_NIHIL (g);
            si (g != NIHIL)
            { CREDO_CHORDA_AEQUALIS_LITERIS (g->vocabulum,
                  "conjecture");
            }

            /* [Fran's thesis; ...] gradus NON est - annotatio libera */
            CREDO_AEQUALIS_I32 (xar_numerus(s->annotationes), (i32)1);
        }
    }
}

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
      Liber liber;

    liber = *(constans Liber*)c->suita;
    {
        i32 k;
        i32 internae = ZEPHYRUM;
        i32 externae = ZEPHYRUM;

        per (k = ZEPHYRUM; k < xar_numerus(liber.sententiae); k++)
        {
            Sententia* s = (Sententia*)xar_obtinere(liber.sententiae,
                (i32)k);
                  i32 j;

            si (s == NIHIL)
            { perge;
            }
            per (j = ZEPHYRUM; j < xar_numerus(s->remissiones); j++)
            {
                Remissio* r = (Remissio*)xar_obtinere(s->remissiones,
                    (i32)j);
                si (r == NIHIL)
                { perge;
                }
                si (r->genus == REMISSIO_INTERNA)
                { internae++; CREDO_VERUM (r->resoluta);
                }
                alioquin
                { externae++;
                }
            }
        }

        CREDO_MAIOR_S32 ((s32)internae, (s32)10);
        CREDO_MAIOR_S32 ((s32)externae, (s32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_anomalias_numerare(&liber,
                                ANOMALIA_REMISSIO_INRESOLUTA),
                            (i32)ZEPHYRUM);
    }
}

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;
      Liber  liber;

    piscina  = c->piscina;
    liber    = *(constans Liber*)c->suita;
    {
          chorda latum     = _fixum(piscina, "punctum_latum.txt");
          chorda angustum  = _fixum(piscina, "punctum_angustum.txt");
          chorda crlf      = _fixum(piscina, "punctum_crlf.txt");
          chorda n_latum;
          chorda n_angustum;
          chorda n_crlf;
        Sigillum s_latum;
        Sigillum s_angustum;
        Sigillum s_crlf;

        n_latum    = sententiae_normalizare(latum, liber.vocabularium,
                                            piscina);
        n_angustum = sententiae_normalizare(angustum,
            liber.vocabularium,
                                            piscina);
        n_crlf     = sententiae_normalizare(crlf, liber.vocabularium,
                                            piscina);

        /* contra CHORDAM probamus, non contra digestum: si differunt,
         * probatio dicit QUID differat, non solum ALIQUID */
        CREDO_CHORDA_AEQUALIS (n_latum, n_angustum);
        CREDO_CHORDA_AEQUALIS (n_latum, n_crlf);
        CREDO_CHORDA_INCIPIT (n_latum, _ch("No. The picture requires"));
        CREDO_FALSUM (chorda_continet(n_latum, _ch("[theorem]")));
        CREDO_FALSUM (chorda_continet(n_latum, _ch("4.1.")));

        s_latum     = sententiae_sigillare(n_latum);
        s_angustum  = sententiae_sigillare(n_angustum);
        s_crlf      = sententiae_sigillare(n_crlf);

        CREDO_VERUM (sigillum_aequale(&s_latum, &s_angustum));
        CREDO_VERUM (sigillum_aequale(&s_latum, &s_crlf));
    }
}

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
      Liber liber;

    liber = *(constans Liber*)c->suita;
    {
              Sententia* s = sententiae_per_locum(&liber,
                  _ch("13.5"));
        IudiciumAncorae iudicium;
               Sigillum falsum;

        CREDO_NON_NIHIL (s);
        si (s != NIHIL)
        {
            iudicium = sententiae_ancoram_iudicare(&liber, _ch("13.5"),
                                                   s->sigillum);
            CREDO_AEQUALIS_I32 (iudicium.status, (i32)ANCORA_INTEGRA);

            /* sigillum alienum ad locum exstantem = MUTATA */
            falsum = sententiae_sigillare(_ch("textus prorsus alius"));
            iudicium = sententiae_ancoram_iudicare(&liber, _ch("13.5"),
                                                   falsum);
            CREDO_AEQUALIS_I32 (iudicium.status, (i32)ANCORA_MUTATA);

            /* sigillum verum ad locum absentem = MOTA, cum loco novo */
            iudicium = sententiae_ancoram_iudicare(&liber, _ch("99.9"),
                                                   s->sigillum);
            CREDO_AEQUALIS_I32 (iudicium.status, (i32)ANCORA_MOTA);
            CREDO_CHORDA_AEQUALIS_LITERIS (iudicium.locus_novus,
                "13.5");

            /* neutrum = ORBA */
            iudicium = sententiae_ancoram_iudicare(&liber, _ch("99.9"),
                                                   falsum);
            CREDO_AEQUALIS_I32 (iudicium.status, (i32)ANCORA_ORBA);
        }
    }
}

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        chorda parens;

        CREDO_VERUM (sententiae_parentem_capere(_ch("4.obj.1.1"),
            &parens));
        CREDO_CHORDA_AEQUALIS_LITERIS (parens, "4.obj.1");
        CREDO_VERUM (sententiae_parentem_capere(_ch("1.1"), &parens));
        CREDO_CHORDA_AEQUALIS_LITERIS (parens, "1");
        CREDO_FALSUM (sententiae_parentem_capere(_ch("7"), &parens));
    }
}

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         Liber vacuus;
         Liber prosa;
        chorda nihil_chorda;

        nihil_chorda.datum    = NIHIL;
        nihil_chorda.mensura  = ZEPHYRUM;

        vacuus = sententiae_legere(nihil_chorda, piscina);
        CREDO_VERUM (vacuus.successus);
        CREDO_AEQUALIS_I32 (xar_numerus(vacuus.sententiae),
            (i32)ZEPHYRUM);

        prosa = sententiae_legere(
            _ch("Prosa mera. Nulla structura. Nullum saeptum.\n"
                "Altera linea quae puncto terminatur.\n"), piscina);
        CREDO_VERUM (prosa.successus);
        CREDO_AEQUALIS_I32 (xar_numerus(prosa.sententiae),
            (i32)ZEPHYRUM);
    }
}

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        /* NOTA: 'duplex' hic adhiberi NEQUIT - macrum latinae ('double').
         * Tertia flagratio huius classis in duobus diebus; vide 01KXTRM0DB. */
        Liber duplicatus;
        Liber apertum;

        duplicatus = sententiae_legere(
            _ch("# T\n\n```\n1. Prima.\n1. Iterum prima.\n```\n"),
            piscina);
        CREDO_FALSUM (duplicatus.successus);
        CREDO_AEQUALIS_I32 (duplicatus.culpa,
            (i32)SENTENTIAE_LOCUS_DUPLEX);
        CREDO_CHORDA_NON_VACUA (duplicatus.causa);

        apertum = sententiae_legere(
            _ch("# T\n\n```\n1. Prima.\n"), piscina);
        CREDO_FALSUM (apertum.successus);
        CREDO_AEQUALIS_I32 (apertum.culpa,
            (i32)SENTENTIAE_SAEPTUM_IMPAR);
        CREDO_CHORDA_NON_VACUA (apertum.causa);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. lectio documenti veri", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "II. prooemium", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "III. vocabularium graduum (decipula c)", _sectio_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "IV. tags per lineas extensa (decipula a)", _sectio_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "V. segmenta nominata (decipula b)", _sectio_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "VI. parens absens est ANOMALIA, non culpa (dec. e)",
        _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "VII. gradus ET annotatio simul (decipula f)", _sectio_septima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "VIII. remissiones", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "IX. involutio sigillum NON mutat", _sectio_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "X. iudicium ancorae", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "XI. parens", _sectio_decima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "XII. inputa degenerata (nihil ruat)", _sectio_decima_secunda,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "XIII. culpae structurae OBSTANT", _sectio_decima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("sententiae", SECTIONES,
        _suitam_parare, NIHIL);
}
