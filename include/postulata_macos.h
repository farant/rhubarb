#ifndef POSTULATA_MACOS_H
#define POSTULATA_MACOS_H

/* postulata_macos.h - in omni .m PRIMUM, ante capita Apple (Cocoa,
 * Foundation, Security, ...). SDK macOS 27 (clang 21) 'inline' nudum
 * fert (xpc/base.h: XPC_INLINE = static inline ...); sub -std=c89
 * 'inline' verbum non est. '__inline__' (GNU) idem sub C89 significat.
 * Non 'static __inline__' ut postulata_android.h: SDK 'static' ipsum
 * scribit (static static vitium esset). */

#define inline __inline__

#endif /* POSTULATA_MACOS_H */
