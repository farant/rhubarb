/* crusta_arbor.c - Aedificator arboris crustae (vide .h)
 *
 * GRADUS (frama): nodus in constructione, locus liberos recipiens,
 * modus lectoris dum gradus in vertice est, status per genus. Gradus
 * clausus nodum suum gradui infra appendit (locus eius currens):
 * lista-nodus appenditur, nodus singularis ponitur.
 *
 * VERBUM: cursus partium ADIACENTIUM (offset partis == finis verbi);
 * pars non adiacens aut trivium aut lexema non-pars verbum claudit.
 * Modus gradus verbi = modus parentis post initium verbi (VERBA post
 * verbum primum imperii, ASSIGNATIONES post aedificatorem).
 *
 * IMPERIUM: status ZEPHYRUM (positio imperii, INITIUM), I (verbum
 * visum, VERBA), II (verbum primum aedificator, ASSIGNATIONES).
 *
 * COMPOSITA (P5): gradus compositi (grex, crustula, conditio, ramus,
 * iteratio, cyclus, repetitio, cursus, electio, optio, arithmetica,
 * iudicium) MUNUS suum ex (genus, locus, status) habent - aut LISTA
 * (sententias recipit: conditio in probatione/liberis, grex apertus,
 * cursus...) aut EXSPECTANS (nomen, 'in', 'do'...) aut CLAUSUM (status
 * II post lexema clausurae: redirectiones exspectat, sententia pendens
 * est ut imperium, '&&' et '|' id involvunt). Functio et socius
 * corpus unum exspectant (locus nodi), sententiae sunt semper. Lexema
 * clausurae ('fi' 'done' 'esac' '}' ')') gradum SUUM proximum petit;
 * quidquid supra iacet absens clauditur (regula html de tag finali).
 * Post clausuram modus POST_COMPOSITUM (bash reservata iterum agnoscit:
 * 'if ((x)) then', '{ { a; } }').
 *
 * HEREDOCA (decretum 01M2NJ16RG): corpus ubi octeti iacent. Post lineam
 * novam (separator aut trivium) cum petitionibus pendentibus gradus
 * heredoc pellitur cuius nodus listae intimae STATIM appenditur (post
 * separatorem aut operatorem qui lineam novam fert), partes in modo
 * HEREDOC petuntur, FINIS regionis delimitatorem et finem ponit,
 * redirectio pendens corpus per referentiam accipit. Mensum bash 5.2:
 * '$(cat <<A)' cum ')' ante lineam novam corpus EXTRA legit (lista
 * exterior), backtick numquam extra regionem (corpus vacuum intra).
 */

#include "crusta_arbor.h"
#include "crusta_lector.h"
#include "crusta_arithmetica.h"
#include "crusta_lexicon.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Gradus et parsatura
 * ================================================== */

/* Iudicium [[ ]]: machina parva - praecedentiae IV (probatio unaria
 * '-f x' et binaria 'a == b'), III ('!'), II ('&&'), I ('||');
 * operanda verba aut inclusae; operandum absens NIHIL in acervo. */
nomen structura {
    /* NIHIL = signum implicitum (iuxtapositio) */
    MateriaToken* tok;
             i32  praecedentia;
             b32  praeposita;
    /* corpora heredoc post lexema signi (P9b; NIHIL = nulla) */
             Xar* corpora;
} IudiciiSignum;

/* operandum in acervo: nodus (NIHIL = absens) + corpora heredoc post
 * lexema eius ultimum (P9b) */
nomen structura {
    MateriaNodus* nodus;
             Xar* corpora;
} OperandumIudicii;

nomen structura {
    Xar* operanda;          /* OperandumIudicii */
    Xar* signa;             /* IudiciiSignum */
    b32  operandum_exspectatur;
    i32  mala;
    /* corpora heredoc ante lexema primum (P9b) */
    Xar* corpora_initii;
    /* ultimum pulsum: ZEPHYRUM nihil, I operandum, II signum */
    i32 ultimum;
} Iudicatio;

nomen structura {
    MateriaNodus* nodus;
             s32  genus;
    /* locus liberos recipiens */
             i32 locus;
    /* modus petendi dum in vertice */
      CrustaModus modus;
              i32 status;
    /* verbum: finis partis ultimae (adiacentia) */
             s32 verbi_finis;
    /* expansio: '/' aut ':' aut ZEPHYRUM */
             s32 divisor;
    /* substitutio backtick: regio aperta */
             b32 gravis;
    /* arithmetica ($((, ((, inclusa, cyclus): machina; folia ei
     * dantur */
    CrustaArithmetica* machina;
    /* iudicium, iudicium-inclusa */
    Iudicatio* iudicatio;
    /* variabilis ultima (subscriptum sequens) */
    MateriaNodus* variabile;
    /* $((: lexema aperturae, situs ante id, prior ligatoris
     * (reversio) */
     MateriaToken* apertura;
      CrustaSitus  situs_initii;
     MateriaToken* prior_antea;
    /* socius: verbum primum post 'coproc' (titulus aut imperium?) */
     MateriaToken* candidatus;
    /* substitutio: petitiones heredoc ante aperturam (regio backtick
          * suas intra claudit) */
              i32 heredoca_ante;
    /* $((: numerus lexematum recordatorum ante aperturam (reversio
     * ea sublata) */
              i32 lexemata_ante;
} Gradus;

nomen structura {
                     Piscina* piscina;
                CrustaLector  lector;
         MateriaLexiconRatum  lexicon;
                         Xar* gradus;       /* Gradus per valorem */
                         Xar* pendentia;    /* MateriaToken* trivia */
                MateriaToken* prior;
                MateriaToken* prior_antea;   /* prior ante prior */
    /* redirectiones heredoc quarum corpus pendet (MateriaNodus*),
     * ordine petitionum lectoris; caput = proxima */
                                        Xar* redirectiones_pendentes;
                                        i32  redirectio_caput;
                   constans CrustaDialectus* dialectus;
                              CrustaParsura* relatio;
                                        b32  memoria_defecit;
    /* lexemata accepta ordine (NIHIL = sine memoria) */
                         Xar* lexemata;
    /* vexillum initium_lineae per regulam lectoris STML: VERUM post
     * trivium LINEA usque ad lexema proximum (vide
     * _lexema_recordare) */
                          b32 post_lineam;
} Aedificatio;

/* lexema acceptum recordare (tegumentum lectoris per corpus) */
interior b32
_lexema_recordare (
       Aedificatio* p,
      MateriaToken* t)
{
    MateriaToken** sedes;

    /* VEXILLUM initium_lineae per regulam qua lector STML
     * (materia_arbor, fixura positionum) eam ex triviis reficit:
     * lexema lineam incipit si trivium muneris LINEA ab lexemate
     * priore intervenit (LAMINA non; separator linea nova
     * SUBSTANTIVUS non, C6); documenti initium lineam incipit.
     * Regula derivata, non factum bash - proiectio eam sic reficit,
     * ergo arbor parsata eam sic ferat aut comparator dissentit
     * (P7). Consumptor crustae nullus. */
    si (t->genus == (s32)CRUSTA_LEX_LINEA)
    {
        p->post_lineam = VERUM;
    }
    alioquin si (!materia_lexicon_trivium_est(&p->lexicon, t->genus))
    {
        materia_token_initium_lineae_ponere(t, p->post_lineam);
        p->post_lineam = FALSUM;
    }
    si (p->lexemata == NIHIL)
    {
        redde VERUM;
    }
    sedes = (MateriaToken**)xar_addere(p->lexemata);
    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    *sedes = t;
    redde VERUM;
}

interior Gradus*
_vertex (
    Aedificatio* p)
{
    i32 n = xar_numerus(p->gradus);

    redde n > ZEPHYRUM ? (Gradus*)xar_obtinere(p->gradus, n
        - I) : NIHIL;
}

interior Gradus*
_infra (
    Aedificatio* p,
            i32  gradus_infra)
{
    i32 n = xar_numerus(p->gradus);

    redde n > gradus_infra
        ? (Gradus*)xar_obtinere(p->gradus, n - I
            - gradus_infra) : NIHIL;
}

interior i32
_loci_numerus (
    s32 genus)
{
    redde CRUSTA_REGISTRUM.genera[genus].loci_numerus;
}

interior MateriaLocusSpecies
_species (
    s32 genus,
    i32 locus)
{
    redde (MateriaLocusSpecies)CRUSTA_REGISTRUM.loci[
        CRUSTA_REGISTRUM.genera[genus].loci_offset + locus].species;
}

interior MateriaNodus*
_nodus (
    Aedificatio* p,
            s32  genus)
{
    MateriaNodus* n = materia_nodus_creare(p->piscina, genus,
        _loci_numerus(genus));

    si (n == NIHIL)
    {
        p->memoria_defecit = VERUM;
    }
    redde n;
}

/* nodum in locum parentis: lista appenditur, singularis ponitur */
interior b32
_nodum_dare (
       Aedificatio* p,
      MateriaNodus* parens,
               i32  locus,
      MateriaNodus* filius)
{
    MateriaLocusSpecies s = _species(parens->genus, locus);

    si (s == MATERIA_LOCUS_LISTA_NODUS)
    {
        redde materia_nodus_appendere(p->piscina, parens, locus,
            materia_valor_nodus(filius), s);
    }
    redde materia_nodus_ponere(parens, locus,
        materia_valor_nodus(filius),
        s);
}

interior b32
_token_ponere (
    MateriaNodus* nodus,
             i32  locus,
    MateriaToken* t)
{
    redde materia_nodus_ponere(nodus, locus, materia_valor_token(t),
        MATERIA_LOCUS_TOKEN);
}

interior b32
_token_appendere (
       Aedificatio* p,
      MateriaNodus* nodus,
               i32  locus,
      MateriaToken* t)
{
    redde materia_nodus_appendere(p->piscina, nodus, locus,
        materia_valor_token(t), MATERIA_LOCUS_LISTA_TOKEN);
}

interior b32
_absens (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde nodus->loci[locus].genus == MATERIA_VALOR_NIHIL;
}

interior Gradus*
_pellere (
    Aedificatio* p,
   MateriaNodus* nodus,
            i32  locus,
    CrustaModus  modus)
{
    Gradus* g = (Gradus*)xar_addere(p->gradus);
       i32  n;

    si (g == NIHIL)
    {
        p->memoria_defecit = VERUM;
        redde NIHIL;
    }
    memset(g, ZEPHYRUM, magnitudo(*g));
    g->nodus  = nodus;
    g->genus  = nodus->genus;
    g->locus  = locus;
    g->modus  = modus;
    n         = xar_numerus(p->gradus);
    si (p->relatio != NIHIL && n > p->relatio->profunditas_maxima)
    {
        p->relatio->profunditas_maxima = n;
    }
    redde g;
}

interior s32
_token_finis (
    constans MateriaToken* t)
{
    redde t->byte_offset + (s32)t->valor.mensura;
}


/* ==================================================
 * Ligator (C7)
 * ================================================== */

interior b32
_cumulare (
       Aedificatio* p,
      MateriaToken* t)
{
    MateriaToken** sedes = (MateriaToken**)xar_addere(p->pendentia);

    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    *sedes = t;
    redde VERUM;
}

interior b32
_solvere (
       Aedificatio* p,
      MateriaToken* sequens)
{
    i32 numerus = xar_numerus(p->pendentia);
    MateriaToken** plana;
    i32 divisio;
    i32 j;

    si (numerus == ZEPHYRUM)
    {
        p->prior_antea  = p->prior;
        p->prior        = sequens;
        redde VERUM;
    }
    plana = (MateriaToken**)piscina_allocare_ordinatum(p->piscina,
        (memoriae_index)magnitudo(MateriaToken*)
            * (memoriae_index)numerus,
        (memoriae_index)magnitudo(MateriaToken*));
    si (plana == NIHIL)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        plana[j] = *(MateriaToken**)xar_obtinere(p->pendentia, j);
    }

    /* divisio post lineam novam ULTIMAM (LINEA aut LAMINA) */
    divisio = ZEPHYRUM;
    si (p->prior != NIHIL)
    {
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (   plana[j]->genus == (s32)CRUSTA_LEX_LINEA
                || plana[j]->genus == (s32)CRUSTA_LEX_LAMINA)
            {
                divisio = j + I;
            }
        }
    }
    si (divisio > ZEPHYRUM)
    {
        si (!materia_token_trivia_post_ponere(p->prior, p->piscina,
            plana,
                divisio))
        {
            redde FALSUM;
        }
    }
    si (divisio < numerus)
    {
        si (!materia_token_trivia_ante_ponere(sequens, p->piscina,
                plana + divisio, numerus - divisio))
        {
            redde FALSUM;
        }
    }
    xar_vacare(p->pendentia);
    p->prior_antea  = p->prior;
    p->prior        = sequens;
    redde VERUM;
}


/* ==================================================
 * Genera gradum
 * ================================================== */

/* gradus qui sententias recipit (lista sententiarum) */
interior b32
_lista_est (
    constans Gradus* g)
{
    commutatio (g->genus)
    {
        casus CRUSTA_GENUS_PROGRAMMA:
        casus CRUSTA_GENUS_PARS_SUBSTITUTIO:
        casus CRUSTA_GENUS_PARS_PROCESSUS:
        casus CRUSTA_GENUS_CURSUS:
        casus CRUSTA_GENUS_RAMUS:
            redde VERUM;
        casus CRUSTA_GENUS_GREX:
        casus CRUSTA_GENUS_CRUSTULA:
            redde g->status < II;
        casus CRUSTA_GENUS_CONDITIO:
            redde g->locus == (i32)CRUSTA_CONDITIO_PROBATIO
                || g->locus == (i32)CRUSTA_CONDITIO_LIBERI;
        casus CRUSTA_GENUS_REPETITIO:
            redde g->locus == (i32)CRUSTA_REPETITIO_PROBATIO;
        casus CRUSTA_GENUS_ELECTIO:
            redde g->locus == (i32)CRUSTA_ELECTIO_LIBERI;
        casus CRUSTA_GENUS_OPTIO:
            redde g->locus == (i32)CRUSTA_OPTIO_LIBERI;
        ordinarius:
            redde FALSUM;
    }
}

/* compositum clausum (post lexema clausurae): redirectiones
 * exspectat, sententia pendens est */
interior b32
_compositum_clausum (
    constans Gradus* g)
{
    commutatio (g->genus)
    {
        casus CRUSTA_GENUS_GREX:
        casus CRUSTA_GENUS_CRUSTULA:
        casus CRUSTA_GENUS_CONDITIO:
        casus CRUSTA_GENUS_ITERATIO:
        casus CRUSTA_GENUS_CYCLUS:
        casus CRUSTA_GENUS_REPETITIO:
        casus CRUSTA_GENUS_ELECTIO:
        casus CRUSTA_GENUS_ARITHMETICA:
        casus CRUSTA_GENUS_IUDICIUM:
            redde g->status == II;
        ordinarius:
            redde FALSUM;
    }
}

/* sententia pendens: separator eam claudit, '&&' '|' involvunt */
interior b32
_sententia_est (
    constans Gradus* g)
{
    redde g->genus == (s32)CRUSTA_GENUS_IMPERIUM
        || g->genus == (s32)CRUSTA_GENUS_PIPA
        || g->genus == (s32)CRUSTA_GENUS_CATENA
        || g->genus == (s32)CRUSTA_GENUS_FUNCTIO
        || g->genus == (s32)CRUSTA_GENUS_SOCIUS
        || _compositum_clausum(g);
}

/* gradus in quo sententia nova aperiri potest (positio imperii):
 * lista, catena, pipa, functio corpus exspectans, socius */
interior b32
_sententiam_admittit (
    constans Gradus* g)
{
    redde _lista_est(g)
        || g->genus == (s32)CRUSTA_GENUS_CATENA
        || g->genus == (s32)CRUSTA_GENUS_PIPA
        || (   g->genus == (s32)CRUSTA_GENUS_FUNCTIO
            && (g->status == I || g->status == III))
        || (g->genus == (s32)CRUSTA_GENUS_SOCIUS && g->status != I);
}

/* functio corpus exspectans: post titulum ('function f {') aut post
 * ')' */
interior b32
_functio_corpus_exspectat (
    constans Gradus* g)
{
    redde g->genus == (s32)CRUSTA_GENUS_FUNCTIO
        && (g->status == I || g->status == III);
}

/* genera quorum clausura absens numeranda est: locus clausurae */
interior s32
_locus_clausurae (
    s32 genus)
{
    commutatio (genus)
    {
        casus CRUSTA_GENUS_PARS_GEMINA:
        casus CRUSTA_GENUS_PARS_VERSA:
            redde (s32)CRUSTA_GEMINA_TOK_CLAUSURA;
        casus CRUSTA_GENUS_PARS_EXPANSIO:
            redde (s32)CRUSTA_EXPANSIO_TOK_CLAUSURA;
        casus CRUSTA_GENUS_PARS_SUBSTITUTIO:
        casus CRUSTA_GENUS_PARS_PROCESSUS:
            redde (s32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA;
        casus CRUSTA_GENUS_TABULATUM:
            redde (s32)CRUSTA_TABULATUM_TOK_CLAUSURA;
        casus CRUSTA_GENUS_PARS_PARAMETRUM:
            redde (s32)CRUSTA_PARAMETRUM_TOK_TITULUS;
        casus CRUSTA_GENUS_PARS_ARITHMETICA:
            redde (s32)CRUSTA_PARS_ARITHMETICA_TOK_CLAUSURA;
        casus CRUSTA_GENUS_ARITHMETICA:
            redde (s32)CRUSTA_ARITHMETICA_TOK_CLAUSURA;
        casus CRUSTA_GENUS_INCLUSA:
            redde (s32)CRUSTA_INCLUSA_TOK_CLAUSURA;
        casus CRUSTA_GENUS_GREX:
            redde (s32)CRUSTA_GREX_TOK_CLAUSURA;
        casus CRUSTA_GENUS_CRUSTULA:
            redde (s32)CRUSTA_CRUSTULA_TOK_CLAUSURA;
        casus CRUSTA_GENUS_CONDITIO:
            redde (s32)CRUSTA_CONDITIO_TOK_CLAUSURA;
        casus CRUSTA_GENUS_ITERATIO:
            redde (s32)CRUSTA_ITERATIO_CURSUS;
        casus CRUSTA_GENUS_CYCLUS:
            redde (s32)CRUSTA_CYCLUS_CURSUS;
        casus CRUSTA_GENUS_REPETITIO:
            redde (s32)CRUSTA_REPETITIO_CURSUS;
        casus CRUSTA_GENUS_CURSUS:
            redde (s32)CRUSTA_CURSUS_TOK_CLAUSURA;
        casus CRUSTA_GENUS_ELECTIO:
            redde (s32)CRUSTA_ELECTIO_TOK_CLAUSURA;
        casus CRUSTA_GENUS_OPTIO:
            redde (s32)CRUSTA_OPTIO_TOK_PARENTHESIS_CLAUSURA;
        casus CRUSTA_GENUS_IUDICIUM:
            redde (s32)CRUSTA_IUDICIUM_TOK_CLAUSURA;
        casus CRUSTA_GENUS_IUDICIUM_INCLUSA:
            redde (s32)CRUSTA_INCLUSA_TOK_CLAUSURA;
        casus CRUSTA_GENUS_FUNCTIO:
            redde (s32)CRUSTA_FUNCTIO_CORPUS;
        casus CRUSTA_GENUS_SOCIUS:
            redde (s32)CRUSTA_SOCIUS_IMPERIUM;
        casus CRUSTA_GENUS_HEREDOC:
            redde (s32)CRUSTA_HEREDOC_TOK_DELIMITATOR;
        ordinarius:
            redde (s32)-I;
    }
}

/* corpora (Xar de MateriaNodus*, NIHIL = nulla) in locum dare */
interior b32
_corpora_dare (
     Aedificatio* p,
    MateriaNodus* parens,
             i32  locus,
             Xar* corpora)
{
    i32 i;

    si (corpora == NIHIL)
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(corpora); i++)
    {
        si (!_nodum_dare(p, parens, locus,
                *(MateriaNodus**)xar_obtinere(corpora, i)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* machinam gradus finire: expressio in locum (cyclus: liberis
 * appenditur), mala numerata; corpora heredoc ante lexema primum in
 * post_aperturam, post expressionem in post_expressionem (cyclus:
 * liberis ordine) - P9b. Loci inclusae, partis arithmeticae et
 * arithmeticae eodem ordine (I, II, III). */
interior b32
_machinam_finire (
    Aedificatio* p,
         Gradus* g)
{
             Xar* initii;
             Xar* post;
    MateriaNodus* e = crusta_arithmetica_finire(g->machina, &initii,
        &post);
             i32 locus = g->genus == (s32)CRUSTA_GENUS_INCLUSA
                 ? (i32)CRUSTA_INCLUSA_EXPRESSIO
                 : g->genus == (s32)CRUSTA_GENUS_PARS_ARITHMETICA
                 ? (i32)CRUSTA_PARS_ARITHMETICA_EXPRESSIO
                 : (i32)CRUSTA_ARITHMETICA_EXPRESSIO;

    si (p->relatio != NIHIL)
    {
        p->relatio->mala += g->machina->mala;
    }
    g->machina = NIHIL;
    si (g->genus == (s32)CRUSTA_GENUS_CYCLUS)
    {
        redde _corpora_dare(p, g->nodus, (i32)CRUSTA_CYCLUS_LIBERI,
            initii)
            && (   e == NIHIL
                || _nodum_dare(p, g->nodus, (i32)CRUSTA_CYCLUS_LIBERI,
                e))
            && _corpora_dare(p, g->nodus, (i32)CRUSTA_CYCLUS_LIBERI,
            post);
    }
    si (!_corpora_dare(p, g->nodus, locus - I, initii))
    {
        redde FALSUM;
    }
    si (   e != NIHIL
        && !materia_nodus_ponere(g->nodus, locus,
        materia_valor_nodus(e),
            MATERIA_LOCUS_NODUS))
    {
        redde FALSUM;
    }
    redde _corpora_dare(p, g->nodus, locus + I, post);
}

interior b32
_iudicationem_finire (
    Aedificatio* p,
         Gradus* g);

interior b32
_iudicatio_operandum (
     Aedificatio* p,
       Iudicatio* iud,
    MateriaNodus* n);

interior b32
_socium_imperare (
    Aedificatio* p,
         Gradus* g);

/* an lista sententiam habeat (separator, heredoc, malum non sunt) */
interior b32
_lista_plena (
    constans MateriaValor* lista)
{
    i32 k;

    si (lista->genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*lista); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*lista,
            k);

        si (   e->genus              == MATERIA_VALOR_NODUS
            && e->datum.nodus->genus != (s32)CRUSTA_GENUS_SEPARATOR
            && e->datum.nodus->genus != (s32)CRUSTA_GENUS_HEREDOC
            && e->datum.nodus->genus != (s32)CRUSTA_GENUS_MALUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* listae compositi quas bash non vacuas poscit (P11b, 'bash -n' 5.2.15
 * mensuratum): grex, crustula, cursus; probatio et corpus conditionis
 * et rami 'elif' (else: corpus); probatio repetitionis. Optio et
 * substitutio vacuae licent. */
interior vacuum
_vacuas_numerare (
              Aedificatio* p,
    constans MateriaNodus* nodus)
{
    i32 vacuae = ZEPHYRUM;

    si (p->relatio == NIHIL)
    {
        redde;
    }
    commutatio (nodus->genus)
    {
        casus CRUSTA_GENUS_GREX:
        casus CRUSTA_GENUS_CRUSTULA:
        casus CRUSTA_GENUS_CURSUS:
            /* liberi: locus I in omnibus tribus */
            si (!_lista_plena(&nodus->loci[CRUSTA_GREX_LIBERI]))
            {
                vacuae++;
            }
            frange;
        casus CRUSTA_GENUS_CONDITIO:
            si (!_lista_plena(&nodus->loci[CRUSTA_CONDITIO_PROBATIO]))
            {
                vacuae++;
            }
            si (!_lista_plena(&nodus->loci[CRUSTA_CONDITIO_LIBERI]))
            {
                vacuae++;
            }
            frange;
        casus CRUSTA_GENUS_RAMUS:
            si (   nodus->loci[CRUSTA_RAMUS_TOK_DEINDE].genus
                    == MATERIA_VALOR_TOKEN
                || nodus->loci[CRUSTA_RAMUS_PROBATIO].genus
                    == MATERIA_VALOR_LISTA)
            {
                si (!_lista_plena(&nodus->loci[CRUSTA_RAMUS_PROBATIO]))
                {
                    vacuae++;
                }
            }
            si (!_lista_plena(&nodus->loci[CRUSTA_RAMUS_LIBERI]))
            {
                vacuae++;
            }
            frange;
        casus CRUSTA_GENUS_REPETITIO:
            si (!_lista_plena(&nodus->loci[CRUSTA_REPETITIO_PROBATIO]))
            {
                vacuae++;
            }
            frange;
        ordinarius:
            frange;
    }
    p->relatio->listae_vacuae += vacuae;
}

/* gradum verticis claudere: nodum gradui infra dare */
interior b32
_claudere (
    Aedificatio* p)
{
     Gradus* g = _vertex(p);
     Gradus* parens;
        s32  locus_clausurae;

    si (g == NIHIL)
    {
        redde FALSUM;
    }
    /* socius cum candidato: candidatus imperium erat */
    si (   g->genus == (s32)CRUSTA_GENUS_SOCIUS && g->status == I
        && !_socium_imperare(p, g))
    {
        redde FALSUM;
    }
    si (g->machina != NIHIL && !_machinam_finire(p, g))
    {
        redde FALSUM;
    }
    si (g->iudicatio != NIHIL && !_iudicationem_finire(p, g))
    {
        redde FALSUM;
    }
    _vacuas_numerare(p, g->nodus);
    locus_clausurae = _locus_clausurae(g->genus);
    si (   locus_clausurae >= ZEPHYRUM
        && _absens(g->nodus, (i32)locus_clausurae)
        && p->relatio      != NIHIL)
    {
        p->relatio->clausurae_absentes++;
    }
    parens = _infra(p, I);
    si (parens == NIHIL)
    {
        redde FALSUM;   /* programma numquam clauditur hac via */
    }
    si (parens->machina != NIHIL)
    {
        /* folium arithmeticae: operandum machinae parentis */
        si (!crusta_arithmetica_operandum(parens->machina, g->nodus))
        {
            redde FALSUM;
        }
    }
    alioquin si (parens->iudicatio != NIHIL)
    {
        si (!_iudicatio_operandum(p, parens->iudicatio, g->nodus))
        {
            redde FALSUM;
        }
    }
        alioquin si (!_nodum_dare(p, parens->nodus, parens->locus,
                     g->nodus))
    {
        redde FALSUM;
    }
    /* imperium sine verbo post liberum primum (assignatio,
     * redirectio): linea nova iam terminat */
    si (   parens->genus  == (s32)CRUSTA_GENUS_IMPERIUM
        && parens->status == ZEPHYRUM)
    {
        parens->modus = CRUSTA_MODUS_ASSIGNATIONES;
    }
    /* redirectio post titulum aedificatoris 'x=(' tollit ('declare -a
     * >o c=(z)' bash errat; P11b mensuratum) */
    si (   parens->genus  == (s32)CRUSTA_GENUS_IMPERIUM
        && parens->status == II
        && g->genus       == (s32)CRUSTA_GENUS_REDIRECTIO)
    {
        parens->modus = CRUSTA_MODUS_VERBA;
    }
    xar_removere_ultimum(p->gradus);
    redde VERUM;
}

interior b32
_verbum_claudere (
    Aedificatio* p);

interior b32
_redirectionem_finire (
    Aedificatio* p);

interior b32
_heredoc_claudere (
    Aedificatio* p);

/* gradus omnes supra numerum datum claudere (quisque via sua: verbum
 * cum reactione parentis, redirectio cum petitione heredoc, ceteri
 * absentes) - regula html de tag finali */
interior b32
_claudere_usque (
    Aedificatio* p,
            i32  numerus_manens)
{
    Gradus* g;

    dum (xar_numerus(p->gradus) > numerus_manens)
    {
        g = _vertex(p);
        si (g->genus == (s32)CRUSTA_GENUS_VERBUM)
        {
            si (!_verbum_claudere(p))
            {
                redde FALSUM;
            }
        }
        alioquin si (g->genus == (s32)CRUSTA_GENUS_REDIRECTIO)
        {
            si (!_redirectionem_finire(p))
            {
                redde FALSUM;
            }
        }
        alioquin si (g->genus == (s32)CRUSTA_GENUS_HEREDOC)
        {
            si (!_heredoc_claudere(p))
            {
                redde FALSUM;
            }
        }
        alioquin si (!_claudere(p))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* index gradus proximi (a vertice) generis dati cuius status < II
 * (apertus); -I nullus. Programma sistit. */
interior s32
_gradus_quaerere (
    Aedificatio* p,
            s32  genus)
{
    s32 i;

    per (i = (s32)xar_numerus(p->gradus) - I; i >= ZEPHYRUM; i--)
    {
        Gradus* h = (Gradus*)xar_obtinere(p->gradus, (i32)i);

        si (h->genus == genus && h->status < II)
        {
            redde i;
        }
        si (h->genus == (s32)CRUSTA_GENUS_PROGRAMMA)
        {
            frange;
        }
    }
    redde (s32)-I;
}

/* gradum verticis claudere SINE datione (involvendus in catenam/pipam):
 * nodum reddit */
interior MateriaNodus*
_tollere (
    Aedificatio* p)
{
          Gradus* g = _vertex(p);
    MateriaNodus* n = g->nodus;

    xar_removere_ultimum(p->gradus);
    redde n;
}

/* post clausuram verbi: gradus infra reagit */
interior b32
_post_verbum (
    Aedificatio* p);

/* verbum in vertice claudere (si adest) */
interior b32
_verbum_claudere (
    Aedificatio* p)
{
    Gradus* g = _vertex(p);

    si (g != NIHIL && g->genus == (s32)CRUSTA_GENUS_VERBUM)
    {
        si (!_claudere(p))
        {
            redde FALSUM;
        }
        redde _post_verbum(p);
    }
    redde VERUM;
}

interior b32
_aedificator_est (
       Aedificatio* p,
      MateriaNodus* verbum)
{
    chorda v;
       s32 i;

    si (!crusta_verbum_staticum(p->piscina, verbum, &v))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; p->dialectus->aedificatores[i] != NIHIL; i++)
    {
        si (   (i32)strlen(p->dialectus->aedificatores[i]) == v.mensura
            && memcmp(p->dialectus->aedificatores[i], v.datum,
                (size_t)v.mensura) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* redirectio completa: petitio heredoc, gradus clausus */
interior b32
_redirectionem_finire (
    Aedificatio* p)
{
          Gradus* g       = _vertex(p);
    MateriaToken* signum  = NIHIL;

    si (g->nodus->loci[CRUSTA_REDIRECTIO_TOK_OPERATOR].genus
            == MATERIA_VALOR_TOKEN)
    {
        signum =
            g->nodus->loci[CRUSTA_REDIRECTIO_TOK_OPERATOR].datum.token;
    }
    si (   signum != NIHIL && signum->valor.mensura >= (i32)II
        && signum->valor.datum[ZEPHYRUM] == '<'
        && signum->valor.datum[I] == '<'
        && !(signum->valor.mensura == (i32)III
        && signum->valor.datum[II] == '<')
        && g->nodus->loci[CRUSTA_REDIRECTIO_SCOPUS].genus
            == MATERIA_VALOR_NODUS)
    {
        MateriaNodus* scopus =
            g->nodus->loci[CRUSTA_REDIRECTIO_SCOPUS].datum.nodus;
        chorda delimitator;
           b32 tabulae = signum->valor.mensura == (i32)III;

        si (!crusta_verbum_staticum(p->piscina, scopus, &delimitator))
        {
            delimitator.mensura  = ZEPHYRUM;
            delimitator.datum    = NIHIL;
        }
        si (!crusta_lector_heredoc_petere(&p->lector, delimitator,
                crusta_verbum_citatum(scopus), tabulae))
        {
            redde FALSUM;
        }
        {
            MateriaNodus** sedes = (MateriaNodus**)xar_addere(
                p->redirectiones_pendentes);

            si (sedes == NIHIL)
            {
                redde FALSUM;
            }
            *sedes = g->nodus;
        }
        si (p->relatio != NIHIL)
        {
            p->relatio->heredoca++;
        }
    }
    redde _claudere(p);
}

interior b32
_post_verbum (
    Aedificatio* p)
{
    Gradus* g = _vertex(p);

    si (g == NIHIL)
    {
        redde VERUM;
    }
    commutatio (g->genus)
    {
        casus CRUSTA_GENUS_REDIRECTIO:
            redde _redirectionem_finire(p);
        casus CRUSTA_GENUS_ASSIGNATIO:
            redde _claudere(p);
        casus CRUSTA_GENUS_ELECTIO:
            /* verbum electionis positum: 'in' exspectatur */
            si (g->status == ZEPHYRUM)
            {
                g->status = I;
            }
            redde VERUM;
        casus CRUSTA_GENUS_IUDICIUM:
        casus CRUSTA_GENUS_IUDICIUM_INCLUSA:
            /* operandum dextrum '=~' lectum: modus IUDICIUM redit */
            g->modus = CRUSTA_MODUS_IUDICIUM;
            redde VERUM;
        casus CRUSTA_GENUS_IMPERIUM:
            /* titulus aedificatoris (declare, local, ...): verbum
             * unum et ultimum; praefixa assignationes aut redirectiones
             * licent, sed redirectio POST assignationem 'x=(' tollit
             * ('A=1 >o declare c=(z)' bash errat, '>o A=1 declare'
             * non - P11b mensuratum) */
            si (   g->status == I
                && g->nodus->loci[CRUSTA_IMPERIUM_LIBERI].genus
                   == MATERIA_VALOR_LISTA)
            {
                MateriaValor* liberi =
                    &g->nodus->loci[CRUSTA_IMPERIUM_LIBERI];
                          i32 n      = materia_valor_lista_numerus(
                              *liberi);
                          i32 verba   = ZEPHYRUM;
                          b32 visa    = FALSUM;
                          b32 obstat  = FALSUM;
                          i32 k;
                MateriaValor* ultimus;

                per (k = ZEPHYRUM; k < n; k++)
                {
                    MateriaValor* e = materia_valor_lista_obtinere(
                        *liberi, k);

                    si (e->genus != MATERIA_VALOR_NODUS)
                    {
                        perge;
                    }
                    si (e->datum.nodus->genus
                        == (s32)CRUSTA_GENUS_VERBUM)
                    {
                        verba++;
                    }
                    alioquin si (e->datum.nodus->genus
                                 == (s32)CRUSTA_GENUS_ASSIGNATIO)
                    {
                        visa = VERUM;
                    }
                    alioquin si (   visa
                                 && e->datum.nodus->genus
                                 == (s32)CRUSTA_GENUS_REDIRECTIO)
                    {
                        obstat = VERUM;
                    }
                }
                ultimus = n > ZEPHYRUM
                    ? materia_valor_lista_obtinere(*liberi, n
                        - I) : NIHIL;
                si (   verba == (i32)I && !obstat && ultimus != NIHIL
                    && ultimus->genus == MATERIA_VALOR_NODUS
                    && ultimus->datum.nodus->genus
                       == (s32)CRUSTA_GENUS_VERBUM
                    && _aedificator_est(p, ultimus->datum.nodus))
                {
                    g->status  = II;
                    g->modus   = CRUSTA_MODUS_ASSIGNATIONES;
                }
            }
            redde VERUM;
        ordinarius:
            redde VERUM;
    }
}

/* gradus pendentes intra sententiam claudere (verbum, parametrum,
 * redirectio, assignatio) usque ad imperium/pipam/catenam/listam */
interior b32
_intra_sententiam_claudere (
    Aedificatio* p)
{
    Gradus* g;

    dum ((g = _vertex(p)) != NIHIL)
    {
        si (_lista_est(g) || _sententia_est(g))
        {
            redde VERUM;
        }
        si (g->genus == (s32)CRUSTA_GENUS_VERBUM)
        {
            si (!_verbum_claudere(p))
            {
                redde FALSUM;
            }
            perge;
        }
        si (g->genus == (s32)CRUSTA_GENUS_REDIRECTIO)
        {
            si (!_redirectionem_finire(p))
            {
                redde FALSUM;
            }
            perge;
        }
        si (!_claudere(p))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* sententiam pendentem totam claudere (imperium, pipa, catena) usque
 * ad listam */
interior b32
_sententiam_claudere (
    Aedificatio* p)
{
    Gradus* g;

    si (!_intra_sententiam_claudere(p))
    {
        redde FALSUM;
    }
    dum ((g = _vertex(p)) != NIHIL && _sententia_est(g))
    {
        si (!_claudere(p))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* rem pendentem involvendam ('&&' '|') tollere: imperium, compositum
 * clausum, functio, socius; compositum sub functione/socio prius in
 * corpus datur (res = functio). NIHIL si nulla. */
interior MateriaNodus*
_rem_tollere (
    Aedificatio* p)
{
    Gradus* g;
    Gradus* parens;

    dum (VERUM)
    {
        g       = _vertex(p);
        parens  = _infra(p, I);
        si (   parens != NIHIL
            && (   g->genus == (s32)CRUSTA_GENUS_IMPERIUM
                || _compositum_clausum(g))
            && (   parens->genus == (s32)CRUSTA_GENUS_FUNCTIO
                || parens->genus == (s32)CRUSTA_GENUS_SOCIUS))
        {
            si (!_claudere(p))
            {
                redde NIHIL;
            }
            perge;
        }
        frange;
    }
    si (   g->genus == (s32)CRUSTA_GENUS_IMPERIUM
        || g->genus == (s32)CRUSTA_GENUS_FUNCTIO
        || g->genus == (s32)CRUSTA_GENUS_SOCIUS
        || _compositum_clausum(g))
    {
        redde _tollere(p);
    }
    redde NIHIL;
}


/* ==================================================
 * Malum, separator, catena, pipa
 * ================================================== */

/* lexema quod grammatica ponere non potest: sententia pendens
 * clauditur, malum listae appenditur (aut mali ultimo adiacenti) */
interior b32
_malum (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* lista;
    MateriaValor* liberi;
    MateriaNodus* malum = NIHIL;

    si (!_sententiam_claudere(p))
    {
        redde FALSUM;
    }
    /* gradus exspectans (nomen, 'do'...) listam non habet: absens
     * clauditur usque ad listam */
    dum (   (lista = _vertex(p)) != NIHIL
         && _species(lista->genus, lista->locus)
            != MATERIA_LOCUS_LISTA_NODUS)
    {
        si (!_claudere(p) || !_sententiam_claudere(p))
        {
            redde FALSUM;
        }
    }
    liberi  = &lista->nodus->loci[lista->locus];
    si (   liberi->genus == MATERIA_VALOR_LISTA
        && materia_valor_lista_numerus(*liberi) > ZEPHYRUM)
    {
        MateriaValor* ultimus = materia_valor_lista_obtinere(*liberi,
            materia_valor_lista_numerus(*liberi) - I);

        si (   ultimus->genus              == MATERIA_VALOR_NODUS
            && ultimus->datum.nodus->genus == (s32)CRUSTA_GENUS_MALUM)
        {
            malum = ultimus->datum.nodus;
        }
    }
    si (malum == NIHIL)
    {
        malum = _nodus(p, (s32)CRUSTA_GENUS_MALUM);
        si (malum == NIHIL)
        {
            redde FALSUM;
        }
        si (!_nodum_dare(p, lista->nodus, lista->locus, malum))
        {
            redde FALSUM;
        }
        si (p->relatio != NIHIL)
        {
            p->relatio->mala++;
        }
    }
    redde _token_appendere(p, malum, (i32)CRUSTA_MALUM_TOKENS, t);
}

interior b32
_separator (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* lista;
    MateriaNodus* s;
    Gradus* g;

    si (!_verbum_claudere(p))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    /* 'for i in a b;' / 'for ((;;));' : separator verborum in
     * lexema proprium; '&' ibi malum (bash idem) */
    si (   (   (g->genus == (s32)CRUSTA_GENUS_ITERATIO
                && (g->status == I || g->status == III))
            || (g->genus == (s32)CRUSTA_GENUS_CYCLUS && g->status == I))
        && t->valor.datum[ZEPHYRUM] != '&')
    {
                i32 locus = g->genus == (s32)CRUSTA_GENUS_ITERATIO
                    ? (i32)CRUSTA_ITERATIO_SEPARATOR
                    : (i32)CRUSTA_CYCLUS_SEPARATOR;

        /* lista: nodus separator, deinde corpora heredoc quae lineam
         * novam sequuntur */
        s = _nodus(p, (s32)CRUSTA_GENUS_SEPARATOR);
        si (   s == NIHIL
            || !_token_ponere(s, (i32)CRUSTA_SEPARATOR_TOK, t)
            || !_nodum_dare(p, g->nodus, locus, s))
        {
            redde FALSUM;
        }
        g->status  = IV;
        g->locus   = locus;
        g->modus   = CRUSTA_MODUS_INITIUM;
        redde VERUM;
    }
    /* separator sine sententia pendente (';' post ';', ';' initio):
     * bash 'syntax error near unexpected token' - malum */
    si (_lista_est(g))
    {
        redde _malum(p, t);
    }
    si (!_sententiam_claudere(p))
    {
        redde FALSUM;
    }
    lista  = _vertex(p);
    si (_species(lista->genus, lista->locus)
        != MATERIA_LOCUS_LISTA_NODUS)
    {
        redde _malum(p, t);   /* gradus exspectans: 'for i ;;'... */
    }
    s      = _nodus(p, (s32)CRUSTA_GENUS_SEPARATOR);
    si (s == NIHIL || !_token_ponere(s, (i32)CRUSTA_SEPARATOR_TOK, t))
    {
        redde FALSUM;
    }
    redde _nodum_dare(p, lista->nodus, lista->locus, s);
}

interior b32
_operator_appendere (
       Aedificatio* p,
      MateriaNodus* parens,
               i32  locus,
      MateriaToken* t)
{
    MateriaNodus* o = _nodus(p, (s32)CRUSTA_GENUS_OPERATOR);

    si (o == NIHIL || !_token_ponere(o, (i32)CRUSTA_OPERATOR_TOK, t))
    {
        redde FALSUM;
    }
    redde _nodum_dare(p, parens, locus, o);
}

/* '&&' '||': rem pendentem (imperium aut pipa) in catenam involvere */
interior b32
_catena (
       Aedificatio* p,
      MateriaToken* t)
{
          Gradus* g;
    MateriaNodus* res = NIHIL;
    MateriaNodus* catena;

    si (!_intra_sententiam_claudere(p))
    {
        redde FALSUM;
    }
    res  = _rem_tollere(p);
    g    = _vertex(p);
    si (g->genus == (s32)CRUSTA_GENUS_PIPA)
    {
        si (res != NIHIL && !_nodum_dare(p, g->nodus, g->locus, res))
        {
            redde FALSUM;
        }
        res  = _tollere(p);
        g    = _vertex(p);
    }
    si (g->genus != (s32)CRUSTA_GENUS_CATENA)
    {
        catena = _nodus(p, (s32)CRUSTA_GENUS_CATENA);
        si (   catena == NIHIL
            || _pellere(p, catena, (i32)CRUSTA_CATENA_LIBERI,
                CRUSTA_MODUS_INITIUM) == NIHIL)
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    si (res != NIHIL && !_nodum_dare(p, g->nodus, g->locus, res))
    {
        redde FALSUM;
    }
    redde _operator_appendere(p, g->nodus, g->locus, t);
}

/* '|' '|&': imperium pendens in pipam involvere */
interior b32
_pipa (
       Aedificatio* p,
      MateriaToken* t)
{
          Gradus* g;
    MateriaNodus* res = NIHIL;
    MateriaNodus* pipa;

    si (!_intra_sententiam_claudere(p))
    {
        redde FALSUM;
    }
    res  = _rem_tollere(p);
    g    = _vertex(p);
    si (g->genus != (s32)CRUSTA_GENUS_PIPA)
    {
        pipa = _nodus(p, (s32)CRUSTA_GENUS_PIPA);
        si (   pipa == NIHIL
            || _pellere(p, pipa, (i32)CRUSTA_PIPA_LIBERI,
                CRUSTA_MODUS_INITIUM) == NIHIL)
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    si (res != NIHIL && !_nodum_dare(p, g->nodus, g->locus, res))
    {
        redde FALSUM;
    }
    redde _operator_appendere(p, g->nodus, g->locus, t);
}

/* '!' aut 'time' in positione imperii: pipa cum praefixis */
interior b32
_praefixum_pipae (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g = _vertex(p);
    MateriaNodus* pipa;

    /* 'a | time b' (bash licet): pipa interior ut membrum; 'a | ! b'
     * bash reicit - malum */
    si (   g->genus == (s32)CRUSTA_GENUS_PIPA
        && !_absens(g->nodus, (i32)CRUSTA_PIPA_LIBERI))
    {
        si (t->valor.datum[ZEPHYRUM] == '!')
        {
            redde _malum(p, t);
        }
        g = NIHIL;
    }
    si (g == NIHIL || g->genus != (s32)CRUSTA_GENUS_PIPA)
    {
        pipa = _nodus(p, (s32)CRUSTA_GENUS_PIPA);
        si (   pipa == NIHIL
            || _pellere(p, pipa, (i32)CRUSTA_PIPA_LIBERI,
                CRUSTA_MODUS_INITIUM) == NIHIL)
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    redde _token_appendere(p, g->nodus, (i32)CRUSTA_PIPA_PRAEFIXA, t);
}

/* 'time -p': '-p' verbum post 'time' in pipa sine imperio */
interior b32
_optio_temporis (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g = _vertex(p);
    MateriaValor* praefixa;
    MateriaValor* ultimum;

    si (   g->genus                 != (s32)CRUSTA_GENUS_PIPA
        || t->genus                 != (s32)CRUSTA_LEX_LITTERALIS
        || t->valor.mensura         != (i32)II
        || t->valor.datum[ZEPHYRUM] != '-'
        || t->valor.datum[I]        != 'p')
    {
        redde FALSUM;
    }
    praefixa = &g->nodus->loci[CRUSTA_PIPA_PRAEFIXA];
    si (   praefixa->genus != MATERIA_VALOR_LISTA
        || !_absens(g->nodus, (i32)CRUSTA_PIPA_LIBERI))
    {
        redde FALSUM;
    }
    ultimum = materia_valor_lista_obtinere(*praefixa,
        materia_valor_lista_numerus(*praefixa) - I);
    si (   ultimum->genus                      != MATERIA_VALOR_TOKEN
        || ultimum->datum.token->valor.mensura != (i32)IV
        || memcmp(ultimum->datum.token->valor.datum, "time", (size_t)IV)
            != ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde _token_appendere(p, g->nodus, (i32)CRUSTA_PIPA_PRAEFIXA, t);
}


/* ==================================================
 * Imperium, assignatio, redirectio
 * ================================================== */

/* imperium aperire si vertex lista, catena aut pipa est */
interior b32
_imperium_aperire (
    Aedificatio* p)
{
    Gradus* g = _vertex(p);
    MateriaNodus* imperium;

    si (g->genus == (s32)CRUSTA_GENUS_IMPERIUM)
    {
        redde VERUM;
    }
    imperium = _nodus(p, (s32)CRUSTA_GENUS_IMPERIUM);
    si (imperium == NIHIL)
    {
        redde FALSUM;
    }
    redde _pellere(p, imperium, (i32)CRUSTA_IMPERIUM_LIBERI,
        CRUSTA_MODUS_INITIUM) != NIHIL;
}

/* modus imperii post liberum primum: VERBA post verbum, aliter
 * ASSIGNATIONES (assignationes et redirectiones ante titulum; linea
 * nova terminat - 'X=1' + linea nova imperium finit, 'A=1 if' verbum
 * 'if' nudum est, bash idem) */
interior CrustaModus
_modus_imperii (
    constans Gradus* g)
{
    si (g->status == I)
    {
        redde CRUSTA_MODUS_VERBA;
    }
    redde CRUSTA_MODUS_ASSIGNATIONES;
}

interior b32
_assignatio_incipere (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g;
    MateriaNodus* a;
    i32 locus = t->genus == (s32)CRUSTA_LEX_ASSIGNATIO_TITULUS
        ? (i32)CRUSTA_ASSIGNATIO_TOK_TITULUS
        : (i32)CRUSTA_ASSIGNATIO_TOK_SUBSCRIPTUM;

    si (!_verbum_claudere(p))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (g->genus != (s32)CRUSTA_GENUS_TABULATUM)
    {
        /* post compositum clausum aut in gradu exspectante:
         * malum ('{ a; } x=1', bash idem) */
        si (   g->genus != (s32)CRUSTA_GENUS_IMPERIUM
            && !_sententiam_admittit(g))
        {
            redde _malum(p, t);
        }
        si (!_imperium_aperire(p))
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    a = _nodus(p, (s32)CRUSTA_GENUS_ASSIGNATIO);
    si (a == NIHIL || !_token_ponere(a, locus, t))
    {
        redde FALSUM;
    }
    redde _pellere(p, a, (i32)CRUSTA_ASSIGNATIO_VALOR,
        g->genus == (s32)CRUSTA_GENUS_TABULATUM ? CRUSTA_MODUS_TABULATUM
            : _modus_imperii(g)) != NIHIL;
}

interior b32
_redirectio_incipere (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g;
    MateriaNodus* r;

    si (!_verbum_claudere(p))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (   g->genus == (s32)CRUSTA_GENUS_REDIRECTIO
        && t->genus == (s32)CRUSTA_LEX_REDIRECTIO_OPERATOR
        && _absens(g->nodus, (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR))
    {
        redde _token_ponere(g->nodus,
            (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR, t);
    }
    si (g->genus == (s32)CRUSTA_GENUS_REDIRECTIO)
    {
        /* redirectio sine scopo (2>) ante alteram: clausa absens */
        si (!_redirectionem_finire(p))
        {
            redde FALSUM;
        }
    }
    si (g->genus == (s32)CRUSTA_GENUS_ASSIGNATIO)
    {
        si (!_claudere(p))
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    /* functio corpus exspectans: 'f() > x' - functio absens clauditur,
     * redirectio imperium suum aperit (bash: error) */
    si (_functio_corpus_exspectat(g))
    {
        si (!_claudere(p))
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    si (   !_compositum_clausum(g)
        && g->genus != (s32)CRUSTA_GENUS_IMPERIUM
        && !_sententiam_admittit(g))
    {
        redde _malum(p, t);   /* gradus exspectans ('for i > x') */
    }
    si (!_compositum_clausum(g) && !_imperium_aperire(p))
    {
        redde FALSUM;
    }
    r = _nodus(p, (s32)CRUSTA_GENUS_REDIRECTIO);
    si (r == NIHIL)
    {
        redde FALSUM;
    }
    si (!_token_ponere(r, t->genus == (s32)CRUSTA_LEX_REDIRECTIO_FD
            ? (i32)CRUSTA_REDIRECTIO_TOK_FD
            : (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR, t))
    {
        redde FALSUM;
    }
    redde _pellere(p, r, (i32)CRUSTA_REDIRECTIO_SCOPUS,
        CRUSTA_MODUS_VERBA)
        != NIHIL;
}


/* ==================================================
 * Partes verbi
 * ================================================== */

interior b32
_pars_simplex (
    s32  genus_lexematis,
    s32* genus_partis)
{
    commutatio (genus_lexematis)
    {
        casus CRUSTA_LEX_LITTERALIS:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_LITTERALIS;
            redde VERUM;
        casus CRUSTA_LEX_EFFUGIUM:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_EFFUGIUM;
            redde VERUM;
        casus CRUSTA_LEX_CONTINUATIO:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_CONTINUATIO;
            redde VERUM;
        casus CRUSTA_LEX_SIMPLEX:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_SIMPLEX;
            redde VERUM;
        casus CRUSTA_LEX_EFFUGIA:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_EFFUGIA;
            redde VERUM;
        casus CRUSTA_LEX_DOMUS:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_DOMUS;
            redde VERUM;
        casus CRUSTA_LEX_REGULA:
            /* operandum dextrum '=~': pars litteralis cuius lexema
             * genus REGULA servat (lint distinguit) */
            *genus_partis = (s32)CRUSTA_GENUS_PARS_LITTERALIS;
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

interior b32
_pars_est (
    s32 genus_lexematis)
{
    s32 g;

    redde _pars_simplex(genus_lexematis, &g)
        || genus_lexematis == (s32)CRUSTA_LEX_GEMINA_APERTURA
        || genus_lexematis == (s32)CRUSTA_LEX_VERSA_APERTURA
        || genus_lexematis == (s32)CRUSTA_LEX_EXPANSIO_APERTURA
        || genus_lexematis == (s32)CRUSTA_LEX_SUBSTITUTIO_APERTURA
        || genus_lexematis == (s32)CRUSTA_LEX_GRAVIS
        || genus_lexematis == (s32)CRUSTA_LEX_PROCESSUS_APERTURA
        || genus_lexematis
            == (s32)CRUSTA_LEX_ARITHMETICA_PARTIS_APERTURA
        || genus_lexematis == (s32)CRUSTA_LEX_PARAMETRUM_SIGILLUM;
}

/* gradus qui partes directe recipit (gemina, versa, heredoc) an
 * verbum poscat */
interior b32
_partes_directe (
    s32 genus)
{
    redde genus == (s32)CRUSTA_GENUS_PARS_GEMINA
        || genus == (s32)CRUSTA_GENUS_PARS_VERSA
        || genus == (s32)CRUSTA_GENUS_HEREDOC;
}

interior b32
_malum (
       Aedificatio* p,
      MateriaToken* t);

interior b32
_optio_aperire (
       Aedificatio* p,
      MateriaToken* parenthesis);

/* nomen (for/select/function): lexema litterale unum in locum */
interior b32
_titulum_ponere (
            Gradus* g,
      MateriaToken* t,
               i32  locus)
{
    si (!_token_ponere(g->nodus, locus, t))
    {
        redde FALSUM;
    }
    g->status = I;
    redde VERUM;
}

/* verbum recipiens invenire aut aperire pro parte ad t; *praeparatum
 * FALSUM = lexema consumptum aliter (nomen, optio temporis, malum) */
interior b32
_verbum_parare (
       Aedificatio* p,
      MateriaToken* t,
               b32* praeparatum)
{
    Gradus* g = _vertex(p);
    MateriaNodus* verbum;
    CrustaModus modus;

    *praeparatum = FALSUM;
    si (g->genus == (s32)CRUSTA_GENUS_VERBUM)
    {
        si (g->verbi_finis == t->byte_offset)
        {
            *praeparatum = VERUM;
            redde VERUM;
        }
        si (!_verbum_claudere(p))
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    si (_partes_directe(g->genus) || g->machina != NIHIL)
    {
        *praeparatum = VERUM;
        redde VERUM;
    }
    /* 'time -p' */
    si (_optio_temporis(p, t))
    {
        redde VERUM;   /* consumptum */
    }
    /* functio corpus exspectans: verbum non est corpus (bash: error)
     * - functio absens clauditur, imperium novum sequitur */
    dum (_functio_corpus_exspectat(g))
    {
        si (!_claudere(p))
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    /* socius: verbum primum litterale candidatus est (titulus an
     * imperium? lexema sequens decernit); aliud imperium aperit */
    si (g->genus == (s32)CRUSTA_GENUS_SOCIUS && g->status == ZEPHYRUM)
    {
        si (t->genus == (s32)CRUSTA_LEX_LITTERALIS)
        {
            g->candidatus  = t;
            g->status      = I;
            redde VERUM;
        }
        g->status = II;
    }
    modus = g->modus;
    si (g->genus == (s32)CRUSTA_GENUS_IMPERIUM)
    {
        si (g->status == ZEPHYRUM)
        {
            g->status  = I;
            g->modus   = CRUSTA_MODUS_VERBA;
        }
        modus = g->modus;
    }
    alioquin si (g->genus == (s32)CRUSTA_GENUS_ITERATIO)
    {
        /* nomen (verba reservata licent: 'for if in'), aut verba
         * post 'in' */
        si (   g->status == ZEPHYRUM
            && (   t->genus == (s32)CRUSTA_LEX_LITTERALIS
                || t->genus == (s32)CRUSTA_LEX_RESERVATUM))
        {
            redde _titulum_ponere(g, t,
                (i32)CRUSTA_ITERATIO_TOK_TITULUS);
        }
        si (g->status != III)
        {
            redde _malum(p, t);
        }
        modus = CRUSTA_MODUS_IN_VERBIS;
    }
    alioquin si (g->genus == (s32)CRUSTA_GENUS_FUNCTIO)
    {
        si (   g->status == ZEPHYRUM
            && t->genus  == (s32)CRUSTA_LEX_LITTERALIS)
        {
            redde _titulum_ponere(g, t,
                (i32)CRUSTA_FUNCTIO_TOK_TITULUS);
        }
        redde _malum(p, t);
    }
    alioquin si (g->genus == (s32)CRUSTA_GENUS_ELECTIO)
    {
        si (g->status == ZEPHYRUM)
        {
            modus = CRUSTA_MODUS_POST_TITULUM;   /* verbum electionis */
        }
        alioquin si (g->locus == (i32)CRUSTA_ELECTIO_LIBERI)
        {
            /* exemplar sine '(' : optio nova (incohata: IN_VERBIS) */
            si (!_optio_aperire(p, NIHIL))
            {
                redde FALSUM;
            }
            g      = _vertex(p);
            modus  = CRUSTA_MODUS_IN_VERBIS;
        }
        alioquin
        {
            redde _malum(p, t);
        }
    }
    alioquin si (   g->genus == (s32)CRUSTA_GENUS_OPTIO
                 && g->locus == (i32)CRUSTA_OPTIO_EXEMPLARIA)
    {
        modus = CRUSTA_MODUS_IN_VERBIS;
    }
    alioquin si (g->genus == (s32)CRUSTA_GENUS_PARS_EXPANSIO)
    {
        modus = g->divisor == (s32)'/'
            ? CRUSTA_MODUS_EXPANSIO_EXEMPLAR
            : g->divisor == (s32)':' ? CRUSTA_MODUS_EXPANSIO_SECTIO
            : CRUSTA_MODUS_EXPANSIO_VERBUM;
    }
    alioquin si (   g->genus == (s32)CRUSTA_GENUS_IUDICIUM
                 || g->genus == (s32)CRUSTA_GENUS_IUDICIUM_INCLUSA
                 || g->genus == (s32)CRUSTA_GENUS_PARS_PARAMETRUM
                 || g->genus == (s32)CRUSTA_GENUS_REDIRECTIO
                 || g->genus == (s32)CRUSTA_GENUS_ASSIGNATIO
                 || g->genus == (s32)CRUSTA_GENUS_TABULATUM)
    {
        modus = g->modus;
    }
    alioquin
    {
        /* positio imperii - aut nihil (post compositum clausum,
         * gradus exspectans): malum */
        si (!_sententiam_admittit(g))
        {
            redde _malum(p, t);
        }
        si (!_imperium_aperire(p))
        {
            redde FALSUM;
        }
        g          = _vertex(p);
        g->status  = I;
        g->modus   = CRUSTA_MODUS_VERBA;
        modus      = g->modus;
    }
    verbum = _nodus(p, (s32)CRUSTA_GENUS_VERBUM);
    si (verbum == NIHIL)
    {
        redde FALSUM;
    }
    *praeparatum = VERUM;
    redde _pellere(p, verbum, (i32)CRUSTA_VERBUM_PARTES, modus)
        != NIHIL;
}

/* finem verbi renovare post partem (simplicem aut compositam
 * clausam) */
interior vacuum
_verbi_finem_ponere (
    Aedificatio* p,
            s32  finis)
{
    Gradus* g = _vertex(p);

    si (g != NIHIL && g->genus == (s32)CRUSTA_GENUS_VERBUM)
    {
        g->verbi_finis = finis;
    }
}

interior b32
_partem_recipere (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g;
    MateriaNodus* pars;
    s32 genus_partis;
    b32 praeparatum;

    si (!_verbum_parare(p, t, &praeparatum))
    {
        redde FALSUM;
    }
    si (!praeparatum)
    {
        redde VERUM;   /* consumptum aliter */
    }
    g = _vertex(p);
    /* finis verbi = finis huius partis (pars composita eum in clausura
     * renovat) */
    _verbi_finem_ponere(p, _token_finis(t));

    si (g->machina != NIHIL && _pars_simplex(t->genus, &genus_partis))
    {
        redde _malum(p, t);   /* octetus alienus in arithmetica */
    }
    si (_pars_simplex(t->genus, &genus_partis))
    {
        pars = _nodus(p, genus_partis);
        si (   pars == NIHIL
            || !_token_ponere(pars, (i32)CRUSTA_PARS_TOK, t)
            || !_nodum_dare(p, g->nodus, g->locus, pars))
        {
            redde FALSUM;
        }
        _verbi_finem_ponere(p, _token_finis(t));
        redde VERUM;
    }
    commutatio (t->genus)
    {
        casus CRUSTA_LEX_PARAMETRUM_SIGILLUM:
            pars = _nodus(p, (s32)CRUSTA_GENUS_PARS_PARAMETRUM);
            si (   pars == NIHIL
                || !_token_ponere(pars,
                (i32)CRUSTA_PARAMETRUM_TOK_SIGILLUM,
                    t))
            {
                redde FALSUM;
            }
            redde _pellere(p, pars, (i32)CRUSTA_PARAMETRUM_TOK_TITULUS,
                g->modus) != NIHIL;
        casus CRUSTA_LEX_GEMINA_APERTURA:
        casus CRUSTA_LEX_VERSA_APERTURA:
            pars = _nodus(p, t->genus == (s32)CRUSTA_LEX_GEMINA_APERTURA
                ? (s32)CRUSTA_GENUS_PARS_GEMINA
                : (s32)CRUSTA_GENUS_PARS_VERSA);
            si (   pars == NIHIL
                || !_token_ponere(pars,
                (i32)CRUSTA_GEMINA_TOK_APERTURA, t))
            {
                redde FALSUM;
            }
            redde _pellere(p, pars, (i32)CRUSTA_GEMINA_PARTES,
                CRUSTA_MODUS_GEMINA) != NIHIL;
        casus CRUSTA_LEX_EXPANSIO_APERTURA:
            pars = _nodus(p, (s32)CRUSTA_GENUS_PARS_EXPANSIO);
            si (   pars == NIHIL
                || !_token_ponere(pars,
                (i32)CRUSTA_EXPANSIO_TOK_APERTURA,
                    t))
            {
                redde FALSUM;
            }
            redde _pellere(p, pars, (i32)CRUSTA_EXPANSIO_ARGUMENTA,
                CRUSTA_MODUS_EXPANSIO) != NIHIL;
        casus CRUSTA_LEX_SUBSTITUTIO_APERTURA:
        casus CRUSTA_LEX_PROCESSUS_APERTURA:
            pars = _nodus(p, t->genus
                == (s32)CRUSTA_LEX_SUBSTITUTIO_APERTURA
                ? (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO
                : (s32)CRUSTA_GENUS_PARS_PROCESSUS);
            si (   pars == NIHIL
                || !_token_ponere(pars,
                    (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA, t))
            {
                redde FALSUM;
            }
            redde _pellere(p, pars, (i32)CRUSTA_SUBSTITUTIO_LIBERI,
                CRUSTA_MODUS_INITIUM) != NIHIL;
        casus CRUSTA_LEX_ARITHMETICA_PARTIS_APERTURA:
        {
                       Gradus* novus;
            CrustaArithmetica* m;

            pars = _nodus(p, (s32)CRUSTA_GENUS_PARS_ARITHMETICA);
            m = (CrustaArithmetica*)piscina_allocare(p->piscina,
                (memoriae_index)magnitudo(CrustaArithmetica));
            si (   pars == NIHIL || m == NIHIL
                || !_token_ponere(pars,
                    (i32)CRUSTA_PARS_ARITHMETICA_TOK_APERTURA, t))
            {
                redde FALSUM;
            }
            crusta_arithmetica_incipere(m, p->piscina, p->dialectus);
            novus = _pellere(p, pars,
                (i32)CRUSTA_PARS_ARITHMETICA_EXPRESSIO,
                CRUSTA_MODUS_ARITHMETICA_SUMMA);
            si (novus == NIHIL)
            {
                redde FALSUM;
            }
            novus->machina = m;
            novus->apertura = t;
            novus->prior_antea = p->prior_antea;
            novus->situs_initii = crusta_lector_situs(&p->lector);
                        novus->situs_initii.cursor = t->byte_offset;
            novus->lexemata_ante = p->lexemata == NIHIL ? ZEPHYRUM
                : xar_numerus(p->lexemata) - I;
            redde VERUM;
        }
        casus CRUSTA_LEX_GRAVIS:
        {
               s32  finis;
               i32  gravis = crusta_lector_cauda(t)->gravis + I;
            Gradus* novus;

            pars = _nodus(p, (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO);
            si (   pars == NIHIL
                || !_token_ponere(pars,
                    (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA, t))
            {
                redde FALSUM;
            }
            finis = crusta_lector_gravem_quaerere(&p->lector,
                p->lector.situs.cursor, gravis);
            si (finis < ZEPHYRUM)
            {
                i32 n = xar_numerus(p->lector.regiones);

                finis = n > ZEPHYRUM
                    ? ((CrustaRegio*)xar_obtinere(p->lector.regiones,
                        n - I))->finis
                    : (s32)p->lector.mensura;
            }
            si (!crusta_lector_regionem_aperire(&p->lector, finis,
                gravis))
            {
                redde FALSUM;
            }
            novus = _pellere(p, pars, (i32)CRUSTA_SUBSTITUTIO_LIBERI,
                CRUSTA_MODUS_INITIUM);
            si (novus == NIHIL)
            {
                redde FALSUM;
            }
            novus->gravis         = VERUM;
            novus->heredoca_ante  = xar_numerus(p->lector.heredoca);
            redde VERUM;
        }
        ordinarius:
            redde FALSUM;
    }
}

/* pars composita in vertice clausa (lexema clausurae positum): gradum
 * claudere et finem verbi renovare */
interior b32
_partem_claudere (
       Aedificatio* p,
      MateriaToken* clausura)
{
    si (!_claudere(p))
    {
        redde FALSUM;
    }
    _verbi_finem_ponere(p, _token_finis(clausura));
    redde VERUM;
}


/* ==================================================
 * Expansio ${ }
 * ================================================== */

interior b32
_expansio (
       Aedificatio* p,
            Gradus* g,
      MateriaToken* t)
{
    commutatio (t->genus)
    {
        casus CRUSTA_LEX_EXPANSIO_PRAEFIXUM:
            redde _token_ponere(g->nodus,
                (i32)CRUSTA_EXPANSIO_TOK_PRAEFIXUM, t);
        casus CRUSTA_LEX_PARAMETRUM_TITULUS:
            g->status = I;
            redde _token_ponere(g->nodus,
                (i32)CRUSTA_EXPANSIO_TOK_TITULUS,
                t);
        casus CRUSTA_LEX_SUBSCRIPTUM:
            redde _token_ponere(g->nodus,
                (i32)CRUSTA_EXPANSIO_TOK_SUBSCRIPTUM, t);
        casus CRUSTA_LEX_EXPANSIO_OPERATOR:
            si (g->status < II)
            {
                g->status = II;
                si (t->valor.datum[ZEPHYRUM] == '/')
                {
                    g->divisor = (s32)'/';
                }
                alioquin si (   t->valor.mensura         == (i32)I
                             && t->valor.datum[ZEPHYRUM] == ':')
                {
                    g->divisor = (s32)':';
                }
                g->modus = g->divisor == (s32)'/'
                    ? CRUSTA_MODUS_EXPANSIO_EXEMPLAR
                    : g->divisor
                        == (s32)':' ? CRUSTA_MODUS_EXPANSIO_SECTIO
                    : CRUSTA_MODUS_EXPANSIO_VERBUM;
                redde _token_ponere(g->nodus,
                    (i32)CRUSTA_EXPANSIO_TOK_OPERATOR, t);
            }
            /* divisor intra argumenta: operator nudus; post '/' primum
             * substitutum sequitur sine divisore */
            si (g->divisor == (s32)'/')
            {
                g->divisor  = ZEPHYRUM;
                g->modus    = CRUSTA_MODUS_EXPANSIO_VERBUM;
            }
            redde _operator_appendere(p, g->nodus, g->locus, t);
        casus CRUSTA_LEX_EXPANSIO_CLAUSURA:
            si (!_token_ponere(g->nodus,
                (i32)CRUSTA_EXPANSIO_TOK_CLAUSURA,
                    t))
            {
                redde FALSUM;
            }
            redde _partem_claudere(p, t);
        ordinarius:
            /* pars in argumentis (status < II: titulus ruptus -
             * totalitas) */
            g->status = II;
            redde _partem_recipere(p, t);
    }
}


/* ==================================================
 * Arithmetica in aedificatore
 * ================================================== */

/* '$((' recusatum (bash: substitutio imperii temptatur): gradus ad
 * partem arithmeticam sublati, situs repositus, lexema '$((' ut '$('
 * '(' relegendum; trivia aperturae in pendentia redeunt */
interior b32
_arithmeticam_recusare (
    Aedificatio* p)
{
         Gradus* g;
    CrustaSitus  s;
            i32  j;

    dum (   (g = _vertex(p)) != NIHIL
         && g->genus         != (s32)CRUSTA_GENUS_PARS_ARITHMETICA)
    {
        xar_removere_ultimum(p->gradus);
    }
    si (g == NIHIL)
    {
        redde FALSUM;
    }
    s                           = g->situs_initii;
        s.arithmetica_recusata  = g->apertura->byte_offset;
    crusta_lector_situm_reponere(&p->lector, s);
    xar_vacare(p->pendentia);
    /* lexemata temptationis sublata (apertura ipsa relegitur) */
    dum (   p->lexemata != NIHIL
         && xar_numerus(p->lexemata) > g->lexemata_ante)
    {
        xar_removere_ultimum(p->lexemata);
    }
    /* vexillum lineae: status ante aperturam = vexillum aperturae */
    p->post_lineam = materia_token_initium_lineae(g->apertura);
    per (j = ZEPHYRUM; j < g->apertura->numerus_ante; j++)
    {
        si (!_cumulare(p, g->apertura->spatia_ante[j]))
        {
            redde FALSUM;
        }
    }
    p->prior = g->prior_antea;
    {
        MateriaToken* apertura = g->apertura;

        xar_removere_ultimum(p->gradus);
        /* verbum infra: '$(' relectum ei adiacens */
        g = _vertex(p);
        si (g != NIHIL && g->genus == (s32)CRUSTA_GENUS_VERBUM)
        {
            g->verbi_finis = apertura->byte_offset;
        }
    }
    redde VERUM;
}

/* '))' : machina finita, clausura posita */
interior b32
_arithmeticam_claudere (
     Aedificatio* p,
          Gradus* g,
    MateriaToken* t)
{
    si (!_machinam_finire(p, g))
    {
        redde FALSUM;
    }
    si (g->genus == (s32)CRUSTA_GENUS_PARS_ARITHMETICA)
    {
        si (!_token_ponere(g->nodus,
                (i32)CRUSTA_PARS_ARITHMETICA_TOK_CLAUSURA, t))
        {
            redde FALSUM;
        }
        redde _partem_claudere(p, t);
    }
    /* cyclus 'for ((...))': separator aut 'do' exspectatur */
    si (g->genus == (s32)CRUSTA_GENUS_CYCLUS)
    {
        si (!_token_ponere(g->nodus,
                (i32)CRUSTA_CYCLUS_TOK_PARENTHESIS_CLAUSURA, t))
        {
            redde FALSUM;
        }
                g->status  = I;
        g->locus           = (i32)CRUSTA_CYCLUS_SEPARATOR;
        g->modus           = CRUSTA_MODUS_POST_COMPOSITUM;
        redde VERUM;
    }
    /* imperium (( )): clausum, redirectiones exspectat, sententia
     * pendens manet */
    si (!_token_ponere(g->nodus, (i32)CRUSTA_ARITHMETICA_TOK_CLAUSURA,
        t))
    {
        redde FALSUM;
    }
    g->status  = II;
    g->locus   = (i32)CRUSTA_ARITHMETICA_REDIRECTIONES;
    g->modus   = CRUSTA_MODUS_POST_COMPOSITUM;
    redde VERUM;
}

/* cyclus: ';' inter clausulas - expressio finita liberis appenditur,
 * operator appenditur, machina nova */
interior b32
_cyclus_separator (
       Aedificatio* p,
            Gradus* g,
      MateriaToken* t)
{
    CrustaArithmetica* m;

    si (!_machinam_finire(p, g))
    {
        redde FALSUM;
    }
    si (!_operator_appendere(p, g->nodus, (i32)CRUSTA_CYCLUS_LIBERI, t))
    {
        redde FALSUM;
    }
    m = (CrustaArithmetica*)piscina_allocare(p->piscina,
        (memoriae_index)magnitudo(CrustaArithmetica));
    si (m == NIHIL)
    {
        redde FALSUM;
    }
    crusta_arithmetica_incipere(m, p->piscina, p->dialectus);
    g->machina = m;
    redde VERUM;
}

/* lexemata in gradu cum machina; *tractatum FALSUM = lexema
 * aedificatori communi relinquitur (partes) */
interior b32
_arithmeticam_tractare (
     Aedificatio* p,
          Gradus* g,
    MateriaToken* t,
             b32* tractatum)
{
    MateriaNodus* n;

    *tractatum = VERUM;
    commutatio (t->genus)
    {
        casus CRUSTA_LEX_NUMERUS:
            n = _nodus(p, (s32)CRUSTA_GENUS_NUMERUS);
            si (   n == NIHIL
                || !_token_ponere(n, (i32)CRUSTA_NUMERUS_TOK, t))
            {
                redde FALSUM;
            }
            g->variabile = NIHIL;
            redde crusta_arithmetica_operandum(g->machina, n);
        casus CRUSTA_LEX_VARIABILIS:
            n = _nodus(p, (s32)CRUSTA_GENUS_VARIABILIS);
            si (   n == NIHIL
                || !_token_ponere(n, (i32)CRUSTA_VARIABILIS_TOK_TITULUS,
                t))
            {
                redde FALSUM;
            }
            g->variabile = n;
            redde crusta_arithmetica_operandum(g->machina, n);
        casus CRUSTA_LEX_SUBSCRIPTUM:
            si (   g->variabile != NIHIL
                && _absens(g->variabile,
                (i32)CRUSTA_VARIABILIS_TOK_SUBSCRIPTUM))
            {
                redde _token_ponere(g->variabile,
                    (i32)CRUSTA_VARIABILIS_TOK_SUBSCRIPTUM, t);
            }
            redde _malum(p, t);
        casus CRUSTA_LEX_ARITHMETICA_OPERATOR:
            g->variabile = NIHIL;
            redde crusta_arithmetica_operator(g->machina, t);
        casus CRUSTA_LEX_PARENTHESIS:
        {
            CrustaArithmetica* m;
                       Gradus* novus;

            n = _nodus(p, (s32)CRUSTA_GENUS_INCLUSA);
            m = (CrustaArithmetica*)piscina_allocare(p->piscina,
                (memoriae_index)magnitudo(CrustaArithmetica));
            si (   n == NIHIL || m == NIHIL
                || !_token_ponere(n, (i32)CRUSTA_INCLUSA_TOK_APERTURA,
                t))
            {
                redde FALSUM;
            }
            crusta_arithmetica_incipere(m, p->piscina, p->dialectus);
            novus = _pellere(p, n, (i32)CRUSTA_INCLUSA_EXPRESSIO,
                CRUSTA_MODUS_ARITHMETICA_INTRA);
            si (novus == NIHIL)
            {
                redde FALSUM;
            }
            novus->machina = m;
            redde VERUM;
        }
        casus CRUSTA_LEX_PARENTHESIS_CLAUSURA:
            si (g->genus == (s32)CRUSTA_GENUS_INCLUSA)
            {
                si (!_token_ponere(g->nodus,
                    (i32)CRUSTA_INCLUSA_TOK_CLAUSURA,
                        t))
                {
                    redde FALSUM;
                }
                redde _claudere(p);
            }
            si (g->genus == (s32)CRUSTA_GENUS_PARS_ARITHMETICA)
            {
                redde _arithmeticam_recusare(p);
            }
            redde _malum(p, t);
        casus CRUSTA_LEX_ARITHMETICA_CLAUSURA:
            redde _arithmeticam_claudere(p, g, t);
        casus CRUSTA_LEX_ARITHMETICA_SEPARATOR:
            si (g->genus == (s32)CRUSTA_GENUS_PARS_ARITHMETICA)
            {
                redde _arithmeticam_recusare(p);
            }
            si (g->genus == (s32)CRUSTA_GENUS_CYCLUS)
            {
                redde _cyclus_separator(p, g, t);
            }
            redde _malum(p, t);
        ordinarius:
            *tractatum = FALSUM;
            redde VERUM;
    }
}


/* ==================================================
 * Composita: aperire et claudere
 * ================================================== */

/* compositum aperire in positione imperii: nodus, tok_apertura,
 * gradus (locus, modus) */
interior b32
_compositum_aperire (
       Aedificatio* p,
      MateriaToken* t,
               s32  genus,
               i32  locus_aperturae,
               i32  locus,
       CrustaModus  modus)
{
    Gradus* g = _vertex(p);
    MateriaNodus* n;

    /* functio corpus exspectans: compositum EST corpus */
    si (!_sententiam_admittit(g))
    {
        redde _malum(p, t);
    }
    n = _nodus(p, genus);
    si (n == NIHIL || !_token_ponere(n, locus_aperturae, t))
    {
        redde FALSUM;
    }
    redde _pellere(p, n, locus, modus) != NIHIL;
}

/* lexema clausurae compositi: gradus proximus generis dati petitur,
 * supra iacentia absentia clauduntur, lexema ponitur, gradus clausus
 * (status II) redirectiones exspectat */
interior b32
_compositum_claudere (
       Aedificatio* p,
      MateriaToken* t,
               s32  genus,
               i32  locus_clausurae,
               i32  locus_redirectionum)
{
       s32  index = _gradus_quaerere(p, genus);
    Gradus* g;

    si (index < ZEPHYRUM)
    {
        redde _malum(p, t);
    }
    si (!_claudere_usque(p, (i32)index + I))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (!_token_ponere(g->nodus, locus_clausurae, t))
    {
        redde FALSUM;
    }
    g->status  = II;
    g->locus   = locus_redirectionum;
    g->modus   = CRUSTA_MODUS_POST_COMPOSITUM;
    redde VERUM;
}

/* 'then': conditio aut ramus (elif) cum probatione aperta proximus */
interior b32
_deinde (
       Aedificatio* p,
      MateriaToken* t)
{
       s32  i;
       s32  index = (s32)-I;
    Gradus* g;

    per (i = (s32)xar_numerus(p->gradus) - I; i >= ZEPHYRUM; i--)
    {
        Gradus* h = (Gradus*)xar_obtinere(p->gradus, (i32)i);

        si (   (   h->genus == (s32)CRUSTA_GENUS_CONDITIO
                && h->locus == (i32)CRUSTA_CONDITIO_PROBATIO)
            || (   h->genus == (s32)CRUSTA_GENUS_RAMUS
                && h->locus == (i32)CRUSTA_RAMUS_PROBATIO))
        {
            index = i;
            frange;
        }
        si (   h->genus == (s32)CRUSTA_GENUS_PROGRAMMA
            || h->genus == (s32)CRUSTA_GENUS_CONDITIO
            || h->genus == (s32)CRUSTA_GENUS_RAMUS)
        {
            frange;
        }
    }
    si (index < ZEPHYRUM)
    {
        redde _malum(p, t);
    }
    si (!_claudere_usque(p, (i32)index + I))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (g->genus == (s32)CRUSTA_GENUS_CONDITIO)
    {
        si (!_token_ponere(g->nodus, (i32)CRUSTA_CONDITIO_TOK_DEINDE,
            t))
        {
            redde FALSUM;
        }
        g->locus = (i32)CRUSTA_CONDITIO_LIBERI;
    }
    alioquin
    {
        si (!_token_ponere(g->nodus, (i32)CRUSTA_RAMUS_TOK_DEINDE, t))
        {
            redde FALSUM;
        }
        g->locus = (i32)CRUSTA_RAMUS_LIBERI;
    }
    g->modus = CRUSTA_MODUS_INITIUM;
    redde VERUM;
}

/* 'elif' / 'else': ramus prior clauditur, ramus novus in rami
 * conditionis proximae */
interior b32
_ramus (
       Aedificatio* p,
      MateriaToken* t,
               b32  cum_probatione)
{
       s32  index = _gradus_quaerere(p, (s32)CRUSTA_GENUS_CONDITIO);
    Gradus* g;
    MateriaNodus* ramus;

    si (index < ZEPHYRUM)
    {
        redde _malum(p, t);
    }
    g = (Gradus*)xar_obtinere(p->gradus, (i32)index);
    si (g->locus == (i32)CRUSTA_CONDITIO_PROBATIO)
    {
        redde _malum(p, t);   /* 'if a; else' sine 'then' */
    }
    si (!_claudere_usque(p, (i32)index + I))
    {
        redde FALSUM;
    }
    g         = _vertex(p);
    g->locus  = (i32)CRUSTA_CONDITIO_RAMI;
    ramus     = _nodus(p, (s32)CRUSTA_GENUS_RAMUS);
    si (   ramus == NIHIL
        || !_token_ponere(ramus, (i32)CRUSTA_RAMUS_TOK_APERTURA, t))
    {
        redde FALSUM;
    }
    redde _pellere(p, ramus, cum_probatione
            ? (i32)CRUSTA_RAMUS_PROBATIO : (i32)CRUSTA_RAMUS_LIBERI,
        CRUSTA_MODUS_INITIUM) != NIHIL;
}

/* 'do': cursus sub iteratione/cyclo/repetitione proxima sine cursu */
interior b32
_cursus (
       Aedificatio* p,
      MateriaToken* t)
{
       s32  i;
       s32  index = (s32)-I;
    Gradus* g;
    MateriaNodus* cursus;
    i32 locus;

    per (i = (s32)xar_numerus(p->gradus) - I; i >= ZEPHYRUM; i--)
    {
        Gradus* h = (Gradus*)xar_obtinere(p->gradus, (i32)i);

        si (   h->status != II
            && (   (   h->genus == (s32)CRUSTA_GENUS_ITERATIO
                    && h->status != ZEPHYRUM
                    && h->locus != (i32)CRUSTA_ITERATIO_CURSUS)
                || (   h->genus == (s32)CRUSTA_GENUS_CYCLUS
                    && h->machina == NIHIL
                    && h->locus != (i32)CRUSTA_CYCLUS_CURSUS)
                || (   h->genus == (s32)CRUSTA_GENUS_REPETITIO
                    && h->locus == (i32)CRUSTA_REPETITIO_PROBATIO)))
        {
            index = i;
            frange;
        }
        si (h->genus == (s32)CRUSTA_GENUS_PROGRAMMA)
        {
            frange;
        }
    }
    si (index < ZEPHYRUM)
    {
        redde _malum(p, t);
    }
    si (!_claudere_usque(p, (i32)index + I))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    locus = g->genus == (s32)CRUSTA_GENUS_ITERATIO
        ? (i32)CRUSTA_ITERATIO_CURSUS
        : g->genus
            == (s32)CRUSTA_GENUS_CYCLUS ? (i32)CRUSTA_CYCLUS_CURSUS
        : (i32)CRUSTA_REPETITIO_CURSUS;
    g->locus  = locus;
    cursus    = _nodus(p, (s32)CRUSTA_GENUS_CURSUS);
    si (   cursus == NIHIL
        || !_token_ponere(cursus, (i32)CRUSTA_CURSUS_TOK_APERTURA, t))
    {
        redde FALSUM;
    }
    redde _pellere(p, cursus, (i32)CRUSTA_CURSUS_LIBERI,
        CRUSTA_MODUS_INITIUM) != NIHIL;
}

/* 'done': cursus proximus clauditur in cyclum suum, cyclus clausus
 * redirectiones exspectat */
interior b32
_cursum_claudere (
       Aedificatio* p,
      MateriaToken* t)
{
       s32  index = _gradus_quaerere(p, (s32)CRUSTA_GENUS_CURSUS);
    Gradus* g;

    si (index < ZEPHYRUM)
    {
        redde _malum(p, t);
    }
    si (!_claudere_usque(p, (i32)index + I))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (   !_token_ponere(g->nodus, (i32)CRUSTA_CURSUS_TOK_CLAUSURA, t)
        || !_claudere(p))
    {
        redde FALSUM;
    }
    g          = _vertex(p);
    g->status  = II;
    g->modus   = CRUSTA_MODUS_POST_COMPOSITUM;
    g->locus   = g->genus == (s32)CRUSTA_GENUS_ITERATIO
        ? (i32)CRUSTA_ITERATIO_REDIRECTIONES
        : g->genus == (s32)CRUSTA_GENUS_CYCLUS
            ? (i32)CRUSTA_CYCLUS_REDIRECTIONES
        : (i32)CRUSTA_REPETITIO_REDIRECTIONES;
    redde VERUM;
}

/* 'in': iteratio post nomen (verba sequuntur) aut electio post
 * verbum (exemplaria sequuntur) */
interior b32
_in (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g = _vertex(p);

    si (g->genus == (s32)CRUSTA_GENUS_ITERATIO && g->status == I)
    {
        si (!_token_ponere(g->nodus, (i32)CRUSTA_ITERATIO_TOK_IN, t))
        {
            redde FALSUM;
        }
        /* status iterationis: ZEPHYRUM nomen exspectat, I nomen visum,
         * III verba post 'in', IV post separatorem ('do' exspectat),
         * II clausa (post 'done') */
        g->status  = III;
        g->locus   = (i32)CRUSTA_ITERATIO_VERBA;
        g->modus   = CRUSTA_MODUS_IN_VERBIS;
        redde VERUM;
    }
    si (g->genus == (s32)CRUSTA_GENUS_ELECTIO && g->status == I)
    {
        si (!_token_ponere(g->nodus, (i32)CRUSTA_ELECTIO_TOK_IN, t))
        {
            redde FALSUM;
        }
        /* status I manet (< II = apertum); locus phasin dicit */
        g->locus = (i32)CRUSTA_ELECTIO_LIBERI;
        g->modus = CRUSTA_MODUS_EXEMPLAR;
        redde VERUM;
    }
    redde _malum(p, t);
}

/* optio nova sub electione (post 'in' aut terminatorem) */
interior b32
_optio_aperire (
       Aedificatio* p,
      MateriaToken* parenthesis)
{
    Gradus* g = _vertex(p);
    MateriaNodus* optio;

    si (   g->genus != (s32)CRUSTA_GENUS_ELECTIO
        || g->locus != (i32)CRUSTA_ELECTIO_LIBERI)
    {
        redde FALSUM;
    }
    optio = _nodus(p, (s32)CRUSTA_GENUS_OPTIO);
    si (optio == NIHIL)
    {
        redde FALSUM;
    }
    si (   parenthesis != NIHIL
        && !_token_ponere(optio, (i32)CRUSTA_OPTIO_TOK_PARENTHESIS,
            parenthesis))
    {
        redde FALSUM;
    }
    /* optio incohata (P11b, bash 5.2 mensuratum): 'esac' post '(' aut
     * '|' verbum est, linea nova ante ')' errat - IN_VERBIS (nulla
     * reservata, linea nova separator). EXEMPLAR solum ante optionem
     * (gradus electionis) */
    redde _pellere(p, optio, (i32)CRUSTA_OPTIO_EXEMPLARIA,
        CRUSTA_MODUS_IN_VERBIS) != NIHIL;
}

/* ';;' ';&' ';;&': optio proxima clauditur */
interior b32
_optionem_terminare (
       Aedificatio* p,
      MateriaToken* t)
{
       s32  index = _gradus_quaerere(p, (s32)CRUSTA_GENUS_OPTIO);
    Gradus* g;

    si (index < ZEPHYRUM)
    {
        redde _malum(p, t);
    }
    si (!_claudere_usque(p, (i32)index + I))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (!_token_ponere(g->nodus, (i32)CRUSTA_OPTIO_TOK_TERMINATOR, t))
    {
        redde FALSUM;
    }
    redde _claudere(p);
}

/* 'f()' in positione imperii: imperium unius verbi litteralis fit
 * functio (nodus imperii numquam appensus - appensio dilata) */
interior b32
_imperium_in_functionem (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g;
    MateriaValor* liberi;
    MateriaNodus* verbum;
    MateriaNodus* pars;
    MateriaNodus* functio;

    si (!_verbum_claudere(p))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (g->genus != (s32)CRUSTA_GENUS_IMPERIUM || g->status != I)
    {
        redde _malum(p, t);
    }
    liberi = &g->nodus->loci[CRUSTA_IMPERIUM_LIBERI];
    si (   liberi->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*liberi) != (i32)I)
    {
        redde _malum(p, t);
    }
    verbum = materia_valor_lista_obtinere(*liberi,
        ZEPHYRUM)->datum.nodus;
    si (   verbum->genus != (s32)CRUSTA_GENUS_VERBUM
        || materia_valor_lista_numerus(
                verbum->loci[CRUSTA_VERBUM_PARTES]) != (i32)I)
    {
        redde _malum(p, t);
    }
    pars =
        materia_valor_lista_obtinere(verbum->loci[CRUSTA_VERBUM_PARTES],
        ZEPHYRUM)->datum.nodus;
    si (pars->genus != (s32)CRUSTA_GENUS_PARS_LITTERALIS)
    {
        redde _malum(p, t);
    }
    functio = _nodus(p, (s32)CRUSTA_GENUS_FUNCTIO);
    si (   functio == NIHIL
        || !_token_ponere(functio, (i32)CRUSTA_FUNCTIO_TOK_TITULUS,
            pars->loci[CRUSTA_PARS_TOK].datum.token)
        || !_token_ponere(functio, (i32)CRUSTA_FUNCTIO_TOK_APERTURA, t))
    {
        redde FALSUM;
    }
    g->nodus   = functio;
    g->genus   = (s32)CRUSTA_GENUS_FUNCTIO;
    g->locus   = (i32)CRUSTA_FUNCTIO_CORPUS;
    g->status  = II;   /* ')' exspectatur */
    g->modus   = CRUSTA_MODUS_POST_TITULUM;
    redde VERUM;
}

/* socius: candidatus imperium erat - imperium cum verbo litterali.
 * pellere: gradus imperii et verbi pulsi (pars adiacens sequi potest);
 * sine pulsu (socius ipse clauditur) imperium in locum ponitur */
interior b32
_socium_imperare_modo (
    Aedificatio* p,
         Gradus* g,
            b32  pellere)
{
    MateriaToken* t = g->candidatus;
    MateriaNodus* imperium;
    MateriaNodus* verbum;
    MateriaNodus* pars;
    Gradus* novus;

    g->status      = II;
    g->candidatus  = NIHIL;
    si (t == NIHIL)
    {
        redde VERUM;
    }
    imperium  = _nodus(p, (s32)CRUSTA_GENUS_IMPERIUM);
    verbum    = _nodus(p, (s32)CRUSTA_GENUS_VERBUM);
    pars      = _nodus(p, (s32)CRUSTA_GENUS_PARS_LITTERALIS);
    si (   imperium == NIHIL || verbum == NIHIL || pars == NIHIL
        || !_token_ponere(pars, (i32)CRUSTA_PARS_TOK, t)
        || !_nodum_dare(p, verbum, (i32)CRUSTA_VERBUM_PARTES, pars))
    {
        redde FALSUM;
    }
    si (!pellere)
    {
        redde _nodum_dare(p, imperium, (i32)CRUSTA_IMPERIUM_LIBERI,
                verbum)
            && _nodum_dare(p, g->nodus, (i32)CRUSTA_SOCIUS_IMPERIUM,
                imperium);
    }
    novus = _pellere(p, imperium, (i32)CRUSTA_IMPERIUM_LIBERI,
        CRUSTA_MODUS_VERBA);
    si (novus == NIHIL)
    {
        redde FALSUM;
    }
    novus->status = I;
    novus = _pellere(p, verbum, (i32)CRUSTA_VERBUM_PARTES,
        CRUSTA_MODUS_VERBA);
    si (novus == NIHIL)
    {
        redde FALSUM;
    }
    novus->verbi_finis = _token_finis(t);
    redde VERUM;
}

interior b32
_socium_imperare (
    Aedificatio* p,
         Gradus* g)
{
    redde _socium_imperare_modo(p, g, FALSUM);
}

/* socius cum candidato: lexema sequens decernit - apertura compositi
 * = candidatus titulus erat; aliud = candidatus imperium */
interior b32
_socium_decernere (
       Aedificatio* p,
            Gradus* g,
      MateriaToken* t)
{
    b32 apertura = t->genus == (s32)CRUSTA_LEX_PARENTHESIS
        || t->genus == (s32)CRUSTA_LEX_ARITHMETICA_APERTURA;

    si (t->genus == (s32)CRUSTA_LEX_RESERVATUM)
    {
        constans character* claves[] = { "if", "for", "select",
            "while", "until", "case", "{", "[[", "function", NIHIL };
        i32 i;

        per (i = ZEPHYRUM; claves[i] != NIHIL; i++)
        {
            si (   (i32)strlen(claves[i]) == t->valor.mensura
                && memcmp(claves[i], t->valor.datum,
                    (size_t)t->valor.mensura) == ZEPHYRUM)
            {
                apertura = VERUM;
            }
        }
    }
    si (apertura)
    {
        si (!_token_ponere(g->nodus, (i32)CRUSTA_SOCIUS_TOK_TITULUS,
                g->candidatus))
        {
            redde FALSUM;
        }
        g->status      = II;
        g->candidatus  = NIHIL;
        g->modus       = CRUSTA_MODUS_INITIUM;
        redde VERUM;
    }
    redde _socium_imperare_modo(p, g, VERUM);
}


/* ==================================================
 * Iudicium [[ ]]
 * ================================================== */

interior Iudicatio*
_iudicatio_creare (
    Aedificatio* p)
{
    Iudicatio* iud = (Iudicatio*)piscina_allocare(p->piscina,
        (memoriae_index)magnitudo(Iudicatio));

    si (iud == NIHIL)
    {
        redde NIHIL;
    }
    memset(iud, ZEPHYRUM, magnitudo(*iud));
    iud->operanda = xar_creare(p->piscina,
        (i32)magnitudo(OperandumIudicii));
    iud->signa = xar_creare(p->piscina,
        (i32)magnitudo(IudiciiSignum));
    iud->operandum_exspectatur = VERUM;
    si (iud->operanda == NIHIL || iud->signa == NIHIL)
    {
        redde NIHIL;
    }
    redde iud;
}

interior b32
_iudicatio_operandum_pellere (
       Iudicatio* iud,
    MateriaNodus* n,
             Xar* corpora)
{
    OperandumIudicii* sedes =
        (OperandumIudicii*)xar_addere(iud->operanda);

    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    sedes->nodus    = n;
    sedes->corpora  = corpora;
    redde VERUM;
}

interior OperandumIudicii
_iudicatio_operandum_tollere (
    Iudicatio* iud)
{
                 i32 n = xar_numerus(iud->operanda);
    OperandumIudicii res;

    res.nodus    = NIHIL;
    res.corpora  = NIHIL;
    si (n == ZEPHYRUM)
    {
        redde res;
    }
    res = *(OperandumIudicii*)xar_obtinere(iud->operanda, n - I);
    xar_removere_ultimum(iud->operanda);
    redde res;
}

interior b32
_nodum_ponere_si (
    MateriaNodus* parens,
             i32  locus,
    MateriaNodus* filius)
{
    si (filius == NIHIL)
    {
        redde VERUM;   /* absens */
    }
    redde materia_nodus_ponere(parens, locus,
        materia_valor_nodus(filius),
        MATERIA_LOCUS_NODUS);
}

/* corpus in corpora (Xar creata si NIHIL) addere (P9b) */
interior b32
_corpus_addere (
     Aedificatio*  p,
             Xar** corpora,
    MateriaNodus*  corpus)
{
    MateriaNodus** sedes;

    si (*corpora == NIHIL)
    {
        *corpora = xar_creare(p->piscina,
            (i32)magnitudo(MateriaNodus*));
        si (*corpora == NIHIL)
        {
            redde FALSUM;
        }
    }
    sedes = (MateriaNodus**)xar_addere(*corpora);
    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    *sedes = corpus;
    redde VERUM;
}

/* signum summum reducere: praeposita (operandum unum), binaria aut
 * coniuncta (duo); operanda absentia NIHIL. CORPORA (P9b): post
 * operandum sinistrum et post signum in listas post_* nodi novi; post
 * operandum ultimum cum nodo novo ascendunt. */
interior b32
_iudicatio_reducere (
    Aedificatio* p,
      Iudicatio* iud)
{
                 i32 n = xar_numerus(iud->signa);
       IudiciiSignum s;
       MateriaNodus* res;
    OperandumIudicii dexter;
    OperandumIudicii sinister;

    si (n == ZEPHYRUM)
    {
        redde VERUM;
    }
    s = *(IudiciiSignum*)xar_obtinere(iud->signa, n - I);
    xar_removere_ultimum(iud->signa);
    si (s.praeposita)
    {
        dexter  = _iudicatio_operandum_tollere(iud);
        res     = _nodus(p, (s32)CRUSTA_GENUS_IUDICIUM_PRAEPOSITA);
        si (   res == NIHIL
            || !_token_ponere(res, (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR,
                s.tok)
            || !_corpora_dare(p, res,
            (i32)CRUSTA_PRAEPOSITA_POST_SIGNUM,
                s.corpora)
            || !_nodum_ponere_si(res, (i32)CRUSTA_PRAEPOSITA_OPERANDUM,
                dexter.nodus))
        {
            redde FALSUM;
        }
        redde _iudicatio_operandum_pellere(iud, res, dexter.corpora);
    }
    dexter    = _iudicatio_operandum_tollere(iud);
    sinister  = _iudicatio_operandum_tollere(iud);
    res = _nodus(p, s.praecedentia < (i32)IV
        ? (s32)CRUSTA_GENUS_IUDICIUM_CONIUNCTA
        : (s32)CRUSTA_GENUS_IUDICIUM_BINARIA);
    si (   res == NIHIL
        || !_nodum_ponere_si(res, (i32)CRUSTA_BINARIA_SINISTER,
            sinister.nodus)
        || !_corpora_dare(p, res, (i32)CRUSTA_BINARIA_POST_SINISTRUM,
            sinister.corpora)
        || (   s.tok != NIHIL
            && !_token_ponere(res, (i32)CRUSTA_BINARIA_TOK_OPERATOR,
                s.tok))
        || !_corpora_dare(p, res, (i32)CRUSTA_BINARIA_POST_SIGNUM,
            s.corpora)
        || !_nodum_ponere_si(res, (i32)CRUSTA_BINARIA_DEXTER,
            dexter.nodus))
    {
        redde FALSUM;
    }
    redde _iudicatio_operandum_pellere(iud, res, dexter.corpora);
}

/* operandum (verbum aut inclusa): pellitur; probatio unaria aut
 * binaria in vertice statim reducitur (verbum unum capiunt) */
interior b32
_iudicatio_operandum (
     Aedificatio* p,
       Iudicatio* iud,
    MateriaNodus* n)
{
    IudiciiSignum* s;
              i32  numerus;

    /* IUXTAPOSITIO ('a b'): signum binarium IMPLICITUM sine lexemate
     * (gradus IV) in adventu ponitur, ut binaria statim formetur et
     * signa inferiora ('!', '&&') eam totam capiant. INVENTUM P9
     * (porta totalitatis): operanda superflua olim in fine iungebantur,
     * post signa reducta - '[[ ! a b' '!' ad 'b' ligabat, emissio
     * '[[ a ! b'. */
    si (!iud->operandum_exspectatur)
    {
        IudiciiSignum* implicitum;

        iud->mala++;
        dum (xar_numerus(iud->signa) > ZEPHYRUM)
        {
            IudiciiSignum* summum = (IudiciiSignum*)xar_obtinere(
                iud->signa, xar_numerus(iud->signa) - I);

            si (summum->praecedentia < (i32)IV)
            {
                frange;
            }
            si (!_iudicatio_reducere(p, iud))
            {
                redde FALSUM;
            }
        }
        implicitum = (IudiciiSignum*)xar_addere(iud->signa);
        si (implicitum == NIHIL)
        {
            redde FALSUM;
        }
        implicitum->tok           = NIHIL;
        implicitum->praeposita    = FALSUM;
        implicitum->praecedentia  = (i32)IV;
        implicitum->corpora       = NIHIL;
    }
    si (!_iudicatio_operandum_pellere(iud, n, NIHIL))
    {
        redde FALSUM;
    }
    iud->operandum_exspectatur  = FALSUM;
    iud->ultimum                = I;
    numerus                     = xar_numerus(iud->signa);
    si (numerus > ZEPHYRUM)
    {
        s = (IudiciiSignum*)xar_obtinere(iud->signa, numerus - I);
        si (s->praecedentia == (i32)IV)
        {
            redde _iudicatio_reducere(p, iud);
        }
    }
    redde VERUM;
}

/* CORPUS INTERPOSITUM (P9b): post lexema ultimum acceptum - operandum
 * summum, signum summum, aut initium */
interior b32
_iudicatio_interponere (
     Aedificatio* p,
       Iudicatio* iud,
    MateriaNodus* corpus)
{
    i32 n;

    si (iud->ultimum == I)
    {
        n = xar_numerus(iud->operanda);
        si (n > ZEPHYRUM)
        {
            OperandumIudicii* o = (OperandumIudicii*)xar_obtinere(
                iud->operanda, n - I);

            redde _corpus_addere(p, &o->corpora, corpus);
        }
    }
    alioquin si (iud->ultimum == II)
    {
        n = xar_numerus(iud->signa);
        si (n > ZEPHYRUM)
        {
            IudiciiSignum* s = (IudiciiSignum*)xar_obtinere(iud->signa,
                n - I);

            redde _corpus_addere(p, &s->corpora, corpus);
        }
    }
    redde _corpus_addere(p, &iud->corpora_initii, corpus);
}

interior b32
_iudicatio_signum (
     Aedificatio* p,
       Iudicatio* iud,
    MateriaToken* t)
{
    IudiciiSignum  s;
    IudiciiSignum* sedes;
              b32  in_tabula = FALSUM;
              i32  i;

    s.tok           = t;
    s.praeposita    = FALSUM;
    s.praecedentia  = (i32)IV;
    s.corpora       = NIHIL;
    si (t->valor.mensura == (i32)I && t->valor.datum[ZEPHYRUM] == '!')
    {
        s.praeposita    = VERUM;
        s.praecedentia  = (i32)III;
    }
    alioquin si (   t->valor.mensura         == (i32)II
                 && t->valor.datum[ZEPHYRUM] == '&')
    {
        s.praecedentia = (i32)II;
    }
    alioquin si (   t->valor.mensura         == (i32)II
                 && t->valor.datum[ZEPHYRUM] == '|')
    {
        s.praecedentia = I;
    }
    alioquin
    {
        per (i = ZEPHYRUM; p->dialectus->iudicii_praeposita[i] != NIHIL;
             i++)
        {
            constans character* v = p->dialectus->iudicii_praeposita[i];

            si (   (i32)strlen(v) == t->valor.mensura
                && memcmp(v, t->valor.datum, (size_t)t->valor.mensura)
                    == ZEPHYRUM)
            {
                s.praeposita  = VERUM;
                in_tabula     = VERUM;
            }
        }
        si (!in_tabula)
        {
            /* binaria (tabula dialecti aut '<' '>' nudi) */
            s.praecedentia = (i32)IV;
        }
    }
    si (s.praeposita)
    {
        /* praepositum post operandum ('a -f b', '() ! x'):
         * iuxtapositio - signum binarium implicitum prius (ut in
         * _iudicatio_operandum), ne signum praepositum super signa
         * inferiora pulsum operandum prius transiliat. INVENTUM P9b
         * (porta totalitatis): '[[ !()-a' emittebat '[[() !-a'. */
        si (!iud->operandum_exspectatur)
        {
            IudiciiSignum* implicitum;

            iud->mala++;
            dum (xar_numerus(iud->signa) > ZEPHYRUM)
            {
                IudiciiSignum* summum = (IudiciiSignum*)xar_obtinere(
                    iud->signa, xar_numerus(iud->signa) - I);

                si (summum->praecedentia < (i32)IV)
                {
                    frange;
                }
                si (!_iudicatio_reducere(p, iud))
                {
                    redde FALSUM;
                }
            }
            implicitum = (IudiciiSignum*)xar_addere(iud->signa);
            si (implicitum == NIHIL)
            {
                redde FALSUM;
            }
            implicitum->tok           = NIHIL;
            implicitum->praeposita    = FALSUM;
            implicitum->praecedentia  = (i32)IV;
            implicitum->corpora       = NIHIL;
        }
        iud->operandum_exspectatur = VERUM;
    }
    alioquin
    {
        si (iud->operandum_exspectatur)
        {
            /* operandum sinistrum absens */
            si (!_iudicatio_operandum_pellere(iud, NIHIL, NIHIL))
            {
                redde FALSUM;
            }
        }
        /* reducere dum signum summum aeque aut artius ligat */
        dum (xar_numerus(iud->signa) > ZEPHYRUM)
        {
            IudiciiSignum* summum = (IudiciiSignum*)xar_obtinere(
                iud->signa, xar_numerus(iud->signa) - I);

            si (summum->praecedentia < s.praecedentia)
            {
                frange;
            }
            si (!_iudicatio_reducere(p, iud))
            {
                redde FALSUM;
            }
        }
        iud->operandum_exspectatur = VERUM;
    }
    sedes = (IudiciiSignum*)xar_addere(iud->signa);
    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    *sedes        = s;
    iud->ultimum  = II;
    redde VERUM;
}

/* expressio finita in locum gradus; operanda superflua (sine
 * operatore) in binarias cum operatore absenti ordine coniunguntur.
 * Corpora initii in post_aperturam, post expressionem in
 * post_expressionem (P9b; iudicium et iudicium-inclusa: loci I, II,
 * III). */
interior b32
_iudicationem_finire (
    Aedificatio* p,
         Gradus* g)
{
           Iudicatio* iud = g->iudicatio;
    OperandumIudicii  e;
                 i32  locus = g->genus == (s32)CRUSTA_GENUS_IUDICIUM
                     ? (i32)CRUSTA_IUDICIUM_EXPRESSIO
                     : (i32)CRUSTA_INCLUSA_EXPRESSIO;

    g->iudicatio = NIHIL;
    si (   iud->operandum_exspectatur
        && xar_numerus(iud->signa) > ZEPHYRUM)
    {
        si (!_iudicatio_operandum_pellere(iud, NIHIL, NIHIL))
        {
            redde FALSUM;
        }
    }
    dum (xar_numerus(iud->signa) > ZEPHYRUM)
    {
        si (!_iudicatio_reducere(p, iud))
        {
            redde FALSUM;
        }
    }
    dum (xar_numerus(iud->operanda) > (i32)I)
    {
        OperandumIudicii dexter;
        OperandumIudicii sinister;
           MateriaNodus* b;

        dexter    = _iudicatio_operandum_tollere(iud);
        sinister  = _iudicatio_operandum_tollere(iud);
        b         = _nodus(p, (s32)CRUSTA_GENUS_IUDICIUM_BINARIA);

        si (   b == NIHIL
            || !_nodum_ponere_si(b, (i32)CRUSTA_BINARIA_SINISTER,
                sinister.nodus)
            || !_corpora_dare(p, b, (i32)CRUSTA_BINARIA_POST_SINISTRUM,
                sinister.corpora)
            || !_nodum_ponere_si(b, (i32)CRUSTA_BINARIA_DEXTER,
                dexter.nodus)
            || !_iudicatio_operandum_pellere(iud, b, dexter.corpora))
        {
            redde FALSUM;
        }
    }
    si (p->relatio != NIHIL)
    {
        p->relatio->mala += iud->mala;
    }
    e = _iudicatio_operandum_tollere(iud);
    redde _corpora_dare(p, g->nodus, locus - I, iud->corpora_initii)
        && _nodum_ponere_si(g->nodus, locus, e.nodus)
        && _corpora_dare(p, g->nodus, locus + I, e.corpora);
}

/* lexemata in gradu iudicii (verbum in vertice iam clausum) */
interior b32
_iudicium_tractare (
       Aedificatio* p,
            Gradus* g,
      MateriaToken* t,
               b32* tractatum)
{
    *tractatum = VERUM;
    commutatio (t->genus)
    {
        casus CRUSTA_LEX_IUDICIUM_OPERATOR:
            si (!_iudicatio_signum(p, g->iudicatio, t))
            {
                redde FALSUM;
            }
            si (   t->valor.mensura         == (i32)II
                && t->valor.datum[ZEPHYRUM] == '='
                && t->valor.datum[I]        == '~')
            {
                g->modus = CRUSTA_MODUS_REGULA;
            }
            redde VERUM;
        casus CRUSTA_LEX_PARENTHESIS:
        {
            MateriaNodus* n = _nodus(p,
                (s32)CRUSTA_GENUS_IUDICIUM_INCLUSA);
            Iudicatio* iud = _iudicatio_creare(p);
               Gradus* novus;

            si (   n == NIHIL || iud == NIHIL
                || !_token_ponere(n, (i32)CRUSTA_INCLUSA_TOK_APERTURA,
                t))
            {
                redde FALSUM;
            }
            novus = _pellere(p, n, (i32)CRUSTA_INCLUSA_EXPRESSIO,
                CRUSTA_MODUS_IUDICIUM);
            si (novus == NIHIL)
            {
                redde FALSUM;
            }
            novus->iudicatio = iud;
            redde VERUM;
        }
        casus CRUSTA_LEX_PARENTHESIS_CLAUSURA:
            si (g->genus == (s32)CRUSTA_GENUS_IUDICIUM_INCLUSA)
            {
                si (!_token_ponere(g->nodus,
                        (i32)CRUSTA_INCLUSA_TOK_CLAUSURA, t))
                {
                    redde FALSUM;
                }
                redde _claudere(p);
            }
            redde _malum(p, t);
        casus CRUSTA_LEX_IUDICIUM_CLAUSURA:
        {
            s32 index = _gradus_quaerere(p, (s32)CRUSTA_GENUS_IUDICIUM);

            si (index < ZEPHYRUM)
            {
                redde _malum(p, t);
            }
            si (!_claudere_usque(p, (i32)index + I))
            {
                redde FALSUM;
            }
            g = _vertex(p);
            si (   !_iudicationem_finire(p, g)
                || !_token_ponere(g->nodus,
                    (i32)CRUSTA_IUDICIUM_TOK_CLAUSURA, t))
            {
                redde FALSUM;
            }
            g->status  = II;
            g->locus   = (i32)CRUSTA_IUDICIUM_REDIRECTIONES;
            g->modus   = CRUSTA_MODUS_POST_COMPOSITUM;
            redde VERUM;
        }
        ordinarius:
            *tractatum = FALSUM;
            redde VERUM;
    }
}


/* ==================================================
 * Heredoca (decretum 01M2NJ16RG)
 * ================================================== */

/* CORPUS COLLOCARE ubi octeti iacent (decretum 01M2NJ16RG; P9b):
 * gradus a vertice quaeritur -
 *  - lista sententiarum (programma, substitutio, compositum apertum,
 *    catena, pipa) aut lacuna compositi cum lista (iteratio:
 *    interiecta post nomen, separator post separatorem; cyclus
 *    separator; electio: interiecta post verbum; optio: exemplaria;
 *    tabulatum: liberi; expansio: argumenta): corpus appenditur;
 *  - machina (arithmetica, pars-arithmetica, inclusa, cyclus intra
 *    '((') aut iudicatio ([[ ]], iudicium-inclusa): corpus post
 *    lexema ultimum interponitur, in listas post_* reductione ponitur;
 *  - functio post titulum aut ')' (linea nova valida): interiecta,
 *    corpus deinde exspectatur;
 *  - lacunae ubi bash errat ('for'/'select'/'case'/'function' ante
 *    titulum, 'f(' ante ')'): gradus absens clauditur, quaestio a
 *    vertice novo iterum.
 * Gradus alius transitus = limes (heredoca_transposita) - nullus
 * notus post P9b. */
interior b32
_corpus_collocare (
     Aedificatio* p,
    MateriaNodus* corpus)
{
    s32 i;
    b32 transitum = FALSUM;

    per (i = (s32)xar_numerus(p->gradus) - I; i >= ZEPHYRUM; i--)
    {
        Gradus* h = (Gradus*)xar_obtinere(p->gradus, (i32)i);
           s32  l = (s32)-I;

        si (h->machina != NIHIL)
        {
            redde crusta_arithmetica_interponere(h->machina, corpus);
        }
        si (h->iudicatio != NIHIL)
        {
            redde _iudicatio_interponere(p, h->iudicatio, corpus);
        }
        si (   (h->genus == (s32)CRUSTA_GENUS_ITERATIO
                && h->status == ZEPHYRUM)
            || (h->genus == (s32)CRUSTA_GENUS_ELECTIO
                && h->status == ZEPHYRUM)
            || (h->genus == (s32)CRUSTA_GENUS_FUNCTIO
                && (h->status == ZEPHYRUM || h->status == II)))
        {
            si (!_claudere_usque(p, (i32)i))
            {
                redde FALSUM;
            }
            i          = (s32)xar_numerus(p->gradus);
            transitum  = FALSUM;
            perge;
        }
        si (   h->genus == (s32)CRUSTA_GENUS_FUNCTIO
            && (h->status == I || h->status == III))
        {
            h->status  = III;
            h->modus   = CRUSTA_MODUS_INITIUM;
            l          = (s32)CRUSTA_FUNCTIO_INTERIECTA;
        }
        alioquin si (   _lista_est(h)
                     || h->genus == (s32)CRUSTA_GENUS_CATENA
                     || h->genus == (s32)CRUSTA_GENUS_PIPA)
        {
            l = (s32)h->locus;
        }
        alioquin commutatio (h->genus)
                 {
            casus CRUSTA_GENUS_ITERATIO:
                si (h->status == I)
                {
                    l = (s32)CRUSTA_ITERATIO_INTERIECTA;
                }
                alioquin si (h->status == IV)
                {
                    l = (s32)CRUSTA_ITERATIO_SEPARATOR;
                }
                frange;
            casus CRUSTA_GENUS_CYCLUS:
                si (h->status == I || h->status == III)
                {
                    l = (s32)CRUSTA_CYCLUS_SEPARATOR;
                }
                frange;
            casus CRUSTA_GENUS_ELECTIO:
                si (   h->locus  == (i32)CRUSTA_ELECTIO_VERBUM
                    && h->status == I)
                {
                    l = (s32)CRUSTA_ELECTIO_INTERIECTA;
                }
                frange;
            casus CRUSTA_GENUS_OPTIO:
                l = (s32)CRUSTA_OPTIO_EXEMPLARIA;
                frange;
            casus CRUSTA_GENUS_TABULATUM:
                l = (s32)CRUSTA_TABULATUM_LIBERI;
                frange;
            casus CRUSTA_GENUS_PARS_EXPANSIO:
                l = (s32)CRUSTA_EXPANSIO_ARGUMENTA;
                frange;
            ordinarius:
                frange;
                 }
        si (l >= ZEPHYRUM)
        {
            si (transitum && p->relatio != NIHIL)
            {
                p->relatio->heredoca_transposita++;
            }
            redde _nodum_dare(p, h->nodus, (i32)l, corpus);
        }
        transitum = VERUM;
    }
    redde _nodum_dare(p,
        ((Gradus*)xar_obtinere(p->gradus, ZEPHYRUM))->nodus,
        (i32)CRUSTA_PROGRAMMA_LIBERI, corpus);
}

/* petitionem proximam aperire: nodus heredoc listae intimae statim
 * appensus, referentia in redirectionem, gradus pulsus (modus per
 * citationem delimitatoris). VERUM etiam si nulla pendet. */
interior b32
_heredoc_aperire_proximum (
    Aedificatio* p)
{
        CrustaHeredocPetitio petitio;
    MateriaNodus* h;
    MateriaNodus* redirectio = NIHIL;

    si (!crusta_lector_heredoca_pendent(&p->lector))
    {
        redde VERUM;
    }
    si (!crusta_lector_heredoc_aperire(&p->lector, &petitio))
    {
        redde FALSUM;
    }
    h = _nodus(p, (s32)CRUSTA_GENUS_HEREDOC);
    si (h == NIHIL)
    {
        redde FALSUM;
    }
    si (!_corpus_collocare(p, h))
    {
        redde FALSUM;
    }
    si (p->redirectio_caput < xar_numerus(p->redirectiones_pendentes))
    {
        redirectio = *(MateriaNodus**)xar_obtinere(
            p->redirectiones_pendentes, p->redirectio_caput);
        p->redirectio_caput++;
    }
    si (   redirectio != NIHIL
        && !materia_nodus_ponere(redirectio,
        (i32)CRUSTA_REDIRECTIO_CORPUS,
            materia_valor_referentia(h), MATERIA_LOCUS_REFERENTIA))
    {
        redde FALSUM;
    }
    redde _pellere(p, h, (i32)CRUSTA_HEREDOC_PARTES, petitio.citatus
            ? CRUSTA_MODUS_HEREDOC_LITTERALE : CRUSTA_MODUS_HEREDOC)
        != NIHIL;
}

/* FINIS regionis heredoc: delimitator et finis in nodum, gradus
 * sublatus (nodus iam in lista), petitio proxima aperitur */
interior b32
_heredoc_claudere (
    Aedificatio* p)
{
    Gradus* g = _vertex(p);
    MateriaToken* delimitator;
    MateriaToken* finis;

    delimitator = crusta_lector_heredoc_delimitator(&p->lector);
        si (delimitator != NIHIL)
        {
        si (   !_lexema_recordare(p, delimitator)
            || !_solvere(p, delimitator)
            || !_token_ponere(g->nodus,
            (i32)CRUSTA_HEREDOC_TOK_DELIMITATOR,
                delimitator))
        {
            redde FALSUM;
        }
        finis = crusta_lector_heredoc_finis(&p->lector);
                si (   finis != NIHIL
                    && (   !_lexema_recordare(p, finis)
                    || !_solvere(p, finis)
                    || !_token_ponere(g->nodus,
                    (i32)CRUSTA_HEREDOC_TOK_FINIS,
                    finis)))
                {
            redde FALSUM;
                }
        }
    alioquin si (p->relatio != NIHIL)
        {
        p->relatio->clausurae_absentes++;
        }
    xar_removere_ultimum(p->gradus);
    redde _heredoc_aperire_proximum(p);
}

/* trivia pendentia omnia 'post' prioris (linea nova trivium ante
 * corpus heredoc: regula C7 eadem, solutio praematura) */
interior b32
_solvere_retro (
    Aedificatio* p)
{
    i32 numerus = xar_numerus(p->pendentia);
    MateriaToken** plana;
    i32 j;

    si (numerus == ZEPHYRUM || p->prior == NIHIL)
    {
        redde VERUM;
    }
    plana = (MateriaToken**)piscina_allocare_ordinatum(p->piscina,
        (memoriae_index)magnitudo(MateriaToken*)
            * (memoriae_index)numerus,
        (memoriae_index)magnitudo(MateriaToken*));
    si (plana == NIHIL)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        plana[j] = *(MateriaToken**)xar_obtinere(p->pendentia, j);
    }
    si (!materia_token_trivia_post_ponere(p->prior, p->piscina, plana,
            numerus))
    {
        redde FALSUM;
    }
    xar_vacare(p->pendentia);
    redde VERUM;
}


/* ==================================================
 * Dispensatio
 * ================================================== */

interior b32
_reservatum_est (
    constans MateriaToken* t,
       constans character* verbum)
{
    redde t->valor.mensura == (i32)strlen(verbum)
        && memcmp(t->valor.datum, verbum, strlen(verbum)) == ZEPHYRUM;
}

/* verbum reservatum in positione imperii (aut nomen post
 * for/select: 'for if in' licet) */
interior b32
_reservatum (
       Aedificatio* p,
            Gradus* g,
      MateriaToken* t)
{
    /* nomen iterationis: verbum reservatum licet (bash) */
    si (   g->genus  == (s32)CRUSTA_GENUS_ITERATIO
        && g->status == ZEPHYRUM
        && !_reservatum_est(t, "in") && !_reservatum_est(t, "do"))
    {
        redde _titulum_ponere(g, t, (i32)CRUSTA_ITERATIO_TOK_TITULUS);
    }

    si (_reservatum_est(t, "!") || _reservatum_est(t, "time"))
    {
        si (!_intra_sententiam_claudere(p))
        {
            redde FALSUM;
        }
        si (!_sententiam_admittit(_vertex(p)))
        {
            redde _malum(p, t);
        }
        redde _praefixum_pipae(p, t);
    }
    si (_reservatum_est(t, "if"))
    {
        redde _compositum_aperire(p, t, (s32)CRUSTA_GENUS_CONDITIO,
            (i32)CRUSTA_CONDITIO_TOK_APERTURA,
            (i32)CRUSTA_CONDITIO_PROBATIO, CRUSTA_MODUS_INITIUM);
    }
    si (_reservatum_est(t, "then"))
    {
        redde _deinde(p, t);
    }
    si (_reservatum_est(t, "elif"))
    {
        redde _ramus(p, t, VERUM);
    }
    si (_reservatum_est(t, "else"))
    {
        redde _ramus(p, t, FALSUM);
    }
    si (_reservatum_est(t, "fi"))
    {
        redde _compositum_claudere(p, t, (s32)CRUSTA_GENUS_CONDITIO,
            (i32)CRUSTA_CONDITIO_TOK_CLAUSURA,
            (i32)CRUSTA_CONDITIO_REDIRECTIONES);
    }
    si (_reservatum_est(t, "for") || _reservatum_est(t, "select"))
    {
        redde _compositum_aperire(p, t, (s32)CRUSTA_GENUS_ITERATIO,
            (i32)CRUSTA_ITERATIO_TOK_APERTURA,
            (i32)CRUSTA_ITERATIO_TOK_TITULUS,
            CRUSTA_MODUS_POST_TITULUM);
    }
    si (_reservatum_est(t, "in"))
    {
        redde _in(p, t);
    }
    si (_reservatum_est(t, "do"))
    {
        redde _cursus(p, t);
    }
    si (_reservatum_est(t, "done"))
    {
        redde _cursum_claudere(p, t);
    }
    si (_reservatum_est(t, "while") || _reservatum_est(t, "until"))
    {
        redde _compositum_aperire(p, t, (s32)CRUSTA_GENUS_REPETITIO,
            (i32)CRUSTA_REPETITIO_TOK_APERTURA,
            (i32)CRUSTA_REPETITIO_PROBATIO, CRUSTA_MODUS_INITIUM);
    }
    si (_reservatum_est(t, "case"))
    {
        redde _compositum_aperire(p, t, (s32)CRUSTA_GENUS_ELECTIO,
            (i32)CRUSTA_ELECTIO_TOK_APERTURA,
            (i32)CRUSTA_ELECTIO_VERBUM,
            CRUSTA_MODUS_POST_TITULUM);
    }
    si (_reservatum_est(t, "esac"))
    {
        redde _compositum_claudere(p, t, (s32)CRUSTA_GENUS_ELECTIO,
            (i32)CRUSTA_ELECTIO_TOK_CLAUSURA,
            (i32)CRUSTA_ELECTIO_REDIRECTIONES);
    }
    si (_reservatum_est(t, "{"))
    {
        redde _compositum_aperire(p, t, (s32)CRUSTA_GENUS_GREX,
            (i32)CRUSTA_GREX_TOK_APERTURA, (i32)CRUSTA_GREX_LIBERI,
            CRUSTA_MODUS_INITIUM);
    }
    si (_reservatum_est(t, "}"))
    {
        redde _compositum_claudere(p, t, (s32)CRUSTA_GENUS_GREX,
            (i32)CRUSTA_GREX_TOK_CLAUSURA,
            (i32)CRUSTA_GREX_REDIRECTIONES);
    }
    si (_reservatum_est(t, "function"))
    {
        redde _compositum_aperire(p, t, (s32)CRUSTA_GENUS_FUNCTIO,
            (i32)CRUSTA_FUNCTIO_TOK_VERBUM, (i32)CRUSTA_FUNCTIO_CORPUS,
            CRUSTA_MODUS_POST_TITULUM);
    }
    si (_reservatum_est(t, "coproc"))
    {
        /* POST_COMPOSITUM: reservata (apertura compositi post
         * titulum), linea nova terminat ('coproc a' + linea nova) */
        redde _compositum_aperire(p, t, (s32)CRUSTA_GENUS_SOCIUS,
            (i32)CRUSTA_SOCIUS_TOK_VERBUM, (i32)CRUSTA_SOCIUS_IMPERIUM,
            CRUSTA_MODUS_POST_COMPOSITUM);
    }
    si (_reservatum_est(t, "[["))
    {
        Iudicatio* iud = _iudicatio_creare(p);

        si (iud == NIHIL)
        {
            redde FALSUM;
        }
        si (!_compositum_aperire(p, t, (s32)CRUSTA_GENUS_IUDICIUM,
                (i32)CRUSTA_IUDICIUM_TOK_APERTURA,
                (i32)CRUSTA_IUDICIUM_EXPRESSIO, CRUSTA_MODUS_IUDICIUM))
        {
            redde FALSUM;
        }
        g = _vertex(p);
        si (g->genus == (s32)CRUSTA_GENUS_IUDICIUM)
        {
            g->iudicatio = iud;
        }
        redde VERUM;
    }
    redde _malum(p, t);   /* ']]' 'in' alienum... */
}

interior b32
_tractare (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g = _vertex(p);

    /* socius cum candidato: lexema hoc decernit */
    si (g->genus == (s32)CRUSTA_GENUS_SOCIUS && g->status == I)
    {
        si (!_socium_decernere(p, g, t))
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    /* iudicium: verbum in vertice ante lexema iudicii clauditur */
    si (   g->genus                == (s32)CRUSTA_GENUS_VERBUM
        && _infra(p, I)->iudicatio != NIHIL
        && (   t->genus == (s32)CRUSTA_LEX_IUDICIUM_OPERATOR
            || t->genus == (s32)CRUSTA_LEX_IUDICIUM_CLAUSURA
            || t->genus == (s32)CRUSTA_LEX_PARENTHESIS
            || t->genus == (s32)CRUSTA_LEX_PARENTHESIS_CLAUSURA))
    {
        si (!_verbum_claudere(p))
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    si (g->iudicatio != NIHIL)
    {
        b32 tractatum;

        si (!_iudicium_tractare(p, g, t, &tractatum))
        {
            redde FALSUM;
        }
        si (tractatum)
        {
            redde VERUM;
        }
    }

    /* partes compositae in vertice quae lexema suum exspectant */
    si (g->genus == (s32)CRUSTA_GENUS_PARS_PARAMETRUM)
    {
        si (t->genus == (s32)CRUSTA_LEX_PARAMETRUM_TITULUS)
        {
            si (!_token_ponere(g->nodus,
                (i32)CRUSTA_PARAMETRUM_TOK_TITULUS,
                    t))
            {
                redde FALSUM;
            }
            redde _partem_claudere(p, t);
        }
        si (!_claudere(p))   /* titulus absens */
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    si (g->genus == (s32)CRUSTA_GENUS_PARS_EXPANSIO)
    {
        /* argumenta: verbum in vertice tractatur infra; expansio ipsa
         * lexemata sua sumit */
        redde _expansio(p, g, t);
    }
    si (   g->genus            == (s32)CRUSTA_GENUS_VERBUM
        && _infra(p, I)->genus == (s32)CRUSTA_GENUS_PARS_EXPANSIO
        && (t->genus == (s32)CRUSTA_LEX_EXPANSIO_CLAUSURA
            || t->genus == (s32)CRUSTA_LEX_EXPANSIO_OPERATOR))
    {
        si (!_claudere(p))
        {
            redde FALSUM;
        }
        redde _expansio(p, _vertex(p), t);
    }
    si (   _partes_directe(g->genus)
        && t->genus == (s32)CRUSTA_LEX_GEMINA_CLAUSURA)
    {
        si (!_token_ponere(g->nodus, (i32)CRUSTA_GEMINA_TOK_CLAUSURA,
            t))
        {
            redde FALSUM;
        }
        redde _partem_claudere(p, t);
    }

    si (g->machina != NIHIL)
    {
        b32 tractatum;

        si (!_arithmeticam_tractare(p, g, t, &tractatum))
        {
            redde FALSUM;
        }
        si (tractatum)
        {
            redde VERUM;
        }
    }

    si (_pars_est(t->genus))
    {
        redde _partem_recipere(p, t);
    }

    commutatio (t->genus)
    {
        casus CRUSTA_LEX_ARITHMETICA_APERTURA:
        {
            MateriaNodus* a;
            CrustaArithmetica* m;
            Gradus* novus;

            m = (CrustaArithmetica*)piscina_allocare(p->piscina,
                (memoriae_index)magnitudo(CrustaArithmetica));
            si (m == NIHIL)
            {
                redde FALSUM;
            }
            crusta_arithmetica_incipere(m, p->piscina, p->dialectus);
            /* 'for ((' : iteratio sine nomine fit cyclus (nodus
             * iterationis numquam appensus) */
            si (   g->genus  == (s32)CRUSTA_GENUS_ITERATIO
                && g->status == ZEPHYRUM
                && g->nodus->loci[CRUSTA_ITERATIO_TOK_APERTURA].datum
                    .token->valor.mensura == (i32)III)
            {
                a = _nodus(p, (s32)CRUSTA_GENUS_CYCLUS);
                si (   a == NIHIL
                    || !_token_ponere(a,
                    (i32)CRUSTA_CYCLUS_TOK_APERTURA,
                        g->nodus->loci[CRUSTA_ITERATIO_TOK_APERTURA]
                            .datum.token)
                    || !_token_ponere(a,
                        (i32)CRUSTA_CYCLUS_TOK_PARENTHESIS, t))
                {
                    redde FALSUM;
                }
                g->nodus    = a;
                g->genus    = (s32)CRUSTA_GENUS_CYCLUS;
                g->locus    = (i32)CRUSTA_CYCLUS_LIBERI;
                g->modus    = CRUSTA_MODUS_ARITHMETICA_SUMMA;
                g->machina  = m;
                redde VERUM;
            }
            si (!_sententiam_admittit(g))
            {
                redde _malum(p, t);
            }
            a = _nodus(p, (s32)CRUSTA_GENUS_ARITHMETICA);
            si (   a == NIHIL
                || !_token_ponere(a,
                (i32)CRUSTA_ARITHMETICA_TOK_APERTURA, t))
            {
                redde FALSUM;
            }
            novus = _pellere(p, a, (i32)CRUSTA_ARITHMETICA_EXPRESSIO,
                CRUSTA_MODUS_ARITHMETICA_SUMMA);
            si (novus == NIHIL)
            {
                redde FALSUM;
            }
            novus->machina = m;
            redde VERUM;
        }
        casus CRUSTA_LEX_TERMINATOR_OPTIONIS:
            redde _optionem_terminare(p, t);
        casus CRUSTA_LEX_SEPARATOR:
        casus CRUSTA_LEX_SEPARATOR_LINEAE:
            si (   g->genus == (s32)CRUSTA_GENUS_TABULATUM
                || _partes_directe(g->genus))
            {
                redde _malum(p, t);
            }
            redde _separator(p, t);
        casus CRUSTA_LEX_ET:
        casus CRUSTA_LEX_AUT:
            redde _catena(p, t);
        casus CRUSTA_LEX_PIPA:
        casus CRUSTA_LEX_PIPA_ERRORIS:
            /* '|' inter exemplaria optionis: operator nudus */
            si (   g->genus            == (s32)CRUSTA_GENUS_VERBUM
                && _infra(p, I)->genus == (s32)CRUSTA_GENUS_OPTIO)
            {
                si (!_verbum_claudere(p))
                {
                    redde FALSUM;
                }
                g = _vertex(p);
            }
            si (   g->genus == (s32)CRUSTA_GENUS_OPTIO
                && g->locus == (i32)CRUSTA_OPTIO_EXEMPLARIA)
            {
                redde _operator_appendere(p, g->nodus, g->locus, t);
            }
            redde _pipa(p, t);
        casus CRUSTA_LEX_ASSIGNATIO_TITULUS:
            redde _assignatio_incipere(p, t);
        casus CRUSTA_LEX_SUBSCRIPTUM:
            si (g->genus == (s32)CRUSTA_GENUS_ASSIGNATIO)
            {
                redde _token_ponere(g->nodus,
                    (i32)CRUSTA_ASSIGNATIO_TOK_SUBSCRIPTUM, t);
            }
            redde _assignatio_incipere(p, t);
        casus CRUSTA_LEX_ASSIGNATIO_OPERATOR:
            si (g->genus == (s32)CRUSTA_GENUS_ASSIGNATIO)
            {
                redde _token_ponere(g->nodus,
                    (i32)CRUSTA_ASSIGNATIO_TOK_OPERATOR, t);
            }
            redde _malum(p, t);
        casus CRUSTA_LEX_REDIRECTIO_FD:
        casus CRUSTA_LEX_REDIRECTIO_OPERATOR:
            redde _redirectio_incipere(p, t);
        casus CRUSTA_LEX_PARENTHESIS:
            /* valor tabulae post '=' */
            si (   g->genus == (s32)CRUSTA_GENUS_ASSIGNATIO
                && !_absens(g->nodus,
                (i32)CRUSTA_ASSIGNATIO_TOK_OPERATOR)
                && _absens(g->nodus, (i32)CRUSTA_ASSIGNATIO_VALOR))
            {
                MateriaNodus* tabula = _nodus(p,
                    (s32)CRUSTA_GENUS_TABULATUM);

                si (   tabula == NIHIL
                    || !_token_ponere(tabula,
                    (i32)CRUSTA_TABULATUM_TOK_APERTURA,
                        t))
                {
                    redde FALSUM;
                }
                redde _pellere(p, tabula, (i32)CRUSTA_TABULATUM_LIBERI,
                    CRUSTA_MODUS_TABULATUM) != NIHIL;
            }
            /* exemplar novum '(a)' */
            si (   g->genus == (s32)CRUSTA_GENUS_ELECTIO
                && g->locus == (i32)CRUSTA_ELECTIO_LIBERI)
            {
                redde _optio_aperire(p, t);
            }
            /* 'function f (' */
            si (   g->genus  == (s32)CRUSTA_GENUS_FUNCTIO
                && g->status == I)
            {
                si (!_token_ponere(g->nodus,
                        (i32)CRUSTA_FUNCTIO_TOK_APERTURA, t))
                {
                    redde FALSUM;
                }
                g->status = II;
                redde VERUM;
            }
            /* 'f()' : imperium unius verbi fit functio */
            si (   g->genus == (s32)CRUSTA_GENUS_VERBUM
                || g->genus == (s32)CRUSTA_GENUS_IMPERIUM)
            {
                redde _imperium_in_functionem(p, t);
            }
            /* subcrusta */
            redde _compositum_aperire(p, t, (s32)CRUSTA_GENUS_CRUSTULA,
                (i32)CRUSTA_CRUSTULA_TOK_APERTURA,
                (i32)CRUSTA_CRUSTULA_LIBERI, CRUSTA_MODUS_INITIUM);
        casus CRUSTA_LEX_PARENTHESIS_CLAUSURA:
        {
            s32 i;
            s32 n           = (s32)xar_numerus(p->gradus);
            s32 claudendum  = (s32)-I;
            s32 index       = (s32)-I;

            per (i = n - I; i >= ZEPHYRUM; i--)
            {
                Gradus* h = (Gradus*)xar_obtinere(p->gradus, (i32)i);

                si (   h->genus == (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO
                    || h->genus == (s32)CRUSTA_GENUS_PARS_PROCESSUS
                    || h->genus == (s32)CRUSTA_GENUS_TABULATUM
                    || (   h->genus == (s32)CRUSTA_GENUS_CRUSTULA
                        && h->status < II)
                    || (   h->genus == (s32)CRUSTA_GENUS_FUNCTIO
                        && h->status == II)
                    || (   h->genus == (s32)CRUSTA_GENUS_OPTIO
                        && h->locus == (i32)CRUSTA_OPTIO_EXEMPLARIA))
                {
                    claudendum  = h->genus;
                    index       = i;
                    frange;
                }
                si (h->genus == (s32)CRUSTA_GENUS_PROGRAMMA)
                {
                    frange;
                }
            }
            si (claudendum < ZEPHYRUM)
            {
                redde _malum(p, t);
            }
            si (!_claudere_usque(p, (i32)index + I))
            {
                redde FALSUM;
            }
            g = _vertex(p);
            si (claudendum == (s32)CRUSTA_GENUS_TABULATUM)
            {
                si (   !_token_ponere(g->nodus,
                        (i32)CRUSTA_TABULATUM_TOK_CLAUSURA, t)
                    || !_claudere(p))   /* tabulatum in assignationem */
                {
                    redde FALSUM;
                }
                redde _claudere(p);     /* assignatio in imperium */
            }
            si (claudendum == (s32)CRUSTA_GENUS_CRUSTULA)
            {
                si (!_token_ponere(g->nodus,
                        (i32)CRUSTA_CRUSTULA_TOK_CLAUSURA, t))
                {
                    redde FALSUM;
                }
                g->status  = II;
                g->locus   = (i32)CRUSTA_CRUSTULA_REDIRECTIONES;
                g->modus   = CRUSTA_MODUS_POST_COMPOSITUM;
                redde VERUM;
            }
            si (claudendum == (s32)CRUSTA_GENUS_FUNCTIO)
            {
                si (!_token_ponere(g->nodus,
                        (i32)CRUSTA_FUNCTIO_TOK_CLAUSURA, t))
                {
                    redde FALSUM;
                }
                g->status  = III;   /* corpus exspectatur */
                g->modus   = CRUSTA_MODUS_INITIUM;
                redde VERUM;
            }
            si (claudendum == (s32)CRUSTA_GENUS_OPTIO)
            {
                si (!_token_ponere(g->nodus,
                        (i32)CRUSTA_OPTIO_TOK_PARENTHESIS_CLAUSURA, t))
                {
                    redde FALSUM;
                }
                g->locus = (i32)CRUSTA_OPTIO_LIBERI;
                g->modus = CRUSTA_MODUS_INITIUM;
                redde VERUM;
            }
            si (!_token_ponere(g->nodus,
                    (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA, t))
            {
                redde FALSUM;
            }
            redde _partem_claudere(p, t);
        }
        casus CRUSTA_LEX_RESERVATUM:
            redde _reservatum(p, g, t);
        ordinarius:
            redde _malum(p, t);
    }
}

/* FINIS intra regionem: gradus possessor regionis (heredoc aut
 * substitutio backtick) proximus; supra iacentia clauduntur. Heredoc:
 * delimitator et finis. Backtick: petitiones heredoc intra regionem
 * factae intra clauduntur (corpus vacuum - bash regionem numquam
 * transit), deinde backtick clausurae petitur */
interior b32
_regionem_finire (
    Aedificatio* p)
{
    Gradus* g;
    MateriaToken* clausura;
    s32 i;
    s32 index = (s32)-I;

    per (i = (s32)xar_numerus(p->gradus) - I; i >= ZEPHYRUM; i--)
    {
        Gradus* h = (Gradus*)xar_obtinere(p->gradus, (i32)i);

        si (   h->genus == (s32)CRUSTA_GENUS_HEREDOC
            || (   h->genus == (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO
                && h->gravis))
        {
            index = i;
            frange;
        }
    }
    si (index < ZEPHYRUM)
    {
        /* regio sine possessore: clauditur */
        si (!_sententiam_claudere(p))
        {
            redde FALSUM;
        }
        crusta_lector_regionem_claudere(&p->lector);
        redde VERUM;
    }
    si (!_claudere_usque(p, (i32)index + I))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (g->genus == (s32)CRUSTA_GENUS_HEREDOC)
    {
        redde _heredoc_claudere(p);
    }
    /* petitiones intra backtick: corpora vacua intra */
    dum (   crusta_lector_heredoca_pendent(&p->lector)
         && p->lector.heredoca_caput >= g->heredoca_ante)
    {
        si (!_heredoc_aperire_proximum(p) || !_heredoc_claudere(p))
        {
            redde FALSUM;
        }
    }
    crusta_lector_regionem_claudere(&p->lector);
    clausura = crusta_lector_proximum(&p->lector, CRUSTA_MODUS_VERBA);
    si (clausura == NIHIL)
    {
        redde FALSUM;
    }
    si (clausura->genus != (s32)CRUSTA_LEX_GRAVIS)
    {
        /* backtick clausurae absens (EOF): lexema alienum reddendum -
         * situs reponitur, substitutio clauditur absens */
        CrustaSitus s = crusta_lector_situs(&p->lector);

        s.cursor = clausura->byte_offset;
        crusta_lector_situm_reponere(&p->lector, s);
        g->gravis = FALSUM;
        redde _claudere(p);
    }
        si (   !_lexema_recordare(p, clausura)
            || !_solvere(p, clausura)
            || !_token_ponere(g->nodus,
            (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA,
            clausura))
        {
        redde FALSUM;
        }
    redde _partem_claudere(p, clausura);
}

/* EOF: gradus omnes clausi, petitiones heredoc pendentes corpora
 * vacua (ambo lexemata absentia), FINIS in caudam */
interior b32
_finem_tractare (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g;

    si (!_claudere_usque(p, I))
    {
        redde FALSUM;
    }
    dum (crusta_lector_heredoca_pendent(&p->lector))
    {
        si (!_heredoc_aperire_proximum(p) || !_heredoc_claudere(p))
        {
            redde FALSUM;
        }
    }
    g = _vertex(p);
    redde _token_ponere(g->nodus, (i32)CRUSTA_PROGRAMMA_CAUDA, t);
}


/* ==================================================
 * Facies publica
 * ================================================== */

MateriaNodus*
crusta_arbor_parsare (
                     Piscina* piscina,
          constans character* fons,
                         i32  mensura,
    constans CrustaDialectus* dialectus,
               CrustaParsura* relatio)
{
    redde crusta_arbor_parsare_cum_lexematis(piscina, fons, mensura,
        dialectus, relatio, NIHIL);
}

MateriaNodus*
crusta_arbor_parsare_cum_lexematis (
                     Piscina* piscina,
          constans character* fons,
                         i32  mensura,
    constans CrustaDialectus* dialectus,
               CrustaParsura* relatio,
                         Xar* lexemata)
{
             Aedificatio p;
      MateriaLexIudicium iudicium;
    MateriaNodus* programma;
    CrustaParsura relatio_propria;

    memset(&p, ZEPHYRUM, magnitudo(p));
    memset(&relatio_propria, ZEPHYRUM, magnitudo(relatio_propria));
    p.piscina        = piscina;
        p.dialectus  = dialectus;
    p.lexemata       = lexemata;
    p.post_lineam    = VERUM;
    p.relatio        = relatio != NIHIL ? relatio : &relatio_propria;
    memset(p.relatio, ZEPHYRUM, magnitudo(*p.relatio));
    si (!materia_lexicon_ratum_facere(&p.lexicon, &CRUSTA_LEXICON,
            &iudicium))
    {
        redde NIHIL;
    }
    si (!crusta_lector_incipere(&p.lector, piscina, fons, mensura,
            dialectus))
    {
        redde NIHIL;
    }
    p.gradus     = xar_creare(piscina, (i32)magnitudo(Gradus));
    p.pendentia  = xar_creare(piscina, (i32)magnitudo(MateriaToken*));
    p.redirectiones_pendentes = xar_creare(piscina,
        (i32)magnitudo(MateriaNodus*));
    si (   p.gradus                  == NIHIL || p.pendentia == NIHIL
        || p.redirectiones_pendentes == NIHIL)
    {
        redde NIHIL;
    }
    programma = _nodus(&p, (s32)CRUSTA_GENUS_PROGRAMMA);
    si (   programma == NIHIL
        || _pellere(&p, programma, (i32)CRUSTA_PROGRAMMA_LIBERI,
            CRUSTA_MODUS_INITIUM) == NIHIL)
    {
        redde NIHIL;
    }

    dum (VERUM)
    {
              Gradus* g = _vertex(&p);
        MateriaToken* t = crusta_lector_proximum(&p.lector, g->modus);

                si (t == NIHIL || p.memoria_defecit)
                {
            redde NIHIL;
                }
        si (!_lexema_recordare(&p, t))
        {
            redde NIHIL;
        }
        si (materia_lexicon_trivium_est(&p.lexicon, t->genus))
        {
            /* trivium verbum claudit, et assignationem sine valore
             * ('a= b': valor absens) */
            si (   g->genus == (s32)CRUSTA_GENUS_VERBUM
                && !_verbum_claudere(&p))
            {
                redde NIHIL;
            }
            g = _vertex(&p);
            si (   g->genus == (s32)CRUSTA_GENUS_ASSIGNATIO
                && !_claudere(&p))
            {
                redde NIHIL;
            }
            si (!_cumulare(&p, t))
            {
                redde NIHIL;
            }
            /* linea nova trivium cum heredoc pendente: trivia post
             * prioris, corpus hic incipit */
            si (   t->genus == (s32)CRUSTA_LEX_LINEA
                && crusta_lector_heredoca_pendent(&p.lector)
                && (   !_solvere_retro(&p)
                    || !_heredoc_aperire_proximum(&p)))
            {
                redde NIHIL;
            }
            perge;
        }
        si (t->genus == (s32)CRUSTA_LEX_FINIS)
        {
            si (xar_numerus(p.lector.regiones) > ZEPHYRUM)
            {
                si (!_regionem_finire(&p))
                {
                    redde NIHIL;
                }
                perge;
            }
            si (!_solvere(&p, t) || !_finem_tractare(&p, t))
            {
                redde NIHIL;
            }
            frange;
        }
        si (!_solvere(&p, t) || !_tractare(&p, t))
        {
            redde NIHIL;
        }
        /* linea nova separator cum heredoc pendente: corpus post
         * separatorem (aut post lexema separatoris iterationis) */
        si (   t->genus == (s32)CRUSTA_LEX_SEPARATOR_LINEAE
            && crusta_lector_heredoca_pendent(&p.lector)
            && !_heredoc_aperire_proximum(&p))
        {
            redde NIHIL;
        }
    }
        p.relatio->sana = p.relatio->mala == ZEPHYRUM
                   && p.relatio->clausurae_absentes == ZEPHYRUM
                   && p.relatio->heredoca_transposita == ZEPHYRUM
                   && p.relatio->listae_vacuae == ZEPHYRUM;
    redde programma;
}


/* ==================================================
 * Valor staticus verbi
 * ================================================== */

/* longitudo partium staticarum (summa octetorum crudorum = tectum
 * valoris decocti); gemina et versa per partes suas UNO gradu (apices
 * intra apices non nidificant); -I si pars non statica adest -
 * decoctio ibi desinit, ergo longitudo quoque.
 *
 * INVENTUM P9 (porta totalitatis): forma prior subarborem TOTAM
 * recursive ambulabat, substitutiones intra. Verbum primum imperii
 * clausum valorem staticum petit (_aedificator_est), ergo in '$(' x N
 * quodque verbum clausum nidum totum infra se ambulabat: tempus
 * quadraticum (XVI milia = XV s) et recursio CCC milium graduum
 * (C milia = SIGSEGV post LXXX s). Nunc numquam in substitutionem
 * descendit. */
interior s32
_longitudo_statica (
    constans MateriaNodus* nodus,
                      i32  locus,
                      b32  intra_apices)
{
    constans MateriaValor* partes  = &nodus->loci[locus];
                      s32  summa   = ZEPHYRUM;
                      i32  k;

    si (partes->genus != MATERIA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*partes); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*partes,
            k);
        constans MateriaNodus* pars;
                          s32  citata;

        si (e->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        pars = e->datum.nodus;
        commutatio (pars->genus)
        {
            casus CRUSTA_GENUS_PARS_LITTERALIS:
            casus CRUSTA_GENUS_PARS_DOMUS:
            casus CRUSTA_GENUS_PARS_EFFUGIUM:
            casus CRUSTA_GENUS_PARS_CONTINUATIO:
            casus CRUSTA_GENUS_PARS_SIMPLEX:
            casus CRUSTA_GENUS_PARS_EFFUGIA:
                si (pars->loci[CRUSTA_PARS_TOK].genus
                    == MATERIA_VALOR_TOKEN)
                {
                    constans MateriaToken* t =
                        pars->loci[CRUSTA_PARS_TOK].datum.token;

                    /* effugia: '\u80' -> '\u0080' longius crudo */
                    summa += (s32)t->valor.mensura
                        * (pars->genus
                            == (s32)CRUSTA_GENUS_PARS_EFFUGIA ? II : I);
                }
                frange;
            casus CRUSTA_GENUS_PARS_GEMINA:
            casus CRUSTA_GENUS_PARS_VERSA:
                si (intra_apices)
                {
                    redde (s32)-I;
                }
                citata = _longitudo_statica(pars,
                    (i32)CRUSTA_GEMINA_PARTES, VERUM);
                si (citata < ZEPHYRUM)
                {
                    redde citata;
                }
                summa += citata;
                frange;
            ordinarius:
                redde (s32)-I;
        }
    }
    redde summa;
}

/* retroversa logica ex cursu crudo n profunditatis d (regula
 * lectoris) */
interior i32
_retro_logica (
    i32 n,
    i32 gravis,
    b32 exuendum)
{
    i32 r = n;
    i32 k;

    per (k = ZEPHYRUM; k < gravis; k++)
    {
        r = exuendum ? r / (i32)II : (r + I) / (i32)II;
    }
    redde r;
}

/* effugium decodare in scriptorem; gemina: regulae "..." */
interior i32
_effugium_decoquere (
      constans MateriaToken* t,
                  character* exitus)
{
          i32 n       = ZEPHYRUM;
          i32 gravis  = crusta_lector_cauda(t)->gravis;
          i32 r;
          i32 scripti = ZEPHYRUM;
    character post;
          i32 k;

    dum (n < t->valor.mensura && t->valor.datum[n] == '\\')
    {
        n++;
    }
    si (n == t->valor.mensura)
    {
        /* cursus purus: paria effugia, impar litterale */
        r = _retro_logica(n, gravis, FALSUM);
        per (k = ZEPHYRUM; k < (r + I) / (i32)II; k++)
        {
            exitus[scripti++] = '\\';
        }
        redde scripti;
    }
    post  = (character)t->valor.datum[n];
    r     = _retro_logica(n, gravis, post == '$' || post == '`');
    per (k = ZEPHYRUM; k < r / (i32)II; k++)
    {
        exitus[scripti++] = '\\';
    }
    exitus[scripti++] = post;
    redde scripti;
}

interior i32
_effugia_decoquere (
      constans MateriaToken* t,
                  character* exitus)
{
    i32 i        = (i32)II;   /* post $' */
    i32 finis    = t->valor.mensura;
    i32 scripti  = ZEPHYRUM;

    si (finis > (i32)II && t->valor.datum[finis - I] == '\'')
    {
        finis--;
    }
    dum (i < finis)
    {
        character c = (character)t->valor.datum[i];

        si (c != '\\' || i + I >= finis)
        {
            exitus[scripti++] = c;
            i++;
            perge;
        }
        c = (character)t->valor.datum[i + I];
        i += II;
        commutatio (c)
        {
            casus 'n':  exitus[scripti++] = '\n'; frange;
            casus 't':  exitus[scripti++] = '\t'; frange;
            casus 'r':  exitus[scripti++] = '\r'; frange;
            casus 'a':  exitus[scripti++] = '\a'; frange;
            casus 'b':  exitus[scripti++] = '\b'; frange;
            casus 'f':  exitus[scripti++] = '\f'; frange;
            casus 'v':  exitus[scripti++] = '\v'; frange;
            casus 'e':
            casus 'E':  exitus[scripti++] = (character)27; frange;
            casus '\\': exitus[scripti++] = '\\'; frange;
            casus '\'': exitus[scripti++] = '\''; frange;
            casus '"':  exitus[scripti++] = '"'; frange;
            casus '?':  exitus[scripti++] = '?'; frange;
            casus 'c':
                /* control: \cX = X & 31 ('?' = DEL; '\c\\' duos
                 * octetos consumit); in fine verbatim (P11b, bash
                 * 5.2 mensuratum) */
                si (i >= finis)
                {
                    exitus[scripti++] = '\\';
                    exitus[scripti++] = 'c';
                    frange;
                }
                {
                    character k = (character)t->valor.datum[i];

                    si (   k == '\\' && i + I < finis
                        && t->valor.datum[i + I] == '\\')
                    {
                        i++;
                    }
                    i++;
                    si (k == '?')
                    {
                        exitus[scripti++] = (character)127;
                    }
                    alioquin
                    {
                        si (k >= 'a' && k <= 'z')
                        {
                            k = (character)(k - ('a' - 'A'));
                        }
                        exitus[scripti++] = (character)(k & 31);
                    }
                }
                frange;
            casus 'x':
            casus 'u':
            casus 'U':
            {
                /* hex: \x II digiti, \u IV, \U VIII; nulli digiti ->
                 * verbatim. Unicode sub LC_ALL=C (aurum oraculi):
                 * < 0x80 octetus, aliter effugium normalizatum \uXXXX
                 * aut \UXXXXXXXX (bash 5.2 in macOS mensuratum; tectum
                 * longitudinis = crudum x II) */
                longus insignatus v;
                              i32 d;
                              i32 maximum;

                v = 0UL;
                d = ZEPHYRUM;
                maximum = c == 'x' ? (i32)II
                    : c == 'u' ? (i32)IV : (i32)VIII;
                dum (d < maximum && i < finis)
                {
                    character h = (character)t->valor.datum[i];
                          i32 valor;

                    si (h >= '0' && h <= '9')
                    {
                        valor = (i32)(h - '0');
                    }
                    alioquin si (h >= 'a' && h <= 'f')
                    {
                        valor = (i32)(h - 'a') + (i32)X;
                    }
                    alioquin si (h >= 'A' && h <= 'F')
                    {
                        valor = (i32)(h - 'A') + (i32)X;
                    }
                    alioquin
                    {
                        frange;
                    }
                    v = v * 16UL + (longus insignatus)valor;
                    i++;
                    d++;
                }
                si (d == ZEPHYRUM)
                {
                    exitus[scripti++] = '\\';
                    exitus[scripti++] = c;
                }
                alioquin si (c == 'x' || v < 128UL)
                {
                    exitus[scripti++] = (character)v;
                }
                alioquin si (v <= 65535UL)
                {
                    scripti += (i32)sprintf(exitus + scripti,
                        "\\u%04lX",
                        v);
                }
                alioquin
                {
                    scripti += (i32)sprintf(exitus + scripti,
                        "\\U%08lX",
                        v);
                }
                frange;
            }
            ordinarius:
                si (c >= '0' && c <= '7')
                {
                    i32 v = (i32)(c - '0');
                    i32 d = I;

                    dum (   d < (i32)III && i < finis
                         && t->valor.datum[i] >= '0'
                         && t->valor.datum[i] <= '7')
                    {
                        v = v * (i32)VIII + (i32)(t->valor.datum[i]
                            - '0');
                        i++;
                        d++;
                    }
                    exitus[scripti++] = (character)v;
                }
                alioquin
                {
                    exitus[scripti++] = '\\';
                    exitus[scripti++] = c;
                }
                frange;
        }
    }
    redde scripti;
}

interior b32
_partes_decoquere (
    constans MateriaNodus* verbum,
                      i32  locus,
                character* exitus,
                      i32* scripti)
{
    constans MateriaValor* partes = &verbum->loci[locus];
                      i32  k;

    si (partes->genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*partes); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*partes,
            k);
        constans MateriaNodus* pars;
        constans MateriaToken* t;

        si (e->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        pars = e->datum.nodus;
        commutatio (pars->genus)
        {
            casus CRUSTA_GENUS_PARS_LITTERALIS:
            casus CRUSTA_GENUS_PARS_DOMUS:
                t = pars->loci[CRUSTA_PARS_TOK].datum.token;
                memcpy(exitus + *scripti, t->valor.datum,
                    (size_t)t->valor.mensura);
                *scripti += t->valor.mensura;
                frange;
            casus CRUSTA_GENUS_PARS_EFFUGIUM:
                t = pars->loci[CRUSTA_PARS_TOK].datum.token;
                *scripti += _effugium_decoquere(t, exitus + *scripti);
                frange;
            casus CRUSTA_GENUS_PARS_CONTINUATIO:
                frange;
            casus CRUSTA_GENUS_PARS_SIMPLEX:
            {
                i32 ab = I;
                i32 ad;

                t   = pars->loci[CRUSTA_PARS_TOK].datum.token;
                ad  = t->valor.mensura;
                si (ad > I && t->valor.datum[ad - I] == '\'')
                {
                    ad--;
                }
                si (ad > ab)
                {
                    memcpy(exitus + *scripti, t->valor.datum + ab,
                        (size_t)(ad - ab));
                    *scripti += ad - ab;
                }
                frange;
            }
            casus CRUSTA_GENUS_PARS_EFFUGIA:
                t = pars->loci[CRUSTA_PARS_TOK].datum.token;
                *scripti += _effugia_decoquere(t, exitus + *scripti);
                frange;
            casus CRUSTA_GENUS_PARS_GEMINA:
            casus CRUSTA_GENUS_PARS_VERSA:
                si (!_partes_decoquere(pars, (i32)CRUSTA_GEMINA_PARTES,
                        exitus, scripti))
                {
                    redde FALSUM;
                }
                frange;
            ordinarius:
                redde FALSUM;   /* expansio: non staticum */
        }
    }
    redde VERUM;
}

b32
crusta_verbum_staticum (
                  Piscina* piscina,
    constans MateriaNodus* verbum,
                   chorda* valor)
{
          s32  longitudo;
    character* area;
          i32  scripti = ZEPHYRUM;

    si (verbum == NIHIL || verbum->genus != (s32)CRUSTA_GENUS_VERBUM)
    {
        redde FALSUM;
    }
    longitudo = _longitudo_statica(verbum, (i32)CRUSTA_VERBUM_PARTES,
        FALSUM);
    si (longitudo < ZEPHYRUM)
    {
        redde FALSUM;
    }
    area = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (area == NIHIL)
    {
        redde FALSUM;
    }
    si (!_partes_decoquere(verbum, (i32)CRUSTA_VERBUM_PARTES, area,
            &scripti))
    {
        redde FALSUM;
    }
    valor->mensura  = scripti;
    valor->datum    = (i8*)area;
    redde VERUM;
}

b32
crusta_effugia_decoquere (
                  Piscina* piscina,
    constans MateriaNodus* pars,
                   chorda* valor)
{
    constans MateriaToken* t;
                character* area;

    si (   pars == NIHIL
        || pars->genus != (s32)CRUSTA_GENUS_PARS_EFFUGIA
        || pars->loci[CRUSTA_PARS_TOK].genus != MATERIA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    t    = pars->loci[CRUSTA_PARS_TOK].datum.token;
    /* decoctio numquam longior quam crudum x II ('\u80' -> '\u0080') */
    area = (character*)piscina_allocare(piscina,
        (memoriae_index)t->valor.mensura * II + I);
    si (area == NIHIL)
    {
        redde FALSUM;
    }
    valor->mensura  = _effugia_decoquere(t, area);
    valor->datum    = (i8*)area;
    redde VERUM;
}

b32
crusta_verbum_citatum (
    constans MateriaNodus* verbum)
{
    constans MateriaValor* partes;
                      i32  k;

    si (verbum == NIHIL || verbum->genus != (s32)CRUSTA_GENUS_VERBUM)
    {
        redde FALSUM;
    }
    partes = &verbum->loci[CRUSTA_VERBUM_PARTES];
    si (partes->genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*partes); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*partes,
            k);
                          s32 g;

        si (e->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        g = e->datum.nodus->genus;
        si (   g == (s32)CRUSTA_GENUS_PARS_SIMPLEX
            || g == (s32)CRUSTA_GENUS_PARS_GEMINA
            || g == (s32)CRUSTA_GENUS_PARS_EFFUGIUM
            || g == (s32)CRUSTA_GENUS_PARS_EFFUGIA
            || g == (s32)CRUSTA_GENUS_PARS_VERSA)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

constans MateriaNodus*
crusta_imperium_titulus (
    constans MateriaNodus* imperium)
{
    constans MateriaValor* liberi;
                      i32  k;

    si (   imperium        == NIHIL
        || imperium->genus != (s32)CRUSTA_GENUS_IMPERIUM)
    {
        redde NIHIL;
    }
    liberi = &imperium->loci[CRUSTA_IMPERIUM_LIBERI];
    si (liberi->genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*liberi); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*liberi,
            k);

        si (   e->genus              == MATERIA_VALOR_NODUS
            && e->datum.nodus->genus == (s32)CRUSTA_GENUS_VERBUM)
        {
            redde e->datum.nodus;
        }
    }
    redde NIHIL;
}

Xar*
crusta_imperium_argumenta (
                  Piscina* piscina,
    constans MateriaNodus* imperium)
{
                      Xar* exitus;
    constans MateriaNodus* titulus = crusta_imperium_titulus(imperium);
    constans MateriaValor* liberi;
                      b32  post_titulum = FALSUM;
                      i32  k;

    exitus = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));
    si (exitus == NIHIL || titulus == NIHIL)
    {
        redde exitus;
    }
    liberi = &imperium->loci[CRUSTA_IMPERIUM_LIBERI];
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*liberi); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*liberi,
            k);

        si (e->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        si (e->datum.nodus == titulus)
        {
            post_titulum = VERUM;
            perge;
        }
        si (   post_titulum
            && e->datum.nodus->genus == (s32)CRUSTA_GENUS_VERBUM)
        {
            MateriaNodus** sedes = (MateriaNodus**)xar_addere(exitus);

            si (sedes == NIHIL)
            {
                redde NIHIL;
            }
            *sedes = e->datum.nodus;
        }
    }
    redde exitus;
}
