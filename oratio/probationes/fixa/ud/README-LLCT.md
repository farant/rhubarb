# Summary

This Universal Dependencies version of the **LLCT** (Late Latin Charter Treebank) consists of an automated conversion of the **LLCT2** treebank from the Latin Dependency Treebank (LDT) format into the Universal Dependencies standard.

# Introduction

The Universal Dependencies version of the **LLCT** (Late Latin Charter Treebank) results from an automated conversion of the **LLCT2** treebank from the Latin Dependency Treebank (LDT) format into the Universal Dependencies standard. The LLCT2 is the second part of three LLCT treebanks, the first part (LLCT1) being available in LDT format (see Korkiakangas, in press) and the third part still under construction (as of 4/2020). 

The LLCT2 contains **521 Early Medieval Latin original documents (charters)** written in Tuscia (Tuscany), Italy, between **AD 774 and 897**. They all represent the **legal (documentary) genre**. Their language is a non-standard variety of Latin which differs from Classical as well as from Medieval Latin in terms of spelling, morphology, and syntax. For a general overview of the LLCT treebanks, see Korkiakangas (in press).

The original annotation follows mainly the *Guidelines for the Syntactic Annotation of Latin Treebanks (v. 1.3)* (Bamman et al., 2007). However, the non-standard features are annotated following an additional set of rules described in (Korkiakangas and Passarotti, 2011) and in (Cecchini et al., 2020).

# Sources, annotation & conversion process

The original data has been annotated automatically according to the LDT style using the first part of the LLCT treebank, i.e. the LLCT1, as the training set, and has then been manually corrected. The UD conversion has been performed automatically, based on a PERL script previously used for the Index Thomisticus treebank (https://github.com/UniversalDependencies/UD_Latin-ITTB), which has been partially rewritten (see Cecchini et al., 2018, 2020). Some minor *a posteriori* corrections have been applied to comply to the UD validation script. 

The underlying text derives from four 19th-century editions: 
1) *Memorie e documenti per servire all'istoria del Ducato di Lucca 5:2*. Ed. by D. Barsocchini. Lucca: Francesco Bertini, 1837; 
2) *Memorie e documenti per servire all'istoria del Ducato di Lucca 5:3*. Ed. by D. Barsocchini. Lucca: Francesco Bertini, 1841; 
3) *Memorie e documenti per servire all'istoria del Ducato di Lucca 4:1*. Ed. by D. Bertini. Lucca: Francesco Bertini, 1818;
4) *Memorie e documenti per servire all'istoria del Ducato di Lucca 4:2*. Ed. by D. Bertini. Lucca: Francesco Bertini, 1836. 

Their text has been manually corrected to meet modern editorial standards (see Korkiakangas, in press). In the LLCT2, each sentence receives a unique identifier that refers to its numbering in the modern volumes of the *Chartae Latinae Antiquiores* (Cavallo et al., 1997-2019). For example, the identifier `document_id='36:1047'-span='6'` means that the sentence at hand can be found in charter no. `1047` of volume no. `36`, and is the `6`th sentence thereof (in the LLCT2, sentences are bound by the punctuation marks `.`, `;` and `:`).

## Splitting of the set

The corpus has been split into **three sets** for training (`train`), development (`dev`) and testing (`test`), roughly following an **80%/10%/10%** ratio. We notice that, in the current UD release, **223 sentences have been left out** from the original LLCT2 corpus, as their conversion requires further consideration regarding how they have to be restructured into the UD standard. This means that some of the charters might display a gap (e.g., charter `36:1049` goes from sentence `30` to `32`). All sentences are in the order in which they appear in their respective charters.

A total of **242411 tokens** is thus subdivided into three sets of **194143** (`train`, 7289 sentences), **24189** (`dev`, 850) and **24079** (`test`, 884) **tokens each**, and care has been taken not to break up the same charter over different sets. 


# Acknowledgements

The conversion of the LLCT2 into the Universal Dependencies was realized by Flavio Massimiliano Cecchini and Marco Passarotti within the LiLa project (Linking Latin, https://lila-erc.eu/). The original LLCT2 was annotated by Timo Korkiakangas under projects funded by the University of Oslo and the Academy of Finland.

# References

* Bamman, D., Crane, G., Passarotti, M., and Raynaud, S.
(2007). *Guidelines for the Syntactic Annotation of Latin
Treebanks (v. 1.3)*. Tufts Published Scholarship. Tufts
University's Digital Collections and Archives, Medford, MA, USA. Permanent URL: http://hdl.handle.net/10427/42683 or https://itreebank.marginalia.it/doc/2007_Passa+Bamman+Crane+Raynaud_Guidelines%20Tb.pdf

* Cavallo, G., Nicolaj, G., et al., editors (1997-2019). *Chartae Latinae Antiquiores: facsimile edition of the Latin charters, 2nd series: ninth century*. Urs Graf Verlag, Dietikon, Zürich, Switzerland.

* Cecchini, F. M., Korkiakangas, T. and Passarotti, M. (2020). A New Latin Treebank for Universal Dependencies: Charters between Ancient Latin and Romance Languages. In *Proceedings of the Twelfth International Conference on Language Resources and Evaluation (LREC2020)*, Marseille, France, May. European Language Resources Association (ELRA).

* Cecchini, F. M., Passarotti, M., Marongiu, P., and Zeman,
D. (2018). Challenges in Converting the Index Thomisticus Treebank into Universal Dependencies. In *Proceedings of the Second Workshop on Universal Dependencies
(UDW 2018) at EMNLP 2018*, pages 27–36, Bruxelles,
Belgium. Special Interest Group on linguistic DATa and
corpus-based approaches to NLP (SIGDAT), ACL. Available at https://www.aclweb.org/anthology/W18-6004.pdf

* Gamba, F. and Zeman, D. (2023a). [Universalising Latin Universal Dependencies: a harmonisation of Latin treebanks in UD](https://aclanthology.org/2023.udw-1.2/). In *Proceedings of the Sixth Workshop on Universal Dependencies (UDW, GURT/SyntaxFest 2023)*, Washington, DC, USA, March. Association for Computational Linguistics (ACL). 

* Gamba, F. and Zeman, D. (2023b). [Latin Morphology through the Centuries: Ensuring Consistency for Better Language Processing](https://ufal.mff.cuni.cz/biblio/attachments/2023-gamba-p3787387064232511302.pdf). In *Proceedings of the Ancient Language Processing Workshop associated with the 14th International Conference on Recent Advances in Natural Language Processing RANLP 2023*, Varna, Bulgaria, September. 

* Korkiakangas, T. (in press). Late Latin Charter Treebank: contents and annotation, in *Corpora*, 16 (2021). Preprint: https://researchportal.helsinki.fi/en/publications/late-latin-charter-treebank-contents-and-annotation

* Korkiakangas, T. and Passarotti, M. (2011). [Challenges in Annotating Medieval Latin Charters](https://jlcl.org/content/2-allissues/12-Heft2-2011/16.pdf). *Journal for Language Technology and Computational Linguistics (JLCL)*, 26(2):105–116, November. In Francesco Mambrini, et al., editors, *Annotation of Corpora for Research
in the Humanities: Proceedings of the ACRH Workshop, Heidelberg, 5. Jan. 2012*.



# Changelog
* 2023-11-15 v2.13
    * Implementation of overall syntactic and morphologic harmonisation following (Gamba & Zeman 2023a) and (Gamba & Zeman 2023b)
    * Orthographic univerbation of the phrase *ne c* by means of multi-word tokens, retaining the individual analyses of its components
    * Complete re-annotation of 16 gapping (i.e. non-projective) [`fixed`]() constructions (others have been kept for the moment since they are not trivially resolved)
* 2022-11-15 v2.11
    * Implementations of the [amendment](https://universaldependencies.org/changes.html#multiple-subjects) for clausal non-verbal copular constructions and corrections regarding multiple subjects and introduction of the `:outer` subtype for subjects and copulas
    * Implementation of the proposal of [`VerbForm`](la-feat/VerbForm) reform as for (Cecchini, 2021; see documentation entry for `VerbForm`)
        * at the same time, introduction of the `TraditionalMood` and `TraditionalTense` features in `MISC` to take into account traditional denominations  
    * Double-pronoun constructions, also called free relatives, have been highlighted semi-automatically by means of the the newly introduced `:relcl` subtype
    * Implementation of the treatment of personal pronouns as `Person`-bearing determiners (relation `det`, part of speech `PRON`) of (proper) nouns, instead as of heads modified by appositions (relation `appos`), as discussed in (Höhn, 2021; UDW 2021, Sofia) 
    * Normalisation of textual gap annotation: no more pseudolemma *missing^token*, but empty lemma (`_`), empty part of speech (`X`) if not retrievable from the context, and possibly empty relation (`dep`) and convenience head attachment if not retrievable from the context
    * Some instances of the correlating construction *tam ... quam et* 'so ... as also' have been (partially semi-automatically) normalised from co-ordinating conjunctions to focaliser + comparative adverbial clause
    * Many "small words" (especially particles) and their features have started being standardised among treebanks. For example, *is*/*ea*/*id* is now always a `PRON` with `PronType=Prs`, *nam* always a `PART`, `AdvType` has been added to many "adverbs"...
        * *et*, *nec* etc. now stay `CCONJ` even when acting as focalisers
        * *unus* is uniformed to a determiner (`DET`) with both cardinal numeral type and indefinite pronoun type, and numeric value `1`
        * multiple (comma-separated) `PronType`s have been revised and reduced to one; the interrogative `Int` value has been streamlined as possible 
    * Remotion of the value `Pos` for the feature `Degree` ("positive" degree is still retrievable as the absence of another degree)
    * Various other occasional corrections of the annotation
        * The LLCT is still waiting for the introduction of inflectional classes and some other lexical features, a complex made more difficult by the particular Latin of these texts
* 2020-05-15 v2.6
  * Initial release in Universal Dependencies.

<pre>
=== Machine-readable metadata (DO NOT REMOVE!) ================================
Data available since: UD v2.6
License: CC BY-SA 4.0
Includes text: yes
Parallel: no
Genre: nonfiction legal
Lemmas: converted from manual
UPOS: converted from manual
XPOS: manual native
Features: converted from manual
Relations: converted with corrections
Contributors: Korkiakangas, Timo; Cecchini, Flavio Massimiliano; Passarotti, Marco
Contributing: elsewhere
Contact: flavio.cecchini@unicatt.it, timo.t.korkiakangas@gmail.com, marco.passarotti@unicatt.it
===============================================================================
</pre>

