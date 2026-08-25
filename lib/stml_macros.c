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

/* Definitio collecta: id internatum -> nodus definitionis.
 * 'praeterita' = ambulatio expansionis eam iam demisit - vocationes
 * eam vident (strata ordine documenti: vocatio ante definitionem =
 * FRAGMENTUM_POSTERIUS, non IGNOTUM). Xar cum scansione lineari -
 * definitiones per documentum paucae. */
nomen structura {
        chorda* id;          /* internatum */
     StmlNodus* definitio;
           b32  praeterita;
} StmlMacroDefinitio;

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
                chorda* fragmentum)
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
                           nodus, nodus->fragmentum_id);
            redde FALSUM;
        }
        nova = (StmlMacroDefinitio*)xar_addere(ctx->definitiones);
        si (nova == NIHIL)
        {
            redde FALSUM;
        }
        nova->id          = nodus->fragmentum_id;
        nova->definitio   = nodus;
        nova->praeterita  = FALSUM;
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

/* Id vocationis ex interiore transclusionis: post '#' usque ad
 * spatium primum aut finem ('#f p="123"' -> 'f'). Argumenta post
 * id T4 parsabit. */
interior chorda*
_vocationis_id (
    StmlMacroContextus* ctx,
             StmlNodus* vocatio)
{
    chorda id;
       i32 finis;

    id.datum    = vocatio->valor->datum + I;
    id.mensura  = vocatio->valor->mensura - I;
    finis       = ZEPHYRUM;
    dum (   finis < id.mensura
         && id.datum[finis] != (i8)' '
         && id.datum[finis] != (i8)'\t'
         && id.datum[finis] != (i8)'\n')
    {
        finis++;
    }
    id.mensura = finis;
    redde chorda_internare(ctx->intern, id);
}

interior StmlNodus*
_expandere_nodum (
             StmlNodus* nodus,
    StmlMacroContextus* ctx);

/* Vocationem implere: corpus definitionis clonatum in parentem
 * splicare, notam registrare. Nota.nodus = clonis PRIMUS splicis
 * (NIHIL si corpus vacuum); liberi implicati. */
interior b32
_vocationem_implere (
             StmlNodus* parens_novus,
             StmlNodus* vocatio,
    StmlMacroContextus* ctx)
{
                chorda* id;
    StmlMacroDefinitio* def;
      StmlExpansioNota* nota;
             StmlNodus* primus;
                   i32  i;
                   i32  num;

    id   = _vocationis_id(ctx, vocatio);
    def  = _definitionem_invenire(ctx, id);
    si (def == NIHIL)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_FRAGMENTUM_IGNOTUM,
                       vocatio, id);
        redde FALSUM;
    }
    si (!def->praeterita)
    {
        _vitium_ponere(ctx, STML_EXPANSIO_FRAGMENTUM_POSTERIUS,
                       vocatio, id);
        redde FALSUM;
    }

    primus = NIHIL;
    si (def->definitio->liberi != NIHIL)
    {
        num = xar_numerus(def->definitio->liberi);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlNodus* corporis;
            StmlNodus* clonis;

            corporis = *(StmlNodus**)xar_obtinere(
                def->definitio->liberi, i);
            si (corporis == NIHIL)
            {
                perge;
            }
            clonis = stml_duplicare(corporis, ctx->piscina,
                                    ctx->intern);
            si (clonis == NIHIL)
            {
                redde FALSUM;
            }
            si (primus == NIHIL)
            {
                primus = clonis;
            }
            (vacuum)stml_liberum_addere(parens_novus, clonis);
        }
    }

    nota = (StmlExpansioNota*)xar_addere(
        ctx->resultus->tabula_expansionum);
    si (nota == NIHIL)
    {
        redde FALSUM;
    }
    nota->nodus          = primus;
    nota->fragmentum_id  = def->id;
    nota->vocatio        = vocatio;
    nota->stratum        = I;
    redde VERUM;
}

/* Ambulatio expansionis: superficialiter duplicare, liberos
 * recursive. Ambulatio iterationem liberorum POSSIDET quia
 * fragmenta (demissa) et vocationes (impletae) in EA
 * intercipiuntur - vocatio liberos plures parit, ergo interceptio
 * in ansa liberorum vivit, non in casu nodi. Cave:
 * stml_duplicare_superficialiter liberos NIHIL relinquit - Xar
 * liberorum hic creatur. */
interior StmlNodus*
_expandere_nodum (
             StmlNodus* nodus,
    StmlMacroContextus* ctx)
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
            StmlNodus* liberum_novum;

            liberum = *(StmlNodus**)xar_obtinere(nodus->liberi, i);
            si (liberum == NIHIL)
            {
                perge;
            }
            si (_est_definitio(liberum))
            {
                StmlMacroDefinitio* def;

                def = _definitionem_invenire(ctx,
                                             liberum->fragmentum_id);
                si (def != NIHIL)
                {
                    def->praeterita = VERUM;
                }
                perge;  /* demissa - visio contenti */
            }
            si (_est_vocatio(liberum))
            {
                si (!_vocationem_implere(novum, liberum, ctx))
                {
                    redde NIHIL;
                }
                perge;
            }
            liberum_novum = _expandere_nodum(liberum, ctx);
            si (liberum_novum == NIHIL)
            {
                redde NIHIL;
            }
            (vacuum)stml_liberum_addere(novum, liberum_novum);
        }
    }
    redde novum;
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

    resultus.radix_expansa = _expandere_nodum(radix, &ctx);
    si (resultus.radix_expansa == NIHIL)
    {
        redde resultus;
    }
    resultus.successus = VERUM;
    redde resultus;
}
