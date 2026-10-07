/* probatio_aemulator_hospes.c - hospes emulatoris (aemulator-plan B4)
 *
 * I: configuratio. II: pons memoriae (effusio in schirmum, responsa
 * ad infantem, initus, magnitudo ad ambos, effectus vocantis, finis,
 * claudere). III: infans OBSTINATUS (tabula propria probationis -
 * sutura): cauda circularis, reservatum responsorum, limes per
 * pulsum, status constans nihil allocat. IV: concha vera (/bin/sh):
 * effusio, quaestio CPR per hospitem responsa, mutatio magnitudinis,
 * exitus. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "aemulator.h"
#include "pseudoterminale.h"
#include "aemulator_hospes.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina* piscina;

/* infans fictus quem probatio regit */
nomen structura {
    Pseudoterminale  pons;
        constans i8* effusio;
                i32  mensura;
                i32  cursor;
                b32  cyclus;      /* effusio sine fine iteratur */
                b32  finis;       /* post effusionem: -1 (EOF) */
                b32  mortuus;     /* finitus VERUM (etiam ante EOF) */
                i32  frustum;     /* octeti per lectionem; 0 = omnes */
                i32  morae;       /* lectiones cum mora non nulla */
                i32  quota;       /* octeti adhuc accipiendi */
                 i8  captum[MXXIV];
                i32  capti;
} Obstinatus;

interior s32
obstinatus_legere (
    vacuum* datum,
        i8* buffer,
       i32  capacitas,
       s32  mora_ms)
{
    Obstinatus* o;
           i32  n;

    o = (Obstinatus*)datum;
    si (mora_ms != ZEPHYRUM)
    {
        o->morae++;
    }
    si (o->cursor >= o->mensura)
    {
        si (!o->cyclus || o->mensura == ZEPHYRUM)
        {
            redde o->finis ? -I : ZEPHYRUM;
        }
        o->cursor = ZEPHYRUM;
    }
    n = o->mensura - o->cursor;
    si (n > capacitas)
    {
        n = capacitas;
    }
    si (o->frustum && n > o->frustum)
    {
        n = o->frustum;
    }
    memcpy(buffer, o->effusio + o->cursor, (memoriae_index)n);
    o->cursor += n;
    redde (s32)n;
}

interior s32
obstinatus_scribere (
          vacuum* datum,
     constans i8* octeti,
             i32  n)
{
    Obstinatus* o;

    o = (Obstinatus*)datum;
    si (n > o->quota)
    {
        n = o->quota;
    }
    si (n > MXXIV - o->capti)
    {
        n = MXXIV - o->capti;
    }
    memcpy(o->captum + o->capti, octeti, (memoriae_index)n);
    o->capti += n;
    o->quota -= n;
    redde (s32)n;
}

interior b32
obstinatus_amplitudo (
    vacuum* datum,
       i32  latitudo,
       i32  altitudo,
       i32  px_latitudo,
       i32  px_altitudo)
{
    (vacuum)datum;
    (vacuum)latitudo;
    (vacuum)altitudo;
    (vacuum)px_latitudo;
    (vacuum)px_altitudo;
    redde VERUM;
}

interior b32
obstinatus_finitus (
                   vacuum* datum,
    PseudoterminaleExitus* exitus)
{
    si (((Obstinatus*)datum)->mortuus && exitus)
    {
        exitus->codex   = VII;
        exitus->signum  = ZEPHYRUM;
    }
    redde ((Obstinatus*)datum)->mortuus;
}

interior s32
obstinatus_fossa (
    vacuum* datum)
{
    (vacuum)datum;
    redde -I;
}

interior vacuum
obstinatus_claudere (
    vacuum* datum)
{
    (vacuum)datum;
}

interior Pseudoterminale*
obstinatum_creare (
    Obstinatus* o)
{
    memset(o, ZEPHYRUM, magnitudo(Obstinatus));
    o->pons.datum      = o;
    o->pons.legere     = obstinatus_legere;
    o->pons.scribere   = obstinatus_scribere;
    o->pons.amplitudo  = obstinatus_amplitudo;
    o->pons.finitus    = obstinatus_finitus;
    o->pons.fossa      = obstinatus_fossa;
    o->pons.claudere   = obstinatus_claudere;
    redde &o->pons;
}

interior vacuum
effusionem_ponere (
             Obstinatus* o,
     constans character* textus)
{
    o->effusio  = (constans i8*)textus;
    o->mensura  = (i32)strlen(textus);
    o->cursor   = ZEPHYRUM;
}

interior b32
textus_est (
       AemulatorHospes* h,
    constans character* expectatum)
{
    redde chorda_aequalis_literis(aemulator_textum_effundere(
        aemulator_hospes_aemulator(h), piscina), expectatum);
}

interior b32
textus_continet (
       AemulatorHospes* h,
    constans character* literae)
{
    chorda c;
       i32 n;
       i32 i;

    c = aemulator_textum_effundere(aemulator_hospes_aemulator(h),
        piscina);
    n = (i32)strlen(literae);
    per (i = ZEPHYRUM; i + n <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, literae, (memoriae_index)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* effectus vocantis: datum vocantis recipitur */
nomen structura {
    i32 campanae;
    i32 tituli;
} Vocans;

interior vacuum
campanam_numerare (
    vacuum* datum)
{
    ((Vocans*)datum)->campanae++;
}

interior vacuum
titulum_numerare (
     vacuum* datum,
     chorda  titulus)
{
    (vacuum)titulus;
    ((Vocans*)datum)->tituli++;
}

interior AemulatorHospes*
hospitem_creare (
    Pseudoterminale* pt,
                i32  latitudo,
                i32  altitudo,
                i32  per_pulsum,
                i32  capacitas)
{
    AemulatorHospesConfiguratio cfg;

    aemulator_hospes_configuratio_initiare(&cfg);
    cfg.aemulator.latitudo = latitudo;
    cfg.aemulator.altitudo = altitudo;
    si (per_pulsum)
    {
        cfg.octeti_per_pulsum = per_pulsum;
    }
    si (capacitas)
    {
        cfg.cauda_capacitas = capacitas;
    }
    redde aemulator_hospes_creare(piscina, &cfg, pt);
}

/* pulsare donec finitus (decem secunda ad summum) */
interior b32
currere (
    AemulatorHospes* h)
{
    AemulatorHospesPulsus p;
                      i32 i;

    per (i = ZEPHYRUM; i < CC; i++)
    {
        p = aemulator_hospes_pulsare(h, L);
        si (p.finitus)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* pulsare donec textus contineat (decem secunda ad summum) */
interior b32
exspectare_textum (
       AemulatorHospes* h,
    constans character* literae)
{
    i32 i;

    per (i = ZEPHYRUM; i < CC; i++)
    {
        (vacuum)aemulator_hospes_pulsare(h, L);
        si (textus_continet(h, literae))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
configurationem_probare (vacuum)
{
    AemulatorHospesConfiguratio cfg;

    imprimere("\n--- I: configuratio ---\n");
    aemulator_hospes_configuratio_initiare(&cfg);
    CREDO_AEQUALIS_I32(cfg.aemulator.latitudo, LXXX);
    CREDO_AEQUALIS_I32(cfg.aemulator.altitudo, XXIV);
    CREDO_AEQUALIS_I32(cfg.octeti_per_pulsum, LXIV * MXXIV);
    CREDO_AEQUALIS_I32(cfg.cauda_capacitas, LXIV * MXXIV);
    CREDO_NIHIL(aemulator_hospes_creare(piscina, &cfg, NIHIL));
}

interior vacuum
memoriam_probare (vacuum)
{
                Pseudoterminale* pt;
                AemulatorHospes* h;
          AemulatorHospesPulsus  p;
          PseudoterminaleExitus  ex;
    AemulatorHospesConfiguratio  cfg;
                         Vocans  v;
                            i32  lat;
                            i32  alt;

    imprimere("\n--- II: pons memoriae ---\n");
    /* magnitudo nuclei ad infantem in creatione */
    pt = pseudoterminale_memoriae_creare(piscina,
        (constans i8*)"salve\x1B[6n", IX, V);
    h  = hospitem_creare(pt, XXX, VII, ZEPHYRUM, ZEPHYRUM);
    CREDO_NON_NIHIL(h);
    pseudoterminale_memoriae_amplitudo(pt, &lat, &alt);
    CREDO_AEQUALIS_I32(lat, XXX);
    CREDO_AEQUALIS_I32(alt, VII);
    CREDO_AEQUALIS_S32(aemulator_hospes_fossa(h), -I);
    /* initus ante pulsum; deinde effusio, responsum CPR post eum */
    CREDO_AEQUALIS_I32(aemulator_hospes_scribere(h,
        (constans i8*)"ls\r", III), III);
    CREDO_FALSUM(aemulator_hospes_exitus(h, &ex));
    p = aemulator_hospes_pulsare(h, ZEPHYRUM);
    CREDO_AEQUALIS_I32(p.lecti, IX);
    CREDO_VERUM(p.mutatum);
    CREDO_VERUM(p.finitus);
    CREDO_VERUM(textus_est(h, "salve"));
    CREDO_VERUM(chorda_aequalis_literis(
        pseudoterminale_memoriae_captum(pt), "ls\r\x1B[1;6R"));
    CREDO_AEQUALIS_I32(p.missi, IX);
    CREDO_VERUM(aemulator_hospes_exitus(h, &ex));
    CREDO_AEQUALIS_I32(ex.codex, V);
    /* pulsus vacuus: nihil mutatum; magnitudo: mutatum, ad ambos */
    p = aemulator_hospes_pulsare(h, ZEPHYRUM);
    CREDO_FALSUM(p.mutatum);
    CREDO_VERUM(p.finitus);
    CREDO_VERUM(aemulator_hospes_amplitudo(h, XL, IX, ZEPHYRUM,
                                           ZEPHYRUM));
    CREDO_FALSUM(aemulator_hospes_amplitudo(h, ZEPHYRUM, IX, ZEPHYRUM,
                                            ZEPHYRUM));
    CREDO_AEQUALIS_I32(
        aemulator_latitudo(aemulator_hospes_aemulator(h)), XL);
    pseudoterminale_memoriae_amplitudo(pt, &lat, &alt);
    CREDO_AEQUALIS_I32(lat, XL);
    CREDO_AEQUALIS_I32(alt, IX);
    CREDO_VERUM(aemulator_hospes_pulsare(h, ZEPHYRUM).mutatum);
    /* claudere idempotens */
    aemulator_hospes_claudere(h);
    aemulator_hospes_claudere(h);
    CREDO_VERUM(aemulator_hospes_pulsare(h, ZEPHYRUM).finitus);
    CREDO_AEQUALIS_I32(aemulator_hospes_scribere(h,
        (constans i8*)"x", I), ZEPHYRUM);

    /* campana et titulus ad vocantem cum datis SUIS */
    memset(&v, ZEPHYRUM, magnitudo(v));
    pt = pseudoterminale_memoriae_creare(piscina,
        (constans i8*)"\x07\x1B]2;t\x07", VII, ZEPHYRUM);
    aemulator_hospes_configuratio_initiare(&cfg);
    cfg.aemulator.effectus.datum = &v;
    cfg.aemulator.effectus.campana = campanam_numerare;
    cfg.aemulator.effectus.titulus = titulum_numerare;
    h = aemulator_hospes_creare(piscina, &cfg, pt);
    (vacuum)aemulator_hospes_pulsare(h, ZEPHYRUM);
    CREDO_AEQUALIS_I32(v.campanae, I);
    CREDO_AEQUALIS_I32(v.tituli, I);
}

interior vacuum
obstinatum_probare (vacuum)
{
                     Obstinatus  o;
                Pseudoterminale* pt;
                AemulatorHospes* h;
          AemulatorHospesPulsus  p;
    AemulatorHospesConfiguratio  cfg;
                             i8  b[C];
                             i8  expectatum[C];
                 memoriae_index  usus;
                            i32  i;

    imprimere("\n--- III: infans obstinatus ---\n");
    /* reservatum: cauda LXIV, initus LX ad summum; responsum in
     * reservato manet donec infans accipit */
    pt  = obstinatum_creare(&o);
    h   = hospitem_creare(pt, X, III, ZEPHYRUM, LXIV);
    memset(b, 'k', magnitudo(b));
    CREDO_AEQUALIS_I32(aemulator_hospes_scribere(h, b, C), LX);
    CREDO_AEQUALIS_I32(aemulator_hospes_scribere(h, b, I), ZEPHYRUM);
    effusionem_ponere(&o, "\x1B[5n");
    p = aemulator_hospes_pulsare(h, ZEPHYRUM);
    CREDO_AEQUALIS_I32(p.lecti, IV);
    CREDO_AEQUALIS_I32(p.missi, ZEPHYRUM);
    /* cauda plena ultra limitem initus (reservatum usum): initus
     * recusatur (i32 insignatus - limes minus mensura non cadat) */
    CREDO_AEQUALIS_I32(aemulator_hospes_scribere(h, b, I), ZEPHYRUM);
    o.quota  = M;
    p        = aemulator_hospes_pulsare(h, ZEPHYRUM);
    CREDO_AEQUALIS_I32(p.missi, LXIV);
    memset(expectatum, 'k', LX);
    memcpy(expectatum + LX, "\x1B[0n", IV);
    CREDO_AEQUALIS_I32(o.capti, LXIV);
    CREDO_VERUM(memcmp(o.captum, expectatum, LXIV) == ZEPHYRUM);

    /* cauda circularis: missio partialis, deinde scriptura trans
     * finem sacculi; ordo servatur; initus usque ad LX (limes),
     * excessus recusatur */
    pt  = obstinatum_creare(&o);
    h   = hospitem_creare(pt, X, III, ZEPHYRUM, LXIV);
    per (i = ZEPHYRUM; i < LX; i++)
    {
        b[i] = (i8)('a' + i % XXVI);
    }
    CREDO_AEQUALIS_I32(aemulator_hospes_scribere(h, b, L), L);
    o.quota = XX;
    CREDO_AEQUALIS_I32(aemulator_hospes_pulsare(h, ZEPHYRUM).missi, XX);
    CREDO_AEQUALIS_I32(aemulator_hospes_scribere(h, b + L, X), X);
    CREDO_AEQUALIS_I32(aemulator_hospes_scribere(h, b, XXX), XX);
    o.quota = M;
    CREDO_AEQUALIS_I32(aemulator_hospes_pulsare(h, ZEPHYRUM).missi,
                       LX);
    memcpy(expectatum, b, LX);
    memcpy(expectatum + LX, b, XX);
    CREDO_AEQUALIS_I32(o.capti, LXXX);
    CREDO_VERUM(memcmp(o.captum, expectatum, LXXX) == ZEPHYRUM);

    /* limes per pulsum: effusio sine fine, CCLVI per pulsum */
    pt  = obstinatum_creare(&o);
    h   = hospitem_creare(pt, XX, V, CCLVI, ZEPHYRUM);
    effusionem_ponere(&o, "linea\r\n");
    o.cyclus = VERUM;
    CREDO_AEQUALIS_I32(aemulator_hospes_pulsare(h, ZEPHYRUM).lecti,
                       CCLVI);
    CREDO_AEQUALIS_I32(aemulator_hospes_pulsare(h, ZEPHYRUM).lecti,
                       CCLVI);

    /* mora solum ante octetum primum: tres lectiones, una mora */
    pt  = obstinatum_creare(&o);
    h   = hospitem_creare(pt, XX, V, ZEPHYRUM, ZEPHYRUM);
    effusionem_ponere(&o, "abcdefghi");
    o.frustum = III;
    CREDO_AEQUALIS_I32(aemulator_hospes_pulsare(h, L).lecti, IX);
    CREDO_AEQUALIS_I32(o.morae, I);

    /* finitus = EOF ET messus: infans mortuus sed effusio nondum
     * lecta -> nondum finitus; EOF sine messe -> nondum finitus */
    pt  = obstinatum_creare(&o);
    h   = hospitem_creare(pt, XX, V, IV, ZEPHYRUM);
    effusionem_ponere(&o, "abcdef");
    o.finis    = VERUM;
    o.mortuus  = VERUM;
    CREDO_VERUM(aemulator_hospes_exitus(h, NIHIL));
    p = aemulator_hospes_pulsare(h, ZEPHYRUM);
    CREDO_AEQUALIS_I32(p.lecti, IV);
    CREDO_FALSUM(p.finitus);
    p = aemulator_hospes_pulsare(h, ZEPHYRUM);
    CREDO_AEQUALIS_I32(p.lecti, II);
    CREDO_VERUM(p.finitus);
    CREDO_VERUM(textus_est(h, "abcdef"));
    pt       = obstinatum_creare(&o);
    h        = hospitem_creare(pt, XX, V, ZEPHYRUM, ZEPHYRUM);
    o.finis  = VERUM;
    CREDO_FALSUM(aemulator_hospes_pulsare(h, ZEPHYRUM).finitus);
    o.mortuus = VERUM;
    CREDO_VERUM(aemulator_hospes_pulsare(h, ZEPHYRUM).finitus);

    /* status constans: lectio, responsa, missio - nihil allocat
     * (historia pagina una: calefactio eam implet, deinde
     * recyclatur) */
    pt  = obstinatum_creare(&o);
    aemulator_hospes_configuratio_initiare(&cfg);
    cfg.aemulator.latitudo = XX;
    cfg.aemulator.altitudo = V;
    cfg.aemulator.historia_octeti = I;
    h = aemulator_hospes_creare(piscina, &cfg, pt);
    effusionem_ponere(&o, "linea \x1B[1mX\x1B[0m\x1B[6n\r\n");
    o.cyclus  = VERUM;
    o.quota   = M * M;
    (vacuum)aemulator_hospes_pulsare(h, ZEPHYRUM);
    o.capti  = ZEPHYRUM;
    usus     = piscina_summa_usus(piscina);
    per (i = ZEPHYRUM; i < C; i++)
    {
        (vacuum)aemulator_hospes_scribere(h, (constans i8*)"q", I);
        (vacuum)aemulator_hospes_pulsare(h, ZEPHYRUM);
        o.capti = ZEPHYRUM;
    }
    CREDO_VERUM(piscina_summa_usus(piscina) == usus);
}

interior vacuum
concham_probare (vacuum)
{
                Pseudoterminale* pt;
                AemulatorHospes* h;
          PseudoterminaleExitus  ex;
    PseudoterminaleConfiguratio  cfg;
             constans character* effusio[]  = { "/bin/sh", "-c",
        "printf 'salve\\nmunde'; exit 4", NIHIL };
    constans character* quaestio[] = { "/bin/sh", "-c",
        "stty -icanon -echo; printf 'ab\\033[6n'; "
        "IFS= read -r -d R r; printf '|%s|' \"${r#?}\"", NIHIL };
    constans character* mutata[]   = { "/bin/sh", "-c",
        "printf parata; read x; stty size", NIHIL };

    imprimere("\n--- IV: concha vera ---\n");
    pseudoterminale_configuratio_initiare(&cfg);

    /* effusio in schirmum; exitus */
    cfg.argumenta = effusio;
    pt = pseudoterminale_posix_creare(piscina, &cfg, NIHIL, NIHIL);
    h = hospitem_creare(pt, XX, V, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(aemulator_hospes_fossa(h) >= ZEPHYRUM);
    CREDO_VERUM(currere(h));
    CREDO_VERUM(textus_est(h, "salve\nmunde"));
    CREDO_VERUM(aemulator_hospes_exitus(h, &ex));
    CREDO_AEQUALIS_I32(ex.codex, IV);
    aemulator_hospes_claudere(h);

    /* quaestio CPR: nucleus respondet, hospes ad infantem mittit,
     * infans responsum legit et imprimit */
    cfg.argumenta = quaestio;
    pt = pseudoterminale_posix_creare(piscina, &cfg, NIHIL, NIHIL);
    h = hospitem_creare(pt, XX, V, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(currere(h));
    CREDO_VERUM(textus_est(h, "ab|[1;3|"));
    aemulator_hospes_claudere(h);

    /* mutatio magnitudinis per hospitem ad infantem */
    cfg.argumenta = mutata;
    pt = pseudoterminale_posix_creare(piscina, &cfg, NIHIL, NIHIL);
    h = hospitem_creare(pt, XXX, VII, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(exspectare_textum(h, "parata"));
    CREDO_VERUM(aemulator_hospes_amplitudo(h, XL, IX, ZEPHYRUM,
                                           ZEPHYRUM));
    CREDO_AEQUALIS_I32(aemulator_hospes_scribere(h,
        (constans i8*)"\r", I), I);
    CREDO_VERUM(currere(h));
    CREDO_VERUM(textus_continet(h, "9 40"));
    aemulator_hospes_claudere(h);
}

s32
principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_aemulator_hospes",
        CCLVI * MXXIV);
    credo_aperire(piscina);

    configurationem_probare();
    memoriam_probare();
    obstinatum_probare();
    concham_probare();

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
