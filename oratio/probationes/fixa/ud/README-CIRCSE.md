# Summary

UD_Latin-CIRCSE is a repository of treebanks featuring Latin texts natively annotated at the CIRCSE Research Centre in Milan (https://centridiricerca.unicatt.it/circse/en.html) following the Universal Dependencies (UD) (https://universaldependencies.org) annotation scheme.
The repository includes prose and poetry texts from different periods.


# Introduction

This treebank repository is a work in progress collective endeavour. Presently, it contains the following annotated texts: Seneca *Hercules Furens*, Seneca *Agamemnon*, Tacitus *Germania*, Seneca *Oedipus*, Seneca *Phoenissae*.

#### Seneca *Hercules Furens*

*Hercules Furens* is a tragedy written by Seneca the younger in 1st century CE. The source text was received with tokenisation, and annotation with respect to lemmatisation, POS tagging, and morphological features from the Opera Latina corpus built by the [LASLA](http://web.philo.ulg.ac.be/lasla/) laboratory in Liège.
In few cases, the [received annotation](http://github.com/CIRCSE/LASLA) with regard to POS tag and morphological annotation was modified during the syntactic annotation; deviations from the received annotation are detailed in the file [SenecaYounger_HercF_LASLA_CIRCSE](https://github.com/CIRCSE/UD_Latin-CIRCSE/blob/main/documentation/SenecaYounger_HercF_LASLA_CIRCSE.md).
The syntactic annotation was performed manually at CIRCSE, and follows the UD scheme. 
The text (7714 tokens, 555 sentences) was enhanced with the annotation of the speakers to whom each sentence is attributed. This annotation, performed manually at CIRCSE, is formatted as a comment in the conllu file following the comment line reporting the text of the sentence, as exemplified in what follows:

#sent_id = Latin_SenecaYounger_HercF_poetry-1

#text = soror Tonantis hoc enim solum mihi nomen relictum est semper alienum Iouem ac templa summi uidua deserui aetheris locumque caelo pulsa paelicibus dedi tellus colenda est paelices caelum tenent

#speaker = Iuno

In cases where more than one speaker utters words in the same sentence, the indication of speakers details the distribution of tokens between the speakers, as exemplified in what follows:

#sent_id = Latin_SenecaYounger_HercF_poetry-291

#text = hic onere uacuam litori puppem applicans repetebat umbras poscit Alcides uiam cedente turba dirus exclamat Charon quo pergis audax

#speaker = Theseus (token 1-16), Charon (token 17-19)

#### Seneca *Agamemnon*

*Agamemnon* is a tragedy written by Seneca the younger in 1st century CE. The source text was received with tokenisation, and annotation with respect to lemmatisation, POS tagging, and morphological features from the Opera Latina corpus built by the  [LASLA](http://web.philo.ulg.ac.be/lasla/) laboratory in Liège.
In few cases, the [received annotation](http://github.com/CIRCSE/LASLA) with regard to POS tag and morphological annotation was modified during the syntactic annotation; deviations from the received annotation are detailed in the file [SenecaYounger_Ag_LASLA_CIRCSE](https://github.com/CIRCSE/UD_Latin-CIRCSE/blob/main/documentation/SenecaYounger_Ag_LASLA_CIRCSE.md).
The syntactic annotation was performed manually at CIRCSE, and follows the UD scheme.
The text (5580 tokens, 409 sentences) was enhanced with the annotation of the speakers to whom each sentence is attributed. This annotation, performed manually at the CIRCSE, is formatted as a comment in the conllu file following the comment line reporting the text of the sentence, as exemplified in what follows:

#sent_id = Latin_SenecaYounger_Ag_poetry-1

#text = opaca linquens Ditis inferni loca adsum profundo Tartari emissus specu incertus utras oderim sedes magis fugio Thyestes inferos superos fugo

#speaker = Thyestis umbra

In cases where more than one speaker utters words in the same sentence, the indication of speakers details the distribution of tokens between the speakers, as exemplified in what follows:

#sent_id = Latin_SenecaYounger_Ag_poetry-199

#text = sistito infestum mare uehit ista Danaos classis et Troas uehit nec plura possunt occupat uocem mare

#speaker = Danai (token 1-10), Eurybates (token 11-16)

In cases of reported speech, the character who utters the reported speech is listed as first; the character reporting the speech is enclosed in round brackets, as exemplified in what follows, where the character named Eurybates reports words uttered by the people of Danai:

#sent_id = Latin_SenecaYounger_Ag_poetry-194

#text = nil nobile ausos pontus atque undae ferunt

#speaker = Danai (Eurybates)


#### Tacitus *Germania*

*Germania* is a treatise written by Cornelius Tacitus between 1st and 2nd century CE.
The text was received with tokenisation, and annotation with respect to lemmatisation, POS tagging, and morphological features from the Opera Latina corpus built by the [LASLA](http://web.philo.ulg.ac.be/lasla/) laboratory in Liège.
The syntactic annotation was performed manually, and follows the UD scheme.
The text consists of 5674 tokens, 299 sentences.

In few cases, the sentence splitting differs from the [received one](http://github.com/CIRCSE/LASLA); deviations from the received annotation are detailed in the file [Tacitus_Ger_LASLA_CIRCSE](https://github.com/CIRCSE/UD_Latin-CIRCSE/blob/main/documentation/Tacitus_Germania_LASLA_CIRCSE.md).


#### Seneca *Oedipus*

*Oedipus* is a tragedy written by Seneca the younger in 1st century CE. The source text was received with tokenisation, and annotation with respect to lemmatisation, POS tagging, and morphological features from the Opera Latina corpus built by the  [LASLA](http://web.philo.ulg.ac.be/lasla/) laboratory in Liège.
In few cases, the [received annotation](http://github.com/CIRCSE/LASLA) with regard to POS tag and morphological annotation was modified during the syntactic annotation; deviations from the received annotation are detailed in the file [SenecaYounger_Oed_LASLA_CIRCSE](https://github.com/CIRCSE/UD_Latin-CIRCSE/blob/main/documentation/SenecaYounger_Oed_LASLA_CIRCSE.md).
The syntactic annotation was performed manually at CIRCSE, and follows the UD scheme.
The text (5931 tokens, 401 sentences) was enhanced with the annotation of the speakers to whom each sentence is attributed. This annotation, performed manually at the CIRCSE, is formatted as a comment in the conllu file following the comment line reporting the text of the sentence.

### Seneca *Phoenissae*

*Phoenissae* is a tragedy written by Seneca the younger in 1st century CE. The source text was received with tokenisation, and annotation with respect to lemmatisation, POS tagging, and morphological features from the *Opera Latina* corpus built by the [LASLA](http://web.philo.ulg.ac.be/lasla/) laboratory in Liège.
In few cases, the [received annotation](http://github.com/CIRCSE/LASLA) with regard to POS tag and morphological annotation was modified during the syntactic annotation; deviations from the received annotation are detailed in the file [SenecaYounger_Phoen_LASLA_CIRCSE](https://github.com/CIRCSE/UD_Latin-CIRCSE/blob/main/documentation/SenecaYounger_Phoen_LASLA_CIRCSE.md)).
The syntactic annotation was first performed automatically with a Stanza model retrained on UD_Latin-CIRCSE treebank v. 2.17 and then manually revised. 
The text (4155 tokens, 308 sentences) was enhanced with the annotation of the speakers to whom each sentence is attributed, following the attribution as in Zwierlein 1986 (Zwierlein, O. (1986) *L. Annaei Senecae Tragoediae, Incertorum auctorum Hercules (Oetaeus), Octauia*. Oxford: Clarendon Press). This annotation, performed manually at CIRCSE, is formatted as a comment in the conllu file following the comment line reporting the text of the sentence, as exemplified in what follows:

#sent_id = Latin_SenecaYounger_Phoen_poetry-1

#text = caeci parentis regimen et fessi unicum lateris leuamen gnata quam tanti est mihi genuisse uel sic desere infaustum patrem

#speaker = Oedipus

In cases where more than one speaker utters words in the same sentence, the indication of speakers details the distribution of tokens between the speakers, as exemplified in what follows:

#sent_id = Latin_SenecaYounger_Phoen_poetry-241 

#text = dixit inridens deus ipsum timebis

#speaker = Iocasta (token 1-3), deus (token 4-5)


The distribution of the Seneca Hercules Furens (tokens: 7714) with respect to the subsets is as follows:
* `train`: 260 sentences (Latin_SenecaYounger_HercF_poetry-1; Latin_SenecaYounger_HercF_poetry-260) - 3884 tokens
* `dev`: 46 sentences (Latin_SenecaYounger_HercF_poetry-261; Latin_SenecaYounger_HercF_poetry-306) - 781 tokens
* `test`: 249 sentences (Latin_SenecaYounger_HercF_poetry-307; Latin_SenecaYounger_HercF_poetry-555) - 3049 tokens


The distribution of the Seneca Agamemnon (tokens: 5580) with respect to the subsets is as follows:
* `train`: 188 sentences (Latin_SenecaYounger_Ag_poetry-1; Latin_SenecaYounger_Ag_poetry-188) - 2795 tokens
* `dev`: 35 sentences (Latin_SenecaYounger_Ag_poetry-189; Latin_SenecaYounger_Ag_poetry-223) - 570 tokens
* `test`: 186 sentences (Latin_SenecaYounger_Ag_poetry-224; Latin_SenecaYounger_Ag_poetry-409) - 2215 tokens


The distribution of the Tacitus Germania (tokens: 5674) with respect to the subsets is as follows:
* `train`: 149 sentences (Latin_Tacitus_Ger_prose-1; Latin_Tacitus_Ger_prose-149) - 2848 tokens
* `dev`: 31 sentences (Latin_Tacitus_Ger_prose-150; Latin_Tacitus_Ger_prose-180) - 577 tokens
* `test`: 119 sentences (Latin_Tacitus_Ger_prose-181; Latin_Tacitus_Ger_prose-299) - 2249 tokens


The distribution of the Seneca Oedipus (tokens: 5931) with respect to the subsets is as follows:
* `train`: 165 sentences (Latin_SenecaYounger_Oedip_poetry-1; Latin_SenecaYounger_Oedip_poetry-165) - 2966 tokens
* `dev`: 24 sentences (Latin_SenecaYounger_Oedip_poetry-166; Latin_SenecaYounger_Oedip_poetry-189) - 616 tokens
* `test`: 212 sentences (Latin_SenecaYounger_Oedip_poetry-190; Latin_SenecaYounger_Oedip_poetry-401) - 2349 tokens


The distribution of the Seneca Phoenissae (tokens: 4156) with respect to the subsets is as follows:
* `train`: 156 sentences (Latin_SenecaYounger_Phoen_poetry-1; Latin_SenecaYounger_Phoen_poetry-156) - 2081 tokens
* `dev`: 25 sentences (Latin_SenecaYounger_Phoen_poetry-157; Latin_SenecaYounger_Phoen_poetry-181) - 434 tokens
* `test`: 127 sentences (Latin_SenecaYounger_Phoen_poetry-182; Latin_SenecaYounger_Phoen_poetry-308) - 1641 tokens


Since the UD_Latin-CIRCSE treebank is a work in progress, its structure is subject to changes.


# Acknowledgments

The annotation of Seneca *Hercules Furens* and *Agamemnon* has been conducted in the framework of the _LiLa: Linking Latin_ project. LiLa has received funding from the European Research Council (ERC) under the European Union’s Horizon 2020 research and innovation programme – Grant Agreement No. 769994. Warmful thanks to Federica Gamba and Flavio Massimiliano Cecchini for their support and precious advices during the annotation process.  
The maintenance of the treebank benefits from the partecipation to [UniDive COST Action CA21167](https://unidive.lisn.upsaclay.fr/).


# References




# Changelog

* 2024-05-15 v2.14
  * Initial release in Universal Dependencies.


<pre>
=== Machine-readable metadata (DO NOT REMOVE!) ================================
Data available since: UD v2.14
License: CC BY-SA 4.0
Includes text: yes
Parallel: no
Genre: fiction poetry
Lemmas: converted with corrections
UPOS: converted with corrections
XPOS: converted with corrections
Features: converted with corrections
Relations: manual native
Contributors: Iurescia, Federica; Gamba, Federica; Cecchini, Flavio Massimiliano; Mambrini, Francesco; Moretti, Giovanni; Passarotti, Marco; Ruffolo, Paolo 
Contributing: here
Contact: federica.iurescia@unicatt.it
===============================================================================
</pre>
