/* silva_quaestiones.c - bibliotheca selectorum nominatorum
 * (contractus + forma plagulae: vide caput)
 *
 * Scanner locorum parametrorum: contextus discriminat -
 *   (a) intra citationes "..." (valores attributorum; citationes
 *       QB sine fugis, ergo finis = citatio proxima);
 *   (b) argumentum pseudo INTEGRUM '($nomen)' - forma exacta sine
 *       spatiis internis (auctor spatia scribens fracturam
 *       "parametrum non adhibitum" claram accipit).
 * '$' extra hos contextus = captura machinae, intacta.
 */
#include "silva_quaestiones.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "stml.h"
#include <string.h>

/* ==================================================
 * Auxilia parva
 * ================================================== */

interior b32
_aequat (chorda c, constans character* litterae)
{
    i32 mensura = (i32)strlen(litterae);

    si (c.mensura != mensura) redde FALSUM;
    si (mensura == ZEPHYRUM) redde VERUM;
    redde memcmp(c.datum, litterae, (size_t)mensura) == ZEPHYRUM
        ? VERUM : FALSUM;
}

interior b32
_chordae_pares_q (chorda a, chorda b)
{
    si (a.mensura != b.mensura) redde FALSUM;
    si (a.mensura == ZEPHYRUM) redde VERUM;
    redde memcmp(a.datum, b.datum, (size_t)a.mensura) == ZEPHYRUM
        ? VERUM : FALSUM;
}

interior b32
_est_spatium_q (i8 c)
{
    redde (c == (i8)' ' || c == (i8)'\t' || c == (i8)'\n'
        || c == (i8)'\r') ? VERUM : FALSUM;
}

/* litterae tag/parametri: [a-z0-9_-] (grammatica machinae) */
interior b32
_est_littera_tituli (i8 c)
{
    redde ((c >= (i8)'a' && c <= (i8)'z')
        || (c >= (i8)'0' && c <= (i8)'9')
        || c == (i8)'-' || c == (i8)'_') ? VERUM : FALSUM;
}

interior chorda
_trimmata (chorda c)
{
    dum (c.mensura > ZEPHYRUM && _est_spatium_q(c.datum[ZEPHYRUM]))
    {
        c.datum++;
        c.mensura--;
    }
    dum (c.mensura > ZEPHYRUM
        && _est_spatium_q(c.datum[c.mensura - I]))
    {
        c.mensura--;
    }
    redde c;
}

/* chorda -> litterae NUL-terminatae in piscina (compilare cstr
 * exigit) */
interior constans character*
_ut_literis (Piscina* piscina, chorda c)
{
    character* litterae = (character*)piscina_allocare(piscina,
        (memoriae_index)(c.mensura + I));

    si (litterae == NIHIL) redde NIHIL;
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(litterae, c.datum, (size_t)c.mensura);
    }
    litterae[c.mensura] = '\0';
    redde litterae;
}

/* culpam texere: "quaestio 'T': nuntius 'R'" (T/R absentia licita) */
interior vacuum
_culpam_ponere (Piscina* piscina, chorda* culpa_out,
    chorda quaestio_titulus, constans character* nuntius, chorda res)
{
    ChordaAedificator* a;

    si (culpa_out == NIHIL) redde;
    a = chorda_aedificator_creare(piscina, (memoriae_index)64);
    si (a == NIHIL) redde;
    si (quaestio_titulus.mensura > ZEPHYRUM)
    {
        (vacuum)chorda_aedificator_appendere_literis(a, "quaestio '");
        (vacuum)chorda_aedificator_appendere_chorda(a,
            quaestio_titulus);
        (vacuum)chorda_aedificator_appendere_literis(a, "': ");
    }
    (vacuum)chorda_aedificator_appendere_literis(a, nuntius);
    si (res.mensura > ZEPHYRUM)
    {
        (vacuum)chorda_aedificator_appendere_literis(a, " '");
        (vacuum)chorda_aedificator_appendere_chorda(a, res);
        (vacuum)chorda_aedificator_appendere_literis(a, "'");
    }
    *culpa_out = chorda_aedificator_finire(a);
}

interior chorda
_chorda_vacua_q (vacuum)
{
    chorda c;

    c.mensura = ZEPHYRUM;
    c.datum = NIHIL;
    redde c;
}

/* ==================================================
 * Scanner locorum parametrorum
 * ================================================== */

nomen structura {
    i32    initium;  /* offset '$' in selectore */
    i32    mensura;  /* longitudo cum '$' */
    chorda titulus;  /* sine '$' - vacuum = fractura validationis */
} LocusParametri;

/* Loca $parametrorum in textu selectoris colligere (contextus (a)
 * et (b) supra). NIHIL = piscina exhausta. */
interior Xar*
_loca_colligere (Piscina* piscina, chorda selector)
{
    Xar* loca = xar_creare(piscina, magnitudo(LocusParametri));
    b32 in_citatione = FALSUM;
    i32 i = ZEPHYRUM;

    si (loca == NIHIL) redde NIHIL;
    dum (i < selector.mensura)
    {
        i8 c = selector.datum[i];

        si (c == (i8)'"')
        {
            in_citatione = in_citatione ? FALSUM : VERUM;
            i++;
            perge;
        }
        si (in_citatione && c == (i8)'$')
        {
            LocusParametri locus;
            i32 finis = i + I;

            dum (finis < selector.mensura
                && _est_littera_tituli(selector.datum[finis]))
            {
                finis++;
            }
            locus.initium = i;
            locus.mensura = finis - i;
            locus.titulus.datum = selector.datum + i + I;
            locus.titulus.mensura = finis - i - I;
            {
                LocusParametri* slot =
                    (LocusParametri*)xar_addere(loca);

                si (slot == NIHIL) redde NIHIL;
                *slot = locus;
            }
            i = finis;
            perge;
        }
        si (!in_citatione && c == (i8)'(' && i + I < selector.mensura
            && selector.datum[i + I] == (i8)'$')
        {
            i32 finis = i + II;

            dum (finis < selector.mensura
                && _est_littera_tituli(selector.datum[finis]))
            {
                finis++;
            }
            /* forma exacta '($nomen)' sola - alioquin textus
             * ordinarius manet */
            si (finis < selector.mensura
                && selector.datum[finis] == (i8)')')
            {
                LocusParametri locus;
                LocusParametri* slot;

                locus.initium = i + I;
                locus.mensura = finis - i - I;
                locus.titulus.datum = selector.datum + i + II;
                locus.titulus.mensura = finis - i - II;
                slot = (LocusParametri*)xar_addere(loca);
                si (slot == NIHIL) redde NIHIL;
                *slot = locus;
                i = finis;
                perge;
            }
        }
        i++;
    }
    redde loca;
}

/* Selectorem completum texere: segmenta inter loca + valores.
 * valores = Xar de chorda (valore), parallelum locis. */
interior chorda
_selectorem_texere (Piscina* piscina, chorda selector, Xar* loca,
    Xar* valores)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)(selector.mensura + 32));
    i32 cursor = ZEPHYRUM;
    i32 k;
    chorda pars;

    si (a == NIHIL) redde _chorda_vacua_q();
    per (k = ZEPHYRUM; k < xar_numerus(loca); k++)
    {
        LocusParametri* locus =
            (LocusParametri*)xar_obtinere(loca, k);
        chorda* valor = (chorda*)xar_obtinere(valores, k);

        pars.datum = selector.datum + cursor;
        pars.mensura = locus->initium - cursor;
        (vacuum)chorda_aedificator_appendere_chorda(a, pars);
        (vacuum)chorda_aedificator_appendere_chorda(a, *valor);
        cursor = locus->initium + locus->mensura;
    }
    pars.datum = selector.datum + cursor;
    pars.mensura = selector.mensura - cursor;
    (vacuum)chorda_aedificator_appendere_chorda(a, pars);
    redde chorda_aedificator_finire(a);
}

/* ==================================================
 * Validatio entrii
 * ================================================== */

/* Parametra declarata contra loca selectoris UTRIMQUE probare +
 * specimen compilare. VERUM = validum; FALSUM + culpa. */
interior b32
_entrium_validare (Piscina* piscina,
    constans SilvaQuaestiones* bibliotheca,
    constans SilvaQuaestioNominata* nominata, chorda* culpa_out)
{
    Xar* loca = _loca_colligere(piscina, nominata->selector);
    i32 k;
    i32 p;

    si (loca == NIHIL)
    {
        _culpam_ponere(piscina, culpa_out, nominata->titulus,
            "piscina exhausta", _chorda_vacua_q());
        redde FALSUM;
    }
    /* omnis locus declaratum nominat */
    per (k = ZEPHYRUM; k < xar_numerus(loca); k++)
    {
        LocusParametri* locus =
            (LocusParametri*)xar_obtinere(loca, k);
        b32 inventum = FALSUM;

        si (locus->titulus.mensura == ZEPHYRUM)
        {
            _culpam_ponere(piscina, culpa_out, nominata->titulus,
                "parametrum vacuum in selectore", _chorda_vacua_q());
            redde FALSUM;
        }
        per (p = ZEPHYRUM; p < xar_numerus(nominata->parametra); p++)
        {
            chorda* declaratum =
                (chorda*)xar_obtinere(nominata->parametra, p);

            si (_chordae_pares_q(*declaratum, locus->titulus))
            {
                inventum = VERUM;
                frange;
            }
        }
        si (!inventum)
        {
            _culpam_ponere(piscina, culpa_out, nominata->titulus,
                "parametrum ignotum in selectore", locus->titulus);
            redde FALSUM;
        }
    }
    /* omne declaratum locum habet */
    per (p = ZEPHYRUM; p < xar_numerus(nominata->parametra); p++)
    {
        chorda* declaratum =
            (chorda*)xar_obtinere(nominata->parametra, p);
        b32 inventum = FALSUM;

        per (k = ZEPHYRUM; k < xar_numerus(loca); k++)
        {
            LocusParametri* locus =
                (LocusParametri*)xar_obtinere(loca, k);

            si (_chordae_pares_q(*declaratum, locus->titulus))
            {
                inventum = VERUM;
                frange;
            }
        }
        si (!inventum)
        {
            _culpam_ponere(piscina, culpa_out, nominata->titulus,
                "parametrum non adhibitum", *declaratum);
            redde FALSUM;
        }
    }
    /* specimen: parametra "x" substituta, compilatio probatur */
    {
        Xar* valores = xar_creare(piscina, magnitudo(chorda));
        chorda specimen;
        constans character* litterae;
        constans character* causa = NIHIL;
        SilvaQuaestio* q;

        si (valores == NIHIL) redde FALSUM;
        per (k = ZEPHYRUM; k < xar_numerus(loca); k++)
        {
            chorda* slot = (chorda*)xar_addere(valores);

            si (slot == NIHIL) redde FALSUM;
            *slot = chorda_ex_literis("x", piscina);
        }
        specimen = _selectorem_texere(piscina, nominata->selector,
            loca, valores);
        litterae = _ut_literis(piscina, specimen);
        si (litterae == NIHIL) redde FALSUM;
        q = silva_quaestio_compilare_cum_registro(piscina,
            bibliotheca->tabularium, bibliotheca->registro,
            litterae, &causa);
        si (q == NIHIL)
        {
            chorda causa_chorda = _chorda_vacua_q();

            si (causa != NIHIL)
            {
                causa_chorda = chorda_ex_literis(causa, piscina);
            }
            _culpam_ponere(piscina, culpa_out, nominata->titulus,
                "selector non compilatur", causa_chorda);
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* ==================================================
 * Lectio plagulae
 * ================================================== */

/* Elementum <quaestio> unum legere (attributa + liberi) */
interior b32
_quaestionem_legere (Piscina* piscina, StmlNodus* elementum,
    SilvaQuaestioNominata* nominata, chorda* culpa_out)
{
    chorda* titulus = stml_attributum_capere(elementum, "titulus");
    chorda* gradus = stml_attributum_capere(elementum, "gradus");
    b32 selector_visus = FALSUM;
    b32 causa_visa = FALSUM;
    i32 k;

    nominata->titulus = _chorda_vacua_q();
    nominata->selector = _chorda_vacua_q();
    nominata->causa = _chorda_vacua_q();
    nominata->gradus = (s32)SILVA_QUAESTIONES_OCULI;
    nominata->parametra = xar_creare(piscina, magnitudo(chorda));
    si (nominata->parametra == NIHIL) redde FALSUM;

    si (titulus == NIHIL || titulus->mensura == ZEPHYRUM)
    {
        _culpam_ponere(piscina, culpa_out, _chorda_vacua_q(),
            "quaestio sine titulo", _chorda_vacua_q());
        redde FALSUM;
    }
    nominata->titulus = *titulus;
    si (gradus == NIHIL)
    {
        _culpam_ponere(piscina, culpa_out, nominata->titulus,
            "gradus absens (oculi|invarians)", _chorda_vacua_q());
        redde FALSUM;
    }
    si (_aequat(*gradus, "oculi"))
    {
        nominata->gradus = (s32)SILVA_QUAESTIONES_OCULI;
    }
    alioquin si (_aequat(*gradus, "invarians"))
    {
        nominata->gradus = (s32)SILVA_QUAESTIONES_INVARIANS;
    }
    alioquin
    {
        _culpam_ponere(piscina, culpa_out, nominata->titulus,
            "gradus ignotus", *gradus);
        redde FALSUM;
    }

    per (k = ZEPHYRUM; k < stml_numerus_liberorum(elementum); k++)
    {
        StmlNodus* liber = *(StmlNodus**)
            xar_obtinere(elementum->liberi, k);

        si (liber == NIHIL || liber->genus != STML_NODUS_ELEMENTUM)
        {
            perge;  /* textus/commenta inter elementa licita */
        }
        si (liber->titulus != NIHIL
            && _aequat(*liber->titulus, "selector"))
        {
            si (selector_visus)
            {
                _culpam_ponere(piscina, culpa_out,
                    nominata->titulus, "selector iteratus",
                    _chorda_vacua_q());
                redde FALSUM;
            }
            selector_visus = VERUM;
            nominata->selector = _trimmata(
                stml_textus_normalizatus(liber, piscina));
        }
        alioquin si (liber->titulus != NIHIL
            && _aequat(*liber->titulus, "causa"))
        {
            si (causa_visa)
            {
                _culpam_ponere(piscina, culpa_out,
                    nominata->titulus, "causa iterata",
                    _chorda_vacua_q());
                redde FALSUM;
            }
            causa_visa = VERUM;
            nominata->causa = _trimmata(
                stml_textus_normalizatus(liber, piscina));
        }
        alioquin si (liber->titulus != NIHIL
            && _aequat(*liber->titulus, "parametrum"))
        {
            chorda* par_titulus =
                stml_attributum_capere(liber, "titulus");
            i32 p;
            chorda* slot;

            si (par_titulus == NIHIL
                || par_titulus->mensura == ZEPHYRUM)
            {
                _culpam_ponere(piscina, culpa_out,
                    nominata->titulus, "parametrum sine titulo",
                    _chorda_vacua_q());
                redde FALSUM;
            }
            per (p = ZEPHYRUM;
                p < xar_numerus(nominata->parametra); p++)
            {
                chorda* prius =
                    (chorda*)xar_obtinere(nominata->parametra, p);

                si (_chordae_pares_q(*prius, *par_titulus))
                {
                    _culpam_ponere(piscina, culpa_out,
                        nominata->titulus, "parametrum iteratum",
                        *par_titulus);
                    redde FALSUM;
                }
            }
            slot = (chorda*)xar_addere(nominata->parametra);
            si (slot == NIHIL) redde FALSUM;
            *slot = *par_titulus;
        }
        alioquin
        {
            _culpam_ponere(piscina, culpa_out, nominata->titulus,
                "elementum ignotum",
                liber->titulus != NIHIL ? *liber->titulus
                    : _chorda_vacua_q());
            redde FALSUM;
        }
    }

    si (!selector_visus || nominata->selector.mensura == ZEPHYRUM)
    {
        _culpam_ponere(piscina, culpa_out, nominata->titulus,
            "selector absens aut vacuus", _chorda_vacua_q());
        redde FALSUM;
    }
    si (!causa_visa || nominata->causa.mensura == ZEPHYRUM)
    {
        _culpam_ponere(piscina, culpa_out, nominata->titulus,
            "causa absens aut vacua (obligatoria)", _chorda_vacua_q());
        redde FALSUM;
    }
    si (nominata->gradus == (s32)SILVA_QUAESTIONES_INVARIANS
        && xar_numerus(nominata->parametra) > ZEPHYRUM)
    {
        _culpam_ponere(piscina, culpa_out, nominata->titulus,
            "invarians cum parametris (percursus valores nescit)",
            _chorda_vacua_q());
        redde FALSUM;
    }
    redde VERUM;
}

SilvaQuaestiones*
silva_quaestiones_legere (
    Piscina*                               piscina,
    constans SilvaRegistrumCoctum*         tabularium,
    constans SilvaQuaestioPseudoRegistrum* registro,
    chorda                                 fons,
    chorda*                                culpa_out)
{
    InternamentumChorda* intern = internamentum_creare(piscina);
    StmlResultus resultus;
    SilvaQuaestiones* bibliotheca;
    i32 k;

    si (culpa_out != NIHIL)
    {
        *culpa_out = _chorda_vacua_q();
    }
    si (intern == NIHIL) redde NIHIL;
    resultus = stml_legere(fons, piscina, intern);
    si (!resultus.successus || resultus.elementum_radix == NIHIL)
    {
        _culpam_ponere(piscina, culpa_out, _chorda_vacua_q(),
            "stml non parsatur", resultus.error);
        redde NIHIL;
    }
    si (resultus.elementum_radix->titulus == NIHIL
        || !_aequat(*resultus.elementum_radix->titulus,
               "quaestiones"))
    {
        _culpam_ponere(piscina, culpa_out, _chorda_vacua_q(),
            "radix non est <quaestiones>", _chorda_vacua_q());
        redde NIHIL;
    }

    bibliotheca = (SilvaQuaestiones*)piscina_allocare(piscina,
        magnitudo(SilvaQuaestiones));
    si (bibliotheca == NIHIL) redde NIHIL;
    bibliotheca->nominatae = xar_creare(piscina,
        magnitudo(SilvaQuaestioNominata));
    si (bibliotheca->nominatae == NIHIL) redde NIHIL;
    bibliotheca->tabularium = tabularium;
    bibliotheca->registro = registro;

    per (k = ZEPHYRUM;
        k < stml_numerus_liberorum(resultus.elementum_radix); k++)
    {
        StmlNodus* liber = *(StmlNodus**)
            xar_obtinere(resultus.elementum_radix->liberi, k);
        SilvaQuaestioNominata nominata;
        i32 prius;

        si (liber == NIHIL || liber->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (liber->titulus == NIHIL
            || !_aequat(*liber->titulus, "quaestio"))
        {
            _culpam_ponere(piscina, culpa_out, _chorda_vacua_q(),
                "elementum ignotum sub radice",
                liber->titulus != NIHIL ? *liber->titulus
                    : _chorda_vacua_q());
            redde NIHIL;
        }
        si (!_quaestionem_legere(piscina, liber, &nominata,
                culpa_out))
        {
            redde NIHIL;
        }
        /* tituli unici */
        per (prius = ZEPHYRUM;
            prius < xar_numerus(bibliotheca->nominatae); prius++)
        {
            SilvaQuaestioNominata* alia = (SilvaQuaestioNominata*)
                xar_obtinere(bibliotheca->nominatae, prius);

            si (_chordae_pares_q(alia->titulus, nominata.titulus))
            {
                _culpam_ponere(piscina, culpa_out,
                    nominata.titulus, "titulus iteratus",
                    _chorda_vacua_q());
                redde NIHIL;
            }
        }
        si (!_entrium_validare(piscina, bibliotheca, &nominata,
                culpa_out))
        {
            redde NIHIL;
        }
        {
            SilvaQuaestioNominata* slot = (SilvaQuaestioNominata*)
                xar_addere(bibliotheca->nominatae);

            si (slot == NIHIL) redde NIHIL;
            *slot = nominata;
        }
    }
    redde bibliotheca;
}

/* ==================================================
 * Accessus
 * ================================================== */

i32
silva_quaestiones_numerus (constans SilvaQuaestiones* bibliotheca)
{
    si (bibliotheca == NIHIL) redde ZEPHYRUM;
    redde xar_numerus(bibliotheca->nominatae);
}

constans SilvaQuaestioNominata*
silva_quaestiones_ad_indicem (
    constans SilvaQuaestiones* bibliotheca,
    i32                        index)
{
    si (bibliotheca == NIHIL
        || index >= xar_numerus(bibliotheca->nominatae))
    {
        redde NIHIL;
    }
    redde (constans SilvaQuaestioNominata*)
        xar_obtinere(bibliotheca->nominatae, index);
}

constans SilvaQuaestioNominata*
silva_quaestiones_invenire (
    constans SilvaQuaestiones* bibliotheca,
    constans character*        titulus)
{
    i32 k;

    si (bibliotheca == NIHIL || titulus == NIHIL) redde NIHIL;
    per (k = ZEPHYRUM; k < xar_numerus(bibliotheca->nominatae); k++)
    {
        SilvaQuaestioNominata* nominata = (SilvaQuaestioNominata*)
            xar_obtinere(bibliotheca->nominatae, k);

        si (_aequat(nominata->titulus, titulus))
        {
            redde nominata;
        }
    }
    redde NIHIL;
}

/* ==================================================
 * Parare
 * ================================================== */

SilvaQuaestio*
silva_quaestiones_parare (
    Piscina*                              piscina,
    constans SilvaQuaestiones*            bibliotheca,
    constans SilvaQuaestioNominata*       nominata,
    constans SilvaQuaestionesArgumentum*  argumenta,
    i32                                   numerus_argumentorum,
    chorda*                               culpa_out)
{
    Xar* loca;
    Xar* valores;
    i32 k;
    i32 p;

    si (culpa_out != NIHIL)
    {
        *culpa_out = _chorda_vacua_q();
    }
    si (bibliotheca == NIHIL || nominata == NIHIL) redde NIHIL;
    si (argumenta == NIHIL)
    {
        numerus_argumentorum = ZEPHYRUM;
    }

    /* argumenta contra declarata: ignota, iterata, valores */
    per (k = ZEPHYRUM; k < numerus_argumentorum; k++)
    {
        constans SilvaQuaestionesArgumentum* argumentum =
            &argumenta[k];
        b32 inventum = FALSUM;
        i32 c;

        per (p = ZEPHYRUM; p < xar_numerus(nominata->parametra); p++)
        {
            chorda* declaratum =
                (chorda*)xar_obtinere(nominata->parametra, p);

            si (_chordae_pares_q(*declaratum, argumentum->titulus))
            {
                inventum = VERUM;
                frange;
            }
        }
        si (!inventum)
        {
            _culpam_ponere(piscina, culpa_out, nominata->titulus,
                "argumentum ignotum", argumentum->titulus);
            redde NIHIL;
        }
        per (p = ZEPHYRUM; p < k; p++)
        {
            constans SilvaQuaestionesArgumentum* prius =
                &argumenta[p];

            si (_chordae_pares_q(prius->titulus, argumentum->titulus))
            {
                _culpam_ponere(piscina, culpa_out, nominata->titulus,
                    "argumentum iteratum", argumentum->titulus);
                redde NIHIL;
            }
        }
        per (c = ZEPHYRUM; c < argumentum->valor.mensura; c++)
        {
            i8 littera = argumentum->valor.datum[c];

            si (littera == (i8)'"' || littera == (i8)'('
                || littera == (i8)')')
            {
                _culpam_ponere(piscina, culpa_out, nominata->titulus,
                    "valor illicitus (characteres \" ( ) vetiti)",
                    argumentum->titulus);
                redde NIHIL;
            }
        }
    }
    /* omne declaratum argumentum habet */
    per (p = ZEPHYRUM; p < xar_numerus(nominata->parametra); p++)
    {
        chorda* declaratum =
            (chorda*)xar_obtinere(nominata->parametra, p);
        b32 inventum = FALSUM;

        per (k = ZEPHYRUM; k < numerus_argumentorum; k++)
        {
            si (_chordae_pares_q(*declaratum,
                    argumenta[k].titulus))
            {
                inventum = VERUM;
                frange;
            }
        }
        si (!inventum)
        {
            _culpam_ponere(piscina, culpa_out, nominata->titulus,
                "argumentum absens", *declaratum);
            redde NIHIL;
        }
    }

    loca = _loca_colligere(piscina, nominata->selector);
    si (loca == NIHIL) redde NIHIL;
    valores = xar_creare(piscina, magnitudo(chorda));
    si (valores == NIHIL) redde NIHIL;
    per (k = ZEPHYRUM; k < xar_numerus(loca); k++)
    {
        LocusParametri* locus = (LocusParametri*)xar_obtinere(loca, k);
        chorda* slot = (chorda*)xar_addere(valores);

        si (slot == NIHIL) redde NIHIL;
        *slot = _chorda_vacua_q();
        per (p = ZEPHYRUM; p < numerus_argumentorum; p++)
        {
            si (_chordae_pares_q(argumenta[p].titulus,
                    locus->titulus))
            {
                *slot = argumenta[p].valor;
                frange;
            }
        }
    }
    {
        chorda completus = _selectorem_texere(piscina,
            nominata->selector, loca, valores);
        constans character* litterae = _ut_literis(piscina, completus);
        constans character* causa = NIHIL;
        SilvaQuaestio* quaestio;

        si (litterae == NIHIL) redde NIHIL;
        quaestio = silva_quaestio_compilare_cum_registro(piscina,
            bibliotheca->tabularium, bibliotheca->registro,
            litterae, &causa);
        si (quaestio == NIHIL)
        {
            chorda causa_chorda = _chorda_vacua_q();

            si (causa != NIHIL)
            {
                causa_chorda = chorda_ex_literis(causa, piscina);
            }
            _culpam_ponere(piscina, culpa_out, nominata->titulus,
                "compilatio deficit", causa_chorda);
            redde NIHIL;
        }
        redde quaestio;
    }
}
