(Pour exécuter les commandes indiquées il faut se placer dans le dossier "Code")

Prérequis :
    - Avoir MSVC
    - Avoir CMake

* Pour compiler le projet : .\build_release.bat

* Pour lancer le projet : .\run_release.bat

(Remarque : En mode Debug, il est nécessaire d'avoir installé le SDK Vulkan, pour que les validation layers soient configurées)

Une fois l'application lancée, plusieurs scènes sont proposées. Il est possible de changer de scène via la fenêtre ImGui.
Les 2 scènes créées pour la phase 3 sont "Demo 1 Phase 3" et "Demo 2 Phase 3 (car collision)"

Les contrôles sont les suivant :
    * Rotation de la caméra :
        - Z (en azerty) ou Q (en qwerty) : Rotation vers le haut
        - Q (en azerty) ou A (en qwerty) : Rotation vers la gauche
        - S                              : Rotation vers le bas
        - D                              : Rotation vers la droite
    
    * Déplacement de la caméra (dépend de la direction dans laquelle regarde la caméra) :
        - Flèche du haut                 : Aller vers l'avant
        - Flèche de gauche               : Aller vers la gauche
        - Flèche du bas                  : Aler vers l'arrière
        - Flèche de droite               : Aller vers la droite
        - A (en azerty) ou Q (en qwerty) : Aller vers le haut
        - E                              : Aller vers le bas

    * Contrôles sur la simulation :
        - R : Réinitialiser la simulation (remettre le blob à sa position initiale)
        - P : Met en pause la simulation (le blob ne bouge plus mais il est quand même possible de déplacer la caméra)

    * Déplacement du blob (dans la scène du blob) :
        - Y : Déplace le blob vers le fond de la scène (par rapport au point de vue initial de la caméra)
	- G : Déplace le blob vers la gauche de la scène (par rapport au point de vue initial de la caméra)
	- H : Déplace le blob vers l'avant de la scène (par rapport au point de vue initial de la caméra)
	- J : Déplace le blob vers la droite de la scène (par rapport au point de vue initial de la caméra)