/* bibliothecae.h - quae plagulae .c interpreti onerandae sint, ex
 * lineis #include codicis ipsius (officina-bibliothecae-spec.md).
 *
 * #include = declaratio: clausura per aedilem (veritas
 * aedificationis domus), numquam index manu factus. Textus in
 * scopum temporarium scribitur (build/bibliothecae_scopi/; via
 * numquam iterata, quia extractor per viam meminit; deletur in omni
 * reditu) et bis derivatur: varians "posix" = quod oneratur;
 * praelatio configurationis = detectio capitum solum macOS.
 *
 * RECUSATIO CLAMOSA (NIHIL + causa, nullus fructus partialis):
 *   - caput cuius implementatio sola .m est (fons et caput nominati)
 *   - caput nusquam inventum
 *   - fons vendoris; obiectum absens
 *   - fons supra BIBLIOTHECAE_MENSURA_MAXIMA (e.g. amalgama silvae)
 * Capita systematis (<stdio.h>) nihil onerant: machinula libc
 * praebet. */

#ifndef BIBLIOTHECAE_H
#define BIBLIOTHECAE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

#define BIBLIOTHECAE_MENSURA_MAXIMA 4194304   /* = interpretare */

nomen structura Bibliothecae Bibliothecae;

/* radix = radix repositorii (aedilis.stml ibi legitur). NIHIL +
 * causa si configuratio aut extractor parari nequit. */
Bibliothecae*
bibliothecae_creare (
               Piscina* piscina,
    constans character* radix,
                chorda* causa_out);

/* textus = C cuius lineae #include bibliothecas declarant (turnus,
 * directivae scripti, "#include \"x.h\"\n" synthetica). Reddit Xar
 * de chorda: viae radici relativae ("lib/chorda.c"), ordine
 * derivationis, sine duplicatis; vacuum si nihil onerandum. */
Xar*
bibliothecae_ex_textu (
     Bibliothecae* b,
           chorda  textus,
          Piscina* piscina,
           chorda* causa_out);

/* via = fons .c radici relativus; fructus ipsam viam PRIMAM
 * continet, deinde clausuram eius. */
Xar*
bibliothecae_ex_via (
          Bibliothecae* b,
    constans character* via,
               Piscina* piscina,
                chorda* causa_out);

/* Mundus totus (cursor; interpretare cum functio in indice ignota):
 * pro omni capite include/ *.h, clausura variantis "posix" per
 * bibliothecae_ex_textu - una implementatio per caput, ergo
 * testa_android.c et *_macos.c numquam intrant. Capita recusata
 * (implementatio sola .m, fons nimis magnus, caput ignotum) totum
 * NON frangunt: in *omissa_out colliguntur, chorda
 * "include/x.h: causa" singula. Capita sine implementatione
 * (latina.h) nihil addunt nec omittuntur. Fructus: Xar de chorda,
 * viae .c radici relativae, sine duplicatis, ordine alphabetico
 * (determinismus). NIHIL + causa solum si include/ legi nequit. */
Xar*
bibliothecae_mundum_totum (
     Bibliothecae*  b,
          Piscina*  piscina,
              Xar** omissa_out,
           chorda*  causa_out);

nomen enumeratio {
    BIBLIOTHECAE_SEDES_INVENTA = 0,
    BIBLIOTHECAE_SEDES_IGNOTA,     /* index absens aut functio
                                    * non in eo */
    BIBLIOTHECAE_SEDES_AMBIGUA
} BibliothecaeSedes;

/* Domus functionis ex indice symbolorum (forma build/nexus.tsv:
 * titulus \t sedes \t functio \t via ...). Solae sedes sub include/
 * et lib/; include/x.h et lib/x.c = familia una (caput praefertur
 * in via_out). AMBIGUA: causa candidatos nominat. */
BibliothecaeSedes
bibliothecae_sedem_quaerere (
    constans character* via_indicis,
    constans character* functio,
               Piscina* piscina,
                chorda* via_out,
                chorda* causa_out);

#endif
