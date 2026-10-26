#ifndef POSTULATA_IOS_H
#define POSTULATA_IOS_H

/* postulata_ios.h - in omni .m iOS PRIMUM, ante capita UIKit (ut
 * postulata_android.h ante capita NDK). UIKit 'static inline' C99
 * fert (UIKIT_STATIC_INLINE, UIGeometry.h, UIOrientation.h); sub
 * -std=c89 'inline' verbum ignotum est - error in capite systematis.
 * '__inline__' (GNU) idem significat; 'static' iam in capite stat,
 * ergo hic non additur (contra Android, ubi 'inline' nudum est).
 * testa-ios T3, 2026-10-23. */

#define inline __inline__

#endif /* POSTULATA_IOS_H */
