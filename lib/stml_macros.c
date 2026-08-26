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
           Xar* loculi;      /* chorda* internata */
           i32  ordo;        /* index collectionis = ordo documenti */
           b32  praeterita;
} StmlMacroDefinitio;

/* Argumentum vocationis: par nomen-valor ex interiore
 * transclusionis parsatum ('p="123"'). */
nomen structura {
    chorda* titulus;  /* internatum */
    chorda* valor;    /* internatum */
} StmlMacroArgumentum;

nomen structura {
                 Piscina* piscina;
     InternamentumChorda* intern;
                     Xar* definitiones;  /* StmlMacroDefinitio */
    StmlExpansioResultus* resultus;      /* campi vitii hic ponuntur */
} StmlMacroContextus;

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

/* Estne nodus definitio macronis? (fragmentum cum id; fragmenta
 * anonyma contentum manent - nihil ea vocare potest) */
interior b32
_est_definitio (
    constans StmlNodus* nodus)
{
    redde    nodus->genus == STML_NODUS_ELEMENTUM
          && nodus->fragmentum
          && nodus->fragmentum_id != NIHIL;
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

interior b32
_loculus_declaratus (
    StmlMacroDefinitio* def,
                chorda* titulus)  /* internatum */
{
    i32 i;
    i32 num;

    num = xar_numerus(def->loculi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        chorda** cella;

        cella = (chorda**)xar_obtinere(def->loculi, i);
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
            chorda   titulus;
            chorda** cella;

            titulus.datum    = attr->valor->datum + I;
            titulus.mensura  = attr->valor->mensura - I;
            cella            = (chorda**)xar_addere(def->loculi);
            si (cella != NIHIL)
            {
                *cella = chorda_internare(ctx->intern, titulus);
            }
        }
    }
}

/* Chordam contra loculos declaratos perscrutari (collectione:
 * referentia non declarata = LOCULUS_IGNOTUS in loco definitionis,
 * ante vocationem ullam). */
interior b32
_chordam_perscrutari (
       constans chorda* textus,
             StmlNodus* nodus,
    StmlMacroDefinitio* def,
    StmlMacroContextus* ctx)
{
    chorda titulus;
       i32 ab;
       i32 initium;
       i32 post;

    ab = ZEPHYRUM;
    dum (_loculum_invenire(textus, ab, &initium, &post, &titulus))
    {
        chorda* titulus_internatus;

        titulus_internatus = chorda_internare(ctx->intern, titulus);
        si (!_loculus_declaratus(def, titulus_internatus))
        {
            _vitium_ponere(ctx, STML_EXPANSIO_LOCULUS_IGNOTUS,
                           nodus, def->id, titulus_internatus);
            redde FALSUM;
        }
        ab = post;
    }
    redde VERUM;
}

/* Corpus definitionis perscrutari: valores textus et attributorum
 * recursive (attributa tagi definitionis IPSIUS non - ea
 * declarationes sunt). */
interior b32
_corpus_perscrutari (
             StmlNodus* nodus,
    StmlMacroDefinitio* def,
    StmlMacroContextus* ctx)
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
        si (   (   liberum->genus == STML_NODUS_TEXTUS
                || liberum->genus == STML_NODUS_TRANSCLUSIO)
            && liberum->valor != NIHIL
            && !_chordam_perscrutari(liberum->valor, liberum, def,
                                     ctx))
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
                                             def, ctx))
                {
                    redde FALSUM;
                }
            }
        }
        si (!_corpus_perscrutari(liberum, def, ctx))
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
                                       magnitudo(chorda*));
        si (nova->loculi == NIHIL)
        {
            redde FALSUM;
        }
        _loculos_declaratos_legere(nodus, nova, ctx);
        si (!_corpus_perscrutari(nodus, nova, ctx))
        {
            redde FALSUM;
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

/* Estne transclusio vocatio macronis? (valor incipit '#') */
interior b32
_est_vocatio (
    constans StmlNodus* nodus)
{
    redde    nodus->genus == STML_NODUS_TRANSCLUSIO
          && nodus->valor != NIHIL
          && nodus->valor->mensura > ZEPHYRUM
          && nodus->valor->datum[ZEPHYRUM] == (i8)'#';
}

interior b32
_est_spatium_interius (
    i8 c)
{
    redde c == (i8)' ' || c == (i8)'\t' || c == (i8)'\n';
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

            arg = (StmlMacroArgumentum*)xar_addere(argumenta);
            si (arg == NIHIL)
            {
                redde FALSUM;
            }
            arg->titulus  = chorda_internare(ctx->intern, titulus);
            arg->valor    = chorda_internare(ctx->intern, valor);
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
 * chorda originalis immutata redditur. */
interior chorda*
_chordam_substituere (
    StmlMacroContextus* ctx,
                chorda* textus,
                   Xar* argumenta)
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
        si (arg != NIHIL)
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
                   Xar* argumenta);

interior b32
_liberum_expandere (
             StmlNodus* parens_novus,
             StmlNodus* liberum,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum,
                   Xar* argumenta);

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
                   i32  stratum,
                   i32  tectum,
    StmlMacroContextus* ctx)
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
    num = xar_numerus(argumenta);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlMacroArgumentum* arg;

        arg = (StmlMacroArgumentum*)xar_obtinere(argumenta, i);
        si (arg != NIHIL && !_loculus_declaratus(def, arg->titulus))
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
        chorda** cella;

        cella = (chorda**)xar_obtinere(def->loculi, i);
        si (   cella                                   != NIHIL
            && _argumentum_invenire(argumenta, *cella) == NIHIL)
        {
            _vitium_ponere(ctx,
                           STML_EXPANSIO_LOCULUS_NON_IMPLETUS,
                           vocatio, def->id, *cella);
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
    si (def->definitio->liberi != NIHIL)
    {
        num = xar_numerus(def->definitio->liberi);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlNodus* corporis;

            corporis = *(StmlNodus**)xar_obtinere(
                def->definitio->liberi, i);
            si (corporis == NIHIL)
            {
                perge;
            }
            si (!_liberum_expandere(parens_novus, corporis, ctx,
                                    stratum + I, def->ordo,
                                    argumenta))
            {
                redde FALSUM;
            }
        }
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
                   Xar* argumenta)
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
    si (argumenta != NIHIL && xar_numerus(argumenta) > ZEPHYRUM)
    {
        si (   (   novum->genus == STML_NODUS_TEXTUS
                || novum->genus == STML_NODUS_TRANSCLUSIO)
            && novum->valor != NIHIL)
        {
            novum->valor = _chordam_substituere(ctx, novum->valor,
                                                argumenta);
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
                        ctx, attr->valor, argumenta);
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
        num = xar_numerus(nodus->liberi);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlNodus* liberum;

            liberum = *(StmlNodus**)xar_obtinere(nodus->liberi, i);
            si (liberum == NIHIL)
            {
                perge;
            }
            si (!_liberum_expandere(novum, liberum, ctx, stratum,
                                    tectum, argumenta))
            {
                redde NIHIL;
            }
        }
    }
    redde novum;
}

/* Liberum unum expandere - interceptio COMMUNIS ambulationis
 * documenti et impletionis corporum (vocatio liberos plures parit,
 * ergo in ansa liberorum vivit, non in casu nodi):
 * - definitio: ambulatione documenti demissa + praeterita notata;
 *   in impletione materia citata OPACA, verbatim clonata;
 * - vocatio: interior effectivus (substitutus in impletione -
 *   transitio argumentorum), deinde impleta;
 * - cetera: recursio ambulationis. */
interior b32
_liberum_expandere (
             StmlNodus* parens_novus,
             StmlNodus* liberum,
    StmlMacroContextus* ctx,
                   i32  stratum,
                   i32  tectum,
                   Xar* argumenta)
{
    StmlNodus* liberum_novum;

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
                                       argumenta)
                : liberum->valor;
        redde _vocationem_implere(parens_novus, liberum,
                                  valor_effectivus, stratum, tectum,
                                  ctx);
    }
    liberum_novum = _expandere_nodum(liberum, ctx, stratum, tectum,
                                     argumenta);
    si (liberum_novum == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)stml_liberum_addere(parens_novus, liberum_novum);
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
    ctx.resultus      = &resultus;
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
        NIHIL);
    si (resultus.radix_expansa == NIHIL)
    {
        redde resultus;
    }
    resultus.successus = VERUM;
    redde resultus;
}
