/* clonatio_posix.c - clonatio extra macOS (vide clonatio.h)
 *
 * clonefile(2) proprium macOS (APFS) est. Linux FICLONE (btrfs, xfs)
 * plagulas singulas solum clonat, non arbores - contractus capitis
 * (arbor tota uno vocamine) hic servari nequit. Ergo FALSUM cum errno
 * ENOTSUP, quod contractus iam nominat; vocator (tools/clonare.c)
 * errorem nuntiat. */

#include "postulata_posix.h"
#include "clonatio.h"
#include <errno.h>

b32
clonatio_facere (
    constans character* fons,
    constans character* destinatio)
{
    (vacuum)fons;
    (vacuum)destinatio;
    errno = ENOTSUP;
    redde FALSUM;
}
