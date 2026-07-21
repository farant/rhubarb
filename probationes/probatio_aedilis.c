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
#include "stml.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    b32      praeteritus;
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
        StmlNodus*           obiectum;
        StmlResultus         relectum;
        chorda               scriptum;
        chorda*              valor;

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
        StmlNodus*           nodus;
        StmlResultus         relectum;
        chorda               scriptum;
        chorda*              valor;

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
        StmlNodus*           radix;
        StmlNodus*           obiecta;
        StmlNodus*           obiectum;
        StmlNodus*           inventum;
        StmlResultus         relectum;
        chorda               scriptum;
        chorda*              valor;
        Xar*                 filii;

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

        obiectum = *(StmlNodus**)xar_obtinere(filii, I);
        valor = stml_attributum_capere(obiectum, "origo");
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "aedilis.stml");
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
