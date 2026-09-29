/* briar_silva.c - Vide briar_silva.h. */

#include "briar_silva.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "silva_lexicon.h"
#include <stdio.h>
#include <string.h>

interior b32
_suffixum_est (
                chorda  c,
    constans character* suffixum)
{
    i32 m = (i32)strlen(suffixum);

    redde (b32)(c.mensura >= m
        && memcmp(c.datum + (c.mensura - m), suffixum, (size_t)m)
            == ZEPHYRUM);
}

SilvaParsura*
briar_silva_parsare (
            SilvaPiscina*  arboris,
                 Piscina*  piscina,
      constans SilexFons*  fons,
            constans Xar*  clausura,
      constans character*  excludere,
      constans character*  via,
      constans character*  textus,
                     i32   mensura,
      constans character** causa)
{
    SilvaContextus* contextus;
             chorda  iso;
             chorda  px;
                b32  inventum_iso = FALSUM;
                b32  inventum_px  = FALSUM;
                i32  k;

    si (causa != NIHIL)
    {
        *causa = NIHIL;
    }
    contextus = silva_contextus_creare(arboris);
    si (contextus == NIHIL)
    {
        redde NIHIL;
    }
    /* lexicon systematis: stand-ins e fonte silicis (T16b) */
    iso = silex_fons_legere(fons, "silva/fontes/systema_c89.h", piscina,
        &inventum_iso);
    px  = silex_fons_legere(fons, "silva/fontes/systema_posix.h",
        piscina,
        &inventum_px);
    si (inventum_iso && inventum_px)
    {
        character* sys;
              i32  m_sys    = ZEPHYRUM;
              b32  fractum  = FALSUM;

        sys = silva_lexicon_componere((constans character*)iso.datum,
            iso.mensura, (constans character*)px.datum, px.mensura,
            textus, mensura, FALSUM, piscina, &m_sys, via, &fractum);
        si (fractum)
        {
            si (causa != NIHIL)
            {
                *causa = "annotatio externa prava (lexicon systematis)";
            }
            redde NIHIL;
        }
        si (   sys != NIHIL
            && !silva_contextus_lexicon_addere(contextus,
            "systema_c89.h",
                   sys, (insignatus integer)m_sys))
        {
            redde NIHIL;
        }
    }
    per (k = ZEPHYRUM; clausura != NIHIL
        && k < xar_numerus(clausura); k++)
    {
        constans SilexRes* res = (constans SilexRes*)xar_obtinere(
            clausura, k);

        si (   !_suffixum_est(res->via, ".h")
            || (excludere != NIHIL
                && chorda_aequalis_literis(res->via, excludere)))
        {
            perge;
        }
        (vacuum)silva_contextus_praebere(contextus,
            chorda_ut_cstr(res->via, piscina),
            (constans character*)res->contentum.datum,
            (insignatus integer)res->contentum.mensura);
    }
    redde silva_parsare_cum_contextu(arboris, contextus, via, textus,
        (insignatus integer)mensura, &SILVA_C89_GRAMMATICA, NIHIL,
        NIHIL,
        NIHIL);
}

/* nodus ERROR primus (profunditate prima) in valore: linea silvae aut
 * 0. Ambulatio per VALORES (NODUS aut LISTA), non per
 * silva_nodus_liberi: radix commissionis parsurae fractae LISTA esse
 * potest, et liberi eam ut nodum legeret. */
interior i32
_errorem_quaerere (
    SilvaValor valor,
       integer fons_index)
{
    si (valor.genus == SILVA_VALOR_NODUS)
    {
         constans SilvaNodus* nodus = valor.datum.nodus;
          insignatus integer  k;

        si (nodus == NIHIL)
        {
            redde ZEPHYRUM;
        }
        si (nodus->genus == (integer)SILVA_C89_GENUS_ERROR)
        {
            insignatus integer la = ZEPHYRUM;
            insignatus integer ca = ZEPHYRUM;
            insignatus integer lb = ZEPHYRUM;
            insignatus integer cb = ZEPHYRUM;

            silva_nodus_extensionem_lineis(nodus, fons_index, &la, &ca,
                &lb, &cb);
            redde (i32)la;
        }
        per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
        {
            i32 linea = _errorem_quaerere(nodus->loci[k], fons_index);

            si (linea > ZEPHYRUM)
            {
                redde linea;
            }
        }
        redde ZEPHYRUM;
    }
    si (valor.genus == SILVA_VALOR_LISTA)
    {
        insignatus integer n = silva_valor_lista_numerus(valor);
        insignatus integer k;

        per (k = ZEPHYRUM; k < n; k++)
        {
            SilvaValor* e = silva_valor_lista_obtinere(valor, k);
                   i32  linea = (e != NIHIL)
                       ? _errorem_quaerere(*e, fons_index) : ZEPHYRUM;

            si (linea > ZEPHYRUM)
            {
                redde linea;
            }
        }
    }
    redde ZEPHYRUM;
}

/* tabula symbolum -> caput domus (corpus.symbola.tsv e fonte: symbolum,
 * genus, caput 'include/x.h' aut radicis clientis); ambigua = symbola in
 * capitibus duobus */
nomen structura {
    TabulaDispersa* capita;    /* symbolum -> chorda* caput ('x.h') */
    TabulaDispersa* ambigua;   /* symbolum -> chorda* caput alterum */
               b32  adest;
} BriarSymbola;

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior chorda
_columna (
    chorda linea,
       i32 quota)
{
    i32 i;
    i32 initium  = ZEPHYRUM;
    i32 n        = ZEPHYRUM;

    per (i = ZEPHYRUM; i <= linea.mensura; i++)
    {
        si (i == linea.mensura || (character)linea.datum[i] == '\t')
        {
            si (n == quota)
            {
                redde chorda_sectio(linea, initium, i);
            }
            n        = n + I;
            initium  = i + I;
        }
    }
    redde _vacua();
}

interior BriarSymbola
_symbola_legere (
               Piscina* piscina,
    constans SilexFons* fons)
{
    BriarSymbola t;
             b32 inventum = FALSUM;
          chorda textus;
             i32 i = ZEPHYRUM;

    t.capita   = tabula_dispersa_creare_chorda(piscina, 8192);
    t.ambigua  = tabula_dispersa_creare_chorda(piscina, 64);
    t.adest    = FALSUM;
    textus = silex_fons_legere(fons, "corpus.symbola.tsv", piscina,
        &inventum);
    si (!inventum || t.capita == NIHIL || t.ambigua == NIHIL)
    {
        redde t;
    }
    t.adest = VERUM;
    dum (i < textus.mensura)
    {
           i32 f = i;
        chorda linea;

        dum (f < textus.mensura && (character)textus.datum[f] != '\n')
        {
            f = f + I;
        }
        linea  = chorda_sectio(textus, i, f);
        i      = f + I;
        si (   linea.mensura             == ZEPHYRUM
            || (character)linea.datum[0] == '#')
        {
            perge;
        }
        {
            chorda  symbolum  = _columna(linea, ZEPHYRUM);
            chorda  caput     = _columna(linea, II);
            chorda* cella;
            vacuum* prior = NIHIL;

                        si (   symbolum.mensura == ZEPHYRUM
                            || caput.mensura    == ZEPHYRUM)
                        {
                perge;
                        }
            /* ordines capitum soli: 'include/x.h' aut radix clientis
             * + 'x.h' (toml Q13: toml/fontes/toml.h) -> 'x.h'; ordines
             * fontium (statica, pro -amalgama) hic praetereuntur */
            si (!silex_titulus_capitis(caput, &caput))
            {
                perge;
            }
            si (tabula_dispersa_invenire(t.capita, symbolum, &prior))
            {
                si (!chorda_aequalis(*(chorda*)prior, caput))
                {
                    cella = (chorda*)piscina_allocare(piscina,
                        (memoriae_index)magnitudo(chorda));
                    *cella = caput;
                    tabula_dispersa_inserere(t.ambigua, symbolum,
                        (vacuum*)cella);
                }
                perge;
            }
            cella = (chorda*)piscina_allocare(piscina,
                (memoriae_index)magnitudo(chorda));
            *cella = caput;
            tabula_dispersa_inserere(t.capita, symbolum,
                (vacuum*)cella);
        }
    }
    redde t;
}

interior chorda
_silva_chorda_ut_chorda (
        Piscina* piscina,
    SilvaChorda  s)
{
    chorda c;

    c.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)(s.mensura + I));
    memcpy(c.datum, s.datum, (size_t)s.mensura);
    c.mensura = (i32)s.mensura;
    redde c;
}

interior chorda
_detondere (
    chorda c)
{
    dum (   c.mensura > ZEPHYRUM
         && ((character)c.datum[c.mensura - I] == ' '
        || (character)c.datum[c.mensura - I] == '\n'
        || (character)c.datum[c.mensura - I] == '\t'))
    {
        c.mensura = c.mensura - I;
    }
    dum (   c.mensura > ZEPHYRUM && ((character)c.datum[0] == ' '
        || (character)c.datum[0] == '\n'
        || (character)c.datum[0] == '\t'))
    {
        c = chorda_sectio(c, I, c.mensura);
    }
    redde c;
}

/* regio lineam '#include "x.h"' ipsa fert? */
interior b32
_regio_includit (
                   Piscina* piscina,
    constans BriarNexusRes* r,
                    chorda  caput)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)128);
    constans character* acus;
    constans character* fenum;

    chorda_aedificator_appendere_literis(aed, "#include \"");
    chorda_aedificator_appendere_chorda(aed, caput);
    chorda_aedificator_appendere_literis(aed, "\"");
        acus  = chorda_ut_cstr(chorda_aedificator_finire(aed),
            piscina);
    fenum     = chorda_ut_cstr(r->contextus, piscina);
    redde (b32)(strstr(fenum, acus) != NIHIL);
}

/* caput symboli in capita addere (dedup); ambiguum -> FALSUM + causa */
interior b32
_caput_addere (
           Piscina* piscina,
      BriarSymbola* t,
     BriarNexusRes* r,
            chorda  symbolum,
               Xar* capita,
    TabulaDispersa* visa,
    TabulaDispersa* definita,
               Xar* pares)
{
    vacuum* caput    = NIHIL;
    vacuum* alterum  = NIHIL;

    si (!tabula_dispersa_invenire(t->capita, symbolum, &caput))
    {
        redde VERUM;   /* ignotum tabulae: clang nominabit */
    }
    /* symbolum quod regio ULLA plagulae declarat (aut caput quod ipsa
     * includit) per <t>_regiones.h omnibus praesto est: numquam
     * derivatur - 'Punctum' regionis punctum.thistle ET mandatum.h
     * (inventum 2026-09-05, post fusionem ludi) */
    si (   definita != NIHIL
        && tabula_dispersa_continet(definita, symbolum))
    {
        redde VERUM;
    }
    /* caput quod regio ipsa includit numquam derivatur (redundantia
     * in capite genito; vide worklog 2026-09-05 de parsura prima) */
    si (_regio_includit(piscina, r, *(chorda*)caput))
    {
        redde VERUM;
    }
    si (tabula_dispersa_invenire(t->ambigua, symbolum, &alterum))
    {
        ChordaAedificator* aed = chorda_aedificator_creare(piscina,
            (memoriae_index)256);

        chorda_aedificator_appendere_literis(aed, "symbolum '");
        chorda_aedificator_appendere_chorda(aed, symbolum);
        chorda_aedificator_appendere_literis(aed,
            "' in capitibus duobus: ");
        chorda_aedificator_appendere_chorda(aed, *(chorda*)caput);
        chorda_aedificator_appendere_literis(aed, ", ");
        chorda_aedificator_appendere_chorda(aed, *(chorda*)alterum);
        chorda_aedificator_appendere_literis(aed,
            " - inclusionem ipse scribe");
        r->causa          = chorda_aedificator_finire(aed);
        r->linea_erroris  = r->linea_initium - I;
        redde FALSUM;
    }
    /* par symboli servatur ANTE plicationem capitum: capita unica
     * sunt, symbola multa idem caput habent */
    si (pares != NIHIL)
    {
        BriarSymbolumDerivatum* par = (BriarSymbolumDerivatum*)
            xar_addere(pares);

        si (par != NIHIL)
        {
            par->titulus  = symbolum;
            par->caput    = *(chorda*)caput;
        }
    }
    si (!tabula_dispersa_continet(visa, *(chorda*)caput))
    {
        chorda* cella = (chorda*)xar_addere(capita);

        *cella = *(chorda*)caput;
        tabula_dispersa_inserere(visa, *(chorda*)caput, caput);
    }
    redde VERUM;
}

interior vacuum
_capita_ordinare (
    Xar* capita)
{
    s32 n = (s32)xar_numerus(capita);
    s32 i;

    per (i = I; i < n; i++)
    {
        chorda clavis  = *(chorda*)xar_obtinere(capita, (i32)i);
           s32 j       = i - I;

        dum (j >= ZEPHYRUM)
        {
            chorda* sedes = (chorda*)xar_obtinere(capita, (i32)j);
               i32  m = sedes->mensura < clavis.mensura ? sedes->mensura
                   : clavis.mensura;
            s32 ordo = (s32)memcmp(sedes->datum, clavis.datum,
                (size_t)m);

            si (   ordo < ZEPHYRUM || (ordo == ZEPHYRUM
                && sedes->mensura <= clavis.mensura))
            {
                frange;
            }
            *(chorda*)xar_obtinere(capita, (i32)(j + I))  = *sedes;
            j                                             = j - I;
        }
        *(chorda*)xar_obtinere(capita, (i32)(j + I)) = clavis;
    }
}

/* symbola implicita + typi ignoti parsurae primae -> capita derivata;
 * FALSUM = ambiguum (causa posita) */
interior b32
_capita_derivare (
          Piscina* piscina,
     BriarSymbola* t,
    BriarNexusRes* r,
       BriarSilva* arbor_silvae,
              Xar* capita,
   TabulaDispersa* definita,
              Xar* pares)
{
        TabulaDispersa* visa;
    insignatus integer  k;
               integer  fons_index =
                   arbor_silvae->parsura->fons_princeps;

    visa = tabula_dispersa_creare_chorda(piscina, 32);
    per (k = ZEPHYRUM; k
        < silva_c89_symbola_numerus(arbor_silvae->semantica); k++)
    {
        constans SemanticaSymbolum* s = silva_c89_symbolum_per_indicem(
            arbor_silvae->semantica, k);

                si (   s->est_implicitum
                    && !_caput_addere(piscina, t, r,
                    _silva_chorda_ut_chorda(piscina, s->titulus),
                    capita, visa,
                    definita, pares))
                {
            redde FALSUM;
                }
    }
    per (k = ZEPHYRUM; k
        < silva_c89_diagnostica_numerus(arbor_silvae->semantica);
        k++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(
            arbor_silvae->semantica, k);
        integer minimum = -I;
        integer maximum = ZEPHYRUM;

        si (   d->codex != (integer)EXAMEN_CODEX_TYPUS_NOMINATUS_IGNOTUS
            || d->nodus == NIHIL)
        {
            perge;
        }
        silva_nodus_extensionem(d->nodus, fons_index, &minimum,
            &maximum);
        si (minimum < ZEPHYRUM || maximum <= minimum)
        {
            perge;
        }
                si (!_caput_addere(piscina, t, r,
                    _detondere(chorda_sectio(
                    r->textus_silvae, (i32)minimum, (i32)maximum)),
                    capita,
                    visa, definita, pares))
                {
            redde FALSUM;
                }
    }
    _capita_ordinare(capita);
    redde VERUM;
}

/* SilvaLineaMappatio: linea textus silvae -> linea .thistle
 * (silva_mortes_scribere, T19b-1) */
interior i32
_lineam_thistle (
    vacuum* datum,
       i32  linea)
{
    redde briar_nexus_linea_silvae((constans BriarNexusRes*)datum,
        linea);
}

/* parsura una: praeludium (latina + trias + capita derivata + exemplar
 * si methodus) + contentum; r->silva ponitur; FALSUM = memoria */
/* textum in praeludium addere, lineas numerare (linea finalis) */
interior vacuum
_praeludium_augere (
    ChordaAedificator* aed,
        BriarNexusRes* r,
               chorda  t)
{
    i32 j;

    chorda_aedificator_appendere_chorda(aed, t);
    per (j = ZEPHYRUM; j < t.mensura; j++)
    {
        si (t.datum[j] == '\n')
        {
            r->praeludium = r->praeludium + I;
        }
    }
    si (t.mensura > ZEPHYRUM && t.datum[t.mensura - I] != '\n')
    {
        chorda_aedificator_appendere_literis(aed, "\n");
        r->praeludium = r->praeludium + I;
    }
}

interior b32
_parsare (
                      Piscina* piscina,
                BriarNexusRes* r,
           constans SilexFons* fons,
                          Xar* capita,
    constans BriarSilvaMembra* membra)
{
     ChordaAedificator* aed;
                   Xar* clausura;
    constans character* causa_lexici = NIHIL;
            BriarSilva* arbor_silvae;
                   i32  k;
        TabulaDispersa* inserta;

        aed = chorda_aedificator_creare(piscina,
            (memoriae_index)(r->contextus.mensura + 256));
    si (aed == NIHIL)
    {
        redde FALSUM;
    }
    r->silva          = NIHIL;
    r->causa          = _vacua();
    r->linea_erroris  = ZEPHYRUM;
    chorda_aedificator_appendere_literis(aed,
        "#include \"latina.h\"\n"
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "#include <string.h>\n");
    r->praeludium = IV;
    inserta = (membra != NIHIL && membra->textus != NIHIL)
        ? tabula_dispersa_creare_chorda(piscina, 16) : NIHIL;
    per (k = ZEPHYRUM; capita != NIHIL && k < xar_numerus(capita); k++)
    {
        chorda  caput  = *(chorda*)xar_obtinere(capita, k);
        vacuum* ordo   = NIHIL;

        /* caput MEMBRI (bibliotheca): textus proprii dependentium
         * (post-ordo) deinde eius, quisque SEMEL (rhombus sine
         * custodibus) - non inclusio: silex caput genitum non
         * invenit; lineae in praeludium numerantur */
        si (   inserta != NIHIL
            && tabula_dispersa_continet(membra->textus, caput))
        {
            Xar* dependentia = NIHIL;
            s32  d;

            si (tabula_dispersa_invenire(membra->ordo, caput, &ordo))
            {
                dependentia = (Xar*)ordo;
            }
            per (d = ZEPHYRUM; d <= (s32)(dependentia != NIHIL
                ? xar_numerus(dependentia) : ZEPHYRUM); d++)
            {
                chorda quod = (dependentia != NIHIL
                    && d < (s32)xar_numerus(dependentia))
                    ? *(chorda*)xar_obtinere(dependentia,
                    (i32)d) : caput;
                vacuum* t = NIHIL;

                si (   tabula_dispersa_continet(inserta, quod)
                    || !tabula_dispersa_invenire(membra->textus, quod,
                           &t))
                {
                    perge;
                }
                tabula_dispersa_inserere(inserta, quod, t);
                _praeludium_augere(aed, r, *(chorda*)t);
            }
            perge;
        }
        chorda_aedificator_appendere_literis(aed, "#include \"");
        chorda_aedificator_appendere_chorda(aed, caput);
        chorda_aedificator_appendere_literis(aed, "\"\n");
        r->praeludium = r->praeludium + I;
    }
    si (briar_nexus_attributum_habet(r, "methodus"))
    {
        chorda_aedificator_appendere_literis(aed,
            "#include \"internuntius.h\"\n"
            "hic_manens InternuntiusTractator"
            " briar_tractator_exemplar;\n");
        r->praeludium = r->praeludium + II;
    }
        r->praeludium_octeti = (i32)chorda_aedificator_longitudo(aed);
    /* contextus (briar_contextus: fragmenta contexta), non contentum */
    chorda_aedificator_appendere_chorda(aed, r->contextus);
    r->textus_silvae = chorda_aedificator_finire(aed);

    clausura = silex_clausuram_e_contentis(piscina, fons,
        &r->textus_silvae, I);
    si (clausura == NIHIL)
    {
        r->causa = chorda_ex_literis(
            "clausura capitum fracta (fons silicis)", piscina);
        r->linea_erroris = r->linea_initium;
        redde VERUM;
    }
    arbor_silvae = (BriarSilva*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(BriarSilva));
    si (arbor_silvae == NIHIL)
    {
        redde FALSUM;
    }
    arbor_silvae->parsura           = NIHIL;
    arbor_silvae->semantica         = NIHIL;
    arbor_silvae->capita_derivata   = NIHIL;
    arbor_silvae->symbola_derivata  = NIHIL;
    arbor_silvae->piscina   =
        silva_piscina_generare_dynamicum("briar_silva",
        (size_t)8388608);
    si (arbor_silvae->piscina == NIHIL)
    {
        redde FALSUM;
    }
    r->silva  = arbor_silvae;
    arbor_silvae->parsura = briar_silva_parsare(arbor_silvae->piscina,
        piscina, fons, clausura, NIHIL, "regio.c",
        (constans character*)r->textus_silvae.datum,
        (i32)r->textus_silvae.mensura, &causa_lexici);
    si (   arbor_silvae->parsura            == NIHIL
        || arbor_silvae->parsura->commissio == NIHIL)
    {
        arbor_silvae->parsura = NIHIL;
        r->causa = chorda_ex_literis(causa_lexici != NIHIL
            ? causa_lexici : "silva: parsura fracta", piscina);
        r->linea_erroris = r->linea_initium;
        redde VERUM;
    }
    si (arbor_silvae->parsura->numerus_errorum > ZEPHYRUM)
    {
        i32 linea =
            _errorem_quaerere(arbor_silvae->parsura->commissio->radix,
            arbor_silvae->parsura->fons_princeps);
                      i32  linea_mortis = ZEPHYRUM;
                character  b[96];
              SilvaChorda  loca;
        ChordaAedificator* causa;

        /* lapide bugs/001: nodus ERROR initium UNITATIS fert (linea
         * prima regionis semper) - mors GLR locum verum dat. Forma
         * UNA silvae (silva_mortes_scribere, T19b-1): ordines
         * 'via:linea:columna' + excerpta, lineae .thistle per
         * briar_nexus_linea_silvae. */
        loca = silva_mortes_scribere(arbor_silvae->piscina,
            r->via_documenti != NIHIL ? r->via_documenti : "regio",
            arbor_silvae->parsura, &SILVA_C89_REGISTRUM,
            (constans character*)r->textus_silvae.datum,
            (i32)r->textus_silvae.mensura, FALSUM, _lineam_thistle,
            (vacuum*)r, &linea_mortis);
        r->linea_erroris = (linea_mortis > ZEPHYRUM) ? linea_mortis
            : (linea > ZEPHYRUM) ? briar_nexus_linea_silvae(r, linea)
            : r->linea_initium;
        sprintf(b, "regio C: parsura fracta (%u %s)",
            arbor_silvae->parsura->numerus_errorum,
            arbor_silvae->parsura->numerus_errorum == I
                ? "error" : "errores");
        causa = chorda_aedificator_creare(piscina, CCLVI);
        si (causa == NIHIL)
        {
            redde FALSUM;
        }
        chorda_aedificator_appendere_literis(causa, b);
        /* ordines locati sub summario (sine linea nova ultima -
         * vocans lineam suam addit) */
        si (loca.mensura > ZEPHYRUM)
        {
            chorda sectio;

            sectio.datum    = (i8*)loca.datum;
            sectio.mensura  = (i32)loca.mensura;
            si (sectio.datum[sectio.mensura - I] == '\n')
            {
                sectio.mensura = sectio.mensura - I;
            }
            chorda_aedificator_appendere_character(causa, '\n');
            chorda_aedificator_appendere_chorda(causa, sectio);
        }
        r->causa = chorda_aedificator_finire(causa);
    }
    arbor_silvae->semantica =
        silva_c89_semantica_analysare(arbor_silvae->piscina,
        arbor_silvae->parsura);
    redde VERUM;
}

/* nomina scopi plagulae in ULLA regione C declarata post parsuram
 * primam (regio ipsa aut capita quae ipsa includit; implicita
 * exclusa) - vide _caput_addere */
interior TabulaDispersa*
_definita_colligere (
    Piscina* piscina,
        Xar* nexus)
{
    TabulaDispersa* t = tabula_dispersa_creare_chorda(piscina, 512);
               i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
             BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);
        insignatus integer  k;

        /* probatio exclusa: eius inclusiones in caput genitum NON
         * fluunt (directivae regionum app solae), ergo symbola per
         * eas declarata radicibus app ignota manent - credo.h
         * piscina.h trahit, app piscina.h derivare debet (inventum
         * fragmenta.thistle 2026-09-05) */
        si (   r->genus != BRIAR_NEXUS_REGIO
            || !briar_nexus_titulus_est(r, "c") || r->est_fragmentum
            || chorda_aequalis_literis(briar_nexus_attributum(r,
                "munus"), "probatio")
            || r->silva == NIHIL || r->silva->semantica == NIHIL)
        {
            perge;
        }
        per (k = ZEPHYRUM; k
            < silva_c89_symbola_numerus(r->silva->semantica); k++)
        {
            constans SemanticaSymbolum* s =
                silva_c89_symbolum_per_indicem(r->silva->semantica, k);
            chorda titulus;

            si (   s->est_implicitum
                || s->profunditas != (insignatus integer)ZEPHYRUM)
            {
                perge;
            }
            titulus = _silva_chorda_ut_chorda(piscina, s->titulus);
            si (!tabula_dispersa_continet(t, titulus))
            {
                tabula_dispersa_inserere(t, titulus, (vacuum*)r);
            }
        }
    }
    redde t;
}

/* regio post parsuram primam: capita derivata (tabula adsit),
 * parsura secunda */
interior b32
_regionem_derivare (
                      Piscina* piscina,
                BriarNexusRes* r,
           constans SilexFons* fons,
                 BriarSymbola* symbola,
               TabulaDispersa* definita,
    constans BriarSilvaMembra* membra)
{
    Xar* capita = xar_creare(piscina, (i32)magnitudo(chorda));
    Xar* pares  = xar_creare(piscina,
        (i32)magnitudo(BriarSymbolumDerivatum));

    si (   r->silva            == NIHIL || r->silva->parsura == NIHIL
        || r->silva->semantica == NIHIL || !symbola->adest)
    {
        si (r->silva != NIHIL)
        {
            r->silva->capita_derivata   = capita;
            r->silva->symbola_derivata  = pares;
        }
        redde VERUM;
    }
    si (!_capita_derivare(piscina, symbola, r, r->silva, capita,
        definita, pares))
    {
        r->silva->capita_derivata   = capita;
        r->silva->symbola_derivata  = pares;
        redde VERUM;   /* ambiguum: causa + linea posita, arbor manet */
    }
    si (xar_numerus(capita) > ZEPHYRUM)
    {
        silva_piscina_destruere(r->silva->piscina);
        si (!_parsare(piscina, r, fons, capita, membra))
        {
            redde FALSUM;
        }
    }
    si (r->silva != NIHIL)
    {
        r->silva->capita_derivata   = capita;
        r->silva->symbola_derivata  = pares;
    }
    redde VERUM;
}

s32
briar_silvam_texere (
                Piscina* piscina,
                    Xar* nexus,
     constans SilexFons* fons,
     constans character* via_documenti)
{
    redde briar_silvam_texere_cum_membris(piscina, nexus, fons,
        via_documenti, NIHIL);
}

/* nomina membrorum in tabulam symbolorum: absens -> capita (caput
 * membri); iam corporis -> ambigua (derivatio ut duo capita
 * refutat, linea regionis) */
interior vacuum
_membra_miscere (
                      Piscina* piscina,
                 BriarSymbola* t,
    constans BriarSilvaMembra* membra)
{
    TabulaIterator  it;
            chorda  titulus;
            vacuum* caput;

    si (membra == NIHIL || membra->nomina == NIHIL)
    {
        redde;
    }
    si (t->capita == NIHIL)
    {
        t->capita   = tabula_dispersa_creare_chorda(piscina, 64);
        t->ambigua  = tabula_dispersa_creare_chorda(piscina, 16);
    }
    it = tabula_dispersa_iterator_initium(membra->nomina);
    dum (tabula_dispersa_iterator_proximum(&it, &titulus, &caput))
    {
        si (tabula_dispersa_continet(t->capita, titulus))
        {
            tabula_dispersa_inserere(t->ambigua, titulus, caput);
        }
        alioquin
        {
            tabula_dispersa_inserere(t->capita, titulus, caput);
        }
    }
    t->adest = VERUM;
}

s32
briar_silvam_texere_cum_membris (
                      Piscina* piscina,
                          Xar* nexus,
           constans SilexFons* fons,
           constans character* via_documenti,
    constans BriarSilvaMembra* membra)
{
                          i32  i;
                          s32  numerus = ZEPHYRUM;
                 BriarSymbola  symbola;
               TabulaDispersa* definita;

    si (piscina == NIHIL || nexus == NIHIL || fons == NIHIL)
    {
        redde -I;
    }
    symbola = _symbola_legere(piscina, fons);
    _membra_miscere(piscina, &symbola, membra);
    /* parsura prima omnium regionum, deinde derivatio: symbola quae
     * regio ulla declarat nulli derivantur */
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

                        si (   r->genus != BRIAR_NEXUS_REGIO
                            || !briar_nexus_titulus_est(r, "c")
                            || r->est_fragmentum
                            || r->linea_erroris > ZEPHYRUM)
                        {
            perge;   /* fragmenta numquam parsantur (in radicibus);
                      * radix a contextu recusata recusata manet */
                        }
        r->via_documenti = via_documenti;
        si (!_parsare(piscina, r, fons, NIHIL, NIHIL))
        {
            redde -I;
        }
        numerus = numerus + I;
    }
    definita = _definita_colligere(piscina, nexus);
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

                        si (   r->genus != BRIAR_NEXUS_REGIO
                            || !briar_nexus_titulus_est(r, "c")
                            || r->est_fragmentum
                            || r->silva == NIHIL)
                        {
            perge;
                        }
        si (!_regionem_derivare(piscina, r, fons, &symbola, definita,
                membra))
        {
            redde -I;
        }
    }
    redde numerus;
}

vacuum
briar_silvam_solvere (
    Xar* nexus)
{
    i32 i;

    si (nexus == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

        si (r->silva != NIHIL && r->silva->piscina != NIHIL)
        {
            silva_piscina_destruere(r->silva->piscina);
        }
        r->silva = NIHIL;
    }
}

BriarSilva*
briar_silvam_capitis_texere (
               Piscina* piscina,
    constans SilexFons* fons,
    constans character* via,
                chorda* textus)
{
              b32  inventum = FALSUM;
           chorda  contentum;
              Xar* clausura;
       BriarSilva* silva;

    textus->datum    = NIHIL;
    textus->mensura  = ZEPHYRUM;
    contentum        = silex_fons_legere(fons, via, piscina, &inventum);
    si (!inventum)
    {
        redde NIHIL;
    }
    /* clausura ex '#include "x.h"' capitis ipsius (e fonte silicis) */
    clausura = silex_clausuram_e_contentis(piscina, fons, &contentum,
        I);
    si (clausura == NIHIL)
    {
        redde NIHIL;
    }
    silva = (BriarSilva*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(BriarSilva));
    si (silva == NIHIL)
    {
        redde NIHIL;
    }
    silva->parsura           = NIHIL;
    silva->semantica         = NIHIL;
    silva->capita_derivata   = NIHIL;
    silva->symbola_derivata  = NIHIL;
    silva->piscina =
        silva_piscina_generare_dynamicum("briar_silva_caput",
        (size_t)8388608);
    si (silva->piscina == NIHIL)
    {
        redde NIHIL;
    }
    /* caput ipsum PRINCIPALE est, non includendum ('excludere') */
    silva->parsura = briar_silva_parsare(silva->piscina, piscina, fons,
        clausura, via, via, (constans character*)contentum.datum,
        (i32)contentum.mensura, NIHIL);
    si (silva->parsura == NIHIL || silva->parsura->commissio == NIHIL)
    {
        briar_silvam_capitis_solvere(silva);
        redde NIHIL;
    }
    silva->semantica = silva_c89_semantica_analysare(silva->piscina,
        silva->parsura);
    *textus = contentum;
    redde silva;
}

vacuum
briar_silvam_capitis_solvere (
    BriarSilva* silva)
{
    si (silva != NIHIL && silva->piscina != NIHIL)
    {
        silva_piscina_destruere(silva->piscina);
        silva->piscina = NIHIL;
    }
}

/* nomina scopi fili regionum planarum: statica VERUM = interiora
 * sola, FALSUM = publica sola (vide briar_silva.h) */
interior Xar*
_nomina_scopi_fili (
    Piscina* piscina,
        Xar* nexus,
        b32  statica)
{
               Xar* nomina = xar_creare(piscina,
                   (i32)magnitudo(BriarNomenPublicum));
    TabulaDispersa* visa   = tabula_dispersa_creare_chorda(piscina, 64);
               i32  i;

    per (i = ZEPHYRUM; nexus != NIHIL && i < xar_numerus(nexus); i++)
    {
        constans BriarNexusRes* r = (constans BriarNexusRes*)
            xar_obtinere(nexus, i);
             insignatus integer k;
                        integer fons_index;

        si (   !briar_nexus_regio_plana(r) || r->silva == NIHIL
            || r->silva->semantica == NIHIL
            || r->silva->parsura   == NIHIL)
        {
            perge;
        }
        fons_index = r->silva->parsura->fons_princeps;
        per (k = ZEPHYRUM; k
            < silva_c89_symbola_numerus(r->silva->semantica); k++)
        {
            constans SemanticaSymbolum* s =
                silva_c89_symbolum_per_indicem(r->silva->semantica, k);
                        integer minimum = -I;
                        integer maximum = ZEPHYRUM;
                         chorda titulus;
                            i32 linea = I;
                            i32 j;
                            b32 staticum = (b32)((s->repositio
                                & REPOSITIO_STATICA) != ZEPHYRUM);

            si (   s->est_implicitum || s->ex_systemate
                || s->profunditas != (insignatus integer)ZEPHYRUM
                || staticum       != statica
                || s->genus       == (s32)SYMBOLUM_PARAMETRUM
                || s->declarans   == NIHIL)
            {
                perge;
            }
            /* in textu regionis ipso, non in praeludio (capita) */
            silva_nodus_extensionem(s->declarans, fons_index, &minimum,
                &maximum);
            si (   minimum < ZEPHYRUM
                || minimum < (integer)r->praeludium_octeti)
            {
                perge;
            }
            titulus = _silva_chorda_ut_chorda(piscina, s->titulus);
            si (   chorda_aequalis_literis(titulus, "main")
                || tabula_dispersa_continet(visa, titulus))
            {
                perge;
            }
            per (j = ZEPHYRUM; j < (i32)minimum
                && j < r->textus_silvae.mensura; j++)
            {
                si (r->textus_silvae.datum[j] == '\n')
                {
                    linea = linea + I;
                }
            }
            tabula_dispersa_inserere(visa, titulus, (vacuum*)nomina);
            {
                BriarNomenPublicum* n = (BriarNomenPublicum*)
                    xar_addere(nomina);

                n->titulus  = titulus;
                n->linea    = briar_nexus_linea_silvae(r, linea);
            }
        }
    }
    redde nomina;
}

Xar*
briar_silva_nomina_publica (
    Piscina* piscina,
        Xar* nexus)
{
    redde _nomina_scopi_fili(piscina, nexus, FALSUM);
}

Xar*
briar_silva_nomina_statica (
    Piscina* piscina,
        Xar* nexus)
{
    redde _nomina_scopi_fili(piscina, nexus, VERUM);
}
