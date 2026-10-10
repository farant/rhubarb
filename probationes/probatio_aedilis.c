/* probatio_aedilis.c - Probationes fundamentorum aedilis (spica 0)
 *
 * Aedilis in via et stml innititur; haec probatio mores quibus
 * fidit FIGIT (lex calibrationis oraculi: specimina ante usum):
 *   - via_normalizare / via_iungere: catena resolutionis
 *     inclusionum ("dirname includentis + rogata -> normalizata")
 *   - stml scriptio/lectio: valores attributorum VERBATIM redeunt
 *     (lector effugia numquam solvit), SED quota duplex (") in
 *     valore NON repraesentabilis est - scriptor crude scribit,
 *     documentum frangitur. Aedilis ergo quotam in valoribus
 *     recusabit (phasis A).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include "xar.h"
#include "stml.h"
#include "internamentum.h"
#include "aedilis.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>


/* ========================================================
 * Extractor fixturarum - machina sine silva agitur: tabula
 * directivarum super fixturas disci veras
 * (probationes/fixa/aedilis/ = radix parva). Plagula ignota =
 * imparsabilis (FALSUM) - tabula clausuram exacte tegere debet.
 * ======================================================== */

interior vacuum
_fix_addere (
                   Xar* xar,
    constans character* literis,
               Piscina* piscina)
{
    chorda* locus;

    locus = (chorda*)xar_addere(xar);
    si (locus != NIHIL)
    {
        *locus = chorda_ex_literis(literis, piscina);
    }
}

interior vacuum
_formam_addere (
    Xar* xar,
    b32  angulata)
{
    b32* locus;

    locus = (b32*)xar_addere(xar);
    si (locus != NIHIL)
    {
        *locus = angulata;
    }
}

interior b32
_extractor_fixturarum (
                vacuum*  datum,
    constans character*  via,
               Piscina*  piscina,
                   Xar** directivae_out,
                   Xar** annotationes_out,
                   b32*  ex_oraculo_out,
                   Xar** angulatae_out)
{
    (vacuum)datum;
    *directivae_out = xar_creare(piscina, (i32)magnitudo(chorda));
    *annotationes_out = xar_creare(piscina,
        (i32)magnitudo(chorda));
    *ex_oraculo_out = FALSUM;
    /* forma ignota ubique praeter citata.c: regula vetus
     * (inresoluta = systema) ibi probatur */
    *angulatae_out = NIHIL;

    si (strstr(via, "fons/citata.c") != NIHIL)
    {
        *angulatae_out = xar_creare(piscina, (i32)magnitudo(b32));
        _fix_addere(*directivae_out, "alpha.h", piscina);
        _formam_addere(*angulatae_out, FALSUM);
        _fix_addere(*directivae_out, "nusquam.h", piscina);
        _formam_addere(*angulatae_out, FALSUM);
        _fix_addere(*directivae_out, "stdint.h", piscina);
        _formam_addere(*angulatae_out, VERUM);
    }
    alioquin si (strstr(via, "fons/scopus.c") != NIHIL)
    {
        _fix_addere(*directivae_out, "alpha.h", piscina);
        _fix_addere(*directivae_out, "beta.h", piscina);
        _fix_addere(*directivae_out, "delta.h", piscina);
        _fix_addere(*directivae_out, "librum.h", piscina);
        _fix_addere(*directivae_out, "stdio.h", piscina);
        _fix_addere(*annotationes_out,
            "obiectum gen/generatum.c", piscina);
        _fix_addere(*annotationes_out, "vexillum -DPROBA",
            piscina);
    }
    alioquin si (strstr(via, "lib/alpha.c") != NIHIL)
    {
        _fix_addere(*directivae_out, "alpha.h", piscina);
        _fix_addere(*directivae_out, "gamma.h", piscina);
    }
    alioquin si (strstr(via, "lib/beta_macos.m") != NIHIL)
    {
        /* cursus minoritatis simulatus: viae iam resolutae */
        *ex_oraculo_out = VERUM;
        _fix_addere(*directivae_out, "include/beta.h", piscina);
    }
    alioquin si (strstr(via, "lib/beta_posix.c") != NIHIL)
    {
        _fix_addere(*directivae_out, "beta.h", piscina);
    }
    alioquin si (strstr(via, "lib/delta_verum.c") != NIHIL)
    {
        _fix_addere(*directivae_out, "delta.h", piscina);
    }
    alioquin si (strstr(via, "fons/annotatio_mala.c") != NIHIL)
    {
        _fix_addere(*annotationes_out, "ignotum quid", piscina);
    }
    alioquin si (strstr(via, "fons/fenestralis.c") != NIHIL)
    {
        /* facultas (fabrica-6 T6): scopi solius */
        _fix_addere(*directivae_out, "gamma.h", piscina);
        _fix_addere(*annotationes_out, "facultas fenestra", piscina);
    }
    alioquin si (strstr(via, "fons/facultas_ignota.c") != NIHIL)
    {
        _fix_addere(*annotationes_out, "facultas tactus", piscina);
    }
    alioquin si (strstr(via, "fons/includens_facultatem.c") != NIHIL)
    {
        _fix_addere(*directivae_out, "facultas_caput.h", piscina);
    }
    alioquin si (strstr(via, "include/facultas_caput.h") != NIHIL)
    {
        /* caput pro includentibus facultatem declarare nequit */
        _fix_addere(*annotationes_out, "facultas rete", piscina);
    }
    alioquin si (strstr(via, "fons/instrumentalis.c") != NIHIL)
    {
        /* instrumentum (fabrica-7 T2): binaria domus quae scopus
         * currit - scopi solius, sub bin/ */
        _fix_addere(*annotationes_out, "instrumentum bin/natura",
            piscina);
        _fix_addere(*annotationes_out, "instrumentum bin/generare",
            piscina);
    }
    alioquin si (strstr(via, "fons/instrumentum_extra.c") != NIHIL)
    {
        _fix_addere(*annotationes_out, "instrumentum tools/natura.sh",
            piscina);
    }
    alioquin si (strstr(via, "fons/instrumentum_duplex.c") != NIHIL)
    {
        _fix_addere(*annotationes_out, "instrumentum bin/natura",
            piscina);
        _fix_addere(*annotationes_out, "instrumentum bin/natura",
            piscina);
    }
    alioquin si (strstr(via, "fons/includens_instrumentum.c") != NIHIL)
    {
        _fix_addere(*directivae_out, "instrumentum_caput.h", piscina);
    }
    alioquin si (strstr(via, "include/instrumentum_caput.h") != NIHIL)
    {
        /* caput pro includentibus instrumentum declarare nequit */
        _fix_addere(*annotationes_out, "instrumentum bin/natura",
            piscina);
    }
    alioquin si (strstr(via, "fons/corpus_malus.c") != NIHIL)
    {
        _fix_addere(*annotationes_out,
            "corpus lib/nusquam_est.c", piscina);
    }
    alioquin si (strstr(via, "include/alpha.h") != NIHIL)
    {
        /* caput implementatorem alterum declarat (verbum corpus) */
        _fix_addere(*annotationes_out,
            "corpus lib/alpha_socius.c", piscina);
    }
    alioquin si (strstr(via, "lib/alpha_socius.c") != NIHIL)
    {
        /* corpus AMBULATUR: directiva eius clausurae accedit */
        _fix_addere(*directivae_out, "epsilon.h", piscina);
    }
    alioquin si (strstr(via, "include/beta.h") != NIHIL)
    {
        /* arista capitis: beta -> gamma (graphi/topologiae
         * probationes; gamma iam per lib/alpha.c in clausura -
         * numeri immutati) */
        _fix_addere(*directivae_out, "gamma.h", piscina);
    }
    alioquin si (strstr(via, "fons/cyclicus.c") != NIHIL)
    {
        _fix_addere(*directivae_out, "cyclus_a.h", piscina);
    }
    alioquin si (strstr(via, "include/cyclus_a.h") != NIHIL)
    {
        _fix_addere(*directivae_out, "cyclus_b.h", piscina);
    }
    alioquin si (strstr(via, "include/cyclus_b.h") != NIHIL)
    {
        _fix_addere(*directivae_out, "cyclus_a.h", piscina);
    }
    alioquin si (strstr(via, "include/purum.h") != NIHIL)
    {
        /* nexus purus (A1b): clausura sine regula nexus */
        _fix_addere(*directivae_out, "gamma.h", piscina);
        _fix_addere(*annotationes_out, "nexus purus", piscina);
    }
    alioquin si (strstr(via, "include/impurum.h") != NIHIL)
    {
        /* promittit, sed beta.h -> lib/beta_macos.m (Cocoa) */
        _fix_addere(*directivae_out, "beta.h", piscina);
        _fix_addere(*annotationes_out, "nexus purus", piscina);
    }
    alioquin si (strstr(via, "include/orbis_a.h") != NIHIL)
    {
        /* catena cum cyclo (b <-> c): nominatio non circuit */
        _fix_addere(*directivae_out, "orbis_b.h", piscina);
        _fix_addere(*annotationes_out, "nexus purus", piscina);
    }
    alioquin si (strstr(via, "include/orbis_b.h") != NIHIL)
    {
        _fix_addere(*directivae_out, "orbis_c.h", piscina);
    }
    alioquin si (strstr(via, "include/orbis_c.h") != NIHIL)
    {
        _fix_addere(*directivae_out, "orbis_b.h", piscina);
        _fix_addere(*directivae_out, "beta.h", piscina);
    }
    alioquin si (strstr(via, "include/proprium.h") != NIHIL)
    {
        /* promittit; implementatio PROPRIA (lib/proprium_macos.m)
         * regulam nexus fert - caput ipsum, non inclusum */
        _fix_addere(*annotationes_out, "nexus purus", piscina);
    }
    alioquin si (strstr(via, "lib/proprium_macos.m") != NIHIL)
    {
        *ex_oraculo_out = VERUM;
    }
    alioquin si (strstr(via, "include/nexus_ignotum.h") != NIHIL)
    {
        _fix_addere(*annotationes_out, "nexus mixtus", piscina);
    }
    alioquin si (   strstr(via, "include/gamma.h")   != NIHIL
                 || strstr(via, "include/delta.h")   != NIHIL
                 || strstr(via, "include/epsilon.h") != NIHIL)
    {
        /* capita fixturarum: sine directivis */
    }
    alioquin
    {
        redde FALSUM;
    }
    redde VERUM;
}

interior AedilisCaput*
_caput_invenire (
        AedilisFructus* fructus,
    constans character* via)
{
    i32 i;
    i32 numerus;

    numerus = xar_numerus(fructus->capita);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        AedilisCaput* caput;

        caput = (AedilisCaput*)xar_obtinere(fructus->capita, i);
        si (chorda_aequalis_literis(caput->via, via))
        {
            redde caput;
        }
    }
    redde NIHIL;
}

interior AedilisObiectum*
_obiectum_invenire (
        AedilisFructus* fructus,
    constans character* via)
{
    i32 i;
    i32 numerus;

    numerus = xar_numerus(fructus->obiecta);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        AedilisObiectum* obiectum;

        obiectum = (AedilisObiectum*)xar_obtinere(
            fructus->obiecta, i);
        si (chorda_aequalis_literis(obiectum->via, via))
        {
            redde obiectum;
        }
    }
    redde NIHIL;
}

s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_aedilis", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: via_normalizare - pinnae resolutionis aedilis
     * ======================================================== */

    {
        chorda v;

        imprimere("\n--- Probans via_normalizare (pinnae) ---\n");

        v = via_normalizare(chorda_ex_literis("foo/../bar", piscina),
            piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(v, "bar");

        v = via_normalizare(chorda_ex_literis("foo/./bar", piscina),
            piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(v, "foo/bar");

        v = via_normalizare(chorda_ex_literis("a//b", piscina),
            piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(v, "a/b");

        v = via_normalizare(chorda_ex_literis("../x", piscina),
            piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(v, "../x");

        v = via_normalizare(chorda_ex_literis("/foo/../bar",
            piscina), piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(v, "/bar");
    }


    /* ========================================================
     * PROBARE: catena resolutionis (via_iungere + normalizare)
     * ======================================================== */

    {
        chorda partes[2];
        chorda iuncta;
        chorda v;

        imprimere("\n--- Probans catenam resolutionis ---\n");

        /* includens lib/hospitium.c rogat "via.h" relate ad
         * directorium suum, deinde -I "include" temptatur; forma
         * "..": speculum-classis (lib/../include/x.h) */
        partes[ZEPHYRUM] = chorda_ex_literis("lib", piscina);
        partes[I] = chorda_ex_literis("../include/hospitium.h",
            piscina);
        iuncta = via_iungere(partes, II, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(iuncta,
            "lib/../include/hospitium.h");

        v = via_normalizare(iuncta, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(v, "include/hospitium.h");
    }


    /* ========================================================
     * PROBARE: stml circuitus - valores attributorum verbatim
     * ======================================================== */

    {
        InternamentumChorda* intern;
                  StmlNodus* obiectum;
               StmlResultus  relectum;
                     chorda  scriptum;
                     chorda* valor;

        imprimere("\n--- Probans stml circuitum attributorum ---\n");

        intern = internamentum_creare(piscina);
        CREDO_NON_NIHIL(intern);

        obiectum = stml_elementum_creare(piscina, intern, "obiectum");
        CREDO_NON_NIHIL(obiectum);
        CREDO_VERUM(stml_attributum_addere(obiectum, piscina, intern,
            "via", "lib/piscina.c"));
        CREDO_VERUM(stml_attributum_addere(obiectum, piscina, intern,
            "vexilla", "-framework Cocoa -O2 -DGRADUS=3"));

        scriptum = stml_scribere(obiectum, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(scriptum);

        relectum = stml_legere(scriptum, piscina, intern);
        CREDO_VERUM(relectum.successus);
        CREDO_NON_NIHIL(relectum.elementum_radix);

        valor = stml_attributum_capere(relectum.elementum_radix,
            "via");
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "lib/piscina.c");

        valor = stml_attributum_capere(relectum.elementum_radix,
            "vexilla");
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor,
            "-framework Cocoa -O2 -DGRADUS=3");
    }


    /* ========================================================
     * PROBARE: characteres asperi in valoribus (verbatim praeter ")
     * ======================================================== */

    {
        InternamentumChorda* intern;
                  StmlNodus* nodus;
               StmlResultus  relectum;
                     chorda  scriptum;
                     chorda* valor;

        imprimere("\n--- Probans characteres asperos ---\n");

        intern = internamentum_creare(piscina);

        /* < > & ' intra valorem quotatum: lector ad quotam
         * clausuram tantum currit - verbatim supersunt */
        nodus = stml_elementum_creare(piscina, intern, "res");
        CREDO_VERUM(stml_attributum_addere(nodus, piscina, intern,
            "asper", "a<b>c&d'e"));
        scriptum = stml_scribere(nodus, piscina, FALSUM);
        relectum = stml_legere(scriptum, piscina, intern);
        CREDO_VERUM(relectum.successus);
        valor = stml_attributum_capere(relectum.elementum_radix,
            "asper");
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "a<b>c&d'e");

        /* quota duplex: scriptor CRUDE scribit (nullum effugium) -
         * documentum emissum quotam praematuram continet, valor
         * relectus truncatur. PINNA LIMITIS: si stml umquam
         * effugia discit, haec probatio frangitur et aedilis
         * custodiam suam relaxare potest. */
        nodus = stml_elementum_creare(piscina, intern, "res");
        CREDO_VERUM(stml_attributum_addere(nodus, piscina, intern,
            "quota", "a\"b"));
        scriptum = stml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_CONTINET(scriptum,
            chorda_ex_literis("a\"b", piscina));
        relectum = stml_legere(scriptum, piscina, intern);
        valor = stml_attributum_capere(relectum.elementum_radix,
            "quota");
        si (valor != NIHIL)
        {
            CREDO_FALSUM(chorda_aequalis_literis(*valor, "a\"b"));
        }
        alioquin
        {
            CREDO_NIHIL(valor);
        }
    }


    /* ========================================================
     * PROBARE: circuitus manifesti figurati (pulchrum)
     * ======================================================== */

    {
        InternamentumChorda* intern;
                  StmlNodus* radix;
                  StmlNodus* obiecta;
                  StmlNodus* obiectum;
                  StmlNodus* inventum;
               StmlResultus  relectum;
                     chorda  scriptum;
                     chorda* valor;
                        Xar* filii;

        imprimere("\n--- Probans manifestum figuratum ---\n");

        intern = internamentum_creare(piscina);

        radix = stml_elementum_creare(piscina, intern,
            "aedilis-manifestum");
        CREDO_VERUM(stml_attributum_addere(radix, piscina, intern,
            "scopus", "lib/hospitium.c"));

        obiecta = stml_elementum_creare(piscina, intern, "obiecta");
        CREDO_VERUM(stml_liberum_addere(radix, obiecta));

        obiectum = stml_elementum_creare(piscina, intern,
            "obiectum");
        CREDO_VERUM(stml_attributum_addere(obiectum, piscina,
            intern, "via", "lib/piscina.c"));
        CREDO_VERUM(stml_attributum_addere(obiectum, piscina,
            intern, "origo", "derivatum"));
        CREDO_VERUM(stml_liberum_addere(obiecta, obiectum));

        obiectum = stml_elementum_creare(piscina, intern,
            "obiectum");
        CREDO_VERUM(stml_attributum_addere(obiectum, piscina,
            intern, "via", "lib/tcp_macos.c"));
        CREDO_VERUM(stml_attributum_addere(obiectum, piscina,
            intern, "origo", "aedilis.stml"));
        CREDO_VERUM(stml_liberum_addere(obiecta, obiectum));

        scriptum = stml_scribere(radix, piscina, VERUM);
        CREDO_CHORDA_NON_VACUA(scriptum);

        relectum = stml_legere(scriptum, piscina, intern);
        CREDO_VERUM(relectum.successus);
        CREDO_NON_NIHIL(relectum.elementum_radix);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *relectum.elementum_radix->titulus, "aedilis-manifestum");

        valor = stml_attributum_capere(relectum.elementum_radix,
            "scopus");
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "lib/hospitium.c");

        inventum = stml_invenire_liberum(relectum.elementum_radix,
            "obiecta");
        CREDO_NON_NIHIL(inventum);
        filii = stml_invenire_omnes_liberos(inventum, "obiectum",
            piscina);
        CREDO_NON_NIHIL(filii);
        CREDO_AEQUALIS_I32(xar_numerus(filii), II);

        obiectum  = *(StmlNodus**)xar_obtinere(filii, I);
        valor     = stml_attributum_capere(obiectum, "origo");
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "aedilis.stml");
    }


    /* ========================================================
     * PROBARE: configurationem legere (fixtura aedilis.stml)
     * ======================================================== */

    {
        AedilisConfiguratio* configuratio;
                     chorda  causa;

        imprimere("\n--- Probans configurationem ---\n");

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        configuratio = aedilis_configurationem_legere(piscina,
            "probationes/fixa/aedilis/aedilis.stml", &causa);
        CREDO_NON_NIHIL(configuratio);
        CREDO_CHORDA_AEQUALIS_LITERIS(configuratio->radix,
            "probationes/fixa/aedilis");
        CREDO_AEQUALIS_I32(xar_numerus(configuratio->inclusa), II);
        CREDO_AEQUALIS_I32(xar_numerus(configuratio->vexilla), II);
        CREDO_AEQUALIS_I32(xar_numerus(configuratio->variantes),
            II);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *(chorda*)xar_obtinere(configuratio->variantes,
                ZEPHYRUM), "macos");
        /* II: beta_macos + proprium_macos (eventus A1b) */
        CREDO_AEQUALIS_I32(
            xar_numerus(configuratio->regulae_nexus), II);
        CREDO_AEQUALIS_I32(
            xar_numerus(configuratio->regulae_vendor), I);
        CREDO_AEQUALIS_I32(
            xar_numerus(configuratio->irregularia), I);
        /* regula compilationis per fontem (fabrica T8: sigillum -O2) */
        CREDO_AEQUALIS_I32(
            xar_numerus(configuratio->regulae_compilationis), I);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            ((AedilisRegulaCompilationis*)xar_obtinere(
                configuratio->regulae_compilationis, ZEPHYRUM))->fons,
            "lib/alpha.c");

        /* configuratio absens = recusatio */
        CREDO_NIHIL(aedilis_configurationem_legere(piscina,
            "probationes/fixa/aedilis/abest.stml", &causa));
        CREDO_CHORDA_NON_VACUA(causa);
    }


    /* ========================================================
     * PROBARE: derivationem (extractor fixturarum, disci verae)
     * ======================================================== */

    {
        AedilisConfiguratio* configuratio;
             AedilisFructus* fructus;
            AedilisObiectum* obiectum;
              AedilisVendor* vendor;
                     chorda  causa;

        imprimere("\n--- Probans derivationem ---\n");

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        configuratio = aedilis_configurationem_legere(piscina,
            "probationes/fixa/aedilis/aedilis.stml", &causa);
        CREDO_NON_NIHIL(configuratio);

        fructus = aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", NIHIL, _extractor_fixturarum, NIHIL,
            &causa);
        CREDO_NON_NIHIL(fructus);
        CREDO_CHORDA_AEQUALIS_LITERIS(fructus->varians, "macos");

        /* obiecta: alpha (conventio), beta_macos (varians),
         * delta_verum (irregulare), generatum (annotatio),
         * alpha_socius (corpus ex capite) */
        CREDO_AEQUALIS_I32(xar_numerus(fructus->obiecta), V);

        /* corpus: caput implementatorem declarat; COMPONITUR cum
         * proba (alpha.c manet) et AMBULATUR (epsilon in capitibus
         * ambulationem probat) */
        obiectum = _obiectum_invenire(fructus,
            "lib/alpha_socius.c");
        CREDO_NON_NIHIL(obiectum);
        CREDO_AEQUALIS_I32(obiectum->origo, AEDILIS_ORIGO_CORPUS);
        CREDO_CHORDA_AEQUALIS_LITERIS(obiectum->caput,
            "include/alpha.h");

        obiectum = _obiectum_invenire(fructus, "lib/alpha.c");
        CREDO_NON_NIHIL(obiectum);
        CREDO_AEQUALIS_I32(obiectum->origo,
            AEDILIS_ORIGO_DERIVATUM);
        CREDO_FALSUM(obiectum->absens);
        CREDO_CHORDA_AEQUALIS_LITERIS(obiectum->caput,
            "include/alpha.h");

        obiectum = _obiectum_invenire(fructus,
            "lib/beta_macos.m");
        CREDO_NON_NIHIL(obiectum);
        CREDO_NON_NIHIL(obiectum->vexilla_nexus);
        CREDO_AEQUALIS_I32(xar_numerus(obiectum->vexilla_nexus),
            I);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *(chorda*)xar_obtinere(obiectum->vexilla_nexus,
                ZEPHYRUM), "-framework Cocoa");
        CREDO_NIHIL(_obiectum_invenire(fructus,
            "lib/beta_posix.c"));

        obiectum = _obiectum_invenire(fructus,
            "lib/delta_verum.c");
        CREDO_NON_NIHIL(obiectum);
        CREDO_AEQUALIS_I32(obiectum->origo,
            AEDILIS_ORIGO_CONFIGURATIO);

        obiectum = _obiectum_invenire(fructus, "gen/generatum.c");
        CREDO_NON_NIHIL(obiectum);
        CREDO_AEQUALIS_I32(obiectum->origo,
            AEDILIS_ORIGO_ANNOTATIO);
        CREDO_VERUM(obiectum->absens);

        /* capita sex (alpha beta gamma delta + librum.h
         * vendicatum + epsilon ex ambulatione corporis); systema
         * unum; vendor unus cum vexillis; vexillum annotatum unum */
        CREDO_AEQUALIS_I32(xar_numerus(fructus->capita), VI);
        CREDO_AEQUALIS_I32(xar_numerus(fructus->systemata), I);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *(chorda*)xar_obtinere(fructus->systemata, ZEPHYRUM),
            "stdio.h");
        CREDO_AEQUALIS_I32(xar_numerus(fructus->vendores), I);
        vendor = (AedilisVendor*)xar_obtinere(fructus->vendores,
            ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(vendor->fons,
            "vendor/librum.c");
        CREDO_NON_NIHIL(vendor->vexilla);
        CREDO_AEQUALIS_I32(
            xar_numerus(fructus->vexilla_annotata), I);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *(chorda*)xar_obtinere(fructus->vexilla_annotata,
                ZEPHYRUM), "-DPROBA");

        /* aristae graphi inclusionum: beta -> gamma (recordata
         * etiam cum gamma iam viso - dedup capitum aristas non
         * devorat); alpha ambulatum sine directivis = NIHIL */
        {
            AedilisCaput* caput;

            caput = _caput_invenire(fructus, "include/beta.h");
            CREDO_NON_NIHIL(caput);
            CREDO_NON_NIHIL(caput->inclusa);
            CREDO_AEQUALIS_I32(xar_numerus(caput->inclusa), I);
            CREDO_CHORDA_AEQUALIS_LITERIS(
                *(chorda*)xar_obtinere(caput->inclusa, ZEPHYRUM),
                "include/gamma.h");

            caput = _caput_invenire(fructus, "include/alpha.h");
            CREDO_NON_NIHIL(caput);
            CREDO_NIHIL(caput->inclusa);
        }

        /* ordo topologicus: Kahn gregatim, ordo inventionis intra
         * gyrum; beta (dependens gammae) post gyrum primum venit */
        {
                           Xar* ordinati;
            constans character* exspectati[VI];
                           i32  i;

            exspectati[ZEPHYRUM]  = "include/alpha.h";
            exspectati[I]         = "include/delta.h";
            exspectati[II]        = "vendor/librum.h";
            exspectati[III]       = "include/gamma.h";
            exspectati[IV]        = "include/epsilon.h";
            exspectati[V]         = "include/beta.h";

            causa.datum    = NIHIL;
            causa.mensura  = ZEPHYRUM;
            ordinati = aedilis_capita_ordinare(fructus, piscina,
                &causa);
            CREDO_NON_NIHIL(ordinati);
            CREDO_AEQUALIS_I32(xar_numerus(ordinati), VI);
            per (i = ZEPHYRUM; i < VI; i++)
            {
                AedilisCaput* caput;

                caput = *(AedilisCaput**)xar_obtinere(ordinati,
                    i);
                CREDO_CHORDA_AEQUALIS_LITERIS(caput->via,
                    exspectati[i]);
            }
        }

        /* varians praescriptus: posix -> beta_posix, non macos */
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", "posix", _extractor_fixturarum,
            NIHIL, &causa);
        CREDO_NON_NIHIL(fructus);
        CREDO_CHORDA_AEQUALIS_LITERIS(fructus->varians, "posix");
        CREDO_NON_NIHIL(_obiectum_invenire(fructus,
            "lib/beta_posix.c"));
        CREDO_NIHIL(_obiectum_invenire(fructus,
            "lib/beta_macos.m"));
    }


    /* ========================================================
     * PROBARE: cyclus inclusionum = recusatio ordinationis
     * ======================================================== */

    {
        AedilisConfiguratio* configuratio;
             AedilisFructus* fructus;
                        Xar* ordinati;
                     chorda  causa;

        imprimere("\n--- Probans cyclum inclusionum ---\n");

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        configuratio = aedilis_configurationem_legere(piscina,
            "probationes/fixa/aedilis/aedilis.stml", &causa);
        CREDO_NON_NIHIL(configuratio);

        /* derivatio ipsa bene - punctum fixum cyclos fert */
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/cyclicus.c", NIHIL, _extractor_fixturarum,
            NIHIL, &causa);
        CREDO_NON_NIHIL(fructus);
        CREDO_AEQUALIS_I32(xar_numerus(fructus->capita), II);

        /* ordinatio recusat clamose, cyclo nominato */
        ordinati = aedilis_capita_ordinare(fructus, piscina,
            &causa);
        CREDO_NIHIL(ordinati);
        CREDO_CHORDA_NON_VACUA(causa);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("cyclus", piscina));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("cyclus_a.h", piscina));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("cyclus_b.h", piscina));
    }


    /* ========================================================
     * PROBARE: recusationes (praecisio aut silentium)
     * ======================================================== */

    {
        AedilisConfiguratio* configuratio;
             AedilisFructus* fructus;
                     chorda  causa;

        imprimere("\n--- Probans recusationes ---\n");

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        configuratio = aedilis_configurationem_legere(piscina,
            "probationes/fixa/aedilis/aedilis.stml", &causa);
        CREDO_NON_NIHIL(configuratio);

        /* plagula imparsabilis (extractor FALSUM) */
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/malus.c", NIHIL, _extractor_fixturarum, NIHIL,
            &causa);
        CREDO_NIHIL(fructus);
        CREDO_CHORDA_NON_VACUA(causa);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("malus", piscina));

        /* annotatio ignota */
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/annotatio_mala.c", NIHIL,
            _extractor_fixturarum, NIHIL, &causa);
        CREDO_NIHIL(fructus);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("ignota", piscina));

        /* corpus absens = annotatio putrida, recusatio */
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/corpus_malus.c", NIHIL,
            _extractor_fixturarum, NIHIL, &causa);
        CREDO_NIHIL(fructus);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("corpus absens", piscina));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("nusquam_est", piscina));
    }


    /* ========================================================
     * PROBARE: scripta emissa (contentum; executio in porta)
     * ======================================================== */

    {
        AedilisConfiguratio* configuratio;
             AedilisFructus* fructus;
                     chorda  scriptum;
                     chorda  causa;

        imprimere("\n--- Probans scripta emissa ---\n");

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        configuratio = aedilis_configurationem_legere(piscina,
            "probationes/fixa/aedilis/aedilis.stml", &causa);
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", NIHIL, _extractor_fixturarum, NIHIL,
            &causa);
        CREDO_NON_NIHIL(fructus);

        /* postura cache-reutens */
        scriptum = aedilis_scriptum_scribere(fructus,
            configuratio, piscina, FALSUM, "abc1234");
        CREDO_CHORDA_NON_VACUA(scriptum);
        /* regula compilationis: vexilla addita ET nomen obiecti ea
         * ferens - obiectum commune (build/aedilis/obiecta) vexillis
         * aliis numquam per mtime pro recenti habetur */
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "compilare \"lib/alpha.c\" \"lib__alpha__O2.o\" \"-O2\"",
            piscina));
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "\"$OBIECTA_DIR/lib__alpha__O2.o\" \\", piscina));
        CREDO_FALSUM(chorda_continet(scriptum, chorda_ex_literis(
            "lib__alpha.o", piscina)));
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "$INCLUSA ${3:-} -c", piscina));
        /* fons sine regula: nomen et linea intacta */
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "compilare \"lib/alpha_socius.c\" \"lib__alpha_socius.o\"\n",
            piscina));
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "compilare \"lib/beta_macos.m\"", piscina));
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "-framework Cocoa", piscina));
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "vendor/librum.c", piscina));
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "-DLIBRUM=1", piscina));
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "gen/generatum.c", piscina));
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "-DPROBA", piscina));
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "build/aedilis/obiecta", piscina));
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "include/alpha.h", piscina));

        /* postura hermetica: obiecta propria, semper recompilat */
        scriptum = aedilis_scriptum_scribere(fructus,
            configuratio, piscina, VERUM, NIHIL);
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "/solitarius", piscina));
        CREDO_FALSUM(chorda_continet(scriptum, chorda_ex_literis(
            "build/aedilis/obiecta\"", piscina)));
    }


    /* ========================================================
     * PROBARE: manifestum scribere + relegere
     * ======================================================== */

    {
        AedilisConfiguratio* configuratio;
             AedilisFructus* fructus;
        InternamentumChorda* intern;
               StmlResultus  relectum;
                  StmlNodus* sectio;
                  StmlNodus* nodus;
                        Xar* obiecta;
                     chorda  manifestum;
                     chorda  causa;
                     chorda* valor;
                        i32  i;
                        i32  numerus;

        imprimere("\n--- Probans manifestum ---\n");

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        configuratio = aedilis_configurationem_legere(piscina,
            "probationes/fixa/aedilis/aedilis.stml", &causa);
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", NIHIL, _extractor_fixturarum, NIHIL,
            &causa);
        CREDO_NON_NIHIL(fructus);

        manifestum = aedilis_manifestum_scribere(fructus, piscina,
            "abc1234");
        CREDO_CHORDA_NON_VACUA(manifestum);

        intern    = internamentum_creare(piscina);
        relectum  = stml_legere(manifestum, piscina, intern);
        CREDO_VERUM(relectum.successus);
        CREDO_NON_NIHIL(relectum.elementum_radix);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *relectum.elementum_radix->titulus,
            "aedilis-manifestum");
        valor = stml_attributum_capere(relectum.elementum_radix,
            "scopus");
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "fons/scopus.c");
        valor = stml_attributum_capere(relectum.elementum_radix,
            "commissum");
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "abc1234");

        sectio = stml_invenire_liberum(relectum.elementum_radix,
            "obiecta");
        CREDO_NON_NIHIL(sectio);
        obiecta = stml_invenire_omnes_liberos(sectio, "obiectum",
            piscina);
        CREDO_AEQUALIS_I32(xar_numerus(obiecta), V);

        /* absens ut attributum boolean; vexillum crudum legibile */
        numerus = xar_numerus(obiecta);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            nodus = *(StmlNodus**)xar_obtinere(obiecta, i);
            valor = stml_attributum_capere(nodus, "via");
            si (valor == NIHIL)
            {
                perge;
            }
            si (chorda_aequalis_literis(*valor,
                    "gen/generatum.c"))
            {
                CREDO_VERUM(stml_attributum_habet(nodus,
                    "absens"));
                valor = stml_attributum_capere(nodus, "origo");
                CREDO_NON_NIHIL(valor);
                CREDO_CHORDA_AEQUALIS_LITERIS(*valor,
                    "annotatio");
            }
            alioquin si (chorda_aequalis_literis(*valor,
                         "lib/beta_macos.m"))
            {
                StmlNodus* vexillum;

                vexillum = stml_invenire_liberum(nodus,
                    "vexillum");
                CREDO_NON_NIHIL(vexillum);
                CREDO_CHORDA_AEQUALIS_LITERIS(
                    stml_textus_internus(vexillum, piscina),
                    "-framework Cocoa");
            }
            alioquin si (chorda_aequalis_literis(*valor,
                         "lib/alpha_socius.c"))
            {
                valor = stml_attributum_capere(nodus, "origo");
                CREDO_NON_NIHIL(valor);
                CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "corpus");
            }
        }

        /* aristae in manifesto: caput beta filium inclusio fert */
        sectio = stml_invenire_liberum(relectum.elementum_radix,
            "capita");
        CREDO_NON_NIHIL(sectio);
        {
            Xar* capita_nodi;
            b32  beta_inventum;

            beta_inventum = FALSUM;
            capita_nodi = stml_invenire_omnes_liberos(sectio,
                "caput", piscina);
            numerus = xar_numerus(capita_nodi);
            per (i = ZEPHYRUM; i < numerus; i++)
            {
                nodus = *(StmlNodus**)xar_obtinere(capita_nodi,
                    i);
                valor = stml_attributum_capere(nodus, "via");
                si (   valor == NIHIL
                    || !chorda_aequalis_literis(*valor,
                        "include/beta.h"))
                {
                    perge;
                }
                beta_inventum = VERUM;
                {
                    StmlNodus* arista;

                    arista = stml_invenire_liberum(nodus,
                        "inclusio");
                    CREDO_NON_NIHIL(arista);
                    valor = stml_attributum_capere(arista, "via");
                    CREDO_NON_NIHIL(valor);
                    CREDO_CHORDA_AEQUALIS_LITERIS(*valor,
                        "include/gamma.h");
                }
            }
            CREDO_VERUM(beta_inventum);
        }
    }


    /* ==================================================
     * PROBARE: inclusiones citatae inresolutae (fabrica P1).
     * Caput citatum nusquam inventum NON est systema verum -
     * manifestum sine eo tacite incompletum est. Sectio
     * inresolutarum id nominat; systemata et --partes ut olim
     * manent (consumptores XII+ ea legunt).
     * ================================================== */

    {
        AedilisConfiguratio* configuratio;
             AedilisFructus* fructus;
                     chorda  causa;
                     chorda  manifestum;

        imprimere("\n--- Probans citatas inresolutas ---\n");

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        configuratio = aedilis_configurationem_legere(piscina,
            "probationes/fixa/aedilis/aedilis.stml", &causa);
        CREDO_NON_NIHIL(configuratio);

        fructus = aedilis_derivare(piscina, configuratio,
            "fons/citata.c", NIHIL, _extractor_fixturarum, NIHIL,
            &causa);
        CREDO_NON_NIHIL(fructus);
        CREDO_NON_NIHIL(fructus->inresolutae);
        CREDO_AEQUALIS_I32(xar_numerus(fructus->inresolutae), I);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *(chorda*)xar_obtinere(fructus->inresolutae, ZEPHYRUM),
            "nusquam.h");
        /* systemata ut olim: ambae inresolutae ibi */
        CREDO_AEQUALIS_I32(xar_numerus(fructus->systemata), II);

        manifestum = aedilis_manifestum_scribere(fructus, piscina,
            "abc1234");
        CREDO_CHORDA_CONTINET(manifestum,
            chorda_ex_literis("inresolutae", piscina));
        CREDO_CHORDA_CONTINET(manifestum,
            chorda_ex_literis("nusquam.h", piscina));

        /* forma ignota (scopus.c): regula vetus, manifestum SINE
         * sectione - manifesta radicum sanarum octetim eadem */
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", NIHIL, _extractor_fixturarum, NIHIL,
            &causa);
        CREDO_NON_NIHIL(fructus);
        CREDO_AEQUALIS_I32(xar_numerus(fructus->inresolutae),
            ZEPHYRUM);
        manifestum = aedilis_manifestum_scribere(fructus, piscina,
            "abc1234");
        CREDO_FALSUM(chorda_continet(manifestum,
            chorda_ex_literis("inresolutae", piscina)));
    }


    /* ========================================================
     * PROBARE: nexus purus (eventus A1b) - <aedilis nexus="purus"/>
     * promittit clausuram SINE regula nexus (frameworks)
     * ======================================================== */

    {
        AedilisConfiguratio* configuratio;
             AedilisFructus* fructus;
                     chorda  causa;

        imprimere("\n--- Probans nexum purum ---\n");

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        configuratio = aedilis_configurationem_legere(piscina,
            "probationes/fixa/aedilis/aedilis.stml", &causa);
        CREDO_NON_NIHIL(configuratio);

        /* purum: promissum servatum */
        CREDO_VERUM(aedilis_nexum_purum_probare(piscina, configuratio,
            "include/purum.h", _extractor_fixturarum, NIHIL, &causa));

        /* impurum: obiectum, vexillum, catena NOMINANTUR */
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_FALSUM(aedilis_nexum_purum_probare(piscina, configuratio,
            "include/impurum.h", _extractor_fixturarum, NIHIL,
            &causa));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("lib/beta_macos.m", piscina));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("-framework Cocoa", piscina));
        CREDO_CHORDA_CONTINET(causa, chorda_ex_literis(
            "include/impurum.h -> include/beta.h -> lib/beta_macos.m",
            piscina));

        /* cyclus in catena (b <-> c): catena semel, non circuit */
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_FALSUM(aedilis_nexum_purum_probare(piscina, configuratio,
            "include/orbis_a.h", _extractor_fixturarum, NIHIL, &causa));
        CREDO_CHORDA_CONTINET(causa, chorda_ex_literis(
            "include/orbis_a.h -> include/orbis_b.h -> include/orbis_c.h"
            " -> include/beta.h -> lib/beta_macos.m", piscina));
        CREDO_FALSUM(chorda_continet(causa, chorda_ex_literis(
            "orbis_c.h -> include/orbis_b.h", piscina)));

        /* sine annotatione: nihil promissum -> VERUM. scopus.c
         * beta.h includit, ergo lib/beta_macos.m (Cocoa) VERE in
         * clausura est - sed nihil promisit. (Olim beta.h ipsum ut
         * scopus: planta 'promissum neglectum' MUTA erat - caput ut
         * scopus implementationem suam non trahit.) */
        CREDO_VERUM(aedilis_nexum_purum_probare(piscina, configuratio,
            "fons/scopus.c", _extractor_fixturarum, NIHIL, &causa));

        /* caput promittens cuius implementatio PROPRIA nexum fert:
         * fractum (lacuna inventa per plantam mutam) */
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_FALSUM(aedilis_nexum_purum_probare(piscina, configuratio,
            "include/proprium.h", _extractor_fixturarum, NIHIL,
            &causa));
        CREDO_CHORDA_CONTINET(causa, chorda_ex_literis(
            "include/proprium.h -> lib/proprium_macos.m [-framework"
            " Cocoa]", piscina));

        /* verbum nexus agnoscitur in derivatione (scopus includens
         * caput purum non recusatur); purum in fructu notatur */
        fructus = aedilis_derivare(piscina, configuratio,
            "include/purum.h", NIHIL, _extractor_fixturarum, NIHIL,
            &causa);
        CREDO_NON_NIHIL(fructus);
        si (fructus != NIHIL)
        {
            CREDO_AEQUALIS_I32(xar_numerus(fructus->pura), I);
        }

        /* valor ignotus: recusatio clamosa */
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        fructus = aedilis_derivare(piscina, configuratio,
            "include/nexus_ignotum.h", NIHIL, _extractor_fixturarum,
            NIHIL, &causa);
        CREDO_NIHIL(fructus);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("nexus", piscina));
    }


    /* ==================================================
     * PROBARE: facultas (fabrica-6 T6) - <aedilis facultas="X"/> in
     * SCOPO solo: fenestra | rete | repositorium; in capite aut valor
     * ignotus -> recusatio nominata; sine annotatione vacua
     * ================================================== */

    {
        AedilisConfiguratio* configuratio;
             AedilisFructus* fructus;
                     chorda  causa;

        imprimere("\n--- Probans facultatem ---\n");
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        configuratio = aedilis_configurationem_legere(piscina,
            "probationes/fixa/aedilis/aedilis.stml", &causa);
        CREDO_NON_NIHIL(configuratio);
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/fenestralis.c", NIHIL, _extractor_fixturarum, NIHIL,
            &causa);
        CREDO_NON_NIHIL(fructus);
        si (fructus != NIHIL)
        {
            CREDO_AEQUALIS_I32(xar_numerus(fructus->facultates), I);
            si (xar_numerus(fructus->facultates) == I)
            {
                CREDO_VERUM(chorda_aequalis_literis(
                    *(chorda*)xar_obtinere(fructus->facultates,
                    ZEPHYRUM), "fenestra"));
            }
        }
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", NIHIL, _extractor_fixturarum, NIHIL,
            &causa);
        CREDO_NON_NIHIL(fructus);
        si (fructus != NIHIL)
        {
            CREDO_AEQUALIS_I32(xar_numerus(fructus->facultates),
                ZEPHYRUM);
        }
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/facultas_ignota.c", NIHIL, _extractor_fixturarum,
            NIHIL, &causa);
        CREDO_NIHIL(fructus);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("facultas ignota", piscina));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("tactus", piscina));
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/includens_facultatem.c", NIHIL, _extractor_fixturarum,
            NIHIL, &causa);
        CREDO_NIHIL(fructus);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("facultas extra scopum", piscina));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("facultas_caput.h", piscina));
    }


    /* ==================================================
     * PROBARE: instrumentum (fabrica-7 T2) - <aedilis
     * instrumentum="bin/X"/> in SCOPO solo, via sub bin/ (binarium
     * domus quod probatio currit; clavis membri identitatem eius
     * sequitur); extra bin/, in capite aut duplex -> recusatio
     * nominata; sine annotatione vacua
     * ================================================== */

    {
        AedilisConfiguratio* configuratio;
             AedilisFructus* fructus;
                     chorda  causa;

        imprimere("\n--- Probans instrumentum ---\n");
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        configuratio = aedilis_configurationem_legere(piscina,
            "probationes/fixa/aedilis/aedilis.stml", &causa);
        CREDO_NON_NIHIL(configuratio);
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/instrumentalis.c", NIHIL, _extractor_fixturarum,
            NIHIL,
            &causa);
        CREDO_NON_NIHIL(fructus);
        si (fructus != NIHIL)
        {
            CREDO_NON_NIHIL(fructus->instrumenta);
        }
        si (fructus != NIHIL && fructus->instrumenta != NIHIL)
        {
            CREDO_AEQUALIS_I32(xar_numerus(fructus->instrumenta), II);
            si (xar_numerus(fructus->instrumenta) == II)
            {
                CREDO_VERUM(chorda_aequalis_literis(
                    *(chorda*)xar_obtinere(fructus->instrumenta,
                    ZEPHYRUM), "bin/natura"));
                CREDO_VERUM(chorda_aequalis_literis(
                    *(chorda*)xar_obtinere(fructus->instrumenta, I),
                    "bin/generare"));
            }
            CREDO_AEQUALIS_I32(xar_numerus(fructus->facultates),
                ZEPHYRUM);
        }
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", NIHIL, _extractor_fixturarum, NIHIL,
            &causa);
        CREDO_NON_NIHIL(fructus);
        si (fructus != NIHIL && fructus->instrumenta != NIHIL)
        {
            CREDO_AEQUALIS_I32(xar_numerus(fructus->instrumenta),
                ZEPHYRUM);
        }
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/instrumentum_extra.c", NIHIL, _extractor_fixturarum,
            NIHIL, &causa);
        CREDO_NIHIL(fructus);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("instrumentum extra bin/", piscina));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("tools/natura.sh", piscina));
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/instrumentum_duplex.c", NIHIL, _extractor_fixturarum,
            NIHIL, &causa);
        CREDO_NIHIL(fructus);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("instrumentum duplex", piscina));
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/includens_instrumentum.c", NIHIL,
            _extractor_fixturarum, NIHIL, &causa);
        CREDO_NIHIL(fructus);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("instrumentum extra scopum", piscina));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("instrumentum_caput.h", piscina));
    }


    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
