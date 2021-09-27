(Pour exécuter les commandes indiquées il faut se placer à la racine du projet)

* Pour compiler le projet : .\build_release.bat

* Pour lancer le projet : .\run_release.bat

Une fois l'application lancée, différents contrôles sont proposés :
    * Rotation de la caméra :
        - Z (en azerty) ou Q (en qwerty) : rotation vers le haut
        - Q (en azerty) ou A (en qwerty) : rotation vers la gauche
        - S                              : rotation vers le bas
        - D                              : rotation vers la droite
    
    * Déplacement de la caméra (dépend de la direction dans laquelle regarde la caméra) :
        - Flèche du haut   : Aller vers l'avant
        - Flèche de gauche : Aller vers la gauche
        - Flèche du bas    : Aler vers l'arrière
        - Flèche de droite : Aller vers la droite
        - O                : Aller vers le haut
        - L                : Aller vers le bas

    * Contrôles sur la simulation :
        - R : Réinitialiser la simulation (position et vélocité de la particule, marquage de la trajectoire)
        - P : Met en pause la simulation (la particule ne se déplace plus, mais il est quand même possible de bouger la caméra)