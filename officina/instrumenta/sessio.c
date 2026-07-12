/* sessio.c - machina sessionis M4b, chunk A (vide sessio.h)
 *
 * Receptum iudicii = examen.c fidele (contextus + systema lexicon +
 * capita praebita + oraculum praeseminatum + BIS-ANALYSIS). Piscinae:
 * app (vita sessionis: turni, relata) + silvae perennis (ctx +
 * systema) + silvae effimera PER OBLATIONEM (parsurae + semantica —
 * destructa ante reditum; disciplina generationum chunk B plena).
 *
 * Duo textus (C3): OSTENSUS (quem usor typavit — artefactum) et
 * INVOLUTUS (sententiae in `void turnus_N(void){…}` — iudicatus et
 * demittendus). Tabula offset linearum per turnum mappat positiones
 * iudicatas in ostensas.
 */

#include "sessio.h"

#include "latina.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "silva.h"

#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define SESSIO_VIA_DOCUMENTI "<sessio>"

/* ==================================================
 * structurae internae
 * ================================================== */

nomen structura {
    chorda textus;       /* finitur '\n'; in piscina sessionis */
    s32    genus;        /* SESSIO_TURNUS_* */
    chorda titulus;      /* declaratum (religatio); vacua nisi */
    chorda involucrum;   /* "turnus_N" si involvendus; vacua nisi */
    i32    lineae;       /* numerus linearum textus */
} TurnusInternus;

/* visus operans per oblationem: turni exsistentes (substitutione
 * applicata) + candidatus appensus */
nomen structura {
    chorda textus;
    s32    genus;
    chorda involucrum;
    i32    lineae;
    i32    ostensum_initium;    /* linea prima ostensa (1-basata) */
    i32    involutum_initium;   /* linea prima TEXTUS in involuto */
    i32    involutum_primus;    /* linea prima spatii pleni */
    i32    involutum_ultimus;   /* linea ultima spatii pleni */
} TurnusVisus;

structura Sessio {
    Piscina*        piscina;
    SilvaPiscina*   piscina_silvae;    /* ctx + systema (perennis) */
    SilvaContextus* ctx;
    SilvaParsura*   systema_parsura;
    SilvaSemantica* systema_semantica;
    Xar*            turni;             /* TurnusInternus */
    i32             numerator;         /* involucra turnus_N */
};

/* ==================================================
 * auxilia chordarum et linearum
 * ================================================== */

interior i32
_lineas_numerare (chorda textus)
{
    i32 n = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] == (i8)'\n')
        {
            n++;
        }
    }
    redde n;
}

interior b32
_spatium_est (i8 c)
{
    redde (c == (i8)' ' || c == (i8)'\t' || c == (i8)'\n'
        || c == (i8)'\r') ? VERUM : FALSUM;
}

interior chorda
_praecisa (chorda textus)
{
    i32 a = ZEPHYRUM;
    i32 b = textus.mensura;

    dum (a < b && _spatium_est(textus.datum[a]))
    {
        a++;
    }
    dum (b > a && _spatium_est(textus.datum[b - I]))
    {
        b--;
    }
    redde chorda_sectio(textus, a, b - a);
}

/* textus turni normalizatus: praecisus + '\n' finali (copia in
 * piscinam) */
interior chorda
_normalizare (Piscina* piscina, chorda textus, b32 cum_semicolo)
{
    chorda nucleus = _praecisa(textus);
    i32 m = nucleus.mensura + (cum_semicolo ? I : ZEPHYRUM) + I;
    i8* datum = piscina_allocare(piscina, (memoriae_index)m);
    chorda fructus;

    fructus.mensura = ZEPHYRUM;
    fructus.datum = NIHIL;
    si (datum == NIHIL)
    {
        redde fructus;
    }
    memcpy(datum, nucleus.datum, (memoriae_index)nucleus.mensura);
    si (cum_semicolo)
    {
        datum[nucleus.mensura] = (i8)';';
    }
    datum[m - I] = (i8)'\n';
    fructus.datum = datum;
    fructus.mensura = m;
    redde fructus;
}

interior b32
_continet (constans i8* datum, i32 mensura, constans character* acus)
{
    i32 acus_m = (i32)strlen(acus);
    i32 k;

    si (acus_m == ZEPHYRUM || mensura < acus_m)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k + acus_m <= mensura; k++)
    {
        si (memcmp(datum + k, acus, (memoriae_index)acus_m)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * textus texere (ostensus + involutus) super visus
 * ================================================== */

interior chorda
_ostensum_texere (TurnusVisus* visus, i32 numerus, Piscina* piscina)
{
    i32 summa = ZEPHYRUM;
    i32 k;
    i8* datum;
    i32 cursor = ZEPHYRUM;
    i32 linea = I;
    chorda fructus;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        summa += visus[k].textus.mensura;
    }
    datum = piscina_allocare(piscina,
        (memoriae_index)(summa > ZEPHYRUM ? summa : I));
    fructus.mensura = ZEPHYRUM;
    fructus.datum = NIHIL;
    si (datum == NIHIL)
    {
        redde fructus;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        visus[k].ostensum_initium = linea;
        memcpy(datum + cursor, visus[k].textus.datum,
            (memoriae_index)visus[k].textus.mensura);
        cursor += visus[k].textus.mensura;
        linea += visus[k].lineae;
    }
    fructus.datum = datum;
    fructus.mensura = cursor;
    redde fructus;
}

interior b32
_involvendus (s32 genus)
{
    redde (genus == SESSIO_TURNUS_SENTENTIA
        || genus == SESSIO_TURNUS_EXPRESSIO) ? VERUM : FALSUM;
}

interior chorda
_involutum_texere (TurnusVisus* visus, i32 numerus, Piscina* piscina)
{
    i32 summa = ZEPHYRUM;
    i32 k;
    i8* datum;
    i32 cursor = ZEPHYRUM;
    i32 linea = I;
    chorda fructus;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        summa += visus[k].textus.mensura;
        si (_involvendus(visus[k].genus))
        {
            summa += visus[k].involucrum.mensura + LXIV;
        }
    }
    datum = piscina_allocare(piscina,
        (memoriae_index)(summa > ZEPHYRUM ? summa : I));
    fructus.mensura = ZEPHYRUM;
    fructus.datum = NIHIL;
    si (datum == NIHIL)
    {
        redde fructus;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        visus[k].involutum_primus = linea;
        si (_involvendus(visus[k].genus))
        {
            integer scripti = sprintf((character*)(datum + cursor),
                "void %.*s(void)\n{\n",
                (int)visus[k].involucrum.mensura,
                (constans character*)visus[k].involucrum.datum);

            cursor += (i32)scripti;
            linea += II;
            visus[k].involutum_initium = linea;
            memcpy(datum + cursor, visus[k].textus.datum,
                (memoriae_index)visus[k].textus.mensura);
            cursor += visus[k].textus.mensura;
            linea += visus[k].lineae;
            datum[cursor] = (i8)'}';
            datum[cursor + I] = (i8)'\n';
            cursor += II;
            visus[k].involutum_ultimus = linea;
            linea += I;
        }
        alioquin
        {
            visus[k].involutum_initium = linea;
            memcpy(datum + cursor, visus[k].textus.datum,
                (memoriae_index)visus[k].textus.mensura);
            cursor += visus[k].textus.mensura;
            linea += visus[k].lineae;
            visus[k].involutum_ultimus = linea - I;
        }
    }
    fructus.datum = datum;
    fructus.mensura = cursor;
    redde fructus;
}

/* mappa: linea involuta -> {index turni, linea ostensa} */
interior s32
_turnum_possidentem (constans TurnusVisus* visus, i32 numerus,
    i32 linea_involuta, i32* linea_ostensa_out)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (linea_involuta >= visus[k].involutum_primus
            && linea_involuta <= visus[k].involutum_ultimus)
        {
            i32 delta = linea_involuta - visus[k].involutum_initium;

            si (delta < ZEPHYRUM)
            {
                delta = ZEPHYRUM;
            }
            si (delta >= visus[k].lineae)
            {
                delta = visus[k].lineae > ZEPHYRUM
                    ? visus[k].lineae - I : ZEPHYRUM;
            }
            *linea_ostensa_out = visus[k].ostensum_initium + delta;
            redde (s32)k;
        }
    }
    *linea_ostensa_out = ZEPHYRUM;
    redde -I;
}

/* ==================================================
 * ambulationes arboris (amalgama)
 * ================================================== */

interior SilvaToken* _lexema_primum_valoris (SilvaValor v);

interior SilvaToken*
_lexema_primum_nodi (constans SilvaNodus* n)
{
    insignatus integer k;

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        SilvaToken* t = _lexema_primum_valoris(n->loci[k]);

        si (t != NIHIL)
        {
            redde t;
        }
    }
    redde NIHIL;
}

interior SilvaToken*
_lexema_primum_valoris (SilvaValor v)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_TOKEN:
            redde v.datum.token;
        casus SILVA_VALOR_NODUS:
            redde _lexema_primum_nodi(v.datum.nodus);
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    SilvaToken* t = _lexema_primum_valoris(*elem);

                    si (t != NIHIL)
                    {
                        redde t;
                    }
                }
            }
            redde NIHIL;
        }
        ordinarius:
            redde NIHIL;
    }
}

/* quaerere nodum generis dati (profunditate prima) */
interior constans SilvaNodus* _nodum_generis_valoris (SilvaValor v,
    integer genus);

interior constans SilvaNodus*
_nodum_generis_nodi (constans SilvaNodus* n, integer genus)
{
    insignatus integer k;

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    si (n->genus == genus)
    {
        redde n;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        constans SilvaNodus* f = _nodum_generis_valoris(n->loci[k],
            genus);

        si (f != NIHIL)
        {
            redde f;
        }
    }
    redde NIHIL;
}

interior constans SilvaNodus*
_nodum_generis_valoris (SilvaValor v, integer genus)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_NODUS:
            redde _nodum_generis_nodi(v.datum.nodus, genus);
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    constans SilvaNodus* f = _nodum_generis_valoris(
                        *elem, genus);

                    si (f != NIHIL)
                    {
                        redde f;
                    }
                }
            }
            redde NIHIL;
        }
        ordinarius:
            redde NIHIL;
    }
}

/* numerare nodos generis dati in subarbore */
interior i32 _numerare_generis_valoris (SilvaValor v, integer genus);

interior i32
_numerare_generis_nodi (constans SilvaNodus* n, integer genus)
{
    i32 summa = ZEPHYRUM;
    insignatus integer k;

    si (n == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (n->genus == genus)
    {
        summa++;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        summa += _numerare_generis_valoris(n->loci[k], genus);
    }
    redde summa;
}

interior i32
_numerare_generis_valoris (SilvaValor v, integer genus)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_NODUS:
            redde _numerare_generis_nodi(v.datum.nodus, genus);
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;
            i32 summa = ZEPHYRUM;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    summa += _numerare_generis_valoris(*elem, genus);
                }
            }
            redde summa;
        }
        ordinarius:
            redde ZEPHYRUM;
    }
}

/* linea radicis (positio fontis) lexematis primi elementi */
interior b32
_elementum_positio (constans SilvaNodus* elementum,
    integer* fons_index_out, i32* linea_out)
{
    SilvaToken* t = _lexema_primum_nodi(elementum);
    SilvaToken* radix;

    si (t == NIHIL)
    {
        redde FALSUM;
    }
    radix = silva_token_radix(t);
    si (radix == NIHIL)
    {
        radix = t;
    }
    *fons_index_out = radix->fons_index;
    *linea_out = (i32)radix->linea;
    redde VERUM;
}

/* ==================================================
 * classificatio
 * ================================================== */

interior s32
_genus_turni_ex_nodo (constans SilvaNodus* n)
{
    commutatio (n->genus)
    {
        casus SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS:
            redde SESSIO_TURNUS_DEFINITIO;
        casus SILVA_C89_GENUS_DECLARATIO:
            redde SESSIO_TURNUS_DECLARATIO;
        casus SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS:
            redde SESSIO_TURNUS_EXPRESSIO;
        ordinarius:
            redde SESSIO_TURNUS_SENTENTIA;
    }
}

/* titulus declaratus (religatio): definitio -> titulus functionis;
 * declaratio -> titulus declaratoris initiati UNICI (multi aut
 * nulli -> vacua) */
interior chorda
_titulum_declaratum (constans SilvaNodus* elementum, Piscina* piscina)
{
    constans SilvaNodus* sedes = NIHIL;
    constans SilvaNodus* titulus_nodus;
    SilvaToken* t;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;

    si (elementum->genus == SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
    {
        sedes = elementum;
    }
    alioquin si (elementum->genus == SILVA_C89_GENUS_DECLARATIO)
    {
        i32 initiati = _numerare_generis_nodi(elementum,
            SILVA_C89_GENUS_DECLARATOR_INITIATUS);

        si (initiati != I)
        {
            redde vacua;
        }
        sedes = _nodum_generis_nodi(elementum,
            SILVA_C89_GENUS_DECLARATOR_INITIATUS);
    }
    si (sedes == NIHIL)
    {
        redde vacua;
    }
    titulus_nodus = _nodum_generis_nodi(sedes,
        SILVA_C89_GENUS_DECLARATOR_TITULUS);
    si (titulus_nodus == NIHIL)
    {
        redde vacua;
    }
    t = _lexema_primum_nodi(titulus_nodus);
    si (t == NIHIL || t->valor.datum == NIHIL)
    {
        redde vacua;
    }
    {
        chorda cruda;

        cruda.datum = (i8*)(vacuum*)(memoriae_index)(constans vacuum*)
            t->valor.datum;
        cruda.mensura = (i32)t->valor.mensura;
        redde chorda_transcribere(cruda, piscina);
    }
}

/* ==================================================
 * receptum iudicii (examen fidele)
 * ================================================== */

interior SilvaSemantica*
_iudicare (Sessio* s, SilvaPiscina* effimera, constans character* fons,
    i32 mensura, SilvaParsura** parsura_out)
{
    SilvaOraculum* oraculum;
    SilvaParsura* parsura;
    SilvaSemantica* sem;

    *parsura_out = NIHIL;
    oraculum = silva_oraculum_creare(effimera);
    si (oraculum != NIHIL && s->systema_semantica != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(
            s->systema_semantica, oraculum);
    }
    parsura = silva_c89_parsare_cum_contextu(effimera, s->ctx,
        SESSIO_VIA_DOCUMENTI, fons, (insignatus integer)mensura,
        oraculum);
    si (parsura == NIHIL || !parsura->successus
        || parsura->commissio == NIHIL)
    {
        redde NIHIL;
    }
    sem = silva_c89_semantica_analysare_cum_systemate(effimera,
        parsura, s->systema_parsura);
    si (sem != NIHIL && oraculum != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(sem, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        (vacuum)silva_recanonicare(parsura->commissio, oraculum,
            silva_c89_resolutor, NIHIL);
        sem = silva_c89_semantica_analysare_cum_systemate(effimera,
            parsura, s->systema_parsura);
    }
    *parsura_out = parsura;
    redde sem;
}

/* ==================================================
 * diagnostica colligere
 * ================================================== */

nomen structura {
    Xar* xar;            /* SessioDiagnosticum */
    b32  reice;
} Collectio;

interior chorda
_chordam_ex_silva (constans SilvaChorda* sc, Piscina* piscina)
{
    chorda cruda;

    si (sc == NIHIL || sc->datum == NIHIL || sc->mensura == ZEPHYRUM)
    {
        cruda.mensura = ZEPHYRUM;
        cruda.datum = NIHIL;
        redde cruda;
    }
    cruda.datum = (i8*)(vacuum*)(memoriae_index)(constans vacuum*)
        sc->datum;
    cruda.mensura = (i32)sc->mensura;
    redde chorda_transcribere(cruda, piscina);
}

interior b32
_via_documenti_est (constans SilvaChorda* via)
{
    constans character* exspectata = SESSIO_VIA_DOCUMENTI;
    memoriae_index m = strlen(exspectata);

    si (via == NIHIL || via->datum == NIHIL
        || (memoriae_index)via->mensura != m)
    {
        redde FALSUM;
    }
    redde memcmp(via->datum, exspectata, m) == ZEPHYRUM
        ? VERUM : FALSUM;
}

interior vacuum
_diagnosticum_addere (Collectio* c, Piscina* piscina, chorda via,
    i32 linea, i32 columna, s32 severitas, s32 codex,
    b32 provisionale, b32 extra_turnum, s32 turnus_vetus,
    constans character* nuntius)
{
    SessioDiagnosticum* d = xar_addere(c->xar);

    si (d == NIHIL)
    {
        redde;
    }
    d->via = via;
    d->linea = linea;
    d->columna = columna;
    d->severitas = severitas;
    d->codex = codex;
    d->provisionale = provisionale;
    d->extra_turnum = extra_turnum;
    d->turnus_vetus = turnus_vetus;
    d->nuntius = chorda_ex_literis(nuntius, piscina);
}

/* colligere diagnostica semantica + syntaxis in visum novum
 * mappata; regula C2: acceptatio = documentum totum, ostensio =
 * intervallum novum + violationes extra (extra_turnum notatae) */
interior vacuum
_colligere (Sessio* s, Collectio* c, SilvaParsura* parsura,
    SilvaSemantica* sem, TurnusVisus* visus, i32 visus_numerus,
    i32 index_novi)
{
    chorda via_doc = chorda_ex_literis(SESSIO_VIA_DOCUMENTI,
        s->piscina);

    /* syntaxis: nodi erroris in radice */
    si (parsura->numerus_errorum > ZEPHYRUM)
    {
        SilvaValor radix = parsura->commissio->radix;
        insignatus integer n = silva_valor_lista_numerus(radix);
        insignatus integer k;

        c->reice = VERUM;
        per (k = ZEPHYRUM; k < n; k++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(radix, k);
            SilvaToken* t;
            SilvaToken* rad;
            i32 linea_ostensa = ZEPHYRUM;
            s32 possessor;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS
                || v->datum.nodus == NIHIL
                || v->datum.nodus->genus != SILVA_C89_GENUS_ERROR)
            {
                perge;
            }
            t = _lexema_primum_valoris(
                silva_c89_error_tokens(v->datum.nodus));
            si (t == NIHIL)
            {
                t = _lexema_primum_nodi(v->datum.nodus);
            }
            si (t == NIHIL)
            {
                _diagnosticum_addere(c, s->piscina, via_doc,
                    ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, -I, FALSUM,
                    FALSUM, -I, "error syntaxis");
                perge;
            }
            rad = silva_token_radix(t);
            si (rad == NIHIL)
            {
                rad = t;
            }
            possessor = _turnum_possidentem(visus, visus_numerus,
                (i32)rad->linea, &linea_ostensa);
            _diagnosticum_addere(c, s->piscina, via_doc,
                linea_ostensa, (i32)rad->columna, ZEPHYRUM, -I,
                FALSUM,
                (possessor >= ZEPHYRUM && possessor != (s32)index_novi)
                    ? VERUM : FALSUM,
                (possessor >= ZEPHYRUM && possessor != (s32)index_novi)
                    ? possessor : -I,
                "error syntaxis");
        }
    }

    /* semantica */
    {
        insignatus integer m = silva_c89_diagnostica_numerus(sem);
        insignatus integer k;

        per (k = ZEPHYRUM; k < m; k++)
        {
            constans SemanticaDiagnosticum* d =
                silva_c89_diagnosticum_per_indicem(sem, k);
            b32 violat;
            b32 in_documento;
            s32 possessor = -I;
            i32 linea_ostensa = ZEPHYRUM;
            b32 extra;

            si (d == NIHIL)
            {
                perge;
            }
            violat = (d->severitas == (int)ZEPHYRUM
                && !d->provisionale) ? VERUM : FALSUM;
            si (violat)
            {
                c->reice = VERUM;
            }
            in_documento = _via_documenti_est(&d->via);
            si (in_documento)
            {
                possessor = _turnum_possidentem(visus, visus_numerus,
                    (i32)d->linea, &linea_ostensa);
            }
            extra = (in_documento && possessor >= ZEPHYRUM
                && possessor != (s32)index_novi) ? VERUM : FALSUM;

            /* regula ostensionis: intervallum novum semper;
             * violationes ubique; cetera extra intervallum tacent
             * (re-emissiones historicae) */
            si (!violat && !(in_documento
                    && possessor == (s32)index_novi))
            {
                perge;
            }
            _diagnosticum_addere(c, s->piscina,
                in_documento ? via_doc
                    : _chordam_ex_silva(&d->via, s->piscina),
                in_documento ? linea_ostensa : (i32)d->linea,
                (i32)d->columna, (s32)d->severitas, (s32)d->codex,
                d->provisionale ? VERUM : FALSUM, extra,
                extra ? possessor : -I, d->causa);
        }
    }
}

/* ==================================================
 * capita praebere (exemplar examen/onerator)
 * ================================================== */

interior character*
_plagulam_legere (Piscina* piscina, constans character* via,
    i32* mensura_out)
{
    FILE* pl = fopen(via, "rb");
    long mensura_l;
    character* textus;

    *mensura_out = ZEPHYRUM;
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_l > 0L ? mensura_l + 1L : I));
    si (textus == NIHIL
        || (mensura_l > 0L
            && fread(textus, I, (memoriae_index)mensura_l, pl)
                != (memoriae_index)mensura_l))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    textus[mensura_l] = '\0';
    *mensura_out = (i32)mensura_l;
    redde textus;
}

interior b32
_praetermittendum (constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

interior vacuum
_capita_praeparare (Sessio* s, TabulaDispersa* visa,
    constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL)
    {
        redde;
    }
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        character via_plena[1024];
        memoriae_index m;

        si (introitus->d_name[ZEPHYRUM] == '.')
        {
            perge;
        }
        si (_praetermittendum(introitus->d_name))
        {
            perge;
        }
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);
        si (introitus->d_type == DT_DIR)
        {
            _capita_praeparare(s, visa, via_plena);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (m >= III && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I] == 'h')
            {
                chorda clavis = chorda_ex_literis(introitus->d_name,
                    s->piscina);
                character* textus;
                i32 mensura;

                si (tabula_dispersa_continet(visa, clavis))
                {
                    perge;
                }
                textus = _plagulam_legere(s->piscina, via_plena,
                    &mensura);
                si (textus == NIHIL)
                {
                    perge;
                }
                si (silva_contextus_praebere(s->ctx,
                        introitus->d_name, textus,
                        (insignatus integer)mensura))
                {
                    (vacuum)tabula_dispersa_inserere(visa, clavis,
                        NIHIL);
                }
            }
        }
    }
    closedir(dir);
}

/* ==================================================
 * API: creare / destruere
 * ================================================== */

Sessio*
sessio_creare (Piscina* piscina, constans SessioConfiguratio* cfg)
{
    Sessio* s;

    si (piscina == NIHIL || cfg == NIHIL)
    {
        redde NIHIL;
    }
    s = piscina_allocare(piscina, magnitudo(Sessio));
    si (s == NIHIL)
    {
        redde NIHIL;
    }
    memset(s, ZEPHYRUM, magnitudo(Sessio));
    s->piscina = piscina;
    s->numerator = I;
    s->turni = xar_creare(piscina, (i32)magnitudo(TurnusInternus));
    si (s->turni == NIHIL)
    {
        redde NIHIL;
    }
    s->piscina_silvae = silva_piscina_generare_dynamicum(
        "sessio_silva", 67108864);
    si (s->piscina_silvae == NIHIL)
    {
        redde NIHIL;
    }
    s->ctx = silva_contextus_creare(s->piscina_silvae);
    si (s->ctx == NIHIL)
    {
        redde NIHIL;
    }
    (vacuum)silva_contextus_latinam_addere(s->ctx);

    si (cfg->radix != NIHIL)
    {
        /* systema ISO (+POSIX si petitum) - exemplar examen */
        character via_sys[600];
        i32 m_iso = ZEPHYRUM;
        character* fons_iso;
        character* fons_sys;
        i32 mensura_sys;

        sprintf(via_sys, "%s/silva/fontes/systema_c89.h",
            cfg->radix);
        fons_iso = _plagulam_legere(piscina, via_sys, &m_iso);
        si (fons_iso == NIHIL)
        {
            redde NIHIL;
        }
        fons_sys = fons_iso;
        mensura_sys = m_iso;
        si (cfg->cum_posix)
        {
            i32 m_px = ZEPHYRUM;
            character* fons_px;

            sprintf(via_sys, "%s/silva/fontes/systema_posix.h",
                cfg->radix);
            fons_px = _plagulam_legere(piscina, via_sys, &m_px);
            si (fons_px == NIHIL)
            {
                redde NIHIL;
            }
            fons_sys = (character*)piscina_allocare(piscina,
                (memoriae_index)(m_iso + m_px + II));
            si (fons_sys == NIHIL)
            {
                redde NIHIL;
            }
            memcpy(fons_sys, fons_iso, (memoriae_index)m_iso);
            fons_sys[m_iso] = '\n';
            memcpy(fons_sys + m_iso + I, fons_px,
                (memoriae_index)m_px);
            mensura_sys = m_iso + I + m_px;
            fons_sys[mensura_sys] = '\0';
        }
        si (!silva_contextus_lexicon_addere(s->ctx, "systema_c89.h",
                fons_sys, (insignatus integer)mensura_sys))
        {
            redde NIHIL;
        }
        s->systema_parsura = silva_c89_parsare(s->piscina_silvae,
            "systema_c89.h", fons_sys,
            (insignatus integer)mensura_sys, NIHIL);
        si (s->systema_parsura == NIHIL
            || s->systema_parsura->numerus_errorum > ZEPHYRUM)
        {
            redde NIHIL;
        }
        s->systema_semantica = silva_c89_semantica_analysare(
            s->piscina_silvae, s->systema_parsura);
        si (s->systema_semantica == NIHIL)
        {
            redde NIHIL;
        }

        si (!cfg->sine_capitibus)
        {
            TabulaDispersa* visa = tabula_dispersa_creare_chorda(
                piscina, DXII);

            si (visa != NIHIL)
            {
                _capita_praeparare(s, visa, cfg->radix);
            }
        }
    }
    redde s;
}

vacuum
sessio_destruere (Sessio* s)
{
    si (s == NIHIL)
    {
        redde;
    }
    si (s->piscina_silvae != NIHIL)
    {
        silva_piscina_destruere(s->piscina_silvae);
        s->piscina_silvae = NIHIL;
    }
}

/* ==================================================
 * API: inspectio
 * ================================================== */

i32
sessio_turni_numerus (constans Sessio* s)
{
    redde s == NIHIL ? ZEPHYRUM : xar_numerus(s->turni);
}

chorda
sessio_turnus_textus (constans Sessio* s, i32 index)
{
    chorda vacua;
    TurnusInternus* t;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (s == NIHIL)
    {
        redde vacua;
    }
    t = xar_obtinere(s->turni, index);
    redde t == NIHIL ? vacua : t->textus;
}

s32
sessio_turnus_genus (constans Sessio* s, i32 index)
{
    TurnusInternus* t;

    si (s == NIHIL)
    {
        redde -I;
    }
    t = xar_obtinere(s->turni, index);
    redde t == NIHIL ? -I : t->genus;
}

chorda
sessio_turnus_nomen (constans Sessio* s, i32 index)
{
    chorda vacua;
    TurnusInternus* t;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (s == NIHIL)
    {
        redde vacua;
    }
    t = xar_obtinere(s->turni, index);
    redde t == NIHIL ? vacua : t->titulus;
}

chorda
sessio_documentum (constans Sessio* s, Piscina* piscina)
{
    i32 n;
    i32 k;
    TurnusVisus* visus;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (s == NIHIL || piscina == NIHIL)
    {
        redde vacua;
    }
    n = xar_numerus(s->turni);
    si (n == ZEPHYRUM)
    {
        redde vacua;
    }
    visus = piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(TurnusVisus));
    si (visus == NIHIL)
    {
        redde vacua;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        TurnusInternus* t = xar_obtinere(s->turni, k);

        visus[k].textus = t->textus;
        visus[k].genus = t->genus;
        visus[k].involucrum = t->involucrum;
        visus[k].lineae = t->lineae;
    }
    redde _ostensum_texere(visus, n, piscina);
}

/* ==================================================
 * API: probatio integritatis (librae) - scanner manualis
 * ================================================== */

b32
sessio_initus_completus (Sessio* s, chorda initus)
{
    i32 rotundae = ZEPHYRUM;   /* ( ) */
    i32 quadrae = ZEPHYRUM;    /* [ ] */
    i32 librae = ZEPHYRUM;     /* { } */
    b32 in_chorda = FALSUM;
    b32 in_charactere = FALSUM;
    b32 in_commento = FALSUM;
    b32 in_commento_lineae = FALSUM;
    i32 k;

    (vacuum)s;
    per (k = ZEPHYRUM; k < initus.mensura; k++)
    {
        i8 c = initus.datum[k];
        i8 proximus = (k + I < initus.mensura)
            ? initus.datum[k + I] : (i8)'\0';

        si (in_commento_lineae)
        {
            si (c == (i8)'\n')
            {
                in_commento_lineae = FALSUM;
            }
            perge;
        }
        si (in_commento)
        {
            si (c == (i8)'*' && proximus == (i8)'/')
            {
                in_commento = FALSUM;
                k++;
            }
            perge;
        }
        si (in_chorda)
        {
            si (c == (i8)'\\')
            {
                k++;
            }
            alioquin si (c == (i8)'"')
            {
                in_chorda = FALSUM;
            }
            perge;
        }
        si (in_charactere)
        {
            si (c == (i8)'\\')
            {
                k++;
            }
            alioquin si (c == (i8)'\'')
            {
                in_charactere = FALSUM;
            }
            perge;
        }
        commutatio ((integer)c)
        {
            casus '/':
                si (proximus == (i8)'*')
                {
                    in_commento = VERUM;
                    k++;
                }
                alioquin si (proximus == (i8)'/')
                {
                    in_commento_lineae = VERUM;
                    k++;
                }
                frange;
            casus '"':  in_chorda = VERUM;     frange;
            casus '\'': in_charactere = VERUM; frange;
            casus '(':  rotundae++;            frange;
            casus ')':  rotundae--;            frange;
            casus '[':  quadrae++;             frange;
            casus ']':  quadrae--;             frange;
            casus '{':  librae++;              frange;
            casus '}':  librae--;              frange;
            ordinarius:                        frange;
        }
    }
    redde (rotundae <= ZEPHYRUM && quadrae <= ZEPHYRUM
        && librae <= ZEPHYRUM && !in_chorda && !in_charactere
        && !in_commento) ? VERUM : FALSUM;
}

/* ==================================================
 * API: turnum offerre
 * ================================================== */

interior SessioRelatum
_relatum_vacuum (vacuum)
{
    SessioRelatum r;

    memset(&r, ZEPHYRUM, magnitudo(SessioRelatum));
    r.verdictum = SESSIO_APPARATUS;
    r.genus = -I;
    r.turnus_substitutus = -I;
    r.turnus_index = -I;
    redde r;
}

/* visum operans construere: turni exsistentes (cum substitutione
 * facultativa) + candidatus (si index_substituti < 0) */
interior TurnusVisus*
_visum_construere (Sessio* s, Piscina* piscina, chorda candidatus,
    s32 genus_candidati, chorda involucrum_candidati,
    s32 index_substituti, i32* numerus_out, i32* index_novi_out)
{
    i32 n = xar_numerus(s->turni);
    i32 summa = (index_substituti >= ZEPHYRUM) ? n : n + I;
    TurnusVisus* visus = piscina_allocare(piscina,
        (memoriae_index)summa * magnitudo(TurnusVisus));
    i32 k;

    si (visus == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        TurnusInternus* t = xar_obtinere(s->turni, k);

        si ((s32)k == index_substituti)
        {
            visus[k].textus = candidatus;
            visus[k].genus = genus_candidati;
            visus[k].involucrum = involucrum_candidati;
            visus[k].lineae = _lineas_numerare(candidatus);
        }
        alioquin
        {
            visus[k].textus = t->textus;
            visus[k].genus = t->genus;
            visus[k].involucrum = t->involucrum;
            visus[k].lineae = t->lineae;
        }
    }
    si (index_substituti < ZEPHYRUM)
    {
        visus[n].textus = candidatus;
        visus[n].genus = genus_candidati;
        visus[n].involucrum = involucrum_candidati;
        visus[n].lineae = _lineas_numerare(candidatus);
        *index_novi_out = n;
    }
    alioquin
    {
        *index_novi_out = (i32)index_substituti;
    }
    *numerus_out = summa;
    redde visus;
}

/* involucrum novum eligere (collisione vitata per textum ostensum) */
interior chorda
_involucrum_eligere (Sessio* s, chorda ostensum)
{
    character tentamen[XXXII];

    per (;;)
    {
        sprintf(tentamen, "turnus_%d", (int)s->numerator);
        s->numerator++;
        si (!_continet(ostensum.datum, ostensum.mensura, tentamen))
        {
            redde chorda_ex_literis(tentamen, s->piscina);
        }
    }
}

SessioRelatum
sessio_turnum_offerre (Sessio* s, chorda initus)
{
    SessioRelatum r = _relatum_vacuum();
    SilvaPiscina* effimera = NIHIL;
    Collectio collectio;
    chorda candidatus;
    chorda praecisus;
    s32 genus = -I;
    chorda titulus_decl;
    chorda involucrum;
    s32 index_substituti = -I;
    b32 reparatum = FALSUM;

    titulus_decl.mensura = ZEPHYRUM;
    titulus_decl.datum = NIHIL;
    involucrum.mensura = ZEPHYRUM;
    involucrum.datum = NIHIL;
    candidatus.mensura = ZEPHYRUM;
    candidatus.datum = NIHIL;

    si (s == NIHIL)
    {
        redde r;
    }
    collectio.xar = xar_creare(s->piscina,
        (i32)magnitudo(SessioDiagnosticum));
    collectio.reice = FALSUM;
    si (collectio.xar == NIHIL)
    {
        redde r;
    }

    praecisus = _praecisa(initus);
    si (praecisus.mensura == ZEPHYRUM)
    {
        r.verdictum = SESSIO_REIECTUM;
        _diagnosticum_addere(&collectio, s->piscina,
            chorda_ex_literis(SESSIO_VIA_DOCUMENTI, s->piscina),
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, -I, FALSUM, FALSUM, -I,
            "turnus vacuus");
        salta finis;
    }

    /* directiva: sine involucro, sine reparatione, sine
     * classificatione (elementa capitum inclusa multa licent) */
    si (praecisus.datum[ZEPHYRUM] == (i8)'#')
    {
        genus = SESSIO_TURNUS_DIRECTIVA;
        candidatus = _normalizare(s->piscina, initus, FALSUM);
        si (candidatus.datum == NIHIL)
        {
            salta finis;
        }
    }
    alioquin
    {
        /* classificatio: parsura textus OSTENSI + candidati nudi
         * (grammatica liberalis); reparatio `;` si syntaxis fracta */
        candidatus = _normalizare(s->piscina, initus, FALSUM);
        si (candidatus.datum == NIHIL)
        {
            salta finis;
        }
        effimera = silva_piscina_generare_dynamicum(
            "sessio_classificatio", 268435456);
        si (effimera == NIHIL)
        {
            salta finis;
        }
        {
            Piscina* piscina_visus = s->piscina;
            i32 visus_numerus = ZEPHYRUM;
            i32 index_novi = ZEPHYRUM;
            TurnusVisus* visus = _visum_construere(s, piscina_visus,
                candidatus, SESSIO_TURNUS_DECLARATIO, involucrum,
                -I, &visus_numerus, &index_novi);
            chorda ostensum;
            SilvaParsura* parsura = NIHIL;
            SilvaSemantica* sem;
            i32 initium_novi;

            si (visus == NIHIL)
            {
                salta finis;
            }
            ostensum = _ostensum_texere(visus, visus_numerus,
                piscina_visus);
            si (ostensum.datum == NIHIL)
            {
                salta finis;
            }
            initium_novi = visus[index_novi].ostensum_initium;
            sem = _iudicare(s, effimera,
                (constans character*)ostensum.datum,
                ostensum.mensura, &parsura);
            si (sem == NIHIL || parsura == NIHIL)
            {
                salta finis;
            }

            /* reparatio C1: syntaxis fracta + candidatus sine
             * terminatore -> tentamen cum `;` */
            si (parsura->numerus_errorum > ZEPHYRUM)
            {
                i8 ultimus = praecisus.datum[praecisus.mensura - I];

                si (ultimus != (i8)';' && ultimus != (i8)'}')
                {
                    chorda reparatus = _normalizare(s->piscina,
                        initus, VERUM);
                    TurnusVisus* visus2;
                    chorda ostensum2;
                    SilvaParsura* parsura2 = NIHIL;
                    SilvaSemantica* sem2;

                    si (reparatus.datum == NIHIL)
                    {
                        salta finis;
                    }
                    visus2 = _visum_construere(s, piscina_visus,
                        reparatus, SESSIO_TURNUS_DECLARATIO,
                        involucrum, -I, &visus_numerus, &index_novi);
                    si (visus2 == NIHIL)
                    {
                        salta finis;
                    }
                    ostensum2 = _ostensum_texere(visus2,
                        visus_numerus, piscina_visus);
                    sem2 = _iudicare(s, effimera,
                        (constans character*)ostensum2.datum,
                        ostensum2.mensura, &parsura2);
                    si (sem2 != NIHIL && parsura2 != NIHIL
                        && parsura2->numerus_errorum == ZEPHYRUM)
                    {
                        candidatus = reparatus;
                        reparatum = VERUM;
                        visus = visus2;
                        ostensum = ostensum2;
                        parsura = parsura2;
                        sem = sem2;
                        initium_novi =
                            visus[index_novi].ostensum_initium;
                    }
                }
            }

            si (parsura->numerus_errorum > ZEPHYRUM)
            {
                /* syntaxis manet fracta: REICE cum extensionibus
                 * errorum (classificatio = textus ostensus, lineae
                 * iam ostensae) */
                r.verdictum = SESSIO_REIECTUM;
                _colligere(s, &collectio, parsura, sem, visus,
                    visus_numerus, index_novi);
                collectio.reice = VERUM;
                salta finis;
            }

            /* elementa nova numerare (fons princeps + intervallum
             * novum) et genus capere */
            {
                SilvaValor radix = parsura->commissio->radix;
                insignatus integer n_rad =
                    silva_valor_lista_numerus(radix);
                insignatus integer k;
                constans SilvaNodus* elementum_novum = NIHIL;
                i32 numerus_novorum = ZEPHYRUM;

                per (k = ZEPHYRUM; k < n_rad; k++)
                {
                    SilvaValor* v = silva_valor_lista_obtinere(radix,
                        k);
                    integer fons_index;
                    i32 linea;

                    si (v == NIHIL || v->genus != SILVA_VALOR_NODUS
                        || v->datum.nodus == NIHIL)
                    {
                        perge;
                    }
                    si (!_elementum_positio(v->datum.nodus,
                            &fons_index, &linea))
                    {
                        perge;
                    }
                    si (fons_index != parsura->fons_princeps
                        || linea < initium_novi)
                    {
                        perge;
                    }
                    numerus_novorum++;
                    si (elementum_novum == NIHIL)
                    {
                        elementum_novum = v->datum.nodus;
                    }
                }
                si (numerus_novorum == ZEPHYRUM
                    || elementum_novum == NIHIL)
                {
                    r.verdictum = SESSIO_REIECTUM;
                    _diagnosticum_addere(&collectio, s->piscina,
                        chorda_ex_literis(SESSIO_VIA_DOCUMENTI,
                            s->piscina),
                        initium_novi, I, ZEPHYRUM, -I, FALSUM,
                        FALSUM, -I, "turnus sine elemento");
                    collectio.reice = VERUM;
                    salta finis;
                }
                si (numerus_novorum > I)
                {
                    r.verdictum = SESSIO_REIECTUM;
                    _diagnosticum_addere(&collectio, s->piscina,
                        chorda_ex_literis(SESSIO_VIA_DOCUMENTI,
                            s->piscina),
                        initium_novi, I, ZEPHYRUM, -I, FALSUM,
                        FALSUM, -I,
                        "turnus multi-elementorum - involve { }");
                    collectio.reice = VERUM;
                    salta finis;
                }
                genus = _genus_turni_ex_nodo(elementum_novum);
                si (genus == SESSIO_TURNUS_DECLARATIO
                    || genus == SESSIO_TURNUS_DEFINITIO)
                {
                    titulus_decl = _titulum_declaratum(
                        elementum_novum, s->piscina);
                }
            }
        }
        silva_piscina_destruere(effimera);
        effimera = NIHIL;
    }

    /* religatio (substitutio in loco): titulus declaratus unicus
     * congruens turno priori declaranti/definienti */
    si (titulus_decl.mensura > ZEPHYRUM)
    {
        i32 n = xar_numerus(s->turni);
        i32 k;

        per (k = ZEPHYRUM; k < n; k++)
        {
            TurnusInternus* t = xar_obtinere(s->turni, k);

            si ((t->genus == SESSIO_TURNUS_DECLARATIO
                    || t->genus == SESSIO_TURNUS_DEFINITIO)
                && t->titulus.mensura > ZEPHYRUM
                && chorda_aequalis(t->titulus, titulus_decl))
            {
                index_substituti = (s32)k;
                frange;
            }
        }
    }

    /* iudicium: textus involutus documenti totius */
    {
        i32 visus_numerus = ZEPHYRUM;
        i32 index_novi = ZEPHYRUM;
        TurnusVisus* visus;
        chorda involutum;
        SilvaParsura* parsura = NIHIL;
        SilvaSemantica* sem;

        si (_involvendus(genus))
        {
            chorda ostensum_praesens = sessio_documentum(s,
                s->piscina);

            involucrum = _involucrum_eligere(s,
                ostensum_praesens.datum == NIHIL
                    ? candidatus : ostensum_praesens);
        }
        visus = _visum_construere(s, s->piscina, candidatus, genus,
            involucrum, index_substituti, &visus_numerus,
            &index_novi);
        si (visus == NIHIL)
        {
            salta finis;
        }
        /* ostensum primum texere (implet ostensum_initium) */
        (vacuum)_ostensum_texere(visus, visus_numerus, s->piscina);
        involutum = _involutum_texere(visus, visus_numerus,
            s->piscina);
        si (involutum.datum == NIHIL)
        {
            salta finis;
        }
        effimera = silva_piscina_generare_dynamicum("sessio_iudicium",
            268435456);
        si (effimera == NIHIL)
        {
            salta finis;
        }
        sem = _iudicare(s, effimera,
            (constans character*)involutum.datum, involutum.mensura,
            &parsura);
        si (sem == NIHIL || parsura == NIHIL)
        {
            salta finis;
        }
        _colligere(s, &collectio, parsura, sem, visus, visus_numerus,
            index_novi);

        si (collectio.reice)
        {
            r.verdictum = SESSIO_REIECTUM;
            salta finis;
        }

        /* ACCIPE: committere (appensio aut substitutio) */
        si (index_substituti >= ZEPHYRUM)
        {
            TurnusInternus* t = xar_obtinere(s->turni,
                (i32)index_substituti);

            t->textus = candidatus;
            t->genus = genus;
            t->titulus = titulus_decl;
            t->lineae = _lineas_numerare(candidatus);
            /* involucrum immotum (declarationes numquam involutae) */
            r.turnus_index = index_substituti;
            r.substitutus = VERUM;
            r.turnus_substitutus = index_substituti;
        }
        alioquin
        {
            TurnusInternus* t = xar_addere(s->turni);

            si (t == NIHIL)
            {
                salta finis;
            }
            t->textus = candidatus;
            t->genus = genus;
            t->titulus = titulus_decl;
            t->involucrum = involucrum;
            t->lineae = _lineas_numerare(candidatus);
            r.turnus_index = (s32)(xar_numerus(s->turni) - I);
        }
        r.verdictum = SESSIO_ACCEPTUM;
    }

finis:
    si (effimera != NIHIL)
    {
        silva_piscina_destruere(effimera);
    }
    r.genus = genus;
    r.reparatum = reparatum;
    r.textus = candidatus;
    r.nomen_declaratum = titulus_decl;
    /* diagnostica in ordinem contiguum copiare */
    {
        i32 n = xar_numerus(collectio.xar);

        si (n > ZEPHYRUM)
        {
            SessioDiagnosticum* ordo = piscina_allocare(s->piscina,
                (memoriae_index)n * magnitudo(SessioDiagnosticum));
            i32 k;

            si (ordo != NIHIL)
            {
                per (k = ZEPHYRUM; k < n; k++)
                {
                    SessioDiagnosticum* d = xar_obtinere(
                        collectio.xar, k);

                    ordo[k] = *d;
                }
                r.diagnostica = ordo;
                r.diagnostica_numerus = (s32)n;
            }
        }
    }
    redde r;
}
