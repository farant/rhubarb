/* conspectus_dies.c - CONSPECTUS 'dies' (proiectiones D4b). Vide
 * conspectus_dies.h.
 *
 * Regulae ex lib/kalendarium.c translatae (pictura eas iam non
 * computat): celebratio hebdomadis (diem_hebdomadis_pingere), festum
 * cellulae mensis (cellulam_pingere), signa diei (agenda_signa_mensis
 * per diem), indentatio communis notarum. Omnia in piscinam conspectus
 * copiantur: status agendae in omni revocatione vacatur
 * (lib/agenda.c, piscina 'status'). */

#include "conspectus_dies.h"
#include "conspectus_infixa.h"
#include "scriba_liber.h"
#include "stml.h"

#include <stdio.h>
#include <string.h>

interior chorda
vacua (vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

interior chorda
clavis_diei (
       Dies  d,
    Piscina* piscina)
{
    character buffer[XVI];

    sprintf(buffer, "%04d-%02d-%02d", (integer)d.annus,
        (integer)d.mensis, (integer)d.dies);
    redde chorda_ex_literis(buffer, piscina);
}

interior b32
dies_aequales (
    Dies a,
    Dies b)
{
    redde a.annus == b.annus && a.mensis == b.mensis
        && a.dies == b.dies;
}

/* indentatio communis (spatia initialia minima) removetur, relativa
 * manet ("     abcd" / "       efg" -> "abcd" / "  efg"); lineae vacuae
 * iam omissae sunt (ex kalendarium.c, A6c - translata verbatim) */
interior vacuum
indentationem_communem_tollere (
    Xar* lineae)
{
    chorda* l;
       s32  minima;
       s32  k;
       i32  i;

    minima = -I;
    per (i = ZEPHYRUM; i < xar_numerus(lineae); i++)
    {
        l = (chorda*)xar_obtinere(lineae, i);
        k = ZEPHYRUM;
        dum (k < (s32)l->mensura && l->datum[k] == ' ')
        {
            k++;
        }
        si (minima < ZEPHYRUM || k < minima)
        {
            minima = k;
        }
    }
    per (i = ZEPHYRUM; minima > ZEPHYRUM
        && i < xar_numerus(lineae); i++)
    {
        l           = (chorda*)xar_obtinere(lineae, i);
        l->datum    += minima;
        l->mensura  -= (i32)minima;
    }
}

/* celebratio hebdomadis: principalis si memoria ad libitum aut maior
 * (dominicae quoque); in feria: celebratio prima quae servari potest */
interior constans Celebratio*
celebratio_hebdomadis (
    constans InformatioDiei* info)
{
    constans Celebratio* celebratio;
                    s32  i;

    celebratio = info->celebratio_principalis;
    si (celebratio && celebratio->gradus >= GRADUS_MEMORIA_AD_LIBITUM)
    {
        redde celebratio;
    }
    per (i = I; i < info->numerus_celebrationum; i++)
    {
        si (info->celebrationes[i].gradus >= GRADUS_MEMORIA_AD_LIBITUM)
        {
            redde &info->celebrationes[i];
        }
    }
    redde NIHIL;
}

DiesConspectus*
conspectus_dies_computare (
    constans DiesFontes* fontes,
                   Dies  dies,
                Piscina* piscina)
{
    DiesConspectus* v;
               Xar* l;
               i32  i;

    si (!fontes || !piscina || !fasti_dies_valida(dies))
    {
        redde NIHIL;
    }
    v = (DiesConspectus*)piscina_allocare(piscina,
        magnitudo(DiesConspectus));
    memset(v, ZEPHYRUM, magnitudo(DiesConspectus));
    v->dies          = dies;
    v->natales       = xar_creare(piscina, (i32)magnitudo(chorda));
    v->apertae       = xar_creare(piscina, (i32)magnitudo(chorda));
    v->lineae_libri  = xar_creare(piscina, (i32)magnitudo(chorda));

    /* liturgia */
    si (fontes->liturgia)
    {
             InformatioDiei* info;
        constans Celebratio* c;

        info = calendarium_obtinere_diem(fontes->liturgia, dies,
            piscina);
        si (info)
        {
            v->liturgia  = VERUM;
            v->color     = info->color_diei;
            c            = celebratio_hebdomadis(info);
            v->celebratio = c ? chorda_transcribere(c->titulus, piscina)
                : vacua();
            c = info->celebratio_principalis;
            si (   c && c->gradus >= GRADUS_MEMORIA
                && c->gradus != GRADUS_DOMINICA)
            {
                v->festum = chorda_transcribere(c->titulus_brevis,
                    piscina);
            }
        }
    }

    /* natales (fontes dierum) */
    l = fontes->fontes ? fasti_fontes_legere(fontes->fontes, dies, dies,
        piscina) : NIHIL;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        constans FastiIntroitus* t = (constans FastiIntroitus*)
            xar_obtinere(l, i);

        si (   dies_aequales(t->dies, dies)
            && chorda_aequalis_literis(t->genus, FASTI_GENUS_NATALIS))
        {
            *(chorda*)xar_addere(v->natales) = chorda_transcribere(
                fasti_introitum_textus(t, piscina), piscina);
        }
    }

    /* res diei et signa */
    si (fontes->agenda)
    {
        l = agenda_res_diei(fontes->agenda, dies, piscina);
        per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
        {
            constans AgendaRes* r = *(constans AgendaRes**)xar_obtinere(
                l, i);

            v->signum.assignatae++;
            si (!r->perfecta)
            {
                v->signum.apertae++;
                *(chorda*)xar_addere(v->apertae) = chorda_transcribere(
                    r->titulus, piscina);
            }
        }
        l = agenda_perfectae_die(fontes->agenda, dies, piscina);
        v->signum.perfectae = l ? xar_numerus(l) : ZEPHYRUM;
    }

    /* liber diei (lectio sola) */
    si (fontes->volumen && fontes->intern)
    {
        character praefixum[XXXII];

        sprintf(praefixum, "dies/%04d-%02d-%02d", (integer)dies.annus,
            (integer)dies.mensis, (integer)dies.dies);
        (vacuum)scriba_liber_lineas_legere(piscina, fontes->intern,
            fontes->volumen, praefixum, ZEPHYRUM, v->lineae_libri);
        indentationem_communem_tollere(v->lineae_libri);
        per (i = ZEPHYRUM; i < xar_numerus(v->lineae_libri); i++)
        {
            chorda* linea = (chorda*)xar_obtinere(v->lineae_libri, i);

            *linea = chorda_transcribere(*linea, piscina);
        }
    }
    redde v;
}

interior vacuum
liberum_textus (
              StmlNodus* parens,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus,
                 chorda  textus)
{
    StmlNodus* n;

    n = stml_elementum_creare(piscina, intern, titulus);
    (vacuum)stml_liberum_addere(n, stml_textum_creare_ex_chorda(piscina,
        intern, textus));
    (vacuum)stml_liberum_addere(parens, n);
}

interior vacuum
liberos_textus (
              StmlNodus* parens,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus,
                    Xar* l)
{
    i32 i;

    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        liberum_textus(parens, piscina, intern, titulus,
            *(chorda*)xar_obtinere(l, i));
    }
}

chorda
conspectus_dies_codificare (
    constans DiesConspectus* c,
                    Piscina* piscina)
{
    InternamentumChorda* intern;
              StmlNodus* radix;
              StmlNodus* signum;
              character  numerus[XVI];

    si (!c || !piscina)
    {
        redde vacua();
    }
    intern  = internamentum_creare(piscina);
    radix   = stml_elementum_creare(piscina, intern, "dies");
    (vacuum)stml_attributum_addere_chorda(radix, piscina, intern,
        "clavis", clavis_diei(c->dies, piscina));
    si (c->liturgia)
    {
        sprintf(numerus, "%d", (integer)c->color);
        (vacuum)stml_attributum_addere(radix, piscina, intern, "color",
            numerus);
        liberum_textus(radix, piscina, intern, "celebratio",
            c->celebratio);
        liberum_textus(radix, piscina, intern, "festum", c->festum);
    }
    liberos_textus(radix, piscina, intern, "natalis", c->natales);
    liberos_textus(radix, piscina, intern, "res", c->apertae);
    signum = stml_elementum_creare(piscina, intern, "signum");
    sprintf(numerus, "%u", (insignatus integer)c->signum.assignatae);
    (vacuum)stml_attributum_addere(signum, piscina, intern,
        "assignatae",
        numerus);
    sprintf(numerus, "%u", (insignatus integer)c->signum.apertae);
    (vacuum)stml_attributum_addere(signum, piscina, intern, "apertae",
        numerus);
    sprintf(numerus, "%u", (insignatus integer)c->signum.perfectae);
    (vacuum)stml_attributum_addere(signum, piscina, intern, "perfectae",
        numerus);
    (vacuum)stml_liberum_addere(radix, signum);
    liberos_textus(radix, piscina, intern, "linea", c->lineae_libri);
    redde stml_scribere(radix, piscina, FALSUM);
}


/* ==================================================
 * Registrum
 * ================================================== */

interior vacuum*
opera_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
    Dies d;

    (vacuum)repertorium;
    si (!fasti_parsare_iso(clavis, &d))
    {
        redde NIHIL;
    }
    redde conspectus_dies_computare((constans DiesFontes*)ctx, d,
        piscina);
}

interior chorda
opera_codificare (
             vacuum* ctx,
    constans vacuum* conspectus,
            Piscina* piscina)
{
    (vacuum)ctx;
    redde conspectus_dies_codificare(
        (constans DiesConspectus*)conspectus, piscina);
}

interior i32
agenda_numerus (
    constans vacuum* documentum)
{
    redde agenda_numerus_rerum((constans AgendaDocumentum*)documentum);
}

interior chorda
agenda_id (
    constans vacuum* documentum,
                i32  index,
            Piscina* piscina)
{
    (vacuum)piscina;
    redde agenda_res_index((constans AgendaDocumentum*)documentum,
        index)->id;
}

interior chorda
agenda_vestigium (
    constans vacuum* documentum,
                i32  index,
            Piscina* piscina)
{
    redde agenda_rem_codificare((constans AgendaDocumentum*)documentum,
        index, piscina);
}

/* claves rei: dies posita (res, signa) et dies perfectionis (signa) */
interior vacuum
agenda_claves (
             vacuum* ctx,
    constans vacuum* documentum,
                i32  index,
                Xar* claves,
            Piscina* piscina)
{
    constans AgendaRes* r;

    (vacuum)ctx;
    r = agenda_res_index((constans AgendaDocumentum*)documentum, index);
    si (r->dies_posita)
    {
        *(chorda*)xar_addere(claves) = clavis_diei(r->dies, piscina);
    }
    si (r->perfecta)
    {
        *(chorda*)xar_addere(claves) = clavis_diei(r->dies_perfectionis,
            piscina);
    }
}

b32
conspectus_dies_registrare (
    ConspectusRegistrum*  repertorium,
             DiesFontes*  fontes,
                    Xar** vitia)
{
     ConspectusOpera  opera;
    ConspectusPerRem  per_rem;
             Piscina* p;
                 b32  sana;

    si (vitia)
    {
        *vitia = NIHIL;
    }
    si (!repertorium || !fontes || !fontes->agenda)
    {
        redde FALSUM;
    }
    p = piscina_generare_dynamicum("conspectus_dies_declaratio",
        MXXIV);
    si (!p)
    {
        redde FALSUM;
    }
    sana = conspectus_declarare(repertorium,
        conspectus_declaratio_infixa(
        "dies", p), vitia);
    piscina_destruere(p);
    si (!sana)
    {
        redde FALSUM;
    }
    opera.computare    = opera_computare;
    opera.codificare   = opera_codificare;
    opera.ctx          = fontes;
    per_rem.numerus    = agenda_numerus;
    per_rem.id         = agenda_id;
    per_rem.vestigium  = agenda_vestigium;
    per_rem.claves     = agenda_claves;
    per_rem.ctx        = NIHIL;
    /* parare vocantis est (conspectibus omnibus registratis) */
    redde conspectus_opera_ponere(repertorium, "dies", opera)
        && conspectus_per_rem_ponere(repertorium, "dies", "agenda",
               per_rem)
        && conspectus_documentum_ligare(repertorium, "agenda",
               fontes->agenda);
}
