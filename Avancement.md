# Modèle acoustique #

## Calcul des MFCC ##
### Implemention ###
  * Découpage du signal en tranche
  * Calcul du MFCC pour chacune des tranches
  * MFCC = FFT + MEL + FILTERS + DCT
| | **Codé** | **Testé** |
|:|:---------|:----------|
| FFT | ![http://www.mezimages.com/up/09/543337-button_ok.gif](http://www.mezimages.com/up/09/543337-button_ok.gif) | ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) |
| MEL | ![http://www.mezimages.com/up/09/543337-button_ok.gif](http://www.mezimages.com/up/09/543337-button_ok.gif) | ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) |
| FILTERS | ![http://www.mezimages.com/up/09/543337-button_ok.gif](http://www.mezimages.com/up/09/543337-button_ok.gif) | ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) |
| DCT | ![http://www.mezimages.com/up/09/543337-button_ok.gif](http://www.mezimages.com/up/09/543337-button_ok.gif) | ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) |

## HMM utilisant les MFCC ##
  * Un HMM par phonème
  * A partir d'une suite de coefficients MFCC, le HMM determine la probabilité d'être un phonème.
  * Etats du HMM : des coefficients MFCC
  * Transitions = proba de passage d'un coeff à l'autre

| | **Codé** | **Testé** |
|:|:---------|:----------|
| Compréhension/validation du principe (ci-dessus) | ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) | ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) |
| Determination base d'apprentissage | ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) | **![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif)**|
| [[HTK](HTK.md)] Apprentissage (détermination des transitions)| ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) | ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) |
| Application à la reconnaissance de phonèmes | ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) | ![http://www.mezimages.com/up/09/543341-button_cancel.gif](http://www.mezimages.com/up/09/543341-button_cancel.gif) |

### Corpus de sons ###
  * Utilisation du corpus SCRIBE

## HMM utilisant les Phonèmes ##
...

# Modèle linguistique #















































