### 8 et 10 octobre 2007 ###
  * Dictionnaire dict pour HVite?
  * SCRIBE avec HTK : Faire fonctionner HVite (reconnait à partir des HMM)
  * SCRIBE avec HTK : Faire fonctionner HResult (teste l'efficacité de l'apprentissage - utilise donc Hvite)
  * Qu'est ce que ça change d'utiliser l'un ou l'autre des fichiers de label ? (yen a plusieurs pour chaque fichier audio dans SCRIBE)
  * Nombre d'état du modèle de markov (quoi choisir)?

### 5 octobre 2007 ###
  * SCRIBE avec HTK : Faire fonctionner HRest (étape suivant hInit) ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif)
    * **8 octobre** <sub>a quoi ça sert exactement? Pour l'instant on est passé direct de HInit à HVite (apprentissage à reconnaissance)</sub>
  * Pres de fin de projet ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) <sub>On verra plus tard</sub>

### 3 octobre 2007 ###
  * Rien de nouveau, beaucoup de retard dans ce qui précède. ![http://www.mezimages.com/up/09/543337-button_ok.gif](http://www.mezimages.com/up/09/543337-button_ok.gif)

### 26 septembre 2007 ###
  * SCRIBE avec HTK : Faire fonctionner le module HLABEL (pour générer les MLF à partir des fichier SCRIBE ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif)
    * **3 octobre** <sub>passé toute l'aprèm à tenter de compiler. Finalement ça fonctionne (la compilationm, en utilisant le makefile qui crée les autres exe), mais ça plante. A terminer donc.</sub>
    * **5 octobre** <sub>lopen ne plante plus mais lsave plante.</sub>
    * **8 octobre** <sub>pas avancé : on arrête pour l'instant car inutile, les outils (les exe) HTK tels que HInit, utilise directement le module pour convertir des fichiers de labels au format SCRIBE (suffit de spécifier ça en option du programme)</sub>
  * SCRIBE avec HTK : Création des fichiers prototypes des modèles de markov (passés en entrée de HInit) ![http://www.mezimages.com/up/09/543337-button_ok.gif](http://www.mezimages.com/up/09/543337-button_ok.gif)
    * **5 octobre** <sub>Utilisation du fichier d'exemple de la doc htk mais incompatible avec le fichier mfc.</sub>
    * **10 octobre** <sub>En fait c'est ok (attention toutefois de ne pas oublier la première ligne un peu bizarre à première vue)</sub>
  * SCRIBE avec HTK : Faire fonctionner HInit ![http://www.mezimages.com/up/09/543337-button_ok.gif](http://www.mezimages.com/up/09/543337-button_ok.gif)
    * **5 octobre** <sub>fichier mfc créé à partir d'un wav avec Hcopy. Prob de compatibilité avec le fichier de prototype. Format SCRIBE pour les label?</sub>
    * **10 octobre** <sub>Oui pour le format SCRIBE. Fichier de prototype ok finalement. Tout fonctionne. En fait 2 manière d'utiliser l'outil : soit on fournit une liste de fichier son très court constituant chacun un exemple de la chose à apprendre (seulement la chose), soit on fournit des fichier son plus long contenant plus de choses, avec les fichiers de label associé afin que HTK retrouve les morceaux qui vont lui servir pour évaluer les coefficient du modèle de markov. On utilise la deuxième solution, avec le corpus scribe, après avoir créé le mfc à partir des SES.</sub>

### 21 septembre 2007 ###
  * MFCC : Obtenir le meme resultat que HTK : tester pour d'autres tranches ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif)
  * MFCC : Ajouter gestion de tranche de taille < 128 (avec des doubles au lieu d'entiers, et des interpolations pour les valeurs des FFT). Si si Julien... ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif)
  * SCRIBE avec HTK : Le format audio (wav sans l'entete est'il lisible par HTK)? ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) <sup>A voir</sup>
  * SCRIBE avec HTK : Format des fichiers de transcription a modifier pour HTK? ![http://www.mezimages.com/up/09/543337-button_ok.gif](http://www.mezimages.com/up/09/543337-button_ok.gif) <sup>Oui, format de SCRIBE à modifier grâce au module HLabel</sup>

### Avant ###
  * MFCC : Faire fonctionner le programme PERL ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif)  <sup>J'y arrive po</sup>
  * MFCC : Obtenir le meme resultat que le programme matlab. ![http://www.mezimages.com/up/09/543337-button_ok.gif](http://www.mezimages.com/up/09/543337-button_ok.gif)  <sup>En retirant des optimisations tels que hammings ou un log</sup>
  * MFCC : Obtenir le meme resultat que HTK ![http://www.mezimages.com/up/09/543337-button_ok.gif](http://www.mezimages.com/up/09/543337-button_ok.gif)  <sup>Pareil à priori (sur la première tranche)</sup>