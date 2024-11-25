// main.c
// Programme Principal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cryptage.h"

// definition pour login
#define MAX_USERS 100
#define MAX_USERNAME_LENGTH 25
#define MAX_PASSWORD_LENGTH 25
#define CSV_FILE "login.csv"

// Structure pour représenter les exercices d'association de mots
typedef struct {
    char mot1[20];
    char mot2[20];
} AssociationDeMots;

// structure pour login
typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} UserInfo;

// Structure pour représenter les exercices vrai/faux
typedef struct {
    char phrase[100];
    bool reponse;
} VraiFaux;

// Structure pour représenter les exercices de traduction
typedef struct {
    char phrase[100];
    char traduction[100];
} DemandeTraduction;

// Fonction pour vérifier si un utilisateur existe dans le fichier CSV
int userExists(const char *username, UserInfo *users, int numUsers) {
    for (int i = 0; i < numUsers; ++i) {
        if (strcmp(users[i].username, username) == 0) {
            return i;  // L'utilisateur existe, retourne l'indice dans le tableau
        }
    }
    return -1;  // L'utilisateur n'existe pas
}

char* cryptPassword(char *password) {
    char *encryptedPassword = malloc(strlen(password) + 1);
    strcpy(encryptedPassword, password);
    crypterChaine(encryptedPassword);
    return encryptedPassword;
}

// Fonction pour ajouter un nouvel utilisateur dans le fichier CSV
void addUser(const char *username, const char *password) {
    char encryptedPassword[MAX_PASSWORD_LENGTH];
    strcpy(encryptedPassword, password);
    cryptPassword(encryptedPassword); // Crypte le mot de passe
    FILE *file = fopen(CSV_FILE, "a");  // Ouvre le fichier en mode ajout
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s;%s\n", username, encryptedPassword);

    fclose(file);
}

// Fonction pour charger les utilisateurs à partir du fichier CSV
int findUser(UserInfo *users) {
    FILE *file = fopen(CSV_FILE, "r");
    if (file == NULL) {
        printf("Le fichier n'existe pas. Création du fichier...\n");
        return 0;
    }

    int numUsers = 0;
    // Vérifie si on peux lire les deux valeurs du fichier CSV
    while (fscanf(file, "%25[^;];%25[^\n]\n", users[numUsers].username, users[numUsers].password) == 2) {
        numUsers++;
        if (numUsers >= MAX_USERS) {
            break;
        }
    }

    fclose(file);
    return numUsers;
}

// Exemples de mot pour les jeux 
AssociationDeMots exercicesAssociation[4] = {{"DOG", "Chien"}, {"CAT", "Chat"}, {"HELLO", "Bonjour"}, {"APPLE", "Pomme"}};
VraiFaux exercicesVraiFaux[5] = {{"The sky is blue. = Le ciel est bleu.", true}, {"La pomme est rouge. = The coconut is red.", false}, {"Hello, how are you? = Bonjour, comment allez-vous ?", true}, {"Il pleut aujourd'hui = It's sunny today.", false}, {"I like coding = J'aime programmer.", true}};
DemandeTraduction exercicesTraduction[5] = {{"Hello", "Bonjour"}, {"Le chien est rapide.", "The dog is fast."}, {"I love programming.", "J'aime programmer."}, {"La mer est belle.", "The sea is beautiful."}, {"What is your name?", "Comment tu t'appelles ?"}};

// Fonction pour l'association de mots
void associationDeMots(int *score) {
    printf("Association de mots :\n");

    for (int i = 0; i < 4; i++) {
        printf("%d. %s - ", i + 1, exercicesAssociation[i].mot1);

        char reponse[20];
        scanf("%s", reponse);

        if (strcasecmp(reponse, exercicesAssociation[i].mot2) == 0) {
            printf("Correct!\n");
            (*score)++;
        } else {
            printf("Incorrect. La bonne réponse est : %s\n", exercicesAssociation[i].mot2);
        }
    }
}

// Fonction pour vrai/faux
void vraiFaux(int *score) {
    printf("\nVrai/Faux :\n");

    for (int i = 0; i < 5; i++) {
        printf("%d. %s\n", i + 1, exercicesVraiFaux[i].phrase);

        int choix;
        printf("Vrai (1) / Faux (0) : ");
        scanf("%d", &choix);

        if (choix == exercicesVraiFaux[i].reponse) {
            printf("Correct!\n");
            (*score)++;
        } else {
            printf("Incorrect.\n");
        }
    }
}

// Fonction pour demande de traduction
void demandeTraduction(int *score) {
    printf("\nDemande de traduction :\n");

    for (int i = 0; i < 5; i++) {
        printf("%d. Traduisez : %s\n", i + 1, exercicesTraduction[i].phrase);

        char reponse[100];
        printf("Votre réponse : ");
        scanf(" %[^\n]", reponse);

        if (strcmp(reponse, exercicesTraduction[i].traduction) == 0) {
            printf("Correct!\n");
            (*score)++;
        } else {
            printf("Incorrect. La bonne réponse est : %s\n", exercicesTraduction[i].traduction);
        }
    }
}

int main() {
    UserInfo users[MAX_USERS];
    int numUsers = findUser(users);

    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Entrez votre nom d'utilisateur : ");
    scanf("%s", username);

    int userIndex = userExists(username, users, numUsers);

    if (userIndex != -1) {
        // L'utilisateur existe
        char *encryptedPassword;
        do {
            printf("Entrez votre mot de passe : ");
            scanf("%s", password);

            encryptedPassword = cryptPassword(password);

            if (strcmp(users[userIndex].password, encryptedPassword) == 0) {
                printf("Connexion réussie. Bienvenue, %s!\n\n", username);
            } else {
                printf("Mot de passe incorrect.\n");
                free(encryptedPassword);
            }
        } while (strcmp(users[userIndex].password, encryptedPassword) != 0);

        free(encryptedPassword);  
    } else {
        // L'utilisateur n'existe pas
        printf("Utilisateur non trouvé. Création du nouvel utilisateur...\n");

        printf("Entrez votre mot de passe : ");
        scanf("%s", password);

        char *encryptedPassword = cryptPassword(password);

        addUser(username, encryptedPassword);

        printf("Nouvel utilisateur créé avec succès. Bienvenue, %s!\n\n", username);

        free(encryptedPassword);
    }


    int score = 0;

    int choix;
    do {
        printf("Score actuel : %d\n\n", score);
        
        printf("Choisissez un type d'exercice :\n");
        printf("1. Association de mots\n");
        printf("2. Vrai/Faux\n");
        printf("3. Demande de traduction\n");
        printf("4. Phrases à trous\n");
        printf("5. Phrases à thèmes\n");
        printf("6. Quitter\n");
        printf("\nVotre choix : ");
        scanf("%d", &choix);
        printf("\n");

    switch (choix) {
        case 1:
            associationDeMots(&score);
            break;
        case 2:
            vraiFaux(&score);
            break;
        case 3:
            demandeTraduction(&score);
            break;
        case 4:
            //création de deux tableaux de caractères choix et options de taille maximale 512 caractères
            char choix[512];
            char options[512];
            //création et initialisation de la variable continuer 
            int continuer = 1;
            //boucle qui permettra de relancer le programme si besoin
            while (continuer) {
                //Affichage de plusieurs phrases de présentation 
                printf("\033[1mBienvenue dans un exercice où vous serez confronté à des phrases à trous à compléter en Anglais\033[0m\n\n");
                printf("•Tapez 1, 2, 3 ou 4 selon la phrase que vous désirez :\n1. Nature\n2. Sport\n3. Cuisine\n4. Cinema\n\n");
                //Saisi de l'utilisateur d'un nombre de manière à choisir une catégorie
                scanf(" %c", &choix[0]);
                //Affichage de la saisi du client
                printf("Vous avez choisi : %c\n", choix[0]);

                //Si l'utilisateur a saisi 1, on rentre dans cette boucle
                if (choix[0] == '1') {
                    //Affichage de l'énoncé et des réponses
                    printf("Voici une phrase à trous concernant la nature. Pour le compléter, aidez-vous d'une des propositions parmi les 4 mises à disposition a, b, c, d :\n");
                    printf("I love exploring the _____ of the forest.\n\n");
                    printf("a. Depths\nb. Creatures\nc. Heights\nd. Trails\n\n");
                    //Saisi de l'utilisateur à propos de la réponse correct selon lui
                    scanf(" %c", &options[0]);
                    //Affichage de la réponse sélectionné
                    printf("Vous avez choisi la réponse : %c\n", options[0]);

                    //Si la a est choisi, on rentre dans la boucle
                    if (options[0] == 'a') {
                        //Affichage de la bonne réponse 
                        printf("La réponse est vrai\n");
                    } else {
                        //Affichage de la mauvaise réponse
                        printf("la réponse est fausse\n");
                    }
                } else if (choix[0] == '2') {
                    printf("Voici une phrase à trous concernant le sport. Pour le compléter, aidez-vous d'une des propositions parmi les 4 mises à disposition a, b, c, d :\n");
                    printf("The team won the championship due to their exceptional _____.\n\n");
                    printf("a. Dress\nb. Light\nc. Performance\nd. Dinner\n\n");
                    scanf(" %c", &options[0]);
                    printf("Vous avez choisi la réponse : %c\n", options[0]);

                    if (options[0] == 'c') {
                        printf("La réponse est vrai\n");
                    } else {
                        printf("La réponse est fausse\n");
                    }
                }else if (choix[0] == '3') {
                    printf("Voici une phrase à trous concernant la cuisine. Pour le compléter, aidez-vous d'une des propositions parmi les 4 mises à disposition a, b, c, d :\n");
                    printf("To make a delicious meal, you need fresh ingredients and proper __________.\n\n");
                    printf("a. Seasoning\nb. Music\nc. Weather\nd. Clothing\n\n");
                    scanf(" %c", &options[0]);
                    printf("Vous avez choisi la réponse : %c\n", options[0]);

                    if (options[0] == 'a') {
                        printf("La réponse est vrai\n");
                    } else {
                        printf("La réponse est fausse\n");
                    }
                }else if (choix[0] == '4') {
                    printf("Voici une phrase à trous concernant le cinéma. Pour le compléter, aidez-vous d'une des propositions parmi les 4 mises à disposition a, b, c, d :\n");
                    printf("I love going to the movie theater to watch the latest __________.\n\n");
                    printf("a. Textbooks\nb. Appliances\nc. Supplies\nd. Movies\n\n");
                    scanf(" %c", &options[0]);
                    printf("Vous avez choisi la réponse : %c\n", options[0]);

                    if (options[0] == 'd') {
                        printf("La réponse est vrai\n");
                    } else {
                        printf("La réponse est fausse\n");
                    }
                }
                //Affichage d'une phrase pour proposer de retourner dans la première boucle
                printf("Voulez-vous travailler une autre phrase à trous ? (1=oui ou bien 0=non) : ");
                //Choix de l'utilisateur selon sa saisi
                scanf("%d", &continuer);
            }

        case 5:
            //Création du tableau de caractères buf avec une capacité maximale de 512 caractères 
            char buf[512];
            //création et initialisation de la variable reprendre
            int reprendre=1;
            //boucle qui permettra de relancer le programme si besoin
            //Affichage de plusieurs phrases de présentation
            printf("\033[1mBienvenue dans un exercice où vous serez confronté à des textes en Anglais\033[0m\n\n");
            while (reprendre) {
                printf("•Tapez 1, 2, 3 ou 4 selon le texte que vous désirez :\n1. Animaux\n2. Maison\n3. Travail\n4. Loisirs\n\n");
                //Saisi de l'utilisateur d'un nombre de manière à choisir une catégorie
                scanf(" %c", &buf[0]); 
                //Affichage de la saisi du client
                printf("Vous avez choisi : %c\n", buf[0]);

                //Comparaison entre buf et 1, si les deux sont égaux on rentre dans la boucle
                if (strcmp(buf,"1")==0) {
                    //Affichage du texte sur les animaux présentée par une phrase d'ouverture
                    printf("\nVoici un texte concernant les animaux :\n");
                    printf("\nAnimals are diverse creatures in different shapes and sizes. Some, like dogs and cats, are our friendly pets. Lions, tigers, and bears live in the wild. Birds fly and sing in the sky. Animals have various homes in oceans, jungles, and deserts.\n\n");
                    //Si buf est égale à 2, on rentre dans la boucle 
                } else if (buf[0] == '2') {
                    //Affichage du texte sur la maison présentée par une phrase d'ouverture
                    printf("\nVoici un texte concernant la maison :\n");
                    printf("\nHome is where we live. It keeps us safe and cozy. Inside, we eat, sleep, and spend time with family. Rooms have beds, kitchens, and comfy sofas. Our house is a special place we call home.\n\n");
                //Si buf est égale à 3, on rentre dans la boucle
                } else if (buf[0] == '3') {
                    //Affichage du texte sur le travail présentée par une phrase d'ouverture
                    printf("\nVoici un texte concernant le travail :\n");
                    printf("\nWork is what grown-ups do to help others or to earn money. People have different jobs, like doctors, teachers, and firefighters. They work in hospitals, schools, or fire stations. Work helps make the world a better place. We can learn many new things by watching and helping others work.\n\n");
                //Si buf est égale à 4, on rentre dans la boucle
                } else if (buf[0] == '4') {
                    //Affichage du texte sur les loisirs présentée par une phrase d'ouverture
                    printf("\nVoici un texte concernant les loisirs :\n");
                    printf("\nCooking, drawing, and playing games are fun hobbies. Some people enjoy reading books or watching movies in their leisure time. Outdoor activities like hiking and swimming bring joy and excitement. Engaging in hobbies helps people relax and have fun.\n\n");
                //Si le chiffre buf est different de 1,2,3,4 on rentre dans cette boucle qui affiche que le chiffre est indisponible
                } else {
                    printf("\nCe chiffre n'est pas attribuée\n");
                }
                //Affichage d'une phrase pour proposer de retourner dans la première boucle
                printf("Voulez-vous voir un des autres textes disponibles ? (1=oui ou bien 0=non) : \n\n");
                //Choix de l'utilisateur selon sa saisi
                scanf("%d", &reprendre);
                }
            break;
        case 6:
            printf("Merci d'avoir joué !\n");
            break;
        } 
    } while (choix != 6);



    return 0;
}
