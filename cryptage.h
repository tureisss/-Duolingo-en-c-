// cryptage.h
// Cryptage d'une chaîne de caractères

#ifndef CRYPTAGE_H
#define CRYPTAGE_H

#define CLE_CRYPT 10 // Clé de cryptage

void crypterChaine(char *chaine) {
    while (*chaine != '\0') {
        *chaine = *chaine + CLE_CRYPT; // On ajoute la clé de cryptage à chaque caractère, cela décale dans la table ASCII
        chaine++;
    }
}

#endif 