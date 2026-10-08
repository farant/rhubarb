/* clonatio.h - clonatio copy-on-write plagulae aut arboris directorii
 * totius (pythonica S3; profilum T9: photographiae 'cp -c -R' plagulam
 * singulam clonat, ~XIII s per photographiam).
 *
 * macOS: clonefile(2) - systema fasciculorum APFS, eadem area; arbor
 * tota uno vocamine in nucleo, spatium nullum donec scribatur.
 * Implementatio: lib/clonatio_macos.c (variantes aedilis.stml).
 */

#ifndef CLONATIO_H
#define CLONATIO_H

#include "latina.h"

/* fontem (plagulam aut directorium cum arbore tota) in destinationem
 * clonare. destinatio NON exstare debet; eadem area (aliter EXDEV).
 * FALSUM: errno ut systema posuit (EEXIST, ENOENT, EXDEV, ENOTSUP). */
b32
clonatio_facere (
    constans character* fons,
    constans character* destinatio);

#endif /* CLONATIO_H */
