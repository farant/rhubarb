/* stml_macros.c - Expansio fragmentorum parametrizatorum (v1)
 *
 * Ambulatio una sinistra-dextra super documentum: definitiones
 * (<#id>) colliguntur ordine documenti et ex emissione DEMITTUNTUR
 * (arbor expansa = visio CONTENTI); vocationes (<<#id args>>)
 * corporibus clonatis impletisque substituuntur; cetera verbatim
 * clonantur. Clonatio: nodi recentes in piscina vocantis
 * (originalis immutabilis - lectio simulationis mutationum I);
 * chordae internatae per punctatorem communicantur (immutabiles).
 *
 * Spec: project-specs/stml-macros-spec.md. Planum:
 * stml-macros-plan.md (T2: sceletum - documentum sine macris
 * clonatur, tabula vacua).
 */

#include "stml_macros.h"
#include "chorda_aedificator.h"

/* Loculus declaratus: titulus (sine '?') + optionalis ('p="@p?"',
 * par. 6.2). Optionalis absens licite manet: NON_IMPLETUS eum
 * praeterit, bracchia nihil COMMUTATIONIS eum vident; referentia
 * extra bracchium praesentiam statuens = LOCULUS_NON_ANGUSTATUS
 * (angustatio fluxus, collectione iudicata). */
nomen structura {
    chorda* titulus;     /* internatum */
       b32  optionalis;
} StmlMacroLoculus;

/* Definitio collecta: id internatum -> nodus definitionis.
 * 'praeterita' = ambulatio expansionis eam iam demisit - vocationes
 * eam vident (strata ordine documenti: vocatio ante definitionem =
 * FRAGMENTUM_POSTERIUS, non IGNOTUM). Xar cum scansione lineari -
 * definitiones per documentum paucae. 'loculi' = nomina declarata
 * in tago aperienti (attr valor '@nomen'; '@.' recusatum - formae
 * sparsae reservatae). */
nomen structura {
        chorda* id;          /* internatum */
     StmlNodus* definitio;
           Xar* loculi;      /* StmlMacroLoculus */
           i32  ordo;        /* index collectionis = ordo documenti */
           b32  praeterita;
} StmlMacroDefinitio;

/* Argumentum vocationis - UNA tabula ambabus formis (par. 6.1:
 * aequivalentia in tabula machinae vivit, corpus formam numquam
 * discernit): inscripta ('p="123"' ex interiore transclusionis)
 * et bloci ('<@p=>...</>' elementa statim sequentia). Scalaris:
 * valor non-NIHIL, arbores NIHIL. Subarboreus: valor NIHIL,
 * arbores = silva EXPANSA (contextu vocantis; splices clonant -
 * fons immutabilis manet, instantiatio quaque sede). */
nomen structura {
    chorda* titulus;  /* internatum */
    chorda* valor;    /* internatum; NIHIL si subarboreus */
       Xar* arbores;  /* StmlNodus* expansi; NIHIL si scalaris */
} StmlMacroArgumentum;

nomen structura {
                 Piscina* piscina;
     InternamentumChorda* intern;
                     Xar* definitiones;  /* StmlMacroDefinitio */
    StmlExpansioResultus* resultus;      /* campi vitii hic ponuntur */

    /* EXEMPLARIA (gradus II) - omnia pigre creata: documentum sine
     * exemplaribus nihil solvit */
                     Xar* relationes;     /* StmlExemplarRelatio */
                     Xar* transparentia;  /* chorda* (tagi internati) */
    /* Radix arboris expansae SUB CONSTRUCTIONE - nodus primus
     * creatus. Ambulatio ordine documenti aedificat, ergo arbor
     * partialis == 'contentum SUPRA' exacte: scopus exemplaris
     * defaltus, lex stratorum gratis (spec par. 2.1/par. 3). */
              StmlNodus* radix_expansa;
    /* Altitudo PORTATIONIS (>0 = intra instantiationem nexus
     * catenae): exemplaria INERTA dum portantur - EXEMPLAR/PER/
     * TRANSPARENTIA/CATENA producta ut datum emittuntur, numquam
     * consumpta nec vitium XIII (refinamentum portationis-contra-
     * applicationem, ratificatio 2026-08-31). */
                    i32 applicatio;
} StmlMacroContextus;

/* Ligamen unum congruentiae laxae: gradus I (nodus) aut II (valor).
 * (Gradus III - silva - capturis quantificatis reservatus.) */
nomen structura {
        chorda* titulus;  /* sine '$', internatum */
     StmlNodus* nodus;    /* NIHIL si valor */
        chorda* valor;    /* NIHIL si nodus */
} StmlExemplarLigamen;

/* Congruentia una: radix + ligamina eius (spec par. 1.2) */
nomen structura {
     StmlNodus* radix;
           Xar* ligamina;  /* StmlExemplarLigamen */
} StmlExemplarCongruentia;

/* Relatio nominata: quod output= ligat, quod de=/congruentia=
 * consumit. Adsignatio unica ordine documenti (strata). */
nomen structura {
        chorda* titulus;       /* '$nomen' internatum, sigillo */
           Xar* congruentiae;  /* StmlExemplarCongruentia */
     StmlNodus* origo;         /* EXEMPLAR originalis (linea vitii) */
           b32  consumpta;
} StmlExemplarRelatio;

interior vacuum
_vitium_ponere (
    StmlMacroContextus* ctx,
    StmlExpansioVitium  vitium,
             StmlNodus* nodus,
                chorda* fragmentum,
                chorda* loculus)
{
    /* primum vincit */
    si (ctx->resultus->vitium != STML_EXPANSIO_BENE)
    {
        redde;
    }
    ctx->resultus->vitium  = vitium;
    ctx->resultus->linea   = nodus != NIHIL ? nodus->linea : ZEPHYRUM;
    si (fragmentum != NIHIL)
    {
        ctx->resultus->fragmentum = *fragmentum;
    }
    si (loculus != NIHIL)
    {
        ctx->resultus->loculus = *loculus;
    }
}

/* Estne nodus definitio TEMPLI? Spatium templi sigillo '@' ducenti
 * signatur ('<#@f>'); fragmenta sine '@' (anonyma aut nominata,
 * e.g. '<#lex1>' arboris) fragmenta CONTENTI sunt - transclusio
 * eorum ALIAS est (identitas rei consumentis), non instantiatio,
 * ergo machina ea numquam tangit. Decretum 2026-08-26:
 * transclusio = alias, templum = instantiatio. */
interior b32
_est_definitio (
    constans StmlNodus* nodus)
{
    redde    nodus->genus == STML_NODUS_ELEMENTUM
          && nodus->fragmentum
          && nodus->fragmentum_id != NIHIL
          && nodus->fragmentum_id->mensura > I
          && nodus->fragmentum_id->datum[ZEPHYRUM] == (i8)'@';
}

interior StmlMacroDefinitio*
_definitionem_invenire (
    StmlMacroContextus* ctx,
                chorda* id)
{
    i32 i;
    i32 num;

    num = xar_numerus(ctx->definitiones);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlMacroDefinitio* def;

        def = (StmlMacroDefinitio*)xar_obtinere(ctx->definitiones, i);
        /* punctatores internati: aequalitas contenti = aequalitas
         * punctatoris */
        si (def != NIHIL && def->id == id)
        {
            redde def;
        }
    }
    redde NIHIL;
}

/* Nomen loculi: [A-Za-z0-9_.-]+. '.' INCLUSUM consulto: formae
 * sparsae reservatae ('&@...liberi;') ut nomina ordinaria
 * scanduntur, numquam declarari possunt ('@.' recusatum infra),
 * ergo LOCULUS_IGNOTUS clamant - reservatio clara, non tacita. */
interior b32
_character_nominis (
    i8 c)
{
    redde    (c >= (i8)'a' && c <= (i8)'z')
          || (c >= (i8)'A' && c <= (i8)'Z')
          || (c >= (i8)'0' && c <= (i8)'9')
          || c == (i8)'_'
          || c == (i8)'-'
          || c == (i8)'.';
}

/* Extensionem '&@nomen;' proximam invenire. Formae imperfectae
 * (sine ';', nomen vacuum) litterae manent - regula entis ignoti.
 * GRAMMATICA UNA collectionis et impletionis: quod collectio
 * iudicat, impletio substituit - numquam divergant. */
interior b32
_loculum_invenire (
    constans chorda* textus,
                i32  ab,
                i32* initium,
                i32* post,
             chorda* titulus)
{
    i32 i;

    i = ab;
    dum (i + II < textus->mensura)
    {
        si (   textus->datum[i]     == (i8)'&'
            && textus->datum[i + I] == (i8)'@')
        {
            i32 n;

            n = i + II;
            dum (   n < textus->mensura
                 && _character_nominis(textus->datum[n]))
            {
                n++;
            }
            si (   n > i + II
                && n < textus->mensura
                && textus->datum[n] == (i8)';')
            {
                *initium          = i;
                *post             = n + I;
                titulus->datum    = textus->datum + i + II;
                titulus->mensura  = n - (i + II);
                redde VERUM;
            }
        }
        i++;
    }
    redde FALSUM;
}

interior StmlMacroLoculus*
_loculus_declaratus (
    StmlMacroDefinitio* def,
                chorda* titulus)  /* internatum */
{
    i32 i;
    i32 num;

    num = xar_numerus(def->loculi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlMacroLoculus* loc;

        loc = (StmlMacroLoculus*)xar_obtinere(def->loculi, i);
        si (loc != NIHIL && loc->titulus == titulus)
        {
            redde loc;
        }
    }
    redde NIHIL;
}

/* Estne loculus in acervo angustationum? (bracchia est/non-nihil
 * COMMUTATIONIS praesentiam statuentia eum inseruerunt) */
interior b32
_in_angustatis (
       Xar* angustati,
    chorda* titulus)  /* internatum */
{
    i32 i;
    i32 num;

    num = xar_numerus(angustati);
    per (i = ZEPHYRUM; i < num; i++)
    {
        chorda** cella;

        cella = (chorda**)xar_obtinere(angustati, i);
        si (cella != NIHIL && *cella == titulus)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Declarationes loculorum ex tago aperienti definitionis: attributa
 * quorum valor '@nomen' est ('@' solum aut '@.' = attributum
 * ordinarium, non declaratio). */
interior vacuum
_loculos_declaratos_legere (
             StmlNodus* nodus,
    StmlMacroDefinitio* def,
    StmlMacroContextus* ctx)
{
    i32 i;
    i32 num;

    si (nodus->attributa == NIHIL)
    {
        redde;
    }
    num = xar_numerus(nodus->attributa);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlAttributum* attr;

        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
        si (   attr                         != NIHIL
            && attr->valor                  != NIHIL
            && attr->valor->mensura > I
            && attr->valor->datum[ZEPHYRUM] == (i8)'@'
            && attr->valor->datum[I]        != (i8)'.')
        {
                      chorda  titulus;
            StmlMacroLoculus* loc;
                         b32  optionalis;

            titulus.datum    = attr->valor->datum + I;
            titulus.mensura  = attr->valor->mensura - I;
            optionalis       = FALSUM;
            /* declaratio optionalis 'p="@p?"' (par. 6.2): '?'
             * caudalis pars declarationis est, non nominis */
            si (   titulus.mensura > I
                && titulus.datum[titulus.mensura - I] == (i8)'?')
            {
                optionalis       = VERUM;
                titulus.mensura  -= I;
            }
            loc = (StmlMacroLoculus*)xar_addere(def->loculi);
            si (loc != NIHIL)
            {
                loc->titulus     = chorda_internare(ctx->intern,
                                                    titulus);
                loc->optionalis  = optionalis;
            }
        }
    }
}

/* Chordam contra loculos declaratos perscrutari (collectione:
 * referentia non declarata = LOCULUS_IGNOTUS in loco definitionis,
 * ante vocationem ullam; referentia loculi OPTIONALIS extra
 * bracchium praesentiam statuens = LOCULUS_NON_ANGUSTATUS -
 * angustatio fluxus, par. 6.2). */
interior b32
_chordam_perscrutari (
       constans chorda* textus,
             StmlNodus* nodus,
    StmlMacroDefinitio* def,
    StmlMacroContextus* ctx,
                   Xar* angustati)
{
    chorda titulus;
       i32 ab;
       i32 initium;
       i32 post;

    ab = ZEPHYRUM;
    dum (_loculum_invenire(textus, ab, &initium, &post, &titulus))
    {
                  chorda* titulus_internatus;
        StmlMacroLoculus* loc;

        titulus_internatus = chorda_internare(ctx->intern, titulus);
        loc = _loculus_declaratus(def, titulus_internatus);
        si (loc == NIHIL)
        {
            _vitium_ponere(ctx, STML_EXPANSIO_LOCULUS_IGNOTUS,
                           nodus, def->id, titulus_internatus);
            redde FALSUM;
        }
        si (   loc->optionalis
            && !_in_angustatis(angustati, loc->titulus))
        {
            _vitium_ponere(ctx,
                           STML_EXPANSIO_LOCULUS_NON_ANGUSTATUS,
                           nodus, def->id, titulus_internatus);
            redde FALSUM;
        }
        ab = post;
    }
    redde VERUM;
}

interior b32
_est_titulo (
    constans StmlNodus* nodus,
    constans character* titulus)
{
    redde    nodus->genus == STML_NODUS_ELEMENTUM
          && nodus->titulus != NIHIL
          && chorda_aequalis_literis(*nodus->titulus, titulus);
}

/* Estne '&@x;' TOTUS textus? (referentia una, ab initio ad finem)
 * Titulus internatus redditur; NIHIL si non totus-ref. */
interior chorda*
_referentia_tota (
    StmlMacroContextus* ctx,
       constans chorda* textus)
{
    chorda titulus;
       i32 initium;
       i32 post;

    si (   _loculum_invenire(textus, ZEPHYRUM, &initium, &post,
                             &titulus)
        && initium == ZEPHYRUM
        && post    == textus->mensura)
    {
        redde chorda_internare(ctx->intern, titulus);
    }
    redde NIHIL;
}

interior b32
_corpus_perscrutari (
             StmlNodus* nodus,
    StmlMacroDefinitio* def,
    StmlMacroContextus* ctx,
                   Xar* angustati);

/* COMMUTATIONEM perscrutari (par. 6.2, collectione - planum
 * statice iudicabile): de totus-ref '&@x;' declaratus (referentia
 * SCRUTANS - ab angustatione exempta, praesente et absente
 * legalis); liberi CASUS + ORDINARIUS ultimus ad summum unus;
 * casus attributum unum ex est/nihil/non-nihil, cetera ignota =
 * malformatio; est sine referentiis (LINEA: litterae, numquam
 * praedicata computata). Bracchia est/non-nihil praesentiam
 * STATUUNT - loculus de in acervum angustationum intra ea;
 * nihil et ORDINARIUS non (ORDINARIUS et praesente et absente
 * exsequitur). */
interior b32
_commutationem_perscrutari (
             StmlNodus* com,
    StmlMacroDefinitio* def,
    StmlMacroContextus* ctx,
                   Xar* angustati)
{
              chorda* de;
              chorda* referentia;
    StmlMacroLoculus* loc;
                 i32  i;
                 i32  num;
                 b32  post_ordinarium;

    de = stml_attributum_capere(com, "de");
    si (de == NIHIL)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_COMMUTATIO_MALFORMATA,
                       com, def->id, NIHIL);
        redde FALSUM;
    }
    referentia = _referentia_tota(ctx, de);
    si (referentia == NIHIL)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_COMMUTATIO_MALFORMATA,
                       com, def->id, NIHIL);
        redde FALSUM;
    }
    loc = _loculus_declaratus(def, referentia);
    si (loc == NIHIL)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_LOCULUS_IGNOTUS, com,
                       def->id, referentia);
        redde FALSUM;
    }

    num             = com->liberi != NIHIL
        ? xar_numerus(com->liberi) : ZEPHYRUM;
    post_ordinarium = FALSUM;
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* bracchium;

        bracchium = *(StmlNodus**)xar_obtinere(com->liberi, i);
        si (   bracchium        == NIHIL
            || bracchium->genus == STML_NODUS_COMMENTUM)
        {
            perge;
        }
        si (post_ordinarium)
        {
            /* ORDINARIUS ultimus (decretum) */
            _vitium_ponere(ctx,
                           STML_EXPANSIO_COMMUTATIO_MALFORMATA,
                           bracchium, def->id, NIHIL);
            redde FALSUM;
        }
        si (_est_titulo(bracchium, "ORDINARIUS"))
        {
            post_ordinarium = VERUM;
            si (!_corpus_perscrutari(bracchium, def, ctx,
                                     angustati))
            {
                redde FALSUM;
            }
            perge;
        }
        si (_est_titulo(bracchium, "CASUS"))
        {
            chorda* est;
               b32  habet_nihil;
               b32  habet_non_nihil;
               i32  formae;

            est              = stml_attributum_capere(bracchium,
                                                      "est");
            habet_nihil      = stml_attributum_capere(bracchium,
                                   "nihil") != NIHIL;
            habet_non_nihil  = stml_attributum_capere(bracchium,
                                   "non-nihil") != NIHIL;
            formae = (est != NIHIL ? I : ZEPHYRUM)
                   + (habet_nihil ? I : ZEPHYRUM)
                   + (habet_non_nihil ? I : ZEPHYRUM);
            si (formae != I)
            {
                _vitium_ponere(ctx,
                               STML_EXPANSIO_COMMUTATIO_MALFORMATA,
                               bracchium, def->id, NIHIL);
                redde FALSUM;
            }
            si (est != NIHIL)
            {
                chorda titulus_ref;
                   i32 initium;
                   i32 post;

                /* LINEA: est littera est - referentia intra eam
                 * praedicatum computatum esset */
                si (_loculum_invenire(est, ZEPHYRUM, &initium,
                                      &post, &titulus_ref))
                {
                    _vitium_ponere(ctx,
                        STML_EXPANSIO_COMMUTATIO_MALFORMATA,
                        bracchium, def->id, NIHIL);
                    redde FALSUM;
                }
            }
            si (est != NIHIL || habet_non_nihil)
            {
                 chorda** cella;
                    b32   bene;

                cella = (chorda**)xar_addere(angustati);
                si (cella == NIHIL)
                {
                    redde FALSUM;
                }
                *cella = loc->titulus;
                bene   = _corpus_perscrutari(bracchium, def, ctx,
                                             angustati);
                xar_removere_ultimum(angustati);
                si (!bene)
                {
                    redde FALSUM;
                }
            }
            alioquin
            {
                si (!_corpus_perscrutari(bracchium, def, ctx,
                                         angustati))
                {
                    redde FALSUM;
                }
            }
            perge;
        }
        _vitium_ponere(ctx, STML_EXPANSIO_COMMUTATIO_MALFORMATA,
                       bracchium, def->id, NIHIL);
        redde FALSUM;
    }
    redde VERUM;
}

/* Corpus definitionis perscrutari: valores textus et attributorum
 * recursive (attributa tagi definitionis IPSIUS non - ea
 * declarationes sunt). 'angustati' = acervus loculorum quorum
 * praesentia in loco currenti statuta est (par. 6.2). */
interior b32
_corpus_perscrutari (
             StmlNodus* nodus,
    StmlMacroDefinitio* def,
    StmlMacroContextus* ctx,
                   Xar* angustati)
{
    i32 i;
    i32 num;

    si (nodus->liberi == NIHIL)
    {
        redde VERUM;
    }
    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
         StmlNodus* liberum;
               i32  j;
               i32  num_attr;

        liberum = *(StmlNodus**)xar_obtinere(nodus->liberi, i);
        si (liberum == NIHIL)
        {
            perge;
        }
        si (_est_definitio(liberum))
        {
            /* fragmentum nidificatum = materia definitionis
             * citata, OPACA: eius '&@' ad loculos SUOS futuros
             * refert, non ad nostros - nec scanditur nec
             * substituitur (regula una utrimque) */
            perge;
        }
        si (_est_titulo(liberum, "COMMUTATIO"))
        {
            /* attributa (de) et bracchia SEORSUM iudicata - de
             * referentia scrutans est, ab angustatione exempta */
            si (!_commutationem_perscrutari(liberum, def, ctx,
                                            angustati))
            {
                redde FALSUM;
            }
            perge;
        }
        si (   (   liberum->genus == STML_NODUS_TEXTUS
                || liberum->genus == STML_NODUS_TRANSCLUSIO)
            && liberum->valor != NIHIL
            && !_chordam_perscrutari(liberum->valor, liberum, def,
                                     ctx, angustati))
        {
            redde FALSUM;
        }
        si (liberum->attributa != NIHIL)
        {
            num_attr = xar_numerus(liberum->attributa);
            per (j = ZEPHYRUM; j < num_attr; j++)
            {
                StmlAttributum* attr;

                attr = (StmlAttributum*)xar_obtinere(
                    liberum->attributa, j);
                si (   attr        != NIHIL
                    && attr->valor != NIHIL
                    && !_chordam_perscrutari(attr->valor, liberum,
                                             def, ctx, angustati))
                {
                    redde FALSUM;
                }
            }
        }
        si (!_corpus_perscrutari(liberum, def, ctx, angustati))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Praetransitus: definitiones colligere ordine documenti (in
 * corpora definitionum NON descendit - fragmentum intra corpus
 * contentum est, non definitio). GEMINUM hic capitur (linea =
 * definitionis secundae). */
interior b32
_definitiones_colligere (
             StmlNodus* nodus,
    StmlMacroContextus* ctx)
{
    i32 i;
    i32 num;

    si (nodus == NIHIL)
    {
        redde VERUM;
    }
    si (_est_definitio(nodus))
    {
        StmlMacroDefinitio* prior;
        StmlMacroDefinitio* nova;

        prior = _definitionem_invenire(ctx, nodus->fragmentum_id);
        si (prior != NIHIL)
        {
            _vitium_ponere(ctx, STML_EXPANSIO_FRAGMENTUM_GEMINUM,
                           nodus, nodus->fragmentum_id, NIHIL);
            redde FALSUM;
        }
        nova = (StmlMacroDefinitio*)xar_addere(ctx->definitiones);
        si (nova == NIHIL)
        {
            redde FALSUM;
        }
        nova->id          = nodus->fragmentum_id;
        nova->definitio   = nodus;
        nova->ordo        = xar_numerus(ctx->definitiones) - I;
        nova->praeterita  = FALSUM;
        nova->loculi      = xar_creare(ctx->piscina,
                                       magnitudo(StmlMacroLoculus));
        si (nova->loculi == NIHIL)
        {
            redde FALSUM;
        }
        _loculos_declaratos_legere(nodus, nova, ctx);
        {
            Xar* angustati;

            angustati = xar_creare(ctx->piscina,
                                   magnitudo(chorda*));
            si (angustati == NIHIL)
            {
                redde FALSUM;
            }
            si (!_corpus_perscrutari(nodus, nova, ctx, angustati))
            {
                redde FALSUM;
            }
        }
        redde VERUM;  /* in corpus non descendere */
    }
    si (nodus->liberi != NIHIL)
    {
        num = xar_numerus(nodus->liberi);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlNodus* liberum;

            liberum = *(StmlNodus**)xar_obtinere(nodus->liberi, i);
            si (   liberum != NIHIL
                && !_definitiones_colligere(liberum, ctx))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* Estne transclusio vocatio templi? (valor incipit '#@' - spatium
 * templi solum; '<<#lex1>>' transclusio contenti manet, quam
 * consumens resolvit) */
interior b32
_est_vocatio (
    constans StmlNodus* nodus)
{
    redde    nodus->genus == STML_NODUS_TRANSCLUSIO
          && nodus->valor != NIHIL
          && nodus->valor->mensura > II
          && nodus->valor->datum[ZEPHYRUM] == (i8)'#'
          && nodus->valor->datum[I] == (i8)'@';
}

interior b32
_est_spatium_interius (
    i8 c)
{
    redde c == (i8)' ' || c == (i8)'\t' || c == (i8)'\n';
}

/* Estne nodus elementum argumenti? ('<@titulus=>' - par. 6.3;
 * statim post vocationem = argumentum VOCATIONIS, par. 6.1) */
interior b32
_est_argumentum_elementi (
    constans StmlNodus* nodus)
{
    redde    nodus->genus == STML_NODUS_ELEMENTUM
          && nodus->attributum_titulus != NIHIL;
}

/* Id vocationis ex interiore transclusionis: post '#' usque ad
 * spatium primum aut finem ('#f p="123"' -> 'f'). 'post' = index
 * in valore ubi argumenta incipiunt. 'valor' = interior EFFECTIVUS
 * (in impletionibus iam substitutus - transitio argumentorum). */
interior chorda*
_vocationis_id (
    StmlMacroContextus* ctx,
                chorda* valor,
                   i32* post)
{
    chorda id;
       i32 finis;

    id.datum    = valor->datum + I;
    id.mensura  = valor->mensura - I;
    finis       = ZEPHYRUM;
    dum (   finis < id.mensura
         && !_est_spatium_interius(id.datum[finis]))
    {
        finis++;
    }
    id.mensura  = finis;
    *post       = I + finis;
    redde chorda_internare(ctx->intern, id);
}

interior StmlMacroArgumentum*
_argumentum_invenire (
        Xar* argumenta,
     chorda* titulus);

/* Argumenta vocationis parsare: paria 'nomen="valor"' post id
 * (citationes ambae; valor sine citationibus usque ad spatium -
 * lenitas attributorum). Verbum nudum sine '=' = argumentum quod
 * loculum nullum nominat -> ARGUMENTUM_SUPERFLUUM (clarum, non
 * tacite praeteritum). */
interior b32
_argumenta_parsare (
    StmlMacroContextus* ctx,
             StmlNodus* vocatio,   /* pro linea vitii solum */
                chorda* valor,     /* interior effectivus */
                   i32  ab,
                   Xar* argumenta)
{
    constans chorda* textus;
                i32  i;

    textus  = valor;
    i       = ab;
    dum (VERUM)
    {
        chorda titulus;
        chorda valor;
           i32 initium;

        dum (   i < textus->mensura
             && _est_spatium_interius(textus->datum[i]))
        {
            i++;
        }
        si (i >= textus->mensura)
        {
            frange;
        }
        initium = i;
        dum (   i < textus->mensura
             && textus->datum[i] != (i8)'='
             && !_est_spatium_interius(textus->datum[i]))
        {
            i++;
        }
        titulus.datum    = textus->datum + initium;
        titulus.mensura  = i - initium;
        si (i >= textus->mensura || textus->datum[i] != (i8)'=')
        {
            _vitium_ponere(ctx,
                           STML_EXPANSIO_ARGUMENTUM_SUPERFLUUM,
                           vocatio, NIHIL,
                           chorda_internare(ctx->intern, titulus));
            redde FALSUM;
        }
        i++;  /* '=' */
        si (   i < textus->mensura
            && (   textus->datum[i] == (i8)'"'
                || textus->datum[i] == (i8)'\''))
        {
            i8 citatio;

            citatio  = textus->datum[i];
            i++;
            initium  = i;
            dum (i < textus->mensura && textus->datum[i] != citatio)
            {
                i++;
            }
            valor.datum    = textus->datum + initium;
            valor.mensura  = i - initium;
            si (i < textus->mensura)
            {
                i++;  /* citatio claudens */
            }
        }
        alioquin
        {
            initium = i;
            dum (   i < textus->mensura
                 && !_est_spatium_interius(textus->datum[i]))
            {
                i++;
            }
            valor.datum    = textus->datum + initium;
            valor.mensura  = i - initium;
        }
        {
            StmlMacroArgumentum* arg;
                         chorda* titulus_internatus;

            titulus_internatus = chorda_internare(ctx->intern,
                                                  titulus);
            si (_argumentum_invenire(argumenta, titulus_internatus)
                    != NIHIL)
            {
                _vitium_ponere(ctx,
                               STML_EXPANSIO_ARGUMENTUM_GEMINUM,
                               vocatio, NIHIL, titulus_internatus);
                redde FALSUM;
            }
            arg = (StmlMacroArgumentum*)xar_addere(argumenta);
            si (arg == NIHIL)
            {
                redde FALSUM;
            }
            arg->titulus  = titulus_internatus;
            arg->valor    = chorda_internare(ctx->intern, valor);
            arg->arbores  = NIHIL;
        }
    }
    redde VERUM;
}

interior StmlMacroArgumentum*
_argumentum_invenire (
        Xar* argumenta,
     chorda* titulus)  /* internatum */
{
    i32 i;
    i32 num;

    num = xar_numerus(argumenta);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlMacroArgumentum* arg;

        arg = (StmlMacroArgumentum*)xar_obtinere(argumenta, i);
        si (arg != NIHIL && arg->titulus == titulus)
        {
            redde arg;
        }
    }
    redde NIHIL;
}

/* Chordam substituere: extensiones '&@nomen;' valoribus
 * argumentorum substitutae (grammatica eadem ac collectio -
 * nomina iam iudicata declarata et impleta). Sine extensione:
 * chorda originalis immutata redditur. 'nodus' pro linea vitii:
 * argumentum SUBARBOREUM in positione chordae = vitium septimum
 * (par. 6.1) - numquam chordificatio tacita; littera manet et
 * custos successus in fine expansionis cadere facit. */
interior chorda*
_chordam_substituere (
    StmlMacroContextus* ctx,
                chorda* textus,
                   Xar* argumenta,
             StmlNodus* nodus)
{
    ChordaAedificator* aed;
               chorda  titulus;
               chorda  fetta;
                  i32  ab;
                  i32  initium;
                  i32  post;

    ab = ZEPHYRUM;
    si (!_loculum_invenire(textus, ab, &initium, &post, &titulus))
    {
        redde textus;
    }
    aed = chorda_aedificator_creare(ctx->piscina,
                                    textus->mensura + XXXII);
    si (aed == NIHIL)
    {
        redde textus;
    }
    dum (_loculum_invenire(textus, ab, &initium, &post, &titulus))
    {
        StmlMacroArgumentum* arg;

        fetta.datum    = textus->datum + ab;
        fetta.mensura  = initium - ab;
        chorda_aedificator_appendere_chorda(aed, fetta);
        arg = _argumentum_invenire(
            argumenta, chorda_internare(ctx->intern, titulus));
        si (arg != NIHIL && arg->arbores != NIHIL)
        {
            _vitium_ponere(ctx, STML_EXPANSIO_ARGUMENTUM_ARBOREUM,
                           nodus, NIHIL, arg->titulus);
            fetta.datum    = textus->datum + initium;
            fetta.mensura  = post - initium;
            chorda_aedificator_appendere_chorda(aed, fetta);
        }
        alioquin si (arg != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed, *arg->valor);
        }
        alioquin
        {
            /* defensivum: numquam per constructionem (collectio
             * iudicavit) - extensio litteralis manet */
            fetta.datum    = textus->datum + initium;
            fetta.mensura  = post - initium;
            chorda_aedificator_appendere_chorda(aed, fetta);
        }
        ab = post;
    }
    fetta.datum    = textus->datum + ab;
    fetta.mensura  = textus->mensura - ab;
    chorda_aedificator_appendere_chorda(aed, fetta);
    redde chorda_internare(ctx->intern,
                           chorda_aedificator_finire(aed));
}


interior StmlNodus*
_expandere_nodum (
             StmlNodus* nodus,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum,
                   Xar* argumenta,
             StmlNodus* parens_novus);

interior b32
_liberos_expandere (
             StmlNodus* parens_novus,
                   Xar* fratres,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum,
                   Xar* argumenta);

/* EXEMPLARIA (gradus II) - impletiones post _liberos_expandere */
interior b32
_transparentiam_legere (
    StmlMacroContextus* ctx,
             StmlNodus* nodus);

interior b32
_exemplar_implere (
             StmlNodus* nodus,
    StmlMacroContextus* ctx);

interior b32
_per_implere (
             StmlNodus* parens_novus,
             StmlNodus* nodus,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum);

interior b32
_catena_implere (
             StmlNodus* nodus,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum);

interior b32
_liberum_expandere (
             StmlNodus* parens_novus,
                   Xar* fratres,
                   i32  index,
                   i32* saltus,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum,
                   Xar* argumenta);

/* Argumenta BLOCI colligere (par. 6.1): cursus maximalis
 * elementorum argumentorum statim post vocationem sequentium pars
 * VOCATIONIS est - hic in tabulam colligitur, in arbore expansa
 * numquam apparet (vocans 'saltus' fratres consumptos praeterit).
 * Classificatio:
 *   liberi nulli (sepulcrum '<@p=/>') -> absentia explicita:
 *     tabulae NON additur, NON_IMPLETUS postea clamat si loculus
 *     declaratus (verbum delendi, decretum par. 6.3);
 *   liberum unum TEXTUS -> scalaris (aequivalentia cum forma
 *     inscripta) - NISI textus TOTUS '&@x;' ad subarborem
 *     vocantis refert: tunc silva TRANSIT (transitio trans
 *     strata; classificatio post considerationem referentiae);
 *   ceteri -> subarboreus: liberi contextu VOCANTIS expansi
 *     (involucrum effimerum liberos colligit), silva in tabula.
 * Nomen bis datum (inscriptum + blocum aut bloca bina) =
 * ARGUMENTUM_GEMINUM - 'ultimus vincit' numquam fit. */
interior b32
_argumenta_bloci_colligere (
    StmlMacroContextus* ctx,
    StmlMacroDefinitio* def,
                   Xar* fratres,
                   i32  index,
                   i32* saltus,
                   Xar* argumenta,
                   Xar* argumenta_vocantis,
                   i32  stratum,
                   i32  tectum)
{
    i32 j;
    i32 num;

    si (fratres == NIHIL)
    {
        redde VERUM;
    }
    num  = xar_numerus(fratres);
    j    = index + I;
    dum (j < num)
    {
        StmlNodus* frater;
           chorda* titulus;
              i32  n_liberorum;

        frater = *(StmlNodus**)xar_obtinere(fratres, j);
        si (frater == NIHIL || !_est_argumentum_elementi(frater))
        {
            frange;
        }
        (*saltus)++;
        titulus = frater->attributum_titulus;
        si (_argumentum_invenire(argumenta, titulus) != NIHIL)
        {
            _vitium_ponere(ctx, STML_EXPANSIO_ARGUMENTUM_GEMINUM,
                           frater, def->id, titulus);
            redde FALSUM;
        }
        n_liberorum = frater->liberi != NIHIL
            ? xar_numerus(frater->liberi) : ZEPHYRUM;
        si (n_liberorum == ZEPHYRUM)
        {
            j++;
            perge;  /* sepulcrum: absentia explicita */
        }
        {
            StmlNodus* primum;

            primum = *(StmlNodus**)xar_obtinere(frater->liberi,
                                                ZEPHYRUM);
            si (   n_liberorum   == I
                && primum        != NIHIL
                && primum->genus == STML_NODUS_TEXTUS
                && primum->valor != NIHIL)
            {
                chorda referentia;
                   i32 initium;
                   i32 post;

                si (   argumenta_vocantis != NIHIL
                    && _loculum_invenire(primum->valor, ZEPHYRUM,
                                         &initium, &post,
                                         &referentia)
                    && initium            == ZEPHYRUM
                    && post               == primum->valor->mensura)
                {
                    StmlMacroArgumentum* arg_relatum;

                    arg_relatum = _argumentum_invenire(
                        argumenta_vocantis,
                        chorda_internare(ctx->intern, referentia));
                    si (   arg_relatum          != NIHIL
                        && arg_relatum->arbores != NIHIL)
                    {
                        StmlMacroArgumentum* arg;

                        arg = (StmlMacroArgumentum*)xar_addere(
                            argumenta);
                        si (arg == NIHIL)
                        {
                            redde FALSUM;
                        }
                        arg->titulus  = titulus;
                        arg->valor    = NIHIL;
                        /* fons immutabilis - splices clonant */
                        arg->arbores  = arg_relatum->arbores;
                        j++;
                        perge;
                    }
                }
                {
                    StmlMacroArgumentum* arg;
                                 chorda* valor;

                    valor = (   argumenta_vocantis != NIHIL
                             && xar_numerus(argumenta_vocantis)
                                    > ZEPHYRUM)
                        ? _chordam_substituere(ctx, primum->valor,
                                               argumenta_vocantis,
                                               primum)
                        : primum->valor;
                    arg = (StmlMacroArgumentum*)xar_addere(
                        argumenta);
                    si (arg == NIHIL)
                    {
                        redde FALSUM;
                    }
                    arg->titulus  = titulus;
                    arg->valor    = valor;
                    arg->arbores  = NIHIL;
                    j++;
                    perge;
                }
            }
        }
        {
                      StmlNodus* involucrum;
            StmlMacroArgumentum* arg;

            involucrum = stml_elementum_creare(ctx->piscina,
                                               ctx->intern, "arg");
            si (involucrum == NIHIL)
            {
                redde FALSUM;
            }
            si (!_liberos_expandere(involucrum, frater->liberi,
                                    ctx, stratum, tectum,
                                    argumenta_vocantis))
            {
                redde FALSUM;
            }
            arg = (StmlMacroArgumentum*)xar_addere(argumenta);
            si (arg == NIHIL)
            {
                redde FALSUM;
            }
            arg->titulus  = titulus;
            arg->valor    = NIHIL;
            arg->arbores  = involucrum->liberi;
            j++;
        }
    }
    redde VERUM;
}

/* Vocationem implere: corpus definitionis per AMBULATIONEM in
 * parentem splicare (copia caeca vocationes interiores verbatim
 * ferret - ambulatio eas expandit), notam registrare.
 * 'valor_effectivus' = interior iam substitutus (transitio
 * argumentorum trans strata - grammatica una, quod collectio
 * iudicat impletio substituit); 'tectum' = resolutio solum ad
 * definitiones ordine < tectum (documenti: numerus totus,
 * praeterita discernit; interiores: ordo definitionis continentis
 * - tectum stricte decrescens, terminatio per constructionem).
 * Nota.nodus = clonis primus splicis, per indicem parentis captus
 * (vocatio interior liberos plures DIRECTE addit). */
interior b32
_vocationem_implere (
             StmlNodus* parens_novus,
             StmlNodus* vocatio,
                chorda* valor_effectivus,
                   Xar* fratres,
                   i32  index,
                   i32* saltus,
                   i32  stratum,
                   i32  tectum,
    StmlMacroContextus* ctx,
                   Xar* argumenta_vocantis)
{
                chorda* id;
    StmlMacroDefinitio* def;
      StmlExpansioNota* nota;
                   Xar* argumenta;
                   i32  post_id;
                   i32  ante_numerus;
                   i32  i;
                   i32  num;

    id   = _vocationis_id(ctx, valor_effectivus, &post_id);
    def  = _definitionem_invenire(ctx, id);
    si (def == NIHIL)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_FRAGMENTUM_IGNOTUM,
                       vocatio, id, NIHIL);
        redde FALSUM;
    }
    si (def->ordo >= tectum || !def->praeterita)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_FRAGMENTUM_POSTERIUS,
                       vocatio, id, NIHIL);
        redde FALSUM;
    }

    /* argumenta parsare et utroque modo iudicare: quisque
     * argumentum loculum declaratum nominat (SUPERFLUUM), quisque
     * loculus declaratus impletur (NON_IMPLETUS) */
    argumenta = xar_creare(ctx->piscina,
                           magnitudo(StmlMacroArgumentum));
    si (argumenta == NIHIL)
    {
        redde FALSUM;
    }
    si (!_argumenta_parsare(ctx, vocatio, valor_effectivus, post_id,
                            argumenta))
    {
        si (ctx->resultus->vitium != STML_EXPANSIO_BENE)
        {
            ctx->resultus->fragmentum = *def->id;
        }
        redde FALSUM;
    }
    /* argumenta bloci ANTE iudicia colliguntur (tabula completa
     * iudicatur: superfluum + non-impletus formas ambas vident) */
    si (!_argumenta_bloci_colligere(ctx, def, fratres, index,
                                    saltus, argumenta,
                                    argumenta_vocantis, stratum,
                                    tectum))
    {
        redde FALSUM;
    }
    num = xar_numerus(argumenta);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlMacroArgumentum* arg;

        arg = (StmlMacroArgumentum*)xar_obtinere(argumenta, i);
        si (   arg                                    != NIHIL
            && _loculus_declaratus(def, arg->titulus) == NIHIL)
        {
            _vitium_ponere(ctx,
                           STML_EXPANSIO_ARGUMENTUM_SUPERFLUUM,
                           vocatio, def->id, arg->titulus);
            redde FALSUM;
        }
    }
    num = xar_numerus(def->loculi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlMacroLoculus* loc;

        loc = (StmlMacroLoculus*)xar_obtinere(def->loculi, i);
        si (   loc != NIHIL
            && !loc->optionalis
            && _argumentum_invenire(argumenta, loc->titulus)
                   == NIHIL)
        {
            _vitium_ponere(ctx,
                           STML_EXPANSIO_LOCULUS_NON_IMPLETUS,
                           vocatio, def->id, loc->titulus);
            redde FALSUM;
        }
    }

    /* nota ANTE impletionem appensa (spec: tabula ordine splicis -
     * ordo vocationum, non perfectionis); punctatores cellularum
     * Xar stabiles trans appensiones, ergo nota trans impletionem
     * tenetur et nodus post impletur */
    nota = (StmlExpansioNota*)xar_addere(
        ctx->resultus->tabula_expansionum);
    si (nota == NIHIL)
    {
        redde FALSUM;
    }
    nota->nodus          = NIHIL;
    nota->fragmentum_id  = def->id;
    nota->vocatio        = vocatio;
    nota->stratum        = stratum + I;

    ante_numerus = parens_novus->liberi != NIHIL
        ? xar_numerus(parens_novus->liberi) : ZEPHYRUM;
    si (!_liberos_expandere(parens_novus, def->definitio->liberi,
                            ctx, stratum + I, def->ordo,
                            argumenta))
    {
        redde FALSUM;
    }
    si (   parens_novus->liberi != NIHIL
        && xar_numerus(parens_novus->liberi) > ante_numerus)
    {
        nota->nodus = stml_liberum_ad_indicem(parens_novus,
                                              ante_numerus);
    }
    redde VERUM;
}

/* Ambulatio expansionis: superficialiter duplicare, substitutio
 * inline (in impletionibus: valores textus/transclusionis/
 * attributorum), liberos per _liberum_expandere. 'argumenta' NIHIL
 * = ambulatio documenti; non-NIHIL (etiam vacua) = intra
 * impletionem. Cave: stml_duplicare_superficialiter liberos NIHIL
 * relinquit - Xar liberorum hic creatur. */
interior StmlNodus*
_expandere_nodum (
             StmlNodus* nodus,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum,
                   Xar* argumenta,
             StmlNodus* parens_novus)
{
    StmlNodus* novum;
          i32  i;
          i32  num;

    novum = stml_duplicare_superficialiter(nodus, ctx->piscina,
                                           ctx->intern);
    si (novum == NIHIL)
    {
        redde NIHIL;
    }
    /* nodus primus creatus = radix arboris expansae - scopus
     * exemplarium defaltus (arbor partialis = contentum supra) */
    si (ctx->radix_expansa == NIHIL)
    {
        ctx->radix_expansa = novum;
    }
    /* ADNEXIO PRAECOX: nodus VACUUS parenti statim adnectitur,
     * liberi postea implentur - spina supra punctum currens
     * semper COMPLETA est, ergo arbor partialis vere est
     * 'contentum supra' (scopus exemplarium). Arbor finalis
     * eadem; visibilitas media sola mutatur. */
    si (parens_novus != NIHIL)
    {
        (vacuum)stml_liberum_addere(parens_novus, novum);
    }
    si (argumenta != NIHIL && xar_numerus(argumenta) > ZEPHYRUM)
    {
        si (   (   novum->genus == STML_NODUS_TEXTUS
                || novum->genus == STML_NODUS_TRANSCLUSIO)
            && novum->valor != NIHIL)
        {
            novum->valor = _chordam_substituere(ctx, novum->valor,
                                                argumenta, novum);
        }
        si (novum->attributa != NIHIL)
        {
            num = xar_numerus(novum->attributa);
            per (i = ZEPHYRUM; i < num; i++)
            {
                StmlAttributum* attr;

                attr = (StmlAttributum*)xar_obtinere(
                    novum->attributa, i);
                si (attr != NIHIL && attr->valor != NIHIL)
                {
                    attr->valor = _chordam_substituere(
                        ctx, attr->valor, argumenta, novum);
                }
            }
        }
    }
    si (nodus->liberi != NIHIL)
    {
        novum->liberi = xar_creare(ctx->piscina,
                                   magnitudo(StmlNodus*));
        si (novum->liberi == NIHIL)
        {
            redde NIHIL;
        }
        si (!_liberos_expandere(novum, nodus->liberi, ctx, stratum,
                                tectum, argumenta))
        {
            redde NIHIL;
        }
    }
    redde novum;
}

/* COMMUTATIONEM implere (par. 6.2): bracchium primum congruens
 * eligere, liberos eius in parentem splicare - nodi COMMUTATIO et
 * CASUS ipsi numquam in arborem expansam veniunt (sicut elementa
 * argumentorum: machina structurae, non contentum). Congruentia:
 * loculus de absens -> nihil; praesens scalaris -> est (aequalitas
 * octetim), deinde non-nihil; praesens SUBARBOREUS -> bracchium
 * est attingi non potest (comparatio silvae cum littera = vitium
 * septimum), non-nihil congruit. ORDINARIUS semper congruit
 * (collectio ultimum probavit). Nullum congruens = CASUS_NULLUS -
 * numquam vacuum tacitum. Forma iam collectione probata -
 * defensiva hic repetita ne arbor manu structa tacite transiret. */
interior b32
_commutationem_implere (
             StmlNodus* parens_novus,
             StmlNodus* com,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum,
                   Xar* argumenta)
{
                 chorda* de;
                 chorda* referentia;
    StmlMacroArgumentum* arg;
                    b32  praesens;
                    b32  scalaris;
                    i32  i;
                    i32  num;

    de          = stml_attributum_capere(com, "de");
    referentia  = de != NIHIL ? _referentia_tota(ctx, de) : NIHIL;
    si (referentia == NIHIL)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_COMMUTATIO_MALFORMATA,
                       com, NIHIL, NIHIL);
        redde FALSUM;
    }
    arg       = _argumentum_invenire(argumenta, referentia);
    praesens  = arg != NIHIL;
    scalaris  = praesens && arg->valor != NIHIL;

    num = com->liberi != NIHIL
        ? xar_numerus(com->liberi) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* bracchium;
              b32  congruit;

        bracchium = *(StmlNodus**)xar_obtinere(com->liberi, i);
        si (   bracchium        == NIHIL
            || bracchium->genus == STML_NODUS_COMMENTUM)
        {
            perge;
        }
        congruit = FALSUM;
        si (_est_titulo(bracchium, "ORDINARIUS"))
        {
            congruit = VERUM;
        }
        alioquin si (_est_titulo(bracchium, "CASUS"))
        {
            chorda* est;

            est = stml_attributum_capere(bracchium, "est");
            si (est != NIHIL)
            {
                si (praesens && !scalaris)
                {
                    _vitium_ponere(ctx,
                        STML_EXPANSIO_ARGUMENTUM_ARBOREUM,
                        bracchium, NIHIL, referentia);
                    redde FALSUM;
                }
                congruit = scalaris
                    && chorda_aequalis(*arg->valor, *est);
            }
            alioquin si (stml_attributum_capere(bracchium,
                             "non-nihil") != NIHIL)
            {
                congruit = praesens;
            }
            alioquin si (stml_attributum_capere(bracchium,
                             "nihil") != NIHIL)
            {
                congruit = (b32)!praesens;
            }
        }
        si (congruit)
        {
            redde _liberos_expandere(parens_novus,
                                     bracchium->liberi, ctx,
                                     stratum, tectum, argumenta);
        }
    }
    _vitium_ponere(ctx, STML_EXPANSIO_CASUS_NULLUS, com, NIHIL,
                   referentia);
    redde FALSUM;
}

/* Liberum unum expandere - interceptio COMMUNIS ambulationis
 * documenti et impletionis corporum (vocatio liberos plures parit
 * et fratres sequentes CONSUMIT, ergo in ansa liberorum vivit,
 * non in casu nodi; 'saltus' = fratres consumpti quos vocans
 * praeterit):
 * - definitio: ambulatione documenti demissa + praeterita notata;
 *   in impletione materia citata OPACA, verbatim clonata;
 * - vocatio: interior effectivus (substitutus in impletione -
 *   transitio argumentorum), deinde impleta cum argumentis bloci
 *   ex fratribus;
 * - textus TOTUS '&@x;' ad subarborem (in impletione): SPLEX
 *   SILVAE - clones recentes quaque sede (instantiatio);
 * - cetera: recursio ambulationis. */
interior b32
_liberum_expandere (
             StmlNodus* parens_novus,
                   Xar* fratres,
                   i32  index,
                   i32* saltus,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum,
                   Xar* argumenta)
{
    StmlNodus* liberum;
    StmlNodus* liberum_novum;

    *saltus = ZEPHYRUM;
    liberum = *(StmlNodus**)xar_obtinere(fratres, index);
    si (liberum == NIHIL)
    {
        redde VERUM;
    }
    si (_est_definitio(liberum))
    {
        si (argumenta == NIHIL)
        {
            StmlMacroDefinitio* def;

            def = _definitionem_invenire(ctx,
                                         liberum->fragmentum_id);
            si (def != NIHIL)
            {
                def->praeterita = VERUM;
            }
            redde VERUM;  /* demissa - visio contenti */
        }
        liberum_novum = stml_duplicare(liberum, ctx->piscina,
                                       ctx->intern);
        si (liberum_novum == NIHIL)
        {
            redde FALSUM;
        }
        (vacuum)stml_liberum_addere(parens_novus, liberum_novum);
        redde VERUM;
    }
    si (_est_vocatio(liberum))
    {
        chorda* valor_effectivus;

        valor_effectivus =
            (argumenta != NIHIL && xar_numerus(argumenta) > ZEPHYRUM)
                ? _chordam_substituere(ctx, liberum->valor,
                                       argumenta, liberum)
                : liberum->valor;
        redde _vocationem_implere(parens_novus, liberum,
                                  valor_effectivus, fratres, index,
                                  saltus, stratum, tectum, ctx,
                                  argumenta);
    }
    si (argumenta != NIHIL && _est_titulo(liberum, "COMMUTATIO"))
    {
        /* in impletione sola interpretata - gradu documenti
         * intacta transit (regula eadem ac '&@x;' littera extra
         * corpora manens) */
        redde _commutationem_implere(parens_novus, liberum, ctx,
                                     stratum, tectum, argumenta);
    }
    /* EXEMPLARIA (gradus II): gradu DOCUMENTI interpretata -
     * consumpta, numquam emissa (invariatum spec par. 0). Intra
     * corpora templorum = porta 'de=@arg' nondum aedificata -
     * CLARE, non tacite (spec par. 8.5). Sub PORTATIONE
     * (applicatio > 0, instantiatio nexus catenae) INERTA: cadunt
     * ad expansionem planam infra - ut datum emittuntur, semantica
     * instantiationis in corporibus (refinamentum vitii XIII). */
    si (   _est_titulo(liberum, "EXEMPLAR")
        || _est_titulo(liberum, "PER")
        || _est_titulo(liberum, "TRANSPARENTIA")
        || _est_titulo(liberum, "CATENA"))
    {
        si (ctx->applicatio == ZEPHYRUM)
        {
            si (argumenta != NIHIL)
            {
                _vitium_ponere(ctx,
                               STML_EXPANSIO_EXEMPLAR_MALFORMATUM,
                               liberum, NIHIL, NIHIL);
                redde FALSUM;
            }
            si (_est_titulo(liberum, "EXEMPLAR"))
            {
                redde _exemplar_implere(liberum, ctx);
            }
            si (_est_titulo(liberum, "PER"))
            {
                redde _per_implere(parens_novus, liberum, ctx,
                                   stratum, tectum);
            }
            si (_est_titulo(liberum, "CATENA"))
            {
                redde _catena_implere(liberum, ctx, stratum,
                                      tectum);
            }
            redde _transparentiam_legere(ctx, liberum);
        }
    }
    si (   argumenta      != NIHIL
        && liberum->genus == STML_NODUS_TEXTUS
        && liberum->valor != NIHIL)
    {
        chorda referentia;
           i32 initium;
           i32 post;

        si (   _loculum_invenire(liberum->valor, ZEPHYRUM,
                                 &initium, &post, &referentia)
            && initium == ZEPHYRUM
            && post    == liberum->valor->mensura)
        {
            StmlMacroArgumentum* arg;

            arg = _argumentum_invenire(
                argumenta,
                chorda_internare(ctx->intern, referentia));
            si (arg != NIHIL && arg->arbores != NIHIL)
            {
                i32 k;
                i32 num_arborum;

                /* custos: parens elementum attributi liberos
                 * TEXTUS solos fert (par. 6.3) - silva ibi vitium
                 * septimum est, non splex */
                si (parens_novus->attributum_titulus != NIHIL)
                {
                    _vitium_ponere(
                        ctx, STML_EXPANSIO_ARGUMENTUM_ARBOREUM,
                        liberum, NIHIL, arg->titulus);
                    redde FALSUM;
                }
                num_arborum = xar_numerus(arg->arbores);
                per (k = ZEPHYRUM; k < num_arborum; k++)
                {
                    StmlNodus* fons_arboris;
                    StmlNodus* clon;

                    fons_arboris = *(StmlNodus**)xar_obtinere(
                        arg->arbores, k);
                    si (fons_arboris == NIHIL)
                    {
                        perge;
                    }
                    clon = stml_duplicare(fons_arboris,
                                          ctx->piscina,
                                          ctx->intern);
                    si (clon == NIHIL)
                    {
                        redde FALSUM;
                    }
                    (vacuum)stml_liberum_addere(parens_novus,
                                                clon);
                }
                redde VERUM;
            }
        }
    }
    liberum_novum = _expandere_nodum(liberum, ctx, stratum, tectum,
                                     argumenta, parens_novus);
    si (liberum_novum == NIHIL)
    {
        redde FALSUM;
    }
    redde VERUM;  /* iam adnexum (adnexio praecox) */
}

/* Ansa liberorum communis: quisque liberum per _liberum_expandere,
 * fratres consumptos (argumenta bloci vocationum) praeteriens. */
interior b32
_liberos_expandere (
             StmlNodus* parens_novus,
                   Xar* fratres,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum,
                   Xar* argumenta)
{
    i32 i;
    i32 num;
    i32 saltus;

    si (fratres == NIHIL)
    {
        redde VERUM;
    }
    num = xar_numerus(fratres);
    per (i = ZEPHYRUM; i < num; i++)
    {
        saltus = ZEPHYRUM;
        si (!_liberum_expandere(parens_novus, fratres, i, &saltus,
                                ctx, stratum, tectum, argumenta))
        {
            redde FALSUM;
        }
        i += saltus;
    }
    redde VERUM;
}


/* ==================================================
 * EXEMPLARIA (spec stml-exemplaria-spec.md par. 2-4, gradus II)
 *
 * Congruentia LAXA: scriptum = requisitum, omissum = liberum;
 * liberi = subsequentia ordinata (avara, cum saltibus); '$x'
 * attributum booleanum = captura nodi, valor '$x' = captura
 * valoris, textus totus '$x' = captura textus; '<*>' elementum
 * quodlibet; nomen iteratum = aequalitas (regula V). Applicatio
 * FLUITANS defalta (temptatio ad omnem radicem subarboris),
 * 'ancorata' = radix scopi sola. PONS = scopus: capturae ut
 * tabula argumentorum adveniunt - PER easdem machinas impletionis
 * adhibet quas vocatio.
 *
 * VISIO CONTENTI in candidatis: fragmenta contenti DISSOLVUNTUR
 * (liberi eorum in sede), transclusiones PRAETERITAE (identitas
 * semel numerata - definitio numerat), elementa attributorum
 * praeterita, tagi perspicui (TRANSPARENTIA) invisibiles, spatium
 * templi '@' opacum. Regulae probatione Q3 probatae (233/233).
 * ================================================== */

interior b32
_est_perspicuum (
    StmlMacroContextus* ctx,
       constans chorda* titulus)
{
    i32 i;

    si (ctx->transparentia == NIHIL || titulus == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(ctx->transparentia); i++)
    {
        /* internati: aequalitas punctatoris */
        si (*(chorda**)xar_obtinere(ctx->transparentia, i)
                == titulus)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_transparentiam_legere (
    StmlMacroContextus* ctx,
             StmlNodus* nodus)
{
    chorda* tags;
       i32  i;
       i32  additi;

    tags = stml_attributum_capere(nodus, "tags");
    si (tags == NIHIL || tags->mensura == ZEPHYRUM)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_TRANSPARENTIA_MALFORMATA,
                       nodus, NIHIL, NIHIL);
        redde FALSUM;
    }
    si (ctx->transparentia == NIHIL)
    {
        ctx->transparentia = xar_creare(ctx->piscina,
                                        magnitudo(chorda*));
        si (ctx->transparentia == NIHIL)
        {
            redde FALSUM;
        }
    }
    additi  = ZEPHYRUM;
    i       = ZEPHYRUM;
    dum (i < tags->mensura)
    {
        i32 initium;

        dum (   i < tags->mensura
             && (   tags->datum[i] == (i8)' '
                 || tags->datum[i] == (i8)'\t'
                 || tags->datum[i] == (i8)'\n'
                 || tags->datum[i] == (i8)'\r'))
        {
            i++;
        }
        initium = i;
        dum (   i < tags->mensura
             && tags->datum[i] != (i8)' '
             && tags->datum[i] != (i8)'\t'
             && tags->datum[i] != (i8)'\n'
             && tags->datum[i] != (i8)'\r')
        {
            i++;
        }
        si (i > initium)
        {
            chorda   pars;
            chorda*  internatum;
            chorda** cella;

            pars.datum    = tags->datum + initium;
            pars.mensura  = i - initium;
            internatum    = chorda_internare(ctx->intern, pars);
            cella         = (chorda**)xar_addere(ctx->transparentia);
            si (internatum == NIHIL || cella == NIHIL)
            {
                redde FALSUM;
            }
            *cella = internatum;
            additi++;
        }
    }
    si (additi == ZEPHYRUM)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_TRANSPARENTIA_MALFORMATA,
                       nodus, NIHIL, NIHIL);
        redde FALSUM;
    }
    redde VERUM;
}

/* Liberi EFFECTIVI candidati (visio contenti - vide caput
 * sectionis). Fragmenta contenti recursione dissolvuntur. */
interior b32
_liberi_effectivi (
    StmlMacroContextus* ctx,
    constans StmlNodus* nodus,
                   Xar* exitus)
{
    i32 i;
    i32 num;

    si (nodus->liberi == NIHIL)
    {
        redde VERUM;
    }
    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* l =
            *(StmlNodus**)xar_obtinere(nodus->liberi, i);
        StmlNodus** cella;

        si (   l        == NIHIL
            || l->genus == STML_NODUS_COMMENTUM
            || l->genus == STML_NODUS_TRANSCLUSIO)
        {
            perge;
        }
        si (l->genus == STML_NODUS_ELEMENTUM)
        {
            si (l->attributum_titulus != NIHIL)
            {
                perge;
            }
            si (l->fragmentum)
            {
                si (   l->fragmentum_id                  != NIHIL
                    && l->fragmentum_id->mensura > ZEPHYRUM
                    && l->fragmentum_id->datum[ZEPHYRUM] == (i8)'@')
                {
                    perge;  /* spatium templi opacum */
                }
                si (!_liberi_effectivi(ctx, l, exitus))
                {
                    redde FALSUM;
                }
                perge;  /* dissolutum - liberi eius iam additi */
            }
            si (_est_perspicuum(ctx, l->titulus))
            {
                perge;
            }
        }
        cella = (StmlNodus**)xar_addere(exitus);
        si (cella == NIHIL)
        {
            redde FALSUM;
        }
        *cella = l;
    }
    redde VERUM;
}

/* Attributum inscriptum per titulum (internati - punctator) */
interior StmlAttributum*
_exemplar_attributum (
    constans StmlNodus* nodus,
    constans    chorda* titulus)
{
    i32 i;
    i32 num;

    si (nodus->attributa == NIHIL)
    {
        redde NIHIL;
    }
    num = xar_numerus(nodus->attributa);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlAttributum* attr =
            (StmlAttributum*)xar_obtinere(nodus->attributa, i);

        si (attr != NIHIL && attr->titulus == titulus)
        {
            redde attr;
        }
    }
    redde NIHIL;
}

/* Ligamen ponere; nomen ITERATUM = aequalitas (regula V), numquam
 * re-ligatio. Gradus mixti (nodus contra valorem) = discrepantia. */
interior b32
_ligamen_ponere (
    StmlMacroContextus* ctx,
                   Xar* ligamina,
                chorda* titulus,
             StmlNodus* nodus,
                chorda* valor)
{
    StmlExemplarLigamen* lig;
                    i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(ligamina); i++)
    {
        lig = (StmlExemplarLigamen*)xar_obtinere(ligamina, i);
        si (lig == NIHIL || lig->titulus != titulus)
        {
            perge;
        }
        si (nodus != NIHIL && lig->nodus != NIHIL)
        {
            chorda sa = stml_scribere(lig->nodus, ctx->piscina,
                                      FALSUM);
            chorda sb = stml_scribere(nodus, ctx->piscina, FALSUM);

            redde chorda_aequalis(sa, sb);
        }
        si (valor != NIHIL && lig->valor != NIHIL)
        {
            redde valor == lig->valor;  /* internati */
        }
        redde FALSUM;  /* gradus mixti */
    }
    lig = (StmlExemplarLigamen*)xar_addere(ligamina);
    si (lig == NIHIL)
    {
        redde FALSUM;
    }
    lig->titulus  = titulus;
    lig->nodus    = nodus;
    lig->valor    = valor;
    redde VERUM;
}

interior b32
_laxa_congruere (
    StmlMacroContextus* ctx,
             StmlNodus* forma,
             StmlNodus* candidatus,
                   Xar* ligamina);

/* Liberi formae contra liberos effectivos candidati: subsequentia
 * ordinata AVARA - quodque liberum formae candidatum congruentem
 * PRIMUM consumit; saltus liberi. Temptationes cadentes ligamina
 * sua truncant (matcher numquam retro tollit - vocans truncat). */
interior b32
_laxa_liberos_congruere (
    StmlMacroContextus* ctx,
             StmlNodus* forma,
             StmlNodus* candidatus,
                   Xar* ligamina)
{
    Xar* effectivi;
    i32  pi;
    i32  ci;
    i32  pnum;
    i32  cnum;

    si (   forma->liberi              == NIHIL
        || xar_numerus(forma->liberi) == ZEPHYRUM)
    {
        redde VERUM;  /* nihil scriptum = nihil requisitum */
    }
    effectivi = xar_creare(ctx->piscina, magnitudo(StmlNodus*));
    si (   effectivi == NIHIL
        || !_liberi_effectivi(ctx, candidatus, effectivi))
    {
        redde FALSUM;
    }
    pnum  = xar_numerus(forma->liberi);
    cnum  = xar_numerus(effectivi);
    ci    = ZEPHYRUM;
    per (pi = ZEPHYRUM; pi < pnum; pi++)
    {
        StmlNodus* pf =
            *(StmlNodus**)xar_obtinere(forma->liberi, pi);
              b32 congruit;

        si (pf == NIHIL || pf->genus == STML_NODUS_COMMENTUM)
        {
            perge;
        }
        congruit = FALSUM;
        dum (ci < cnum)
        {
            StmlNodus* cc =
                *(StmlNodus**)xar_obtinere(effectivi, ci);
                  i32 ante = xar_numerus(ligamina);

            ci++;
            si (_laxa_congruere(ctx, pf, cc, ligamina))
            {
                congruit = VERUM;
                frange;
            }
            xar_truncare(ligamina, ante);
        }
        si (!congruit)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Congruentia laxa nodi unius (vide caput sectionis) */
interior b32
_laxa_congruere (
    StmlMacroContextus* ctx,
             StmlNodus* forma,
             StmlNodus* candidatus,
                   Xar* ligamina)
{
    si (forma->genus == STML_NODUS_TEXTUS)
    {
        si (   candidatus->genus != STML_NODUS_TEXTUS
            || forma->valor      == NIHIL
            || candidatus->valor == NIHIL)
        {
            redde FALSUM;
        }
        /* '$x' totus = captura textus (scalaris) */
        si (   forma->valor->mensura > I
            && forma->valor->datum[ZEPHYRUM] == (i8)'$')
        {
            chorda  pars;
            chorda* titulus_capturae;

            pars.datum        = forma->valor->datum + I;
            pars.mensura      = forma->valor->mensura - I;
            titulus_capturae  = chorda_internare(ctx->intern, pars);
            si (titulus_capturae == NIHIL)
            {
                redde FALSUM;
            }
            redde _ligamen_ponere(ctx, ligamina, titulus_capturae,
                                  NIHIL, candidatus->valor);
        }
        redde chorda_aequalis(*forma->valor, *candidatus->valor);
    }
    si (forma->genus == STML_NODUS_TRANSCLUSIO)
    {
        redde candidatus->genus == STML_NODUS_TRANSCLUSIO
            && forma->valor != NIHIL
            && candidatus->valor != NIHIL
            && chorda_aequalis(*forma->valor, *candidatus->valor);
    }
    si (   forma->genus      != STML_NODUS_ELEMENTUM
        || candidatus->genus != STML_NODUS_ELEMENTUM)
    {
        redde FALSUM;
    }
    si (   candidatus->fragmentum
        || candidatus->attributum_titulus != NIHIL
        || forma->titulus                 == NIHIL
        || candidatus->titulus            == NIHIL)
    {
        redde FALSUM;
    }
    /* tag: '<*>' = elementum quodlibet; alioquin punctator (internum
     * unum per catenam - arbor formae et candidati idem internum) */
    si (   !chorda_aequalis_literis(*forma->titulus, "*")
        && forma->titulus != candidatus->titulus)
    {
        redde FALSUM;
    }
    /* attributa formae: scripta = requisita, omissa = libera */
    si (forma->attributa != NIHIL)
    {
        i32 i;
        i32 num = xar_numerus(forma->attributa);

        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlAttributum* fa =
                (StmlAttributum*)xar_obtinere(forma->attributa, i);
            StmlAttributum* ca;

            si (fa == NIHIL || fa->titulus == NIHIL)
            {
                perge;
            }
            /* '$x' booleanum = captura NODI */
            si (   fa->titulus->mensura > I
                && fa->titulus->datum[ZEPHYRUM] == (i8)'$')
            {
                chorda  pars;
                chorda* titulus_capturae;

                pars.datum    = fa->titulus->datum + I;
                pars.mensura  = fa->titulus->mensura - I;
                titulus_capturae = chorda_internare(ctx->intern,
                                                    pars);
                si (   titulus_capturae == NIHIL
                    || !_ligamen_ponere(ctx, ligamina,
                            titulus_capturae, candidatus, NIHIL))
                {
                    redde FALSUM;
                }
                perge;
            }
            /* valor '$x' = captura VALORIS attributi */
            si (   fa->valor                  != NIHIL
                && fa->valor->mensura > I
                && fa->valor->datum[ZEPHYRUM] == (i8)'$')
            {
                chorda  pars;
                chorda* titulus_capturae;

                ca = _exemplar_attributum(candidatus, fa->titulus);
                si (ca == NIHIL || ca->valor == NIHIL)
                {
                    redde FALSUM;  /* scriptum = requisitum */
                }
                pars.datum    = fa->valor->datum + I;
                pars.mensura  = fa->valor->mensura - I;
                titulus_capturae = chorda_internare(ctx->intern,
                                                    pars);
                si (   titulus_capturae == NIHIL
                    || !_ligamen_ponere(ctx, ligamina,
                            titulus_capturae, NIHIL, ca->valor))
                {
                    redde FALSUM;
                }
                perge;
            }
            /* littera: adsit et aequet */
            ca = _exemplar_attributum(candidatus, fa->titulus);
            si (ca == NIHIL)
            {
                redde FALSUM;
            }
            si ((fa->valor != NIHIL) != (ca->valor != NIHIL))
            {
                redde FALSUM;
            }
            si (   fa->valor != NIHIL
                && !chorda_aequalis(*fa->valor, *ca->valor))
            {
                redde FALSUM;
            }
        }
    }
    redde _laxa_liberos_congruere(ctx, forma, candidatus, ligamina);
}

/* Applicatio: temptatio ad hunc nodum, deinde (fluitans) descensus
 * in subarbores contenti. opus_ligamina = Xar laboris (truncatur
 * ante quamque temptationem); successu ligamina in copiam NOVAM
 * copiantur. FALSUM = defectus memoriae solum. */
interior b32
_exemplar_petere (
    StmlMacroContextus* ctx,
             StmlNodus* forma,
             StmlNodus* nodus,
                   b32  ancorata,
                   Xar* congruentiae,
                   Xar* opus_ligamina)
{
    i32 i;
    i32 num;

    si (nodus == NIHIL)
    {
        redde VERUM;
    }
    si (   nodus->genus              == STML_NODUS_ELEMENTUM
        && !nodus->fragmentum
        && nodus->attributum_titulus == NIHIL
        && !_est_perspicuum(ctx, nodus->titulus))
    {
        xar_truncare(opus_ligamina, ZEPHYRUM);
        si (_laxa_congruere(ctx, forma, nodus, opus_ligamina))
        {
            StmlExemplarCongruentia* con;
                                Xar* ligamina;

            ligamina = xar_creare(ctx->piscina,
                                  magnitudo(StmlExemplarLigamen));
            si (ligamina == NIHIL)
            {
                redde FALSUM;
            }
            num = xar_numerus(opus_ligamina);
            per (i = ZEPHYRUM; i < num; i++)
            {
                StmlExemplarLigamen* cella =
                    (StmlExemplarLigamen*)xar_addere(ligamina);

                si (cella == NIHIL)
                {
                    redde FALSUM;
                }
                *cella = *(StmlExemplarLigamen*)xar_obtinere(
                    opus_ligamina, i);
            }
            con = (StmlExemplarCongruentia*)xar_addere(
                congruentiae);
            si (con == NIHIL)
            {
                redde FALSUM;
            }
            con->radix     = nodus;
            con->ligamina  = ligamina;
        }
    }
    /* ancorata: radix scopi sola - descensus solum dum nodus
     * elementum nondum est (involucrum documenti) */
    si (ancorata && nodus->genus == STML_NODUS_ELEMENTUM)
    {
        redde VERUM;
    }
    si (nodus->liberi == NIHIL)
    {
        redde VERUM;
    }
    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* l =
            *(StmlNodus**)xar_obtinere(nodus->liberi, i);

        si (   l        == NIHIL
            || l->genus == STML_NODUS_COMMENTUM
            || l->genus == STML_NODUS_TRANSCLUSIO)
        {
            perge;
        }
        si (l->genus == STML_NODUS_ELEMENTUM)
        {
            si (l->attributum_titulus != NIHIL)
            {
                perge;
            }
            si (   l->fragmentum
                && l->fragmentum_id                  != NIHIL
                && l->fragmentum_id->mensura > ZEPHYRUM
                && l->fragmentum_id->datum[ZEPHYRUM] == (i8)'@')
            {
                perge;  /* spatium templi opacum */
            }
            si (_est_perspicuum(ctx, l->titulus))
            {
                perge;
            }
        }
        si (!_exemplar_petere(ctx, forma, l, ancorata, congruentiae,
                              opus_ligamina))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior StmlExemplarRelatio*
_relationem_invenire (
    StmlMacroContextus* ctx,
                chorda* titulus)
{
    i32 i;

    si (ctx->relationes == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(ctx->relationes); i++)
    {
        StmlExemplarRelatio* rel =
            (StmlExemplarRelatio*)xar_obtinere(ctx->relationes, i);

        si (rel != NIHIL && rel->titulus == titulus)
        {
            redde rel;
        }
    }
    redde NIHIL;
}

/* Nucleus gradus exemplaris: modus/ancorata/forma legere,
 * congruentias super fontem colligere, modum applicare. Communis
 * inter EXEMPLAR nominatum (fons = de= resolutum aut NIHIL = radix
 * expansa) et gradus CATENAE (fons = relatio gradus prioris
 * filata). 'titulus' nuntiis vitiorum solis servit. */
interior b32
_exemplar_nucleus (
    StmlMacroContextus*  ctx,
             StmlNodus*  nodus,
                   Xar*  fons,
                chorda*  titulus,
                   Xar** exitus)
{
                    chorda* modus;
                 StmlNodus* forma;
                       Xar* congruentiae;
                       Xar* opus;
                       b32  ancorata;
                       i32  i;
                       i32  num;

    *exitus  = NIHIL;
    modus    = stml_attributum_capere(nodus, "modus");
    si (   modus != NIHIL
        && !chorda_aequalis_literis(*modus, "omnia")
        && !chorda_aequalis_literis(*modus, "unum")
        && !chorda_aequalis_literis(*modus, "primum")
        && !chorda_aequalis_literis(*modus, "optional"))
    {
        _vitium_ponere(ctx, STML_EXPANSIO_EXEMPLAR_MALFORMATUM,
                       nodus, NIHIL, modus);
        redde FALSUM;
    }
    ancorata = stml_attributum_capere(nodus, "ancorata") != NIHIL;

    /* corpus: elementum UNUM (spec par. 2.4 - corpus silvestre sub
     * applicatione fluitanti clarum, alternativis nominatis) */
    forma = NIHIL;
    num   = nodus->liberi != NIHIL
          ? xar_numerus(nodus->liberi) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(nodus->liberi, i);

        si (l == NIHIL || l->genus == STML_NODUS_COMMENTUM)
        {
            perge;
        }
        si (forma != NIHIL)
        {
            _vitium_ponere(ctx, STML_EXPANSIO_CORPUS_SILVESTRE,
                           nodus, NIHIL, NIHIL);
            redde FALSUM;
        }
        forma = l;
    }
    si (forma == NIHIL || forma->genus != STML_NODUS_ELEMENTUM)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_CORPUS_SILVESTRE, nodus,
                       NIHIL, NIHIL);
        redde FALSUM;
    }

    congruentiae = xar_creare(ctx->piscina,
                              magnitudo(StmlExemplarCongruentia));
    opus = xar_creare(ctx->piscina,
                      magnitudo(StmlExemplarLigamen));
    si (congruentiae == NIHIL || opus == NIHIL)
    {
        redde FALSUM;
    }

    si (fons != NIHIL)
    {
        num = xar_numerus(fons);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlExemplarCongruentia* ordo =
                (StmlExemplarCongruentia*)xar_obtinere(fons, i);

            si (   ordo != NIHIL
                && !_exemplar_petere(ctx, forma, ordo->radix,
                        ancorata, congruentiae, opus))
            {
                redde FALSUM;
            }
        }
    }
    alioquin si (ctx->radix_expansa != NIHIL)
    {
        /* scopus defaltus: arbor expansa PARTIALIS = contentum
         * supra (lex stratorum per constructionem) */
        si (!_exemplar_petere(ctx, forma, ctx->radix_expansa,
                              ancorata, congruentiae, opus))
        {
            redde FALSUM;
        }
    }

    /* modus (spec par. 2.3): defaltus omnia; unum/optional CLARE */
    num = xar_numerus(congruentiae);
    si (   modus != NIHIL && chorda_aequalis_literis(*modus, "unum")
        && num   != I)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_UNUM_VIOLATUM, nodus,
                       NIHIL, titulus);
        redde FALSUM;
    }
    si (   modus != NIHIL
        && chorda_aequalis_literis(*modus, "optional")
        && num > I)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_OPTIONAL_MULTIPLEX,
                       nodus, NIHIL, titulus);
        redde FALSUM;
    }
    si (   modus != NIHIL
        && chorda_aequalis_literis(*modus, "primum")
        && num > I)
    {
        xar_truncare(congruentiae, I);
    }
    *exitus = congruentiae;
    redde VERUM;
}

/* EXEMPLAR implere (gradu documenti): congruentias colligere,
 * modum applicare, relationem registrare. Nodus CONSUMITUR -
 * numquam emissus (invariatum spec par. 0). */
interior b32
_exemplar_implere (
             StmlNodus* nodus,
    StmlMacroContextus* ctx)
{
                    chorda* output;
                    chorda* de;
                    chorda* output_internatum;
                       Xar* fons;
                       Xar* congruentiae;
       StmlExemplarRelatio* rel;

    output = stml_attributum_capere(nodus, "output");
    si (   output                  == NIHIL || output->mensura < II
        || output->datum[ZEPHYRUM] != (i8)'$')
    {
        _vitium_ponere(ctx, STML_EXPANSIO_EXEMPLAR_SINE_EXITU,
                       nodus, NIHIL, output);
        redde FALSUM;
    }
    output_internatum = chorda_internare(ctx->intern, *output);
    si (output_internatum == NIHIL)
    {
        redde FALSUM;
    }
    si (_relationem_invenire(ctx, output_internatum) != NIHIL)
    {
        /* adsignatio unica (strata) - nomen bis ligatum */
        _vitium_ponere(ctx, STML_EXPANSIO_EXEMPLAR_MALFORMATUM,
                       nodus, NIHIL, output_internatum);
        redde FALSUM;
    }

    fons  = NIHIL;
    de    = stml_attributum_capere(nodus, "de");
    si (de != NIHIL)
    {
        chorda* de_internatum;

        si (de->mensura < II || de->datum[ZEPHYRUM] != (i8)'$')
        {
            _vitium_ponere(ctx, STML_EXPANSIO_EXEMPLAR_MALFORMATUM,
                           nodus, NIHIL, de);
            redde FALSUM;
        }
        de_internatum = chorda_internare(ctx->intern, *de);
        rel = de_internatum != NIHIL
            ? _relationem_invenire(ctx, de_internatum) : NIHIL;
        si (rel == NIHIL)
        {
            _vitium_ponere(ctx, STML_EXPANSIO_SCOPUS_IGNOTUS,
                           nodus, NIHIL, de);
            redde FALSUM;
        }
        rel->consumpta  = VERUM;
        fons            = rel->congruentiae;
    }

    si (!_exemplar_nucleus(ctx, nodus, fons, output_internatum,
                           &congruentiae))
    {
        redde FALSUM;
    }

    si (ctx->relationes == NIHIL)
    {
        ctx->relationes = xar_creare(
            ctx->piscina, magnitudo(StmlExemplarRelatio));
        si (ctx->relationes == NIHIL)
        {
            redde FALSUM;
        }
    }
    rel = (StmlExemplarRelatio*)xar_addere(ctx->relationes);
    si (rel == NIHIL)
    {
        redde FALSUM;
    }
    rel->titulus       = output_internatum;
    rel->congruentiae  = congruentiae;
    rel->origo         = nodus;
    rel->consumpta     = FALSUM;
    redde VERUM;
}

/* Corpus catenae processare: liberos ambulare, nexus ordine
 * filare. Fragmenta DISSOLVUNTUR (saccharum '<(>' = ornamentum,
 * numquam onus - ratificatio V), commenta et definitiones
 * praetermissa (praetransitus definitiones iam collegit),
 * vocationes sub PORTATIONE instantiatae (exemplaria producta
 * inerta emittuntur, deinde ut nexus leguntur - genus intrans =
 * genus exiens), CATENA nidificata PLANATUR (lex DISTRIBUTIO,
 * tertia apparitio: caput interius relationem influentem accipit,
 * cauda interior nexui exteriori proximo alimentum dat). Filum
 * per 'fons' geritur: nexus quisque relationem gradus prioris
 * legit, novam gignit - nomina intermedia numquam exsistunt
 * ('consumpta per constructionem'). */
interior b32
_catena_corpus_processare (
    StmlMacroContextus*  ctx,
                   Xar*  fratres,
                   i32   stratum,
                   i32   tectum,
                chorda*  titulus,
                   Xar** fons,
                   i32*  numerus)
{
    i32 i;
    i32 num;

    si (fratres == NIHIL)
    {
        redde VERUM;
    }
    num = xar_numerus(fratres);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* l;

        l = *(StmlNodus**)xar_obtinere(fratres, i);
        si (l == NIHIL || l->genus == STML_NODUS_COMMENTUM)
        {
            perge;
        }
        si (_est_definitio(l))
        {
            perge;
        }
        si (l->genus == STML_NODUS_ELEMENTUM && l->fragmentum)
        {
            si (!_catena_corpus_processare(ctx, l->liberi, stratum,
                                           tectum, titulus, fons,
                                           numerus))
            {
                redde FALSUM;
            }
            perge;
        }
        si (_est_vocatio(l))
        {
            StmlNodus* involucrum;
                  i32  saltus;
                  b32  bene;

            involucrum = stml_elementum_creare(ctx->piscina,
                                               ctx->intern,
                                               "catena-nexus");
            si (involucrum == NIHIL)
            {
                redde FALSUM;
            }
            ctx->applicatio++;
            bene = _liberum_expandere(involucrum, fratres, i,
                                      &saltus, ctx, stratum,
                                      tectum, NIHIL);
            ctx->applicatio--;
            si (!bene)
            {
                redde FALSUM;
            }
            i += saltus;
            si (!_catena_corpus_processare(ctx, involucrum->liberi,
                                           stratum, tectum,
                                           titulus,
                                           fons, numerus))
            {
                redde FALSUM;
            }
            perge;
        }
        si (_est_titulo(l, "EXEMPLAR"))
        {
            Xar* exitus;

            /* nexus utrimque apertus: de=/output= vetita in nexu
             * (output in nexu = uncus RESERVATUS - ratificationes
             * III/IV); modus= gradui licet */
            si (   stml_attributum_capere(l, "de")     != NIHIL
                || stml_attributum_capere(l, "output") != NIHIL)
            {
                _vitium_ponere(ctx,
                               STML_EXPANSIO_CATENA_MALFORMATA,
                               l, NIHIL, titulus);
                redde FALSUM;
            }
            si (!_exemplar_nucleus(ctx, l, *fons, titulus,
                                   &exitus))
            {
                redde FALSUM;
            }
            *fons = exitus;
            (*numerus)++;
            perge;
        }
        si (_est_titulo(l, "CATENA"))
        {
            /* nexus catenatus NUDUS solum: attributa sedi usus
             * pertinent, non fragmento condito (utrimque apertus) */
            si (   stml_attributum_capere(l, "de")       != NIHIL
                || stml_attributum_capere(l, "output")   != NIHIL
                || stml_attributum_capere(l, "modus")    != NIHIL
                || stml_attributum_capere(l, "ancorata") != NIHIL)
            {
                _vitium_ponere(ctx,
                               STML_EXPANSIO_CATENA_MALFORMATA,
                               l, NIHIL, titulus);
                redde FALSUM;
            }
            si (!_catena_corpus_processare(ctx, l->liberi, stratum,
                                           tectum, titulus, fons,
                                           numerus))
            {
                redde FALSUM;
            }
            perge;
        }
        /* liber alienus (textus, elementum non-nexus, transclusio
         * contenti) - CLARE, non tacite */
        _vitium_ponere(ctx, STML_EXPANSIO_CATENA_MALFORMATA, l,
                       NIHIL, titulus);
        redde FALSUM;
    }
    redde VERUM;
}

/* CATENA implere (gradu documenti): involucrum quod nexus ordine
 * filat - exitus gradus N = fons gradus N+1. Saccharum super
 * catenationem nominatam (inventum Rotae I: fistulatio =
 * nominatio); punctator relationis directe filatur, relatio
 * caudae sola sub output= registratur, ergo custos XVIII in cauda
 * concentratur. Involucrum ipsum output= fert (fragmenta condita
 * utrimque aperta - ratificatio I) et de= sub lege eadem ac
 * EXEMPLAR (defaltus radix). Spec par. 8 ianua XII. */
interior b32
_catena_implere (
             StmlNodus* nodus,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum)
{
                    chorda* output;
                    chorda* de;
                    chorda* output_internatum;
                       Xar* fons;
                       i32  numerus;
       StmlExemplarRelatio* rel;

    output = stml_attributum_capere(nodus, "output");
    si (   output                  == NIHIL || output->mensura < II
        || output->datum[ZEPHYRUM] != (i8)'$')
    {
        _vitium_ponere(ctx, STML_EXPANSIO_EXEMPLAR_SINE_EXITU,
                       nodus, NIHIL, output);
        redde FALSUM;
    }
    output_internatum = chorda_internare(ctx->intern, *output);
    si (output_internatum == NIHIL)
    {
        redde FALSUM;
    }
    si (_relationem_invenire(ctx, output_internatum) != NIHIL)
    {
        /* adsignatio unica (strata) - nomen bis ligatum */
        _vitium_ponere(ctx, STML_EXPANSIO_CATENA_MALFORMATA, nodus,
                       NIHIL, output_internatum);
        redde FALSUM;
    }
    si (   stml_attributum_capere(nodus, "modus")    != NIHIL
        || stml_attributum_capere(nodus, "ancorata") != NIHIL)
    {
        /* modus/ancorata GRADIBUS pertinent, non involucro */
        _vitium_ponere(ctx, STML_EXPANSIO_CATENA_MALFORMATA, nodus,
                       NIHIL, output_internatum);
        redde FALSUM;
    }

    fons  = NIHIL;
    de    = stml_attributum_capere(nodus, "de");
    si (de != NIHIL)
    {
        chorda* de_internatum;

        si (de->mensura < II || de->datum[ZEPHYRUM] != (i8)'$')
        {
            _vitium_ponere(ctx, STML_EXPANSIO_CATENA_MALFORMATA,
                           nodus, NIHIL, de);
            redde FALSUM;
        }
        de_internatum = chorda_internare(ctx->intern, *de);
        rel = de_internatum != NIHIL
            ? _relationem_invenire(ctx, de_internatum) : NIHIL;
        si (rel == NIHIL)
        {
            _vitium_ponere(ctx, STML_EXPANSIO_SCOPUS_IGNOTUS,
                           nodus, NIHIL, de);
            redde FALSUM;
        }
        rel->consumpta  = VERUM;
        fons            = rel->congruentiae;
    }

    numerus = ZEPHYRUM;
    si (!_catena_corpus_processare(ctx, nodus->liberi, stratum,
                                   tectum, output_internatum,
                                   &fons, &numerus))
    {
        redde FALSUM;
    }
    si (numerus == ZEPHYRUM)
    {
        /* catena vacua: nihil output ligaret (interior vacua sub
         * planatione = identitas licita; totalis vacua = vitium) */
        _vitium_ponere(ctx, STML_EXPANSIO_CATENA_MALFORMATA, nodus,
                       NIHIL, output_internatum);
        redde FALSUM;
    }

    si (ctx->relationes == NIHIL)
    {
        ctx->relationes = xar_creare(
            ctx->piscina, magnitudo(StmlExemplarRelatio));
        si (ctx->relationes == NIHIL)
        {
            redde FALSUM;
        }
    }
    rel = (StmlExemplarRelatio*)xar_addere(ctx->relationes);
    si (rel == NIHIL)
    {
        redde FALSUM;
    }
    rel->titulus       = output_internatum;
    rel->congruentiae  = fons;
    rel->origo         = nodus;
    rel->consumpta     = FALSUM;
    redde VERUM;
}

/* Ligamina -> tabula argumentorum: PONS ipse (spec par. 3).
 * Nodus = argumentum subarboreum (splex clonat - PER consumens
 * copiae est); valor = scalaris. */
interior Xar*
_ligamina_ad_argumenta (
    StmlMacroContextus* ctx,
                   Xar* ligamina)
{
    Xar* argumenta;
    i32  i;

    argumenta = xar_creare(ctx->piscina,
                           magnitudo(StmlMacroArgumentum));
    si (argumenta == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(ligamina); i++)
    {
        StmlExemplarLigamen* lig =
            (StmlExemplarLigamen*)xar_obtinere(ligamina, i);
        StmlMacroArgumentum* arg;

        si (lig == NIHIL)
        {
            perge;
        }
        arg = (StmlMacroArgumentum*)xar_addere(argumenta);
        si (arg == NIHIL)
        {
            redde NIHIL;
        }
        arg->titulus  = lig->titulus;
        arg->valor    = lig->valor;
        arg->arbores  = NIHIL;
        si (lig->nodus != NIHIL)
        {
            StmlNodus** cella;

            arg->arbores = xar_creare(ctx->piscina,
                                      magnitudo(StmlNodus*));
            cella = arg->arbores != NIHIL
                  ? (StmlNodus**)xar_addere(arg->arbores) : NIHIL;
            si (cella == NIHIL)
            {
                redde NIHIL;
            }
            *cella = lig->nodus;
        }
    }
    redde argumenta;
}

/* PER implere: instantiatio per congruentiam - corpus inscriptum
 * aut delegatio 'voca="#@f"'. Capturae ut argumenta adveniunt
 * (pons = scopus); machinae impletionis EAEDEM ac vocationis. */
interior b32
_per_implere (
             StmlNodus* parens_novus,
             StmlNodus* nodus,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum)
{
                 chorda* congruentia;
                 chorda* voca;
                 chorda* congruentia_internata;
    StmlExemplarRelatio* rel;
     StmlMacroDefinitio* def;
                    i32  i;
                    i32  num;

    congruentia = stml_attributum_capere(nodus, "congruentia");
    si (   congruentia                  == NIHIL
        || congruentia->mensura < II
        || congruentia->datum[ZEPHYRUM] != (i8)'$')
    {
        _vitium_ponere(ctx, STML_EXPANSIO_EXEMPLAR_MALFORMATUM,
                       nodus, NIHIL, congruentia);
        redde FALSUM;
    }
    congruentia_internata = chorda_internare(ctx->intern,
                                             *congruentia);
    rel = congruentia_internata != NIHIL
        ? _relationem_invenire(ctx, congruentia_internata) : NIHIL;
    si (rel == NIHIL)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_SCOPUS_IGNOTUS, nodus,
                       NIHIL, congruentia);
        redde FALSUM;
    }
    rel->consumpta = VERUM;

    def   = NIHIL;
    voca  = stml_attributum_capere(nodus, "voca");
    si (voca != NIHIL)
    {
        chorda  id_pars;
        chorda* id;

        si (   voca->mensura < III
            || voca->datum[ZEPHYRUM] != (i8)'#'
            || voca->datum[I]        != (i8)'@')
        {
            _vitium_ponere(ctx, STML_EXPANSIO_EXEMPLAR_MALFORMATUM,
                           nodus, NIHIL, voca);
            redde FALSUM;
        }
        id_pars.datum = voca->datum + I;
        id_pars.mensura = voca->mensura - I;
        id = chorda_internare(ctx->intern, id_pars);
        def = id != NIHIL ? _definitionem_invenire(ctx, id) : NIHIL;
        si (def == NIHIL)
        {
            _vitium_ponere(ctx, STML_EXPANSIO_FRAGMENTUM_IGNOTUM,
                           nodus, id, NIHIL);
            redde FALSUM;
        }
        si (def->ordo >= tectum || !def->praeterita)
        {
            _vitium_ponere(ctx, STML_EXPANSIO_FRAGMENTUM_POSTERIUS,
                           nodus, id, NIHIL);
            redde FALSUM;
        }
    }

    num = xar_numerus(rel->congruentiae);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlExemplarCongruentia* con =
            (StmlExemplarCongruentia*)xar_obtinere(
                rel->congruentiae, i);
        Xar* argumenta;

        si (con == NIHIL)
        {
            perge;
        }
        argumenta = _ligamina_ad_argumenta(ctx, con->ligamina);
        si (argumenta == NIHIL)
        {
            redde FALSUM;
        }
        si (def != NIHIL)
        {
            /* delegatio: loculi iudicantur ut vocatio */
            StmlExpansioNota* nota;
                         i32  j;
                         i32  ante_numerus;

            per (j = ZEPHYRUM; j < xar_numerus(argumenta); j++)
            {
                StmlMacroArgumentum* arg =
                    (StmlMacroArgumentum*)xar_obtinere(argumenta,
                                                       j);

                si (   arg != NIHIL
                    && _loculus_declaratus(def, arg->titulus)
                           == NIHIL)
                {
                    _vitium_ponere(ctx,
                        STML_EXPANSIO_ARGUMENTUM_SUPERFLUUM,
                        nodus, def->id, arg->titulus);
                    redde FALSUM;
                }
            }
            per (j = ZEPHYRUM; j < xar_numerus(def->loculi); j++)
            {
                StmlMacroLoculus* loc =
                    (StmlMacroLoculus*)xar_obtinere(def->loculi,
                                                    j);

                si (   loc != NIHIL
                    && !loc->optionalis
                    && _argumentum_invenire(argumenta,
                           loc->titulus) == NIHIL)
                {
                    _vitium_ponere(ctx,
                        STML_EXPANSIO_LOCULUS_NON_IMPLETUS,
                        nodus, def->id, loc->titulus);
                    redde FALSUM;
                }
            }
            nota = (StmlExpansioNota*)xar_addere(
                ctx->resultus->tabula_expansionum);
            si (nota == NIHIL)
            {
                redde FALSUM;
            }
            nota->nodus          = NIHIL;
            nota->fragmentum_id  = def->id;
            nota->vocatio        = nodus;
            nota->stratum        = stratum + I;
            ante_numerus = parens_novus->liberi != NIHIL
                ? xar_numerus(parens_novus->liberi) : ZEPHYRUM;
            si (!_liberos_expandere(parens_novus,
                    def->definitio->liberi, ctx, stratum + I,
                    def->ordo, argumenta))
            {
                redde FALSUM;
            }
            si (   parens_novus->liberi != NIHIL
                && xar_numerus(parens_novus->liberi)
                       > ante_numerus)
            {
                nota->nodus = stml_liberum_ad_indicem(
                    parens_novus, ante_numerus);
            }
        }
        alioquin si (!_liberos_expandere(parens_novus,
                         nodus->liberi, ctx, stratum + I, tectum,
                         argumenta))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

StmlExpansioResultus
stml_expandere (
              StmlNodus* radix,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlExpansioResultus resultus;
      StmlMacroContextus ctx;

    resultus.successus           = FALSUM;
    resultus.radix_expansa       = NIHIL;
    resultus.tabula_expansionum  = NIHIL;
    resultus.vitium              = STML_EXPANSIO_BENE;
    resultus.linea               = ZEPHYRUM;
    resultus.fragmentum.datum    = NIHIL;
    resultus.fragmentum.mensura  = ZEPHYRUM;
    resultus.loculus.datum       = NIHIL;
    resultus.loculus.mensura     = ZEPHYRUM;

    si (radix == NIHIL || piscina == NIHIL || intern == NIHIL)
    {
        redde resultus;
    }
    resultus.tabula_expansionum =
        xar_creare(piscina, magnitudo(StmlExpansioNota));
    si (resultus.tabula_expansionum == NIHIL)
    {
        redde resultus;
    }

    ctx.piscina  = piscina;
    ctx.intern   = intern;
    ctx.definitiones  = xar_creare(piscina,
                                   magnitudo(StmlMacroDefinitio));
    ctx.resultus    = &resultus;
    ctx.relationes  = NIHIL;   /* pigre - documentum sine
                                   * exemplaribus nihil solvit */
    ctx.transparentia  = NIHIL;
    ctx.radix_expansa  = NIHIL;
    ctx.applicatio     = ZEPHYRUM;
    si (ctx.definitiones == NIHIL)
    {
        redde resultus;
    }

    /* praetransitus: definitiones + GEMINUM */
    si (!_definitiones_colligere(radix, &ctx))
    {
        redde resultus;
    }

    resultus.radix_expansa = _expandere_nodum(
        radix, &ctx, ZEPHYRUM, xar_numerus(ctx.definitiones),
        NIHIL, NIHIL);
    si (resultus.radix_expansa == NIHIL)
    {
        redde resultus;
    }
    /* custos finis exemplarium: relatio quam nihil consumpsit =
     * codex mortuus (spec par. 2.2). MOLLIS - ambulatio perfecta
     * est, successus cadit per custodem vitii infra. */
    si (ctx.relationes != NIHIL)
    {
        i32 r;

        per (r = ZEPHYRUM; r < xar_numerus(ctx.relationes); r++)
        {
            StmlExemplarRelatio* rel;

            rel = (StmlExemplarRelatio*)xar_obtinere(
                ctx.relationes, r);
            si (rel != NIHIL && !rel->consumpta)
            {
                _vitium_ponere(&ctx,
                    STML_EXPANSIO_EXITUS_NON_CONSUMPTUS,
                    rel->origo, NIHIL, rel->titulus);
            }
        }
    }
    /* custos vitii mollis: _chordam_substituere vitium ponit sed
     * ambulationem non frangit (littera manet) - successus hic
     * cadit, non tacite transit */
    resultus.successus = (resultus.vitium == STML_EXPANSIO_BENE);
    redde resultus;
}


/* ==================================================
 * DISTRIBUTIO (contractus et leges in stml_macros.h)
 *
 * Ambulatio secunda, a machina expansionis SEIUNCTA: involucra
 * (elementa nominata quorum liberi item fragmenta anonyma sunt)
 * in fratres typo involucri dissolvuntur. Clonatio ubique - arbor
 * originalis intacta (contractus duarum arborum). Liberi item in
 * elementa constructa MOVENTUR (clones proiectionis sunt, numquam
 * nodi originales).
 * ================================================== */

nomen structura {
                    Piscina* piscina;
        InternamentumChorda* intern;
    StmlDistributioResultus* resultus;
} StmlDistContextus;

/* item = fragmentum anonymum */
interior b32
_dist_est_item (
    StmlNodus* nodus)
{
    redde nodus != NIHIL
        && nodus->genus == STML_NODUS_ELEMENTUM
        && nodus->fragmentum
        && nodus->fragmentum_id == NIHIL;
}

/* involucrabile = elementum contenti NOMINATUM (non fragmentum,
 * non elementum attributi '@', non augmentatio '%') */
interior b32
_dist_est_nominatum (
    StmlNodus* nodus)
{
    redde nodus != NIHIL
        && nodus->genus == STML_NODUS_ELEMENTUM
        && !nodus->fragmentum
        && nodus->attributum_titulus == NIHIL
        && nodus->augmentum_clavis == NIHIL
        && nodus->titulus != NIHIL
        && nodus->titulus->mensura > ZEPHYRUM;
}

/* Nominatne item attributum? Inscriptum AUT elementum praefixi
 * (sepulcra inclusa) - collisio ITEM-VINCIT per suppressionem
 * attributi involucri eiusdem nominis. */
interior b32
_dist_item_nominat (
    StmlNodus* item,
       chorda* titulus)
{
    i32 i;
    i32 num;

    si (item == NIHIL || titulus == NIHIL)
    {
        redde FALSUM;
    }
    si (item->attributa != NIHIL)
    {
        num = xar_numerus(item->attributa);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlAttributum* a;

            a = (StmlAttributum*)xar_obtinere(item->attributa, i);
            si (   a->titulus != NIHIL
                && chorda_aequalis(*a->titulus, *titulus))
            {
                redde VERUM;
            }
        }
    }
    si (item->liberi != NIHIL)
    {
        num = xar_numerus(item->liberi);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlNodus* l;

            l = *(StmlNodus**)xar_obtinere(item->liberi, i);
            si (   l->genus              != STML_NODUS_ELEMENTUM
                || l->attributum_titulus == NIHIL)
            {
                frange;  /* praefixum finitur */
            }
            si (chorda_aequalis(*l->attributum_titulus, *titulus))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

interior vacuum
_dist_vitium (
        StmlDistContextus* ctx,
    StmlDistributioVitium  vitium,
                      i32  linea,
                   chorda* titulus)
{
    si (ctx->resultus->vitium != STML_DISTRIBUTIO_BENE)
    {
        redde;  /* primum vincit */
    }
    ctx->resultus->vitium  = vitium;
    ctx->resultus->linea   = linea;
    si (titulus != NIHIL)
    {
        ctx->resultus->titulus = *titulus;
    }
}

interior b32
_dist_appendere (
    StmlDistContextus* ctx,
            StmlNodus* parens,
            StmlNodus* nodus)
{
    StmlNodus** cella;

    si (parens->liberi == NIHIL)
    {
        parens->liberi = xar_creare(ctx->piscina,
                                    magnitudo(StmlNodus*));
        si (parens->liberi == NIHIL)
        {
            _dist_vitium(ctx, STML_DISTRIBUTIO_MEMORIA, ZEPHYRUM,
                         NIHIL);
            redde FALSUM;
        }
    }
    cella = (StmlNodus**)xar_addere(parens->liberi);
    si (cella == NIHIL)
    {
        _dist_vitium(ctx, STML_DISTRIBUTIO_MEMORIA, ZEPHYRUM,
                     NIHIL);
        redde FALSUM;
    }
    *cella         = nodus;
    nodus->parens  = parens;
    redde VERUM;
}

interior b32
_dist_nodum (
    StmlDistContextus* ctx,
            StmlNodus* parens,
            StmlNodus* nodus);

/* Dissolutio: novum (liberi iam distributi) aut appenditur
 * intactum (nulla item) aut in fratres typo involucri
 * dissolvitur. Auto-recursiva: elementum constructum ipsum
 * involucrum esse potest (listae anonymae nidificatae typo eodem
 * PLANANTUR). */
interior b32
_dist_dissolvere (
    StmlDistContextus* ctx,
            StmlNodus* parens,
            StmlNodus* novum)
{
    i32 i;
    i32 num;
    i32 praefixum;
    i32 numerus_item;
    b32 mixtum;
    i32 linea_mixti;

    num = (novum->liberi != NIHIL) ? xar_numerus(novum->liberi)
                                   : ZEPHYRUM;

    /* praefixum elementorum attributi = attributa involucri ipsius */
    praefixum = ZEPHYRUM;
    dum (praefixum < num)
    {
        StmlNodus* l;

        l = *(StmlNodus**)xar_obtinere(novum->liberi, praefixum);
        si (   l->genus              != STML_NODUS_ELEMENTUM
            || l->attributum_titulus == NIHIL)
        {
            frange;
        }
        praefixum++;
    }

    /* census: item omnia aut nulla (commenta transeunt) */
    numerus_item  = ZEPHYRUM;
    mixtum        = FALSUM;
    linea_mixti   = ZEPHYRUM;
    per (i = praefixum; i < num; i++)
    {
        StmlNodus* l;

        l = *(StmlNodus**)xar_obtinere(novum->liberi, i);
        si (_dist_est_item(l))
        {
            numerus_item++;
        }
        alioquin si (l->genus != STML_NODUS_COMMENTUM && !mixtum)
        {
            mixtum       = VERUM;
            linea_mixti  = l->linea;
        }
    }

    si (numerus_item == ZEPHYRUM)
    {
        redde _dist_appendere(ctx, parens, novum);
    }
    si (mixtum)
    {
        _dist_vitium(ctx, STML_DISTRIBUTIO_MIXTA, linea_mixti,
                     novum->titulus);
        redde FALSUM;
    }

    per (i = praefixum; i < num; i++)
    {
        StmlNodus* l;

        l = *(StmlNodus**)xar_obtinere(novum->liberi, i);
        si (l->genus == STML_NODUS_COMMENTUM)
        {
            si (!_dist_appendere(ctx, parens, l))
            {
                redde FALSUM;
            }
            perge;
        }

        /* elementum novum: typus et attributa involucri, corpus
         * et dispositio item */
        {
            StmlNodus* e;
                  i32  j;
                  i32  num_j;

            e = stml_duplicare_superficialiter(novum, ctx->piscina,
                                               ctx->intern);
            si (e == NIHIL)
            {
                _dist_vitium(ctx, STML_DISTRIBUTIO_MEMORIA,
                             ZEPHYRUM, NIHIL);
                redde FALSUM;
            }
            e->spatia_ante       = l->spatia_ante;
            e->spatia_post       = l->spatia_post;
            e->spatia_clausurae  = l->spatia_clausurae;
            e->clausura_anonyma  = l->clausura_anonyma;
            e->linea             = l->linea;

            /* attributa inscripta: involucrum (item-vincit
             * filtratum) deinde item */
            e->attributa = NIHIL;
            si (   (   novum->attributa != NIHIL
                    && xar_numerus(novum->attributa) > ZEPHYRUM)
                || (   l->attributa != NIHIL
                    && xar_numerus(l->attributa) > ZEPHYRUM))
            {
                e->attributa = xar_creare(ctx->piscina,
                                          magnitudo(StmlAttributum));
                si (e->attributa == NIHIL)
                {
                    _dist_vitium(ctx, STML_DISTRIBUTIO_MEMORIA,
                                 ZEPHYRUM, NIHIL);
                    redde FALSUM;
                }
                num_j = (novum->attributa != NIHIL)
                            ? xar_numerus(novum->attributa)
                            : ZEPHYRUM;
                per (j = ZEPHYRUM; j < num_j; j++)
                {
                    StmlAttributum* a;
                    StmlAttributum* cella;

                    a = (StmlAttributum*)xar_obtinere(
                        novum->attributa, j);
                    si (_dist_item_nominat(l, a->titulus))
                    {
                        perge;
                    }
                    cella = (StmlAttributum*)xar_addere(
                        e->attributa);
                    si (cella == NIHIL)
                    {
                        _dist_vitium(ctx, STML_DISTRIBUTIO_MEMORIA,
                                     ZEPHYRUM, NIHIL);
                        redde FALSUM;
                    }
                    *cella = *a;
                }
                num_j = (l->attributa != NIHIL)
                            ? xar_numerus(l->attributa)
                            : ZEPHYRUM;
                per (j = ZEPHYRUM; j < num_j; j++)
                {
                    StmlAttributum* a;
                    StmlAttributum* cella;

                    a = (StmlAttributum*)xar_obtinere(l->attributa,
                                                      j);
                    cella = (StmlAttributum*)xar_addere(
                        e->attributa);
                    si (cella == NIHIL)
                    {
                        _dist_vitium(ctx, STML_DISTRIBUTIO_MEMORIA,
                                     ZEPHYRUM, NIHIL);
                        redde FALSUM;
                    }
                    *cella = *a;
                }
            }

            /* liberi: praefixum involucri (filtratum, clonatum per
             * item - nodus communis parentes plures habere nequit)
             * deinde liberi item MOTI */
            e->liberi = NIHIL;
            per (j = ZEPHYRUM; j < praefixum; j++)
            {
                StmlNodus* ae;
                StmlNodus* clon;

                ae = *(StmlNodus**)xar_obtinere(novum->liberi, j);
                si (_dist_item_nominat(l, ae->attributum_titulus))
                {
                    perge;
                }
                clon = stml_duplicare(ae, ctx->piscina, ctx->intern);
                si (clon == NIHIL)
                {
                    _dist_vitium(ctx, STML_DISTRIBUTIO_MEMORIA,
                                 ZEPHYRUM, NIHIL);
                    redde FALSUM;
                }
                si (!_dist_appendere(ctx, e, clon))
                {
                    redde FALSUM;
                }
            }
            num_j = (l->liberi != NIHIL) ? xar_numerus(l->liberi)
                                         : ZEPHYRUM;
            per (j = ZEPHYRUM; j < num_j; j++)
            {
                StmlNodus* liberum;

                liberum = *(StmlNodus**)xar_obtinere(l->liberi, j);
                si (!_dist_appendere(ctx, e, liberum))
                {
                    redde FALSUM;
                }
            }

            /* recursio planationis */
            si (!_dist_dissolvere(ctx, parens, e))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

interior b32
_dist_liberos (
    StmlDistContextus* ctx,
            StmlNodus* parens_novus,
                  Xar* fratres)
{
    i32 i;
    i32 num;

    num = xar_numerus(fratres);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* liberum;

        liberum = *(StmlNodus**)xar_obtinere(fratres, i);
        si (!_dist_nodum(ctx, parens_novus, liberum))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_dist_nodum (
    StmlDistContextus* ctx,
            StmlNodus* parens,
            StmlNodus* nodus)
{
    StmlNodus* novum;

    si (nodus == NIHIL)
    {
        redde VERUM;
    }
    si (nodus->genus != STML_NODUS_ELEMENTUM)
    {
        novum = stml_duplicare(nodus, ctx->piscina, ctx->intern);
        si (novum == NIHIL)
        {
            _dist_vitium(ctx, STML_DISTRIBUTIO_MEMORIA, ZEPHYRUM,
                         NIHIL);
            redde FALSUM;
        }
        redde _dist_appendere(ctx, parens, novum);
    }

    novum = stml_duplicare_superficialiter(nodus, ctx->piscina,
                                           ctx->intern);
    si (novum == NIHIL)
    {
        _dist_vitium(ctx, STML_DISTRIBUTIO_MEMORIA, ZEPHYRUM,
                     NIHIL);
        redde FALSUM;
    }
    si (nodus->liberi != NIHIL)
    {
        novum->liberi = xar_creare(ctx->piscina,
                                   magnitudo(StmlNodus*));
        si (novum->liberi == NIHIL)
        {
            _dist_vitium(ctx, STML_DISTRIBUTIO_MEMORIA, ZEPHYRUM,
                         NIHIL);
            redde FALSUM;
        }
        si (!_dist_liberos(ctx, novum, nodus->liberi))
        {
            redde FALSUM;
        }
    }
    si (_dist_est_nominatum(novum))
    {
        redde _dist_dissolvere(ctx, parens, novum);
    }
    redde _dist_appendere(ctx, parens, novum);
}

StmlDistributioResultus
stml_distribuere (
              StmlNodus* radix,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlDistributioResultus  resultus;
          StmlDistContextus  ctx;
                  StmlNodus* radix_nova;

    resultus.successus         = FALSUM;
    resultus.radix_distributa  = NIHIL;
    resultus.vitium            = STML_DISTRIBUTIO_BENE;
    resultus.linea             = ZEPHYRUM;
    resultus.titulus.datum     = NIHIL;
    resultus.titulus.mensura   = ZEPHYRUM;

    si (radix == NIHIL || piscina == NIHIL || intern == NIHIL)
    {
        redde resultus;
    }

    ctx.piscina   = piscina;
    ctx.intern    = intern;
    ctx.resultus  = &resultus;

    radix_nova = stml_duplicare_superficialiter(radix, piscina,
                                                intern);
    si (radix_nova == NIHIL)
    {
        resultus.vitium = STML_DISTRIBUTIO_MEMORIA;
        redde resultus;
    }
    si (radix->liberi != NIHIL)
    {
        radix_nova->liberi = xar_creare(piscina,
                                        magnitudo(StmlNodus*));
        si (radix_nova->liberi == NIHIL)
        {
            resultus.vitium = STML_DISTRIBUTIO_MEMORIA;
            redde resultus;
        }
        si (!_dist_liberos(&ctx, radix_nova, radix->liberi))
        {
            redde resultus;  /* vitium iam positum */
        }
    }
    resultus.successus = (resultus.vitium == STML_DISTRIBUTIO_BENE);
    si (resultus.successus)
    {
        resultus.radix_distributa = radix_nova;
    }
    redde resultus;
}


/* ==================================================
 * CONGRUENTIA STRICTA (spec exemplarium par. 4, gradus I)
 *
 * Ex silva_arbor.c promota (recognitio parametrorum, 2026-08-26)
 * - extractio MOVET, non emendat: logica verbatim, possessio
 * sola mutata (scriptor -> piscina + intern explicita).
 *
 * NOTA UNIFICATIONIS: grammatica referentiae totius
 * (_congruentia_referentia_tota) eadem est ac _loculum_invenire
 * supra - lex una, sedes DUAE adhuc in plagula una. Unificatio
 * suo tempore, cum modus laxus aedificetur, non hic.
 * ================================================== */

/* Estne valor textus TOTUS extensio '&@x;'? Nomen (sine
 * sigillis) redditur. */
interior b32
_congruentia_referentia_tota (
    constans chorda* valor,
             chorda* titulus_exitus)
{
    i32 i;

    si (   valor                            == NIHIL
        || valor->mensura < IV
        || valor->datum[ZEPHYRUM]           != (i8)'&'
        || valor->datum[I]                  != (i8)'@'
        || valor->datum[valor->mensura - I] != (i8)';')
    {
        redde FALSUM;
    }
    per (i = II; i < valor->mensura - I; i++)
    {
        i8 c = valor->datum[i];

        si (!(   (c >= (i8)'a' && c <= (i8)'z')
              || (c >= (i8)'A' && c <= (i8)'Z')
              || (c >= (i8)'0' && c <= (i8)'9')
              || c == (i8)'_'
              || c == (i8)'-'
              || c == (i8)'.'))
        {
            redde FALSUM;
        }
    }
    titulus_exitus->datum    = valor->datum + II;
    titulus_exitus->mensura  = valor->mensura - III;
    redde VERUM;
}

interior b32
_congruentia_attributa_aequalia (
    constans StmlNodus* a,
    constans StmlNodus* b)
{
    i32 na;
    i32 nb;
    i32 i;

    na = a->attributa != NIHIL ? xar_numerus(a->attributa)
                               : ZEPHYRUM;
    nb = b->attributa != NIHIL ? xar_numerus(b->attributa)
                               : ZEPHYRUM;
    si (na != nb)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < na; i++)
    {
        StmlAttributum* aa;
        StmlAttributum* ab;

        aa = (StmlAttributum*)xar_obtinere(a->attributa, i);
        ab = (StmlAttributum*)xar_obtinere(b->attributa, i);
        si (   aa          == NIHIL || ab == NIHIL
            || aa->titulus == NIHIL || ab->titulus == NIHIL
            || !chorda_aequalis(*aa->titulus, *ab->titulus))
        {
            redde FALSUM;
        }
        si ((aa->valor != NIHIL) != (ab->valor != NIHIL))
        {
            redde FALSUM;
        }
        si (   aa->valor != NIHIL
            && !chorda_aequalis(*aa->valor, *ab->valor))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Silvae binae octetim aequales? (loculus iteratus - regula V:
 * capturae non-lineares aequalitas sunt, ne tacite divergant) */
interior b32
_congruentia_silvae_aequales (
    Piscina* piscina,
        Xar* a,
        Xar* b)
{
    i32 i;

    si (xar_numerus(a) != xar_numerus(b))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(a); i++)
    {
        StmlNodus* na = *(StmlNodus**)xar_obtinere(a, i);
        StmlNodus* nb = *(StmlNodus**)xar_obtinere(b, i);
           chorda  sa;
           chorda  sb;

        si (na == NIHIL || nb == NIHIL)
        {
            redde FALSUM;
        }
        sa = stml_scribere(na, piscina, FALSUM);
        sb = stml_scribere(nb, piscina, FALSUM);
        si (!chorda_aequalis(sa, sb))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
stml_congruere_strictum (
                Piscina* piscina,
    InternamentumChorda* intern,
              StmlNodus* templum,
              StmlNodus* candidatus,
                    Xar* capturae,
                    Xar* paria)
{
    i32 nd;
    i32 nc;
    i32 i;

    si (   piscina  == NIHIL || intern == NIHIL
        || templum  == NIHIL || candidatus == NIHIL
        || capturae == NIHIL)
    {
        redde FALSUM;
    }
    si (templum->genus != candidatus->genus)
    {
        redde FALSUM;
    }
    si (   templum->genus == STML_NODUS_TEXTUS
        || templum->genus == STML_NODUS_TRANSCLUSIO)
    {
        redde templum->valor != NIHIL && candidatus->valor != NIHIL
            && chorda_aequalis(*templum->valor, *candidatus->valor);
    }
    si (templum->genus != STML_NODUS_ELEMENTUM)
    {
        redde FALSUM;
    }
    si (   templum->titulus    == NIHIL
        || candidatus->titulus == NIHIL
        || !chorda_aequalis(*templum->titulus, *candidatus->titulus)
        || templum->fragmentum != candidatus->fragmentum
        || templum->crudus     != candidatus->crudus
        || (templum->attributum_titulus != NIHIL)
               != (candidatus->attributum_titulus != NIHIL)
        || !_congruentia_attributa_aequalia(templum, candidatus))
    {
        redde FALSUM;
    }

    /* captura silvae: liberum templi UNICUM textus totus '&@x;' -
     * liberi candidati OMNES capti (saltem unus, ne argumentum
     * vacuum sepulcrum fieret) */
    nd = stml_numerus_liberorum(templum);
    nc = stml_numerus_liberorum(candidatus);
    si (nd == I)
    {
        StmlNodus* ld = stml_liberum_ad_indicem(templum, ZEPHYRUM);
           chorda  titulus_capturae;

        si (   ld        != NIHIL
            && ld->genus == STML_NODUS_TEXTUS
            && ld->valor != NIHIL
            && _congruentia_referentia_tota(ld->valor,
                   &titulus_capturae))
        {
            StmlCaptura* captura;
                 chorda* titulus_internatus;
                    Xar* nodi;
                    i32  j;

            si (nc < I)
            {
                redde FALSUM;
            }
            titulus_internatus = chorda_internare(intern,
                titulus_capturae);
            nodi = xar_creare(piscina, magnitudo(StmlNodus*));
            si (titulus_internatus == NIHIL || nodi == NIHIL)
            {
                redde FALSUM;
            }
            per (j = ZEPHYRUM; j < nc; j++)
            {
                StmlNodus** cella =
                    (StmlNodus**)xar_addere(nodi);

                si (cella == NIHIL)
                {
                    redde FALSUM;
                }
                *cella = stml_liberum_ad_indicem(candidatus, j);
            }
            /* loculus iteratus: silvae aequales (regula V) */
            per (j = ZEPHYRUM; j < xar_numerus(capturae); j++)
            {
                StmlCaptura* prior =
                    (StmlCaptura*)xar_obtinere(capturae, j);

                si (   prior          != NIHIL
                    && prior->titulus == titulus_internatus)
                {
                    redde _congruentia_silvae_aequales(piscina,
                        prior->nodi, nodi);
                }
            }
            captura = (StmlCaptura*)xar_addere(capturae);
            si (captura == NIHIL)
            {
                redde FALSUM;
            }
            captura->titulus  = titulus_internatus;
            captura->nodi     = nodi;
            /* par pro involucro capturante (elementum candidati ad
             * elementum templi) */
            si (paria != NIHIL)
            {
                StmlCongruentiaPar* par =
                    (StmlCongruentiaPar*)xar_addere(paria);

                si (par == NIHIL)
                {
                    redde FALSUM;
                }
                par->vetus = candidatus;
                par->novus = templum;
            }
            redde VERUM;
        }
    }
    si (nd != nc)
    {
        redde FALSUM;
    }
    si (paria != NIHIL)
    {
        StmlCongruentiaPar* par =
            (StmlCongruentiaPar*)xar_addere(paria);

        si (par == NIHIL)
        {
            redde FALSUM;
        }
        par->vetus = candidatus;
        par->novus = templum;
    }
    per (i = ZEPHYRUM; i < nd; i++)
    {
        StmlNodus* ld = stml_liberum_ad_indicem(templum, i);
        StmlNodus* lc = stml_liberum_ad_indicem(candidatus, i);

        si (   ld == NIHIL || lc == NIHIL
            || !stml_congruere_strictum(piscina, intern, ld, lc,
                   capturae, paria))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}
