/* demo-snapshot.c - GENERATUM (knotapel/archive.sh) - DO NOT EDIT
 *
 * knotapel/demo_122_cell_b_phase_diagram/main.c frozen with its house-library closure as ONE file:
 * headers in dependency order, library sources with file-local
 * names renamed per file (#define/#undef), main.c last; '#line'
 * names each original file. Compile and run:
 *
 *   clang -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -fbracket-depth=512 -O2 -g demo-snapshot.c -o demo-snapshot
 *
 * Commit (library closure clean): ab3c9aad7a04744b160745e8b6df51dbb350e6f4
 * Regenerate: ./knotapel/archive.sh knotapel/demo_122_cell_b_phase_diagram/main.c
 * Verified: live build and snapshot gave byte-identical output.
 * Sources (git blob hashes):
 *   f45b10ad9c303c02d43655b950600fcdab8997bb  include/latina.h
 *   5921a2ecc9323870519bcb945788366a9291753f  include/surdus.h
 *   5d43ea07a96abb236f9f77b4430185a4e09d2536  include/surdus_interna.h
 *   6e3945b8a0ce6a556478fbe4249ff3c6fb5cdbe4  lib/surdus.c
 *   2c31a73a9849a5436a3922a958a0f9d10d2542d3  knotapel/demo_122_cell_b_phase_diagram/main.c (uncommitted, embedded verbatim)
 */

#line 1 "include/latina.h"
/* latina.h - Verba clavis C89 Latine reddita (lexicon domus) */
#ifndef LATINA_H
#define LATINA_H

#include <stddef.h>

#define character     char
#define brevis             short
#define integer         int
#define longus            long
#define fluitans        float
#define duplex            double

#define vacuum            void
#define signatus         signed
#define insignatus  unsigned
#define constans        const
#define volatilis        volatile
#define sponte            auto
#define registrum     register
#define staticus         static
#define    externus         extern

#define si                    if
#define alioquin        else
#define commutatio    switch
#define casus                case
#define ordinarius    default
#define per                    for
#define dum                 while
#define fac                 do
#define frange             break
#define perge             continue
#define salta                goto
#define redde                return

#define structura        struct
#define unio                 union
#define enumeratio     enum
#define nomen             typedef

#define magnitudo     sizeof

#define principale     main

#define NIHIL                NULL
#define VERUM             1
#define FALSUM             0

/* NUMERI ROMANI - GENERATUM ex numerus_romanus_scribere
 * (tools/latina_numeri.sh -scribere) - NE MANU MUTES; porta
 * 'generata' iudicat. Omnes ZEPHYRUM-MMMCMXCIX ordine: numerus
 * Romanus classicus ad MMMCMXCIX finit - maiores ut expressio
 * (vinculum ut '* M'): IV * M (4000), IV * MXXIV (4096); vide
 * numerus_romanus_exprimere. Omne numerale identificator
 * reservatus est: capita systematis ANTE latina.h includenda
 * (dns_util.h membra 'MD' et 'MX' habet). */
#define ZEPHYRUM         0
#define I                1
#define II               2
#define III              3
#define IV               4
#define V                5
#define VI               6
#define VII              7
#define VIII             8
#define IX               9
#define X                10
#define XI               11
#define XII              12
#define XIII             13
#define XIV              14
#define XV               15
#define XVI              16
#define XVII             17
#define XVIII            18
#define XIX              19
#define XX               20
#define XXI              21
#define XXII             22
#define XXIII            23
#define XXIV             24
#define XXV              25
#define XXVI             26
#define XXVII            27
#define XXVIII           28
#define XXIX             29
#define XXX              30
#define XXXI             31
#define XXXII            32
#define XXXIII           33
#define XXXIV            34
#define XXXV             35
#define XXXVI            36
#define XXXVII           37
#define XXXVIII          38
#define XXXIX            39
#define XL               40
#define XLI              41
#define XLII             42
#define XLIII            43
#define XLIV             44
#define XLV              45
#define XLVI             46
#define XLVII            47
#define XLVIII           48
#define XLIX             49
#define L                50
#define LI               51
#define LII              52
#define LIII             53
#define LIV              54
#define LV               55
#define LVI              56
#define LVII             57
#define LVIII            58
#define LIX              59
#define LX               60
#define LXI              61
#define LXII             62
#define LXIII            63
#define LXIV             64
#define LXV              65
#define LXVI             66
#define LXVII            67
#define LXVIII           68
#define LXIX             69
#define LXX              70
#define LXXI             71
#define LXXII            72
#define LXXIII           73
#define LXXIV            74
#define LXXV             75
#define LXXVI            76
#define LXXVII           77
#define LXXVIII          78
#define LXXIX            79
#define LXXX             80
#define LXXXI            81
#define LXXXII           82
#define LXXXIII          83
#define LXXXIV           84
#define LXXXV            85
#define LXXXVI           86
#define LXXXVII          87
#define LXXXVIII         88
#define LXXXIX           89
#define XC               90
#define XCI              91
#define XCII             92
#define XCIII            93
#define XCIV             94
#define XCV              95
#define XCVI             96
#define XCVII            97
#define XCVIII           98
#define XCIX             99
#define C                100
#define CI               101
#define CII              102
#define CIII             103
#define CIV              104
#define CV               105
#define CVI              106
#define CVII             107
#define CVIII            108
#define CIX              109
#define CX               110
#define CXI              111
#define CXII             112
#define CXIII            113
#define CXIV             114
#define CXV              115
#define CXVI             116
#define CXVII            117
#define CXVIII           118
#define CXIX             119
#define CXX              120
#define CXXI             121
#define CXXII            122
#define CXXIII           123
#define CXXIV            124
#define CXXV             125
#define CXXVI            126
#define CXXVII           127
#define CXXVIII          128
#define CXXIX            129
#define CXXX             130
#define CXXXI            131
#define CXXXII           132
#define CXXXIII          133
#define CXXXIV           134
#define CXXXV            135
#define CXXXVI           136
#define CXXXVII          137
#define CXXXVIII         138
#define CXXXIX           139
#define CXL              140
#define CXLI             141
#define CXLII            142
#define CXLIII           143
#define CXLIV            144
#define CXLV             145
#define CXLVI            146
#define CXLVII           147
#define CXLVIII          148
#define CXLIX            149
#define CL               150
#define CLI              151
#define CLII             152
#define CLIII            153
#define CLIV             154
#define CLV              155
#define CLVI             156
#define CLVII            157
#define CLVIII           158
#define CLIX             159
#define CLX              160
#define CLXI             161
#define CLXII            162
#define CLXIII           163
#define CLXIV            164
#define CLXV             165
#define CLXVI            166
#define CLXVII           167
#define CLXVIII          168
#define CLXIX            169
#define CLXX             170
#define CLXXI            171
#define CLXXII           172
#define CLXXIII          173
#define CLXXIV           174
#define CLXXV            175
#define CLXXVI           176
#define CLXXVII          177
#define CLXXVIII         178
#define CLXXIX           179
#define CLXXX            180
#define CLXXXI           181
#define CLXXXII          182
#define CLXXXIII         183
#define CLXXXIV          184
#define CLXXXV           185
#define CLXXXVI          186
#define CLXXXVII         187
#define CLXXXVIII        188
#define CLXXXIX          189
#define CXC              190
#define CXCI             191
#define CXCII            192
#define CXCIII           193
#define CXCIV            194
#define CXCV             195
#define CXCVI            196
#define CXCVII           197
#define CXCVIII          198
#define CXCIX            199
#define CC               200
#define CCI              201
#define CCII             202
#define CCIII            203
#define CCIV             204
#define CCV              205
#define CCVI             206
#define CCVII            207
#define CCVIII           208
#define CCIX             209
#define CCX              210
#define CCXI             211
#define CCXII            212
#define CCXIII           213
#define CCXIV            214
#define CCXV             215
#define CCXVI            216
#define CCXVII           217
#define CCXVIII          218
#define CCXIX            219
#define CCXX             220
#define CCXXI            221
#define CCXXII           222
#define CCXXIII          223
#define CCXXIV           224
#define CCXXV            225
#define CCXXVI           226
#define CCXXVII          227
#define CCXXVIII         228
#define CCXXIX           229
#define CCXXX            230
#define CCXXXI           231
#define CCXXXII          232
#define CCXXXIII         233
#define CCXXXIV          234
#define CCXXXV           235
#define CCXXXVI          236
#define CCXXXVII         237
#define CCXXXVIII        238
#define CCXXXIX          239
#define CCXL             240
#define CCXLI            241
#define CCXLII           242
#define CCXLIII          243
#define CCXLIV           244
#define CCXLV            245
#define CCXLVI           246
#define CCXLVII          247
#define CCXLVIII         248
#define CCXLIX           249
#define CCL              250
#define CCLI             251
#define CCLII            252
#define CCLIII           253
#define CCLIV            254
#define CCLV             255
#define CCLVI            256
#define CCLVII           257
#define CCLVIII          258
#define CCLIX            259
#define CCLX             260
#define CCLXI            261
#define CCLXII           262
#define CCLXIII          263
#define CCLXIV           264
#define CCLXV            265
#define CCLXVI           266
#define CCLXVII          267
#define CCLXVIII         268
#define CCLXIX           269
#define CCLXX            270
#define CCLXXI           271
#define CCLXXII          272
#define CCLXXIII         273
#define CCLXXIV          274
#define CCLXXV           275
#define CCLXXVI          276
#define CCLXXVII         277
#define CCLXXVIII        278
#define CCLXXIX          279
#define CCLXXX           280
#define CCLXXXI          281
#define CCLXXXII         282
#define CCLXXXIII        283
#define CCLXXXIV         284
#define CCLXXXV          285
#define CCLXXXVI         286
#define CCLXXXVII        287
#define CCLXXXVIII       288
#define CCLXXXIX         289
#define CCXC             290
#define CCXCI            291
#define CCXCII           292
#define CCXCIII          293
#define CCXCIV           294
#define CCXCV            295
#define CCXCVI           296
#define CCXCVII          297
#define CCXCVIII         298
#define CCXCIX           299
#define CCC              300
#define CCCI             301
#define CCCII            302
#define CCCIII           303
#define CCCIV            304
#define CCCV             305
#define CCCVI            306
#define CCCVII           307
#define CCCVIII          308
#define CCCIX            309
#define CCCX             310
#define CCCXI            311
#define CCCXII           312
#define CCCXIII          313
#define CCCXIV           314
#define CCCXV            315
#define CCCXVI           316
#define CCCXVII          317
#define CCCXVIII         318
#define CCCXIX           319
#define CCCXX            320
#define CCCXXI           321
#define CCCXXII          322
#define CCCXXIII         323
#define CCCXXIV          324
#define CCCXXV           325
#define CCCXXVI          326
#define CCCXXVII         327
#define CCCXXVIII        328
#define CCCXXIX          329
#define CCCXXX           330
#define CCCXXXI          331
#define CCCXXXII         332
#define CCCXXXIII        333
#define CCCXXXIV         334
#define CCCXXXV          335
#define CCCXXXVI         336
#define CCCXXXVII        337
#define CCCXXXVIII       338
#define CCCXXXIX         339
#define CCCXL            340
#define CCCXLI           341
#define CCCXLII          342
#define CCCXLIII         343
#define CCCXLIV          344
#define CCCXLV           345
#define CCCXLVI          346
#define CCCXLVII         347
#define CCCXLVIII        348
#define CCCXLIX          349
#define CCCL             350
#define CCCLI            351
#define CCCLII           352
#define CCCLIII          353
#define CCCLIV           354
#define CCCLV            355
#define CCCLVI           356
#define CCCLVII          357
#define CCCLVIII         358
#define CCCLIX           359
#define CCCLX            360
#define CCCLXI           361
#define CCCLXII          362
#define CCCLXIII         363
#define CCCLXIV          364
#define CCCLXV           365
#define CCCLXVI          366
#define CCCLXVII         367
#define CCCLXVIII        368
#define CCCLXIX          369
#define CCCLXX           370
#define CCCLXXI          371
#define CCCLXXII         372
#define CCCLXXIII        373
#define CCCLXXIV         374
#define CCCLXXV          375
#define CCCLXXVI         376
#define CCCLXXVII        377
#define CCCLXXVIII       378
#define CCCLXXIX         379
#define CCCLXXX          380
#define CCCLXXXI         381
#define CCCLXXXII        382
#define CCCLXXXIII       383
#define CCCLXXXIV        384
#define CCCLXXXV         385
#define CCCLXXXVI        386
#define CCCLXXXVII       387
#define CCCLXXXVIII      388
#define CCCLXXXIX        389
#define CCCXC            390
#define CCCXCI           391
#define CCCXCII          392
#define CCCXCIII         393
#define CCCXCIV          394
#define CCCXCV           395
#define CCCXCVI          396
#define CCCXCVII         397
#define CCCXCVIII        398
#define CCCXCIX          399
#define CD               400
#define CDI              401
#define CDII             402
#define CDIII            403
#define CDIV             404
#define CDV              405
#define CDVI             406
#define CDVII            407
#define CDVIII           408
#define CDIX             409
#define CDX              410
#define CDXI             411
#define CDXII            412
#define CDXIII           413
#define CDXIV            414
#define CDXV             415
#define CDXVI            416
#define CDXVII           417
#define CDXVIII          418
#define CDXIX            419
#define CDXX             420
#define CDXXI            421
#define CDXXII           422
#define CDXXIII          423
#define CDXXIV           424
#define CDXXV            425
#define CDXXVI           426
#define CDXXVII          427
#define CDXXVIII         428
#define CDXXIX           429
#define CDXXX            430
#define CDXXXI           431
#define CDXXXII          432
#define CDXXXIII         433
#define CDXXXIV          434
#define CDXXXV           435
#define CDXXXVI          436
#define CDXXXVII         437
#define CDXXXVIII        438
#define CDXXXIX          439
#define CDXL             440
#define CDXLI            441
#define CDXLII           442
#define CDXLIII          443
#define CDXLIV           444
#define CDXLV            445
#define CDXLVI           446
#define CDXLVII          447
#define CDXLVIII         448
#define CDXLIX           449
#define CDL              450
#define CDLI             451
#define CDLII            452
#define CDLIII           453
#define CDLIV            454
#define CDLV             455
#define CDLVI            456
#define CDLVII           457
#define CDLVIII          458
#define CDLIX            459
#define CDLX             460
#define CDLXI            461
#define CDLXII           462
#define CDLXIII          463
#define CDLXIV           464
#define CDLXV            465
#define CDLXVI           466
#define CDLXVII          467
#define CDLXVIII         468
#define CDLXIX           469
#define CDLXX            470
#define CDLXXI           471
#define CDLXXII          472
#define CDLXXIII         473
#define CDLXXIV          474
#define CDLXXV           475
#define CDLXXVI          476
#define CDLXXVII         477
#define CDLXXVIII        478
#define CDLXXIX          479
#define CDLXXX           480
#define CDLXXXI          481
#define CDLXXXII         482
#define CDLXXXIII        483
#define CDLXXXIV         484
#define CDLXXXV          485
#define CDLXXXVI         486
#define CDLXXXVII        487
#define CDLXXXVIII       488
#define CDLXXXIX         489
#define CDXC             490
#define CDXCI            491
#define CDXCII           492
#define CDXCIII          493
#define CDXCIV           494
#define CDXCV            495
#define CDXCVI           496
#define CDXCVII          497
#define CDXCVIII         498
#define CDXCIX           499
#define D                500
#define DI               501
#define DII              502
#define DIII             503
#define DIV              504
#define DV               505
#define DVI              506
#define DVII             507
#define DVIII            508
#define DIX              509
#define DX               510
#define DXI              511
#define DXII             512
#define DXIII            513
#define DXIV             514
#define DXV              515
#define DXVI             516
#define DXVII            517
#define DXVIII           518
#define DXIX             519
#define DXX              520
#define DXXI             521
#define DXXII            522
#define DXXIII           523
#define DXXIV            524
#define DXXV             525
#define DXXVI            526
#define DXXVII           527
#define DXXVIII          528
#define DXXIX            529
#define DXXX             530
#define DXXXI            531
#define DXXXII           532
#define DXXXIII          533
#define DXXXIV           534
#define DXXXV            535
#define DXXXVI           536
#define DXXXVII          537
#define DXXXVIII         538
#define DXXXIX           539
#define DXL              540
#define DXLI             541
#define DXLII            542
#define DXLIII           543
#define DXLIV            544
#define DXLV             545
#define DXLVI            546
#define DXLVII           547
#define DXLVIII          548
#define DXLIX            549
#define DL               550
#define DLI              551
#define DLII             552
#define DLIII            553
#define DLIV             554
#define DLV              555
#define DLVI             556
#define DLVII            557
#define DLVIII           558
#define DLIX             559
#define DLX              560
#define DLXI             561
#define DLXII            562
#define DLXIII           563
#define DLXIV            564
#define DLXV             565
#define DLXVI            566
#define DLXVII           567
#define DLXVIII          568
#define DLXIX            569
#define DLXX             570
#define DLXXI            571
#define DLXXII           572
#define DLXXIII          573
#define DLXXIV           574
#define DLXXV            575
#define DLXXVI           576
#define DLXXVII          577
#define DLXXVIII         578
#define DLXXIX           579
#define DLXXX            580
#define DLXXXI           581
#define DLXXXII          582
#define DLXXXIII         583
#define DLXXXIV          584
#define DLXXXV           585
#define DLXXXVI          586
#define DLXXXVII         587
#define DLXXXVIII        588
#define DLXXXIX          589
#define DXC              590
#define DXCI             591
#define DXCII            592
#define DXCIII           593
#define DXCIV            594
#define DXCV             595
#define DXCVI            596
#define DXCVII           597
#define DXCVIII          598
#define DXCIX            599
#define DC               600
#define DCI              601
#define DCII             602
#define DCIII            603
#define DCIV             604
#define DCV              605
#define DCVI             606
#define DCVII            607
#define DCVIII           608
#define DCIX             609
#define DCX              610
#define DCXI             611
#define DCXII            612
#define DCXIII           613
#define DCXIV            614
#define DCXV             615
#define DCXVI            616
#define DCXVII           617
#define DCXVIII          618
#define DCXIX            619
#define DCXX             620
#define DCXXI            621
#define DCXXII           622
#define DCXXIII          623
#define DCXXIV           624
#define DCXXV            625
#define DCXXVI           626
#define DCXXVII          627
#define DCXXVIII         628
#define DCXXIX           629
#define DCXXX            630
#define DCXXXI           631
#define DCXXXII          632
#define DCXXXIII         633
#define DCXXXIV          634
#define DCXXXV           635
#define DCXXXVI          636
#define DCXXXVII         637
#define DCXXXVIII        638
#define DCXXXIX          639
#define DCXL             640
#define DCXLI            641
#define DCXLII           642
#define DCXLIII          643
#define DCXLIV           644
#define DCXLV            645
#define DCXLVI           646
#define DCXLVII          647
#define DCXLVIII         648
#define DCXLIX           649
#define DCL              650
#define DCLI             651
#define DCLII            652
#define DCLIII           653
#define DCLIV            654
#define DCLV             655
#define DCLVI            656
#define DCLVII           657
#define DCLVIII          658
#define DCLIX            659
#define DCLX             660
#define DCLXI            661
#define DCLXII           662
#define DCLXIII          663
#define DCLXIV           664
#define DCLXV            665
#define DCLXVI           666
#define DCLXVII          667
#define DCLXVIII         668
#define DCLXIX           669
#define DCLXX            670
#define DCLXXI           671
#define DCLXXII          672
#define DCLXXIII         673
#define DCLXXIV          674
#define DCLXXV           675
#define DCLXXVI          676
#define DCLXXVII         677
#define DCLXXVIII        678
#define DCLXXIX          679
#define DCLXXX           680
#define DCLXXXI          681
#define DCLXXXII         682
#define DCLXXXIII        683
#define DCLXXXIV         684
#define DCLXXXV          685
#define DCLXXXVI         686
#define DCLXXXVII        687
#define DCLXXXVIII       688
#define DCLXXXIX         689
#define DCXC             690
#define DCXCI            691
#define DCXCII           692
#define DCXCIII          693
#define DCXCIV           694
#define DCXCV            695
#define DCXCVI           696
#define DCXCVII          697
#define DCXCVIII         698
#define DCXCIX           699
#define DCC              700
#define DCCI             701
#define DCCII            702
#define DCCIII           703
#define DCCIV            704
#define DCCV             705
#define DCCVI            706
#define DCCVII           707
#define DCCVIII          708
#define DCCIX            709
#define DCCX             710
#define DCCXI            711
#define DCCXII           712
#define DCCXIII          713
#define DCCXIV           714
#define DCCXV            715
#define DCCXVI           716
#define DCCXVII          717
#define DCCXVIII         718
#define DCCXIX           719
#define DCCXX            720
#define DCCXXI           721
#define DCCXXII          722
#define DCCXXIII         723
#define DCCXXIV          724
#define DCCXXV           725
#define DCCXXVI          726
#define DCCXXVII         727
#define DCCXXVIII        728
#define DCCXXIX          729
#define DCCXXX           730
#define DCCXXXI          731
#define DCCXXXII         732
#define DCCXXXIII        733
#define DCCXXXIV         734
#define DCCXXXV          735
#define DCCXXXVI         736
#define DCCXXXVII        737
#define DCCXXXVIII       738
#define DCCXXXIX         739
#define DCCXL            740
#define DCCXLI           741
#define DCCXLII          742
#define DCCXLIII         743
#define DCCXLIV          744
#define DCCXLV           745
#define DCCXLVI          746
#define DCCXLVII         747
#define DCCXLVIII        748
#define DCCXLIX          749
#define DCCL             750
#define DCCLI            751
#define DCCLII           752
#define DCCLIII          753
#define DCCLIV           754
#define DCCLV            755
#define DCCLVI           756
#define DCCLVII          757
#define DCCLVIII         758
#define DCCLIX           759
#define DCCLX            760
#define DCCLXI           761
#define DCCLXII          762
#define DCCLXIII         763
#define DCCLXIV          764
#define DCCLXV           765
#define DCCLXVI          766
#define DCCLXVII         767
#define DCCLXVIII        768
#define DCCLXIX          769
#define DCCLXX           770
#define DCCLXXI          771
#define DCCLXXII         772
#define DCCLXXIII        773
#define DCCLXXIV         774
#define DCCLXXV          775
#define DCCLXXVI         776
#define DCCLXXVII        777
#define DCCLXXVIII       778
#define DCCLXXIX         779
#define DCCLXXX          780
#define DCCLXXXI         781
#define DCCLXXXII        782
#define DCCLXXXIII       783
#define DCCLXXXIV        784
#define DCCLXXXV         785
#define DCCLXXXVI        786
#define DCCLXXXVII       787
#define DCCLXXXVIII      788
#define DCCLXXXIX        789
#define DCCXC            790
#define DCCXCI           791
#define DCCXCII          792
#define DCCXCIII         793
#define DCCXCIV          794
#define DCCXCV           795
#define DCCXCVI          796
#define DCCXCVII         797
#define DCCXCVIII        798
#define DCCXCIX          799
#define DCCC             800
#define DCCCI            801
#define DCCCII           802
#define DCCCIII          803
#define DCCCIV           804
#define DCCCV            805
#define DCCCVI           806
#define DCCCVII          807
#define DCCCVIII         808
#define DCCCIX           809
#define DCCCX            810
#define DCCCXI           811
#define DCCCXII          812
#define DCCCXIII         813
#define DCCCXIV          814
#define DCCCXV           815
#define DCCCXVI          816
#define DCCCXVII         817
#define DCCCXVIII        818
#define DCCCXIX          819
#define DCCCXX           820
#define DCCCXXI          821
#define DCCCXXII         822
#define DCCCXXIII        823
#define DCCCXXIV         824
#define DCCCXXV          825
#define DCCCXXVI         826
#define DCCCXXVII        827
#define DCCCXXVIII       828
#define DCCCXXIX         829
#define DCCCXXX          830
#define DCCCXXXI         831
#define DCCCXXXII        832
#define DCCCXXXIII       833
#define DCCCXXXIV        834
#define DCCCXXXV         835
#define DCCCXXXVI        836
#define DCCCXXXVII       837
#define DCCCXXXVIII      838
#define DCCCXXXIX        839
#define DCCCXL           840
#define DCCCXLI          841
#define DCCCXLII         842
#define DCCCXLIII        843
#define DCCCXLIV         844
#define DCCCXLV          845
#define DCCCXLVI         846
#define DCCCXLVII        847
#define DCCCXLVIII       848
#define DCCCXLIX         849
#define DCCCL            850
#define DCCCLI           851
#define DCCCLII          852
#define DCCCLIII         853
#define DCCCLIV          854
#define DCCCLV           855
#define DCCCLVI          856
#define DCCCLVII         857
#define DCCCLVIII        858
#define DCCCLIX          859
#define DCCCLX           860
#define DCCCLXI          861
#define DCCCLXII         862
#define DCCCLXIII        863
#define DCCCLXIV         864
#define DCCCLXV          865
#define DCCCLXVI         866
#define DCCCLXVII        867
#define DCCCLXVIII       868
#define DCCCLXIX         869
#define DCCCLXX          870
#define DCCCLXXI         871
#define DCCCLXXII        872
#define DCCCLXXIII       873
#define DCCCLXXIV        874
#define DCCCLXXV         875
#define DCCCLXXVI        876
#define DCCCLXXVII       877
#define DCCCLXXVIII      878
#define DCCCLXXIX        879
#define DCCCLXXX         880
#define DCCCLXXXI        881
#define DCCCLXXXII       882
#define DCCCLXXXIII      883
#define DCCCLXXXIV       884
#define DCCCLXXXV        885
#define DCCCLXXXVI       886
#define DCCCLXXXVII      887
#define DCCCLXXXVIII     888
#define DCCCLXXXIX       889
#define DCCCXC           890
#define DCCCXCI          891
#define DCCCXCII         892
#define DCCCXCIII        893
#define DCCCXCIV         894
#define DCCCXCV          895
#define DCCCXCVI         896
#define DCCCXCVII        897
#define DCCCXCVIII       898
#define DCCCXCIX         899
#define CM               900
#define CMI              901
#define CMII             902
#define CMIII            903
#define CMIV             904
#define CMV              905
#define CMVI             906
#define CMVII            907
#define CMVIII           908
#define CMIX             909
#define CMX              910
#define CMXI             911
#define CMXII            912
#define CMXIII           913
#define CMXIV            914
#define CMXV             915
#define CMXVI            916
#define CMXVII           917
#define CMXVIII          918
#define CMXIX            919
#define CMXX             920
#define CMXXI            921
#define CMXXII           922
#define CMXXIII          923
#define CMXXIV           924
#define CMXXV            925
#define CMXXVI           926
#define CMXXVII          927
#define CMXXVIII         928
#define CMXXIX           929
#define CMXXX            930
#define CMXXXI           931
#define CMXXXII          932
#define CMXXXIII         933
#define CMXXXIV          934
#define CMXXXV           935
#define CMXXXVI          936
#define CMXXXVII         937
#define CMXXXVIII        938
#define CMXXXIX          939
#define CMXL             940
#define CMXLI            941
#define CMXLII           942
#define CMXLIII          943
#define CMXLIV           944
#define CMXLV            945
#define CMXLVI           946
#define CMXLVII          947
#define CMXLVIII         948
#define CMXLIX           949
#define CML              950
#define CMLI             951
#define CMLII            952
#define CMLIII           953
#define CMLIV            954
#define CMLV             955
#define CMLVI            956
#define CMLVII           957
#define CMLVIII          958
#define CMLIX            959
#define CMLX             960
#define CMLXI            961
#define CMLXII           962
#define CMLXIII          963
#define CMLXIV           964
#define CMLXV            965
#define CMLXVI           966
#define CMLXVII          967
#define CMLXVIII         968
#define CMLXIX           969
#define CMLXX            970
#define CMLXXI           971
#define CMLXXII          972
#define CMLXXIII         973
#define CMLXXIV          974
#define CMLXXV           975
#define CMLXXVI          976
#define CMLXXVII         977
#define CMLXXVIII        978
#define CMLXXIX          979
#define CMLXXX           980
#define CMLXXXI          981
#define CMLXXXII         982
#define CMLXXXIII        983
#define CMLXXXIV         984
#define CMLXXXV          985
#define CMLXXXVI         986
#define CMLXXXVII        987
#define CMLXXXVIII       988
#define CMLXXXIX         989
#define CMXC             990
#define CMXCI            991
#define CMXCII           992
#define CMXCIII          993
#define CMXCIV           994
#define CMXCV            995
#define CMXCVI           996
#define CMXCVII          997
#define CMXCVIII         998
#define CMXCIX           999
#define M                1000
#define MI               1001
#define MII              1002
#define MIII             1003
#define MIV              1004
#define MV               1005
#define MVI              1006
#define MVII             1007
#define MVIII            1008
#define MIX              1009
#define MX               1010
#define MXI              1011
#define MXII             1012
#define MXIII            1013
#define MXIV             1014
#define MXV              1015
#define MXVI             1016
#define MXVII            1017
#define MXVIII           1018
#define MXIX             1019
#define MXX              1020
#define MXXI             1021
#define MXXII            1022
#define MXXIII           1023
#define MXXIV            1024
#define MXXV             1025
#define MXXVI            1026
#define MXXVII           1027
#define MXXVIII          1028
#define MXXIX            1029
#define MXXX             1030
#define MXXXI            1031
#define MXXXII           1032
#define MXXXIII          1033
#define MXXXIV           1034
#define MXXXV            1035
#define MXXXVI           1036
#define MXXXVII          1037
#define MXXXVIII         1038
#define MXXXIX           1039
#define MXL              1040
#define MXLI             1041
#define MXLII            1042
#define MXLIII           1043
#define MXLIV            1044
#define MXLV             1045
#define MXLVI            1046
#define MXLVII           1047
#define MXLVIII          1048
#define MXLIX            1049
#define ML               1050
#define MLI              1051
#define MLII             1052
#define MLIII            1053
#define MLIV             1054
#define MLV              1055
#define MLVI             1056
#define MLVII            1057
#define MLVIII           1058
#define MLIX             1059
#define MLX              1060
#define MLXI             1061
#define MLXII            1062
#define MLXIII           1063
#define MLXIV            1064
#define MLXV             1065
#define MLXVI            1066
#define MLXVII           1067
#define MLXVIII          1068
#define MLXIX            1069
#define MLXX             1070
#define MLXXI            1071
#define MLXXII           1072
#define MLXXIII          1073
#define MLXXIV           1074
#define MLXXV            1075
#define MLXXVI           1076
#define MLXXVII          1077
#define MLXXVIII         1078
#define MLXXIX           1079
#define MLXXX            1080
#define MLXXXI           1081
#define MLXXXII          1082
#define MLXXXIII         1083
#define MLXXXIV          1084
#define MLXXXV           1085
#define MLXXXVI          1086
#define MLXXXVII         1087
#define MLXXXVIII        1088
#define MLXXXIX          1089
#define MXC              1090
#define MXCI             1091
#define MXCII            1092
#define MXCIII           1093
#define MXCIV            1094
#define MXCV             1095
#define MXCVI            1096
#define MXCVII           1097
#define MXCVIII          1098
#define MXCIX            1099
#define MC               1100
#define MCI              1101
#define MCII             1102
#define MCIII            1103
#define MCIV             1104
#define MCV              1105
#define MCVI             1106
#define MCVII            1107
#define MCVIII           1108
#define MCIX             1109
#define MCX              1110
#define MCXI             1111
#define MCXII            1112
#define MCXIII           1113
#define MCXIV            1114
#define MCXV             1115
#define MCXVI            1116
#define MCXVII           1117
#define MCXVIII          1118
#define MCXIX            1119
#define MCXX             1120
#define MCXXI            1121
#define MCXXII           1122
#define MCXXIII          1123
#define MCXXIV           1124
#define MCXXV            1125
#define MCXXVI           1126
#define MCXXVII          1127
#define MCXXVIII         1128
#define MCXXIX           1129
#define MCXXX            1130
#define MCXXXI           1131
#define MCXXXII          1132
#define MCXXXIII         1133
#define MCXXXIV          1134
#define MCXXXV           1135
#define MCXXXVI          1136
#define MCXXXVII         1137
#define MCXXXVIII        1138
#define MCXXXIX          1139
#define MCXL             1140
#define MCXLI            1141
#define MCXLII           1142
#define MCXLIII          1143
#define MCXLIV           1144
#define MCXLV            1145
#define MCXLVI           1146
#define MCXLVII          1147
#define MCXLVIII         1148
#define MCXLIX           1149
#define MCL              1150
#define MCLI             1151
#define MCLII            1152
#define MCLIII           1153
#define MCLIV            1154
#define MCLV             1155
#define MCLVI            1156
#define MCLVII           1157
#define MCLVIII          1158
#define MCLIX            1159
#define MCLX             1160
#define MCLXI            1161
#define MCLXII           1162
#define MCLXIII          1163
#define MCLXIV           1164
#define MCLXV            1165
#define MCLXVI           1166
#define MCLXVII          1167
#define MCLXVIII         1168
#define MCLXIX           1169
#define MCLXX            1170
#define MCLXXI           1171
#define MCLXXII          1172
#define MCLXXIII         1173
#define MCLXXIV          1174
#define MCLXXV           1175
#define MCLXXVI          1176
#define MCLXXVII         1177
#define MCLXXVIII        1178
#define MCLXXIX          1179
#define MCLXXX           1180
#define MCLXXXI          1181
#define MCLXXXII         1182
#define MCLXXXIII        1183
#define MCLXXXIV         1184
#define MCLXXXV          1185
#define MCLXXXVI         1186
#define MCLXXXVII        1187
#define MCLXXXVIII       1188
#define MCLXXXIX         1189
#define MCXC             1190
#define MCXCI            1191
#define MCXCII           1192
#define MCXCIII          1193
#define MCXCIV           1194
#define MCXCV            1195
#define MCXCVI           1196
#define MCXCVII          1197
#define MCXCVIII         1198
#define MCXCIX           1199
#define MCC              1200
#define MCCI             1201
#define MCCII            1202
#define MCCIII           1203
#define MCCIV            1204
#define MCCV             1205
#define MCCVI            1206
#define MCCVII           1207
#define MCCVIII          1208
#define MCCIX            1209
#define MCCX             1210
#define MCCXI            1211
#define MCCXII           1212
#define MCCXIII          1213
#define MCCXIV           1214
#define MCCXV            1215
#define MCCXVI           1216
#define MCCXVII          1217
#define MCCXVIII         1218
#define MCCXIX           1219
#define MCCXX            1220
#define MCCXXI           1221
#define MCCXXII          1222
#define MCCXXIII         1223
#define MCCXXIV          1224
#define MCCXXV           1225
#define MCCXXVI          1226
#define MCCXXVII         1227
#define MCCXXVIII        1228
#define MCCXXIX          1229
#define MCCXXX           1230
#define MCCXXXI          1231
#define MCCXXXII         1232
#define MCCXXXIII        1233
#define MCCXXXIV         1234
#define MCCXXXV          1235
#define MCCXXXVI         1236
#define MCCXXXVII        1237
#define MCCXXXVIII       1238
#define MCCXXXIX         1239
#define MCCXL            1240
#define MCCXLI           1241
#define MCCXLII          1242
#define MCCXLIII         1243
#define MCCXLIV          1244
#define MCCXLV           1245
#define MCCXLVI          1246
#define MCCXLVII         1247
#define MCCXLVIII        1248
#define MCCXLIX          1249
#define MCCL             1250
#define MCCLI            1251
#define MCCLII           1252
#define MCCLIII          1253
#define MCCLIV           1254
#define MCCLV            1255
#define MCCLVI           1256
#define MCCLVII          1257
#define MCCLVIII         1258
#define MCCLIX           1259
#define MCCLX            1260
#define MCCLXI           1261
#define MCCLXII          1262
#define MCCLXIII         1263
#define MCCLXIV          1264
#define MCCLXV           1265
#define MCCLXVI          1266
#define MCCLXVII         1267
#define MCCLXVIII        1268
#define MCCLXIX          1269
#define MCCLXX           1270
#define MCCLXXI          1271
#define MCCLXXII         1272
#define MCCLXXIII        1273
#define MCCLXXIV         1274
#define MCCLXXV          1275
#define MCCLXXVI         1276
#define MCCLXXVII        1277
#define MCCLXXVIII       1278
#define MCCLXXIX         1279
#define MCCLXXX          1280
#define MCCLXXXI         1281
#define MCCLXXXII        1282
#define MCCLXXXIII       1283
#define MCCLXXXIV        1284
#define MCCLXXXV         1285
#define MCCLXXXVI        1286
#define MCCLXXXVII       1287
#define MCCLXXXVIII      1288
#define MCCLXXXIX        1289
#define MCCXC            1290
#define MCCXCI           1291
#define MCCXCII          1292
#define MCCXCIII         1293
#define MCCXCIV          1294
#define MCCXCV           1295
#define MCCXCVI          1296
#define MCCXCVII         1297
#define MCCXCVIII        1298
#define MCCXCIX          1299
#define MCCC             1300
#define MCCCI            1301
#define MCCCII           1302
#define MCCCIII          1303
#define MCCCIV           1304
#define MCCCV            1305
#define MCCCVI           1306
#define MCCCVII          1307
#define MCCCVIII         1308
#define MCCCIX           1309
#define MCCCX            1310
#define MCCCXI           1311
#define MCCCXII          1312
#define MCCCXIII         1313
#define MCCCXIV          1314
#define MCCCXV           1315
#define MCCCXVI          1316
#define MCCCXVII         1317
#define MCCCXVIII        1318
#define MCCCXIX          1319
#define MCCCXX           1320
#define MCCCXXI          1321
#define MCCCXXII         1322
#define MCCCXXIII        1323
#define MCCCXXIV         1324
#define MCCCXXV          1325
#define MCCCXXVI         1326
#define MCCCXXVII        1327
#define MCCCXXVIII       1328
#define MCCCXXIX         1329
#define MCCCXXX          1330
#define MCCCXXXI         1331
#define MCCCXXXII        1332
#define MCCCXXXIII       1333
#define MCCCXXXIV        1334
#define MCCCXXXV         1335
#define MCCCXXXVI        1336
#define MCCCXXXVII       1337
#define MCCCXXXVIII      1338
#define MCCCXXXIX        1339
#define MCCCXL           1340
#define MCCCXLI          1341
#define MCCCXLII         1342
#define MCCCXLIII        1343
#define MCCCXLIV         1344
#define MCCCXLV          1345
#define MCCCXLVI         1346
#define MCCCXLVII        1347
#define MCCCXLVIII       1348
#define MCCCXLIX         1349
#define MCCCL            1350
#define MCCCLI           1351
#define MCCCLII          1352
#define MCCCLIII         1353
#define MCCCLIV          1354
#define MCCCLV           1355
#define MCCCLVI          1356
#define MCCCLVII         1357
#define MCCCLVIII        1358
#define MCCCLIX          1359
#define MCCCLX           1360
#define MCCCLXI          1361
#define MCCCLXII         1362
#define MCCCLXIII        1363
#define MCCCLXIV         1364
#define MCCCLXV          1365
#define MCCCLXVI         1366
#define MCCCLXVII        1367
#define MCCCLXVIII       1368
#define MCCCLXIX         1369
#define MCCCLXX          1370
#define MCCCLXXI         1371
#define MCCCLXXII        1372
#define MCCCLXXIII       1373
#define MCCCLXXIV        1374
#define MCCCLXXV         1375
#define MCCCLXXVI        1376
#define MCCCLXXVII       1377
#define MCCCLXXVIII      1378
#define MCCCLXXIX        1379
#define MCCCLXXX         1380
#define MCCCLXXXI        1381
#define MCCCLXXXII       1382
#define MCCCLXXXIII      1383
#define MCCCLXXXIV       1384
#define MCCCLXXXV        1385
#define MCCCLXXXVI       1386
#define MCCCLXXXVII      1387
#define MCCCLXXXVIII     1388
#define MCCCLXXXIX       1389
#define MCCCXC           1390
#define MCCCXCI          1391
#define MCCCXCII         1392
#define MCCCXCIII        1393
#define MCCCXCIV         1394
#define MCCCXCV          1395
#define MCCCXCVI         1396
#define MCCCXCVII        1397
#define MCCCXCVIII       1398
#define MCCCXCIX         1399
#define MCD              1400
#define MCDI             1401
#define MCDII            1402
#define MCDIII           1403
#define MCDIV            1404
#define MCDV             1405
#define MCDVI            1406
#define MCDVII           1407
#define MCDVIII          1408
#define MCDIX            1409
#define MCDX             1410
#define MCDXI            1411
#define MCDXII           1412
#define MCDXIII          1413
#define MCDXIV           1414
#define MCDXV            1415
#define MCDXVI           1416
#define MCDXVII          1417
#define MCDXVIII         1418
#define MCDXIX           1419
#define MCDXX            1420
#define MCDXXI           1421
#define MCDXXII          1422
#define MCDXXIII         1423
#define MCDXXIV          1424
#define MCDXXV           1425
#define MCDXXVI          1426
#define MCDXXVII         1427
#define MCDXXVIII        1428
#define MCDXXIX          1429
#define MCDXXX           1430
#define MCDXXXI          1431
#define MCDXXXII         1432
#define MCDXXXIII        1433
#define MCDXXXIV         1434
#define MCDXXXV          1435
#define MCDXXXVI         1436
#define MCDXXXVII        1437
#define MCDXXXVIII       1438
#define MCDXXXIX         1439
#define MCDXL            1440
#define MCDXLI           1441
#define MCDXLII          1442
#define MCDXLIII         1443
#define MCDXLIV          1444
#define MCDXLV           1445
#define MCDXLVI          1446
#define MCDXLVII         1447
#define MCDXLVIII        1448
#define MCDXLIX          1449
#define MCDL             1450
#define MCDLI            1451
#define MCDLII           1452
#define MCDLIII          1453
#define MCDLIV           1454
#define MCDLV            1455
#define MCDLVI           1456
#define MCDLVII          1457
#define MCDLVIII         1458
#define MCDLIX           1459
#define MCDLX            1460
#define MCDLXI           1461
#define MCDLXII          1462
#define MCDLXIII         1463
#define MCDLXIV          1464
#define MCDLXV           1465
#define MCDLXVI          1466
#define MCDLXVII         1467
#define MCDLXVIII        1468
#define MCDLXIX          1469
#define MCDLXX           1470
#define MCDLXXI          1471
#define MCDLXXII         1472
#define MCDLXXIII        1473
#define MCDLXXIV         1474
#define MCDLXXV          1475
#define MCDLXXVI         1476
#define MCDLXXVII        1477
#define MCDLXXVIII       1478
#define MCDLXXIX         1479
#define MCDLXXX          1480
#define MCDLXXXI         1481
#define MCDLXXXII        1482
#define MCDLXXXIII       1483
#define MCDLXXXIV        1484
#define MCDLXXXV         1485
#define MCDLXXXVI        1486
#define MCDLXXXVII       1487
#define MCDLXXXVIII      1488
#define MCDLXXXIX        1489
#define MCDXC            1490
#define MCDXCI           1491
#define MCDXCII          1492
#define MCDXCIII         1493
#define MCDXCIV          1494
#define MCDXCV           1495
#define MCDXCVI          1496
#define MCDXCVII         1497
#define MCDXCVIII        1498
#define MCDXCIX          1499
#define MD               1500
#define MDI              1501
#define MDII             1502
#define MDIII            1503
#define MDIV             1504
#define MDV              1505
#define MDVI             1506
#define MDVII            1507
#define MDVIII           1508
#define MDIX             1509
#define MDX              1510
#define MDXI             1511
#define MDXII            1512
#define MDXIII           1513
#define MDXIV            1514
#define MDXV             1515
#define MDXVI            1516
#define MDXVII           1517
#define MDXVIII          1518
#define MDXIX            1519
#define MDXX             1520
#define MDXXI            1521
#define MDXXII           1522
#define MDXXIII          1523
#define MDXXIV           1524
#define MDXXV            1525
#define MDXXVI           1526
#define MDXXVII          1527
#define MDXXVIII         1528
#define MDXXIX           1529
#define MDXXX            1530
#define MDXXXI           1531
#define MDXXXII          1532
#define MDXXXIII         1533
#define MDXXXIV          1534
#define MDXXXV           1535
#define MDXXXVI          1536
#define MDXXXVII         1537
#define MDXXXVIII        1538
#define MDXXXIX          1539
#define MDXL             1540
#define MDXLI            1541
#define MDXLII           1542
#define MDXLIII          1543
#define MDXLIV           1544
#define MDXLV            1545
#define MDXLVI           1546
#define MDXLVII          1547
#define MDXLVIII         1548
#define MDXLIX           1549
#define MDL              1550
#define MDLI             1551
#define MDLII            1552
#define MDLIII           1553
#define MDLIV            1554
#define MDLV             1555
#define MDLVI            1556
#define MDLVII           1557
#define MDLVIII          1558
#define MDLIX            1559
#define MDLX             1560
#define MDLXI            1561
#define MDLXII           1562
#define MDLXIII          1563
#define MDLXIV           1564
#define MDLXV            1565
#define MDLXVI           1566
#define MDLXVII          1567
#define MDLXVIII         1568
#define MDLXIX           1569
#define MDLXX            1570
#define MDLXXI           1571
#define MDLXXII          1572
#define MDLXXIII         1573
#define MDLXXIV          1574
#define MDLXXV           1575
#define MDLXXVI          1576
#define MDLXXVII         1577
#define MDLXXVIII        1578
#define MDLXXIX          1579
#define MDLXXX           1580
#define MDLXXXI          1581
#define MDLXXXII         1582
#define MDLXXXIII        1583
#define MDLXXXIV         1584
#define MDLXXXV          1585
#define MDLXXXVI         1586
#define MDLXXXVII        1587
#define MDLXXXVIII       1588
#define MDLXXXIX         1589
#define MDXC             1590
#define MDXCI            1591
#define MDXCII           1592
#define MDXCIII          1593
#define MDXCIV           1594
#define MDXCV            1595
#define MDXCVI           1596
#define MDXCVII          1597
#define MDXCVIII         1598
#define MDXCIX           1599
#define MDC              1600
#define MDCI             1601
#define MDCII            1602
#define MDCIII           1603
#define MDCIV            1604
#define MDCV             1605
#define MDCVI            1606
#define MDCVII           1607
#define MDCVIII          1608
#define MDCIX            1609
#define MDCX             1610
#define MDCXI            1611
#define MDCXII           1612
#define MDCXIII          1613
#define MDCXIV           1614
#define MDCXV            1615
#define MDCXVI           1616
#define MDCXVII          1617
#define MDCXVIII         1618
#define MDCXIX           1619
#define MDCXX            1620
#define MDCXXI           1621
#define MDCXXII          1622
#define MDCXXIII         1623
#define MDCXXIV          1624
#define MDCXXV           1625
#define MDCXXVI          1626
#define MDCXXVII         1627
#define MDCXXVIII        1628
#define MDCXXIX          1629
#define MDCXXX           1630
#define MDCXXXI          1631
#define MDCXXXII         1632
#define MDCXXXIII        1633
#define MDCXXXIV         1634
#define MDCXXXV          1635
#define MDCXXXVI         1636
#define MDCXXXVII        1637
#define MDCXXXVIII       1638
#define MDCXXXIX         1639
#define MDCXL            1640
#define MDCXLI           1641
#define MDCXLII          1642
#define MDCXLIII         1643
#define MDCXLIV          1644
#define MDCXLV           1645
#define MDCXLVI          1646
#define MDCXLVII         1647
#define MDCXLVIII        1648
#define MDCXLIX          1649
#define MDCL             1650
#define MDCLI            1651
#define MDCLII           1652
#define MDCLIII          1653
#define MDCLIV           1654
#define MDCLV            1655
#define MDCLVI           1656
#define MDCLVII          1657
#define MDCLVIII         1658
#define MDCLIX           1659
#define MDCLX            1660
#define MDCLXI           1661
#define MDCLXII          1662
#define MDCLXIII         1663
#define MDCLXIV          1664
#define MDCLXV           1665
#define MDCLXVI          1666
#define MDCLXVII         1667
#define MDCLXVIII        1668
#define MDCLXIX          1669
#define MDCLXX           1670
#define MDCLXXI          1671
#define MDCLXXII         1672
#define MDCLXXIII        1673
#define MDCLXXIV         1674
#define MDCLXXV          1675
#define MDCLXXVI         1676
#define MDCLXXVII        1677
#define MDCLXXVIII       1678
#define MDCLXXIX         1679
#define MDCLXXX          1680
#define MDCLXXXI         1681
#define MDCLXXXII        1682
#define MDCLXXXIII       1683
#define MDCLXXXIV        1684
#define MDCLXXXV         1685
#define MDCLXXXVI        1686
#define MDCLXXXVII       1687
#define MDCLXXXVIII      1688
#define MDCLXXXIX        1689
#define MDCXC            1690
#define MDCXCI           1691
#define MDCXCII          1692
#define MDCXCIII         1693
#define MDCXCIV          1694
#define MDCXCV           1695
#define MDCXCVI          1696
#define MDCXCVII         1697
#define MDCXCVIII        1698
#define MDCXCIX          1699
#define MDCC             1700
#define MDCCI            1701
#define MDCCII           1702
#define MDCCIII          1703
#define MDCCIV           1704
#define MDCCV            1705
#define MDCCVI           1706
#define MDCCVII          1707
#define MDCCVIII         1708
#define MDCCIX           1709
#define MDCCX            1710
#define MDCCXI           1711
#define MDCCXII          1712
#define MDCCXIII         1713
#define MDCCXIV          1714
#define MDCCXV           1715
#define MDCCXVI          1716
#define MDCCXVII         1717
#define MDCCXVIII        1718
#define MDCCXIX          1719
#define MDCCXX           1720
#define MDCCXXI          1721
#define MDCCXXII         1722
#define MDCCXXIII        1723
#define MDCCXXIV         1724
#define MDCCXXV          1725
#define MDCCXXVI         1726
#define MDCCXXVII        1727
#define MDCCXXVIII       1728
#define MDCCXXIX         1729
#define MDCCXXX          1730
#define MDCCXXXI         1731
#define MDCCXXXII        1732
#define MDCCXXXIII       1733
#define MDCCXXXIV        1734
#define MDCCXXXV         1735
#define MDCCXXXVI        1736
#define MDCCXXXVII       1737
#define MDCCXXXVIII      1738
#define MDCCXXXIX        1739
#define MDCCXL           1740
#define MDCCXLI          1741
#define MDCCXLII         1742
#define MDCCXLIII        1743
#define MDCCXLIV         1744
#define MDCCXLV          1745
#define MDCCXLVI         1746
#define MDCCXLVII        1747
#define MDCCXLVIII       1748
#define MDCCXLIX         1749
#define MDCCL            1750
#define MDCCLI           1751
#define MDCCLII          1752
#define MDCCLIII         1753
#define MDCCLIV          1754
#define MDCCLV           1755
#define MDCCLVI          1756
#define MDCCLVII         1757
#define MDCCLVIII        1758
#define MDCCLIX          1759
#define MDCCLX           1760
#define MDCCLXI          1761
#define MDCCLXII         1762
#define MDCCLXIII        1763
#define MDCCLXIV         1764
#define MDCCLXV          1765
#define MDCCLXVI         1766
#define MDCCLXVII        1767
#define MDCCLXVIII       1768
#define MDCCLXIX         1769
#define MDCCLXX          1770
#define MDCCLXXI         1771
#define MDCCLXXII        1772
#define MDCCLXXIII       1773
#define MDCCLXXIV        1774
#define MDCCLXXV         1775
#define MDCCLXXVI        1776
#define MDCCLXXVII       1777
#define MDCCLXXVIII      1778
#define MDCCLXXIX        1779
#define MDCCLXXX         1780
#define MDCCLXXXI        1781
#define MDCCLXXXII       1782
#define MDCCLXXXIII      1783
#define MDCCLXXXIV       1784
#define MDCCLXXXV        1785
#define MDCCLXXXVI       1786
#define MDCCLXXXVII      1787
#define MDCCLXXXVIII     1788
#define MDCCLXXXIX       1789
#define MDCCXC           1790
#define MDCCXCI          1791
#define MDCCXCII         1792
#define MDCCXCIII        1793
#define MDCCXCIV         1794
#define MDCCXCV          1795
#define MDCCXCVI         1796
#define MDCCXCVII        1797
#define MDCCXCVIII       1798
#define MDCCXCIX         1799
#define MDCCC            1800
#define MDCCCI           1801
#define MDCCCII          1802
#define MDCCCIII         1803
#define MDCCCIV          1804
#define MDCCCV           1805
#define MDCCCVI          1806
#define MDCCCVII         1807
#define MDCCCVIII        1808
#define MDCCCIX          1809
#define MDCCCX           1810
#define MDCCCXI          1811
#define MDCCCXII         1812
#define MDCCCXIII        1813
#define MDCCCXIV         1814
#define MDCCCXV          1815
#define MDCCCXVI         1816
#define MDCCCXVII        1817
#define MDCCCXVIII       1818
#define MDCCCXIX         1819
#define MDCCCXX          1820
#define MDCCCXXI         1821
#define MDCCCXXII        1822
#define MDCCCXXIII       1823
#define MDCCCXXIV        1824
#define MDCCCXXV         1825
#define MDCCCXXVI        1826
#define MDCCCXXVII       1827
#define MDCCCXXVIII      1828
#define MDCCCXXIX        1829
#define MDCCCXXX         1830
#define MDCCCXXXI        1831
#define MDCCCXXXII       1832
#define MDCCCXXXIII      1833
#define MDCCCXXXIV       1834
#define MDCCCXXXV        1835
#define MDCCCXXXVI       1836
#define MDCCCXXXVII      1837
#define MDCCCXXXVIII     1838
#define MDCCCXXXIX       1839
#define MDCCCXL          1840
#define MDCCCXLI         1841
#define MDCCCXLII        1842
#define MDCCCXLIII       1843
#define MDCCCXLIV        1844
#define MDCCCXLV         1845
#define MDCCCXLVI        1846
#define MDCCCXLVII       1847
#define MDCCCXLVIII      1848
#define MDCCCXLIX        1849
#define MDCCCL           1850
#define MDCCCLI          1851
#define MDCCCLII         1852
#define MDCCCLIII        1853
#define MDCCCLIV         1854
#define MDCCCLV          1855
#define MDCCCLVI         1856
#define MDCCCLVII        1857
#define MDCCCLVIII       1858
#define MDCCCLIX         1859
#define MDCCCLX          1860
#define MDCCCLXI         1861
#define MDCCCLXII        1862
#define MDCCCLXIII       1863
#define MDCCCLXIV        1864
#define MDCCCLXV         1865
#define MDCCCLXVI        1866
#define MDCCCLXVII       1867
#define MDCCCLXVIII      1868
#define MDCCCLXIX        1869
#define MDCCCLXX         1870
#define MDCCCLXXI        1871
#define MDCCCLXXII       1872
#define MDCCCLXXIII      1873
#define MDCCCLXXIV       1874
#define MDCCCLXXV        1875
#define MDCCCLXXVI       1876
#define MDCCCLXXVII      1877
#define MDCCCLXXVIII     1878
#define MDCCCLXXIX       1879
#define MDCCCLXXX        1880
#define MDCCCLXXXI       1881
#define MDCCCLXXXII      1882
#define MDCCCLXXXIII     1883
#define MDCCCLXXXIV      1884
#define MDCCCLXXXV       1885
#define MDCCCLXXXVI      1886
#define MDCCCLXXXVII     1887
#define MDCCCLXXXVIII    1888
#define MDCCCLXXXIX      1889
#define MDCCCXC          1890
#define MDCCCXCI         1891
#define MDCCCXCII        1892
#define MDCCCXCIII       1893
#define MDCCCXCIV        1894
#define MDCCCXCV         1895
#define MDCCCXCVI        1896
#define MDCCCXCVII       1897
#define MDCCCXCVIII      1898
#define MDCCCXCIX        1899
#define MCM              1900
#define MCMI             1901
#define MCMII            1902
#define MCMIII           1903
#define MCMIV            1904
#define MCMV             1905
#define MCMVI            1906
#define MCMVII           1907
#define MCMVIII          1908
#define MCMIX            1909
#define MCMX             1910
#define MCMXI            1911
#define MCMXII           1912
#define MCMXIII          1913
#define MCMXIV           1914
#define MCMXV            1915
#define MCMXVI           1916
#define MCMXVII          1917
#define MCMXVIII         1918
#define MCMXIX           1919
#define MCMXX            1920
#define MCMXXI           1921
#define MCMXXII          1922
#define MCMXXIII         1923
#define MCMXXIV          1924
#define MCMXXV           1925
#define MCMXXVI          1926
#define MCMXXVII         1927
#define MCMXXVIII        1928
#define MCMXXIX          1929
#define MCMXXX           1930
#define MCMXXXI          1931
#define MCMXXXII         1932
#define MCMXXXIII        1933
#define MCMXXXIV         1934
#define MCMXXXV          1935
#define MCMXXXVI         1936
#define MCMXXXVII        1937
#define MCMXXXVIII       1938
#define MCMXXXIX         1939
#define MCMXL            1940
#define MCMXLI           1941
#define MCMXLII          1942
#define MCMXLIII         1943
#define MCMXLIV          1944
#define MCMXLV           1945
#define MCMXLVI          1946
#define MCMXLVII         1947
#define MCMXLVIII        1948
#define MCMXLIX          1949
#define MCML             1950
#define MCMLI            1951
#define MCMLII           1952
#define MCMLIII          1953
#define MCMLIV           1954
#define MCMLV            1955
#define MCMLVI           1956
#define MCMLVII          1957
#define MCMLVIII         1958
#define MCMLIX           1959
#define MCMLX            1960
#define MCMLXI           1961
#define MCMLXII          1962
#define MCMLXIII         1963
#define MCMLXIV          1964
#define MCMLXV           1965
#define MCMLXVI          1966
#define MCMLXVII         1967
#define MCMLXVIII        1968
#define MCMLXIX          1969
#define MCMLXX           1970
#define MCMLXXI          1971
#define MCMLXXII         1972
#define MCMLXXIII        1973
#define MCMLXXIV         1974
#define MCMLXXV          1975
#define MCMLXXVI         1976
#define MCMLXXVII        1977
#define MCMLXXVIII       1978
#define MCMLXXIX         1979
#define MCMLXXX          1980
#define MCMLXXXI         1981
#define MCMLXXXII        1982
#define MCMLXXXIII       1983
#define MCMLXXXIV        1984
#define MCMLXXXV         1985
#define MCMLXXXVI        1986
#define MCMLXXXVII       1987
#define MCMLXXXVIII      1988
#define MCMLXXXIX        1989
#define MCMXC            1990
#define MCMXCI           1991
#define MCMXCII          1992
#define MCMXCIII         1993
#define MCMXCIV          1994
#define MCMXCV           1995
#define MCMXCVI          1996
#define MCMXCVII         1997
#define MCMXCVIII        1998
#define MCMXCIX          1999
#define MM               2000
#define MMI              2001
#define MMII             2002
#define MMIII            2003
#define MMIV             2004
#define MMV              2005
#define MMVI             2006
#define MMVII            2007
#define MMVIII           2008
#define MMIX             2009
#define MMX              2010
#define MMXI             2011
#define MMXII            2012
#define MMXIII           2013
#define MMXIV            2014
#define MMXV             2015
#define MMXVI            2016
#define MMXVII           2017
#define MMXVIII          2018
#define MMXIX            2019
#define MMXX             2020
#define MMXXI            2021
#define MMXXII           2022
#define MMXXIII          2023
#define MMXXIV           2024
#define MMXXV            2025
#define MMXXVI           2026
#define MMXXVII          2027
#define MMXXVIII         2028
#define MMXXIX           2029
#define MMXXX            2030
#define MMXXXI           2031
#define MMXXXII          2032
#define MMXXXIII         2033
#define MMXXXIV          2034
#define MMXXXV           2035
#define MMXXXVI          2036
#define MMXXXVII         2037
#define MMXXXVIII        2038
#define MMXXXIX          2039
#define MMXL             2040
#define MMXLI            2041
#define MMXLII           2042
#define MMXLIII          2043
#define MMXLIV           2044
#define MMXLV            2045
#define MMXLVI           2046
#define MMXLVII          2047
#define MMXLVIII         2048
#define MMXLIX           2049
#define MML              2050
#define MMLI             2051
#define MMLII            2052
#define MMLIII           2053
#define MMLIV            2054
#define MMLV             2055
#define MMLVI            2056
#define MMLVII           2057
#define MMLVIII          2058
#define MMLIX            2059
#define MMLX             2060
#define MMLXI            2061
#define MMLXII           2062
#define MMLXIII          2063
#define MMLXIV           2064
#define MMLXV            2065
#define MMLXVI           2066
#define MMLXVII          2067
#define MMLXVIII         2068
#define MMLXIX           2069
#define MMLXX            2070
#define MMLXXI           2071
#define MMLXXII          2072
#define MMLXXIII         2073
#define MMLXXIV          2074
#define MMLXXV           2075
#define MMLXXVI          2076
#define MMLXXVII         2077
#define MMLXXVIII        2078
#define MMLXXIX          2079
#define MMLXXX           2080
#define MMLXXXI          2081
#define MMLXXXII         2082
#define MMLXXXIII        2083
#define MMLXXXIV         2084
#define MMLXXXV          2085
#define MMLXXXVI         2086
#define MMLXXXVII        2087
#define MMLXXXVIII       2088
#define MMLXXXIX         2089
#define MMXC             2090
#define MMXCI            2091
#define MMXCII           2092
#define MMXCIII          2093
#define MMXCIV           2094
#define MMXCV            2095
#define MMXCVI           2096
#define MMXCVII          2097
#define MMXCVIII         2098
#define MMXCIX           2099
#define MMC              2100
#define MMCI             2101
#define MMCII            2102
#define MMCIII           2103
#define MMCIV            2104
#define MMCV             2105
#define MMCVI            2106
#define MMCVII           2107
#define MMCVIII          2108
#define MMCIX            2109
#define MMCX             2110
#define MMCXI            2111
#define MMCXII           2112
#define MMCXIII          2113
#define MMCXIV           2114
#define MMCXV            2115
#define MMCXVI           2116
#define MMCXVII          2117
#define MMCXVIII         2118
#define MMCXIX           2119
#define MMCXX            2120
#define MMCXXI           2121
#define MMCXXII          2122
#define MMCXXIII         2123
#define MMCXXIV          2124
#define MMCXXV           2125
#define MMCXXVI          2126
#define MMCXXVII         2127
#define MMCXXVIII        2128
#define MMCXXIX          2129
#define MMCXXX           2130
#define MMCXXXI          2131
#define MMCXXXII         2132
#define MMCXXXIII        2133
#define MMCXXXIV         2134
#define MMCXXXV          2135
#define MMCXXXVI         2136
#define MMCXXXVII        2137
#define MMCXXXVIII       2138
#define MMCXXXIX         2139
#define MMCXL            2140
#define MMCXLI           2141
#define MMCXLII          2142
#define MMCXLIII         2143
#define MMCXLIV          2144
#define MMCXLV           2145
#define MMCXLVI          2146
#define MMCXLVII         2147
#define MMCXLVIII        2148
#define MMCXLIX          2149
#define MMCL             2150
#define MMCLI            2151
#define MMCLII           2152
#define MMCLIII          2153
#define MMCLIV           2154
#define MMCLV            2155
#define MMCLVI           2156
#define MMCLVII          2157
#define MMCLVIII         2158
#define MMCLIX           2159
#define MMCLX            2160
#define MMCLXI           2161
#define MMCLXII          2162
#define MMCLXIII         2163
#define MMCLXIV          2164
#define MMCLXV           2165
#define MMCLXVI          2166
#define MMCLXVII         2167
#define MMCLXVIII        2168
#define MMCLXIX          2169
#define MMCLXX           2170
#define MMCLXXI          2171
#define MMCLXXII         2172
#define MMCLXXIII        2173
#define MMCLXXIV         2174
#define MMCLXXV          2175
#define MMCLXXVI         2176
#define MMCLXXVII        2177
#define MMCLXXVIII       2178
#define MMCLXXIX         2179
#define MMCLXXX          2180
#define MMCLXXXI         2181
#define MMCLXXXII        2182
#define MMCLXXXIII       2183
#define MMCLXXXIV        2184
#define MMCLXXXV         2185
#define MMCLXXXVI        2186
#define MMCLXXXVII       2187
#define MMCLXXXVIII      2188
#define MMCLXXXIX        2189
#define MMCXC            2190
#define MMCXCI           2191
#define MMCXCII          2192
#define MMCXCIII         2193
#define MMCXCIV          2194
#define MMCXCV           2195
#define MMCXCVI          2196
#define MMCXCVII         2197
#define MMCXCVIII        2198
#define MMCXCIX          2199
#define MMCC             2200
#define MMCCI            2201
#define MMCCII           2202
#define MMCCIII          2203
#define MMCCIV           2204
#define MMCCV            2205
#define MMCCVI           2206
#define MMCCVII          2207
#define MMCCVIII         2208
#define MMCCIX           2209
#define MMCCX            2210
#define MMCCXI           2211
#define MMCCXII          2212
#define MMCCXIII         2213
#define MMCCXIV          2214
#define MMCCXV           2215
#define MMCCXVI          2216
#define MMCCXVII         2217
#define MMCCXVIII        2218
#define MMCCXIX          2219
#define MMCCXX           2220
#define MMCCXXI          2221
#define MMCCXXII         2222
#define MMCCXXIII        2223
#define MMCCXXIV         2224
#define MMCCXXV          2225
#define MMCCXXVI         2226
#define MMCCXXVII        2227
#define MMCCXXVIII       2228
#define MMCCXXIX         2229
#define MMCCXXX          2230
#define MMCCXXXI         2231
#define MMCCXXXII        2232
#define MMCCXXXIII       2233
#define MMCCXXXIV        2234
#define MMCCXXXV         2235
#define MMCCXXXVI        2236
#define MMCCXXXVII       2237
#define MMCCXXXVIII      2238
#define MMCCXXXIX        2239
#define MMCCXL           2240
#define MMCCXLI          2241
#define MMCCXLII         2242
#define MMCCXLIII        2243
#define MMCCXLIV         2244
#define MMCCXLV          2245
#define MMCCXLVI         2246
#define MMCCXLVII        2247
#define MMCCXLVIII       2248
#define MMCCXLIX         2249
#define MMCCL            2250
#define MMCCLI           2251
#define MMCCLII          2252
#define MMCCLIII         2253
#define MMCCLIV          2254
#define MMCCLV           2255
#define MMCCLVI          2256
#define MMCCLVII         2257
#define MMCCLVIII        2258
#define MMCCLIX          2259
#define MMCCLX           2260
#define MMCCLXI          2261
#define MMCCLXII         2262
#define MMCCLXIII        2263
#define MMCCLXIV         2264
#define MMCCLXV          2265
#define MMCCLXVI         2266
#define MMCCLXVII        2267
#define MMCCLXVIII       2268
#define MMCCLXIX         2269
#define MMCCLXX          2270
#define MMCCLXXI         2271
#define MMCCLXXII        2272
#define MMCCLXXIII       2273
#define MMCCLXXIV        2274
#define MMCCLXXV         2275
#define MMCCLXXVI        2276
#define MMCCLXXVII       2277
#define MMCCLXXVIII      2278
#define MMCCLXXIX        2279
#define MMCCLXXX         2280
#define MMCCLXXXI        2281
#define MMCCLXXXII       2282
#define MMCCLXXXIII      2283
#define MMCCLXXXIV       2284
#define MMCCLXXXV        2285
#define MMCCLXXXVI       2286
#define MMCCLXXXVII      2287
#define MMCCLXXXVIII     2288
#define MMCCLXXXIX       2289
#define MMCCXC           2290
#define MMCCXCI          2291
#define MMCCXCII         2292
#define MMCCXCIII        2293
#define MMCCXCIV         2294
#define MMCCXCV          2295
#define MMCCXCVI         2296
#define MMCCXCVII        2297
#define MMCCXCVIII       2298
#define MMCCXCIX         2299
#define MMCCC            2300
#define MMCCCI           2301
#define MMCCCII          2302
#define MMCCCIII         2303
#define MMCCCIV          2304
#define MMCCCV           2305
#define MMCCCVI          2306
#define MMCCCVII         2307
#define MMCCCVIII        2308
#define MMCCCIX          2309
#define MMCCCX           2310
#define MMCCCXI          2311
#define MMCCCXII         2312
#define MMCCCXIII        2313
#define MMCCCXIV         2314
#define MMCCCXV          2315
#define MMCCCXVI         2316
#define MMCCCXVII        2317
#define MMCCCXVIII       2318
#define MMCCCXIX         2319
#define MMCCCXX          2320
#define MMCCCXXI         2321
#define MMCCCXXII        2322
#define MMCCCXXIII       2323
#define MMCCCXXIV        2324
#define MMCCCXXV         2325
#define MMCCCXXVI        2326
#define MMCCCXXVII       2327
#define MMCCCXXVIII      2328
#define MMCCCXXIX        2329
#define MMCCCXXX         2330
#define MMCCCXXXI        2331
#define MMCCCXXXII       2332
#define MMCCCXXXIII      2333
#define MMCCCXXXIV       2334
#define MMCCCXXXV        2335
#define MMCCCXXXVI       2336
#define MMCCCXXXVII      2337
#define MMCCCXXXVIII     2338
#define MMCCCXXXIX       2339
#define MMCCCXL          2340
#define MMCCCXLI         2341
#define MMCCCXLII        2342
#define MMCCCXLIII       2343
#define MMCCCXLIV        2344
#define MMCCCXLV         2345
#define MMCCCXLVI        2346
#define MMCCCXLVII       2347
#define MMCCCXLVIII      2348
#define MMCCCXLIX        2349
#define MMCCCL           2350
#define MMCCCLI          2351
#define MMCCCLII         2352
#define MMCCCLIII        2353
#define MMCCCLIV         2354
#define MMCCCLV          2355
#define MMCCCLVI         2356
#define MMCCCLVII        2357
#define MMCCCLVIII       2358
#define MMCCCLIX         2359
#define MMCCCLX          2360
#define MMCCCLXI         2361
#define MMCCCLXII        2362
#define MMCCCLXIII       2363
#define MMCCCLXIV        2364
#define MMCCCLXV         2365
#define MMCCCLXVI        2366
#define MMCCCLXVII       2367
#define MMCCCLXVIII      2368
#define MMCCCLXIX        2369
#define MMCCCLXX         2370
#define MMCCCLXXI        2371
#define MMCCCLXXII       2372
#define MMCCCLXXIII      2373
#define MMCCCLXXIV       2374
#define MMCCCLXXV        2375
#define MMCCCLXXVI       2376
#define MMCCCLXXVII      2377
#define MMCCCLXXVIII     2378
#define MMCCCLXXIX       2379
#define MMCCCLXXX        2380
#define MMCCCLXXXI       2381
#define MMCCCLXXXII      2382
#define MMCCCLXXXIII     2383
#define MMCCCLXXXIV      2384
#define MMCCCLXXXV       2385
#define MMCCCLXXXVI      2386
#define MMCCCLXXXVII     2387
#define MMCCCLXXXVIII    2388
#define MMCCCLXXXIX      2389
#define MMCCCXC          2390
#define MMCCCXCI         2391
#define MMCCCXCII        2392
#define MMCCCXCIII       2393
#define MMCCCXCIV        2394
#define MMCCCXCV         2395
#define MMCCCXCVI        2396
#define MMCCCXCVII       2397
#define MMCCCXCVIII      2398
#define MMCCCXCIX        2399
#define MMCD             2400
#define MMCDI            2401
#define MMCDII           2402
#define MMCDIII          2403
#define MMCDIV           2404
#define MMCDV            2405
#define MMCDVI           2406
#define MMCDVII          2407
#define MMCDVIII         2408
#define MMCDIX           2409
#define MMCDX            2410
#define MMCDXI           2411
#define MMCDXII          2412
#define MMCDXIII         2413
#define MMCDXIV          2414
#define MMCDXV           2415
#define MMCDXVI          2416
#define MMCDXVII         2417
#define MMCDXVIII        2418
#define MMCDXIX          2419
#define MMCDXX           2420
#define MMCDXXI          2421
#define MMCDXXII         2422
#define MMCDXXIII        2423
#define MMCDXXIV         2424
#define MMCDXXV          2425
#define MMCDXXVI         2426
#define MMCDXXVII        2427
#define MMCDXXVIII       2428
#define MMCDXXIX         2429
#define MMCDXXX          2430
#define MMCDXXXI         2431
#define MMCDXXXII        2432
#define MMCDXXXIII       2433
#define MMCDXXXIV        2434
#define MMCDXXXV         2435
#define MMCDXXXVI        2436
#define MMCDXXXVII       2437
#define MMCDXXXVIII      2438
#define MMCDXXXIX        2439
#define MMCDXL           2440
#define MMCDXLI          2441
#define MMCDXLII         2442
#define MMCDXLIII        2443
#define MMCDXLIV         2444
#define MMCDXLV          2445
#define MMCDXLVI         2446
#define MMCDXLVII        2447
#define MMCDXLVIII       2448
#define MMCDXLIX         2449
#define MMCDL            2450
#define MMCDLI           2451
#define MMCDLII          2452
#define MMCDLIII         2453
#define MMCDLIV          2454
#define MMCDLV           2455
#define MMCDLVI          2456
#define MMCDLVII         2457
#define MMCDLVIII        2458
#define MMCDLIX          2459
#define MMCDLX           2460
#define MMCDLXI          2461
#define MMCDLXII         2462
#define MMCDLXIII        2463
#define MMCDLXIV         2464
#define MMCDLXV          2465
#define MMCDLXVI         2466
#define MMCDLXVII        2467
#define MMCDLXVIII       2468
#define MMCDLXIX         2469
#define MMCDLXX          2470
#define MMCDLXXI         2471
#define MMCDLXXII        2472
#define MMCDLXXIII       2473
#define MMCDLXXIV        2474
#define MMCDLXXV         2475
#define MMCDLXXVI        2476
#define MMCDLXXVII       2477
#define MMCDLXXVIII      2478
#define MMCDLXXIX        2479
#define MMCDLXXX         2480
#define MMCDLXXXI        2481
#define MMCDLXXXII       2482
#define MMCDLXXXIII      2483
#define MMCDLXXXIV       2484
#define MMCDLXXXV        2485
#define MMCDLXXXVI       2486
#define MMCDLXXXVII      2487
#define MMCDLXXXVIII     2488
#define MMCDLXXXIX       2489
#define MMCDXC           2490
#define MMCDXCI          2491
#define MMCDXCII         2492
#define MMCDXCIII        2493
#define MMCDXCIV         2494
#define MMCDXCV          2495
#define MMCDXCVI         2496
#define MMCDXCVII        2497
#define MMCDXCVIII       2498
#define MMCDXCIX         2499
#define MMD              2500
#define MMDI             2501
#define MMDII            2502
#define MMDIII           2503
#define MMDIV            2504
#define MMDV             2505
#define MMDVI            2506
#define MMDVII           2507
#define MMDVIII          2508
#define MMDIX            2509
#define MMDX             2510
#define MMDXI            2511
#define MMDXII           2512
#define MMDXIII          2513
#define MMDXIV           2514
#define MMDXV            2515
#define MMDXVI           2516
#define MMDXVII          2517
#define MMDXVIII         2518
#define MMDXIX           2519
#define MMDXX            2520
#define MMDXXI           2521
#define MMDXXII          2522
#define MMDXXIII         2523
#define MMDXXIV          2524
#define MMDXXV           2525
#define MMDXXVI          2526
#define MMDXXVII         2527
#define MMDXXVIII        2528
#define MMDXXIX          2529
#define MMDXXX           2530
#define MMDXXXI          2531
#define MMDXXXII         2532
#define MMDXXXIII        2533
#define MMDXXXIV         2534
#define MMDXXXV          2535
#define MMDXXXVI         2536
#define MMDXXXVII        2537
#define MMDXXXVIII       2538
#define MMDXXXIX         2539
#define MMDXL            2540
#define MMDXLI           2541
#define MMDXLII          2542
#define MMDXLIII         2543
#define MMDXLIV          2544
#define MMDXLV           2545
#define MMDXLVI          2546
#define MMDXLVII         2547
#define MMDXLVIII        2548
#define MMDXLIX          2549
#define MMDL             2550
#define MMDLI            2551
#define MMDLII           2552
#define MMDLIII          2553
#define MMDLIV           2554
#define MMDLV            2555
#define MMDLVI           2556
#define MMDLVII          2557
#define MMDLVIII         2558
#define MMDLIX           2559
#define MMDLX            2560
#define MMDLXI           2561
#define MMDLXII          2562
#define MMDLXIII         2563
#define MMDLXIV          2564
#define MMDLXV           2565
#define MMDLXVI          2566
#define MMDLXVII         2567
#define MMDLXVIII        2568
#define MMDLXIX          2569
#define MMDLXX           2570
#define MMDLXXI          2571
#define MMDLXXII         2572
#define MMDLXXIII        2573
#define MMDLXXIV         2574
#define MMDLXXV          2575
#define MMDLXXVI         2576
#define MMDLXXVII        2577
#define MMDLXXVIII       2578
#define MMDLXXIX         2579
#define MMDLXXX          2580
#define MMDLXXXI         2581
#define MMDLXXXII        2582
#define MMDLXXXIII       2583
#define MMDLXXXIV        2584
#define MMDLXXXV         2585
#define MMDLXXXVI        2586
#define MMDLXXXVII       2587
#define MMDLXXXVIII      2588
#define MMDLXXXIX        2589
#define MMDXC            2590
#define MMDXCI           2591
#define MMDXCII          2592
#define MMDXCIII         2593
#define MMDXCIV          2594
#define MMDXCV           2595
#define MMDXCVI          2596
#define MMDXCVII         2597
#define MMDXCVIII        2598
#define MMDXCIX          2599
#define MMDC             2600
#define MMDCI            2601
#define MMDCII           2602
#define MMDCIII          2603
#define MMDCIV           2604
#define MMDCV            2605
#define MMDCVI           2606
#define MMDCVII          2607
#define MMDCVIII         2608
#define MMDCIX           2609
#define MMDCX            2610
#define MMDCXI           2611
#define MMDCXII          2612
#define MMDCXIII         2613
#define MMDCXIV          2614
#define MMDCXV           2615
#define MMDCXVI          2616
#define MMDCXVII         2617
#define MMDCXVIII        2618
#define MMDCXIX          2619
#define MMDCXX           2620
#define MMDCXXI          2621
#define MMDCXXII         2622
#define MMDCXXIII        2623
#define MMDCXXIV         2624
#define MMDCXXV          2625
#define MMDCXXVI         2626
#define MMDCXXVII        2627
#define MMDCXXVIII       2628
#define MMDCXXIX         2629
#define MMDCXXX          2630
#define MMDCXXXI         2631
#define MMDCXXXII        2632
#define MMDCXXXIII       2633
#define MMDCXXXIV        2634
#define MMDCXXXV         2635
#define MMDCXXXVI        2636
#define MMDCXXXVII       2637
#define MMDCXXXVIII      2638
#define MMDCXXXIX        2639
#define MMDCXL           2640
#define MMDCXLI          2641
#define MMDCXLII         2642
#define MMDCXLIII        2643
#define MMDCXLIV         2644
#define MMDCXLV          2645
#define MMDCXLVI         2646
#define MMDCXLVII        2647
#define MMDCXLVIII       2648
#define MMDCXLIX         2649
#define MMDCL            2650
#define MMDCLI           2651
#define MMDCLII          2652
#define MMDCLIII         2653
#define MMDCLIV          2654
#define MMDCLV           2655
#define MMDCLVI          2656
#define MMDCLVII         2657
#define MMDCLVIII        2658
#define MMDCLIX          2659
#define MMDCLX           2660
#define MMDCLXI          2661
#define MMDCLXII         2662
#define MMDCLXIII        2663
#define MMDCLXIV         2664
#define MMDCLXV          2665
#define MMDCLXVI         2666
#define MMDCLXVII        2667
#define MMDCLXVIII       2668
#define MMDCLXIX         2669
#define MMDCLXX          2670
#define MMDCLXXI         2671
#define MMDCLXXII        2672
#define MMDCLXXIII       2673
#define MMDCLXXIV        2674
#define MMDCLXXV         2675
#define MMDCLXXVI        2676
#define MMDCLXXVII       2677
#define MMDCLXXVIII      2678
#define MMDCLXXIX        2679
#define MMDCLXXX         2680
#define MMDCLXXXI        2681
#define MMDCLXXXII       2682
#define MMDCLXXXIII      2683
#define MMDCLXXXIV       2684
#define MMDCLXXXV        2685
#define MMDCLXXXVI       2686
#define MMDCLXXXVII      2687
#define MMDCLXXXVIII     2688
#define MMDCLXXXIX       2689
#define MMDCXC           2690
#define MMDCXCI          2691
#define MMDCXCII         2692
#define MMDCXCIII        2693
#define MMDCXCIV         2694
#define MMDCXCV          2695
#define MMDCXCVI         2696
#define MMDCXCVII        2697
#define MMDCXCVIII       2698
#define MMDCXCIX         2699
#define MMDCC            2700
#define MMDCCI           2701
#define MMDCCII          2702
#define MMDCCIII         2703
#define MMDCCIV          2704
#define MMDCCV           2705
#define MMDCCVI          2706
#define MMDCCVII         2707
#define MMDCCVIII        2708
#define MMDCCIX          2709
#define MMDCCX           2710
#define MMDCCXI          2711
#define MMDCCXII         2712
#define MMDCCXIII        2713
#define MMDCCXIV         2714
#define MMDCCXV          2715
#define MMDCCXVI         2716
#define MMDCCXVII        2717
#define MMDCCXVIII       2718
#define MMDCCXIX         2719
#define MMDCCXX          2720
#define MMDCCXXI         2721
#define MMDCCXXII        2722
#define MMDCCXXIII       2723
#define MMDCCXXIV        2724
#define MMDCCXXV         2725
#define MMDCCXXVI        2726
#define MMDCCXXVII       2727
#define MMDCCXXVIII      2728
#define MMDCCXXIX        2729
#define MMDCCXXX         2730
#define MMDCCXXXI        2731
#define MMDCCXXXII       2732
#define MMDCCXXXIII      2733
#define MMDCCXXXIV       2734
#define MMDCCXXXV        2735
#define MMDCCXXXVI       2736
#define MMDCCXXXVII      2737
#define MMDCCXXXVIII     2738
#define MMDCCXXXIX       2739
#define MMDCCXL          2740
#define MMDCCXLI         2741
#define MMDCCXLII        2742
#define MMDCCXLIII       2743
#define MMDCCXLIV        2744
#define MMDCCXLV         2745
#define MMDCCXLVI        2746
#define MMDCCXLVII       2747
#define MMDCCXLVIII      2748
#define MMDCCXLIX        2749
#define MMDCCL           2750
#define MMDCCLI          2751
#define MMDCCLII         2752
#define MMDCCLIII        2753
#define MMDCCLIV         2754
#define MMDCCLV          2755
#define MMDCCLVI         2756
#define MMDCCLVII        2757
#define MMDCCLVIII       2758
#define MMDCCLIX         2759
#define MMDCCLX          2760
#define MMDCCLXI         2761
#define MMDCCLXII        2762
#define MMDCCLXIII       2763
#define MMDCCLXIV        2764
#define MMDCCLXV         2765
#define MMDCCLXVI        2766
#define MMDCCLXVII       2767
#define MMDCCLXVIII      2768
#define MMDCCLXIX        2769
#define MMDCCLXX         2770
#define MMDCCLXXI        2771
#define MMDCCLXXII       2772
#define MMDCCLXXIII      2773
#define MMDCCLXXIV       2774
#define MMDCCLXXV        2775
#define MMDCCLXXVI       2776
#define MMDCCLXXVII      2777
#define MMDCCLXXVIII     2778
#define MMDCCLXXIX       2779
#define MMDCCLXXX        2780
#define MMDCCLXXXI       2781
#define MMDCCLXXXII      2782
#define MMDCCLXXXIII     2783
#define MMDCCLXXXIV      2784
#define MMDCCLXXXV       2785
#define MMDCCLXXXVI      2786
#define MMDCCLXXXVII     2787
#define MMDCCLXXXVIII    2788
#define MMDCCLXXXIX      2789
#define MMDCCXC          2790
#define MMDCCXCI         2791
#define MMDCCXCII        2792
#define MMDCCXCIII       2793
#define MMDCCXCIV        2794
#define MMDCCXCV         2795
#define MMDCCXCVI        2796
#define MMDCCXCVII       2797
#define MMDCCXCVIII      2798
#define MMDCCXCIX        2799
#define MMDCCC           2800
#define MMDCCCI          2801
#define MMDCCCII         2802
#define MMDCCCIII        2803
#define MMDCCCIV         2804
#define MMDCCCV          2805
#define MMDCCCVI         2806
#define MMDCCCVII        2807
#define MMDCCCVIII       2808
#define MMDCCCIX         2809
#define MMDCCCX          2810
#define MMDCCCXI         2811
#define MMDCCCXII        2812
#define MMDCCCXIII       2813
#define MMDCCCXIV        2814
#define MMDCCCXV         2815
#define MMDCCCXVI        2816
#define MMDCCCXVII       2817
#define MMDCCCXVIII      2818
#define MMDCCCXIX        2819
#define MMDCCCXX         2820
#define MMDCCCXXI        2821
#define MMDCCCXXII       2822
#define MMDCCCXXIII      2823
#define MMDCCCXXIV       2824
#define MMDCCCXXV        2825
#define MMDCCCXXVI       2826
#define MMDCCCXXVII      2827
#define MMDCCCXXVIII     2828
#define MMDCCCXXIX       2829
#define MMDCCCXXX        2830
#define MMDCCCXXXI       2831
#define MMDCCCXXXII      2832
#define MMDCCCXXXIII     2833
#define MMDCCCXXXIV      2834
#define MMDCCCXXXV       2835
#define MMDCCCXXXVI      2836
#define MMDCCCXXXVII     2837
#define MMDCCCXXXVIII    2838
#define MMDCCCXXXIX      2839
#define MMDCCCXL         2840
#define MMDCCCXLI        2841
#define MMDCCCXLII       2842
#define MMDCCCXLIII      2843
#define MMDCCCXLIV       2844
#define MMDCCCXLV        2845
#define MMDCCCXLVI       2846
#define MMDCCCXLVII      2847
#define MMDCCCXLVIII     2848
#define MMDCCCXLIX       2849
#define MMDCCCL          2850
#define MMDCCCLI         2851
#define MMDCCCLII        2852
#define MMDCCCLIII       2853
#define MMDCCCLIV        2854
#define MMDCCCLV         2855
#define MMDCCCLVI        2856
#define MMDCCCLVII       2857
#define MMDCCCLVIII      2858
#define MMDCCCLIX        2859
#define MMDCCCLX         2860
#define MMDCCCLXI        2861
#define MMDCCCLXII       2862
#define MMDCCCLXIII      2863
#define MMDCCCLXIV       2864
#define MMDCCCLXV        2865
#define MMDCCCLXVI       2866
#define MMDCCCLXVII      2867
#define MMDCCCLXVIII     2868
#define MMDCCCLXIX       2869
#define MMDCCCLXX        2870
#define MMDCCCLXXI       2871
#define MMDCCCLXXII      2872
#define MMDCCCLXXIII     2873
#define MMDCCCLXXIV      2874
#define MMDCCCLXXV       2875
#define MMDCCCLXXVI      2876
#define MMDCCCLXXVII     2877
#define MMDCCCLXXVIII    2878
#define MMDCCCLXXIX      2879
#define MMDCCCLXXX       2880
#define MMDCCCLXXXI      2881
#define MMDCCCLXXXII     2882
#define MMDCCCLXXXIII    2883
#define MMDCCCLXXXIV     2884
#define MMDCCCLXXXV      2885
#define MMDCCCLXXXVI     2886
#define MMDCCCLXXXVII    2887
#define MMDCCCLXXXVIII   2888
#define MMDCCCLXXXIX     2889
#define MMDCCCXC         2890
#define MMDCCCXCI        2891
#define MMDCCCXCII       2892
#define MMDCCCXCIII      2893
#define MMDCCCXCIV       2894
#define MMDCCCXCV        2895
#define MMDCCCXCVI       2896
#define MMDCCCXCVII      2897
#define MMDCCCXCVIII     2898
#define MMDCCCXCIX       2899
#define MMCM             2900
#define MMCMI            2901
#define MMCMII           2902
#define MMCMIII          2903
#define MMCMIV           2904
#define MMCMV            2905
#define MMCMVI           2906
#define MMCMVII          2907
#define MMCMVIII         2908
#define MMCMIX           2909
#define MMCMX            2910
#define MMCMXI           2911
#define MMCMXII          2912
#define MMCMXIII         2913
#define MMCMXIV          2914
#define MMCMXV           2915
#define MMCMXVI          2916
#define MMCMXVII         2917
#define MMCMXVIII        2918
#define MMCMXIX          2919
#define MMCMXX           2920
#define MMCMXXI          2921
#define MMCMXXII         2922
#define MMCMXXIII        2923
#define MMCMXXIV         2924
#define MMCMXXV          2925
#define MMCMXXVI         2926
#define MMCMXXVII        2927
#define MMCMXXVIII       2928
#define MMCMXXIX         2929
#define MMCMXXX          2930
#define MMCMXXXI         2931
#define MMCMXXXII        2932
#define MMCMXXXIII       2933
#define MMCMXXXIV        2934
#define MMCMXXXV         2935
#define MMCMXXXVI        2936
#define MMCMXXXVII       2937
#define MMCMXXXVIII      2938
#define MMCMXXXIX        2939
#define MMCMXL           2940
#define MMCMXLI          2941
#define MMCMXLII         2942
#define MMCMXLIII        2943
#define MMCMXLIV         2944
#define MMCMXLV          2945
#define MMCMXLVI         2946
#define MMCMXLVII        2947
#define MMCMXLVIII       2948
#define MMCMXLIX         2949
#define MMCML            2950
#define MMCMLI           2951
#define MMCMLII          2952
#define MMCMLIII         2953
#define MMCMLIV          2954
#define MMCMLV           2955
#define MMCMLVI          2956
#define MMCMLVII         2957
#define MMCMLVIII        2958
#define MMCMLIX          2959
#define MMCMLX           2960
#define MMCMLXI          2961
#define MMCMLXII         2962
#define MMCMLXIII        2963
#define MMCMLXIV         2964
#define MMCMLXV          2965
#define MMCMLXVI         2966
#define MMCMLXVII        2967
#define MMCMLXVIII       2968
#define MMCMLXIX         2969
#define MMCMLXX          2970
#define MMCMLXXI         2971
#define MMCMLXXII        2972
#define MMCMLXXIII       2973
#define MMCMLXXIV        2974
#define MMCMLXXV         2975
#define MMCMLXXVI        2976
#define MMCMLXXVII       2977
#define MMCMLXXVIII      2978
#define MMCMLXXIX        2979
#define MMCMLXXX         2980
#define MMCMLXXXI        2981
#define MMCMLXXXII       2982
#define MMCMLXXXIII      2983
#define MMCMLXXXIV       2984
#define MMCMLXXXV        2985
#define MMCMLXXXVI       2986
#define MMCMLXXXVII      2987
#define MMCMLXXXVIII     2988
#define MMCMLXXXIX       2989
#define MMCMXC           2990
#define MMCMXCI          2991
#define MMCMXCII         2992
#define MMCMXCIII        2993
#define MMCMXCIV         2994
#define MMCMXCV          2995
#define MMCMXCVI         2996
#define MMCMXCVII        2997
#define MMCMXCVIII       2998
#define MMCMXCIX         2999
#define MMM              3000
#define MMMI             3001
#define MMMII            3002
#define MMMIII           3003
#define MMMIV            3004
#define MMMV             3005
#define MMMVI            3006
#define MMMVII           3007
#define MMMVIII          3008
#define MMMIX            3009
#define MMMX             3010
#define MMMXI            3011
#define MMMXII           3012
#define MMMXIII          3013
#define MMMXIV           3014
#define MMMXV            3015
#define MMMXVI           3016
#define MMMXVII          3017
#define MMMXVIII         3018
#define MMMXIX           3019
#define MMMXX            3020
#define MMMXXI           3021
#define MMMXXII          3022
#define MMMXXIII         3023
#define MMMXXIV          3024
#define MMMXXV           3025
#define MMMXXVI          3026
#define MMMXXVII         3027
#define MMMXXVIII        3028
#define MMMXXIX          3029
#define MMMXXX           3030
#define MMMXXXI          3031
#define MMMXXXII         3032
#define MMMXXXIII        3033
#define MMMXXXIV         3034
#define MMMXXXV          3035
#define MMMXXXVI         3036
#define MMMXXXVII        3037
#define MMMXXXVIII       3038
#define MMMXXXIX         3039
#define MMMXL            3040
#define MMMXLI           3041
#define MMMXLII          3042
#define MMMXLIII         3043
#define MMMXLIV          3044
#define MMMXLV           3045
#define MMMXLVI          3046
#define MMMXLVII         3047
#define MMMXLVIII        3048
#define MMMXLIX          3049
#define MMML             3050
#define MMMLI            3051
#define MMMLII           3052
#define MMMLIII          3053
#define MMMLIV           3054
#define MMMLV            3055
#define MMMLVI           3056
#define MMMLVII          3057
#define MMMLVIII         3058
#define MMMLIX           3059
#define MMMLX            3060
#define MMMLXI           3061
#define MMMLXII          3062
#define MMMLXIII         3063
#define MMMLXIV          3064
#define MMMLXV           3065
#define MMMLXVI          3066
#define MMMLXVII         3067
#define MMMLXVIII        3068
#define MMMLXIX          3069
#define MMMLXX           3070
#define MMMLXXI          3071
#define MMMLXXII         3072
#define MMMLXXIII        3073
#define MMMLXXIV         3074
#define MMMLXXV          3075
#define MMMLXXVI         3076
#define MMMLXXVII        3077
#define MMMLXXVIII       3078
#define MMMLXXIX         3079
#define MMMLXXX          3080
#define MMMLXXXI         3081
#define MMMLXXXII        3082
#define MMMLXXXIII       3083
#define MMMLXXXIV        3084
#define MMMLXXXV         3085
#define MMMLXXXVI        3086
#define MMMLXXXVII       3087
#define MMMLXXXVIII      3088
#define MMMLXXXIX        3089
#define MMMXC            3090
#define MMMXCI           3091
#define MMMXCII          3092
#define MMMXCIII         3093
#define MMMXCIV          3094
#define MMMXCV           3095
#define MMMXCVI          3096
#define MMMXCVII         3097
#define MMMXCVIII        3098
#define MMMXCIX          3099
#define MMMC             3100
#define MMMCI            3101
#define MMMCII           3102
#define MMMCIII          3103
#define MMMCIV           3104
#define MMMCV            3105
#define MMMCVI           3106
#define MMMCVII          3107
#define MMMCVIII         3108
#define MMMCIX           3109
#define MMMCX            3110
#define MMMCXI           3111
#define MMMCXII          3112
#define MMMCXIII         3113
#define MMMCXIV          3114
#define MMMCXV           3115
#define MMMCXVI          3116
#define MMMCXVII         3117
#define MMMCXVIII        3118
#define MMMCXIX          3119
#define MMMCXX           3120
#define MMMCXXI          3121
#define MMMCXXII         3122
#define MMMCXXIII        3123
#define MMMCXXIV         3124
#define MMMCXXV          3125
#define MMMCXXVI         3126
#define MMMCXXVII        3127
#define MMMCXXVIII       3128
#define MMMCXXIX         3129
#define MMMCXXX          3130
#define MMMCXXXI         3131
#define MMMCXXXII        3132
#define MMMCXXXIII       3133
#define MMMCXXXIV        3134
#define MMMCXXXV         3135
#define MMMCXXXVI        3136
#define MMMCXXXVII       3137
#define MMMCXXXVIII      3138
#define MMMCXXXIX        3139
#define MMMCXL           3140
#define MMMCXLI          3141
#define MMMCXLII         3142
#define MMMCXLIII        3143
#define MMMCXLIV         3144
#define MMMCXLV          3145
#define MMMCXLVI         3146
#define MMMCXLVII        3147
#define MMMCXLVIII       3148
#define MMMCXLIX         3149
#define MMMCL            3150
#define MMMCLI           3151
#define MMMCLII          3152
#define MMMCLIII         3153
#define MMMCLIV          3154
#define MMMCLV           3155
#define MMMCLVI          3156
#define MMMCLVII         3157
#define MMMCLVIII        3158
#define MMMCLIX          3159
#define MMMCLX           3160
#define MMMCLXI          3161
#define MMMCLXII         3162
#define MMMCLXIII        3163
#define MMMCLXIV         3164
#define MMMCLXV          3165
#define MMMCLXVI         3166
#define MMMCLXVII        3167
#define MMMCLXVIII       3168
#define MMMCLXIX         3169
#define MMMCLXX          3170
#define MMMCLXXI         3171
#define MMMCLXXII        3172
#define MMMCLXXIII       3173
#define MMMCLXXIV        3174
#define MMMCLXXV         3175
#define MMMCLXXVI        3176
#define MMMCLXXVII       3177
#define MMMCLXXVIII      3178
#define MMMCLXXIX        3179
#define MMMCLXXX         3180
#define MMMCLXXXI        3181
#define MMMCLXXXII       3182
#define MMMCLXXXIII      3183
#define MMMCLXXXIV       3184
#define MMMCLXXXV        3185
#define MMMCLXXXVI       3186
#define MMMCLXXXVII      3187
#define MMMCLXXXVIII     3188
#define MMMCLXXXIX       3189
#define MMMCXC           3190
#define MMMCXCI          3191
#define MMMCXCII         3192
#define MMMCXCIII        3193
#define MMMCXCIV         3194
#define MMMCXCV          3195
#define MMMCXCVI         3196
#define MMMCXCVII        3197
#define MMMCXCVIII       3198
#define MMMCXCIX         3199
#define MMMCC            3200
#define MMMCCI           3201
#define MMMCCII          3202
#define MMMCCIII         3203
#define MMMCCIV          3204
#define MMMCCV           3205
#define MMMCCVI          3206
#define MMMCCVII         3207
#define MMMCCVIII        3208
#define MMMCCIX          3209
#define MMMCCX           3210
#define MMMCCXI          3211
#define MMMCCXII         3212
#define MMMCCXIII        3213
#define MMMCCXIV         3214
#define MMMCCXV          3215
#define MMMCCXVI         3216
#define MMMCCXVII        3217
#define MMMCCXVIII       3218
#define MMMCCXIX         3219
#define MMMCCXX          3220
#define MMMCCXXI         3221
#define MMMCCXXII        3222
#define MMMCCXXIII       3223
#define MMMCCXXIV        3224
#define MMMCCXXV         3225
#define MMMCCXXVI        3226
#define MMMCCXXVII       3227
#define MMMCCXXVIII      3228
#define MMMCCXXIX        3229
#define MMMCCXXX         3230
#define MMMCCXXXI        3231
#define MMMCCXXXII       3232
#define MMMCCXXXIII      3233
#define MMMCCXXXIV       3234
#define MMMCCXXXV        3235
#define MMMCCXXXVI       3236
#define MMMCCXXXVII      3237
#define MMMCCXXXVIII     3238
#define MMMCCXXXIX       3239
#define MMMCCXL          3240
#define MMMCCXLI         3241
#define MMMCCXLII        3242
#define MMMCCXLIII       3243
#define MMMCCXLIV        3244
#define MMMCCXLV         3245
#define MMMCCXLVI        3246
#define MMMCCXLVII       3247
#define MMMCCXLVIII      3248
#define MMMCCXLIX        3249
#define MMMCCL           3250
#define MMMCCLI          3251
#define MMMCCLII         3252
#define MMMCCLIII        3253
#define MMMCCLIV         3254
#define MMMCCLV          3255
#define MMMCCLVI         3256
#define MMMCCLVII        3257
#define MMMCCLVIII       3258
#define MMMCCLIX         3259
#define MMMCCLX          3260
#define MMMCCLXI         3261
#define MMMCCLXII        3262
#define MMMCCLXIII       3263
#define MMMCCLXIV        3264
#define MMMCCLXV         3265
#define MMMCCLXVI        3266
#define MMMCCLXVII       3267
#define MMMCCLXVIII      3268
#define MMMCCLXIX        3269
#define MMMCCLXX         3270
#define MMMCCLXXI        3271
#define MMMCCLXXII       3272
#define MMMCCLXXIII      3273
#define MMMCCLXXIV       3274
#define MMMCCLXXV        3275
#define MMMCCLXXVI       3276
#define MMMCCLXXVII      3277
#define MMMCCLXXVIII     3278
#define MMMCCLXXIX       3279
#define MMMCCLXXX        3280
#define MMMCCLXXXI       3281
#define MMMCCLXXXII      3282
#define MMMCCLXXXIII     3283
#define MMMCCLXXXIV      3284
#define MMMCCLXXXV       3285
#define MMMCCLXXXVI      3286
#define MMMCCLXXXVII     3287
#define MMMCCLXXXVIII    3288
#define MMMCCLXXXIX      3289
#define MMMCCXC          3290
#define MMMCCXCI         3291
#define MMMCCXCII        3292
#define MMMCCXCIII       3293
#define MMMCCXCIV        3294
#define MMMCCXCV         3295
#define MMMCCXCVI        3296
#define MMMCCXCVII       3297
#define MMMCCXCVIII      3298
#define MMMCCXCIX        3299
#define MMMCCC           3300
#define MMMCCCI          3301
#define MMMCCCII         3302
#define MMMCCCIII        3303
#define MMMCCCIV         3304
#define MMMCCCV          3305
#define MMMCCCVI         3306
#define MMMCCCVII        3307
#define MMMCCCVIII       3308
#define MMMCCCIX         3309
#define MMMCCCX          3310
#define MMMCCCXI         3311
#define MMMCCCXII        3312
#define MMMCCCXIII       3313
#define MMMCCCXIV        3314
#define MMMCCCXV         3315
#define MMMCCCXVI        3316
#define MMMCCCXVII       3317
#define MMMCCCXVIII      3318
#define MMMCCCXIX        3319
#define MMMCCCXX         3320
#define MMMCCCXXI        3321
#define MMMCCCXXII       3322
#define MMMCCCXXIII      3323
#define MMMCCCXXIV       3324
#define MMMCCCXXV        3325
#define MMMCCCXXVI       3326
#define MMMCCCXXVII      3327
#define MMMCCCXXVIII     3328
#define MMMCCCXXIX       3329
#define MMMCCCXXX        3330
#define MMMCCCXXXI       3331
#define MMMCCCXXXII      3332
#define MMMCCCXXXIII     3333
#define MMMCCCXXXIV      3334
#define MMMCCCXXXV       3335
#define MMMCCCXXXVI      3336
#define MMMCCCXXXVII     3337
#define MMMCCCXXXVIII    3338
#define MMMCCCXXXIX      3339
#define MMMCCCXL         3340
#define MMMCCCXLI        3341
#define MMMCCCXLII       3342
#define MMMCCCXLIII      3343
#define MMMCCCXLIV       3344
#define MMMCCCXLV        3345
#define MMMCCCXLVI       3346
#define MMMCCCXLVII      3347
#define MMMCCCXLVIII     3348
#define MMMCCCXLIX       3349
#define MMMCCCL          3350
#define MMMCCCLI         3351
#define MMMCCCLII        3352
#define MMMCCCLIII       3353
#define MMMCCCLIV        3354
#define MMMCCCLV         3355
#define MMMCCCLVI        3356
#define MMMCCCLVII       3357
#define MMMCCCLVIII      3358
#define MMMCCCLIX        3359
#define MMMCCCLX         3360
#define MMMCCCLXI        3361
#define MMMCCCLXII       3362
#define MMMCCCLXIII      3363
#define MMMCCCLXIV       3364
#define MMMCCCLXV        3365
#define MMMCCCLXVI       3366
#define MMMCCCLXVII      3367
#define MMMCCCLXVIII     3368
#define MMMCCCLXIX       3369
#define MMMCCCLXX        3370
#define MMMCCCLXXI       3371
#define MMMCCCLXXII      3372
#define MMMCCCLXXIII     3373
#define MMMCCCLXXIV      3374
#define MMMCCCLXXV       3375
#define MMMCCCLXXVI      3376
#define MMMCCCLXXVII     3377
#define MMMCCCLXXVIII    3378
#define MMMCCCLXXIX      3379
#define MMMCCCLXXX       3380
#define MMMCCCLXXXI      3381
#define MMMCCCLXXXII     3382
#define MMMCCCLXXXIII    3383
#define MMMCCCLXXXIV     3384
#define MMMCCCLXXXV      3385
#define MMMCCCLXXXVI     3386
#define MMMCCCLXXXVII    3387
#define MMMCCCLXXXVIII   3388
#define MMMCCCLXXXIX     3389
#define MMMCCCXC         3390
#define MMMCCCXCI        3391
#define MMMCCCXCII       3392
#define MMMCCCXCIII      3393
#define MMMCCCXCIV       3394
#define MMMCCCXCV        3395
#define MMMCCCXCVI       3396
#define MMMCCCXCVII      3397
#define MMMCCCXCVIII     3398
#define MMMCCCXCIX       3399
#define MMMCD            3400
#define MMMCDI           3401
#define MMMCDII          3402
#define MMMCDIII         3403
#define MMMCDIV          3404
#define MMMCDV           3405
#define MMMCDVI          3406
#define MMMCDVII         3407
#define MMMCDVIII        3408
#define MMMCDIX          3409
#define MMMCDX           3410
#define MMMCDXI          3411
#define MMMCDXII         3412
#define MMMCDXIII        3413
#define MMMCDXIV         3414
#define MMMCDXV          3415
#define MMMCDXVI         3416
#define MMMCDXVII        3417
#define MMMCDXVIII       3418
#define MMMCDXIX         3419
#define MMMCDXX          3420
#define MMMCDXXI         3421
#define MMMCDXXII        3422
#define MMMCDXXIII       3423
#define MMMCDXXIV        3424
#define MMMCDXXV         3425
#define MMMCDXXVI        3426
#define MMMCDXXVII       3427
#define MMMCDXXVIII      3428
#define MMMCDXXIX        3429
#define MMMCDXXX         3430
#define MMMCDXXXI        3431
#define MMMCDXXXII       3432
#define MMMCDXXXIII      3433
#define MMMCDXXXIV       3434
#define MMMCDXXXV        3435
#define MMMCDXXXVI       3436
#define MMMCDXXXVII      3437
#define MMMCDXXXVIII     3438
#define MMMCDXXXIX       3439
#define MMMCDXL          3440
#define MMMCDXLI         3441
#define MMMCDXLII        3442
#define MMMCDXLIII       3443
#define MMMCDXLIV        3444
#define MMMCDXLV         3445
#define MMMCDXLVI        3446
#define MMMCDXLVII       3447
#define MMMCDXLVIII      3448
#define MMMCDXLIX        3449
#define MMMCDL           3450
#define MMMCDLI          3451
#define MMMCDLII         3452
#define MMMCDLIII        3453
#define MMMCDLIV         3454
#define MMMCDLV          3455
#define MMMCDLVI         3456
#define MMMCDLVII        3457
#define MMMCDLVIII       3458
#define MMMCDLIX         3459
#define MMMCDLX          3460
#define MMMCDLXI         3461
#define MMMCDLXII        3462
#define MMMCDLXIII       3463
#define MMMCDLXIV        3464
#define MMMCDLXV         3465
#define MMMCDLXVI        3466
#define MMMCDLXVII       3467
#define MMMCDLXVIII      3468
#define MMMCDLXIX        3469
#define MMMCDLXX         3470
#define MMMCDLXXI        3471
#define MMMCDLXXII       3472
#define MMMCDLXXIII      3473
#define MMMCDLXXIV       3474
#define MMMCDLXXV        3475
#define MMMCDLXXVI       3476
#define MMMCDLXXVII      3477
#define MMMCDLXXVIII     3478
#define MMMCDLXXIX       3479
#define MMMCDLXXX        3480
#define MMMCDLXXXI       3481
#define MMMCDLXXXII      3482
#define MMMCDLXXXIII     3483
#define MMMCDLXXXIV      3484
#define MMMCDLXXXV       3485
#define MMMCDLXXXVI      3486
#define MMMCDLXXXVII     3487
#define MMMCDLXXXVIII    3488
#define MMMCDLXXXIX      3489
#define MMMCDXC          3490
#define MMMCDXCI         3491
#define MMMCDXCII        3492
#define MMMCDXCIII       3493
#define MMMCDXCIV        3494
#define MMMCDXCV         3495
#define MMMCDXCVI        3496
#define MMMCDXCVII       3497
#define MMMCDXCVIII      3498
#define MMMCDXCIX        3499
#define MMMD             3500
#define MMMDI            3501
#define MMMDII           3502
#define MMMDIII          3503
#define MMMDIV           3504
#define MMMDV            3505
#define MMMDVI           3506
#define MMMDVII          3507
#define MMMDVIII         3508
#define MMMDIX           3509
#define MMMDX            3510
#define MMMDXI           3511
#define MMMDXII          3512
#define MMMDXIII         3513
#define MMMDXIV          3514
#define MMMDXV           3515
#define MMMDXVI          3516
#define MMMDXVII         3517
#define MMMDXVIII        3518
#define MMMDXIX          3519
#define MMMDXX           3520
#define MMMDXXI          3521
#define MMMDXXII         3522
#define MMMDXXIII        3523
#define MMMDXXIV         3524
#define MMMDXXV          3525
#define MMMDXXVI         3526
#define MMMDXXVII        3527
#define MMMDXXVIII       3528
#define MMMDXXIX         3529
#define MMMDXXX          3530
#define MMMDXXXI         3531
#define MMMDXXXII        3532
#define MMMDXXXIII       3533
#define MMMDXXXIV        3534
#define MMMDXXXV         3535
#define MMMDXXXVI        3536
#define MMMDXXXVII       3537
#define MMMDXXXVIII      3538
#define MMMDXXXIX        3539
#define MMMDXL           3540
#define MMMDXLI          3541
#define MMMDXLII         3542
#define MMMDXLIII        3543
#define MMMDXLIV         3544
#define MMMDXLV          3545
#define MMMDXLVI         3546
#define MMMDXLVII        3547
#define MMMDXLVIII       3548
#define MMMDXLIX         3549
#define MMMDL            3550
#define MMMDLI           3551
#define MMMDLII          3552
#define MMMDLIII         3553
#define MMMDLIV          3554
#define MMMDLV           3555
#define MMMDLVI          3556
#define MMMDLVII         3557
#define MMMDLVIII        3558
#define MMMDLIX          3559
#define MMMDLX           3560
#define MMMDLXI          3561
#define MMMDLXII         3562
#define MMMDLXIII        3563
#define MMMDLXIV         3564
#define MMMDLXV          3565
#define MMMDLXVI         3566
#define MMMDLXVII        3567
#define MMMDLXVIII       3568
#define MMMDLXIX         3569
#define MMMDLXX          3570
#define MMMDLXXI         3571
#define MMMDLXXII        3572
#define MMMDLXXIII       3573
#define MMMDLXXIV        3574
#define MMMDLXXV         3575
#define MMMDLXXVI        3576
#define MMMDLXXVII       3577
#define MMMDLXXVIII      3578
#define MMMDLXXIX        3579
#define MMMDLXXX         3580
#define MMMDLXXXI        3581
#define MMMDLXXXII       3582
#define MMMDLXXXIII      3583
#define MMMDLXXXIV       3584
#define MMMDLXXXV        3585
#define MMMDLXXXVI       3586
#define MMMDLXXXVII      3587
#define MMMDLXXXVIII     3588
#define MMMDLXXXIX       3589
#define MMMDXC           3590
#define MMMDXCI          3591
#define MMMDXCII         3592
#define MMMDXCIII        3593
#define MMMDXCIV         3594
#define MMMDXCV          3595
#define MMMDXCVI         3596
#define MMMDXCVII        3597
#define MMMDXCVIII       3598
#define MMMDXCIX         3599
#define MMMDC            3600
#define MMMDCI           3601
#define MMMDCII          3602
#define MMMDCIII         3603
#define MMMDCIV          3604
#define MMMDCV           3605
#define MMMDCVI          3606
#define MMMDCVII         3607
#define MMMDCVIII        3608
#define MMMDCIX          3609
#define MMMDCX           3610
#define MMMDCXI          3611
#define MMMDCXII         3612
#define MMMDCXIII        3613
#define MMMDCXIV         3614
#define MMMDCXV          3615
#define MMMDCXVI         3616
#define MMMDCXVII        3617
#define MMMDCXVIII       3618
#define MMMDCXIX         3619
#define MMMDCXX          3620
#define MMMDCXXI         3621
#define MMMDCXXII        3622
#define MMMDCXXIII       3623
#define MMMDCXXIV        3624
#define MMMDCXXV         3625
#define MMMDCXXVI        3626
#define MMMDCXXVII       3627
#define MMMDCXXVIII      3628
#define MMMDCXXIX        3629
#define MMMDCXXX         3630
#define MMMDCXXXI        3631
#define MMMDCXXXII       3632
#define MMMDCXXXIII      3633
#define MMMDCXXXIV       3634
#define MMMDCXXXV        3635
#define MMMDCXXXVI       3636
#define MMMDCXXXVII      3637
#define MMMDCXXXVIII     3638
#define MMMDCXXXIX       3639
#define MMMDCXL          3640
#define MMMDCXLI         3641
#define MMMDCXLII        3642
#define MMMDCXLIII       3643
#define MMMDCXLIV        3644
#define MMMDCXLV         3645
#define MMMDCXLVI        3646
#define MMMDCXLVII       3647
#define MMMDCXLVIII      3648
#define MMMDCXLIX        3649
#define MMMDCL           3650
#define MMMDCLI          3651
#define MMMDCLII         3652
#define MMMDCLIII        3653
#define MMMDCLIV         3654
#define MMMDCLV          3655
#define MMMDCLVI         3656
#define MMMDCLVII        3657
#define MMMDCLVIII       3658
#define MMMDCLIX         3659
#define MMMDCLX          3660
#define MMMDCLXI         3661
#define MMMDCLXII        3662
#define MMMDCLXIII       3663
#define MMMDCLXIV        3664
#define MMMDCLXV         3665
#define MMMDCLXVI        3666
#define MMMDCLXVII       3667
#define MMMDCLXVIII      3668
#define MMMDCLXIX        3669
#define MMMDCLXX         3670
#define MMMDCLXXI        3671
#define MMMDCLXXII       3672
#define MMMDCLXXIII      3673
#define MMMDCLXXIV       3674
#define MMMDCLXXV        3675
#define MMMDCLXXVI       3676
#define MMMDCLXXVII      3677
#define MMMDCLXXVIII     3678
#define MMMDCLXXIX       3679
#define MMMDCLXXX        3680
#define MMMDCLXXXI       3681
#define MMMDCLXXXII      3682
#define MMMDCLXXXIII     3683
#define MMMDCLXXXIV      3684
#define MMMDCLXXXV       3685
#define MMMDCLXXXVI      3686
#define MMMDCLXXXVII     3687
#define MMMDCLXXXVIII    3688
#define MMMDCLXXXIX      3689
#define MMMDCXC          3690
#define MMMDCXCI         3691
#define MMMDCXCII        3692
#define MMMDCXCIII       3693
#define MMMDCXCIV        3694
#define MMMDCXCV         3695
#define MMMDCXCVI        3696
#define MMMDCXCVII       3697
#define MMMDCXCVIII      3698
#define MMMDCXCIX        3699
#define MMMDCC           3700
#define MMMDCCI          3701
#define MMMDCCII         3702
#define MMMDCCIII        3703
#define MMMDCCIV         3704
#define MMMDCCV          3705
#define MMMDCCVI         3706
#define MMMDCCVII        3707
#define MMMDCCVIII       3708
#define MMMDCCIX         3709
#define MMMDCCX          3710
#define MMMDCCXI         3711
#define MMMDCCXII        3712
#define MMMDCCXIII       3713
#define MMMDCCXIV        3714
#define MMMDCCXV         3715
#define MMMDCCXVI        3716
#define MMMDCCXVII       3717
#define MMMDCCXVIII      3718
#define MMMDCCXIX        3719
#define MMMDCCXX         3720
#define MMMDCCXXI        3721
#define MMMDCCXXII       3722
#define MMMDCCXXIII      3723
#define MMMDCCXXIV       3724
#define MMMDCCXXV        3725
#define MMMDCCXXVI       3726
#define MMMDCCXXVII      3727
#define MMMDCCXXVIII     3728
#define MMMDCCXXIX       3729
#define MMMDCCXXX        3730
#define MMMDCCXXXI       3731
#define MMMDCCXXXII      3732
#define MMMDCCXXXIII     3733
#define MMMDCCXXXIV      3734
#define MMMDCCXXXV       3735
#define MMMDCCXXXVI      3736
#define MMMDCCXXXVII     3737
#define MMMDCCXXXVIII    3738
#define MMMDCCXXXIX      3739
#define MMMDCCXL         3740
#define MMMDCCXLI        3741
#define MMMDCCXLII       3742
#define MMMDCCXLIII      3743
#define MMMDCCXLIV       3744
#define MMMDCCXLV        3745
#define MMMDCCXLVI       3746
#define MMMDCCXLVII      3747
#define MMMDCCXLVIII     3748
#define MMMDCCXLIX       3749
#define MMMDCCL          3750
#define MMMDCCLI         3751
#define MMMDCCLII        3752
#define MMMDCCLIII       3753
#define MMMDCCLIV        3754
#define MMMDCCLV         3755
#define MMMDCCLVI        3756
#define MMMDCCLVII       3757
#define MMMDCCLVIII      3758
#define MMMDCCLIX        3759
#define MMMDCCLX         3760
#define MMMDCCLXI        3761
#define MMMDCCLXII       3762
#define MMMDCCLXIII      3763
#define MMMDCCLXIV       3764
#define MMMDCCLXV        3765
#define MMMDCCLXVI       3766
#define MMMDCCLXVII      3767
#define MMMDCCLXVIII     3768
#define MMMDCCLXIX       3769
#define MMMDCCLXX        3770
#define MMMDCCLXXI       3771
#define MMMDCCLXXII      3772
#define MMMDCCLXXIII     3773
#define MMMDCCLXXIV      3774
#define MMMDCCLXXV       3775
#define MMMDCCLXXVI      3776
#define MMMDCCLXXVII     3777
#define MMMDCCLXXVIII    3778
#define MMMDCCLXXIX      3779
#define MMMDCCLXXX       3780
#define MMMDCCLXXXI      3781
#define MMMDCCLXXXII     3782
#define MMMDCCLXXXIII    3783
#define MMMDCCLXXXIV     3784
#define MMMDCCLXXXV      3785
#define MMMDCCLXXXVI     3786
#define MMMDCCLXXXVII    3787
#define MMMDCCLXXXVIII   3788
#define MMMDCCLXXXIX     3789
#define MMMDCCXC         3790
#define MMMDCCXCI        3791
#define MMMDCCXCII       3792
#define MMMDCCXCIII      3793
#define MMMDCCXCIV       3794
#define MMMDCCXCV        3795
#define MMMDCCXCVI       3796
#define MMMDCCXCVII      3797
#define MMMDCCXCVIII     3798
#define MMMDCCXCIX       3799
#define MMMDCCC          3800
#define MMMDCCCI         3801
#define MMMDCCCII        3802
#define MMMDCCCIII       3803
#define MMMDCCCIV        3804
#define MMMDCCCV         3805
#define MMMDCCCVI        3806
#define MMMDCCCVII       3807
#define MMMDCCCVIII      3808
#define MMMDCCCIX        3809
#define MMMDCCCX         3810
#define MMMDCCCXI        3811
#define MMMDCCCXII       3812
#define MMMDCCCXIII      3813
#define MMMDCCCXIV       3814
#define MMMDCCCXV        3815
#define MMMDCCCXVI       3816
#define MMMDCCCXVII      3817
#define MMMDCCCXVIII     3818
#define MMMDCCCXIX       3819
#define MMMDCCCXX        3820
#define MMMDCCCXXI       3821
#define MMMDCCCXXII      3822
#define MMMDCCCXXIII     3823
#define MMMDCCCXXIV      3824
#define MMMDCCCXXV       3825
#define MMMDCCCXXVI      3826
#define MMMDCCCXXVII     3827
#define MMMDCCCXXVIII    3828
#define MMMDCCCXXIX      3829
#define MMMDCCCXXX       3830
#define MMMDCCCXXXI      3831
#define MMMDCCCXXXII     3832
#define MMMDCCCXXXIII    3833
#define MMMDCCCXXXIV     3834
#define MMMDCCCXXXV      3835
#define MMMDCCCXXXVI     3836
#define MMMDCCCXXXVII    3837
#define MMMDCCCXXXVIII   3838
#define MMMDCCCXXXIX     3839
#define MMMDCCCXL        3840
#define MMMDCCCXLI       3841
#define MMMDCCCXLII      3842
#define MMMDCCCXLIII     3843
#define MMMDCCCXLIV      3844
#define MMMDCCCXLV       3845
#define MMMDCCCXLVI      3846
#define MMMDCCCXLVII     3847
#define MMMDCCCXLVIII    3848
#define MMMDCCCXLIX      3849
#define MMMDCCCL         3850
#define MMMDCCCLI        3851
#define MMMDCCCLII       3852
#define MMMDCCCLIII      3853
#define MMMDCCCLIV       3854
#define MMMDCCCLV        3855
#define MMMDCCCLVI       3856
#define MMMDCCCLVII      3857
#define MMMDCCCLVIII     3858
#define MMMDCCCLIX       3859
#define MMMDCCCLX        3860
#define MMMDCCCLXI       3861
#define MMMDCCCLXII      3862
#define MMMDCCCLXIII     3863
#define MMMDCCCLXIV      3864
#define MMMDCCCLXV       3865
#define MMMDCCCLXVI      3866
#define MMMDCCCLXVII     3867
#define MMMDCCCLXVIII    3868
#define MMMDCCCLXIX      3869
#define MMMDCCCLXX       3870
#define MMMDCCCLXXI      3871
#define MMMDCCCLXXII     3872
#define MMMDCCCLXXIII    3873
#define MMMDCCCLXXIV     3874
#define MMMDCCCLXXV      3875
#define MMMDCCCLXXVI     3876
#define MMMDCCCLXXVII    3877
#define MMMDCCCLXXVIII   3878
#define MMMDCCCLXXIX     3879
#define MMMDCCCLXXX      3880
#define MMMDCCCLXXXI     3881
#define MMMDCCCLXXXII    3882
#define MMMDCCCLXXXIII   3883
#define MMMDCCCLXXXIV    3884
#define MMMDCCCLXXXV     3885
#define MMMDCCCLXXXVI    3886
#define MMMDCCCLXXXVII   3887
#define MMMDCCCLXXXVIII  3888
#define MMMDCCCLXXXIX    3889
#define MMMDCCCXC        3890
#define MMMDCCCXCI       3891
#define MMMDCCCXCII      3892
#define MMMDCCCXCIII     3893
#define MMMDCCCXCIV      3894
#define MMMDCCCXCV       3895
#define MMMDCCCXCVI      3896
#define MMMDCCCXCVII     3897
#define MMMDCCCXCVIII    3898
#define MMMDCCCXCIX      3899
#define MMMCM            3900
#define MMMCMI           3901
#define MMMCMII          3902
#define MMMCMIII         3903
#define MMMCMIV          3904
#define MMMCMV           3905
#define MMMCMVI          3906
#define MMMCMVII         3907
#define MMMCMVIII        3908
#define MMMCMIX          3909
#define MMMCMX           3910
#define MMMCMXI          3911
#define MMMCMXII         3912
#define MMMCMXIII        3913
#define MMMCMXIV         3914
#define MMMCMXV          3915
#define MMMCMXVI         3916
#define MMMCMXVII        3917
#define MMMCMXVIII       3918
#define MMMCMXIX         3919
#define MMMCMXX          3920
#define MMMCMXXI         3921
#define MMMCMXXII        3922
#define MMMCMXXIII       3923
#define MMMCMXXIV        3924
#define MMMCMXXV         3925
#define MMMCMXXVI        3926
#define MMMCMXXVII       3927
#define MMMCMXXVIII      3928
#define MMMCMXXIX        3929
#define MMMCMXXX         3930
#define MMMCMXXXI        3931
#define MMMCMXXXII       3932
#define MMMCMXXXIII      3933
#define MMMCMXXXIV       3934
#define MMMCMXXXV        3935
#define MMMCMXXXVI       3936
#define MMMCMXXXVII      3937
#define MMMCMXXXVIII     3938
#define MMMCMXXXIX       3939
#define MMMCMXL          3940
#define MMMCMXLI         3941
#define MMMCMXLII        3942
#define MMMCMXLIII       3943
#define MMMCMXLIV        3944
#define MMMCMXLV         3945
#define MMMCMXLVI        3946
#define MMMCMXLVII       3947
#define MMMCMXLVIII      3948
#define MMMCMXLIX        3949
#define MMMCML           3950
#define MMMCMLI          3951
#define MMMCMLII         3952
#define MMMCMLIII        3953
#define MMMCMLIV         3954
#define MMMCMLV          3955
#define MMMCMLVI         3956
#define MMMCMLVII        3957
#define MMMCMLVIII       3958
#define MMMCMLIX         3959
#define MMMCMLX          3960
#define MMMCMLXI         3961
#define MMMCMLXII        3962
#define MMMCMLXIII       3963
#define MMMCMLXIV        3964
#define MMMCMLXV         3965
#define MMMCMLXVI        3966
#define MMMCMLXVII       3967
#define MMMCMLXVIII      3968
#define MMMCMLXIX        3969
#define MMMCMLXX         3970
#define MMMCMLXXI        3971
#define MMMCMLXXII       3972
#define MMMCMLXXIII      3973
#define MMMCMLXXIV       3974
#define MMMCMLXXV        3975
#define MMMCMLXXVI       3976
#define MMMCMLXXVII      3977
#define MMMCMLXXVIII     3978
#define MMMCMLXXIX       3979
#define MMMCMLXXX        3980
#define MMMCMLXXXI       3981
#define MMMCMLXXXII      3982
#define MMMCMLXXXIII     3983
#define MMMCMLXXXIV      3984
#define MMMCMLXXXV       3985
#define MMMCMLXXXVI      3986
#define MMMCMLXXXVII     3987
#define MMMCMLXXXVIII    3988
#define MMMCMLXXXIX      3989
#define MMMCMXC          3990
#define MMMCMXCI         3991
#define MMMCMXCII        3992
#define MMMCMXCIII       3993
#define MMMCMXCIV        3994
#define MMMCMXCV         3995
#define MMMCMXCVI        3996
#define MMMCMXCVII       3997
#define MMMCMXCVIII      3998
#define MMMCMXCIX        3999
/* finis numerorum generatorum */

#define imprimere     printf
#define liberare         free
#define memoriae_allocare    malloc
#define exire                exit

#define interior         static
#define hic_manens     static
#define universalis static

#define FILUM FILE

nomen insignatus character    i8;
nomen insignatus brevis         i16;
nomen insignatus integer       i32;
nomen insignatus longus longus    i64;

nomen signatus character    s8;
nomen signatus brevis            s16;
nomen signatus integer         s32;
nomen signatus longus longus    s64;

nomen fluitans                  f32;
nomen duplex                         f64;

nomen integer                    b32;

nomen size_t                                 memoriae_index;

#endif /* LATINA_H */
#line 1 "include/surdus.h"
/* surdus.h - Surdi: summae radicum quadratarum, signum EXACTUM, celeriter
 *
 * "Surdus": radix irrationalis (interpretes Latini al-Khwarizmi, 'asamm
 * = mutus; Anglice surd). Hic x = sum_S c_S sqrt(prod_{i in S} p_i),
 * S selectio primorum p_1..p_k distinctorum (k <= 3), c_S integri s64.
 * Typus valoris: nulla piscina, nulla allocatio.
 *
 * SIGNUM per tres gradus interiores, omnes EXACTI:
 *   1. nullum: x == 0 sse omnes coefficientes 0 (basis super Q
 *      linearis independens) - gratis;
 *   2. filtrum duplex certificatum: |fl(x)| > limes erroris -> signum
 *      eius (limes demonstratus in lib/surdus.c; contractio FMA limitem
 *      non frangit, rotundationes tantum minuit);
 *   3. quadratio recursiva in s64 custodito: x = a + b sqrt(p),
 *      signum(x) = signum(a) signum(a^2 - p b^2) ubi a, b signis
 *      oppositis.
 * RECUSAT (FALSUM), numquam coniectat: arithmetica quae s64 excederet,
 * et gradus 3 super x minimo non nullo cuius quadrata excederent. Tum
 * vocans ad extensio (magni) recurrit. Rationales: vocans ad integros
 * scalat (D119: comparatio per VIII multiplicata).
 *
 * USUS:
 *   s32 primi[III] = { II, III, V };
 *   SurdiSpatium sp;
 *   Surdus x, y;
 *   s32 s;
 *   si (surdi_spatium(primi, III, &sp)
 *       && surdus_basis(&sp, I, LXX, &x)            70 sqrt2
 *       && surdus_subtrahe(surdus_ex_s64(XCIX), x, &y)
 *       && surdus_signum(&sp, y, &s))                99 - 70 sqrt2 > 0
 *       ...
 *
 * Vide lib/surdus.worklog.md.
 */
/* <aedilis corpus="lib/surdus.c"/> */
#ifndef SURDUS_H
#define SURDUS_H



#define SURDUS_RADICES_MAXIMAE  III


/* ==================================================
 * Spatium (corpus Q(sqrt p_1, ..., sqrt p_k))
 * ================================================== */

/* Membra a surdi_spatium ponuntur, a vocante non mutanda (producta et
 * radices praecomputatae: ansa calida signi et multiplicationis). */
nomen structura {
    s32 primi[III];       /* primi distincti, ordine crescente */
    s32 numerus;          /* k = 0..3 */
    s64 producta[VIII];   /* producta[S] = prod_{i in S} p_i (< 2^45) */
    f64 radices[VIII];    /* fl(sqrt(producta[S])) */
} SurdiSpatium;

/* FALSUM (exitus non tangitur): numerus > 3 aut < 0, primus non primus
 * aut >= 2^15, ordo non stricte crescens */
b32
surdi_spatium (
    constans          s32* primi,
                      s32  numerus,
             SurdiSpatium* exitus);


/* ==================================================
 * Surdus
 * ================================================== */

/* c[S], S mascula bitorum super primos (bitum i <-> primi[i]) */
nomen structura {
    s64 c[VIII];
} Surdus;

Surdus
surdus_ex_s64 (
    s64 x);

/* c sqrt(prod primorum in 'selectio'); FALSUM si selectio >= 2^k */
b32
surdus_basis (
    constans SurdiSpatium* sp,
                      i32  selectio,
                      s64  c,
                   Surdus* exitus);

b32
surdus_est_nullum (
    Surdus x);


/* ==================================================
 * Arithmetica custodita: FALSUM si s64 excederetur (exitus non
 * tangitur). Coefficientes extra basin spatii (S >= 2^k) nulli esse
 * debent: multiplica, signum et compara eos RECUSANT (FALSUM);
 * adde/subtrahe/scala spatium non vident et eos tantum transferunt.
 * ================================================== */

b32
surdus_adde (
    Surdus  a,
    Surdus  b,
    Surdus* exitus);

b32
surdus_subtrahe (
    Surdus  a,
    Surdus  b,
    Surdus* exitus);

b32
surdus_scala (
    Surdus  a,
       s64  c,
    Surdus* exitus);

b32
surdus_multiplica (
    constans SurdiSpatium* sp,
                   Surdus  a,
                   Surdus  b,
                   Surdus* exitus);


/* ==================================================
 * Signum
 * ================================================== */

/* -1, 0, +1 EXACTE; FALSUM (exitus non tangitur) solum si gradus 3
 * excederet aut coefficiens extra basin non nullus */
b32
surdus_signum (
    constans SurdiSpatium* sp,
                   Surdus  x,
                      s32* exitus);

/* signum(a - b); FALSUM si differentia aut signum excederet */
b32
surdus_compara (
    constans SurdiSpatium* sp,
                   Surdus  a,
                   Surdus  b,
                      s32* exitus);

#endif /* SURDUS_H */
#line 1 "include/surdus_interna.h"
/* surdus_interna.h - INTERNA surdi: signum cum gradu decernente. NON
 * API - solum lib/surdus.c et probationes id includunt; consumptores
 * surdus.h solum. Praefixum 'surdi_' (genetivus) ab API 'surdus_'
 * distinguit (surdi_spatium publicum excipitur: genetivus naturalis).
 *
 * Probationes gradum 3 (quadratio) directe contra oraculum agere debent,
 * non sperare ut inputa casu eo perveniant: filtrum = FALSUM gradum 2
 * omittit. */

#ifndef SURDUS_INTERNA_H
#define SURDUS_INTERNA_H



/* gradus decernens: I nullum aut rationale, II filtrum duplex, III
 * quadratio (maximus per recursionem). Contractus signi idem ac
 * surdus_signum; gradus NIHIL licet. */
b32
surdi_signum_gradu (
    constans SurdiSpatium* sp,
                   Surdus  x,
                      b32  filtrum,
                      s32* exitus,
                      s32* gradus);

#endif /* SURDUS_INTERNA_H */
/* lib/surdus.c: statica per plagulam renominata */
#define _adde_tuta _adde_tuta_surdus
#define _bita _bita_surdus
#define _bita_maxima _bita_maxima_surdus
#define _est_primus _est_primus_surdus
#define _extra_basin _extra_basin_surdus
#define _magnitudo _magnitudo_surdus
#define _multiplica_k _multiplica_k_surdus
#define _multiplica_tuta _multiplica_tuta_surdus
#define _signum_k _signum_k_surdus
#define _subtrahe_tuta _subtrahe_tuta_surdus
#line 1 "lib/surdus.c"
/* surdus.c - Surdi: summae radicum quadratarum, signum exactum
 *
 * BASIS: S mascula super primos (bitum i <-> primi[i]); sqrt(P_S) sqrt(P_T)
 * = P_{S & T} sqrt(P_{S ^ T}), ergo productum: c[S ^ T] += a[S] b[T]
 * producta[S & T].
 *
 * CUSTODIA s64: ante multiplicationem bita maxima computantur; si
 * bA + bB + bP + k <= 63 (|termini| < 2^(bA+bB+bP), 2^k termini per
 * coefficientem) via celeris sine custodia, aliter per operationem
 * custodita (divisio) et FALSUM in excessu.
 *
 * FILTRUM (gradus 2), u = 2^-53, n = 2^k <= 8 termini:
 *   terminus t_S = fl(fl(c_S) r_S), r_S = fl(sqrt(P_S)) (P_S < 2^45
 *   exacte repraesentabile, sqrt IEEE recte rotundata): tres
 *   rotundationes, |t_S - c_S sqrt(P_S)| <= 3.01 u |c_S sqrt(P_S)|;
 *   summa recursiva n terminorum: <= (n-1) 1.01 u sum|t_S|.
 *   Ergo |fl(x) - x| <= (n + 3) u M, M = sum|t_S| (fere). Limes adhibitus
 *   E = 2 (n + 4) u M: factor II M ipsum rotundatum et ordinis secundi
 *   terminos tegit. |fl(x)| > E -> signum exactum. FMA (contractio)
 *   rotundationem unam tollit, numquam addit: limes manet.
 *   Valores integri: nullus subfluxus; |c| < 2^63, r < 2^23: nullus
 *   superfluxus.
 *
 * QUADRATIO (gradus 3): x = a + b sqrt(p_{k-1}), a, b in spatio k-1;
 * signa a, b recursive (cum filtro); opposita -> signum(a) signum(a^2 -
 * p b^2) (numquam 0: aliter sqrt(p) in spatio minore esset).
 * Vide lib/surdus.worklog.md.
 */


#include <math.h>

#define S64_MAXIMUS  0x7FFFFFFFFFFFFFFFLL
#define S64_MINIMUS  (-S64_MAXIMUS - 1LL)
#define UNITAS_ROTUNDATIONIS  1.1102230246251565e-16   /* 2^-53 */


/* ==================================================
 * Auxilia
 * ================================================== */

interior i64
_magnitudo (
    s64 x)
{
    redde x < 0 ? (i64)0 - (i64)x : (i64)x;
}

/* numerus bitorum: 0 -> 0, 1 -> 1, 2^62 -> 63 */
interior s32
_bita (
    i64 x)
{
    s32 n = ZEPHYRUM;

    si (x >> XXXII)
    {
        n += XXXII;
        x >>= XXXII;
    }
    si (x >> XVI)
    {
        n += XVI;
        x >>= XVI;
    }
    si (x >> VIII)
    {
        n += VIII;
        x >>= VIII;
    }
    si (x >> IV)
    {
        n += IV;
        x >>= IV;
    }
    dum (x)
    {
        n++;
        x >>= I;
    }
    redde n;
}

interior b32
_adde_tuta (
    s64  a,
    s64  b,
    s64* r)
{
    si (   (b > 0 && a > S64_MAXIMUS - b)
        || (b < 0 && a < S64_MINIMUS - b))
    {
        redde FALSUM;
    }
    *r = a + b;
    redde VERUM;
}

/* a - b sine negatione: -(-2^63) indefinitum esset (comportamentum
 * indefinitum quod nulla probatio certe capit - planta 'custodia -2^63
 * omissa' superstes erat, compilatore casu recte agente) */
interior b32
_subtrahe_tuta (
    s64  a,
    s64  b,
    s64* r)
{
    si (   (b < 0 && a > S64_MAXIMUS + b)
        || (b > 0 && a < S64_MINIMUS + b))
    {
        redde FALSUM;
    }
    *r = a - b;
    redde VERUM;
}

/* |a b| <= 2^63 - 1, aliter FALSUM (etiam -2^63 exactum recusatur) */
interior b32
_multiplica_tuta (
    s64  a,
    s64  b,
    s64* r)
{
    i64 ma = _magnitudo(a);
    i64 mb = _magnitudo(b);

    si (ma == 0 || mb == 0)
    {
        *r = ZEPHYRUM;
        redde VERUM;
    }
    si (ma > (i64)S64_MAXIMUS / mb)
    {
        redde FALSUM;
    }
    *r = a * b;
    redde VERUM;
}

interior s32
_bita_maxima (
    constans Surdus* x,
                s32  n)
{
    i64 maxima = 0;
    s32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        i64 m = _magnitudo(x->c[i]);

        si (m > maxima)
        {
            maxima = m;
        }
    }
    redde _bita(maxima);
}

/* productum in spatio primorum k priorum */
interior b32
_multiplica_k (
    constans SurdiSpatium* sp,
    constans       Surdus* a,
    constans       Surdus* b,
                      s32  k,
                   Surdus* exitus)
{
       s32 n = (s32)I << k;
    Surdus r;
       s32 s;
       s32 t;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        r.c[s] = ZEPHYRUM;
    }
    si (_bita_maxima(a, n) + _bita_maxima(b, n)
            + _bita((i64)sp->producta[n - I]) + k <= LXIII)
    {
        /* via celeris: nullus excessus possibilis */
        per (s = ZEPHYRUM; s < n; s++)
        {
            si (a->c[s] == 0)
            {
                perge;
            }
            per (t = ZEPHYRUM; t < n; t++)
            {
                r.c[s ^ t] += a->c[s] * b->c[t] * sp->producta[s & t];
            }
        }
    }
    alioquin
    {
        per (s = ZEPHYRUM; s < n; s++)
        {
            per (t = ZEPHYRUM; t < n; t++)
            {
                s64 terminus;

                si (   !_multiplica_tuta(a->c[s], b->c[t], &terminus)
                    || !_multiplica_tuta(terminus, sp->producta[s & t],
                        &terminus)
                    || !_adde_tuta(r.c[s ^ t], terminus, &r.c[s ^ t]))
                {
                    redde FALSUM;
                }
            }
        }
    }
    *exitus = r;
    redde VERUM;
}


/* ==================================================
 * Spatium
 * ================================================== */

interior b32
_est_primus (
    s32 p)
{
    s32 d;

    si (p < II)
    {
        redde FALSUM;
    }
    /* d <= p/d: d*d numquam formatur (p prope 2^31 excederet s32) */
    per (d = II; d <= p / d; d++)
    {
        si (p % d == 0)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
surdi_spatium (
    constans          s32* primi,
                      s32  numerus,
             SurdiSpatium* exitus)
{
    SurdiSpatium sp;
             s32 i;
             s32 s;

    si (numerus < 0 || numerus > SURDUS_RADICES_MAXIMAE)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (   primi[i] >= 32768 || !_est_primus(primi[i])
            || (i > 0 && primi[i] <= primi[i - I]))
        {
            redde FALSUM;
        }
        sp.primi[i] = primi[i];
    }
    per (; i < III; i++)
    {
        sp.primi[i] = ZEPHYRUM;
    }
    sp.numerus = numerus;
    per (s = ZEPHYRUM; s < VIII; s++)
    {
        s64 productum = I;

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            si (s & (I << i))
            {
                productum *= sp.primi[i];
            }
        }
        sp.producta[s] = s < ((s32)I << numerus) ? productum : ZEPHYRUM;
        sp.radices[s] = sqrt((f64)sp.producta[s]);
    }
    *exitus = sp;
    redde VERUM;
}


/* ==================================================
 * Surdus
 * ================================================== */

Surdus
surdus_ex_s64 (
    s64 x)
{
    Surdus r;
       s32 s;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        r.c[s] = ZEPHYRUM;
    }
    r.c[ZEPHYRUM] = x;
    redde r;
}

b32
surdus_basis (
    constans SurdiSpatium* sp,
                      i32  selectio,
                      s64  c,
                   Surdus* exitus)
{
    si (selectio >= ((i32)I << sp->numerus))
    {
        redde FALSUM;
    }
    *exitus              = surdus_ex_s64(ZEPHYRUM);
    exitus->c[selectio]  = c;
    redde VERUM;
}

b32
surdus_est_nullum (
    Surdus x)
{
    s32 s;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        si (x.c[s] != 0)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
surdus_adde (
    Surdus  a,
    Surdus  b,
    Surdus* exitus)
{
    Surdus r;
       s32 s;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        si (!_adde_tuta(a.c[s], b.c[s], &r.c[s]))
        {
            redde FALSUM;
        }
    }
    *exitus = r;
    redde VERUM;
}

b32
surdus_subtrahe (
    Surdus  a,
    Surdus  b,
    Surdus* exitus)
{
    Surdus r;
       s32 s;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        si (!_subtrahe_tuta(a.c[s], b.c[s], &r.c[s]))
        {
            redde FALSUM;
        }
    }
    *exitus = r;
    redde VERUM;
}

b32
surdus_scala (
    Surdus  a,
       s64  c,
    Surdus* exitus)
{
    Surdus r;
       s32 s;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        si (!_multiplica_tuta(a.c[s], c, &r.c[s]))
        {
            redde FALSUM;
        }
    }
    *exitus = r;
    redde VERUM;
}

/* coefficiens extra basin spatii (S >= 2^k) non nullus? Tales
 * recusantur: aliter signum et productum eos ut 0 legerent, est_nullum
 * ut non nullos (recensio 2026-10-09, S3) */
interior b32
_extra_basin (
    constans SurdiSpatium* sp,
    constans       Surdus* x)
{
    s32 s;

    per (s = (s32)I << sp->numerus; s < VIII; s++)
    {
        si (x->c[s] != 0)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
surdus_multiplica (
    constans SurdiSpatium* sp,
                   Surdus  a,
                   Surdus  b,
                   Surdus* exitus)
{
    si (_extra_basin(sp, &a) || _extra_basin(sp, &b))
    {
        redde FALSUM;
    }
    redde _multiplica_k(sp, &a, &b, sp->numerus, exitus);
}


/* ==================================================
 * Signum
 * ================================================== */

interior b32
_signum_k (
    constans SurdiSpatium* sp,
    constans       Surdus* x,
                      s32  k,
                      b32  filtrum,
                      s32* signum,
                      s32* gradus)
{
       s32 n = (s32)I << k;
       s32 s;
       s32 sa;
       s32 sb;
       s32 sd;
       s32 ga = I;
       s32 gb = I;
       s32 gd = I;
    Surdus a;
    Surdus b;
    Surdus d;

    /* gradus 1: nullum, aut rationale */
    per (s = I; s < n; s++)
    {
        si (x->c[s] != 0)
        {
            frange;
        }
    }
    si (s == n)
    {
        *signum = (x->c[ZEPHYRUM] > 0) - (x->c[ZEPHYRUM] < 0);
        *gradus = I;
        redde VERUM;
    }
    /* gradus 2: filtrum duplex certificatum */
    si (filtrum)
    {
        f64 summa = 0.0;
        f64 moles = 0.0;

        per (s = ZEPHYRUM; s < n; s++)
        {
            f64 terminus = (f64)x->c[s] * sp->radices[s];

            summa += terminus;
            moles += fabs(terminus);
        }
        si (fabs(summa) > 2.0 * (f64)(n
            + IV) * UNITAS_ROTUNDATIONIS * moles)
        {
            *signum = summa > 0.0 ? I : -(s32)I;
            *gradus = II;
            redde VERUM;
        }
    }
    /* gradus 3: x = a + b sqrt(p_{k-1}) */
    per (s = ZEPHYRUM; s < VIII; s++)
    {
        a.c[s] = ZEPHYRUM;
        b.c[s] = ZEPHYRUM;
    }
    per (s = ZEPHYRUM; s < n / II; s++)
    {
        a.c[s] = x->c[s];
        b.c[s] = x->c[s + n / II];
    }
    si (   !_signum_k(sp, &a, k - I, filtrum, &sa, &ga)
        || !_signum_k(sp, &b, k - I, filtrum, &sb, &gb))
    {
        redde FALSUM;
    }
    *gradus = ga > gb ? ga : gb;
    si (sb == 0 || sa == sb)
    {
        *signum = sa;
        redde VERUM;
    }
    si (sa == 0)
    {
        *signum = sb;
        redde VERUM;
    }
    /* signa opposita: signum(a) signum(a^2 - p b^2) */
    si (   !_multiplica_k(sp, &a, &a, k - I, &a)
        || !_multiplica_k(sp, &b, &b, k - I, &b)
        || !surdus_scala(b, (s64)sp->primi[k - I], &b)
        || !surdus_subtrahe(a, b, &d)
        || !_signum_k(sp, &d, k - I, filtrum, &sd, &gd))
    {
        redde FALSUM;
    }
    *signum = sa * sd;
    *gradus = III;
    redde VERUM;
}

b32
surdi_signum_gradu (
    constans SurdiSpatium* sp,
                   Surdus  x,
                      b32  filtrum,
                      s32* exitus,
                      s32* gradus)
{
    s32 signum;
    s32 g;

    si (   _extra_basin(sp, &x)
        || !_signum_k(sp, &x, sp->numerus, filtrum, &signum, &g))
    {
        redde FALSUM;
    }
    *exitus = signum;
    si (gradus)
    {
        *gradus = g;
    }
    redde VERUM;
}

b32
surdus_signum (
    constans SurdiSpatium* sp,
                   Surdus  x,
                      s32* exitus)
{
    redde surdi_signum_gradu(sp, x, VERUM, exitus, NIHIL);
}

b32
surdus_compara (
    constans SurdiSpatium* sp,
                   Surdus  a,
                   Surdus  b,
                      s32* exitus)
{
    Surdus d;

    si (!surdus_subtrahe(a, b, &d))
    {
        redde FALSUM;
    }
    redde surdus_signum(sp, d, exitus);
}
#undef S64_MAXIMUS
#undef S64_MINIMUS
#undef UNITAS_ROTUNDATIONIS
#undef _adde_tuta
#undef _bita
#undef _bita_maxima
#undef _est_primus
#undef _extra_basin
#undef _magnitudo
#undef _multiplica_k
#undef _multiplica_tuta
#undef _signum_k
#undef _subtrahe_tuta
#line 1 "knotapel/demo_122_cell_b_phase_diagram/main.c"
/*
 * KNOTAPEL DEMO 122: Cell B's Exact Phase Diagram
 * ================================================================
 *
 * D97 asked why D96's Cell B (the six 45-degree elements of zeta_8,
 * rotations about three orthogonal axes) computes XOR for every set,
 * and answered with an angle sweep: the same three axes at half-angles
 * 10, 15, ..., 90 degrees, capacity at N = 3..6. It reported a plateau
 * from 25 to 75 degrees, "45 is the midpoint", an "isolated resonance"
 * at 35 degrees (N = 5), and "total collapse" at 90.
 *
 * The swept family is exact in a way the sweep does not use. Each
 * element is (cos t, +-sin t d_j); a signed sum is (n0 cos t, sin t
 * sum_j n_j d_j) with SMALL INTEGERS n0, n_j fixed by the mask. So:
 *   - the axis cell does not depend on t: largest |n_j|, ties exactly
 *     when maxima are equal, the rule taking the first in D97's order;
 *   - n0 = 0, n != 0: angle exactly 180 degrees - a tie at m = k/2;
 *   - n = 0: real sum (sector 0 or k-1), or zero;
 *   - otherwise the sector depends on u = tan^2 t only, and changes
 *     exactly at u* = n0^2 tan^2(j pi/24) / r, r = |n|^2, j = 1..11
 *     (every sector boundary of k = 6, 12, 24 is a multiple of pi/24).
 *
 * This demo sorts every breakpoint exactly (surdus: tan^2(j pi/24) =
 * (4 - 4 cos(j pi/12)) / (4 + 4 cos(j pi/12)) in Z[sqrt2, sqrt3]),
 * then evaluates rule, robust and possible on every open interval and
 * at every breakpoint by RANK - u's position among the sorted
 * breakpoints answers every sector question. The result is Cell B's
 * capacity for every half-angle in (0, 90] degrees, with exact
 * endpoints; D97's 21 angles are placed into it (multiples of 15
 * degrees exactly, the others by a certified float gap) and its
 * float sweep is reproduced first.
 *
 * Mode: DEMO122_CELER = the replica and N = 3 only (plants).
 *
 * House library: surdus.h. Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_122_cell_b_phase_diagram/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0;
static int n_fail = 0;

static void
check (
    const char *msg,
    int         ok)
{
    if (ok) {
        n_pass++;
        printf("  PASS: %s\n", msg);
    } else {
        n_fail++;
        printf("  FAIL: %s\n", msg);
    }
}


static int quick = 0;

/* ================================================================
 * D95/D96's group (D121's port of D95's integer code)
 * ================================================================ */

typedef struct { int a; int b; } Zr2;

static Zr2 zr2_make (int a, int b) { Zr2 r; r.a = a; r.b = b; return r; }
static Zr2 zr2_add (Zr2 x, Zr2 y)
{ Zr2 r; r.a = x.a + y.a; r.b = x.b + y.b; return r; }
static Zr2 zr2_sub (Zr2 x, Zr2 y)
{ Zr2 r; r.a = x.a - y.a; r.b = x.b - y.b; return r; }
static Zr2 zr2_neg (Zr2 x) { Zr2 r; r.a = -x.a; r.b = -x.b; return r; }
static Zr2 zr2_mul (Zr2 x, Zr2 y)
{
    Zr2 r;
    r.a = x.a * y.a + 2 * x.b * y.b;
    r.b = x.a * y.b + x.b * y.a;
    return r;
}
static int zr2_eq (Zr2 x, Zr2 y) { return x.a == y.a && x.b == y.b; }
static Zr2 zr2_div2 (Zr2 x) { Zr2 r; r.a = x.a / 2; r.b = x.b / 2; return r; }

typedef struct { Zr2 a, b, c, d; } QZ8;

static QZ8 qz8_make (Zr2 a, Zr2 b, Zr2 c, Zr2 d)
{ QZ8 r; r.a = a; r.b = b; r.c = c; r.d = d; return r; }
static int qz8_eq (const QZ8 *p, const QZ8 *q)
{
    return zr2_eq(p->a, q->a) && zr2_eq(p->b, q->b)
        && zr2_eq(p->c, q->c) && zr2_eq(p->d, q->d);
}
static QZ8 qz8_neg (const QZ8 *q)
{ return qz8_make(zr2_neg(q->a), zr2_neg(q->b), zr2_neg(q->c), zr2_neg(q->d)); }
static QZ8 qz8_conj (const QZ8 *q)
{ return qz8_make(q->a, zr2_neg(q->b), zr2_neg(q->c), zr2_neg(q->d)); }

static QZ8
qz8_mul (
    const QZ8 *p,
    const QZ8 *q)
{
    QZ8 r;
    Zr2 t;

    t = zr2_mul(p->a, q->a);
    t = zr2_sub(t, zr2_mul(p->b, q->b));
    t = zr2_sub(t, zr2_mul(p->c, q->c));
    t = zr2_sub(t, zr2_mul(p->d, q->d));
    r.a = zr2_div2(t);
    t = zr2_mul(p->a, q->b);
    t = zr2_add(t, zr2_mul(p->b, q->a));
    t = zr2_add(t, zr2_mul(p->c, q->d));
    t = zr2_sub(t, zr2_mul(p->d, q->c));
    r.b = zr2_div2(t);
    t = zr2_mul(p->a, q->c);
    t = zr2_sub(t, zr2_mul(p->b, q->d));
    t = zr2_add(t, zr2_mul(p->c, q->a));
    t = zr2_add(t, zr2_mul(p->d, q->b));
    r.c = zr2_div2(t);
    t = zr2_mul(p->a, q->d);
    t = zr2_add(t, zr2_mul(p->b, q->c));
    t = zr2_sub(t, zr2_mul(p->c, q->b));
    t = zr2_add(t, zr2_mul(p->d, q->a));
    r.d = zr2_div2(t);
    return r;
}

#define MAX_GRP 64

static QZ8 g_z8[MAX_GRP];
static int g_z8_size = 0;
static int g_level[MAX_GRP];      /* derived level 0..3 */
static int g_null[MAX_GRP];       /* Re = 0 */
static int g_cell[MAX_GRP];       /* A..E = 0..4 */
static int g_in_g1[MAX_GRP];      /* commutator subgroup */
static int g_single_comm[MAX_GRP];

static int
find_z8 (
    const QZ8 *q)
{
    int i;
    QZ8 nq = qz8_neg(q);

    for (i = 0; i < g_z8_size; i++) {
        if (qz8_eq(q, &g_z8[i]) || qz8_eq(&nq, &g_z8[i])) {
            return i;
        }
    }
    return -1;
}

static void
build_z8_d95 (void)
{
    QZ8 gens[4];
    int prev;
    int i;
    int gi;
    int rd;

    gens[0] = qz8_make(zr2_make(0,1), zr2_make(0,1), zr2_make(0,0),
        zr2_make(0,0));
    gens[1] = qz8_conj(&gens[0]);
    gens[2] = qz8_make(zr2_make(0,1), zr2_make(0,0), zr2_make(0,0),
        zr2_make(0,-1));
    gens[3] = qz8_conj(&gens[2]);
    g_z8[0] = qz8_make(zr2_make(2,0), zr2_make(0,0), zr2_make(0,0),
        zr2_make(0,0));
    g_z8_size = 1;
    for (gi = 0; gi < 4; gi++) {
        if (find_z8(&gens[gi]) < 0 && g_z8_size < MAX_GRP) {
            g_z8[g_z8_size++] = gens[gi];
        }
    }
    rd = 1;
    do {
        prev = g_z8_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                QZ8 prod = qz8_mul(&g_z8[i], &gens[gi]);

                if (find_z8(&prod) < 0 && g_z8_size < MAX_GRP) {
                    g_z8[g_z8_size++] = prod;
                }
            }
        }
        rd++;
    } while (g_z8_size > prev && rd < 20);
}

static QZ8
z8_commutator (
    const QZ8 *a,
    const QZ8 *b)
{
    QZ8 ai = qz8_conj(a);
    QZ8 bi = qz8_conj(b);
    QZ8 ab = qz8_mul(a, b);
    QZ8 abi = qz8_mul(&ab, &ai);

    return qz8_mul(&abi, &bi);
}

static int
close_subgroup (
    int *in_set)
{
    int changed;
    int i;
    int j;
    int count = 0;

    in_set[0] = 1;
    do {
        changed = 0;
        for (i = 0; i < g_z8_size; i++) {
            QZ8 inv;
            int k;

            if (!in_set[i]) continue;
            inv = qz8_conj(&g_z8[i]);
            k = find_z8(&inv);
            if (k >= 0 && !in_set[k]) { in_set[k] = 1; changed = 1; }
            for (j = 0; j < g_z8_size; j++) {
                QZ8 prod;

                if (!in_set[j]) continue;
                prod = qz8_mul(&g_z8[i], &g_z8[j]);
                k = find_z8(&prod);
                if (k >= 0 && !in_set[k]) { in_set[k] = 1; changed = 1; }
            }
        }
    } while (changed);
    for (i = 0; i < g_z8_size; i++) {
        count += in_set[i];
    }
    return count;
}

/* D95 Phase 1-2 / D96 cells; sizes[] = derived series */
static int
derive_structure (
    int *sizes)
{
    int level_set[MAX_GRP];
    int comm_set[MAX_GRP];
    int level;
    int n_levels = 0;
    int i;
    int j;

    memset(g_single_comm, 0, sizeof(g_single_comm));
    for (i = 0; i < g_z8_size; i++) {
        for (j = 0; j < g_z8_size; j++) {
            QZ8 cm = z8_commutator(&g_z8[i], &g_z8[j]);
            int k = find_z8(&cm);

            if (k >= 0) g_single_comm[k] = 1;
        }
    }
    memcpy(g_in_g1, g_single_comm, sizeof(g_in_g1));
    (void)close_subgroup(g_in_g1);
    for (i = 0; i < g_z8_size; i++) {
        level_set[i] = 1;
        g_level[i] = 0;
    }
    sizes[0] = g_z8_size;
    for (level = 1; level < 10; level++) {
        int cur;

        memset(comm_set, 0, sizeof(comm_set));
        for (i = 0; i < g_z8_size; i++) {
            if (!level_set[i]) continue;
            for (j = 0; j < g_z8_size; j++) {
                QZ8 cm;
                int k;

                if (!level_set[j]) continue;
                cm = z8_commutator(&g_z8[i], &g_z8[j]);
                k = find_z8(&cm);
                if (k >= 0) comm_set[k] = 1;
            }
        }
        cur = close_subgroup(comm_set);
        sizes[level] = cur;
        for (i = 0; i < g_z8_size; i++) {
            if (level_set[i] && !comm_set[i]) g_level[i] = level - 1;
        }
        memcpy(level_set, comm_set, sizeof(level_set));
        n_levels = level;
        if (cur <= 1) {
            for (i = 0; i < g_z8_size; i++) {
                if (comm_set[i]) g_level[i] = level;
            }
            break;
        }
    }
    for (i = 0; i < g_z8_size; i++) {
        g_null[i] = g_z8[i].a.a == 0 && g_z8[i].a.b == 0;
        g_cell[i] = g_level[i] == 0 ? (g_null[i] ? 0 : 1)
            : g_level[i] == 1 ? 2 : g_level[i] == 2 ? 3 : 4;
    }
    return n_levels;
}


/* ================================================================
 * D97's float sweep, verbatim up to names (latina.h: no 'si')
 * ================================================================ */

typedef struct { double a, b, c, d; } Quat;

#define MAX_DIR 64
#define MAX_ACT 65536

static double g_dir[MAX_DIR][3];
static int    g_nd = 0;
static Quat   g_cat[128];
static int    g_cat_size = 0;
static int    cell_class0[MAX_ACT];
static int    cell_class1[MAX_ACT];
static int    touched_cells[MAX_ACT];
static Quat   cellb_float[8];
static int    cellb_count = 0;
static double cellb_dirs[3][3];
static int    cellb_n_dirs = 0;

static Quat
qz8_to_quat (
    const QZ8 *q)
{
    static const double SQRT2 = 1.4142135623730950488;
    Quat r;

    r.a = ((double)q->a.a + (double)q->a.b * SQRT2) / 2.0;
    r.b = ((double)q->b.a + (double)q->b.b * SQRT2) / 2.0;
    r.c = ((double)q->c.a + (double)q->c.b * SQRT2) / 2.0;
    r.d = ((double)q->d.a + (double)q->d.b * SQRT2) / 2.0;
    return r;
}

/* D97 extract_cell_b + Phase 1 direction extraction */
static void
d97_cell_b (void)
{
    int i;
    int j;

    cellb_count = 0;
    for (i = 0; i < g_z8_size; i++) {
        if (g_level[i] == 0 && !g_null[i]) {
            cellb_float[cellb_count++] = qz8_to_quat(&g_z8[i]);
        }
    }
    cellb_n_dirs = 0;
    for (i = 0; i < cellb_count; i++) {
        double qa = cellb_float[i].a, qb = cellb_float[i].b;
        double qc = cellb_float[i].c, qd = cellb_float[i].d;
        double nv, ax, ay, az;
        int    found = 0;

        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        if (nv < 1e-12) continue;
        ax = qb/nv; ay = qc/nv; az = qd/nv;
        for (j = 0; j < cellb_n_dirs; j++) {
            double d1 = fabs(cellb_dirs[j][0]-ax) + fabs(cellb_dirs[j][1]-ay)
                + fabs(cellb_dirs[j][2]-az);
            double d2 = fabs(cellb_dirs[j][0]+ax) + fabs(cellb_dirs[j][1]+ay)
                + fabs(cellb_dirs[j][2]+az);

            if (d1 < 1e-8 || d2 < 1e-8) { found = 1; break; }
        }
        if (!found && cellb_n_dirs < 3) {
            cellb_dirs[cellb_n_dirs][0] = ax;
            cellb_dirs[cellb_n_dirs][1] = ay;
            cellb_dirs[cellb_n_dirs][2] = az;
            cellb_n_dirs++;
        }
    }
}

static void
d97_build_dirs (void)
{
    int i;
    int j;

    g_nd = 0;
    for (i = 0; i < g_cat_size; i++) {
        double qa = g_cat[i].a, qb = g_cat[i].b;
        double qc = g_cat[i].c, qd = g_cat[i].d;
        double nv, ax, ay, az;
        int    found = 0;

        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        if (nv < 1e-12) continue;
        ax = qb/nv; ay = qc/nv; az = qd/nv;
        for (j = 0; j < g_nd; j++) {
            double d1 = fabs(g_dir[j][0]-ax) + fabs(g_dir[j][1]-ay)
                + fabs(g_dir[j][2]-az);
            double d2 = fabs(g_dir[j][0]+ax) + fabs(g_dir[j][1]+ay)
                + fabs(g_dir[j][2]+az);

            if (d1 < 1e-8 || d2 < 1e-8) { found = 1; break; }
        }
        if (!found && g_nd < MAX_DIR) {
            g_dir[g_nd][0] = ax; g_dir[g_nd][1] = ay; g_dir[g_nd][2] = az;
            g_nd++;
        }
    }
}

static int
d97_vor_cell (
    double ax,
    double ay,
    double az)
{
    int    i;
    int    best = 0;
    double bd = -2.0;

    for (i = 0; i < g_nd; i++) {
        double dp = fabs(ax*g_dir[i][0] + ay*g_dir[i][1] + az*g_dir[i][2]);

        if (dp > bd) { bd = dp; best = i; }
    }
    return best;
}

static int
d97_phase_cell (
    double sa,
    double sb,
    double sc,
    double sd,
    int    k_sec)
{
    double n2 = sa*sa + sb*sb + sc*sc + sd*sd;
    double nm, qa, half_ang, ang, rv;
    int    sec, vor, n_vor;

    n_vor = g_nd + 1;
    if (n2 < 1e-24) return (k_sec - 1) * n_vor + g_nd;
    nm = sqrt(n2);
    qa = sa / nm;
    if (qa > 1.0) qa = 1.0;
    if (qa < -1.0) qa = -1.0;
    half_ang = acos(qa);
    ang = 2.0 * half_ang * 180.0 / M_PI;
    sec = (int)(ang * (double)k_sec / 360.0);
    if (sec >= k_sec) sec = k_sec - 1;
    if (sec < 0) sec = 0;
    rv = sqrt(sb*sb + sc*sc + sd*sd);
    if (rv / nm < 1e-12) { vor = g_nd; }
    else { vor = d97_vor_cell(sb / rv, sc / rv, sd / rv); }
    return sec * n_vor + vor;
}

static int
popcount (
    int x)
{
    int c = 0;

    while (x) { c += x & 1; x >>= 1; }
    return c;
}

static int
d97_test_xor (
    const int *indices,
    int        n_weights,
    int        k_sec)
{
    int n_masks = 1 << n_weights;
    int n_touched = 0;
    int mask;
    int i;
    int result = 1;

    for (mask = 0; mask < n_masks && result; mask++) {
        double sa = 0, sb = 0, sc = 0, sd = 0;
        int    cls;
        int    cell;

        for (i = 0; i < n_weights; i++) {
            const Quat *q = &g_cat[indices[i]];
            double sign = ((mask >> i) & 1) ? 1.0 : -1.0;

            sa += sign * q->a; sb += sign * q->b;
            sc += sign * q->c; sd += sign * q->d;
        }
        cell = d97_phase_cell(sa, sb, sc, sd, k_sec);
        cls = popcount(mask) & 1;
        if (cell_class0[cell] == 0 && cell_class1[cell] == 0) {
            touched_cells[n_touched++] = cell;
        }
        if (cls == 0) {
            cell_class0[cell]++;
            if (cell_class1[cell] > 0) result = 0;
        } else {
            cell_class1[cell]++;
            if (cell_class0[cell] > 0) result = 0;
        }
    }
    for (i = 0; i < n_touched; i++) {
        cell_class0[touched_cells[i]] = 0;
        cell_class1[touched_cells[i]] = 0;
    }
    return result;
}

static int
next_combo (
    int *combo,
    int  n,
    int  bf)
{
    int i = n - 1;

    while (i >= 0) {
        combo[i]++;
        if (combo[i] <= bf - n + i) {
            int j;

            for (j = i + 1; j < n; j++) combo[j] = combo[j - 1] + 1;
            return 1;
        }
        i--;
    }
    return 0;
}

static const int KS[3] = { 6, 12, 24 };

/* D97 Phase 3 at one half-angle (degrees): XOR count for N */
static int
d97_sweep_count (
    double degrees,
    int    n_w)
{
    double ha = degrees * M_PI / 180.0;
    double co = cos(ha);
    double sn = sin(ha);
    int    combo[8];
    int    count = 0;
    int    di;
    int    i;

    g_cat_size = 0;
    for (di = 0; di < cellb_n_dirs; di++) {
        Quat q1, q2;

        q1.a = co;
        q1.b = sn * cellb_dirs[di][0];
        q1.c = sn * cellb_dirs[di][1];
        q1.d = sn * cellb_dirs[di][2];
        q2.a = co;
        q2.b = -sn * cellb_dirs[di][0];
        q2.c = -sn * cellb_dirs[di][1];
        q2.d = -sn * cellb_dirs[di][2];
        g_cat[g_cat_size++] = q1;
        g_cat[g_cat_size++] = q2;
    }
    d97_build_dirs();
    for (i = 0; i < n_w; i++) combo[i] = i;
    do {
        int ki;

        for (ki = 0; ki < 3; ki++) {
            if (d97_test_xor(combo, n_w, KS[ki])) { count++; break; }
        }
    } while (next_combo(combo, n_w, g_cat_size));
    return count;
}

/* ================================================================
 * Verdicts (D119-D121's engine: rule, robust, possible by DPLL)
 * ================================================================ */

#define MAX_TIES 64
#define MAX_MASKS 256
#define MAX_CELLS 65536

typedef struct {
    int float_cell;
    int exact_cell;
    int n_ties;
    int ties[MAX_TIES];
} MaskCell;

static unsigned char seen0[MAX_CELLS];
static unsigned char seen1[MAX_CELLS];

typedef struct {
    int v_float;
    int v_rule;
    int v_robust;
    int v_possible;
    int undecided;
    int tied;
    int clash;
} Verdict;

static int
labels_pass (
    const MaskCell *mc,
    int             n_masks,
    const int      *tt,
    int             use_float)
{
    int touched[MAX_MASKS];
    int nt = 0;
    int m;
    int ok = 1;

    for (m = 0; m < n_masks && ok; m++) {
        int cell = use_float ? mc[m].float_cell : mc[m].exact_cell;

        if (!seen0[cell] && !seen1[cell]) {
            touched[nt++] = cell;
        }
        if (tt[m]) {
            seen1[cell] = 1;
            ok = !seen0[cell];
        } else {
            seen0[cell] = 1;
            ok = !seen1[cell];
        }
    }
    for (m = 0; m < nt; m++) {
        seen0[touched[m]] = 0;
        seen1[touched[m]] = 0;
    }
    return ok;
}

static int
robust_pass (
    const MaskCell *mc,
    int             n_masks,
    const int      *tt)
{
    static int touched[MAX_MASKS * MAX_TIES];
    int nt = 0;
    int m;
    int k;
    int ok = 1;

    for (m = 0; m < n_masks && ok; m++) {
        for (k = 0; k < mc[m].n_ties && ok; k++) {
            int cell = mc[m].ties[k];

            if (!seen0[cell] && !seen1[cell]) {
                touched[nt++] = cell;
            }
            if (tt[m]) {
                seen1[cell] = 1;
                ok = !seen0[cell];
            } else {
                seen0[cell] = 1;
                ok = !seen1[cell];
            }
        }
    }
    for (m = 0; m < nt; m++) {
        seen0[touched[m]] = 0;
        seen1[touched[m]] = 0;
    }
    return ok;
}

/* some resolution passes <=> SAT: variable x_c = label of cell c; a
 * mask with truth value 1 needs (OR of x_c over its tied cells), one
 * with value 0 needs (OR of NOT x_c). DPLL with unit propagation;
 * decision budget -> undecided (counted, never guessed). */
#define MAX_VARS (MAX_MASKS * MAX_TIES)

static int  sat_var_of[MAX_CELLS];      /* cell -> var + 1 (0 = none) */
static int  sat_cells[MAX_VARS];
static int  sat_nv;
static int  sat_val[MAX_VARS];          /* -1 unassigned, 0, 1 */
static int  sat_trail[MAX_VARS];
static int  sat_nt;
static long sat_decisions;
static const MaskCell *sat_mc;
static const int      *sat_tt;
static int             sat_n;

/* returns 0 on conflict */
static int
sat_propagate (void)
{
    int changed = 1;

    while (changed) {
        int m;

        changed = 0;
        for (m = 0; m < sat_n; m++) {
            int want = sat_tt[m] ? 1 : 0;
            int free_var = -1;
            int n_free = 0;
            int sat = 0;
            int j;

            for (j = 0; j < sat_mc[m].n_ties && !sat; j++) {
                int v = sat_var_of[sat_mc[m].ties[j]] - 1;

                if (sat_val[v] == want) {
                    sat = 1;
                } else if (sat_val[v] < 0) {
                    if (free_var != v) {
                        n_free++;
                    }
                    free_var = v;
                }
            }
            if (sat) {
                continue;
            }
            if (n_free == 0) {
                return 0;
            }
            if (n_free == 1) {
                sat_val[free_var] = want;
                sat_trail[sat_nt++] = free_var;
                changed = 1;
            }
        }
    }
    return 1;
}

static int
sat_solve (void)
{
    int mark = sat_nt;
    int m;
    int pick = -1;
    int pick_val = 0;
    int r;

    if (!sat_propagate()) {
        goto fail;
    }
    /* first unsatisfied clause: branch on its first free variable */
    for (m = 0; m < sat_n && pick < 0; m++) {
        int want = sat_tt[m] ? 1 : 0;
        int sat = 0;
        int j;
        int cand = -1;

        for (j = 0; j < sat_mc[m].n_ties; j++) {
            int v = sat_var_of[sat_mc[m].ties[j]] - 1;

            if (sat_val[v] == want) {
                sat = 1;
                break;
            }
            if (sat_val[v] < 0 && cand < 0) {
                cand = v;
            }
        }
        if (!sat) {
            pick = cand;
            pick_val = want;
        }
    }
    if (pick < 0) {
        return 1;                    /* every clause satisfied */
    }
    if (++sat_decisions > 200000L) {
        r = -1;
        goto out;
    }
    sat_val[pick] = pick_val;
    sat_trail[sat_nt++] = pick;
    r = sat_solve();
    if (r != 0) {
        goto out;
    }
    sat_val[pick] = 1 - pick_val;
    r = sat_solve();
    if (r != 0) {
        goto out;
    }
fail:
    r = 0;
out:
    if (r != 1) {
        while (sat_nt > mark) {
            sat_val[sat_trail[--sat_nt]] = -1;
        }
    }
    return r;
}

static int
possible_pass (
    const MaskCell *mc,
    int             n_masks,
    const int      *tt,
    int            *undecided)
{
    int m;
    int j;
    int r;

    sat_nv = 0;
    for (m = 0; m < n_masks; m++) {
        for (j = 0; j < mc[m].n_ties; j++) {
            int cell = mc[m].ties[j];

            if (sat_var_of[cell] == 0) {
                sat_cells[sat_nv] = cell;
                sat_val[sat_nv] = -1;
                sat_var_of[cell] = ++sat_nv;
            }
        }
    }
    sat_mc = mc;
    sat_tt = tt;
    sat_n = n_masks;
    sat_nt = 0;
    sat_decisions = 0;
    r = sat_solve();
    for (j = 0; j < sat_nv; j++) {
        sat_var_of[sat_cells[j]] = 0;
    }
    if (r < 0) {
        (*undecided)++;
        return 0;
    }
    return r;
}


/* ================================================================
 * The exact family: entry e = (cos t, eps_e sin t d_{dir_e}),
 * dir_e = e/2, eps_e = +1 (even e) or -1 (odd e) - D97's sweep order;
 * axis j of the activation = d_j (D97's build_dirs over the sweep)
 * ================================================================ */

#define N_AXES 3
#define N_VOR  4                   /* 3 axes + 'no axis' */

static SurdiSpatium spatium;

/* 4 cos(j pi/12) in Z[sqrt2, sqrt3] (D120): bit 1 sqrt2, 2 sqrt3, 3 sqrt6 */
static Surdus
cos4_surdus (
    int j)
{
    Surdus r = surdus_ex_s64(0);
    int    neg = 0;
    int    b;

    j %= 24;
    if (j < 0) j += 24;
    if (j > 12) j = 24 - j;
    if (j > 6) { j = 12 - j; neg = 1; }
    switch (j) {
    case 0: r.c[0] = 4; break;
    case 1: r.c[3] = 1; r.c[1] = 1; break;
    case 2: r.c[2] = 2; break;
    case 3: r.c[1] = 2; break;
    case 4: r.c[0] = 2; break;
    case 5: r.c[3] = 1; r.c[1] = -1; break;
    default: break;
    }
    if (neg) {
        for (b = 0; b < 8; b++) r.c[b] = -r.c[b];
    }
    return r;
}

/* tan^2(jj pi/24) = (4 - 4C) / (4 + 4C), C = cos(jj pi/12) */
static Surdus
tan2_num (
    int jj)
{
    Surdus r = cos4_surdus(jj);
    int    b;

    for (b = 0; b < 8; b++) r.c[b] = -r.c[b];
    r.c[0] += 4;
    return r;
}

static Surdus
tan2_den (
    int jj)
{
    Surdus r = cos4_surdus(jj);

    r.c[0] += 4;
    return r;
}

static int
surd_sign_or_die (
    Surdus x)
{
    s32 sg = 0;

    if (!surdus_signum(&spatium, x, &sg)) {
        printf("  FATAL: surdus refused\n");
        exit(1);
    }
    return (int)sg;
}

/* sign(a_sq * Ta / ra - b_sq * Tb / rb) for breakpoint values n0^2 T(jj)/r */
static int
cmp_values (
    long a_sq, long ra, int ja,
    long b_sq, long rb, int jb)
{
    Surdus lhs;
    Surdus rhs;
    Surdus d;

    if (!surdus_multiplica(&spatium, tan2_num(ja), tan2_den(jb), &lhs)
        || !surdus_scala(lhs, (s64)(a_sq * rb), &lhs)
        || !surdus_multiplica(&spatium, tan2_num(jb), tan2_den(ja), &rhs)
        || !surdus_scala(rhs, (s64)(b_sq * ra), &rhs)
        || !surdus_subtrahe(lhs, rhs, &d)) {
        printf("  FATAL: surdus refused\n");
        exit(1);
    }
    return surd_sign_or_die(d);
}

/* ---------- breakpoints ---------- */

#define MAX_BP 4096
#define MAX_N0 7
#define MAX_R  64

typedef struct {
    int    n0;                     /* |n0| */
    long   n0sq;
    long   r;
    int    jj;
    double val;                    /* float value, for reading only */
} Breakpoint;

static Breakpoint bp[MAX_BP];
static int        n_bp = 0;
static int        n_rank = 0;      /* distinct values */
static int        bp_rank[MAX_BP];
static int        rank_of[MAX_N0][MAX_R][12];
static double     rank_val[MAX_BP];
static int        pair_seen[MAX_N0][MAX_R];

static void
mask_numbers (
    const int *idx,
    int        n_w,
    int        mask,
    int       *n0,
    int        nv[N_AXES])
{
    int i;

    *n0 = 0;
    nv[0] = nv[1] = nv[2] = 0;
    for (i = 0; i < n_w; i++) {
        int sg = ((mask >> i) & 1) ? 1 : -1;
        int e = idx[i];

        *n0 += sg;
        nv[e / 2] += sg * ((e % 2 == 0) ? 1 : -1);
    }
}

static void
collect_breakpoints (void)
{
    int n_w;
    int i;
    int j;

    memset(pair_seen, 0, sizeof(pair_seen));
    for (n_w = 3; n_w <= 6; n_w++) {
        int combo[8];

        for (i = 0; i < n_w; i++) combo[i] = i;
        do {
            int mask;

            for (mask = 0; mask < (1 << n_w); mask++) {
                int n0;
                int nv[N_AXES];
                int r;

                mask_numbers(combo, n_w, mask, &n0, nv);
                r = nv[0]*nv[0] + nv[1]*nv[1] + nv[2]*nv[2];
                if (n0 != 0 && r > 0) {
                    pair_seen[n0 < 0 ? -n0 : n0][r] = 1;
                }
            }
        } while (next_combo(combo, n_w, 6));
    }
    n_bp = 0;
    for (i = 1; i < MAX_N0; i++) {
        for (j = 1; j < MAX_R; j++) {
            int jj;

            if (!pair_seen[i][j]) continue;
            for (jj = 1; jj <= 11; jj++) {
                double t = tan((double)jj * M_PI / 24.0);

                bp[n_bp].n0 = i;
                bp[n_bp].n0sq = (long)i * i;
                bp[n_bp].r = j;
                bp[n_bp].jj = jj;
                bp[n_bp].val = (double)(i * i) * t * t / (double)j;
                n_bp++;
            }
        }
    }
}

static void
sort_breakpoints (void)
{
    int i;
    int j;

    /* insertion sort, exact comparator */
    for (i = 1; i < n_bp; i++) {
        Breakpoint key = bp[i];

        j = i - 1;
        while (j >= 0 && cmp_values(bp[j].n0sq, bp[j].r, bp[j].jj,
                key.n0sq, key.r, key.jj) > 0) {
            bp[j + 1] = bp[j];
            j--;
        }
        bp[j + 1] = key;
    }
    n_rank = 0;
    for (i = 0; i < n_bp; i++) {
        if (i > 0 && cmp_values(bp[i - 1].n0sq, bp[i - 1].r, bp[i - 1].jj,
                bp[i].n0sq, bp[i].r, bp[i].jj) == 0) {
            bp_rank[i] = bp_rank[i - 1];
        } else {
            bp_rank[i] = n_rank;
            rank_val[n_rank] = bp[i].val;
            n_rank++;
        }
        rank_of[bp[i].n0][bp[i].r][bp[i].jj] = bp_rank[i];
    }
}

/* ---------- positions: u = tan^2 t ---------- */

typedef struct {
    int kind;                      /* 0 interval, 1 at a breakpoint, 2 t = 90 */
    int rank;                      /* interval: below rank 'rank'; point: = */
} Position;

/* sign(u - value(n0, r, jj)) */
static int
u_cmp (
    Position p,
    int      n0abs,
    int      r,
    int      jj)
{
    int rk = rank_of[n0abs][r][jj];

    if (p.kind == 1) {
        return (p.rank > rk) - (p.rank < rk);
    }
    return rk < p.rank ? 1 : -1;   /* interval 'rank': above ranks < rank */
}

/* exact cell of one mask at position p, for k; tie set */
static void
exact_cell (
    Position  p,
    int       n0,
    const int nv[N_AXES],
    int       k,
    MaskCell *out)
{
    int r = nv[0]*nv[0] + nv[1]*nv[1] + nv[2]*nv[2];
    int a0 = n0 < 0 ? -n0 : n0;
    int sec = 0;
    int tie_m = -1;
    int axes[N_AXES];
    int n_axes = 0;
    int axis_rule = N_AXES;
    int a;
    int m;

    if (p.kind == 2) {
        a0 = 0;                    /* cos t = 0: real part vanishes */
        n0 = 0;
    }
    if (r == 0) {
        int cell;

        if (n0 == 0) {
            cell = (k - 1) * N_VOR + N_AXES;      /* zero sum */
        } else {
            cell = (n0 > 0 ? 0 : k - 1) * N_VOR + N_AXES;
        }
        out->float_cell = out->exact_cell = cell;
        out->n_ties = 1;
        out->ties[0] = cell;
        return;
    }
    if (n0 == 0) {
        sec = k / 2;
        tie_m = k / 2;
    } else if (n0 > 0) {
        for (m = 1; m < k / 2; m++) {
            int sg = u_cmp(p, a0, r, m * 24 / k);

            if (sg >= 0) sec++;
            if (sg == 0) tie_m = m;
        }
    } else {
        sec = k / 2;
        for (m = k / 2 + 1; m < k; m++) {
            int sg = u_cmp(p, a0, r, (k - m) * 24 / k);

            if (sg <= 0) sec++;
            if (sg == 0) tie_m = m;
        }
    }
    {
        int mx = 0;

        for (a = 0; a < N_AXES; a++) {
            int v = nv[a] < 0 ? -nv[a] : nv[a];

            if (v > mx) mx = v;
        }
        for (a = 0; a < N_AXES; a++) {
            int v = nv[a] < 0 ? -nv[a] : nv[a];

            if (v == mx) {
                axes[n_axes++] = a;
                if (a < axis_rule) axis_rule = a;
            }
        }
    }
    out->exact_cell = sec * N_VOR + axis_rule;
    out->float_cell = out->exact_cell;
    out->n_ties = 0;
    for (a = 0; a < n_axes; a++) {
        if (tie_m >= 0) {
            out->ties[out->n_ties++] = (tie_m - 1) * N_VOR + axes[a];
        }
        out->ties[out->n_ties++] = sec * N_VOR + axes[a];
    }
}

typedef struct {
    int rule;
    int robust;
    int possible;
    int tied;
} SetVerdict;

static MaskCell g_mc[MAX_MASKS];

static SetVerdict
judge (
    Position   p,
    const int *idx,
    int        n_w)
{
    SetVerdict v;
    int        tt[MAX_MASKS];
    int        n0s[MAX_MASKS];
    int        nvs[MAX_MASKS][N_AXES];
    int        n_masks = 1 << n_w;
    int        clash = 0;
    int        m;
    int        l;
    int        t;

    memset(&v, 0, sizeof(v));
    for (m = 0; m < n_masks; m++) {
        tt[m] = popcount(m) & 1;
        mask_numbers(idx, n_w, m, &n0s[m], nvs[m]);
    }
    /* same exact point, different truth value: no tie rule separates */
    for (m = 0; m < n_masks && !clash; m++) {
        for (l = m + 1; l < n_masks && !clash; l++) {
            if (tt[m] != tt[l]
                && (p.kind == 2 || n0s[m] == n0s[l])
                && nvs[m][0] == nvs[l][0] && nvs[m][1] == nvs[l][1]
                && nvs[m][2] == nvs[l][2]) {
                clash = 1;
            }
        }
    }
    for (t = 0; t < 3; t++) {
        int undecided = 0;
        int rb;

        for (m = 0; m < n_masks; m++) {
            exact_cell(p, n0s[m], nvs[m], KS[t], &g_mc[m]);
            if (g_mc[m].n_ties > 1) v.tied = 1;
        }
        v.rule |= labels_pass(g_mc, n_masks, tt, 0);
        rb = robust_pass(g_mc, n_masks, tt);
        v.robust |= rb;
        if (!v.possible) {
            v.possible = rb || (!clash
                && possible_pass(g_mc, n_masks, tt, &undecided));
        }
        if (undecided) {
            printf("  FATAL: SAT budget\n");
            exit(1);
        }
    }
    return v;
}

/* capacity signature at a position: per N = 3..6, counts of rule,
 * robust, possible, tied */
typedef struct {
    int c[4][4];
} Signature;

static Signature
signature (
    Position p,
    int      n_hi)
{
    Signature sg;
    int       n_w;

    memset(&sg, 0, sizeof(sg));
    for (n_w = 3; n_w <= n_hi; n_w++) {
        int combo[8];
        int i;

        for (i = 0; i < n_w; i++) combo[i] = i;
        do {
            SetVerdict v = judge(p, combo, n_w);

            sg.c[n_w - 3][0] += v.rule;
            sg.c[n_w - 3][1] += v.robust;
            sg.c[n_w - 3][2] += v.possible;
            sg.c[n_w - 3][3] += v.tied;
        } while (next_combo(combo, n_w, 6));
    }
    return sg;
}

/* rank of the value n0^2 tan^2(jj pi/24) / r among the breakpoints
 * (exact comparison; -1 if no breakpoint has that value) */
static int
rank_of_value (
    long n0sq,
    long r,
    int  jj)
{
    int i;

    for (i = 0; i < n_bp; i++) {
        if (cmp_values(n0sq, r, jj, bp[i].n0sq, bp[i].r, bp[i].jj) == 0) {
            return bp_rank[i];
        }
    }
    return -1;
}

/* text 'arctan(n0 tan(j pi/24) / sqrt r)' for a rank, smallest n0 */
static void
closed_form (
    int   rank,
    char *out)
{
    int i;
    int best = -1;

    for (i = 0; i < n_bp; i++) {
        if (bp_rank[i] == rank && (best < 0 || bp[i].n0 < bp[best].n0
                || (bp[i].n0 == bp[best].n0 && bp[i].r < bp[best].r))) {
            best = i;
        }
    }
    {
        /* reduce n0^2/r: (2, 8) -> (1, 2) when the numerator stays square */
        long a = bp[best].n0sq;
        long b = bp[best].r;
        long g = a;
        long h = b;
        long n0 = bp[best].n0;
        long q;

        while (h) { long t = g % h; g = h; h = t; }
        a /= g;
        b /= g;
        for (q = 1; q * q < a; q++) { }
        if (q * q == a) {
            n0 = q;
        } else {
            b = bp[best].r;
        }
        if (b == 1) {
            sprintf(out, "arctan(%ld tan(%d pi/24))", n0, bp[best].jj);
        } else {
            sprintf(out, "arctan(%ld tan(%d pi/24)/sqrt %ld)", n0,
                bp[best].jj, b);
        }
    }
}

static double
degrees_of (
    double u)
{
    return atan(sqrt(u)) * 180.0 / M_PI;
}

/* ---------- placing an angle ---------- */

/* exact for multiples of 15 degrees: u = tan^2(jj pi/24), jj = 2..10 */
static Position
place_exact (
    int jj)
{
    Position p;
    int      lo = 0;
    int      i;

    p.kind = 0;
    p.rank = 0;
    for (i = 0; i < n_bp; i++) {
        int sg = cmp_values(1, 1, jj, bp[i].n0sq, bp[i].r, bp[i].jj);

        if (sg == 0) {
            p.kind = 1;
            p.rank = bp_rank[i];
            return p;
        }
        if (sg > 0 && bp_rank[i] + 1 > lo) lo = bp_rank[i] + 1;
    }
    p.rank = lo;
    return p;
}

/* exact for a rational u = pn/pd: sign(pn r (4 + 4C) - pd n0^2 (4 - 4C)) */
static Position
place_rational (
    long pn,
    long pd)
{
    Position p;
    int      lo = 0;
    int      i;

    p.kind = 0;
    p.rank = 0;
    for (i = 0; i < n_bp; i++) {
        Surdus lhs;
        Surdus rhs;
        Surdus d;
        int    sg;

        if (!surdus_scala(tan2_den(bp[i].jj), (s64)(pn * bp[i].r), &lhs)
            || !surdus_scala(tan2_num(bp[i].jj), (s64)(pd * bp[i].n0sq),
            &rhs) || !surdus_subtrahe(lhs, rhs, &d)) {
            printf("  FATAL: surdus refused\n");
            exit(1);
        }
        sg = surd_sign_or_die(d);
        if (sg == 0) {
            p.kind = 1;
            p.rank = bp_rank[i];
            return p;
        }
        if (sg > 0 && bp_rank[i] + 1 > lo) lo = bp_rank[i] + 1;
    }
    p.rank = lo;
    return p;
}

/* by float, certified: refuses (gap < 1e-9 relative) */
static int
place_float (
    double    degrees,
    Position *p)
{
    double u = tan(degrees * M_PI / 180.0);
    int    i;
    int    lo = 0;

    u *= u;
    for (i = 0; i < n_rank; i++) {
        if (fabs(u - rank_val[i]) < 1e-9 * u) {
            return 0;
        }
        if (u > rank_val[i]) lo = i + 1;
    }
    p->kind = 0;
    p->rank = lo;
    return 1;
}

/* ================================================================
 * Main
 * ================================================================ */

/* D97's sweep as printed (knotapel/demo_97_cell_b_perfect, run of the
 * unmodified source 2026-10-10): XOR counts N = 3..6 of 20, 15, 6, 1 */
#define N_ANG 21
static const double D97_ANG[N_ANG] = { 10, 15, 20, 25, 30, 35, 40, 42, 44,
    45, 46, 48, 50, 55, 60, 65, 70, 75, 80, 85, 90 };
static const int D97_X[N_ANG][4] = {
    { 20, 3, 0, 1 }, { 20, 15, 0, 1 }, { 20, 15, 0, 1 }, { 20, 15, 6, 1 },
    { 20, 15, 6, 1 }, { 20, 15, 0, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 1 },
    { 20, 15, 6, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 1 },
    { 20, 15, 6, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 1 },
    { 20, 15, 6, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 0 }, { 20, 0, 0, 0 },
    { 0, 0, 0, 0 } };

static Signature sig_int[MAX_BP + 1];
static Signature sig_pt[MAX_BP];
static Signature sig_90;
static int       d97_equals_rule = 1;   /* Part D */

static const Signature *
sig_at (
    Position p)
{
    return p.kind == 2 ? &sig_90 : p.kind == 1 ? &sig_pt[p.rank]
        : &sig_int[p.rank];
}

static int
same_col (
    const Signature *a,
    const Signature *b,
    int              col)
{
    int n;

    for (n = 0; n < 4; n++) {
        if (a->c[n][col] != b->c[n][col]) return 0;
    }
    return 1;
}

/* one diagram: merge consecutive positions with equal counts in 'col' */
static void
print_diagram (
    const char *title,
    int         col)
{
    /* positions in order: I0, P0, I1, P1, ..., I_R, then 90 */
    int n_pos = 2 * n_rank + 2;
    int start = 0;
    int i;

    printf("\n  --- %s (XOR sets passing, N = 3/4/5/6 of 20/15/6/1) ---\n",
        title);
    for (i = 1; i <= n_pos; i++) {
        const Signature *cur;
        const Signature *first;

        first = start == n_pos - 1 ? &sig_90 : (start % 2 == 0)
            ? &sig_int[start / 2] : &sig_pt[start / 2];
        if (i < n_pos) {
            cur = i == n_pos - 1 ? &sig_90 : (i % 2 == 0)
                ? &sig_int[i / 2] : &sig_pt[i / 2];
            if (same_col(first, cur, col)) continue;
        }
        {
            int    e = i - 1;
            double lo;
            double hi;
            char   lb;
            char   rb;

            if (start == n_pos - 1) {
                lo = 90.0; lb = '[';
            } else if (start % 2 == 0) {
                lo = start == 0 ? 0.0 : degrees_of(rank_val[start / 2 - 1]);
                lb = '(';
            } else {
                lo = degrees_of(rank_val[start / 2]); lb = '[';
            }
            if (e == n_pos - 1) {
                hi = 90.0; rb = ']';
            } else if (e % 2 == 0) {
                hi = e / 2 == n_rank ? 90.0 : degrees_of(rank_val[e / 2]);
                rb = ')';
            } else {
                hi = degrees_of(rank_val[e / 2]); rb = ']';
            }
            {
                char cf[96];

                strcpy(cf, "90");
                if (e < n_pos - 1 && (e % 2 == 1 || e / 2 < n_rank)) {
                    closed_form(e / 2, cf);       /* the region's upper end */
                }
                if (start == e && start % 2 == 1) {
                    printf("    = %9.5f deg        | %2d %2d %d %d   %s\n", lo,
                        first->c[0][col], first->c[1][col],
                        first->c[2][col], first->c[3][col], cf);
                } else {
                    printf("    %c%9.5f, %9.5f%c  | %2d %2d %d %d   to %s\n",
                        lb, lo, hi, rb, first->c[0][col], first->c[1][col],
                        first->c[2][col], first->c[3][col], cf);
                }
            }
        }
        start = i;
    }
}

int
main (void)
{
    int  sizes[10];
    int  i;
    char msg[256];

    quick = getenv("DEMO122_CELER") != NULL;
    printf("KNOTAPEL DEMO 122: Cell B's Exact Phase Diagram\n");
    printf("===============================================\n");
    {
        s32 primi[3];

        primi[0] = 2;
        primi[1] = 3;
        primi[2] = 5;
        check("surd field Q(sqrt2, sqrt3, sqrt5)",
            surdi_spatium(primi, 3, &spatium));
    }

    /* ---------- Part A: Cell B and D97's sweep ---------- */
    printf("\n=== Part A: Cell B (D96/D97) and D97's float sweep ===\n");
    build_z8_d95();
    (void)derive_structure(sizes);
    d97_cell_b();
    {
        int ok = cellb_count == 6 && cellb_n_dirs == 3;

        /* exact: a = sqrt2/2 (Zr2 (0,1)), one nonzero vector coordinate */
        for (i = 0; i < g_z8_size; i++) {
            const QZ8 *q = &g_z8[i];
            int nz;

            if (g_level[i] != 0 || g_null[i]) continue;
            nz = (q->b.a || q->b.b) + (q->c.a || q->c.b) + (q->d.a || q->d.b);
            ok &= q->a.a == 0 && (q->a.b == 1 || q->a.b == -1) && nz == 1;
        }
        check("Cell B = 6 elements (+-45 degrees about 3 orthogonal axes: "
            "a = sqrt2/2, one nonzero vector coordinate)", ok);
    }
    {
        int ok = 1;
        int ai;
        int n_hi = quick ? 3 : 6;

        printf("    angle | D97 float replica (N = 3/4/5/6)\n");
        for (ai = 0; ai < N_ANG; ai++) {
            int c[4];
            int ni;

            for (ni = 0; ni < 4; ni++) {
                c[ni] = 3 + ni <= n_hi ? d97_sweep_count(D97_ANG[ai], 3 + ni)
                    : D97_X[ai][ni];
                ok &= c[ni] == D97_X[ai][ni];
            }
            printf("    %5.1f | %2d %2d %d %d\n", D97_ANG[ai], c[0], c[1], c[2],
                c[3]);
        }
        check("D97's sweep reproduced at all 21 angles", ok);
    }

    /* ---------- Part B: breakpoints ---------- */
    printf("\n=== Part B: breakpoints u* = n0^2 tan^2(j pi/24) / r ===\n");
    collect_breakpoints();
    sort_breakpoints();
    {
        int ok = 1;

        for (i = 1; i < n_bp; i++) {
            ok &= bp[i - 1].val <= bp[i].val * (1.0 + 1e-12);
        }
        /* the rank invariant: equal values share one rank, distinct values
         * strictly increase with rank (each neighbour pair, exactly) */
        for (i = 1; i < n_bp; i++) {
            int c = cmp_values(bp[i - 1].n0sq, bp[i - 1].r, bp[i - 1].jj,
                bp[i].n0sq, bp[i].r, bp[i].jj);

            ok &= c <= 0 && ((c == 0) == (bp_rank[i] == bp_rank[i - 1]))
                && bp_rank[i] - bp_rank[i - 1] == (c == 0 ? 0 : 1);
        }
        sprintf(msg, "%d breakpoints, %d distinct values, exactly sorted "
            "(equal values share a rank; float values agree)", n_bp, n_rank);
        check(msg, ok && n_rank > 0);
    }

    /* ---------- Part C: the phase diagram ---------- */
    printf("\n=== Part C: Cell B's capacity at every half-angle ===\n");
    {
        Position p;
        int      n_hi = quick ? 3 : 6;

        for (i = 0; i <= n_rank; i++) {
            p.kind = 0;
            p.rank = i;
            sig_int[i] = signature(p, n_hi);
            if (i < n_rank) {
                p.kind = 1;
                sig_pt[i] = signature(p, n_hi);
            }
        }
        p.kind = 2;
        p.rank = 0;
        sig_90 = signature(p, n_hi);
    }
    print_diagram("exact rule", 0);
    print_diagram("robust (every tie resolution)", 1);
    print_diagram("possible (some tie resolution)", 2);

    /* ---------- Part D: D97's angles, exactly ---------- */
    printf("\n=== Part D: D97's 21 angles placed exactly ===\n");
    printf("    angle | D97 float   | exact rule  | robust      | possible\n");
    {
        int ok = 1;
        int ai;

        for (ai = 0; ai < N_ANG; ai++) {
            double   ang = D97_ANG[ai];
            Position p;
            const Signature *sg;
            int      jj = (int)(ang / 7.5 + 0.5);

            if (ang >= 90.0) {
                p.kind = 2;
                p.rank = 0;
            } else if (fabs(ang - 7.5 * (double)jj) < 1e-9 && jj % 2 == 0) {
                p = place_exact(jj);
            } else if (!place_float(ang, &p)) {
                printf("    %5.1f | too close to a breakpoint\n", ang);
                ok = 0;
                continue;
            }
            sg = sig_at(p);
            {
                int n;

                for (n = 0; n < 4; n++) {
                    d97_equals_rule &= sg->c[n][0] == D97_X[ai][n];
                }
            }
            printf("    %5.1f | %2d %2d %d %d | %2d %2d %d %d | %2d %2d %d %d | "
                "%2d %2d %d %d%s\n", ang, D97_X[ai][0], D97_X[ai][1],
                D97_X[ai][2], D97_X[ai][3], sg->c[0][0], sg->c[1][0],
                sg->c[2][0], sg->c[3][0], sg->c[0][1], sg->c[1][1],
                sg->c[2][1], sg->c[3][1], sg->c[0][2], sg->c[1][2],
                sg->c[2][2], sg->c[3][2], p.kind == 1 ? "  (ON a breakpoint)"
                : p.kind == 2 ? "  (90: every sum a tie)" : "");
        }
        check("every D97 angle placed (multiples of 15 exactly, others by a "
            "certified float gap)", ok);
    }

    if (!quick) {
        Position p;
        int      ok;
        int      r_lo = -1;
        int      r_d1 = -1;
        int      r_d2 = -1;
        int      r_hi = -1;
        int      full;

        printf("\n=== Part E: the claims, exactly ===\n");
        /* ranks of the four boundaries, located by their closed forms */
        r_lo = rank_of_value(9, 1, 1);
        r_d1 = rank_of_value(25, 1, 1);
        r_d2 = rank_of_value(9, 1, 2);
        r_hi = rank_of_value(1, 2, 11);
        /* robust 100% at every N exactly on the open intervals strictly
         * inside (lo, d1) and (d2, hi), nowhere else */
        ok = r_lo >= 0 && r_d1 > r_lo && r_d2 > r_d1 && r_hi > r_d2;
        for (i = 0; i <= n_rank && ok; i++) {
            const Signature *sg = &sig_int[i];
            int inside = (i > r_lo && i <= r_d1) || (i > r_d2 && i <= r_hi);

            full = sg->c[0][1] == 20 && sg->c[1][1] == 15 && sg->c[2][1] == 6
                && sg->c[3][1] == 1;
            ok = full == inside;
        }
        check("robust 100% at every N on exactly TWO open plateaus: "
            "(arctan(3 tan pi/24), arctan(5 tan pi/24)) = (21.552, 33.355) "
            "and (arctan(3 tan pi/12), arctan(tan(11pi/24)/sqrt2)) = (38.794, "
            "79.453) degrees - not 25..75", ok);
        ok = 1;
        for (i = r_d1 + 1; i <= r_d2; i++) {
            ok &= sig_int[i].c[2][1] == 0 && sig_int[i].c[2][2] == 0;
        }
        check("D97's '35 degree resonance' is a 5.44-degree BAND: on every "
            "interval of [33.355, 38.794] N = 5 fails robustly AND possibly",
            ok && r_d2 - r_d1 >= 1);
        p = place_exact(6);
        check("45 degrees is ON a breakpoint (u = 1) and robustly 100% - not "
            "the plateau's midpoint (59.12)", p.kind == 1
            && sig_at(p)->c[0][1] == 20 && sig_at(p)->c[1][1] == 15
            && sig_at(p)->c[2][1] == 6 && sig_at(p)->c[3][1] == 1);
        check("90 degrees: robust 0 at every N, possible 100% at every N "
            "(every sum a tie at 180 degrees)",
            sig_90.c[0][1] == 0 && sig_90.c[1][1] == 0 && sig_90.c[2][1] == 0
            && sig_90.c[3][1] == 0 && sig_90.c[0][2] == 20
            && sig_90.c[1][2] == 15 && sig_90.c[2][2] == 6
            && sig_90.c[3][2] == 1);
        ok = 1;
        for (i = 0; i <= n_rank; i++) {
            int n;

            for (n = 0; n < 4; n++) {
                ok &= sig_int[i].c[n][0] == sig_int[i].c[n][1];
            }
        }
        check("on every open interval the exact rule equals robust: the "
            "rule's tie choices matter only AT breakpoints", ok);
        check("D97's float sweep equals the exact rule at all 21 angles",
            d97_equals_rule);
        {
            /* oracle.py (cos^2 comparison in Q(sqrt2, sqrt3), no tan, no
             * breakpoints), run 2026-10-10: rule, robust, possible per
             * N = 3..6 at u = tan^2 t - constants, not a live call */
            static const long U[10][2] = { { 1, 100 }, { 1, 10 }, { 4, 25 },
                { 1, 3 }, { 49, 100 }, { 1, 1 }, { 3, 1 }, { 25, 1 },
                { 30, 1 }, { 400, 1 } };
            static const int OR[11][4][3] = {
                { { 8, 8, 8 }, { 3, 3, 15 }, { 0, 0, 0 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 0, 0, 0 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 0, 0, 0 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 0, 0, 0 } },
                { { 12, 12, 20 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
                { { 0, 0, 20 }, { 0, 0, 15 }, { 0, 0, 6 }, { 0, 0, 1 } } };
            int pt;
            int n;
            int c;

            ok = 1;
            for (pt = 0; pt < 11; pt++) {
                const Signature *sg;

                if (pt < 10) {
                    p = place_rational(U[pt][0], U[pt][1]);
                } else {
                    p.kind = 2;
                    p.rank = 0;
                }
                sg = sig_at(p);
                for (n = 0; n < 4; n++) {
                    for (c = 0; c < 3; c++) {
                        ok &= sg->c[n][c] == OR[pt][n][c];
                    }
                }
            }
            check("oracle.py's rule / robust / possible at 11 points (u = "
                "1/100 .. 400 and 90 degrees, incl. breakpoints 30, 45, 60 "
                "degrees) == the diagram", ok);
        }
        {
            /* oracle.py at the region boundaries themselves, u = A
             * tan^2(jj pi/24) / B exactly (where rule, robust, possible
             * part ways), same run */
            static const long BPT[7][3] = { { 1, 1, 1 }, { 9, 1, 1 },
                { 25, 1, 1 }, { 9, 1, 2 }, { 1, 2, 11 }, { 1, 1, 11 },
                { 8, 1, 1 } };
            static const int ORB[7][4][3] = {
                { { 8, 8, 20 }, { 3, 3, 15 }, { 0, 0, 0 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 0, 0, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 0, 0, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 0, 0, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 0, 1 } },
                { { 20, 20, 20 }, { 3, 0, 3 }, { 6, 6, 6 }, { 0, 0, 0 } },
                { { 20, 20, 20 }, { 3, 3, 15 }, { 0, 0, 0 }, { 1, 1, 1 } } };
            int pt;
            int n;
            int c;

            ok = 1;
            for (pt = 0; pt < 7; pt++) {
                int rk = rank_of_value(BPT[pt][0], BPT[pt][1],
                    (int)BPT[pt][2]);

                ok &= rk >= 0;
                if (rk < 0) continue;
                for (n = 0; n < 4; n++) {
                    for (c = 0; c < 3; c++) {
                        ok &= sig_pt[rk].c[n][c] == ORB[pt][n][c];
                    }
                }
            }
            check("oracle.py AT 7 region boundaries (7.5, 20.42, 21.55, "
                "33.36, 38.79, 79.45, 82.5 degrees; rule, robust and possible "
                "part ways there) == the diagram", ok);
        }
    }

    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
