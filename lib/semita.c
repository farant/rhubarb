/* semita.c - semitae: viae ut segmenta arboris status (include/
 * semita.h; project-specs/semitae-spec.md R1). */

#include "semita.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

#define SEMITA_VITIUM  DXII


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

/* vitium formae "%.*s ... %.*s" (duo argumenta chordae); FALSUM */
interior b32
_vitium (
                  chorda* vitium,
                 Piscina* piscina,
      constans character* forma,
                  chorda  a,
                  chorda  b)
{
    character buffer[SEMITA_VITIUM];

    si (vitium)
    {
        sprintf(buffer, forma, (integer)a.mensura,
            a.datum ? (constans character*)a.datum : "",
            (integer)b.mensura, b.datum ? (constans character*)b.datum
                                        : "");
        *vitium = chorda_ex_literis(buffer, piscina);
    }
    redde FALSUM;
}

interior chorda
_nomen (
    constans StmlNodus* n)
{
    redde (n && n->titulus) ? *n->titulus : _vacua();
}

/* liberi elementares nodi (ordine) */
interior i32
_numerus_liberorum (
    constans StmlNodus* n)
{
    redde (n && n->liberi) ? xar_numerus(n->liberi) : ZEPHYRUM;
}

interior StmlNodus*
_liberum (
    constans StmlNodus* n,
                   i32  i)
{
    StmlNodus* l;

    l = *(StmlNodus**)xar_obtinere(n->liberi, i);
    redde (l && l->genus == STML_NODUS_ELEMENTUM) ? l : NIHIL;
}

/* liberum elementum nomine dato (primum); NIHIL si nullum */
interior StmlNodus*
_liberum_nomine (
    constans StmlNodus* n,
                chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < _numerus_liberorum(n); i++)
    {
        StmlNodus* l = _liberum(n, i);

        si (l && chorda_aequalis(_nomen(l), titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}

/* valor attributi nodi; vacua si abest */
interior chorda*
_attributum (
    constans StmlNodus* n,
                chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; n && n->attributa && i < xar_numerus(
        n->attributa); i++)
    {
        StmlAttributum* a = (StmlAttributum*)xar_obtinere(n->attributa,
            i);

        si (a->titulus && chorda_aequalis(*a->titulus, titulus))
        {
            redde a->valor;
        }
    }
    redde NIHIL;
}

/* attributum adest? (valor vacuus "" in STML lecto = valor NIHIL) */
interior b32
_adest (
    constans StmlNodus* n,
                chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; n && n->attributa && i < xar_numerus(
        n->attributa); i++)
    {
        StmlAttributum* a = (StmlAttributum*)xar_obtinere(n->attributa,
            i);

        si (a->titulus && chorda_aequalis(*a->titulus, titulus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* liber hereditarius: organum ligamen a parente fert */
interior b32
_hereditarius (
    constans Organum* o)
{
    i32 i;

    per (i = ZEPHYRUM; i < organum_numerus_ligaminum(o); i++)
    {
        si (organum_ligamen_index(o, i)->a_parente)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ramus liberi montationis (conventio: <genus id="nomen"/>) */
interior InsulaRamus
_ramus_liberi (
        constans InsulaRamus* parens,
    constans OrganumMontatio* m,
                     Piscina* piscina)
{
    redde insula_ramus_liberi(parens, chorda_ut_cstr(m->genus, piscina),
        chorda_ut_cstr(m->titulus, piscina));
}


/* ==================================================
 * Iudicium (arbor tota ante scripturam)
 * ================================================== */

interior b32
_iudicare (
           SemitaOrganum  organa,
                  vacuum* ctx,
        constans Organum* o,
      constans StmlNodus* n,
                     b32  liber,
    constans InsulaRamus* ramus,
                 Piscina* piscina,
                  chorda* vitium)
{
    b32 hereditarius;
    i32 i;
    i32 j;

    hereditarius = liber && _hereditarius(o);
    per (i = ZEPHYRUM; n->attributa && i < xar_numerus(n->attributa);
         i++)
    {
                StmlAttributum* a;
        constans OrganumCampus* k;

        a = (StmlAttributum*)xar_obtinere(n->attributa, i);
        k = organum_campus(o, *a->titulus);
        si (!k || k->semita == ORGANUM_SEMITA_NULLA)
        {
            redde _vitium(vitium, piscina,
                "<%.*s> attributum non in semita: %.*s", _nomen(n),
                *a->titulus);
        }
        si (hereditarius && k->semita == ORGANUM_SEMITA_LIGATIO)
        {
            redde _vitium(vitium, piscina,
                "<%.*s> %.*s: ligatio hereditaria (a parente)",
                _nomen(n),
                *a->titulus);
        }
        si (   a->valor && a->valor->mensura > ZEPHYRUM
            && !organum_valorem_iudicare(k, *a->valor, piscina))
        {
            redde _vitium(vitium, piscina,
                "<%.*s> %.*s: valor typo campi non congruit", _nomen(n),
                *a->titulus);
        }
    }
    per (i = ZEPHYRUM; i < _numerus_liberorum(n); i++)
    {
                       StmlNodus* l;
        constans OrganumMontatio* m;
                constans Organum* lo;
                     InsulaRamus  lr;

        l = _liberum(n, i);
        si (!l)
        {
            perge;
        }
        m = organum_montatio(o, _nomen(l));
        si (!m)
        {
            redde _vitium(vitium, piscina,
                "<%.*s> montatio ignota: %.*s", _nomen(n), _nomen(l));
        }
        per (j = ZEPHYRUM; j < i; j++)
        {
            si (   _liberum(n, j)
                && chorda_aequalis(_nomen(_liberum(n, j)),
                _nomen(l)))
            {
                redde _vitium(vitium, piscina,
                    "<%.*s> montatio bis: %.*s", _nomen(n), _nomen(l));
            }
        }
        lo = organa(ctx, m->genus);
        si (!lo)
        {
            redde _vitium(vitium, piscina,
                "montatio %.*s: genus ignotum %.*s", m->titulus,
                m->genus);
        }
        si (ramus)
        {
            lr = _ramus_liberi(ramus, m, piscina);
            si (!insula_ramus_nodus(&lr))
            {
                redde _vitium(vitium, piscina,
                    "<%.*s> montatio non montata: %.*s", _nomen(n),
                    m->titulus);
            }
        }
        si (!_iudicare(organa, ctx, lo, l, VERUM, ramus ? &lr : NIHIL,
                piscina, vitium))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Applicatio
 * ================================================== */

nomen structura {
     constans character* titulus;
     constans character* valor;      /* "" = tollere (ad absens) */
                 chorda  scriptor;
} Scriptura;

nomen structura {
          Xar* scripturae;   /* Scriptura */
       chorda  scriptor;     /* grex currens */
} GrexScripturae;

interior vacuum
_scripturas_ponere (
              StmlNodus* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    GrexScripturae* g;
               i32  i;

    g = (GrexScripturae*)ctx;
    per (i = ZEPHYRUM; i < xar_numerus(g->scripturae); i++)
    {
        Scriptura* s = (Scriptura*)xar_obtinere(g->scripturae, i);

        si (!chorda_aequalis(s->scriptor, g->scriptor))
        {
            perge;
        }
        si (!s->valor[ZEPHYRUM])
        {
            (vacuum)insula_attributum_tollere(radix, s->titulus);
        }
        alioquin
        {
            insula_attributum_ponere(radix, piscina, intern, s->titulus,
                s->valor);
        }
    }
}

/* valor normatus: vacuus / absens -> absens (aut vacua) */
interior b32
_valores_aequales (
    constans OrganumCampus* k,
                    chorda* currens,
                    chorda* novus)
{
    chorda a;
    chorda b;

    a = (currens && currens->mensura > ZEPHYRUM) ? *currens
        : (k->habet_absens ? k->absens : _vacua());
    b = (novus && novus->mensura > ZEPHYRUM) ? *novus
        : (k->habet_absens ? k->absens : _vacua());
    redde chorda_aequalis(a, b);
}

interior b32
_applicare_nodum (
           SemitaOrganum  organa,
                  vacuum* ctx,
        constans Organum* o,
      constans StmlNodus* n,
    constans InsulaRamus* ramus,
                 Piscina* piscina,
                  chorda* vitium)
{
               Xar* scripturae;
               b32  mutata;
               b32  nominata;
               i32  i;
               i32  j;
    GrexScripturae  grex;

    scripturae  = xar_creare(piscina, (i32)magnitudo(Scriptura));
    mutata      = FALSUM;
    nominata    = FALSUM;
    per (i = ZEPHYRUM; n->attributa && i < xar_numerus(n->attributa);
         i++)
    {
                StmlAttributum* a;
        constans OrganumCampus* k;
                     Scriptura* s;

        a = (StmlAttributum*)xar_obtinere(n->attributa, i);
        k = organum_campus(o, *a->titulus);
        si (k->semita == ORGANUM_SEMITA_LIGATIO)
        {
            nominata = VERUM;
        }
        si (   k->semita == ORGANUM_SEMITA_LIGATIO
            && !_valores_aequales(k, insula_ramus_attributum(ramus,
                   chorda_ut_cstr(k->titulus, piscina)), a->valor))
        {
            mutata = VERUM;
        }
        s           = (Scriptura*)xar_addere(scripturae);
        s->titulus  = chorda_ut_cstr(k->titulus, piscina);
        s->valor = a->valor ? chorda_ut_cstr(*a->valor,
            piscina) : "";
        s->scriptor = k->scriptor;
    }
    /* R4d: ligatio nominata TOTA - campi ligationis non nominati ad
     * absens */
    per (i = ZEPHYRUM; nominata && i < organum_numerus_camporum(o); i++)
    {
        constans OrganumCampus* k = organum_campus_index(o, i);
                     Scriptura* s;

        si (   k->semita != ORGANUM_SEMITA_LIGATIO
            || _adest(n, k->titulus))
        {
            perge;
        }
        si (!_valores_aequales(k, insula_ramus_attributum(ramus,
                chorda_ut_cstr(k->titulus, piscina)), NIHIL))
        {
            mutata = VERUM;
        }
        s            = (Scriptura*)xar_addere(scripturae);
        s->titulus   = chorda_ut_cstr(k->titulus, piscina);
        s->valor     = "";
        s->scriptor  = k->scriptor;
    }
    /* ligatione mutata: status ambitus ligationis non datus: absens */
    per (i = ZEPHYRUM; mutata && i < organum_numerus_camporum(o); i++)
    {
        constans OrganumCampus* k = organum_campus_index(o, i);
                     Scriptura* s;

        si (   k->semita  != ORGANUM_SEMITA_STATUS
            || k->ambitus != ORGANUM_AMBITUS_LIGATIO
            || _attributum(n, k->titulus))
        {
            perge;
        }
        s            = (Scriptura*)xar_addere(scripturae);
        s->titulus   = chorda_ut_cstr(k->titulus, piscina);
        s->valor     = "";
        s->scriptor  = k->scriptor;
    }
    /* scriptura una per scriptorem distinctum */
    grex.scripturae = scripturae;
    per (i = ZEPHYRUM; i < xar_numerus(scripturae); i++)
    {
        chorda scriptor;
        chorda prior;
           b32 visus;

        scriptor  = ((Scriptura*)xar_obtinere(scripturae, i))->scriptor;
        visus     = FALSUM;
        per (j = ZEPHYRUM; j < i && !visus; j++)
        {
            visus =
                chorda_aequalis(((Scriptura*)xar_obtinere(scripturae,
                j))->scriptor, scriptor);
        }
        si (visus)
        {
            perge;
        }
        grex.scriptor  = scriptor;
        prior          = ramus->repo->scriptor;
        insula_scriptorem_ponere(ramus->repo, scriptor);
        si (!mutare_ramum(ramus, _scripturas_ponere, &grex))
        {
            insula_scriptorem_ponere(ramus->repo, prior);
            redde _vitium(vitium, piscina,
                "<%.*s> scriptura recusata: %.*s", _nomen(n),
                insula_causa(ramus->repo));
        }
        insula_scriptorem_ponere(ramus->repo, prior);
    }
    /* liberi post parentem */
    per (i = ZEPHYRUM; i < _numerus_liberorum(n); i++)
    {
                       StmlNodus* l;
        constans OrganumMontatio* m;
                     InsulaRamus  lr;

        l = _liberum(n, i);
        si (!l)
        {
            perge;
        }
        m   = organum_montatio(o, _nomen(l));
        lr  = _ramus_liberi(ramus, m, piscina);
        si (!_applicare_nodum(organa, ctx, organa(ctx, m->genus), l,
            &lr,
                piscina, vitium))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
semita_applicare (
           SemitaOrganum  organa,
                  vacuum* ctx,
    constans InsulaRamus* ramus,
      constans StmlNodus* semita,
                 Piscina* piscina,
                  chorda* vitium)
{
    constans Organum* o;

    si (!organa || !ramus || !semita)
    {
        redde _vitium(vitium, piscina, "semita nulla%.*s%.*s", _vacua(),
            _vacua());
    }
    o = organa(ctx, _nomen(semita));
    si (!o)
    {
        redde _vitium(vitium, piscina, "genus ignotum: %.*s%.*s",
            _nomen(semita), _vacua());
    }
    si (!_iudicare(organa, ctx, o, semita, FALSUM, ramus, piscina,
            vitium))
    {
        redde FALSUM;
    }
    redde _applicare_nodum(organa, ctx, o, semita, ramus, piscina,
        vitium);
}


/* ==================================================
 * Lectio
 * ================================================== */

/* plena: campi omnes (absentes expliciti), liberi montati omnes */
interior StmlNodus*
_legere (
           SemitaOrganum  organa,
                  vacuum* ctx,
        constans Organum* o,
    constans InsulaRamus* ramus,
                  chorda  titulus,
                     b32  liber,
                     b32  plena,
                     b32  persistens,
                 Piscina* piscina,
     InternamentumChorda* intern)
{
    StmlNodus* n;
          b32  hereditarius;
          b32  nominata;
          s32  prima;
          i32  i;

    n = stml_elementum_creare(piscina, intern, chorda_ut_cstr(titulus,
        piscina));
    si (!n)
    {
        redde NIHIL;
    }
    hereditarius  = liber && _hereditarius(o);
    nominata      = FALSUM;
    prima         = -I;
    per (i = ZEPHYRUM; i < organum_numerus_camporum(o); i++)
    {
        constans OrganumCampus* k = organum_campus_index(o, i);
                        chorda* a;

        si (   k->semita == ORGANUM_SEMITA_NULLA
            || (hereditarius && k->semita == ORGANUM_SEMITA_LIGATIO)
            || (persistens && k->vita != ORGANUM_VITA_PERSISTENS))
        {
            perge;
        }
        si (k->semita == ORGANUM_SEMITA_LIGATIO && prima < ZEPHYRUM)
        {
            prima = (s32)i;
        }
        a = insula_ramus_attributum(ramus, chorda_ut_cstr(k->titulus,
            piscina));
        si (plena)
        {
            (vacuum)stml_attributum_addere_chorda(n, piscina, intern,
                chorda_ut_cstr(k->titulus, piscina), (a && a->mensura
                > ZEPHYRUM) ? *a : k->habet_absens ? k->absens
                : _vacua());
            perge;
        }
        si (   !a || a->mensura == ZEPHYRUM
            || (k->habet_absens && chorda_aequalis(*a, k->absens)))
        {
            perge;
        }
        (vacuum)stml_attributum_addere_chorda(n, piscina, intern,
            chorda_ut_cstr(k->titulus, piscina), *a);
        si (k->semita == ORGANUM_SEMITA_LIGATIO)
        {
            nominata = VERUM;
        }
    }
    /* R4d: radix ligationem semper nominat - omnibus ad absens campus
     * primus ligationis (absens aut vacuus); persistens: solum si
     * campus ligationis persistens ullus */
    si (!liber && !plena && !nominata && prima >= ZEPHYRUM)
    {
        constans OrganumCampus* k = organum_campus_index(o, (i32)prima);

        (vacuum)stml_attributum_addere_chorda(n, piscina, intern,
            chorda_ut_cstr(k->titulus, piscina), k->habet_absens
            ? k->absens : _vacua());
    }
    per (i = ZEPHYRUM; i < organum_numerus_montationum(o); i++)
    {
        constans OrganumMontatio* m = organum_montatio_index(o, i);
                constans Organum* lo;
                     InsulaRamus  lr;
                       StmlNodus* l;

        lo = organa(ctx, m->genus);
        lr = _ramus_liberi(ramus, m, piscina);
        si (!lo || !insula_ramus_nodus(&lr))
        {
            perge;
        }
        l = _legere(organa, ctx, lo, &lr, m->titulus, VERUM, plena,
            persistens, piscina, intern);
        si (   l
            && (plena || (l->attributa && xar_numerus(l->attributa)
            > ZEPHYRUM) || _numerus_liberorum(l) > ZEPHYRUM))
        {
            (vacuum)stml_liberum_addere(n, l);
        }
    }
    redde n;
}

StmlNodus*
semita_legere (
            SemitaOrganum  organa,
                   vacuum* ctx,
     constans InsulaRamus* ramus,
       constans character* genus,
                  Piscina* piscina,
      InternamentumChorda* intern)
{
    constans Organum* o;

    si (!organa || !ramus || !genus)
    {
        redde NIHIL;
    }
    o = organa(ctx, chorda_ex_literis(genus, piscina));
    redde o ? _legere(organa, ctx, o, ramus, chorda_ex_literis(genus,
        piscina), FALSUM, FALSUM, FALSUM, piscina, intern) : NIHIL;
}

StmlNodus*
semita_legere_plenam (
            SemitaOrganum  organa,
                   vacuum* ctx,
     constans InsulaRamus* ramus,
       constans character* genus,
                  Piscina* piscina,
      InternamentumChorda* intern)
{
    constans Organum* o;

    si (!organa || !ramus || !genus)
    {
        redde NIHIL;
    }
    o = organa(ctx, chorda_ex_literis(genus, piscina));
    redde o ? _legere(organa, ctx, o, ramus, chorda_ex_literis(genus,
        piscina), FALSUM, VERUM, FALSUM, piscina, intern) : NIHIL;
}

StmlNodus*
semita_legere_persistentem (
            SemitaOrganum  organa,
                   vacuum* ctx,
     constans InsulaRamus* ramus,
       constans character* genus,
                  Piscina* piscina,
      InternamentumChorda* intern)
{
    constans Organum* o;

    si (!organa || !ramus || !genus)
    {
        redde NIHIL;
    }
    o = organa(ctx, chorda_ex_literis(genus, piscina));
    redde o ? _legere(organa, ctx, o, ramus, chorda_ex_literis(genus,
        piscina), FALSUM, FALSUM, VERUM, piscina, intern) : NIHIL;
}


/* ==================================================
 * URI
 * ================================================== */

interior b32
_liber_effugii (
    i8 c)
{
    redde (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.'
        || c == '~';
}

interior vacuum
_effugere (
    ChordaAedificator* a,
               chorda  valor)
{
       hic_manens constans character hex[] = "0123456789ABCDEF";
                                 i32 i;

    per (i = ZEPHYRUM; i < valor.mensura; i++)
    {
        i8 c = valor.datum[i];

        si (_liber_effugii(c))
        {
            chorda_aedificator_appendere_character(a, (character)c);
        }
        alioquin
        {
            insignatus character u = (insignatus character)c;

            chorda_aedificator_appendere_character(a, '%');
            chorda_aedificator_appendere_character(a, hex[u >> IV]);
            chorda_aedificator_appendere_character(a, hex[u & XV]);
        }
    }
}

interior s32
_hex_valor (
    i8 c)
{
    si (c >= '0' && c <= '9')
    {
        redde (s32)(c - '0');
    }
    si (c >= 'A' && c <= 'F')
    {
        redde (s32)(c - 'A') + X;
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (s32)(c - 'a') + X;
    }
    redde -I;
}

/* %XX solvere; FALSUM si effugium malum */
interior b32
_solvere (
      chorda  textus,
     Piscina* piscina,
      chorda* exitus)
{
    i32 i;
    i32 n;

    exitus->datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)textus.mensura + I);
    si (!exitus->datum)
    {
        redde FALSUM;
    }
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si (textus.datum[i] == '%')
        {
            s32 h;
            s32 l;

            si (i + II >= textus.mensura)
            {
                redde FALSUM;
            }
            h = _hex_valor(textus.datum[i + I]);
            l = _hex_valor(textus.datum[i + II]);
            si (h < ZEPHYRUM || l < ZEPHYRUM)
            {
                redde FALSUM;
            }
            exitus->datum[n++]  = (i8)(h * XVI + l);
            i                   += II;
        }
        alioquin
        {
            exitus->datum[n++] = textus.datum[i];
        }
    }
    exitus->mensura = n;
    redde VERUM;
}

/* via: genus/ligatio... (ultimae vacuae omissae; media vacua =
 * absens); FALSUM si media vacua sine absente */
interior b32
_viam_scribere (
    ChordaAedificator* a,
     constans Organum* o,
   constans StmlNodus* n,
               chorda  genus)
{
    i32 i;
    s32 ultima;
    b32 nominata;

    _effugere(a, genus);
    ultima    = -I;
    nominata  = FALSUM;
    per (i = ZEPHYRUM; i < organum_numerus_camporum(o); i++)
    {
        constans OrganumCampus* k = organum_campus_index(o, i);
                        chorda* v;

        v = _attributum(n, k->titulus);
        si (   k->semita == ORGANUM_SEMITA_LIGATIO
            && _adest(n, k->titulus))
        {
            nominata = VERUM;
        }
        si (   k->semita == ORGANUM_SEMITA_LIGATIO && v
            && v->mensura > ZEPHYRUM)
        {
            ultima = (s32)i;
        }
    }
    /* R4d: ligatio nominata, omnes vacui: "genus/" */
    si (nominata && ultima < ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, '/');
    }
    per (i = ZEPHYRUM; ultima >= ZEPHYRUM && (s32)i <= ultima; i++)
    {
        constans OrganumCampus* k = organum_campus_index(o, i);
                        chorda* v;

        si (k->semita != ORGANUM_SEMITA_LIGATIO)
        {
            perge;
        }
        v = _attributum(n, k->titulus);
        chorda_aedificator_appendere_character(a, '/');
        si (v && v->mensura > ZEPHYRUM)
        {
            _effugere(a, *v);
        }
        alioquin si (k->habet_absens)
        {
            _effugere(a, k->absens);
        }
        alioquin
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* contextus quaestionis */
nomen structura {
          SemitaOrganum  organa;
                 vacuum* ctx;
                Piscina* piscina;
                    b32  lista;      /* liberi proprii: URI propria */
                    Xar* uris;       /* lista: Xar de chorda */
                    b32  bonum;
} Scriptor;

interior vacuum
_uri_nodi (
              Scriptor* s,
      constans Organum* o,
    constans StmlNodus* n,
                chorda  genus);

/* claves nodi (praefixum punctatum) et liberorum */
interior vacuum
_quaestionem_scribere (
             Scriptor* s,
    ChordaAedificator* a,
     constans Organum* o,
   constans StmlNodus* n,
               chorda  praefixum,
                  b32  radix,
                  b32* primus)
{
    i32 i;

    per (i = ZEPHYRUM; i < organum_numerus_camporum(o); i++)
    {
        constans OrganumCampus* k = organum_campus_index(o, i);
                        chorda* v;

        si (   k->semita == ORGANUM_SEMITA_NULLA
            || (radix && k->semita == ORGANUM_SEMITA_LIGATIO))
        {
            perge;
        }
        v = _attributum(n, k->titulus);
        si (!v)
        {
            perge;
        }
        chorda_aedificator_appendere_character(a, *primus ? '?' : '&');
        *primus = FALSUM;
        chorda_aedificator_appendere_chorda(a, praefixum);
        chorda_aedificator_appendere_chorda(a, k->titulus);
        chorda_aedificator_appendere_character(a, '=');
        _effugere(a, *v);
    }
    per (i = ZEPHYRUM; i < organum_numerus_montationum(o); i++)
    {
        constans OrganumMontatio* m = organum_montatio_index(o, i);
                constans Organum* lo;
                       StmlNodus* l;

        l   = _liberum_nomine(n, m->titulus);
        lo  = l ? s->organa(s->ctx, m->genus) : NIHIL;
        si (!lo)
        {
            perge;
        }
        si (s->lista && !_hereditarius(lo))
        {
            _uri_nodi(s, lo, l, m->genus);
            perge;
        }
        _quaestionem_scribere(s, a, lo, l, chorda_concatenare(
            chorda_concatenare(praefixum, m->titulus, s->piscina),
            chorda_ex_literis(".", s->piscina), s->piscina), FALSUM,
            primus);
    }
}

/* URI nodi (via ligationis suae + quaestio); in lista additur */
interior vacuum
_uri_nodi (
              Scriptor* s,
      constans Organum* o,
    constans StmlNodus* n,
                chorda  genus)
{
    ChordaAedificator* a;
                  b32  primus;
               chorda* sedes;

    a = chorda_aedificator_creare(s->piscina, (memoriae_index)CXXVIII);
    si (!a || !_viam_scribere(a, o, n, genus))
    {
        s->bonum = FALSUM;
        redde;
    }
    /* sedes ANTE liberos proprios (ordo: parens, deinde liberi);
     * Xar sedes inter additiones stabiles servat */
    sedes   = (chorda*)xar_addere(s->uris);
    primus  = VERUM;
    _quaestionem_scribere(s, a, o, n, _vacua(), VERUM, &primus);
    *sedes  = chorda_aedificator_finire(a);
}

interior Xar*
_uris (
           SemitaOrganum  organa,
                  vacuum* ctx,
      constans StmlNodus* semita,
                     b32  lista,
                 Piscina* piscina)
{
    constans Organum* o;
            Scriptor  s;
              chorda  vitium;
                 Xar* uris;

    si (!organa || !semita)
    {
        redde NIHIL;
    }
    o = organa(ctx, _nomen(semita));
    si (   !o || !_iudicare(organa, ctx, o, semita, FALSUM, NIHIL,
               piscina, &vitium))
    {
        redde NIHIL;
    }
    uris       = xar_creare(piscina, (i32)magnitudo(chorda));
    s.organa   = organa;
    s.ctx      = ctx;
    s.piscina  = piscina;
    s.lista    = lista;
    s.uris     = uris;
    s.bonum    = uris != NIHIL;
    si (s.bonum)
    {
        _uri_nodi(&s, o, semita, _nomen(semita));
    }
    redde s.bonum ? uris : NIHIL;
}

chorda
semita_ad_uri (
          SemitaOrganum  organa,
                 vacuum* ctx,
     constans StmlNodus* semita,
                Piscina* piscina)
{
    Xar* uris;

    uris = _uris(organa, ctx, semita, FALSUM, piscina);
    redde (uris && xar_numerus(uris) == I)
        ? *(chorda*)xar_obtinere(uris, ZEPHYRUM) : _vacua();
}

Xar*
semita_ad_uri_lista (
          SemitaOrganum  organa,
                 vacuum* ctx,
     constans StmlNodus* semita,
                Piscina* piscina)
{
    redde _uris(organa, ctx, semita, VERUM, piscina);
}

/* segmentum [initium, finis) chordae */
interior chorda
_sectio (
    chorda c,
       i32 initium,
       i32 finis)
{
    chorda s;

    s.datum    = c.datum + initium;
    s.mensura  = finis - initium;
    redde s;
}

StmlNodus*
semita_ex_uri (
            SemitaOrganum  organa,
                   vacuum* ctx,
                   chorda  uri,
                  Piscina* piscina,
      InternamentumChorda* intern,
                   chorda* vitium)
{
    constans Organum* o;
           StmlNodus* radix;
              chorda  via;
              chorda  quaestio;
              chorda  genus;
              chorda  valor;
                 i32  i;
                 i32  initium;
                 i32  segmentum;
                 i32  campus;

    si (!organa)
    {
        redde NIHIL;
    }
    /* via et quaestio */
    via       = uri;
    quaestio  = _vacua();
    per (i = ZEPHYRUM; i < uri.mensura; i++)
    {
        si (uri.datum[i] == '?')
        {
            via       = _sectio(uri, ZEPHYRUM, i);
            quaestio  = _sectio(uri, i + I, uri.mensura);
            frange;
        }
    }
    /* via: genus, deinde ligatio ordine manifesti */
    o          = NIHIL;
    radix      = NIHIL;
    initium    = ZEPHYRUM;
    segmentum  = ZEPHYRUM;
    campus     = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= via.mensura; i++)
    {
        si (i < via.mensura && via.datum[i] != '/')
        {
            perge;
        }
        si (!_solvere(_sectio(via, initium, i), piscina, &valor))
        {
            _vitium(vitium, piscina, "effugium malum: %.*s%.*s",
                _sectio(via, initium, i), _vacua());
            redde NIHIL;
        }
        initium = i + I;
        si (segmentum++ == ZEPHYRUM)
        {
            genus  = valor;
            o      = organa(ctx, genus);
            si (!o)
            {
                _vitium(vitium, piscina, "genus ignotum: %.*s%.*s",
                    genus,
                    _vacua());
                redde NIHIL;
            }
            radix = stml_elementum_creare(piscina, intern,
                chorda_ut_cstr(
                genus, piscina));
            perge;
        }
        dum (   campus < organum_numerus_camporum(o)
             && organum_campus_index(o, campus)->semita
                != ORGANUM_SEMITA_LIGATIO)
        {
            campus++;
        }
        si (campus >= organum_numerus_camporum(o))
        {
            _vitium(vitium, piscina, "%.*s: segmentum superfluum: %.*s",
                genus, valor);
            redde NIHIL;
        }
        /* R4d: segmentum vacuum quoque ligationem nominat (ad
         * absens) */
        (vacuum)stml_attributum_addere_chorda(radix, piscina, intern,
            chorda_ut_cstr(organum_campus_index(o, campus)->titulus,
            piscina), valor);
        campus++;
    }
    /* quaestio: clavis.punctata=valor */
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; quaestio.mensura > ZEPHYRUM
        && i <= quaestio.mensura;
         i++)
    {
             chorda  par;
             chorda  clavis;
             chorda  nomen_campi;
          StmlNodus* nodus;
   constans Organum* organum_nodi;
                i32  j;
                s32  aequale;
                i32  pars;

        si (i < quaestio.mensura && quaestio.datum[i] != '&')
        {
            perge;
        }
        par      = _sectio(quaestio, initium, i);
        initium  = i + I;
        aequale  = -I;
        per (j = ZEPHYRUM; j < par.mensura; j++)
        {
            si (par.datum[j] == '=')
            {
                aequale = (s32)j;
                frange;
            }
        }
        si (aequale < ZEPHYRUM)
        {
            _vitium(vitium, piscina, "par sine '=': %.*s%.*s", par,
                _vacua());
            redde NIHIL;
        }
        clavis = _sectio(par, ZEPHYRUM, (i32)aequale);
        si (!_solvere(_sectio(par, (i32)aequale + I, par.mensura),
            piscina,
                &valor))
        {
            _vitium(vitium, piscina, "effugium malum: %.*s%.*s", par,
                _vacua());
            redde NIHIL;
        }
        /* montationes per puncta */
        nodus         = radix;
        organum_nodi  = o;
        pars          = ZEPHYRUM;
        per (j = ZEPHYRUM; j <= clavis.mensura; j++)
        {
            constans OrganumMontatio* m;
                           StmlNodus* l;
                              chorda  nomen_partis;

            si (j < clavis.mensura && clavis.datum[j] != '.')
            {
                perge;
            }
            si (j == clavis.mensura)
            {
                frange;   /* pars ultima = campus (infra) */
            }
            nomen_partis  = _sectio(clavis, pars, j);
            pars          = j + I;
            m = organum_montatio(organum_nodi,
                nomen_partis);
            si (!m)
            {
                _vitium(vitium, piscina, "montatio ignota: %.*s (%.*s)",
                    nomen_partis, clavis);
                redde NIHIL;
            }
            l = _liberum_nomine(nodus, m->titulus);
            si (!l)
            {
                l = stml_elementum_creare(piscina, intern,
                    chorda_ut_cstr(
                    m->titulus, piscina));
                (vacuum)stml_liberum_addere(nodus, l);
            }
            nodus         = l;
            organum_nodi  = organa(ctx, m->genus);
            si (!organum_nodi)
            {
                _vitium(vitium, piscina, "genus ignotum: %.*s%.*s",
                    m->genus, _vacua());
                redde NIHIL;
            }
        }
        nomen_campi = _sectio(clavis, pars, clavis.mensura);
        si (   !organum_campus(organum_nodi, nomen_campi)
            || organum_campus(organum_nodi, nomen_campi)->semita
               == ORGANUM_SEMITA_NULLA)
        {
            _vitium(vitium, piscina,
                "campus non in semita: %.*s (%.*s)",
                nomen_campi, clavis);
            redde NIHIL;
        }
        si (_attributum(nodus, nomen_campi))
        {
            _vitium(vitium, piscina, "clavis bis: %.*s%.*s", clavis,
                _vacua());
            redde NIHIL;
        }
        (vacuum)stml_attributum_addere_chorda(nodus, piscina, intern,
            chorda_ut_cstr(nomen_campi, piscina), valor);
    }
    si (   !radix || !_iudicare(organa, ctx, o, radix, FALSUM, NIHIL,
            piscina, vitium))
    {
        redde NIHIL;
    }
    redde radix;
}
